# Arbor M2 §2 — `<parsura>` Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: use
> superpowers:executing-plans (inline) or
> superpowers:subagent-driven-development. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** A whole C file projects to one canonical STML `<parsura>`
document and loads back such that `silva_scribere_fontem` reproduces
the original file byte for byte — including latinized sources, whose
subtrees refuse at 100% today.

**Architecture:** The parsura layer is appended to
`silva/fontes/silva_arbor.c`, sharing M1's `ArborScriptor` and
`ArborLector` because fragment numbering and the fragment table are
both **document-scoped** (see the File Structure correction). It owns
the document envelope, the section layout, and the *hoist* — splitting
document-order content back into `radix` / `directivae` / `regiones`.
M1's per-node walk is reused internally. In T6 the node writer stops
refusing non-FONS tokens and emits the nested origin form instead.

**Tech stack:** C89 under the house flags, silva's vendored stml,
`credo` for tests.

**Spec:** `project-specs/arbor-parsura-spec.md` — read §1 and §2
before T1, §3 before T6, §6 before T5.

## Status (2026-08-21)

| tranche | state |
|---|---|
| T1 envelope | shipped `bcd8a80f` |
| T2 directives + hoist | shipped `117a58b8` |
| T3a boundary measured + pinned | shipped `d1799aed` |
| T3b gap-aware cursor | shipped `6171d8eb` |
| T4 comparator | shipped `87b1eee3` |
| T5 M2 gate | shipped `e70f2588` |
| T6 nested origin | shipped `befe0d71` |
| T6b anchor follows origin chain | shipped `913299ee` |
| T7 latinized tier + apparatus gate | **DONE** — 154/154 both oracles |
| T8 amalgam | **DONE** — 3 prototypes public, hospes 39/39 through the header, calibration confirmed hospes catches a planted type error INDEPENDENTLY of the excludenda harvest (two gates, not one as the plan assumed) |

**Both gates at full coverage**: M1 281/281 both oracles, M2 78/78
byte-exact, zero refusals, zero divergences on either.

Two plan/spec corrections were made by measurement and are recorded
in place: the File Structure correction below, and spec §1's anchor
claim (the envelope carries none, but top-level children must).

---

## Global Constraints

- C89 only. House flags: `-std=c89 -pedantic -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings -Wno-long-long
  -Wno-overlength-strings`. The real list is in `compile_tests.sh`,
  not root CLAUDE.md.
- Latin identifiers, comments, messages. **Every word `#define`d in
  `latina.h` is a forbidden identifier** — `nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `exire`, `brevis` are the ones
  that have actually bitten. Use `titulus` / `tabularium`. Run
  `./silva/censor.sh <file>` on every new file.
- `i32`/`i64` are **UNSIGNED**; use `s32`/`s64` for anything signed.
  `chorda` is **not** null-terminated.
- **Every new `interior` helper takes a `_parsura_` prefix.** In a single-file amalgam all statics share one namespace;
  this collided in M1 (`_nodi_aequales` against `silva_glr.c`).
- Refusals are loud: `successus = FALSUM` + a **static** `causa`,
  never a silent skip. Reader vitia carry a `linea`.
- Run `./silva/formator.sh <file> -scribere` after each task. It
  reformats between edits, so `Edit` anchors go stale — prefer
  rewriting a whole function over patching it.
- Build/test: `./silva/compile_probationes.sh [filter]`. New test
  files need **zero registration**. **Exit 2 = nothing ran.**
- After ANY `silva/fontes` edit, `./silva/amalgamare.sh` must pass.

---

## File Structure

| file | responsibility |
|---|---|
| `silva/fontes/silva_arbor.c` | **modify** — parsura writer + reader + hoist appended (T1–T3); nested origin replaces the non-FONS refusal (T6) |
| `silva/fontes/silva_arbor.h` | **modify** — add the parsura surface (T1) and the origin tags (T6) |
| `silva/fontes/silva_arbor_aequalitas.c` | **modify, T4** — comparator past trees |
| `silva/probationes/probatio_silva_arbor_parsura.c` | **new** — unit tests (T1–T4) |
| `silva/probationes/probatio_silva_arbor_plagula.c` | **new** — the M2 gate, both corpus tiers (T5, T7) |
| `silva/amalgama/silva.h` | **modify, T8** — public prototypes, vanilla C89 |
| `silva/instrumenta/principalia/amalgamator.c` | **modify, T8** — `CADENDA_TYPEDEF` |
| `silva/instrumenta/principalia/hospes.c` | **modify, T8** — exercise the surface through the header |

**CORRECTION (2026-08-20, priced at the seam, not from memory).** An
earlier draft of this table put the parsura layer in a new
`silva_arbor_parsura.c`. That is wrong, for three reasons found by
reading the code:

1. `silva_arbor_scribere_nodum` returns **`chorda` text**, not an
   `StmlNodus*`. A separate file cannot splice node elements into a
   `<parsura>` envelope without re-parsing its own output.
2. **The fragment counter is per-call.** `ArborScriptor.numerus_notarum`
   (`silva_arbor.c:473`) restarts at zero on every call, so two
   top-level nodes would each emit `<#lex1>` and the document would
   carry duplicate ids the reader resolves wrongly. Fragments appear
   only on AMBIGUUS nodes, so this is invisible to fixtures and would
   surface as a corpus bug — the class that cost M1 two rounds.
   Fragment numbering is inherently **document-scoped**, so the
   document writer must share the node writer's state.
