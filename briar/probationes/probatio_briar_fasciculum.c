/* probatio_briar_fasciculum.c - '-app' (spec par. 4.8) SINE fenestra
 * et SINE decodificatore: consilium ex nexu fixorum (opus III plani 7),
 * scriptor super Imago ficta (opus IV). */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_fasciculum.h"
#include "briar_nexus.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";

/* fixum legere, nexum texere, consilium reddere */
interior b32
_consilium (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* titulus,
          constans character* via_icon,
    BriarFasciculumConsilium* c,
                      chorda* causa,
                         i32* linea)
{
    constans character* via = chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(FIXA, piscina),
        chorda_ex_literis(titulus, piscina), piscina), piscina);
                 chorda textus = filum_legere_totum(via, piscina);
          MateriaNodus* doc;
                   Xar* nexus;

    *linea = ZEPHYRUM;
    si (textus.mensura == ZEPHYRUM)
    {
        *causa = chorda_ex_literis("fixum non lectum", piscina);
        redde FALSUM;
    }
    doc    = briar_arbor_parsare(piscina,
        (constans character*)textus.datum, (i32)textus.mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    redde briar_fasciculum_consilium(piscina, nexus, via, via_icon, c,
        causa, linea);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_fasciculum",
        (memoriae_index)4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans consilium: salve_vitreum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern,
            "salve_vitreum.thistle", NIHIL, &c, &causa, &linea));
        /* '_' in '-': identitas characteres suos non habet */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve-vitreum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
        CREDO_CHORDA_VACUA(c.versio);
        CREDO_CHORDA_VACUA(c.via_icon);
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_app,
            "briar/probationes/fixa/thistle/salve_vitreum.app");
        CREDO_AEQUALIS_I32(c.linea_briar, ZEPHYRUM);
    }

    imprimere("\n--- Probans consilium: salve (sine fenestra) ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "salve.thistle", NIHIL,
            &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve");
        /* nomen e plagula, nulla fenestra */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
    }

    imprimere("\n--- Probans consilium: briar vestitum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            NIHIL, &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "Vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.versio, "2.1");
        /* icon attributi ad directorium PLAGULAE resolvitur */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon,
            "briar/probationes/fixa/thistle/signa/vestitum.png");
        CREDO_AEQUALIS_I32(c.linea_briar, VIII);

        /* vexillum -icon attributum vincit, et intactum manet */
        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            "alibi/icon.png", &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon, "alibi/icon.png");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
    }

    imprimere("\n--- Probans recusationes cum linea ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_identitas_prava.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, IV);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("vitium_malum", piscina));

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_briar_duplex.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, VI);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("linea 4", piscina));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
