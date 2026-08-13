# vitrea_macos worklog

## 2026-07-20 — Phase B: first-fix build, first-run round trip

The whole platform file (~650 lines) compiled clean under the full
strict flag set on the FIRST attempt, and the demo's automatic
bridge smoke passed on the FIRST live run:

    [vitrea] salve vocatum
    [vitrea] confirmatum: salve, munde!

The confirmatum line is load-bearing: the page chains a second
vocare off the first reply's .then(), so its appearance proves the
COMPLETE cycle headless — capsula → scheme handler → injected
runtime → postMessage → cauda+wake → blocking-pump drain →
dispatch → reply → escaper → _accipere → promise resolution →
second round trip. Idle CPU measured 0.0% (exit bar met exactly).

Why first-fix worked: every WebKit behavior this file depends on
was pinned by the calibration spike BEFORE the file existed
(NSHTTPURLResponse-200 rule, dummy authority, wake contract), and
every codebase contact was pinned by the explorers (NSWindow from
tractationem_nativam, MRC dialect, capsula copy semantics,
PiscinaNotatio names). The file was transcription, not discovery.
Calibration-first pays off exactly here.

Decisions recorded:
- Piscina reset ON ARRIVAL TO EMPTY CAUDA (not on drain-empty):
  early-exit from the drain loop can never leak growth, and a
  non-empty cauda is never reset under a held chorda.
- Navigation policy DEFERRED to Phase C: the decidePolicy delegate
  method takes a block parameter; skipping it keeps the file
  literally block-free rather than block-literal-free.
- vitrea → internuntius include is for the escaper only; the
  dependency direction is fine (internuntius stays pure).
- Copy-and-reset serving per spec v2 §4: dataWithBytes (WebKit
  owns its copy) then immediate reficere — zero lifetime coupling
  with the async scheme task.

## 2026-07-20 — Phase C: navigation policy

decidePolicyForNavigationAction shipped. The dialect decision that
unblocked it (Fran, decretum 01KXZZDPNK): calling a RECEIVED block
parameter — `decisor(WKNavigationActionPolicyAllow)` — is allowed;
block LITERALS (^{}) stay forbidden. The file is block-literal-free,
no longer block-free.

Policy shape: origin captured ONCE in creare (capsula → fixed
"capsula"/"radix"; URL mode → NSURL-parsed scheme/host/port, and an
unparseable URL now fails creare BEFORE the dynamic piscinae are
allocated, so failure leaks nothing). decidePolicy allows `about:`
(WebKit's internal blank pages) and exact scheme+host+port matches;
everything else → navigationes_recusatae++ + Cancel. Subresource
requests (fetch, scheme-handler assets) never pass through
decidePolicy — navigations only.

Port caveat worth remembering: NSURL gives no port for
`http://127.0.0.1` (→ -1 sentinel), so a dev config URL written
WITHOUT an explicit port only matches portless navigation URLs.
Dev URLs in practice always carry `:PORT`; if one ever doesn't,
same-origin JS navigation still matches (same portless form).

Smoke design — failure is self-announcing: index.html auto-attempts
`location.href = 'https://exemplum.externum.invalidum/'` after the
salve round trip, then reads fructus through the bridge and echoes
`navigationes recusatae = 1` via confirmare. If the policy ever
breaks, the page NAVIGATES AWAY, the JS context dies, and the smoke
line simply never prints. The echo also proves the page survived
the attempt — a successful navigation could not have produced it.

## 2026-08-13 — PNG encoding moved out of AppKit (the seam moved down)

`vitrea_imaginem_petere` no longer encodes PNG. It now extracts pixels
into an `Imago` and hands them to `imago_png_scribere` (portable C89).

**The rule this established: the platform boundary belongs at pixel
acquisition, not at file format.** "Capture this WKWebView's contents"
is genuinely macOS-bound. "Serialize RGBA to PNG" is arithmetic. AppKit
was doing both, which is exactly why the encode chain had been
copy-pasted into `tools/vitrea_spica.m` as well — there was no portable
place to put it.

Two things worth knowing:

**1. The type was dragging the decoder.** Adding `#include "imago_png.h"`
put `lib/imago.c` AND `vendor/stb_image.h` into the closure of every
vitrea app — measured with `bin/aedilis apps/forum/forum.c --partes`.
Cause: `Imago` (a 3-field struct) lived in `imago.h` next to the
stb_image-backed loaders, so wanting the type meant importing the
decoder. Fixed by splitting the struct into `include/imago_typus.h`, a
header with no implementation file, which therefore drags nothing.
`imago_png.h` and `imago_collatio.h` now include only that. Forum's
closure went from `vendores 1` back to `vendores 0`.

This is the same error as the PNG seam itself, one level down: things
that don't belong together sharing a header. Worth watching for — the
include graph is where it shows up, and `--partes` is how you see it.

**2. Row order was the real risk, and no unit test could catch it.**
If `CGBitmapContext` had been bottom-row-first, every screenshot would
be vertically flipped — and encoder and decoder would happily agree on
the flipped buffer, so a round-trip test stays green. Verified instead
with a scratch program (no window needed): build a CGImage whose top
half is red and bottom half blue, run the identical extraction, and
check byte 0. Result: row 0 in memory IS the top row. The emitted PNG
was then viewed directly — red above, blue below.

Alpha is un-premultiplied on the way out (CG premultiplies, PNG does
not), with a clamp because rounding can push a channel past 255. For
opaque snapshots — nearly all of them — it is a no-op.

**Still duplicated:** `tools/vitrea_spica.m` keeps its own
NSBitmapImageRep encode. It is a WebKit calibration probe whose job is
exercising raw platform primitives, and reusing the new path would mean
exposing a CG-typed function from the library. Left deliberately;
revisit if a third site ever appears.
