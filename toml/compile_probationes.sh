#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/toml.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="toml" tegit="materia/fontes/* toml/*.sh toml/aedificatio.stml toml/fontes/* toml/grammatica/* toml/instrumenta/* toml/probationes/*" currit="tools/fabrica.c"/>
if [ -z "${TOML_PROBATIONES_EFFUSIO:-}" ]; then
    export TOML_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/toml.log"
    exit "${PIPESTATUS[0]}"
fi

# toml/compile_probationes.sh - IANUA probationum toml (cursores ad gradus
# T2, Franus 2026-10-12: ianua tenuis manet). Suita est compositum
# probationes_toml (toml/aedificatio.stml): quaeque probatio actio sua,
# verdictum clavatum iis quae legit. 'bin/fabrica probare' membra stala
# currit (acta impressa), fresca SERVAT ('servata', linea 'servatae N/M'
# cum causa iudicis) - eadem vocatio ac porta (silva.py).
#
# Usage:
#   ./toml/compile_probationes.sh              # omnes (fresca servata)
#   ./toml/compile_probationes.sh lector       # quarum titulus "lector" continet
#   ./toml/compile_probationes.sh -vis [...]   # omnes (electae) iterum
#
# Exitus: 0 sanum, 1 fractae, 2 NULLA CURSA (filtrum sine paribus,
# fabrica occupata aut declarationes fractae).

set -u
RADIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX_DIR" || exit 2
[ -x bin/fabrica ] || ./tools/fabrica_struere.sh >&2 \
    || { echo "FRACTA: bin/fabrica struere nequit - NULLA PROBATIO CURSA"; exit 2; }
# SERA CURSORIS (tools/sera.sh; porta sera VIII): cursus duo eiusdem
# suitae - secundus exspectat (ad SERA_TECTUM), deinde exitus 2. Sine
# 'exec': trap EXIT seram dimittit (exec eum perderet, sera vetus
# maneret)
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$RADIX_DIR/toml/build/cursor.sera" || exit 2
bin/fabrica probare probationes_toml "$@"
exit $?
