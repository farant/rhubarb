# norma spec 3 - slice B1: drafts inferred from samples

Born 2026-10-09 (Fran: inference for the herbarium pile - "work
afterwards can update code manually at least using all the outlier
payloads"; discriminators "make sense as an important thing"; "B1 first"
before a map type). Ledger …MPPK82. Follows norma-spec-2 (slice A: the
`.norma` face, which is the draft's review format) and herbarium-spec-2
(the guard that fills the pile).

*The question: given the JSON an API actually sent, can the house write
a first draft of its schema that a person can review in a diff - honest
about what the samples show and silent about what they do not?*

## 0. Data (main = quarta = a3a15e52)

- norma core: builders, `NormaVisus`, judge, export, `norma_gignere`
  (TYPICA / FINES / INVALIDA, per-path seeding, optional-field presence
  from its own stream). Format checks (`date-time`, `uuid`, `email`,
  `uri`) are static in `lib/norma.c:661-708`; reachable publicly by
  judging a value against `norma_forma(norma_textus(p), f)`.
- `.norma` writer (`norma_stml_scribere`) and reader; `bin/norma c|iudicare`.
- JSON: `json_genus` (NULLUM BOOLEAN INTEGER FLUITANS CHORDA TABULATUM
  OBJECTUM), `json_objectum_iterator`, `json_tabulatum_numerus/obtinere`.
- herbarium piles: `herbarium_enumerare(dir)` -> specimens with `status`
  and verbatim `corpus`; default home `~/.rhubarb/herbarium/<host>/`.
- Doctrine already in the house (`natura/natura.canon` header):
  "numeri OBSERVATI non sunt numeri POSTULATI" - a canon inferred from
  one corpus fossilizes it. Hence: a draft, never truth.

## I. Framing

New library `norma_inferre` (depends on norma, json; not on stml) and a
new verb `bin/norma inferre`. The draft is a `Norma*`; the writer from
slice A makes it a `.norma`. B1 has no evidence report and no drift
comparison - those are B2.

## II. API (shape; exact header at task B1.0)

```c
nomen structura {
    NormaModus  modus;             /* objecta et discrimina; ordinarium NOTANDUM */
           b32  formae;            /* formas proponere; ordinarium VERUM */
           b32  electio;           /* electiones textuum proponere; ordinarium FALSUM */
           b32  fines;             /* intra / longitudo ex observatis; ordinarium FALSUM */
           i32  exempla_minima;    /* formae/electio/fines solum si >= N; 0 = V */
           i32  electio_maxima;    /* valores distincti maximi electionis; 0 = VIII */
           i32  discrimen_maximum; /* valores tag distincti maximi; 0 = XVI */
} InferentiaOptiones;

nomen structura Inferentia Inferentia;   /* opaca */

InferentiaOptiones inferentia_optiones_ordinariae (vacuum);
Inferentia* inferentia_creare     (Piscina* p, constans InferentiaOptiones* o);
vacuum      inferentia_addere     (Inferentia* inf, JsonValor* valor);
Inferentia* inferentia_coniungere (constans Inferentia* a, constans Inferentia* b,
                                   Piscina* p);   /* lex: = addere omnia utriusque */
i32         inferentia_numerus    (constans Inferentia* inf);
Norma*      inferentia_normam     (constans Inferentia* inf, Piscina* p);
```

## III. Rules - the draft says only what the samples show

Inside, one SHAPE per position (root; each object field; each array's
elements), accumulated per sample:

| Kept | Per position |
|---|---|
| presence | instances in which the position occurred / parent instances |
| types | count per `JsonGenus` |
| strings | distinct values up to a cap (internal only), length min/max, format survivors |
| numbers | min / max (integer and float kept apart) |
| arrays | element shape (merged), length min/max |
| objects | per key: presence + child shape; per DISCRIMINATOR CANDIDATE (§IV): per tag value, a sub-shape |

Turning a shape into a draft:

- **Type.** One genus -> that genus. INTEGER + FLUITANS -> `numerus`.
  X + NULLUM -> X with `aut_nullum`. NULLUM alone -> `nullum`. Anything
  else incompatible (string + object...) -> `liberum`.
- **Required** = present in EVERY parent instance seen. Otherwise
  optional.
- **Objects** get `modus` from the options (NOTANDUM by default): the
  draft reports surprises, it does not reject them.
- **Arrays** -> `tabulatum` of the element shape; never seen non-empty ->
  `tabulatum` of `liberum`.
- **Formats** (on by default, needs >= `exempla_minima` strings): the
  first of `date-time`, `uuid`, `email`, `uri` that EVERY string sample
  passes, checked through the public judge.
- **Enums** (off by default; needs >= `exempla_minima` samples and <=
  `electio_maxima` distinct values) -> `electio`. **Bounds** (off by
  default) -> `intra` / `intra_fluitans` / `longitudo` from observed
  min/max. Off by default because they freeze the corpus and copy values
  out of possibly private responses.
- **Values never leave the samples** except: discriminator tags
  (structural) and, on request, enums.
- **Map-like positions** (an object position with > XXXII distinct keys,
  none present in more than half the instances): emitted as an
  `apertum` objectum with no fields and the descriptio
  `inferentia: positio tabulae similis (N claves distinctae)` - the
  reviewer sees it; a real map type (`ceteri`) is a later slice.

## IV. Discriminator detection

At an object position, a key T is a CANDIDATE while it has been present
in every instance, always a string, with at most `discrimen_maximum`
distinct values. For each surviving candidate the shape keeps one
sub-shape per tag value (built from just the instances with that value).
A candidate that breaks a condition is dropped for good (merging keeps
only candidates alive in both sides).

At draft time, for each candidate with >= II distinct values:

- `gain(T)` = sum over values v of |keys required within v's instances|
  minus (number of values) x |keys required over all instances|. It
  counts the keys that become required once the instances are split.
- T qualifies if `gain(T) > 0` and at least two values have different
  key sets.
- The qualifying candidate with the largest gain wins; ties prefer the
  key named `type`, then the lexically smallest name.
- **Evidence (amended B1.2, measured):** only tag values seen at least
  twice count toward gain and toward "different key sets", and at least
  two such values are required. A one-sample partition makes every key
  "required", so an id-like string field (new value per sample) would
  otherwise become a false discriminator. A value seen once still gets
  its own `variatio` in the draft.

The winner becomes `discrimen(T)` with `modus` from the options and one
`variatio` per value, each variant's `objectum` drafted from its own
sub-shape only (the tag field itself is implicit, as in the core).
Nested positions are handled the same way (Anthropic `content[]`
blocks, and a whole host pile's top-level message/error union).

## V. `bin/norma inferre`

```
bin/norma inferre [-titulus T] [-status 200] [-electio] [-fines]
                  [-sine_formis] [-clausum] <via>...
```

A `<via>` is a `.json` file (one sample) or a herbarium pile directory
(its specimens, filtered by `-status`, default 200, bodies parsed; a
body that is not JSON is skipped and counted on stderr). Writes the
draft `.norma` (one named norma, default `responsum`) to stdout; stderr
gets `N exempla, M omissa`. Exit 0 with a draft, 2 with no usable sample
or a usage error.

## VI. Testing - three laws, each born red

1. **Soundness.** Every sample passes the schema inferred from it
   (`validum`), and in NOTANDUM mode raises no note except at map-like
   positions.
2. **Merge law.** For splits A|B of a corpus: the JSON Schema export of
   `coniungere(infer(A), infer(B))` equals that of `infer(A ∪ B)`; and a
   shuffled order gives the same export.
3. **Recovery.** From known schemas (the two of `probatio_norma_gignere`,
   one with a discriminator over `content[]`, plus one with nested
   arrays, nullable fields and a top-level union) generate CCC values
   (TYPICA + FINES), infer, and compare STRUCTURE (a skeleton walk:
   genus, field names, required-ness, discriminator key and variant
   set, nullable): required stays required; an optional field the
   generator sometimes omitted comes back optional; the discriminator
   and every variant are recovered. Bounds, enums and descriptions are
   not compared (off by default).
4. **Smoke on real data.** The committed vates specimens
   (`probationes/fixa/vates/herbarium`) through `bin/norma inferre`
   (CLI path) give a draft that law 1 holds on, written in formatter
   form.

Plants: required computed as "present in any"; discriminator never
chosen (gain ignored); merge summing presence wrong; null not folded
into `aut_nullum`; format proposed when one sample fails it.

## VII. Tasks (quarta; one per turn, Fran approves each)

| Task | Content |
|---|---|
| B1.0 | header `include/norma_inferre.h` to Fran |
| B1.1 | core: shapes, addere, coniungere, rules of §III (no discriminator, no map flag), `inferentia_normam`; laws 1-2 |
| B1.2 | discriminator detection (§IV); law 3 (recovery) |
| B1.3 | map-like flag, enum/bound options, `bin/norma inferre` (files + herbarium piles); law 4 smoke |

## VIII. Not in this slice

- Evidence report (counts, "absent in 3/50", discriminator scores) and
  `bin/norma comparare` (declared vs inferred drift, coverage) - B2.
- A map type (`ceteri`: undeclared keys whose values share one schema)
  - own slice; B1 flags map-like positions instead.
- Recursive schemas, tuple arrays, string patterns, cross-field
  constraints.
- Inferring from non-JSON bodies (skipped, counted).

## AUDIENDA

- Not measured: memory per shape for large piles - distinct-value caps
  bound strings, but every discriminator candidate keeps sub-shapes per
  value (worst case XVI x keys). B1.2 measures on a CCC-sample corpus.
- Not verified: that `norma_gignere` TYPICA omits optional fields often
  enough in CCC samples for law 3's "optional comes back optional" (if
  not, the test uses more samples or FINES, which omits them - B1.2
  checks the generator's presence rate first).
- The gain rule's behaviour on real piles where a variant has only one
  sample (every key "required" within it) - B1.3 smoke shows it; B2's
  evidence report is where it becomes visible to the reviewer.

## As built (B1, 2026-10-09, quarta feefdf65..)

B1.0 header feefdf65; B1.1 core bbcb998b; B1.2 discriminator 1ba96bb9;
B1.3 map flag, opt-in enums/bounds, `bin/norma inferre` (this commit).
Departures: the discriminator evidence threshold (§IV, amended B1.2);
law 4 runs in-library in the suite (`herbarium_enumerare` over the
committed specimens) and through the CLI by hand; on real data the
threshold leaves `content[]` undivided while a variant has one sample
(seen in the smoke draft) - B2's evidence report is where a reviewer
learns why.
