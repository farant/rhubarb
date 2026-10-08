#!/usr/bin/env python3
"""aemulator_esctest_causae.py - tabulam esctest cum causis pinnare.

Ex build/aemulator_esctest/eventus.tsv (status per probationem) et
casibus (--test-case-dir: octeti probationis) causam omnis fracti
attribuit et probationes/fixa/aemulator/esctest.tsv scribit. Vocatur per
./tools/aemulator_esctest.sh -pinnare (aemulator-plan D7a; olim in
scriptis temporariis sessionum).

Causa = VERDICTUM: proprietas:
  POSTEA   - decisio XXVI: VT420 et ultra, postea non numquam
  CONSULTO - consulto aliter (Ghostty, securitas, xterm proprium)
  LACUNA   - proprietas quae deest et habenda est
Nulla fracta sine causa: '?' exitum 1 reddit.

Usus: python3 tools/aemulator_esctest_causae.py
"""
import collections
import re
import sys

AEDIFICIUM = 'build/aemulator_esctest'
FIXA = 'probationes/fixa/aemulator/esctest.tsv'
EFFUGIUM = r'\x1b'
CSI = EFFUGIUM + r'\['

POSTEA = 'POSTEA (decisio XXVI): '
CONSULTO = 'CONSULTO: '
LACUNA = 'LACUNA: '

# ordo valet: prima forma quae in octetis casus invenitur causam dat
CAUSAE = [
    (POSTEA + 'margines-lr DECLRMM/DECSLRM (VT420)',
     CSI + r'\?69[hl]|' + CSI + r'\d+;\d*s|' + CSI + r';\d+s'),
    (POSTEA + 'DECSCL gradus conformitatis', CSI + r'[\d;]*"p'),
    (LACUNA + 'protectio DECSCA/SPA/DECSED/DECSEL (Ghostty habet)',
     EFFUGIUM + r'[VW]|' + CSI + r'\d*"q|' + CSI + r'\?\d*[JK]'),
    (POSTEA + 'rectangula VT420 DECFRA/DECCRA/DECERA/DECSERA',
     CSI + r'[\d;]*\$[xvz{rt]'),
    (POSTEA + 'columnae VT420 DECBI/DECFI/DECIC/DECDC',
     EFFUGIUM + r'[69]|' + CSI + r"[\d;]*'[}~]"),
    (CONSULTO + 'DECRQM modus non honoratus: 0 ut Ghostty (xterm IV '
     'permanenter remotum aut modum habet)', CSI + r'\??[\d;]*\$p'),
    ('DECRQSS', EFFUGIUM + r'P\$q'),
    (CONSULTO + 'colores speciales OSC 5/105 (xterm; Ghostty nihil agit)',
     EFFUGIUM + r'\](5|105)[;\x07\x1b]'),
    (CONSULTO + 'colores: spatia CIE/TekHVC et rgbi: (Ghostty quoque '
     'caret)', r'CIE|TekHVC|rgbi:'),
    (LACUNA + 'colores OSC 4/10-19/104-119',
     EFFUGIUM + r'\](4|1\d|104|11\d);'),
    (LACUNA + 'OSC 52 delectus', EFFUGIUM + r'\]52;'),
    ('XTWINOPS', CSI + r'(?!18t)[\d;]+t|' + CSI + r'>[\d;]*t'),
    (LACUNA + 'XTSAVE/XTRESTORE CSI ? s/r', CSI + r'\?[\d;]+[sr]'),
    (CONSULTO + 'DECCOLM latitudinem non mutat: delet, regionem et '
     'cursorem restituit, hospes fenestram regit (D7c)',
     CSI + r'\?(3|40)[hl]'),
    (CONSULTO + 'MoreFix 41 (xterm curses)', CSI + r'\?41[hl]'),
    (LACUNA + 'DECID (ESC Z = DA1)', EFFUGIUM + r'Z'),
    ('DECDSR', CSI + r'\?[\d;]+n'),
]

# DECDSR per nomen: status apparatus VT420+ postea, ceteri responsa fixa
DECDSR_POSTEA = ('DECCKSR', 'DECMSR', 'DSRDataIntegrity',
                 'DSRMultipleSessionStatus')


