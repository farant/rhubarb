/* probatio_oratio_vocabularium_en.c - Vocabularium Anglicum ex Moby (T15a)
 *
 * I.   FONS venditus: octeti pinnati (FONTES.md), sigillum SHA-256
 *      pinnatum, CRLF ultimum.
 * II.  ONUS: census pinnatus (recorda, verba, locutiones, octeti alti,
 *      codices ignoti I = 'cowardic'), tempus publicatum.
 * III. QUAESTIO: the Dv; engineer Nt; running AN; well-known A; aa duo
 *      (AA, aa) ordine plagulae; of duo (OF N, of P); THE plicatum;
 *      xyzzy vacuum; locutio 'a la mode' numquam; cowardic Ne.
 * IV.  CLASSES: litterae legendae -> classes universales; 'e' NIHIL.
 * V.   LEX RECORDI: fontes inlineares mali sistunt cum linea nominata
 *      (signum deest, signa plura, LF solum, forma vacua, codices vacui,
 *      linea ultima sine CRLF, linea vacua); fons bonus inlinearis
 *      oneratur et quaeritur.
 * Culpa plantata: littera '!' e legenda sublata (codices ignoti
 * CDXXXIV pro I).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_vocabularium_en.h"
#include "oratio_vocabularium.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OCTETI_PINNA          3237558
#define SIGILLUM_PINNA        "cc81458b820a3625"
#define RECORDA_PINNA         233356
#define VERBA_PINNA           197387
#define LOCUTIONES_PINNA      35969
#define OCTETI_ALTI_PINNA     1231
#define CODICES_IGNOTI_PINNA  1

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior chorda
_l (
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;
    chorda c;

    u.c        = literae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literae);
    redde c;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

/* codices recordi k-ti quaesitionis ("" si deest) */
interior chorda
_codices (
    constans OratioVocabulariumEn* voc,
                              Xar* x,
                              i32  k)
{
    si (x == NIHIL || k >= xar_numerus(x))
    {
        redde _l("");
    }
    redde oratio_vocabularium_en_recordum(voc,
        *(s32*)xar_obtinere(x, k))->codices;
}

interior chorda
_forma (
    constans OratioVocabulariumEn* voc,
                              Xar* x,
                              i32  k)
{
    si (x == NIHIL || k >= xar_numerus(x))
    {
        redde _l("");
    }
    redde oratio_vocabularium_en_recordum(voc,
        *(s32*)xar_obtinere(x, k))->forma;
}

interior constans OratioAnalysisEn*
_analysis (
    Xar* x,
    i32  k)
{
    redde (constans OratioAnalysisEn*)xar_obtinere(x, k);
}

/* an analysis k-ta regulam tituli dati ferat */
interior b32
_regula_est (
                   Xar* x,
                   i32  k,
    constans character* titulus)
{
    constans OratioAnalysisEn* a;

    si (x == NIHIL || k >= xar_numerus(x))
    {
        redde FALSUM;
    }
    a = _analysis(x, k);
    redde (b32)(a->regula >= ZEPHYRUM
        && strcmp(ORATIO_REGULAE_EN[a->regula].titulus, titulus)
            == ZEPHYRUM);
}

/* index analysis primae regulae tituli dati, -I si nulla */
interior s32
_regula_ubi (
                   Xar* x,
    constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; x != NIHIL && k < xar_numerus(x); k++)
    {
        si (_regula_est(x, k, titulus))
        {
            redde (s32)k;
        }
    }
    redde (s32)-I;
}

/* basis analysis primae regulae tituli dati ("" si nulla) */
interior chorda
_basis_regulae (
                   Xar* x,
    constans character* titulus)
{
    s32 k = _regula_ubi(x, titulus);

    si (k < ZEPHYRUM)
    {
        redde _l("");
    }
    redde _analysis(x, (i32)k)->basis;
}

