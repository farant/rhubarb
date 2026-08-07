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

## 2026-08-07 — Task 8: the corpus assertions, and natura's two debts

Tasks 1-7 were hand-verified end to end and **none of it survived into CI**.
This closes that: `probationes/probatio_natura_canones.c`, 67 assertions,
0.4 s.

### The test-design law, and what it cost

Task 4's implementer put the rule that governs this file: asserting *"no
citation emitted"* alone **would have passed before the fixes too**. What
discriminated was the census reclassification — a site moving from one bucket
to another. So the question asked of every assertion here was *what would this
have done yesterday?*, and where the answer was "passed", the assertion was
rewritten as a **pair whose halves differ in one dimension and where one half
must go red**:

| pair | the half that must fail |
|---|---|
| document that resolves its citation / one that dangles | the dangling one — a canon with *no* citations accepts both |
| name corpus that is clean / two that collide | the clean one — it proves the write path works, so exit 2 elsewhere is the guard and not a failed `fopen` |
| natura part `necessaria="verum"` / `necessaria="falsum"` | neither — the point is they come out **identical**, which is the leniency doctrine |

Every section was then driven red by a planted fault and restored. The matrix,
run for real:

| planted | assertions that went red | that stayed green |
|---|---|---|
| delete 1 of 217 `<citatio>` from the monolith | dangling doc now legal (2); header number ≠ emitted count (2) | the resolving doc — correctly, it never needed the citation |
| `minimum="1"` on rosa-canina's `radix` | the site assertion + the corpus sweep | `flos`, its non-necessary twin |
| preface says 2056 → 2057 | census arithmetic | everything else |
| second `radix="verum"` in `animal.canon` | radix uniqueness | — |
| `<unicitas>` removed from `animal.canon` | unicitas presence | — |
| `<citatio>` added to a per-module canon | per-module-citation-free | — |
| collision fixture **de**-collided | exit code, file-written, stderr-names-the-guard | the kebab-fold fixture — the two shapes bind independently |
| citation aimed at `locus-qui-non-est` | the audit (`malae 1`) | the census — count unchanged, which is exactly the shape of the two bugs Task 4 fixed |

That last row is the one worth remembering. Both Task 4 bugs (NUL in `ad=`,
the `etiam=` blind spot) emitted a citation **into a genus the author never
named** while the census counted the site as resolved. Count-based assertions
cannot see that; the audit can, and it is now permanent rather than a one-off
review.

### Numbers are NOT pinned

Nothing in the test says "217 citations" or "34 canons". A fixed number starts
lying the moment a model is added — the same argument the generated preface
makes about itself. What is asserted are **relations**:

- `citatae == xar_numerus(canon->citationes) == count of <citatio> nodes`
  (prose in the header bound to structure on disk)
- `apertae + posteri + multiplices + ignotae == nudae`, `citatae + nudae == sedes`
- **`count(natura/cocta/*.canon) == count(natura/*.genera) + 1`** — derived, so
  it also catches a new model whose canon was never generated

### Two things the test needs that a probatio usually does not

**It spawns `bin/natura_canones`.** The collision guard is `interior` in a
file with a `principale`, so it cannot be linked; the only honest test is to
run the binary on a corpus with a planted collision. Fixtures are committed
under `probationes/exempla/nc_nomina_{sana,gemina,kebab}/` — two `.genera`
files each, outside the `natura/` glob so no other tool sees them. If the
binary is absent the test builds it once via `natura_struere.sh`; **staleness
is deliberately not handled here** — that is `natura_canones.sh -probare`'s
job, and a test that reddens because a tool was never built says nothing
about the tool.

**It uses a scratch piscina per file** in the 34-canon sweep. The monolith
alone is 640 KB of source; 34 trees held at once would be held for no reason.

### The gate chain had a hole; the hook narrows it, the TOOL closes it

**Correction, 2026-08-07 (review).** An earlier version of this entry said the
hook alone closed the hole. It did not, and the difference is one commit wide.

