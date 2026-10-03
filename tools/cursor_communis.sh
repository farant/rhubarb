#!/bin/bash
# tools/cursor_communis.sh - CURSOR COMMUNIS: cursores probationum per
# thesaurum (fabrica plan 2, migratio ad compilatorem; exemplar toml T5
# 215f88cc). FONTANDUM, non exsequendum.
#
# Vocator ante fontationem definit: RADIX_DIR, BUILD_DIR (absoluti),
# GCC_FLAGS, INCLUDE_FLAGS (tabulae). Functiones:
#   cursor_instrumenta_parare     bin/aedilis + bin/compilator (struit si
#                                 absunt); THESAURUS unus (FABRICA_THESAURUS,
#                                 ordinarie build/aedilis/obiecta)
#   cursor_clausuras_derivare D   clausura cuiusque D/probatio_*.c ab aedile
#                                 -> $BUILD_DIR/clausurae/<nomen>.lst
#   cursor_fontes_compilare       unio clausurarum per compilatorem ->
#                                 $BUILD_DIR/<basis>.o (basis gemina = FRACTA)
#   cursor_probationem_struere F B  probatio F per compilatorem
#                                 ($BUILD_DIR/probationes/) et nexus cum
#                                 clausura SUA -> binarium B
# Viae fontium ABSOLUTAE, cwd vocantis: -g utramque infigit - obiecta
# cursori veteri (clang directo) octetim aequalia (oraculum
# tools/cursoris_oraculum.sh). Mandata compilationis in
# $BUILD_DIR/clausurae/mandata.tsv (obiectum TAB argumenta) - oraculum
# determinismum clang inde iudicat.

# vexilla domus (tools/vexilla.sh, sedes una): GCC_FLAGS vocatoris
# praevalet; absens -> VEXILLA_C89
source "$RADIX_DIR/tools/vexilla.sh"
[ -n "${GCC_FLAGS[0]+x}" ] || GCC_FLAGS=("${VEXILLA_C89[@]}")

cursor_instrumenta_parare () {
    [ -x "$RADIX_DIR/bin/aedilis" ] || "$RADIX_DIR/tools/aedilis_struere.sh" >&2 \
        || { echo "FRACTA: bin/aedilis struere nequit"; return 1; }
    [ -x "$RADIX_DIR/bin/compilator" ] || "$RADIX_DIR/tools/compilator_struere.sh" >&2 \
        || { echo "FRACTA: bin/compilator struere nequit"; return 1; }
    COMPILATOR="$RADIX_DIR/bin/compilator"
    # thesaurus unus pro aedile (recorda extractionis) et compilatore:
    # sine eo aedilis clausuras omni cursu de novo parsat
    THESAURUS="${FABRICA_THESAURUS:-$RADIX_DIR/build/aedilis/obiecta}"
    export FABRICA_THESAURUS="$THESAURUS"
    CLAUSURAE_DIR="$BUILD_DIR/clausurae"
    mkdir -p "$CLAUSURAE_DIR" "$BUILD_DIR/probationes"
    : > "$CLAUSURAE_DIR/mandata.tsv"
    return 0
}

cursor_clausuras_derivare () {
    local dir="$1" test_file name
    shopt -s nullglob
    for test_file in "$dir"/probatio_*.c; do
        name="$(basename "$test_file" .c)"
        if ! (cd "$RADIX_DIR" && bin/aedilis "${test_file#"$RADIX_DIR"/}" \
                --enumerare --thesaurus "$THESAURUS") \
                > "$CLAUSURAE_DIR/$name.lst"; then
            echo "FRACTA (clausura): $name"
            shopt -u nullglob
            return 1
        fi
    done
    shopt -u nullglob
    return 0
}

# compilatio una per thesaurum; mandatum notatur
_cursor_compilare () {
    local src="$1" obj="$2"
    printf '%s\t%s\n' "$obj" "${GCC_FLAGS[*]} ${INCLUDE_FLAGS[*]} -c $src" \
        >> "$CLAUSURAE_DIR/mandata.tsv"
    "$COMPILATOR" "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"
}

cursor_fontes_compilare () {
    local fons numerus=0 gemina
    gemina="$(cat "$CLAUSURAE_DIR"/*.lst 2>/dev/null | sort -u \
        | sed 's|.*/||' | sort | uniq -d | head -3)"
    if [ -n "$gemina" ]; then
        echo "FRACTA: bases geminae in clausuris (obiecta collidunt): $(echo $gemina)"
        return 1
    fi
    while IFS= read -r fons; do
        if ! _cursor_compilare "$RADIX_DIR/$fons" \
                "$BUILD_DIR/$(basename "$fons" .c).o"; then
            echo "FRACTA: $fons"
            return 1
        fi
        numerus=$((numerus + 1))
    done < <(cat "$CLAUSURAE_DIR"/*.lst 2>/dev/null | sort -u)
    echo "  fontes clausurarum: $numerus (per bin/compilator)"
    return 0
}

cursor_probationem_struere () {
    local test_file="$1" bin="$2" name obj objs="" fons
    name="$(basename "$test_file" .c)"
    obj="$BUILD_DIR/probationes/$name.o"
    while IFS= read -r fons; do
        objs="$objs $BUILD_DIR/$(basename "$fons" .c).o"
    done < "$CLAUSURAE_DIR/$name.lst"
    _cursor_compilare "$test_file" "$obj" || return 1
    # shellcheck disable=SC2086 (objs: verba consulto scissa)
    clang "${GCC_FLAGS[@]}" "$obj" $objs -o "$bin"
}
