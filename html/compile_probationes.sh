#!/bin/bash
# <cursus titulus="html/compile_probationes.sh" versio="I"/>
# CHASSIS CURSUUM (analytica T6): cursus in annalibus
source "$(dirname "${BASH_SOURCE[0]}")/../tools/cursus.sh"

# Effusio cursus ultimi semper in build/test_logs/html.log
# radicis (stdout + stderr; codex exitus per PIPESTATUS servatus).
# <porta titulus="html" tegit="html/fontes/* html/grammatica/* html/probationes/* materia/fontes/* md/fontes/*" currit="tools/fabrica.c"/>
if [ -z "${HTML_PROBATIONES_EFFUSIO:-}" ]; then
    export HTML_PROBATIONES_EFFUSIO=1
    # effusio in plagula TEMPORARIA extra arborem, post cursum in locum
    # movetur: bin/fabrica arborem totam ante et post actionem
    # photographat - log in arbore crescens actionem 'scripsit extra
    # vestigium' faceret (frigida b8ea30f0: md_corpus, toml_corpus,
    # aedilis FRACTA, cursores ad gradus T7)
    mkdir -p "$(dirname "$0")/../build/test_logs"
    _effusio="$(mktemp "${TMPDIR:-/tmp}/html.log.XXXXXX")"
    "$0" "$@" 2>&1 | tee "$_effusio"
    _rc="${PIPESTATUS[0]}"
    mv -f "$_effusio" "$(dirname "$0")/../build/test_logs/html.log"
    exit "$_rc"
fi

# html/compile_probationes.sh - IANUA probationum html (cursores ad
# gradus, expeditio ...W0ZBW 2026-10-14; exemplar css). Suita est
# compositum probationes_html (html/aedificatio.stml): quaeque probatio actio sua,
# verdictum clavatum iis quae legit. 'bin/fabrica probare' membra stala
# currit (acta impressa), fresca SERVAT ('servata', linea 'servatae N/M'
# cum causa iudicis) - eadem vocatio ac porta (silva.py).
#
# Usage:
#   ./html/compile_probationes.sh              # omnes (fresca servata)
#   ./html/compile_probationes.sh selector     # quarum titulus "selector" continet
#   ./html/compile_probationes.sh -vis [...]   # omnes (electae) iterum
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
sera_capere "$RADIX_DIR/html/build/cursor.sera" || exit 2
bin/fabrica probare probationes_html "$@"
exit $?
