#!/bin/bash
# natura-custos.sh - PostToolUse (Write|Edit): iudicium bibliothecae
# semanticae in plagulam .genera modo scriptam - canalis PUSH
# (exemplar censor-custos.sh).
#
# Disciplina silentii: exitus purus = NIHIL in contextum;
# flagrationes solae additionalContext iniciunt (informatio, non
# obex - turnus pergit).
#
# MOMENTUM: bin/natura_examen exemplaria OMNIA onerat etiamsi unum
# iudicetur, quia regulae II/III/IV trans exemplaria transeunt -
# iudicium plagulae SOLIUS eas videre non potest. Filtrum post
# nexuram applicatur. Sumptus: XXIX millesima (natura_visus.sh
# XIV secunda - unde porta C, non shell).
#
# PORTAE DUAE, axes DIVERSI: (I) natura_examen = an exemplar ipsum
# sanum sit; (II) natura_canones -probare = an canones cocti ex eo
# recentes sint. Alterum sine altero fieri potest, ergo ambae
# semper currunt et in UNUM nuntium coeunt. Sumptus totalis fere
# unum secundum per conservationem (fere omnis in canonibus XXXIV
# regenerandis et conferendis) - pretium contra litteram generatam
# quae tacite a fonte suo discedit, in arbore COMMISSA.

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.genera) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PORTA="$RADIX/bin/natura_examen"
COCTOR="$RADIX/tools/natura_canones.sh"

# UNUM obiectum JSON per vocationem: nuntii in NUNTIO congeruntur,
# emissio semel in fine fit. Antea ramus quisque ipse emittebat et
# exibat - quod contractum servabat dum nemo ramum addebat. Porta
# canonum ramum ADDIT (orthogonalis: exemplar sanum esse et
# canones eius rancidos simul licet), ergo contractus structura
# tenetur, non diligentia.
NUNTIUS=""

adicere () {
    if [ -n "$NUNTIUS" ]; then
        NUNTIUS="$NUNTIUS"$'\n'"$1"
    else
        NUNTIUS="$1"
    fi
}

# porta absens = silentium, NON successus tacitus: si numquam
# structa est, dic id semel potius quam nihil agere fingens
if [ ! -x "$PORTA" ]; then
    adicere "NATURA: bin/natura_examen abest - porta .genera TACET. Strue: ./tools/natura_struere.sh"
else
    OUT=$(cd "$RADIX" && "$PORTA" -plagula "$FILE" -machina 2>&1)
    STATUS=$?
    ROWS=$(printf '%s\n' "$OUT" | grep '^VULNUS' | head -10)

    # exitus II = NIHIL ONERATUM (radix vacua/mota) - defectus portae
    # ipsius, qui tacite ut sanitas legi NON debet
    if [ "$STATUS" -eq 2 ]; then
        adicere "NATURA: porta NIHIL oneravit (exitus II) - exemplaria non inventa, iudicium NULLUM factum est:
$OUT"
    elif [ -n "$ROWS" ]; then
        adicere "NATURA (uncus post-editionem): contractus violatus in plagula modo scripta - campi: GRADUS	REGULA	EXEMPLAR	ENS	NUNTIUS
$ROWS
Regulae: natura/METAMODULUS.md par.8. Corpus totum: ./bin/natura_examen"
    # VULNERA alibi (haec plagula sana, bibliotheca fracta) - dic,
    # quia nexus fractus saepe ex hac editione natus est
    elif [ "$STATUS" -eq 1 ]; then
        ALIA=$(cd "$RADIX" && "$PORTA" -machina 2>/dev/null | grep -c '^VULNUS')
        adicere "NATURA: haec plagula sana, sed bibliotheca $ALIA VULNERA alibi fert (nexus trans-exemplaris fortasse ab hac editione fractus). Vide: ./bin/natura_examen"
    fi
fi

# ------------------------------------------------------------------
# rancor canonum coctorum: .genera mutatum canonem natura/cocta/
# stalum relinquit. Littera generata committitur, ergo rancor
# TACITUS in arborem labi potest - hic est locus ubi se prodit.
#
# Exitus I et II nuntios DIVERSOS merent. I = RANCIDUS, regeneratio
# statim sanat. II = porta ipsa cursum non peregit (instrumentum
# absens aut stalum) - et regeneratio instrumento STALO est
# praecise quomodo littera stala benedicitur. Qui eos confundit
# auctori dicit ut regeneret cum res vera aliud poscat.
if [ ! -x "$COCTOR" ]; then
    adicere "NATURA: tools/natura_canones.sh abest - porta rancoris TACET."
else
    # SINE 'cd': natura_canones.sh se ipsum ad radicem ponit (per
    # $0), ergo a quovis loco currit. 'cd X && cmd' hic damnosum
    # esset - cd lapsus exitum I daret, quem porta RANCOREM legeret:
    # clamor falsus ex defectu unde silentium deberetur.
    RQ=$("$COCTOR" -probare 2>&1 >/dev/null)
    RSTATUS=$?
    if [ "$RSTATUS" -eq 1 ]; then
        RANCIDI=$(printf '%s\n' "$RQ" | grep '^natura_canones: RANCIDUS ' | awk '{print $3}' | head -5 | paste -sd' ' -)
        OMNES=$(printf '%s\n' "$RQ" | grep -c '^natura_canones: RANCIDUS ')
        # truncatio TACITA nulla: si plures quam quinque, dic quot
        [ -n "$RANCIDI" ] || RANCIDI="nomina non lecta"
        [ "$OMNES" -gt 5 ] && RANCIDI="$RANCIDI (+ $((OMNES - 5)) alii)"
        adicere "NATURA: canones cocti RANCIDI ($RANCIDI) - regenera: ./tools/natura_canones.sh"
    elif [ "$RSTATUS" -ne 0 ]; then
        CAUSA=$(printf '%s\n' "$RQ" | tail -3)
        adicere "NATURA: porta rancoris NIHIL probavit (exitus $RSTATUS) - canones cocti NON collati sunt:
$CAUSA"
    fi
fi

[ -n "$NUNTIUS" ] || exit 0
jq -n --arg c "$NUNTIUS" '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:$c}}'
exit 0
