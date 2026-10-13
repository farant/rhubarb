#!/bin/bash
# tools/gradus_oraculum.sh - ORACULUM conversionis cursoris in gradus
# (cursores ad gradus T2; olim tools/toml_gradus_oraculum.sh, fabrica-6
# T8): cursor VETUS subsystematis (<sub>/compile_probationes.sh ex
# commisso REF) et compositum probationes_<sub> in arbore EADEM;
# transitus/fractura per probationem congruere debent. Conversio nulla
# committitur nisi oraculum congruit.
#
#   ./tools/gradus_oraculum.sh SUB [-ref REF] [-machina]
#
# REF ordinarie HEAD (cursor nondum conversus). Cursor vetus in
# <sub>/ (BASH_SOURCE eius radicem dat) ut plagula occulta temporaria
# scribitur et post cursum deletur. Compositum per 'bin/fabrica probare
# -vis' (membra OMNIA vere currunt - oraculum reusum non credit).
#
# Effusio: 'DISSONAT <probatio> cursor=X gradus=Y' per discordiam
# (-machina: 'congruit|DISSONAT<TAB>probatio<TAB>cursor<TAB>gradus' per
# probationem), 'fractae cursoris/gradus: ...', summae amborum,
# 'oraculum SUB: congruunt N/M'. Exitus: 0 omnes congruunt · 1
# dissonantia · 2 nihil iudicatum (usus, REF sine cursore, bin/fabrica
# deest, cursus nihil dedit).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
SUB="" REF=HEAD MACHINA=0
while [ $# -gt 0 ]; do
    case "$1" in
        -ref) REF="${2:-}"; shift 2 ;;
        -machina) MACHINA=1; shift ;;
        -*) echo "oraculum: vexillum ignotum $1" >&2; exit 2 ;;
        *) [ -z "$SUB" ] || { echo "oraculum: subsystema unum" >&2; exit 2; }
           SUB="$1"; shift ;;
    esac
done
[ -n "$SUB" ] && [ -n "$REF" ] || {
    echo "usus: ./tools/gradus_oraculum.sh SUB [-ref REF] [-machina]" >&2; exit 2; }
[ -x bin/fabrica ] || { echo "oraculum $SUB: bin/fabrica deest" >&2; exit 2; }
T="$(mktemp -d)"
VETUS="$SUB/.oraculum_cursor_vetus.sh"
trap 'rm -rf "$T" "$VETUS"' EXIT
git show "$REF:$SUB/compile_probationes.sh" > "$VETUS" 2>/dev/null || {
    echo "oraculum $SUB: $REF:$SUB/compile_probationes.sh abest" >&2; exit 2; }
chmod +x "$VETUS"

# I. cursor vetus: '--- NOMEN praeteriit' / '--- NOMEN FRACTA' /
#    'FRACTA (compilatio): NOMEN'
"./$VETUS" > "$T/cursor" 2>&1
sed -n 's/^--- \(probatio_[A-Za-z0-9_]*\) praeteriit.*/\1 transiit/p;
        s/^--- \(probatio_[A-Za-z0-9_]*\) FRACTA.*/\1 fracta/p;
        s/^FRACTA (compilatio): \(probatio_[A-Za-z0-9_]*\).*/\1 fracta/p' \
    "$T/cursor" | sort -u > "$T/c"
[ -s "$T/c" ] || { echo "oraculum $SUB: cursor nihil cucurrit" >&2; tail -5 "$T/cursor" >&2; exit 2; }

# II. gradus: probare -vis (omnia currunt), lineae MEMBRUM
bin/fabrica probare -machina -vis "probationes_$SUB" > "$T/gradus" 2>&1
awk -F'\t' '$1 == "MEMBRUM" {
        print $2 " " (($3 == "transiit" || $3 == "servata") ? "transiit" : "fracta")
    }' "$T/gradus" | sort -u > "$T/g"
[ -s "$T/g" ] || { echo "oraculum $SUB: gradus nihil iudicavit" >&2; tail -5 "$T/gradus" >&2; exit 2; }

# III. comparatio per probationem (unio nominum; absens ex parte una =
#      DISSONAT)
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
awk -F'\t' '$1 == "PROBARE" { printf "PROBARE %s: %s/%s\n", $2, $3, $4 }' "$T/gradus"
grep -E '^[A-Z_]+ PROBATIONES: ' "$T/cursor" | tail -1
echo "oraculum $SUB: congruunt $n/$m"
[ "$n" -eq "$m" ] && exit 0
exit 1
