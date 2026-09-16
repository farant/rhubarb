# crusta — orientation

*You are reading this because you touched a file in `crusta/`.*

Shell parser on **materia** (sixth client, after the C89 shim, css,
md, oratio and html): POSIX sh plus bash 5.2's whole default-option
syntax, total, byte-exact through the emitter, projected to STML,
judged by a canon, proven against bash's own deparse (`declare -f`)
and `bash -n`. "crusta" = shell (Latin; the legati doctrine's
"instrumentum crustae"). Design: `project-specs/crusta-arbor-spec.md`
(decisions C1–C15; §3 lexer, §4 declaration, §5 builder, §6
arithmetic, §7 cooked view and oracles, §8 gates; Fran's research
notes as Appendix A). Plan: `project-specs/crusta-arbor-plan.md`
(P1–P12, one commit each, every gate born red). Decrees: 01M2NJ16RG
(the heredoc body sits where its bytes are; the redirection names it
by `corpus:referentia`) and 01M2NJ1JR7 (the lector is a function of
mode and position; the builder owns the mode stack, iteratively).
Findings at find-time: `crusta/fontes/crusta_arbor.worklog.md` (from
P3 on).

## Praesens status (2026-09-16 — P6 done)

- **P6, cases and corpus:** `crusta_exempla.{h,c}` reads the Oils
  `#### titulus` / `## SECTIO:` … `## END` case format (copies into the
  piscina; CRLF kept in data) and carries the manifest
  `CRUSTA_FREEBSD_PLAGULAE` (89 scripts of FreeBSD `bin/sh/tests/
  parser`, BSD-2, `probationes/fixa/crusta/FONTES.md`, fetch approved by
  Fran). Fixtures `probationes/fixa/crusta/pathologiae.sh` (32 blocks:
  spec A.9 plus P6's additions) and `adversarius.sh` (17, CRLF
  throughout, everything left open, a 200-deep `$( )`, a backslash at
  EOF). Gate `exempla` (reader contract, both fixtures, the manifest
  pinned: 89 files, 17,297 bytes). Gate `corpus`: every tracked `.sh`
  (the RUNNER writes `build/crusta_corpus.lst` with `git ls-files`
  before the loop; a missing list is CREDO_CULPA) + fixtures + FreeBSD
  through parse → emit → memcmp, plus LECTOR COVERAGE through the
  builder's own pulls (`crusta_arbor_parsare_cum_lexematis` records
  every received token; the gate concatenates them): 231 house files,
  817,646 bytes, and 91 fixtures, all byte-identical, coverage clean,
  house corpus ZERO mala. What it found: (1) a command with only
  assignments kept INITIUM mode, where a newline is trivia, so the
  next line joined it — ~150 house files had mala at `then`/`do`/`(`
  until the mode after a first non-word child became ASSIGNATIONES;
  (2) FreeBSD heredoc14–16.0 put heredoc bodies after a loop's
  separator, a loop name and a case word: the declaration grew
  `iteratio.interiecta`, `electio.interiecta`, and the two
  `tok_separator` tokens became `separator` LISTS (separator node,
  then bodies; 152 loci, LOCI_NOMINATI regenerated, pin CLII); bodies
  in the gaps that keep no list (`for` before its name, a function's
  title/parens, inside `(( ))`/`[[ ]]`) land in the nearest list below
  out of byte order and are counted `heredoca_transposita` (sana FALSUM,
  a named limit, one case pinned). Plants: corpus — separator nodes
  dropped; exempla — `## END` kept in the section.
