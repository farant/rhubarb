# pensa K3 — Checklist block in the day view Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no subagents. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** the calendar's day view shows that day's checklist items
(from the `pensa` book) as selectable, tickable rows between the open
to-dos and the Completed section.

**Architecture:** the calendar opens ONE `PensaDocumentum` per pane at
mount (on the volume it already receives) and reads the outline from
the `pensa` book whenever it lists a day. Checklist rows join the
existing album (`k` heading, `k<n>` rows, album `res` = -(n+1)) and the
day view's selection: `res_electa` may hold `k<n>`; j/k walk a single
day sequence (open to-do ids, `k<n>`, completed ids). Ticking reuses
the "complete" command: its requirement becomes "an agenda item, or a
checklist row on today or a past day". Agenda behaviour is unchanged.

**Tech Stack:** C89 in Latin; `pensa` (K2), `membrum__album`,
`membrum__praecepta`, `figura`, credo v2.

**Spec:** `project-specs/pensa-spec.md` §V.1; design approved by Fran
2026-10-23 in chat (order to-dos -> Checklist -> Completed; selection
`k<n>`; today/past tickable, future dim; ticked = checked + dim +
strikethrough, stays in place; item commands greyed; `u` does not undo
ticks; K4 before K5 kept).

## Global Constraints

- Same C89/Latin rules as K1/K2: no Roman-numeral identifiers, `i32`
  unsigned (`s32` for -1 sentinels), `chorda` not NUL-terminated.
- Text handed to `mandata_textus` must live in the mandata pool
  (kalendarium worklog A2b): checklist text is read with `md->piscina`
  and transcribed.
- A calendar without a volume (tests, `volumen` NIHIL) shows no
  checklist and behaves exactly as before.
- Agenda-view behaviour and every existing calendar test stay unchanged.
- kalendarium now links `pensa` and `recursus`: add both to
  ludus_tessera's hand list and regenerate
  `./tools/compile_tests_fontes_generare.sh`.
- Commit only through `silva.commissio`, only on Fran's word.

## Header text (approved with the plan)

`include/kalendarium.h` — no signature changes. New paragraph after
"VISUS DIEI (A3; ...)":

```c
 * INDEX COTIDIANUS (pensa K3; visus diei, volumine dato): post res
 * apertas titulus "k" (PARTES_INDEX, fascia "Checklist"; in pagina
 * nova "k-c") et ordines "k<n>" (PARTES_ITEM): res pensorum diei
 * (pensa_res_diei super librum "pensa" voluminis, ordine formae);
 * deinde "c" et perfectae. Electio "k<n>" (res_electa); j/k per res et
 * pensa ordine indicis; x aut ictus in capsa signum ponit aut tollit
 * (pensa_ponere) - solum die hodierno aut praeterito (hodie ex porta);
 * dies futuri obscuri, x nihil agit. Pensum signatum: capsa signata,
 * titulus obscurus linea transversa (ut res perfecta), loco suo manet.
 * Mandata rei (titulus, dies, nota, rubricae, < >, X) pensis non
 * praesto (in menu obscura); u signa non revocat (x iterum).
```

`apps/calendarium/organum.stml` — `res_electa` note gains "aut k<n>
(pensum diei, K3)"; then `./tools/organa_infigere.sh`.

## Review Focus

1. **Volume without a `pensa` book** (every existing vicus volume
   today): no `k` rows, selection and paging exactly as before. Pinned
   in Task 2.
2. **Template edited under a stored selection** (`res_electa` = `k5`,
   day has 3 items): falls back to the first row, never a dangling
   selection that blocks j/k. Pinned in Task 2.
3. **Checklist spilling across pages** (default 280x256 pane): j/k
   cross pages, the heading repeats as `k-c`, page turns select a
   checklist row. Pinned in Task 2.
4. **Future day**: `x` and a box click tick nothing (asked of the tick
   store); past day ticks. Pinned in Task 3.
5. **Unbounded memory per frame**: a long-lived `PensaDocumentum`
   queried every frame must not grow its pool. Pinned in Task 1.

---

### Task 1: `pensa` scratch pool (no growth per query)

**Files:**
- Modify: `lib/pensa.c` (`PensaDocumentum`, `acta_legere`,
  `pensa_aperire`, `pensa_factum`, `pensa_ponere`)
- Modify: `probationes/probatio_pensa.c` (section `memoria`)

**Interfaces:** none change (`include/pensa.h` untouched).

- [ ] **Step 1: Failing test** (before `SECTIONES`; register
`{ "memoria", _sectio_memoria, NIHIL, NIHIL, NIHIL, NIHIL },`)

