#!/bin/bash

# tools/briar_fumus.sh - PORTA: briar INSTITUTUS plagulas thistle vere
# currit, extra arborem rhubarb, e corpore INFIXO?
#
# CUR HAEC PORTA EXSISTIT
# =======================
# briar/compile_probationes.sh fabricam sine clang iudicat (spec par. 8
# P3); tools/briar.c a nulla suite compilatur. Quod usor vere currit -
# '#!/usr/bin/env briar' in directorio quolibet, ~/.bin/briar, corpus
# infixum, aedificare.sh per clang, exec - HIC solum probatur. Semita
# distributionis est, et ea sola corrumpitur cum lib/ mutatur sed
# ~/.bin/briar non renovatur (praerequisitum: ./tools/briar_struere.sh
# prius - corpus vetus VIRIDE MENTITUR).
#
# QUID PROBAT
#   I.   ./salve.thistle extra arborem -> 'salve, munde' (corpus INFIXUM:
#        briar -versio ibi '(discus)' dicere NON debet)
#   II.  ./salve.thistle -probatio -> exitus 0, OMNIA PRAETERIERUNT
#   III. ./punctum.thistle -> 'summa 3'; -probatio viridis
#   IV.  ./salve_vitreum.thistle -struere -> directorium; bin/ adest;
#        -probatio viridis (app vitrea nectitur, fenestra NON aperitur)
#   V.   PLANTA: ./probatio_rubra.thistle -probatio exitum non-zerum
#        dare DEBET - porta quae rubrum videre nequit mortua est
#   VI.  recusatio: ./duo_principalia.thistle exitu 1 cum 'duo principalia'
#   VII. (-agere) app vitrea per bin/manus agitur: affordantiae >= I
#        (bulla), premere, textus corporis 'salve, munde' continet.
#        FENESTRA VERA apparet - manu currendum, non in suite.
#
# Usus:
#   ./tools/briar_fumus.sh [-agere] [-servare]
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita desunt).
# Linea ultima: 'fumus briar: sanum' | 'fumus briar: FRACTUM'.

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

BRIAR="$HOME/.bin/briar"
if [ ! -x "$BRIAR" ]; then
    echo "FUMUS: ~/.bin/briar abest - ./tools/briar_struere.sh prius" >&2
    exit 2
fi
if [ "$AGERE" = 1 ] && [ ! -x bin/manus ]; then
    echo "FUMUS: bin/manus abest - ./compile_tools.sh manus_instrumentum prius" >&2
    exit 2
fi
FIXA="$RADIX/briar/probationes/fixa/thistle"
for f in salve punctum salve_vitreum adversa/probatio_rubra adversa/duo_principalia; do
    [ -f "$FIXA/$f.thistle" ] || { echo "FUMUS: fixum abest: $f.thistle" >&2; exit 2; }
done

AREA="$(mktemp -d /tmp/briar_fumus.XXXXXX)" || exit 2
# HOME PROPRIUM (exportatum): briar ~/.rhubarb/briar/<t>-<clavis>/ scribit,
# manus tabularium sessionum in $HOME/.rhubarb/manus servat - sine hoc
# porta in datis VERIS sordes relinqueret
DOMUS="$AREA/domus"
mkdir -p "$DOMUS/.rhubarb" || exit 2
export HOME="$DOMUS"

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
    echo "fumus briar: FRACTUM"
    exit 1
}

cp "$FIXA/salve.thistle" "$FIXA/punctum.thistle" "$FIXA/salve_vitreum.thistle" \
   "$FIXA/adversa/probatio_rubra.thistle" "$FIXA/adversa/duo_principalia.thistle" \
   "$AREA/" || exit 2
