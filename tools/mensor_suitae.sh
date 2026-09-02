#!/bin/bash
# tools/mensor_suitae.sh - metra suitae, FONTANDA a cursoribus probationum
#
# usus (in cursore, post RADIX cognitum):
#   source "$RADIX/tools/mensor_suitae.sh"
#   mensor_suitae_incipere "silva."      # praefixum titulorum; "" = radix
#   ... per probationem:
#   t0=$(mensor_suitae_nunc); clang ...;  mensor_suitae_compilatio "$nomen" "$t0"
#   t0=$(mensor_suitae_nunc); "$bin";     mensor_suitae_cursus     "$nomen" "$t0"
#       (MSU_ULTIMA = secunda ultimae mensurae, imprimenda a cursore)
#   ... in fine:
#   mensor_suitae_compilatio_secunda "$nomen" "$s"   (secunda data: cursor parallelus)
#   mensor_suitae_tardissimae 5           # imprimit (semper, etiam sine mensore)
#   mensor_suitae_finire "" "$totae" "$fractae" "$recompilatae"
#       (totum "" = ab initio sessionis computatur)
#
# FORMA EADEM ac compile_tests.sh radicis - arbor: totum > compilatio
# / cursus > probationes singulae; praevolatus = residuum (totum -
# compilatio - cursus: quod nemo nominatim metitur - et quod 2026-09-02
# LXXIII secunda gradus dormientis celavit; residuum crescens = phases
# singulas metire, non coniecta). Tituli PRAEFIXATI ut facies
# (apps/mensor) suitas separet: "" radix, "silva." silva, ...
#
# NUMQUAM suitam frangit: mensor absens aut fractus tacet, omnis
# vocatio '|| true'; MENSOR_TACET=1 metra omnino claudit. Porta:
# ./tools/mensor_suitae_fumus.sh (volumen temporarium).

MSU_RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSU_PRAEFIXUM=""
MSU_MENSOR=""
MSU_INITIUM=""
MSU_CURSUS=""
MSU_COMPILATIO=""
MSU_ULTIMA=""

mensor_suitae_nunc () {
    perl -MTime::HiRes -e 'print Time::HiRes::time'
}

mensor_suitae_incipere () {
    MSU_PRAEFIXUM="${1:-}"
    MSU_INITIUM=$(mensor_suitae_nunc)
    MSU_CURSUS=$(mktemp)
    MSU_COMPILATIO=$(mktemp)
    MSU_MENSOR=""
    if [ -z "${MENSOR_TACET:-}" ] && [ -x "$MSU_RADIX/bin/mensor" ]; then
        if eval "$("$MSU_RADIX/bin/mensor" sessio 2>/dev/null)" 2>/dev/null; then
            MSU_MENSOR="$MSU_RADIX/bin/mensor"
        fi
    fi
    return 0
}

# _mensor_suitae_duratio <t0> -> secunda (in MSU_ULTIMA)
_mensor_suitae_duratio () {
    local t1
    t1=$(mensor_suitae_nunc)
    MSU_ULTIMA=$(echo "$t1 - $1" | bc)
    case "$MSU_ULTIMA" in .*) MSU_ULTIMA="0$MSU_ULTIMA" ;; esac
}

mensor_suitae_compilatio () {
    _mensor_suitae_duratio "$2"
    [ -n "$MSU_COMPILATIO" ] && echo "$MSU_ULTIMA $1" >> "$MSU_COMPILATIO"
    return 0
}

mensor_suitae_cursus () {
    _mensor_suitae_duratio "$2"
    [ -n "$MSU_CURSUS" ] && echo "$MSU_ULTIMA $1" >> "$MSU_CURSUS"
    return 0
}

# formae cum secundis DATIS (cursores paralleli: operarius metitur,
# parens colligit)
mensor_suitae_compilatio_secunda () {   # <nomen> <secunda>
    [ -n "$MSU_COMPILATIO" ] && echo "$2 $1" >> "$MSU_COMPILATIO"
    return 0
}

mensor_suitae_cursus_secunda () {       # <nomen> <secunda>
    [ -n "$MSU_CURSUS" ] && echo "$2 $1" >> "$MSU_CURSUS"
    return 0
}

mensor_suitae_tardissimae () {
    local n="${1:-5}"
    if [ -n "$MSU_CURSUS" ] && [ -s "$MSU_CURSUS" ]; then
        echo "Tardissimae:"
        sort -rn "$MSU_CURSUS" | head -n "$n" | while read -r duratio nomen; do
            printf "  %9ss  %s\n" "$duratio" "$nomen"
        done
    fi
    return 0
}

