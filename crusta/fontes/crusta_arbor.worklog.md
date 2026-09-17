# crusta_arbor — worklog

Append-only, dated. Finds at find-time for the crusta builder (and the
lector, which has no worklog of its own).

## 2026-09-16 — P2 (lector) and P3 (builder, part one)

**Delayed append of sentences.** materia's lists are append-only
(`materia_valor_lista_appendere`, no removal), so `&&` and `|` cannot
"move the last child into a catena/pipa". The builder therefore keeps a
sentence node (imperium, pipa, catena) in its frame and appends it to
the enclosing list only when the frame CLOSES; `&&` pops the finished
item without appending (`_tollere`), creates the catena, appends the
item to it and pushes the catena frame. Byte order holds because
nothing else is appended to that list while the item is on the stack.
The heredoc placement of P5 will rely on the same invariant: the
separator that ends the line is appended after the item, and the body
goes after the separator.

**A trivium closes an empty assignment.** `a= b`: after the `=` the
assignment frame waits for a value; the blank must close it (value
absent), or the next word lands in it. Found by the arbor gate's
absent-slot row on the first run. The same is NOT true of a redirection:
`2> f` keeps waiting across the blank, as bash does.

**`\r` is an ordinary byte** (lector). bash treats it so: a CRLF script
has words ending in `\r` and a bare `\r` between commands is a command
named `\r`. Being faithful means the newline token is always `\n` alone
and materia's `cr` flag on TERMINATOR tokens never fires for crusta.
Named deviation, pinned in the lector gate.

**Backslash runs inside backticks.** bash strips one backslash level per
backtick depth; toward `$` and `` ` `` a run of n halves (the odd one is
absorbed), toward anything else it rounds up (pairs become one, a
trailing single stays). Tokens keep the raw bytes; the depth rides in
the token's private tail (`CrustaCauda.gravis`); the genus is decided
from the logical count; `crusta_verbum_staticum` strips again to
decode. Continuation and double-quote escapes inside backticks use the
depth-0 rule — a named corner.

**Three continuation flags in the lector** (`titulus_exspectatur`,
`assignationis_gradus` 1/2) survive exactly one pull. They exist because
`$$$x` cannot be lexed by one byte of lookback (`$$` then `$x`), and
because the subscript and `=` of `a[i]=v` are separate tokens after the
name.

**The formator and `#define` runs (quaestio 01M2NNE6WS).** A block of
object-like `#define`s between two functions makes the formator indent
the following definition as a declaration group (column 23). Also, a
one-line `si (...) x = y; alioquin si (...) ...` chain is reflowed into
a cascade of continuation lines. Write every branch as a block; never
put `#define`s between functions.

**Named intermediates in P3** (replaced by P4/P5): `$((` is a literal
part and its `))` become mala; reserved words other than `!` and `time`
are mala; `(` at command position is a malum; `[[`, `((` are mala.

## 2026-09-16 — P4 (arithmetic)

**A missing operand is the one the machine was expecting.** `$((1 +))`
first came out as `binaria(∅, +, 1)` and emitted `$(( +1))`: the
reduction popped the only operand as the RIGHT one. Now a reduction
forced while an operand is still expected (a trailing operator, or an
operator arriving right after another) takes its right operand as
absent, so `1 + * 2` becomes `binaria(binaria(1,+,∅), *, 2)` and the
bytes stay in order. Totality and the byte law together decide which
side is missing; validity is a separate count (`mala`).

**A completed ternary reduces when the outer colon arrives.**
`1?0?4:5:6`: after `4:5` the inner ternary holds its colon and waits
for a right operand; the second `:` belongs to the outer `?`, so the
loop reduces a colon-bearing ternary on top before attaching. A `?`
arriving over a completed ternary is NOT reduced (right-assoc:
`a ? b : c ? d : e`).

**`$((` refused = rewind, not repair.** bash tries arithmetic and, on
failure, re-parses as `$( (`. The builder drops the frames above and
including the `pars-arithmetica`, rewinds the lector to the opener's
offset with `situs.arithmetica_recusata` set (the lector then emits
`$(` for those bytes), pushes the opener's `ante` trivia back into the
ligator's pending list, restores `prior`, and sets the enclosing
word's adjacency to the opener's offset so the re-lexed `$(` joins the
same word. Tokens lexed inside the abandoned attempt are garbage in
the piscina; their bytes are re-read.

**`(( ))` stays pending after `))`.** A compound command must still
take redirections and be wrappable by `&&`/`|`, so its frame keeps
status II ("clausum") with mode VERBA and its receiving locus set to
`redirectiones`; the sentence-closing logic treats it like an
imperium. P5 gives the same shape to every compound.

## 2026-09-16 — P5 (builder, part two: compounds, functions, `[[ ]]`, heredocs)

