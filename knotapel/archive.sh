#!/bin/bash
#
# knotapel/archive.sh - freeze a demo that uses house libraries into ONE
# compilable file, demo-snapshot.c, next to its main.c.
#
# Usage: ./knotapel/archive.sh <demo_dir>/main.c [program args...]
#
# Why: a demo that includes house headers (laqueus.h, ...) builds live
# against rhubarb's include/ + lib/, which keep evolving. Its findings
# must stay reproducible, so when the demo is finished its closure is
# frozen: headers in dependency order, every library source with its
# file-local names renamed per file (#define s s_<stem> / #undef), the
# demo's main.c last; local includes blanked (line numbers kept),
# '#line 1 "path"' per file so compiler errors name the original file.
# Mechanism: silva/instrumenta/silva_conflatio (shared with briar
# -amalgama), driven by silva/instrumenta/principalia/conflator.c.
#
# Refuses (nothing written):
#   - any library file in the closure with uncommitted changes (the
#     commit recorded in the banner must mean something; the demo's own
#     main.c may be uncommitted - it is embedded verbatim and its blob
#     hash recorded);
#   - vendor/ or Objective-C in the closure, or a closure file outside
#     include/, lib/ and the client roots (conflator);
#   - an existing demo-snapshot.c this script did not write;
#   - a snapshot whose output differs from the live build's: both are
#     built and RUN with the same arguments, and stdout+stderr+exit code
#     must be byte-identical.
#
# Regression fixture: knotapel/archive-trial/ (12-vertex trefoil).

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$ROOT" || exit 1

fail() { echo "archive: $1" >&2; exit 1; }

[ $# -ge 1 ] || fail "usage: knotapel/archive.sh <demo_dir>/main.c [args...]"
MAIN_ARG="$1"; shift

# repo-relative path of main.c
case "$MAIN_ARG" in
    /*) MAIN_ABS="$MAIN_ARG" ;;
    *)  MAIN_ABS="$(cd "$OLDPWD" 2>/dev/null && cd "$(dirname "$MAIN_ARG")" && pwd)/$(basename "$MAIN_ARG")" ;;
esac
[ -f "$MAIN_ABS" ] || [ -f "$ROOT/$MAIN_ARG" ] || fail "no such file: $MAIN_ARG"
[ -f "$MAIN_ABS" ] || MAIN_ABS="$ROOT/$MAIN_ARG"
MAIN="${MAIN_ABS#"$ROOT"/}"
[ "$MAIN" != "$MAIN_ABS" ] || fail "$MAIN_ARG is outside the repository"
case "$MAIN" in *.c) ;; *) fail "$MAIN is not a .c file" ;; esac

DEMO_DIR="$(dirname "$MAIN")"
STEM="$(basename "$MAIN" .c)"
OUT="$DEMO_DIR/demo-snapshot.c"
WORK="build/archivum/$(echo "$DEMO_DIR" | sed 's|/|__|g')"
MARK="GENERATUM (knotapel/archive.sh)"

if [ -f "$OUT" ] && ! head -1 "$OUT" | grep -qF "$MARK"; then
    fail "$OUT exists and was not written by archive.sh - move it away first"
fi

rm -rf "$WORK"
mkdir -p "$WORK"

# ------------------------------------------------------------------
# tools: aedilis (closure + live build), conflator (snapshot)
# ------------------------------------------------------------------
./tools/aedilis_struere.sh > "$WORK/aedilis_struere.log" 2>&1 \
    || fail "aedilis build failed (see $WORK/aedilis_struere.log)"
./bin/aedilis silva/instrumenta/principalia/conflator.c > "$WORK/conflator_aedilis.log" 2>&1 \
    && bash build/aedilis/conflator/struere.sh > "$WORK/conflator_struere.log" 2>&1 \
    || fail "conflator build failed (see $WORK/conflator_*.log)"
CONFLATOR="build/aedilis/conflator/conflator"

# ------------------------------------------------------------------
# closure + uncommitted check
# ------------------------------------------------------------------
./bin/aedilis "$MAIN" --partes > "$WORK/partes.tsv" 2> "$WORK/partes.err" \
    || fail "aedilis could not derive the closure of $MAIN: $(head -3 "$WORK/partes.err")"
awk -F'\t' '$1=="C" || $1=="O" { print $2 }' "$WORK/partes.tsv" | sort -u > "$WORK/closure.txt"
[ -s "$WORK/closure.txt" ] || fail "$MAIN includes no house library - nothing to archive (just copy main.c)"

DIRTY="$(git status --porcelain -- $(cat "$WORK/closure.txt") 2>/dev/null)"
[ -z "$DIRTY" ] || fail "uncommitted changes in the library closure - commit first:
$DIRTY"

# ------------------------------------------------------------------
# statics table (corpus.symbola.tsv, from silva's nexus) + client roots
# ------------------------------------------------------------------
# shellcheck source=/dev/null
source tools/corpus_infixum.sh
corpus_symbola_generare > "$WORK/symbola.log" 2>&1 \
    || fail "statics table (corpus.symbola.tsv) could not be generated (see $WORK/symbola.log)"
ROOT_FLAGS=()
for r in "${RADICES_CLIENTIUM[@]}"; do
    ROOT_FLAGS+=("-r" "$r/")
done

# ------------------------------------------------------------------
# banner
# ------------------------------------------------------------------
# shellcheck source=/dev/null
source tools/vexilla.sh
FLAGS="${VEXILLA_C89[*]}"
COMMIT="$(git rev-parse HEAD)"
if [ -n "$(git status --porcelain -- "$MAIN")" ]; then
    MAIN_STATE="uncommitted, embedded verbatim"
else
    MAIN_STATE="committed"
fi
{
    echo "/* demo-snapshot.c - $MARK - DO NOT EDIT"
    echo " *"
    echo " * $MAIN frozen with its house-library closure as ONE file:"
    echo " * headers in dependency order, library sources with file-local"
    echo " * names renamed per file (#define/#undef), main.c last; '#line'"
    echo " * names each original file. Compile and run:"
    echo " *"
    echo " *   clang $FLAGS demo-snapshot.c -o demo-snapshot"
    echo " *"
    echo " * Commit (library closure clean): $COMMIT"
    echo " * Regenerate: ./knotapel/archive.sh $MAIN${*:+ $*}"
    echo " * Verified: live build and snapshot gave byte-identical output."
    echo " * Sources (git blob hashes):"
    while IFS= read -r f; do
        echo " *   $(git hash-object "$f")  $f"
    done < "$WORK/closure.txt"
    echo " *   $(git hash-object "$MAIN")  $MAIN ($MAIN_STATE)"
    echo " */"
    echo
} > "$WORK/banner.txt"

