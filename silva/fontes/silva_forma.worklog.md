# silva_forma.c - worklog

## 2026-10-26 — T1: fragment parsing (transformare-plan.md)

`silva_formam_parsare(piscina, textus, mensura, EXPRESSIO|SENTENTIA, &causa)`: holes `$x` ->
`__trf_x`, `$...r` -> `__trfv_r` (string/char literals and /* */ comments untouched; a name used
both ways is refused), wrapped as `vacuum __trf_forma(vacuum) { FRAG; }`, parsed with the compiled
Latin lexicon (`silva_contextus_latinam_addere` - no file reads), then walked by SLOT NAME
(`silva_nodi_locus`, over SILVA_C89_REGISTRUM) to the one statement or its `expressio`.

Things learned on the way (all now in code comments):
- The commission root (`parsura->commissio->radix`) is a LISTA of top-level items (directives,
  latina's declarations, the wrapper definition, the tail) - not a node.
- `argumenta` of a call is a LISTA alternating argument nodes and comma tokens, even for one
  argument - the `$...` hole will match a list suffix.
- An EXPRESSIO wrapper adds `;`, so a statement fragment given as EXPRESSIO parses as statement +
  `sententia-vacua`; empty statements are not counted, so the cause reads "non est expressio".
- Kind names are Latin (`si`, `vocatio`, `binarium` with `sinister`/`dexter`, `folium-integer`);
  ZEPHYRUM arrives as `folium-integer` (post-expansion tree).
Test: silva/probationes/probatio_silva_forma.c (4 sections, 37 assertions). Plants: literal skipping
removed, empty statement counted, '$...' not recognized - each red.
New silva/fontes file -> fontes_generata.h and the amalgam regenerate (fabrica links the amalgam:
rebuild bin/fabrica after).

## 2026-10-26 — T2: matching (silva_forma_congruit, silva_nodi_aequales)

Recursive comparison of MateriaValor pairs: NODUS = same kind + same slot count + slot-wise; TOKEN =
equal `valor` (trivia lives on the tokens, never in valor, so spacing and comments are ignored for
free; post-expansion values make si/if and ZEPHYRUM/0 equal); LISTA = element-wise; REFERENTIA
(identifier -> declaration links) ignored. A `forma_modus` flag separates matching (holes are
wildcards) from pure equality (`silva_nodi_aequales`: hole names are ordinary identifiers).
`$x` captures the SOURCE node itself (pointer identity, tested); a repeated `$x` needs an equal
subtree - and captures are now always tracked internally, because with capturae==NIHIL the
repeated-hole rule was silently skipped (caught by the 'x[i] + x[j]' test). `$...r` must be the
last list element and captures the rest (MateriaValor elements, commas included); when the source
list is exhausted, the comma before it matches empty too, so `fopen($p, $...r)` matches
`fopen(a)` but not `fopen()`.

T1 fix found here: an EXPRESSIO fragment `fopen(via)` wrapped as `fopen(via);` parsed as a
DECLARATION (variable `via` of type `fopen` - no prototype in the wrapper). Expressions are now
wrapped as `redde FRAG;` and read from the redde node's `valor` slot - a return value can only be
an expression. SENTENTIA patterns keep the ambiguity for bare calls (documented LIMES in the
header): write those as EXPRESSIO patterns; T3/T4 match them against the expression inside the
statement. The sententia-vacua skip became dead code and was removed.
Tests: probatio_silva_forma V-VIII (72 assertions total). Plants red: token values not compared,
repeated hole without equality, empty-rest comma rule removed, equality treating holes as
wildcards, expression wrapper without 'redde'.

## 2026-10-26 T3 situs: census of call sites by consumer

`silva_situs_invenire` walks the CANONICAL spine only: at an ambiguus
node (commissio->genus_ambigui) descend into interpretationes[canonica],
never all branches. Two traps found while testing:

- A fixture with `remove(v);` never exercises the ambiguous path: the
  oracle already knows `remove` (system lexicon) at commit time, so the
  ambiguity COLLAPSES and no ambiguus node survives. A plant that forced
  interpretation 0 stayed green. Surviving ambiguity needs a name the
  oracle does not know at commit time: an undeclared `ignota(v);`
  survives and policy X8 makes the EXPRESSION reading canonical (index
  1, not 0). The test asserts that case; the plant is red now.
- Without the oracle (plain silva_c89_parsare, arbor.sh) `remove(v);`
  canonical = the declaration reading. Census must use the examen
  recipe (silva_iudicium), never a bare parse.

in_macro = the call's `(` or `)` is not a fons token of the principal
file. The NAME may be expanded (object-like alias `imprimere` ->
printf is still the file's own call), but parens from a macro body
(`DELE(v)`) or from a macro ARGUMENT copy (`VOCA(remove(v))`: argument
tokens are copies with expansio origin) are not rewritable. macro =
outermost name along the origin chain until the invocation is fons.

Consumer = first ancestor that USES the value: parentheses, ambiguus and
`(vacuum)` casts are see-through (a void cast discards the value; the
spec example `(vacuum)remove($1);` is a statement row). A
declarator-initiatus climbs to its declaratio so the row reads
`s32 r = remove($1);`.

Key vs spelling: key = consumer genus + post-expansion token values
(ZEPHYRUM and 0 group together, matching how T2 patterns match);
spelling = the consumer's SOURCE BYTES with argument extents -> $k and
statement bodies -> `...`, comments dropped, whitespace collapsed,
string literals verbatim. Token-radix spelling was rejected: a
function-like macro's tokens all share the invocation radix, so
`MACRO(y)` would print as `MACRO y`. LIMES kept: a function-like macro
invocation at the END of a consumer ends the extent before its `)`.

Census run 2026-10-26: remove over lib/tools/probationes = 58 sites in 3
shapes, 82 s for 1372 files, matches git grep exactly (grep's extras:
5 comments + 2 calls inside silex.c string literals; census extras: 3
calls on one line). fopen over 8 trees = 264 sites, 35 shapes (mostly
variable names), 14 via APERIRE in natura_examen.c, 1 knotapel snapshot
RECUSO (examen size limits).
