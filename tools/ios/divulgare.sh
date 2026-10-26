#!/bin/bash
# tools/ios/divulgare.sh - divulgatio applicationis domus in TestFlight
# (testa-ios C2, spec II.3): Franus dicit "publish <app>", Claudius hoc
# currit. Recusationes ANTE omne rete; deinde ratio parata (-parare,
# idempotens), numerus proximus, aedificatio App Store (app.sh
# -divulgatio), missio usque ad VALID, notae "What to Test" cum linea
# 'commit <hash>' (D11), grex Familia, invitationes.
#
# Usus: ./tools/ios/divulgare.sh <app> -siccum
#       ./tools/ios/divulgare.sh <app> -notae <plagula>
#   -siccum  nihil mittit: applicatio, versio, commissum ultimum
#            divulgatum (ex Apple) et commissa ab eo - unde Claudius
#            notas scribit
#   -notae   textus "What to Test" (sine linea commit: additur)
# Versio: apps/<app>/versio (linea una, ut 1.2) aliter 1.0 (D10).
# Solum main (D8): arca principalis, ramus main, arbor munda.
# R8 (tempus evolutionis, C2): DIVULGATIO_EXTRA_MAIN=1 aliam arcam/ramum
# admittit (arbor munda tamen); ramus in linea commit notatur.
#
# Exitus: 0 | 1 fractum (Apple, rete, aedificatio) | 2 recusatio |
#         3 pactum Apple non acceptum
set -u
export LC_ALL=C
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

apparatus () { echo "ios/divulgare.sh: $*" >&2; exit 2; }
fractum () { echo "ios/divulgare.sh: FRACTUM: $*" >&2; exit 1; }

APP=""; MODUS=""; NOTAE=""
while [ $# -gt 0 ]; do
    case "$1" in
        -siccum) MODUS=siccum ;;
        -notae) [ $# -ge 2 ] || apparatus "-notae plagulam postulat"
                MODUS=notae; NOTAE="$2"; shift ;;
        -*) apparatus "optio ignota: $1" ;;
        *) [ -z "$APP" ] || apparatus "applicatio una: $APP et $1"; APP="$1" ;;
    esac
    shift
done
[ -n "$APP" ] && [ -n "$MODUS" ] \
    || apparatus "usus: $0 <app> -siccum | -notae <plagula>"
case "$APP" in
    ""|*[!a-z]*) apparatus "applicatio '$APP': litterae minusculae solae" ;;
esac
FONS="apps/$APP/$APP.c"
[ -f "$FONS" ] || apparatus "$FONS deest"

# --- I. recusationes (ante omne rete) ---------------------------------
RAMUS=$(git rev-parse --abbrev-ref HEAD 2> /dev/null) || apparatus "non repositorium git"
COMMUNIS=$(cd "$(git rev-parse --git-common-dir)" && pwd -P)
PROPRIUS=$(cd "$(git rev-parse --git-dir)" && pwd -P)
EXTRA=0
if [ "$COMMUNIS" != "$PROPRIUS" ] || [ "$RAMUS" != main ]; then
    [ "${DIVULGATIO_EXTRA_MAIN:-}" = 1 ] \
        || apparatus "solum main divulgatur (D8): haec arca $(pwd -P), ramus $RAMUS - in arca principali super main curre"
    EXTRA=1
    echo "R8: divulgatio extra main (ramus $RAMUS) - DIVULGATIO_EXTRA_MAIN=1"
fi
SORDES=$(git status --porcelain)
[ -z "$SORDES" ] || apparatus "arbor non munda (D8: aedificatio commisso respondeat):
$SORDES"
CFG="$HOME/.rhubarb/ios/nundinae_apple.stml"
[ -f "$CFG" ] || apparatus "configuratio $CFG deest (spec II.5)"
CLAVIS=$(sed -n 's/.*via_clavis="\([^"]*\)".*/\1/p' "$CFG" | head -1)
case "$CLAVIS" in "~/"*) CLAVIS="$HOME/${CLAVIS#\~/}" ;; esac
[ -n "$CLAVIS" ] && [ -f "$CLAVIS" ] || apparatus "clavis API deest (via_clavis in $CFG)"
[ "$(stat -f %Lp "$CLAVIS")" = 600 ] \
    || apparatus "clavis API $CLAVIS non 0600 (chmod 600)"
command -v xcrun > /dev/null || apparatus "xcrun deest (Xcode)"
[ -x bin/aedilis ] || apparatus "bin/aedilis deest - ./tools/aedilis_struere.sh"

