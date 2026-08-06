#!/bin/bash

# tools/natura_metamodulus_generare.sh [-probare]
#
# Catalogum vocabularii in natura/METAMODULUS.md e natura/natura.canon
# GENERAT (inter signa GENERATUM INITIUM/FINIS) - littera e canone,
# doctrina manu. Mos manifestorum silvae: littera generata committitur,
# et porta (-probare) rancorem clamat.
#
#   sine argumento   catalogum regenerare et in documentum inserere
#   -probare         solum conferre: exitus 0 recens, 1 RANCIDUS,
#                    2 defectus instrumenti (numquam sanitas tacita)

set -u
cd "$(dirname "$0")/.." || exit 2

DOC=natura/METAMODULUS.md
CANON=natura/natura.canon
PORTA=bin/canon_examen
M_INI='<!-- GENERATUM INITIUM: catalogus vocabularii e natura/natura.canon -->'
M_FIN='<!-- GENERATUM FINIS -->'

if [ ! -x "$PORTA" ]; then
    echo "natura_metamodulus: $PORTA abest - strue: ./tools/canon_struere.sh" >&2
    exit 2
fi
for _f in lib/canon.c include/canon.h tools/canon_examen.c lib/stml.c; do
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_metamodulus: $PORTA STALUS - strue: ./tools/canon_struere.sh" >&2
        exit 2
    fi
done

TMPD=build/natura_metamodulus_tmp
mkdir -p "$TMPD"

"$PORTA" -index "$CANON" > "$TMPD/index.tsv"
if [ ! -s "$TMPD/index.tsv" ]; then
    echo "natura_metamodulus: index vacuus ($CANON)" >&2
    exit 2
fi

awk -F'\t' '
$1=="E" { ne++; eord[ne]=$2; eintra[$2]=$3; eradix[$2]=$4; etex[$2]=$5 }
$1=="A" { na[$2]++; an[$2,na[$2]]=$3; ag[$2,na[$2]]=$4; anec[$2,na[$2]]=$5; aord[$2,na[$2]]=$6 }
$1=="O" { k=$2 SUBSEP $3; ao[k] = (k in ao) ? ao[k] "\\|" $4 : $4 }
$1=="L" { nl[$2]++; ln[$2,nl[$2]]=$3; lmin[$2,nl[$2]]=$4; lmax[$2,nl[$2]]=$5 }
$1=="U" { nu++; un[nu]=$2; ua[nu]=$3; us[nu]=$4; ui[nu]=$5 }
$1=="C" { nc++; cn[nc]=$2; ca[nc]=$3; cad[nc]=$4; cs[nc]=$5; cin[nc]=$6 }
END {
  print "*Catalogus GENERATUS - noli manu emendare. Fons:"
  print "`natura/natura.canon`; regenera:"
  print "`./tools/natura_metamodulus_generare.sh`. Attributa"
  print "necessaria **pinguia**; genus additum ubi non textus;"
  print "=valor praestitutus (ordinarius=); cardinalitas"
  print "liberorum min..max.*"
  print ""
  print "| Elementum | Attributa | Liberi | Textus |"
  print "|---|---|---|---|"
  for (i=1;i<=ne;i++) {
    e=eord[i]
    cell="`" e "`"
    if (eradix[e]=="verum") cell = cell " (radix)"
    if (eintra[e]!="-")     cell = cell " (intra `" eintra[e] "`)"
    as=""
    for (j=1;j<=na[e];j++) {
      s = (anec[e,j]=="verum") ? "**" an[e,j] "**" : an[e,j]
      if (ag[e,j]!="textus") s = s ":" ag[e,j]
      k = e SUBSEP an[e,j]
      if (k in ao) s = s "{" ao[k] "}"
      if (aord[e,j]!="-") s = s "=" aord[e,j]
      as = as ? as ", " s : s
    }
    if (as=="") as = "&mdash;"
    ls=""
    for (j=1;j<=nl[e];j++) {
      mx = (lmax[e,j]=="-") ? "*" : lmax[e,j]
      s = "`" ln[e,j] "` " lmin[e,j] ".." mx
      ls = ls ? ls ", " s : s
    }
    if (ls=="") ls = "&mdash;"
    tx = etex[e]
    if (tx=="-") tx="&mdash;"; else if (tx=="verum") tx="licet"
    print "| " cell " | " as " | " ls " | " tx " |"
  }
  if (nu>0) {
    print ""
    print "**Unicitates:**"
    for (i=1;i<=nu;i++) {
      s = "- `" un[i] "`: attributum `" ua[i] "` super " us[i]
      if (ui[i]!="-") s = s " (intra `" ui[i] "`)"
      print s
    }
  }
  if (nc>0) {
    print ""
    print "**Citationes** (clavis-relationes intra documentum):"
    for (i=1;i<=nc;i++) {
      s = "- `" cn[i] "`: attributum `" ca[i] "` → `" cad[i] "`"
      if (cs[i]!="-")  s = s " super " cs[i]
      if (cin[i]!="-") s = s " (intra `" cin[i] "`)"
      print s
    }
  }
}
' "$TMPD/index.tsv" > "$TMPD/catalogus.md"

if ! grep -qF "$M_INI" "$DOC"; then
    echo "natura_metamodulus: signum GENERATUM INITIUM abest in $DOC" >&2
    exit 2
fi

awk -v ini="$M_INI" -v fin="$M_FIN" -v nov="$TMPD/catalogus.md" '
  $0 == ini { print; while ((getline l < nov) > 0) print l; close(nov); intus=1; next }
  $0 == fin { intus=0; print; next }
  !intus    { print }
' "$DOC" > "$TMPD/doc.md"

if [ "${1:-}" = "-probare" ]; then
    if cmp -s "$TMPD/doc.md" "$DOC"; then
        echo "natura_metamodulus: catalogus recens"
        exit 0
    fi
    echo "natura_metamodulus: catalogus RANCIDUS - canon mutatus sine regeneratione. Regenera: ./tools/natura_metamodulus_generare.sh" >&2
    exit 1
fi

mv "$TMPD/doc.md" "$DOC"
echo "natura_metamodulus: catalogus scriptus in $DOC"
