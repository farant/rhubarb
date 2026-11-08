#!/bin/bash
# tools/instrumentum_unitas.sh - instrumentum ex PLAGULA UNA (aedificatio
# unitatis): clausura vera radicis (bin/aedilis --partes) per conflatorem
# (silva/instrumenta/silva_conflatio) in plagulam unam conflatur et ut
# unitas translationis una compilatur. Compilator functiones parvas
# trans plagulas (xar_obtinere, tabula, piscina) inserere potest -
# percursus -semantica 337,1 -> 290,3 G (budget X, 2026-11-07), relatio
# eadem. '#line' plagulam quamque nominat: errores compilatoris, lineae
# depuratoris et 'sample' plagulas originales monstrant.
#
# Frater tools/instrumentum_nectere.sh, EADEM pacta (porta
# nexus-instrumentorum):
#   - stalum = binarium absens, aut ingressus quivis STRICTE recentior
#     (find -newer): plagulae clausurae (ex partes.tsv servato - inclusio
#     nova plagulam includentem mutat, ergo clausura renovatur), vexilla,
#     involucrum vocans, hoc scriptum, conflator;
#   - binarium in '<bin>.nexus.<pid>' deinde 'mv -f' (inodus novus);
#   - vocationes concurrentes: conflatio sub sera (mkdir) - nexus silvae
#     et tabula staticorum non concurrunt; compilatio extra seram, quaeque
#     in temporario suo.
#
# Tabula staticorum = statica scopi plagulae ex nexu silvae (build/
# nexus.tsv, symbola_tabulam_scribere in tools/corpus_infixum.sh) cum
# radicibus vocantis - RECENS necessaria: staticum non renominatum quod
# collidit saepe errorem dat, sed 'static int x;' bis = definitiones
# tentativae TACITE coniunctae, et macra plagulae sine #undef sequentes
# mutant.
#
# Usus:  source tools/instrumentum_unitas.sh
#        instrumentum_unitatis_nectere <bin> <radix.c> [radix_clientis]...
#            -- <argumenta clang SINE fontibus, SINE -o>
#        (radix.c et radices relativae ad radicem repositorii; radices
#        sine '/' finali, ut RADICES_CLIENTIUM - e.g. silva/fontes)
# Exitus: 0 recens aut nexum | 1 fractum (temporaria deleta)
# INSTRUMENTUM_NECTERE_SOLUM=1: post nexum EXIT 0 (ut instrumentum_nectere)
# Cache: <bin>.unitas/ (partes.tsv, <stirps>.c conflata - lectu utilis)

