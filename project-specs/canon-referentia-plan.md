# Canon Referentia (# / . signa) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement the value-grammar trichotomy from
`project-specs/canon-referentia-spec.md`: `#x` = individuum (mandatory on
declarations, verbatim key matching), `.x` = kind (vocabulary check against
the citatio `ad=` list), bare = literal always.

**Architecture:** Two new value-genera in lib/canon.c (`identitas`,
`referentia`) enforced by `valor_congruit`; a two-way dispatch in citatio
pass II (`.` → vocabulary, everything else → verbatim key lookup, which
covers both `#` and legacy bare canons like natura.canon); the generator
emits the new genera everywhere plus per-module citations where the target
closure stays inside the module; one bundled regeneration+migration commit
keeps all gates green.

**Tech Stack:** C89 (latina.h), STML, credo tests, house build scripts.

## Global Constraints

- C89 strict: `-pedantic -Wall -Wextra -Werror -Wconversion
  -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
  -Wwrite-strings`. All identifiers/comments Latin; latina.h macro words
  are FORBIDDEN identifiers (`nomen`, `casus`, `ordinarius`, `per`,
  `registrum`, `magnitudo`, ...).
- `i32`/`i64` are UNSIGNED (`s32`/`s64` signed). Never subtract on an
  unsigned counter. `chorda` is NOT null-terminated.
- Tests: `./compile_tests.sh <pattern>`; **exit 2 = NOTHING RAN**; never
  gate on a piped `$?`. Cold `./silva/examen.sh <file>` is the authority
  when hook output races edits (legati header cache never revalidates).
- After editing `include/canon.h`: rebuild `./tools/canon_struere.sh` AND
  `./tools/natura_struere.sh` (natura_examen embeds canon).
- Pre-fix-oracle law: every "must fail" test is verified to FAIL on the
  pre-change code (each task says which assertion is the oracle).
- NEVER stage `FAQ.md`, `gesta/annales/*` (Fran's staging files). Stage
  by explicit path. Commits end with
  `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.
- The `#` character is the **signum**, never "sigillum" (that word is the
  content-seal concept). Spec §0.

---

### Task 1: Genera valorum `identitas` + `referentia`

**Files:**
- Modify: `include/canon.h` (CanonGenusValoris, ~line 63-75)
- Modify: `lib/canon.c` (`genus_legere` ~137, `valor_congruit` ~205)
- Modify: `canon.canon` (attributum genus electio, ~line 79-98)
- Test: `probationes/probatio_canon.c`

**Interfaces:**
- Consumes: existing `CanonGenusValoris`, `valor_congruit(v, a)`.
- Produces: `CANON_GENUS_IDENTITAS = VIII`, `CANON_GENUS_REFERENTIA = IX`
  (public, canon.h); genus strings `"identitas"`/`"referentia"` legal in
  canons. Tasks 2-4 rely on these exact names.

- [ ] **Step 1: Write the failing tests** — add to
  `probationes/probatio_canon.c` after the existing fixture strings:

```c
/* signa (spec canon-referentia): identitas '#' poscit, referentia
 * '#' aut '.', nudum in utroque VITIUM - referentia litterale
 * non est */
interior constans character* CANON_SIGNORUM =
    "<canon dialectus=\"grex\" versio=\"1\">\n"
    "  <elementum nomen=\"grex\" radix=\"verum\">\n"
    "    <liberum nomen=\"ovis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"ovis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <attributum nomen=\"custos\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* GREX_SANUS =
    "<grex>\n"
    "  <ovis nomen=\"#agna-prima\" custos=\".ovis\"/>\n"
    "  <ovis nomen=\"#agna-altera\" custos=\"#agna-prima\"/>\n"
    "</grex>\n";

/* quinque vitia typorum: nomen nudum, referentia nuda, signum
 * sine corpore (bis - '#' et '.'), signum identitatis falsum */
interior constans character* GREX_MALUS =
    "<grex>\n"
    "  <ovis nomen=\"agna-nuda\"/>\n"
    "  <ovis nomen=\"#agna\" custos=\"ovis-nuda\"/>\n"
    "  <ovis nomen=\"#\"/>\n"
    "  <ovis nomen=\"#agna-quarta\" custos=\".\"/>\n"
    "  <ovis nomen=\".agna-puncto\"/>\n"
    "</grex>\n";
```

  And the test block in `principale`, after the index-citation section:

```c
    /* ========================================================
     * PROBARE: genera valorum identitas/referentia (signa)
     * ======================================================== */

    {
        Canon* grex;
        Xar*   vitia;

        imprimere("\n--- Probans signa valorum ---\n");

        grex = canon_ex_literis(CANON_SIGNORUM, piscina, intern);
        CREDO_NON_NIHIL (grex);

        vitia = iudicare_literis(grex, GREX_SANUS, piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        vitia = iudicare_literis(grex, GREX_MALUS, piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), V);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), V);
    }
```

