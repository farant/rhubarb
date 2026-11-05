#!/bin/bash
# tools/ios_struere_porta.sh - PORTA ios-struere (testa-ios-spec III.4):
# applicatio pictoris pro SIMULATORE iOS struitur, sine telephono, signo
# ad hoc solo. Bibliotheca communis non portabilis (vocatio macOS sola,
# caput UIKit absens) compilationem aut nexum frangit, plagula nominata.
# Xcode aut SDK iphonesimulator absens = RECUSATIO (exitus 2, signum
# nullum - porta 'non cucurrit'), numquam saltus tacitus.
# <porta titulus="ios-struere" gradus="fusio" tegit="include/* lib/*.c lib/*_ios.m apps/pictor/* tools/ios/* tools/vexilla.sh aedilis.stml"/>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
# divulgare.sh: -siccum et -notae simul recusantur (ultimum vincebat -
# 'siccum' tacite neglectum, pictor X vere divulgatus 2026-11-04).
# Applicatio 'zz' non est: etiam sine recusatione ante omne rete cadit.
per_ordinem () {
    local exitus rc
    exitus=$(./tools/ios/divulgare.sh zz "$@" 2>&1); rc=$?
    if [ $rc -ne 2 ] || ! printf '%s' "$exitus" | grep -q "simul"; then
        echo "PORTA IOS-STRUERE: FRACTA (divulgare.sh $* non recusat:" \
            "exitus $rc, $exitus)"
        exit 1
    fi
}
per_ordinem -siccum -notae /dev/null
per_ordinem -notae /dev/null -siccum
# tergum-plan T4: UIBackgroundModes [remote-notification] in Info.plist
# solum si profilum push fert (aps-environment); bis = idem
modi_tergi () {
    local dir cum sine info rc
    apparatus () { echo "PORTA IOS-STRUERE: FRACTA (modi tergi: $*)"; exit 1; }
    . tools/ios/communia.sh
    dir=$(mktemp -d -t modi_tergi) || apparatus "mktemp"
    cum="$dir/cum.plist"; sine="$dir/sine.plist"; info="$dir/Info.plist"
    plutil -create xml1 "$sine" && plutil -insert Entitlements -dictionary "$sine" \
        && plutil -insert Entitlements.get-task-allow -bool true "$sine" \
        && cp "$sine" "$cum" \
        && plutil -insert Entitlements.aps-environment -string production "$cum" \
        || apparatus "profila ficta"
    fictum_info () {
        sed -e "s/@TITULUS@/zz/g" -e "s/@INVOLUCRUM@/org.example.zz/g" \
            -e "s/@PLATFORMA@/iPhoneOS/g" -e "s/@VERSIO@/1.0/g" \
            -e "s/@AEDIFICATIO@/1/g" tools/ios/Info.forma.plist > "$info" \
            || apparatus "Info.plist fictum"
    }
    fictum_info
    ios_modos_tergi_ponere "$sine" "$info"; rc=$?
    [ $rc -eq 0 ] || apparatus "sine aps exitus $rc"
    plutil -extract UIBackgroundModes raw -o - "$info" > /dev/null 2>&1 \
        && apparatus "sine aps UIBackgroundModes adest"
    ios_modos_tergi_ponere "$cum" "$info" && ios_modos_tergi_ponere "$cum" "$info" \
        || apparatus "cum aps exitus non 0"
    [ "$(plutil -extract UIBackgroundModes.0 raw -o - "$info" 2>/dev/null)" = remote-notification ] \
        || apparatus "cum aps remote-notification deest"
    [ "$(plutil -extract UIBackgroundModes raw -o - "$info")" = 1 ] \
        || apparatus "bis positum: non unum elementum"
    plutil -lint "$info" > /dev/null || apparatus "Info.plist invalidum"
    rm -rf "$dir"
}
( modi_tergi ) || exit 1
./tools/ios/app.sh apps/pictor/pictor.c -simulator
rc=$?
case $rc in
    0) echo "PORTA IOS-STRUERE: viridis (pictor)" ;;
    1) echo "PORTA IOS-STRUERE: FRACTA (pictor)" ;;
    *) echo "PORTA IOS-STRUERE: RECUSATA (apparatus, exitus $rc) - Xcode: xcode-select -p" >&2 ;;
esac
exit $rc
