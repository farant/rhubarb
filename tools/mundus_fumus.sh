#!/bin/bash
# tools/mundus_fumus.sh - fumus bin/mundus (plastica-plan T9): condere
# in volumen temporarium, specimen, recusationes. Nihil extra
# directorium temporarium scribit. Manu curritur post
# ./tools/mundus_struere.sh et ./tools/statuae_struere.sh.
#
# Usus: ./tools/mundus_fumus.sh     Exitus: 0 omnia bona, 1 fracta.
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
B=bin/mundus
S=bin/statuae
F=probationes/fixa/statuae
T="$(mktemp -d "${TMPDIR:-/tmp}/mundus_fumus.XXXXXX")"
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
loci() {  # tituli locorum voluminis, commate iuncti
    "$S" enumerare -volumen "$V" -loci 2>/dev/null | cut -f1 | tr '\n' ','
}
[ -x "$B" ] || { echo "mundus_fumus: $B abest (./tools/mundus_struere.sh)"; exit 1; }
[ -x "$S" ] || { echo "mundus_fumus: $S abest (./tools/statuae_struere.sh)"; exit 1; }
V="$T/proba.volumen"
D="$T/loci"
mkdir -p "$D"

proba "sine argumentis usus" 2 "$B"
proba "imperium ignotum usus" 2 "$B" saltare -volumen "$V"
proba "condere sine -volumen usus" 2 "$B" condere -loci "$D"
proba "condere positivum usus" 2 "$B" condere "$D" -volumen "$V"
proba "condere cum -o usus" 2 "$B" condere -volumen "$V" -o "$T/x.png"
proba "specimen sine -o usus" 2 "$B" specimen plastica/res/puteus.stml
proba "specimen sine re usus" 2 "$B" specimen -o "$T/x.png"
proba "specimen cum -volumen usus" 2 "$B" specimen plastica/res/puteus.stml -o "$T/x.png" -volumen "$V"
proba "specimen locus malus usus" 2 "$B" specimen plastica/res/puteus.stml -o "$T/x.png" -locus "duo tres"
proba "specimen gyrus malus usus" 2 "$B" specimen plastica/res/puteus.stml -o "$T/x.png" -gyrus multum
[ ! -e "$V" ] || { echo "FRACTA: usus volumen creavit"; fracta=$((fracta + 1)); }
[ ! -e "$T/x.png" ] || { echo "FRACTA: usus PNG scripsit"; fracta=$((fracta + 1)); }
proba "condere directorium absens" 1 "$B" condere -volumen "$V" -loci "$T/nullum"
proba "condere directorium vacuum" 1 "$B" condere -volumen "$V" -loci "$D"
contine "nullae plagulae nominantur" "$T/err" "nullae plagulae"

# dispositio repositorii ipsa componitur
proba "dispositiones repositorii" 0 "$B" condere -volumen "$T/vera.volumen"
contine "clearing additus" "$T/out" "^addita    clearing   "

# locus cum puteo
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
</locus>
XX
proba "condere locum" 0 "$B" condere -volumen "$V" -loci "$D"
contine "pratum additum" "$T/out" "^addita    pratum   1 res, [0-9]* vertices, cellae gressibiles 60626 -> "
VERT="$(sed -n 's/^addita    pratum   1 res, \([0-9]*\) vertices.*/\1/p' "$T/out")"
contine "summa" "$T/out" "^mundus: 1 additae, 0 mutatae, 0 eaedem, 0 recusatae$"
grep -q "cellae gressibiles 60626 -> 60626" "$T/out" && { echo "FRACTA: puteus nullam cellam clausit"; fracta=$((fracta + 1)); }
[ "$(loci)" = "pratum," ] || { echo "FRACTA: locus in volumine: $(loci)"; fracta=$((fracta + 1)); }
proba "locus non inter personas" 0 "$S" enumerare -volumen "$V"
[ ! -s "$T/out" ] || { echo "FRACTA: locus inter personas"; fracta=$((fracta + 1)); }
proba "condere iterum" 0 "$B" condere -volumen "$V" -loci "$D"
contine "pratum idem" "$T/out" "^eadem     pratum   "
# dispositio mutata: octeti substituti
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
  <res fons="plastica/res/puteus.stml" locus="24 31"/>
</locus>
XX
proba "condere mutatam" 0 "$B" condere -volumen "$V" -loci "$D"
contine "pratum mutatum" "$T/out" "^mutata    pratum   2 res, "
# res eadem bis posita semel tenetur: vertices non crescunt
contine "puteus bis positus semel tenetur" "$T/out" "^mutata    pratum   2 res, $VERT vertices, "
SIG="$("$S" enumerare -volumen "$V" -loci | cut -f2)"

# recusationes: nihil scribitur, ceterae eunt (review focus 5)
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29"/>
  <res fons="plastica/res/puteus.stml" locus="-500 -500"/>
</locus>
XX
cat > "$D/b_ignota.stml" <<XX
<locus titulus="ignota" basis="$F/planum.glb">

  <res fons="plastica/res/nulla.stml" locus="27 29"/>
</locus>
XX
printf '<res titulus="palus">\n  <capsa magnitudo="0.1 2 0.1" signum="solidum"/>\n</res>\n' > "$T/palus.stml"
cat > "$D/c_palus.stml" <<XX
<locus titulus="palus" basis="$F/planum.glb">
  <res fons="$T/palus.stml" locus="27 29"/>
