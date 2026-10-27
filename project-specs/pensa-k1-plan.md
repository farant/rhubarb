# pensa K1 — `recursus` Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no subagents. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** a pure C89 library that reads English schedule phrases
("every other monday from oct 26 2026") into an opaque rule and answers
which days a rule includes — with a corpus file that is both its test
suite and its documentation.

**Architecture:** `lib/recursus.c` = tokenizer -> ordered pattern
functions (each returns 0 no-match / 1 match / -1 committed error) ->
`structura Recursus` (private) -> evaluation by Julian day arithmetic
over `fasti`. `probationes/probatio_recursus.c` walks
`lib/recursus.corpus.stml`: every `<exemplum>` is parsed, its canonical
form and its days in a window checked, and the canonical form parsed
AGAIN (round trip); every `<vitium>` must be rejected with an error
starting with the given text.

**Tech Stack:** C89 in Latin (`latina.h`), `fasti` (dates), `chorda` /
`chorda_aedificator` / `xar` / `piscina`, `stml` + `filum` (test only),
credo v2 (`CredoSectio`, `credo_suitam_currere`).

**Spec:** `project-specs/pensa-spec.md` §III (and §I doctrine).

## Global Constraints

- Flags: `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion
  -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`;
  C89 (no `//`, declarations at block top, no `long long`).
- `i32`/`i64` are UNSIGNED; `s32`/`s64` signed. `DiesIulianus` = `s64`.
- Identifiers, comments in Latin; user-visible messages in English.
  Never use a `latina.h` macro word (or any Roman numeral) as an
  identifier (`nomen`, `per`, `dies` is fine, `mi`/`di`/`CD` are not).
- `chorda` is NOT NUL-terminated.
- No raw `fopen`: files via `filum_legere_totum`.
- New `lib/*.c` -> regenerate `./tools/compile_tests_fontes_generare.sh`.
- Weekday numbering = `fasti_dies_hebdomadis`: 0 = Sunday … 6 = Saturday.
- Anchors carry their year; an interval rule (> 1) without an anchor, or
  whose anchor is not itself a member, is an error (spec §III.3).
- Interval caps: days <= 366, weeks <= 52 (keeps `recursus_proximus`'s
  8-year search complete).
- Commit only through `silva.commissio` and only on Fran's word.

## Review Focus

1. **Untidy typing** — `  Every   Day.` (case, extra spaces, trailing
   period) must parse like `every day`. Pinned in Task 1's corpus.
2. **Windows across year end and leap day** — Fridays over 2026-12-28 …
   2027-01-10; "last day of the month" in February 2028 (29th). Pinned
   in Tasks 2 and 4.
3. **Overlong input** — > 64 words or a 40-character word must give a
   clean error, never overflow. Pinned in Task 6.
4. **Degenerate calls** — NIHIL rule, inverted window, invalid day:
   0 days / FALSUM, no crash or hang. Pinned in Task 6.
5. **Far anchors and the search limit** — every other monday from
   1990-01-01 must keep its phase (next from 2026-10-26 is 2026-11-02);
   "every year on feb 29" from 2097-03-01 must find 2104-02-29 across
   the non-leap 2100. Pinned in Task 6.

All expected dates in this plan were computed with Python's `datetime`
(2026-10-19 is a Monday; 2026-10-27 a Tuesday; 2026-11-01 a Sunday).

---

### Task 1: Skeleton — API, tokenizer, "every day", corpus harness

**Files:**
- Create: `include/recursus.h`
- Create: `lib/recursus.c`
- Create: `lib/recursus.corpus.stml`
- Create: `probationes/probatio_recursus.c`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Produces (public, final for K1):
  `Recursus* recursus_legere(chorda phrasis, Piscina* piscina, chorda* vitium)`,
  `b32 recursus_continet(constans Recursus* r, Dies d)`,
  `i32 recursus_dies(constans Recursus* r, Dies prima, Dies ultima, Xar* exitus)` (Xar of `Dies`),
  `b32 recursus_proximus(constans Recursus* r, Dies ab, Dies* exitus)`,
  `chorda recursus_scribere(constans Recursus* r, Piscina* piscina)`.
- Produces (private, used by later tasks): `Lector`, `Verbum`,
  `est()`, `currens()`, `vitium_ponere()`, `verba_iungere()`,
  `FORMAE[]`, `VOCABULA[]`, `verbum_notum()`, `diem_scribere()`.

- [ ] **Step 1: Write the header**

`include/recursus.h`:

```c
/* recursus.h - SCHEDULA NATURALIA (pensa K1; project-specs/pensa-spec.md
 * III): phrasis Anglica ("every other monday from oct 26 2026") ->
 * regula opaca -> dies. Purus: nec volumen nec horologium - vocans dies
 * dat. Parsator determinatus: phrasis ignota = vitium nominatum,
 * numquam coniectura. Corpus lib/recursus.corpus.stml = probationes et
 * documentatio; forma canonica iterum lecta eandem regulam dat. */

#ifndef RECURSUS_H
#define RECURSUS_H

/* <aedilis corpus="lib/recursus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fasti.h"

/* opaca: interior crescere potest (uniones, exceptiones, finis) */
nomen structura Recursus Recursus;

/* phrasis -> regula in piscina; NIHIL et *vitium (Anglice; vitium NIHIL
 * licet) si non intellegitur */
Recursus*
recursus_legere (
       chorda  phrasis,
     Piscina* piscina,
       chorda* vitium);

/* dies in regula? (NIHIL aut dies invalidus: FALSUM) */
b32
recursus_continet (
    constans Recursus* r,
                 Dies  d);

/* dies [prima, ultima] (inclusive) ordine in exitus (Xar de Dies);
 * numerus additorum (fenestra inversa: 0) */
i32
recursus_dies (
    constans Recursus* r,
                 Dies  prima,
                 Dies  ultima,
                  Xar* exitus);

/* dies primus >= ab in *exitus; FALSUM si nullus intra annos VIII */
b32
recursus_proximus (
    constans Recursus* r,
                 Dies  ab,
                 Dies* exitus);

/* forma canonica Anglica ("Every other Monday from Mon 26 Oct 2026") */
chorda
recursus_scribere (
    constans Recursus* r,
              Piscina* piscina);

#endif /* RECURSUS_H */
```

- [ ] **Step 2: Write the corpus (Task 1 lines)**

`lib/recursus.corpus.stml`:

```xml
<!-- recursus.corpus.stml - CORPUS schedularum (pensa K1; spec
     project-specs/pensa-spec.md III.4). exemplum: phrasis, forma
     canonica, fenestra [ab, ad] et dies EXACTI (dies vacuus nusquam:
     attributum vacuum "true" legeretur); forma canonica iterum lecta
     eadem dare debet. vitium: phrasis recusata, vitium incipit textu
     dato. Formam novam docere = lineam addere, rubram videre,
     viridem facere. Dies computati per Python datetime. -->
<corpus>
  <exemplum phrasis="every day" canon="Every day"
            ab="2026-10-19" ad="2026-10-21"
            dies="2026-10-19 2026-10-20 2026-10-21"/>
  <exemplum phrasis="daily" canon="Every day"
            ab="2026-10-30" ad="2026-11-01"
            dies="2026-10-30 2026-10-31 2026-11-01"/>
  <exemplum phrasis="  Every   Day." canon="Every day"
            ab="2026-10-19" ad="2026-10-19" dies="2026-10-19"/>
  <vitium phrasis="fortnightly"><@incipit=>unknown word "fortnightly"</></vitium>
  <vitium phrasis="every day please"><@incipit=>unknown word "please"</></vitium>
  <vitium phrasis="every"><@incipit=>can't read "every"</></vitium>
</corpus>
```

(If `&quot;` is not decoded inside attribute values by `stml`, write the
two `unknown word` incipits without quotes and match them by
`unknown word` only — check in Step 6 and keep whichever works; the
error text itself keeps its quotes.)

- [ ] **Step 3: Write the failing test**

`probationes/probatio_recursus.c`:

