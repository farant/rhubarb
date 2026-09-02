#!/bin/bash

# gesta/tabulariumd.sh - launcher daemonis TCP tabularii (F0 forum)
# (exemplar tabularium.sh: garrulitas stderr SOLA - "portus N" in
# stdout = semen fumi; constructio clausura-podata; -radix SEMPER
# appenditur)
#
# Usus: ./gesta/tabulariumd.sh                  # mundus fori, portus 8753
#       ./gesta/tabulariumd.sh -struere         # aedifica et exi
#       ./gesta/tabulariumd.sh -portus 0 ...    # auto-portus (fumus)

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
BIN="$BUILD_DIR/tabulariumd"
mkdir -p "$BUILD_DIR" "$GESTA_DIR/annales"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include" "-I$GESTA_DIR/fontes"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$GESTA_DIR/tabulariumd_fontes_generata.sh"
declare -a VENDOR_FLAGS=(
    "-O2" "-DSQLITE_ENABLE_FTS5" "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DQS=0" "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_LOAD_EXTENSION" "-DSQLITE_OMIT_DEPRECATED"
    "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"
)

newest_header () {
    find "$RADIX_DIR/include" "$GESTA_DIR/fontes" -name '*.h' -newer "$1" 2>/dev/null | head -1
}

# ── clausura vera inclusionum (01KXZYFVER; exemplar tabularium.sh) ──
FONTES_OMNES=()
for f in "${RADIX_FONTES[@]}"; do FONTES_OMNES+=("$RADIX_DIR/lib/$f.c"); done
for f in "$GESTA_DIR"/fontes/*.c; do FONTES_OMNES+=("$f"); done
FONTES_OMNES+=("$GESTA_DIR/instrumenta/tabulariumd_principale.c")

DEPENDENTIAE="$(clang -MM "${INCLUDE_FLAGS[@]}" "${FONTES_OMNES[@]}" 2>/dev/null \
    | awk '{ if (sub(/\\$/,"")) printf "%s ", $0; else print }')"

deps_obiecti () {   # $1 = basis (sine .c); lineae plures = unio tuta
    printf '%s\n' "$DEPENDENTIAE" | grep "^$1\.o:" | tr ' ' '\n' | grep '^/'
}
recentius_ex () {   # $1 = scopus; stdin = viae; prima recentior imprimitur
    while IFS= read -r via; do
        [ -n "$via" ] && [ "$via" -nt "$1" ] && { echo "$via"; return 0; }
    done
    return 0
}
fons_stalus () {    # $1 = basis, $2 = scopus; exitus 0 = recompilandum
    if [ -n "$DEPENDENTIAE" ]; then
        [ -n "$(deps_obiecti "$1" | recentius_ex "$2")" ]
    else
        [ -n "$(newest_header "$2")" ]
    fi
}

sqlite_obj="$BUILD_DIR/sqlite3.o"
if [ ! -f "$sqlite_obj" ] || [ "$RADIX_DIR/vendor/sqlite3.c" -nt "$sqlite_obj" ]; then
    echo "  [vendor] sqlite3.c (laxum)" >&2
    clang -c "${VENDOR_FLAGS[@]}" "$RADIX_DIR/vendor/sqlite3.c" -o "$sqlite_obj" >&2 || exit 1
fi
obj_files="$sqlite_obj"

for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus "$f" "$obj"; then
        echo "  [dep] $f.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" >&2 || exit 1
    fi
    obj_files="$obj_files $obj"
done

for src in "$GESTA_DIR"/fontes/*.c; do
    base="$(basename "$src" .c)"
    obj="$BUILD_DIR/$base.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || fons_stalus "$base" "$obj"; then
        echo "  [gesta] $base.c" >&2
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" >&2 || exit 1
    fi
    obj_files="$obj_files $obj"
done

main_src="$GESTA_DIR/instrumenta/tabulariumd_principale.c"
obj_recentius=""
for o in $obj_files; do
    if [ "$o" -nt "$BIN" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || [ "$main_src" -nt "$BIN" ] || [ -n "$obj_recentius" ] || fons_stalus tabulariumd_principale "$BIN"; then
    echo "  [nexus] tabulariumd" >&2
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$main_src" $obj_files -o "$BIN" >&2 || exit 1
fi

# vigilia: manifestum proprium (tabulariumd.vigilia) - resident
# fori suam staleness ipse nuntiat
MANIFEST="$BUILD_DIR/tabulariumd.vigilia"
{
    echo "# clausura vera launcheri (clang -MM; exemplar tabularium.sh)"
    echo "$RADIX_DIR/vendor/sqlite3.c"
    if [ -n "$DEPENDENTIAE" ]; then
        printf '%s\n' "$DEPENDENTIAE" | tr ' ' '\n' | grep '^/' | sort -u
    else
        for f in "${FONTES_OMNES[@]}"; do echo "$f"; done
        find "$RADIX_DIR/include" "$GESTA_DIR/fontes" -name '*.h'
    fi
} > "$MANIFEST" 2>/dev/null

# -struere: aedifica et exi (semen fumi + praestructio + explorator
# renovationis; linea prima stdout = signum novum)
if [ "${1:-}" = "-struere" ]; then
    shasum -a 256 "$BIN" 2>/dev/null | cut -c1-64
    exit 0
fi

SIGNUM="$(shasum -a 256 "$BIN" 2>/dev/null | cut -c1-64)"
if [ -n "$SIGNUM" ]; then
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR" -signum "$SIGNUM" -binarium "$BIN" -manifestum "$MANIFEST"
else
    cd "$RADIX_DIR" && exec "$BIN" "$@" -radix "$RADIX_DIR"
fi
