/* partitio_aestimatio.c - iudicium partitionum retentum (T35 b; vide
 * partitio_aestimatio.h) */
#include "partitio_aestimatio.h"
#include <string.h>

constans character* constans
    PARTITIO_TITULI_HABITUUM[PARTITIO_HABITUS_NUMERUS] = {
    "coactus", "ordinatus", "apertus"
};


/* ==================================================
 * Adiutores interni
 * ================================================== */

interior memoriae_index
_saltem_unum (
    i32 numerus)
{
    redde (memoriae_index)(numerus > ZEPHYRUM ? numerus : I);
}

interior vacuum*
_allocare_nullum (
           Piscina* piscina,
    memoriae_index  mensura)
{
    vacuum* memoria;

    memoria = piscina_allocare(piscina, mensura);
    si (memoria != NIHIL)
    {
        memset(memoria, ZEPHYRUM, mensura);
    }
    redde memoria;
}

/* numerus DISCIPLINAE: totalis minus sors retenta - lex retentionis uno
 * loco, a basi et a suffragiis communis */
interior i32
_disciplina (
    i32 totalis,
    i32 in_sorte)
{
    redde totalis - in_sorte;
}

interior memoriae_index
_locus (
    i32 g,
    i32 a,
    i32 s,
    i32 numerus_aureorum,
    i32 numerus_sortium)
{
    redde ((memoriae_index)g * numerus_aureorum + a) * numerus_sortium
        + s;
}


/* ==================================================
 * Aestimatio
 * ================================================== */

