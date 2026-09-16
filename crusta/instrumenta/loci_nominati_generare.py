#!/usr/bin/env python3
"""loci_nominati_generare.py - tabula LOCI_NOMINATI portae registri

Ex crusta/grammatica/crusta.registrum.stml ordines tabulae
LOCI_NOMINATI[] (probatio_crusta_registrum.c) et GENERA_EXSPECTATA[]
imprimit: enumeratio crusta_registrum.h contra titulos declarationis,
CL ordines. Genera quae enumerationem communem habent (PRAEFIXA infra)
seorsum enumerantur. Usus:

    ./crusta/instrumenta/loci_nominati_generare.py            # ordines
    ./crusta/instrumenta/loci_nominati_generare.py -genera    # tituli

Effusio in probationem manu transcribitur (locus generis novi:
declaratio + enumeratio + hic + canon + sigillum).
"""
import re
import sys

RADIX = __file__.rsplit('/crusta/', 1)[0]
DECLARATIO = RADIX + '/crusta/grammatica/crusta.registrum.stml'

PRAEFIXA = {
    'pars-litteralis': 'PARS', 'pars-effugium': 'PARS',
    'pars-continuatio': 'PARS', 'pars-simplex': 'PARS',
    'pars-effugia': 'PARS', 'pars-domus': 'PARS',
    'pars-gemina': 'GEMINA', 'pars-versa': 'GEMINA',
    'pars-parametrum': 'PARAMETRUM', 'pars-expansio': 'EXPANSIO',
    'pars-substitutio': 'SUBSTITUTIO', 'pars-processus': 'SUBSTITUTIO',
    'pars-arithmetica': 'PARS_ARITHMETICA',
    'iudicium-praeposita': 'PRAEPOSITA', 'iudicium-binaria': 'BINARIA',
    'iudicium-coniuncta': 'BINARIA', 'iudicium-inclusa': 'INCLUSA',
}


def genera():
    textus = open(DECLARATIO).read()
    exitus = []
    for m in re.finditer(r'<genus titulus="([^"]+)"(.*?)</genus>', textus,
                         re.S):
        loci = re.findall(r'<locus titulus="([^"]+)"', m.group(2))
        exitus.append((m.group(1), loci))
    return exitus


def principale():
    gg = genera()
    if '-genera' in sys.argv:
        print(',\n'.join('    "%s"' % t for t, _ in gg))
        return
    ordines = []
    for titulus, loci in gg:
        genus = 'CRUSTA_GENUS_' + titulus.upper().replace('-', '_')
        praefixum = PRAEFIXA.get(titulus, titulus.upper())
        for locus in loci:
            ordines.append('    { (s32)%s,\n        (i32)CRUSTA_%s_%s, "%s" },'
                           % (genus, praefixum, locus.upper(), locus))
    ordines[-1] = ordines[-1].rstrip(',')
    print('\n'.join(ordines))
    sys.stderr.write('genera %d, loci %d\n'
                     % (len(gg), sum(len(l) for _, l in gg)))


if __name__ == '__main__':
    principale()
