/*
 * mensura_quaestio.c - quaestiones nominatae super cursus (vide
 * include/mensura_quaestio.h). Purae: Xar cursuum intrat, Xar ordinum
 * exit; disco et ambitu non utuntur.
 */

#include "latina.h"
#include "mensura_quaestio.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

structura CursusIndex {
      constans Xar* cursus;
    TabulaDispersa* per_id;      /* id -> Cursus* */
    TabulaDispersa* filii;       /* parens -> Xar de (Cursus*) */
           Piscina* piscina;
};

CursusIndex*
cursus_indicem_struere (
    constans Xar* cursus,
         Piscina* piscina)
{
    CursusIndex* index;
            i32  k;

    si (cursus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    index = (CursusIndex*)piscina_allocare(piscina,
        magnitudo(CursusIndex));
    si (index == NIHIL)
    {
        redde NIHIL;
    }
    index->cursus   = cursus;
    index->piscina  = piscina;
    index->per_id = tabula_dispersa_creare_chorda(piscina,
        (i32)CCLVI);
    index->filii = tabula_dispersa_creare_chorda(piscina,
        (i32)CCLVI);
    si (index->per_id == NIHIL || index->filii == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(cursus); k++)
    {
        Cursus* c      = (Cursus*)xar_obtinere(cursus, k);
        vacuum* locus  = NIHIL;
           Xar* fratres;

        (vacuum)tabula_dispersa_inserere(index->per_id, c->initium.id,
            c);
        si (c->initium.parens.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!tabula_dispersa_invenire(index->filii, c->initium.parens,
                &locus))
        {
            fratres = xar_creare(piscina, (i32)magnitudo(Cursus*));
            si (fratres == NIHIL)
            {
                redde NIHIL;
            }
            (vacuum)tabula_dispersa_inserere(index->filii,
                c->initium.parens, fratres);
        }
        alioquin
        {
            fratres = (Xar*)locus;
        }
        *(Cursus**)xar_addere(fratres) = c;
    }
    redde index;
}

constans Cursus*
cursus_invenire (
    constans CursusIndex* index,
                  chorda  id)
{
    vacuum* locus = NIHIL;

    si (   index == NIHIL
        || !tabula_dispersa_invenire(index->per_id, id, &locus))
    {
        redde NIHIL;
    }
    redde (constans Cursus*)locus;
}


/* ==================================================
 * FILTRUM ET ORDO
 * ================================================== */

interior i32
_mascula (
    CursusStatus status)
{
    commutatio (status)
    {
        casus CURSUS_TRANSIIT:  redde CURSUS_MASCULA_TRANSIIT;
        casus CURSUS_FRACTUS:   redde CURSUS_MASCULA_FRACTUS;
        ordinarius:             redde CURSUS_MASCULA_INTERRUPTUS;
    }
}

interior b32
_congruit (
           constans Cursus* c,
    constans CursusFiltrum* f)
{
    si (f == NIHIL)
    {
        redde VERUM;
    }
    si (   f->titulus.mensura > ZEPHYRUM
        && !chorda_aequalis(c->initium.titulus, f->titulus))
    {
        redde FALSUM;
    }
    si (   f->praefixum.mensura > ZEPHYRUM
        && !chorda_incipit(c->initium.titulus, f->praefixum))
    {
        redde FALSUM;
    }
    si (   f->arbor.mensura > ZEPHYRUM
        && !chorda_aequalis(c->initium.arbor, f->arbor))
    {
        redde FALSUM;
    }
    si (   f->ramus.mensura > ZEPHYRUM
        && !chorda_aequalis(c->initium.ramus, f->ramus))
    {
        redde FALSUM;
    }
    si (   f->nota.mensura > ZEPHYRUM
        && !chorda_incipit(c->finis.nota, f->nota))
    {
        redde FALSUM;
    }
    si (f->status != ZEPHYRUM && (f->status & _mascula(c->status)) == 0)
    {
        redde FALSUM;
    }
    si (f->post_ms > (s64)ZEPHYRUM && c->initium.initium < f->post_ms)
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* ordo: initium, deinde sedes in tabula (par stabile) */
nomen structura {
    constans Cursus* cursus;
                s64  initium;
                i32  sedes;
} OrdinandumCursus;

interior integer
_recens_primum (
    constans vacuum* a,
    constans vacuum* b)
{
    constans OrdinandumCursus* x = (constans OrdinandumCursus*)a;
    constans OrdinandumCursus* y = (constans OrdinandumCursus*)b;

    si (x->initium != y->initium)
    {
        redde (x->initium < y->initium) ? I : -I;
    }
    redde (x->sedes < y->sedes) ? I : (x->sedes > y->sedes) ? -I : 0;
}

interior integer
_antiquissimum_primum (
    constans vacuum* a,
    constans vacuum* b)
{
    redde -_recens_primum(a, b);
}

/* cursus filtrati ordinati in tabula temporaria; numerus_out */
interior OrdinandumCursus*
_colligere (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                        b32  recens_primum,
                    Piscina* piscina,
                        i32* numerus_out)
{
    OrdinandumCursus* tabula;
                 i32  n = ZEPHYRUM;
                 i32  k;

    *numerus_out = ZEPHYRUM;
    tabula = (OrdinandumCursus*)piscina_allocare(piscina,
        (memoriae_index)(
        xar_numerus(index->cursus) + I) * magnitudo(OrdinandumCursus));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* c =
            (constans Cursus*)xar_obtinere(index->cursus,
            k);

        si (_congruit(c, filtrum))
        {
            tabula[n].cursus   = c;
            tabula[n].initium  = c->initium.initium;
            tabula[n].sedes    = k;
            n++;
        }
    }
    qsort(tabula, (size_t)n, magnitudo(OrdinandumCursus),
        recens_primum
        ? _recens_primum : _antiquissimum_primum);
    *numerus_out = n;
    redde tabula;
}


/* ==================================================
 * QUAESTIONES
 * ================================================== */

Xar*
cursus_quaerere_historia (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
    OrdinandumCursus* tabula;
                 Xar* ordines;
                 i32  n;
                 i32  k;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    tabula   = _colligere(index, filtrum, VERUM, piscina, &n);
    ordines  = xar_creare(piscina, (i32)magnitudo(Cursus*));
    si (tabula == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (   filtrum != NIHIL && filtrum->numerus > ZEPHYRUM
            && k       >= filtrum->numerus)
        {
            frange;
        }
        *(constans Cursus**)xar_addere(ordines) = tabula[k].cursus;
    }
    redde ordines;
}

interior integer
_s64_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x = *(constans s64*)a;
    s64 y = *(constans s64*)b;

    redde (x < y) ? -I : (x > y) ? I : 0;
}

