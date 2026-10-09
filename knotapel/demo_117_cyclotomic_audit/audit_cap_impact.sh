#!/bin/bash
# Demo 117: what the direction cap changed in demos 80-83.
#
# audit_dircap.sh finds the cap biting in demo 79 (see audit_d79.sh) and in
# demos 80-83, all at MAX_DIR 512. This script builds each of demos 80-83
# UNMODIFIED and FULL (MAX_DIR 65536, and in 81-83 MAX_ACT 131072: lifting
# the direction cap alone pushes k x cells past MAX_ACT 16384, which cuts
# the k ladder and fails one test in each), runs both in parallel, and
# prints the test totals and every output line that changed. About 8
# minutes on 7 cores.
#
# Usage: knotapel/demo_117_cyclotomic_audit/audit_cap_impact.sh <work_dir>
set -u
KNOTAPEL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${1:?usage: audit_cap_impact.sh <work_dir>}"
mkdir -p "$WORK" && cd "$WORK" || exit 2
DEMOS="demo_80_group_finiteness demo_81_capacity_scaling demo_82_crossing_depth demo_83_framing_test"
for d in $DEMOS; do
    cp "$KNOTAPEL/$d/main.c" "$d.orig.c"
    grep -c '^#define MAX_DIR 512$' "$d.orig.c" | grep -qx 1 \
        || { echo "audit_cap_impact: MAX_DIR anchor not found in $d"; exit 2; }
    sed -e 's|^#define MAX_DIR 512$|#define MAX_DIR 65536|' \
        -e 's|^#define MAX_ACT 16384$|#define MAX_ACT 131072|' "$d.orig.c" > "$d.full.c"
    cc -std=c89 -O2 -o "$d.orig" "$d.orig.c" -lm && cc -std=c89 -O2 -o "$d.full" "$d.full.c" -lm || exit 2
done
for d in $DEMOS; do echo "$d.orig"; echo "$d.full"; done \
    | xargs -P 8 -I{} sh -c './{} > {}.log 2>&1'
for d in $DEMOS; do
    echo "== $d: tests $(grep -o '[0-9]* pass, [0-9]* fail' $d.orig.log | tail -1) -> $(grep -o '[0-9]* pass, [0-9]* fail' $d.full.log | tail -1)"
    diff "$d.orig.log" "$d.full.log"
done
exit 0
