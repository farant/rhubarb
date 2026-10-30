#!/bin/bash
# tools/instrumenta_fumus.sh - porta 'instrumenta' (spec unitates-codicis
# par. VII semis, U5 b): OMNIS PROGRAMMA DOMESTICUS AEDIFICATUR.
#
# Mensura 2026-10-16 (bin/fabrica portae relatio): ~CIX plagulae codicis
# domestici manu scripti in NULLA porta - introitus programmatum
# (SUB/instrumenta/*.c, tools/*.c, glutinum applicationum) quos nihil
# compilat post mutationem API bibliothecae: putrescunt tacite.
#
# Plagulae per IUDICEM 'instrumenta' (unitates.stml), numquam indice:
# cura domestica, vita viva/vetusta, munus fons/scriptum, provenientia
# manu, lingua c89/thistle - vendor, knotapel, archivum, fixturae
# (adversa briar quae fallere DEBENT), generata cadunt per politicam.
#
#   C        introitus (principale/main) per recipe AEDILIS ipsum:
#            'bin/aedilis F --exitus D' (manifestum, scriptum, binarium
#            in build/instrumenta_fumus/<via>/ - basis communis numquam
#            obruitur; obiecta in thesauro PROPRIO --obiecta (communis
#            build/aedilis/obiecta numquam tangitur: obiecta provenientiae
#            ibi nata ab installatoribus deleta 'scripsit extra vestigium'
#            in fusione 2026-10-17 fecerunt)), deinde
#            'sh D/struere.sh'. NUMQUAM curritur.
#   thistle  'bin/briar -f RADIX -struere', deinde regio
#            munus="probatio" (si adest) per -probatio - auctor eam
#            curri tutam declaravit. HOME temporarium: thesaurus briar
#            (~/.rhubarb/briar) Frani numquam tangitur. bin/briar ARBORIS,
#            numquam ~/.bin/briar (ex main).
#
# NOTA (non FRACTUM): fons generatus sub build/ absens (capsula nondum
# sanata) - nominatur cum remedio; porta fabricam ipsa non sanat (sera
# fabricae intra portam sub fabrica).
#
# Gradus fusio: plagulae dynamice ex iudice, ergo porta omni mutationi
# debetur - semel ante fusionem (~XXII s calide, II-IV min post caput
# fundamenti mutatum).
#
# Usus: ./tools/instrumenta_fumus.sh
# Exitus: 0 sanum · 1 FRACTUM (nominata) · 2 iudicare nequit (bin/aedilis,
#         bin/briar aut iudex deest, nihil iudicatum)
# <porta titulus="instrumenta" gradus="fusio" tegit="tools/instrumenta_fumus.sh unitates.stml" currit="tools/aedilis.c tools/briar.c" legit="*.c *.h *.m *.stml *.thistle"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
_fractum () { echo "  FRACTUM $1"; echo "fumus instrumentorum: FRACTUM"; exit 2; }
[ -x bin/aedilis ] || _fractum "bin/aedilis deest (./tools/aedilis_struere.sh)"
T="$(mktemp -d "${TMPDIR:-/tmp}/instrumenta_fumus.XXXXXX")"
trap 'rm -rf "$T"' EXIT
B="build/instrumenta_fumus"
mkdir -p "$B" "$T/domus"
export FABRICA_THESAURUS="$RADIX/build/aedilis/obiecta"
# MEMORIA ORACULI (budget B1 E6, 2026-10-28): clang -MM fontium .m (numquam
# in thesauro) semel per cursum, non per programma - arbor intra portam
# gelata; directorium cum $T deletur (inter cursus vetus mentiretur)
MEMORIA_ORACULI="$T/memoria_oraculi"
mkdir -p "$MEMORIA_ORACULI"

git ls-files -co --exclude-standard '*.c' '*.thistle' \
    | ./tools/iudex_unitatum.sh instrumenta > "$T/iudicatae" 2> "$T/iudex_err" \
    || { sed 's/^/      /' "$T/iudex_err" | head -5; _fractum "tools/iudex_unitatum.sh instrumenta nequit"; }
grep '\.c$' "$T/iudicatae" | while IFS= read -r f; do
    grep -qE '^(principale|main) *\(' "$f" && echo "$f"
done > "$T/c"
grep '\.thistle$' "$T/iudicatae" > "$T/thistle" || true
nc=$(wc -l < "$T/c" | tr -d ' ')
nt=$(wc -l < "$T/thistle" | tr -d ' ')
[ $((nc + nt)) -gt 0 ] || _fractum "nihil iudicatum (iudex 'instrumenta' vacuus)"
if [ "$nt" -gt 0 ] && [ ! -x bin/briar ]; then
    _fractum "bin/briar arboris deest ($nt thistle) - bin/fabrica sanare bin/briar"
fi

fracta=0; notae=0; structa=0; probatae=0
while IFS= read -r f; do
    D="$B/$(echo "$f" | sed 's|/|__|g; s|\.c$||')"
    mkdir -p "$D"
    if ! bin/aedilis "$f" --exitus "$D" --obiecta "$B/obiecta" --thesaurus "$FABRICA_THESAURUS" \
            --memoria-oraculi "$MEMORIA_ORACULI" > "$D/aedilis.log" 2>&1; then
        echo "  FRACTUM $f - aedilis: $(grep -m1 -E 'RECUSAT|error' "$D/aedilis.log" | cut -c1-140)"
        fracta=$((fracta + 1)); continue
    fi
    absentes=$(grep -oE '^compilare "build/[^"]+"' "$D/struere.sh" | sed 's/^compilare "//; s/"$//' \
        | while IFS= read -r g; do [ -f "$g" ] || echo "$g"; done | tr '\n' ' ')
    if [ -n "$absentes" ]; then
        echo "  NOTA $f - fons generatus absens: ${absentes% } (bin/fabrica sanare ${absentes% })"
        notae=$((notae + 1)); continue
    fi
    if ! sh "$D/struere.sh" > "$D/log" 2>&1; then
        echo "  FRACTUM $f - $(grep -m1 -E 'error|Undefined symbols|ld:' "$D/log" | cut -c1-150)"
        grep -A2 'Undefined symbols' "$D/log" | sed -n 2,3p | sed 's/^/      /'
        fracta=$((fracta + 1)); continue
    fi
    structa=$((structa + 1))
done < "$T/c"

while IFS= read -r t; do
    [ -n "$t" ] || continue
    if ! HOME="$T/domus" bin/briar -f "$RADIX" -struere "$t" > "$T/briar.log" 2>&1; then
        echo "  FRACTUM $t - briar -struere: $(grep -m1 -E 'error|FRACT|RECUS' "$T/briar.log" | cut -c1-140)"
        fracta=$((fracta + 1)); continue
    fi
    structa=$((structa + 1))
    if grep -q 'munus="probatio"' "$t"; then
        if ! HOME="$T/domus" bin/briar -f "$RADIX" -probatio "$t" > "$T/briar.log" 2>&1; then
            echo "  FRACTUM $t - briar -probatio: $(grep -m1 -E 'FRACT|error|assert' "$T/briar.log" | cut -c1-140)"
            fracta=$((fracta + 1)); continue
        fi
        probatae=$((probatae + 1))
    fi
done < "$T/thistle"

echo "  programmata: $nc C + $nt thistle iudicata; $structa structa, $probatae regiones probationis cursae, $notae notae, $fracta fracta"
if [ "$fracta" -ne 0 ]; then
    echo "fumus instrumentorum: FRACTUM"
    exit 1
fi
echo "fumus instrumentorum: sanum"
exit 0
