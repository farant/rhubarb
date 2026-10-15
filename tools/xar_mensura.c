/* xar_mensura.c - MENSURA XAR: nanosecundae per operationem
 *
 * Viae metiendae (quaeque bis: IV * MXXIV elementa, in cella manent;
 * et M * MXXIV, memoriam principalem tangunt):
 *   addere       xar_addere per elementum (piscina recens per gyrum)
 *   seriatim     ansa domus: i < xar_numerus(x), xar_obtinere(x, i)
 *   temere       xar_obtinere ad indices fortuitos (sors, semen fixum)
 *   iterator     xar_iterator_proximum
 *   lexemata     Xar indicum ad structuras: *(T**)xar_obtinere(x, i)
 *                deinde ->genus (forma ansarum silvae)
 *   plana        tabula C plana, ansa eadem (limes inferior)
 * Tempus = tempus PROCESSORIS (clock), optimum ex V mensuris; gyri ut
 * mensura una ~L ms teneat. addere etiam octetos et allocationes per
 * elementum ex piscina refert (piscina_summa_usus,
 * piscina_numerus_allocationum).
 *
 * Usus: ./bin/aedilis tools/xar_mensura.c && bash
 *   build/aedilis/xar_mensura/struere.sh &&
 *   ./build/aedilis/xar_mensura/xar_mensura [-machina]
 *   -machina: MENSURA <via> <elementa> <ns/op> <octeti/op> <alloc/op>
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "sors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MENSURAE V

/* consumptor: ne compilator calculum ut mortuum deleat */
interior volatilis i32 consumptor;

nomen structura {
    i32 genus;
    i32 positus;
} LexemaFictum;

nomen structura {
    f64 nanosecundae;
    f64 octeti;
    f64 allocationes;
} Fructus;

interior f64
_secundae (
    clock_t initium,
    clock_t finis)
{
    redde (f64)(finis - initium) / (f64)CLOCKS_PER_SEC;
}

/* Xar de i32 0..n-1 */
interior Xar*
_xar_implere (
     Piscina* piscina,
         i32  n)
{
    Xar* x = xar_creare(piscina, (i32)magnitudo(i32));
    i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        *(i32*)xar_addere(x) = i;
    }
    redde x;
}

interior Fructus
_addere_metiri (
    i32 n,
    i32 gyri)
{
    Fructus fructus;
        f64 optimum = -1.0;
        i32 m;

    memset(&fructus, ZEPHYRUM, magnitudo(fructus));
    per (m = ZEPHYRUM; m < MENSURAE; m++)
    {
        clock_t summa = ZEPHYRUM;
            i32 g;

        per (g = ZEPHYRUM; g < gyri; g++)
        {
            Piscina* p = piscina_generare_dynamicum("xar_mensura",
                (memoriae_index)65536);
                 Xar* x;
                 i32  i;
             clock_t  initium;

            si (p == NIHIL)
            {
                redde fructus;
            }
            initium  = clock();
            x        = xar_creare(p, (i32)magnitudo(i32));
            per (i = ZEPHYRUM; i < n; i++)
            {
                *(i32*)xar_addere(x) = i;
            }
            summa += clock() - initium;
            si (g == ZEPHYRUM && m == ZEPHYRUM)
            {
                fructus.octeti = (f64)piscina_summa_usus(p) / (f64)n;
                fructus.allocationes =
                    (f64)piscina_numerus_allocationum(p) / (f64)n;
            }
            piscina_destruere(p);
        }
        {
            f64 t = (f64)summa / (f64)CLOCKS_PER_SEC;

            si (optimum < 0.0 || t < optimum)
            {
                optimum = t;
            }
        }
    }
    fructus.nanosecundae = optimum * 1e9 / ((f64)n * (f64)gyri);
    redde fructus;
}

