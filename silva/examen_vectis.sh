#!/bin/bash

# silva/examen_vectis.sh - VECTIS DIFFERENTIALIS M4a (chunk D)
#
# Modus ordinarius: ① percursus fugarum oraculi (fixturae C99/GNU
#   contra chordam sigillatam - omnes REICI debent); ② corpus
#   invalidum: examen REICE ad pinnas EXSPECTA (linea:CODEX) ET
#   oraculum clang consentiens.
# Modus -corpus: latus examinis super corpus verum - [verdictum
#   REICE] percursus contra tabulam exclusionum pinnatam.
# Exit: 0 = vectis tenet | 1 = discrepantia
#
# Chorda oraculi SIGILLATA (officina-m4a-spec.md §V); versio clang
# scribitur (linea acceptationis inter versiones movetur!).

set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"

FIXA="$SILVA_DIR/probationes/fixa/examinis"
declare -a ORACULUM=(
    clang -x c -std=c89 -pedantic-errors
    -Wno-long-long -Wno-overlength-strings
    -Werror=c99-designator
    -Wno-error=implicit-function-declaration
    -Wno-strict-prototypes
    -ferror-limit=0 -fno-caret-diagnostics -fsyntax-only
)

echo "oraculum: $(clang --version | head -1)"
fracta=0

