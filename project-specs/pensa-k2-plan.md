# pensa K2 — `pensa` library Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no subagents. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** the checklist library: parse the outline text into schedule
groups (sections, per-section dedent, heading = top-level line with
children), list the items for a day, and keep ticks as volume acts.

**Architecture:** `lib/pensa.c` with a PURE parser (`pensa_formam_legere`:
text -> groups, each holding its `recursus` rule or its error) and
`pensa_res_diei`; a tick store `PensaDocumentum` that replays acts of
genus `pensa` into a hash table (key = ISO day + `\x1f` + item text,
last act wins) and reads new acts before every query; a thin book reader
`pensa_textum_legere` over a new `scriba_liber_textum_legere` that keeps
blank lines.

**Tech Stack:** C89 in Latin, `recursus` (K1), `volumen` acts,
`tabula_dispersa`, `stml`, `scriba_liber`, credo v2.

**Spec:** `project-specs/pensa-spec.md` §IV (IV.1 revised with Fran
2026-10-23: sections, dedent, children).

## Global Constraints

- Same flags and C89/Latin rules as K1 (pensa-k1-plan.md "Global
  Constraints"): no Roman-numeral identifiers (`L`, `C`, `D`, `M`, `I`,
  `V`, `X` …), `i32` unsigned, `chorda` not NUL-terminated.
- The library never reads a clock; "today and past are tickable" is the
  calendar's rule (K3).
- Act data is STML text content (escaped by the writer), never an
  attribute value (K1: `&quot;` stays encoded in attributes).
- New `lib/pensa.c` -> `./tools/compile_tests_fontes_generare.sh`.
- Commit only through `silva.commissio`, only on Fran's word.

## Review Focus

1. **Odd text** — Windows line ends (`\r\n`), tabs, trailing spaces,
   a final line without `\n`, an empty text: groups exactly as the
   spaces-only form. Pinned in Task 2.
2. **Item text with markup characters** — `R&D "notes" <x>` must tick
   and read back identically (STML escaping). Pinned in Task 3.
3. **Two documents on one volume** — a tick by one is seen by the other
   on its next query; no lost or doubled acts. Pinned in Task 3.
4. **Same item in two groups on one day** — listed once, ticked once.
   Pinned in Task 2.
5. **Blank-line handling across pages** — a page break separates
   sections; leading/trailing blank lines of a page do not invent
   sections or items. Pinned in Task 1.

---

### Task 1: `scriba_liber_textum_legere` (book text with blank lines)

**Files:**
- Modify: `include/scriba_liber.h`, `lib/scriba_liber.c`
- Modify: `probationes/probatio_scriba_liber.c` (section `textus`)

**Interfaces:**
- Produces: `b32 scriba_liber_textum_legere(Piscina* piscina,
  InternamentumChorda* intern, Volumen* volumen,
  constans character* praefixum, chorda* exitus)`.

- [ ] **Step 1: Failing test**

In `probationes/probatio_scriba_liber.c`, before `SECTIONES`:

```c
/* pensa K2: textus libri cum lineis vacuis; paginae "\n\n" separatae;
 * lineae vacuae finales paginae demptae; liber absens vacuus */
interior vacuum
_sectio_textus (
    CredoContextus* c)
{
              Volumen* vol;
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;
               chorda  textus;

    (vacuum)c;
    vol = volumen_temporarium(piscina, "probatio_scriba_liber");
    CREDO_NECESSE_NON_NIHIL(vol);

    CREDO_GRADUS("liber absens: vacuus");
    CREDO_VERUM(scriba_liber_textum_legere(piscina, intern, vol, "pensa",
        &textus));
    CREDO_AEQUALIS_I32(textus.mensura, ZEPHYRUM);

    CREDO_GRADUS("paginae duae, lineae vacuae servatae");
    l = scriba_liber_aperire_in_spatio(piscina, intern, vol, "pensa", XL,
        X);
    CREDO_NECESSE_NON_NIHIL(l);
    d = scriba_liber_pagina(l, lit("1"));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, piscina, d->latitudo,
        d->altitudo, "vitamins\n\nweekends\n    mow the lawn\n\n");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    d = scriba_liber_paginam_condere(l, lit("2"));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, piscina, d->latitudo,
        d->altitudo, "  sundays\n    mass");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_VERUM(scriba_liber_textum_legere(piscina, intern, vol, "pensa",
        &textus));
    CREDO_CHORDA_AEQUALIS_LITERIS(textus,
        "vitamins\n\nweekends\n    mow the lawn\n\n  sundays\n    mass");
    volumen_claudere(vol);
}
```

