#!/bin/bash

# tools/amalgamata_probare.sh - amalgamata COMMISSA compilare
#
# Usus: ./tools/amalgamata_probare.sh
# Exitus: 0 = omnia compilant | 1 = fractum
#
# QUID PROBAT, ET QUID NON
# ------------------------
# PROBAT: artificium COMMISSUM (silva/amalgama/silva.c et fratres)
# adhuc sub severitate domus plena compilare. Id est: nemo eum manu
# corrupit, nemo fructum fractum commisit, nemo caput manu scriptum
# (silva.h) a corpore divergere sivit.
#
# NON PROBAT - et hoc alta voce dicendum est - semitam
# REGENERATIONIS. Omnes fracturae 2026-08-20 inventae ibi vivebant:
# recipe amalgamatoris divergens (tessera + officina amalgamari
# omnino non poterant), manifesta rancida, listae geminae. Per illa
# omnia artificia commissa PURE COMPILABANT, quia artificium vetus
# rectum manet dum machina quae id gignit frangitur.
#
# Ergo: haec porta VILIS est (< III sec.) et classem propriam capit
# (putredinem artificii); semita regenerationis portam SUAM petit
# (vide tools/porta_vetustatis.sh in vectibus, et amalgamare.sh
# ipsum, quod ~LXXIX sec. pro tribus proiectis sumit).
#
# Obiectum VERUM scribitur, non /dev/null: '-o /dev/null' probat te
# compilare POSSE, non compilavisse (lectio domus).
# <porta titulus="amalgamata" genus="praeiudicium" tegit="*/amalgama/*"/>

set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

# vexilla: tools/vexilla.sh (una sedes). Olim copia propria hic stabat
# - et Linux (-D_DEFAULT_SOURCE, 2026-10-26) eam non attigit: silva et
# tessera in snprintf / struct sigaction cadebant. -O2 et -g omissa:
# porta VILIS manet (compilatio sola probatur, obiectum non curritur).
source "$SCRIPT_DIR/vexilla.sh"
declare -a VEXILLA=()
for _v in "${VEXILLA_C89[@]}"; do
    case "$_v" in -O2|-g) ;; *) VEXILLA+=("$_v") ;; esac
done

STATIO="build/amalgamata_probatio"
mkdir -p "$STATIO"

fracta=0
summa=0
for amalgama in silva/amalgama/silva.c \
                tessera/amalgama/tessera.c \
                officina/amalgama/officina.c; do
    [ -f "$amalgama" ] || continue
    summa=$((summa + 1))
    basis="$(basename "$amalgama" .c)"
    obiectum="$STATIO/$basis.o"
    rm -f "$obiectum"

    # officina ex ARCHITECTURA a silva pendet (caput amalgamatis
    # silvae) - inclusio unica documentata, ceteris innocua
    if ! clang "${VEXILLA[@]}" -Isilva/amalgama \
            -c "$amalgama" -o "$obiectum" 2> "$STATIO/$basis.err"; then
        echo "[AMALGAMA FRACTA] $amalgama"
        head -8 "$STATIO/$basis.err"
        fracta=$((fracta + 1))
        continue
    fi
    if [ ! -s "$obiectum" ]; then
        echo "[AMALGAMA VANA] $amalgama - obiectum vacuum"
        fracta=$((fracta + 1))
    fi
done

if [ "$summa" -eq 0 ]; then
    echo "amalgamata_probare: NULLUM amalgama inventum - recusatum" >&2
    exit 1
fi

echo "amalgamata: $summa compilata, $fracta fracta"
[ "$fracta" -eq 0 ] || exit 1
