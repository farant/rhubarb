/* consilium.c - CONSILIUM: nucleus purus consilii (plan / apply); caput
 * include/consilium.h, spec project-specs/consilium-spec.md (S2)
 *
 * Nulla I/O. Gradus pauci (decades): arcus per paria graduum et
 * effectuum computantur, attingibilitas per matricem - simplicitas
 * ante celeritatem donec consumptor aliud poscat.
 */
#include "consilium.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "iter_directoria.h"
#include "internamentum.h"
#include "stml.h"
#include <string.h>

structura Consilium {
           Piscina* piscina;
               Xar* gradus;     /* ConsiliumGradus* */
    TabulaDispersa* index;      /* titulus -> ConsiliumGradus* */
};

hic_manens constans character* constans _VERBA[] = {
    "legere", "inspicere", "enumerare", "scribere", "delere", "movere",
    "exsequi", "fontare", "ambitum_legere", "instituere", "rete",
    "committere"
};

hic_manens constans character* constans _CLASSES[] = {
    "arbor", "build", "instrumentum_domus", "domus", "externa",
    "systema", "temporaria"
};

constans character*
consilium_verbum_titulus (
    ConsiliumVerbum verbum)
{
    redde (i32)verbum < (i32)CONSILIUM_VERBA_NUMERUS
        ? _VERBA[verbum] : "?";
}

constans character*
consilium_classis_titulus (
    ConsiliumClassis classis)
{
    redde (i32)classis < (i32)CONSILIUM_CLASSES_NUMERUS
        ? _CLASSES[classis] : "?";
}


/* ==================================================
 * Fabrica consilii
 * ================================================== */

Consilium*
consilium_creare (
    Piscina* piscina)
{
    Consilium* c = (Consilium*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Consilium));

    c->piscina  = piscina;
    c->gradus   = xar_creare(piscina, (i32)magnitudo(ConsiliumGradus*));
    c->index    = tabula_dispersa_creare_chorda(piscina, LXIV);
    redde c;
}

ConsiliumGradus*
consilium_gradum_addere (
     Consilium* consilium,
        chorda  titulus,
        chorda  genus,
        chorda  auctor)
{
    ConsiliumGradus* g = NIHIL;

    si (!tabula_dispersa_invenire(consilium->index, titulus,
            (vacuum**)&g))
    {
        g = (ConsiliumGradus*)piscina_allocare(consilium->piscina,
            (memoriae_index)magnitudo(ConsiliumGradus));
        g->titulus  = titulus;
        g->genus    = genus;
        g->effectus    = xar_creare(consilium->piscina,
            (i32)magnitudo(ConsiliumEffectus));
        g->sequitur    = xar_creare(consilium->piscina,
            (i32)magnitudo(chorda));
        g->vestigium   = xar_creare(consilium->piscina,
            (i32)magnitudo(chorda));
        g->auctores    = xar_creare(consilium->piscina,
            (i32)magnitudo(chorda));
        g->caecus = FALSUM;
        g->sumptus_ms = -I;
        g->ordo_additionis = xar_numerus(consilium->gradus);
        *(ConsiliumGradus**)xar_addere(consilium->gradus) = g;
        tabula_dispersa_inserere(consilium->index, titulus, g);
    }
    si (auctor.mensura > ZEPHYRUM)
    {
        *(chorda*)xar_addere(g->auctores) = auctor;
    }
    redde g;
}

ConsiliumEffectus
consilium_effectus (
    ConsiliumVerbum verbum,
             chorda res)
{
    ConsiliumEffectus e;

    e.verbum         = verbum;
    e.res            = res;
    e.forma          = CONSILIUM_VIA;
    e.classis        = CONSILIUM_ARBOR;
    e.certitudo      = CONSILIUM_PLENA;
    e.cognitio       = CONSILIUM_PROPOSITUM;
    e.exstat         = VERUM;
    e.causa.datum    = NIHIL;
    e.causa.mensura  = ZEPHYRUM;
    redde e;
}

vacuum
consilium_effectum_addere (
               Consilium* consilium,
         ConsiliumGradus* gradus,
       ConsiliumEffectus  effectus)
{
    i32 i;

    (vacuum)consilium;
    per (i = ZEPHYRUM; i < xar_numerus(gradus->effectus); i++)
    {
        constans ConsiliumEffectus* e = (constans ConsiliumEffectus*)
            xar_obtinere(gradus->effectus, i);

        si (   e->verbum == effectus.verbum
            && e->forma  == effectus.forma
            && e->exstat == effectus.exstat
            && chorda_aequalis(e->res, effectus.res))
        {
            redde;
        }
    }
    *(ConsiliumEffectus*)xar_addere(gradus->effectus) = effectus;
}

constans Xar*
consilium_gradus (
    constans Consilium* consilium)
{
    redde consilium->gradus;
}

ConsiliumGradus*
consilium_gradum_invenire (
     constans Consilium* consilium,
                 chorda  titulus)
{
    ConsiliumGradus* g = NIHIL;

    si (!tabula_dispersa_invenire(consilium->index, titulus,
            (vacuum**)&g))
    {
        redde NIHIL;
    }
    redde g;
}


/* ==================================================
 * Congruentia rerum
 * ================================================== */

interior b32
_scriptor (
    ConsiliumVerbum v)
{
    redde v == CONSILIUM_SCRIBERE || v == CONSILIUM_DELERE
        || v == CONSILIUM_MOVERE || v == CONSILIUM_INSTITUERE;
}

