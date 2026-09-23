#!/bin/bash
# differentia_exemplariorum.sh - PORTA REGRESSIONIS extractoris gradus II
#
# Usage:  ./crusta/instrumenta/differentia_exemplariorum.sh
#         EXEMPLARIA_SCRIBERE=1 ... (aurum renovare - mutatio CONSULTA
#                                    extractoris aut regulae solum)
# Exit:   0 concordes | 1 divergentiae | 2 nihil mensuratum
#
# QUID PROBAT: extractorem (materia_exemplaria) regulae I ordines
# EOSDEM adhuc reddere super CORPORE CONGELATO - CCXXXIV plagulae .sh
# domus ut in commissione CORPUS_COMMISSIO stabant, per 'git archive'
# in directorium temporarium extractae (lectio sola: arbor laboris,
# index, HEAD, stash intacta). Rete regressionis LATUM: vitium
# descensus (subarbor capta iterum descensa) MCXXXVI ordines movit -
# fixturae parvae id non viderent.
#
# CUR CORPUS CONGELATUM (2026-09-23): aurum olim contra corpus VIVUM
# comparabatur. Omnis linea addita in quavis plagula .sh ordines
# translatos faciebat (ter uno die, nulla inventione mutata), et
# remedium semper 'renova et inspice' erat - ritus qui oculos docet
# non inspicere, et per quem regressio vera tandem transiret. Aurum
# instantanea est; instantanea INPUT congelatum postulat. Inventa in
# plagulis NOVIS lintri ipsius sunt (diagnostica tempore commissionis),
# non huius portae.
#
# HISTORIA: aurum natum est ex extractore PYTHONICO (deletus
# 2026-09-18), qui cum C super hoc corpore nulla divergentia
# consenserat. C hodie super eodem corpore aurum illud OCTETIM reddit
# (mensuratum 2026-09-23) - provenientia ergo nihil iam differt; aurum
# est simpliciter responsum verificatum extractoris super corpore
# fixo.
#
# RENOVATIO: legitima SOLUM cum extractor aut regula I CONSULTO
# mutantur - differentiam lege ut quodvis aurum. Corpus ipsum
# renovari potest (CORPUS_COMMISSIO promota + aurum renovatum, UNA
# commissione, causa nominata) cum plagulae novae constructiones
# ferunt tegendas.
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
# differunt. Ergo utrumque latus numeratur ET numerus PANGITUR.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
cd "$RADIX_DIR" || exit 2
AURUM_DIR="crusta/probationes/fixa/exemplaria"
# CORPUS CONGELATUM: arbor huius commissionis (extractor pythonicus
# deletus, aurum natum). Promovere = consulto, cum auro, una commissione.
CORPUS_COMMISSIO="ad73f5390697a4ee4845cb8e104a12e538b9f100"
TEMP="$(mktemp -d)" || exit 2
# mktemp deficiens -> 'tar -C ""' in directorio currenti extraheret
if [ -z "$TEMP" ] || [ ! -d "$TEMP" ]; then
    echo "differentia: directorium temporarium non creatum - nihil mensuratum" >&2
    exit 2
fi
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

# Commissio absens (clonus vadosus, historia putata) = RECUSATIO
# nominata, numquam corpus vivum tacite substitutum
if ! git cat-file -e "${CORPUS_COMMISSIO}^{commit}" 2>/dev/null; then
    echo "differentia: commissio corporis $CORPUS_COMMISSIO absens (clonus vadosus?) - nihil mensuratum" >&2
    exit 2
fi
CORPUS="$TEMP/corpus"
mkdir -p "$CORPUS" || exit 2
# LECTIO SOLA: git archive arborem commissionis in fluxum tar scribit;
# arbor laboris, index, HEAD, stash non tanguntur
git archive "$CORPUS_COMMISSIO" | tar -x -C "$CORPUS" --include='*.sh' \
    || { echo "differentia: extractio corporis fracta - nihil mensuratum" >&2; exit 2; }

