#!/bin/bash
# differentia_exemplariorum.sh - PORTA REGENS extractoris gradus II
#
# Usage:  ./crusta/instrumenta/differentia_exemplariorum.sh
#         EXEMPLARIA_SCRIBERE=1 ... (aurum renovare - CAUSAM NOMINA)
# Exit:   0 concordes | 1 divergentiae | 2 nihil mensuratum
#
# AURUM EST QUOD EXTRACTOR ALTER DIXIT. Usque ad 2026-09-18 extractor
# BIS exstabat - in C (materia_exemplaria) et in Pythone (stml vertere
# -> html.parser -> _relata) - et haec porta ambos super corpore domus
# currebat et differebat. Duae implementationes nihil communicantes
# consentiunt solum si ambae rectae sunt; ea est causa cur fides
# extractori C hinc venit (CCXXXIV plagulae, nulla divergentia, et
# culpae plantatae rubere eam posse probaverunt).
#
# Deinde latus pythonicum deletum est. Latera AMBO denuo derivare
# significaret C contra se ipsum currere - differentia quae FALLERE NON
# POTEST, quod peius est quam porta nulla quia 'ok' nihilominus
# imprimit. Ergo quod extractor alter ULTIMO dixit congelatum est
# (crusta/probationes/fixa/exemplaria/, vide README ibi), et porta nunc
# rogat: DICITNE C ADHUC QUOD AMBO CONSENSERUNT?
#
# DUAE COMPARATIONES, QUIA UNA SOLA CAECA EST:
#   CRUDA  bracchia ut regula ea scribit (DLXX ordines) - porta
#          EXTRACTORIS, superficies lata ubi vitium descensus aut
#          notae apparet
#   PLENA  post subtractionem declaratam (XIV ordines) - porta
#          SUBTRACTIONIS
# Sola comparatio plena XIV ordines tantum videret, et vitium in
# ordinibus subtractis nusquam appareret.
#
# LATUS VACUUM = PORTA MORTUA: plagulae duae vacuae sine divergentia
# differunt, et ita domus haec portam ultimam expedivit quae nihil
# mensurabat. Ergo utrumque latus numeratur ET numerus PANGITUR.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
cd "$RADIX_DIR" || exit 2
AURUM_DIR="crusta/probationes/fixa/exemplaria"
TEMP="$(mktemp -d)"
trap 'rm -rf "$TEMP"' EXIT

# NUMERI PANGUNTUR (mensurati 2026-09-18). Si moventur sine causa, aut
# corpus crevit aut regula mutata est - utrumque nominandum.
#
# DLXVIII -> DLXX (opus V): arcus ipse crusta/facies.sh peperit, quae
# '! [ "$BIN" -nt "$SRC" ]' fert - '-nt' RECTE negatum, ergo bracchium
# utrumque uno crescit et PLENA IMMOTA manet. Porta hoc ipsa invenit.
PIN_CRUDA=570
PIN_PLENA=14
# Per bracchium, intra 'cruda' - census regulae I, olim in
# pythonica/probatio_silva.py pinnatus.
PIN_AEQUALITAS=292
PIN_NEGATA=278

