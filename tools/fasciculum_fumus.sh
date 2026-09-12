#!/bin/bash

# tools/fasciculum_fumus.sh - FUMUS: fasciculus .app a bibliotheca
# structus a macOS VERE accipitur et duplici ictu APERITUR?
#
# CUR HAEC PORTA EXSISTIT
# =======================
# probatio_fasciculum CXXXII assertiones fert et quaestionem unam quae
# revera refert respondere NON POTEST: an Finder fasciculum aperiat.
# lsregister et mdls tabulam usoris mutant; 'open' actus SCRINII est.
# Ergo suita hermetica manet et haec quaestio HIC, ubi homo spectare
# potest, rogatur (forma tools/briar_fumus.sh -agere).
#
# QUID PROBAT (sine -agere; nihil aperitur)
#   I.   instrumentum fasciculum cum icone struit -> semita impressa
#   II.  plutil -lint plistam GENITAM accipit (par. VIII: oraculum
#        alienum, machina nulla nova)
#   III. arbor: Contents/Info.plist, Contents/MacOS/<exs> cum bite +x,
#        Contents/Resources/<icon>
#   IV.  plutil -extract CFBundleExecutable: APPLE clavem nostram
#        legit - testimonium fortius quam lector noster sibi consentiens
#   V.   lector noster fasciculum relegit: identitas, titulus, versio
#   VI.  ADVERSA a: exsecutabile absens -> RECUSATUM EXSECUTABILE
#        (porta quae recusationem non videt mortua est)
#   VII. ADVERSA b: plista DEPRAVATA -> plutil -lint DEFICIT
#        (aliter lint semper 'OK' diceret et nihil iudicaret)
#
# QUOD SCRINIUM POSCIT ('-agere' solum)
#   VIII. open <Foo.app> et SENTINELLA exspectatur: binarium fasciculi
#        plagulam iuxta se ipsum scribit (via ex argv[0]), ergo
#        'apertusne est?' RES est, non quod quis vidit. NOTA: 'open'
#        per launchd transit, quod ambitum nostrum NON heredat - ideo
#        sentinella in area temporaria iacet, non in $HOME.
#
# Usus:
#   ./tools/fasciculum_fumus.sh [-agere] [-servare]
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM (praerequisita desunt).
# Linea ultima: 'fumus fasciculum: sanum' | 'fumus fasciculum: FRACTUM'.

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

INSTRUMENTUM="$RADIX/bin/fasciculum_instrumentum"
if [ ! -x "$INSTRUMENTUM" ]; then
    echo "FUMUS: bin/fasciculum_instrumentum abest -" \
         "./compile_tools.sh fasciculum_instrumentum prius" >&2
    exit 2
fi
if [ -z "$(ls -A "$RADIX/build"/*.o 2>/dev/null)" ]; then
    echo "FUMUS: build/*.o absunt - ./compile_tests.sh prius" >&2
    exit 2
fi
command -v plutil >/dev/null 2>&1 \
    || { echo "FUMUS: plutil abest (macOS poscitur)" >&2; exit 2; }
command -v clang >/dev/null 2>&1 \
    || { echo "FUMUS: clang abest" >&2; exit 2; }

AREA="$(mktemp -d /tmp/fasciculum_fumus.XXXXXX)" || exit 2
FASC="$AREA/Probatio.app"
SENTINELLA="$FASC/Contents/MacOS/salve_fasciculum.vixit"

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
    echo "fumus fasciculum: FRACTUM"
    exit 1
}

echo "FUMUS: area $AREA"

# ---- binarium SENTINELLAE: plagulam iuxta se ipsum scribit ----
# Fixum est, non codex domus: nihil ex arbore includit, ut fasciculus
# ipse nullo strato domestico ad currendum egeat. Viam ex argv[0]
# derivat quia launchd ambitum nostrum non heredat.
cat > "$AREA/salve_fasciculum.c" <<'FINIS_C'
#include <stdio.h>
#include <string.h>

