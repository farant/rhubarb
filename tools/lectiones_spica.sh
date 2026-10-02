#!/bin/bash
# tools/lectiones_spica.sh - SPICA libri lectionum (fabrica plan 2 T1):
# liber aedilis contra manifestum quod idem cursus scribit.
#
# Quaestio (spec 2 par. II.4): videtne liber omnia quae manifestum
# nominat? Transitus = liber >= manifestum, et omnis lectio extra
# manifestum genere suo nominata. Lacuna una non explicata = consilium
# nondum paratum.
#
# Usus: ./tools/lectiones_spica.sh <radix.c> [...]
# Exitus: 0 nulla lacuna (manifestum totum in libro) · 1 lacuna ·
#         2 usus aut aedilis fractus.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ $# -ge 1 ] || { echo "usus: lectiones_spica.sh <radix.c> [...]" >&2; exit 2; }
[ -x bin/aedilis ] || { echo "bin/aedilis deest" >&2; exit 2; }
RADIX="$(pwd)"
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
lacunae=0

# via normata: './' et radix absoluta demptae
_normare () {
    sed -e "s|^$RADIX/||" -e 's|^\./||' -e 's|//*|/|g'
}

for fons in "$@"; do
    rm -f "$T/l.tsv" "$T/m.stml"
    if ! FABRICA_LECTIONES="$T/l.tsv" ./bin/aedilis "$fons" --enumerare \
            --manifestum "$T/m.stml" > /dev/null 2> "$T/err"; then
        echo "spica: aedilis fractus: $fons"; head -3 "$T/err"; exit 2
    fi
    # manifestum: viae obiectorum et capitum. Elementa trans lineas
    # scribuntur (via= in linea sequenti) - plagula prius complanatur.
    # Capita SYSTEMATIS (in arbore non exstantia: stdio.h, sys/wait.h)
    # a compilatore veniunt, non ab arbore - aedilis ea non legit;
    # identitas compilatoris (Q6) ea tegit. Seorsum numerantur.
    tr '\n' ' ' < "$T/m.stml" \
        | grep -oE '<(obiectum|caput)[^>]*via="[^"]+"' \
        | sed -E 's/.*via="([^"]+)".*/\1/' | _normare | sort -u > "$T/omnia"
    : > "$T/manifestum"; : > "$T/systemata"
    # regula: quod in arbore non exstat a systemate venit (etiam cum
    # '/': sys/wait.h)
    while IFS= read -r v; do
        if [ -e "$v" ]; then echo "$v" >> "$T/manifestum"
        else echo "$v" >> "$T/systemata"; fi
    done < "$T/omnia"
    # liber: L SOLUM = visa. Exitus aedilis ex CONTENTIS manifesti
    # pendet; X (exstantia sola) mutationem contentorum non videret -
    # vestigium ex X solo clavem falsam daret (planta T1: L demptum,
    # X omnia tegebat, porta viridis manebat)
    awk -F'\t' '$1=="L" { print $2 }' "$T/l.tsv" | _normare \
        | sort -u > "$T/visa"
    comm -23 "$T/manifestum" "$T/visa" > "$T/absentia"
    comm -13 "$T/manifestum" "$T/visa" > "$T/extra"
    n_m=$(wc -l < "$T/manifestum" | tr -d ' ')
    n_l=$(wc -l < "$T/l.tsv" | tr -d ' ')
    echo "== $fons: manifestum $n_m viae, liber $n_l lineae" \
         "($(cut -f1 "$T/l.tsv" | sort | uniq -c | tr -s ' ' | tr '\n' ' '))"
    if [ -s "$T/absentia" ]; then
        echo "  IN MANIFESTO, NON IN LIBRO (lacuna):"
        sed 's/^/    /' "$T/absentia"
        lacunae=$((lacunae + 1))
    else
        echo "  in manifesto, non in libro: nihil"
    fi
    # extra per genus: configuratio, capita systematis, cetera visa
    echo "  in libro, non in manifesto (visa):"
    sed 's/^/    /' "$T/extra" | head -40
    [ "$(wc -l < "$T/extra")" -gt 40 ] && echo "    ... (+$(( $(wc -l < "$T/extra") - 40 )))"
    echo "  capita systematis (nominata, a compilatore, non lecta): $(tr '\n' ' ' < "$T/systemata")"
    echo "  quaesita absentia (A): $(awk -F'\t' '$1=="A"' "$T/l.tsv" | wc -l | tr -d ' ')" \
         "· enumerata (D): $(awk -F'\t' '$1=="D"' "$T/l.tsv" | wc -l | tr -d ' ')"
done
[ "$lacunae" -eq 0 ] && { echo "spica: nulla lacuna"; exit 0; }
echo "spica: LACUNAE in $lacunae radicibus"
exit 1
