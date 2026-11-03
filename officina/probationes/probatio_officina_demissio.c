/* probatio_officina_demissio.c - Demissio M1b A: plagulae
 * scalares + fluxus DEMITTUNTUR (sine sistere!), scripturae
 * aureae fixae, vectis lectoris byte-idem. Aurea absentia
 * PINGITUR (inspicienda ante commissionem). */

#include "officina_demissio.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via_partis)
{
    character via[CCLVI];
    FILE* plagula;
    chorda fructus;
    long longitudo;
    constans character* radix = getenv("RHUBARB_RADIX");

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde fructus;
    }
    fseek(plagula, 0L, SEEK_END);
    longitudo = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (longitudo > 0L)
    {
        fructus.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)longitudo);
        si (   fructus.datum != NIHIL
            && fread(fructus.datum, I, (memoriae_index)longitudo,
                   plagula) == (memoriae_index)longitudo)
        {
            fructus.mensura = (i32)longitudo;
        }
        alioquin
        {
            fructus.datum = NIHIL;
        }
    }
    fclose(plagula);
    redde fructus;
}

interior vacuum
_plagulam_scribere (
    constans character* via_partis,
                chorda  contenta)
{
    character via[CCLVI];
    FILE* plagula;
    constans character* radix = getenv("RHUBARB_RADIX");

    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = lectiones_fopen(via, "wb");
    si (plagula == NIHIL)
    {
        redde;
    }
    (vacuum)fwrite(contenta.datum, I,
        (memoriae_index)contenta.mensura, plagula);
    fclose(plagula);
}

interior MedullaModulus*
_demittere (
               Piscina* piscina,
          SilvaPiscina* piscina_silvae,
                chorda  fons,
    constans character* titulus)
{
    SilvaParsura* parsura = silva_c89_parsare(piscina_silvae,
        "probatio.c", (constans character*)fons.datum, fons.mensura,
        NIHIL);
    SilvaSemantica* sem;

    si (parsura == NIHIL || parsura->numerus_errorum != ZEPHYRUM)
    {
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare(piscina_silvae, parsura);
    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    redde demissio_currere(piscina, parsura, sem, _ch(titulus));
}

interior vacuum
_plagulam_probare (
               Piscina* piscina,
          SilvaPiscina* piscina_silvae,
    constans character* via_fontis,
    constans character* via_aurei,
    constans character* titulus)
{
            chorda fons = _plagulam_legere(piscina, via_fontis);
    MedullaModulus* modulus;
            chorda  scriptura;
               i32  i;
               i32  m;

    CREDO_VERUM (fons.mensura > ZEPHYRUM);
    modulus = _demittere(piscina, piscina_silvae, fons, titulus);
    CREDO_NON_NIHIL (modulus);
    si (modulus == NIHIL)
    {
        redde;
    }
    /* NULLA sistere: plagulae A plene demittuntur */
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(modulus->causae),
        ZEPHYRUM);
    /* omnes functiones terminatae */
    m = xar_numerus(modulus->functiones);
    CREDO_VERUM (m > ZEPHYRUM);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaFunctio** functio = (MedullaFunctio**)xar_obtinere(
            modulus->functiones, i);

        CREDO_VERUM (medulla_functio_terminata(*functio));
    }
    scriptura = medulla_textum_scribere(piscina, modulus);
    CREDO_VERUM (scriptura.mensura > ZEPHYRUM);
    /* vectis lectoris: scriptura byte-idem per circulum */
    {
                   i32  linea = ZEPHYRUM;
        MedullaModulus* iterum = medulla_textum_legere(piscina,
            scriptura, &linea);

        CREDO_NON_NIHIL (iterum);
        CREDO_AEQUALIS_I32 ((i32)linea, ZEPHYRUM);
        si (iterum != NIHIL)
        {
            CREDO_CHORDA_AEQUALIS (scriptura,
                medulla_textum_scribere(piscina, iterum));
        }
    }
    /* aureum: comparare aut pingere */
    {
        chorda aureum = _plagulam_legere(piscina, via_aurei);

        si (aureum.mensura == ZEPHYRUM)
        {
            _plagulam_scribere(via_aurei, scriptura);
            imprimere("AUREUM FIXUM: %s (inspicienda!)\n",
                via_aurei);
        }
        alioquin
        {
            CREDO_CHORDA_AEQUALIS (scriptura, aureum);
        }
    }
}

