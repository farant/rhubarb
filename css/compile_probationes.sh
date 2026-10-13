#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/css.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="css" tegit="css/fontes/* css/grammatica/* css/probationes/* materia/fontes/*" currit="tools/fabrica.c"/>
if [ -z "${CSS_PROBATIONES_EFFUSIO:-}" ]; then
    export CSS_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/css.log"
    exit "${PIPESTATUS[0]}"
fi

# css/compile_probationes.sh - IANUA probationum css (cursores ad gradus
# T3; exemplar toml/compile_probationes.sh). Suita est compositum
# probationes_css (css/aedificatio.stml): quaeque probatio actio sua,
# verdictum clavatum iis quae legit. 'bin/fabrica probare' membra stala
# currit (acta impressa), fresca SERVAT ('servata', linea 'servatae N/M'
# cum causa iudicis) - eadem vocatio ac porta (silva.py).
#
# Usage:
#   ./css/compile_probationes.sh              # omnes (fresca servata)
#   ./css/compile_probationes.sh selector     # quarum titulus "selector" continet
#   ./css/compile_probationes.sh -vis [...]   # omnes (electae) iterum
#
# Exitus: 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum sine paribus,
# fabrica occupata aut declarationes fractae).

set -u
RADIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX_DIR" || exit 2
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >&2 \
    || { echo "FRACTA: bin/fabrica struere nequit - NULLA PROBATIO CURSA"; exit 2; }
exec bin/fabrica probare probationes_css "$@"