and register `{ "textus", _sectio_textus, _parare, NIHIL, NIHIL, NIHIL },`
after the `comes` row. Run `./compile_tests.sh probatio_scriba_liber` —
Expected: compile FAIL (`scriba_liber_textum_legere` undeclared).

(`scriba_liber_pagina(l, "1")` exists because opening a new book writes
page "1"; page "2" is created by `scriba_liber_paginam_condere`.)

- [ ] **Step 2: Header**

`include/scriba_liber.h`, before `#endif`:

```c
/* pensa K2: textus libri TOTUS cum lineis vacuis (sectiones separant),
 * LECTIO SOLA: pagina quaeque lineis suis (vacuae finales demptae),
 * paginae "\n\n" separatae; liber absens = textus vacuus (VERUM);
 * FALSUM si index malus. praefixum NIHIL = liber communis */
b32
scriba_liber_textum_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* exitus);
```

- [ ] **Step 3: Implementation** (end of `lib/scriba_liber.c`)

```c
b32
scriba_liber_textum_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* exitus)
{
                     chorda  index;
                     chorda  spatium;
                        Xar* nomina;
                        b32  inventum;
                        b32  prima;
                        i32  i;
                        i32  linea;
                        s32  ultima;
          ChordaAedificator* a;
           ScribaDocumentum* doc;
    constans TabulaCharacterum* t;

    si (!piscina || !intern || !volumen || !exitus)
    {
        redde FALSUM;
    }
    *exitus = chorda_vacua_libri();
    vias_libri(praefixum, piscina, &index, &spatium);
    nomina = nomina_legere(piscina, intern, volumen, index, &inventum);
    si (!inventum)
    {
        redde VERUM;
    }
    si (!nomina)
    {
        redde FALSUM;
    }
    a      = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    prima  = VERUM;
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        doc = scriba_documentum_aperire(piscina, intern, volumen,
            chorda_ut_cstr(chorda_concatenare(spatium,
            *(chorda*)xar_obtinere(nomina, i), piscina), piscina));
        t = doc ? scriba_documentum_tabula(doc) : NIHIL;
        ultima = -I;
        per (linea = ZEPHYRUM; t && linea < (i32)t->altitudo; linea++)
        {
            si (lineam_legere(t, linea, piscina).mensura > ZEPHYRUM)
            {
                ultima = (s32)linea;
            }
        }
        si (ultima < ZEPHYRUM)
        {
            perge;
        }
        si (!prima)
        {
            chorda_aedificator_appendere_literis(a, "\n\n");
        }
        prima = FALSUM;
        per (linea = ZEPHYRUM; (s32)linea <= ultima; linea++)
        {
            si (linea > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '\n');
            }
            chorda_aedificator_appendere_chorda(a, lineam_legere(t, linea,
                piscina));
        }
    }
    *exitus = chorda_aedificator_finire(a);
    redde VERUM;
}
```

(`lineam_legere` returns an empty chorda for a whitespace-only line and
keeps leading spaces — verified in K1's audienda. If `t->altitudo` is
already `i32`, drop the cast.)

- [ ] **Step 4: Run** — `./compile_tests.sh probatio_scriba_liber`
Expected: PASS.

- [ ] **Step 5: Plants**
- `"\n\n"` -> `"\n"` between pages (red: textus).
- `ultima = (s32)linea;` guarded by `> ZEPHYRUM` -> `>= ZEPHYRUM` (red:
  trailing blank lines kept).

---

### Task 2: the parser and items for a day

**Files:**
- Create: `include/pensa.h` (full approved API)
- Create: `lib/pensa.c` (parser + `pensa_res_diei`; tick store stubs
  come in Task 3 — write only the parser part now)
- Create: `probationes/probatio_pensa.c` (sections `forma`, `dies`)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: `recursus_legere`, `recursus_continet` (K1).
- Produces: `PensaGrex`, `PensaForma`, `pensa_formam_legere`,
  `pensa_res_diei`.

- [ ] **Step 1: Header** — `include/pensa.h`:

