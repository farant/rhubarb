#!/bin/bash
# tools/plastica_fumus.sh - fumus bin/plastica (plastica-plan T3, T4, T7):
# specimen putei (plastica/res/puteus.stml) et recusationes. Nihil
# extra directorium temporarium scribit. Manu curritur post
# ./tools/plastica_struere.sh.
#
# Usus: ./tools/plastica_fumus.sh     Exitus: 0 omnia bona, 1 fracta.
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
B=bin/plastica
R=plastica/res
T="$(mktemp -d "${TMPDIR:-/tmp}/plastica_fumus.XXXXXX")"
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
[ -x "$B" ] || { echo "plastica_fumus: $B abest (./tools/plastica_struere.sh)"; exit 1; }

proba "sine argumentis usus" 2 "$B"
proba "imperium ignotum usus" 2 "$B" saltare "$R/puteus.stml" -o "$T/x.png"
proba "specimen sine -o usus" 2 "$B" specimen "$R/puteus.stml"
proba "specimen sine plagula usus" 2 "$B" specimen -o "$T/x.png"
proba "vexillum ignotum usus" 2 "$B" specimen "$R/puteus.stml" -o "$T/x.png" -magnus
[ ! -e "$T/x.png" ] || { echo "FRACTA: usus PNG scripsit"; fracta=$((fracta + 1)); }

