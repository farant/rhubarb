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
#   VII. -amalgama: ./salve.thistle -amalgama et ./derivatum.thistle
#        -amalgama plagulas unas scribunt; ordo compilationis in linea
#        II cuiusque (contractus tituli) per clang SOLAM cucurrit,
#        programma 'salve, munde' / 'longitudo 12' imprimit, probatio
#        amalgamae viridis - effugium vere sine include/ nec lib/
#   VIII. fragmenta (spec par. 3.4): ./fragmenta.thistle -> 'salve,
#        fragmenta: 6'; -probatio viridis; -partes fragmenta nominat
#        ('#otiosum non adhibitum', '#salutatio ... 49, 70')
#   IX.  VERITAS '#line': ./fragmentum_erratum.thistle aedificari non
#        potest et error clang lineam FRAGMENTI in .thistle nominat -
#        tabula linearum contextus vera; nihil aliud id probat
#   X.   fenestra NATIVA: project-specs/exempla/salutatio.thistle -struere
#        (ludus, lib/fenestra_macos.m in clausura) aedificatur et
#        nectitur - ordo planus fontes explicitos et frameworks fert;
#        fenestra NON aperitur (-struere solum)
#   XI.  facies (par. 4.6): ./salutatio2.thistle -html paginam scribit;
#        notae involucri consumptae, '#principale' adest, fragmentum
#        COMMUNE '#repositorium' usus DUOS fert (tergum-nexus grex est,
#        non numerus), contexta duo; plagula FRACTA (circulus) paginam
#        tamen dat causam nominans (lex F4)
#   XII. spectator sine fenestra (par. 4.7): briar-spectator -parare
#        paginam scribit et exit, et ea pagina '-html' paginae OCTETIM
#        aequalis est - artificium unum, binaria duo. (Area extra
#        repositorium iacet, ergo ambo corpus INFIXUM idem legunt;
#        intra arborem briar corpus DISCI legeret et clavis differret.)
#        Nulla probatio intra processum unum hoc videre potest.
#   XIII. fasciculus (par. 4.8): ./salve_vitreum.thistle -app scribit
#        salve_vitreum.app iuxta thistle; identitas (plutil)
#        org.rhubarb.briar.salve-vitreum, exsecutabile adest; icon
#        infixus X plagulas iconset dat (iconutil NUMERATUR: rc 0 etiam
#        in continente fracto); -app iterum proprium REPONIT (plagula
#        stala evanescit); -icon fons_256.png VII plagulas dat;
#        fasciculus ALIENUS (fasciculum_instrumentum) recusatur, intactus
#
# QUAE FENESTRAM VERAM APERIUNT ('-agere' solum; manu currenda, non in
# suite - scrinium poscunt):
#   XIV. app vitrea per bin/manus agitur: affordantiae >= I (bulla),
#        premere, textus corporis 'salve, munde' continet
#   XV.  spectator per bin/manus: pagina onerata ('#principale' in
#        corpore), symbolum derivatum premitur, tabella caput
#        'piscina.h' ostendit - PONS respondit
#   XVI. fasciculus per Launch Services: open salve_vitreum.app --args
#        -vivum -portus P (mensuratum 2026-09-14: portum aperit);
#        manus ADHAERET (applicationem non gignit) et affordantias
#        videt; finire sessionem SOLAM claudit, ergo processus per
#        viam fasciculi necatur; icon in Dock oculo inspicitur
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
# gradus XIII: fasciculus alienus struitur, plutil et iconutil oracula
if [ ! -x bin/fasciculum_instrumentum ]; then
    echo "FUMUS: bin/fasciculum_instrumentum abest - ./compile_tools.sh fasciculum_instrumentum prius" >&2
    exit 2
fi
for instr in plutil iconutil; do
    command -v "$instr" >/dev/null 2>&1 \
        || { echo "FUMUS: $instr abest (macOS) - gradus XIII eo eget" >&2; exit 2; }
done
[ -f "$RADIX/probationes/fixa/icones/fons_256.png" ] \
    || { echo "FUMUS: fixum abest: probationes/fixa/icones/fons_256.png" >&2; exit 2; }
