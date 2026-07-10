#!/bin/bash

# officina/auspex_posix.sh - certificatio formarum systema_posix.h
# (exemplar auspex: clang consentit aut dissentit)
#
# Duae TU eandem TABULAM assertionum compilant:
#   TU verum:    capita VERA (<sys/stat.h>, <sys/time.h>)
#   TU nostrum:  systema_posix.h + praeludium (typi systema_c89.h)
# Ambae virides => formae nostrae ≡ formae hospitis (sizeof + offset
# omnium camporum + valores macrorum). Forma falsa = corruptio
# memoriae tacita interpretata - haec porta ante machinulam stat.
#
# Usus: ./officina/auspex_posix.sh   # ex radice repositorii

set -u
OFF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$OFF_DIR/.." && pwd)"
cd "$RADIX_DIR"

GEN_DIR="$OFF_DIR/build/auspex_posix"
mkdir -p "$GEN_DIR"

# TABULA UNA - fons veritatis assertionum (campus offset mensura)
TABULA_STAT="\
st_dev 0 4
st_mode 4 2
st_nlink 6 2
st_ino 8 8
st_uid 16 4
st_gid 20 4
st_rdev 24 4
st_atimespec 32 16
st_mtimespec 48 16
st_ctimespec 64 16
st_birthtimespec 80 16
st_size 96 8
st_blocks 104 8
st_blksize 112 4
st_flags 116 4
st_gen 120 4"

emittere_asserta() {
    echo "IUNGO_ASSERTA(sizeof(struct stat) == 144);"
    echo "IUNGO_ASSERTA(sizeof(struct timeval) == 16);"
    echo "IUNGO_ASSERTA(sizeof(struct timespec) == 16);"
    echo "IUNGO_ASSERTA(OFF(timeval, tv_sec) == 0);"
    echo "IUNGO_ASSERTA(sizeof(((struct timeval*)0)->tv_sec) == 8);"
    echo "IUNGO_ASSERTA(OFF(timeval, tv_usec) == 8);"
    echo "IUNGO_ASSERTA(sizeof(((struct timeval*)0)->tv_usec) == 4);"
    echo "IUNGO_ASSERTA(OFF(timespec, tv_nsec) == 8);"
    while read -r campus off mensura; do
        echo "IUNGO_ASSERTA(OFF(stat, $campus) == $off);"
        echo "IUNGO_ASSERTA(sizeof(((struct stat*)0)->$campus) == $mensura);"
    done <<< "$TABULA_STAT"
    echo "IUNGO_ASSERTA(S_IFMT  == 0170000);"
    echo "IUNGO_ASSERTA(S_IFDIR == 0040000);"
    echo "IUNGO_ASSERTA(S_IFREG == 0100000);"
    echo "IUNGO_ASSERTA(S_IRUSR == 0400);"
    echo "IUNGO_ASSERTA(S_IWUSR == 0200);"
    echo "IUNGO_ASSERTA(S_ISDIR(0040755) && !S_ISDIR(0100644));"
    echo "IUNGO_ASSERTA(S_ISREG(0100644) && !S_ISREG(0040755));"
}

PRAEAMBULUM='#define IUNGO2(a,b) a##b
#define IUNGO(a,b) IUNGO2(a,b)
#define IUNGO_ASSERTA(cond) \
    typedef char IUNGO(assertum_, __LINE__)[(cond) ? 1 : -1]
#define OFF(S, C) ((unsigned long)&(((struct S*)0)->C))'

# TU verum
{
    echo "#include <sys/stat.h>"
    echo "#include <sys/time.h>"
    echo "$PRAEAMBULUM"
    emittere_asserta
} > "$GEN_DIR/verum.c"

# TU nostrum (praeludium = typi quos systema_c89.h praebet)
{
    echo "typedef unsigned long size_t;"
    echo "typedef long time_t;"
    echo "#include \"silva/fontes/systema_posix.h\""
    echo "$PRAEAMBULUM"
    emittere_asserta
} > "$GEN_DIR/nostrum.c"

BENE=1
for tu in verum nostrum; do
    if clang -std=c99 -Wall -Wextra -Werror -Wno-long-long -c \
        -I"$RADIX_DIR" "$GEN_DIR/$tu.c" -o /dev/null 2>"$GEN_DIR/$tu.error"; then
        echo "auspex_posix: TU $tu CONSENTIT"
    else
        echo "auspex_posix: TU $tu DISSENTIT:"
        head -20 "$GEN_DIR/$tu.error"
        BENE=0
    fi
done

if [ $BENE -eq 1 ]; then
    echo "auspex_posix: FORMAE CERTIFICATAE (stat 144 + timeval 16 + timespec 16 + macra)"
    exit 0
fi
exit 1