VIAE=()
while IFS= read -r v; do
    case "$v" in probationes/fixa/*) continue ;; esac
    VIAE+=("$CORPUS/$v")
done < <(git ls-tree -r --name-only "$CORPUS_COMMISSIO" | grep '\.sh$')
if [ "${#VIAE[@]}" -eq 0 ]; then
    echo "differentia: corpus vacuum - nihil mensuratum" >&2
    exit 2
fi
echo "differentia: ${#VIAE[@]} plagulae (corpus ${CORPUS_COMMISSIO:0:8}) contra aurum in $AURUM_DIR"

# Gradus I CRIBRATUR: facies utrumque gradum reddit, aurum gradum II
# solum fert. Campus IX codicem fert. Praefixum directorii temporarii
# e campo I demitur - viae ut in arbore commissionis.
cribrare () {
    awk -F'\t' -v p="$CORPUS/" 'BEGIN { OFS = "\t" }
        $9 ~ /^lint:/ {
            if (index($1, p) == 1) { $1 = substr($1, length(p) + 1) }
            print
        }'
}

# LINTRUM REGULAE I SOLIUS. Aurum regulam I SOLAM tegit (natum antequam
# regula II esset, et solum ordines regulae I cum extractore altero
# comparati sunt). Lintro pleno adhibito ordines regulae II
# admiscerentur et aurum scopum suum tacite mutaret; regula II infra
# (PARS B) suo modo probatur.
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
    # Corpus congelatum: aurum movetur SOLUM si extractor aut regula
    # mutata est. Differentia infra = mutatio illa ipsa - lege eam.
    echo "differentia: AURUM RENOVATUR (corpus ${CORPUS_COMMISSIO:0:8}) - lege differentiam PRIMUM:" >&2
    for modus in cruda plena; do
        diff -u "$AURUM_DIR/domus.$modus.tsv" "$TEMP/c.$modus.tsv" >&2 || true
    done
    cp "$TEMP/c.cruda.tsv" "$AURUM_DIR/domus.cruda.tsv"
    cp "$TEMP/c.plena.tsv" "$AURUM_DIR/domus.plena.tsv"
    echo "differentia: aurum scriptum ($(wc -l < "$AURUM_DIR/domus.cruda.tsv" | tr -d ' ') cruda, $(wc -l < "$AURUM_DIR/domus.plena.tsv" | tr -d ' ') plena) - AMBO commitenda, CAUSAM (mutationem extractoris/regulae) NOMINA" >&2
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
#
# DUO CORPORA (2026-09-23): PINNA super corpore CONGELATO (stabilitas:
# numerus nisi regula mutata numquam movetur - olim 37 super corpore
# VIVO, quod omnis vocatio clang addita movebat); SUBMISSIO GREP super
# corpore VIVO (oraculum independens ubique valet, plagulae novae
# quoque). LXX > XXXVII quia corpus congelatum commissionem regulae II
# (efc52a92: duae plagulae emendatae, octo annotatae) praecedit.
PIN_VEX_CRUDA=70
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
    echo "  ok   vexilla: $n_vex sedes (pinna, corpus ${CORPUS_COMMISSIO:0:8})"
fi

# REGULA SUBMISSA GREP (oraculum independens) - super corpore VIVO
VIAE_VIVAE=()
while IFS= read -r v; do
    case "$v" in probationes/fixa/*) continue ;; esac
    VIAE_VIVAE+=("$v")
done < <(git ls-files '*.sh')
if [ "${#VIAE_VIVAE[@]}" -eq 0 ]; then
    echo "differentia: corpus vivum vacuum - nihil mensuratum" >&2
    exit 2
fi
./crusta/facies.sh "${VIAE_VIVAE[@]}" -machina -sine-excusatione -cruda 2>/dev/null \
    | awk -F'\t' '$9 == "lint:vexilla-domus"' > "$TEMP/vex.viva.tsv" || true
awk -F'\t' '{print $1}' "$TEMP/vex.viva.tsv" | LC_ALL=C sort -u > "$TEMP/vex.regula.txt"
# LINEAE IUNCTAE: vocatio clang per lineas plures distendi potest, et
# grep linea-basatus eam non videret - regula tunc grep EXCEDERET sine
# ullo vitio, et porta lupum clamaret. Iunctio superset LAXIOREM facit,
# quod est direptio recta pro assertione 'regula submissa grep'
# (mensuratum: XI plagulae iunctim contra X lineatim - adhuc artum).
: > "$TEMP/vex.grep.txt"
for f in "${VIAE_VIVAE[@]}"; do
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
    echo "  ok   vexilla: regula submissa grep, corpus vivum ($(wc -l < "$TEMP/vex.regula.txt" | tr -d ' ') plagulae contra $(wc -l < "$TEMP/vex.grep.txt" | tr -d ' ') grep, ${#VIAE_VIVAE[@]} plagulae)"
fi

if [ "$exitus" -eq 0 ]; then
    echo "differentia exemplariorum: CONCORDES (corpus ${CORPUS_COMMISSIO:0:8}: ${#VIAE[@]} plagulae, $PIN_CRUDA cruda, $PIN_PLENA plena, $PIN_VEX_CRUDA vexilla; vivum ${#VIAE_VIVAE[@]} plagulae sub grep)"
fi
exit "$exitus"
