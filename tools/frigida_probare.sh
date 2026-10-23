#!/bin/bash
# <cursus titulus="tools/frigida_probare.sh" versio="I"/>
# CHASSIS CURSUUM (analytica T6): cursus in annalibus
source "$(dirname "${BASH_SOURCE[0]}")/cursus.sh"
# frigida_probare.sh - omnes suitae in arbore FRIGIDA (clone novus)
#
# Quaestio …QY4 (2026-09-28): suita quae initum gitignoratum legit
# (build/, data/, fixa non commissa) in hac machina viridis est, in
# clone quavis rubra - "viride" tunc "viride in hoc disco" significat.
# Hoc instrumentum arborem operis novam ex COMMISSO facit (git worktree
# add, ramus temporarius), omnes suitas sine aedificatione praevia
# currit, rubras nominat, arborem delet.
#
# Mensurat COMMISSUM, non arborem operis: mutationes non commissae non
# videntur. Tardum (~12 min) - instrumentum ante fusionem aut per
# intervalla, non porta commissionis (Fran, 2026-09-28).
#
# Suitae: radix (./compile_tests.sh) + omne <sub>/compile_probationes.sh
# (per globum - suitae novae sponte intrant).
#
# Usus (ex radice):
#   ./tools/frigida_probare.sh [-ref <commissum>] [-suitae "radix toml"]
#                              [-servare]
#   -ref      commissum mensurandum (ordinarium HEAD)
#   -suitae   subset nominatum (ordinarium: omnes)
#   -servare  arborem et acta non delere (ad inspiciendum; signum
#             SERVATUM in area - purgatio orphanorum eam non tangit)
#
# PURGATIO (parcum YAPY6G, 2026-10-15: III arbores 14 GB + rami
# frigida-* relicti, deinde 6.8 GB post cursum rubrum): arbor et ramus
# SEMPER delentur - etiam rubro (acta sola manent, MB; arbor ex commisso
# iterum fit per -ref <sha> -servare) et interruptione (trap EXIT INT
# TERM HUP). In initio orphani priores (frigida-<sha>-<pid>, pid mortuus,
# sine SERVATUM) delentur, linea una quisque.
#
# Exitus: 0 omnes virides / 1 aliqua rubra (nominata) / 2 praeparatio
# fracta.

RADIX="$(cd "$(dirname "$0")/.." && pwd)"
REF=HEAD
SUITAE=""
SERVARE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -ref)     REF="$2"; shift 2 ;;
        -suitae)  SUITAE="$2"; shift 2 ;;
        -servare) SERVARE=1; shift ;;
        *) echo "frigida: argumentum ignotum: $1" >&2; exit 2 ;;
    esac
done

cd "$RADIX" || exit 2
SHA="$(git rev-parse --verify "$REF^{commit}" 2>/dev/null)" \
    || { echo "frigida: commissum ignotum: $REF" >&2; exit 2; }
BREVE="${SHA:0:8}"
AREA="$(mktemp -d "${TMPDIR:-/tmp}/frigida.XXXXXX")" || exit 2
ARBOR="$AREA/arbor"
ACTA="$AREA/acta"
RAMUS="frigida-$BREVE-$$"
mkdir -p "$ACTA"

# acta servanda: rubrum (nomina in actis) - arbor tamen deletur
ACTA_SERVARE=0
PURGATUM=0
purgare () {
    [ "$PURGATUM" = 1 ] && return
    PURGATUM=1
    if [ "$SERVARE" = 1 ]; then
        : > "$AREA/SERVATUM"
        echo "frigida: servatum - arbor $ARBOR, acta $ACTA, ramus $RAMUS"
        return
    fi
    git -C "$RADIX" worktree remove --force "$ARBOR" >/dev/null 2>&1
    git -C "$RADIX" branch -D "$RAMUS" >/dev/null 2>&1
    if [ "$ACTA_SERVARE" = 1 ]; then
        rm -rf "$ARBOR"
    else
        rm -rf "$AREA"
    fi
}

# ORPHANI: rami frigida-<sha>-<pid> quorum processus non vivit (pid
# reusus: imperium eius frigida_probare non continet) - arbor (nisi
# SERVATUM in area eius) et ramus delentur
orphanos_purgare () {
    local ramus pid arbor area mensura
    for ramus in $(git -C "$RADIX" for-each-ref --format='%(refname:short)' 'refs/heads/frigida-*'); do
        pid="${ramus##*-}"
        case "$pid" in ''|*[!0-9]*) continue ;; esac
        if kill -0 "$pid" 2>/dev/null \
            && ps -p "$pid" -o command= 2>/dev/null | grep -q frigida_probare; then
            continue
        fi
        arbor="$(git -C "$RADIX" worktree list --porcelain \
            | awk -v r="refs/heads/$ramus" '/^worktree /{w=substr($0,10)} $0=="branch " r {print w}')"
        area="$(dirname "${arbor:-/nusquam/x}")"
        if [ -n "$arbor" ] && [ -e "$area/SERVATUM" ]; then
            echo "frigida: orphanus SERVATUS relictus: $ramus ($arbor)"
            continue
        fi
        mensura=""
        if [ -n "$arbor" ] && [ -d "$arbor" ]; then
            mensura="$(du -sh "$arbor" 2>/dev/null | cut -f1)"
            git -C "$RADIX" worktree remove --force "$arbor" >/dev/null 2>&1
            rm -rf "$arbor"
        fi
        git -C "$RADIX" branch -D "$ramus" >/dev/null 2>&1
        echo "frigida: orphanus purgatus: $ramus${arbor:+ ($arbor, ${mensura:-?})}"
    done
    git -C "$RADIX" worktree prune >/dev/null 2>&1
}

