#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/android.log (ut
# tools/linux/probatio.sh); ante 'cd' infra, ne "$0" relativum pereat.
if [ -z "${ANDROID_PROBATIO_EFFUSIO:-}" ]; then
    export ANDROID_PROBATIO_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../../build/test_logs"
    # tee signa gregis (Ctrl-C) ignorat: alioquin moritur, et nuntius
    # purgationis in fd 3 SIGPIPE cursum interiorem necat (exitus 141,
    # nuntius perditus - recensio 2026-10-23, in tabula probatum)
    "$0" "$@" 2>&1 | (trap '' INT TERM; tee "$(dirname "$0")/../../build/test_logs/android.log")
    exit "${PIPESTATUS[0]}"
fi

# tools/android/probatio.sh - probationes credo in tabula Android
# (android-gradus II.4-II.5)
#
# Usus: ./tools/android/probatio.sh [-servare | -purgare] <nomina...>
#       e.g. ./tools/android/probatio.sh piscina chorda xar json volumen
#
# Clausura per 'bin/aedilis P --varians android,posix --partes'; NDK
# clang (tools/android/ndk_fixum.sh) cum VEXILLA_C89 (tools/vexilla.sh);
# binarium nudatum in tabulam, ibi curritur.
#
# TABULA: radix UNA /data/local/tmp/rhubarb - extra eam nihil scribitur
# aut deletur. Initio deletur et renascitur (tmp/, build/); suitae cum
# TMPDIR=<radix>/tmp, HOME=<radix> (identitas machinae: .rhubarb/machina,
# machina_posix.c) et cwd=<radix> currunt; FINE (etiam Ctrl-C) mensura
# nuntiatur et radix deletur. -servare: radix manet (via nuntiatur).
# -purgare: radicem delet, nihil currit.
#
# Exitus: 0 omnes praeteritae | 1 fractae (compilatio et fons .m
# numerantur) | 2 apparatus (NDK, adb, tabula, aedilis)
set -u
# effusio suitarum UTF-8 et octetos quoslibet fert: tr/awk/grep macOS sub
# locale UTF-8 "Illegal byte sequence" clamant et lineam EXITUS perdunt
export LC_ALL=C
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

RADIX_TABULAE=/data/local/tmp/rhubarb
# stdout verum servatum (fd 3): trap EXIT intra mandatum redirectum
# (effusio suitae > $D/effusio.txt) currere potest - nuntius purgationis
# alioquin in effusionem suitae caderet (inventum 2026-10-23, TERM)
exec 3>&1
ADB="${ADB:-$HOME/Library/Android/sdk/platform-tools/adb}"
SERVARE=0
PURGARE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -servare) SERVARE=1; shift ;;
        -purgare) PURGARE=1; shift ;;
        *) break ;;
    esac
done

apparatus () {
    echo "android/probatio.sh: $*" >&2
    exit 2
}

# --- tabula: exacte una, auctorizata -------------------------------
[ -x "$ADB" ] || apparatus "adb deest ($ADB)"
ADB_TABULAE=$("$ADB" devices | awk 'NR > 1 && NF >= 2 {print $1 " " $2}')
if [ -n "${ANDROID_SERIAL:-}" ]; then
    STATUS=$(printf '%s\n' "$ADB_TABULAE" | awk -v s="$ANDROID_SERIAL" '$1 == s {print $2}')
    [ -n "$STATUS" ] || apparatus "tabula $ANDROID_SERIAL non connexa"
else
    NUMERUS=$(printf '%s\n' "$ADB_TABULAE" | grep -c .)
    [ "$NUMERUS" -ge 1 ] || apparatus "nulla tabula connexa (adb devices vacuum)"
    [ "$NUMERUS" -eq 1 ] || apparatus "plures tabulae ($NUMERUS) - ANDROID_SERIAL elige"
    ANDROID_SERIAL=$(printf '%s\n' "$ADB_TABULAE" | awk '{print $1}')
    STATUS=$(printf '%s\n' "$ADB_TABULAE" | awk '{print $2}')
fi
[ "$STATUS" = "device" ] || apparatus "tabula $ANDROID_SERIAL status '$STATUS' - 'Allow USB debugging?' in tabula accipe"
export ANDROID_SERIAL

