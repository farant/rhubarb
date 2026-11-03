/* probatio_temperley.c - Probationes modulorum standard Temperley-Lieb
 *
 * Oracula in probatione ipsa, a bibliotheca aliena:
 *   - dimensiones ex formula binomiali et summa d^2 = Catalan(n);
 *   - structura ex paribus (involutio, arcus non transeuntes, defectus
 *     extra arcus, numerus defectuum, formae crescentes);
 *   - enumeratio independens: omnes 3^n formae textus ordine
 *     lexicographico (( < ) < |), servatae quae status sunt, aequales
 *     seriei moduli esse debent;
 *   - oraculum actionis: regulae (a)-(d) in formam textus propriam
 *     applicatae, imago in serie propria quaesita (relationes TL sola
 *     e_i = 0 non videnti);
 *   - relationes TL exacte (index, potentia delta, nullum), n <= XVI.
 * Vide project-specs/temperley-lieb-plan.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "temperley.h"

#define FILA_ORACULI  X           /* enumeratio 3^n et oraculum actionis */
#define SIGNUM_INTACTUM  ((i32)0xDEADUL)


/* ==================================================
 * Auxilia
 * ================================================== */

interior i64
_binomium (
    s32 n,
    s32 k)
{
    i64 r = I;
    s32 t;

    si (k < ZEPHYRUM || k > n)
    {
        redde ZEPHYRUM;
    }
    per (t = I; t <= k; t++)
    {
        r = r * (i64)(n - k + t) / (i64)t;
    }
    redde r;
}

/* forma propria: codex base III, puncto 0 summo, ( = 0, ) = 1, | = 2 */
interior b32
_forma_status (
    i32  codex,
    i32  fila,
    i32  defectus,
     s8* signa)
{
    i32 x;
    s32 profunditas        = ZEPHYRUM;
    i32 numerus_defectuum  = ZEPHYRUM;

    per (x = fila; x > ZEPHYRUM; x--)
    {
        signa[x - I]  = (s8)(codex % III);
        codex         /= III;
    }
    per (x = ZEPHYRUM; x < fila; x++)
    {
        si (signa[x] == ZEPHYRUM)
        {
            profunditas++;
        }
        alioquin si (signa[x] == I)
        {
            si (profunditas == ZEPHYRUM)
            {
                redde FALSUM;
            }
            profunditas--;
        }
        alioquin
        {
            si (profunditas != ZEPHYRUM)
            {
                redde FALSUM;
            }
            numerus_defectuum++;
        }
    }
    redde profunditas == ZEPHYRUM && numerus_defectuum == defectus;
}

/* paria ex forma (acervus parenthesium) */
interior vacuum
_paria_ex_forma (
    constans s8* signa,
            i32  fila,
            s32* paria)
{
    i32 acervus[TEMPERLEY_FILA_MAXIMA];
    i32 altitudo = ZEPHYRUM;
    i32 x;

    per (x = ZEPHYRUM; x < fila; x++)
    {
        si (signa[x] == ZEPHYRUM)
        {
            acervus[altitudo++] = x;
        }
        alioquin si (signa[x] == I)
        {
            i32 a = acervus[--altitudo];

            paria[a] = (s32)x;
            paria[x] = (s32)a;
        }
        alioquin
        {
            paria[x] = TEMPERLEY_DEFECTUS;
        }
    }
}

interior i32
_codex_ex_paribus (
    constans s32* paria,
             i32  fila)
{
    i32 codex = ZEPHYRUM;
    i32 x;

    per (x = ZEPHYRUM; x < fila; x++)
    {
        codex = codex * III + (paria[x] == TEMPERLEY_DEFECTUS ? II
            : (paria[x] > (s32)x ? ZEPHYRUM : I));
    }
    redde codex;
}

