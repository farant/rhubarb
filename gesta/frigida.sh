#!/bin/bash

# gesta/frigida.sh - via scripturae frigida (residens absens, aut
# vocans qui MCP loqui nequit: uncus commissi, pythonica).
# nota_frigida construit et currit. Usus:
#   ./gesta/frigida.sh <res|titulus> <textus...>
#   ./gesta/frigida.sh -crea <genus> <titulus> [textus]
#   ./gesta/frigida.sh [-actor A] [-origo O] -status  <res> <novus>
#   ./gesta/frigida.sh [-actor A] [-origo O] -mutatio <res> <clavis> <valor>
#   ./gesta/frigida.sh [-actor A] [-origo O] -nexus   <res> <verbum> <alterum>
#   ./gesta/frigida.sh -res <res>          (LECTIO: breviarium, nihil scribit)
#   ./gesta/frigida.sh -mappa               (LECTIO: arbor regionum, nomina sola - ut 'tree')
#   ./gesta/frigida.sh -mappa-plena         (LECTIO: arbor cum '<visio (>' et '<principium (>' lineis)
#   ./gesta/frigida.sh -inventarium <res>   (LECTIO: cellae in forma machinae - ordo\tlens\tgenus\tvalor)
# Exitus: 0 scriptum | 1 recusatum (nihil scriptum) | 2 usus

set -u

GESTA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$GESTA_DIR/.." && pwd)"
BUILD_DIR="$GESTA_DIR/build"
BIN="$BUILD_DIR/nota_frigida"
mkdir -p "$BUILD_DIR" "$GESTA_DIR/annales"

# obiecta per launcher servi aedificantur (eadem vexilla; </dev/null
# = servus statim EOF exit 0 post constructionem, scrinium intactum)
"$GESTA_DIR/tabularium.sh" </dev/null >/dev/null 2>&1 || true

# vexilla: tools/vexilla.sh (una sedes; LVII copiae olim, 2026-09-02)
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

main_src="$GESTA_DIR/instrumenta/nota_frigida.c"

# INDEX OBIECTORUM DERIVATUS (2026-09-21; desideratum 01KYPZ4T6J).
# Olim hic index MANU scriptus stabat, et MENTITUS EST: tabularium.c
# 'processus' adhibere coepit (renovare, 2026-07-29), index id
# nescivit, et via frigida NEXU FRACTO iacuit duos menses - nemine
# sciente, quia porta nulla eam currebat. Nunc: index fontium radicis
# AB AEDILE generatus (idem quem launcher servi legit) + omnes fontes
# gestae. Clausura huius instrumenti intra clausuram servi iacet.
source "$GESTA_DIR/tabularium_fontes_generata.sh"
obj_files="$BUILD_DIR/sqlite3.o"
for f in "${RADIX_FONTES[@]}"; do
    obj_files="$obj_files $BUILD_DIR/$(basename "$f").o"
done
for f in "$GESTA_DIR"/fontes/*.c; do
    obj_files="$obj_files $BUILD_DIR/$(basename "$f" .c).o"
done

obj_recentius=""
for o in $obj_files; do
    if ! [ "$BIN" -nt "$o" ]; then obj_recentius="$o"; break; fi
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$main_src" ] || [ -n "$obj_recentius" ]; then
    echo "  [nexus] nota_frigida" >&2
    clang "${GCC_FLAGS[@]}" -I"$RADIX_DIR/include" -I"$GESTA_DIR/fontes" "$main_src" $obj_files -o "$BIN" >&2 || exit 1
fi

cd "$RADIX_DIR" && exec "$BIN" "$@"
