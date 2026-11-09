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
# INTERIORA (budget XIII gradus II, 2026-11-09): binarium semper cum
# -Wl,-dead_strip nectitur, et functiones MORTUAE (prototypum in capite
# clausurae, sed in binario nexo absentes: a 'main' non attinguntur aut
# ubique insertae) in aedificatione PROXIMA 'static' fiunt - conflator
# -i 'static ' ante prototypum inserit (silva_conflatio.h). Compilator
# corpora eorum non generat: compilatio -19%, textus -24%; cursus +0,4%
# (praemium 'ultima vocatio statici'; PGO id tollit - worklog).
#   candidata = functiones externae cum prototypo in capite clausurae:
#               sedes prototypi ex 'selecta -sedes' (silva, EXACTA),
#               nomen ex nexu (ordo 'sedes functio' intra declarationem);
#   mortua    = candidata sine symbolo T/t in binario nexo (nm -U),
#               in <bin>.unitas/mortua.txt servata;
#   interiora = sedes candidatorum quorum nomen in mortua.txt PRIORE.
# Tabula STALA semper TUTA: 'static' in unitate una morem non mutat,
# optimizationem solam - functio 'mortua' quae revixit static manet,
# vivit, et aedificatione proxima ex tabula exit. Aedificatio FRIGIDA
# (mortua.txt absens) plana est. Sedes capitis per CONTENTUM servantur
# (md5 capitis et selecta.c in linea prima: tempus plagulae restitutum
# cache non fallit); selecta solum pro capite mutato currit.
# UNITAS_INTERIORA=0: tabula non applicatur (mortua tamen renovantur).
#
# Usus:  source tools/instrumentum_unitas.sh
#        instrumentum_unitatis_nectere <bin> <radix.c> [radix_clientis]...
#            -- <argumenta clang SINE fontibus, SINE -o>
#        (radix.c et radices relativae ad radicem repositorii; radices
#        sine '/' finali, ut RADICES_CLIENTIUM - e.g. silva/fontes)
# Exitus: 0 recens aut nexum | 1 fractum (temporaria deleta)
# INSTRUMENTUM_NECTERE_SOLUM=1: post nexum EXIT 0 (ut instrumentum_nectere)
# Cache: <bin>.unitas/ (partes.tsv, <stirps>.c conflata - lectu utilis;
#        mortua.txt, interiora.tsv applicata, sedes/<via>.tsv per caput)

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
            "$cache" "${radices[@]}"; then
        rm -rf "$sera" "$opus"
        return 1
    fi
    cp "$opus/interiora.tsv" "$cache/interiora.tsv.$$" \
        && mv -f "$cache/interiora.tsv.$$" "$cache/interiora.tsv"
    cp "$opus/partes.tsv" "$cache/partes.tsv.$$" \
        && mv -f "$cache/partes.tsv.$$" "$cache/partes.tsv"
    cp "$opus/$stirps.c" "$cache/$stirps.c.$$" \
        && mv -f "$cache/$stirps.c.$$" "$cache/$stirps.c"
    rm -rf "$sera"

    # compilatio extra seram; e radice repositorii (viae '#line' relativae)
    local temporarium="$bin.nexus.$$"
    # -dead_strip semper (mortua ex binario leguntur); functio static
    # facta et non vocata sub -Werror -Wunused-function caderet
    local -a vexilla_u=("-Wl,-dead_strip")
    if [ -s "$opus/interiora.tsv" ]; then
        vexilla_u+=("-Wno-unused-function")
    fi
    if ! ( cd "$radix_repo" && clang "$@" "${vexilla_u[@]}" \
               "$opus/$stirps.c" -o "$temporarium" ); then
        rm -rf "$opus" "$temporarium" "$temporarium.dSYM"
        return 1
    fi
    # mortua pro aedificatione PROXIMA: candidata sine symbolo textus
    # (T externum, t internum) in binario nexo
    nm -U "$temporarium" 2> /dev/null \
        | awk '$2 == "T" || $2 == "t" { sub(/^_/, "", $3); print $3 }' \
        > "$opus/viva.txt"
    if [ -s "$opus/viva.txt" ] && [ -f "$opus/candidata.tsv" ]; then
        awk -F'\t' 'NR == FNR { viva[$1] = 1; next }
                    !($1 in viva) && !visa[$1]++ { print $1 }' \
            "$opus/viva.txt" "$opus/candidata.tsv" \
            > "$cache/mortua.txt.$$" \
            && mv -f "$cache/mortua.txt.$$" "$cache/mortua.txt"
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
    local radix_repo="$1" radix_c="$2" opus="$3" stirps="$4" cache="$5"
    shift 5
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
        # ordines 'sedes' clausurae SEMEL (grep super nexum totum):
        # tabula staticorum et iunctio interiorum eos legunt
        sedes_clausurae_scribere "$radix_repo" "$opus/viae.txt" \
            > "$opus/sedes_nexus.tsv" \
            || { echo "[unitas] ordines nexus non lecti" >&2; exit 1; }
        {
            echo "# statica (instrumentum_unitas: $radix_c; radices: $*)"
            _symbola_seligere "$@" < "$opus/sedes_nexus.tsv"
        } > "$opus/symbola.tsv" \
            || { echo "[unitas] tabula staticorum fracta" >&2; exit 1; }
        _unitatis_interiora_scribere "$opus" "$cache" \
            || { echo "[unitas] tabula interiorum fracta" >&2; exit 1; }
        local -a vexilla_i=()
        if [ -s "$opus/interiora.tsv" ]; then
            vexilla_i=("-i" "$opus/interiora.tsv")
        fi
        {
            echo "/* $stirps.c (unitas) - GENERATUM (tools/instrumentum_unitas.sh) -"
            echo " * NE MANU EDITES: $radix_c cum clausura sua in plagula una."
            echo " * '#line' plagulam quamque originalem nominat. */"
            echo
        } > "$opus/prooemium.txt"
        build/aedilis/conflator/conflator "${vexilla_r[@]}" \
            ${vexilla_i[@]+"${vexilla_i[@]}"} \
            "$opus/partes.tsv" "$opus/symbola.tsv" "$radix_c" \
            "$opus/prooemium.txt" "$opus/$stirps.c" \
            > "$opus/conflatio.log" 2>&1 \
            || { echo "[unitas] conflator recusavit:" \
                     "$(cat "$opus/conflatio.log")" >&2; exit 1; }
    )
}

