#!/bin/bash
# silva/sample.sh - profilum parsurae per 'sample' (macOS, sine radice)
#
# usus: ./silva/sample.sh <plagula.c> [-iter N] [-secunda S] [-nudum]
#
# computus -iter N sub sample S secundorum; imprimit tabulam foliorum
# (functiones summae acervi, ordine ponderis) et scribit
#   build/sample/<nomen>.txt      effusum sample crudum (arbor vocationum)
#   build/sample/<nomen>.folded   acervi plicati 'a;b;c numerus' (flamma)
# Methodus: ceterae mensurae (computus, mensor) dicunt QUANTUM;
# haec dicit UBI. Ordinarius: iter C, secunda V (plagula parva: -iter CCC).
set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"
via=""; iter=100; secunda=5; nudum=""
while [ $# -gt 0 ]; do
    case "$1" in
        -iter) iter="$2"; shift 2 ;;
        -secunda) secunda="$2"; shift 2 ;;
        -nudum) nudum="-nudum"; shift ;;
        *) via="$1"; shift ;;
    esac
done
[ -n "$via" ] || { echo "usus: sample.sh <plagula.c> [-iter N] [-secunda S] [-nudum]" >&2; exit 2; }
[ -f "$via" ] || { echo "sample: plagula absens: $via" >&2; exit 2; }
# computus aedificatum (involucrum reaedificat si fontes recentiores)
./silva/computus.sh "$via" -machina $nudum >/dev/null 2>&1 || { echo "sample: computus fractus super $via" >&2; exit 1; }
mkdir -p build/sample
nomen="$(basename "$via" .c)"
crudum="build/sample/$nomen.txt"
silva/build/computus "$via" -iter "$iter" $nudum >/dev/null 2>&1 &
pid=$!
sleep 0.3
sample "$pid" "$secunda" -mayDie -file "$crudum" >/dev/null 2>&1
wait "$pid"
[ -s "$crudum" ] || { echo "sample: nihil captum (processus ante sample finitus? -iter maius da)" >&2; exit 1; }
python3 - "$crudum" "build/sample/$nomen.folded" <<'PY'
import sys, re
crudum, plicatum = sys.argv[1], sys.argv[2]
t = open(crudum, errors='replace').read()
# I. tabula foliorum
m = re.search(r'Sort by top of stack.*?\n(.*?)\n\n', t, re.S)
folia = []
if m:
    for l in m.group(1).splitlines():
        mm = re.match(r'\s*(\S+)\s+\(in ([^)]+)\)\s+(\d+)', l)
        if mm: folia.append((int(mm.group(3)), mm.group(1), mm.group(2)))
summa = sum(n for n, _, _ in folia) or 1
print('folia (summa acervi), %d specimina:' % summa)
for n, f, lib in folia[:25]:
    print('  %5d  %5.1f%%  %s%s' % (n, 100.0 * n / summa, f, '' if lib.startswith(('computus', 'tempus')) else '  (' + lib + ')'))
# II. acervi plicati ex arbore vocationum
g = re.search(r'Call graph:\n(.*?)\n\n', t, re.S)
lineae = g.group(1).splitlines() if g else []
rx = re.compile(r'^([\s+!:|]*)(\d+) (\S+)\s+\(in ')
nodi = []   # (depth, count, name)
for l in lineae:
    mm = rx.match(l)
    if not mm: continue
    praef = mm.group(1)
    depth = len(praef)
    nodi.append((depth, int(mm.group(2)), mm.group(3)))
# self = count - sum(children); emit path;self
out = []
stack = []  # (depth, name, count, childsum)
def emit(i):
    d, n, c, cs = stack[i]
    s = c - cs
    if s > 0:
        out.append('%s %d' % (';'.join(x[1] for x in stack[:i + 1]), s))
for d, c, n in nodi:
    while stack and stack[-1][0] >= d:
        emit(len(stack) - 1); stack.pop()
    if stack: stack[-1] = (stack[-1][0], stack[-1][1], stack[-1][2], stack[-1][3] + c)
    stack.append((d, n, c, 0))
while stack:
    emit(len(stack) - 1); stack.pop()
open(plicatum, 'w').write('\n'.join(out) + '\n')
print('acervi plicati: %s (%d lineae); crudum: %s' % (plicatum, len(out), crudum))
PY
