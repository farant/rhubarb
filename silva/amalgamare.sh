#!/bin/bash

# silva/amalgamare.sh - Amalgama generare et verificare (dev-time)
#
# Usage:
#   ./amalgamare.sh          # generat amalgama/silva.c + compilat
#
# Compiles the amalgamator (instrumenta/principalia/amalgamator.c +
# the silva lexer it tokenizes with), runs it, then VERIFIES the
# output: standalone compile of amalgama/silva.c under the full flag
# set. Output is COMMITTED (the deliverable artifact).
# Chunk B adds: silva.h + silva module bodies + host-pollution test
# + nm-intersection check.

set -u

SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
BUILD_DIR="$SILVA_DIR/build"
AMALGAMA_DIR="$SILVA_DIR/amalgama"
mkdir -p "$BUILD_DIR" "$AMALGAMA_DIR"

cd "$SILVA_DIR"

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=(
    "-I$RADIX_DIR/include"
    "-I$SILVA_DIR/fontes"
)

# ---- 0. PORTA VETUSTATIS: manifesta derivationi congruere ----
# amalgamare.sh manifesta sua NON regenerat - ex COMMISSIS struit.
# Ergo modulus novus in fontibus (aut vocator novus functionis
# vendicatae) sine regeneratione = amalgama tacite FALSA quae portas
# I-VI omnes transit, quia nihil quod abest quaeritur.
#
# MENSURATUM 2026-08-20 (die quo haec porta nata est): manifestum
# fontium ab 7ba8d84 rancidum iacuerat - postulata_posix.h in
# clausura silvae erat, in manifesto aberat - et nemo id viderat.
# Porta id primo cursu invenit.
#
# Confertio INCONDITA (non per mtime, non 'si plagula nova apparet'):
# excludenda etiam tunc mutantur cum functio vendicata iam nota
# vocatorem novum accipit, quod nulla plagula nova prodit.
#
# Prius omnibus: fractura hic nata est ante quam quicquam struitur,
# ne opus perfectum abiciatur.
# ---- 1. dependency objects (rhubarb libs + the silva lexer) ----
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar" "tabula_dispersa"
    "friatio"
)
# capita mutata sine recompilo = corruptio ABI (classis excubitoris;
# custos tesserae aequatus - desideratum 01KY2P5X)
newest_header () {
    find "$RADIX_DIR/include" "$SILVA_DIR/fontes" \
        "$SILVA_DIR/instrumenta" \
        -name '*.h' -newer "$1" 2>/dev/null | head -1
}

obj_files=""
for f in "${RADIX_FONTES[@]}"; do
    src="$RADIX_DIR/lib/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [dep] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done
for f in "silva_token" "silva_lexema"; do
    src="$SILVA_DIR/fontes/$f.c"
    obj="$BUILD_DIR/$f.o"
    if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ] || [ -n "$(newest_header "$obj")" ]; then
        echo "  [silva] $f.c"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj" || exit 1
    fi
    obj_files="$obj_files $obj"
done

# ---- 1.5 PORTA VETUSTATIS: manifesta derivationi congruere ----
# POST obiecta, ANTE amalgamationem. Non prius: probatio
# excludendorum amalgamatorem NECTIT, qui obiecta calefacta petit -
# in arbore frigida porta vere-prima se ipsam frangeret (mensuratum
# 2026-08-20 in tessera/officina, quorum obiecta mechanismi nondum
# exstabant). Hic tamen adhuc ante opus CARUM stat: amalgamatio,
# compilatio standalone, hospes, nm, censura omnia sequuntur.
# Ratio plena: tools/porta_vetustatis.sh
echo "  [porta 0] vetustas manifestorum"
"$RADIX_DIR/tools/porta_vetustatis.sh" silva "$BUILD_DIR/vetustas" || exit 1

# ---- 2. build + run the amalgamator (manifest + shared mechanism) ----
# Fontes ex POLITICA, non hic enumerati: haec lista et ea scriptoris
# excludendorum idem esse DEBENT, et cum geminae essent, divergerunt
# (vide amalgamatoris_fontes in fontes_politica.sh).
# shellcheck source=/dev/null
. "$SILVA_DIR/instrumenta/principalia/fontes_politica.sh"
FONTES_AMALGAMATORIS=""
for f in $(amalgamatoris_fontes); do
    FONTES_AMALGAMATORIS="$FONTES_AMALGAMATORIS $RADIX_DIR/$f"
done
echo "  [silva] amalgamator.c (fontes ex politica)"
# shellcheck disable=SC2086
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -I"$SILVA_DIR/instrumenta" \
    $FONTES_AMALGAMATORIS $obj_files \
    -o "$BUILD_DIR/amalgamator" || exit 1

"$BUILD_DIR/amalgamator" "$RADIX_DIR" "$AMALGAMA_DIR/silva.c" || exit 1

