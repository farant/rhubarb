#!/bin/bash
# silva/m3_probare.sh - PORTAE M3: acceptio phasis V (silva -> materia)
#
# Usage:
#   ./silva/m3_probare.sh
#
# Contractus exitus: 0 omnes portae ad pignus · 1 porta infra pignus,
# fixa extra pignus, aut probatio fracta · 2 NIHIL CURSUM (scriptum
# absens, linea numeri non inventa - numquam ut ZEPHYRUM lecta).
#
# CUR (project-specs/silva-migratio-plan.md, Task 1): materia-spec §7
# sex portas nominat quas silva PER MATERIAM transire debet antequam
# congelatio finiatur. Numeri 2026-08 erant; hic cursor eos HODIE
# metitur ex scriptis ipsis portarum (logica earum non exscripta) et
# contra pignora infra confert. Mensura 2026-09-24:
#
#   porta        fons                                        2026-08  hodie
#   subarbores   probatio_silva_arbor_circuitus (M1, A et B) 281      492*
#   planae       probatio_silva_arbor_plagula (M2, planum)   78       78
#   latinae      probatio_silva_arbor_plagula (lib/*.c)      154      182
#   hospes       instrumenta/principalia/hospes.c + amalgama 39       40
#   adversarii   probatio_silva_arbor_parsura (T7b)          24       24
#   haruspex     silva/haruspex.sh (TU contra clang)         243      279
#
#   latinae 182 -> 181 (2026-09-29): lib/toml.c deletum in toml Q12
#     (74642c4f) - cliens toml/fontes eum substituit; porta tunc non
#     debita erat, ergo contractio hic primum visa.
#   * 2026-09-25 (silva-migratio T10d): M1 subarbores OMNES generum
#     probandorum confert, non IV per genus - shim (nodi supremi
#     omnes) recessit. 281 -> 492, corpus idem, utroque oraculo.
#
# DUO GENERA PIGNORIS.
#   fixum = corpus congelatum (fixa roundtrip, casus in codice): numerus
#           EXACTUS - infra aut supra pignus rubet; motus pignoris
#           commissio est quae causam dicit.
#   vivum = corpus quod cum arbore crescit (lib/*.c, capita praebita):
#           omnes transire debent ET numerus >= pignus (limen contra
#           corpus tacite contractum - glob fractus). Supra pignus =
#           nota, non fractura: bibliotheca nova portam non frangit.
#
# HOSPES ex amalgamate COMMISSO struitur, in build/m3/ - NUMQUAM per
# amalgamare.sh, quod silva/amalgama/silva.c REGENERAT (artefactum
# commissum). Obiecta hic non in silva/build: glob oraculi (struere)
# obiectum alienum ibi nectere conaretur.

set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="$RADIX/build/m3"
mkdir -p "$BUILD"
source "$RADIX/tools/vexilla.sh"
source "$RADIX/tools/sera.sh"
sera_capere "$BUILD/cursor.sera" || exit 2

# ---- PIGNORA: titulus genus pignus ----
PIGNORA="
subarbores fixum 492
planae     fixum 78
latinae    vivum 181
hospes     fixum 40
adversarii fixum 24
haruspex   vivum 279
"

T0=$(date +%s)
FRACTA=""        # causae exitus I
DEEST=""         # causae exitus II

# ---- 1. probationes silvae: una vocatio, filtrum 'arbor_' ----
# (circuitus, plagula, parsura parallelae; sedes quoque currit)
EFFUSIO_SILVAE="$BUILD/silva.effusio"
"$RADIX/silva/compile_probationes.sh" arbor_ > "$EFFUSIO_SILVAE" 2>&1
RC_SILVAE=$?

sectio () {   # <nomen probationis> -> lineae sectionis eius
    awk -v n="=== $1 ===" '
        $0 == n { intus = 1; next }
        /^=== / { intus = 0 }
        intus' "$EFFUSIO_SILVAE"
}

for p in arbor_circuitus arbor_plagula arbor_parsura; do
    if ! grep -q "^--- probatio_silva_$p praeteriit" "$EFFUSIO_SILVAE"; then
        if grep -q "^=== probatio_silva_$p ===" "$EFFUSIO_SILVAE"; then
            FRACTA="$FRACTA
  probatio_silva_$p fracta (vide $EFFUSIO_SILVAE)"
        else
            DEEST="$DEEST
  probatio_silva_$p non cucurrit (exitus cursoris $RC_SILVAE)"
        fi
    fi
done

# ---- 2. hospes ex amalgamate commisso ----
AMALGAMA="$RADIX/silva/amalgama/silva.c"
if [ ! -f "$AMALGAMA" ]; then
    DEEST="$DEEST
  $AMALGAMA abest"
    EFFUSIO_HOSPITIS=""