3. `ArborLector.fragmenta` is document-scoped for the same reason, and
   `silva_arbor_legere` consumes a whole document, not a subtree.

The alternative — promoting `ArborScriptor`, `_numerare_nodum`,
`_scribere_nodum_internum` and a lector entry to a silva-internal
header — is *more* change to shipped, gated M1 code than appending is,
and widens the symbol set the amalgam's nm-intersection gate watches.
Size is not an objection: `silva_arbor.c` is 2,730 lines against
`silva_expandere.c`'s 3,070.

---

## T1 — Envelope: `<parsura>`, `<fontes>`, tree, `<cauda>`

The smallest thing that round-trips a whole file: one with no
directives and no conditionals. Establishes the document skeleton and
the two entry points everything later hangs off.

**Interfaces produced** (add to `silva/fontes/silva_arbor.h`):

```c
#define SILVA_ARBOR_TAG_PARSURA  "parsura"
#define SILVA_ARBOR_TAG_FONTES   "fontes"
#define SILVA_ARBOR_TAG_FONS     "fons"
#define SILVA_ARBOR_TAG_CAUDA    "cauda"

SilvaArborScriptura
silva_arbor_scribere_parsuram (
                          Piscina* piscina,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
                               s32 fons_index,
              InternamentumChorda* intern);

SilvaParsura*
silva_arbor_legere_parsuram (
                          Piscina* piscina,
              InternamentumChorda* intern,
                            chorda textus,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
                 SilvaArborVitium* vitium);
```

`SilvaArborScriptura` and `SilvaArborVitium` already exist
(`silva_arbor.h`) — reuse, do not redeclare.

- [ ] **Step 1 — append a banner section to `silva_arbor.c`** with a
  header comment. State:
  the document is a projection of the FILE not the parse (spec §1);
  the emission closure is the six things `silva_scribere_fontem`
  reads; strata and health flags are excluded because layers are
  outputs of expansion and a document you can edit must not contain
  them. Name the `_parsura_` static prefix rule and why.