Task 7 measured the shape: `natura-custos` gates the canon, `canon-custos`
gates the reader, and **nobody ever edits a generated canon**. Regenerating via
`natura_canones.sh` is a tool write, not an Edit, so no hook fires and the
reader goes stale behind a fresh canon, silently.

`natura-custos.sh` now runs `canon_coquere.sh -probare` as a third gate. Exit 1
and exit 2 get **different** messages, mirroring the canon gate directly above
it — "regenerate" is the wrong advice when the gate could not run, because
regenerating with a stale tool is precisely how stale output gets blessed.

**But the hook is one edit LATE, measured by walking the real workflow:**

| step | what happens | who says anything |
|---|---|---|
| edit a `.genera` | canon now stale; reader still matches the **old** canon on disk | gate 2: `canones cocti RANCIDI`. Gate 3 correctly silent — nothing is stale yet |
| run `natura_canones.sh` as instructed | canons rewritten — **this is the moment the reader goes stale** | *nobody.* A tool write, not an Edit. The tree can be committed here |
| next `.genera` edit, whenever that is | — | gate 3, at last |

So the hook covers the *steady* state and misses the *transition*, which is
exactly the window in which someone commits.

**The fix is in `natura_canones.sh` itself**: after regenerating (non-`-probare`
mode only) it runs `canon_coquere.sh -probare` and, if readers are now stale,
prints `CATENA INCOMPLETA` and **exits 3**. The tool that creates the staleness
is the one that reports it, at the moment it creates it.

Two deliberate limits:

- **It does not regenerate the readers.** `cocta.registrum` carries entries with
  nothing to do with natura, and one regenerator silently driving another
  couples things that should stay separable. Message and exit code, not action.
- **Exit 3 is new and documented in the usage header.** Only humans invoke the
  bare form (grepped: the hook uses `-probare` exclusively), so the stronger
  contract — *exit 0 means the whole chain is fresh*, not merely *I wrote some
  files* — costs nothing and stops `... && git commit`.

A message-only fix (gate 2 advising "…then run canon_coquere.sh") was considered
and rejected as the primary mechanism: a gate whose closure depends on a human
following an instruction is the thing gates exist to replace. The advice is in
gate 3's message as well, but it is not what closes the hole.

Birth-tested in four states, since a hook that prints nothing looks identical
healthy or dead: clean (baseline only), reader stale, **both** stale (one JSON
object, three messages, chain order), and `cocta.registrum` removed (exit 2
message naming the cause). One `hookSpecificOutput` envelope throughout — the
accumulator introduced for the second gate is what makes the third free.

**Side fix, forced by the change.** `canon_coquere.sh` used a *fixed*
`build/canon_coquere_tmp`. Task 6 already hit this on `natura_canones.sh` and
solved it with `$$` + `trap`; adding a second trigger to `canon_coquere.sh`
recreates the race, so it got the same treatment. Verified the gate still
fires on a planted fault and leaves no directories behind.

### Do NOT generate the monolith's reader

Measured, not guessed (Task 7): planta's **72 elements → 5,312 lines**. The
monolith has **1,982**, i.e. ~27.5×, extrapolating to **~146,000 lines /
4.2 MB in one translation unit** — which would link into *every* test binary,
the way `planta_lectio.c` now does. The monolith's reader wants splitting,
probably by module, and that is its own piece of work with its own design.
Per-module readers are the usable artifact today.

### Coverage is narrow by design — do not over-read it

`probatio_planta_lectio` exercises **2 of 7 kinds** and none of
`generat`/`laborat`, so the non-empty multiplex-child path in the generated
reader is **compiled and never executed**. That was deliberate — Task 7's goal
was proving the chain, not covering the canon — but a later reader could
mistake a green suite for coverage.

### The chain proof, so it survives compaction

Change `natura/planta.genera:190` `<valor nomen="habitus">frutex</valor>` to
`scandens`, then:

```
bin/natura_canones -modulus planta -radix <dir> -ad <file>
bin/canon_coquere <file> -praefixum Planta -caput ... -corpus ...
```

