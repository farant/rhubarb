#!/bin/bash

# silva/renominare_proba.sh - porta aurea renominationis (fixa)
#
# Casus contra effusus pinnatos (-machina) et plagulas post
# scriptionem exspectatas. Curritur per ./silva/renominare.sh -proba
# (binarium iam aedificatum). Exit: 0 OMNIA | 1 fracturae.

set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"

BIN="$SILVA_DIR/build/renominare"
F="silva/probationes/fixa/renominationis"
S="$F/scaena"
fracturae=0

proba () {
    # proba <titulus> <exitus_exspectatus> <aureum> -- mandatum...
    titulus="$1"; ex_exp="$2"; aureum="$3"; shift 3
    [ "$1" = "--" ] && shift
    fructus="$("$@" 2>/dev/null)"
    ex_verus=$?
    if [ "$ex_verus" != "$ex_exp" ]; then
        echo "FRACTUM  $titulus: exitus $ex_verus != $ex_exp"
        fracturae=$((fracturae+1))
        return
    fi
    if [ -n "$aureum" ]; then
        if ! diff -q <(printf '%s\n' "$fructus") "$F/$aureum" >/dev/null 2>&1; then
            echo "FRACTUM  $titulus: effusum aureo non congruit ($aureum)"
            printf '%s\n' "$fructus" | diff - "$F/$aureum" | head -8
            fracturae=$((fracturae+1))
            return
        fi
    fi
    echo "bene     $titulus"
}

# I. statica homonyma sine -via -> refusio
proba "statica-homonyma-refusio" 1 aureum_homonyma.tsv -- \
    "$BIN" adiutor iuvans -machina \
    $F/planum_a.c $F/planum_b.c $F/planum_caput.h

# II. -via planum (discriminatio ligaminis)
proba "planum-via-staticum" 0 aureum_via.tsv -- \
    "$BIN" adiutor iuvans -machina -via $F/planum_a.c \
    $F/planum_a.c $F/planum_b.c $F/planum_caput.h

# III. entitas externa trans plagulas
proba "planum-externum" 0 aureum_externum.tsv -- \
    "$BIN" res_communis commune -machina \
    $F/planum_a.c $F/planum_b.c $F/planum_caput.h

# IV. collisio nominis novi
proba "porta-collisionis" 1 "" -- \
    "$BIN" adiutor publica -machina -via $F/planum_a.c \
    $F/planum_a.c $F/planum_b.c $F/planum_caput.h

# V. verbum clavium
proba "porta-verbi-c89" 1 "" -- \
    "$BIN" adiutor while -machina -via $F/planum_a.c $F/planum_a.c

# VI. macro definitum
proba "porta-macronis" 1 "" -- \
    "$BIN" adiutor GEMINUM -machina -via $F/planum_a.c $F/planum_a.c

# VII. symbolum ignotum
proba "symbolum-ignotum" 1 "" -- \
    "$BIN" phantasma quid -machina $F/planum_a.c

# VIII. scriptio in scaena + verificatio + plagulae exspectatae
rm -rf "$S"; mkdir -p "$S"
cp $F/planum_a.c $F/planum_b.c $F/planum_caput.h "$S/"
if "$BIN" adiutor iuvans -scribere -via $S/planum_a.c \
       $S/planum_a.c $S/planum_b.c $S/planum_caput.h \
       >/dev/null 2>&1; then
    for p in planum_a.c planum_b.c planum_caput.h; do
        if ! diff -q "$S/$p" "$F/post_scriptionem/$p" >/dev/null 2>&1; then
            echo "FRACTUM  scriptio-scaenae: $p exspectato non congruit"
            diff "$S/$p" "$F/post_scriptionem/$p" | head -6
            fracturae=$((fracturae+1))
        fi
    done
    [ "$fracturae" = 0 ] && echo "bene     scriptio-scaenae"
else
    echo "FRACTUM  scriptio-scaenae: exitus non 0"
    fracturae=$((fracturae+1))
fi
rm -rf "$S"

if [ "$fracturae" = 0 ]; then
    echo "PORTA AUREA: OMNIA BENE"
    exit 0
fi
echo "PORTA AUREA: $fracturae FRACTURAE"
exit 1