PartitioAestimatio*
partitio_aestimare (
               Piscina* piscina,
     constans Partitio* p,
     constans Partitio* aurum,
     constans Partitio* sortes,
                   i32  limen)
{
    PartitioAestimatio* e;
                   i32* numeri_sortium;   /* [(g*A+a)*S+s] */
                   i32* aurea_sortium;    /* [a*S+s] */
                   i32* aurea;            /* [a] */
                   i32* totales;          /* [g] ordines gregis */
                   i32  numerus_gregum;
                   i32  numerus_aureorum;
                   i32  numerus_sortium;
                   i64  maxima_summa;
                   b32  captivus_limine = FALSUM;
                   i32  i;
                   i32  g;
                   i32  a;
                   i32  s;

    si (   piscina    == NIHIL || p == NIHIL || aurum == NIHIL
        || sortes     == NIHIL
        || p->numerus != aurum->numerus
        || p->numerus != sortes->numerus)
    {
        redde NIHIL;
    }
    numerus_gregum    = p->numerus_gregum;
    numerus_aureorum  = aurum->numerus_gregum;
    numerus_sortium   = sortes->numerus_gregum;

    e = (PartitioAestimatio*)_allocare_nullum(piscina,
        magnitudo(PartitioAestimatio));
    si (e == NIHIL)
    {
        redde NIHIL;
    }
    e->numerus_gregum    = numerus_gregum;
    e->numerus_aureorum  = numerus_aureorum;
    e->numerus_sortium   = numerus_sortium;
    e->sortes           = (AestimatioSortis*)_allocare_nullum(piscina,
        _saltem_unum(numerus_sortium) * magnitudo(AestimatioSortis));
    e->suffragia        = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_sortium)
        * magnitudo(i32));
    e->numeri           = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_aureorum)
        * magnitudo(i32));
    numeri_sortium      = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_aureorum)
        * _saltem_unum(numerus_sortium) * magnitudo(i32));
    aurea_sortium       = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_aureorum) * _saltem_unum(numerus_sortium)
        * magnitudo(i32));
    aurea               = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_aureorum) * magnitudo(i32));
    totales             = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * magnitudo(i32));
    e->captivus         = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * magnitudo(i32));
    e->habitus          = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * magnitudo(i32));
    e->mutabilis        = (b32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * magnitudo(b32));
    e->retenti          = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_sortium)
        * magnitudo(i32));
    e->recti_gregum     = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_sortium)
        * magnitudo(i32));
    e->margo            = (i32*)_allocare_nullum(piscina,
        _saltem_unum(numerus_gregum) * _saltem_unum(numerus_sortium)
        * magnitudo(i32));
    si (   e->sortes     == NIHIL || e->suffragia == NIHIL
        || e->numeri     == NIHIL || numeri_sortium == NIHIL
        || aurea_sortium == NIHIL || aurea == NIHIL
        || totales       == NIHIL || e->captivus == NIHIL
        || e->habitus    == NIHIL || e->mutabilis == NIHIL
        || e->retenti    == NIHIL || e->recti_gregum == NIHIL
        || e->margo      == NIHIL)
    {
        redde NIHIL;
    }
    per (g = ZEPHYRUM; g < numerus_gregum; g++)
    {
        e->captivus[g] = numerus_sortium;
    }

    /* I. numeri: grex x aurum x sors */
    per (i = ZEPHYRUM; i < p->numerus; i++)
    {
        memoriae_index locus;

        g = p->grex[i];
        a = aurum->grex[i];
        s = sortes->grex[i];
        locus = _locus(g, a, s, numerus_aureorum, numerus_sortium);
        numeri_sortium[locus] = numeri_sortium[locus] + I;
        totales[g] = totales[g] + I;
        e->numeri[(memoriae_index)g * numerus_aureorum + a] =
            e->numeri[(memoriae_index)g * numerus_aureorum + a] + I;
        aurea_sortium[(memoriae_index)a * numerus_sortium + s] =
            aurea_sortium[(memoriae_index)a * numerus_sortium + s] + I;
        aurea[a] = aurea[a] + I;
    }

    /* II. sors quaeque retenta */
    per (s = ZEPHYRUM; s < numerus_sortium; s++)
    {
        AestimatioSortis* sors          = &e->sortes[s];
                     i32  basis_sortis  = ZEPHYRUM;
                     i32  optima        = ZEPHYRUM;

        /* basis: aureus disciplinae maximus; par -> index minimus */
        per (a = ZEPHYRUM; a < numerus_aureorum; a++)
        {
            i32 in_sorte;
            i32 disciplina;

            in_sorte   = aurea_sortium[(memoriae_index)a
                * numerus_sortium + s];
            disciplina     = _disciplina(aurea[a], in_sorte);
            sors->ordines  = sors->ordines + in_sorte;
            si (a == ZEPHYRUM || disciplina > optima)
            {
                optima        = disciplina;
                basis_sortis  = a;
            }
        }
        si (numerus_aureorum > ZEPHYRUM)
        {
            sors->basis = aurea_sortium[(memoriae_index)basis_sortis
                * numerus_sortium + s];
        }

        per (g = ZEPHYRUM; g < numerus_gregum; g++)
        {
            memoriae_index locus_gs  = (memoriae_index)g
                * numerus_sortium + s;
                       i32 disciplina  = ZEPHYRUM;
                       i32 in_sorte    = ZEPHYRUM;
                       i32 maxima      = ZEPHYRUM;
                       i32 secundum    = ZEPHYRUM;
                       i32 candidatum  = ZEPHYRUM;
                       i32 paria       = ZEPHYRUM;
                       i32 suffragium  = numerus_aureorum;
                       i32 responsum   = basis_sortis;

            per (a = ZEPHYRUM; a < numerus_aureorum; a++)
            {
                i32 numerus_in_sorte;
                i32 numerus_disciplinae;

                numerus_in_sorte    = numeri_sortium[_locus(g, a, s,
                    numerus_aureorum, numerus_sortium)];
                numerus_disciplinae = _disciplina(
                    e->numeri[(memoriae_index)g * numerus_aureorum + a],
                    numerus_in_sorte);
                disciplina  = disciplina + numerus_disciplinae;
                in_sorte    = in_sorte + numerus_in_sorte;
                si (a == ZEPHYRUM || numerus_disciplinae > maxima)
                {
                    secundum    = a == ZEPHYRUM ? ZEPHYRUM : maxima;
                    maxima      = numerus_disciplinae;
                    candidatum  = a;
                    paria       = I;
                }
                alioquin si (numerus_disciplinae == maxima)
                {
                    secundum  = maxima;
                    paria     = paria + I;
                }
                alioquin si (numerus_disciplinae > secundum)
                {
                    secundum = numerus_disciplinae;
                }
            }
            si (numerus_aureorum > ZEPHYRUM && disciplina >= limen)
            {
                suffragium  = paria > I ? basis_sortis : candidatum;
                responsum   = suffragium;
            }
            alioquin
            {
                sors->inaestimati = sors->inaestimati + in_sorte;
            }
            e->suffragia[locus_gs]  = suffragium;
            e->retenti[locus_gs]    = in_sorte;
            e->margo[locus_gs]      = maxima - secundum;
            /* captivus: ordines omnes gregis in hac sorte */
            si (in_sorte > ZEPHYRUM && in_sorte == totales[g])
            {
                e->captivus[g] = s;
            }
            si (numerus_aureorum > ZEPHYRUM)
            {
                i32 recti_gregis = numeri_sortium[_locus(g, responsum,
                    s, numerus_aureorum, numerus_sortium)];

                sors->recti                = sors->recti + recti_gregis;
                e->recti_gregum[locus_gs]  = recti_gregis;
            }
        }
        e->summa.ordines = e->summa.ordines + sors->ordines;
        e->summa.recti = e->summa.recti + sors->recti;
        e->summa.basis = e->summa.basis + sors->basis;
        e->summa.inaestimati = e->summa.inaestimati + sors->inaestimati;
    }

    /* III. habitus et mutabilitas gregum (T36 a, decisio LVI): coactus
     * = sors quaeque suffragatur unanimis (margo == disciplina);
     * ordinatus = sors quaeque suffragatur margine > 0, idem ubique;
     * apertus aliter (sine suffragio, par, discors) */
    per (g = ZEPHYRUM; g < numerus_gregum; g++)
    {
        i32 primum    = numerus_aureorum;
        b32 apertus   = FALSUM;
        b32 unanimis  = VERUM;

        per (s = ZEPHYRUM; s < numerus_sortium; s++)
        {
            memoriae_index locus_gs    = (memoriae_index)g
                * numerus_sortium + s;
                       i32 votum       = e->suffragia[locus_gs];
                       i32 disciplina  = totales[g]
                           - e->retenti[locus_gs];

            si (   votum              == numerus_aureorum
                || e->margo[locus_gs] == ZEPHYRUM)
            {
                apertus = VERUM;
            }
            si (e->margo[locus_gs] != disciplina)
            {
                unanimis = FALSUM;
            }
            si (votum == numerus_aureorum)
            {
                perge;
            }
            si (primum == numerus_aureorum)
            {
                primum = votum;
            }
            alioquin si (votum != primum)
            {
                e->mutabilis[g] = VERUM;
            }
        }
        si (e->mutabilis[g])
        {
            e->greges_mutabiles  = e->greges_mutabiles + I;
            apertus              = VERUM;
        }
        e->habitus[g] = apertus ? (i32)PARTITIO_HABITUS_APERTUS
            : unanimis ? (i32)PARTITIO_HABITUS_COACTUS
            : (i32)PARTITIO_HABITUS_ORDINATUS;
    }

    /* IV. puritas in specimine, lucrum, inaestimabilis */
    maxima_summa = ZEPHYRUM;
    per (g = ZEPHYRUM; g < numerus_gregum; g++)
    {
        i32 maximum = ZEPHYRUM;

        per (a = ZEPHYRUM; a < numerus_aureorum; a++)
        {
            si (e->numeri[(memoriae_index)g * numerus_aureorum + a]
                > maximum)
            {
                maximum = e->numeri[(memoriae_index)g * numerus_aureorum
                    + a];
            }
        }
        maxima_summa = maxima_summa + (i64)maximum;
    }
    si (p->numerus > ZEPHYRUM)
    {
        e->puritas_intra = (i32)(maxima_summa * (i64)M
            / (i64)p->numerus);
    }
    e->lucrum = (s32)e->summa.recti - (s32)e->summa.basis;
    /* captivi (decisio LV): grex captivus cum ordinibus >= limen
     * columnam inaestimabilem signat praeter regulam dimidii */
    per (g = ZEPHYRUM; g < numerus_gregum; g++)
    {
        si (e->captivus[g] < numerus_sortium)
        {
            e->greges_captivi   = e->greges_captivi + I;
            e->ordines_captivi  = e->ordines_captivi + totales[g];
            si (totales[g] >= limen)
            {
                captivus_limine = VERUM;
            }
        }
    }
    e->inaestimabilis = (b32)(II * e->summa.inaestimati
        > e->summa.ordines || captivus_limine);
    redde e;
}


