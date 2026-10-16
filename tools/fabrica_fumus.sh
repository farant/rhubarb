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
#   XLI  familiae installatorum: actio cum II+ manifestis -> installator
#        '-familia' cum scopis OMNIBUS (arbor frigida, quaestio …J3108ZW6)
#   XLII instrumentum (fabrica-7 T2): <aedilis instrumentum="bin/X"/> ->
#        linea ingressus binarii in clave membri, commissum non
#   XLIII debita scripturae (fabrica-7 T3): membrum debitum suum extra
#        aream scribit; alienum ibi recusatur; census scriptum/stalum
#   XLIV portae (portae-declaratae T3): usus, registrum absens/malum ->
#        2; clavis sine annotatione -> RECUSATIO nominata, 2; debita in
#        repositorio git: caput -> clausura vera fontis quem cursor legit,
#        MANU, INTECTUM
#   XLV  clavis thesauri extractoris (circulus commissionis P1): bin/aedilis
#        et bin/fabrica (binaria DIVERSA) clavem EANDEM ex digesto infixo
#        habent - editio extra extractorem thesaurum non refrigerat
#   XLVI probare (cursores ad gradus T1): membra cursa acta sua
#        imprimunt, fresca 'servata' cum causa iudicis (linea
#        'servatae N/M'), summa, exitus 1 si fracta; FILTRUM, -vis,
#        filtrum sine paribus -> 2, compositum ignotum -> 2, -machina;
#        filtrum ut via ('t/probatio_a.c' -> probatio_a; cursores veteres)
#   XLVII indices memoriae (cursores ad gradus T3b): quaestiones per
#        titulum (lectiones, particulae, cursus) INDICEM utuntur - sine eo
#        quaeque tabulam totam legit (ordo per locos ~2.5 s calidus)
#   XLVIII gradus solus (T3b): probare compositum unum explicat - alienum
#        putridum (debitum membri ignoti) probare alterius non obstat;
#        ipsum nominatum et sanare plenum recusant (2, causa nominata)
#   XLIX identitas compilatoris semel per processum (T3b): 'clang
#        -print-prog-name' olim omni compilatione et actione (XIII in
#        mundo III membrorum; ~3 s in -vis css); nunc II constantes
#   L    photographia thesaurum praeterit (T3c): scriptura ibi semper
#        licita (fabrica_ordo), ergo ambulatio eius nihil iudicat -
#        XL milia plagularum in thesauro 'probare -vis' (X membra, XX
#        photographiae) < 1.0 s PROCESSORIS tardius (planta ~2.15 s;
#        murale impressum, non iudicatum - sub onere fallebat)
#   LI   ambitus declaratus in clave (T4): membrum frescum cum valore
#        variabilis declaratae MUTATO currit (COMPUTUS_SCRIBERE=1 olim
#        servabatur - aurum non rescribebatur); valore eodem servatur
#   LII  actiones in annalibus (analytica T3): cursus per membrum actum
#        in diario mensurae, parens et contextus ex ambitu hereditati,
#        exitus ex eventu (FRACTUM = 1), nota = eventus
#   LIII sectiones in annalibus (analytica T3b): ex plagula credo membri
#        cursus filius per sectionem (<membrum>#<sectio>), synthetica
#        nihil
#   LIV  radix aliena (analytica T4): cursus radicem veram fert, contextus
#        alterius arboris non hereditatur
#   LV   custodia diarii (analytica T4): radix non repositorium in diarium
#        ordinarium nihil scribit; repositorium scribit
#   LVI  corpus actionis (analytica T4): acta membri in diurna sub id
#        cursus, campus acta = sigillum
#   LVII sectio rupta (analytica T4 2b-ii): INCIPIT sine SECTIO -> causa
#        "sectio 'X' RUIT; N non cursae", cursus filius #X fractus
#   LVIII catena iudicii (parcum 190VQB): g (a->b), h (b->c); a mutato
#        'iudicare -plenus c' NON RECENS sed FORTASSE 'supra: g' (olim
#        RECENS - h contra b stalum iudicabatur); arcus DATORUM solum
#        (k cum praecondicione g, sine ingressu eius = RECENS); post
#        sanare c RECENS
#
# Exitus: 0 sanum · 1 fractum · 2 bin/fabrica deest.
# <porta titulus="fabrica-fumus" gradus="fusio" instrumentum="bin/fabrica" tegit="*/aedificatio.stml aedificatio.canon fabrica.canon tools/fabrica_struere.sh"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
F="$RADIX/bin/fabrica"
[ -x "$F" ] || { echo "fumus fabricae: bin/fabrica deest (./tools/fabrica_struere.sh)"; exit 2; }
T="$(mktemp -d)"
ORPH="build/aedilis/fumus_orphanum_fabricae"
trap 'rm -rf "$T" "$ORPH"' EXIT
# annales cursuum (analytica T3): actiones fictae numquam in diario vero
export MENSOR_DIARIUM="$T/cursus.diarium" MENSOR_DIURNA="$T/diurna"
unset MENSOR_PARENS MENSOR_COMMISSUM MENSOR_SESSIO CURSUS_NULLUS
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
# humana in modo machinae); SECTIO post IUDICIUM membri (fabrica-7 T1:
# suita classica -> 'totum' syntheticum, 0 0 sine compendio credo)
(cd "$T/r" && "$F" iudicare -plenus -machina probationes_t) > "$T/m1" 2>/dev/null; rc1=$?
(cd "$T/r" && "$F" sanare -machina probationes_t) > "$T/m2" 2>/dev/null; rc2=$?
ignotae=$(cat "$T/m1" "$T/m2" | awk -F'\t' '$1 !~ /^(IUDICIUM|SECTIO|COMPOSITUM|VERDICTUM|SANANDA|ORPHANUM|BINARIA|PRAECONDICIO|SANATIO|AGITUR|UNDA|NOTA|SUMMA)$/ || NF < 2' | wc -l | tr -d ' ')
if [ "$rc1" -eq 1 ] && grep -q "^IUDICIUM	RECENS	build/fabrica/area/probationes_t/probatio_a/verdictum.txt	" "$T/m1" \
   && grep -qE "^IUDICIUM	(STALUM|IGNOTUM)	build/fabrica/area/probationes_t/probatio_b/verdictum.txt	" "$T/m1" \
   && grep -A1 "^IUDICIUM	RECENS	build/fabrica/area/probationes_t/probatio_a/verdictum.txt	" "$T/m1" | grep -q "^SECTIO	probationes_t/probatio_a	totum	TRANSIIT	0	0	$" \
   && grep -q "^VERDICTUM	probationes_t	1	2	probationes_t/probatio_b$" "$T/m1" \
   && grep -q "^SUMMA	" "$T/m1" \
   && [ "$rc2" -eq 0 ] && grep -q "^SANATIO	SANATUM	probationes_t/probatio_b	" "$T/m2" \
   && grep -q "^VERDICTUM	probationes_t	2	2	$" "$T/m2" && [ "$ignotae" -eq 0 ]; then echo "  XL   lineae machinae (iudicare, sanare)  OK"; else echo "  XL   FRACTUM (rc=$rc1 $rc2, lineae ignotae $ignotae)"; cat "$T/m1" "$T/m2" | sed 's/^/      /' | head -20; fracta=1; fi

