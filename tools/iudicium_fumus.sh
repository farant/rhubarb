#!/bin/bash
# tools/iudicium_fumus.sh - porta natalis actionis 'iudicium' (fabrica
# spec 3 T7, par. VI et XIII). Radix temporaria: generator unus (gen ->
# build/gen.h ex gen/fons.txt) et porta ficta porta_x - porta.sh fontat
# lib.sh, src/a.c per bin/compilator VERUM compilat (lector per filum:
# vestigium verum), programma currit, et flag.txt per bash legit (lectio
# quam liber NON videt - auditus eam capere debet).
#
#   I    sanare: transitus servatus (nulla 'non servatus'), iudicare RECENS
#   P3   README mutatum -> RECENS (finis portae)
#   P1   caput src/a.h mutatum (per compilatorem lectum) -> STALUM nominans
#   P2   scriptum fontatum lib.sh mutatum -> IGNOTUM (clavis per fontationes)
#   P7   'source "$NESCIO"' in porta.sh -> IGNOTUM 'fontatio irresoluta'
#   P8   gen/fons.txt mutatum: sanare build/gen.h (ut porta() facit) ->
#        regeneratum, verdictum non reutilis (IGNOTUM: ingressus declaratus
#        clavem mutat); sanare -> currit, RECENS
#   P6   porta fracta: FRACTUM, verdictum absens; iterum sanare -> iterum
#        currit (defectus numquam servatur)
#   AUD  flag.txt (lectio bash, extra librum) mutatum sub transitu RECENS:
#        iudicare caecum RECENS; sanare -audit -> AUDITUM_DISCORS; restitutum
#        sanare -audit -> 'auditus: transitus iterum congruit'
#
# Alibi tecta (nominata, non repetita): P4 IO cruda -> lectiones_lint
# (planta T3); P5 FIFO, P9 ambitus, P10 relinkatio -> probatio_fabrica
# (transitus VIII/III, instrumentum_domus).
#
# Exitus: 0 sanum · 1 fractum · 2 instrumenta desunt.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
F="$RADIX/bin/fabrica"
C="$RADIX/bin/compilator"
for b in "$F" "$C"; do
    [ -x "$b" ] || { echo "fumus iudicii: $b deest"; exit 2; }
done
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
R="$T/r"
fracta=0
export FABRICA_FONTATIONES="$RADIX/crusta/fontationes.sh"
unset FABRICA_THESAURUS FABRICA_LECTIONES FABRICA_AUDITUS

