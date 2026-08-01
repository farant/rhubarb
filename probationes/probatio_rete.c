/* probatio_rete.c - enumeratio interfacierum
 *
 * DIFFICULTAS: fructus a statu reticuli machinae pendet, qui
 * probationi ignotus est (wifi? hotspot? nihil?). Ergo nihil de
 * NUMERO aut de addressibus SPECIFICIS asseritur - solum
 * INVARIANTIA quae vera esse debent quicquid reticulum facit:
 * fines servati, campi impleti, formae validae, exclusiones
 * observatae. Probatio quae "en0 adest" poposcisset in machina
 * cablata aut in Docker caderet, et illa fractura de codice
 * nihil diceret.
 */

#include "latina.h"
#include "rete.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

/* forma IPv4: quattuor numeri puncto separati, singuli 0-255 */
interior b32
_forma_ipv4_valida (constans character* s)
{
    i32 puncta = ZEPHYRUM;
    i32 digiti = ZEPHYRUM;
    i32 valor  = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; s[i] != '\0'; i++)
    {
        si (s[i] == '.')
        {
            si (digiti == ZEPHYRUM || valor > CCLV)
            {
                redde FALSUM;
            }
            puncta++;
            digiti = ZEPHYRUM;
            valor = ZEPHYRUM;
        }
        alioquin si (s[i] >= '0' && s[i] <= '9')
        {
            valor = valor * X + (i32)(s[i] - '0');
            digiti++;
            si (digiti > III)
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    redde (puncta == III && digiti > ZEPHYRUM && valor <= CCLV)
        ? VERUM : FALSUM;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    piscina = piscina_generare_dynamicum("probatio_rete", 65536);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- argumenta mala ---- */
    {
        ReteInterfacies unum;

        imprimere("\n--- Probans fines ---\n");
        CREDO_AEQUALIS_I32(rete_addressus_locales(NIHIL, X),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32(rete_addressus_locales(&unum, ZEPHYRUM),
            ZEPHYRUM);
    }

    /* ---- invariantia fructus ---- */
    {
        ReteInterfacies tabulatum[16];
        i32 numerus;
        i32 i;

        imprimere("\n--- Probans invariantia ---\n");
        memset(tabulatum, ZEPHYRUM, magnitudo(tabulatum));
        numerus = rete_addressus_locales(tabulatum, XVI);

        /* fines servati */
        CREDO_VERUM(numerus <= XVI);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            imprimere("  %s = %s\n", tabulatum[i].titulus,
                tabulatum[i].addressum);
            /* ambo campi impleti et terminati */
            CREDO_VERUM(strlen(tabulatum[i].titulus) > ZEPHYRUM);
            CREDO_VERUM(strlen(tabulatum[i].addressum) > ZEPHYRUM);
            CREDO_VERUM(strlen(tabulatum[i].titulus)
                < RETE_TITULUS_MENSURA);
            CREDO_VERUM(strlen(tabulatum[i].addressum)
                < RETE_ADDRESSUM_MENSURA);
            /* forma valida */
            CREDO_VERUM(_forma_ipv4_valida(tabulatum[i].addressum));
            /* exclusiones promissae: loopback et nexus-localis */
            CREDO_VERUM(strncmp(tabulatum[i].addressum, "127.", IV)
                != ZEPHYRUM);
            CREDO_VERUM(strncmp(tabulatum[i].addressum, "169.254.",
                VIII) != ZEPHYRUM);
        }

        /* capacitas angusta: numquam supra petitum */
        si (numerus > ZEPHYRUM)
        {
            ReteInterfacies unum;
            i32 pauci;

            memset(&unum, ZEPHYRUM, magnitudo(unum));
            pauci = rete_addressus_locales(&unum, I);
            CREDO_VERUM(pauci <= I);
            si (pauci == I)
            {
                CREDO_VERUM(strlen(unum.addressum) > ZEPHYRUM);
            }
        }
    }

    /* probatio ipsa numerum non poscit: machina sine reticulo
     * ZEPHYRUM legitime reddit, et id NON est fractura */
    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