- [ ] **Step 2: Run to verify failure.** `./compile_tests.sh probatio_canon`
  — compile FAILS only if you already added enum members; otherwise runs
  and the GREX_MALUS assertion FAILS (old `genus_legere` maps unknown
  genus strings to TEXTUS, which accepts everything: 0 vitia ≠ IV). **That
  assertion is the oracle.**

- [ ] **Step 3: Add the enum members** in `include/canon.h` — change:

```c
    CANON_GENUS_COMPOSITUM = VII
```

  to (keep the existing comment above VII untouched):

```c
    CANON_GENUS_COMPOSITUM = VII,
    /* signa (spec canon-referentia): valores se nuntiant.
     * identitas  = '#' + corpus compositum (nomen proprium - et
     *              declaratio et referentia, congruentia verbatim)
     * referentia = '#' aut '.' + corpus compositum ('.' = genus e
     *              vocabulario; valor nudus VITIUM est) */
    CANON_GENUS_IDENTITAS  = VIII,
    CANON_GENUS_REFERENTIA = IX
```

- [ ] **Step 4: Teach `genus_legere`** (lib/canon.c ~140) — add before the
  `"compositum"` line:

```c
    si (chorda_aequalis_literis(*s, "identitas"))
        redde CANON_GENUS_IDENTITAS;
    si (chorda_aequalis_literis(*s, "referentia"))
        redde CANON_GENUS_REFERENTIA;
```

- [ ] **Step 5: Teach `valor_congruit`** — add before the `ordinarius:`
  case (the function already declares `i32 i;` at the top):

```c
        casus CANON_GENUS_IDENTITAS:
        casus CANON_GENUS_REFERENTIA:
        {
            character signum;

            si (v->mensura < II)
            {
                redde FALSUM;   /* signum sine corpore, aut nihil */
            }
            signum = (character)v->datum[ZEPHYRUM];
            si (a->genus == CANON_GENUS_IDENTITAS)
            {
                si (signum != '#')
                {
                    redde FALSUM;
                }
            }
            alioquin si (signum != '#' && signum != '.')
            {
                redde FALSUM;
            }
            per (i = I; i < v->mensura; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z') ||
                      (c >= '0' && c <= '9') ||
                      c == '_' || c == '*' || c == '-'))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }
```

- [ ] **Step 6: Add the optiones to `canon.canon`** — in
  `<elementum nomen="attributum">`, genus electio, after
  `<optio>compositum</optio>` add:

```xml
      <optio>identitas</optio>
      <optio>referentia</optio>
```

  and extend that attribute's `nota=` text with: `; identitas/referentia =
  signa ('#' individuum, '.' genus) - spec canon-referentia`.

- [ ] **Step 7: Rebuild + run.** `./tools/canon_struere.sh &&
  ./tools/natura_struere.sh`, then `./compile_tests.sh probatio_canon` —
  expect PASS (105 assertions). Cold-verify sources:
  `./silva/examen.sh lib/canon.c` → ACCIPE.

- [ ] **Step 8: Commit.**

```bash
git add include/canon.h lib/canon.c canon.canon probationes/probatio_canon.c
git commit -m "canon: genera valorum identitas/referentia - signa se nuntiant

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 2: Citatio — dispositio signorum + `CANON_VOCABULUM_IGNOTUM`

**Files:**
- Modify: `include/canon.h` (CanonVitiumGenus, ~line 82-91)
- Modify: `lib/canon.c` (citatio pass II, ~line 1450-1470;
  `canon_nuntius` ~1500)
- Test: `probationes/probatio_canon.c`

**Interfaces:**
- Consumes: Task 1's genera; existing `_suggestio(quaestio, acies, na,
  piscina)` (canon.c:316), `ci->ad_elementa` (Xar de chorda*), the
  `memoriae_index` cast idiom from canon.c:1023.
- Produces: `CANON_VOCABULUM_IGNOTUM = XI` (public); pass II semantics:
  `.` → vocabulary, all else → verbatim key lookup (covers `#` AND legacy
  bare canons — natura.canon must keep working unchanged).

- [ ] **Step 1: Write the failing tests** — new fixtures:

