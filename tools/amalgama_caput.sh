#!/bin/bash
# tools/amalgama_caput.sh - caput VERBATIM ex amalgamate extrahere
# (silva-migratio T19a; desideratum ...XHAW3H "amalgama dimidiata")
#
# Usus:
#   ./tools/amalgama_caput.sh <amalgama.c> <caput.h> <exitus_dir>
#       caput intra amalgama.c scriptum in <exitus_dir>/<basis caput.h>
#       extrahit; exitus 0 si caput vivum idem est, 3 si differt
#       (MONITUM in stderr - extractum tamen scriptum)
#   ./tools/amalgama_caput.sh <amalgama.c> <caput.h> -comparare
#       nihil scribit; 0 idem, 3 differt, 1 signa fracta
#
# CUR. Amalgamator caput publicum VERBATIM in .c ponit (inter signum
# "/* ===== <caput.h> (verbatim) ===== */" et "/* ===== ex
# include/latina.h ===== */"). Consumptor qui contra caput VIVUM
# compilat sed silva.o ex .c COMMISSO nectit, ABI falsam aedificat si
# caput manu mutatum est ante regenerationem (T13b: aedilis offsetibus
# falsis lexemata legebat, manifestum corpus unum TACITE amisit).
# Caput ex .c ipso = par semper congruens per constructionem.
#
# Exitus: 0 idem · 3 differt (amalgama dimidiata - curre
# <proiectum>/amalgamare.sh) · 1 signa absentia/fracta · 2 usus.
set -u
if [ $# -ne 3 ]; then
    echo "usus: amalgama_caput.sh <amalgama.c> <caput.h> <exitus_dir|-comparare>" >&2
    exit 2
fi
AMALGAMA="$1"
CAPUT="$2"
EXITUS="$3"
BASIS="$(basename "$CAPUT")"
SIGNUM="/* ================= $BASIS (verbatim) ================= */"
FINIS="/* ================= ex include/latina.h ================= */"

n_signa=$(awk -v S="$SIGNUM" -v F="$FINIS" \
    '$0 == S {s++} $0 == F {f++} END {print s+0, f+0}' "$AMALGAMA")
if [ "$n_signa" != "1 1" ]; then
    echo "FRACTA: $AMALGAMA - signa capitis (verbatim, latina) $n_signa, exspectata 1 1" >&2
    exit 1
fi

# inter signa; linea vacua ultima = '\n' praefixum signi latinae
extractum="$(mktemp)"
trap 'rm -f "$extractum"' EXIT
awk -v S="$SIGNUM" -v F="$FINIS" '
    $0 == S { intus = 1; next }
    $0 == F { exit }
    intus   { lineae[++n] = $0 }
    END     { if (n > 0 && lineae[n] == "") n--
              for (i = 1; i <= n; i++) print lineae[i] }' \
    "$AMALGAMA" > "$extractum"

if [ "$EXITUS" != "-comparare" ]; then
    mkdir -p "$EXITUS"
    # scriptura SOLUM si contentum mutatum (2026-10-02): copia omni
    # vocatione mtime capitis renovabat, et vocantes qui per mtime
    # iudicant (aedilis_struere.sh) semper stalum videbant
    if ! cmp -s "$extractum" "$EXITUS/$BASIS"; then
        cp "$extractum" "$EXITUS/$BASIS"
    fi
fi
if cmp -s "$extractum" "$CAPUT"; then
    exit 0
fi
echo "MONITUM: amalgama dimidiata - $CAPUT differt a capite intra $AMALGAMA" >&2
echo "  (caput manu mutatum ante regenerationem; curre $(dirname "$(dirname "$AMALGAMA")")/amalgamare.sh)" >&2
exit 3
