#!/bin/bash
# Demo 117, Part C: demo 90's stack-array overrun.
#
# Phase 2d of demo 90 writes n_gen x n_depth cross-depth sums into
# `double depth_angles[MAX_QCAT]` (4,096) on the stack: 5 x 1,152 = 5,760
# at depth 7 and 5 x 1,804 = 9,020 at depth 8. This script builds demo 90
# UNMODIFIED at -O0 (knotapel's plain `cc` build) and -O2, and a PATCHED
# copy (the array made static, sized for 20 generators x 4,096), runs all
# three and compares their output.
#
# Then a FULL copy: the overrun fixed and `count_distinct`'s silent cap
# (MAX_DISTINCT 2,048) lifted, with the call sites that truncate their input
# to 2,048 entries. Phase 1's four distinct counts are all the cap as
# shipped, and its two failing checks compare 2048 < 2048; phase 2c counts
# only the first 2,048 of its 50,000 pairs. (Found by review I of demo 117.)
#
# Usage: knotapel/demo_117_cyclotomic_audit/audit_d90.sh <work_dir>
set -u
KNOTAPEL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${1:?usage: audit_d90.sh <work_dir>}"
mkdir -p "$WORK" && cd "$WORK" || exit 2
cp "$KNOTAPEL/demo_90_sum_angle_structure/main.c" orig.c
grep -c '            double depth_angles\[MAX_QCAT\];' orig.c | grep -qx 1 \
    || { echo "audit_d90: patch anchor not found"; exit 2; }
sed 's|            double depth_angles\[MAX_QCAT\];|            static double depth_angles[MAX_QCAT * 20];|' orig.c > patched.c
for a in '^#define MAX_DISTINCT 2048$' 'n_cross > 2048 ? 2048 : n_cross' 'n_shal > 2048 ? 2048 : n_shal' \
         'n_deep2 > 2048 ? 2048 : n_deep2' 'n_cd8 > MAX_DISTINCT ? MAX_DISTINCT : n_cd8'; do
    grep -c "$a" patched.c | grep -qx 1 || { echo "audit_d90: anchor '$a' not found"; exit 2; }
done
sed -e 's|^#define MAX_DISTINCT 2048$|#define MAX_DISTINCT 200000|' \
    -e 's|n_cross > 2048 ? 2048 : n_cross|n_cross|' -e 's|n_shal > 2048 ? 2048 : n_shal|n_shal|' \
    -e 's|n_deep2 > 2048 ? 2048 : n_deep2|n_deep2|' \
    -e 's|n_cd8 > MAX_DISTINCT ? MAX_DISTINCT : n_cd8|n_cd8|' patched.c > full.c
cc -std=c89 -O0 -o o0 orig.c -lm && cc -std=c89 -O2 -o o2 orig.c -lm \
    && cc -std=c89 -O0 -o patched patched.c -lm && cc -std=c89 -O2 -o full full.c -lm || exit 2
./o0 > o0.log 2>&1; rc0=$?
./o2 > o2.log 2>&1; rc2=$?
./patched > patched.log 2>&1; rcp=$?
echo "unmodified -O0: exit $rc0 (139 = segmentation fault); last phase: $(grep -o 'starting phase[0-9a-z]*' o0.log | tail -1)"
echo "unmodified -O2: exit $rc2; patched: exit $rcp (2 = two prediction tests fail as shipped)"
if cmp -s o2.log patched.log; then
    echo "unmodified -O2 output == patched output ($(wc -l < patched.log | tr -d ' ') lines)"
else
    echo "unmodified -O2 output DIFFERS from patched:"
    diff o2.log patched.log | head -20
fi
./full > full.out 2> full.err; rcf=$?
echo "full (overrun fixed, MAX_DISTINCT lifted): exit $rcf; $(grep -o 'Results: .*' full.out)"
for v in patched full; do
    f=$v.log; [ $v = full ] && f=full.out
    echo "  $v: $(grep -E 'distinct sum angles' $f | sed 's/ distinct sum angles://; s/  */ /g' | tr '\n' ';')"
    echo "  $v: $(grep -E 'Deep x Deep|Shallow x Deep' $f | sed 's/  */ /g' | tr '\n' ';')"
done