```c
/* pensa.h - INDEX COTIDIANUS (pensa K2; project-specs/pensa-spec.md IV):
 * forma = textus ordinatus in SECTIONES (lineis vacuis separatas), quae
 * quaeque per se dedentatur; in sectione linea summa CUM liberis
 * (lineis magis indentatis) = schedula (recursus) et liberi res eius;
 * linea summa SINE liberis = res cotidiana (numquam schedula). Signa =
 * acta in volumine (genus "pensa"), dies + textus rei. Horologium
 * numquam legitur: vocans dies dat (regula "hodie et praeterita" est
 * kalendarii, K3). */

#ifndef PENSA_H
#define PENSA_H

/* <aedilis corpus="lib/pensa.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "fasti.h"
#include "recursus.h"

/* grex: schedula et res eius */
nomen structura {
       chorda  schedula;    /* phrasis ut scripta; vacua = "every day"
                             * implicitus (lineae sine liberis) */
    Recursus* recursus;     /* NIHIL si schedula non legitur */
       chorda  vitium;      /* nuntius recursus_legere; vacuum si legitur */
          Xar* res;         /* chorda: textus rerum (spatia extrema
                             * dempta) */
} PensaGrex;

nomen structura {
    Xar* greges;            /* PensaGrex, ordine formae */
} PensaForma;

/* textus -> forma (purus); NIHIL tantum si piscina deficit */
PensaForma*
pensa_formam_legere (
      chorda  textus,
    Piscina* piscina);

/* textus formae ex libro scribae "pensa" (scriba_liber_textum_legere);
 * vacuus si liber nondum est */
chorda
pensa_textum_legere (
                Volumen* volumen,
    InternamentumChorda* intern,
                Piscina* piscina);

/* res diei d: gregum quorum schedula d continet, ordine formae, textu
 * iterato semel; greges cum vitio nihil dant. Xar de chorda */
Xar*
pensa_res_diei (
    constans PensaForma* forma,
                   Dies  d,
               Piscina* piscina);

/* signa: acta <pensum actio="factum|infectum" dies="YYYY-MM-DD">textus
 * rei</pensum>; ultimum actum per diem et rem vincit */
nomen structura PensaDocumentum PensaDocumentum;

PensaDocumentum*
pensa_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* acta nova (etiam aliorum processuum) prius leguntur */
b32
pensa_factum (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res);

/* actum solum si status mutatur; FALSUM si scriptura deficit aut res
 * vacua */
b32
pensa_ponere (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res,
                b32  factum);

#endif /* PENSA_H */
```

- [ ] **Step 2: Failing tests** — `probationes/probatio_pensa.c`:

