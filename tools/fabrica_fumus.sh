#!/bin/bash
# tools/fabrica_fumus.sh - porta natalis bin/fabrica (plan 1a T3; porta
# in PORTAE ab T8). Radices temporariae (cwd = radix fabricae) pro
# casibus fractis; arbor viva pro iudice ipso.
#
#   I    sine argumentis -> 2 (usus)
#   II   vexillum ignotum -> 2
#   III  radix sine subsystematibus -> 2 'nihil iudicatum'
#   IV   subsystema declaratum sine aedificatio.stml -> 2, nominatum
#   V    titulus duplex TRANS subsystemata -> 2, ambae sedes
#   VI   arbor viva: iudex ipse PRIMA linea (IGNOTUM usque ad T7)
#   VII  digestum fabrica -> LXIV hex; actio ignota -> 2
#   VIII manifestum cuius scopus abest -> ORPHANUM nominatum (et sub
#        -plenus: vexillum filtrum non est)
#   IX   iudex PRIMUS etiam post artificium stalum prius declaratum
#
# Exitus: 0 sanum · 1 fractum · 2 bin/fabrica deest.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
F="$RADIX/bin/fabrica"
[ -x "$F" ] || { echo "fumus fabricae: bin/fabrica deest (./tools/fabrica_struere.sh)"; exit 2; }
T="$(mktemp -d)"
ORPH="build/aedilis/fumus_orphanum_fabricae"
trap 'rm -rf "$T" "$ORPH"' EXIT
fracta=0

"$F" > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q '^usus:' "$T/o"; then echo "  I    sine argumentis -> 2            OK"; else echo "  I    FRACTUM (rc=$rc)"; fracta=1; fi

"$F" iudicare -xyz > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q 'vexillum ignotum' "$T/o"; then echo "  II   vexillum ignotum -> 2           OK"; else echo "  II   FRACTUM (rc=$rc)"; fracta=1; fi

# radices temporariae: fabrica.stml + aedilis.stml (existentia sola)
radix () { rm -rf "$T/r"; mkdir -p "$T/r"; : > "$T/r/aedilis.stml"; printf '%s\n' "$1" > "$T/r/fabrica.stml"; }
actio () { printf '<aedificatio>\n  <actio titulus="%s" genus="generator">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' "$1"; }

radix '<fabrica titulus="t"/>'
(cd "$T/r" && "$F" iudicare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q 'nihil iudicatum' "$T/o"; then echo "  III  sine subsystematibus -> 2        OK"; else echo "  III  FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

radix '<fabrica titulus="t"><subsystema via="deest"/></fabrica>'
(cd "$T/r" && "$F" iudicare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q 'sine declarationibus: deest/aedificatio.stml' "$T/o"; then echo "  IV   subsystema sine declaratione -> 2 OK"; else echo "  IV   FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

radix '<fabrica titulus="t"><subsystema via="p"/><subsystema via="q"/></fabrica>'
mkdir -p "$T/r/p" "$T/r/q"; actio gemina > "$T/r/p/aedificatio.stml"; actio gemina > "$T/r/q/aedificatio.stml"
(cd "$T/r" && "$F" iudicare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q "q/aedificatio.stml:2: titulus duplex 'gemina' (prior p/aedificatio.stml:2)" "$T/o"; then echo "  V    titulus duplex trans subsyst. -> 2 OK"; else echo "  V    FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

"$F" iudicare > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && head -1 "$T/o" | grep -q '^IGNOTUM bin/fabrica - sine provenientia'; then echo "  VI   iudex ipse prima linea          OK"; else echo "  VI   FRACTUM (rc=$rc)"; head -3 "$T/o"; fracta=1; fi

"$F" digestum fabrica > "$T/o" 2>&1; rc=$?
"$F" digestum nusquam_actio > "$T/o2" 2>&1; rc2=$?
if [ "$rc" -eq 0 ] && grep -qE '^[0-9a-f]{64}$' "$T/o" && [ "$rc2" -eq 2 ]; then echo "  VII  digestum: LXIV hex; ignota -> 2 OK"; else echo "  VII  FRACTUM (rc=$rc rc2=$rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

mkdir -p "$ORPH"
printf '<aedilis-manifestum\n    scopus="nusquam/fumus_fabricae.c"\n   varians="macos">\n</aedilis-manifestum>\n' > "$ORPH/manifestum.stml"
"$F" iudicare > "$T/o" 2>&1
# idem sub -plenus (vitium T4: quodvis argumentum, vexillum quoque,
# orphana celabat) - radix temporaria, ne regeneratio tota curratur
radix '<fabrica titulus="t"><subsystema via="p"/></fabrica>'
mkdir -p "$T/r/p" "$T/r/build/aedilis/orba"; actio sola > "$T/r/p/aedificatio.stml"
printf '<aedilis-manifestum scopus="nusquam.c" varians="macos">\n</aedilis-manifestum>\n' > "$T/r/build/aedilis/orba/manifestum.stml"
(cd "$T/r" && "$F" iudicare -plenus) > "$T/o2" 2>&1
if grep -q "^ORPHANUM: $ORPH/ (scopus nusquam/fumus_fabricae.c absens)" "$T/o" && grep -q '^ORPHANUM: build/aedilis/orba/' "$T/o2"; then echo "  VIII manifestum orphanum nominatum    OK"; else echo "  VIII FRACTUM"; grep ORPHANUM "$T/o" | head -3; fracta=1; fi

# IX - ordo: iudex PRIMUS etiam cum artificium stalum ANTE eum
# declaratum est (VI in arbore viva unum artificium solum habet -
# ordinem non probat)
radix '<fabrica titulus="t"><subsystema via="p"/><subsystema via="."/></fabrica>'
mkdir -p "$T/r/p"; actio prius > "$T/r/p/aedificatio.stml"; : > "$T/r/a"
printf '<aedificatio>\n  <actio titulus="fabrica" genus="institutio">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="bin/fabrica" provenientia="relatio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && head -1 "$T/o" | grep -q '^IGNOTUM bin/fabrica' && grep -q '^STALUM b\|^IGNOTUM b ' "$T/o"; then echo "  IX   iudex primus ante stala priora   OK"; else echo "  IX   FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

if [ "$fracta" -ne 0 ]; then echo "fumus fabricae: FRACTUM"; exit 1; fi
echo "fumus fabricae: sanum (IX/IX)"
exit 0
