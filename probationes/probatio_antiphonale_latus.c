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
        AntiphonalePorta  porta;
    /* porta ficta: horologium a probatione positum */
                     b32  sonat;
                     s64  pulsus;
                     b32  recusare;
                     i32  incepta;
                     s64  pulsus_incepti;
             constans i8* carmen_incepti;
                     i32  quaesita;
                     s64  pulsus_quaesiti;
                     i32  sistita;
} Mundus;

interior b32
_incipere (
    vacuum* ctx,
    chorda  carmen,
       s64  pulsus)
{
    Mundus* w = (Mundus*)ctx;

    w->incepta++;
    w->pulsus_incepti = pulsus;
    w->carmen_incepti = carmen.datum;
    si (w->recusare)
    {
        redde FALSUM;
    }
    w->sonat   = VERUM;
    w->pulsus  = pulsus;
    redde VERUM;
}

interior vacuum
_quaerere (
    vacuum* ctx,
       s64  pulsus)
{
    Mundus* w = (Mundus*)ctx;

    w->quaesita++;
    w->pulsus_quaesiti  = pulsus;
    w->pulsus           = pulsus;
}

interior vacuum
_sistere (
    vacuum* ctx)
{
    Mundus* w = (Mundus*)ctx;

    w->sistita++;
    w->sonat = FALSUM;
}

interior s64
_pulsus (
    vacuum* ctx)
{
    Mundus* w = (Mundus*)ctx;

    redde w->sonat ? w->pulsus : -I;
}

interior Mundus*
_mundum_porta (
         CredoContextus* c,
     constans character* titulus,
     constans character* cantus,
                    b32  cum_porta)
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
    w->porta.incipere  = _incipere;
    w->porta.quaerere  = _quaerere;
    w->porta.sistere   = _sistere;
    w->porta.pulsus    = _pulsus;
    w->porta.ctx       = w;
    w->intern          = internamentum_creare(c->piscina);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !antiphonale_montare(&w->m, c->piscina, w->intern, w->repo,
               "a1", w->via, LATITUDO, ALTITUDO,
               cum_porta ? &w->porta : NIHIL, NIHIL))
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

interior Mundus*
_mundum (
         CredoContextus* c,
     constans character* titulus,
     constans character* cantus)
{
    redde _mundum_porta(c, titulus, cantus, VERUM);
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

/* fundus planus intra limbum: rectangulum COLOR_BACKGROUND (VI, VIII,
 * L - XII, A - XVI) - textura foci solum in margine */
interior b32
_fundus_intra_limbum (
    Mandata* md)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus          == MANDATUM_RECTANGULUM && x->impletum
            && x->color.genus    == COLOR_MANDATI_THEMA
            && x->color.valor    == (i32)COLOR_BACKGROUND
            && x->fines.x        == VI && x->fines.y == VIII
            && x->fines.latitudo == LATITUDO - XII
            && x->fines.altitudo == ALTITUDO - XVI)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
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
    CREDO_VERUM (_fundus_intra_limbum(md));
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

/* directorium absens (recensio finalis): specula iterum temptatur sine
 * incremento piscinae et non crebrius quam omni CCL ms; directorio et
 * plagula creatis latus eam capit */