# INTERRUPTIO: suita in fundo + 'wait' (wait signo statim redit; sine
# eo bash trap differt donec suita finita - minuta). Arbor processuum
# suitae necatur (non 'kill 0': vocans, e.g. fusio_probare, in eodem
# grege processuum stat), deinde EXIT -> purgare
NATUS=""
matare () {
    local p
    for p in $(pgrep -P "$1" 2>/dev/null); do matare "$p"; done
    kill -TERM "$1" 2>/dev/null
}
interrumpere () {
    trap - INT TERM HUP
    if [ -n "$NATUS" ]; then
        echo "frigida: interrupta - suita necatur, arbor purgatur" >&2
        matare "$NATUS"
        wait "$NATUS" 2>/dev/null
    fi
    exit 130
}

orphanos_purgare
trap purgare EXIT
trap interrumpere INT TERM HUP

git worktree add -q -b "$RAMUS" "$ARBOR" "$SHA" >/dev/null 2>&1 \
    || { echo "frigida: git worktree add defecit" >&2; PURGATUM=1; rm -rf "$AREA"; exit 2; }

if [ -z "$SUITAE" ]; then
    SUITAE="radix"
    for c in "$ARBOR"/*/compile_probationes.sh; do
        SUITAE="$SUITAE $(basename "$(dirname "$c")")"
    done
fi

echo "frigida: commissum $BREVE, arbor $ARBOR"
# MENSURA (2026-10-22): arbor frigida bin/mensor non habet - mensor
# HUIUS arboris suitis datur (metra suitarum: compile_tests.sh,
# tools/mensor_suitae.sh), et suita quaeque cursus suus est (filius
# cursus frigidae: 'mensor quaestio filii' ubi tempus abiit). Mensor
# absens = sine mensura, numquam fractura.
if [ -x "$RADIX/bin/mensor" ]; then
    export MENSOR_BINARIUM="$RADIX/bin/mensor"
fi
_suitam_incipere () {   # <suita> -> SUITA_CURSUS, MENSOR_PARENS suitae
    SUITA_CURSUS=""
    SUITA_PARENS_VETUS="${MENSOR_PARENS:-}"
    SUITA_MS0="$(perl -MTime::HiRes -e 'printf "%d", Time::HiRes::time * 1000')"
    [ -n "${MENSOR_BINARIUM:-}" ] || return 0
    SUITA_CURSUS="$("$MENSOR_BINARIUM" cursus incipere -titulus "frigida:$1" \
        2>/dev/null | sed -n 's/^MENSOR_PARENS=//p')"
    [ -n "$SUITA_CURSUS" ] && export MENSOR_PARENS="$SUITA_CURSUS"
    return 0
}
_suitam_finire () {     # <exitus>
    local ms1
    ms1="$(perl -MTime::HiRes -e 'printf "%d", Time::HiRes::time * 1000')"
    if [ -n "${SUITA_CURSUS:-}" ]; then
        "$MENSOR_BINARIUM" cursus finire -id "$SUITA_CURSUS" -exitus "$1" \
            -duratio_ms "$((ms1 - SUITA_MS0))" >/dev/null 2>&1 || true
    fi
    if [ -n "$SUITA_PARENS_VETUS" ]; then
        export MENSOR_PARENS="$SUITA_PARENS_VETUS"
    else
        unset MENSOR_PARENS
    fi
    return 0
}
RUBRAE=0
for s in $SUITAE; do
    t0=$(date +%s)
    _suitam_incipere "$s"
    if [ "$s" = radix ]; then
        ( cd "$ARBOR" && ./compile_tests.sh ) > "$ACTA/$s.log" 2>&1 &
    elif [ -x "$ARBOR/$s/compile_probationes.sh" ]; then
        ( cd "$ARBOR" && "./$s/compile_probationes.sh" ) > "$ACTA/$s.log" 2>&1 &
    else
        echo "  $s: SUITA IGNOTA"; RUBRAE=$((RUBRAE + 1)); _suitam_finire 2; continue
    fi
    NATUS=$!
    wait "$NATUS"
    rc=$?
    NATUS=""
    _suitam_finire "$rc"
    dt=$(( $(date +%s) - t0 ))
    if [ "$rc" = 0 ]; then
        printf "  %-12s viridis (%ds)\n" "$s" "$dt"
    else
        RUBRAE=$((RUBRAE + 1))
        # nomina rubrarum: radix 'Failed: a b', subsystemata 'FRACTAE: a b'
        nomina="$(LC_ALL=C grep -a -h -E '^(.\[31m)?Failed: |^FRACTAE: ' "$ACTA/$s.log" \
            | tail -1 | sed -E 's/\x1b\[[0-9;]*m//g; s/^(Failed|FRACTAE): //')"
        printf "  %-12s RUBRA rc=%s (%ds): %s\n" "$s" "$rc" "$dt" "${nomina:-(vide acta)}"
    fi
done

if [ "$RUBRAE" = 0 ]; then
    echo "frigida: omnes virides ($BREVE)"
    purgare
    exit 0
fi
echo "frigida: $RUBRAE suitae rubrae ($BREVE) - acta: $ACTA"
echo "frigida: arbor deleta; ad inspiciendum: ./tools/frigida_probare.sh -ref $BREVE -suitae \"<rubrae>\" -servare"
ACTA_SERVARE=1
purgare
exit 1