/* CLAVIS COMMISSI (vide caput): maior 'commissio' transiens cum nota
 * 'commissum H' -> H; aliter sordidus -> commissum + '+'; aliter
 * commissum. Ascensus ad LXIV gradus (circulus fractus non pendet). */
interior chorda
_clavis_commissi (
    constans CursusIndex* index,
         constans Cursus* c,
                 Piscina* piscina,
                     b32* per_commissionem)
{
     constans character* praefixum  = "commissum ";
        constans Cursus* maior      = c;
                    i32  gradus;

    *per_commissionem = FALSUM;
    per (gradus = ZEPHYRUM; gradus < LXIV; gradus++)
    {
        maior = cursus_invenire(index, maior->initium.parens);
        si (maior == NIHIL)
        {
            frange;
        }
        si (chorda_aequalis_literis(maior->initium.titulus,
            "commissio"))
        {
            si (   maior->status == CURSUS_TRANSIIT
                && chorda_incipit(maior->finis.nota, chorda_ex_literis(
                       praefixum, piscina)))
            {
                *per_commissionem = VERUM;
                redde chorda_sectio(maior->finis.nota, (i32)strlen(
                    praefixum), maior->finis.nota.mensura);
            }
            frange;   /* tentamen fractum: mutatio sordida manet */
        }
    }
    si (c->initium.sordidum)
    {
        redde chorda_concatenare(c->initium.commissum,
            chorda_ex_literis("+", piscina), piscina);
    }
    redde c->initium.commissum;
}

Xar*
cursus_quaerere_tendentia (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
          CursusFiltrum  f;
       OrdinandumCursus* tabula;
                 chorda* claves;
                    Xar* ordines;
         TabulaDispersa* per_commissum;
                    i32  n;
                    i32  k;
                    i32  g;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    /* transeuntes SOLI: duratio fracti non est tempus operis */
    memset(&f, ZEPHYRUM, magnitudo(f));
    si (filtrum != NIHIL)
    {
        f = *filtrum;
    }
    f.status   = CURSUS_MASCULA_TRANSIIT;
    f.numerus  = ZEPHYRUM;
    tabula     = _colligere(index, &f, FALSUM, piscina, &n);
    claves = (chorda*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(chorda));
    ordines = xar_creare(piscina, (i32)magnitudo(CursusTendentia));
    per_commissum = tabula_dispersa_creare_chorda(piscina, (i32)LXIV);
    si (   tabula        == NIHIL || claves == NIHIL || ordines == NIHIL
        || per_commissum == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        b32 per_commissionem = FALSUM;

        claves[k] = _clavis_commissi(index, tabula[k].cursus, piscina,
            &per_commissionem);
        si (!tabula_dispersa_continet(per_commissum, claves[k]))
        {
            CursusTendentia* t = (CursusTendentia*)xar_addere(ordines);

            memset(t, ZEPHYRUM, magnitudo(CursusTendentia));
            t->commissum         = claves[k];
            t->per_commissionem  = per_commissionem;
            t->primum            = tabula[k].cursus->initium.initium;
            (vacuum)tabula_dispersa_inserere(per_commissum, claves[k],
                (vacuum*)(memoriae_index)xar_numerus(ordines));
        }
    }
    /* per commissum: durationes ordinatae -> mediana inferior */
    per (g = ZEPHYRUM; g < xar_numerus(ordines); g++)
    {
        CursusTendentia* t = (CursusTendentia*)xar_obtinere(ordines, g);
                    s64* durationes = (s64*)piscina_allocare(piscina,
                        (memoriae_index)(n + I) * magnitudo(s64));
                    i32 m = ZEPHYRUM;

        si (durationes == NIHIL)
        {
            redde NIHIL;
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (chorda_aequalis(claves[k], t->commissum))
            {
                durationes[m++] = tabula[k].cursus->finis.duratio_ms;
            }
        }
        qsort(durationes, (size_t)m, magnitudo(s64), _s64_comparare);
        t->numerus     = m;
        t->minima_ms   = durationes[ZEPHYRUM];
        t->maxima_ms   = durationes[m - I];
        t->mediana_ms  = durationes[(m - I) / II];
    }
    redde ordines;
}

/* filii cursus, ordine durationis descendentis */
interior integer
_duratio_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Cursus* x = *(constans Cursus* constans*)a;
    constans Cursus* y = *(constans Cursus* constans*)b;

    si (x->finis.duratio_ms != y->finis.duratio_ms)
    {
        redde (x->finis.duratio_ms < y->finis.duratio_ms) ? I : -I;
    }
    redde (x->initium.initium < y->initium.initium) ? -I
        : (x->initium.initium > y->initium.initium) ? I : 0;
}

interior vacuum
_arborem_addere (
    constans CursusIndex* index,
         constans Cursus* c,
                     i32  profunditas,
                     i32  pars,
                     i32  maxima,
                     Xar* ordines)
{
     CursusNodus*  nodus = (CursusNodus*)xar_addere(ordines);
          vacuum*  locus = NIHIL;
             Xar*  fratres;
 constans Cursus** ordinati;
             i32   k;
             i32   n;

    si (nodus == NIHIL)
    {
        redde;
    }
    nodus->cursus       = c;
    nodus->profunditas  = profunditas;
    nodus->pars         = pars;
    si (   (maxima > ZEPHYRUM && profunditas >= maxima)
        || !tabula_dispersa_invenire(index->filii, c->initium.id,
        &locus))
    {
        redde;
    }
    fratres  = (Xar*)locus;
    n        = (i32)xar_numerus(fratres);
    ordinati = (constans Cursus**)piscina_allocare(index->piscina,
        (memoriae_index)(n + I) * magnitudo(Cursus*));
    si (ordinati == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        ordinati[k] = *(constans Cursus**)xar_obtinere(fratres, k);
    }
    qsort((vacuum*)ordinati, (size_t)n, magnitudo(Cursus*),
        _duratio_descendens);
    per (k = ZEPHYRUM; k < n; k++)
    {
        i32 pars_filii = (c->finis.duratio_ms > (s64)ZEPHYRUM)
            ? (i32)(ordinati[k]->finis.duratio_ms * (s64)M
                / c->finis.duratio_ms) : ZEPHYRUM;

        _arborem_addere(index, ordinati[k], profunditas + I, pars_filii,
            maxima, ordines);
    }
}