# instrumentum per aedilis (fontes > obiecta -> recompilat)
NUNDINAE=build/aedilis/nundinae_apple/nundinae_apple
mkdir -p build/ios/divulgatio || apparatus "build/ios/divulgatio creari nequit"
{ bin/aedilis tools/nundinae_apple.c && bash build/aedilis/nundinae_apple/struere.sh; } \
    > build/ios/divulgatio/nundinae.log 2>&1 \
    || { tail -5 build/ios/divulgatio/nundinae.log >&2; apparatus "nundinae_apple aedificari nequit"; }

VERSIO=1.0
if [ -f "apps/$APP/versio" ]; then
    VERSIO=$(head -1 "apps/$APP/versio" | tr -d ' \r')
fi
printf '%s' "$VERSIO" | grep -qxE '[0-9]+(\.[0-9]+){0,2}' \
    || apparatus "apps/$APP/versio '$VERSIO': 1.0 aut 1.2.3"
COMMISSUM=$(git rev-parse --short=12 HEAD)

# --- II. commissum ultimum (D11, ex Apple) -----------------------------
ULTIMUM=$("$NUNDINAE" -ultimum "$APP")
RC=$?
[ "$RC" = 0 ] || exit "$RC"

if [ "$MODUS" = siccum ]; then
    echo "applicatio: $APP (earth.golden.$APP) versio $VERSIO"
    echo "commissum: $COMMISSUM ($RAMUS)"
    if [ -z "$ULTIMUM" ]; then
        echo "ultimum: nullum (prima divulgatio) - commissa recentia:"
        git log --oneline -20
    elif ! git cat-file -e "$ULTIMUM^{commit}" 2> /dev/null; then
        echo "ultimum: $ULTIMUM IGNOTUM in hoc repositorio - commissa recentia:"
        git log --oneline -20
    elif ! git merge-base --is-ancestor "$ULTIMUM" HEAD; then
        echo "ultimum: $ULTIMUM non antecessor HEAD - commissa ab radice communi:"
        git log --oneline "$(git merge-base "$ULTIMUM" HEAD)..HEAD"
    else
        echo "ultimum: $ULTIMUM - commissa ab eo:"
        git log --oneline "$ULTIMUM..HEAD"
    fi
    exit 0
fi

# --- III. divulgatio -----------------------------------------------------
[ -f "$NOTAE" ] && [ -s "$NOTAE" ] || apparatus "notae $NOTAE desunt aut vacuae"
grep -qE '^commit [0-9a-f]{7,40}( |$)' "$NOTAE" \
    && apparatus "notae iam lineam 'commit <hash>' habent - additur sponte"

PARATIO=$("$NUNDINAE" -parare "$APP")
RC=$?
printf '%s\n' "$PARATIO"
[ "$RC" = 0 ] || exit "$RC"
PROFILUM=$(printf '%s\n' "$PARATIO" | sed -n 's/^diploma App Store: \(.*\) [a-zA-Z]*$/\1/p')
IDENTITAS=$(printf '%s\n' "$PARATIO" | sed -n 's/.*SHA-1 \([0-9A-F]\{40\}\).*/\1/p' | head -1)
[ -f "$PROFILUM" ] || fractum "profilum ex -parare non lectum"
[ -n "$IDENTITAS" ] || fractum "identitas (SHA-1) ex -parare non lecta"

AEDIFICATIO=$("$NUNDINAE" -proxima "$APP")
RC=$?
[ "$RC" = 0 ] || exit "$RC"
echo "aedificatio proxima: $AEDIFICATIO"

tools/ios/app.sh "$FONS" -divulgatio -profilum "$PROFILUM" \
    -identitas "$IDENTITAS" -versio "$VERSIO" -aedificatio "$AEDIFICATIO" \
    -commissum "$COMMISSUM" || exit $?
D="build/ios/divulgatio/$APP"
IPA="$D/$APP.ipa"
[ -f "$IPA" ] || fractum "$IPA deest post app.sh"

# notae + linea commit (D11; R8: ramus notatur)
LINEA="commit $COMMISSUM"
[ "$EXTRA" = 1 ] && LINEA="$LINEA ($RAMUS)"
{ cat "$NOTAE"; printf '\n\n%s\n' "$LINEA"; } > "$D/notae.txt" \
    || fractum "$D/notae.txt scribi nequit"

"$NUNDINAE" -mittere "$APP" "$IPA" "$VERSIO" "$AEDIFICATIO" || exit $?
"$NUNDINAE" -distribuere "$APP" "$AEDIFICATIO" -notae "$D/notae.txt"
RC=$?
if [ "$RC" != 0 ]; then
    echo "ios/divulgare.sh: aedificatio $AEDIFICATIO VALID sed distributio fracta - iterum: $NUNDINAE -distribuere $APP $AEDIFICATIO -notae $D/notae.txt" >&2
    exit "$RC"
fi
echo "DIVULGATUM: $APP $VERSIO ($AEDIFICATIO), $LINEA, grex Familia (TestFlight)"
exit 0