```c
/* dispositio signorum in citatione: '.' vocabularium (sine
 * clavibus), '#' clavis verbatim; forma vetus nuda (natura.canon)
 * clavem verbatim retinet - probatur infra quod fixturae veteres
 * (bibliotheca, silvula) IMMUTATAE virent */
interior constans character* CANON_GREGIS_CITATI =
    "<canon dialectus=\"grex2\" versio=\"1\">\n"
    "  <elementum nomen=\"grex2\" radix=\"verum\">\n"
    "    <liberum nomen=\"ovis\"/>\n"
    "    <liberum nomen=\"canis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"ovis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <attributum nomen=\"custos\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"canis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "  </elementum>\n"
    "  <citatio nomen=\"custodum\" attributum=\"custos\"\n"
    "    ad=\"canis/nomen\" super=\"ovis\"/>\n"
    "</canon>\n";

interior constans character* GREX2_SANUS =
    "<grex2>\n"
    "  <canis nomen=\"#canis-unus\"/>\n"
    "  <ovis nomen=\"#agna\" custos=\".canis\"/>\n"
    "  <ovis nomen=\"#agnella\" custos=\"#canis-unus\"/>\n"
    "</grex2>\n";

/* '.ovis' extra vocabularium (index = canis solum);
 * '#nemo' clavem non habet */
interior constans character* GREX2_MALUS =
    "<grex2>\n"
    "  <ovis nomen=\"#agna\" custos=\".ovis\"/>\n"
    "  <ovis nomen=\"#agnella\" custos=\"#nemo\"/>\n"
    "</grex2>\n";
```

  Test block:

```c
    /* ========================================================
     * PROBARE: dispositio signorum in citatione
     * ======================================================== */

    {
        Canon* grex2;
        Xar*   vitia;

        imprimere("\n--- Probans dispositionem signorum ---\n");

        grex2 = canon_ex_literis(CANON_GREGIS_CITATI,
                                 piscina, intern);
        CREDO_NON_NIHIL (grex2);

        vitia = iudicare_literis(grex2, GREX2_SANUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        vitia = iudicare_literis(grex2, GREX2_MALUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VOCABULUM_IGNOTUM), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);
    }
```

- [ ] **Step 2: Verify failure.** Compile fails on
  `CANON_VOCABULUM_IGNOTUM` (undefined). Add the enum member first (Step
  3), rebuild, then the run-time oracle: `GREX2_SANUS` expects 0 but old
  code key-looks-up `.canis` verbatim → CITATIO_IRRITA → **the sanus 0
  assertion is the oracle** (fails before the dispatch, passes after).

- [ ] **Step 3: Add the vitium member** in include/canon.h —
  `CANON_RADIX_MALA = X` is the last member; give it a trailing comma and
  append:

```c
    CANON_VOCABULUM_IGNOTUM   = XI  /* '.genus' extra indicem ad= */
```

- [ ] **Step 4: canon_nuntius** (lib/canon.c ~1500) — add before
  `ordinarius:`:

```c
        casus CANON_VOCABULUM_IGNOTUM:
            redde "vocabulum extra petitum citationis";
```

- [ ] **Step 5: The dispatch** — in citatio pass II (lib/canon.c, the
  block ending with the CITATIO_IRRITA vitium_addere), replace:

```c
                    v = stml_attributum_capere(n, attr_cstr);
                    si (!v)
                    {
                        perge;
                    }
                    si (!tabula_dispersa_continet(claves, *v))
                    {
                        vitium_addere(vitia, CANON_CITATIO_IRRITA,
                            n, n->titulus, v, ZEPHYRUM, ZEPHYRUM);
                    }
```

  with:

```c
                    v = stml_attributum_capere(n, attr_cstr);
                    si (!v || v->mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    si ((character)v->datum[ZEPHYRUM] == '.')
                    {
                        /* referentia generis: vocabularium ex
                         * indice ad_elementa - clavibus nihil
                         * opus, ergo etiam in canone moduli
                         * currit (spec par. 5) */
                        chorda corpus;
                        b32    notum;
                        i32    mv;

                        corpus = chorda_sectio(*v, I, v->mensura);
                        notum  = FALSUM;
                        per (mv = ZEPHYRUM;
                             mv < xar_numerus(ci->ad_elementa);
                             mv++)
                        {
                            chorda** t;

                            t = (chorda**)xar_obtinere(
                                ci->ad_elementa, mv);
                            si (chorda_aequalis(corpus, **t))
                            {
                                notum = VERUM;
                                frange;
                            }
                        }
                        si (!notum)
                        {
                            chorda* sug;
                            i32     nv;

                            sug = NIHIL;
                            nv  = xar_numerus(ci->ad_elementa);
                            si (nv > ZEPHYRUM)
                            {
                                chorda* acies_v;
                                i32     iv;

                                acies_v = (chorda*)
                                    piscina_allocare(piscina,
                                        magnitudo(chorda) *
                                        (memoriae_index)nv);
                                per (iv = ZEPHYRUM; iv < nv; iv++)
                                {
                                    acies_v[iv] = **(chorda**)
                                        xar_obtinere(
                                            ci->ad_elementa, iv);
                                }
                                sug = _suggestio(&corpus, acies_v,
                                                 nv, piscina);
                            }
                            vitium_addere(vitia,
                                CANON_VOCABULUM_IGNOTUM, n,
                                n->titulus, sug ? sug : v,
                                ZEPHYRUM, ZEPHYRUM);
                        }
                    }
                    alioquin si (!tabula_dispersa_continet(claves,
                                                           *v))
                    {
                        /* '#' verbatim (claves quoque signum
                         * ferunt) ET forma vetus nuda (canones
                         * sine signis - natura.canon) eadem via:
                         * clavis verbatim */
                        vitium_addere(vitia, CANON_CITATIO_IRRITA,
                            n, n->titulus, v, ZEPHYRUM, ZEPHYRUM);
                    }
```

