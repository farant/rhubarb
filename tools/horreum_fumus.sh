#!/bin/bash
# tools/horreum_fumus.sh - fumus VIVUS horrei contra situlam veram
# (horreum-plan T6): ponere / capere / inspicere / enumerare /
# praesignare (GET per curl) / multipars (XVII MiB = II partes) /
# purgatio. Omnia sub horreum-fumus/<tempus>/; purgatio etiam post
# defectum (trap). Nihil secreti scribit: URL praesignata numquam
# imprimitur.
#
# Usus:   tools/horreum_fumus.sh PROFILUM
#         (HORREUM=via binarii; ordinarium build/aedilis/horreum/horreum)
# Exitus: 0 omnia bene; 1 gradus defecit (nominatur); 2 usus.
set -u
[ $# -eq 1 ] || { echo "usus: tools/horreum_fumus.sh PROFILUM" >&2; exit 2; }
PROFILUM="$1"
H="${HORREUM:-build/aedilis/horreum/horreum}"
[ -x "$H" ] || { echo "horreum_fumus: $H deest (bin/aedilis tools/horreum.c)" >&2; exit 2; }
D="$(mktemp -d "${TMPDIR:-/tmp}/horreum-fumus.XXXXXX")"
R="horreum-fumus/$(date -u +%Y%m%dT%H%M%SZ)"

h () { "$H" -profilum "$PROFILUM" "$@"; }

purgare () {
    h delere "$R/parvum.txt" "$R/magnum.bin" >/dev/null 2>&1
    h pendentes "$R/" -abicere >/dev/null 2>&1
    rm -rf "$D"
}
trap purgare EXIT

fractum () { echo "FUMUS FRACTUS ($PROFILUM): $*" >&2; exit 1; }

printf 'salve, horreum\n' > "$D/parvum.txt"
h ponere "$R/parvum.txt" "$D/parvum.txt" >/dev/null || fractum "ponere parvum"
h capere "$R/parvum.txt" "$D/parvum.reditum" >/dev/null || fractum "capere parvum"
cmp -s "$D/parvum.txt" "$D/parvum.reditum" || fractum "parvum non idem"
h inspicere "$R/parvum.txt" | grep -q "^mensura	15$" || fractum "inspicere mensura"
h enumerare "$R/" | grep -q "	$R/parvum.txt$" || fractum "enumerare"

URL="$(h praesignare GET "$R/parvum.txt" -secundae 120)" || fractum "praesignare"
curl -sf -o "$D/parvum.praesignatum" "$URL" || fractum "GET praesignatum (curl)"
unset URL
cmp -s "$D/parvum.txt" "$D/parvum.praesignatum" || fractum "praesignatum non idem"

dd if=/dev/urandom of="$D/magnum.bin" bs=1048576 count=17 2>/dev/null
h ponere "$R/magnum.bin" "$D/magnum.bin" | grep -q -- "-2$" || fractum "multipars (ETag -2)"
h capere "$R/magnum.bin" "$D/magnum.reditum" >/dev/null || fractum "capere magnum"
cmp -s "$D/magnum.bin" "$D/magnum.reditum" || fractum "magnum non idem"

h delere "$R/parvum.txt" "$R/magnum.bin" >/dev/null || fractum "delere"
[ -z "$(h enumerare "$R/")" ] || fractum "relicta post delere"
[ -z "$(h pendentes "$R/")" ] || fractum "onera pendentia"
echo "FUMUS BONUS ($PROFILUM): ponere capere inspicere enumerare praesignare multipars purgatio"
