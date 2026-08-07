#!/bin/bash

# tools/natura_canones.sh [-probare]
#
# Canones ex natura/*.genera generare aut rancorem probare.
# Mos portae METAMODULI: littera generata committitur, porta
# rancorem clamat.
#
#   sine argumento   omnes regenerare (in locis veris)
#   -probare         conferre solum: exitus 0 recentes, 1 RANCIDI,
#                    2 defectus instrumenti (numquam sanitas tacita)
#
# MANIFESTUM NULLUM: corpus ipsum manifestum est. Omne
# natura/<stirps>.genera canonem natura/cocta/<stirps>.canon parit,
# et monolithus natura/cocta/individua.canon omnia iungit. (Contra
# cocta.registrum, quod canon_coquere.sh eo solo eget quod fontes
# eius ex directorio numerari NEQUEUNT.)
#
# Porta OUTPUT confert, numquam tempora: commentarium solum in
# .genera additum generationem non mutat, ergo rancorem NON parit.
# Porta temporum de commentariis clamaret, et porta quae falso
# clamat mox neglegitur - id est mors portae.

set -u
cd "$(dirname "$0")/.." || exit 2

PORTA=bin/natura_canones
COCTA=natura/cocta
STRUCTOR=./tools/natura_struere.sh

# ------------------------------------------------------------------
# argumenta

PROBARE=0
if [ "$#" -gt 1 ]; then
    echo "usus: ./tools/natura_canones.sh [-probare]" >&2
    exit 2
fi
if [ "$#" -eq 1 ]; then
    if [ "$1" = "-probare" ]; then
        PROBARE=1
    else
        # argumentum ignotum TACITE ignoratum '-probar' in
        # regenerationem verteret: petitor iudicium petivit,
        # scripturam accepit.
        echo "natura_canones: argumentum ignotum '$1'" >&2
        echo "usus: ./tools/natura_canones.sh [-probare]" >&2
        exit 2
    fi
fi

# ------------------------------------------------------------------
# porta ipsa: instrumentum stalum litteram stalam benediceret

if [ ! -x "$PORTA" ]; then
    echo "natura_canones: $PORTA abest - strue: $STRUCTOR" >&2
    exit 2
fi
for _f in lib/natura.c include/natura.h lib/stml.c \
          tools/natura_canones.c tools/natura_canones_emissio.c \
          tools/natura_canones.h; do
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_canones: $PORTA STALUS ($_f recentior) - strue: $STRUCTOR" >&2
        exit 2
    fi
done

mkdir -p "$COCTA" || exit 2
TMPD=build/natura_canones_tmp
mkdir -p "$TMPD" || exit 2
PROBANDUM="$TMPD/probandum.canon"
QUERELA="$TMPD/querela.txt"

rancidi=0
facti=0
moduli=0
MONSTRATA="$TMPD/monstrata.txt"
: > "$MONSTRATA"

# Querela NOVA semper monstratur; repetita sola tacet.
#
# Pessulus pristinus (primam querelam monstrare, deinde silere) ex
# praemissa FALSA natus est: putavi querelam omni vocatione eandem
# esse quia de corpore toto esset. MENSURATUM aliter - octo moduli
# monita PROPRIA fundunt (valores planati, praestituta omissa), et
# monolithus, ultimus generatus ergo a pessulo SEMPER devoratus,
# maximum omnium fundit (citationes 217, valores 22 planati,
# praestituta 51 omissa).
#
# Ea nuntia EMISSIONIS DAMNOSAE sunt - valores planati, praestituta
# in canonem non transeuntia - id est genus quod minime taceri
# debet, in instrumento cuius doctrina ipsa est damnum numquam
# TACITE fieri. Pessulus rem quam custodire debebat delebat.
#
# $1 = via scripti: lineae eam nominantes de UNO scripto sunt
# (nuntii progressus), non de damno. Via secatur, non verbum:
# nuntius mutari potest, munus lineae non.
querelam_novam_monstrare () {
    local linea
    [ -s "$QUERELA" ] || return 0
    while IFS= read -r linea || [ -n "$linea" ]; do
        case "$linea" in *"$1"*) continue ;; esac
        if ! grep -q -F -x -- "$linea" "$MONSTRATA" 2>/dev/null; then
            printf '%s\n' "$linea" >&2
            printf '%s\n' "$linea" >> "$MONSTRATA"
        fi
    done < "$QUERELA"
    return 0
}

# $1 = via vera, $2... = argumenta portae (sine -ad)
conferre_aut_scribere () {
    local vera="$1"; shift
    local exitus
    local scriptum

    if [ "$PROBARE" = "1" ]; then
        scriptum="$PROBANDUM"
        # exemplar vetus tollitur: aliter porta quae nihil scripsit
        # cum exemplari PRIORIS moduli conferret.
        rm -f "$PROBANDUM"
    else
        scriptum="$vera"
    fi

    "$PORTA" "$@" -ad "$scriptum" 2>"$QUERELA"
    exitus=$?
    if [ "$exitus" -ne 0 ]; then
        cat "$QUERELA" >&2
        echo "natura_canones: '$vera' generari nequit (exitus $exitus)" >&2
        exit 2
    fi
    querelam_novam_monstrare "$scriptum"

    # instrumentum exitum 0 reddere ET nihil scribere posset: tunc
    # porta nihil conferret et tacite sanaret.
    if [ ! -s "$scriptum" ]; then
        echo "natura_canones: '$vera' - exemplar vacuum aut absens post generationem" >&2
        exit 2
    fi

    if [ "$PROBARE" = "1" ]; then
        # Emissor NULLAM viam coquit: linea 'Regenera' litterale
        # fixum est et origo MODULUM nominat, non plagulam. Ergo
        # bytes recta conferuntur, sine substitutione ulla. Si
        # substitutio umquam necessaria videbitur, id signum est
        # emissorem viam coquere coepisse - id nuntia, noli
        # circumire.
        if ! cmp -s "$PROBANDUM" "$vera"; then
            echo "natura_canones: RANCIDUS $vera - regenera: ./tools/natura_canones.sh" >&2
            rancidi=$((rancidi + 1))
        fi
    fi
    facti=$((facti + 1))
}

# ------------------------------------------------------------------
# corpus = manifestum

for fons in natura/*.genera; do
    [ -e "$fons" ] || continue
    stirps=$(basename "$fons" .genera)
    moduli=$((moduli + 1))
    conferre_aut_scribere "$COCTA/$stirps.canon" -modulus "$stirps"
done

# Custodia ante monolithum: sine ea corpus VACUUM unum canonem
# (monolithum) faceret, facti unum esset, et porta exitum 0 redderet
# nihil vere collato.
if [ "$moduli" -eq 0 ]; then
    echo "natura_canones: NULLUM exemplar in natura/*.genera - nihil factum" >&2
    exit 2
fi

conferre_aut_scribere "$COCTA/individua.canon" -totum

if [ "$facti" -eq 0 ]; then
    echo "natura_canones: NIHIL factum" >&2
    exit 2
fi

if [ "$PROBARE" = "1" ]; then
    if [ "$rancidi" -gt 0 ]; then
        echo "natura_canones: canones $rancidi ex $facti RANCIDI" >&2
        exit 1
    fi
    echo "natura_canones: canones $facti recentes"
else
    echo "natura_canones: canones $facti cocti" >&2
fi
exit 0
