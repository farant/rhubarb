#!/bin/bash
# tools/musicxml_silvestria.sh - porta 'musicxml-oraculum' (plan musicxml P8)
#
# Corpus silvestre (OpenScore, CC0; ./tools/musicxml_arcessere.sh
# -silvestria) contra aurum commissum: probatio_musicxml_suite cum
# MUSICXML_SILVESTRIA. Corpus absens = OMISSA (numquam silentio sana).
#
# SEMPER SUB CUSTODE (tools/custos.sh): limes memoriae X GB (vestigium,
# compressa et permutata inclusa - RSS sub compressione macOS mentitur).
# 2026-10-30: cursus primus sine custode et sine piscina per fontem ad
# ~LXXV GB crevit (Franus vidit). Limes excessus = FRACTA, non mora.
# Tectum sectionis credo (fine sectionis iudicatum) IV GB: partitura
# maxima (Reger op. 74, XVI MB XML) ~1.7 GB apicis lectoris postulat.
#
# Usus:   ./tools/musicxml_silvestria.sh            (confert)
#         MUSICXML_TABULAS_SCRIBERE=dir ./tools/musicxml_silvestria.sh
#                                                   (tabulas + sigilla scribit)
# Effusio: 'MUSICXML SILVESTRIA: sana N/N' | 'FRACTA ...' | 'OMISSA: ...'
# <porta titulus="musicxml-oraculum" tegit="tools/musicxml_silvestria.sh probationes/fixa/musicxml/silvestria.manifestum probationes/fixa/musicxml/silvestria.sigilla probationes/fixa/musicxml/aurum_silvestre/*" currit="probationes/probatio_musicxml_suite.c lib/musicxml.c lib/musicxml_ordo.c"/>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
CORPUS="$PWD/build/musicxml/silvestria"
if [ ! -d "$CORPUS" ]; then
    echo "MUSICXML SILVESTRIA: OMISSA: corpus abest ($CORPUS; ./tools/musicxml_arcessere.sh -silvestria)"
    exit 0
fi
ACTA="build/musicxml/silvestria_acta.log"
CUSTOS_MEMORIA_MB="${CUSTOS_MEMORIA_MB:-10240}" CUSTOS_MORA="${CUSTOS_MORA:-1800}" \
CREDO_MEMORIA_MB="${CREDO_MEMORIA_MB:-4096}" \
MUSICXML_SILVESTRIA="$CORPUS" \
    ./tools/custos.sh -- ./compile_tests.sh probatio_musicxml_suite > "$ACTA" 2>&1
rc=$?
grep -E "MUSICXML SILVESTRIA|DISCORS|aurum :|nostra:|FASCICULUS MUTATUS|SIGILLUM DISCORS|CUSTOS:" "$ACTA"
if grep -q "memoria excessa\|mora excessa" "$ACTA"; then
    echo "MUSICXML SILVESTRIA: FRACTA (custos: limes excessus - vide $ACTA)"
    exit 1
fi
grep -q "MUSICXML SILVESTRIA: \(sana\|scriptae\|OMISSA\)" "$ACTA" || {
    echo "MUSICXML SILVESTRIA: FRACTA (rc=$rc; vide $ACTA)"
    exit 1
}
exit 0