interior vacuum
_directorium_absens_probare (
    CredoContextus* c)
{
             Mundus* w;
            Mandata* md;
     memoriae_index  usus;
                i32  k;
          character  directorium[CCLVI];

    w = _mundum(c, "absens/x.gabc", NIHIL);
    CREDO_NECESSE (w != NIHIL);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "missing"));

    CREDO_GRADUS ("pulsus multi: piscina non crescit");
    (vacuum)antiphonale_pulsare(&w->m);
    usus = piscina_summa_usus(c->piscina);
    per (k = ZEPHYRUM; k < CC; k++)
    {
        (vacuum)antiphonale_pulsare(&w->m);
    }
    CREDO_AEQUALIS_I64 ((i64)piscina_summa_usus(c->piscina), (i64)usus);

    CREDO_GRADUS ("mora sine specula: saltem CCL ms");
    CREDO_MAIOR_AUT_AEQUALIS_S64 ((s64)antiphonale_mora(&w->m),
        (s64)CCL);

    CREDO_GRADUS ("directorium et plagula creata: latus capit");
    sprintf(directorium, "%s/absens", credo_area(c));
    CREDO_NECESSE (filum_directorium_creare(directorium));
    CREDO_NECESSE (filum_scribere_literis(w->via, CANTUS_BONUS));
    CREDO_VERUM (antiphonale_pulsare(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Probatio"));
    CREDO_FALSUM (_textus_continet(md, "missing"));
    CREDO_AEQUALIS_S64 ((s64)antiphonale_mora(&w->m), -I);
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


/* ==================================================
 * P7: sonus in latere
 * ================================================== */

/* textus syllabae illuminatae (COLOR_ACCENT_PRIMARY); "" si nullus */
interior constans character*
_illuminata (
    Mandata* md,
    Piscina* p)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_TEXTUS
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)COLOR_ACCENT_PRIMARY)
        {
            redde chorda_ut_cstr(x->textus, p);
        }
    }
    redde "";
}

/* ictus in textu syllabae picto (exacte "textus") */
interior b32
_ictus_textus (
                Mundus* w,
    constans character* textus,
               Piscina* p)
{
              Mandata* md;
    constans Mandatum* x;
              Eventus  e;
                  i32  i;

    md = _reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus             = EVENTUS_MUS_DEPRESSUS;
            e.datum.mus.x       = x->fines.x + II;
            e.datum.mus.y       = x->fines.y + II;
            e.datum.mus.botton  = MUS_SINISTER;
            dispensator_tractare(w->d, &e);
            e.genus = EVENTUS_MUS_LIBERATUS;
            dispensator_tractare(w->d, &e);
            dispensator_recomponere(w->d);
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_spatium (
    Mundus* w)
{
    _clavis(w, (clavis_t)' ', ' ');
}

/* tempus ficti ponit, deinde pulsus lateris */
interior b32
_ad_pulsum (
    Mundus* w,
       s64  pulsus)
{
    b32 mutatum;

    w->pulsus  = pulsus;
    mutatum    = antiphonale_pulsare(&w->m);
    dispensator_recomponere(w->d);
    redde mutatum;
}

#define CANTUS_TRES \
    "name:Tres;\ninitial-style:0;\n%%\n(c4) a(g) b(h) c(g)\n"
#define CANTUS_SPIRITUS \
    "name:Spiritus;\ninitial-style:0;\n%%\n(c4) a(gh/hg) (;) b(g.)\n"

interior vacuum
_sonare_probare (
    CredoContextus* c)
{
    Mundus* w;

    w = _mundum(c, "tres.gabc", CANTUS_TRES);
    CREDO_NECESSE (w != NIHIL);

    CREDO_GRADUS ("Spatium: a pulso 0; iterum: sistit");
    _spatium(w);
    CREDO_AEQUALIS_I32 (w->incepta, I);
    CREDO_AEQUALIS_S64 (w->pulsus_incepti, ZEPHYRUM);
    CREDO_NON_NIHIL (w->carmen_incepti);
    CREDO_VERUM (w->sonat);
    _spatium(w);
    CREDO_AEQUALIS_I32 (w->sistita, I);
    CREDO_FALSUM (w->sonat);

    CREDO_GRADUS ("globulus canendi ictu idem");
    CREDO_VERUM (_ictus(w, "globulus_canere", c->piscina));
    CREDO_AEQUALIS_I32 (w->incepta, II);
    CREDO_VERUM (_ictus(w, "globulus_canere", c->piscina));
    CREDO_AEQUALIS_I32 (w->sistita, II);

    CREDO_GRADUS ("syllaba electa (ictus sistente): ab ea incipit");
    CREDO_VERUM (_ictus_textus(w, "b", c->piscina));
    CREDO_AEQUALIS_I32 (w->incepta, II);
    _spatium(w);
    CREDO_AEQUALIS_I32 (w->incepta, III);
    CREDO_AEQUALIS_S64 (w->pulsus_incepti, CDLXXX);
}

interior vacuum
_illuminationem_probare (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = _mundum(c, "spiritus.gabc", CANTUS_SPIRITUS);
    CREDO_NECESSE (w != NIHIL);
    _spatium(w);
    CREDO_VERUM (w->sonat);

    CREDO_GRADUS ("ad initium nexus: syllaba eius");
    (vacuum)_ad_pulsum(w, ZEPHYRUM);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "a");
    (vacuum)_ad_pulsum(w, MCDXL);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "a");

    CREDO_GRADUS ("in spiritu (MCMXX..MMCLX): prior manet");
    (vacuum)_ad_pulsum(w, MM);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "a");
    CREDO_VERUM (_ad_pulsum(w, MMCLX));
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "b");

    CREDO_GRADUS ("post finem: nulla");
    w->sonat = FALSUM;
    CREDO_VERUM (_ad_pulsum(w, ZEPHYRUM));
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "");
}

