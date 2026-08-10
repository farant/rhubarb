# Necessitudines — first-class relation kinds for natura

Spec, 2026-08-10. From the relation-corpus classification (this session)
+ Fran's rulings. Addresses gaps 1 (relation vocabulary) and 2 (roles)
of nota 01KZMA913V; gives the `inversa` wound (01KZC7EFMR) its lawful
resolution.

## 1. Why (the measurements)

`bin/natura relationes -machina`: 432 usages, 325 names, 272 singletons
(83%). Classification into ~17 families (working file: this session's
relationes_familiae.md; the families move into the core module, §5).
Structural discoveries the design answers:

1. **Role-frames are the biggest family (~36 names)** — singletons
   cluster ON act-genera as participant roles (venditio: emptor/
   venditor/res_vendita/solutum; iter: a_loco/ad_locum/per_viam).
   Terminus a quo / ad quem independently reinvented 3×.
2. **Morphological fragmentation** — one relation, several spellings by
   gender/voice agreement (situm_in/sita_in/situs_in;
   custodita_a/custodita_per/custoditur_ab).
3. **Undeclared converse pairs** — pollinat/pollinatur_a,
   continet/continetur_in, legit/legitur_a…; the current format can
   only say `inversa=` (boolean) or `a=` (direction), never "these two
   names are one relation."
4. **The `_de` pattern** (~14 names): relation = bearer's kind + "of".
5. **Synonym duplication** a dictionary would prevent (dependet_ex vs
   pendet_ex).
6. **Reuse tracks generality**: de(9), continet(9), asseritur_per(8)
   converge; specific names singleton. causat: 1 dictionary use, yet
   it is rhubarb.census's spine.

The corpus says what things ARE richly and how they STAND crudely:
`<proprietas>` is typed/bounded/defaulted; `<relatio>` is a bare string
+ target + 3 booleans.

## 2. Decreta (decision log, Fran 2026-08-10)

- **First-class relation kinds** ("necessitudines"), declared once,
  dictionary-grade: definitio, glossae, `sub=` inheritance. The 17
  families are the taxonomy spine (a familia is just a high
  necessitudo — no special construct).
- **Identity / label / surface-form** are three layers. A use site is a
  NAMED SLOT TYPED BY A RELATION-KIND — symmetric with proprietas
  (named slot typed by a value-kind). Labels are local; identities
  global; scriptions are spelling.
- **Defaulting rule**: when a slot's name IS a declared necessitudo's
  name (or scriptio), identity binds automatically. The common case
  stays one line; you write the citation only when the domain word
  diverges (emptor) or two same-kind slots must be distinguished
  (a_loco/ad_locum).
- **Frames: explicit grouping element** inside the act-genus
  (`<termini>` + `<terminus>`), ends labeled `munus=`. Roles are
  frame-ends, not genera ("genera nuda munera sunt", round 50).
- **Converses: one entity, two names** (`conversum=`). One identity,
  two direction-spellings; use sites pick the direction that reads
  naturally.
- **Home: hybrid.** Core module `natura/necessitudines.genera` holds
  families + general workhorses; domain models may declare
  domain-specific necessitudines as species of those (cross-model
  `sub=` already exists).
- **v1 posture: monitum + exemplars.** Undeclared slot names warn
  (aggregated); vulnus only for citations that fail to resolve or
  break end-types. Corpus converges module by module; the gate
  graduates later.
- Ergonomics is a first-class constraint throughout: richness at the
  declaration (once), lightness at the use site (many).

## 3. The model

### 3.1 Necessitudo (declared relation kind)

New element, sibling of `<genus>` in a model:

```xml
<necessitudo nomen="continet" conversum="continetur_in"
    a="*" ad="*" sub="pars_totum">
  <definitio>Totum quod partem intra se habet ...</definitio>
  <glossa lingua="en">Containment: the whole holds the part.</glossa>
</necessitudo>
```

- `nomen=` — the primary direction's name. Shares the house name
  convention with genera (quaere answers for both; rule-15 uniqueness
  within the model).
- `conversum=` — the converse direction's name (optional; symmetric
  relations omit it). ONE identity: a use site named by either
  direction binds to this declaration, direction recorded by which
  name it used.
- `a=` / `ad=` — end constraints: what kinds may stand at each end
  (`a` = bearer end, `ad` = target end; `*` = open). Citations of
  entity genera — the residual, lawful coupling.
- `sub=` — family/parent necessitudo, inheritance as for genera.
  End constraints may NARROW down the chain, never widen (same law as
  relation-narrowing today).
