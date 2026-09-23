#!/bin/sh
# stml_aurea_generare.sh - aurum pontis stml_textus_internus
# (re)generare. Vide probationes/probatio_stml_aurea.c pro doctrina:
# aurum est TESTIMONIUM moris veteris - numquam regenerare sine
# iudicio divergentiarum (diff .aurum .novum) contra spec
# project-specs/stml-trivia-spec.md.
#
# CORPUS CONGELATUM (2026-09-23): fontes = COPIAE in
# probationes/fixa/stml_aurea/fontes/ (via originalis + '.fons', ne
# ambulatores domus '*.stml' eas videant). Olim 'find .' super arbore
# VIVA: omnis editio fontis portam radicis rubebat, et XCIV e CXLVIII
# fontibus NON TRACTATI erant (officina/build/census aedificata,
# stml-parser-reference ignorata) - clonus recens rubebat. Aurum
# instantanea est; instantanea input congelatum postulat (doctrina
# 01M365VXAF). Corpus renovare = copias consulto renovare + aurum
# regenerare, UNA commissione, causa nominata.
set -e
cd "$(dirname "$0")/.."

BIN=build/probationes/probatio_stml_aurea
if [ ! -x "$BIN" ]; then
    echo "probatio_stml_aurea non aedificata - curre:" >&2
    echo "  ./compile_tests.sh stml_aurea" >&2
    exit 2
fi

mkdir -p probationes/fixa/stml_aurea

FONTES_DIR=probationes/fixa/stml_aurea/fontes
VIAE=$(find "$FONTES_DIR" -name '*.fons' | LC_ALL=C sort)

if [ -z "$VIAE" ]; then
    echo "nullae copiae fontium in $FONTES_DIR - aliquid valde mali" >&2
    exit 2
fi

# shellcheck disable=SC2086
exec "$BIN" -generare $VIAE
