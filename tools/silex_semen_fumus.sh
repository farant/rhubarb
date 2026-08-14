#!/bin/bash

# tools/silex_semen_fumus.sh - PORTA: semen vitreum silicis vere
# aedificatur?
#
# CUR HAEC PORTA EXSISTIT
# =======================
# probationes/probatio_silex.c MANIFESTUM solum iudicat - quas
# plagulas silex scribat, non an compilentur. Caput eius id aperte
# fatetur: "Scaffoldatum hic NON compilatur - id cibus caninus est
# (silicetum), non suite."
#
# Consequentia MENSURATA (2026-08-14): templum vitreum ad atrium
# versum est; atrium imperium trahit -> hospitium -> tls ->
# tls_macos.m, quod Security poscit. Ordo genitus '-framework
# Security' NON ligabat. Nexus deficiebat in omni proiecto novo -
# et suite tota viridis manebat, quia nulla probatio semen umquam
# aedificabat. Porta ipsa vitium primum invenit quo nata est.
#
# QUID PROBAT
#   I.   silex semen vitreum excudere potest E CORPORE INFIXO
#        (area extra arborem rhubarb sita est de industria: ea
#        semita est quam usor vere currit, et ea sola corrumpi
#        potest cum lib/ mutatur sed bin/silex non renovatur)
#   II.  ordo aedificandi genitus vere compilat ET NECTIT
#   III. probationes genitae transeunt
#   IV.  (-agere) app aperitur et per bin/manus agi potest,
#        id est '-vivum' in semine vere vivit
#
# Usus:
#   ./tools/silex_semen_fumus.sh [-agere] [-servare]
#     -agere    app quoque aperire et agitare. FENESTRA VERA
#               apparet - ergo manu currendum, non in suite.
#     -servare  aream non delere (ad inspiciendum)
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita
#         desunt - numquam cum silentio viridi confundendum).

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX" || exit 2

AGERE=0
SERVARE=0
for arg in "$@"; do
    case "$arg" in
        -agere)   AGERE=1 ;;
        -servare) SERVARE=1 ;;
        *)
            echo "usus: $0 [-agere] [-servare]" >&2
            exit 2
            ;;
    esac
done

if [ ! -x bin/silex ]; then
    echo "FUMUS: bin/silex abest - ./tools/silex_struere.sh prius" >&2
    exit 2
fi
if [ "$AGERE" = 1 ] && [ ! -x bin/manus ]; then
    echo "FUMUS: bin/manus abest -" \
         "./compile_tools.sh manus_instrumentum prius" >&2
    exit 2
fi

TITULUS=semenfumi
AREA="$(mktemp -d /tmp/silex_semen_fumus.XXXXXX)" || exit 2
PROIECTUM="$AREA/$TITULUS"
# HOME PROPRIUM: semen ~/.rhubarb/<titulus>.volumen scribit. Sine
# hoc porta in datis VERIS Fran sordes relinqueret.
DOMUS="$AREA/domus"
mkdir -p "$DOMUS/.rhubarb" || exit 2

purgare () {
    if [ "$SERVARE" = 1 ]; then
        echo "FUMUS: area servata: $AREA"
    else
        rm -rf "$AREA"
    fi
}

deficere () {
    echo "FUMUS DEFECIT: $1" >&2
    if [ -n "${2:-}" ] && [ -f "$2" ]; then
        echo "--- ultimae XXV lineae ($2) ---" >&2
        tail -25 "$2" >&2
    fi
    SERVARE=1
    purgare
    exit 1
}

echo "FUMUS: area $AREA"
echo "FUMUS: $(./bin/silex -versio | tr '\n' ' ')"

# ---- I. excudere (E CORPORE: cwd extra arborem rhubarb) ----
echo "FUMUS: I. silex novum $TITULUS -vitrea"
( cd "$AREA" && "$RADIX/bin/silex" novum "$TITULUS" -vitrea ) \
    > "$AREA/novum.log" 2>&1 \
    || deficere "silex novum defecit" "$AREA/novum.log"
[ -f "$PROIECTUM/aedificare.sh" ] \
    || deficere "aedificare.sh non genitum" "$AREA/novum.log"

# Corpus infixum adhibitum esse PROBANDUM - si silex fabricam in
# disco invenisset, porta semitam falsam probaret.
grep -q "corpus commit=" "$AREA/novum.log" \
    || deficere "corpus infixum NON adhibitum (fabrica in disco
    inventa?) - porta semitam distributionis probare desiit" \
        "$AREA/novum.log"

