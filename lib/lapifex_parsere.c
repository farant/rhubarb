/* lapifex_parsere.c - Implementatio Motoris Parsationis LR(1)
 *
 * Shift-reduce parser usans tabulam ACTION/GOTO.
 */

#include "lapifex_parsere.h"
#include "xar.h"

/* ================================================
 * Elementum Acervi (internum)
 * ================================================ */

nomen structura {
    s32  status;
    s64  valor;
} LapifexAcervusElementum;

/* ================================================
 * Parsare - Motor Principalis
 * ================================================ */

LapifexParsaturaFructus
lapifex_parsare(
    LapifexTabula*           tabula,
    constans LapifexSignum*  signa,
    s32                      numerus,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus)
{
    LapifexParsaturaFructus  fructus;
    Xar*                     acervus;
    LapifexAcervusElementum* elem;
    s32                      positio;

    /* Initializare fructum */
    fructus.valor = 0;
    fructus.successus = FALSUM;
    fructus.error.status = 0;
    fructus.error.terminalis = 0;
    fructus.error.positio = 0;
    fructus.error.est_error = FALSUM;

    si (!tabula)
    {
        fructus.error.est_error = VERUM;
        redde fructus;
    }

    /* Creare acervum */
    acervus = xar_creare(tabula->grammatica->piscina,
        (i32)magnitudo(LapifexAcervusElementum));

    /* Impellere statum initialem (0, 0) */
    elem = (LapifexAcervusElementum*)xar_addere(acervus);
    elem->status = ZEPHYRUM;
    elem->valor = 0;

    positio = ZEPHYRUM;

    /* Ansa principalis */
    per (;;)
    {
        LapifexAcervusElementum* culmen;
        s32                      status_currens;
        s32                      terminalis;
        Xar*                     actiones;
        i32                      num_actiones;
        LapifexActioIntroitus*   actio;

        /* Obtinere culmen acervi */
        culmen = (LapifexAcervusElementum*)xar_obtinere(
            acervus, (i32)xar_numerus(acervus) - I);
        si (!culmen) frange;

        status_currens = culmen->status;

        /* Determinare terminalem currentem */
        si (positio < numerus)
        {
            terminalis = signa[positio].terminalis;
        }
        alioquin
        {
            terminalis = -I;  /* EOF / $ */
        }

        /* Quaerere actiones */
        actiones = lapifex_actiones_quaerere(tabula, status_currens, terminalis);
        num_actiones = actiones ? (i32)xar_numerus(actiones) : ZEPHYRUM;

        si (num_actiones == ZEPHYRUM)
        {
            /* Error: nulla actio */
            fructus.error.est_error = VERUM;
            fructus.error.status = status_currens;
            fructus.error.terminalis = terminalis;
            fructus.error.positio = positio;
            redde fructus;
        }

        si (num_actiones > I)
        {
            /* Conflictus: GLR necessarius (Phase 5) */
            fructus.error.est_error = VERUM;
            fructus.error.status = status_currens;
            fructus.error.terminalis = terminalis;
            fructus.error.positio = positio;
            redde fructus;
        }

        actio = (LapifexActioIntroitus*)xar_obtinere(actiones, ZEPHYRUM);
        si (!actio) frange;

        commutatio (actio->actio)
        {
            casus LAPIFEX_ACTIO_TRANSPONERE:
            {
                /* SHIFT: impellere statum novum, consumere token */
                LapifexAcervusElementum* novum;
                s64 val;

                val = (positio < numerus) ? signa[positio].valor : 0;

                novum = (LapifexAcervusElementum*)xar_addere(acervus);
                novum->status = actio->valor;
                novum->valor = val;
                positio++;
                frange;
            }

            casus LAPIFEX_ACTIO_REDUCERE:
            {
                /* REDUCE: applicare productionem */
                LapifexProductio*        prod;
                i32                      num_dextrum;
                s64                      valori[LAPIFEX_MAXIMUS_DEXTRUM];
                i32                      i;
                s64                      valor_novus;
                LapifexAcervusElementum* post_culmen;
                s32                      goto_status;
                LapifexAcervusElementum* novum;

                prod = (LapifexProductio*)xar_obtinere(
                    tabula->grammatica->productiones,
                    (i32)actio->valor);
                si (!prod) frange;

                num_dextrum = (i32)xar_numerus(prod->dextrum);

                /* Colligere valori ex culmine acervi */
                per (i = ZEPHYRUM; i < num_dextrum && i < LAPIFEX_MAXIMUS_DEXTRUM; i++)
                {
                    LapifexAcervusElementum* e;
                    i32 idx;

                    idx = (i32)xar_numerus(acervus) - num_dextrum + i;
                    e = (LapifexAcervusElementum*)xar_obtinere(acervus, idx);
                    valori[i] = e ? e->valor : 0;
                }

                /* Removere num_dextrum elementa */
                per (i = ZEPHYRUM; i < num_dextrum; i++)
                {
                    xar_removere_ultimum(acervus);
                }

                /* Applicare callback reductionis */
                si (reductio)
                {
                    valor_novus = reductio(
                        prod->index, valori, (s32)num_dextrum, contextus);
                }
                alioquin
                {
                    valor_novus = 0;
                }

                /* GOTO: invenire statum novum */
                post_culmen = (LapifexAcervusElementum*)xar_obtinere(
                    acervus, (i32)xar_numerus(acervus) - I);
                si (!post_culmen)
                {
                    fructus.error.est_error = VERUM;
                    fructus.error.status = status_currens;
                    fructus.error.terminalis = terminalis;
                    fructus.error.positio = positio;
                    redde fructus;
                }

                goto_status = lapifex_goto_quaerere(
                    tabula, post_culmen->status, prod->sinistrum);

                si (goto_status < ZEPHYRUM)
                {
                    fructus.error.est_error = VERUM;
                    fructus.error.status = post_culmen->status;
                    fructus.error.terminalis = terminalis;
                    fructus.error.positio = positio;
                    redde fructus;
                }

                /* Impellere statum novum */
                novum = (LapifexAcervusElementum*)xar_addere(acervus);
                novum->status = goto_status;
                novum->valor = valor_novus;
                frange;
            }

            casus LAPIFEX_ACTIO_ACCIPERE:
            {
                /* ACCEPT: parsura completa */
                LapifexAcervusElementum* fructus_elem;

                /* Valor finalis est in acervus[1] */
                fructus_elem = (LapifexAcervusElementum*)xar_obtinere(
                    acervus, I);

                fructus.successus = VERUM;
                fructus.valor = fructus_elem ? fructus_elem->valor : 0;
                redde fructus;
            }

            ordinarius:
            {
                /* Error ignotus */
                fructus.error.est_error = VERUM;
                fructus.error.status = status_currens;
                fructus.error.terminalis = terminalis;
                fructus.error.positio = positio;
                redde fructus;
            }
        }
    }

    /* Non debemus huc pervenire */
    fructus.error.est_error = VERUM;
    redde fructus;
}
