/*
 * mensura.c - Mensurae nidificatae
 *
 * Horologium: gettimeofday. NON monotonum est (NTP saltare potest),
 * ergo delta negativa fieri potest. s64 eam MONSTRAT; typus
 * insignatus eam in numerum immanem verteret et ramum tacite
 * numquam currentem pareret (vide codicem 80).
 */

#include "postulata_posix.h"

#include "mensura.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define MENSURA_CAMPI VII

/* Linea maxima diarii. Constans INTEGRA manet ut fgets eam sine
 * conversione accipiat - magnitudo() insignatum longum reddit et
 * conversio ad integrum monitum meret */
#define MENSURA_LINEA_MAXIMA MMMMXCVI

/* Numerator intra processum: duae mensurae eodem millisecundo in
 * eodem processu aliter claves easdem haberent */
interior i32 _numerator = ZEPHYRUM;


/* ============================================================
 * Tempus et claves
 * ============================================================ */

s64
mensura_momentum_nunc (vacuum)
{
    structura timeval nunc;

    si (gettimeofday(&nunc, NIHIL) != 0)
    {
        redde (s64)0;
    }

    redde (s64)nunc.tv_sec * (s64)M + (s64)nunc.tv_usec / (s64)M;
}

chorda
mensura_sessionem_novam (
    Piscina* piscina)
{
    character tabula[XXXII];

    si (piscina == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }

    /* Tempore praefixa => ordo lexicalis = ordo temporis */
    sprintf(tabula, "%012lx", (insignatus longus)mensura_momentum_nunc());

    redde chorda_ex_literis(tabula, piscina);
}

chorda
mensura_id_novum (
    Piscina* piscina)
{
    character tabula[LXIV];

    si (piscina == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }

    _numerator++;

    sprintf(tabula, "%012lx-%x-%x",
            (insignatus longus)mensura_momentum_nunc(),
            (insignatus integer)getpid(),
            (insignatus integer)_numerator);

    redde chorda_ex_literis(tabula, piscina);
}


/* ============================================================
 * Forma lineae
 * ============================================================ */

interior b32
_campus_purus (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (   c.datum[i] == '\t' || c.datum[i] == '\n'
            || c.datum[i] == '\r')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

chorda
mensura_ad_lineam (
    constans Mensura* mensura,
             Piscina* piscina)
{
    ChordaAedificator* aedificator;
            character  numeri[LXIV];
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (mensura == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }

    /* Campus qui separatorem continet formam frangeret, et lector
     * eam TACITE male legeret - ergo recusamus */
    si (   !_campus_purus(mensura->sessio) || !_campus_purus(mensura->id)
        || !_campus_purus(mensura->parens) || !_campus_purus(mensura->titulus)
        || !_campus_purus(mensura->unitas))
    {
        redde vacua;
    }

    aedificator = chorda_aedificator_creare(piscina, (memoriae_index)CC);
    si (aedificator == NIHIL)
    {
        redde vacua;
    }

    chorda_aedificator_appendere_chorda(aedificator, mensura->sessio);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, mensura->id);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, mensura->parens);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, mensura->titulus);
    chorda_aedificator_appendere_character(aedificator, '\t');

    /* Sex figurae = microsecunda; locale "C" manet quia setlocale
     * numquam vocamus, ergo punctum decimale stabile est */
    sprintf(numeri, "%.6f", mensura->valor);
    chorda_aedificator_appendere_literis(aedificator, numeri);
    chorda_aedificator_appendere_character(aedificator, '\t');

    chorda_aedificator_appendere_chorda(aedificator, mensura->unitas);
    chorda_aedificator_appendere_character(aedificator, '\t');

    sprintf(numeri, "%ld", (longus)mensura->momentum);
    chorda_aedificator_appendere_literis(aedificator, numeri);
    chorda_aedificator_appendere_character(aedificator, '\n');

    redde chorda_aedificator_finire(aedificator);
}

