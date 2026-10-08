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
#   VI   arbor viva: iudex ipse RECENS (relatio T7)
#   VII  digestum fabrica -> LXIV hex; actio ignota -> 2
#   VIII manifestum cuius scopus abest -> ORPHANUM nominatum (et sub
#        -plenus: vexillum filtrum non est)
#   IX   iudex PRIMUS etiam post artificium stalum prius declaratum
#   X    -provenientia ex quovis directorio
#   XI   digestum == ingressus relatus (una functio sigilli)
#   XII  -plenus sub sera alius (build/fabrica/sera) -> 2, nominata;
#        sera liberata -> currit (T8: duo iudices plena clausuras
#        mutuo vacuaverant, T6)
#   XIII binaria in bin/ sine declaratione -> linea una numerata (Q36)
#   XIV  -tacta: via tangens -> generatum iudicatum, installatum NON;
#        via nulla tangens -> exitus 0 'nulla ... tacta' (commissio, T8)
#   XV   compositum nominatum: partes iudicantur, linea COMPOSITUM
#        pessimum nominat (pars stala), pars recens non nominatur
#        (plan 1b T2)
#   XVI  sanare: stalum -> SANATUM, in loco scriptum, deinde RECENS
#   XVII sanare: exitus 0 sine scriptura -> FRACTUM 'exitus 0 sed non
#        RECENS' (post-condicio; installator 'cp' larvatus, spec 0.6)
#   XVIII sanare: superior fractus -> inferior OMISSUM nominans eum
#   XIX  sanare sub sera aliena -> 2, sera nominata
#   XX   sanare cum iudice ipso non recenti -> 2, fabrica_struere.sh
#        nominatum, nihil actum (plan 1b T3)
#   XXI  sanare: scriptura extra vestigium -> FRACTUM nominans eam
#   XXII sanare -siccum: actiones independentes disiunctae -> unda una
#        (plan 1b T4)
#   XXIII sanare: FABRICA_AGIT=1 in actis (scripta productores
#        nidificatos omittunt; plan 1b T5 D3/D4)
#   XXIV copia ~/.bin (HOME in radice temporaria - ~/.bin VERUM numquam
#        tangitur): absens -> STALUM sub celeri; sanare -> instituta;
#        binarium mutatum -> STALUM sub celeri; copia impossibilis ->
#        FRACTUM per exitum (plan 1b T5, Q42)
#   XXV  cursus: actio sanata in build/fabrica.db scribitur; -siccum
#        postea tempus aestimat, non 'tempus ignotum' (plan 1b T7)
#   XXVI purgare: thesaurus manu structus (sigilla vera per shasum),
#        VI generationes - prima sola clavem k1 tenet: index primus,
#        actio k1, blobus eius deleti (III), cetera servata; -verificare
#        blobum servatum corruptum delet (plan 2 T3)
#   XXVII familia in disco vero: II plagulae congruentes -> II actiones
#        (titulus familia:basis), alia non; plagula nova -> actio nova
#        STALUM (exitus absens) (plan 2 T4)
#   XXVIII simul: IV actiones tutae (lectiones), II s singulae,
#        FABRICA_FILA=IV -> tempus < IV s (plan 2 T6)
#   XXIX simul fractura: una ex IV fracta -> ceterae SANATUM, dependens
#        OMISSUM nominans eam, exitus 1
#   XXX  non tuta numquam simul: intervallum eius nullum alium tangit
#   XXXI iudicium simul (T6b, praevisio): IV generatores tuti, II s sub
#        iudice singuli, -plenus FABRICA_FILA=IV -> < IV s, verdicta recta
#   ... XXXII-XXXVI: canon, census, repositorium, stadium iudicum (infra)
#   XXXVII probationes_c vere (fabrica-6 T6c): III membra (facultas: nexus
#        solum), RECENS, bibliotheca mutata -> membrum suum solum
#   XXXVIII probationes_c: cursus fractus -> FRACTUM, area orphana
#   XXXIX post et compositum (T7): producens primum, VERDICTUM N/M
#   XL   lineae machinae (H1): iudicare/sanare -machina, genera nota sola
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

# VI (T7): iudex ipse RECENS - relatio sua digesto hodierno congruit
"$F" iudicare bin/fabrica > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && ! grep -q '^STALUM\|^IGNOTUM' "$T/o"; then echo "  VI   iudex ipse recens (relatio)      OK"; else echo "  VI   FRACTUM (rc=$rc)"; head -3 "$T/o"; fracta=1; fi

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

# X (T7): '-provenientia' ex QUOVIS directorio (ante custodiam cwd)
(cd "$T" && "$F" -provenientia) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && [ "$(sed -n 1p "$T/o")" = "provenientia 1" ] && grep -q '^artificium bin/fabrica$' "$T/o"; then echo "  X    -provenientia extra radicem       OK"; else echo "  X    FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XI (T7): 'digestum' == ingressus relatus - scriptum (installator) et
# iudex EADEM functione sigillant (T7: _digestum olim exclusionem
# omittebat, tres digesta diversa)
D="$("$F" digestum fabrica 2>/dev/null)"
R="$("$F" -provenientia | sed -n 's/^ingressus //p')"
if [ -n "$D" ] && [ "$D" = "$R" ]; then echo "  XI   digestum == relatio binarii      OK"; else echo "  XI   FRACTUM (digestum $D, relatio $R)"; fracta=1; fi

