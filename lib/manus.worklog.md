# manus.worklog.md

## 2026-08-13 — birth, and the bug that only a passing test could find

`manus` is the ergonomic layer over `imperium`: press, type, **wait**,
judge. Design and rationale live in `manus.h`; this file is the case law.

### The one that matters: a library that always failed, and looked fine

`_credo_notare_chorda` takes six arguments and **no condition**. It is the
primitive behind `CREDO_CHORDAE_AEQUALES` — it compares `valor_primus`
against `valor_secundus` and passes if they're equal.

I used it as if it were a general "record a verdict plus two values"
recorder. So every manus assertion compared the *seen* value against the
*expected* value as strings:

```
"1"  vs  "I aut plura elementa"     → never equal → always FRACTA
```

Every assertion in the library failed unconditionally, regardless of what
the page reported.

**And all three failure tests passed while this was live.** They were
supposed to fail. A library that always fails is indistinguishable from a
correct one when every test you've written expects failure. Only the happy
path could expose it — it reported `Speratus: I aut plura elementa /
Receptus: 1`, i.e. it saw exactly the one element it wanted and still
called it broken.

The primitive that takes an explicit verdict is `_credo_notare`, whose
last parameter is `b32 praeteritus`.

**Lesson worth more than the fix:** the planted-fault discipline says
prove the gate goes red. That is necessary and not sufficient. A gate
stuck red is as broken as a gate stuck green, and only a case that
*should* pass can tell them apart. Test both poles or you've tested one
bit of information.

### Expected and actual are inverted from the obvious reading

credo prints `valor_primus` as **"Receptus"** and `valor_secundus` as
**"Speratus"** — actual *before* expected. I had them the other way, so
every failure message would have read backwards. Check `lib/credo.c`'s
printer before wiring a new assertion family; the parameter names don't
tell you.

### `credo_invenire_fractos` was declared and never defined

Sat in `credo.h` since the beginning with no definition and no caller in
the tree. `probatio_manus` is the first thing to want it, because
inspecting the failure *message* is the entire point of a planted fault.

Implemented to return **copies** in a fresh chain. Filtering the original
list in place would have to rewrite `sequens`, corrupting the chain that
`credo_numerus_praeteriti` and `credo_imprimere_compendium` both walk.

### One arena cannot be both scratch and durable

The ticket-poll loop takes a `piscina_notare` per iteration and rewinds
after each HTTP request — without that, a two-second wait piles up dozens
of response buffers. But the *result* has to outlive the rewind, and I
allocated it from the same arena after the mark. `piscina_reficere` then
rewound over the copy and the next request overwrote it.

manus now owns a `scriptorium` (its own arena, reset per request) with
`piscina` kept durable. `_servare` is the explicit crossing between them.
**manus owns the scriptorium, so `manus_claudere` must be called** — it's
the only thing in the library with an ownership obligation.

Note the symptom was partial: `visum` survived (it had been copied into a
fixed `character[]` early), `ok` did not. A half-corrupted read is more
confusing than a crash — it looks like a logic bug, and I diagnosed it as
one before finding the credo primitive was the real cause.

### Two deadlines, deliberately

The wait happens **in the page** (a `setTimeout` loop returning a Promise;
imperium already awaits promises). The C side polls the ticket with its
own, slightly longer deadline. Both are needed:

| deadline | means | your bug is |
|---|---|---|
| in-page (`ok:false`) | app answered; element never appeared | in the feature |
| C-side (still `pendens`) | app stopped answering | app wedged or dead |

If only the page held a deadline, a wedged webview would pend forever —
the in-page timer is exactly the thing not running. **The page cannot
police itself.** Keeping the two messages distinct is most of the value:
a UI failure that can't say whether your app is broken or *gone* teaches
people to stop reading UI failures.

### The sampling argument (the real reason for in-page waiting)

Polling from outside, each POST evaluates the condition **once** — so
sampling rate is welded to network rate, ~40 samples in 2s, each costing
a round trip. In-page, the condition is evaluated ~125 times in the same
2s at **zero** round trips; the GETs only ask whether an answer exists.

It does *not* save connections (the ticket still gets polled). It
decouples sampling from the network. That's what catches an 80ms spinner,
and a `MutationObserver` — the tool that would catch it reliably — can
only run inside the page.

### What the test proves, and what it does not

`probatio_manus` drives a **hand-rolled HTTP server in a forked child**,
deliberately *not* hospitium+imperium, so a bug there can't mask a bug
here. It proves manus's half of the protocol: ticket POST, poll loop
(the simulator returns `pendens` on the first poll, so a loop that
doesn't loop fails), all three response shapes, both deadline paths
producing *different* messages, session poisoning (two calls, one
recorded assertion), and dead-app → `NIHIL` rather than a mute manus.

It proves **nothing** about the generated JS in a real WKWebView. A
simulator that faked a page would be "my JS against my DOM" — the defect
that took `probatio_fori.js` green and lying four times. That confidence
has to come from a smoke test against a live app.

Failing assertions are captured in a **separate credo session**
(`_capere`), so a test whose subject is failure messages doesn't turn the
suite red. The main session then asserts *about* the capture — the
planted fault becomes a positive assertion.

## 2026-08-13 — the smoke test, and two bugs no unit test could reach

`../silicetum/laboratorium/fumus.sh` runs manus against the real app:
real window, real WebKit, real DOM, real bridge. It found two defects
within minutes of first running, and **neither was reachable from
`probatio_manus`** — both times because my simulator encoded my
assumptions rather than the server's behaviour.

### 1. imperium answers the ticket POST with `202`, not `200`

`_tesseram_petere` gated on `status == CC`. imperium replies **202
Accepted** — correct, since a ticket is an acceptance, not a result.

The unit test could not catch this: I wrote the simulator to return 200
*because that's what I assumed*. Twenty-four green assertions agreed
with me and disagreed with the server. Now the library accepts any 2xx,
and the simulator returns 202 like the real thing.

**A fixture written by the same hand that wrote the code tests
self-consistency, not correctness.** Same family as the grep that
confirmed its own spelling error (natura.worklog.md).

### 2. A bound port is not a ready app — and the failure is unfalsifiable

`manus_incipere` returned as soon as the port answered. But the app binds
hospitium *before* vitrea loads its page, and in `-vivum` the page is
served **by that same hospitium**. Between those two moments the port
answers and the page does not exist.

A command sent into that window is lost in a way that cannot report
itself: in an unloaded document `internuntius` is undefined, so
imperium's wrapper throws — and its *error* path calls `internuntius`
too. Neither a result nor a fault can ever come back. **The ticket pends
forever and nothing is broken.** Measured: manus reported "applicatio non
respondit" about a perfectly healthy app, while curl drove that same app
successfully in the same second.

Fix: `_paratus` proves the **full round trip** (send `1`, require an
answer), not the socket. Both `manus_aperire` and `manus_incipere` now
demand it. A `tacens` flag suppresses `_frangere` during the probe, where
failure is expected rather than fatal.

Corollary for the simulator: the readiness probe must succeed in *every*
scenario, so "wedged" and "throwing" now mean an app that was healthy and
*then* failed — which is what those words mean in the real world anyway.

### Diagnosis notes (two wrong turns, worth recording)

- I blamed **keep-alive** first: http.c's read loop is `} dum (n > 0);`
  — it reads until the socket closes and ignores `Content-Length`
  entirely. That's true, and a real limitation (the client cannot use a
  persistent connection). It was **not** the cause: http.c also sends
  `Connection: close` on every request unconditionally, so its own
  requests always terminate. I added a redundant header before checking
  and had to take it back.
