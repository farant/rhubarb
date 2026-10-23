"""probatio_communia.py - praefatio communis probationum pythonicae
(probatio_silva.py, probatio_silva_arboris.py): ambitus isolatus (fabrica
ficta, tempora et annales in T), greges (cursus pythonica#<grex>), credo,
abortum nominatum. Importatur per 'from probatio_communia import *' -
__all__ nomina omnia praeter dunder fert (etiam _grex, _GREX).
SIGNUM = signum portae in abortu (pythonica / pythonica-arboris)."""
import glob
import os
import re
import subprocess
import sys
import time

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SIGNUM = 'PYTHONICA'
sys.path.insert(0, os.path.join(RADIX, 'pythonica'))
import silva  # noqa: E402

# umbra translationis (T4) clausa per probationes: portae fictae
# annotationem <porta> non habent (bin/fabrica portae eas recusaret);
# sectio 'translatio receptorum, umbra' eam ipsa aperit
os.environ['PORTAE_TRANSLATIO_UMBRA'] = '0'

T = os.path.join(RADIX, 'build', 'pythonica')
os.makedirs(T, exist_ok=True)
fracta = []


def _fabrica_ficta(rc, effusio='', via_argv=None):
    """bin/fabrica fictus: argv (si via_argv) scribit, effusionem et
    exitum datum reddit - commissiones probationum hermeticae manent"""
    via = os.path.join(T, 'fabrica_ficta_%d' % rc)
    with open(via, 'w') as f:
        f.write('#!/bin/bash\n')
        if via_argv:
            f.write('printf "%%s\\n" "$@" > %s\n' % via_argv)
        f.write('printf %%s %s\n' % repr(effusio).replace('\\n', '\n')
                .replace('\\t', '\t'))
        f.write('exit %d\n' % rc)
    os.chmod(via, 0o755)
    return via


# omnes commissiones probationum sub fabrica ficta sana (exitus 0).
# 'portae ...' (debita, ordo, translatio) contractu VACUO respondet: debita
# computabilia et nulla - olim lineam NOTA ubique reddebat, computatio
# debitorum frangebatur et commissio 'petitae solae' tacite pergebat
# (parcum …CYF3PX: nunc recusatio)
def _fabrica_ficta_ordinaria():
    via = os.path.join(T, 'fabrica_ficta_ordinaria')
    with open(via, 'w') as f:
        f.write('#!/bin/bash\n'
                'if [ "$1" = portae ]; then exit 0; fi\n'
                "printf 'NOTA\\tnulla artificia generata a viis tacta\\n'\n"
                'exit 0\n')
    os.chmod(via, 0o755)
    return via


silva.FABRICA_BIN = _fabrica_ficta_ordinaria()

# tempora portarum et phasium (parcum fabricae …AR15): in T, numquam in
# build/portae/tempora.tsv vivum (portae fictae ibi mentirentur)
# (env: operarii umbrae, processus filii, silvam de novo important)
_TEMPORA_VERA = os.path.join(silva.PORTAE_DIR, 'tempora.tsv')
_tempora_vera_ante = (os.path.getsize(_TEMPORA_VERA)
                      if os.path.exists(_TEMPORA_VERA) else -1)
silva.TEMPORA_VIA = os.path.join(T, 'tempora.tsv')
os.environ['SILVA_TEMPORA_VIA'] = silva.TEMPORA_VIA
if os.path.exists(silva.TEMPORA_VIA):
    os.unlink(silva.TEMPORA_VIA)

# GREGES (analytica T6): tempus per gregem huius suitae in annalibus
# VERIS - destinationes verae ANTE isolationem servantur (commissiones
# fictae infra in T manent, greges ipsi cursus veri sunt)
_GREGES_AMBITUS = dict((k, os.environ.get(k)) for k in (
    'MENSOR_DIARIUM', 'MENSOR_VOLUMEN', 'MENSOR_ACTA', 'MENSOR_DIURNA'))

# annales cursuum (analytica T3): in T, numquam in ~/.rhubarb vivo -
# portae et commissiones fictae ibi historiam veram mentirentur
for _k, _v in (('MENSOR_DIARIUM', 'cursus.diarium'),
               ('MENSOR_VOLUMEN', 'cursus.volumen'),
               ('MENSOR_ACTA', 'cursus_acta'),
               ('MENSOR_DIURNA', 'cursus_diurna')):
    os.environ[_k] = os.path.join(T, _v)
# curatio separata (analytica T6c) NON in suita: commissiones fictae ut
# cursus summi finiunt - proles detracta diarium T dum probationes legunt
# conderet (fluxa)
os.environ['MENSOR_CURARE_NULLUS'] = '1'
# ambitus cursuum IN INITIO suitae (porta vera parentem legitime
# hereditat): post cursus omnes idem esse debet. Comparatio 'ante
# sectionem' caeca erat - commissiones priores iam effluxerant (planta
# Y2 T3 superfuit)
_CLAVES_CURSUS = ('MENSOR_PARENS', 'MENSOR_SESSIO', 'MENSOR_COMMISSUM',
                  'MENSOR_SORDIDUM', 'MENSOR_RAMUS', 'MENSOR_ARBOR',
                  'MENSOR_MACHINA', 'MENSOR_VOCATOR')
