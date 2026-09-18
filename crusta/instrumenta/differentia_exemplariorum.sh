#!/bin/bash
# differentia_exemplariorum.sh - PORTA REGENS arcus exemplariorum in C
#
# Usage:  ./crusta/instrumenta/differentia_exemplariorum.sh [-verbose]
# Exit:   0 concordes | 1 divergentiae | 2 nihil mensuratum
#
# DUAE IMPLEMENTATIONES, CORPUS UNUM. Extractor prior (pythonica,
# html.parser) et extractor novus (C) ordines eosdem super plagulis
# domus OMNIBUS reddere debent. Haec est methodus eadem qua arcus
# sedium 5,501,210 elementa probavit: nulla alia probatio extractorem
# super corpore vero exercet, et nihil Pythonis deleri potest donec
# haec porta viridis sit.
#
# DUAE COMPARATIONES, QUIA UNA SOLA CAECA EST:
#   CRUDA  bracchia ut regula ea scribit (DLXVIII ordines) - porta
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
#
# ORDO: latera ambo ordinantur (LC_ALL=C), quia ordo Pythonis ordo
# congruentiarum est et ordo noster (initium, codex) - res quam hic
# non probamus.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
cd "$RADIX_DIR" || exit 2
REGULA="crusta/lintrum/nt-aequalitas.stml"
TEMP="$(mktemp -d)"
trap 'rm -rf "$TEMP"' EXIT

# NUMERI PANGUNTUR (mensurati 2026-09-18, post transitum). Si moventur
# sine causa, aut corpus crevit aut regula mutata est - utrumque
# nominandum. Historia in pythonica/probatio_silva.py.
#
# DLXVIII -> DLXX (opus V): arcus ipse crusta/facies.sh peperit, quae
# '! [ "$BIN" -nt "$SRC" ]' fert - '-nt' RECTE negatum, ergo bracchium
# utrumque uno crescit et PLENA IMMOTA manet. Porta hoc ipsa invenit:
# numerus motus causam poposcit et causam habuit.
PIN_CRUDA=570
PIN_PLENA=14

VIAE=()
while IFS= read -r v; do
    case "$v" in probationes/fixa/*) continue ;; esac
    VIAE+=("$v")
done < <(git ls-files '*.sh')
if [ "${#VIAE[@]}" -eq 0 ]; then
    echo "differentia: corpus vacuum - nihil mensuratum" >&2
    exit 2
fi
echo "differentia: ${#VIAE[@]} plagulae, regula $REGULA"

printf '%s\n' "${VIAE[@]}" > "$TEMP/viae.txt"

# --- latus pythonicum -------------------------------------------------
python3 - "$TEMP" "$REGULA" <<'PYEOF' || exit 2
import sys
sys.path.insert(0, 'pythonica')
import silva

temp, regula = sys.argv[1], sys.argv[2]
viae = [l.rstrip('\n') for l in open(temp + '/viae.txt') if l.strip()]
# INSTRUMENTUM PRIMUM CALEFACIT, SERIE. 'exemplaria' sex operarios
# parallelos mittit, et si arbor crustae post aedificationem recentem
# nondum ligata est, operarii in eam simul currunt et plagulae
# 'fractae' redeunt - mensuratum 2026-09-18, statim post plantam.
# Vocatio una serialis ligationem perficit; deinde parallelismus tutus
# est. Sine hac, porta post aedificationem quamlibet vacillat.
silva.arbor(viae[0])
ex = silva.exemplaria(viae, regula)
if ex.fracturae:
    sys.stderr.write('differentia: %d plagulae fractae (prima: %s)\n'
                     % (len(ex.fracturae), sorted(ex.fracturae)[0]))
    sys.exit(2)
cruda = silva.diagnostica_ex_congruentiis(ex.congruentiae)
plena = silva.diagnostica_ex_congruentiis(
    silva.congruentiae_minus(ex.congruentiae, 'nt-aequalitas',
                             'nt-negata'))
open(temp + '/py.cruda.tsv', 'w').write(silva.diagnostica_tsv(cruda))
open(temp + '/py.plena.tsv', 'w').write(silva.diagnostica_tsv(plena))
PYEOF

# --- latus C ----------------------------------------------------------
# Gradus I CRIBRATUR: facies utrumque gradum reddit, extractor prior
# gradum II solum novit. Campus IX codicem fert.
cribrare () { awk -F'\t' '$9 ~ /^lint:/' ; }

./crusta/facies.sh "${VIAE[@]}" -machina -sine-excusatione -cruda \
    | cribrare > "$TEMP/c.cruda.tsv" || true
./crusta/facies.sh "${VIAE[@]}" -machina -sine-excusatione \
    | cribrare > "$TEMP/c.plena.tsv" || true

exitus=0
for modus in cruda plena; do
    py="$TEMP/py.$modus.tsv"
    c="$TEMP/c.$modus.tsv"
    n_py=$(wc -l < "$py" | tr -d ' ')
    n_c=$(wc -l < "$c" | tr -d ' ')
    if [ "$modus" = "cruda" ]; then pin=$PIN_CRUDA; else pin=$PIN_PLENA; fi
    if [ "$n_py" -eq 0 ] || [ "$n_c" -eq 0 ]; then
        echo "differentia: $modus: LATUS VACUUM (py $n_py, c $n_c) - nihil mensuratum" >&2
        exit 2
    fi
    if [ "$n_py" -ne "$pin" ]; then
        echo "differentia: $modus: pythonica $n_py ordines, pinna $pin - corpus aut regula mutata" >&2
        exit 2
    fi
    LC_ALL=C sort "$py" > "$TEMP/py.$modus.ord"
    LC_ALL=C sort "$c"  > "$TEMP/c.$modus.ord"
    if diff -u "$TEMP/py.$modus.ord" "$TEMP/c.$modus.ord" > "$TEMP/$modus.diff"; then
        echo "  ok   $modus: $n_py ordines utrimque, nulla divergentia"
    else
        n_div=$(grep -c '^[+-][^+-]' "$TEMP/$modus.diff" || true)
        echo "  FRACTA $modus: py $n_py / c $n_c, $n_div lineae discordes" >&2
        head -20 "$TEMP/$modus.diff" >&2
        exitus=1
    fi
done

if [ "$exitus" -eq 0 ]; then
    echo "differentia exemplariorum: CONCORDES (${#VIAE[@]} plagulae, $PIN_CRUDA cruda, $PIN_PLENA plena)"
fi
exit "$exitus"
