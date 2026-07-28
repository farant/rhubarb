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

TABULA_DIRENT="\
d_ino 0 8
d_seekoff 8 8
d_reclen 16 2
d_namlen 18 2
d_type 20 1
d_name 21 1024"

TABULA_TERMIOS="\
c_iflag 0 8
c_oflag 8 8
c_cflag 16 8
c_lflag 24 8
c_cc 32 20
c_ispeed 56 8
c_ospeed 64 8"

TABULA_ADDRINFO="\
ai_flags 0 4
ai_family 4 4
ai_socktype 8 4
ai_protocol 12 4
ai_addrlen 16 4
ai_canonname 24 8
ai_addr 32 8
ai_next 40 8"

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
    # Unda 2 (hospitium): typi + valores macrorum fossarum/signorum
    echo "IUNGO_ASSERTA(sizeof(pid_t) == 4);"
    echo "IUNGO_ASSERTA(sizeof(ssize_t) == 8);"
    echo "IUNGO_ASSERTA(sizeof(socklen_t) == 4);"
    echo "IUNGO_ASSERTA(sizeof(useconds_t) == 4);"
    echo "IUNGO_ASSERTA(SIGINT == 2 && SIGKILL == 9 && SIGTERM == 15);"
    echo "IUNGO_ASSERTA(SOL_SOCKET == 0xffff);"
    echo "IUNGO_ASSERTA(SO_REUSEADDR == 0x0004);"
    echo "IUNGO_ASSERTA(SO_KEEPALIVE == 0x0008);"
    echo "IUNGO_ASSERTA(SO_NOSIGPIPE == 0x1022);"
    echo "IUNGO_ASSERTA(IPPROTO_TCP == 6);"
    echo "IUNGO_ASSERTA(TCP_NODELAY == 0x01);"
    # Unda 3 (re-pinnatio 2026-07-27): dirent + mman + termios +
    # ioctl + utime + sigaction + errno
    echo "IUNGO_ASSERTA(sizeof(struct dirent) == 1048);"
    while read -r campus off mensura; do
        echo "IUNGO_ASSERTA(OFF(dirent, $campus) == $off);"
        echo "IUNGO_ASSERTA(sizeof(((struct dirent*)0)->$campus) == $mensura);"
    done <<< "$TABULA_DIRENT"
    echo "IUNGO_ASSERTA(DT_DIR == 4);"
    echo "IUNGO_ASSERTA(PROT_READ == 0x01 && PROT_WRITE == 0x02);"
    echo "IUNGO_ASSERTA(MAP_PRIVATE == 0x0002 && MAP_ANON == 0x1000);"
    echo "IUNGO_ASSERTA(sizeof(struct termios) == 72);"
    while read -r campus off mensura; do
        echo "IUNGO_ASSERTA(OFF(termios, $campus) == $off);"
        echo "IUNGO_ASSERTA(sizeof(((struct termios*)0)->$campus) == $mensura);"
    done <<< "$TABULA_TERMIOS"
    echo "IUNGO_ASSERTA(TCSAFLUSH == 2 && ISIG == 0x00000080);"
    echo "IUNGO_ASSERTA(VMIN == 16 && VTIME == 17);"
    echo "IUNGO_ASSERTA(VINTR == 8 && VQUIT == 9);"
    echo "IUNGO_ASSERTA(_POSIX_VDISABLE == 0xff);"
    echo "IUNGO_ASSERTA(SIGBUS == 10 && SIGALRM == 14);"
    echo "IUNGO_ASSERTA(OFF(stat, st_mtime) == 48 && OFF(stat, st_atime) == 32);"
    echo "IUNGO_ASSERTA(O_WRONLY == 0x0001 && O_CREAT == 0x0200 && O_TRUNC == 0x0400);"
    # Promotio blocorum tcp_posix/reactor (01KYB2Z7XA)
    echo "IUNGO_ASSERTA(sizeof(struct sockaddr) == 16);"
    echo "IUNGO_ASSERTA(OFF(sockaddr, sa_family) == 1 && OFF(sockaddr, sa_data) == 2);"
    echo "IUNGO_ASSERTA(sizeof(struct in_addr) == 4);"
    echo "IUNGO_ASSERTA(sizeof(struct sockaddr_in) == 16);"
    echo "IUNGO_ASSERTA(OFF(sockaddr_in, sin_port) == 2 && OFF(sockaddr_in, sin_addr) == 4 && OFF(sockaddr_in, sin_zero) == 8);"
    echo "IUNGO_ASSERTA(sizeof(struct addrinfo) == 48);"
    while read -r campus off mensura; do
        echo "IUNGO_ASSERTA(OFF(addrinfo, $campus) == $off);"
        echo "IUNGO_ASSERTA(sizeof(((struct addrinfo*)0)->$campus) == $mensura);"
    done <<< "$TABULA_ADDRINFO"
    echo "IUNGO_ASSERTA(AF_UNSPEC == 0 && AF_INET == 2 && SOCK_STREAM == 1);"
    echo "IUNGO_ASSERTA(SO_RCVTIMEO == 0x1006 && SO_SNDTIMEO == 0x1005);"
    echo "IUNGO_ASSERTA(INADDR_ANY == 0u);"
    echo "IUNGO_ASSERTA(sizeof(struct pollfd) == 8);"
    echo "IUNGO_ASSERTA(OFF(pollfd, events) == 4 && OFF(pollfd, revents) == 6);"
    echo "IUNGO_ASSERTA(sizeof(nfds_t) == 4);"
    echo "IUNGO_ASSERTA(POLLIN == 0x0001 && POLLOUT == 0x0004);"
    echo "IUNGO_ASSERTA(POLLERR == 0x0008 && POLLHUP == 0x0010);"
    echo "IUNGO_ASSERTA(sizeof(struct winsize) == 8);"
    echo "IUNGO_ASSERTA(OFF(winsize, ws_row) == 0 && OFF(winsize, ws_col) == 2);"
    echo "IUNGO_ASSERTA(TIOCGWINSZ == 0x40087468UL);"
    echo "IUNGO_ASSERTA(sizeof(struct utimbuf) == 16);"
    echo "IUNGO_ASSERTA(OFF(utimbuf, actime) == 0 && OFF(utimbuf, modtime) == 8);"
    echo "IUNGO_ASSERTA(sizeof(struct sigaction) == 16);"
    echo "IUNGO_ASSERTA(OFF(sigaction, sa_handler) == 0);"
    echo "IUNGO_ASSERTA(sizeof(((struct sigaction*)0)->sa_handler) == 8);"
    echo "IUNGO_ASSERTA(OFF(sigaction, sa_mask) == 8 && OFF(sigaction, sa_flags) == 12);"
    echo "IUNGO_ASSERTA(sizeof(sigset_t) == 4);"
    echo "IUNGO_ASSERTA(SIGCONT == 19 && SIGTSTP == 18 && SIGWINCH == 28);"
    echo "IUNGO_ASSERTA(EEXIST == 17);"
    echo "IUNGO_ASSERTA(EINTR == 4 && EAGAIN == 35 && EISCONN == 56);"
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
    echo "#include <sys/socket.h>"
    echo "#include <sys/wait.h>"
    echo "#include <netinet/in.h>"
    echo "#include <netinet/tcp.h>"
    echo "#include <signal.h>"
    echo "#include <unistd.h>"
    echo "#include <dirent.h>"
    echo "#include <sys/mman.h>"
    echo "#include <termios.h>"
    echo "#include <sys/ioctl.h>"
    echo "#include <utime.h>"
    echo "#include <errno.h>"
    echo "#include <stdio.h>"
    echo "#include <fcntl.h>"
    echo "#include <poll.h>"
    echo "#include <arpa/inet.h>"
    echo "#include <netdb.h>"
    echo "$PRAEAMBULUM"
    emittere_asserta
} > "$GEN_DIR/verum.c"

# TU nostrum (praeludium = typi quos systema_c89.h praebet)
{
    echo "typedef unsigned long size_t;"
    echo "typedef long time_t;"
    echo "typedef struct __sFILE FILE;"
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
    echo "auspex_posix: FORMAE CERTIFICATAE (stat 144 + timeval/timespec 16 + dirent 1048 + termios 72 + winsize/utimbuf/sigaction + macra)"
    exit 0
fi
exit 1
