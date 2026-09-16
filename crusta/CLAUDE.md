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

## Praesens status (2026-09-16 — P1 done)

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
