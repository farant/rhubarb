# Census Implementation Plan (individua seminata + .census)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement `project-specs/census-spec.md`: individuum entries stop
being tags, the projection emits `natura/cocta/semina.census`, `.census`
replaces the per-module document extensions.

**Architecture:** Generator + registry only; lib/canon.c untouched. Three
green tasks: (1) registry + hortus migration (standalone — enrollment-by-
kind already resolves under current closures); (2) individuum exclusion
bundle (generator + regen + fixtures); (3) semina.census emission + the
self-judging gate. Then records.

**Tech Stack:** C89 latina, STML, credo, house build scripts.

## Global Constraints

Same as canon-referentia-plan.md (C89 strict flags, Latin, i32/i64
UNSIGNED, chorda not NUL-terminated, exit 2 = nothing ran, pre-fix
oracles, never stage FAQ.md / gesta/annales/*, commits end with
`Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`, cold
`./silva/examen.sh` is the authority over racing hooks, test pattern is
`./compile_tests.sh canon|natura|planta_lectio` — never `probatio_*`).

**Key discovered facts** (verified 2026-08-08):
- Individuum detection: `ens->nodus->titulus == "individuum"` (NcEns and
  NaturaRes both carry the source `StmlNodus* nodus`).
- 112 dictionary individua across 15 modules; carl_linnaeus carries
  `<valor nomen="nomina|natus|mortuus">`, a `<historia>` of `<eventum
  quando= actio=>text</eventum>`, and a curatorial `<nota>` (nota is
  SKIPPED in semina — it is about the entry, not the individual).
- Entity loops that build canon elementa: `_canonem_totum_scribere`
  (~1573) and `_canonem_modulo_scribere` (~1367) — exclusion lands
  there, NOT in `_entia_colligere` (the -inspicere path keeps seeing
  individua).
- Relations whose relatio-level `ad=` names an individuum directly
  (e.g. planta `auctor_nominis ad="carl_linnaeus"`) fall back to the
  KIND's closure (`r->genus_suum`) — weaker (any enrolled persona
  key accepted) but sound; "specifically Linnaeus" needs canon growth
  we refuse.

---

### Task 1: `.census` registry + hortus migration (standalone green)

**Files:** `canones.registrum`, `natura/exempla/hortus.planta` →
(git mv) `natura/exempla/hortus.census`, `probationes/probatio_planta_lectio.c`
(path string ~line 11 comment + ~line 75 `filum_legere_totum`).

- [ ] Registry: delete the `.planta` and `.individua` key lines; add
  `.census	natura/cocta/individua.canon`; rewrite the individua-keys
  comment block: extensio una `.census` documenta individua omnia fert;
  canones moduli PROFILI sunt, per `canon_examen -canon <via>` adibiles;
  historia clavium retiratarum in git manet.
- [ ] `git mv natura/exempla/hortus.planta natura/exempla/hortus.census`;
  add the enrollment line as the FIRST child of `<individua>`:
  `<persona nomen="#carl_linnaeus"/>`; rewrite the header paragraph that
  documents the class-D wart — it DIES: under `.census` the document
  routes to the monolith, the enrollment seats Linnaeus, and
  `auctor-nominis="#carl_linnaeus"` RESOLVES.
- [ ] probatio_planta_lectio: path → `hortus.census`. RISK CHECK: the
  generated planta reader meets the foreign `<persona>` element — the
  lector is benignus (unknown elements skipped), but if any assertion
  counts children of `<individua>`, adjust the expectation, not the
  document.
- [ ] Gates: `bin/canon_examen natura/exempla/hortus.census` → plagulae 1
  / VITIA 0 (the auctor citation now RESOLVING is the point);
  `./compile_tests.sh planta_lectio` green; hook fires on the .census
  write (registry-driven — no hook edit).
- [ ] Commit: `canon: extensio .census una - hortus migratus, verruca
  classis D moritur`.

### Task 2: individuum exclusion (generator bundle, ONE commit)

**Files:** `tools/natura_canones.c`, `tools/natura_canones_emissio.c`
(praefatio sentence only — via NC_PRAEFATIO_FORMA in natura_canones.c),
`probationes/probatio_natura_canones.c`, regenerated `natura/cocta/*`.

- [ ] Helper (natura_canones.c, near `_titulum_semel_addere`, + prototype):

```c
/* an nodus fontis individuum sit - individuum tags non gignit
 * (spec census par. 1: praedicabile non est) */
interior b32
_nodus_individuum_est(
    constans StmlNodus*  n)
{
    redde (b32)(n && n->titulus &&
                chorda_aequalis_literis(*n->titulus, "individuum"));
}
```

- [ ] Exclusion in BOTH canon writers' entity loops (after the modulus
  filter in `_canonem_modulo_scribere`, at loop top in
  `_canonem_totum_scribere`):

```c
        si (_nodus_individuum_est(e->nodus))
        {
            perge;   /* individua data sunt, non grammatica -
                      * semina.census ea fert (spec par. 2) */
        }
```

- [ ] `_clausuram_colligere`: in the `res_suae` loop and the `etiam=`
  pass, before `_titulum_semel_addere`, skip when
  `_nodus_individuum_est(r->nodus)` (a cast may be needed:
  the helper takes `constans StmlNodus*`).
- [ ] `_petitum_citabile` res branch: when the resolved res is an
  individuum, fall back to the kind:

```c
            si (_nodus_individuum_est(r->nodus))
            {
                *clausura_ex = _clausuram_colligere(bib,
                    r->genus_suum, modulus_requisitus, piscina);
                si (!*clausura_ex)
                {
                    redde NC_CIT_ALIENA;
                }
                redde NC_CIT_FIT;
            }
```

  (the existing modulus check + single-title path stays for
  species/cultivar res).
- [ ] Praefatio (NC_PRAEFATIO_FORMA, after the Signa block, no new %u):

```c
"  INDIVIDUA dictionarii elementa NON sunt (praedicabile non est):\n" \
"  semina.census ea ut DATA fert; inscriptio per tag generis fit.\n" \
```

- [ ] Fixtures (probatio_natura_canones.c): DOC_INSCRIPTUS and
  DOC_NON_INSCRIPTUS swap `<carl-linnaeus nomen="#carl_linnaeus"/>` for
  `<persona nomen="#carl_linnaeus"/>` (comment: inscriptio per tag
  GENERIS — individuum tags mortua). New section XI after X:

```c
    /* ========================================================
     * XI. INDIVIDUA NON SUNT TAGS (spec census par. 1)
     * ======================================================== */

    {
        i32 ignota;
        i32 omnia;
        chorda cl;

        imprimere("\n--- XI. individua non tags ---\n");

        cl = chorda_ex_literis("carl-linnaeus", piscina);
        CREDO_NIHIL (_elementum_invenire(monolithus_radix, cl));

        /* impostoris foramen mortuum: tag individuI ignotum est */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <carl-linnaeus nomen=\"#impostor\"/>\n"
            "</individua>\n",
            CANON_ELEMENTUM_IGNOTUM, &ignota, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (ignota, (i32)I);
    }
```

  (`_documentum_iudicare` takes the literae directly — verify its
  signature accepts a literal; if it takes `constans character*`, this
  is fine.) ORACLES: both assertions fail today (element exists; tag is
  legal).
- [ ] Regenerate (`./tools/natura_struere.sh && ./tools/natura_canones.sh`),
  expect: element counts drop, citations recount (persona closures lose
  14 individua titles; auctor-nominis citations now
  `ad="persona <species...>/nomen"`), `-probare` chain to 0. Run
  `canon`, `natura`, `planta_lectio` suites — ALL green (hortus.census
  from Task 1 must still resolve: enrollment is by persona tag, whose
  keys the thinned closure still collects).
- [ ] Commit: `natura: individua tags non sunt - clausurae ad taxonomiam,
  foramen impostoris mortuum`.

### Task 3: semina.census emission + self-judging gate (ONE commit)

**Files:** `tools/natura_canones_emissio.c` (new writer + header
prototype in `tools/natura_canones.h`), `tools/natura_canones.c`
(driver call site — find where `_canonem_totum_scribere` is invoked in
`principale` and emit semina beside it, same output directory),
`probationes/probatio_natura_canones.c`, generated
`natura/cocta/semina.census`.

- [ ] Writer contract (natura_canones.h):

```c
/* censum seminum scribere: individua dictionarii ut DATA - instantiae
 * tagorum generum suorum. Valores NODI PROPRII soli (hereditata per
 * ordinarius= tagi veniunt); relationes signum ferunt ('#' si petitum
 * individuum, '.' si genus); historia subarbor transcribitur (textus
 * normalizatus); nota curatorialis OMITTITUR. Redde FALSUM ut
 * _canonem_emittere: nihil scriptum valet. */
b32
_censum_seminum_scribere(
    NaturaBibliotheca*   bib,
    Xar*                 entia,
    constans character*  via,
    Piscina*             piscina);
```

- [ ] Implementation shape (emissio.c): open via, fputs the GENERATUM
  signum comment (head-3 window!) + `<individua>` root; loop entia,
  skip non-individua (`!_nodus_individuum_est` — export the Task 2
  helper via natura_canones.h or duplicate locally with a comment);
  per individuum: `_kebab_scribere` the KIND tag
  (`ens->genus->titulus`), `nomen="#"` + the natura name VERBATIM
  (snake — legal identitas body); iterate the ens's OWN node children:
  `<valor nomen=X>text</valor>` — map X against the folded membra of
  `_elementum_aedificare(bib, ens, VERUM, NIHIL, piscina)`:
  attribute-form member → attribute (NC_ORIGO_RELATIO values get the
  signum by resolving the value against bib: `natura_ens` →
  individuum? `#` : `.`; note kind names also resolve via
  `natura_genus`); liberum-form member (nomina) → child element with
  the valor text; then the `<historia>` child subtree if present:
  `<eventum quando=".." actio="..">normalized text</eventum>`;
  `<nota>` skipped. Close tags, fclose judged (mos
  `_canonem_modulo_scribere`).
