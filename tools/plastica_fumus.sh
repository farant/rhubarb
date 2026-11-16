#!/bin/bash
# tools/plastica_fumus.sh - fumus bin/plastica (plastica-plan T3, T4, T7;
# plastica-motus-plan T3):
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

# figura cum motu (plastica-motus-plan T3): numeri cyclorum et linea
# motus in PNG alterum
cat > "$T/pupa.stml" <<'FIGURA'
<figura titulus="pupa">
  <articulus titulus="corpus">
    <capsa magnitudo="1 2 1" color="#2040c0"/>
  </articulus>
  <positura titulus="alta"><art titulus="corpus" motus="0 0.5 0"/></positura>
  <cyclus titulus="salit" duratio="1" circulus="verum">
    <clavis tempus="0" positura="quies"/>
    <clavis tempus="0.5" positura="alta"/>
    <momentum titulus="apex" tempus="0.5"/>
  </cyclus>
  <cyclus titulus="stat" duratio="2">
    <clavis tempus="0" positura="alta"/>
  </cyclus>
</figura>
FIGURA
proba "specimen figurae cum motu" 0 "$B" specimen "$T/pupa.stml" -o "$T/pupa.png" -motus "$T/pupa_motus.png"
file "$T/pupa.png" 2>/dev/null | grep -q "640 x 640" || { echo "FRACTA: figura: pagina 640 x 640 abest"; fracta=$((fracta + 1)); }
file "$T/pupa_motus.png" 2>/dev/null | grep -q "1280 x 480" || { echo "FRACTA: figura: linea motus 1280 x 480 abest"; fracta=$((fracta + 1)); }
contine "cyclus primus" "$T/out" "^cyclus 0 salit: duratio 1.00  infimum 0.00 .. 0.50  hiatus 0.000$"
contine "momentum cycli" "$T/out" "^  momentum apex 0.50$"
contine "cyclus alter" "$T/out" "^cyclus 1 stat: duratio 2.00  infimum 0.50 .. 0.50  hiatus 0.000$"
contine "mensura lineae" "$T/out" "pupa_motus.png: 1280 x 480$"
[ "$(grep -c '  momentum ' "$T/out")" -eq 1 ] || { echo "FRACTA: momentum sub cyclo alieno"; fracta=$((fracta + 1)); }
cp "$T/pupa_motus.png" "$T/motus_prima.png"
proba "specimen figurae iterum" 0 "$B" specimen "$T/pupa.stml" -o "$T/pupa.png" -motus "$T/pupa_motus.png"
cmp -s "$T/pupa_motus.png" "$T/motus_prima.png" || { echo "FRACTA: linea motus non determinata"; fracta=$((fracta + 1)); }
# sine -motus: numeri tamen, linea nulla
rm -f "$T/pupa_motus.png"
proba "specimen figurae sine -motus" 0 "$B" specimen "$T/pupa.stml" -o "$T/pupa.png"
contine "numeri sine linea" "$T/out" "^cyclus 0 salit: "
[ ! -e "$T/pupa_motus.png" ] || { echo "FRACTA: linea sine -motus scripta"; fracta=$((fracta + 1)); }
# -motus rem (non figuram) recusat; nihil scribitur
proba "-motus cum re recusat" 1 "$B" specimen "$R/puteus.stml" -o "$T/r.png" -motus "$T/r_motus.png"
contine "causa -motus cum re" "$T/err" "figura"
[ ! -e "$T/r.png" ] && [ ! -e "$T/r_motus.png" ] || { echo "FRACTA: -motus cum re PNG scripsit"; fracta=$((fracta + 1)); }
proba "-motus sine via usus" 2 "$B" specimen "$T/pupa.stml" -o "$T/x.png" -motus
proba "-motus bis usus" 2 "$B" specimen "$T/pupa.stml" -o "$T/x.png" -motus "$T/a.png" -motus "$T/b.png"
proba "texturae cum -motus usus" 2 "$B" texturae -o "$T/y.png" -motus "$T/a.png"
proba "via lineae mala" 1 "$B" specimen "$T/pupa.stml" -o "$T/pupa.png" -motus "$T/nullum/dir/m.png"
# figura sine cyclis: pagina, linea nulla, exitus 0
printf '<figura titulus="a">\n<articulus titulus="b"><capsa magnitudo="1 1 1"/></articulus>\n</figura>\n' > "$T/quieta.stml"
proba "figura sine cyclis" 0 "$B" specimen "$T/quieta.stml" -o "$T/quieta.png" -motus "$T/quieta_motus.png"
contine "sine cyclis nuntiatur" "$T/out" "figura sine cyclis"
[ -e "$T/quieta.png" ] && [ ! -e "$T/quieta_motus.png" ] || { echo "FRACTA: figura sine cyclis: pagina aut linea"; fracta=$((fracta + 1)); }
# motus recusatus: via et linea, nihil scriptum
printf '<figura titulus="a">\n<articulus titulus="b"><capsa magnitudo="1 1 1"/></articulus>\n<cyclus titulus="c" duratio="1">\n<clavis tempus="0" positura="nulla"/>\n</cyclus>\n</figura>\n' > "$T/motus_malus.stml"
proba "motus recusatus" 1 "$B" specimen "$T/motus_malus.stml" -o "$T/mm.png" -motus "$T/mm_motus.png"
contine "via et linea motus" "$T/err" "^$T/motus_malus.stml:4: "
contine "causa motus" "$T/err" "nulla"
[ ! -e "$T/mm.png" ] && [ ! -e "$T/mm_motus.png" ] || { echo "FRACTA: motus recusatus PNG scripsit"; fracta=$((fracta + 1)); }

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