# omnis res in plastica/res componitur et paginam dat
for f in "$R"/*.stml; do
    n="$(basename "$f" .stml)"
    proba "specimen $n" 0 "$B" specimen "$f" -o "$T/$n.png"
    file "$T/$n.png" 2>/dev/null | grep -q "640 x 640" || { echo "FRACTA: specimen $n: PNG 640 x 640 abest"; fracta=$((fracta + 1)); }
    contine "numeri $n" "$T/out" " partes ("
    contine "fines $n" "$T/out" "^fines: x "
    contine "vestigium $n" "$T/out" "^vestigium: "
done
proba "specimen putei" 0 "$B" specimen "$R/puteus.stml" -o "$T/puteus.png"
contine "partes putei" "$T/out" "29 partes (0 invisae), 452 triangula"
contine "tabula partium: color, signum, titulus" "$T/out" "  0  red      solidum     saxum imum"
contine "pars sine signo" "$T/out" "orange   -           funis"
contine "puteus obstat" "$T/out" "^vestigium: 52 cellae solidae"
cp "$T/puteus.png" "$T/prima.png"
proba "specimen putei iterum" 0 "$B" specimen "$R/puteus.stml" -o "$T/puteus.png"
cmp -s "$T/puteus.png" "$T/prima.png" || { echo "FRACTA: pagina non determinata"; fracta=$((fracta + 1)); }

printf '<res titulus="a">\n  <capsa magnitudo="1 1 1"/>\n  <globus radius="1"/>\n</res>\n' > "$T/mala.stml"
proba "elementum ignotum recusat" 1 "$B" specimen "$T/mala.stml" -o "$T/mala.png"
contine "via et linea" "$T/err" "^$T/mala.stml:3: "
contine "causa" "$T/err" "globus"
[ ! -e "$T/mala.png" ] || { echo "FRACTA: recusata PNG scripsit"; fracta=$((fracta + 1)); }
printf 'non stml <' > "$T/textus.stml"
proba "non-STML recusat" 1 "$B" specimen "$T/textus.stml" -o "$T/mala.png"
proba "plagula absens" 1 "$B" specimen "$T/nulla.stml" -o "$T/mala.png"
proba "via PNG mala" 1 "$B" specimen "$R/puteus.stml" -o "$T/nullum/dir/x.png"
proba "provenientia" 0 "$B" -provenientia

# texturae: pagina generum (III tabulae 384 x 384), determinata
proba "texturae" 0 "$B" texturae -o "$T/texturae.png"
file "$T/texturae.png" 2>/dev/null | grep -q "1152 x 384" || { echo "FRACTA: texturae: PNG 1152 x 384 abest"; fracta=$((fracta + 1)); }
contine "genus primum" "$T/out" "^  0  cortex$"
contine "genus ultimum" "$T/out" "^  8  granum$"
cp "$T/texturae.png" "$T/texturae_prima.png"
proba "texturae iterum" 0 "$B" texturae -o "$T/texturae.png"
cmp -s "$T/texturae.png" "$T/texturae_prima.png" || { echo "FRACTA: pagina texturarum non determinata"; fracta=$((fracta + 1)); }
proba "texturae sine -o usus" 2 "$B" texturae
proba "texturae cum plagula usus" 2 "$B" texturae "$R/puteus.stml" -o "$T/y.png"
printf '<res titulus="a">\n  <capsa magnitudo="1 1 1" textura="marmor"/>\n</res>\n' > "$T/textura.stml"
proba "textura ignota recusat" 1 "$B" specimen "$T/textura.stml" -o "$T/mala.png"
contine "via et linea texturae" "$T/err" "^$T/textura.stml:2: "
contine "causa texturae" "$T/err" "marmor"

# condere (plastica-plan T7): res in volumen temporarium
S="bin/statuae"
[ -x "$S" ] || { echo "plastica_fumus: $S abest (./tools/statuae_struere.sh)"; exit 1; }
V="$T/proba.volumen"
D="$T/fontes"
mkdir -p "$D"
cp "$R/puteus.stml" "$D/b_puteus.stml"
printf '<res titulus="crate">\n  <capsa magnitudo="1 1 1" textura="tabulae" color="#8c6840" signum="solidum"/>\n</res>\n' > "$D/a_arca.stml"
proba "condere sine -volumen usus" 2 "$B" condere -fontes "$D"
proba "condere vexillum ignotum usus" 2 "$B" condere -volumen "$V" -magnus
proba "condere positivum usus" 2 "$B" condere "$D" -volumen "$V"
[ ! -e "$V" ] || { echo "FRACTA: usus volumen creavit"; fracta=$((fracta + 1)); }
proba "condere directorium absens" 1 "$B" condere -volumen "$V" -fontes "$T/nullum"
mkdir -p "$T/vacuum"
proba "condere directorium vacuum" 1 "$B" condere -volumen "$V" -fontes "$T/vacuum"
contine "nullae plagulae nominantur" "$T/err" "nullae plagulae"
proba "condere duas res" 0 "$B" condere -volumen "$V" -fontes "$D"
contine "arca addita" "$T/out" "^addita    crate$"
contine "puteus additus" "$T/out" "^addita    well$"
contine "summa" "$T/out" "^plastica: 2 additae, 0 mutatae, 0 eaedem, 0 recusatae$"
[ "$(head -1 "$T/out")" = "addita    crate" ] || { echo "FRACTA: ordo nominum (a_arca prima)"; fracta=$((fracta + 1)); }
proba "res in volumine" 0 "$S" enumerare -volumen "$V" -res
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "crate,well," ] || { echo "FRACTA: res in volumine: $(cut -f1 "$T/out" | tr '\n' ',')"; fracta=$((fracta + 1)); }
proba "res non inter personas" 0 "$S" enumerare -volumen "$V"
[ ! -s "$T/out" ] || { echo "FRACTA: res inter personas"; fracta=$((fracta + 1)); }
# nomina partium in GLB condito (octeti massae in volumine crudi)
grep -a -q '"name":"saxum imum"' "$V" || { echo "FRACTA: nomina partium in GLB absunt"; fracta=$((fracta + 1)); }
# cursus alter nihil scribit
proba "condere iterum" 0 "$B" condere -volumen "$V" -fontes "$D"
contine "arca eadem" "$T/out" "^eadem     crate$"
contine "puteus idem" "$T/out" "^eadem     well$"
contine "summa eadem" "$T/out" "^plastica: 0 additae, 0 mutatae, 2 eaedem, 0 recusatae$"
# plagula mutata: octeti substituti, ordo manet
printf '<res titulus="crate">\n  <capsa magnitudo="1 2 1" textura="tabulae" color="#8c6840" signum="solidum"/>\n</res>\n' > "$D/a_arca.stml"
proba "condere mutatam" 0 "$B" condere -volumen "$V" -fontes "$D"
contine "arca mutata" "$T/out" "^mutata    crate$"
contine "puteus intactus" "$T/out" "^eadem     well$"
proba "ordo post mutationem" 0 "$S" enumerare -volumen "$V" -res
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "crate,well," ] || { echo "FRACTA: ordo post mutationem"; fracta=$((fracta + 1)); }
# titulus personae: recusatur, ceterae tamen eunt (review focus 4)
proba "persona addita" 0 "$S" addere probationes/fixa/statuae/lapis.glb -volumen "$V" -titulus "golem"
printf '<res titulus="golem">\n  <capsa magnitudo="1 1 1"/>\n</res>\n' > "$D/c_golem.stml"
printf '<res titulus="post">\n  <capsa magnitudo="0.2 2 0.2" color="#6b4a2b"/>\n</res>\n' > "$D/d_palus.stml"
printf '<res titulus="post">\n  <capsa magnitudo="0.3 2 0.3"/>\n</res>\n' > "$D/e_palus_alter.stml"
printf '<res titulus="broken">\n  <capsa magnitudo="1 1 1"/>\n  <globus radius="1"/>\n</res>\n' > "$D/f_fracta.stml"
proba "condere cum recusatis" 1 "$B" condere -volumen "$V" -fontes "$D"
contine "titulus personae recusatur" "$T/err" "^$D/c_golem.stml: recusata: titulus iam persona aut locus in volumine est: golem$"
contine "titulus bis recusatur" "$T/err" "^$D/e_palus_alter.stml: recusata: titulus iam ab alia plagula huius cursus datus: post$"
contine "fracta cum linea" "$T/err" "^$D/f_fracta.stml:3: "
contine "palus tamen additus" "$T/out" "^addita    post$"
contine "summa cum recusatis" "$T/out" "^plastica: 1 additae, 0 mutatae, 2 eaedem, 3 recusatae$"
proba "persona intacta" 0 "$S" enumerare -volumen "$V"
[ "$(cut -f1 "$T/out")" = "golem" ] || { echo "FRACTA: persona mutata"; fracta=$((fracta + 1)); }
# res sine fonte nominatur, non removetur
rm "$D/b_puteus.stml" "$D/c_golem.stml" "$D/e_palus_alter.stml" "$D/f_fracta.stml"
proba "condere sine fonte putei" 0 "$B" condere -volumen "$V" -fontes "$D"
contine "sine fonte" "$T/out" "^sine fonte: well$"
proba "res sine fonte manet" 0 "$S" enumerare -volumen "$V" -res
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "crate,well,post," ] || { echo "FRACTA: res sine fonte remota: $(cut -f1 "$T/out" | tr '\n' ',')"; fracta=$((fracta + 1)); }
# res condita per lectorem et pictorem it
proba "enumerare cum sigillo" 0 "$S" enumerare -volumen "$V" -res
[ "$(awk -F'\t' '$1=="well"{print length($2)}' "$T/out")" -gt 8 ] || { echo "FRACTA: sigillum putei"; fracta=$((fracta + 1)); }

if [ "$fracta" -eq 0 ]; then echo "PLASTICA FUMUS: OMNIA BONA"; exit 0; fi
echo "PLASTICA FUMUS: $fracta FRACTA"; exit 1
