#!/bin/bash
# examen-custos.sh - PostToolUse (Write|Edit): iudicium C89 in
# plagulam .c modo scriptam - canalis PUSH (praecedens
# censor-custos; dum clangd quoque currit = differentialis VIVUS
# in sessionibus veris, calibratio ante substitutionem lsp).
#
# Disciplina silentii: nulla diagnostica (praeter infra) = NIHIL
# in contextum. Exclusiones lexici (exclusiones.txt pinnatae)
# TACENT - strepitus deorsum-lexici doceret negligere. Capita .h
# non iudicantur sola (per includentes iudicantur); scratchpad/
# build/fixa exclusa (probae consulto pravae).

INPUT=$(cat)
FILE=$(printf '%s' "$INPUT" | jq -r '.tool_input.file_path // empty')

case "$FILE" in
    *.c) ;;
    *) exit 0 ;;
esac
case "$FILE" in
    */scratchpad/*|*/build/*|*/fixa/*|*/amalgama/*) exit 0 ;;
esac
[ -f "$FILE" ] || exit 0

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

case "$FILE" in
    "$RADIX"/*) REL="./${FILE#"$RADIX"/}" ;;
    *) exit 0 ;;   # extra repositorium
esac
# -posix ubi POSIX iure adhibetur (exclusiones NON consultae tunc,
# ne diagnostica VERA in illis plagulis sileant):
#   - probationes radicis (fixturae fork/fossae/sockets)
#   - lib/*_posix.c: nomen ipsum stratum platformae DECLARAT
#   - plagulae paucae quae tempus/fossas systematis legunt et olim
#     TOTAE excludebantur - iudicium rectum exclusioni caecae praestat
#     (2026-07-24: quinque earum sub -posix MUNDAE sunt)
# CAUTIO: hic solum plagulae quae sub -posix VERE mundae sunt.
# tcp_posix.c deest CONSULTO - lexicon superficiem socketorum
# (sockaddr_in/addrinfo cum formis certificandis) nondum fert, ergo
# -posix ei XXXV violationes adhuc pareret. Exclusio eam tegat donec
# lexicon crescat aut donec blocum <externa> suum ferat.
#
# reactor.c ADDITA 2026-07-25: superficiem poll.h SUAM per blocum
# <externa caput="poll.h"> fert (lib/reactor.c), ergo sub -posix
# MUNDA est (XI violationes -> ZERO). Ex CAECA iudicata facta.
#
# NOTA DE DIVERGENTIA (01KYBAG1MJ): reactor.c in exclusiones.txt
# MANET quia sweep corporis (percursus) -posix omnino non routat -
# ergo uncus eam mundam, sweep REICE vocat. Hoc consulto factum est:
# alterum erat eam CAECAM relinquere. Quarta plagula huius classis
# (cum filum/via/uuid); sanabitur cum routing unificabitur.
POSIX_VEX=""
case "$REL" in
    ./probationes/*) POSIX_VEX="-posix" ;;
    ./lib/processus_posix.c|./lib/moneta.c|./lib/fasti.c|./lib/filum.c|./lib/via.c|./lib/uuid.c|./lib/reactor.c)
        POSIX_VEX="-posix" ;;
esac

if [ -z "$POSIX_VEX" ]; then
    EXCL="$RADIX/silva/probationes/fixa/examinis/exclusiones.txt"
    [ -f "$EXCL" ] && grep -qxF "$REL" "$EXCL" && exit 0
fi

OUT=$(cd "$RADIX" && ./silva/examen.sh "$REL" $POSIX_VEX -machina 2>/dev/null)
ROWS=$(printf '%s\n' "$OUT" | awk -F'\t' \
    '$1!="VERDICTUM" && $4!="infra" {
        p = ($6=="1") ? " (provisionale)" : "";
        print $1":"$2":"$3": ["$4"] "$7 p
    }' | head -10)
[ -z "$ROWS" ] && exit 0
VERD=$(printf '%s\n' "$OUT" | awk -F'\t' '$1=="VERDICTUM"{print $2}')

jq -n --arg r "EXAMEN C89 (uncus post-editionem, verdictum ${VERD:-?}):
$ROWS
violatio = constrictio C89 (clang -pedantic-errors reiciet); suspectum = licitum sed suspectum (vocatio implicita etc.). Effusor: ./silva/examen.sh <plagula>. Detail: silva/phase-log.md M4a." \
    '{additionalContext:$r}'
exit 0
