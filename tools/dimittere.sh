#!/bin/bash

# tools/dimittere.sh - applicationem in hospitem remotum dimittere
#
# Usus: tools/dimittere.sh [-siccum|-agere] <app> <alias-ssh>
#       e.g. tools/dimittere.sh -siccum exemplar smaragda
#
# -siccum (DEFALTA): omnia mandata imprimit, NIHIL agit - Franus ea
#   legit et approbat. -agere: agit.
#
# Iter: binarium staticum (tools/linux/aedificare.sh) -> scp in /tmp
# hospitis -> /opt/<app>/<app>.new -> prius in .prior -> mv atomicum ->
# systemctl restart -> /salus per ssh. Salus fracta: .prior
# restituitur, restart, exitus 1 cum nuntio nominato.
#
# Portus salutis: DIMITTERE_PORTUS (defalta 3010).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2

MODUS=siccum
case "${1:-}" in
    -siccum) shift ;;
    -agere) MODUS=agere; shift ;;
esac
[ $# -eq 2 ] || { echo "usus: $0 [-siccum|-agere] <app> <alias-ssh>" >&2; exit 2; }
APP="$1"
ALIAS="$2"
PORTUS="${DIMITTERE_PORTUS:-3010}"
[ -f "apps/$APP/$APP.c" ] || { echo "dimittere: apps/$APP/$APP.c deest" >&2; exit 2; }

locale () {
    echo "[locale] $*"
    [ "$MODUS" = agere ] || return 0
    "$@"
}

remote () {
    echo "[$ALIAS] $1"
    [ "$MODUS" = agere ] || return 0
    ssh "$ALIAS" "$1"
}

restituere () {
    remote "sudo mv /opt/$APP/$APP.prior /opt/$APP/$APP && sudo systemctl restart $APP"
}

locale tools/linux/aedificare.sh "apps/$APP/$APP.c" || exit 1
locale scp -q "build/linux/$APP" "$ALIAS:/tmp/$APP.new" || exit 1
remote "sudo install -o root -g root -m 0755 /tmp/$APP.new /opt/$APP/$APP.new && rm -f /tmp/$APP.new" || exit 1
remote "if [ -f /opt/$APP/$APP ]; then sudo cp -p /opt/$APP/$APP /opt/$APP/$APP.prior; fi" || exit 1
remote "sudo mv /opt/$APP/$APP.new /opt/$APP/$APP" || exit 1
if [ "$MODUS" = siccum ]; then
    echo "[$ALIAS] sudo systemctl restart $APP"
    echo "[$ALIAS] sleep 2; curl -fsS http://127.0.0.1:$PORTUS/salus"
    echo "[si salus fallit] [$ALIAS] sudo mv /opt/$APP/$APP.prior /opt/$APP/$APP && sudo systemctl restart $APP"
    exit 0
fi
if ! remote "sudo systemctl restart $APP" \
    || ! remote "sleep 2; curl -fsS http://127.0.0.1:$PORTUS/salus"; then
    echo "dimittere: salus $APP in $ALIAS FRACTA - .prior restituitur" >&2
    restituere
    exit 1
fi
echo "dimittere: $APP in $ALIAS - salus bona"
