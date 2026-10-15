/* unitates.c - UNITATES CODICIS: lector politicae (unitates.stml) et
 * declarationum (DIR/unitas.stml), responsum 'quid est haec plagula';
 * caput include/unitates.h
 *
 * Lectio TOTA ante recusationem: causae omnes una nominantur. I/O
 * nulla hic - sutura plagulas enumerat et legit. Provenientia PIGRE
 * legitur (linea prima per suturam), ut lectio decem milia plagularum
 * non legat.
 */
#include "unitates.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "stml.h"
#include <string.h>

#define UNITATES_POLITICA "unitates.stml"
#define UNITATES_DECLARATIO "unitas.stml"
#define UNITATES_GENUS_BIBLIOTHECAE "/elementa_programmatis/bibliotheca"

/* regula muneris una (ordine politicae) */
nomen structura {
    chorda  titulus;
    chorda  segmentum;     /* vacuum = quodvis */
       Xar* extensiones;   /* chorda; vacuus = quaevis */
       i32  ordo;          /* I-basatus */
} UnitatesMunus;

nomen structura {
    chorda  titulus;
       Xar* extensiones;   /* chorda */
} UnitatesLingua;

nomen structura {
    chorda via;           /* via naturae */
    chorda cura;          /* praestituta */
} UnitatesGenus;

nomen structura {
    chorda  titulus;
       Xar* curae;
       Xar* vitae;
       Xar* munera;
       Xar* provenientiae;
       Xar* linguae;
} UnitatesIudex;

structura Unitates {
    constans UnitatesSutura* sutura;
                        Xar* curae;            /* chorda */
                        Xar* vitae;            /* chorda */
                        Xar* usus;             /* chorda */
                        Xar* genera;           /* UnitatesGenus */
                        Xar* munera;           /* ordine regularum */
                        Xar* munerum_tituli;   /* chorda, singuli */
                        Xar* linguae;          /* UnitatesLingua */
                        Xar* iudices;          /* UnitatesIudex */
                        Xar* provenientiae;    /* manu generatum */
                     chorda  capita;
                     chorda  fontes;
                     chorda  probationes;
                     chorda  praefixum;
                        Xar* partes;           /* chorda */
                        Xar* familiae;         /* chorda */
                        Xar* omnes;            /* Unitas* */
                        Xar* directoria;       /* Unitas* */
             TabulaDispersa* stemmata;         /* stemma -> Unitas* */
};


/* ==================================================
 * Auxilia chordarum
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_finit (
                 chorda  s,
     constans character* suffixum)
{
    i32 n = (i32)strlen(suffixum);

    redde s.mensura >= n && memcmp(s.datum + (s.mensura - n), suffixum,
        (size_t)n) == ZEPHYRUM;
}

interior b32
_finit_chorda (
    chorda s,
    chorda suffixum)
{
    redde s.mensura >= suffixum.mensura
        && memcmp(s.datum + (s.mensura - suffixum.mensura),
            suffixum.datum, (size_t)suffixum.mensura) == ZEPHYRUM;
}

interior b32
_incipit_chorda (
    chorda s,
    chorda praefixum)
{
    redde s.mensura >= praefixum.mensura
        && memcmp(s.datum, praefixum.datum, (size_t)praefixum.mensura)
            == ZEPHYRUM;
}

/* './x' -> 'x' */
interior chorda
_via_norma (
    chorda via)
{
    dum (   via.mensura  >= II && via.datum[ZEPHYRUM] == '.'
         && via.datum[I] == '/')
    {
        via.datum    += II;
        via.mensura  -= II;
    }
    redde via;
}

/* valor spatiis divisus -> Xar de chorda */
interior Xar*
_verba (
      chorda  valor,
     Piscina* piscina)
{
    Xar* verba = xar_creare(piscina, (i32)magnitudo(chorda));
    i32  i;
    s32  initium = -I;

    per (i = ZEPHYRUM; i <= valor.mensura; i++)
    {
        b32 spatium = i == valor.mensura || valor.datum[i] == ' '
            || valor.datum[i] == '\t' || valor.datum[i] == '\n';

        si (!spatium && initium < ZEPHYRUM)
        {
            initium = (s32)i;
        }
        alioquin si (spatium && initium >= ZEPHYRUM)
        {
            *(chorda*)xar_addere(verba) = chorda_sectio(valor,
                (i32)initium, i);
            initium = -I;
        }
    }
    redde verba;
}

interior b32
_in_indice (
     constans Xar* index_chordarum,
           chorda  valor)
{
    i32 i;

    si (index_chordarum == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(index_chordarum); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(index_chordarum, i),
                valor))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* "a, b, c" */
interior chorda
_noti (
    constans Xar* index_chordarum,
         Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(index_chordarum); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ", ");
        }
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(index_chordarum, i));
    }
    redde chorda_aedificator_finire(a);
}

/* via -> directorium ('' in radice) et titulus plagulae */
interior vacuum
_viam_findere (
    chorda  via,
    chorda* directorium,
    chorda* basis)
{
    i32 i;

    per (i = via.mensura; i > ZEPHYRUM; i--)
    {
        si (via.datum[i - I] == '/')
        {
            *directorium  = chorda_sectio(via, ZEPHYRUM, i - I);
            *basis        = chorda_sectio(via, i, via.mensura);
            redde;
        }
    }
    *directorium  = _vacua();
    *basis        = via;
}


/* ==================================================
 * Causae recusationis (exemplar portae.c)
 * ================================================== */

nomen structura {
    ChordaAedificator* textus;
                  i32  numerus;
                  Xar* recusationes;
              Piscina* piscina;
} UnitatesCausae;

/* causa nova: "via:linea: ante medium post" */
interior vacuum
_causa (
        UnitatesCausae* k,
                chorda  via,
                   i32  linea,
    constans character* ante,
                chorda  medium,
    constans character* post)
{
       ChordaAedificator* a = k->textus;
       ChordaAedificator* c = chorda_aedificator_creare(k->piscina,
           (memoriae_index)LXIV);
       UnitatesRecusatio* r = (UnitatesRecusatio*)xar_addere(
           k->recusationes);

    si (k->numerus > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '\n');
    }
    k->numerus++;
    si (via.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, via);
        si (linea > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ':');
            chorda_aedificator_appendere_i32(a, linea);
        }
        chorda_aedificator_appendere_literis(a, ": ");
    }
    chorda_aedificator_appendere_literis(c, ante);
    chorda_aedificator_appendere_chorda(c, medium);
    chorda_aedificator_appendere_literis(c, post);
    r->via    = via;
    r->linea  = via.mensura > ZEPHYRUM ? linea : ZEPHYRUM;
    r->causa  = chorda_aedificator_finire(c);
    chorda_aedificator_appendere_chorda(a, r->causa);
}

