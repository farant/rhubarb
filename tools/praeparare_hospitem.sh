#!/bin/bash

# tools/praeparare_hospitem.sh - hospitem semel praeparare pro
# applicatione (usor, directoria, systemd, nginx, DNS, certbot)
#
# Usus: tools/praeparare_hospitem.sh [-siccum|-agere] [-sine-certbot]
#           <app> <alias-ssh> <hospes-dns> <ip>
#       e.g. tools/praeparare_hospitem.sh -siccum exemplar smaragda \
#           exemplar.golden.earth 165.227.210.190
#
# -siccum (DEFALTA): omnia mandata imprimit, NIHIL agit. -agere: agit.
# -sine-certbot: certbot non curritur sed imprimitur - Franus eum in
#   termino suo currit (epistulam inscribit), postquam DNS propagatum
#   est (instrumentum sine TTY interactivum non est).
#
# HOSPES BROWNFIELD: solum res propriae tanguntur - usor <app>,
# /opt/<app>, /var/lib/<app>, unitas <app>.service, situs nginx
# <hospes-dns>. 'nginx -t' ANTE reload. certbot interactivus (Franus
# epistulam inscribit). Codex invitationis fortuitus in hospite
# creatur (/var/lib/<app>/invitatio, 0600) - numquam in hac machina.
#
# DNS: build/aedilis/oceanus/oceanus assecurare (A <titulus> <ip>):
# idempotens - creat, mutat aut nihil agit.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2

MODUS=siccum
case "${1:-}" in
    -siccum) shift ;;
    -agere) MODUS=agere; shift ;;
esac
CERTBOT=1
if [ "${1:-}" = "-sine-certbot" ]; then
    CERTBOT=0
    shift
fi
[ $# -eq 4 ] || {
    echo "usus: $0 [-siccum|-agere] [-sine-certbot] <app> <alias-ssh> <hospes-dns> <ip>" >&2
    exit 2
}
APP="$1"
ALIAS="$2"
HOSPES="$3"
IP="$4"
TITULUS="${HOSPES%%.*}"
DOMINIUM="${HOSPES#*.}"
[ "$TITULUS" != "$HOSPES" ] || { echo "praeparare: hospes-dns sine dominio: $HOSPES" >&2; exit 2; }
for f in "apps/$APP/$APP.service" "apps/$APP/$APP.nginx"; do
    [ -f "$f" ] || { echo "praeparare: $f deest" >&2; exit 2; }
done

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

remote_tty () {
    echo "[$ALIAS (interactivum)] $1"
    [ "$MODUS" = agere ] || return 0
    ssh -t "$ALIAS" "$1"
}

locale ./bin/aedilis tools/oceanus.c || exit 1
locale bash build/aedilis/oceanus/struere.sh || exit 1
locale build/aedilis/oceanus/oceanus assecurare "$DOMINIUM" A "$TITULUS" "$IP" 3600 || exit 1
remote "id -u $APP >/dev/null 2>&1 || sudo useradd --system --no-create-home --shell /usr/sbin/nologin $APP" || exit 1
remote "sudo install -d -o root -g root -m 0755 /opt/$APP" || exit 1
remote "sudo install -d -o $APP -g $APP -m 0700 /var/lib/$APP" || exit 1
remote "sudo test -f /var/lib/$APP/invitatio || sudo sh -c 'umask 077; head -c 18 /dev/urandom | base64 > /var/lib/$APP/invitatio; chown $APP:$APP /var/lib/$APP/invitatio'" || exit 1
locale scp -q "apps/$APP/$APP.service" "apps/$APP/$APP.nginx" "$ALIAS:/tmp/" || exit 1
remote "sudo install -o root -g root -m 0644 /tmp/$APP.service /etc/systemd/system/$APP.service && rm -f /tmp/$APP.service" || exit 1
remote "sudo systemctl daemon-reload && sudo systemctl enable $APP" || exit 1
remote "sudo install -o root -g root -m 0644 /tmp/$APP.nginx /etc/nginx/sites-available/$HOSPES && rm -f /tmp/$APP.nginx" || exit 1
remote "sudo ln -sf /etc/nginx/sites-available/$HOSPES /etc/nginx/sites-enabled/$HOSPES" || exit 1
remote "sudo nginx -t && sudo systemctl reload nginx" || exit 1
if [ "$CERTBOT" = 1 ]; then
    remote_tty "sudo certbot --nginx -d $HOSPES" || exit 1
else
    echo "praeparare: certbot Franus ipse currat: ssh -t $ALIAS sudo certbot --nginx -d $HOSPES"
fi
echo "praeparare: codex invitationis: ssh $ALIAS sudo cat /var/lib/$APP/invitatio"
