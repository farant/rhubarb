# js_lexema worklog

## 2026-08-18 — initial implementation (the two named hazards)

Shipped 356/356 first logic-run (one -Wcomment fix: a literal `/*`
inside a block comment; one gate collision, below). The decisions:

**Regex vs division — the heuristic and its edges.** `/` is judged
by the PREVIOUS SIGNIFICANT token (trivia skipped), tracked as
genus + span in the lexator. Division after: identifier, number,
string, regex, template TOTA/FINIS, the value-bearing keywords
(`this true false null super`), and punctuation ending in `)` or
`]`. Regex after: everything else (other keywords, other
punctuation, start of input, template INITIUM/MEDIA). Two edges
worth remembering:
- after `}` → regex (block-end presumed; object-literal-then-divide
  is essentially unwritten). After `)` → division ALWAYS — this is
  the house simplification; `if (x) /re/.test(y)` would mislex, and
  the arbor is the place to re-judge if that ever matters.
- `++`/`--` are presumed POSTFIX → division (`a++ / b`). Prefix
  `++` followed by a regex is not a thing anyone writes.
- Comment check comes BEFORE the regex decision: `//` and `/*` are
  never regex starts.

**Template literals — the stack.** Split esprima-style: TOTA
(`` `x` ``), INITIUM (`` `x${ ``), MEDIA (`}x${`), FINIS
(`` }x` ``). The lexator keeps a stack of open interpolations, each
entry counting `{` opened inside it; a `}` resumes the template
ONLY when the innermost count is 0 — that's what makes
`` `x${ {a:1}.a }y` `` come out right (inner `}` is punctuation,
outer resumes). Depth cap 64; past it, `${` stays inside the
template span — degradation, never a crash. Templates span
newlines as single multi-line tokens (the coloring pipa's
line-splitter already handles that class).

**One INTERPUNCTIO genus, maximal munch.** `>>>=` down to singles,
with the ES2020 `?.`-before-digit exception (`c?.5:d` is ternary).
Guessing 45 operator genera today would lock the API; the arbor
gets its own classifier over spans when it arrives.

**Keywords.** JS_LEX_VERBUM = ES5 reserved + let/const/class/
extends/super/static/yield + this/true/false/null. The
division-context subset lives in a separate internal table.

**Gate collision worth remembering:** the repo-wide js_syntaxis
pre-flight (JXA `new Function`) sweeps ALL `*.js` — including a
deliberately-broken adversarial fixture. Resolution: the
adversarius is named `adversarius.textus` (fixtures are DATA); the
corpus loop reads every file regardless of extension. Bonus: the
sweep now validates the three CLEAN snapshots as real JS — a free
second oracle of a different kind on the fixture corpus itself.

**Corpus note.** componentia.js (113KB, regex-bearing) lexes with
zero imperfecta/ignota and roundtrips byte-exact — the heuristic's
real-world gate. House JS is pure ES5 (zero backticks/arrows), so
the ES2015 paths are carried by the hand-written
dialectus_2026-08-18.js fixture, which must also stay clean.
