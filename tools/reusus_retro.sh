#!/bin/bash
# tools/reusus_retro.sh - REUSUS RETRO verdicti (fabrica plan 5 T4):
# transitus servatus actionis TITULUS inter commissiones consecutivas
# ultimas N reutilis fuisset? Ingressus hodierni ex vestigio (build/
# fabrica.db), clavi effectus (crusta/effectus.sh -clavis), ingressibus
# declaratis (aedificatio.stml) et clausuris binariorum domus (bin/aedilis
# sub FABRICA_LECTIONES) in REGULAS vertuntur; 'git diff --name-status'
# cuiusque commissionis contra parentem primum eas probat.
#
# Usage:  ./tools/reusus_retro.sh TITULUS [-n N] [-addere PLAGULA]
#         [-specificatio PLAGULA]
#   -addere        regulae additae (forma infra) - ut vestigium VETUS
#                  reconstruatur (enumerationes, ingressus remoti)
#   -specificatio  regulas scribit et exit (nihil probat)
# Regula (tab separata): genus  via  exemplar  causa
#   F  via mutata, addita aut deleta     (lectio L, octeti, fasciculus)
#   E  via addita aut deleta             (X, A, probatio)
#   N  in directorio via plagula addita/deleta nomine exemplar (glob)
#                                        (D, nomina, globus)
#   T  quidvis sub via mutatum           (arbor)
# Exitus: 0 relatio scripta · 2 usus / memoria absens.
#
# APPROXIMATIONES (nominatae): viae sub build/ et absolutae (systema)
# git non videt - derivata ex fontibus suis computantur (actio
# producens: ingressus eius uno gradu); ambitus et identitas clang
# omittuntur; clausura binarii domus = lectiones aedilis HODIERNAE.
# Instrumentum, non porta (C14).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
TITULUS="${1:-}"; shift || true
[ -n "$TITULUS" ] || { sed -n '9,13p' "$0" >&2; exit 2; }
N=40; ADDITA=""; SCRIBERE=""
while [ $# -gt 0 ]; do
    case "$1" in
        -n) N="$2"; shift 2 ;;
        -addere) ADDITA="$2"; shift 2 ;;
        -specificatio) SCRIBERE="$2"; shift 2 ;;
        *) echo "reusus_retro: optio ignota $1" >&2; exit 2 ;;
    esac
done
[ -f build/fabrica.db ] || { echo "reusus_retro: build/fabrica.db deest" >&2; exit 2; }
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
REG="$T/regulae.tsv"

# via servanda? (relativa, non build/)
_servanda () { case "$1" in /*|build/*|*/build/*|'') return 1 ;; esac; return 0; }

# clausura binarii domus bin/X: lectiones aedilis super tools/X.c
_clausura () {
    local bin="$1" fons="tools/$(basename "$1").c" g v
    [ -f "$fons" ] || { printf 'F\t%s\t\tprovenientia %s\n' "$bin" "$bin"; return; }
    rm -f "$T/cl.txt"
    FABRICA_LECTIONES="$T/cl.txt" bin/aedilis "$fons" --enumerare > /dev/null 2>&1
    while IFS=$'\t' read -r g v _; do
        v="${v#./}"
        _servanda "$v" || continue
        case "$g" in
            L) printf 'F\t%s\t\tprovenientia %s: %s\n' "$v" "$bin" "$v" ;;
            X|A) printf 'E\t%s\t\tprovenientia %s: %s\n' "$v" "$bin" "$v" ;;
        esac
    done < "$T/cl.txt"
}

# lineae clavis effectus scripti -> regulae
_effectus () {
    local via="$1" g a b
    ./crusta/effectus.sh -clavis "$via" 2>/dev/null | while IFS=$'\t' read -r g a b; do
        case "$g" in
            octeti) _servanda "$a" && printf 'F\t%s\t\tclavis effectus: %s\n' "$a" "$a" ;;
            probatio) _servanda "$a" && [ "$a" != "." ] && printf 'E\t%s\t\tclavis effectus: probatio %s\n' "$a" "$a" ;;
            nomina) _servanda "$a" && printf 'N\t%s\t%s\tclavis effectus: nomina %s%s\n' "${a%/}" "$b" "$a" "$b" ;;
            globus) _servanda "$a" && printf 'N\t%s\t%s\tclavis effectus: globus %s\n' "$(dirname "$a")" "$(basename "$a")" "$a" ;;
            arbor) _servanda "$a" && printf 'T\t%s\t\tclavis effectus: arbor %s\n' "${a%/}" "$a" ;;
            provenientia) _clausura "$a" ;;
        esac
    done
}

# ingressus declarati actionis TITULUS (et actionum producentium build/)
PLAGULAE_AED="$(ls aedificatio.stml */aedificatio.stml 2>/dev/null)"
_ingressus () {
    local titulus="$1" gradus="$2"
    # shellcheck disable=SC2086 (plagulae: verba consulto scissa)
    awk -v t="$titulus" '
        $0 ~ "<actio titulus=\"" t "\"" { intus = 1 }
        intus && /<ingressus / {
            g = $0; sub(/.*genus="/, "", g); sub(/".*/, "", g)
            v = $0; sub(/.*via="/, "", v); sub(/".*/, "", v)
            print g "\t" v
        }
        intus && /<\/actio>/ { intus = 0 }' $PLAGULAE_AED |
    while IFS=$'\t' read -r g v; do
        case "$g" in
            fasciculus)
                if _servanda "$v"; then
                    printf 'F\t%s\t\tingressus %s\n' "$v" "$v"
                elif [ "$gradus" -eq 0 ]; then
                    # productum build/: actio producens, gradu uno
                    # shellcheck disable=SC2086
                    p="$(awk -v e="$v" '
                        /<actio titulus="/ { a = $0; sub(/.*titulus="/, "", a); sub(/".*/, "", a) }
                        $0 ~ "<exitus via=\"" e "\"" { print a; exit }' $PLAGULAE_AED)"
                    [ -n "$p" ] && _ingressus "$p" 1
                fi ;;
            effectus) _effectus "$v" ;;
            instrumentum_domus) _clausura "$v" ;;
        esac
    done
}

