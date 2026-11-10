/* membrum__delectus.c - delectus (include/membrum__delectus.h).
 * Status ut membrum__elenchi scribitur (campi rami hospitis, scriptore
 * hospitis); menu ipsum elenchus est. */
#include "membrum__delectus.h"
#include "figura.h"
#include "fons.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>

#define CAMPUS_APERTI   "delectus"
#define CAMPUS_ELECTAE  "delectus_electa"

/* botton: margo intra, latitudo litterae, sagitta */
#define MARGO_BOTTONIS    IV
#define LATITUDO_LITTERAE VI
#define SAGITTAE_LATITUDO V
#define SAGITTAE_SPATIUM  IV

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

nomen structura {
    ParCampi* pares;
         i32  numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex = (GrexCamporum*)ctx;
        ParCampi* par;
             i32  i;

    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        par = grex->pares + i;
        si (!par->valor || !par->valor[ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, par->titulus);
            perge;
        }
        insula_attributum_ponere(radix, p, in, par->titulus,
            par->valor);
    }
}

/* campi ambo scriptura una, scriptore hospitis; valor vacuus aut
 * NIHIL campum tollit; titulus_secundus NIHIL = campus unus */
interior b32
campos_scribere (
           InsulaRamus* ramus,
    constans character* scriptor,
    constans character* titulus_primus,
    constans character* valor_primus,
    constans character* titulus_secundus,
    constans character* valor_secundus,
               Piscina* p)
{
        ParCampi pares[II];
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    pares[ZEPHYRUM].titulus  = titulus_primus;
    pares[ZEPHYRUM].valor    = valor_primus;
    pares[I].titulus         = titulus_secundus;
    pares[I].valor           = valor_secundus;
    grex.pares               = pares;
    grex.numerus             = titulus_secundus ? II : I;
    prior                    = ramus->repo->scriptor;
    insula_scriptorem_ponere(ramus->repo, chorda_ex_literis(scriptor,
        p));
    ok = mutare_ramum(ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(ramus->repo, prior);
    redde ok;
}

interior b32
electam_scribere (
           InsulaRamus* ramus,
    constans character* scriptor,
                   s32  electa,
               Piscina* p)
{
    character numerus[XVI];

    sprintf(numerus, "%d", (integer)electa);
    redde campos_scribere(ramus, scriptor, CAMPUS_ELECTAE, numerus,
        NIHIL, NIHIL, p);
}


/* ==================================================
 * Botton
 * ================================================== */

i32
delectus_bottonis_latitudo (
    chorda textus)
{
    redde MARGO_BOTTONIS + textus.mensura * LATITUDO_LITTERAE
        + SAGITTAE_SPATIUM + SAGITTAE_LATITUDO + MARGO_BOTTONIS;
}

Componens*
delectus_bottonem_componere (
     constans character* id,
                  Fines  fines,
     constans character* actio,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Componens* c;

    si (!id || !piscina || !intern)
    {
        redde NIHIL;
    }
    c = componens_creare(piscina, intern, id, PARTES_BOTTONE);
    si (!c)
    {
        redde NIHIL;
    }
    componens_ponere_fines(c, fines);
    si (actio)
    {
        componens_ponere_actio(c, actio);
    }
    redde c;
}

vacuum
delectus_bottonem_pingere (
    Mandata* md,
      Fines  fines,
     chorda  textus,
        b32  apertus,
        b32  habilis)
{
    ColorMandati color;
          chorda vacuus;
         Punctum a;
         Punctum b;
             s32 x;
             s32 y;
             s32 k;

    si (!md)
    {
        redde;
    }
    vacuus.datum    = NIHIL;
    vacuus.mensura  = ZEPHYRUM;
    /* corpus: globulus sine textu (margo, fundus, status) */
    figura_globulum_pingere(md, fines, vacuus,
        (apertus ? FIGURA_ELECTUM : ZEPHYRUM)
        | (habilis ? ZEPHYRUM : FIGURA_OBSCURUM));
    si (   fines.latitudo < MARGO_BOTTONIS * II + SAGITTAE_LATITUDO
        || fines.altitudo < VIII)
    {
        redde;
    }
    color = figura_color_thematis(habilis ? COLOR_TEXT
                                          : COLOR_TEXT_DIM);
    y     = fines.y + (fines.altitudo - VIII) / II;
    mandata_textus(md, fines.x + MARGO_BOTTONIS, y, textus,
        FONS_PARVUS, color);
    /* sagitta deorsum: lineae V, III, I pixelorum */
    x = fines.x + fines.latitudo - MARGO_BOTTONIS - SAGITTAE_LATITUDO;
    y = fines.y + (fines.altitudo - III) / II;
    per (k = ZEPHYRUM; k < III; k++)
    {
        a.x = x + k;
        a.y = y + k;
        b.x = x + SAGITTAE_LATITUDO - I - k;
        b.y = y + k;
        mandata_linea(md, a, b, I, color);
    }
}


/* ==================================================
 * Status
 * ================================================== */

b32
delectus_aperire (
           InsulaRamus* ramus,
    constans character* scriptor,
    constans character* id,
                   s32  electa,
               Piscina* piscina)
{
    character numerus[XVI];

    si (!ramus || !scriptor || !id || !id[ZEPHYRUM] || !piscina)
    {
        redde FALSUM;
    }
    numerus[ZEPHYRUM] = '\0';
    si (electa >= ZEPHYRUM)
    {
        sprintf(numerus, "%d", (integer)electa);
    }
    redde campos_scribere(ramus, scriptor, CAMPUS_APERTI, id,
        CAMPUS_ELECTAE, numerus, piscina);
}

b32
delectus_claudere (
           InsulaRamus* ramus,
    constans character* scriptor,
               Piscina* piscina)
{
    si (!ramus || !scriptor || !piscina)
    {
        redde FALSUM;
    }
    redde campos_scribere(ramus, scriptor, CAMPUS_APERTI, "",
        CAMPUS_ELECTAE, "", piscina);
}

b32
delectus_apertus (
    constans InsulaRamus* ramus,
      constans character* id)
{
    chorda* a;

    si (!ramus)
    {
        redde FALSUM;
    }
    a = insula_ramus_attributum(ramus, CAMPUS_APERTI);
    si (!a || a->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde !id || chorda_aequalis_literis(*a, id);
}

s32
delectus_electa (
    constans InsulaRamus* ramus,
                     i32  numerus)
{
    chorda* a;
       s32  electa;

    si (!ramus)
    {
        redde -I;
    }
    a = insula_ramus_attributum(ramus, CAMPUS_ELECTAE);
    si (   !a || !chorda_ut_s32(*a, &electa) || electa < ZEPHYRUM
        || electa >= (s32)numerus)
    {
        redde -I;
    }
    redde electa;
}


/* ==================================================
 * Menu et eventa
 * ================================================== */

Componens*
delectus_componere (
      constans Elenchus* optiones,
                  Fines  botton,
                  Fines  area,
     constans character* actio,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Punctum ancora;
      Fines f;

    si (   !optiones || optiones->numerus == ZEPHYRUM || !piscina
        || !intern)
    {
        redde NIHIL;
    }
    ancora.x  = botton.x;
    ancora.y  = botton.y + botton.altitudo;
    f         = elenchus_fines(optiones, ancora, area);
    /* elenchus sursum reflexit aut coegit (infra non capit): supra
     * bottonem, ne eum tegat */
    si (f.y < ancora.y)
    {
        f.y = botton.y - f.altitudo;
        si (f.y < area.y)
        {
            f.y = area.y;
        }
    }
    redde elenchus_componere(optiones, f, actio, piscina, intern);
}

DelectusExitus
delectus_eventum_agere (
           InsulaRamus* ramus,
    constans character* scriptor,
     constans Elenchus* optiones,
    constans Componens* nodus,
      constans Eventus* ev,
                   s32* electum,
               Piscina* piscina)
{
          Elenchus e;
    ElenchusExitus exitus;
               s32 index;
               s32 electa;

    si (   !ramus || !scriptor || !optiones || !ev || !piscina
        || !delectus_apertus(ramus, NIHIL))
    {
        redde DELECTUS_NIHIL;
    }
    e         = *optiones;
    e.electa  = delectus_electa(ramus, e.numerus);
    index     = elenchus_index(nodus);
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_MOTUS:
            si (   index >= ZEPHYRUM && index < (s32)e.numerus
                && e.res[index].habilis && index != e.electa)
            {
                (vacuum)electam_scribere(ramus, scriptor, index,
                    piscina);
            }
            redde DELECTUS_DEGLUTITUM;
        casus EVENTUS_MUS_DEPRESSUS:
            si (index >= ZEPHYRUM && index < (s32)e.numerus)
            {
                si (   !e.res[index].habilis
                    || ev->datum.mus.botton != MUS_SINISTER)
                {
                    redde DELECTUS_DEGLUTITUM;
                }
                (vacuum)delectus_claudere(ramus, scriptor, piscina);
                si (electum)
                {
                    *electum = index;
                }
                redde DELECTUS_ELECTUM;
            }
            si (nodus && nodus->partes == PARTES_DIALOGUS)
            {
                redde DELECTUS_DEGLUTITUM;
            }
            (vacuum)delectus_claudere(ramus, scriptor, piscina);
            redde DELECTUS_CLAUSUM;
        casus EVENTUS_MUS_DUPLEX:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_TEXTUS:
            redde DELECTUS_DEGLUTITUM;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            frange;
        ordinarius:
            redde DELECTUS_NIHIL;
    }
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        (vacuum)delectus_claudere(ramus, scriptor, piscina);
        redde DELECTUS_CLAUSUM;
    }
    electa = e.electa;
    exitus = elenchus_clavem_agere(&e, ev, &electa);
    si (exitus == ELENCHUS_MOTUM)
    {
        (vacuum)electam_scribere(ramus, scriptor, electa, piscina);
    }
    alioquin si (exitus == ELENCHUS_ELECTUM)
    {
        (vacuum)delectus_claudere(ramus, scriptor, piscina);
        si (electum)
        {
            *electum = electa;
        }
        redde DELECTUS_ELECTUM;
    }
    redde DELECTUS_DEGLUTITUM;
}
