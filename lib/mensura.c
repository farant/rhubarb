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
#include "lectiones.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define MENSURA_CAMPI VII

/* Linea maxima diarii. Constans INTEGRA manet ut fgets eam sine
 * conversione accipiat - magnitudo() insignatum longum reddit et
 * conversio ad integrum monitum meret */
#define MENSURA_LINEA_MAXIMA (IV * MXXIV)

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
    sprintf(tabula, "%012lx",
        (insignatus longus)mensura_momentum_nunc());

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
    si (   !_campus_purus(mensura->sessio)
        || !_campus_purus(mensura->id)
        || !_campus_purus(mensura->parens)
        || !_campus_purus(mensura->titulus)
        || !_campus_purus(mensura->unitas))
    {
        redde vacua;
    }

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)CC);
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

    plagula = lectiones_fopen(via_diarii, "ab");
    si (plagula == NIHIL)
    {
        redde FALSUM;
    }

    scripta = fwrite(linea.datum, (size_t)I, (size_t)linea.mensura,
        plagula);
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

    plagula = lectiones_fopen(via_diarii, "rb");
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

        /* CURSUS (analytica T1): lineae 'cursus' genere suo */
        si (   linea.mensura > (i32)VII
            && memcmp(linea.datum, "cursus\t", (size_t)VII) == ZEPHYRUM)
        {
            CursusInitium initium;
              CursusFinis finis;
                      b32 est_finis;

            si (   !mensura_cursus_ex_linea(linea, &initium, &finis,
                       &est_finis, piscina)
                || !volumen_actum_appendere(volumen, "cursus", linea))
            {
                malformatae++;
                perge;
            }
            fructus.conditae++;
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
    plagula = lectiones_fopen(via_diarii, "wb");
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


/* ============================================================
 * CURSUS (analytica T1)
 * ============================================================ */

#define CURSUS_CAMPI_INITII  XIV
#define CURSUS_CAMPI_FINIS   XI

/* argv effugere: \\ \t \n \r - mandatum ut datum servatur */
interior vacuum
_effugere (
    ChordaAedificator* a,
               chorda  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        commutatio (c.datum[i])
        {
            casus '\\': chorda_aedificator_appendere_literis(a, "\\\\");
                frange;
            casus '\t': chorda_aedificator_appendere_literis(a, "\\t");
                frange;
            casus '\n': chorda_aedificator_appendere_literis(a, "\\n");
                frange;
            casus '\r': chorda_aedificator_appendere_literis(a, "\\r");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)c.datum[i]);
                frange;
        }
    }
}

interior chorda
_reddere_effugium (
     chorda  c,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(c.mensura + I));
                   i32 i;

    si (a == NIHIL)
    {
        redde c;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] == '\\' && i + I < c.mensura)
        {
            i++;
            commutatio (c.datum[i])
            {
                casus 't': chorda_aedificator_appendere_character(a,
                               '\t');
                    frange;
                casus 'n': chorda_aedificator_appendere_character(a,
                               '\n');
                    frange;
                casus 'r': chorda_aedificator_appendere_character(a,
                               '\r');
                    frange;
                ordinarius:
                    chorda_aedificator_appendere_character(a,
                        (character)c.datum[i]);
                    frange;
            }
            perge;
        }
        chorda_aedificator_appendere_character(a,
            (character)c.datum[i]);
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
_campum_addere (
    ChordaAedificator* a,
               chorda  c)
{
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_chorda(a, c);
}

interior vacuum
_numerum_addere (
    ChordaAedificator* a,
                  s64  n)
{
    character numeri[XXXII];

    sprintf(numeri, "%ld", (longus)n);
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_literis(a, numeri);
}

