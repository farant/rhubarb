# Arbor Porphyriana — implementation plan

Executes project-specs/arbor-porphyriana-spec.md. Grounded in three
Explore maps (explora-stml / explora-canonem / explora-oneratorem,
2026-08-10); every anchor below is from a verified read, not memory.

**Structure: rename first, unify second.** Tasks 1–3 are grammar
groundwork (each independently green). Task 4 is THE RENAME — every
element-name string flips to its dotted form across canon, corpus,
loader, tools, fixtures, with species still loading as res. Task 5 is
THE UNIFICATION — species/cultivar become kinds. Task 6 surfaces
determinations. Task 7 regenerates and reconciles. Task 8 records.
Separating rename-risk from semantics-risk gives each phase its own
clean gate.

## Global constraints

- C89, Latin identifiers, latina.h macros forbidden as identifiers;
  chorda NOT null-terminated (the quaesitor landmine below is exactly
  this); i32/i64/b32 unsigned.
- Suites: `./compile_tests.sh <substring>` (exit 2 = NOTHING RAN);
  natura binaries `./tools/natura_struere.sh`; canon binaries
  `./tools/canon_struere.sh`; cold `./silva/examen.sh` is the
  authority over warm hooks/legati.
- Planted-fault law: every gate proven RED before its green is
  believed.
- Hooks will fire mid-migration (natura-custos on .genera,
  canon-custos on .canon; visus staleness gate exits 2 after loader
  edits until struere) — that is the gates working; rebuild, don't
  bypass.
- Never commit Fran's staging files (FAQ.md, gesta/annales/*). Stage
  by explicit path.

## Decisions resolved during mapping (veto here if wrong)

1. **Selectors: dotted elements stay unaddressable.** lib/selectio.c
   reads leading `.` as class/label (selectio.c:496); no escape is
   added this arc. Registry note in stml-visio (alongside the
   `.ns::label` claimant at stml-visio.md:77). Trigger to revisit: the
   first real need to select/transclude a kind element.
2. **canon.canon blocker → new value-genus, not a looser
   COMPOSITUM.** elementum/@nomen and liberum/@nomen (canon.canon:55,
   :117) move to a new genus admitting one optional leading `.` +
   compositum body; attributum/@nomen (:79) STAYS compositum, so the
   canon grammar mirrors the lexer exactly (dotted tags, never dotted
   attributes).
3. **`gerens=` values stay undotted** (natura.canon:146,154-155
   species|individuum): they name projection LEVELS, not elements.
4. **Content models become identical** except sub=/modulus= (genus
   only — a .species derives both from position): .genus gains
   historia/certitudo/fons/etiam/nota=; .species/.cultivar gain the
   full kind set. cultivar admission stays under .species only.
5. **The scanner-invisibility of dotted tags** (silva_annotationes.c
   `_est_ancoratum` requires `<`+letter) is deliberate and recorded;
   no change.

---

## Task 1 — stml: fix the pre-existing empty-name segfault

Any tag name starting outside `[a-zA-Z_:]` produces an empty-chorda
name that is dereferenced downstream: `<9bad nomen="x"/>` SEGFAULTS
canon_examen today (measured, exit 139); `<.species>textus</.species>`
silently corrupts (successus=1, tree mangled via anonymous-close
swallowing). Fix independent of the dot feature and prerequisite to it.

- [ ] Failing tests in probationes/probatio_stml.c (round-trip section
      style, ~:631): `<9bad/>`, `<.x>y</.x>`, `<a><.b>x</.b></a>` must
      all yield `successus == FALSUM` with a status, not a crash and
      not a silent tree. Run `./compile_tests.sh stml` — expect crash/
      failure (RED measured).
- [ ] Guard the tag path: after `titulus = _tok_legere_nomen(ctx)` at
      lib/stml.c:586 (and the capture-name reads at :509, :539, and
      close-tag :561 — close tags with empty names must NOT fall into
      the `clausura_anonyma` arm at :1243-1250 unless the tag is
      literally `</>`), a zero-length name where a name is required →
      error token → parse fails with the existing malformed-tag
      status. Mirror of the attribute guard ten lines away
      (stml.c:287).
- [ ] `./compile_tests.sh stml` green; `./silva/amalgamare.sh`
      (regenerates silva/amalgama/silva.c through its 6 gates);
      silva fidelity suite green.
- [ ] Commit (stml crash fix stands on its own merits).

## Task 2 — stml: leading-dot tag names

