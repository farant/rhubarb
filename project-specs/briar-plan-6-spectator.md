# briar plan 6 — spectator (the viewer, `-visio`)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit).
> Steps use checkbox syntax. Written 2026-09-11 from briar-spec.md §4.7
> (v1.8, approved by Fran the same day). Not started.

**Goal:** `./x.thistle -visio` opens a window on the page `-html`
writes, and clicking a library symbol shows its definition — parsed
from the corpus on demand, cached per file.

**Architecture:** one new binary, `bin/briar-spectator`, on silex's
shape and atrium's scaffold: briar's modules + the corpus + vitrea.
It repeats NO rendering — it calls `briar_faciem_fingere`, writes the
page, and points atrium's `capsula_radix` at the directory holding it.
briar stays headless; `-visio` execs. Stage (a) is the window, stage
(b) the bridge.

**Tech stack:** C89 in Latin, briar modules, silva (amalgam), silex
closure, atrium + vitrea + fenestra, capsula, credo gates,
`tools/briar_fumus.sh`, `bin/manus` for the `-agere` stage.

**Spec:** `project-specs/briar-spec.md` §4.7 (S1–S4, the bridge
contract, the gate story), §4.6 (the page it shows), §5 (flag table),
§6 (gate row), §8 (P7), §9 (what stays reserved).

## Global constraints

- C89, `latina.h` words; latina macros are forbidden identifiers. A
  glob of the form `lib` slash star dot c inside a block comment breaks
  compilation (`-Wcomment`) — write it in words.
- Main tree, branch `main`. New C files through `scribe <via> <<'EOF'`;
  format `./silva/formator.sh <viae> -scribere` then `-vitia` =
  CONFORMIS; commit through `silva.commissio_umbra(msg, viae, portae)`
  with individual paths, portae `['briar', 'briar-fumus', 'pythonica']`.
- **The formatter reflows signatures**, so exact-match anchors on
  formatted C rot between edits — use regex when re-editing a signature
  (bit plan 5 three times).
- The commit-blocking Latin lint is live: check new words with
  `./oratio/quaere.sh -machina <verbum>` BEFORE writing them, and run
  `. tools/unci-git/lint_latinus.sh && lint_latinus` before committing.
- No clang, no window and no network in the suite. The window is
  `-agere` only.
- Every new gate is born red by a planted fault, the plant is checked
  GONE (`grep -c` = 0), and **the number of reddened assertions is
  read, not just the colour** — a plant that barely reddens is naming
  a coverage hole (plan 5, T3).

## File structure

| file | responsibility |
|---|---|
| `tools/briar_spectator.c` (new) | the binary: fabricate → render → write → atrium → bridge |
| `tools/briar_spectator_struere.sh` (new) | build + install `~/.bin/briar-spectator` (exemplar: `silex_struere.sh`) |
| `briar/fontes/briar_symbolum.h/.c` (new) | the `facies.symbolum` answer as a PURE function over the corpus, with the per-file silva cache — headless, so the gate never needs a window |
| `briar/probationes/probatio_briar_spectator.c` (new) | the gate: the handler's answers + byte identity with `-html` |
| `briar/facies/facies.js` | the bridge → island → nothing fallback |
| `briar/fontes/briar_imperium.h/.c`, `tools/briar.c` | `BRIAR_ACTIO_VISIO`; exec, refuse by name if absent |
| `tools/briar_fumus.sh` | stage XII under `-agere` (window driven by `bin/manus`) |
| `briar/CLAUDE.md`, worklog, spec §4.7 as-built + §8 P7 | docs |

---

### Task 1 (stage a): the window

**Files:** create `tools/briar_spectator.c`, `tools/briar_spectator_struere.sh`, `briar/probationes/probatio_briar_spectator.c`; modify `briar/fontes/briar_imperium.h/.c`, `briar/probationes/probatio_briar_imperium.c`, `tools/briar.c`.

**Interfaces produced:** none new in briar's libraries — Task 1 is a
binary and a flag. The page-writing step must be a named function in
`tools/briar_spectator.c` so Task 1's gate can call it without a
window:

```c
/* plagulam thistle in paginam reddere et in <domus>/facies/ scribere;
 * viam paginae reddere (aut vacuam + causa). NIHIL aperit. */
interior chorda
_paginam_parare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans SilexFons* fons,
     constans character* via_thistle,
                 chorda* causa);
```

- [ ] **Step 1: the failing gate.** `probatio_briar_spectator.c`: the byte-identity assertion FIRST, because it is the one that keeps two paths honest — render `fixa/thistle/salve.thistle` through `briar_faciem_fingere` with the REAL chrome (read from `briar/facies/`, not test chrome: this asserts what ships), and compare byte for byte with what the same call produces through the spectator's own `_paginam_parare`. Run red (no such file yet).

- [ ] **Step 2: the binary.** `tools/briar_spectator.c` on the generated vitrea main's shape (`fixa/fabrica/salve_vitreum/fontes/salve_vitreum.c` is the model, and `tools/silex.c` for the corpus): `principale(argc, argv)` → read the thistle path → fabricate exactly as `-html` does → `briar_faciem_fingere` → write `<t>.html` into `<domus proiecti>/facies/` → `atrium_creare` with `figura.capsula = &capsula_facies_briar`, `figura.capsula_radix = <that dir>`, `figura.via_initialis = "<t>.html"` → `atrium_vexilla_legere(&figura, argc, argv)` so `-radix` and the driver port still work → loop. The corpus comes from `capsula_corpus_silicis` (declare the symbol directly; never include a generated header).

