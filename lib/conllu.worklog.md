# conllu worklog

## 2026-10-17 — moved into the house tier (knotapel D130 T1a)

Was `oratio/fontes/oratio_conllu.{h,c}` (oratio T13, T38 a: see
`oratio/fontes/oratio_arbor.worklog.md` and
`oratio/fontes/oratio_oraculum.worklog.md` for its history). Moved by
Fran's decision 4 (a) in `knotapel/demo_130_latin_tangles/plan.md`:
archive.sh's conflator accepts closure files only from include/, lib/
and the client roots (materia/fontes, toml/fontes), so a knotapel demo
could not freeze with oratio's reader in its closure; and a data-format
reader is house-tier material like json and toml.

Changes, behaviour otherwise identical:

- prefix `oratio_` dropped (conllu_legere, conllu_textus,
  conllu_spatium_post; ConlluLexema, ConlluSententia);
- own error type `ConlluVitium { linea, causa }` instead of
  OratioVocabulariumVitium, which pulled oratio_vocabularium.c (1,380
  lines) into every closure. Consumers only ever read linea and causa;
- `ConlluLexema.caput`: HEAD parsed (-1 for '_' or non-numeric, at most
  nine digits) - no new reader errors, a bad head surfaces in
  `dependentia_validare` via `conllu_arbor` (mapped to n + 1, i.e.
  CAPUT_EXTRA, not to 0 which would read as a second root);
- `conllu_proprietas(feats, "Case")`: the key must match whole and be
  followed by '=' ("Case" never matches "Case[psor]");
- `conllu_arbor`: syntactic words (ranges skipped) as a
  DependentiaArbor plus `verba[k - 1]` back to each word's lexeme;
  FALSUM if IDs are not 1..n consecutive.

oratio consumers switched (oratio_oraculum.{h,c}, instrumenta/oraculum.c
with its own `vitium_conllu`, probatio_oratio_oraculum.c); oratio's
compile_probationes.sh hand list gained dependentia + conllu (its tools
build from include closure). The reader's own checks moved from
probatio_oratio_oraculum.c section I to `probationes/probatio_conllu.c`
(oratio keeps the genre-of-sentence checks that use the parse); oratio
oraculum suite green (1,630) and its per-file coverage pins unchanged.

Suite: 7 sections, 109 checks, including the five vendored treebanks
(sentences, syntactic words, ranges equal FONTES.md; every sentence a
sound tree). Plants: 5, all red. Clean under ASan/UBSan + poisoned pool
(by hand; not in venenum, whose closures stay math-tier).
