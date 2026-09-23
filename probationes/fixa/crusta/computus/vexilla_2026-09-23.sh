#!/bin/bash
# tools/vexilla.sh - vexilla compilatoris C89 domus: UNA sedes, fontanda
#
# Ante 2026-09-02 LVII scripta (cursores, involucra instrumentorum,
# struere) tabulam suam GCC_FLAGS ferebant - tres variantes silentes
# (sine -fbracket-depth, sine -Wno-overlength-strings) et una cum -O2
# (run_benchmark.sh). Receptum LVII vicibus copiatum = classis quae
# membrum videbatur (amalgamatio.worklog). Nunc:
#
#   source "$RADIX_DIR/tools/vexilla.sh"
#   declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")          # forma ordinaria
#   declare -a GCC_FLAGS=("${VEXILLA_C89[@]}" "-O2")    # benchmark
#
# Superpositum trium variantium: -fbracket-depth=512 limitem solum
# tollit, -Wno-overlength-strings monitum pedanticum solum tacet -
# neutrum aedificationem mutare potest. Gradus optimizationis hic
# ponitur (una linea, una commissio, mensurata: stml.c parsura 148 ->
# 53 ms sub -O2; -Werror mundum super radicem et silvam probatum).
# Porta: ./tools/vexilla_fumus.sh (tabula vera, scripta sine copia).
#
# CLAUDE.md radicis tabulam brevem exhibet (sine -std=c89 etc.) -
# haec est tabula VERA (memoria: 'the real compile flags').

declare -a VEXILLA_C89=(
    "-std=c89"
    "-pedantic"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wcast-qual"
    "-Wstrict-prototypes"
    "-Wmissing-prototypes"
    "-Wwrite-strings"
    "-Wno-long-long"
    "-Wno-overlength-strings"
    "-fbracket-depth=512"
    "-O2"
    "-g"
)

# SIGILLUM VEXILLORUM: cursores obiecta per mtime renovant (fons aut
# caput recentius obiecto), ergo vexillum mutatum SOLUM nihil
# recompilaret - suita obiecta -O0 vetera curreret et 'viridis'
# diceret. Hic: hash tabulae in build/vexilla.sigillum; mutatus ->
# include/latina.h tangitur (contentum intactum; omnis plagula domus
# eam includit), ergo omnis cache mtime domus semel frigescit. Pretium
# = una aedificatio frigida per mutationem vexillorum, non tacita
# mendacia.
_vexilla_radix="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
_vexilla_hash="$(printf '%s\n' "${VEXILLA_C89[@]}" | cksum | cut -d' ' -f1)"
_vexilla_sigillum="$_vexilla_radix/build/vexilla.sigillum"
if [ ! -f "$_vexilla_sigillum" ] || [ "$(cat "$_vexilla_sigillum" 2>/dev/null)" != "$_vexilla_hash" ]; then
    mkdir -p "$_vexilla_radix/build"
    printf '%s\n' "$_vexilla_hash" > "$_vexilla_sigillum"
    touch "$_vexilla_radix/include/latina.h"
    echo "vexilla: tabula mutata (sigillum $_vexilla_hash) - latina.h tacta, aedificatio frigida sequitur" >&2
fi
