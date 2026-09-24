#!/usr/bin/env python3
"""inventarium_suitarum.py - derivatio inventarii 'suitae probationum'.

Proponit (non scribit): ordines = scripta forma cursoris in arbore
(compile_*.sh, *fumus.sh, *probare.sh, *_porta.sh), et cellas DERIVATAS
lentium 'in PORTAE' (ita-non) et 'porta' (textus: nomen portae in
pythonica/silva.py PORTAE). Effusio JSON ad instrumentum tabularii
'inventarium' mittenda (actus ordines, deinde cellae cum fons
'derivatum'). Spec: project-specs/inventarium-spec.md §6.

Usus: python3 tools/inventarium_suitarum.py [ordines|cellae]
"""
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


if __name__ == '__main__':
    modus = sys.argv[1] if len(sys.argv) > 1 else 'ordines'
    print(json.dumps(ordines() if modus == 'ordines' else cellae(),
                     ensure_ascii=False, separators=(',', ':')))
