#!/bin/bash
# tools/android/ndk_instituere.sh - NDK fixum depromere, verificare,
# instituere (android-gradus II.3). Manu, numquam porta.
#
# Usus: ./tools/android/ndk_instituere.sh                  # depromit + instituit
#       ./tools/android/ndk_instituere.sh -verificare ZIP  # solum confert
#
# Recusat (exitus 2, causa nominata): magnitudo, sha1 (manifestum
# Google), sha256 (fixum nostrum) discrepantes; directorium alienum in
# sede fixa. Institutum rectum nuntiatur et relinquitur (exitus 0).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2
. tools/android/ndk_fixum.sh

verificare () {
    local m s1 s256
    m=$(stat -f %z "$1" 2>/dev/null) || { echo "ndk: $1 legi nequit" >&2; return 2; }
    [ "$m" = "$NDK_MAGNITUDO" ] || { echo "ndk: magnitudo $m != $NDK_MAGNITUDO (fixum)" >&2; return 2; }
    s1=$(shasum -a 1 "$1" | cut -d' ' -f1)
    [ "$s1" = "$NDK_SHA1" ] || { echo "ndk: sha1 $s1 != $NDK_SHA1 (manifestum Google)" >&2; return 2; }
    s256=$(shasum -a 256 "$1" | cut -d' ' -f1)
    [ "$s256" = "$NDK_SHA256" ] || { echo "ndk: sha256 $s256 != $NDK_SHA256 (fixum nostrum)" >&2; return 2; }
    return 0
}

institutum () {
    grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null
}

if [ "${1:-}" = "-verificare" ]; then
    [ $# -eq 2 ] || { echo "usus: $0 -verificare ZIP" >&2; exit 2; }
    verificare "$2" || exit 2
    echo "ndk: $2 congruit (NDK $NDK_NOMEN $NDK_VERSIO)"
    exit 0
fi
[ $# -eq 0 ] || { echo "usus: $0 [-verificare ZIP]" >&2; exit 2; }

if institutum; then
    echo "ndk: $NDK_NOMEN ($NDK_VERSIO) iam institutum: $NDK_SEDES"
    exit 0
fi
if [ -e "$NDK_SEDES" ]; then
    echo "ndk: $NDK_SEDES exstat sed Pkg.Revision non est $NDK_VERSIO - manu inspice et remove" >&2
    exit 2
fi
T=$(mktemp -d "${TMPDIR:-/tmp}/ndk_instituere.XXXXXX") || exit 2
trap 'rm -rf "$T"' EXIT
echo "ndk: depromo $NDK_URL ($NDK_MAGNITUDO octeti)"
curl -fL --progress-bar -o "$T/ndk.zip" "$NDK_URL" || { echo "ndk: depromptio fracta" >&2; exit 2; }
verificare "$T/ndk.zip" || exit 2
unzip -q "$T/ndk.zip" -d "$T/x" || { echo "ndk: unzip fractum" >&2; exit 2; }
mkdir -p "$(dirname "$NDK_SEDES")" || exit 2
mv "$T/x/android-ndk-$NDK_NOMEN" "$NDK_SEDES" || { echo "ndk: translatio fracta" >&2; exit 2; }
institutum || { echo "ndk: post institutionem Pkg.Revision non congruit" >&2; exit 2; }
echo "ndk: $NDK_NOMEN ($NDK_VERSIO) institutum: $NDK_SEDES"
