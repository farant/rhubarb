#include "servitium.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Creatio
 * ================================================== */

RegistrumServitii*
registrum_servitii_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    InternamentumChorda* intern)
{
    RegistrumServitii* reg;

    si (!piscina || !repositorium || !intern)
    {
        redde NIHIL;
    }

    reg = (RegistrumServitii*)piscina_allocare(piscina, magnitudo(RegistrumServitii));
    si (!reg)
    {
        redde NIHIL;
    }

    reg->piscina      = piscina;
    reg->repositorium = repositorium;
    reg->intern       = intern;

    reg->nomina = tabula_dispersa_creare_chorda(piscina, XXXII);
    si (!reg->nomina)
    {
        redde NIHIL;
    }

    redde reg;
}


/* ==================================================
 * Registratio
 * ================================================== */

b32
servitium_registrare(
    RegistrumServitii*  reg,
    constans character* titulus,
    chorda*             entitas_id)
{
    chorda    titulus_chorda;
    character nota_buffer[CXXVIII];
    Entitas*  entitas;

    si (!reg || !titulus || !entitas_id)
    {
        redde FALSUM;
    }

    /* Verificare si entitas existit */
    entitas = reg->repositorium->capere_entitatem(
        reg->repositorium->datum,
        entitas_id);

    si (!entitas)
    {
        redde FALSUM;
    }

    /* Creare titulus chorda */
    titulus_chorda = chorda_ex_literis(titulus, reg->piscina);

    /* Inserere in registrum */
    si (!tabula_dispersa_inserere(reg->nomina, titulus_chorda, entitas_id))
    {
        redde FALSUM;
    }

    /* Addere nota "#service::titulus" ad entitatem */
    sprintf(nota_buffer, "%s%s", SERVITIUM_NOTA_PRAEFIXUM, titulus);
    reg->repositorium->nota_addere(
        reg->repositorium->datum,
        entitas,
        nota_buffer);

    redde VERUM;
}

b32
servitium_registrare_entitatem(
    RegistrumServitii*  reg,
    constans character* titulus,
    Entitas*            entitas)
{
    si (!reg || !titulus || !entitas)
    {
        redde FALSUM;
    }

    redde servitium_registrare(reg, titulus, entitas->id);
}

b32
servitium_deregistrare(
    RegistrumServitii*  reg,
    constans character* titulus)
{
    chorda titulus_chorda;

    si (!reg || !titulus)
    {
        redde FALSUM;
    }

    titulus_chorda = chorda_ex_literis(titulus, reg->piscina);

    redde tabula_dispersa_delere(reg->nomina, titulus_chorda);
}


/* ==================================================
 * Quaestio
 * ================================================== */

chorda*
servitium_obtinere_id(
    RegistrumServitii*  reg,
    constans character* titulus)
{
    vacuum* valor;

    si (!reg || !titulus)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire_literis(reg->nomina, titulus, &valor))
    {
        redde (chorda*)valor;
    }

    redde NIHIL;
}

Entitas*
servitium_obtinere(
    RegistrumServitii*  reg,
    constans character* titulus)
{
    chorda* entitas_id;

    si (!reg || !titulus)
    {
        redde NIHIL;
    }

    entitas_id = servitium_obtinere_id(reg, titulus);
    si (!entitas_id)
    {
        redde NIHIL;
    }

    redde reg->repositorium->capere_entitatem(
        reg->repositorium->datum,
        entitas_id);
}

b32
servitium_habet(
    RegistrumServitii*  reg,
    constans character* titulus)
{
    si (!reg || !titulus)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet_literis(reg->nomina, titulus);
}

i32
servitium_numerus(
    RegistrumServitii* reg)
{
    si (!reg)
    {
        redde ZEPHYRUM;
    }

    redde tabula_dispersa_numerus(reg->nomina);
}


/* ==================================================
 * Auto-Discovery
 * ================================================== */