/* piscina silvae PROPRIA sectionis (olim una omnibus blocis) */
interior vacuum
_silvam_parare (
    CredoContextus* c)
{
    c->datum = silva_piscina_generare_dynamicum(
        "probatio_demissio_silva", 33554432);
    si (c->datum == NIHIL)
    {
        imprimere("FRACTA: piscina silvae\n");
    }
}

interior vacuum
_silvam_purgare (
    CredoContextus* c)
{
    si (c->datum != NIHIL)
    {
        silva_piscina_destruere((SilvaPiscina*)c->datum);
    }
}


/* ==================================================
 * PROBARE: plagulae demissae, aureae, vectis lectoris
 * ================================================== */

interior vacuum
_scalaria_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/scalaria.c",
        "officina/probationes/fixa/scalaria.medulla", "scalaria.c");
}

interior vacuum
_fluxum_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/fluxus.c",
        "officina/probationes/fixa/fluxus.medulla", "fluxus.c");
}

interior vacuum
_aggregata_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/aggregata.c",
        "officina/probationes/fixa/aggregata.medulla", "aggregata.c");
}

interior vacuum
_statica_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/statica.c",
        "officina/probationes/fixa/statica.medulla", "statica.c");
}


/* ==================================================
 * PROBARE: structura directa: duplum = functio prima scalariae
 * ================================================== */

interior vacuum
_structuram_probare (
    CredoContextus* c)
{
         Piscina* piscina         = c->piscina;
    SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        chorda fons = _plagulam_legere(piscina,
            "officina/probationes/fixa/scalaria.c");
        MedullaModulus* modulus = _demittere(piscina,
            piscina_silvae, fons, "scalaria.c");

        CREDO_NON_NIHIL (modulus);
        si (modulus != NIHIL)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    ZEPHYRUM);
               constans MedullaBloccus* bloccus;
            constans MedullaInstructio* ultima;
                                   i32  n;

            CREDO_NON_NIHIL (functio);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(
                (*functio)->parametra), I);
            bloccus = medulla_bloccum_obtinere(*functio, ZEPHYRUM);
            CREDO_NON_NIHIL ((constans vacuum*)bloccus);
            n = xar_numerus(bloccus->instructiones);
            CREDO_VERUM (n >= II);
            ultima = (constans MedullaInstructio*)xar_obtinere(
                bloccus->instructiones, n - I);
            CREDO_AEQUALIS_S32 (ultima->op, MEDULLA_OP_REDDE);
        }
    }
}


/* ==================================================
 * PROBARE: distillatio linearum (M2a): origo -> (via, linea)
 * ================================================== */

interior vacuum
_distillationem_probare (
    CredoContextus* c)
{
         Piscina* piscina         = c->piscina;
    SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        chorda fons = _plagulam_legere(piscina,
            "officina/probationes/fixa/scalaria.c");
        SilvaParsura* parsura = silva_c89_parsare(piscina_silvae,
            "probatio.c", (constans character*)fons.datum,
            fons.mensura, NIHIL);
        SilvaSemantica* sem;
        MedullaModulus* modulus;
        MedullaLineae* lineae;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina_silvae, parsura);
        CREDO_NON_NIHIL (sem);
        modulus = demissio_currere(piscina, parsura, sem,
            _ch("scalaria.c"));
        CREDO_NON_NIHIL (modulus);
        lineae = demissio_lineas_colligere(piscina, modulus,
            parsura);
        CREDO_NON_NIHIL (lineae);
        si (modulus != NIHIL && lineae != NIHIL)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    ZEPHYRUM);
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(*functio, ZEPHYRUM);
            constans MedullaInstructio* instructio =
                (constans MedullaInstructio*)xar_obtinere(
                    bloccus->instructiones, ZEPHYRUM);
            chorda via;
               i32 linea = ZEPHYRUM;

            CREDO_NON_NIHIL ((constans vacuum*)instructio->origo);
            CREDO_VERUM (medulla_lineam_quaerere(lineae,
                instructio->origo, &via, &linea));
            CREDO_VERUM (linea > ZEPHYRUM);
            CREDO_VERUM (chorda_aequalis_literis(via, "probatio.c"));
            /* origo alienum -> FALSUM */
            CREDO_FALSUM (medulla_lineam_quaerere(lineae,
                (constans structura SilvaNodus*)(constans vacuum*)
                    &via, NIHIL, NIHIL));
        }
    }
}


