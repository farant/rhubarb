#!/bin/bash
# <cursus titulus="md/compile_probationes.sh" versio="I"/>
# CHASSIS CURSUUM (analytica T6): cursus in annalibus
source "$(dirname "${BASH_SOURCE[0]}")/../tools/cursus.sh"

# Effusio cursus ultimi semper in build/test_logs/md.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="md" tegit="materia/fontes/* md/fontes/* md/grammatica/* md/html/* md/probationes/*" currit="tools/fabrica.c" translatio="numquam"/>
if [ -z "${MD_PROBATIONES_EFFUSIO:-}" ]; then
    export MD_PROBATIONES_EFFUSIO=1
    # effusio in plagula TEMPORARIA extra arborem, post cursum in locum
    # movetur: bin/fabrica arborem totam ante et post actionem
    # photographat - log in arbore crescens actionem 'scripsit extra
    # vestigium' faceret (frigida b8ea30f0: md_corpus, toml_corpus,
    # aedilis FRACTA, cursores ad gradus T7)
    mkdir -p "$(dirname "$0")/../build/test_logs"
    _effusio="$(mktemp "${TMPDIR:-/tmp}/md.log.XXXXXX")"
    "$0" "$@" 2>&1 | tee "$_effusio"
    _rc="${PIPESTATUS[0]}"
    mv -f "$_effusio" "$(dirname "$0")/../build/test_logs/md.log"
    exit "$_rc"
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
# SERA CURSORIS (tools/sera.sh; porta sera VIII): cursus duo eiusdem
# suitae - secundus exspectat (ad SERA_TECTUM), deinde exitus 2. Sine
# 'exec': trap EXIT seram dimittit (exec eum perderet, sera vetus
# maneret)
source "$RADIX_DIR/tools/sera.sh"
sera_capere "$RADIX_DIR/md/build/cursor.sera" || exit 2
bin/fabrica probare probationes_md "$@"
exit $?
