#!/bin/bash

# officina/auspex_android.sh - certificatio formarum NDK Android in
# silva/fontes/systema_posix.h (sectiones "android/...", testa B1 T3b,
# 2026-10-23; exemplar officina/auspex_posix.sh)
#
# Duae TU eandem TABULAM assertionum compilant, ambae per clang NDK
# (aarch64-linux-android30, forma vera tabulae Fire):
#   TU verum:    capita NDK VERA (<android/native_activity.h> ...)
#   TU nostrum:  systema_posix.h + praeludium (typi systema_c89.h)
# Ambae virides => formae nostrae == formae NDK (sizeof + offset omnium
# camporum quos testa tangit + valores enumerationum et macrorum).
# Examen lib/testa_android.c contra hoc caput iudicat - forma falsa =
# iudicium falsum.
#
# Usus: ./officina/auspex_android.sh   # ex radice repositorii
# Exitus: 0 certificatum | 1 dissentit | 2 NDK absens (numquam saltus
#         tacitus)

set -u
OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
cd "$RADIX_DIR" || exit 2

# shellcheck source=/dev/null
. tools/android/ndk_fixum.sh
CLANG="$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang"
if [ ! -x "$CLANG" ]; then
    echo "auspex_android: NDK absens: $CLANG (./tools/android/ndk_instituere.sh)" >&2
    exit 2
fi

GEN_DIR="$OFF_DIR/build/auspex_android"
mkdir -p "$GEN_DIR"

# TABULAE UNAE - fons veritatis (structura campus offset mensura)
TABULA_CAMPORUM="\
ARect left 0 4
ARect top 4 4
ARect right 8 4
ARect bottom 12 4
ANativeWindow_Buffer width 0 4
ANativeWindow_Buffer height 4 4
ANativeWindow_Buffer stride 8 4
ANativeWindow_Buffer format 12 4
ANativeWindow_Buffer bits 16 8
ANativeWindow_Buffer reserved 24 24
ANativeActivity callbacks 0 8
ANativeActivity vm 8 8
ANativeActivity env 16 8
ANativeActivity clazz 24 8
ANativeActivity internalDataPath 32 8
ANativeActivity externalDataPath 40 8
ANativeActivity sdkVersion 48 4
ANativeActivity instance 56 8
ANativeActivity assetManager 64 8
ANativeActivity obbPath 72 8
ANativeActivityCallbacks onStart 0 8
ANativeActivityCallbacks onResume 8 8
ANativeActivityCallbacks onSaveInstanceState 16 8
ANativeActivityCallbacks onPause 24 8
ANativeActivityCallbacks onStop 32 8
ANativeActivityCallbacks onDestroy 40 8
ANativeActivityCallbacks onWindowFocusChanged 48 8
ANativeActivityCallbacks onNativeWindowCreated 56 8
ANativeActivityCallbacks onNativeWindowResized 64 8
ANativeActivityCallbacks onNativeWindowRedrawNeeded 72 8
ANativeActivityCallbacks onNativeWindowDestroyed 80 8
ANativeActivityCallbacks onInputQueueCreated 88 8
ANativeActivityCallbacks onInputQueueDestroyed 96 8
ANativeActivityCallbacks onContentRectChanged 104 8
ANativeActivityCallbacks onConfigurationChanged 112 8
ANativeActivityCallbacks onLowMemory 120 8"

TABULA_MENSURARUM="\
ARect 16
ANativeWindow_Buffer 48
ANativeActivity 80
ANativeActivityCallbacks 128"

