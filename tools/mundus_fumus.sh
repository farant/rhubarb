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

# ostia (ostium-plan T1): cum loco scribuntur; sola mutata "mutata";
# recusatum locum totum recusat, nihil scriptum
contine "pratum sine ostiis" "$T/out" "^mutata    pratum   2 res, .*, ostia 0$"
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
  <res fons="plastica/res/puteus.stml" locus="24 31"/>
  <ostium locus="27 29" scaena="hut-door"/>
</locus>
XX
proba "condere cum ostio" 0 "$B" condere -volumen "$V" -loci "$D"
contine "ostium solum additum: mutata" "$T/out" "^mutata    pratum   2 res, .*, ostia 1$"
proba "condere cum ostio iterum" 0 "$B" condere -volumen "$V" -loci "$D"
contine "ostium idem: eadem" "$T/out" "^eadem     pratum   2 res, .*, ostia 1$"
# campus unus ostii mutatus (radius; deinde locus, deinde scaena):
# "mutata", et iterum "eadem"
for O in 'locus="27 29" radius="2" scaena="hut-door"' 'locus="27 30" radius="2" scaena="hut-door"' 'locus="27 30" radius="2" scaena="hut-gate"'; do
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
  <res fons="plastica/res/puteus.stml" locus="24 31"/>
  <ostium $O/>
</locus>
XX
proba "condere ostium mutatum ($O)" 0 "$B" condere -volumen "$V" -loci "$D"
contine "ostium mutatum ($O): mutata" "$T/out" "^mutata    pratum   2 res, .*, ostia 1$"
proba "condere ostium mutatum iterum ($O)" 0 "$B" condere -volumen "$V" -loci "$D"
contine "ostium mutatum iterum ($O): eadem" "$T/out" "^eadem     pratum   2 res, .*, ostia 1$"
done
SIG_OSTII="$("$S" enumerare -volumen "$V" -loci | cut -f2)"
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
  <ostium locus="27 29" scaena="hut-door"/>

  <ostium locus="-500 -500" scaena="hut-door"/>
</locus>
XX
proba "ostium extra locum recusatur" 1 "$B" condere -volumen "$V" -loci "$D"
contine "ostium recusatum cum linea" "$T/err" "a_pratum.stml:5: .*outside"
[ "$("$S" enumerare -volumen "$V" -loci | cut -f2)" = "$SIG_OSTII" ] || { echo "FRACTA: ostium recusatum locum mutavit"; fracta=$((fracta + 1)); }
# ostium contra locum INSTRUCTUM probatur: in media turri solida
# nusquam intra radium stare licet (in loco nudo liceret)
printf '<res titulus="turris">\n  <capsa magnitudo="3 5 3" signum="solidum"/>\n</res>\n' > "$T/turris.stml"
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="$T/turris.stml" locus="27 29"/>
  <ostium locus="27 29" radius="0.5" scaena="hut-door"/>
</locus>
XX
proba "ostium in turri recusatur" 1 "$B" condere -volumen "$V" -loci "$D"
contine "ostium in turri cum linea" "$T/err" "a_pratum.stml:3: .*stand"
[ "$("$S" enumerare -volumen "$V" -loci | cut -f2)" = "$SIG_OSTII" ] || { echo "FRACTA: ostium in turri locum mutavit"; fracta=$((fracta + 1)); }
cat > "$D/a_pratum.stml" <<XX
<locus titulus="pratum" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29" gyrus="20"/>
  <res fons="plastica/res/puteus.stml" locus="24 31"/>
</locus>
XX
proba "condere sine ostio rursus" 0 "$B" condere -volumen "$V" -loci "$D"
contine "ostium sublatum: mutata" "$T/out" "^mutata    pratum   2 res, .*, ostia 0$"
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
contine "nudus tamen additus" "$T/out" "^addita    nudus   0 res, [0-9]* vertices, cellae gressibiles 60626 -> 60626, ostia 0$"
contine "summa cum recusatis" "$T/out" "^mundus: 1 additae, 0 mutatae, 0 eaedem, 7 recusatae$"
[ "$(loci)" = "pratum,nudus," ] || { echo "FRACTA: loci post recusationes: $(loci)"; fracta=$((fracta + 1)); }
[ "$("$S" enumerare -volumen "$V" -loci | head -1 | cut -f2)" = "$SIG" ] || { echo "FRACTA: dispositio recusata locum mutavit"; fracta=$((fracta + 1)); }

