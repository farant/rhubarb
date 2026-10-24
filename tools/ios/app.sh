#!/bin/bash
# tools/ios/app.sh - applicatio testae iOS (testa-ios-spec II.4): clausura
# aedilis (--varians ios,posix: lib/testa_ios.m per testa.h) -> xcrun
# clang -> <X>.app (Info.plist ex tools/ios/Info.forma.plist) -> codesign
# -> simctl (simulator). Sine projecto Xcode (D3). Frater apk.sh.
#
# Usus: ./tools/ios/app.sh <apps/X/X.c> [-simulator | -tabula] [-currere]
#                          [-fumus] [-servare]
#   -simulator  (ordinarium) SDK iphonesimulator, signum ad hoc
#   -tabula     telephonum (testa-ios T4)
#   -currere    institutio + lanceatio (fenestra Simulatoris aperitur)
#   -fumus      lanceatio cum -fumus: EXITUS=, fumus.png (R4: ictus
#               syntheticus a testa ipsa); -servare: post fumum non
#               deinstallat
# Simulator: IOS_SIMULATOR (UDID aut nomen), aliter is qui iam currit,
# aliter "iPhone 15 Pro Max" (telephonum Frani) runtime novissimo.
#
# Exitus: 0 | 1 constructio fracta (compilatio, nexus, fumus) | 2 apparatus
set -u
export LC_ALL=C
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

apparatus () { echo "ios/app.sh: $*" >&2; exit 2; }
fractum () { echo "ios/app.sh: FRACTUM: $*" >&2; exit 1; }

SCOPUS=simulator; CURRERE=0; FUMUS=0; SERVARE=0; FONS=""
while [ $# -gt 0 ]; do
    case "$1" in
        -simulator) SCOPUS=simulator ;;
        -tabula) SCOPUS=tabula ;;
        -currere) CURRERE=1 ;;
        -fumus) FUMUS=1 ;;
        -servare) SERVARE=1 ;;
        -*) apparatus "optio ignota: $1" ;;
        *) [ -z "$FONS" ] || apparatus "fons unus: $FONS et $1"; FONS="$1" ;;
    esac
    shift
done
[ -n "$FONS" ] || apparatus "usus: $0 <apps/X/X.c> [-simulator|-tabula] [-currere] [-fumus] [-servare]"
[ -f "$FONS" ] || apparatus "$FONS deest"
[ "$SCOPUS" = simulator ] || apparatus "-tabula: nondum (testa-ios T4)"
TITULUS=$(basename "$FONS" .c)
case "$TITULUS" in
    ""|*[!a-z]*) apparatus "titulus '$TITULUS': litterae minusculae solae (involucrum earth.golden.$TITULUS)" ;;
esac
INVOLUCRUM="earth.golden.$TITULUS"

# --- instrumenta -------------------------------------------------------
SDK=iphonesimulator
SCOPUS_CLANG=arm64-apple-ios17.0-simulator
PLATFORMA=iPhoneSimulator
command -v xcrun > /dev/null || apparatus "xcrun deest (Xcode)"
xcrun --sdk "$SDK" --show-sdk-path > /dev/null 2>&1 \
    || apparatus "SDK $SDK deest (Xcode: xcode-select -p = $(xcode-select -p 2>/dev/null))"
[ -x bin/aedilis ] || apparatus "bin/aedilis deest - ./tools/aedilis_struere.sh"
command -v plutil > /dev/null || apparatus "plutil deest"

