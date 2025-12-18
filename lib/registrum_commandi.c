#include "registrum_commandi.h"
#include "chorda.h"
#include <string.h>

/* ==================================================
 * Registry - Creatio
 * ================================================== */

RegistrumCommandi*
registrum_commandi_creare(
    Piscina* piscina)
{
    RegistrumCommandi* reg;

    reg = piscina_allocare(piscina, magnitudo(RegistrumCommandi));
    si (!reg)
    {
        redde NIHIL;
    }

    reg->piscina = piscina;
    reg->tabula = tabula_dispersa_creare_chorda(piscina, LXIV);  /* 64 initial capacity */

    si (!reg->tabula)
    {
        redde NIHIL;
    }

    redde reg;
}


/* ==================================================
 * Registry - Registratio
 * ================================================== */

vacuum
registrum_commandi_registrare(
    RegistrumCommandi* reg,
    constans character* nomen_commandi,
    FunctioCommand functio,
    vacuum* datum)
{
    Command* command;
    chorda clavis;
    i32 i;

    si (!reg || !nomen_commandi || !functio)
    {
        redde;
    }

    /* Allocate command */
    command = piscina_allocare(reg->piscina, magnitudo(Command));
    si (!command)
    {
        redde;
    }

    /* Copy name */
    per (i = ZEPHYRUM; i < XXXII - I && nomen_commandi[i] != '\0'; i++)
    {
        command->titulus[i] = nomen_commandi[i];
    }
    command->titulus[i] = '\0';

    command->executare = functio;
    command->datum = datum;

    /* Create chorda key */
    clavis = chorda_ex_literis(nomen_commandi, reg->piscina);

    /* Insert into hash table */
    tabula_dispersa_inserere(reg->tabula, clavis, command);
}


/* ==================================================
 * Registry - Executio
 * ================================================== */

b32
registrum_commandi_executare(
    RegistrumCommandi* reg,
    constans character* nomen_commandi,
    ContextusCommandi* ctx)
{
    Command* command;
    vacuum* valor;
    b32 inventus;

    si (!reg || !nomen_commandi || !ctx)
    {
        redde FALSUM;
    }

    /* Look up command by C string */
    inventus = tabula_dispersa_invenire_literis(reg->tabula, nomen_commandi, &valor);
    si (!inventus)
    {
        redde FALSUM;  /* Command not found */
    }

    command = (Command*)valor;

    /* Ponere datum registratum in contextu */
    ctx->datum_registratus = command->datum;

    /* Execute command */
    redde command->executare(ctx);
}


/* ==================================================
 * Registry - Interrogatio
 * ================================================== */

b32
registrum_commandi_habet(
    RegistrumCommandi* reg,
    constans character* nomen_commandi)
{
    si (!reg || !nomen_commandi)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet_literis(reg->tabula, nomen_commandi);
}