# _msu_addere <titulus> <valor> <unitas> [parens]
_msu_addere () {
    [ -n "$MSU_MENSOR" ] || return 0
    if [ -n "${4:-}" ]; then
        "$MSU_MENSOR" addere -titulus "$MSU_PRAEFIXUM$1" -valor "$2" \
            -unitas "$3" -parens "$4" >/dev/null 2>&1 || true
    else
        "$MSU_MENSOR" addere -titulus "$MSU_PRAEFIXUM$1" -valor "$2" \
            -unitas "$3" >/dev/null 2>&1 || true
    fi
}

# _msu_nodus <titulus> <valor> <unitas> [parens] -> clavem imprimit
_msu_nodus () {
    [ -n "$MSU_MENSOR" ] || { echo ""; return 0; }
    if [ -n "${4:-}" ]; then
        "$MSU_MENSOR" addere -titulus "$MSU_PRAEFIXUM$1" -valor "$2" \
            -unitas "$3" -parens "$4" -id 2>/dev/null || echo ""
    else
        "$MSU_MENSOR" addere -titulus "$MSU_PRAEFIXUM$1" -valor "$2" \
            -unitas "$3" -id 2>/dev/null || echo ""
    fi
}

# mensor_suitae_finire <totum|""> <totae> <fractae> <recompilatae> [fila] [murus]
# fila: operarii paralleli (2026-09-02) - cum > I 'cursus'/'compilatio'
# summae temporum SIMUL currentium sunt (contentione inflatae), 'totum'
# murus; praevolatus tunc = totum - murus (murus = tempus muri
# fascis probationum, a cursore datum), non totum - summae (quod
# negativum fieret). suita.fila conditur ut historia legibilis maneat.
mensor_suitae_finire () {
    local totum="${1:-}"
    local totae="${2:-0}"
    local fractae="${3:-0}"
    local recompilatae="${4:-0}"
    local fila="${5:-1}"
    local murus="${6:-}"
    local summa_cursus summa_compilationis praevolatus
    local radix nodus_compilationis nodus_cursus

    if [ -z "$totum" ] && [ -n "$MSU_INITIUM" ]; then
        _mensor_suitae_duratio "$MSU_INITIUM"
        totum="$MSU_ULTIMA"
    fi
    if [ -n "$MSU_MENSOR" ]; then
        summa_cursus=$(awk '{s+=$1} END {printf "%.6f", s+0}' \
            "$MSU_CURSUS" 2>/dev/null || echo 0)
        summa_compilationis=$(awk '{s+=$1} END {printf "%.6f", s+0}' \
            "$MSU_COMPILATIO" 2>/dev/null || echo 0)
        if [ "$fila" -gt 1 ] 2>/dev/null && [ -n "$murus" ]; then
            praevolatus=$(echo "$totum - $murus" | bc 2>/dev/null || echo 0)
        else
            praevolatus=$(echo "$totum - $summa_compilationis - $summa_cursus" \
                | bc 2>/dev/null || echo 0)
        fi

        radix=$(_msu_nodus "suita.tempus.totum" "$totum" secunda)
        nodus_compilationis=$(_msu_nodus "suita.tempus.compilatio" \
            "$summa_compilationis" secunda "$radix")
        nodus_cursus=$(_msu_nodus "suita.tempus.cursus" \
            "$summa_cursus" secunda "$radix")
        _msu_addere "suita.tempus.praevolatus" "$praevolatus" secunda "$radix"
        _msu_addere "suita.probationes.totae"   "$totae"   numerus "$radix"
        _msu_addere "suita.probationes.fractae" "$fractae" numerus "$radix"
        _msu_addere "suita.bibliothecae.recompilatae" "$recompilatae" \
            veritas "$radix"
        _msu_addere "suita.fila" "$fila" numerus "$radix"
        "$MSU_MENSOR" addere -tabula "$MSU_CURSUS" \
            -praefixum "${MSU_PRAEFIXUM}probatio.cursus." -unitas secunda \
            -parens "$nodus_cursus" >/dev/null 2>&1 || true
        "$MSU_MENSOR" addere -tabula "$MSU_COMPILATIO" \
            -praefixum "${MSU_PRAEFIXUM}probatio.compilatio." -unitas secunda \
            -parens "$nodus_compilationis" >/dev/null 2>&1 || true
        "$MSU_MENSOR" condere >/dev/null 2>&1 || true
    fi
    rm -f "$MSU_CURSUS" "$MSU_COMPILATIO"
    MSU_CURSUS=""; MSU_COMPILATIO=""
    return 0
}
