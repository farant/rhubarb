#!/bin/bash
# tools/codificator_a64_oraculum.sh - aurum codificatoris A64 ex clang
#
# Legit probationes/fixa/codificator_a64/semen.tsv (manu scriptum),
# addit lineas generatas (bitmask: 5334 exemplaria LXIV; FMOV: CCLVI
# imm8), textum per
#     clang -cc1as -triple arm64-apple-macosx -filetype asm -show-encoding
# mittit et aurum.tsv scribit. Verba conferuntur, numquam textus
# disassemblatus. Lineae "!causa" (FALSUM exspectatum) clang non
# videt.
#
# Portae hoc NUMQUAM vocant (aurum commissum est; Xcode non
# requiritur). Usus: post semen mutatum aut clang renovatum; iteratio
# aurum octetim idem reddit (cmp).
set -euo pipefail
cd "$(dirname "$0")/.."
exec python3 -I - <<'PY'
import os, subprocess, sys, tempfile

D = 'probationes/fixa/codificator_a64'

lineae = []
for linea in open(D + '/semen.tsv', encoding='utf-8'):
    linea = linea.rstrip('\n')
    if not linea or linea.startswith('#'):
        continue
    campi = linea.split('\t')
    if len(campi) != 10:
        sys.exit('semen: linea mala: %r' % linea)
    lineae.append(campi)
semen_numerus = len(lineae)

def larva(n):
    return (1 << n) - 1

# DecodeBitMasks, forma canonica (immr < magnitudo elementi)
def bitmask(n, immr, imms):
    clavis = (n << 6) | (~imms & 0x3f)
    if clavis == 0:
        return None
    longitudo = clavis.bit_length() - 1
    if longitudo < 1:
        return None
    amplitudo = 1 << longitudo
    unae = (imms & (amplitudo - 1)) + 1
    if unae == amplitudo or immr >= amplitudo:
        return None
    e = larva(unae)
    if immr:
        e = ((e >> immr) | (e << (amplitudo - immr))) & larva(amplitudo)
    k = amplitudo
    while k < 64:
        e |= e << k
        k *= 2
    return e

for n in (0, 1):
    for immr in range(64):
        for imms in range(64):
            e = bitmask(n, immr, imms)
            if e is not None:
                lineae.append(['AND_IMM', '64', '0', '1', '0', '0',
                               '0x%x' % e, '0', '-',
                               'and x0, x1, #0x%x' % e])

# VFPExpandImm
def fmov(imm8):
    b = (imm8 >> 6) & 1
    cd = (imm8 >> 4) & 3
    exponens = cd - 3 if b else cd + 1
    v = (16 + (imm8 & 0xf)) / 16.0 * (2.0 ** exponens)
    return -v if imm8 & 0x80 else v

for i in range(256):
    lineae.append(['FMOV_IMM', '64', '0', '0', '0', '0', str(i), '0',
                   '-', 'fmov d0, #%r' % fmov(i)])

textus = [c[9] for c in lineae if not c[9].startswith('!')]
with tempfile.TemporaryDirectory() as t:
    via = os.path.join(t, 'aurum.s')
    with open(via, 'w') as f:
        f.write(''.join('\t%s\n' % x for x in textus))
    fructus = subprocess.run(
        ['clang', '-cc1as', '-triple', 'arm64-apple-macosx',
         '-filetype', 'asm', '-show-encoding', '-o', '-', via],
        capture_output=True, text=True)
if fructus.returncode != 0:
    sys.exit('clang: ' + fructus.stderr)

verba = []
for l in fructus.stdout.splitlines():
    if '; encoding: [' not in l:
        continue
    o = l.split('; encoding: [')[1].split(']')[0].split(',')
    if len(o) != 4:
        sys.exit('encoding non IV octetorum: %r' % l)
    verba.append(int(o[0], 16) | int(o[1], 16) << 8
                 | int(o[2], 16) << 16 | int(o[3], 16) << 24)
if len(verba) != len(textus):
    sys.exit('clang %d verba pro %d instructionibus'
             % (len(verba), len(textus)))

exitus = [
    '# aurum codificatoris A64 - NOLI MANU MUTARE',
    '# ex semen.tsv (%d) + bitmask + FMOV per'
    ' tools/codificator_a64_oraculum.sh' % semen_numerus,
    '# verba: clang -cc1as -show-encoding; FALSUM = codificare refutat',
    '# forma\tlatitudo\trd\trn\trm\tra\timmediatum\ttranslatio'
    '\tconditio\tverbum\ttextus',
]
it = iter(verba)
refutandae = 0
for c in lineae:
    if c[9].startswith('!'):
        verbum = 'FALSUM'
        refutandae += 1
    else:
        verbum = '0x%08x' % next(it)
    exitus.append('\t'.join(c[:9] + [verbum, c[9]]))
with open(D + '/aurum.tsv', 'w', encoding='utf-8') as f:
    f.write('\n'.join(exitus) + '\n')
print('aurum: %d lineae (%d verba, %d refutandae)'
      % (len(lineae), len(verba), refutandae))
PY
