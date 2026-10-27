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
      Verbum  verba[VERBA_MAXIMA];
         i32  numerus;
         i32  locus;
     Piscina* piscina;
      chorda  phrasis;     /* purgata (spatia extrema dempta) */
      chorda  vitium;
} Lector;

nomen s32 (*FormaRecursus)(Lector* lector, Recursus* r);

/* verba nota (Task 2-5 addunt) */
hic_manens constans character* constans VOCABULA[] = {
    "every", "each", "day", "daily", "everyday",
    "weekday", "weekdays", "weekend", "weekends", "and",
    "other", "week", "weeks", "days", "on", "from", "starting",
    "the", "of", "month", "monthly", "last", "first", "second", "third",
    "fourth", "year", "yearly", "annually", "once",
    NIHIL
};


/* ==================================================
 * Verba
 * ================================================== */

interior b32
cifra_est (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
littera_est (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

interior character
minuscula (
    character c)
{
    redde (c >= 'A' && c <= 'Z') ? (character)(c - 'A' + 'a') : c;
}

/* numerus, ordinale ("15th"), ISO */
interior vacuum
verbum_classificare (
    Verbum* v)
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

    a = chorda_aedificator_creare(lector->piscina,
        (memoriae_index)LXIV);
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

    a = chorda_aedificator_creare(lector->piscina,
        (memoriae_index)LXIV);
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

    c = chorda_aedificator_creare(lector->piscina,
        (memoriae_index)XXXII);
    per (i = a; i < b && i < lector->numerus; i++)
    {
        si (i > a)
        {
            chorda_aedificator_appendere_character(c, ' ');
        }
        chorda_aedificator_appendere_literis(c,
            lector->verba[i].textus);
    }
    redde chorda_aedificator_finire(c);
}

/* phrasis -> verba (minusculis; ',' verbum suum; '&' = "and"; '.' et
 * spatia separant) */
interior b32
verba_secare (
    Lector* lector)
{
          i32  i;
          i32  n;
       Verbum* v;
    character  c;

    lector->numerus  = ZEPHYRUM;
    i                = ZEPHYRUM;
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
            (vacuum)vitium_ponere(lector, lector->phrasis,
                "schedule too long");
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
                (vacuum)vitium_ponere(lector, lector->phrasis,
                    "word too long");
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
currens (
    Lector* lector)
{
    redde lector->locus
        < lector->numerus ? &lector->verba[lector->locus] : NIHIL;
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

/* nomina dierum Anglica, 0 = dominica */
hic_manens constans character* constans DIES_HEBDOMADIS[] = {
    "sunday", "monday", "tuesday", "wednesday", "thursday", "friday",
    "saturday"
};

/* index diei (0..6) verbi t: nomen plenum, plurale, aut praefixum
 * III+ litterarum ("mon", "tues", "thurs"); -I si nullus */
interior s32
diei_index (
    constans character* t)
{
    i32 n;
    i32 m;
    s32 w;

    n = (i32)strlen(t);
    per (w = ZEPHYRUM; w < VII; w++)
    {
        m = (i32)strlen(DIES_HEBDOMADIS[w]);
        si (   strcmp(t, DIES_HEBDOMADIS[w]) == ZEPHYRUM
            || (n == m + I
                && strncmp(t, DIES_HEBDOMADIS[w], m) == ZEPHYRUM
                && t[m] == 's')
            || (n >= III && n < m
                && strncmp(t, DIES_HEBDOMADIS[w], n) == ZEPHYRUM))
        {
            redde w;
        }
    }
    redde -I;
}

hic_manens constans character* constans MENSES[] = {
    "january", "february", "march", "april", "may", "june", "july",
    "august", "september", "october", "november", "december"
};

/* mensis 1..12 verbi t: plenum aut praefixum III+ ("sept"); 0 si nullus */
interior s32
mensis_index (
    constans character* t)
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

interior b32
verbum_notum (
    constans Verbum* v)
{
    i32 i;

    si (   v->numerus_est || v->iso_est || v->iso_malum
        || strcmp(v->textus, ",") == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (diei_index(v->textus) >= ZEPHYRUM)
    {
        redde VERUM;
    }
    si (mensis_index(v->textus) > ZEPHYRUM)
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
        chorda_aedificator_appendere_chorda(a,
            fasti_nomen_diei_hebdomadis(
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
          || ((est(lector, "every") || est(lector, "each"))
              && est(lector, "day"))))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_HEBDOMAS;
    r->intervallum  = I;
    r->hebdomas     = DIES_OMNES;
    redde I;
}

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
                  i32  initium;
                  s32  w;
                  s32  m;
                  s32  n;
                  s32  annus;
               Verbum* v;
    ChordaAedificator* a;

    m        = ZEPHYRUM;
    n        = ZEPHYRUM;
    initium  = lector->locus;
    v        = currens(lector);
    si (v && v->iso_malum)
    {
        lector->locus++;
        redde vitium_ponere(lector, verba_iungere(lector, initium,
            lector->locus),
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
            redde vitium_ponere(lector, verba_iungere(lector, initium,
                lector->locus),
                "add the year");
        }
        annus = v->numerus;
        lector->locus++;
        *d = fasti_dies(annus, m, n);
        si (!fasti_dies_valida(*d))
        {
            redde vitium_ponere(lector, verba_iungere(lector, initium,
                lector->locus),
                "no such date");
        }
    }
    si (w >= ZEPHYRUM && fasti_dies_hebdomadis(*d) != w)
    {
        a = chorda_aedificator_creare(lector->piscina,
            (memoriae_index)LXIV);
        diem_scribere(a, *d, lector->piscina);
        chorda_aedificator_appendere_literis(a, " is a ");
        chorda_aedificator_appendere_chorda(a,
            fasti_nomen_diei_hebdomadis(
            fasti_dies_hebdomadis(*d), FALSUM, FASTI_LINGUA_ANGLICA,
            lector->piscina));
        redde vitium_ponere(lector, verba_iungere(lector, initium,
            lector->locus),
            chorda_ut_cstr(chorda_aedificator_finire(a),
            lector->piscina));
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
    alter  = est(lector, "other");
    n      = II;
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

/* "of (the|each|every) month" */
interior b32
mensis_finem_legere (
    Lector* lector)
{
    redde est(lector, "of")
        && (est(lector, "the") || est(lector, "each")
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
        redde vitium_ponere(lector, chorda_ex_literis(v->textus,
            lector->piscina),
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

    si (est(lector, "first"))
    { *o = I;   redde I;
    }
    si (est(lector, "second"))
    { *o = II;  redde I;
    }
    si (est(lector, "third"))
    { *o = III; redde I;
    }
    si (est(lector, "fourth"))
    { *o = IV;  redde I;
    }
    si (est(lector, "last"))
    { *o = -I;  redde I;
    }
    v = currens(lector);
    si (!v || !v->ordinale)
    {
        redde ZEPHYRUM;
    }
    lector->locus++;
    si (v->numerus < I || v->numerus > IV)
    {
        redde vitium_ponere(lector, chorda_ex_literis(v->textus,
            lector->piscina),
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
    si (   !diem_hebdomadis_legere(lector, &w)
        || !mensis_finem_legere(lector))
    {
        redde ZEPHYRUM;
    }
    r->genus        = GENUS_MENSIS_ORDO;
    r->intervallum  = I;
    r->ordo         = o;
    r->hebdomas     = BITUS(w);
    redde I;
}

/* mensis et dies SINE anno: "oct 14", "october 14th", "14 october";
 * locus restitutus si non */
interior b32
mensem_diem_legere (
    Lector* lector,
       s32* m,
       s32* n)
{
       i32  initium;
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
annuum_signum_legere (
    Lector* lector)
{
    redde (est(lector, "every") && est(lector, "year"))
        || est(lector, "yearly")
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
        redde vitium_ponere(lector, verba_iungere(lector, initium,
            initium + II),
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

hic_manens constans FormaRecursus FORMAE[] = {
    cotidie_legere,
    dies_intervallo_legere,
    hebdomadas_intervallo_legere,
    feriae_legere,
    hebdomadem_legere,
    mensis_diem_legere,
    mensis_ordinem_legere,
    annum_legere,
    semel_legere,
    NIHIL
};

interior b32
regulam_perficere (
      Lector* lector,
    Recursus* r)
{
    ChordaAedificator* a;
                  b32  hebdomadae;

    hebdomadae = r->genus == GENUS_HEBDOMAS;
    si (r->genus == GENUS_SEMEL && r->ancoratus)
    {
        (vacuum)vitium_ponere(lector, lector->phrasis,
            "a single date can't have a start");
        redde FALSUM;
    }
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
        a = chorda_aedificator_creare(lector->piscina,
            (memoriae_index)LXIV);
        diem_scribere(a, r->ancora, lector->piscina);
        chorda_aedificator_appendere_literis(a, " is not a ");
        dies_seriem_scribere(a, r->hebdomas, FALSUM, VERUM,
            lector->piscina);
        (vacuum)vitium_ponere(lector, lector->phrasis, chorda_ut_cstr(
            chorda_aedificator_finire(a), lector->piscina));
        redde FALSUM;
    }
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
        lector->vitium = chorda_ex_literis("empty schedule",
            lector->piscina);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < lector->numerus; i++)
    {
        si (!verbum_notum(&lector->verba[i]))
        {
            (vacuum)vitium_citatum(lector, "unknown word",
                chorda_ex_literis(lector->verba[i].textus,
                lector->piscina));
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; FORMAE[k]; k++)
    {
        memset(r, ZEPHYRUM, magnitudo(Recursus));
        lector->locus  = ZEPHYRUM;
        res            = FORMAE[k](lector, r);
        si (res < ZEPHYRUM)
        {
            redde FALSUM;
        }
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
    dum (   phrasis.mensura > ZEPHYRUM
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

/* dies Iulianus dominicae hebdomadis diei d (hebdomades a dominica) */
interior DiesIulianus
dominica (
    Dies d)
{
    redde fasti_ad_iulianum(d) - (MoraDierum)fasti_dies_hebdomadis(d);
}

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
        casus GENUS_MENSIS_DIES:
            redde d.dies == (r->dies_mensis < ZEPHYRUM
                ? fasti_dies_in_mense(d.mensis,
                d.annus) : r->dies_mensis);
        casus GENUS_MENSIS_ORDO:
            si (!(r->hebdomas & BITUS(fasti_dies_hebdomadis(d))))
            {
                redde FALSUM;
            }
            redde r->ordo < ZEPHYRUM
                ? d.dies + VII > fasti_dies_in_mense(d.mensis, d.annus)
                : (d.dies - I) / VII + I == r->ordo;
        casus GENUS_ANNUS:
            redde d.mensis == r->mensis && d.dies == r->dies_mensis;
        casus GENUS_SEMEL:
            redde fasti_aequalis(d, r->semel);
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
    commutatio (r->genus)
    {
        casus GENUS_HEBDOMAS:
            hebdomadem_scribere(a, r, piscina);
            frange;
        casus GENUS_DIES:
            si (r->intervallum == II)
            {
                chorda_aedificator_appendere_literis(a,
                    "Every other day");
            }
            alioquin
            {
                character numerus[XXXII];

                sprintf(numerus, "Every %d days",
                    (integer)r->intervallum);
                chorda_aedificator_appendere_literis(a, numerus);
            }
            frange;
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
                chorda_aedificator_appendere_literis(a, r->ordo
                    < ZEPHYRUM
                    ? "last" : ORDINES[r->ordo]);
                chorda_aedificator_appendere_character(a, ' ');
                dies_seriem_scribere(a, r->hebdomas, FALSUM, FALSUM,
                    piscina);
                chorda_aedificator_appendere_literis(a,
                    " of each month");
            }
            frange;
        casus GENUS_ANNUS:
            {
                character numerus[XXXII];

                sprintf(numerus, "Every year on %d ",
                    (integer)r->dies_mensis);
                chorda_aedificator_appendere_literis(a, numerus);
                chorda_aedificator_appendere_chorda(a,
                    fasti_nomen_mensis(
                    r->mensis, FALSUM, FASTI_LINGUA_ANGLICA, piscina));
            }
            frange;
        casus GENUS_SEMEL:
            chorda_aedificator_appendere_literis(a, "Once, ");
            diem_scribere(a, r->semel, piscina);
            frange;
        ordinarius:
            frange;
    }
    si (r->ancoratus)
    {
        chorda_aedificator_appendere_literis(a, " from ");
        diem_scribere(a, r->ancora, piscina);
    }
    redde chorda_aedificator_finire(a);
}
