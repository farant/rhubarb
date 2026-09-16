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