```c
/* probatio_pensa.c - index cotidianus (pensa K2): forma (sectiones,
 * dedentatio, capita cum liberis, res cotidianae, vitia), res diei,
 * signa (acta, ultimum vincit, documenta duo), textus libri. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "fasti.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "recursus.h"
#include "pensa.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* greges ut "schedula:res|res;schedula:res" (schedula vacua: "*";
 * vitium: "!schedula") */
interior chorda
formam_describere (
    constans PensaForma* f,
               Piscina* p)
{
    ChordaAedificator* a;
                  i32  i;
                  i32  k;

    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    per (i = ZEPHYRUM; f && i < xar_numerus(f->greges); i++)
    {
        constans PensaGrex* g;

        g = (constans PensaGrex*)xar_obtinere(f->greges, i);
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ';');
        }
        si (!g->recursus)
        {
            chorda_aedificator_appendere_character(a, '!');
        }
        si (g->schedula.mensura == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '*');
        }
        chorda_aedificator_appendere_chorda(a, g->schedula);
        chorda_aedificator_appendere_character(a, ':');
        per (k = ZEPHYRUM; k < xar_numerus(g->res); k++)
        {
            si (k > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '|');
            }
            chorda_aedificator_appendere_chorda(a,
                *(chorda*)xar_obtinere(g->res, k));
        }
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
formam (
    constans character* textus,
               Piscina* p)
{
    redde formam_describere(pensa_formam_legere(chorda_ex_literis(textus,
        p), p), p);
}

/* res diei ut "a|b|c" */
interior chorda
res_diei (
    constans character* textus,
                   Dies  d,
               Piscina* p)
{
    ChordaAedificator* a;
                  Xar* l;
                  i32  i;

    l = pensa_res_diei(pensa_formam_legere(chorda_ex_literis(textus, p),
        p), d, p);
    a = chorda_aedificator_creare(p, (memoriae_index)LXIV);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '|');
        }
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(l, i));
    }
    redde chorda_aedificator_finire(a);
}

#define FORMA_EXEMPLI \
    "vitamins\n" \
    "morning offering\n" \
    "\n" \
    "weekends\n" \
    "    mow the lawn\n" \
    "    vitamins\n" \
    "\n" \
    "    every other monday from oct 26 2026\n" \
    "        call mom\n" \
    "\n" \
    "fortnightly\n" \
    "    water plants\n" \
    "evening prayer\n"

interior vacuum
_sectio_forma (
    CredoContextus* c)
{
    Piscina* p;

    p = c->piscina;
    CREDO_GRADUS("exemplum: sectiones, dedentatio, vitium, cotidiana");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam(FORMA_EXEMPLI, p),
        "*:vitamins|morning offering;weekends:mow the lawn|vitamins;"
        "every other monday from oct 26 2026:call mom;"
        "!fortnightly:water plants;*:evening prayer");

    CREDO_GRADUS("textus vacuus; lineae vacuae solae");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("", p), "");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("\n  \n\n", p), "");

    CREDO_GRADUS("sectio tota indentata sine liberis: cotidiana");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("   x\n   y", p), "*:x|y");

    CREDO_GRADUS("tab, CRLF, spatia finalia, linea ultima sine fine");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam(
        "sundays  \r\n\tmass \r\n\r\nrosary", p),
        "sundays:mass;*:rosary");

    CREDO_GRADUS("liberi profundiores: res eiusdem capitis");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("weekdays\n  a\n    b\n  c", p),
        "weekdays:a|b|c");

    CREDO_GRADUS("linea prima magis indentata quam sequens");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("  a\nmondays\n  c", p),
        "*:a;mondays:c");

    CREDO_GRADUS("in sectione una: cotidiana, caput, cotidiana - ordo");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("a\nmondays\n  x\nb", p),
        "*:a;mondays:x;*:b");

    CREDO_GRADUS("vitium in grege servatur");
    {
        PensaForma* f;
        PensaGrex* g;

        f = pensa_formam_legere(chorda_ex_literis("fortnightly\n  x", p), p);
        CREDO_NECESSE(f && xar_numerus(f->greges) == I);
        g = (PensaGrex*)xar_obtinere(f->greges, ZEPHYRUM);
        CREDO_VERUM(g->recursus == NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS(g->vitium,
            "unknown word \"fortnightly\"");
    }
}

interior vacuum
_sectio_dies (
    CredoContextus* c)
{
    Piscina* p;

    p = c->piscina;
    CREDO_GRADUS("sabbatum: cotidiana, weekends, iterata semel");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXIV), p),
        "vitamins|morning offering|mow the lawn|evening prayer");
    CREDO_GRADUS("lunae 26 Oct: call mom; vitium nihil dat");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXVI), p),
        "vitamins|morning offering|call mom|evening prayer");
    CREDO_GRADUS("martis 27 Oct: nihil schedulae");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXVII), p),
        "vitamins|morning offering|evening prayer");
    CREDO_GRADUS("lunae 2 Nov: hebdomas altera, nulla");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, XI, II), p),
        "vitamins|morning offering|evening prayer");
    CREDO_GRADUS("forma NIHIL: Xar vacuus");
    CREDO_AEQUALIS_I32(xar_numerus(pensa_res_diei(NIHIL,
        fasti_dies(MMXXVI, X, XXVI), p)), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dies", _sectio_dies, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pensa", SECTIONES);
}
```

Run: `./compile_tests.sh probatio_pensa` — Expected: link FAIL.

- [ ] **Step 3: Implementation** — `lib/pensa.c` (parser part):