TABULA_VALORUM="\
WINDOW_FORMAT_RGBA_8888 1
ALOOPER_POLL_CALLBACK -2
ALOOPER_EVENT_INPUT 1
AINPUT_EVENT_TYPE_KEY 1
AINPUT_EVENT_TYPE_MOTION 2
AKEY_EVENT_ACTION_UP 1
AKEYCODE_BACK 4
AMOTION_EVENT_ACTION_MASK 255
AMOTION_EVENT_ACTION_POINTER_INDEX_MASK 65280
AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT 8
AMOTION_EVENT_ACTION_DOWN 0
AMOTION_EVENT_ACTION_UP 1
AMOTION_EVENT_ACTION_MOVE 2
AMOTION_EVENT_ACTION_CANCEL 3
AMOTION_EVENT_ACTION_POINTER_DOWN 5
AMOTION_EVENT_ACTION_POINTER_UP 6
AMOTION_EVENT_TOOL_TYPE_FINGER 1
AMOTION_EVENT_TOOL_TYPE_STYLUS 2
AMOTION_EVENT_TOOL_TYPE_MOUSE 3
AMOTION_EVENT_TOOL_TYPE_ERASER 4
ANDROID_LOG_INFO 4
PROP_VALUE_MAX 92"

emittere_asserta() {
    while read -r structura magnitudo; do
        echo "IUNGO_ASSERTA(sizeof(struct $structura) == $magnitudo);"
    done <<< "$TABULA_MENSURARUM"
    while read -r structura campus off mensura; do
        echo "IUNGO_ASSERTA(OFF($structura, $campus) == $off);"
        echo "IUNGO_ASSERTA(sizeof(((struct $structura*)0)->$campus) == $mensura);"
    done <<< "$TABULA_CAMPORUM"
    while read -r titulus valor; do
        echo "IUNGO_ASSERTA(($titulus) == ($valor));"
    done <<< "$TABULA_VALORUM"
}

# OFF per __builtin_offsetof: clang XVIII (NDK) formam '&((S*)0)->C' ut
# constantem non accipit (-Wgnu-folding-constant); nullum caput poscit
PRAEAMBULUM='#define IUNGO2(a,b) a##b
#define IUNGO(a,b) IUNGO2(a,b)
#define IUNGO_ASSERTA(cond) \
    typedef char IUNGO(assertum_, __LINE__)[(cond) ? 1 : -1]
#define OFF(S, C) __builtin_offsetof(struct S, C)'

# TU verum
{
    echo "#include <android/native_activity.h>"
    echo "#include <android/native_window.h>"
    echo "#include <android/rect.h>"
    echo "#include <android/looper.h>"
    echo "#include <android/input.h>"
    echo "#include <android/keycodes.h>"
    echo "#include <android/choreographer.h>"
    echo "#include <android/configuration.h>"
    echo "#include <android/log.h>"
    echo "#include <sys/system_properties.h>"
    echo "$PRAEAMBULUM"
    emittere_asserta
} > "$GEN_DIR/verum.c"

# TU nostrum (praeludium = typi quos systema_c89.h praebet)
{
    echo "typedef unsigned long size_t;"
    echo "typedef long time_t;"
    echo "typedef struct __sFILE FILE;"
    # via absoluta: nulla radix -I manu (porta vexilla)
    echo "#include \"$RADIX_DIR/silva/fontes/systema_posix.h\""
    echo "$PRAEAMBULUM"
    emittere_asserta
} > "$GEN_DIR/nostrum.c"

BENE=1
for tu in verum nostrum; do
    # clang NDK ORACULUM est hic: vexilla EXPERIMENTI, non domus (ut
    # auspex_posix)
    if "$CLANG" --target="aarch64-linux-android$NDK_API_MINIMA" -std=c99 \
        -Wall -Wextra -Werror -Wno-unused-local-typedef -c \
        "$GEN_DIR/$tu.c" -o /dev/null 2>"$GEN_DIR/$tu.error"; then
        echo "auspex_android: TU $tu CONSENTIT"
    else
        echo "auspex_android: TU $tu DISSENTIT:"
        head -20 "$GEN_DIR/$tu.error"
        BENE=0
    fi
done

if [ $BENE -eq 1 ]; then
    echo "auspex_android: FORMAE CERTIFICATAE (ARect 16 + ANativeWindow_Buffer 48 + ANativeActivity 80 + callbacks 128 + $(printf '%s\n' "$TABULA_VALORUM" | grep -c .) valores; NDK $NDK_NOMEN aarch64-android$NDK_API_MINIMA)"
    exit 0
fi
exit 1
