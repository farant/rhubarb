#!/bin/bash
# tools/statuae_fumus.sh - fumus bin/statuae (statuae-plan T6): imperia
# omnia contra volumen TEMPORARIUM (numquam volumen vivum) et fixa
# probationes/fixa/statuae. Manu curritur post ./tools/statuae_struere.sh.
#
# Usus: ./tools/statuae_fumus.sh     Exitus: 0 omnia bona, 1 fracta.
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"
B=bin/statuae
F=probationes/fixa/statuae
T="$(mktemp -d "${TMPDIR:-/tmp}/statuae_fumus.XXXXXX")"
trap 'rm -rf "$T"' EXIT
V="$T/fumus.volumen"
fracta=0
proba() {  # proba <descriptio> <exitus speratus> <mandatum...>
    local d="$1" sp="$2"; shift 2
    "$@" >"$T/out" 2>"$T/err"; local rc=$?
    if [ "$rc" -ne "$sp" ]; then
        echo "FRACTA: $d (exitus $rc, speratus $sp)"; sed 's/^/    /' "$T/err"
        fracta=$((fracta + 1))
    fi
}
contine() {  # contine <descriptio> <plagula> <textus>
    grep -q -- "$3" "$2" || { echo "FRACTA: $1 ('$3' abest)"; fracta=$((fracta + 1)); }
}
[ -x "$B" ] || { echo "statuae_fumus: $B abest (./tools/statuae_struere.sh)"; exit 1; }

proba "sine argumentis usus" 2 "$B"
proba "addere sine -volumen usus" 2 "$B" addere "$F/lapis.glb"
proba "imperium ignotum usus" 2 "$B" saltare -volumen "$V"
proba "enumerare volumen absens" 1 "$B" enumerare -volumen "$V"
[ ! -e "$V" ] || { echo "FRACTA: enumerare volumen creavit"; fracta=$((fracta + 1)); }

proba "addere lapidem" 0 "$B" addere "$F/lapis.glb" -volumen "$V"
contine "summarium lapidis" "$T/out" "lapis: 54 nodi, 334 trianguli, 2 cycli"
proba "addere fungum titulo" 0 "$B" addere "$F/fungus.glb" -volumen "$V" -titulus "mushroom man"
proba "addere lapidem iterum" 0 "$B" addere "$F/lapis.glb" -volumen "$V"
proba "enumerare" 0 "$B" enumerare -volumen "$V"
[ "$(wc -l < "$T/out" | tr -d ' ')" = 2 ] || { echo "FRACTA: enumerare non II lineae"; fracta=$((fracta + 1)); }
[ "$(sed -n 1p "$T/out" | cut -f1)" = "lapis" ] || { echo "FRACTA: ordo (lapis primus)"; fracta=$((fracta + 1)); }
[ "$(sed -n 2p "$T/out" | cut -f1)" = "mushroom man" ] || { echo "FRACTA: titulus datus"; fracta=$((fracta + 1)); }

head -c 5000 "$F/lapis.glb" > "$T/truncata.glb"
proba "addere truncatam recusat" 1 "$B" addere "$T/truncata.glb" -volumen "$V"
contine "causa truncatae" "$T/err" "cut short"
printf 'non glb' > "$T/textus.glb"
proba "addere non-GLB recusat" 1 "$B" addere "$T/textus.glb" -volumen "$V"
proba "addere plagulam absentem" 1 "$B" addere "$T/nulla.glb" -volumen "$V"
proba "addere titulo malo" 1 "$B" addere "$F/lapis.glb" -volumen "$V" -titulus "a/b"
proba "enumerare post recusationes" 0 "$B" enumerare -volumen "$V"
[ "$(wc -l < "$T/out" | tr -d ' ')" = 2 ] || { echo "FRACTA: recusata volumen tetigit"; fracta=$((fracta + 1)); }

proba "removere" 0 "$B" removere lapis -volumen "$V"
proba "removere ignotum innocuum" 0 "$B" removere nemo -volumen "$V"
proba "enumerare post remotionem" 0 "$B" enumerare -volumen "$V"
[ "$(cut -f1 "$T/out")" = "mushroom man" ] || { echo "FRACTA: remotio"; fracta=$((fracta + 1)); }

# loci (mundus-plan T8): -locus, -loci; latus $models locum non videt
proba "addere locum" 0 "$B" addere "$F/planum.glb" -volumen "$V" -titulus "clearing" -locus
proba "enumerare loci" 0 "$B" enumerare -volumen "$V" -loci
[ "$(cut -f1 "$T/out")" = "clearing" ] || { echo "FRACTA: enumerare -loci"; fracta=$((fracta + 1)); }
proba "enumerare personae sine loco" 0 "$B" enumerare -volumen "$V"
[ "$(cut -f1 "$T/out")" = "mushroom man" ] || { echo "FRACTA: locus inter personas"; fracta=$((fracta + 1)); }
proba "titulus loci ut persona recusatur" 1 "$B" addere "$F/lapis.glb" -volumen "$V" -titulus "clearing"
proba "titulus personae ut locus recusatur" 1 "$B" addere "$F/planum.glb" -volumen "$V" -titulus "mushroom man" -locus
proba "-locus cum enumerare usus" 2 "$B" enumerare -volumen "$V" -locus
proba "-loci cum addere usus" 2 "$B" addere "$F/lapis.glb" -volumen "$V" -loci
proba "removere locum" 0 "$B" removere clearing -volumen "$V"
proba "enumerare loci post remotionem" 0 "$B" enumerare -volumen "$V" -loci
[ ! -s "$T/out" ] || { echo "FRACTA: locus remotus manet"; fracta=$((fracta + 1)); }

for m in crudum atkinson bayer exemplaria nullum; do
    proba "pingere $m" 0 "$B" pingere "$F/lapis.glb" -o "$T/$m.png" -modus "$m" -cyclus walk -tempus 0.35
    file "$T/$m.png" 2>/dev/null | grep -q "360 x 450" || { echo "FRACTA: pingere $m: PNG 360 x 450 abest"; fracta=$((fracta + 1)); }
done
proba "pingere mensura et visus" 0 "$B" pingere "$F/fungus.glb" -o "$T/f.png" -latitudo 100 -altitudo 80 -gyrus -40 -inclinatio 20 -planum -cyclus 0
file "$T/f.png" | grep -q "100 x 80" || { echo "FRACTA: pingere mensura"; fracta=$((fracta + 1)); }
proba "pingere cyclus ignotus" 1 "$B" pingere "$F/lapis.glb" -o "$T/x.png" -cyclus volare
contine "cycli enumerati" "$T/err" "boulder_throw"
proba "pingere modus ignotus" 2 "$B" pingere "$F/lapis.glb" -o "$T/x.png" -modus pulcher
proba "pingere sine -o usus" 2 "$B" pingere "$F/lapis.glb"
proba "pingere numerus malus usus" 2 "$B" pingere "$F/lapis.glb" -o "$T/x.png" -latitudo abc
proba "provenientia" 0 "$B" -provenientia

if [ "$fracta" -eq 0 ]; then echo "STATUAE FUMUS: OMNIA BONA"; exit 0; fi
echo "STATUAE FUMUS: $fracta FRACTA"; exit 1