. tools/vexilla.sh
vexilla_radices ""
CC=(xcrun --sdk "$SDK" clang -target "$SCOPUS_CLANG")
# .c et .m eisdem vexillis domus (ut fenestra_macos.m in aedilis)
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}" "-DTESTA_TITULUS=\"$TITULUS\"")
VFLAGS=(-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- constructio -------------------------------------------------------
D="build/ios/$SDK/$TITULUS"
APP="$D/$TITULUS.app"
rm -rf "$D" && mkdir -p "$D/obiecta" "$APP" || apparatus "$D creari nequit"
PARTES=$(bin/aedilis "$FONS" --varians ios,posix --partes 2>&1) \
    || apparatus "aedilis fracta: $PARTES"
NEXUS=$(bin/aedilis "$FONS" --varians ios,posix --nexus 2>&1) \
    || apparatus "aedilis --nexus fracta: $NEXUS"
OBIECTA=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "O" {print $2}')
VENDITI=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "V" {print $2}')
printf '%s\n' "$OBIECTA" | grep -qx 'lib/testa_ios.m' \
    || fractum "lib/testa_ios.m non in clausura ($FONS testa.h includit?)"
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
# vexilla nexus ex regulis aedilis.stml ('-framework UIKit' = verba II)
NEXUS_V=()
while read -r linea; do
    [ -n "$linea" ] || continue
    # shellcheck disable=SC2206
    NEXUS_V+=($linea)
done <<< "$NEXUS"
"${CC[@]}" -o "$APP/$TITULUS" "${OBJ[@]}" "${NEXUS_V[@]}" -lm > "$D/nexus.log" 2>&1 \
    || { tail -15 "$D/nexus.log" >&2; fractum "nexus (symbolum absens = pars platformae deest)"; }

sed -e "s/@TITULUS@/$TITULUS/g" -e "s/@INVOLUCRUM@/$INVOLUCRUM/g" \
    -e "s/@PLATFORMA@/$PLATFORMA/g" tools/ios/Info.forma.plist > "$APP/Info.plist"
plutil -lint "$APP/Info.plist" > /dev/null || fractum "Info.plist invalidum"
printf 'APPL????' > "$APP/PkgInfo"
# simulator: signum ad hoc (sine identitate)
codesign --force --sign - --timestamp=none "$APP" > "$D/codesign.log" 2>&1 \
    || { cat "$D/codesign.log" >&2; fractum "codesign ad hoc"; }
echo "APP: $APP ($(du -sk "$APP" | awk '{print $1}') KiB, ${#OBJ[@]} obiecta)"
[ "$CURRERE" = 1 ] || [ "$FUMUS" = 1 ] || exit 0

# --- simulator -----------------------------------------------------------
UDID=""
if [ -n "${IOS_SIMULATOR:-}" ]; then
    UDID=$(xcrun simctl list devices available | grep -F "$IOS_SIMULATOR" \
        | sed -n 's/.*(\([0-9A-F-]\{36\}\)).*/\1/p' | tail -1)
    [ -n "$UDID" ] || apparatus "IOS_SIMULATOR='$IOS_SIMULATOR' non inventum (xcrun simctl list devices available)"
fi
if [ -z "$UDID" ]; then
    UDID=$(xcrun simctl list devices booted | grep '(Booted)' \
        | sed -n 's/.*(\([0-9A-F-]\{36\}\)) (Booted).*/\1/p' | head -1)
fi
if [ -z "$UDID" ]; then
    # runtimes ordine ascendente: ultimum = novissimum
    UDID=$(xcrun simctl list devices available | grep -F 'iPhone 15 Pro Max (' \
        | sed -n 's/.*(\([0-9A-F-]\{36\}\)).*/\1/p' | tail -1)
    [ -n "$UDID" ] || apparatus "nullus simulator 'iPhone 15 Pro Max' (IOS_SIMULATOR pone)"
fi
NOMEN_SIM=$(xcrun simctl list devices | grep -F "$UDID" | sed 's/^ *//; s/ (.*//' | head -1)
if ! xcrun simctl list devices booted | grep -qF "$UDID"; then
    xcrun simctl boot "$UDID" > "$D/simulator.log" 2>&1 \
        || { cat "$D/simulator.log" >&2; apparatus "simctl boot $UDID fractum"; }
fi
xcrun simctl bootstatus "$UDID" -b > /dev/null 2>&1 \
    || apparatus "simulator $UDID non incitatus (bootstatus)"
echo "simulator: $NOMEN_SIM ($UDID)"

# praesentia (recensio B1 I1): ABSENS solum si listapps successit et
# involucrum deest; error quilibet = praesens (data usoris manent)
PRAEINSTALLATUM=1
INSTITUTUM_NOSTRUM=0
if APPS=$(xcrun simctl listapps "$UDID" 2>/dev/null) && [ -n "$APPS" ] \
    && ! printf '%s\n' "$APPS" | grep -qF "\"$INVOLUCRUM\""; then
    PRAEINSTALLATUM=0
fi
fumum_finire () {
    if [ "$PRAEINSTALLATUM" = 1 ] || [ "$INSTITUTUM_NOSTRUM" != 1 ]; then
        echo "servatum: $INVOLUCRUM ante fumum aderat (data usoris manent)"
        return 0
    fi
    if [ "$SERVARE" = 1 ]; then
        echo "servatum: $INVOLUCRUM (xcrun simctl uninstall $UDID $INVOLUCRUM)"
        return 0
    fi
    xcrun simctl uninstall "$UDID" "$INVOLUCRUM" > /dev/null 2>&1
    if xcrun simctl listapps "$UDID" 2>/dev/null | grep -qF "\"$INVOLUCRUM\""; then
        echo "DEINSTALLATIO FRACTA: $INVOLUCRUM manet in simulatore" >&2
        return 1
    fi
    echo "deinstallatum: $INVOLUCRUM"
}
CONSOLA_PID=""
finis () {
    local status=$?
    [ -z "$CONSOLA_PID" ] || kill "$CONSOLA_PID" 2> /dev/null
    if [ "$FUMUS" = 1 ] && ! fumum_finire && [ "$status" -lt 2 ]; then
        status=2
    fi
    exit "$status"
}
trap finis EXIT
trap 'exit 130' INT TERM

xcrun simctl install "$UDID" "$APP" > "$D/instituere.log" 2>&1 \
    || { cat "$D/instituere.log" >&2; apparatus "simctl install fractum"; }
INSTITUTUM_NOSTRUM=1
echo "institutum: $INVOLUCRUM in $NOMEN_SIM"

if [ "$FUMUS" = 1 ]; then
    # --console-pty: stdout applicationis in plagulam; lanceatio manet
    # donec applicatio exit (exit() post EXITUS=)
    xcrun simctl launch --console-pty --terminate-running-process "$UDID" \
        "$INVOLUCRUM" -fumus > "$D/consola.txt" 2>&1 &
    CONSOLA_PID=$!
    EXITUS=""
    for _ in $(seq 1 60); do
        EXITUS=$(tr -d '\r' < "$D/consola.txt" | awk -F'EXITUS=' 'NF > 1 {print $2}' | tail -1)
        [ -n "$EXITUS" ] && break
        kill -0 "$CONSOLA_PID" 2> /dev/null || break
        sleep 1
    done
    sleep 1
    kill "$CONSOLA_PID" 2> /dev/null
    CONSOLA_PID=""
    [ -n "$EXITUS" ] || apparatus "nulla linea EXITUS intra LX s (consola: $D/consola.txt)"
    DATA=$(xcrun simctl get_app_container "$UDID" "$INVOLUCRUM" data 2>/dev/null) \
        || apparatus "receptaculum datorum $INVOLUCRUM ignotum"
    cp "$DATA/Documents/fumus.png" "$D/fumus.png" 2> /dev/null
    grep -q 'ludus: quadra=' "$D/consola.txt" || fractum "linea mensurarum deest ($D/consola.txt)"
    [ "$EXITUS" = 0 ] || fractum "EXITUS=$EXITUS ($D/consola.txt)"
    [ "$(head -c 4 "$D/fumus.png" 2>/dev/null | od -An -c | tr -d ' ')" = "211PNG" ] \
        || fractum "fumus.png non PNG ($D/fumus.png)"
    echo "FUMUS IOS: EXITUS=0, imago $D/fumus.png"
    exit 0
fi

if [ "$CURRERE" = 1 ]; then
    open -a Simulator --args -CurrentDeviceUDID "$UDID" > /dev/null 2>&1
    xcrun simctl launch --terminate-running-process "$UDID" "$INVOLUCRUM" \
        > "$D/lanceatio.log" 2>&1 \
        || { cat "$D/lanceatio.log" >&2; apparatus "simctl launch fractum"; }
    echo "currit: $INVOLUCRUM in $NOMEN_SIM (effusio: xcrun simctl launch --console-pty $UDID $INVOLUCRUM)"
fi
exit 0