**A frame's role comes from (genus, locus, status), not from genus
alone.** A `conditio` is a LIST while its `probatio` or `liberi` locus
receives sentences, EXPECTING between (`then`/`elif` due), and a
SENTENCE (status II) after `fi`, when it takes redirections and `&&`
or `|` wrap it like an imperium. `_lista_est`, `_sententia_est`,
`_compositum_clausum` and `_sententiam_admittit` read the frame, so
every closing keyword is one call: find the nearest open frame of its
genus, close everything above it with absent tokens (html's end-tag
rule, `_claudere_usque`), put the token, flip to status II. Status
numbers are per genus; the iteratio uses 0/I/III/IV (name, name seen,
verba, separator seen) so that II always means "closed".

**bash recognises reserved words right after `))`, `]]`, `}` and `)`**
(`if ((x)) then`, `until [[ a ]] do`, `{ { a; } }`, all measured) but
not after a plain word (`while true do` is an error). That is a
nineteenth lector mode, `POST_COMPOSITUM`: reserved words and
redirections on, assignments off (`[[ a ]] x=1` is an error), newline
terminates. `coproc` asks the same mode for its first word, because
`coproc a` followed by a newline must end there.

**`name()` costs nothing because of the delayed append.** When `(`
arrives on an imperium of exactly one literal word, the imperium node
is simply forgotten (it was never appended) and the frame becomes a
`functio` with `tok_titulus` = that word's token. `coproc NAME cmd`
versus `coproc cmd`: the first literal word is held as a candidate;
the next token decides (a compound opener makes it the title, anything
else makes it the command's first word, materialised as an imperium
whose word frame stays open so an adjacent part can still join).

**Heredoc bodies and the two substitutions (bash 5.2 measured, refines
decree 01M2NJ16RG).** `echo $(cat <<A)` + newline + body: bash warns
"unterminated here-document" for the substitution and then reads the
body from the OUTER text after the newline — so the petition stays
queued and the body lands in the outer list. `` echo `cat <<A` `` +
newline + body: bash never reads past the closing backtick; the body
is empty inside the substitution and the following lines run as
commands. The builder closes petitions made inside a backtick region
at the region's end (`heredoca_ante` on the substitution frame marks
which petitions are its own).

**A newline that is trivia still starts a body.** After `cat <<A |`
the newline is a `LINEA` trivium (INITIUM mode). The pending trivia
are bound `post` of the `|` at once (`_solvere_retro`, the same C7
division since the newline is last) and the body is opened; the
`heredoc` node goes to the innermost frame that holds a sentence list
— the pipa — right after the operator. Named corner: a newline that
lands in `iteratio.tok_separator` (only reachable through `$( )` in
the word list) has no following list, so that body would be appended
to the enclosing list BEFORE the loop; bash refuses a bare `<<` in
that position, the `$( )` route is left as a pathology for P6.

**`[[ ]]` machine.** Four levels: unary and binary tests (IV, reduced
as soon as their operand arrives — they take one word, never an
expression), `!` (III, lazy: `! a == b` is `!(a == b)`), `&&` (II),
`||` (I); parens are frames with their own machine. Two juxtaposed
operands (`[[ a b ]]`) become an `iudicium-binaria` with the operator
absent and count as a malum; bash prints "conditional binary operator
expected" but `bash -n` still exits 0 — a known `sana`/`bash -n`
divergence for P11's sanitas oracle, like a heredoc cut off by EOF
(bash: warning, exit 0; crusta: closure absent).

**`A=1 if` is a command named `if`** (bash measured), so a
`RESERVATUM` token arriving on an imperium becomes a `pars-litteralis`
whose token keeps genus RESERVATUM. The P3 case that pinned it as a
malum was an intermediate and is rewritten.

## 2026-09-16 — P6 (cases and corpus)

**The corpus found the bug every house script had.** 231 tracked `.sh`
files, byte-identical from the first run, but ~150 of them "insane"
with mala at `then`, `do`, `(`. Bisecting a script to the shortest
suffix that still produced the malum named the culprit every time: an
ASSIGNMENT-ONLY command on the previous line (`OBJ=""`, `RESET=...`,
`radix=$(...)`). A command frame with only assignments (status 0) kept
asking the lector for INITIUM, where a newline is a trivium, so the
newline never ended the command and the next line's `for`/`if`/
`name()` joined it as words. The mode after a first child that is not
a word is now ASSIGNATIONES (assignments still recognised, newline
terminates, reserved words are plain words — `A=1 if` stays a command
named `if`, now a LITTERALIS token). With that one fix the whole house
corpus parses with ZERO mala and zero absent closures. The lesson is
the html one again: a corpus finds what forty hand cases cannot, and
the first failure is worth reading before the tally.

**FreeBSD put heredoc bodies in the gaps.** `read x <<EOF; for i in
"$x"` + newline + body + `do …`, `read x <<EOF; for i` + newline, and
`read x <<EOF; case $x` + newline (heredoc14–16.0). The named corner
from P5 was not a corner: a `<<` on a command EARLIER on the line
leaves its body to whatever newline comes next, including the loop's
separator and the trivia after a loop name or a case word. The
declaration grew: `iteratio.separator` and `cyclus.separator` are now
LISTS (the separator node, then the bodies that follow it),
`iteratio.interiecta` and `electio.interiecta` hold bodies after the
name / the word (150 → 152 loci, seal not yet born). `_lista_recipiens`
returns (frame, locus) by phase. Gaps that keep no list — `for` before
its name, a function's title and parens, inside `(( ))` and `[[ ]]` —
are crossed and the body lands in the nearest list BELOW, out of byte
order: counted as `heredoca_transposita`, `sana` FALSUM, one case pinned
(`cat <<A; for` + newline). A limit named is better than a limit
hidden.

