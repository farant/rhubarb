# Arbor Porphyriana — species sunt genera, signum `.` in nomine

Spec for the unification of natura's kind layer: `<species>` (and
`<cultivar>`) become registered kinds — nested-genus authoring sugar —
and every kind-minting element takes a leading `.` in its tag name
(`<.genus>`, `<.species>`, `<.cultivar>`), extending the house sigil
system from citation sites to creation sites.

Status: DESIGN — decreta Franis 2026-08-10 (colloquium post-compact).
Predecessor rulings this builds on: necessitudines
(project-specs/necessitudines-spec.md, 01KZP4B8KY) and the signa
migration (`&nomen;` individua, 01KZPEXT74).

---

## §0 Decreta (Fran, 2026-08-10)

1. **Species are genera.** `<species>` is authoring sugar for a nested
   genus: same registered thing (NATURA_ENS_GENUS), the element name
   carries only the *role* (inline subdivision of the enclosing kind).
2. **The dot is part of the name.** Kind-minting elements are named
   `.genus`, `.species`, `.cultivar` — the leading `.` is inside the
   element name (canon says `nomen=".species"`), not a parser-level
   marker. Mechanism stays dumb; meaning lives in doctrine + canon.
3. **Individua stay unmarked.** No `<&…>` tag form. The absolute line
   (universal vs. particular) is visible as dotted-vs-undotted:
   `<.species>` mints a kind, `<individuum>` records a particular.
4. **The dot is an open registry.** Future kind-minting elements
   (whatever a domain demands) are self-announcing: any `.`-prefixed
   tag reads as "this mints a kind" even to a reader who has never
   seen that particular element name.

## §1 Diagnosis — what the corpus already says

Measured 2026-08-10 (re-measure: the `via=` commands):

- **271 `<species>` + 113 `<individuum>` + 1 `<cultivar>` = 385 res;
  188 `<genus>`** (via= `grep -h '<species ' natura/*.genera | wc -l`
  etc). 70% of all "res" are kinds stored as instances.
- **The loader conflates the absolute line**: one branch
  (lib/natura.c:309) treats species/individuum/cultivar identically —
  all become NaturaRes. The relative distinction (genus vs species) is
  hardcoded as two C types; the absolute one (kind vs individual) is
  erased. Exactly backwards from the Porphyrian tree, where genus and
  species are *relative roles* and only universal-vs-particular is
  absolute.
- **The apis symptom bundle** (natura/animal.genera:104):
  `<species nomen="apis" gradus="genus">` — element name and rank
  attribute contradict; apis cannot be instantiated (species content
  model admits no `<individuum>`); rosa, the same biological rank, IS
  a `<genus>` with sub= chains and full affordances. Same rank, two
  representations, different powers.
- **Currying-only species cannot recurse.** Species today have one
  verb: *fill* (`<valor>`/`<relatum>` against the parent's
  declarations — see natura/identificatio.genera, where every species
  of schema_identificandi is pure valor-fill). But isbn nests isbn_10
  and isbn_13, and isbn cannot declare what they need to curry
  (check-digit modulus, prefix space). The tree exists structurally
  while the semantics deny it. Scholastically: a species that can only
  fill parental slots has no *differentia* — the differentia adds
  being (new proprietates), it does not merely select.
- **The canon already blesses the fix.** The genus content model
  (natura/natura.canon:63-87) already admits `<valor>` with the nota:
  *"valor ad gradum GENERIS proprietatem hereditatam figit — proprium
  generis subordinati (§3)"* — plus `<relatum>`, nested `<genus>`
  ("nidificatio = profunditas taxinomica"), `<species>`, and
  `<individuum>`. The grammar carries the doctrine; the loader lags.
- **13 kind-level valores exist in the wild** (planta: rosa
  durata=perennis, malus habitus=arbor, …). Canon admits them, corpus
  authors them, no consumer reads them (NaturaGenus is a thin wrapper;
  "cetera hinc leguntur" — but nothing *legit* genus-level valores).
  A live unread-determination gap this migration closes.