b32
mensura_ex_linea (
     chorda  linea,
    Mensura* exitus,
    Piscina* piscina)
{
    chorda campi[MENSURA_CAMPI];
       i32 numerus_camporum;
    i32       i, initium;
    character  tabula[LXIV];
    character* finis;

    si (exitus == NIHIL || piscina == NIHIL || linea.datum == NIHIL)
    {
        redde FALSUM;
    }

    /* Lineam novam terminalem tollere */
    dum (   linea.mensura > ZEPHYRUM
         && (linea.datum[linea.mensura - I] == '\n'
        || linea.datum[linea.mensura - I] == '\r'))
    {
        linea.mensura--;
    }

    numerus_camporum  = ZEPHYRUM;
    initium           = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= linea.mensura; i++)
    {
        si (i == linea.mensura || linea.datum[i] == '\t')
        {
            si (numerus_camporum >= (i32)MENSURA_CAMPI)
            {
                redde FALSUM;   /* campi nimii */
            }
            campi[numerus_camporum].datum    = linea.datum + initium;
            campi[numerus_camporum].mensura  = i - initium;
            numerus_camporum++;
            initium = i + I;
        }
    }

    si (numerus_camporum != (i32)MENSURA_CAMPI)
    {
        redde FALSUM;
    }

    exitus->sessio   = campi[0];
    exitus->id       = campi[I];
    exitus->parens   = campi[II];
    exitus->titulus  = campi[III];
    exitus->unitas   = campi[V];

    /* valor */
    si (campi[IV].mensura >= (i32)LXIV)
    {
        redde FALSUM;
    }
    memcpy(tabula, campi[IV].datum, (size_t)campi[IV].mensura);
    tabula[campi[IV].mensura]  = '\0';
    exitus->valor              = strtod(tabula, &finis);
    si (finis == tabula)
    {
        redde FALSUM;   /* nullus numerus */
    }

    /* momentum */
    si (campi[VI].mensura >= (i32)LXIV)
    {
        redde FALSUM;
    }
    memcpy(tabula, campi[VI].datum, (size_t)campi[VI].mensura);
    tabula[campi[VI].mensura]  = '\0';
    exitus->momentum           = (s64)strtol(tabula, &finis, X);
    si (finis == tabula)
    {
        redde FALSUM;
    }

    redde VERUM;
}


/* ============================================================
 * Diarium
 * ============================================================ */

b32
mensura_annotare (
                constans character* via_diarii,
                  constans Mensura* mensura,
                           Piscina* piscina)
{
    chorda linea;
    FILE*  plagula;
    size_t scripta;

    si (via_diarii == NIHIL || mensura == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }

    linea = mensura_ad_lineam(mensura, piscina);
    si (linea.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    plagula = fopen(via_diarii, "ab");
    si (plagula == NIHIL)
    {
        redde FALSUM;
    }

    scripta = fwrite(linea.datum, (size_t)I, (size_t)linea.mensura, plagula);
    fclose(plagula);

    redde (scripta == (size_t)linea.mensura) ? VERUM : FALSUM;
}


/* ============================================================
 * Conditio in volumen
 * ============================================================ */

MensuraFructus
mensura_condere (
      constans character* via_diarii,
                 Volumen* volumen,
                 Piscina* piscina)
{
    MensuraFructus fructus;
    FILE*          plagula;
    character      tabula[MENSURA_LINEA_MAXIMA];
    i32            malformatae;

    fructus.successus      = FALSUM;
    fructus.conditae       = ZEPHYRUM;
    fructus.causa.datum    = NIHIL;
    fructus.causa.mensura  = ZEPHYRUM;

    si (via_diarii == NIHIL || volumen == NIHIL || piscina == NIHIL)
    {
        redde fructus;
    }

    plagula = fopen(via_diarii, "rb");
    si (plagula == NIHIL)
    {
        fructus.causa = chorda_ex_literis(
            "Diarium aperiri non potest", piscina);
        redde fructus;
    }

    malformatae = ZEPHYRUM;

    si (!volumen_transactionem_incipere(volumen))
    {
        fclose(plagula);
        fructus.causa = chorda_ex_literis(
            "Transactio incipi non potest", piscina);
        redde fructus;
    }

    dum (fgets(tabula, (integer)magnitudo(tabula), plagula) != NIHIL)
    {
         chorda linea;
        Mensura mensura;

        linea.datum    = (i8*)tabula;
        linea.mensura  = (i32)strlen(tabula);

        /* Terminatorem lineae NON condere: actum in volumine
         * recordum est, non linea plagulae - qui postea quaerit
         * eum tacite in ultimo campo inveniret */
        dum (   linea.mensura > ZEPHYRUM
             && (tabula[linea.mensura - I] == '\n'
            || tabula[linea.mensura - I] == '\r'))
        {
            linea.mensura--;
        }

        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }

        si (!mensura_ex_linea(linea, &mensura, piscina))
        {
            malformatae++;
            perge;
        }

        si (volumen_actum_appendere(volumen, "mensura", linea))
        {
            fructus.conditae++;
        }
        alioquin
        {
            malformatae++;
        }
    }

    fclose(plagula);

    si (!volumen_transactionem_committere(volumen))
    {
        fructus.causa = chorda_ex_literis(
            "Transactio committi non potest", piscina);
        redde fructus;
    }

    /* Diarium conditum truncare: aliter conditio proxima easdem
     * mensuras iterum conderet */
    plagula = fopen(via_diarii, "wb");
    si (plagula != NIHIL)
    {
        fclose(plagula);
    }

    fructus.successus = VERUM;

    si (malformatae > ZEPHYRUM)
    {
        character nuntius[C];
        sprintf(nuntius, "%d lineae malformatae praeteritae",
                (integer)malformatae);
        fructus.causa = chorda_ex_literis(nuntius, piscina);
    }

    redde fructus;
}
