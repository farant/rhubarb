/*
 * dithering.c - Atkinson dithering implementation
 *
 * Implementatio Atkinson error diffusion pro Aquinas palette.
 */

#include "dithering.h"
#include <string.h>

/* ============================================================
 * Aquinas Palette Data
 * ============================================================ */

constans i8 AQUINAS_PALETTE[XVI][III] = {
    {0x00, 0x00, 0x00},  /* 0:  Niger (Black) */
    {0x34, 0x44, 0x24},  /* 1:  Griseus Obscurus (Dark Gray) */
    {0x7C, 0x5C, 0x7C},  /* 2:  Malva (Mauve) */
    {(i8)0xC8, (i8)0xB8, (i8)0xA0},  /* 3:  Griseus Medius */
    {(i8)0xE8, (i8)0xD8, (i8)0xC0},  /* 4:  Griseus Lucidus */
    {(i8)0xFC, (i8)0xF0, (i8)0xE8},  /* 5:  Albus (White) */
    {(i8)0x80, 0x20, 0x20},  /* 6:  Ruber Obscurus (Dark Red) */
    {0x60, (i8)0xBC, (i8)0xFC},  /* 7:  Caeruleus (Blue) */
    {(i8)0xFC, 0x18, 0x70},  /* 8:  Roseus (Pink) */
    {(i8)0xA4, 0x68, 0x00},  /* 9:  Aurum Obscurum (Dark Gold) */
    {(i8)0xE8, (i8)0xB4, 0x00},  /* 10: Aurum Medium */
    {(i8)0xFC, (i8)0xD8, 0x40},  /* 11: Aurum Lucidum (Bright Gold) */
    {0x40, (i8)0x80, 0x20},  /* 12: Folium Obscurum (Dark Leaf) */
    {(i8)0x80, (i8)0xC0, 0x40},  /* 13: Folium Medium */
    {(i8)0xA0, (i8)0xF0, 0x60},  /* 14: Folium Lucidum (Bright Leaf) */
    {(i8)0xB4, (i8)0x94, 0x7C},  /* 15: Griseus Calidus (Warm Gray) */
};

constans character* AQUINAS_NOMINA[XVI] = {
    "Niger",
    "Griseus Obscurus",
    "Malva",
    "Griseus Medius",
    "Griseus Lucidus",
    "Albus",
    "Ruber Obscurus",
    "Caeruleus",
    "Roseus",
    "Aurum Obscurum",
    "Aurum Medium",
    "Aurum Lucidum",
    "Folium Obscurum",
    "Folium Medium",
    "Folium Lucidum",
    "Griseus Calidus"
};

/* ============================================================
 * Atkinson Offsets
 * ============================================================
 *
 *     [*] [1] [1]
 * [1] [1] [1]
 *     [1]
 */
hic_manens constans s32 ATKINSON_OFFSETS[VI][II] = {
    {1, 0}, {2, 0},           /* dextra 1, dextra 2 */
    {-1, 1}, {0, 1}, {1, 1},  /* proxima linea: sinistra, centrum, dextra */
    {0, 2}                    /* duas lineas infra */
};

/* ============================================================
 * Functiones Auxiliares Internae
 * ============================================================ */

/*
 * Invenire colorem proximum per distantiam RGB
 */
hic_manens s32
_invenire_proximum_colorem(
    s32 r, s32 g, s32 b,
    constans b32 colores_activi[XVI])
{
    s32 optimus_index = 0;
    s32 optima_distantia = 0x7FFFFFFF;  /* Maximum s32 */
    s32 i;

    per (i = 0; i < XVI; i++)
    {
        s32 dr, dg, db, dist;

        si (!colores_activi[i]) perge;

        dr = r - (s32)(insignatus character)AQUINAS_PALETTE[i][0];
        dg = g - (s32)(insignatus character)AQUINAS_PALETTE[i][1];
        db = b - (s32)(insignatus character)AQUINAS_PALETTE[i][2];
        dist = dr * dr + dg * dg + db * db;

        si (dist < optima_distantia)
        {
            optima_distantia = dist;
            optimus_index = i;
        }
    }

    redde optimus_index;
}

