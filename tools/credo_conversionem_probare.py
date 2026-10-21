#!/usr/bin/env python3
"""tools/credo_conversionem_probare.py - conversionem suitae credo v1 -> v2
IUDICAT (migratio subsystematum, gradus I; ledger 01M5B3BT9R71).

Pro quaque suita (VIA data, aut suitae mutatae contra HEAD):
  I    v2: credo_suitam_currere in versione nova
  II   assertiones: macra CREDO_* statice numerata, HEAD == nova
  III  ianua subsystematis (<sub>/compile_probationes.sh, radix:
       ./compile_tests.sh) versionem novam struit et currit: exitus 0
  IV   binarium ipsum ex radice (RHUBARB_RADIX, CREDO_VERDICTA): SUITA
       TRANSIIT, sectiones k/k, Totalis et Praeteriti == versionis HEAD
  V    MEMORIA: sectio fracta genere credo_memoria -> verdictum MEMORIA
       (suita EXTRAHITUR, manu tractanda - Franus 10-20)
  VI   ordo: CREDO_ORDO=mixtus:1..N (ordinarium III) - SUITA TRANSIIT,
       Totalis idem (status communis inter sectiones occultus)
  VII  examen: ./silva/examen.sh -> ACCIPE

Versio HEAD per ianuam EANDEM struitur: textus HEAD in locum ponitur,
struitur et curritur, deinde versio nova SEMPER restituitur (finally;
scriptura plana + tactus, ut aedificatores mtime videant - numquam cp -p).
Copia novae in build/credo_conversio/<suita>.nova.c. Ianuae filtrum per
SUBSTRINGAM legunt (probatio_gesta tres suitas struit) - nihil refert:
binarium suitae ipsum curritur, structum post initium cursus.

CAUTIO: arborem VIVAM tangit (textus HEAD ad tempus) - numquam dum portae
commissionis currunt (arbor congelata), numquam duo simul in arbore una.

Usus (ex radice):
  ./tools/credo_conversionem_probare.py [-ordo N] [VIA...]
Effusio: TSV, linea per suitam: verdictum via assertiones totalis
sectiones ordo nota. Exitus 0 omnes BONUM | 1 aliqua MALUM aut MEMORIA |
2 usus / nihil iudicandum."""
import os
import re
import subprocess
import sys
import tempfile
import time

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
COPIAE = os.path.join(RADIX, 'build', 'credo_conversio')
FORMA_ASSERTIONIS = re.compile(
    r'\bCREDO_(?!SECTIO\b|GRADUS\b|H\b|MORA_ORDINARIA_MS\b)[A-Z_0-9]+\s*\(')
SUITA_VIA = re.compile(r'^(?:([a-z_]+)/)?probationes/(probatio_[a-z_0-9]+)\.c$')
TEMPUS_CURSUS = 900


def _assertiones(textus):
    return len(FORMA_ASSERTIONIS.findall(textus))


def _numerus(effusio, clavis):
    """numerus post clavem in occursu ULTIMO (puncta progressus lineam
    eandem praecedunt - numquam '^')."""
    inventa = re.findall(re.escape(clavis) + r'\s+(\d+)', effusio)
    return int(inventa[-1]) if inventa else None


def _locus(via):
    m = SUITA_VIA.match(via)
    if not m:
        return None
    subsystema, suita = m.group(1) or 'radix', m.group(2)
    if subsystema == 'radix':
        ianua = ['./compile_tests.sh', suita]
        binaria = ['build/probationes/' + suita]
    else:
        ianua_via = subsystema + '/compile_probationes.sh'
        if not os.path.exists(os.path.join(RADIX, ianua_via)):
            return None
        textus = open(os.path.join(RADIX, ianua_via), errors='replace').read()
        if 'fabrica probare' in textus:
            ianua = ['./' + ianua_via, '-vis', suita]
        else:
            ianua = ['./' + ianua_via, suita]
        binaria = [subsystema + '/build/' + suita,
                   'build/fabrica/area/probationes_%s/%s/%s'
                   % (subsystema, suita, suita)]
    return subsystema, suita, ianua, binaria