# XII (T8): sera iudicii pleni - flock(2) per python (eadem sera
# nuclei ac filum_seram_capere), radix temporaria
radix '<fabrica titulus="t"><subsystema via="p"/></fabrica>'
mkdir -p "$T/r/p" "$T/r/build/fabrica"; actio sola > "$T/r/p/aedificatio.stml"
python3 -c 'import fcntl,sys,time
f=open(sys.argv[1],"a"); fcntl.flock(f,fcntl.LOCK_EX)
open(sys.argv[2],"w").close(); time.sleep(60)' "$T/r/build/fabrica/sera" "$T/tenet" &
TENENS=$!
n=0; while [ ! -f "$T/tenet" ] && [ "$n" -lt 100 ]; do sleep 0.1; n=$((n+1)); done
(cd "$T/r" && "$F" iudicare -plenus) > "$T/o" 2>&1; rc=$?
kill "$TENENS" 2>/dev/null; wait "$TENENS" 2>/dev/null
(cd "$T/r" && "$F" iudicare -plenus) > "$T/o2" 2>&1; rc2=$?
if [ "$rc" -eq 2 ] && grep -q 'build/fabrica/sera' "$T/o" && [ "$rc2" -ne 2 ]; then echo "  XII  -plenus sub sera aliena -> 2      OK"; else echo "  XII  FRACTUM (rc=$rc rc2=$rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

# XIII (Q36): bin/ sine declaratione numeratur, declarata non
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
mkdir -p "$T/r/bin"; : > "$T/r/bin/declaratum"; : > "$T/r/bin/ignotum"; : > "$T/r/a"
printf '<aedificatio>\n  <actio titulus="d" genus="institutio">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="bin/declaratum" provenientia="relatio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare) > "$T/o" 2>&1
if grep -q '^IGNOTUM: 1 binaria in bin/ sine declaratione' "$T/o"; then echo "  XIII bin/ sine declaratione numerata  OK"; else echo "  XIII FRACTUM"; cat "$T/o"; fracta=1; fi

# XIV (T8 gradus III): -tacta - generatum tactum iudicatur (hic
# IGNOTUM: mandatum vacuum), institutio eiusdem ingressus NON
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
: > "$T/r/a"; : > "$T/r/b"
printf '<aedificatio>\n  <actio titulus="g" genus="generator">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="i" genus="institutio">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="bin/i" provenientia="relatio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare -plenus -tacta a) > "$T/o" 2>&1; rc=$?
(cd "$T/r" && "$F" iudicare -plenus -tacta alia) > "$T/o2" 2>&1; rc2=$?
if [ "$rc" -eq 1 ] && grep -q '^IGNOTUM b ' "$T/o" && ! grep -q 'bin/i' "$T/o" \
   && [ "$rc2" -eq 0 ] && grep -q 'nulla artificia generata a viis tacta' "$T/o2"; then echo "  XIV  -tacta: generata tacta sola       OK"; else echo "  XIV  FRACTUM (rc=$rc rc2=$rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

# XV (1b T2): compositum - generatores scripti: b novum scribit (stalum
# contra 'vetus' commissum), c idem (recens)
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
: > "$T/r/a"; printf 'vetus\n' > "$T/r/b"; printf 'idem\n' > "$T/r/c"
printf 'printf "novum\\n" > "$FABRICA_SCRIPTURA/b"\n' > "$T/r/gen_b.sh"
printf 'printf "idem\\n" > "$FABRICA_SCRIPTURA/c"\n' > "$T/r/gen_c.sh"
printf '<aedificatio>\n  <actio titulus="g" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_b.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="h" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_c.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="c" provenientia="regeneratio"/>\n  </actio>\n  <compositum titulus="omnia">\n    <pars artificium="b"/>\n    <pars actio="h"/>\n  </compositum>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare -plenus omnia) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^STALUM b ' "$T/o" && grep -q '^COMPOSITUM omnia - STALUM: b$' "$T/o"; then echo "  XV   compositum: pessimum partium      OK"; else echo "  XV   FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XVI-XX (1b T3): sanare. Generator scriptus: sub iudice (FABRICA_SCRIPTURA)
# in scripturam, sub sanare in loco ('.')
sanare_radix () {
    radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
    : > "$T/r/a"; printf 'vetus\n' > "$T/r/b"; printf 'c:vetus\n' > "$T/r/c"
    printf '<aedificatio>\n  <actio titulus="g" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_b.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="h" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_c.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="b"/>\n    <exitus via="c" provenientia="regeneratio"/>\n  </actio>\n%b</aedificatio>\n' "$1" > "$T/r/aedificatio.stml"
    printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "c:%%s" "$(cat b)" > "$D/c"\n' > "$T/r/gen_c.sh"
}
sanare_radix ''
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare) > "$T/o" 2>&1; rc=$?
(cd "$T/r" && "$F" iudicare -plenus) > "$T/o2" 2>&1; rc2=$?
if [ "$rc" -eq 0 ] && grep -q '^SANATUM *g ' "$T/o" && grep -q '^SANATUM *h ' "$T/o" && [ "$(cat "$T/r/b")" = "novum" ] && [ "$rc2" -eq 0 ]; then echo "  XVI  sanare: stala sanata, deinde recentia OK"; else echo "  XVI  FRACTUM (rc=$rc rc2=$rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

sanare_radix ''
printf '[ -n "$FABRICA_SCRIPTURA" ] && printf "novum\\n" > "$FABRICA_SCRIPTURA/b"\nexit 0\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare b) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^FRACTUM *g .*exitus 0 sed non RECENS' "$T/o"; then echo "  XVII sanare: exitus 0 non RECENS -> FRACT. OK"; else echo "  XVII FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

