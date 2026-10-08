"""pythonica/profilare.py - ubi tempus portae pythonica consumitur
(fabrica plan 5 T9).

cProfile super pythonica/probatio_silva.py et tempus muri per mandatum
filii: subprocess.Popen involvitur (argv[0], argv[1], ab initio ad
wait/communicate, sedes vocantis in probatio_silva.py et functio
silva.py proxima).

Usus:  python3 -B pythonica/profilare.py <praefixum_exitus>
Effusio: <praefixum>.prof (cProfile; pstats legit) et
<praefixum>.filii.tsv (programma, argumentum primum, secundae, sedes
probationis, functio silvae). Exitus = exitus probationum.
"""
import cProfile
import os
import runpy
import subprocess
import sys
import time
import traceback

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROBATIONES = os.path.join(RADIX, 'pythonica', 'probatio_silva.py')

filii = []
_PopenVerum = subprocess.Popen


def _sedes(cauda):
    """prima linea in acervo cuius plagula cum cauda desinit"""
    for gradus in reversed(traceback.extract_stack()[:-2]):
        if gradus.filename.endswith(cauda):
            return gradus
    return None


class PopenMensuratum(_PopenVerum):
    def __init__(self, argumenta, *reliqua, **optiones):
        self._initium = time.time()
        if isinstance(argumenta, (list, tuple)):
            self._verba = [str(v) for v in argumenta[:2]]
        else:
            self._verba = [str(argumenta).split(' ')[0], 'sh -c']
        probatio = _sedes('probatio_silva.py')
        self._probatio = 'probatio:%d' % probatio.lineno if probatio else ''
        silva = None
        for gradus in reversed(traceback.extract_stack()[:-1]):
            if (gradus.filename.endswith('silva.py')
                    and not gradus.filename.endswith('probatio_silva.py')):
                silva = gradus
                break
        self._silva = silva.name if silva else ''
        self._notatum = False
        super().__init__(argumenta, *reliqua, **optiones)

    def _notare(self):
        if not self._notatum:
            self._notatum = True
            filii.append((self._verba[0],
                          self._verba[1] if len(self._verba) > 1 else '',
                          time.time() - self._initium, self._probatio,
                          self._silva))

    def wait(self, *reliqua, **optiones):
        responsum = super().wait(*reliqua, **optiones)
        self._notare()
        return responsum

    def communicate(self, *reliqua, **optiones):
        responsum = super().communicate(*reliqua, **optiones)
        self._notare()
        return responsum


def principale():
    if len(sys.argv) != 2:
        print('usus: python3 -B pythonica/profilare.py <praefixum>',
              file=sys.stderr)
        return 2
    praefixum = sys.argv[1]
    os.chdir(RADIX)
    sys.path.insert(0, os.path.join(RADIX, 'pythonica'))
    sys.argv = [PROBATIONES]
    subprocess.Popen = PopenMensuratum
    profilum = cProfile.Profile()
    initium = time.time()
    exitus = 0
    try:
        profilum.runcall(runpy.run_path, PROBATIONES, run_name='__main__')
    except SystemExit as e:
        exitus = e.code or 0
    finally:
        profilum.dump_stats(praefixum + '.prof')
        with open(praefixum + '.filii.tsv', 'w') as plagula:
            for linea in filii:
                plagula.write('%s\t%s\t%.3f\t%s\t%s\n' % linea)
        print('PROFILUM: %.1f s, exitus %s, filii %d'
              % (time.time() - initium, exitus, len(filii)), file=sys.stderr)
    return exitus


if __name__ == '__main__':
    sys.exit(principale())
