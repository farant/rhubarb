# Arbor M2 §2 — the `<parsura>` full-document form (spec)

Status: **design approved 2026-08-20** (Fran, section by section);
**BUILT and GATED 2026-08-21** through T6b — see
`arbor-parsura-plan.md` for the tranche table. Both gates at full
coverage: M1 subtrees 281/281 on both oracles, M2 whole files 78/78
byte-exact, zero refusals and zero divergences on either.
Remaining in M2: latinized corpus tier (T7), amalgam (T8), canon
projection (§3, independent).

**§1 carries a correction made by measurement** — read the block
quote there before trusting the anchor rule.

Read order on re-entry: this file → `arbor-stml-spec-v2.md` §0 (the
seven assumptions measurement corrected in M1) → `arbor-stml-visio.md`
§4 (the doctrine boundary this spec is required to restate) →
`silva/fontes/silva_arbor.worklog.md`.

Sibling documents this one deliberately does **not** duplicate:
`arbor-stml-exhibita.md` §II holds the origin-pool sketch that §3
below supersedes; `stml-visio.md` §2 holds the sigil registry that §4
below extends.

---

## 0. What measurement decided

Two probes were run before this design was written. Both are recorded
here because both changed a decision, and because a number that is not
written down becomes an assumption within one session.

### 0.1 The emission closure already exists

The design question was expected to be "how do we serialize
`SilvaParsura`'s fields". It is not. `silva_scribere_fontem`
(`silva/fontes/silva_scribere.h:84`) **already reconstructs a whole
file from a parsura, byte-exact.** So the question a `<parsura>`
document answers is narrower and better posed:

> what must a loaded document hand back so that the existing emitter
> produces identical bytes?

Reading its implementation (`silva_scribere.c:675`), that closure is
exactly six things: the node tree (`commissio->radix`), `directivae`,
the region tree (`expansio->regiones`, which carries the untaken
arms), the EOF token's tail trivia (`lexema_finis`, or for an included
file the last token of `expansio->includenda[via]`), the `fontes`
index→path table, and `expansio->extenta` (the function-like
invocation laminae the expansion boundary consults).

Everything else in `SilvaParsura` is parse telemetry the emitter never
reads.

### 0.2 Verbosity is structural, not origin-driven

Measured 2026-08-20 (probe `mensura_verbositatis.c`, scratchpad, not
committed).

**(A) Structural inflation, measured** — 73 files of
`probationes/fixa/roundtrip/`, 255 top-level nodes written, 8 refused:

| | bytes | vs source |
|---|---|---|
| C source | 53,765 | — |
| STML as M1 writes it (pretty) | 1,173,082 | **21.8×** |
| STML non-whitespace | 527,724 | **9.8×** |

**(B) Origin load, modelled from measured inputs** — `lib/chorda.c`
(44,028 bytes) parsed with its full include closure supplied:

- 8,956 tree tokens, of which **923 EXPANSIO** (10.3%), 0 CHORDA
- 37 distinct macros, 24.9 uses each on average
- most frequent: `ZEPHYRUM`, 195 uses
- verbose origin annotation ≈ 84,916 bytes; compact form ≈ 13,853
  (**84% off that slice**)

**The fraction that matters**: applying the corpus ratio, origin
annotation is ~16% of non-whitespace content and ~8% of the pretty
document. Compressing it 84% wins ~7% of the file.

**(C) LATINIZED inflation, measured 2026-08-21 (T7, M2 documents)** —
(A) above measured *plain C under M1*. Latinized code under M2 runs
roughly **3× worse**, because every `si`/`per`/`redde`/`NIHIL` is an
expanded token carrying a nested `<expansio>` (macro name, def-site
reference, invocation token with its trivia):

| file | source | document | ratio |
|---|---|---|---|
| `lib/tempus.c` | 15,696 | 1,235,758 | **78.7×** |
| `lib/flatura.c` | 56,891 | 4,217,944 | **74.1×** |
| `lib/arbor2_glr_tabula.c` | 1,525,680 | 83,418,424 | **54.7×** |

The largest file is the *least* inflated — a generated parser table is
mostly data, not latinized keywords. So ~70× is the NORMAL latinized
rate, not a pathological outlier; the 83 MB document is simply 70×
applied to a 1.5 MB input.

**Consequence for a real consumer**: an 83 MB / 1.9M-line document for
one source file is a genuine constraint for solarium, and the first
lever is still conclusion 2 below (the hardcoded pretty flag), not
fragments.

Three conclusions, all acted on below:

1. **Fragment compression is not built for M2** (§4). It buys 7% and
   would pull unbuilt STML machinery onto M2's critical path.
