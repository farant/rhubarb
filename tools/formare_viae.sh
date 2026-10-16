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
# QUAE plagulae: iudex formator_commissionis (unitates.stml; filtrum
# tools/iudex_unitatum.sh; spec unitates-codicis U4) - domestica, non
# archivata, fontes/capita/probationes, MANU scripta (generatum numquam:
# veritas eius generator est, fabrica P2), lingua c89 (.c .h). Exclusa
# per proprietates, non per nomina directoriorum: knotapel remissum
# (Franus 2026-10-03), vendor oracula aliena (vendor olim lineis mutatis
# formabatur - Franus 2026-10-15: numquam), archivum archivatum, fixa
# fixturae, amalgama productum (forma eius gradus generationis esset).
# vendor/ etiam continet arbores upstream octetim idem (vendor/bearssl)
# et amalgamas GENERATAS (tls-bearssl R5, 2026-10-15) - cura aliena.
# Causa cur via non formatur: bin/fabrica quid -iudex
# formator_commissionis VIA. Scriptura semper TOTA.
# Exitus semper 0: forma monet, non obstat (examen et lint obstant) -
# iudex fractus tamen CLAMAT (nulla forma scripta, causa nominata).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 0

INDEX=0
if [ "${1:-}" = "-index" ]; then
    INDEX=1
    shift
fi

# solae .c/.h rogantur (ceterae nihil ad formatorem); provenientia ex
# arbore operis legitur: in modo -index plagulae cum arbore ab indice
# differente infra omittuntur, ergo arbor = index. Omissae causam suam
# nominant (stderr, '-causae')
candidatae=()
for via in "$@"; do
    case "$via" in *.c|*.h) [ -f "$via" ] && candidatae+=("$via") ;; esac
done
[ "${#candidatae[@]}" -gt 0 ] || exit 0
if ! iudicatae="$(printf '%s\n' "${candidatae[@]}" \
        | ./tools/iudex_unitatum.sh -causae formator_commissionis 2> >(sed 's/^/formator (commissio): /' >&2))"; then
    echo "formator (commissio): iudex formator_commissionis NON respondit (tools/iudex_unitatum.sh) - NULLA forma scripta" >&2
    exit 0
fi
while IFS= read -r via; do
    [ -n "$via" ] && [ -f "$via" ] || continue
    if [ "$INDEX" -eq 1 ] && [ -n "$(git diff --name-only -- "$via" 2>/dev/null)" ]; then
        echo "formator (commissio): $via NON formata - arbor operis ab indice differt (commissio partialis)" >&2
        continue
    fi
    fout=$(./silva/formator.sh "$via" -scribere 2>&1 >/dev/null)
    if printf '%s\n' "$fout" | grep -q 'formator: scriptum'; then
        if [ "$INDEX" -eq 1 ]; then
            git add -- "$via"
            echo "formator (commissio): $via FORMATA, index repositus" >&2
        else
            echo "formator (commissio): $via FORMATA ante iudicium" >&2
        fi
    fi
    printf '%s\n' "$fout" | grep -E 'extra extenta|recusat|ignota|maxime|malformata|fracta' | sed 's/^/  /' >&2 || true
done <<< "$iudicatae"
exit 0