{
    # vestigium servatum
    sqlite3 -separator $'\t' build/fabrica.db \
        "SELECT DISTINCT genus, via FROM lectiones WHERE titulus = '$TITULUS'" |
    while IFS=$'\t' read -r g v; do
        v="${v#./}"
        _servanda "$v" || continue
        case "$g" in
            L) printf 'F\t%s\t\tlectio %s\n' "$v" "$v" ;;
            X|A) printf 'E\t%s\t\tlectio %s\n' "$v" "$v" ;;
            D) printf 'N\t%s\t*\tenumeratio %s\n' "${v%/}" "$v" ;;
        esac
    done
    _ingressus "$TITULUS" 0
    [ -n "$ADDITA" ] && grep -v '^#' "$ADDITA"
} | sort -u > "$REG"
if [ ! -s "$REG" ]; then
    echo "reusus_retro: nulla regula pro $TITULUS (vestigium nullum?)" >&2; exit 2
fi
if [ -n "$SCRIBERE" ]; then
    cp "$REG" "$SCRIBERE"; echo "reusus_retro: $(wc -l < "$REG" | tr -d ' ') regulae -> $SCRIBERE"; exit 0
fi

# commissiones (parens primus) et differentiae
: > "$T/diff.tsv"
git log --first-parent -n "$N" --format='%H %h' | while read -r h brevis; do
    git rev-parse -q --verify "$h^" > /dev/null || continue
    git diff --no-renames --name-status "$h^" "$h" | while IFS=$'\t' read -r s v; do
        printf '%s\t%s\t%s\n' "$brevis" "$s" "$v"
    done
    printf '%s\t-\t-\n' "$brevis"   # signum commissionis (etiam sine mutatione)
done > "$T/diff.tsv"

awk -F'\t' -v titulus="$TITULUS" '
    function glob_re(g,   r) {
        r = g; gsub(/[.+^$(){}|\\]/, "\\\\&", r); gsub(/\*/, "[^/]*", r); gsub(/\?/, "[^/]", r)
        return "^" r "$"
    }
    FNR == NR {
        if ($1 == "F") F[$2] = $4
        else if ($1 == "E") E[$2] = $4
        else if ($1 == "N") { nN++; Nd[nN] = $2; Nre[nN] = glob_re($3); Nc[nN] = $4 }
        else if ($1 == "T") { nT++; Td[nT] = $2; Tc[nT] = $4 }
        next
    }
    {
        c = $1
        if (!(c in visa)) { visa[c] = 1; ordo[++nc] = c }
        if ($2 == "-") next
        if (causa[c] != "") next
        s = substr($2, 1, 1); v = $3
        if (v in F) { causa[c] = F[v]; next }
        if ((s == "A" || s == "D") && (v in E)) { causa[c] = E[v]; next }
        if (s == "A" || s == "D") {
            d = v; sub(/\/[^\/]*$/, "", d); if (d == v) d = "."
            b = v; sub(/.*\//, "", b)
            for (k = 1; k <= nN; k++) if (d == Nd[k] && b ~ Nre[k]) { causa[c] = Nc[k]; next }
        }
        for (k = 1; k <= nT; k++) if (index(v, Td[k] "/") == 1) { causa[c] = Tc[k]; next }
    }
    END {
        reusi = 0
        for (i = 1; i <= nc; i++) {
            c = ordo[i]
            if (causa[c] == "") { reusi++; printf "%s  REUSUS\n", c }
            else { printf "%s  IRRITUM  %s\n", c, causa[c]; n[causa[c]]++ }
        }
        printf "\nreusus retro %s: %d/%d (%d%%)\n", titulus, reusi, nc, nc ? (100 * reusi) / nc : 0
        printf "causae:\n"
        for (k in n) printf "%6d  %s\n", n[k], k | "sort -rn"
    }' "$REG" "$T/diff.tsv"
exit 0
