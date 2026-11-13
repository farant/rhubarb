#!/bin/bash
# tools/psalmodia_oraculum.sh - porta 'psalmodia-oraculum' (psalmodia
# plan-2 T8; mos musicxml_silvestria.sh)
#
# Corpus GregoBase (bacor v0.4, CC0; ./tools/gabc_arcessere.sh, in
# build/, numquam commissum) contra tabulam tonorum et aptatorem:
# probatio_psalmodia_corpus cum PSALMODIA_CORPUS - syllabae, EUOUAE,
# psalmi scripti; discordiae cum manifesto commisso
# (probationes/fixa/psalmodia/discordiae.tsv) conferuntur: linea nova
# aut evanida = FRACTA (decretum Frani 5: tabula numquam ad corpus
# flectitur). Corpus absens = OMISSA (numquam silentio sana).
#
# SUB CUSTODE (tools/custos.sh): limes IV GB (cursus primus 2026-11-13:
# apex 143 MB, 11 s - plagula quaeque piscinam suam habet).
#
# Usus:     ./tools/psalmodia_oraculum.sh
# Effusio:  'PSALMODIA ORACULUM: sana' | 'FRACTA ...' | 'OMISSA: ...'
# Discordiae hodiernae: build/psalmodia/discordiae_<genus>.tsv
# <porta titulus="psalmodia-oraculum" tegit="tools/psalmodia_oraculum.sh probationes/fixa/psalmodia/discordiae.tsv musica/toni_psalmorum.stml" currit="probationes/probatio_psalmodia_corpus.c lib/psalmodia.c lib/psalmodia_tabula.c lib/syllabae_latinae.c lib/gabc.c"/>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
CORPUS="$PWD/build/gabc/bacor_v0.4/x"
if [ ! -d "$CORPUS/gabc" ]; then
    echo "PSALMODIA ORACULUM: OMISSA: corpus abest ($CORPUS; ./tools/gabc_arcessere.sh)"
    exit 0
fi
mkdir -p build/psalmodia
ACTA="build/psalmodia/oraculum_acta.log"
CUSTOS_MEMORIA_MB="${CUSTOS_MEMORIA_MB:-4096}" CUSTOS_MORA="${CUSTOS_MORA:-1800}" \
PSALMODIA_CORPUS="$CORPUS" \
    ./tools/custos.sh -- ./compile_tests.sh probatio_psalmodia_corpus > "$ACTA" 2>&1
rc=$?
# puncta credo eadem linea praecedunt: tolluntur
sed 's/^\.*//' "$ACTA" | grep -E "^  (syllabae|euouae|psalmi|psalmus):|NOVA:|SOLUTA|manifestum abest|CUSTOS:"
if grep -q "memoria excessa\|mora excessa" "$ACTA"; then
    echo "PSALMODIA ORACULUM: FRACTA (custos: limes excessus - vide $ACTA)"
    exit 1
fi
if [ "$rc" -ne 0 ] || ! grep -q "TEST PASSED: probatio_psalmodia_corpus" "$ACTA"; then
    echo "PSALMODIA ORACULUM: FRACTA (rc=$rc; vide $ACTA)"
    exit 1
fi
echo "PSALMODIA ORACULUM: sana"
exit 0
