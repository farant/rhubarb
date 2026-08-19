/*
 * machina_posix.c - Identitas machinae (POSIX + Darwin)
 *
 * Fons identitatis per platformam differt, sed AMBO fontes iam sunt
 * id quod volumus - identificator machinae stabilis. Nos eos
 * sigillamus solum ut opaci et unius longitudinis fiant.
 */

#include "postulata_posix.h"

#include "machina.h"
#include "sigillum.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#ifdef __APPLE__
#include <uuid/uuid.h>
#endif


/* ============================================================
 * Nucleus purus
 * ============================================================ */

chorda
machina_identitas_ex_fonte (
    constans chorda  fons,
            Piscina* piscina)
{
     Sigillum sigillum;
    character hex[SIGILLUM_HEX_MENSURA];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (   piscina      == NIHIL || fons.datum == NIHIL
        || fons.mensura == ZEPHYRUM)
    {
        redde vacua;
    }

    sigillum = sigillum_computare(fons.datum, (memoriae_index)fons.mensura);
    sigillum_hex(&sigillum, hex);

    /* Truncatio: LXIV litterae in tabulis legi non possunt, XVI
     * possunt, et LXIV bita machinis paucis abunde sufficiunt */
    hex[MACHINA_IDENTITAS_LONGITUDO] = '\0';

    redde chorda_ex_literis(hex, piscina);
}


/* ============================================================
 * Fons platformae
 * ============================================================ */

chorda
machina_identitas (
    Piscina* piscina)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (piscina == NIHIL)
    {
        redde vacua;
    }

#ifdef __APPLE__
    {
        uuid_t          id;
        structura timespec mora;
        chorda          fons;

        mora.tv_sec  = 0;
        mora.tv_nsec = 0;

        si (gethostuuid(id, &mora) == 0)
        {
            fons.datum   = (i8*)id;
            fons.mensura = (i32)XVI;   /* uuid_t = XVI octeti */
            redde machina_identitas_ex_fonte(fons, piscina);
        }
    }
#else
    {
        FILE*     plagula;
        character tabula[LXIV];
        size_t    lecta;
        chorda    fons;

        plagula = fopen("/etc/machine-id", "rb");
        si (plagula != NIHIL)
        {
            lecta = fread(tabula, (size_t)I, magnitudo(tabula), plagula);
            fclose(plagula);

            si (lecta > 0)
            {
                fons.datum    = (i8*)tabula;
                fons.mensura  = (i32)lecta;
                redde machina_identitas_ex_fonte(fons, piscina);
            }
        }
    }
#endif

    /* RECUSAT. Constans hic mensuras omnium machinarum
     * confunderet - id est vitium quod haec bibliotheca vetat. */
    redde vacua;
}


/* ============================================================
 * Nota plena
 * ============================================================ */

MachinaNota
machina_nota (
    Piscina* piscina)
{
          MachinaNota nota;
    structura utsname  nomina;
               longus  numerus_nucleorum;
    ChordaAedificator* aedificator;

    nota.valida                = FALSUM;
    nota.identitas.datum       = NIHIL;
    nota.identitas.mensura     = ZEPHYRUM;
    nota.systema.datum         = NIHIL;
    nota.systema.mensura       = ZEPHYRUM;
    nota.architectura.datum    = NIHIL;
    nota.architectura.mensura  = ZEPHYRUM;
    nota.nuclei                = ZEPHYRUM;
    nota.causa.datum           = NIHIL;
    nota.causa.mensura         = ZEPHYRUM;

    si (piscina == NIHIL)
    {
        redde nota;
    }

    nota.identitas = machina_identitas(piscina);
    si (nota.identitas.mensura == ZEPHYRUM)
    {
        nota.causa = chorda_ex_literis(
            "Identitas machinae deprehendi non potest "
            "(gethostuuid et /etc/machine-id ambo defecerunt)",
            piscina);
        redde nota;
    }

    si (uname(&nomina) == 0)
    {
        aedificator = chorda_aedificator_creare(piscina, (i32)C);
        si (aedificator != NIHIL)
        {
            chorda_aedificator_appendere_literis(aedificator, nomina.sysname);
            chorda_aedificator_appendere_literis(aedificator, " ");
            chorda_aedificator_appendere_literis(aedificator, nomina.release);
            nota.systema = chorda_aedificator_finire(aedificator);
        }
        nota.architectura = chorda_ex_literis(nomina.machine, piscina);
    }

    numerus_nucleorum = sysconf(_SC_NPROCESSORS_ONLN);
    si (numerus_nucleorum > 0)
    {
        nota.nuclei = (i32)numerus_nucleorum;
    }

    nota.valida = VERUM;

    redde nota;
}
