/* probatio_antiphonale_latus.c - cantus gabc in latere (antiphonale.h;
 * gabc pars II P6)
 *
 * Latus solum (ut probatio_aetherophonum): repositorium, montatio,
 * dispensator, eventa ficta; plagulae in area probationis (credo_area).
 *
 * aperire: titulus (name) et lineae systematis pinguntur, linea ima
 * "1/1"; titulus lateris = name, aliter nomen plagulae. servatio
 * (Review Focus 1): vitium, plagula vacua, deletio -> pictura servata,
 * vexillum (linea:columna, causa / empty / missing); creatio bona ->
 * pictura nova, vexillum ablatum. paginae: PageDown / PageUp, sagittae
 * ictu; + / - spatium, syllaba prima visa manet. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "filum.h"
#include "antiphonale.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  CDLXXX
#define ALTITUDO  CCCXX

#define CANTUS_BONUS \
    "name:Probatio;\ninitial-style:0;\n%%\n(c4) a(g) b(h) c(g)\n"
#define CANTUS_ALTER \
    "name:Alter;\ninitial-style:0;\n%%\n(c4) d(g) e(h)\n"
#define CANTUS_MALUS \
    "name:Probatio;\n%%\n(c4) a(g\n"
#define CANTUS_SINE_TITULO \
    "initial-style:0;\n%%\n(c4) a(g)\n"

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
     AntiphonaleMontatio  m;
             Dispensator* d;
               character  via[CCLVI];
} Mundus;

interior Mundus*
_mundum (
         CredoContextus* c,
     constans character* titulus,
     constans character* cantus)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    si (titulus[ZEPHYRUM] != '\0')
    {
        sprintf(w->via, "%s/%s", credo_area(c), titulus);
    }
    si (cantus && !filum_scribere_literis(w->via, cantus))
    {
        redde NIHIL;
    }
    w->intern  = internamentum_creare(c->piscina);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !antiphonale_montare(&w->m, c->piscina, w->intern, w->repo,
               "a1", w->via, LATITUDO, ALTITUDO, NIHIL, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, antiphonale_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior Mandata*
_reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md = mandata_creare(p, w->intern);

    dispensator_recomponere(w->d);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* textus pictus qui litteras datas continet */
interior b32
_textus_continet (
               Mandata* md,
    constans character* pars)
{
    constans Mandatum* x;
                  i32  i;
                  i32  k;
                  i32  n;

    n = (i32)strlen(pars);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus != MANDATUM_TEXTUS)
        {
            perge;
        }
        per (k = ZEPHYRUM; k + n <= x->textus.mensura; k++)
        {
            si (memcmp(x->textus.datum + k, pars, (size_t)n)
                == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* rectangula impleta colore thematis dato */
interior i32
_quadrata (
           Mandata* md,
        ColorThema  color)
{
    constans Mandatum* x;
                  i32  i;
                  i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_RECTANGULUM
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)color)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_clavis (
       Mundus* w,
     clavis_t  clavis,
          s32  producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = clavis;
    e.datum.clavis.producta  = producta;
    e.datum.clavis.runa      = producta;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* ictus in medio componentis nominati */
interior b32
_ictus (
                Mundus* w,
    constans character* id,
               Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (!c)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior s32
_campus (
                Mundus* w,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);
       s32  v;

    si (!a || !chorda_ut_s32(*a, &v))
    {
        redde praestitutum;
    }
    redde v;
}


/* ==================================================
 * I: aperire
 * ================================================== */

interior vacuum
_aperire_probare (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;
     chorda  titulus;

    w = _mundum(c, "probatio.gabc", CANTUS_BONUS);
    CREDO_NECESSE (w != NIHIL);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Probatio"));
    CREDO_VERUM (_textus_continet(md, "1/1"));
    CREDO_MAIOR_AUT_AEQUALIS_I32 (_quadrata(md, COLOR_TEXT_DIM), IV);
    CREDO_AEQUALIS_I32 (_quadrata(md, COLOR_ERROR), ZEPHYRUM);
    titulus = antiphonale_titulus(&w->m, c->piscina);
    CREDO_VERUM (chorda_aequalis_literis(titulus, "Probatio"));

    w = _mundum(c, "sine_titulo.gabc", CANTUS_SINE_TITULO);
    CREDO_NECESSE (w != NIHIL);
    titulus = antiphonale_titulus(&w->m, c->piscina);
    CREDO_VERUM (chorda_aequalis_literis(titulus, "sine_titulo.gabc"));

    /* sine via: nuntium */
    w = _mundum(c, "", NIHIL);
    CREDO_NECESSE (w != NIHIL);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "$antiphonale"));
}


/* ==================================================
 * II: servatio (Review Focus 1)
 * ================================================== */