```c
/* probatio_recursus.c - schedula naturalia (pensa K1): corpus
 * lib/recursus.corpus.stml - exemplum legitur, forma canonica, dies in
 * fenestra, iter (forma canonica iterum lecta eadem); vitium recusatur
 * cum initio vitii dato. Sectio 'api' (Task 6): proximus, continet,
 * NIHIL, fenestra inversa, phrasis longa. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include "filum.h"
#include "fasti.h"
#include "recursus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_CORPORIS "lib/recursus.corpus.stml"

/* dies regulae in [ab, ad] ut "YYYY-MM-DD YYYY-MM-DD" */
interior chorda
dies_scribere (
    constans Recursus* r,
               chorda  ab,
               chorda  ad,
              Piscina* p)
{
                 Dies  prima;
                 Dies  ultima;
                  Xar* l;
    ChordaAedificator* a;
                  i32  i;

    l = xar_creare(p, (i32)magnitudo(Dies));
    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    si (!fasti_parsare_iso(ab, &prima) || !fasti_parsare_iso(ad, &ultima))
    {
        redde chorda_ex_literis("fenestra mala", p);
    }
    (vacuum)recursus_dies(r, prima, ultima, l);
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_chorda(a, fasti_formare_diem(
            *(Dies*)xar_obtinere(l, i), FASTI_FORMA_ISO, p));
    }
    redde chorda_aedificator_finire(a);
}

interior b32
aequalis (
    constans character* quid,
                 chorda  receptum,
                 chorda  speratum)
{
    si (chorda_aequalis(receptum, speratum))
    {
        redde VERUM;
    }
    fprintf(stderr, "  %s: '%.*s' (speratum '%.*s')\n", quid,
        (int)receptum.mensura, (constans character*)receptum.datum,
        (int)speratum.mensura, (constans character*)speratum.datum);
    redde FALSUM;
}

/* exemplum: legere, canon, dies; forma canonica iterum: eadem */
interior b32
exemplum_probare (
    StmlNodus* n,
      Piscina* p)
{
       chorda* phrasis;
       chorda* canon;
       chorda* ab;
       chorda* ad;
       chorda* dies;
    Recursus* r;
    Recursus* iterum;
        chorda  vitium;
        chorda  c;

    phrasis  = stml_attributum_capere(n, "phrasis");
    canon    = stml_attributum_capere(n, "canon");
    ab       = stml_attributum_capere(n, "ab");
    ad       = stml_attributum_capere(n, "ad");
    dies     = stml_attributum_capere(n, "dies");
    si (!phrasis || !canon || !ab || !ad || !dies)
    {
        fprintf(stderr, "  exemplum sine attributo\n");
        redde FALSUM;
    }
    r = recursus_legere(*phrasis, p, &vitium);
    si (!r)
    {
        fprintf(stderr, "  '%.*s' recusata: %.*s\n", (int)phrasis->mensura,
            (constans character*)phrasis->datum, (int)vitium.mensura,
            (constans character*)vitium.datum);
        redde FALSUM;
    }
    c = recursus_scribere(r, p);
    si (   !aequalis("canon", c, *canon)
        || !aequalis("dies", dies_scribere(r, *ab, *ad, p), *dies))
    {
        redde FALSUM;
    }
    iterum = recursus_legere(c, p, &vitium);
    si (!iterum)
    {
        fprintf(stderr, "  canon '%.*s' non legitur: %.*s\n",
            (int)c.mensura, (constans character*)c.datum,
            (int)vitium.mensura, (constans character*)vitium.datum);
        redde FALSUM;
    }
    redde aequalis("canon iterum", recursus_scribere(iterum, p), *canon)
        && aequalis("dies iterum", dies_scribere(iterum, *ab, *ad, p),
               *dies);
}

/* vitium: recusata, vitium incipit textu dato */
interior b32
vitium_probare (
    StmlNodus* n,
      Piscina* p)
{
     chorda* phrasis;
     chorda* incipit;
    chorda  vitium;

    phrasis  = stml_attributum_capere(n, "phrasis");
    incipit  = stml_attributum_capere(n, "incipit");
    si (!phrasis || !incipit)
    {
        fprintf(stderr, "  vitium sine attributo\n");
        redde FALSUM;
    }
    si (recursus_legere(*phrasis, p, &vitium))
    {
        fprintf(stderr, "  '%.*s' accepta (recusanda)\n",
            (int)phrasis->mensura, (constans character*)phrasis->datum);
        redde FALSUM;
    }
    si (   vitium.mensura < incipit->mensura
        || memcmp(vitium.datum, incipit->datum, (size_t)incipit->mensura)
           != ZEPHYRUM)
    {
        fprintf(stderr, "  vitium '%.*s' (incipere debet '%.*s')\n",
            (int)vitium.mensura, (constans character*)vitium.datum,
            (int)incipit->mensura, (constans character*)incipit->datum);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
    InternamentumChorda* intern;
                 chorda  textus;
           StmlResultus  res;
             StmlNodus* n;
                    i32  i;
                    i32  exempla;
                    i32  vitia;

    intern  = internamentum_creare(c->piscina);
    textus  = filum_legere_totum(VIA_CORPORIS, c->piscina);
    CREDO_NECESSE(textus.mensura > ZEPHYRUM);
    res = stml_legere_ex_literis(chorda_ut_cstr(textus, c->piscina),
        c->piscina, intern);
    CREDO_NECESSE(res.successus && res.elementum_radix);
    exempla  = ZEPHYRUM;
    vitia    = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(res.elementum_radix);
         i++)
    {
        n = stml_liberum_ad_indicem(res.elementum_radix, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        CREDO_GRADUS(chorda_ut_cstr(*stml_attributum_capere(n, "phrasis"),
            c->piscina));
        si (chorda_aequalis_literis(*n->titulus, "exemplum"))
        {
            exempla++;
            CREDO_VERUM(exemplum_probare(n, c->piscina));
        }
        alioquin si (chorda_aequalis_literis(*n->titulus, "vitium"))
        {
            vitia++;
            CREDO_VERUM(vitium_probare(n, c->piscina));
        }
        alioquin
        {
            fprintf(stderr, "  elementum ignotum in corpore\n");
            CREDO_VERUM(FALSUM);
        }
    }
    /* porta tacita = porta mortua: corpus vere legitur */
    CREDO_VERUM(exempla > ZEPHYRUM);
    CREDO_VERUM(vitia > ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "corpus", _sectio_corpus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("recursus", SECTIONES);
}
```

- [ ] **Step 4: Run it to see it fail**

Run: `./compile_tests.sh probatio_recursus`
Expected: link/compile FAIL (`recursus_legere` undefined) — the library
does not exist yet.

- [ ] **Step 5: Write the library core**

File layout (C needs definitions before use; later tasks insert into
these blocks): **Verba** (tokenizer, `est`, vitium helpers,
`verbum_notum`) -> **Scriptio** (`diem_scribere`; Task 2 adds the weekday
list writers here) -> **Formae** (readers and patterns, `FORMAE[]`) ->
**Lectio** (`regulam_perficere`, `legere_intra`, `recursus_legere`) ->
**Aestimatio** -> **Forma canonica** (`recursus_scribere`).
`recursus_continet` is declared in the header, so `regulam_perficere`
may call it before its definition.

`lib/recursus.c`:

