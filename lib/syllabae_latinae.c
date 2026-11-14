/* syllabae_latinae.c - verbum Latinum -> syllabae et accentus
 *
 * Regulae in capite (include/syllabae_latinae.h); mensura in
 * lib/syllabae_latinae.worklog.md. Gradus: litterae (octeti ->
 * Littera, signa combinata litterae priori adhaerent), vocales
 * (regulae V), nuclei (diphthongi), limites (cogens, praefixum,
 * caterva longissima incipiens), spatia.
 */

#include "syllabae_latinae.h"
#include "utf8.h"
#include <string.h>

/* verba brevia in acervo; longiora in piscina */
#define LITTERAE_LOCALES  LXIV


/* ==================================================
 * Litterae
 * ================================================== */

nomen structura {
          i32 initium;    /* octetus primus */
          i32 finis;      /* post signa combinata */
    character basis;      /* 'a'..'z'; 'A' = æ, 'O' = œ */
          b32 acutus;
          b32 diaeresis;
          b32 cogit;      /* '-' cogens ante hanc litteram */
          b32 vocalis;    /* post regulas V */
} Littera;

/* U+00C0..U+00FF: basis per indicem (cp & 0x1F), '.' = non littera */
hic_manens constans character BASES_LATINAE[] =
    "aaaaaaAceeeeiiii.nooooo.ouuuuy.y";

/* codepoint -> basis (0 si non littera), acutus, diaeresis */
interior character
_basis (
    s32  cp,
    b32* acutus,
    b32* diaeresis)
{
    i32 x;

    *acutus     = FALSUM;
    *diaeresis  = FALSUM;
    si (cp >= 'a' && cp <= 'z')
    {
        redde (character)cp;
    }
    si (cp >= 'A' && cp <= 'Z')
    {
        redde (character)(cp - 'A' + 'a');
    }
    si (cp >= 0xC0 && cp <= 0xFF)
    {
        x = (i32)(cp & 0x1F);
        si (BASES_LATINAE[x] == '.' || (cp <= 0xDF && x >= 0x1E))
        {
            redde 0;
        }
        *acutus = x == 0x01 || x == 0x09 || x == 0x0D || x == 0x13
            || x == 0x1A || x == 0x1D;
        *diaeresis = x == 0x04 || x == 0x0B || x == 0x0F || x == 0x16
            || x == 0x1C || cp == 0xFF;
        redde BASES_LATINAE[x];
    }
    si (cp == 0x152 || cp == 0x153)
    {
        redde 'O';
    }
    si (cp == 0x178)
    {
        *diaeresis = VERUM;
        redde 'y';
    }
    si (cp == 0x1FC || cp == 0x1FD)
    {
        *acutus = VERUM;
        redde 'A';
    }
    redde 0;
}

interior b32
_vocalis_basis (
    character c)
{
    redde c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'
        || c == 'y' || c == 'A' || c == 'O';
}

/* octeti -> litterae; redde numerum */
interior i32
_litteras_legere (
     chorda  verbum,
    Littera* litterae)
{
    constans i8* finis    = verbum.datum + verbum.mensura;
            i32  n        = ZEPHYRUM;
            i32  positio  = ZEPHYRUM;
            s32  lineola  = -I;  /* '-' litterae proxime sequens */

    dum (positio < verbum.mensura)
    {
        constans i8* p        = verbum.datum + positio;
                i32  initium  = positio;
                s32  cp;
                b32  acutus;
                b32  diaeresis;
          character  basis;

        cp       = utf8_decodere(&p, finis);
        positio  = (i32)(p - verbum.datum);
        si (   cp >= 0x300 && cp <= 0x36F && n > ZEPHYRUM
            && litterae[n - I].finis == initium)
        {
            litterae[n - I].finis = positio;
            si (cp == 0x301)
            {
                litterae[n - I].acutus = VERUM;
            }
            si (cp == 0x308)
            {
                litterae[n - I].diaeresis = VERUM;
            }
            perge;
        }
        basis = _basis(cp, &acutus, &diaeresis);
        si (basis != 0)
        {
            litterae[n].initium    = initium;
            litterae[n].finis      = positio;
            litterae[n].basis      = basis;
            litterae[n].acutus     = acutus;
            litterae[n].diaeresis  = diaeresis;
            litterae[n].cogit     = lineola >= ZEPHYRUM
                && (s32)initium == lineola + I;
            litterae[n].vocalis   = FALSUM;
            n++;
            lineola = -I;
            perge;
        }
        lineola = (cp == '-' && n > ZEPHYRUM
            && litterae[n - I].finis == initium) ? (s32)initium : -I;
    }
    redde n;
}