/*
 * Computare luminantiam (0-255)
 * Formula: 0.3*R + 0.59*G + 0.11*B
 * In fixed point: (77*R + 150*G + 28*B) >> 8
 */
hic_manens s32
_computare_luminantiam(s32 r, s32 g, s32 b)
{
    redde (77 * r + 150 * g + 28 * b) >> 8;
}

/*
 * Clamp valor ad 0-255
 */
hic_manens s32
_clamp(s32 v)
{
    si (v < 0) redde 0;
    si (v > 255) redde 255;
    redde v;
}

/* ============================================================
 * Functiones Publicae
 * ============================================================ */

DitheringFructus
dithering_atkinson_colorum(
    constans i8*  rgba,
    i32           latitudo,
    i32           altitudo,
    constans b32  colores_activi[XVI],
    Piscina*      piscina)
{
    DitheringFructus fructus;
    s32* error_r;
    s32* error_g;
    s32* error_b;
    s32 x, y, i;
    s32 lat = (s32)latitudo;
    s32 alt = (s32)altitudo;
    s32 num_pixela = lat * alt;

    /* Initiare fructum */
    fructus.successus = FALSUM;
    fructus.indices = NIHIL;
    fructus.latitudo = latitudo;
    fructus.altitudo = altitudo;

    /* Validare */
    si (rgba == NIHIL || piscina == NIHIL || latitudo == 0 || altitudo == 0)
    {
        redde fructus;
    }

    /* Allocare output indices */
    fructus.indices = (i8*)piscina_allocare(piscina, (memoriae_index)num_pixela);
    si (fructus.indices == NIHIL)
    {
        redde fructus;
    }

    /* Allocare error buffers (usant s32 pro negativis valoribus) */
    error_r = (s32*)piscina_allocare(piscina, (memoriae_index)(num_pixela * (s32)magnitudo(s32)));
    error_g = (s32*)piscina_allocare(piscina, (memoriae_index)(num_pixela * (s32)magnitudo(s32)));
    error_b = (s32*)piscina_allocare(piscina, (memoriae_index)(num_pixela * (s32)magnitudo(s32)));

    si (error_r == NIHIL || error_g == NIHIL || error_b == NIHIL)
    {
        redde fructus;
    }

    /* Copiare RGBA ad error buffers */
    per (i = 0; i < num_pixela; i++)
    {
        error_r[i] = (s32)(insignatus character)rgba[i * 4 + 0];
        error_g[i] = (s32)(insignatus character)rgba[i * 4 + 1];
        error_b[i] = (s32)(insignatus character)rgba[i * 4 + 2];
    }

    /* Atkinson dithering */
    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 idx = y * lat + x;
            s32 old_r = _clamp(error_r[idx]);
            s32 old_g = _clamp(error_g[idx]);
            s32 old_b = _clamp(error_b[idx]);
            s32 color_idx;
            s32 new_r, new_g, new_b;
            s32 err_r, err_g, err_b;
            s32 j;

            /* Invenire colorem proximum */
            color_idx = _invenire_proximum_colorem(old_r, old_g, old_b, colores_activi);
            fructus.indices[idx] = (i8)color_idx;

            /* Obtinere RGB coloris electi */
            new_r = (s32)(insignatus character)AQUINAS_PALETTE[color_idx][0];
            new_g = (s32)(insignatus character)AQUINAS_PALETTE[color_idx][1];
            new_b = (s32)(insignatus character)AQUINAS_PALETTE[color_idx][2];

            /* Computare errorem (dividere per 8 pro Atkinson) */
            err_r = (old_r - new_r) / 8;
            err_g = (old_g - new_g) / 8;
            err_b = (old_b - new_b) / 8;

            /* Diffundere errorem ad 6 vicinos */
            per (j = 0; j < VI; j++)
            {
                s32 nx = x + ATKINSON_OFFSETS[j][0];
                s32 ny = y + ATKINSON_OFFSETS[j][1];

                si (nx >= 0 && nx < lat && ny >= 0 && ny < alt)
                {
                    s32 nidx = ny * lat + nx;
                    error_r[nidx] += err_r;
                    error_g[nidx] += err_g;
                    error_b[nidx] += err_b;
                }
            }
        }
    }

    fructus.successus = VERUM;
    redde fructus;
}

