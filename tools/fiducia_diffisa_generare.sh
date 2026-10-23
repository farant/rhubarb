#!/bin/bash
# tools/fiducia_diffisa_generare.sh - tabula diffisarum GENERATA (fiducia)
#
# lib/fiducia_diffisa_tabula.c ex instantanea OneCRL fixa
# (probationes/fixa/fiducia/onecrl/<dies>/changeset.json + SHA256SUMS)
# et probationes/fixa/fiducia/diffisa.stml per
# tools/fiducia_diffisa_generare.c (aedilis struit: json.h, stml.h).
#
# RENOVATIO (consulta, numquam porta): instantaneam novam in directorium
# novum datatum (dies UTC 'timestamp' changeset) cape -
#   curl -o changeset.json 'https://firefox.settings.services.mozilla.com/v1/buckets/security-state/collections/onecrl/changeset?_expected=0'
#   shasum -a 256 changeset.json > SHA256SUMS
# - DATA infra muta, regenera, differentiam recense, committe, strue,
# instrue in servis (fiducia-linux-spec II.4).
#
# Usus: ./tools/fiducia_diffisa_generare.sh            # scribit
#       ./tools/fiducia_diffisa_generare.sh -probare   # recens? 0/1 (2 fractum)
#       ./tools/fiducia_diffisa_generare.sh -fumus     # regulae recusationis
#
# -fumus: indices manu MALI et instantanea mala in directorio temporario
# (genus ignotum, base64 malum, attributum deest, clavis non XXXII
# octeti, SHA256SUMS non congruens, directorium non datatum) -
# generator quemque recusare debet (exitus 2) causa NOMINATA; et index
# bonus duarum diffisarum super instantaneam vacuam = tabula duarum,
# ordinata. Exitus 0 sanum, 1 FRACTUM. (Ut pecunia_monetae: manu, non
# porta - plan ruling T2.)
#
# -probare EXACTA (cmp): plagula commissa = effusio generatoris octetim
# (GENERATUM in linea prima - uncus pre-commit eam non format).
set -u
export LC_ALL=C
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$RADIX_DIR"

DATA="probationes/fixa/fiducia/onecrl/2026-10-21"
MANU="probationes/fixa/fiducia/diffisa.stml"
EXITUS="lib/fiducia_diffisa_tabula.c"
PROBARE=0
FUMUS=0
for arg in "$@"; do
    case "$arg" in
        -probare) PROBARE=1 ;;
        -fumus) FUMUS=1 ;;
        *) echo "usus: fiducia_diffisa_generare.sh [-probare|-fumus]" >&2
           exit 2 ;;
    esac
done

if [ ! -x bin/aedilis ]; then
    echo "fiducia_diffisa_generare: bin/aedilis abest - strue: ./tools/aedilis_struere.sh" >&2
    exit 2
fi
bin/aedilis tools/fiducia_diffisa_generare.c >/dev/null \
    || { echo "fiducia_diffisa_generare: aedilis fracta" >&2; exit 2; }
sh build/aedilis/fiducia_diffisa_generare/struere.sh >/dev/null \
    || { echo "fiducia_diffisa_generare: structura fracta" >&2; exit 2; }
BIN="build/aedilis/fiducia_diffisa_generare/fiducia_diffisa_generare"

summa () {   # directorium: SHA256SUMS changeset.json
    ( cd "$1" && shasum -a 256 changeset.json > SHA256SUMS )
}

