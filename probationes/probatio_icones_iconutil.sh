#!/bin/bash
# probatio_icones_iconutil.sh - ORACULUM ALIENUM: iconutil (instrumentum
# Apple) continentem .icns NOSTRUM et directorium .iconset NOSTRUM
# iudicat, UTRAQUE directione. Exemplar: probatio_plist_plutil.sh.
#
# CONTROLLUM PRIMUM: icon APPLE (Calculator) per iconutil convertitur
# ANTE nostrum, ut oraculum ipsum probetur antequam de nostro iudicet.
#
# QUID ORACULUM IGNOSCIT (mensuratum 2026-09-12, lib/icones.worklog.md):
# iconutil rc=0 reddit pro longitudine declarata uno octeto BREVIORE,
# signo PNG corrupto, codice ignoto (chunkus tacite omissus) et chunko
# SINE capite (plagula UNA extracta, successus nuntiatus); solum
# longitudinem declaratam MAIOREM recusat. ERGO haec porta NUMERUM
# plagularum et DIMENSIONES cuiusque asserit, numquam rc solum.
#
# Instrumentum bin/icones_instrumentum SEMPER hic reaedificatur:
# compile_tool_if_needed binarium cum FONTE instrumenti solo confert,
# non cum obiectis bibliothecae, ergo instrumentum vetus bibliothecam
# heri probaret dum hodie viridis nuntiatur.
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita desunt -
#         numquam cum silentio viridi confundendum).

set -u
RADIX="$(cd "$(dirname "$0")/.." && pwd)"
cd "$RADIX" || exit 2

FIXUM=probationes/fixa/icones/fons_256.png
CALCULATOR=/System/Applications/Calculator.app/Contents/Resources/AppIcon.icns
INSTRUMENTUM=bin/icones_instrumentum

[ -x /usr/bin/iconutil ] || { echo "NIHIL CURSUM: iconutil abest" >&2; exit 2; }
[ -x /usr/bin/sips ] || { echo "NIHIL CURSUM: sips abest" >&2; exit 2; }
command -v perl > /dev/null 2>&1 \
    || { echo "NIHIL CURSUM: perl abest" >&2; exit 2; }
[ -f "$FIXUM" ] || { echo "NIHIL CURSUM: fixum abest: $FIXUM" >&2; exit 2; }
[ -f "$CALCULATOR" ] || {
    echo "NIHIL CURSUM: icon Apple ad controllum abest: $CALCULATOR" >&2
    exit 2; }
