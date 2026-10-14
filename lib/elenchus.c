/* elenchus.c - menu contextuale (include/elenchus.h). Mensurae in
 * pixelis ex fonte parvo (6x8): res XVI (II cellulae, ut ordines
 * indicum), divisor VIII, margo VI utrimque, intervallum XII inter
 * textum et clavem, spatium II supra et infra, umbra II. */
#include "elenchus.h"
#include "figura.h"
#include "fons.h"
#include "chorda.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>

#define ELENCHUS_LINEA         XVI   /* res */
#define ELENCHUS_DIVISOR       VIII  /* fascia divisoris */
#define ELENCHUS_MARGO         VI    /* textus ab ore */
#define ELENCHUS_INTERVALLUM   XII   /* textus -> clavis */
#define ELENCHUS_SPATIUM       II    /* supra primam, infra ultimam */
#define ELENCHUS_UMBRA         II
#define ELENCHUS_TEXTUS_Y      IV    /* textus intra rem */

hic_manens constans character praefixum_rei[] = "elenchus.";


/* ==================================================
 * Mensurae
 * ================================================== */

/* corpus (sine umbra): latitudo et altitudo */
interior vacuum
corpus_metiri (
    constans Elenchus* e,
                  s32* latitudo,
                  s32* altitudo)
{
    s32 textus;
    s32 clavis;
    s32 n;
    i32 i;

    textus     = ZEPHYRUM;
    clavis     = ZEPHYRUM;
    *altitudo  = II * ELENCHUS_SPATIUM;
    per (i = ZEPHYRUM; e && i < e->numerus; i++)
    {
        n = e->res[i].textus ? (s32)strlen(e->res[i].textus) : ZEPHYRUM;
        si (n > textus)
        {
            textus = n;
        }
        n = e->res[i].clavis ? (s32)strlen(e->res[i].clavis) : ZEPHYRUM;
        si (n > clavis)
        {
            clavis = n;
        }
        *altitudo += ELENCHUS_LINEA + (e->res[i].divisor && i > ZEPHYRUM
            ? ELENCHUS_DIVISOR : ZEPHYRUM);
    }
    *latitudo = II * ELENCHUS_MARGO
        + textus * fons_latitudo(FONS_PARVUS)
        + (clavis > ZEPHYRUM ? ELENCHUS_INTERVALLUM + clavis
        * fons_latitudo(FONS_PARVUS) : ZEPHYRUM);
}

/* fines rei i in spatio menu (intra margines) */
interior Fines
fines_rei (
    constans Elenchus* e,
                  i32  index,
                  s32  latitudo_corporis)
{
    Fines f;
      i32 i;

    f.x         = I;
    f.y         = ELENCHUS_SPATIUM;
    f.latitudo  = latitudo_corporis - II;
    f.altitudo  = ELENCHUS_LINEA;
    per (i = ZEPHYRUM; i <= index; i++)
    {
        si (e->res[i].divisor && i > ZEPHYRUM)
        {
            f.y += ELENCHUS_DIVISOR;
        }
        si (i < index)
        {
            f.y += ELENCHUS_LINEA;
        }
    }
    redde f;
}

Fines
elenchus_fines (
    constans Elenchus* e,
              Punctum  ancora,
                Fines  area)
{
    Fines f;
      s32 latitudo;
      s32 altitudo;

    corpus_metiri(e, &latitudo, &altitudo);
    f.latitudo  = latitudo + ELENCHUS_UMBRA;
    f.altitudo  = altitudo + ELENCHUS_UMBRA;
    f.x         = ancora.x;
    f.y         = ancora.y;
    /* reflexio (Mac): sinistrorsum / sursum ab ancora */
    si (f.x + f.latitudo > area.x + area.latitudo)
    {
        f.x = ancora.x - f.latitudo;
    }
    si (f.y + f.altitudo > area.y + area.altitudo)
    {
        f.y = ancora.y - f.altitudo;
    }
    /* coactio: dexter / inferus prius, deinde sinister / superus
     * (angulus superus sinister semper visibilis) */
    si (f.x + f.latitudo > area.x + area.latitudo)
    {
        f.x = area.x + area.latitudo - f.latitudo;
    }
    si (f.x < area.x)
    {
        f.x = area.x;
    }
    si (f.y + f.altitudo > area.y + area.altitudo)
    {
        f.y = area.y + area.altitudo - f.altitudo;
    }
    si (f.y < area.y)
    {
        f.y = area.y;
    }
    redde f;
}


/* ==================================================
 * Arbor
 * ================================================== */

Componens*
elenchus_componere (
       constans Elenchus* e,
                   Fines  fines,
      constans character* actio,
                 Piscina* piscina,
     InternamentumChorda* intern)
{
     Componens* radix;
     Componens* r;
           s32  latitudo;
           s32  altitudo;
     character  id[XXXII];
           i32  i;

    si (!e || !piscina || !intern)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "elenchus",
        PARTES_DIALOGUS);
    si (!radix)
    {
        redde NIHIL;
    }
    componens_ponere_fines(radix, fines);
    corpus_metiri(e, &latitudo, &altitudo);
    per (i = ZEPHYRUM; i < e->numerus; i++)
    {
        sprintf(id, "%s%d", praefixum_rei, (integer)i);
        r = componens_creare(piscina, intern, id, PARTES_OPTIO);
        si (!r)
        {
            redde NIHIL;
        }
        componens_ponere_fines(r, fines_rei(e, i, latitudo));
        si (e->res[i].textus)
        {
            componens_ponere_titulum(r, e->res[i].textus);
        }
        si (actio)
        {
            componens_ponere_actio(r, actio);
        }
        r->claudit = e->res[i].habilis;
        componens_addere_liberum(radix, r);
    }
    redde radix;
}