chorda
mensura_cursus_initium_ad_lineam (
    constans CursusInitium* i,
                   Piscina* piscina)
{
    ChordaAedificator* a;
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (i == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    si (   !_campus_purus(i->id) || !_campus_purus(i->parens)
        || !_campus_purus(i->sessio) || !_campus_purus(i->titulus)
        || !_campus_purus(i->vocator) || !_campus_purus(i->commissum)
        || !_campus_purus(i->ramus) || !_campus_purus(i->arbor)
        || !_campus_purus(i->machina) || i->id.mensura == ZEPHYRUM)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (a == NIHIL)
    {
        redde vacua;
    }
    chorda_aedificator_appendere_literis(a, "cursus\tinitium");
    _campum_addere(a, i->id);
    _campum_addere(a, i->parens);
    _campum_addere(a, i->sessio);
    _campum_addere(a, i->titulus);
    chorda_aedificator_appendere_character(a, '\t');
    _effugere(a, i->argv);
    _campum_addere(a, i->vocator);
    _campum_addere(a, i->commissum);
    _numerum_addere(a, i->sordidum ? (s64)I : (s64)ZEPHYRUM);
    _campum_addere(a, i->ramus);
    _campum_addere(a, i->arbor);
    _campum_addere(a, i->machina);
    _numerum_addere(a, i->initium);
    chorda_aedificator_appendere_character(a, '\n');
    redde chorda_aedificator_finire(a);
}

chorda
mensura_cursus_finis_ad_lineam (
    constans CursusFinis* f,
                 Piscina* piscina)
{
    ChordaAedificator* a;
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   f == NIHIL || piscina == NIHIL || !_campus_purus(f->id)
        || !_campus_purus(f->acta) || f->id.mensura == ZEPHYRUM)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CXXVIII);
    si (a == NIHIL)
    {
        redde vacua;
    }
    chorda_aedificator_appendere_literis(a, "cursus\tfinis");
    _campum_addere(a, f->id);
    _numerum_addere(a, f->momentum);
    _numerum_addere(a, f->duratio_ms);
    _numerum_addere(a, (s64)f->exitus);
    _numerum_addere(a, f->rss_maxima);
    _numerum_addere(a, f->cpu_usoris_ms);
    _numerum_addere(a, f->cpu_systematis_ms);
    _campum_addere(a, f->acta);
    _numerum_addere(a, f->acta_mensura);
    chorda_aedificator_appendere_character(a, '\n');
    redde chorda_aedificator_finire(a);
}

/* campos ad TAB scindere (maxime numerus); numerus inventus redditur */
interior i32
_campos_scindere (
    chorda  linea,
    chorda* campi,
       i32  maximum)
{
    i32 n        = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;
    i32 i;

    dum (   linea.mensura > ZEPHYRUM
         && (   linea.datum[linea.mensura - I] == '\n'
             || linea.datum[linea.mensura - I] == '\r'))
    {
        linea.mensura--;
    }
    per (i = ZEPHYRUM; i <= linea.mensura; i++)
    {
        si (i == linea.mensura || linea.datum[i] == '\t')
        {
            si (n >= maximum)
            {
                redde maximum + I;   /* campi nimii */
            }
            campi[n].datum    = linea.datum + initium;
            campi[n].mensura  = i - initium;
            n++;
            initium = i + I;
        }
    }
    redde n;
}

/* numerus integer ex campo; FALSUM si non numerus totus */
interior b32
_numerum_legere (
    chorda  campus,
       s64* exitus)
{
    character  tabula[XXXII];
    character* finis;

    si (campus.mensura == ZEPHYRUM || campus.mensura >= (i32)XXXII)
    {
        redde FALSUM;
    }
    memcpy(tabula, campus.datum, (size_t)campus.mensura);
    tabula[campus.mensura]  = '\0';
    *exitus                 = (s64)strtol(tabula, &finis, X);
    redde (finis != tabula && *finis == '\0') ? VERUM : FALSUM;
}

