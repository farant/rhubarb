# Sententiae — specification v2 (codebase-grounded)

**Date:** 2026-07-25
**Supersedes for implementation:** `sententiae-spec.md` (design intent stands;
this file is where it meets the code)
**Sources:** four parallel explorations — forum app/UI, genera+store+FTS,
ancorae+gesta, parser substrate. Interview record in
`sententiae-interview.md`.

Every number below was read from source or measured against a live store.
Where an exploration ran an experiment, it is marked **[verified]**.

---

## I. The answer sheet

v1 left nine open questions. Seven are now closed.

| # | Question | Answer |
|---|---|---|
| 1 | Does `ancorae.genus` accept a new value? | **YES, unconditionally** — it is an open string, never validated. `locus` stores and round-trips today. **[verified]** |
| 5 | Unicode folding in the seal? | **No.** 53 non-ASCII chars in the whole corpus, all dashes; NFC ≡ NFD ≡ file. Line endings only. **[verified]** |
| 6 | Field size ceiling? | **None below 64 MiB.** 45,144 bytes stored and auto-FTS-indexed. `forum.db` already holds 51,473 bytes. **[verified]** |
| 7 | Can Scholia filter anchored comments? | **No** — no filter, sort, collapse, or pagination exists, and the list caps at 200 with no offset anywhere. |
| 8 | Typed relation verbs? | **YES, today.** `verbum` is free text; 11 verbs already live on the board; `citat`/`respondet`/`refutat` work. Backlinks render with no new query. **[verified]** |
| 2 | Where does reconciliation surface? | Decided below (§VI) |
| 4 | Paste UI location? | Decided below (§V) |
| 3 | Grade filter | Still deferred; cheap once §VI exists |
| 9 | `sententia` ↔ `liber` relation | Still open; §VIII names the shape |

### The one that matters most, stated precisely

`gesta` contains **two** references to `ancorae`, and neither looks inside
the array: one marks it a system key exempt from field judgment
(`gesta.c:1883-1890`), one skips it when building the FTS corpus
(`gesta.c:3495-3504`). The only validation anywhere is at the MCP boundary
(`tabularium.c:2655-2666`) and it is *"is this a JSON array"*. The documented
set `symbolum|via|nid` lives in a tool-description **string**, not in code.

So:

> **Storing a `locus` anchor: zero code. Resolving one and reporting drift:
> all new code — but in the cleanest extension slot in the subsystem.**

`_ancoras_reddere` (`tabularium.c:898-1003`) is an if/else-if chain with **no
terminal else**, so an unknown genus renders its header and then silently
resolves nothing. Adding `locus` is one `alioquin si` branch before
`:1001` plus a resolver function. No schema, no migration, no consumer.

---

## II. Corrections to v1

### II.1 `via` is the precedent, not `nid`

v1's §VI.3 pointed at the anchor machinery generally. The exploration
sharpened it: **`nid` is the wrong model and `via` is the right one.**

A nid is a ULID **minted into the source text itself** — `identitates.sh
-mintare -scribere` rewrites the file to insert it. That is exactly what
§II.3 forbids ("the parse is one-way; the app never writes the document"),
and it is precisely what lets nid work without any drift detection: identity
is physically embedded, so the index is pure derivation.

`via` is the only genus that carries a real seal and does a real comparison
(`tabularium.c:971-1001`): file-absent → CAUTIO, seal differs → *"contentum
motum"*, seal matches → *"sigillum congruit"*.

`locus` is **`via` at finer scope** — a point inside the artifact rather
than the whole file, with a *normalized* seal rather than raw bytes.

Inherit from `via`: the three-state output shape; reading into `pn` (the
per-message arena) not `t->piscina`. Inherit from `nid`: CAUTIO text that
**names the remedy**. Break from `via`: it hashes raw bytes with no
normalization — normalization is the whole point of §V.1 and belongs in
`lib/sententiae.c`, with tabularium only ever comparing hex.

### II.2 `mutatio` on `fons` is clean — the villa instinct does not transfer

v1 inherited villa's rule that actions are `nota` + `origo`, never
`mutatio`, because mutatio is judged and would emit custody notes.

**Tested directly.** A runtime `liber` genus declaring `fons` as `area`:

