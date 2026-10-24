#!/bin/bash
# tools/interrogationes_fumus.sh - porta 'interrogationes' (spec
# unitates-codicis par. VII, U5b gradus 3): RESPONSUM MORTUUM. Omnis
# ancora responsi (lib/S.interrogationes.stml) rem veram nominare
# debet:
#
#   symbolum  sedem (definitio aut declaratio) habet in plagula MANU
#             scripta cuius munus caput aut fons est - tabula
#             silva/nexus.sh (renovata semel, deinde percursus UNUS).
#             Sedes in fixtura, amalgama aut generato NON vivificat:
#             copia congelata symbolum deletum diu servat et mortem
#             celaret. Munus et provenientia per bin/fabrica quid
#             (lib/unitates.c), numquam index viarum.
#   via       plagula aut directorium in arbore exstat
#   imperium  verbum primum: cum '/' plagula exsecutabilis radici
#             relativa; sine '/' mandatum in PATH (command -v)
#
# Prius: 'quid -interrogationes' sine vitio (lectio ipsa: porta
# unitates gradus VIII). Mortua NOMINANTUR (via:linea, genus ancorae,
# valor); numerus solus non sufficit.
#
# -tegmen [include/X.h ...]: RELATIO (non porta) - functiones quas caput
# declarat (tabula nexus, sedes generis functio) quae nullam ancoram
# 'symbolum' habent: aut opus scribendum, aut non vere publicae. Sine
# capitibus: capita unitatum quae iam interrogationes habent. Exitus 0
# (relatio), 2 iudicare nequit.
#
# Usus: ./tools/interrogationes_fumus.sh [-tegmen [CAPUT...]]
# Exitus: 0 sanum · 1 FRACTUM (responsum mortuum aut vitium lectionis)
#         · 2 iudicare nequit (bin/fabrica deest, tabula nexus fracta,
#         nulla interrogatio - porta quae nihil iudicat mortua est)
# <porta titulus="interrogationes" genus="praeiudicium" tegit="*.interrogationes.stml interrogationes.canon include/* tools/interrogationes_fumus.sh" currit="tools/fabrica.c silva/instrumenta/principalia/nexus.c" legit="*.c *.h *.stml"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
_fractum () { echo "  FRACTUM $1"; echo "fumus interrogationum: FRACTUM"; exit 2; }
[ -x bin/fabrica ] || _fractum "bin/fabrica deest (./tools/fabrica_struere.sh)"
T="$(mktemp -d "${TMPDIR:-/tmp}/interrogationes_fumus.XXXXXX")"
trap 'rm -rf "$T"' EXIT

bin/fabrica quid -interrogationes -machina > "$T/i" 2> "$T/i_err"; rc=$?
if [ "$rc" -eq 2 ]; then
    sed 's/^/      /' "$T/i_err" | head -5
    _fractum "bin/fabrica quid -interrogationes nequit (exitus 2)"
fi
n=$(awk -F'\t' '$1 == "SUMMA" { print $3 }' "$T/i")

if [ "${1:-}" = "-tegmen" ]; then
    shift
    capita=("$@")
    if [ "${#capita[@]}" -eq 0 ]; then
        # capita possessorum: stemma S -> include/S.h (si exstat)
        while IFS= read -r t; do
            [ -f "include/$t.h" ] && capita+=("include/$t.h")
        done < <(awk -F'\t' '$1 == "INTERROGATIO" { print $2 }' "$T/i" | sort -u)
    fi
    [ "${#capita[@]}" -gt 0 ] || _fractum "nulla capita (-tegmen include/X.h)"
    ./silva/nexus.sh -renovare > "$T/renovatio" 2>&1 \
        || { sed 's/^/      /' "$T/renovatio" | tail -5; _fractum "silva/nexus.sh -renovare fractum"; }
    awk -F'\t' '$1 == "INTERROGATIO" { k = split($8, s, " "); for (j = 1; j <= k; j++) print s[j] }' \
        "$T/i" | sort -u > "$T/ancorata"
    printf '%s\n' "${capita[@]}" > "$T/capita"
    awk -F'\t' 'NR == FNR { c[$1] = 1; next }
        $2 == "sedes" && $3 == "functio" && ($4 in c) { print $4 "\t" $1 }' \
        "$T/capita" build/nexus.tsv | sort -u > "$T/functiones"
    for caput in "${capita[@]}"; do
        omnes=$(awk -F'\t' -v c="$caput" '$1 == c' "$T/functiones" | wc -l | tr -d ' ')
        nudae=$(awk -F'\t' -v c="$caput" 'NR == FNR { a[$1] = 1; next } $1 == c && !($2 in a) { print $2 }' \
            "$T/ancorata" "$T/functiones")
        tectae=$((omnes - $(printf '%s' "$nudae" | grep -c .)))
        echo "$caput: $tectae/$omnes functiones ancoratae"
        [ -n "$nudae" ] && printf '%s\n' "$nudae" | sed 's/^/  sine interrogatione: /'
    done
    exit 0
