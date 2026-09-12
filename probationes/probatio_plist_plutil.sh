#!/bin/bash
# probatio_plist_plutil.sh - ORACULUM ALIENUM: plutil (instrumentum
# Apple) plists NOSTRAS iudicat. Exemplar: probatio_qr_gyrus.sh, ubi
# CoreImage codices nostros legit.
#
# CONTROLLUM PRIMUM: plist ab APPLE scripta linteatur ANTE nostram, ut
# oraculum ipsum probetur. Si controllum cadit, instrumentum fractum
# est (aut fixum corruptum), non codex noster.
#
# CALIBRATIO NOMINATA (plist-spec par. VII): plutil plist SINE
# declaratione generis accipit et indentationem ignorat - ergo
# silentium eius neutrum probat; ea probatio_plist.c ipsa asserit (G3).
# Quod HIC probatur: Apple octetos nostros ut plist validam accipit.
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita desunt -
#         numquam cum silentio viridi confundendum).

set -u
RADIX="$(cd "$(dirname "$0")/.." && pwd)"
cd "$RADIX" || exit 2

FIXUM=probationes/fixa/plist/apple_info.plist
BINARIUM=build/probationes/probatio_plist

[ -x /usr/bin/plutil ] || { echo "NIHIL CURSUM: plutil abest" >&2; exit 2; }
[ -f "$FIXUM" ] || { echo "NIHIL CURSUM: fixum abest: $FIXUM" >&2; exit 2; }
[ -x "$BINARIUM" ] || {
    echo "NIHIL CURSUM: $BINARIUM abest - ./compile_tests.sh plist prius" >&2
    exit 2; }

AREA="$(mktemp -d /tmp/probatio_plist_plutil.XXXXXX)" || exit 2
purgare () { rm -rf "$AREA"; }
deficere () {
    echo "FUMUS DEFECIT: $1" >&2
    [ -n "${2:-}" ] && [ -f "$2" ] && tail -20 "$2" >&2
    purgare
    exit 1
}

# ---- I. CONTROLLUM: oraculum plist VERAM accipit ----
if ! /usr/bin/plutil -lint "$FIXUM" > "$AREA/controllum.log" 2>&1; then
    deficere "CONTROLLUM FRACTUM: plutil plist Apple recusavit" \
        "$AREA/controllum.log"
fi

# ---- II. nostra scriptio (probatio_plist eam in PLIST_AREA ponit) ----
if ! PLIST_AREA="$AREA" "$BINARIUM" > "$AREA/probatio.log" 2>&1; then
    deficere "probatio_plist fracta" "$AREA/probatio.log"
fi
[ -f "$AREA/nostra.plist" ] || {
    echo "NIHIL CURSUM: probatio_plist nihil in PLIST_AREA scripsit" >&2
    purgare; exit 2; }

# ---- III. plutil NOSTRAM linteat ----
if ! /usr/bin/plutil -lint "$AREA/nostra.plist" \
        > "$AREA/nostra.log" 2>&1; then
    deficere "plutil plist NOSTRAM recusavit" "$AREA/nostra.log"
fi

# ---- IV. utraque ad formam canonicam versa, deinde conferre ----
/usr/bin/plutil -convert xml1 -o "$AREA/fixum.xml1" "$FIXUM" \
    || deficere "conversio fixi fracta" "$AREA/controllum.log"
/usr/bin/plutil -convert xml1 -o "$AREA/nostra.xml1" \
    "$AREA/nostra.plist" || deficere "conversio nostrae fracta" \
    "$AREA/nostra.log"
if ! diff -u "$AREA/fixum.xml1" "$AREA/nostra.xml1" \
        > "$AREA/differentia" 2>&1; then
    deficere "formae canonicae DISCREPANT" "$AREA/differentia"
fi

echo "plist plutil: controllum + nostra linteata, formae canonicae congruunt"
purgare
exit 0
