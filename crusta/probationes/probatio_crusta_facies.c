/* probatio_crusta_facies.c - Facies diagnosticorum crustae
 * (project-specs/exemplaria-c-plan.md opus IV)
 *
 * REGULA VERA, non fixta: haec crusta/lintrum/ ipsum legit, ergo
 * porta regulam quam domus currit probat. Regula fixta hic probaret
 * extractorem solum, et divergentia inter regulam probatam et regulam
 * curritam invisibilis maneret.
 *
 * ASYMMETRIA EST PORTA. Fixturae inventum gradus I (clausura absens)
 * ET inventum gradus II ('-nt' nudum) simul ferunt, et excusatio
 * ALTERUM SOLUM tegit. Si ambo excusarentur, porta 'unum cadit,
 * alterum manet' discernere non posset - et facies quae gradum II
 * omnino omitteret viridis maneret.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_facies.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

/* gradus I (clausura '{' absens) + gradus II ('-nt' nudum) */
hic_manens constans character* FONS_UTERQUE =
    "#!/bin/bash\n"
    "[ $a -nt $b ]\n"
    "{ echo x\n";

/* idem, sed inventum gradus II SOLUM excusatum */
hic_manens constans character* FONS_EXCUSATUS =
    "#!/bin/bash\n"
    "# <tolera codex=\"lint:nt-aequalitas\" (>consulto\n"
    "[ $a -nt $b ]\n"
    "{ echo x\n";

/* '-nt' NEGATUM: bracchium auxiliare eum tollit, ergo inventum nullum
 * - et hoc subtractionem per regulam VERAM probat */
hic_manens constans character* FONS_NEGATUS =
    "#!/bin/bash\n"
    "! [ $a -nt $b ]\n";

/* -I = Xar abest: numerus ZEPHYRUM a 'nihil inventum' differt */
hic_manens s32
_quot (
       constans Xar* d,
 constans character* codex)
{
    i32 k;
    s32 summa = ZEPHYRUM;

    si (d == NIHIL)
    {
        redde (s32)-I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)xar_obtinere(d, k);

        si (strcmp(x->codex, codex) == ZEPHYRUM)
        {
            summa++;
        }
    }
    redde summa;
}

/* num codex ullus praefixum hoc ferat (gradus I crustae) */
hic_manens b32
_ullus_gradus_primi (
    constans Xar* d)
{
    i32 k;

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)xar_obtinere(d, k);

        si (strncmp(x->codex, "lint:", V) != ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

integer
principale (
    vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
 constans character* causa = NIHIL;

    piscina = piscina_generare_dynamicum("probatio_facies", 4194304);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    {
        Xar* d;

        imprimere("\n--- I. Gradus AMBO per faciem unam ---\n");
        d = crusta_diagnostica_omnia(piscina, FONS_UTERQUE,
                (i32)strlen(FONS_UTERQUE), NIHIL, &causa);
        si (causa != NIHIL)
        {
            imprimere("causa: %s\n", causa);
        }
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        /* 'optiones' NIHIL = regulae OMNES in crusta/lintrum/ */
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), (s32)I);
        CREDO_VERUM (_ullus_gradus_primi(d));
        /* bracchium auxiliare inventum non est */
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-negata"), ZEPHYRUM);
    }

    {
        chorda textus;

        imprimere("\n--- II. Gradus I: textus utrumque pingit ---\n");
        causa   = NIHIL;
        textus  = crusta_diagnostica_textus(piscina, "probatio.sh",
                      FONS_UTERQUE, (i32)strlen(FONS_UTERQUE), VERUM,
                      NIHIL, &causa);
        CREDO_NIHIL (causa);
        CREDO_MAIOR_I32 (textus.mensura, ZEPHYRUM);
        CREDO_CHORDA_CONTINET (textus,
            chorda_ex_literis("lint:nt-aequalitas", piscina));
        CREDO_CHORDA_CONTINET (textus,
            chorda_ex_literis("probatio.sh:", piscina));
        /* excerptum '^~~~' adest cum poscitur */
        CREDO_CHORDA_CONTINET (textus,
            chorda_ex_literis("^", piscina));
    }

    {
        Xar* d;

        imprimere("\n--- III. Excusatio gradum II SOLUM tollit ---\n");
        causa  = NIHIL;
        d      = crusta_diagnostica_omnia(piscina, FONS_EXCUSATUS,
                     (i32)strlen(FONS_EXCUSATUS), NIHIL, &causa);
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), ZEPHYRUM);
        /* gradus I MANET - asymmetria est porta */
        CREDO_VERUM (_ullus_gradus_primi(d));
    }

    {
        Xar* d;

        imprimere("\n--- IV. '-nt' negatum: subtractio ---\n");
        causa  = NIHIL;
        d      = crusta_diagnostica_omnia(piscina, FONS_NEGATUS,
                     (i32)strlen(FONS_NEGATUS), NIHIL, &causa);
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-negata"), ZEPHYRUM);
    }

    {
                    Xar* d;
         CrustaOptiones  optiones;

        imprimere("\n--- V. 'crudum': bracchia AMBO apparent ---\n");
        memset(&optiones, ZEPHYRUM, magnitudo(optiones));
        optiones.crudum  = VERUM;
        causa            = NIHIL;
        d = crusta_diagnostica_omnia(piscina, FONS_NEGATUS,
                (i32)strlen(FONS_NEGATUS), &optiones, &causa);
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        /* NUMERI DISPARES: crudum I et I, subtractum ZEPHYRUM et
         * ZEPHYRUM - fixtura cuius numeri aequales essent vexillum
         * probare non posset */
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), (s32)I);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-negata"), (s32)I);
    }

    {
                    Xar* d;
         CrustaOptiones  optiones;

        imprimere("\n--- VI. Lintrum absens REFUTATUR nominatim ---\n");
        memset(&optiones, ZEPHYRUM, magnitudo(optiones));
        optiones.lintrum  = "crusta/lintrum-quod-non-est";
        causa             = NIHIL;
        d = crusta_diagnostica_omnia(piscina, FONS_UTERQUE,
                (i32)strlen(FONS_UTERQUE), &optiones, &causa);
        /* SILENTIUM PERNICIES ESSET: lintrum absens inventa ZERO
         * redderet, quod a 'plagula sana' non differt */
        CREDO_NIHIL (d);
        CREDO_NON_NIHIL (causa);
        si (causa != NIHIL)
        {
            CREDO_VERUM (strstr(causa, "lintrum-quod-non-est")
                != NIHIL);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
