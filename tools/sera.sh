#!/bin/bash
# tools/sera.sh - SERA cursorum, FONTANDA a cursoribus probationum et
# involucris quae in directorium aedificationis suitae scribunt.
#
# CUR (2026-09-07): regula 'cursores eiusdem suitae numquam simul'
# in capite sessionis vivebat, non in instrumento: cursus duo eiusdem
# suitae (sessiones duae in arbore una, uncus commissionis dum porta
# currit, involucrum quod obiecta struit) obiecta eadem scriberent et
# binarium currens obtererent (SIGKILL 137). silva/nexus.sh seram
# suam hodie mane accepit; haec est lex UNA pro omnibus (nexus.sh eam
# quoque fontat).
#
# usus (in cursore, post BUILD_DIR cognitum):
#   source "$RADIX_DIR/tools/sera.sh"
#   sera_capere "$BUILD_DIR/cursor.sera" || exit 2     # 2 = NIHIL cucurrit
#   ... (sera_dimittere in exitu sponte per trap EXIT)
#
# LEX: sera = directorium (mkdir atomicum) cum 'radix' et 'pid'.
#   tenta       -> exspectatio (linea una in stderr), usque ad tectum
#                  (argumentum II, aliter SERA_TECTUM, aliter DC s),
#                  deinde 1 cum nuntio qui tenentem nominat - numquam
#                  tacite, numquam furtim.
#   VETUS       -> derelicta et capta: tenens mortuus (ps -p), aut
#                  radix aliena (sera in clonem umbrae copiata - tenens
#                  in arbore alia currit), aut aetas > LX min (tenens
#                  pendens), aut sine pid post > I min (reliquia).
#   REENTRANTIA -> SERA_TENTA (ambitus) = via serae quam avus tenet:
#                  filius eiusdem serae nihil facit nec dimittit
#                  (involucrum quod cursorem vocat; pythonica quae
#                  cursorem vocat: silva.sera exportat idem).
#   via sine suffixo '.sera' refutatur (rm -rf custoditum).
# Speculum Pythonicum: pythonica/silva.py class Sera (lex eadem, porta
# eadem probat utrumque contra alterum). Porta natalis:
# tools/sera_fumus.sh.

SERA_VIA=""
SERA_RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

sera_tenens () {   # <via> -> pid tenentis (aut '?')
    local pid
    pid=$(cat "$1/pid" 2>/dev/null)
    echo "${pid:-?}"
}

sera_vetus () {   # <via> -> 0 si derelinquenda
    local pid radix
    pid=$(cat "$1/pid" 2>/dev/null)
    radix=$(cat "$1/radix" 2>/dev/null)
    if [ -z "$pid" ]; then
        # mkdir factum, pid nondum scriptum (fenestra brevis) aut
        # reliquia sine pid: aetas iudicat
        [ -n "$(find "$1" -maxdepth 0 -mmin +1 2>/dev/null)" ] && return 0
        return 1
    fi
    [ "$radix" != "$SERA_RADIX" ] && return 0
    ps -p "$pid" > /dev/null 2>&1 || return 0
    [ -n "$(find "$1" -maxdepth 0 -mmin +60 2>/dev/null)" ] && return 0
    return 1
}

sera_capere () {   # <via> [tectum_s] -> 0 capta | 1 non capta
    local via="$1" tectum="${2:-${SERA_TECTUM:-600}}" exspectatum=0
    case "$via" in
        *.sera) ;;
        *) echo "sera: via sine suffixo .sera refutata: $via" >&2; return 1 ;;
    esac
    if [ -n "${SERA_TENTA:-}" ] && [ "$SERA_TENTA" = "$via" ]; then
        return 0     # avus tenet - reentrantia
    fi
    mkdir -p "$(dirname "$via")" 2>/dev/null
    while ! mkdir "$via" 2>/dev/null; do
        if sera_vetus "$via"; then
            echo "sera: $via vetus (pid $(sera_tenens "$via")) - derelicta" >&2
            rm -rf "$via"
            continue
        fi
        if [ "$exspectatum" -eq 0 ]; then
            echo "sera: $via tenetur (pid $(sera_tenens "$via")) - exspecto usque ad ${tectum}s" >&2
        fi
        if [ "$exspectatum" -ge "$tectum" ]; then
            echo "sera: $via tenetur post ${tectum}s (pid $(sera_tenens "$via")) - NIHIL cursum" >&2
            return 1
        fi
        sleep 1
        exspectatum=$((exspectatum + 1))
    done
    # radix ANTE pid: sera cum radice sed sine pid = fenestra scripturae
    # (tenta), sera cum pid sed sine radice numquam exsistit
    echo "$SERA_RADIX" > "$via/radix"
    echo "$$" > "$via/pid"
    SERA_VIA="$via"
    export SERA_TENTA="$via"
    trap sera_dimittere EXIT
    return 0
}

sera_dimittere () {
    [ -n "$SERA_VIA" ] || return 0
    if [ "$(cat "$SERA_VIA/pid" 2>/dev/null)" = "$$" ]; then
        rm -rf "$SERA_VIA"
    fi
    SERA_VIA=""
    unset SERA_TENTA
    return 0
}
