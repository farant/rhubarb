# toml_arbor worklog (client toml, plan project-specs/toml-arbor-plan.md)

## 2026-09-28 — Q4: the builder, and three finds

**Newline by mode needed a second switch in the lector.** Q3's lector
decided LINEA_FINIS vs LINEA by bracket depth alone, so a blank line or a
comment-only line at the top level produced a substantive newline — a
`linea` node per blank line and comments bound to newline tokens. Added
`toml_lector_sententiam_ponere`: a newline terminates only when a
statement (pair, header, malum) is open; otherwise it is trivia. The
builder sets it from the frame on top (anything but the document frame).
Same law as crusta C6, one more input to the lector's function.

**TOML's trivia rule is not crusta's.** crusta binds backward through the
LAST newline (a heredoc body must follow its newline). TOML: after a
line-ending newline, or at the start of the document, everything pending
goes ANTE the next token (a comment above a key belongs to that key; so do
blank lines); otherwise trivia up to and including the FIRST newline goes
POST the previous token (`a = 1 # c`: the comment belongs to the value;
inside an array `1, # c⏎ 2` binds to the comma's line), the rest ANTE the
next. The initium_lineae flag follows the STML reader's derivation
exactly (a LINEA trivium intervened; LINEA_FINIS, being substantive, does
not count) — crusta P7's lesson, applied from the start.

**An empty file arrives as a NULL pointer.** `filum_legere_totum` returns
`datum == NIHIL` for a 0-byte file; `toml_lector_incipere` refused a NULL
source even with length 0, so toml-test's `valid/empty-nothing.toml` was
the one byte-law failure of 679 on the corpus gate's first run. The lector
now treats (NIHIL, 0) as "". Pinned in the arbor gate.

Recovery design, as built: a bad token at statement level starts a malum
that absorbs to the next LINEA_FINIS (statement open ⇒ that newline IS a
terminator); inside brackets to the next ',' or closer; a bad VALUE
becomes the pair's valor (so `a = hello` keeps its key). Key segments
must alternate with dots — `a 1`, `a..b` end the key and the rest of the
line is a malum; a trailing dot stays in the tree for cooking to judge.
A pair aborted mid-line counts its missing '=' and value as absentiae
(the declaration then derives both diagnostics); one error can therefore
yield two or three diagnostics on a line — named here, revisit in Q7 if
the rendering reads noisy.

---

## 2026-09-28 — Q7a scalar cooking (toml_scalaris)

**The measurement drove the slice.** 266 invalid toml-test cases parse
clean; 201 of them are scalar (control, encoding, string, integer,
float, the four datetime directories). The gate asserts exactly that
number is now rejected by cooking, and the 73 other scalar-category
cases by syntax, so a regression in either layer moves a pinned count.

**One owner for scalar diagnostics.** Two walks could each report a bad
escape: the future table walk (Q7b) that cooks values, and a token walk
that sees comments. Decided: `toml_scalaria_iudicare` (the token walk)
owns every scalar diagnostic; value cooking takes diagnostica NIHIL. The
token walk also covers quoted keys and comments, which a value walk
would miss.

**Malum substance is not judged; its trivia are.** A string inside a
`malum` already sits under a syntax error; judging it too would put two
diagnostics on one mistake. Comments attached to malum tokens are still
judged (a control byte in a comment is its own error).

**Numbers: classify before judging.** The decimal branch decides
integer vs float by scanning for `.`/`e`/`E` first, so `03.14` is
"fluitans pravum: zephyra praevia" and `007` "integer pravum" — the
message names what the author was writing. Hex digits are checked
against the base with a second lookup at base 16 to tell "digit outside
the base" (`0o8`) from "unexpected character" (`0xg`).

**Wild check (probe, not a gate yet):** 35 wild files get a scalar
diagnostic; joined against `build/aurum_silvestre.txt` (paths there
use `~`), all 35 are tomllib-INVALIDUM (toml_edit and CPython test
fixtures). Q9 makes this a gate.

**Second 60.** RFC 3339 allows a leap second; tomllib (Python datetime)
refuses 60. The plan said allow; kept, named here and in the header for
Q9's differential.