- [ ] **Step 2 — the envelope, write side.** Build an `StmlNodus`
  tree as M1's writer does, then one `stml_scribere(radix, piscina,
  VERUM)`. Attributes on `<parsura>`: `grammatica` (the parameter),
  `registrum-sigillum` (from `silva_arbor_sigillum`) — this is the
  name M1 already uses (`silva_arbor.c:1449`), not `sigillum`, `fons` (the path from
  `expansio->fontes[fons_index].via`). **No `ancora`, no
  `linea-initium`** — a file is the beginning, so positions derive
  from offset 0, line 1 (spec §1).
- [ ] **Step 3 — `<fontes>`.** One `<fons index="N" via="..."/>` per
  entry of `parsura->expansio->fontes` (`Xar` of `SilvaFons` **by
  value** — `silva_expandere.h:176`). Mark lexicon sources with a
  bare `lexicon` attribute.
- [ ] **Step 4 — the tree.** Drive M1's internal walk directly, ONE
  `ArborScriptor` for the whole document: `_numerare_nodum` over
  every top-level node first (pass I — counting fragment uses and
  capturing nothing else), then `_scribere_nodum_internum` per node
  (pass II), appending each returned `StmlNodus*` as a child of
  `<parsura>`. **Do not call the public `silva_arbor_scribere_nodum`
  per node** — it returns text, and it resets `numerus_notarum`, so
  two nodes would both emit `<#lex1>` and the document would carry
  duplicate ids.
  **The anchor fields on the scriptor are unused here** — a file
  starts at offset 0 — but `_numerare_nodum` sets them anyway; leave
  them set and simply do not write them onto the envelope.
- [ ] **Step 5 — `<cauda>`.** Emit `parsura->lexema_finis` as one
  `lex-eof` element inside `<cauda>`, following
  `silva_scribere.c:721-726`'s guard: emit only when
  `fons_index < 0` or `lexema_finis->fons_index == fons_index`. The
  included-file fallback (last token of
  `expansio->includenda[via]`) is **T2's** problem — for now, refuse
  loudly with `"cauda includendi nondum lata"` if that branch is
  reached.
- [ ] **Step 6 — the read side.** Validate before constructing, as
  `silva_arbor_legere` does: envelope tag present; `grammatica`
  matches; **`registrum-sigillum` matches or REFUSE** (a tree judged by the
  wrong vocabulary is a lie); every child element is a known
  section or a known genus. Allocate a `SilvaParsura`; create its
  `SilvaExpansio` with `silva_expansio_creare(piscina)` and refill
  `expansio->fontes` from `<fontes>` (the writer needs it back —
  `silva_arbor_scribere_nodum` requires a non-NIHIL expansio to
  resolve `fons_index`, and `silva_scribere_fontem` reads
  `expansio->regiones`); fill `commissio->radix` from the node
  children via `silva_arbor_legere`; set `lexema_finis`. Set
  `fons_princeps` from the envelope's `fons` attribute. Zero every
  excluded telemetry field explicitly — do not leave them
  uninitialised.