FIXA="$RADIX/briar/probationes/fixa/thistle"
for f in salve punctum derivatum fragmenta salve_vitreum adversa/probatio_rubra adversa/duo_principalia adversa/fragmentum_erratum; do
    [ -f "$FIXA/$f.thistle" ] || { echo "FUMUS: fixum abest: $f.thistle" >&2; exit 2; }
done
[ -f "$RADIX/project-specs/exempla/salutatio.thistle" ] \
    || { echo "FUMUS: exemplum abest: project-specs/exempla/salutatio.thistle" >&2; exit 2; }
[ -f "$RADIX/project-specs/exempla/salutatio2.thistle" ] \
    || { echo "FUMUS: exemplum abest: project-specs/exempla/salutatio2.thistle" >&2; exit 2; }

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

cp "$FIXA/salve.thistle" "$FIXA/punctum.thistle" "$FIXA/derivatum.thistle" \
   "$FIXA/fragmenta.thistle" "$FIXA/salve_vitreum.thistle" \
   "$FIXA/adversa/probatio_rubra.thistle" "$FIXA/adversa/duo_principalia.thistle" \
   "$FIXA/adversa/fragmentum_erratum.thistle" \
   "$FIXA/adversa/fragmentum_circulus.thistle" \
   "$RADIX/project-specs/exempla/salutatio.thistle" \
   "$RADIX/project-specs/exempla/salutatio2.thistle" \
   "$RADIX/probationes/fixa/icones/fons_256.png" \
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

# ---- VII. -amalgama: plagula una, clang SOLA (linea II = ordo) ----
echo "FUMUS: VII. ./salve.thistle -amalgama, ./derivatum.thistle -amalgama (clang sola)"
for t in salve derivatum fragmenta; do
    ( cd "$AREA" && "./$t.thistle" -amalgama ) > "$AREA/${t}_amalgama.log" 2>&1 \
        || deficere "$t -amalgama defecit" "$AREA/${t}_amalgama.log"
    [ -f "$AREA/$t.c" ] && [ -f "$AREA/probatio_$t.c" ] \
        || deficere "amalgama $t.c / probatio_$t.c abest" "$AREA/${t}_amalgama.log"
    for p in "$t" "probatio_$t"; do
        ORDO="$(sed -n '2s/^ \* //p' "$AREA/$p.c")"
        case "$ORDO" in
            clang\ *) ;;
            *) deficere "linea II amalgamae $p.c non ordo clang: [$ORDO]" ;;
        esac
        ( cd "$AREA" && eval "$ORDO" ) > "$AREA/${p}_clang.log" 2>&1 \
            || deficere "amalgama $p.c clang sola non compilat" "$AREA/${p}_clang.log"
    done
    ( cd "$AREA" && "./probatio_$t" ) > "$AREA/${t}_amalgama_probatio.log" 2>&1 \
        || deficere "probatio amalgamae $t rubra" "$AREA/${t}_amalgama_probatio.log"
done
( cd "$AREA" && ./salve ) > "$AREA/salve_amalgama_cursus.log" 2>&1
grep -q 'salve, munde' "$AREA/salve_amalgama_cursus.log" \
    || deficere "amalgama salve: 'salve, munde' non impressum" "$AREA/salve_amalgama_cursus.log"
( cd "$AREA" && ./derivatum ) > "$AREA/derivatum_amalgama_cursus.log" 2>&1
grep -q 'longitudo 12' "$AREA/derivatum_amalgama_cursus.log" \
    || deficere "amalgama derivatum: 'longitudo 12' non impressum" "$AREA/derivatum_amalgama_cursus.log"
( cd "$AREA" && ./fragmenta ) > "$AREA/fragmenta_amalgama_cursus.log" 2>&1
grep -q 'salve, fragmenta: 6' "$AREA/fragmenta_amalgama_cursus.log" \
    || deficere "amalgama fragmenta: 'salve, fragmenta: 6' non impressum" "$AREA/fragmenta_amalgama_cursus.log"
echo "FUMUS:    amalgamae: $(wc -l < "$AREA/salve.c" | tr -d ' ') lineae salve.c, $(wc -l < "$AREA/derivatum.c" | tr -d ' ') derivatum.c, $(wc -l < "$AREA/fragmenta.c" | tr -d ' ') fragmenta.c"

