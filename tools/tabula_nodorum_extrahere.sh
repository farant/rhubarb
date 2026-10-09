#!/bin/bash
# tools/tabula_nodorum_extrahere.sh - extractum KnotInfo pro tabula nodorum
#
# Ex instantaneo database_knotinfo 2026.10.5 (github.com/soehms/
# database_knotinfo, data KnotInfo: C. Livingston, A. H. Moore,
# knotinfo.math.indiana.edu) nodos usque ad XIII transitus extrahit:
#
#   probationes/fixa/knotinfo/2026.10.5/nodi_xiii.tsv  (COMMITTITUR)
#       titulus, numerus transituum, symmetria, codex PD - solum
#   build/tabula_nodorum/knotinfo_polynomia.tsv      (NON committitur)
#       titulus, Alexander, Jones KnotInfo - ad collationem solam
#
# deinde tools/tabula_nodorum_generare.sh -collatio: polynomia per
# laqueus ex codicibus PD computata cum columnis KnotInfo conferuntur;
# discrepantia = exitus 1. Polynomia KnotInfo numquam in repositorium
# veniunt: tabula ea ipsa computat.
#
# Usus: ./tools/tabula_nodorum_extrahere.sh <knotinfo_data_complete.csv>
#   (plagula: raw.githubusercontent.com/soehms/database_knotinfo/
#    2026.10.5/database_knotinfo/csv_data/knotinfo_data_complete.csv)
set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

SHA_FIXUM="eb511ebc61204bc1c6d700d9da42257dcbb0369cb6756e89ed978a7821867c7d"
VERSIO="2026.10.5"
EXITUS="probationes/fixa/knotinfo/$VERSIO/nodi_xiii.tsv"
POLYNOMIA="build/tabula_nodorum/knotinfo_polynomia.tsv"

fracta() { echo "tabula_nodorum_extrahere: $1" >&2; exit 1; }

[ $# -eq 1 ] || fracta "usus: tabula_nodorum_extrahere.sh <knotinfo_data_complete.csv>"
CSV="$1"
[ -f "$CSV" ] || fracta "plagula non exstat: $CSV"
SHA="$(shasum -a 256 "$CSV" | cut -d' ' -f1)"
[ "$SHA" = "$SHA_FIXUM" ] || fracta "sha256 $SHA != $SHA_FIXUM (instantaneum $VERSIO non est)"

mkdir -p "$(dirname "$EXITUS")" "$(dirname "$POLYNOMIA")"
# columnae (1-basatae): 1 titulus, 27 pd_notation, 29 crossing_number,
# 63 alexander_polynomial, 67 jones_polynomial, 127 symmetry_type; lineae
# 1-2 = capita (nomina, tituli ostensi)
awk -F'|' -v versio="$VERSIO" -v sha="$SHA_FIXUM" -v pol="$POLYNOMIA" '
    BEGIN {
        print "# tabula nodorum: extractum KnotInfo (C. Livingston, A. H. Moore,"
        print "# knotinfo.math.indiana.edu) per database_knotinfo " versio
        print "# (github.com/soehms/database_knotinfo), knotinfo_data_complete.csv"
        print "# sha256 " sha
        print "# columnae: titulus, transitus, symmetria, codex PD (KnotTheory)"
        print "# GENERATUM: tools/tabula_nodorum_extrahere.sh - NE EDITA MANU"
    }
    NR <= 2 { next }
    $29 ~ /^[0-9]+$/ && $29 + 0 <= 13 {
        sym = $127; sub(/[ \t]+$/, "", sym)
        print $1 "\t" $29 "\t" sym "\t" $27
        print $1 "\t" $63 "\t" $67 > pol
    }' "$CSV" > "$EXITUS" || fracta "extractio fracta"

N=$(grep -vc '^#' "$EXITUS")
# A002863: 1 + 249 (<= X) + 552 + 2176 + 9988
[ "$N" -eq 12966 ] || fracta "nodi $N, non 12966"
echo "tabula_nodorum_extrahere: $EXITUS ($N nodi); collatio cum polynomiis KnotInfo..."
./tools/tabula_nodorum_generare.sh -collatio "$POLYNOMIA" || fracta "collatio fracta"
