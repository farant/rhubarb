#!/usr/bin/env python3
"""inventarium_suitarum.py - derivatio inventarii 'suitae probationum'.

Proponit (non scribit): ordines = scripta forma cursoris in arbore
(compile_*.sh, *fumus.sh, *probare.sh, *_porta.sh), et cellas DERIVATAS
lentium 'in PORTAE' (ita-non) et 'porta' (textus: nomen portae in
pythonica/silva.py PORTAE). Effusio JSON ad instrumentum tabularii
'inventarium' mittenda (actus ordines, deinde cellae cum fons
'derivatum'). Spec: project-specs/inventarium-spec.md §6.

Modus 'currit' (portae debitae T3) proponit cellas lentis 'currit
binaria': quae probationes suitae CURRUNT (graphus inclusionum eas non
videt). Vocantes system/processus_* per ./silva/nexus.sh; litterae
chordarum per lexemata silvae (./silva/arbor.sh -nudum: commenta absunt,
macrones expansi); retinentur solae quarum verbum primum scriptum in
arbore EXSISTENS nominat, aut programma bin/ vel */build/ (binaria
probationum suitae ipsius omissa). Binarium ad fontem introitus per
nomen (<nomen>_principale.c aut <nomen>.c) mappatur; ambigua aut ignota
in stderr pro homine nominantur.

Usus: python3 tools/inventarium_suitarum.py [ordines|cellae|currit]
"""
import html
import os
import json
import re
import subprocess
import sys

FORMA = re.compile(r'(compile_(tests|probationes)|probare|_porta|fumus)\.sh$')


def ordines():
    viae = subprocess.run(['git', 'ls-files'], capture_output=True,
                          text=True, check=True).stdout.split()
    return sorted(v for v in viae if FORMA.search(v))


def portae():
    """via scripti -> nomen portae, ex PORTAE in pythonica/silva.py"""
    s = open('pythonica/silva.py').read()
    i = s.index('PORTAE = {')
    j = s.index('\n}\n', i)
    tabula = {}
    for m in re.finditer(r"^    '([a-z_-]+)': \(\[\s*'\./([^']+)'", s[i:j],
                         re.M):
        tabula.setdefault(m.group(2), []).append(m.group(1))
    return tabula


def cellae():
    p = portae()
    fructus = []
    for via in ordines():
        nomina = p.get(via, [])
        fructus.append({'ordo': via, 'lens': 'in PORTAE',
                        'valor': {'genus': 'ita-non',
                                  'valor': 'ita' if nomina else 'non'}})
        if nomina:
            fructus.append({'ordo': via, 'lens': 'porta',
                            'valor': {'genus': 'textus',
                                      'valor': ', '.join(nomina)}})
        # lens complementaria NON APPLICABILIS (decretum ...SD7JR):
        # porta registrata causam extra PORTAE non habet, et scriptum
        # extra PORTAE nomen portae non habet
        fructus.append({'ordo': via,
                        'lens': 'cur extra PORTAE' if nomina else 'porta',
                        'valor': {'genus': 'non-applicabile',
                                  'valor': 'in PORTAE' if nomina
                                  else 'extra PORTAE'}})
    return fructus


VOCATI = ('system', 'processus_exsequi', 'processus_incipere')
LITTERA = re.compile(r'<lex-string-lit[^>]*>(?:<<[^>]*>>)?'
                     r'"((?:[^"\\]|\\.)*)"')
BINARIUM = re.compile(r'^(bin/[a-z_]+|[a-z_]+/build/[a-z_]+)$')


def _vocantes():
    """plagulae probationum quae system/processus_* vocant (nexus)"""
    plagulae = set()
    for v in VOCATI:
        eff = subprocess.run(['./silva/nexus.sh', v], capture_output=True,
                             text=True).stdout
        for m in re.finditer(r'^\s+(\S+) \(\d+\):', eff, re.M):
            if re.search(r'probationes/probatio_[^/]+\.c$', m.group(1)):
                plagulae.add(m.group(1))
    return sorted(plagulae)


def _introitus(binarium, fontes):
    """binarium -> fons introitus per nomen, aut None (ambiguum/ignotum)"""
    b = os.path.basename(binarium)
    c = [f for f in fontes if '/probationes/' not in f and (
        f.endswith('/%s_principale.c' % b) or f.endswith('/%s.c' % b))]
    return c[0] if len(c) == 1 else None


def currit():
    sys.path.insert(0, 'pythonica')
    import silva
    viae = set(subprocess.run(['git', 'ls-files'], capture_output=True,
                              text=True, check=True).stdout.split())
    fontes = sorted(v for v in viae if v.endswith('.c'))
    # probatio -> ordo (cursor suitae) per SUITAE et lentem 'porta'
    porta_ordinis = {}
    for o, nomina in portae().items():
        for p in nomina:
            porta_ordinis[p] = o
    per_ordinem = {}
    for plagula in _vocantes():
        suita = [p for p, (d, _) in silva.SUITAE.items()
                 if os.path.dirname(plagula) == d]
        if not suita or suita[0] not in porta_ordinis:
            print('ignotum: %s nullius suitae' % plagula, file=sys.stderr)
            continue
        ordo = porta_ordinis[suita[0]]
        stml = subprocess.run(['./silva/arbor.sh', plagula, '-nudum'],
                              capture_output=True, text=True).stdout
        for m in LITTERA.finditer(stml):
            verba = html.unescape(m.group(1)).split()
            if not verba:
                continue
            v = verba[0][2:] if verba[0].startswith('./') else verba[0]
            if v in viae and v.endswith('.sh'):
                per_ordinem.setdefault(ordo, set()).add(v)
            elif BINARIUM.match(v) and 'probatio' not in v:
                e = _introitus(v, fontes)
                if e:
                    per_ordinem.setdefault(ordo, set()).add(e)
                else:
                    print('homini: %s currit %s (fons introitus ignotus)'
                          % (plagula, v), file=sys.stderr)
    return [{'ordo': o, 'lens': 'currit binaria',
             'valor': {'genus': 'textus', 'valor': ', '.join(sorted(b))}}
            for o, b in sorted(per_ordinem.items())]


if __name__ == '__main__':
    modus = sys.argv[1] if len(sys.argv) > 1 else 'ordines'
    fructus = (ordines() if modus == 'ordines' else
               currit() if modus == 'currit' else cellae())
    print(json.dumps(fructus, ensure_ascii=False, separators=(',', ':')))