# sub sera, e radice repositorii: candidata.tsv ('nomen via linea
# columna' functionum externarum cum prototypo in capite clausurae) et
# interiora.tsv ('via linea columna' eorum quae in mortua.txt sunt) in
# <opus>. Ingressus: <opus>/viae.txt, <opus>/sedes_nexus.tsv.
_unitatis_interiora_scribere () {
    local opus="$1" cache="$2"
    local selector='declaratio:habet(declarator-functionis)'
    local fons_selectae="silva/instrumenta/principalia/selecta.c"
    local -a capita=()
    local -a stala=()
    local caput clavis clavis_selectae k
    : > "$opus/interiora.tsv"
    while IFS= read -r caput; do
        capita+=("$caput")
    done < <(grep '\.h$' "$opus/viae.txt")
    [ "${#capita[@]}" -gt 0 ] || { : > "$opus/candidata.tsv"; return 0; }
    # claves contenti: 'md5 -r' = '<digestum> <via>' per lineam
    md5 -r "$fons_selectae" "${capita[@]}" > "$opus/claves.txt" || return 1
    clavis_selectae="$(awk 'NR == 1 { print $1 }' "$opus/claves.txt")"
    k=0
    while read -r clavis caput; do
        k=$((k + 1))
        [ "$k" -eq 1 ] && continue
        if [ "$(head -1 "$cache/sedes/$caput.tsv" 2> /dev/null)" \
                != "# $clavis $clavis_selectae" ]; then
            stala+=("$clavis $caput")
        fi
    done < "$opus/claves.txt"
    if [ "${#stala[@]}" -gt 0 ]; then
        # selecta structa (nihil currit: INSTRUMENTUM_NECTERE_SOLUM)
        INSTRUMENTUM_NECTERE_SOLUM=1 bash silva/selecta.sh \
            > "$opus/selecta.log" 2>&1 \
            || { echo "[unitas] selecta non structa ($opus/selecta.log)" \
                     >&2; return 1; }
        local par
        for par in "${stala[@]}"; do
            clavis="${par%% *}"
            caput="${par#* }"
            mkdir -p "$cache/sedes/$(dirname "$caput")" || return 1
            (
                # exitus 1 = nulla congruentia (caput sine functionibus);
                # 2 = fractura: caput sine sedibus manet (TUTUM - minus
                # interiorum), sed NON servatur
                {
                    echo "# $clavis $clavis_selectae"
                    silva/build/selecta "$caput" "$selector" -omnia -sedes
                } > "$cache/sedes/$caput.tsv.$$" 2>> "$opus/selecta.err"
                rc=$?
                if [ "$rc" -le 1 ]; then
                    mv -f "$cache/sedes/$caput.tsv.$$" \
                        "$cache/sedes/$caput.tsv"
                else
                    echo "[unitas] selecta -sedes $caput fracta (rc $rc):" \
                        "caput sine interioribus" >&2
                    rm -f "$cache/sedes/$caput.tsv.$$" \
                        "$cache/sedes/$caput.tsv"
                fi
            ) &
        done
        wait
    fi
    for caput in "${capita[@]}"; do
        [ -f "$cache/sedes/$caput.tsv" ] \
            && grep -v '^#' "$cache/sedes/$caput.tsv"
    done > "$opus/sedes_capitum.tsv"
    # iunctio: ordo nexus 'sedes functio' (scopus 0, non staticus) in
    # capite, linea nominis intra [linea, linea_ultima] declarationis ->
    # initium declarationis. Typedef indicis functionis selectori quoque
    # congruit, sed ordinem 'functio' in nexu non habet: excidit.
    awk -F'\t' '
        NR == FNR {
            n[$1]++
            a[$1, n[$1]] = $2; c[$1, n[$1]] = $3; u[$1, n[$1]] = $4
            next
        }
        $2 == "sedes" && $3 == "functio" && $7 == "0" && $8 != "staticus" \
                && ($4 in n) {
            for (k = 1; k <= n[$4]; k++) {
                if ($5 >= a[$4, k] && $5 <= u[$4, k]) {
                    print $1 "\t" $4 "\t" a[$4, k] "\t" c[$4, k]
                    break
                }
            }
        }' "$opus/sedes_capitum.tsv" "$opus/sedes_nexus.tsv" \
        | sort -u > "$opus/candidata.tsv"
    if [ "${UNITAS_INTERIORA:-1}" != 0 ] && [ -s "$cache/mortua.txt" ]; then
        awk -F'\t' 'NR == FNR { mortua[$1] = 1; next }
                    ($1 in mortua) { print $2 "\t" $3 "\t" $4 }' \
            "$cache/mortua.txt" "$opus/candidata.tsv" \
            | sort -u > "$opus/interiora.tsv"
    fi
    return 0
}
