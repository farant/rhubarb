/*
 * probatio_machina.c - Probationes identitatis machinae
 *
 * SECTIO I CARDO EST. Officium quod constantem redderet omnes
 * probationes obvias praeteriret: stabilis esset, hex esset,
 * longitudinis rectae esset. SOLA probatio quae fontes DUOS confert
 * eam deprehendit - ideo nucleus purus seorsum expositus est.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "machina.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

interior b32
_omnes_hex (
    chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character x = (character)c.datum[i];

        si (!((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f')))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


    /* ========================================================
     * I. Nucleus purus: fontes DIVERSI, identitates DIVERSAE
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fons_a, fons_b;
        chorda id_a, id_b, id_a_iterum;

        fons_a = chorda_ex_literis("machina-alpha", piscina);
        fons_b = chorda_ex_literis("machina-beta", piscina);

        id_a         = machina_identitas_ex_fonte(fons_a, piscina);
        id_b         = machina_identitas_ex_fonte(fons_b, piscina);
        id_a_iterum  = machina_identitas_ex_fonte(fons_a, piscina);

        /* HAEC probatio constantem deprehendit - nulla alia potest */
        CREDO_FALSUM(chorda_aequalis(id_a, id_b));

        /* Determinatio: idem fons, idem exitus */
        CREDO_VERUM(chorda_aequalis(id_a, id_a_iterum));

        /* OPACITAS: identitas fontem non repetit */
        CREDO_FALSUM(chorda_aequalis(id_a, fons_a));

        /* Forma */
        CREDO_AEQUALIS_I32(id_a.mensura,
            (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_AEQUALIS_I32(id_b.mensura,
            (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(_omnes_hex(id_a));
        CREDO_VERUM(_omnes_hex(id_b));

        /* Fons unius characteris differens sufficit */
        {
            chorda fons_c = chorda_ex_literis("machina-alphb", piscina);
            chorda id_c = machina_identitas_ex_fonte(fons_c, piscina);
            CREDO_FALSUM(chorda_aequalis(id_a, id_c));
        }
    }
}


    /* ========================================================
     * II. Fons vacuus RECUSATUR
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda vacua, exitus;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;

        exitus = machina_identitas_ex_fonte(vacua, piscina);
        CREDO_AEQUALIS_I32(exitus.mensura, ZEPHYRUM);

        exitus = machina_identitas_ex_fonte(
                     chorda_ex_literis("quidquid", piscina), NIHIL);
        CREDO_AEQUALIS_I32(exitus.mensura, ZEPHYRUM);
    }
}


    /* ========================================================
     * III. Identitas huius machinae
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda prima, altera;

        prima   = machina_identitas(piscina);
        altera  = machina_identitas(piscina);

        /* Vacua hic significat fontem deesse - vitium verum in
         * machina quae aut gethostuuid aut /etc/machine-id habet */
        CREDO_AEQUALIS_I32(prima.mensura,
                           (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(_omnes_hex(prima));

        /* STABILIS: bis vocata idem reddit */
        CREDO_VERUM(chorda_aequalis(prima, altera));

        imprimere("    identitas: %.*s\n", (integer)prima.mensura,
                  (constans character*)prima.datum);
    }
}


    /* ========================================================
     * IV. Nota plena
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MachinaNota nota;

        nota = machina_nota(piscina);

        CREDO_VERUM(nota.valida);
        CREDO_AEQUALIS_I32(nota.identitas.mensura,
                           (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(chorda_aequalis(nota.identitas,
                                    machina_identitas(piscina)));

        CREDO_CHORDA_NON_VACUA(nota.systema);
        CREDO_CHORDA_NON_VACUA(nota.architectura);
        CREDO_MAIOR_I32(nota.nuclei, ZEPHYRUM);

        /* Causa VACUA cum valida - aliter nuntius errorem simularet */
        CREDO_AEQUALIS_I32(nota.causa.mensura, ZEPHYRUM);

        imprimere("    systema: %.*s | architectura: %.*s | nuclei: %u\n",
                  (integer)nota.systema.mensura,
                  (constans character*)nota.systema.datum,
                  (integer)nota.architectura.mensura,
                  (constans character*)nota.architectura.datum,
                  nota.nuclei);
    }
}


    /* ========================================================
     * V. Piscina absens
     * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        MachinaNota nota;
             chorda id;

        id = machina_identitas(NIHIL);
        CREDO_AEQUALIS_I32(id.mensura, ZEPHYRUM);

        nota = machina_nota(NIHIL);
        CREDO_FALSUM(nota.valida);
    }
}


    /* ========================================================
     * VI. Identitas ex plagula (Android: nullus fons systematis;
     *     android-gradus T4, Franus 2026-10-23 optio I)
     *
     * Fortuitum SEMEL scriptum, deinde lectum - id quod
     * /etc/machine-id Linux est. Plagula brevis numquam
     * rescribitur: identitas mutata acta machinarum confunderet.
     * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* dir = "build/probatio_machina_plagula";
        constans character* via =
            "build/probatio_machina_plagula/machina";
        constans character* altera =
            "build/probatio_machina_plagula/altera";
        constans character* curta =
            "build/probatio_machina_plagula/curta";
                    chorda a;
                    chorda b;
                    chorda alia;
                    chorda contentum;
               FilumStatus status;

        CREDO_VERUM(filum_directorium_creare_cum_parentibus(dir));
        (vacuum)filum_delere(via);
        (vacuum)filum_delere(altera);
        (vacuum)filum_delere(curta);

        /* abest -> creatur: XVI hex, plagula 0600 */
        a = machina_identitas_ex_plagula(via, piscina);
        CREDO_AEQUALIS_I32(a.mensura, XVI);
        CREDO_VERUM(filum_existit(via));
        CREDO_VERUM(filum_status(via, &status));
        CREDO_AEQUALIS_I32((i32)status.modus, (i32)0600);
        contentum = filum_legere_totum(via, piscina);
        CREDO_VERUM(contentum.mensura >= XXXII);

        /* adest -> eadem (persistens, non iterum fortuita) */
        b = machina_identitas_ex_plagula(via, piscina);
        CREDO_VERUM(chorda_aequalis(a, b));

        /* plagula altera -> identitas altera (fortuitum verum) */
        alia = machina_identitas_ex_plagula(altera, piscina);
        CREDO_AEQUALIS_I32(alia.mensura, XVI);
        CREDO_FALSUM(chorda_aequalis(a, alia));

        /* curta -> recusatio, plagula intacta */
        CREDO_VERUM(filum_scribere_literis(curta, "abc"));
        CREDO_AEQUALIS_I32(
            machina_identitas_ex_plagula(curta, piscina).mensura,
            ZEPHYRUM);
        contentum = filum_legere_totum(curta, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(contentum, "abc");

        /* via NIHIL -> vacua */
        CREDO_AEQUALIS_I32(
            machina_identitas_ex_plagula(NIHIL, piscina).mensura,
            ZEPHYRUM);

        (vacuum)filum_delere(via);
        (vacuum)filum_delere(altera);
        (vacuum)filum_delere(curta);
        /* serae numquam a filum deletae (filum.h): hic purgantur */
        (vacuum)filum_delere("build/probatio_machina_plagula/machina.sera");
        (vacuum)filum_delere("build/probatio_machina_plagula/altera.sera");
        (vacuum)filum_delere("build/probatio_machina_plagula/curta.sera");
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. Fontes diversi -> identitates diversae", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "II. Fons vacuus", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. Identitas machinae", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IV. Nota plena", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. Piscina absens", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI. Identitas ex plagula", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("machina", SECTIONES);
}
