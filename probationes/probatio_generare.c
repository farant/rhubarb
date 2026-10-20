/* probatio_generare.c - Probationes pro generare directivis */

/* <aedilis instrumentum="bin/generare"/> */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>

/* instrumenta ficta in build/, numquam iuxta binaria installata in
 * bin/ (fabrica plan 5 T5b: scriptura extra vestigium portae) */
#define INSTRUMENTA "build/probatio_generare_instrumenta"
#define GENERARE    "./bin/generare --instrumenta " INSTRUMENTA " "


/* ========================================================================
 * Reproducere logicam extractionis directivarum ex generare.c
 * (Functiones interiores non accessibiles ex alio filo)
 * ======================================================================== */

#define GENERARE_PRAEFIXUM_MENSURA  XIII
#define GENERARE_SUFFIXUM_MENSURA   II

nomen structura {
    chorda  instrumentum;
       Xar* argumenta;
       s32  versus;
} ProbatioDirectiva;

interior s32
_computare_lineas (
    chorda contentum,
       i32 usque_ad)
{
    s32 numerus;
    i32 i;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < usque_ad && i < contentum.mensura; i++)
    {
        si (contentum.datum[i] == '\n')
        {
            numerus++;
        }
    }

    redde numerus;
}

interior Xar*
_extrahere_directivas (
     chorda  contentum,
    Piscina* piscina)
{
        Xar* directivae;
     chorda  praefixum;
     chorda  suffixum;
        s32  positio;
     chorda  reliquum;
        s32  versus_currens;
         i8  _praefixum_buf[] = {
        '/', '*', ' ', 'g', 'e', 'n', 'e', 'r', 'a', 'r', 'e', ':', ' '
    };
         i8 _suffixum_buf[] = { '*', '/' };

    directivae = xar_creare(piscina, (i32)magnitudo(ProbatioDirectiva));
    praefixum = chorda_ex_buffer(_praefixum_buf,
        GENERARE_PRAEFIXUM_MENSURA);
    suffixum = chorda_ex_buffer(_suffixum_buf,
        GENERARE_SUFFIXUM_MENSURA);

    reliquum        = contentum;
    versus_currens  = I;

    dum (reliquum.mensura > ZEPHYRUM)
    {
                   chorda  inventum;
                      s32  finis_idx;
                   chorda  corpus;
                   chorda  praecisa;
        ProbatioDirectiva* dir;
                      i32  idx;
                      i32  signi_initium;
                      s32  lineas_ante;

        positio = chorda_invenire_index(reliquum, praefixum);
        si (positio < ZEPHYRUM)
        {
            frange;
        }

        lineas_ante     = _computare_lineas(reliquum, (i32)positio);
        versus_currens  += lineas_ante;

        inventum = chorda_sectio(reliquum,
                                 (i32)positio
                                     + GENERARE_PRAEFIXUM_MENSURA,
                                 reliquum.mensura);

        finis_idx = chorda_invenire_index(inventum, suffixum);
        si (finis_idx < ZEPHYRUM)
        {
            frange;
        }

        corpus    = chorda_sectio(inventum, ZEPHYRUM, (i32)finis_idx);
        praecisa  = chorda_praecidere(corpus);

        si (praecisa.mensura > ZEPHYRUM)
        {
            dir = (ProbatioDirectiva*)xar_addere(directivae);
            si (dir != NIHIL)
            {
                dir->argumenta = xar_creare(piscina,
                    (i32)magnitudo(chorda));
                dir->instrumentum.datum    = NIHIL;
                dir->instrumentum.mensura  = ZEPHYRUM;
                dir->versus                = versus_currens;

                idx = ZEPHYRUM;
                dum (idx < praecisa.mensura)
                {
                    dum (   idx < praecisa.mensura
                         && (praecisa.datum[idx] == ' '
                        || praecisa.datum[idx] == '\t'
                        || praecisa.datum[idx] == '\n'
                        || praecisa.datum[idx] == '\r'))
                    {
                        idx++;
                    }

                    si (idx >= praecisa.mensura)
                    {
                        frange;
                    }

                    signi_initium = idx;
                    dum (   idx < praecisa.mensura
                         && praecisa.datum[idx] != ' '
                         && praecisa.datum[idx] != '\t'
                         && praecisa.datum[idx] != '\n'
                         && praecisa.datum[idx] != '\r')
                    {
                        idx++;
                    }

                    {
                        chorda signum;
                        signum = chorda_sectio(praecisa, signi_initium,
                            idx);

                        si (dir->instrumentum.datum == NIHIL)
                        {
                            dir->instrumentum =
                                chorda_transcribere(signum, piscina);
                        }
                        alioquin
                        {
                            chorda* arg;
                            arg = (chorda*)xar_addere(dir->argumenta);
                            si (arg != NIHIL)
                            {
                                *arg = chorda_transcribere(signum,
                                    piscina);
                            }
                        }
                    }
                }
            }
        }

        {
            i32 finis_directivae;
            finis_directivae = (i32)positio + GENERARE_PRAEFIXUM_MENSURA
                + (i32)finis_idx
                                   + GENERARE_SUFFIXUM_MENSURA;
            versus_currens += _computare_lineas(
                chorda_sectio(reliquum, (i32)positio, finis_directivae),
                finis_directivae - (i32)positio);
            reliquum = chorda_sectio(reliquum, finis_directivae,
                reliquum.mensura);
        }
    }

    redde directivae;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */


    /* ========================================================
     * PROBARE: Una directiva simplex
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;
                   chorda* arg;

        contentum = chorda_ex_literis(
            "/* generare: meum_instrumentum arg1 arg2 */\n"
            "int main(void) { return 0; }\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), I);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir->instrumentum,
            "meum_instrumentum");
        CREDO_AEQUALIS_I32(xar_numerus(dir->argumenta), II);

        arg = (chorda*)xar_obtinere(dir->argumenta, ZEPHYRUM);
        CREDO_NON_NIHIL(arg);
        CREDO_CHORDA_AEQUALIS_LITERIS(*arg, "arg1");

        arg = (chorda*)xar_obtinere(dir->argumenta, I);
        CREDO_NON_NIHIL(arg);
        CREDO_CHORDA_AEQUALIS_LITERIS(*arg, "arg2");
    }
}


    /* ========================================================
     * PROBARE: Multiplices directivae
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir0;
        ProbatioDirectiva* dir1;

        contentum = chorda_ex_literis(
            "/* generare: primum a b */\n"
            "int foo(void);\n"
            "/* generare: secundum x y z */\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), II);

        dir0 = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir0);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir0->instrumentum, "primum");
        CREDO_AEQUALIS_I32(xar_numerus(dir0->argumenta), II);

        dir1 = (ProbatioDirectiva*)xar_obtinere(directivae, I);
        CREDO_NON_NIHIL(dir1);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir1->instrumentum, "secundum");
        CREDO_AEQUALIS_I32(xar_numerus(dir1->argumenta), III);
    }
}


    /* ========================================================
     * PROBARE: Nullae directivae
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda  contentum;
           Xar* directivae;

        contentum = chorda_ex_literis(
            "/* Hoc est commentarium normale */\n"
            "int main(void) { return 0; }\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), ZEPHYRUM);
    }
}


    /* ========================================================
     * PROBARE: Directiva cum multis argumentis
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;
                   chorda* arg;

        contentum = chorda_ex_literis(
            "/* generare: nuntium_schema_generare persona.nuntium gen.h gen.c */\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), I);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir->instrumentum,
            "nuntium_schema_generare");
        CREDO_AEQUALIS_I32(xar_numerus(dir->argumenta), III);

        arg = (chorda*)xar_obtinere(dir->argumenta, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(*arg, "persona.nuntium");

        arg = (chorda*)xar_obtinere(dir->argumenta, I);
        CREDO_CHORDA_AEQUALIS_LITERIS(*arg, "gen.h");

        arg = (chorda*)xar_obtinere(dir->argumenta, II);
        CREDO_CHORDA_AEQUALIS_LITERIS(*arg, "gen.c");
    }
}


    /* ========================================================
     * PROBARE: Spatium album extra
     * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;

        contentum = chorda_ex_literis(
            "/* generare:   instrumentum   arg1   arg2   */\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), I);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir->instrumentum,
            "instrumentum");
        CREDO_AEQUALIS_I32(xar_numerus(dir->argumenta), II);
    }
}


    /* ========================================================
     * PROBARE: Directiva sine argumentis (solum instrumentum)
     * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;

        contentum = chorda_ex_literis(
            "/* generare: solum_instrumentum */\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), I);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir->instrumentum,
            "solum_instrumentum");
        CREDO_AEQUALIS_I32(xar_numerus(dir->argumenta), ZEPHYRUM);
    }
}


    /* ========================================================
     * PROBARE: Commentarium similare sed non directiva
     * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda  contentum;
           Xar* directivae;

        contentum = chorda_ex_literis(
            "/* generari: non_est_directiva */\n"
            "/* generate: non_est_directiva */\n"
            "/* gen: non_est_directiva */\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), ZEPHYRUM);
    }
}


    /* ========================================================
     * PROBARE: Directiva in medio fili
     * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;

        contentum = chorda_ex_literis(
            "#include <stdio.h>\n"
            "\n"
            "/* Commentarium normale */\n"
            "/* generare: medio arg1 */\n"
            "\n"
            "int main(void) { return 0; }\n",
            piscina);

        directivae = _extrahere_directivas(contentum, piscina);

        CREDO_AEQUALIS_I32(xar_numerus(directivae), I);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_CHORDA_AEQUALIS_LITERIS(dir->instrumentum, "medio");
    }
}


    /* ========================================================
     * PROBARE: Numeri versuum
     * ======================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                   chorda  contentum;
                      Xar* directivae;
        ProbatioDirectiva* dir;

        /* Directiva in linea 1 */
        contentum = chorda_ex_literis(
            "/* generare: primum */\n",
            piscina);
        directivae = _extrahere_directivas(contentum, piscina);
        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_AEQUALIS_S32(dir->versus, I);

        /* Directiva in linea 4 */
        contentum = chorda_ex_literis(
            "#include <stdio.h>\n"
            "\n"
            "/* commentarium */\n"
            "/* generare: quartum */\n",
            piscina);
        directivae = _extrahere_directivas(contentum, piscina);
        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_NON_NIHIL(dir);
        CREDO_AEQUALIS_S32(dir->versus, IV);

        /* Duae directivae: linea 2 et linea 5 */
        contentum = chorda_ex_literis(
            "\n"
            "/* generare: alpha */\n"
            "int x;\n"
            "\n"
            "/* generare: beta */\n",
            piscina);
        directivae = _extrahere_directivas(contentum, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(directivae), II);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, ZEPHYRUM);
        CREDO_AEQUALIS_S32(dir->versus, II);

        dir = (ProbatioDirectiva*)xar_obtinere(directivae, I);
        CREDO_AEQUALIS_S32(dir->versus, V);
    }
}


    /* ========================================================
     * PROBARE: End-to-end - variabiles ambitus
     * ======================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           b32 scriptum;
           s32 codex;
        chorda indicator_contentum;

        /* Scribere filum .c cum directiva in linea 3 */
        scriptum = filum_scribere_literis(
            "/tmp/probatio_generare_env.c",
            "#include <stdio.h>\n"
            "\n"
            "/* generare: env_probatio */\n"
            "int main(void) { return 0; }\n");
        CREDO_VERUM(scriptum);

        /* Creare scriptum quod scribit variabiles ambitus ad filum */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(INSTRUMENTA));
        scriptum = filum_scribere_literis(
            INSTRUMENTA "/env_probatio",
            "#!/bin/sh\n"
            "echo \"FILUM=$GENERARE_FILUM\" > /tmp/probatio_generare_env_out\n"
            "echo \"VERSUS=$GENERARE_VERSUS\" >> /tmp/probatio_generare_env_out\n");
        CREDO_VERUM(scriptum);

        codex = system("chmod +x " INSTRUMENTA "/env_probatio");
        CREDO_AEQUALIS_S32(codex, ZEPHYRUM);

        filum_delere("/tmp/probatio_generare_env_out");

        codex = system(GENERARE "/tmp/probatio_generare_env.c");
        CREDO_AEQUALIS_S32(codex, ZEPHYRUM);

        /* Legere et verificare output */
        CREDO_VERUM(filum_existit("/tmp/probatio_generare_env_out"));
        indicator_contentum = filum_legere_totum(
            "/tmp/probatio_generare_env_out", piscina);
        CREDO_VERUM(indicator_contentum.mensura > ZEPHYRUM);

        /* Verificare GENERARE_FILUM */
        {
            chorda filum_acus;
            filum_acus = chorda_ex_literis(
                "FILUM=/tmp/probatio_generare_env.c", piscina);
            CREDO_VERUM(chorda_continet(indicator_contentum,
                filum_acus));
        }

        /* Verificare GENERARE_VERSUS=3 */
        {
            chorda versus_acus;
            versus_acus = chorda_ex_literis("VERSUS=3", piscina);
            CREDO_VERUM(chorda_continet(indicator_contentum,
                versus_acus));
        }

        /* Purgare */
        filum_delere("/tmp/probatio_generare_env.c");
        filum_delere("/tmp/probatio_generare_env_out");
        filum_delere(INSTRUMENTA "/env_probatio");
    }
}


    /* ========================================================
     * PROBARE: End-to-end - scribere filum, currere generare
     * ======================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 scriptum;
        s32 codex;

        scriptum = filum_scribere_literis(
            "/tmp/probatio_generare_test.c",
            "/* generare: echo_test */\n"
            "int main(void) { return 0; }\n");
        CREDO_VERUM(scriptum);

        scriptum = filum_scribere_literis(
            INSTRUMENTA "/echo_test",
            "#!/bin/sh\ntouch /tmp/probatio_generare_indicator\n");
        CREDO_VERUM(scriptum);

        codex = system("chmod +x " INSTRUMENTA "/echo_test");
        CREDO_AEQUALIS_S32(codex, ZEPHYRUM);

        filum_delere("/tmp/probatio_generare_indicator");

        codex = system(GENERARE "/tmp/probatio_generare_test.c");
        CREDO_AEQUALIS_S32(codex, ZEPHYRUM);

        CREDO_VERUM(filum_existit("/tmp/probatio_generare_indicator"));

        filum_delere("/tmp/probatio_generare_test.c");
        filum_delere("/tmp/probatio_generare_indicator");
        filum_delere(INSTRUMENTA "/echo_test");
    }
}


    /* ========================================================
     * PROBARE: End-to-end - error terminat
     * ======================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 scriptum;
        s32 codex;

        scriptum = filum_scribere_literis(
            "/tmp/probatio_generare_error.c",
            "/* generare: instrumentum_non_existens arg1 */\n");
        CREDO_VERUM(scriptum);

        codex =
            system(GENERARE "/tmp/probatio_generare_error.c 2>/dev/null");
        CREDO_INAEQUALITAS_S32(codex, ZEPHYRUM);

        filum_delere("/tmp/probatio_generare_error.c");
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "una directiva simplex", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "multiplices directivas", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nullas directivas", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "directivam cum multis argumentis", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "spatium album extra", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "directivam sine argumentis", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "commentarium non-directivam", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "directivam in medio fili", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "numeros versuum", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variabiles ambitus", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "end-to-end", _sectio_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error terminat", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("generare", SECTIONES);
}
