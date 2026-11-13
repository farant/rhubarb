#!/bin/bash
# tools/scaena_fumus.sh - fumus bin/scaena (graphis-plan T7): casa
# (apps/scaena/loci) in volumen temporarium condita, index, remotio
# et recusationes. Nihil extra directorium temporarium scribit. Manu
# curritur post ./tools/scaena_struere.sh.
#
# Usus: ./tools/scaena_fumus.sh     Exitus: 0 omnia bona, 1 fracta.
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
B=bin/scaena
L=apps/scaena/loci
T="$(mktemp -d "${TMPDIR:-/tmp}/scaena_fumus.XXXXXX")"
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
caret() {  # caret <descriptio> <plagula> <textus>
    ! grep -q -- "$3" "$2" || { echo "FRACTA: $1 ('$3' adest)"; fracta=$((fracta + 1)); }
}
summa_voluminis() { cksum < "$1"; }
[ -x "$B" ] || { echo "scaena_fumus: $B abest (./tools/scaena_struere.sh)"; exit 1; }
V="$T/proba.volumen"

proba "sine argumentis usus" 2 "$B"
proba "imperium ignotum usus" 2 "$B" saltare -volumen "$V"
proba "condere sine -volumen usus" 2 "$B" condere
proba "condere positivum usus" 2 "$B" condere "$L" -volumen "$V"
proba "condere vexillum ignotum usus" 2 "$B" condere -volumen "$V" -magnus
proba "condere -volumen bis usus" 2 "$B" condere -volumen "$V" -volumen "$V"
proba "condere -fontes sine valore usus" 2 "$B" condere -volumen "$V" -fontes
proba "removere sine titulo usus" 2 "$B" removere -volumen "$V"
proba "removere duo tituli usus" 2 "$B" removere -volumen "$V" a b
proba "removere sine -volumen usus" 2 "$B" removere hut-door
proba "index cum -fontes usus" 2 "$B" index -volumen "$V" -fontes "$L"
proba "index sine -volumen usus" 2 "$B" index
[ ! -e "$V" ] || { echo "FRACTA: usus volumen creavit"; fracta=$((fracta + 1)); }
proba "provenientia" 0 "$B" -provenientia

proba "condere directorium absens" 1 "$B" condere -volumen "$V" -fontes "$T/nullum"
mkdir -p "$T/vacuum"
cp "$L/casa_focus.stml" "$T/vacuum/"
proba "condere sine plagulis scaenarum" 1 "$B" condere -volumen "$V" -fontes "$T/vacuum"
contine "nullae plagulae nominantur" "$T/err" "nullae plagulae"
proba "index voluminis absentis" 1 "$B" index -volumen "$V"
proba "removere ex volumine absenti" 1 "$B" removere -volumen "$V" hut-door
[ ! -e "$V" ] || { echo "FRACTA: recusatio volumen creavit"; fracta=$((fracta + 1)); }

# casa vera, directoriis ordinariis
proba "condere casam" 0 "$B" condere -volumen "$V"
contine "focus additus" "$T/out" "^addita    hut-hearth$"
contine "ostium additum" "$T/out" "^addita    hut-door$"
contine "summa" "$T/out" "^scaena: 2 additae, 0 mutatae, 0 eaedem$"
[ "$(head -1 "$T/out")" = "addita    hut-hearth" ] || { echo "FRACTA: ordo plagulae (hut-hearth primus)"; fracta=$((fracta + 1)); }
caret "nullus sine fonte" "$T/out" "sine fonte"
proba "index casae" 0 "$B" index -volumen "$V"
contine "focus cum rebus" "$T/out" "^hut-hearth  (5 res)$"
contine "ostium cum rebus" "$T/out" "^hut-door  (4 res)$"
contine "res sine exitu" "$T/out" "^    hearth$"
contine "exitus in prospectum" "$T/out" "^    turn -> hut-door$"
contine "exitus foras" "$T/out" "^    door -> mundus$"
contine "summa indicis" "$T/out" "^scaena: 2 prospectus$"
cp "$T/out" "$T/index_primus"
# cursus alter nihil scribit
proba "condere iterum" 0 "$B" condere -volumen "$V"
contine "focus idem" "$T/out" "^eadem     hut-hearth$"
contine "ostium idem" "$T/out" "^eadem     hut-door$"
contine "summa eadem" "$T/out" "^scaena: 0 additae, 0 mutatae, 2 eaedem$"