The value lands in the generated C as `PLANTA_ROSA_CANINA_HABITUS_SCANDENS`
and turns **exactly one** assertion red. One `<valor>` in natura, two
generations, one C enum member.

---

## 2026-08-07 — Task 8 fix round: three things the review found

### 1. `intra=` had NO guard anywhere, and canon cannot supply one

The brief asked for a scoping assertion; I did not write it, and worse, the
test's own element lookup **deliberately skipped** `intra=` elements, so
nothing in the file touched spec §4.2 at all.

The reviewer measured the cost. Simulating the generator dropping `intra=`,
producing two `<elementum nomen="radix">` in one canon:

```
dup radix elements now: 2
canon_examen: plagulae 1 / VITIA 0        (exit 0)
probatio_natura_canones:  67/67, Fracti 0
```

Both watchers silent — and **`canon_examen` cannot catch it by design.**
`canon.canon` records that unicitas runs document-wide, so *"element name once
per canon"* is inexpressible **precisely because `intra=` makes repeated names
legal**. This is the same shape as the `<optio>` mistake I caught on myself: a
thing canon cannot say, where a test was the only possible coverage.

Reproduced both scenarios myself, and `canon_examen` exits 0 for each.

Fixed in two layers:

- **the site pair** the review specified — `<elementum nomen="radix"
  intra="rosa-canina">` exists **and** no *unscoped* `radix` does. The second
  half is what catches the regression; the first alone would survive a
  generator that left the scoped element and added a bare one.
- **a corpus-wide invariant**, `_nuda_gemina_numerare`: across all 34 canons,
  **unscoped element names must be unique within a canon**. Scoped ones share
  names freely (`historia` appears in nearly every kind); unscoped ones cannot.
  Dropping `intra=` collapses the second set into the first and they collide.

Measured: dropping `intra=` from one element reddens 2 assertions; dropping it
from all of `planta.canon` reddens 4 and reports **54 colliding bare names**.

### 2. The suite green-lit a generator whose source no longer had the guard

Nothing tied `bin/natura_canones` to its source and `compile_tests.sh` never
builds it. The reviewer neutered the collision guard in
`tools/natura_canones.c`, left the older binary in place, and got:

```
Totalis: 67  Praeteriti: 67  Fracti: 0   ✓ TEST PASSED
```

The assertions themselves are sound — rebuilt against the neutered source, the
6 guard assertions go red. Only the **input** was unpinned.

Now `_fons_recentior_binario` stats the binary against
`natura_canones.{c,h}` + `natura_canones_emissio.c` and fails with
`STALUS ('<file>' recentior) - strue primum`. **Absent and stale stay
different causes:** absent builds once (a suite that reddens because a tool was
never built gets ignored), stale **refuses** — certifying a binary that no
longer matches its source is worse than not testing it. Both paths verified.

This is the `-o /dev/null` lesson in another costume: *proving you CAN compile
is not proving you DID.* Here, proving a binary passes is not proving that
binary came from the source in the tree.

### 3. Accounting correction

I reported **122/122**. In an isolated worktree the suite is **122/117/5**, with
the same five failing identically at parent `b06135e` — pre-existing and
environment-dependent. The attestable claim is **+1 test, +1 passing, 0 new
failures**; "122/122" was a main-tree figure and should not have been stated as
the suite's condition.

### Deferred (review Minors, recorded so they are not lost)

The partial-sweep `frange` lets zero counts read as success; gate 3 truncates
with `head -5` where the gate directly above computes an overflow count under a
comment reading `truncatio TACITA nulla`; `mkdir -p` unchecked in
`canon_coquere.sh`; `citantes_per_modulum` counts two distinct failures in one
counter; the new fixtures trip `natura-custos` on edit.

---

## 2026-08-07 — fix wave before merge (four items from the whole-branch triage)

Sixty lines across six files, none of them changing emitted output: the 34
canons and both generated readers are byte-identical to `6106ae0` afterwards.
Method for every item: build the pre-fix binary from `HEAD` beside the fixed
one and run **both** against the same planted condition. What follows is what
the measurements said, including where they contradicted the triage.

