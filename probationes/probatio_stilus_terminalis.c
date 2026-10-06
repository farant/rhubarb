/* probatio_stilus_terminalis.c - codex stili (modulus 004, T1):
 * codificatio canonica (subset tesserae octetim idem ac
 * _stilum_emittere hodiernus), ornamenta et sublineae omnes, colores
 * (tabula, RGB, CCLVI quantizatum), prior aequalis = nihil; tabula
 * CCLVI xterm; stilus_quantizare contra copiam CONGELATAM _cclvi
 * tesserae (oraculum manet post T3, cum originale deletur). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "stilus_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Oraculum: tessera_opus.c _cclvi @ 65ff3eb1, verbatim */
interior i32
_cclvi_tesserae (
    i32 color)
{
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };
                        i32 index[III];
                        i32 d_cubi    = ZEPHYRUM;
                        i32 d_grisei  = ZEPHYRUM;
                        i32 summa     = ZEPHYRUM;
                        i32 griseus;
                        i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 c = (s32)((color >> (XVI - VIII * k)) & 0xFF);
        s32 d;
        i32 j;

        index[k] = ZEPHYRUM;
        per (j = I; j < VI; j++)
        {
            s32 prior  = c - (s32)GRADUS[index[k]];
            s32 hic    = c - (s32)GRADUS[j];

            si (hic * hic < prior * prior)
            {
                index[k] = j;
            }
        }
        d       = c - (s32)GRADUS[index[k]];
        d_cubi  += (i32)(d * d);
        summa   += (i32)c;
    }
    griseus = (summa / III < VIII) ? ZEPHYRUM
                                   : (summa / III - VIII + V) / X;
    si (griseus > XXIII)
    {
        griseus = XXIII;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 d = (s32)((color >> (XVI - VIII * k)) & 0xFF)
            - (s32)(VIII + X * griseus);

        d_grisei += (i32)(d * d);
    }
    si (d_grisei < d_cubi)
    {
        redde CCXXXII + griseus;
    }
    redde XVI + XXXVI * index[0] + VI * index[1] + index[2];
}

interior StilusColor
_color (
    StilusColorGenus genus,
                 i32 valor)
{
    StilusColor c;

    c.genus = genus;
    c.valor = valor;
    redde c;
}

interior b32
_aequat (
                      Piscina* piscina,
    constans StilusTerminalis* prior,
    constans StilusTerminalis* st,
                          i32  codificatio,
           constans character* expectatum)
{
     ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);
                chorda  c;

    stilus_codificare(a, prior, st, codificatio);
    c = chorda_aedificator_finire(a);
    si (!chorda_aequalis_literis(c, expectatum))
    {
        imprimere("  exspectatum %s\n  actuale     %.*s\n", expectatum
            + I,
            (int)c.mensura, (constans character*)c.datum + I);
        redde FALSUM;
    }
    redde VERUM;
}

/* T2: octeti -> lexema (series_terminalis) -> stilus_applicare */
interior s32
_applicare_octetos (
              chorda  octeti,
    StilusTerminalis* st)
{
    hic_manens Piscina* piscina = NIHIL;
          SeriesLector* lector;
          SeriesLexema  l;
           constans i8* p       = octeti.datum;
           constans i8* finis   = octeti.datum + octeti.mensura;
                   s32  ignota  = ZEPHYRUM;
                   b32  ullum   = FALSUM;

    si (piscina == NIHIL)
    {
        piscina = piscina_generare_dynamicum("lexemator", M * M);
    }
    lector = series_lectorem_creare(piscina);
    /* omnes series applicantur (codificatio longa dividitur) */
    dum (   p < finis
         && series_lexema_proximum(lector, &p, finis, &l)
             != SERIES_NIHIL)
    {
        ignota  += (s32)stilus_applicare(&l, st);
        ullum   = VERUM;
    }
    redde ullum ? ignota : -I;
}

