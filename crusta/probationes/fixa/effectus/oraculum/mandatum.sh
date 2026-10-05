#!/bin/bash
# oraculum fixum: mandata per tabulam (SIP: argv) (effectus T5)
mkdir -p build
cat data/a.txt > /dev/null
head -1 data/b.txt > /dev/null
cp data/a.txt build/c.txt
sort -o build/s.txt data/a.txt
grep -c a data/a.txt data/b.txt > /dev/null
perl -e "print 1" > /dev/null
