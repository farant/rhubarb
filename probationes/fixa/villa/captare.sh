#!/bin/bash

# probationes/fixa/villa/captare.sh - fixa vera ex servo capere
# (exemplar officina/captare.sh: BIS capere, PIN solum si octetim
# idem - alioquin varians NOMINATUR, non tacite portam flaccidam
# facit)
#
# Usus: ./probationes/fixa/villa/captare.sh <alias_ssh>
#       ./probationes/fixa/villa/captare.sh smaragda
#
# Lectiones SOLAE - nihil in servo mutatur. Effusio cruda hic
# scribitur; EXPURGATIO MANU sequitur ante commissionem (nginx -T
# dominia vera et vias certificatorum fert).

set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../../.." || exit 1

ALIAS="${1:-smaragda}"
DIR="probationes/fixa/villa"
CRUDA="$DIR/cruda"
mkdir -p "$CRUDA"

SSH_OPTS=(-o BatchMode=yes -o ConnectTimeout=8)

# unum imperium bis capere; idem = PIN, diversum = VARIANS nominatum
capere () {
    local nomen="$1"; shift
    local imperium="$*"
    local a="$CRUDA/$nomen.a"
    local b="$CRUDA/$nomen.b"

    ssh "${SSH_OPTS[@]}" "$ALIAS" "$imperium" > "$a" 2>/dev/null
    ssh "${SSH_OPTS[@]}" "$ALIAS" "$imperium" > "$b" 2>/dev/null

    if [ ! -s "$a" ]; then
        echo "  VACUUM   $nomen (imperium fefellit?)"
        return
    fi
    if cmp -s "$a" "$b"; then
        mv "$a" "$CRUDA/$nomen.txt"
        rm -f "$b"
        echo "  PIN      $nomen ($(wc -l < "$CRUDA/$nomen.txt" | tr -d ' ') lineae)"
    else
        echo "  VARIANS  $nomen - .a et .b servata, differentiam inspice"
    fi
}

echo "capiens ex '$ALIAS' (bis quaeque, lectiones solae)..."

# I. nginx: configuratio resoluta tota (fons situum + certificatorum)
capere nginx_T 'sudo nginx -T 2>/dev/null'

# II. systemctl show: forma clavis=valor, unitates plures uno vocamine
# Result + ExecMainStatus additae 2026-07-24: sine eis forma unitatis
# FRACTAE causam suam non fert (vide captare_fracta.sh).
PROPRIETATES='Id,Description,LoadState,ActiveState,SubState,UnitFileState,ActiveEnterTimestamp,MainPID,NRestarts,Result,ExecMainStatus'
capere systemctl_show_activae \
    "systemctl show nginx.service smaragda.service litestream.service --property=$PROPRIETATES --no-pager"

# II-bis. unitas IGNOTA in MEDIO responsi multiplicis - probat
#         recordum non-inventum vicinos suos non corrumpere
capere systemctl_show_medio_ignota \
    "systemctl show nginx.service nulla-tale.service litestream.service --property=$PROPRIETATES --no-pager"

# III. forma DEFECTUS: unitas ignota (LoadState=not-found)
capere systemctl_show_ignota \
    "systemctl show nulla-tale-servitium.service --property=$PROPRIETATES --no-pager"

# IV. forma INACTIVA: unitas exsistens sed non currens
capere systemctl_show_inactiva \
    "systemctl show \$(systemctl list-unit-files --type=service --state=disabled --no-pager --plain | awk 'NR==2{print \$1}') --property=$PROPRIETATES --no-pager"

# V. inventio: quid currit (cum LEGENDA in calce - parsator ea
#    desinere debet)
capere list_units \
    'systemctl list-units --type=service --state=running --no-pager --plain'

# VI. discus (POSIX -P: columnae fixae)
capere df 'df -P /'

# VII. tempus surrectionis (/proc: forma stabilis in aeternum,
#      contra 'uptime' cuius prosa per versiones mutat)
capere uptime 'cat /proc/uptime'

# VIII. memoria (/proc/meminfo: idem argumentum)
capere meminfo 'head -5 /proc/meminfo'

# IX. list-units VACUUM: caput + linea vacua + 'N loaded units
#     listed.' sine ulla unitate. Forma vacua propria est, non
#     absentia formae - parsator ZEPHYRUM reddat, non ruat.
capere list_units_vacuae \
    'systemctl list-units --state=failed --no-pager --plain'

# X. diaria VACUA: journalctl praefationem 'Hint:' MULTILINEAM ante
#    '-- No entries --' emittit cum usor coetum 'adm' non habet.
#    Facies eam monstraret nisi quis eam tolleret.
capere journalctl_vacua \
    'journalctl -u nginx.service -n 3 --no-pager 2>&1'

echo
echo "cruda in $CRUDA/"
echo "PROXIMUM: expurgare manu -> $DIR/*.txt, deinde committere"
echo "(nginx_T dominia vera et vias certificatorum fert)"
