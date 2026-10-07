/* tabula_nodorum.c - tabula nodorum et agnitio (vide
 * include/tabula_nodorum.h; data in lib/tabula_nodorum_data.c,
 * GENERATA)
 */
#include "tabula_nodorum.h"
#include "chorda.h"
#include <string.h>

/* sedes dispersionis Alexander (potentia II > XIIDCCCCLXVI) */
#define TABULA_NODORUM_SEDES ((i32)0x8000UL)

structura TabulaNodorum {
           i32  numerus;
    Polynomium* alexander;       /* forma normalis */
    Polynomium* jones;
    Polynomium* jones_speculum;  /* J(1/t) */
           s32* amplitudo;       /* summus - imus Alexander */
           s64* determinans;     /* |Delta(-1)|, 0 si non capit */
           i32* sedes;           /* primus index cuiusque sedis */
           i32* sequens;         /* index sequens in eadem sede */
};

/* FNV-1a super octetos textus Alexander (forma
 * polynomium_ad_chordam) */
interior i32
_sedes (
    constans character* textus,
                    i32  longitudo)
{
    i32 h = (i32)0x811C9DC5UL;
    i32 k;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        h = (h
            ^ (i32)(insignatus character)textus[k]) * (i32)0x01000193UL;
    }
    redde h & (TABULA_NODORUM_SEDES - I);
}

/* |p(-1)| in s64; 0 si non capit (filtrum tunc non adhibetur) */
interior s64
_determinans (
    Polynomium  p,
       Piscina* piscina)
{
    Fractio valor;
        s64 d = ZEPHYRUM;

    si (   !polynomium_valor(p, fractio_ex_s64(-I), piscina, &valor)
        || !magnus_ad_s64(magnus_absolutum(fractio_numerator(valor),
            piscina), &d))
    {
        redde ZEPHYRUM;
    }
    redde d;
}

/* primus index nodi cuius Alexander textus datus est; numerus si
 * nullus. Sequentes per t->sequens (ordine tabulae). */
interior i32
_primus_alexander (
    constans TabulaNodorum* t,
                    chorda  textus)
{
    i32 k = t->sedes[_sedes((constans character*)textus.datum,
        textus.mensura)];

    dum (   k < t->numerus && !chorda_aequalis_literis(textus,
        TABULA_NODORUM[k].alexander))
    {
        k = t->sequens[k];
    }
    redde k;
}

/* index sequens eiusdem Alexander post k; numerus si nullus */
interior i32
_sequens_alexander (
    constans TabulaNodorum* t,
                       i32  k)
{
    i32 j = t->sequens[k];

    dum (   j < t->numerus && strcmp(TABULA_NODORUM[j].alexander,
        TABULA_NODORUM[k].alexander) != ZEPHYRUM)
    {
        j = t->sequens[j];
    }
    redde j;
}

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
    t->determinans     = (s64*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(s64));
    t->sedes           = (i32*)piscina_allocare(piscina,
        (memoriae_index)TABULA_NODORUM_SEDES * magnitudo(i32));
    t->sequens         = (i32*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(i32));
    per (k = ZEPHYRUM; k < TABULA_NODORUM_SEDES; k++)
    {
        t->sedes[k] = n;
    }
    /* ordine inverso inserti: catenae ordine tabulae */
    per (k = n; k > ZEPHYRUM; k--)
    {
         constans character* a = TABULA_NODORUM[k - I].alexander;
                        i32  h = _sedes(a, (i32)strlen(a));

        t->sequens[k - I]  = t->sedes[h];
        t->sedes[h]        = k - I;
    }
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
        t->determinans[k]     = _determinans(t->alexander[k], piscina);
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

/* compositum i # j (fila speculi omnia) contra jones */
interior vacuum
_compositum (
    constans TabulaNodorum* tabula,
                       i32  i,
                       i32  j,
                Polynomium  jones,
                   Piscina* piscina,
                   Agnitio* exitus,
                       i32  maximus,
                       i32* numerus)
{
    b32 si_speculum;
    b32 sj_speculum;

    per (si_speculum = FALSUM; si_speculum <= VERUM; si_speculum++)
    {
        si (   si_speculum
            && tabula_nodorum_amphichiralis(&TABULA_NODORUM[i]))
        {
            frange;
        }
        per (sj_speculum = FALSUM; sj_speculum <= VERUM; sj_speculum++)
        {
            Polynomium productum = polynomium_nullum();

            si (   sj_speculum
                && tabula_nodorum_amphichiralis(&TABULA_NODORUM[j]))
            {
                frange;
            }
            /* i == j: (speculum, non) = (non, speculum) */
            si (i == j && sj_speculum < si_speculum)
            {
                perge;
            }
            si (   polynomium_multiplica(_jones(tabula, i, si_speculum),
                    _jones(tabula, j, sj_speculum), piscina, &productum)
                && polynomium_aequalis(productum, jones))
            {
                _notare(exitus, maximus, numerus, i, si_speculum, j,
                    sj_speculum);
            }
        }
    }
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
               s32 amplitudo;
               s64 determinans;

    si (!polynomium_normale(alexander, piscina, &normalis))
    {
        piscina_reficere(piscina, nota);
        redde ZEPHYRUM;
    }
    amplitudo = polynomium_gradus_summus(normalis)
        - polynomium_gradus_imus(normalis);
    determinans = _determinans(normalis, piscina);
    /* primi: Alexander per dispersionem, ut picti, deinde speculum
     * (amphichiralis semel) */
    per (i = _primus_alexander(tabula, polynomium_ad_chordam(normalis,
        't',
            piscina)); i < tabula->numerus; i =
            _sequens_alexander(tabula,
            i))
    {
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
    /* compositi duorum non trivialium: Delta = Delta_i Delta_j, ergo
     * det_i | det, amplitudo_i <= amplitudo (aequalis: factor alter
     * Alexander 1 habet, e.g. 11n_34); quotiens exactus per
     * dispersionem quaeritur (j >= i, ordine tabulae: par semel) */
    per (i = ZEPHYRUM; i < tabula->numerus; i++)
    {
        PiscinaNotatio nota_par;
            Polynomium quotiens = polynomium_nullum();
                   i32 j;

        si (   TABULA_NODORUM[i].transitus == ZEPHYRUM
            || tabula->amplitudo[i] > amplitudo
            || (determinans != ZEPHYRUM && tabula->determinans[i]
                != ZEPHYRUM && determinans % tabula->determinans[i]
                != ZEPHYRUM))
        {
            perge;
        }
        nota_par = piscina_notare(piscina);
        si (polynomium_divide_exacte(normalis, tabula->alexander[i],
                piscina, &quotiens))
        {
            per (j = _primus_alexander(tabula, polynomium_ad_chordam(
                    quotiens, 't', piscina)); j < tabula->numerus;
                j = _sequens_alexander(tabula, j))
            {
                si (j >= i && TABULA_NODORUM[j].transitus != ZEPHYRUM)
                {
                    _compositum(tabula, i, j, jones, piscina, exitus,
                        maximus, &numerus);
                }
            }
        }
        piscina_reficere(piscina, nota_par);
    }
    piscina_reficere(piscina, nota);
    redde numerus;
}
