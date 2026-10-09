#!/bin/bash
# Demo 117, Part D: demo 79's direction caps.
#
# Demo 79 collects the distinct rotation axes of its zeta_12 quaternion
# catalog into `g_dir[MAX_DIR]` and silently stops adding past the cap
# (`if (!found && g_nd < MAX_DIR)`). main.c caps at 64 and the companion
# programs at 512; the published "64 directions / 65 cells" (256-entry
# catalog) and "512 directions" (4,096-entry catalog) are those caps.
# main.c also reads `dir_count[g_nd]` one past `int dir_count[MAX_DIR]`
# (the identity row of its table), which UBSan flags.
#
# main.c also limits activations to MAX_ACT_CELLS 1,024 cells, so with 113
# cells its k ladder stops at k = 8 (65 cells allowed k = 12). The XOR12
# programs stop at a fixed number of winners: xor12_fast at 50 (the
# published "50+"), xor12_retest at 20.
#
# This script builds main.c UNMODIFIED at -O0 and -O2, with the overrun
# fixed (array sized MAX_DIR + 1), and UNCAPPED (MAX_DIR 4096);
# catalog_test.c unmodified and uncapped (MAX_DIR 65536); and xor12_fast.c
# unmodified and FULL (MAX_DIR and MAX_ACT 65536, so every k of the ladder
# fits 2,044 cells). It prints the direction counts, the XOR ladders and
# the k / cell tally of the XOR12 winners. About 55 minutes on one core,
# most of it main.c four times and xor12_fast unmodified.
#
# Usage: knotapel/demo_117_cyclotomic_audit/audit_d79.sh <work_dir>
set -u
DEMO="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/demo_79_zeta12_capacity"
WORK="${1:?usage: audit_d79.sh <work_dir>}"
mkdir -p "$WORK" && cd "$WORK" || exit 2
cp "$DEMO/main.c" orig.c
cp "$DEMO/catalog_test.c" ct_orig.c
cp "$DEMO/xor12_fast.c" fast_orig.c
anchor() { grep -c "$1" "$2" | grep -qx 1 || { echo "audit_d79: anchor '$1' not found in $2"; exit 2; }; }
anchor '^#define MAX_DIR 64$' orig.c
anchor '^    int dir_count\[MAX_DIR\];$' orig.c
anchor '^#define MAX_DIR 512$' ct_orig.c
anchor '^#define MAX_DIR 512$' fast_orig.c
anchor '^#define MAX_ACT 8192$' fast_orig.c
sed 's|^    int dir_count\[MAX_DIR\];$|    int dir_count[MAX_DIR + 1];|' orig.c > fix.c
sed 's|^#define MAX_DIR 64$|#define MAX_DIR 4096|' fix.c > uncap.c
sed 's|^#define MAX_DIR 512$|#define MAX_DIR 65536|' ct_orig.c > ct_uncap.c
sed -e 's|^#define MAX_DIR 512$|#define MAX_DIR 65536|' \
    -e 's|^#define MAX_ACT 8192$|#define MAX_ACT 65536|' fast_orig.c > fast_full.c
cc -std=c89 -O0 -o o0 orig.c -lm && cc -std=c89 -O2 -o o2 orig.c -lm \
    && cc -std=c89 -O2 -o fix fix.c -lm && cc -std=c89 -O2 -o uncap uncap.c -lm \
    && cc -std=c89 -O2 -o ct_orig ct_orig.c -lm \
    && cc -std=c89 -O2 -o ct_uncap ct_uncap.c -lm \
    && cc -std=c89 -O2 -o fast_orig fast_orig.c -lm \
    && cc -std=c89 -O2 -o fast_full fast_full.c -lm || exit 2
for v in o0 o2 fix uncap ct_orig ct_uncap fast_orig fast_full; do ./$v > $v.log 2>&1; done
for v in o0 o2 fix uncap; do
    echo "== main.c $v: $(grep -o 'S2 directions: [0-9]*' $v.log), identity row: $(grep '^    id ' $v.log | awk '{print $NF}'), $(grep -o 'Results: .*' $v.log)"
done
if cmp -s o2.log fix.log; then
    echo "unmodified -O2 output == overrun-fixed output (the overrun only changes -O0's identity row)"
else
    echo "unmodified -O2 output DIFFERS from overrun-fixed:"; diff o2.log fix.log | head -10
fi
for v in o2 uncap; do
    echo "== XOR ladder, $v:"
    grep -E '^  XOR(6|8|10|12): ' $v.log
done
echo "== catalog_test directions (4,096-entry catalog): capped $(grep -m1 -o 'Directions: [0-9]*' ct_orig.log), uncapped $(grep -m1 -o 'Directions: [0-9]*' ct_uncap.log)"
for v in fast_orig fast_full; do
    echo "== xor12_fast $v: $(grep -m1 -o 'Directions: [0-9]*' $v.log), $(grep -o 'XOR12 winners found: [0-9]*' $v.log), $(grep -o 'Unique candidates: [0-9]*' $v.log); by k:"
    grep -o 'at k=[0-9]* ([0-9]* cells)' $v.log | sort | uniq -c | sort -t= -k2 -n
done
