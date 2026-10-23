#!/bin/bash
# tools/fabrica_fumus_vivus.sh - porta fabrica-fumus-vivus: casus fumi
# fabricae qui arborem VIVAM legunt (iudex ipse, digestum veri, iudicium
# arboris totae, census, familiae installatorum, indices memoriae vivae).
# Translati verbatim ex tools/fabrica_fumus.sh (2026-10-23, desideratum
# …VXWK8): auditus lectionum per casus hos solos arborem totam legere
# monstravit; ceteri in mundis temporariis currunt (porta fabrica-fumus).
# Numeri casuum originales servantur.
#
# Exitus: 0 sanum | 1 FRACTUM | 2 bin/fabrica deest.
# <porta titulus="fabrica-fumus-vivus" gradus="fusio" currit="tools/fabrica.c" legit="*"/>
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
set -u
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

# radices temporariae (casus VIII)
radix () { rm -rf "$T/r"; mkdir -p "$T/r"; : > "$T/r/aedilis.stml"; printf '%s\n' "$1" > "$T/r/fabrica.stml"; }
actio () { printf '<aedificatio>\n  <actio titulus="%s" genus="generator">\n    <ingressus genus="fasciculus" via="a"/>\n    <exitus via="b" provenientia="regeneratio"/>\n  </actio>\n</aedificatio>\n' "$1"; }

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

# XI (T7): 'digestum' == ingressus relatus - scriptum (installator) et
# iudex EADEM functione sigillant (T7: _digestum olim exclusionem
# omittebat, tres digesta diversa)
D="$("$F" digestum fabrica 2>/dev/null)"
R="$("$F" -provenientia | sed -n 's/^ingressus //p')"
if [ -n "$D" ] && [ "$D" = "$R" ]; then echo "  XI   digestum == relatio binarii      OK"; else echo "  XI   FRACTUM (digestum $D, relatio $R)"; fracta=1; fi

# XXXIII (fabrica-6 T1): CENSUS CHASSIS - 'bin/fabrica census' genera
# registrata cum proprietatibus (TSV) enumerat; numerus = registrum
# (XIII post T2: repositorium), nullum genus sine linea; par res/clavis
(cd "$RADIX" && bin/fabrica census) > "$T/census.o" 2>&1; rc=$?
n_gen=$(grep -c '^genus	' "$T/census.o")
n_summa=$(sed -n 's/^census: genera \([0-9]*\).*/\1/p' "$T/census.o")
if [ "$rc" -eq 0 ] && [ -n "$n_summa" ] && [ "$n_gen" -eq "$n_summa" ] && [ "$n_gen" -ge 13 ] && grep -q '^genus	fasciculus	par:plagula/contentum	' "$T/census.o" && grep -q '^genus	repositorium	par:repositorium/commissum	' "$T/census.o"; then echo "  XXXIII census chassis ($n_gen genera)      OK"; else echo "  XXXIII FRACTUM (census: rc=$rc, genera $n_gen, summa $n_summa)"; head -5 "$T/census.o" | sed 's/^/      /'; fracta=1; fi

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

# XLVII (cursores ad gradus T3b): INDICES MEMORIAE - radix XLVI (memoria
# a probare creata) et memoria viva (migratio in apertura): consilium
# quaestionis sqlite 'USING ... INDEX' pro quaestionibus per titulum
indices_ok=1; indices_mala=""
for db in "$RADIX/build/fabrica.db"; do
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

if [ "$fracta" -ne 0 ]; then echo "fumus fabricae vivus: FRACTUM"; exit 1; fi
echo "fumus fabricae vivus: sanum (VI VII VIII XI XXXIII XLI XLVII)"
exit 0
