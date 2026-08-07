# Glossae — localized documentation for genera and canons

**Date:** 2026-08-07
**Status:** spec v1 (conversation-derived; plan phase does codebase grounding)
**Depends on:** natura loader (lib/natura.c), canon (lib/canon.c), natura_canones projection

## 1. Intent

The Latin tag vocabulary of the `.genera` corpus is intentional and permanent —
identifiers never localize. Documentation is the opposite: it is inherently
vernacular and plural. Rather than picking one documentation language, the doc
layer is *localized from birth*: the same canonical Latin element carries
glosses in multiple languages (English and French to start).

This is the medieval **glossa** arrangement: a canonical Latin text with
vernacular glosses attached to specific words. The gloss explains; it never
renames.

Three deliverables:

1. A `<glossa>` element interspersed in `.genera` files (and admitted by the
   canon grammar so generated + hand-written canons carry docs).
2. A lint (`bin/natura_glossae`) that reports documentation coverage and can
   gate on it.
3. A generated single-file HTML page fusing encyclopedia and worklist.

## 2. The element

```xml
<genus nomen="planta" sub="vivens" modulus="corporalia">
  <definitio>
    Vivens quod se ex loco non movet et lucem in cibum vertit ...
  </definitio>
  <glossa lingua="en">A plant: a living thing rooted in place that
    turns light into food; vegetative soul only.</glossa>
  <glossa lingua="fr">Une plante : un vivant enraciné qui transforme
    la lumière en nourriture ; âme végétative seule.</glossa>
  ...
</genus>
```

- **Placement (v1 surface):** child of `<genus>` elements in `.genera` files.
  Species, proprietates, partes, machinae etc. are named deferrals (§9).
- **`lingua=`** is REQUIRED, an ISO 639-1 code treated as an untranslated
  technical term (like "offset"). Open set; the initial demand-list is
  `la,en,fr` (with `la` satisfied by `<definitio>`, §5 — only `en`/`fr`
  require glossae).
- **Content:** normalized prose text (stml_textus_normalizatus semantics);
  multi-line fine. No markup inside, no expression language, ever.
- **One glossa per language per element** — duplicates are a LINT finding,
  not a canon vitium (§5, division of labor).

### Relation to existing elements

- **`<definitio>`** (already exists, 151/177 genera, species too) is the
  canonical **Latin** documentation. It is not renamed, not migrated, and no
  `lingua="la"` glossa duplicates it. The lint counts `<definitio>` as the
  `la` column of coverage — the 26 definition-less genera become visible in
  the same census.
- **`<nota>`** (already exists, pervasive) is the author's Latin margin
  commentary. Untouched, never linted. Glossa is the reader's contract;
  nota is the writer's voice.

## 3. Grammar touch points

Three layers learn the element:

1. **`.genera` grammar** — the embedded canon that `natura_examen` applies
   admits `<glossa lingua="...">` as a child of `<genus>`. The loader
   (`lib/natura.c`, legere phase) parses glossae and CARRIES them on the
   genus (the projection and the lint both read them from the bibliotheca —
   tolerate-and-drop is not enough).
2. **`canon.canon`** — element definitions admit `<glossa>` children with
   required `lingua=`. Consequence: hand-written canons (canon.canon itself,
   others) can be glossed too, not only generated ones. canon.canon judges
   itself, so its own update must be self-consistent.
3. **`lib/canon.c`** — glossa is a known shape (lingua attribute required,
   cardinality multae) but canon judgment never ACTS on glossa content.

**Division of labor: canon checks shape, lint checks coverage/policy.**
Missing-language and duplicate-language are lint findings. This keeps the
canon layer free of documentation policy.

## 4. Projection (natura_canones)

- When writing the elementum for a genus, copy its glossae through verbatim —
  in the **monolith** (individua.canon) and in **per-module canons** both.
  The canon becomes the self-documenting artifact: whoever holds
  `planta.canon` (a foreign tool, an LLM consuming a typed report) gets the
  docs without ever seeing `.genera`.
- Species elements carry no glossae in v1 (they have none to carry).
- `.census` documents never carry glossae — docs describe KINDS; individua
  are data.