# ------------------------------------------------------------------
# snapshot
# ------------------------------------------------------------------
"$CONFLATOR" "${ROOT_FLAGS[@]}" "$WORK/partes.tsv" corpus.symbola.tsv \
    "$MAIN" "$WORK/banner.txt" "$WORK/demo-snapshot.c" > "$WORK/conflator.log" 2>&1 \
    || fail "conflator refused: $(cat "$WORK/conflator.log")"
# shellcheck disable=SC2086
clang "${VEXILLA_C89[@]}" "$WORK/demo-snapshot.c" -o "$WORK/demo-snapshot" \
    > "$WORK/snapshot_compile.log" 2>&1 \
    || fail "snapshot does not compile under house flags:
$(head -20 "$WORK/snapshot_compile.log")"

# ------------------------------------------------------------------
# live build
# ------------------------------------------------------------------
./bin/aedilis "$MAIN" > "$WORK/live_aedilis.log" 2>&1 \
    && bash "build/aedilis/$STEM/struere.sh" > "$WORK/live_struere.log" 2>&1 \
    || fail "live build failed:
$(tail -20 "$WORK/live_struere.log" 2>/dev/null)"
cp "build/aedilis/$STEM/$STEM" "$WORK/live"

# ------------------------------------------------------------------
# run both, compare
# ------------------------------------------------------------------
"$WORK/live" "$@" > "$WORK/live.out" 2>&1
echo "exit $?" >> "$WORK/live.out"
"$WORK/demo-snapshot" "$@" > "$WORK/snapshot.out" 2>&1
echo "exit $?" >> "$WORK/snapshot.out"
if ! cmp -s "$WORK/live.out" "$WORK/snapshot.out"; then
    echo "archive: live and snapshot outputs DIFFER - not written:" >&2
    diff "$WORK/live.out" "$WORK/snapshot.out" | head -20 >&2
    exit 1
fi

cp "$WORK/demo-snapshot.c" "$OUT"
echo "archive: $OUT written ($(wc -l < "$OUT" | tr -d ' ') lines, $(wc -l < "$WORK/closure.txt" | tr -d ' ') library files, output identical: $(wc -l < "$WORK/live.out" | tr -d ' ') lines)"