interior b32
_lector (
    ConsiliumVerbum v)
{
    redde v == CONSILIUM_LEGERE || v == CONSILIUM_INSPICERE
        || v == CONSILIUM_ENUMERARE || v == CONSILIUM_EXSEQUI
        || v == CONSILIUM_FONTARE;
}

/* pars litteralis exemplaris (ante '*' aut '?'); via tota si VIA */
interior chorda
_litteralis (
    constans ConsiliumEffectus* e)
{
    i32 i;

    si (e->forma != CONSILIUM_GLOBUS)
    {
        redde e->res;
    }
    per (i = ZEPHYRUM; i < e->res.mensura; i++)
    {
        si (e->res.datum[i] == '*' || e->res.datum[i] == '?')
        {
            frange;
        }
    }
    redde chorda_sectio(e->res, ZEPHYRUM, i);
}

interior b32
_incipit (
    chorda s,
    chorda p)
{
    redde s.mensura >= p.mensura && memcmp(s.datum, p.datum,
        (size_t)p.mensura) == ZEPHYRUM;
}

/* forma effectiva: enumeratio directorii omnem rem sub eo tangit */
interior ConsiliumForma
_forma (
    constans ConsiliumEffectus* e)
{
    redde e->verbum == CONSILIUM_ENUMERARE && e->forma == CONSILIUM_VIA
        ? CONSILIUM_PRAEFIXUM : e->forma;
}

/* congruuntne res duorum effectuum? inter duo exemplaria
 * conservative */
interior b32
_congruunt (
    constans ConsiliumEffectus* a,
    constans ConsiliumEffectus* b)
{
    ConsiliumForma fa = _forma(a);
    ConsiliumForma fb = _forma(b);

    si (fa == CONSILIUM_VIA && fb == CONSILIUM_VIA)
    {
        redde chorda_aequalis(a->res, b->res);
    }
    si (fa == CONSILIUM_VIA)
    {
        redde fb == CONSILIUM_PRAEFIXUM ? _incipit(a->res, b->res)
            : directorium_titulus_congruit(a->res, b->res);
    }
    si (fb == CONSILIUM_VIA)
    {
        redde fa == CONSILIUM_PRAEFIXUM ? _incipit(b->res, a->res)
            : directorium_titulus_congruit(b->res, a->res);
    }
    {
        chorda la = _litteralis(a);
        chorda lb = _litteralis(b);

        redde _incipit(la, lb) || _incipit(lb, la);
    }
}

/* scribitne gradus g rem lectam per e? (spec M1: tum ingressus non
 * est) */
interior b32
_ipse_scribit (
      constans ConsiliumGradus* g,
    constans ConsiliumEffectus* e)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(g->effectus); k++)
    {
        constans ConsiliumEffectus* w = (constans ConsiliumEffectus*)
            xar_obtinere(g->effectus, k);

        si (_scriptor(w->verbum) && _congruunt(w, e))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Ordo
 * ================================================== */

interior chorda
_ratio (
                Piscina* piscina,
     constans character* a,
                 chorda  b,
     constans character* c,
                 chorda  d,
     constans character* e)
{
    ChordaAedificator* x = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);

    chorda_aedificator_appendere_literis(x, a);
    chorda_aedificator_appendere_chorda(x, b);
    chorda_aedificator_appendere_literis(x, c);
    chorda_aedificator_appendere_chorda(x, d);
    chorda_aedificator_appendere_literis(x, e);
    redde chorda_aedificator_finire(x);
}

/* arcus semel per par (causa prima manet); matrix[n*i+j]: i -> j */
interior vacuum
_arcum_addere (
               ConsiliumOrdo* o,
                         i32* matrix,
                         i32  n,
    constans ConsiliumGradus* prior,
    constans ConsiliumGradus* posterior,
                      chorda  causa)
{
    ConsiliumArcus* a;
               i32  i = prior->ordo_additionis;
               i32  j = posterior->ordo_additionis;

    si (i == j || matrix[n * i + j])
    {
        redde;
    }
    matrix[n * i + j]  = I;
    a                  = (ConsiliumArcus*)xar_addere(o->arcus);
    a->prior           = prior;
    a->posterior       = posterior;
    a->causa           = causa;
}