- I blamed **arena corruption** for the always-false verdict before
  finding the credo primitive. The arena bug was real and worth fixing,
  but it was not that symptom's cause. Two true findings that explain
  nothing are still not the explanation.

The differential test settled it in one run: `fumus.sh -adhaerere`
(attach) passed 10/10 while spawn failed. Attach vs spawn differ in
exactly one thing, which pointed straight at startup timing.

### Both gates proven red

- library: forcing `v.ok = VERUM` → 6 red in `probatio_manus`.
- app: emptying `laboratorium.js` so the bridge never fires → the
  `#salutatio` assertions go red while the static `h1` assertions stay
  green. The async assertion fails *specifically*, so it isn't vacuous.

## 2026-08-13 — the interaction half, and 'visible' vs 'present'

Until today `manus_premere` and `manus_scribere` were called by
**nothing** — not the unit test, not the smoke test. Their generated JS
(`e.click()`, `e.value=`, `dispatchEvent(new Event('input'))`) had never
been evaluated by a browser. The lab app had no button and no input, so
it *couldn't* be tested.

Experiment 0001 (three inputs, a button, a two-scene swap driven through
the bridge) closed that. The smoke test went 10 → **39 assertions**, and
every previously-dead surface is now exercised: scribere, premere, the
silent probes (`existit`/`numerus`/`textus`), `manus_aestimare`, bare
`CREDO_MANUS_ABEST`, and the bridge's *error* path.

Good news first: **scribere and premere worked on the first real run.**
The click drove the bridge, the summary came back, the read-backs
matched.

### The finding: `hidden` is not `absent`

`CREDO_MANUS_ABEST(m, "#forma fieldset")` failed — expected 0, saw 1.
The section had `hidden = true`, but the fieldset inside it is still in
the DOM. `querySelectorAll` counts it.

This is the lying-green class again, and it would have been permanent: a
suite asserting "the old screen went away" that passes when the screen is
merely invisible tests nothing about the transition — which is the one
thing a UI test is for.

**Every selector in manus now means VISIBLE**, via `MANUS_JS_VISUS`
(`v()` / `q()` / `qn()` injected into every generated snippet): not
`display:none`, not `visibility:hidden`, non-empty box. Actions too —
pressing something the user cannot see is never intended. Raw-DOM
questions go through `manus_aestimare`.

Deliberately paired assertions in the test: **both** `ABEST` on the old
scene and `EXISTIT` on the new one. Absence alone can't tell a completed
transition from a broken page.

### A textual replace that changed a return type

Converting the call sites, I ran
`s/var n=document.querySelectorAll(/var n=qn(/` — which left the trailing
`.length` in the *following* literal, producing `qn(".tessera").length`.
`qn` returns a number, so `.length` was `undefined`, and every assertion
reported `visum: "undefined"`.

The rename was correct; the *shape* of the call changed with it and the
regex couldn't see that. **A mechanical rename is only safe when the new
callee has the old one's return type** — here `NodeList` became `number`.
Dumping the actual generated JS (`build/manus_ultimum.js`, written by the
unit-test simulator) found it in one look, after guessing had failed
twice.

That dump file is worth remembering: it is the only place the real
generated JS is visible without a browser.

## 2026-08-13 — actionability, borrowed from Cypress and measured first

Cypress refuses to act on an element unless it is visible, **enabled**,
**not covered**, and scrolled into view. Before adopting that, I measured
whether our gaps were real, in the actual webview:

```
disabled click   -> ictus=0     click did nothing, silently
enabled  click   -> ictus=1
obtectum click   -> ictus=2     fired THROUGH a full-screen overlay
elementFromPoint -> DIV#        hit-test works
```

Both were **lying greens in shipped code**: `manus_premere` returned
`{ok:true,visum:"pressum"}` for a disabled button that swallowed the
event, and for a button under an overlay a user could never reach.
`.click()` does no hit-testing, so JS can click what a person cannot.

`MANUS_JS_AGIBILE` (`act()`) now gates every action and returns a
**named** reason — "impeditum (disabled)" / "obtectum a &lt;div#velum&gt;" /
"nullum elementum visibile" — three very different bugs that all used to
report "pressum". Because actions run through the wait loop, the gate
**retries**: a button enabled after a bridge round trip is waited for,
not failed on.

### Failure screenshots (opt-in, not default)

`manus_imaginem_culpae_ponere(m, via)` captures the screen at the moment
`_frangere` first fires. A failure message says what was expected and
what was seen; it cannot say what the *screen* looked like, and an
unattended run loses that forever.

Deliberately opt-in — a library that writes files unasked is rude. Three
implementation notes:

- `manus_imaginem` refuses when the manus is fracta, so `_frangere`
  can't call it. Factored a bare `_imaginem` that neither checks state
  nor breaks; the public wrapper keeps both behaviours.
- Short deadline (`MORA_BREVIS`). A dead app is the *common* failure
  cause, and `MORA_LONGA` would add ten seconds to every one.
- `in_imagine` guards re-entry: a failing capture would otherwise
  re-enter `_frangere`.

Verified by looking at the PNG: it shows the form mid-failure *and* the
page auto-scrolled, which is `scrollIntoView` from the new gate — the
gate visible in its own evidence.

### Test-structure trap: closing a spawned manus kills the app

The deliberate-failure cases need a *fresh* manus (fracta tacet). My
first attempt did `manus_claudere(m)` then re-opened — but `m` came from
`manus_incipere`, and claudere kills what it spawned. The re-open then
found nothing.

