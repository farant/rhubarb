/* tabula_nodorum.c - tabula nodorum et agnitio (vide
 * include/tabula_nodorum.h; data in lib/tabula_nodorum_data.c,
 * GENERATA)
 */
#include "tabula_nodorum.h"
#include "chorda.h"
#include <string.h>

structura TabulaNodorum {
           i32  numerus;
    Polynomium* alexander;       /* forma normalis */
    Polynomium* jones;
    Polynomium* jones_speculum;  /* J(1/t) */
           s32* amplitudo;       /* summus - imus Alexander */
};

i32
tabula_nodorum_numerus (vacuum)
{
    redde TABULA_NODORUM_NUMERUS;
}

constans NodusTabulae*
tabula_nodorum_nodus (
    i32 i)
{
    si (i >= TABULA_NODORUM_NUMERUS)
    {
        redde NIHIL;
    }
    redde &TABULA_NODORUM[i];
}

constans NodusTabulae*
tabula_nodorum_quaere (
    constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < TABULA_NODORUM_NUMERUS; k++)
    {
        si (strcmp(TABULA_NODORUM[k].titulus, titulus) == ZEPHYRUM)
        {
            redde &TABULA_NODORUM[k];
        }
    }
    redde NIHIL;
}

constans i32*
tabula_nodorum_pd (
    constans NodusTabulae* n)
{
    si (n->transitus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde TABULA_NODORUM_PD + n->initium_pd;
}

b32
tabula_nodorum_amphichiralis (
    constans NodusTabulae* n)
{
    redde n->symmetria != TABULA_NODORUM_REVERSIBILIS
        && n->symmetria != TABULA_NODORUM_CHIRALIS;
}

TabulaNodorum*
tabula_nodorum_aperire (
    Piscina* piscina)
{
    TabulaNodorum* t = (TabulaNodorum*)piscina_allocare(piscina,
        magnitudo(TabulaNodorum));
              i32 n = TABULA_NODORUM_NUMERUS;
              i32 k;

    t->numerus         = n;
    t->alexander       = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->jones           = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->jones_speculum  = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->amplitudo       = (s32*)piscina_allocare(piscina,
        (memoriae_index)n
        * magnitudo(s32));
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (   !polynomium_ex_chorda(chorda_ex_literis(
                TABULA_NODORUM[k].alexander, piscina), 't', piscina,
                &t->alexander[k])
            || !polynomium_ex_chorda(chorda_ex_literis(
                TABULA_NODORUM[k].jones, piscina), 't', piscina,
                &t->jones[k]))
        {
            redde NIHIL;
        }
        t->jones_speculum[k]  = polynomium_inversum(t->jones[k],
            piscina);
        t->amplitudo[k]       =
            polynomium_gradus_summus(t->alexander[k])
            - polynomium_gradus_imus(t->alexander[k]);
    }
    redde t;
}

/* candidatus numeratur; scribitur si locus restat */
interior vacuum
_notare (
    Agnitio* exitus,
        i32  maximus,
        i32* numerus,
        i32  primus,
        b32  primus_speculum,
        i32  secundus,
        b32  secundus_speculum)
{
    si (*numerus < maximus)
    {
        Agnitio* a = &exitus[*numerus];

        a->primus           = &TABULA_NODORUM[primus];
        a->primus_speculum  = primus_speculum;
        a->secundus           = secundus
            == TABULA_NODORUM_NUMERUS ? NIHIL
            : &TABULA_NODORUM[secundus];
        a->secundus_speculum  = secundus_speculum;
    }
    (*numerus)++;
}

/* Jones nodi k: diagramma tabulae aut speculum eius */
interior Polynomium
_jones (
    constans TabulaNodorum* t,
                       i32  k,
                       b32  speculum)
{
    redde speculum ? t->jones_speculum[k] : t->jones[k];
}

i32
tabula_nodorum_agnoscere (
    constans TabulaNodorum* tabula,
                Polynomium  alexander,
                Polynomium  jones,
                   Piscina* piscina,
                   Agnitio* exitus,
                       i32  maximus)
{
    PiscinaNotatio nota      = piscina_notare(piscina);
        Polynomium normalis  = polynomium_nullum();
               i32 numerus   = ZEPHYRUM;
               i32 i;
               i32 j;
               s32 amplitudo;

    si (!polynomium_normale(alexander, piscina, &normalis))
    {
        piscina_reficere(piscina, nota);
        redde ZEPHYRUM;
    }
    amplitudo = polynomium_gradus_summus(normalis)
        - polynomium_gradus_imus(normalis);
    /* primi: ut picti, deinde speculum (amphichiralis semel) */
    per (i = ZEPHYRUM; i < tabula->numerus; i++)
    {
        si (!polynomium_aequalis(tabula->alexander[i], normalis))
        {
            perge;
        }
        si (polynomium_aequalis(tabula->jones[i], jones))
        {
            _notare(exitus, maximus, &numerus, i, FALSUM,
                TABULA_NODORUM_NUMERUS, FALSUM);
        }
        si (   !tabula_nodorum_amphichiralis(&TABULA_NODORUM[i])
            && polynomium_aequalis(tabula->jones_speculum[i], jones))
        {
            _notare(exitus, maximus, &numerus, i, VERUM,
                TABULA_NODORUM_NUMERUS, FALSUM);
        }
    }
    /* compositi duorum non trivialium: amplitudines Alexander
     * summantur, deinde producta exacta */
    per (i = ZEPHYRUM; i < tabula->numerus; i++)
    {
        si (   TABULA_NODORUM[i].transitus == ZEPHYRUM
            || tabula->amplitudo[i]        >= amplitudo)
        {
            perge;
        }
        per (j = i; j < tabula->numerus; j++)
        {
            PiscinaNotatio nota_par;
                Polynomium productum = polynomium_nullum();
                       b32 si_speculum;
                       b32 sj_speculum;

            si (   TABULA_NODORUM[j].transitus == ZEPHYRUM
                || tabula->amplitudo[i] + tabula->amplitudo[j]
                    != amplitudo)
            {
                perge;
            }
            nota_par = piscina_notare(piscina);
            si (   !polynomium_multiplica(tabula->alexander[i],
                    tabula->alexander[j], piscina, &productum)
                || !polynomium_aequalis(productum, normalis))
            {
                piscina_reficere(piscina, nota_par);
                perge;
            }
            per (si_speculum = FALSUM; si_speculum
                <= VERUM; si_speculum++)
            {
                si (   si_speculum
                    && tabula_nodorum_amphichiralis(&TABULA_NODORUM[i]))
                {
                    frange;
                }
                per (sj_speculum = FALSUM; sj_speculum <= VERUM;
                    sj_speculum++)
                {
                    si (   sj_speculum
                        && tabula_nodorum_amphichiralis(
                            &TABULA_NODORUM[j]))
                    {
                        frange;
                    }
                    /* i == j: (speculum, non) = (non, speculum) */
                    si (i == j && sj_speculum < si_speculum)
                    {
                        perge;
                    }
                    si (   polynomium_multiplica(_jones(tabula, i,
                        si_speculum),
                            _jones(tabula, j, sj_speculum), piscina,
                            &productum)
                        && polynomium_aequalis(productum, jones))
                    {
                        _notare(exitus, maximus, &numerus, i,
                            si_speculum,
                            j, sj_speculum);
                    }
                }
            }
            piscina_reficere(piscina, nota_par);
        }
    }
    piscina_reficere(piscina, nota);
    redde numerus;
}