# magnitudo, semen et sparsio (plastica II T2)
E="$T/loci_varia"
mkdir -p "$E"
cat > "$E/a.stml" <<XX
<locus titulus="varia" basis="$F/planum.glb">
  <res fons="plastica/res/saxum_parvum.stml" locus="27 29"/>
  <res fons="plastica/res/saxum_parvum.stml" locus="29 31" magnitudo="2"/>
</locus>
XX
proba "condere scalam" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "duae res eiusdem fontis" "$T/out" "^addita    varia   2 res, "
VERT1="$(sed -n 's/^addita    varia   2 res, \([0-9]*\) vertices.*/\1/p' "$T/out")"
CELL1="$(sed -n 's/.*cellae gressibiles 60626 -> \([0-9]*\), ostia 0$/\1/p' "$T/out")"
# semen aliud: statua alia, vertices plures
cat > "$E/a.stml" <<XX
<locus titulus="varia" basis="$F/planum.glb">
  <res fons="plastica/res/saxum_parvum.stml" locus="27 29"/>
  <res fons="plastica/res/saxum_parvum.stml" locus="29 31" magnitudo="2" semen="4"/>
</locus>
XX
proba "condere semen" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "semen locum mutat" "$T/out" "^mutata    varia   2 res, "
VERT2="$(sed -n 's/^mutata    varia   2 res, \([0-9]*\) vertices.*/\1/p' "$T/out")"
[ "${VERT2:-0}" -gt "${VERT1:-999999999}" ] || { echo "FRACTA: semen aliud statuam aliam non dedit ($VERT1 -> $VERT2)"; fracta=$((fracta + 1)); }
# semen V contra IV: saxum aliud, octeti alii
sed -i '' 's/semen="4"/semen="5"/' "$E/a.stml"
proba "condere semen aliud" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "semen aliud rem aliam dat" "$T/out" "^mutata    varia   2 res, "
# scala II plures cellas claudit quam scala I
cat > "$E/a.stml" <<XX
<locus titulus="varia" basis="$F/planum.glb">
  <res fons="plastica/res/saxum_parvum.stml" locus="27 29"/>
  <res fons="plastica/res/saxum_parvum.stml" locus="29 31"/>
</locus>
XX
proba "condere sine scala" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
CELL0="$(sed -n 's/.*cellae gressibiles 60626 -> \([0-9]*\), ostia 0$/\1/p' "$T/out")"
[ "${CELL1:-999999}" -lt "${CELL0:-0}" ] || { echo "FRACTA: scala II non plures cellas clausit ($CELL0 / $CELL1)"; fracta=$((fracta + 1)); }
# sparsio: XII frutices, tres varietates; determinata
cat > "$E/a.stml" <<XX
<locus titulus="varia" basis="$F/planum.glb">
  <res fons="plastica/res/puteus.stml" locus="27 29"/>
  <sparsio fons="plastica/res/frutex.stml" numerus="12" semen="5"
           area="14 28 44 36" distantia="2" magnitudo_varia="0.2"
           gyrus_varius="verum" varietates="3"/>
</locus>
XX
proba "condere sparsionem" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "sparsio XIII res dat" "$T/out" "^mutata    varia   13 res, "
proba "condere sparsionem iterum" 0 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "sparsio determinata" "$T/out" "^eadem     varia   13 res, "
proba "mappa sparsionis" 0 "$B" mappa "$E/a.stml" -o "$T/sp.png"
[ "$(grep -c 'plastica/res/frutex.stml  (.*)  linea 3$' "$T/out")" -eq 12 ] || { echo "FRACTA: legenda sparsionis: $(grep -c 'frutex' "$T/out") lineae"; fracta=$((fracta + 1)); }
contine "res scripta prima" "$T/out" "^ *1  plastica/res/puteus.stml  (27.00, 29.00)  linea 2$"
# sparsio quae non capit: recusatur cum linea et numero positarum
cat > "$E/a.stml" <<XX
<locus titulus="varia" basis="$F/planum.glb">
  <sparsio fons="plastica/res/frutex.stml" numerus="50"
           area="26 28 29 31" distantia="2"/>