```c
/* pensa.c - index cotidianus (include/pensa.h) */
#include "pensa.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include "scriba_liber.h"

#include <string.h>

#define TABULATIO  IV     /* '\t' = IV columnae indentationis */

nomen structura {
    chorda textus;        /* spatiis extremis demptis; vacua = linea
                           * vacua (sectiones separat) */
       i32 indentatio;    /* columnae (tab = TABULATIO) */
} LineaFormae;

/* spatia, tabulae et '\r' extrema dempta */
interior chorda
purgare (chorda c)
{
    dum (c.mensura > ZEPHYRUM
        && (c.datum[ZEPHYRUM] == ' ' || c.datum[ZEPHYRUM] == '\t'))
    {
        c.datum++;
        c.mensura--;
    }
    dum (c.mensura > ZEPHYRUM && (c.datum[c.mensura - I] == ' '
        || c.datum[c.mensura - I] == '\t'
        || c.datum[c.mensura - I] == '\r'))
    {
        c.mensura--;
    }
    redde c;
}

interior PensaGrex*
gregem_addere (
    PensaForma* forma,
         chorda  schedula,
       Piscina* piscina)
{
    PensaGrex* g;

    g = (PensaGrex*)xar_addere(forma->greges);
    si (!g)
    {
        redde NIHIL;
    }
    memset(g, ZEPHYRUM, magnitudo(PensaGrex));
    g->schedula  = schedula;
    g->res       = xar_creare(piscina, (i32)magnitudo(chorda));
    g->recursus  = recursus_legere(schedula.mensura > ZEPHYRUM ? schedula
        : chorda_ex_literis("every day", piscina), piscina, &g->vitium);
    redde g;
}

interior constans LineaFormae*
linea_ad (
    Xar* lineae,
     i32  k)
{
    redde (constans LineaFormae*)xar_obtinere(lineae, k);
}

/* sectio [a, b): dedentatio (indentatio minima), deinde linea summa cum
 * liberis = caput, sine liberis = res cotidiana */
interior vacuum
sectionem_legere (
    PensaForma* forma,
           Xar* lineae,
           i32  a,
           i32  b,
       Piscina* piscina)
{
    PensaGrex* g;
    PensaGrex* cotidianus;
          i32  minima;
          i32  k;
          i32  j;
          i32  h;
       chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    minima = linea_ad(lineae, a)->indentatio;
    per (k = a + I; k < b; k++)
    {
        si (linea_ad(lineae, k)->indentatio < minima)
        {
            minima = linea_ad(lineae, k)->indentatio;
        }
    }
    cotidianus  = NIHIL;
    k           = a;
    dum (k < b)
    {
        j = k + I;
        dum (j < b && linea_ad(lineae, j)->indentatio > minima)
        {
            j++;
        }
        si (j > k + I)
        {
            g = gregem_addere(forma, linea_ad(lineae, k)->textus, piscina);
            per (h = k + I; g && h < j; h++)
            {
                *(chorda*)xar_addere(g->res) = linea_ad(lineae, h)->textus;
            }
            cotidianus = NIHIL;
        }
        alioquin
        {
            si (!cotidianus)
            {
                cotidianus = gregem_addere(forma, vacua, piscina);
            }
            si (cotidianus)
            {
                *(chorda*)xar_addere(cotidianus->res) =
                    linea_ad(lineae, k)->textus;
            }
        }
        k = j;
    }
}

PensaForma*
pensa_formam_legere (
      chorda  textus,
    Piscina* piscina)
{
     PensaForma* forma;
            Xar* lineae;
    LineaFormae* l;
          chorda  linea;
             i32  i;
             i32  k;
             i32  a;
             i32  initium;
             i32  columnae;

    si (!piscina)
    {
        redde NIHIL;
    }
    forma = (PensaForma*)piscina_allocare(piscina, magnitudo(PensaForma));
    si (!forma)
    {
        redde NIHIL;
    }
    forma->greges  = xar_creare(piscina, (i32)magnitudo(PensaGrex));
    lineae         = xar_creare(piscina, (i32)magnitudo(LineaFormae));
    si (!forma->greges || !lineae)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    dum (i < textus.mensura)
    {
        initium = i;
        dum (i < textus.mensura && textus.datum[i] != '\n')
        {
            i++;
        }
        linea.datum    = textus.datum + initium;
        linea.mensura  = i - initium;
        i++;
        l = (LineaFormae*)xar_addere(lineae);
        si (!l)
        {
            redde NIHIL;
        }
        columnae = ZEPHYRUM;
        per (k = ZEPHYRUM; k < linea.mensura && (linea.datum[k] == ' '
             || linea.datum[k] == '\t'); k++)
        {
            columnae += linea.datum[k] == '\t' ? TABULATIO : I;
        }
        l->indentatio  = columnae;
        l->textus      = purgare(linea);
    }
    a = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= xar_numerus(lineae); k++)
    {
        si (   k == xar_numerus(lineae)
            || linea_ad(lineae, k)->textus.mensura == ZEPHYRUM)
        {
            si (k > a)
            {
                sectionem_legere(forma, lineae, a, k, piscina);
            }
            a = k + I;
        }
    }
    redde forma;
}

Xar*
pensa_res_diei (
    constans PensaForma* forma,
                   Dies  d,
               Piscina* piscina)
{
                  Xar* exitus;
    constans PensaGrex* g;
                chorda  r;
                   b32  iterata;
                   i32  i;
                   i32  k;
                   i32  m;

    exitus = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; exitus && forma && i < xar_numerus(forma->greges);
         i++)
    {
        g = (constans PensaGrex*)xar_obtinere(forma->greges, i);
        si (!g->recursus || !recursus_continet(g->recursus, d))
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(g->res); k++)
        {
            r        = *(chorda*)xar_obtinere(g->res, k);
            iterata  = FALSUM;
            per (m = ZEPHYRUM; m < xar_numerus(exitus); m++)
            {
                si (chorda_aequalis(*(chorda*)xar_obtinere(exitus, m), r))
                {
                    iterata = VERUM;
                }
            }
            si (!iterata)
            {
                *(chorda*)xar_addere(exitus) = r;
            }
        }
    }
    redde exitus;
}
```