/* valor ignotus: "ante 'valor' ignotus (noti: a, b)" */
interior vacuum
_ignotum (
        UnitatesCausae* k,
                chorda  via,
                   i32  linea,
    constans character* quid,
                chorda  valor,
          constans Xar* noti)
{
    ChordaAedificator* c = chorda_aedificator_creare(k->piscina,
        (memoriae_index)LXIV);

    chorda_aedificator_appendere_chorda(c, valor);
    chorda_aedificator_appendere_literis(c, "' ignotus (noti: ");
    chorda_aedificator_appendere_chorda(c, _noti(noti, k->piscina));
    chorda_aedificator_appendere_literis(c, ")");
    _causa(k, via, linea, quid, chorda_aedificator_finire(c), "");
}

interior chorda
_valor (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    redde v != NIHIL ? *v : _vacua();
}

/* vocabulum CLAUSUM attributorum: ignotum = causa */
interior vacuum
_attributa_clausa (
                 StmlNodus* nodus,
    constans character* constans* nota,
                    chorda  via,
            UnitatesCausae* k)
{
    i32 i;
    i32 j;

    si (nodus->attributa == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nodus->attributa); i++)
    {
        StmlAttributum* at = (StmlAttributum*)xar_obtinere(
            nodus->attributa, i);
        b32 notum = FALSUM;

        per (j = ZEPHYRUM; nota[j] != NIHIL && !notum; j++)
        {
            notum = chorda_aequalis_literis(*at->titulus, nota[j]);
        }
        si (!notum)
        {
            _causa(k, via, nodus->linea, "attributum ignotum '",
                *at->titulus, "'");
        }
    }
}

/* attributum necessarium: absens aut vacuum = causa */
interior chorda
_necessarium (
             StmlNodus* nodus,
    constans character* titulus,
                chorda  via,
        UnitatesCausae* k)
{
    chorda v = _valor(nodus, titulus);

    si (v.mensura == ZEPHYRUM)
    {
        _causa(k, via, nodus->linea, "attributum necessarium '",
            chorda_ex_literis(titulus, k->piscina), "' abest");
    }
    redde v;
}

/* valores indicis contra notos: ignotum quodque = causa */
interior vacuum
_indicem_iudicare (
        UnitatesCausae* k,
                chorda  via,
                   i32  linea,
    constans character* quid,
                   Xar* valores,
          constans Xar* noti)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(valores); i++)
    {
        chorda v = *(chorda*)xar_obtinere(valores, i);

        si (!_in_indice(noti, v))
        {
            _ignotum(k, via, linea, quid, v, noti);
        }
    }
}


/* ==================================================
 * Politica (unitates.stml)
 * ================================================== */

hic_manens constans character* constans _attributa_tituli[] = {
    "titulus", "descriptio", NIHIL
};
hic_manens constans character* constans _attributa_generis[] = {
    "via", "cura", "descriptio", NIHIL
};
hic_manens constans character* constans _attributa_muneris[] = {
    "titulus", "segmentum", "extensiones", NIHIL
};
hic_manens constans character* constans _attributa_linguae[] = {
    "titulus", "extensiones", NIHIL
};
hic_manens constans character* constans _attributa_iudicis[] = {
    "titulus", "curae", "vitae", "munera", "provenientiae", "linguae",
        "descriptio",
    NIHIL
};
hic_manens constans character* constans _attributa_bibliothecarum[] = {
    "capita", "fontes", "probationes", "praefixum", "partes", NIHIL
};
hic_manens constans character* constans _attributa_familiae[] = {
    "radix", NIHIL
};

