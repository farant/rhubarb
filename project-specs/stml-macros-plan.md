# STML Macros v1 — Implementation Plan

> Spec: `stml-macros-spec.md` (2026-08-25, approved). Interview:
> `stml-macros-interview.md`. Execute inline on main per house
> convention; no subagents. Tasks are checkbox-tracked; each ends
> green with its own commit.

**Goal:** `stml_expandere` — parameterized-fragment expansion as a
pure tree→tree projection — plus one real arbor slice proving it.

**Architecture:** one new TU (`lib/stml_macros.c` +
`include/stml_macros.h`) against the public stml.h tree API
(`stml_duplicare` deep-clones; substitution is a walk over the
clone). One lexer touch in lib/stml.c (quote-aware `>>`). Side-table
provenance (sedes precedent). File keeps macro form; expansion at
load.

## Global constraints

- Real compile flags live in `compile_tests.sh` (adds `-std=c89
  -Wno-long-long -Wno-overlength-strings` over the CLAUDE.md list).
- Latin identifiers/comments/messages; EVERY latina.h #define is a
  reserved word (check before coining); `i32`/`i64` are UNSIGNED
  (s32/s64 for anything signed; loop bounds derived by subtraction
  need a guard — 911cdb9d); `chorda` is NOT null-terminated.
- Born-red first, every behavior. Suite exit contract: 2 = NOTHING
  RAN. Formator conformity on every touched file
  (`./silva/formator.sh <via>`; 3 standing divergences in lib/stml.c
  at 348/4134/5620 are noise — leave them).
- After lib/stml.c edits: `./silva/amalgamare.sh` + silva suite +
  `./tools/natura_struere.sh`. After new lib/probatio:
  `./tools/compile_tests_fontes_generare.sh`.
- credo pattern per probationes/probatio_piscina.c; test binaries
  land in /tmp/<name>.

---

## Task 1 — Quote-aware `>>` close-scan

**Files:** Modify `lib/stml.c` (`_tok_legere_transclusio`, the scan
loop at ~1222). Test: `probationes/probatio_stml.c` (transclusion
section near the existing `<<#frag1>>` roundtrip fixtures).

- [ ] **1.1 Failing test** — in the transclusion section add:

```c
/* Scan '>>' citationum-conscius (macros v1: valores argumentorum
 * '>>' continere possunt - antea lexema praemature secabatur) */
res = stml_legere_ex_literis(
    "<radix><<#f a=\"x>>y\">></radix>", piscina, intern);
CREDO_VERUM (res.successus);
si (res.successus)
{
    StmlNodus* trans;

    trans = stml_liberum_ad_indicem(res.elementum_radix, ZEPHYRUM);
    CREDO_NON_NIHIL (trans);
    si (trans != NIHIL)
    {
        CREDO_AEQUALIS_I32 ((i32)trans->genus,
                            (i32)STML_NODUS_TRANSCLUSIO);
        CREDO_CHORDA_AEQUALIS_LITERIS (*trans->valor,
                                       "#f a=\"x>>y\"");
    }
    serialized = stml_scribere(res.radix, piscina, FALSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (serialized,
        "<radix><<#f a=\"x>>y\">></radix>");
}
```

- [ ] **1.2 Run** `./compile_tests.sh stml` — expect the new
  assertions RED (valor truncates at the inner `>>`).
- [ ] **1.3 Implement** — in the `dum` scan loop, before the `>>`
  check: if the current byte is `"` or `'`, advance past the
  matching close quote (or to end of input) and `perge`. Unclosed
  quote = scan to EOF (the token then fails to close — existing
  lenient behavior).
- [ ] **1.4 Green** — `./compile_tests.sh stml` all four suites.
- [ ] **1.5 Retinue** — `./silva/amalgamare.sh`; silva suite 50/50;
  `./tools/natura_struere.sh`; formator check. Commit
  (`stml: scan '>>' transclusionis citationum-conscius`).

---

## Task 2 — TU skeleton: expandere of a macro-free document

**Files:** Create `include/stml_macros.h`, `lib/stml_macros.c`,
`probationes/probatio_stml_macros.c`.

**Interfaces (produced, frozen for all later tasks):**

