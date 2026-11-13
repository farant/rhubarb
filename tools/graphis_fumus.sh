#!/bin/bash
# tools/graphis_fumus.sh - fumus bin/graphis (graphis-plan T4):
# scheda, quadrum et iudicium picturae fixae
# (probationes/fixa/graphis/cubiculum.stml) et recusationes. Nihil
# extra directorium temporarium scribit. Manu curritur post
# ./tools/graphis_struere.sh.
#
# Usus: ./tools/graphis_fumus.sh     Exitus: 0 omnia bona, 1 fracta.
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
B=bin/graphis
P=probationes/fixa/graphis/cubiculum.stml
T="$(mktemp -d "${TMPDIR:-/tmp}/graphis_fumus.XXXXXX")"
trap 'rm -rf "$T"' EXIT
fracta=0
proba() {  # proba <descriptio> <exitus speratus> <mandatum...>
    local d="$1" sp="$2"; shift 2
    "$@" >"$T/out" 2>"$T/err"; local rc=$?
    if [ "$rc" -ne "$sp" ]; then
        echo "FRACTA: $d (exitus $rc, speratus $sp)"; sed 's/^/    /' "$T/err"
        fracta=$((fracta + 1))
    fi
}
contine() {  # contine <descriptio> <plagula> <textus>
    grep -q -- "$3" "$2" || { echo "FRACTA: $1 ('$3' abest)"; fracta=$((fracta + 1)); }
}
[ -x "$B" ] || { echo "graphis_fumus: $B abest (./tools/graphis_struere.sh)"; exit 1; }

proba "sine argumentis usus" 2 "$B"
proba "imperium ignotum usus" 2 "$B" saltare "$P" -o "$T/x.png"
proba "specimen sine -o usus" 2 "$B" specimen "$P"
proba "specimen sine plagula usus" 2 "$B" specimen -o "$T/x.png"
proba "specimen cum duabus plagulis usus" 2 "$B" specimen "$P" "$P" -o "$T/x.png"
proba "vexillum ignotum usus" 2 "$B" specimen "$P" -o "$T/x.png" -magnus
proba "quadrum sine -fines usus" 2 "$B" quadrum "$P" -o "$T/x.png"
proba "quadrum cum finibus malis usus" 2 "$B" quadrum "$P" -o "$T/x.png" -fines "1 2 3"
proba "quadrum cum cauda usus" 2 "$B" quadrum "$P" -o "$T/x.png" -fines "1 2 3 4 5"
proba "iudicare cum -o usus" 2 "$B" iudicare "$P" -o "$T/x.png"
proba "iudicare sine plagula usus" 2 "$B" iudicare
[ ! -e "$T/x.png" ] || { echo "FRACTA: usus PNG scripsit"; fracta=$((fracta + 1)); }

proba "specimen cubiculi" 0 "$B" specimen "$P" -o "$T/scheda.png"
file "$T/scheda.png" 2>/dev/null | grep -q "1448 x 480" || { echo "FRACTA: scheda 1448 x 480 abest"; fracta=$((fracta + 1)); }
contine "numerus regionum" "$T/out" "^regiones: 2$"
contine "regio prima: color, titulus, capsa" "$T/out" "  1  red     fenestra .* px  (2[0-9]*,5[0-9]*)-(2[0-9]*,1[0-9]*)$"
contine "regio altera" "$T/out" "  2  blue    stragulum "
contine "magnitudo imaginis" "$T/out" "scheda.png: 1448 x 480$"
cp "$T/scheda.png" "$T/prima.png"
proba "specimen iterum" 0 "$B" specimen "$P" -o "$T/scheda.png"
cmp -s "$T/scheda.png" "$T/prima.png" || { echo "FRACTA: scheda non determinata"; fracta=$((fracta + 1)); }

proba "quadrum fenestrae" 0 "$B" quadrum "$P" -o "$T/quadrum.png" -fines "210 44 80 70"
file "$T/quadrum.png" 2>/dev/null | grep -q "320 x 280" || { echo "FRACTA: quadrum 320 x 280 abest"; fracta=$((fracta + 1)); }
proba "quadrum extra picturam recusat" 1 "$B" quadrum "$P" -o "$T/extra.png" -fines "300 200 100 100"
contine "fines nominantur" "$T/err" "non intra picturam"
[ ! -e "$T/extra.png" ] || { echo "FRACTA: quadrum recusatum PNG scripsit"; fracta=$((fracta + 1)); }

proba "iudicare cubiculum" 0 "$B" iudicare "$P"
contine "accipe" "$T/out" "^ACCIPE $P$"

printf '<pictura>\n  <rectangulum fines="0 0 9 9" colores="6"/>\n  <circulus/>\n</pictura>\n' > "$T/mala.stml"
proba "elementum ignotum recusat" 1 "$B" specimen "$T/mala.stml" -o "$T/mala.png"
contine "via et linea" "$T/err" "^$T/mala.stml:3: "
contine "causa" "$T/err" "circulus"
[ ! -e "$T/mala.png" ] || { echo "FRACTA: recusata PNG scripsit"; fracta=$((fracta + 1)); }
proba "plagula absens" 1 "$B" specimen "$T/nulla.stml" -o "$T/nulla.png"
contine "plagula nominatur" "$T/err" "nulla.stml"
proba "iudicare: una bona, una mala" 1 "$B" iudicare "$P" "$T/mala.stml"
contine "bona accipitur" "$T/out" "^ACCIPE $P$"
contine "mala nominatur" "$T/err" "^$T/mala.stml:3: "

# partes ex -fontes; recusatio in parte viam partis nominat
mkdir -p "$T/partes"
printf '<pars>\n  <rectangulum fines="0 0 20 20" colores="6"/>\n</pars>\n' > "$T/partes/bona.stml"
printf '<pars>\n\n  <rectangulum fines="0 0 20" colores="6"/>\n</pars>\n' > "$T/partes/fracta.stml"
printf '<pictura>\n  <usus fons="bona" locus="5 5"/>\n</pictura>\n' > "$T/usus.stml"
printf '<pictura>\n  <usus fons="fracta"/>\n</pictura>\n' > "$T/usus_mala.stml"
proba "pars ex -fontes" 0 "$B" iudicare "$T/usus.stml" -fontes "$T/partes"
proba "pars non in directorio ordinario" 1 "$B" iudicare "$T/usus.stml"
contine "pars non inventa" "$T/err" "^$T/usus.stml:2: .*'bona' was not found"
proba "pars fracta" 1 "$B" iudicare "$T/usus_mala.stml" -fontes "$T/partes"
contine "via partis et linea" "$T/err" "^$T/partes/fracta.stml:3: "

if [ "$fracta" -eq 0 ]; then
    echo "GRAPHIS FUMUS: omnia bona"; exit 0
fi
echo "GRAPHIS FUMUS: $fracta fracta"; exit 1
