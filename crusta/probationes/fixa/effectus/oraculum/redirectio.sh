#!/bin/bash
# oraculum fixum: redirectiones (effectus T5)
mkdir -p build
cat < data/a.txt > /dev/null
echo x > build/o.txt
echo y >> build/o.txt
while read -r l; do :; done < data/b.txt