```c
/* stml_macros.h - expansio fragmentorum parametrizatorum (v1).
 * Plagula formam macroneam SERVAT; expansio proiectio pura
 * tree->tree tempore onerationis; arbor originalis intacta. */
#include "stml.h"

nomen enumeratio {
    STML_EXPANSIO_BENE                 = ZEPHYRUM,
    STML_EXPANSIO_FRAGMENTUM_IGNOTUM   = I,
    STML_EXPANSIO_FRAGMENTUM_POSTERIUS = II,
    STML_EXPANSIO_FRAGMENTUM_GEMINUM   = III,
    STML_EXPANSIO_LOCULUS_NON_IMPLETUS = IV,
    STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM= V,
    STML_EXPANSIO_LOCULUS_IGNOTUS      = VI
} StmlExpansioVitium;

nomen structura {
    StmlNodus* nodus;           /* radix splicis in arbore expansa */
       chorda* fragmentum_id;
    StmlNodus* vocatio;         /* nodus transclusionis ORIGINALIS */
           i32 stratum;         /* profunditas impletionis, I-basata */
} StmlExpansioNota;

nomen structura {
           b32  successus;
    StmlNodus*  radix_expansa;
          Xar*  tabula_expansionum;  /* StmlExpansioNota */
    StmlExpansioVitium vitium;
           i32  linea;              /* nodi peccantis */
        chorda  fragmentum;         /* id in quaestione (aut vacua) */
        chorda  loculus;            /* loculus in quaestione (aut vacua) */
} StmlExpansioResultus;

StmlExpansioResultus
stml_expandere (StmlNodus* radix, Piscina* piscina,
                InternamentumChorda* intern);
```

- [ ] **2.1 Failing test** — probatio skeleton (header comment
  naming the two-tree contract; piscina + credo boilerplate per
  probatio_stml_sedes.c), first fixture:

```c
fons = chorda_ex_literis("<radix><a x=\"1\"/>t</radix>", piscina);
res  = stml_legere(fons, piscina, intern);
CREDO_VERUM (res.successus);
exp  = stml_expandere(res.radix, piscina, intern);
CREDO_VERUM (exp.successus);
CREDO_NON_NIHIL (exp.radix_expansa);
CREDO_VERUM (exp.radix_expansa != res.radix);
CREDO_AEQUALIS_I32 (xar_numerus(exp.tabula_expansionum), ZEPHYRUM);
/* emissio expansae == emissio originalis (nihil expandendum) */
CREDO_CHORDA_AEQUALIS (stml_scribere(exp.radix_expansa, piscina, FALSUM),
                       stml_scribere(res.radix, piscina, FALSUM));
```

- [ ] **2.2 Wire the build** —
  `./tools/compile_tests_fontes_generare.sh`; run
  `./compile_tests.sh stml_macros` TWICE if the first pass exits 2
  (new-suite discovery); expect RED (link succeeds, expandere stub
  returns vacuum result).
- [ ] **2.3 Implement** — `stml_expandere` = recursive
  `_expandere_nodum`: this task clones everything via
  `stml_duplicare_superficialiter` + child recursion (the walk owns
  child iteration because later tasks intercept fragments/calls);
  tabula = `xar_creare(piscina, magnitudo(StmlExpansioNota))`.
- [ ] **2.4 Green**, formator on all three new files, commit
  (`stml_macros: sceletum - expansio documenti sine macris`).

---

## Task 3 — Definitions + zero-arg calls + the three fragment vitia

**Behavior:** definitions collected in document order and DROPPED
from output; `<<#id>>` splices a clone of the body children;
non-`#` transclusions pass through; nota per splice (stratum I).
Strata law: a call resolves only against definitions whose document
position PRECEDES it (for calls inside bodies: the position of the
definition that contains them).

- [ ] **3.1 Failing tests** (each its own credo section):

```c
/* (a) definitio demissa + vocatio impleta */
"<radix><#f><a/><b/></#><<#f>></radix>"
    -> expansa scribit "<radix><a/><b/></radix>"
    -> tabula: I nota; fragmentum_id "f"; stratum I;
       nota.vocatio == nodus transclusionis in ARBORE ORIGINALI
       (identitas monstrata per aequalitatem punctatoris)
/* (b) transclusio selectoris transit */
"<radix><<caput titulus>></radix>" -> emissio == fons; tabula vacua
/* (c) FRAGMENTUM_IGNOTUM */
"<radix><<#nemo>></radix>" -> !successus; vitium == ..IGNOTUM;
    fragmentum == "nemo"
/* (d) FRAGMENTUM_POSTERIUS */
"<radix><<#f>><#f><a/></#></radix>" -> !successus; ..POSTERIUS
/* (e) FRAGMENTUM_GEMINUM */
"<radix><#f><a/></#><#f><b/></#></radix>" -> !successus; ..GEMINUM
/* (f) arbor originalis intacta: scribere(originalis) idem ante et
   post expansionem (fixtura (a) reuso) */
/* (g) fragmentum INTRA corpus = contentum (transit in visionem
   contenti, non collectum): "<radix><#f><#g/></#><<#f>></radix>"
   -> "<radix><#g/></radix>" */
```

