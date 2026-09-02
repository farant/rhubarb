#!/bin/bash
# differre_fumus.sh - porta natalis modi -verdictum.
#   I   spatia sola      -> VERDICTUM cosmetica, exitus 0
#   II  commentarium     -> documentaria: exitus 1 (gradus cosmetica),
#                           0 (gradus documentaria)
#   III lexema mutatum   -> substantiva, exitus 1 (utroque gradu)
#   IV  idem             -> cosmetica (nulla differentia), exitus 0
#   V   functio addita   -> substantiva (ADDITA), exitus 1
# Usage: ./silva/differre_fumus.sh     exit 0 sanum | 1 fractum
set -u
RADIX="$(git rev-parse --show-toplevel)" || exit 2
cd "$RADIX" || exit 2
T=build/differre_fumus; mkdir -p "$T"; fracta=0
printf '/* f */\nvacuum\nf (vacuum)\n{\n    i32 x;\n\n    x  = I;\n    redde;\n}\n' > "$T/a.c"
sed 's/x  = I;/x = I;/' "$T/a.c" > "$T/cos.c"
sed 's|/\* f \*/|/* f - mutatum */|' "$T/a.c" > "$T/doc.c"
sed 's/x  = I;/x = II;/' "$T/a.c" > "$T/sub.c"
{ cat "$T/a.c"; printf '\nvacuum\ng (vacuum)\n{\n    redde;\n}\n'; } > "$T/add.c"
casus() { # nomen vetus novum gradus exitus_exspectatus verd_exspectatum
    out=$(./silva/differre.sh "$2" "$3" -machina -verdictum $4 2>/dev/null); rc=$?
    verd=$(printf '%s\n' "$out" | awk -F'\t' '$1=="VERDICTUM"{print $2}')
    if [ "$rc" -eq "$5" ] && [ "$verd" = "$6" ]; then echo "  $1  OK ($verd, rc=$rc)"; else echo "  $1  FRACTUM (rc=$rc verd=$verd; exspectata rc=$5 $6)"; printf '%s\n' "$out" | head -5; fracta=1; fi
}
casus "I   spatia sola          " "$T/a.c" "$T/cos.c" cosmetica 0 cosmetica
casus "II.a commentarium/cosm.  " "$T/a.c" "$T/doc.c" cosmetica 1 documentaria
casus "II.b commentarium/doc.   " "$T/a.c" "$T/doc.c" documentaria 0 documentaria
casus "III lexema mutatum       " "$T/a.c" "$T/sub.c" documentaria 1 substantiva
casus "IV  idem                 " "$T/a.c" "$T/a.c" cosmetica 0 cosmetica
casus "V   functio addita       " "$T/a.c" "$T/add.c" documentaria 1 substantiva
[ "$fracta" -ne 0 ] && { echo "fumus differre: FRACTUM"; exit 1; }
echo "fumus differre: sanum (VI/VI)"; exit 0
