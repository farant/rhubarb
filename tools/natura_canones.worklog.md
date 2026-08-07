# natura_canones worklog

## 2026-08-06/07 — Task 4: the monolith, and why most relations do NOT become citations

Two decisions here were measured rather than reasoned-from-the-spec, and both
went against the shape the task brief assumed. Recording them because the
evidence is cheap to lose and expensive to re-derive.

### 1. A citation may only target a LEAF kind

The brief's worked example was `<rosa-canina auctor-nominis="...">` citing
`persona/nomen`. That example, implemented literally, ships a canon that
**rejects correct documents.**

`lib/canon.c` collects citation keys from nodes whose title matches
`ad_elementum` **exactly** (`chorda_aequalis(n->titulus, ci->ad_elementum)`).
There is no element inheritance. But `persona` has 14 `individuum` entries in
natura, and in the monolith every one of them is its own element. So the
entirely reasonable document

```xml
<rosa-canina nomen="rosa-mea" auctor-nominis="linnaeus-noster"/>
<carl-linnaeus nomen="linnaeus-noster"/>
```

has its key under `<carl-linnaeus>`, not `<persona>`, and the citation fires
`CITATIO_IRRITA`. Measured, not predicted — I built the lax canon and ran it:

```
/tmp/subgenus.laxum:2: <rosa-canina> citatio non resoluta
    (clavis absens in scopo): linnaeus-noster
```

The same document under the shipped monolith exits 0.

So a relation becomes a citation only when its target has **no descendants**
(no subgenera, no dictionary entries). Direct children suffice for the test —
a child genus is already a distinct element regardless of its own depth.

Corpus census (640 relation declarations, apparatus-folded, inheritance
included):

| | count |
|---|---|
| `ad="*"` or absent — no key possible | 157 |
| target has descendants — would false-positive | 366 |
| target is a leaf — citation is exact | 108 |
| target unresolvable | 7 |
| target is a dictionary entry (always a leaf) | 2 |

