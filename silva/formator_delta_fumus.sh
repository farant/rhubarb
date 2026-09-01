#!/bin/bash

# formator_delta_fumus.sh - porta natalis modi -delta/-contra.
#   I   -contra: basis cum divergentia una; opus eandem + novam +
#       sine vetere -> +1 -1, exitus 1; sedes novae impressae
#   II  -contra: opus = basis cum linea addita SUPRA (numeri lineae
#       omnes labuntur) -> +0 -0, exitus 0 (contentum, non numeri)
#   III -delta HEAD super plagulam tractatam intactam -> +0, exitus 0
#   IV  -delta super plagulam extra git (novam) -> basis absens,
#       omnia nova, exitus 1
# Usage: ./silva/formator_delta_fumus.sh      exit 0 sanum | 1 fractum

set -u
RADIX="$(git rev-parse --show-toplevel)" || exit 2
cd "$RADIX" || exit 2
T=build/formator_delta_fumus
mkdir -p "$T"
fracta=0

# basis: 'i32  k;' (spatia duo = columnae-binae) + linea longa
printf '#include "latina.h"\n\ninteger\nprincipale (vacuum)\n{\n    i32  k;\n\n    k = ZEPHYRUM;\n    redde k;\n}\n' > "$T/basis.c"
# opus: divergentia vetus manet, nova additur (linea nimis longa), nihil evanescit... et II: vetus evanescit
printf '#include "latina.h"\n\n/* commentarium valde longum quod limitem septuaginta duorum octetorum certe excedit */\ninteger\nprincipale (vacuum)\n{\n    i32 k;\n\n    k = ZEPHYRUM;\n    redde k;\n}\n' > "$T/opus.c"
out=$(./silva/formator.sh "$T/opus.c" -contra "$T/basis.c" 2>/dev/null); rc=$?
if [ "$rc" -eq 1 ] && printf '%s\n' "$out" | grep -q '+1 novae, -1 evanidae' && printf '%s\n' "$out" | grep -q 'longitudo-lxxii'; then echo "  I   -contra +1 -1 (sedes nominata)   OK"; else echo "  I   FRACTUM (rc=$rc)"; printf '%s\n' "$out"; fracta=1; fi

{ echo '/* linea addita supra - numeri omnes labuntur */'; cat "$T/basis.c"; } > "$T/lapsum.c"
out=$(./silva/formator.sh "$T/lapsum.c" -contra "$T/basis.c" 2>/dev/null); rc=$?
if [ "$rc" -eq 0 ] && printf '%s\n' "$out" | grep -q '+0 novae, -0 evanidae'; then echo "  II  -contra numeri lapsi -> +0 -0     OK"; else echo "  II  FRACTUM (rc=$rc)"; printf '%s\n' "$out"; fracta=1; fi

out=$(./silva/formator.sh lib/piscina.c -delta HEAD 2>/dev/null); rc=$?
if [ "$rc" -eq 0 ] && printf '%s\n' "$out" | grep -q '+0 novae'; then echo "  III -delta HEAD intacta -> +0         OK"; else echo "  III FRACTUM (rc=$rc)"; printf '%s\n' "$out"; fracta=1; fi

out=$(./silva/formator.sh "$T/opus.c" -delta HEAD 2>/dev/null); rc=$?
if [ "$rc" -eq 1 ] && printf '%s\n' "$out" | grep -q 'basis absens'; then echo "  IV  -delta plagula nova -> omnia nova OK"; else echo "  IV  FRACTUM (rc=$rc)"; printf '%s\n' "$out"; fracta=1; fi

[ "$fracta" -ne 0 ] && { echo "fumus delta: FRACTUM"; exit 1; }
echo "fumus delta: sanum (IV/IV)"; exit 0
