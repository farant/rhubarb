#!/bin/bash
# tools/reusus_gradus_ab.sh - A/B REUSUS (fabrica-6 T9): actio gradus
# (membra, per probationem) contra portam cursoris (verdictum totum)
# super commissiones EASDEM, per tools/reusus_retro.sh (vestigia
# hodierna in build/fabrica.db; membra cum -fons: clausura aedilis +
# aedilis.stml).
#
#   ./tools/reusus_gradus_ab.sh GRADUS PORTA DIRECTORIUM [-n N]
#   e.g. ./tools/reusus_gradus_ab.sh probationes_toml porta_toml \
#            toml/probationes -n 150
#   PORTA '-' (fabrica-7 T6): cursor sine actione declarata (radix:
#   compile_tests.sh per silva.porta) - verdictum totum reusum solum si
#   OMNIA membra reusa et plagulae cursoris (-cursor 'a b ...')
#   immutatae; tempus = mediana portae in build/portae/tempora.tsv
#   (-porta-titulus, e.g. radix)
#
# Mensurae: commissiones quibus porta reusa esset; quibus compositum
# gradus totum reusum esset; membra reusa (membrum x commissio);
# probationes cursae (porta irrita = omnes M); tempus ex cursu ultimo
# SANATO (build/fabrica.db cursus: porta tota; membrum = cursus solus -
# compilatio per thesaurum utrimque, non numerata). Instrumentum, non
# porta. Exitus: 0 relatio · 2 usus / memoria absens.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
GRADUS="${1:-}"; PORTA="${2:-}"; DIR="${3:-}"
[ -n "$GRADUS" ] && [ -n "$PORTA" ] && [ -n "$DIR" ] || { sed -n '7,9p' "$0" >&2; exit 2; }
shift 3
N=150; CURSOR=""; PORTA_TITULUS=""
while [ $# -gt 0 ]; do
    case "$1" in
        -n) N="$2"; shift 2 ;;
        -cursor) CURSOR="$2"; shift 2 ;;
        -porta-titulus) PORTA_TITULUS="$2"; shift 2 ;;
        *) echo "reusus_gradus_ab: optio ignota $1" >&2; exit 2 ;;
    esac
done
[ -f build/fabrica.db ] || { echo "reusus_gradus_ab: build/fabrica.db deest" >&2; exit 2; }
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT

_ms () {   # duratio cursus ultimi SANATI tituli
    sqlite3 build/fabrica.db "SELECT duratio_ms FROM cursus WHERE titulus = '$1'
        AND eventus = 'SANATUM' ORDER BY initium DESC LIMIT 1"
}

if [ "$PORTA" != "-" ]; then
    ./tools/reusus_retro.sh "$PORTA" -n "$N" > "$T/porta.txt" || exit 2
    sed -nE 's/^([0-9a-f]+)  (REUSUS|IRRITUM).*/\1\t\2/p' "$T/porta.txt" > "$T/porta.tsv"
    printf '%s\t%s\n' "$PORTA" "$(_ms "$PORTA")" > "$T/tempora.tsv"
else
    [ -n "$PORTA_TITULUS" ] || { echo "reusus_gradus_ab: PORTA '-' postulat -porta-titulus" >&2; exit 2; }
    PORTA="cursor_$PORTA_TITULUS"
    ms=$(awk -F'\t' -v t="$PORTA_TITULUS" '$2 == "porta" && $3 == t {print $4}' build/portae/tempora.tsv |
        sort -n | awk '{a[NR] = $1} END {if (NR) printf "%d", a[int((NR + 1) / 2)] * 1000}')
    [ -n "$ms" ] || { echo "reusus_gradus_ab: tempus portae $PORTA_TITULUS nullum in build/portae/tempora.tsv" >&2; exit 2; }
    printf '%s\t%s\n' "$PORTA" "$ms" > "$T/tempora.tsv"
    # commissiones quae plagulam cursoris tangunt (porta.tsv post membra)
    : > "$T/cursor_tacta"
    for f in $CURSOR; do
        git log --first-parent -n "$N" --format='%h' -- "$f" >> "$T/cursor_tacta"
    done
fi

: > "$T/membra.tsv"
sqlite3 build/fabrica.db "SELECT DISTINCT titulus FROM lectiones
    WHERE titulus LIKE '$GRADUS/%' ORDER BY titulus" > "$T/nomina"