```c
/* recursus.c - schedula naturalia (include/recursus.h) */
#include "recursus.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define VERBA_MAXIMA      LXIV     /* verba phraseos summum */
#define VERBUM_MAXIMUM    XXXII    /* octeti verbi, NUL incluso */
#define DIES_OMNES        0x7F     /* dominica (bit 0) .. sabbatum (6) */
#define FERIAE            0x3E     /* lunae .. veneris */
#define FINES_HEBDOMADIS  0x41     /* dominica et sabbatum */
#define PROXIMI_LIMES     (CCCLXVI * VIII)  /* dies quaesiti summum */
#define BITUS(w)          ((i32)I << (i32)(w))

nomen enumeratio {
    GENUS_HEBDOMAS = I,   /* dies hebdomadis, omni intervallo hebdomadum */
    GENUS_DIES,           /* omni intervallo dierum ab ancora */
    GENUS_MENSIS_DIES,    /* dies mensis (-I ultimus) */
    GENUS_MENSIS_ORDO,    /* dies hebdomadis ordine in mense (-I ultimus) */
    GENUS_ANNUS,          /* mensis et dies quoque anno */
    GENUS_SEMEL           /* dies unus */
} GenusRecursus;

structura Recursus {
    GenusRecursus genus;
              s32 intervallum;   /* >= I */
              i32 hebdomas;      /* BITUS(w), w 0 = dominica */
              s32 ordo;          /* GENUS_MENSIS_ORDO: I..IV, -I */
              s32 dies_mensis;   /* I..XXXI, -I ultimus */
              s32 mensis;        /* GENUS_ANNUS: I..XII */
              b32 ancoratus;     /* "from": initium (et phasis) */
             Dies ancora;
             Dies semel;         /* GENUS_SEMEL */
};

nomen structura {
    character textus[VERBUM_MAXIMUM];  /* minusculis, NUL */
          b32 numerus_est;             /* cifrae, fortasse ordinale */
          s32 numerus;
          b32 ordinale;                /* "1st", "15th" */
          b32 iso_est;                 /* YYYY-MM-DD validus */
          b32 iso_malum;               /* forma ISO, dies impossibilis */
         Dies iso;
} Verbum;

nomen structura {
      Verbum verba[VERBA_MAXIMA];
         i32 numerus;
         i32 locus;
    Piscina* piscina;
      chorda phrasis;     /* purgata (spatia extrema dempta) */
      chorda vitium;
} Lector;

nomen s32 (*FormaRecursus)(Lector* lector, Recursus* r);

/* verba nota (Task 2-5 addunt) */
hic_manens constans character* constans VOCABULA[] = {
    "every", "each", "day", "daily", "everyday", NIHIL
};

/* ==================================================
 * Verba
 * ================================================== */

interior b32
cifra_est (character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
littera_est (character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

interior character
minuscula (character c)
{
    redde (c >= 'A' && c <= 'Z') ? (character)(c - 'A' + 'a') : c;
}

/* numerus, ordinale ("15th"), ISO */
interior vacuum
verbum_classificare (Verbum* v)
{
    i32 n;
    i32 k;
    s32 x;

    n = (i32)strlen(v->textus);
    si (   n == X && v->textus[IV] == '-' && v->textus[VII] == '-'
        && cifra_est(v->textus[ZEPHYRUM]))
    {
        v->iso = fasti_dies(
            (s32)((v->textus[ZEPHYRUM] - '0') * M
                + (v->textus[I] - '0') * C + (v->textus[II] - '0') * X
                + (v->textus[III] - '0')),
            (s32)((v->textus[V] - '0') * X + (v->textus[VI] - '0')),
            (s32)((v->textus[VIII] - '0') * X + (v->textus[IX] - '0')));
        v->iso_est    = fasti_dies_valida(v->iso);
        v->iso_malum  = !v->iso_est;
        redde;
    }
    x = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n && cifra_est(v->textus[k]); k++)
    {
        x = x * X + (s32)(v->textus[k] - '0');
    }
    si (k == ZEPHYRUM || k > VI)
    {
        redde;
    }
    si (k == n)
    {
        v->numerus_est  = VERUM;
        v->numerus      = x;
    }
    alioquin si (   k + II == n
                 && (   strcmp(v->textus + k, "st") == ZEPHYRUM
                     || strcmp(v->textus + k, "nd") == ZEPHYRUM
                     || strcmp(v->textus + k, "rd") == ZEPHYRUM
                     || strcmp(v->textus + k, "th") == ZEPHYRUM))
    {
        v->numerus_est  = VERUM;
        v->ordinale     = VERUM;
        v->numerus      = x;
    }
}

/* vitium in lectore: "<prae>: <post>" (prae vacua: post solum) */
interior s32
vitium_ponere (
                Lector* lector,
                 chorda  prae,
    constans character* post)
{
    ChordaAedificator* a;

    a = chorda_aedificator_creare(lector->piscina, (memoriae_index)LXIV);
    si (prae.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, prae);
        chorda_aedificator_appendere_literis(a, ": ");
    }
    chorda_aedificator_appendere_literis(a, post);
    lector->vitium = chorda_aedificator_finire(a);
    redde -I;
}

/* 'quid "textus"' (verbum ignotum, phrasis illegibilis) */
interior s32
vitium_citatum (
                Lector* lector,
    constans character* quid,
                 chorda  citatum)
{
    ChordaAedificator* a;

    a = chorda_aedificator_creare(lector->piscina, (memoriae_index)LXIV);
    chorda_aedificator_appendere_literis(a, quid);
    chorda_aedificator_appendere_literis(a, " \"");
    chorda_aedificator_appendere_chorda(a, citatum);
    chorda_aedificator_appendere_character(a, '"');
    lector->vitium = chorda_aedificator_finire(a);
    redde -I;
}

/* verba [a, b) spatio iuncta (nuntiis vitiorum) */
interior chorda
verba_iungere (
    Lector* lector,
        i32  a,
        i32  b)
{
    ChordaAedificator* c;
                  i32  i;

    c = chorda_aedificator_creare(lector->piscina, (memoriae_index)XXXII);
    per (i = a; i < b && i < lector->numerus; i++)
    {
        si (i > a)
        {
            chorda_aedificator_appendere_character(c, ' ');
        }
        chorda_aedificator_appendere_literis(c, lector->verba[i].textus);
    }
    redde chorda_aedificator_finire(c);
}

/* phrasis -> verba (minusculis; ',' verbum suum; '&' = "and"; '.' et
 * spatia separant) */
interior b32
verba_secare (Lector* lector)
{
       i32 i;
       i32 n;
    Verbum* v;
    character c;

    lector->numerus = ZEPHYRUM;
    i = ZEPHYRUM;
    dum (i < lector->phrasis.mensura)
    {
        c = (character)lector->phrasis.datum[i];
        si (c == ' ' || c == '\t' || c == '.')
        {
            i++;
            perge;
        }
        si (lector->numerus >= VERBA_MAXIMA)
        {
            (vacuum)vitium_ponere(lector, lector->phrasis, "schedule too long");
            redde FALSUM;
        }
        v = &lector->verba[lector->numerus];
        memset(v, ZEPHYRUM, magnitudo(Verbum));
        si (c == ',' || c == '&')
        {
            strcpy(v->textus, c == ',' ? "," : "and");
            lector->numerus++;
            i++;
            perge;
        }
        si (!littera_est(c) && !cifra_est(c))
        {
            chorda signum;

            signum.datum    = lector->phrasis.datum + i;
            signum.mensura  = I;
            (vacuum)vitium_citatum(lector, "unknown character", signum);
            redde FALSUM;
        }
        n = ZEPHYRUM;
        dum (i < lector->phrasis.mensura)
        {
            c = (character)lector->phrasis.datum[i];
            si (!littera_est(c) && !cifra_est(c) && c != '-')
            {
                frange;
            }
            si (n >= VERBUM_MAXIMUM - I)
            {
                (vacuum)vitium_ponere(lector, lector->phrasis, "word too long");
                redde FALSUM;
            }
            v->textus[n++] = minuscula(c);
            i++;
        }
        v->textus[n] = '\0';
        verbum_classificare(v);
        lector->numerus++;
    }
    redde VERUM;
}

interior Verbum*
currens (Lector* lector)
{
    redde lector->locus < lector->numerus ? &lector->verba[lector->locus] : NIHIL;
}

/* verbum currens == t: progredi */
interior b32
est (
                Lector* lector,
    constans character* t)
{
    Verbum* v;

    v = currens(lector);
    si (v && strcmp(v->textus, t) == ZEPHYRUM)
    {
        lector->locus++;
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
verbum_notum (constans Verbum* v)
{
    i32 i;

    si (   v->numerus_est || v->iso_est || v->iso_malum
        || strcmp(v->textus, ",") == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; VOCABULA[i]; i++)
    {
        si (strcmp(v->textus, VOCABULA[i]) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Scriptio
 * ================================================== */

/* "Mon 26 Oct 2026" */
interior vacuum
diem_scribere (
    ChordaAedificator* a,
                 Dies  d,
              Piscina* p)
{
    character numerus[XVI];

    chorda_aedificator_appendere_chorda(a, fasti_nomen_diei_hebdomadis(
        fasti_dies_hebdomadis(d), VERUM, FASTI_LINGUA_ANGLICA, p));
    sprintf(numerus, " %d ", (integer)d.dies);
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_chorda(a, fasti_nomen_mensis(d.mensis,
        VERUM, FASTI_LINGUA_ANGLICA, p));
    sprintf(numerus, " %d", (integer)d.annus);
    chorda_aedificator_appendere_literis(a, numerus);
}

/* ==================================================
 * Formae (ordine temptantur; 0 non, I congruit, -I vitium)
 * ================================================== */

/* "every day", "each day", "daily", "everyday" */
interior s32
cotidie_legere (
      Lector* lector,
    Recursus* r)
{
    si (!(   est(lector, "daily") || est(lector, "everyday")
          || ((est(lector, "every") || est(lector, "each")) && est(lector, "day"))))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_HEBDOMAS;
    r->intervallum  = I;
    r->hebdomas     = DIES_OMNES;
    redde I;
}

hic_manens constans FormaRecursus FORMAE[] = {
    cotidie_legere,
    NIHIL
};

/* regula perfecta post formam (Task 3: ancorae) */
interior b32
regulam_perficere (
      Lector* lector,
    Recursus* r)
{
    (vacuum)lector;
    (vacuum)r;
    redde VERUM;
}

/* phrasis tota: forma quaeque ab initio; congruens et ad finem = regula */
interior b32
legere_intra (
      Lector* lector,
    Recursus* r)
{
    i32 i;
    i32 k;
    s32 res;

    si (!verba_secare(lector))
    {
        redde FALSUM;
    }
    si (lector->numerus == ZEPHYRUM)
    {
        lector->vitium = chorda_ex_literis("empty schedule", lector->piscina);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < lector->numerus; i++)
    {
        si (!verbum_notum(&lector->verba[i]))
        {
            (vacuum)vitium_citatum(lector, "unknown word",
                chorda_ex_literis(lector->verba[i].textus, lector->piscina));
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; FORMAE[k]; k++)
    {
        memset(r, ZEPHYRUM, magnitudo(Recursus));
        lector->locus = ZEPHYRUM;
        res = FORMAE[k](lector, r);
        si (res < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (res > ZEPHYRUM && lector->locus == lector->numerus)
        {
            redde regulam_perficere(lector, r);
        }
    }
    (vacuum)vitium_citatum(lector, "can't read", lector->phrasis);
    redde FALSUM;
}

Recursus*
recursus_legere (
      chorda  phrasis,
    Piscina* piscina,
      chorda* vitium)
{
      Lector* lector;
    Recursus  r;
    Recursus* exitus;

    si (vitium)
    {
        vitium->datum    = NIHIL;
        vitium->mensura  = ZEPHYRUM;
    }
    si (!piscina)
    {
        redde NIHIL;
    }
    lector = (Lector*)piscina_allocare(piscina, magnitudo(Lector));
    si (!lector)
    {
        redde NIHIL;
    }
    memset(lector, ZEPHYRUM, magnitudo(Lector));
    lector->piscina = piscina;
    /* spatia extrema dempta: nuntii phrasim purgatam citant */
    dum (phrasis.mensura > ZEPHYRUM && (phrasis.datum[ZEPHYRUM] == ' '
        || phrasis.datum[ZEPHYRUM] == '\t'))
    {
        phrasis.datum++;
        phrasis.mensura--;
    }
    dum (phrasis.mensura > ZEPHYRUM
        && (phrasis.datum[phrasis.mensura - I] == ' '
            || phrasis.datum[phrasis.mensura - I] == '\t'))
    {
        phrasis.mensura--;
    }
    lector->phrasis = phrasis;
    si (!legere_intra(lector, &r))
    {
        si (vitium)
        {
            *vitium = lector->vitium;
        }
        redde NIHIL;
    }
    exitus = (Recursus*)piscina_allocare(piscina, magnitudo(Recursus));
    si (exitus)
    {
        *exitus = r;
    }
    redde exitus;
}

/* ==================================================
 * Aestimatio
 * ================================================== */

b32
recursus_continet (
    constans Recursus* r,
                 Dies  d)
{
    si (!r || !fasti_dies_valida(d))
    {
        redde FALSUM;
    }
    si (r->ancoratus && fasti_ante(d, r->ancora))
    {
        redde FALSUM;
    }
    commutatio (r->genus)
    {
        casus GENUS_HEBDOMAS:
            redde (r->hebdomas & BITUS(fasti_dies_hebdomadis(d)))
                != ZEPHYRUM;
        ordinarius:
            redde FALSUM;
    }
    redde FALSUM;
}

i32
recursus_dies (
    constans Recursus* r,
                 Dies  prima,
                 Dies  ultima,
                  Xar* exitus)
{
    DiesIulianus j;
    DiesIulianus finis;
            Dies d;
             i32 n;

    si (   !r || !exitus || !fasti_dies_valida(prima)
        || !fasti_dies_valida(ultima))
    {
        redde ZEPHYRUM;
    }
    n      = ZEPHYRUM;
    finis  = fasti_ad_iulianum(ultima);
    per (j = fasti_ad_iulianum(prima); j <= finis; j++)
    {
        d = fasti_ex_iuliano(j);
        si (recursus_continet(r, d))
        {
            *(Dies*)xar_addere(exitus) = d;
            n++;
        }
    }
    redde n;
}

b32
recursus_proximus (
    constans Recursus* r,
                 Dies  ab,
                 Dies* exitus)
{
    DiesIulianus j;
             s32 k;
            Dies d;

    si (!r || !exitus || !fasti_dies_valida(ab))
    {
        redde FALSUM;
    }
    si (r->ancoratus && fasti_ante(ab, r->ancora))
    {
        ab = r->ancora;
    }
    j = fasti_ad_iulianum(ab);
    per (k = ZEPHYRUM; k < PROXIMI_LIMES; k++, j++)
    {
        d = fasti_ex_iuliano(j);
        si (recursus_continet(r, d))
        {
            *exitus = d;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Forma canonica
 * ================================================== */

chorda
recursus_scribere (
    constans Recursus* r,
              Piscina* piscina)
{
    ChordaAedificator* a;

    si (!r || !piscina)
    {
        chorda c;

        c.datum    = NIHIL;
        c.mensura  = ZEPHYRUM;
        redde c;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    si (r->genus == GENUS_HEBDOMAS && r->hebdomas == DIES_OMNES)
    {
        chorda_aedificator_appendere_literis(a, "Every day");
    }
    si (r->ancoratus)
    {
        chorda_aedificator_appendere_literis(a, " from ");
        diem_scribere(a, r->ancora, piscina);
    }
    redde chorda_aedificator_finire(a);
}
```