# condere figuras (plastica-motus-plan T4): personae in volumen
VF="$T/figurae.volumen"
DF="$T/fontes_f"
G="$T/figurae"
mkdir -p "$DF" "$G"
printf '<res titulus="cista">\n  <capsa magnitudo="1 1 1"/>\n</res>\n' > "$DF/a_cista.stml"
cp "$T/pupa.stml" "$G/b_pupa.stml"
proba "condere -figurae sine via usus" 2 "$B" condere -volumen "$VF" -fontes "$DF" -figurae
proba "condere -figurae bis usus" 2 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G" -figurae "$G"
proba "condere figurae absentes" 1 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$T/nullum"
proba "condere figurae vacuae" 1 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$T/vacuum"
contine "figurae vacuae nominantur" "$T/err" "nullae plagulae"
[ ! -e "$VF" ] || { echo "FRACTA: directorium figurarum malum volumen creavit"; fracta=$((fracta + 1)); }
proba "condere figuram" 0 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G"
contine "res ante figuras" "$T/out" "^addita    cista$"
contine "figura addita" "$T/out" "^figura addita    pupa$"
contine "summa rerum" "$T/out" "^plastica: 1 additae, 0 mutatae, 0 eaedem, 0 recusatae$"
contine "summa figurarum" "$T/out" "^plastica: figurae: 1 additae, 0 mutatae, 0 eaedem, 0 recusatae$"
[ "$(tail -1 "$T/out")" = "plastica: figurae: 1 additae, 0 mutatae, 0 eaedem, 0 recusatae" ] || { echo "FRACTA: summa figurarum non ultima"; fracta=$((fracta + 1)); }
proba "figura inter personas" 0 "$S" enumerare -volumen "$VF"
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "pupa," ] || { echo "FRACTA: figura inter personas: $(cut -f1 "$T/out" | tr '\n' ',')"; fracta=$((fracta + 1)); }
proba "figura non inter res" 0 "$S" enumerare -volumen "$VF" -res
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "cista," ] || { echo "FRACTA: figura inter res"; fracta=$((fracta + 1)); }
# nomina articulorum et momenta cum figura condita eunt (plastica-motus-plan T8)
proba "inspicere figuram" 0 "$S" inspicere pupa -volumen "$VF"
contine "summa inspectionis" "$T/out" "^pupa: 3 nodi, 2 cycli, 1 momenta$"
contine "nomen articuli conditum" "$T/out" "^nodus 1 corpus$"
contine "momentum conditum" "$T/out" "^momentum salit apex 0.50$"
[ "$(grep -c '^momentum ' "$T/out")" -eq 1 ] || { echo "FRACTA: momenta condita non I"; fracta=$((fracta + 1)); }
# cycli et nomina articulorum in GLB condito
grep -a -q '"name":"salit"' "$VF" || { echo "FRACTA: cyclus in GLB abest"; fracta=$((fracta + 1)); }
grep -a -q '"name":"corpus"' "$VF" || { echo "FRACTA: nomen articuli in GLB abest"; fracta=$((fracta + 1)); }
proba "condere figuram iterum" 0 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G"
contine "figura eadem" "$T/out" "^figura eadem     pupa$"
contine "summa figurarum eadem" "$T/out" "^plastica: figurae: 0 additae, 0 mutatae, 1 eaedem, 0 recusatae$"
sed 's/duratio="2"/duratio="3"/' "$T/pupa.stml" > "$G/b_pupa.stml"
proba "condere figuram mutatam" 0 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G"
contine "figura mutata" "$T/out" "^figura mutata    pupa$"
# -fontes sine -figurae: figurae non tanguntur
proba "condere sine figuris" 0 "$B" condere -volumen "$VF" -fontes "$DF"
[ "$(grep -c figura "$T/out")" -eq 0 ] || { echo "FRACTA: figurae sine -figurae tactae"; fracta=$((fracta + 1)); }
# recusatae: ceterae tamen eunt; persona empta 'sine fonte' non dicitur
proba "persona empta" 0 "$S" addere probationes/fixa/statuae/fungus.glb -volumen "$VF" -titulus "fungus"
cp "$T/motus_malus.stml" "$G/a_mala.stml"
printf '<res titulus="res_inter_figuras">\n  <capsa magnitudo="1 1 1"/>\n</res>\n' > "$G/c_res.stml"
sed 's/titulus="pupa"/titulus="cista"/' "$T/pupa.stml" > "$G/d_cista.stml"
cp "$T/pupa.stml" "$G/e_pupa_altera.stml"
sed 's/titulus="pupa"/titulus="nova"/' "$T/pupa.stml" > "$G/f_nova.stml"
sed 's/titulus="pupa"/titulus="inter_res"/' "$T/pupa.stml" > "$DF/b_figura.stml"
proba "res antiqua in volumine" 0 "$S" addere probationes/fixa/statuae/fungus.glb -volumen "$VF" -titulus "antiqua" -res
sed 's/titulus="pupa"/titulus="antiqua"/' "$T/pupa.stml" > "$G/g_antiqua.stml"
proba "condere figuras cum recusatis" 1 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G"
contine "motus malus cum linea" "$T/err" "^$G/a_mala.stml:4: "
contine "res inter figuras recusatur" "$T/err" "^$G/c_res.stml:1: .*<figura>"
contine "titulus rei huius cursus recusatur" "$T/err" "^$G/d_cista.stml: recusata: titulus iam ab alia plagula huius cursus datus: cista$"
contine "titulus rei in volumine recusatur" "$T/err" "^$G/g_antiqua.stml: recusata: titulus iam res aut locus in volumine est: antiqua$"
contine "figura bis recusatur" "$T/err" "^$G/e_pupa_altera.stml: recusata: titulus iam ab alia plagula huius cursus datus: pupa$"
contine "figura inter res recusatur" "$T/err" "^$DF/b_figura.stml: recusata: figura inter res (directorium figurarum eam poscit): inter_res$"
contine "nova tamen addita" "$T/out" "^figura addita    nova$"
contine "summa figurarum cum recusatis" "$T/out" "^plastica: figurae: 1 additae, 0 mutatae, 1 eaedem, 5 recusatae$"
contine "summa rerum cum recusata" "$T/out" "^plastica: 0 additae, 0 mutatae, 1 eaedem, 1 recusatae$"
contine "res sine fonte tamen dicitur" "$T/out" "^sine fonte: antiqua$"
[ "$(grep -c 'sine fonte' "$T/out")" -eq 1 ] || { echo "FRACTA: persona 'sine fonte' dicta"; fracta=$((fracta + 1)); }
proba "personae post recusatas" 0 "$S" enumerare -volumen "$VF"
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "pupa,fungus,nova," ] || { echo "FRACTA: personae post recusatas: $(cut -f1 "$T/out" | tr '\n' ',')"; fracta=$((fracta + 1)); }
# figurae solae recusatae: exitus tamen 1
rm "$DF/b_figura.stml"
proba "condere figuras solas recusatas" 1 "$B" condere -volumen "$VF" -fontes "$DF" -figurae "$G"
contine "res omnes bonae" "$T/out" "^plastica: 0 additae, 0 mutatae, 1 eaedem, 0 recusatae$"
# figurae cum ossibus (plastica-motus-plan T5): plagula ossium in
# directorio 'ossa' iuxta directorium figurae
O="$T/corpora"
mkdir -p "$O/figurae" "$O/ossa" "$T/fontes_o"
printf '<res titulus="arca">\n  <capsa magnitudo="1 1 1"/>\n</res>\n' > "$T/fontes_o/a_arca.stml"
cat > "$O/ossa/duo.stml" <<'OSSA'
<ossa titulus="duo">
  <articulus titulus="pelvis" locus="0 1 0">
    <articulus titulus="crus_sin" locus="0.2 0 0"/>
    <articulus titulus="crus_dex" locus="-0.2 0 0"/>
  </articulus>
  <positura titulus="gradus">
    <art titulus="crus_sin" rotatio="30 0 0"/>
  </positura>
  <positura titulus="gradus_dex" speculum="gradus"/>
  <cyclus titulus="walk" duratio="1" circulus="verum">
    <clavis tempus="0" positura="gradus"/>
    <clavis tempus="0.5" positura="gradus_dex"/>
  </cyclus>
