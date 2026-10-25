#!/bin/bash
# tools/ios/app.sh - applicatio testae iOS (testa-ios-spec II.4): clausura
# aedilis (--varians ios,posix: lib/testa_ios.m per testa.h) -> xcrun
# clang -> <X>.app (Info.plist ex tools/ios/Info.forma.plist) -> codesign
# -> simctl (simulator) aut devicectl (telephonum). Sine projecto Xcode
# (D3). Frater apk.sh.
#
# Usus: ./tools/ios/app.sh <apps/X/X.c> [-simulator | -tabula] [-currere]
#                          [-fumus] [-servare]
#   -simulator  (ordinarium) SDK iphonesimulator, signum ad hoc
#   -tabula     telephonum (USB aut rete): profilum evolutionis '<turma>.*'
#               (D7), identitas eius ex claviario, devicectl
#   -currere    institutio + lanceatio (simulator: fenestra aperitur)
#   -fumus      lanceatio cum -fumus: EXITUS=, fumus.png (R4: ictus
#               syntheticus a testa ipsa); -servare: post fumum non
#               deinstallat. Applicatio ANTE fumum instituta numquam
#               deinstallatur (picturae in telephono).
# Simulator: IOS_SIMULATOR (UDID aut nomen), aliter is qui iam currit,
# aliter "iPhone 15 Pro Max" (telephonum Frani) runtime novissimo.
# Telephonum: IOS_DEVICE, IOS_PROFILUM (tools/ios/communia.sh).
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
TITULUS=$(basename "$FONS" .c)
case "$TITULUS" in
    ""|*[!a-z]*) apparatus "titulus '$TITULUS': litterae minusculae solae (involucrum earth.golden.$TITULUS)" ;;
esac
INVOLUCRUM="earth.golden.$TITULUS"

# --- instrumenta -------------------------------------------------------
if [ "$SCOPUS" = tabula ]; then
    SDK=iphoneos; SCOPUS_CLANG=arm64-apple-ios17.0; PLATFORMA=iPhoneOS
else
    SDK=iphonesimulator; SCOPUS_CLANG=arm64-apple-ios17.0-simulator
    PLATFORMA=iPhoneSimulator
fi
command -v xcrun > /dev/null || apparatus "xcrun deest (Xcode)"
xcrun --sdk "$SDK" --show-sdk-path > /dev/null 2>&1 \
    || apparatus "SDK $SDK deest (Xcode: xcode-select -p = $(xcode-select -p 2>/dev/null))"
[ -x bin/aedilis ] || apparatus "bin/aedilis deest - ./tools/aedilis_struere.sh"
command -v plutil > /dev/null || apparatus "plutil deest"
D="build/ios/$SDK/$TITULUS"
APP="$D/$TITULUS.app"
rm -rf "$D" && mkdir -p "$D/obiecta" "$APP" || apparatus "$D creari nequit"

# telephonum, profilum, identitas ANTE compilationem (apparatus cito)
if [ "$SCOPUS" = tabula ]; then
    . tools/ios/communia.sh
    ios_tabulam_eligere
    ios_profilum_eligere "$D"
    ios_identitatem_eligere
    echo "telephonum: $IOS_NOMEN ($IOS_MODELLUM, iOS $IOS_VERSIO); profilum $(basename "$IOS_PROFILUM"); $IOS_IDENTITAS_NOMEN"
fi

