#!/bin/bash
# tools/toml_gradus_oraculum.sh - ORACULUM gradus toml (fabrica-6 T8):
# cursor toml (toml/compile_probationes.sh, porta_toml) et actio gradus
# 'probationes_toml' (probationes_c, toml/aedificatio.stml) in arbore
# EADEM; transitus/fractura per probationem congruere debent.
#
#   ./tools/toml_gradus_oraculum.sh [-machina]
#
# Effusio: 'congruit/DISSONAT <probatio> cursor=X gradus=Y' per
# probationem (-machina: TSV), deinde 'fractae cursoris: ...',
# 'fractae gradus: ...' et 'oraculum toml: congruunt N/M'.
# Exitus: 0 omnes congruunt · 1 dissonantia · 2 cursus fractus (nihil
# iudicatum: bin/fabrica deest, cursor nihil cucurrit).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
MACHINA=0
[ "${1:-}" = "-machina" ] && MACHINA=1
[ -x bin/fabrica ] || { echo "oraculum toml: bin/fabrica deest" >&2; exit 2; }
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT

# I. cursor: '--- NOMEN praeteriit' / '--- NOMEN FRACTA' /
#    'FRACTA (compilatio): NOMEN'
./toml/compile_probationes.sh > "$T/cursor" 2>&1
sed -n 's/^--- \(probatio_[a-z_]*\) praeteriit.*/\1 transiit/p;
        s/^--- \(probatio_[a-z_]*\) FRACTA.*/\1 fracta/p;
        s/^FRACTA (compilatio): \(probatio_[a-z_]*\).*/\1 fracta/p' \
    "$T/cursor" | sort -u > "$T/c"
[ -s "$T/c" ] || { echo "oraculum toml: cursor nihil cucurrit" >&2; tail -5 "$T/cursor" >&2; exit 2; }

# II. gradus: sanare (membra stala), deinde iudicium plenum per membrum
bin/fabrica sanare probationes_toml > "$T/sanare" 2>&1
bin/fabrica iudicare -plenus -omnia -machina probationes_toml > "$T/gradus" 2>&1
# lineae machinae (fabrica-6 H1): IUDICIUM status via causa
awk -F'\t' '$1 == "IUDICIUM" {
        v = $3
        if (v !~ /^build\/fabrica\/area\/probationes_toml\/[a-z_]*\/verdictum\.txt$/) next
        sub(/^build\/fabrica\/area\/probationes_toml\//, "", v); sub(/\/verdictum\.txt$/, "", v)
        print v " " ($2 == "RECENS" ? "transiit" : "fracta")
    }' "$T/gradus" | sort -u > "$T/g"
[ -s "$T/g" ] || { echo "oraculum toml: gradus nihil iudicavit" >&2; tail -5 "$T/gradus" >&2; exit 2; }

# III. comparatio per probationem (unio nominum)
cut -d' ' -f1 "$T/c" "$T/g" | sort -u > "$T/nomina"
n=0; m=0
while read -r nomen; do
    c=$(awk -v n="$nomen" '$1==n {print $2}' "$T/c"); c=${c:-absens}
    g=$(awk -v n="$nomen" '$1==n {print $2}' "$T/g"); g=${g:-absens}
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
awk -F'\t' '$1 == "VERDICTUM" { printf "VERDICTUM %s: %s/%s\n", $2, $3, $4 }' "$T/gradus"
grep '^TOML PROBATIONES' "$T/cursor"
echo "oraculum toml: congruunt $n/$m"
[ "$n" -eq "$m" ] && exit 0
exit 1