- [ ] Driver: in `principale`'s monolith branch, after the monolith
  write succeeds, call `_censum_seminum_scribere(bib, entia,
  "<dir>/semina.census", piscina)` (derive the path the same way
  individua.canon's is derived); failure = same abort path.
- [ ] Gate (probatio section XI extension or XII): after regen,
  `filum_legere_totum("natura/cocta/semina.census")` non-empty;
  contains `persona nomen="#carl_linnaeus"` (chorda_invenire_index);
  judged under the monolith → 0 vitia (the SELF-JUDGING gate — reuse
  `_documentum_iudicare` on the file's contents with any-genus count
  == 0 via omnia). ORACLE: file absent today.
- [ ] Regenerate + all suites + `bin/canon_examen
  natura/cocta/semina.census` (0 vitia). Commit: `natura: semina.census
  - dictionarium in censum proiectum, porta se-iudicans`.

### Task 4: acta

- [ ] Worklog entries (tools/natura_canones.worklog.md; lib/canon
  untouched so no canon worklog).
- [ ] Tabularium: `gerere` nota on decretum `01KZEMFMDF` — IMPLETUM with
  commits + measured numbers (element count delta, citation recount,
  semina size); status stays (decreta have no lifecycle to close).
- [ ] Memory: natura-project.md — replace the "NOT YET IMPLEMENTED" note
  with SHIPPED + numbers; MEMORY.md natura line gains "semina.census".
- [ ] Commit worklogs.

## Non-changes, with cause

- lib/canon.c, canon.canon, canon_coquere: untouched (spec par. 0) —
  enrollment and citation resolve through existing machinery.
- `_entia_colligere` keeps individua (inspicere still sees them).
- cocta.registrum: planta reader entry unchanged (its canon only
  thinned).
- Restriction test (section X) unchanged — DOC_PURUS uses species tags.
- semina liberum-content beyond nomina/historia: whatever `<valor>`
  liberum-form members exist are emitted as text children uniformly; no
  per-module special cases.