i32
servitium_discoperire(
    RegistrumServitii* reg)
{
    Xar*    entitates;
    i32     i;
    i32     numerus;
    i32     discooperti;
    i32     praefixum_len;

    si (!reg)
    {
        redde ZEPHYRUM;
    }

    praefixum_len = (i32)strlen(SERVITIUM_NOTA_PRAEFIXUM);

    /* Quaerere entitates cum praefixo notae */
    entitates = reg->repositorium->quaerere_cum_praefixo_notae(
        reg->repositorium->datum,
        SERVITIUM_NOTA_PRAEFIXUM);

    si (!entitates)
    {
        redde ZEPHYRUM;
    }

    numerus     = xar_numerus(entitates);
    discooperti = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Entitas** slot;
        Entitas*  entitas;
        i32       j;
        i32       numerus_notarum;

        slot = (Entitas**)xar_obtinere(entitates, i);
        si (!slot || !*slot)
        {
            perge;
        }
        entitas = *slot;

        /* Iterare per notas entitatis */
        numerus_notarum = xar_numerus(entitas->notae);

        per (j = ZEPHYRUM; j < numerus_notarum; j++)
        {
            chorda** nota_slot;
            chorda*  nota;

            nota_slot = (chorda**)xar_obtinere(entitas->notae, j);
            si (!nota_slot || !*nota_slot)
            {
                perge;
            }
            nota = *nota_slot;

            /* Verificare praefixum */
            si (nota->mensura > praefixum_len)
            {
                b32 aequalis;
                i32 k;

                aequalis = VERUM;
                per (k = ZEPHYRUM; k < praefixum_len; k++)
                {
                    si (nota->datum[k] != (i8)SERVITIUM_NOTA_PRAEFIXUM[k])
                    {
                        aequalis = FALSUM;
                        frange;
                    }
                }

                si (aequalis)
                {
                    /* Extrahire nomen post praefixum */
                    character  nomen_buffer[LXIV];
                    i32        nomen_len;
                    chorda     nomen_chorda;

                    nomen_len = nota->mensura - praefixum_len;
                    si (nomen_len > LXIII)
                    {
                        nomen_len = LXIII;
                    }

                    memcpy(nomen_buffer,
                           nota->datum + praefixum_len,
                           (memoriae_index)nomen_len);
                    nomen_buffer[nomen_len] = '\0';

                    /* Registrare si non iam registratum */
                    si (!servitium_habet(reg, nomen_buffer))
                    {
                        nomen_chorda = chorda_ex_literis(nomen_buffer, reg->piscina);

                        tabula_dispersa_inserere(reg->nomina, nomen_chorda, entitas->id);
                        discooperti++;
                    }
                }
            }
        }
    }

    redde discooperti;
}


/* ==================================================
 * Utilitas
 * ================================================== */

vacuum
servitium_imprimere(
    RegistrumServitii* reg)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;

    si (!reg)
    {
        imprimere("RegistrumServitii: NIHIL\n");
        redde;
    }

    imprimere("RegistrumServitii (%d servitia):\n", servitium_numerus(reg));

    iter = tabula_dispersa_iterator_initium(reg->nomina);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        chorda*  entitas_id;
        Entitas* entitas;
        chorda*  titulus;

        entitas_id = (chorda*)valor;
        entitas = reg->repositorium->capere_entitatem(
            reg->repositorium->datum,
            entitas_id);

        si (entitas)
        {
            titulus = entitas_titulum_capere(entitas);
            imprimere("  %.*s -> %.*s (%.*s)\n",
                      clavis.mensura, clavis.datum,
                      entitas_id->mensura, entitas_id->datum,
                      titulus ? titulus->mensura : III,
                      titulus ? (character*)titulus->datum : "???");
        }
        alioquin
        {
            imprimere("  %.*s -> %.*s (non inventum)\n",
                      clavis.mensura, clavis.datum,
                      entitas_id->mensura, entitas_id->datum);
        }
    }
}
