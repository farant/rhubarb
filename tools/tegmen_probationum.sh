#!/bin/bash
# tools/tegmen_probationum.sh - TEGMEN MENSURATUM: quantum bibliothecae
# lib/S.c suita SUA (probationes/probatio_S.c) vere exercet - lineae,
# functiones, rami per llvm-cov, non nomina in probationibus quaesita
# (mentio 82% dicebat; mensura xar 78% linearum, 65% ramorum).
#
# Structura PRIVATA: bin/aedilis scriptum suitae in build/tegmen/<S>/
# gignit (--exitus), obiecta in build/tegmen/obiecta (--obiecta, thesaurus
# proprius - communis build/aedilis/obiecta numquam tangitur; lectio
# portae instrumenta 2026-10-17); VEXILLA scripti '-O0 -g
# -fprofile-instr-generate -fcoverage-mapping' fiunt. Substitutio
# probatur: scriptum sine eis = exitus 2 (transformatio tacita nihil
# mensurat, sed 0% mentiretur).
#
# Suita ex radice currit (ut compile_tests.sh), termino MORA s; suita
# rubra tegmen tamen refert (nota RUBRA). Relatio, non porta.
#
# Effusio (-machina, TSV):
#   TEGMEN  S  lineae_tectae lineae  functiones_tectae functiones
#           rami_tecti rami  exitus_suitae
#   NUDA    S  functio                (numquam intrata)
#   LINEAE  S  a-b,c-d,...            (lineae numquam actae)
#   SINE    S  causa                  (sine suita, structura fracta...)
# Homo: tabula una per bibliothecam + functiones nudae.
#
# -sanitare: SANITAS loco tegminis - eadem structura privata (build/
# tegmen_sanitas/), VEXILLA '-O1 -g -fno-omit-frame-pointer
# -fsanitize=address,undefined' (+ -Wno-deprecated-declarations: SDK sprintf
# sub sanitatoribus deprecatum notat, -Werror structuram frangeret); suita currit, relatio ASan (prima
# fractura, abortat) et UBSan (omnes sedes 'runtime error' singulae,
# pergit). Probationes VIRIDES quae memoriam corrumpunt hic rubescunt.
#   SANITAS S  status(sana|ASAN|UBSAN|ASAN+UBSAN|RUBRA) exitus_suitae
#   UB      S  sedes: nuntius
#   ASAN    S  genus  locus_primus_arboris
#
# Usus: ./tools/tegmen_probationum.sh [-machina] [-sanitare] [-mora S]
#       S... | -omnes
# Exitus: 0 relatio · 2 usus, llvm-cov absens, substitutio vexillorum
#         defecit
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
MACHINA=0; MORA=300; OMNES=0; SANITARE=0; BIBLIOTHECAE=()
while [ $# -gt 0 ]; do
    case "$1" in
        -machina) MACHINA=1 ;;
        -mora) shift; MORA="${1:-300}" ;;
        -omnes) OMNES=1 ;;
        -sanitare) SANITARE=1 ;;
        -*) echo "usus: $0 [-machina] [-mora S] S... | -omnes" >&2; exit 2 ;;
        *) BIBLIOTHECAE+=("$1") ;;
    esac
    shift