- [ ] Failing tests first: parse + byte-exact round-trip of
      `<.species nomen="apis"><.b>x</.b></.species>` (non-pretty
      write, cmp equal — idiom at probatio_stml.c:2128-2134); dotted
      raw-content element closes correctly; NEGATIVE pins:
      `<a .attr="x"/>` still fails (attribute names never dotted),
      `<.>`/`</.>`/`<..x>` fail (dot must be followed by a name-start
      char, exactly one leading dot). RED measured.
- [ ] Implement in `_tok_legere_nomen` (lib/stml.c:144-168): accept
      one leading `.` IFF the next character satisfies
      `_est_nomen_initium`. Do NOT touch `_est_nomen_initium` itself —
      it is the attribute gate at stml.c:280; the four tag call sites
      (:509, :539, :561, :586) get dots through the shared reader, the
      attribute site (:286) stays gated. Writer paths (:3684, :3727,
      :3771, :3860, :3958), raw-close matchers (:963-975, :1049-1056)
      and tag dispatch (:1086-1145) are measured dot-transparent — no
      changes.
- [ ] lib/coloratio.c same rule at its tag-position name-starts
      (:895, :964; its private `_est_nomen_initium` at :22-27 gains
      dot ONLY via the tag paths, or add a tag-start helper).
- [ ] include/stml.h grammar doc block (:11-34) records the form.
- [ ] `./compile_tests.sh stml` green; `./silva/amalgamare.sh` +
      silva suite; commit.

## Task 3 — canon: value-genus for dotted element names

canon.canon types elementum/liberum `nomen=` as COMPOSITUM
(`[a-zA-Z0-9_*-]`, lib/canon.c:499-520) — `nomen=".genus"` is vitium
IV today (measured). natura.canon self-judges via canones.registrum:34.

- [ ] Failing test: scratch canon declaring
      `<elementum nomen=".probandum">` judged by canon.canon → vitium
      IV (RED = current behavior, measured); after the change → clean;
      AND `<attributum nomen=".x">` still vitium (attributes keep
      compositum).
- [ ] New CanonGenusValoris `CANON_GENUS_TITULUS_ELEMENTI` (enum in
      include/canon.h:62-82 family; `genus_legere` mapping at
      lib/canon.c:155; `valor_congruit` case = optional single leading
      `.` then the COMPOSITUM body). canon.canon:55 and :117 move to
      it; :79 stays compositum.
- [ ] probatio_canon section for the new genus (both polarities).
- [ ] `./tools/canon_struere.sh` (staleness gate lists lib/stml.c —
      it will already be tripping from Tasks 1–2);
      `./compile_tests.sh canon` green; commit.

## Task 4 — THE RENAME (no semantics change)

Everything that spells the three names flips to `.genus` / `.species`
/ `.cultivar` in one task; species still load as RES. Loud almost
everywhere (undeclared element = 2 vitia each); the silent sites are
listed and get explicit attention.

- [ ] Tripwire proof first: rename natura.canon only (blocks 63-87 →
      `.genus`, 255-272 → `.species`, 289-298 → `.cultivar`; liberum
      admissions :38, :77, :79, :267, :268, :269, :284; unicitas
      super= list :335-336 → `.genus .species individuum .cultivar
      necessitudo`) — then `bin/canon_examen natura/animal.genera`
      must SCREAM (ELEMENTUM_IGNOTUM on undotted corpus). That
      measured scream is the migration tripwire, same posture as the
      signa arc.
- [ ] Content-model unification while in the file (decision 4):
      .species/.cultivar gain glossa, proprietates, partes, actiones,
      relationes, termini, machina_statuum, individuum (all the
      genus children); .genus gains historia, certitudo=, fons=,
      etiam=, nota=. No sub=/modulus= on .species/.cultivar.