[ -s "$T/nomina" ] || { echo "reusus_gradus_ab: nulla vestigia membrorum $GRADUS/" >&2; exit 2; }
while read -r m; do
    fons="$DIR/${m#"$GRADUS"/}.c"
    [ -f "$fons" ] || { echo "reusus_gradus_ab: fons membri deest: $fons" >&2; exit 2; }
    ./tools/reusus_retro.sh "$m" -fons "$fons" -n "$N" > "$T/m.txt" || exit 2
    sed -nE "s#^([0-9a-f]+)  (REUSUS|IRRITUM) *(.*)#$m\t\1\t\2\t\3#p" "$T/m.txt" >> "$T/membra.tsv"
    printf '%s\t%s\n' "$m" "$(_ms "$m")" >> "$T/tempora.tsv"
done < "$T/nomina"

# PORTA '-': verdictum totum cursoris = omnia membra reusa et cursor
# intactus (commissiones ordine membrorum)
if [ ! -f "$T/porta.tsv" ]; then
    awk -F'\t' 'FILENAME ~ /cursor_tacta/ { tacta[$1] = 1; next }
        { c = $2; if (!(c in visa)) { visa[c] = 1; ordo[++n] = c }
          if ($3 != "REUSUS") irr[c] = 1 }
        END { for (i = 1; i <= n; i++) { c = ordo[i]
              print c "\t" ((c in irr) || (c in tacta) ? "IRRITUM" : "REUSUS") } }' \
        "$T/cursor_tacta" "$T/membra.tsv" > "$T/porta.tsv"
fi

awk -F'\t' -v gradus="$GRADUS" -v porta="$PORTA" -v n_arg="$N" '
    FILENAME ~ /tempora/ { ms[$1] = $2 + 0; next }
    FILENAME ~ /porta.tsv/ { pc[$1] = $2; if (!($1 in visa)) { visa[$1] = 1; ordo[++nc] = $1 }; next }
    {
        m = $1; c = $2
        if (!(m in mv)) { mv[m] = 1; mem[++nm] = m }
        st[m, c] = $3
        if ($3 == "IRRITUM") causa[$4]++
    }
    END {
        preusi = 0; greusi = 0; mreusi = 0; pcursae = 0; gcursae = 0
        pms = 0; gms = 0
        for (i = 1; i <= nc; i++) {
            c = ordo[i]
            if (pc[c] == "REUSUS") preusi++
            else { pcursae += nm; pms += ms[porta] }
            omnia = 1
            for (k = 1; k <= nm; k++) {
                m = mem[k]
                if (st[m, c] == "REUSUS") { mreusi++; mr[m]++ }
                else { omnia = 0; gcursae++; gms += ms[m] }
            }
            if (omnia) greusi++
        }
        printf "A/B reusus: %d commissiones, %d membra (%s contra %s)\n\n", nc, nm, gradus, porta
        printf "porta %-22s reusa %3d/%d (%d%%)   probationes cursae %5d   tempus ~%5.0f s\n",
            porta, preusi, nc, nc ? 100 * preusi / nc : 0, pcursae, pms / 1000
        printf "gradus %-21s totum %3d/%d (%d%%)   probationes cursae %5d   tempus ~%5.0f s\n",
            gradus, greusi, nc, nc ? 100 * greusi / nc : 0, gcursae, gms / 1000
        printf "membra reusa %d/%d (%d%%)\n", mreusi, nc * nm, nc * nm ? 100 * mreusi / (nc * nm) : 0
        if (pcursae > 0)
            printf "lucrum: probationes cursae -%d%%, tempus -%d%%\n",
                100 - 100 * gcursae / pcursae, pms ? 100 - 100 * gms / pms : 0
        printf "\nper membrum (reusus, tempus cursus):\n"
        for (k = 1; k <= nm; k++) {
            m = mem[k]
            printf "  %-46s %3d/%d (%3d%%)  %6.1f s\n", m, mr[m], nc, nc ? 100 * mr[m] / nc : 0, ms[m] / 1000
        }
        printf "\ncausae irritorum membrorum (summa membrorum):\n"
        for (k in causa) printf "%6d  %s\n", causa[k], k | "sort -rn | head -12"
    }' "$T/tempora.tsv" "$T/porta.tsv" "$T/membra.tsv"
exit 0