Xar*
cursus_quaerere_filii (
    constans CursusIndex* index,
                  chorda  id,
                     i32  profunditas_maxima,
                 Piscina* piscina)
{
    constans Cursus* radix = cursus_invenire(index, id);
                Xar* ordines;

    si (radix == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    ordines = xar_creare(piscina, (i32)magnitudo(CursusNodus));
    si (ordines == NIHIL)
    {
        redde NIHIL;
    }
    _arborem_addere(index, radix, ZEPHYRUM, (i32)M, profunditas_maxima,
        ordines);
    redde ordines;
}

interior integer
_summa_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    constans CursusSumma* x = (constans CursusSumma*)a;
    constans CursusSumma* y = (constans CursusSumma*)b;
                     s64  pondus_primi = x->octeti
                         > (s64)ZEPHYRUM ? x->octeti
                         : x->ms_summa;
                     s64 pondus_alteri = y->octeti
                         > (s64)ZEPHYRUM ? y->octeti
                         : y->ms_summa;

    redde (pondus_primi < pondus_alteri) ? I : (pondus_primi
        > pondus_alteri) ? -I : 0;
}

/* Xar summarum in tabulam contiguam, ordinatam, Xar novum */
interior Xar*
_summas_ordinare (
         Xar* summae,
     Piscina* piscina)
{
    CursusSumma* tabula;
            Xar* ordines;
            i32  n = (i32)xar_numerus(summae);
            i32  k;

    tabula = (CursusSumma*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(CursusSumma));
    ordines = xar_creare(piscina, (i32)magnitudo(CursusSumma));
    si (tabula == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        tabula[k] = *(CursusSumma*)xar_obtinere(summae, k);
    }
    qsort(tabula, (size_t)n, magnitudo(CursusSumma), _summa_descendens);
    per (k = ZEPHYRUM; k < n; k++)
    {
        *(CursusSumma*)xar_addere(ordines) = tabula[k];
    }
    redde ordines;
}

/* segmentum ultimum tituli: post '/', '#' aut ':' ultimum */
interior chorda
_segmentum_ultimum (
    chorda titulus)
{
    s32 i;

    per (i = (s32)titulus.mensura - I; i >= ZEPHYRUM; i--)
    {
        character c = (character)titulus.datum[i];

        si (c == '/' || c == '#' || c == ':')
        {
            redde chorda_sectio(titulus, (i32)(i + I), titulus.mensura);
        }
    }
    redde titulus;
}

interior b32
_titulus_similis (
    chorda titulus,
    chorda quaesitum)
{
    chorda segmentum = _segmentum_ultimum(quaesitum);

    redde chorda_continet(titulus, quaesitum)
           || (   segmentum.mensura > ZEPHYRUM
               && chorda_aequalis(_segmentum_ultimum(titulus),
               segmentum));
}

/* numerus descendens, deinde titulus (ordo stabilis) */
interior integer
_similis_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    constans CursusSumma* x = (constans CursusSumma*)a;
    constans CursusSumma* y = (constans CursusSumma*)b;

    si (x->numerus != y->numerus)
    {
        redde (x->numerus < y->numerus) ? I : -I;
    }
    redde (integer)chorda_comparare(x->titulus, y->titulus);
}