Right shape: keep the spawned manus alive as the scene-manager, and open
a **separate attached** manus per deliberate failure. Closing an attached
manus is harmless because its `processus` is NIHIL. Ownership is the
whole distinction: `claudere` kills only what it started.

Smoke test 39 → **52 assertions**.

### Ideas from Cypress deliberately NOT taken

- **Clock control** (`cy.clock`/`cy.tick`) — collides head-on with the
  design: our in-page wait loop *is* `Date.now()` + `setTimeout`.
  Freezing the clock freezes our own polling. Possible only if the loop
  captures the originals before stubbing. Real trap, not a quick win.
- **Network stubbing** (`cy.intercept`) — here the "network" is the
  internuntius bridge, so stubbing it means asserting against a
  simulacrum. That is precisely what produced the 202-vs-200 bug and
  `probatio_fori.js`'s four green lies. Refused on principle.
- **Chained implicit subject** (`cy.get().click()`) — C89 has no fluent
  chains, and a "current subject" on the handle would add hidden state.

Still open from that review: `contains`-style selection by visible text,
a command log in failure messages, and splitting `ABEST` (invisible) from
`ABEST_OMNINO` (not in DOM) the way Cypress separates `not.be.visible`
from `not.exist`.

## 2026-08-13 — contains, ABEST_OMNINO, and a page-error collector

Smoke test 52 → **68 assertions**.

### `contains` — press what the text says

`manus_premere_textum(m, "condere")` and `CREDO_MANUS_TEXTUM(m, ...)`.
"Press the button that says CONDERE" is the intent; `#mittere` is merely
how it happens to be written today. Text-based tests survive markup
churn and read as intent.

The load-bearing detail is **deepest match**. `<body>` contains every
string on the page, `<main>` nearly every one — so a naive "first
element containing t" presses `<body>` forever, green and useless. `qt()`
collects visible matches and returns the one with no other match inside
it.

### ABEST vs ABEST_OMNINO

Cypress separates `not.be.visible` from `not.exist` and it was right to.
Today's visibility change made `ABEST` mean invisible; that can't catch
an app that leaks hidden nodes forever, because hidden is exactly what it
now tolerates. `CREDO_MANUS_ABEST_OMNINO` uses raw `querySelectorAll`.

Demonstrating the distinction needed care: asserting the *failing* side
would have reddened the suite. Used the **silent probes** instead — the
same element measured both ways in one breath:

```
manus_existit("#fructus fieldset")            -> FALSUM   (invisible)
aestimare("...querySelectorAll(...).length")  -> "1"      (in DOM)
```

Two answers, one element. If they agreed, the distinction would be
decorative.

### Page-error collector

Injected at `aperire`/`incipere`, no app cooperation needed — manus can
already eval arbitrary JS through imperium, so the collector rides in on
the channel that exists.

Three sources, because **none of them catches the others**:

| source | catches |
|---|---|
| `addEventListener('error')` | uncaught throws, resource load failures |
| `'unhandledrejection'` | rejected promises with no `.catch` |
| wrapped `console.error` | what the code deliberately reports |

The middle one matters most here: the entire bridge is promises, and a
rejected promise with no handler writes **nothing** anywhere by default.

Why this is the last lying-green preventer: **a page can throw
continuously while every assertion passes.** A view that failed halfway
often shows the same text as one that succeeded. Verified with a planted
fault that changes no DOM at all — `setTimeout(function(){
nulla_functio_omnino(); })` — so every ordinary assertion still passed
while the collector caught
`ReferenceError: Can't find variable: nulla_functio_omnino`.

`manus_errores()` is a **query** and does not break the manus (proven in
the test: `manus_fracta` still FALSUM after two planted errors);
`CREDO_MANUS_SINE_ERRORIBUS` is the assertion that does.

**What it cannot catch: anything before `manus_aperire`** — page load
itself, most importantly. The collector isn't installed yet. The fix
belongs one layer down: vitrea already injects an internuntius JS shim,
and installing the handlers there would give *every* app error-forwarding
to C, not just apps under test. Not built; the right home for it is
`lib/vitrea_macos.m`, not manus.

Cosmetic gap: eval'd code reports `@ undefined:1` for filename, since it
has no source URL.

### Tooling notes

- The post-edit hook caught `Momentum * Momentum` and `Mora * Mora`
  (point × point and duration × duration are meaningless under fasti's
  contract — do the arithmetic in bare `s64` and bless the result once),
  plus `usleep` being obsolete under XPG7. That last one would have
  survived here and broken under `porta_linux.sh`.
- silva named both stale call sites the instant `_tesseram_petere` lost a
  parameter. Faster and surer than re-reading the file.
- **Two silva false positives**: `nanosleep` and `INADDR_LOOPBACK` both
  reported as unknown identifiers; both compile clean under the full flag
  set with `-Wall -Werror`. Darwin header knowledge gap, not a defect in
  the code.
- `probatio_planta_lectio` fails on `main` independently of this work —
  verified by running it against `git show HEAD:lib/credo.c`. Pre-existing.

## 2026-08-13 — my own timeout budget was the flaky thing

The smoke test went 68/68, then 67/68, then 68/68. Flaky — in the
library whose entire purpose is removing flakiness. Worth recording in
full because the diagnosis was not where I first looked.

Printing the actual cause instead of guessing:

```
causa: 'manus_premere fefellit: #impedita - elementum impeditum (disabled)'   ok
causa: 'applicatio non respondit intra terminum ...'                          FLAKY
```

Not a wrong actionability verdict — a **timeout**.

Why that assertion specifically: pressing a disabled button is the only
case that runs the page's timeout to the *full* 2s, because the
condition can never become true. Everything else resolves in
milliseconds. So it was the only assertion sitting near the deadline at
all.

And the deadline was mine. `MANUS_GRATIA` was 500ms, but the grace does
not merely cover the promise settling — it covers the **return trip**:
the reply crosses internuntius into the host app's event loop, and that
loop pulses whenever it likes. Laboratorium ticks at 200ms, so 500ms of
grace was two and a half ticks of headroom. Under load it lost roughly
one run in three.

Raised to 1500ms. Manus cannot know the host's tick period, so the grace
must be generous rather than tuned. It costs nothing in the happy case —
grace only applies once the page has already hit its own deadline — and
a genuinely dead app is simply reported a second later.

Four consecutive clean runs after the change, all with the correct cause.

