#include "postulata_posix.h"
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "tabula_dispersa.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina;
        s32  sectio;

    sectio  = argc > I ? (s32)atoi(argv[I]) : ZEPHYRUM;
    piscina = piscina_generare_dynamicum("sonda", 65536);
    credo_aperire(piscina);

    /* D2: renovatio non crescit */
    si (sectio == ZEPHYRUM || sectio == II)
    {
        TabulaDispersa* tabula;
                chorda  claves[XIII];
             character  alveus[XVI];
                   i32  i;
                vacuum* valor;

        imprimere("\n--- Probans inserere (renovatio non allocat) ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);
        per (i = ZEPHYRUM; i < XIII; i++)
        {
            sprintf(alveus, "a%d", (s32)i);
            claves[i] = chorda_ex_literis(alveus, piscina);
            tabula_dispersa_inserere(tabula, claves[i], (vacuum*)(longus)i);
        }
        CREDO_AEQUALIS_I32(tabula->capacitas, XVI);

        /* 13/16 > 0.75, sed clavis iam adest: nulla crescentia */
        CREDO_VERUM(tabula_dispersa_inserere(tabula, claves[ZEPHYRUM],
                        (vacuum*)(longus)XCIX));
        CREDO_AEQUALIS_I32(tabula->capacitas, XVI);
        CREDO_AEQUALIS_I32(tabula->numerus, XIII);
        valor = NIHIL;
        CREDO_VERUM(tabula_dispersa_invenire(tabula, claves[ZEPHYRUM], &valor));
        CREDO_AEQUALIS_I32((i32)(longus)valor, XCIX);
    }

    /* D5: renovatio in iteratione */
    si (sectio == ZEPHYRUM || sectio == V)
    {
        TabulaDispersa* tabula;
        TabulaIterator  iter;
                chorda  clavis;
             character  alveus[XVI];
                   i32  visi[XIII];
                   i32  i;
                   i32  bis;
                   i32  numquam;
                vacuum* valor;

        imprimere("\n--- Probans iterator cum renovatione valorum ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);
        per (i = ZEPHYRUM; i < XIII; i++)
        {
            sprintf(alveus, "a%d", (s32)i);
            tabula_dispersa_inserere(tabula,
                chorda_ex_literis(alveus, piscina), (vacuum*)(longus)i);
            visi[i] = ZEPHYRUM;
        }

        iter = tabula_dispersa_iterator_initium(tabula);
        dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
        {
            visi[(i32)(longus)valor]++;
            /* renovatio pura: idem valor, clavis iam adest */
            tabula_dispersa_inserere(tabula, clavis, valor);
        }

        bis     = ZEPHYRUM;
        numquam = ZEPHYRUM;
        per (i = ZEPHYRUM; i < XIII; i++)
        {
            si (visi[i] > I)        bis++;
            si (visi[i] == ZEPHYRUM) numquam++;
        }
        CREDO_AEQUALIS_I32(bis, ZEPHYRUM);
        CREDO_AEQUALIS_I32(numquam, ZEPHYRUM);
    }

    /* D3: tumuli sine crescentia */
    si (sectio == ZEPHYRUM || sectio == III)
    {
        TabulaDispersa* tabula;
                chorda  clavis;
             character  alveus[XVI];
                   i32  i;

        imprimere("\n--- Probans tumulos: purgatio sine crescentia ---\n");

        tabula = tabula_dispersa_creare_chorda(piscina, XVI);
        per (i = ZEPHYRUM; i < M; i++)
        {
            sprintf(alveus, "t%d", (s32)i);
            clavis = chorda_ex_literis(alveus, piscina);
            CREDO_VERUM(tabula_dispersa_inserere(tabula, clavis,
                            (vacuum*)(longus)I));
            CREDO_VERUM(tabula_dispersa_delere(tabula, clavis));
        }
        CREDO_AEQUALIS_I32(tabula->numerus, ZEPHYRUM);
        CREDO_MINOR_AUT_AEQUALIS_I32(tabula->capacitas, XXXII);
    }

    /* D6: renovatio clavem quoque renovat */
    si (sectio == ZEPHYRUM || sectio == VI)
    {
        TabulaDispersa* tabula;
        TabulaIterator  iter;
                chorda  prima;
                chorda  secunda;
                chorda  servata;
                vacuum* valor;

        imprimere("\n--- Probans inserere (renovatio clavis mutuatae) ---\n");

        tabula  = tabula_dispersa_creare_chorda(piscina, XVI);
        prima   = chorda_ex_literis("beta", piscina);
        secunda = chorda_ex_literis("beta", piscina);
        CREDO_INAEQUALITAS_PTR(prima.datum, secunda.datum);

        tabula_dispersa_inserere(tabula, prima,   (vacuum*)(longus)I);
        tabula_dispersa_inserere(tabula, secunda, (vacuum*)(longus)II);
        CREDO_AEQUALIS_I32(tabula->numerus, I);

        iter = tabula_dispersa_iterator_initium(tabula);
        CREDO_VERUM(tabula_dispersa_iterator_proximum(&iter, &servata, &valor));
        CREDO_AEQUALIS_I32((i32)(longus)valor, II);
        CREDO_AEQUALIS_PTR(servata.datum, secunda.datum);
    }

    /* D7: capacitas impossibilis */
    si (sectio == ZEPHYRUM || sectio == VII)
    {
        imprimere("\n--- Probans creare cum capacitate impossibili ---\n");
        CREDO_NIHIL(tabula_dispersa_creare_chorda(piscina, (i32)0x80000001UL));
        CREDO_NIHIL(tabula_dispersa_creare_chorda(piscina, (i32)0xFFFFFFFFUL));
    }

    /* D1: fractura allocationis in piscina fixa */
    si (sectio == ZEPHYRUM || sectio == I)
    {
               Piscina* fixa;
        TabulaDispersa* tabula;
                chorda  claves[CXXVIII];
             character  alveus[XVI];
                   i32  i;
                   i32  inserti;
                   b32  fractura_visa;
                vacuum* valor;

        imprimere("\n--- Probans inserere in piscina fixa (fractura) ---\n");

        fixa   = piscina_generare_certae_magnitudinis("tabula_fixa", MMXLVIII);
        tabula = tabula_dispersa_creare_chorda(fixa, XVI);
        CREDO_NON_NIHIL(tabula);

        inserti       = ZEPHYRUM;
        fractura_visa = FALSUM;
        per (i = ZEPHYRUM; i < CXXVIII; i++)
        {
            sprintf(alveus, "k%d", (s32)i);
            claves[i] = chorda_ex_literis(alveus, piscina);
            si (!tabula_dispersa_inserere(tabula, claves[i],
                    (vacuum*)(longus)(i + I)))
            {
                fractura_visa = VERUM;
                frange;
            }
            inserti++;
        }

        CREDO_VERUM(fractura_visa);
        si (fractura_visa)
        {
            CREDO_AEQUALIS_I32(tabula->numerus, inserti);
            CREDO_FALSUM(tabula_dispersa_continet(tabula, claves[inserti]));
            per (i = ZEPHYRUM; i < inserti; i++)
            {
                valor = NIHIL;
                CREDO_VERUM(tabula_dispersa_invenire(tabula, claves[i], &valor));
                CREDO_AEQUALIS_I32((i32)(longus)valor, i + I);
            }
        }
        piscina_destruere(fixa);
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
