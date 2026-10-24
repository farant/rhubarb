# tools/android/communia.sh - communia scriptorum Android (source, non
# exec): adb, tabula UNA auctorizata, sera hospitis per tabulam
# (~/.rhubarb/android/<serial>.sera, omnibus arboribus communis).
# Vocans 'apparatus' (exitus 2 cum nuntio) ANTE 'source' definit.
# Ex tools/android/probatio.sh extractum (testa T3) - mores idem.

ADB="${ADB:-$HOME/Library/Android/sdk/platform-tools/adb}"

A () { "$ADB" "$@"; }

# ANDROID_SERIAL ponit et exportat; aliter apparatus
android_tabulam_eligere () {
    local tabulae status numerus
    [ -x "$ADB" ] || apparatus "adb deest ($ADB)"
    tabulae=$("$ADB" devices | awk 'NR > 1 && NF >= 2 {print $1 " " $2}')
    if [ -n "${ANDROID_SERIAL:-}" ]; then
        status=$(printf '%s\n' "$tabulae" | awk -v s="$ANDROID_SERIAL" '$1 == s {print $2}')
        [ -n "$status" ] || apparatus "tabula $ANDROID_SERIAL non connexa"
    else
        numerus=$(printf '%s\n' "$tabulae" | grep -c .)
        [ "$numerus" -ge 1 ] || apparatus "nulla tabula connexa (adb devices vacuum)"
        [ "$numerus" -eq 1 ] || apparatus "plures tabulae ($numerus) - ANDROID_SERIAL elige"
        ANDROID_SERIAL=$(printf '%s\n' "$tabulae" | awk '{print $1}')
        status=$(printf '%s\n' "$tabulae" | awk '{print $2}')
    fi
    [ "$status" = "device" ] || apparatus "tabula $ANDROID_SERIAL status '$status' - 'Allow USB debugging?' in tabula accipe"
    export ANDROID_SERIAL
}

# sera (recensio 2026-10-23): cursus plures eandem tabulam non tangunt;
# mkdir atomicum + pid; possessor mortuus = sera stala, capitur
ANDROID_SERA_NOSTRA=0
android_seram_capere () {
    local alius
    ANDROID_SERA_DIR="$HOME/.rhubarb/android"
    ANDROID_SERA="$ANDROID_SERA_DIR/$ANDROID_SERIAL.sera"
    mkdir -p "$ANDROID_SERA_DIR" || apparatus "directorium serae $ANDROID_SERA_DIR creari nequit"
    if ! mkdir "$ANDROID_SERA" 2>/dev/null; then
        alius=$(cat "$ANDROID_SERA/pid" 2>/dev/null)
        if [ -z "$alius" ] || kill -0 "$alius" 2>/dev/null; then
            apparatus "tabula $ANDROID_SERIAL occupata (cursus pid ${alius:-ignotus}; sera $ANDROID_SERA) - exspecta; si nullus cursus vivit: rm -rf $ANDROID_SERA"
        fi
        { rm -rf "$ANDROID_SERA" && mkdir "$ANDROID_SERA" 2>/dev/null; } \
            || apparatus "sera $ANDROID_SERA capi nequit"
    fi
    echo $$ > "$ANDROID_SERA/pid"
    ANDROID_SERA_NOSTRA=1
}

android_seram_liberare () {
    if [ "$ANDROID_SERA_NOSTRA" = 1 ]; then
        rm -rf "$ANDROID_SERA"
        ANDROID_SERA_NOSTRA=0
    fi
}