**Lector coverage through the builder's own pulls.** `crusta_arbor_
parsare_cum_lexematis` records every token the builder receives (the
main loop, the heredoc delimiter and end tokens, the backtick closer)
and drops the tokens of an abandoned `$((` attempt (`lexemata_ante` on
the frame); the corpus gate concatenates the values and compares with
the source. Clean over 322 files — the P2 gate could only measure
coverage per hand case.

**The reader copies.** `crusta_exempla` first sliced into the source
(no copies); `-Wcast-qual` refused the `const char*` → `i8*` slice
because `chorda.datum` is not const. It copies into the piscina, as
html's reader does. A `spec/*.test.sh` in a block comment is a
`-Wcomment` error (the `/*` inside) — the third time this repo has
been bitten by a glob in a comment.

## 2026-09-16 — P7 (STML projection)

**`initium_lineae` is derived, so the builder must derive it the same
way.** materia's STML reader does not read a per-token `linea-initium`
for source tokens (it is written only for DERIVED tokens); it
reconstructs the flag in its position-fixing pass: `post_lineam` is set
by every trivium of munus LINEA (not LAMINA) and consumed by the next
lexeme; the envelope carries the first lexeme's flag. crusta's lector
never set it, so the parsed tree had FALSUM everywhere while the re-read
tree had VERUM after every LINEA trivium: 315 of 322 corpus files
differed on the envelope (a shebang comment, then a LINEA trivium, then
the first lexeme) and eight inline cases structurally (`a &&\n\n  b`,
`if a\nthen`, heredoc bodies after `&&`…). Fix: `_lexema_recordare` —
the one function every received token passes — sets the flag by the
reader's rule; the `$((` rewind restores `post_lineam` from the opener's
own flag. The flag has no bash meaning: after a substantive newline
separator it is FALSUM, after a trivium newline (a blank line after
`&&`, the line after `then`) VERUM. It exists so the projection
round-trips; css and html sidestep it by having no LINEA munus at all.

**A trailing newline in a mixed element was silently lost — now
refused.** `a 'b` + newline (a quote open to EOF) parses into a
`simplex` token whose value ends in `\n` and carries ante trivia (the
space). A token with trivia is a MIXED element, which cannot take the
raw form, so the value is written escaped; STML's trivia ownership law
then hands the trailing whitespace run through the newline to layout,
and the reader's value lacks it: two bytes lost, two STML texts of
different length, no refusal. Measured with a scratch probe: `'b \n`,
`'b\n `, `'b\n\n`, `'b\t\n` all lose the whole edge run; `'b ` (no
newline) survives; `'b\n` as a SOLE child is raw and survives.
materia's `_textus_tutus` now refuses a value whose leading or trailing
whitespace run contains a newline (`valor lexematis textui non tutus
(mixtum)`) — the refusal materia's orientation predicted for
significant-whitespace tokens with trivia (D7), reached from the other
side. In the real corpus only a quote open at EOF hits it. materia,
css, md, html and oratio suites unchanged by the guard.

**The raw-form limit needs the right prefix and a raw element.** The
plan said `</lex-…>`; crusta's lexeme elements are `crusta-litteralis`,
`crusta-commentum`, … A bare `</crusta-litteralis>` in a command lexes
as two redirections and a word; a single-quoted one is a `simplex`
written escaped (`&lt;`), so it passes. What refuses: a comment
carrying `</crusta-commentum>` (trivia are raw) and a heredoc body line
carrying `</crusta-litteralis>` (a sole-child part). The fixture's
`forma-cruda` case was corrected to a comment. The previous whole-file
refusal came from a token spanning cases (open constructs of earlier
cases change the lexing context), which the new line no longer
triggers; the whole-file result is measured and pinned as measured.

**FIDELITAS holds.** The reader's cursor counts newlines inside token
values, so positions survive the substantive newline separator and
multi-line heredoc bodies; asserted on all 473 documents. STML is
≈ 28× the source here (html: 15×) — every token is an element with
its trivia series.

**The token tail is not projected.** `CrustaCauda.gravis` (backtick
depth) lives in the token's private tail, which materia does not write
(no frons hook). The gate hands the reader the same `forma` so re-read
tokens have a zeroed tail; `crusta_verbum_staticum` on a re-read tree
would decode backtick-nested words at depth 0. No consumer yet; the
seam is materia's frons.

## 2026-09-16 — P8 (canon)

**Measure the species tables, do not read them.** The canon must say
which genera each slot may hold. html read that by hand out of its
builder; for crusta's 152 slots I wrote a scratch probe that walks every
parsed tree (P7's 473 documents plus ~200 corner cases written for the
purpose) and tallies, per (genus, locus), the child node genera, token
genera and reference targets. Two runs, one merge, and the tables were
right on the first judgment — 491 documents, zero violations. Where the
corpus never produced a shape (ternaries, prefix operators, a loop with
redirections) the corner cases did, and those joined the gate's inline
set so the canon stays falsified there too. Lessons from the table: a
catena never nests a catena and never holds a separator (flat by
construction); a pipa may hold a pipa (`a | time b`); `$`-parts sit
directly as arithmetic operands (`$((a + ${b} + $(c)))`), not wrapped in
a verbum; `[[` lexes as a reserved word (tok_apertura of iudicium is
`crusta-reservatum`); a loop name may be a reserved word (`for if in
a`); `electio.liberi` can hold a malum (a stray `;;`); `iudicium-binaria`
can sit in a binaria's dexter (`[[ a == ]]`, totality).

**A first-run green is proven by its plants.** Both plants went red the
way the plan required: the deleted `verbum` rule fired the drift guard
(0 rules) and every document's judgment (`elementum extra canonem
<verbum>`); the false seal fired the guard (pinna vs vivum) and every
document (`valor generi attributi non congruit 'registrum-sigillum'`).

**`linea-initium` on the envelope is an electio.** STML interns a bare
attribute as `true`, so silva's canon declares it `genus="electio"`
with the one option; `veritas` would demand `verum|falsum`. crusta's
envelope carries it whenever the first lexeme begins a line, which
after P7's flag rule is nearly always.

## 2026-09-16 — P9 (reservation and totality)

**An "iterative builder" can still recurse through a helper.** The frame
stack never recursed, but `crusta_verbum_staticum` measured the word's
byte length with `_longitudo_cruda`, a recursive walk of the entire
subtree — including every `$( )` inside the word — before decoding,
which stops at the first substitution anyway. The builder asks for the
static value of every command's first word (`_aedificator_est`: is it
`declare`/`local`/…?). In `$($($(…` each first word closes around the
whole nest below it, so the walk ran once per level over everything
below: `$(` ×16,000 took 15 s, ×100,000 took 80 s and then overflowed
the stack. Found by the depth probe before the totality gate existed;
`sample` pointed straight at it. `_longitudo_statica` now measures only
what the decoder accepts (token parts, and gemina/versa one level down)
and returns -1 at the first non-static part. Pinned twice: the arbor
gate's `CREDO_NON_PENDET` at 20,000 deep (2 s; it was ~24 s), and the
totality gate's six 100,000-deep parse pins. The planted fault is this
walk put back. Lesson: when a pin says "the builder never recurses",
the probe must cover every function the builder CALLS, not the loop.

**The mutation corpus found three more (8 failures in 2,934 cases).**
Delta debugging (`minuere.py` in the scratchpad: drop chunks while the
failure class holds) reduced each to 4–12 bytes:
- `((a\ti`, `((t"`, `((g n` → arithmetic juxtaposition. The machine
  pushed an operand while none was expected and `crusta_arithmetica_
  finire` returned only the top operand: bytes lost. Now a juxtaposed
  operand first reduces every pending sign, then pushes an implied
  binary sign with NO token and precedence 0 (`IUXTAPOSITIO`), so the
  tree is `binaria(a, _, b)` in byte order; mala++; the evaluator refuses
  it (operator slot not a token).
- `[[ ! { a`, `[[ "" ! "" P` → `[[ ]]` juxtaposition was counted on
  arrival but JOINED at the end, after the signs were reduced, so a
  pending `!` bound the last operand: emission `[[ a ! b`. Now the
  implied sign (level IV, tokenless; `_iudicatio_reducere` skips the
  operator slot when the token is NIHIL) goes on the stack at arrival,
  the binaria forms at once, and `!` later takes the whole of it. Traced
  by hand before coding: `! a b`, `"" ! "" P`, `a && b c`, `a -f b c`
  all keep byte order.
- `${@E`, `${E[]t` (and `${x[1][2]}`) → the lector classified by byte
  class alone: any letter inside `${` was a PARAMETRUM_TITULUS, so a
  second name (after the special `@`, after a subscript) wrote the
  builder's name slot twice and materia refused (`dominus duplex`,
  parse NIHIL). bash -n accepts all three (bad substitution is a runtime
  error). Now position decides: a name only right after `${` or after a
  `#`/`!` prefix; `[` right after `]` is not a subscript. The rest lexes
  as the literal the builder already sends into `argumenta`, as `${xy z}`
  always did. Side effect, correct: `${#@}` now has `@` as the name
  (it was an operator token).

**The transposed-heredoc limit is a byte-law hole, and bash disagrees
with it inside expressions.** A mutated pathology file reduced to
`[[ $(<<A)` + newline + `y`: the body's newline sits inside `[[ ]]`, a
frame with no list, so P6's rule appends the body to the nearest list
below — before the `[[` in byte order. bash 5.2 measured: inside
`[[ ]]` and `${ }` it READS the body after the newline (`[[ -n $(cat
<<A)` / hello / A / `]]` prints yes; `${x:-$(cat <<A)` prints hello),
warning "unterminated here-document" and exiting 0; after `for`,
`select`, `function` or `f(` the newline is a syntax error. A real fix
needs a body node INSIDE expression trees (declaration + canon + seal),
a design call, so the totality gate names the class instead: a
differing emission passes only if `relatio.heredoca_transposita > 0`
and the length equals the source (bytes moved, none lost), and the
count is printed (1). Filed as a quaestio.

**Depth, measured at -O2 (the house flags).** Emission dies between
40,000 and 45,000 NODES (grex nesting: 1 node per level; `$(`: 3 per
level, alive at 13,000, dead at 16,000) — materia's recursive emitter,
same as html. The STML writer does not die at 10,000; it runs for
minutes (size quadratic: 500 deep 3.6 MB, 1,000 14 MB, 2,000 56 MB in
5 s, 4,000 225 MB in 43 s) and at 100,000 it overflows in 0.07 s.
The constant evaluator survives `1+(` ×130,000 and dies by 160,000;
nested bare parentheses survive any depth because `inclusa` is a tail
call under -O2 — so the pin uses the non-tail form.

**Reservatio shapes differ from the plan's guess.** extglob is not a
`crustula`: in VERBA mode `(` after words is a malum (bash errors at
the same `(`), then `a|b` is an ordinary pipa and `)` a second malum.
`alias x=…` parses `x=…` as an assignment because `alias` is in the
assignment-builtin table.

## 2026-09-16 — P9b (heredoc bodies in every gap)

**Inventory before design.** A probe (`lacuna_probe.c` in the
scratchpad: parse, emit, report transpositions and byte identity) over
~45 hand-built gap positions, each also checked with `bash -n`, sorted
the gaps: bash READS bodies inside `[[ ]]` (after `[[`, operand,
operator, `!`, `(`, `)`), inside `(( ))`/`$(( ))` (after opener,
operand, operator, `?`, `:`, `(`), in `for ((` clauses, and after
`function f` / `f()` / `function f ()`; it REJECTS a newline right after
`for`, `select`, `function`, `case`, and inside `f(`. One gap was never
counted: `for ((i=0` + newline appended the body to `cyclus.liberi`
before the pending expression — the machine only appends at `;`.

**A body lives in the list right after the token its newline follows.**
For fixed-shape frames a slot does it (`functio.interiecta`). For
expressions the node that will own the gap does not exist yet (the sign
is still on the stack), so the machines record bodies against the LAST
thing pushed — top operand, top sign, the ternary's colon, or the start
— and place them when the node is built: `binaria(sinister,
post_sinistrum, op, post_signum, dexter)`. Bodies after the LAST child
cannot go inside the node (they'd precede nothing) so they travel up
with the new node on the operand stack, and at `finire` whatever trails
the root goes to `post_expressionem`; bodies before any token go to
`post_aperturam`. The slot is always adjacent to its token, so emission
order equals byte order by construction. `_machinam_finire` relies on
`post_aperturam = expressio - 1` and `post_expressionem = expressio + 1`
in all three arithmetic containers (and in `iudicium`/`iudicium-
inclusa`), documented at the call.

**Where bash errors, degrade the tree, not the bytes.** For `for` +
newline and friends there is no bash reading to follow; closing the open
frame absent (`_claudere_usque`) puts the construct in its list, then the
body after it — byte order kept, `clausurae_absentes` makes `sana`
FALSUM exactly where bash says syntax error. A function that already
has its name takes a body into `interiecta` and switches to "expecting
body": bash rejects `function f` + newline + `()`, so a later `(` opens
a subshell body instead of the name's parens (which would emit before
`interiecta`).

**The strict fuzz found the next one immediately.** With transposition
gone the pathology mutation that had been "explained" by the limit was
still different: `[[ !()-a` emitted `[[() !-a`. A prefix operator (`-a`)
arriving after a complete operand was pushed above the pending `!`
without the implied juxtaposition sign, so the end-of-expression join
reordered bytes. Same rule as P9's operand case: insert the implied
binary sign first. Lesson: a named exemption in a fuzz gate hides the
next bug behind it; removing the exemption was worth more than the
count it printed.

**Tool slip, recovered.** A shell line meant for inspection contained a
stray `git stash push crusta/fontes/crusta_arbor.c`; it stashed the whole
uncommitted builder change. `git stash show` confirmed one file, `git
stash pop` restored it, the new functions were verified present. Never
put git state commands in exploratory one-liners.

**`$((` rewind with a body inside** (`echo $(( $(cat <<A)` + newline
… `) )`) round-trips byte-identically: the refused attempt's frames and
machine vanish with the lector rewind and the re-parse reads the body
again.

## 2026-09-16 — P10 (computus, instruments)

Mechanical transcription from html: `html` → `crusta`, `.html` → `.sh`,
the parse call gains the dialect and report arguments, and the STML
read-back gets `consilium.forma.mensura_caudae = sizeof(CrustaCauda)`
(P7's rule: the reader must create tokens with the same tail). html's
gate header said "PORTA COMPUTI CSS" (a copy slip from css) — corrected
in the crusta twin; html's own header is left for its owner. Numbers
worth remembering: the adversarial fixture's STML is 614× its source
because of one 200-deep `$(` case — the projection's size grows with
depth squared (P9 measured the curve); every other file is 25–35×.

## 2026-09-16 — P11a (the cooked view)

**Measure the printer's shape before writing it.** A probe harness
(`f() {` + case + `}` + `declare -f f`, `cat -e` to see trailing spaces)
over ~90 snippets showed `declare -f` is a stream machine, not a
per-genus pretty printer: indentation, a skip-indent counter, a
connection depth, two modes (function body vs reparsed `$( )`), a list
of deferred heredoc redirections, and a one-shot flag that swallows the
next `;` after a heredoc flush. Every rule in the plan's "measured
mechanics" block came from a probe; the ones that would never have been
guessed: `elif` prints as `else` + nested `if … fi`; `<&-` prints
`0>&-`; `for ((;;))` prints `((1; 1; 1))` and each clause loses only its
LEADING whitespace; `cat <<A; b; c` prints `b` without `;` (the flag);
`a && cat <<B || c` puts ` || c` on the line after the body (`&&`/`||`
nest left, so the inner connection flushes before the outer connector)
while `a | cat <<B | c` prints ` |` before the body (pipes nest right);
`$( )` is reparsed and reprinted at parse time (one-line form, a
newline connector kept as a bare newline) but backticks are verbatim;
`$'…'` is decoded and re-single-quoted.

**Decision: the printer copies the machine, not the tree.** The plan
said "one function per genus"; that shape cannot place a heredoc body
after the NEXT connector or suppress a `;` two commands later. The
printer (`crusta_coctum.c`) walks our tree but reconstructs bash's
connection nesting while it prints: a list of n sentences is n−1
left-nested connections (a trailing `&` after `;` binds to the last
command), a catena left-nested, a pipa right-nested with `|&` as
`2>&1` on the left command. Heredoc bodies are reached through the
redirection's `corpus` reference; heredoc nodes in lists are skipped.
The gate's 36 expected texts were generated by running bash (scratch
generator → C table), never typed; 36/36 on the first compile.

**Cross-check beyond the gate (scratch, P11b's preview):** the fixtures
wrapped as the oracle will wrap them — 121 cases, 9 rejected by bash
(no golden), 91 of the remaining 112 byte-identical, 21 not. The 21 by
class, P11b's opening inventory:
- printer: `$( (echo) )` — bash inserts a space after `$(` when the
  reprinted text starts with `(` (ours prints `$((`, a different
  construct) — 4 cases; `\<newline>` inside `${ }` and backticks is not
  dropped (line-cont3/4/8/9/10); inside `$((` bash keeps the
  continuation verbatim (line-cont7) where we drop it; heredoc4.0 lost a
  `;` after `}` (post-heredoc flag or group flush order).
- decoder (`_effugia_decoquere`): `\cX` control, `\u`/`\U` to UTF-8,
  `\c\\` (dollar-quote5/6/7/13).
- parser (arbor gate first): `declare -a c=([0]=z)` → 2 mala;
  `x | esac)` as a case pattern (case2.0); `i\<newline>f` reserved word
  across a continuation (line-cont1); line-cont5/6 mala; heredoc5.0 two
  absent closures where bash reads it.

**Traps met again:** `casus` (case) and `interior` (static) as local
names — examen reports a syntax node and an implicit declaration, not a
macro name. `op`/`fd` are two-letter locals the lint refuses; the
dictionary does not know `asyncus`, `penultimum`, `numeralis`,
`separatores`, `indentare` (renamed before staging). The formator does
not reflow comments: long comment lines are rewrapped by hand.

## 2026-09-16 — P11b (the oracles)

**The environment is part of the golden.** The instrument runs
`env -i LC_ALL=C PATH=/nonexistent /opt/homebrew/bin/bash -r`: a fixed
locale, and a restricted shell with no PATH, because the wrapper only
DEFINES the case — a case that closes the wrapper's `}` early would run
the rest at top level. The header records the version line and this
environment. Probing `\u` escapes from zsh lied twice: zsh's own
`printf` decoded the escapes before bash saw them. The golden written
by the instrument (processus_exsequi, no shell in between) is the
authority: under C, `\u`/`\U` below 0x80 is a byte, anything else is
re-spelled `\uXXXX`/`\UXXXXXXXX` (`\u80` → ``, `\UFFFF` →
`￿`, `ሴ5` → `ሴ` + `5`). Re-spelling can LENGTHEN the
text, so both buffer bounds for `$'…'` (the static-value length walk and
`crusta_effugia_decoquere`) doubled.

**One case list, three consumers.** `crusta_oraculum.{h,c}` enumerates
pathologiae (by title) and FreeBSD (whole file), wraps, and looks up a
golden section by key — the instrument writes with it, the two gates
read with it, so a key can never drift between writer and reader. The
golden format is the case reader's: a `declare -f` output containing a
line `## END` would close the section early, so the instrument refuses
it by name (zero today).

**Numbers.** Birth: oraculum 92/112, differentia 115/121, house mala 7 —
all seven from `pathologiae.sh`, a tracked `.sh`; the corpus gate
already excluded the two fixtures by name, differentia now uses the same
rule (birth 0). After the loop: oraculum 103, differentia 118, house
234/234 `declare -f` exact live (the two without a golden are the
fixtures), sanity 236/236.

**What the classes were.**
- `$( (a) )` prints `$( ( a ))` (a space when the reprinted text starts
  with `(`), but `$((d); e)` stays VERBATIM: bash tried arithmetic,
  refused, and kept the text. Detected in the tree by adjacency (the
  first token of the substitution starts at the byte after `$(`).
- Backticks: backslash-newline removed everywhere inside, even in single
  quotes, when the backslash run is odd.
- A function definition clears the heredoc `;`-flag; a group does not
  (`g() { cat <<A && b … }; c` prints `};`, `{ cat <<A; }; b` prints `}`).
- Assignment builtins after prefixes: `A=1 declare c=(z)`, `>o declare`,
  `>o A=1 declare` fine; `A=1 >o declare`, `declare -a >o c=(z)`,
  `declare c=(z) >o d=(y)` errors. The builder required the builtin to be
  the ONLY child; now the only verbum, last, with no redirection after an
  assignment, and a redirection after the builtin word switches the mode
  back to VERBA.
- Case patterns: `esac` closes the case only as a clause's first word
  without `(`; bash rejects a newline anywhere inside a begun clause. The
  optio frame now lexes in IN_VERBIS (no reserved words, newline a
  separator) instead of EXEMPLAR, which stays for the electio frame. The
  old pin `case x in a esac` moved from 1 to 2 absent closures (both
  invalid in bash; no mode reserves `esac` with newline as separator, and
  the newline rule is worth more).
- Empty compound lists (`{ }`, `( )`, `if then`, `do done`, empty
  tests, `coproc { }`) are bash errors; `case` bodies and `$( )` may be
  empty. New `CrustaParsura.listae_vacuae`, counted when the node closes;
  the tree is unchanged, `sana` FALSUM. (Cases and rule written in one
  edit — the red evidence is the differentia run before: `{ }` sana 1.)

**Left, named (desideratum):** bash removes backslash-newline BEFORE it
tokenises — `i\`+newline+`f` is `if`, `$(\`+newline+`(` is `$((`,
`${v\`+newline+`#…}` is `${v#…}`. Our lector treats the pair as a word
part or trivium, so reserved words, operators and expansion syntax split
across it are not recognised (FreeBSD line-cont1/4/5/6/7/8/9/10). A
lector-level join is a redesign of recognition in every mode. And
heredoc5.0: a heredoc inside a backtick that spans lines, whose outer
body follows the backtick's closing line.

**Tool finding, fixed separately (`c4acd7b0`):** `silva.planta` treated
`error:` ANYWHERE in the gate output as a build break; the differentia
gate prints bash's `syntax error: unexpected end of file` for each
disagreement, so a true red was refused twice (two compiling plants —
compiling the planted copy by hand proved it). Now it matches diagnostic
line shapes.

**computus golden regenerated (cause):** pathologiae.sh's `tabulata`
now parses as an assignment (tokens 396 → 400, nodes 540 → 539, STML
61,897 → 62,525); the two house runners show +1 allocation and +8 pool
bytes (the builtin check now runs after prefixes; the doubled decoder
bound). Trees of house files unchanged.

## 2026-09-16 — P12 (closure)

Plan CLOSED. Nothing was built in P12; the records were reshaped. The
orientation file had grown into a per-task diary (466 lines) and is now
html's shape; the diary's content is not lost — each task's detail is in
the plan's "Executed" notes and in the entries above. Spec §11 records
where the build left the design (19 modes, 176 loci, 16 arithmetic
levels, the cooked view as a stream machine, the oracle environment) and
answers §10 with measurements. The phase-log carries an INTENTIO written
at closure that says so: none was filed before P1. The shim audit ran
396/396 at closure; there was no before-count because there was no
INTENTIO — a lesson for the next client: file the INTENTIO with the plan
commit, and run the shim then.

This worklog stays the find-time record for `crusta_arbor.c` and its
neighbours; new entries go below this one when crusta is touched again
(the tail desideratum 01M2PN1VYH, a formatter, a lint).

## 2026-09-17 — diagnostics: declared, derived, and checked against the counters (B3)

crusta declares its error shapes in the registry (39 attributes: 35
loci with `absentia`/`vacua`, `malum` with `diagnosticum`, three
genera `inanis`), and `materia_diagnostica_derivare` turns them into
located records. No new genus or locus, so the seal did not move:
`873ce8f4` before and after, which is the B1 design claim measured on
a real client. One class has no declarable trace — an `elif` with an
empty condition depends on `tok_deinde` — so the builder emits it
(`_diagnosticum_emittere` → `CrustaParsura.diagnostica`, delta D6).

The four counters stay as an INDEPENDENT oracle
(`crusta_diagnostica_concordia`): per class, presence must agree
(the arithmetic machine counts one trace several times, D7), and
`sana` must be exactly "no diagnostics at all". First run, zero
disagreements: house 235 files (0 errors, 0 warnings — the corpus is
clean and pinned so), oracle cases 121 (mala 13/13, closures 5/5,
empty lists 3/3), adversarial 17 (mala 3/3, closures 10/10), and all
2,934 fuzz cases in the totality gate.

**A green plant is a finding.** Removing `inanis="verum"` from
`separator` left the gate GREEN: no corpus anywhere holds a list of
only separators. Adding sixteen inline shapes (`{ ; }`, `{ }`, `( )`,
`if a; then fi`, `while; do a; done`, `[[ a b ]]`, `(( 1 2 ))`,
unterminated quote and backtick, …) gave the gate its own teeth —
mala 7/7, closures 4/4, empty 7/7, 4 warnings — and then the plant on
`malum`'s `inanis` went red exactly on `{ ; }` and `while; do a; done`.
`{ ; }` holds a `malum`, not a separator (D5 measured this), so
`malum`'s `inanis` is the rule that carries; `separator`'s and
`heredoc`'s mirror `_lista_plena`'s ignore-list and remain
UNEXERCISED by any corpus — kept so the two implementations agree by
construction, named here so nobody mistakes them for tested.

Plants: VACUA counted as a closure → red; `malum` `inanis` removed →
red on the two inline shapes; both green on revert.

## 2026-09-17 — step 6: the derived diagnostics beside bash's own verdict

Two rising pins in `probatio_crusta_differentia.c`, both reading the
DERIVED diagnostics rather than `CrustaParsura`'s counters, so they are
a third path over the same cases: `PINNA_GRAVITATIS` ("no erratum"
agrees with bash's exit 0) and `PINNA_LINEARUM` (the first erratum's
`tractus.linea` equals the `line N` in bash's own error text).

**The line pin, 7/9, and why the two misses are a law rather than a
residue.** All six cases where bash names a token (`syntax error near
unexpected token`) agree on the line. Of the three where bash says
`unexpected end of file`, one agrees and two do not: `crlf` (bash 5, us
2) and `si-apertum` (bash 2, us 1). In both, bash reports the line it
gave up on — the end of the file — and we report the end of the
unfinished construct. Keeping ours is deliberate: pointing at the `if`
that never closed helps a reader more than pointing at EOF. Planting
against it (ABSENTIA's zero-width point moved from the node's end to
its start, in `materia_diagnostica.c`) turns the gate red on exactly
`lineae_concordes` and green on revert.

**The gravitas pin was DEAD when first measured, and finding that out
was the work.** It read 118 — the same number `concordes` read, case
for case. Two plants could not move it: demoting the heredoc-at-EOF
warning to an error did nothing (that case was already discordant for
another reason, so the count never changed), and then removing EVERY
`gravitas="monitum"` from the whole declaration also did nothing. The
reason is exact: no oracle case carried a monitum without also carrying
an erratum, so "no erratum" and "sana" could not disagree anywhere.

The four shapes D9 measured for precisely this purpose — `[[ a b ]]`,
`(( 1 2 ))`, `(( 3 ! ))`, `$(( 1 + ))`, all of which bash accepts with
exit 0 (the first with a message, the rest silently, since bash does
not evaluate arithmetic at parse time) — lived only as inline shapes in
the `diagnostica` gate. They had no bash golden, so `differentia` could
not see them. Appended to `pathologiae.sh` and the goldens regenerated,
gravitas rises to 122 while `concordes` stays at 118: the gap of four
IS the monita, which is what D9 designed the pin to show. The identical
plant — every `gravitas="monitum"` removed — now goes red on exactly
those four cases, and leaves `concordes` at 118 untouched.

**A pin you cannot make fail is measuring its neighbour, not the
world.** This is the same lesson step 3 learned from the green
`separator` plant, met from the other side: there the rule had no case,
here the case had no golden. Both times the fix was to widen the
corpus, and both times the gate was worthless until it was.

**Widening a corpus moves pins in six files.** The four cases moved:
`exempla` 32 → 36 cases and 32 → 36 newline-terminated, plus the tail
assertion (index 31 `parenthesis-vaga` → 35 `arith-operandum-absens`);
`stml` 49 → 53 fixture documents and 47 → 51 identical round trips;
`canon` 47 → 51 judged; `totalitas` 33 → 37 CRLF cases; `oraculum`
121 → 125 cases, 112 → 115 goldens, `declare -f` pares 103 → 106;
`differentia` 121 → 125; and one row of the computus golden
(`pathologiae.sh` 1,735 → 1,883 bytes, 400 → 420 tokens, 539 → 561
nodes — the other four rows byte-identical, which is the check that the
change was local). Regenerated with `COMPUTUS_SCRIBERE=1`.

**The Edit tool normalizes line endings; `pathologiae.sh` carries CR on
purpose.** Editing the fixture through it to change one appended line
silently stripped all four CR bytes from the `crlf` case, and the next
golden regeneration duly recorded bash accepting the file (status 2 → 0)
— a fixture quietly neutered, visible only because the golden diff
showed a case I had not touched. Restored from git and re-appended with
`printf >>`, which is a pure append: 8 insertions, 0 deletions. Same
family as "never `silva.Editio` on a `.sh`" — for a fixture whose BYTES
are the point, only append or write the bytes; never round-trip it
through a tool that may reflow or normalize. **A diff line you did not
intend is the finding, even when the gate that follows is green.**