interior vacuum
_quaerere_probare (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = _mundum(c, "tres.gabc", CANTUS_TRES);
    CREDO_NECESSE (w != NIHIL);

    CREDO_GRADUS ("sistente: ictus eligit, nihil quaerit");
    CREDO_VERUM (_ictus_textus(w, "c", c->piscina));
    CREDO_AEQUALIS_I32 (w->quaesita, ZEPHYRUM);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "c");
    _clavis(w, CLAVIS_SINISTER, ZEPHYRUM);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "b");

    CREDO_GRADUS ("sonante: ictus quaerit nexum primum syllabae");
    _spatium(w);
    CREDO_AEQUALIS_S64 (w->pulsus_incepti, CDLXXX);
    CREDO_VERUM (_ictus_textus(w, "c", c->piscina));
    CREDO_AEQUALIS_I32 (w->quaesita, I);
    CREDO_AEQUALIS_S64 (w->pulsus_quaesiti, CMLX);

    CREDO_GRADUS ("sagittae: syllaba prior / proxima");
    (vacuum)_ad_pulsum(w, CMLX);
    _clavis(w, CLAVIS_SINISTER, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (w->pulsus_quaesiti, CDLXXX);
    (vacuum)_ad_pulsum(w, CDLXXX);
    _clavis(w, CLAVIS_DEXTER, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (w->pulsus_quaesiti, CMLX);
}

interior vacuum
_permutationem_probare (
    CredoContextus* c)
{
         Mundus* w;
    constans i8* vetus;

    w = _mundum(c, "tres.gabc", CANTUS_TRES);
    CREDO_NECESSE (w != NIHIL);
    _spatium(w);
    (vacuum)_ad_pulsum(w, CDLXXX);
    vetus = w->carmen_incepti;

    CREDO_GRADUS ("versio nova sonante: incipere(novum, P) semel");
    CREDO_NECESSE (filum_scribere_literis(w->via,
        "name:Tres;\ninitial-style:0;\n%%\n(c4) a(g) b(i) c(g)\n"));
    CREDO_VERUM (_ad_pulsum(w, CDLXXX));
    CREDO_AEQUALIS_I32 (w->incepta, II);
    CREDO_AEQUALIS_S64 (w->pulsus_incepti, CDLXXX);
    CREDO_VERUM (w->carmen_incepti != vetus);
    CREDO_AEQUALIS_I32 (w->sistita, ZEPHYRUM);

    CREDO_GRADUS ("versio mala sonante: vetus pergit");
    CREDO_NECESSE (filum_scribere_literis(w->via, CANTUS_MALUS));
    CREDO_VERUM (_ad_pulsum(w, CDLXXX));
    CREDO_AEQUALIS_I32 (w->incepta, II);
    CREDO_AEQUALIS_I32 (w->sistita, ZEPHYRUM);
    CREDO_VERUM (w->sonat);

    CREDO_GRADUS ("versio brevior pulso: sistere");
    (vacuum)_ad_pulsum(w, MCC);
    CREDO_NECESSE (filum_scribere_literis(w->via,
        "name:Tres;\ninitial-style:0;\n%%\n(c4) a(g)\n"));
    (vacuum)_ad_pulsum(w, MCC);
    CREDO_AEQUALIS_I32 (w->incepta, II);
    CREDO_AEQUALIS_I32 (w->sistita, I);
}

interior vacuum
_mutum_probare (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    CREDO_GRADUS ("cantus sine notis: pingitur, Spatium nihil, causa");
    w = _mundum(c, "mutus.gabc",
        "name:Mutus;\ninitial-style:0;\n%%\n(c4) a() b()\n");
    CREDO_NECESSE (w != NIHIL);
    _spatium(w);
    CREDO_AEQUALIS_I32 (w->incepta, ZEPHYRUM);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Mutus"));
    CREDO_VERUM (_textus_continet(md, "not playable"));
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "");

    CREDO_GRADUS ("porta recusat (mutus): nihil illuminatum");
    w = _mundum(c, "tres.gabc", CANTUS_TRES);
    CREDO_NECESSE (w != NIHIL);
    w->recusare = VERUM;
    _spatium(w);
    CREDO_AEQUALIS_I32 (w->incepta, I);
    CREDO_FALSUM (w->sonat);
    (vacuum)_ad_pulsum(w, ZEPHYRUM);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "");

    CREDO_GRADUS ("porta nulla: nihil, nulla ruina");
    w = _mundum_porta(c, "tres2.gabc", CANTUS_TRES, FALSUM);
    CREDO_NECESSE (w != NIHIL);
    _spatium(w);
    (vacuum)antiphonale_pulsare(&w->m);
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "");
}