</locus>
XX
cat > "$D/d_basis.stml" <<XX
<locus titulus="sine basi" basis="$T/nulla.glb"/>
XX
printf 'non stml <' > "$D/e_fracta.stml"
proba "persona addita" 0 "$S" addere "$F/lapis.glb" -volumen "$V" -titulus "golem"
cat > "$D/f_golem.stml" <<XX
<locus titulus="golem" basis="$F/planum.glb"/>
XX
cat > "$D/g_nudus.stml" <<XX
<locus titulus="nudus" basis="$F/planum.glb"/>
XX
cat > "$D/h_nudus_alter.stml" <<XX
<locus titulus="nudus" basis="$F/planum.glb"/>
XX
proba "condere cum recusatis" 1 "$B" condere -volumen "$V" -loci "$D"
contine "extra locum cum linea" "$T/err" "^$D/a_pratum.stml:3: the spot lies outside the level$"
contine "fons ignotus nominatur" "$T/err" "^plastica/res/nulla.stml:0: "
contine "positio fontis ignoti" "$T/err" "^$D/b_ignota.stml:3: "
contine "palus tenuis cum linea" "$T/err" "^$D/c_palus.stml:2: the asset is marked solid but blocks no cell"
contine "basis absens" "$T/err" "^$D/d_basis.stml: recusata: basis legi non potest"
contine "non STML" "$T/err" "^$D/e_fracta.stml:"
contine "titulus personae" "$T/err" "^$D/f_golem.stml: recusata: titulus iam persona aut res in volumine est: golem$"
contine "titulus bis" "$T/err" "^$D/h_nudus_alter.stml: recusata: titulus iam ab alia dispositione huius cursus datus: nudus$"
contine "nudus tamen additus" "$T/out" "^addita    nudus   0 res, [0-9]* vertices, cellae gressibiles 60626 -> 60626$"
contine "summa cum recusatis" "$T/out" "^mundus: 1 additae, 0 mutatae, 0 eaedem, 7 recusatae$"
[ "$(loci)" = "pratum,nudus," ] || { echo "FRACTA: loci post recusationes: $(loci)"; fracta=$((fracta + 1)); }
[ "$("$S" enumerare -volumen "$V" -loci | head -1 | cut -f2)" = "$SIG" ] || { echo "FRACTA: dispositio recusata locum mutavit"; fracta=$((fracta + 1)); }

# specimen
proba "specimen putei" 0 "$B" specimen plastica/res/puteus.stml -o "$T/s.png"
file "$T/s.png" 2>/dev/null | grep -q "960 x 360" || { echo "FRACTA: specimen: PNG 960 x 360 abest"; fracta=$((fracta + 1)); }
contine "specimen locum dicit" "$T/out" "res ad (.*) altitudo .*persona ad ("
contine "specimen cellas dicit" "$T/out" "^cellae gressibiles 60626 -> "
cp "$T/s.png" "$T/s_prima.png"
proba "specimen iterum" 0 "$B" specimen plastica/res/puteus.stml -o "$T/s.png"
cmp -s "$T/s.png" "$T/s_prima.png" || { echo "FRACTA: specimen non determinatum"; fracta=$((fracta + 1)); }
proba "specimen loco dato" 0 "$B" specimen plastica/res/puteus.stml -o "$T/t.png" -locus "27 29" -gyrus 20
contine "locus datus" "$T/out" "res ad (27.00, 29.00) altitudo 4.50, gyrus 20"
cmp -s "$T/t.png" "$T/s.png" && { echo "FRACTA: -locus picturam non mutavit"; fracta=$((fracta + 1)); }
# tabulae duae differunt; persona in utraque picta
DA="$(sed -n 's/^tabula atkinson: digestum \([0-9a-f]*\),.*/\1/p' "$T/out")"
DE="$(sed -n 's/^tabula exemplaria: digestum \([0-9a-f]*\),.*/\1/p' "$T/out")"
[ -n "$DA" ] && [ -n "$DE" ] && [ "$DA" != "$DE" ] || { echo "FRACTA: tabulae Atkinson et exemplaria non differunt ($DA / $DE)"; fracta=$((fracta + 1)); }
for t in atkinson exemplaria; do
    P="$(sed -n "s/^tabula $t: digestum [0-9a-f]*, persona \([0-9]*\) pixela$/\1/p" "$T/out")"
    [ "${P:-0}" -gt 500 ] || { echo "FRACTA: persona in tabula $t non picta (${P:-nulla} pixela)"; fracta=$((fracta + 1)); }
done
# gyrus picturam mutat
proba "specimen gyro alio" 0 "$B" specimen plastica/res/puteus.stml -o "$T/g.png" -locus "27 29" -gyrus 110
cmp -s "$T/g.png" "$T/t.png" && { echo "FRACTA: -gyrus picturam non mutavit"; fracta=$((fracta + 1)); }
proba "specimen extra locum" 1 "$B" specimen plastica/res/puteus.stml -o "$T/u.png" -locus "-500 -500"
contine "causa loci" "$T/err" "outside the level"
[ ! -e "$T/u.png" ] || { echo "FRACTA: recusatum PNG scripsit"; fracta=$((fracta + 1)); }
proba "specimen rei fractae" 1 "$B" specimen "$D/e_fracta.stml" -o "$T/u.png"
proba "specimen rei absentis" 1 "$B" specimen "$T/nulla.stml" -o "$T/u.png"
proba "provenientia" 0 "$B" -provenientia

if [ "$fracta" -eq 0 ]; then echo "MUNDUS FUMUS: OMNIA BONA"; exit 0; fi
echo "MUNDUS FUMUS: $fracta FRACTA"; exit 1