/* fons malus inlinearis: onus NIHIL cum linea et causa datis */
interior vacuum
_malus (
               Piscina* piscina,
    constans character* fons,
                   i32  linea,
    constans character* causa)
{
    OratioVocabulariumVitium vitium;
    OratioVocabulariumEn* voc;

    memset(&vitium, ZEPHYRUM, magnitudo(vitium));
    voc = oratio_vocabularium_en_onerare(piscina, _l(fons), &vitium);
    CREDO_NIHIL (voc);
    CREDO_AEQUALIS_I32 (vitium.linea, linea);
    CREDO_VERUM (vitium.causa != NIHIL
        && strcmp(vitium.causa, causa) == ZEPHYRUM);
    si (   voc                         != NIHIL || vitium.causa == NIHIL
        || strcmp(vitium.causa, causa) != ZEPHYRUM)
    {
        imprimere("    FRACTA: '%s' -> linea %d causa %s (exspectata %d %s)\n",
            fons, (integer)vitium.linea,
            vitium.causa ? vitium.causa : "(nulla)", (integer)linea,
            causa);
    }
}

s32
principale (vacuum)
{
                    Piscina* piscina;
         constans character* radix;
                  character  via[1024];
                     chorda  fons;
       OratioVocabulariumEn* voc;
   OratioVocabulariumVitium  vitium;
 OratioVocabulariumEnCensus  census;
                    clock_t  ante;

    piscina =
        piscina_generare_dynamicum("probatio_oratio_vocabularium_en",
        268435456);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- I. Fons venditus ---\n");
    sprintf(via, "%s/oratio/vocabularium/en/mobypos.txt", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        CREDO_CULPA ("oratio/vocabularium/en/mobypos.txt absens");
        credo_imprimere_compendium();
        redde I;
    }
    imprimere("  octeti %d\n", (integer)fons.mensura);
    CREDO_AEQUALIS_I32 (fons.mensura, (i32)OCTETI_PINNA);
    CREDO_VERUM (fons.mensura >= (i32)II
        && fons.datum[fons.mensura - (i32)II] == '\r'
        && fons.datum[fons.mensura - I] == '\n');
    {
        chorda sigillum = oratio_vocabularium_sigillum(piscina, fons);

        imprimere("  sigillum %.*s\n", (integer)sigillum.mensura,
            (constans character*)sigillum.datum);
        CREDO_VERUM (_aequalis(sigillum, SIGILLUM_PINNA));
    }

    imprimere("\n--- II. Onus ---\n");
    memset(&vitium, ZEPHYRUM, magnitudo(vitium));
    ante  = clock();
    voc   = oratio_vocabularium_en_onerare(piscina, fons, &vitium);
    CREDO_NON_NIHIL (voc);
    si (voc == NIHIL)
    {
        imprimere("  vitium: %s:%d %s\n", vitium.plagula,
            (integer)vitium.linea, vitium.causa);
        credo_imprimere_compendium();
        redde I;
    }
    census = oratio_vocabularium_en_census(voc);
    imprimere("  recorda %d  verba %d  locutiones %d  octeti alti %d  codices ignoti %d (%.*s)  %.0f ms\n",
        (integer)census.recorda, (integer)census.verba,
        (integer)census.locutiones, (integer)census.octeti_alti,
        (integer)census.codices_ignoti,
        (integer)census.codex_ignotus_primus.mensura,
        (constans character*)census.codex_ignotus_primus.datum,
        1000.0 * (duplex)(clock() - ante) / (duplex)CLOCKS_PER_SEC);
    CREDO_AEQUALIS_I32 (census.recorda, (i32)RECORDA_PINNA);
    CREDO_AEQUALIS_I32 (census.verba, (i32)VERBA_PINNA);
    CREDO_AEQUALIS_I32 (census.locutiones, (i32)LOCUTIONES_PINNA);
    CREDO_AEQUALIS_I32 (census.verba + census.locutiones,
        census.recorda);
    CREDO_AEQUALIS_I32 (census.octeti_alti, (i32)OCTETI_ALTI_PINNA);
    CREDO_AEQUALIS_I32 (census.codices_ignoti,
        (i32)CODICES_IGNOTI_PINNA);
    CREDO_VERUM (_aequalis(census.codex_ignotus_primus, "cowardic"));

    imprimere("\n--- III. Quaestio ---\n");
    {
        Xar* x;

        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("the"));
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_aequalis(_codices(voc, x, ZEPHYRUM), "Dv"));
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("THE"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);   /* plicatio */
        x = oratio_vocabularium_en_quaerere(piscina, voc,
            _l("engineer"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_aequalis(_codices(voc, x, ZEPHYRUM), "Nt"));
        x = oratio_vocabularium_en_quaerere(piscina, voc,
            _l("running"));
        CREDO_VERUM (_aequalis(_codices(voc, x, ZEPHYRUM), "AN"));
        x = oratio_vocabularium_en_quaerere(piscina, voc,
            _l("well-known"));
        CREDO_VERUM (_aequalis(_codices(voc, x, ZEPHYRUM), "A"));
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("aa"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_aequalis(_forma(voc, x, ZEPHYRUM), "AA"));
        CREDO_VERUM (_aequalis(_forma(voc, x, I), "aa"));
        CREDO_VERUM (oratio_vocabularium_en_recordum(voc,
                *(s32*)xar_obtinere(x, ZEPHYRUM))->linea
            < oratio_vocabularium_en_recordum(voc,
                *(s32*)xar_obtinere(x, I))->linea);
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("of"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_aequalis(_forma(voc, x, ZEPHYRUM), "OF"));
        CREDO_VERUM (_aequalis(_codices(voc, x, I), "P"));
        x = oratio_vocabularium_en_quaerere(piscina, voc,
            _l("cowardic"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_aequalis(_codices(voc, x, ZEPHYRUM), "Ne"));
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("xyzzy"));
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_en_quaerere(piscina, voc,
            _l("a la mode"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);   /* locutio */
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l("mode"));
        CREDO_VERUM (xar_numerus(x) >= I);
        x = oratio_vocabularium_en_quaerere(piscina, voc, _l(""));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
    }

    imprimere("\n--- IV. Classes ---\n");
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('N'),
        "substantivum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('p'),
        "substantivum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('h'),
        "substantivum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('V'),
        "verbum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('t'),
        "verbum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('i'),
        "verbum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('A'),
        "adiectivum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('v'),
        "adverbium") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('C'),
        "coniunctio-coordinans") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('P'),
        "adpositio") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('!'),
        "interiectio") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('r'),
        "pronomen") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('D'),
        "determinans") == ZEPHYRUM);
    CREDO_VERUM (strcmp(oratio_vocabularium_en_classis('I'),
        "determinans") == ZEPHYRUM);
    CREDO_NIHIL (oratio_vocabularium_en_classis('e'));
    CREDO_NIHIL (oratio_vocabularium_en_classis('x'));
    CREDO_VERUM (strlen(ORATIO_VOCABULARIUM_EN_CODICES) == (size_t)XV);

    imprimere("\n--- V. Lex recordi ---\n");
    {
        OratioVocabulariumEn* bonus;
        Xar* x;

        memset(&vitium, ZEPHYRUM, magnitudo(vitium));
        bonus = oratio_vocabularium_en_onerare(piscina,
            _l("the\\Dv\r\nA la\\h\r\nengineer\\Nt\r\n"), &vitium);
        CREDO_NON_NIHIL (bonus);
        si (bonus != NIHIL)
        {
            OratioVocabulariumEnCensus c =
                oratio_vocabularium_en_census(bonus);

            CREDO_AEQUALIS_I32 (c.recorda, (i32)III);
            CREDO_AEQUALIS_I32 (c.verba, (i32)II);
            CREDO_AEQUALIS_I32 (c.locutiones, I);
            CREDO_AEQUALIS_I32 (c.codices_ignoti, ZEPHYRUM);
            x = oratio_vocabularium_en_quaerere(piscina, bonus,
                _l("Engineer"));
            CREDO_AEQUALIS_I32 (xar_numerus(x), I);
            CREDO_VERUM (oratio_vocabularium_en_recordum(bonus,
                *(s32*)xar_obtinere(x, ZEPHYRUM))->linea == (i32)III);
        }
        _malus(piscina, "the\\Dv\r\nbad\r\n", (i32)II,
            "signum '\\' deest");
        _malus(piscina, "a\\b\\c\r\n", I, "signa '\\' plura");
        _malus(piscina, "the\\Dv\nx\\N\r\n", I,
            "finis lineae non CRLF");
        _malus(piscina, "\\N\r\n", I, "forma vacua");
        _malus(piscina, "the\\\r\n", I, "codices vacui");
        _malus(piscina, "the\\Dv\r\nrun\\V", (i32)II,
            "linea ultima sine CRLF");
        _malus(piscina, "\r\n", I, "signum '\\' deest");
        _malus(piscina, "the\\Dv\r\n\r\nx\\N\r\n", (i32)II,
            "signum '\\' deest");
    }

        imprimere("\n--- VI. Regulae morphologicae (T15b) ---\n");
    {
        i32  r;
        Xar* x;

        CREDO_VERUM (ORATIO_REGULAE_EN_NUMERUS >= (i32)XV);
        per (r = ZEPHYRUM; r < ORATIO_REGULAE_EN_NUMERUS; r++)
        {
            constans OratioRegulaEn* rg = &ORATIO_REGULAE_EN[r];
                                i32  q;

            CREDO_VERUM (rg->titulus != NIHIL && rg->suffixum != NIHIL
                && rg->substitutio != NIHIL && rg->causa != NIHIL
                && strlen(rg->suffixum) > ZEPHYRUM
                && strlen(rg->causa) > ZEPHYRUM
                && rg->basis_minima >= I);
            per (q = ZEPHYRUM; q < r; q++)
            {
                si (strcmp(ORATIO_REGULAE_EN[q].titulus, rg->titulus)
                    == ZEPHYRUM)
                {
                    CREDO_CULPA ("titulus regulae duplex");
                }
            }
        }
        /* forma exacta semper prior, deinde regulae */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("running"));
        CREDO_NON_NIHIL (x);
        CREDO_VERUM (xar_numerus(x) >= (i32)II);
        CREDO_VERUM (_analysis(x, ZEPHYRUM)->regula == (s32)-I);
        CREDO_VERUM (_aequalis(_analysis(x, ZEPHYRUM)->basis,
            "running"));
        CREDO_VERUM (strcmp(_analysis(x, ZEPHYRUM)->classis,
            "adiectivum")
            == ZEPHYRUM);
        CREDO_VERUM (_regula_est(x, I, "participium-ing-geminatum"));
        CREDO_VERUM (_aequalis(_analysis(x, I)->basis, "run"));
        CREDO_VERUM (strcmp(_analysis(x, I)->classis, "verbum")
            == ZEPHYRUM);
        /* pluralia: classis ex basi */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("values"));
        CREDO_VERUM (xar_numerus(x) >= I);
        CREDO_VERUM (_regula_est(x, ZEPHYRUM, "pluralis-s"));
        CREDO_VERUM (_aequalis(_analysis(x, ZEPHYRUM)->basis, "value"));
        CREDO_VERUM (strcmp(_analysis(x, ZEPHYRUM)->classis,
            "substantivum")
            == ZEPHYRUM);
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("ids"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "pluralis-s"), "id"));   /* basis II litterarum */
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("gets"));
        CREDO_VERUM (xar_numerus(x) >= I
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "verbum")
                == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("Boxes"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "pluralis-es")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "box"));
                /* entries: Moby eam ut 'p' fert - forma exacta prima, regula post */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("entries"));
        CREDO_VERUM (xar_numerus(x) >= (i32)II
            && _analysis(x, ZEPHYRUM)->regula == (s32)-I
            && _regula_ubi(x, "pluralis-ies") >= I
            && _aequalis(_basis_regulae(x, "pluralis-ies"), "entry"));
        /* praeterita */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("tested"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "praeteritum-ed")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "test")
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "verbum")
                == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("shared"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "praeteritum-ed-e")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "share"));
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("tried"));
        CREDO_VERUM (_regula_ubi(x, "praeteritum-ied") >= I
            && _aequalis(_basis_regulae(x, "praeteritum-ied"), "try"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("planned"));
        CREDO_VERUM (_regula_ubi(x, "praeteritum-ed-geminatum") >= I
            && _aequalis(_basis_regulae(x, "praeteritum-ed-geminatum"),
                "plan"));
        CREDO_VERUM (_regula_ubi(x, "praeteritum-ed") < ZEPHYRUM);   /* 'plann' nulla */
        /* participia */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("testing"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "participium-ing")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "test"));
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("making"));
        CREDO_VERUM (_regula_ubi(x, "participium-ing-e") >= I
            && _aequalis(_basis_regulae(x, "participium-ing-e"),
            "make"));
        /* possessivum, adverbia, gradus */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("engineer's"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "possessivum")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "engineer")
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "substantivum")
                == ZEPHYRUM);
                /* adverbia Moby ipsa fert (v): regulae post formam exactam */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("structurally"));
        CREDO_VERUM (_regula_ubi(x, "adverbium-ly") >= I
            && _aequalis(_basis_regulae(x, "adverbium-ly"),
            "structural")
            && strcmp(_analysis(x, (i32)_regula_ubi(x, "adverbium-ly"))
                ->classis, "adverbium") == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("simply"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "adverbium-ly-le"),
            "simple"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("happily"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "adverbium-ily"),
            "happy"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("higher"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "comparativus-er")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "high")
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "adiectivum")
                == ZEPHYRUM);
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("larger"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "comparativus-er-e"),
            "large"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("highest"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "superlativus-est")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "high"));
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("largest"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "superlativus-est-e"),
            "large"));
        /* compositum: partes omnes exactae (self, test; byte, exact);
         * 'non' Moby non fert - non-null compositum NON est (inventum) */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("self-test"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum")
            && _analysis(x, ZEPHYRUM)->recordum == (s32)-I
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "self-test")
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "substantivum")
                == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("Byte-Exact"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum")
            && _aequalis(_analysis(x, ZEPHYRUM)->basis, "byte-exact"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("a-plot"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum"));
                /* praefixa (ORATIO_PRAEFIXA_EN): non-null compositum, classis
         * partis ultimae (null A); praefixa sola nihil; pars per regulam */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("non-null"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum")
            && strcmp(_analysis(x, ZEPHYRUM)->classis, "adiectivum")
                == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("multi-strand"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("non-multi"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("consists-in"));
        CREDO_VERUM (xar_numerus(x) >= I
            && _regula_est(x, ZEPHYRUM, "compositum"));
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("non-xyzzy"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        /* contractiones; apostrophus typographica plicata */
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("we're"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "contractio-'re"),
            "we")
            && strcmp(_analysis(x, (i32)_regula_ubi(x,
            "contractio-'re"))
                ->classis, "pronomen") == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("I\xe2\x80\x99m"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "contractio-'m"),
            "i"));
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("don't"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "contractio-n't"),
            "do")
            && strcmp(_analysis(x, (i32)_regula_ubi(x,
            "contractio-n't"))
                ->classis, "verbum") == ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("can't"));
        CREDO_VERUM (_regula_ubi(x, "contractio-n't") < ZEPHYRUM);   /* 'ca' N */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("dkc\xe2\x80\x99s"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);   /* dkc ignotum */
        /* gradus ex adverbio (near v) */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("nearest"));
        CREDO_VERUM (_aequalis(_basis_regulae(x, "superlativus-est"),
            "near"));
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("-null"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("self--test"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        /* limina: basis nimis brevis ('k' < III), suffixum solum, ignotum */
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("ks"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
                x = oratio_vocabularium_en_analysare(piscina, voc,
                    _l("xys"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);   /* basis 'xy' ignota */
        x = oratio_vocabularium_en_analysare(piscina, voc,
            _l("xyzzyed"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_en_analysare(piscina, voc, _l(""));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        /* forma exacta sine regula: 'the' unam analysin exactam */
        x = oratio_vocabularium_en_analysare(piscina, voc, _l("the"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_analysis(x, ZEPHYRUM)->regula == (s32)-I);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