- [ ] **Step 3: the struere script** `tools/briar_spectator_struere.sh` on `silex_struere.sh`'s model: briar objects (minus `probatio_*`), the corpus capsula, the facies capsula, atrium/vitrea/fenestra and the `.m` files with `-framework Cocoa -framework WebKit`; `rm -f` before `cp` into `~/.bin` (macOS SIGKILLs an overwritten running binary).

- [ ] **Step 4: the flag.** `BRIAR_ACTIO_VISIO` in `briar_imperium` (+ the assertions plan 5 added for `-html`, same shape, and the refusal message naming it); in `tools/briar.c`, exec `briar-spectator` with the file and remaining args. **Absent binary = refusal BY NAME** naming `tools/briar_spectator_struere.sh` — briar never opens a window itself.

- [ ] **Step 5: green + plant.** Gate green; plant by making `_paginam_parare` write through a second `briar_faciem_fingere` call with test chrome — byte identity must redden. Restore, `grep -c` = 0.

- [ ] **Step 6: by hand.** `./tools/briar_spectator_struere.sh`, then `cd project-specs/exempla && briar -visio salutatio2.thistle`. **Look at it.** Does the page in the window match the file in the browser? Record what you changed after looking.

- [ ] **Step 7: format, lint, commit** (portae `['briar', 'pythonica']`).

---

### Task 2 (stage b): the bridge

**Files:** create `briar/fontes/briar_symbolum.h/.c`; modify `tools/briar_spectator.c`, `briar/facies/facies.js`, `briar/probationes/probatio_briar_spectator.c`, `tools/briar_fumus.sh`, docs.

**Interfaces produced:**

```c
/* briar_symbolum.h - definitio symboli bibliothecae EX CORPORE.
 * Functio PURA: nulla fenestra, nullus pons - ergo porta eam sine
 * ulla re vitrea probat. Cursor (arbores parsatae per plagulam)
 * datur, non intus absconditur: spectator eum trans vocamina tenet. */
nomen structura BriarCursorSymbolorum BriarCursorSymbolorum;

BriarCursorSymbolorum*
briar_cursorem_symbolorum_creare (Piscina*, constans SilexFons*);

nomen structura {
       b32 inventum;
    chorda caput;       /* 'piscina.h' */
    chorda via;         /* 'lib/piscina.c' */
    chorda definitio;   /* extensio verbatim */
       i32 linea;
} BriarSymboliResponsum;

BriarSymboliResponsum
briar_symbolum_quaerere (
    BriarCursorSymbolorum* cursor,
                   chorda  titulus,
                   chorda  caput);
```

- [ ] **Step 1: failing assertions**, all headless: `piscina_generare_dynamicum` + `piscina.h` → found, `via` = `lib/piscina.c`, the definition text starts with its return type and contains the function's name, `linea` > 0 · a header with NO twin (pick one from the corpus with no `lib/` file) → `inventum` FALSUM, no crash · a symbol absent from the corpus → FALSUM · a symbol DECLARED in a header but not defined in the twin → FALSUM (not a half-answer) · the cache: two calls for two symbols in the same file return identical bytes for each, and the second call parses nothing (assert by a counter on the cursor).

- [ ] **Step 2: run red. Step 3: implement** `briar_symbolum.c`: header → twin by silex's rule; read from the corpus (`silex_fons_legere`), never from disk; parse once with silva; find the definition through the semantica the way `briar_facies` does; keep parsed trees in a `TabulaDispersa` on the cursor keyed by path.

- [ ] **Step 4: wire the bridge.** In `tools/briar_spectator.c`, register `facies.symbolum` through `internuntius_praebere(atrium_internuntius(atrium), ...)` with the house handler signature; the cursor lives in the app's piscina across calls.

- [ ] **Step 5: `facies.js`** gains the fallback chain — bridge if `internuntius` exists, else the inline island, else nothing. The page must still read with scripting off; re-run the facies gate, and the `-html` goldens must NOT move (the island and markup are unchanged).

- [ ] **Step 6: green + plant.** Plant by making the cache return the FIRST file's tree for every path — the identical-bytes assertions must redden, and count how many.

- [ ] **Step 7: fumus stage XII (`-agere`)**, on stage IV's model: start the spectator on `salutatio2.thistle`, read `atrium_portus`, `bin/manus` → body contains `frag-principale` (page loaded), press a `data-s` symbol by text, body then contains `piscina.h` (bridge answered). Non-`-agere` runs skip it.

- [ ] **Step 8: docs + seal.** `briar/CLAUDE.md` (the spectator, the laws it inherits, `-visio` refusing by name), worklog (what the window taught that the page did not), spec §4.7 as-built + §8 P7 DONE, memory, ledger nota. Commit with portae `['briar', 'briar-fumus', 'pythonica']`; rebake `~/.bin/briar` and `~/.bin/briar-spectator`.

---

## Self-review

**Spec coverage.** S1 (C tool on silex's shape) T1 step 2–3 · S2 (a then
b) = the two tasks · S3 (`capsula_radix`, one self-contained file) T1
step 2 · S4 (one chrome, degrading) T2 step 5. The bridge contract T2
steps 1–4. Every gate row of §4.7: the handler's five headless cases
(T2 step 1), byte identity (T1 step 1), stage XII (T2 step 7).

**Named risks.** (1) `atrium_vexilla_legere` consumes argv for atrium's
own flags — the thistle path must be read BEFORE it, or `-radix` and
the file argument will fight; T1 step 2 orders them. (2) The byte-
identity assertion uses the REAL chrome, so it will move whenever
`facies.css` changes — that is intended (it asserts what ships), but it
means the assertion compares two in-memory renders rather than pinning
a golden, so it stays stable under restyling. (3) A `.m` link plus two
frameworks is new for briar's build; if it fights, T1 step 3 is where
it surfaces, before any design rests on it.