ls build/*.o > /dev/null 2>&1 || {
    echo "NIHIL CURSUM: build/*.o absunt - ./compile_tests.sh prius" >&2
    exit 2; }

AREA="$(mktemp -d /tmp/probatio_icones_iconutil.XXXXXX)" || exit 2
purgare () { rm -rf "$AREA"; }
deficere () {
    echo "PORTA DEFECIT: $1" >&2
    [ -n "${2:-}" ] && [ -f "$2" ] && tail -20 "$2" >&2
    purgare
    exit 1
}

# octetos IV magni-endianos ad sedem mutare: perl (suita iam eo utitur)
longitudinem_mutare () {
    perl -e 'open(F, "+<", $ARGV[0]) or die; binmode F;
             seek(F, $ARGV[1], 0); read(F, $b, 4);
             seek(F, $ARGV[1], 0);
             print F pack("N", unpack("N", $b) + $ARGV[2]); close F' \
        "$1" "$2" "$3"
}

# ---- 0. instrumentum ex obiectis PRAESENTIBUS ----
./compile_tools.sh icones_instrumentum > "$AREA/instrumentum.log" 2>&1 \
    || deficere "instrumentum aedificari non potuit" \
                "$AREA/instrumentum.log"

# ---- I. CONTROLLUM: iconutil icon APPLE convertit ----
iconutil -c iconset -o "$AREA/controllum.iconset" "$CALCULATOR" \
    > "$AREA/controllum.log" 2>&1 \
    || deficere "CONTROLLUM FRACTUM: iconutil icon Apple recusavit" \
                "$AREA/controllum.log"
[ "$(ls "$AREA/controllum.iconset" | wc -l | tr -d ' ')" -ge 1 ] \
    || deficere "CONTROLLUM FRACTUM: iconutil nihil ex icone Apple extraxit"

# ---- II. fons MXXIV (sips ex fixo) -> .iconset et .icns NOSTRA ----
sips -z 1024 1024 "$FIXUM" --out "$AREA/fons_1024.png" \
    > "$AREA/sips.log" 2>&1 \
    || deficere "sips fontem MXXIV facere non potuit" "$AREA/sips.log"
"$INSTRUMENTUM" -fons "$AREA/fons_1024.png" -radix "$AREA" \
    > "$AREA/struere.log" 2>&1 \
    || deficere "instrumentum icones struere non potuit" "$AREA/struere.log"
N=$(ls "$AREA/AppIcon.iconset" | wc -l | tr -d ' ')
[ "$N" = 10 ] \
    || deficere ".iconset nostrum $N plagulas fert, X exspectatae" \
                "$AREA/struere.log"

# ---- III. DIRECTIO A: .icns NOSTRUM -> iconutil -> .iconset ----
iconutil -c iconset -o "$AREA/ex_icns.iconset" "$AREA/AppIcon.icns" \
    > "$AREA/a.log" 2>&1 \
    || deficere "iconutil .icns NOSTRUM recusavit" "$AREA/a.log"
# NUMERUS et DIMENSIONES, non rc: iconutil rc=0 cum plagula una reddit
N=$(ls "$AREA/ex_icns.iconset" | wc -l | tr -d ' ')
[ "$N" = 10 ] \
    || deficere "iconutil ex .icns nostro $N plagulas extraxit, X exspectatae (rc=0 non sufficit)"
for par in 16x16:16 16x16@2x:32 32x32:32 32x32@2x:64 128x128:128 \
           128x128@2x:256 256x256:256 256x256@2x:512 512x512:512 \
           512x512@2x:1024; do
    nomen_plagulae="icon_${par%%:*}.png"
    px="${par##*:}"
    f="$AREA/ex_icns.iconset/$nomen_plagulae"
    [ -f "$f" ] || deficere "iconutil $nomen_plagulae non extraxit"
    lat=$(sips -g pixelWidth "$f" | awk '/pixelWidth/ {print $2}')
    alt=$(sips -g pixelHeight "$f" | awk '/pixelHeight/ {print $2}')
    if [ "$lat" != "$px" ] || [ "$alt" != "$px" ]; then
        deficere "$nomen_plagulae ex .icns nostro: ${lat}x${alt}, ${px}x${px} exspectatum"
    fi
done

# ---- IV. DIRECTIO B: .iconset NOSTRUM -> iconutil -> .icns APPLE ----
iconutil -c icns -o "$AREA/ex_set.icns" "$AREA/AppIcon.iconset" \
    > "$AREA/b.log" 2>&1 \
    || deficere "iconutil .iconset NOSTRUM recusavit" "$AREA/b.log"
"$INSTRUMENTUM" -legere "$AREA/AppIcon.icns" > "$AREA/tabula_nostra" 2>&1 \
    || deficere "-legere .icns NOSTRUM recusavit" "$AREA/tabula_nostra"
"$INSTRUMENTUM" -legere "$AREA/ex_set.icns" > "$AREA/tabula_apple" 2>&1 \
    || deficere "-legere .icns APPLE recusavit" "$AREA/tabula_apple"

# CODICES: setum idem (Apple 'info' addit; ordo Apple alius est)
awk '{print $1}' "$AREA/tabula_nostra" | sort > "$AREA/codices_nostri"
awk '$1 != "info" {print $1}' "$AREA/tabula_apple" | sort \
    > "$AREA/codices_apple"
diff -u "$AREA/codices_nostri" "$AREA/codices_apple" \
    > "$AREA/codices.diff" 2>&1 \
    || deficere "codices .icns DISCREPANT (nostri contra Apple)" \
                "$AREA/codices.diff"

# DIMENSIONES ubi uterque PNG fert: Apple ic04/ic05 ARGB scribit
awk '$3 == "png" {print $1, $4}' "$AREA/tabula_nostra" | sort \
    > "$AREA/dim_nostrae"
awk '$3 == "png" {print $1, $4}' "$AREA/tabula_apple" | sort \
    > "$AREA/dim_apple"
join "$AREA/dim_nostrae" "$AREA/dim_apple" > "$AREA/dim_iuncta"
awk '$2 != $3' "$AREA/dim_iuncta" > "$AREA/dim.diff"
if [ -s "$AREA/dim.diff" ]; then
    deficere "dimensiones chunkorum DISCREPANT (codex nostra apple)" \
             "$AREA/dim.diff"
fi
# collatio NON fere vacua: aliter 'nulla discrepantia' nihil probat
COMMUNES=$(wc -l < "$AREA/dim_iuncta" | tr -d ' ')
[ "$COMMUNES" -ge 8 ] \
    || deficere "solum $COMMUNES chunki PNG communes - collatio fere vacua" \
                "$AREA/dim_iuncta"
grep -q '^ic04 [0-9]* argb' "$AREA/tabula_apple" \
    || echo "NOTA: Apple ic04 non iam ARGB - D9 recognoscendum" >&2

# ---- V. ADVERSA a: oraculum 'NON' dicere potest ----
cp "$AREA/AppIcon.icns" "$AREA/longior.icns"
longitudinem_mutare "$AREA/longior.icns" 4 1 \
    || deficere "adversum a fingi non potuit"
if iconutil -c iconset -o "$AREA/longior.iconset" "$AREA/longior.icns" \
        > "$AREA/va.log" 2>&1; then
    deficere "iconutil longitudinem declaratam MAIOREM accepit - oraculum 'non' dicere non potest" \
             "$AREA/va.log"
fi
if "$INSTRUMENTUM" -legere "$AREA/longior.icns" > "$AREA/va_nostra.log" 2>&1
then
    deficere "-legere nostrum longitudinem falsam accepit" \
             "$AREA/va_nostra.log"
fi
grep -q 'RECUSATUM LONGITUDO' "$AREA/va_nostra.log" \
    || deficere "-legere recusationem LONGITUDO non nominat" \
                "$AREA/va_nostra.log"

# ---- VI. ADVERSA b: chunkus SINE capite - rc iconutil non sufficit ----
cp "$AREA/AppIcon.icns" "$AREA/sine_capite.icns"
longitudinem_mutare "$AREA/sine_capite.icns" 12 -8 \
    || deficere "adversum b fingi non potuit"
if iconutil -c iconset -o "$AREA/sine_capite.iconset" \
        "$AREA/sine_capite.icns" > "$AREA/vb.log" 2>&1; then
    N=$(ls "$AREA/sine_capite.iconset" 2>/dev/null | wc -l | tr -d ' ')
    [ "$N" = 10 ] \
        && deficere "chunkus sine capite X plagulas dedit - adversum non operatur"
    echo "NOTA: iconutil rc=0 cum $N plagula(is) ex continente fracto - numerus id capit, rc non"
else
    echo "NOTA: iconutil chunkum sine capite nunc RECUSAT - lenitas mensurata mutata est"
fi
if "$INSTRUMENTUM" -legere "$AREA/sine_capite.icns" \
        > "$AREA/vb_nostra.log" 2>&1; then
    deficere "-legere nostrum chunkum sine capite accepit" \
             "$AREA/vb_nostra.log"
fi
grep -q 'RECUSATUM CHUNKUS' "$AREA/vb_nostra.log" \
    || deficere "-legere recusationem CHUNKUS non nominat" \
                "$AREA/vb_nostra.log"

echo "icones iconutil: controllum; .icns nostrum X plagulis, .iconset nostrum $COMMUNES chunkis PNG communibus; bis adversum"
purgare
exit 0
