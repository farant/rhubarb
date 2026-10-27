#!/bin/bash
# tools/secretum_mittere.sh - clavem localem in hospitem remotum mittere,
# numquam visam (communio-plan-3 T1; idea Frani 2026-10-27)
#
# Usus: tools/secretum_mittere.sh [-siccum|-agere] [-renovare]
#           <clavis-localis> <app> <alias-ssh> [nomen-remotum]
#       e.g. tools/secretum_mittere.sh -agere ~/.rhubarb/resend.clavis
#            exemplar smaragda
#
# Claves domi manent ubi ceterae (~/.rhubarb/<provisor>.clavis, 0600);
# hoc instrumentum unam in /var/lib/<app>/<nomen> hospitis ponit,
# possessore <app>, modo 0600. CLAVIS NUMQUAM in argumentis, in filo
# temporario, in effusione: per stdin ssh in 'sudo install /dev/stdin'
# fluit. Confirmatio: SHA-256 utrimque computatur et confertur - solum
# 'congruit' aut 'differt' imprimitur (nec clavis nec sigillum eius).
#
# -siccum (DEFALTA): mandata imprimit, NIHIL agit (ut dimittere.sh).
# -agere: agit. -renovare: clavem remotam exsistentem substituit (sine
#   eo recusatur - rotatio consulta, numquam casu).
# Recusat: clavis localis non filum regulare, nexus symbolicus, aut
#   modus latior quam 0600; app/nomen extra [a-z0-9._-].
# SECRETUM_MITTERE_SSH: mandatum ssh (probationes: ssh fictum).
#
# Exitus: 0 congruit (aut siccum); 1 recusatum / differt / defectus;
#         2 usus malus.

set -u

usus () {
    echo "usus: tools/secretum_mittere.sh [-siccum|-agere] [-renovare]" \
        "<clavis-localis> <app> <alias-ssh> [nomen-remotum]" >&2
    exit 2
}

deficere () {
    echo "secretum_mittere: $1" >&2
    exit 1
}

MODUS=siccum
RENOVARE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -siccum) MODUS=siccum; shift ;;
        -agere) MODUS=agere; shift ;;
        -renovare) RENOVARE=1; shift ;;
        -*) usus ;;
        *) break ;;
    esac
done
[ $# -ge 3 ] && [ $# -le 4 ] || usus
CLAVIS="$1"
APP="$2"
ALIAS="$3"
NOMEN="${4:-$(basename "$CLAVIS")}"
SSH="${SECRETUM_MITTERE_SSH:-ssh}"

# nomina in mandato remoto: nihil nisi [a-z0-9._-] (nulla injectio)
for x in "$APP" "$NOMEN"; do
    case "$x" in
        ''|*[!a-z0-9._-]*|.*) echo "secretum_mittere: nomen malum: $x" >&2
            exit 2 ;;
    esac
done
case "$ALIAS" in
    ''|*[!A-Za-z0-9._@-]*) echo "secretum_mittere: alias malus" >&2
        exit 2 ;;
esac

[ -f "$CLAVIS" ] && [ ! -L "$CLAVIS" ] \
    || deficere "clavis non filum regulare: $CLAVIS"
MODUS_FILI="$(stat -f %Lp "$CLAVIS" 2>/dev/null || stat -c %a "$CLAVIS")"
case "$MODUS_FILI" in
    600|400) ;;
    *) deficere "clavis modus latior quam 0600 (chmod 600 $CLAVIS)" ;;
esac

DEST="/var/lib/$APP/$NOMEN"
EXSISTIT="sudo -n test -e $DEST"
INSTALLARE="sudo -n install -o $APP -g $APP -m 0600 /dev/stdin $DEST"
SIGILLARE="sudo -n sha256sum $DEST"

if [ "$MODUS" = siccum ]; then
    echo "[$ALIAS] $EXSISTIT    (exsistit sine -renovare: recusatur)"
    echo "[$ALIAS] $INSTALLARE    < $CLAVIS (per stdin)"
    echo "[$ALIAS] $SIGILLARE    (cum locali confertur: congruit/differt)"
    exit 0
fi

if "$SSH" "$ALIAS" "$EXSISTIT" < /dev/null; then
    [ "$RENOVARE" -eq 1 ] \
        || deficere "$DEST iam exsistit in $ALIAS (-renovare ad substituendum)"
fi
"$SSH" "$ALIAS" "$INSTALLARE" < "$CLAVIS" \
    || deficere "installatio in $ALIAS fallit"

if command -v shasum > /dev/null 2>&1; then
    LOCALE="$(shasum -a 256 < "$CLAVIS" | awk '{print $1}')"
else
    LOCALE="$(sha256sum < "$CLAVIS" | awk '{print $1}')"
fi
REMOTUM="$("$SSH" "$ALIAS" "$SIGILLARE" < /dev/null | awk '{print $1}')"
if [ -n "$REMOTUM" ] && [ "$REMOTUM" = "$LOCALE" ]; then
    LOCALE=""; REMOTUM=""
    echo "congruit: $ALIAS:$DEST (possessor $APP, 0600)"
    exit 0
fi
LOCALE=""; REMOTUM=""
echo "differt: $ALIAS:$DEST - copia non congruit" >&2
echo "differt"
exit 1
