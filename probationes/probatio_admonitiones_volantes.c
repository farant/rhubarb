/* probatio_admonitiones_volantes.c - admonitiones volantes (toasts;
 * colloquia-plan T4b): visibiles ex indice et tempore (NUNTIUS VIII s,
 * ATTENTIO XXX s, III maximum, lectae non), pulsus mutationem
 * nuntiat, 'x' dimittit (non legit), corpus portam vocat et legit,
 * sine tempore nihil, positio (ad dextrum, sub linea tabularum),
 * pictura (titulus accentu, corpus), radix 0 x 0. */
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
#include "admonitiones.h"
#include "admonitiones_volantes.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
            Admonitiones* index;
       VolantiumMontatio  m;
             Dispensator* d;
                     s64  nunc;
                     i32  actae;
               character  acta[LXIV];
} Mundus;

interior b32
porta_ficta (
                vacuum* ctx,
    constans Admonitio* x)
{
    Mundus* w;

    w = (Mundus*)ctx;
    w->actae++;
    sprintf(w->acta, "%.*s", (integer)(x->id.mensura < LX
        ? x->id.mensura : LX), (constans character*)x->id.datum);
    redde VERUM;
}

interior s64
nunc_fictum (
    vacuum* ctx)
{
    redde ((Mundus*)ctx)->nunc;
}

interior vacuum
fictam_addere (
                 Mundus* w,
     constans character* id,
                    s64  tempus,
     GradusAdmonitionis  gradus,
                Piscina* p)
{
    Admonitio x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.id       = chorda_ex_literis(id, p);
    x.fons     = chorda_ex_literis("probatio", p);
    x.titulus  = chorda_ex_literis(id, p);
    x.corpus   = chorda_ex_literis("corpus", p);
    x.tempus   = tempus;
    x.gradus   = gradus;
    (vacuum)admonitiones_addere(w->index, &x);
}

/* stratum CDLXXX x CDLXXX; a0 NUNTIUS 1000, a1 ATTENTIO 2000, a2
 * NUNTIUS 3000 (+ n - III NUNTIUS ultra, 4000...); nunc 8000 */
interior Mundus*
mundus (
    CredoContextus* c,
               i32  n)
{
       Mundus* w;
        Motus* motus;
          i32  i;
    character  id[XVI];

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern  = internamentum_creare(c->piscina);
    w->index   = admonitiones_creare(c->piscina, C);
    w->nunc    = (s64)VIII * (s64)M;
    per (i = ZEPHYRUM; i < n; i++)
    {
        sprintf(id, "a%d", (integer)i);
        fictam_addere(w, id, (s64)(i + I) * (s64)M, i == I
            ? ADMONITIO_ATTENTIO : ADMONITIO_NUNTIUS, c->piscina);
    }
    w->repo = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !volantes_montare(&w->m, c->piscina, w->intern, w->repo,
               "stratum_volantes", w->index, porta_ficta, w, CDLXXX,
               CDLXXX, NIHIL))
    {
        redde NIHIL;
    }
    w->m.nunc      = nunc_fictum;
    w->m.nunc_ctx  = w;
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, volantes_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior Componens*
componens (
                 Mundus* w,
     constans character* id,
                Piscina* p)
{
    dispensator_recomponere(w->d);
    redde componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
}

/* tabellae visibiles ut "a2 a1 a0" (titulus = id) */
interior b32
tabellae_sunt (
                 Mundus* w,
     constans character* exspectatae,
                Piscina* p)
{
    character  visae[CXXVIII];
    character  id[VIII];
          i32  k;
      Mandata* md;
     Mandatum* x;
          i32  i;

    md = mandata_creare(p, w->intern);
    dispensator_recomponere(w->d);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    visae[ZEPHYRUM] = '\0';
    per (k = I; k <= IV; k++)
    {
        sprintf(id, "v%d", (integer)k);
        si (!componens(w, id, p))
        {
            frange;
        }
    }
    /* tituli picti ordine picturae */
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        x = mandata_obtinere(md, i);
        si (   x->genus == MANDATUM_TEXTUS && x->textus.mensura == II
            && x->textus.datum[ZEPHYRUM] == 'a')
        {
            si (visae[ZEPHYRUM])
            {
                strcat(visae, " ");
            }
            strncat(visae, (constans character*)x->textus.datum, II);
        }
    }
    si (   strcmp(visae, exspectatae) != ZEPHYRUM || k - I != (i32)(
        strlen(exspectatae) + I) / III)
    {
        imprimere("  tabellae: '%s' (componentes %d)\n", visae,
            (integer)(k - I));
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
ictus (
                 Mundus* w,
     constans character* id,
                Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens(w, id, p);
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
    redde VERUM;
}

/* visibiles: aetas per gradum; tempus progrediens; pulsus */
interior vacuum
_probare_visibiles (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, III);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(tabellae_sunt(w, "a2 a1 a0", c->piscina));
    CREDO_VERUM(volantes_pulsare(&w->m));
    CREDO_FALSUM(volantes_pulsare(&w->m));
    /* nunc 12000: a2 (9 s) et a0 (11 s) exspiraverunt, a1 ATTENTIO
     * (10 s) manet */
    w->nunc = (s64)XII * (s64)M;
    CREDO_VERUM(volantes_pulsare(&w->m));
    CREDO_VERUM(tabellae_sunt(w, "a1", c->piscina));
    /* nunc 32000: a1 (30 s) exspiravit */
    w->nunc = (s64)XXXII * (s64)M;
    CREDO_VERUM(volantes_pulsare(&w->m));
    CREDO_VERUM(tabellae_sunt(w, "", c->piscina));
    CREDO_FALSUM(volantes_pulsare(&w->m));
}

