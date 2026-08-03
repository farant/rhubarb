/* officina_regio.c - Regio: memoria possessa machinulae (M2a)
 *
 * Allocator acervi: index liber simplex (primus-aptus, LIFO) cum
 * capitibus XVI-octetorum. Vexillum liberum in bit infimo
 * magnitudinis (magnitudines multipla XVI - bits infimi vacui).
 * Sine coalescentia v1: vocatores malloc in corpore CONCENTRATI
 * post piscinam (allocationes paucae, magnae - fundus-libc);
 * coalescentia = statio nominata, numeratores M2c decernent.
 *
 * Reusus LIFO: liberare-tum-allocare eiusdem magnitudinis EUNDEM
 * monstratorem reddit - determinismus probabilis (pinnatur in
 * probatione).
 */

#include "postulata_posix.h"
#include "officina_regio.h"

#include <string.h>
#include <sys/mman.h>

/* basis fixa: determinismus (interview Q9). 0x300000000000 =
 * spatium altum, liberum in macOS arm64; si mmap eam non dat,
 * regio_generare NIHIL reddit (numquam basim aliam tacite). */
#define REGIO_BASIS_VALOR   ((memoriae_index)0x300000000000UL)
#define REGIO_TOTA          ((memoriae_index)0x1000000000UL)  /* 64GB */
#define REGIO_GLOBALIA_MAG  ((memoriae_index)0x200000000UL)   /*  8GB */
#define REGIO_STIVA_MAG     ((memoriae_index)0x100000000UL)   /*  4GB */

#define ACERVUS_ORDINATIO   ((memoriae_index)16)
#define ACERVUS_VENENUM_ALLOCATIONIS  0xAA
#define ACERVUS_VENENUM_LIBERATIONIS  0xDD

/* caput ante quemque locellum acervi; XVI octeti - fructus manet
 * XVI-ordinatus */
nomen structura CaputAcervi CaputAcervi;
structura CaputAcervi {
    memoriae_index magnitudo_cum_vexillo;  /* payload; bit 0 = liber */
    CaputAcervi*   sequens_liber;          /* index liber (si liber) */
};

structura Regio {
    Piscina* piscina;
    i8*      basis;
    b32      custodia;

    /* globalia */
    i8*             globalia_initium;
    memoriae_index  globalia_cursor;       /* offset intra aream */

    /* stiva */
    i8*             stiva_initium;

    /* acervus */
    i8*             acervus_initium;
    memoriae_index  acervus_magnitudo;
    memoriae_index  acervus_cursor;        /* offset (fines cumuli) */
    CaputAcervi*    index_liber;

    /* census */
    memoriae_index  acervus_octeti_usi;
    memoriae_index  acervus_apex;
    memoriae_index  numerus_allocationum;
    memoriae_index  numerus_liberationum;
};

/* ==================================================
 * Vita
 * ================================================== */

