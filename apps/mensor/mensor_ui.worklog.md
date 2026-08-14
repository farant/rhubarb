# mensor_ui.worklog.md

## 2026-08-13 — session picker, and the bug it exposed

First feature built under Fran's "drive the app you're building" loop:
mensor_ui gained `-vivum` when it moved onto atrium, `bin/manus` can
now drive it, so I could add a feature and verify it without a round
trip to Fran.

### The feature

`reddere()` hardwired `ultima` (latest session) in four places. Now
there's a `sessioElecta` module variable and a row of real `<button>`
elements, one per session.

Deliberately real buttons, not divs with click handlers: manus sees
buttons as `premenda` affordances and a screen reader announces them.
Each gets a stable `id="sessio-N"` and an `aria-label` (`sessio 3
fcbeae67`), because manus's `_lab()` prefers aria-label — so building
for drivability and building for accessibility are the same work here.

Selection state lives in the module, not the DOM: `reddere()` rebuilds
`#corpus` wholesale, so anything stored only in the DOM would die on
every render. Selection is also validated against present sessions —
the data reloads while the window lives, and a vanished key would
render empty cards with no visible cause.

The trend chart keeps all sessions and marks the selected one with a
dashed vertical line, drawn *before* the series so it doesn't cover them.

### The bug it exposed

With the picker I looked at session 1 for the first time, and the flame
graph showed a single `xml 0.1s` bar stretched across the full width of
a 180-second run.

`flammaPingere` took `summa = max(root.valor)` and then placed **every**
root at `x=0` with the full width. With one root — which is what every
session since `757ff96` has — that is exactly correct. With N roots they
stack on top of each other and you see only the last one drawn, which is
why the label was `probatio.compilatio.probatio_xml`: last in the array.

Latent since the flame graph was written this morning, and invisible
until something let me look at an old session.

Fixed to sum the roots and lay them side by side proportionally.
Identical output for the single-root case, correct for N.

**No real session exercises the fix**, though — every session since the
first has exactly one root, and session 1 now takes the no-tree branch
below. So I verified it against a synthetic two-root tree (30s/10s) in
the live page: `x=0 w=748` and `x=750 w=248`, i.e. 75%/25%. Under the
old code both were `x=0 w=998`.

### Second finding: a correct picture can still lie

Session 1 has 265 measurements and **zero** parent links — it predates
`757ff96`, which is when the suite began emitting the tree. Laying those
265 roots side by side is correctly *drawn* and still false: the totals
double-count, since `totum` contains `cursus` which contains every test.

So a flat session now gets a named explanation instead of a picture:
"Sessio sine arbore: 265 mensurae planae … Suita arborem emittere
coepit a 757ff96." A flame graph represents a tree; when there is no
tree, saying so beats drawing something defensible-but-wrong.

### Build note

`mensor_ui_struere.sh`, never aedilis alone — the front end is embedded
in a capsula, so editing `index.html` and building with aedilis gives a
**successful build running the old page**.

## 2026-08-13 — collatio: the question the library was built for

`mensura` stores losslessly *specifically* so you can ask "which commit
made this slower" — its design note says RRD structurally cannot answer
that. The viewer couldn't ask it either, until now.

**Baseline picker is a `<select>`, not a second button row.** Eleven
sessions would mean twenty-two buttons; a hundred would mean two
hundred. A select grows with the list; buttons don't. It also exercises
a genus of affordance nothing else here had — manus reports it as
`eligenda`, and `manus_scribere` drives it (sets `.value`, fires
`change`).

Sign convention is fixed and stated on screen: **electa − basis, so
positive means slower now.** The other order gives correct numbers with
inverted meaning, which is worse than being wrong.

Rows where a test exists on only one side sort **first**, before any
delta. A test appearing or vanishing is a bigger change than any timing
difference, and sorting by |delta| would bury it — its delta is null.

First real use immediately paid: session 11 vs session 5 (both full
suites) shows **totum +8.51s but cursus −3.24s**. Execution got
*faster*; the regression is not in the tests. That is the analysis this
whole stack exists to make cheap.

Cards now carry stable ids (`#tabula-collatio`, `#collatio-summa`, …).
I had been driving the page with `.tabula h2[2]`-style index selectors
and adding a card shifted them — index selectors are fragile the moment
the page grows.

### The bug that ate the middle of this session

Rebuilding mid-work started killing the app: `-json` exited **137**,
manual launches died silently, `bin/manus` reported a port that never
opened. The launcher log was empty because the app never got far enough
to print.

Crash report gave it up: `CODESIGNING / Taskgated Invalid Signature`,
ten crashes. **macOS caches a binary's code signature per vnode. `cp`
over a binary that recently ran leaves new content validated against
the cached old signature, and the kernel SIGKILLs it.** Intermittent by
nature — it depends on whether the cache is warm, which is exactly why I
had recorded an "unexplained SIGKILL (137)" earlier in the day and moved
on.

Fixed in all three struere scripts: `rm -f` the destination before `cp`,
so the copy lands on a fresh inode with nothing cached to disagree with.
0/10 failures after, previously intermittent.

Worth remembering as a shape: **the empty log was the clue.** A process
that dies before its first `printf` was killed from outside, not broken
inside.
