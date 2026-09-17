/* probatio_crusta_differentia.c - Porta sanitatis contra 'bash -n'
 * (P11b)
 *
 * (a) Quisque casus oraculi cum sectione 'status' in auro sanitas.txt:
 *     casus crudus (non involutus) parsatur; verdictum nostrum
 *     relatio.sana (mala nulla, clausurae omnes) contra 'bash -n -c'
 *     status == 0. CONCORDES pinnati ASCENDENTES; discordes SEMPER
 *     nominantur cum verdictis ambobus - divergentiae notae (quaestio
 *     01M2NT4CDD): bash monet sed exitus 0 (heredoc ad EOF, '[[ a b
 *     ]]', arithmetica tempore parsurae non iudicata).
 * (b) Corpus domus: omnis plagula build/crusta_corpus.lst (cursor eam
 *     scribit) praeter fixa pathologiae.sh et adversarius.sh
 *     parsatur, mala summantur - pinna DESCENDENS tantum, finis
 *     ZEPHYRUM.
 *
 * Mensura sui: casus, aura et plagulae domus numerantur et pinnantur.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CASUS_NUMERUS      121
#define AURA_NUMERUS       121
#define PINNA_CONCORDIUM   118
#define MALA_DOMUS_PINNA   0
#define DOMUS_MINIMUM      200

interior i32
_status (
    chorda textus)
{
    i32 k;
    i32 v = ZEPHYRUM;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] < '0' || textus.datum[k] > '9')
        {
            redde (i32)CCLV;
        }
        v = v * (i32)X + (i32)(textus.datum[k] - '0');
    }
    redde textus.mensura > ZEPHYRUM ? v : (i32)CCLV;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix = getenv("RHUBARB_RADIX");
    constans character* causa;
                   Xar* collectio;
                   Xar* aurum;
             character* fons;
             character* lista;
                   i32  mensura    = ZEPHYRUM;
                   i32  concordes  = ZEPHYRUM;
                   i32  discordes  = ZEPHYRUM;
                   i32  sine_auro  = ZEPHYRUM;
                   i32  plagulae   = ZEPHYRUM;
                   i32  mala       = ZEPHYRUM;
                   i32  k;

    piscina = piscina_generare_dynamicum("probatio_crusta_differentia",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }


    /* ==================================================
     * PROBARE: sanitas contra 'bash -n'
     * ================================================== */

    imprimere("\n--- Probans sanitatem contra aurum bash -n ---\n");

    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    fons = crusta_oraculum_legere(piscina, radix,
        CRUSTA_ORACULUM_SANITAS, &mensura);
    si (collectio == NIHIL || fons == NIHIL)
    {
        CREDO_CULPA (collectio == NIHIL ? "casus oraculi non lecti"
            : "aurum sanitas.txt absens - ./crusta/oraculum.sh "
              "-scribere");
        credo_imprimere_compendium();
        redde I;
    }
    aurum = crusta_exempla_legere(piscina, fons, mensura);
    CREDO_NON_NIHIL (aurum);
    si (aurum == NIHIL)
    {
        redde I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(collectio); k++)
    {
        constans CrustaCasusOraculi* c =
            (constans CrustaCasusOraculi*)xar_obtinere(collectio, k);
                              chorda status;
                              chorda erratum;
                       CrustaParsura relatio;
                       MateriaNodus* arbor;
                                 b32  accepta;
                                 b32  sana;

        status = crusta_oraculum_aurum(aurum, c->clavis, "status");
        si (status.datum == NIHIL)
        {
            sine_auro++;
            perge;
        }
        erratum = crusta_oraculum_aurum(aurum, c->clavis, "ERRATUM");
        arbor   = crusta_arbor_parsare(piscina,
            (constans character*)c->datum.datum, c->datum.mensura,
            &CRUSTA_BASH, &relatio);
        accepta  = _status(status) == ZEPHYRUM ? VERUM : FALSUM;
        sana     = (arbor != NIHIL && relatio.sana) ? VERUM : FALSUM;
        si (accepta == sana)
        {
            concordes++;
        }
        alioquin
        {
            discordes++;
            imprimere("    discors %.*s: bash status %.*s ('%.*s'), "
                "crusta sana %d (mala %d, clausurae absentes %d)\n",
                (integer)c->clavis.mensura,
                (constans character*)c->clavis.datum,
                (integer)status.mensura,
                (constans character*)status.datum,
                (integer)erratum.mensura,
                erratum.datum != NIHIL
                    ? (constans character*)erratum.datum : "",
                (integer)sana, (integer)relatio.mala,
                (integer)relatio.clausurae_absentes);
        }
    }
    imprimere("    casus %d, aura %d: concordes %d, discordes %d, sine "
        "auro %d (pinna concordium %d)\n",
        (integer)xar_numerus(collectio), (integer)xar_numerus(aurum),
        (integer)concordes, (integer)discordes, (integer)sine_auro,
        (integer)PINNA_CONCORDIUM);
    si (concordes > (i32)PINNA_CONCORDIUM)
    {
        imprimere("    NOTA: concordes %d > pinna %d - pinnam "
            "attolle\n",
            (integer)concordes, (integer)PINNA_CONCORDIUM);
    }
    CREDO_AEQUALIS_I32 (xar_numerus(collectio), (i32)CASUS_NUMERUS);
    CREDO_AEQUALIS_I32 (xar_numerus(aurum), (i32)AURA_NUMERUS);
    CREDO_AEQUALIS_I32 (concordes + discordes, (i32)AURA_NUMERUS);
    CREDO_VERUM (concordes >= (i32)PINNA_CONCORDIUM);


    /* ==================================================
     * PROBARE: mala corporis domus
     * ================================================== */

    imprimere("\n--- Probans mala corporis domus ---\n");

    lista = crusta_oraculum_legere(piscina, radix,
        "build/crusta_corpus.lst", &mensura);
    si (lista == NIHIL)
    {
        CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam "
            "scribit");
    }
    alioquin
    {
        i32 ab = ZEPHYRUM;

        dum (ab < mensura)
        {
                  i32  ad = ab;
            character  via[1024];
              Piscina* propria;
            character* textus;
                  i32  longitudo = ZEPHYRUM;
        CrustaParsura  relatio;

            dum (ad < mensura && lista[ad] != '\n')
            {
                ad++;
            }
            si (ad == ab || ad - ab >= (i32)magnitudo(via))
            {
                ab = ad + I;
                perge;
            }
            memcpy(via, lista + ab, (size_t)(ad - ab));
            via[ad - ab]  = '\0';
            ab            = ad + I;
            /* fixa pathologica mala sua habent (porta corporis
             * eadem regula eas excludit; exempla et reservatio eas
             * pinnant) */
            si (   strcmp(via, "probationes/fixa/crusta/pathologiae.sh")
                    == ZEPHYRUM
                || strcmp(via, "probationes/fixa/crusta/adversarius.sh")
                    == ZEPHYRUM)
            {
                perge;
            }
            propria = piscina_generare_dynamicum("differentia_domus",
                1048576);
            si (propria == NIHIL)
            {
                CREDO_CULPA ("piscina domus");
                frange;
            }
            textus = crusta_oraculum_legere(propria, radix, via,
                &longitudo);
            si (textus == NIHIL)
            {
                imprimere("    plagula absens: %s\n", via);
                CREDO_CULPA ("plagula corporis absens");
                piscina_destruere(propria);
                perge;
            }
            plagulae++;
            si (   crusta_arbor_parsare(propria, textus, longitudo,
                    &CRUSTA_BASH, &relatio) != NIHIL
                && relatio.mala > ZEPHYRUM)
            {
                mala += relatio.mala;
                imprimere("    %s: mala %d\n", via,
                    (integer)relatio.mala);
            }
            piscina_destruere(propria);
        }
    }
    imprimere("    plagulae domus %d: mala %d (pinna %d, descendens)\n",
        (integer)plagulae, (integer)mala, (integer)MALA_DOMUS_PINNA);
    CREDO_VERUM (plagulae >= (i32)DOMUS_MINIMUM);
    CREDO_VERUM (mala <= (i32)MALA_DOMUS_PINNA);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