- `<scriptio>` children — surface spellings that bind to this
  declaration (grammatical agreement variants):

```xml
<necessitudo nomen="situm_in" a="*" ad="locus" sub="locatio">
  <scriptio>sita_in</scriptio>
  <scriptio>situs_in</scriptio>
</necessitudo>
```

- Admits `<nota>` like every kind-tag. No machina/partes/proprietates
  in v1 (relations don't get IDs — their relata do; a necessitudo is
  a KIND, addressable by name like any genus).

### 3.2 Use sites (existing `<relatio>`, extended)

```xml
<relatio nomen="pars_de" ad="organizatio"/>            <!-- binds by name -->
<relatio nomen="dependentia" ad="*"/>                  <!-- ad-hoc at family level -->
<relatio nomen="fiduciarius" necessitudo="possessio_iuridica"
    ad="persona"/>                                     <!-- domain label, cited identity -->
```

- New attribute `necessitudo=` — explicit identity citation.
- Defaulting: absent `necessitudo=`, if `nomen=` matches a declared
  necessitudo's nomen/conversum/scriptio, it binds; otherwise the slot
  is UNBOUND (legal, monitum-counted).
- `a=`/`inversa=` on use sites remain legal during convergence;
  migrated sites should state the converse-direction name instead.
  This retires the boolean/name conflation without a flag day.
- Site-level `ad=` must lie WITHIN the declaration's end constraint
  (narrowing allowed, violation = vulnus).

### 3.3 Termini (frames)

Inside an act-genus, an explicit frame:

```xml
<genus nomen="venditio">
  <definitio>… Venditio res non est sed ACCIDIT …</definitio>
  <termini>
    <terminus munus="emptor"     necessitudo="participatio" ad="persona" necessaria="verum"/>
    <terminus munus="venditor"   necessitudo="participatio" ad="persona" necessaria="verum"/>
    <terminus munus="res_vendita" necessitudo="obiectum_actus" ad="*"/>
    <terminus munus="solutum"    necessitudo="obiectum_actus" ad="pecunia"/>
  </termini>
</genus>
```

- `munus=` — the end's label, REQUIRED, unique within the frame.
  Munus labels are frame-LOCAL: they do not enter the global name
  convention (emptor collides with nothing outside venditio).
- `necessitudo=` — identity citation, same defaulting rule as slots.
- `ad=`, `necessaria=`, `multiplex=`, `<nota>` as on relatio.
- A `<terminus>` IS a relation slot for every consumer: apparatus
  lists the frame as the act's checklist ("a venditio — who bought?
  who sold? what? for how much?"); the canon projection treats it
  exactly as it treats `<relatio>` (§6 gate).
- One `<termini>` per genus; inheritance: a sub-act inherits the
  parent's frame and may ADD termini or NARROW inherited ends
  (restated by munus), never remove.

## 4. What this resolves by construction

- **inversa wound (01KZC7EFMR)**: the two standing corpus vulnera
  migrate to conversum-declared necessitudines; `inversa=`/`a=`
  become legacy spellings on unconverged sites.
- **munus (gap 2)**: a role is a named end of a frame. One entity may
  stand at many ends (cliens AND servus); roles were never genera.
- **Morphology**: scriptiones collapse spelling variants into one
  identity; correct Latin agreement stops splitting the corpus.
- **Synonyms**: quaere answers "is there already a necessitudo for
  X?" before minting (same OCCUPATUM/UMBRATUM/LIBERUM discipline).

## 5. v1 scope

1. **Format**: `<necessitudo>` (+scriptio), `<termini>`/`<terminus>`,
   `necessitudo=` on relatio — natura.canon vocabulary + loader
   (lib/natura.c) structs/indexing/binding + resolution across models
   (two-phase legere→nectere as today).
2. **Core module** `natura/necessitudines.genera`: the family spine
   (~17 declarations from the classification, names subject to normal
   collision checks) + the multi-use workhorses (de, continet,
   asseritur_per, parit, auctor, sequitur, versio_de, normata_per,
   orta_ex, edita_a, pars_de, pendet_ex — dependet_ex becomes a
   scriptio or dies as a synonym, ruled at migration).
3. **Exemplar migrations** (proof of each mechanism):
   - commercium: venditio gains its frame (frames)
   - hortus/viventia: pollinat/pollinatur_a (conversum)
   - the census spine: causat, pendet_ex, tendit_ad, minuitur_per,
     natum_ex declared (rhubarb.census becomes a BOUND document)
   - one scriptio case: situm_in family