DitheringFructus
dithering_atkinson_griseum(
    constans i8*  rgba,
    i32           latitudo,
    i32           altitudo,
    constans b32  colores_activi[XVI],
    constans i32  puncta[III],
    Piscina*      piscina)
{
    DitheringFructus fructus;
    s32* error_lum;
    s32 x, y, i;
    s32 lat = (s32)latitudo;
    s32 alt = (s32)altitudo;
    s32 num_pixela = lat * alt;

    /* Colores activi ordinati per luminantiam */
    s32 colores_ordinati[XVI];
    s32 luminantiae[XVI];
    s32 num_activi = 0;

    /* Initiare fructum */
    fructus.successus = FALSUM;
    fructus.indices = NIHIL;
    fructus.latitudo = latitudo;
    fructus.altitudo = altitudo;

    /* Validare */
    si (rgba == NIHIL || piscina == NIHIL || latitudo == 0 || altitudo == 0)
    {
        redde fructus;
    }

    /* Colligere colores activos cum luminantiis */
    per (i = 0; i < XVI; i++)
    {
        si (colores_activi[i])
        {
            s32 r = (s32)(insignatus character)AQUINAS_PALETTE[i][0];
            s32 g = (s32)(insignatus character)AQUINAS_PALETTE[i][1];
            s32 b = (s32)(insignatus character)AQUINAS_PALETTE[i][2];

            colores_ordinati[num_activi] = i;
            luminantiae[num_activi] = _computare_luminantiam(r, g, b);
            num_activi++;
        }
    }

    si (num_activi == 0)
    {
        redde fructus;
    }

    /* Ordinare per luminantiam (bubble sort, num_activi parvus) */
    per (i = 0; i < num_activi - 1; i++)
    {
        s32 j;
        per (j = 0; j < num_activi - 1 - i; j++)
        {
            si (luminantiae[j] > luminantiae[j + 1])
            {
                s32 temp_lum = luminantiae[j];
                s32 temp_col = colores_ordinati[j];
                luminantiae[j] = luminantiae[j + 1];
                colores_ordinati[j] = colores_ordinati[j + 1];
                luminantiae[j + 1] = temp_lum;
                colores_ordinati[j + 1] = temp_col;
            }
        }
    }

    /* Allocare output indices */
    fructus.indices = (i8*)piscina_allocare(piscina, (memoriae_index)num_pixela);
    si (fructus.indices == NIHIL)
    {
        redde fructus;
    }

    /* Allocare error buffer */
    error_lum = (s32*)piscina_allocare(piscina, (memoriae_index)(num_pixela * (s32)magnitudo(s32)));
    si (error_lum == NIHIL)
    {
        redde fructus;
    }

    /* Copiare luminantiam ad error buffer */
    per (i = 0; i < num_pixela; i++)
    {
        s32 r = (s32)(insignatus character)rgba[i * 4 + 0];
        s32 g = (s32)(insignatus character)rgba[i * 4 + 1];
        s32 b = (s32)(insignatus character)rgba[i * 4 + 2];
        error_lum[i] = _computare_luminantiam(r, g, b);
    }

    /* Atkinson dithering */
    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 idx = y * lat + x;
            s32 old_lum = _clamp(error_lum[idx]);
            s32 color_bucket = num_activi - 1;
            s32 color_idx;
            s32 new_lum;
            s32 err_lum;
            s32 j;

            /* Invenire bucket per puncta */
            per (j = 0; j < III && j < num_activi - 1; j++)
            {
                si (old_lum < (s32)puncta[j])
                {
                    color_bucket = j;
                    frange;
                }
            }

            /* Obtinere colorem ex bucket */
            color_idx = colores_ordinati[color_bucket];
            fructus.indices[idx] = (i8)color_idx;

            /* Computare luminantiam target (linearis per bucket) */
            new_lum = (color_bucket * 255) / (num_activi > 1 ? num_activi - 1 : 1);

            /* Computare errorem */
            err_lum = (old_lum - new_lum) / 8;

            /* Diffundere errorem */
            per (j = 0; j < VI; j++)
            {
                s32 nx = x + ATKINSON_OFFSETS[j][0];
                s32 ny = y + ATKINSON_OFFSETS[j][1];

                si (nx >= 0 && nx < lat && ny >= 0 && ny < alt)
                {
                    s32 nidx = ny * lat + nx;
                    error_lum[nidx] += err_lum;
                }
            }
        }
    }

    fructus.successus = VERUM;
    redde fructus;
}