A () { "$ADB" "$@"; }

# "purgatum" SOLUM si tabula deletionem confirmat (signum PURGATUM ab
# ipsa tabula redditum): tabula amissa (cablum, adb) nuntiatur, numquam
# purgatio ficta (recensio I1, 2026-10-23). FALSUM si non confirmata.
purgare () {
    local kib confirmatio
    kib=$(A shell "du -sk $RADIX_TABULAE 2>/dev/null" 2>/dev/null | tr -d '\r' | awk '{print $1}')
    confirmatio=$(A shell "rm -rf $RADIX_TABULAE && echo PURGATUM" 2>/dev/null | tr -d '\r')
    if [ "$confirmatio" != "PURGATUM" ]; then
        echo "PURGATIO FRACTA: tabula deletionem $RADIX_TABULAE non confirmavit - radix fortasse manet; tabula reconnexa: ./tools/android/probatio.sh -purgare" >&3
        return 1
    fi
    echo "vestigium in tabula: ${kib:-0} KiB, purgatum" >&3
    return 0
}

# --- sera tabulae (recensio 2026-10-23): cursus plures (arbores operis
# parallelae) eandem tabulam tangunt - initium alterius radicem alterius
# in media suita deleret (rubrum fictum). Sera in HOSPITE per tabulam,
# omnibus arboribus communis (~/.rhubarb), mkdir atomicum + pid; in
# tabula nihil extra radicem scribitur (spec II.5).
SERA_DIR="$HOME/.rhubarb/android"
SERA="$SERA_DIR/$ANDROID_SERIAL.sera"
SERA_NOSTRA=0
RADIX_NOSTRA=0
sera_capere () {
    local alius
    mkdir -p "$SERA_DIR" || apparatus "directorium serae $SERA_DIR creari nequit"
    if ! mkdir "$SERA" 2>/dev/null; then
        alius=$(cat "$SERA/pid" 2>/dev/null)
        if [ -z "$alius" ] || kill -0 "$alius" 2>/dev/null; then
            apparatus "tabula $ANDROID_SERIAL occupata (cursus pid ${alius:-ignotus}; sera $SERA) - exspecta; si nullus cursus vivit: rm -rf $SERA"
        fi
        # possessor mortuus: sera stala, capitur
        { rm -rf "$SERA" && mkdir "$SERA" 2>/dev/null; } \
            || apparatus "sera $SERA capi nequit"
    fi
    echo $$ > "$SERA/pid"
    SERA_NOSTRA=1
}

# finis UNUS omnes exitus regit: radicem SUAM solum purgat, seram SUAM
# solum liberat; purgatio fracta -> exitus 2 (contractus 0/1/2/130)
finis () {
    local status=$?
    if [ "$RADIX_NOSTRA" = 1 ]; then
        if [ "$SERVARE" = 1 ]; then
            echo "radix servata: $RADIX_TABULAE (./tools/android/probatio.sh -purgare)" >&3
        elif ! purgare && [ "$status" -lt 2 ]; then
            status=2
        fi
    fi
    if [ "$SERA_NOSTRA" = 1 ]; then
        rm -rf "$SERA"
    fi
    exit "$status"
}

sera_capere
trap finis EXIT
trap 'exit 130' INT TERM

if [ "$PURGARE" = 1 ]; then
    purgare || exit 2
    exit 0
fi
[ $# -ge 1 ] || apparatus "usus: $0 [-servare | -purgare] <nomina probationum...>"

# --- NDK fixum -------------------------------------------------------
. tools/android/ndk_fixum.sh
grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null \
    || apparatus "NDK $NDK_VERSIO deest aut alienum ($NDK_SEDES) - ./tools/android/ndk_instituere.sh"
NDK_BIN="$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin"

ABI=$(A shell getprop ro.product.cpu.abi | tr -d '\r')
API=$(A shell getprop ro.build.version.sdk | tr -d '\r')
case "$ABI" in
    arm64-v8a) TRIPLEX="aarch64-linux-android$NDK_API_MINIMA" ;;
    armeabi-v7a) TRIPLEX="armv7a-linux-androideabi$NDK_API_MINIMA" ;;
    *) apparatus "ABI ignota: '$ABI'" ;;