# XLI (quaestio …J3108ZW6): FAMILIAE INSTALLATORUM - actio cum II+
# ingressibus 'manifestum' (familia binariorum: canon, natura) digestum
# OMNIUM manifestorum poscit, sed binarium primum ante ceteros struitur;
# installator ergo '-familia' cum scopis OMNIBUS ante binarium primum
# vocat (arbor frigida: scopus omissus = digestum fractum, nullum
# binarium). Lectio sola arboris vivae: scopi '-familia' (tools/X.c) ==
# manifesta declarata (build/aedilis/X/manifestum.stml).
xli=""; xli_n=0
while IFS='|' read -r act scr man; do
    xli_n=$((xli_n + 1))
    fam=$(awk '/provenientia_obiectum.sh" -familia/{f=1} f{print} f && !/\\$/{exit}' "$RADIX/${scr#./}" 2>/dev/null \
        | grep -o 'tools/[a-z_]*\.c' | sed 's|^tools/||; s|\.c$||' | sort | tr '\n' ' ')
    dec=$(printf '%s\n' $man | sort | tr '\n' ' ')
    [ "$fam" = "$dec" ] || xli="$xli $act (familia: ${fam:-nulla}/ declarata: $dec)"
done < <(awk '/<actio titulus=/{t=$0; sub(/.*titulus="/,"",t); sub(/".*/,"",t); n=0; m=""; c=""}
              /genus="manifestum"/{n++; v=$0; sub(/.*build\/aedilis\//,"",v); sub(/\/manifestum.stml.*/,"",v); m=m" "v}
              /verbum! \(>.*_struere.sh/{c=$0; sub(/.*\(>/,"",c); sub(/[ \t]*$/,"",c)}
              /<\/actio>/{ if (n >= 2 && c != "") print t "|" c "|" m }' "$RADIX/aedificatio.stml")
# et MODUS ipse: manifestum familiae absens (arbor frigida) per '-familia'
# renascitur (artificium build/ regenerabile; interruptio = struere
# proximum id reficit)
MX="$RADIX/build/aedilis/natura_quaesitor/manifestum.stml"
if [ -x "$RADIX/bin/aedilis" ] && [ -f "$MX" ]; then
    mv "$MX" "$T/manifestum_xli.stml"
    "$RADIX/tools/provenientia_obiectum.sh" -familia tools/natura_quaesitor.c >/dev/null 2>&1; rc=$?
    [ "$rc" -eq 0 ] && [ -f "$MX" ] || { xli="$xli -familia manifestum non renovavit (rc=$rc)"; [ -f "$MX" ] || mv "$T/manifestum_xli.stml" "$MX"; }
fi
if [ "$xli_n" -ge 2 ] && [ -z "$xli" ]; then echo "  XLI  familiae installatorum ($xli_n): -familia == manifesta declarata, manifestum absens renascitur OK"; else echo "  XLI  FRACTUM (familiae $xli_n):$xli"; fracta=1; fi

# XLII (fabrica-7 T2): INSTRUMENTUM VERE - b declarat <aedilis
# instrumentum="bin/instr"/> (aedilis verus); bin/instr = scriptum cuius
# '-provenientia' lineam ingressus dat. Linea ingressus mutata -> b solum
# iterum; linea commissum sola mutata -> nihil (provenientia, ut
# instrumentum_domus actionum)
radix_c
mkdir -p "$T/r/bin"
printf '#!/bin/sh\necho "ingressus I"\necho "commissum I"\n' > "$T/r/bin/instr"; chmod +x "$T/r/bin/instr"
printf '/* <aedilis instrumentum="bin/instr"/> */\nint main(void) { return 0; }\n' > "$T/r/t/probatio_b.c"
(cd "$T/r" && "$F" sanare "$VA" "$VB") > "$T/o" 2>&1; rc1=$?
printf '#!/bin/sh\necho "ingressus I"\necho "commissum II"\n' > "$T/r/bin/instr"
(cd "$T/r" && "$F" sanare "$VA" "$VB") > "$T/o2" 2>&1; rc2=$?
printf '#!/bin/sh\necho "ingressus II"\necho "commissum II"\n' > "$T/r/bin/instr"
(cd "$T/r" && "$F" sanare "$VA" "$VB") > "$T/o3" 2>&1; rc3=$?
if [ "$rc1" -eq 0 ] && [ "$(grep -c '^SANATUM' "$T/o")" -eq 2 ] \
   && [ "$rc2" -eq 0 ] && ! grep -q '^SANATUM' "$T/o2" \
   && [ "$rc3" -eq 0 ] && grep -q '^SANATUM *probationes_t/probatio_b' "$T/o3" && ! grep -q 'probatio_a' "$T/o3"; then echo "  XLII instrumentum: ingressus binarii in clave, commissum non OK"; else echo "  XLII FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/o" "$T/o2" "$T/o3" | sed 's/^/      /' | head -20; fracta=1; fi

# XLIII (fabrica-7 T3): DEBITA SCRIPTURAE VERE - a debitum suum
# build/a.db scribit (photographia vera, extra aream): SANATUM, census
# 'scriptum'; b ibi scribens -> FRACTUM nominatum; a non iam scribens ->
# census 'stalum'
radix_c
SCRIBENS='#include <stdio.h>\n#include "bib.h"\nint main(void) { FILE* f = fopen("build/a.db", "w"); if (f) { fputs("a\\n", f); fclose(f); } return bib() == 3 ? 0 : 1; }\n'
printf "$SCRIBENS" > "$T/r/t/probatio_a.c"
printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n    <debitum_scripturae membrum="probatio_a" via="build/a.db" causa="fumus"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" sanare "$VA" "$VB") > "$T/o" 2>&1; rc1=$?
(cd "$T/r" && "$F" census) > "$T/c1" 2>&1
printf '#include <stdio.h>\nint main(void) { FILE* f = fopen("build/a.db", "w"); if (f) { fputs("b\\n", f); fclose(f); } return 0; }\n' > "$T/r/t/probatio_b.c"
(cd "$T/r" && "$F" sanare "$VB") > "$T/o2" 2>&1; rc2=$?
printf '#include "bib.h"\nint main(void) { return bib() == 3 ? 0 : 1; }\n' > "$T/r/t/probatio_a.c"
(cd "$T/r" && "$F" sanare "$VA") > "$T/o3" 2>&1; rc3=$?
(cd "$T/r" && "$F" census) > "$T/c2" 2>&1
if [ "$rc1" -eq 0 ] && grep -q "^debitum	probationes_t	probatio_a	build/a.db	scriptum	fumus	" "$T/c1" \
   && grep -q '^census: debita 1 (actiones 1, stala 0, ignota 0)$' "$T/c1" \
   && [ "$rc2" -eq 1 ] && grep -q '^FRACTUM *probationes_t/probatio_b .*build/a.db' "$T/o2" \
   && [ "$rc3" -eq 0 ] && grep -q "^debitum	probationes_t	probatio_a	build/a.db	stalum	" "$T/c2" \
   && grep -q '^census: debita 1 (actiones 1, stala 1, ignota 0)$' "$T/c2"; then echo "  XLIII debita scripturae: suum scribit, alienum recusatur, stalum nominatur OK"; else echo "  XLIII FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/o" "$T/c1" "$T/o2" "$T/o3" "$T/c2" | grep -v '^genus' | sed 's/^/      /' | head -24; fracta=1; fi

# XLIV (portae-declaratae T3): bin/fabrica portae - CLI super lib/portae.c
# in repositorio git temporario (clausura per aedilis verum, tabula
# effectus crustae copiata)
radix_c
mkdir -p "$T/r/crusta"
cp "$RADIX/crusta/effectus_mandata.stml" "$T/r/crusta/"
printf '#!/bin/bash\n# <porta titulus="a"/>\ncat lib/bib.c\n' > "$T/r/a.sh"
printf '#!/bin/bash\necho b\n' > "$T/r/b.sh"
printf '#!/bin/bash\n# <porta manu causa="homo videt"/>\necho m\n' > "$T/r/m.sh"
: > "$T/r/z.txt"
(cd "$T/r" && git init -q . && git add -A) > /dev/null 2>&1
printf 'a\ta.sh\n' > "$T/p1.tsv"
printf 'a\ta.sh\nb\tb.sh\n' > "$T/p2.tsv"
printf 'a\n' > "$T/p3.tsv"
(cd "$T/r" && "$F" portae) > "$T/o1" 2>&1; rc1=$?
(cd "$T/r" && "$F" portae declaratae -portae "$T/absens.tsv") > "$T/o2" 2>&1; rc2=$?
(cd "$T/r" && "$F" portae declaratae -machina -portae "$T/p3.tsv") > "$T/o3" 2>&1; rc3=$?
(cd "$T/r" && "$F" portae declaratae -machina -portae "$T/p2.tsv") > "$T/o4" 2>&1; rc4=$?
(cd "$T/r" && "$F" portae debitae -machina -portae "$T/p1.tsv" include/bib.h m.sh z.txt) > "$T/o5" 2>&1; rc5=$?
if [ "$rc1" -eq 2 ] && grep -q '^usus: bin/fabrica portae debitae' "$T/o1" \
   && [ "$rc2" -eq 2 ] && grep -q 'registrum absens' "$T/o2" \
   && [ "$rc3" -eq 2 ] && grep -q 'p3.tsv:1: linea registri mala' "$T/o3" \
   && [ "$rc4" -eq 2 ] && grep -qx "RECUSATIO		0	porta 'b' (cursor b.sh) sine annotatione <porta titulus=\"...\"/>" "$T/o4" \
   && [ "$rc5" -eq 0 ] \
   && grep -qx 'DEBITUM	a	0	include/bib.h in clausura lib/bib.c (lectio (cat) in a.sh)' "$T/o5" \
   && grep -qx 'DEBITUM	m.sh	1	m.sh (cursor ipse)' "$T/o5" \
   && grep -qx 'INTECTUM	z.txt' "$T/o5" && [ "$(wc -l < "$T/o5")" -eq 3 ]; then echo "  XLIV portae: usus, registrum, recusatio, debita per clausuram veram OK"; else echo "  XLIV FRACTUM (rc=$rc1 $rc2 $rc3 $rc4 $rc5)"; cat "$T/o1" "$T/o2" "$T/o3" "$T/o4" "$T/o5" | sed 's/^/      /' | head -24; fracta=1; fi

# XLV (circulus commissionis P1): CLAVIS THESAURI EXTRACTORIS in arbore
# viva - bin/aedilis et bin/fabrica binaria diversa sunt, clavis tamen
# eadem (digestum actionis 'aedilis' infixum, non binarium): thesaurus
# communis, et fabrica nova ex editione extra extractorem eum non
# refrigerat (mensuratum: XLI s -> IV.VIII s)
KA="$("$RADIX/bin/aedilis" --clavis-thesauri 2>&1)"; rca=$?
KF="$("$F" clavis-thesauri 2>&1)"; rcf=$?
if [ "$rca" -eq 0 ] && [ "$rcf" -eq 0 ] && [ "$KA" = "$KF" ] \
   && printf '%s' "$KF" | grep -q '	digestum$' \
   && ! cmp -s "$RADIX/bin/aedilis" "$F"; then echo "  XLV  clavis thesauri: aedilis == fabrica, ex digesto OK"; else echo "  XLV  FRACTUM (aedilis: $KA rc=$rca | fabrica: $KF rc=$rcf)"; fracta=1; fi

# XLVI (cursores ad gradus T1): PROBARE - compositum ut porta et ut homo.
# Radix XXXVII: a (bibliotheca) et c transeunt, b exitu 4 frangitur;
# quaeque probatio lineam suam dicit. Cursus I: omnes currunt (acta
# impressa), 2/3, exitus 1. Cursus II idem: a et c SERVATAE (acta non
# impressa, causa iudicis in linea 'servatae 2/3'), b iterum currit.
# Filtrum 'probatio_a' -> 1/1 servata, exitus 0; -vis -> a iterum currit;
# filtrum sine paribus -> 2; compositum ignotum -> 2; -machina: MEMBRUM
# per membrum et PROBARE summa
radix_c
printf '#include <stdio.h>\n#include "bib.h"\nint main(void) { printf("a dicit salve\\n"); return bib() == 3 ? 0 : 1; }\n' > "$T/r/t/probatio_a.c"
printf '#include <stdio.h>\nint main(void) { printf("b dicit vae\\n"); return 4; }\n' > "$T/r/t/probatio_b.c"
printf '#include <stdio.h>\nint main(void) { printf("c dicit salve\\n"); return 0; }\n' > "$T/r/t/probatio_c.c"
(cd "$T/r" && "$F" probare probationes_t) > "$T/p1" 2>&1; rc1=$?
(cd "$T/r" && "$F" probare probationes_t) > "$T/p2" 2>&1; rc2=$?
(cd "$T/r" && "$F" probare probationes_t probatio_a) > "$T/p3" 2>&1; rc3=$?
(cd "$T/r" && "$F" probare -vis probationes_t probatio_a) > "$T/p4" 2>&1; rc4=$?
(cd "$T/r" && "$F" probare probationes_t zzz) > "$T/p5" 2>&1; rc5=$?
(cd "$T/r" && "$F" probare probationes_nulla) > "$T/p6" 2>&1; rc6=$?
(cd "$T/r" && "$F" probare -machina probationes_t) > "$T/p7" 2>/dev/null; rc7=$?
# filtrum ut via (cursores veteres: 'dir/probatio_a.c' -> 'probatio_a')
(cd "$T/r" && "$F" probare probationes_t t/probatio_a.c) > "$T/p8" 2>&1; rc8=$?
if [ "$rc1" -eq 1 ] && grep -qx 'a dicit salve' "$T/p1" && grep -qx 'b dicit vae' "$T/p1" \
   && grep -q '^--- probatio_a praeteriit' "$T/p1" && grep -q '^--- probatio_b FRACTA' "$T/p1" \
   && grep -qx 'probationes_t: 2/3 praeteritae \[cursae 3, servatae 0\]' "$T/p1" \
   && grep -qx 'FRACTAE: probatio_b' "$T/p1" \
   && [ "$rc2" -eq 1 ] && ! grep -q 'a dicit salve' "$T/p2" && grep -qx 'b dicit vae' "$T/p2" \
   && grep -q '^--- probatio_a servata' "$T/p2" \
   && grep -q '^servatae 2/3: .*lectiones congruunt' "$T/p2" \
   && grep -qx 'probationes_t: 2/3 praeteritae \[cursae 1, servatae 2\]' "$T/p2" \
   && [ "$rc3" -eq 0 ] && grep -qx 'probationes_t: 1/1 praeteritae \[cursae 0, servatae 1\]' "$T/p3" \
   && [ "$rc4" -eq 0 ] && grep -qx 'a dicit salve' "$T/p4" \
   && grep -qx 'probationes_t: 1/1 praeteritae \[cursae 1, servatae 0\]' "$T/p4" \
   && [ "$rc5" -eq 2 ] && grep -q 'NULLA PROBATIO CURSA' "$T/p5" \
   && [ "$rc6" -eq 2 ] && grep -q "compositum ignotum 'probationes_nulla'" "$T/p6" \
   && [ "$rc7" -eq 1 ] && grep -q '^MEMBRUM	probatio_a	servata	' "$T/p7" \
   && grep -q '^MEMBRUM	probatio_b	fracta	' "$T/p7" \
   && grep -qx 'PROBARE	probationes_t	2	3	1	2' "$T/p7" \
   && [ "$rc8" -eq 0 ] && grep -qx 'probationes_t: 1/1 praeteritae \[cursae 0, servatae 1\]' "$T/p8"; then echo "  XLVI probare: acta, servatae cum causa, filtrum, -vis, -machina OK"; else echo "  XLVI FRACTUM (rc=$rc1 $rc2 $rc3 $rc4 $rc5 $rc6 $rc7 $rc8)"; cat "$T/p1" "$T/p2" "$T/p3" "$T/p7" "$T/p8" | sed 's/^/      /' | head -40; fracta=1; fi

# XLVII (cursores ad gradus T3b): INDICES MEMORIAE - radix XLVI (memoria
# a probare creata) et memoria viva (migratio in apertura): consilium
# quaestionis sqlite 'USING ... INDEX' pro quaestionibus per titulum
indices_ok=1; indices_mala=""
for db in "$T/r/build/fabrica.db" "$RADIX/build/fabrica.db"; do
    [ -f "$db" ] || { indices_ok=0; indices_mala="$indices_mala absens:$db"; continue; }
    for q in "SELECT DISTINCT genus, via FROM lectiones WHERE titulus = 'x'" \
             "SELECT genus, via, sigillum FROM lectiones WHERE titulus = 'x'" \
             "SELECT via, sigillum FROM particulae WHERE titulus = 'x'" \
             "SELECT duratio_ms FROM cursus WHERE titulus = 'x'"; do
        if ! sqlite3 "$db" "EXPLAIN QUERY PLAN $q" 2>/dev/null | grep -q 'USING .*INDEX'; then
            indices_ok=0; indices_mala="$indices_mala [${db##*/r/}: ${q%% WHERE*}]"
        fi
    done
done
if [ "$indices_ok" -eq 1 ]; then echo "  XLVII indices memoriae (lectiones, particulae, cursus) OK"; else echo "  XLVII FRACTUM (sine indice:$indices_mala)"; fracta=1; fi

# XLVIII (cursores ad gradus T3b): GRADUS SOLUS - radix XXXVII cum
# composito altero probationes_u cuius debitum membrum ignotum nominat
radix_c
mkdir -p "$T/r/u"
printf 'int main(void) { return 0; }\n' > "$T/r/u/probatio_x.c"
printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n  </actio>\n  <actio titulus="probationes_u" genus="iudicium">\n    <probationes_c exemplar="u/probatio_*.c"/>\n    <debitum_scripturae membrum="probatio_zeta" via="build/z.db" causa="putridum"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && "$F" probare probationes_t) > "$T/g1" 2>&1; rc1=$?
(cd "$T/r" && "$F" probare probationes_u) > "$T/g2" 2>&1; rc2=$?
(cd "$T/r" && "$F" sanare probationes_t) > "$T/g3" 2>&1; rc3=$?
if [ "$rc1" -eq 0 ] && grep -qx 'probationes_t: 3/3 praeteritae \[cursae 3, servatae 0\]' "$T/g1" \
   && [ "$rc2" -eq 2 ] && grep -q "membrum ignotum 'probatio_zeta'" "$T/g2" \
   && [ "$rc3" -eq 2 ] && grep -q "membrum ignotum 'probatio_zeta'" "$T/g3"; then echo "  XLVIII gradus solus: alienum putridum non obstat, ipsum recusat OK"; else echo "  XLVIII FRACTUM (rc=$rc1 $rc2 $rc3)"; cat "$T/g1" "$T/g2" "$T/g3" | sed 's/^/      /' | head -20; fracta=1; fi

# XLIX (cursores ad gradus T3b): IDENTITAS COMPILATORIS SEMEL - radix
# XXXVII, clang fictus primus in PATH (vocationem '-print-prog-name'
# notat, deinde clang verum exsequitur); probare -vis: compilationes et
# actiones plures, identitates DUAE constantes (olim XIII): sutura
# (clavis actionis: metadata binarii) et compilator (contentum per
# thesaurum) - semel quaeque per processum, non per compilationem
radix_c
mkdir -p "$T/via"
CLANG_VERUM="$(command -v clang)"
printf '#!/bin/sh\ncase "$*" in *-print-prog-name*) echo x >> "%s/identitates";; esac\nexec "%s" "$@"\n' "$T" "$CLANG_VERUM" > "$T/via/clang"
chmod +x "$T/via/clang"
: > "$T/identitates"
(cd "$T/r" && PATH="$T/via:$PATH" "$F" probare -vis probationes_t) > "$T/i1" 2>&1; rc1=$?
ni=$(wc -l < "$T/identitates" | tr -d ' ')
if [ "$rc1" -eq 0 ] && [ "$ni" -le 2 ] && grep -qx 'probationes_t: 3/3 praeteritae \[cursae 3, servatae 0\]' "$T/i1"; then echo "  XLIX identitas compilatoris semel per processum OK"; else echo "  XLIX FRACTUM (rc=$rc1, identitates $ni)"; sed 's/^/      /' "$T/i1" | tail -5; fracta=1; fi

# L (cursores ad gradus T3c): PHOTOGRAPHIA SINE THESAURO - radix cum X
# probationibus vacuis; 'probare -vis' sine thesauro, deinde cum XL
# milibus plagularum in build/aedilis/obiecta/actiones (cursus primus
# post creationem calefacit, secundus mensuratur). Differentia tempus
# PROCESSORIS (filii: user+sys) < 1.0 s; murale solum impressum
# (2026-10-15, …GBVETQ: murale sub onere arborum aliarum fallebat -
# exspectatio processoris non est ambulatio). Mensuratum: sanum ~0.02 s,
# planta (thesaurus ambulatus) ~2.15 s processoris, ~2.5 s murale
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
cp "$RADIX/aedilis.stml" "$T/r/aedilis.stml"
mkdir -p "$T/r/t"
for i in 0 1 2 3 4 5 6 7 8 9; do printf 'int main(void) { return 0; }\n' > "$T/r/t/probatio_m$i.c"; done
printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
# tempus probare -vis, MINIMUM duorum: 'processor murale' (processor =
# user+sys filiorum, RUSAGE_CHILDREN)
tempus_vis () {
    python3 - "$T/r" "$F" <<'PY'
import resource, subprocess, sys, time
radix, f = sys.argv[1], sys.argv[2]
mur, proc = [], []
for _ in range(2):
    a = time.time()
    r0 = resource.getrusage(resource.RUSAGE_CHILDREN)
    r = subprocess.run([f, 'probare', '-vis', 'probationes_t'], cwd=radix,
                       capture_output=True)
    r1 = resource.getrusage(resource.RUSAGE_CHILDREN)
    mur.append(time.time() - a)
    proc.append(r1.ru_utime - r0.ru_utime + r1.ru_stime - r0.ru_stime)
    if r.returncode != 0:
        print('rc%d' % r.returncode); sys.exit(0)
print('%.2f %.2f' % (min(proc), min(mur)))
PY
}
(cd "$T/r" && "$F" probare probationes_t) > /dev/null 2>&1
sine=$(tempus_vis)
python3 -c "
import os
d = os.path.join('$T/r', 'build/aedilis/obiecta/actiones')
for i in range(400):
    p = os.path.join(d, '%02x' % (i % 256), '%03d' % i)
    os.makedirs(p, exist_ok=True)
    for j in range(100):
        open(os.path.join(p, 'r%d' % j), 'w').close()
"
(cd "$T/r" && "$F" probare -vis probationes_t) > /dev/null 2>&1
cum=$(tempus_vis)
read -r sine_p sine_m _ <<< "$sine ?"
read -r cum_p cum_m _ <<< "$cum ?"
if python3 -c "import sys; sys.exit(0 if float('$cum_p') - float('$sine_p') < 1.0 else 1)" 2>/dev/null; then echo "  L    photographia thesaurum praeterit (processor sine $sine_p s, cum XL milibus $cum_p s; murale $sine_m / $cum_m s) OK"; else echo "  L    FRACTUM (processor sine $sine_p s, cum XL milibus plagularum in thesauro $cum_p s - limes 1.0 s; murale $sine_m / $cum_m s)"; fracta=1; fi

# LI (cursores ad gradus T4): AMBITUS DECLARATUS IN CLAVE - membrum unum
# quod FUMUS_DECLARATUM imprimit; declaratum per <ambitus variabilis>
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
cp "$RADIX/aedilis.stml" "$T/r/aedilis.stml"
mkdir -p "$T/r/t"
printf '#include <stdio.h>\n#include <stdlib.h>\nint main(void) { const char* v = getenv("FUMUS_DECLARATUM"); printf("valor: %%s\\n", v ? v : "absens"); return 0; }\n' > "$T/r/t/probatio_v.c"
printf '<aedificatio>\n  <actio titulus="probationes_t" genus="iudicium">\n    <probationes_c exemplar="t/probatio_*.c"/>\n    <ambitus variabilis="FUMUS_DECLARATUM"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
(cd "$T/r" && env -u FUMUS_DECLARATUM "$F" probare probationes_t) > "$T/a1" 2>&1; rc1=$?
(cd "$T/r" && FUMUS_DECLARATUM=1 "$F" probare probationes_t) > "$T/a2" 2>&1; rc2=$?
(cd "$T/r" && FUMUS_DECLARATUM=1 "$F" probare probationes_t) > "$T/a3" 2>&1; rc3=$?
(cd "$T/r" && env -u FUMUS_DECLARATUM "$F" probare probationes_t) > "$T/a4" 2>&1; rc4=$?
if [ "$rc1$rc2$rc3$rc4" = "0000" ] && grep -qx 'valor: absens' "$T/a1" \
   && grep -qx 'valor: 1' "$T/a2" && grep -q 'cursae 1, servatae 0' "$T/a2" \
   && grep -q 'cursae 0, servatae 1' "$T/a3" \
   && grep -qx 'valor: absens' "$T/a4" && grep -q 'cursae 1, servatae 0' "$T/a4"; then echo "  LI   ambitus declaratus in clave: valor mutatus currit, idem servatur OK"; else echo "  LI   FRACTUM (rc=$rc1$rc2$rc3$rc4)"; tail -2 "$T/a2" "$T/a3" "$T/a4" | sed 's/^/      /'; fracta=1; fi

# LII (analytica T3): ACTIONES IN ANNALIBUS - radix_c + d (a transit; d
# vere fracta - c nexu solo transit) sub MENSOR_PARENS et contextu
# hereditato EIUSDEM arboris (MENSOR_ARBOR = radix, T4): cursus unus per
# membrum actum, parens hereditatus, exitus ex eventu (FRACTUM = 1),
# nota = eventus, commissum hereditatum (git non vocatur). Diarium
# crudum legitur (awk) - porta fabricae bin/mensor non eget
radix_c
printf 'int main(void) { return 1; }\n' > "$T/r/t/probatio_d.c"
RP=$(cd "$T/r" && pwd -P)
(cd "$T/r" && MENSOR_PARENS=fumus-lii MENSOR_COMMISSUM=commissum-lii MENSOR_SESSIO=sessio-lii \
    MENSOR_ARBOR="$RP" "$F" probare -vis probationes_t) > "$T/lii.e" 2>&1
awk -F'\t' -v p=fumus-lii '$1 == "cursus" && $2 == "initium" && $4 == p { t[$3] = $6; c[$3] = $9 }
    $1 == "cursus" && $2 == "finis" { e[$3] = $6; n[$3] = $12 }
    END { for (i in t) if (t[i] ~ /probatio_[ad]$/) print t[i] "\t" e[i] "\t" substr(n[i], 1, 7) "\t" c[i] }' \
    "$MENSOR_DIARIUM" 2>/dev/null | sort > "$T/lii"
printf 'probationes_t/probatio_a\t0\tSANATUM\tcommissum-lii\nprobationes_t/probatio_d\t1\tFRACTUM\tcommissum-lii\n' > "$T/lii.exp"
if cmp -s "$T/lii" "$T/lii.exp"; then echo "  LII  actiones in annalibus: cursus per membrum, parens et contextus hereditati, exitus et nota ex eventu OK"; else echo "  LII  FRACTUM"; sed 's/^/      lectum: /' "$T/lii"; fracta=1; fi

# LIII (analytica T3b): SECTIONES IN ANNALIBUS - membrum s plagulam
# CREDO_VERDICTA ipsum scribit (contractus credo sine credo: alpha
# TRANSIIT 40 ms, beta FRACTA 60 ms); cursus filius per sectionem sub
# cursu membri: titulus <membrum>#<sectio>, exitus ex exitu sectionis,
# duratio = ms sectionis, nota = exitus + fractura. Plagula synthetica
# ('totum' a fabrica scriptum, membrum a) filium NULLUM dat
radix_c
cat > "$T/r/t/probatio_s.c" <<'CEOF'
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    const char* v = getenv("CREDO_VERDICTA");
    FILE* f;
    if (v == 0) { return 3; }
    f = fopen(v, "w");
    if (f == 0) { return 4; }
    fprintf(f, "SECTIO\tprobatio_s\talpha\tTRANSIIT\t3\t3\t40\t\t\t\t\n");
    fprintf(f, "SECTIO\tprobatio_s\tbeta\tFRACTA\t1\t2\t60\tt.c:9\tcredo_verum\tx == y\t\n");
    fprintf(f, "SUITA\tprobatio_s\tFRACTA\t1\t2\t4\t5\t100\n");
    fclose(f);
    return 1;
}
CEOF
(cd "$T/r" && MENSOR_PARENS=fumus-liii MENSOR_COMMISSUM=commissum-liii MENSOR_SESSIO=sessio-liii \
    "$F" probare -vis probationes_t) > "$T/liii.e" 2>&1
awk -F'\t' '$1 == "cursus" && $2 == "initium" { t[$3] = $6; p[$3] = $4 }
    $1 == "cursus" && $2 == "finis" { e[$3] = $6; d[$3] = $5; n[$3] = $12 }
    END { for (i in t) if (t[i] ~ /#/ && t[p[i]] ~ /^probationes_t\// && p[p[i]] == "fumus-liii")
              print t[i] "\t" t[p[i]] "\t" e[i] "\t" d[i] "\t" substr(n[i], 1, 6) }' \
    "$MENSOR_DIARIUM" 2>/dev/null | sort > "$T/liii"
printf 'probationes_t/probatio_s#alpha\tprobationes_t/probatio_s\t0\t40\tTRANSI\nprobationes_t/probatio_s#beta\tprobationes_t/probatio_s\t1\t60\tFRACTA\n' > "$T/liii.exp"
if cmp -s "$T/liii" "$T/liii.exp"; then echo "  LIII sectiones in annalibus: filius per sectionem sub membro, exitus et duratio ex credo, synthetica nihil OK"; else echo "  LIII FRACTUM"; sed 's/^/      lectum: /' "$T/liii"; fracta=1; fi

# LIV (analytica T4): RADIX ALIENA - contextus hereditatus arborem
# ALIAM nominat (MENSOR_ARBOR=/alibi, commissum alienum): cursus radicem
# VERAM fert (pwd -P), commissum et ramus vacui (radix non repositorium)
# - actio fixturae numquam ut actio arboris sessionis legitur
radix_c
RP=$(cd "$T/r" && pwd -P)
(cd "$T/r" && MENSOR_PARENS=fumus-liv MENSOR_COMMISSUM=commissum-alienum MENSOR_RAMUS=ramus-alienus \
    MENSOR_ARBOR=/alibi MENSOR_SESSIO=sessio-liv "$F" probare -vis probationes_t) > "$T/liv.e" 2>&1
awk -F'\t' -v p=fumus-liv '$1 == "cursus" && $2 == "initium" && $4 == p && $6 ~ /probatio_a$/ { print $9 "|" $11 "|" $12 }' \
    "$MENSOR_DIARIUM" 2>/dev/null | head -1 > "$T/liv"
if [ "$(cat "$T/liv")" = "||$RP" ]; then echo "  LIV  radix aliena: cursus radicem veram fert, contextus alienus non hereditatus OK"; else echo "  LIV  FRACTUM (lectum: $(cat "$T/liv"), exspectatum: ||$RP)"; fracta=1; fi

# LV (analytica T4): CUSTODIA DIARII - MENSOR_DIARIUM NON positum, HOME
# temporarium (diarium ordinarium = $HOME/.rhubarb/mensurae.diarium):
# radix fixturae (non repositorium) NIHIL scribit; eadem radix ut
# repositorium (git init + commissum) scribit, commisso suo
radix_c
# directorium diarii EXSTAT (aliter "nihil scriptum" ex fopen fracto
# veniret, non ex custodia - pars prima vacue transibat)
mkdir -p "$T/domus/.rhubarb"
(cd "$T/r" && env -u MENSOR_DIARIUM -u MENSOR_DIURNA HOME="$T/domus" MENSOR_ARBOR=/alibi MENSOR_COMMISSUM=c-alienum \
    "$F" probare -vis probationes_t) > "$T/lv.e" 2>&1
lv1=$(cat "$T/domus/.rhubarb/mensurae.diarium" "$T/domus/.rhubarb/diurna/index.tsv" 2>/dev/null | wc -l | tr -d ' ')
# diarium EXPRESSUM, diurna NON: fumus qui diarium solum isolat (ut
# fabrica-fumus ante T4) corpora in diurnam veram non scribit - custodia
# corporum propria (planta F6 T4 superfuit: casus ille non probabatur)
mkdir -p "$T/domus2/.rhubarb"
(cd "$T/r" && env -u MENSOR_DIURNA HOME="$T/domus2" MENSOR_DIARIUM="$T/lv.diarium" MENSOR_ARBOR=/alibi \
    MENSOR_COMMISSUM=c-alienum "$F" probare -vis probationes_t) > "$T/lv.e3" 2>&1
lv4=$(cat "$T/domus2/.rhubarb/diurna/index.tsv" 2>/dev/null | wc -l | tr -d ' ')
lv5=$(grep -c '^cursus	initium' "$T/lv.diarium" 2>/dev/null)
(cd "$T/r" && git init -q . && git add -A && git -c user.email=f@f -c user.name=f commit -q -m i) > /dev/null 2>&1
sha=$(cd "$T/r" && git rev-parse HEAD)
(cd "$T/r" && env -u MENSOR_DIARIUM -u MENSOR_DIURNA HOME="$T/domus" MENSOR_ARBOR=/alibi MENSOR_COMMISSUM=c-alienum \
    "$F" probare -vis probationes_t) > "$T/lv.e2" 2>&1
lv3=$(grep -c '^positum' "$T/domus/.rhubarb/diurna/index.tsv" 2>/dev/null)
lv2=$(awk -F'\t' '$1 == "cursus" && $2 == "initium" && $6 ~ /probatio_a$/ { print $9 }' "$T/domus/.rhubarb/mensurae.diarium" 2>/dev/null | head -1)
if [ "$lv1" = "0" ] && [ "$lv2" = "$sha" ] && [ "${lv3:-0}" -gt 0 ] && [ "$lv4" = "0" ] && [ "${lv5:-0}" -gt 0 ]; then echo "  LV   custodia diarii et diurnae: fixtura nihil scribit, repositorium scribit commisso suo OK"; else echo "  LV   FRACTUM (fixtura lineae $lv1, repositorium commissum '$lv2' pro '$sha', corpora repositorii ${lv3:-0}, corpora fixturae sine diurna expressa $lv4, lineae diarii expressi ${lv5:-0})"; fracta=1; fi

# LVI (analytica T4): CORPUS ACTIONIS IN DIURNA - membrum d effundit et
# frangitur; acta eius (build/fabrica/acta/<titulus>.log) sub id cursus
# in diurna ponuntur: linea positum clavis = id, fractum 1, corpus
# effusionem continet, campus acta lineae finis = sigillum corporis
radix_c
printf '#include <stdio.h>\nint main(void) { printf("d dicit: fracta\\n"); return 1; }\n' > "$T/r/t/probatio_d.c"
RP=$(cd "$T/r" && pwd -P)
(cd "$T/r" && MENSOR_PARENS=fumus-lvi MENSOR_COMMISSUM=commissum-lvi MENSOR_SESSIO=sessio-lvi \
    MENSOR_ARBOR="$RP" "$F" probare -vis probationes_t) > "$T/lvi.e" 2>&1
id_d=$(awk -F'\t' '$1 == "cursus" && $2 == "initium" && $4 == "fumus-lvi" && $6 ~ /probatio_d$/ { print $3 }' "$MENSOR_DIARIUM" | head -1)
acta_d=$(awk -F'\t' -v i="$id_d" '$1 == "cursus" && $2 == "finis" && $3 == i { print $10 }' "$MENSOR_DIARIUM" | head -1)
pos_d=$(awk -F'\t' -v i="$id_d" '$1 == "positum" && $3 == i { print $6 "|" $7 }' "$MENSOR_DIURNA/index.tsv" 2>/dev/null | head -1)
sig_d=${pos_d#*|}
if [ -n "$id_d" ] && [ "${pos_d%%|*}" = "1" ] && [ "$acta_d" = "$sig_d" ] \
   && grep -q "d dicit: fracta" "$MENSOR_DIURNA/corpora/${sig_d:0:2}/${sig_d:2}" 2>/dev/null; then echo "  LVI  corpus actionis in diurna sub id cursus, acta = sigillum OK"; else echo "  LVI  FRACTUM (id '$id_d', positum '$pos_d', acta '$acta_d')"; fracta=1; fi

# LVII (analytica T4 2b-ii): SECTIO RUPTA NOMINATA - membrum r contractum
# credo ipsum scribit (SECTIONES 3, alpha TRANSIIT, INCIPIT beta) deinde
# SIGSEGV: causa membri 'sectio 'beta' RUIT; 1 non cursae' (non 'SUITA
# absens'), cursus filius #beta fractus nota RUIT; q ruit INTER sectiones
# (alpha notata): nulla RUIT, '1 non cursae'
radix_c
cat > "$T/r/t/probatio_r.c" <<'CEOF'
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    const char* v = getenv("CREDO_VERDICTA");
    FILE* f;
    if (v == 0) { return 3; }
    f = fopen(v, "w");
    if (f == 0) { return 4; }
    fprintf(f, "SECTIONES\tprobatio_r\t3\n");
    fprintf(f, "INCIPIT\tprobatio_r\talpha\n");
    fprintf(f, "SECTIO\tprobatio_r\talpha\tTRANSIIT\t1\t1\t5\t\t\t\t\n");
    fprintf(f, "INCIPIT\tprobatio_r\tbeta\n");
    fflush(f);
    raise(SIGSEGV);
    return 0;
}
CEOF
# q: ruina INTER sectiones (alpha notata, beta numquam incepta) -
# nulla sectio RUIT (alpha non duplicatur), '1 non cursae'
cat > "$T/r/t/probatio_q.c" <<'CEOF'
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    const char* v = getenv("CREDO_VERDICTA");
    FILE* f;
    if (v == 0) { return 3; }
    f = fopen(v, "w");
    if (f == 0) { return 4; }
    fprintf(f, "SECTIONES\tprobatio_q\t2\n");
    fprintf(f, "INCIPIT\tprobatio_q\talpha\n");
    fprintf(f, "SECTIO\tprobatio_q\talpha\tTRANSIIT\t1\t1\t5\t\t\t\t\n");
    fflush(f);
    raise(SIGSEGV);
    return 0;
}
CEOF
RP=$(cd "$T/r" && pwd -P)
(cd "$T/r" && MENSOR_PARENS=fumus-lvii MENSOR_COMMISSUM=commissum-lvii MENSOR_SESSIO=sessio-lvii \
    MENSOR_ARBOR="$RP" "$F" probare -vis probationes_t) > "$T/lvii" 2>&1
nb=$(awk -F'\t' '$1 == "cursus" && $2 == "initium" { t[$3] = $6 } $1 == "cursus" && $2 == "finis" { e[$3] = $6; n[$3] = $12 }
    END { for (i in t) if (t[i] == "probationes_t/probatio_r#beta") print e[i] "|" substr(n[i], 1, 4) }' "$MENSOR_DIARIUM" | head -1)
if grep -q "probatio_r FRACTA.*sectio 'beta' RUIT; 1 non cursae" "$T/lvii" && ! grep -q "probatio_r FRACTA.*SUITA absens" "$T/lvii" \
   && [ "$nb" = "1|RUIT" ] && grep -q "probatio_q FRACTA.* - exitus 139: 1 non cursae" "$T/lvii" \
   && ! grep -q "probatio_q FRACTA.*RUIT" "$T/lvii"; then echo "  LVII sectio rupta nominata: RUIT, non cursae numeratae, cursus #beta fractus OK"; else echo "  LVII FRACTUM (cursus #beta '$nb')"; grep "probatio_r" "$T/lvii" | sed 's/^/      /' | head -4; fracta=1; fi

# LVIII (parcum 190VQB): CATENA IUDICII - iudicium artificii ingressus
# DIRECTOS solum iudicabat: g (a->b) et h (b->c), a mutato, 'iudicare
# -plenus c' RECENS dicebat (regeneratio h ex b stalo congruit) - binarium
# ex capsula stala 'RECENS' in praevolatu commissionis (bin/briar,
# 2026-10-15). Nunc catena per arcus DATORUM (exitus alterius in loco
# ingressus) iudicatur: c FORTASSE 'supra: g'. Praecondicio arcus datorum
# non est: k (e->d, praecondicio g) RECENS manet. Controla: ante
# mutationem et post sanare c RECENS.
radix '<fabrica titulus="t"><subsystema via="."/></fabrica>'
printf 'unum\n' > "$T/r/a"; printf 'e\n' > "$T/r/e"
printf '<aedificatio>\n  <actio titulus="g" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_b.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="h" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_c.sh\n    </mandatum>\n    <ingressus genus="fasciculus" via="b"/>\n    <exitus via="c" provenientia="regeneratio"/>\n  </actio>\n  <actio titulus="k" genus="generator">\n    <mandatum>\n      <verbum! (>sh\n      <verbum! (>gen_d.sh\n    </mandatum>\n    <praecondicio actio="g"/>\n    <ingressus genus="fasciculus" via="e"/>\n    <exitus via="d" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' > "$T/r/aedificatio.stml"
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "b:%%s" "$(cat a)" > "$D/b"\n' > "$T/r/gen_b.sh"
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "c:%%s" "$(cat b)" > "$D/c"\n' > "$T/r/gen_c.sh"
printf 'D="${FABRICA_SCRIPTURA:-.}"; printf "d:%%s" "$(cat e)" > "$D/d"\n' > "$T/r/gen_d.sh"
(cd "$T/r" && "$F" sanare) > "$T/lviii_s" 2>&1; rcs=$?
(cd "$T/r" && "$F" iudicare -plenus -machina c) > "$T/lviii_0" 2>&1; rc0=$?
printf 'duo\n' > "$T/r/a"
(cd "$T/r" && "$F" iudicare -plenus -machina c) > "$T/lviii_1" 2>&1; rc1=$?
(cd "$T/r" && "$F" iudicare -plenus -machina d) > "$T/lviii_2" 2>&1; rc2=$?
(cd "$T/r" && "$F" sanare) > /dev/null 2>&1
(cd "$T/r" && "$F" iudicare -plenus -machina c) > "$T/lviii_3" 2>&1; rc3=$?
if [ "$rcs" -eq 0 ] && [ "$rc0" -eq 0 ] && grep -q '^IUDICIUM	RECENS	c	' "$T/lviii_0" \
   && [ "$rc1" -eq 1 ] && grep -q '^IUDICIUM	FORTASSE	c	.*supra: g' "$T/lviii_1" \
   && [ "$rc2" -eq 0 ] && grep -q '^IUDICIUM	RECENS	d	' "$T/lviii_2" \
   && [ "$rc3" -eq 0 ] && grep -q '^IUDICIUM	RECENS	c	' "$T/lviii_3"; then echo "  LVIII catena iudicii: c FORTASSE supra g, praecondicio non propagat, post sanare RECENS OK"; else echo "  LVIII FRACTUM (sanare rc=$rcs; c ante rc=$rc0, post a rc=$rc1, d rc=$rc2, post sanare rc=$rc3)"; for k in 0 1 2 3; do grep '^IUDICIUM' "$T/lviii_$k" | sed "s/^/      [$k] /"; done; fracta=1; fi

# LIX (colloquia T8c): PROGRESSUS - sub MENSOR_PARENS eiusdem arboris
# mensurae parenti adhaerentes: fabrica.ambitus et fabrica.undae initio,
# fabrica.iudicata ultima = ambitus (ultima semper scribitur); radix
# aliena sine diario expresso (custodia LV) NIHIL progressus
radix_c
RP=$(cd "$T/r" && pwd -P)
(cd "$T/r" && MENSOR_PARENS=fumus-lix MENSOR_SESSIO=sessio-lix MENSOR_ARBOR="$RP" \
    "$F" probare -vis probationes_t) > "$T/lix.e" 2>&1
lix_a=$(awk -F'\t' '$3 == "fumus-lix" && $4 == "fabrica.ambitus" { print $5 + 0 }' "$MENSOR_DIARIUM" | tail -1)
lix_u=$(awk -F'\t' '$3 == "fumus-lix" && $4 == "fabrica.undae" { print $5 + 0 }' "$MENSOR_DIARIUM" | tail -1)
lix_i=$(awk -F'\t' '$3 == "fumus-lix" && $4 == "fabrica.iudicata" { print $5 + 0 }' "$MENSOR_DIARIUM" | tail -1)
mkdir -p "$T/domus3/.rhubarb"
(cd "$T/r" && env -u MENSOR_DIARIUM -u MENSOR_DIURNA HOME="$T/domus3" MENSOR_PARENS=fumus-lix-alienum \
    MENSOR_ARBOR=/alibi "$F" probare -vis probationes_t) > "$T/lix.e2" 2>&1
lix_x=$(grep -c 'fabrica\.' "$T/domus3/.rhubarb/mensurae.diarium" 2>/dev/null)
if [ "${lix_a:-0}" -ge 1 ] && [ "${lix_u:-0}" -ge 1 ] && [ "$lix_i" = "$lix_a" ] && [ "${lix_x:-0}" = "0" ]; then echo "  LIX  progressus: ambitus et undae initio, iudicata ultima = ambitus, radix aliena nihil OK"; else echo "  LIX  FRACTUM (ambitus '${lix_a}', undae '${lix_u}', iudicata ultima '${lix_i}', lineae alienae ${lix_x:-0})"; fracta=1; fi

if [ "$fracta" -ne 0 ]; then echo "fumus fabricae: FRACTUM"; exit 1; fi
echo "fumus fabricae: sanum (LIX/LIX)"
exit 0
