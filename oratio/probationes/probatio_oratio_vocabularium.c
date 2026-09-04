/* probatio_oratio_vocabularium.c - Vocabularium Latinum ex WORDS (T7)
 *
 * I.   FONTES venditi: octeti pinnati (FONTES.md), CRLF servatum.
 * II.  COCTIO: numeri recordorum pinnati; recorda decocta contra
 *      fontes (stirps prima 'A' N 9 8 'M N' X X X C G; 'a' PREP ABL;
 *      'vir' N adest; flexio prima N 1 1 'NOM S C' 1 1 'a' X A; flexio
 *      sine fine; additamentum primum PREFIX 'ec'; TACKON 'que';
 *      unicum primum 'agantur').
 * III. TABULA COMMISSA oratio/vocabularium/la.bin == coctio octetim;
 *      sigillum SHA-256 pinnatum; octeti publicati.
 * IV.  RECENSIO tabulae commissae: numeri == capitis; copia truncata
 *      refutatur; octetus mutatus sigillum mutat.
 * V.   ASSERTIO COLUMNARUM: fontes mutati (spatium stirpis DICTLINE,
 *      mensura finis INFLECTS, ADDONS truncatum, forma UNIQUES duplex)
 *      coctionem sistunt cum plagula et linea nominatis.
 * Culpa plantata: columna translationis DICTLINE (DL_TRAN_AB) mota.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_vocabularium.h"
#include "nuntium.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIGILLUM_PINNA      "f598155c06f52682"
#define OCTETI_TABULAE      3816483
#define OCTETI_DICTLINE     6115855
#define OCTETI_INFLECTS     129314
#define OCTETI_ADDONS       34697
#define OCTETI_UNIQUES      9642
#define STIRPES_PINNA       39335
#define FLEXIONES_PINNA     1797
#define ADDITAMENTA_PINNA   343
#define UNICA_PINNA         79

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
    exitus->datum    = memoria;
    exitus->mensura  = (i32)longitudo;
    redde VERUM;
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

interior b32
_incipit (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura >= l
        && memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM);
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    memcpy(d.datum, c.datum, (size_t)c.mensura);
    d.mensura = c.mensura;
    redde d;
}

/* octetum primum literarum in chorda (offset) aut -I */
interior s32
_invenire (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + l <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (size_t)l) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

/* recorda tabulae ambulare: primum cuiusque generis et quaesita */
nomen structura {
              OratioStirps stirps_prima;
              OratioStirps stirps_tertia;
                       b32 vir_adest;
              OratioFlexio flexio_prima;
              OratioFlexio flexio_n;       /* N 1 1 NOM S C 1 1 a X A */
                       b32 flexio_n_adest;
                       b32 flexio_sine_fine;

    OratioAdditamentum additamentum_primum;
                   b32 que_tackon;
          OratioUnicum unicum_primum;
                   i32 stirpes;
                   i32 flexiones;
                   i32 additamenta;
                   i32 unica;
} Ambulatio;