/* maximum III recentissimae; lecta non volat */
interior vacuum
_probare_maximum (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, V);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(tabellae_sunt(w, "a4 a3 a2", c->piscina));
    CREDO_VERUM(admonitionem_legere_notare(w->index, chorda_ex_literis(
        "a3", c->piscina)));
    CREDO_VERUM(tabellae_sunt(w, "a4 a2 a1", c->piscina));
}

/* 'x' dimittit (non legit); corpus portam vocat et legit */
interior vacuum
_probare_ictus (
    CredoContextus* c)
{
     Mundus* w;
     chorda* a;

    w = mundus(c, III);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(ictus(w, "x1", c->piscina));
    CREDO_VERUM(tabellae_sunt(w, "a1 a0", c->piscina));
    a = insula_ramus_attributum(&w->m.ramus, "dimissae");
    CREDO_NECESSE(a != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "a2");
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(w->index), III);
    CREDO_AEQUALIS_I32(w->actae, ZEPHYRUM);
    /* corpus a1 (nunc v1) */
    CREDO_VERUM(ictus(w, "v1", c->piscina));
    CREDO_AEQUALIS_I32(w->actae, I);
    CREDO_VERUM(strcmp(w->acta, "a1") == ZEPHYRUM);
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(w->index), II);
    CREDO_VERUM(tabellae_sunt(w, "a0", c->piscina));
    /* dimissio altera: dimissae servat a2 (adhuc volabilis) */
    CREDO_VERUM(ictus(w, "x1", c->piscina));
    a = insula_ramus_attributum(&w->m.ramus, "dimissae");
    CREDO_NECESSE(a != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "a0 a2");
    CREDO_VERUM(tabellae_sunt(w, "", c->piscina));
}

/* sine tempore nihil; positio; radix 0 x 0; pictura */
interior vacuum
_probare_formam (
    CredoContextus* c)
{
       Mundus* w;
    Componens* v;
    Componens* x;
    Componens* radix;

    w = mundus(c, III);
    CREDO_NECESSE(w != NIHIL);
    radix = componens(w, "volantes", c->piscina);
    CREDO_NECESSE(radix != NIHIL);
    CREDO_AEQUALIS_S32(radix->fines.latitudo, ZEPHYRUM);
    CREDO_AEQUALIS_S32(radix->fines.altitudo, ZEPHYRUM);
    v = componens(w, "v1", c->piscina);
    x = componens(w, "x1", c->piscina);
    CREDO_NECESSE(v && x);
    /* ad dextrum: cellula marginis; sub linea tabularum (II lineae) */
    CREDO_AEQUALIS_S32(x->fines.x + x->fines.latitudo, CDLXXX - VI);
    CREDO_AEQUALIS_S32(v->fines.y, XVI);
    CREDO_AEQUALIS_S32(v->fines.x + v->fines.latitudo, x->fines.x);
    CREDO_AEQUALIS_S32(v->fines.altitudo, XXXII);
    /* tabella secunda infra, intervallo lineae */
    v = componens(w, "v2", c->piscina);
    CREDO_NECESSE(v != NIHIL);
    CREDO_AEQUALIS_S32(v->fines.y, XVI + XXXII + VIII);
    /* sine tempore: nulla */
    w->m.nunc = NIHIL;
    CREDO_VERUM(componens(w, "v1", c->piscina) == NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "visibiles", _probare_visibiles, NIHIL, NIHIL, NIHIL, NIHIL },
    { "maximum", _probare_maximum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ictus", _probare_ictus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _probare_formam, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("admonitiones_volantes", SECTIONES);
}