- [ ] **Step 6: Regenerate link lists, format, run**

Run:
```bash
./tools/compile_tests_fontes_generare.sh
./silva/formator.sh lib/recursus.c -scribere
./silva/formator.sh include/recursus.h -scribere
./silva/formator.sh probationes/probatio_recursus.c -scribere
./compile_tests.sh probatio_recursus
```
Expected: PASS, "corpus" section with 6 steps. If an incipit with
`&quot;` fails because stml leaves the entity encoded, apply the
fallback from Step 2.

- [ ] **Step 7: Check the corpus file's unit classification**

Run: `bin/fabrica quid lib/recursus.corpus.stml`
Expected: a unit/role (data file of `recursus`). If it reports the file
unclassified, add a `<iudex>` line for `lib/*.corpus.stml` in
`unitates.stml` (memory: never a regex exclusion list) and re-run.

- [ ] **Step 8: Plant (silva.planta), then report to Fran**

Plants, each must go red in `probatio_recursus`:
- `est(lector, "daily")` -> `est(lector, "dailyx")` (corpus line "daily").
- in `verba_secare`, drop `|| c == '.'` (line "  Every   Day.").
- in `recursus_scribere`, `"Every day"` -> `"Every Day"` (canon).

No commit yet (K1 commits once, Task 6).

---

### Task 2: Weekly schedules — weekday names, lists, weekdays/weekends

**Files:**
- Modify: `lib/recursus.c` (vocabulary, weekday reader, two patterns,
  canonical weekly text)
- Modify: `lib/recursus.corpus.stml`

**Interfaces:**
- Consumes: Task 1 `Lector`, `est`, `currens`, `FORMAE[]`, `VOCABULA[]`.
- Produces: `diem_hebdomadis_legere(Lector*, s32* w)` (0=Sunday),
  `dies_seriem_legere(Lector*, i32* mask)`, `hebdomadem_scribere(...)`
  (used by Task 3 for intervals).

- [ ] **Step 1: Add corpus lines (red)**

Append inside `<corpus>`:

```xml
  <exemplum phrasis="weekdays" canon="Weekdays"
            ab="2026-10-23" ad="2026-10-27"
            dies="2026-10-23 2026-10-26 2026-10-27"/>
  <exemplum phrasis="weekends" canon="Weekends"
            ab="2026-10-23" ad="2026-10-27"
            dies="2026-10-24 2026-10-25"/>
  <exemplum phrasis="every weekday" canon="Weekdays"
            ab="2026-10-24" ad="2026-10-26" dies="2026-10-26"/>
  <exemplum phrasis="mondays" canon="Mondays"
            ab="2026-10-19" ad="2026-11-02"
            dies="2026-10-19 2026-10-26 2026-11-02"/>
  <exemplum phrasis="every tuesday" canon="Tuesdays"
            ab="2026-10-19" ad="2026-10-31"
            dies="2026-10-20 2026-10-27"/>
  <exemplum phrasis="tues" canon="Tuesdays"
            ab="2026-10-19" ad="2026-10-20" dies="2026-10-20"/>
  <exemplum phrasis="mon, wed, fri" canon="Mondays, Wednesdays and Fridays"
            ab="2026-10-19" ad="2026-10-25"
            dies="2026-10-19 2026-10-21 2026-10-23"/>
  <exemplum phrasis="tuesdays and thursdays" canon="Tuesdays and Thursdays"
            ab="2026-10-19" ad="2026-10-25"
            dies="2026-10-20 2026-10-22"/>
  <exemplum canon="Weekends" ab="2026-10-19" ad="2026-10-25"
            dies="2026-10-24 2026-10-25"><@phrasis=>saturdays &amp; sundays</></exemplum>
  <exemplum phrasis="fridays" canon="Fridays"
            ab="2026-12-28" ad="2027-01-10"
            dies="2027-01-01 2027-01-08"/>
  <vitium phrasis="mondays and"><@incipit=>can't read "mondays and"</></vitium>
```

Run: `./compile_tests.sh probatio_recursus` — Expected: FAIL at
"weekdays" (`unknown word "weekdays"`).

- [ ] **Step 2: Implement**

In `lib/recursus.c`:

1. Extend `VOCABULA[]` (keep `NIHIL` last):

```c
hic_manens constans character* constans VOCABULA[] = {
    "every", "each", "day", "daily", "everyday",
    "weekday", "weekdays", "weekend", "weekends", "and",
    NIHIL
};
```

2. Add after `verbum_notum`'s helpers (before `verbum_notum`), and make
`verbum_notum` accept weekday names:

```c
/* nomina dierum Anglica, 0 = dominica */
hic_manens constans character* constans DIES_HEBDOMADIS[] = {
    "sunday", "monday", "tuesday", "wednesday", "thursday", "friday",
    "saturday"
};

/* index diei (0..6) verbi t: nomen plenum, plurale, aut praefixum
 * III+ litterarum ("mon", "tues", "thurs"); -I si nullus */
interior s32
diei_index (constans character* t)
{
    i32 n;
    i32 m;
    s32 w;

    n = (i32)strlen(t);
    per (w = ZEPHYRUM; w < VII; w++)
    {
        m = (i32)strlen(DIES_HEBDOMADIS[w]);
        si (   strcmp(t, DIES_HEBDOMADIS[w]) == ZEPHYRUM
            || (n == m + I && strncmp(t, DIES_HEBDOMADIS[w], m) == ZEPHYRUM
                && t[m] == 's')
            || (n >= III && n < m
                && strncmp(t, DIES_HEBDOMADIS[w], n) == ZEPHYRUM))
        {
            redde w;
        }
    }
    redde -I;
}
```

and in `verbum_notum`, before the `VOCABULA` loop:

```c
    si (diei_index(v->textus) >= ZEPHYRUM)
    {
        redde VERUM;
    }
```

3. Readers and patterns (before `FORMAE[]`):

```c
interior b32
diem_hebdomadis_legere (
    Lector* lector,
       s32* w)
{
    Verbum* v;

    v = currens(lector);
    si (!v || (*w = diei_index(v->textus)) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    lector->locus++;
    redde VERUM;
}

/* dies { [,] [and] dies } -> mascula */
interior b32
dies_seriem_legere (
    Lector* lector,
       i32* mascula)
{
    s32 w;
    i32 servatus;

    *mascula = ZEPHYRUM;
    si (!diem_hebdomadis_legere(lector, &w))
    {
        redde FALSUM;
    }
    *mascula |= BITUS(w);
    dum (VERUM)
    {
        servatus = lector->locus;
        (vacuum)est(lector, ",");
        (vacuum)est(lector, "and");
        si (!diem_hebdomadis_legere(lector, &w))
        {
            lector->locus = servatus;
            frange;
        }
        *mascula |= BITUS(w);
    }
    redde VERUM;
}

/* "weekdays", "every weekday", "weekends", "every weekend" */
interior s32
feriae_legere (
      Lector* lector,
    Recursus* r)
{
    (vacuum)(est(lector, "every") || est(lector, "each"));
    si (est(lector, "weekdays") || est(lector, "weekday"))
    {
        r->hebdomas = FERIAE;
    }
    alioquin si (est(lector, "weekends") || est(lector, "weekend"))
    {
        r->hebdomas = FINES_HEBDOMADIS;
    }
    alioquin
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_HEBDOMAS;
    r->intervallum  = I;
    redde I;
}

/* "[every|each] <dies> [, and <dies>...]" */
interior s32
hebdomadem_legere (
      Lector* lector,
    Recursus* r)
{
    (vacuum)(est(lector, "every") || est(lector, "each"));
    si (!dies_seriem_legere(lector, &r->hebdomas))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_HEBDOMAS;
    r->intervallum  = I;
    redde I;
}
```

4. `FORMAE[]`:

```c
hic_manens constans FormaRecursus FORMAE[] = {
    cotidie_legere,
    feriae_legere,
    hebdomadem_legere,
    NIHIL
};
```

5. Canonical weekly text — add to the **Scriptio** block (after
`diem_scribere`; Task 3's `regulam_perficere` uses
`dies_seriem_scribere`):