. tools/vexilla.sh
vexilla_radices ""
CC=(xcrun --sdk "$SDK" clang -target "$SCOPUS_CLANG")
# .c et .m eisdem vexillis domus (ut fenestra_macos.m in aedilis)
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}" "-DTESTA_TITULUS=\"$TITULUS\"")
VFLAGS=(-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- constructio -------------------------------------------------------
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
# icon (testa-ios C2 T3): apps/<app>/icon.png (quadratum, latus MXXIV
# dividens) -> MXXIV RGB sine alpha (icones_amplificare) -> catalogus
# AppIcon -> Assets.car per actool (Apple; D6) -> claves in Info.plist
ICON="$(dirname "$FONS")/icon.png"
if [ -f "$ICON" ]; then
    if ! bin/icones_instrumentum 2>&1 | grep -q amplificare; then
        ./compile_tools.sh icones_instrumentum > "$D/icones.log" 2>&1 \
            || { tail -5 "$D/icones.log" >&2; fractum "icones_instrumentum aedificari nequit"; }
    fi
    CATALOGUS="$D/Assets.xcassets"
    mkdir -p "$CATALOGUS/AppIcon.appiconset" || apparatus "$CATALOGUS creari nequit"
    bin/icones_instrumentum -amplificare "$ICON" 1024 \
        "$CATALOGUS/AppIcon.appiconset/icon_1024.png" > "$D/icon.log" 2>&1 \
        || { cat "$D/icon.log" >&2; fractum "icon $ICON (quadratum, latus 1024 dividens)"; }
    printf '%s\n' '{"info":{"author":"rhubarb","version":1}}' > "$CATALOGUS/Contents.json"
    printf '%s\n' '{"images":[{"filename":"icon_1024.png","idiom":"universal","platform":"ios","size":"1024x1024"}],"info":{"author":"rhubarb","version":1}}' \
        > "$CATALOGUS/AppIcon.appiconset/Contents.json"
    xcrun actool "$CATALOGUS" --compile "$APP" --platform "$SDK" \
        --minimum-deployment-target 17.0 --app-icon AppIcon --target-device iphone \
        --output-partial-info-plist "$D/icon_partialis.plist" > "$D/actool.log" 2>&1 \
        || { tail -20 "$D/actool.log" >&2; fractum "actool"; }
    [ -f "$APP/Assets.car" ] || { tail -20 "$D/actool.log" >&2; fractum "Assets.car deest post actool"; }
    /usr/libexec/PlistBuddy -c "Merge $D/icon_partialis.plist" "$APP/Info.plist" > /dev/null \
        || fractum "claves iconis in Info.plist misceri nequeunt"
    # CFBundleIconName etiam in summo (validatio Apple ITMS-90713; Xcode
    # utrumque ponit) - actool eum solum in CFBundleIcons ponit
    /usr/libexec/PlistBuddy -c "Add :CFBundleIconName string AppIcon" \
        "$APP/Info.plist" > /dev/null 2>&1 \
        || /usr/libexec/PlistBuddy -c "Set :CFBundleIconName AppIcon" \
               "$APP/Info.plist" > /dev/null \
        || fractum "CFBundleIconName in Info.plist poni nequit"
fi
if [ "$SCOPUS" = tabula ]; then
    # D7: profilum inclusum + iura ex eo (turma.involucrum, debugging)
    cp "$IOS_PROFILUM" "$APP/embedded.mobileprovision" \
        || apparatus "profilum exscribi nequit"
    cat > "$D/iura.plist" <<EOF_IURA
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>application-identifier</key>
	<string>$IOS_TURMA.$INVOLUCRUM</string>
	<key>com.apple.developer.team-identifier</key>
	<string>$IOS_TURMA</string>
	<key>get-task-allow</key>
	<true/>
</dict>
</plist>
EOF_IURA
    plutil -lint "$D/iura.plist" > /dev/null || fractum "iura.plist invalidum"
    codesign --force --sign "$IOS_IDENTITAS" --entitlements "$D/iura.plist" \
        --timestamp=none "$APP" > "$D/codesign.log" 2>&1 \
        || { cat "$D/codesign.log" >&2; apparatus "codesign ($IOS_IDENTITAS_NOMEN) fractum - claviarium clausum?"; }
    codesign --verify --strict "$APP" > /dev/null 2>&1 || fractum "codesign --verify"
else
    # simulator: signum ad hoc (sine identitate)
    codesign --force --sign - --timestamp=none "$APP" > "$D/codesign.log" 2>&1 \
        || { cat "$D/codesign.log" >&2; fractum "codesign ad hoc"; }
fi
echo "APP: $APP ($(du -sk "$APP" | awk '{print $1}') KiB, ${#OBJ[@]} obiecta)"
[ "$CURRERE" = 1 ] || [ "$FUMUS" = 1 ] || exit 0

# --- apparatus scopi: eligere, praesentia, instituere, deinstallare ------
# D8 (Review Focus 5): Xcode vetus contra iOS novum - causa nominata,
# NUMQUAM iteratio caeca
d8_diagnosis () {
    if grep -qiE 'ddi|developer disk image|not supported|unsupported|newer version|requires.*(xcode|ios)|incompatible' "$1"; then
        apparatus "$2 fractum, verisimiliter versio (Xcode $(xcodebuild -version 2>/dev/null | head -1 | awk '{print $2}') contra iOS ${IOS_VERSIO:-?}; testa-ios D8: Sequoia + Xcode 26) - NON iterare, rem Frano refer ($1)"
    fi
    apparatus "$2 fractum ($1)"
}

if [ "$SCOPUS" = tabula ]; then
    ios_seram_capere
    UBI="$IOS_NOMEN"
    # praesentia (recensio B1 I1): ABSENS solum si quaestio successit et
    # applicatio deest; error quilibet = praesens (picturae manent)
    praesens () {
        local n
        xcrun devicectl device info apps --device "$IOS_TABULA" \
            --bundle-id "$INVOLUCRUM" --json-output "$D/applicationes.json" \
            > /dev/null 2>&1 || return 0
        n=$(plutil -extract result.apps raw -o - "$D/applicationes.json" 2>/dev/null) || return 0
        [ "$n" != 0 ]
    }
    deinstallare () {
        xcrun devicectl device uninstall app --device "$IOS_TABULA" "$INVOLUCRUM" > /dev/null 2>&1
    }
    liberare () { ios_seram_liberare; }
else
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
    UBI=$(xcrun simctl list devices | grep -F "$UDID" | sed 's/^ *//; s/ (.*//' | head -1)
    if ! xcrun simctl list devices booted | grep -qF "$UDID"; then
        xcrun simctl boot "$UDID" > "$D/simulator.log" 2>&1 \
            || { cat "$D/simulator.log" >&2; apparatus "simctl boot $UDID fractum"; }
    fi
    xcrun simctl bootstatus "$UDID" -b > /dev/null 2>&1 \
        || apparatus "simulator $UDID non incitatus (bootstatus)"
    echo "simulator: $UBI ($UDID)"
    praesens () {
        local apps
        apps=$(xcrun simctl listapps "$UDID" 2>/dev/null) && [ -n "$apps" ] || return 0
        printf '%s\n' "$apps" | grep -qF "\"$INVOLUCRUM\""
    }
    deinstallare () { xcrun simctl uninstall "$UDID" "$INVOLUCRUM" > /dev/null 2>&1; }
    liberare () { :; }
fi

PRAEINSTALLATUM=1
INSTITUTUM_NOSTRUM=0
praesens || PRAEINSTALLATUM=0
fumum_finire () {
    if [ "$PRAEINSTALLATUM" = 1 ] || [ "$INSTITUTUM_NOSTRUM" != 1 ]; then
        echo "servatum: $INVOLUCRUM ante fumum aderat (data usoris manent)"
        return 0
    fi
    if [ "$SERVARE" = 1 ]; then
        echo "servatum: $INVOLUCRUM in $UBI (-servare)"
        return 0
    fi
    deinstallare
    if praesens; then
        echo "DEINSTALLATIO FRACTA (aut ignota): $INVOLUCRUM manet in $UBI" >&2
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
    liberare
    exit "$status"
}
trap finis EXIT
trap 'exit 130' INT TERM

if [ "$SCOPUS" = tabula ]; then
    xcrun devicectl device install app --device "$IOS_TABULA" "$APP" \
        > "$D/instituere.log" 2>&1 \
        || { tail -20 "$D/instituere.log" >&2; d8_diagnosis "$D/instituere.log" "devicectl install"; }
else
    xcrun simctl install "$UDID" "$APP" > "$D/instituere.log" 2>&1 \
        || { cat "$D/instituere.log" >&2; apparatus "simctl install fractum"; }
fi
INSTITUTUM_NOSTRUM=1
echo "institutum: $INVOLUCRUM in $UBI"

if [ "$FUMUS" = 1 ]; then
    # consola: stdout applicationis in plagulam; lanceatio manet donec
    # applicatio exit (exit() post EXITUS=)
    if [ "$SCOPUS" = tabula ]; then
        xcrun devicectl device process launch --device "$IOS_TABULA" --console \
            --terminate-existing "$INVOLUCRUM" -fumus > "$D/consola.txt" 2>&1 &
    else
        xcrun simctl launch --console-pty --terminate-running-process "$UDID" \
            "$INVOLUCRUM" -fumus > "$D/consola.txt" 2>&1 &
    fi
    CONSOLA_PID=$!
    EXITUS=""
    for _ in $(seq 1 90); do
        EXITUS=$(tr -d '\r' < "$D/consola.txt" | awk -F'EXITUS=' 'NF > 1 {print $2}' | tail -1)
        [ -n "$EXITUS" ] && break
        kill -0 "$CONSOLA_PID" 2> /dev/null || break
        sleep 1
    done
    sleep 1
    kill "$CONSOLA_PID" 2> /dev/null
    CONSOLA_PID=""
    if [ -z "$EXITUS" ]; then
        [ "$SCOPUS" = tabula ] && d8_diagnosis "$D/consola.txt" "lanceatio (nulla linea EXITUS intra XC s)"
        apparatus "nulla linea EXITUS intra XC s (consola: $D/consola.txt)"
    fi
    if [ "$SCOPUS" = tabula ]; then
        xcrun devicectl device copy from --device "$IOS_TABULA" \
            --domain-type appDataContainer --domain-identifier "$INVOLUCRUM" \
            --source Documents/fumus.png --destination "$D/fumus.png" \
            > "$D/exscribere.log" 2>&1
    else
        DATA=$(xcrun simctl get_app_container "$UDID" "$INVOLUCRUM" data 2>/dev/null) \
            || apparatus "receptaculum datorum $INVOLUCRUM ignotum"
        cp "$DATA/Documents/fumus.png" "$D/fumus.png" 2> /dev/null
    fi
    grep -q 'ludus: quadra=' "$D/consola.txt" || fractum "linea mensurarum deest ($D/consola.txt)"
    [ "$EXITUS" = 0 ] || fractum "EXITUS=$EXITUS ($D/consola.txt)"
    [ "$(head -c 4 "$D/fumus.png" 2>/dev/null | od -An -c | tr -d ' ')" = "211PNG" ] \
        || fractum "fumus.png non PNG ($D/fumus.png)"
    echo "FUMUS IOS ($SCOPUS): EXITUS=0, imago $D/fumus.png"
    exit 0
fi

if [ "$CURRERE" = 1 ]; then
    if [ "$SCOPUS" = tabula ]; then
        xcrun devicectl device process launch --device "$IOS_TABULA" \
            --terminate-existing "$INVOLUCRUM" > "$D/lanceatio.log" 2>&1 \
            || { tail -20 "$D/lanceatio.log" >&2; d8_diagnosis "$D/lanceatio.log" "devicectl launch"; }
        echo "currit: $INVOLUCRUM in $UBI (effusio: xcrun devicectl device process launch --console --device $IOS_TABULA $INVOLUCRUM)"
    else
        open -a Simulator --args -CurrentDeviceUDID "$UDID" > /dev/null 2>&1
        xcrun simctl launch --terminate-running-process "$UDID" "$INVOLUCRUM" \
            > "$D/lanceatio.log" 2>&1 \
            || { cat "$D/lanceatio.log" >&2; apparatus "simctl launch fractum"; }
        echo "currit: $INVOLUCRUM in $UBI (effusio: xcrun simctl launch --console-pty $UDID $INVOLUCRUM)"
    fi
fi
exit 0
