#!/bin/bash
# tools/android/apk.sh - APK applicationis testae Android (testa-spec
# II.7): clausura aedilis (--varians android,posix: testa_android.c per
# testa.h) -> NDK clang -> lib<X>.so -> aapt2 + zip + zipalign +
# apksigner -> build/android/arm64-v8a/apk/<X>/<X>.apk
#
# Usus: ./tools/android/apk.sh <apps/X/X.c> [-instituere] [-currere]
#                              [-fumus] [-servare]
#   -instituere  adb install -r     -currere  am start (implicat -instituere)
#   -fumus       testa-spec II.8 (T4)  -servare  post fumum non deinstallat
#
# Clavis: ~/.rhubarb/android/debug.jks (semel per keytool; JKS quia
# Java 8 systematis PKCS12 novum legere nequit - spica). SOLUM probatio:
# signum 'android' notum, numquam emissio (B6).
#
# Exitus: 0 | 1 constructio fracta (compilatio, nexus, .m) | 2 apparatus
set -u
export LC_ALL=C
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

apparatus () { echo "android/apk.sh: $*" >&2; exit 2; }
fractum () { echo "android/apk.sh: FRACTUM: $*" >&2; exit 1; }

INSTITUERE=0; CURRERE=0; FUMUS=0; SERVARE=0; FONS=""
while [ $# -gt 0 ]; do
    case "$1" in
        -instituere) INSTITUERE=1 ;;
        -currere) CURRERE=1; INSTITUERE=1 ;;
        -fumus) FUMUS=1; INSTITUERE=1 ;;
        -servare) SERVARE=1 ;;
        -*) apparatus "optio ignota: $1" ;;
        *) [ -z "$FONS" ] || apparatus "fons unus: $FONS et $1"; FONS="$1" ;;
    esac
    shift
done
[ -n "$FONS" ] || apparatus "usus: $0 <apps/X/X.c> [-instituere] [-currere] [-fumus] [-servare]"
[ -f "$FONS" ] || apparatus "$FONS deest"
TITULUS=$(basename "$FONS" .c)
case "$TITULUS" in
    ""|*[!a-z]*) apparatus "titulus '$TITULUS': litterae minusculae solae (involucrum earth.golden.$TITULUS)" ;;
esac
INVOLUCRUM="earth.golden.$TITULUS"

# --- instrumenta -------------------------------------------------------
. tools/android/ndk_fixum.sh
grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null \
    || apparatus "NDK absens: $NDK_SEDES ($NDK_VERSIO) - ./tools/android/ndk_instituere.sh"
NDK_BIN="$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin"
for instr in aapt2 zipalign apksigner; do
    [ -x "$INSTRUMENTA_SEDES/$instr" ] || apparatus "instrumentum absens: $INSTRUMENTA_SEDES/$instr"
done
[ -f "$PLATFORMA_JAR" ] || apparatus "platforma absens: $PLATFORMA_JAR"
command -v keytool > /dev/null || apparatus "keytool deest (Java)"
command -v zip > /dev/null || apparatus "zip deest"
[ -x bin/aedilis ] || apparatus "bin/aedilis deest - ./tools/aedilis_struere.sh"