**The general lesson**: a two-clock design needs the outer clock sized
against the *slowest thing between the clocks*, not against the inner
clock. I sized it against the inner one and got a test that blamed a
healthy app one time in three — which is exactly the failure mode this
library exists to abolish.

## 2026-08-13 — coverage: 5 → 28 of 29 symbols, automatically

Before this, `probatio_manus` exercised **five** of manus's 29 callable
symbols. Everything else — press, type, contains, the error collector,
visibility, the actionability gate — lived only in `fumus.sh`, which
sits in a sibling repo, opens a window, and **nothing runs
automatically**. `./compile_tests.sh` would have stayed green through a
regression in any of it.

That is the same disease as the rest of this file: something that looks
like coverage and isn't.

### The division of labour, stated so it isn't forgotten

- **smoke test owns BEHAVIOUR** — does the JS do the right thing in a
  real WKWebView. Only a browser can answer that.
- **unit test owns STRUCTURE AND PLUMBING** — what the C returns, whether
  it poisons, what the cause says, and what JS it *generated*.

A unit test that faked browser behaviour would be my-JS-against-my-DOM
again. So the simulator returns canned verdicts and never pretends to be
a page.

### What made it possible

Two scenario kinds with **controllable verdicts** (`SCEN_OK`,
`SCEN_RECUSANS`, plus `SCEN_ERRORES` / `SCEN_SINE_COLL` for the error
parse), and `_agere_capere(scenario, operatio)` which runs one operation
and captures four things at once: return value, fracta, cause text, and
**the JS actually POSTed**.

The JS capture is the interesting half. It turns "someone deleted the
guard" — previously invisible — into a red test.

### Two traps hit while writing it

**1. `credo_claudere()` is global.** `_agere_capere` closes the session
it opened, so calling it from *inside* the main session killed that
session: `FATALE: credo nunquam aperitum`. All captures must run in
phase I, before the main session opens, with phase II asserting about
them. Same rule the older `_capere` already followed.

**2. A regression guard that could not fail.** I first asserted
`_continet(js, "act(")` — but `MANUS_JS_AGIBILE` injects
`function act(e){` into *every* snippet, so the string is present even
when the call site is deleted. The guard would have passed forever.

Found it by planting the fault and watching the test stay green, which
is exactly why the planted fault is not optional. Guards now assert the
**call site** (`var c=act(e)`, `var e=qt(`, `var n=qn(`) — text that
appears only where the thing is actually used, not where it is defined.

**Proven red**: replacing `var c=act(e)` with `var c=null` (removing the
actionability gate) now fails 3 unit assertions. This morning that change
was invisible to the automated suite.

### What is still smoke-only, honestly

`manus_incipere` — it spawns a real binary, and the simulator is a forked
child rather than an executable. Faking it would test the fake. 28 of 29
is where this stops.

## 2026-08-13 — actiones per unam semitam (hygiene)

`manus_premere_textum` describebat `_agere` totam - portam
agibilitatis inclusam - quia solo RESOLUTORE differt: `q(` selectorem
sumit, `qt(` textum. Cetera (porta, gyrus, nuntius) eadem erant.

Resolutor nunc parameter est; tres actiones lineam unam quaeque
sunt, et porta UNO loco emittitur (prius duobus).

**Quid lucramur, et quid non.** Lineae ~35 ablatae sunt - id
incidens est. Pretium verum: actio QUARTA (eligere, purgare,
submittere?) portam OMITTERE non potest. Prius quisque additor
occasionem tertiam habebat eam obliviscendi, ET TACITE: actio sine
porta perfecte agit donec elementum impeditum aut obtectum occurrat,
et tunc VIRIDE mentitur. Nunc quicumque `opus` tradit portam iam
supra se habet.

Nuntii fracturae quoque unius formae facti sunt (premere_textum
olim textum virgulis cingebat, premere selectorem nudum ferebat -
discrepantia quam ipse induxeram scribendo alteram seorsum).

**Regula generalis**: custos qui invariantem PROBAT subsidium est;
structura quae violationem EXPRIMERE non potest remedium. Custodem
primum sumpsi quia vilis erat et duplicatio iam aderat - causa
honesta, non principalis.

Portam ablatam probationes adhuc III assertis capiunt (una sedes,
tres exitus publici). Fumus 68/68 et unitas 83/83 per mutationem
intacta - quod erat rete quod refactionem tutam fecit.

## 2026-08-13 — purgatio errorum + gemelli _MORA desiderati

**Acervus errorum solum CRESCEBAT.** Ergo primus error
`CREDO_MANUS_SINE_ERRORIBUS` in reliquum cursum INUTILEM faciebat:
omne vocamen sequens eundem errorem VETEREM referebat, et error NOVUS
ad assertum LX a vetere ad assertum V discerni non poterat (functio
'primum' reddit, non 'ultimum').

Me ipsum momordit: sectio errorum probationis fumi ULTIMA poni
debebat, et nihil in superficie eam constrictionem nuntiabat.

Peius in usu vero: probatio quae semitam culpae CONSULTO exercet -
validatio fallens, iteratio, CDIV exspectatum - omnes custodes post
se necaret. Scriptor tunc aut assertum delet aut discit id
neglegere; utrumque custodem occidit.

`manus_errores_purgare` acervum in loco mutat (`a.length=0`), non
reponit: custodes ad ILLUM acervum ligati sunt, et eum reponere illos
ad relictum scribere sineret - errores postea deleti viderentur.

**Probatio quae hoc probare potuit erat FUMUS, non unitas.**
Simulacrum responsa scripta reddit, ergo 'numerus post purgationem
nullus est' ei sensu caret; unitas solum vocamen MISSUM esse et JS
rectum (`a.length=0`) probat. Acervus VERUS in pagina VERA vacuari
debuit: fumus nunc duos errores plantat, purgat, nullum invenit, et
CREDO_MANUS_SINE_ERRORIBUS iterum TRANSIT.

Additi quoque tres gemelli `_MORA` qui desiderabantur -
TEXTUS_CONTINET, TEXTUM_ABEST, ABEST_OMNINO. Ceteri quinque eos
habebant; asymmetria illa nusquam intenta erat, et primus qui moram
longiorem in 'continet' poposcisset eam invenisset abesse sine causa.

Unitas LXXXIII -> XC; fumus LXVIII -> LXX.

## 2026-08-13 — manus_reficere: terminus sectionis

'Manus fracta tacet' intra CATENAM causalem recta est - si `premere`
fefellit, asserta de scaena inde secutura sensu carent. Inter
sectiones INDEPENDENTES pura iactura: sectio III cadit, sectiones
XII-XX numquam currunt, et unum vitium per cursum discis.