### `fclose` was the last place a wrong artifact reached git under a success message

Forced with a 600 KiB HFS+ RAM disk rather than argued about. Filled to leave
less space than the canon needs, the pre-fix binary wrote **466,944 of 590,145
bytes** of the monolith — truncated mid-token, `<attributum n` — printed
`'…' scriptus (elementa 561)` and **exited 0**. The shell gate only checks the
file is non-empty, so it passes; `&& git commit` commits it. Same shape at the
per-module site: 16,384 of 21,435 bytes.

The general lesson is the buffering one: **`fwrite` succeeding is not the write
happening.** Everything still in the stdio buffer lands at `fclose`, so
`fclose` is where disk-full and quota surface — and it was the one call whose
return nobody read.

### `%.*s` stops at a NUL — the triage had this door misclassified

The review listed `fprintf(f, "<optio>%.*s</optio>", …)` as writing a raw NUL
into the file, alongside `_kebab_scribere`. Measured, it does not: a precision
does not make `%s` read past a null terminator (C89 §7.9.6.1). So the three
name-path doors had **three different failure modes**, not two:

- `_kebab_scribere` — `putc` per byte, genuine raw write. **4 raw 0x00 bytes**
  measured in a canon reported as `scriptus`.
- `_appellatio_emissa` — copies the NUL, terminates after it. Declared
  `folium\0zzz`, emitted `nomen="folium"`. Silent substitution.
- `<optio>%.*s` — silent truncation, no NUL in the file.

Worth keeping because the *fix* is the same one sweep either way, but the
*diagnosis* would have sent someone looking for raw bytes that were never there.

### Assertions that compare an accumulator against zero cannot see a short sweep

Five of the six corpus assertions are `accumulator == ZEPHYRUM`, and an
accumulator never incremented is zero. So a `frange` out of the sweep passes
all five while the summary prints the number it *intended* to read. Planting
`frange` on the **last** iteration (the monolith is third-from-last in
directory order, so `monolithi == 1` still holds) reddened **only** the new
`recensiti == xar_numerus(canones)` assertion — nothing else noticed 33 of 34.

Generalisable: any sweep whose assertions are all zero-comparisons needs a
completed-iteration count, or it reports on the corpus it meant to read.

### `-nt` against a missing file is false, so a renamed source leaves a guard silently

Both freshness scripts degraded this way, and `include/stml.h` was missing from
both lists while `include/canon.h` sat correctly beside `lib/canon.c` in the
same list — the right pattern present and applied to one library only.
`NC_FONTES` in the probatio claimed parity with the shell list under a comment
saying so while listing half of it; with `lib/natura.c` newer the shell gate
refused at exit 2 while the suite stayed green at 73/73 on that same binary.

One sentence covers all of it: **a guard must fail loudly when it cannot check,
and must not claim coverage it does not have.**

### A gate that refuses valid input is a dead gate, and this one was one quote away

`_valorem_planare` applied attribute-value rules to element text. A `"` is
correctly fatal in an attribute; in `<optio>` content it is legal STML and legal
XML. Measured: one quote in one option made the pre-fix binary abort that
module's entire canon. Zero quoted options in the corpus today, so it was
latent — but unlike every other deferred item it is triggered by prose a person
writes, not by a corrupt byte. Now parameterised (`quota_licet`), and the
quote-bearing canon it produces passes `canon_examen` at `VITIA 0`.

### `_stirpem_scribere`: unreachable, and still undefined behaviour

`i32` is unsigned, so a title shorter than `.genera` wrapped, clamped to 255,
and `memcpy`'d 255 bytes out of a shorter chorda. Driven directly through a
throwaway harness (`#include` the translation unit with `principale` renamed):
AddressSanitizer reports `stack-buffer-overflow / READ of size 255`. The guard
that made it unreachable had drifted into a different loop from the use. A
function that does not hold its own precondition loses it the moment a second
caller is born.

### Tooling note

`./silva/examen.sh` returned `ACCIPE` on a file `clang -Werror` rejected for
`'/*' within block comment` — I had written `tools/*` inside a block comment.
The build caught it immediately, but `-Wcomment` is a gap in the C89 judge.