/* ==================================================
 * Regulae V: vocales
 * ================================================== */

/* k et k + 1 in eadem parte (nullus '-' cogens inter eas) */
interior b32
_eadem_pars (
    constans Littera* l,
                 i32  n,
                 i32  k)
{
    redde k + I < n && !l[k + I].cogit;
}

interior vacuum
_vocales_notare (
    Littera* l,
        i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        character c = l[k].basis;
              b32 sequens_vocalis = _eadem_pars(l, n, k)
                  && _vocalis_basis(l[k + I].basis);

        l[k].vocalis = _vocalis_basis(c);
        si (!l[k].vocalis || l[k].acutus || l[k].diaeresis)
        {
            perge;
        }
        si (c == 'i')
        {
            /* initio ante vocalem (nisi ii), aut inter vocales */
            si (   (k == ZEPHYRUM || l[k].cogit)
                && sequens_vocalis && l[k + I].basis != 'i')
            {
                l[k].vocalis = FALSUM;
            }
            si (   k > ZEPHYRUM && _eadem_pars(l, n, k - I)
                && l[k - I].vocalis && sequens_vocalis)
            {
                l[k].vocalis = FALSUM;
            }
        }
        si (c == 'u')
        {
            /* qu; ngu ante vocalem */
            si (   k > ZEPHYRUM && _eadem_pars(l, n, k - I)
                && l[k - I].basis == 'q')
            {
                l[k].vocalis = FALSUM;
            }
            si (   k > I && _eadem_pars(l, n, k - I)
                && _eadem_pars(l, n, k - II) && l[k - I].basis == 'g'
                && l[k - II].basis == 'n' && sequens_vocalis)
            {
                l[k].vocalis = FALSUM;
            }
        }
    }
}


/* ==================================================
 * Partes, sceleta, verba nota
 * ================================================== */

#define NUDUM_MAXIMUM  XVI

/* litterae partis primae minusculae ('A' = æ, 'O' = œ) */
interior i32
_nudum (
    constans Littera* l,
                 i32  n,
           character* nudum)
{
    i32 k = ZEPHYRUM;

    dum (   k < n && k < NUDUM_MAXIMUM - I && (k == ZEPHYRUM
        || !l[k].cogit))
    {
        nudum[k] = l[k].basis;
        k++;
    }
    nudum[k] = '\0';
    redde k;
}