esac
[ "$API" -ge "$NDK_API_MINIMA" ] 2>/dev/null \
    || apparatus "API tabulae '$API' < $NDK_API_MINIMA (fixum)"

. tools/vexilla.sh
vexilla_radices ""
CC=("$NDK_BIN/clang" "--target=$TRIPLEX")
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}")
VFLAGS=(-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- radix tabulae: initio munda, fine semper deleta (finis supra) -----
A shell "rm -rf $RADIX_TABULAE && mkdir -p $RADIX_TABULAE/tmp $RADIX_TABULAE/build" \
    || apparatus "radix $RADIX_TABULAE creari nequit"
RADIX_NOSTRA=1

echo "tabula $ANDROID_SERIAL: $ABI, API $API; NDK $NDK_NOMEN, $TRIPLEX"
FRACTAE=""
for titulus in "$@"; do
    PROBATIO="probationes/probatio_${titulus}.c"
    [ -f "$PROBATIO" ] || apparatus "$PROBATIO deest"
    PARTES=$(bin/aedilis "$PROBATIO" --varians android,posix --partes 2>&1) \
        || apparatus "aedilis fracta ($titulus): $PARTES"
    OBIECTA=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "O" {print $2}')
    VENDITI=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "V" {print $2}')
    echo "== $titulus =="
    if printf '%s\n' "$OBIECTA" | grep -q '\.m$'; then
        echo "FRACTA: fons .m in clausura (codex Mac solum): $(printf '%s\n' "$OBIECTA" | grep '\.m$' | tr '\n' ' ')"
        FRACTAE="$FRACTAE $titulus"
        continue
    fi
    D="build/android/$ABI/$titulus"
    rm -rf "$D" && mkdir -p "$D"
    VOBJ=()
    for v in $VENDITI; do
        o="$D/v_$(basename "$v" .c).o"
        "${CC[@]}" "${VFLAGS[@]}" -c "$v" -o "$o" > "$D/venditum.log" 2>&1 \
            || { echo "FRACTA: venditum $v"; tail -5 "$D/venditum.log"; FRACTAE="$FRACTAE $titulus"; continue 2; }
        VOBJ+=("$o")
    done
    # OBIECTA: viae sine spatiis (aedilis), ergo verba nuda
    "${CC[@]}" "${VEXILLA[@]}" "$PROBATIO" $OBIECTA ${VOBJ[@]+"${VOBJ[@]}"} -lm \
        -o "$D/probatio_$titulus" > "$D/compilatio.log" 2>&1 \
        || { echo "FRACTA: COMPILATIO"; tail -15 "$D/compilatio.log"; FRACTAE="$FRACTAE $titulus"; continue; }
    "$NDK_BIN/llvm-strip" -o "$D/probatio_$titulus.nudum" "$D/probatio_$titulus" \
        || apparatus "llvm-strip fractum ($titulus)"
    A push "$D/probatio_$titulus.nudum" "$RADIX_TABULAE/probatio_$titulus" > /dev/null \
        || apparatus "adb push fractum ($titulus)"
    A shell "cd $RADIX_TABULAE && chmod 755 probatio_$titulus && HOME=$RADIX_TABULAE TMPDIR=$RADIX_TABULAE/tmp ./probatio_$titulus; echo EXITUS=\$?" \
        > "$D/effusio.txt" 2>&1
    tr -d '\r' < "$D/effusio.txt" | grep -v '^EXITUS=' | tail -6
    EXITUS=$(tr -d '\r' < "$D/effusio.txt" | awk -F= '/^EXITUS=/ {print $2}' | tail -1)
    # linea EXITUS absens = tabula amissa (cablum, adb), non suita fracta
    [ -n "$EXITUS" ] || apparatus "tabula amissa dum $titulus currit (linea EXITUS nulla) - cablum et 'adb devices' inspice"
    [ "$EXITUS" = "0" ] || FRACTAE="$FRACTAE $titulus"
done

if [ -z "$FRACTAE" ]; then
    echo "PROBATIONES ANDROID ($ABI, API $API): PRAETERITAE ($#)"
    exit 0
fi
echo "PROBATIONES ANDROID ($ABI, API $API): FRACTAE:$FRACTAE" >&2
exit 1