/* titulus in indicem (bis = causa) */
interior vacuum
_titulum_addere (
                   Xar* index_chordarum,
             StmlNodus* nodus,
                chorda  via,
        UnitatesCausae* k,
    constans character* quid)
{
    chorda t = _necessarium(nodus, "titulus", via, k);

    _necessarium(nodus, "descriptio", via, k);
    si (t.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (_in_indice(index_chordarum, t))
    {
        _causa(k, via, nodus->linea, quid, t, "' bis declaratus");
        redde;
    }
    *(chorda*)xar_addere(index_chordarum) = t;
}

interior vacuum
_politicam_legere (
       Unitates* u,
      StmlNodus* radix,
         chorda  via,
 UnitatesCausae* k,
        Piscina* piscina)
{
    i32  i;
    i32  ordo = ZEPHYRUM;
    b32  bibliothecae = FALSUM;
    Xar* iudicum_nodi = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    Xar* generum_nodi = xar_creare(piscina, (i32)magnitudo(StmlNodus*));

    si (!chorda_aequalis_literis(*radix->titulus, "unitates"))
    {
        _causa(k, via, radix->linea, "radix non est <unitates>: <",
            *radix->titulus, ">");
        redde;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);
           chorda  t;

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        t = *n->titulus;
        si (chorda_aequalis_literis(t, "cura"))
        {
            _attributa_clausa(n, _attributa_tituli, via, k);
            _titulum_addere(u->curae, n, via, k, "cura '");
        }
        alioquin si (chorda_aequalis_literis(t, "vita"))
        {
            _attributa_clausa(n, _attributa_tituli, via, k);
            _titulum_addere(u->vitae, n, via, k, "vita '");
        }
        alioquin si (chorda_aequalis_literis(t, "usus"))
        {
            _attributa_clausa(n, _attributa_tituli, via, k);
            _titulum_addere(u->usus, n, via, k, "usus '");
        }
        alioquin si (chorda_aequalis_literis(t, "genus"))
        {
            _attributa_clausa(n, _attributa_generis, via, k);
            *(StmlNodus**)xar_addere(generum_nodi) = n;
        }
        alioquin si (chorda_aequalis_literis(t, "munus"))
        {
            UnitatesMunus* m    = (UnitatesMunus*)xar_addere(u->munera);
                   chorda  ext  = _valor(n, "extensiones");

            _attributa_clausa(n, _attributa_muneris, via, k);
            m->titulus      = _necessarium(n, "titulus", via, k);
            m->segmentum    = _valor(n, "segmentum");
            m->extensiones  = _verba(ext, piscina);
            m->ordo         = ++ordo;
            si (   m->segmentum.mensura        == ZEPHYRUM
                && xar_numerus(m->extensiones) == ZEPHYRUM)
            {
                _causa(k, via, n->linea, "regula munus '", m->titulus,
                    "' vacua (nec segmentum nec extensiones)");
            }
            si (   m->titulus.mensura > ZEPHYRUM
                && !_in_indice(u->munerum_tituli, m->titulus))
            {
                *(chorda*)xar_addere(u->munerum_tituli) = m->titulus;
            }
        }
        alioquin si (chorda_aequalis_literis(t, "lingua"))
        {
            UnitatesLingua* l;
                    chorda  tl;

            _attributa_clausa(n, _attributa_linguae, via, k);
            tl          = _necessarium(n, "titulus", via, k);
            l           =(UnitatesLingua*)xar_addere(u->linguae);
            l->titulus  = tl;
            l->extensiones  = _verba(_necessarium(n, "extensiones", via,
                k), piscina);
        }
        alioquin si (chorda_aequalis_literis(t, "iudex"))
        {
            _attributa_clausa(n, _attributa_iudicis, via, k);
            *(StmlNodus**)xar_addere(iudicum_nodi) = n;
        }
        alioquin si (chorda_aequalis_literis(t, "bibliothecae"))
        {
            _attributa_clausa(n, _attributa_bibliothecarum, via, k);
            si (bibliothecae)
            {
                _causa(k, via, n->linea, "<bibliothecae> bis", _vacua(),
                    "");
            }
            bibliothecae    = VERUM;
            u->capita       = _necessarium(n, "capita", via, k);
            u->fontes       = _necessarium(n, "fontes", via, k);
            u->probationes  = _necessarium(n, "probationes", via, k);
            u->praefixum    = _necessarium(n, "praefixum", via, k);
            u->partes       = _verba(_necessarium(n, "partes", via, k),
                piscina);
        }
        alioquin si (chorda_aequalis_literis(t, "familia"))
        {
            _attributa_clausa(n, _attributa_familiae, via, k);
            *(chorda*)xar_addere(u->familiae) = _necessarium(n, "radix",
                via, k);
        }
        alioquin
        {
            _causa(k, via, n->linea, "elementum ignotum <", t, ">");
        }
    }

    /* genera et iudices post curas, munera, linguas omnes (ordo
     * plagulae liber) */
    per (i = ZEPHYRUM; i < xar_numerus(generum_nodi); i++)
    {
            StmlNodus* n = *(StmlNodus**)xar_obtinere(generum_nodi, i);
        UnitatesGenus* g;
               chorda  via_generis   = _necessarium(n, "via", via, k);
               chorda  cura_generis  = _necessarium(n, "cura", via, k);
                  i32  j;
                  b32  bis = FALSUM;

        per (j = ZEPHYRUM; j < xar_numerus(u->genera); j++)
        {
            bis = bis || chorda_aequalis(
                ((UnitatesGenus*)xar_obtinere(u->genera, j))->via,
                via_generis);
        }
        si (bis)
        {
            _causa(k, via, n->linea, "genus '", via_generis,
                "' bis declaratum");
            perge;
        }
        si (   cura_generis.mensura > ZEPHYRUM
            && !_in_indice(u->curae, cura_generis))
        {
            _ignotum(k, via, n->linea, "cura '", cura_generis,
                u->curae);
        }
        g        = (UnitatesGenus*)xar_addere(u->genera);
        g->via   = via_generis;
        g->cura  = cura_generis;
    }
    per (i = ZEPHYRUM; i < xar_numerus(iudicum_nodi); i++)
    {
            StmlNodus* n = *(StmlNodus**)xar_obtinere(iudicum_nodi, i);
        UnitatesIudex* d;
               chorda  dt = _necessarium(n, "titulus", via, k);
                  Xar* linguarum_tituli = xar_creare(piscina,
                      (i32)magnitudo(chorda));
        i32 j;
        b32 bis = FALSUM;

        _necessarium(n, "descriptio", via, k);
        per (j = ZEPHYRUM; j < xar_numerus(u->iudices); j++)
        {
            bis = bis || chorda_aequalis(
                ((UnitatesIudex*)xar_obtinere(u->iudices, j))->titulus,
                dt);
        }
        si (bis)
        {
            _causa(k, via, n->linea, "iudex '", dt, "' bis declaratus");
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(u->linguae); j++)
        {
            *(chorda*)xar_addere(linguarum_tituli) =
                ((UnitatesLingua*)xar_obtinere(u->linguae, j))->titulus;
        }
        d           = (UnitatesIudex*)xar_addere(u->iudices);
        d->titulus  = dt;
        d->curae          = _verba(_necessarium(n, "curae", via, k),
            piscina);
        d->vitae          = _verba(_necessarium(n, "vitae", via, k),
            piscina);
        d->munera         = _verba(_necessarium(n, "munera", via, k),
            piscina);
        d->provenientiae  = _verba(_necessarium(n, "provenientiae", via,
            k), piscina);
        d->linguae        = _verba(_necessarium(n, "linguae", via, k),
            piscina);
        _indicem_iudicare(k, via, n->linea, "cura '", d->curae,
            u->curae);
        _indicem_iudicare(k, via, n->linea, "vita '", d->vitae,
            u->vitae);
        _indicem_iudicare(k, via, n->linea, "munus '", d->munera,
            u->munerum_tituli);
        _indicem_iudicare(k, via, n->linea, "provenientia '",
            d->provenientiae, u->provenientiae);
        _indicem_iudicare(k, via, n->linea, "lingua '", d->linguae,
            linguarum_tituli);
    }
    si (!bibliothecae)
    {
        _causa(k, via, ZEPHYRUM, "<bibliothecae> abest", _vacua(), "");
    }
}


