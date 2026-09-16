# crusta_arbor — the shell tree, the substrate's sixth client

*2026-09-16. Designed with Fran, the session after html's O7c closed.
Sixth materia client after the C89 shim, css, md, oratio and html;
sibling of `project-specs/html-arbor-spec.md`, whose as-built shape
(§11 there) this spec copies wherever the language allows. Grown from
Fran's research notes of 2026-09-15 ("Background Research: Ideas for a
Bash Parser"), reproduced as Appendix A with their status line kept:
the notes are research, this spec is the decisions.*

**What it is.** A parser for shell scripts — POSIX sh plus bash 5.2's
whole default-option syntax — as a materia tree: total, byte-exact
through the emitter, projected to STML and read back, judged by a
hand-written canon, benched, and proven against two oracles that need
no fetch: bash's own deparse (`declare -f`) for structure and `bash -n`
for validity. Consumers (a `.sh` formatter, a lint, argument grammars
of known commands, a shell) are later plans; this one delivers the
library and the proof.

**Why "crusta".** Latin for shell, and house vocabulary already (the
legati doctrine calls a shell tool an "instrumentum crustae"). `bash`
is unknown to the glossary and names a moving target; the client is
"POSIX plus an enumerated bash list", pinned to one bash version. The
dialect is a table inside the declaration, so a POSIX-only or zsh mode
later is a second table, never a rename.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| C1 | **Deliverable of this plan = library + oracle**, the html shape; consumers are later plans | Keeps the plan the size html's was; the formatter (arc 2) starts on a proven parser. Fran, 2026-09-16 (option a). |
| C2 | **Dialect = bash 5.2.15's default-option grammar, whole**; the census list is the first gate's floor | The nine constructs the house never uses cost eight table rows and one lexer mode; the deviation list then holds only what cannot be parsed statically (extglob, aliases, `set -o posix`, `eval` bodies). Fran: "we want to build a thorough bash parser as best we can" (option b). |
| C3 | **Corpus = the house scripts + FreeBSD `bin/sh/tests/parser/` vendored (BSD-2)**; Oils `spec/` waits for the tail | One ask before the fetch; bash's own tests are GPLv3 and are never vendored. Fran (option a). |
| C4 | **Pull lexer as a function of (mode, position); the builder owns the mode stack; the builder is iterative** | The research's §5.1 and the house depth law together. Recursive descent rejected (C stack under `$(` ×100,000); the md line model rejected (quotes, `$( )`, continuations and `\|` all cross lines). |
| C5 | **The heredoc body sits where its bytes are; the redirection names it by `corpus:referentia`** (decree) | Emission walks loci in byte order; a body under its redirection would print before the newline. html's `sedes` machinery, reused unchanged. |
| C6 | **Newline is a separator token only where it terminates something; elsewhere it is trivia**, and the lexer's mode decides which genus it emits | Two genera for the same bytes, chosen by the mode, never re-labelled afterwards. |
| C7 | **Trivia bind backward through the last newline** (the inverse of css's division rule) | A trailing comment belongs to its line; a heredoc body must follow the newline that precedes it (C5). |
| C8 | **`catena` and `pipa` exist only when their operator is present**; a lone `echo` is a bare `imperium` | No fictions: a pipeline of one command is not a pipeline. The canon names the union of list items once (`sententia`). |
| C9 | **Redirections on compound commands are a trailing `redirectiones` list on each compound genus** | Nine genera carry the locus; the alternative wraps every compound command in a node that usually holds nothing. |
| C10 | **Words are lists of parts, parsed at parse time; expansion does not exist** | The research's §5.3. Part tokens are contiguous slices, so no derivation machinery; escapes and continuations are their own parts so a static value is a fold over parts. |
| C11 | **Arithmetic is its own module with its own gate and a constant evaluator** | The one closed grammar in the language; bash's deparse prints arithmetic verbatim, so only evaluation proves the tree's shape. |
| C12 | **Totality answers error recovery**: every byte lands in the tree, a token the grammar cannot place is a `malum`, validity is a later judgment | A formatter continues for free; `bash -n` agreement is a pinned gate, not a parser mode. |
| C13 | **Nothing reserved in v1** (html §11.6's law) | Appending a locus is a declaration append and a seal move; no format migration exists. The sub-parser layer (§9) needs no locus today. |
| C14 | **Oracle goldens for the fixed cases; the house scripts are judged live by an instrument** | A golden keyed to files that change with every commit would go stale daily; the fixed cases (pathologies, FreeBSD) are stable. The house-corpus pins that need no bash (byte law, zero mala) stay in the gates. |
| C15 | **Two decrees at approval**: C5 and C4's mode ownership | Filed in the ledger 2026-09-16 against desideratum 01M2MC9M29. |

---

## 1. The census that chose the dialect (2026-09-16)

230 tracked `.sh` files (`git ls-files`), about 21,400 lines; shebangs
`#!/bin/bash` 177, `#!/usr/bin/env bash` 2, `#!/bin/sh` 9.

| construct | files | sites | | construct | files | sites |
|---|---|---|---|---|---|---|
| `[[ ]]` | 17 | 29 | | `$'…'` | 16 | 32 |
| `(( ))` / `$(( ))` | 49 | 135 | | `$"…"` | 5 | 13 |
| `a=( )` | 25 | 55 | | `<<<` | 5 | 15 |
| `${a[i]}` | 150 | 649 | | `<<` heredoc | 20 | 38 |
| `local` | 24 | 128 | | `${x:n:m}` | 56 | 101 |
| `declare`/`typeset` | 124 | 238 | | `${x#…}` `${x%…}` | 38 | 106 |
| `function` keyword | 2 | 7 | | `${x//…}` | 3 | 3 |
| `name() {` | 97 | 188 | | `${!x}` | 2 | 4 |
| `<( )` `>( )` | 24 | 37 | | brace expansion | 22 | 28 |
| `&>` `&>>` | 4 | 7 | | backticks | 3 | 25 |
| `case` | 69 | 111 | | `eval` | 5 | 5 |
| `until` | 1 | 2 | | `for (( ))` | 1 | 1 |

Never used: `|&`, `;;&`, `;&`, `<<-`, `{fd}>`, `select`, `time`,
`coproc`, `=~`, extglob, `let`, `alias`, `set -o posix`, `mapfile`,
CRLF. The only `shopt` in the house is `nullglob` (a runtime option),
so the parse-time grammar is fixed in every script. zsh syntax: none
(the zsh idioms in the house notes belong to the interactive shell).

Oracle binary: Homebrew bash 5.2.15 at `/opt/homebrew/bin/bash`.
Apple's `/bin/bash` is 3.2 and prints a different deparse; every gate
and instrument names the binary. No dash on the machine.

---

## 2. Architecture and files

```
crusta/
  grammatica/crusta.registrum.stml       the declaration (§4), loci in BYTE order
  grammatica/crusta.canon                hand-written, drift-guarded, seal pinned
  fontes/crusta_registrum_coctum.{h,c}   GENERATED by ./materia/coquere.sh — never hand-edited
  fontes/crusta_registrum.h              slot enums by hand; CrustaDialectus + CRUSTA_BASH tables
  fontes/crusta_lexicon.{h,c}            MateriaLexiconCoctum rows, prefix "crusta-"
  fontes/crusta_lector.{h,c}             the lexer: proximum(lector, modus), regions, heredoc reader
  fontes/crusta_arbor.{h,c}              the builder: frames, ligator, totality; verbum/imperium accessors
  fontes/crusta_arithmetica.{h,c}        (( )) expressions: shunting-yard + constant evaluator
  fontes/crusta_coctum.{h,c}             the cooked view = bash's declare -f normal form
  fontes/crusta_exempla.{h,c}            reader for '#### name' case files (pathologies, goldens, FreeBSD)
  fontes/crusta_computus.{h,c}           bench twin
  instrumenta/{arbor,coctum,computus,oraculum}.c
  probationes/probatio_crusta_*.c        fourteen gates (§8)
  probationes/fixa/computus/basis.tsv
  compile_probationes.sh · arbor.sh · coctum.sh · computus.sh · oraculum.sh
probationes/fixa/crusta/
  pathologiae.sh                         the research appendix's snippets + additions, '#### title' blocks
  adversarius.sh                         CRLF, unclosed everything, NUL inline in the probatio
  oraculum/expectata.txt                 declare -f goldens (bash version in the header)
  oraculum/validitas.txt                 bash -n goldens
  freebsd/…  + FONTES.md                 the vendored parser cases, origin, commit, licence text
```

Pipeline: bytes → `crusta_lector` (one token per pull, mode chosen by
the builder) → `crusta_arbor` (frame stack; binds trivia; builds
`MateriaNodus` against the generated registry) → `materia_scribere_nodum`
as the separating byte oracle → `materia_arbor_scribere_nodum` /
`_legere` / `_aequalis` (projection, two cycles, comparator) →
`crusta.canon`. The cooked view reads the finished tree. The runner
compiles `materia/fontes/*.c` into `crusta/build/` as html's does:
crusta consumes materia, never contains it; the substrate is expected
to need ZERO changes (html needed none) and any change goes through
`./materia/shim_probare.sh`.

Entry points:

```c
MateriaNodus* crusta_arbor_parsare (Piscina*, constans character* fons, i32 mensura,
                                    constans CrustaDialectus*, CrustaParsura* relatio);
/* relatio: mala counted, absent closing tokens counted, depth reached */
b32     crusta_verbum_staticum (Piscina*, constans MateriaNodus* verbum, chorda* valor);
        /* decoded value when no expansion part is present; FALSUM = not static */
constans MateriaNodus* crusta_imperium_titulus (constans MateriaNodus* imperium);  /* first verbum */
Xar*    crusta_imperium_argumenta (Piscina*, constans MateriaNodus* imperium);      /* verba after it */
chorda  crusta_coctum_scribere (Piscina*, constans MateriaNodus* radix);
b32     crusta_arithmetica_aestimare (constans MateriaNodus* expressio, s64* valor);
```

---

## 3. The lexer (`crusta_lector`)

**One function.** `crusta_lector_proximum(lector, modus)` returns the
next `MateriaToken`: a chorda slice into the source, byte offset,
1-based line and column, genus from the lexicon. The lexer's whole
state is the cursor, a region stack and the heredoc queue; the mode is
the whole context. Tokens are slices, so backtracking is resetting the
cursor (used once, §5 `$((`).

**Modes (15).**

| mode | asked when | what changes |
|---|---|---|
| `INITIUM` | command position (nothing pending) | reserved words recognised as a whole unquoted word from the dialect table; `name=` `name[…]=` `name+=` are assignment tokens; `((` and `[[` open sublanguages; `!`/`time` are pipeline prefixes; newline is TRIVIA |
| `VERBA` | after the command word | words are words (`if`, `a=b` are parts); newline is a SEPARATOR |
| `ASSIGNATIONES` | after an assignment builtin (`alias declare export local readonly typeset`) | as `VERBA` but assignments and `=(…)` arrays still recognised (bash's `PST_COMPASSIGN`) |
| `POST_TITULUM` | after `for`/`select`, after the loop name, after the `case` word | one word or reserved word (`in`, `do`, `((`); newline is trivia; no assignments |
| `IN_VERBIS` | the `for … in` word list | newline or `;` ends the list (the separator token) |
| `EXEMPLAR` | case pattern position | `(` optional open, `\|` alternates, `)` ends; newline trivia |
| `GEMINA` | inside `"…"` | only `\$ \` \" \\ \newline` escape; `$…` parts; `"` closes |
| `EXPANSIO` | inside `${` after the name | the operator set `:- := :? :+ : # ## % %% / // /# /% ^ ^^ , ,, @ *` |
| `EXPANSIO_VERBUM` | the argument word of `${x:-…}` | a word that ends at `}` (nested `${` counted) |
| `ARITHMETICA_SUMMA` | inside `$((`/`((`/`for ((` at paren depth 0 | C-like operators, numbers with bases, names, `<`/`>` comparisons; `))` is ONE closing token; `;` separates `for` clauses |
| `ARITHMETICA_INTRA` | the same inside `(` … `)` | as above; `)` closes an `inclusa`, `))` is two of them |
| `IUDICIUM` | inside `[[ ]]` | `&& \|\| ! ( ) < > = == != =~` and the `-x` tests are operators; `]]` closes; newline trivia |
| `REGULA` | right operand of `=~` | one regex word: `( ) \|` belong to it until blank, `&&`, `\|\|` or `]]`; quoted portions stay separate parts |
| `HEREDOC` | body lines, unquoted delimiter | parts as `GEMINA` but `"` is literal; a literal never crosses a line end |
| `HEREDOC_LITTERALE` | body lines, quoted delimiter | one literal token per body |

The builder picks `ARITHMETICA_SUMMA` or `_INTRA` from its own
`inclusa` depth: that is the parser-owned mode stack doing the work
bash's `parser_state` bitfield does.

**Regions.** Heredoc bodies and backtick bodies are DELIMITED FIRST and
parsed second (bash's own order): the lexer finds the end (the
delimiter line; the unescaped closing backtick) and pushes a region
limit; the builder parses inside it with the ordinary modes; `FINIS`
inside a region ends the region. `$( )` is parsed to find its end
(POSIX's order): there is no region, the builder's substitution frame
closes on the `)` that reaches it. A backtick region carries a DEPTH
`d`: inside it the lexer reads `\`×d followed by `` ` ``, `\` or `$` as
that byte, so `` `echo \$x` `` yields a `pars-parametrum` whose sigil
token is the two bytes `\$`, and nested backticks by escaping are the
next depth — no unescaped copy is ever parsed (the byte law).

**Token genera** (the lexicon rows, `crusta-` prefix; the exact list
is the lexicon file, asserted by title by the registrum gate):

| group | genera | species | munus |
|---|---|---|---|
| structure | `RESERVATUM`, `ASSIGNATIO_TITULUS`, `ASSIGNATIO_SUBSCRIPTUM`, `ASSIGNATIO_OPERATOR`, `REDIRECTIO_FD`, `REDIRECTIO_OPERATOR`, `PIPA`, `PIPA_ERRORIS` (`\|&`), `ET`, `AUT`, `SEPARATOR` (`;` `&`), `SEPARATOR_LINEAE`, `TERMINATOR_OPTIONIS` (`;;` `;&` `;;&`), `PARENTHESIS`, `PARENTHESIS_CLAUSURA`, `UNCUS`, `UNCUS_CLAUSURA`, `ARITHMETICA_APERTURA` (`((`), `ARITHMETICA_CLAUSURA`, `IUDICIUM_APERTURA` (`[[`), `IUDICIUM_CLAUSURA` | FIXUM (VERBATIM where the bytes vary: names, fd, subscript); `SEPARATOR_LINEAE` TERMINATOR `"\n"` | SUBSTANTIVUM |
| word parts | `LITTERALIS`, `EFFUGIUM`, `CONTINUATIO`, `SIMPLEX`, `GEMINA_APERTURA`, `GEMINA_CLAUSURA`, `EFFUGIA` (`$'…'`), `VERSA_APERTURA` (`$"`), `PARAMETRUM_SIGILLUM`, `PARAMETRUM_TITULUS`, `EXPANSIO_APERTURA` (`${`), `EXPANSIO_PRAEFIXUM`, `EXPANSIO_OPERATOR`, `EXPANSIO_CLAUSURA`, `SUBSTITUTIO_APERTURA` (`$(`, `` ` ``), `SUBSTITUTIO_CLAUSURA`, `ARITHMETICA_PARTIS_APERTURA` (`$((`), `PROCESSUS_APERTURA` (`<(` `>(`), `DOMUS` (tilde prefix) | VERBATIM / FIXUM | SUBSTANTIVUM |
| heredoc | `HEREDOC_DELIMITATOR` (the delimiter line) | VERBATIM | SUBSTANTIVUM |
| arithmetic | `NUMERUS`, `VARIABILIS`, `ARITHMETICA_OPERATOR`, `ARITHMETICA_SEPARATOR` (`;` in `for ((`) | VERBATIM / FIXUM | SUBSTANTIVUM |
| condition | `IUDICIUM_OPERATOR`, `REGULA` (the regex word) | VERBATIM | SUBSTANTIVUM |
| trivia | `SPATIUM` (blanks and tabs), `COMMENTUM` (`#` to line end, newline excluded), `LINEA` (the trivia newline), `LAMINA` (`\`+newline between words) | VERBATIM; `LINEA` TERMINATOR | SPATIUM / COMMENTUM / LINEA / LAMINA |
| end | `FINIS` | FIXUM `""` | FINIS |

`SEPARATOR_LINEAE` and `LINEA` are the same bytes with different roles
(C6). The capability question materia asks at load ("does the language
have a `LINEA` munus?") is answered YES here, as md answers it; the
stml gate measures what FIDELITAS makes of a language where the same
byte is sometimes substantive. If the answer is a refusal, it is
pinned by cause like html's.

**Words are runs.** A word is a maximal run of adjacent part tokens;
adjacency is byte offset (a `CONTINUATIO` part, `\`+newline inside a
word, keeps the run adjacent — between words the same bytes are a
`LAMINA` trivium, decided by the previous byte). `"a"b'c'` is one word
of three parts; `a b` is two words. The builder groups; the lexer never
says "word begins".

**Fd gluing** at word start in every word mode: digits or `{name}`
immediately followed by `<` or `>` are `REDIRECTIO_FD`; otherwise the
digits are a literal part (`x2>f` is the word `x2` then `>`).

**Parameter forms.** `$` followed by a name, a digit, or one of
`@ * # ? - $ ! 0` is `PARAMETRUM_SIGILLUM` + `PARAMETRUM_TITULUS`
(`$10` is `$1` then the literal `0`); a lone `$` is a literal.
Inside `${`: `#` followed by a name char is `EXPANSIO_PRAEFIXUM`,
`#` followed by `}` is the title `#`; `!` likewise.

**The trivia rule (C7).** Pending trivia are divided at the LAST
newline trivium: everything up to and including it binds `post` to the
previous significant token; the remainder (the next line's leading
blanks) binds `ante` to the next token. Trivia before the first token
of the file (the shebang, blank lines) are all `ante` of that token;
trivia before `FINIS` go to it and land in `programma.cauda`.

**Heredocs.** On `<<` or `<<-` the lexer reads the delimiter word,
records (delimiter bytes with quotes removed, quoted?, strip-tabs?) in
its queue, and continues on the line. The builder, at the first newline
token it receives afterwards (separator or trivium; a newline inside
quotes is part bytes and never reaches the builder as a token), calls
`crusta_lector_heredoc(lector)` once per queued entry, in order: the
body comes back as part tokens (`HEREDOC`) or one literal
(`HEREDOC_LITTERALE`), then `HEREDOC_DELIMITATOR` (the delimiter line
without its newline), then the newline ending it as a `SEPARATOR_LINEAE`
token that the `heredoc` node holds in `tok_finis`. Both are absent
when the body runs to EOF (bash's "delimited by end-of-file"). `<<-`
keeps the leading tabs in the tokens (the cooked view strips them) and
matches the delimiter line with its tabs stripped. A `<<` whose line
ends inside a still-open quote waits for the next unquoted newline
(POSIX: "after the next NEWLINE token").

---

## 4. The node vocabulary (the declaration)

Loci in byte order (`materia_scribere_nodum` walks loci in registry
order). An absent slot is meaning (html's H4): no synthetic tokens,
ever. Genera are appended, never reordered; the seal in `crusta.canon`
moves by hand with a cause. 50 genera, 152 loci (150 at P1; P6 added
`iteratio.interiecta` and `electio.interiecta` and turned the two
`tok_separator` tokens into `separator` lists — the FreeBSD corpus put
heredoc bodies in those gaps).

```xml
<registrum grammatica="crusta" praefixum="CRUSTA" typus="CrustaGenus"
  sedes="crusta/fontes">

  <!-- programma et listae: nulla genus 'lista' - lista est locus liberi
       quilibet, ordine octetorum: sententiae, separator, heredoc, malum -->
  <genus titulus="programma">
    <locus titulus="liberi" species="lista-nodus"/>
    <locus titulus="cauda"  species="token"/>          <!-- FINIS -->
  </genus>
  <genus titulus="separator">                          <!-- ; & aut linea nova terminans -->
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="malum">                              <!-- lexemata quae grammatica ponere non potuit -->
    <locus titulus="tokens" species="lista-token"/>
  </genus>
  <genus titulus="heredoc">                            <!-- corpus ubi octeti iacent (C5); id= scopus referentiae -->
    <locus titulus="partes"          species="lista-nodus"/>
    <locus titulus="tok_delimitator" species="token"/>
    <locus titulus="tok_finis"       species="token"/>
  </genus>

  <!-- sententiae -->
  <genus titulus="catena">                             <!-- pipa|imperium, operator(&&,||), ... -->
    <locus titulus="liberi" species="lista-nodus"/>
  </genus>
  <genus titulus="pipa">                               <!-- praefixa: time, -p, ! -->
    <locus titulus="praefixa" species="lista-token"/>
    <locus titulus="liberi"   species="lista-nodus"/>  <!-- imperium, operator(|,|&), ... -->
  </genus>
  <genus titulus="operator">                           <!-- operator nudus intra listam -->
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="imperium">                           <!-- assignatio | verbum | redirectio, ordine octetorum -->
    <locus titulus="liberi" species="lista-nodus"/>
  </genus>
  <genus titulus="assignatio">
    <locus titulus="tok_titulus"     species="token"/>
    <locus titulus="tok_subscriptum" species="token"/> <!-- [..] crudum, optionale -->
    <locus titulus="tok_operator"    species="token"/> <!-- = aut += -->
    <locus titulus="valor"           species="nodus"/> <!-- verbum aut tabulatum; absens pro a= -->
  </genus>
  <genus titulus="tabulatum">                          <!-- ( verbum ... ) ; [i]=v = assignatio filia -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="redirectio">
    <locus titulus="tok_fd"       species="token"/>    <!-- digiti aut {titulus}, optionale -->
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="scopus"       species="nodus"/>    <!-- verbum -->
    <locus titulus="corpus"       species="referentia"/> <!-- heredoc, solum << et <<- -->
  </genus>
  <genus titulus="functio">
    <locus titulus="tok_verbum"   species="token"/>    <!-- 'function', optionale -->
    <locus titulus="tok_titulus"  species="token"/>
    <locus titulus="tok_apertura" species="token"/>    <!-- ( -->
    <locus titulus="tok_clausura" species="token"/>    <!-- ) -->
    <locus titulus="corpus"       species="nodus"/>    <!-- imperium compositum -->
  </genus>
  <genus titulus="grex">                               <!-- { ... } -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="liberi"        species="lista-nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="crustula">                           <!-- ( ... ) -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="liberi"        species="lista-nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="conditio">                           <!-- if probatio then liberi rami fi -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="probatio"      species="lista-nodus"/>
    <locus titulus="tok_deinde"    species="token"/>
    <locus titulus="liberi"        species="lista-nodus"/>
    <locus titulus="rami"          species="lista-nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="ramus">                              <!-- elif (IV loci) aut else (probatio, tok_deinde absentes) -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="probatio"     species="lista-nodus"/>
    <locus titulus="tok_deinde"   species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
  </genus>
  <genus titulus="iteratio">                           <!-- for/select titulus [interiecta] [in verba] separator cursus -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="tok_titulus"   species="token"/>
    <locus titulus="interiecta"    species="lista-nodus"/> <!-- corpora heredoc post titulum (P6: 'read x <<EOF; for i' + linea nova) -->
    <locus titulus="tok_in"        species="token"/>
    <locus titulus="verba"         species="lista-nodus"/>
    <locus titulus="separator"     species="lista-nodus"/> <!-- separator (; aut linea nova quae verba terminat) et corpora heredoc quae eum sequuntur (P6) -->
    <locus titulus="cursus"        species="nodus"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="cyclus">                             <!-- for (( a; b; c )) -->
    <locus titulus="tok_apertura"             species="token"/>
    <locus titulus="tok_parenthesis"          species="token"/>
    <locus titulus="liberi"                   species="lista-nodus"/> <!-- expressio, operator(;), ... -->
    <locus titulus="tok_parenthesis_clausura" species="token"/>
    <locus titulus="separator"                species="lista-nodus"/> <!-- ut in iteratione (P6) -->
    <locus titulus="cursus"                   species="nodus"/>
    <locus titulus="redirectiones"            species="lista-nodus"/>
  </genus>
  <genus titulus="repetitio">                          <!-- while/until probatio cursus -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="probatio"      species="lista-nodus"/>
    <locus titulus="cursus"        species="nodus"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="cursus">                             <!-- do ... done -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="electio">                            <!-- case verbum [interiecta] in optiones esac -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="verbum"        species="nodus"/>
    <locus titulus="interiecta"    species="lista-nodus"/> <!-- corpora heredoc post verbum (P6: 'read x <<EOF; case $x' + linea nova) -->
    <locus titulus="tok_in"        species="token"/>
    <locus titulus="liberi"        species="lista-nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="optio">                              <!-- [(] exemplar | exemplar ) liberi ;; -->
    <locus titulus="tok_parenthesis"          species="token"/>
    <locus titulus="exemplaria"               species="lista-nodus"/> <!-- verbum, operator(|), ... -->
    <locus titulus="tok_parenthesis_clausura" species="token"/>
    <locus titulus="liberi"                   species="lista-nodus"/>
    <locus titulus="tok_terminator"           species="token"/>       <!-- ;; ;& ;;& ; absens ante esac -->
  </genus>
  <genus titulus="arithmetica">                        <!-- (( expressio )) ut imperium -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="expressio"     species="nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="iudicium">                           <!-- [[ ... ]] ut imperium -->
    <locus titulus="tok_apertura"  species="token"/>
    <locus titulus="expressio"     species="nodus"/>
    <locus titulus="tok_clausura"  species="token"/>
    <locus titulus="redirectiones" species="lista-nodus"/>
  </genus>
  <genus titulus="socius">                             <!-- coproc [TITULUS] imperium -->
    <locus titulus="tok_verbum"  species="token"/>
    <locus titulus="tok_titulus" species="token"/>
    <locus titulus="imperium"    species="nodus"/>
  </genus>

  <!-- verba et partes (C10) -->
  <genus titulus="verbum">
    <locus titulus="partes" species="lista-nodus"/>
  </genus>
  <genus titulus="pars-litteralis">   <locus titulus="tok" species="token"/> </genus>
  <genus titulus="pars-effugium">     <locus titulus="tok" species="token"/> </genus>   <!-- \x -->
  <genus titulus="pars-continuatio">  <locus titulus="tok" species="token"/> </genus>   <!-- \ linea nova intra verbum -->
  <genus titulus="pars-simplex">      <locus titulus="tok" species="token"/> </genus>   <!-- '...' totum -->
  <genus titulus="pars-gemina">                                                          <!-- "..." -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="partes"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="pars-effugia">      <locus titulus="tok" species="token"/> </genus>   <!-- $'...' totum -->
  <genus titulus="pars-versa">                                                           <!-- $"..." -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="partes"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="pars-parametrum">                                                      <!-- $x $1 $? -->
    <locus titulus="tok_sigillum" species="token"/>
    <locus titulus="tok_titulus"  species="token"/>
  </genus>
  <genus titulus="pars-expansio">                                                        <!-- ${ [#|!] titulus [sub] [op] argumenta } -->
    <locus titulus="tok_apertura"    species="token"/>
    <locus titulus="tok_praefixum"   species="token"/>
    <locus titulus="tok_titulus"     species="token"/>
    <locus titulus="tok_subscriptum" species="token"/>
    <locus titulus="tok_operator"    species="token"/>
    <locus titulus="argumenta"       species="lista-nodus"/>  <!-- verbum, operator(/ aut :), verbum -->
    <locus titulus="tok_clausura"    species="token"/>
  </genus>
  <genus titulus="pars-substitutio">                                                     <!-- $( ... ) aut `...` -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>  <!-- lista programmatis -->
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="pars-arithmetica">                                                     <!-- $(( expressio )) -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="expressio"    species="nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="pars-processus">                                                       <!-- <( ... ) >( ... ) -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
  <genus titulus="pars-domus">        <locus titulus="tok" species="token"/> </genus>   <!-- ~ aut ~user in initio verbi -->

  <!-- arithmetica (C11): folia etiam partes ($x, ${..}, $(..)) -->
  <genus titulus="numerus">    <locus titulus="tok" species="token"/> </genus>
  <genus titulus="variabilis">
    <locus titulus="tok_titulus"     species="token"/>
    <locus titulus="tok_subscriptum" species="token"/>
  </genus>
  <genus titulus="praeposita">                         <!-- -x !x ~x ++x --x +x -->
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="operandum"    species="nodus"/>
  </genus>
  <genus titulus="postposita">                         <!-- x++ x-- -->
    <locus titulus="operandum"    species="nodus"/>
    <locus titulus="tok_operator" species="token"/>
  </genus>
  <genus titulus="binaria">                            <!-- etiam assignationes et comma -->
    <locus titulus="sinister"     species="nodus"/>
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="dexter"       species="nodus"/>
  </genus>
  <genus titulus="ternaria">
    <locus titulus="probatio"     species="nodus"/>
    <locus titulus="tok_quaestio" species="token"/>
    <locus titulus="sinister"     species="nodus"/>
    <locus titulus="tok_colon"    species="token"/>
    <locus titulus="dexter"       species="nodus"/>
  </genus>
  <genus titulus="inclusa">                            <!-- ( expressio ) -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="expressio"    species="nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>

  <!-- iudicium [[ ]]: operanda sunt verba; verbum nudum = probatio chordae non vacuae -->
  <genus titulus="iudicium-praeposita">                <!-- -f x ; ! c -->
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="operandum"    species="nodus"/>
  </genus>
  <genus titulus="iudicium-binaria">                   <!-- a == b ; a =~ re (dexter: verbum partis unius REGULA) -->
    <locus titulus="sinister"     species="nodus"/>
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="dexter"       species="nodus"/>
  </genus>
  <genus titulus="iudicium-coniuncta">                 <!-- a && b ; a || b (&& artius) -->
    <locus titulus="sinister"     species="nodus"/>
    <locus titulus="tok_operator" species="token"/>
    <locus titulus="dexter"       species="nodus"/>
  </genus>
  <genus titulus="iudicium-inclusa">                   <!-- ( c ) -->
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="expressio"    species="nodus"/>
    <locus titulus="tok_clausura" species="token"/>
  </genus>
</registrum>
```

Notes on shape:

- **List items** (`programma.liberi`, every `liberi` of a compound,
  `pars-substitutio.liberi`, `pars-processus.liberi`, `optio.liberi`,
  `conditio.probatio`, `repetitio.probatio`, `ramus.probatio`,
  `ramus.liberi`): `catena | pipa | imperium | functio | grex |
  crustula | conditio | iteratio | cyclus | repetitio | electio |
  arithmetica | iudicium | socius | separator | heredoc | malum`. The
  canon names this union once as `sententia`.
- **Brace expansion** `{a,b}` and globs are expansion-time and stay
  literal bytes. **Tilde** is a part because it is recognisable
  lexically (word start, up to the first unquoted `/`) and a lint
  wants it.
- **Heredoc bodies** reuse the part genera; the delimiter's quoting
  decides whether the body has parts or one literal.
- The `corpus` referentia makes materia write `id="nodN"` on the
  `heredoc` and `<corpus(> #nodN` on the `redirectio`; the canon
  declares `id` on `heredoc`.
- **Absent-slot table** (the arbor gate asserts it row by row):

| source | absent |
|---|---|
| `a=` | `assignatio.valor` |
| `>f` | `redirectio.tok_fd` |
| `f() {…}` | `functio.tok_verbum` |
| `function f {…}` | `functio.tok_apertura`, `tok_clausura` |
| `for x; do …` | `iteratio.tok_in`, `verba` empty |
| `for x do …` / `for x\ndo …` | also `separator` (the newline is trivia here) |
| `else` branch | `ramus.probatio`, `tok_deinde` |
| last `case` item before `esac` | `optio.tok_terminator` |
| `case x in a) ;; esac` | `optio.tok_parenthesis` |
| `coproc cmd` | `socius.tok_titulus` |
| `${x}` | `pars-expansio.tok_praefixum`, `tok_subscriptum`, `tok_operator`, `argumenta` empty |
| `${x:-}` | `argumenta` empty, operator present |
| `cat <<A` at EOF with no delimiter line | `heredoc.tok_delimitator`, `tok_finis` |
| any construct cut off by EOF | its closing tokens |

---

## 5. The builder (`crusta_arbor`)

**Frames.** One loop: pull a token in the mode the top frame asks for,
dispatch on (frame genus, token genus), never recurse. A frame is
`{nodus, genus, locus receiving the next child, modus, status}` on an
`Xar`. The frames ARE the mode stack: pushing `pars-gemina` switches
the pull to `GEMINA`; pushing `pars-substitutio` switches to `INITIUM`
with a fresh command context; popping restores the enclosing mode. A
word frame survives a nested program: `a$(b)c` is one `verbum` of
three parts because the substitution frame sat on top of it and the
next literal is adjacent.

**Positional rules live in frame status.** `INITIUM` is asked only
while the current `imperium` has no `verbum` yet; after the first word
the frame asks `VERBA`, or `ASSIGNATIONES` when that word is in the
dialect's assignment-builtin table. A `RESERVATUM` in `INITIUM` opens
its construct, or, when it cannot stand there (`fi` with no `conditio`
open, `then` outside `if`, `done`, `esac`, `}` with no `grex`, `;;`
outside `case`, a stray `)`, `;` or `&` with nothing pending), becomes a
`malum` in the current list (C12).

**Terminators.** A separator token closes the open
`imperium`/`pipa`/`catena` chain down to the innermost list and appends
a `separator` there. `&&`/`||` wrap the pending item in a `catena`
(created on the first operator, reused after); `|`/`|&` wrap in a `pipa`
under it; `!` and `time` open the `pipa` first. Compound keywords close
their own construct only: `fi` pops to the nearest `conditio`,
everything between closing with absent tokens (html's end-tag rule), so
`if a; then { b; fi` gives a `grex` with `tok_clausura` absent, not a
lost `fi`.

**Words.** Part tokens accumulate in the open `verbum` while adjacent;
a non-adjacent part starts a new `verbum`. A single unquoted
`pars-litteralis` word in `INITIUM` position that equals a reserved word
never reaches the builder as a word: the lexer emitted `RESERVATUM`.
The loop name after `for`/`select` and the function name are single
literal parts; anything else there is a malum.

**Redirections** attach to the open `imperium` (in `liberi`, byte
order) or, after a compound command's closing token, to its
`redirectiones`; a redirection with nothing open (`> f` alone) opens
an `imperium` with no words, which is legal shell.

**EOF.** Every open frame closes with its closing tokens absent; the
`FINIS` token, with the trivia before it, goes to `programma.cauda`.

**Heredoc placement (decree I, C5).** On the first newline token after
one or more `<<` redirections, the builder reads each body in order
and appends each `heredoc` node to the *innermost open list*
immediately after the child holding that newline: the `separator`
node, or the node whose last token carries the newline as `post`
trivia (the `operator` of `cat <<A |`, the `&&` of `cat <<A &&`).
Then it writes `corpus:referentia` on the waiting `redirectio`. The
bytes are in order for the emitter with no special case; the STML
projection carries the reference; a heredoc inside `$( )` lands in the
substitution's list, one whose `)` closed before the newline lands in
the outer list (bash's reading). Two bodies on one line queue in order.

**`$((`** (bash 5.2's own rule): try `pars-arithmetica`; if the
expression does not close with `))`, reset the cursor to the `$(` and
re-parse as `pars-substitutio` whose first command is a `crustula`.

**Regions**: a `heredoc` or backtick frame sets the lexer's region
limit; `FINIS` inside a region pops the frame, not the parse.

**Named deviations** (each a row of the reservatio gate, pinned by its
measured effect):

| construct | treatment |
|---|---|
| extglob `@( ) ?( ) *( ) +( ) !( )` | parsed with the option OFF, always: a word, a `crustula`, and mala; counted, never accepted |
| aliases | never expanded; `alias x='if'` then `x …` parses as a command named `x` |
| `eval`, `trap`, `bash -c`, `source`d text | opaque words; a consumer may re-enter the parser on the static value (§9) |
| `set -o posix`, `shopt -s …` in the script | ignored; the grammar is bash 5.2's default options for the whole file |
| `$"…"`, `$'…'` | parsed, never translated or decoded in the tree (`crusta_verbum_staticum` decodes `$'…'` on request) |
| `$((` ambiguity | arithmetic first, substitution on failure (bash's order) |

**Dialect table** (`crusta_registrum.h`, M7: a table, never a program):

```c
nomen structura {
    constans character* constans* reservata;          /* if then else elif fi case esac for select while until do done in function time coproc { } [[ ]] ! */
    constans character* constans* aedificatores;      /* alias declare export local readonly typeset */
    constans character* constans* redirectiones;      /* < > >> <& >& <> >| &> &>> << <<- <<< */
    constans character* constans* terminatores;       /* ;; ;& ;;& */
    constans CrustaOperator*      arithmetici;        /* titulus, praecedentia, associatio dextra? (13 gradus, ordo bash) */
    constans character* constans* iudicii_praeposita; /* -a -b -c -d -e -f -g -h -k -p -r -s -t -u -w -x -G -L -N -O -S -z -n -o -v -R */
    constans character* constans* iudicii_binaria;    /* -nt -ot -ef -eq -ne -lt -le -gt -ge == = != =~ < > */
} CrustaDialectus;

externus constans CrustaDialectus CRUSTA_BASH;        /* 5.2.15 */
```

The builder never recurses; the projection and the emitter are
materia's and recursive; the totalitas gate pins where the first dies
(`01M1FAD8`), as html's does.

---

## 6. Arithmetic and `[[ ]]`

**`crusta_arithmetica`.** bash's `(( ))` is C's expression grammar
minus casts and calls, plus `**`, the comma and the assignment set.
`crusta_arithmetica_parsare(lector, frame)` runs shunting-yard over
two explicit stacks (operands, operators) from `CRUSTA_BASH.arithmetici`
(13 levels, bash order: postfix; prefix `! ~ ++ -- + -`; `**` right;
`* / %`; `+ -`; `<< >>`; `< <= > >=`; `== !=`; `&`; `^`; `|`; `&&`;
`||`; `?:` right; `= *= /= %= += -= <<= >>= &= ^= |=` right; `,`).
Leaves: `numerus` (decimal, `0x…`, octal `0…`, `base#digits`),
`variabilis` (with a raw `[…]` subscript), and the word parts
`pars-parametrum`, `pars-expansio`, `pars-substitutio`, which the lexer
emits in the arithmetic modes as the same genera. Iterative, so
`((((((…` to any depth is a totality pin. Inside `for ((`, the three
clauses are separated by `ARITHMETICA_SEPARATOR` tokens that become
`operator` nodes in `cyclus.liberi`; empty clauses are simply absent
expressions between them.

**Constant evaluator.** `crusta_arithmetica_aestimare(expressio, s64*)`
over trees with no `variabilis` and no parts: 64-bit wrapping like
bash, `**` by repeated squaring, division and modulo by zero refused
(FALSUM). It exists for the gate: bash's deparse prints arithmetic
verbatim, so `declare -f` proves nothing about precedence; the
arithmetic gate compares each fixture expression's value with a golden
from `bash -c 'echo $((…))'`. A lint gets a constant folder for free.

**`[[ ]]`** is parsed by the builder itself into four genera: operands
are `verbum` nodes; `-f x` and `! c` are `iudicium-praeposita`; the
binary operators from the dialect table make `iudicium-binaria`;
`&&`/`||` make `iudicium-coniuncta` (`&&` binds tighter, both
left-assoc); parens make `iudicium-inclusa`; a bare `verbum` is the
non-empty-string test. After `=~` the builder asks one token in
`REGULA` mode, which becomes a single-part `verbum` (quoted portions
inside it stay their own parts: bash treats quoted regex bytes
literally, and a lint needs the distinction). `[ … ]` and `test` are
plain commands with words, as in bash.

---

## 7. The cooked view and the oracles

**`crusta_coctum_scribere(radix) → chorda`** prints the tree in bash's
`declare -f` normal form, the role `html_coctum` plays against
html5lib's dumps. Rules measured on 5.2.15 before building:

- header `f () \n{ \n` (trailing spaces kept), body at four spaces per
  level, `}` closing;
- `;` after every list item but the last of a group; `&` for a
  background item; `&&`/`||` on one line;
- simple commands: assignments and words in source order, then EVERY
  redirection after the words (`2>&1 A=1 cmd x >o` → `A=1 cmd x 2>&1 > o`),
  a space after the operator; array literals verbatim;
- comments and blank lines dropped; word bytes verbatim (quotes,
  `$'…'`, braces, arithmetic inside `$(( ))` and `(( ))`);
- `|&` printed `2>&1 |`; `h() ( : )` printed `function h () \n{ \n    ( : )\n}`
  (the `function` keyword and `()` both always present);
- `if a; then\n    b;\nfi`; `while :; do\n    …;\ndone`;
  `for i in a b;\ndo\n    …;\ndone` (newline before `do`); `case $x in \n`
  (trailing space) with items `a | b)`, the list one level deeper, `;;`
  at item level, an empty list printed as a blank line;
- heredoc bodies inline right after the pipeline operator's line
  (`cat <<A <<B |\none\nA\ntwo $x\nB\n  wc -l`);
- `[[ ]]` reconstructed with single spaces; `time`, `select`, `coproc c { \n    :\n};`.

The rest is discovered one failure class at a time, each a rule in the
view, never in the parser.

**Oracle I — structure, `declare -f`.** A case is wrapped as
`f() {\n<case>\n}\ndeclare -f f`, fed to `/opt/homebrew/bin/bash`; the
printed function is the expected text. We parse the same wrapped text
and print through the cooked view; equal or not. Expected texts are a
golden, `probationes/fixa/crusta/oraculum/expectata.txt`, in the
`#### name` block format with the bash version in its header, written
by `./crusta/oraculum.sh -scribere` (a named cause) and checked by
`-probare` (rerun bash, diff the golden: the silva-canon freshness
precedent). The gate reads the golden, spawns nothing, and is
deterministic. A bash other than 5.2 is refused by the instrument.

**Oracle II — validity, `bash -n`.** Same mechanics
(`oraculum/validitas.txt`: exit status and first error line per case).
Our verdict is VALID iff the tree has zero `malum` nodes and no absent
closing token (`CrustaParsura`); agreement pinned rising. This is where
the pathologies bash rejects are proven to land as mala and nowhere
else.

**Cases.** One reader (`crusta_exempla`, `#### title` blocks, the Oils
`spec/` format so an Oils file reads unchanged later) over three
sources: `pathologiae.sh` (the research appendix's snippets plus
`<( )`, `>( )`, `|&`, `&>`, `{fd}>`, `;;&`, `;&`, `<<-`, `<<<`, `=~`,
`coproc`, `select`, `time`, `for (( ))`, arrays, `$"…"`, a CRLF case,
`$((` both ways, `$(` containing `)` in a comment, nested backticks,
a heredoc inside `$( )` and one whose `)` closes before the newline);
the FreeBSD `parser/` cases (vendored, C3); the house scripts (230,
judged live by the instrument's `-domus` mode, C14, and reported in the
phase-log — the gates pin what needs no bash: byte law, zero mala).

---

## 8. Gates

Fourteen probationes, `./crusta/compile_probationes.sh [filter]`,
exit 2 = NOTHING RAN, every one born red by a planted fault that fails
the assertion, not the build.

| probatio | asserts | planted fault at birth |
|---|---|---|
| `registrum` | generated tables byte-equal to a fresh render (`materia_registrum_recens`), lexicon rows by title, loci contiguous, dialect tables sorted and duplicate-free | two lexicon rows swapped |
| `lector` | byte coverage: tokens + trivia concatenated equal the source on every fixture, per mode path; heredoc queue order; fd gluing; the three `$` cases | a trivium dropped at the division |
| `arbor` | inline cases through `materia_scribere_nodum` + `memcmp`; the absent-slot table row by row; positional rules (`echo if`, `x=if`, `A=1 echo B=2`); stray keywords as mala; `$((` both ways; backticks by depth; word runs | `tok_clausura` of `grex` never assigned |
| `corpus` | house scripts and FreeBSD cases byte-identical; self-measured (files, bytes) | `cauda` skipped for one file |
| `stml` | write → read → write, two cycles byte-equal; comparator STRUCTURALIS (and FIDELIS if the LINEA munus permits — measured, refusal pinned by cause); every `corpus` reference carried; re-read emission = input; over the corpus | the re-read tree mutated before comparison |
| `canon` | drift guard both ways, seal pin against the live `materia_arbor_sigillum`, every corpus document judged | one genus rule deleted |
| `reservatio` | each named deviation lands as measured (extglob's malum count on a fixture, alias-as-command, `eval` opaque) | a deviation's expected count edited |
| `totalitas` | random bytes, mutated and truncated corpus, CRLF, unclosed quotes/heredocs/`$(`, `$(` and `((` and `"` to 100,000 deep under `CREDO_NON_RUIT`; the first recursive walker's limit pinned | the frame stack replaced by recursion for one construct |
| `computus` | golden `fixa/computus/basis.tsv` (deterministic columns) | a golden count edited |
| `arithmetica` | structure (precedence, associativity, bases, ternary, comma, assignments) on inline cases; constant evaluation against `oraculum/arithmetica.txt` | one precedence row swapped |
| `coctum` | the printer's rules on inline cases (indent, `;` placement, redirect order, `|&`, function form, heredoc inline) | the redirect reorder disabled |
| `exempla` | the case reader: counts and bytes of `pathologiae.sh` and the FreeBSD set pinned | a block boundary rule broken |
| `oraculum` | `declare -f` agreement over the fixed cases, pinned RISING; `ORACULUM_OMNIA=1` lists failures, `ORACULUM_EXEMPLUM=name` one case | the pin raised above the count |
| `differentia` | `bash -n` agreement pinned rising; house-corpus mala pinned at ZERO once reached (falling never) | the verdict inverted for one class |

Every probatio self-measures its coverage ("N/N clean" of nothing is
the lying-green class). `./materia/shim_probare.sh` runs at the end of
the client as the phase-boundary audit.

**Instruments:** `./crusta/arbor.sh x.sh [-tacitus]` (projection);
`./crusta/coctum.sh x.sh` (the normal form — the first useful tool, a
script's canonical rendering); `./crusta/computus.sh x.sh [-machina]
[-iter N]`; `./crusta/oraculum.sh -scribere | -probare | -domus`.
Pythonica: `PORTAE['crusta']`, `silva.metiri('.sh')`.

---

## 9. Named futures (design banked, nothing built)

- **Formatter for `.sh`** (arc 2): a printer over the byte tree that
  respects trivia (comments, blank lines, continuations) and reflows
  only what the house style names, so `silva.Editio` can touch runners
  again (today it reflows by width and breaks bash, quaestio filed
  2026-09-15). The cooked view is NOT it: the cooked view is bash's
  normal form, comments dropped.
- **Lint** (arc 3): unquoted expansions, `$?` after a pipe, `[` vs
  `[[`, undefined names, unreachable `;;`, the house's own rules.
- **Sub-parsers over words** (Fran's question, 2026-09-16). Two layers,
  both over `verbum` as the uniform unit: (i) *argument grammars* of
  known commands as TABLES (which options take values, positionals),
  a lint-shaped consumer over `crusta_imperium_titulus/argumenta` and
  `crusta_verbum_staticum`; (ii) *true sublanguages inside a word* —
  regex for `grep`/`sed`/`=~`, awk programs, `printf` formats, `find`
  expressions, `sed` scripts, and first of all `eval`/`trap`/`bash -c`
  strings, which are crusta parsing crusta. Each is a parser over the
  DECODED value, derived bytes: the byte law forbids splicing that into
  the byte tree as source, and materia has both seams already — md's
  derived tokens (`fons_index` ≠ 0, omitted by the emitter, present
  only when they differ) and the origin hook (`MateriaOrigoUncus`, born
  for C89 macro expansion) mapping derived bytes back to the part they
  came from. A foreign-grammar result (regex, awk) is its own materia
  tree with its own registry, linked from the consumer's table; a
  same-grammar result (`eval`) may later become a `derivatum:nodus`
  locus on `verbum`, appended with a seal move (C13). Nothing in this
  plan changes for it: parts are fine-grained enough that decoding is a
  fold, and the accessors exist from H3.
- **POSIX-only and zsh dialect tables**: second `CrustaDialectus`
  instances; the parser has no branch on dialect, only table lookups.
- **Oils `spec/`** as a fourth case source when the tail asks.
- **A shell** (the OS vision): the expansion pass and an executor over
  this tree; not before a shell is wanted.

---

## 10. What this client tests in the substrate

Named before building so the answer is an answer either way:

- a `referentia` whose target is a LATER sibling in byte order (html's
  `sedes` pointed backward and upward) — the reader resolves after the
  whole tree, so it should be free; measured;
- the same bytes as two genera by mode (newline), and a language that
  has a `LINEA` munus AND substantive newlines — what FIDELITAS makes
  of it;
- trivia bound backward (`post`) as the common case; css/html/C89 bind
  forward;
- a `lista-token` locus in daily use (`pipa.praefixa`, `malum.tokens`);
- parts whose token values contain newlines (heredoc literals,
  `pars-simplex` across lines) through the STML raw form;
- an iterative builder with a region stack under materia's recursive
  walkers, depth pins again.

§10 of html's spec named JS as next; Fran's order (2026-09-16) is
crusta first, JS after: the regex-vs-division mode question JS needs
is rehearsed here fifteen times on a grammar a tenth the size.

---

## Appendix A — Fran's research notes (2026-09-15), verbatim

*Status: research notes, not yet doctrine. Exploratory — claims here
have not set hard enough to freeze addresses on. Fran, 2026-09-15.*

### A.1 The question

Is "a bash parser" a well-founded project in the same sense that "a
CSS parser" or "a JavaScript parser" is?

Answer in three parts:

1. **The grammar layer**: yes, and it is small. Comparable to CSS.
2. **The token layer**: no, not in the same sense. Shell has no
   context-free tokenizer, and this is not an accident of bash but a
   property written into POSIX.
3. **"Parse all bash programs"**: ill-posed. Both the text being
   parsed and the grammar being used are computed at runtime. The
   tractable target is different, and stating it precisely is most of
   the design work. See A.4.

### A.2 The grammar layer

The outer structure is an ordinary recursive-descent grammar:

```
program          → complete_command*
complete_command → and_or (separator and_or)* separator?
and_or           → pipeline (('&&' | '||') newline* pipeline)*
pipeline         → '!'? command ('|' newline* command)*
command          → simple_command | compound_command redirect*
                 | function_definition
compound         → brace_group | subshell | for | case | if | while
                 | until
```

POSIX publishes this (XCU §2.10). dash's hand-written parser covers the
whole POSIX core in a couple thousand lines of C. bash's `parse.y` is a
yacc grammar of a few pages.

The grammar has never been the hard part. Anyone who has written a
recursive-descent parser can write this part in a weekend. The trap is
believing that is the project.

### A.3 Catalogue of pathologies

These are the things that make the lexer non-autonomous. Each one is a
concrete test case; the appendix collects them as snippets.

**A.3.1 Reserved words are positional.** `if` is a keyword only in
command position.

```sh
if true; then :; fi    # keyword
echo if                # plain word
x=if                   # plain word
```

The lexer cannot decide this. The parser knows how many words have
been consumed in the current simple command; the lexer does not.

**A.3.2 Assignment prefixes are positional.**

```sh
A=1 cmd                # A=1 is an assignment
cmd A=1                # A=1 is an argument, identical bytes
```

Same failure as A.3.1, and it interacts with it: `A=1 if` is a syntax
error but `A=1 echo if` is not.

**A.3.3 Here-documents suspend and resume across lines.** The operator
`<<WORD` is recognized mid-line, but the body starts on the next line,
after the remainder of the current line has been lexed. Multiple
here-docs on one line queue in order.

```sh
cat <<A <<B | wc -l
first body
A
second body
B
```

Consequence: the lexer needs a pending-heredoc queue and line-granular
positioning. This alone kills the "pull tokens on demand from a
character stream" model — the reader must be able to jump ahead and
come back.

**A.3.4 `$( )` cannot be scanned by counting parentheses.** Quotes,
comments, nested substitutions, and nested here-docs all live inside.
POSIX states outright that the extent must be found by recursively
parsing the contents.

```sh
echo $(echo ")" # )
echo "x")
```

**A.3.5 `$((` is ambiguous.** `$((a))` is arithmetic expansion;
`$( (a) )` written as `$((a); b)` is command substitution of a
subshell. Resolved only by attempting one and backtracking. bash gets
this wrong in some versions, which is itself useful data.

**A.3.6 Distinct sublanguages with their own lexers.** Each of these
has an operator set and word-termination rule that does not match
ordinary shell words:

| Construct | Notes |
|---|---|
| `$(( ))`, `(( ))` | C-like arithmetic; `<` is comparison not redirection |
| `[[ ]]` | `=~` right operand lexed differently from everything else, and differently across bash versions |
| case patterns | `)` terminates, `\|` alternates |
| glob / extglob | `@(a\|b)` only when `shopt -s extglob` |
| `${ }` | own operator set: `##`, `%%`, `:-`, `//`, `^^`, `@Q` … |
| `$'...'` | ANSI-C escapes; different again from `'...'` |

**A.3.7 Redirection fd numbers must be lexically glued.**

```sh
2>file      # fd 2 redirect
2 >file     # word "2", then stdout redirect
x2>file     # word "x2", then stdout redirect
```

Word termination depends on what follows the digits.

**A.3.8 Backticks have different rules than `$()`.** Backslash handling
differs, and nesting is by escaping rather than by recursion. A correct
parser needs a separate path, not an alias.

**A.3.9 Runtime state changes the grammar.**

- `shopt -s extglob` turns a syntax error into a pattern.
- `set -o posix` changes reserved-word handling.
- alias expansion happens at parse time on text defined at runtime.
- `eval` re-enters the parser on a string that does not exist until
  the program runs.

This is the bullet that moves the problem from "hard" to "different
category." See A.4.

### A.4 What "parse bash" can honestly mean

Because of A.3.9, there is no total function from bash source text to
a syntax tree. Andy Chu (Oils) argues this at length and it appears
sound.

The tractable target, which every serious implementation actually
adopts:

> A complete parser for a given source text under a fixed set of shell
> options, producing a tree in which eval/alias sites are nodes rather
> than expanded.

That is sufficient for linters (shellcheck), formatters (shfmt), static
analysis (Debian's CoLiS project), and for implementing a shell. It is
not sufficient for "prove what this script does," and no shell parser
gives you that.

Scope decision to make deliberately, not by drift: which dialect and
which option set is the 1.0 target. POSIX sh only (dash-shaped) is a
genuinely complete and finite artifact. "bash" is a moving target
defined by a codebase. Given the one-release-is-the-only-release
posture, POSIX sh + an explicitly enumerated bash extension list is the
version that can actually be finished.

### A.5 Architecture that works

**A.5.1 Lexer modes, selected by the parser.** The lexer becomes a pure
function of (mode, position) rather than an autonomous stream. The
parser owns a mode stack and asks for the next token in a named mode:
`LEX_NORMAL`, `LEX_DQ`, `LEX_ARITH`, `LEX_VSUB_ARG`, `LEX_CASE_PAT`,
`LEX_DBRACKET`, `LEX_HEREDOC_BODY`, … Oils uses on the order of a
dozen.

This is the same trick JavaScript needs for regex-vs-division and
template literals — just applied twelve times instead of twice. Once
you stop trying to make the lexer autonomous, every pathology in A.3
becomes routine.

**A.5.2 Whole source in one buffer; tokens are slices.**

```c
typedef struct {
    int mode;      /* which lexer mode produced it */
    int kind;
    int start;     /* byte offset into the single source buffer */
    int len;
} Token;
```

No per-token allocation. Makes the here-doc "jump ahead to the next
line, come back" manoeuvre trivial, because you are indexing rather
than streaming. Arena for the AST; free the arena, not the nodes. This
is the stb / Muratori-shaped version of the design and it happens to be
the one the problem actually wants.

**A.5.3 Parse and expand are strictly separate passes.** Parsing
produces a word as a list of parts:

```
Word = [ Literal | SingleQuoted | DoubleQuoted[parts]
       | ParamSub | CommandSub[ast] | ArithSub[ast] | TildeSub ]
```

Nothing expands during parsing. Tilde, parameter, arithmetic, command
substitution, field splitting, and globbing all happen in a second pass
at eval time, over that structure.

Conflating these two is the principal way hand-rolled shell parsers go
wrong. It is what makes quoting bugs unfixable later.

**A.5.4 Errors carry byte offsets.** Because tokens are slices, every
diagnostic can point at exact source extents for free. Worth committing
to from the first line of code.

### A.6 Prior art to read

| Source | Why |
|---|---|
| dash — `parser.c` | Minimal complete POSIX parser in C. The model for scope. |
| bash — `parse.y` + its lexer | The full surface, including the `parser_state` bitfield that carries all the context yacc cannot. Read it for the catalogue of what bash actually does, not for architecture. |
| Oils (oils-for-unix) — docs on lexer modes | Cleanest written formulation of parser-drives-lexer. Also the "you can't statically parse shell" argument. |
| mvdan/sh | Reference for how much bash surface a static parser covers in practice; shfmt is a parse/print round trip. |
| Morbig (Régis-Gianas, Jeannerod, Treinen — CoLiS/Debian) | Paper enumerates the same pathologies and proves out a specific architecture; validated against Debian's maintainer scripts. Best academic account. |
| Smoosh (Michael Greenberg) | Executable formal semantics for POSIX sh. libdash exposes dash's real AST to OCaml — a second machine-readable tree to diff against. |

### A.7 Test resources — the html5lib question

There is no exact analog to html5lib-tests. html5lib works because
HTML5 specifies tree construction byte-for-byte, so an
implementation-independent input → expected tree corpus is meaningful.
Shells have no standard AST and no standard serialization of one, so
nobody has published that artifact.

What exists splits into corpora and oracles.

**A.7.1 Behavioral corpora**

| Corpus | Format | Notes |
|---|---|---|
| Oils `spec/` | `#### name`, script, `## STDOUT:` / `## status:` / `## END`, with per-shell override blocks | Closest in spirit. Records what bash, dash, mksh, zsh, busybox ash and osh each do — that disagreement table is the html5lib property you want. ~50 lines to ingest. Apache 2. |
| mksh `check.t` | `name:` / `description:` / `stdin:` / `expected-stdout:`, `---` separated | One enormous inherited-from-pdksh file. Unusually heavy on parser corners. |
| FreeBSD `bin/sh/tests/` | small numbered scripts, with an actual `parser/` subdirectory | Heredocs, `$''`, alias-vs-reserved-word, case patterns, arith lexing. BSD-2 — the cleanest license here if you ever vendor cases into a zero-dependency repo. |
| bash `tests/` | `.tests` + `.right` expected-output pairs | heredoc, quote, case, redir, arith, braces. Authoritative for bash. GPLv3 — do not vendor. |
| busybox `shell/ash_test/` | `.tests` / `.right` pairs by feature | Small and readable. |

All of these test the whole shell, not the parser. For parser work,
keep the cases and discard those whose expected output depends on
environment.

**A.7.2 Oracles.** The real substitute for html5lib's expected trees is
that several implementations will print their parse:

- `osh -n` — prints Oils' AST in lispy form. A real inspectable tree.
- bash deparse — wrap a snippet in a function, then `declare -f` it;
  bash prints its own AST rendered back to normalized source. Diffing
  your re-print against bash's localizes disagreements fast.
- `shfmt` — parse/print round trip over a large chunk of bash.
- libdash — dash's AST, machine-readable.
- `bash -n` / `dash -n` — pure accept/reject, cheap, catches an
  enormous amount under differential testing.

**A.7.3 The corpus nobody advertises.** For "does it parse real scripts
at all": the Morbig/CoLiS people ran a static POSIX shell parser over
the tens of thousands of maintainer scripts in Debian (postinst, prerm,
and friends). "Every `.sh` and every maintainer script on the system"
is the shell equivalent of "parse the web," and it is sitting on any
Debian box.

**A.7.4 Proposed harness**

1. Ingest Oils `spec/` as the primary corpus — machine-readable and
   already annotated with who-disagrees-with-whom.
2. `bash -n` / `dash -n` over the Debian script corpus as the crash and
   coverage net.
3. bash `declare -f` round trip as the tree oracle for agreement on
   structure.
4. Hand-write cases for every pathology in A.3 as the regression floor.

That reaches most of what html5lib-tests would have given, at the cost
of writing the harness yourself.

### A.8 Open questions (answered by §0 of this spec)

- A.8.1 Dialect scope for 1.0 → C2.
- A.8.2 Library, shell, or analysis tool → C1 (library first).
- A.8.3 `eval` and alias sites → opaque nodes, named deviations (§5).
- A.8.4 Arithmetic as its own module → C11.
- A.8.5 Error-recovery posture → C12 (totality).
- A.8.6 A printer in scope → two of them (§7).

### A.9 Pathology snippets (the first gate's floor)

```sh
# 3.1 / 3.2 positional keywords and assignments
echo if; x=if; A=1 echo B=2

# 3.3 queued here-docs
cat <<A <<B | wc -l
one
A
two
B

# 3.4 command substitution containing a comment containing a paren
echo $(echo ")" # )
echo "x")

# 3.5 arith vs subshell ambiguity
echo $((1+1)); (echo $( (echo nested) ))

# 3.6 sublanguage lexing
[[ abc =~ ^a.c$ ]]; case x in a|b) ;; esac; echo $'\t'; echo ${x:-d}

# 3.7 fd gluing
exec 2>/dev/null; echo 2 >/dev/null; echo x2>/dev/null

# 3.8 backtick escaping
echo `echo \`echo deep\``

# 3.9 runtime grammar change
shopt -s extglob; echo @(a|b)
alias x='if true; then'; x echo hi; fi
```