/* series propria statuum W_{fila, defectus}: codices crescentes */
interior i32*
_series_propria (
         i32  fila,
         i32  defectus,
     Piscina* piscina,
         i32* numerus)
{
    i32  summa = I;
    i32* series;
    i32  codex;
    i32  n = ZEPHYRUM;
     s8  signa[TEMPERLEY_FILA_MAXIMA];
    i32  x;

    per (x = ZEPHYRUM; x < fila; x++)
    {
        summa *= III;
    }
    series = (i32*)piscina_allocare(piscina, (memoriae_index)summa
        * magnitudo(i32));
    per (codex = ZEPHYRUM; codex < summa; codex++)
    {
        si (_forma_status(codex, fila, defectus, signa))
        {
            series[n++] = codex;
        }
    }
    *numerus = n;
    redde series;
}

interior s32
_quaere_codicem (
    constans i32* series,
             i32  numerus,
             i32  codex)
{
    i32 inferior = ZEPHYRUM;
    i32 superior = numerus;

    dum (inferior < superior)
    {
        i32 medius = inferior + (superior - inferior) / II;

        si (series[medius] < codex)
        {
            inferior = medius + I;
        }
        alioquin
        {
            superior = medius;
        }
    }
    redde (inferior < numerus
        && series[inferior] == codex) ? (s32)inferior
        : -I;
}

/* actio exacta in vectorem basis: (nullum, index, potentia delta) */
nomen structura {
    b32 nullum;
    i32 index;
    i32 potentia;
} Basis;

interior Basis
_age (
    constans TemperleyModulus* m,
                          i32  i,
                        Basis  v)
{
    i32 imago = ZEPHYRUM;
    s32 genus;

    si (v.nullum)
    {
        redde v;
    }
    genus = temperley_actio(m, i, v.index, &imago);
    si (genus == TEMPERLEY_NIHIL || genus == TEMPERLEY_ERRATUM)
    {
        v.nullum = VERUM;
        redde v;
    }
    v.index = imago;
    si (genus == TEMPERLEY_DELTA)
    {
        v.potentia++;
    }
    redde v;
}

interior b32
_aequales (
    Basis a,
    Basis b)
{
    si (a.nullum || b.nullum)
    {
        redde a.nullum == b.nullum;
    }
    redde a.index == b.index && a.potentia == b.potentia;
}