- **P5, the builder part two:** every compound command (`grex`,
  `crustula`, `conditio`/`ramus`, `iteratio`/`cyclus`/`repetitio`
  with `cursus`, `electio`/`optio`), the three function forms (`f()`,
  `function f`, `function f ()` — `name()` converts the one-word
  imperium in place, free because of the delayed append), `coproc`
  (`socius`, first word held as a candidate until the next token
  says title or command), `[[ ]]` (`iudicium` with its own four-level
  machine; `=~` pulls one REGULA token into a literal part), and the
  heredoc placement of decree 01M2NJ16RG: a body opens at the first
  newline token after its redirection (separator or trivium), the
  `heredoc` node is appended at once to the innermost SENTENCE LIST
  (programma, substitution, pipa/catena, a compound's list), the
  redirection gets `corpus` by referentia. Measured on bash 5.2:
  `$(cat <<A)` + newline reads the body OUTSIDE, a backtick never
  reads past its closing quote (empty body inside, closure absent).
  A frame's ROLE is (genus, locus, status): list / expecting / closed
  (status II = redirections, wrappable by `&&` `|`); a closing keyword
  finds the nearest open frame of its genus and closes everything
  above it absent (html's end-tag rule). Nineteenth lector mode
  `POST_COMPOSITUM` (bash recognises reserved words after `))` `]]`
  `}` `)`: `if ((x)) then`, `{ { a; } }`). `A=1 if` is a command named
  `if` (RESERVATUM as a literal part). Gate `arbor`: 87 cases, 752
  assertions; born red for P5 by sending every heredoc body to the
  program list (the two bodies after `cat <<A <<B |` then emit before
  the pipe). Known `sana` vs `bash -n` divergences: heredoc cut off by
  EOF and `[[ a b ]]` (bash warns, exits 0). Named corner: a heredoc
  whose newline lands in a loop separator or after a loop name or case word — RESOLVED in P6 by declaration.
- **P4, arithmetic (`crusta_arithmetica.{h,c}`):** the precedence
  machine (shunting-yard over two explicit stacks) that the BUILDER
  drives: every frame with a `machina` (`pars-arithmetica`,
  `arithmetica`, `inclusa`) hands leaves (`numerus`, `variabilis` with
  a raw subscript, word parts `$x` `${…}` `$( )`) to it as operands
  and `ARITHMETICA_OPERATOR` tokens as operators; closing a frame
  gives its node to the parent's machine when the parent has one.
  Table `CRUSTA_BASH.arithmetici`: 41 rows, 16 levels measured on
  bash 5.2 (prefix binds TIGHTER than `**`: `-2**2` is 4; `**`,
  ternary and assignments right-assoc). Totality: a missing operand
  is an absent locus (`1 +`), `?` without `:` a ternary without colon,
  `:` without `?` a binary. `$((` follows bash's rule: arithmetic
  first, and on a `)` at depth zero or a `;` the frames are dropped,
  the lector is rewound to the `$((` with `situs.arithmetica_recusata`
  set so it re-lexes as `$(` + `(`, and the opener's trivia return to
  the ligator. `(( ))` as a command is a sentence that stays pending
  after `))` for redirections (`_compositum_clausum`), so `&&`/`|`
  wrap it like an imperium. `crusta_arithmetica_aestimare` folds
  constant trees with 64-bit wrapping. Gate `arithmetica`: structure
  cases, the ambiguity both ways, 43 values from bash 5.2.15, six
  refusals; born red by swapping the precedence of `*` and `+`.

## P3 (2026-09-16)

- **P3, the builder part one (`crusta_arbor.{h,c}`):**
  `crusta_arbor_parsare(piscina, fons, mensura, dialectus, &relatio)` —
  iterative over a frame stack that IS the mode stack; `programma`,
  `imperium` (assignments positional, words as parts, redirections
  with the heredoc petition at the delimiter word), `catena`/`pipa`
  only when their operator is present (`!` and `time -p` open a pipa
  with `praefixa`), separators, `malum` for what the grammar cannot
  place (a separator with nothing pending is one too), EOF closes
  every frame with absent tokens and counts them. A SENTENCE NODE IS
  APPENDED TO ITS LIST WHEN ITS FRAME CLOSES, not when it opens (see
  the worklog: materia lists only append, so `&&`/`|` wrap the finished
  item instead of moving it). The ligator binds trivia backward through
  the last newline (C7). `CrustaParsura` reports mala, absent closures,
  max depth, heredoc petitions, `sana`. Accessors
  `crusta_verbum_staticum` (decodes quotes, escapes with backtick
  depth, `$'…'`), `crusta_verbum_citatum`, `crusta_imperium_titulus`,
  `crusta_imperium_argumenta`. Gate `arbor`: 39 cases through the byte
  emitter and structural asserts; born red by the ligator dividing
  after the first newline instead of the last. Intermediates until
  P4/P5: `$((` is a literal part (its `))` mala), reserved words other
  than `!`/`time`, `(`, `[[`, `((` are mala.
- Findings: `crusta/fontes/crusta_arbor.worklog.md`.

## P2 (2026-09-16)

- **P2, the lector (`crusta_lector.{h,c}`):** `crusta_lector_proximum(lector,
  modus)` — a function of mode and position (decree 01M2NJ1JR7);
  **18 modes** (the spec's 15 plus `TABULATUM` for the inside of an
  array literal `( … )`, and `EXPANSIO_EXEMPLAR` / `EXPANSIO_SECTIO`
  for the argument words of `${x/a/b}` and `${x:1:2}`, where `/` or `:`
  ends the word — the lector cannot know which operator opened the
  argument, the builder can). Tokens are raw-byte slices; the token's
  private tail `CrustaCauda.gravis` carries the backtick depth so a
  decoder can strip the backslashes bash strips (rule in the source
  header: toward `$` and `` ` `` a run halves per level, toward
  anything else it rounds up). Regions delimit heredoc and backtick
  bodies first; `crusta_lector_gravem_quaerere` finds a closing
  backtick at a depth; the heredoc queue is fed by the builder
  (`heredoc_petere`) after the delimiter word closes. Three
  continuation flags survive exactly one pull (`titulus_exspectatur`
  after `$`, `assignationis_gradus` after an assignment name or
  subscript). Named deviations found here: `\r` is an ordinary byte
  (bash's own behaviour: a CRLF script has words ending in `\r`; the
  newline token is always `\n` alone), and a `~` after `:` in an
  assignment value is literal bytes, not a `pars-domus`. Lexicon
  genus `ASSIGNATIO_SUBSCRIPTUM` renamed `SUBSCRIPTUM` (it serves
  expansions and arithmetic variables too). Gate `lector`: 47 cases
  pulled by explicit mode sequences, byte coverage per case; born red
  by a LAMINA one byte short.
- **Formator finding (quaestio 01M2NNE6WS):** a run of `#define`s
  between functions makes the formator indent the following function
  definition as a declaration group. Never put `#define`s between
  functions; the lector gate uses wrapper functions per mode instead.
- The named-slot table of the registrum gate is generated by
  `crusta/instrumenta/loci_nominati_generare.py` (`-genera` for the
  title list).

## P1 (2026-09-16)

- **Built:** declaration `crusta/grammatica/crusta.registrum.stml` →
  GENERATED `crusta_registrum_coctum.{h,c}` (50 genera, 150 loci; one
  `referentia` locus, `redirectio.corpus`); hand slot enums and the
  dialect tables `CrustaDialectus` / `CRUSTA_BASH` in
  `crusta_registrum.h` (reserved words, assignment builtins, redirect
  operators, case terminators, `[[ ]]` operator sets; the arithmetic
  operator table is EMPTY until P4); `crusta_lexicon` (50 token genera,
  prefix `crusta-`; the newline is TWO genera, `SEPARATOR_LINEAE`
  substantive and `LINEA` trivium, chosen by the lector's mode; the
  `LINEA` munus is PRESENT, md's answer, not html's); the runner.
- **Gates (1 of 14, `./crusta/compile_probationes.sh`):** registrum
  (generated tables byte-equal to a fresh render, lexicon order by
  title, 150 named slots against the table, dialect tables
  NIHIL-terminated and duplicate-free, one minimal tree through
  writer → reader → writer). Born red: two lexicon rows swapped.
- **Next:** P2 the lector (`crusta_lector`, 15 modes, regions, the
  heredoc queue).

## Laws (spec §0)

- **Absent slots are meaning** (html's H4): no synthetic tokens, ever.
- **Newline by mode** (C6): separator where it terminates something,
  trivium elsewhere; the lector's mode decides, never a relabelling.
- **Trivia bind backward through the last newline** (C7): a trailing
  comment belongs to its line; a heredoc body follows the newline.
- **Totality** (C12): every byte lands in the tree; a token the
  grammar cannot place is a `malum`; validity is a later judgment.
- **Nothing reserved** (C13): extension = declaration append + seal
  move; sub-parsers over words are a named future (spec §9).

## The registry is generated

`CrustaGenus`, `CRUSTA_GENUS_NUMERUS_GENERUM`, `CRUSTA_REGISTRUM` come
from `crusta/grammatica/crusta.registrum.stml` via `./materia/coquere.sh
crusta/grammatica/crusta.registrum.stml -scribere`. To add a genus or
locus: append to the declaration (locus order = emission order = byte
order), run `-scribere`, add the slot enum in `crusta_registrum.h`, the
row in `LOCI_NOMINATI` (registrum probatio, generated from the
declaration by the P1 script — regenerate it the same way), the rule in
`crusta.canon` (P8), and move the seal there by hand with a cause. Node
sizes come from `CRUSTA_REGISTRUM.genera[g].loci_numerus`, never a hand
count. A `nota` in the declaration is copied VERBATIM into the
generated C comments: write notes in words, never XML entities.

## Currere

```
./crusta/compile_probationes.sh            # omnes
./crusta/compile_probationes.sh registrum  # filtrum substringae
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/crusta.log`.
The oracle binary is `/opt/homebrew/bin/bash` (5.2.15); Apple's
`/bin/bash` is 3.2 and prints a different deparse.