def _struere(ianua, binaria):
    """ianua currit; binarium suitae (structum post initium) redditur."""
    initium = time.time() - 1
    r = subprocess.run(ianua, cwd=RADIX, capture_output=True, text=True,
                       errors='replace', timeout=TEMPUS_CURSUS * 2)
    recentia = [b for b in binaria
                if os.path.exists(os.path.join(RADIX, b))
                and os.path.getmtime(os.path.join(RADIX, b)) >= initium]
    recentia.sort(key=lambda b: os.path.getmtime(os.path.join(RADIX, b)))
    return r.returncode, (recentia[-1] if recentia else None), \
        r.stdout + r.stderr


def _currere(binarium, ordo=None):
    """binarium ex radice; numeri ex compendio, verdicta ex TSV (v2)."""
    descriptor, verdicta = tempfile.mkstemp(prefix='credo_conversio.', suffix='.tsv')
    os.close(descriptor)
    os.unlink(verdicta)
    ambitus = dict(os.environ, RHUBARB_RADIX=RADIX, CREDO_VERDICTA=verdicta)
    ambitus.pop('CREDO_SECTIO', None)
    if ordo is not None:
        ambitus['CREDO_ORDO'] = 'mixtus:%d' % ordo
    else:
        ambitus.pop('CREDO_ORDO', None)
    try:
        r = subprocess.run([os.path.join(RADIX, binarium)], cwd=RADIX,
                           env=ambitus, capture_output=True, text=True,
                           errors='replace', timeout=TEMPUS_CURSUS)
        codex, effusio = r.returncode, r.stdout + r.stderr
    except subprocess.TimeoutExpired:
        codex, effusio = 'TEMPUS', ''
    fructus = {'codex': codex,
               'totalis': _numerus(effusio, 'Totalis:'),
               'praeteriti': _numerus(effusio, 'Praeteriti:'),
               'suita': None, 'sectiones': None, 'memoria': []}
    if os.path.exists(verdicta):
        for linea in open(verdicta, errors='replace'):
            campi = linea.rstrip('\n').split('\t')
            if campi[0] == 'SUITA' and len(campi) >= 5:
                fructus['suita'] = campi[2]
                fructus['sectiones'] = (int(campi[3]), int(campi[4]))
            elif (campi[0] == 'SECTIO' and len(campi) > 8
                  and campi[8] == 'credo_memoria'):
                fructus['memoria'].append(campi[2])
        os.unlink(verdicta)
    return fructus


