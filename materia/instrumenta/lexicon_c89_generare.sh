#!/bin/bash
# lexicon_c89_generare.sh - Descriptorem lexicalem C89 ex fontibus
# silvae generare.
#
# PHASIS I SOLUM. Hoc scriptum exsistit ut tabula generum XCV
# TRANSCRIBI non debeat: transcriptio manualis est ipsum genus
# erroris quod descriptor tollere debet. Faber (phasis II) hoc
# substituet, ex definitione linguae generans potius quam ex
# silva legens.
#
# Fontes veritatis (silva, gelata):
#   silva/fontes/silva_token.h   enumeratio -> ordo + nomina
#   silva/fontes/silva_arbor.c   ORTHOGRAPHIAE -> textus fixus
#
# Regulae speciei (ex silva_arbor.c:1022-1046, _trivium_scribere):
#   EOF                      -> FIXUM ""      (contentum nullum umquam)
#   SPATIA, TABULAE          -> REPETITUM     (n=" " numerum ferunt)
#   NOVA_LINEA, CONTINUATIO  -> TERMINATOR    (vexillum crlf ferunt)
#   orthographia adest       -> FIXUM
#   aliter                   -> VERBATIM
#
# Regulae muneris (ex silva_nodus.c:900-918 et silva_arbor.c:3527):
#   EOF -> FINIS; SPATIA/TABULAE -> SPATIUM; NOVA_LINEA -> LINEA;
#   CONTINUATIO -> LAMINA (lineam LOGICAM non incipit);
#   COMMENTUM_* -> COMMENTUM; ceterum -> SUBSTANTIVUM

set -eu

RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TOKEN_H="$RADIX/silva/fontes/silva_token.h"
ARBOR_C="$RADIX/silva/fontes/silva_arbor.c"
EXITUS_DIR="${1:-$RADIX/materia/probationes}"

for f in "$TOKEN_H" "$ARBOR_C"; do
    [ -f "$f" ] || { echo "DEEST: $f" >&2; exit 1; }
done

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

# ordo enumerationis
awk '/^nomen enumeratio \{/,/\} SilvaLexemaGenus;/' "$TOKEN_H" \
  | grep -o "SILVA_LEX_[A-Z_0-9]*" | grep -v NUMERUS_GENERUM > "$TMP/ordo"