interior b32
_chorda_est (
                chorda  c,
    constans character* textus,
                   i32  longitudo)
{
    i32 x;

    si (c.mensura != longitudo)
    {
        redde FALSUM;
    }
    per (x = ZEPHYRUM; x < longitudo; x++)
    {
        si ((character)c.datum[x] != textus[x])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_dimensiones (
    CredoContextus* c)
{
    i32 n;
    i32 j;

    per (n = I; n <= TEMPERLEY_FILA_MAXIMA; n++)
    {
        i64 summa = ZEPHYRUM;
        i64 summa_exspectata = _binomium((s32)(II * n), (s32)n)
            / (i64)(n + I);

        per (j = n % II; j <= n; j += II)
        {
            TemperleyModulus* m = temperley_modulus_creare(n, j,
                c->piscina);
            s32 h = (s32)((n - j) / II);
            i64 d = _binomium((s32)n, h) - _binomium((s32)n, h - I);

            CREDO_NECESSE_NON_NIHIL(m);
            CREDO_AEQUALIS_I64((i64)temperley_modulus_dimensio(m), d);
            CREDO_AEQUALIS_I32(temperley_modulus_fila(m), n);
            CREDO_AEQUALIS_I32(temperley_modulus_defectus(m), j);
            summa += d * d;
        }
        CREDO_AEQUALIS_I64(summa, summa_exspectata);
    }
}

interior vacuum
_probare_structuram (
    CredoContextus* c)
{
    i32 n;
    i32 j;

    per (n = I; n <= TEMPERLEY_FILA_MAXIMA; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            TemperleyModulus* m = temperley_modulus_creare(n, j,
                c->piscina);
                         i32 d;
                         i32 s;
                         i32 errores = ZEPHYRUM;
                      chorda prior;

            CREDO_NECESSE_NON_NIHIL(m);
            d              = temperley_modulus_dimensio(m);
            prior.mensura  = ZEPHYRUM;
            prior.datum    = NIHIL;
            per (s = ZEPHYRUM; s < d; s++)
            {
                   s32 paria[TEMPERLEY_FILA_MAXIMA];
                   i32 defectus = ZEPHYRUM;
                   i32 x;
                   i32 y;
                chorda forma;

                si (!temperley_status_paria(m, s, paria))
                {
                    errores++;
                    perge;
                }
                per (x = ZEPHYRUM; x < n; x++)
                {
                    si (paria[x] == TEMPERLEY_DEFECTUS)
                    {
                        defectus++;
                        /* nullus arcus defectum includit */
                        per (y = ZEPHYRUM; y < n; y++)
                        {
                            si (   paria[y] > (s32)y && y < x
                                && paria[y] > (s32)x)
                            {
                                errores++;
                            }
                        }
                    }
                    alioquin si (   paria[x] < ZEPHYRUM
                                 || paria[x]             >= (s32)n
                                 || paria[x]             == (s32)x
                                 || paria[(i32)paria[x]] != (s32)x)
                    {
                        errores++;
                    }
                    alioquin si (paria[x] > (s32)x)
                    {
                        /* arcus (x, paria[x]) nullum alium transit */
                        per (y = x + I; (s32)y < paria[x]; y++)
                        {
                            si (   paria[y] != TEMPERLEY_DEFECTUS
                                && paria[y] > paria[x])
                            {
                                errores++;
                            }
                        }
                    }
                }
                si (defectus != j)
                {
                    errores++;
                }
                /* formae stricte crescentes: ( < ) < | etiam in ASCII */
                forma = temperley_status_ad_chordam(m, s, c->piscina);
                si (forma.mensura != n)
                {
                    errores++;
                }
                alioquin si (prior.datum != NIHIL)
                {
                    i32 k = ZEPHYRUM;

                    dum (k < n && forma.datum[k] == prior.datum[k])
                    {
                        k++;
                    }
                    si (k == n || forma.datum[k] < prior.datum[k])
                    {
                        errores++;
                    }
                }
                prior = forma;
            }
            CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
        }
    }
}

interior vacuum
_probare_enumerationem (
    CredoContextus* c)
{
    i32 n;
    i32 j;

    per (n = I; n <= FILA_ORACULI; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            TemperleyModulus* m = temperley_modulus_creare(n, j,
                c->piscina);
            i32  numerus;
            i32* series = _series_propria(n, j, c->piscina, &numerus);
            i32  s;
            i32  errores = ZEPHYRUM;

            CREDO_NECESSE_NON_NIHIL(m);
            CREDO_AEQUALIS_I32(temperley_modulus_dimensio(m), numerus);
            per (s = ZEPHYRUM; s < numerus
                && s < temperley_modulus_dimensio(m); s++)
            {
                s32 paria[TEMPERLEY_FILA_MAXIMA];

                (vacuum)temperley_status_paria(m, s, paria);
                si (_codex_ex_paribus(paria, n) != series[s])
                {
                    errores++;
                }
            }
            CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
        }
    }
}