- [ ] **3.2 Red** → **3.3 Implement**: tabula dispersa
  (chorda-interned id → StmlNodus* definitionis + index ordinis);
  the walk at a fragment node with `fragmentum_id`: collect (geminum
  check), return without emitting; at a transclusion node whose
  valor begins `#`: interior parse (id only, this task), lookup +
  order check, clone each body child (`stml_duplicare`), append,
  record nota. Anonymous fragments (`<#>`) are NOT definitions —
  they pass through as content (no id to call).
- [ ] **3.4 Green**, commit
  (`stml_macros: definitiones + vocationes nudae + tria vitia`).

---

## Task 4 — Loculi: declarations, filling, the three loculus vitia

**Behavior:** slot declarations `attr="@nomen"` on the definition
tag; call arguments fill them; substitution in attribute values
(whole `valor == "&@n;"` and interpolated spans) and text nodes;
`&@...` spread forms and undeclared references = LOCULUS_IGNOTUS at
COLLECTION time (loud at the definition site); missing/extra at
call time. `&@x;` outside any definition body stays literal.

- [ ] **4.1 Failing tests**:

```c
/* (a) impletio attributi tota */
"<radix><#f p=\"@p\"><a x=\"&@p;\"/></#><<#f p=\"123\">></radix>"
    -> "<radix><a x=\"123\"/></radix>"
/* (b) interpolata */  x="pre-&@p;-post"  ->  x="pre-123-post"
/* (c) impletio textus */
"<#t v=\"@v\"><m>&@v;!</m></#><<#t v=\"salve\">>"
    -> "<m>salve!</m>"
/* (d) LOCULUS_NON_IMPLETUS: vocatio sine p */
/* (e) ARGUMENTUM_SUPERFLUUM: vocatio cum q non declarato */
/* (f) LOCULUS_IGNOTUS: corpus refert &@ignotum; non declaratum
   (vitium in COLLECTIONE - linea definitionis) */
/* (g) '&@x;' EXTRA corpus definitionis = littera:
   "<radix>&@x;</radix>" -> emissio == fons; successus */
/* (h) argumenta cum '>>' in valore (fixtura Task 1 composita):
   <<#f p="a>>b">> -> x="a>>b" */
```

- [ ] **4.2 Red** → **4.3 Implement**:
  - `_vocationem_parsare`: interior chorda → id + argument pairs
    (name `=` quoted value; quote-aware; same charset as attribute
    names). Unquoted values follow the attribute lexer's leniency.
  - Declaration read at collection: definition-tag attributes whose
    valor begins `@` declare loculi (name = valor after `@`);
    collection-time body scan for `&@name;` spans (text valors +
    attribute valors, recursive) against the declared set.
  - Fill: after `stml_duplicare` of a body child, `_substituere`
    walk: for each attributum valor and text valor containing
    `&@`, rebuild via chorda_aedificator (span replace, intern).
    `&@name` without `;` = literal (the AT&T rule).
- [ ] **4.4 Green**, commit
  (`stml_macros: loculi - impletio et tria vitia loculorum`).

---

## Task 5 — Nested calls and strata

- [ ] **5.1 Failing tests**:

```c
/* (a) corpus vocat macronem PRIOREM: stratum II in tabula */
"<radix><#a><x/></#><#b><<#a>><y/></#><<#b>></radix>"
    -> "<radix><x/><y/></radix>";
    tabula: II notae - {id "b", stratum I}, {id "a", stratum II}
    (ordo: notae in ordine splicis)
/* (b) corpus vocat macronem POSTERIOREM = ..POSTERIUS */
"<radix><#b><<#a>></#><#a><x/></#><<#b>></radix>" -> !successus
/* (c) loculi trans strata: b transfert argumentum ad a?
   NON in v1 - argumenta corporis interioris litteralia sunt
   (nulla transitio implicita); fixtura asserit '&@p;' in corpore
   b NON declaratum in b = LOCULUS_IGNOTUS (lex clara) */
```