## 2026-08-07 — clausura pro folio: subsumptio compilata

The leaf rule (`petitum resolvi debet ET folium esse`) is gone.
`_petitum_citabile` now returns the target's CLOSURE — the kind,
its subgenera transitively, their res, plus etiam=-bearing res —
and the emitter writes it as canon's new list-form `ad=`. Same
move as the inheritance fold: canon stays extensional, the
generator translates intension at build time.

Measured: citations 217 → 1021 (the 804 posteri sites, to the
site). Census causes now apertae 438 + multiplices 587 + ignotae
10 = 1035; NC_CIT_POSTERI deleted. Section VI's audit got
STRONGER: every title of every index must resolve AND declare the
key — 1021 recensitae, malae 0.

Trap honored from the old `_posteros_habet`: etiam= res are NOT
in `res_suae` (loader fills that index by nesting only), so the
closure does a separate pass over res_omnes against the collected
genera set. Same blindness that bit the leaf predicate once.

Res targets stay single-title closures: NaturaRes carries no
posteri IN THE GRAPH (nested species land in the enclosing
genus's res_suae, flat) — if a relation ever targets a res with
ontological descendants, that is a loader question, not an
emitter one.

## 2026-08-07 (later) — signa emitted + per-module citations

nomen= now emits genus="identitas", relations genus="referentia"
in BOTH canon kinds — the nomen/compositum typing divergence died
as a side effect (it was a task-boundary scar: the monolith task
widened relation typing under `si (monolithus)` and nobody
revisited per-module because nothing forced it).

Per-module canons now cite wherever the target closure lies wholly
inside the module (`_clausuram_colligere` gained
modulus_requisitus; impure closure → NIHIL → NC_CIT_ALIENA, which
never reaches the census because the monolith passes requisitus
NIHIL). Measured: communicatio 2, lingua_programmandi 60,
elementa_programmatis 33, others — the "per modulum nullae esse
debent" gate in the corpus audit INVERTED to "moduli VERE citant".

The module with the intra-module nuntius→actor pair is
communicatio, not a "nuntium" module — the plan guessed the
filename from the citation name and the verification grep caught
it. Modules are files; citations are named after entities.

hortus.planta migrated (all identities sigiled); planta_lectio's
expectations updated to the VERBATIM sigiled values — the reader
strips nothing, per spec. Readers were byte-identical after
regeneration (coquere maps both old and new genera to verbatim
chorda*), so -probare exit 0 was honest, not stale.

## 2026-08-08 — census: individua become data, semina.census born

Third projection output shipped. Individuum entries are no longer
tags (element count 1982 → 1522; closures taxonomy-sized; the
impostor hole and `.carl-linnaeus` nonsense died); `.census` is the
one document extension (`.planta`/`.individua` retired, module
canons live on as opt-in profiles via `canon_examen -canon`);
`semina.census` = the 112 dictionary individua as generated
instances of their kinds' tags — own-node valores as attributes
(inherited defaults stay in the tag), historia transcribed, nota
skipped, relation values signum-prefixed by target type.

The self-judging gate (generated data under generated grammar)
found TWO real corpus wounds on first contact: B and ABC carried
vita=mortua, a state the lingua machine never had (→ derelicta,
the machine's own terminal); via_appia's quando="-312" is the BC
date the dies genus can't say — third ictus on 01KZC7F388, wound
TOLERATED by name in the suite (omnia==I with the res id; when the
ruling lands, the assertion goes red and the tolerance is deleted
— deliberately).

Structural notes: `_elementum_aedificare` + `NcCensusRelationum`
moved to the shared header (emissio calls the plicator per
individuum for member appellations); relations whose relatio-level
ad= names an individuum fall back to the KIND's closure (any
enrolled persona satisfies — "Linnaeus himself" is beyond canon,
deliberately); enrollment-by-kind worked BEFORE the exclusion
landed (ad= lists always carried the kind titles), which is what
let the hortus migration ship standalone-green first.
