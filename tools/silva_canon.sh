#!/bin/bash

# tools/silva_canon.sh [-probare]
#
# Canonem silva/c89.canon ex grammatica generare aut rancorem probare.
# Mos portae METAMODULI, idem quem tools/natura_canones.sh tenet:
# littera generata committitur, porta rancorem clamat.
#
#   sine argumento   regenerare (in loco vero)
#                    exitus 0 = catena TOTA recens; 2 defectus;
#                    3 canon scriptus sed VALIDITAS eius inexplorata
#   -probare         conferre solum: exitus 0 recens, 1 RANCIDUS,
#                    2 defectus instrumenti (numquam sanitas tacita)
#
# CUR EXITUS III EXSISTIT: regeneratio ab INSTRUMENTO fit, non ab
# editione - ergo nullus uncus accenditur et arbor cum artificio
# infirmato committi potest, nemine dicente. Hic canon LECTORES
# coctos non habet (contra naturam), ergo exitus 3 alteram partem
# contractus tenet: 'ignotum numquam pro sano nuntiatur'. Si canon
# scribitur sed iudicari NON potest (bin/canon_examen abest), id
# exitus 3 est, non 0.
#
# PORTA OUTPUT CONFERT, NUMQUAM TEMPORA (lex a natura_canones
# empta): commentarium in grammatica additum generationem non
# mutat, ergo rancorem non parit. Porta temporum de commentariis
# clamaret, et porta quae falso clamat mox neglegitur - id est mors
# portae. Ideo etiam caput canonis vias INVOCATIONIS non fert
# (basis sola): duae regenerationes rectae dissentire non debent.

set -u
cd "$(dirname "$0")/.." || exit 2

GENERATOR=silva/build/generator
GRAMMATICA=silva/grammatica/c89.stml
DIALECTUS=c89
CANON=silva/c89.canon
STRUCTOR=./silva/generare.sh
IUDEX=bin/canon_examen
METACANON=canon.canon

# ------------------------------------------------------------------
# argumenta

PROBARE=0
if [ "$#" -gt 1 ]; then
    echo "usus: ./tools/silva_canon.sh [-probare]" >&2
    exit 2
fi
if [ "$#" -eq 1 ]; then
    if [ "$1" = "-probare" ]; then
        PROBARE=1
    else
        # Argumentum ignotum TACITE ignoratum '-probar' in
        # SCRIPTIONEM verteret: petitor iudicium petivit,
        # scripturam accepit.
        echo "silva_canon: argumentum ignotum '$1'" >&2
        echo "usus: ./tools/silva_canon.sh [-probare]" >&2
        exit 2
    fi
fi

# ------------------------------------------------------------------
# porta ipsa: instrumentum stalum litteram stalam benediceret

if [ ! -x "$GENERATOR" ]; then
    echo "silva_canon: $GENERATOR abest - strue: $STRUCTOR" >&2
    exit 2
fi
if [ ! -e "$GRAMMATICA" ]; then
    echo "silva_canon: grammatica '$GRAMMATICA' ABEST" >&2
    exit 2
fi
# CAPITA CUM CORPORIBUS: '-nt' plagulae ABSENTIS falsum est, ergo
# fons renominatus ex custodia TACITE cadit - porta muta a porta
# mortua non discernitur. Absentia ergo defectus est, non silentium.
#
# GRAMMATICA HIC ABEST CONSULTO. Custodes hi rogant 'an BINARIUM
# stalum sit', et grammatica in binarium non compilatur - ad
# tempus cursus legitur. Eam addere casum PRIMARIUM everteret:
# grammatica emendata exitum 2 ('instrumentum stalum') pareret ubi
# exitus 1 ('canon rancidus') debetur - id est, porta rem quam
# custodire nata est TEGERET. Praesentia eius supra probatur.
for _f in silva/instrumenta/silva_canon_coquere.c \
          silva/instrumenta/silva_canon_coquere.h \
          silva/instrumenta/silva_generare.c \
          silva/instrumenta/silva_generare.h \
          silva/instrumenta/principalia/generator.c \
          silva/fontes/silva_token.c \
          silva/fontes/silva_token.h; do
    if [ ! -e "$_f" ]; then
        echo "silva_canon: custos '$_f' ABEST - index custodum fontem nominat qui non est" >&2
        exit 2
    fi
    if [ "$_f" -nt "$GENERATOR" ]; then
        echo "silva_canon: $GENERATOR STALUS ($_f recentior) - strue: $STRUCTOR" >&2
        exit 2
    fi