# ---- 3. VERIFICATIO: standalone compile under the FULL flag set ----
# (no include paths - self-contained; NO unused suppression: the
# manifest exclusion lists ARE the zero-unused guarantee, S44)
echo "  [verificatio] amalgama/silva.c standalone (severitas plena)"
clang "${GCC_FLAGS[@]}" \
    -c "$AMALGAMA_DIR/silva.c" -o "$BUILD_DIR/amalgama_verificatio.o" || exit 1

# ---- 4. VERIFICATIO: hospes (host-pollution S43 + equivalence) ----
# hospes.c is VANILLA C89: includes ONLY silva.h; declares variables
# named si/per/character/nomen/structura/redde/dum/vacuum; runs
# parse->emit fixtures through the amalgam.
echo "  [verificatio] hospes.c (pollutio + aequivalentia)"
clang "${GCC_FLAGS[@]}" -I"$AMALGAMA_DIR" \
    "$SILVA_DIR/instrumenta/principalia/hospes.c" \
    "$BUILD_DIR/amalgama_verificatio.o" \
    -o "$BUILD_DIR/hospes" || exit 1
"$BUILD_DIR/hospes" || exit 1

# ---- 5. VERIFICATIO: intersectio nm (symbola communia = 0) ----
# The amalgam's externals must not collide with rhubarb's own
# objects (the sim (4) measurement, enforced forever).
nm -gU "$BUILD_DIR/amalgama_verificatio.o" | awk '{print $3}' | sort > "$BUILD_DIR/syms_amalgama.txt"
: > "$BUILD_DIR/syms_rhubarb.txt"
for f in "${RADIX_FONTES[@]}"; do
    nm -gU "$BUILD_DIR/$f.o" 2>/dev/null | awk '{print $3}' >> "$BUILD_DIR/syms_rhubarb.txt"
done
sort -o "$BUILD_DIR/syms_rhubarb.txt" "$BUILD_DIR/syms_rhubarb.txt"
COMMUNIA=$(comm -12 "$BUILD_DIR/syms_amalgama.txt" "$BUILD_DIR/syms_rhubarb.txt" | wc -l | tr -d ' ')
if [ "$COMMUNIA" != "0" ]; then
    echo "amalgamare: FRACTA - $COMMUNIA symbola communia cum rhubarb:"
    comm -12 "$BUILD_DIR/syms_amalgama.txt" "$BUILD_DIR/syms_rhubarb.txt" | head -10
    exit 1
fi

# ---- 6. CENSURA: censor latinae (instrumenta prima; DECISUS
# 2026-07-10: in portis compilationis a die primo - plagulae
# recentes sunt ubi flagrationes accenduntur) ----
CENSOR="$RADIX_DIR/silva/censor.sh"
# (a) corpus regressionis: fixturae flagrare DEBENT (XIII benedictae)
CENSURA_PROBA=$("$CENSOR" \
    "$SILVA_DIR/probationes/fixa/censoris/flagrationes.c" \
    "$SILVA_DIR/probationes/fixa/censoris/fragores.c.fragor" 2>/dev/null)
CENSURA_EXITUS=$?
CENSURA_NUMERUS=$(echo "$CENSURA_PROBA" | grep -c '\[CENSURA\]')
if [ "$CENSURA_EXITUS" != "1" ] || [ "$CENSURA_NUMERUS" != "13" ]; then
    echo "amalgamare: FRACTA - corpus censoris non flagrat ut benedictum (exitus $CENSURA_EXITUS, $CENSURA_NUMERUS/13)"
    exit 1
fi
# (a-bis) classis B: culpae PROBABILES in fragoribus (VI benedictae,
# una per formam historicam - nomen/registrum/magnitudo/structura/
# commutatio/externus). Sine hoc pino convictio tacite mori posset:
# ordines [CULPA PROBABILIS] exitum non mutant (classis B consulto),
# ergo nulla porta alia eos videret.
CULPAE_NUMERUS=$(echo "$CENSURA_PROBA" | grep -c 'CULPA PROBABILIS')
if [ "$CULPAE_NUMERUS" != "6" ]; then
    echo "amalgamare: FRACTA - censor culpas probabiles non nominat ($CULPAE_NUMERUS/6)"
    exit 1
fi
# (b) fontes silvae puri sunto (instrumenta/*.c = moduli communes -
# silva_amalgama, silva_lexicon... - eadem porta ac principalia)
if ! "$CENSOR" "$SILVA_DIR"/fontes/*.c "$SILVA_DIR"/fontes/*.h \
        "$SILVA_DIR"/instrumenta/*.c \
        "$SILVA_DIR"/instrumenta/principalia/*.c \
        "$SILVA_DIR"/probationes/probatio_*.c \
        > "$BUILD_DIR/censura.txt" 2>/dev/null; then
    echo "amalgamare: CENSURA LATINAE FRACTA:"
    grep '\[CENSURA\]\|\[SINE ARBORE\]' "$BUILD_DIR/censura.txt" | head -10
    exit 1
fi

echo "amalgamare: VERIFICATUM (standalone + hospes + nm-intersectio 0 + censura)"
