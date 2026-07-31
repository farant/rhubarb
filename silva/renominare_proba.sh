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

# IX. localis planus (-intra): declaratio + usus IV
proba "localis-planum" 0 aureum_localis.tsv -- \
    "$BIN" cursor k -machina -intra adhibere $F/planum_locales.c

# X. umbrae sine -linea -> refusio listans lineas
proba "localis-umbrae-refusio" 1 "" -- \
    "$BIN" temp x -machina -intra umbrae $F/planum_locales.c

# XI. -linea electio (umbra prima; soror aliena manet)
proba "localis-linea" 0 aureum_linea.tsv -- \
    "$BIN" temp x -machina -intra umbrae -linea 28 \
    $F/planum_locales.c

# XII. discriminatio localis contra file-scope (copia)
proba "localis-discriminatio" 0 aureum_copia.tsv -- \
    "$BIN" copia acervus -machina -intra umbrae \
    $F/planum_locales.c

# XIII. porta capturae (novum intra functionem)
proba "localis-captura" 1 "" -- \
    "$BIN" cursor summa -machina -intra adhibere \
    $F/planum_locales.c

# XIX. unda prototyporum: parametrum definitionis + prototypi
proba "parametrum-prototypi" 0 aureum_datum.tsv -- \
    "$BIN" datum copia_datorum -machina -intra adhibere \
    $F/planum_locales.c

# XX. nomen prototypi divergens -> relatum manuale, non tactum
proba "parametrum-divergens" 0 aureum_divergens.tsv -- \
    "$BIN" alius x -machina -intra divergens $F/planum_locales.c

# XIV. scriptio localis in scaena
rm -rf "$S"; mkdir -p "$S"
cp $F/planum_locales.c "$S/"
if "$BIN" copia acervus -intra umbrae -scribere \
       $S/planum_locales.c >/dev/null 2>&1 \
   && diff -q "$S/planum_locales.c" \
          "$F/post_scriptionem/planum_locales.c" >/dev/null 2>&1; then
    echo "bene     scriptio-localis"
else
    echo "FRACTUM  scriptio-localis"
    fracturae=$((fracturae+1))
fi
rm -rf "$S"

# XV. membrum typedef-baptizatum (Saccus.pondus; catena inclusa)
proba "membrum-saccus" 0 aureum_saccus.tsv -- \
    "$BIN" pondus onus -machina -membrum Saccus \
    $F/planum_membra.c

# XVI. membrum tag-nominatum (cista.pondus)
proba "membrum-cista" 0 aureum_cista.tsv -- \
    "$BIN" pondus onus -machina -membrum cista \
    $F/planum_membra.c

# XVII. collisio membri (Saccus iam mensuram habet)
proba "membrum-collisio" 1 "" -- \
    "$BIN" pondus mensura -machina -membrum Saccus \
    $F/planum_membra.c

# XVIII. scriptio membri in scaena
rm -rf "$S"; mkdir -p "$S"
cp $F/planum_membra.c "$S/"
if "$BIN" pondus onus -membrum Saccus -scribere \
       $S/planum_membra.c >/dev/null 2>&1 \
   && diff -q "$S/planum_membra.c" \
          "$F/post_scriptionem/planum_membra.c" >/dev/null 2>&1; then
    echo "bene     scriptio-membri"
else
    echo "FRACTUM  scriptio-membri"
    fracturae=$((fracturae+1))
fi
rm -rf "$S"

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
