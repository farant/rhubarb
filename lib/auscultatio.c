/* auscultatio.c - Carmen auditum per numeros (vide
 * include/auscultatio.h)
 *
 * REDDERE: concentus super exitum scripturae (mixtura in aperiendo
 * servata, momentum = quadra reddita, mora 0); pulsus X ms ut vicus:
 * concentus_agere, deinde cantor_agere, deinde quadra pulsus
 * redduntur. Finis: carmen totum redditum ET nulla vox vivit, aut cauda
 * AUSCULTATIO_CAUDA consumpta. Basis (concentus_initium ad incipiendum)
 * postea abscinditur: quadrum 0 = pulsus 0.
 */

#include "auscultatio.h"
#include "concentus.h"
#include "mixtura.h"
#include "sonus.h"
#include <math.h>
#include <string.h>

/* pulsus hospitis (ut vicus) */
#define AUSCULTATIO_PULSUS_MS X
#define AUSCULTATIO_PULSUS_QUADRA (SONUS_FREQUENTIA / C)

nomen structura {
    Mixtura* mixtura;
} Scriptura;

interior b32
_aperire (
     vacuum* ctx,
    Mixtura* mixtura)
{
    ((Scriptura*)ctx)->mixtura = mixtura;
    redde VERUM;
}

interior vacuum
_claudere (
    vacuum* ctx)
{
    (vacuum)ctx;
}

interior s64
_momentum (
    vacuum* ctx)
{
    Scriptura* s = (Scriptura*)ctx;

    redde s->mixtura ? mixtura_momentum(s->mixtura) : ZEPHYRUM;
}

interior i32
_mora (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde ZEPHYRUM;
}

interior b32
_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    redde mixtura_iubere(((Scriptura*)ctx)->mixtura, iussum);
}

interior b32
_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    Scriptura* s = (Scriptura*)ctx;

    redde s->mixtura && mixtura_nuntium_legere(s->mixtura, nuntius);
}

interior s64
_quadrum (
    constans Carmen* carmen,
                s64  pulsus)
{
    redde cantus_quadrum_pulsus(carmen->tempora,
        carmen->numerus_temporum,
        pulsus);
}

/* mensurae carminis: ultima quae ante finem incipit (I saltem) */
interior i32
_mensurae (
    constans Carmen* carmen)
{
    i32 m = I;

    dum (carmen_pulsus_mensurae(carmen, m + I) < carmen->pulsus)
    {
        m++;
    }
    redde m;
}

b32
auscultatio_reddere (
             constans Carmen* carmen,
    constans CantorResolutor* resolutor,
                     Piscina* piscina,
                         f32** exempla,
                         i32* quadra)
{
       MixturaForma  forma;
    ConcentusExitus  exitus;
          Scriptura  scriptura;
          Concentus* concentus;
             Cantor* cantor;
                f32* x;
                s64  carminis = _quadrum(carmen, carmen->pulsus);
                s64  basis;
                s64  capacitas;
                s64  scripta  = ZEPHYRUM;
                s64  nunc     = ZEPHYRUM;

    *exempla               = NIHIL;
    *quadra                = ZEPHYRUM;
    forma.voces            = LXIV;
    forma.iussa            = CCLVI;
    forma.nuntii           = CXXVIII;
    scriptura.mixtura      = NIHIL;
    exitus.aperire         = _aperire;
    exitus.claudere        = _claudere;
    exitus.momentum        = _momentum;
    exitus.mora            = _mora;
    exitus.iubere          = _iubere;
    exitus.nuntium_legere  = _nuntium_legere;
    exitus.ctx             = &scriptura;
    concentus              = concentus_creare(piscina, &forma, &exitus);
    cantor = concentus != NIHIL ? cantor_creare(piscina, concentus,
        resolutor) : NIHIL;
    si (cantor == NIHIL || !concentus_aperire(concentus))
    {
        redde FALSUM;
    }
    basis      = concentus_initium(concentus);
    capacitas  = basis + carminis + AUSCULTATIO_CAUDA
        + AUSCULTATIO_PULSUS_QUADRA;
    x = (f32*)piscina_allocare(piscina,
        (memoriae_index)capacitas * II * magnitudo(f32));
    si (x == NIHIL)
    {
        concentus_destruere(concentus);
        redde FALSUM;
    }
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    (vacuum)cantor_incipere(cantor, carmen, ZEPHYRUM);
    per (;;)
    {
        b32 vivit = concentus_agere(concentus, nunc);

        vivit = cantor_agere(cantor) || vivit;
        si (   (!vivit && scripta >= basis + carminis)
            || scripta + AUSCULTATIO_PULSUS_QUADRA > capacitas)
        {
            frange;
        }
        mixtura_reddere(scriptura.mixtura, x + II * scripta,
            AUSCULTATIO_PULSUS_QUADRA);
        scripta  += AUSCULTATIO_PULSUS_QUADRA;
        nunc     += AUSCULTATIO_PULSUS_MS;
    }
    concentus_destruere(concentus);
    si (scripta > basis)
    {
        memmove(x, x + II * basis,
            (size_t)(scripta - basis) * II * magnitudo(f32));
        *quadra = (i32)(scripta - basis);
    }
    *exempla = x;
    redde VERUM;
}

