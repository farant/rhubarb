/* provenientia.c - relatio '-provenientia' binariorum installatorum
 * (vide include/provenientia.h; plan fabrica 1a T7) */

#include "provenientia.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

chorda
provenientia_textus (
    constans ProvenientiaRelatio* relatio,
                         Piscina* piscina)
{
    ChordaAedificator* aedificator;

    aedificator = chorda_aedificator_creare(piscina, 160);
    si (aedificator == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        "provenientia 1\nartificium ");
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        relatio->artificium);
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        "\ningressus ");
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        relatio->ingressus);
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        "\ncommissum ");
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        relatio->commissum);
    (vacuum)chorda_aedificator_appendere_character(aedificator, '\n');
    redde chorda_aedificator_finire(aedificator);
}

b32
provenientia_respondere (
                              s32   argc,
                        character** argv,
     constans ProvenientiaRelatio*  relatio)
{
    si (argc != II || strcmp(argv[1], "-provenientia") != 0)
    {
        redde FALSUM;
    }
    /* forma UNA: textus quem probatio figit ipse scribitur */
    {
         Piscina* piscina;
          chorda  textus;

        piscina = piscina_generare_dynamicum("provenientia", 1024);
        si (piscina == NIHIL)
        {
            redde VERUM;
        }
        textus = provenientia_textus(relatio, piscina);
        (vacuum)fwrite(textus.datum, 1, (memoriae_index)textus.mensura,
            stdout);
        piscina_destruere(piscina);
    }
    redde VERUM;
}
