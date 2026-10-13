#!/bin/bash
# md/corpus_indicem.sh - index corporis markdown pro probationibus md:
# omnis .md a git tracta, viae radici relativae -> md/build/corpus_md.txt
# (probationes arbor, canon, lexema, stml id legunt; lista absens =
# CREDO_CULPA).
#
# Actio fabricae 'md_corpus' (md/aedificatio.stml; cursores ad gradus
# T4, exemplar toml/corpus_indicem.sh): olim cursor vetus id per bash
# scribebat. Sub FABRICA_SCRIPTURA (iudicium -plenus) in scripturam
# scribit, numquam in arborem.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXITUS="md/build/corpus_md.txt"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
else
    EXITUS="$RADIX/$EXITUS"
fi
mkdir -p "$(dirname "$EXITUS")" || exit 1
# <tolera codex="lint:effectus-mandatum-ignotum" (>index git: hoc scriptum generator actionis md_corpus est - exitus omni iudicio regeneratur (fabrica spec 3 par. III.4)
git -C "$RADIX" ls-files '*.md' > "$EXITUS"
