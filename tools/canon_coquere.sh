#!/bin/bash

# tools/canon_coquere.sh [-probare]
#
# Lectores generatos ex cocta.registrum regenerare aut probare.
# Mos portae METAMODULI: littera generata committitur, porta
# rancorem clamat.
#
#   sine argumento   omnes lectores regenerare (in locis veris)
#   -probare         solum conferre: exitus 0 recentes, 1 RANCIDI,
#                    2 defectus instrumenti (numquam sanitas tacita)

set -u
cd "$(dirname "$0")/.." || exit 2

MANIFESTUM=cocta.registrum
PORTA=bin/canon_coquere

if [ ! -x "$PORTA" ]; then
    echo "canon_coquere: $PORTA abest - strue: ./tools/canon_struere.sh" >&2
    exit 2
fi
# CAPUT CUM CORPORE SUO: canon.h iam iuxta canon.c stabat, stml.h
# non - et lector stml.h mutato aeque stalus fit. Forma recta in
# eodem indice iam aderat, uni bibliothecae applicata, alteri non.
# Absentia praeterea defectus est, non silentium: '-nt'
# plagulae absentis falsum est, unde fons renominatus ex custodia
# TACITE caderet.
for _f in lib/canon.c include/canon.h tools/canon_coquere.c \
          lib/stml.c include/stml.h; do
    if [ ! -e "$_f" ]; then
        echo "canon_coquere: custos '$_f' ABEST - index custodum fontem nominat qui non est" >&2
        exit 2
    fi
    if [ "$_f" -nt "$PORTA" ]; then
        echo "canon_coquere: $PORTA STALUS ($_f recentior) - strue: ./tools/canon_struere.sh" >&2
        exit 2
    fi
done
if [ ! -f "$MANIFESTUM" ]; then
    echo "canon_coquere: $MANIFESTUM abest" >&2
    exit 2
fi

# TMPD per PROCESSUM proprium ($$), mos natura_canones.sh: duo
# unci hanc portam nunc accendunt (canon-custos in .canon,
# natura-custos in .genera), et duae conservationes celeres
# cursus duos simul dant. Loco fixo PROBANDUM inter cursus
# intertexeretur, et PROBANDUM alienum collatum RANCOREM FALSUM
# pareret - id est porta quae de re sana clamat, quod portae mors
# est. (Substitutiones sed infra valorem TMPD verum adhibent,
# ergo nomen mutatum eas non tangit.)
TMPD=build/canon_coquere_tmp.$$
mkdir -p "$TMPD" || exit 2
# apostrophus SIMPLEX: '$TMPD' ad tempus laquei solvitur, ergo
# nomen uno loco solo stat (via litteralis repetita duo loca
# consentire cogeret, et RELATIVA est).
trap 'rm -rf "$TMPD"' EXIT

# FORMA PARS ARTIFICII EST, non gradus posterior.
#
# lib/ et include/ FORMATORE purgata sunt, sed lector coctus
# GENERATUR et emissor formam domus nescit. Sine hoc gradu circulus
# oritur: regeneratio opus formatoris delet, formatio recentiam
# frangit - neuter status quietus, ergo lectores RANCIDI perpetuo
# stabant (mensuratum 2026-08-22, res 01M0NXT3QD).
#
# Emissorem regulas formatoris docere fontem formae SECUNDUM
# faceret. Hic formator unus manet et emissor earum ignarus - una
# forma, unus custos.
#
# EXITUS I EXSPECTATUS EST, non defectus: formator lineas nimis
# longas (longitudo-lxxii) in littera generata NUNTIAT sed non
# refingit. Ergo I patimur, II solum clamat. Gradum in exitu
# formatoris ponere portam nostram perpetuo frangeret.
#
# Formator PUNCTUM FIXUM est (scriptio secunda octetos non mutat -
# probatum), ergo collatio post formationem stabilis est.
FORMATOR=./silva/formator.sh

_formare () {
    local via="$1"
    local rc
    if [ ! -x "$FORMATOR" ]; then
        echo "canon_coquere: $FORMATOR abest - forma pars artificii est" >&2
        exit 2
    fi
    "$FORMATOR" "$via" -scribere > /dev/null 2>&1
    rc=$?
    if [ "$rc" -ge 2 ]; then
        echo "canon_coquere: formator '$via' legere non potuit (exitus $rc)" >&2
        exit 2
    fi
    return 0
}

rancidi=0
facti=0
while IFS='	' read -r canon praefixum caput corpus; do
    case "$canon" in ''|\#*) continue ;; esac

    if [ "${1:-}" = "-probare" ]; then
        "$PORTA" "$canon" -praefixum "$praefixum" \
            -caput "$TMPD/probandum.h" -corpus "$TMPD/probandum.c" \
            2>/dev/null || { echo "canon_coquere: '$canon' coqui nequit" >&2; exit 2; }
        # copia temporalis in TRIBUS locis a vera divergit: iussum
        # regenerationis (viae), custos includendi (PROBANDUM_H),
        # linea #include (basis capitis). Omnes substituendae ne
        # differentia falsa ex viis ipsis nascatur.
        caput_basis=$(basename "$caput")
        custos_verus=$(echo "$caput_basis" | tr 'a-z.-' 'A-Z__')
        sed -e "s|$TMPD/probandum.h|$caput|g" \
            -e "s|$TMPD/probandum.c|$corpus|g" \
            -e "s|PROBANDUM_H|$custos_verus|g" \
            "$TMPD/probandum.h" > "$TMPD/probandum2.h"
        sed -e "s|$TMPD/probandum.h|$caput|g" \
            -e "s|$TMPD/probandum.c|$corpus|g" \
            -e "s|\"probandum.h\"|\"$caput_basis\"|g" \
            "$TMPD/probandum.c" > "$TMPD/probandum2.c"
        # Formatio POST substitutionem, ordine eodem quo semita
        # scriptionis: aliter probandum et verum per gradum unum
        # differrent et porta rancorem falsum nuntiaret.
        _formare "$TMPD/probandum2.h"
        _formare "$TMPD/probandum2.c"
        if ! cmp -s "$TMPD/probandum2.h" "$caput" \
           || ! cmp -s "$TMPD/probandum2.c" "$corpus"; then
            echo "canon_coquere: RANCIDUS $caput / $corpus (fons: $canon) - regenera: ./tools/canon_coquere.sh" >&2
            rancidi=$((rancidi + 1))
        fi
        facti=$((facti + 1))
    else
        "$PORTA" "$canon" -praefixum "$praefixum" \
            -caput "$caput" -corpus "$corpus" || exit 2
        _formare "$caput"
        _formare "$corpus"
        facti=$((facti + 1))
    fi
done < "$MANIFESTUM"

if [ "$facti" -eq 0 ]; then
    echo "canon_coquere: NIHIL in manifesto - nihil factum" >&2
    exit 2
fi

if [ "${1:-}" = "-probare" ]; then
    if [ "$rancidi" -gt 0 ]; then
        exit 1
    fi
    echo "canon_coquere: lectores $facti recentes"
fi
exit 0
