#include "latina.h"
#include "numerus_romanus.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* PROBARE: legere respuit formas non canonicas (grex subtractivus post
 * gregem minorem: VIX = XIV, DCM = MCD) */
interior vacuum
_sectio_non_canonica (
    CredoContextus* c)
{
                Piscina* p = c->piscina;
    hic_manens constans character* vitiosa[] = {
        "VIX", "VIV", "LXC", "LXL", "DCM", "DCD", "MDCM", "XCVIV",
        "MVIV", "vix", "viv", "lxc", "dcm", NIHIL
    };
         i32 k;

    per (k = ZEPHYRUM; vitiosa[k] != NIHIL; k++)
    {
        i32 valor = DCCLXXVII;

        si (numerus_romanus_legere(chorda_ex_literis(vitiosa[k], p),
                                   &valor))
        {
            imprimere("  acceptum non canonicum: %s -> %u\n",
                      vitiosa[k], valor);
        }
        CREDO_FALSUM(numerus_romanus_legere(
            chorda_ex_literis(vitiosa[k], p), NIHIL));
    }
    piscina_vacare(p);
}

/* PROBARE: legere nihil ultra MMMCMXCIX reddit (scribere(4400) vacuum) */
interior vacuum
_sectio_ultra_maximum (
    CredoContextus* c)
{
                Piscina* p = c->piscina;
    hic_manens constans character* ultra[] = {
        "MMMDCM", "MMMCMLXC", "MMMDCMLXCVIX", "mmmdcmlxcvix",
        "MMMDCMLXCVIV", NIHIL
    };
         i32 k;

    per (k = ZEPHYRUM; ultra[k] != NIHIL; k++)
    {
        i32 valor = ZEPHYRUM;
        b32 r     = numerus_romanus_legere(
            chorda_ex_literis(ultra[k], p), &valor);

        CREDO_VERUM(!r || valor <= MMM + CM + XC + IX);
        CREDO_FALSUM(r);
    }
    piscina_vacare(p);
}

/* PROBARE: legere acceptum == canonicum, omnes chordae IVXLCDM
 * longitudinis I-VI (s acceptum => scribere(valor) == s) */
interior vacuum
_sectio_exhaustiva (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
    hic_manens constans character litterae[VII] = {
        'I', 'V', 'X', 'L', 'C', 'D', 'M'
    };
       character buf[VI];
             i32 indices[VI];
             i32 longitudo;
             i32 k;
             i32 mendae = ZEPHYRUM;

    per (longitudo = I; longitudo <= VI; longitudo++)
    {
        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            indices[k] = ZEPHYRUM;
        }
        dum (VERUM)
        {
            chorda s;
               i32 valor = ZEPHYRUM;
               s32 j;

            per (k = ZEPHYRUM; k < longitudo; k++)
            {
                buf[k] = litterae[indices[k]];
            }
            s.datum    = (i8*)buf;
            s.mensura  = longitudo;
            si (numerus_romanus_legere(s, &valor))
            {
                chorda retro = numerus_romanus_scribere(valor, p);

                si (!chorda_aequalis(retro, s))
                {
                    si (mendae < V)
                    {
                        imprimere("  %.*s -> %u non canonicum\n",
                                  (integer)longitudo, buf, valor);
                    }
                    mendae++;
                }
                piscina_vacare(p);
            }
            per (j = (s32)longitudo - I; j >= ZEPHYRUM; j--)
            {
                si (++indices[j] < VII)
                {
                    frange;
                }
                indices[j] = ZEPHYRUM;
            }
            si (j < ZEPHYRUM)
            {
                frange;
            }
        }
    }
    CREDO_VERUM(mendae == ZEPHYRUM);
}

/* PROBARE: exprimere - summa terminorum int excedens (i64) fert
 * (2147483649: omnis terminus int capit, summa non) */
interior vacuum
_sectio_summa_lata (
    CredoContextus* c)
{
     Piscina* p = c->piscina;
         i64  n = (i64)MMCXLVII * M * M + (i64)CDLXXXIII * M + DCXLIX;
       chorda e = numerus_romanus_exprimere(n, NIHIL, p);

    imprimere("  %.*s\n", (integer)e.mensura, (constans character*)e.datum);
    CREDO_VERUM(chorda_aequalis_literis(e,
        "(i64)MMCXLVII * M * M + CDLXXXIII * M + DCXLIX"));
    piscina_vacare(p);
}

/* PROBARE: exprimere - factor binarius iam multiplicatus parentheses
 * non iterat */
interior vacuum
_sectio_parentheses (
    CredoContextus* c)
{
     Piscina* p = c->piscina;
         i64  n = ((i64)IV * M + XCVII) * MXXIV * MXXIV;
       chorda e = numerus_romanus_exprimere(n, NIHIL, p);

    imprimere("  %.*s\n", (integer)e.mensura, (constans character*)e.datum);
    CREDO_VERUM(chorda_aequalis_literis(e,
        "(i64)(IV * M + XCVII) * MXXIV * MXXIV"));
    piscina_vacare(p);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere non canonica", _sectio_non_canonica, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere ultra maximum", _sectio_ultra_maximum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere exhaustiva", _sectio_exhaustiva, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exprimere summa lata", _sectio_summa_lata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exprimere parentheses", _sectio_parentheses, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("numerus_romanus_drafts", SECTIONES);
}
