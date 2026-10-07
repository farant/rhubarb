#!/bin/bash
# tools/aemulator_esctest.sh - esctest2 (../esctest2 @ 2798f12) intra
# aemulatorem currit (aemulator-plan B4b). esctest2 GPL-2.0: numquam in
# repositorium copiatur - solum ut infans currit (mos oraculi bash).
#
# Usus:
#   ./tools/aemulator_esctest.sh [-include REGEX]  # currit; tabula in
#                                                  # build/aemulator_esctest/eventus.tsv
#   ./tools/aemulator_esctest.sh -probare           # tabula == fixa pinnata
#   ESCTEST_DIR=<via> ...                           # clonus alius
#
# Optiones esctest: --expected-terminal=xterm --xterm-checksum=334
# (cellula vacua = spatium, ut nucleus noster) --max-vt-level=4 (DECRQCRA
# poscit IV; lectio_schirmi in ductore aperta, decisio XVIII)
# --xterm-reverse-wrap=383 (D2: XLV et MXLV ut xterm recens et Ghostty -
# XLV solum in lineas involutas, MXLV trans summam; ordinarium 0 XLV
# vetus exspectat).
# Exitus: 0 congruit/scriptum; 1 discrepantia; 2 NIHIL CUCURRIT
# (clonus deest aut commissum alienum - nominatur).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
RADIX_DIR="$PWD"
ESCTEST_DIR="${ESCTEST_DIR:-$RADIX_DIR/../esctest2}"
ESCTEST_COMMISSUM="2798f12"
BUILD="build/aemulator_esctest"
FIXA="probationes/fixa/aemulator/esctest.tsv"
si_fracta() { echo "aemulator_esctest: $1" >&2; exit "${2:-1}"; }

PROBARE=0
INCLUDE=()
while [ $# -gt 0 ]; do
    case "$1" in
        -probare) PROBARE=1 ;;
        -include) shift; INCLUDE=("--include=$1") ;;
        *) si_fracta "usus: aemulator_esctest.sh [-include REGEX] [-probare]" 2 ;;
    esac
    shift
done

# ---- clavus: clonus ad commissum fixum ----
[ -f "$ESCTEST_DIR/esctest/esctest.py" ] \
    || si_fracta "clonus esctest2 deest: $ESCTEST_DIR (git clone https://github.com/ThomasDickey/esctest2 ../esctest2; git -C ../esctest2 checkout $ESCTEST_COMMISSUM)" 2
caput="$(git -C "$ESCTEST_DIR" rev-parse --short=7 HEAD 2>/dev/null)"
[ "$caput" = "$ESCTEST_COMMISSUM" ] \
    || si_fracta "esctest2 ad '$caput', non ad $ESCTEST_COMMISSUM (git -C $ESCTEST_DIR checkout $ESCTEST_COMMISSUM)" 2
command -v python3 >/dev/null || si_fracta "python3 deest" 2

# ---- constructio ----
source tools/vexilla.sh
mkdir -p "$BUILD"
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 2
obiecta=()
while IFS= read -r fons; do
    case "$fons" in lib/*.c) ;; *) continue ;; esac
    obj="$BUILD/$(basename "$fons" .c).o"
    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$fons" ]; then
        clang "${VEXILLA_C89[@]}" -Iinclude -c "$fons" -o "$obj" || si_fracta "$fons" 2
    fi
    obiecta+=("$obj")
done < <(./bin/aedilis tools/aemulator_esctest.c --enumerare 2>/dev/null)
clang "${VEXILLA_C89[@]}" -Iinclude -c tools/aemulator_esctest.c \
    -o "$BUILD/ductor.o" || si_fracta "ductor" 2
clang "$BUILD/ductor.o" "${obiecta[@]}" -o "$BUILD/aemulator_esctest" \
    || si_fracta "nexus" 2

# ---- cursus ----
rm -f "$BUILD/esctest.log"
"$BUILD/aemulator_esctest" python3 "$ESCTEST_DIR/esctest/esctest.py" \
    --expected-terminal=xterm --xterm-checksum=334 --max-vt-level=4 \
    --xterm-reverse-wrap=383 \
    --timeout=0.5 --no-print-logs --logfile="$RADIX_DIR/$BUILD/esctest.log" \
    "${INCLUDE[@]+"${INCLUDE[@]}"}" > "$BUILD/ductor.out" 2>&1
codex=$?
[ -s "$BUILD/esctest.log" ] || { cat "$BUILD/ductor.out" >&2; si_fracta "esctest nihil scripsit (codex $codex)" 2; }

# ---- tabula: nomen<TAB>status (transiit|fractum|omissum|notum) ----
awk '
    /Run test: / { if (nomen != "") print nomen "\tfractum"; sub(/.*Run test: /, ""); nomen = $0; next }
    nomen != "" && /Passed\./ { print nomen "\ttransiit"; nomen = ""; next }
    nomen != "" && /Fails as expected/ { print nomen "\tnotum"; nomen = ""; next }
    nomen != "" && /Skipped because/ { print nomen "\tomissum"; nomen = ""; next }
    nomen != "" && /\*\*\* TEST .* FAILED/ { print nomen "\tfractum"; nomen = ""; next }
    END { if (nomen != "") print nomen "\tfractum" }
' "$BUILD/esctest.log" | sort > "$BUILD/eventus.tsv"
for s in transiit fractum omissum notum; do
    printf '%s %s  ' "$s" "$(grep -c "	$s\$" "$BUILD/eventus.tsv")"
done
echo "($(tail -1 "$BUILD/ductor.out"))"

if [ "$PROBARE" -eq 1 ]; then
    [ -f "$FIXA" ] || si_fracta "fixa deest: $FIXA" 2
    if ! grep -v '^#' "$FIXA" | cut -f1,2 | diff -u - "$BUILD/eventus.tsv"; then
        si_fracta "eventus != fixa ($FIXA): fractum novum aut debitum solutum - promove aut emenda" 1
    fi
    echo "aemulator_esctest: eventus == fixa ($(grep -cv '^#' "$FIXA") probationes)"
fi
exit 0