- [ ] **Step 6: Rebuild + run.** `./tools/canon_struere.sh &&
  ./tools/natura_struere.sh`; `./compile_tests.sh canon` — probatio_canon
  all green INCLUDING the untouched bibliotheca/silvula legacy fixtures
  (that is the back-compat gate); probatio_natura_canones still green
  (committed monolith's citations are `#`-free both sides — legacy branch
  covers them). Cold examen ACCIPE on lib/canon.c.

- [ ] **Step 7: Commit.**

```bash
git add include/canon.h lib/canon.c probationes/probatio_canon.c
git commit -m "canon: citatio signa disponit - '.' vocabularium, cetera clavis verbatim

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 3: Generator + regeneratio + migratio (the green-keeping bundle)

Everything in this task lands in ONE commit: the generator changes, all 34
regenerated canons, the regenerated readers, the migrated hortus.planta,
and the updated DOC_* fixtures. Splitting it leaves committed artifacts
stale against the generator (probatio_natura_canones reads the COMMITTED
monolith; probatio_planta_lectio reads hortus).

**Files:**
- Modify: `tools/natura_canones.c` (relatio branch ~1230-1265; enum
  NcCitatioStatus ~93; `_petitum_citabile` ~1030-1115;
  `_clausuram_colligere` ~965-1025; `_apparatum_plicare` +
  `_elementum_aedificare` signatures/prototypes ~150-180; praefatio ~45)
- Modify: `tools/natura_canones_emissio.c` (line ~809 identity genus;
  citation comment block ~1149)
- Modify: `tools/canon_coquere.c` (`_genus_ex` ~307-329)
- Modify: `natura/exempla/hortus.planta` (full rewrite below)
- Modify: `probationes/probatio_natura_canones.c` (DOC_* fixtures)
- Regenerate: `natura/cocta/*.canon` (34), `include/planta_lectio.h`,
  `lib/planta_lectio.c`, `cocta.registrum`

**Interfaces:**
- Consumes: Task 1 genera strings, Task 2 dispatch semantics.
- Produces: canons whose `nomen=` is `genus="identitas"`, relations
  `genus="referentia"`, per-module `<citatio>` where closure ⊆ module.
  New generator enum member `NC_CIT_ALIENA = V`.
  `_petitum_citabile(bib, nodus, multiplex, modulus_requisitus,
  clausura_ex, piscina)`;
  `_clausuram_colligere(bib, g, modulus_requisitus, piscina)` returning
  NIHIL when the closure crosses the required module.

- [ ] **Step 1: Generator — relatio branch.** In `_apparatum_plicare`
  (natura_canones.c), replace the whole relatio branch (the
  `m->genus_valoris = "nomen";` line, the `si (monolithus)` block with the
  LATIUS SCRIBERE comment, the compositum override, and the citation
  block) with:

```c
        alioquin si (chorda_aequalis_literis(*am->nodus->titulus,
                                             "relatio"))
        {
            m->origo         = NC_ORIGO_RELATIO;
            /* referentia UBIQUE (spec canon-referentia par. 4):
             * signum valoris ('#' individuum, '.' genus) se ipsum
             * nuntiat, ergo genus valoris idem est in canone
             * moduli et monolitho - divergentia vetus
             * nomen/compositum hoc ipso moritur. certitudo= et
             * fons= intacta manent (claves NATURAE citant, sine
             * signo) - ea non hac via emittuntur. */
            m->genus_valoris = "referentia";

            {
                Xar*            clausura;
                NcCitatioStatus status;

                status = _petitum_citabile(bib, am->nodus,
                    (b32)(m->discrimen == NC_MEMBRUM_LIBERUM),
                    monolithus ? NIHIL : modulus_cstr,
                    &clausura, piscina);
                si (status == NC_CIT_FIT)
                {
                    m->citatio_ad = clausura;
                }
                _censum_notare(census, status);
            }
        }
