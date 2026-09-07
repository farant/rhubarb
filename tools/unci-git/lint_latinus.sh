#!/bin/bash
# lint_latinus.sh - LINT LATINUS identificatorum ad commissionem
# (functio communis unci pre-commit et pre-merge-commit; decisio
# Frani 2026-09-07: OBSTAT, non monet).
#
# CUR: porta orationis (probatio_oratio_vocabula) verba identificatorum
# ignota contra copiam toleratam iudicat, sed solae sessiones quae
# portam 'oratio' nominant eam currunt - verba pictoris XXVIII et
# fixtura briar main intraverunt iniudicata, una fusione post inventa.
# Commissio est sedes quam nulla via editionis praeterit (ut examen).
#
# QUID: plagulae .c/.h/.m in indice (ACMR, exclusis scratchpad/build/
# fixa/amalgama/archivum/knotapel/vendor) -> index symbolorum sanatus
# (./silva/nexus.sh -renovare: incrementalis ~I s; PLENUS ~LXX s si
# instrumentum novum aut tabula absens - nuntiatur) -> ./oratio/
# vocabula.sh -nova (relatio: verbum, identificator, sedes, EXITUS
# tres, exemplaria glossarii parabilia):
#   0 nihil novi        -> tacet (linea una)
#   1 verba nova        -> OBSTAT (exit 1), relatio impressa
#   2 apparatus fractus -> OBSTAT (porta fracta clamat, numquam tacet)
# Nulla plagula = 'nihil iudicatum' (silentium ambiguum vetitum).
# Viae git-TRACTATAE solae numerantur (opus alienae sessionis non
# commissum pinnam non ferit); index nexus ARBOREM legit, ergo arbor
# iudicatur ut in examine.
#
# Evasio consulta (causa in nuntio commissionis): git commit --no-verify
# aut git merge --no-verify. Porta natalis: fumus.sh gradus IX-XI.
# UNCUS_LINT_VIAE="a.c" = viae explicitae (probationi).

lint_latinus () {
    local viae out rc
    if [ -n "${UNCUS_LINT_VIAE:-}" ]; then
        viae="$UNCUS_LINT_VIAE"
    else
        viae="$(git diff --cached --name-only --diff-filter=ACMR -- '*.c' '*.h' '*.m' 2>/dev/null \
            | grep -vE '(^|/)(scratchpad|build|fixa|amalgama|archivum|knotapel|vendor)/')"
    fi
    if [ -z "$viae" ]; then
        echo "lint latinus (uncus): nulla plagula .c/.h in commissione - nihil iudicatum" >&2
        return 0
    fi
    mkdir -p build
    echo "lint latinus (uncus): index symbolorum sanatur (incrementalis ~I s; plenus ~LXX s si instrumentum novum aut tabula absens)" >&2
    if ! ./silva/nexus.sh -renovare > build/uncus_nexus.log 2>&1; then
        echo "lint latinus (uncus): index nexus NON renovatus (build/uncus_nexus.log) - APPARATUS FRACTUS, porta clamat, obstat" >&2
        return 1
    fi
    if grep -q 'plenus' build/uncus_nexus.log 2>/dev/null; then
        echo "  (percursus PLENUS cucurrit: instrumentum novum aut tabula absens)" >&2
    fi
    out=$(./oratio/vocabula.sh -nova 2>&1); rc=$?
    case "$rc" in
        0)
            echo "lint latinus (uncus): nihil novi - $(printf '%s\n' "$out" | tail -1 | sed 's/^ *//')" >&2
            return 0 ;;
        1)
            {
                echo "LINT LATINUS (uncus): verba identificatorum IGNOTA NOVA - COMMISSIO OBSTATA"
                printf '%s\n' "$out"
                echo "  Evasio consulta (causa in nuntio commissionis): git commit --no-verify / git merge --no-verify"
            } >&2
            return 1 ;;
        *)
            {
                echo "lint latinus (uncus): APPARATUS FRACTUS (vocabula.sh exit $rc) - porta clamat, obstat:"
                printf '%s\n' "$out" | tail -6 | sed 's/^/  /'
            } >&2
            return 1 ;;
    esac
}
