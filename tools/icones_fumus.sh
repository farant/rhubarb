#!/bin/bash

# tools/icones_fumus.sh - FUMUS: an Finder iconem nostram PINGIT?
#
# CUR HAEC PORTA EXSISTIT
# =======================
# probatio_icones et porta iconutil continentem nostrum VALIDUM esse
# probant; neutra respondere potest an FINDER eum PINGAT. Spec icones
# par. XI AUDIENDUM - RESPONSUM 2026-09-12: PNG in ic04/ic05 Finder ut
# STREPITUM pingit (iconutil idem legit; sips recte - oraculum lene).
# Nunc ic04/ic05 ARGB praemultiplicatum ferunt, ut Apple ipsa; haec
# porta videt an Finder id ut discum pingat.
#
# CUR FASCICULI QUATTUOR
# ======================
#   Apple.app       icon Calculatoris ipsum: CONTROLLUM. Si hic quoque
#                   icon generalis apparet, ambitus (cache iconum)
#                   culpandus est, non continens noster.
#   Plenus.app      omnia latera nostra.
#   SolumXVI.app    SOLUM ic04 (16 px ARGB). In scrinio Retina Finder ad
#                   XVI puncta repraesentationem XXXII px eligit, ergo
#                   Plenus ic04 numquam ostendit; hic ic04 sola est -
#                   discus noster (ad maiora obscurus), non strepitus.
#   SolumXXXII.app  SOLUM ic11 (PNG) + ic05 (ARGB): Finder ad magna ic11
#                   eligit, ergo ic05 hic non videtur - porta I4 eam
#                   per iconutil pixelatim iudicat.
#
# Usus:
#   ./tools/icones_fumus.sh [-agere]
#     sine -agere: struit et iudicat (-legere, iconutil, plutil), viam
#                  dat - nihil in scrinio aperitur
#     -agere:      praeterea DIRECTORIUM fasciculorum in Finder aperit;
#                  applicationes NON currunt ('open <dir>')
#
# Area SEMPER servatur: homo spectare debet.
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita desunt).
# Linea ultima: 'fumus icones: sanum' | 'fumus icones: FRACTUM'.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX" || exit 2

AGERE=0
for arg in "$@"; do
    case "$arg" in
        -agere) AGERE=1 ;;
        *)
            echo "usus: $0 [-agere]" >&2
            exit 2
            ;;
    esac
done

FIXUM=probationes/fixa/icones/fons_256.png
CALCULATOR=/System/Applications/Calculator.app/Contents/Resources/AppIcon.icns
ICONES="$RADIX/bin/icones_instrumentum"
FASCICULUM="$RADIX/bin/fasciculum_instrumentum"

for instrumentum in iconutil sips plutil clang open; do
    command -v "$instrumentum" > /dev/null 2>&1 \
        || { echo "FUMUS: $instrumentum abest (macOS poscitur)" >&2; exit 2; }
done
[ -f "$FIXUM" ] || { echo "FUMUS: fixum abest: $FIXUM" >&2; exit 2; }
[ -f "$CALCULATOR" ] \
    || { echo "FUMUS: icon Apple abest: $CALCULATOR" >&2; exit 2; }