/* ==================================================
 * PROBARE: limina vocationum (M5 B4) - typi argumentorum,
 * argumenta_fixa, signaturae externorum, parametra aggregata
 * ================================================== */

interior constans MedullaFunctio*
_functionem_invenire (
    constans MedullaModulus* modulus,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(modulus->functiones); i++)
    {
        MedullaFunctio** functio = (MedullaFunctio**)xar_obtinere(
            modulus->functiones, i);

        si (chorda_aequalis_literis((*functio)->titulus, titulus))
        {
            redde *functio;
        }
    }
    redde NIHIL;
}

interior constans MedullaSignatura*
_signaturam_invenire (
    constans MedullaModulus* modulus,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(modulus->symbola); i++)
    {
        constans MedullaSymbolum* symbolum =
            medulla_symbolum_obtinere(modulus, (s32)i);

        si (   chorda_aequalis_literis(symbolum->titulus, titulus)
            && symbolum->signatura >= ZEPHYRUM)
        {
            redde (constans MedullaSignatura*)xar_obtinere(
                modulus->signaturae, (i32)symbolum->signatura);
        }
    }
    redde NIHIL;
}

/* vocatio n-ta (0-basis) functionis, ordine canonico */
interior constans MedullaInstructio*
_vocationem_invenire (
    constans MedullaFunctio* functio,
                        i32  n)
{
    i32 b;
    i32 visae = ZEPHYRUM;

    per (b = ZEPHYRUM; b < xar_numerus(functio->blocci); b++)
    {
        constans MedullaBloccus* bloccus = medulla_bloccum_obtinere(
            functio, (s32)b);
        i32 k;

        per (k = ZEPHYRUM; k < xar_numerus(bloccus->instructiones);
             k++)
        {
            constans MedullaInstructio* instructio =
                (constans MedullaInstructio*)xar_obtinere(
                    bloccus->instructiones, k);

            si (instructio->op == (s32)MEDULLA_OP_VOCARE)
            {
                si (visae == n)
                {
                    redde instructio;
                }
                visae++;
            }
        }
    }
    redde NIHIL;
}

interior constans MedullaTypusLiminis*
_typum_argumenti (
       constans MedullaFunctio* functio,
    constans MedullaInstructio* instructio,
                           i32  k)
{
    redde (constans MedullaTypusLiminis*)xar_obtinere(
        functio->operanda_typi, (i32)instructio->extra_index + k);
}

interior constans MedullaTypusLiminis*
_limen_signaturae (
    constans MedullaSignatura* signatura,
                          i32  k)
{
    redde (constans MedullaTypusLiminis*)xar_obtinere(
        signatura->parametra, k);
}