```c
/* dies mascula, ordine a die lunae: "Monday, Wednesday and Friday"
 * (plurale: "Mondays, ...") */
interior vacuum
dies_seriem_scribere (
    ChordaAedificator* a,
                  i32  mascula,
                  b32  plurale,
                  b32  vel,
              Piscina* p)
{
    s32 k;
    s32 w;
    i32 omnes;
    i32 scripti;

    omnes = ZEPHYRUM;
    per (k = ZEPHYRUM; k < VII; k++)
    {
        si (mascula & BITUS(k))
        {
            omnes++;
        }
    }
    scripti = ZEPHYRUM;
    per (k = I; k <= VII; k++)
    {
        w = k % VII;     /* lunae .. sabbati, dominica ultima */
        si (!(mascula & BITUS(w)))
        {
            perge;
        }
        si (scripti > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, scripti + I == omnes
                ? (vel ? " or " : " and ") : ", ");
        }
        chorda_aedificator_appendere_chorda(a, fasti_nomen_diei_hebdomadis(
            w, FALSUM, FASTI_LINGUA_ANGLICA, p));
        si (plurale)
        {
            chorda_aedificator_appendere_character(a, 's');
        }
        scripti++;
    }
}

interior vacuum
hebdomadem_scribere (
    ChordaAedificator* a,
    constans Recursus* r,
              Piscina* p)
{
    character numerus[XXXII];

    si (r->intervallum <= I)
    {
        si (r->hebdomas == DIES_OMNES)
        {
            chorda_aedificator_appendere_literis(a, "Every day");
        }
        alioquin si (r->hebdomas == FERIAE)
        {
            chorda_aedificator_appendere_literis(a, "Weekdays");
        }
        alioquin si (r->hebdomas == FINES_HEBDOMADIS)
        {
            chorda_aedificator_appendere_literis(a, "Weekends");
        }
        alioquin
        {
            dies_seriem_scribere(a, r->hebdomas, VERUM, FALSUM, p);
        }
        redde;
    }
    si (r->intervallum == II)
    {
        chorda_aedificator_appendere_literis(a, "Every other ");
    }
    alioquin
    {
        sprintf(numerus, "Every %d weeks on ", (integer)r->intervallum);
        chorda_aedificator_appendere_literis(a, numerus);
    }
    dies_seriem_scribere(a, r->hebdomas, FALSUM, FALSUM, p);
}
```

and in `recursus_scribere` replace the `"Every day"` branch with a
dispatch:

```c
    commutatio (r->genus)
    {
        casus GENUS_HEBDOMAS:
            hebdomadem_scribere(a, r, piscina);
            frange;
        ordinarius:
            frange;
    }
```

(fasti's English weekday name is assumed to be "Monday" etc.; if it is
not capitalized, capitalize the first letter in `dies_seriem_scribere`.)

- [ ] **Step 3: Format, run**

Run: `./silva/formator.sh lib/recursus.c -scribere && ./compile_tests.sh probatio_recursus`
Expected: PASS (17 corpus elements).

- [ ] **Step 4: Plants, report**

- `n >= III` -> `n > V` in `diei_index` (red: "mon, wed, fri" —
  three-letter abbreviations stop matching).
- `" and "` -> `" & "` in `dies_seriem_scribere` (canon).
- `FERIAE` `0x3E` -> `0x3C` (weekdays days).

---

### Task 3: Dates, "from", intervals and anchors

**Files:**
- Modify: `lib/recursus.c` (month names, date reader with optional
  weekday check, `from` clause, two interval patterns, anchor rules,
  continet for intervals, canonical intervals)
- Modify: `lib/recursus.corpus.stml`

**Interfaces:**
- Consumes: Task 2 `diem_hebdomadis_legere`, `dies_seriem_legere`,
  `dies_seriem_scribere`, `hebdomadem_scribere`.
- Produces: `diem_legere(Lector*, Dies*)` (0/1/-1; year required),
  `mensem_legere(Lector*, s32*)`, `numerum_legere(Lector*, s32*)`,
  `ab_legere(Lector*, Recursus*)` — used by Tasks 4–5.

- [ ] **Step 1: Add corpus lines (red)**

```xml
  <exemplum phrasis="every other monday from oct 26 2026"
            canon="Every other Monday from Mon 26 Oct 2026"
            ab="2026-10-20" ad="2026-11-30"
            dies="2026-10-26 2026-11-09 2026-11-23"/>
  <exemplum phrasis="every 2 weeks on monday from 2026-10-26"
            canon="Every other Monday from Mon 26 Oct 2026"
            ab="2026-10-19" ad="2026-11-10"
            dies="2026-10-26 2026-11-09"/>
  <exemplum phrasis="every other week on tuesdays and thursdays starting oct 27, 2026"
            canon="Every other Tuesday and Thursday from Tue 27 Oct 2026"
            ab="2026-10-26" ad="2026-11-13"
            dies="2026-10-27 2026-10-29 2026-11-10 2026-11-12"/>
  <exemplum phrasis="every other week on mondays and fridays from fri 23 oct 2026"
            canon="Every other Monday and Friday from Fri 23 Oct 2026"
            ab="2026-10-19" ad="2026-11-08"
            dies="2026-10-23 2026-11-02 2026-11-06"/>
  <exemplum phrasis="every 3 weeks on friday from 23 oct 2026"
            canon="Every 3 weeks on Friday from Fri 23 Oct 2026"
            ab="2026-10-19" ad="2026-11-30"
            dies="2026-10-23 2026-11-13"/>
  <exemplum phrasis="every 3 days from 2026-10-23"
            canon="Every 3 days from Fri 23 Oct 2026"
            ab="2026-10-20" ad="2026-11-01"
            dies="2026-10-23 2026-10-26 2026-10-29 2026-11-01"/>
  <exemplum phrasis="every other day from oct 30 2026"
            canon="Every other day from Fri 30 Oct 2026"
            ab="2026-10-29" ad="2026-11-04"
            dies="2026-10-30 2026-11-01 2026-11-03"/>
  <exemplum phrasis="every 1 week on mon" canon="Mondays"
            ab="2026-10-19" ad="2026-10-20" dies="2026-10-19"/>
  <exemplum phrasis="mondays from 2026-11-01"
            canon="Mondays from Sun 1 Nov 2026"
            ab="2026-10-19" ad="2026-11-10"
            dies="2026-11-02 2026-11-09"/>
  <exemplum phrasis="daily from nov 1 2026"
            canon="Every day from Sun 1 Nov 2026"
            ab="2026-10-31" ad="2026-11-02"
            dies="2026-11-01 2026-11-02"/>
  <vitium phrasis="every other monday"
          incipit="every other monday: from which day?"/>
  <vitium phrasis="every 3 days" incipit="every 3 days: from which day?"/>
  <vitium phrasis="every other monday from oct 26"
          incipit="oct 26: add the year"/>
  <vitium phrasis="every other monday from oct 27 2026"
          incipit="every other monday from oct 27 2026: Tue 27 Oct 2026 is not a Monday"/>
  <vitium phrasis="every other monday from fri 26 oct 2026"
          incipit="fri 26 oct 2026: Mon 26 Oct 2026 is a Monday"/>
  <vitium phrasis="every 0 days from 2026-10-23"
          incipit="every 0 days from 2026-10-23: the interval must be 1 or more"/>
  <vitium phrasis="every 53 weeks on monday from oct 26 2026"
          incipit="every 53 weeks on monday from oct 26 2026: the interval can be at most 52 weeks"/>
  <vitium phrasis="every 400 days from 2026-10-23"
          incipit="every 400 days from 2026-10-23: the interval can be at most 366 days"/>
  <vitium phrasis="every other monday from 2026-02-30"
          incipit="2026-02-30: no such date"/>
```

Run tests — Expected: FAIL at the first new line (`unknown word "other"`).

- [ ] **Step 2: Implement — vocabulary and months**

`VOCABULA[]` gains:
`"other", "week", "weeks", "days", "on", "from", "starting",`.

Add:

```c
hic_manens constans character* constans MENSES[] = {
    "january", "february", "march", "april", "may", "june", "july",
    "august", "september", "october", "november", "december"
};

/* mensis 1..12 verbi t: plenum aut praefixum III+ ("sept"); 0 si nullus */
interior s32
mensis_index (constans character* t)
{
    i32 n;
    s32 m;

    n = (i32)strlen(t);
    per (m = ZEPHYRUM; m < XII; m++)
    {
        si (   strcmp(t, MENSES[m]) == ZEPHYRUM
            || (n >= III && n < (i32)strlen(MENSES[m])
                && strncmp(t, MENSES[m], n) == ZEPHYRUM))
        {
            redde m + I;
        }
    }
    redde ZEPHYRUM;
}
```

and in `verbum_notum`: `si (mensis_index(v->textus) > ZEPHYRUM) redde VERUM;`.

- [ ] **Step 3: Implement — readers**

(Ruling, Task 1: `verba_iungere` — written in Task 1's block — is added
HERE, in the **Verba** block, because `-Wunused-function` rejects it
until its first caller, `diem_legere`.)