/* arcus omnes in matricem (i32[n*n], 1 = arcus) */
interior i32*
_arcus_computare (
    constans Consilium* c,
         ConsiliumOrdo* o,
               Piscina* piscina)
{
    i32  n = xar_numerus(c->gradus);
    i32* m = (i32*)piscina_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n * n : I)
        * (memoriae_index)magnitudo(i32));
    i32 i;
    i32 j;
    i32 p;
    i32 q;

    memset(m, ZEPHYRUM, (size_t)(n > ZEPHYRUM ? n * n : I)
        * magnitudo(i32));
    /* data: scriptor ante lectorem */
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans ConsiliumGradus* a = *(ConsiliumGradus**)xar_obtinere(
            c->gradus, i);

        per (j = ZEPHYRUM; j < n; j++)
        {
            constans ConsiliumGradus* b = *(ConsiliumGradus**)
                xar_obtinere(c->gradus, j);

            si (i == j)
            {
                perge;
            }
            per (p = ZEPHYRUM; p < xar_numerus(a->effectus); p++)
            {
                constans ConsiliumEffectus* w = (constans
                    ConsiliumEffectus*)xar_obtinere(a->effectus, p);

                si (!_scriptor(w->verbum))
                {
                    perge;
                }
                per (q = ZEPHYRUM; q < xar_numerus(b->effectus); q++)
                {
                    constans ConsiliumEffectus* r = (constans
                        ConsiliumEffectus*)xar_obtinere(b->effectus, q);

                    si (   _lector(r->verbum) && _congruunt(w, r)
                        && !_ipse_scribit(b, r))
                    {
                        _arcum_addere(o, m, n, a, b, _ratio(piscina,
                            r->exstat ? "" : "absentia: ",
                            a->titulus, " scribit ", r->res,
                            " quod legitur"));
                    }
                }
            }
        }
    }
    /* sequitur expresse */
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans ConsiliumGradus* b = *(ConsiliumGradus**)xar_obtinere(
            c->gradus, j);

        per (q = ZEPHYRUM; q < xar_numerus(b->sequitur); q++)
        {
                              chorda  t;
            constans ConsiliumGradus* a;

            t = *(chorda*)xar_obtinere(b->sequitur, q);
            a = consilium_gradum_invenire(c, t);

            si (a == NIHIL)
            {
                *(chorda*)xar_addere(o->recusationes) = _ratio(piscina,
                    "", b->titulus, " sequitur ", t,
                    ": non in consilio");
                perge;
            }
            _arcum_addere(o, m, n, a, b, chorda_ex_literis(
                "sequitur expresse", piscina));
        }
    }
    /* murus gradus caeci */
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans ConsiliumGradus* g = *(ConsiliumGradus**)xar_obtinere(
            c->gradus, i);

        si (!g->caecus)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < n; j++)
        {
            constans ConsiliumGradus* h = *(ConsiliumGradus**)
                xar_obtinere(c->gradus, j);

            si (j < i)
            {
                _arcum_addere(o, m, n, h, g, chorda_ex_literis(
                    "murus gradus caeci", piscina));
            }
            alioquin si (j > i)
            {
                _arcum_addere(o, m, n, g, h, chorda_ex_literis(
                    "murus gradus caeci", piscina));
            }
        }
    }
    redde m;
}

ConsiliumOrdo*
consilium_ordinare (
    constans Consilium* consilium,
               Piscina* piscina)
{
    ConsiliumOrdo* o = (ConsiliumOrdo*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(ConsiliumOrdo));
              i32  n = xar_numerus(consilium->gradus);
              i32* m;
              i32* gradus_in;   /* arcus intrantes restantes */
              b32* facti;
              i32  facti_n = ZEPHYRUM;
              i32  i;
              i32  j;

    o->undae = xar_creare(piscina, (i32)magnitudo(Xar*));
    o->arcus = xar_creare(piscina,
        (i32)magnitudo(ConsiliumArcus));
    o->cyclus = xar_creare(piscina,
        (i32)magnitudo(ConsiliumGradus*));
    o->recusationes  = xar_creare(piscina, (i32)magnitudo(chorda));
    m                = _arcus_computare(consilium, o, piscina);
    gradus_in = (i32*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * (memoriae_index)magnitudo(i32));
    facti = (b32*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * (memoriae_index)magnitudo(b32));
    per (j = ZEPHYRUM; j < n; j++)
    {
        gradus_in[j]  = ZEPHYRUM;
        facti[j]      = FALSUM;
        per (i = ZEPHYRUM; i < n; i++)
        {
            gradus_in[j] += m[n * i + j];
        }
    }
    /* undae (Kahn): omnes sine prioribus restantibus, ordine
     * additionis */
    dum (facti_n < n)
    {
        Xar* unda = xar_creare(piscina,
            (i32)magnitudo(ConsiliumGradus*));

        per (j = ZEPHYRUM; j < n; j++)
        {
            si (!facti[j] && gradus_in[j] == ZEPHYRUM)
            {
                *(ConsiliumGradus**)xar_addere(unda) =
                    *(ConsiliumGradus**)xar_obtinere(consilium->gradus,
                    j);
            }
        }
        si (xar_numerus(unda) == ZEPHYRUM)
        {
            frange;
        }
        per (i = ZEPHYRUM; i < xar_numerus(unda); i++)
        {
            i32 k = (*(ConsiliumGradus**)xar_obtinere(unda,
                i))->ordo_additionis;

            facti[k] = VERUM;
            facti_n++;
            per (j = ZEPHYRUM; j < n; j++)
            {
                gradus_in[j] -= m[n * k + j];
            }
        }
        *(Xar**)xar_addere(o->undae) = unda;
    }
    /* cyclus: e restantibus retro per arcus intrantes usque ad
     * reditum */
    si (facti_n < n)
    {
        s32* visus = (s32*)piscina_allocare(piscina, (memoriae_index)(n
            + I)
            * (memoriae_index)magnitudo(s32));
        i32 k       = n;
        s32 passus  = ZEPHYRUM;

        per (j = ZEPHYRUM; j < n; j++)
        {
            visus[j] = -I;
            si (k == n && !facti[j])
            {
                k = j;
            }
        }
        dum (visus[k] < ZEPHYRUM)
        {
            visus[k] = passus++;
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (!facti[i] && m[n * i + k])
                {
                    frange;
                }
            }
            k = i;
        }
        /* k in cyclo; ab eo retro usque ad k */
        {
            i32  initium  = k;
            Xar* retro    = xar_creare(piscina, (i32)magnitudo(i32));

            fac
            {
                *(i32*)xar_addere(retro) = k;
                per (i = ZEPHYRUM; i < n; i++)
                {
                    si (!facti[i] && m[n * i + k])
                    {
                        frange;
                    }
                }
                k = i;
            }
            dum (k != initium);
            /* ordo prior -> posterior (retro collectum: inverte) */
            per (i = xar_numerus(retro); i > ZEPHYRUM; i--)
            {
                *(ConsiliumGradus**)xar_addere(o->cyclus) =
                    *(ConsiliumGradus**)xar_obtinere(consilium->gradus,
                    *(i32*)xar_obtinere(retro, i - I));
            }
        }
    }
    redde o;
}


