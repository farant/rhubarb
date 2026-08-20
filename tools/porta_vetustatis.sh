#!/bin/bash

# tools/porta_vetustatis.sh - PORTA VETUSTATIS manifestorum amalgamatis
#
# Usus: ./tools/porta_vetustatis.sh <proiectum> [<statio>]
#   proiectum: silva | tessera | officina
#   statio:    directorium pro relationibus (ordinarius: build/vetustas)
#
# Exitus: 0 = manifesta recentia | 1 = rancida | 2 = usus
#
# CUR HAEC PORTA EXSTAT
# ---------------------
# amalgamare.sh manifesta sua NON regenerat - ex COMMISSIS struit.
# Ergo modulus novus in fontibus, aut vocator novus functionis
# vendicatae, sine regeneratione = amalgama tacite FALSA quae portas
# omnes proprias transit, quia nihil quod ABEST quaeritur.
#
# MENSURATUM 2026-08-20, vitio plantato, ambae partes:
#   sine porta: amalgamator successit sine querela, modulus novus
#               prorsus abfuit (grep = 0), et fructus sub vexillis
#               domus PLENIS pure compilavit - omissio invisibilis;
#   cum porta:  exitus 1, modulus nominatus, mandatum regenerationis
#               nominatum, ante quam quicquam struitur.
# Eodem die rancorem VERUM primo cursu cepit: fontes_generata.h
# silvae postulata_posix.h ab 7ba8d84 omittebat.
#
# CONFERTIO INCONDITA de industria - non per mtime, non 'si plagula
# nova apparet': excludenda etiam tunc mutantur cum functio vendicata
# IAM NOTA vocatorem novum accipit, quod nulla plagula nova prodit.
#
# UNA porta, TRIA proiecta. Prius haec massa in silva/amalgamare.sh
# sola vivebat; tessera et officina eandem rimam apertam ferebant.
# Lectio diei: exclusio aut lista UNIUS casus scripta, cuius ratio
# CLASSEM tegit, membrum secundum tacite fallit.

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

[ $# -ge 1 ] || {
    echo "usus: porta_vetustatis.sh <proiectum> [<statio>]" >&2
    exit 2
}
PROIECTUM="$1"
STATIO="${2:-build/vetustas}"
mkdir -p "$STATIO"

RELATIO_FONTIUM="$STATIO/${PROIECTUM}_fontes.txt"
RELATIO_EXCLUDENDORUM="$STATIO/${PROIECTUM}_excludenda.txt"

if ! ./tools/amalgama_fontes_generare.sh -probare "$PROIECTUM" \
        > "$RELATIO_FONTIUM" 2>&1; then
    echo "porta vetustatis ($PROIECTUM): manifestum FONTIUM rancidum:"
    grep -v '^  \[dep\]\|^  \[amalgama\]\|paratum$' \
        "$RELATIO_FONTIUM" | head -12
    exit 1
fi

if ! ./tools/amalgama_excludenda_generare.sh -probare "$PROIECTUM" \
        > "$RELATIO_EXCLUDENDORUM" 2>&1; then
    echo "porta vetustatis ($PROIECTUM): manifestum EXCLUDENDORUM rancidum:"
    grep -v '^  gyrus ' "$RELATIO_EXCLUDENDORUM" | head -12
    exit 1
fi

exit 0