</ossa>
OSSA
cat > "$O/figurae/gemellus.stml" <<'FIGURA'
<figura titulus="gemellus" ossa="duo">
  <articulus titulus="pelvis"><capsa magnitudo="0.6 0.6 0.4"/></articulus>
  <articulus titulus="crus_sin"><capsa magnitudo="0.2 1 0.2" locus="0 -1 0"/></articulus>
  <articulus titulus="crus_dex"><capsa magnitudo="0.2 1 0.2" locus="0 -1 0"/></articulus>
</figura>
FIGURA
proba "specimen figurae cum ossibus" 0 "$B" specimen "$O/figurae/gemellus.stml" -o "$T/gem.png" -motus "$T/gem_motus.png"
contine "cyclus ossium" "$T/out" "^cyclus 0 walk: duratio 1.00 "
contine "solida figurae in ossibus" "$T/out" "gemellus.stml: 3 partes "
file "$T/gem_motus.png" 2>/dev/null | grep -q "1280 x 240" || { echo "FRACTA: ossa: linea motus 1280 x 240 abest"; fracta=$((fracta + 1)); }
# via sine directorio: ossa in ../ossa loci currentis
BA="$(pwd)/$B"
( cd "$O/figurae" && "$BA" specimen gemellus.stml -o "$T/gem_nuda.png" > "$T/out_nuda" 2> "$T/err_nuda" ) || { echo "FRACTA: specimen via nuda: $(cat "$T/err_nuda")"; fracta=$((fracta + 1)); }
# ossa absentia: via figurae, linea attributi
sed 's/ossa="duo"/ossa="nulla"/' "$O/figurae/gemellus.stml" > "$T/orbus.stml"
mkdir -p "$T/ossa"
proba "ossa absentia" 1 "$B" specimen "$T/orbus.stml" -o "$T/orbus.png"
contine "ossa absentia nominantur" "$T/err" "^$T/orbus.stml:1: unknown skeleton 'nulla'$"
[ ! -e "$T/orbus.png" ] || { echo "FRACTA: ossa absentia PNG scripsit"; fracta=$((fracta + 1)); }
# error in plagula ossium: via ET linea illius plagulae
sed 's/titulus="crus_sin" rotatio/titulus="genu" rotatio/' "$O/ossa/duo.stml" > "$O/ossa/mala.stml"
sed 's/ossa="duo"/ossa="mala"/; s/titulus="gemellus"/titulus="claudus"/' "$O/figurae/gemellus.stml" > "$O/claudus.stml"
mkdir -p "$O/alia" && mv "$O/claudus.stml" "$O/alia/claudus.stml"
proba "error in ossibus" 1 "$B" specimen "$O/alia/claudus.stml" -o "$T/claudus.png"
contine "via et linea ossium" "$T/err" "^$O/alia/../ossa/mala.stml:7: .*genu"
# error in figura cum ossibus: via figurae
sed 's/titulus="crus_dex"/titulus="cauda"/' "$O/figurae/gemellus.stml" > "$O/alia/caudatus.stml"
proba "error in figura cum ossibus" 1 "$B" specimen "$O/alia/caudatus.stml" -o "$T/caudatus.png"
contine "via figurae" "$T/err" "^$O/alia/caudatus.stml:4: .*cauda"
# condere: figura cum ossibus persona fit; ossa ipsa non conduntur
proba "condere figuram cum ossibus" 0 "$B" condere -volumen "$T/ossa.volumen" -fontes "$T/fontes_o" -figurae "$O/figurae"
contine "figura cum ossibus addita" "$T/out" "^figura addita    gemellus$"
proba "condere figuram cum ossibus iterum" 0 "$B" condere -volumen "$T/ossa.volumen" -fontes "$T/fontes_o" -figurae "$O/figurae"
contine "figura cum ossibus eadem" "$T/out" "^figura eadem     gemellus$"
# ossa mutata figuram mutant
sed 's/rotatio="30 0 0"/rotatio="35 0 0"/' "$O/ossa/duo.stml" > "$T/duo.nova" && mv "$T/duo.nova" "$O/ossa/duo.stml"
proba "condere post ossa mutata" 0 "$B" condere -volumen "$T/ossa.volumen" -fontes "$T/fontes_o" -figurae "$O/figurae"
contine "ossa mutata figuram mutant" "$T/out" "^figura mutata    gemellus$"
proba "personae cum ossibus" 0 "$S" enumerare -volumen "$T/ossa.volumen"
[ "$(cut -f1 "$T/out" | tr '\n' ',')" = "gemellus," ] || { echo "FRACTA: personae cum ossibus: $(cut -f1 "$T/out" | tr '\n' ',')"; fracta=$((fracta + 1)); }