```

- [ ] **Step 2: Thread `modulus_cstr`.** `_apparatum_plicare` gains
  parameter `constans character* modulus_cstr` (after `monolithus`);
  update its prototype and both call sites in `_elementum_aedificare`,
  which computes it once near its top:

```c
    constans character* modulus_cstr;

    modulus_cstr = NIHIL;
    si (!monolithus && ens->modulus)
    {
        modulus_cstr = chorda_ut_cstr(*ens->modulus, piscina);
    }
```

- [ ] **Step 3: Enum + decision.** Add to NcCitatioStatus:

```c
    NC_CIT_ALIENA    = V    /* petitum extra modulum domesticum -
                             * canon moduli clavem tenere nequit
                             * (spec par. 6; monolithus numquam:
                             * requisitum eius NIHIL est) */
```

  `_petitum_citabile` gains `constans character* modulus_requisitus`
  (before `clausura_ex`; update prototype). Its genus branch becomes:

```c
    si (g)
    {
        *clausura_ex = _clausuram_colligere(bib, g,
                                            modulus_requisitus,
                                            piscina);
        si (!*clausura_ex)
        {
            redde NC_CIT_ALIENA;
        }
        redde NC_CIT_FIT;
    }
```

  and the res branch:

```c
        e = natura_ens_in(bib, modulus_cstr, ad_cstr);
        si (e && e->discrimen == NATURA_ENS_RES)
        {
            NaturaRes* r;
            Xar*       tituli;
            chorda**   locus;

            r = (NaturaRes*)e->corpus;
            si (modulus_requisitus && r->modulus &&
                !chorda_aequalis_literis(*r->modulus,
                                         modulus_requisitus))
            {
                redde NC_CIT_ALIENA;
            }
            tituli = xar_creare(piscina, (i32)magnitudo(chorda*));
            locus  = (chorda**)xar_addere(tituli);
            *locus = r->titulus;
            *clausura_ex = tituli;
            redde NC_CIT_FIT;
        }
```

  NOTE: `_censum_notare` needs NO new casus — NC_CIT_ALIENA only occurs
  with a non-NIHIL requisitus (per-module), and per-module passes census
  NIHIL. Add that as a one-line comment beside the commutatio.

- [ ] **Step 4: Closure module purity.** `_clausuram_colligere` gains
  `constans character* modulus_requisitus` (before `piscina`; update
  prototype). Inside the genera loop, before adding each genus's titles:

```c
        si (modulus_requisitus && g2->modulus &&
            !chorda_aequalis_literis(*g2->modulus,
                                     modulus_requisitus))
        {
            redde NIHIL;   /* clausura modulum transit - canon
                            * moduli citare nequit (spec par. 6) */
        }
```

  and in the etiam= pass, when a matching res is found, the same check on
  `r->modulus` before `_titulum_semel_addere` — if it fails,
  `redde NIHIL;`.

- [ ] **Step 5: Identity genus + comments.** In natura_canones_emissio.c
  change line ~809:

```c
    fputs("    <attributum nomen=\"nomen\" genus=\"compositum\"/>\n", f);
```

  to:

```c
    fputs("    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n", f);
```

  In `_citationes_scribere`'s generated comment block, replace the final
  line `"alium fert. -->\n"` sequence so the comment ends:

```c
                      "       colliguntur et subgenus titulum "
                      "alium fert. Signa: '#'\n"
                      "       individuum inscriptum (clavis "
                      "verbatim), '.' genus e\n"
                      "       vocabulario indicis. -->\n", f);
```

- [ ] **Step 6: Praefatio.** In NC_PRAEFATIO_FORMA, after the
  `titulum EXACTUM colliguntur...` line and before `%u nomine solo`, add
  (no new %u — keep all six anchors byte-identical):

```c
"  Signa valorum: '#' individuum inscriptum citat (clavis\n" \
"  verbatim), '.' genus e vocabulario indicis ad= (sine\n" \
"  clavibus). Valor nudus in referentia vitium TYPI est.\n" \
```

- [ ] **Step 7: canon_coquere mapping.** In `_genus_ex`
  (canon_coquere.c:307), before the final `redde CANON_GENUS_TEXTUS;`:

```c
    /* signa (spec canon-referentia par. 7): valor VERBATIM manet,
     * signo incluso - chorda* ut textus, consulto (degradatio
     * TACITA vetita: nominatim, non per casum ordinarium) */
    si (chorda_aequalis_literis(*s, "identitas"))
        redde CANON_GENUS_TEXTUS;
    si (chorda_aequalis_literis(*s, "referentia"))
        redde CANON_GENUS_TEXTUS;