else
    # effusio PURGATA ante omnia: aliter '40/40' cursus prioris
    # superstes legeretur si amalgama in cache stat
    : > "$BUILD/hospes.effusio"
    if [ ! -f "$BUILD/amalgama.o" ] || ! [ "$BUILD/amalgama.o" -nt "$AMALGAMA" ] \
       || ! [ "$BUILD/amalgama.o" -nt "$RADIX/silva/amalgama/silva.h" ]; then
        clang "${VEXILLA_C89[@]}" -c "$AMALGAMA" -o "$BUILD/amalgama.o" \
            >> "$BUILD/hospes.effusio" 2>&1
    fi
    clang "${VEXILLA_C89[@]}" -I"$RADIX/silva/amalgama" \
        "$RADIX/silva/instrumenta/principalia/hospes.c" "$BUILD/amalgama.o" \
        -o "$BUILD/hospes" >> "$BUILD/hospes.effusio" 2>&1 \
      && "$BUILD/hospes" >> "$BUILD/hospes.effusio" 2>&1
    RC_HOSPITIS=$?
    EFFUSIO_HOSPITIS="$BUILD/hospes.effusio"
    [ "$RC_HOSPITIS" -eq 0 ] || FRACTA="$FRACTA
  hospes fractus (exitus $RC_HOSPITIS, vide $EFFUSIO_HOSPITIS)"
fi

# ---- 3. haruspex ----
EFFUSIO_HARUSPICIS="$BUILD/haruspex.effusio"
"$RADIX/silva/haruspex.sh" > "$EFFUSIO_HARUSPICIS" 2>&1
RC_HARUSPICIS=$?
[ "$RC_HARUSPICIS" -eq 0 ] || FRACTA="$FRACTA
  haruspex fractus (exitus $RC_HARUSPICIS, vide $EFFUSIO_HARUSPICIS)"

# ---- 4. numeri ex lineis ipsis ----
# Quaeque functio 'transeuntes totum' imprimit, aut nihil si linea abest.
num_subarbores () {
    sectio probatio_silva_arbor_circuitus | awk '
        /oraculum A \(arbor\):/  { a = $4; t = $6 }
        /oraculum B \(octeti\):/ { b = $4; u = $6 }
        END { if (a != "" && b != "" && t == u)
                  print ((a < b) ? a : b), t }'
}
num_plagulae () {   # <planum|latinum>
    sectio probatio_silva_arbor_plagula | awk -v quod="$1" '
        /^--- PORTA M2/            { gradus = "planum" }
        /^--- GRADUS LATINIZATUS/  { gradus = "latinum" }
        /DIRECTA exacta:/  && gradus == quod { d = $3; dt = $5 }
        /OCTETIM EXACTAE:/ && gradus == quod { o = $3; ot = $5 }
        END { if (d != "" && o != "" && dt == ot)
                  print ((d < o) ? d : o), ot }'
}
num_adversarii () {
    sectio probatio_silva_arbor_parsura | awk '
        /discrepantiae ab expectatione:/ { d = $4; t = $6 }
        END { if (t != "") print t - d, t }'
}
num_hospes () {
    [ -n "$EFFUSIO_HOSPITIS" ] || return 0
    sed -n 's|^hospes: \([0-9]*\)/\([0-9]*\) fideles.*|\1 \2|p' "$EFFUSIO_HOSPITIS"
}
num_haruspex () {
    sed -n 's|^haruspex: \([0-9]*\) TU compilatae, \([0-9]*\) dissentientes|\1 \2|p' \
        "$EFFUSIO_HARUSPICIS" | awk '{ print $1 - $2, $1 }'
}

# ---- 5. iudicium ----
BONAE=0
NOTAE=""
while read -r titulus genus pignus; do
    [ -n "$titulus" ] || continue
    case "$titulus" in
        subarbores) valor="$(num_subarbores)" ;;
        planae)     valor="$(num_plagulae planum)" ;;
        latinae)    valor="$(num_plagulae latinum)" ;;
        hospes)     valor="$(num_hospes)" ;;
        adversarii) valor="$(num_adversarii)" ;;
        haruspex)   valor="$(num_haruspex)" ;;
    esac
    if [ -z "$valor" ]; then
        printf 'm3 %-11s ?     (linea numeri non inventa)\n' "$titulus"
        DEEST="$DEEST
  $titulus: linea numeri non inventa"
        continue
    fi
    read -r transeuntes totum <<< "$valor"
    printf 'm3 %-11s %s/%s\n' "$titulus" "$transeuntes" "$totum"
    if [ "$transeuntes" -ne "$totum" ]; then
        FRACTA="$FRACTA
  $titulus: $transeuntes/$totum"
    elif [ "$genus" = fixum ] && [ "$totum" -ne "$pignus" ]; then
        FRACTA="$FRACTA
  $titulus: $totum != pignus fixum $pignus (corpus mutatum - pignus manu movendum cum causa)"
    elif [ "$genus" = vivum ] && [ "$totum" -lt "$pignus" ]; then
        FRACTA="$FRACTA
  $titulus: $totum < limen vivum $pignus (corpus contractum?)"
    else
        BONAE=$((BONAE + 1))
        [ "$genus" = vivum ] && [ "$totum" -gt "$pignus" ] && NOTAE="$NOTAE
  $titulus: $totum > limen $pignus (corpus crevit; limen movere licet)"
    fi
done <<< "$PIGNORA"

echo "murus: $(( $(date +%s) - T0 ))s"
[ -n "$NOTAE" ] && echo "NOTAE:$NOTAE"
if [ -n "$DEEST" ]; then
    echo "DEEST:$DEEST"
    [ -n "$FRACTA" ] && echo "FRACTA:$FRACTA"
    echo "VERDICTUM: M3 NIHIL CURSUM ($BONAE/6)"
    exit 2
fi
if [ -n "$FRACTA" ]; then
    echo "FRACTA:$FRACTA"
    echo "VERDICTUM: M3 $BONAE/6"
    exit 1
fi
echo "VERDICTUM: M3 6/6"
exit 0