# ---- VIII. fragmenta: textura (spec par. 3.4) ----
echo "FUMUS: VIII. ./fragmenta.thistle (+ -probatio, -partes)"
( cd "$AREA" && ./fragmenta.thistle ) > "$AREA/fragmenta.log" 2>&1 \
    || deficere "fragmenta.thistle defecit" "$AREA/fragmenta.log"
grep -q 'salve, fragmenta: 6' "$AREA/fragmenta.log" \
    || deficere "'salve, fragmenta: 6' non impressum" "$AREA/fragmenta.log"
( cd "$AREA" && ./fragmenta.thistle -probatio ) > "$AREA/fragmenta_probatio.log" 2>&1 \
    || deficere "fragmenta -probatio defecit" "$AREA/fragmenta_probatio.log"
( cd "$AREA" && ./fragmenta.thistle -partes ) > "$AREA/fragmenta_partes.log" 2>&1 \
    || deficere "fragmenta -partes defecit" "$AREA/fragmenta_partes.log"
grep -q '^#otiosum.*non adhibitum' "$AREA/fragmenta_partes.log" \
    || deficere "-partes '#otiosum ... non adhibitum' non nominat" "$AREA/fragmenta_partes.log"
grep -q '^#salutatio.*adhibitum:lineae 49, 70' "$AREA/fragmenta_partes.log" \
    || deficere "-partes '#salutatio ... 49, 70' non nominat" "$AREA/fragmenta_partes.log"

# ---- IX. VERITAS '#line': error clang lineam FRAGMENTI nominet ----
LINEA_ERRATI="$(grep -n 'non numerus' "$AREA/fragmentum_erratum.thistle" | head -1 | cut -d: -f1)"
echo "FUMUS: IX. ./fragmentum_erratum.thistle (error clang ad lineam $LINEA_ERRATI exspectatus)"
if ( cd "$AREA" && ./fragmentum_erratum.thistle ) > "$AREA/erratum.log" 2>&1; then
    deficere "fragmentum_erratum aedificatum est - error typi non visus" "$AREA/erratum.log"
fi
grep -q "fragmentum_erratum.thistle:${LINEA_ERRATI}:" "$AREA/erratum.log" \
    || deficere "error clang lineam fragmenti ($LINEA_ERRATI) non nominat - tabula linearum mentitur" "$AREA/erratum.log"

# ---- X. fenestra NATIVA (Objective-C in clausura): -struere solum ----
echo "FUMUS: X. ./salutatio.thistle -struere (fenestra nativa, .m + frameworks; non aperitur)"
SALUTATIO_DIR="$( cd "$AREA" && ./salutatio.thistle -struere 2>"$AREA/salutatio.err" | tail -1 )" \
    || deficere "salutatio -struere defecit" "$AREA/salutatio.err"
[ -x "$SALUTATIO_DIR/bin/salutatio" ] \
    || deficere "binarium nativum abest: $SALUTATIO_DIR/bin/salutatio" "$AREA/salutatio.err"
grep -q 'lib/fenestra_macos.m' "$SALUTATIO_DIR/aedificare.sh" \
    || deficere "aedificare.sh salutationis fenestra_macos.m non nominat" "$SALUTATIO_DIR/aedificare.sh"
if grep -q 'lib/\*\.c' "$SALUTATIO_DIR/aedificare.sh"; then
    deficere "aedificare.sh salutationis globum 'lib/*.c' adhuc fert" "$SALUTATIO_DIR/aedificare.sh"
fi

# ---- XI. facies: pagina litterata (-html; nihil aperit) ----
echo "FUMUS: XI. ./salutatio2.thistle -html (pagina litterata)"
PAGINA="$( cd "$AREA" && ./salutatio2.thistle -html 2>"$AREA/facies.err" | tail -1 )" \
    || deficere "salutatio2 -html defecit" "$AREA/facies.err"
[ -s "$PAGINA" ] || deficere "pagina vacua aut absens: $PAGINA" "$AREA/facies.err"
grep -q '<!--@' "$PAGINA" \
    && deficere "nota involucri non consumpta in pagina" "$PAGINA"
grep -q 'id="frag-principale"' "$PAGINA" \
    || deficere "sedes fragmenti #principale abest" "$PAGINA"
