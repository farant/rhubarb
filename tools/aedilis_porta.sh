#!/bin/bash

# tools/aedilis_porta.sh - PORTA CORPORIS aedilis (Phasis B)
#
# Quaeque probatio radicis: (1) derivatio + emissio scriptorum
# (bin/aedilis), (2) structura per scriptum emissum, (3)
# differentia-clausurae contra clang -MM (unio super TU clausurae).
# Porta per codicem exitus; summarium demum.
#
# CURSUS PROBATIONUM NON HIC (fabrica-7 T7): probationes ipsae membra
# 'probationes_radicis' sunt (porta radix per fabricam) - indices manu
# (GUI, reticulares, repositorium vivum) nunc annotationes facultatis
# in fontibus, scripta in build/ debita membrorum. Haec porta CORPUS
# aedilis probat: derivatio, scriptum emissum, clausura vera.
#
# Praesupponit: bin/aedilis paratum (./tools/aedilis_struere.sh)
# et artificia generata recentia (suite semel cursa - capsulae
# speculi, schemata, etc.).
# <porta titulus="aedilis" gradus="fusio" instrumentum="bin/aedilis" tegit="include/* lib/*.c lib/*.m probationes/probatio_*.c tools/aedilis_struere.sh"/>

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
mkdir -p build/aedilis


numerus=0
fracta=""
initium=$(date +%s)

# MEMORIA ORACULI (2026-10-02): clang -MM effusio per fontem SEMEL per
# cursum (CCCCCCCXXIX invocationes pro CCI fontibus olim; differentia
# CLIII s -> LXXIV s, effusiones CXCIX probationum octetim eaedem).
# Directorium RECENS omni cursu, deletum in fine: arbor intra cursum
# gelata est, inter cursus non - memoria vetus capita falsa daret
# (planta: introitus fictus -> 'ORACULUM SOLUM' fictum).
MEMORIA_ORACULI="$(mktemp -d)"
# THESAURUS (fabrica plan 2 T3): cursus portae = generatio una
# (purgatio generationes ultimas servat, non processus aedilis)
export THESAURUS_GENERATIO="${THESAURUS_GENERATIO:-$(date -u +%Y%m%dT%H%M%S)-porta-aedilis-$$}"
trap 'rm -rf "$MEMORIA_ORACULI"' EXIT

for f in probationes/probatio_*.c; do
    basis="$(basename "$f" .c)"
    case "$basis" in
        *_benchmark) continue ;;
    esac
    numerus=$((numerus + 1))

    if ! ./bin/aedilis "$f" --thesaurus build/aedilis/obiecta > /dev/null 2>"build/aedilis/porta_$basis.err"; then
        echo "FRACTA (derivatio): $basis"
        head -3 "build/aedilis/porta_$basis.err"
        fracta="$fracta $basis(derivatio)"
        continue
    fi
    if ! bash "build/aedilis/$basis/struere.sh" > /dev/null 2>"build/aedilis/porta_$basis.err"; then
        echo "FRACTA (structura): $basis"
        tail -5 "build/aedilis/porta_$basis.err"
        fracta="$fracta $basis(structura)"
        continue
    fi
    if ! ./bin/aedilis "$f" --differentia --memoria-oraculi "$MEMORIA_ORACULI" \
            --thesaurus build/aedilis/obiecta \
            > "build/aedilis/porta_$basis.diff" 2>&1; then
        echo "DIFFERENTIA: $basis"
        grep -E "NOS SOLI|ORACULUM SOLUM|RECUSAT" \
            "build/aedilis/porta_$basis.diff" | head -5
        fracta="$fracta $basis(differentia)"
        continue
    fi
    echo "bona: $basis"
done

# NEXUS PURUS (eventus A1b, 2026-10-01): capita <aedilis nexus="purus"/>
# clausuram sine regula nexus (frameworks) servant; catena fracta
# nominatur. Nullum promittens = porta mortua -> fracta quoque.
# PLURES (2026-10-22): --enumerare --plures = concatenatio vocationum
# singularum, manifesta eadem (praeter 'generatum') - fontes varii:
# simplex, cum Objective-C (vicus), instrumentum, duplex intra listam
PLURES_D="$(mktemp -d)"
printf '%s\n' probationes/probatio_piscina.c probationes/probatio_vicus_saltus.c \
    tools/generare.c probationes/probatio_stml.c probationes/probatio_piscina.c \
    > "$PLURES_D/lista"
mkdir -p "$PLURES_D/singula" "$PLURES_D/plures"
: > "$PLURES_D/singula.txt"
while IFS= read -r f; do
    ./bin/aedilis "$f" --enumerare --thesaurus build/aedilis/obiecta \
        --manifestum "$PLURES_D/singula/$(echo "$f" | sed 's|/|__|g').stml" \
        >> "$PLURES_D/singula.txt" 2>/dev/null
done < "$PLURES_D/lista"
: > "$PLURES_D/partes_singula.tsv"
: > "$PLURES_D/aristae_singula.tsv"
while IFS= read -r f; do
    ./bin/aedilis "$f" --partes >> "$PLURES_D/partes_singula.tsv" 2>/dev/null
    ./bin/aedilis "$f" --aristae >> "$PLURES_D/aristae_singula.tsv" 2>/dev/null
done < "$PLURES_D/lista"
./bin/aedilis --plures "$PLURES_D/lista" \
    --partes --partes-ad "$PLURES_D/partes_plures.tsv" \
    --aristae --aristae-ad "$PLURES_D/aristae_plures.tsv" > /dev/null 2>&1
if    cmp -s "$PLURES_D/partes_singula.tsv" "$PLURES_D/partes_plures.tsv" \
   && cmp -s "$PLURES_D/aristae_singula.tsv" "$PLURES_D/aristae_plures.tsv"; then
    echo "bona: --plures --partes/--aristae = singula"
else
    echo "FRACTA (plures partes/aristae): a vocationibus singulis differt"
    fracta="$fracta plures_partes"
fi
if    ./bin/aedilis --enumerare --plures "$PLURES_D/lista" \
          --thesaurus build/aedilis/obiecta --manifesta "$PLURES_D/plures" \
          > "$PLURES_D/plures.txt" 2>/dev/null \
   && cmp -s "$PLURES_D/singula.txt" "$PLURES_D/plures.txt" \
   && diff -r -I 'generatum="[0-9]*"' "$PLURES_D/singula" "$PLURES_D/plures" > /dev/null; then
    echo "bona: --plures = singula ($(wc -l < "$PLURES_D/plures.txt" | tr -d ' ') lineae)"
else
    echo "FRACTA (plures): --plures a vocationibus singulis differt"
    fracta="$fracta plures"
fi
rm -rf "$PLURES_D"
if ! ./bin/aedilis --nexus-purus 2>&1; then
    echo "FRACTA (nexus purus)"
    fracta="$fracta nexus_purus"
fi

finis=$(date +%s)
echo ""
echo "PORTA AEDILIS: $numerus probationes, $((finis - initium))s"
if [ -n "$fracta" ]; then
    echo "FRACTAE:$fracta"
    exit 1
fi
echo "OMNES BONAE"
exit 0
