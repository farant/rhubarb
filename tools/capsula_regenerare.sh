#!/bin/bash
# tools/capsula_regenerare.sh - capsulam ex TOML regenerare (plan 1a T5)
#
# bin/capsula_generare exitum IUXTA plagulam TOML scribit (capsula_X.{c,h}
# in directorio eius). Sub FABRICA_SCRIPTURA (bin/fabrica -plenus):
# directorium TOML in scripturam speculatur (viae eaedem), generator
# ibi currit - arbor intacta. Sine eo: regeneratio in loco, ut olim.
#
# Usus: ./tools/capsula_regenerare.sh <via/ad/X.toml>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
RADIX="$(pwd)"
TOML="${1:?usus: capsula_regenerare.sh <via.toml>}"
[ -f "$TOML" ] || { echo "capsula_regenerare: $TOML deest" >&2; exit 1; }
[ -x bin/capsula_generare ] || ./compile_tools.sh capsula_generare >/dev/null \
    || { echo "capsula_regenerare: capsula_generare non structum" >&2; exit 1; }
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    DIR="$(dirname "$TOML")"
    mkdir -p "$FABRICA_SCRIPTURA/$DIR" || exit 1
    cp -R "$DIR/." "$FABRICA_SCRIPTURA/$DIR/" || exit 1
    cd "$FABRICA_SCRIPTURA" || exit 1
fi
exec "$RADIX/bin/capsula_generare" "$TOML" >/dev/null
