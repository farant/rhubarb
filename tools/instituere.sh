#!/bin/bash
# tools/instituere.sh - binarium in ~/.bin (in PATH) instituere
# (fabrica 1b T5: copia actio sua iudicata, 'institutio_X')
#
# rm ante cp: macOS binarium superscriptum SIGKILL dat (signatura per
# vnode condita) - inodus recens obligatorius. Copia fracta exitum 1
# reddit (olim 'cp' sine custodia: exitus 0 et 'institutum' dictum).
# FABRICA_SCRIPTURA (iudex): copia in scripturam, nomen basis idem.
#
# Usus: ./tools/instituere.sh bin/X
# Exitus: 0 institutum · 1 fractum · 2 usus.
set -u
B="${1:?usus: instituere.sh bin/X}"
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ -f "$B" ] || { echo "instituere: $B abest" >&2; exit 1; }
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    DEST="$FABRICA_SCRIPTURA"
else
    DEST="$HOME/.bin"
fi
mkdir -p "$DEST" || exit 1
T="$DEST/$(basename "$B")"
rm -f "$T" || exit 1
cp "$B" "$T" || { echo "instituere: copia fracta: $T" >&2; exit 1; }
echo "institutum: $T"