# fragmentum COMMUNE: usus DUO (programma et probatio) - hic probatur
# quod tergum-nexus GREX est, non numerus unus
grep -q 'id="frag-repositorium"' "$PAGINA" \
    || deficere "sedes fragmenti #repositorium abest" "$PAGINA"
# caput fragmenti linea UNA post sedem sequitur; nexus in eo numerantur
N_USUS="$( grep -A1 'id="frag-repositorium"' "$PAGINA" | grep 'fr-usus' \
    | grep -o 'href="#l[0-9]*"' | wc -l | tr -d ' ' )"
[ "$N_USUS" = 2 ] \
    || deficere "#repositorium usus II exspectati (programma + probatio), inventi $N_USUS" "$PAGINA"
# numerare, numquam absentiam: contexta = radices duae
N_CONTEXTA="$( grep -c '<details class="fr-contextum"' "$PAGINA" )"
[ "$N_CONTEXTA" = 2 ] \
    || deficere "contexta II exspectata (radix + probatio), inventa $N_CONTEXTA" "$PAGINA"
# insula symbolorum: capita derivata quae plagula nusquam scribit
grep -q '"caput":"piscina.h"' "$PAGINA" \
    || deficere "insula symbolorum piscina.h non nominat" "$PAGINA"
# plagula FRACTA paginam tamen dat, causam nominans (lex F4)
PAGINA_FRACTA="$( cd "$AREA" && ./fragmentum_circulus.thistle -html 2>"$AREA/facies_fracta.err" | tail -1 )" \
    || deficere "circulus -html defecit (pagina semper reddi debet)" "$AREA/facies_fracta.err"
[ -s "$PAGINA_FRACTA" ] || deficere "pagina plagulae fractae vacua" "$AREA/facies_fracta.err"
grep -q 'circulus' "$PAGINA_FRACTA" \
    || deficere "pagina fracta causam 'circulus' non nominat" "$PAGINA_FRACTA"
echo "FUMUS:    pagina: $( wc -c < "$PAGINA" | tr -d ' ' ) octeti, contexta $N_CONTEXTA, #repositorium usus $N_USUS"

# ---- XII. spectator: identitas paginae (sine fenestra) ----
# Area EXTRA repositorium est, ergo briar et spectator corpus INFIXUM
# idem legunt - et identitas octetorum sensum habet. (Intra arborem
# briar corpus DISCI legit, ergo stampa et clavis differunt: identitas
# 'eodem corpore' valet, non absolute.)
if command -v briar-spectator >/dev/null 2>&1; then
    echo "FUMUS: XII. briar-spectator -parare (identitas contra -html)"
    SPEC_PAGINA="$( cd "$AREA" && briar-spectator -parare salutatio2.thistle 2>"$AREA/spectator.err" | tail -1 )" \
        || deficere "spectator -parare defecit" "$AREA/spectator.err"
    [ -s "$SPEC_PAGINA" ] || deficere "pagina spectatoris vacua" "$AREA/spectator.err"
    cmp -s "$SPEC_PAGINA" "$AREA/salutatio2.html" \
        || deficere "pagina spectatoris et pagina '-html' DIFFERUNT - artificium unum esse debet" "$AREA/spectator.err"
    echo "FUMUS:    identitas: $( wc -c < "$SPEC_PAGINA" | tr -d ' ' ) octeti, octetim aequales"
else
    echo "FUMUS: XII. OMISSA (briar-spectator non institutus)"
fi

# ---- XIII. fasciculus: -app (par. 4.8; nihil aperit) ----
echo "FUMUS: XIII. ./salve_vitreum.thistle -app (fasciculus cum icone)"
APP="$( cd "$AREA" && ./salve_vitreum.thistle -app 2>"$AREA/app.err" | tail -1 )"
case "$APP" in
    */salve_vitreum.app) ;;
    *) deficere "fasciculus non iuxta thistle: [$APP]" "$AREA/app.err" ;;
esac
[ -d "$APP" ] || deficere "fasciculus abest: $APP" "$AREA/app.err"
ID="$(plutil -extract CFBundleIdentifier raw "$APP/Contents/Info.plist" 2>/dev/null)"
[ "$ID" = "org.rhubarb.briar.salve-vitreum" ] \
    || deficere "identitas [$ID], exspectata org.rhubarb.briar.salve-vitreum" "$APP/Contents/Info.plist"
