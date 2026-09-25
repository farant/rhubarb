#!/bin/bash
# materia/oraculum_silvae_struere.sh - oraculum_silvae bis aedificare:
# silva AD PIGNUS (ex amalgamate commissi) et silva VIVA
#
# Usage:
#   ./materia/oraculum_silvae_struere.sh        # ambo binaria
#
# Effectus:
#   build/oraculum_silvae/<PIGNUS>/oraculum   silva ad pignus
#   build/oraculum_silvae/vivum               silva viva (silva/build)
# Contractus exitus: 0 ambo aedificata · 1 compilatio/nexus fractus aut
# ISOLATIO violata · 2 NIHIL (obiecta silvae absunt, git fractus).
#
# PIGNUS = COMMISSUM quod oraculum EST (silva-migratio-plan T2). Omne
# commissum ante T6 silvam eandem fert; post T6 pignus NUMQUAM
# progreditur - motus eius commissio est quae causam dicit. Amalgama
# non exscribitur in arborem (XC M linearum): git show id ad pignus
# extrahit in build/, semel per pignus.
#
# ISOLATIO (Review focus 1): binarium pignoris NIHIL nectit praeter
# obiectum instrumenti et obiectum amalgamatis - linea nexus infra id
# structura praestat. Et symbola quae instrumentum poscit aut ab
# amalgamate definiuntur aut in indice libc infra stant; aliud
# quodlibet = functio domus per errorem vocata -> exitus I.
#
# Binarium VIVUM obiecta silva/build nectit (shim olim idem) -
# ea ./silva/compile_probationes.sh struit; hic non struuntur.

set -u
PIGNUS=7a4847b0
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BASIS="$RADIX/build/oraculum_silvae"
SEDES="$BASIS/$PIGNUS"
FONS="$RADIX/materia/instrumenta/oraculum_silvae.c"
source "$RADIX/tools/vexilla.sh"
source "$RADIX/tools/sera.sh"
mkdir -p "$SEDES"
sera_capere "$BASIS/cursor.sera" || exit 2

# ---- 1. amalgama ad pignus (semel per pignus) ----
if [ ! -f "$SEDES/silva.c" ] || [ ! -f "$SEDES/silva.h" ]; then
    git -C "$RADIX" show "$PIGNUS:silva/amalgama/silva.c" > "$SEDES/silva.c.tmp" \
      && git -C "$RADIX" show "$PIGNUS:silva/amalgama/silva.h" > "$SEDES/silva.h" \
      && mv "$SEDES/silva.c.tmp" "$SEDES/silva.c" \
      || { echo "DEEST: git show $PIGNUS:silva/amalgama/silva.{c,h}" >&2; exit 2; }
fi
if [ ! -f "$SEDES/amalgama.o" ]; then
    echo "  [pignus $PIGNUS] silva.c (semel)"
    clang "${VEXILLA_C89[@]}" -c "$SEDES/silva.c" -o "$SEDES/amalgama.o" \
        || { echo "FRACTA: amalgama ad pignus" >&2; rm -f "$SEDES/amalgama.o"; exit 1; }
fi

# ---- 2. instrumentum ad pignus ----
clang "${VEXILLA_C89[@]}" -DORACULUM_PIGNUS \
      -I"$SEDES" -I"$RADIX/include" \
      -c "$FONS" -o "$SEDES/oraculum.o" \
    || { echo "FRACTA: oraculum_silvae.c (pignus)" >&2; exit 1; }

# ISOLATIO: poscenda instrumenti - definita amalgamatis = libc solum
LIBC="fclose fopen fprintf fread free fseek ftell fwrite malloc memcmp
      printf putchar puts sprintf strcmp strcpy strlen __stderrp
      __chkstk_darwin __sprintf_chk __stack_chk_fail __stack_chk_guard"
# (secunda linea: symbola quae compilator ipse gignit - printf in
# puts/putchar, custodia acervi, sprintf munitum; libc et runtime)
nm -u "$SEDES/oraculum.o" | sed 's/^_//' | sort -u > "$SEDES/poscenda.txt"
nm -gU "$SEDES/amalgama.o" | awk '{print $3}' | sed 's/^_//' | sort -u \
    > "$SEDES/definita.txt"
ALIENA=$(comm -23 "$SEDES/poscenda.txt" "$SEDES/definita.txt" \
         | grep -v -x -F -f <(printf '%s\n' $LIBC))
if [ -n "$ALIENA" ]; then
    echo "ISOLATIO VIOLATA: instrumentum ad pignus symbola poscit quae" >&2
    echo "neque amalgama neque libc praebent:" >&2
    echo "$ALIENA" | sed 's/^/  /' >&2
    exit 1
fi

clang "${VEXILLA_C89[@]}" "$SEDES/oraculum.o" "$SEDES/amalgama.o" \
      -o "$SEDES/oraculum" \
    || { echo "FRACTA: nexus oraculi ad pignus" >&2; exit 1; }

# ---- 3. instrumentum vivum ----
# exclusio (olim shim eadem) (amalgama, instrumenta, probationes)
OBIECTA=$(ls "$RADIX"/silva/build/*.o 2>/dev/null \
          | grep -v -E "/(fons_|nexus_|instr_|probatio_|silva_amalgama\.o|amalgama_verificatio\.o|apparatus\.o)")
if [ -z "$OBIECTA" ]; then
    echo "DEEST: silva/build/*.o - curre ./silva/compile_probationes.sh primum" >&2
    exit 2
fi
clang "${VEXILLA_C89[@]}" \
      -I"$RADIX/include" -I"$RADIX/silva/fontes" -I"$RADIX/silva/instrumenta" \
      -I"$RADIX/materia/fontes" \
      -c "$FONS" -o "$BASIS/vivum.o" \
    || { echo "FRACTA: oraculum_silvae.c (vivum)" >&2; exit 1; }
# shellcheck disable=SC2086
clang "${VEXILLA_C89[@]}" "$BASIS/vivum.o" $OBIECTA -o "$BASIS/vivum" \
    || { echo "FRACTA: nexus oraculi vivi" >&2; exit 1; }

echo "oraculum_silvae: pignus $PIGNUS + vivum aedificata"
exit 0