2. **55% of an arbor document is pretty-printer indentation.**
   `silva_arbor.c:1490` hardcodes `stml_scribere(involucrum, piscina,
   VERUM)` — there is no `pulchrum` knob. If size ever becomes a real
   constraint, that one-line parameter is the first lever, not a
   fragment system.
3. When compression *is* built, the target worth aiming at is
   **repeated structural shapes** (the 9.8×), not macro origins.

### 0.3 Method warning, fired twice

A reach measurement means nothing except in the apparatus real
consumers have. Silva never opens files itself; unsupplied includes
stay unknown, so `latina.h` is never read, `si`/`per` stay
identifiers, nothing expands, and the probe reports a confidently
false number that *looks like data rather than like a missing test*.

This failed on 2026-08-19 (probe v1 reported "0 refusals, all FONS" —
the exact inverse of the truth), and failed again on 2026-08-20 in the
shell, where zsh does **not** word-split an unquoted variable and the
whole header list arrived as one argument. The second time it was
caught only because the probe printed a `CAUTIO` line per unread
header. See §6.5 — this becomes a gate, not a habit.

---

## 1. What a `<parsura>` is

**A projection of the FILE, not of the parse.** Its contents are the
emission closure of §0.1 and nothing else.

**Excluded**: `strata`, the expansion journal (`acta`), and every
counter and health flag — `est_intermissa`, `expansio_decisa`,
`fines_tactae`, `segmenta_ultra_limen`, `numerus_errorum`,
`numerus_segmentorum`, `fusiones`, `transmutationes`,
`transmutationes_negatae`, `eventa_marginis_novi`, `frons_maxima`,
`regiones_textae`, `regiones_omissae`.

**Why strata are excluded** (Fran, 2026-08-20 — the reason is better
than "they are derivable"): STML is a mutable format tractable to
edits, and **layers are outputs of expansion, so mutating one is
incoherent.** A document that could hold an edited file beside an
unedited stratum is a document that can lie — the same law that made
M1 derive positions instead of carrying them. `<strata>` gets the
reserved-name treatment `origo=` got in M1: written down, not built.

**Why the health flags are excluded**: the emitter reads none of them,
and `est_intermissa` in particular is already visible in the tree — a
truncated parse puts its tail in an ERROR node. A second, forgeable
copy of a fact the shape already states is exactly what §2's rule
about `est_sumptum` refuses.

**One simplification falls out.** M1 subtree documents carry `ancora`
and `linea-initium` because a subtree cannot know what precedes it. A
file *is* the beginning: at file scope every position derives from
offset 0, line 1, and both attributes disappear.

> **STATUS 2026-08-21 (T7 complete): all SIX closure elements of
> §0.1 are now built and gated.** `extenta` — the function-like
> invocation laminae — was the one shipped last; T1–T6 built five of
> six, and no plain-C fixture could expose the gap. Latinized corpus
> now round-trips **154/154 through STML, matching silva's own
> 154/154**. A seventh case was added that §0.1 did not name:
> **empty macro invocations**, which produce no token and so are
> carried as reinserenda (`<invocatio-vacua>`) exactly like consumed
> directive lines.

