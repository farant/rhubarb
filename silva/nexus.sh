#!/bin/bash

# silva/nexus.sh - nexus (instrumenta prima #4)
#
# Usage:
#   ./silva/nexus.sh <symbolum> [-omnia]    sedes + usus symboli
#   ./silva/nexus.sh -similis <quaestio>    nomina similia
#   ./silva/nexus.sh -vetus <symbolum>      sine sanatione (tabula
#                                           vetus consulto)
#   ./silva/nexus.sh -renovare              tabulam regenerare
#                                           (incrementale ~1-2s;
#                                           -plenus vim facit)
# Exit:  0 inventum | 1 non inventum | 2 tabula deest / usus
#
# SE-SANANS (2026-07-29, 01KYQ4H06F): quaestio quaeque percursum
# incrementalem ANTE responsum currit (nulla-mutatio ~0.4s) -
# tabula stala non recusat, MENTITUR; porta ad tempus quaestionis
# pertinet. Renovatione fracta (fons medio-editu?): responsum e
# tabula VETERE cum cautione GRAVI - numquam obstructio, numquam
# tacite. Tabula build/nexus.tsv DISPONIBILIS: numquam committitur.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
mkdir -p "$BUILD_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
# MUNDUS AMALGAMATIS (LEGATUS v0.1b): sweep contra amalgama
# aedificatur (fontes numquam); logica ordinum in
# instrumenta/nexus_ordines.{h,c} communis cum legato. Barra
# migrationis: paritas octetim tsv (vide legatus.worklog).
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/amalgama"
    "-I$SILVA_DIR/instrumenta"
)
# GENERATUM AB AEDILE - fontes derivati (regeneratio: vide snippet)
source "$SILVA_DIR/nexus_fontes_generata.sh"

# COMPILATIO PER THESAURUM (fabrica plan 2, migratio nexus 2026-10-03):
# olim mtime + 'caput recentissimum in include/ OMNIA recompilat' ->
# relinkatio -> 'instrumentum novum -> plenus' = percursus PLENUS
# (~190 s, 1685 plagulae) post QUAMVIS mutationem capitis, etiam non
# inclusi (include/fabrica.h). Nunc bin/compilator: obiectum rescribitur
# SOLUM si octeti mutantur, ergo 'binarium recentius omni obiecto' =
# nihil mutatum. Sine compilatore (clonus recens, aedificari nequit):
# clang cum regulis veteribus (mtime + caput recentissimum).
COMPILATOR="$RADIX_DIR/bin/compilator"
if [ ! -x "$COMPILATOR" ]; then
    "$RADIX_DIR/tools/compilator_struere.sh" > /dev/null 2>&1 || COMPILATOR=""