```

- [ ] **Step 8: Migrate hortus.planta** — full new content:

```xml
<!--
  natura/exempla/hortus.planta

  Documentum verum sub planta.canon (ipso ex natura/planta.genera
  generato). Non fixum probationis solum: TESTIMONIUM catenae -
  genus semel descriptum tria parit, bibliothecam tagorum,
  schema, structuram C.

  SIGNA (spec canon-referentia): '#' nomen proprium individui -
  in declaratione ET in referentia, congruentia verbatim; '.'
  genus e vocabulario; valor nudus litteralis est. nomen=
  identitas est, genere 'identitas' - orthographia (lineola,
  linea subducta) stylus merus est, signum mechanismus.

  auctor-nominis='#carl_linnaeus' sub canone moduli TYPATUR sed
  non resolvitur (petitum trans modulos - classis D): documentum
  .planta personam inscribere nequit. Sub monolitho inscriptio
  poscitur (census sodales suos sedet).

  Quae absunt PROBANT: habitus et altitudo-matura tacent, ergo
  ordinarius= (ex <valor> naturae per catenam vectus) apparere
  debet - et diversus per genus (frutex rosae, arbor mali).
-->
<individua>
  <rosa-canina nomen="#rosa-ad-murum"
               status-vita="florens"
               auctor-nominis="#carl_linnaeus">
    <radix nota="muro proxima"/>
    <historia>
      <eventum quando="2024-04" actio="nominari"/>
    </historia>
  </rosa-canina>

  <granny-smith nomen="#malus-prima" altitudo-matura="4"/>
</individua>
```

- [ ] **Step 9: Update DOC_* fixtures** in probatio_natura_canones.c —
  exact replacements (every identity and instance reference gains `#`):
  - DOC_RESOLVENS: `nomen="#domus-nostra"`,
    `inscriptio-eius="#via-lata-x"`, `nomen="#via-lata-x"`
  - DOC_PENDENS: `nomen="#domus-nostra"`,
    `inscriptio-eius="#nemo-omnino"`, `nomen="#via-lata-x"`
  - DOC_CLAUSURA_RESOLVENS: `nomen="#apis-prima"`,
    `nomen="#rosa-prima"`, `pollinatur-a="#apis-prima"`
  - DOC_CLAUSURA_PENDENS: `nomen="#apis-prima"`,
    `nomen="#rosa-prima"`, `pollinatur-a="#nemo-omnino"`
  - DOC_GEMINUM_TRANS and DOC_GEMINUM_IDEM: `nomen="#idem-nomen"`
  - Then `grep -n 'nomen="[^#]' probationes/probatio_natura_canones.c`
    and prefix `#` on any remaining DOC fixture identity values (NOT on
    canon-fixture attribute DECLARATIONS like `nomen="nomen"` — only
    document-side values).

- [ ] **Step 10: Rebuild, regenerate, verify.**

```bash
./tools/natura_struere.sh && ./tools/canon_struere.sh
./tools/natura_canones.sh          # expect: canones 34 cocti;
                                   # citationes > 1021 (per-module joins)
./tools/natura_canones.sh -probare; echo "exit: $?"
                                   # exit 3 = readers stale -> next line
./tools/canon_coquere.sh           # regenerate planta_lectio
./tools/natura_canones.sh -probare; echo "exit: $?"   # expect 0
grep -l "<citatio" natura/cocta/*.canon | head       # expect nuntium.canon
                                                     # among per-module
grep 'genus="identitas"' natura/cocta/planta.canon | head -1
```

- [ ] **Step 11: Full suites.**
  `./compile_tests.sh canon > /tmp/t1.log 2>&1; grep -E "PASSED|FAILED" /tmp/t1.log`
  then the same for `planta_lectio` and `natura`. ALL PASS. The praefatio
  census section reconciles automatically (anchors unchanged, numbers
  live).

- [ ] **Step 12: Commit** (explicit paths — generator sources, the two
  emission files, coquere, hortus, the test file, all of natura/cocta/,
  include/planta_lectio.h, lib/planta_lectio.c, cocta.registrum):

```bash
git add tools/natura_canones.c tools/natura_canones.h \
  tools/natura_canones_emissio.c tools/canon_coquere.c \
  natura/exempla/hortus.planta probationes/probatio_natura_canones.c \
  natura/cocta include/planta_lectio.h lib/planta_lectio.c \
  cocta.registrum
git commit -m "natura: signa emittuntur - identitas/referentia ubique, citationes moduli

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 4: Portae novae — vocabularium, inscriptio, canon moduli, restrictio

**Files:**
- Test: `probationes/probatio_natura_canones.c`

**Interfaces:**
- Consumes: regenerated monolith + `natura/cocta/nuntium.canon` +
  `natura/cocta/planta.canon` from Task 3; `_documentum_iudicare`,
  `canon_legere`, `filum_legere_totum` already in the file.

- [ ] **Step 1: New fixtures** (after DOC_CLAUSURA_PENDENS):

```c
/* '.apis' = genus e vocabulario - sententia gradus GENERIS
 * ('a apibus pollinatur, apis nulla certa') ANTE signa
 * inexprimibilis erat */