/* ==================================================
 * Conflictus
 * ================================================== */

/* attingitne i j per arcus ordinis? (matrix ex ordine reconstructa) */
interior b32
_attingit (
    constans ConsiliumOrdo* o,
                       i32  n,
                       i32  ab,
                       i32  ad,
                   Piscina* piscina)
{
    i32* acervus = (i32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * (memoriae_index)magnitudo(i32));
    b32* visus = (b32*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * (memoriae_index)magnitudo(b32));
    i32 summum = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        visus[i] = FALSUM;
    }
    acervus[summum++]  = ab;
    visus[ab]          = VERUM;
    dum (summum > ZEPHYRUM)
    {
        i32 k = acervus[--summum];

        si (k == ad)
        {
            redde VERUM;
        }
        per (i = ZEPHYRUM; i < xar_numerus(o->arcus); i++)
        {
            constans ConsiliumArcus* a = (constans ConsiliumArcus*)
                xar_obtinere(o->arcus, i);
            i32 t = a->posterior->ordo_additionis;

            si (a->prior->ordo_additionis == k && !visus[t])
            {
                visus[t]           = VERUM;
                acervus[summum++]  = t;
            }
        }
    }
    redde FALSUM;
}

Xar*
consilium_conflictus (
     constans Consilium* consilium,
 constans ConsiliumOrdo* ordo,
                Piscina* piscina)
{
    Xar* fructus = xar_creare(piscina,
        (i32)magnitudo(ConsiliumConflictus));
    i32 n = xar_numerus(consilium->gradus);
    i32 i;
    i32 j;
    i32 p;
    i32 q;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans ConsiliumGradus* a = *(ConsiliumGradus**)xar_obtinere(
            consilium->gradus, i);

        per (j = i + I; j < n; j++)
        {
            constans ConsiliumGradus* b = *(ConsiliumGradus**)
                xar_obtinere(consilium->gradus, j);
                                  b32 inventus = FALSUM;

            per (p = ZEPHYRUM; p < xar_numerus(a->effectus)
                && !inventus;
                 p++)
            {
                constans ConsiliumEffectus* x = (constans
                    ConsiliumEffectus*)xar_obtinere(a->effectus, p);

                si (!_scriptor(x->verbum))
                {
                    perge;
                }
                per (q = ZEPHYRUM; q < xar_numerus(b->effectus)
                     && !inventus; q++)
                {
                    constans ConsiliumEffectus* y = (constans
                        ConsiliumEffectus*)xar_obtinere(b->effectus, q);

                    si (   !_scriptor(y->verbum) || !_congruunt(x, y)
                        || (   x->verbum == CONSILIUM_DELERE
                            && y->verbum == CONSILIUM_DELERE))
                    {
                        perge;
                    }
                    si (   _attingit(ordo, n, i, j, piscina)
                        || _attingit(ordo, n, j, i, piscina))
                    {
                        perge;
                    }
                    {
                        ConsiliumConflictus* k = (ConsiliumConflictus*)
                            xar_addere(fructus);

                        k->a      = a;
                        k->b      = b;
                        k->res    = x->res;
                        inventus  = VERUM;
                    }
                }
            }
        }
    }
    redde fructus;
}


/* ==================================================
 * Politica
 * ================================================== */

interior b32
_regula_congruit (
    constans ConsiliumRegula* r,
  constans ConsiliumEffectus* e)
{
    si (r->verbum != CONSILIUM_OMNIA && r->verbum != (s32)e->verbum)
    {
        redde FALSUM;
    }
    si (   r->classis != CONSILIUM_OMNIA
        && r->classis != (s32)e->classis)
    {
        redde FALSUM;
    }
    si (r->exemplar.mensura > ZEPHYRUM)
    {
        ConsiliumEffectus x = consilium_effectus(e->verbum,
            r->exemplar);

        x.forma = CONSILIUM_GLOBUS;
        redde _congruunt(e, &x);
    }
    redde VERUM;
}