(Do not include `stml.h`/`tabula_dispersa.h`/`scriba_liber.h` until
Task 3 if `-Wunused` complains about nothing — includes are fine; only
unused FUNCTIONS fail. The three tick functions are declared in the
header but defined in Task 3: the test binary does not call them yet,
so the link succeeds.)

- [ ] **Step 4: Regenerate link lists, run**

```bash
./tools/compile_tests_fontes_generare.sh
./compile_tests.sh probatio_pensa
```
Expected: PASS (sections `forma`, `dies`).

- [ ] **Step 5: Plants**
- `> minima` -> `>= minima` in the children scan (red: exemplum).
- drop `cotidianus = NIHIL;` after a heading (red: the final
  "*:evening prayer" group merges into the first).
- `iterata = VERUM;` -> `iterata = FALSUM;` (red: sabbatum lists
  vitamins twice).
- `'\r'` removed from `purgare` (red: CRLF case).

---

### Task 3: ticks (`PensaDocumentum`)

**Files:**
- Modify: `lib/pensa.c` (tick store)
- Modify: `probationes/probatio_pensa.c` (section `signa`)

**Interfaces:**
- Consumes: `volumen_actum_appendere`, `volumen_acta_generum_legere`
  (Xar of `VolumenActum`: `seq`, `datum`), `tabula_dispersa_*`, `stml_*`.
- Produces: `pensa_aperire`, `pensa_factum`, `pensa_ponere`.

- [ ] **Step 1: Failing test** (before `SECTIONES`; register
`{ "signa", _sectio_signa, NIHIL, NIHIL, NIHIL, NIHIL },`)

```c
interior vacuum
_sectio_signa (
    CredoContextus* c)
{
                Piscina* p;
    InternamentumChorda* intern;
                Volumen* vol;
        PensaDocumentum* a;
        PensaDocumentum* b;
                   Dies  d;
                   Dies  e;
                    s64  acta;

    p       = c->piscina;
    intern  = internamentum_creare(p);
    vol     = volumen_temporarium(p, "probatio_pensa");
    CREDO_NECESSE_NON_NIHIL(vol);
    d = fasti_dies(MMXXVI, X, XXIII);
    e = fasti_dies(MMXXVI, X, XXIV);
    a = pensa_aperire(p, intern, vol);
    CREDO_NECESSE_NON_NIHIL(a);

    CREDO_GRADUS("ponere, legere, idem iterum sine actu");
    CREDO_FALSUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        VERUM));
    CREDO_VERUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_FALSUM(pensa_factum(a, e, chorda_ex_literis("vitamins", p)));
    acta = volumen_summa_actorum(vol);
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        VERUM));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta);

    CREDO_GRADUS("tollere: ultimum vincit");
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        FALSUM));
    CREDO_FALSUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta + I);

    CREDO_GRADUS("textus cum signis notationis");
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p), VERUM));
    CREDO_VERUM(pensa_factum(a, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p)));

    CREDO_GRADUS("documentum alterum videt; scriptura eius videtur");
    b = pensa_aperire(p, intern, vol);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(pensa_factum(b, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p)));
    CREDO_FALSUM(pensa_factum(b, d, chorda_ex_literis("vitamins", p)));
    CREDO_VERUM(pensa_ponere(b, e, chorda_ex_literis("mass", p), VERUM));
    CREDO_VERUM(pensa_factum(a, e, chorda_ex_literis("mass", p)));

    CREDO_GRADUS("res vacua recusatur; spatia extrema demuntur");
    CREDO_FALSUM(pensa_ponere(a, d, chorda_ex_literis("   ", p), VERUM));
    CREDO_VERUM(pensa_factum(a, e, chorda_ex_literis("  mass ", p)));
    volumen_claudere(vol);
}
```

