/* probatio_macho.c - Probationes bibliothecae Mach-O */

#include "latina.h"
#include "macho.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


    /* ==================================================
	 * Probare macho_aperire cum fixture "simplex"
	 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via = "probationes/fixa/simplex";
                     MachO* macho;
                       i32  genus_cpu;
                       i32  numerus_mandatorum;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        /* Verificare est 64-bit */
        CREDO_VERUM(macho_est_64bit(macho));

        /* Verificare genus processoris (ARM64 in Apple Silicon) */
        genus_cpu = macho_genus_processoris(macho);
        imprimere("  CPU type: 0x%x\n", genus_cpu);
        CREDO_VERUM(genus_cpu == MACHO_CPU_TYPE_X86_64

            || genus_cpu == MACHO_CPU_TYPE_ARM64);

        /* Verificare genus filum est executabile */
        CREDO_AEQUALIS_I32(macho_genus_filum(macho),
            MACHO_GENUS_EXECUTABILE);

        /* Verificare habet mandata oneris */
        numerus_mandatorum = macho_numerus_mandatorum(macho);
        imprimere("  Load commands: %d\n", numerus_mandatorum);
        CREDO_VERUM(numerus_mandatorum > ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare macho_aperire cum fixture "cum_functione"
	 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via = "probationes/fixa/cum_functione";
                     MachO* macho;
                       i32  numerus_mandatorum;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        /* Verificare est 64-bit */
        CREDO_VERUM(macho_est_64bit(macho));

        /* Verificare genus filum */
        CREDO_AEQUALIS_I32(macho_genus_filum(macho),
            MACHO_GENUS_EXECUTABILE);

        /* Verificare habet mandata oneris */
        numerus_mandatorum = macho_numerus_mandatorum(macho);
        imprimere("  Load commands: %d\n", numerus_mandatorum);
        CREDO_VERUM(numerus_mandatorum > ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare iterationem mandatorum oneris
	 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           constans character* via = "probationes/fixa/simplex";
                        MachO* macho;
        MachoIteratorMandatum  iter;
              MandatumOnustum* cmd;
                          i32  numerus_iteratus;
                          b32  invenit_segment_64;
                          b32  invenit_symtab;
                          b32  invenit_dysymtab;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        numerus_iteratus    = ZEPHYRUM;
        invenit_segment_64  = FALSUM;
        invenit_symtab      = FALSUM;
        invenit_dysymtab    = FALSUM;

        iter = macho_iterator_mandatorum_initium(macho);
        dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
        {
                       i32 genus    = mandatum_genus(cmd);
            memoriae_index mensura  = mandatum_magnitudo(cmd);

            imprimere("  [%d] genus=0x%x mensura=%zu\n",
                      numerus_iteratus, genus, mensura);

            numerus_iteratus++;

            /* Verificare genus mandati */
            si (genus == MACHO_LC_SEGMENT_64) invenit_segment_64 =
                                                  VERUM;
            si (genus == MACHO_LC_SYMTAB) invenit_symtab         =
                                              VERUM;
            si (genus == MACHO_LC_DYSYMTAB) invenit_dysymtab     =
                                                VERUM;

            /* Verificare mensura rationabilis */
            CREDO_VERUM(mensura >= VIII);  /* Minimum load command size */

            /* Verificare datum non NIHIL */
            CREDO_NON_NIHIL(mandatum_datum(cmd));
        }

        /* Verificare numerus iteratus concordat cum header */
        CREDO_AEQUALIS_I32(numerus_iteratus,
            macho_numerus_mandatorum(macho));

        /* Verificare invenit mandata communia */
        CREDO_VERUM(invenit_segment_64);
        CREDO_VERUM(invenit_symtab);

        imprimere("  Totalis iteratus: %d\n", numerus_iteratus);
        imprimere("  Invenit LC_SEGMENT_64: %s\n",
            invenit_segment_64 ? "sic" : "non");
        imprimere("  Invenit LC_SYMTAB: %s\n",
            invenit_symtab ? "sic" : "non");
        imprimere("  Invenit LC_DYSYMTAB: %s\n",
            invenit_dysymtab ? "sic" : "non");
    }
}


    /* ==================================================
	 * Probare macho_iterator_mandatorum_finis
	 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           constans character* via = "probationes/fixa/simplex";
                        MachO* macho;
        MachoIteratorMandatum  iter;
              MandatumOnustum* cmd;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        iter = macho_iterator_mandatorum_initium(macho);

        /* Iterator non debet esse finis ad initium */
        CREDO_FALSUM(macho_iterator_mandatorum_finis(&iter));

        /* Iterat omnes */
        dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
        {
            /* nihil */
        }

        /* Nunc debet esse finis */
        CREDO_VERUM(macho_iterator_mandatorum_finis(&iter));

        /* Proximum debet reddere NIHIL */
        cmd = macho_iterator_mandatorum_proximum(&iter);
        CREDO_NIHIL(cmd);
    }
}


    /* ==================================================
	 * Probare concordantiam mandatum_datum
	 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           constans character* via = "probationes/fixa/simplex";
                        MachO* macho;
        MachoIteratorMandatum  iter;
              MandatumOnustum* cmd;
                          i32  numerus_verificatus;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        numerus_verificatus  = ZEPHYRUM;
        iter                 = macho_iterator_mandatorum_initium(macho);

        dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
        {
            constans i32* raw = (constans i32*)mandatum_datum(cmd);

            /* Prima duo i32 in raw data debent esse genus, mensura */
            CREDO_AEQUALIS_I32(raw[0], mandatum_genus(cmd));
            CREDO_AEQUALIS_I32(raw[1], (i32)mandatum_magnitudo(cmd));

            numerus_verificatus++;
        }

        imprimere("  Verificatus: %d mandata\n", numerus_verificatus);
        CREDO_VERUM(numerus_verificatus > ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare macho_filum_aperire
	 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via = "probationes/fixa/simplex";
                MachoFilum* filum;
                     MachO* imago;
                       i32  numerus;
                       i32  genus_cpu;
                       i32  subgenus_cpu;

        filum = macho_filum_aperire(via, piscina);
        CREDO_NON_NIHIL(filum);

        /* Verificare numerus imaginum */
        numerus = macho_filum_numerus_imago(filum);
        imprimere("  Numerus imaginum: %d\n", numerus);
        CREDO_AEQUALIS_I32(numerus, I);

        /* Obtinere imaginem */
        imago = macho_filum_imago(filum, ZEPHYRUM);
        CREDO_NON_NIHIL(imago);

        /* Verificare genus processoris */
        genus_cpu = macho_filum_genus_processoris(filum, ZEPHYRUM);
        imprimere("  CPU type: 0x%x\n", genus_cpu);
        CREDO_VERUM(genus_cpu == MACHO_CPU_TYPE_X86_64

            || genus_cpu == MACHO_CPU_TYPE_ARM64);

        /* Verificare subgenus */
        subgenus_cpu = macho_filum_subgenus_processoris(filum,
            ZEPHYRUM);
        imprimere("  CPU subtype: 0x%x\n", subgenus_cpu);

        /* Verificare imago ex filum concordat cum macho_aperire */
        CREDO_AEQUALIS_I32(macho_genus_processoris(imago), genus_cpu);
    }
}


    /* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
             MachO* macho;
        MachoFilum* filum;

        /* macho_aperire cum via NIHIL */
        macho = macho_aperire(NIHIL, piscina);
        CREDO_NIHIL(macho);

        /* macho_filum_aperire cum via NIHIL */
        filum = macho_filum_aperire(NIHIL, piscina);
        CREDO_NIHIL(filum);

        /* macho_ex_memoria cum datum NIHIL */
        macho = macho_ex_memoria(NIHIL, C, piscina);
        CREDO_NIHIL(macho);

        /* Interrogatio cum macho NIHIL */
        CREDO_AEQUALIS_I32(macho_genus_processoris(NIHIL), ZEPHYRUM);
        CREDO_AEQUALIS_I32(macho_numerus_mandatorum(NIHIL), ZEPHYRUM);
        CREDO_FALSUM(macho_est_64bit(NIHIL));
    }
}


    /* ==================================================
	 * Probare filum non existens
	 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via = "probationes/fixa/non_existens";
                     MachO* macho;
        constans character* error;

        macho = macho_aperire(via, piscina);
        CREDO_NIHIL(macho);

        error = macho_error_recens();
        imprimere("  Error: %s\n", error ? error : "(nullus)");
        CREDO_NON_NIHIL(error);
    }
}


    /* ==================================================
	 * Probare ambos fixtures habent structuram similem
	 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MachO* simplex;
        MachO* cum_functione;
          i32  num_simplex;
          i32  num_functione;

        simplex = macho_aperire("probationes/fixa/simplex", piscina);
        cum_functione = macho_aperire("probationes/fixa/cum_functione",
            piscina);

        CREDO_NON_NIHIL(simplex);
        CREDO_NON_NIHIL(cum_functione);

        /* Ambo debent esse 64-bit */
        CREDO_VERUM(macho_est_64bit(simplex));
        CREDO_VERUM(macho_est_64bit(cum_functione));

        /* Ambo debent esse executabiles */
        CREDO_AEQUALIS_I32(macho_genus_filum(simplex),
            MACHO_GENUS_EXECUTABILE);
        CREDO_AEQUALIS_I32(macho_genus_filum(cum_functione),
            MACHO_GENUS_EXECUTABILE);

        /* Ambo debent habere eundem genus processoris */
        CREDO_AEQUALIS_I32(macho_genus_processoris(simplex),
                           macho_genus_processoris(cum_functione));

        /* cum_functione debet habere plura mandata (habet symbola, etc) */
        num_simplex    = macho_numerus_mandatorum(simplex);
        num_functione  = macho_numerus_mandatorum(cum_functione);

        imprimere("  simplex mandata: %d\n", num_simplex);
        imprimere("  cum_functione mandata: %d\n", num_functione);

        /* Nota: non verificamus num_functione > num_simplex quia
		 * compiler potest optimizare similiter */
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "macho_aperire (simplex)", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "macho_aperire (cum_functione)", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "iterationem mandatorum oneris", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "macho_iterator_mandatorum_finis", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "concordantiam mandatum_datum", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "macho_filum_aperire", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "NIHIL handling", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum non existens", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Comparans ambos fixtures", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("macho", SECTIONES);
}