interior vacuum
_limina_probare (
    CredoContextus* c)
{
          Piscina* piscina         = c->piscina;
     SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;
           chorda  fons;
    MedullaModulus* modulus;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    fons = _plagulam_legere(piscina,
        "officina/probationes/fixa/aggregata.c");
    modulus = _demittere(piscina, piscina_silvae, fons,
        "aggregata.c");
    CREDO_NON_NIHIL (modulus);
    si (modulus == NIHIL)
    {
        redde;
    }
    /* $voca: externa(n + 1, 2.5) + scribens(2, n, 7L) */
    {
        constans MedullaFunctio* voca = _functionem_invenire(modulus,
            "voca");
        constans MedullaInstructio* externa;
        constans MedullaInstructio* scribens;

        CREDO_NON_NIHIL ((constans vacuum*)voca);
        si (voca == NIHIL)
        {
            redde;
        }
        externa   = _vocationem_invenire(voca, ZEPHYRUM);
        scribens  = _vocationem_invenire(voca, I);
        CREDO_NON_NIHIL ((constans vacuum*)externa);
        CREDO_NON_NIHIL ((constans vacuum*)scribens);
        si (externa == NIHIL || scribens == NIHIL)
        {
            redde;
        }
        CREDO_AEQUALIS_S32 (externa->argumenta_fixa, -I);
        CREDO_AEQUALIS_S32 (scribens->argumenta_fixa, I);
        CREDO_AEQUALIS_S32 (_typum_argumenti(voca, externa,
            ZEPHYRUM)->typus, MEDULLA_TYPUS_S32);
        CREDO_AEQUALIS_S32 (_typum_argumenti(voca, externa, I)->typus,
            MEDULLA_TYPUS_F64);
        /* variadica: typi promoti (7L = s64) */
        CREDO_AEQUALIS_S32 (_typum_argumenti(voca, scribens,
            II)->typus, MEDULLA_TYPUS_S64);
    }
    /* $facere declarata solum: signatura (r8, s32, s32) -> vacuum */
    {
        constans MedullaSignatura* facere = _signaturam_invenire(
            modulus, "facere");

        CREDO_NON_NIHIL ((constans vacuum*)facere);
        si (facere == NIHIL)
        {
            redde;
        }
        CREDO_AEQUALIS_S32 (facere->typus_reditus,
            MEDULLA_TYPUS_NIHIL);
        CREDO_FALSUM (facere->est_variadica);
        CREDO_AEQUALIS_I32 (xar_numerus(facere->parametra), III);
        CREDO_VERUM (_limen_signaturae(facere, ZEPHYRUM)->est_reditus);
        CREDO_AEQUALIS_I32 (_limen_signaturae(facere,
            ZEPHYRUM)->magnitudo_octetorum, VIII);
        CREDO_AEQUALIS_S32 (_limen_signaturae(facere, I)->typus,
            MEDULLA_TYPUS_S32);
        CREDO_AEQUALIS_S32 (_limen_signaturae(facere, II)->typus,
            MEDULLA_TYPUS_S32);
    }
    /* $duplicare: %reditus + aggregatum valore a8 */
    {
        constans MedullaFunctio* duplicare = _functionem_invenire(
            modulus, "duplicare");
        constans MedullaParametrum* p0;
        constans MedullaParametrum* p1;

        CREDO_NON_NIHIL ((constans vacuum*)duplicare);
        si (duplicare == NIHIL)
        {
            redde;
        }
        p0 = (constans MedullaParametrum*)xar_obtinere(
            duplicare->parametra, ZEPHYRUM);
        p1 = (constans MedullaParametrum*)xar_obtinere(
            duplicare->parametra, I);
        CREDO_VERUM (p0->limen.est_reditus);
        CREDO_AEQUALIS_S32 (p1->limen.typus, MEDULLA_TYPUS_AGGREGATUM);
        CREDO_AEQUALIS_I32 (p1->limen.magnitudo_octetorum, VIII);
        CREDO_FALSUM (p1->limen.est_reditus);
    }
}

