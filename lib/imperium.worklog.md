# imperium.worklog.md

## 2026-08-12 — birth, and two bugs of the worst kind

`imperium` exists because `vitrea_aestimare` pushes JS into a live
webview and **returns nothing**. An agent could make the app *do*
things but never *ask* it anything — and a test that cannot ask is not
a test.

The whole library is two things that already existed, wired together:
`hospitium` (outside→in) and `internuntius` (inside→out), both already
pumped on the same loop by the app. Nothing new was needed except the
plumbing between them.

### Why tickets instead of one held request

Hospitium is single-threaded and its handlers must return fast. Holding
a request open while waiting for the answer would starve the very loop
that has to deliver it — instant deadlock. So `POST` returns a ticket
and `GET /imperium/<n>` polls. Two round trips costs an agent nothing.

### BUG 1 — the always-null (the dangerous one)

First version wrapped the caller's JS in a function body:

```js
var r = (function(){ 1+1 })();      /* → undefined */
```

A function body's last expression is a *statement*, not a return value.
So **every command reported `{"status":"perfectum","valor":null}`** — a
tool that always succeeds and never tells you anything. It looks green
forever. This is the same family as the four ways `probatio_fori.js`
went green while lying.

Fix: indirect `eval`, which yields the **completion value** — exactly
what a browser console does:

```js
var r = (0,eval)("1+1");          /* → 2                */
var r = (0,eval)("var x=1; x+1"); /* → 2 (completion)   */
```

Cost: the JS must be embedded as an escaped string rather than raw. That
is the trade, and it is worth it. `(0,eval)` (indirect) keeps global
scope.

`probatio_imperium` now asserts the literal string `eval` appears in the
generated wrapper, so reverting to the function-body form fails loudly
instead of going quietly green.

### BUG 2 — unquoted error text

`chorda_aedificator_appendere_evasus_json` escapes the *content* but does
not add the surrounding quotes. So error responses emitted
`{"nuntius":Can't find variable: x}` — invalid JSON. Add your own `"`.

### The seam pattern, and why it matters here

`vitrea` exposes seam-shaped twins so consumers never need the `Vitrea`
type: `vitrea_missor` (internuntius), `vitrea_aestimator` (speculum),
and now `vitrea_imaginator` (imperium). imperium declares its own
identically-shaped typedefs, so the function pointers are compatible
without a cast and **imperium has no vitrea dependency at all**.

That is what lets `probatio_imperium` drive the entire loop with no
window: a fake aestimator captures the JS, and `internuntius_tractare`
feeds a crafted message back as if the page had called. 32 assertions,
fully deterministic.

## 2026-08-12 — snapshots from inside, not through the OS

`screencapture -l<windowid>` fails with *"could not create image from
window"* without Screen Recording permission. Worse, the full-screen
`-R` form returns **exit 0** and writes a file — which on macOS 14+
without that permission is wallpaper only. A success that means nothing.

`WKWebView takeSnapshotWithConfiguration:completionHandler:` is strictly
better:

| | screencapture | in-process |
|---|---|---|
| permission | Screen Recording | **none** |
| conditions | frontmost, unoccluded, on-screen | none |
| scope | **can capture the whole desktop** | own content only |

The last row is the real argument: an in-process snapshot **cannot**
contain anything belonging to the user. The safety is structural, not
behavioural.

**Blocks compile under the house flags.** No `.m` in the tree used one
before, and the snapshot API requires a completion handler. Measured:
`-std=c89 -pedantic -Werror -Wconversion …` accepts `^{ }` in ObjC mode,
because ObjC already departs from C89 (`@interface` is not C89 either).

Caveat: it captures *web content*, not native window chrome (no title
bar). For a UI lab that is what you want. `WKSnapshotConfiguration` also
takes a sub-rect and a target width — the right granularity for the
armarium's future visual goldens.

### Flag gotcha

`lib/vitrea_macos.m` needs **`-Wno-overlength-strings`** (it embeds the
internuntius JS shim, ~1380 chars; C90 guarantees only 509). It also
needs `-Wno-long-long` like everything else, since `latina.h` defines
i64/s64 as `long long`. The flag list in root `CLAUDE.md` omits
`-std=c89`, `-Wno-long-long` **and** `-Wno-overlength-strings` — the
real set is in `compile_tests.sh`. Bit me twice in one session.

## 2026-08-13 — vivarium (baculi inclusi)

Fran's framing was better than my own: the point is not the ~45 lines
saved, it is **discoverability** — a new vitrea app should get `-vivum`
out of the box rather than by copying wiring out of another app.

### Why not in vitrea

`vitrea.h` declares a permanent scope: *"adhaesio texturae + origo
assetorum + transportus chordarum, IN AETERNUM"*. An HTTP server behind
a vitrea flag would break that, and would make every vitrea app link
hospitium whether or not it serves anything.

### Why here

imperium already includes hospitium and internuntius, so vivarium adds
no dependency. And all three seams (`ImperiumAestimator`,
`ImperiumImaginator`, `InternuntiusMissor`) are vitrea-free — so
vivarium, like imperium, never learns what a Vitrea is. That is also
why it can be tested with no window at all.

### The premise this corrects

This header said imperium builds on *"duas res quae IAM ADSUNT"*. True
for forum and villa, which have hospitium and internuntius for real
RPC. **False for a read-only viewer**: mensor_ui had neither, and had
to stand up an HTTP server and an RPC bridge purely to get a JS eval
channel. Vivarium absorbs that difference — bring your own, or get one
made.

### Constraint found by a failing test

My first test handed vivarium the `inx` that already had an imperium
bound to it, and it refused. That was **correct**: `imperium_praebere`
registers the `imperium.responsum` bridge method, and a duplicate
registration fails — so **one internuntius carries exactly one
imperium**. Nobody had written that down. Now it is in the header and
pinned as a test case, so passing a bound bridge is a named refusal
rather than a baffling failure.

### Note on reading diagnostics

examen reported the new struct's seam types as `[infra]` — below
threshold, which I read as noise. They were a real error: I had placed
the struct *before* the typedefs it references. `infra` is not
automatically noise.