interior b32
_incipit (
    constans character* nudum,
    constans character* praefixum)
{
    redde strncmp(nudum, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* eu diphthongus: heu ceu seu neu, euge, pneu-, euchar- */
interior b32
_eu_diphthongus (
    constans character* nudum,
                   i32  k)
{
    redde (k == I && (strcmp(nudum, "heu") == ZEPHYRUM
                      || strcmp(nudum, "ceu") == ZEPHYRUM
                      || strcmp(nudum, "seu") == ZEPHYRUM
                      || strcmp(nudum, "neu") == ZEPHYRUM))
        || (k == ZEPHYRUM && (strcmp(nudum, "euge") == ZEPHYRUM
                              || _incipit(nudum, "euchar")))
        || (k == II && _incipit(nudum, "pneu"));
}

/* verba integra: limites ante litteras (indices partis primae) */
nomen structura {
    constans character* initium;
                   i32  limites[II];
                   i32  numerus;
} VerbumCompositum;

hic_manens constans VerbumCompositum COMPOSITA[] = {
    { "sicut",      { III, ZEPHYRUM }, I },
    { "etenim",     { II, ZEPHYRUM }, I },
    { "nobiscum",   { V, ZEPHYRUM }, I },
    { "vobiscum",   { V, ZEPHYRUM }, I },
    { "superexalt", { V, VII }, II },
    { NIHIL,        { ZEPHYRUM, ZEPHYRUM }, ZEPHYRUM }
};

/* praefixa ante vocalem */
hic_manens constans character* PRAEFIXA[] = {
    "ab", "ad", "ex", "in", "ob", "per", "red", "sub", "trans", NIHIL
};

/* similia non composita: regula communis */
hic_manens constans character* NON_COMPOSITA[] = {
    "abyss", "abel", "adam", "adip", "adeps", "adon", "reduc", "redux",
    "redupl", NIHIL
};

/* limites coacti (indices litterarum ante quas syllaba incipit);
 * redde numerum (<= II) */
interior i32
_limites_praefixi (
      constans Littera* l,
                   i32  n,
    constans character* nudum,
                   i32  longitudo,
                   i32* limites)
{
    i32 i;
    i32 k;

    per (i = ZEPHYRUM; COMPOSITA[i].initium != NIHIL; i++)
    {
        si (_incipit(nudum, COMPOSITA[i].initium))
        {
            per (k = ZEPHYRUM; k < COMPOSITA[i].numerus; k++)
            {
                limites[k] = COMPOSITA[i].limites[k];
            }
            redde COMPOSITA[i].numerus;
        }
    }
    per (i = ZEPHYRUM; NON_COMPOSITA[i] != NIHIL; i++)
    {
        si (_incipit(nudum, NON_COMPOSITA[i]))
        {
            redde ZEPHYRUM;
        }
    }
    per (i = ZEPHYRUM; PRAEFIXA[i] != NIHIL; i++)
    {
        k = (i32)strlen(PRAEFIXA[i]);
        si (   k < longitudo && k < n && _incipit(nudum, PRAEFIXA[i])
            && l[k].vocalis)
        {
            limites[ZEPHYRUM] = k;
            redde I;
        }
    }
    /* sus- ante consonantem (sus-cí-pe, sus-tí-ne-o) */
    si (III < longitudo && _incipit(nudum, "sus") && !l[III].vocalis)
    {
        limites[ZEPHYRUM] = III;
        redde I;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Regulae C: caterva incipiens
 * ================================================== */

/* signum consonantis: littera aut digraphus (ch ph th rh qu gu) */
nomen structura {
          i32 littera;     /* index primae */
    character prima;
    character secunda;     /* 0 = littera una */
} Signum;

interior b32
_muta (
    constans Signum* s)
{
    si (s->secunda == 'h')
    {
        redde s->prima == 'c' || s->prima == 'p' || s->prima == 't';
    }
    redde s->secunda == 0 && (s->prima == 'b' || s->prima == 'c'
        || s->prima == 'd' || s->prima == 'f' || s->prima == 'g'
        || s->prima == 'p' || s->prima == 't');
}

interior b32
_simplex (
    constans Signum* s,
          character  c)
{
    redde s->secunda == 0 && s->prima == c;
}

/* muta + liquida: + r omnes mutae; + l praeter d t th */
interior b32
_muta_liquida (
    constans Signum* a,
    constans Signum* b)
{
    si (!_muta(a))
    {
        redde FALSUM;
    }
    si (_simplex(b, 'r'))
    {
        redde VERUM;
    }
    redde _simplex(b, 'l') && !_simplex(a, 'd') && !_simplex(a, 't')
        && !(a->prima == 't' && a->secunda == 'h');
}

/* signa [a, n) syllabam incipere possunt */
interior b32
_incipere_potest (
    constans Signum* s,
                i32  a,
                i32  n)
{
    constans character* PARIA = "gnctptmnpssm";
                   i32  i;

    si (n - a == I)
    {
        redde VERUM;
    }
    si (n - a == II)
    {
        si (_muta_liquida(&s[a], &s[a + I]))
        {
            redde VERUM;
        }
        si (   _simplex(&s[a], 's') && (_simplex(&s[a + I], 'c')
            || _simplex(&s[a + I], 'p') || _simplex(&s[a + I], 't')
            || (s[a + I].prima == 'c' && s[a + I].secunda == 'h')))
        {
            redde VERUM;
        }
        per (i = ZEPHYRUM; PARIA[i] != '\0'; i += II)
        {
            si (   _simplex(&s[a], PARIA[i])
                && _simplex(&s[a + I], PARIA[i + I]))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    si (n - a == III)
    {
        redde _simplex(&s[a], 's') && (_simplex(&s[a + I], 'c')
            || _simplex(&s[a + I], 'p') || _simplex(&s[a + I], 't'))
            && (_simplex(&s[a + II], 'l') || _simplex(&s[a + II], 'r'));
    }
    redde FALSUM;
}

/* consonantes [a, b) inter nucleos -> index litterae ubi syllaba
 * sequens incipit */
interior i32
_limes_catervae (
    constans Littera* l,
                 i32  a,
                 i32  b)
{
    Signum signa[LITTERAE_LOCALES];
       i32 n = ZEPHYRUM;
       i32 k = a;
       i32 t;

    dum (k < b)
    {
        si (n == LITTERAE_LOCALES)
        {
            redde l[b - I].vocalis ? b : b - I;
        }
        signa[n].littera  = k;
        signa[n].prima    = l[k].basis;
        signa[n].secunda  = 0;
        si (   k + I < b && l[k].finis == l[k + I].initium
            && (   (l[k + I].basis == 'h' && (l[k].basis == 'c'
                    || l[k].basis == 'p' || l[k].basis == 't'
                    || l[k].basis == 'r'))
                || (l[k + I].basis == 'u' && (l[k].basis == 'q'
                    || l[k].basis == 'g'))))
        {
            signa[n].secunda = l[k + I].basis;
            k++;
        }
        n++;
        k++;
    }
    per (t = ZEPHYRUM; t < n; t++)
    {
        si (_incipere_potest(signa, t, n))
        {
            redde signa[t].littera;
        }
    }
    redde b;
}


/* ==================================================
 * Divisio
 * ================================================== */

b32
syllabae_latinae_dividere (
           chorda  verbum,
          Piscina* piscina,
    VerbumLatinum* exitus)
{
          Littera  locales[LITTERAE_LOCALES];
              i32  initia_loc[LITTERAE_LOCALES];
              i32  fines_loc[LITTERAE_LOCALES];
          Littera* l       = locales;
              i32* initia  = initia_loc;   /* nuclei: littera prima */
              i32* fines   = fines_loc;     /* nuclei: littera ultima */
        character  nudum[NUDUM_MAXIMUM];
              i32  longitudo;
              i32  limites[II];
              i32  numerus_limitum;
              i32  n;
              i32  m = ZEPHYRUM;
              i32  k;
              i32  j;

    si (exitus == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    exitus->syllabae  = NIHIL;
    exitus->numerus   = ZEPHYRUM;
    exitus->accentus  = -I;
    exitus->notatus   = FALSUM;
    si (verbum.datum == NIHIL || verbum.mensura <= ZEPHYRUM)
    {
        redde VERUM;
    }
    si (verbum.mensura > LITTERAE_LOCALES)
    {
        l = (Littera*)piscina_allocare(piscina,
            (memoriae_index)verbum.mensura * magnitudo(Littera));
        initia = (i32*)piscina_allocare(piscina,
            (memoriae_index)verbum.mensura * magnitudo(i32));
        fines = (i32*)piscina_allocare(piscina,
            (memoriae_index)verbum.mensura * magnitudo(i32));
        si (l == NIHIL || initia == NIHIL || fines == NIHIL)
        {
            redde FALSUM;
        }
    }

    n = _litteras_legere(verbum, l);
    _vocales_notare(l, n);
    longitudo = _nudum(l, n, nudum);

    /* nuclei: vocales; ae oe au (eu in verbis notis) diphthongi */
    k = ZEPHYRUM;
    dum (k < n)
    {
        si (!l[k].vocalis)
        {
            k++;
            perge;
        }
        initia[m]  = k;
        fines[m]   = k;
        si (   _eadem_pars(l, n, k) && l[k + I].vocalis
            && !l[k + I].diaeresis && l[k].finis == l[k + I].initium)
        {
            character a = l[k].basis;
            character b = l[k + I].basis;

            si (   (a == 'a' && (b == 'e' || b == 'u'))
                || (a == 'o' && b == 'e')
                || (a == 'e' && b == 'u' && k < longitudo
                    && _eu_diphthongus(nudum, k)))
            {
                fines[m] = k + I;
            }
        }
        k = fines[m] + I;
        m++;
    }
    si (m == ZEPHYRUM)
    {
        redde VERUM;
    }

    exitus->syllabae = (SyllabaLatina*)piscina_allocare(piscina,
        (memoriae_index)m * magnitudo(SyllabaLatina));
    si (exitus->syllabae == NIHIL)
    {
        redde FALSUM;
    }
    exitus->numerus = m;
    numerus_limitum = _limites_praefixi(l, n, nudum, longitudo,
        limites);

    /* limites inter nucleos */
    exitus->syllabae[ZEPHYRUM].initium = ZEPHYRUM;
    per (j = I; j < m; j++)
    {
        i32 a       = fines[j - I] + I;
        i32 b       = initia[j];
        s32 limes   = -I;
        b32 cogens  = FALSUM;
        i32 i;

        per (k = a; k <= b; k++)
        {
            si (l[k].cogit)
            {
                limes   = (s32)k;
                cogens  = VERUM;
            }
        }
        per (i = ZEPHYRUM; limes < ZEPHYRUM && i < numerus_limitum; i++)
        {
            si (limites[i] >= a && limites[i] <= b)
            {
                limes = (s32)limites[i];
            }
        }
        si (limes < ZEPHYRUM)
        {
            limes = (s32)_limes_catervae(l, a, b);
        }
        exitus->syllabae[j].initium   = l[limes].initium;
        exitus->syllabae[j - I].finis = cogens
            ? l[limes].initium - I : l[limes].initium;
    }
    exitus->syllabae[m - I].finis = verbum.mensura;

    /* accentus: acutus ultimus in nucleo; sine acuto duae -> prima */
    per (j = ZEPHYRUM; j < m; j++)
    {
        per (k = initia[j]; k <= fines[j]; k++)
        {
            si (l[k].acutus)
            {
                exitus->accentus  = (s32)j;
                exitus->notatus   = VERUM;
            }
        }
    }
    si (!exitus->notatus && m == II)
    {
        exitus->accentus = ZEPHYRUM;
    }
    redde VERUM;
}


/* ==================================================
 * Vocalis
 * ================================================== */

b32
syllabae_latinae_vocalis (
    chorda  verbum,
       i32  positio,
       i32* longitudo)
{
    constans i8* finis;
    constans i8* p;
            s32  cp;
            b32  acutus;
            b32  diaeresis;
            i32  q;

    si (verbum.datum == NIHIL || positio >= verbum.mensura)
    {
        redde FALSUM;
    }
    finis  = verbum.datum + verbum.mensura;
    p      = verbum.datum + positio;
    cp     = utf8_decodere(&p, finis);
    si (!_vocalis_basis(_basis(cp, &acutus, &diaeresis)))
    {
        redde FALSUM;
    }
    q = (i32)(p - verbum.datum);
    dum (q < verbum.mensura)
    {
        constans i8* r = verbum.datum + q;

        cp = utf8_decodere(&r, finis);
        si (cp < 0x300 || cp > 0x36F)
        {
            frange;
        }
        q = (i32)(r - verbum.datum);
    }
    si (longitudo != NIHIL)
    {
        *longitudo = q - positio;
    }
    redde VERUM;
}