chmod +x "$AREA"/*.thistle
echo "FUMUS: area $AREA"
echo "FUMUS: $( cd "$AREA" && "$BRIAR" -versio | tr '\n' ' ' )"

# ---- 0. corpus INFIXUM adhibitum esse PROBANDUM ----
if ( cd "$AREA" && "$BRIAR" -versio ) | grep -q '(discus)'; then
    deficere "corpus in disco inventum extra arborem - porta semitam falsam probaret"
fi

# ---- I. ./salve.thistle ----
echo "FUMUS: I. ./salve.thistle"
( cd "$AREA" && ./salve.thistle ) > "$AREA/salve.log" 2>&1 \
    || deficere "salve.thistle defecit" "$AREA/salve.log"
grep -q 'salve, munde' "$AREA/salve.log" \
    || deficere "'salve, munde' non impressum" "$AREA/salve.log"

# ---- II. ./salve.thistle -probatio ----
echo "FUMUS: II. ./salve.thistle -probatio"
( cd "$AREA" && ./salve.thistle -probatio ) > "$AREA/salve_probatio.log" 2>&1 \
    || deficere "salve -probatio defecit" "$AREA/salve_probatio.log"
grep -q 'OMNIA PRAETERIERUNT' "$AREA/salve_probatio.log" \
    || deficere "probatio sine OMNIA PRAETERIERUNT" "$AREA/salve_probatio.log"

# ---- III. ./punctum.thistle ----
echo "FUMUS: III. ./punctum.thistle (+ -probatio)"
( cd "$AREA" && ./punctum.thistle ) > "$AREA/punctum.log" 2>&1 \
    || deficere "punctum.thistle defecit" "$AREA/punctum.log"
grep -q 'summa 3' "$AREA/punctum.log" \
    || deficere "'summa 3' non impressum" "$AREA/punctum.log"
( cd "$AREA" && ./punctum.thistle -probatio ) > "$AREA/punctum_probatio.log" 2>&1 \
    || deficere "punctum -probatio defecit" "$AREA/punctum_probatio.log"

# ---- IV. ./salve_vitreum.thistle -struere + -probatio ----
echo "FUMUS: IV. ./salve_vitreum.thistle -struere (+ -probatio)"
VITREUM_DIR="$( cd "$AREA" && ./salve_vitreum.thistle -struere 2>"$AREA/vitreum.err" | tail -1 )" \
    || deficere "salve_vitreum -struere defecit" "$AREA/vitreum.err"
[ -x "$VITREUM_DIR/bin/salve_vitreum" ] \
    || deficere "binarium vitreum abest: $VITREUM_DIR/bin/salve_vitreum" "$AREA/vitreum.err"
( cd "$AREA" && ./salve_vitreum.thistle -probatio ) > "$AREA/vitreum_probatio.log" 2>&1 \
    || deficere "salve_vitreum -probatio defecit" "$AREA/vitreum_probatio.log"

# ---- V. PLANTA: probatio rubra rubra videatur ----
echo "FUMUS: V. ./probatio_rubra.thistle -probatio (RUBRA exspectata)"
if ( cd "$AREA" && ./probatio_rubra.thistle -probatio ) > "$AREA/rubra.log" 2>&1; then
    deficere "probatio rubra exitum 0 dedit - porta rubrum non videt" "$AREA/rubra.log"
fi

# ---- VI. recusatio cum linea ----
echo "FUMUS: VI. ./duo_principalia.thistle (recusatio exspectata)"
if ( cd "$AREA" && ./duo_principalia.thistle ) > "$AREA/duo.log" 2>&1; then
    deficere "duo_principalia non recusatum" "$AREA/duo.log"
fi
grep -q 'duo principalia' "$AREA/duo.log" \
    || deficere "recusatio sine causa 'duo principalia'" "$AREA/duo.log"

if [ "$AGERE" = 0 ]; then
    echo "FUMUS: FACTUM (cursum, probatum, structum, recusatum; planta rubra)"
    echo "FUMUS: '-agere' addens fenestram quoque aperit et agitat"
    purgare
    echo "fumus briar: sanum"
    exit 0
fi

# ---- VII. agere: app vitrea per manus ----
echo "FUMUS: VII. bin/manus incipere $VITREUM_DIR/bin/salve_vitreum -vivum"
SESSIO="$( cd "$AREA" && "$RADIX/bin/manus" incipere "$VITREUM_DIR/bin/salve_vitreum" -vivum \
    2>"$AREA/manus.err" )" \
    || deficere "manus incipere defecit" "$AREA/manus.err"
PORTUS="$(printf '%s\n' "$SESSIO" | grep -oE '[0-9]{4,5}' | head -1)"
[ -n "$PORTUS" ] || deficere "portus e responso manus legi non potuit: [$SESSIO]" "$AREA/manus.err"
sublevare () { "$RADIX/bin/manus" -s "$PORTUS" finire >/dev/null 2>&1; }
"$RADIX/bin/manus" -s "$PORTUS" affordantiae -machina > "$AREA/afford.tsv" 2>"$AREA/afford.err"
N_AFF="$(wc -l < "$AREA/afford.tsv" | tr -d ' ')"
if [ "$N_AFF" -lt 1 ]; then
    sublevare
    deficere "affordantiae vacuae - bulla 'tange' non visa" "$AREA/afford.err"
fi
echo "FUMUS:    affordantiae: $N_AFF"
"$RADIX/bin/manus" -s "$PORTUS" premere-textum tange > "$AREA/premere.log" 2>&1 \
    || { sublevare; deficere "premere 'tange' defecit" "$AREA/premere.log"; }
CORPUS="$("$RADIX/bin/manus" -s "$PORTUS" -exspecta textus body 2>"$AREA/textus.err")"
sublevare
case "$CORPUS" in
    *"salve, munde"*) echo "FUMUS:    corpus: ...salve, munde" ;;
    *) deficere "corpus paginae 'salve, munde' non continet: [$CORPUS] - pons 'salve' tacuit" "$AREA/textus.err" ;;
esac

echo "FUMUS: FACTUM (cursum, probatum, structum, recusatum, planta rubra, actum)"
purgare
echo "fumus briar: sanum"
exit 0