- **Namespace merge is collision-free**: zero names shared between the
  genus and species populations (via= comm -12 on sorted nomen=
  extractions). Zero `&species-name;` citations exist anywhere, so no
  citation changes space.
- **semina.census is untouched**: 113 seeds = exactly the individuum
  population. Species never emitted seeds.

## §2 Doctrine

**Porphyrian relativity.** Genus and species are roles, not natures:
every intermediate kind is a species of what is above and a genus of
what is below. The registry therefore has exactly two ontological
levels: **kind** (NATURA_ENS_GENUS — arbitrarily nested and ranked)
and **individuum** (NaturaRes). `<.species>` vs `<.genus sub=…>` differ
only in authoring posture: species is the inline form for sub-kinds
small enough to live in the parent's dictionary section; genus-with-sub=
is the standalone form for heavyweight or cross-model kinds.

**Every kind can do two things, at every depth:**

- **Close** inherited questions: `<valor>` on a kind = *determinatio* —
  fixes a property the parent left open, binding all members
  (apis victus=herbivorus: every bee). This is the structural home of
  the *proprium* (taxonomy gap 3): where a value sits IS its modality.
- **Open** new questions: declare proprietates, relationes, actiones,
  partes, termini, machina_statuum of its own — the differentia's new
  being, which its own sub-kinds then curry (isbn declares the modulus
  question; isbn_10/isbn_13 answer it).

Three-layer reading, uniform at every depth:
*proprietas opens · valor-on-kind closes · valor-on-individuum reports.*

`<relatum>` on a kind = determination of an inherited relation
(apis pollinat malus_domestica narrows pollinat ad="planta" —
specialization of the relatum end).

**Markedness.** The leading `.` marks the exceptional act — minting a
universal — in documents that are mostly data. Creation site and
citation site now rhyme: `<.species nomen="apis">` mints what `.apis`
cites, exactly as `<#intro>` mints what `#intro` cites. The sigil
tripartition is now complete at both sites:

| space | citation | creation |
|---|---|---|
| kind | `.nomen` | `<.elementum nomen=…>` |
| individuum | `&nomen;` | `<individuum nomen=…>` (unmarked; data is the default) |
| document | `#nomen` | `<#nomen>` |

**gradus= stays domain vocabulary** (biology's familia/genus/species
ladder etc.), orthogonal to structure, on any kind element.

**What `<valor>`-on-kind does NOT yet decide**: essential determinatio
(sub-kinds may not override) vs default (they may). This stays an
unmarked ambiguity — one level deeper and much less pressing than the
proprium question was. Out of scope (§9).

## §3 Grammar — STML lexer

- The tag-open dispatch (lib/stml.c:1102-1135 — the tag-sigil registry
  in code: `?` PI, `!D` doctype, `/#`/`#` fragments, `(` capture) does
  not change shape. Instead, `.` becomes a legal **name-start character
  in tag-name position only** — attribute names stay `[a-zA-Z_:]`.
  Concretely: `_tok_legere_tag` and close-tag reading accept a leading
  `.` before the ordinary name characters; `_tok_legere_nomen` used for
  attribute names is untouched (split the entry point or pass a flag —
  implementation detail for the plan).
- The dot is **inside the name** (decretum 2): the element's chorda is
  ".species". Close tags `</.species>` match by ordinary name equality;
  byte-exact round-trip is free (the name contains the dot, the writer
  emits the name).
- **silva vendors its own stml copy** — re-amalgamate
  (`amalgamare.sh`) so selectors and examen see the same grammar.
- **stml-visio.md registry row**: `.` in tag-name position = kind-
  minting element, VIVIT, with the open-registry rationale (decretum
  4). Note the two recorded cautions: (a) examen scanner-trigger law —
  a `<.genus`-shaped sequence in any comment/string of a scanned file
  evokes the tag; (b) if house selectors ever adopt CSS-style `.class`
  syntax, dotted element names need an escaping story. We own both
  grammars; registry note, not blocker.

## §4 Vocabulary — natura.canon

