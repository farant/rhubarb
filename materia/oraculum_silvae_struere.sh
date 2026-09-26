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

# ---- 1. amalgama ad pignus (semel per pignus) - PURUM ----
if [ ! -f "$SEDES/silva.pignus.c" ] || [ ! -f "$SEDES/silva.h" ]; then
    git -C "$RADIX" show "$PIGNUS:silva/amalgama/silva.c" > "$SEDES/silva.pignus.c.tmp" \
      && git -C "$RADIX" show "$PIGNUS:silva/amalgama/silva.h" > "$SEDES/silva.h" \
      && mv "$SEDES/silva.pignus.c.tmp" "$SEDES/silva.pignus.c" \
      || { echo "DEEST: git show $PIGNUS:silva/amalgama/silva.{c,h}" >&2; exit 2; }
fi

# ---- 1b. LEXICON LATINUM HODIERNUM (silva-migratio T18) ----
# CODEX ex pignore, INITIA ex hodie. Silva latina.h non e disco legit
# sed copiam in binario COMPILATAM fert (silva_latina_datum.c, fons 0
# ante plagulam) - ergo binarium pignoris latina.h commissi 7a4847b0
# secum portabat, et quaelibet mutatio latina.h quae definitionem
# macri movet (linea descriptionis T18: +I linea, +LXV octeti) omnem
# plagulam domus 'dividebat' (DCCCX innominati: sedes lexematum ex
# macris latinis - linea/octetus intra latina.h - in columna stml et
# comparatore; arbor, emissio, errores, semantica AEQUALES). Latina
# INITIUM est, non codex iudicatus (ut stand-ins systematis, quos
# ambo latera e disco legunt): datum hodiernum in amalgama pignoris
# transplantatur. Mensuratum in arbore scalpta: transplantatione
# facta oraculum purum (0/0/0). Pignus NON movetur.
# CUSTODIAE: signa initii et finis semel in utroque (ancora ad
# initium lineae), mensura transplantata == mensura dati; aliter
# FRACTA clamans (numquam tacite pignus crudum). Sigillum dati
# memoratur - amalgama pignoris recompilatur solum latina mutata.
DATUM="$RADIX/silva/fontes/silva_latina_datum.c"
INITIUM='constans character silva_latina_textus[] = {'
FINIS='constans i32 silva_latina_mensura ='
SIGILLUM_DATI=$(shasum "$DATUM" 2>/dev/null | cut -d' ' -f1)
[ -n "$SIGILLUM_DATI" ] || { echo "DEEST: $DATUM" >&2; exit 2; }
_signa () {  # $1 = plagula -> "initia fines" (ancora ad initium lineae)
    awk -v I="$INITIUM" -v F="$FINIS" \
        'index($0, I) == 1 {i++} index($0, F) == 1 {f++}
         END {print i+0, f+0}' "$1"
}
if [ ! -f "$SEDES/silva.c" ] \
   || [ "$(cat "$SEDES/latina.sigillum" 2>/dev/null)" != "$SIGILLUM_DATI" ]; then
    for f in "$DATUM" "$SEDES/silva.pignus.c"; do
        if [ "$(_signa "$f")" != "1 1" ]; then
            echo "FRACTA: transplantatio latinae - $f: signa (initia fines) $(_signa "$f"), exspectata 1 1" >&2
            exit 1
        fi
    done
    awk -v I="$INITIUM" -v F="$FINIS" -v D="$DATUM" '
        BEGIN {
            while ((getline l < D) > 0) {
                if (index(l, I) == 1) intus = 1
                if (intus) truncus = truncus l "\n"
                if (intus && index(l, F) == 1) intus = 0
            }
        }
        index($0, I) == 1 { printf "%s", truncus; salta = 1; next }
        salta && index($0, F) == 1 { salta = 0; next }
        salta { next }
        { print }' "$SEDES/silva.pignus.c" > "$SEDES/silva.c.tmp" \
        || { echo "FRACTA: transplantatio latinae (awk)" >&2; exit 1; }
    if [ "$(grep "^$FINIS" "$DATUM")" != "$(grep "^$FINIS" "$SEDES/silva.c.tmp")" ]; then
        echo "FRACTA: transplantatio latinae - mensura in amalgamate pignoris != mensura dati" >&2
        exit 1
    fi
    mv "$SEDES/silva.c.tmp" "$SEDES/silva.c"
    echo "$SIGILLUM_DATI" > "$SEDES/latina.sigillum"
    rm -f "$SEDES/amalgama.o"
    echo "  [pignus $PIGNUS] latina hodierna transplantata ($(grep "^$FINIS" "$DATUM"))"
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