# textus mutatus: massa substituta, ordo manet
D="$T/fontes"
mkdir -p "$D"
cp "$L"/*.stml "$D/"
sed 's/You turn your back to the fire\./You turn away./' "$L/casa.scaena.stml" > "$D/casa.scaena.stml"
cmp -s "$L/casa.scaena.stml" "$D/casa.scaena.stml" && { echo "FRACTA: fumus: textus mutandus non inventus"; fracta=$((fracta + 1)); }
proba "condere mutatam" 0 "$B" condere -volumen "$V" -fontes "$D"
contine "focus mutatus" "$T/out" "^mutata    hut-hearth$"
contine "ostium intactum" "$T/out" "^eadem     hut-door$"
contine "summa mutatae" "$T/out" "^scaena: 0 additae, 1 mutatae, 1 eaedem$"
proba "index post mutationem" 0 "$B" index -volumen "$V"
cmp -s "$T/out" "$T/index_primus" || { echo "FRACTA: index post mutationem alius"; fracta=$((fracta + 1)); }

# recusationes: NIHIL scribitur, etiam si plagulae aliae bonae et mutatae
ante="$(summa_voluminis "$V")"
intactum() {  # intactum <descriptio>
    [ "$(summa_voluminis "$V")" = "$ante" ] || { echo "FRACTA: $1: volumen mutatum"; fracta=$((fracta + 1)); }
    contine "$1: nihil scriptum nuntiatur" "$T/err" "nihil scriptum$"
    [ ! -s "$T/out" ] || { echo "FRACTA: $1: stdout non vacuum"; fracta=$((fracta + 1)); }
}
cp "$L/casa.scaena.stml" "$D/casa.scaena.stml"
printf '<scaenae>\n  <prospectus titulus="cella" pictura="nulla_pictura">\n    <descriptio>A cellar.</descriptio>\n  </prospectus>\n</scaenae>\n' > "$D/z_mala.scaena.stml"
proba "pictura absens recusat" 1 "$B" condere -volumen "$V" -fontes "$D"
contine "via plagulae et linea" "$T/err" "^$D/z_mala.scaena.stml:2: .*nulla_pictura"
intactum "pictura absens"
rm "$D/z_mala.scaena.stml"
# recusatio in pictura nata: via picturae
sed 's/<grex titulus="hearth">/<grex titulus="furnace">/' "$L/casa_focus.stml" > "$D/casa_focus.stml"
proba "regio sine re recusat" 1 "$B" condere -volumen "$V" -fontes "$D"
contine "plagula scaenarum nominatur" "$T/err" "^$D/casa.scaena.stml:[0-9]*: "
intactum "regio sine re"
printf '<pictura>\n  <circulus/>\n</pictura>\n' > "$D/casa_focus.stml"
proba "pictura fracta recusat" 1 "$B" condere -volumen "$V" -fontes "$D"
contine "via picturae et linea" "$T/err" "^$D/casa_focus.stml:2: .*circulus"
intactum "pictura fracta"
cp "$L/casa_focus.stml" "$D/casa_focus.stml"
# recusatio in parte nata: via partis
P="$T/partes"
mkdir -p "$P"
cp graphis/res/*.stml "$P/"
printf '<pars>\n\n  <rectangulum fines="0 0 20" colores="6"/>\n</pars>\n' > "$P/sella.stml"
proba "pars fracta recusat" 1 "$B" condere -volumen "$V" -fontes "$D" -partes "$P"
contine "via partis et linea" "$T/err" "^$P/sella.stml:3: "
intactum "pars fracta"
# titulus idem in duabus plagulis
cp "$L/casa.scaena.stml" "$D/z_iterata.scaena.stml"
proba "titulus bis recusat" 1 "$B" condere -volumen "$V" -fontes "$D"
contine "titulus bis nominatur" "$T/err" "^$D/z_iterata.scaena.stml: recusata: titulus prospectus iam ab alia plagula huius cursus datus: hut-hearth$"
intactum "titulus bis"
rm "$D/z_iterata.scaena.stml"
proba "index post recusationes" 0 "$B" index -volumen "$V"
cmp -s "$T/out" "$T/index_primus" || { echo "FRACTA: index post recusationes alius"; fracta=$((fracta + 1)); }

# prospectus sine fonte nominantur, non removentur
E="$T/alia"
mkdir -p "$E"
cp "$L"/casa_*.stml "$E/"
sed 's/hut-/cot-/g' "$L/casa.scaena.stml" > "$E/tugurium.scaena.stml"
proba "condere alios" 0 "$B" condere -volumen "$V" -fontes "$E"
contine "alius additus" "$T/out" "^addita    cot-hearth$"
contine "focus sine fonte" "$T/out" "^sine fonte: hut-hearth$"
contine "ostium sine fonte" "$T/out" "^sine fonte: hut-door$"
contine "summa aliorum" "$T/out" "^scaena: 2 additae, 0 mutatae, 0 eaedem$"
proba "index quattuor" 0 "$B" index -volumen "$V"
[ "$(grep -v '^ ' "$T/out" | cut -d' ' -f1 | tr '\n' ',')" = "hut-hearth,hut-door,cot-hearth,cot-door,scaena:," ] || { echo "FRACTA: ordo indicis: $(grep -v '^ ' "$T/out" | cut -d' ' -f1 | tr '\n' ',')"; fracta=$((fracta + 1)); }
contine "exitus renominatus" "$T/out" "^    turn -> cot-door$"

# removere
proba "removere" 0 "$B" removere -volumen "$V" cot-hearth
contine "remota nuntiatur" "$T/out" "^remota    cot-hearth$"
proba "removere iterum recusat" 1 "$B" removere -volumen "$V" cot-hearth
contine "absens nominatur" "$T/err" "prospectus 'cot-hearth' in volumine non est"
proba "removere titulum malum" 1 "$B" removere -volumen "$V" "Malus Titulus"
proba "index post remotionem" 0 "$B" index -volumen "$V"
contine "tres manent" "$T/out" "^scaena: 3 prospectus$"
caret "remotus abest" "$T/out" "^cot-hearth"

if [ "$fracta" -eq 0 ]; then
    echo "SCAENA FUMUS: omnia bona"; exit 0
fi
echo "SCAENA FUMUS: $fracta fracta"; exit 1
