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