. tools/vexilla.sh
vexilla_radices ""
CC=("$NDK_BIN/clang" "--target=aarch64-linux-android$NDK_API_MINIMA")
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}" -fPIC "-DTESTA_TITULUS=\"$TITULUS\"")
VFLAGS=(-O2 -fPIC -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- constructio -------------------------------------------------------
D="build/android/arm64-v8a/apk/$TITULUS"
rm -rf "$D" && mkdir -p "$D/obiecta" "$D/radix/lib/arm64-v8a" || apparatus "$D creari nequit"
PARTES=$(bin/aedilis "$FONS" --varians android,posix --partes 2>&1) \
    || apparatus "aedilis fracta: $PARTES"
OBIECTA=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "O" {print $2}')
VENDITI=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "V" {print $2}')
if printf '%s\n' "$OBIECTA" | grep -q '\.m$'; then
    fractum "fons .m in clausura (codex Mac solum): $(printf '%s\n' "$OBIECTA" | grep '\.m$' | tr '\n' ' ')"
fi
OBJ=()
# OBIECTA: viae sine spatiis (aedilis), ergo verba nuda
for f in "$FONS" $OBIECTA; do
    o="$D/obiecta/$(printf '%s' "$f" | tr '/' '_').o"
    "${CC[@]}" "${VEXILLA[@]}" -c "$f" -o "$o" >> "$D/compilatio.log" 2>&1 \
        || { tail -15 "$D/compilatio.log" >&2; fractum "compilatio $f"; }
    OBJ+=("$o")
done
for v in $VENDITI; do
    o="$D/obiecta/v_$(basename "$v" .c).o"
    "${CC[@]}" "${VFLAGS[@]}" -c "$v" -o "$o" >> "$D/compilatio.log" 2>&1 \
        || { tail -15 "$D/compilatio.log" >&2; fractum "venditum $v"; }
    OBJ+=("$o")
done
# paginae XVI KiB (Android XV+: C30 Pro 2026-10-23 pagesize.max 16384):
# segmenta ad XVI KiB alignata in utrisque machinis onerantur
SO="$D/radix/lib/arm64-v8a/lib$TITULUS.so"
"${CC[@]}" -shared -o "$SO" "${OBJ[@]}" -landroid -llog -lm \
    -Wl,--no-undefined "-Wl,-soname,lib$TITULUS.so" \
    -Wl,-z,max-page-size=16384 > "$D/nexus.log" 2>&1 \
    || { tail -15 "$D/nexus.log" >&2; fractum "nexus (symbolum absens = pars platformae deest)"; }
"$NDK_BIN/llvm-strip" "$SO" || apparatus "llvm-strip fractum"

sed "s/@TITULUS@/$TITULUS/g" tools/android/AndroidManifest.forma.xml > "$D/AndroidManifest.xml"
"$INSTRUMENTA_SEDES/aapt2" link -o "$D/basis.apk" --manifest "$D/AndroidManifest.xml" \
    -I "$PLATFORMA_JAR" > "$D/aapt2.log" 2>&1 \
    || { cat "$D/aapt2.log" >&2; fractum "aapt2 link"; }
(cd "$D/radix" && zip -q -r ../basis.apk lib) || fractum "zip"
"$INSTRUMENTA_SEDES/zipalign" -p -f 4 "$D/basis.apk" "$D/alignatum.apk" || fractum "zipalign"
CLAVIS="$HOME/.rhubarb/android/debug.jks"
if [ ! -f "$CLAVIS" ]; then
    mkdir -p "$(dirname "$CLAVIS")" || apparatus "$(dirname "$CLAVIS") creari nequit"
    keytool -genkeypair -keystore "$CLAVIS" -storetype JKS -storepass android \
        -keypass android -alias rhubarb -keyalg RSA -keysize 2048 -validity 10000 \
        -dname "CN=rhubarb debug" > /dev/null 2>&1 || apparatus "keytool fractum"
    chmod 600 "$CLAVIS"
fi
"$INSTRUMENTA_SEDES/apksigner" sign --ks "$CLAVIS" --ks-pass pass:android \
    --key-pass pass:android --ks-key-alias rhubarb --out "$D/$TITULUS.apk" \
    "$D/alignatum.apk" > "$D/apksigner.log" 2>&1 \
    || { cat "$D/apksigner.log" >&2; fractum "apksigner"; }
echo "APK: $D/$TITULUS.apk ($(du -k "$D/$TITULUS.apk" | awk '{print $1}') KiB, ${#OBJ[@]} obiecta)"
[ "$INSTITUERE" = 1 ] || exit 0

# --- tabula --------------------------------------------------------------
. tools/android/communia.sh
android_tabulam_eligere
android_seram_capere
# fumus (testa-spec II.8): proprietas -> lanceatio -> ictus per 'input
# swipe' -> EXITUS= in logcat -> fumus.png per run-as -> deinstallatio
# (regula Frani: nihil in tabula manet nisi -servare)
QUADRA_FUMI=600   # ~10 s: spatium ictui inter lanceationem et finem
fumum_finire () {
    A shell setprop debug.rhubarb.fumus 0 > /dev/null 2>&1
    # applicatio ANTE fumum instituta = data usoris (picturae liberorum):
    # numquam deinstallatur - fumus volumine temporario pingit
    if [ "${PRAEINSTALLATUM:-1}" = 1 ]; then
        echo "servatum: $INVOLUCRUM ante fumum aderat (data usoris manent)"
        return 0
    fi
    if [ "$SERVARE" = 1 ]; then
        echo "servatum: $INVOLUCRUM (adb uninstall $INVOLUCRUM)"
        return 0
    fi
    A uninstall "$INVOLUCRUM" > /dev/null 2>&1
    if A shell pm list packages 2>/dev/null | tr -d '\r' | grep -qx "package:$INVOLUCRUM"; then
        echo "DEINSTALLATIO FRACTA: $INVOLUCRUM manet in tabula" >&2
        return 1
    fi
    echo "deinstallatum: $INVOLUCRUM"
}
finis () {
    local status=$?
    if [ "$FUMUS" = 1 ] && ! fumum_finire && [ "$status" -lt 2 ]; then
        status=2
    fi
    android_seram_liberare
    exit "$status"
}
trap finis EXIT

trap 'exit 130' INT TERM
PRAEINSTALLATUM=0
if A shell pm list packages 2>/dev/null | tr -d '\r' | grep -qx "package:$INVOLUCRUM"; then
    PRAEINSTALLATUM=1
fi
A install -r "$D/$TITULUS.apk" > "$D/instituere.log" 2>&1 \
    || { cat "$D/instituere.log" >&2; apparatus "adb install fractum"; }
echo "institutum: $INVOLUCRUM in $ANDROID_SERIAL"
if [ "$FUMUS" = 1 ]; then
    # schirmus dormiens aut clausus = activitas in pausa, quadra nulla
    # (testa recte): apparatus statim, non XL s exspectatio (C30 2026-10-23)
    if ! A shell dumpsys power 2>/dev/null | tr -d '\r' | grep -q 'mWakefulness=Awake' \
        || A shell dumpsys window 2>/dev/null | tr -d '\r' | grep -q 'isKeyguardShowing=true'; then
        apparatus "tabula $ANDROID_SERIAL dormit aut clausa est - expergefac et resera, deinde iterum"
    fi
    A shell setprop debug.rhubarb.fumus "$QUADRA_FUMI" || apparatus "setprop fractum"
    A logcat -c
    A shell am start -W -n "$INVOLUCRUM/android.app.NativeActivity" > /dev/null \
        || apparatus "am start fractum"
    sleep 1
    # ictus trans tabulam (puncta fenestrae, landscape 1024x600)
    A shell input swipe 300 250 700 350 400 || apparatus "input swipe fractum"
    EXITUS=""
    for _ in $(seq 1 40); do
        EXITUS=$(A logcat -d -s "$TITULUS:I" 2>/dev/null | tr -d '\r' \
            | awk -F'EXITUS=' 'NF > 1 {print $2}' | tail -1)
        [ -n "$EXITUS" ] && break
        sleep 1
    done
    A logcat -d -s "$TITULUS:I" 2>/dev/null | tr -d '\r' > "$D/logcat.txt"
    [ -n "$EXITUS" ] || apparatus "nulla linea EXITUS intra XL s (logcat: $D/logcat.txt)"
    A exec-out run-as "$INVOLUCRUM" cat files/fumus.png > "$D/fumus.png" 2>/dev/null
    grep -q 'ludus: quadra=' "$D/logcat.txt" || fractum "linea mensurarum deest ($D/logcat.txt)"
    [ "$EXITUS" = 0 ] || fractum "EXITUS=$EXITUS ($D/logcat.txt)"
    [ "$(head -c 4 "$D/fumus.png" | od -An -c | tr -d ' ')" = "211PNG" ] \
        || fractum "fumus.png non PNG ($D/fumus.png)"
    echo "FUMUS ANDROID: EXITUS=0, imago $D/fumus.png"
    exit 0
fi

if [ "$CURRERE" = 1 ]; then
    A shell am start -n "$INVOLUCRUM/android.app.NativeActivity" > /dev/null \
        || apparatus "am start fractum"
    echo "currit: $INVOLUCRUM (logcat: adb logcat -s $TITULUS)"
fi
exit 0
