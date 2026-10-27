#!/bin/bash
# tools/linux/fiducia_vivum.sh - fiducia VIVA sub Linux (fiducia-linux T4)
#
# tools/fiducia_vivum.c in vase (glibc aut musl) compilat ut probatio.sh
# (clausura aedilis --varians posix) et CUM RETE currit contra hospites
# publicos, sub vectura utraque (RHUBARB_TLS=tls13, bearssl):
#   accipi debent: DigitalOcean Spaces, cloudflare.com (catena eadem ac
#     R2: Google Trust Services WE1 -> GTS Root R4, ECDSA), badssl.com
#   nuntiatur: r2.cloudflarestorage.com nudus - certificatum eius
#     2026-09-09 EXPIRAVIT (inventum 2026-10-22; nemo eo hospite utitur,
#     R2 verum = <rationarium>.r2... - horreum_vivum.sh); rationarium
#     fictum handshake_failure a Cloudflare ipso accipit
#   reici debent: badssl expired / wrong.host / untrusted-root /
#     self-signed / sha1-intermediate (hoc etiam expiratum - causa
#     expiratum, non algorithmus)
#   nuntiatur solum: revoked.badssl.com (revocatio non iudicatur, spec IV)
# Instrumentum MANU (rete publicum), non porta. Exitus 0 omnia ut
# exspectata.
#
# Usus: ./tools/linux/fiducia_vivum.sh [-musl|-glibc]
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2
IMAGO=glibc
case "${1:-}" in
    -musl) IMAGO=musl ;;
    -glibc|"") ;;
    *) echo "usus: $0 [-musl|-glibc]" >&2; exit 2 ;;
esac
RADICES="$(bin/aedilis --radices 2>/dev/null | sed 's/^/-I/' | tr '\n' ' ')"
PARTES=$(bin/aedilis tools/fiducia_vivum.c --varians posix --partes 2>/dev/null \
    | awk -F'\t' '$1=="O" {printf "%s ", $2}')
VENDITI=$(bin/aedilis tools/fiducia_vivum.c --varians posix --partes 2>/dev/null \
    | awk -F'\t' '$1=="V" {printf "%s ", $2}')
[ -n "$PARTES" ] || { echo "fiducia_vivum: clausura vacua (aedilis)" >&2; exit 2; }

HOSPITES="+nyc3.digitaloceanspaces.com +cloudflare.com ~r2.cloudflarestorage.com \
+badssl.com \
-expired.badssl.com=expiratum -wrong.host.badssl.com=nomen \
-untrusted-root.badssl.com=fide -self-signed.badssl.com=fide \
-sha1-intermediate.badssl.com= ~revoked.badssl.com"

docker run --rm -v "$PWD":/rhubarb:ro -w /rhubarb "rhubarb-proba-$IMAGO" sh -c "
VOBJ=''
for v in $VENDITI; do b=\$(basename \$v .c); gcc -O2 -DBR_ENABLE_INTRINSICS=1 -c \$v -o /tmp/v_\$b.o || exit 2; VOBJ=\"\$VOBJ /tmp/v_\$b.o\"; done
gcc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -Wno-type-limits -O2 $RADICES tools/fiducia_vivum.c $PARTES \$VOBJ -lm -o /tmp/fiducia_vivum || exit 2
fractae=0
for vectura in tls13 bearssl; do
    RHUBARB_TLS=\$vectura /tmp/fiducia_vivum $HOSPITES || fractae=1
done
exit \$fractae
"