- [ ] Corpus flip: perl over natura/*.genera + probationes/exempla/
      *.genera (12 `<genus` tags in 8 files) + the inline fixtures in
      probationes/probatio_natura.c and the GLOSSA_* fixtures in
      probatio_canon.c (:536, :545, :554). Open AND close tags; tag-
      anchored patterns only (`<genus ` / `<genus>` / `</genus>` etc.)
      — `genus=` ATTRIBUTES everywhere must survive untouched, and
      silva/grammatica/grammatica.canon:137 declares an unrelated
      `genus` element that must NOT be touched.
- [ ] Code literal flip, same commit (species still → NaturaRes):
      - lib/natura.c:277, :309-311, :1059, :1077-1080
      - tools/natura_canones.c:836-838 (THE silent one: the
        `_valores_applicare` ancestor-walk gate — stale strings drop
        inherited valores from generated canons without a word)
      - tools/natura_canones.sh:203 driver grep `<.genus ` +its test
        mirror probatio_natura_canones.c:1748 (move together)
      - tools/natura_examen.c:438, :477-478, :517-519, :545, :566-568
        unchanged (relatio/terminus stay), :870, :918-922 unchanged,
        :1055-1056, :1084 — i.e. the "genus"/"species"/"individuum"/
        "cultivar" literals only
      - tools/natura_quaesitor.c strncmp sites :1110, :1133-1134,
        :1157-1158, :1811-1819 — dotted strings AND lengths
        (".species"=VIII, ".cultivar"=IX; "individuum" unchanged) —
        these compare non-terminated chorda data by prefix and fail
        SILENTLY if missed (census species→0 with no error)
- [ ] Gates: `./tools/natura_struere.sh`; `./tools/canon_struere.sh`;
      cold `bin/canon_examen` green over all 35 modules;
      `bin/natura_examen` summary UNCHANGED (`genera 188 / res 385 /
      necessitudines 65 / VULNERA 0`); `bin/natura census -machina`
      species=271 individua=113 cultivares=1 (proves the strncmp
      sites were hit); `./compile_tests.sh natura` + `canon` green;
      `./tools/natura_canones.sh` regenerate + `-probare`;
      generated planta.canon DIFF: inherited valores present
      (proves natura_canones.c:836-838 was hit). Commit.

## Task 5 — THE UNIFICATION (species/cultivar become kinds)

- [ ] Loader (lib/natura.c):
      - Route `.species`/`.cultivar` into the genus branch (:277-307):
        NaturaGenus, parens = ambiens kind, genera_omnia,
        NATURA_ENS_GENUS. `individuum` keeps the res branch alone.
        Nested kinds recurse with themselves as ambiens; keep passing
        ambiens_res so an `<individuum>` under a former species gets
        genus_suum = that kind and continens = NIHIL (correct new
        semantics; res_nidificatae becomes individua-in-individua
        only).
      - Port dual membership to kinds: NaturaGenus gains
        `parens_etiam` (include/natura.h:54-63; doctrine comment
        :65-79 updated); the etiam= nectere pass (:1486-1514) gains a
        genera walk; `pertinens_etiam` derivation at :1056-1057
        extended for kind context. One wild case:
        scriptum_conchae etiam="plagula_fontis".
      - Rule VI vocabulary (LANDMINE): :1681-1711 reads
        gradus_assensus's res_suae — now reads its sub-kinds
        (liberi). Miss this and all 26 certitudo= sites become
        rule-VI vulnera (loud, and the fixture pins it: rule VI
        count at probatio_natura.c:756).
      - Rule XXII arm :839-844 (`NATURA_ENS_RES` → containing genus)
        now resolves former species AS the kind — stricter and
        correct; re-measure which sites pass.
      - apparatus/maiores/liberi: no structural change — species
        enter genera_omnia, the reverse-index pass (:1651-1679)
        builds ~272 new liberi edges, natura_genus starts returning
        them.
- [ ] Consumers:
      - tools/natura_canones.c: `_entia_colligere` (:376-419) — former
        species arrive via genera_omnia (est_res=FALSUM); the est_res
        ancestor-walk arm in `_valores_applicare` (:825-844) goes
        dead — remove it, the genus-parens walk (:847-853) subsumes;
        `_clausuram_colligere` (:999-1041) walks liberi/sub-kinds
        where it walked res_suae, so citation ad= closures keep their
        272 entries; seeds still gated by `_nodus_individuum_est`
        (:907-917) — semina UNCHANGED.
      - tools/natura_quaesitor.c: former species hit walk 1
        (genera_omnia) — GENUS display arm must show `(sub parens)` so
        the mensuratio/fundamentum assertion keeps its meaning;
        census re-partition (quot species→kinds; keys stay, values
        move: genera 460, res 113); `_species_scribere`/
        `_nidificatas_scribere` iterate liberi for kind children +
        res_suae for individua.
      - tools/natura_examen.c: -tabulae/-corpus re-walk raw STML by
        NAME (they never see the graph): genera.txt now collects
        `.genus|.species|.cultivar`; res tables/S rows = individuum
        only; glossae denominator grows to 460 (definitio fallback
        already in place, :485-489).
      - tools/natura_visus.sh: rule-VI mirror :220 checks GENERA
        instead of RES; §IV Dictionarium and counts re-partition;
        umbra classifier :126-140 unaffected (relatio|terminus).
      - tools/natura_apparatus_umbra.sh: former species enter the G
        stream — output grows; differential gate against
        natura_apparatus still holds.
- [ ] Fixture flips in probationes/probatio_natura.c, each rewritten
      deliberately (the block at :401-410 is an explicit regression
      guard against exactly this change — invert it with a comment
      recording the intent): genera IX→XIV, res V→ZEPHYRUM (:282-285);
      rosa IS a genus (:306, :309-310); corona via parens_etiam
      (:348-357); granny/malus_d as parens chains (:383-431);
      vivens->liberi count (:467-468). probatio_natura_quaesitor.c:
      `RES\tiudicium` → `GENUS\tiudicium` (:265), isbn_10 display
      (:391-395), apparatus malus glyphs (:400-403).
- [ ] Gates: struere; `bin/natura_examen` = `genera 460 / res 113 /
      VULNERA 0` (the 0 proves rule VI survived); `bin/natura census
      -machina` reconciles (genera=188+271+1, res=113);
      `bin/natura quaere rosa` → GENUS; `bin/natura quaere apis` →
      GENUS (sub animal); suites natura + canon green. Commit.

## Task 6 — determinations surfaced

- [ ] Failing test: `bin/natura apparatus rosa` (or fixture kin) must
      show `durata = perennis (determinatum a rosa)`-style lines —
      assert on a fixture with a kind-level valor; RED first.
- [ ] `apparatui_contribuere` (lib/natura.c:1948-1961) harvests
      `<valor>`/`<relatum>` alongside the five containers (auctor
      already tracked in NaturaApparatusMembrum); `_membrum_scribere`
      (tools/natura_quaesitor.c:943-1078) gains `valor`/`relatum`
      arms: name, text/target, auctor. ~204 species-level + 16
      genus-level determinations become visible.
- [ ] Apparatus count assertions that move (probatio_natura.c:494-498)
      updated deliberately. Gates: suites green; umbra differential
      still green (umbra script ignores valor rows — confirm or
      extend). Commit.

## Task 7 — regeneration + world reconciliation

- [ ] `./tools/natura_canones.sh` — all 34 canons + semina.
      Gates: semina.census BYTE-IDENTICAL (113 seeds — individua
      filter untouched); `bin/natura_canones -index` = 573 rows with
      exactly 272 flipped res→genus in the last column;
      `./tools/natura_canones.sh -probare` exit 0 (not 3);
      `./tools/canon_coquere.sh -probare`; probatio_planta_lectio
      suite green (hortus.census still validates; bag layout
      unchanged — rosa-canina=1, granny-smith=1, stirps kinds=0).
- [ ] `./tools/natura_visus.sh` regenerates INDEX.md (counts:
      genera 460 · res dictionarii 113); `natura_glossae` run —
      coverage becomes 188-hand + 272-fallback of 460 (recorded, not
      gated).
- [ ] Cross-suite battery: `./compile_tests.sh natura`, `canon`,
      `stml`; cold `./silva/examen.sh` on every touched .c; full
      `bin/natura_examen` MONITA reviewed (proprietas-types that
      resolved to former species stop being monita — count the drop).
      Commit.

## Task 8 — records

- [ ] project-specs/stml-visio.md: registry — `.` element-position =
      kind-minting VIVIT (creation-site row beside the citation row
      at :75); reconcile the `.ns::label` claimant (:77); notes:
      selector-unaddressable (decision 1, with trigger), scanner-
      invisible (decision 5).
- [ ] natura/METAMODULUS.md: §3 element reference (.genus/.species/
      .cultivar, shared content model), the proprium doctrine
      promoted (proprietas opens · valor-on-kind closes ·
      valor-on-individuum reports), rule VI/XXII notes.
- [ ] Worklogs: lib/stml.worklog.md (crash fix + dot), natura
      worklog (unification arc, landmine ledger), canon worklog
      (value-genus).
- [ ] Spec §10 rulings appendix (anything decided mid-build);
      ledger: decretum 01KZPK4B7X impletum nota; memory:
      natura-project + canon-project lines (counts, dotted forms,
      determinations).
- [ ] Final commit; tree clean except Fran's staging files.