sanare_radix ''
printf 'if [ -n "$FABRICA_SCRIPTURA" ]; then printf "novum\\n" > "$FABRICA_SCRIPTURA/b"; exit 0; fi\necho "fractura scripta" >&2; exit 3\n' > "$T/r/gen_b.sh"
printf 'c:alienum\n' > "$T/r/c"
(cd "$T/r" && "$F" sanare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^FRACTUM *g .*exitus 3: fractura scripta' "$T/o" && grep -q '^OMISSUM *h - dependentia fracta: g' "$T/o" && [ -f "$T/r/build/fabrica/acta/g.log" ]; then echo "  XVIII sanare: superior fractus -> OMISSUM OK"; else echo "  XVIII FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

sanare_radix ''
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
mkdir -p "$T/r/build/fabrica"; rm -f "$T/tenet"
python3 -c 'import fcntl,sys,time
f=open(sys.argv[1],"a"); fcntl.flock(f,fcntl.LOCK_EX)
open(sys.argv[2],"w").close(); time.sleep(60)' "$T/r/build/fabrica/sera" "$T/tenet" &
TENENS=$!
n=0; while [ ! -f "$T/tenet" ] && [ "$n" -lt 100 ]; do sleep 0.1; n=$((n+1)); done
(cd "$T/r" && "$F" sanare) > "$T/o" 2>&1; rc=$?
kill "$TENENS" 2>/dev/null; wait "$TENENS" 2>/dev/null
if [ "$rc" -eq 2 ] && grep -q 'build/fabrica/sera' "$T/o" && [ "$(cat "$T/r/b")" = "vetus" ]; then echo "  XIX  sanare sub sera aliena -> 2       OK"; else echo "  XIX  FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XX: radix declarat bin/fabrica (relatio) quod abest -> iudex non
# recens; generator in loco signum relinqueret si curreret
sanare_radix '  <actio titulus="fabrica" genus="institutio">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="bin/fabrica" provenientia="relatio"/>\n  </actio>\n'
printf 'D="${FABRICA_SCRIPTURA:-.}"; [ -z "$FABRICA_SCRIPTURA" ] && : > signum_acti; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 2 ] && grep -q 'fabrica_struere.sh' "$T/o" && [ ! -f "$T/r/signum_acti" ]; then echo "  XX   iudex non recens -> 2, nihil actum OK"; else echo "  XX   FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XXI (1b T4): g scribit b (exitus) ET alia.txt (extra vestigium)
sanare_radix ''
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"; [ -z "$FABRICA_SCRIPTURA" ] && : > alia.txt; exit 0\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare b) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^FRACTUM *g .*scripsit extra vestigium: alia.txt' "$T/o"; then echo "  XXI  scriptura extra vestigium -> FRACT. OK"; else echo "  XXI  FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XXII (1b T4): g (b) et k (d) independentes, ambae stalae -> unda una
sanare_radix '  <actio titulus="k" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_d.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="d" provenientia="regeneratio"/>\n  </actio>\n'
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/d"\n' > "$T/r/gen_d.sh"
printf 'vetus\n' > "$T/r/d"; printf 'c:vetus' > "$T/r/c"
(cd "$T/r" && "$F" sanare -siccum b d) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^  I: g k$' "$T/o" && [ "$(cat "$T/r/b")" = "vetus" ]; then echo "  XXII -siccum: undae (simul possent)   OK"; else echo "  XXII FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XXIII (1b T5): generator in loco SOLUM sub FABRICA_AGIT scribit
sanare_radix ''
printf 'if [ -z "$FABRICA_SCRIPTURA" ] && [ "${FABRICA_AGIT:-}" != 1 ]; then echo "FABRICA_AGIT deest" >&2; exit 4; fi\nD="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare b) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q '^SANATUM *g ' "$T/o"; then echo "  XXIII FABRICA_AGIT in actis          OK"; else echo "  XXIII FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

# XXIV (1b T5): copia ~/.bin; HOME redirectus, ~/.bin verum custoditur
VERUM_ANTE="$(shasum "$HOME/.bin/briar" 2>/dev/null)"
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
mkdir -p "$T/r/bin" "$T/r/tools" "$T/domus"
cp tools/instituere.sh "$T/r/tools/instituere.sh"
printf 'binarium I\n' > "$T/r/bin/x"
printf '<aedificatio>\n  <actio titulus="institutio_x" genus="institutio" celer="verum">\n    <mandatum>\n      <verbum! (>./tools/instituere.sh\n      <verbum! (>bin/x\n    </mandatum>\n    <ingressus genus="fasciculus" via="bin/x"/>\n    <exitus via="~/.bin/x" scriptura="x" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && HOME="$T/domus" "$F" iudicare) > "$T/o" 2>&1; rc1=$?
(cd "$T/r" && HOME="$T/domus" "$F" sanare) > "$T/o2" 2>&1; rc2=$?
COPIA=1; cmp -s "$T/r/bin/x" "$T/domus/.bin/x" && COPIA=0
(cd "$T/r" && HOME="$T/domus" "$F" iudicare) > "$T/o3" 2>&1; rc3=$?
printf 'binarium II\n' > "$T/r/bin/x"
(cd "$T/r" && HOME="$T/domus" "$F" iudicare) > "$T/o4" 2>&1; rc4=$?
chmod 555 "$T/domus/.bin"
(cd "$T/r" && HOME="$T/domus" "$F" sanare) > "$T/o5" 2>&1; rc5=$?
chmod 755 "$T/domus/.bin"
VERUM_POST="$(shasum "$HOME/.bin/briar" 2>/dev/null)"
if [ "$rc1" -eq 1 ] && grep -q '^STALUM ~/.bin/x - artificium absens' "$T/o" \
   && [ "$rc2" -eq 0 ] && [ "$COPIA" -eq 0 ] \
   && [ "$rc3" -eq 0 ] && [ "$rc4" -eq 1 ] && grep -q '^STALUM ~/.bin/x' "$T/o4" \
   && [ "$rc5" -eq 1 ] && grep -q '^FRACTUM *institutio_x .*exitus 1' "$T/o5" \
   && [ "$VERUM_ANTE" = "$VERUM_POST" ]; then echo "  XXIV copia ~/.bin (HOME redirectus)   OK"; else echo "  XXIV FRACTUM (rc=$rc1 $rc2 $rc3 $rc4 $rc5)"; cat "$T/o" "$T/o2" "$T/o3" "$T/o4" "$T/o5"; fracta=1; fi

# XXV (1b T7): cursus - sanare scribit, -siccum aestimat
sanare_radix ''
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "novum\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
(cd "$T/r" && "$F" sanare -siccum b) > "$T/o" 2>&1; rc1=$?
(cd "$T/r" && "$F" sanare b) > "$T/o2" 2>&1; rc2=$?
printf 'vetus\n' > "$T/r/b"
(cd "$T/r" && "$F" sanare -siccum b) > "$T/o3" 2>&1; rc3=$?
if [ "$rc1" -eq 1 ] && grep -q '^AGENDUM *g (tempus ignotum)' "$T/o" \
   && [ "$rc2" -eq 0 ] && [ "$rc3" -eq 1 ] && grep -q '^AGENDUM *g (~[0-9]*\.[0-9] s)' "$T/o3"; then echo "  XXV  cursus: siccum aestimat         OK"; else echo "  XXV  FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/o" "$T/o2" "$T/o3"; fracta=1; fi

# XXVI (plan 2 T3): purgare - generationes servatae V (A3)
radix '<fabrica titulus="t"/>'
O="$T/r/build/aedilis/obiecta"
pone_blobum () { local h; h=$(printf '%s' "$1" | shasum -a 256 | cut -c1-64); mkdir -p "$O/blobi/${h:0:2}"; printf '%s' "$1" > "$O/blobi/${h:0:2}/${h:2}"; echo "$h"; }
pone_actionem () { mkdir -p "$O/actiones/${1:0:2}"; printf '%s\n' "$2" > "$O/actiones/${1:0:2}/${1:2}"; }
b1=$(pone_blobum "unus"); b2=$(pone_blobum "duo")
k1=$(printf 'k1' | shasum -a 256 | cut -c1-64); k2=$(printf 'k2' | shasum -a 256 | cut -c1-64)
pone_actionem "$k1" "$b1"; pone_actionem "$k2" "$b2"
mkdir -p "$O/generationes"
printf '%s\n' "$k1" > "$O/generationes/20260101T000000-1-0001.lst"
for g in 2 3 4 5 6; do printf '%s\n' "$k2" > "$O/generationes/2026010${g}T000000-1-0001.lst"; done
(cd "$T/r" && "$F" purgare) > "$T/o" 2>&1; rc1=$?
printf 'X' >> "$O/blobi/${b2:0:2}/${b2:2}"
(cd "$T/r" && "$F" purgare -verificare) > "$T/o2" 2>&1; rc2=$?
if [ "$rc1" -eq 0 ] && grep -q 'purgare: 3 deleta' "$T/o" \
   && [ ! -e "$O/blobi/${b1:0:2}/${b1:2}" ] && [ ! -e "$O/actiones/${k1:0:2}/${k1:2}" ] \
   && [ ! -e "$O/generationes/20260101T000000-1-0001.lst" ] && [ -e "$O/actiones/${k2:0:2}/${k2:2}" ] \
   && [ "$rc2" -eq 0 ] && grep -q 'purgare: 1 deleta' "$T/o2" && [ ! -e "$O/blobi/${b2:0:2}/${b2:2}" ]; then echo "  XXVI purgare: generationes V servatae OK"; else echo "  XXVI FRACTUM (rc=$rc1 $rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

# XXVII (plan 2 T4): familia - templum STML, instantia per plagulam
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
mkdir -p "$T/r/t" "$T/r/o"
printf 'a\n' > "$T/r/t/probatio_a.c"; printf 'b\n' > "$T/r/t/probatio_b.c"; : > "$T/r/t/alia.c"
printf 'a\n' > "$T/r/o/probatio_a"; printf 'b\n' > "$T/r/o/probatio_b"
printf 'D="${FABRICA_SCRIPTURA:-.}"; mkdir -p "$D/o"; cp "$1" "$D/o/$(basename "$1" .c)"\n' > "$T/r/gen.sh"
printf '<aedificatio>\n  <familia titulus="pf" via="t" praefixum="probatio_" suffixum=".c">\n    <#@instantia basis="@basis" fons="@fons">\n      <actio genus="generator">\n        <mandatum>\n          <verbum! (>sh\n          <verbum! (>gen.sh\n          <verbum! (>&@fons;\n        </mandatum>\n        <ingressus genus="fasciculus" via="gen.sh"/>\n        <exitus via="o/&@basis;" provenientia="regeneratio"/>\n      </actio>\n    </#>\n  </familia>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare -plenus -omnia) > "$T/o" 2>&1; rc1=$?
printf 'c\n' > "$T/r/t/probatio_c.c"
(cd "$T/r" && "$F" iudicare -plenus -omnia) > "$T/o2" 2>&1; rc2=$?
if [ "$rc1" -eq 0 ] && grep -q '^RECENS o/probatio_a ' "$T/o" && grep -q '^RECENS o/probatio_b ' "$T/o" && ! grep -q 'alia' "$T/o" \
   && [ "$rc2" -eq 1 ] && grep -q '^STALUM o/probatio_c ' "$T/o2"; then echo "  XXVII familia: instantiae per plagulam OK"; else echo "  XXVII FRACTUM (rc=$rc1 $rc2)"; cat "$T/o" "$T/o2"; fracta=1; fi

# XXVIII-XXX (plan 2 T6): sanare simul. gen.sh dormit SOLUM in loco
# (sub iudice FABRICA_SCRIPTURA ponitur): tempus = schedula, non iudicium
nunc () { perl -MTime::HiRes=time -e 'printf "%.3f\n", time'; }
simul_radix () {   # $1: actiones "titulus:tuta" ..., $2: dependens extra
    radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
    mkdir -p "$T/r/o"
    cat > "$T/r/gen.sh" <<'GEN'
n="$1"; D="${FABRICA_SCRIPTURA:-.}"; mkdir -p "$D/o"
if [ -z "${FABRICA_SCRIPTURA:-}" ]; then
    mkdir -p "v/$n"; perl -MTime::HiRes=time -e 'printf "%.3f\n", time' > "v/$n/initium"
    sleep "${MORA:-2}"
    perl -MTime::HiRes=time -e 'printf "%.3f\n", time' > "v/$n/finis"
    [ "$n" = "${FRANGE:-}" ] && exit 3
fi
printf '%s\n' "$n" > "$D/o/$n"
GEN
    {
        printf '<aedificatio>\n'
        for par in $1; do
            n="${par%%:*}"; tuta="${par##*:}"
            printf 'vetus\n' > "$T/r/o/$n"
            if [ "$tuta" = "t" ]; then l=' lectiones="verum"'; else l=''; fi
            printf '  <actio titulus="%s" genus="generator"%s>\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen.sh\n      <verbum! (>%s\n    </mandatum>\n    <vestigium via="v/%s"/>\n    <ingressus genus="fasciculus" via="gen.sh"/>\n    <exitus via="o/%s" provenientia="regeneratio"/>\n  </actio>\n' "$n" "$l" "$n" "$n" "$n"
        done
        printf '%b' "${2:-}"
        printf '</aedificatio>\n'
    } > "$T/r/aedificatio.stml"
}
simul_radix "a:t b:t c:t d:t"
t0=$(nunc); (cd "$T/r" && FABRICA_FILA=4 "$F" sanare) > "$T/o" 2>&1; rc=$?; t1=$(nunc)
dur=$(echo "$t1 - $t0" | bc)
if [ "$rc" -eq 0 ] && [ "$(grep -c '^SANATUM' "$T/o")" -eq 4 ] && [ "$(echo "$dur < 4" | bc)" -eq 1 ]; then echo "  XXVIII simul: IV x II s in $dur s     OK"; else echo "  XXVIII FRACTUM (rc=$rc, $dur s)"; cat "$T/o"; fracta=1; fi

simul_radix "a:t b:t c:t d:t" '  <actio titulus="e" genus="generator" lectiones="verum">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen.sh\n      <verbum! (>e\n    </mandatum>\n    <vestigium via="v/e"/>\n    <ingressus genus="fasciculus" via="o/b"/>\n    <exitus via="o/e" provenientia="regeneratio"/>\n  </actio>\n'
printf 'vetus\n' > "$T/r/o/e"
(cd "$T/r" && FABRICA_FILA=4 FRANGE=b MORA=0 "$F" sanare) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^FRACTUM *b ' "$T/o" && [ "$(grep -cE '^SANATUM *(a|c|d) ' "$T/o")" -eq 3 ] \
   && grep -qE '^OMISSUM *e .*dependentia fracta: b' "$T/o"; then echo "  XXIX simul fractura: ceterae sanatae, dependens omissum OK"; else echo "  XXIX FRACTUM (rc=$rc)"; cat "$T/o"; fracta=1; fi

simul_radix "a:t b:t u:n c:t"
(cd "$T/r" && FABRICA_FILA=4 MORA=1 "$F" sanare) > "$T/o" 2>&1; rc=$?
tangit=0
ui=$(cat "$T/r/v/u/initium" 2>/dev/null || echo 0); uf=$(cat "$T/r/v/u/finis" 2>/dev/null || echo 0)
for n in a b c; do
    i=$(cat "$T/r/v/$n/initium" 2>/dev/null || echo 0); f=$(cat "$T/r/v/$n/finis" 2>/dev/null || echo 0)
    [ "$(echo "$i < $uf && $ui < $f" | bc)" -eq 1 ] && tangit=1
done
if [ "$rc" -eq 0 ] && [ "$(grep -c '^SANATUM' "$T/o")" -eq 4 ] && [ "$tangit" -eq 0 ] && [ "$ui" != 0 ]; then echo "  XXX  non tuta numquam simul           OK"; else echo "  XXX  FRACTUM (rc=$rc, tangit=$tangit)"; cat "$T/o"; fracta=1; fi

# XXXI (T6b): iudicium -plenus simul - gen.sh dormit SUB IUDICE solo
simul_radix "a:t b:t c:t d:t"
cat > "$T/r/gen.sh" <<'GEN'
n="$1"; D="${FABRICA_SCRIPTURA:-.}"; mkdir -p "$D/o"
[ -n "${FABRICA_SCRIPTURA:-}" ] && sleep 2
printf '%s\n' "$n" > "$D/o/$n"
GEN
printf 'a\n' > "$T/r/o/a"; printf 'b\n' > "$T/r/o/b"
t0=$(nunc); (cd "$T/r" && FABRICA_FILA=4 "$F" iudicare -plenus -omnia) > "$T/o" 2>&1; rc=$?; t1=$(nunc)
dur=$(echo "$t1 - $t0" | bc)
if [ "$rc" -eq 1 ] && grep -q '^RECENS o/a ' "$T/o" && grep -q '^RECENS o/b ' "$T/o" \
   && grep -q '^STALUM o/c ' "$T/o" && grep -q '^STALUM o/d ' "$T/o" && [ "$(echo "$dur < 4" | bc)" -eq 1 ]; then echo "  XXXI iudicium simul: IV x II s in $dur s OK"; else echo "  XXXI FRACTUM (rc=$rc, $dur s)"; cat "$T/o"; fracta=1; fi

# XXXII (parcum ...9XNXY, 2026-10-05): DECLARATIONES CONTRA CANONEM.
# aedificatio.canon post fabrica slice 3 stalus erat (VI vitia in
# toml/aedificatio.stml: iudicium, fontationes, instrumentum_domus,
# identitas_clang, verdictum) et nulla porta id videbat - inventarium
# hanc portam 'tegit viae aedificatio.canon' nominabat, sed iudicium
# nullum currebat. Nunc: omnis aedificatio.stml quam fabrica.stml
# nominat per bin/canon_examen (registrum: radix <aedificatio>).
[ -x "$RADIX/bin/canon_examen" ] || "$RADIX/tools/canon_struere.sh" >/dev/null 2>&1
vitia_canonis=0
for sub in $(sed -n 's/.*<subsystema via="\([^"]*\)".*/\1/p' "$RADIX/fabrica.stml"); do
    d="$RADIX/$sub/aedificatio.stml"
    [ -f "$d" ] || continue
    if ! (cd "$RADIX" && bin/canon_examen "$sub/aedificatio.stml") > "$T/canon.o" 2>&1; then
        vitia_canonis=1; sed 's/^/      /' "$T/canon.o" | head -8
    fi
done
# et canon ipse contra canon.canon (familia via genus="via" sex
# hebdomades latuit - eadem classis)
if ! (cd "$RADIX" && bin/canon_examen aedificatio.canon) > "$T/canon.o" 2>&1; then
    vitia_canonis=1; sed 's/^/      /' "$T/canon.o" | head -4
fi
if [ -x "$RADIX/bin/canon_examen" ] && [ "$vitia_canonis" -eq 0 ]; then echo "  XXXII declarationes contra aedificatio.canon OK"; else echo "  XXXII FRACTUM (declarationes contra canonem)"; fracta=1; fi

# XXXIII (fabrica-6 T1): CENSUS CHASSIS - 'bin/fabrica census' genera
# registrata cum proprietatibus (TSV) enumerat; numerus = registrum
# (XIII post T2: repositorium), nullum genus sine linea; par res/clavis
(cd "$RADIX" && bin/fabrica census) > "$T/census.o" 2>&1; rc=$?
n_gen=$(grep -c '^genus	' "$T/census.o")
n_summa=$(sed -n 's/^census: genera \([0-9]*\).*/\1/p' "$T/census.o")
if [ "$rc" -eq 0 ] && [ -n "$n_summa" ] && [ "$n_gen" -eq "$n_summa" ] && [ "$n_gen" -ge 13 ] && grep -q '^genus	fasciculus	par:plagula/contentum	' "$T/census.o" && grep -q '^genus	repositorium	par:repositorium/commissum	' "$T/census.o"; then echo "  XXXIII census chassis ($n_gen genera)      OK"; else echo "  XXXIII FRACTUM (census: rc=$rc, genera $n_gen, summa $n_summa)"; head -5 "$T/census.o" | sed 's/^/      /'; fracta=1; fi

# XXXIV (fabrica-6 T2): REPOSITORIUM VERUM - ingressus res="repositorium"
# clavis="commissum" per lib/git (sutura vera): generator memorabilis
# sanatus -> iudicium per memoriam RECENS; commissio nova (HEAD alius)
# -> clavis mutata, memoria non congruit (non RECENS)
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
: > "$T/r/a"
printf '<aedificatio>\n  <actio titulus="g" genus="generator" memorabilis="verum">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_b.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <ingressus res="repositorium" clavis="commissum" via="."/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "b\\n" > "$D/b"\n' > "$T/r/gen_b.sh"
G="git -C $T/r -c user.name=fumus -c user.email=fumus@fumus"
$G init -q && $G add -A && $G commit -q -m primum
(cd "$T/r" && "$F" sanare) > "$T/o" 2>&1
(cd "$T/r" && "$F" iudicare b) > "$T/o2" 2>&1
$G commit -q --allow-empty -m alterum
(cd "$T/r" && "$F" iudicare b) > "$T/o3" 2>&1
if grep -q '^SANATUM *g' "$T/o" && grep -q '^fabrica: 1 recentia' "$T/o2" && grep -q '^NON IUDICATUM b' "$T/o3"; then echo "  XXXIV repositorium: HEAD novus clavem mutat OK"; else echo "  XXXIV FRACTUM (repositorium)"; cat "$T/o" "$T/o2" "$T/o3" | sed 's/^/      /' | head -12; fracta=1; fi

# XXXV-XXXVI (fabrica-6 T3): STADIUM IUDICUM - actio iudex="verum" stala:
# iudicium ceterorum recusatur nominans iudicem; sanare iudicem PRIMUM sanat
radix_iudicum () {
    radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
    printf 'a I\n' > "$T/r/a"; printf 'c I\n' > "$T/r/c"
    printf '<aedificatio>\n  <actio titulus="j" genus="generator" iudex="verum">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_j.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="jb" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="g" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_d.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="c"/>\n    <exitus via="d" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
    printf 'D="${FABRICA_SCRIPTURA:-.}"; cat a > "$D/jb"\n' > "$T/r/gen_j.sh"
    printf 'D="${FABRICA_SCRIPTURA:-.}"; cat c > "$D/d"\n' > "$T/r/gen_d.sh"
    cp "$T/r/a" "$T/r/jb"; cp "$T/r/c" "$T/r/d"
}
radix_iudicum
printf 'a II\n' > "$T/r/a"
(cd "$T/r" && "$F" iudicare -plenus d) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 1 ] && grep -q '^STALUM jb ' "$T/o" && grep -q '^NON IUDICATUM d - iudex j non recens - sana j prius' "$T/o"; then echo "  XXXV iudex stalus: ceteri recusati       OK"; else echo "  XXXV FRACTUM (rc=$rc)"; cat "$T/o" | sed 's/^/      /' | head -8; fracta=1; fi
radix_iudicum
printf 'a II\n' > "$T/r/a"
(cd "$T/r" && "$F" sanare d) > "$T/o" 2>&1; rc=$?
if [ "$rc" -eq 0 ] && grep -q '^SANATUM *j ' "$T/o" && [ "$(cat "$T/r/jb")" = "a II" ]; then echo "  XXXVI sanare: iudex stalus PRIMUS sanatus OK"; else echo "  XXXVI FRACTUM (rc=$rc)"; cat "$T/o" | sed 's/^/      /' | head -8; fracta=1; fi