That yields **217 citations** over the 561 elements (higher than 108 because
dictionary entries inherit their genus's relations and get their own).

This is the house rule *porta quae falso clamat neglegitur* applied literally:
57% coverage that is always right beats 100% coverage that rejects real
documents.

**Multiplex relations get no citation at all.** Their citing node is the
relation-child element, and `super=` matches on node title only — it does not
respect `intra=`. Relation-child names are shared across kinds with *different*
targets (`continet` alone has 7), so one citation would falsely fire on the
other 6. Genera are scoped by `intra=`; citations are not.

### 2. The reference side had to widen with the identity

Task 3 typed the identity `nomen=` as `compositum` (nomen + hyphen). Every
reference-shaped attribute was still `nomen`. That combination means an
identity spelled `via-lata-x` is **legal to declare and impossible to
reference**. Proven by narrowing the shipped canon back to `nomen` and
re-running the passing document:

```
valor generi attributi non congruit: inscriptio-eius
```

So in the monolith all relation-origin attributes are `compositum`, not only
the cited ones — in the monolith any relation value may name an in-document
identity, and the canon *limits, it does not demand* (spec §3.4). Forbidding a
hyphen where nothing is resolved would be strictness with no purchase.

`certitudo=` and `fons=` stay `nomen` deliberately: they key into natura's
`<fontes clavis=>`, not into the document. Widening them would be cargo-cult.

Per-module canons keep `nomen` throughout — there the value is a natura key in
snake (§4.4) and the loader resolves it across files.

### 3. Emitter changes, and the contract

`_canonem_emittere` was contracted unchanged. Four changes were unavoidable;
all are model-driven and **inert** for the per-module path — `planta.canon`
regenerates byte-identical to the committed file, and all 33 module canons
judge clean with zero citations and no `compositum` outside the identity.

- `praefatio` parameter. The brief also required a plain-language caveat in the
  generated header, and those two requirements cannot both hold: the signum
  needs `GENERATUM` **and** `NOLI MANU MUTARE` inside `head -3`, so threading
  prose through `fons=` pushes the signum out of the window and silently
  disarms `generata-custos.sh`. A separate comment after the signum never
  moves it.
- Citation emission driven by `NcMembrum.citatio_ad`. The *decision* stays in
  `natura_canones.c`; the emitter only writes what the model already says.
- Multiplex relation `ad=` genus read from `m->genus_valoris` instead of a
  hardcoded `"nomen"` — otherwise the monolith contradicts itself, with simple
  relations accepting a hyphen and multiplex ones not.
- `_generi_congruit` now allows `-` under `compositum`. It is documented as a
  mirror of `lib/canon.c valor_congruit`; `compositum` was previously
  unreachable there and silently judged as `nomen`. Now that we emit it, the
  mirror has to cover it.

### 4. Traps hit

- **My own prose broke a count.** The preface originally contained the literal
  text `<citatio>`; `grep -c '<citatio'` then reported 218 against the tool's
  217. Element names in generated comments are now written without angle
  brackets, and the comment says why.
- The brief's fixture used `nomen="rosa ad murum"` — spaces are illegal under
  `compositum`. Spec §5.3 records this exact example being wrong twice before;
  the brief regressed it a third time. A hand-written example is not evidence.

### 5. `unicitas` over `nomen=` — added in review (both modes)

Spec §5.3 requires it and no task had implemented it. Without it, `nomen=` is
an identity that nothing enforces and §3.2's addressability criterion is
unbacked.

**One identity space across all kinds**, not one uniqueness per kind — §5.3
says *across all kind elements*, and it is what makes a citation unambiguous:
if `<persona nomen="x">` and `<inscriptio nomen="x">` could coexist, then
`ad="…/nomen"` would not name one thing. Verified in both shapes: two entities
of the same kind sharing a name is rejected, and so is a name shared across two
different kinds.

`super=` must list every kind element and is **obligatory** — `lib/canon.c`
skips any node whose title is not in `super`, so an empty `super` judges
nothing at all. That is the mute gate `canon.canon`'s own header (IV) forbids.
Only kinds are listed: parts and multiplex children carry `nota=`/`ad=`, never
`nomen=`, so listing them would judge nothing and bloat the attribute.

The list is **line-wrapped**, which is safe for both readers and was checked
rather than assumed: `chorda_fissio` splits on space and `chorda_praecidere`
trims via `isspace()`, so a newline works as a separator; a conforming XML
reader normalizes it to a space (§3.3.3) and gets the same list. Confirmed by
the firing test — the matched names sit on lines 18 and 19 of a 105-line
attribute.

### 6. The header states relation coverage with LIVE numbers

"The monolith uses real `citatio`" invites the reader to assume relations are
checked. They mostly are not: 217 of 2056 relation sites in the canon. The
preface now prints the census and its causes, computed at generation time.

Deliberately **not** hardcoded in a comment: the corpus grows, and a fixed
number starts lying the moment someone adds a model. A generated file should
say how much of itself it actually checks.

Note the denominator differs from §1's table — 2056 *sites* here versus 640
*declarations* there. Dictionary entries inherit their genus's apparatus, so
one declaration becomes many sites. Sites are what appear in the canon, so
sites are what the canon reports.

### 7. Two review findings, both about the citation predicate lying quietly

Neither was live in the corpus. Both made the generator capable of emitting a
citation it had no right to emit, with no diagnostic — and in both cases the
census would have counted the site under `citatae`, so the preface would have
**overstated what is safely checked**. That is the worst shape a bug can take
here, because the number in the header is the thing a reader trusts instead of
re-deriving.

**A NUL in `ad=` aimed a citation at a genus nobody named.** `chorda` carries a
length, not a terminator, so it can hold an embedded NUL; `chorda_ut_cstr`
faithfully copies it and appends a terminator, and the name lookup then reads
only the prefix. Measured with a control pair:

| corpus | pre-fix emitted | pre-fix census |
|---|---|---|
| `ad="folium_unum\0zzz"` (15 bytes) | `<citatio … ad="folium-unum/nomen"/>` | 0 unresolved |
| `ad="folium_unumzzz"` (14 bytes) | *no citation* | 1 unresolved |

Two byte-strings that differ only by a NUL produced *opposite* outcomes, and
the wrong one was silent. This is the class commit `72149cc` closed on the
**value** path (`NC_VALOR_NULLUS`); I reopened it on the **lookup** path one
commit later, at five sites in a file that previously had zero `chorda_ut_cstr`
calls.

Fixed at the boundary rather than per site: `_cstr_tutum` is now the only
place in this file that calls `chorda_ut_cstr`, it refuses loudly on an
embedded NUL, and the five call sites collapsed to two guarded conversions. The
next person who needs a C string here inherits the refusal instead of
re-earning it. A refused name is reported as unresolvable, which is *true* — no
such name exists in the corpus.

**`_posteros_habet` was blind to `etiam=`.** A res carrying `etiam="G"` genuinely
**is** a G (natura.h, *membrum essentiale duplex*), but `lib/natura.c:1011`
records that as `r->genus_etiam` **without** adding the res to `G->res_suae` —
that index is filled by nesting only. So such a G looked like a leaf and got a
citation it cannot support. Demonstrated by planting
`<species nomen="tabula_lignea" etiam="signum"/>`: pre-fix the generated canon
**rejected a correct document**, naming an absent key for something that really
is a `signum`; post-fix the same document is accepted and the census moves the
site from `citatae` to `posteri`.

Not live today — the corpus's one `etiam=` targets `plagula_fontis`, which is
not a citation target — which is exactly why it needed catching by construction
rather than by observation.

Both fixes are inert on the real corpus: all 33 module canons and the monolith
regenerate byte-identically against pre-fix output (34/34).

### 8. Known gaps handed forward

- `tools/natura_canones.sh` **does not exist**, but every generated canon's
  header says `Regenera: ./tools/natura_canones.sh`. Task 5's to create; it
  must now regenerate **both** modes.
- There is no `probatio_natura_canones.c`. All verification so far is by hand.

## 2026-08-07 — Task 5: the driver and the `-probare` gate

`tools/natura_canones.sh` now exists, closing the gap section 8 handed forward:
every generated canon's header said `Regenera: ./tools/natura_canones.sh`, and
`generata-custos.sh` tells a blocked reader to "muta FONTEM et regenera" — until
now both pointed at nothing. All 34 canons (33 modules + monolith) are committed;
previously only `planta` and `individua` were, which made per-module regressions
uncheckable, since a missing baseline and a regression look identical.

### No `sed` substitutions were needed — and that is load-bearing

`canon_coquere.sh` needs three substitutions because it bakes temp paths into its
output (the regen command, the include guard, the `#include` basename), and a
false-positive bug came from missing one. This emitter bakes **nothing**: the
`Regenera:` line is a fixed literal and provenance names the *module* (`e planta`),
not a plagula. Verified rather than assumed — generated `planta` into an unrelated
scratch path and it came out byte-identical to the committed canon, with zero hits
for `/tmp`, `/private`, `/Users`, `build/`. The detector was itself checked with a
positive control (same grep, pattern `Regenera`, hits), because a `grep` that
matches nothing and a `grep` that is broken print the same thing.

**If a substitution ever becomes necessary, that is the signal the emitter has
started baking a path.** Report it; do not work around it in the shell.

### Birth test: both halves, because only the pair proves anything

- Semantic fault (new `<proprietas>` in `planta.genera`) → `RANCIDUS` on
  `planta.canon` **and** `individua.canon`, exit 1. The fault was confirmed real
  *before* trusting the verdict — it changes emitted output at 7 sites, since it
  inherits down the whole planta chain.
- Comment-only edit → exit 0. This is the half that distinguishes a semantic gate
  from an mtime check wearing its clothes. `git diff --stat` confirmed the file was
  genuinely modified, so the green is not vacuous.

An mtime gate would cry wolf on every comment edit, and a gate that cries wolf is
one people learn to ignore — which is how a gate dies while still appearing to run.

### Guard: the gate must never exit 0 having compared nothing

Three defect modes, all exit 2, all exercised: missing/stale binary (named the
culprit file), unknown argument, empty corpus. The argument check matters more than
it looks — a typo'd `-probar` silently falling through to the write path would turn
"judge this" into "rewrite this", which is the same class of bug as gating on a
piped `$?`.

The empty-corpus guard is **defense in depth, not a fix**: measured, the tool itself
already exits 2 there (`NULLUM exemplar in 'natura'`). The script-level guard buys a
message naming the real cause and an assertion that does not depend on the tool
continuing to behave that way.

Also: `rm -f` the temp before each generation, and require the output non-empty
afterwards. A tool that exits 0 while writing nothing would otherwise leave the
gate comparing the *previous* module's bytes.

### Per-invocation stderr is filtered by path, not by wording

The tool re-emits its corpus-wide warnings (5 lines: property genera outside the
canon) on all 34 invocations, plus one per-target `'X' scriptus` progress line.
Showing everything is 200 lines of noise in Task 6's hook; showing the first
invocation verbatim is worse — it names one module, reading as though only one file
was written, and in `-probare` mode it leaks the temp path.

So novel warnings are always shown and never suppressed (visible degradation is
house law), while the progress line is dropped by filtering on the *target path*
rather than on the message text: the wording may change, the line's job may not.

**Correction, same day — the first version of this got it backwards.** What
shipped first was a boolean latch: show the first invocation's stderr, then go
silent. The justification written into the code was that the querela is *"eadem
omni vocatione quia de corpore toto est"* — identical every time because it
describes the whole corpus.

**That premise is false, and measuring it takes one loop.** Running all 33 modules
individually, eight emit warnings the corpus-wide set does not contain — `actus`
(1 value flattened), `identificatio` (15), `norma` (4), `opus_scriptum` (2 values
+ 5 defaults dropped), `lingua_programmandi` (4 defaults), and `organizatio` /
`persona` / `plagula_computatralis` (14 each). Worse, the **monolith is generated
last**, so the latch guaranteed its warnings were never seen — and it emits the
largest set of all: 217 citations, 22 values flattened, **51 defaults dropped**.

So the latch destroyed precisely what it was written to protect. These are
lossy-emission notices — values flattened, defaults that never reach the canon —
in the one tool whose stated doctrine is that degradation is never silent. And it
applied in **both** modes, since the redirect is unconditional, so plain
regeneration was equally mute.

The fix is to **accumulate rather than latch**: each line is printed unless some
earlier invocation already printed that exact line. Repetition stays quiet;
novelty always prints. Output went from 6 lines to 15, and the monolith's three
now appear.

*Known limit, stated rather than left to be found:* dedup is by exact message
text, so when three modules emit an identical `praestituta 14 omissa`, it prints
once. No distinct fact is lost, but multiplicity and module attribution are. The
messages do not name their module, and prefixing them would either misattribute
the corpus-wide lines to whichever module ran first or reinflate the output to
~180 lines. Run a single `-modulus` if you need to know which model a warning came
from.

The general lesson is the one this file keeps relearning: **a comment asserting a
property is not evidence of it.** The premise was plausible, cheap to check, and
wrong, and it sat in the code as a confident justification for the bug.

---

## 2026-08-07 — Registry dispatch, the `.genera` hook, and a race the hook would have lit

Task 6: bind instance documents to the generated canons, and make a stale canon
announce itself on save.

### Extension keys only, and why the missing root key is the design

`canones.registrum` states **RADIX VINCIT** — a root-element key beats an
extension key. Both `individua.canon` (monolith) and `planta.canon` root at
`<individua>`. So a `<individua>` root key would bind *every* instance document
to whichever canon claimed it, and it would do so silently, because being
judged against the wrong canon still produces a confident verdict.

Measured, on one byte-identical document containing a `<canis>`:

    hortus/vivarium.individua  ->  individua.canon  ->  exit 0
    hortus/vivarium.planta     ->  planta.canon     ->  exit 1
        <individua> liberum hic non licet: canis
        <canis> elementum extra canonem: an rosa-canina?

Same bytes, opposite verdicts, and both correct — that difference is exactly
what a root key would have erased. A comment above the two lines records this,
because the omission looks like an oversight and invites a "fix".

The other 32 module canons stay out of the registry and are reached with
`canon_examen -canon <via>`. That flag already existed (`canon_examen.c:421`);
the brief's step 3 was obsolete.

### The hook now emits from one place, because it grew a second axis

`natura-custos.sh` previously emitted-and-exited inside each branch. That kept
the one-JSON-per-invocation contract only as long as nobody added a branch. The
rancour gate *is* a new branch and it is **orthogonal** to the others — a model
can be semantically sound while its cooked canon is stale, and both facts want
saying in the same breath. So messages now accumulate into `NUNTIUS` and emit
once at the end: the contract is held by structure, not by remembering.

Verified with `jq -s length` = 1 while both a VULNUS and a rancour line were
present.

### Exit 1 and exit 2 must not give the same advice

- exit 1 = `RANCIDUS` -> regenerate. The stale canons are **named** in the
  message (editing `planta.genera` correctly names both `planta.canon` and the
  `individua.canon` monolith).
- exit 2 = the gate did not run. Here the message passes through the tail of the
  tool's own stderr rather than hardcoding "run natura_struere.sh".

That last choice earned itself immediately. A *third* exit-2 shape showed up in
testing that the brief did not anticipate: when the `.genera` has a semantic
fault, the generator refuses to emit a canon it could not load back
(`canon se ipsum onerare non posset, RECUSATUR`) and exits 2. Hardcoded "the
binary is stale, rebuild it" advice would have been simply wrong there.
Telling an author to regenerate with a stale binary is precisely how stale
output gets blessed, so the two paths are kept apart.

### The race: pre-existing, harmless, and armed by this hook

`PROBANDUM`/`QUERELA`/`MONSTRATA` sat at fixed paths under
`build/natura_canones_tmp`. Two concurrent runs interleave, and a `PROBANDUM`
written by *another* run compared against a real canon yields a **false
RANCIDUS**. Nobody ran this concurrently before — but the hook fires on every
save, and two quick `.genera` saves is ordinary editing. A `$$` suffix plus an
EXIT trap closes all three at once.

Proven rather than assumed. Six concurrent runs, old fixed path:

    exits: 2 2 1 2 0 0
    natura_canones: RANCIDUS natura/cocta/norma.canon   <- FALSE

Six concurrent runs, `$$` path: `0 0 0 0 0 0`, no leftover directories.

*Cost:* the hook goes from ~0.03 s to ~0.95 s per `.genera` save, essentially
all of it the 34-canon regenerate-and-compare. Judged worth it — the thing it
prevents is generated output silently drifting from its source in a committed
tree — but if `.genera` editing ever gets interactive this is the line item.

### Unrelated finding, left alone

The library carries **3 pre-existing rule-8 wounds** at HEAD, so the hook is
never silent on a `.genera` save. Both predate this task series
(`git log -S` -> 7099651, 4f9f82e):

- `inversa` has `genus="veritas"`, but `pharmacon`/`plagula_computatralis` use
  it to name the inverse relation (`inversa="coquitur"`) rather than assert a
  boolean. The model and the usage genuinely disagree; someone has to decide
  which is right.
- `vectura` has `quando="-312"` against `genus="dies"` — the library models
  Roman roads and cannot currently express a BC date.

Not touched: out of scope, and neither is a regression.