done
if [ "$OMNES" -eq 1 ]; then
    for h in include/*.h; do
        s=$(basename "$h" .h)
        [ -f "lib/$s.c" ] && BIBLIOTHECAE+=("$s")
    done
fi
[ "${#BIBLIOTHECAE[@]}" -gt 0 ] || { echo "usus: $0 [-machina] [-mora S] S... | -omnes" >&2; exit 2; }
[ -x bin/aedilis ] || { echo "tegmen: bin/aedilis deest (./tools/aedilis_struere.sh)" >&2; exit 2; }
LLVM_COV=$(xcrun --find llvm-cov 2>/dev/null) || { echo "tegmen: llvm-cov deest (xcrun)" >&2; exit 2; }
LLVM_PROFDATA=$(xcrun --find llvm-profdata 2>/dev/null) || { echo "tegmen: llvm-profdata deest" >&2; exit 2; }
GENUS=tegmen
VEXILLA_NOVA="-O0 -g -fprofile-instr-generate -fcoverage-mapping"
if [ "$SANITARE" -eq 1 ]; then
    GENUS=tegmen_sanitas
    VEXILLA_NOVA="-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined -Wno-deprecated-declarations"
fi
OBIECTA="$RADIX/build/$GENUS/obiecta"
mkdir -p "$OBIECTA"

pc () { [ "$2" -gt 0 ] && echo "$((100 * $1 / $2))%" || echo "-"; }

_sine () {
    if [ "$MACHINA" -eq 1 ]; then printf 'SINE\t%s\t%s\n' "$1" "$2"; else printf '  %-24s %s\n' "$1" "$2"; fi
}

if [ "$MACHINA" -eq 0 ] && [ "$SANITARE" -eq 0 ]; then
    printf '  %-24s %14s %12s %12s  %s\n' "bibliotheca" "lineae" "functiones" "rami" "suita"
fi
for s in "${BIBLIOTHECAE[@]}"; do
    suita="probationes/probatio_$s.c"
    fons="lib/$s.c"
    [ -f "$fons" ] || { _sine "$s" "lib/$s.c deest"; continue; }
    [ -f "$suita" ] || { _sine "$s" "sine suita ($suita)"; continue; }
    B="$RADIX/build/$GENUS/$s"
    rm -rf "$B"; mkdir -p "$B"
    if ! bin/aedilis "$suita" --exitus "$B" --obiecta "$OBIECTA" > "$B/aedilis.log" 2>&1; then
        _sine "$s" "aedilis fracta: $(grep -m1 -E 'RECUSAT|error' "$B/aedilis.log" | cut -c1-100)"; continue
    fi
    sed -e "s/^VEXILLA=\"\\(.*\\) -O2 -g\"\$/VEXILLA=\"\\1 $VEXILLA_NOVA\"/" \
        "$B/struere.sh" > "$B/struere_tegmen.sh"
    if ! grep -qF -- " $VEXILLA_NOVA\"" "$B/struere_tegmen.sh"; then
        echo "tegmen: substitutio vexillorum defecit ($B/struere.sh: linea VEXILLA forma nova?) - nihil mensuratum" >&2
        exit 2
    fi
    if ! sh "$B/struere_tegmen.sh" > "$B/struere.log" 2>&1; then
        _sine "$s" "structura fracta: $(grep -m1 -E 'error|Undefined' "$B/struere.log" | cut -c1-100)"; continue
    fi
    exe="$B/probatio_$s"
    [ -x "$exe" ] || { _sine "$s" "binarium suitae absens"; continue; }
    if [ "$SANITARE" -eq 1 ]; then
        ASAN_OPTIONS="detect_leaks=0:abort_on_error=0" UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=0" \
            perl -e 'alarm shift; exec @ARGV' "$MORA" "$exe" > "$B/suita.log" 2>&1
        rc=$?
        ub=$(grep -E 'runtime error:' "$B/suita.log" | sed -E "s|^$RADIX/||" | sort -u)
        asan=$(grep -m1 -oE 'ERROR: AddressSanitizer: [a-z-]+' "$B/suita.log" | sed 's/ERROR: AddressSanitizer: //')
        locus=$(grep -A40 'ERROR: AddressSanitizer' "$B/suita.log" \
            | grep -m1 -oE 'in [A-Za-z_][A-Za-z0-9_]* [A-Za-z0-9_]+\.[cm]:[0-9]+' | sed 's/^in //' || true)
        status=sana
        [ -n "$asan" ] && status=ASAN
        [ -n "$ub" ] && status=$([ -n "$asan" ] && echo ASAN+UBSAN || echo UBSAN)
        [ "$status" = sana ] && [ "$rc" -ne 0 ] && status=RUBRA
        if [ "$MACHINA" -eq 1 ]; then
            printf 'SANITAS\t%s\t%s\t%s\n' "$s" "$status" "$rc"
            [ -n "$ub" ] && echo "$ub" | while IFS= read -r x; do printf 'UB\t%s\t%s\n' "$s" "$x"; done
            [ -n "$asan" ] && printf 'ASAN\t%s\t%s\t%s\n' "$s" "$asan" "$locus"
        else
            printf '  %-24s %-11s exitus %s\n' "$s" "$status" "$rc"
            [ -n "$asan" ] && echo "      ASan: $asan @ $locus"
            [ -n "$ub" ] && echo "$ub" | head -5 | sed 's/^/      UB: /'
        fi
        continue
    fi
    LLVM_PROFILE_FILE="$B/suita.profraw" perl -e 'alarm shift; exec @ARGV' "$MORA" "$exe" > "$B/suita.log" 2>&1
    rc=$?
    [ -s "$B/suita.profraw" ] || { _sine "$s" "nulla mensura (suita exitus $rc, termino $MORA s?)"; continue; }
    "$LLVM_PROFDATA" merge -sparse "$B/suita.profraw" -o "$B/suita.profdata" 2> "$B/profdata.log" \
        || { _sine "$s" "llvm-profdata fracta"; continue; }
    "$LLVM_COV" report "$exe" -instr-profile="$B/suita.profdata" "$fons" > "$B/relatio.txt" 2>&1
    "$LLVM_COV" report -show-functions "$exe" -instr-profile="$B/suita.profdata" "$fons" > "$B/functiones.txt" 2>&1
    "$LLVM_COV" show "$exe" -instr-profile="$B/suita.profdata" "$fons" > "$B/lineae.txt" 2>&1
    # ordo report: Regions Miss Cover Functions Miss Cover Lines Miss Cover Branches Miss Cover
    read -r lt l ft f bt b < <(awk -v n="/$fons" 'substr($1, length($1) - length(n) + 1) == n {
        printf "%d %d %d %d %d %d\n", $8 - $9, $8, $5 - $6, $5, $11 - $12, $11 }' "$B/relatio.txt")
    [ -n "${l:-}" ] || { _sine "$s" "llvm-cov sine linea $fons (relatio.txt)"; continue; }
    nudae=$(awk '$4 == "0.00%" && $2 > 0 && $1 !~ /^(Name|File|TOTAL|-)/ { print $1 }' "$B/functiones.txt")
    lineae=$(awk -F'|' 'NF >= 3 { gsub(/ /, "", $1); gsub(/ /, "", $2);
        if ($2 == "0" && $1 ~ /^[0-9]+$/) print $1 }' "$B/lineae.txt" \
        | awk 'NR == 1 { a = $1; p = $1; next } $1 == p + 1 { p = $1; next }
               { printf "%s%s", (o++ ? "," : ""), (a == p ? a : a "-" p); a = $1; p = $1 }
               END { if (NR) printf "%s%s\n", (o ? "," : ""), (a == p ? a : a "-" p) }')
    nota=$([ "$rc" -eq 0 ] && echo "viridis" || echo "RUBRA (exitus $rc)")
    if [ "$MACHINA" -eq 1 ]; then
        printf 'TEGMEN\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$s" "$lt" "$l" "$ft" "$f" "$bt" "$b" "$rc"
        for x in $nudae; do printf 'NUDA\t%s\t%s\n' "$s" "$x"; done
        [ -n "$lineae" ] && printf 'LINEAE\t%s\t%s\n' "$s" "$lineae"
    else
        printf '  %-24s %5s %8s %5s %6s %5s %6s  %s\n' "$s" "$(pc "$lt" "$l")" "$lt/$l" \
            "$(pc "$ft" "$f")" "$ft/$f" "$(pc "$bt" "$b")" "$bt/$b" "$nota"
        [ -n "$nudae" ] && echo "      nudae: $(echo $nudae)"
    fi
done
exit 0
