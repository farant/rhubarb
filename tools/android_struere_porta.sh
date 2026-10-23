#!/bin/bash
# tools/android_struere_porta.sh - PORTA android-struere (testa-spec
# II.10): APK pictoris SINE tabula struitur. Bibliotheca communis non
# portabilis (vocatio platformae, fons .m) nexum frangit, symbolo
# nominato (-Wl,--no-undefined). NDK aut SDK absens = RECUSATIO
# (exitus 2, signum nullum - porta 'non cucurrit'), numquam saltus
# tacitus.
# <porta titulus="android-struere" gradus="fusio" instrumentum="bin/aedilis" tegit="include/* lib/*.c apps/pictor/* tools/android/* tools/vexilla.sh silva/fontes/systema_posix.h officina/auspex_android.sh"/>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
# lexicon NDK examinis (T3b) contra capita vera - forma falsa = iudicium
# falsum in omni plagula Android
./officina/auspex_android.sh
rc=$?
if [ "$rc" = 0 ]; then
    ./tools/android/apk.sh apps/pictor/pictor.c
    rc=$?
fi
case $rc in
    0) echo "PORTA ANDROID-STRUERE: viridis (pictor)" ;;
    1) echo "PORTA ANDROID-STRUERE: FRACTA (pictor)" ;;
    *) echo "PORTA ANDROID-STRUERE: RECUSATA (apparatus, exitus $rc) - NDK/SDK: tools/android/ndk_fixum.sh" >&2 ;;
esac
exit $rc
