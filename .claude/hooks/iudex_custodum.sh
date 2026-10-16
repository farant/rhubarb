#!/bin/bash
# iudex_custodum.sh - FONTANDUM a custodibus post-editionem: iudicatne
# iudex unitatum (unitates.stml <iudex>) plagulam modo scriptam? Custodes
# indicem directoriorum suum NON tenent (spec unitates-codicis U4).
#
#   custos_iudicat IUDEX VIA_ABSOLUTA
#     0 iudicatur | 1 non (extra repositorium, ignorata git, aut iudex
#     eam excludit) | 2 iudex fractus aut absens - CUSTOS_IUDEX_CAUSA
#     nominat (custos CLAMAT, non tacet: porta muta = porta mortua)
#
# Arbor = arbor PLAGULAE (git rev-parse), non custodis: politica et
# iudex eius arboris valent. Binarium arboris directe vocatur (~XXX ms)
# si fontibus suis recentius est; aliter tools/iudex_unitatum.sh se
# struit (~CD ms). Causa cur plagula non iudicatur: bin/fabrica quid
# -iudex IUDEX VIA.

custos_iudicat () {
    local iudex="$1" via="$2" arbor rel bin fons stalum=0 ex err
    CUSTOS_IUDEX_CAUSA=""
    arbor="$(git -C "$(dirname "$via")" rev-parse --show-toplevel 2>/dev/null)" || return 1
    case "$via" in
        "$arbor"/*) rel="${via#"$arbor"/}" ;;
        *) return 1 ;;
    esac
    if git -C "$arbor" check-ignore -q -- "$rel" 2>/dev/null; then
        return 1
    fi
    if [ ! -f "$arbor/unitates.stml" ] || [ ! -x "$arbor/tools/iudex_unitatum.sh" ]; then
        CUSTOS_IUDEX_CAUSA="arbor $arbor sine unitates.stml aut tools/iudex_unitatum.sh (fusio main poscitur) - custos tacet"
        return 2
    fi
    bin="$arbor/build/iudex_unitatum/iudex_unitatum"
    [ -x "$bin" ] || stalum=1
    for fons in tools/iudex_unitatum.c lib/unitates.c lib/unitates_arboris.c \
                include/unitates.h include/unitates_arboris.h; do
        # <tolera codex="lint:nt-aequalitas" (>aequalitas = STALUM (binarium non strictius recentius -> se struit): directio tuta, recompilatio superflua sola
        [ "$stalum" = 1 ] || [ "$bin" -nt "$arbor/$fons" ] || stalum=1
    done
    err="$(mktemp "${TMPDIR:-/tmp}/iudex_custodum.XXXXXX")"
    if [ "$stalum" = 1 ]; then
        ex="$(cd "$arbor" && ./tools/iudex_unitatum.sh "$iudex" "$rel" 2>"$err")"
    else
        ex="$(cd "$arbor" && "$bin" "$iudex" "$rel" 2>"$err")"
    fi || {
        CUSTOS_IUDEX_CAUSA="iudex $iudex fractus in $arbor - custos tacet: $(grep -v '^ *\[' "$err" | head -3 | tr '\n' ' ')"
        rm -f "$err"; return 2; }
    rm -f "$err"
    [ "$ex" = "$rel" ]
}

# custos_iudicat + egressus custodis: 1 -> exit 0 tacite; 2 -> nuntius
# unus (additionalContext) et exit 0. Vocatur ut 'custos_admittere
# TITULUS_CUSTODIS IUDEX "$FILE" || exit 0'.
custos_admittere () {
    local custos="$1" rc
    custos_iudicat "$2" "$3"; rc=$?
    if [ "$rc" -eq 2 ]; then
        jq -n --arg r "$custos: $CUSTOS_IUDEX_CAUSA" \
            '{hookSpecificOutput:{hookEventName:"PostToolUse",additionalContext:$r}}'
    fi
    return "$rc"
}