ls "$RADIX/build"/*.o > /dev/null 2>&1 \
    || { echo "FUMUS: build/*.o absunt - ./compile_tests.sh prius" >&2
         exit 2; }

AREA="$(mktemp -d /tmp/icones_fumus.XXXXXX)" || exit 2
FASCIC="$AREA/fasciculi"

deficere () {
    echo "FUMUS DEFECIT: $1" >&2
    if [ -n "${2:-}" ] && [ -f "$2" ]; then
        echo "--- ultimae XX lineae ($2) ---" >&2
        tail -20 "$2" >&2
    fi
    echo "FUMUS: area servata: $AREA"
    echo "fumus icones: FRACTUM"
    exit 1
}

echo "FUMUS: area $AREA"

# ---- 0. instrumenta ex obiectis PRAESENTIBUS ----
echo "FUMUS: 0. instrumenta reaedificantur"
./compile_tools.sh icones_instrumentum > "$AREA/instr_icones.log" 2>&1 \
    || deficere "icones_instrumentum aedificari non potuit" \
                "$AREA/instr_icones.log"
./compile_tools.sh fasciculum_instrumentum > "$AREA/instr_fasc.log" 2>&1 \
    || deficere "fasciculum_instrumentum aedificari non potuit" \
                "$AREA/instr_fasc.log"

# exsecutabile minimum: fasciculum exsecutabile EXSISTERE poscit. Numquam
# curritur - -agere directorium aperit, non applicationes.
cat > "$AREA/salve.c" <<'FINIS_C'
int main (void)
{
    return 0;
}
FINIS_C
source "$RADIX/tools/vexilla.sh"
clang "${VEXILLA_C89[@]}" "$AREA/salve.c" -o "$AREA/salve" \
    > "$AREA/salve.log" 2>&1 \
    || deficere "exsecutabile minimum compilari non potuit" \
                "$AREA/salve.log"

# ---- I. fons MXXIV et continentes tres ----
echo "FUMUS: I. fons MXXIV, continentes tres"
sips -z 1024 1024 "$FIXUM" --out "$AREA/fons_1024.png" \
    > "$AREA/sips.log" 2>&1 \
    || deficere "sips fontem facere non potuit" "$AREA/sips.log"

struere () {
    # $1 titulus, $2 latera ("" = omnia)
    mkdir -p "$AREA/$1.opus" || exit 2
    if [ -n "$2" ]; then
        "$ICONES" -fons "$AREA/fons_1024.png" -radix "$AREA/$1.opus" \
            -titulus "$1" -latera "$2" > "$AREA/$1.struere.log" 2>&1
    else
        "$ICONES" -fons "$AREA/fons_1024.png" -radix "$AREA/$1.opus" \
            -titulus "$1" > "$AREA/$1.struere.log" 2>&1
    fi || deficere "icones $1 struere non potuit" "$AREA/$1.struere.log"
    "$ICONES" -legere "$AREA/$1.opus/$1.icns" > "$AREA/$1.tabula" 2>&1 \
        || deficere "-legere $1 recusavit" "$AREA/$1.tabula"
}
struere Plenus ""
struere SolumXVI 16
struere SolumXXXII 32

# ---- II. quod quisque continens fert ----
echo "FUMUS: II. tabulae chunkorum"
[ "$(wc -l < "$AREA/Plenus.tabula" | tr -d ' ')" = 10 ] \
    || deficere "Plenus X chunkos non fert" "$AREA/Plenus.tabula"
[ "$(wc -l < "$AREA/SolumXVI.tabula" | tr -d ' ')" = 1 ] \
    && grep -q '^ic04 [0-9]* argb$' "$AREA/SolumXVI.tabula" \
    || deficere "SolumXVI non SOLUM ic04 ARGB fert" \
                "$AREA/SolumXVI.tabula"
[ "$(wc -l < "$AREA/SolumXXXII.tabula" | tr -d ' ')" = 2 ] \
    && grep -q '^ic11 [0-9]* png 32x32$' "$AREA/SolumXXXII.tabula" \
    && grep -q '^ic05 [0-9]* argb$' "$AREA/SolumXXXII.tabula" \
    || deficere "SolumXXXII non SOLUM ic11 (PNG) + ic05 (ARGB) fert" \
                "$AREA/SolumXXXII.tabula"
for titulus in SolumXVI SolumXXXII; do
    sed "s/^/FUMUS:    $titulus: /" "$AREA/$titulus.tabula"
done

# ---- III. iconutil quemque legit (NUMERUS plagularum, non rc) ----
echo "FUMUS: III. iconutil"
for par in Plenus:10 SolumXVI:1 SolumXXXII:2; do
    titulus="${par%%:*}"
    exspectatae="${par##*:}"
    iconutil -c iconset -o "$AREA/$titulus.ex.iconset" \
        "$AREA/$titulus.opus/$titulus.icns" \
        > "$AREA/$titulus.iconutil.log" 2>&1 \
        || deficere "iconutil $titulus recusavit" \
                    "$AREA/$titulus.iconutil.log"
    extractae=$(ls "$AREA/$titulus.ex.iconset" | wc -l | tr -d ' ')
    [ "$extractae" = "$exspectatae" ] \
        || deficere "iconutil ex $titulus $extractae plagulas extraxit, $exspectatae exspectatae"
done

# ---- IV. fasciculi: Apple (controllum) et tres nostri ----
echo "FUMUS: IV. fasciculi"
cp "$CALCULATOR" "$AREA/Apple.icns" || exit 2

fasciculum () {
    # $1 titulus, $2 via .icns
    "$FASCICULUM" -radix "$FASCIC/$1.app" \
        -identitas "org.rhubarb.icones.fumus.$(echo "$1" | tr 'A-Z' 'a-z')" \
        -titulus "$1" -exsecutabile "$AREA/salve" -icon "$2" \
        > "$AREA/$1.fasc.log" 2>&1 \
        || deficere "fasciculus $1 struere non potuit" "$AREA/$1.fasc.log"
    plutil -lint "$FASCIC/$1.app/Contents/Info.plist" \
        > "$AREA/$1.lint.log" 2>&1 \
        || deficere "Info.plist $1 a plutil recusata" "$AREA/$1.lint.log"
    icon=$(plutil -extract CFBundleIconFile raw -o - \
        "$FASCIC/$1.app/Contents/Info.plist" 2>/dev/null)
    [ -n "$icon" ] && [ -f "$FASCIC/$1.app/Contents/Resources/$icon" ] \
        || deficere "$1: CFBundleIconFile [$icon] in Resources abest"
}
fasciculum Apple "$AREA/Apple.icns"
fasciculum Plenus "$AREA/Plenus.opus/Plenus.icns"
fasciculum SolumXVI "$AREA/SolumXVI.opus/SolumXVI.icns"
fasciculum SolumXXXII "$AREA/SolumXXXII.opus/SolumXXXII.icns"
echo "FUMUS:    $(ls -d "$FASCIC"/*.app | wc -l | tr -d ' ') fasciculi in $FASCIC"

# ---- V. quod homo spectare debet ----
cat <<FINIS
FUMUS: FACTUM (continentes iudicati, iconutil legit, fasciculi structi).

  AUDIENDUM (spec icones par. XI) RESPONSUM: PNG in ic04 = strepitus.
  Nunc ARGB praemultiplicatum - Finder id ut discum pingere DEBET.

  Directorium: $FASCIC
  In Finder visum INDICIS (List, XVI puncta) et visum ICONUM (Icons).

    Apple.app       icon Calculatoris  -> CONTROLLUM: si hic generalis
                                          apparet, ambitus culpandus est
    Plenus.app      discus noster omnibus magnitudinibus
    SolumXVI.app    discus (ad magna obscurus, margo SINE halone albo)
                    STREPITUS aut margo ALBESCENS -> ARGB fractum
    SolumXXXII.app  discus (Finder ic11 eligit)

  Scrinium Retina ad XVI puncta repraesentationem XXXII px eligit: ideo
  Plenus.app ic04 numquam ostendit, et SolumXVI exsistit.
FINIS

if [ "$AGERE" = 1 ]; then
    echo "FUMUS: VI. open $FASCIC (SCRINIUM - applicationes NON currunt)"
    open "$FASCIC" > "$AREA/open.log" 2>&1 \
        || deficere "'open' directorium recusavit" "$AREA/open.log"
else
    echo "FUMUS: manu:  open '$FASCIC'"
    echo "FUMUS: aut:   $0 -agere"
fi
echo "FUMUS: area servata: $AREA"
echo "fumus icones: sanum"
exit 0
