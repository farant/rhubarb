#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/silva.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="silva" tegit="materia/fontes/* silva/*_fontes_generata.sh silva/fontes/* silva/grammatica/* silva/instrumenta/* silva/probationes/* silva/quaestiones.stml" currit="tools/fabrica.c"/>
if [ -z "${SILVA_PROBATIONES_EFFUSIO:-}" ]; then
    export SILVA_PROBATIONES_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../build/test_logs/silva.log"
    exit "${PIPESTATUS[0]}"
fi

# silva/compile_probationes.sh - IANUA probationum silvae (cursores ad
# gradus T5; exemplar toml/compile_probationes.sh). Suita est compositum
# probationes_silva (silva/aedificatio.stml): quaeque probatio actio
# sua, CLAUSURAM SUAM sola nectit, verdictum clavatum iis quae legit.
# Olim cursor omnes fontes silvae et instrumenta in omnem probationem
# nectebat (indices manu, regula capitis recentissimi, excubitor,
# operarii paralleli) - corpus absens (silva_arbor_aequalitas.c) ita
# celabatur. 'bin/fabrica probare' membra stala currit (acta impressa),
# fresca SERVAT ('servata', linea 'servatae N/M' cum causa iudicis).
#
# Usage:
#   ./silva/compile_probationes.sh              # omnes (fresca servata)
#   ./silva/compile_probationes.sh lexema       # quarum titulus "lexema" continet
#   ./silva/compile_probationes.sh -vis [...]   # omnes (electae) iterum
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
sera_capere "$RADIX_DIR/silva/build/cursor.sera" || exit 2
bin/fabrica probare probationes_silva "$@"
exit $?