/* scriptura intra vestigium? (vestigium vacuum = nulla restrictio) */
interior b32
_intra_vestigium (
    constans ConsiliumGradus* g,
  constans ConsiliumEffectus* e)
{
    i32 i;

    si (xar_numerus(g->vestigium) == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(g->vestigium); i++)
    {
        ConsiliumEffectus v = consilium_effectus(CONSILIUM_SCRIBERE,
            *(chorda*)xar_obtinere(g->vestigium, i));

        v.forma = CONSILIUM_GLOBUS;
        si (e->forma == CONSILIUM_VIA
            ? directorium_titulus_congruit(e->res, v.res)
            : _congruunt(e, &v))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

Xar*
consilium_iudicare (
    constans Consilium* consilium,
          constans Xar* regulae,
               Piscina* piscina)
{
    Xar* fructus = xar_creare(piscina,
        (i32)magnitudo(ConsiliumIudicium));
    i32 i;
    i32 k;
    i32 r;

    per (i = ZEPHYRUM; i < xar_numerus(consilium->gradus); i++)
    {
        constans ConsiliumGradus* g = *(ConsiliumGradus**)xar_obtinere(
            consilium->gradus, i);

        per (k = ZEPHYRUM; k < xar_numerus(g->effectus); k++)
        {
            constans ConsiliumEffectus* e =
                (constans ConsiliumEffectus*)
                xar_obtinere(g->effectus, k);

            si (_scriptor(e->verbum) && !_intra_vestigium(g, e))
            {
                ConsiliumIudicium* j = (ConsiliumIudicium*)xar_addere(
                    fructus);

                j->gradus     = g;
                j->effectus   = e;
                j->sententia  = CONSILIUM_VETITUM;
                j->regula     = chorda_ex_literis("vestigium", piscina);
                j->causa      = chorda_ex_literis("scriptura extra "
                    "vestigium gradus", piscina);
                perge;
            }
            per (r = ZEPHYRUM; regulae != NIHIL
                 && r < xar_numerus(regulae); r++)
            {
                constans ConsiliumRegula* reg =
                    (constans ConsiliumRegula*)
                    xar_obtinere(regulae, r);

                si (!_regula_congruit(reg, e))
                {
                    perge;
                }
                si (reg->sententia != CONSILIUM_LICET)
                {
                    ConsiliumIudicium* j =
                        (ConsiliumIudicium*)xar_addere(
                        fructus);

                    j->gradus     = g;
                    j->effectus   = e;
                    j->sententia  = reg->sententia;
                    j->regula     = reg->titulus;
                    j->causa      = reg->causa;
                }
                frange;
            }
        }
    }
    redde fructus;
}


/* ==================================================
 * Effusio
 * ================================================== */

interior constans character*
_sententia_titulus (
    ConsiliumSententia s)
{
    redde s == CONSILIUM_VETITUM ? "VETITUM"
        : s == CONSILIUM_ROGANDUM ? "ROGANDUM" : "LICET";
}

chorda
consilium_scribere (
         constans Consilium* consilium,
     constans ConsiliumOrdo* ordo,
               constans Xar* conflictus,
               constans Xar* iudicia,
                        b32  machina,
                    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(IV * M));
                   i32 i;
                   i32 j;

    (vacuum)consilium;
    per (i = ZEPHYRUM; ordo != NIHIL
        && i < xar_numerus(ordo->undae); i++)
    {
        constans Xar* unda = *(Xar**)xar_obtinere(ordo->undae, i);

        si (!machina)
        {
            chorda_aedificator_appendere_literis(a, "unda ");
            chorda_aedificator_appendere_i32(a, i + I);
            chorda_aedificator_appendere_literis(a, ":");
        }
        per (j = ZEPHYRUM; j < xar_numerus(unda); j++)
        {
            constans ConsiliumGradus* g =
                *(ConsiliumGradus**)xar_obtinere(
                unda, j);

            si (machina)
            {
                chorda_aedificator_appendere_literis(a, "UNDA\t");
                chorda_aedificator_appendere_i32(a, i + I);
                chorda_aedificator_appendere_character(a, '\t');
                chorda_aedificator_appendere_chorda(a, g->titulus);
                chorda_aedificator_appendere_character(a, '\t');
                chorda_aedificator_appendere_chorda(a, g->genus);
                chorda_aedificator_appendere_character(a, '\n');
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(a, j == ZEPHYRUM
                    ? " " : " | ");
                chorda_aedificator_appendere_chorda(a, g->titulus);
            }
        }
        si (!machina)
        {
            chorda_aedificator_appendere_character(a, '\n');
        }
    }
    per (i = ZEPHYRUM; ordo != NIHIL
        && i < xar_numerus(ordo->arcus); i++)
    {
        constans ConsiliumArcus* r = (constans ConsiliumArcus*)
            xar_obtinere(ordo->arcus, i);

        chorda_aedificator_appendere_literis(a, machina ? "ARCUS\t"
            : "  ");
        chorda_aedificator_appendere_chorda(a, r->prior->titulus);
        chorda_aedificator_appendere_literis(a,
            machina ? "\t" : " -> ");
        chorda_aedificator_appendere_chorda(a, r->posterior->titulus);
        chorda_aedificator_appendere_literis(a, machina ? "\t" : "  (");
        chorda_aedificator_appendere_chorda(a, r->causa);
        chorda_aedificator_appendere_literis(a, machina ? "\n" : ")\n");
    }
    si (ordo != NIHIL && xar_numerus(ordo->cyclus) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, machina ? "CYCLUS"
            : "CYCLUS:");
        per (i = ZEPHYRUM; i < xar_numerus(ordo->cyclus); i++)
        {
            chorda_aedificator_appendere_character(a, machina ? '\t'
                : ' ');
            chorda_aedificator_appendere_chorda(a,
                (*(ConsiliumGradus**)xar_obtinere(ordo->cyclus,
                i))->titulus);
        }
        chorda_aedificator_appendere_character(a, '\n');
    }
    per (i = ZEPHYRUM; ordo != NIHIL
         && i < xar_numerus(ordo->recusationes); i++)
    {
        chorda_aedificator_appendere_literis(a, machina ? "RECUSATIO\t"
            : "RECUSATIO: ");
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            ordo->recusationes, i));
        chorda_aedificator_appendere_character(a, '\n');
    }
    per (i = ZEPHYRUM; conflictus != NIHIL
         && i < xar_numerus(conflictus); i++)
    {
        constans ConsiliumConflictus* k =
            (constans ConsiliumConflictus*)
            xar_obtinere(conflictus, i);

        chorda_aedificator_appendere_literis(a, machina ? "CONFLICTUS\t"
            : "CONFLICTUS: ");
        chorda_aedificator_appendere_chorda(a, k->a->titulus);
        chorda_aedificator_appendere_literis(a,
            machina ? "\t" : " et ");
        chorda_aedificator_appendere_chorda(a, k->b->titulus);
        chorda_aedificator_appendere_literis(a, machina ? "\t"
            : " scribunt ");
        chorda_aedificator_appendere_chorda(a, k->res);
        chorda_aedificator_appendere_character(a, '\n');
    }
    per (i = ZEPHYRUM; iudicia != NIHIL
        && i < xar_numerus(iudicia); i++)
    {
        constans ConsiliumIudicium* k = (constans ConsiliumIudicium*)
            xar_obtinere(iudicia, i);

        chorda_aedificator_appendere_literis(a, machina ? "IUDICIUM\t"
            : "");
        si (!machina)
        {
            chorda_aedificator_appendere_literis(a,
                _sententia_titulus(k->sententia));
            chorda_aedificator_appendere_literis(a, ": ");
        }
        chorda_aedificator_appendere_chorda(a, k->gradus->titulus);
        chorda_aedificator_appendere_character(a, machina ? '\t' : ' ');
        chorda_aedificator_appendere_literis(a,
            consilium_verbum_titulus(k->effectus->verbum));
        chorda_aedificator_appendere_character(a, machina ? '\t' : ' ');
        chorda_aedificator_appendere_chorda(a, k->effectus->res);
        si (machina)
        {
            chorda_aedificator_appendere_character(a, '\t');
            chorda_aedificator_appendere_literis(a,
                _sententia_titulus(k->sententia));
        }
        chorda_aedificator_appendere_literis(a,
            machina ? "\t" : " <- ");
        chorda_aedificator_appendere_chorda(a, k->regula);
        chorda_aedificator_appendere_literis(a, machina ? "\t" : ": ");
        chorda_aedificator_appendere_chorda(a, k->causa);
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Domus (S4)
 * ================================================== */

interior b32
_sub (
                 chorda  via,
     constans character* radix)
{
    i32 n;

    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    n = (i32)strlen(radix);
    redde via.mensura > n && memcmp(via.datum, radix, (size_t)n)
        == ZEPHYRUM && via.datum[n] == '/';
}

ConsiliumClassis
consilium_classis_viae (
                  chorda  via,
      constans character* radix,
      constans character* domus)
{
    chorda relativa = via;
       i32 i;

    si (   via.mensura >= I && via.datum[ZEPHYRUM] == '~'
        && (via.mensura == I || via.datum[I] == '/'))
    {
        redde CONSILIUM_DOMUS;
    }
    si (via.mensura > ZEPHYRUM && via.datum[ZEPHYRUM] == '/')
    {
        si (!_sub(via, radix))
        {
            redde _sub(via,
                domus) ? CONSILIUM_DOMUS : CONSILIUM_EXTERNA;
        }
        relativa = chorda_sectio(via, (i32)strlen(radix) + I,
            via.mensura);
    }
    si (   relativa.mensura >= IV && memcmp(relativa.datum, "bin/", IV)
        == ZEPHYRUM)
    {
        redde CONSILIUM_INSTRUMENTUM_DOMUS;
    }
    si (relativa.mensura >= VI && memcmp(relativa.datum, "build/", VI)
        == ZEPHYRUM)
    {
        redde CONSILIUM_BUILD;
    }
    per (i = ZEPHYRUM; i + VII <= relativa.mensura; i++)
    {
        si (memcmp(relativa.datum + i, "/build/", VII) == ZEPHYRUM)
        {
            redde CONSILIUM_BUILD;
        }
    }
    redde CONSILIUM_ARBOR;
}

ConsiliumRegula
consilium_regula_arboris_secundariae (
    Piscina* piscina)
{
    ConsiliumRegula r;

    r.titulus    = chorda_ex_literis("arbor_secundaria", piscina);
    r.verbum     = CONSILIUM_OMNIA;
    r.classis    = (s32)CONSILIUM_DOMUS;
    r.exemplar   = chorda_ex_literis("", piscina);
    r.sententia  = CONSILIUM_VETITUM;
    r.causa      = chorda_ex_literis("in arbore secundaria nihil extra "
        "arborem scribitur (institutiones ex main solo)", piscina);
    redde r;
}


/* ==================================================
 * Documentum (consilium.canon, S3a)
 * ================================================== */

hic_manens constans character* constans _FORMAE[] = {
    "via", "praefixum", "globus", NIHIL
};
hic_manens constans character* constans _CERTITUDINES[] = {
    "plena", "partialis", "nulla", NIHIL
};
hic_manens constans character* constans _COGNITIONES[] = {
    "propositum", "declaratum", "derivatum", "observatum", NIHIL
};
hic_manens constans character* constans _SENTENTIAE[] = {
    "licet", "rogandum", "vetitum", NIHIL
};

hic_manens constans character* constans _ATTR_REGULAE[] = {
    "titulus", "verbum", "classis", "exemplar", "sententia", "causa",
        NIHIL
};
hic_manens constans character* constans _ATTR_GRADUS[] = {
    "titulus", "genus", "auctor", "caecus", "sumptus_ms", NIHIL
};
hic_manens constans character* constans _ATTR_EFFECTUS[] = {
    "verbum", "res", "forma", "classis", "certitudo", "cognitio",
        "exstat",
    "causa", NIHIL
};
hic_manens constans character* constans _ATTR_SEQUITUR[] = {
    "titulus", NIHIL
};
hic_manens constans character* constans _ATTR_VESTIGII[] = {
    "exemplar", NIHIL
};
hic_manens constans character* constans _ATTR_RADICIS[] = {
    NIHIL
};

nomen structura {
    ChordaAedificator* textus;
                  i32  numerus;
              Piscina* piscina;
} ConsiliumCausae;

interior vacuum
_causa (
        ConsiliumCausae* k,
                    i32  linea,
     constans character* ante,
                 chorda  medium,
     constans character* post)
{
    si (k->numerus > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(k->textus, '\n');
    }
    k->numerus++;
    chorda_aedificator_appendere_literis(k->textus, "linea ");
    chorda_aedificator_appendere_i32(k->textus, linea);
    chorda_aedificator_appendere_literis(k->textus, ": ");
    chorda_aedificator_appendere_literis(k->textus, ante);
    chorda_aedificator_appendere_chorda(k->textus, medium);
    chorda_aedificator_appendere_literis(k->textus, post);
}

interior chorda
_valor (
             StmlNodus* n,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(n, titulus);
    chorda  vacua;

    si (v != NIHIL)
    {
        redde *v;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* index nominis in tabula (NIHIL terminata); -I = ignotum */
interior s32
_index (
                     chorda  v,
    constans character* constans* tabula)
{
    s32 i;

    per (i = ZEPHYRUM; tabula[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(v, tabula[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* valor tabulae aut causa ('verbum 'x' ignotum (nota: a, b)'); vacuus =
 * praestitutum */
interior s32
_enumeratum (
       ConsiliumCausae* k,
            StmlNodus* n,
    constans character* attributum,
    constans character* constans* tabula,
                   s32  praestitutum)
{
    chorda v = _valor(n, attributum);
       s32 i;

    si (v.mensura == ZEPHYRUM)
    {
        redde praestitutum;
    }
    i = _index(v, tabula);
    si (i < ZEPHYRUM)
    {
        ChordaAedificator* a = chorda_aedificator_creare(k->piscina,
            (memoriae_index)LXIV);
                       s32 j;

        chorda_aedificator_appendere_literis(a, attributum);
        chorda_aedificator_appendere_literis(a, " '");
        chorda_aedificator_appendere_chorda(a, v);
        chorda_aedificator_appendere_literis(a, "' ignotum (nota:");
        per (j = ZEPHYRUM; tabula[j] != NIHIL; j++)
        {
            chorda_aedificator_appendere_character(a, ' ');
            chorda_aedificator_appendere_literis(a, tabula[j]);
        }
        chorda_aedificator_appendere_literis(a, ")");
        _causa(k, n->linea, "", chorda_aedificator_finire(a), "");
        redde praestitutum;
    }
    redde i;
}

interior b32
_veritas (
       ConsiliumCausae* k,
             StmlNodus* n,
    constans character* attributum,
                   b32  praestitutum)
{
    chorda v = _valor(n, attributum);

    si (v.mensura == ZEPHYRUM)
    {
        redde praestitutum;
    }
    si (   chorda_aequalis_literis(v, "verum")
        || chorda_aequalis_literis(v, "true"))
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(v, "falsum"))
    {
        redde FALSUM;
    }
    _causa(k, n->linea, "veritas mala '", v, "' (verum aut falsum)");
    redde praestitutum;
}

interior vacuum
_attributa_clausa (
       ConsiliumCausae* k,
            StmlNodus* n,
    constans character* constans* nota)
{
    i32 i;

    si (n->attributa == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(n->attributa); i++)
    {
        StmlAttributum* at = (StmlAttributum*)xar_obtinere(n->attributa,
            i);

        si (_index(*at->titulus, nota) < ZEPHYRUM)
        {
            _causa(k, n->linea, "attributum ignotum '", *at->titulus,
                "'");
        }
    }
}

interior chorda
_necessarium (
       ConsiliumCausae* k,
             StmlNodus* n,
    constans character* attributum)
{
    chorda v = _valor(n, attributum);

    si (v.mensura == ZEPHYRUM)
    {
        _causa(k, n->linea, "attributum necessarium '",
            chorda_ex_literis(attributum, k->piscina), "' abest");
    }
    redde v;
}

/* verbum aut classis cum titulis consilium.h */
interior s32
_nomen_enumeratum (
        ConsiliumCausae* k,
              StmlNodus* n,
     constans character* attributum,
                    b32  verbum,
                    s32  praestitutum)
{
    constans character* tabula[XVI];
                   s32  numerus = verbum ? (s32)CONSILIUM_VERBA_NUMERUS
                       : (s32)CONSILIUM_CLASSES_NUMERUS;
                   s32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        tabula[i] =
            verbum ? consilium_verbum_titulus((ConsiliumVerbum)i)
            : consilium_classis_titulus((ConsiliumClassis)i);
    }
    tabula[numerus] = NIHIL;
    redde _enumeratum(k, n, attributum, tabula, praestitutum);
}

interior vacuum
_gradum_legere (
            Consilium* c,
            StmlNodus* n,
      ConsiliumCausae* k)
{
    ConsiliumGradus* g;
             chorda  titulus;
             chorda  sumptus;
                i32  i;

    _attributa_clausa(k, n, _ATTR_GRADUS);
    titulus = _necessarium(k, n, "titulus");
    si (titulus.mensura == ZEPHYRUM)
    {
        redde;
    }
    g = consilium_gradum_addere(c, titulus, _valor(n, "genus"),
        _valor(n, "auctor"));
    g->caecus  = _veritas(k, n, "caecus", g->caecus);
    sumptus    = _valor(n, "sumptus_ms");
    si (sumptus.mensura > ZEPHYRUM)
    {
        s32 v = ZEPHYRUM;

        per (i = ZEPHYRUM; i < sumptus.mensura; i++)
        {
            si (sumptus.datum[i] < '0' || sumptus.datum[i] > '9')
            {
                _causa(k, n->linea, "sumptus_ms non numerus '", sumptus,
                    "'");
                v = -I;
                frange;
            }
            v = v * X + (s32)(sumptus.datum[i] - '0');
        }
        g->sumptus_ms = v;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(n); i++)
    {
        StmlNodus* m = stml_liberum_ad_indicem(n, i);

        si (m->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (chorda_aequalis_literis(*m->titulus, "effectus"))
        {
            ConsiliumEffectus e;
                       chorda res;

            _attributa_clausa(k, m, _ATTR_EFFECTUS);
            res = _necessarium(k, m, "res");
            si (_valor(m, "verbum").mensura == ZEPHYRUM)
            {
                (vacuum)_necessarium(k, m, "verbum");
                perge;
            }
            e = consilium_effectus((ConsiliumVerbum)_nomen_enumeratum(k,
                m, "verbum", VERUM, (s32)CONSILIUM_LEGERE), res);
            e.forma      = (ConsiliumForma)_enumeratum(k, m, "forma",
                _FORMAE, (s32)CONSILIUM_VIA);
            e.classis    = (ConsiliumClassis)_nomen_enumeratum(k, m,
                "classis", FALSUM, (s32)CONSILIUM_ARBOR);
            e.certitudo  = (ConsiliumCertitudo)_enumeratum(k, m,
                "certitudo", _CERTITUDINES, (s32)CONSILIUM_PLENA);
            e.cognitio   = (ConsiliumCognitio)_enumeratum(k, m,
                "cognitio", _COGNITIONES, (s32)CONSILIUM_PROPOSITUM);
            e.exstat  = _veritas(k, m, "exstat", VERUM);
            e.causa   = _valor(m, "causa");
            si (res.mensura > ZEPHYRUM)
            {
                consilium_effectum_addere(c, g, e);
            }
        }
        alioquin si (chorda_aequalis_literis(*m->titulus, "sequitur"))
        {
            chorda t;

            _attributa_clausa(k, m, _ATTR_SEQUITUR);
            t = _necessarium(k, m, "titulus");
            si (t.mensura > ZEPHYRUM)
            {
                *(chorda*)xar_addere(g->sequitur) = t;
            }
        }
        alioquin si (chorda_aequalis_literis(*m->titulus, "vestigium"))
        {
            chorda x;

            _attributa_clausa(k, m, _ATTR_VESTIGII);
            x = _necessarium(k, m, "exemplar");
            si (x.mensura > ZEPHYRUM)
            {
                *(chorda*)xar_addere(g->vestigium) = x;
            }
        }
        alioquin
        {
            _causa(k, m->linea, "elementum ignotum <", *m->titulus,
                "> in <gradus>");
        }
    }
}

interior vacuum
_regulam_legere (
            StmlNodus* n,
                  Xar* regulae,
      ConsiliumCausae* k)
{
    ConsiliumRegula r;

    _attributa_clausa(k, n, _ATTR_REGULAE);
    r.titulus   = _necessarium(k, n, "titulus");
    r.verbum    = _valor(n, "verbum").mensura > ZEPHYRUM
        ? _nomen_enumeratum(k, n, "verbum", VERUM, CONSILIUM_OMNIA)
        : CONSILIUM_OMNIA;
    r.classis   = _valor(n, "classis").mensura > ZEPHYRUM
        ? _nomen_enumeratum(k, n, "classis", FALSUM, CONSILIUM_OMNIA)
        : CONSILIUM_OMNIA;
    r.exemplar  = _valor(n, "exemplar");
    si (_valor(n, "sententia").mensura == ZEPHYRUM)
    {
        (vacuum)_necessarium(k, n, "sententia");
    }
    r.sententia = (ConsiliumSententia)_enumeratum(k, n, "sententia",
        _SENTENTIAE, (s32)CONSILIUM_LICET);
    r.causa                                 = _valor(n, "causa");
    *(ConsiliumRegula*)xar_addere(regulae)  = r;
}

Consilium*
consilium_legere (
      chorda   textus,
     Piscina*  piscina,
         Xar** regulae_out,
      chorda*  causa_out)
{
          ConsiliumCausae  k;
                Consilium* c;
                      Xar* regulae;
             StmlResultus  r;
                      i32  i;

    k.textus = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
    k.numerus = ZEPHYRUM;
    k.piscina = piscina;
    c = consilium_creare(piscina);
    regulae = xar_creare(piscina, (i32)magnitudo(ConsiliumRegula));
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        _causa(&k, r.linea_erroris, "STML illegibile",
            chorda_ex_literis(
            "", piscina), "");
    }
    alioquin si (!chorda_aequalis_literis(*r.elementum_radix->titulus,
                 "consilium"))
    {
        _causa(&k, r.elementum_radix->linea,
            "radix non est <consilium>: <",
            *r.elementum_radix->titulus, ">");
    }
    alioquin
    {
        StmlNodus* radix = r.elementum_radix;

        _attributa_clausa(&k, radix, _ATTR_RADICIS);
        per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
        {
            StmlNodus* n = stml_liberum_ad_indicem(radix, i);

            si (n->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            si (chorda_aequalis_literis(*n->titulus, "gradus"))
            {
                _gradum_legere(c, n, &k);
            }
            alioquin si (chorda_aequalis_literis(*n->titulus, "regula"))
            {
                _regulam_legere(n, regulae, &k);
            }
            alioquin
            {
                _causa(&k, n->linea, "elementum ignotum <", *n->titulus,
                    ">");
            }
        }
    }
    si (k.numerus > ZEPHYRUM)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(k.textus);
        }
        redde NIHIL;
    }
    si (causa_out != NIHIL)
    {
        causa_out->datum    = NIHIL;
        causa_out->mensura  = ZEPHYRUM;
    }
    si (regulae_out != NIHIL)
    {
        *regulae_out = regulae;
    }
    redde c;
}