fi
[ "${n:-0}" -gt 0 ] || _fractum "nulla interrogatio iudicata"
fracta=0
if [ "$rc" -ne 0 ]; then
    echo "  vitia lectionis:"
    awk -F'\t' '$1 == "VITIUM" { print "      " $2 ":" $3 ": " $4 }' "$T/i"
    fracta=1
fi

# ancorae: 'genus<TAB>valor<TAB>via:linea' (symbola et viae spatiis)
awk -F'\t' '$1 == "INTERROGATIO" {
    loc = $3 ":" $4
    k = split($8, s, " "); for (j = 1; j <= k; j++) print "symbolum\t" s[j] "\t" loc
    k = split($9, v, " "); for (j = 1; j <= k; j++) print "via\t" v[j] "\t" loc
    if ($10 != "") print "imperium\t" $10 "\t" loc
}' "$T/i" > "$T/ancorae"
ns=$(awk -F'\t' '$1 == "symbolum"' "$T/ancorae" | wc -l | tr -d ' ')
nv=$(awk -F'\t' '$1 == "via"' "$T/ancorae" | wc -l | tr -d ' ')
ni=$(awk -F'\t' '$1 == "imperium"' "$T/ancorae" | wc -l | tr -d ' ')

# symbola: tabula renovata semel, percursus unus pro omnibus
mortua=0
if [ "$ns" -gt 0 ]; then
    ./silva/nexus.sh -renovare > "$T/renovatio" 2>&1 \
        || { sed 's/^/      /' "$T/renovatio" | tail -5; _fractum "silva/nexus.sh -renovare fractum"; }
    head -1 build/nexus.tsv 2>/dev/null | grep -q '^# nexus.tsv GENERATUM' \
        || _fractum "build/nexus.tsv sine capite (tabula fracta)"
    awk -F'\t' '$1 == "symbolum" { print $2 }' "$T/ancorae" | sort -u > "$T/quaesita"
    awk -F'\t' 'NR == FNR { q[$1] = 1; next }
        $2 == "sedes" && ($1 in q) { print $1 "\t" $4 }' \
        "$T/quaesita" build/nexus.tsv | sort -u > "$T/sedes"
    cut -f2 "$T/sedes" | sort -u > "$T/sedium_viae"
    : > "$T/verae"
    if [ -s "$T/sedium_viae" ]; then
        # shellcheck disable=SC2046
        bin/fabrica quid -machina $(cat "$T/sedium_viae") > "$T/quid" 2>&1 \
            || _fractum "bin/fabrica quid in sedibus nequit"
        awk -F'\t' '$2 == "munus" { m[$1] = $3 } $2 == "provenientia" { p[$1] = $3 }
            END { for (v in m) if ((m[v] == "caput" || m[v] == "fons") && p[v] == "manu") print v }' \
            "$T/quid" | sort -u > "$T/verae"
    fi
    awk -F'\t' 'NR == FNR { vera[$1] = 1; next } ($2 in vera) { print $1 }' \
        "$T/verae" "$T/sedes" | sort -u > "$T/viva"
    while IFS='	' read -r genus valor loc; do
        [ "$genus" = "symbolum" ] || continue
        grep -qxF "$valor" "$T/viva" && continue
        if grep -q "^$valor	" "$T/sedes"; then
            echo "  MORTUUM $loc symbolum $valor - sedes solum extra fontem manu scriptum ($(grep "^$valor	" "$T/sedes" | cut -f2 | head -2 | tr '\n' ' '))"
        else
            echo "  MORTUUM $loc symbolum $valor - nusquam definitum (silva/nexus.sh)"
        fi
        mortua=$((mortua + 1))
    done < "$T/ancorae"
fi
while IFS='	' read -r genus valor loc; do
    case "$genus" in
        via)
            [ -e "$valor" ] && continue
            echo "  MORTUUM $loc via $valor - non exstat"
            mortua=$((mortua + 1)) ;;
        imperium)
            verbum="${valor%% *}"
            case "$verbum" in
                */*) [ -f "$verbum" ] && [ -x "$verbum" ] && continue
                     echo "  MORTUUM $loc imperium '$valor' - $verbum non exsecutabilis" ;;
                *)   command -v "$verbum" > /dev/null 2>&1 && continue
                     echo "  MORTUUM $loc imperium '$valor' - $verbum non in PATH" ;;
            esac
            mortua=$((mortua + 1)) ;;
    esac
done < "$T/ancorae"

echo "  interrogationes $n; ancorae: symbola $ns, viae $nv, imperia $ni; mortua $mortua"
if [ "$fracta" -ne 0 ] || [ "$mortua" -ne 0 ]; then
    echo "fumus interrogationum: FRACTUM"
    exit 1
fi
echo "fumus interrogationum: sanum"
exit 0
