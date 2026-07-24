#!/bin/bash

# probationes/fixa/villa/captare_fracta.sh - formam unitatis FRACTAE
# capere.
#
# HOC SCRIPTUM SERVUM MUTAT. Ideo a captare.sh SEPARATUM est: illud
# promittit 'lectiones solae' et promissum id servandum est. Qui
# mutat, clamet.
#
# QUID FACIT: unitatem systemd temporariam creat quae statim
# /bin/false currit (ergo cadit), formam eius capit, deinde eam
# TOLLIT. Servitia viva (nginx, app, litestream) NON tanguntur -
# nihil restartatur, nihil aliud scribitur.
#
# CUR OMNINO: villa tota ideo exsistit ut ostendat cum aliquid
# fractum sit. Sine hoc fixo parsator contra sanitatem SOLAM
# probatus est, et formam fracti primum videret quo momento aliquid
# vere frangeretur - quod est pessimum momentum discendi.
#
# CUR NON MANU FINGATUR: disciplina fixorum (bis capere, octetim
# comparare) ideo exsistit ut formae FICTAE arceantur. Valores qui
# maxime important - ActiveState=failed, Result=exit-code,
# ExecMainStatus - sunt praecise ii quos fingere falleremur.
#
# Usus: ./probationes/fixa/villa/captare_fracta.sh <alias_ssh>
#
# Effusio cruda in cruda/; EXPURGATIO MANU sequitur.

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../../.." || exit 1

ALIAS="${1:-smaragda}"
DIR="probationes/fixa/villa"
CRUDA="$DIR/cruda"
mkdir -p "$CRUDA"

PROPRIETATES='Id,Description,LoadState,ActiveState,SubState,UnitFileState,ActiveEnterTimestamp,MainPID,NRestarts,Result,ExecMainStatus'

echo "CAUTIO: hoc scriptum unitatem temporariam in '$ALIAS' creat"
echo "        et deinde tollit. Servitia viva non tanguntur."
echo

# Scriptum remotum: creare -> capere BIS -> tollere. 'trap ... EXIT'
# tollit etiam si captura in medio cadat - unitas orphana in servo
# vero relicta damnum verum esset.
REMOTUM=$(cat <<'REMOTE'
set -u
U=villa-probatio-temporaria
F=/etc/systemd/system/$U.service

cleanup () {
    sudo systemctl stop $U.service    >/dev/null 2>&1
    sudo systemctl disable $U.service >/dev/null 2>&1
    sudo rm -f $F
    sudo systemctl daemon-reload      >/dev/null 2>&1
    sudo systemctl reset-failed $U.service >/dev/null 2>&1
}
trap cleanup EXIT

sudo tee $F >/dev/null <<'UNIT'
[Unit]
Description=Villa probatio temporaria (fixtura - delenda)

[Service]
Type=oneshot
ExecStart=/bin/false
UNIT

sudo systemctl daemon-reload
sudo systemctl start $U.service >/dev/null 2>&1
sleep 1

echo "===fracta.a==="
systemctl show $U.service --property=PROPS --no-pager
echo "===fracta.b==="
systemctl show $U.service --property=PROPS --no-pager
echo "===units.a==="
systemctl list-units --state=failed --no-pager --plain
echo "===units.b==="
systemctl list-units --state=failed --no-pager --plain
echo "===finis==="
REMOTE
)
REMOTUM="${REMOTUM//PROPS/$PROPRIETATES}"

ssh -o BatchMode=yes -o ConnectTimeout=10 "$ALIAS" 'bash -s' \
    <<< "$REMOTUM" > "$CRUDA/fracta_omnia.txt" 2>&1

if ! grep -q '===finis===' "$CRUDA/fracta_omnia.txt"; then
    echo "FRACTUM: capturam non perfecit - inspice"
    echo "         $CRUDA/fracta_omnia.txt"
    echo "CAUTIO: unitatem in servo manu verifica:"
    echo "  ssh $ALIAS 'systemctl show villa-probatio-temporaria.service" \
         "--property=LoadState'"
    exit 1
fi

# sectiones findere et PIN solum si a et b idem sunt
findere () {
    awk -v a="===$1.a===" -v b="===$1.b===" '
        $0==a {in_a=1; next} $0==b {in_a=0; in_b=1; next}
        /^===/ {in_a=0; in_b=0}
        in_a {print > (dir "/" nomen ".a")}
        in_b {print > (dir "/" nomen ".b")}
    ' dir="$CRUDA" nomen="$2" "$CRUDA/fracta_omnia.txt"

    if cmp -s "$CRUDA/$2.a" "$CRUDA/$2.b"; then
        mv "$CRUDA/$2.a" "$CRUDA/$2.txt"
        rm -f "$CRUDA/$2.b"
        echo "  PIN      $2"
    else
        echo "  VARIANS  $2 - .a et .b servata"
    fi
}

findere fracta systemctl_show_fracta
findere units  list_units_fractae

echo
echo "cruda in $CRUDA/"
echo "unitas remota TOLLITUR a trap remoto; verifica:"
echo "  ssh $ALIAS 'systemctl list-units --state=failed --plain --no-legend'"
echo "PROXIMUM: expurgare manu -> $DIR/*.txt, deinde committere"
