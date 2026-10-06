#!/bin/bash
# tools/formare_viae.sh - SCRIPTURA AUTOMATICA formae plagularum C
# commissionis: UNA sedes regularum, duo vocantes.
#
# CUR (parcum …QGT1, 2026-10-02): uncus prae-commissionis plagulas .c
# formabat POST iudicium fabricae et portas silva.commissio - artificium
# generatum ex plagula ita reformata (amalgama silvae ex lib/xar.c,
# e5112e37) STALUM committebatur, nemine vidente. Nunc commissio hoc
# scriptum ANTE lint, fabricam, portas vocat (arbor operis): octeti
# iudicati = octeti commissi, et uncus postea nihil mutat.
#
# Usus:
#   ./tools/formare_viae.sh [-index] via...
#     sine -index: plagulas ARBORIS OPERIS format (silva.commissio)
#     -index:      plagulas INDICIS (uncus pre-commit): commissio
#                  partialis (arbor ab indice differt) non formatur, et
#                  plagula formata in indicem reponitur (git add)
#
# Regulae (olim in tools/unci-git/pre-commit solo):
#   - solae *.c et *.h; directoria scratchpad build fixa amalgama
#     archivum oracula knotapel numquam (oracula/: glutinum C99 circa
#     oracula aliena - formator C89 id non legit; dispositio D0;
#     knotapel/ exclusa tota, decisio Frani 2026-10-03: suae regulae
#     ibi, knotapel/CLAUDE.md)
#   - plagula GENERATA (GENERATUM in linea prima) numquam: veritas eius
#     generator est (fabrica P2, via B, 2026-09-29)
#   - vendor/: terra non evoluta - intra lineas mutatas solas
#     (formator -lineae); ceterae: scriptura TOTA (evolutio 2026-09-01)
# Exitus semper 0: forma monet, non obstat (examen et lint obstant).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 0

INDEX=0
if [ "${1:-}" = "-index" ]; then
    INDEX=1
    shift
fi

for via in "$@"; do
    case "$via" in
        *.c|*.h) ;;
        *) continue ;;
    esac
    if printf '%s\n' "$via" \
            | grep -qE '(^|/)(scratchpad|build|fixa|amalgama|archivum|oracula|knotapel)/'; then
        continue
    fi
    [ -f "$via" ] || continue
    if [ "$INDEX" -eq 1 ]; then
        linea_prima="$(git show ":$via" 2>/dev/null | head -n 1)"
    else
        linea_prima="$(head -n 1 "$via")"
    fi
    if printf '%s\n' "$linea_prima" | grep -qw 'GENERATUM'; then
        echo "formator (commissio): $via GENERATUM - non formata (octeti generatoris)" >&2
        continue
    fi
    if [ "$INDEX" -eq 1 ] && [ -n "$(git diff --name-only -- "$via" 2>/dev/null)" ]; then
        echo "formator (commissio): $via NON formata - arbor operis ab indice differt (commissio partialis)" >&2
        continue
    fi
    case "$via" in
        vendor/*)
            # lineae mutatae: contra indicem (uncus) aut contra HEAD
            # (arbor); plagula nova non tracta = tota
            if [ "$INDEX" -eq 1 ]; then
                differentia="$(git diff --cached -U0 -- "$via" 2>/dev/null)"
            elif git ls-files --error-unmatch -- "$via" > /dev/null 2>&1; then
                differentia="$(git diff -U0 HEAD -- "$via" 2>/dev/null)"
            else
                differentia="@@ -0,0 +1,$(wc -l < "$via" | tr -d ' ') @@"
            fi
            rangae=$(printf '%s\n' "$differentia" | awk '/^@@/ {
                split($3, p, /[+,]/); a = p[2]; n = (p[3] == "" ? 1 : p[3]);
                if (n > 0) printf "-lineae %d-%d ", a, a + n - 1 }')
            [ -z "$rangae" ] && continue
            # shellcheck disable=SC2086
            fout=$(./silva/formator.sh "$via" -scribere $rangae 2>&1 >/dev/null)
            modus="intra functiones mutatas" ;;
        *)
            fout=$(./silva/formator.sh "$via" -scribere 2>&1 >/dev/null)
            modus="tota" ;;
    esac
    if printf '%s\n' "$fout" | grep -q 'formator: scriptum'; then
        if [ "$INDEX" -eq 1 ]; then
            git add -- "$via"
            echo "formator (commissio): $via FORMATA ($modus), index repositus" >&2
        else
            echo "formator (commissio): $via FORMATA ($modus) ante iudicium" >&2
        fi
    fi
    printf '%s\n' "$fout" | grep -E 'extra extenta|recusat|ignota|maxime|malformata|fracta' | sed 's/^/  /' >&2 || true
done
exit 0