Run — Expected: link FAIL (`pensa_aperire` undefined).

- [ ] **Step 2: Implementation** (append to `lib/pensa.c`)

```c
/* ==================================================
 * Signa
 * ================================================== */

structura PensaDocumentum {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
         TabulaDispersa* signa;    /* clavis -> &signum_factum aut
                                    * &signum_infectum */
                    s64  seq;      /* actum ultimum lectum */
};

hic_manens constans character* constans GENERA_PENSORUM[] = {
    "pensa", NIHIL
};

/* valores tabulae: inscriptiones duae distinctae */
hic_manens character signum_factum;
hic_manens character signum_infectum;

/* "YYYY-MM-DD\x1f<res>" */
interior chorda
clavis_signi (
       Dies  d,
     chorda  res,
    Piscina* piscina)
{
    redde chorda_concatenare(chorda_concatenare(fasti_formare_diem(d,
        FASTI_FORMA_ISO, piscina), chorda_ex_literis("\x1f", piscina),
        piscina), res, piscina);
}

/* acta nova (post doc->seq) in tabulam */
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
              i32  i;

    acta = volumen_acta_generum_legere(doc->volumen, doc->seq,
        GENERA_PENSORUM, doc->piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        actum     = (VolumenActum*)xar_obtinere(acta, i);
        doc->seq  = actum->seq;
        res = stml_legere_ex_literis(chorda_ut_cstr(actum->datum,
            doc->piscina), doc->piscina, doc->intern);
        si (!res.successus || !res.elementum_radix)
        {
            perge;
        }
        actio   = stml_attributum_capere(res.elementum_radix, "actio");
        dies    = stml_attributum_capere(res.elementum_radix, "dies");
        textus  = purgare(stml_textus_internus(res.elementum_radix,
            doc->piscina));
        si (   !actio || !dies || textus.mensura == ZEPHYRUM
            || !fasti_parsare_iso(*dies, &d))
        {
            perge;
        }
        (vacuum)tabula_dispersa_inserere(doc->signa, clavis_signi(d,
            textus, doc->piscina),
            chorda_aequalis_literis(*actio, "factum")
            ? (vacuum*)&signum_factum : (vacuum*)&signum_infectum);
    }
}

PensaDocumentum*
pensa_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    PensaDocumentum* doc;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    doc = (PensaDocumentum*)piscina_allocare(piscina,
        magnitudo(PensaDocumentum));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PensaDocumentum));
    doc->piscina  = piscina;
    doc->intern   = intern;
    doc->volumen  = volumen;
    doc->signa    = tabula_dispersa_creare_chorda(piscina, LXIV);
    si (!doc->signa)
    {
        redde NIHIL;
    }
    acta_legere(doc);
    redde doc;
}

b32
pensa_factum (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res)
{
    vacuum* valor;

    res = purgare(res);
    si (!doc || res.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    acta_legere(doc);
    redde tabula_dispersa_invenire(doc->signa, clavis_signi(d, res,
        doc->piscina), &valor) && valor == (vacuum*)&signum_factum;
}

b32
pensa_ponere (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res,
                b32  factum)
{
    StmlNodus* n;
    StmlNodus* t;

    res = purgare(res);
    si (!doc || res.mensura == ZEPHYRUM || !fasti_dies_valida(d))
    {
        redde FALSUM;
    }
    si (pensa_factum(doc, d, res) == (factum ? VERUM : FALSUM))
    {
        redde VERUM;
    }
    n = stml_elementum_creare(doc->piscina, doc->intern, "pensum");
    t = stml_textum_creare_ex_chorda(doc->piscina, doc->intern, res);
    si (   !n || !t
        || !stml_attributum_addere(n, doc->piscina, doc->intern, "actio",
               factum ? "factum" : "infectum")
        || !stml_attributum_addere_chorda(n, doc->piscina, doc->intern,
               "dies", fasti_formare_diem(d, FASTI_FORMA_ISO,
               doc->piscina))
        || !stml_liberum_addere(n, t)
        || volumen_actum_appendere(doc->volumen, "pensa",
               stml_scribere(n, doc->piscina, FALSUM)) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    acta_legere(doc);
    redde VERUM;
}
```