def xtwinops(titulus):
    """XTWINOPS per nomen probationis."""
    si_acervus = 'Push' in titulus or 'Pop' in titulus
    if si_acervus:
        return (CONSULTO + 'acervus titulorum 22t/23t habemus (D7a), sed '
                'esctest eum per 21t legit - titulum reddere consulto '
                'recusamus')
    if 'ReportIconLabel' in titulus or 'ReportWindowLabel' in titulus:
        return (CONSULTO + 'XTWINOPS titulum reddere 20t/21t (securitas: '
                'titulus ut initus redit)')
    if 'ResetTitleMode' in titulus:
        return CONSULTO + 'modi tituli xterm (CSI > t)'
    return (CONSULTO + 'XTWINOPS fenestram movere/mutare (programma '
            'fenestram non regit, ut Ghostty)')


# DECRQSS: SGR, DECSTBM, DECSCUSR habemus (D7a); cetera VT420
def decrqss(titulus):
    """DECRQSS per nomen probationis."""
    return (POSTEA + 'DECRQSS status VT420 (DECSACE, DECSASD, DECSLPP, '
            'DECSNLS, DECSSDT)')


def decdsr(titulus):
    """DECDSR per nomen probationis."""
    if titulus.endswith('DECXCPR'):
        return LACUNA + 'DECXCPR CSI ? 6 n (positio cum pagina)'
    if titulus.split('test_DECDSR_')[-1] in DECDSR_POSTEA:
        return POSTEA + 'DECDSR status VT420 (summa, macro, sessiones)'
    return (LACUNA + 'DECDSR status apparatus (responsa fixa: typographum, '
            'UDK, claviatura, locator)')


def causam_invenire(titulus, octeti):
    """Causa fracti unius."""
    # casus solum mandatum primum capit: OSC 105 post OSC 4 invisibile
    if titulus.startswith('ResetSpecialColorTests.'):
        return CAUSAE[7][0]
    if titulus.split('.')[0] in ('DATests', 'DA2Tests'):
        return (CONSULTO + 'identitas Ghostty (decisio V): DA1 ?62;22 / '
                'DA2 >1;0;0 - esctest identitatem xterm exspectat')
    for causa, forma in CAUSAE:
        if re.search(forma, octeti):
            if causa == 'XTWINOPS':
                return xtwinops(titulus)
            if causa == 'DECDSR':
                return decdsr(titulus)
            if causa == 'DECRQSS':
                return decrqss(titulus)
            return causa
    if titulus.startswith('XtermWinopsTests.'):
        return xtwinops(titulus)
    return '?'


def principale():
    eventus = dict(linea.rstrip('\n').split('\t')
                   for linea in open(AEDIFICIUM + '/eventus.tsv'))
    numeri = collections.Counter()
    ordines = []
    for titulus, status in sorted(eventus.items()):
        causa = ''
        if status == 'fractum':
            try:
                octeti = open(AEDIFICIUM + '/casus/' + titulus + '.txt',
                              'rb').read().decode('latin-1')
            except FileNotFoundError:
                octeti = ''
            causa = causam_invenire(titulus, octeti)
            numeri[causa] += 1
        if status == 'notum':
            causa = 'esctest knownBug (xterm)'
        if status == 'omissum':
            causa = 'VT V (--max-vt-level=4)'
        ordines.append((titulus, status, causa))
    caput = (
        '# esctest2 @ 2798f12 intra aemulatorem (tools/aemulator_esctest.sh '
        '-probare;\n'
        '# pinnatur per -pinnare -> tools/aemulator_esctest_causae.py)\n'
        '# forma: nomen<TAB>status<TAB>causa - status: transiit|fractum|'
        'omissum|notum\n'
        '# Optiones: --expected-terminal=xterm --xterm-checksum=334 '
        '--max-vt-level=4\n'
        '# --xterm-reverse-wrap=383\n'
        '# Causa fracti = VERDICTUM: proprietas. POSTEA (decisio XXVI: VT420\n'
        '# et ultra), CONSULTO (aliter consulto), LACUNA (habenda). Nulla\n'
        '# fracta sine causa. Mutatio status -probare frangit: promove.\n')
    with open(FIXA, 'w') as scriptum:
        scriptum.write(caput)
        for ordo in ordines:
            scriptum.write('%s\t%s\t%s\n' % ordo)
    for causa, numerus in numeri.most_common():
        print('%4d %s' % (numerus, causa))
    if numeri['?']:
        print('aemulator_esctest_causae: %d fracta sine causa'
              % numeri['?'], file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(principale())
