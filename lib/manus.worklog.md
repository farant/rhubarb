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
