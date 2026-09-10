#!/bin/bash
# metire.sh <exitus.tsv> : oraculum -machina super plagulas VIII (ambitus a vocante)
OUT="$1"
: > "$OUT"
for f in oratio/probationes/fixa/ud/la_circse-ud-test.conllu oratio/probationes/fixa/ud/la_llct-ud-dev.conllu oratio/probationes/fixa/ud/la_llct-ud-test.conllu oratio/probationes/fixa/ud/en_ewt-ud-dev.conllu oratio/build/ud/la_ittb-ud-test.conllu oratio/build/ud/la_perseus-ud-test.conllu oratio/build/ud/la_proiel-ud-test.conllu oratio/build/ud/la_udante-ud-test.conllu; do
    ./oratio/oraculum.sh "$f" -machina >> "$OUT" 2>&1
done
echo "FINIS $(grep -c SUMMA "$OUT") summae"
