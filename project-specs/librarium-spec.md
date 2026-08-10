# Librarium — natura ut bibliotheca unificata omnium .census

Spec for the cross-document individua arc: natura's kinds AND
individua become one library addressable by any `.census` file —
globally unique entities, canon/examen-checked addressing, and the
`<% &y;>` augmentation. Executes the parked design (parcum
**01KZPA16E4**, consensed 2026-08-10; stml-visio.md §5) — trigger
fired by Fran's architectural declaration 2026-08-10.

Build order (Fran-approved): **W1 (embedded export table) → W3
(augmentation) → W2 (collision tripwire)** — nothing spends time in
an unlawful state: the tripwire lands only after the census stub's
one real claim has a lawful home.

Grounded by explorer maps 2026-08-10 (stml dispatch; emission +
generated-canon + registry). All anchors verified.

---

## §0 Current state (measured)

- **Citation scope is document-internal.** Keys are collected from
  the judged document's identitas attributes; the library's
  individua exist as DATA in semina.census and are never consulted
  (individua.canon's own praefatio says so). Hence the
  rhubarb.census:62 stub: `<lingua-programmandi nomen="&c;">` whose
  only content is a nota confessing "MESOFACTUM: duplicatum lexici
  naturae — citatio trans documenta nondum exsistit."
- **Registry**: ALL `*.census` dispatch to natura/cocta/
  individua.canon by extension (canones.registrum:29; the
  `<individua>` root key is deliberately forbidden — RADIX VINCIT
  would misfire on per-module canons). So W1 lands for every census
  automatically, semina.census included (see §3 exemption).
- **The export set already exists as an enumeration**: the semina
  loop (natura_canones_emissio.c:1642-1655, filter
  `_nodus_individuum_est(e->nodus) && e->genus` at :1648) IS the
  114-key set; keys are emitted `&<titulus>;` — sigiled, VERBATIM
  snake (:1408-1412). Key comparison in the citation machinery is
  verbatim-with-sigil (signa migration ruling) — the embedded block
  therefore carries the SAME sigiled verbatim form.
- **`<%` is a loud error today** — MEASURED (canon_examen: "parsari
  nequit, gradus I"): unclaimed sigil (zero literal `%` in
  lib/stml.c), same path as the asserted `<@x>` test. The dispatch
  slot at lib/stml.c:1197-1201 is clean.
- **The collision set is ALREADY non-empty — 2 live keys**
  (measured: semina mints 114, rhubarb.census mints 51,
  intersection 2): `&c;` (same genus both sides — the known stub)
  and **`&aedilis;`** (rhubarb: instrumentum-lineae with `adhibet
  ad="&silva;"` + nota; semina: instrumentum-aedificationis with
  `invocat ad="&clang;"` — SAME REFERENT, DIFFERENT GENUS). Both
  must be resolved as part of the arc, not discovered by the gate;
  W2's tripwire turns rhubarb.census red the day it ships
  otherwise. Baseline note: all three censuses judge VITIA 0 today,
  and rhubarb.census is fully self-contained (mints all 30 keys it
  cites) — the self-containment W1 exists to relax.
- **Key symmetry CONFIRMED at the seams** (insert :1905, lookup
  :2003): keys stored and compared verbatim WITH sigil and
  semicolon (`&c;` byte-identical both sides). The embedded list
  stores the full sigiled form. (Stale `'#'` wording in the comment
  at lib/canon.c:2006-2009 — fix in passing.)
- **Single scope in practice**: zero of the 1163 generated
  citationes carry intra=, so every .census judgment runs one
  document-wide scope whose claves table is built at
  lib/canon.c:1867 — THE W1 SEAM: seed that table with the external
  keys before passus I, marking seeded entries with a sentinel
  StmlNodus* so W2 can distinguish external (citable-only) from
  document-minted. W2's mirror sits in the unicitas loop at :1820.

## §1 W1 — the embedded export table (claves externae)

**Shape** (tail of individua.canon, slot between `_citationes_scribere`
:1887 and the `</canon>` fputs :1888):

```xml
<claves-externae fons="natura">
  <clavis genus="canis">&laika;</clavis>
  <clavis genus="lingua-programmandi">&c;</clavis>
  … 114 …
</claves-externae>
```

- Text = the sigiled verbatim key exactly as citation comparison
  uses it. `genus=` = the KEBAB kind-element name in the same canon
  — this is what makes W3 possible (the augmentation judgment finds
  the target's kind element by this attribute) and what makes the
  block a true mini-manifest (W5's stamping adds `versio=` beside
  `fons=` later, no restructuring).
- **Emission**: new `_claves_externas_scribere(f, bib, entia)`
  reusing the semina filter/enumeration; `_canonem_emittere`
  (:1804-1810) gains the param — individua are currently FILTERED
  OUT of `elementa` upstream (natura_canones.c:1674-1678), so the
  caller passes `entia` through. Monolith call site only
  (:1693-1694); per-module canons unaffected.
- **canon.canon**: root content model (canon.canon:40-47) gains
  `<liberum nomen="claves-externae"/>`; element declarations for
  `claves-externae` (attr fons=; liberum clavis 1..*) and `clavis`
  (attr genus=; textus) modeled on the unicitas/citatio blocks
  (:136-148, :152+). Self-hosting keeps judging it.
- **lib/canon.c**: TRAP — the top-level dispatch loop (:1011)
  SILENTLY SKIPS children lacking `nomen=` (:1017-1021). The
  claves-externae branch must be handled explicitly (alongside
  "citatio" :1253 / "unicitas" :1372), never left to fall through
  that guard. Canon struct gains the external key table
  (TabulaDispersa key→genus); the citation-resolution path checks
  document keys first, then external keys; vitium text unchanged
  ("clavis absens in scopo") when neither has it.
- **A sentence to retire**: canon.canon:153-155's citatio nota
  says "Trans plagulas = gradus III, oneratoris manet" — W1 IS
  gradus III arriving through the canon itself; update the nota in
  the same edit.
- **Self-hosting battery fires on ANY canon.canon edit**
  (probatio_canon L1097-1100 self-judgment expecting exactly 0
  vitia + four sibling-canon assertions): the new liberum under
  the root AND both element declarations land in one
  self-consistent edit or the suite reddens there before anything
  else runs.
- **Gates (planted-fault)**: scratch census citing `&laika;` with
  no local declaration → RED today ("clavis absens"), GREEN after;
  citing `&nemo_est;` stays a vitium (negative pin); a canon with
  the block REMOVED must fail the same citation (proves the block
  is actually read — the silent-drop trap demands this);
  `-probare` regeneration byte-gate covers the block from birth;
  generata-custos head-3 signum contract untouched (praefatio
  ordering preserved).
- **Payoff gate**: rhubarb.census's `scripta-in="&c;"` citations
  resolve with the stub still present (stub becomes REDUNDANT in
  W1, unlawful only at W2).

## §2 W3 — augmentation `<% &y;>`

**Grammar** (stml layer):

- `<% &y;> … </%>` — target citation after the sigil, no verb name
  (one lawful verb ⇒ the sigil carries it). Whitespace after `<%`
  permitted; the target is a single `&…;` key.
- Lexer: new dispatch arm at lib/stml.c:1197-1201;
  `STML_TOKEN_PERCENTUM = XVIII` (+ close handling for `</%>`).
- Node: discriminator field beside `fragmentum` (include/
  stml.h:150) — the `fragmentum` precedent, NOT a new node genus
  (every `genus == STML_NODUS_ELEMENTUM` consumer stays
  untouched). Target key stored in `titulus` or a dedicated
  chorda* — plan decides; MUST also be initialized in
  `_parser_creare_nodus` (:1254-1266 currently leaves
  fragmentum/fragmentum_id uninitialized — do not copy that bug).
- Writer: new arm near :3932 emitting `<% ` + key + `>`, close
  `</%>` near :4033 — byte-exact round trip gated.
- Strictum/negative pins: `<%>` (no target), `<% laika>` (bare,
  unsigiled), `<% .canis>` (kind not individuum) = vitia.

**Judgment** (canon layer — this is where additive-only becomes
mechanical):

1. Target key must exist in scope (external ∪ internal) — vitium
   if not ("augmentation of the nonexistent").
2. The target's kind element is found via the claves-externae
   `genus=` attribute; the overlay's children are judged against
   THAT element's content model — no new vocabulary machinery.
3. **Additive-only = the LIBERUM/ATTRIBUTUM split the generator
   already computes** (natura_canones.c:1298-1300: multiplex →
   NC_MEMBRUM_LIBERUM → `<liberum>` without `maximum="1"`).
   Overlay children matching the kind's unbounded `<liberum>`
   members (+ universal nota/historia) are lawful; anything that
   exists as an `<attributum>` (single-valued) on the kind =
   conflict-vitium; unknown members = ordinary canon vitium.
   Attribute augmentation on `<%` itself: none (only the target
   key) — attributes are single-valued by nature.
4. Provenance: overlay children carry `fons=`/`certitudo=` as any
   census content does; nothing new.

**Migration**: rhubarb.census stub becomes

```xml
<% &c;>
  <nota>C89 stricte (-pedantic).</nota>
</%>
```

**Out of W3's scope**: the fold VIEW (base + overlays inspector) —
read-side projection, trails separately; validation never needs it.

## §3 W2 — the collision tripwire

- New vitium `CANON_CLAVIS_COLLISA = XV` (the enum currently ends
  at CANON_VALOR_EXTRA_FINES = XIV): a document MINTS an identitas
  key that exists in claves-externae → collision ("suppositum
  aequivocum esse nequit"). Sits beside the existing
  within-document unicitas machinery (NOMEN_BIS); one lookup in
  the W1 table. (The probatio banner "classes vitiorum omnes
  decem" is already stale at fourteen — renumber it in this arc.)
- **The semina exemption** (discovered in mapping): semina.census
  is itself a `.census` judged by individua.canon, and its keys
  ARE the external set — the library's own export must not collide
  with itself. RULE: the generated semina root carries
  `fons="natura"` (one attribute added at emission :1639, admitted
  in the generated canon's root element); the collision check
  skips documents whose root `fons=` equals the claves-externae
  `fons=`. Self-describing, tamper-evident (semina is
  custos-guarded generated output). Authored documents never carry
  it honestly — and dishonestly carrying it is a hand-edit of a
  signed generated file, which generata-custos already treats as
  the crime it is.
- **Planted faults**: rhubarb.census's TWO measured collisions ARE
  the faults — the tripwire must scream at both before their
  migrations, and go green after. `&c;`: stub deleted, nota becomes
  the `<% &c;>` augmentation. `&aedilis;`: per Fran's ruling on the
  genus disagreement (library-wins + augmentation / etiam= dual
  membership / census-wins) — the census's `adhibet ad="&silva;"`
  and nota move into `<% &aedilis;>` either way. End-to-end tests
  land in probatio_natura_canones.c (the .census-loading suite);
  unit tests in probatio_canon.c (:981 citation family slot, :1036
  collision slot).

## §4 Sequencing + gates summary

1. W1: emission + canon.canon + scope union. Battery: canon +
   natura suites, -probare, scratch-census planted faults, cold
   examen. Regenerate world.
2. W3: stml lexer/writer (+ amalgam + silva suite) → canon
   judgment → migrate rhubarb.census stub → suites + byte
   round-trip gates.
3. W2: collision vitium + semina fons= + tripwire proof (RED on
   pre-migration census re-creation planted in a fixture, GREEN on
   corpus) → full battery.

## §5 Out of scope (named)

- W4 world declaration (explicit multi-library imports +
  versions) — the registry's `.census` row IS the implicit world
  until a second library exists.
- W5 librarium stamping/attestation (versio= + stamp in the
  claves-externae block; vendoring posture).
- Fold-view inspector (apparatus-shaped projection of base +
  augmentations with per-claim provenance).
- Per-document "which externals do I cite" REPORT (cheap visus
  addition; derived, never authored).

## §6 Rulings (Fran, 2026-08-10, post-mapping)

1. **Order W1→W3→W2 stands** (scout's W1+W2-first considered and
   declined): migrations happen once, lawfully, finally — no
   relations parked in comments during an interim. Collision
   protection arrives last; acceptable while the corpus has one
   author.
2. **&aedilis;: library wins.** instrumentum-aedificationis stands
   (essence over operation — Pieper); the census's `adhibet
   ad="&silva;"` + nota become the `<% &aedilis;>` augmentation at
   W3's migration. CLI-ness is invocation, not classification.

## §7 W1 build record + W3 pickup notes (2026-08-10, 591a6ea)

**W1 IMPLETUM.** All §1 gates ran: &laika; probe RED→GREEN measured;
&nemo_est; negative held; block-stripped canon broke the citation
(block provably read); five-file world VITIA 0; both -probare 0;
inscription doctrine retired openly (DOC_NON_INSCRIPTUS now uses a
truly-unknown key; DOC_CLAVIS_MUNDI pins the librarium behavior).
Deviation from §0's seed-at-:1867 sketch: lookup FALLBACK to one
canon-held table instead of seeding per-citation scope tables
(114×1163 inserts avoided; W2 reads the same table).

Repeatable block-removal gate:
`grep -v '<clavis |claves-externae' natura/cocta/individua.canon >
/tmp/sine.canon && bin/canon_examen -canon /tmp/sine.canon <census>`
— must produce the clavis-absens vitium.

**W3 pickup (fresh-session checklist)** — spec §2 has the design;
these are the map details not repeated there:
- stml probatio slot: after the "genus titulus" section closing
  brace (probatio_canon.c:~1414 pre-W1; find the section by its
  banner). probatio_canon's piscina is 262144 — large fixture sets
  may need it raised. probatio_stml round-trip idiom: :2128-2134.
- Parser dispatch case slot: `_parser_legere_nodus` beside the
  ERRATUM arm; `_parser_creare_nodus` leaves fragmentum/
  fragmentum_id UNINITIALIZED — initialize the new discriminator,
  do not copy that bug.
- W3 canon-judgment consumes canon->claves_externae (key →
  CanonClavisExterna{clavis, genus}) — the genus attr finds the
  target's kind element; additive-only = the member exists as an
  unbounded `<liberum>` on that element (single-valued members are
  `<attributum>` — mechanical conflict test).
- Migration order inside W3: grammar green first (stml suite +
  amalgam + silva), then canon judgment, then rhubarb.census
  migrations (&c; stub → `<% &c;>` nota; &aedilis; entry →
  `<% &aedilis;>` with adhibet+nota, library genus stands per §6).
- After W3, W2 is small: vitium XV beside NOMEN_BIS (:1820 mirror),
  semina root fons= exemption, tripwire proof on the two measured
  collisions.
