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
