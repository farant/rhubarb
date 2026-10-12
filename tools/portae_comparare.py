#!/usr/bin/env python3
"""portae_comparare.py - COMPARATIO TEMPORARIA portarum debitarum (plan
portae-declaratae T4; delenda in T5 per principium …Z2Y555).

Responsum VETUS (silva.portae_debitae: inventarium tabularii +
clausurae) contra NOVUM (bin/fabrica portae tactus -omnes: annotationes
<porta> + derivatio in C) super viam OMNEM arboris, via per viam.
Effusio: summarium per portam (soli veteri / soli novo), deinde viae.

Usus: python3 tools/portae_comparare.py [-viae] [relatio.txt]
"""
import collections
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'pythonica'))
import silva  # noqa: E402


def novum(tabula):
    """{via: {(porta, manu)}} ex lineis TANGIT"""
    r = subprocess.run(['bin/fabrica', 'portae', 'tactus', '-machina',
                        '-omnes', '-portae', tabula],
                       capture_output=True, text=True)
    if r.returncode != 0:
        sys.exit('novum fractum (rc=%d): %s%s' % (r.returncode, r.stdout,
                                                  r.stderr))
    fructus = collections.defaultdict(set)
    for linea in r.stdout.splitlines():
        f = linea.split('\t')
        if f[0] == 'TANGIT':
            fructus[f[1]].add((f[2], f[3] == '1'))
    return fructus


def vetus(viae, nomina):
    """{via: {(porta, manu)}} per silva.portae_debitae, via per viam;
    inventarium et sigillum arboris semel (cursus unus, arbor gelata)"""
    inv = silva.inventarium()
    sig = silva.sigillum_arboris()
    silva.inventarium = lambda res=None: inv
    silva.sigillum_arboris = lambda: sig
    fructus = {}
    for i, v in enumerate(viae):
        debita, _ = silva.portae_debitae([v])
        fructus[v] = set((nomina.get(d.porta, d.porta), d.manu)
                         for d in debita)
        if i % 1000 == 0:
            print('  vetus %d/%d' % (i, len(viae)), file=sys.stderr)
    return fructus


def main():
    viae_scribere = '-viae' in sys.argv
    argumenta = [a for a in sys.argv[1:] if a != '-viae']
    viae = subprocess.run(['git', 'ls-files'], capture_output=True,
                          text=True, check=True).stdout.split()
    with tempfile.NamedTemporaryFile('w', suffix='.tsv', delete=False) as t:
        t.write(silva.portae_tabula())
        tabula = t.name
    # porta manu veteris = ordo tabularii (via cursoris, argumenta
    # dempta); nova = titulus si registrata
    nomina = {}
    for linea in subprocess.run(
            ['bin/fabrica', 'portae', 'declaratae', '-machina', '-portae',
             tabula], capture_output=True, text=True).stdout.splitlines():
        f = linea.split('\t')
        if f[0] == 'PORTA' and f[2] == '1':
            nomina[f[3]] = f[1]
    n = novum(tabula)
    os.unlink(tabula)
    o = vetus(viae, nomina)
    soli_veteri = collections.defaultdict(list)
    soli_novo = collections.defaultdict(list)
    for v in viae:
        a, b = o.get(v, set()), n.get(v, set())
        for p in a - b:
            soli_veteri[p].append(v)
        for p in b - a:
            soli_novo[p].append(v)
    lineae = ['COMPARATIO: %d viae; differentiae %d (soli veteri %d, soli'
              ' novo %d)' % (len(viae), sum(map(len, soli_veteri.values()))
                             + sum(map(len, soli_novo.values())),
                             sum(map(len, soli_veteri.values())),
                             sum(map(len, soli_novo.values())))]
    for titulus, d in (('SOLI VETERI', soli_veteri), ('SOLI NOVO', soli_novo)):
        lineae.append(titulus + ':')
        for (p, m), vs in sorted(d.items(), key=lambda x: -len(x[1])):
            lineae.append('  %-28s %s %5d  e.g. %s' % (
                p, 'M' if m else ' ', len(vs), ', '.join(vs[:3])))
            if viae_scribere:
                lineae += ['      ' + v for v in vs]
    textus = '\n'.join(lineae) + '\n'
    if argumenta:
        open(argumenta[0], 'w').write(textus)
    sys.stdout.write(textus)


if __name__ == '__main__':
    main()