fi
newest_header () {
    find "$RADIX_DIR/include" -name '*.h' -newer "$1" 2>/dev/null | head -1
}
# compilare <fons> <obiectum> <capita_extra...> -- <vexilla...>
#   per compilatorem: semper vocatur (ipse iudicat, identicum non
#   rescribit). Sine eo: mos vetus (mtime fontis/capitum + caput
#   recentissimum). Nuntius '[nota] fons' SOLUM si obiectum mutatum.
compilare () {
    local nota="$1" src="$2" obj="$3" ante=""
    shift 3
    local capita=()
    while [ "$#" -gt 0 ] && [ "$1" != "--" ]; do capita+=("$1"); shift; done
    shift
    [ -f "$obj" ] && ante="$(stat -f '%m%i' "$obj")"
    if [ -n "$COMPILATOR" ]; then
        "$COMPILATOR" "$@" -c "$src" -o "$obj" || return 1
    else
        local c stalum=0
        { [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; } && stalum=1
        for c in "${capita[@]+"${capita[@]}"}"; do ! [ "$obj" -nt "$c" ] && stalum=1; done
        [ "$stalum" = "1" ] && { clang "$@" -c "$src" -o "$obj" || return 1; }
    fi
    [ "$(stat -f '%m%i' "$obj" 2>/dev/null)" != "$ante" ] && echo "  [$nota] $(basename "$src")" >&2
    return 0
}
# recentius <binarium> <obiecta...>: 0 si binarium exstat et STRICTE
# recentius omni obiecto (cum compilatore: obiectum mutatum = octeti
# mutati). Sine FABRICA_AGIT (nexus instrumentum quaestionis est, non
# installator: sub executore idem iudicium).
recentius () {
    local binarium="$1" f
    shift
    [ -f "$binarium" ] || return 1
    for f in "$@"; do
        ! [ "$binarium" -nt "$f" ] && return 1
    done
    return 0
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    compilare dep "$RADIX_DIR/lib/$f.c" "$BUILD_DIR/$f.o" -- \
        "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || exit 1
    obj_files="$obj_files $BUILD_DIR/$f.o"
done

# silva.h mutatio formae sine recompilo = corruptio ABI (inventum
# v0.2 in officina; TERTIUM exemplar manu volutum - excubitor chunk 1)
SILVA_H="$SILVA_DIR/amalgama/silva.h"

compilare amalgama "$SILVA_DIR/amalgama/silva.c" \
    "$BUILD_DIR/nexus_amalgama_silva.o" "$SILVA_H" -- "${GCC_FLAGS[@]}" || exit 1
compilare ordines "$SILVA_DIR/instrumenta/nexus_ordines.c" \
    "$BUILD_DIR/nexus_ordines.o" "$SILVA_DIR/instrumenta/nexus_ordines.h" \
    "$SILVA_H" -- "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || exit 1
sweep_objs="$obj_files $BUILD_DIR/nexus_amalgama_silva.o $BUILD_DIR/nexus_ordines.o"

# CLI (tabulam legit - silva non tangit; obiecta bibliothecae sola).
# Olim omni quaestione relinkabatur; nunc solum si obiectum mutatum.
CLI_SRC="$SILVA_DIR/instrumenta/principalia/nexus.c"
CLI_OBJ="$BUILD_DIR/nexus_cli.o"
CLI_BIN="$BUILD_DIR/nexus"
compilare cli "$CLI_SRC" "$CLI_OBJ" -- "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || exit 1
if ! recentius "$CLI_BIN" "$CLI_OBJ" $obj_files; then
    source "$RADIX_DIR/tools/nexus_recens.sh"
    nectere_atomice "$CLI_BIN" "${GCC_FLAGS[@]}" "$CLI_OBJ" $obj_files || exit 1
fi

cd "$RADIX_DIR"

# renovatio: sweep corporis (nexus_percursus) - VARIANTE CELERI
# (-O2 -flto, exemplar cursor -celer; 143s -> ~55s mensuratum
# 2026-07-14, PARITAS OCTETIM contra -O0 probata in ambabus
# tabulis). Obiecta celeria in build/celer/ - NUMQUAM mixta cum
# -O0. RUNG 5: modus INCREMENTALIS ordinarius (copia laboris per
# stampam GENERATUM + clausuram reversam graphi; commissio typica
# 1-2s); -plenus vim facit. INSTRUMENTUM NOVUM -> plenus coactum
# (tabula mixta duorum parsatorum numquam nascitur). FUNCTIO
# (2026-07-29): eadem semita et imperio -renovare et sanationi
# quaestionis - dissentire non possunt.
renovatio () {
    CELER_DIR="$BUILD_DIR/celer"
    mkdir -p "$CELER_DIR"
    CELER_FLAGS=("${GCC_FLAGS[@]}" "-O2" "-flto")
    celer_objs=""
    for f in "${RADIX_FONTES[@]}"; do
        compilare "celer dep" "$RADIX_DIR/lib/$f.c" "$CELER_DIR/$f.o" -- \
            "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || return 1
        celer_objs="$celer_objs $CELER_DIR/$f.o"
    done
    compilare "celer amalgama" "$SILVA_DIR/amalgama/silva.c" \
        "$CELER_DIR/amalgama_silva.o" "$SILVA_H" -- "${CELER_FLAGS[@]}" || return 1
    compilare "celer ordines" "$SILVA_DIR/instrumenta/nexus_ordines.c" \
        "$CELER_DIR/nexus_ordines.o" "$SILVA_DIR/instrumenta/nexus_ordines.h" \
        "$SILVA_H" -- "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || return 1
    celer_objs="$celer_objs $CELER_DIR/amalgama_silva.o $CELER_DIR/nexus_ordines.o"
    SWEEP_SRC="$SILVA_DIR/instrumenta/principalia/nexus_percursus.c"
    SWEEP_OBJ="$CELER_DIR/nexus_percursus.o"
    SWEEP_BIN="$CELER_DIR/nexus_percursus"
    compilare "celer percursus" "$SWEEP_SRC" "$SWEEP_OBJ" "$SILVA_H" -- \
        "${CELER_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" || return 1
    plenus_vis=""
    if ! recentius "$SWEEP_BIN" "$SWEEP_OBJ" $celer_objs; then
        source "$RADIX_DIR/tools/nexus_recens.sh"
        nectere_atomice "$SWEEP_BIN" "${CELER_FLAGS[@]}" "$SWEEP_OBJ" \
            $celer_objs || return 1
        # instrumentum novum (obiectum quodvis OCTETIS mutatum):
        # iudicia mutari potuerunt -> plenus (numquam tacite)
        plenus_vis="-plenus"
        echo "  [instrumentum novum -> plenus]" >&2
    fi
    mkdir -p "$RADIX_DIR/build"
    # SERA (2026-09-07): percursus duo simul (unci commissionis
    # sessionum duarum, excubitor, sanatio quaestionis) .nova eandem
    # scriberent et se invicem truncarent. Lex communis tools/sera.sh
    # (tenens mortuus / arbor aliena / aetas derelicta; exspectatio
    # CXX s, deinde renovatio omissa - 1, vocator iudicat).
    local rc
    source "$RADIX_DIR/tools/sera.sh"
    sera_capere "$RADIX_DIR/build/nexus.sera" 120 || return 1
    # $plenus_vis SINE virgulis consulto (vacuum evanescit - bash
    # scissio verborum; tabula vacua sub set -u in bash 3.2 fallit)
    "$SWEEP_BIN" $plenus_vis "$@"
    rc=$?
    sera_dimittere
    return $rc
}

if [ "${1:-}" = "-renovare" ]; then
    shift   # args reliqua (-plenus, -omnia) ad sweep transeunt
    renovatio "$@"
    exit $?
fi

# sanatio quaestionis (01KYQ4H06F): percursus incrementalis ANTE
# responsum - ipse mensura staleness praecisa est (nulla-mutatio
# ~0.4s). -vetus = tabula vetus consulto (fons medio-editu notus).
SANATIO=1
if [ "${1:-}" = "-vetus" ]; then
    SANATIO=0
    shift
fi
TSV="$RADIX_DIR/build/nexus.tsv"
if [ "$SANATIO" = "1" ]; then
    # captura substitutione: percursus fistulam vocatoris numquam
    # tangit - '| head' quaestionem truncans sanationem SIGPIPE
    # occidere non potest (mensuratum in probatione viva)
    if ! SANATIO_ACTA="$(renovatio 2>&1)"; then
        printf '%s\n' "$SANATIO_ACTA" >&2
        echo "CAUTIO GRAVIS: renovatio FRACTA (fons medio-editu?) -" >&2
        echo "               responsum e tabula VETERE" >&2
    elif [ -n "$SANATIO_ACTA" ]; then
        printf '%s\n' "$SANATIO_ACTA" >&2
    fi
else
    # -vetus: cautio vetustatis sola (mores pristini)
    if [ -f "$TSV" ]; then
        RECENTIOR=$(find "$RADIX_DIR" -name '*.c' -newer "$TSV" \
            -not -path '*/build/*' -not -path '*/.git/*' 2>/dev/null | head -1)
        if [ -n "$RECENTIOR" ]; then
            echo "CAUTIO: nexus.tsv vetustior quam $RECENTIOR" >&2
            echo "        (renovare: ./silva/nexus.sh -renovare)" >&2
        fi
    fi
fi

exec "$CLI_BIN" "$@"
