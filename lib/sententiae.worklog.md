# sententiae — worklog

## 2026-07-25 — S0/S1: the parser and the seal

**INTENTIO.** Build the pure half of sententiae: parse an
explicitly-addressed outline document into a rebuildable projection, and
seal each point so anchors can detect drift. No store, no UI, no I/O —
gated offline in root `probationes/` against the real corpus
(`de-imagine.md`, 273 points).

**RELATIO.** Shipped. 81 asserts, suite 106 → 107. Compiled clean under
the full flag set on the first attempt; censor PURUS, examen ACCIPE.

Design follows `villa_lectores` closely and deliberately: arena last and
never stored, internal helpers return views, public functions transcribe
at the build boundary, refusal is a struct field whose `causa` always
names the reason, derived judgments (`retractum`, `inresolutum`) computed
once in the library so consumers cannot disagree.

---

### Two things the design got wrong, both caught before they cost anything

**1. `PARENS_ABEST` as a structural error would have blocked the real
document on the first run.**

The header originally listed "parent does not exist" among the errors
that refuse a parse. Checking against the corpus before writing the
implementation: **nine loci have no parent**, and all nine are the same
shape —

```
4.obj.1   7.obj.1   10.obj.1   14.obj.1   15.obj.1
15.obj.2  18.obj.1  24.obj.1   24.obj.2
```

A *named* segment introduces a level that has no owning point. `4.obj`
never exists; `4.obj.1` is the first objection. So the rule was demoted
to an anomaly, and narrowed: it fires only when every segment of the
missing parent is numeric. That still catches a real typo (`1.1.1` with
no `1.1`) while accepting named branches. Fires zero times on the corpus,
which is the correct answer.

Worth naming the near-miss: this was found by *querying the corpus*
before implementing, not by running the parser. Had it been found by
running, it would have looked like a parser bug for a while.

**2. The retraction count was 8; it is 7.**

Stated as "8 retractions" in the spec, the interview record, the header
trap, and repeatedly in conversation. The parser found 7 and was right.

Eight *lines* contain `retracted`. Line 17 is the **vocabulary
declaration** in the prooemium — `[retracted: ...]` with a literal
ellipsis, declaring that the grade exists and takes a payload. The other
seven are applications (14.1, 29.1–29.6).

So the trap is sharper than first written: of the seven retracted points,
**six span lines**, and a line-based extractor finds exactly **one**.

The error is instructive rather than embarrassing: it was a *category*
confusion between declaring a grade and applying one — which is the exact
prooemium/body distinction this parser is built around. Measuring with
`grep -c` counted both because grep has no notion of the split. The fix
is in the test, the header, and all three spec files.

---

### The pipeline order is not a preference

```
join continuations → extract tags → extract references
```

A tag extractor that runs before joining sees six of the seven retracted
points as unterminated brackets and silently drops them. This is why
`_pendentem_condere` finishes the joined text *first* and only then calls
`_tags_extrahere`. Any future optimisation that fuses those passes
reintroduces the bug.

### Column-0 discipline, with a number

42 indented lines in the corpus match the address grammar if leading
whitespace is stripped before testing. The invariant is not stylistic —
it is the only thing distinguishing an address from a sentence that
happens to begin with a word and a period.

### The grammar needed tightening

`segmentum := [0-9]+ | [a-z]+` in *initial* position admits any English
word ending in a period — 275 matches instead of 273, both extras being
prose in the prooemium. The first segment must be numeric. Independently,
the prooemium ends at the first fence rather than the first
address-shaped line; the naive rule truncates front matter mid-sentence
at line 15, destroying the grade vocabulary and the siglum table *while
reporting success*.

---

### Three copies of subtle code, extracted before compiling

Finalising a pending sententia happens at three points — fence close,
blank line, next address. The first draft inlined it three times.

Villa's worklog already records where that leads ("two copies of subtle
code drift"), and the consequence here is worse than usual: a divergence
between the three would produce a point with a wrong seal, i.e. a lying
anchor. Extracted to `_pendentem_condere`, which also clears `pendens`
and `aed` itself so no caller has to remember to.

The refactor was done *before* the first compile, on the grounds that
"it works, clean it later" is how three copies become permanent.

---

### `duplex` is a latina.h macro — third firing of the class

`Liber duplex;` expands to `Liber double;`. Renamed to `duplicatus`.

Third time in two days after `ordinarius` and `nomen`. What is worth
recording is the tool behaviour, not the mistake:

```
[suspectum in errore] ... macra latinae intra fracturam parsationis:
  'principale', 'vacuum', 'si', 'imprimere', 'redde', 'I', 'FALSUM',
  'ZEPHYRUM', ...
conditio:     PURUS
```

Censor **saw** the parse fracture and listed eight macros in the region,
none of which was the culprit — then reported `PURUS`. Clang named it
instantly. Recorded as a third `ictus` on `01KXTRM0DB`; a tool that sees
a fault but cannot name it costs a round trip through the compiler, which
is a tool that ends up unused.

---

### Details worth keeping

- **`_album` is a four-character test on `i8`, not `<ctype.h>`.**
  `character` is signed here, so a UTF-8 lead byte would go negative and
  `isspace()` would be undefined. Copied from villa for the same reason.
- **`chorda_sectio` returns empty on an out-of-range slice** rather than
  erroring, so a bounds bug presents as a legitimately-absent field.
  Avoided by constructing views by hand with explicit arithmetic.
- **`_locus_capere` excludes the trailing period** from the returned
  locus: `4.obj.1.1`, not `4.obj.1.1.` — the canonical citation form
  (`DI/4.obj.1.1`).
- **Normalization and sealing are separate public functions.** The
  rewrap test asserts against the normalized *chorda* first and the
  digest second, so a failure says what differed rather than only that
  something did. This immediately paid off while debugging the CRLF
  fixture.
- **`sententiae_per_locum` is a linear scan.** 273 points makes the
  duplicate-locus check O(n²) ≈ 37k chorda compares, microseconds. A
  `tabula_dispersa` was specced and skipped as premature; revisit only if
  a document reaches thousands of points.
- **`ANCORA_MOTA` is detected and never followed.** Following handles
  pure reordering invisibly but must guess when text was both moved and
  edited, and a wrong guess is a lying anchor — worse than a broken one,
  because nothing signals it.

### Instrumenta

- **adhibita:** censor, examen, clang direct, `compile_tests.sh
  sententiae`, `compile_tests_fontes_generare.sh`, ad-hoc corpus queries
- **fructus:** the corpus queries were the highest-yield instrument by a
  distance — the parent-absence finding and the grammar tightening both
  came from querying `de-imagine.md` directly rather than from running
  code. Examen and censor were fast and silent-when-clean on the header
  and the library.
- **asperitates:** censor's `PURUS`-with-`suspectum` on a parse fracture
  (see above). Also: `compile_tests.sh` emits ANSI colour into its
  summary, so reading the result programmatically needs a workaround —
  already filed as `01KYB29BC6` with two ictus.
- **desiderata:** none new. The `Scanner`-struct extraction from villa's
  line helpers now has its second consumer (this library copied
  `_linea_proxima`, `_verbum`-style helpers, `_album`, `_ch`, `_vacua`,
  `_copia`); a third would justify promoting them. Noted in spec-v2 §VIII.9.

### Next

S2 is the store: the `liber` genus, the collision check that must read
the `genera` table rather than trust the `addere` response, and the
paste/mutatio path. The parser is complete and gated; nothing downstream
needs to re-derive its behaviour.