Regio*
regio_generare (Piscina* piscina)
{
    Regio* regio;
    vacuum* datus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    datus = mmap((vacuum*)REGIO_BASIS_VALOR, REGIO_TOTA,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    si (datus == MAP_FAILED)
    {
        redde NIHIL;
    }
    si (datus != (vacuum*)REGIO_BASIS_VALOR)
    {
        /* basis aliena: determinismus periret - honeste recusa */
        munmap(datus, REGIO_TOTA);
        redde NIHIL;
    }

    regio = piscina_allocare(piscina, magnitudo(Regio));
    si (regio == NIHIL)
    {
        munmap(datus, REGIO_TOTA);
        redde NIHIL;
    }
    memset(regio, ZEPHYRUM, magnitudo(Regio));

    regio->piscina = piscina;
    regio->basis = (i8*)datus;
    regio->custodia = FALSUM;

    regio->globalia_initium = regio->basis;
    regio->globalia_cursor = ZEPHYRUM;

    regio->stiva_initium = regio->basis + REGIO_GLOBALIA_MAG;

    regio->acervus_initium = regio->stiva_initium + REGIO_STIVA_MAG;
    regio->acervus_magnitudo = REGIO_TOTA - REGIO_GLOBALIA_MAG
        - REGIO_STIVA_MAG;
    regio->acervus_cursor = ZEPHYRUM;
    regio->index_liber = NIHIL;

    redde regio;
}

vacuum
regio_destruere (Regio* regio)
{
    si (regio == NIHIL)
    {
        redde;
    }
    munmap((vacuum*)regio->basis, REGIO_TOTA);
    regio->basis = NIHIL;
}

/* ==================================================
 * Fines + custodia
 * ================================================== */

vacuum*
regio_basis (constans Regio* regio)
{
    redde (vacuum*)regio->basis;
}

memoriae_index
regio_magnitudo_tota (constans Regio* regio)
{
    (vacuum)regio;
    redde REGIO_TOTA;
}

b32
regio_continet (constans Regio* regio, constans vacuum* locus)
{
    constans i8* p = (constans i8*)locus;

    si (regio == NIHIL || regio->basis == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)(p >= regio->basis && p < regio->basis + REGIO_TOTA);
}

vacuum
regio_custodiam_ponere (Regio* regio, b32 custodia)
{
    regio->custodia = custodia;
}

b32
regio_custodia (constans Regio* regio)
{
    redde regio->custodia;
}

/* ==================================================
 * Globalia
 * ================================================== */

vacuum*
regio_globalia_allocare (Regio* regio,
    memoriae_index magnitudo_octetorum, memoriae_index ordinatio)
{
    memoriae_index cursor;
    i8* fructus;

    si (regio == NIHIL || magnitudo_octetorum == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (ordinatio == ZEPHYRUM)
    {
        ordinatio = I;
    }

    /* rotundare cursorem ad ordinationem (potestas duorum) */
    cursor = (regio->globalia_cursor + ordinatio - I)
        & ~(ordinatio - I);
    si (cursor + magnitudo_octetorum > REGIO_GLOBALIA_MAG)
    {
        redde NIHIL;
    }

    fructus = regio->globalia_initium + cursor;
    regio->globalia_cursor = cursor + magnitudo_octetorum;
    redde (vacuum*)fructus;
}

memoriae_index
regio_globalia_usus (constans Regio* regio)
{
    redde regio->globalia_cursor;
}

/* ==================================================
 * Stiva
 * ================================================== */

vacuum*
regio_stiva_initium (constans Regio* regio)
{
    redde (vacuum*)regio->stiva_initium;
}

memoriae_index
regio_stiva_magnitudo_octetorum (constans Regio* regio)
{
    (vacuum)regio;
    redde REGIO_STIVA_MAG;
}

/* ==================================================
 * Acervus
 * ================================================== */

interior memoriae_index
_rotundare_xvi (memoriae_index n)
{
    redde (n + ACERVUS_ORDINATIO - I) & ~(ACERVUS_ORDINATIO - I);
}

interior CaputAcervi*
_caput_de_loco (vacuum* locus)
{
    redde (CaputAcervi*)(vacuum*)((i8*)locus
        - magnitudo(CaputAcervi));
}

interior vacuum*
_locus_de_capite (CaputAcervi* caput)
{
    redde (vacuum*)((i8*)(vacuum*)caput
        + magnitudo(CaputAcervi));
}

interior memoriae_index
_caput_magnitudo (constans CaputAcervi* caput)
{
    redde caput->magnitudo_cum_vexillo & ~(memoriae_index)I;
}

interior b32
_caput_liber (constans CaputAcervi* caput)
{
    redde (b32)(caput->magnitudo_cum_vexillo & (memoriae_index)I);
}

/* validitas capitis: intra acervum usum, magnitudo sana */
interior b32
_caput_validum (constans Regio* regio, constans CaputAcervi* caput)
{
    constans i8* p = (constans i8*)(constans vacuum*)caput;
    memoriae_index m;

    si (p < regio->acervus_initium
        || p >= regio->acervus_initium + regio->acervus_cursor)
    {
        redde FALSUM;
    }
    si ((memoriae_index)(p - regio->acervus_initium)
        % ACERVUS_ORDINATIO != ZEPHYRUM)
    {
        redde FALSUM;
    }
    m = _caput_magnitudo(caput);
    si (m == ZEPHYRUM || m % ACERVUS_ORDINATIO != ZEPHYRUM
        || m > regio->acervus_cursor)
    {
        redde FALSUM;
    }
    redde VERUM;
}

vacuum*
regio_allocare (Regio* regio, memoriae_index n)
{
    CaputAcervi* caput;
    CaputAcervi* prior;
    vacuum* fructus;
    memoriae_index rotundata;

    si (regio == NIHIL || n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    rotundata = _rotundare_xvi(n);

    /* primus-aptus in indice libero */
    prior = NIHIL;
    caput = regio->index_liber;
    dum (caput != NIHIL)
    {
        si (_caput_magnitudo(caput) >= rotundata)
        {
            frange;
        }
        prior = caput;
        caput = caput->sequens_liber;
    }

    si (caput != NIHIL)
    {
        /* evellere de indice */
        si (prior == NIHIL)
        {
            regio->index_liber = caput->sequens_liber;
        }
        alioquin
        {
            prior->sequens_liber = caput->sequens_liber;
        }

        /* scindere si residuum locellum integrum fert */
        si (_caput_magnitudo(caput) >= rotundata
            + magnitudo(CaputAcervi) + ACERVUS_ORDINATIO)
        {
            CaputAcervi* residuum;
            memoriae_index magnitudo_residui;

            magnitudo_residui = _caput_magnitudo(caput) - rotundata
                - magnitudo(CaputAcervi);
            residuum = (CaputAcervi*)(vacuum*)((i8*)_locus_de_capite(caput)
                + rotundata);
            residuum->magnitudo_cum_vexillo = magnitudo_residui
                | (memoriae_index)I;
            residuum->sequens_liber = regio->index_liber;
            regio->index_liber = residuum;

            caput->magnitudo_cum_vexillo = rotundata;
        }
        alioquin
        {
            caput->magnitudo_cum_vexillo = _caput_magnitudo(caput);
        }
        caput->sequens_liber = NIHIL;
    }
    alioquin
    {
        /* cumulus crescit */
        memoriae_index opus = magnitudo(CaputAcervi) + rotundata;

        si (regio->acervus_cursor + opus > regio->acervus_magnitudo)
        {
            redde NIHIL;
        }
        caput = (CaputAcervi*)(vacuum*)(regio->acervus_initium
            + regio->acervus_cursor);
        regio->acervus_cursor += opus;
        caput->magnitudo_cum_vexillo = rotundata;
        caput->sequens_liber = NIHIL;
    }

    fructus = _locus_de_capite(caput);
    regio->acervus_octeti_usi += _caput_magnitudo(caput);
    si (regio->acervus_octeti_usi > regio->acervus_apex)
    {
        regio->acervus_apex = regio->acervus_octeti_usi;
    }
    regio->numerus_allocationum += I;

    si (regio->custodia)
    {
        memset(fructus, ACERVUS_VENENUM_ALLOCATIONIS,
            _caput_magnitudo(caput));
    }
    redde fructus;
}

b32
regio_liberare (Regio* regio, vacuum* locus)
{
    CaputAcervi* caput;

    si (regio == NIHIL || locus == NIHIL)
    {
        redde FALSUM;
    }
    caput = _caput_de_loco(locus);
    si (!_caput_validum(regio, caput) || _caput_liber(caput))
    {
        redde FALSUM;
    }

    si (regio->custodia)
    {
        memset(locus, ACERVUS_VENENUM_LIBERATIONIS,
            _caput_magnitudo(caput));
    }

    regio->acervus_octeti_usi -= _caput_magnitudo(caput);
    regio->numerus_liberationum += I;

    caput->magnitudo_cum_vexillo |= (memoriae_index)I;
    caput->sequens_liber = regio->index_liber;
    regio->index_liber = caput;
    redde VERUM;
}

vacuum*
regio_reallocare (Regio* regio, vacuum* locus, memoriae_index n)
{
    CaputAcervi* caput;
    memoriae_index vetus;
    vacuum* novus;

    si (regio == NIHIL)
    {
        redde NIHIL;
    }
    si (locus == NIHIL)
    {
        redde regio_allocare(regio, n);
    }
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }

    caput = _caput_de_loco(locus);
    si (!_caput_validum(regio, caput) || _caput_liber(caput))
    {
        redde NIHIL;
    }
    vetus = _caput_magnitudo(caput);

    si (_rotundare_xvi(n) <= vetus)
    {
        /* contractio: idem monstrator (pinnatur in probatione) */
        redde locus;
    }

    novus = regio_allocare(regio, n);
    si (novus == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(novus, locus, vetus);
    regio_liberare(regio, locus);
    redde novus;
}

memoriae_index
regio_acervus_usus (constans Regio* regio)
{
    redde regio->acervus_octeti_usi;
}

memoriae_index
regio_acervus_apex (constans Regio* regio)
{
    redde regio->acervus_apex;
}

memoriae_index
regio_numerus_allocationum (constans Regio* regio)
{
    redde regio->numerus_allocationum;
}

memoriae_index
regio_numerus_liberationum (constans Regio* regio)
{
    redde regio->numerus_liberationum;
}