staticus constans character* DOC_GENUS_REF =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" pollinatur-a=\".apis\"/>\n"
    "</individua>\n";

staticus constans character* DOC_GENUS_IGNOTUM =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" pollinatur-a=\".piscis-volans\"/>\n"
    "</individua>\n";

/* INSCRIPTIO (spec par. 3.1): census sodales suos sedet - linea
 * una individuum dictionarii praesentat, et referentia resolvit */
staticus constans character* DOC_INSCRIPTUS =
    "<individua>\n"
    "  <carl-linnaeus nomen=\"#carl_linnaeus\"/>\n"
    "  <rosa nomen=\"#rosa-una\" auctor-nominis=\"#carl_linnaeus\"/>\n"
    "</individua>\n";

staticus constans character* DOC_NON_INSCRIPTUS =
    "<individua>\n"
    "  <rosa nomen=\"#rosa-una\" auctor-nominis=\"#carl_linnaeus\"/>\n"
    "</individua>\n";

/* CANON MODULI: nuntium petitum intra modulum habet (nuntius
 * missus-a -> actor, ambo nuntii) - canon moduli citare potest */
staticus constans character* DOC_NUNTII_SANUS =
    "<individua>\n"
    "  <actor nomen=\"#actor-unus\"/>\n"
    "  <nuntius nomen=\"#n-unus\" missus-a=\"#actor-unus\"/>\n"
    "</individua>\n";

staticus constans character* DOC_NUNTII_PENDENS =
    "<individua>\n"
    "  <nuntius nomen=\"#n-unus\" missus-a=\"#nemo\"/>\n"
    "</individua>\n";

/* RESTRICTIO: documentum uni-modulare sine referentiis trans
 * modulos verdictum IDEM sub canone moduli et monolitho accipit
 * (spec par. 1 - divergentia = scopus, numquam accidens) */
staticus constans character* DOC_PURUS =
    "<individua>\n"
    "  <rosa-canina nomen=\"#rosa-una\" status-vita=\"florens\"/>\n"
    "</individua>\n";

staticus constans character* DOC_PURUS_VITIOSUS =
    "<individua>\n"
    "  <rosa-canina nomen=\"#rosa-una\" status-vita=\"volans\"/>\n"
    "</individua>\n";
```

- [ ] **Step 2: Test sections** (after section VI, before VII):

```c
    /* ========================================================
     * VIII. SIGNA - vocabularium, inscriptio, canon moduli
     * ======================================================== */

    {
        i32 irrita;
        i32 vocab;
        i32 omnia;

        imprimere("\n--- VIII. signa (vocabularium/inscriptio) ---\n");

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_GENUS_REF,
            CANON_VOCABULUM_IGNOTUM, &vocab, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (vocab, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_GENUS_IGNOTUM, CANON_VOCABULUM_IGNOTUM, &vocab,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (vocab, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);

        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_INSCRIPTUS,
            CANON_CITATIO_IRRITA, &irrita, &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_NON_INSCRIPTUS, CANON_CITATIO_IRRITA, &irrita,
            &omnia, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);
        CREDO_AEQUALIS_I32 (omnia, (i32)I);
    }

    /* ========================================================
     * IX. CANON MODULI CITAT (nuntium: actor intra modulum)
     *     et RESTRICTIO (planta: verdicta congruunt)
     * ======================================================== */

    {
        chorda  fons_moduli;
        chorda  causa;
        Canon*  nuntium;
        Canon*  planta;
        i32     irrita;
        i32     mala;
        i32     omnia_m;
        i32     omnia_t;

        imprimere("\n--- IX. canon moduli + restrictio ---\n");

        causa.datum   = NIHIL;
        causa.mensura = ZEPHYRUM;
        fons_moduli = filum_legere_totum(
                          "natura/cocta/nuntium.canon", piscina);
        CREDO_MAIOR_I32 (fons_moduli.mensura, (i32)ZEPHYRUM);
        nuntium = canon_legere(fons_moduli, piscina, intern,
                               &causa);
        CREDO_NON_NIHIL (nuntium);
        CREDO_MAIOR_I32 (xar_numerus(nuntium->citationes),
                         (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(nuntium,
            DOC_NUNTII_SANUS, CANON_CITATIO_IRRITA, &irrita,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)ZEPHYRUM);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(nuntium,
            DOC_NUNTII_PENDENS, CANON_CITATIO_IRRITA, &irrita,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (irrita, (i32)I);

        fons_moduli = filum_legere_totum(
                          "natura/cocta/planta.canon", piscina);
        CREDO_MAIOR_I32 (fons_moduli.mensura, (i32)ZEPHYRUM);
        planta = canon_legere(fons_moduli, piscina, intern, &causa);
        CREDO_NON_NIHIL (planta);

        /* restrictio: verdicta TOTA congruunt (omnia, non genus
         * unum) - sub utroque canone */
        CREDO_VERUM (_documentum_iudicare(planta, DOC_PURUS,
            CANON_VALOR_MALUS, &mala, &omnia_m, piscina, intern));
        CREDO_VERUM (_documentum_iudicare(monolithus, DOC_PURUS,
            CANON_VALOR_MALUS, &mala, &omnia_t, piscina, intern));
        CREDO_AEQUALIS_I32 (omnia_m, omnia_t);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)ZEPHYRUM);

        CREDO_VERUM (_documentum_iudicare(planta,
            DOC_PURUS_VITIOSUS, CANON_VALOR_MALUS, &mala,
            &omnia_m, piscina, intern));
        CREDO_AEQUALIS_I32 (mala, (i32)I);
        CREDO_VERUM (_documentum_iudicare(monolithus,
            DOC_PURUS_VITIOSUS, CANON_VALOR_MALUS, &mala,
            &omnia_t, piscina, intern));
        CREDO_AEQUALIS_I32 (mala, (i32)I);
        CREDO_AEQUALIS_I32 (omnia_m, omnia_t);
        CREDO_AEQUALIS_I32 (omnia_m, (i32)I);
    }
