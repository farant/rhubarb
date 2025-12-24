/* bitsyfont_ad_fons.c - Convertere Bitsy font ad fons_6x8.h formatum
 *
 * Usus: ./bitsyfont_ad_fons Alcuin.bitsyfont > alcuin_fons.h
 *
 * Legit formatum Bitsy:
 *   FONT Alcuin
 *   SIZE 6 8
 *   CHAR 65
 *   001100
 *   000110
 *   ...
 *
 * Scribit formatum fons_6x8.h:
 *   {0x30, 0x18, ...}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LATITUDO_MAXIMA 8
#define ALTITUDO_MAXIMA 8
#define NUM_GLYPHAE     256

/* Glypha data: VIII bytes per character */
static unsigned char glyphae[NUM_GLYPHAE][ALTITUDO_MAXIMA];
static int glyphae_praesens[NUM_GLYPHAE];  /* Tractat quae glyphae definitae sunt */

/* Convertere lineam binariam ad byte
 * "001100" -> 0x30 (bits 5,4 set in high portion)
 * Bit 7 = leftmost pixel, bit 2 = rightmost of 6 pixels
 */
static unsigned char
linea_ad_byte(const char* linea, int longitudo)
{
    unsigned char fructus = 0;
    int i;

    for (i = 0; i < longitudo && i < LATITUDO_MAXIMA; i++)
    {
        if (linea[i] == '1')
        {
            /* Bit 7 est pixelum sinistrum */
            fructus |= (unsigned char)(0x80 >> i);
        }
    }

    return fructus;
}

