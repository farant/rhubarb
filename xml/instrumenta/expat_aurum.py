# xml/instrumenta/expat_aurum.py - aurum oraculorum differentialium
# super corpus silvestre (planum xml-arbor P12).
#
# Pro omni plagula manifesti (silvestria.manifestum) cuius sigillum
# congruit: verdictum expat (spatiis solutis, entitatibus externis
# numquam lectis) et sigillum sha256 formae canonicae (si accepta),
# verdictum libxml2 (xmllint --noout --nonet). Plagulae mutatae aut
# absentes OMITTUNTUR (porta eas numerat). Exitus DETERMINISTICUS
# (sine die): actio fabricae regenerat, vestigium portae stabile.
#
# Linea: sha256_plagulae<TAB>V|I<TAB>sha256_canonici|-<TAB>V|I
#
# PARALLELUM (budget B2 X1, 2026-11-02): olim seriatim ~XC-CXIX s per
# regenerationem (actio IV-VI vicibus per fusionem): xmllint processus
# per plagulam (~XII ms x ~VII CMXXX) et forma canonica expat in
# Pythone (~XL s). Nunc expat per processus (ProcessPoolExecutor;
# filii -I -B parentis hereditant - nulla __pycache__) et xmllint per
# fila (exspectatio processus GIL solvit), simul. Lineae ORDINE
# MANIFESTI - octeti idem (probatum contra versionem seriatam: VII
# CMI lineae octetim aequales).
#
# Usus: python3 -I expat_aurum.py MANIFESTUM > aurum.tsv
import concurrent.futures
import hashlib
import importlib.util
import os
import subprocess
import sys

_s = importlib.util.spec_from_file_location(
    'expat_canonicum',
    os.path.join(os.path.dirname(os.path.abspath(__file__)),
                 'expat_canonicum.py'))
expat_canonicum = importlib.util.module_from_spec(_s)
_s.loader.exec_module(expat_canonicum)


def _xmllint(via):
    return subprocess.run(['xmllint', '--noout', '--nonet', via],
                          stdout=subprocess.DEVNULL,
                          stderr=subprocess.DEVNULL).returncode == 0


def _plagula(par):
    """(sigillum, via) -> (bene, canon), aut None si plagula absens aut
    sigillum mutatum (omittitur)"""
    sigillum, via = par
    try:
        octeti = open(via, 'rb').read()
    except OSError:
        return None
    if hashlib.sha256(octeti).hexdigest() != sigillum:
        return None
    bene, forma = expat_canonicum.canonicum(octeti, True)
    canon = (hashlib.sha256(forma.encode('utf-8', 'surrogatepass'))
             .hexdigest() if bene else '-')
    return bene, canon


def main():
    domus = os.path.expanduser('~')
    print('# aurum_silvestre.tsv - expat et libxml2 super silvestria '
          '(xml/instrumenta/expat_aurum.py)')
    paria = []
    for linea in open(sys.argv[1], encoding='utf-8'):
        if linea.startswith('#'):
            continue
        sigillum, via = linea.rstrip('\n').split('\t')
        if via.startswith('~'):
            via = domus + via[1:]
        paria.append((sigillum, via))
    fila = os.cpu_count() or 4
    with concurrent.futures.ThreadPoolExecutor(max_workers=fila) as lint, \
            concurrent.futures.ProcessPoolExecutor(max_workers=fila) as pp:
        # xmllint pro omni plagula (absentes/mutatae infra omittuntur;
        # xmllint super eas innocuus, exitus neglectus)
        lintae = [lint.submit(_xmllint, via) for _, via in paria]
        for (sigillum, _), res, l in zip(
                paria, pp.map(_plagula, paria, chunksize=64), lintae):
            if res is None:
                continue
            bene, canon = res
            print('%s\t%s\t%s\t%s' % (sigillum, 'V' if bene else 'I', canon,
                                      'V' if l.result() else 'I'))


if __name__ == '__main__':
    main()