Xar*
cursus_quaerere_similes (
      constans CursusIndex* index,
    constans CursusFiltrum* filtrum,
                       i32  maximi,
                       i32* noti_out,
                   Piscina* piscina)
{
      CursusFiltrum  solum_titulus;
             chorda  quaesitum;
                Xar* similes;
                Xar* ordines;
     TabulaDispersa* per_titulum;
        CursusSumma* tabula;
                i32  k;
                i32  n;

    si (noti_out != NIHIL)
    {
        *noti_out = ZEPHYRUM;
    }
    si (index == NIHIL || filtrum == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    quaesitum = filtrum->titulus.mensura > ZEPHYRUM ? filtrum->titulus
        : filtrum->praefixum;
    si (quaesitum.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    memset(&solum_titulus, ZEPHYRUM, magnitudo(CursusFiltrum));
    solum_titulus.titulus = filtrum->titulus;
    solum_titulus.praefixum = filtrum->praefixum;
    similes = xar_creare(piscina, (i32)magnitudo(CursusSumma));
    ordines = xar_creare(piscina, (i32)magnitudo(CursusSumma));
    per_titulum = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    si (similes == NIHIL || ordines == NIHIL || per_titulum == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* c =
            (constans Cursus*)xar_obtinere(index->cursus, k);
                  vacuum* locus = NIHIL;
             CursusSumma* s;

        si (_congruit(c, &solum_titulus))
        {
            si (noti_out != NIHIL)
            {
                (*noti_out)++;
            }
            perge;
        }
        si (!_titulus_similis(c->initium.titulus, quaesitum))
        {
            perge;
        }
        si (tabula_dispersa_invenire(per_titulum, c->initium.titulus,
                &locus))
        {
            s = (CursusSumma*)xar_obtinere(similes,
                (i32)(memoriae_index)locus - I);
        }
        alioquin
        {
            s = (CursusSumma*)xar_addere(similes);
            si (s == NIHIL)
            {
                redde NIHIL;
            }
            memset(s, ZEPHYRUM, magnitudo(CursusSumma));
            s->titulus = c->initium.titulus;
            (vacuum)tabula_dispersa_inserere(per_titulum,
                c->initium.titulus,
                (vacuum*)(memoriae_index)xar_numerus(similes));
        }
        s->numerus++;
    }
    si (noti_out != NIHIL && *noti_out > ZEPHYRUM)
    {
        redde ordines;     /* titulus notus: filtra cetera exclusisse */
    }
    n = (i32)xar_numerus(similes);
    si (n == ZEPHYRUM)
    {
        redde ordines;
    }
    tabula = (CursusSumma*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(CursusSumma));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        tabula[k] = *(CursusSumma*)xar_obtinere(similes, k);
    }
    qsort(tabula, (size_t)n, magnitudo(CursusSumma),
        _similis_descendens);
    per (k = ZEPHYRUM; k < n && (maximi <= ZEPHYRUM || k < maximi); k++)
    {
        *(CursusSumma*)xar_addere(ordines) = tabula[k];
    }
    redde ordines;
}

Xar*
cursus_quaerere_summa (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
               Xar* summae;
    TabulaDispersa* per_titulum;
    TabulaDispersa* parentes;
               i32  k;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    summae       = xar_creare(piscina, (i32)magnitudo(CursusSumma));
    per_titulum  = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    parentes     = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    si (summae == NIHIL || per_titulum == NIHIL || parentes == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* c =
            (constans Cursus*)xar_obtinere(index->cursus,
            k);
         constans Cursus* pater;
                  vacuum* locus = NIHIL;
             CursusSumma* s;
                  chorda  clavis_patris;

        si (!_congruit(c, filtrum))
        {
            perge;
        }
        si (tabula_dispersa_invenire(per_titulum, c->initium.titulus,
                &locus))
        {
            s = (CursusSumma*)xar_obtinere(summae,
                (i32)(memoriae_index)locus - I);
        }
        alioquin
        {
            s = (CursusSumma*)xar_addere(summae);
            memset(s, ZEPHYRUM, magnitudo(CursusSumma));
            s->titulus = c->initium.titulus;
            (vacuum)tabula_dispersa_inserere(per_titulum,
                c->initium.titulus,
                (vacuum*)(memoriae_index)xar_numerus(summae));
        }
        s->numerus++;
        s->ms_summa += c->finis.duratio_ms;
        /* parentes distincti per titulum: clavis = titulus + '\x1f' +
         * titulus patris */
        pater = cursus_invenire(index, c->initium.parens);
        clavis_patris = chorda_concatenare(c->initium.titulus,
            chorda_concatenare(chorda_ex_literis("\x1f", piscina),
            pater != NIHIL ? pater->initium.titulus : c->initium.parens,
            piscina), piscina);
        si (!tabula_dispersa_continet(parentes, clavis_patris))
        {
            (vacuum)tabula_dispersa_inserere(parentes, clavis_patris,
                (vacuum*)s);
            s->parentes++;
        }
    }
    redde _summas_ordinare(summae, piscina);
}

/* descendens FRACTUS profundissimus (par: recentior) */
interior vacuum
_culpam_quaerere (
    constans CursusIndex*  index,
         constans Cursus*  c,
                     i32   profunditas,
         constans Cursus** culpa,
                     i32*  culpa_profunditas)
{
    vacuum* locus = NIHIL;
       Xar* fratres;
       i32  k;

    si (!tabula_dispersa_invenire(index->filii, c->initium.id, &locus))
    {
        redde;
    }
    fratres = (Xar*)locus;
    per (k = ZEPHYRUM; k < xar_numerus(fratres); k++)
    {
        constans Cursus* f = *(constans Cursus**)xar_obtinere(fratres,
            k);

        si (f->status == CURSUS_TRANSIIT)
        {
            perge;
        }
        si (   profunditas + I > *culpa_profunditas
            || (   profunditas + I == *culpa_profunditas
                && *culpa != NIHIL
                && f->initium.initium > (*culpa)->initium.initium))
        {
            *culpa              = f;
            *culpa_profunditas  = profunditas + I;
        }
        _culpam_quaerere(index, f, profunditas + I, culpa,
            culpa_profunditas);
    }
}

Xar*
cursus_quaerere_commissiones (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
              CursusFiltrum  f;
           OrdinandumCursus* tabula;
                        Xar* ordines;
         constans character* praefixum = "commissum ";
                        i32  n;
                        i32  k;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    memset(&f, ZEPHYRUM, magnitudo(f));
    si (filtrum != NIHIL)
    {
        f = *filtrum;
    }
    si (f.titulus.mensura == ZEPHYRUM)
    {
        f.titulus = chorda_ex_literis("commissio", piscina);
    }
    tabula   = _colligere(index, &f, VERUM, piscina, &n);
    ordines  = xar_creare(piscina, (i32)magnitudo(CursusCommissio));
    si (tabula == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        CursusCommissio* t;
                    i32  profunditas = ZEPHYRUM;

        si (f.numerus > ZEPHYRUM && k >= f.numerus)
        {
            frange;
        }
        t = (CursusCommissio*)xar_addere(ordines);
        memset(t, ZEPHYRUM, magnitudo(CursusCommissio));
        t->commissio = tabula[k].cursus;
        si (t->commissio->status != CURSUS_TRANSIIT)
        {
            _culpam_quaerere(index, t->commissio, ZEPHYRUM, &t->culpa,
                &profunditas);
        }
        si (chorda_incipit(t->commissio->finis.nota, chorda_ex_literis(
                praefixum, piscina)))
        {
            t->commissum = chorda_sectio(t->commissio->finis.nota,
                (i32)strlen(praefixum),
                t->commissio->finis.nota.mensura);
        }
        alioquin
        {
            t->commissum = chorda_ex_literis("", piscina);
        }
    }
    redde ordines;
}

/* argv primum verbum (usque ad spatium) */
interior chorda
_argv_primum (
    chorda argv)
{
    i32 k;

    per (k = ZEPHYRUM; k < argv.mensura; k++)
    {
        si (argv.datum[k] == ' ')
        {
            redde chorda_sectio(argv, ZEPHYRUM, k);
        }
    }
    redde argv;
}

Xar*
cursus_quaerere_numquam (
    constans CursusIndex* index,
            constans Xar* universum,
                 Piscina* piscina)
{
               Xar* ordines;
    TabulaDispersa* visi;
               i32  k;
               i32  j;

    si (index == NIHIL || universum == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    ordines  = xar_creare(piscina, (i32)magnitudo(chorda));
    visi     = tabula_dispersa_creare_chorda(piscina, (i32)CCLVI);
    si (ordines == NIHIL || visi == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* c =
            (constans Cursus*)xar_obtinere(index->cursus,
            k);

        /* valor = signum praesentiae solum (non NIHIL) */
        (vacuum)tabula_dispersa_inserere(visi, c->initium.titulus,
            (vacuum*)ordines);
    }
    per (j = ZEPHYRUM; j < xar_numerus(universum); j++)
    {
        chorda u = *(constans chorda*)xar_obtinere(universum, j);
        chorda cauda = chorda_concatenare(chorda_ex_literis("/",
            piscina),
            u, piscina);
           b32 visum = tabula_dispersa_continet(visi, u);

        per (k = ZEPHYRUM; !visum
            && k < xar_numerus(index->cursus); k++)
        {
            constans Cursus* c = (constans Cursus*)xar_obtinere(
                index->cursus, k);
                      chorda primum = _argv_primum(c->initium.argv);

            visum = chorda_aequalis(primum, u)
                || chorda_terminatur(primum, cauda);
        }
        si (!visum)
        {
            *(chorda*)xar_addere(ordines) = u;
        }
    }
    redde ordines;
}

Xar*
cursus_quaerere_discus (
    constans Xar* introitus,
         Piscina* piscina)
{
               Xar* summae;
    TabulaDispersa* per_titulum;
               i32  k;

    si (introitus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    summae       = xar_creare(piscina, (i32)magnitudo(CursusSumma));
    per_titulum  = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    si (summae == NIHIL || per_titulum == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(introitus); k++)
    {
        constans DiurnaIntroitus* i = (constans DiurnaIntroitus*)
            xar_obtinere(introitus, k);
                          vacuum* locus = NIHIL;
                     CursusSumma* s;

        si (i->forma == DIURNA_DELETUM)
        {
            perge;
        }
        si (tabula_dispersa_invenire(per_titulum, i->titulus, &locus))
        {
            s = (CursusSumma*)xar_obtinere(summae,
                (i32)(memoriae_index)locus - I);
        }
        alioquin
        {
            s = (CursusSumma*)xar_addere(summae);
            memset(s, ZEPHYRUM, magnitudo(CursusSumma));
            s->titulus = i->titulus;
            (vacuum)tabula_dispersa_inserere(per_titulum, i->titulus,
                (vacuum*)(memoriae_index)xar_numerus(summae));
        }
        s->numerus++;
        s->octeti += i->mensura;
    }
    redde _summas_ordinare(summae, piscina);
}


/* ==================================================
 * STATISTICA ET LENTESCENTES (Franus 10-14)
 * ================================================== */

/* cursus filtrati per titulum, ordine temporis (antiquissimum primum):
 * Xar de Xar de (constans Cursus*) */
interior Xar*
_per_titulum (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
          CursusFiltrum  f;
       OrdinandumCursus* tabula;
                    Xar* greges;
         TabulaDispersa* per_titulum;
                    i32  n;
                    i32  k;

    memset(&f, ZEPHYRUM, magnitudo(f));
    si (filtrum != NIHIL)
    {
        f = *filtrum;
    }
    f.numerus    = ZEPHYRUM;
    tabula       = _colligere(index, &f, FALSUM, piscina, &n);
    greges       = xar_creare(piscina, (i32)magnitudo(Xar*));
    per_titulum  = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    si (tabula == NIHIL || greges == NIHIL || per_titulum == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        vacuum* locus = NIHIL;
           Xar* grex;

        si (tabula_dispersa_invenire(per_titulum,
                tabula[k].cursus->initium.titulus, &locus))
        {
            grex = (Xar*)locus;
        }
        alioquin
        {
            grex = xar_creare(piscina, (i32)magnitudo(Cursus*));
            si (grex == NIHIL)
            {
                redde NIHIL;
            }
            *(Xar**)xar_addere(greges) = grex;
            (vacuum)tabula_dispersa_inserere(per_titulum,
                tabula[k].cursus->initium.titulus, grex);
        }
        *(constans Cursus**)xar_addere(grex) = tabula[k].cursus;
    }
    redde greges;
}

/* mediana inferior durationum transeuntium grex[ab, ad) */
interior s64
_mediana (
    constans Xar* grex,
             i32  ab,
             i32  ad,
         Piscina* piscina,
             s64* maxima_out,
             i32* numerus_out)
{
    s64* durationes = (s64*)piscina_allocare(piscina, (memoriae_index)(
        ad - ab + I) * magnitudo(s64));
    i32 m = ZEPHYRUM;
    i32 k;

    si (durationes == NIHIL)
    {
        redde (s64)ZEPHYRUM;
    }
    per (k = ab; k < ad; k++)
    {
        constans Cursus* c = *(constans Cursus**)xar_obtinere(grex, k);

        si (c->status == CURSUS_TRANSIIT)
        {
            durationes[m++] = c->finis.duratio_ms;
        }
    }
    si (numerus_out != NIHIL)
    {
        *numerus_out = m;
    }
    si (m == ZEPHYRUM)
    {
        redde (s64)ZEPHYRUM;
    }
    qsort(durationes, (size_t)m, magnitudo(s64), _s64_comparare);
    si (maxima_out != NIHIL)
    {
        *maxima_out = durationes[m - I];
    }
    redde durationes[(m - I) / II];
}

interior integer
_statistica_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    constans CursusStatistica* x = (constans CursusStatistica*)a;
    constans CursusStatistica* y = (constans CursusStatistica*)b;

    redde (x->mediana_ms < y->mediana_ms) ? I
        : (x->mediana_ms > y->mediana_ms) ? -I : 0;
}

Xar*
cursus_quaerere_statistica (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
                 Xar* greges;
                 Xar* ordines;
    CursusStatistica* tabula;
                 i32  g;
                 i32  k;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    greges   = _per_titulum(index, filtrum, piscina);
    ordines  = xar_creare(piscina, (i32)magnitudo(CursusStatistica));
    si (greges == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    tabula = (CursusStatistica*)piscina_allocare(piscina,
        (memoriae_index)(
        xar_numerus(greges) + I) * magnitudo(CursusStatistica));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (g = ZEPHYRUM; g < xar_numerus(greges); g++)
    {
            constans Xar* grex  = *(Xar**)xar_obtinere(greges, g);
        CursusStatistica* s     = &tabula[g];

        memset(s, ZEPHYRUM, magnitudo(CursusStatistica));
        s->titulus = (*(constans Cursus**)xar_obtinere(grex, ZEPHYRUM))
            ->initium.titulus;
        s->numerus = (i32)xar_numerus(grex);
        per (k = ZEPHYRUM; k < xar_numerus(grex); k++)
        {
            si ((*(constans Cursus**)xar_obtinere(grex, k))->status
                != CURSUS_TRANSIIT)
            {
                s->fracti++;
            }
        }
        s->mediana_ms = _mediana(grex, ZEPHYRUM, (i32)xar_numerus(grex),
            piscina, &s->maxima_ms, NIHIL);
    }
    qsort(tabula, (size_t)xar_numerus(greges),
        magnitudo(CursusStatistica),
        _statistica_descendens);
    per (g = ZEPHYRUM; g < xar_numerus(greges); g++)
    {
        *(CursusStatistica*)xar_addere(ordines) = tabula[g];
    }
    redde ordines;
}

interior integer
_ratio_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    constans CursusLentescens* x = (constans CursusLentescens*)a;
    constans CursusLentescens* y = (constans CursusLentescens*)b;

    redde (x->ratio_permille < y->ratio_permille) ? I
        : (x->ratio_permille > y->ratio_permille) ? -I : 0;
}

Xar*
cursus_quaerere_lentescentes (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                        i32  fenestra,
                        i32  limes_permille,
                    Piscina* piscina)
{
     CursusFiltrum  f;
               Xar* greges;
               Xar* ordines;
  CursusLentescens* tabula;
               i32  n = ZEPHYRUM;
               i32  g;
               i32  k;

    si (index == NIHIL || piscina == NIHIL || fenestra <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    /* transeuntes soli: tempus fracti non est tempus operis */
    memset(&f, ZEPHYRUM, magnitudo(f));
    si (filtrum != NIHIL)
    {
        f = *filtrum;
    }
    f.status  = CURSUS_MASCULA_TRANSIIT;
    greges    = _per_titulum(index, &f, piscina);
    ordines   = xar_creare(piscina, (i32)magnitudo(CursusLentescens));
    si (greges == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    tabula = (CursusLentescens*)piscina_allocare(piscina,
        (memoriae_index)(
        xar_numerus(greges) + I) * magnitudo(CursusLentescens));
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (g = ZEPHYRUM; g < xar_numerus(greges); g++)
    {
        constans Xar* grex   = *(Xar**)xar_obtinere(greges, g);
                 i32  totus  = (i32)xar_numerus(grex);
                 i32  limes  = totus - fenestra;
                 s64  prior;
                 s64  recens;
                 i32  supra = ZEPHYRUM;

        /* priores saltem tot quot fenestra: historia brevis nihil
         * dicit */
        si (totus < fenestra * II)
        {
            perge;
        }
        prior = _mediana(grex, ZEPHYRUM, limes, piscina, NIHIL, NIHIL);
        recens = _mediana(grex, limes, totus, piscina, NIHIL, NIHIL);
        si (prior <= (s64)ZEPHYRUM)
        {
            perge;
        }
        per (k = limes; k < totus; k++)
        {
            si ((*(constans Cursus**)xar_obtinere(grex,
                k))->finis.duratio_ms
                > prior)
            {
                supra++;
            }
        }
        si (   recens * (s64)M < prior * (s64)limes_permille
            || supra < fenestra - I)
        {
            perge;
        }
        tabula[n].titulus = (*(constans Cursus**)xar_obtinere(grex,
            ZEPHYRUM))->initium.titulus;
        tabula[n].priores            = limes;
        tabula[n].mediana_prior_ms   = prior;
        tabula[n].mediana_recens_ms  = recens;
        tabula[n].supra              = supra;
        tabula[n].ratio_permille     = (i32)(recens * (s64)M / prior);
        n++;
    }
    qsort(tabula, (size_t)n, magnitudo(CursusLentescens),
        _ratio_descendens);
    per (k = ZEPHYRUM; k < n; k++)
    {
        *(CursusLentescens*)xar_addere(ordines) = tabula[k];
    }
    redde ordines;
}


/* ==================================================
 * CURRENTES (colloquia T8d)
 * ================================================== */

/* facta cursus ex mensuris (ordine actorum): custos et debiti
 * ultimi; ambitus ultimus, iudicata solum POST eum (fabrica in porta
 * eadem bis vocata - iudices, deinde ceterae - ambitum novum incipit) */
interior vacuum
_facta_colligere (
      constans Xar* mensurae,
            chorda  id,
     CursusCurrens* r)
{
    i32 k;

    per (k = ZEPHYRUM; mensurae != NIHIL && k < xar_numerus(mensurae);
         k++)
    {
        constans Mensura* m = (constans Mensura*)xar_obtinere(mensurae,
            k);

        si (!chorda_aequalis(m->parens, id))
        {
            perge;
        }
        si (chorda_aequalis_literis(m->titulus, MENSURA_CUSTOS))
        {
            r->custos = (s32)m->valor;
        }
        alioquin si (chorda_aequalis_literis(m->titulus,
                     MENSURA_PORTAE))
        {
            r->debiti = (i32)m->valor;
        }
        alioquin si (chorda_aequalis_literis(m->titulus,
                     MENSURA_AMBITUS))
        {
            r->ambitus   = (i32)m->valor;
            r->iudicata  = ZEPHYRUM;
        }
        alioquin si (chorda_aequalis_literis(m->titulus,
                     MENSURA_IUDICATA))
        {
            r->iudicata = (i32)m->valor;
        }
        alioquin si (   m->titulus.mensura > (i32)(magnitudo(
                         MENSURA_COLLOQUIUM) - I)
                     && memcmp(m->titulus.datum, MENSURA_COLLOQUIUM,
                         magnitudo(MENSURA_COLLOQUIUM) - I) == ZEPHYRUM)
        {
            /* colloquium (T9): id post praefixum */
            r->colloquium.datum    = m->titulus.datum + (magnitudo(
                MENSURA_COLLOQUIUM) - I);
            r->colloquium.mensura  = m->titulus.mensura
                - (i32)(magnitudo(
                MENSURA_COLLOQUIUM) - I);
        }
    }
}

/* filii FINITI qui consilio numerantur: sub commissione portae
 * ('porta:'), sub cetero membra acta (nota non IUDICIUM - regenerationes
 * iudicis membra non sunt) */
interior vacuum
_filios_numerare (
    constans CursusIndex* index,
         constans Cursus* c,
           CursusCurrens* r)
{
    vacuum* locus = NIHIL;
       Xar* fratres;
       b32  commissio;
       i32  k;

    si (!tabula_dispersa_invenire(index->filii, c->initium.id, &locus))
    {
        redde;
    }
    fratres = (Xar*)locus;
    commissio = chorda_aequalis_literis(c->initium.titulus,
        "commissio");
    per (k = ZEPHYRUM; k < xar_numerus(fratres); k++)
    {
        constans Cursus* f = *(constans Cursus**)xar_obtinere(fratres,
            k);

        si (f->status == CURSUS_INTERRUPTUS)
        {
            perge;
        }
        si (commissio
            ? !chorda_incipit(f->initium.titulus, chorda_ex_literis(
                "porta:", index->piscina))
            : chorda_incipit(f->finis.nota,
            chorda_ex_literis("IUDICIUM",
                index->piscina)))
        {
            perge;
        }
        r->perfecti++;
        si (f->status == CURSUS_FRACTUS)
        {
            r->fracti++;
            /* fracta nominata (T10a): tituli spatiis iuncti */
            r->fracta = (r->fracta.mensura == ZEPHYRUM)
                ? f->initium.titulus
                : chorda_concatenare(chorda_concatenare(r->fracta,
                    chorda_ex_literis(" ", index->piscina),
                    index->piscina), f->initium.titulus,
                    index->piscina);
        }
    }
}

/* historia tituli: V ultimi transeuntes (ordine initii) - minima et
 * maxima duratio */
interior vacuum
_historiam_colligere (
    constans CursusIndex* index,
                  chorda  titulus,
           CursusCurrens* r)
{
    s64 initia[V];
    s64 durationes[V];
    i32 n = ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* h =
            (constans Cursus*)xar_obtinere(index->cursus,
            k);
                    i32 vetustissimus;

        si (   h->status != CURSUS_TRANSIIT
            || !chorda_aequalis(h->initium.titulus, titulus))
        {
            perge;
        }
        si (n < (i32)V)
        {
            initia[n]      = h->initium.initium;
            durationes[n]  = h->finis.duratio_ms;
            n++;
            perge;
        }
        vetustissimus = ZEPHYRUM;
        per (j = I; j < (i32)V; j++)
        {
            si (initia[j] < initia[vetustissimus])
            {
                vetustissimus = j;
            }
        }
        si (h->initium.initium > initia[vetustissimus])
        {
            initia[vetustissimus]      = h->initium.initium;
            durationes[vetustissimus]  = h->finis.duratio_ms;
        }
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        si (j == ZEPHYRUM || durationes[j] < r->minima_ms)
        {
            r->minima_ms = durationes[j];
        }
        si (j == ZEPHYRUM || durationes[j] > r->maxima_ms)
        {
            r->maxima_ms = durationes[j];
        }
    }
}

/* ordo initii: ascendens (filii) aut descendens (radices) */
interior integer
_initium_ascendens (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x = (*(constans Cursus* constans*)a)->initium.initium;
    s64 y = (*(constans Cursus* constans*)b)->initium.initium;

    redde (x < y) ? -I : (x > y) ? I : ZEPHYRUM;
}

interior integer
_initium_descendens (
    constans vacuum* a,
    constans vacuum* b)
{
    redde -_initium_ascendens(a, b);
}

interior vacuum
_currentem_addere (
    constans CursusIndex* index,
            constans Xar* mensurae,
         constans Cursus* c,
                     i32  profunditas,
                  chorda  colloquium,
                     Xar* ordines)
{
     CursusCurrens*  r      = (CursusCurrens*)xar_addere(ordines);
            vacuum*  locus  = NIHIL;
               Xar*  fratres;
   constans Cursus** aperti;
               i32   n;
               i32   k;

    si (r == NIHIL)
    {
        redde;
    }
    memset(r, ZEPHYRUM, magnitudo(CursusCurrens));
    r->cursus       = c;
    r->profunditas  = profunditas;
    _facta_colligere(mensurae, c->initium.id, r);
    /* colloquium radicis filiis hereditatur (T9) */
    si (r->colloquium.mensura == ZEPHYRUM)
    {
        r->colloquium = colloquium;
    }
    colloquium = r->colloquium;
    _filios_numerare(index, c, r);
    _historiam_colligere(index, c->initium.titulus, r);
    si (!tabula_dispersa_invenire(index->filii, c->initium.id, &locus))
    {
        redde;
    }
    fratres  = (Xar*)locus;
    aperti   = (constans Cursus**)piscina_allocare(index->piscina,
        (memoriae_index)(xar_numerus(fratres)
            + I) * magnitudo(Cursus*));
    si (aperti == NIHIL)
    {
        redde;
    }
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(fratres); k++)
    {
        constans Cursus* f = *(constans Cursus**)xar_obtinere(fratres,
            k);

        si (f->status == CURSUS_INTERRUPTUS)
        {
            aperti[n++] = f;
        }
    }
    qsort((vacuum*)aperti, (size_t)n, magnitudo(Cursus*),
        _initium_ascendens);
    per (k = ZEPHYRUM; k < n; k++)
    {
        _currentem_addere(index, mensurae, aperti[k], profunditas + I,
            colloquium, ordines);
    }
}

Xar*
cursus_quaerere_currentes (
       constans CursusIndex* index,
               constans Xar* mensurae,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
               Xar*  ordines;
   constans Cursus** radices;
               i32   n;
               i32   k;

    si (index == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    ordines  = xar_creare(piscina, (i32)magnitudo(CursusCurrens));
    radices  = (constans Cursus**)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(index->cursus) + I)
        * magnitudo(Cursus*));
    si (ordines == NIHIL || radices == NIHIL)
    {
        redde NIHIL;
    }
    /* radix aperta: parens nullus, ignotus aut non apertus (cursus
     * filius necatus sub parente finito) */
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(index->cursus); k++)
    {
        constans Cursus* c =
            (constans Cursus*)xar_obtinere(index->cursus,
            k);
        constans Cursus* parens;

        si (c->status != CURSUS_INTERRUPTUS || !_congruit(c, filtrum))
        {
            perge;
        }
        parens = (c->initium.parens.mensura > ZEPHYRUM)
            ? cursus_invenire(index, c->initium.parens) : NIHIL;
        si (parens != NIHIL && parens->status == CURSUS_INTERRUPTUS)
        {
            perge;
        }
        radices[n++] = c;
    }
    qsort((vacuum*)radices, (size_t)n, magnitudo(Cursus*),
        _initium_descendens);
    per (k = ZEPHYRUM; k < n; k++)
    {
        _currentem_addere(index, mensurae, radices[k], ZEPHYRUM,
            chorda_ex_literis("", piscina), ordines);
    }
    redde ordines;
}