```c
interior b32
mensem_legere (
    Lector* lector,
       s32* m)
{
    Verbum* v;

    v = currens(lector);
    si (!v || (*m = mensis_index(v->textus)) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    lector->locus++;
    redde VERUM;
}

/* numerus nudus (non ordinale, non ISO) */
interior b32
numerum_legere (
    Lector* lector,
       s32* n)
{
    Verbum* v;

    v = currens(lector);
    si (!v || !v->numerus_est || v->ordinale)
    {
        redde FALSUM;
    }
    *n = v->numerus;
    lector->locus++;
    redde VERUM;
}

/* dies cum anno: ISO | [<dies hebd.> [,]] (<mensis> <n> | <n> <mensis>)
 * [,] <annus>. 0 non dies (locus restitutus), I, -I vitium */
interior s32
diem_legere (
    Lector* lector,
      Dies* d)
{
       i32 initium;
       s32 w;
       s32 m;
       s32 n;
       s32 annus;
    Verbum* v;
    ChordaAedificator* a;

    m        = ZEPHYRUM;
    n        = ZEPHYRUM;
    initium  = lector->locus;
    v        = currens(lector);
    si (v && v->iso_malum)
    {
        lector->locus++;
        redde vitium_ponere(lector, verba_iungere(lector, initium, lector->locus),
            "no such date");
    }
    w = -I;
    si (diem_hebdomadis_legere(lector, &w))
    {
        (vacuum)est(lector, ",");
    }
    v = currens(lector);
    si (v && v->iso_est)
    {
        *d = v->iso;
        lector->locus++;
    }
    alioquin
    {
        si (mensem_legere(lector, &m))
        {
            v = currens(lector);
            si (!v || !v->numerus_est)
            {
                lector->locus = initium;
                redde ZEPHYRUM;
            }
            n = v->numerus;
            lector->locus++;
        }
        alioquin si (v && v->numerus_est)
        {
            n = v->numerus;
            lector->locus++;
            si (!mensem_legere(lector, &m))
            {
                lector->locus = initium;
                redde ZEPHYRUM;
            }
        }
        alioquin
        {
            lector->locus = initium;
            redde ZEPHYRUM;
        }
        (vacuum)est(lector, ",");
        v = currens(lector);
        si (!v || !v->numerus_est || v->ordinale || v->numerus < M)
        {
            redde vitium_ponere(lector, verba_iungere(lector, initium, lector->locus),
                "add the year");
        }
        annus = v->numerus;
        lector->locus++;
        *d = fasti_dies(annus, m, n);
        si (!fasti_dies_valida(*d))
        {
            redde vitium_ponere(lector, verba_iungere(lector, initium, lector->locus),
                "no such date");
        }
    }
    si (w >= ZEPHYRUM && fasti_dies_hebdomadis(*d) != w)
    {
        a = chorda_aedificator_creare(lector->piscina, (memoriae_index)LXIV);
        diem_scribere(a, *d, lector->piscina);
        chorda_aedificator_appendere_literis(a, " is a ");
        chorda_aedificator_appendere_chorda(a, fasti_nomen_diei_hebdomadis(
            fasti_dies_hebdomadis(*d), FALSUM, FASTI_LINGUA_ANGLICA,
            lector->piscina));
        redde vitium_ponere(lector, verba_iungere(lector, initium, lector->locus),
            chorda_ut_cstr(chorda_aedificator_finire(a), lector->piscina));
    }
    redde I;
}

/* "from|starting <dies>" -> ancora; 0 nulla, I, -I */
interior s32
ab_legere (
      Lector* lector,
    Recursus* r)
{
    s32 res;

    si (!(est(lector, "from") || est(lector, "starting")))
    {
        redde ZEPHYRUM;
    }
    res = diem_legere(lector, &r->ancora);
    si (res == ZEPHYRUM)
    {
        redde vitium_ponere(lector, lector->phrasis,
            "from which day? add \"from\" and a date with its year");
    }
    si (res > ZEPHYRUM)
    {
        r->ancoratus = VERUM;
    }
    redde res;
}
```

(`diem_scribere` is in the **Scriptio** block, above the readers.)

- [ ] **Step 4: Implement — interval patterns and anchor rules**

```c
/* "every other day", "every <n> days" */
interior s32
dies_intervallo_legere (
      Lector* lector,
    Recursus* r)
{
    s32 n;

    si (!est(lector, "every"))
    {
        redde ZEPHYRUM;
    }
    si (est(lector, "other"))
    {
        n = II;
    }
    alioquin si (!numerum_legere(lector, &n))
    {
        redde ZEPHYRUM;
    }
    si (!(est(lector, "day") || est(lector, "days")))
    {
        redde ZEPHYRUM;
    }
    r->genus        = n == I ? GENUS_HEBDOMAS : GENUS_DIES;
    r->hebdomas     = DIES_OMNES;
    r->intervallum  = n;
    redde I;
}

/* "every other <dies>", "every other week on <dies>", "every <n>
 * week(s) on <dies>" */
interior s32
hebdomadas_intervallo_legere (
      Lector* lector,
    Recursus* r)
{
    s32 n;
    b32 alter;

    si (!est(lector, "every"))
    {
        redde ZEPHYRUM;
    }
    alter = est(lector, "other");
    n     = II;
    si (!alter && !numerum_legere(lector, &n))
    {
        redde ZEPHYRUM;
    }
    si (est(lector, "week") || est(lector, "weeks"))
    {
        si (!est(lector, "on"))
        {
            redde ZEPHYRUM;
        }
    }
    alioquin si (!alter)
    {
        redde ZEPHYRUM;
    }
    si (!dies_seriem_legere(lector, &r->hebdomas))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_HEBDOMAS;
    r->intervallum  = n;
    redde I;
}
```

`FORMAE[]`:

```c
hic_manens constans FormaRecursus FORMAE[] = {
    cotidie_legere,
    dies_intervallo_legere,
    hebdomadas_intervallo_legere,
    feriae_legere,
    hebdomadem_legere,
    NIHIL
};
```

In `legere_intra`, after a pattern matched (`res > ZEPHYRUM`), read the
optional `from` clause before checking the end:

```c
        si (res > ZEPHYRUM)
        {
            res = ab_legere(lector, r);
            si (res < ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (lector->locus == lector->numerus)
            {
                redde regulam_perficere(lector, r);
            }
        }
```

`regulam_perficere` becomes:

```c
interior b32
regulam_perficere (
      Lector* lector,
    Recursus* r)
{
    ChordaAedificator* a;
                  b32  hebdomadae;

    hebdomadae = r->genus == GENUS_HEBDOMAS;
    si (r->intervallum < I)
    {
        (vacuum)vitium_ponere(lector, lector->phrasis,
            "the interval must be 1 or more");
        redde FALSUM;
    }
    si (hebdomadae ? r->intervallum > LII : r->intervallum > CCCLXVI)
    {
        (vacuum)vitium_ponere(lector, lector->phrasis, hebdomadae
            ? "the interval can be at most 52 weeks"
            : "the interval can be at most 366 days");
        redde FALSUM;
    }
    si (r->intervallum > I && !r->ancoratus)
    {
        (vacuum)vitium_ponere(lector, lector->phrasis,
            "from which day? add \"from\" and a date with its year");
        redde FALSUM;
    }
    si (r->intervallum > I && !recursus_continet(r, r->ancora))
    {
        a = chorda_aedificator_creare(lector->piscina, (memoriae_index)LXIV);
        diem_scribere(a, r->ancora, lector->piscina);
        chorda_aedificator_appendere_literis(a, " is not a ");
        dies_seriem_scribere(a, r->hebdomas, FALSUM, VERUM, lector->piscina);
        (vacuum)vitium_ponere(lector, lector->phrasis, chorda_ut_cstr(
            chorda_aedificator_finire(a), lector->piscina));
        redde FALSUM;
    }
    redde VERUM;
}
```

Note the "every 3 days" no-anchor error: the phrase is the whole typed
phrase, so the incipit `every 3 days: from which day?` matches.

- [ ] **Step 5: Implement — evaluation and canonical form**

`recursus_continet`'s switch:

```c
    commutatio (r->genus)
    {
        casus GENUS_HEBDOMAS:
            si (!(r->hebdomas & BITUS(fasti_dies_hebdomadis(d))))
            {
                redde FALSUM;
            }
            redde r->intervallum <= I
                || ((dominica(d) - dominica(r->ancora)) / VII)
                       % (s64)r->intervallum == ZEPHYRUM;
        casus GENUS_DIES:
            redde (fasti_ad_iulianum(d) - fasti_ad_iulianum(r->ancora))
                % (s64)r->intervallum == ZEPHYRUM;
        ordinarius:
            redde FALSUM;
    }
```

with, above it:

```c
/* dies Iulianus dominicae hebdomadis diei d (hebdomades a dominica) */
interior DiesIulianus
dominica (Dies d)
{
    redde fasti_ad_iulianum(d) - (MoraDierum)fasti_dies_hebdomadis(d);
}
```

(`d >= ancora` is guaranteed by the anchor check at the top of
`recursus_continet`, so the differences are non-negative.)

`recursus_scribere`'s switch gains:

```c
        casus GENUS_DIES:
            si (r->intervallum == II)
            {
                chorda_aedificator_appendere_literis(a, "Every other day");
            }
            alioquin
            {
                character numerus[XXXII];

                sprintf(numerus, "Every %d days", (integer)r->intervallum);
                chorda_aedificator_appendere_literis(a, numerus);
            }
            frange;
```

- [ ] **Step 6: Format, run**

Expected: PASS (all exempla incl. round trips like
`every other monday from mon 26 oct 2026`).

- [ ] **Step 7: Plants, report**

- `% (s64)r->intervallum` -> `% (s64)(r->intervallum + I)` in the
  weekly branch (red: every other monday days).
- In `regulam_perficere`, drop the membership check (red: oct 27 vitium).
- In `diem_legere`, drop the whole "add the year" check (red: vitium
  `oct 26: add the year`).
- `dominica()` returns `fasti_ad_iulianum(d)` (red: every other week
  on tuesdays and thursdays).

---

### Task 4: Monthly schedules

**Files:**
- Modify: `lib/recursus.c`, `lib/recursus.corpus.stml`

**Interfaces:**
- Consumes: Tasks 1–3 readers.
- Produces: `GENUS_MENSIS_DIES`, `GENUS_MENSIS_ORDO` evaluation and text.

- [ ] **Step 1: Add corpus lines (red)**

```xml
  <exemplum phrasis="the 1st of the month" canon="The 1st of each month"
            ab="2026-10-15" ad="2026-12-01"
            dies="2026-11-01 2026-12-01"/>
  <exemplum phrasis="monthly on the 15th" canon="The 15th of each month"
            ab="2026-10-01" ad="2026-11-30"
            dies="2026-10-15 2026-11-15"/>
  <exemplum phrasis="the 31st of every month" canon="The 31st of each month"
            ab="2026-09-01" ad="2026-12-31"
            dies="2026-10-31 2026-12-31"/>
  <exemplum phrasis="last day of the month" canon="The last day of each month"
            ab="2026-10-15" ad="2027-02-28"
            dies="2026-10-31 2026-11-30 2026-12-31 2027-01-31 2027-02-28"/>
  <exemplum phrasis="last day of the month" canon="The last day of each month"
            ab="2028-02-01" ad="2028-03-31"
            dies="2028-02-29 2028-03-31"/>
  <exemplum phrasis="first monday of the month"
            canon="The first Monday of each month"
            ab="2026-10-01" ad="2026-11-30"
            dies="2026-10-05 2026-11-02"/>
  <exemplum phrasis="the last friday of each month"
            canon="The last Friday of each month"
            ab="2026-10-01" ad="2026-11-30"
            dies="2026-10-30 2026-11-27"/>
  <exemplum phrasis="last friday of the month"
            canon="The last Friday of each month"
            ab="2026-12-01" ad="2026-12-31" dies="2026-12-25"/>
  <exemplum phrasis="third thursday of the month"
            canon="The third Thursday of each month"
            ab="2026-10-01" ad="2026-10-31" dies="2026-10-15"/>
  <exemplum phrasis="2nd tuesday of every month"
            canon="The second Tuesday of each month"
            ab="2026-10-01" ad="2026-10-31" dies="2026-10-13"/>
  <vitium phrasis="the 32nd of the month"
          incipit="32nd: a month has at most 31 days"/>
  <vitium phrasis="5th monday of the month"><@incipit=>5th: at most the fourth (or "last")</></vitium>
  <vitium phrasis="fifth monday of the month" incipit="unknown word"/>
```