interior b32
_applicat (
                 Piscina* piscina,
      constans character* octeti,
                     i32  ignota,
        StilusTerminalis* st)
{
    s32 n = _applicare_octetos(chorda_ex_literis(octeti, piscina), st);

    si (n != (s32)ignota)
    {
        imprimere("  %s: ignota %d (exspectata %d)\n", octeti + I,
            (int)n, (int)ignota);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_nativus (
    constans StilusTerminalis* st)
{
    StilusTerminalis n;

    stilus_nativus(&n);
    redde stilus_aequalis(st, &n);
}

interior b32
_color_est (
         StilusColor c,
    StilusColorGenus genus,
                 i32 valor)
{
    redde (b32)(c.genus == genus && c.valor == valor);
}

/* codificare (PLENA) deinde applicare in lectum */
interior b32
_reditus (
                      Piscina* piscina,
    constans StilusTerminalis* s,
             StilusTerminalis* lectus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);

    stilus_codificare(a, NIHIL, s, STILUS_CODIFICATIO_PLENA);
    redde (b32)(_applicare_octetos(chorda_aedificator_finire(a), lectus)
        == ZEPHYRUM);
}

s32 principale (vacuum)
{
              Piscina* piscina;
     StilusTerminalis  st;
     StilusTerminalis  alius;
                  i32  r;
                  i32  g;
                  i32  b;
                  i32  discordes = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_stilus_terminalis",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. nativus: reditio nuda ---\n");
    stilus_nativus(&st);
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0m"));

    imprimere("\n--- II. subset tesserae: ut _stilum_emittere ---\n");
    st.ornamenta       = STILUS_CRASSUM | STILUS_CURSIVUM
        | STILUS_INVERSUM
        | STILUS_TRANSFIXUM;
    st.sublinea        = STILUS_SUBLINEA_SIMPLEX;
    st.color_litterae  = _color(STILUS_COLOR_RGB, 0x112233);
    st.color_fundi     = _color(STILUS_COLOR_RGB, 0xFF8000);
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;1;3;4;7;9;38;2;17;34;51;48;2;255;128;0m"));
    st.ornamenta  = STILUS_OBSCURUM;
    st.sublinea   = STILUS_SUBLINEA_NULLA;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_CCLVI,
        "\033[0;2;38;5;235;48;5;208m"));

    imprimere("\n--- III. ornamenta omnia, ordine canonico ---\n");
    stilus_nativus(&st);
    st.ornamenta = STILUS_CRASSUM | STILUS_OBSCURUM | STILUS_CURSIVUM
        | STILUS_NICTANS | STILUS_INVERSUM | STILUS_INVISIBILE
        | STILUS_TRANSFIXUM | STILUS_SUPERLINEA;
    st.sublinea  = STILUS_SUBLINEA_SIMPLEX;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;1;2;3;4;5;7;8;9;53m"));

    imprimere("\n--- III-bis. ultra XXIV parametra: altera ---\n");
    stilus_nativus(&st);
    st.ornamenta = STILUS_CRASSUM | STILUS_OBSCURUM | STILUS_CURSIVUM
        | STILUS_NICTANS | STILUS_INVERSUM | STILUS_INVISIBILE
        | STILUS_TRANSFIXUM | STILUS_SUPERLINEA;
    st.sublinea         = STILUS_SUBLINEA_LINEOLATA;
    st.color_litterae   = _color(STILUS_COLOR_RGB, 0x010203);
    st.color_fundi      = _color(STILUS_COLOR_RGB, 0x040506);
    st.color_sublineae  = _color(STILUS_COLOR_RGB, 0x070809);
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;1;2;3;4:5;5;7;8;9;53;38;2;1;2;3;48;2;4;5;6m"
        "\033[58;2;7;8;9m"));

    imprimere("\n--- IV. genera sublineae ---\n");
    stilus_nativus(&st);
    st.sublinea = STILUS_SUBLINEA_DUPLEX;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:2m"));
    st.sublinea = STILUS_SUBLINEA_UNDULATA;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:3m"));
    st.sublinea = STILUS_SUBLINEA_PUNCTATA;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:4m"));
    st.sublinea = STILUS_SUBLINEA_LINEOLATA;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:5m"));

    imprimere("\n--- V. colores: tabula, sublineae, CCLVI ---\n");
    stilus_nativus(&st);
    st.color_litterae   = _color(STILUS_COLOR_TABULA, V);
    st.color_fundi      = _color(STILUS_COLOR_TABULA, CC);
    st.color_sublineae  = _color(STILUS_COLOR_RGB, 0x0A0B0C);
    st.sublinea         = STILUS_SUBLINEA_UNDULATA;
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:3;38;5;5;48;5;200;58;2;10;11;12m"));
    /* CCLVI: RGB quantizatur, tabula manet */
    CREDO_VERUM (_aequat(piscina, NIHIL, &st, STILUS_CODIFICATIO_CCLVI,
        "\033[0;4:3;38;5;5;48;5;200;58;5;232m"));

    imprimere("\n--- VI. prior: aequalis nihil, aliter plena ---\n");
    alius = st;
    CREDO_VERUM (stilus_aequalis(&st, &alius));
    CREDO_VERUM (_aequat(piscina, &alius, &st, STILUS_CODIFICATIO_PLENA,
        ""));
    alius.ornamenta = STILUS_CRASSUM;
    CREDO_FALSUM (stilus_aequalis(&st, &alius));
    CREDO_VERUM (_aequat(piscina, &alius, &st, STILUS_CODIFICATIO_PLENA,
        "\033[0;4:3;38;5;5;48;5;200;58;2;10;11;12m"));

    imprimere("\n--- VII. tabula CCLVI xterm ---\n");
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(ZEPHYRUM), 0x000000);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(I), 0xCD0000);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(XII), 0x5C5CFF);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(XV), 0xFFFFFF);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(XVI), 0x000000);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(CXCVI), 0xFF0000);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(CCXXXI), 0xFFFFFF);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(CCXXXII), 0x080808);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(CCLV), 0xEEEEEE);
    CREDO_AEQUALIS_I32 (stilus_tabulae_color(CCLVI), ZEPHYRUM);

    imprimere("\n--- VIII. quantizatio contra _cclvi tesserae ---\n");
    CREDO_AEQUALIS_I32 (stilus_quantizare(0x000000), XVI);
    CREDO_AEQUALIS_I32 (stilus_quantizare(0xFF0000), CXCVI);
    CREDO_AEQUALIS_I32 (stilus_quantizare(0x808080), CCXLIV);
    per (r = ZEPHYRUM; r < CCLVI; r += XVII)
    {
        per (g = ZEPHYRUM; g < CCLVI; g += XVII)
        {
            per (b = ZEPHYRUM; b < CCLVI; b += XVII)
            {
                i32 c = (r << XVI) | (g << VIII) | b;

                si (stilus_quantizare(c) != _cclvi_tesserae(c))
                {
                    discordes++;
                }
            }
        }
    }
    CREDO_AEQUALIS_I32 (discordes, ZEPHYRUM);
    /* gradus aequidistantes (115, 155, 195, 235: inter gradus
     * cubi): gradus INFERIOR. Reticulum XVII eos numquam tangit
     * (planta T1 id monstravit) - ergo valores aequidistantes ipsi */
    {
        hic_manens constans i32 AEQUI[VI] = { 0, 115, 155, 195, 235,
            255 };

        discordes = ZEPHYRUM;
        per (r = ZEPHYRUM; r < VI; r++)
        {
            per (g = ZEPHYRUM; g < VI; g++)
            {
                per (b = ZEPHYRUM; b < VI; b++)
                {
                    i32 c = (AEQUI[r] << XVI) | (AEQUI[g] << VIII)
                        | AEQUI[b];

                    si (stilus_quantizare(c) != _cclvi_tesserae(c))
                    {
                        discordes++;
                    }
                }
            }
        }
        CREDO_AEQUALIS_I32 (discordes, ZEPHYRUM);
    }
    /* cubus vincit, 115 -> gradus 95 (index I), non 135 */
    CREDO_AEQUALIS_I32 (stilus_quantizare(0x7300FF), LVII);

    imprimere("\n--- IX. decodificatio: vectores Ghostty ---\n");
    /* resetum: vacuum aut 0 */
    CREDO_VERUM (_applicat(piscina, "\033[m", ZEPHYRUM, &st));
    CREDO_VERUM (_nativus(&st));
    CREDO_VERUM (_applicat(piscina, "\033[0m", ZEPHYRUM, &st));
    CREDO_VERUM (_nativus(&st));
    /* 38;2 directus; nimis brevis = ignotum (consumitur) */
    CREDO_VERUM (_applicat(piscina, "\033[38;2;40;44;52m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_RGB,
        0x282C34));
    CREDO_VERUM (_applicat(piscina, "\033[38;2;44;52m", I, &st));
    CREDO_VERUM (_applicat(piscina, "\033[48;2;40;44;52m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_fundi, STILUS_COLOR_RGB,
        0x282C34));
    /* multiplex: 0 deinde 38;2 */
    CREDO_VERUM (_applicat(piscina, "\033[1;0;38;2;40;44;52m", ZEPHYRUM,
        &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, ZEPHYRUM);
    /* ':' in parametro non permisso: ignotum, deinde bold */
    CREDO_VERUM (_applicat(piscina, "\033[0:4;1m", I, &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, STILUS_CRASSUM);
    CREDO_VERUM (_applicat(piscina, "\033[0:4:2;1m", I, &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, STILUS_CRASSUM);
    /* bold / italic / blink / inverse / invisible / strike + reseta */
    CREDO_VERUM (_applicat(piscina, "\033[1;2;3;5;7;8;9;53m", ZEPHYRUM,
        &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, STILUS_CRASSUM | STILUS_OBSCURUM
        | STILUS_CURSIVUM | STILUS_NICTANS | STILUS_INVERSUM
        | STILUS_INVISIBILE | STILUS_TRANSFIXUM | STILUS_SUPERLINEA);
    /* 22 crassum ET obscurum tollit */
    CREDO_VERUM (_applicat(piscina, "\033[22;23;25;27;28;29;55m",
        ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, ZEPHYRUM);
    CREDO_VERUM (_applicat(piscina, "\033[6m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, STILUS_NICTANS);
    /* sublinea: 4, 24, 21, 4:0..4:5 */
    CREDO_VERUM (_applicat(piscina, "\033[4m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea, (i32)STILUS_SUBLINEA_SIMPLEX);
    CREDO_VERUM (_applicat(piscina, "\033[24m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea, (i32)STILUS_SUBLINEA_NULLA);
    CREDO_VERUM (_applicat(piscina, "\033[21m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea, (i32)STILUS_SUBLINEA_DUPLEX);
    CREDO_VERUM (_applicat(piscina, "\033[4:3m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea,
        (i32)STILUS_SUBLINEA_UNDULATA);
    CREDO_VERUM (_applicat(piscina, "\033[4:4m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea,
        (i32)STILUS_SUBLINEA_PUNCTATA);
    CREDO_VERUM (_applicat(piscina, "\033[4:5m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea,
        (i32)STILUS_SUBLINEA_LINEOLATA);
    CREDO_VERUM (_applicat(piscina, "\033[4:0m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea, (i32)STILUS_SUBLINEA_NULLA);
    CREDO_VERUM (_applicat(piscina, "\033[4:1m", ZEPHYRUM, &st));
    CREDO_AEQUALIS_I32 ((i32)st.sublinea, (i32)STILUS_SUBLINEA_SIMPLEX);
    /* 4:2 deinde bold; 4:2:3 nimis = ignotum, deinde bold */
    CREDO_VERUM (_applicat(piscina, "\033[0;4:2;1m", ZEPHYRUM, &st));
    CREDO_VERUM (   st.sublinea == STILUS_SUBLINEA_DUPLEX
                 && st.ornamenta == STILUS_CRASSUM);
    CREDO_VERUM (_applicat(piscina, "\033[0;4:2:3;1m", I, &st));
    CREDO_VERUM (   st.sublinea == STILUS_SUBLINEA_NULLA
                 && st.ornamenta == STILUS_CRASSUM);
    /* VIII colores: 31 43 90 103 */
    CREDO_VERUM (_applicat(piscina, "\033[31;43;90;103m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_TABULA,
        VIII));
    CREDO_VERUM (_color_est(st.color_fundi, STILUS_COLOR_TABULA, XI));
    CREDO_VERUM (_applicat(piscina, "\033[31;43m", ZEPHYRUM, &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_TABULA, I));
    CREDO_VERUM (_color_est(st.color_fundi, STILUS_COLOR_TABULA, III));
    CREDO_VERUM (_applicat(piscina, "\033[39;49m", ZEPHYRUM, &st));
    CREDO_VERUM (   st.color_litterae.genus == STILUS_COLOR_NATIVUS
                 && st.color_fundi.genus == STILUS_COLOR_NATIVUS);
    /* CCLVI */
    CREDO_VERUM (_applicat(piscina, "\033[38;5;161;48;5;236m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_TABULA,
        CLXI));
    CREDO_VERUM (_color_est(st.color_fundi, STILUS_COLOR_TABULA,
        CCXXXVI));
    CREDO_VERUM (_applicat(piscina, "\033[58;5;9m", ZEPHYRUM, &st));
    CREDO_VERUM (_color_est(st.color_sublineae, STILUS_COLOR_TABULA,
        IX));
    CREDO_VERUM (_applicat(piscina, "\033[59m", ZEPHYRUM, &st));
    CREDO_VERUM (st.color_sublineae.genus == STILUS_COLOR_NATIVUS);
    /* ':' formae: 48:2:1:2:3, 58:2:1:2:3, cum spatio coloris */
    CREDO_VERUM (_applicat(piscina, "\033[48:2:1:2:3m", ZEPHYRUM, &st));
    CREDO_VERUM (_color_est(st.color_fundi, STILUS_COLOR_RGB,
        0x010203));
    CREDO_VERUM (_applicat(piscina, "\033[58:2:1:2:3m", ZEPHYRUM, &st));
    CREDO_VERUM (_color_est(st.color_sublineae, STILUS_COLOR_RGB,
        0x010203));
    CREDO_VERUM (_applicat(piscina, "\033[38:2:0:1:2:3m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_RGB,
        0x010203));
    CREDO_VERUM (_applicat(piscina, "\033[38:2::1:2:3m", ZEPHYRUM,
        &st));
    CREDO_VERUM (_color_est(st.color_litterae, STILUS_COLOR_RGB,
        0x010203));
    /* ';' formae: spatium coloris NON legitur (r = 0); '3' sequens
     * parametrum suum est (cursivum), ut Ghostty */
    CREDO_VERUM (_applicat(piscina, "\033[0;38;2;0;1;2;3m", ZEPHYRUM,
        &st));
    CREDO_VERUM (   _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0x000102)
                 && st.ornamenta == STILUS_CURSIVUM);
    /* nimis ':' : ignotum, deinde bold */
    CREDO_VERUM (_applicat(piscina, "\033[0;38:2:0:1:2:3:4;1m", I,
        &st));
    CREDO_VERUM (   st.color_litterae.genus == STILUS_COLOR_NATIVUS
                 && st.ornamenta == STILUS_CRASSUM);
    /* spatium coloris + parametrum extra; sine spatio + extra */
    CREDO_VERUM (_applicat(piscina, "\033[0;38:2:0:1:2:3;1m", ZEPHYRUM,
        &st));
    CREDO_VERUM (   _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0x010203)
                 && st.ornamenta == STILUS_CRASSUM);
    CREDO_VERUM (_applicat(piscina, "\033[0;38:2:1:2:3;1m", ZEPHYRUM,
        &st));
    CREDO_VERUM (   _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0x010203)
                 && st.ornamenta == STILUS_CRASSUM);
    /* sublinea, fundus, littera mixta */
    CREDO_VERUM (_applicat(piscina,
        "\033[0;4;38;2;255;247;219;48;2;242;93;147;4m", ZEPHYRUM, &st));
    CREDO_VERUM (   st.sublinea == STILUS_SUBLINEA_SIMPLEX
                 && _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0xFFF7DB)
                 && _color_est(st.color_fundi, STILUS_COLOR_RGB,
                 0xF25D93));
    /* kakoune (bis) */
    CREDO_VERUM (_applicat(piscina,
        "\033[0;4:3;38;2;175;175;215;58:2::190:80:70m", ZEPHYRUM, &st));
    CREDO_VERUM (   st.sublinea == STILUS_SUBLINEA_UNDULATA
                 && _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0xAFAFD7)
                 && _color_est(st.color_sublineae, STILUS_COLOR_RGB,
                        0xBE5046));
    CREDO_VERUM (_applicat(piscina,
        "\033[0;4:3;38;2;51;51;51;48;2;170;170;170;58;2;255;97;136m",
        ZEPHYRUM, &st));
    CREDO_VERUM (   st.sublinea == STILUS_SUBLINEA_UNDULATA
                 && _color_est(st.color_litterae, STILUS_COLOR_RGB,
                        0x333333)
                 && _color_est(st.color_fundi, STILUS_COLOR_RGB,
                 0xAAAAAA)
                 && _color_est(st.color_sublineae, STILUS_COLOR_RGB,
                        0xFF6188));
    /* 58:4: (finis ':' sine sub-parametro): sine ruina; cursus ':'
     * malus = ignotum UNUM (Ghostty II numerat: 58, deinde 4:) */
    CREDO_VERUM (_applicat(piscina, "\033[0;58:4:m", I, &st));
    /* 38;5 truncatum: ignotum, reliqua consumuntur (5 NON nictans) */
    CREDO_VERUM (_applicat(piscina, "\033[0;38;5m", I, &st));
    CREDO_AEQUALIS_I32 (st.ornamenta, ZEPHYRUM);
    /* privatum ('>4;2m', modifyOtherKeys xterm): NON SGR */
    st.ornamenta = STILUS_CRASSUM;
    CREDO_VERUM (_applicat(piscina, "\033[>4;2m", ZEPHYRUM, &st));
    CREDO_VERUM (   st.ornamenta == STILUS_CRASSUM
                 && st.sublinea == STILUS_SUBLINEA_NULLA);

    imprimere("\n--- X. reditus: applicare(codificare(s)) == s ---\n");
    {
        constans StilusColor colores[VI] = {
            { STILUS_COLOR_NATIVUS, 0 }, { STILUS_COLOR_TABULA, 0 },
            { STILUS_COLOR_TABULA, 15 }, { STILUS_COLOR_TABULA, 200 },
            { STILUS_COLOR_RGB, 0x010203 }, { STILUS_COLOR_RGB,
                0xFFFFFF }
        };
        i32 o;
        i32 fracti = ZEPHYRUM;

        per (o = ZEPHYRUM; o < CCLVI; o++)
        {
            i32 k;

            per (k = ZEPHYRUM; k < XXXVI; k++)
            {
                StilusTerminalis s;
                StilusTerminalis lectus;

                stilus_nativus(&s);
                s.ornamenta        = o;
                s.sublinea         = (StilusSublinea)((o + k) % VI);
                s.color_litterae   = colores[k % VI];
                s.color_fundi      = colores[(k / VI) % VI];
                s.color_sublineae  = colores[(o + k) % VI];
                /* initium SORDIDUM: reditio plena omnia ponere debet */
                stilus_nativus(&lectus);
                lectus.ornamenta    = STILUS_INVERSUM;
                lectus.color_fundi  = colores[III];
                si (   !_reditus(piscina, &s, &lectus)
                    || !stilus_aequalis(&s, &lectus))
                {
                    fracti++;
                }
            }
        }
        CREDO_AEQUALIS_I32 (fracti, ZEPHYRUM);
    }
    /* CCLVI: RGB in tabulam quantizatum */
    {
         StilusTerminalis  s;
         StilusTerminalis  lectus;
        ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);
                   chorda  c;

        stilus_nativus(&s);
        s.color_litterae = _color(STILUS_COLOR_RGB, 0xFF0000);
        stilus_codificare(a, NIHIL, &s, STILUS_CODIFICATIO_CCLVI);
        c = chorda_aedificator_finire(a);
        stilus_nativus(&lectus);
        CREDO_AEQUALIS_S32 (_applicare_octetos(c, &lectus), ZEPHYRUM);
        CREDO_VERUM (_color_est(lectus.color_litterae,
            STILUS_COLOR_TABULA,
            CXCVI));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