4. **Gates** (natura_examen + loader):
   - vulnus: `necessitudo=` citation that doesn't resolve; site
     `ad=` outside declared end constraint; duplicate munus within a
     frame; sub= widening an end constraint.
   - monitum (AGGREGATED — one line with count + top names; full
     list behind a tool flag, not 272 lines of noise): slot names
     that bind to no declaration.
   - exit conventions as house law (2 = nothing ran).
5. **Tools**: `bin/natura quaere` answers for necessitudines
   (occupatum/umbratum/liberum incl. scriptiones + conversa);
   `natura_quaere.sh -a` / apparatus shows frames as checklists and
   bound identities on slots; INDEX.md gains a necessitudines
   section (generated).
6. **Docs**: METAMODULUS gains the necessitudo/termini contract
   (loader-rule table extended); worklog carries the classification
   findings.

## 6. Regression gates

- **Canon projection byte-gate**: for UNMIGRATED modules, generated
  canons must be byte-identical before/after the loader change (the
  two-chains precedent). For migrated modules, `<terminus>` must
  project exactly as the `<relatio>` lines it replaced — byte-identical
  canons there too. This is the proof the apparatus chain didn't move.
- Suite: probatio_natura sections for binding (name/conversum/
  scriptio/explicit citation), end-narrowing, frame inheritance,
  munus uniqueness, monitum aggregation; each gate proven by a
  planted fault (red first).
- `bin/natura census` / `relationes` extended to report bound vs
  unbound counts — the convergence metric for the monitum era.

## 7. Non-goals (v1)

- NO corpus-wide migration (272 singletons converge module by module;
  the monitum count is the burn-down metric).
- NO renaming of the `<relatio>` use-site element.
- NO n-ary machinery beyond frames (the act-genus IS the reification;
  Davidson's event variable, scholastic termini actūs).
- NO expression language (house law, standing).
- NO change to generated-canon SHAPE (referentia stay as they are);
  enriched projection (e.g. frame-aware document checking) is a
  future arc.
- Properties typed by genera (the 5 orphan property-types finding —
  "a typed reference is a relation") are NOT touched; that migration
  becomes cheap after this arc but is its own decision.

## 8. Open questions for review

1. **Element name**: `necessitudo` (bond — Cicero) vs `habitudo`
   (standing-toward — the scholastic technical term). Spec assumes
   necessitudo; both LIBERUM, swap is one rename before build.
2. **dependet_ex vs pendet_ex**: scriptio of one declaration, or kill
   one name at migration?
3. Does the frame wrapper admit frame-level attributes later
   (e.g. a frame-wide necessitudo default)? v1 says no (YAGNI).
4. Should `bin/natura relationes` learn a `-familiae` view (grouped by
   bound family) in v1 or wait for the burn-down to need it?

## 9. Rulings at build (2026-08-10, arc shipped e49a61b..7da1869)

- **Declarations-only model** (measured blocker): canon's
  `minimum="1"` on root genus DROPPED; loader rule XXV enforces "at
  least one genus OR necessitudo" — co-occurrence above the canon,
  the "ad AUT a" precedent. Fran's ruling.
- **necessitudo** stands as the element name (8.1 closed by review).
- **dependet_ex = scriptio of pendet_ex** (8.2 closed — identical
  sense, one identity). 8.3 and 8.4 remain deferred, YAGNI.
- **necessaria= on terminus is apparatus-only** — the projection
  ignores it by standing doctrine ("necessitas ontologica obligatio
  documenti non est"). Stated in the canon nota and METAMODULUS §8.
- **Frame migration ordering rule**: `<termini>` must occupy the
  document position of the `<relationes>` it replaces and munus=
  must equal the old nomen=, or generated canons move (member order
  = container document order). Proven: venditio byte-identical.
- **Monitum XX is per-model** (modulus= always set — a NIHIL-module
  diagnostic would print on every `-plagula` run) and **silent while
  zero necessitudines are declared** (a metric without a norm
  measures nothing). Slots with a broken explicit citation count as
  XXI only, never double as unbound.
- **Family name collision measured**: `promulgatio` taken
  (/actus/promulgatio) → family named `evulgatio`. 88 names checked,
  1 collision — the collision praeceptum paid.
- **Genus-less modules cook no canon**: the driver skips them (the
  generator's refusal to write an empty canon stands); the canones
  reconciliation invariant counts COOKING modules + 1.
- **Numbers at ship**: 64+1 declarations; 432 slots → 163 LIGATA
  (38%) by name alone, 269 soluta (the burn-down baseline);
  VULNERA 2 → 0 (corpus SANUM, first time); 4 true citations born
  in the monolith (pollinat + heirs).