- [ ] **Step 2: Implement**

`VOCABULA[]` gains: `"the", "of", "month", "monthly", "last", "first",
"second", "third", "fourth",`.

```c
/* "of (the|each|every) month" */
interior b32
mensis_finem_legere (Lector* lector)
{
    redde est(lector, "of") && (est(lector, "the") || est(lector, "each")
        || est(lector, "every")) && est(lector, "month");
}

/* dies mensis: numerus (nudus aut ordinale) -> n; -I vitium si > 31 */
interior s32
diem_mensis_legere (
    Lector* lector,
       s32* n)
{
    Verbum* v;

    v = currens(lector);
    si (!v || !v->numerus_est)
    {
        redde ZEPHYRUM;
    }
    lector->locus++;
    si (v->numerus < I || v->numerus > XXXI)
    {
        redde vitium_ponere(lector, chorda_ex_literis(v->textus, lector->piscina),
            "a month has at most 31 days");
    }
    *n = v->numerus;
    redde I;
}

/* "(the) <n>th of ... month", "monthly on (the) <n>th", "(the) last
 * day of ... month" */
interior s32
mensis_diem_legere (
      Lector* lector,
    Recursus* r)
{
    s32 n;
    s32 res;

    si (est(lector, "monthly"))
    {
        si (!est(lector, "on"))
        {
            redde ZEPHYRUM;
        }
        (vacuum)est(lector, "the");
        res = diem_mensis_legere(lector, &n);
        si (res <= ZEPHYRUM)
        {
            redde res;
        }
    }
    alioquin
    {
        (vacuum)est(lector, "the");
        si (est(lector, "last"))
        {
            si (!est(lector, "day"))
            {
                redde ZEPHYRUM;
            }
            n = -I;
        }
        alioquin
        {
            res = diem_mensis_legere(lector, &n);
            si (res <= ZEPHYRUM)
            {
                redde res;
            }
        }
        si (!mensis_finem_legere(lector))
        {
            redde ZEPHYRUM;
        }
    }
    r->genus        = GENUS_MENSIS_DIES;
    r->intervallum  = I;
    r->dies_mensis  = n;
    redde I;
}

/* ordo: first..fourth, last, 1st..4th -> I..IV, -I */
interior s32
ordinem_legere (
    Lector* lector,
       s32* o)
{
    Verbum* v;

    si (est(lector, "first"))       { *o = I;   redde I; }
    si (est(lector, "second"))      { *o = II;  redde I; }
    si (est(lector, "third"))       { *o = III; redde I; }
    si (est(lector, "fourth"))      { *o = IV;  redde I; }
    si (est(lector, "last"))        { *o = -I;  redde I; }
    v = currens(lector);
    si (!v || !v->ordinale)
    {
        redde ZEPHYRUM;
    }
    lector->locus++;
    si (v->numerus < I || v->numerus > IV)
    {
        redde vitium_ponere(lector, chorda_ex_literis(v->textus, lector->piscina),
            "at most the fourth (or \"last\")");
    }
    *o = v->numerus;
    redde I;
}

/* "(the) <ordo> <dies> of (the|each|every) month" */
interior s32
mensis_ordinem_legere (
      Lector* lector,
    Recursus* r)
{
    s32 o;
    s32 w;
    s32 res;

    (vacuum)est(lector, "the");
    res = ordinem_legere(lector, &o);
    si (res <= ZEPHYRUM)
    {
        redde res;
    }
    si (!diem_hebdomadis_legere(lector, &w) || !mensis_finem_legere(lector))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_MENSIS_ORDO;
    r->intervallum  = I;
    r->ordo         = o;
    r->hebdomas     = BITUS(w);
    redde I;
}
```

Note: the formatter will expand the one-line `si` blocks of
`ordinem_legere` into house style; write them expanded if preferred.
Order matters: `mensis_diem_legere` comes BEFORE `mensis_ordinem_legere`
in `FORMAE[]`. "the 32nd of the month" commits its error in
`diem_mensis_legere` (any number > 31). "5th monday of the month" passes
`diem_mensis_legere` (5 <= 31) but then fails `mensis_finem_legere` on
"monday", so the pattern returns 0, the locus is restored, and
`mensis_ordinem_legere` reports "5th: at most the fourth".

`FORMAE[]`: insert `mensis_diem_legere, mensis_ordinem_legere,` after
`hebdomadem_legere`.

Evaluation (`recursus_continet`):

```c
        casus GENUS_MENSIS_DIES:
            redde d.dies == (r->dies_mensis < ZEPHYRUM
                ? fasti_dies_in_mense(d.mensis, d.annus) : r->dies_mensis);
        casus GENUS_MENSIS_ORDO:
            si (!(r->hebdomas & BITUS(fasti_dies_hebdomadis(d))))
            {
                redde FALSUM;
            }
            redde r->ordo < ZEPHYRUM
                ? d.dies + VII > fasti_dies_in_mense(d.mensis, d.annus)
                : (d.dies - I) / VII + I == r->ordo;
```

Canonical (`recursus_scribere`):

```c
        casus GENUS_MENSIS_DIES:
            si (r->dies_mensis < ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a,
                    "The last day of each month");
            }
            alioquin
            {
                character numerus[XXXII];

                sprintf(numerus, "The %d%s of each month",
                    (integer)r->dies_mensis,
                    fasti_suffixum_ordinale(r->dies_mensis));
                chorda_aedificator_appendere_literis(a, numerus);
            }
            frange;
        casus GENUS_MENSIS_ORDO:
            {
                hic_manens constans character* constans ORDINES[] = {
                    "", "first", "second", "third", "fourth"
                };

                chorda_aedificator_appendere_literis(a, "The ");
                chorda_aedificator_appendere_literis(a, r->ordo < ZEPHYRUM
                    ? "last" : ORDINES[r->ordo]);
                chorda_aedificator_appendere_character(a, ' ');
                dies_seriem_scribere(a, r->hebdomas, FALSUM, FALSUM,
                    piscina);
                chorda_aedificator_appendere_literis(a, " of each month");
            }
            frange;
```

- [ ] **Step 3: Format, run** — Expected: PASS.

- [ ] **Step 4: Plants, report**

- `d.dies + VII >` -> `d.dies + VI >` (red: last friday).
- `fasti_dies_in_mense(d.mensis, d.annus)` in MENSIS_DIES ->
  `XXXI` (red: last day of the month).
- `v->numerus > XXXI` -> `v->numerus > XXXII` (red: 32nd vitium).

---

### Task 5: Yearly and one-off dates

**Files:**
- Modify: `lib/recursus.c`, `lib/recursus.corpus.stml`

**Interfaces:**
- Consumes: Task 3 `diem_legere`, `mensem_legere`.
- Produces: `GENUS_ANNUS`, `GENUS_SEMEL` evaluation and text.

- [ ] **Step 1: Add corpus lines (red)**

```xml
  <exemplum phrasis="every year on oct 14" canon="Every year on 14 October"
            ab="2026-01-01" ad="2027-12-31"
            dies="2026-10-14 2027-10-14"/>
  <exemplum phrasis="yearly on october 14th" canon="Every year on 14 October"
            ab="2026-10-14" ad="2026-10-14" dies="2026-10-14"/>
  <exemplum phrasis="14 october every year" canon="Every year on 14 October"
            ab="2026-10-01" ad="2026-10-31" dies="2026-10-14"/>
  <exemplum phrasis="every year on feb 29" canon="Every year on 29 February"
            ab="2026-01-01" ad="2028-12-31" dies="2028-02-29"/>
  <exemplum phrasis="2026-10-30" canon="Once, Fri 30 Oct 2026"
            ab="2026-10-01" ad="2026-12-31" dies="2026-10-30"/>
  <exemplum phrasis="october 30, 2026" canon="Once, Fri 30 Oct 2026"
            ab="2026-10-29" ad="2026-10-31" dies="2026-10-30"/>
  <vitium phrasis="oct 30" incipit="oct 30: add the year"/>
  <vitium phrasis="every year on feb 30" incipit="feb 30: no such date"/>
  <vitium phrasis="2026-10-30 from 2026-10-01"
          incipit="2026-10-30 from 2026-10-01: a single date can't have a start"/>
```

- [ ] **Step 2: Implement**

`VOCABULA[]` gains: `"year", "yearly", "annually", "once",`.