int main (int numerus, char** argumenta)
{
    char via[2048];
    FILE* plagula;

    if (numerus < 1 || argumenta[0] == NULL)
    {
        return 1;
    }
    if (strlen(argumenta[0]) + 8 >= sizeof(via))
    {
        return 1;
    }
    strcpy(via, argumenta[0]);
    strcat(via, ".vixit");
    plagula = fopen(via, "w");
    if (plagula == NULL)
    {
        return 1;
    }
    fputs("vixi\n", plagula);
    fclose(plagula);
    printf("vixi\n");
    return 0;
}
FINIS_C

source "$RADIX/tools/vexilla.sh"
clang "${VEXILLA_C89[@]}" "$AREA/salve_fasciculum.c" \
    -o "$AREA/salve_fasciculum" > "$AREA/sentinella.log" 2>&1 \
    || deficere "binarium sentinellae compilari non potuit" \
                "$AREA/sentinella.log"

# icon FICTUS: fasciculum octetos ponit, non iudicat (spec par. XI)
printf 'icns\0\0\0\0' > "$AREA/probatio.icns"

# ---- I. struere ----
echo "FUMUS: I. instrumentum -radix Probatio.app (cum icone)"
"$INSTRUMENTUM" -radix "$FASC" \
    -identitas "org.rhubarb.probatio.fumus" \
    -titulus "Probatio" \
    -exsecutabile "$AREA/salve_fasciculum" \
    -icon "$AREA/probatio.icns" \
    -versio "1.2" -aedificatio "7" \
    > "$AREA/struere.log" 2>&1 \
    || deficere "instrumentum fasciculum struere non potuit" \
                "$AREA/struere.log"
grep -q "Probatio.app" "$AREA/struere.log" \
    || deficere "semita fasciculi non impressa" "$AREA/struere.log"

PLISTA="$FASC/Contents/Info.plist"
EXS="$FASC/Contents/MacOS/salve_fasciculum"
ICON="$FASC/Contents/Resources/probatio.icns"

# ---- II. plutil -lint: oraculum ALIENUM ----
echo "FUMUS: II. plutil -lint Info.plist"
plutil -lint "$PLISTA" > "$AREA/lint.log" 2>&1 \
    || deficere "plutil plistam GENITAM recusavit" "$AREA/lint.log"

# ---- III. arbor et bit +x ----
echo "FUMUS: III. arbor fasciculi"
[ -f "$PLISTA" ] || deficere "Info.plist abest: $PLISTA"
[ -f "$EXS" ]    || deficere "exsecutabile abest: $EXS"
[ -f "$ICON" ]   || deficere "icon abest: $ICON"
[ -x "$EXS" ] \
    || deficere "exsecutabile bitem +x NON fert - fasciculus duplici ictu taceret: $EXS"
if [ -x "$PLISTA" ]; then
    deficere "Info.plist exsecutabilis est - modus non selectivus"
fi
echo "FUMUS:    $(find "$FASC" -type f | wc -l | tr -d ' ') plagulae in fasciculo"

# ---- IV. APPLE clavem nostram legat ----
echo "FUMUS: IV. plutil -extract CFBundleExecutable"
NOMEN_EXS="$(plutil -extract CFBundleExecutable raw -o - "$PLISTA" \
    2>"$AREA/extract.err")" \
    || deficere "plutil CFBundleExecutable extrahere non potuit" \
                "$AREA/extract.err"
[ "$NOMEN_EXS" = "salve_fasciculum" ] \
    || deficere "CFBundleExecutable = [$NOMEN_EXS], 'salve_fasciculum' exspectatum"
NOMEN_ICON="$(plutil -extract CFBundleIconFile raw -o - "$PLISTA" \
    2>/dev/null)"
[ "$NOMEN_ICON" = "probatio.icns" ] \
    || deficere "CFBundleIconFile = [$NOMEN_ICON], 'probatio.icns' exspectatum"