```c
/* K3: documentum diu vivens, quaestio per picturam: piscina eius non
 * crescit (scheda interna); signa alterna clavem novam non faciunt */
interior vacuum
_sectio_memoria (
    CredoContextus* c)
{
                Piscina* p;
    InternamentumChorda* intern;
                Volumen* vol;
        PensaDocumentum* a;
                   Dies  d;
                 chorda  res;
                 chorda  absens;
         memoriae_index  usus;
                    i32  i;

    p       = c->piscina;
    intern  = internamentum_creare(p);
    vol     = volumen_temporarium(p, "probatio_pensa");
    CREDO_NECESSE_NON_NIHIL(vol);
    d       = fasti_dies(MMXXVI, X, XXIII);
    a       = pensa_aperire(p, intern, vol);
    CREDO_NECESSE_NON_NIHIL(a);
    res     = chorda_ex_literis("vitamins", p);
    absens  = chorda_ex_literis("mass", p);
    CREDO_VERUM(pensa_ponere(a, d, res, VERUM));

    CREDO_GRADUS("quaestiones C: piscina non crescit");
    usus = piscina_summa_usus(p);
    per (i = ZEPHYRUM; i < C; i++)
    {
        (vacuum)pensa_factum(a, d, res);
        (vacuum)pensa_factum(a, d, absens);
    }
    CREDO_AEQUALIS_S64((s64)piscina_summa_usus(p), (s64)usus);

    CREDO_GRADUS("signa alterna X: clavis eadem, piscina non crescit");
    usus = piscina_summa_usus(p);
    per (i = ZEPHYRUM; i < X; i++)
    {
        CREDO_VERUM(pensa_ponere(a, d, res, FALSUM));
        CREDO_VERUM(pensa_ponere(a, d, res, VERUM));
    }
    CREDO_AEQUALIS_S64((s64)piscina_summa_usus(p), (s64)usus);
    CREDO_VERUM(pensa_factum(a, d, res));
    volumen_claudere(vol);
}
```

Run `./compile_tests.sh probatio_pensa` — Expected: FAIL in `memoria`
at the first CREDO_AEQUALIS_S64 (pool grows).

- [ ] **Step 2: Implementation** (`lib/pensa.c`)

`PensaDocumentum` gains, after `volumen`:

```c
                Piscina* scheda;   /* quaestio una (vacatur) */
```

`pensa_aperire`, after `doc->volumen  = volumen;`:

```c
    doc->scheda   = piscina_generare_dynamicum("pensa_scheda", XVI * M);
```

and its NIHIL check becomes `si (!doc->scheda || !doc->signa)`.

`acta_legere`: vacate the scratch pool first; read, parse and trim in
it; look the key up in scratch and only copy it into `doc->piscina`
when it is new:

```c
interior vacuum
acta_legere (PensaDocumentum* doc)
{
              Xar* acta;
     VolumenActum* actum;
     StmlResultus  res;
          chorda* actio;
          chorda* dies;
            Dies  d;
           chorda  textus;
           chorda  clavis;
           vacuum* valor;
           vacuum* signum;
              i32  i;

    piscina_vacare(doc->scheda);
    acta = volumen_acta_generum_legere(doc->volumen, doc->seq,
        GENERA_PENSORUM, doc->scheda);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        actum     = (VolumenActum*)xar_obtinere(acta, i);
        doc->seq  = actum->seq;
        res = stml_legere(actum->datum, doc->scheda, doc->intern);
        si (!res.successus || !res.elementum_radix)
        {
            perge;
        }
        actio   = stml_attributum_capere(res.elementum_radix, "actio");
        dies    = stml_attributum_capere(res.elementum_radix, "dies");
        textus  = purgare(stml_textus_internus(res.elementum_radix,
            doc->scheda));
        si (   !actio || !dies || textus.mensura == ZEPHYRUM
            || !fasti_parsare_iso(*dies, &d))
        {
            perge;
        }
        signum = chorda_aequalis_literis(*actio, "factum")
               ? (vacuum*)&signum_factum : (vacuum*)&signum_infectum;
        /* clavis nota: valor renovatur; nova: clavis in piscina
         * documenti (tabula clavem per referentiam tenet) */
        clavis = clavis_signi(d, textus, doc->scheda);
        si (tabula_dispersa_invenire(doc->signa, clavis, &valor))
        {
            (vacuum)tabula_dispersa_inserere(doc->signa, clavis, signum);
        }
        alioquin
        {
            (vacuum)tabula_dispersa_inserere(doc->signa,
                chorda_transcribere(clavis, doc->piscina), signum);
        }
    }
}
```