/* modus: 0 seriatim, I temere, II iterator, III lexemata, IV plana */
interior f64
_legere_metiri (
         i32 modus,
         i32 n,
         i32 gyri)
{
     Piscina* p = piscina_generare_dynamicum("xar_mensura_lectio",
         (memoriae_index)1048576);
          Xar* x;
          Xar* indices_lexematum;
          i32* indices;
          i32* plana;
 LexemaFictum* lexemata;
         Sors  s;
          f64  optimum = -1.0;
          i32  m;
          i32  i;

    si (p == NIHIL)
    {
        redde -1.0;
    }
    x        = _xar_implere(p, n);
    indices  = (i32*)malloc((memoriae_index)n * magnitudo(i32));
    plana    = (i32*)malloc((memoriae_index)n * magnitudo(i32));
    lexemata = (LexemaFictum*)malloc((memoriae_index)n
        * magnitudo(LexemaFictum));
    indices_lexematum = xar_creare(p, (i32)magnitudo(LexemaFictum*));
    si (indices == NIHIL || plana == NIHIL || lexemata == NIHIL)
    {
        redde -1.0;
    }
    sors_seminare(&s, (i64)XLII, (i64)ZEPHYRUM);
    per (i = ZEPHYRUM; i < n; i++)
    {
        indices[i] = sors_intra(&s, n);
        plana[i] = i;
        lexemata[i].genus = i & XV;
        lexemata[i].positus = i;
        *(LexemaFictum**)xar_addere(indices_lexematum) = &lexemata[i];
    }
    per (m = ZEPHYRUM; m < MENSURAE; m++)
    {
        clock_t initium  = clock();
            i32 summa    = ZEPHYRUM;
            i32 g;
            f64 t;

        per (g = ZEPHYRUM; g < gyri; g++)
        {
            si (modus == ZEPHYRUM)
            {
                per (i = ZEPHYRUM; i < xar_numerus(x); i++)
                {
                    summa += *(i32*)xar_obtinere(x, i);
                }
            }
            alioquin si (modus == I)
            {
                per (i = ZEPHYRUM; i < n; i++)
                {
                    summa += *(i32*)xar_obtinere(x, indices[i]);
                }
            }
            alioquin si (modus == II)
            {
                XarIterator  it = xar_iterator_initium(x);
                        i32* v;

                dum ((v = (i32*)xar_iterator_proximum(&it)) != NIHIL)
                {
                    summa += *v;
                }
            }
            alioquin si (modus == III)
            {
                per (i = ZEPHYRUM; i
                    < xar_numerus(indices_lexematum); i++)
                {
                    summa += (*(LexemaFictum**)xar_obtinere(
                        indices_lexematum, i))->genus;
                }
            }
            alioquin
            {
                per (i = ZEPHYRUM; i < n; i++)
                {
                    summa += plana[i];
                }
            }
        }
        consumptor  = summa;
        t           = _secundae(initium, clock());
        si (optimum < 0.0 || t < optimum)
        {
            optimum = t;
        }
    }
    free(indices);
    free(plana);
    free(lexemata);
    piscina_destruere(p);
    redde optimum * 1e9 / ((f64)n * (f64)gyri);
}

s32
principale (
          s32   argc,
    character** argv)
{
    hic_manens constans character* constans tituli[] = {
        "seriatim", "temere", "iterator", "lexemata", "plana"
    };
    hic_manens constans i32 mensurae_n[] = { IV * MXXIV, M * MXXIV };
                        b32 machina = (argc > I
                            && strcmp(argv[I], "-machina") == ZEPHYRUM);
                    i32 k;
                    i32 modus;

    si (!machina)
    {
        imprimere("xar_mensura: ns/op, tempus processoris, optimum ex"
            " %d\n", (integer)MENSURAE);
        imprimere("  %-10s %9s %9s %9s %9s\n", "via", "elementa",
            "ns/op",
            "oct/elem", "alloc/el");
    }
    per (k = ZEPHYRUM; k < II; k++)
    {
            i32 n     = mensurae_n[k];
            i32 gyri  = (n <= IV * MXXIV) ? M : V;
        Fructus f     = _addere_metiri(n, gyri);

        si (machina)
        {
            imprimere("MENSURA\taddere\t%d\t%.3f\t%.2f\t%.4f\n",
                (integer)n, f.nanosecundae, f.octeti, f.allocationes);
        }
        alioquin
        {
            imprimere("  %-10s %9d %9.3f %9.2f %9.4f\n", "addere",
                (integer)n, f.nanosecundae, f.octeti, f.allocationes);
        }
        gyri = (n <= IV * MXXIV) ? (V * M) : XX;
        per (modus = ZEPHYRUM; modus < V; modus++)
        {
            f64 ns = _legere_metiri(modus, n, gyri);

            si (machina)
            {
                imprimere("MENSURA\t%s\t%d\t%.3f\t-\t-\n",
                    tituli[modus],
                    (integer)n, ns);
            }
            alioquin
            {
                imprimere("  %-10s %9d %9.3f\n", tituli[modus],
                    (integer)n,
                    ns);
            }
        }
    }
    redde ZEPHYRUM;
}