# membra (plastica-motus-plan T6): figura vera cum ossibus bipedis -
# lapsus pedum in cyclo "walk" solo
proba "specimen goblini" 0 "$B" specimen plastica/figurae/goblin.stml -o "$T/goblin.png" -motus "$T/goblin_motus.png"
contine "lapsus gressus" "$T/out" "^cyclus [0-9]* walk: .*  hiatus [0-9.]*  lapsus [0-9]\.[0-9][0-9]$"
contine "idle sine lapsu" "$T/out" "^cyclus [0-9]* idle: .*  hiatus [0-9.]*$"
[ "$(grep -c '  lapsus ' "$T/out")" -eq 1 ] || { echo "FRACTA: lapsus extra gressum"; fracta=$((fracta + 1)); }
# celeritas ad altitudinem figurae relata: figura I alta quae stat,
# gressus 0.5 s - pes immotus 4 * (1 / 2) * 0.5 = 1.00 labitur
cat > "$T/stans.stml" <<'FIGURA'
<figura titulus="stans">
  <articulus titulus="pelvis" locus="0 0.5 0">
    <capsa magnitudo="0.4 0.5 0.4"/>
    <articulus titulus="femur">
      <articulus titulus="crus" locus="0 -0.25 0">
        <articulus titulus="pes" locus="0 -0.25 0">
          <capsa magnitudo="0.1 0.1 0.1"/>
        </articulus>
      </articulus>
    </articulus>
  </articulus>
  <membrum titulus="perna" radix="femur" medium="crus" finis="pes" flexus="ante"/>
  <positura titulus="stat"><art titulus="pelvis" rotatio="0 0 0"/></positura>
  <cyclus titulus="walk" duratio="0.5">
    <clavis tempus="0" positura="stat"/>
  </cyclus>