```
gerere {actus:"mutatio", clavis:"fons", valor:"…"}      → clean, no note
gerere {actus:"mutatio", clavis:"ignotus", valor:"…"}   → violatio camporum + salus cautio
```

`_campos_iudicare` only fires when the genus carries a `campi` array, and
only complains about keys **not** in it. So the rule that generalizes is
**"declare the field, then mutatio is clean"** — not "always use nota".
Villa used `nota` because it had no declared field to mutate.

`nota` would in fact be *wrong* here: it appends to `notae[]` and never sets
a field, so the fold would never expose `fons`.

### II.3 `status` is reserved — use `status_libri`

The reserved set is **eight**, not two:

```
genus · titulus · corpus · tags · ancorae · signatura · status · notae
```

A `status` field in `campi` is silently unjudged *and* collides with the
lifecycle column. Two live traps follow, both silent:

- **`status` inside `datum` bypasses the state machine entirely** — field
  judgment skips it, the merge is blind, the projection copies it to the
  column. Setting it at creation also suppresses `status_initialis`.
- **`datum` beats the `corpus` argument** — `addere` writes `corpus` first,
  then merges `datum` over it.

### II.4 The address grammar was wrong

`locus := segmentum (…)` with `segmentum := [0-9]+ | [a-z]+` admits any
English word ending in a period. Measured: **275** column-0 matches instead
of 273, the extras being prose in the prooemium. The first segment must be
**numeric**. Independently, the prooemium ends at the first **fence**, not
the first address-shaped line — the naive rule truncates front matter at
`de-imagine.md:15`, mid-sentence, destroying the grade vocabulary and the
siglum table while reporting success.

Also measured: **42 indented lines** would match if a parser trimmed before
testing. The column-0 invariant is load-bearing, with a number.

### II.5 Seal must be SHA-256

`sigillum_computare` (`include/sigillum.h`). Never `chorda_friare` or
anything in the 32-bit family — at 32 bits, 273 points carries ~1-in-10⁵
birthday collision per document, and a seal collision resolves an anchor
`INTEGRUM` against the **wrong claim**. The house already draws this
distinction: *friatio* disperses, *sigillum* identifies.

---

## III. What has no existing machinery

Three things the spec assumes are cheap and are not:

**1. Nothing sweeps anchors.** `_ancoras_reddere` has exactly **one** caller
— `_tab_res`, non-`breviter` branch only. Anchors are not checked at write
time, not in `census`, not in `acta`, not in `quaerere`, not in the entity
markdown, and not in `res {breviter}`. Drift surfaces only when a human
opens one res in full.

**2. Anchors are FTS-invisible.** Skipped by the corpus builder. **[verified]**
— a `quaerere` for a term inside an anchor scope returns nothing. Finding
"every comment anchored into DI" requires a `LIKE` scan over `res.datum`;
there is no index and no anchor table.

**3. `sigillum` absence is silent.** Present-and-matching prints a marker;
present-and-differing prints CAUTIO; **absent prints nothing at all** — an
unsealed anchor is indistinguishable from a verified one.

Consequence: **§V.3's paste gate cannot be built on existing machinery.**
"Which anchors point into this document" is a question nothing can currently
answer. This is the single largest piece of genuinely new work in the
feature, and it belongs in S4.

---

## IV. The parser (S0/S1)

### IV.1 Model on `villa_lectores`, closely

`include/villa_lectores.h` · `lib/villa_lectores.c` ·
`probationes/probatio_villa_lectores.c` — read all three before starting.

Inherit:

- **arena is always the last parameter**, never stored in the result, never
  global; and it appears *only* when chordae are produced
- **internal helpers return views; public functions transcribe** at the
  `_condere` boundary through one `_copia` helper
- **refusals are a struct field, not a return code** — `successus` + enum +
  a `causa` chorda that always names the reason, built by one helper so no
  path can forget a field
- **derived judgments computed once in the library** (villa's
  `inventa/currit/fracta`) — sententiae's `retractum`, `gradus_ignotus`
- **header sections that enumerate traps** — `villa_lectores.h:285-311` is
  the model; §III.4's multi-line tags and §II.4's grammar trap belong there
- from STML, steal only `linea_erroris` / `columna_erroris` in the result
  struct — §IV.6 requires structural errors to name the line