# XXXVII-XXXVIII (fabrica-6 T6c): PROBATIONES_C VERE - radix temporaria cum
# aedilis.stml vero, bibliotheca (include/bib.h + lib/bib.c), probationes
# t/probatio_a.c (bibliotheca), t/probatio_b.c (sola), t/probatio_c.c
# (facultas fenestra): sanare -> III SANATUM (c nexu solo), iudicare ->
# RECENS; lib/bib.c mutatus -> a solum iterum; b fractus -> FRACTUM, area
# membri deleti -> ORPHANUM
radix_c () {
    radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
    cp "$RADIX/aedilis.stml" "$T/r/aedilis.stml"
    mkdir -p "$T/r/include" "$T/r/lib" "$T/r/t"
    printf '#ifndef BIB_H\n#define BIB_H\nint bib(void);\n#endif\n' > "$T/r/include/bib.h"
    printf '#include "bib.h"\nint bib(void) { return 3; }\n' > "$T/r/lib/bib.c"
    printf '#include "bib.h"\nint main(void) { return bib() == 3 ? 0 : 1; }\n' > "$T/r/t/probatio_a.c"
    # b: ambitus EXACTUS - variabilis fabricae (FUMUS_ALIENUM) invisibilis
    printf '#include <stdlib.h>\nint main(void) { return getenv("FUMUS_ALIENUM") == 0 ? 0 : 5; }\n' > "$T/r/t/probatio_b.c"
    printf '/* <aedilis facultas="fenestra"/> */\nint main(void) { return 1; }\n' > "$T/r/t/probatio_c.c"
    printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
}
VA="build/fabrica/area/probationes_t/probatio_a/verdictum.txt"
VB="build/fabrica/area/probationes_t/probatio_b/verdictum.txt"
VC="build/fabrica/area/probationes_t/probatio_c/verdictum.txt"
radix_c
(cd "$T/r" && FUMUS_ALIENUM=1 "$F" sanare "$VA" "$VB" "$VC") > "$T/o" 2>&1; rc1=$?
(cd "$T/r" && "$F" iudicare -plenus "$VA" "$VB" "$VC") > "$T/o2" 2>&1; rc2=$?
printf '#include "bib.h"\n/* mutatus */\nint bib(void) { return 3; }\n' > "$T/r/lib/bib.c"
(cd "$T/r" && "$F" sanare "$VA" "$VB" "$VC") > "$T/o3" 2>&1; rc3=$?
if [ "$rc1" -eq 0 ] && [ "$(grep -c '^SANATUM' "$T/o")" -eq 3 ] \
   && grep -q 'transiit (nexus solum: facultas fenestra)' "$T/r/$VC" \
   && [ "$rc2" -eq 0 ] && [ "$rc3" -eq 0 ] \
   && grep -q '^SANATUM *probationes_t/probatio_a' "$T/o3" && ! grep -q 'probatio_b\|probatio_c' "$T/o3"; then echo "  XXXVII probationes_c: nexus, cursus, reusus OK"; else echo "  XXXVII FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/o" "$T/o2" "$T/o3" | sed 's/^/      /' | head -20; fracta=1; fi