[ -x "$APP/Contents/MacOS/salve_vitreum" ] \
    || deficere "exsecutabile in fasciculo abest" "$AREA/app.err"
ICNS="$APP/Contents/Resources/salve_vitreum.icns"
# iconutil rc=0 etiam in continente fracto (icones par. 12.3): NUMERUS
iconutil -c iconset "$ICNS" -o "$AREA/ordinarius.iconset" >/dev/null 2>&1
N_ORD="$(ls "$AREA/ordinarius.iconset" 2>/dev/null | wc -l | tr -d ' ')"
[ "$N_ORD" = 10 ] \
    || deficere "icon ordinarius (MXXIV px): X plagulae exspectatae, inventae $N_ORD" "$AREA/app.err"

# proprius REPONITUR: plagula stala evanescit
touch "$APP/Contents/Resources/vetus.txt"
( cd "$AREA" && ./salve_vitreum.thistle -app ) > "$AREA/app2.log" 2>&1 \
    || deficere "-app iteratum (fasciculus proprius) defecit" "$AREA/app2.log"
[ -f "$APP/Contents/Resources/vetus.txt" ] \
    && deficere "plagula stala superest - fasciculus non repositus" "$AREA/app2.log"

# -icon: forma vexilli (CCLVI px -> VII plagulae, mensuratum 2026-09-14)
( cd "$AREA" && "$BRIAR" -app -icon fons_256.png salve_vitreum.thistle ) \
    > "$AREA/app_icon.log" 2>&1 || deficere "-app -icon defecit" "$AREA/app_icon.log"
iconutil -c iconset "$ICNS" -o "$AREA/proprius.iconset" >/dev/null 2>&1
N_PROP="$(ls "$AREA/proprius.iconset" 2>/dev/null | wc -l | tr -d ' ')"
[ "$N_PROP" = 7 ] \
    || deficere "icon CCLVI px: VII plagulae exspectatae, inventae $N_PROP" "$AREA/app_icon.log"

# ALIENUS recusatur et intactus manet
mkdir -p "$AREA/alienum" && cp "$AREA/salve_vitreum.thistle" "$AREA/alienum/"
"$RADIX/bin/fasciculum_instrumentum" -radix "$AREA/alienum/salve_vitreum.app" \
    -identitas org.aliud.alienum -exsecutabile /bin/echo > "$AREA/alienum.log" 2>&1 \
    || deficere "fasciculus alienus creari non potuit" "$AREA/alienum.log"
if ( cd "$AREA/alienum" && "$BRIAR" -app salve_vitreum.thistle ) > "$AREA/alienum_app.log" 2>&1; then
    deficere "fasciculus ALIENUS repositus est" "$AREA/alienum_app.log"
fi
grep -q 'alienus' "$AREA/alienum_app.log" \
    || deficere "recusatio 'alienus' non nominat" "$AREA/alienum_app.log"
[ "$(plutil -extract CFBundleIdentifier raw "$AREA/alienum/salve_vitreum.app/Contents/Info.plist")" = org.aliud.alienum ] \
    || deficere "fasciculus alienus mutatus est" "$AREA/alienum_app.log"
echo "FUMUS:    fasciculus: $ID, icon $N_ORD/$N_PROP plagulae, proprius repositus, alienus recusatus"

if [ "$AGERE" = 0 ]; then
    echo "FUMUS: FACTUM (cursum, probatum, structum, recusatum, planta rubra, amalgamatum, contextum, #line verum, nativum ligatum, facies, spectator, fasciculus)"
    echo "FUMUS: '-agere' addens fenestram quoque aperit et agitat"
    purgare
    echo "fumus briar: sanum"
    exit 0
fi

# ---- XIV. agere: app vitrea per manus ----
echo "FUMUS: XIV. bin/manus incipere $VITREUM_DIR/bin/salve_vitreum -vivum"
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