`manus_reficere` terminum praebet (quod Cypress per `it()` facit).

**ITER PLENUM PROBAT ANTEQUAM REFICIAT** - et hoc totum consilium
est. Duo genera defectus, quae haec bibliotheca ubique distinguit,
hic quoque distinguenda sunt:

| casus | applicatio | actio |
|---|---|---|
| assertum fefellit | VIVIT | reficitur, VERUM |
| applicatio abiit | mortua | FRACTA MANET, FALSUM |

Sine probatione manus contra cadaver reficeretur, et quaeque sectio
sequens terminum plenum ureret: viginti sectiones = LXX secunda nihil
agendi, finita viginti nuntiis identicis 'non respondit'. Cum ea,
applicatio mortua semel cadit et silentium post eam RECTUM est.

Errores quoque purgantur: terminus sectionis utrumque vult, et qui
reficere meminit purgare oblivisceretur.

Probatum utroque latere: SCEN_RECUSANS (actio fallit, app viva) ->
reficere VERUM et manus integra; SCEN_PENDENS (app muta) -> reficere
FALSUM et manus fracta manet.

### Quod reficere NON facit, et cur

Statum applicationis non tangit. Tres gradus refectionis sunt:

| gradus | reficit | NON reficit |
|---|---|---|
| manus_reficere | vexillum fracturae | DOM, promptuarium |
| pagina recargata | + DOM, scaenam, campos | promptuarium |
| processus renovatus | + omnia in memoria | **promptuarium - plagula est** |

Ergo ne processus quidem renovatus independentiam dat cum
applicatio in plagulam scribit. **Isolatio officium APPLICATIONIS
est, non agitatoris**: agitator recargationem praebere potest,
statum-nullum non potest. Unde `-volumen` in laboratorio et
`volumen_temporarium` in bibliotheca - eadem ratio qua `-portus`:
probatio identitatem applicationis regere debet, et status identitas
est non minus quam portus.

Recargatio (gradus II) NON aedificata - cum promptuarium recens sit,
sectiones fere sine ea independent. Si umquam opus erit, CAVE:
custodes vitreae AtDocumentStart iniecti recargationem SUPERSUNT, sed
collector manus (post aperire iniectus) NON - recargatio eum
deleret, et manus_errores tacite in semitam 'collector abest'
caderet, nullos errores in perpetuum nuntians.

## 2026-08-13 — affordantiae: enumeration, the missing primitive

Fran's framing while we were polishing `-vivum` ergonomics: driving a
vitrea app is *"a weird hybrid of a text browser and a normal browser"*
— a screenshot shows you the buttons but not their selectors.

He was right that manus overlapped with what I'd sketched, and more
right than I realised. manus already had the whole Cypress layer:
launch/attach, click-by-text (innermost match, because `<body>` contains
everything), auto-waiting assertions, visibility-not-presence, page
error collection. My proposed "poll the ticket every 10ms from the
client" was **specifically what this header already rejected** — external
polling misses transient conditions, and 2s of it costs 40 connections
where one suffices.

**The actual gap was enumeration.** Every entry point — `premere`,
`scribere`, `existit`, `numerus`, `textus` — takes a selector you
already know. Nothing answered "what is here?"

### Design points that are load-bearing

**Reuses `v()` rather than redefining it.** If enumeration had its own
visibility rule it would list affordances that `manus_premere` then
refuses, and the two definitions would drift silently. The impediment
strings are the same literals `act()` produces, so the list tells you
in advance exactly what an action would say.