interior vacuum
_probare_oraculum_actionis (
    CredoContextus* c)
{
    i32 n;
    i32 j;

    per (n = II; n <= FILA_ORACULI; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            TemperleyModulus* m = temperley_modulus_creare(n, j,
                c->piscina);
            i32  numerus;
            i32* series = _series_propria(n, j, c->piscina, &numerus);
            i32  s;
            i32  i;
            i32  errores          = ZEPHYRUM;
            i32  casus_visi[III]  = { ZEPHYRUM, ZEPHYRUM, ZEPHYRUM };

            CREDO_NECESSE_NON_NIHIL(m);
            per (s = ZEPHYRUM; s < numerus; s++)
            {
                per (i = I; i < n; i++)
                {
                     s8 signa[TEMPERLEY_FILA_MAXIMA];
                    s32 paria[TEMPERLEY_FILA_MAXIMA];
                    i32 a = i - I;
                    i32 b = i;
                    s32 genus_exspectatum;
                    s32 index_exspectatus = (s32)s;
                    s32 genus;
                    i32 imago = SIGNUM_INTACTUM;

                    (vacuum)_forma_status(series[s], n, j, signa);
                    _paria_ex_forma(signa, n, paria);
                    si (paria[a] == (s32)b)
                    {
                        genus_exspectatum = TEMPERLEY_DELTA;
                    }
                    alioquin si (   paria[a] == TEMPERLEY_DEFECTUS
                                 && paria[b] == TEMPERLEY_DEFECTUS)
                    {
                        genus_exspectatum = TEMPERLEY_NIHIL;
                    }
                    alioquin
                    {
                        s32 pa = paria[a];
                        s32 pb = paria[b];

                        paria[a] = (s32)b;
                        paria[b] = (s32)a;
                        si (pa != TEMPERLEY_DEFECTUS)
                        {
                            paria[(i32)pa] = pb;
                        }
                        si (pb != TEMPERLEY_DEFECTUS)
                        {
                            paria[(i32)pb] = pa;
                        }
                        genus_exspectatum = TEMPERLEY_UNUM;
                        index_exspectatus = _quaere_codicem(series,
                            numerus,
                            _codex_ex_paribus(paria, n));
                    }
                    genus = temperley_actio(m, i, s, &imago);
                    si (genus != genus_exspectatum)
                    {
                        errores++;
                    }
                    alioquin si (genus == TEMPERLEY_NIHIL)
                    {
                        si (imago != SIGNUM_INTACTUM)
                        {
                            errores++;
                        }
                    }
                    alioquin si (   (s32)imago != index_exspectatus
                                 || index_exspectatus < ZEPHYRUM)
                    {
                        errores++;
                    }
                    si (genus >= ZEPHYRUM && genus <= II)
                    {
                        casus_visi[genus]++;
                    }
                }
            }
            CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
            /* genera omnia visa ubi possibilia sunt */
            si (n >= III && j >= II)
            {
                CREDO_VERUM(casus_visi[TEMPERLEY_NIHIL] > ZEPHYRUM);
            }
            si (n >= II && j + II <= n)
            {
                CREDO_VERUM(casus_visi[TEMPERLEY_DELTA] > ZEPHYRUM);
            }
        }
    }
}

interior vacuum
_probare_relationes (
    CredoContextus* c)
{
    i32 n;
    i32 j;

    per (n = II; n <= TEMPERLEY_FILA_MAXIMA; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            TemperleyModulus* m = temperley_modulus_creare(n, j,
                c->piscina);
                         i32 d;
                         i32 s;
                         i32 i;
                         i32 k;
                         i32 errores = ZEPHYRUM;

            CREDO_NECESSE_NON_NIHIL(m);
            d = temperley_modulus_dimensio(m);
            per (s = ZEPHYRUM; s < d; s++)
            {
                Basis v;

                v.nullum    = FALSUM;
                v.index     = s;
                v.potentia  = ZEPHYRUM;
                per (i = I; i < n; i++)
                {
                    Basis ei      = _age(m, i, v);
                    Basis ei_bis  = _age(m, i, ei);

                    /* e_i^2 = delta e_i */
                    ei.potentia++;
                    si (!_aequales(ei_bis, ei))
                    {
                        errores++;
                    }
                    ei.potentia--;
                    si (i + I < n)
                    {
                        /* e_i e_(i+1) e_i = e_i et e_(i+1) e_i e_(i+1) = e_(i+1) */
                        si (!_aequales(_age(m, i, _age(m, i + I, _age(m,
                            i, v))),
                            ei))
                        {
                            errores++;
                        }
                        si (!_aequales(_age(m, i + I, _age(m, i, _age(m,
                            i + I,
                            v))), _age(m, i + I, v)))
                        {
                            errores++;
                        }
                    }
                    per (k = i + II; k < n; k++)
                    {
                        si (!_aequales(_age(m, i, _age(m, k, v)),
                            _age(m, k,
                            _age(m, i, v))))
                        {
                            errores++;
                        }
                    }
                }
            }
            CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
        }
    }
}

