#!/bin/bash
# tools/sonda_fumus.sh - porta 'sonda' (credo memoria T7): custos
# memoriae sondae per lectorem domus UNUM (bin/mensor memoria ->
# memoria_processus, compressa inclusa), non per 'top' interpretatum.
# Sondae fixae in plagula temporaria, -nudus (sine ASan: hard_rss_limit
# ASan ipse aliter prius occideret - custos NOSTER probandus est).
#   I   sonda parva transit: exitus 0, nulla CUSTODIA
#   II  sonda CCLVI MiB tenens (XX s natura) sub SONDA_MEMORIA_MB=64:
#       occisa, 'CUSTODIA: memoria excessa (vestigium N MB > 64 MB',
#       intra X s
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="sonda" tegit="tools/sonda.sh tools/sonda_fumus.sh" currit="tools/mensor.c"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
if [ ! -x bin/mensor ] || [ ! -x bin/aedilis ]; then
    echo "fumus sonda: bin/mensor aut bin/aedilis absens - nihil actum"
    exit 2
fi
# fixae INTRA arborem (build/): aedilis plagulas extra radicem recusat
mkdir -p build
T=$(mktemp -d build/sonda_fumus.XXXXXX)
trap 'rm -rf "$T" build/sonda/sonda_fumus_parva build/sonda/sonda_fumus_magna' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }

# I
cat > "$T/sonda_fumus_parva.c" <<'FONS'
#include <stdio.h>
int main(void)
{
    printf("parva\n");
    return 0;
}
FONS
./tools/sonda.sh -nudus "$T/sonda_fumus_parva.c" > "$T/i" 2>&1; r1=$?
[ "$r1" -eq 0 ] && grep -q "^parva$" "$T/i" && ! grep -q CUSTODIA "$T/i"
credo $? "I    sonda parva transit, nulla custodia (rc $r1)"

# II - tabula universalis: malloc+memset sine usu compilator delere potest
cat > "$T/sonda_fumus_magna.c" <<'FONS'
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char* tenuti[16];
int main(void)
{
    int k;
    for (k = 0; k < 16; k++)
    {
        tenuti[k] = malloc(16u << 20);
        if (tenuti[k]) { memset(tenuti[k], 0x5a, 16u << 20); }
    }
    sleep(20);
    for (k = 0; k < 16; k++)
    {
        if (tenuti[k] && tenuti[k][k] != 0x5a) { return 5; }
    }
    return 0;
}
FONS
t0=$(date +%s)
SONDA_MEMORIA_MB=64 ./tools/sonda.sh -nudus "$T/sonda_fumus_magna.c" > "$T/ii" 2>&1; r2=$?
t1=$(date +%s)
[ "$r2" -ne 0 ] && grep -qE "CUSTODIA: memoria excessa \(vestigium [0-9]+ MB > 64 MB" "$T/ii" && [ $((t1 - t0)) -lt 10 ]
credo $? "II   sonda CCLVI MiB sub LXIV occisa, vestigium nominatum ($((t1 - t0)) s, rc $r2: $(grep -o 'CUSTODIA: .*' "$T/ii" | head -1))"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus sonda: sanum (II/II)"; exit 0; fi
echo "fumus sonda: FRACTUM ($fracta)"; exit 1
