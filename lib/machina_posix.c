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
#include "lectiones.h"
#include "filum.h"
#include "moneta.h"

#include <stdio.h>
#include <stdlib.h>
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

    sigillum = sigillum_computare(fons.datum,
        (memoriae_index)fons.mensura);
    sigillum_hex(&sigillum, hex);

    /* Truncatio: LXIV litterae in tabulis legi non possunt, XVI
     * possunt, et LXIV bita machinis paucis abunde sufficiunt */
    hex[MACHINA_IDENTITAS_LONGITUDO] = '\0';

    redde chorda_ex_literis(hex, piscina);
}


/* ============================================================
 * Identitas persistens (fons ubi systema nullum praebet)
 * ============================================================ */

/* Plagulam novam sub sera scribere: XVI octeti fortuiti ut XXXII
 * litterae hex, in 'via.novum' modo 0600, deinde renominatio - lector
 * numquam plagulam semiscriptam videt. FALSUM si fortuita absunt aut
 * scriptura deficit (nihil in 'via' relinquitur). */
interior b32
_plagulam_identitatis_scribere (
    constans character* via,
    constans character* via_nova)
{
    constans character* digiti = "0123456789abcdef";
                    i8  octeti[XVI];
             character  hex[XXXIII];
                chorda  contentum;
                   i32  i;

    si (!moneta_octeti_fortuiti(octeti, (i32)XVI))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < (i32)XVI; i++)
    {
        hex[i * II]      = digiti[(octeti[i] >> IV) & 0xF];
        hex[i * II + I]  = digiti[octeti[i] & 0xF];
    }
    hex[XXXII] = '\0';
    /* firmiter (write + fsync) ANTE renominationem: alioquin post
     * defectum potentiae plagula vacua sub nomine vero manere potest
     * (allocatio dilata ext4/f2fs), quae ut 'curta' in aeternum
     * recusaretur (recensio 2026-10-23). Reliquiae .novum prius
     * deletae, quia firmiter APPENDIT. */
    contentum.datum    = (i8*)hex;
    contentum.mensura  = (i32)XXXII;
    (vacuum)filum_delere(via_nova);
    si (   !filum_appendere_firmiter(via_nova, contentum)
        || !filum_modum_ponere(via_nova, 0600)
        || !filum_movere(via_nova, via))
    {
        (vacuum)filum_delere(via_nova);
        redde FALSUM;
    }
    redde VERUM;
}

chorda
machina_identitas_ex_plagula (
    constans character* via,
               Piscina* piscina)
{
        chorda  vacua;
        chorda  contentum;
     character  via_serae[DXII];
     character  via_nova[DXII];
     FilumSera* sera;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (via == NIHIL || piscina == NIHIL || strlen(via) + VII > DXII)
    {
        redde vacua;
    }
    sprintf(via_serae, "%s.sera", via);
    sprintf(via_nova, "%s.novum", via);

    /* sera per totam seriem EXISTIT -> SCRIBERE -> LEGERE: duo
     * processus simul incipientes identitatem unam vident */
    sera = filum_seram_capere(via_serae, (i32)MM, piscina);
    si (sera == NIHIL)
    {
        redde vacua;
    }
    si (   !filum_existit(via) && !_plagulam_identitatis_scribere(via,
            via_nova))
    {
        filum_seram_liberare(sera);
        redde vacua;
    }
    contentum = filum_legere_totum(via, piscina);
    filum_seram_liberare(sera);

    /* curta: recusatio, numquam rescriptio (RECUSAT supra) */
    si (contentum.mensura < (i32)XVI)
    {
        redde vacua;
    }
    redde machina_identitas_ex_fonte(contentum, piscina);
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
#elif defined(__ANDROID__)
    {
        /* Android: nec gethostuuid nec /etc/machine-id. Identitas
         * persistens sub $HOME/.rhubarb/machina - cursor tabulae HOME
         * radicem suam ponit, applicatio directorium suum privatum
         * (identitas = haec institutio; android-gradus T4) */
        constans character* domus;
                 character  directorium[DXII];
                 character  via[DXII];

        domus = getenv("HOME");
        si (domus != NIHIL && domus[0] != '\0' && strlen(domus) < CDXL)
        {
            sprintf(directorium, "%s/.rhubarb", domus);
            sprintf(via, "%s/machina", directorium);
            si (filum_directorium_creare_cum_parentibus(directorium))
            {
                redde machina_identitas_ex_plagula(via, piscina);
            }
        }
    }
#else
    {
        FILE*     plagula;
        character tabula[LXIV];
        size_t    lecta;
        chorda    fons;

        plagula = lectiones_fopen("/etc/machine-id", "rb");
        si (plagula != NIHIL)
        {
            lecta = fread(tabula, (size_t)I, magnitudo(tabula),
                plagula);
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
            chorda_aedificator_appendere_literis(aedificator,
                nomina.sysname);
            chorda_aedificator_appendere_literis(aedificator, " ");
            chorda_aedificator_appendere_literis(aedificator,
                nomina.release);
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