interior vacuum
_probare_quaerere (
    CredoContextus* c)
{
    i32 n;
    i32 j;
    TemperleyModulus* m;
    s32 transeuntia[IV]    = { II, III, ZEPHYRUM, I };
    s32 inclusum[III]      = { II, TEMPERLEY_DEFECTUS, ZEPHYRUM };
    s32 non_reciproca[IV]  = { I, ZEPHYRUM, III, I };
    s32 extra[IV]          = { I, ZEPHYRUM, III, IV };
    s32 nimis_parva[IV]    = { I, ZEPHYRUM, -II, II };

    per (n = I; n <= XII; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            i32 s;
            i32 errores = ZEPHYRUM;

            m = temperley_modulus_creare(n, j, c->piscina);
            CREDO_NECESSE_NON_NIHIL(m);
            per (s = ZEPHYRUM; s < temperley_modulus_dimensio(m); s++)
            {
                s32 paria[TEMPERLEY_FILA_MAXIMA];

                (vacuum)temperley_status_paria(m, s, paria);
                si (temperley_status_quaerere(m, paria) != (s32)s)
                {
                    errores++;
                }
            }
            CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
        }
    }
    m = temperley_modulus_creare(IV, ZEPHYRUM, c->piscina);
    CREDO_AEQUALIS_S32(temperley_status_quaerere(m, transeuntia),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_S32(temperley_status_quaerere(m, non_reciproca),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_S32(temperley_status_quaerere(m, extra),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_S32(temperley_status_quaerere(m, nimis_parva),
        TEMPERLEY_ERRATUM);
    m = temperley_modulus_creare(III, I, c->piscina);
    CREDO_AEQUALIS_S32(temperley_status_quaerere(m, inclusum),
        TEMPERLEY_ERRATUM);
}

interior vacuum
_probare_exempla (
    CredoContextus* c)
{
    TemperleyModulus* m;
                 i32  imago = SIGNUM_INTACTUM;

    /* W_{4,0}: (()) , ()() */
    m = temperley_modulus_creare(IV, ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_I32(temperley_modulus_dimensio(m), II);
    CREDO_VERUM(_chorda_est(temperley_status_ad_chordam(m, ZEPHYRUM,
        c->piscina), "(())", IV));
    CREDO_VERUM(_chorda_est(temperley_status_ad_chordam(m, I,
        c->piscina),
        "()()", IV));
    CREDO_AEQUALIS_S32(temperley_actio(m, I, I, &imago),
        TEMPERLEY_DELTA);
    CREDO_AEQUALIS_I32(imago, I);
    CREDO_AEQUALIS_S32(temperley_actio(m, II, I, &imago),
        TEMPERLEY_UNUM);
    CREDO_AEQUALIS_I32(imago, ZEPHYRUM);
    CREDO_AEQUALIS_S32(temperley_actio(m, II, ZEPHYRUM, &imago),
        TEMPERLEY_DELTA);
    CREDO_AEQUALIS_I32(imago, ZEPHYRUM);
    CREDO_AEQUALIS_S32(temperley_actio(m, I, ZEPHYRUM, &imago),
        TEMPERLEY_UNUM);
    CREDO_AEQUALIS_I32(imago, I);

    /* W_{3,1}: ()| , |() - (c) in utramque partem */
    m = temperley_modulus_creare(III, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_I32(temperley_modulus_dimensio(m), II);
    CREDO_VERUM(_chorda_est(temperley_status_ad_chordam(m, ZEPHYRUM,
        c->piscina), "()|", III));
    CREDO_VERUM(_chorda_est(temperley_status_ad_chordam(m, I,
        c->piscina),
        "|()", III));
    CREDO_AEQUALIS_S32(temperley_actio(m, I, I, &imago),
        TEMPERLEY_UNUM);
    CREDO_AEQUALIS_I32(imago, ZEPHYRUM);
    CREDO_AEQUALIS_S32(temperley_actio(m, II, ZEPHYRUM, &imago),
        TEMPERLEY_UNUM);
    CREDO_AEQUALIS_I32(imago, I);

    /* W_{2,2}: || - e_1 = 0, imago non tacta */
    m = temperley_modulus_creare(II, II, c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    imago = SIGNUM_INTACTUM;
    CREDO_AEQUALIS_S32(temperley_actio(m, I, ZEPHYRUM, &imago),
        TEMPERLEY_NIHIL);
    CREDO_AEQUALIS_I32(imago, SIGNUM_INTACTUM);

    /* W_{1,1}: | - nulla actio */
    m = temperley_modulus_creare(I, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_I32(temperley_modulus_dimensio(m), I);
    CREDO_VERUM(_chorda_est(temperley_status_ad_chordam(m, ZEPHYRUM,
        c->piscina), "|", I));
}

interior vacuum
_probare_refutationes (
    CredoContextus* c)
{
    TemperleyModulus* m;
                 i32  imago      = SIGNUM_INTACTUM;
                 s32  paria[IV]  = { -VII, -VII, -VII, -VII };

    CREDO_NIHIL(temperley_modulus_creare(ZEPHYRUM, ZEPHYRUM,
        c->piscina));
    CREDO_NIHIL(temperley_modulus_creare(XVII, I, c->piscina));
    CREDO_NIHIL(temperley_modulus_creare(IV, I, c->piscina));
    CREDO_NIHIL(temperley_modulus_creare(III, V, c->piscina));
    CREDO_NIHIL(temperley_modulus_creare(II, IV, c->piscina));

    m = temperley_modulus_creare(IV, ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(temperley_actio(m, ZEPHYRUM, ZEPHYRUM, &imago),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_S32(temperley_actio(m, IV, ZEPHYRUM, &imago),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_S32(temperley_actio(m, I, II, &imago),
        TEMPERLEY_ERRATUM);
    CREDO_AEQUALIS_I32(imago, SIGNUM_INTACTUM);
    CREDO_FALSUM(temperley_status_paria(m, II, paria));
    CREDO_AEQUALIS_S32(paria[ZEPHYRUM], -VII);
    CREDO_AEQUALIS_I32(temperley_status_ad_chordam(m, II,
        c->piscina).mensura,
        ZEPHYRUM);
}

interior vacuum
_aedifica_omnes (
    Piscina* piscina)
{
    i32 n;
    i32 j;

    per (n = I; n <= TEMPERLEY_FILA_MAXIMA; n++)
    {
        per (j = n % II; j <= n; j += II)
        {
            (vacuum)temperley_modulus_creare(n, j, piscina);
        }
    }
}

interior vacuum
_probare_non_ruit (
    CredoContextus* c)
{
    CREDO_NON_RUIT(_aedifica_omnes(c->piscina));
}


/* ==================================================
 * Suita
 * ================================================== */

interior constans CredoSectio SECTIONES[] = {
    { "aedificatio non ruit", _probare_non_ruit, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "dimensiones", _probare_dimensiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "structura ex paribus", _probare_structuram, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "enumeratio independens", _probare_enumerationem, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "oraculum actionis", _probare_oraculum_actionis, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "relationes", _probare_relationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quaerere", _probare_quaerere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exempla", _probare_exempla, NIHIL, NIHIL, NIHIL, NIHIL },
    { "refutationes", _probare_refutationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("temperley", SECTIONES);
}
