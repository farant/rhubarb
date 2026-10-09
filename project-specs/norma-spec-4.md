# norma spec 4 - slice B2: evidence in the draft, and drift

Born 2026-10-09 (Fran chose evidence INLINE in the draft, option A).
Follows norma-spec-3 (B1: `norma_inferre`, `bin/norma inferre`).
Ledger …MPPK82.

*The question: when the house hands a reviewer a draft schema, can every
line say why it is what it is - and can a declared schema be checked
against what an API actually sends?*

## 0. Data (quarta af736ad3)

- Measured 2026-10-09: STML comments inside a `.norma` are accepted by
  canon (0 vitia), skipped by the reader (`bin/norma c` loads the file),
  and kept by `stml formare` on their own line before the element. A
  comment INSIDE `<campus>` forces block form; placed BEFORE the element
  it keeps capture form.
- `stml_commentum_creare(piscina, intern, textus)` exists
  (`include/stml.h:652`).
- The B1 smoke draft of the committed vates specimens leaves `content[]`
  undivided (one `server_tool_use` among `text` blocks) and the reviewer
  cannot see why - the motivating case.
- `norma_inferre` (B1) keeps per position: presence, type counts,
  distinct values (internal), lengths, discriminator candidates with
  partitions.

## I. Framing

Two parts, layered so no library learns more than it must:

1. **A writer hook** in `norma_stml`: a callback that returns a comment
   for a node; the writer places it as an STML comment before that
   node's element. Generic - inference is one user.
2. **Evidence** in `norma_inferre`: a draft whose nodes carry their
   evidence, plus the callback that hands it to the writer.
3. **Drift** in `norma_inferre`: `norma_comparare(declared, inference)`
   walks a declared schema beside the inferred shapes and lists
   discrepancies; `bin/norma comparare` prints them.

## II. API (shape; exact headers at task B2.0)

```c
/* norma_stml.h (additive) */
/* commentarius: textus pro nodo, chorda vacua = nullum; scriptor
 * commentum STML ANTE elementum nodi ponit. Textus cum "--" recusatur. */
nomen chorda (*NormaCommentarius)(
    constans Norma* nodus, Piscina* piscina, vacuum* datum);

chorda norma_stml_scribere_cum_commentis (
    constans NormaNominata* normae, i32 numerus, Piscina* piscina,
    chorda* causa, NormaCommentarius commentarius, vacuum* datum);

/* norma_inferre.h (additive) */
nomen structura InferentiaTestimonia InferentiaTestimonia;  /* opaca */

/* adumbratio + testimonium cuiusque nodi */
Norma* inferentia_normam_testatam (constans Inferentia* inf, Piscina* p,
                                   InferentiaTestimonia** testimonia);
/* NormaCommentarius pro norma_stml_scribere_cum_commentis (datum =
 * testimonia) */
chorda inferentia_commentarius (constans Norma* nodus, Piscina* p,
                                vacuum* testimonia);

nomen enumeratio {
    NORMA_DISCREPANTIA_FORTASSE_REQUISITUM, /* optionale, in omnibus visum */
    NORMA_DISCREPANTIA_REQUISITUM_ABSENS,   /* requisitum, in quibusdam deest */
    NORMA_DISCREPANTIA_NUMQUAM_VISUM,       /* declaratum, numquam visum */
    NORMA_DISCREPANTIA_NON_DECLARATUM,      /* visum, non declaratum */
    NORMA_DISCREPANTIA_GENUS_LATIUS,        /* genera visa extra declaratum */
    NORMA_DISCREPANTIA_NULLUM_NOVUM,        /* null visum, non aut_nullum */
    NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA,
    NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA,
    NORMA_DISCREPANTIA_NON_COMPARABILE      /* e.g. tag non candidatus */
} NormaDiscrepantiaGenus;

nomen structura {
                    chorda  via;     /* $.content[].type ; $["a.b"] */
    NormaDiscrepantiaGenus  genus;
                    chorda  nuntius; /* numeri, numquam valores */
} NormaDiscrepantia;

Xar* norma_comparare (constans Norma* declarata,
                      constans Inferentia* inf, Piscina* p);
```

`norma_inferre.h` includes `norma_stml.h` for the callback TYPE only;
`lib/norma_inferre.c` calls no STML function (layering kept).

## III. What the evidence says