printf 'int main(void) { return 4; }\n' > "$T/r/t/probatio_b.c"
(cd "$T/r" && "$F" sanare "$VB") > "$T/o" 2>&1; rc1=$?
mkdir -p "$T/r/build/fabrica/area/probationes_t/probatio_deletum"
# verritio sine argumentis iudicia omittit: actio ordinaria una, ne
# 'nihil iudicatum' ante orphana exeat
: > "$T/r/a"; : > "$T/r/b"
printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n  </actio>\n  <actio titulus="g" genus="generator">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" iudicare) > "$T/o2" 2>&1
if [ "$rc1" -eq 1 ] && grep -q '^FRACTUM *probationes_t/probatio_b .*exitus 4' "$T/o" \
   && grep -q '^ORPHANUM: build/fabrica/area/probationes_t/probatio_deletum/' "$T/o2"; then echo "  XXXVIII probationes_c: fractum, orphanum OK"; else echo "  XXXVIII FRACTUM (rc=$rc1)"; cat "$T/o" "$T/o2" | sed 's/^/      /' | head -12; fracta=1; fi

# XXXIX (fabrica-6 T7): POST ET COMPOSITUM VERE - generator g scribit
# build/corpus.lst; probatio a eum legit (exitus 3 si abest); actio gradus
# <post actio="g"/>: 'sanare probationes_t' (titulus compositi) g PRIMUM
# sanat; iudicare -> VERDICTUM 2/2; b mutatus -> 1/2 nominans b
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
cp "$RADIX/aedilis.stml" "$T/r/aedilis.stml"
mkdir -p "$T/r/t"
printf 'fons I\n' > "$T/r/src.txt"
printf 'D="${FABRICA_SCRIPTURA:-.}"; mkdir -p "$D/build"; cp src.txt "$D/build/corpus.lst"\n' > "$T/r/gen_g.sh"
printf '#include <stdio.h>\nint main(void) { FILE* f = fopen("build/corpus.lst", "r"); if (f == 0) { return 3; } fclose(f); return 0; }\n' > "$T/r/t/probatio_a.c"
printf 'int main(void) { return 0; }\n' > "$T/r/t/probatio_b.c"
printf '<aedificatio>\n  <actio titulus="g" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_g.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="src.txt"/>\n    <exitus via="build/corpus.lst" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n    <post actio="g"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" sanare probationes_t) > "$T/o" 2>&1; rc1=$?
(cd "$T/r" && "$F" iudicare -plenus probationes_t) > "$T/o2" 2>&1; rc2=$?
printf 'int main(void) { return 0; } /* II */\n' > "$T/r/t/probatio_b.c"
(cd "$T/r" && "$F" iudicare -plenus probationes_t) > "$T/o3" 2>&1; rc3=$?
lg=$(grep -n '^SANATUM *g ' "$T/o" | cut -d: -f1); la=$(grep -n '^SANATUM *probationes_t/probatio_a' "$T/o" | cut -d: -f1)
# sanare compositi nominati VERDICTUM ipse dat (T10: porta per gradum)
if [ "$rc1" -eq 0 ] && [ -n "$lg" ] && [ -n "$la" ] && [ "$lg" -lt "$la" ] \
   && grep -q '^VERDICTUM probationes_t: 2/2$' "$T/o" \
   && [ "$rc2" -eq 0 ] && grep -q '^VERDICTUM probationes_t: 2/2$' "$T/o2" \
   && [ "$rc3" -eq 1 ] && grep -q '^VERDICTUM probationes_t: 1/2 - non recentia: probationes_t/probatio_b$' "$T/o3"; then echo "  XXXIX post et compositum: ordo, VERDICTUM N/M OK"; else echo "  XXXIX FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/o" "$T/o2" "$T/o3" | sed 's/^/      /' | head -24; fracta=1; fi