VIAE=()
while IFS= read -r v; do
    case "$v" in probationes/fixa/*) continue ;; esac
    VIAE+=("$v")
done < <(git ls-files '*.sh')
if [ "${#VIAE[@]}" -eq 0 ]; then
    echo "differentia: corpus vacuum - nihil mensuratum" >&2
    exit 2
fi
echo "differentia: ${#VIAE[@]} plagulae contra aurum in $AURUM_DIR"

# Gradus I CRIBRATUR: facies utrumque gradum reddit, aurum gradum II
# solum fert. Campus IX codicem fert.
cribrare () { awk -F'\t' '$9 ~ /^lint:/' ; }

./crusta/facies.sh "${VIAE[@]}" -machina -sine-excusatione -cruda \
    | cribrare | LC_ALL=C sort > "$TEMP/c.cruda.tsv" || true
./crusta/facies.sh "${VIAE[@]}" -machina -sine-excusatione \
    | cribrare | LC_ALL=C sort > "$TEMP/c.plena.tsv" || true

if [ "${EXEMPLARIA_SCRIBERE:-}" = "1" ]; then
    # AUDITIO, NON RENOVATIO TACITA: aurum ex PYTHONE natum est; hinc
    # ex C renovatur, ergo renovatio incauta portam circularem facit.
    # Lex domus: aurum numquam sine auditione renovatur.
    echo "differentia: AURUM RENOVATUR ex latere C - lege differentiam PRIMUM:" >&2
    for modus in cruda plena; do
        diff -u "$AURUM_DIR/domus.$modus.tsv" "$TEMP/c.$modus.tsv" >&2 || true
    done
    cp "$TEMP/c.cruda.tsv" "$AURUM_DIR/domus.cruda.tsv"
    cp "$TEMP/c.plena.tsv" "$AURUM_DIR/domus.plena.tsv"
    echo "differentia: aurum scriptum ($(wc -l < "$AURUM_DIR/domus.cruda.tsv" | tr -d ' ') cruda, $(wc -l < "$AURUM_DIR/domus.plena.tsv" | tr -d ' ') plena) - CAUSAM in commissione NOMINA" >&2
    exit 0
fi

exitus=0
for modus in cruda plena; do
    aurum="$AURUM_DIR/domus.$modus.tsv"
    c="$TEMP/c.$modus.tsv"
    if [ ! -s "$aurum" ]; then
        echo "differentia: $modus: AURUM ABSENS aut vacuum ($aurum) - nihil mensuratum" >&2
        exit 2
    fi
    n_aurum=$(wc -l < "$aurum" | tr -d ' ')
    n_c=$(wc -l < "$c" | tr -d ' ')
    if [ "$modus" = "cruda" ]; then pin=$PIN_CRUDA; else pin=$PIN_PLENA; fi
    if [ "$n_c" -eq 0 ]; then
        echo "differentia: $modus: LATUS C VACUUM - nihil mensuratum" >&2
        exit 2
    fi
    if [ "$n_aurum" -ne "$pin" ]; then
        echo "differentia: $modus: aurum $n_aurum ordines, pinna $pin - aurum mutatum sine pinna?" >&2
        exit 2
    fi
    if diff -u "$aurum" "$c" > "$TEMP/$modus.diff"; then
        echo "  ok   $modus: $n_aurum ordines, nulla divergentia ab auro"
    else
        n_div=$(grep -c '^[+-][^+-]' "$TEMP/$modus.diff" || true)
        echo "  FRACTA $modus: aurum $n_aurum / c $n_c, $n_div lineae discordes" >&2
        head -20 "$TEMP/$modus.diff" >&2
        exitus=1
    fi
done

# CENSUS PER BRACCHIUM: numeri regulae I ipsi, ex auro crudo. Summa
# sola bracchium unum crescere et alterum decrescere celaret.
n_aeq=$(awk -F'\t' '$9 == "lint:nt-aequalitas"' "$AURUM_DIR/domus.cruda.tsv" | wc -l | tr -d ' ')
n_neg=$(awk -F'\t' '$9 == "lint:nt-negata"' "$AURUM_DIR/domus.cruda.tsv" | wc -l | tr -d ' ')
if [ "$n_aeq" -eq "$PIN_AEQUALITAS" ] && [ "$n_neg" -eq "$PIN_NEGATA" ]; then
    echo "  ok   census: nt-aequalitas $n_aeq, nt-negata $n_neg, differentia $PIN_PLENA"
else
    echo "  FRACTA census: nt-aequalitas $n_aeq (pinna $PIN_AEQUALITAS), nt-negata $n_neg (pinna $PIN_NEGATA)" >&2
    exitus=1
fi

if [ "$exitus" -eq 0 ]; then
    echo "differentia exemplariorum: CONCORDES (${#VIAE[@]} plagulae, $PIN_CRUDA cruda, $PIN_PLENA plena)"
fi
exit "$exitus"
