#!/bin/sh
# tools/linux/percursus.sh - percursus compilationis INTRA vas
# (porta_linux.sh hoc vocat; solum non currere - /ex scribit)
#
# Omnes lib/*.c + gesta/fontes/*.c sub gcc, vexilla domus minus
# clang-solum (-fbracket-depth) plus decreta portae:
#   -Wno-type-limits  (decretum 2026-08-03: idioma custodiae
#                      bilateralis paritas clang - gcc bracchium
#                      mortuum insignati flagat quod stilus noster
#                      consultus est)
# Fractae in TSV; comparatio contra pinnas HOSPITIS fit.
set -u

FLAGS="-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -Iinclude -Igesta/fontes"
SUMMA=/ex/percursus_summa.tsv
: > "$SUMMA"
mkdir -p /ex/errores

{ gcc --version | head -1; } > /ex/versiones.txt 2>&1

for f in lib/*.c gesta/fontes/*.c; do
  b=$(echo "$f" | tr '/' '_')
  out="/ex/errores/${b}.txt"
  gcc $FLAGS -c "$f" -o /tmp/o.o > "$out" 2>&1
  ex=$?
  ner=$(grep -c 'error:' "$out")
  printf '%s\t%s\t%s\n' "$f" "$ex" "$ner" >> "$SUMMA"
  [ "$ex" = "0" ] && rm -f "$out"
done

awk -F'\t' '{t++; if($2==0) s++} END {printf "%d/%d mundae\n", s, t}' "$SUMMA"
