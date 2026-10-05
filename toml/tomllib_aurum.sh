#!/bin/bash
# toml/tomllib_aurum.sh - aurea oraculi tomllib generare (planum
# toml-arbor Q1). DUO exitus:
#   toml/probationes/fixa/tomllib/aurum.txt   toml-test 1.0.0 (COMMISSUM;
#       ex datis MIT derivatum)
#   toml/build/aurum_silvestre.txt            corpus silvestre (NON
#       commissum: contentum alienum ut JSON - decisio Frani 2026-09-28,
#       manifestum solum; ex plagulis vivis regeneratur ut porta eas legit)
#
# Usus: ./toml/tomllib_aurum.sh [-silvestre]   (-silvestre: solum
#       aureum silvestre, ut porta Q9 regeneret)
# Clausula: '#### <via>' / '## STATUS: VALIDUM|INVALIDUM|ABEST' /
# [JSON signatum una linea | '## CAUSA: ...'] / '## FINIS'.
# Viae toml-test relativae ad fixa/toml-test/tests; silvestres cum '~'.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
FIXA="$RADIX/toml/probationes/fixa"
HARNESS="$RADIX/toml/instrumenta/tomllib_tagatum.py"
_caput () {
    echo "# $1 - GENERATUM a toml/tomllib_aurum.sh - NE MANU EDITES"
    if [ "${2:-}" = "-sine-die" ]; then
        echo "# $(python3 --version 2>&1) tomllib"
    else
        echo "# $(python3 --version 2>&1) tomllib; $(date -u +%Y-%m-%d)"
    fi
}
if [ "${1:-}" != "-silvestre" ]; then
    mkdir -p "$FIXA/tomllib"
    EXITUS="$FIXA/tomllib/aurum.txt"
    {
        _caput aurum.txt
        echo "# toml-test: fixa/toml-test (PROVENIENTIA.md), index files-toml-1.0.0"
        grep '\.toml$' "$FIXA/toml-test/tests/files-toml-1.0.0" \
            | python3 "$HARNESS" -aurum "$FIXA/toml-test/tests"
    } > "$EXITUS" || exit 1
    # <tolera codex="lint:effectus-irresolutum" (>exitum suum modo scriptum numerat (nuntius solus)
    echo "aurum toml-test: $(grep -c '^####' "$EXITUS") clausulae -> $EXITUS"
fi
# aurum silvestre = actio fabricae 'toml_aurum_silvestre' (fabrica spec 3
# T4): octeti DETERMINISTICI (sine die - aliter regeneratio cotidie
# 'mutatum' iudicaret et vestigium portae toml cotidie moveret); sub
# FABRICA_SCRIPTURA (iudicium -plenus) in scripturam scribit
EXITUS="toml/build/aurum_silvestre.txt"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
else
    EXITUS="$RADIX/$EXITUS"
fi
mkdir -p "$(dirname "$EXITUS")"
{
    _caput aurum_silvestre.txt -sine-die
    echo "# silvestria.manifestum (plagulae vivae; sigillum in porta iudicatur)"
    grep -v '^#' "$FIXA/silvestria.manifestum" | cut -f2 \
        | python3 "$HARNESS" -aurum "$RADIX"
} > "$EXITUS" || exit 1
# <tolera codex="lint:effectus-irresolutum" (>exitum suum modo scriptum numerat (nuntius solus)
echo "aurum silvestre: $(grep -c '^####' "$EXITUS") clausulae -> $EXITUS"