# XL (fabrica-6 H1): LINEAE MACHINAE - contractus consumptorum (silva.py,
# generata, oraculum toml): arbor XXXIX, b mutatus. iudicare -machina:
# IUDICIUM per membrum, VERDICTUM <c> 1 2 <b>, SUMMA; sanare -machina:
# SANATIO b, VERDICTUM <c> 2 2; OMNIS linea genus notum fert (nulla forma
# humana in modo machinae)
(cd "$T/r" && "$F" iudicare -plenus -machina probationes_t) > "$T/m1" 2>/dev/null; rc1=$?
(cd "$T/r" && "$F" sanare -machina probationes_t) > "$T/m2" 2>/dev/null; rc2=$?
ignotae=$(cat "$T/m1" "$T/m2" | awk -F'\t' '$1 !~ /^(IUDICIUM|COMPOSITUM|VERDICTUM|SANANDA|ORPHANUM|BINARIA|PRAECONDICIO|SANATIO|AGITUR|UNDA|NOTA|SUMMA)$/ || NF < 2' | wc -l | tr -d ' ')
if [ "$rc1" -eq 1 ] && grep -q "^IUDICIUM	RECENS	build/fabrica/area/probationes_t/probatio_a/verdictum.txt	" "$T/m1" \
   && grep -qE "^IUDICIUM	(STALUM|IGNOTUM)	build/fabrica/area/probationes_t/probatio_b/verdictum.txt	" "$T/m1" \
   && grep -q "^VERDICTUM	probationes_t	1	2	probationes_t/probatio_b$" "$T/m1" \
   && grep -q "^SUMMA	" "$T/m1" \
   && [ "$rc2" -eq 0 ] && grep -q "^SANATIO	SANATUM	probationes_t/probatio_b	" "$T/m2" \
   && grep -q "^VERDICTUM	probationes_t	2	2	$" "$T/m2" && [ "$ignotae" -eq 0 ]; then echo "  XL   lineae machinae (iudicare, sanare)  OK"; else echo "  XL   FRACTUM (rc=$rc1 $rc2, lineae ignotae $ignotae)"; cat "$T/m1" "$T/m2" | sed 's/^/      /' | head -20; fracta=1; fi

if [ "$fracta" -ne 0 ]; then echo "fumus fabricae: FRACTUM"; exit 1; fi
echo "fumus fabricae: sanum (XL/XL)"
exit 0