Copy verbatim on line one: `_ch` (the `-Wcast-qual` union workaround),
`_vacua`, `_copia`, `_album`, `_linea_proxima`, `_verbum`.

**Transcribe rather than view**, even though sententiae's input outlives the
parse (unlike villa's process output). Half the strings are new allocations
regardless because normalization produces new text, and a view-holding
`Sententia` silently couples the projection's lifetime to the source
chorda's arena.

### IV.2 Containers

| | |
|---|---|
| `piscina` | threaded last, never stored. `piscina_notare`/`_reficere` to discard a failed parse wholesale |
| `Xar` ×4 | `sententiae`, `partes`, `anomaliae`, `gradus`. **Elements never move**, so the §VII reference graph can point at points while the list still grows |
| `tabula_dispersa` ×1 | `locus → index`, for duplicate-address detection (O(n²) without it) and internal reference resolution |
| `internamentum` | **no** — strings are nearly all unique. Store grades as `i32` indices into the vocabulary Xar, which also makes §V.1 step 3 an integer comparison |

### IV.3 Two public functions, not one

```c
chorda   sententiae_normalizare (chorda textus, constans Vocabularium*, Piscina*);
Sigillum sententiae_sigillare   (chorda textus_normalizatus);
```

Splitting them is what makes "rewrapping leaves the seal unchanged" a useful
test: asserted against the digest it says only that something differed;
against the normalized string it says exactly what.

### IV.4 Build

1. Create `include/sententiae.h` + `lib/sententiae.c`. **Nothing is
   registered** — `include/X.h → lib/X.c` resolution is automatic
   (`lib/aedilis.c:462-501`).
2. Create `probationes/probatio_sententiae.c`; `./compile_tests.sh
   sententiae` finds it by glob the moment it exists.
3. **Run `./tools/compile_tests_fontes_generare.sh` once.** Until then
   `lib/sententiae.c` is not in the object list. The failure is a loud
   link error, not a silent miss.
4. No hook registration — a pure library touching no POSIX is judged in
   plain C89, which is what we want.

### IV.5 Fixtures

`probationes/fixa/sententiae/`, holding a **copy** of `de-imagine.md` so
editing the live document cannot redden the suite, plus minimal twins for
drift (same document, one paragraph rewrapped; one with CRLF line endings).

**Copy the `_fixum` gate verbatim** from `probatio_villa_lectores.c:29-49`.
`filum_legere_totum` returns an *empty chorda* for a missing file, and a
parser given empty input cheerfully reports "nothing found" — a typo'd path
yields a green test that asserts nothing. Sententiae is more exposed than
villa was: "273 points" would fail loudly on a missing file, but "8
retracted points" over a *truncated* one would not.

Pin exact counts, not `> 0`: **273** points, 14 parts, max depth 4, **8**
retracted points (not 1), 21 `obj` addresses, 8 grades collected with 0
undeclared, 17 internal references resolved, and a rewrap leaving a seal
unchanged.

---

## V. The store (S2)

### V.1 The `liber` genus

```json
{"clavis":"liber","titulus_monstrans":"Liber",
 "campi":[{"clavis":"siglum","typus":"textus"},
          {"clavis":"slug","typus":"textus"},
          {"clavis":"status_libri","typus":"textus"},
          {"clavis":"fons","typus":"area"}],
 "campus_tituli":"titulus"}
```

`fons` as a genus field rather than the reserved `corpus` key keeps the body
out of the entity markdown projection (which would otherwise rewrite a 45KB
file on every touch, once per tag).

### V.2 Genus collision — the standing guidance does not work

The note carried since villa says to check *"the `clavis` AND the addere
response for `iam occupata`"*. **The violation is not in the response.**
Both collisions were
run live and both returned clean successes; the violation appears only
afterwards, in the entity's `notae`, as a machine note.

Worse, the clobber was demonstrated accidentally: after a colliding
definition overwrote a `liber` registry row, a legitimately-fielded entity
was judged against the *hijacked* schema and every real field was reported
`clavis extra campos` — as a note, so the entity was created anyway.

**An app must check the `genera` table before writing a definitio.** Treat
any hit as occupied. This is worth correcting in CLAUDE.md and filing as a
defect — the note exists, but in the one place nobody looks at creation time.

### V.3 Paste writes the body to four places

`tessellae.datum`, `tabularium.jsonl`, `res.datum`, `res_fts.corpus`.
Measured: one paste grew a scratch annals file **13 KB → 59 KB**. The live
`tabularium.jsonl` is currently 485 KB, so one *De Imagine* paste is ~10% of
the entire ledger and each re-paste adds ~46 KB permanently.

Workable, and worth stating plainly rather than discovering. Mitigations
that already hold: `entities/` never receives `fons`; `acta` summarizes it
as empty (`fons` is not in the summary key list).

**But plain `res` on a liber dumps 45 KB into the MCP response** — the
non-`breviter` branch prints raw datum verbatim. The app must never call it,
and neither should I post-compaction. A `fons` suppression in the raw-datum
print would be a one-line kindness worth filing.

---

## VI. The app (S3–S7)

### VI.0 Step zero: fix the launcher

`apps/forum/forum.sh` **omits `capsula_generare`**. Villa's launcher does it
and names the bug in a comment. Every edit to `index.html` is invisible
until the capsula is regenerated by hand. They are currently in sync, so the
staleness is latent — which is worse, because the first symptom will arrive
mid-UI-work and look like a rendering bug. Four-line fix, matching
`apps/villa/villa.sh:13-21`, including the `[ -x ]` guards forum also lacks.

### VI.1 The response-buffer ceiling drives the read design

```c
lib/cliens_tabularii.c:16  #define RESPONSUM_CAPACITAS 262144
```

One liber measured **47,176 bytes** through `legere` — the whole `datum` is
embedded per row, no truncation, no field filter. **Five or six documents
saturate the buffer**, and overflow is not clean: the read loop breaks
mid-stream, `json_legere` fails, and the UI shows a generic error it will
probably swallow anyway (`internuntius.vocare` rejects with an `Error` whose
text is on `.message`; the app's error display reads `.nuntius`).

**The failure mode for the sixth document is a blank tab.**

Compounded by `setInterval(anquirere, 2000)` refetching the active view
every two seconds, and the Tags tab sweeping all user genera.

Required:

1. **`fons` never appears in a list read.** Listing shows title, siglum,
   point count, status. The body is fetched only on open.
2. **Exclude `liber` from the tags sweep**, as `codex`/`scholium` already
   are.
3. Any paste box or jump box needs `data-inter="verum"` or polling wipes it
   every two seconds.

### VI.2 A reading tab is pure JS

Tabs are 100% client-side; C knows nothing about them. A tab is a
`<section>`, a nav `<a>`, entries in `vertere()`, and a branch in
`anquirere()`. **No C change is needed for reading** — `res_legere` and
`transmittere` already cover any genus.

One trap: `nav-articuli` is active *by exclusion* — a chain of
`novus !== '…'` tests. Forgetting to add `sententiae` lights up Articuli on
the new tab.

The only new C is the parser handler, e.g.
`sententias_parsare {fons} → {puncta, gradus, anomaliae, …}`. Note
everything is blocking on the UI thread, so parse one document per call.

### VI.3 What the reader reuses, and what it must not

Reuse: the global 22px monospace; `.linea` / `.numerus-lineae` gutter pair;
`body.latus` for width; line selection and hover highlight; `.linea.adnotata`
for the annotated-line marker; `cartam_scholii_facere` for comment cards;
`elementum_commentarii` for threads (it returns a detached node, so it drops
into a right panel unmodified); `corpus_cum_tagis` for clickable inline refs
— the exact shape `(see 8.2)` needs; `.chip` for grades and sigla; and
**`.retractum { color: #6b6357 }`, which is already precisely the dimmed
treatment §VIII.4 specifies.**

**Do not reuse the row-alignment strategy.** `reddere_codicem` emits one
flex row per anchor with `align-items: stretch`, so a tall comment card
*inserts vertical whitespace into the document body* — the source comment
says so approvingly. That is the opposite of §VIII.3's "the column never
shifts". Also, existing comments render on the **left**; the spec's layout
is parts-left / text-centre / panel-right. That is a new layout, not a tweak.

### VI.4 Search: parse in the app (§X.1)

Points-as-entities is ruled out — 273 `creatio` events per paste, and
**gesta has no delete**, so a re-paste can only add. It would also give
points ULIDs, the identity §VI.2 refuses to bind to.

A `puncta` table + its own FTS is the eventual answer (migration VII is a
two-line change; the `membra` consumer is the template; `gesta_replicare`
must learn to clear it). For v1, parse `fons` at query time — the parser
exists by then, and it preserves the rebuildable-projection property exactly.

**Document-level search is already free**: the FTS corpus builder sweeps
every string datum field, so `fons` is whole-document searchable the moment
it is written. **[verified]** — terms deep inside the body match.

**There is no stemmer at all** (not "English only"): `misericordia` will not
match `misericordiam`. For a corpus this Latin the search UI must append
`*` to terms, as `_similia_reddere` already does internally, or inflected
searches fail silently and look like missing content.

### VI.5 Reconciliation surfaces in the document view (answers q2)

Not `census`, not a global surface. The reconciliation report is about one
document's internal consistency, and the paste gate already establishes the
document view as where document-level judgments appear.

---

## VII. Revised staging

| Stage | Content | New code | Gate |
|---|---|---|---|
| **S0** | parser core | `lib/sententiae.c` | root `probationes/`, offline |
| **S1** | normalize + seal + drift classify | same lib | offline, twin fixtures |
| **S2** | `liber` genus, collision check, paste/mutatio path | JS only | headless |
| **S3** | reading view — **launcher fix first** | JS + one C handler | hand-run fumus |
| **S4** | comments, `locus` resolver, paste gate, `limbus` | **C: tabularium branch** | mixed |
| **S5** | references, external resolution, backlinks | JS | pure over parser output |
| **S6** | generated views + reconciliation | JS | pure |
| **S7** | point tags + point search | JS | headless |

**S4 is the real work.** It is the only stage needing a C change outside the
parser, and the only one whose machinery does not exist: an anchor lookup
answering "which anchors point into this document". Everything else is
either a pure library gated offline, or JS over tools that already work.

S0+S1 carry the whole correctness risk and should be complete and gated
before any interface exists.

---

## VIII. To file

1. ~~CLAUDE.md correction~~ — **corrected 2026-07-25 in the villa memory
   note, which is where the guidance actually lived** (not CLAUDE.md; that
   was a misattribution). Genus-collision guidance named a response check
   that does not work through MCP. Check `genera` before writing.
2. **`_tab_legere` embeds full `datum` per row** with no field filter — a
   general context-budget hazard, not sententiae-specific. Fifty libri ≈
   2.3 MB.
3. **`res` non-`breviter` prints raw datum verbatim** — a `fons`-class
   suppression would be one line.
4. **`_indicem_onerare` never retries** (`tabularium.c:480` sets the tried
   flag before the NIHIL check), so a missing index at daemon start is
   permanent. `_nides_onerare` gets it right. Don't clone the broken one.
5. **`forum.sh` missing `capsula_generare`** and the `[ -x ]` guards.
6. **`internuntius.vocare` rejects with `.message`, app reads `.nuntius`** —
   real daemon error text is silently dropped in the UI.
7. **`_tab_legere`'s `respondet-ad` LIKE** rests on "the forum's verb is
   unique", which new verbs erode. Materialize the verb before adding
   per-verb queries.
8. **Anchors are FTS-invisible and unswept** — worth a desideratum
   independent of sententiae.
9. Possible extraction: a `Scanner` struct (`{chorda; positus; numerus_lineae}`)
   once a third consumer of villa's line helpers exists.

---

## IX. Risks

**The paste-gate anchor lookup (S4).** Nothing existing answers "which
anchors point into DI". Options: a `LIKE` scan over `res.datum` (works now,
ugly, unindexed), or an anchor index (correct, more work). Decide at S4 with
a measurement, not in advance.

**Ledger growth.** ~46 KB per paste, permanently. Twenty pastes roughly
triple `tabularium.jsonl`. Acceptable, but if drafting turns out to mean
frequent re-pastes, versioning-by-replacement will want revisiting.

**S3's layout is new work, not reuse.** The primitives transfer; the
arrangement does not. Budget accordingly rather than assuming the Scholia
reader is 80% of the answer — it is closer to 40%, and the 60% is the part
that keeps the column still.