/* ==================================================
 * Catena
 * ================================================== */

vacuum
partitio_catena_optiones_initium (
    PartitioCatenaOptiones* optiones)
{
    si (optiones == NIHIL)
    {
        redde;
    }
    optiones->limen           = XX;
    optiones->lucrum_minimum  = ZEPHYRUM;
    optiones->sortes_vetant   = VERUM;
    optiones->semen           = NIHIL;
}

PartitioCatena*
partitio_catenam_struere (
                         Piscina* piscina,
      constans Partitio* constans* columnae,
                              i32 k,
               constans Partitio* aurum,
               constans Partitio* sortes,
    constans PartitioCatenaOptiones* optiones)
{
         PartitioCatena* catena;
      constans Partitio* currens;
     PartitioAestimatio* aestimatio_currens;
                    b32* adhibita;
                    b32* tentata;
                    b32* cadit;
                    i32* recti;
                    i32  limen;
                    i32  lucrum_minimum;
                    b32  sortes_vetant;
                    i32  i;
                    i32  s;

    si (   piscina  == NIHIL || aurum == NIHIL || sortes == NIHIL
        || optiones == NIHIL || (columnae == NIHIL && k > ZEPHYRUM)
        || (   optiones->semen != NIHIL
            && optiones->semen->numerus != aurum->numerus))
    {
        redde NIHIL;
    }
    limen           = optiones->limen;
    lucrum_minimum  = optiones->lucrum_minimum > ZEPHYRUM
        ? optiones->lucrum_minimum : optiones->limen;
    sortes_vetant   = optiones->sortes_vetant;
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (   columnae[i]          == NIHIL
            || columnae[i]->numerus != aurum->numerus)
        {
            redde NIHIL;
        }
    }
    catena = (PartitioCatena*)_allocare_nullum(piscina,
        magnitudo(PartitioCatena));
    si (catena == NIHIL)
    {
        redde NIHIL;
    }
    catena->columnae      = (i32*)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(i32));
    catena->partes        = (Partitio**)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(Partitio*));
    catena->aestimationes = (PartitioAestimatio**)_allocare_nullum(
        piscina, _saltem_unum(k) * magnitudo(PartitioAestimatio*));
    adhibita = (b32*)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(b32));
    tentata  = (b32*)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(b32));
    cadit    = (b32*)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(b32));
    recti    = (i32*)_allocare_nullum(piscina,
        _saltem_unum(k) * magnitudo(i32));
    si (   catena->columnae == NIHIL || catena->partes == NIHIL
        || catena->aestimationes == NIHIL || adhibita == NIHIL
        || tentata == NIHIL || cadit == NIHIL || recti == NIHIL)
    {
        redde NIHIL;
    }
    currens = optiones->semen != NIHIL ? optiones->semen
        : partitio_una(piscina, aurum->numerus);
    aestimatio_currens = currens != NIHIL
        ? partitio_aestimare(piscina, currens, aurum, sortes, limen)
        : NIHIL;
    si (aestimatio_currens == NIHIL)
    {
        redde NIHIL;
    }
    catena->semen    = currens;
    catena->initium  = aestimatio_currens;

    dum (catena->numerus < k)
    {
        PiscinaNotatio nota;
                   b32 acceptum = FALSUM;

        /* candidatae in regione notata: numeri soli servantur */
        nota = piscina_notare(piscina);
        per (i = ZEPHYRUM; i < k; i++)
        {
                       Partitio* candidata;
             PartitioAestimatio* e;

            si (adhibita[i])
            {
                perge;
            }
            candidata = partitio_infimum(piscina, currens, columnae[i]);
            e = candidata != NIHIL
                ? partitio_aestimare(piscina, candidata, aurum, sortes,
                    limen)
                : NIHIL;
            si (e == NIHIL)
            {
                piscina_reficere(piscina, nota);
                redde NIHIL;
            }
            recti[i] = e->summa.recti;
            cadit[i] = FALSUM;
            per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
            {
                si (e->sortes[s].recti
                    < aestimatio_currens->sortes[s].recti)
                {
                    cadit[i] = VERUM;
                    frange;
                }
            }
        }
        piscina_reficere(piscina, nota);

        per (i = ZEPHYRUM; i < k; i++)
        {
            tentata[i] = FALSUM;
        }
        dum (VERUM)
        {
            i32 optimus = k;

            per (i = ZEPHYRUM; i < k; i++)
            {
                si (adhibita[i] || tentata[i])
                {
                    perge;
                }
                si (optimus == k || recti[i] > recti[optimus])
                {
                    optimus = i;
                }
            }
            si (optimus == k)
            {
                frange;
            }
            /* optima lucro deficiens: ceterae quoque (recti minores) */
            si (  (s32)recti[optimus]
                - (s32)aestimatio_currens->summa.recti
                < (s32)lucrum_minimum)
            {
                frange;
            }
            si (sortes_vetant && cadit[optimus])
            {
                tentata[optimus] = VERUM;
                perge;
            }
            {
                           Partitio* accepta;
                 PartitioAestimatio* e;

                accepta = partitio_infimum(piscina, currens,
                    columnae[optimus]);
                e = accepta != NIHIL
                    ? partitio_aestimare(piscina, accepta, aurum,
                    sortes,
                        limen)
                    : NIHIL;
                si (e == NIHIL)
                {
                    redde NIHIL;
                }
                catena->columnae[catena->numerus] = optimus;
                catena->partes[catena->numerus] = accepta;
                catena->aestimationes[catena->numerus] = e;
                catena->numerus = catena->numerus + I;
                adhibita[optimus] = VERUM;
                currens = accepta;
                aestimatio_currens = e;
                acceptum = VERUM;
            }
            frange;
        }
        si (!acceptum)
        {
            frange;
        }
    }
    redde catena;
}