# ---- XV. agere: spectator, pagina et PONS ----
if command -v briar-spectator >/dev/null 2>&1; then
    echo "FUMUS: XV. briar-spectator salutatio2.thistle (fenestra + pons)"
    SPEC_SESSIO="$( cd "$AREA" && "$RADIX/bin/manus" incipere \
        "$(command -v briar-spectator)" "$AREA/salutatio2.thistle" -vivum \
        2>"$AREA/spec_manus.err" )" \
        || deficere "spectator per manus incipere defecit" "$AREA/spec_manus.err"
    SPEC_PORTUS="$(printf '%s\n' "$SPEC_SESSIO" | grep -oE '[0-9]{4,5}' | head -1)"
    [ -n "$SPEC_PORTUS" ] \
        || deficere "portus spectatoris legi non potuit: [$SPEC_SESSIO]" "$AREA/spec_manus.err"
    spec_sublevare () { "$RADIX/bin/manus" -s "$SPEC_PORTUS" finire >/dev/null 2>&1; }
    SPEC_CORPUS="$("$RADIX/bin/manus" -s "$SPEC_PORTUS" -exspecta textus body 2>"$AREA/spec_textus.err")"
    case "$SPEC_CORPUS" in
        *"principale"*) echo "FUMUS:    pagina onerata" ;;
        *) spec_sublevare
           deficere "corpus paginae spectatoris '#principale' non fert" "$AREA/spec_textus.err" ;;
    esac
    # PONS: symbolum derivatum premere -> definitio VERA ex corpore
    "$RADIX/bin/manus" -s "$SPEC_PORTUS" premere-textum \
        piscina_generare_dynamicum > "$AREA/spec_premere.log" 2>&1 \
        || { spec_sublevare; deficere "symbolum premere defecit" "$AREA/spec_premere.log"; }
    SPEC_POST="$("$RADIX/bin/manus" -s "$SPEC_PORTUS" -exspecta textus body 2>"$AREA/spec_post.err")"
    spec_sublevare
    case "$SPEC_POST" in
        *"piscina.h"*) echo "FUMUS:    pons respondit: piscina.h" ;;
        *) deficere "tabella caput 'piscina.h' non ostendit - pons tacuit" "$AREA/spec_post.err" ;;
    esac
else
    echo "FUMUS: XV. OMISSA (briar-spectator non institutus)"
fi

# ---- XVI. agere: fasciculus per Launch Services ----
echo "FUMUS: XVI. open salve_vitreum.app (Launch Services, -portus)"
PORTUS_APP=18765
# Launch Services stdout applicationis abicit: in aream dirigitur
open -n --stdout "$AREA/app_vivum.out" --stderr "$AREA/app_vivum.err" \
    "$APP" --args -vivum -portus "$PORTUS_APP" -retro \
    || deficere "open fasciculi defecit"
# manus applicationem NON gignit: 'adhaerere' sessionem portui ligat
# ('-s P' sine adhaesione = 'sessio ignota'); iteratur donec respondeat
fasciculum_necare () { pkill -f 'salve_vitreum.app/Contents/MacOS/salve_vitreum' >/dev/null 2>&1; }
k=0
until "$RADIX/bin/manus" adhaerere "$PORTUS_APP" >/dev/null 2>&1 \
      && "$RADIX/bin/manus" -s "$PORTUS_APP" affordantiae -machina > "$AREA/app_afford.tsv" 2>"$AREA/app_afford.err"; do
    k=$((k + 1))
    if [ "$k" -ge 20 ]; then
        fasciculum_necare
        deficere "fasciculus apertus in portu $PORTUS_APP non respondit" "$AREA/app_vivum.out"
    fi
    sleep 0.5
done
N_APP="$(wc -l < "$AREA/app_afford.tsv" | tr -d ' ')"
# finire sessionem ADHAESAM solam claudit (mensuratum): processus necandus
"$RADIX/bin/manus" -s "$PORTUS_APP" finire >/dev/null 2>&1
fasciculum_necare
[ "$N_APP" -ge 1 ] || deficere "fasciculus apertus: affordantiae vacuae" "$AREA/app_afford.err"
echo "FUMUS:    fasciculus apertus, affordantiae $N_APP"
echo "FUMUS:    ASPICE MANU: icon (discus fons_256) in Dock - A8 mensuratum 2026-09-14: statim renovatur"

echo "FUMUS: FACTUM (cursum, probatum, structum, recusatum, planta rubra, amalgamatum, contextum, #line verum, nativum ligatum, facies, spectator, fasciculus, actum)"
purgare
echo "fumus briar: sanum"
exit 0