_AMBITUS_CURSUS_INITIO = dict((k, os.environ.get(k)) for k in _CLAVES_CURSUS)
for _f in glob.glob(os.path.join(T, 'cursus.diarium*')) + glob.glob(
        os.path.join(T, 'cursus.volumen*')):
    os.unlink(_f)


# grex apertus: id cursus, initium (monotonum), fracta ante
_GREX = {'id': None, 'initium': 0.0, 'fracta': 0}


def _grex_ambitus():
    """ambitus pro cursibus gregum: destinationes VERAE et contextus
    initii suitae (parens = porta vera, si sub porta)"""
    env = dict(os.environ)
    for k, v in list(_GREGES_AMBITUS.items()) + list(
            _AMBITUS_CURSUS_INITIO.items()):
        if v is None:
            env.pop(k, None)
        else:
            env[k] = v
    return env


def _grex_claudere():
    """gregem apertum finire: exitus 1 si probatio ulla intra eum fracta"""
    if _GREX['id'] is None:
        return
    n = len(fracta) - _GREX['fracta']
    subprocess.run([silva.MENSOR_BIN, 'cursus', 'finire', '-id', _GREX['id'],
                    '-exitus', '1' if n else '0', '-duratio_ms',
                    str(int((time.monotonic() - _GREX['initium']) * 1000)),
                    '-nota', '%d fracta' % n], env=_grex_ambitus(),
                   capture_output=True)
    _GREX['id'] = None


def _grex(nomen):
    """caput gregis (olim print('--- nomen ---')) + cursus
    'pythonica#nomen': prior clauditur, novus aperitur. Ruina media =
    grex INTERRUPTUS (finis numquam scriptus) - verum."""
    print('--- %s ---' % nomen)
    _grex_claudere()
    if os.environ.get('CURSUS_NULLUS') or not os.access(silva.MENSOR_BIN,
                                                        os.X_OK):
        return
    r = subprocess.run([silva.MENSOR_BIN, 'cursus', 'incipere', '-titulus',
                        'pythonica#' + nomen.replace('\t', ' ')],
                       env=_grex_ambitus(), capture_output=True, text=True,
                       errors='replace')
    for l in r.stdout.splitlines():
        if l.startswith('MENSOR_PARENS='):
            _GREX['id'] = l.split('=', 1)[1]
            _GREX['initium'] = time.monotonic()
            _GREX['fracta'] = len(fracta)


def _cursus_lineae():
    """lineae 'CURSUS' bin/mensor (columnae 0-14: CURSUS id parens titulus
    status exitus duratio commissum sordidum ramus arbor vocator acta argv
    nota)"""
    r = subprocess.run([silva.MENSOR_BIN, 'cursus', 'legere', '-machina'],
                       capture_output=True, text=True, errors='replace')
    return [l.split('\t') for l in r.stdout.splitlines()
            if l.startswith('CURSUS\t')]


def _tempora_lineae():
    if not os.path.exists(silva.TEMPORA_VIA):
        return []
    return [l.split('\t') for l in open(silva.TEMPORA_VIA).read().splitlines()]


def credo(cond, titulus, causa=None):
    """causa: textus (e.g. stderr instrumenti) in fractura impressus -
    remedium quod instrumentum nominat non in actis sepelitur"""
    print(('  ok   ' if cond else '  FRACTUM ') + titulus)
    if not cond:
        fracta.append(titulus)
        if causa:
            for linea in str(causa).strip().splitlines()[-5:]:
                print('      causa: ' + linea)


def _abortum(genus, ex, tb):
    """exceptio non capta = ABORTUM NOMINATUM: sine hoc signum portae
    ('PYTHONICA: sana|FRACTA') aberat ('signum absens'), causa in
    actis sepulta, et probationes sequentes tacite non currebant
    (2026-10-02: oraculum oratio rancidum -> IndexError)."""
    import traceback
    traceback.print_exception(genus, ex, tb)
    principale = os.path.abspath(sys.argv[0])
    lineae = [f.lineno for f in traceback.extract_tb(tb)
              if os.path.abspath(f.filename) == principale]
    print('\n%s: FRACTA %d + ABORTUM (%s: %s, %s:%s)'
          ' - probationes sequentes NON cursae'
          % (SIGNUM, len(fracta), genus.__name__, ex,
             os.path.basename(principale), lineae[-1] if lineae else '?'))


sys.excepthook = _abortum

# fixtura actorum radicis (greges fracturae et probatio_currere)
ACTA_RADIX = ('Testing: probatio_x\n----\n.F\n FRACTA (credo_verum): a == b at probationes/probatio_x.c:12\n'
              '  Speratus: 1\n  Receptus: 2\n\n=== CREDO COMPENDIUM ===\nTotalis:    3\nPraeteriti: 2\n'
              'Fracti:     1\n\nConditio: FRACTA\n✗ TEST FAILED: probatio_x (.2s)\n'
              'Testing: probatio_y\n...\nTotalis:    2\nConditio: OMNIA PRAETERIERUNT\n'
              '✓ TEST PASSED: probatio_y (.1s)\nTests Passed: 1\nTests Failed: 1\n')

__all__ = [_n for _n in list(globals()) if not _n.startswith('__')]
