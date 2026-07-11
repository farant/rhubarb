/* vindex_onerator.c - onerator mundi app-lateris (vide .h) */

#include "vindex_onerator.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

structura VindexOnerator {
    Piscina*         piscina_ctx;      /* app: xar, buffers */
    OfficinaPiscina* piscina_off;      /* moduli, lineae, indicium */
    SilvaPiscina*    piscina_silvae;   /* ctx + systema */
    SilvaContextus*  ctx;
    SilvaParsura*    systema_parsura;
    SilvaSemantica*  systema_semantica;
    Xar*             moduli;           /* MedullaModulus* */
    Xar*             lineae;           /* MedullaLineae* */
    IndiciumScriptor* scriptor;
    IndiciumLector*  lector;
    Conexio*         conexio;
    character        radix[512];
};

interior OfficinaChorda
_och (constans character* literis)
{
    OfficinaChorda c;
    unio { constans character* c; insignatus character* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (insignatus integer)strlen(literis);
    redde c;
}

interior character*
_legere_cruda (Piscina* piscina, constans character* via,
    long* mensura_fructus)
{
    FILE* pl = fopen(via, "rb");
    long mensura;
    character* fons;

    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (fons == NIHIL
        || fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    fons[mensura] = '\0';
    *mensura_fructus = mensura;
    redde fons;
}

/* ==================================================
 * capita praeparare (exemplar interpretare/vectis)
 * ================================================== */

interior b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior vacuum
_caput_praebere (VindexOnerator* o, TabulaDispersa* visa,
    constans character* via, constans character* titulus)
{
    long mensura_l = 0L;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, o->piscina_ctx);
    si (tabula_dispersa_continet(visa, clavis)) redde;
    textus = _legere_cruda(o->piscina_ctx, via, &mensura_l);
    si (textus == NIHIL) redde;
    si (silva_contextus_praebere(o->ctx, titulus, textus,
            (insignatus integer)mensura_l))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

interior vacuum
_capita_praeparare (VindexOnerator* o, TabulaDispersa* visa,
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(o, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_praebere(o, visa, via_plena,
                    introitus->d_name);
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * API
 * ================================================== */

VindexOnerator*
vindex_onerator_creare (constans character* radix)
{
    VindexOnerator* o;
    Piscina* piscina_ctx = piscina_generare_dynamicum(
        "vindex_onerator", 8388608);

    si (piscina_ctx == NIHIL) redde NIHIL;
    o = piscina_allocare(piscina_ctx,
        magnitudo(VindexOnerator));
    si (o == NIHIL) redde NIHIL;
    memset(o, ZEPHYRUM, magnitudo(VindexOnerator));
    o->piscina_ctx = piscina_ctx;
    si (strlen(radix) >= magnitudo(o->radix)) redde NIHIL;
    strcpy(o->radix, radix);
    o->piscina_off = officina_piscina_generare_dynamicum(
        "vindex_off", 33554432);
    o->piscina_silvae = silva_piscina_generare_dynamicum(
        "vindex_silva", 8388608);
    si (o->piscina_off == NIHIL || o->piscina_silvae == NIHIL)
    {
        redde NIHIL;
    }
    o->ctx = silva_contextus_creare(o->piscina_silvae);
    o->moduli = xar_creare(piscina_ctx,
        (i32)magnitudo(vacuum*));
    o->lineae = xar_creare(piscina_ctx,
        (i32)magnitudo(vacuum*));
    o->scriptor = indicium_scriptor_creare(o->piscina_off);
    si (o->ctx == NIHIL || o->moduli == NIHIL
        || o->lineae == NIHIL || o->scriptor == NIHIL)
    {
        redde NIHIL;
    }

    /* systema ISO+POSIX concatenatum */
    {
        character via_iso[600];
        character via_posix[600];
        long m_iso = 0L;
        long m_posix = 0L;
        character* fons_iso;
        character* fons_posix;
        character* fons_sys;
        long mensura_sys;

        sprintf(via_iso, "%s/silva/fontes/systema_c89.h",
            o->radix);
        sprintf(via_posix, "%s/silva/fontes/systema_posix.h",
            o->radix);
        fons_iso = _legere_cruda(piscina_ctx, via_iso, &m_iso);
        fons_posix = _legere_cruda(piscina_ctx, via_posix,
            &m_posix);
        si (fons_iso == NIHIL || fons_posix == NIHIL)
        {
            redde NIHIL;
        }
        mensura_sys = m_iso + 1L + m_posix;
        fons_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(mensura_sys + 1L));
        si (fons_sys == NIHIL) redde NIHIL;
        memcpy(fons_sys, fons_iso, (memoriae_index)m_iso);
        fons_sys[m_iso] = '\n';
        memcpy(fons_sys + m_iso + 1L, fons_posix,
            (memoriae_index)m_posix);
        si (!silva_contextus_lexicon_addere(o->ctx,
                "systema_c89.h", fons_sys,
                (insignatus integer)mensura_sys))
        {
            redde NIHIL;
        }
        o->systema_parsura = silva_c89_parsare(o->piscina_silvae,
            "systema_c89.h", fons_sys,
            (insignatus integer)mensura_sys, NIHIL);
        si (o->systema_parsura == NIHIL
            || o->systema_parsura->numerus_errorum > 0U)
        {
            redde NIHIL;
        }
        o->systema_semantica = silva_c89_semantica_analysare(
            o->piscina_silvae, o->systema_parsura);
        si (o->systema_semantica == NIHIL) redde NIHIL;
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL) redde NIHIL;
        _capita_praeparare(o, visa, o->radix);
    }
    redde o;
}

b32
vindex_onerator_demittere (VindexOnerator* o,
    constans character* via)
{
    character via_lecta[1024];
    long mensura_l = 0L;
    SilvaPiscina* piscina_arboris;
    character* fons;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    MedullaLineae* lineae = NIHIL;

    si (o == NIHIL || via == NIHIL) redde FALSUM;
    sprintf(via_lecta, "%s/%s", o->radix, via);
    piscina_arboris = silva_piscina_generare_dynamicum(
        "vindex_arbor", 8388608);
    si (piscina_arboris == NIHIL) redde FALSUM;
    fons = _legere_cruda(o->piscina_ctx, via_lecta, &mensura_l);
    si (fons == NIHIL)
    {
        silva_piscina_destruere(piscina_arboris);
        redde FALSUM;
    }
    oraculum = silva_oraculum_creare(piscina_arboris);
    si (oraculum != NIHIL && o->systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            o->systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris,
        o->ctx, via, fons, (insignatus integer)mensura_l,
        oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->numerus_errorum > 0U)
    {
        silva_piscina_destruere(piscina_arboris);
        redde FALSUM;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(
        piscina_arboris, parsura, o->systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(
            piscina_arboris, parsura, o->systema_parsura);
    }
    si (sem == NIHIL)
    {
        silva_piscina_destruere(piscina_arboris);
        redde FALSUM;
    }
    modulus = demissio_currere(o->piscina_off, parsura, sem,
        _och(via));
    si (modulus != NIHIL)
    {
        /* fenestra collige-ante-destrue: lineae + indicium */
        lineae = demissio_lineas_colligere(o->piscina_off, modulus,
            parsura);
        (vacuum)indicium_modulum_colligere(o->scriptor, modulus,
            parsura, sem);
    }
    silva_piscina_destruere(piscina_arboris);
    si (modulus == NIHIL) redde FALSUM;
    {
        vacuum** locellus = xar_addere(o->moduli);
        vacuum** locellus_l = xar_addere(o->lineae);

        si (locellus == NIHIL || locellus_l == NIHIL) redde FALSUM;
        *locellus = modulus;
        *locellus_l = lineae;
    }
    redde VERUM;
}

Machinula*
vindex_onerator_nectere (VindexOnerator* o,
    constans character* via_indicii)
{
    Regio* regio;
    Machinula* machinula;
    i32 k;

    si (o == NIHIL) redde NIHIL;
    regio = regio_generare(o->piscina_off);
    si (regio == NIHIL) redde NIHIL;
    o->conexio = conexio_creare(o->piscina_off, regio);
    si (o->conexio == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(o->moduli); k++)
    {
        si (!conexio_modulum_addere(o->conexio,
                *(MedullaModulus**)xar_obtinere(o->moduli, k)))
        {
            redde NIHIL;
        }
    }
    si (!conexio_nectere(o->conexio)) redde NIHIL;
    machinula = machinula_creare(o->piscina_off, o->conexio,
        regio);
    si (machinula == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(o->lineae); k++)
    {
        MedullaLineae* lineae_k = *(MedullaLineae**)xar_obtinere(
            o->lineae, k);

        si (lineae_k != NIHIL)
        {
            machinula_lineas_praebere(machinula, (integer)k,
                lineae_k);
        }
    }
    si (via_indicii != NIHIL)
    {
        si (!indicium_scribere(o->scriptor, o->conexio,
                via_indicii))
        {
            redde NIHIL;
        }
        o->lector = indicium_aperire(o->piscina_off, via_indicii);
        si (o->lector == NIHIL) redde NIHIL;
    }
    redde machinula;
}

IndiciumLector*
vindex_onerator_lector (VindexOnerator* o)
{
    redde (o != NIHIL) ? o->lector : NIHIL;
}

OfficinaChorda
vindex_onerator_querela (VindexOnerator* o)
{
    OfficinaChorda vacua;

    si (o != NIHIL && o->conexio != NIHIL)
    {
        constans OfficinaChorda* symbolum =
            conexio_querela_symbolum(o->conexio);

        si (symbolum != NIHIL)
        {
            redde *symbolum;
        }
    }
    vacua.datum = NIHIL;
    vacua.mensura = 0U;
    redde vacua;
}
