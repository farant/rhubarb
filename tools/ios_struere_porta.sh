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
./tools/ios/app.sh apps/pictor/pictor.c -simulator
rc=$?
case $rc in
    0) echo "PORTA IOS-STRUERE: viridis (pictor)" ;;
    1) echo "PORTA IOS-STRUERE: FRACTA (pictor)" ;;
    *) echo "PORTA IOS-STRUERE: RECUSATA (apparatus, exitus $rc) - Xcode: xcode-select -p" >&2 ;;
esac
exit $rc