(Updating a found key passes the scratch chorda only for the lookup;
the stored key is the old persistent one — `tabula_dispersa_inserere`
keeps the slot's key on update.)

`pensa_factum`: the query key goes to scratch —
`clavis_signi(d, res, doc->scheda)`.

`pensa_ponere`: the act is built in scratch — every `doc->piscina` in
the `n`/`t`/attribute/`fasti_formare_diem`/`stml_scribere` lines becomes
`doc->scheda` (it is vacated by the `acta_legere` inside `pensa_factum`
above, and again after the append).

- [ ] **Step 3: Run** `./compile_tests.sh probatio_pensa` — Expected:
PASS (forma, dies, signa, liber, memoria).

- [ ] **Step 4: Plants**
- `volumen_acta_generum_legere(..., doc->scheda)` -> `doc->piscina`
  (red: first assertion).
- drop the `invenire` branch (always insert the transcribed key) (red:
  second assertion).
- `clavis_signi(d, res, doc->scheda)` in `pensa_factum` ->
  `doc->piscina` (red: first assertion).

---

### Task 2: rows, selection, pages (`kalendarium.c`)

**Files:**
- Modify: `lib/kalendarium.c`
- Modify: `include/kalendarium.h` (header text above)
- Modify: `apps/calendarium/organum.stml` (+ `./tools/organa_infigere.sh`)
- Modify: `ludus_tessera/compile_probationes.sh` (hand list: `"recursus"`,
  `"pensa"` after `"agenda"`)
- Modify: `probationes/probatio_kalendarium.c` (fixture + section
  `pensa_ordines`)
- Regenerate: `compile_tests_fontes_generata.sh`

**Interfaces:**
- Consumes: `pensa_aperire`, `pensa_textum_legere`,
  `pensa_formam_legere`, `pensa_res_diei` (Xar of `chorda`).
- Produces (internal): `pensa_diei(m, dies, p)` (Xar of chorda),
  `electiones_diei(m, p)` (Xar of chorda: open ids, `k<n>`, completed
  ids), `res_electa(m, l, p)` (new `p`), `KalendariumFigurae.pensa`.

- [ ] **Step 1: Fixture and failing test** (`probatio_kalendarium.c`;
add `#include "pensa.h"`)

```c
/* K3: forma exempli in libro "pensa" voluminis mundi (pagina XL x X).
 * Dies XIV (feria IV, hodie): vitamins, morning offering, mass; XV:
 * vitamins, morning offering; XVII (sabbatum): + mow the lawn */
hic_manens constans character PENSA_EXEMPLI[] =
    "vitamins\nmorning offering\n\nwednesdays\n  mass\n\nweekends\n"
    "  mow the lawn";

interior b32
pensa_scribere (
                 Mundus* x,
                Piscina* p,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = scriba_liber_aperire_in_spatio(p, x->intern, x->volumen, "pensa",
        XL, X);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, p, d->latitudo,
        d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* agenda diei XIV (agendam_parare) + pensa; superficies magna (index
 * totus in pagina una); visus diei */
interior b32
pensa_parare (
             Mundus* x,
     CredoContextus* c)
{
    x->latitudo  = CCCLX;
    x->altitudo  = CDLXXX;
    si (   !agendam_parare(x, c)
        || !pensa_scribere(x, c->piscina, PENSA_EXEMPLI))
    {
        redde FALSUM;
    }
    reditus(x);
    dispensator_recomponere(x->d);
    redde campus_est(x, "visus", "dies");
}

/* ordines indicis (r<n>, k, k-c, k<n>, c, c-c) ordine arboris */
interior b32
indicem_esse (
                 Mundus* x,
     constans character* exspectati)
{
    Componens* radix;
    Componens* l;
    character  visi[CCLVI];
          i32  i;
          i32  k;
          b32  noster;

    radix           = dispensator_arbor(x->d);
    k               = ZEPHYRUM;
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
        i++)
    {
        l       = componens_liberum(radix, i);
        noster  = l->id.mensura >= I && (   l->id.datum[ZEPHYRUM] == 'k'
               || chorda_aequalis_literis(l->id, "c")
               || chorda_aequalis_literis(l->id, "c-c")
               || (   l->id.datum[ZEPHYRUM] == 'r' && l->id.mensura >= II
                   && l->id.datum[I] >= '0' && l->id.datum[I] <= '9'));
        si (!noster || k + l->id.mensura + II >= (i32)magnitudo(visi))
        {
            perge;
        }
        si (k > ZEPHYRUM)
        {
            visi[k++] = ' ';
        }
        memcpy(visi + k, l->id.datum, (size_t)l->id.mensura);
        k        += l->id.mensura;
        visi[k]  = '\0';
    }
    redde strcmp(visi, exspectati) == ZEPHYRUM;
}

/* K3: ordines pensorum inter res apertas et perfectas; electio per
 * j/k; ictus eligit; dies alius; electio invalida; sine libro nihil;
 * paginae (superficies ordinaria) */
interior vacuum
_probare_pensa_ordines (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(pensa_parare(x, c));

    CREDO_GRADUS("XIV: r1, Checklist, pensa III, Completed");
    CREDO_VERUM(indicem_esse(x, "r1 k k0 k1 k2 c r2 r3"));

    CREDO_GRADUS("j/k: r1 -> k0 .. k2 -> r2, et retro");
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k0"));
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
    clavis(x, 'k');
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));

    CREDO_GRADUS("ictus in ordine pensi eligit");
    CREDO_VERUM(ictus_in(x, "k2", c->piscina));
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));

    CREDO_GRADUS("XVII (sabbatum): pensa III, nullae res");
    clavis(x, 'l');
    clavis(x, 'l');
    clavis(x, 'l');
    CREDO_VERUM(indicem_esse(x, "k k0 k1 k2"));

    CREDO_GRADUS("electio k5 invalida (forma mutata): prima, j procedit");
    CREDO_VERUM(campum_scribere_probatio(x, "res_electa", "k5"));
    dispensator_recomponere(x->d);
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));
}

/* K3: liber "pensa" absens - index ut ante (r1 c r2 r3) */
interior vacuum
_probare_pensa_absentia (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    reditus(x);
    dispensator_recomponere(x->d);
    CREDO_VERUM(indicem_esse(x, "r1 c r2 r3"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "2"));
}

/* K3: superficies ordinaria (capacitas IX): pensa per paginas; titulus
 * "k-c" in pagina nova; j paginam sequitur; ] pensum eligit */
interior vacuum
_probare_pensa_paginas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(pensa_scribere(x, c->piscina, PENSA_EXEMPLI));
    reditus(x);
    dispensator_recomponere(x->d);

    CREDO_GRADUS("pagina I: r1 k k0 k1");
    CREDO_VERUM(indicem_esse(x, "r1 k k0 k1"));

    CREDO_GRADUS("j ter: k2 in pagina II cum titulo iterato");
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
    CREDO_VERUM(campus_est(x, "pagina", "1"));
    CREDO_VERUM(indicem_esse(x, "k-c k2 c r2"));

    CREDO_GRADUS("[ et ]: pagina II incipit a pensa k2");
    clavis(x, '[');
    CREDO_VERUM(campus_est(x, "res_electa", "1"));
    clavis(x, ']');
    CREDO_VERUM(campus_est(x, "res_electa", "k2"));
}
```

`campum_scribere_probatio` (fixture helper; writes as the calendar's
writer through `mutare_ramum` + a one-attribute mutator, as
`campum_scribere` in kalendarium.c does - the owners refuse other
writers silently):

```c
nomen structura {
    constans character* titulus;
    constans character* valor;
} ParProbationis;

interior vacuum
campum_ponere_probatio (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParProbationis* par;

    par = (ParProbationis*)ctx;
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

interior b32
campum_scribere_probatio (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    ParProbationis par;
            chorda prior;
               b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = x->repo->scriptor;
    insula_scriptorem_ponere(x->repo, chorda_ex_literis(
        "calendarium.agere", x->m.piscina));
    ok = mutare_ramum(&x->m.ramus, campum_ponere_probatio, &par);
    insula_scriptorem_ponere(x->repo, prior);
    redde ok;
}
```

Register:

```c
    { "pensa_ordines", _probare_pensa_ordines, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_absentia", _probare_pensa_absentia, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pensa_paginae", _probare_pensa_paginas, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
```

Expected numbers (computed from `capacitas_indicis`, default 280x256):
256/8 = 32 − 2 − 3 − 5 = 22 → ×50/100 = 11 → −2 = 9 cells; rows are 2
cells: page 1 = r1 k k0 k1 (8); page 2 = k-c k2 c r2 (8); page 3 = c-c
r3. Large pane 360x480: 60 − 10 = 50 → 25 − 2 = 23: one page.

Run `./compile_tests.sh probatio_kalendarium` — Expected: FAIL (no `k`
rows) in `pensa_ordines` and `pensa_paginae`; `pensa_absentia` PASS
(it pins the old behaviour).

- [ ] **Step 2: Implementation** (`lib/kalendarium.c`)

1. `#include "pensa.h"`; `KalendariumFigurae` gains
   `PensaDocumentum* pensa;   /* K3: signa (NIHIL sine volumine) */`;
   in `kalendarium_montare`, inside `si (m->figurae_ctx)`, after
   `->volumen  = volumen;`:

```c
        ((KalendariumFigurae*)m->figurae_ctx)->pensa = volumen
            ? pensa_aperire(piscina, intern, volumen) : NIHIL;
```

2. Move the forward declarations of `frustum_identitatis` and
   `numerus_identitatis` (now at ~l.1101 and ~l.2133) up, right after
   `dies_aequales` (they are needed by `res_electa`).

3. After `signati`:

```c
/* K3: res indicis cotidiani diei (Xar de chorda, ordine formae; liber
 * "pensa" voluminis); vacuum sine volumine */
interior Xar*
pensa_diei (
    constans KalendariumMontatio* m,
                             Dies  dies,
                          Piscina* p)
{
    constans KalendariumFigurae* ctx;

    ctx = (constans KalendariumFigurae*)m->figurae_ctx;
    si (!ctx || !ctx->pensa)
    {
        redde xar_creare(p, (i32)magnitudo(chorda));
    }
    redde pensa_res_diei(pensa_formam_legere(pensa_textum_legere(
        ctx->volumen, m->intern, p), p), dies, p);
}
```

4. After `ordines_diei`:

```c
/* K3: claves "k0".."k<n-1>" in l */
interior vacuum
claves_pensorum_addere (
         Xar* l,
         i32  numerus,
    Piscina* p)
{
    character id[XVI];
          i32 i;

    per (i = ZEPHYRUM; l && i < numerus; i++)
    {
        sprintf(id, "k%d", (integer)i);
        *(chorda*)xar_addere(l) = chorda_ex_literis(id, p);
    }
}

/* K3: electiones visus diei ordine indicis: res apertae (id), pensa
 * ("k<n>"), res perfectae (id) - Xar de chorda */
interior Xar*
electiones_diei (
    constans KalendariumMontatio* m,
                          Piscina* p)
{
                   Xar* l;
                   Xar* res;
                   Xar* pensa;
    constans AgendaRes* r;
                    b32  posita;
                    i32  i;

    l       = xar_creare(p, (i32)magnitudo(chorda));
    res     = ordines_diei(m, kalendarium_dies_electus(m), p);
    pensa   = pensa_diei(m, kalendarium_dies_electus(m), p);
    posita  = FALSUM;
    per (i = ZEPHYRUM; l && res && i < xar_numerus(res); i++)
    {
        r = *(constans AgendaRes**)xar_obtinere(res, i);
        si (r->perfecta && !posita)
        {
            claves_pensorum_addere(l, pensa ? xar_numerus(pensa)
                : ZEPHYRUM, p);
            posita = VERUM;
        }
        *(chorda*)xar_addere(l) = r->id;
    }
    si (!posita)
    {
        claves_pensorum_addere(l, pensa ? xar_numerus(pensa) : ZEPHYRUM,
            p);
    }
    redde l;
}
```

5. `res_electa` gains `Piscina* p` and a day-view branch (agenda view
   unchanged):

```c
interior chorda
res_electa (
    constans KalendariumMontatio* m,
                             Xar* l,
                         Piscina* p)
{
    chorda* a;
       Xar* e;
       s32  k;
       i32  i;

    a = insula_ramus_attributum(&m->ramus, "res_electa");
    /* K3: visus diei - res et pensa ordine indicis; ignota = prima */
    si (visus_diei(m))
    {
        e = electiones_diei(m, p);
        k = ZEPHYRUM;
        per (i = ZEPHYRUM; a && e && i < xar_numerus(e); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(e, i), *a))
            {
                k = (s32)i;
                frange;
            }
        }
        redde (e && xar_numerus(e) > ZEPHYRUM)
            ? *(chorda*)xar_obtinere(e, (i32)k) : id_vacuum();
    }
    k = a ? index_rei(l, *a) : -I;
    ... (old body unchanged)
}
```

   Callers gain the pool: `promptam_committere` (`p`),
   `ordines_indicis` (`p`), `contextum_parare` (`p`), `ordines_pingere`
   (`md->piscina`).

6. `pagina_rei`: `k<n>` maps to album `res` -(n+1):

```c
    s32 res;
    s32 n;

    p = ...;
    si (!(visus_diei(m) || visus_agendae(m)) || !p)
    {
        redde -I;
    }
    si (visus_diei(m) && numerus_identitatis(id, 'k', &n) && n >= ZEPHYRUM)
    {
        res = -(n + I);
    }
    alioquin
    {
        r = m->documentum ? agenda_res(m->documentum, id) : NIHIL;
        si (!r)
        {
            redde -I;
        }
        res = (s32)r->ordo;
    }
    l = ordines_indicis(m, p);
    redde l ? album_pagina_rei(l, res, pagina_currens(m, l)) : -I;
```

7. `ordines_indicis`, day branch: the checklist block goes where the
   "c" band starts (or at the end):

```c
/* K3: titulus "k" et ordines "k<n>" (res albi -(n+1)) */
interior vacuum
pensa_in_album (
     Album* l,
       Xar* pensa)
{
    character id[XVI];
          i32 n;

    si (!pensa || xar_numerus(pensa) == ZEPHYRUM)
    {
        redde;
    }
    (vacuum)album_addere(l, "k", ALBUM_TITULUS, II, ZEPHYRUM);
    per (n = ZEPHYRUM; n < xar_numerus(pensa); n++)
    {
        sprintf(id, "k%d", (integer)n);
        (vacuum)album_addere(l, id, ALBUM_RES, II, -(s32)(n + I));
    }
}
```

   In the day loop: `pensa = pensa_diei(m, kalendarium_dies_electus(m),
   p); posita = FALSUM;` before it; inside, the `si (r->perfecta &&
   !divisor)` block first calls `pensa_in_album(l, pensa); posita =
   VERUM;` then adds "c"; after the loop `si (!posita) pensa_in_album(l,
   pensa);`.

8. `paginam_vertere`: a page whose first item is a checklist row
   selects it:

```c
    res = album_res_prima(l, k);
    si (res < ZEPHYRUM)
    {
        sprintf(id, "k%d", (integer)(-res - I));
        redde campos_duos_scribere(m, "pagina", pagina, "res_electa", id);
    }
    r = res_ordinis(m, res);
```

   (`res` s32, `id` character[XVI] declared.)

9. `ictus_ordinis`, first:

```c
    s32 n;

    /* K3: ordo pensi "k<n>" (visus diei) */
    si (   nodus && visus_diei(m)
        && numerus_identitatis(nodus->id, 'k', &n) && n >= ZEPHYRUM)
    {
        redde electionem_scribere(m, nodus->id);
    }
```

10. j/k in the day view walk `electiones_diei` (agenda view keeps
    `rem_gradi`):

```c
/* K3: visus diei - electio proxima/prior in electionibus diei */
interior b32
electionem_gradi (
    ContextusMandati* cm,
                 s32  gradus)
{
    Xar* e;
    s32  k;
    i32  i;

    e = electiones_diei(cm->m, cm->p);
    k = -I;
    per (i = ZEPHYRUM; e && i < xar_numerus(e); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(e, i), cm->id))
        {
            k = (s32)i;
            frange;
        }
    }
    si (k < ZEPHYRUM || k + gradus < ZEPHYRUM
        || k + gradus >= (s32)xar_numerus(e))
    {
        redde VERUM;
    }
    redde electionem_scribere(cm->m, *(chorda*)xar_obtinere(e,
        (i32)(k + gradus)));
}
```

    `fac_rem_proximam`: `redde visus_diei(cm->m) ? electionem_gradi(cm,
    I) : rem_gradi(cm, I);` (and `-I` in `fac_rem_priorem`).

11. Header text into `include/kalendarium.h`; manifest note; run
    `./tools/organa_infigere.sh`; hand list; regenerate the link lists.

- [ ] **Step 3: Run** `./compile_tests.sh probatio_kalendarium` and
`./compile_tests.sh probatio_vicus_kalendarium` — Expected: PASS (all
old sections unchanged + 3 new).

- [ ] **Step 4: Plants**
- `pensa_in_album` after the loop only (drop the in-loop call) (red:
  `pensa_ordines` order).
- `electionem_gradi` -> old `rem_gradi` in `fac_rem_proximam` (red:
  j into k0).
- `res = -(n + I)` -> `res = -n` in `pagina_rei` (red: paging — k0
  maps to 0 = birthday/title).
- `paginam_vertere` without the `res < 0` branch (red: `]` selects k2).
- `ictus_ordinis` without the `k` branch (red: click selects).

---

### Task 3: ticking (`x`, box click, today rule)

**Files:**
- Modify: `lib/kalendarium.c` (`ContextusMandati`, `contextum_parare`,
  `REQUIRIT_SIGNANDUM`, `requisita_kalendarii`, `fac_perficere`,
  `TABULA_MANDATORUM` row of `calendarium.perficere`)
- Modify: `probationes/probatio_kalendarium.c` (section `pensa_signa`)

- [ ] **Step 1: Failing test**

```c
/* K3: x et ictus in capsa signant (hodie, praeterito); futurus nihil;
 * menu rei in pensa: perficere sola praesto */
interior vacuum
_probare_pensa_signa (
    CredoContextus* c)
{
             Mundus* x;
    PensaDocumentum* b;
               Fines  f;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(pensa_parare(x, c));
    b = pensa_aperire(c->piscina, x->intern, x->volumen);
    CREDO_NECESSE_NON_NIHIL(b);

    CREDO_GRADUS("hodie: x in k0 signat, iterum tollit");
    clavis(x, 'j');
    clavis(x, 'x');
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("vitamins", c->piscina)));
    CREDO_VERUM(campus_est(x, "res_electa", "k0"));
    clavis(x, 'x');
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("vitamins", c->piscina)));

    CREDO_GRADUS("ictus in capsa k1: eligit et signat");
    f = fines_componentis(x, "k1", c->piscina);
    ictus_ad(x, f.x + X, f.y + IV);
    CREDO_VERUM(campus_est(x, "res_electa", "k1"));
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("morning offering", c->piscina)));

    CREDO_GRADUS("res agendae: x adhuc perficit");
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'x');
    CREDO_VERUM(agenda_res(x->documentum, id_ex(I, c->piscina))
        ->perfecta);

    CREDO_GRADUS("XV (futurus): x et capsa nihil agunt");
    clavis(x, 'l');
    dispensator_recomponere(x->d);
    clavis(x, 'x');
    f = fines_componentis(x, "k0", c->piscina);
    ictus_ad(x, f.x + X, f.y + IV);
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XV),
        chorda_ex_literis("vitamins", c->piscina)));
    CREDO_FALSUM(pensa_factum(b, fasti_dies(MMXXVI, X, XV),
        chorda_ex_literis("morning offering", c->piscina)));

    CREDO_GRADUS("XIII (praeteritus): x signat");
    clavis(x, 'h');
    clavis(x, 'h');
    dispensator_recomponere(x->d);
    clavis(x, 'x');
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIII),
        chorda_ex_literis("vitamins", c->piscina)));
}
```

(On XV and XIII there are no agenda rows, so the first row is k0. The
future check asks the tick store, not the volume's act count: the
fixture's conservatio writes to the same volume.)

Register `{ "pensa_signa", _probare_pensa_signa, mundum_parare,
mundum_purgare, NIHIL, NIHIL },`. Run — Expected: FAIL at the first
`pensa_factum` (x needs an agenda item).

- [ ] **Step 2: Implementation**

```c
#define REQUIRIT_SIGNANDUM  IV   /* res electa, aut pensum hodie aut
                                  * ante (K3) */
```

`ContextusMandati` gains:

```c
                    s32  pensum;      /* K3: index pensi, -1 = nullum */
                 chorda  res_pensi;   /* textus eius */
                    b32  signandum;   /* dies hodie aut ante */
```

`contextum_parare`, after `cm->index = -I;` set `cm->pensum = -I;`;
inside the `VISUS_INDICIS` block (after `cm->r = ...`):

```c
        si (   visus_diei(m) && numerus_identitatis(cm->id, 'k', &n)
            && n >= ZEPHYRUM)
        {
            Xar* pensa;

            pensa = pensa_diei(m, cm->dies, p);
            si (pensa && (i32)n < xar_numerus(pensa))
            {
                cm->pensum     = n;
                cm->res_pensi  = *(chorda*)xar_obtinere(pensa, (i32)n);
                cm->signandum  = fasti_comparare(cm->dies, hodie(m))
                               <= ZEPHYRUM;
            }
        }
```

`requisita_kalendarii` adds:

```c
        && (   !(requisita & REQUIRIT_SIGNANDUM) || cm->r
            || (cm->pensum >= ZEPHYRUM && cm->signandum))
```

`calendarium.perficere` row: `REQUIRIT_REM` -> `REQUIRIT_SIGNANDUM`.

`fac_perficere`, first:

```c
    si (cm->pensum >= ZEPHYRUM)
    {
        PensaDocumentum* doc;

        doc = ((KalendariumFigurae*)cm->m->figurae_ctx)->pensa;
        redde !doc || pensa_ponere(doc, cm->dies, cm->res_pensi,
            !pensa_factum(doc, cm->dies, cm->res_pensi)) || VERUM;
    }
```

(`|| VERUM`: the key is consumed either way, as for agenda items.)

- [ ] **Step 3: Run** both calendar suites — Expected: PASS. The
`elenchus_rei` section (menu greying) must stay green unchanged.

- [ ] **Step 4: Plants**
- `cm->signandum = ... <= ZEPHYRUM` -> `VERUM` (red: future acts).
- `REQUIRIT_SIGNANDUM` -> `REQUIRIT_REM` on the perficere row (red:
  first tick).
- `!pensa_factum(...)` -> `VERUM` (red: untick).

---

### Task 4: drawing (band, rows, ticks, dim future) + screenshot

**Files:**
- Modify: `lib/kalendarium.c` (`greges_pingere`, `ordines_pingere`,
  new `pensum_pingere`)
- Modify: `probationes/probatio_kalendarium.c` (section
  `pensa_figurae`)

- [ ] **Step 1: Failing test**

```c
/* K3: fascia "Checklist"; capsa (signata post x), linea per titulum;
 * electus fundo selectionis; futurus obscurus; "k-c" in pagina II */
interior vacuum
_probare_pensa_figuras (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(pensa_parare(x, c));
    clavis(x, 'j');
    clavis(x, 'x');
    clavis(x, 'j');
    md = reddere(x, c->piscina);

    CREDO_GRADUS("fascia Checklist");
    f = fines_componentis(x, "k", c->piscina);
    CREDO_VERUM(fascia_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "Checklist"));

    CREDO_GRADUS("k0 signatum: capsa signata, linea, obscurum");
    f = fines_componentis(x, "k0", c->piscina);
    CREDO_VERUM(capsa_in(md, f, VERUM));
    CREDO_VERUM(linea_transversa_in(md, f));
    CREDO_VERUM(textus_coloris_est(md, f, "vitamins", COLOR_TEXT_DIM));

    CREDO_GRADUS("k1 electum: capsa vacua, accentus, textus plenus");
    f = fines_componentis(x, "k1", c->piscina);
    CREDO_VERUM(capsa_in(md, f, FALSUM));
    CREDO_VERUM(accentus_in(md, f));
    CREDO_VERUM(textus_coloris_est(md, f, "morning offering",
        COLOR_TEXT));

    CREDO_GRADUS("k2 non electum hodie: textus plenus, sine linea");
    f = fines_componentis(x, "k2", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "mass", COLOR_TEXT));
    CREDO_FALSUM(linea_transversa_in(md, f));

    CREDO_GRADUS("XV futurus: k0 (non electum - electio k1 manet) "
        "obscurum, capsa vacua");
    clavis(x, 'l');
    md = reddere(x, c->piscina);
    f  = fines_componentis(x, "k0", c->piscina);
    CREDO_VERUM(textus_coloris_est(md, f, "vitamins", COLOR_TEXT_DIM));
    CREDO_VERUM(capsa_in(md, f, FALSUM));
}

/* K3: superficies ordinaria - "k-c" fascia Checklist in pagina II */
interior vacuum
_probare_pensa_titulum_iteratum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(agendam_parare(x, c));
    CREDO_NECESSE(pensa_scribere(x, c->piscina, PENSA_EXEMPLI));
    reditus(x);
    clavis(x, ']');
    md = reddere(x, c->piscina);
    f  = fines_componentis(x, "k-c", c->piscina);
    CREDO_VERUM(fascia_in(md, f));
    CREDO_VERUM(textus_in_continet(md, f, "Checklist"));
}
```

Register both (`pensa_figurae`, `pensa_titulus_iteratus`). Run —
Expected: FAIL at `fascia_in` (no figure for `k`).

- [ ] **Step 2: Implementation**

`greges_pingere`, before the "caput" branch:

```c
        /* K3: "k" Checklist (dies; "k-c" iteratus) */
        si (   l->partes == PARTES_INDEX
            && (   chorda_aequalis_literis(l->id, "k")
                || chorda_aequalis_literis(l->id, "k-c")))
        {
            figura_fasciam_pingere(md, l->fines);
            figura_textum_audacem(md, l->fines.x + VI, l->fines.y + IV,
                chorda_ex_literis("Checklist", md->piscina),
                figura_color_thematis(COLOR_TEXT));
            perge;
        }
```

New, before `ordines_pingere`:

```c
/* K3: ordo pensi: electus fundo, ceteri linea infra; capsa (signum si
 * factum); titulus obscurus si factum aut dies futurus (nisi electus);
 * factum linea per titulum */
interior vacuum
pensum_pingere (
                Mandata* md,
     constans Componens* l,
                 chorda  textus,
                    b32  factum,
                    b32  electus,
                    b32  signandum)
{
         Punctum  a;
         Punctum  b;
    ColorMandati  color;
             s32  spatium;
             s32  n;

    si (electus)
    {
        figura_ordinem_electum_pingere(md, l->fines);
    }
    alioquin
    {
        a.x = l->fines.x;
        a.y = l->fines.y + l->fines.altitudo - I;
        b.x = l->fines.x + l->fines.latitudo - I;
        b.y = a.y;
        mandata_linea(md, a, b, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    figura_capsam_pingere(md, l->fines.x + VIII, l->fines.y + III,
        factum);
    color    = figura_color_thematis((!electus && (factum || !signandum))
             ? COLOR_TEXT_DIM : COLOR_TEXT);
    spatium  = l->fines.latitudo - XXVI;
    mandata_textus(md, l->fines.x + XXII, l->fines.y + IV, praecidere(
        chorda_transcribere(textus, md->piscina), spatium), ZEPHYRUM,
        color);
    si (factum)
    {
        n = (s32)textus.mensura;
        si (n * KALENDARIUM_LITTERA > spatium)
        {
            n = spatium / KALENDARIUM_LITTERA;
        }
        a.x = l->fines.x + XXII;
        a.y = l->fines.y + IV + KALENDARIUM_LINEA / II;
        b.x = a.x + n * KALENDARIUM_LITTERA - I;
        b.y = a.y;
        mandata_linea(md, a, b, I, color);
    }
}
```

`ordines_pingere`: new locals `Xar* pensa; PensaDocumentum* doc; b32
signandum;`; after `signa = ...`:

```c
    pensa      = agenda ? NIHIL : pensa_diei(m, dies, md->piscina);
    doc        = contextus->pensa;
    signandum  = fasti_comparare(dies, hodie(m)) <= ZEPHYRUM;
```

in the loop, after the `n` branch:

```c
        if (   l->partes == PARTES_ITEM && pensa
            && numerus_identitatis(l->id, 'k', &id) && id >= ZEPHYRUM
            && (i32)id < xar_numerus(pensa))
        {
            chorda t;

            t = *(chorda*)xar_obtinere(pensa, (i32)id);
            pensum_pingere(md, l, t, doc && pensa_factum(doc, dies, t),
                chorda_aequalis(l->id, electa), signandum);
            perge;
        }
```

(written with `si`, of course.)

- [ ] **Step 3: Run** both calendar suites — Expected: PASS.

- [ ] **Step 4: Plants**
- drop the `k`/`k-c` branch in `greges_pingere` (red: fascia).
- `factum` -> `FALSUM` in the `pensum_pingere` call (red: capsa signata).
- `!signandum` dropped from the colour (red: future dim).

- [ ] **Step 5: Screenshot** — headless probe (recipe in
`probatio_ludus_imago.c`, memory app-screenshots): vicus 960x600, fixed
clock 2026-10-14, a calendar pane in day view with `PENSA_EXEMPLI` and
one tick; Read the PNG; show Fran. Probe stays in the scratchpad.

---

### Task 5: vicus end-to-end, worklog, spec, commit

**Files:**
- Modify: `probationes/probatio_vicus_kalendarium.c` (section `pensa`)
- Modify: `lib/kalendarium.worklog.md`, `project-specs/pensa-spec.md`
  (§VII K3 status; AUDIENDA: the day-view selection item resolved)

- [ ] **Step 1: Test** (add `#include "pensa.h"`)

```c
/* K3: calendarium vici: liber "pensa" voluminis vici, visus diei XIV,
 * x signum in volumine ponit (documentum alterum videt) */
interior vacuum
_probare_pensa (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;
        PensaDocumentum* b;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    l = scriba_liber_aperire_in_spatio(c->piscina,
        internamentum_creare(c->piscina), x->volumen, "pensa", XL, X);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina))
          : NIHIL;
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo, "vitamins");
    CREDO_NECESSE(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE_NON_NIHIL(kalendarium_frontale(v));
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#k0"));
    CREDO_VERUM(manus_ludus_clavem(m, 'x', ZEPHYRUM));
    b = pensa_aperire(c->piscina, internamentum_creare(c->piscina),
        x->volumen);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(pensa_factum(b, fasti_dies(MMXXVI, X, XIV),
        chorda_ex_literis("vitamins", c->piscina)));
}
```

Register `{ "pensa", _probare_pensa, mundum_parare, mundum_purgare,
NIHIL, NIHIL },`. (Written after Tasks 2–4, it is expected to pass at
once; its red is the plant below.)

- [ ] **Step 2: Plant** — in `kalendarium_montare`, `->pensa = NIHIL`
always (red: this section; the calendar's own suites stay green only
because they pass the volume — the vicus wiring is what this pins).

- [ ] **Step 3: Worklog** (`lib/kalendarium.worklog.md`): the selection
model (`k<n>`, `electiones_diei`, album res -(n+1)), the REQUIRIT_SIGNANDUM
rule, why ticks are not in agenda undo, the per-frame read of the book
(measure: `sample` or a 100-frame loop like the week view's; record the
number), the pensa scratch-pool fix and why (long-lived document queried
per frame).

- [ ] **Step 4: Spec** — §VII K3 -> DONE <date>; AUDIENDA: remove the
"day view selection read from the key table only" item (resolved: read
`contextum_parare`, `rem_gradi`, `res_electa`, album paging).

- [ ] **Step 5: Lint**

```bash
VOCABULA_VIAE_ADDITAE="lib/kalendarium.c include/kalendarium.h lib/pensa.c probationes/probatio_kalendarium.c probationes/probatio_vicus_kalendarium.c probationes/probatio_pensa.c" ./oratio/vocabula.sh -nova
```

Expected: `NOVA 0`.

- [ ] **Step 6: Commit (on Fran's word)** via `silva.commissio`, gates
radix/generata/aedilis (+ owed: ludus_tessera, vicus); paths: the files
above + `apps/calendarium/organum.stml`, `lib/organa_infixa.c`,
`ludus_tessera/compile_probationes.sh`, `compile_tests_fontes_generata.sh`,
`project-specs/pensa-k3-plan.md`.
