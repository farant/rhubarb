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
    *.c|*.m) ;;
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
# Plagulae .m: examen ObjC non iudicat (silva dialectum non discet) -
# censor gradus MACRO est, ergo solus iudex domesticus earum
# (01KXZYG2SE). Suspecta tacent (macra in fracturis ObjC = usus
# legitimus); push solum [CENSURA] aut [CULPA PROBABILIS]. NB porta
# in TEXTU stat: censor PURUS + exitus 0 etiam cum convictione.
case "$FILE" in
    *.m)
        OUT=$(cd "$RADIX" && ./silva/censor.sh "$REL" 2>/dev/null)
        ROWS=$(printf '%s\n' "$OUT" \
            | grep -E '^\[(CENSURA|CULPA PROBABILIS)\]' | head -10)
        [ -z "$ROWS" ] && exit 0
        jq -n --arg r "CENSOR LATINAE (uncus post-editionem, .m - macra reservata):
$ROWS
Verbum latina.h ut identificator = fractura compilationis cryptica. Effusor: ./silva/censor.sh <plagula>." \
            '{additionalContext:$r}'
        exit 0
        ;;
esac

# TABULA ROUTING -posix DELETA 2026-07-25 (01KYBAG1MJ, 01KYB3NNR4).
# examen sectiones lexici EX INCLUSIONIBUS plagulae ipsius derivat -
# '#include <sys/select.h>' est dictum "superficie select utor" -
# ergo lista hic nulla opus est, et illa quae hic vivebat causa
# divergentiae erat (LSP/percursus eam non legebant).
# Mensuratum ante deletionem: derivatio lexicon PLENUM aequat in
# omnibus XXXII plagulis quae capita POSIX includunt, et omnes VIII
# probationes + omnes VI plagulae quae hic routabantur ACCIPE sunt
# sine vexillo ullo.
OUT=$(cd "$RADIX" && ./silva/examen.sh "$REL" -machina 2>/dev/null)
VERD=$(printf '%s\n' "$OUT" | awk -F'\t' '$1=="VERDICTUM"{print $2}')
# RECUSO = fines tactae (expansio trunca): zero ordines NON est
# munditia - silentium hic mentiretur (01KXS2ETAE)
if [ "$VERD" = "RECUSO" ]; then
    jq -n --arg r "EXAMEN C89 (uncus post-editionem): VERDICTUM RECUSO - plagula limina silvae excedit (expansio trunca), iudicium fidele impossibile. Iudex efficax solus = clang -std=c89 -Werror." \
        '{additionalContext:$r}'
    exit 0
fi
ROWS=$(printf '%s\n' "$OUT" | awk -F'\t' \
    '$1!="VERDICTUM" && $1!~/^#/ && $4!="infra" {
        p = ($6=="1") ? " (provisionale)" : "";
        print $1":"$2":"$3": ["$4"] "$7 p
    }' | head -10)
[ -z "$ROWS" ] && exit 0

# Exclusiones POST iudicium consuluntur, non ANTE (mutatio
# 2026-07-25). Antea plagula exclusa numquam iudicabatur, ergo
# plagula quae munda FIEBAT (derivatione aut bloco <externa>) tacita
# manebat donec quis listam manu emendaret. Nunc exclusio STREPITUM
# silet, numquam verdictum mundum - et plagula sanata statim iudicari
# incipit sine ulla lista tangenda. Baselinea corporis (percursus)
# intacta manet, quae eandem plagulam adhuc legit.
EXCL="$RADIX/silva/probationes/fixa/examinis/exclusiones.txt"
# forma pinnae: via<TAB>causa ('#' commentaria) - columna prima sola
[ -f "$EXCL" ] && grep -v '^#' "$EXCL" | cut -f1 | grep -qxF "$REL" && exit 0

jq -n --arg r "EXAMEN C89 (uncus post-editionem, verdictum ${VERD:-?}):
$ROWS
violatio = constrictio C89 (clang -pedantic-errors reiciet); suspectum = licitum sed suspectum (vocatio implicita etc.). Effusor: ./silva/examen.sh <plagula>. Detail: silva/phase-log.md M4a." \
    '{additionalContext:$r}'
exit 0