/* ==================================================
 * Declarationes (DIR/unitas.stml)
 * ================================================== */

hic_manens constans character* constans _attributa_unitatis[] = {
    "titulus", "genus", "descriptio", "cura", "vita", "successor",
        "usus",
    NIHIL
};
hic_manens constans character* constans _attributa_originis[] = {
    "via", "fons", "versio", "licentia", NIHIL
};

interior constans UnitatesGenus*
_genus_invenire (
    constans Unitates* u,
               chorda  via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(u->genera); i++)
    {
        UnitatesGenus* g = (UnitatesGenus*)xar_obtinere(u->genera, i);

        si (chorda_aequalis(g->via, via))
        {
            redde g;
        }
    }
    redde NIHIL;
}

interior Xar*
_generum_viae (
    constans Unitates* u,
              Piscina* piscina)
{
    Xar* v = xar_creare(piscina, (i32)magnitudo(chorda));
    i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(u->genera); i++)
    {
        *(chorda*)xar_addere(v) =
            ((UnitatesGenus*)xar_obtinere(u->genera, i))->via;
    }
    redde v;
}

/* identitas = genus + titulus */
interior constans Unitas*
_unitas_eadem (
    constans Unitates* u,
               chorda  genus,
               chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(u->omnes); i++)
    {
        Unitas* x = *(Unitas**)xar_obtinere(u->omnes, i);

        si (   chorda_aequalis(x->genus, genus)
            && chorda_aequalis(x->titulus, titulus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

interior vacuum
_declarationem_legere (
       Unitates* u,
      StmlNodus* radix,
         chorda  via,
         chorda  directorium,
 UnitatesCausae* k,
        Piscina* piscina)
{
    Unitas* x;
    chorda  usus;
       i32  i;

    si (!chorda_aequalis_literis(*radix->titulus, "unitas"))
    {
        _causa(k, via, radix->linea, "radix non est <unitas>: <",
            *radix->titulus, ">");
        redde;
    }
    _attributa_clausa(radix, _attributa_unitatis, via, k);
    x = (Unitas*)piscina_allocare(piscina, (memoriae_index)magnitudo(
        Unitas));
    x->titulus     = _necessarium(radix, "titulus", via, k);
    x->genus       = _necessarium(radix, "genus", via, k);
    x->descriptio  = _necessarium(radix, "descriptio", via, k);
    x->fons        = UNITAS_DIRECTORII;
    x->via         = directorium;
    x->declaratio  = via;
    x->cura        = _valor(radix, "cura");
    x->vita        = _valor(radix, "vita");
    x->successor   = _valor(radix, "successor");
    x->usus        = NIHIL;
    x->parens      = NIHIL;
    si (   x->genus.mensura > ZEPHYRUM && _genus_invenire(u, x->genus)
            == NIHIL)
    {
        _ignotum(k, via, radix->linea, "genus '", x->genus,
            _generum_viae(u, piscina));
    }
    si (x->cura.mensura > ZEPHYRUM && !_in_indice(u->curae, x->cura))
    {
        _ignotum(k, via, radix->linea, "cura '", x->cura, u->curae);
    }
    si (x->vita.mensura > ZEPHYRUM && !_in_indice(u->vitae, x->vita))
    {
        _ignotum(k, via, radix->linea, "vita '", x->vita, u->vitae);
    }
    si (chorda_aequalis_literis(x->vita, "vetusta")
        != (x->successor.mensura > ZEPHYRUM))
    {
        _causa(k, via, radix->linea, "vita vetusta et successor simul "
            "declarantur (alterum sine altero)", _vacua(), "");
    }
    usus = _valor(radix, "usus");
    si (usus.mensura > ZEPHYRUM)
    {
        x->usus = _verba(usus, piscina);
        _indicem_iudicare(k, via, radix->linea, "usus '", x->usus,
            u->usus);
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!chorda_aequalis_literis(*n->titulus, "origo"))
        {
            _causa(k, via, n->linea, "elementum ignotum <", *n->titulus,
                ">");
            perge;
        }
        _attributa_clausa(n, _attributa_originis, via, k);
        _necessarium(n, "via", via, k);
        _necessarium(n, "fons", via, k);
        _necessarium(n, "versio", via, k);
        _necessarium(n, "licentia", via, k);
    }
    si (_unitas_eadem(u, x->genus, x->titulus) != NIHIL)
    {
        _causa(k, via, radix->linea, "unitas '", x->titulus,
            "' bis declarata in eodem genere");
        redde;
    }
    *(Unitas**)xar_addere(u->omnes)       = x;
    *(Unitas**)xar_addere(u->directoria)  = x;
}

/* unitas directorii intima quae viam continet (exclusa 'praeter') */
interior Unitas*
_directorium_continens (
    constans Unitates* u,
               chorda  via,
      constans Unitas* praeter)
{
    Unitas* optima = NIHIL;
       i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(u->directoria); i++)
    {
        Unitas* x = *(Unitas**)xar_obtinere(u->directoria, i);
           b32  continet;

        si (x == praeter)
        {
            perge;
        }
        continet = x->via.mensura == ZEPHYRUM
            || (   via.mensura > x->via.mensura
                && _incipit_chorda(via, x->via)
                && via.datum[x->via.mensura] == '/');
        si (   continet && (optima == NIHIL
                || x->via.mensura > optima->via.mensura))
        {
            optima = x;
        }
    }
    redde optima;
}


/* ==================================================
 * Bibliothecae (conventio stemmatis)
 * ================================================== */

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

/* stemma S est pars (S = BASIS + PARS, BASIS stemma)? -> basis */
interior b32
_pars_stemmatis (
    constans Unitates* u,
         constans Xar* stemmata,
               chorda  stemma,
               chorda* basis,
               chorda* pars)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(u->partes); i++)
    {
        chorda p = *(chorda*)xar_obtinere(u->partes, i);

        si (stemma.mensura > p.mensura && _finit_chorda(stemma, p))
        {
            chorda b = chorda_sectio(stemma, ZEPHYRUM,
                stemma.mensura - p.mensura);

            si (_in_indice(stemmata, b))
            {
                *basis  = b;
                *pars   = p;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_bibliothecas_colligere (
         Unitates* u,
     constans Xar* viae,
   UnitatesCausae* k,
          Piscina* piscina)
{
       Xar* stemmata  = xar_creare(piscina, (i32)magnitudo(chorda));
       Xar* capita    = xar_creare(piscina, (i32)magnitudo(chorda));
       Xar* partes    = xar_creare(piscina, (i32)magnitudo(chorda));
    Unitas* continens;
       i32  i;
       b32  progressus;

    per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
    {
        chorda via = _via_norma(*(chorda*)xar_obtinere(viae, i));
        chorda dir;
        chorda basis;
        chorda s = _vacua();

        _viam_findere(via, &dir, &basis);
        si (chorda_aequalis(dir, u->capita) && _finit(basis, ".h"))
        {
            s = chorda_sectio(basis, ZEPHYRUM, basis.mensura - II);
            si (!_in_indice(capita, s))
            {
                *(chorda*)xar_addere(capita) = s;
            }
        }
        alioquin si (   chorda_aequalis(dir, u->fontes)
                     && (_finit(basis, ".c") || _finit(basis, ".m")))
        {
            s = chorda_sectio(basis, ZEPHYRUM, basis.mensura - II);
        }
        si (s.mensura > ZEPHYRUM && !_in_indice(stemmata, s))
        {
            *(chorda*)xar_addere(stemmata) = s;
        }
    }
    xar_ordinare(stemmata, _chordas_comparare);
    si (   xar_numerus(stemmata) > ZEPHYRUM
        && _genus_invenire(u, chorda_ex_literis(
            UNITATES_GENUS_BIBLIOTHECAE, piscina)) == NIHIL)
    {
        _causa(k, chorda_ex_literis(UNITATES_POLITICA, piscina),
            ZEPHYRUM,
            "stemmata bibliothecarum exstant sed genus '",
            chorda_ex_literis(UNITATES_GENUS_BIBLIOTHECAE, piscina),
            "' non citabile");
    }
    continens = _directorium_continens(u, chorda_concatenare(u->capita,
        chorda_ex_literis("/x", piscina), piscina), NIHIL);

    /* transitus I: bibliothecae propriae; partes seponuntur */
    per (i = ZEPHYRUM; i < xar_numerus(stemmata); i++)
    {
                   chorda  s = *(chorda*)xar_obtinere(stemmata, i);
                   chorda  basis;
                   chorda  pars;
                   Unitas* x;
        ChordaAedificator* d;

        si (_pars_stemmatis(u, stemmata, s, &basis, &pars))
        {
            *(chorda*)xar_addere(partes) = s;
            perge;
        }
        d = chorda_aedificator_creare(piscina, (memoriae_index)XXXII);
        chorda_aedificator_appendere_literis(d, "stemma ");
        chorda_aedificator_appendere_chorda(d, _in_indice(capita, s)
            ? u->capita : u->fontes);
        chorda_aedificator_appendere_character(d, '/');
        chorda_aedificator_appendere_chorda(d, s);
        chorda_aedificator_appendere_literis(d, _in_indice(capita, s)
            ? ".h" : ".c");
        x = (Unitas*)piscina_allocare(piscina,
            (memoriae_index)magnitudo(
            Unitas));
        x->titulus     = s;
        x->genus       = chorda_ex_literis(UNITATES_GENUS_BIBLIOTHECAE,
            piscina);
        x->descriptio  = _vacua();
        x->fons        = UNITAS_BIBLIOTHECAE;
        x->via         = s;
        x->declaratio  = chorda_aedificator_finire(d);
        x->cura        = _vacua();
        x->vita        = _vacua();
        x->successor   = _vacua();
        x->usus        = NIHIL;
        x->parens      = continens;
        si (_unitas_eadem(u, x->genus, s) != NIHIL)
        {
            _causa(k, x->declaratio, ZEPHYRUM, "bibliotheca '", s,
                "' et unitas directorii eiusdem generis");
            perge;
        }
        *(Unitas**)xar_addere(u->omnes) = x;
        tabula_dispersa_inserere(u->stemmata, s, x);
    }

    /* transitus II: partes ad basim (basis ipsa pars esse potest) */
    progressus = VERUM;
    dum (progressus)
    {
        progressus = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(partes); i++)
        {
            chorda  s = *(chorda*)xar_obtinere(partes, i);
            chorda  basis;
            chorda  pars;
            vacuum* x = NIHIL;

            si (tabula_dispersa_continet(u->stemmata, s))
            {
                perge;
            }
            _pars_stemmatis(u, stemmata, s, &basis, &pars);
            si (tabula_dispersa_invenire(u->stemmata, basis, &x))
            {
                tabula_dispersa_inserere(u->stemmata, s, x);
                progressus = VERUM;
            }
        }
    }

    /* familiae: radix stemma proprium; liberi intra eam */
    per (i = ZEPHYRUM; i < xar_numerus(u->familiae); i++)
    {
        chorda  r = *(chorda*)xar_obtinere(u->familiae, i);
        vacuum* x = NIHIL;

        si (r.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (   !tabula_dispersa_invenire(u->stemmata, r, &x)
            || !chorda_aequalis(((Unitas*)x)->titulus, r))
        {
            _causa(k, chorda_ex_literis(UNITATES_POLITICA, piscina),
                ZEPHYRUM, "familia '", r,
                "' sine bibliotheca propria (stemma ignotum aut pars)");
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(u->omnes); i++)
    {
        Unitas* x       = *(Unitas**)xar_obtinere(u->omnes, i);
        chorda  optima  = _vacua();
           i32  j;

        si (x->fons != UNITAS_BIBLIOTHECAE)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(u->familiae); j++)
        {
            chorda r = *(chorda*)xar_obtinere(u->familiae, j);

            si (   x->titulus.mensura > r.mensura + I
                && _incipit_chorda(x->titulus, r)
                && x->titulus.datum[r.mensura] == '_'
                && r.mensura > optima.mensura)
            {
                optima = r;
            }
        }
        si (optima.mensura > ZEPHYRUM)
        {
            vacuum* p = NIHIL;

            si (tabula_dispersa_invenire(u->stemmata, optima, &p))
            {
                x->parens = (Unitas*)p;
            }
        }
    }
}


/* ==================================================
 * Lectio
 * ================================================== */

interior StmlNodus*
_parsare (
             chorda  textus,
             chorda  via,
            Piscina* piscina,
InternamentumChorda* intern,
     UnitatesCausae* k)
{
    StmlResultus r = stml_legere(textus, piscina, intern);

    si (!r.successus || r.elementum_radix == NIHIL)
    {
        _causa(k, via, r.linea_erroris, "STML illegibile", _vacua(),
            "");
        redde NIHIL;
    }
    redde r.elementum_radix;
}

Unitates*
unitates_legere (
     constans UnitatesSutura*  sutura,
                     Piscina*  piscina,
                      chorda*  causa_out,
                         Xar** recusationes_out)
{
               Unitates* u;
         UnitatesCausae  k;
    InternamentumChorda* intern;
                    Xar* viae;
                 chorda  via_politicae;
                 chorda  textus;
              StmlNodus* radix;
                    i32  i;

    k.textus        = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
    k.numerus       = ZEPHYRUM;
    k.recusationes  = xar_creare(piscina,
        (i32)magnitudo(UnitatesRecusatio));
    k.piscina  = piscina;
    intern     = internamentum_creare(piscina);

    u = (Unitates*)piscina_allocare(piscina, (memoriae_index)magnitudo(
        Unitates));
    u->sutura  = sutura;
    u->curae   = xar_creare(piscina, (i32)magnitudo(chorda));
    u->vitae   = xar_creare(piscina, (i32)magnitudo(chorda));
    u->usus    = xar_creare(piscina, (i32)magnitudo(chorda));
    u->genera          = xar_creare(piscina, (i32)magnitudo(
        UnitatesGenus));
    u->munera          = xar_creare(piscina, (i32)magnitudo(
        UnitatesMunus));
    u->munerum_tituli  = xar_creare(piscina, (i32)magnitudo(chorda));
    u->linguae         = xar_creare(piscina, (i32)magnitudo(
        UnitatesLingua));
    u->iudices         = xar_creare(piscina, (i32)magnitudo(
        UnitatesIudex));
    u->provenientiae   = _verba(chorda_ex_literis("manu generatum",
        piscina), piscina);
    u->capita       = _vacua();
    u->fontes       = _vacua();
    u->probationes  = _vacua();
    u->praefixum    = _vacua();
    u->partes       = xar_creare(piscina, (i32)magnitudo(chorda));
    u->familiae     = xar_creare(piscina, (i32)magnitudo(chorda));
    u->omnes        = xar_creare(piscina, (i32)magnitudo(Unitas*));
    u->directoria   = xar_creare(piscina, (i32)magnitudo(Unitas*));
    u->stemmata     = tabula_dispersa_creare_chorda(piscina, CCLVI);

    via_politicae = chorda_ex_literis(UNITATES_POLITICA, piscina);
    si (!sutura->plagulam_legere(sutura->datum, via_politicae, piscina,
            &textus))
    {
        _causa(&k, via_politicae, ZEPHYRUM, "politica legi nequit",
            _vacua(), "");
    }
    alioquin
    {
        radix = _parsare(textus, via_politicae, piscina, intern, &k);
        si (radix != NIHIL)
        {
            _politicam_legere(u, radix, via_politicae, &k, piscina);
        }
    }

    viae = sutura->plagulas_enumerare(sutura->datum, piscina);
    si (viae == NIHIL)
    {
        _causa(&k, _vacua(), ZEPHYRUM, "enumeratio plagularum fracta",
            _vacua(), "");
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
    }
    per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
    {
        chorda via = _via_norma(*(chorda*)xar_obtinere(viae, i));
        chorda dir;
        chorda basis;

        _viam_findere(via, &dir, &basis);
        si (!chorda_aequalis_literis(basis, UNITATES_DECLARATIO))
        {
            perge;
        }
        si (!sutura->plagulam_legere(sutura->datum, via, piscina,
                &textus))
        {
            _causa(&k, via, ZEPHYRUM, "declaratio legi nequit",
                _vacua(),
                "");
            perge;
        }
        radix = _parsare(textus, via, piscina, intern, &k);
        si (radix != NIHIL)
        {
            _declarationem_legere(u, radix, via, dir, &k, piscina);
        }
    }
    si (_directorium_continens(u, chorda_ex_literis("x", piscina),
        NIHIL)
        == NIHIL)
    {
        _causa(&k, chorda_ex_literis(UNITATES_DECLARATIO, piscina),
            ZEPHYRUM, "unitas radicis abest (unitas.stml in radice)",
            _vacua(), "");
    }
    alioquin
    {
        per (i = ZEPHYRUM; i < xar_numerus(u->directoria); i++)
        {
            Unitas* x = *(Unitas**)xar_obtinere(u->directoria, i);

            si (x->via.mensura > ZEPHYRUM)
            {
                x->parens = _directorium_continens(u, x->via, x);
            }
        }
        _bibliothecas_colligere(u, viae, &k, piscina);
    }

    si (k.numerus > ZEPHYRUM)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(k.textus);
        }
        si (recusationes_out != NIHIL)
        {
            *recusationes_out = k.recusationes;
        }
        redde NIHIL;
    }
    si (causa_out != NIHIL)
    {
        *causa_out = _vacua();
    }
    si (recusationes_out != NIHIL)
    {
        *recusationes_out = NIHIL;
    }
    redde u;
}

constans Xar*
unitates_omnes (
    constans Unitates* unitates)
{
    redde unitates->omnes;
}


/* ==================================================
 * Responsum
 * ================================================== */

interior chorda
_textus (
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

/* unitas plagulae: bibliotheca per stemma, aliter directorium */
interior constans Unitas*
_unitas_viae (
    constans Unitates* u,
               chorda  via,
              Piscina* piscina,
               chorda* causa)
{
    chorda  dir;
    chorda  basis;
    chorda  s         = _vacua();
       b32  probatio  = FALSUM;
    Unitas* x;

    _viam_findere(via, &dir, &basis);
    si (chorda_aequalis(dir, u->capita) && _finit(basis, ".h"))
    {
        s = chorda_sectio(basis, ZEPHYRUM, basis.mensura - II);
    }
    alioquin si (   chorda_aequalis(dir, u->fontes)
                 && _finit(basis, ".worklog.md"))
    {
        s = chorda_sectio(basis, ZEPHYRUM, basis.mensura - XI);
    }
    alioquin si (   chorda_aequalis(dir, u->fontes)
                 && (_finit(basis, ".c") || _finit(basis, ".m")))
    {
        s = chorda_sectio(basis, ZEPHYRUM, basis.mensura - II);
    }
    alioquin si (   chorda_aequalis(dir, u->probationes)
                 && _incipit_chorda(basis, u->praefixum)
                 && _finit(basis, ".c")
                 && basis.mensura > u->praefixum.mensura + II)
    {
        s = chorda_sectio(basis, u->praefixum.mensura,
            basis.mensura - II);
        probatio = VERUM;
    }
    si (s.mensura > ZEPHYRUM && !probatio)
    {
        vacuum* p = NIHIL;

        si (tabula_dispersa_invenire(u->stemmata, s, &p))
        {
            x = (Unitas*)p;
            *causa = chorda_aequalis(x->titulus, s)
                ? _textus(piscina, "stemma ", s, "", _vacua(), "")
                : _textus(piscina, "stemma ", s, " pars bibliothecae ",
                    x->titulus, "");
            redde x;
        }
    }
    si (probatio)
    {
        /* praefixum longissimum ad '_': bibliotheca prius, deinde
         * unitas directorii eiusdem tituli */
        chorda c = s;

        dum (c.mensura > ZEPHYRUM)
        {
            vacuum* p = NIHIL;
               i32  i;

            si (tabula_dispersa_invenire(u->stemmata, c, &p))
            {
                *causa = _textus(piscina, "probatio stemmatis ", c, "",
                    _vacua(), "");
                redde (Unitas*)p;
            }
            per (i = ZEPHYRUM; i < xar_numerus(u->directoria); i++)
            {
                x = *(Unitas**)xar_obtinere(u->directoria, i);
                si (chorda_aequalis(x->titulus, c))
                {
                    *causa = _textus(piscina, "probatio unitatis ", c,
                        " (", x->declaratio, ")");
                    redde x;
                }
            }
            dum (c.mensura > ZEPHYRUM && c.datum[c.mensura - I] != '_')
            {
                c.mensura--;
            }
            si (c.mensura > ZEPHYRUM)
            {
                c.mensura--;
            }
        }
    }
    x = _directorium_continens(u, via, NIHIL);
    *causa = _textus(piscina, "directorium ", x->via.mensura > ZEPHYRUM
        ? x->via : chorda_ex_literis(".", piscina), " (", x->declaratio,
        ")");
    redde x;
}

interior b32
_segmentum_habet (
    chorda via,
    chorda segmentum)
{
    i32 initium = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < via.mensura; i++)
    {
        si (via.datum[i] == '/')
        {
            si (chorda_aequalis(chorda_sectio(via, initium, i),
                    segmentum))
            {
                redde VERUM;
            }
            initium = i + I;
        }
    }
    redde FALSUM;
}

interior b32
_extensio_congruit (
     constans Xar* extensiones,
           chorda  basis,
           chorda* congruens)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(extensiones); i++)
    {
        chorda e = *(chorda*)xar_obtinere(extensiones, i);

        si (_finit_chorda(basis, e))
        {
            si (congruens != NIHIL)
            {
                *congruens = e;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* proprietates sine provenientia (illa pigre legitur) */
interior vacuum
_respondere (
             constans Unitates* u,
                        chorda  via,
                       Piscina* piscina,
             UnitatisResponsum* r)
{
    constans Unitas* x;
             chorda  dir;
             chorda  basis;
                i32  i;

    r->via     = via;
    r->unitas  = _unitas_viae(u, via, piscina, &r->unitatis_causa);
    _viam_findere(via, &dir, &basis);

    r->cura.valor = _vacua();
    per (x = r->unitas; x != NIHIL; x = x->parens)
    {
        si (x->cura.mensura > ZEPHYRUM)
        {
            r->cura.valor = x->cura;
            r->cura.causa = x->declaratio;
            frange;
        }
    }
    si (r->cura.valor.mensura == ZEPHYRUM)
    {
        constans UnitatesGenus* g = _genus_invenire(u,
            r->unitas->genus);

        r->cura.valor  = g != NIHIL ? g->cura : _vacua();
        r->cura.causa  = _textus(piscina, "genus ", r->unitas->genus,
            " (", chorda_ex_literis(UNITATES_POLITICA, piscina), ")");
    }

    r->vita.valor  = chorda_ex_literis("viva", piscina);
    r->vita.causa  = chorda_ex_literis("praestitutum (vita nulla "
        "declarata)", piscina);
    per (x = r->unitas; x != NIHIL; x = x->parens)
    {
        si (x->vita.mensura > ZEPHYRUM)
        {
            r->vita.valor = x->vita;
            r->vita.causa = x->declaratio;
            frange;
        }
    }

    r->munus.valor  = _vacua();
    r->munus.causa  = chorda_ex_literis("nulla regula munus congruit",
        piscina);
    per (i = ZEPHYRUM; i < xar_numerus(u->munera); i++)
    {
        UnitatesMunus* m = (UnitatesMunus*)xar_obtinere(u->munera, i);
                  b32  congruit = (m->segmentum.mensura == ZEPHYRUM
                      || _segmentum_habet(via, m->segmentum))
                      && (xar_numerus(m->extensiones) == ZEPHYRUM
                      || _extensio_congruit(m->extensiones, basis,
                      NIHIL));

        si (congruit)
        {
            ChordaAedificator* c = chorda_aedificator_creare(piscina,
                (memoriae_index)LXIV);

            chorda_aedificator_appendere_literis(c, "regula munus ");
            chorda_aedificator_appendere_i32(c, m->ordo);
            chorda_aedificator_appendere_literis(c, " (");
            si (m->segmentum.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(c, "segmentum ");
                chorda_aedificator_appendere_chorda(c, m->segmentum);
                si (xar_numerus(m->extensiones) > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(c, ", ");
                }
            }
            si (xar_numerus(m->extensiones) > ZEPHYRUM)
            {
                i32 j;

                per (j = ZEPHYRUM; j < xar_numerus(m->extensiones); j++)
                {
                    si (j > ZEPHYRUM)
                    {
                        chorda_aedificator_appendere_character(c, ' ');
                    }
                    chorda_aedificator_appendere_chorda(c,
                        *(chorda*)xar_obtinere(m->extensiones, j));
                }
            }
            chorda_aedificator_appendere_literis(c, ")");
            r->munus.valor = m->titulus;
            r->munus.causa = chorda_aedificator_finire(c);
            frange;
        }
    }

    r->lingua.valor = _vacua();
    r->lingua.causa = chorda_ex_literis("extensio ignota", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(u->linguae); i++)
    {
        UnitatesLingua* l = (UnitatesLingua*)xar_obtinere(u->linguae,
            i);
                chorda e;

        si (_extensio_congruit(l->extensiones, basis, &e))
        {
            r->lingua.valor  = l->titulus;
            r->lingua.causa  = _textus(piscina, "extensio ", e, "",
                _vacua(), "");
            frange;
        }
    }

    r->provenientia.valor = _vacua();
    r->provenientia.causa = chorda_ex_literis("non lecta", piscina);
}

interior vacuum
_provenientia_legere (
             constans Unitates* u,
                       Piscina* piscina,
             UnitatisResponsum* r)
{
    chorda textus;
    chorda prima;
       i32 i;

    si (!u->sutura->plagulam_legere(u->sutura->datum, r->via, piscina,
            &textus))
    {
        r->provenientia.valor  = _vacua();
        r->provenientia.causa  =
            chorda_ex_literis("plagula legi nequit",
            piscina);
        redde;
    }
    prima = textus;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '\n')
        {
            prima = chorda_sectio(textus, ZEPHYRUM, i);
            frange;
        }
    }
    si (chorda_continet(prima, chorda_ex_literis("GENERATUM", piscina)))
    {
        r->provenientia.valor  = chorda_ex_literis("generatum",
            piscina);
        r->provenientia.causa  = chorda_ex_literis("linea I: GENERATUM",
            piscina);
    }
    alioquin
    {
        r->provenientia.valor  = chorda_ex_literis("manu", piscina);
        r->provenientia.causa  = chorda_ex_literis("linea I sine "
            "GENERATUM", piscina);
    }
}

b32
unitates_quid (
             constans Unitates* unitates,
                        chorda  via,
                       Piscina* piscina,
             UnitatisResponsum* responsum)
{
    via = _via_norma(via);
    si (via.mensura == ZEPHYRUM || via.datum[ZEPHYRUM] == '/')
    {
        redde FALSUM;
    }
    _respondere(unitates, via, piscina, responsum);
    _provenientia_legere(unitates, piscina, responsum);
    redde VERUM;
}

/* proprietas in indice iudicis? aliter causa */
interior b32
_admittit (
                   Xar* index_iudicis,
    UnitatesProprietas  p,
    constans character* quid,
    constans character* indicis,
                chorda  iudex,
               Piscina* piscina,
                chorda* causa_out)
{
    ChordaAedificator* c;

    si (_in_indice(index_iudicis, p.valor))
    {
        redde VERUM;
    }
    si (causa_out != NIHIL)
    {
        c = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
        chorda_aedificator_appendere_literis(c, quid);
        chorda_aedificator_appendere_character(c, ' ');
        si (p.valor.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(c, p.valor);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(c, "nullum");
        }
        chorda_aedificator_appendere_literis(c, " (");
        chorda_aedificator_appendere_chorda(c, p.causa);
        chorda_aedificator_appendere_literis(c, ") non in ");
        chorda_aedificator_appendere_literis(c, indicis);
        chorda_aedificator_appendere_literis(c, " iudicis ");
        chorda_aedificator_appendere_chorda(c, iudex);
        *causa_out = chorda_aedificator_finire(c);
    }
    redde FALSUM;
}

UnitatesIudicium
unitates_iudicat (
    constans Unitates* unitates,
               chorda  iudex,
               chorda  via,
              Piscina* piscina,
               chorda* causa_out)
{
    constans UnitatesIudex* d = NIHIL;
         UnitatisResponsum  r;
                       i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(unitates->iudices); i++)
    {
        UnitatesIudex* x = (UnitatesIudex*)xar_obtinere(
            unitates->iudices, i);

        si (chorda_aequalis(x->titulus, iudex))
        {
            d = x;
            frange;
        }
    }
    si (d == NIHIL)
    {
        si (causa_out != NIHIL)
        {
            Xar* noti = xar_creare(piscina, (i32)magnitudo(chorda));

            per (i = ZEPHYRUM; i < xar_numerus(unitates->iudices); i++)
            {
                *(chorda*)xar_addere(noti) = ((UnitatesIudex*)
                    xar_obtinere(unitates->iudices, i))->titulus;
            }
            *causa_out = _textus(piscina, "iudex '", iudex,
                "' ignotus (noti: ", _noti(noti, piscina), ")");
        }
        redde UNITATES_IUDEX_IGNOTUS;
    }
    via = _via_norma(via);
    si (via.mensura == ZEPHYRUM || via.datum[ZEPHYRUM] == '/')
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_ex_literis("via vacua aut absoluta",
                piscina);
        }
        redde UNITATES_NON_IUDICAT;
    }
    _respondere(unitates, via, piscina, &r);
    si (   !_admittit(d->curae, r.cura, "cura", "curae", iudex, piscina,
               causa_out)
        || !_admittit(d->vitae, r.vita, "vita", "vitae", iudex, piscina,
               causa_out)
        || !_admittit(d->munera, r.munus, "munus", "munera", iudex,
               piscina, causa_out)
        || !_admittit(d->linguae, r.lingua, "lingua", "linguae", iudex,
               piscina, causa_out))
    {
        redde UNITATES_NON_IUDICAT;
    }
    _provenientia_legere(unitates, piscina, &r);
    si (!_admittit(d->provenientiae, r.provenientia, "provenientia",
            "provenientiae", iudex, piscina, causa_out))
    {
        redde UNITATES_NON_IUDICAT;
    }
    si (causa_out != NIHIL)
    {
        *causa_out = _vacua();
    }
    redde UNITATES_IUDICAT;
}
