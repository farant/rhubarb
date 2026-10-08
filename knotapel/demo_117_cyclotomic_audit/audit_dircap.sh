#!/bin/bash
# Demo 117: which demos hit their direction cap?
#
# Demos 74-97 copy one helper that collects distinct rotation axes into
# `g_dir[MAX_DIR]` and silently stops adding when the array is full:
#     if (!found && g_nd < MAX_DIR) {
# This script copies every demo source containing `g_nd < MAX_DIR`,
# rewrites that test so a refused direction is COUNTED on stderr
# (`g_nd < MAX_DIR || (fprintf(stderr, "DIRCAP\n"), 0)` - same truth value,
# so the program behaves exactly as shipped), builds at -O2, runs each under
# a time limit, and writes dircap.tsv: demo file, MAX_DIR, exit code,
# seconds, refused directions (0 = the cap never bit).
#
# Usage: knotapel/demo_117_cyclotomic_audit/audit_dircap.sh <work_dir> [limit_s] [jobs]
set -u
KNOTAPEL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${1:?usage: audit_dircap.sh <work_dir> [limit_s] [jobs]}"
LIMIT="${2:-1800}"
JOBS="${3:-6}"
mkdir -p "$WORK" || exit 2
WORK="$(cd "$WORK" && pwd)"

unus() {
    local src="$1" titulus cap t0 rc
    titulus="$(basename "$(dirname "$src")")__$(basename "$src" .c)"
    mkdir -p "$WORK/$titulus" && cd "$WORK/$titulus" || return
    cap="$(grep -o '^#define MAX_DIR [0-9]*' "$src" | awk '{print $3}')"
    sed 's/&& g_nd < MAX_DIR) {/\&\& (g_nd < MAX_DIR || (fprintf(stderr, "DIRCAP\\n"), 0))) {/' \
        "$src" > prog.c
    if [ "$(grep -c 'DIRCAP' prog.c)" != "$(grep -c 'g_nd < MAX_DIR' "$src")" ]; then
        printf '%s\t%s\tpatch\t0\t-\n' "$titulus" "$cap"; return
    fi
    cc -std=c89 -O2 -o prog prog.c -lm 2> build.log \
        || { printf '%s\t%s\tbuild\t0\t-\n' "$titulus" "$cap"; return; }
    t0=$(date +%s)
    timeout "$LIMIT" ./prog > out.log 2> err.log; rc=$?
    printf '%s\t%s\t%s\t%s\t%s\n' "$titulus" "$cap" "$rc" "$(( $(date +%s) - t0 ))" \
        "$(grep -c '^DIRCAP$' err.log)"
}
export -f unus
export WORK LIMIT

grep -l 'g_nd < MAX_DIR' "$KNOTAPEL"/demo_*/*.c | grep -v 'snapshot' \
    | xargs -P "$JOBS" -I{} bash -c 'unus "$@"' _ {} > "$WORK/dircap.unsorted"
{ printf 'demo\tmax_dir\texit\tseconds\trefused\n'; sort "$WORK/dircap.unsorted"; } > "$WORK/dircap.tsv"
cat "$WORK/dircap.tsv"