```

  No new helpers: `_documentum_iudicare` already takes the Canon* as its
  first parameter (sections III-V pass `monolithus`), and comparing the
  `omnia` totals covers the whole-verdict restriction check.

- [ ] **Step 3: Run.** `./compile_tests.sh natura` — both suites PASS.
  Oracles: DOC_GENUS_REF acceptance and DOC_INSCRIPTUS acceptance both
  FAIL if run against Task-2-era code with the OLD monolith (no `.`
  vocabulary, no sigiled keys) — they are meaningful only post-Task-3,
  which is why they live in this task.

- [ ] **Step 4: Commit.**

```bash
git add probationes/probatio_natura_canones.c
git commit -m "natura: portae signorum - vocabularium, inscriptio, canon moduli, restrictio

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 5: Documenta et acta

**Files:**
- Modify: `lib/canon.worklog.md`, `tools/natura_canones.worklog.md`
  (dated entries: what the signa changed, the legacy-bare discovery in
  natura.canon, the coquere verbatim mapping decision)
- Tabularium (MCP): `gerere` on quaestio `01KZECZ2CZ` — nota naming the
  resolution (inscriptio + signa, commits listed) then `status` →
  `clausum`; `addere` desideratum "signa in .genera" (ruling 2's named
  future); `gerere` nota on `01KZE65SAM` (typing divergence closed by
  referentia-ubique)
- Memory: update `canon-project.md` + `natura-project.md` with the signa
  grammar (one paragraph each); MEMORY.md natura/canon lines mention
  "signa #/. shipped"

- [ ] **Step 1:** Write both worklog entries (append-only, dated
  2026-08-XX of execution day).
- [ ] **Step 2:** File the tabularium events listed above.
- [ ] **Step 3:** Update the two memory files + MEMORY.md pointers.
- [ ] **Step 4: Commit** (worklogs only — memory lives outside the repo):

```bash
git add lib/canon.worklog.md tools/natura_canones.worklog.md
git commit -m "canon+natura: acta signorum in worklogs

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

## Non-changes, recorded with cause

- `natura/natura.canon` and all `.genera` files: UNTOUCHED (ruling 2 —
  bare references are that dialect's law; the legacy citatio branch
  exists precisely for it).
- `probatio_canon.c` legacy fixtures (bibliotheca, silvula, capsae):
  UNTOUCHED — they are the back-compat gate for the bare branch.
- emissio's praestitutum speculum (`natura_canones_emissio.c:452-473`):
  no referentia case — relations never carry `ordinarius=` (defaults are
  property-side). If that ever changes, the speculum must learn the
  signa; a comment there is NOT added now (YAGNI).
- Per-module canons do NOT get vocabulary-only citations for
  cross-module targets: the citatio construct carries both checks, and a
  `#`-ref against a key that cannot exist in-dialect would false-reject
  (spec §5's uncited list covers these honestly).