s32
elenchus_index (
    constans Componens* c)
{
    chorda numerus;
       s32 index;
       i32 n;

    n = (i32)strlen(praefixum_rei);
    si (   !c || c->partes != PARTES_OPTIO || c->id.mensura <= n
        || memcmp(c->id.datum, praefixum_rei, (memoriae_index)n)
        != ZEPHYRUM)
    {
        redde -I;
    }
    numerus.datum    = c->id.datum + n;
    numerus.mensura  = c->id.mensura - n;
    redde chorda_ut_s32(numerus, &index) && index >= ZEPHYRUM ? index
        : -I;
}


/* ==================================================
 * Figura
 * ================================================== */

vacuum
elenchus_figura (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     constans Elenchus* e;
                 Fines  corpus;
                 Fines  r;
               Punctum  a;
               Punctum  b;
                   s32  latitudo;
                   s32  altitudo;
                   s32  n;
                   i32  i;

    (vacuum)thema;
    e = (constans Elenchus*)ctx;
    si (!c || !md || !e)
    {
        redde;
    }
    corpus_metiri(e, &latitudo, &altitudo);
    corpus.x         = ZEPHYRUM;
    corpus.y         = ZEPHYRUM;
    corpus.latitudo  = latitudo;
    corpus.altitudo  = altitudo;
    figura_umbram_pingere(md, corpus);
    mandata_rectangulum(md, corpus, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    mandata_rectangulum(md, corpus, figura_color_thematis(COLOR_TEXT),
        FALSUM);
    per (i = ZEPHYRUM; i < e->numerus; i++)
    {
        r = fines_rei(e, i, latitudo);
        si (e->res[i].divisor && i > ZEPHYRUM)
        {
            a.x = I;
            a.y = r.y - ELENCHUS_DIVISOR / II;
            b.x = latitudo - II;
            b.y = a.y;
            mandata_linea(md, a, b, I, figura_color_thematis(
                COLOR_TEXT_DIM));
        }
        si ((s32)i == e->electa && e->res[i].habilis)
        {
            mandata_rectangulum(md, r, figura_color_thematis(
                COLOR_SELECTION), VERUM);
        }
        si (e->res[i].textus)
        {
            mandata_textus(md, ELENCHUS_MARGO, r.y + ELENCHUS_TEXTUS_Y,
                chorda_transcribere(chorda_ex_literis(e->res[i].textus,
                md->piscina), md->piscina), FONS_PARVUS,
                figura_color_thematis(e->res[i].habilis ? COLOR_TEXT
                : COLOR_TEXT_DIM));
        }
        si (e->res[i].clavis)
        {
            n = (s32)strlen(e->res[i].clavis) * fons_latitudo(
                FONS_PARVUS);
            mandata_textus(md, latitudo - ELENCHUS_MARGO - n, r.y
                + ELENCHUS_TEXTUS_Y, chorda_transcribere(
                chorda_ex_literis(e->res[i].clavis, md->piscina),
                md->piscina), FONS_PARVUS, figura_color_thematis(
                COLOR_TEXT_DIM));
        }
    }
}


/* ==================================================
 * Claves
 * ================================================== */

/* proxima habilis a 'ab' gradu (+1 / -1), circumiens; -1 si nulla */
interior s32
habilem_quaerere (
    constans Elenchus* e,
                  s32  ab,
                  s32  gradus)
{
    s32 k;
    s32 i;

    per (k = I; k <= (s32)e->numerus; k++)
    {
        i = ab + gradus * k;
        i = ((i % (s32)e->numerus) + (s32)e->numerus) % (s32)e->numerus;
        si (e->res[i].habilis)
        {
            redde i;
        }
    }
    redde -I;
}

ElenchusExitus
elenchus_clavem_agere (
     constans Elenchus* e,
      constans Eventus* ev,
                   s32* electa)
{
    clavis_t clavis;
         s32 gradus;
         s32 nova;
         b32 littera;

    si (   !e || !ev || !electa || e->numerus == ZEPHYRUM
        || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde ELENCHUS_NIHIL;
    }
    clavis   = ev->datum.clavis.clavis;
    littera  = !(ev->datum.clavis.modificantes
             & (i32)(MOD_IMPERIUM | MOD_SUPER));
    si (clavis == CLAVIS_REDITUS)
    {
        redde (   e->electa >= ZEPHYRUM && e->electa < (s32)e->numerus
               && e->res[e->electa].habilis) ? ELENCHUS_ELECTUM
            : ELENCHUS_NIHIL;
    }
    si (   clavis == CLAVIS_DEORSUM
        || (littera && clavis == (clavis_t)'j'))
    {
        gradus = I;
    }
    alioquin si (   clavis == CLAVIS_SURSUM
                 || (littera && clavis == (clavis_t)'k'))
    {
        gradus = -I;
    }
    alioquin
    {
        redde ELENCHUS_NIHIL;
    }
    /* ex -1: Deorsum ab ante primam, Sursum ab post ultimam */
    nova = habilem_quaerere(e, e->electa >= ZEPHYRUM ? e->electa
        : (gradus > ZEPHYRUM ? -I : (s32)e->numerus), gradus);
    si (nova < ZEPHYRUM || nova == e->electa)
    {
        redde ELENCHUS_NIHIL;
    }
    *electa = nova;
    redde ELENCHUS_MOTUM;
}