```c
/* mensis et dies SINE anno: "oct 14", "october 14th", "14 october";
 * locus restitutus si non */
interior b32
mensem_diem_legere (
    Lector* lector,
       s32* m,
       s32* n)
{
       i32 initium;
    Verbum* v;

    initium = lector->locus;
    si (mensem_legere(lector, m))
    {
        v = currens(lector);
        si (v && v->numerus_est)
        {
            *n = v->numerus;
            lector->locus++;
            redde VERUM;
        }
    }
    alioquin
    {
        v = currens(lector);
        si (v && v->numerus_est && !v->iso_est)
        {
            *n = v->numerus;
            lector->locus++;
            si (mensem_legere(lector, m))
            {
                redde VERUM;
            }
        }
    }
    lector->locus = initium;
    redde FALSUM;
}

interior b32
annuum_signum_legere (Lector* lector)
{
    redde (est(lector, "every") && est(lector, "year")) || est(lector, "yearly")
        || est(lector, "annually");
}

/* "every year|yearly|annually (on) <mensis dies>", "<mensis dies> every
 * year|yearly|annually" */
interior s32
annum_legere (
      Lector* lector,
    Recursus* r)
{
    s32 m;
    s32 n;
    i32 initium;

    initium = lector->locus;
    si (annuum_signum_legere(lector))
    {
        (vacuum)est(lector, "on");
        initium = lector->locus;
        si (!mensem_diem_legere(lector, &m, &n))
        {
            redde ZEPHYRUM;
        }
    }
    alioquin
    {
        lector->locus = initium;
        si (!mensem_diem_legere(lector, &m, &n))
        {
            redde ZEPHYRUM;
        }
        si (!annuum_signum_legere(lector))
        {
            redde ZEPHYRUM;
        }
    }
    /* anno bissextili mensura maxima (29 Feb licet) */
    si (n < I || n > fasti_dies_in_mense(m, MMXXVIII))
    {
        redde vitium_ponere(lector, verba_iungere(lector, initium, initium + II),
            "no such date");
    }
    r->genus        = GENUS_ANNUS;
    r->intervallum  = I;
    r->mensis       = m;
    r->dies_mensis  = n;
    redde I;
}

/* "[once [,]] <dies cum anno>" */
interior s32
semel_legere (
      Lector* lector,
    Recursus* r)
{
    s32 res;

    si (est(lector, "once"))
    {
        (vacuum)est(lector, ",");
    }
    res = diem_legere(lector, &r->semel);
    si (res <= ZEPHYRUM)
    {
        redde res;
    }
    r->genus        = GENUS_SEMEL;
    r->intervallum  = I;
    redde I;
}
```

`FORMAE[]`: append `annum_legere, semel_legere,` (in this order, after
the monthly patterns — "oct 30" must reach `semel_legere`'s year error
only after `annum_legere` declined it).

`regulam_perficere`, first check:

```c
    si (r->genus == GENUS_SEMEL && r->ancoratus)
    {
        (vacuum)vitium_ponere(lector, lector->phrasis,
            "a single date can't have a start");
        redde FALSUM;
    }
```

Evaluation:

```c
        casus GENUS_ANNUS:
            redde d.mensis == r->mensis && d.dies == r->dies_mensis;
        casus GENUS_SEMEL:
            redde fasti_aequalis(d, r->semel);
```

Canonical:

```c
        casus GENUS_ANNUS:
            {
                character numerus[XXXII];

                sprintf(numerus, "Every year on %d ",
                    (integer)r->dies_mensis);
                chorda_aedificator_appendere_literis(a, numerus);
                chorda_aedificator_appendere_chorda(a, fasti_nomen_mensis(
                    r->mensis, FALSUM, FASTI_LINGUA_ANGLICA, piscina));
            }
            frange;
        casus GENUS_SEMEL:
            chorda_aedificator_appendere_literis(a, "Once, ");
            diem_scribere(a, r->semel, piscina);
            frange;
```

- [ ] **Step 3: Format, run** — Expected: PASS.

- [ ] **Step 4: Plants, report**

- `fasti_dies_in_mense(m, MMXXVIII)` -> `fasti_dies_in_mense(m,
  MMXXVII)` (red: feb 29 exemplum).
- Drop the SEMEL `ancoratus` check (red: the from vitium).
- `"Once, "` -> `"Once "` (red: canon).

---

### Task 6: API edges, worklog, commit

**Files:**
- Modify: `probationes/probatio_recursus.c` (section `api`)
- Create: `lib/recursus.worklog.md`
- Modify: `project-specs/pensa-spec.md` (status: K1 done)

**Interfaces:**
- Consumes: the public API only.

- [ ] **Step 1: Write the API section (Review Focus 3–5)**

Add before `SECTIONES` and register `{ "api", _sectio_api, NIHIL, NIHIL,
NIHIL, NIHIL },`:

```c
interior Recursus*
regula (
    constans character* phrasis,
               Piscina* p)
{
    redde recursus_legere(chorda_ex_literis(phrasis, p), p, NIHIL);
}

interior b32
dies_est (
                 Dies  d,
    constans character* iso,
              Piscina* p)
{
    redde chorda_aequalis_literis(fasti_formare_diem(d, FASTI_FORMA_ISO,
        p), iso);
}

interior vacuum
_sectio_api (
    CredoContextus* c)
{
    Recursus* r;
         Dies  d;
          Xar* l;
       chorda  vitium;
    character  longa[CCC];
          i32  i;

    CREDO_GRADUS("proximus: post ancoram, ante ancoram, semel praeteritum");
    r = regula("every other monday from oct 26 2026", c->piscina);
    CREDO_NECESSE(r != NIHIL);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXVII), &d));
    CREDO_VERUM(dies_est(d, "2026-11-09", c->piscina));
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, I), &d));
    CREDO_VERUM(dies_est(d, "2026-10-26", c->piscina));
    r = regula("2026-10-30", c->piscina);
    CREDO_FALSUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXXI), &d));

    CREDO_GRADUS("ancora longinqua: phasis servatur");
    r = regula("every other monday from jan 1 1990", c->piscina);
    CREDO_NECESSE(r != NIHIL);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXVI), &d));
    CREDO_VERUM(dies_est(d, "2026-11-02", c->piscina));

    CREDO_GRADUS("29 Feb trans annum 2100 (non bissextilem)");
    r = regula("every year on feb 29", c->piscina);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXCVII, III, I), &d));
    CREDO_VERUM(dies_est(d, "2104-02-29", c->piscina));

    CREDO_GRADUS("continet");
    r = regula("weekends", c->piscina);
    CREDO_VERUM(recursus_continet(r, fasti_dies(MMXXVI, X, XXIV)));
    CREDO_FALSUM(recursus_continet(r, fasti_dies(MMXXVI, X, XXVI)));
    CREDO_FALSUM(recursus_continet(r, fasti_dies(MMXXVI, II, XXIX)));

    CREDO_GRADUS("NIHIL et fenestra inversa");
    l = xar_creare(c->piscina, (i32)magnitudo(Dies));
    CREDO_FALSUM(recursus_continet(NIHIL, fasti_dies(MMXXVI, X, XXIV)));
    CREDO_AEQUALIS_I32(recursus_dies(NIHIL, fasti_dies(MMXXVI, X, I),
        fasti_dies(MMXXVI, X, XXXI), l), ZEPHYRUM);
    CREDO_AEQUALIS_I32(recursus_dies(r, fasti_dies(MMXXVI, X, XXXI),
        fasti_dies(MMXXVI, X, I), l), ZEPHYRUM);
    CREDO_FALSUM(recursus_proximus(NIHIL, fasti_dies(MMXXVI, X, I), &d));
    CREDO_VERUM(recursus_scribere(NIHIL, c->piscina).mensura == ZEPHYRUM);
    CREDO_VERUM(recursus_legere(chorda_ex_literis("", c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(chorda_aequalis_literis(vitium, "empty schedule"));
    CREDO_VERUM(recursus_legere(chorda_ex_literis("daily", c->piscina),
        NIHIL, &vitium) == NIHIL);

    CREDO_GRADUS("phrasis longa: vitium nominatum, nullus exitus");
    memset(longa, ZEPHYRUM, magnitudo(longa));
    per (i = ZEPHYRUM; i < LXX; i++)
    {
        strcat(longa, "mon ");
    }
    CREDO_VERUM(recursus_legere(chorda_ex_literis(longa, c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(strstr(chorda_ut_cstr(vitium, c->piscina),
        "schedule too long") != NIHIL);
    memset(longa, 'x', XL);
    longa[XL] = '\0';
    CREDO_VERUM(recursus_legere(chorda_ex_literis(longa, c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(strstr(chorda_ut_cstr(vitium, c->piscina),
        "word too long") != NIHIL);
}
```

(`MMXCVII` = 2097, `MMXXVI` = 2026: every numeral 0–3999 is a
`latina.h` macro. "too long" errors carry the phrase as prefix, hence
`strstr`; "empty schedule" has no prefix.)

- [ ] **Step 2: Run** — `./compile_tests.sh probatio_recursus`
Expected: PASS (both sections).

- [ ] **Step 3: Plants**

- `PROXIMI_LIMES` `(CCCLXVI * VIII)` -> `(CCCLXVI * VI)` (red: 2104).
- `VERBA_MAXIMA` guard: in `verba_secare`, `>=` -> `>` (must not
  overflow: red or ASan-style crash; if it crashes rather than goes red,
  revert and plant `"schedule too long"` text instead).
- `recursus_dies` with inverted window: plant `j <= finis` -> `j !=
  finis` (red/hang guard: the window test must FAIL, not hang — if it
  hangs, the plant is too strong; use `CREDO_NON_PENDET` per memory).

- [ ] **Step 4: Worklog and spec status**

`lib/recursus.worklog.md` — dated entry: grammar shape (ordered
patterns, 0/1/-1), the corpus/round-trip contract, the anchor-year
rule, the interval caps and why (8-year search), the spec's oct 27
error caught by the independent date check, and how to teach a new
phrase (add a corpus line first).

`project-specs/pensa-spec.md` §VII: K1 row -> "DONE <date> <sha>".

- [ ] **Step 5: Lint, then commit (on Fran's word)**

```bash
VOCABULA_VIAE_ADDITAE="include/recursus.h lib/recursus.c probationes/probatio_recursus.c" ./oratio/vocabula.sh -nova
```
Expected: `NOVA 0`; rename any flagged identifier to a known form.

Commit via `silva.commissio` (detached, gates `radix generata aedilis`,
`sine_fabrica` as in prior secunda commits) with paths:
`include/recursus.h lib/recursus.c lib/recursus.corpus.stml
lib/recursus.worklog.md probationes/probatio_recursus.c
compile_tests_fontes_generata.sh project-specs/pensa-spec.md
project-specs/pensa-k1-plan.md` (+ `unitates.stml` if Task 1 Step 7
changed it).
