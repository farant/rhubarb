#include "registrum_widget.h"
#include "internamentum.h"
#include <string.h>


/* ==================================================
 * Creatio
 * ================================================== */

RegistrumWidget*
registrum_widget_creare(
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    RegistrumWidget* reg;

    si (!piscina || !intern)
    {
        redde NIHIL;
    }

    reg = piscina_allocare(piscina, magnitudo(RegistrumWidget));
    si (!reg)
    {
        redde NIHIL;
    }

    reg->piscina = piscina;
    reg->intern = intern;
    reg->numerus = ZEPHYRUM;

    redde reg;
}


/* ==================================================
 * Registratio
 * ================================================== */

b32
registrum_widget_registrare(
    RegistrumWidget*     reg,
    constans character*  titulus,
    FunctioWidgetFactory factory)
{
    chorda* titulus_intern;

    si (!reg || !titulus || !factory)
    {
        redde FALSUM;
    }

    si (reg->numerus >= REGISTRUM_WIDGET_MAXIMUS)
    {
        redde FALSUM;  /* Plenum */
    }

    /* Internare titulus */
    titulus_intern = chorda_internare_ex_literis(reg->intern, titulus);
    si (!titulus_intern)
    {
        redde FALSUM;
    }

    /* Addere introitus */
    reg->introitus[reg->numerus].titulus = titulus_intern;
    reg->introitus[reg->numerus].factory = factory;
    reg->numerus++;

    redde VERUM;
}


/* ==================================================
 * Lookup
 * ================================================== */

FunctioWidgetFactory
registrum_widget_invenire(
    RegistrumWidget* reg,
    chorda*          titulus)
{
    s32 i;

    si (!reg || !titulus)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < reg->numerus; i++)
    {
        /* Comparare per pointer (internatum) vel per contentum */
        si (reg->introitus[i].titulus == titulus ||
            chorda_aequalis(*reg->introitus[i].titulus, *titulus))
        {
            redde reg->introitus[i].factory;
        }
    }

    redde NIHIL;
}

FunctioWidgetFactory
registrum_widget_invenire_literis(
    RegistrumWidget*    reg,
    constans character* titulus)
{
    s32 i;

    si (!reg || !titulus)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < reg->numerus; i++)
    {
        si (chorda_aequalis_literis(*reg->introitus[i].titulus, titulus))
        {
            redde reg->introitus[i].factory;
        }
    }

    redde NIHIL;
}