> **CORRECTED BY MEASUREMENT (2026-08-21, T3b + T6b).** The claim
> above is right about the **envelope** and wrong about everything
> inside it. Two cases make derivation *impossible*, not merely
> inconvenient, so positions there are **carried**:
>
> 1. **Top-level children carry their own anchor.** Non-tree content
>    (a degraded region's laminae) can sit *inside* a node's byte
>    span — a region inside an initializer, say — so document order
>    cannot express byte order, and one linear cursor mis-places
>    everything after the first such node.
> 2. **Expanded tokens carry `b`/`linea`/`columna`, and only they
>    do.** `silva_token_ex_expansione` (`silva_token.c:78`) copies
>    the *def-site's* coordinates onto an expanded token, so its
>    position belongs to a **different file** and is absent from this
>    byte stream entirely. Deriving it would be invention.
>
> The law "a canonical document must not be able to lie" is intact:
> it governs where derivation is *possible*. A third consequence —
> an anchor must follow the origin chain to the stratum-0 invocation,
> because that is where emission actually begins — is why
> `_parsura_lexema_emissionis` exists. See the worklog's
> "ANCHOR LAW" section; this rule has now failed three times in
> three different faces.

### 1.1 Doctrine paragraph (required restatement)

`arbor-stml-visio.md` §4 requires this be named in every downstream
spec, so: **arbor documents are code documents, and code documents may
be languages.** A compact authored form whose expansion is the
reader's job is lawful here in a way it is not in a census. The line
that stays uncrossed is *computation*: substitution yes, evaluation
never. `<<x | transform>>` pipe-transforms remain barred from
truth-files (`stml-visio.md` §2), and law 2 — no expression language
in the data plane — is not touched by anything in this spec.

---

## 2. Document shape

**Document order is file order.** Directives sit where they were
written. This is safe because document order and loaded structure are
different things: a `<directiva>` element is distinguishable from a
node, so the loader **hoists** directives back into
`parsura->directivae`, and the *loaded tree* keeps the structural
purity `silva_scribere.h:19` protects ("ordinatio per offset — licita
HIC SOLUM, res strati fluxus, non arboris").

```stml
<parsura grammatica="c89" sigillum="a3f01c9e" fons="lib/chorda.c">
  <fontes>
    <fons index="0" via="lib/chorda.c"/>
    <fons index="3" via="include/latina.h" lexicon/>
  </fontes>

  <directiva><lex-hash/><lex-identificator>include</lex-identificator>
             <lex-chorda>"chorda.h"</lex-chorda></directiva>

  <definitio-functionis> <!-- M1 vocabulary, unchanged --> </definitio-functionis>

  <conditionalis conditio="7">
    <ramus sumptus><expressio>…</expressio> <!-- nodes --> </ramus>
    <ramus><expressio>…</expressio><cruda> <!-- tokens --> </cruda></ramus>
  </conditionalis>

  <cauda><lex-eof><ante><nova-linea/></ante></lex-eof></cauda>
</parsura>
```

**The document has exactly two kinds of content: nodes and token
sequences**, and M1's vocabulary already writes and reads both. No new
content kind is introduced. This holds because `SilvaRamus`
(`silva_expandere.h:96`) stores an untaken arm as `lexemata_cruda` —
an `Xar` of **tokens**, lexed but never parsed — and stores
`directiva` and `expressio` the same way.

**`est_sumptum` is deliberately not carried.** VISIO holds that "the
evaluated default track is just one configuration among many", so
stamping one configuration onto the document as an attribute would
present it as a property of the file. It also need not be carried: a
taken arm appears as nodes and an untaken arm appears as `<cruda>`
tokens, so the distinction is already structural. The `sumptus`
attribute in the sketch above is therefore **derived on write and
ignored on read** — legibility only, never consulted.

**Raw-tag alternative, considered and rejected**: untaken arms could
have used STML raw content tags (`<ramus!>`), which would be terser.
Rejected because it would introduce a second representation of tokens
(re-lexed on load), and a second scanner is exactly the thing M1
learned must be gated. One mechanism is worth the bytes.

---

## 3. Origin, in the nested form

Supersedes the pool sketch in `arbor-stml-exhibita.md` §II.

**FONS tokens carry nothing** — no attribute, no child (spec v1 §6,
unchanged). ~90% of tokens pay zero.

For the rest, the origin element nests **inside** the token element,
beside the value text and the trivia children M1 already puts there:

```stml
<lex-integer>0<expansio macro="ZEPHYRUM" def-f="3" def-l="52">
  <lex-identificator><ante><spatia n="1"/></ante>ZEPHYRUM</lex-identificator>
</expansio></lex-integer>
```

- The nested child is a **full token, not a name** — the invocation is
  where trivia lives (`invocatio trivia fert`, `silva_nodus.h`).
- The def-site is a **reference** (`def-f`, `def-l`), never inlined,
  so `latina.h`'s tokens are not copied into every document that uses
  a keyword.
- **Chains nest literally.** A macro produced by another macro puts an
  `<expansio>` inside the inner token. This is the property that makes
  nesting better than a pool: a chain is containment, not a sequence
  of `#id` hops.
- **Mixed content is already solved.** `<identificator>i8<post>…</post>
  </identificator>` is value-text-plus-child today, and the M1 reader
  takes **direct** text children only, because `stml_textus_internus`
  is descendant-wide and would otherwise swallow the child.

**Shared invocations use the dual law** (spec v1 §6). `CR(x)` is four
stratum-0 tokens producing several expanded ones, so the invocation is
a named fragment at first use and a transclusion after —
`<#inv7>…</#inv7>` then `<<#inv7>>`. Identity is preserved by
construction; duplicating would lie, and would make the emitter
re-emit `CR(x)` once per expanded token (it dedups by byte interval —
`silva_scribere.h:23`).

Two amendments to the banked design:

**Stringification arm renamed.** Spec v1 §6 lists `<expansio>`,
`<pasta>`, `<chorda>`, `<api>`. `chorda` is also a lexeme genus; while
arbor's namespaces are deliberately not disjoint, this collision is
gratuitous. The union field is `origo.datum.stringificatio` — use
**`<stringificatio>`**, matching the struct.

**Caecatio is out, and reserved.** `SilvaCaecatio*` hangs off the
expansio and pasta arms and exhibita flagged it as an M2 audit item.
It is not in the emission closure — hidesets stop recursive
re-expansion, and reconstruction never consults them. Reserved as
`cauda="#c7"`, unbuilt. It returns only if re-expansion of a loaded
parsura becomes a requirement, which is an M3+ question.

**Arms carried from day one**: EXPANSIO and CHORDA/stringification
(measured 2026-08-19: `credo.h` uses `#x`, so every probatio in the
repo produces the latter). PASTA and API may be staged behind a
**named** refusal — never a silent omission — though with four arms in
the union, filling all four may be cheaper than three plus a gate.

---

## 4. Slots — reserved, not built

`&…;` is the reference delimiter; the sigil inside names the space.
`&x;` world-space, **`&@x;` template-space**. This keeps the
2026-08-10 three-space carve (`#` document · `&nomen;` world · `.`
kind) intact rather than making template-space a sub-case of
world-space.

```stml
<!-- definition -->
<#lex-zephyrum position="@position">
  <lex-integer position="&@position;">0</lex-integer>
</#lex-zephyrum>

<!-- use -->
<#lex-zephyrum position="123"/>
```

| form | position | meaning |
|---|---|---|
| `&@name;` | attribute value | slot value, whole or interpolated |
| `&@name;` | text / children | named slot's content |
| `&@...children;` | children | **spread** of all unnamed children, in order |
| `<lex-x &@position;/>` | attribute *name* | sugar: slot name is the attribute name — reserved, never required |

**Slots are declared, not inferred.** A fragment names its slots on
its own opening tag with the G2 form `attr="@name"` (see the example
above), and `&@name;` in the body may only reference a declared slot.
An undeclared reference is a `vitium`. Inferring the slot set from
usage would make a typo silently mint a new empty slot, which is the
same failure constraint 1 forbids.

**The `...` prefix is load-bearing** (Fran, 2026-08-20): it marks
spread as a distinct construct, so `children` is not a magic reserved
name. `&@children;` is an ordinary named slot; `&@...children;` is the
spread. No collision is possible, and the prefix leaves room for
further spreads instead of hardcoding one.

Three constraints:

1. **No computation.** No conditionals, loops, expressions, or
   defaults. A missing slot is a `vitium` — law 6 says every reference
   failure is loud, and a silently-empty slot is precisely the
   fallback chain that law forbids.
2. **Define-side and use-side stay visually distinct** — `&@x;`
   appears only in templates. This is the surviving discipline from
   the dead `:`/`::` syntax (`stml-visio.md` §2).
3. **Canon-gated.** Declared per dialect, refused in data dialects.
   This is "canon limits, never demands" doing the work that keeps
   `@`'s MORTUUM verdict true where it was meant to be true — the
   registry killed `@x` because *data* documents do not compute, and
   §1.1 exempts code documents only.

**Relation to the registry.** `@x` value interpolation and `<@$1/>`
macro plumbing are listed MORTUUM. Parameterized fragments (G2:
`<#f attr="@hole">` + `<<#f attr="v">>`) are listed **RESERVATUM** in
`stml-visio.md` §6, with "YAGNI until then (venditio is one frame)".
This spec is the second and much stronger puller — every keyword in
every latinized file — and extends the reservation with **children
slots**, which G2's form did not cover.

**Deferred on a number**: §0.2 measured the win at ~7% of document
size. Reserved here in full so the next session inherits the
semantics; built when something needs it, aimed at structural
repetition rather than macro origins.

---

## 5. Architecture

**New file.** `silva_arbor.c` is already ~2,700 lines. The parsura
layer lives in `silva/fontes/silva_arbor_parsura.c`, with its surface
added to the existing `silva/fontes/silva_arbor.h`.

```c
SilvaArborScriptura
silva_arbor_scribere_parsuram (
    Piscina*                       piscina,
    constans SilvaParsura*         parsura,
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            grammatica,
    s32                            fons_index,
    InternamentumChorda*           intern);

SilvaParsura*
silva_arbor_legere_parsuram (
    Piscina*                       piscina,
    InternamentumChorda*           intern,
    chorda                         textus,
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            grammatica,
    SilvaArborVitium*              vitium);
```

- `grammatica` is a parameter for M1's reason: the registry cannot
  name itself.
- `fons_index` is a parameter for `silva_scribere_fontem`'s reason: a
  parsura holds several files, and a document is about one of them.
- M1's node writer and reader are called **unchanged** for the tree.
  The new layer owns the envelope, the sections, and the hoist.

**The hoist is the one genuinely new algorithm** — splitting
document-order content back into `radix` / `directivae` / `regiones`
where silva expects it. It is where the first real bug should be
expected, and it should be planted-faulted deliberately (§6).

**Every `interior` helper takes a `_parsura_` prefix.** In a
single-file amalgam all statics share one namespace; this collided
once already in M1 (`_nodi_aequales` against `silva_glr.c`).

**Amalgam obligations** (both have bitten): new public types go in
`silva/amalgama/silva.h` **and** in the amalgamator's
`CADENDA_TYPEDEF`; and because `silva.c` does not include `silva.h`,
the new surface is unproven until `hospes.c` calls it through the
header.

---

## 6. Gates

### 6.1 The verdict: byte equality against the file on disk

Parse → write STML → read → `silva_scribere_fontem` → compare to the
**original source bytes**.

What makes this stronger than M1's gate is that the expected value is
**external to silva** — it is the file, not silva's own output — so a
bug symmetric across silva's writer and reader cannot hide in it. This
is the VISIO oracle itself ("code→STML→load→emit == code"), the
dormant half of the fidelity oracle this parcum is named for.

### 6.2 The comparator is the diagnosis, not a second verdict

M1 framed oracles A and B as two disjoint verdicts. At file scale that
framing stops being right, because B now has ground truth. A's real
job is what it actually did in T6: when B fails, `campus` and `via`
name *which field* diverged, which settled every divergence in one run
instead of a manual bisection. `silva_arbor_aequalis` needs extending
past trees to cover directivae, regions and tail trivia.

(Note for a future spec revision: `arbor-stml-spec-v2.md` §8's stated
justification for oracle B is **wrong** — measured in T6, "A CEPIT, B
CEPIT". The correction lives in the M1 test header and in the ledger.
§6.2 here is the corrected framing.)

### 6.3 Corpus, in two tiers

- **Plain C** — the 73 files of `probationes/fixa/roundtrip/`. Should
  reach 100%. M1 refuses 8 of 263 top-level nodes there (measured
  2026-08-20), all at the expansion boundary; §3 is exactly what
  removes them.
- **Latinized** — `lib/*.c` with full include closures supplied
  (`bin/aedilis <fons.c> --partes` gives the closure). This corpus
  refuses at **100%** today. Moving it to green is M2's actual claim.

### 6.4 Planted faults per carried field

With the T6 lesson attached: plant at an **interior** position, never
at a boundary a later fix might exempt. A planted fault sitting where
an exemption is later carved out goes silent with no signal — that
happened in M1's T4 comparator test and was caught only by accident.

### 6.5 A gate on the measurement apparatus itself

The include closure must be supplied or nothing expands and every
number is confidently wrong (§0.3, failed twice). The harness must
**refuse loudly** when a file that transitively includes `latina.h`
yields zero EXPANSIO tokens, rather than reporting a clean pass. This
is a substrate/gate/refusal-loud instance: the tool gets its gate at
birth.

---

## 7. Reserved, with landing spots (no silent caps)

| reserved | spelling | landing |
|---|---|---|
| expansion strata | `<strata>` | M3+, if ever — §1 argues it should not be a document section at all |
| hideset | `cauda="#c7"` | returns only if re-expansion of a loaded parsura becomes a requirement |
| slot templates | `&@name;`, `&@...children;` | built when a consumer needs it; §0.2 priced it at ~7% |
| pretty/compact switch | `pulchrum` parameter | one line at `silva_arbor.c:1490`; the real size lever |
| structural compression | — | the 9.8× is where compression should aim; unmeasured |
| PASTA / API origin arms | `<pasta>`, `<api>` | may stage behind a **named** refusal |
| canon projection | — | M2 §3, independent of this spec; blocked on canon-side adstricta uniqueness (`canon.canon:20-27`) |

## 8. Open

- Whether `silva_arbor_aequalis` extending to non-tree content should
  stay one function or split — decide at the seam, not from memory.
- Whether an included file's document (`fons_index` > 0) is the same
  form or needs its own envelope. `silva_scribere_fontem` already
  takes the parameter, so the machinery exists; the document question
  is untouched.
- Raw-tag terminator collision was rejected along with raw tags (§2),
  but if raw tags ever return for size reasons, content containing the
  terminator string is an unhandled edge.
