#!/bin/bash
# tools/instituere.sh - binarium in ~/.bin (in PATH) instituere
# (fabrica 1b T5: copia actio sua iudicata, 'institutio_X')
#
# rm ante cp: macOS binarium superscriptum SIGKILL dat (signatura per
# vnode condita) - inodus recens obligatorius. Copia fracta exitum 1
# reddit (olim 'cp' sine custodia: exitus 0 et 'institutum' dictum).
# FABRICA_SCRIPTURA (iudex): copia in scripturam, nomen basis idem.
#
# ARBOR SECUNDARIA (decretum Frani 2026-10-24): ex arbore secundaria
# (worktree: '.git' plagula, non directorium) NIHIL in ~/.bin
# instituitur - main solus instituit (bin/fabrica sanare installata).
# Recusatio clara (exitus III), numquam successus tacitus; scripta
# *_struere.sh '-si-licet' dant: tunc institutio omissa nuntiatur et
# exitus 0 (aedificatio ipsa non frangitur). FABRICA_SCRIPTURA (iudex,
# directorium temporarium) ~/.bin non tangit, ergo semper licet.
#
# Usus: ./tools/instituere.sh [-si-licet] bin/X
# Exitus: 0 institutum (aut omissum sub -si-licet) · 1 fractum · 2 usus
#         · 3 arbor secundaria (vetitum).
set -u
SI_LICET=0
if [ "${1:-}" = "-si-licet" ]; then
    SI_LICET=1
    shift
fi
B="${1:?usus: instituere.sh [-si-licet] bin/X}"
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ -f "$B" ] || { echo "instituere: $B abest" >&2; exit 1; }
if [ -z "${FABRICA_SCRIPTURA:-}" ] && [ -f .git ]; then
    if [ "$SI_LICET" = 1 ]; then
        echo "instituere: arbor secundaria - $B NON institutum" \
             "(main solus instituit: bin/fabrica sanare installata)"
        exit 0
    fi
    echo "instituere: VETITUM in arbore secundaria ($PWD): $B non" \
         "institutum - main solus in ~/.bin instituit" \
         "(bin/fabrica sanare installata in main)" >&2
    exit 3
fi
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    DEST="$FABRICA_SCRIPTURA"
else
    DEST="$HOME/.bin"
fi
mkdir -p "$DEST" || exit 1
T="$DEST/$(basename "$B")"
rm -f "$T" || exit 1
cp "$B" "$T" || { echo "instituere: copia fracta: $T" >&2; exit 1; }
echo "institutum: $T"