**But it does NOT use `act()`,** because `act()` calls `scrollIntoView`
— it *moves the page*. An enumeration that scrolls changes the very
thing it reports. So: `disabled` is judged identically (it's pure),
occlusion is judged only inside the viewport, and outside it we return
no impediment rather than guessing. Nothing is lost: the action itself
scrolls and re-judges at action time, so no stale verdict propagates.

**Selectors are `#id` when available, else an nth-of-type path.** Paths
are fragile under mutation — but this list is an *instantaneous* view,
never stored, so the fragility has no time in which to hurt.

### Proven twice, deliberately

The header's own law: the mock tests the C protocol, and confidence in
generated JS comes from a live app. Both were done.

*Live* (`mensor_ui -vivum` + an injected fixture with the hard cases —
real WebKit, real layout, only the content synthetic): 7 of 9 elements
enumerated. `display:none` and `type=hidden` absent as required; the
disabled button carried `elementum impeditum (disabled)`; the button
under a full-size overlay carried `elementum obtectum a <div#velum>` —
**real occlusion through real layout**; the `aria-label` link titled
"Nexus Nominatus" rather than its text "clic".

*Headless* (two new mock scenarios): field-by-field parse, plus a
response that is well-formed but **not an array** → manus must BREAK.
An empty list there would say "nothing is here", which is the lie this
house keeps designing against.

The mock's `x: -5` is deliberate. Position fields are `s32` because an
element scrolled above the margin is genuinely negative; in `i32`
(unsigned, house-wide) that becomes enormous. The assertion guards it.

### An assertion of mine that was wrong, and failed correctly

I first asserted `scrollIntoView` was **absent** from the emitted JS.
It failed — because `MANUS_JS_VISUS` chains to `MANUS_JS_AGIBILE`, which
*defines* `act()`, so the text is present as dead code that enumeration
never calls. Presence of a literal is not a calling convention. Replaced
with an assertion on what the enumeration actually invokes
(`impedimentum:_imp(e,rc)`), which is the property I meant.

### Still to come

No CLI. manus remains reachable only from compiled C, which is why the
`-vivum` session that started this conversation was driven with raw curl
and a hand-rolled worse version of a library we already own. That's the
next piece.

## 2026-08-13 — bin/manus: the CLI (tools/manus_instrumentum.c)

The library existed; the *surface* didn't. Everything manus does was
reachable only from compiled C, which is why the `-vivum` session that
started this whole thread was driven with raw curl.

Before / after, same task:

```
# before
./bin/mensor_ui -vivum -portus 8792 > log 2>&1 &
PID=$!; sleep 4
T=$(curl -s -X POST --data "$JS" .../imperium)
N=$(echo "$T" | tr -dc '0-9')
sleep 1
curl -s .../imperium/$N
kill $PID

# after
P=$(bin/manus incipere ./bin/mensor_ui)
bin/manus affordantiae
bin/manus finire
```

**Session = port.** No opaque handle was invented: the port is already
unique, already in every message, and already what you'd grep for. A
handle that *hid* the port would add a lookup that can be wrong.

**Ambiguity refuses.** With two sessions live and no `-s`, it exits 2 and
names both rather than picking. Driving the wrong app silently is the
failure this house keeps designing against. Verified with two live apps.

**Why not `manus_incipere` to launch:** it captures the child's stdout
through pipes, and a pipe with no reader blocks or kills the app once the
CLI exits — but a *session* must outlive the CLI. So the tool forks
itself with `setsid` and redirects to a log file, which also makes
`bin/manus effusio` possible later.

### Three bugs, and the shape they shared

**1. `mkdir` after the fork.** The session dir was created in
`_sessionem_scribere`, which runs *after* launch — so the child's log
`open()` failed, its output went nowhere, and when the app failed the
tool pointed me at a file that did not exist. A failure that deleted its
own diagnosis.

**2. A silent child.** `execv` failure went straight to `_exit(127)` with
nothing written. Fixed by writing `strerror(errno)` to the
already-redirected log — which is correct behavior, not just
instrumentation, and it's what let me confirm `execv` was *succeeding*
for `/bin/echo` while producing no output.

**3. The real one: `manus_incipere(piscina, NIHIL, portus, mora)`.**
I wanted "wait for the port" and manus_incipere waits for a port, so I
passed NIHIL for the argv it requires and wrote a comment asserting it
would then *only* wait. I never verified that. It was killing my forked
child — `/bin/echo` produced a 0-byte log until I removed the call, and
21 bytes immediately after.

The shared shape is worth naming: **all three were me asserting behavior
in a comment instead of checking it.** The comment made the code read as
if it had been reasoned about. Replaced with `_portum_exspectare`, which
polls `manus_aperire` on a bounded deadline.

(That poll uses `nanosleep`, which is *not* a violation of this library's
no-sleep law: that law governs page conditions, where waiting must happen
inside the page or transients are missed. Waiting for a process to exist
is outside the page and cannot be observed from within it —
`manus_incipere` itself does the same.)

### silva shim

`setsid` and `execv` were missing from `silva/fontes/systema_posix.h`, so
examen reported implicit calls. Healed the shim rather than pinning the
diagnostic (house doctrine). Amalgam regenerated — silva/CLAUDE.md warns
the suite does not catch a stale one. silva 38/38, auspex certifies.

## 2026-08-13 — two defects found by a test-user subagent

Fran's experiment: brief a subagent as a developer investigating a slow
test suite, give it `bin/manus` and the app, tell it **not** to read any
source, and collect where it got stuck. It reported friction on both the
app and the tool. I verified its two most serious claims myself before
acting — both real.

### `manus_scribere` returned ok:true while doing nothing

```
bin/manus scribere '#comparanda' '1 · 64d3dbf0'   → exit 0, nothing changed
```

`.value` on a `<select>` only takes if it matches an option's *value*;
any other string is silently discarded. The old code assigned and
returned `{ok:true}` unconditionally.

The uncomfortable part: **I had demonstrated this exact call "working"
earlier the same day** — but I'd pulled the option's `value` out with
`aestimare` first. My verification was privileged: it depended on
knowledge the tool never surfaces. A user who reads the screen sees only
the label.

Fixed with three changes, of which only the first is select-specific:
1. `<select>` matches by option value **or visible text** — the text is
   what the user can actually see.
2. An element with no `.value` is **refused**, not silently written to.
3. **The value is re-read after assignment.** A number input rejecting
   letters, or a `maxlength` truncating, would have lied in exactly the
   same way. This is the general fix; the select was just the instance
   that got caught.

### `manus_textus` returned invisible content

```
bin/manus textus body   → 32,594 bytes, including the page's entire
                          inline <script> with implementation comments
```

`textContent` reads everything, including what no user can see. That
directly contradicts this library's founding law — *"OMNIA 'VISIBILE'
SIGNIFICANT, NON 'PRAESENS'"* — which every selector and assertion
already obeys. `manus_textus` was the one hole.

Now uses `innerText` (which respects visibility and drops script/style),
and for a `<select>` returns the **selected option's text** rather than
its value. The old behavior returned an empty string when the selected
option had an empty value, which read as "the selector matched nothing".

It also breached the experiment's own "don't read source" rule through
an ordinary read command — the agent disclosed that rather than quietly
using what it saw.

### Test coverage

Five new assertions in the JS-form regression block plus two for textus.
These guard the *shape* of the emitted JS, not the behavior — the mock
has no page, so behavior was proven live against the running app, per
this file's standing split. Suite: 112/112.

## 2026-08-13 — scroll, whitespace matching, and a latent fixture bug

Second pass on the test-user report. The first item turned out not to be
what the agent (or I) thought.

### `premere-textum` was a whitespace bug, not a sibling-node bug

The agent reported that `premere-textum 'praevolatus 75.0s'` failed while
that exact text was on screen, and guessed the label was two adjacent
sibling spans. It isn't — it's a single SVG `<text>`. The page writes
`nomen + "  " + valor`, **two spaces**, and the browser collapses them
when painting. The user reads one space; `textContent` holds two.

So the real defect is broader than reported: `_tm` compared raw
`textContent` byte-for-byte against text the user reads *collapsed*. Any
multi-space or indented markup breaks the same way.

Fixed with `_nz` — collapse whitespace runs, trim, applied to **both**
sides. `_tx` now also prefers `innerText` where it exists (SVG has none,
so `textContent` remains the fallback), which brings text matching under
the same visible-not-present law as everything else.

### And the fix exposed a second defect

Once matching worked, the click failed with `e.click is not a function`.
`.click()` is `HTMLElement`-only; the match was an SVG `<text>`. So
chart elements — flame bars, trend points — could never be clicked at
all. Now: use `.click()` when it exists, otherwise dispatch a real
`MouseEvent`. The page can't tell the difference.

Good sequence to notice: fixing the finder revealed the actor was broken.
Neither was visible while the other failed first.

### Absence now has its own message

`act(null)` returned "nullum elementum visibile" for both resolvers, so a
text query that matched nothing said the same thing as a selector that
matched nothing — and the agent read it as "this isn't on the page" when
the real cause was "no text is written that way". `_agere` now emits a
resolver-specific message before the actionability gate.

### `volvere`

`imago` captures the viewport, so anything below the fold was invisible
and the agent had to fall back to `aestimare` + `window.scrollTo`, which
is documented nowhere. Added `manus_volvere_ad(selector)` and
`manus_volvere(±pixels)`, CLI `volvere <selector|±N>` (a CSS identifier
cannot begin with a digit or sign, so the discrimination is a language
rule, not a guess).

Deliberately **not** behind the actionability gate: scrolling to a
disabled or covered element is legitimate — you're looking, not acting.
Asserted in the tests so nobody "fixes" it later.

### The latent bug my change surfaced

Adding ~300 bytes of JS preamble broke five affordantiae assertions —
while the same call worked fine against the live app. Cause: the mock
server read **once** into a 4096-byte buffer. The affordantiae request
had been sitting just under that; the preamble pushed it over, leaving
unread bytes in the receive buffer, and `close()` with unread data sends
RST — which destroyed the response that had already been written. The
client saw "applicatio iussum non accepit".

The fixture had been wrong the whole time. My change only crossed the
threshold. Now it loops until headers are complete and `Content-Length`
is satisfied, into a 16 KB buffer — the same accept/drain/close rule the
http and tcp fixtures learned last year.

Worth keeping: **live worked, mock failed.** That asymmetry pointed
straight at the harness and saved me from hunting a phantom regression
in the library.

## 2026-08-13 — output that says what it is

Last two items from the test-user report. They looked like separate
complaints and turned out to be one defect.

**The reported symptoms.** `affordantiae -machina` emits nine unlabeled
columns; the agent guessed the last four were x/y/w/h. And `manus
sessiones` printed three bare numbers, which it confused with the app's
own "SESSIONES" panel (test-run records).

**The bounding-box guess was right, which is worse than wrong.** `x y
width height` and `x1 y1 x2 y2` are equally standard encodings. A wrong
guess surfaces immediately; a right guess is a coin-flip that happened to
land, and the next reader may flip differently. Empty fields make it
harder still — `valor` and `impedimentum` both blank produce a run of
consecutive tabs, and counting those by eye is unreliable in a way that
silently shifts every later column.

**The naming collision is not a naming error.** *Sessio* correctly names
a driven-app instance and correctly names a test-run record; any app
could collide. Renaming manus's verb would relocate the problem, not
remove it. What was actually missing was a label — three bare numbers
give no clue which domain you are in. `portus / pid / applicatio` answers
it without renaming anything.

**One principle, not two patches:** the human form is self-describing;
`-machina` stays pure and the *usage text* carries the column names.
That keeps `-machina` awk-safe (no header line to skip, per the house
rule) and puts the column list where the agent had already looked and
found nothing.

Also: the empty case now prints its reason to **stderr**, so
`sessiones -machina` produces zero bytes on stdout rather than a
human sentence a parser would choke on.

Verified the actual claim rather than the appearance — piped both
emitters through `awk -F'\t'` with no line skipped, and asserted every
affordance row has exactly 9 fields.

## 2026-08-13 — lectio: the reading half

Fran asked how the interface felt after a day of heavy use. The honest
answer was a diagnosis: **I live in `aestimare`.** Almost every
verification I ran that day was raw JS —
`Array.prototype.map.call(document.querySelectorAll(...))` over and over
— not the typed verbs.

A tool whose escape hatch is the main road is telling you something about
its vocabulary. The specific shape: **acting is well covered, reading is
barely covered.** Four action verbs; one reading verb returning a single
string; one enumerator that only sees *interactive* things. Nothing
answered "what does it say?"

`affordantiae` proved enumeration was the missing primitive for
interaction. The same hole existed on the content side and I hadn't
noticed because I'd been papering over it with JS.

**One rule, no per-tag magic.** For each visible element matching the
selector, the cells are the visible *element children's* text; with no
element children, its own text is the single cell. That covers every case
I'd hand-written:

    lege "tr"       → cells of a row
    lege ".tabula h2" → one heading per line
    lege ".sessio"  → the button's spans as columns

Row lengths therefore **vary**, which the mock scenario exercises
deliberately — a parser assuming fixed width would break on real content.

Reuses `v()` and `_nz`/`_tx` from the preamble, so "what the user reads"
has one definition shared with text matching, not two.

### Empty text queries now refuse

`premere-textum ''` matched everything, `qt` picked the innermost, and it
clicked `<body>` with exit 0. **That is how I produced two vacuous passes
in one session** — an `aestimare` returned `null`, I piped it into
`premere-textum`, and it "worked". I put that down to my own sloppiness
at the time. Half of it was the tool accepting a query that cannot mean
anything. Guarded in C (with a named cause) and in `_tm` (so page-text
assertions don't match everything either).

### Noted, not built: `exspectare`

Asymmetry worth recording: **CLI actions wait, CLI queries don't.**
`premere` goes through `_agere`, which retries in the page until the
element appears; `existit`/`numerus`/`textus`/`lege` go through
`_interrogare`, which is documented as answering immediately.

So a shell script driving an async UI has no way to wait for a
*condition* and gets pushed back toward `sleep` — the one thing this
library refuses to offer. That is exactly the gap `CREDO_MANUS_*` fills
in C.

The right port is **not** the assertions as assertions: an assertion is
*wait + record*, and only the wait means anything outside a credo
session. Ported as waiting predicates (exit 0 met / 1 timed out, carrying
the existing "expected X, saw Y" text), they'd be what makes shell
driving non-flaky. Open design question is one generic `exspectare` with
a condition enum versus typed siblings mirroring the macros — a header
decision, so it waits for Fran.

## 2026-08-13 — exspectatio: the reads learn to wait

Fran's question closed this properly: *"can the script present a
synchronous way to interact with something async — you ask for an action
and it doesn't return until the thing is visible and clicked, within a
timeout?"*

**That is already true of actions**, and I hadn't said so plainly.
Measured:

| element appears at | `bin/manus premere` |
|---|---|
| 800 ms | exit 0 after **934 ms** — it waited |
| 5000 ms | exit 1 after 2777 ms, cause named |

`premere`/`scribere`/`premere-textum` go through `_agere`, whose promise
re-tests in the page until the element appears or the deadline passes.
So his mental model was right; the docs just never said it out loud.

**The gap was only the reads.** `existit`/`numerus`/`textus`/`lege` go
through `_interrogare` — "statim respondet". So a shell script could act
synchronously but not *read* synchronously, which is what pushed it back
toward `sleep`.

That reframing shrank the job. I had been about to propose a six-verb
`exspectare` family mirroring the assertion macros. What was actually
needed:

- **`-exspecta` on the reads** — same discipline the actions already
  have, no new vocabulary
- **`abest <selector>`** — because "retry until non-empty" *structurally
  cannot* express disappearance: a spinner going away, a dialog closing,
  a row being deleted

One library function covers both: `manus_exspectare(sel, adesse, mora)`.
It waits on **visible presence or absence** — not text, not counts. Those
are assertion-shaped, and at a CLI you'd want to compare the value in the
shell anyway, where `lege` now hands it to you.

The wait stays **in the page** (`_js_exspectare`, ~62 re-tests/sec, one
network round trip). An external poll would have been the exact thing
this library's header forbids, and I nearly wrote it.

Failure names the count, not just the fact: `(visa: 0)` vs `(visa: 1)` —
"nothing appeared" and "one is still there" are different problems.

Verified live at both edges: `.tarda` appearing at 700 ms read empty
without the flag and correctly with it; `#rota` removed at 700 ms
satisfied `abest`; an element that stays timed out with *adhuc adest
#tabula-sessiones (visa: 1)*.

### examen gap that bit

`i32 k;` after a statement — C89 forbids it, clang caught it, and
**examen said ACCIPE**. That is ledger `01KZBYEHJP` ("declaratio post
sententiam (C99) non deprehenditur") firing in practice rather than in
theory. Worth knowing: on declaration order, the compiler is the
authority, not our judge.

## 2026-08-17 — the THIRD copy of the text judgment, found by `<li>`

Laboratorium experiment 0004 (command palette) tripped a failure that
looked impossible: `CREDO_MANUS_TEXTUS_CONTINET(m, ".tm-electus",
"Omnia servare")` reported `Receptus: 0` while `manus textus
'.tm-electus'` on the same live page returned the text correctly. The
assertion and the instrument disagreed about the same element.

Cause: `_manus_credo_textum` carried its own inline copy of the
element-text judgment — the ORIGINAL buggy form, `e.value!==undefined ?
e.value : e.textContent`. `HTMLLIElement.value` is a real DOM property
(the ordinal for `<ol>` items), a NUMBER, default 0 — so the assertion
read `"0"` for every list item. This is the same defect class as the
2026-08-14 `<button>.value` finding, except worse: button gave `""`
(suspicious), li gives `"0"` (looks like data).

The `manus_textus` comment had already prophesied this: "DUAE FORMAE
EIUSDEM IUDICII hic vivunt... quod ipsum est cur duplicatio nocet." It
was three forms, not two, and the third had the oldest bug.

Fix: `_tx` (in the preamble every script already receives) is now the
ONE form, upgraded with `manus_textus`'s SELECT semantics (selected
option's TEXT, not `.value` — the user reads words). `manus_textus` and
`_manus_credo_textum` both collapsed to `_tx(e)` calls. That also
retired the known `_tx`-vs-`manus_textus` SELECT divergence in the
direction the comment already argued for. Text search (`premere-textum`)
now matches selects by their visible option text too — behavior change,
correct direction.

Regression pins in probatio_manus updated to pin the NEW form,
including `visum:_tx(e)` in the textus body and the negative on the old
branch. The lesson worth keeping: **when an assertion and its query
tool can disagree about the same element, every green assertion is
suspect** — unify the judgment, don't patch the copy that got caught.

## 2026-08-17 — consolidation pass: the write side, the affordance dump, the CLI port

Follow-through on the `_tx` unification, same law ("forma una ex fonte
uno venit"), three more sites measured and fixed:

**`scribere` accepted anything with `.value`** — which includes button
(`""`), checkbox/radio (`"on"`), even `<li>` (ordinal). Measured live:
`scribere '#tm-aperire' text` on a button exited 0 while the button
visibly changed nothing — the write went into a property no user sees.
The acceptance test for "where can a user type" already existed as
`_fscr` (the measured focus-on-click table premere uses). Extracted it
to `MANUS_JS_FSCR`; `MANUS_JS_FOCARE` = FSCR + the focus action;
scribere now refuses non-writing surfaces by name ("superficies
scribendi non est"). SELECT keeps its own option-matching branch.
Note the negative pin still holds: `focus_ponere` deliberately does
NOT carry `_fscr` — asking is not simulating.

**affordantiae `valor` read raw `.value`** — same unguarded read.
Now `(g===2||g===3)?_tx(e):''` — the affordance dump already
classifies elements (`_gen`), so the guard reuses ITS judgment plus
`_tx`: text fields report their value, selects their option TEXT,
buttons/checkboxes report nothing instead of noise.

**`manus incipere` + caller `-portus` hung confusingly** (bit me this
session): incipere picks its own port and waits on it; a caller-
supplied `-portus` made the app open a different one, and the failure
message pointed at `-vivum`. Now refused up front with the mechanism
named: portus = sessio, so a caller port is a contradiction, not a
configuration.

**fumus section boundaries**: `manus_reficere` before 0002/0003/0004/
Imago. PLANTED FAULT proved it: a broken assertion in 0002 produced 10
failures all confined to 0002 — 0003, 0004 and the screenshot ran
clean with their own verdicts. Before the boundaries the same plant
cascaded into every later section (14 from 1). Within-section poison
stays, deliberately: the boundary is the section.

probatio_manus 195/195 (two new pins); laboratorium fumus 173/173.

## 2026-08-18 — scribere does NOT leave focus in the field (measured)

Composite-experiment finding (laboratorium 0017): after
`manus_scribere`, `document.activeElement` is BODY — the typed
text lands (value + input events fire) but native focus does not
persist in the field. Every earlier green run that sent native
keys after scribere (0014's Enter-commit) passed only because a
prior click handler had explicitly focused the field
(vertere → campus.focus()). The working contract: **scribere,
then manus_focus_ponere, then native keys.** Recorded in the lab
memory + 0017 inventum; desideratum filed for scribere to retain
focus (would match user-typing semantics and remove the
trap-shaped step).

## 2026-09-02 — deadlines per handle; the readiness probe pays grace too

Suite timing: probatio_manus was 12.5 s with 0.03 s of CPU. Every
wait against the hanging fake server cost its deadline PLUS
MANUS_GRATIA (1.5 s): reload-after-death 7.0 s, the pending capture
3.4 s, the size-absent capture 1.7 s. Deadlines moved from constants
into the handle (mora_ordinaria, gratia; manus_terminos_ponere), and
the first attempt still left 6 s — because manus_aperire's own
readiness probe is an _iussum and runs before any setter can apply.
Hence manus_aperire_terminis (deadlines before the probe);
manus_aperire is now the default-deadline wrapper. The test opens
only the hanging scenarios (PENDENS, MAGN_ABSENS) with short
deadlines; responding scenarios keep production values. 12.5 → 2.1 s.
Lesson: a per-handle setting is useless for whatever the constructor
does before returning the handle.