vacuum
dithering_indices_ad_rgba(
    constans i8*  indices,
    i8*           rgba_output,
    i32           latitudo,
    i32           altitudo)
{
    s32 i;
    s32 num_pixela = (s32)latitudo * (s32)altitudo;

    si (indices == NIHIL || rgba_output == NIHIL)
    {
        redde;
    }

    per (i = 0; i < num_pixela; i++)
    {
        s32 color_idx = (s32)(insignatus character)indices[i];
        si (color_idx < 0) color_idx = 0;
        si (color_idx > XV) color_idx = XV;

        rgba_output[i * 4 + 0] = AQUINAS_PALETTE[color_idx][0];
        rgba_output[i * 4 + 1] = AQUINAS_PALETTE[color_idx][1];
        rgba_output[i * 4 + 2] = AQUINAS_PALETTE[color_idx][2];
        rgba_output[i * 4 + 3] = (i8)255;
    }
}

vacuum
dithering_color_ex_indice(s32 index, i8* r, i8* g, i8* b)
{
    si (index < 0) index = 0;
    si (index > XV) index = XV;

    si (r) *r = AQUINAS_PALETTE[index][0];
    si (g) *g = AQUINAS_PALETTE[index][1];
    si (b) *b = AQUINAS_PALETTE[index][2];
}

/* ============================================================
 * Praeparatores (Presets)
 * ============================================================ */

vacuum
dithering_praeparare_omnes(b32 colores[XVI])
{
    s32 i;
    per (i = 0; i < XVI; i++)
    {
        colores[i] = VERUM;
    }
}

vacuum
dithering_praeparare_griseum(b32 colores[XVI])
{
    s32 i;
    /* Grayscale: 0, 1, 3, 4, 5, 15 */
    per (i = 0; i < XVI; i++)
    {
        colores[i] = FALSUM;
    }
    colores[0] = VERUM;   /* Niger */
    colores[1] = VERUM;   /* Griseus Obscurus */
    colores[3] = VERUM;   /* Griseus Medius */
    colores[4] = VERUM;   /* Griseus Lucidus */
    colores[5] = VERUM;   /* Albus */
    colores[15] = VERUM;  /* Griseus Calidus */
}

vacuum
dithering_praeparare_calidus(b32 colores[XVI])
{
    s32 i;
    /* Warm: 0, 5, 6, 9, 10, 11, 15 */
    per (i = 0; i < XVI; i++)
    {
        colores[i] = FALSUM;
    }
    colores[0] = VERUM;   /* Niger */
    colores[5] = VERUM;   /* Albus */
    colores[6] = VERUM;   /* Ruber Obscurus */
    colores[9] = VERUM;   /* Aurum Obscurum */
    colores[10] = VERUM;  /* Aurum Medium */
    colores[11] = VERUM;  /* Aurum Lucidum */
    colores[15] = VERUM;  /* Griseus Calidus */
}

vacuum
dithering_praeparare_frigidus(b32 colores[XVI])
{
    s32 i;
    /* Cool: 0, 1, 5, 7, 12, 13, 14 */
    per (i = 0; i < XVI; i++)
    {
        colores[i] = FALSUM;
    }
    colores[0] = VERUM;   /* Niger */
    colores[1] = VERUM;   /* Griseus Obscurus */
    colores[5] = VERUM;   /* Albus */
    colores[7] = VERUM;   /* Caeruleus */
    colores[12] = VERUM;  /* Folium Obscurum */
    colores[13] = VERUM;  /* Folium Medium */
    colores[14] = VERUM;  /* Folium Lucidum */
}

vacuum
dithering_praeparare_monochromaticus(b32 colores[XVI])
{
    s32 i;
    /* Mono: 0, 5 */
    per (i = 0; i < XVI; i++)
    {
        colores[i] = FALSUM;
    }
    colores[0] = VERUM;   /* Niger */
    colores[5] = VERUM;   /* Albus */
}
