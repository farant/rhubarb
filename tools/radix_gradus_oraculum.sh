#!/bin/bash
# tools/radix_gradus_oraculum.sh - ORACULUM suitae radicis (fabrica-7 T5):
# cursor radicis (compile_tests.sh, porta radix) et actio gradus
# 'probationes_radicis' (probationes_c, aedificatio.stml) in arbore
# EADEM. Per probationem congruere debent:
#   - genus: transiit | fracta | aedificata (cursor: GUI aedificata aut
#     reticularis exclusa; gradus: 'nexus solum' facultatis);
#   - numeri assertionum praeteriti/totales: cursor ex compendio credo
#     ULTIMO acti sui (build/test_logs/singulae/<nomen>.log), gradus ex
#     summa linearum SECTIO (fabrica-7 T1; suita classica = 'totum').
#
#   ./tools/radix_gradus_oraculum.sh [-machina]
#
# Effusio: 'DISSONAT <probatio> cursor=G:P/T gradus=G:P/T' per
# dissonantiam (-machina: TSV omnium), deinde 'fractae cursoris: ...',
# 'fractae gradus: ...', VERDICTUM et 'oraculum radicis: congruunt N/M'.
# Exitus: 0 omnes congruunt · 1 dissonantia · 2 cursus fractus (nihil
# iudicatum: bin/fabrica deest, cursor nihil cucurrit).
set -u
# octeti, non characteres: effusiones probationum UTF-8 invalidum
# consulto ferunt (probatio_json) - awk macOS sub locale UTF-8 abortat
# ('towc: multibyte conversion failure') et numeri 0/0 falso fiebant
export LC_ALL=C
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
MACHINA=0
[ "${1:-}" = "-machina" ] && MACHINA=1
[ -x bin/fabrica ] || { echo "oraculum radicis: bin/fabrica deest" >&2; exit 2; }
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
SINGULAE=build/test_logs/singulae

# numeri compendii ULTIMI acti (credo: 'Totalis:', 'Praeteriti:');
# absentes = 0/0 (probatio sine credo)
numeri () {
    [ -f "$1" ] || { printf '0/0'; return; }
    awk '/^Totalis:/ {t = $2} /^Praeteriti:/ {p = $2}
         END {printf "%d/%d", p, t}' "$1"
}

# I. cursor: verdicta per probationem ex .res (rc tc tr), GUI ex lineis
#    'GUI APP BUILT' / reticulares ex 'RETICULARIS EXCLUSUM'
./compile_tests.sh > "$T/cursor" 2>&1
: > "$T/c"
for r in "$SINGULAE"/*.res; do
    [ -f "$r" ] || continue
    n=$(basename "$r" .res)
    read -r rc tc _ < "$r"
    g=fracta; [ "$rc" = "0" ] && g=transiit
    [ "$rc" = "2" ] && [ "$tc" = "0" ] && g=fracta
    printf '%s %s %s\n' "$n" "$g" "$(numeri "$SINGULAE/$n.log")" >> "$T/c"
done
sed 's/\x1b\[[0-9;]*m//g' "$T/cursor" \
    | sed -n 's/.*RETICULARIS EXCLUSUM: \(probatio_[a-z0-9_]*\).*/\1/p' \
    | while read -r n; do printf '%s aedificata 0/0\n' "$n"; done >> "$T/c"
# GUI: quod cursor VERE struxit ('GUI APP BUILT: <nomen>'); structura
# fracta = fracta (fabrica-7 T7: olim index GUI_LISTA aedilis_porta.sh)
sed 's/\x1b\[[0-9;]*m//g' "$T/cursor" \
    | sed -n 's/.*GUI APP BUILT: \(probatio_[a-z0-9_]*\).*/\1 aedificata 0\/0/p;
              s/.*BUILD FAILED: \(probatio_[a-z0-9_]*\).*/\1 fracta 0\/0/p' >> "$T/c"
sort -u -k1,1 "$T/c" -o "$T/c"
[ "$(grep -c ' transiit ' "$T/c")" -gt 0 ] || { echo "oraculum radicis: cursor nihil cucurrit" >&2; tail -5 "$T/cursor" >&2; exit 2; }

# II. gradus: sanare (membra stala), deinde iudicium plenum - IUDICIUM
#     per membrum, SECTIO post eum; nota verdicti 'nexus solum' =
#     aedificata
bin/fabrica sanare probationes_radicis > "$T/sanare" 2>&1
bin/fabrica iudicare -plenus -omnia -machina probationes_radicis > "$T/gradus" 2>&1
P=build/fabrica/area/probationes_radicis
awk -F'\t' -v P="$P/" '
    $1 == "IUDICIUM" {
        v = $3
        if (index(v, P) != 1 || v !~ /\/verdictum\.txt$/) next
        sub(P, "", v); sub(/\/verdictum\.txt$/, "", v)
        status[v] = ($2 == "RECENS") ? "transiit" : "fracta"
        ordo[++n] = v
    }
    $1 == "SECTIO" {
        m = $2; sub(/^probationes_radicis\//, "", m)
        p[m] += $5; t[m] += $6
    }
    END {
        for (i = 1; i <= n; i++) {
            v = ordo[i]
            printf "%s %s %d/%d\n", v, status[v], p[v], t[v]
        }
    }' "$T/gradus" > "$T/g0"
: > "$T/g"
while read -r n s q; do
    if [ "$s" = transiit ] && grep -q 'nexus solum' "$P/$n/verdictum.txt" 2>/dev/null; then
        s=aedificata; q=0/0
    fi
    printf '%s %s %s\n' "$n" "$s" "$q" >> "$T/g"
done < "$T/g0"
sort -u -k1,1 "$T/g" -o "$T/g"
[ -s "$T/g" ] || { echo "oraculum radicis: gradus nihil iudicavit" >&2; tail -5 "$T/gradus" >&2; exit 2; }

# III. comparatio per probationem (unio nominum); numeri pro
#      aedificatis non comparantur (nihil cursum)
cut -d' ' -f1 "$T/c" "$T/g" | sort -u > "$T/nomina"
n=0; m=0
while read -r nomen; do
    c=$(awk -v n="$nomen" '$1==n {print $2 ":" $3}' "$T/c"); c=${c:-absens}
    g=$(awk -v n="$nomen" '$1==n {print $2 ":" $3}' "$T/g"); g=${g:-absens}
    m=$((m + 1))
    if [ "$c" = "$g" ]; then
        n=$((n + 1)); v=congruit
    else
        v=DISSONAT
    fi
    if [ "$MACHINA" -eq 1 ]; then
        printf '%s\t%s\t%s\t%s\n' "$v" "$nomen" "$c" "$g"
    elif [ "$v" = DISSONAT ]; then
        printf 'DISSONAT %s cursor=%s gradus=%s\n' "$nomen" "$c" "$g"
    fi
done < "$T/nomina"
echo "fractae cursoris: $(awk '$2=="fracta" {printf "%s ", $1}' "$T/c")"
echo "fractae gradus: $(awk '$2=="fracta" {printf "%s ", $1}' "$T/g")"
echo "sanatae gradus: $(grep -c '^SANATUM\|^FRACTUM' "$T/sanare")"
awk -F'\t' '$1 == "VERDICTUM" { printf "VERDICTUM %s: %s/%s\n", $2, $3, $4 }' "$T/gradus"
sed 's/\x1b\[[0-9;]*m//g' "$T/cursor" | grep -E '^Tests (Total|Passed|Failed):'
echo "oraculum radicis: congruunt $n/$m"
[ "$n" -eq "$m" ] && exit 0
exit 1