/* Parsare filum bitsyfont */
static int
parsare_bitsyfont(FILE* filum)
{
    char linea[256];
    int character_currens = -1;
    int ordo_currens = 0;
    int latitudo = 6;
    int altitudo = 8;

    while (fgets(linea, sizeof(linea), filum))
    {
        /* Removere newline */
        size_t len = strlen(linea);
        while (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r'))
        {
            linea[--len] = '\0';
        }

        /* Praetermittere lineas vacuas */
        if (len == 0)
        {
            continue;
        }

        /* Parsare FONT */
        if (strncmp(linea, "FONT ", 5) == 0)
        {
            /* Nomen fontis - ignorare */
            continue;
        }

        /* Parsare SIZE */
        if (strncmp(linea, "SIZE ", 5) == 0)
        {
            sscanf(linea + 5, "%d %d", &latitudo, &altitudo);
            if (altitudo > ALTITUDO_MAXIMA)
            {
                fprintf(stderr, "Altitudo %d nimis magna (max %d)\n",
                        altitudo, ALTITUDO_MAXIMA);
                return -1;
            }
            continue;
        }

        /* Parsare CHAR */
        if (strncmp(linea, "CHAR ", 5) == 0)
        {
            character_currens = atoi(linea + 5);
            ordo_currens = 0;

            if (character_currens < 0 || character_currens >= NUM_GLYPHAE)
            {
                fprintf(stderr, "Character %d extra limites\n", character_currens);
                character_currens = -1;
            }
            continue;
        }

        /* Si habemus character currens, haec est linea bitmap */
        if (character_currens >= 0 && ordo_currens < altitudo)
        {
            /* Verificare quod linea continet solum 0 et 1 */
            int valida = 1;
            size_t i;

            for (i = 0; i < len && i < (size_t)latitudo; i++)
            {
                if (linea[i] != '0' && linea[i] != '1')
                {
                    valida = 0;
                    break;
                }
            }

            if (valida && len >= (size_t)latitudo)
            {
                glyphae[character_currens][ordo_currens] =
                    linea_ad_byte(linea, latitudo);
                ordo_currens++;

                if (ordo_currens >= altitudo)
                {
                    glyphae_praesens[character_currens] = 1;
                }
            }
        }
    }

    return 0;
}

/* Scribere header C */
static void
scribere_header(const char* nomen_fontis)
{
    int i, j;
    int primus;

    printf("/* Fons bitmap generatus ex %s */\n", nomen_fontis);
    printf("/* Magnitudo characteris: 6x8 pixela */\n");
    printf("\n");
    printf("#ifndef FONS_ALCUIN_6X8_H\n");
    printf("#define FONS_ALCUIN_6X8_H\n");
    printf("\n");
    printf("#include \"latina.h\"\n");
    printf("\n");
    printf("/* Quodque byte repraesentat unum ordinem characteris */\n");
    printf("/* Bit VII est pixelum sinistrum, bit 0 est pixelum dextrum */\n");
    printf("interior constans i8 fons_alcuin_6x8[CCLVI][VIII] = {\n");

    for (i = 0; i < NUM_GLYPHAE; i++)
    {
        /* Commentum cum numero characteris */
        if (i >= 0x20 && i < 0x7F)
        {
            printf("    /* 0x%02X '%c' */     {", i, (char)i);
        }
        else
        {
            printf("    /* 0x%02X  */     {", i);
        }

        /* Scribere VIII bytes */
        primus = 1;
        for (j = 0; j < ALTITUDO_MAXIMA; j++)
        {
            if (!primus) printf(", ");
            printf("0x%02X", glyphae[i][j]);
            primus = 0;
        }

        if (i < NUM_GLYPHAE - 1)
        {
            printf("},\n");
        }
        else
        {
            printf("}\n");
        }
    }

    printf("};\n");
    printf("\n");
    printf("#endif /* FONS_ALCUIN_6X8_H */\n");
}

/* Scribere modo glyphas quae differunt (pro merge) */
static void
scribere_differentias(void)
{
    int i, j;
    int numerus = 0;

    printf("/* Glyphae ex Alcuin.bitsyfont quae non vacuae sunt */\n");
    printf("/* Usus: merge haec in fons_6x8.h ubi slots vacui sunt */\n");
    printf("\n");

    for (i = 0; i < NUM_GLYPHAE; i++)
    {
        if (!glyphae_praesens[i])
        {
            continue;
        }

        /* Verificare si glypha non vacua */
        int non_vacua = 0;
        for (j = 0; j < ALTITUDO_MAXIMA; j++)
        {
            if (glyphae[i][j] != 0)
            {
                non_vacua = 1;
                break;
            }
        }

        if (non_vacua)
        {
            if (i >= 0x20 && i < 0x7F)
            {
                printf("    /* 0x%02X '%c' */     {", i, (char)i);
            }
            else
            {
                printf("    /* 0x%02X  */     {", i);
            }

            for (j = 0; j < ALTITUDO_MAXIMA; j++)
            {
                if (j > 0) printf(", ");
                printf("0x%02X", glyphae[i][j]);
            }
            printf("},\n");
            numerus++;
        }
    }

    fprintf(stderr, "Inventae %d glyphae non vacuae\n", numerus);
}

int
main(int argc, char** argv)
{
    FILE* filum;
    int modus_diff = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usus: %s [-d] <bitsyfont>\n", argv[0]);
        fprintf(stderr, "  -d  Scribere solum differentias (pro merge)\n");
        return 1;
    }

    /* Verificare pro -d flag */
    if (argc >= 3 && strcmp(argv[1], "-d") == 0)
    {
        modus_diff = 1;
        argv++;
    }

    filum = fopen(argv[1], "r");
    if (!filum)
    {
        fprintf(stderr, "Non potest aperire: %s\n", argv[1]);
        return 1;
    }

    /* Initiare glyphas ad zeros */
    memset(glyphae, 0, sizeof(glyphae));
    memset(glyphae_praesens, 0, sizeof(glyphae_praesens));

    if (parsare_bitsyfont(filum) < 0)
    {
        fclose(filum);
        return 1;
    }

    fclose(filum);

    if (modus_diff)
    {
        scribere_differentias();
    }
    else
    {
        scribere_header(argv[1]);
    }

    return 0;
}
