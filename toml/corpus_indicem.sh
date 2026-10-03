#!/bin/bash
# toml/corpus_indicem.sh - index corporis domus pro porta corporis toml:
# omnis .toml tracta extra fixa clientis, viae radici relativae ->
# build/toml_corpus.lst (probatio_toml_corpus et toml_corpus_ambulare id
# legunt; lista absens = CREDO_CULPA).
#
# Actio fabricae 'toml_corpus' (toml/aedificatio.stml; fabrica spec 3
# T4): olim cursor id per bash scribebat, et vestigium portae lectionem
# indicis sine scriptore declarato ferebat ('ingressus build/ sine
# domino'). Sub FABRICA_SCRIPTURA (iudicium -plenus) in scripturam
# scribit, numquam in arborem.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXITUS="build/toml_corpus.lst"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
else
    EXITUS="$RADIX/$EXITUS"
fi
mkdir -p "$(dirname "$EXITUS")" || exit 1
TRACTAE="$(git -C "$RADIX" ls-files '*.toml')" || exit 1
# grep sine paribus exitum 1 reddit: index vacuus licet, non fractum
printf '%s\n' "$TRACTAE" | grep -v '^toml/probationes/fixa/' > "$EXITUS" || :
