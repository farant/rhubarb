#!/bin/bash
# oraculum fixum: lectio ante scripturam (status trans cursus)
mkdir -p build
x="$(cat build/status.txt 2>/dev/null)"
read -r y < build/status.txt 2> /dev/null
echo "z${x}${y}" > build/status.txt