- [ ] **5.2 Red** → **5.3 Implement**: the T3 fill (blind
  `stml_duplicare` of body children) becomes the WALK applied to
  body children — same `_expandere_nodum` recursion, now carrying
  (stratum+I, resolution ceiling = the calling definition's
  order-index, the substitution map). A blind deep-copy would carry
  nested calls verbatim; the walk expands them. Notas for inner
  splices record their own vocatio (the call node inside the
  DEFINITION body — original tree). Recursion needs no depth guard:
  the ceiling strictly decreases (earlier-only), termination by
  construction — assert with a three-level chain fixture.
- [ ] **5.4 Green**, commit (`stml_macros: strata ordine documenti`).

---

## Task 6 — Suite consolidation + amalgam readiness

- [ ] **6.1** Full `./compile_tests.sh stml` + `stml_macros` green;
  full root suite green (141 with the new suite).
- [ ] **6.2** Formator all touched files; worklog entry
  (`lib/stml_macros.worklog.md` — birth note: design pointers,
  the duplicare-carries-the-clone discovery).
- [ ] **6.3** Commit (`stml_macros: consolidatio suitarum`).

---

## Task 7 — The arbor slice

The exploratory task: contract exact, code discovered at the seam.

- [ ] **7.1 Orient** — silva/CLAUDE.md protocol: phase-log tail;
  read `silva/fontes/silva_arbor_parsura.c` (the M2 loader) and the
  emitter sites (`./silva/nexus.sh silva_arbor` family) for the
  repeated lexeme shape (spec's lex-zephyrum-class: identical
  token elements differing only in `position`).
- [ ] **7.2 Pick ONE shape** and record it in the silva phase-log
  INTENTIO (which element, how many occurrences in the 73-file
  corpus tier — measure, don't guess).
- [ ] **7.3 Loader first** (read side is safe before the emitter
  changes): parsura load calls `stml_expandere` after
  `stml_legere`; macro-free documents pass through (Task 2 contract
  — zero behavior change; corpus gates stay green). Silva's amalgam
  now needs stml_macros: `silva/fontes_politica.sh` review →
  `tools/amalgama_fontes_generare.sh` →
  `tools/amalgama_excludenda_generare.sh` → hand-extend
  `amalgama/silva.h` CADENDA_TYPEDEF (StmlExpansioVitium,
  StmlExpansioNota, StmlExpansioResultus) → hospes exercise (one
  stml_expandere call) → `./silva/amalgamare.sh` VERIFICATUM.
- [ ] **7.4 Emitter** — the chosen shape emitted as `<#...>`
  definition + `<<#... position="N">>` calls. Born-red gate first:
  a fixture in `silva/probationes/` asserting the generated
  document CONTAINS the fragment definition and calls, and that
  the loaded (expanded) semantics equal the pre-macro form.
- [ ] **7.5 The M2 byte-gate holds** — probatio_silva_arbor_plagula
  (78+154) green: file on disk = macro form, roundtrip byte-exact
  through pretty (macro nodes are ordinary nodes to the writer).
- [ ] **7.6** Silva suite 50/50 + root suite green; phase-log
  RELATIO with instrument debrief; commit
  (`arbor: fetta macronea - forma communis una ut fragmentum`).

---

## Task 8 — Bookkeeping

- [ ] **8.1** stml-visio.md §9.1: pointer to spec + status
  (VIVIT-partial: params shipped, query fragments outstanding);
  sigil registry row for `&@x;` (VIVIT, template-space).
- [ ] **8.2** arbor-parsura-spec.md §4: "reserved, not built" note
  gains "BUILT 2026-XX-XX — see stml-macros-spec.md".
- [ ] **8.3** MEMORY.md: bench line ("expand an STML document →
  stml_expandere; file keeps macro form") + stml-text-semantics
  topic-file addendum. Ledger: parcum 01M0XJQAVP notas per task,
  clausum at milestone end. Commit (`gesta: macros v1 acta`).

## Open at execution time (deliberately)

- The exact arbor shape (7.2 measures it).
- Whether `stml_macros.h` content merges into stml.h at amalgam
  time or rides as a second vendored header (follow the existing
  amalgama header pattern when 7.3 reaches it).
- Pretty-writer treatment of fragment interiors is KNOWN-thin
  (desideratum 01M0GR6Y1T, linked) — if the arbor slice trips it,
  that desideratum activates rather than this plan absorbing it.
