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

# LINTRUM REGULAE I SOLIUS. Aurum quod hic iacet ex PYTHONE natum est,
# et Python regulam I SOLAM umquam cucurrit - deletus est antequam
# regula II scripta esset. Lintro pleno adhibito ordines regulae II
# admiscerentur, quos C SOLUS peperit, et plagula una partim TESTIMONIUM
# partim IMAGO SUI fieret sine ulla nota quae utrum sit distingueret.
# Auctoritas auri tanta maneat quanta est.
REG_I="$TEMP/lintrum_i"
mkdir -p "$REG_I"
cp crusta/lintrum/nt-aequalitas.stml "$REG_I/" || exit 2

CRUSTA_LINTRUM="$REG_I" ./crusta/facies.sh "${VIAE[@]}" \
    -machina -sine-excusatione -cruda \
    | cribrare | LC_ALL=C sort > "$TEMP/c.cruda.tsv" || true
CRUSTA_LINTRUM="$REG_I" ./crusta/facies.sh "${VIAE[@]}" \
    -machina -sine-excusatione \
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

# ==================================================
# PARS B: REGULA II (vexilla-domus) - PINNA, NON AURUM
# ==================================================
#
# DISTINCTIO QUAE SERVANDA EST. Aurum supra ex implementatione ALTERA
# venit, ergo RECTITUDINEM probat. Hic numeri ex C ipso veniunt, ergo
# STABILITATEM solam probant - motum sine causa capiunt, rectitudinem
# numquam. Domus utrumque genus iam habet (oraculum crustae = bash
# ipse, rectitudo; computus = numeri nostri, stabilitas), et ea
# confundere esset auctoritatem sumere quam numerus non habet.
#
# ORACULUM INDEPENDENS TAMEN ADEST, crudum sed vere alienum: grep.
# Mechanismus alius, vitia alia. Regula SUBMISSA grep esse debet -
# grep enim vexilla in CHORDIS et COMMENTARIIS numerat quae vocatio
# nulla sunt (mensuratum: .claude/hooks/examen-custos.sh nuntium
# hominis fert, tools/vexilla_fumus.sh formam CANONICAM cum nuntio
# credonis). Si regula grep EXCEDAT, regula plus videt quam textus
# fert - quod fieri non potest, ergo vitium.
PIN_VEX_CRUDA=37
VEX_CONTRACTUS='-std=c89|-pedantic|-Wall|-Wextra|-Werror|-Wconversion|-Wsign-conversion|-Wcast-qual|-Wstrict-prototypes|-Wmissing-prototypes|-Wwrite-strings'

./crusta/facies.sh "${VIAE[@]}" -machina -sine-excusatione -cruda 2>/dev/null \
    | awk -F'\t' '$9 == "lint:vexilla-domus"' > "$TEMP/vex.cruda.tsv" || true
n_vex=$(wc -l < "$TEMP/vex.cruda.tsv" | tr -d ' ')
if [ "$n_vex" -eq 0 ]; then
    echo "  FRACTA vexilla: ZERO sedes - regula muta, nihil mensuratum" >&2
    exitus=1
elif [ "$n_vex" -ne "$PIN_VEX_CRUDA" ]; then
    echo "  FRACTA vexilla: $n_vex sedes, pinna $PIN_VEX_CRUDA - corpus aut regula mutata, causam nomina" >&2
    exitus=1
else
    echo "  ok   vexilla: $n_vex sedes (pinna), regula post excusationes TACET"
fi

# REGULA SUBMISSA GREP (oraculum independens)
awk -F'\t' '{print $1}' "$TEMP/vex.cruda.tsv" | LC_ALL=C sort -u > "$TEMP/vex.regula.txt"
# LINEAE IUNCTAE: vocatio clang per lineas plures distendi potest, et
# grep linea-basatus eam non videret - regula tunc grep EXCEDERET sine
# ullo vitio, et porta lupum clamaret. Iunctio superset LAXIOREM facit,
# quod est direptio recta pro assertione 'regula submissa grep'
# (mensuratum: XI plagulae iunctim contra X lineatim - adhuc artum).
: > "$TEMP/vex.grep.txt"
for f in "${VIAE[@]}"; do
    tr '\n' ' ' < "$f" | grep -qE "clang[^|]*($VEX_CONTRACTUS)" \
        && echo "$f" >> "$TEMP/vex.grep.txt"
done
LC_ALL=C sort -u -o "$TEMP/vex.grep.txt" "$TEMP/vex.grep.txt"
extra=$(comm -23 "$TEMP/vex.regula.txt" "$TEMP/vex.grep.txt")
if [ -n "$extra" ]; then
    echo "  FRACTA vexilla: regula grep EXCEDIT (plus videt quam textus fert):" >&2
    printf '%s\n' "$extra" >&2
    exitus=1
else
    echo "  ok   vexilla: regula submissa grep ($(wc -l < "$TEMP/vex.regula.txt" | tr -d ' ') plagulae contra $(wc -l < "$TEMP/vex.grep.txt" | tr -d ' ') grep)"
fi

if [ "$exitus" -eq 0 ]; then
    echo "differentia exemplariorum: CONCORDES (${#VIAE[@]} plagulae, $PIN_CRUDA cruda, $PIN_PLENA plena, $PIN_VEX_CRUDA vexilla)"
fi
exit "$exitus"