done

# TMPD per PROCESSUM proprium ($$): duo cursus simul (uncus +
# manus) loco fixo inter se intertexerentur, et PROBANDUM alienum
# collatum RANCOREM FALSUM pareret.
TMPD=build/silva_canon_tmp.$$
mkdir -p "$TMPD" || exit 2
trap 'rm -rf "$TMPD"' EXIT
PROBANDUM="$TMPD/probandum.canon"

if ! "$GENERATOR" -canon "$GRAMMATICA" "$DIALECTUS" "$PROBANDUM"; then
    echo "silva_canon: generator defecit" >&2
    exit 2
fi
if [ ! -s "$PROBANDUM" ]; then
    echo "silva_canon: canon probandus VACUUS - generator tacite defecit" >&2
    exit 2
fi

# ------------------------------------------------------------------
# collatio OUTPUT

if [ ! -e "$CANON" ]; then
    rancidus=1
    causa="canon '$CANON' omnino ABEST"
elif cmp -s "$PROBANDUM" "$CANON"; then
    rancidus=0
    causa=""
else
    rancidus=1
    causa="canon a grammatica DIVERGIT"
fi

sigillum_novum="$(grep -o 'sigillum="[a-f0-9]*"' "$PROBANDUM" | head -1)"
sigillum_vetus=""
[ -e "$CANON" ] && sigillum_vetus="$(grep -o 'sigillum="[a-f0-9]*"' "$CANON" | head -1)"

if [ "$PROBARE" -eq 1 ]; then
    if [ "$rancidus" -eq 0 ]; then
        echo "silva_canon: $CANON recens ($sigillum_novum)"
        exit 0
    fi
    echo "silva_canon: RANCIDUS - $causa" >&2
    # Sigillum DISCERNIT quid mutatum sit: fontes derivationis
    # (grammatica, tabulae, nomina) an EMISSOR solus. Duo remedia
    # diversa sunt, ergo distinctio utilis.
    if [ -n "$sigillum_vetus" ] && [ "$sigillum_vetus" != "$sigillum_novum" ]; then
        echo "  sigillum mutatum: $sigillum_vetus -> $sigillum_novum (fontes derivationis)" >&2
    else
        echo "  sigillum IDEM ($sigillum_novum) - ergo EMISSOR mutatus, non grammatica" >&2
    fi
    echo "  regenera: ./tools/silva_canon.sh (aut $STRUCTOR)" >&2
    exit 1
fi

# ------------------------------------------------------------------
# scriptio

if [ "$rancidus" -eq 0 ]; then
    echo "silva_canon: $CANON iam recens ($sigillum_novum)"
else
    cp "$PROBANDUM" "$CANON" || exit 2
    echo "silva_canon: $CANON SCRIPTUS ($sigillum_novum)"
fi

# Validitas: canon scriptus qui a canon.canon iudicari NON potest
# ignotus est, et ignotum pro sano nuntiare est id ipsum quod
# exitus 3 vetat.
if [ ! -x "$IUDEX" ] || [ ! -e "$METACANON" ]; then
    echo "silva_canon: $IUDEX aut $METACANON abest - canon scriptus, VALIDITAS INEXPLORATA" >&2
    exit 3
fi
if ! "$IUDEX" -canon "$METACANON" "$CANON" >/dev/null 2>&1; then
    echo "silva_canon: canon scriptus sed a $METACANON REICITUR" >&2
    "$IUDEX" -canon "$METACANON" "$CANON" >&2 2>&1 | head -20
    exit 3
fi
echo "silva_canon: catena recens ($METACANON canonem probat)"
exit 0