mkdir -p "$R/bin" "$R/src" "$R/gen"
ln -s "$C" "$R/bin/compilator"
: > "$R/aedilis.stml"
printf '<fabrica titulus="fumus"><subsystema via="."/></fabrica>\n' > "$R/fabrica.stml"
cat > "$R/aedificatio.stml" <<'DECL'
<aedificatio>
  <actio titulus="gen" genus="generator">
    <mandatum>
      <verbum! (>./gen.sh
    </mandatum>
    <ingressus genus="fasciculus" via="gen.sh"/>
    <ingressus genus="fasciculus" via="gen/fons.txt"/>
    <exitus via="build/gen.h" provenientia="regeneratio"/>
  </actio>
  <actio titulus="porta_x" genus="iudicium" lectiones="verum">
    <mandatum>
      <verbum! (>./porta.sh
    </mandatum>
    <vestigium via="build/x"/>
    <ingressus genus="fontationes" via="porta.sh"/>
    <ingressus genus="instrumentum_domus" via="bin/compilator"/>
    <ingressus genus="identitas_clang" via="clang"/>
    <ingressus genus="fasciculus" via="build/gen.h"/>
    <exitus via="build/fabrica/verdicta/x.txt" provenientia="verdictum"/>
  </actio>
</aedificatio>
DECL
cat > "$R/gen.sh" <<'GEN'
#!/bin/bash
D="${FABRICA_SCRIPTURA:-.}"
mkdir -p "$D/build"
printf '#define GEN_VALOR %s\n' "$(cat gen/fons.txt)" > "$D/build/gen.h"
GEN
cat > "$R/lib.sh" <<'LIB'
# lib.sh - fontatum a porta.sh
FUMUS_LIB=1
LIB
cat > "$R/porta.sh" <<'PORTA'
#!/bin/bash
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
rm -f build/fabrica/verdicta/x.txt
mkdir -p build/x build/fabrica/verdicta
bin/compilator -std=c89 -Isrc -Ibuild -c src/a.c -o build/x/a.o || exit 1
clang build/x/a.o -o build/x/a || exit 1
./build/x/a || exit 1
[ "$(cat flag.txt)" = ok ] || exit 1
echo "x: sanum" > build/fabrica/verdicta/x.txt
PORTA
chmod +x "$R/gen.sh" "$R/porta.sh"
printf '/* a.h */\n' > "$R/src/a.h"
printf '#include "a.h"\n#include "gen.h"\nint main(void) { return GEN_VALOR < 0; }\n' > "$R/src/a.c"
printf '3\n' > "$R/gen/fons.txt"
printf 'ok\n' > "$R/flag.txt"
printf 'fumus\n' > "$R/README"
V="build/fabrica/verdicta/x.txt"

fab () { (cd "$R" && "$F" "$@") > "$T/o" 2>&1; }
status () { fab iudicare -omnia "$V"; grep -E "^(RECENS|STALUM|IGNOTUM) $V" "$T/o" | head -1; }
ok () { echo "  $1 OK"; }
non () { echo "  $1 FRACTUM"; sed 's/^/      /' "$T/o" | head -8; fracta=1; }

# I
fab sanare "$V"
if grep -q '^SANATUM     porta_x' "$T/o" && ! grep -q 'non servatus' "$T/o" \
   && status | grep -q '^RECENS'; then ok "I    transitus servatus, RECENS"; else non "I   "; fi

# P3
printf 'mutatum\n' >> "$R/README"
if status | grep -q '^RECENS'; then ok "P3   README mutatum -> RECENS"; else non "P3  "; fi

# P1
printf '/* mutatum */\n' >> "$R/src/a.h"
s="$(status)"
if echo "$s" | grep -q '^STALUM.*src/a.h'; then ok "P1   caput mutatum -> STALUM nominatum"; else echo "      $s"; non "P1  "; fi
printf '/* a.h */\n' > "$R/src/a.h"

# P2
printf '# mutatum\n' >> "$R/lib.sh"
if status | grep -q '^IGNOTUM'; then ok "P2   scriptum fontatum mutatum -> IGNOTUM"; else non "P2  "; fi
printf '# lib.sh - fontatum a porta.sh\nFUMUS_LIB=1\n' > "$R/lib.sh"

# P7
cp "$R/porta.sh" "$T/porta.bonum"
printf 'source "$NESCIO"\n' >> "$R/porta.sh"
fab iudicare -omnia "$V"
if grep -q "^IGNOTUM $V.*fontatio irresoluta" "$T/o"; then ok "P7   source \"\$X\" -> IGNOTUM nominatum"; else non "P7  "; fi
cp "$T/porta.bonum" "$R/porta.sh"
if status | grep -q '^RECENS'; then ok "     restitutum -> RECENS"; else non "     restitutum"; fi

# P8
printf '4\n' > "$R/gen/fons.txt"
fab sanare build/gen.h
s="$(status)"
# build/gen.h ingressus DECLARATUS est (ordo gen -> porta): clavis mutata =
# IGNOTUM; lectio sola (non declarata) STALUM daret - ambo 'non reutilis'
if grep -q 'GEN_VALOR 4' "$R/build/gen.h" && echo "$s" | grep -qE "^(STALUM|IGNOTUM) $V"; then ok "P8   generator: build/gen.h regeneratum, transitus non reutilis"; else echo "      $s"; non "P8  "; fi
fab sanare "$V"
if grep -q '^SANATUM     porta_x' "$T/o" && status | grep -q '^RECENS'; then ok "     sanare -> currit, RECENS"; else non "     sanare"; fi

# P6
printf -- '-1\n' > "$R/gen/fons.txt"
fab sanare "$V"; rc=$?
if [ "$rc" -eq 1 ] && grep -q 'FRACTUM     porta_x' "$T/o" && [ ! -f "$R/$V" ]; then ok "P6   porta fracta -> FRACTUM, verdictum absens"; else non "P6  "; fi
fab sanare "$V"
if grep -q 'FRACTUM     porta_x' "$T/o"; then ok "     iterum currit (defectus non servatus)"; else non "     iterum"; fi
printf '4\n' > "$R/gen/fons.txt"
fab sanare "$V"
if status | grep -q '^RECENS'; then ok "     restitutum -> RECENS"; else non "     restitutum"; fi

# AUD
printf 'non\n' > "$R/flag.txt"
if status | grep -q '^RECENS'; then ok "AUD  lectio extra librum: iudicium caecum RECENS"; else non "AUD caecum"; fi
fab sanare -audit "$V"; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^AUDITUM_DISCORS porta_x' "$T/o"; then ok "     sanare -audit -> AUDITUM_DISCORS"; else non "AUD discors"; fi
printf 'ok\n' > "$R/flag.txt"
fab sanare "$V"
fab sanare -audit "$V"
if grep -q 'auditus: transitus iterum congruit' "$T/o"; then ok "     restitutum: auditus congruit"; else non "AUD congruit"; fi

if [ "$fracta" -ne 0 ]; then echo "fumus iudicii: FRACTUM"; exit 1; fi
echo "fumus iudicii: sanum"
exit 0