/* LINEA CURRENTIS (colloquia T10a) */

#define CURRENS_CAMPI XIX

hic_manens constans character* constans VITAE_NOMINA[] = {
    "vivus", "mortuus", "ignotus"
};

/* campus sine TAB et linea nova */
interior b32
_campus_purus (
    chorda c)
{
    i32 k;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        si (   c.datum[k] == '\t' || c.datum[k] == '\n'
            || c.datum[k] == '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_campum_addere (
    ChordaAedificator* a,
               chorda  c)
{
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_chorda(a, c);
}

interior vacuum
_numerum_addere (
    ChordaAedificator* a,
                  s64  n)
{
    character numeri[XXXII];

    sprintf(numeri, "%ld", (longus)n);
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_literis(a, numeri);
}

chorda
cursus_currentem_ad_lineam (
    constans CursusCurrens* r,
                CursusVita  vita,
                   Piscina* piscina)
{
    constans CursusInitium* i;
         ChordaAedificator* a;
                    chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   r == NIHIL || r->cursus == NIHIL || piscina == NIHIL
        || (i32)vita < ZEPHYRUM || (i32)vita > (i32)CURSUS_VITA_IGNOTA)
    {
        redde vacua;
    }
    i = &r->cursus->initium;
    si (   !_campus_purus(i->id) || !_campus_purus(i->parens)
        || !_campus_purus(i->titulus) || !_campus_purus(i->arbor)
        || !_campus_purus(i->ramus) || !_campus_purus(r->colloquium)
        || !_campus_purus(r->fracta))
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (a == NIHIL)
    {
        redde vacua;
    }
    chorda_aedificator_appendere_literis(a, "CURRENS");
    _numerum_addere(a, (s64)r->profunditas);
    _campum_addere(a, i->id);
    _campum_addere(a, i->parens);
    _campum_addere(a, i->titulus);
    _campum_addere(a, i->arbor);
    _campum_addere(a, i->ramus);
    _numerum_addere(a, i->initium);
    _numerum_addere(a, (s64)r->custos);
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_literis(a, VITAE_NOMINA[vita]);
    _numerum_addere(a, (s64)r->perfecti);
    _numerum_addere(a, (s64)r->fracti);
    _numerum_addere(a, (s64)r->debiti);
    _numerum_addere(a, (s64)r->iudicata);
    _numerum_addere(a, (s64)r->ambitus);
    _numerum_addere(a, r->minima_ms);
    _numerum_addere(a, r->maxima_ms);
    _campum_addere(a, r->colloquium);
    _campum_addere(a, r->fracta);
    chorda_aedificator_appendere_character(a, '\n');
    redde chorda_aedificator_finire(a);
}