/* HFA (AAPCS64): 1-4 membra eiusdem typi fluitantis; aliter aN */
interior vacuum
_hfa_probare (
    CredoContextus* c)
{
               Piscina* piscina         = c->piscina;
          SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;
    constans character* fons_literis =
        "struct V { double x, y; };\n"
        "struct Q { float a[3]; float b; };\n"
        "struct T { long a, b, c; };\n"
        "struct M { double d; float f; };\n"
        "struct V f(struct V v);\n"
        "long g(struct T t, struct Q q, struct M m);\n"
        "double h(void)\n"
        "{\n"
        "    struct V v; struct T t; struct Q q; struct M m;\n"
        "    v.x = 1.0; v.y = 2.0; t.a = 1; t.b = 2; t.c = 3;\n"
        "    return f(v).x + (double)g(t, q, m);\n"
        "}\n";
    MedullaModulus* modulus;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    modulus = _demittere(piscina, piscina_silvae, _ch(fons_literis),
        "hfa.c");
    CREDO_NON_NIHIL (modulus);
    si (modulus == NIHIL)
    {
        redde;
    }
    {
        constans MedullaSignatura* f = _signaturam_invenire(modulus,
            "f");
        constans MedullaSignatura* g = _signaturam_invenire(modulus,
            "g");

        CREDO_NON_NIHIL ((constans vacuum*)f);
        CREDO_NON_NIHIL ((constans vacuum*)g);
        si (f == NIHIL || g == NIHIL)
        {
            redde;
        }
        /* f: (rh2xf64, h2xf64) -> vacuum */
        CREDO_AEQUALIS_I32 (xar_numerus(f->parametra), II);
        CREDO_VERUM (_limen_signaturae(f, ZEPHYRUM)->est_reditus);
        CREDO_AEQUALIS_S32 (_limen_signaturae(f, ZEPHYRUM)->hfa_typus,
            MEDULLA_TYPUS_F64);
        CREDO_AEQUALIS_S32 (_limen_signaturae(f, I)->hfa_typus,
            MEDULLA_TYPUS_F64);
        CREDO_AEQUALIS_I32 (_limen_signaturae(f,
            I)->magnitudo_octetorum, XVI);
        /* g: (a24, h4xf32, a16) -> s64 */
        CREDO_AEQUALIS_S32 (g->typus_reditus, MEDULLA_TYPUS_S64);
        CREDO_AEQUALIS_S32 (_limen_signaturae(g, ZEPHYRUM)->typus,
            MEDULLA_TYPUS_AGGREGATUM);
        CREDO_AEQUALIS_I32 (_limen_signaturae(g,
            ZEPHYRUM)->magnitudo_octetorum, XXIV);
        CREDO_AEQUALIS_S32 (_limen_signaturae(g, ZEPHYRUM)->hfa_typus,
            MEDULLA_TYPUS_NIHIL);
        CREDO_AEQUALIS_S32 (_limen_signaturae(g, I)->hfa_typus,
            MEDULLA_TYPUS_F32);
        CREDO_AEQUALIS_I32 (_limen_signaturae(g,
            I)->magnitudo_octetorum, XVI);
        /* membra mixta (f64 + f32) -> non HFA */
        CREDO_AEQUALIS_S32 (_limen_signaturae(g, II)->hfa_typus,
            MEDULLA_TYPUS_NIHIL);
        CREDO_AEQUALIS_I32 (_limen_signaturae(g,
            II)->magnitudo_octetorum, XVI);
    }
    /* vocatio f(v): argumenta typata eadem ac signatura */
    {
        constans MedullaFunctio* h = _functionem_invenire(modulus,
            "h");
        constans MedullaInstructio* vocatio_f;

        CREDO_NON_NIHIL ((constans vacuum*)h);
        si (h == NIHIL)
        {
            redde;
        }
        vocatio_f = _vocationem_invenire(h, ZEPHYRUM);
        CREDO_NON_NIHIL ((constans vacuum*)vocatio_f);
        si (vocatio_f == NIHIL)
        {
            redde;
        }
        CREDO_VERUM (_typum_argumenti(h, vocatio_f,
            ZEPHYRUM)->est_reditus);
        CREDO_AEQUALIS_S32 (_typum_argumenti(h, vocatio_f,
            I)->hfa_typus, MEDULLA_TYPUS_F64);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "demissionem: officina/probationes/fixa/scalaria.c",
        _scalaria_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/fluxus.c",
        _fluxum_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/aggregata.c",
        _aggregata_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/statica.c",
        _statica_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "structuram (duplum)", _structuram_probare, _silvam_parare,
        _silvam_purgare, NIHIL, NIHIL },
    { "distillationem linearum", _distillationem_probare,
        _silvam_parare, _silvam_purgare, NIHIL, NIHIL },
    { "limina", _limina_probare, _silvam_parare, _silvam_purgare,
        NIHIL, NIHIL },
    { "hfa", _hfa_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_demissio", SECTIONES);
}