Comments are Latin, counts and decisions only - never literal values,
except discriminator tag values (structural) and enums when requested.

| Where | Comment |
|---|---|
| root | `N exempla` |
| field (any node in an object) | `visum P/Q` (instances with the key / object instances) |
| more than one genus, or nullable | `genera: textus 41, nullum 2` |
| strings | `distincti K` or `distincti > 16` |
| arrays | `longitudo 0..7` (structural, not a value) |
| format proposed | `forma date-time: 50/50` |
| discriminator chosen | `discrimen 'type': lucrum G` (+ `alter 'kind' lucrum H` when another qualified) |
| discriminator rejected (a candidate with >= II values) | `discrimen 'type' reiectum: valores bis visi I (II postulati)` / `lucrum 0` / `claves partitionum aequales` |

`bin/norma inferre` writes the evidence by default; `-sine_testimoniis`
omits it. A node without evidence (e.g. a nullum leaf) gets no comment.

## IV. Drift - `norma_comparare`

Walks the declared schema beside the inferred ROOT SHAPE (not the
inferred draft), so it sees counts:

- object field: declared required + presence < instances ->
  REQUISITUM_ABSENS; declared optional + presence == instances ->
  FORTASSE_REQUISITUM; declared + presence 0 -> NUMQUAM_VISUM; key in the
  shape, not declared -> NON_DECLARATUM.
- genus: any observed JSON genus outside the declared one (integer
  under `numerus` is fine; anything under `liberum` is fine) ->
  GENUS_LATIUS; NULLUM observed, declared not `aut_nullum` ->
  NULLUM_NOVUM.
- discriminator: if the shape has a live candidate on the declared key,
  each variant is compared against its partition; declared variants with
  no partition -> VARIATIO_NUMQUAM_VISA; partitions with no declared
  variant -> VARIATIO_NON_DECLARATA. No live candidate ->
  NON_COMPARABILE (one finding, no descent).
- arrays: element vs element shape.
- Only positions the samples reached are compared below the top: a
  field never seen produces NUMQUAM_VISUM once, no findings inside it.

`bin/norma comparare <x.norma> [-norma T] [-status N] <via>...` prints
`via: GENUS: nuntius` lines; exit 0 none, 1 drift found, 2 usage /
load error.

## V. Testing

1. **Writer hook**: comments land before the right elements; the result
   reads back (`norma_stml_legere`) to the same schema as without
   comments; it is in formatter form (`stml_scribere` of its parse ==
   itself); a comment containing `--` is refused.
2. **Evidence content** on known corpora: presence counts, genus
   counts, distinct counts; a chosen and a rejected discriminator with
   the exact reasons; the SECRETUM corpus yields no literal value in any
   comment.
3. **Drift**: one fixture per discrepancy genus (declared schema +
   samples), each asserting the finding and its path; a declared schema
   equal in structure to the samples yields none; the committed vates
   specimens against `lib/vates_responsum.norma` yield the expected
   coverage findings (e.g. `thinking` / `redacted_thinking` variants
   never seen) - asserted as present, not as an exhaustive list.
4. Plants: comment placed after instead of before (hook test red);
   presence miscounted in evidence; FORTASSE_REQUISITUM never raised;
   variants compared against the whole shape instead of partitions.

## VI. Tasks (quarta; one per turn)

| Task | Content |
|---|---|
| B2.0 | headers to Fran: `norma_stml.h` hook, `norma_inferre.h` evidence + drift |
| B2.1 | writer hook + `inferentia_normam_testatam` / `inferentia_commentarius`; `bin/norma inferre` writes evidence by default (`-sine_testimoniis`); tests 1-2 |
| B2.2 | `norma_comparare` + `bin/norma comparare`; test 3 incl. the vates smoke |

## VII. Not in this slice

- A map type (`ceteri`) - own slice.
- Evidence for drafts built by hand (the hook is generic, but only
  inference fills it now).
- Drift on literal values (enum members, bounds) - counts only.

## AUDIENDA

- Not verified: how `stml_scribere` lays out a comment before a capture
  child (`<tabulatum(>` + comment + type) - B2.1 measures and the test
  pins the form.
- Not decided: evidence on discriminator VARIANT objects (presence within
  the partition, `visum P/Q` against partition instances) - B2.1 uses
  partition counts, stated in the comment.
