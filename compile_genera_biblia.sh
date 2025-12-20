#!/bin/bash
# compile_genera_biblia.sh - Compile the Bible generator tool
#
# Usage: ./compile_genera_biblia.sh
#        ./bin/genera_biblia events.db > lib/biblia_dr.c

mkdir -p bin

gcc -o bin/genera_biblia tools/genera_biblia.c -lsqlite3

if [ $? -eq 0 ]; then
    echo "Compiled: bin/genera_biblia"
    echo "Usage: ./bin/genera_biblia events.db > lib/biblia_dr.c"
else
    echo "Compilation failed"
    exit 1
fi
