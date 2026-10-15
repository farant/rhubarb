#!/bin/bash
# oratio/vocabula.sh - recensio vocabulorum: identificatores et commentaria (Latine, T10) aut prosa markdown (Anglice, T15a) contra vocabularia
#
# Usage:  ./oratio/vocabula.sh [-symbola | -commenta | -omnia | -prosa | -nova] [-machina] [-omnes] [-omnes-viae] [-tectum N]
#         ./oratio/vocabula.sh -iudicandae VIA...   (viae quas lint iudicat - iudex lint_latinus, unitates.stml)
#         -nova = verba ignota NOVA contra copiam toleratam cum exitibus (ante commissionem; exitus 1 si adsunt)
# Exit:   0 sanum | 1 fractum | 2 usus/plagula absens
# Instrumentum ex clausura sua; binarium in oratio/build/vocabula.
set -u
ORATIO_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$ORATIO_DIR/.." && pwd)"
BUILD_DIR="$ORATIO_DIR/build"
# SERA suitae orationis (tools/sera.sh, 2026-09-07): obiecta et
# binaria in oratio/build sub cursore currenti non tanguntur; cursor
# ab hoc vocatus reentrat; exitus 2 si tenta post SERA_TECTUM.
mkdir -p "$BUILD_DIR"
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$BUILD_DIR/cursor.sera" || exit 2
BIN="$BUILD_DIR/vocabula"
SRC="$ORATIO_DIR/instrumenta/vocabula.c"
# Instrumentum ex clausura SUA (tools/cursor_communis.sh, 2026-10-14):
# olim obiecta oratio/build/*.o ligabantur quae cursor ULTIMO struxerat
# (vetustas nulla iudicata - instrumentum contra fontes veteres)
source "$RADIX_DIR/tools/cursor_communis.sh" || exit 2
cursor_instrumenta_parare >&2 || exit 2
cursor_instrumentum_struere "$SRC" "$BIN" >&2 || exit 2
git -C "$RADIX_DIR" ls-files "lib/*.c" "silva/fontes/*.c" > "$BUILD_DIR/corpus_c.txt"
git -C "$RADIX_DIR" ls-files "*.c" "*.h" > "$BUILD_DIR/corpus_tractatae.txt"
# VOCABULA_VIAE_ADDITAE="a.c b.h": viae COMMISSIONIS ipsius (lint
# praevium silva.commissio, uncus) tractatae numerantur etiam nondum
# tractae - verba plagulae NOVAE ante portas iudicantur (desideratum
# ...HYDS: olim filtrum 'tractatae' eas abiciebat et solus uncus post
# portas eas videbat). Opus alienae sessionis non nominatum non numeratur.
for v in ${VOCABULA_VIAE_ADDITAE:-}; do
    printf '%s\n' "${v#./}" >> "$BUILD_DIR/corpus_tractatae.txt"
done
git -C "$RADIX_DIR" ls-files "*.md" > "$BUILD_DIR/corpus_md.txt"
# INDEX RENOVATUR ANTE IUDICIUM (circulus commissionis P2, desideratum
# 01M46ER35): modi qui identificatores legunt (omnes praeter -commenta
# et -prosa) build/nexus.tsv per percursum incrementalem (~I s) primum
# sanant - olim index stalus 'NOVA 0' mendax post editionem dabat.
# VOCABULA_INDEX_RENOVATUS=1: vocans (uncus) id modo fecit.
renovandum=1
for a in "$@"; do
    case "$a" in -commenta|-prosa|-iudicandae) renovandum=0 ;; esac
done
if [ "$renovandum" = 1 ] && [ "${VOCABULA_INDEX_RENOVATUS:-0}" != 1 ]; then
    mkdir -p "$RADIX_DIR/build"
    if ! "$RADIX_DIR/silva/nexus.sh" -renovare > "$RADIX_DIR/build/vocabula_nexus.log" 2>&1; then
        echo "vocabula: index symbolorum NON renovatus (build/vocabula_nexus.log) - iudicium staleo indice mentiretur" >&2
        exit 2
    fi
fi
# non exec: crusta manet ut sera suitae usque ad finem instrumenti
# teneatur et in exitu dimittatur (exec trap EXIT praeterit -
# sera cum pid mortuo relicta, 2026-09-07)
RHUBARB_RADIX="$RADIX_DIR" "$BIN" "$@"
exit $?
