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
# Usus: python3 -I expat_aurum.py MANIFESTUM > aurum.tsv
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


def main():
    domus = os.path.expanduser('~')
    print('# aurum_silvestre.tsv - expat et libxml2 super silvestria '
          '(xml/instrumenta/expat_aurum.py)')
    for linea in open(sys.argv[1], encoding='utf-8'):
        if linea.startswith('#'):
            continue
        sigillum, via = linea.rstrip('\n').split('\t')
        if via.startswith('~'):
            via = domus + via[1:]
        try:
            octeti = open(via, 'rb').read()
        except OSError:
            continue
        if hashlib.sha256(octeti).hexdigest() != sigillum:
            continue
        bene, forma = expat_canonicum.canonicum(octeti, True)
        canon = (hashlib.sha256(forma.encode('utf-8', 'surrogatepass'))
                 .hexdigest() if bene else '-')
        lint = subprocess.run(['xmllint', '--noout', '--nonet', via],
                              stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL).returncode == 0
        print('%s\t%s\t%s\t%s' % (sigillum, 'V' if bene else 'I', canon,
                                  'V' if lint else 'I'))


if __name__ == '__main__':
    main()