def _iudicare(via, ordines):
    locus = _locus(via)
    if locus is None:
        return 'MALUM', ['via non suita subsystematis']
    subsystema, suita, ianua, binaria = locus
    nova = open(os.path.join(RADIX, via), errors='replace').read()
    r = subprocess.run(['git', 'show', 'HEAD:' + via], cwd=RADIX,
                       capture_output=True, text=True, errors='replace')
    vetus = r.stdout if r.returncode == 0 else None
    nota, campi = [], {}

    # I, II: statice
    if 'credo_suitam_currere' not in nova:
        nota.append('non v2')
    a1 = _assertiones(nova)
    a0 = _assertiones(vetus) if vetus is not None else None
    campi['assertiones'] = '%s>%d' % (a0, a1)
    if a0 is not None and a0 != a1:
        nota.append('assertiones %d->%d' % (a0, a1))

    # versio HEAD per ianuam eandem (textus ad tempus in loco)
    ante = None
    if vetus is not None and vetus != nova:
        os.makedirs(COPIAE, exist_ok=True)
        with open(os.path.join(COPIAE, suita + '.nova.c'), 'w') as f:
            f.write(nova)
        try:
            with open(os.path.join(RADIX, via), 'w') as f:
                f.write(vetus)
            rc0, binarium0, _ = _struere(ianua, binaria)
            if binarium0 is not None:
                ante = _currere(binarium0)
            else:
                nota.append('ANTE non structa (ianua exitus %s)' % rc0)
        finally:
            with open(os.path.join(RADIX, via), 'w') as f:
                f.write(nova)
            os.utime(os.path.join(RADIX, via), None)
        if ante is not None and ante['codex'] != 0:
            nota.append('ANTE exitus %s' % ante['codex'])

    # III: versio nova per ianuam
    rc1, binarium1, effusio1 = _struere(ianua, binaria)
    if rc1 != 0:
        nota.append('ianua exitus %s' % rc1)
    if binarium1 is None:
        nota.append('binarium non structum')
        campi.update(totalis='-', sectiones='-', ordo='-')
        return 'MALUM', nota, campi

    # IV, V: cursus ipse
    post = _currere(binarium1)
    t0 = ante['totalis'] if ante else None
    campi['totalis'] = '%s>%s' % (t0, post['totalis'])
    campi['sectiones'] = ('%d/%d' % post['sectiones']
                          if post['sectiones'] else '-')
    if post['codex'] != 0:
        nota.append('exitus %s' % post['codex'])
    if post['suita'] != 'TRANSIIT':
        nota.append('SUITA %s' % post['suita'])
    if post['sectiones'] and post['sectiones'][0] != post['sectiones'][1]:
        nota.append('sectiones %d/%d' % post['sectiones'])
    if ante and ante['codex'] == 0:
        for clavis in ('totalis', 'praeteriti'):
            if ante[clavis] != post[clavis]:
                nota.append('%s %s->%s' % (clavis, ante[clavis],
                                           post[clavis]))
    memoria = list(post['memoria'])

    # VI: ordo mixtus
    fracti = []
    for semen in range(1, ordines + 1):
        mixtus = _currere(binarium1, semen)
        memoria += [s for s in mixtus['memoria'] if s not in memoria]
        if (mixtus['codex'] != 0 or mixtus['suita'] != 'TRANSIIT'
                or mixtus['totalis'] != post['totalis']):
            fracti.append(str(semen))
    campi['ordo'] = ('sanus %d' % ordines if not fracti
                     else 'FRACTUS semine ' + ','.join(fracti))
    if fracti:
        nota.append('ordo mixtus fractus (semina %s)' % ','.join(fracti))

    # VII: examen
    ex = subprocess.run(['./silva/examen.sh', via], cwd=RADIX,
                        capture_output=True, text=True, errors='replace')
    if 'verdictum: ACCIPE' not in ex.stdout + ex.stderr:
        nota.append('examen REICE')

    if memoria:
        nota.insert(0, 'credo_memoria in ' + ', '.join(memoria))
        return 'MEMORIA', nota, campi
    return ('BONUM' if not [n for n in nota if not n.startswith('ANTE')]
            else 'MALUM'), nota, campi


def main(argv):
    ordines, viae = 3, []
    i = 0
    while i < len(argv):
        if argv[i] == '-ordo' and i + 1 < len(argv) and argv[i + 1].isdigit():
            ordines = int(argv[i + 1])
            i += 2
        elif argv[i].startswith('-'):
            sys.stderr.write(__doc__)
            return 2
        else:
            viae.append(os.path.relpath(os.path.abspath(argv[i]), RADIX))
            i += 1
    if not viae:
        r = subprocess.run(['git', 'diff', '--name-only', 'HEAD'], cwd=RADIX,
                           capture_output=True, text=True)
        viae = [v for v in r.stdout.split() if SUITA_VIA.match(v)]
    if not viae:
        sys.stderr.write('credo_conversionem_probare: nihil iudicandum\n')
        return 2
    print('verdictum\tvia\tassertiones\ttotalis\tsectiones\tordo\tnota')
    malae = 0
    for via in viae:
        fructus = _iudicare(via, ordines)
        verdictum, nota = fructus[0], fructus[1]
        campi = fructus[2] if len(fructus) > 2 else {}
        if verdictum != 'BONUM':
            malae += 1
        print('\t'.join([verdictum, via,
                         campi.get('assertiones', '-'),
                         campi.get('totalis', '-'),
                         campi.get('sectiones', '-'),
                         campi.get('ordo', '-'),
                         '; '.join(nota)]), flush=True)
    return 1 if malae else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
