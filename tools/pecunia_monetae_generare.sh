#!/bin/bash
# tools/pecunia_monetae_generare.sh - tabula monetarum GENERATA (pecunia)
#
# lib/pecunia_monetae.c ex indicibus ISO 4217 fixis
# (probationes/fixa/iso4217/<publicatum>/list-one.xml, list-three.xml)
# et probationes/fixa/iso4217/supplementum.stml per
# tools/pecunia_monetae_generare.c (aedilis struit: xml.h, stml.h).
# Indicibus novis: directorium novum datatum, DATA infra muta, regenera,
# differentiam recense (pecunia-spec II.9).
#
# Usus: ./tools/pecunia_monetae_generare.sh            # scribit
#       ./tools/pecunia_monetae_generare.sh -probare   # recens? 0/1 (2 fractum)
#       ./tools/pecunia_monetae_generare.sh -fumus     # regulae recusationis
#
# -fumus: supplementa MALA in directorio temporario (ordo contradicens,
# superfluus, bis, historica non retracta, extra_iso retracta, sine
# fonte) - generator quemque recusare debet (exitus 2) causa NOMINATA.
# Data vera has regulas numquam tangunt; sine fumo regula deleta muta
# esset. Exitus 0 sanum, 1 FRACTUM.
#
# -probare EXACTA (cmp): plagula commissa = effusio generatoris octetim
# (GENERATUM in linea prima - uncus pre-commit eam non format).
set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

DATA="probationes/fixa/iso4217/2026-09-17"
SUPPLEMENTUM="probationes/fixa/iso4217/supplementum.stml"
EXITUS="lib/pecunia_monetae.c"
PROBARE=0
FUMUS=0
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        -fumus) FUMUS=1 ;;
        *) echo "usus: pecunia_monetae_generare.sh [-probare|-fumus]" >&2
           exit 2 ;;
    esac
done

if [ ! -x bin/aedilis ]; then
    echo "pecunia_monetae_generare: bin/aedilis abest - strue: ./tools/aedilis_struere.sh" >&2
    exit 2
fi
bin/aedilis tools/pecunia_monetae_generare.c >/dev/null \
    || { echo "pecunia_monetae_generare: aedilis fracta" >&2; exit 2; }
sh build/aedilis/pecunia_monetae_generare/struere.sh >/dev/null \
    || { echo "pecunia_monetae_generare: structura fracta" >&2; exit 2; }
BIN="build/aedilis/pecunia_monetae_generare/pecunia_monetae_generare"

if [ "$FUMUS" = 1 ]; then
    T="$(mktemp -d "${TMPDIR:-/tmp}/pecunia_fumus.XXXXXX")"
    trap 'rm -rf "$T"' EXIT
    FRACTA=0
    # casus: titulus | ordo insertus | fragmentum causae exspectatum
    casus () {
        sed "s#</supplementum>#$2\n</supplementum>#" "$SUPPLEMENTUM" > "$T/s.stml"
        if "$BIN" "$DATA" "$T/s.stml" "$T/x.c" 2> "$T/err"; then
            echo "  FRACTUM $1: generator accepit"; FRACTA=1; return
        fi
        if ! grep -q -- "$3" "$T/err"; then
            echo "  FRACTUM $1: causa alia: $(cat "$T/err")"; FRACTA=1; return
        fi
        echo "  sanum $1"
    }
    R='<moneta codex="%s" exponens="%s" status="%s" titulus="t" fons="%s"/>'
    casus contradicit "$(printf "$R" USD 3 extra_iso f)" "contradicit indici uno"
    casus superfluum  "$(printf "$R" USD 2 extra_iso f)" "superfluum"
    casus bis         "$(printf "$R" GGP 2 extra_iso f)" "bis eundem codicem"
    casus non_retracta "$(printf "$R" ZZQ 2 historica f)" "historica non in indice tertio"
    casus retracta    "$(printf "$R" ESP 0 extra_iso f)" "extra_iso tamen in indice tertio"
    casus sine_fonte  "$(printf "$R" ZZQ 2 extra_iso '')" "fons"
    if [ "$FRACTA" = 0 ]; then
        echo "fumus pecunia_monetae: sanum"; exit 0
    fi
    echo "fumus pecunia_monetae: FRACTUM"; exit 1
fi

if [ "$PROBARE" = 1 ]; then
    TMP="$(mktemp)"
    trap 'rm -f "$TMP"' EXIT
    "$BIN" "$DATA" "$SUPPLEMENTUM" "$TMP" || exit 2
    if cmp -s "$TMP" "$EXITUS"; then
        echo "pecunia_monetae_generare: $EXITUS recens"
        exit 0
    fi
    echo "pecunia_monetae_generare: $EXITUS RANCIDA (regenera: ./tools/pecunia_monetae_generare.sh)"
    exit 1
fi
# FABRICA_SCRIPTURA (bin/fabrica -plenus)
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
    mkdir -p "$(dirname "$EXITUS")" || exit 2
fi
"$BIN" "$DATA" "$SUPPLEMENTUM" "$EXITUS"