interior vacuum
_dispositionem_vivam_probare (
    CredoContextus* c)
{
    character  cantus[(IV * M)];
    character  syllaba[XXXII];
       Mundus* w;
      Mandata* md;
          i32  i;

    strcpy(cantus, "name:Longus;\ninitial-style:0;\n%%\n(c4)");
    per (i = ZEPHYRUM; i < XXX; i++)
    {
        sprintf(syllaba, " q%d(g) (z)", (integer)i);
        strcat(cantus, syllaba);
    }
    w = _mundum(c, "longus.gabc", cantus);
    CREDO_NECESSE (w != NIHIL);
    _spatium(w);

    CREDO_GRADUS ("pagina syllabam sonantem sequitur");
    (vacuum)_ad_pulsum(w, (s64)(XXV * CDLXXX));
    md = _reddere(w, c->piscina);
    CREDO_MAIOR_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "q25");

    CREDO_GRADUS ("spatium mutatum sonante: illuminata in conspectu");
    _clavis(w, (clavis_t)'-', '-');
    (vacuum)_ad_pulsum(w, (s64)(XXV * CDLXXX));
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "q25");
    _clavis(w, (clavis_t)'+', '+');
    _clavis(w, (clavis_t)'+', '+');
    _clavis(w, (clavis_t)'+', '+');
    (vacuum)_ad_pulsum(w, (s64)(XXV * CDLXXX));
    md = _reddere(w, c->piscina);
    CREDO_LITERAE_AEQUALES (_illuminata(md, c->piscina), "q25");

    CREDO_GRADUS ("pagina ad initium: syllaba prima");
    (vacuum)_ad_pulsum(w, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aperire", _aperire_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servatio", _servatum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "directorium absens", _directorium_absens_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "paginae", _paginas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sonare", _sonare_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "illuminatio", _illuminationem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "quaerere", _quaerere_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "permutatio (G18)", _permutationem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mutum (RF2)", _mutum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dispositio viva (RF5)", _dispositionem_vivam_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("antiphonale_latus", SECTIONES);
}
