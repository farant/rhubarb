#!/bin/bash

# tools/amalgama_auditor.sh - auditor listarum amalgamatorum
#
# AMALGAMA Phasis B: manifesta manualia (silva/tessera/officina
# amalgamatores) contra clausuras ab aedile derivatas comparare -
# instrumentum OCULORUM (differentia-modo), non porta: exitus 0
# nisi fractura operationis; inventa Frani leguntur ante
# migrationem (Phasis C).
#
# Lineae per proiectum:
#   1. amalgamare.sh curritur (portae propriae + binarium recens)
#   2. listae manuales: <proj>/build/amalgamator --enumerare
#      (CV/OV capita/corpora vendicata; CP/OP propria)
#   3. derivatae: unio `bin/aedilis <radix> --partes` super radices
#      OV+OP (radices ipsae ad corpora derivata adduntur - scopus
#      numquam obiectum sui ipsius)
#   4. sacculi: CONSENSUS / DERIVATA-SOLA / MANUS-SOLA (capita,
#      corpora)
#   5. ordo manualis capitum contra aristas derivatas (--aristae):
#      violatio = caput ante dependentiam suam
#
# Usus: ./tools/amalgama_auditor.sh [proiectum...]
#       (ordinarie: silva tessera officina)

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

PROIECTA=("$@")
if [ ${#PROIECTA[@]} -eq 0 ]; then
    PROIECTA=(silva tessera officina)
fi

# semper struere: binarium stalum optiones novas ignorat tacite
# (classis obiecti-stali - proba prima huius ipsius auditoris)
./tools/aedilis_struere.sh || exit 1

STATIO_RADIX="build/aedilis/auditor"
mkdir -p "$STATIO_RADIX"

for proj in "${PROIECTA[@]}"; do
    echo "================================================"
    echo "AUDITOR: $proj"
    echo "================================================"
    STATIO="$STATIO_RADIX/$proj"
    rm -rf "$STATIO"
    mkdir -p "$STATIO"

    # -- 1. amalgamare (portae propriae; binarium amalgamatoris) --
    if ! "./$proj/amalgamare.sh" > "$STATIO/amalgamare.log" 2>&1; then
        echo "  FRACTURA: $proj/amalgamare.sh (vide $STATIO/amalgamare.log)"
        exit 1
    fi
    BINARIUM="$proj/build/amalgamator"
    [ -x "$BINARIUM" ] || { echo "  FRACTURA: $BINARIUM deest"; exit 1; }

    # -- 2. listae manuales --
    "$BINARIUM" --enumerare > "$STATIO/manus.tsv" || {
        echo "  FRACTURA: --enumerare"; exit 1; }
    awk -F'\t' '$1=="CV"||$1=="CP" { print $2 }' "$STATIO/manus.tsv" \
        | sort -u > "$STATIO/manus_capita.txt"
    awk -F'\t' '$1=="OV"||$1=="OP" { print $2 }' "$STATIO/manus.tsv" \
        | sort -u > "$STATIO/manus_corpora.txt"
    # ordo manualis capitum (CV deinde CP, sine sort - ipse ordo)
    awk -F'\t' '$1=="CV"||$1=="CP" { print $2 }' "$STATIO/manus.tsv" \
        > "$STATIO/manus_ordo.txt"

    # -- 3. derivatio: unio super radices corporum --
    : > "$STATIO/partes.tsv"
    : > "$STATIO/aristae.tsv"
    FRACTAE=0
    while IFS= read -r radix; do
        if ! ./bin/aedilis "$radix" --partes >> "$STATIO/partes.tsv" \
                2> "$STATIO/derivatio.err"; then
            echo "  DERIVATIO FRACTA: $radix"
            cat "$STATIO/derivatio.err" | head -2
            FRACTAE=$((FRACTAE+1))
            continue
        fi
        ./bin/aedilis "$radix" --aristae >> "$STATIO/aristae.tsv" \
            2>/dev/null || true
        echo "$radix" >> "$STATIO/radices_bonae.txt"
    done < "$STATIO/manus_corpora.txt"

    awk -F'\t' '$1=="C" { print $2 }' "$STATIO/partes.tsv" \
        | sort -u > "$STATIO/derivata_capita.txt"
    { awk -F'\t' '$1=="O" { print $2 }' "$STATIO/partes.tsv"
      cat "$STATIO/manus_corpora.txt"
    } | sort -u > "$STATIO/derivata_corpora.txt"
    sort -u "$STATIO/aristae.tsv" > "$STATIO/aristae_unicae.tsv"

    # -- 4. sacculi --
    echo ""
    echo "-- CAPITA (manus $(wc -l < "$STATIO/manus_capita.txt" | tr -d ' ') / derivata $(wc -l < "$STATIO/derivata_capita.txt" | tr -d ' ')) --"
    echo "  DERIVATA-SOLA (aedilis videt, manifestum non fert):"
    comm -13 "$STATIO/manus_capita.txt" "$STATIO/derivata_capita.txt" \
        | sed 's/^/    /'
    echo "  MANUS-SOLA (manifestum fert, aedilis non derivat):"
    comm -23 "$STATIO/manus_capita.txt" "$STATIO/derivata_capita.txt" \
        | sed 's/^/    /'

    echo ""
    echo "-- CORPORA (manus $(wc -l < "$STATIO/manus_corpora.txt" | tr -d ' ') / derivata $(wc -l < "$STATIO/derivata_corpora.txt" | tr -d ' ')) --"
    echo "  DERIVATA-SOLA:"
    comm -13 "$STATIO/manus_corpora.txt" "$STATIO/derivata_corpora.txt" \
        | sed 's/^/    /'
    echo "  MANUS-SOLA:"
    comm -23 "$STATIO/manus_corpora.txt" "$STATIO/derivata_corpora.txt" \
        | sed 's/^/    /'

    # -- 5. ordo manualis contra aristas derivatas --
    echo ""
    echo "-- ORDO CAPITUM MANUALIS contra aristas derivatas --"
    VIOLATIONES=$(awk -F'\t' '
        NR == FNR { ordo[$0] = FNR; next }
        {
            includens = $1; inclusum = $2
            if ((includens in ordo) && (inclusum in ordo) \
                && ordo[inclusum] > ordo[includens]) {
                printf "    %s ante dependentiam %s\n", \
                    includens, inclusum
            }
        }
    ' "$STATIO/manus_ordo.txt" "$STATIO/aristae_unicae.tsv")
    if [ -n "$VIOLATIONES" ]; then
        echo "  VIOLATIONES:"
        echo "$VIOLATIONES"
    else
        echo "  ordo manualis = linearizatio valida aristarum ✓"
    fi
    if [ "$FRACTAE" -gt 0 ]; then
        echo ""
        echo "  NB: $FRACTAE radices non derivatae (vide supra)"
    fi
    echo ""
done

echo "AUDITOR AMALGAMATIS: perfectum (instrumentum oculorum - sacculi supra leguntur)"