interior b32
_ambulare (
      Piscina* piscina,
       chorda  tabula,
    Ambulatio* a)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, tabula);
              i32  tag;
              i32  wire;

    memset(a, ZEPHYRUM, magnitudo(*a));
    si (l == NIHIL)
    {
        redde FALSUM;
    }
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        chorda nidus = nuntium_legere_nuntium(l);

        si (nuntium_lector_error(l))
        {
            redde FALSUM;
        }
        commutatio (tag)
        {
            casus ORATIO_VOC_STIRPS:
            {
                OratioStirps r;

                si (!oratio_vocabularium_stirps_legere(piscina, nidus,
                    &r))
                {
                    redde FALSUM;
                }
                a->stirpes = a->stirpes + I;
                si (a->stirpes == I)
                {
                    a->stirps_prima = r;
                }
                si (a->stirpes == (i32)III)
                {
                    a->stirps_tertia = r;
                }
                si (   r.pars == ORATIO_PARS_N
                    && _aequalis(r.stirpes[ZEPHYRUM],
                        "vir"))
                {
                    a->vir_adest = VERUM;
                }
                frange;
            }
            casus ORATIO_VOC_FLEXIO:
            {
                OratioFlexio r;

                si (!oratio_vocabularium_flexio_legere(piscina, nidus,
                    &r))
                {
                    redde FALSUM;
                }
                                a->flexiones = a->flexiones + I;
                si (a->flexiones == I)
                {
                    a->flexio_prima = r;
                }
                si (   !a->flexio_n_adest && r.pars == ORATIO_PARS_N
                    && r.declinatio == I && r.varians == I
                    && _aequalis(r.accidentia, "NOM S C"))
                {
                    a->flexio_n        = r;
                    a->flexio_n_adest  = VERUM;
                }

                si (   r.mensura       == ZEPHYRUM
                    && r.finis.mensura == ZEPHYRUM)
                {
                    a->flexio_sine_fine = VERUM;
                }
                frange;
            }
            casus ORATIO_VOC_ADDITAMENTUM:
            {
                OratioAdditamentum r;

                si (!oratio_vocabularium_additamentum_legere(piscina,
                    nidus,
                        &r))
                {
                    redde FALSUM;
                }
                a->additamenta = a->additamenta + I;
                si (a->additamenta == I)
                {
                    a->additamentum_primum = r;
                }
                si (   r.genus == ORATIO_PARS_TACKON
                    && _aequalis(r.forma, "que"))
                {
                    a->que_tackon = VERUM;
                }
                frange;
            }
            casus ORATIO_VOC_UNICUM:
            {
                OratioUnicum r;

                si (!oratio_vocabularium_unicum_legere(piscina, nidus,
                    &r))
                {
                    redde FALSUM;
                }
                a->unica = a->unica + I;
                si (a->unica == I)
                {
                    a->unicum_primum = r;
                }
                frange;
            }
            ordinarius:
                frange;   /* caput */
        }
    }
    redde (b32)!nuntium_lector_error(l);
}

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix;
                character  via[1024];
 OratioVocabulariumFontes  fontes;
                   chorda  tabula;
                   chorda  commissa;
 OratioVocabulariumCensus  census;
 OratioVocabulariumVitium  vitium;
                   chorda  sigillum;

    piscina = piscina_generare_dynamicum("probatio_oratio_vocabularium",
        268435456);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- I. Fontes venditi (oratio/vocabularium/la) ---\n");
    sprintf(via, "%s/oratio/vocabularium/la/DICTLINE.GEN", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fontes.dictline));
    sprintf(via, "%s/oratio/vocabularium/la/INFLECTS.LAT", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fontes.inflects));
    sprintf(via, "%s/oratio/vocabularium/la/ADDONS.LAT", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fontes.addons));
    sprintf(via, "%s/oratio/vocabularium/la/UNIQUES.LAT", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fontes.uniques));
    imprimere("  DICTLINE %d  INFLECTS %d  ADDONS %d  UNIQUES %d octeti\n",
        (integer)fontes.dictline.mensura,
        (integer)fontes.inflects.mensura,
        (integer)fontes.addons.mensura,
        (integer)fontes.uniques.mensura);
    CREDO_AEQUALIS_I32 (fontes.dictline.mensura, (i32)OCTETI_DICTLINE);
    CREDO_AEQUALIS_I32 (fontes.inflects.mensura, (i32)OCTETI_INFLECTS);
    CREDO_AEQUALIS_I32 (fontes.addons.mensura,   (i32)OCTETI_ADDONS);
    CREDO_AEQUALIS_I32 (fontes.uniques.mensura,  (i32)OCTETI_UNIQUES);
        {
        /* CRLF verbatim: linea prima '\r' ante '\n' primum finit */
        s32 nl = _invenire(fontes.dictline, "\n");

        CREDO_VERUM (nl > ZEPHYRUM
            && fontes.dictline.datum[(i32)nl - I] == '\r');
    }

    si (   fontes.dictline.datum == NIHIL
        || fontes.inflects.datum == NIHIL
        || fontes.addons.datum   == NIHIL
        || fontes.uniques.datum  == NIHIL)
    {
        CREDO_CULPA ("fontes absunt - nihil ultra probatur");
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- II. Coctio ---\n");
    {
        clock_t ante = clock();
        clock_t post;
            b32 sana = oratio_vocabularium_coquere(piscina, &fontes,
                "mk270/whitakers-words@1f2f0fb0", &tabula, &census,
                &vitium);
        Ambulatio a;

        post = clock();
        si (!sana)
        {
            imprimere("  COCTIO SISTIT: %s:%d %s\n", vitium.plagula,
                (integer)vitium.linea, vitium.causa);
        }
        CREDO_VERUM (sana);
        imprimere("  stirpes %d  flexiones %d  additamenta %d  unica %d  octeti %d  ms %.1f\n",
            (integer)census.stirpes, (integer)census.flexiones,
            (integer)census.additamenta, (integer)census.unica,
            (integer)tabula.mensura,
            1000.0 * (duplex)(post - ante) / (duplex)CLOCKS_PER_SEC);
        CREDO_AEQUALIS_I32 (census.stirpes,     (i32)STIRPES_PINNA);
        CREDO_AEQUALIS_I32 (census.flexiones,   (i32)FLEXIONES_PINNA);
        CREDO_AEQUALIS_I32 (census.additamenta, (i32)ADDITAMENTA_PINNA);
        CREDO_AEQUALIS_I32 (census.unica,       (i32)UNICA_PINNA);
        CREDO_AEQUALIS_I32 (tabula.mensura,     (i32)OCTETI_TABULAE);

        /* recorda decocta contra fontes */
        CREDO_VERUM (_ambulare(piscina, tabula, &a));
        CREDO_AEQUALIS_I32 (a.stirpes, census.stirpes);
        CREDO_AEQUALIS_I32 (a.flexiones, census.flexiones);
        CREDO_AEQUALIS_I32 (a.additamenta, census.additamenta);
        CREDO_AEQUALIS_I32 (a.unica, census.unica);
        CREDO_VERUM (_aequalis(a.stirps_prima.stirpes[ZEPHYRUM], "A"));
        CREDO_VERUM (_aequalis(a.stirps_prima.stirpes[I], ""));
        CREDO_AEQUALIS_S32 ((s32)a.stirps_prima.pars,
            (s32)ORATIO_PARS_N);
        CREDO_AEQUALIS_I32 (a.stirps_prima.declinatio, (i32)IX);
        CREDO_AEQUALIS_I32 (a.stirps_prima.varians, (i32)VIII);
        CREDO_VERUM (_aequalis(a.stirps_prima.species, "M N"));
        CREDO_VERUM (a.stirps_prima.aetas == 'X'
            && a.stirps_prima.area == 'X'
            && a.stirps_prima.geo == 'X'
            && a.stirps_prima.frequentia == 'C'
            && a.stirps_prima.fons == 'G');
        CREDO_VERUM (_incipit(a.stirps_prima.sensus,
            "Aulus (Roman praenomen)"));
        CREDO_AEQUALIS_I32 (a.stirps_prima.linea, I);
        CREDO_VERUM (_aequalis(a.stirps_tertia.stirpes[ZEPHYRUM], "a"));
        CREDO_AEQUALIS_S32 ((s32)a.stirps_tertia.pars,
            (s32)ORATIO_PARS_PREP);
        CREDO_VERUM (_aequalis(a.stirps_tertia.species, "ABL"));
        CREDO_AEQUALIS_I32 (a.stirps_tertia.declinatio, ZEPHYRUM);
        CREDO_VERUM (a.vir_adest);
                /* recordum primum ordine plagulae: pars et linea publicatae */
        imprimere("  flexio prima: %s linea %d\n",
            oratio_pars_titulus(a.flexio_prima.pars),
            (integer)a.flexio_prima.linea);
        CREDO_MAIOR_I32 (a.flexio_prima.linea, ZEPHYRUM);
        CREDO_VERUM (a.flexio_n_adest);
        CREDO_AEQUALIS_I32 (a.flexio_n.clavis, I);
        CREDO_AEQUALIS_I32 (a.flexio_n.mensura, I);
        CREDO_VERUM (_aequalis(a.flexio_n.finis, "a"));
        CREDO_VERUM (a.flexio_n.aetas == 'X'
            && a.flexio_n.frequentia == 'A');

        CREDO_VERUM (a.flexio_sine_fine);
        CREDO_AEQUALIS_S32 ((s32)a.additamentum_primum.genus,
            (s32)ORATIO_PARS_PREFIX);
        CREDO_VERUM (_aequalis(a.additamentum_primum.forma, "ec"));
        CREDO_VERUM (_aequalis(a.additamentum_primum.descriptio,
            "PACK PACK"));
        CREDO_VERUM (_incipit(a.additamentum_primum.sensus,
            "is there any...that?"));
        CREDO_VERUM (a.que_tackon);
        CREDO_VERUM (_aequalis(a.unicum_primum.forma, "agantur"));
        CREDO_VERUM (_incipit(a.unicum_primum.descriptio,
            "V 3 1 PRES PASSIVE SUB 3 P IMPERS"));
        CREDO_VERUM (_incipit(a.unicum_primum.sensus,
            "let them be treated;"));
        CREDO_VERUM (strcmp(oratio_pars_titulus(ORATIO_PARS_VPAR),
            "VPAR")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)oratio_pars_ex_titulo(_copia(piscina,
            a.additamentum_primum.descriptio)),
            (s32)ORATIO_PARS_NUMERUS_PARTIUM);
    }

    imprimere("\n--- III. Tabula commissa == coctio; sigillum ---\n");
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &commissa));
    sigillum = oratio_vocabularium_sigillum(piscina, commissa);
    imprimere("  la.bin %d octeti, sigillum %.*s (pinna %s)\n",
        (integer)commissa.mensura, (integer)sigillum.mensura,
        (constans character*)sigillum.datum, SIGILLUM_PINNA);
    CREDO_AEQUALIS_I32 (commissa.mensura, tabula.mensura);
    CREDO_VERUM (commissa.mensura == tabula.mensura
        && memcmp(commissa.datum, tabula.datum, (size_t)tabula.mensura)
            == ZEPHYRUM);
    si (!_aequalis(sigillum, SIGILLUM_PINNA))
    {
        imprimere("  SIGILLUM MOTUM: ./oratio/vocabularium.sh -coquere -scribere, pinna cum causa nominata\n");
    }
    CREDO_VERUM (_aequalis(sigillum, SIGILLUM_PINNA));
    CREDO_VERUM (_aequalis(oratio_vocabularium_sigillum(piscina,
        tabula),
        SIGILLUM_PINNA));

    imprimere("\n--- IV. Recensio tabulae commissae ---\n");
    {
        OratioVocabulariumCensus c2;
        clock_t ante = clock();
        clock_t post;
        b32 sana = oratio_vocabularium_recensere(piscina, commissa, &c2,
            &vitium);
        chorda truncata;
        chorda mutata;

        post = clock();
        CREDO_VERUM (sana);
        imprimere("  recensio %.1f ms\n",
            1000.0 * (duplex)(post - ante) / (duplex)CLOCKS_PER_SEC);
        CREDO_AEQUALIS_I32 (c2.stirpes, census.stirpes);
        CREDO_AEQUALIS_I32 (c2.flexiones, census.flexiones);
        CREDO_AEQUALIS_I32 (c2.additamenta, census.additamenta);
        CREDO_AEQUALIS_I32 (c2.unica, census.unica);
        CREDO_AEQUALIS_I32 (c2.versio, (i32)ORATIO_VOCABULARIUM_VERSIO);
        CREDO_VERUM (_aequalis(c2.fons,
            "mk270/whitakers-words@1f2f0fb0"));
        truncata          = commissa;
        truncata.mensura  = truncata.mensura - (i32)100;
        CREDO_FALSUM (oratio_vocabularium_recensere(piscina, truncata,
            &c2,
            &vitium));
        imprimere("  truncata: %s @%d\n", vitium.causa,
            (integer)vitium.offset);
        mutata = _copia(piscina, commissa);
        mutata.datum[mutata.mensura / (i32)II] =
            (i8)(mutata.datum[mutata.mensura / (i32)II] ^ 0x01);
        CREDO_FALSUM (_aequalis(oratio_vocabularium_sigillum(piscina,
            mutata),
            SIGILLUM_PINNA));
    }

    imprimere("\n--- V. Assertio columnarum: fontes mutati sistunt ---\n");
    {
        OratioVocabulariumFontes f2 = fontes;
                          chorda ex;
                             s32 sedes;

        /* DICTLINE: spatium post stirpem primam lineae I sublatum */
        f2.dictline                 = _copia(piscina, fontes.dictline);
        f2.dictline.datum[(i32)18]  = 'x';
        CREDO_FALSUM (oratio_vocabularium_coquere(piscina, &f2,
            "probatio",
            &ex, NIHIL, &vitium));
        CREDO_VERUM (strcmp(vitium.plagula, "DICTLINE.GEN")
            == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vitium.linea, I);
        imprimere("  DICTLINE: %s:%d %s\n", vitium.plagula,
            (integer)vitium.linea, vitium.causa);

        /* INFLECTS: mensura finis recordi primi 1 -> 2 ("1 1 a" -> "1 2 a") */
        f2           = fontes;
        f2.inflects  = _copia(piscina, fontes.inflects);
        sedes        = _invenire(f2.inflects, "NOM S C  1 1 a");
        CREDO_VERUM (sedes >= ZEPHYRUM);
        si (sedes >= ZEPHYRUM)
        {
                        f2.inflects.datum[(i32)sedes + (i32)11] = '2';

            CREDO_FALSUM (oratio_vocabularium_coquere(piscina, &f2,
                "probatio",
                &ex, NIHIL, &vitium));
            CREDO_VERUM (strcmp(vitium.plagula, "INFLECTS.LAT")
                == ZEPHYRUM);
            CREDO_MAIOR_I32 (vitium.linea, ZEPHYRUM);
            imprimere("  INFLECTS: %s:%d %s\n", vitium.plagula,
                (integer)vitium.linea, vitium.causa);
        }

        /* ADDONS: linea ultima (sensus) amota */
        f2         = fontes;
        f2.addons  = fontes.addons;
        dum (   f2.addons.mensura > ZEPHYRUM
             && (f2.addons.datum[f2.addons.mensura - I] == '\n'
                || f2.addons.datum[f2.addons.mensura - I] == '\r'))
        {
            f2.addons.mensura = f2.addons.mensura - I;
        }
        dum (   f2.addons.mensura > ZEPHYRUM
             && f2.addons.datum[f2.addons.mensura - I] != '\n')
        {
            f2.addons.mensura = f2.addons.mensura - I;
        }
        CREDO_FALSUM (oratio_vocabularium_coquere(piscina, &f2,
            "probatio",
            &ex, NIHIL, &vitium));
        CREDO_VERUM (strcmp(vitium.plagula, "ADDONS.LAT") == ZEPHYRUM);
        imprimere("  ADDONS: %s:%d %s\n", vitium.plagula,
            (integer)vitium.linea, vitium.causa);

        /* UNIQUES: forma prima duplex ("agantur" -> "aga tur") */
        f2                          = fontes;
        f2.uniques                  = _copia(piscina, fontes.uniques);
        f2.uniques.datum[(i32)III]  = ' ';
        CREDO_FALSUM (oratio_vocabularium_coquere(piscina, &f2,
            "probatio",
            &ex, NIHIL, &vitium));
        CREDO_VERUM (strcmp(vitium.plagula, "UNIQUES.LAT") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vitium.linea, I);
        imprimere("  UNIQUES: %s:%d %s\n", vitium.plagula,
            (integer)vitium.linea, vitium.causa);
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
