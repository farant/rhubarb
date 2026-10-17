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
                chorda_aedificator_appendere_character(a, ' ');
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
