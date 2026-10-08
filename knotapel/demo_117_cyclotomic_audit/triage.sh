#!/bin/bash
# Demo 117, Part 0: triage of the cyclotomic demos with UBSan.
#
# Every demo whose main.c uses cyclotomic arithmetic is copied to a scratch
# directory (some demos write files where they run), compiled UNMODIFIED
# with -fsanitize=undefined (signed overflow, shifts, division, bounds,
# ...), and run under a time limit. UBSan reports each faulting source
# location once per process (it deduplicates), so the counts are SITES, not
# occurrences.
#
# Usage: knotapel/demo_117_cyclotomic_audit/triage.sh <work_dir> [limit_s] [jobs] [demo ...]
#   (no demo names: every cyclotomic demo; with names: only those, written
#    to <work_dir>/triage_selected.tsv)
# Output: <work_dir>/triage.tsv and <work_dir>/<demo>/{build,run}.log
set -u
KNOTAPEL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${1:?usage: triage.sh <work_dir> [limit_s] [jobs]}"
LIMIT="${2:-600}"
JOBS="${3:-6}"
mkdir -p "$WORK"
WORK="$(cd "$WORK" && pwd)"

shift $(( $# < 3 ? $# : 3 ))
SELECTED="$*"
DEMOS=$(cd "$KNOTAPEL" && grep -rl -i 'cyclotomic\|zeta_8\|Z\[zeta\|zeta8\|Cyc8\|cyc_\|Cyc16\|Cyc5\|Cyc24\|zeta_12\|zeta_16' --include=main.c . \
    | sed 's|^\./||; s|/main.c$||' | sort)

one() {
    demo="$1"
    out="$WORK/$demo"
    rm -rf "$out"
    mkdir -p "$out/src"
    cp -R "$KNOTAPEL/$demo/." "$out/src/"
    # demos 107-109 include "../raqiya/raqiya.h"
    [ -d "$KNOTAPEL/raqiya" ] && cp -R "$KNOTAPEL/raqiya" "$out/raqiya"
    rm -f "$out/src/"*.o
    start=$(date +%s)
    if ! clang -std=c89 -O1 -g -fsanitize=undefined -fno-omit-frame-pointer \
            -o "$out/demo" "$out/src/main.c" -lm > "$out/build.log" 2>&1; then
        printf '%s\tbuild_error\t0\t0\t0\t-\n' "$demo"
        return
    fi
    (cd "$out/src" && UBSAN_OPTIONS=print_stacktrace=0 timeout "$LIMIT" "$out/demo") \
        > "$out/run.log" 2>&1
    rc=$?
    secs=$(( $(date +%s) - start ))
    status=ok
    [ "$rc" -eq 124 ] && status=timeout
    [ "$rc" -ne 0 ] && [ "$rc" -ne 124 ] && status="rc$rc"
    sites=$(grep -c 'runtime error:' "$out/run.log")
    overflow=$(grep 'runtime error:' "$out/run.log" | grep -c 'overflow')
    first=$(grep -m1 'runtime error:' "$out/run.log" | sed "s|$out/src/||" | cut -c1-160)
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$demo" "$status" "$secs" "$sites" "$overflow" "${first:--}"
}
export -f one
export WORK KNOTAPEL LIMIT

TABLE="$WORK/triage.tsv"
if [ -n "$SELECTED" ]; then
    DEMOS="$SELECTED"
    TABLE="$WORK/triage_selected.tsv"
fi
{
    printf 'demo\tstatus\tseconds\tubsan_sites\toverflow_sites\tfirst_site\n'
    printf '%s\n' $DEMOS | xargs -P "$JOBS" -I{} bash -c 'one "$@"' _ {} | sort
} > "$TABLE"
echo "triage: $(printf '%s\n' $DEMOS | wc -l | tr -d ' ') demos -> $TABLE"
