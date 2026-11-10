#!/bin/bash
# generare.sh - fixa exemplariorum (exemplaria-velox T1): imagines
# Frani ad DC latitudinis per 'sips' diminutae, deinde CONGELATAE.
#
#   lapis_600.png   golem lapideus (stone-golem.png, MDCCLVI x MMCC)
#                   -> DC x DCCLI; quadro animato 3D proximus
#   pratum_600.png  pratum photographicum (test-image.png, MDXXXVI x
#                   MMXLVIII) -> DC x DCCC
#
# Fontes in ~/Downloads Frani, non in arbore: fixa ipsa commissa sunt,
# hoc scriptum solum narrat unde venerint (non regeneratur).
set -eu
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
sips --resampleWidth 600 "$HOME/Downloads/stone-golem.png" \
    --out "$DIR/lapis_600.png" > /dev/null
sips --resampleWidth 600 "$HOME/Downloads/test-image.png" \
    --out "$DIR/pratum_600.png" > /dev/null
sips -g pixelWidth -g pixelHeight "$DIR/lapis_600.png" "$DIR/pratum_600.png"