/* numerus integer (signum licet) campi totius; FALSUM si non */
interior b32
_numerum_campi (
    chorda  campus,
       s64* valor)
{
    i32 k          = ZEPHYRUM;
    b32 negativus  = FALSUM;

    si (campus.mensura > ZEPHYRUM && campus.datum[ZEPHYRUM] == '-')
    {
        negativus  = VERUM;
        k          = I;
    }
    si (k >= campus.mensura)
    {
        redde FALSUM;
    }
    *valor = (s64)ZEPHYRUM;
    per (; k < campus.mensura; k++)
    {
        si (campus.datum[k] < '0' || campus.datum[k] > '9')
        {
            redde FALSUM;
        }
        *valor = *valor * (s64)X + (s64)(campus.datum[k] - '0');
    }
    si (negativus)
    {
        *valor = -*valor;
    }
    redde VERUM;
}

b32
cursus_currentem_ex_linea (
                 chorda  linea,
     CursusCurrensLinea* exitus)
{
    chorda campi[CURRENS_CAMPI];
       s64 n[XIX];
       i32 numerus = ZEPHYRUM;
       i32 initium = ZEPHYRUM;
       i32 k;

    si (exitus == NIHIL || linea.datum == NIHIL)
    {
        redde FALSUM;
    }
    dum (   linea.mensura > ZEPHYRUM
         && (   linea.datum[linea.mensura - I] == '\n'
             || linea.datum[linea.mensura - I] == '\r'))
    {
        linea.mensura--;
    }
    per (k = ZEPHYRUM; k <= linea.mensura; k++)
    {
        si (k == linea.mensura || linea.datum[k] == '\t')
        {
            si (numerus >= (i32)CURRENS_CAMPI)
            {
                redde FALSUM;   /* campi nimii */
            }
            campi[numerus].datum    = linea.datum + initium;
            campi[numerus].mensura  = k - initium;
            numerus++;
            initium = k + I;
        }
    }
    si (   numerus != (i32)CURRENS_CAMPI
        || !chorda_aequalis_literis(campi[ZEPHYRUM], "CURRENS"))
    {
        redde FALSUM;
    }
    /* numeri: I, VII, VIII, X..XVI */
    si (   !_numerum_campi(campi[I], &n[I])
        || !_numerum_campi(campi[VII], &n[VII])
        || !_numerum_campi(campi[VIII], &n[VIII]))
    {
        redde FALSUM;
    }
    per (k = X; k <= XVI; k++)
    {
        si (!_numerum_campi(campi[k], &n[k]))
        {
            redde FALSUM;
        }
    }
    memset(exitus, ZEPHYRUM, magnitudo(CursusCurrensLinea));
    exitus->vita = CURSUS_VITA_IGNOTA;
    per (k = ZEPHYRUM; k <= (i32)CURSUS_VITA_IGNOTA; k++)
    {
        si (chorda_aequalis_literis(campi[IX], VITAE_NOMINA[k]))
        {
            frange;
        }
    }
    si (k > (i32)CURSUS_VITA_IGNOTA)
    {
        redde FALSUM;
    }
    exitus->vita         = (CursusVita)k;
    exitus->profunditas  = (i32)n[I];
    exitus->id           = campi[II];
    exitus->parens       = campi[III];
    exitus->titulus      = campi[IV];
    exitus->arbor        = campi[V];
    exitus->ramus        = campi[VI];
    exitus->initium      = n[VII];
    exitus->custos       = (s32)n[VIII];
    exitus->perfecti     = (i32)n[X];
    exitus->fracti       = (i32)n[XI];
    exitus->debiti       = (i32)n[XII];
    exitus->iudicata     = (i32)n[XIII];
    exitus->ambitus      = (i32)n[XIV];
    exitus->minima_ms    = n[XV];
    exitus->maxima_ms    = n[XVI];
    exitus->colloquium   = campi[XVII];
    exitus->fracta       = campi[XVIII];
    redde VERUM;
}