</locus>
XX
SIGV="$("$S" enumerare -volumen "$T/varia.volumen" -loci | cut -f2)"
proba "sparsio nimia recusatur" 1 "$B" condere -volumen "$T/varia.volumen" -loci "$E"
contine "sparsio nimia cum linea" "$T/err" "^$E/a.stml:2: only [0-9]* of 50 copies could be placed"
[ "$("$S" enumerare -volumen "$T/varia.volumen" -loci | cut -f2)" = "$SIGV" ] || { echo "FRACTA: sparsio recusata locum mutavit"; fracta=$((fracta + 1)); }

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

# pixelum <png> <x> <y>: "r g b" pixeli (lector PNG parvus: VIII bit,
# RGB aut RGBA, sine intertextura); alba <png> <x0> <y0> <x1> <y1>:
# numerus pixelorum NON alborum in rectangulo
cat > "$T/pixelum.py" <<'XX'
import sys, zlib, struct
def lege(via):
    d = open(via, 'rb').read()
    assert d[:8] == b'\x89PNG\r\n\x1a\n'
    p = 8; idat = b''; w = h = n = 0
    while p < len(d):
        m, t = struct.unpack('>I4s', d[p:p+8]); c = d[p+8:p+8+m]; p += 12 + m
        if t == b'IHDR':
            w, h, bits, genus, _, _, inter = struct.unpack('>IIBBBBB', c)
            assert bits == 8 and inter == 0 and genus in (2, 6)
            n = 4 if genus == 6 else 3
        elif t == b'IDAT':
            idat += c
    r = zlib.decompress(idat); s = w * n; prior = bytearray(s); lineae = []
    for y in range(h):
        f = r[y * (s + 1)]; l = bytearray(r[y * (s + 1) + 1:(y + 1) * (s + 1)])
        for i in range(s):
            a = l[i - n] if i >= n else 0; b = prior[i]; c = prior[i - n] if i >= n else 0
            if f == 1: l[i] = (l[i] + a) & 255
            elif f == 2: l[i] = (l[i] + b) & 255
            elif f == 3: l[i] = (l[i] + (a + b) // 2) & 255
            elif f == 4:
                q = a + b - c; pa = abs(q - a); pb = abs(q - b); pc = abs(q - c)
                l[i] = (l[i] + (a if pa <= pb and pa <= pc else (b if pb <= pc else c))) & 255
        lineae.append(l); prior = l
    return w, h, n, lineae
w, h, n, L = lege(sys.argv[2])
if sys.argv[1] == 'pixelum':
    x, y = int(sys.argv[3]), int(sys.argv[4])
    print(*L[y][x * n:x * n + 3])
else:
    x0, y0, x1, y1 = map(int, sys.argv[3:7])
    print(sum(1 for y in range(y0, y1) for x in range(x0, x1) if tuple(L[y][x * n:x * n + 3]) != (255, 255, 255)))
XX
pixelum() { python3 -I "$T/pixelum.py" pixelum "$@"; }
non_alba() { python3 -I "$T/pixelum.py" alba "$@"; }

# mappa (plastica II T1): tabula gressus dispositionis in imagine
L=apps/mundus/loci/clearing.stml
proba "mappa sine -o usus" 2 "$B" mappa "$L"
proba "mappa sine dispositione usus" 2 "$B" mappa -o "$T/m.png"
proba "mappa cum -locus usus" 2 "$B" mappa "$L" -o "$T/m.png" -locus "1 2"
proba "mappa cum -volumen usus" 2 "$B" mappa "$L" -o "$T/m.png" -volumen "$V"
[ ! -e "$T/m.png" ] || { echo "FRACTA: usus mappam scripsit"; fracta=$((fracta + 1)); }
proba "mappa" 0 "$B" mappa "$L" -o "$T/m.png"
file "$T/m.png" 2>/dev/null | grep -q "1040 x 1040" || { echo "FRACTA: mappa: PNG 1040 x 1040 abest"; fracta=$((fracta + 1)); }
contine "mappa numeri" "$T/out" "^$L: [0-9]* res, cellae gressibiles [0-9]* (nudae 60626)$"
contine "mappa attingibiles" "$T/out" "^attingibiles ab initio (31.88, 31.88): [0-9]* (imae [0-9]*, mediae [0-9]*, summae [0-9]*)$"
SUMMAE="$(sed -n 's/^attingibiles.*summae \([0-9]*\))$/\1/p' "$T/out")"
[ "${SUMMAE:-0}" -gt 1000 ] || { echo "FRACTA: mappa: stratum summum non attingitur (${SUMMAE:-nihil})"; fracta=$((fracta + 1)); }
LINEA="$(grep -n 'puteus.stml' "$L" | head -1 | cut -d: -f1)"
contine "mappa legenda cum linea" "$T/out" "^ *1  plastica/res/puteus.stml  (27.00, 29.00)  linea $LINEA$"
# pixela: IV per cellam, margo XVI; (x, z) mundi -> 16 + 16 * x
# initium (31.9, 31.9) attactum: viride; angulus (62, 2) gressibilis sed
# non attactus: roseum (rubrum vincit); puteus (27, 29): punctum notae
read -r PR PG PB <<XX
$(pixelum "$T/m.png" 526 526)
XX
[ "$PG" -gt "$PR" ] || { echo "FRACTA: mappa: initium non viride ($PR $PG $PB)"; fracta=$((fracta + 1)); }
read -r PR PG PB <<XX
$(pixelum "$T/m.png" 1010 50)
XX
[ "$PR" -gt "$PG" ] || { echo "FRACTA: mappa: cellae non attactae non notatae ($PR $PG $PB)"; fracta=$((fracta + 1)); }
[ "$(pixelum "$T/m.png" 450 480)" = "0 0 0" ] || { echo "FRACTA: mappa: nota putei abest ($(pixelum "$T/m.png" 450 480))"; fracta=$((fracta + 1)); }
cp "$T/m.png" "$T/m_prima.png"
proba "mappa iterum" 0 "$B" mappa "$L" -o "$T/m.png"
cmp -s "$T/m.png" "$T/m_prima.png" || { echo "FRACTA: mappa non determinata"; fracta=$((fracta + 1)); }
proba "mappa nuda" 0 "$B" mappa "$L" -o "$T/n.png" -nudus
contine "nuda numeri" "$T/out" "^$L: locus nudus, cellae gressibiles 60626$"
contine "nuda attingibiles" "$T/out" "^attingibiles ab initio (31.88, 31.88): 42881 (imae 23767, mediae 19114, summae 0)$"
grep -q "puteus" "$T/out" && { echo "FRACTA: mappa nuda legendam habet"; fracta=$((fracta + 1)); }
cmp -s "$T/n.png" "$T/m.png" && { echo "FRACTA: mappa nuda eadem ac instructa"; fracta=$((fracta + 1)); }
proba "mappa dispositionis malae" 1 "$B" mappa "$D/a_pratum.stml" -o "$T/x.png"
contine "mappa causam dicit" "$T/err" "^$D/a_pratum.stml:3: the spot lies outside the level$"
[ ! -e "$T/x.png" ] || { echo "FRACTA: mappa recusata PNG scripsit"; fracta=$((fracta + 1)); }

# quadrum: quadra loci instructi cum persona in punctis datis
proba "quadrum sine -locus usus" 2 "$B" quadrum "$L" -o "$T/q.png"
proba "quadrum sine -o usus" 2 "$B" quadrum "$L" -locus "29 31"
proba "quadrum locus malus usus" 2 "$B" quadrum "$L" -o "$T/q.png" -locus "duo tres"
proba "quadrum cum -nudus usus" 2 "$B" quadrum "$L" -o "$T/q.png" -locus "29 31" -nudus
[ ! -e "$T/q.png" ] || { echo "FRACTA: usus quadrum scripsit"; fracta=$((fracta + 1)); }
proba "quadrum unum" 0 "$B" quadrum "$L" -o "$T/q1.png" -locus "29 31"
file "$T/q1.png" 2>/dev/null | grep -q "480 x 360" || { echo "FRACTA: quadrum unum: PNG 480 x 360 abest"; fracta=$((fracta + 1)); }
contine "quadrum locum dicit" "$T/out" "^quadrum 1: (29.00, 31.00) altitudo 4\.[0-9]*, persona [0-9]* pixela$"
P="$(sed -n 's/^quadrum 1: .*persona \([0-9]*\) pixela$/\1/p' "$T/out")"
[ "${P:-0}" -gt 500 ] || { echo "FRACTA: quadrum: persona non picta (${P:-nulla})"; fracta=$((fracta + 1)); }
proba "quadrum alibi" 0 "$B" quadrum "$L" -o "$T/q2.png" -locus "17.5 17.5"
cmp -s "$T/q1.png" "$T/q2.png" && { echo "FRACTA: quadra locorum diversorum eadem"; fracta=$((fracta + 1)); }
proba "quadra quattuor" 0 "$B" quadrum "$L" -o "$T/q4.png" -locus "29 31" -locus "17.5 17.5" -locus "-500 -500" -locus "36.5 47.5"
file "$T/q4.png" 2>/dev/null | grep -q "1440 x 720" || { echo "FRACTA: quadra quattuor: PNG 1440 x 720 abest"; fracta=$((fracta + 1)); }
contine "quadrum quartum" "$T/out" "^quadrum 4: (36.50, 47.50) altitudo "
contine "quadrum ubi stare non licet" "$T/err" "^quadrum 3: (-500.00, -500.00): persona ibi stare non potest$"
# tabellae suo quaeque loco: I et II pictae, III vacua, IV in ordine altero
[ "$(non_alba "$T/q4.png" 0 0 480 360)" -gt 100000 ] || { echo "FRACTA: quadrum: tabella I vacua"; fracta=$((fracta + 1)); }
[ "$(non_alba "$T/q4.png" 480 0 960 360)" -gt 100000 ] || { echo "FRACTA: quadrum: tabella II vacua"; fracta=$((fracta + 1)); }
[ "$(non_alba "$T/q4.png" 960 0 1440 360)" -eq 0 ] || { echo "FRACTA: quadrum: tabella III non vacua"; fracta=$((fracta + 1)); }
[ "$(non_alba "$T/q4.png" 0 360 480 720)" -gt 100000 ] || { echo "FRACTA: quadrum: tabella IV vacua"; fracta=$((fracta + 1)); }
[ "$(non_alba "$T/q4.png" 480 360 1440 720)" -eq 0 ] || { echo "FRACTA: quadrum: tabellae V et VI non vacuae"; fracta=$((fracta + 1)); }
cp "$T/q4.png" "$T/q4_prima.png"
proba "quadra iterum" 0 "$B" quadrum "$L" -o "$T/q4.png" -locus "29 31" -locus "17.5 17.5" -locus "-500 -500" -locus "36.5 47.5"
cmp -s "$T/q4.png" "$T/q4_prima.png" || { echo "FRACTA: quadra non determinata"; fracta=$((fracta + 1)); }
proba "quadrum nusquam" 1 "$B" quadrum "$L" -o "$T/q0.png" -locus "-500 -500"
[ ! -e "$T/q0.png" ] || { echo "FRACTA: quadrum sine persona PNG scripsit"; fracta=$((fracta + 1)); }
proba "quadrum dispositionis malae" 1 "$B" quadrum "$D/a_pratum.stml" -o "$T/x.png" -locus "29 31"

if [ "$fracta" -eq 0 ]; then echo "MUNDUS FUMUS: OMNIA BONA"; exit 0; fi
echo "MUNDUS FUMUS: $fracta FRACTA"; exit 1
