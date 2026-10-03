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

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