if [ "$FUMUS" = 1 ]; then
    T="$(mktemp -d "${TMPDIR:-/tmp}/diffisa_fumus.XXXXXX")"
    trap 'rm -rf "$T"' EXIT
    FRACTA=0
    V="$T/2026-01-01"
    mkdir -p "$V"
    printf '{"metadata":{},"timestamp":0,"changes":[]}\n' > "$V/changeset.json"
    summa "$V"
    # MC4xCzAJBgNVBAYTAlVT = base64 Name brevis; AQ== serie 01;
    # clavis XXXII octeti zephyrorum = AAAA...AA=
    K="AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
    N="MA4xDDAKBgNVBAMMA2FiYw=="
    D='<diffisa genus="%s" primum="%s" secundum="%s" causa="c" dies="%s" fons="%s"/>'
    manu () { printf '<diffisae>\n%s\n</diffisae>\n' "$1" > "$T/m.stml"; }
    casus () {   # titulus | directorium | fragmentum causae
        if "$BIN" "$2" "$T/m.stml" "$T/x.c" 2> "$T/err"; then
            echo "  FRACTUM $1: generator accepit"; FRACTA=1; return
        fi
        if ! grep -q -- "$3" "$T/err"; then
            echo "  FRACTUM $1: causa alia: $(cat "$T/err")"; FRACTA=1; return
        fi
        echo "  sanum $1"
    }
    manu "$(printf "$D" vitium "$N" AQ== 2026-01-01 f)"
    casus genus "$V" "genus ignotum"
    manu "$(printf "$D" emittens_series '###' AQ== 2026-01-01 f)"
    casus base64 "$V" "base64 malum"
    manu "$(printf "$D" emittens_series "$N" AQ== 2026-01-01 '')"
    casus sine_fonte "$V" "attributum deest"
    manu "$(printf "$D" subiectum_clavis "$N" AQ== 2026-01-01 f)"
    casus clavis_brevis "$V" "non XXXII octeti"
    manu "$(printf "$D" emittens_series "$N" AQ== 2026-1-1 f)"
    casus dies "$V" "dies non YYYY-MM-DD"
    manu ""
    printf ' ' >> "$V/changeset.json"
    casus summa "$V" "SHA256SUMS non congruit"
    summa "$V"
    mkdir -p "$T/nondatatum"; cp "$V/changeset.json" "$V/SHA256SUMS" "$T/nondatatum/"
    casus nondatatum "$T/nondatatum" "non YYYY-MM-DD"
    # bonus: duae, ordine (emittens_series ante subiectum_clavis)
    manu "$(printf "$D" subiectum_clavis "$N" "$K" 2026-01-01 f)
$(printf "$D" emittens_series "$N" AQ== 2026-01-01 f)"
    if "$BIN" "$V" "$T/m.stml" "$T/x.c" 2> "$T/err" \
        && grep -q "fiducia_diffisae_numerus = 2;" "$T/x.c" \
        && [ "$(grep -o 'FIDUCIA_DIFFISA_[A-Z_]*,' "$T/x.c" | head -2 | tr '\n' ' ')" = "FIDUCIA_DIFFISA_EMITTENS_SERIES, FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS, " ] \
        && grep -q '"c (2026-01-01, f)"' "$T/x.c"; then
        echo "  sanum bonus"
    else
        echo "  FRACTUM bonus: $(cat "$T/err")"; FRACTA=1
    fi
    if [ "$FRACTA" = 0 ]; then
        echo "fumus fiducia_diffisa: sanum"; exit 0
    fi
    echo "fumus fiducia_diffisa: FRACTUM"; exit 1
fi

if [ "$PROBARE" = 1 ]; then
    TMP="$(mktemp)"
    trap 'rm -f "$TMP"' EXIT
    "$BIN" "$DATA" "$MANU" "$TMP" || exit 2
    if cmp -s "$TMP" "$EXITUS"; then
        echo "fiducia_diffisa_generare: $EXITUS recens"
        exit 0
    fi
    echo "fiducia_diffisa_generare: $EXITUS RANCIDA (regenera: ./tools/fiducia_diffisa_generare.sh)"
    exit 1
fi
# FABRICA_SCRIPTURA (bin/fabrica -plenus)
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
    mkdir -p "$(dirname "$EXITUS")" || exit 2
fi
"$BIN" "$DATA" "$MANU" "$EXITUS"
