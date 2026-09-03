#!/bin/bash
# silva/scribe.sh - 'cat' quod examen currit (desideratum 01M1KZWVCV, Fran)
#
# Usage:
#   ./silva/scribe.sh <via> [-tacitus] <<'EOF' ... EOF     (stdin -> via)
#   scribe <via>                                           (per ~/.bin)
# Exit:  0 ACCIPE | 1 REICE | 2 usus, scriptura fracta aut INFRA |
#        3 RECUSO (fines examinis) | 4 SCRIPTUM SED NON IUDICATUM
#        (suffixum non .c/.h, aut via extra repositorium)
#
# CUR: plagula nova per heredoc scripta ansam examinis praeterit
# (uncus Edit non ardet, Editio.applicare() non vocatur) - examen eam
# in commissione primum videt. Hic scriptura et iudicium unum
# vocamen sunt. Plagula SEMPER scribitur (refusio clara, numquam
# plagula absens: agens eam videre et emendare debet); verdictum
# examinis exitum dat. Formator NON currit: commissio plagulas totas
# format (lex domus: scribe, committe, numquam forma inter editiones).
#
# Viae relativae ad DIRECTORIUM VOCANTIS resolvuntur (ut cat), non
# ad radicem - contra examen.sh/selecta.sh quae radicem poscunt.
#
# IUDICAT LEGEM C89 (examen), NON compilabilitatem: -Wsign-compare,
# trigrapha, typi absentes clang in cursu suitae capit. Sumptus: ~0.6 s.
set -u
VOCANS="$PWD"
SILVA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"

VIA=""
TACITUS=0
for a in "$@"; do
    case "$a" in
        -tacitus) TACITUS=1 ;;
        -h|--help) sed -n '2,20p' "$0"; exit 2 ;;
        -*) echo "scribe: vexillum ignotum '$a'" >&2; exit 2 ;;
        *) if [ -n "$VIA" ]; then echo "scribe: via una sola" >&2; exit 2; fi
           VIA="$a" ;;
    esac
done
if [ -z "$VIA" ]; then
    echo "usus: scribe <via> [-tacitus] < textus" >&2
    exit 2
fi
case "$VIA" in
    /*) ABSOLUTA="$VIA" ;;
    *)  ABSOLUTA="$VOCANS/$VIA" ;;
esac
PARENS="$(dirname "$ABSOLUTA")"
if [ ! -d "$PARENS" ]; then
    echo "scribe: directorium parens absens: $PARENS (mkdir explicite)" >&2
    exit 2
fi

# scriptura - stdin totus, octetim
if ! cat > "$ABSOLUTA"; then
    echo "scribe: scriptura fracta: $ABSOLUTA" >&2
    exit 2
fi
ABSOLUTA="$(readlink -f "$ABSOLUTA")"   # normata (../ sublata) post scripturam
OCTETI=$(wc -c < "$ABSOLUTA" | tr -d ' ')
LINEAE=$(wc -l < "$ABSOLUTA" | tr -d ' ')
echo "scriptum: $VIA ($OCTETI octeti, $LINEAE lineae)"

# iudicium - .c/.h intra repositorium solum
case "$ABSOLUTA" in
    *.c|*.h) ;;
    *) echo "non iudicatum: suffixum non .c/.h (examen C89 solum)"; exit 4 ;;
esac
case "$ABSOLUTA" in
    "$RADIX_DIR"/*) RELATIVA="${ABSOLUTA#"$RADIX_DIR"/}" ;;
    *) echo "non iudicatum: via extra repositorium ($RADIX_DIR)"; exit 4 ;;
esac

cd "$RADIX_DIR" || exit 2
EFFUSUS="$(./silva/examen.sh "$RELATIVA" 2>&1)"
RC=$?
if [ "$TACITUS" -eq 1 ]; then
    echo "$EFFUSUS" | grep -E 'ACCIPE|REICE|RECUSO|INFRA' | tail -3
else
    echo "$EFFUSUS"
fi
case "$RC" in
    0) echo "examen: ACCIPE ($RELATIVA)" ;;
    1) echo "examen: REICE ($RELATIVA) - plagula scripta manet, emenda" ;;
    2) echo "examen: INFRA - apparatus (nihil iudicatum)" ;;
    3) echo "examen: RECUSO - extra fines examinis" ;;
    *) echo "examen: exitus ignotus $RC" ;;
esac
exit "$RC"