b32
auscultatio_metiri (
                 constans Carmen* carmen,
                    constans f32* exempla,
                             i32  quadra,
                         Piscina* piscina,
            AuscultatioSummarium* summarium)
{
    f64 summa = 0.0;
    i32 m;
    i32 i;

    summarium->apex     = 0.0f;
    summarium->tonsa    = ZEPHYRUM;
    summarium->rms      = 0.0f;
    summarium->numerus  = _mensurae(carmen);
    summarium->mensurae = (f32*)piscina_allocare(piscina,
        (memoriae_index)summarium->numerus * magnitudo(f32));
    si (summarium->mensurae == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < II * quadra; i++)
    {
        f32 v = exempla[i] < 0.0f ? -exempla[i] : exempla[i];

        summarium->apex = v > summarium->apex ? v : summarium->apex;
        si (v >= 1.0f)
        {
            summarium->tonsa++;
        }
        summa += (f64)exempla[i] * (f64)exempla[i];
    }
    si (quadra > ZEPHYRUM)
    {
        summarium->rms = (f32)sqrt(summa / (f64)(II * quadra));
    }
    /* mensura: summa intra exempla, divisa per longitudinem TOTAM
     * mensurae (pars ultra exempla = silentium) */
    per (m = I; m <= summarium->numerus; m++)
    {
        s64 ab = _quadrum(carmen, carmen_pulsus_mensurae(carmen, m));
        s64 ad = _quadrum(carmen, carmen_pulsus_mensurae(carmen, m
            + I));
        s64 q;
        f64 s = 0.0;

        per (q = ab; q < ad && q < (s64)quadra; q++)
        {
            s += (f64)exempla[II * q] * (f64)exempla[II * q]
                + (f64)exempla[II * q + I] * (f64)exempla[II * q + I];
        }
        summarium->mensurae[m - I] = ad > ab
            ? (f32)sqrt(s / (f64)(II * (ad - ab))) : 0.0f;
    }
    redde VERUM;
}

b32
auscultatio_partem (
             constans Carmen* carmen,
                         i32  pars,
    constans CantorResolutor* resolutor,
                     Piscina* piscina,
        AuscultatioSummarium* summarium)
{
        Carmen  sola;
    CarmenPars* partes;
           f32* x;
           i32  n;
           i32  i;

    si (pars < ZEPHYRUM || pars >= carmen->numerus_partium)
    {
        redde FALSUM;
    }
    partes = (CarmenPars*)piscina_allocare(piscina,
        (memoriae_index)carmen->numerus_partium
            * magnitudo(CarmenPars));
    si (partes == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        partes[i]        = carmen->partes[i];
        partes[i].solus  = i == pars;
    }
    sola         = *carmen;
    sola.partes  = partes;
    redde auscultatio_reddere(&sola, resolutor, piscina, &x, &n)
        && auscultatio_metiri(&sola, x, n, piscina, summarium);
}

/* nota aliqua partis sonat in [ab, ad) pulsuum */
interior b32
_sonat (
    constans CarmenPars* pars,
                    s64  ab,
                    s64  ad)
{
    i32 f;
    i32 j;

    per (f = ZEPHYRUM; f < pars->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &pars->frusta[f];

        per (j = ZEPHYRUM; j < fr->numerus; j++)
        {
            s64 initium = fr->pulsus + fr->notae[j].pulsus;

            si (initium < ad && initium + fr->notae[j].longitudo > ab)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

i32
auscultatio_tacitae (
                  constans Carmen* carmen,
    constans AuscultatioSummarium* summarium,
                              i32* mensurae,
                              i32  capacitas)
{
    b32 solus  = FALSUM;
    i32 n      = ZEPHYRUM;
    i32 m;
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        solus = solus || carmen->partes[i].solus;
    }
    per (m = I; m <= summarium->numerus; m++)
    {
        s64 ab     = carmen_pulsus_mensurae(carmen, m);
        s64 ad     = carmen_pulsus_mensurae(carmen, m + I);
        b32 debet  = FALSUM;

        si (summarium->mensurae[m - I] >= AUSCULTATIO_TACITUM)
        {
            perge;
        }
        /* partes quae sonare debent: non mutae; si sola quaedam,
         * solae */
        per (i = ZEPHYRUM; i < carmen->numerus_partium && !debet; i++)
        {
            constans CarmenPars* p = &carmen->partes[i];

            debet = !p->mutus && (!solus || p->solus)
                && _sonat(p, ab, ad);
        }
        si (debet)
        {
            si (n < capacitas)
            {
                mensurae[n] = m;
            }
            n++;
        }
    }
    redde n;
}