# ① percursus fugarum: C99/GNU contra oraculum - omnes REICI
echo "--- percursus fugarum oraculi ---"
for f in "$FIXA"/fugae/*.fuga; do
    if "${ORACULUM[@]}" "$f" >/dev/null 2>&1; then
        echo "  FUGA: $(basename "$f") oraculo ACCEPTUM (C99/GNU!)"
        fracta=1
    fi
done
[ "$fracta" -eq 0 ] && echo "  fugae: 0 (omnes reiectae)"

# tabula nominum codicum ex capite (numerus = ordo enumerationis)
MAPPA="$SILVA_DIR/build/codices_mappa.txt"
grep -o 'EXAMEN_CODEX_[A-Z_]*' "$SILVA_DIR/fontes/silva_c89_semantica.h" \
    | awk '!visa[$0]++ && $0 != "EXAMEN_CODEX_NUMERUS" {print NR-1"\t"$0}' \
    > "$MAPPA"

# ② corpus invalidum: examen ad pinnas + oraculum consentiens
echo "--- corpus invalidum (pinnae EXSPECTA) ---"
for f in "$FIXA"/*.invalidum; do
    basis="$(basename "$f")"
    exspecta="$(grep -o 'EXSPECTA [0-9]*:[A-Z_]*' "$f" \
        | sed 's/EXSPECTA //')"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "REICE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum (non REICE)"
        fracta=1
        continue
    fi
    # quaeque pinna: (linea, codex-nomen) in effuso -machina
    for pinna in $exspecta; do
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $5==c {inventum=1} END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
    done
    # oraculum consentiens (exclusiones nominatae infra)
    case "$basis" in
        *) if "${ORACULUM[@]}" "$f" >/dev/null 2>&1; then
               echo "  DISCREPANTIA: $basis oraculum ACCIPIT," \
                    "examen REICIT"
               fracta=1
           fi ;;
    esac
done

# ②b corpus domesticum: legale C89 (verdictum ACCIPE manet) sed
#   ordines DOMESTICUM ad pinnas; oraculum clang easdem lineas monet
#   ET numerus monitorum == numerus pinnarum (aequalitas honesta).
#   Vexillum oraculi per fixturam: directivum "ORACULUM -W..." in
#   commentario; ordinarius -Wsign-conversion.
echo "--- corpus domesticum (pinnae EXSPECTA + oraculum) ---"
declare -a ORACULUM_BASIS_D=(
    clang -x c -std=c89 -pedantic
    -Wno-long-long -fno-caret-diagnostics -fsyntax-only
)
for f in "$FIXA"/*.domesticum; do
    [ -e "$f" ] || continue
    basis="$(basename "$f")"
    exspecta="$(grep -o 'EXSPECTA [0-9]*:[A-Z_]*' "$f" \
        | sed 's/EXSPECTA //')"
    vexillum="$(grep -o 'ORACULUM -W[a-z-]*' "$f" | head -1 \
        | sed 's/ORACULUM //')"
    [ -z "$vexillum" ] && vexillum="-Wsign-conversion"
    exemplar="${vexillum#-W}"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "ACCIPE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum" \
             "(non ACCIPE - legale C89!)"
        fracta=1
        continue
    fi
    monita="$("${ORACULUM_BASIS_D[@]}" "$vexillum" "$f" 2>&1)"
    for pinna in $exspecta; do
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $4=="domesticum" && $5==c {inventum=1}
             END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
        if ! printf '%s\n' "$monita" | grep -q ":$linea:.*$exemplar"; then
            echo "  ORACULUM DISSENTIT: $basis linea $linea" \
                 "(clang non monet)"
            fracta=1
        fi
    done
    n_pinnae="$(printf '%s\n' "$exspecta" | grep -c . || true)"
    n_monita="$(printf '%s\n' "$monita" | grep -c "$exemplar" \
        || true)"
    if [ "$n_pinnae" != "$n_monita" ]; then
        echo "  DISCREPANTIA NUMERI: $basis pinnae=$n_pinnae" \
             "monita oraculi=$n_monita"
        fracta=1
    fi
done

# ②c corpus severum: gradus SEVERI (supra oraculum - clang SILET ad
#   pinnas, id ipsum proprietas est); verdictum ACCIPE; ordines
#   codicum pinnatorum == pinnae exacte (lineae toleratae QUIETAE);
#   IRRITUM 0. Vexillum oraculi per directivum "ORACULUM -W..." ut
#   supra.
echo "--- corpus severum (pinnae + oraculum inversum) ---"
for f in "$FIXA"/*.severum; do
    [ -e "$f" ] || continue
    basis="$(basename "$f")"
    exspecta="$(grep -o 'EXSPECTA [0-9]*:[A-Z_]*' "$f" \
        | sed 's/EXSPECTA //')"
    vexillum="$(grep -o 'ORACULUM -W[a-z-]*' "$f" | head -1 \
        | sed 's/ORACULUM //')"
    [ -z "$vexillum" ] && vexillum="-Wsign-conversion"
    exemplar="${vexillum#-W}"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "ACCIPE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum (non ACCIPE)"
        fracta=1
        continue
    fi
    monita="$("${ORACULUM_BASIS_D[@]}" "$vexillum" "$f" 2>&1)"
    n_pinnae=0
    codices_pinnati=""
    for pinna in $exspecta; do
        n_pinnae=$((n_pinnae + 1))
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        codices_pinnati="$codices_pinnati $numerus"
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $4=="domesticum" && $5==c {inventum=1}
             END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
        if printf '%s\n' "$monita" | grep -q ":$linea:.*$exemplar"; then
            echo "  ORACULUM FLAGRAT: $basis linea $linea" \
                 "(gradus severus supra oraculum esse debet!)"
            fracta=1
        fi
    done
    n_severa="$(printf '%s\n' "$effusum" | awk -F'\t' \
        -v cs="$codices_pinnati" \
        'BEGIN{n=split(cs,a," "); for(i=1;i<=n;i++) cc[a[i]]=1}
         ($5 in cc){k++} END{print k+0}')"
    if [ "$n_severa" != "$n_pinnae" ]; then
        echo "  DISCREPANTIA NUMERI: $basis severa=$n_severa" \
             "pinnae=$n_pinnae (tolerata flagrant?)"
        fracta=1
    fi
    n_irrita="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$5==56' | wc -l | tr -d ' ')"
    if [ "$n_irrita" != "0" ]; then
        echo "  IRRITUM: $basis $n_irrita TOLERA irrita"
        fracta=1
    fi
done

# ③ -corpus: columna verdicti percursus contra exclusiones pinnatas
if [ "${1:-}" = "-corpus" ]; then
    echo "--- corpus verum (percursus REICE vs exclusiones) ---"
    ./silva/percursus.sh -semantica 2>&1 \
        | grep 'verdictum REICE' | sed 's/\[verdictum REICE\] //' \
        | sort > "$SILVA_DIR/build/reice_currentes.txt"
    if ! diff -u "$FIXA/exclusiones.txt" \
            "$SILVA_DIR/build/reice_currentes.txt"; then
        echo "  DISCREPANTIA: REICE corporis != exclusiones pinnatae"
        fracta=1
    else
        echo "  exclusiones: $(wc -l < "$FIXA/exclusiones.txt" \
            | tr -d ' ') pinnatae, omnes notae"
    fi
fi

if [ "$fracta" -eq 0 ]; then
    echo "VECTIS TENET"
    exit 0
fi
echo "VECTIS FRACTUS"
exit 1