# ---- II. aedificare (compilare ET NECTERE) ----
echo "FUMUS: II. ./aedificare.sh"
( cd "$PROIECTUM" && ./aedificare.sh ) \
    > "$AREA/aedificare.log" 2>&1 \
    || deficere "aedificatio defecit" "$AREA/aedificare.log"
[ -x "$PROIECTUM/bin/$TITULUS" ] \
    || deficere "binarium abest quamquam aedificatio exitum 0 dedit
    (ordo mv defecit?)" "$AREA/aedificare.log"

# ---- III. probationes genitae ----
echo "FUMUS: III. ./probare.sh"
( cd "$PROIECTUM" && HOME="$DOMUS" ./probare.sh ) \
    > "$AREA/probare.log" 2>&1 \
    || deficere "probationes genitae defecerunt" "$AREA/probare.log"

if [ "$AGERE" = 0 ]; then
    echo "FUMUS: FACTUM (excusum, aedificatum, probatum)"
    echo "FUMUS: '-agere' addens fenestram quoque aperit et agitat"
    purgare
    exit 0
fi

# ---- IV. agere: '-vivum' in semine vere vivit? ----
#
# HOME hic EXPORTANDUM, non uni vocationi praefigendum: manus
# tabularium sessionum in $HOME/.rhubarb/manus servat. Cum solum
# 'incipere' domum fictam videret, sessio illic scribebatur et
# vocationes sequentes eam in domo VERA quaerebant - 'sessio
# ignota', quamquam app optime vivebat. Defectio quae app arguit
# cum instrumentum reum esset.
export HOME="$DOMUS"

echo "FUMUS: IV. bin/manus incipere ./bin/$TITULUS -vivum"
SESSIO="$( cd "$PROIECTUM" && \
    "$RADIX/bin/manus" incipere "./bin/$TITULUS" -vivum \
    2>"$AREA/manus.err" )" \
    || deficere "manus incipere defecit" "$AREA/manus.err"

PORTUS="$(printf '%s\n' "$SESSIO" | grep -oE '[0-9]{4,5}' | head -1)"
[ -n "$PORTUS" ] \
    || deficere "portus e responso manus legi non potuit:
    [$SESSIO]" "$AREA/manus.err"
echo "FUMUS:    sessio $PORTUS"

sublevare () {
    "$RADIX/bin/manus" -s "$PORTUS" finire >/dev/null 2>&1
}

# AFFORDANTIAE VACUAE HIC RECTAE SUNT, nec defectio: pagina
# seminis titulus et paragraphus est, sine ullo quod premi possit.
# (Prima forma huius portae numerum > 0 poscebat et app innocentem
# arguebat.) Quod HIC probatur canalis est, non copia: vocatio
# ipsa succedere debet.
"$RADIX/bin/manus" -s "$PORTUS" affordantiae -machina \
    > "$AREA/afford.tsv" 2>"$AREA/afford.err"
STATUS_AFF=$?
if [ "$STATUS_AFF" = 2 ]; then
    sublevare
    deficere "manus affordantiae: NIHIL ACTUM (exitus II) -
    canalis imperii non respondit" "$AREA/afford.err"
fi
echo "FUMUS:    affordantiae:" \
     "$(wc -l < "$AREA/afford.tsv" | tr -d ' ') (vacuae rectae:" \
     "pagina seminis nihil premendum habet)"

# LIQUOR VERUS: pons loquitur? Semen '#salutatio' e vocatione
# 'salve' implet ('…' in HTML iacet donec responsum veniat). Ergo
# textus '…' manens significat internuntium tacuisse - quod nexus
# viridis et fenestra aperta soli numquam deprehenderent.
SALUS="$("$RADIX/bin/manus" -s "$PORTUS" -exspecta \
    textus '#salutatio' 2>"$AREA/textus.err")"
sublevare
case "$SALUS" in
    ''|'…')
        deficere "'#salutatio' = [$SALUS] - pons 'salve' tacuit
    (app vivit sed internuntius mortuus est)" "$AREA/textus.err"
        ;;
esac
echo "FUMUS:    salutatio: $SALUS"

echo "FUMUS: FACTUM (excusum, aedificatum, probatum, actum)"
purgare
exit 0
