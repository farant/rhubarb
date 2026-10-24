#!/bin/bash
# tools/linux/horreum_vivum.sh - horreum VIVUM sub Linux (fiducia-linux T4)
#
# bin/horreum in vase compilat (clausura aedilis --varians posix:
# fiducia_posix, nomina_posix, tls13/bearssl) et UNAM petitionem LECTIONIS
# mittit: inspicere clavis quae non exstat (horreum-probatio/
# nusquam-linux) - responsum 404 = TLS (fasciculus systematis, BearSSL,
# indicium diffisarum) ET signatura SigV4 sub Linux valent.
#
# PROFILUM: ~/.rhubarb/horreum/<titulus>.clavis SOLUM, LECTIONI tantum
# in vas inseritur (-v ...:ro); hoc scriptum id numquam legit nec
# imprimit. Franus id ipse currit (! ./tools/linux/horreum_vivum.sh
# digitalocean). -vestigium nullum (vestigium hospitem rationarii R2
# tenet). Nihil scribitur in situlam.
#
# Usus: ./tools/linux/horreum_vivum.sh TITULUS [-musl|-glibc]
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2
[ $# -ge 1 ] || { echo "usus: $0 TITULUS [-musl|-glibc]" >&2; exit 2; }
TITULUS="$1"
case "$TITULUS" in
    *[!a-z0-9_-]*|"") echo "horreum_vivum: titulus malus" >&2; exit 2 ;;
esac
IMAGO=glibc
[ "${2:-}" = "-musl" ] && IMAGO=musl
TABELLA="$HOME/.rhubarb/horreum/$TITULUS.clavis"
[ -f "$TABELLA" ] || { echo "horreum_vivum: profilum $TITULUS deest" >&2; exit 2; }

RADICES="$(bin/aedilis --radices 2>/dev/null | sed 's/^/-I/' | tr '\n' ' ')"
PARTES=$(bin/aedilis tools/horreum.c --varians posix --partes 2>/dev/null \
    | awk -F'\t' '$1=="O" {printf "%s ", $2}')
VENDITI=$(bin/aedilis tools/horreum.c --varians posix --partes 2>/dev/null \
    | awk -F'\t' '$1=="V" {printf "%s ", $2}')
[ -n "$PARTES" ] || { echo "horreum_vivum: clausura vacua (aedilis)" >&2; exit 2; }

docker run --rm -v "$PWD":/rhubarb:ro -v "$TABELLA":/profilum/$TITULUS.clavis:ro \
    -w /rhubarb "rhubarb-proba-$IMAGO" sh -c "
VOBJ=''
for v in $VENDITI; do b=\$(basename \$v .c); gcc -O2 -DBR_ENABLE_INTRINSICS=1 -c \$v -o /tmp/v_\$b.o || exit 2; VOBJ=\"\$VOBJ /tmp/v_\$b.o\"; done
gcc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -O2 $RADICES tools/horreum.c $PARTES \$VOBJ -lm -o /tmp/horreum || exit 2
echo 'horreum_vivum ($IMAGO): inspicere horreum-probatio/nusquam-linux'
/tmp/horreum -profilum /profilum/$TITULUS.clavis inspicere horreum-probatio/nusquam-linux
echo \"exitus \$? (exspectatum: I cum 404 - TLS et signatura valent)\"
"
