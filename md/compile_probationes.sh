#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/md.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="md" tegit="materia/fontes/* md/fontes/* md/grammatica/* md/html/* md/probationes/*" currit="tools/fabrica.c"/>
if [ -z "${MD_PROBATIONES_EFFUSIO:-}" ]; then
    export MD_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/md.log"
    exit "${PIPESTATUS[0]}"
fi

# md/compile_probationes.sh - IANUA probationum md (cursores ad gradus
# T4; exemplar toml/compile_probationes.sh). Suita est compositum
# probationes_md (md/aedificatio.stml): quaeque probatio actio sua,
# verdictum clavatum iis quae legit; index corporis (md/build/
# corpus_md.txt) per actionem md_corpus. Olim indices fontium manu
# scripti, regula 'caput recentissimum' (mtime) et custodia excubitoris.
# 'bin/fabrica probare' membra stala currit (acta impressa), fresca
# SERVAT ('servata', linea 'servatae N/M' cum causa iudicis).
#
# Usage:
#   ./md/compile_probationes.sh              # omnes (fresca servata)
#   ./md/compile_probationes.sh lexema       # quarum titulus "lexema" continet
#   ./md/compile_probationes.sh -vis [...]   # omnes (electae) iterum
#
# Exitus: 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum sine paribus,
# fabrica occupata aut declarationes fractae).

set -u
RADIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX_DIR" || exit 2
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >&2 \
    || { echo "FRACTA: bin/fabrica struere nequit - NULLA PROBATIO CURSA"; exit 2; }
exec bin/fabrica probare probationes_md "$@"