interior vacuum
_servatum_probare (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;
        i32  lineae;

    w = _mundum(c, "servatio.gabc", CANTUS_BONUS);
    CREDO_NECESSE (w != NIHIL);
    md      = _reddere(w, c->piscina);
    lineae  = _quadrata(md, COLOR_TEXT_DIM);
    CREDO_FALSUM (antiphonale_pulsare(&w->m));

    CREDO_GRADUS ("vitium: pictura servata, vexillum linea:columna");
    CREDO_NECESSE (filum_scribere_literis(w->via, CANTUS_MALUS));
    CREDO_VERUM (antiphonale_pulsare(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Probatio"));
    CREDO_AEQUALIS_I32 (_quadrata(md, COLOR_TEXT_DIM), lineae);
    CREDO_MAIOR_I32 (_quadrata(md, COLOR_ERROR), ZEPHYRUM);
    CREDO_VERUM (_textus_continet(md, "3:"));
    CREDO_FALSUM (antiphonale_pulsare(&w->m));

    CREDO_GRADUS ("plagula vacua: idem");
    CREDO_NECESSE (filum_scribere_literis(w->via, ""));
    CREDO_VERUM (antiphonale_pulsare(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Probatio"));
    CREDO_VERUM (_textus_continet(md, "empty"));

    CREDO_GRADUS ("deleta: missing; creata bona: pictura nova");
    CREDO_NECESSE (filum_delere(w->via));
    CREDO_VERUM (antiphonale_pulsare(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Probatio"));
    CREDO_VERUM (_textus_continet(md, "missing"));
    CREDO_NECESSE (filum_scribere_literis(w->via, CANTUS_ALTER));
    CREDO_VERUM (antiphonale_pulsare(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Alter"));
    CREDO_FALSUM (_textus_continet(md, "Probatio"));
    CREDO_AEQUALIS_I32 (_quadrata(md, COLOR_ERROR), ZEPHYRUM);
}


/* ==================================================
 * III: paginae et spatium
 * ================================================== */

/* syllaba prima picta: numerus minimus textuum "qN"; -1 nullus */
interior s32
_prima_visa (
    Mandata* md)
{
    constans Mandatum* x;
                  i32  i;
                  i32  k;
                  s32  n;
                  s32  minima;

    minima = -I;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus != MANDATUM_TEXTUS || x->textus.mensura < II
            || x->textus.datum[ZEPHYRUM] != 'q')
        {
            perge;
        }
        n = ZEPHYRUM;
        per (k = I; k < x->textus.mensura; k++)
        {
            si (x->textus.datum[k] < '0' || x->textus.datum[k] > '9')
            {
                n = -I;
                frange;
            }
            n = n * X + (s32)(x->textus.datum[k] - '0');
        }
        si (n >= ZEPHYRUM && (minima < ZEPHYRUM || n < minima))
        {
            minima = n;
        }
    }
    redde minima;
}

/* textus "qN" exacte pictus? */
interior b32
_syllaba_picta (
    Mandata* md,
        s32  n)
{
    constans Mandatum* x;
                  i32  i;
            character  textus[XVI];

    sprintf(textus, "q%d", (integer)n);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_paginas_probare (
    CredoContextus* c)
{
    character  cantus[(IV * M)];
    character  syllaba[XXXII];
       Mundus* w;
      Mandata* md;
          i32  i;
          s32  prima;

    strcpy(cantus, "name:Longus;\ninitial-style:0;\n%%\n(c4)");
    per (i = ZEPHYRUM; i < XXX; i++)
    {
        sprintf(syllaba, " q%d(g) (z)", (integer)i);
        strcat(cantus, syllaba);
    }
    w = _mundum(c, "longus.gabc", cantus);
    CREDO_NECESSE (w != NIHIL);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "1/"));
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_prima_visa(md), ZEPHYRUM);

    CREDO_GRADUS ("PageDown, PageUp (non ante primam)");
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), I);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "2/"));
    CREDO_MAIOR_S32 (_prima_visa(md), ZEPHYRUM);
    _clavis(w, CLAVIS_PAGINA_SURSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);
    _clavis(w, CLAVIS_PAGINA_SURSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);

    CREDO_GRADUS ("sagittae ictu");
    CREDO_VERUM (_ictus(w, "sagitta_proxima", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), I);
    CREDO_VERUM (_ictus(w, "sagitta_prior", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);

    CREDO_GRADUS ("spatium + et -: syllaba prima visa manet");
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    md     = _reddere(w, c->piscina);
    prima  = _prima_visa(md);
    CREDO_MAIOR_S32 (prima, ZEPHYRUM);
    _clavis(w, (clavis_t)'+', '+');
    CREDO_AEQUALIS_S32 (_campus(w, "spatium", VIII), X);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_syllaba_picta(md, prima));
    _clavis(w, (clavis_t)'-', '-');
    _clavis(w, (clavis_t)'-', '-');
    CREDO_AEQUALIS_S32 (_campus(w, "spatium", VIII), VI);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_syllaba_picta(md, prima));
    _clavis(w, (clavis_t)'-', '-');
    CREDO_AEQUALIS_S32 (_campus(w, "spatium", VIII), VI);
    _clavis(w, (clavis_t)'=', '=');
    CREDO_AEQUALIS_S32 (_campus(w, "spatium", VIII), VIII);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aperire", _aperire_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servatio", _servatum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae", _paginas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("antiphonale_latus", SECTIONES);
}
