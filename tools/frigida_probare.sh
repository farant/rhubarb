#!/bin/bash
# frigida_probare.sh - omnes suitae in arbore FRIGIDA (clone novus)
#
# Quaestio …QY4 (2026-09-28): suita quae initum gitignoratum legit
# (build/, data/, fixa non commissa) in hac machina viridis est, in
# clone quavis rubra - "viride" tunc "viride in hoc disco" significat.
# Hoc instrumentum arborem operis novam ex COMMISSO facit (git worktree
# add, ramus temporarius), omnes suitas sine aedificatione praevia
# currit, rubras nominat, arborem delet.
#
# Mensurat COMMISSUM, non arborem operis: mutationes non commissae non
# videntur. Tardum (~12 min) - instrumentum ante fusionem aut per
# intervalla, non porta commissionis (Fran, 2026-09-28).
#
# Suitae: radix (./compile_tests.sh) + omne <sub>/compile_probationes.sh
# (per globum - suitae novae sponte intrant).
#
# Usus (ex radice):
#   ./tools/frigida_probare.sh [-ref <commissum>] [-suitae "radix toml"]
#                              [-servare]
#   -ref      commissum mensurandum (ordinarium HEAD)
#   -suitae   subset nominatum (ordinarium: omnes)
#   -servare  arborem et acta non delere (ad inspiciendum)
#
# Exitus: 0 omnes virides / 1 aliqua rubra (nominata) / 2 praeparatio
# fracta.

RADIX="$(cd "$(dirname "$0")/.." && pwd)"
REF=HEAD
SUITAE=""
SERVARE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -ref)     REF="$2"; shift 2 ;;
        -suitae)  SUITAE="$2"; shift 2 ;;
        -servare) SERVARE=1; shift ;;
        *) echo "frigida: argumentum ignotum: $1" >&2; exit 2 ;;
    esac
done

cd "$RADIX" || exit 2
SHA="$(git rev-parse --verify "$REF^{commit}" 2>/dev/null)" \
    || { echo "frigida: commissum ignotum: $REF" >&2; exit 2; }
BREVE="${SHA:0:8}"
AREA="$(mktemp -d "${TMPDIR:-/tmp}/frigida.XXXXXX")" || exit 2
ARBOR="$AREA/arbor"
ACTA="$AREA/acta"
RAMUS="frigida-$BREVE-$$"
mkdir -p "$ACTA"

purgare () {
    if [ "$SERVARE" = 1 ]; then
        echo "frigida: servatum - arbor $ARBOR, acta $ACTA, ramus $RAMUS"
        return
    fi
    git -C "$RADIX" worktree remove --force "$ARBOR" >/dev/null 2>&1
    git -C "$RADIX" branch -D "$RAMUS" >/dev/null 2>&1
    rm -rf "$AREA"
}

git worktree add -q -b "$RAMUS" "$ARBOR" "$SHA" >/dev/null 2>&1 \
    || { echo "frigida: git worktree add defecit" >&2; rm -rf "$AREA"; exit 2; }

if [ -z "$SUITAE" ]; then
    SUITAE="radix"
    for c in "$ARBOR"/*/compile_probationes.sh; do
        SUITAE="$SUITAE $(basename "$(dirname "$c")")"
    done
fi

echo "frigida: commissum $BREVE, arbor $ARBOR"
RUBRAE=0
for s in $SUITAE; do
    t0=$(date +%s)
    if [ "$s" = radix ]; then
        ( cd "$ARBOR" && ./compile_tests.sh ) > "$ACTA/$s.log" 2>&1
    elif [ -x "$ARBOR/$s/compile_probationes.sh" ]; then
        ( cd "$ARBOR" && "./$s/compile_probationes.sh" ) > "$ACTA/$s.log" 2>&1
    else
        echo "  $s: SUITA IGNOTA"; RUBRAE=$((RUBRAE + 1)); continue
    fi
    rc=$?
    dt=$(( $(date +%s) - t0 ))
    if [ "$rc" = 0 ]; then
        printf "  %-12s viridis (%ds)\n" "$s" "$dt"
    else
        RUBRAE=$((RUBRAE + 1))
        # nomina rubrarum: radix 'Failed: a b', subsystemata 'FRACTAE: a b'
        nomina="$(LC_ALL=C grep -a -h -E '^(.\[31m)?Failed: |^FRACTAE: ' "$ACTA/$s.log" \
            | tail -1 | sed -E 's/\x1b\[[0-9;]*m//g; s/^(Failed|FRACTAE): //')"
        printf "  %-12s RUBRA rc=%s (%ds): %s\n" "$s" "$rc" "$dt" "${nomina:-(vide acta)}"
    fi
done

if [ "$RUBRAE" = 0 ]; then
    echo "frigida: omnes virides ($BREVE)"
    purgare
    exit 0
fi
echo "frigida: $RUBRAE suitae rubrae ($BREVE) - acta: $ACTA"
SERVARE=1
purgare
exit 1
