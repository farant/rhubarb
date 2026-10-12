#!/bin/bash
# credo_ordo_mixtus.sh - suitae SECTIONUM (credo v2) ordine MIXTO currunt
# (CREDO_ORDO=mixtus:<semen>, credo II T6): status communis occultus inter
# sectiones (sectio quae alteram ante se cursam supponit) ordine tabulae
# latet, ordine mixto frangit. Semen fractum NOMINATUR - cursus idem
# iteratur: CREDO_ORDO=mixtus:<semen> build/probationes/<suita>.
#
# Instrumentum, NON porta (spec credo 2, Q6): per intervalla aut post
# conversionem suitae curritur, numquam in commissione.
#
# Suitae: probationes/*.c quae credo_suitam_currere vocant (ordinarium),
# aut nominatae. Quaeque PRIMUM ordine tabulae struitur et curritur
# (./compile_tests.sh <suita>): basis rubra = nihil mixtum, nominatur.
#
# Usus (ex radice):
#   ./tools/credo_ordo_mixtus.sh [-semina N] [-initium S] [suita...]
#   -semina   semina per suitam (ordinarium V)
#   -initium  semen primum (ordinarium I; semina S..S+N-1)
#   suita     nomen plagulae sine '.c' (probatio_xar), aut via
#
# Effusio: linea per suitam ('sana N/N' | 'FRACTA semine S: sectiones
# ...' | 'basis rubra' | 'non structa'), deinde summa. Exitus 0 omnes
# sanae / 1 aliqua fracta aut basis rubra / 2 usus.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
SEMINA=5
INITIUM=1
SUITAE=()
while [ $# -gt 0 ]; do
    case "$1" in
        -semina)  SEMINA="$2"; shift 2 ;;
        -initium) INITIUM="$2"; shift 2 ;;
        -*) echo "credo_ordo_mixtus: argumentum ignotum: $1" >&2; exit 2 ;;
        *) s="${1##*/}"; SUITAE+=("${s%.c}"); shift ;;
    esac
done
case "$SEMINA$INITIUM" in *[!0-9]*) echo "credo_ordo_mixtus: -semina/-initium numeri" >&2; exit 2 ;; esac
if [ ${#SUITAE[@]} -eq 0 ]; then
    for f in probationes/*.c; do
        grep -q 'credo_suitam_currere' "$f" && { s="${f##*/}"; SUITAE+=("${s%.c}"); }
    done
fi
T="$(mktemp -d "${TMPDIR:-/tmp}/credo_ordo.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT

malae=0
for suita in "${SUITAE[@]}"; do
    bin="build/probationes/$suita"
    if ! ./compile_tests.sh "$suita" > "$T/$suita.basis" 2>&1; then
        echo "  $suita: basis rubra (ordo tabulae) - nihil mixtum"
        malae=$((malae + 1)); continue
    fi
    [ -x "$bin" ] || { echo "  $suita: non structa ($bin)"; malae=$((malae + 1)); continue; }
    fractae=""
    s=$INITIUM
    while [ "$s" -lt $((INITIUM + SEMINA)) ]; do
        if ! CREDO_ORDO="mixtus:$s" CREDO_VERDICTA="$T/v.tsv" "$bin" > "$T/o" 2>&1; then
            sect="$(awk -F'\t' '$1 == "SECTIO" && $4 != "TRANSIIT" && $4 != "NOTA_FRACTA" && $4 != "OMISSA" {printf "%s%s(%s)", sep, $3, $4; sep=", "}' "$T/v.tsv" 2>/dev/null)"
            [ -f "$T/v.tsv" ] && grep -q '^SUITA	' "$T/v.tsv" || sect="${sect:+$sect, }SUITA absens (ruit)"
            fractae="$fractae
      semine $s: ${sect:-?}"
        fi
        rm -f "$T/v.tsv"
        s=$((s + 1))
    done
    if [ -z "$fractae" ]; then
        echo "  $suita: sana $SEMINA/$SEMINA semina"
    else
        echo "  $suita: FRACTA ordine mixto (iterare: CREDO_ORDO=mixtus:<semen> $bin)$fractae"
        malae=$((malae + 1))
    fi
done
echo
echo "credo_ordo_mixtus: ${#SUITAE[@]} suitae, $SEMINA semina ($INITIUM..$((INITIUM + SEMINA - 1))), malae $malae"
[ "$malae" -eq 0 ]