</figura>
FIGURA
proba "specimen stantis" 0 "$B" specimen "$T/stans.stml" -o "$T/stans.png"
contine "lapsus ad altitudinem" "$T/out" "^cyclus 0 walk: duratio 0.50 .*  lapsus 1\.00$"
# figura sine membris: gressus sine lapsu
proba "gressus sine membris" 0 "$B" specimen "$O/figurae/gemellus.stml" -o "$T/gem.png"
contine "walk sine lapsu" "$T/out" "^cyclus 0 walk: .*  hiatus [0-9.]*$"
# directoria ordinaria: plastica/res et plastica/figurae sub loco currenti;
# figurae absentes sine errore
mkdir -p "$T/alibi/plastica/res"
cp "$DF/a_cista.stml" "$T/alibi/plastica/res/"
( cd "$T/alibi" && "$RADIX_DIR/$B" condere -volumen "$T/alibi.volumen" >"$T/out" 2>"$T/err" ); rc=$?
[ "$rc" -eq 0 ] || { echo "FRACTA: ordinaria sine figuris (exitus $rc)"; fracta=$((fracta + 1)); }
[ "$(grep -c figura "$T/out")" -eq 0 ] || { echo "FRACTA: figurae ordinariae absentes nuntiatae"; fracta=$((fracta + 1)); }
mkdir -p "$T/alibi/plastica/figurae"
( cd "$T/alibi" && "$RADIX_DIR/$B" condere -volumen "$T/alibi.volumen" >"$T/out" 2>"$T/err" ); rc=$?
[ "$rc" -eq 0 ] || { echo "FRACTA: figurae ordinariae vacuae (exitus $rc)"; fracta=$((fracta + 1)); }
cp "$T/pupa.stml" "$T/alibi/plastica/figurae/"
( cd "$T/alibi" && "$RADIX_DIR/$B" condere -volumen "$T/alibi.volumen" >"$T/out" 2>"$T/err" ); rc=$?
[ "$rc" -eq 0 ] || { echo "FRACTA: figurae ordinariae (exitus $rc)"; fracta=$((fracta + 1)); }
contine "figura ordinaria addita" "$T/out" "^figura addita    pupa$"
# omnis figura in plastica/figurae componitur et lineam motus dat
for f in plastica/figurae/*.stml; do
    [ -e "$f" ] || continue
    n="$(basename "$f" .stml)"
    proba "specimen figurae $n" 0 "$B" specimen "$f" -o "$T/f_$n.png" -motus "$T/f_${n}_motus.png"
    contine "cycli figurae $n" "$T/out" "^cyclus 0 "
done

if [ "$fracta" -eq 0 ]; then echo "PLASTICA FUMUS: OMNIA BONA"; exit 0; fi
echo "PLASTICA FUMUS: $fracta FRACTA"; exit 1