instrumentum_unitatis_nectere () {
    local bin="$1"
    local radix_c="$2"
    shift 2
    local -a radices=()
    while [ $# -gt 0 ] && [ "$1" != "--" ]; do
        radices+=("$1")
        shift
    done
    [ "${1:-}" = "--" ] && shift
    local instrumenta radix_repo cache stirps stalum=0
    instrumenta="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    radix_repo="$(cd "$instrumenta/.." && pwd)"
    cache="$bin.unitas"
    stirps="$(basename "$radix_c" .c)"
    local -a ingressus=(
        "$instrumenta/vexilla.sh"
        "$instrumenta/instrumentum_unitas.sh"
        "${BASH_SOURCE[1]}"
        "$radix_repo/$radix_c"
        "$radix_repo/silva/instrumenta/silva_conflatio.c"
        "$radix_repo/silva/instrumenta/silva_conflatio.h"
        "$radix_repo/silva/instrumenta/principalia/conflator.c"
    )
    if [ ! -f "$bin" ] || [ ! -f "$cache/partes.tsv" ]; then
        stalum=1
    else
        local via
        while IFS= read -r via; do
            ingressus+=("$radix_repo/$via")
        done < <(awk -F'\t' '$1=="C" || $1=="O" { print $2 }' \
                     "$cache/partes.tsv")
        if [ -n "$(find "${ingressus[@]}" -maxdepth 0 -newer "$bin" \
                      -print -quit 2> /dev/null)" ]; then
            stalum=1
        fi
    fi
    if [ "$stalum" -eq 0 ]; then
        [ -n "${INSTRUMENTUM_NECTERE_SOLUM:-}" ] && exit 0
        return 0
    fi

    local opus="$cache/opus.$$"
    local sera="$cache/sera"
    local mora=0
    mkdir -p "$cache" "$opus" || return 1
    # sera: mkdir atomicum; possessor mortuus (pid in sera absens) frangitur
    until mkdir "$sera" 2> /dev/null; do
        local possessor
        possessor="$(cat "$sera/pid" 2> /dev/null)"
        if [ -n "$possessor" ] && ! kill -0 "$possessor" 2> /dev/null; then
            rm -rf "$sera"
            continue
        fi
        mora=$((mora + 1))
        if [ "$mora" -gt 3000 ]; then   # ~X minuta
            echo "[unitas] sera $sera non soluta" >&2
            rm -rf "$opus"
            return 1
        fi
        sleep 0.2
    done
    echo $$ > "$sera/pid"
    if ! _unitatem_conflare "$radix_repo" "$radix_c" "$opus" "$stirps" \
            "${radices[@]}"; then
        rm -rf "$sera" "$opus"
        return 1
    fi
    cp "$opus/partes.tsv" "$cache/partes.tsv.$$" \
        && mv -f "$cache/partes.tsv.$$" "$cache/partes.tsv"
    cp "$opus/$stirps.c" "$cache/$stirps.c.$$" \
        && mv -f "$cache/$stirps.c.$$" "$cache/$stirps.c"
    rm -rf "$sera"

    # compilatio extra seram; e radice repositorii (viae '#line' relativae)
    local temporarium="$bin.nexus.$$"
    if ! ( cd "$radix_repo" && clang "$@" "$opus/$stirps.c" \
               -o "$temporarium" ); then
        rm -rf "$opus" "$temporarium" "$temporarium.dSYM"
        return 1
    fi
    rm -rf "$opus"
    mv -f "$temporarium" "$bin" || return 1
    # clang -g compilans et nectens '<exitus>.dSYM' iuxta temporarium
    # scribit: symbola cum binario migrant (ut instrumentum_nectere)
    if [ -d "$temporarium.dSYM" ]; then
        rm -rf "$bin.dSYM"
        mv -f "$temporarium.dSYM" "$bin.dSYM" 2> /dev/null \
            || rm -rf "$temporarium.dSYM"
    fi
    [ -n "${INSTRUMENTUM_NECTERE_SOLUM:-}" ] && exit 0
    return 0
}

# sub sera: clausura, conflator, nexus, tabula staticorum, conflatio
_unitatem_conflare () {
    local radix_repo="$1" radix_c="$2" opus="$3" stirps="$4"
    shift 4
    local -a vexilla_r=()
    local r
    for r in "$@"; do
        vexilla_r+=("-r" "$r/")
    done
    (
        cd "$radix_repo" || exit 1
        ./bin/aedilis "$radix_c" --partes > "$opus/partes.tsv" \
            2> "$opus/partes.err" \
            || { echo "[unitas] aedilis clausuram $radix_c non dedit:" \
                     "$(head -3 "$opus/partes.err")" >&2; exit 1; }
        ./bin/aedilis silva/instrumenta/principalia/conflator.c \
            > "$opus/conflator.log" 2>&1 \
            && bash build/aedilis/conflator/struere.sh \
                >> "$opus/conflator.log" 2>&1 \
            || { echo "[unitas] conflator non structus" \
                     "($opus/conflator.log)" >&2; exit 1; }
        awk -F'\t' '$1=="C" || $1=="O" { print $2 }' "$opus/partes.tsv" \
            > "$opus/viae.txt"
        # nexus renovatur solum si plagula clausurae eo STRICTE recentior
        # (aut deest): tabula ordines clausurae SOLAE legit, ergo nexus
        # vetustior quam nulla plagula clausurae eas recentes tenet
        # (renovatio etiam sine mutatione ~1,5 s). Cautio: plagula cum
        # tempore VETERE restituta (cp -p, silva.planta) post nexum
        # renovatum hic non videtur - contentum restitutum tunc idem est
        # ac ante plantam, nexus tamen plantam tenet.
        local -a viae_clausurae=()
        local via_c
        while IFS= read -r via_c; do
            viae_clausurae+=("$via_c")
        done < "$opus/viae.txt"
        if [ ! -f build/nexus.tsv ] \
                || [ -n "$(find "${viae_clausurae[@]}" -maxdepth 0 \
                            -newer build/nexus.tsv -print -quit \
                            2> /dev/null)" ]; then
            ./silva/nexus.sh -renovare > "$opus/nexus.log" 2>&1 \
                || { echo "[unitas] nexus silvae non renovatus" >&2; exit 1; }
        fi
        # shellcheck source=/dev/null
        source tools/corpus_infixum.sh
        {
            echo "# statica (instrumentum_unitas: $radix_c; radices: $*)"
            symbola_clausurae_scribere "$radix_repo" "$opus/viae.txt" "$@"
        } > "$opus/symbola.tsv" \
            || { echo "[unitas] tabula staticorum fracta" >&2; exit 1; }
        {
            echo "/* $stirps.c (unitas) - GENERATUM (tools/instrumentum_unitas.sh) -"
            echo " * NE MANU EDITES: $radix_c cum clausura sua in plagula una."
            echo " * '#line' plagulam quamque originalem nominat. */"
            echo
        } > "$opus/prooemium.txt"
        build/aedilis/conflator/conflator "${vexilla_r[@]}" \
            "$opus/partes.tsv" "$opus/symbola.tsv" "$radix_c" \
            "$opus/prooemium.txt" "$opus/$stirps.c" \
            > "$opus/conflatio.log" 2>&1 \
            || { echo "[unitas] conflator recusavit:" \
                     "$(cat "$opus/conflatio.log")" >&2; exit 1; }
    )
}