# ---- V. lector noster ----
echo "FUMUS: V. instrumentum -legere"
"$INSTRUMENTUM" -legere "$FASC" > "$AREA/legere.log" 2>&1 \
    || deficere "lector fasciculum relegere non potuit" \
                "$AREA/legere.log"
grep -q "identitas: org.rhubarb.probatio.fumus" "$AREA/legere.log" \
    || deficere "identitas relecta non congruit" "$AREA/legere.log"
grep -q "titulus: Probatio" "$AREA/legere.log" \
    || deficere "titulus relectus non congruit" "$AREA/legere.log"
grep -q "versio: 1.2" "$AREA/legere.log" \
    || deficere "versio relecta non congruit" "$AREA/legere.log"

# ---- VI. ADVERSA a: exsecutabile absens RECUSETUR ----
echo "FUMUS: VI. adversa: exsecutabile absens (RECUSATIO exspectata)"
if "$INSTRUMENTUM" -radix "$AREA/Nusquam.app" \
        -identitas "org.rhubarb.nusquam" \
        -exsecutabile "$AREA/nusquam_xyz" \
        > "$AREA/adversa.log" 2>&1; then
    deficere "exsecutabile absens NON recusatum - porta rubrum non videt" \
             "$AREA/adversa.log"
fi
grep -q 'RECUSATUM EXSECUTABILE' "$AREA/adversa.log" \
    || deficere "recusatio statum EXSECUTABILE non nominat" \
                "$AREA/adversa.log"
[ -d "$AREA/Nusquam.app" ] \
    && deficere "fasciculus creatus est quamquam recusatum (reddere purum non fuit)"

# ---- VII. ADVERSA b: lint ipse rubrum videre DEBET ----
echo "FUMUS: VII. adversa: plista depravata (plutil DEFICERE debet)"
cp -R "$FASC" "$AREA/Depravata.app" || exit 2
printf '<?xml version="1.0"?>\n<plist><dict><key>' \
    > "$AREA/Depravata.app/Contents/Info.plist"
if plutil -lint "$AREA/Depravata.app/Contents/Info.plist" \
        > "$AREA/lint_fracta.log" 2>&1; then
    deficere "plutil plistam DEPRAVATAM accepit - lint nihil iudicat" \
             "$AREA/lint_fracta.log"
fi

if [ "$AGERE" = 0 ]; then
    echo "FUMUS: FACTUM (structum, lintatum, arbor, Apple legit, relectum, bis adversum)"
    echo "FUMUS: fasciculus NON apertus est - actus scrinii est."
    echo "FUMUS: manu:  open '$FASC'"
    echo "FUMUS:        deinde exspecta plagulam '$SENTINELLA'"
    echo "FUMUS: aut:   $0 -agere   (aperit et sentinellam exspectat)"
    SERVARE=1
    purgare
    echo "fumus fasciculum: sanum"
    exit 0
fi

# ---- VIII. -agere: APERIRE et sentinellam exspectare ----
echo "FUMUS: VIII. open $FASC (SCRINIUM)"
rm -f "$SENTINELLA"
open "$FASC" > "$AREA/open.log" 2>&1 \
    || deficere "'open' fasciculum recusavit" "$AREA/open.log"

MORA=0
while [ "$MORA" -lt 20 ]; do
    if [ -f "$SENTINELLA" ]; then
        break
    fi
    sleep 1
    MORA=$((MORA + 1))
done

if [ ! -f "$SENTINELLA" ]; then
    echo "FUMUS: sentinella post ${MORA}s ABEST: $SENTINELLA" >&2
    echo "FUMUS: (fasciculus fortasse apertus est sed non cucurrit -" \
         "Console.app et 'log show --last 2m' causam nominant)" >&2
    deficere "fasciculus non cucurrit - AUDIENDUM specificationis RUBRUM"
fi
echo "FUMUS:    sentinella adest post ${MORA}s: $(cat "$SENTINELLA")"

echo "FUMUS: FACTUM (structum, lintatum, arbor, Apple legit, relectum, bis adversum, APERTUM)"
purgare
echo "fumus fasciculum: sanum"
exit 0