b32
mensura_cursus_ex_linea (
           chorda  linea,
    CursusInitium* initium_out,
      CursusFinis* finis_out,
              b32* est_finis_out,
          Piscina* piscina)
{
    chorda campi[CURSUS_CAMPI_INITII];
       i32 n;
       s64 numerus;

    si (   linea.datum == NIHIL || initium_out == NIHIL
        || finis_out   == NIHIL || est_finis_out == NIHIL
        || piscina     == NIHIL)
    {
        redde FALSUM;
    }
    n = _campos_scindere(linea, campi, (i32)CURSUS_CAMPI_INITII);
    si (n < II || !chorda_aequalis_literis(campi[ZEPHYRUM], "cursus"))
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(campi[I], "initium"))
    {
        si (n != (i32)CURSUS_CAMPI_INITII)
        {
            redde FALSUM;
        }
        memset(initium_out, ZEPHYRUM, magnitudo(CursusInitium));
        initium_out->id         = campi[II];
        initium_out->parens     = campi[III];
        initium_out->sessio     = campi[IV];
        initium_out->titulus    = campi[V];
        initium_out->argv       = _reddere_effugium(campi[VI], piscina);
        initium_out->vocator    = campi[VII];
        initium_out->commissum  = campi[VIII];
        si (!_numerum_legere(campi[IX], &numerus))
        {
            redde FALSUM;
        }
        initium_out->sordidum  = (numerus != (s64)ZEPHYRUM);
        initium_out->ramus     = campi[X];
        initium_out->arbor     = campi[XI];
        initium_out->machina   = campi[XII];
        si (!_numerum_legere(campi[XIII], &initium_out->initium))
        {
            redde FALSUM;
        }
        *est_finis_out = FALSUM;
        redde VERUM;
    }
    si (chorda_aequalis_literis(campi[I], "finis"))
    {
        si (n != (i32)CURSUS_CAMPI_FINIS)
        {
            redde FALSUM;
        }
        memset(finis_out, ZEPHYRUM, magnitudo(CursusFinis));
        finis_out->id = campi[II];
        si (   !_numerum_legere(campi[III], &finis_out->momentum)
            || !_numerum_legere(campi[IV], &finis_out->duratio_ms)
            || !_numerum_legere(campi[V], &numerus)
            || !_numerum_legere(campi[VI], &finis_out->rss_maxima)
            || !_numerum_legere(campi[VII], &finis_out->cpu_usoris_ms)
            || !_numerum_legere(campi[VIII],
                   &finis_out->cpu_systematis_ms)
            || !_numerum_legere(campi[X], &finis_out->acta_mensura))
        {
            redde FALSUM;
        }
        finis_out->exitus  = (s32)numerus;
        finis_out->acta    = campi[IX];
        *est_finis_out     = VERUM;
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_lineam_appendere (
    constans character* via_diarii,
                chorda  linea)
{
     FILE* plagula;
    size_t scripta;

    si (via_diarii == NIHIL || linea.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    plagula = lectiones_fopen(via_diarii, "ab");
    si (plagula == NIHIL)
    {
        redde FALSUM;
    }
    scripta = fwrite(linea.datum, (size_t)I, (size_t)linea.mensura,
        plagula);
    fclose(plagula);
    redde (scripta == (size_t)linea.mensura) ? VERUM : FALSUM;
}

b32
mensura_cursum_incipere (
        constans character* via_diarii,
    constans CursusInitium* initium,
                   Piscina* piscina)
{
    redde _lineam_appendere(via_diarii,
        mensura_cursus_initium_ad_lineam(initium, piscina));
}

b32
mensura_cursum_finire (
      constans character* via_diarii,
    constans CursusFinis* finis,
                 Piscina* piscina)
{
    redde _lineam_appendere(via_diarii,
        mensura_cursus_finis_ad_lineam(finis, piscina));
}

interior s32
_cursus_ordinare (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x = ((constans Cursus*)a)->initium.initium;
    s64 y = ((constans Cursus*)b)->initium.initium;

    redde (x < y) ? -(s32)I : (x > y) ? (s32)I : (s32)ZEPHYRUM;
}

Xar*
mensura_cursus_legere (
    Volumen* volumen,
    Piscina* piscina)
{
               Xar* acta;
               Xar* cursus;
    TabulaDispersa* per_id;
               i32  k;

    si (volumen == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    acta = volumen_acta_legere(volumen, (s64)ZEPHYRUM, piscina);
    si (acta == NIHIL)
    {
        redde NIHIL;
    }
    cursus = xar_creare(piscina, (i32)magnitudo(Cursus));
    per_id = tabula_dispersa_creare_chorda(piscina, CCLVI);
    si (cursus == NIHIL || per_id == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(acta); k++)
    {
        constans VolumenActum* actum = (constans VolumenActum*)
            xar_obtinere(acta, k);
                 CursusInitium  initium;
                   CursusFinis  finis;
                           b32  est_finis;
                        vacuum* inventum;

        si (   !chorda_aequalis_literis(actum->genus, "cursus")
            || !mensura_cursus_ex_linea(actum->datum, &initium, &finis,
                   &est_finis, piscina))
        {
            perge;
        }
        si (!est_finis)
        {
            Cursus* novus = (Cursus*)xar_addere(cursus);

            si (novus == NIHIL)
            {
                redde NIHIL;
            }
            memset(novus, ZEPHYRUM, magnitudo(Cursus));
            novus->initium  = initium;
            novus->status   = CURSUS_INTERRUPTUS;
            (vacuum)tabula_dispersa_inserere(per_id, initium.id, novus);
            perge;
        }
        si (tabula_dispersa_invenire(per_id, finis.id, &inventum))
        {
            Cursus* r = (Cursus*)inventum;

            r->finis   = finis;
            r->status  = (finis.exitus == ZEPHYRUM) ? CURSUS_TRANSIIT
                : CURSUS_FRACTUS;
        }
    }
    /* ordo initii; xar_ordinare stabilis (aequalitas: ordine actorum).
     * NB monstratores tabulae post ordinationem invalidi - non iam
     * adhibentur */
    xar_ordinare(cursus, _cursus_ordinare);
    redde cursus;
}