(`pensa_factum` with an untrimmed query trims it — the "  mass " case.
`!fasti_dies_valida` guards garbage days.)

- [ ] **Step 3: Run** — Expected: PASS (three sections).

- [ ] **Step 4: Plants**
- `chorda_aequalis_literis(*actio, "factum")` -> `(*actio, "facta")`
  (red: every tick reads as untick).
- drop the early "same state" return in `pensa_ponere` (red: act count).
- in `pensa_factum` drop `acta_legere(doc);` (red: document b's tick not
  seen by a).
- `stml_textus_internus` -> `chorda_ex_literis("x", ...)` is too blunt;
  instead plant `purgare(` removed around `stml_textus_internus` (red
  only if trivia leaks — if green, record that the writer adds no
  spaces and drop the plant).

---

### Task 4: book reader, worklog, commit

**Files:**
- Modify: `lib/pensa.c` (`pensa_textum_legere`)
- Modify: `probationes/probatio_pensa.c` (section `liber`)
- Create: `lib/pensa.worklog.md`
- Modify: `project-specs/pensa-spec.md` (§VII K2 status)

- [ ] **Step 1: Failing test** (register `liber`)

```c
interior vacuum
_sectio_liber (
    CredoContextus* c)
{
                Piscina* p;
    InternamentumChorda* intern;
                Volumen* vol;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;

    p       = c->piscina;
    intern  = internamentum_creare(p);
    vol     = volumen_temporarium(p, "probatio_pensa");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_AEQUALIS_I32(pensa_textum_legere(vol, intern, p).mensura,
        ZEPHYRUM);
    l = scriba_liber_aperire_in_spatio(p, intern, vol, "pensa", XL, X);
    CREDO_NECESSE_NON_NIHIL(l);
    d = scriba_liber_pagina(l, chorda_ex_literis("1", p));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, p, d->latitudo,
        d->altitudo, "vitamins\n\nweekends\n  mow the lawn");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(formam_describere(pensa_formam_legere(
        pensa_textum_legere(vol, intern, p), p), p),
        "*:vitamins;weekends:mow the lawn");
    volumen_claudere(vol);
}
```

- [ ] **Step 2: Implementation**

```c
chorda
pensa_textum_legere (
                Volumen* volumen,
    InternamentumChorda* intern,
                Piscina* piscina)
{
    chorda textus;

    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    (vacuum)scriba_liber_textum_legere(piscina, intern, volumen, "pensa",
        &textus);
    redde textus;
}
```

- [ ] **Step 3: Run** `./compile_tests.sh probatio_pensa` and
`./compile_tests.sh probatio_scriba_liber` — Expected: PASS both.

- [ ] **Step 4: Plant** — `"pensa"` -> `"pensum"` in
`pensa_textum_legere` (red: liber section).

- [ ] **Step 5: Worklog, spec, lint**

`lib/pensa.worklog.md`: the outline rule (sections, per-section dedent,
children make a heading, consecutive childless lines = one implicit
group), ticks as plain acts (no historia: an untick is an act; last act
wins; reads new acts before every query, so other panes and processes
are seen), item text as text CONTENT not attribute (K1 lesson).
`pensa-spec.md` §VII: K2 -> DONE <date> <sha>.

```bash
VOCABULA_VIAE_ADDITAE="include/pensa.h lib/pensa.c probationes/probatio_pensa.c lib/scriba_liber.c include/scriba_liber.h probationes/probatio_scriba_liber.c" ./oratio/vocabula.sh -nova
```
Expected: `NOVA 0` (rename or glossary per the house rule otherwise).

- [ ] **Step 6: Commit (on Fran's word)** via `silva.commissio`: paths
`include/pensa.h lib/pensa.c lib/pensa.worklog.md
probationes/probatio_pensa.c include/scriba_liber.h lib/scriba_liber.c
probationes/probatio_scriba_liber.c compile_tests_fontes_generata.sh
project-specs/pensa-spec.md project-specs/pensa-k2-plan.md`.