- [ ] **Step 7 — `silva_committere`.** Call it on the rebuilt
  commissio. It already does the pater fixup; there is no separate
  "recompute pater" step (measured in M1's T5b).
- [ ] **Step 8 — the failing test first.** In
  `probatio_silva_arbor_parsura.c`, take the literal source
  `"int n = 0;\n"`, parse it, write, read, then
  `silva_scribere_fontem` the loaded parsura and assert the bytes
  equal the original source exactly. Expect FAIL before Step 2.
- [ ] **Step 9 — run it.** `./silva/compile_probationes.sh
  arbor_parsura`. **Exit 2 means the filter matched nothing**, not
  that it passed.
- [ ] **Step 10 — sigil sensitivity.** Assert a document whose
  `registrum-sigillum` attribute is hand-mutated by one nibble is REFUSED,
  with a `causa` naming the seal.
- [ ] **Step 11 — censor, formator, amalgam.**
  `./silva/censor.sh silva/fontes/silva_arbor.c`, then
  `./silva/formator.sh … -scribere`, then `./silva/amalgamare.sh`.
- [ ] **Step 12 — commit.**

---

## T2 — Directives, in document order, and the hoist

**Interfaces consumed:** T1's two entry points.
**Interfaces produced:** `#define SILVA_ARBOR_TAG_DIRECTIVA
"directiva"`; the hoist behaviour later tasks rely on.

- [ ] **Step 1 — write side.** `parsura->directivae` is an `Xar` of
  `Xar*` of `SilvaToken*` — one inner list per consumed directive
  line (`silva_parsare.h:64`). Emit each as one `<directiva>`
  element whose children are the line's tokens, using M1's existing
  token-element writer. **Filter by `fons_index`** exactly as
  `silva_scribere.c:696-705` does.
- [ ] **Step 2 — placement.** Interleave `<directiva>` elements among
  the tree's top-level nodes **in file order**, by comparing the
  directive's first token `byte_offset` against the top-level nodes'
  extents. Offset-ordering is lawful here and only here — this is a
  file-level stream concern, not a tree concern
  (`silva_scribere.h:19`).
- [ ] **Step 3 — the hoist, read side.** On load, walk `<parsura>`'s
  children: `<directiva>` elements are collected into
  `parsura->directivae`; everything else becomes a `radix` child.
  **This is the one genuinely new algorithm in the plan** — the
  loaded tree must contain no directive, or structural purity is
  lost.
- [ ] **Step 4 — the failing test.** Source
  `"#include \"a.h\"\nint n = 0;\n"`. Assert byte-exact round trip
  AND that the loaded `radix` has exactly one child (the
  declaration), AND that `xar_numerus(loaded->directivae) == 1`.
  The second and third assertions are the point: byte equality alone
  would pass even if the directive were wrongly left in the tree.
- [ ] **Step 5 — run, expect FAIL, implement, expect PASS.**
- [ ] **Step 6 — the included-file cauda.** Now implement the branch
  T1 refused: when `lexema_finis` belongs to another source, take
  the last token of `expansio->includenda[via]` when it is
  `SILVA_LEX_EOF` (`silva_scribere.c:727-750`). Test with a
  two-file parse.
- [ ] **Step 7 — planted fault.** Temporarily emit directives in
  offset order but *after* all nodes; confirm the byte gate fails.
  Revert. This proves placement is actually load-bearing.
- [ ] **Step 8 — censor, formator, amalgam, commit.**

---

## T3 — Conditional regions

**Interfaces produced:** `#define SILVA_ARBOR_TAG_CONDITIONALIS
"conditionalis"`, `_RAMUS "ramus"`, `_EXPRESSIO "expressio"`,
`_CRUDA "cruda"`.

- [ ] **Step 1 — read the shapes first.** `SilvaRegio`
  (`silva_expandere.h:114`) holds `rami`, `filiae`, `est_texta`,
  `directiva_finis`, `est_imperfecta`. `SilvaRamus` (`:96`) holds
  `directiva`, `expressio`, `lexemata_cruda`, `est_sumptum`,
  `conditio_id`. **A region owns its own structural lines** —
  `#if`/`#elif`/`#else`/`#endif` never enter `directivae_out`, so
  T2's hoist must not see them.
- [ ] **Step 2 — write side.** One `<conditionalis conditio="N">`
  per top-level region in `parsura->expansio->regiones`, recursing
  through `filiae`. Per `SilvaRamus`: `<ramus>` containing
  `<expressio>` (the condition tokens; omitted for `#else`) and
  either nodes (taken, woven) or `<cruda>` wrapping
  `lexemata_cruda` (untaken). Emit `directiva` and
  `directiva_finis` token lines inside the region element, not via
  T2's hoist.
- [ ] **Step 3 — `sumptus` is derived, not carried.** Write the
  attribute for legibility; **ignore it entirely on read**. Which
  arm was taken is already structural (nodes vs `<cruda>`), and
  VISIO holds the default track is one configuration among many
  (spec §2). Assert this directly: a document with `sumptus`
  hand-flipped to the wrong arm must still round-trip byte-exact.
- [ ] **Step 4 — the failing test.** Source with
  `#if 0 … #else … #endif` where the untaken arm contains code that
  would not parse. Assert byte-exact round trip and that the
  untaken arm's bytes survive verbatim.
- [ ] **Step 5 — run, implement, pass.**
- [ ] **Step 6 — `est_imperfecta`.** A region with EOF before
  `#endif` has `directiva_finis == NIHIL`. Test it; do not let it
  crash or silently emit a closing line that was never there.
- [ ] **Step 7 — censor, formator, amalgam, commit.**

---

## T4 — Comparator past trees

The verdict is byte equality (T5). This is the **diagnosis** — when
the gate fails, it must name which field diverged. In M1 printing
`campus` and `via` settled every divergence in one run, three times,
after three wrong guesses.

**Interfaces produced:**

```c
b32
silva_arbor_parsurae_aequales (
          constans SilvaParsura* a,
          constans SilvaParsura* b,
    SilvaArborComparatioModus  modus,
        SilvaArborDifferentia* differentia);
```

- [ ] **Step 1 — extend `SilvaArborDifferentia` if needed.** It
  carries `campus`, `nodus_a/b`, `lexema_a/b`, `locus`, `index`,
  `via`. Non-tree divergences have no `SilvaNodus` — set them NIHIL
  and put the section name in `campus` (e.g. `"directiva/lexema"`)
  and the index in `index`. **Do not widen the struct** unless a
  test proves it insufficient; it is already in `CADENDA_TYPEDEF`.
- [ ] **Step 2 — compare the tree** by delegating per top-level node
  to the existing `silva_arbor_aequalis`, prefixing `via` with the
  node index.
- [ ] **Step 3 — compare directivae**: count, then per line token
  count, then per token genus / value bytes / trivia series.
- [ ] **Step 4 — compare regions**: recursively — arm count, per arm
  `conditio_id`, `expressio` tokens, and either the woven nodes or
  the `cruda` token list. **Do not compare `est_sumptum`** (T3 Step
  3).
- [ ] **Step 5 — compare `lexema_finis`** including its trivia. Tail
  trivia is the field most likely to be silently dropped.
- [ ] **Step 6 — planted fault per compared field.** For each of the
  five above, mutate one field in a loaded parsura and assert the
  comparator names *that* field in `campus`. **Plant at an interior
  position, never at a boundary** — M1's T4 planted its fault at the
  comparison root, and when the fix carved out a root exemption the
  test went silent with no signal.
- [ ] **Step 7 — censor, formator, amalgam, commit.**

---

## T5 — The M2 gate, plain-C tier

**The verdict.** Parse → write STML → read → `silva_scribere_fontem`
→ compare to the **original file bytes**. The expected value is
external to silva, so a bug symmetric across silva's writer and
reader cannot hide in it.

- [ ] **Step 1 — new file** `probatio_silva_arbor_plagula.c`,
  modelled on `probatio_silva_arbor_circuitus.c`: per-file `opus`
  piscina, repo root from `RHUBARB_RADIX` (default `".."`),
  `dirent.h` walk, `CausaNumerata` tally of refusal causes.
- [ ] **Step 2 — the loop** over `probationes/fixa/roundtrip/*.c`
  (73 files). Per file: read bytes, parse, write parsura, read back,
  `silva_scribere_fontem(loaded, …, fons_index)`, `memcmp` against
  the file bytes. **`fons_index` is `parsura->fons_princeps`** — the
  index of the primary source, which is not 0 when includes were
  supplied first (`silva_parsare.h:69`). Pass the same value to the
  writer, and assert the original and loaded parsuras agree on it.
- [ ] **Step 3 — on mismatch, run the comparator** (T4) between the
  original and loaded parsura and print `campus` + `via`. A byte
  diff alone is not a diagnostic at this scale.
- [ ] **Step 4 — assert the target.** 73/73 byte-exact. M1 refuses 8
  of 263 top-level nodes on this corpus, all at the expansion
  boundary — those are T6's, so **if any file still refuses here,
  record the cause and do not mask it**; the count must reach 0
  after T6, and the test should assert the expected count for the
  current tranche rather than "≥ some number".
- [ ] **Step 5 — the representational audit.** Tally every field
  that does not survive, by name, and print it. This is the
  deliverable the phase owes (visio §2.1), and in M1 it found two
  real bugs the unit tests could not.
- [ ] **Step 6 — commit.**

---

## T6 — Nested origin form

This is what unblocks our own code. Read spec §3 first.

- [ ] **Step 1 — tags.** Add to `silva_arbor.h`:
  `SILVA_ARBOR_TAG_EXPANSIO "expansio"`, `_PASTA "pasta"`,
  `_STRINGIFICATIO "stringificatio"`, `_API "api"`.
  **`<stringificatio>`, not `<chorda>`** — spec v1 §6 said `chorda`,
  but that is also a lexeme genus and the collision is gratuitous;
  the union field is `origo.datum.stringificatio`.
- [ ] **Step 2 — write side, in `silva_arbor.c`.** Where the writer
  currently refuses non-FONS, emit instead: an origin element
  nested **inside** the token element, beside the value text and the
  `<ante>`/`<post>` trivia children. FONS tokens get nothing at all.
  The nested child is a **full token element** (the invocation
  carries the trivia — `invocatio trivia fert`), not a name.
- [ ] **Step 3 — def-site by reference.** On `<expansio>`:
  `macro` (from `origo.datum.expansio.nomen_macro`), `def-f`
  (`corpus->fons_index`), `def-l` (`corpus->linea`). **Never
  inline the def-site tokens** — that would copy `latina.h` into
  every document.
- [ ] **Step 4 — chains nest literally.** If the invocation token's
  own origo is non-FONS, recurse: its element carries its own
  origin child. Test a two-level macro explicitly.
- [ ] **Step 5 — shared invocations.** A function-like macro's
  invocation is the origin of several expanded tokens. Use the dual
  law, exactly as M1's two-pass fragment machinery already does for
  AMBIGUUS arms: first use writes `<#invN>…</#invN>`, later uses
  write `<<#invN>>`. **Count in pass one, emit in pass two** — the
  first use cannot know a fragment is needed otherwise.
  `credo.h`'s `CR(x)` is the natural fixture.
- [ ] **Step 6 — beware the substring trap.** `<<#invN>>` *contains*
  `<#inv`, so counting fragments by substring counts transclusions
  too. This bit M1. Check containment, not the opening syntax.
- [ ] **Step 7 — read side.** Reconstruct `SilvaOrigo` from the
  nested element: set `genus`, `nomen_macro` (interned),
  `invocatio` (the nested token), and `corpus` — which cannot be a
  real pointer into a file we did not load, so build a **synthetic
  token** carrying `fons_index`/`linea` from `def-f`/`def-l` and
  mark it as having no true site (`byte_offset` −1). Document that
  asymmetry in the header; it is the honest limit of a reference.
- [ ] **Step 8 — caecatio stays out.** `SilvaCaecatio*` on the
  expansio and pasta arms is **not** in the emission closure —
  hidesets stop recursive re-expansion and reconstruction never
  consults them. Set NIHIL on load and note the reservation
  (`cauda="#c7"`) in a comment.
- [ ] **Step 9 — arms.** EXPANSIO and stringification are required.
  PASTA and API may refuse — but **the refusal must be named**
  (`causa = "origo PASTA nondum lata"`), never a silent omission.
  Given the union has four arms, filling all four may be cheaper
  than three plus a gate; decide at the code, not from here.
- [ ] **Step 10 — unit tests.** `"#define DUO 2\nint a = DUO;\n"`
  round-trips byte-exact; the tree token is `2`; its origin element
  names `DUO`. Then a `latina.h`-style two-level case.
- [ ] **Step 11 — re-run T5.** The 8 refusals on the plain-C corpus
  must now be 0, and the assertion from T5 Step 4 updated to 0.
- [ ] **Step 12 — censor, formator, amalgam, commit.**

---

## T7 — Latinized tier, and the apparatus gate

The claim M2 actually makes: our own code, which refuses at 100%
today.

- [ ] **Step 1 — the apparatus gate FIRST, before any measurement.**
  Spec §6.5. In the test harness, after parsing a file that
  transitively includes `latina.h`, assert the tree contains **at
  least one EXPANSIO token**; if zero, FAIL loudly with
  `"clausura inclusionum non praebita"`. This has fired twice in two
  days from two different causes and both times produced a number
  that looked like data rather than a missing test. Build the gate
  before the thing it guards.
- [ ] **Step 2 — closure supply.** `bin/aedilis <fons.c> --partes`
  emits tab-separated rows; the `C` rows are the local header
  closure. Feed each to `silva_includendum_praebere(exp, <basename>,
  text, len)` — **by basename**, since includes are resolved by
  name. Silva never opens files itself.
- [ ] **Step 3 — shell caution.** If any harness script passes the
  closure through a variable, **zsh does not word-split unquoted
  variables** — use `${=VAR}` or pass paths explicitly. This
  silently defeated the probe on 2026-08-20.
- [ ] **Step 4 — the loop** over `lib/*.c`, same shape as T5 but with
  the closure supplied. Byte-exact against the file on disk.
- [ ] **Step 5 — record the number, whatever it is.** Refusal
  causes tallied by name. A partial result with named causes is the
  deliverable; a masked pass is not.
- [ ] **Step 6 — worklog entry** in
  `silva/fontes/silva_arbor.worklog.md` with the before/after reach
  numbers (100% refusal → measured), and any bug the corpus found
  that the unit tests could not. That class is why this tier exists.
- [ ] **Step 7 — commit.**

---

## T8 — Amalgam integration

Both traps here have already bitten once.

- [ ] **Step 1 — regenerate the manifests, in this order.**
  `./tools/amalgama_fontes_generare.sh silva`, then
  `./tools/amalgama_excludenda_generare.sh silva`. `amalgamare.sh`
  does **not** regenerate them — it rebuilds from the committed
  `fontes_generata.h` / `excludenda_generata.h`, so skipping this
  produces a silently wrong amalgam that passes its own gates.
  **Fontes drift is silent; excludenda drift is loud.**
- [ ] **Step 2 — extend `silva/amalgama/silva.h`** with the public
  parsura surface in **vanilla C89** — `int`/`const`,
  `SilvaPiscina*`, `SilvaChorda`, no latina macros. The host must
  never see `si`/`per`/`character`.
- [ ] **Step 3 — `CADENDA_TYPEDEF`.** Any **new public type** must be
  added to the list in
  `silva/instrumenta/principalia/amalgamator.c:55`, which drops the
  internal duplicate so each type exists once per TU. C89 forbids
  duplicate *enumerators* too, so enums need it as well. If T1–T7
  introduced no new type (they should not have —
  `SilvaArborScriptura`, `SilvaArborVitium`,
  `SilvaArborDifferentia`, `SilvaParsura` are all already listed),
  record that explicitly rather than skipping the check.
- [ ] **Step 4 — extend `hospes.c`.** `silva.c` does **not** include
  `silva.h`, so nothing in the build compiles the hand-written
  prototypes against the amalgamated bodies — adding declarations
  and watching gates go green proves only that the header *parses*.
  Add a full parsura round trip through the header, beside the
  existing arbor exercise at `hospes.c:1546-1620`.
- [ ] **Step 5 — calibrate the gate.** Deliberately declare one
  parameter with the wrong type (M1 used `grammatica` as `int`),
  confirm `hospes` fails to compile naming it, then revert. A gate
  never seen to fail is a dead gate.
- [ ] **Step 6 — `./silva/amalgamare.sh`** — standalone
  full-severity compile, hospes, nm-intersection 0, censura.
- [ ] **Step 7 — full suite.** `./silva/compile_probationes.sh` and
  `./compile_tests.sh`. Note the three pre-existing root failures
  (`natura_canones`, `natura_glossae`, `planta_lectio`) so they are
  not mistaken for regressions.
- [ ] **Step 8 — RELATIO** to the ledger (`01M0DMSTEJ`) with the
  instrument debrief: adhibita / fructus / asperitates / desiderata.
- [ ] **Step 9 — commit.**

---

## Deferred, with landing spots (no silent caps)

| deferred | landing |
|---|---|
| `<strata>` | spec §1 argues it should never be a document section; reserved name only |
| caecatio (`cauda="#c7"`) | returns only if re-expansion of a loaded parsura becomes a requirement (M3+) |
| slot templates (`&@name;`, `&@...children;`) | spec §4 reserves the full semantics; priced at ~7% of document size |
| `pulchrum` parameter | one line at `silva_arbor.c:1490`; the real size lever, measured at 55% of document bytes |
| structural compression | the 9.8× is where compression should aim; unmeasured |
| canon projection | M2 §3, independent of this plan; blocked on canon-side `adstricta` uniqueness (`canon.canon:20-27`) |
| included-file documents (`fons_index` > 0) | machinery exists (`silva_scribere_fontem` takes the parameter); the document question is untouched — spec §8 |