- Existing `-probare` semantic-staleness machinery covers regeneration.
- `semina.census` unchanged.

## 5. The lint — bin/natura_glossae

Thin `tools/natura_glossae.sh` wrapper over a C tool built on the same
loader `natura_examen` uses (loads ALL models; cross-model subordination
already handled there).

**Default run = REPORT** (useful while red — coverage starts near 0 for
en/fr):

```
GLOSSAE   la       en      fr
planta    9/9      4/9     0/9
animal    14/15    12/15   2/15
...
SUMMA     151/177  41/177  9/177

CARENTIA (en): planta: rosa, malus ...
```

- `la` column = `<definitio>` presence; `en`/`fr` columns = `<glossa>`
  presence.
- The CARENTIA list is the worklist docs get written from.
- **`-porta`** = gate mode: exit 1 on any gap in the demanded languages.
  Graduates into natura_visus / CI the day coverage is total; until then the
  default report never blocks.
- **`-linguae en,fr`** overrides the demand-list (default `la,en,fr`).
- Also owns the **duplicate-lingua** check (same language twice on one
  genus = finding).
- Denominator = genus elements proper (177 today), not species/umbrae/
  individua.
- Exit discipline per house law: exit 2 = NOTHING RAN (bad filter/args),
  never gate on piped `$?`.

## 6. HTML output

`-html [via]` on the same tool (default `natura/cocta/glossae.html`).
**Encyclopedia and worklist fused** — one self-contained file, no external
assets, no JS dependencies:

- Genera grouped by model, each model header showing its coverage fraction
  per language.
- Per genus: Latin name, `sub=` lineage (`planta → rosa`), the `<definitio>`
  as the Latin entry, then each demanded language's glossa — or a visible
  **⚠ deest** gap marker where missing.
- Coverage summary table at top.
- The page is simultaneously the readable documentation artifact and the
  visual worklist; gaps disappear from the page as they are paid.

## 7. Testing

- **Canon fixtures:** glossa under genus accepted; glossa missing `lingua=`
  rejected; glossa in a `.census` document rejected. Every must-fail
  verified failing on pre-change code first (pre-fix-oracle law).
- **Projection completeness sentinels:** assert a SPECIFIC glossa string
  appears in a specific generated canon (monolith and one per-module) —
  validity gates do not see omissions; completeness needs its own sentinels.
- **Lint:** counts verified on fixtures (documented, undocumented, and
  duplicate-lingua cases); `-porta` exit codes; `la`-via-definitio counting.
- **HTML:** generated file contains a known glossa string and a known gap
  marker (sentinel pair — presence AND absence surfaced).
- Loader: glossae survive legere→nectere and are reachable from the
  bibliotheca (differential or direct assertion).

## 8. The debt made visible

177 genera × 2 vernaculars ≈ 354 short paragraphs of actual authorship. The
tooling makes the debt visible and burnable-down; writing the glosses is
editorial work (LLM-draftable model-by-model, human-reviewed).

## 9. Named deferrals

- **Surface extension**: species (already have definitio — natural next
  step), proprietates/optiones, partes, machinae statuum. The denominator
  grows in announced steps, never silently.
- **Demand-list config home** beyond the CLI flag (if `-linguae` stops
  being enough).
- **Rendering elsewhere**: librarium / encyclopedia proper is the obvious
  eventual consumer of glossed canons.
- **Glossing hand-written canons** in practice (grammar admits it from
  birth; no corpus obligation yet).

## 10. Decision record

| Decision | Choice |
| --- | --- |
| Doc surface v1 | genus-level only (177 loci) |
| Language keys | ISO 639-1, untranslated technical term |
| Latin docs | existing `<definitio>` = the `la` column; no glossa la |
| Carry-through | emit glossae into generated .canon files |
| Lint shape | census tool, report default + `-porta` gate mode |
| Policy vs shape | canon checks shape only; lint owns coverage + duplicates |
| HTML | fused encyclopedia + gaps, single self-contained file, `-html` |
| nota | untouched, never linted |