# ORTHOGRAPHIAE: genus -> literale (aut NIHIL)
awk '/^\} ORTHOGRAPHIAE\[/{intus=1; next}
     intus && /^\};/{intus=0}
     intus && /^ *\{ *SILVA_LEX_/ {
       line=$0
       sub(/^ *\{ */,"",line)
       g=substr(line, 1, index(line,",")-1); gsub(/ /,"",g)
       rest=substr(line, index(line,",")+1)
       sub(/^ */,"",rest)
       # ANCORA AD FINEM LINEAE: sub(/ *\}.*$/) orthographiam "}"
       # (BRACE_CLAUSA) truncabat - primum } inventum, non ultimum.
       sub(/[ \t]*\},?[ \t]*$/,"",rest)
       print g "\t" rest
     }' "$ARBOR_C" > "$TMP/orth"

N_ORDO=$(wc -l < "$TMP/ordo")
N_ORTH=$(wc -l < "$TMP/orth")
if [ "$N_ORDO" -ne "$N_ORTH" ]; then
    echo "DISCREPANTIA: enumeratio $N_ORDO, ORTHOGRAPHIAE $N_ORTH" >&2
    exit 1
fi
# ORTHOGRAPHIAE ordinem enumerationis sequi DEBET (silva id ipsa
# custodit, silva_arbor.c:336) - hic quoque probatur, ne generatio
# tacite permutet.
if ! diff -q <(cut -f1 "$TMP/orth") "$TMP/ordo" >/dev/null; then
    echo "ORTHOGRAPHIAE ordinem enumerationis NON sequitur" >&2
    exit 1
fi


# ---- caput: enumeratio indicum + declaratio ----
{
cat <<'HCAPUT'
/* lexicon_c89.h - Indices generum lexicalium C89
 *
 * GENERATUM per materia/instrumenta/lexicon_c89_generare.sh.
 * NOLI MANU EMENDARE.
 *
 * Ordo enumerationis ordinem SilvaLexemaGenus EXACTE sequitur -
 * id est quod shim phasis I gratis reddit: index unus utrimque
 * idem genus nominat, ergo conversione non opus est.
 */

#ifndef LEXICON_C89_H
#define LEXICON_C89_H

#include "materia_lexicon.h"

nomen enumeratio {
HCAPUT
awk '{ t=$1; sub(/^SILVA_LEX_/, "", t); printf("    LEXICON_C89_GENUS_%s%s\n", t, (NR==1 ? " = 0," : ",")) }' "$TMP/ordo"
cat <<'HCAUDA'

    LEXICON_C89_NUMERUS_GENERUM
} LexiconC89Genus;

externus constans MateriaLexiconCoctum LEXICON_C89;

#endif /* LEXICON_C89_H */
HCAUDA
} > "$EXITUS_DIR/lexicon_c89.h"

{
cat <<'CAPUT'
/* lexicon_c89.c - Descriptor lexicalis C89 pro materia
 *
 * GENERATUM per materia/instrumenta/lexicon_c89_generare.sh.
 * NOLI MANU EMENDARE - fontes sunt silva_token.h (enumeratio) et
 * silva_arbor.c (ORTHOGRAPHIAE).
 *
 * SEDES TEMPORARIA. Hic descriptor ad frontem C89 pertinet, quae
 * in silva vivit; in probationibus materiae vivit dum phasis I
 * currit, quia vehiculum portae est (shim C89 -> materia). Phasis
 * V eum in silvam movet. Materia ipsa lingua CARERE debet (M8).
 */

#include "lexicon_c89.h"

hic_manens constans MateriaLexGenus GENERA_C89[] = {
CAPUT

paste "$TMP/ordo" <(cut -f2 "$TMP/orth") | awk -F'\t' '
function spec(g, o) {
  if (g == "SILVA_LEX_EOF")                                    return "FIXUM"
  if (g == "SILVA_LEX_SPATIA" || g == "SILVA_LEX_TABULAE")     return "REPETITUM"
  if (g == "SILVA_LEX_NOVA_LINEA" || g == "SILVA_LEX_CONTINUATIO") return "TERMINATOR"
  if (o != "NIHIL")                                            return "FIXUM"
  return "VERBATIM"
}
function mun(g) {
  if (g == "SILVA_LEX_EOF")                                    return "FINIS"
  if (g == "SILVA_LEX_SPATIA" || g == "SILVA_LEX_TABULAE")     return "SPATIUM"
  if (g == "SILVA_LEX_NOVA_LINEA")                             return "LINEA"
  if (g == "SILVA_LEX_CONTINUATIO")                            return "LAMINA"
  if (g ~ /^SILVA_LEX_COMMENTUM_/)                             return "COMMENTUM"
  return "SUBSTANTIVUM"
}
function orth(g, o,   s) {
  s = spec(g, o)
  if (s == "VERBATIM") return "NIHIL"
  if (o != "NIHIL")    return o
  if (g == "SILVA_LEX_EOF")         return "\"\""
  if (g == "SILVA_LEX_SPATIA")      return "\" \""
  if (g == "SILVA_LEX_TABULAE")     return "\"\\t\""
  if (g == "SILVA_LEX_NOVA_LINEA")  return "\"\\n\""
  if (g == "SILVA_LEX_CONTINUATIO") return "\"\\\\\\n\""
  return "NIHIL"
}
{
  g = $1; o = $2
  titulus = g; sub(/^SILVA_LEX_/, "", titulus)
  printf("    { \"%s\",%*s%s,%*sMATERIA_LEX_%s,%*sMATERIA_MUNUS_%s },\n",
         titulus, 26 - length(titulus), "", orth(g, o),
         22 - length(orth(g, o)), "", spec(g, o),
         13 - length(spec(g, o)), "", mun(g))
}'

cat <<'CAUDA'
};

constans MateriaLexiconCoctum LEXICON_C89 = {
    GENERA_C89,
    (i32)(magnitudo(GENERA_C89) / magnitudo(GENERA_C89[0])),
    "lex-",
    (s32)LEXICON_C89_GENUS_SPATIA
};
CAUDA
} > "$EXITUS_DIR/lexicon_c89.c"


# ---- PORTA: praedictionem contra oraculum INDEPENDENS probare ----
# _textum_fert() in silva/instrumenta/silva_canon_coquere.c lista
# MANU SCRIPTA est, in plagula quam regulae supra numquam legunt.
# Praedictio: species==VERBATIM eam EXACTE aequat. Si divergunt,
# aut modellum falsum est aut silva mutata - utrumque generationem
# tacite procedere NON debet.
CANON_C="$RADIX/silva/instrumenta/silva_canon_coquere.c"
if [ -f "$CANON_C" ]; then
    awk '/^_textum_fert/,/^\}/' "$CANON_C" \
      | grep -o "SILVA_LEX_[A-Z_0-9]*" | sed 's/^SILVA_LEX_//' | sort > "$TMP/fert"
    grep '^    {' "$EXITUS_DIR/lexicon_c89.c" | grep 'MATERIA_LEX_VERBATIM' \
      | sed 's/^    { "\([A-Z_0-9]*\)".*/\1/' | sort > "$TMP/verbatim"
    if diff -q "$TMP/fert" "$TMP/verbatim" >/dev/null; then
        echo "  porta: species==VERBATIM == _textum_fert ($(wc -l < "$TMP/verbatim" | tr -d ' ') genera) OK"
    else
        echo "PORTA FRACTA: species==VERBATIM a _textum_fert divergit" >&2
        diff "$TMP/fert" "$TMP/verbatim" >&2
        exit 1
    fi
else
    echo "CAUTIO: $CANON_C deest - porta praedictionis MORTUA" >&2
fi

echo "generatum: $EXITUS_DIR/lexicon_c89.{h,c} ($(grep -c '^    {' "$EXITUS_DIR/lexicon_c89.c") genera)"