- Elements renamed: `genus` → `.genus`, `species` → `.species`,
  `cultivar` → `.cultivar` — in their `<elementum nomen=…>` blocks and
  everywhere they appear as admitted children (`<liberum nomen=…>`).
- **`.species` content model converges on `.genus`'s full set**:
  gains glossa, proprietates, partes, actiones, relationes, termini,
  machina_statuum, individuum, and modulus= — keeps its provenance
  attributes (certitudo=, fons=, etiam=) and everything it had.
  `.cultivar` likewise.
- `.species` gets **no sub= attribute**: nesting IS its parentage
  (that is what the element name means). A kind needing an
  out-of-place parent is spelled `<.genus sub=…>`.
- **Free tripwire**: after migration, undotted `<genus>`/`<species>`/
  `<cultivar>` are undeclared elements — canon_examen screams without
  any bespoke migration code (same posture as the `#` tripwire in the
  signa migration).

## §5 Loader — lib/natura.c

- One registration path for `.genus`/`.species`/`.cultivar` →
  NATURA_ENS_GENUS, NaturaGenus, genera_omnia. Nesting implies parens
  (already the NaturaGenus pattern: "sub= resolutum, vel genus ambiens
  si nidificatum"). The species/cultivar branch that built NaturaRes
  dies; `individuum` becomes the only res-minting element.
- **etiam= on a kind** (1 wild case: scriptum_conchae
  etiam="plagula_fontis"): NaturaGenus gains a second parent
  (parens_etiam), resolved in nectere like genus_etiam is for res —
  the dual-essential-membership doctrine (2026-08-08) ports from the
  res chain to the kind chain. Apparatus walks both.
- **Determinations become readable**: consumers that walk kinds gain
  access to kind-level `<valor>`/`<relatum>` from the retained nodus.
  Minimum bar this arc: the apparatus (natura_quaere -a) shows
  inherited properties WITH the determinations that close them, and
  which ancestor closed them (e.g. `durata = perennis (determinatum a
  rosa)`); open properties remain the checklist.
- Counts after: ~460 genera, 113 res. Reconciliation gate:
  genera_post = 188 + 271 + 1 and res_post = 113, measured not
  assumed.

## §6 Consumer sweep

Same list the necessitudines arc touched; the plan grounds each with
file:line via Explore agents:

- **tools/natura_canones.c** (fold): species now project as kinds —
  they enter generated canon vocabularies as citable `.nomen` values
  wherever kinds are admitted. This is the payoff: a census can
  finally declare `genus="apis"`.
- **tools/natura_canones_emissio.c**: kind emission covers the new
  population; semina emission (individua) unchanged — gate: semina
  regenerates byte-identical.
- **tools/natura_examen.c**: -corpus/-tabulae tables re-partition
  (species rows move from res tables to kind tables); glossae.txt
  grows by definitio-derived fallback rows for the new kinds
  (fallback already exists; hand-glossing is curation, §9).
- **tools/natura_quaesitor.c**: quaere display (species entries print
  as GENUS with their role; SENTENTIA tri-state unaffected in logic,
  only in populations); apparatus determinations (§5); relationes
  walker unaffected (relatio/terminus sites live on kinds either way).
- **tools/natura_apparatus_umbra.sh**, **tools/natura_visus.sh**:
  awk/grep patterns follow the dotted names and the re-partition;
  INDEX counts flip.
- **bin/canon_examen / tools/canon_coquere.sh**: regenerate after the
  canon change; `-probare` reader gate is the semantic check.
- **rhubarb.census / exempla / documentatio censuses**: instance
  documents — no `<species>` elements exist in them; genus= attribute
  values are names, sigil-free; expected untouched (verify, don't
  assume).
- **probationes fixtures**: probatio_natura*, probatio_canon GREX,
  quaesitor/canones exempla — dotted-name migration + expectation
  updates (counts, discriminants).

## §7 Rule interactions (METAMODULUS §8)

- **Unicitas**: species names join kind-space; merge measured
  collision-free. The unicitas rule now judges one combined kind
  namespace per world plus the individuum namespace.
- **Rule XXII** (end constraints, direction-aware): kinds formerly
  species become valid narrowing targets for a=/ad= — genus_subsumitur
  now sees through species parentage (it walks parens, so this is
  free once parens exists).
- **Rule XXV** (model minimum): unchanged in text; more elements now
  count as genus.
- **Rule II discriminant** ("ad necessitudinem nominat, non ens") and
  rules XX/XXI/XXIII: unaffected — relation machinery binds by name
  against kinds, and kinds only grew in number.
- **§3/§4 of METAMODULUS** regenerate/update: element reference rows
  for .genus/.species/.cultivar, the proprium nota promoted from
  parenthetical to stated doctrine (§2 wording above).

## §8 Migration

1. Grammar first (lexer + amalgam), gated by an stml probatio that
   parses and byte-round-trips dotted tags — planted fault: a dotted
   tag must FAIL to parse before the lexer change, pass after.
2. Canon rename + content-model convergence; canon_coquere regenerate.
3. Loader unification + etiam-on-kind + reconciliation counts.
4. Corpus rename: `<genus ` → `<.genus ` (+ close tags) etc. across
   natura/*.genera and fixtures. DANGER NOTED: `genus=` as an
   ATTRIBUTE is everywhere and must not be touched — patterns anchor
   on `<`/`</`. Perl with word-boundary + tag-anchor, verified by the
   tripwire (undotted = canon vitium) and by counts.
5. Consumers (§6), each with its existing gate; suite battery
   (./compile_tests.sh natura, canon; tools/natura_struere.sh,
   tools/canon_struere.sh; tools/natura_canones.sh -probare).
6. Docs: METAMODULUS, natura.worklog, stml-visio registry row,
   memory natura-project/canon-project lines.

Planted-fault discipline throughout (house law): every gate proven RED
before its green is believed.

## §9 Out of scope (named, not designed)

- **Overridability of determinations** (essential vs default valor on
  a kind) — stays unmarked; revisit when a corpus case forces it.
- **Grammar-level kind-marker generalization** — other vocabularies
  minting kinds via `.`-tags with generic tool support: RESERVATUM in
  stml-visio until a second vocabulary wants it.
- **Hand-glossing the 272 new kinds** (glossae are definitio-fallback
  meanwhile) — curation arc, not this migration.
- **Taxonomy gap 4** (finis in prose only) — untouched by this arc.

---

## §10 Rulings from the build (2026-08-10, commits e825ce3..b6f5d5c)

1. **The empty-name crash predated the arc** and was two-faced:
   segfault with attributes, SILENT tree corruption without (the
   anonymous-close machinery swallowed the malformed close). Fixed
   first, on its own merits; `<>` lenity was pinned by an existing
   test and preserved (strictum judges TITULUS_VACUUS).
2. **Canon needed a second grammar change** beyond the plan: a new
   value-genus `titulus` (compositum + one optional leading dot) for
   elementum/liberum/intra names — attributum names stay compositum,
   so the canon grammar mirrors the lexer exactly.
3. **Rename-then-unify held.** T4's gates: examen summary byte-equal
   (188/385/65/0), census exact, cocta byte-identical, INDEX
   regenerated identical. T5's gates: 460/113/VULNERA 0, -index 573
   rows with exactly 272 flipped.
4. **The apparatus is a shared API** — T6's widening (valor/relatum
   members) leaked into the canones fold and shadowed a real
   proprietas; the -probare byte-gate caught it; the fold skips
   determination members (T7). Conservation proven by
   glossae/ordinarius/element counts per generated file.
5. **Deviation**: examen -tabulae/-corpus (and therefore visus
   tables/INDEX counts) stay raw-partitioned — the document layer
   still says "species", the graph layer says "kind". Green and
   self-consistent; repartition is a contained follow-up.
6. **Pre-existing debt surfaced**: documentatio-probatio.census
   VITIA 7 (quaestio 01KZPSA3G1) — measured against the pre-arc
   canon, identical.
7. Rule III's "proprietas typed by a res" monitum disappeared BY
   DESIGN (fixture updated to ZEPHYRUM with the reason in place).
