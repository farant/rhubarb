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

/* CASUS FRANIS - causa totius arcus clausurae. Commentum bash
 * sequentiam claudentem elementi SUI ferens. Ante gradum III
 * proiectio STML RECUSABATUR ('valor sequentiam claudentem fert'),
 * ergo plagula bash VALIDA lintrum frangebat (exitus I), et regulae
 * gradus II super eam currere omnino non poterant quia proiectione
 * carebant - vitium de causa sua TACENS. Scala fugae eam nunc fert
 * (gradus I-II), materia recusationem deposuit (gradus III). */
hic_manens constans character* FONS_CLAUSURA =
    "#!/bin/bash\n"
    "# vide </crusta-commentum> hic\n"
    "echo hi\n";

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

interior vacuum
_ambo_probare (
    CredoContextus* c)
{
               Piscina* piscina  = c->piscina;
    constans character* causa    = NIHIL;

    {
        Xar* d;

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
}

interior vacuum
_textum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
        chorda textus;

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
}

interior vacuum
_excusationem_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
        Xar* d;

        causa  = NIHIL;
        d      = crusta_diagnostica_omnia(piscina, FONS_EXCUSATUS,
                     (i32)strlen(FONS_EXCUSATUS), NIHIL, &causa);
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), ZEPHYRUM);
        /* gradus I MANET - asymmetria est porta */
        CREDO_VERUM (_ullus_gradus_primi(d));
    }
}

interior vacuum
_negatum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
        Xar* d;

        causa  = NIHIL;
        d      = crusta_diagnostica_omnia(piscina, FONS_NEGATUS,
                     (i32)strlen(FONS_NEGATUS), NIHIL, &causa);
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-aequalitas"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_quot(d, "lint:nt-negata"), ZEPHYRUM);
    }
}

interior vacuum
_crudum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
                    Xar* d;
         CrustaOptiones  optiones;

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
}

interior vacuum
_absentem_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
                    Xar* d;
         CrustaOptiones  optiones;

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
}

interior vacuum
_clausuram_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* causa;

    {
        Xar* d;

        causa  = NIHIL;
        d      = crusta_diagnostica_omnia(piscina, FONS_CLAUSURA,
                     (i32)strlen(FONS_CLAUSURA), NIHIL, &causa);
        si (causa != NIHIL)
        {
            imprimere("causa: %s\n", causa);
        }
        /* TRIA asserenda, non unum - casus VI supra causam docet:
         * 'inventa ZEPHYRUM' a 'nihil cucurrit' NON differt. Ergo
         * Xar ADESSE debet et causa ABESSE, et tunc demum numerus
         * ZEPHYRUM 'plagula munda' significat.
         *
         * QUID OLIM RUBEBAT - mensuratum planta, non coniectatum:
         * NON causa. Custode vetere reddito, causa NIHIL MANET et
         * Xar ADEST; inventum 'materia:scriptura' intus sedet, quia
         * scriptura fracta DIAGNOSTICUM fit, non defectus
         * instrumenti. Id ipsum Fran vidit:
         * 'c2.sh:3:1: [erratum] materia:scriptura'. Ergo NUMERUS
         * hanc portam tenet, non causa - quae tamen manet, quia
         * defectum instrumenti a plagula munda distinguit.
         *
         * Haec porta symptoma Franis ipsum tenet: non circuitum
         * octetorum (quem probatio_crusta_stml tenet) sed LINTRUM,
         * quod ille re vera fractum vidit. */
        CREDO_NON_NIHIL (d);
        CREDO_NIHIL (causa);
        si (d != NIHIL && xar_numerus(d) > ZEPHYRUM)
        {
            i32 k;

            /* NOMINARE, non solum numerare: pinna quae numerum dicit
             * infirmior est quam quae rem NOMINAT */
            per (k = ZEPHYRUM; k < xar_numerus(d); k++)
            {
                constans MateriaDiagnosticum* x =
                    (constans MateriaDiagnosticum*)xar_obtinere(d, k);

                imprimere("  INVENTUM %s\n", x->codex);
            }
        }
        CREDO_AEQUALIS_I32 (xar_numerus(d), ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. Gradus AMBO per faciem unam",
        _ambo_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. Gradus I: textus utrumque pingit",
        _textum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. Excusatio gradum II SOLUM tollit",
        _excusationem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV. '-nt' negatum: subtractio",
        _negatum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. 'crudum': bracchia AMBO apparent",
        _crudum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI. Lintrum absens REFUTATUR nominatim",
        _absentem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VII. Clausura propria: lintrum MUNDUM",
        _clausuram_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("crusta_facies", SECTIONES);
}
