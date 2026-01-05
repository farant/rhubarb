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
    s16* error_buf;  /* Interleaved RGB error: 3 rows only */
    s32 x, y, i;
    s32 lat = (s32)latitudo;
    s32 alt = (s32)altitudo;
    s32 num_pixela = lat * alt;
    s32 row_stride;

    /* Pre-compute active palette for faster lookup */
    s32 active_palette[XVI][III];
    s32 num_active = 0;
    s32 active_indices[XVI];

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

    /* Build active palette array */
    per (i = 0; i < XVI; i++)
    {
        si (colores_activi[i])
        {
            active_palette[num_active][0] = (s32)(insignatus character)AQUINAS_PALETTE[i][0];
            active_palette[num_active][1] = (s32)(insignatus character)AQUINAS_PALETTE[i][1];
            active_palette[num_active][2] = (s32)(insignatus character)AQUINAS_PALETTE[i][2];
            active_indices[num_active] = i;
            num_active++;
        }
    }

    si (num_active == 0)
    {
        redde fructus;
    }

    /* Allocare output indices */
    fructus.indices = (i8*)piscina_allocare(piscina, (memoriae_index)num_pixela);
    si (fructus.indices == NIHIL)
    {
        redde fructus;
    }

    /* Allocare error buffer - only 3 rows needed, interleaved RGB (s16 sufficient) */
    row_stride = lat * III;
    error_buf = (s16*)piscina_allocare(piscina, (memoriae_index)(row_stride * III * (s32)magnitudo(s16)));
    si (error_buf == NIHIL)
    {
        redde fructus;
    }
    memset(error_buf, 0, (memoriae_index)(row_stride * III * (s32)magnitudo(s16)));

    /* Single-pass Atkinson dithering */
    per (y = 0; y < alt; y++)
    {
        s32 row0 = (y % III) * row_stride;
        s32 row1 = ((y + I) % III) * row_stride;
        s32 row2 = ((y + II) % III) * row_stride;
        s32 rgba_row = y * lat * IV;

        per (x = 0; x < lat; x++)
        {
            s32 ex = x * III;
            s32 rgba_idx = rgba_row + x * IV;

            /* Get pixel + accumulated error */
            s32 old_r = (s32)(insignatus character)rgba[rgba_idx + 0] + error_buf[row0 + ex + 0];
            s32 old_g = (s32)(insignatus character)rgba[rgba_idx + 1] + error_buf[row0 + ex + 1];
            s32 old_b = (s32)(insignatus character)rgba[rgba_idx + 2] + error_buf[row0 + ex + 2];

            /* Clamp inline */
            si (old_r < 0) old_r = 0; alioquin si (old_r > 255) old_r = 255;
            si (old_g < 0) old_g = 0; alioquin si (old_g > 255) old_g = 255;
            si (old_b < 0) old_b = 0; alioquin si (old_b > 255) old_b = 255;

            /* Find closest color - inline */
            {
                s32 best_idx = 0;
                s32 best_dist = 0x7FFFFFFF;
                s32 j;

                per (j = 0; j < num_active; j++)
                {
                    s32 dr = old_r - active_palette[j][0];
                    s32 dg = old_g - active_palette[j][1];
                    s32 db = old_b - active_palette[j][2];
                    s32 dist = dr * dr + dg * dg + db * db;

                    si (dist < best_dist)
                    {
                        best_dist = dist;
                        best_idx = j;
                    }
                }

                fructus.indices[y * lat + x] = (i8)active_indices[best_idx];

                /* Compute error (>> 3 = divide by 8) */
                {
                    s32 err_r = (old_r - active_palette[best_idx][0]) >> III;
                    s32 err_g = (old_g - active_palette[best_idx][1]) >> III;
                    s32 err_b = (old_b - active_palette[best_idx][2]) >> III;

                    /* Diffuse to 6 neighbors - unrolled with bounds checks */
                    /* Right 1: (x+1, y) */
                    si (x + I < lat)
                    {
                        error_buf[row0 + (x + I) * III + 0] += (s16)err_r;
                        error_buf[row0 + (x + I) * III + 1] += (s16)err_g;
                        error_buf[row0 + (x + I) * III + 2] += (s16)err_b;
                    }
                    /* Right 2: (x+2, y) */
                    si (x + II < lat)
                    {
                        error_buf[row0 + (x + II) * III + 0] += (s16)err_r;
                        error_buf[row0 + (x + II) * III + 1] += (s16)err_g;
                        error_buf[row0 + (x + II) * III + 2] += (s16)err_b;
                    }
                    /* Next row: (x-1, y+1), (x, y+1), (x+1, y+1) */
                    si (y + I < alt)
                    {
                        si (x > 0)
                        {
                            error_buf[row1 + (x - I) * III + 0] += (s16)err_r;
                            error_buf[row1 + (x - I) * III + 1] += (s16)err_g;
                            error_buf[row1 + (x - I) * III + 2] += (s16)err_b;
                        }
                        error_buf[row1 + x * III + 0] += (s16)err_r;
                        error_buf[row1 + x * III + 1] += (s16)err_g;
                        error_buf[row1 + x * III + 2] += (s16)err_b;
                        si (x + I < lat)
                        {
                            error_buf[row1 + (x + I) * III + 0] += (s16)err_r;
                            error_buf[row1 + (x + I) * III + 1] += (s16)err_g;
                            error_buf[row1 + (x + I) * III + 2] += (s16)err_b;
                        }
                    }
                    /* Two rows down: (x, y+2) */
                    si (y + II < alt)
                    {
                        error_buf[row2 + x * III + 0] += (s16)err_r;
                        error_buf[row2 + x * III + 1] += (s16)err_g;
                        error_buf[row2 + x * III + 2] += (s16)err_b;
                    }
                }
            }

            /* Clear this error cell for reuse */
            error_buf[row0 + ex + 0] = 0;
            error_buf[row0 + ex + 1] = 0;
            error_buf[row0 + ex + 2] = 0;
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
