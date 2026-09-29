/* runae_generare.c - Tabulas runarum GENERARE ex datis Unicode
 *
 * Usus: runae_generare <radix_ucd> <exitus.c>
 *
 * Legit (radix_ucd = probationes/fixa/unicode/<versio>/):
 *   DerivedGeneralCategory.txt  - Cc Cs Zl Zp, Mn Me
 *   EastAsianWidth.txt          - W, F
 *   DerivedCoreProperties.txt   - Default_Ignorable_Code_Point
 *   GraphemeBreakProperty.txt   - Regional_Indicator, V, T, Prepend
 *   emoji-data.txt              - Emoji_Modifier
 * et scribit lib/runae_tabulae.c (tabula duorum graduum; vide
 * include/runae_tabulae.h). Lineae '# @missing:' ut lineae datorum
 * tractantur (valor ordinarius) et ANTE data stare debent.
 *
 * REGULA LATITUDINIS = Ghostty (uucode wcwidth_standalone +
 * wcwidth_zero_in_grapheme, uucode_config.zig WidthComponent); vide
 * lib/runae.phase-log.md U2 et _latitudo infra.
 *
 * Instrumentum aetatis evolutionis; tools/runae_generare.sh id struit
 * et currit (-probare: tabula commissa recens?).
 */
#include "latina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNAE_NUMERUS 0x110000
#define TRUNCUS 256
#define TRUNCI_PRIMI (RUNAE_NUMERUS / TRUNCUS)
#define LINEA_MAXIMA 1024

/* Vexilla proprietatum per runam */
#define V_CATEGORIA_NULLA      0x0001   /* Cc Cs Zl Zp */
#define V_SIGNUM_NM     0x0002   /* Mn Me */
#define V_LATA          0x0004   /* EAW W aut F */
#define V_IGNORABILIS   0x0008   /* Default_Ignorable_Code_Point */
#define V_RI            0x0010   /* GCB Regional_Indicator */
#define V_MEDIAE_FINALES       0x0020   /* GCB V aut T */
#define V_PRAEPOSITUM   0x0040   /* GCB Prepend */
#define V_MODIFICATOR   0x0080   /* Emoji_Modifier */

hic_manens i16 vexilla[RUNAE_NUMERUS];
hic_manens i8  valores[RUNAE_NUMERUS];
hic_manens i16 gradus_primus[TRUNCI_PRIMI];
hic_manens i16 trunci_unici[TRUNCI_PRIMI];   /* index trunci primi */
hic_manens i32 numerus_unicorum = ZEPHYRUM;


/* ==================================================
 * Lectio linearum UCD
 * ================================================== */

interior vacuum
_purgare (
    character* s)
{
    character* initium = s;
          i32  longitudo;

    dum (*initium == ' ' || *initium == '\t')
    {
        initium++;
    }
    si (initium != s)
    {
        memmove(s, initium, strlen(initium) + I);
    }
    longitudo = (i32)strlen(s);
    dum (   longitudo > ZEPHYRUM
         && (s[longitudo - I] == ' ' || s[longitudo - I] == '\t'
             || s[longitudo - I] == '\n' || s[longitudo - I] == '\r'))
    {
        s[--longitudo] = '\0';
    }
}

interior b32
_hex (
    constans character* s,
                   s32* valor)
{
    s32 v = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (s[k] != '\0')
    {
        character c = s[k];
              s32 d;

        si (c >= '0' && c <= '9')
        {
            d = (s32)(c - '0');
        }
        alioquin si (c >= 'A' && c <= 'F')
        {
            d = (s32)(c - 'A') + X;
        }
        alioquin si (c >= 'a' && c <= 'f')
        {
            d = (s32)(c - 'a') + X;
        }
        alioquin
        {
            redde FALSUM;
        }
        v = v * XVI + d;
        k++;
    }
    *valor = v;
    redde (b32)(k > ZEPHYRUM && v < RUNAE_NUMERUS);
}

/* Lineam in campos scindere: "A..B ; v1 ; v2 # commentum". Reddit
 * numerum camporum (0 = linea sine datis); campi purgati. */
interior i32
_campi (
    character* linea,
    character* campi[III])
{
    character* commentum;
    character* cursor  = linea;
          i32  n       = ZEPHYRUM;

    commentum = strchr(linea, '#');
    si (commentum != NIHIL)
    {
        *commentum = '\0';
    }
    dum (n < III)
    {
        character* sep = strchr(cursor, ';');

        campi[n] = cursor;
        si (sep != NIHIL)
        {
            *sep = '\0';
        }
        _purgare(campi[n]);
        n++;
        si (sep == NIHIL)
        {
            frange;
        }
        cursor = sep + I;
    }
    si (n == I && campi[ZEPHYRUM][ZEPHYRUM] == '\0')
    {
        redde ZEPHYRUM;
    }
    redde n;
}

/* Proprietas: vexillum ponere aut purgare pro intervallo */
nomen i16 (*Interpres)(constans character* v1, constans character* v2,
    i16* mascula);

interior b32
_filum_legere (
     constans character* radix,
     constans character* titulus,
              Interpres  interpres)
{
    character via[LINEA_MAXIMA];
    character linea[LINEA_MAXIMA];
        FILE* f;
          b32 data_visa = FALSUM;
          i32 numerus_linea = ZEPHYRUM;

    sprintf(via, "%s/%s", radix, titulus);
    f = fopen(via, "r");
    si (f == NIHIL)
    {
        fprintf(stderr, "runae_generare: legi non potest: %s\n", via);
        redde FALSUM;
    }
    dum (fgets(linea, LINEA_MAXIMA, f) != NIHIL)
    {
         character* campi[III];
         character* textus = linea;
               i32  n;
               s32  prima;
               s32  ultima;
               i16  mascula;
               i16  positum;
         character* puncta;
               s32  r;

        numerus_linea++;
        si (strncmp(linea, "# @missing:", XI) == ZEPHYRUM)
        {
            textus = linea + XI;
        }
        alioquin si (linea[ZEPHYRUM] == '#')
        {
            perge;
        }
        n = _campi(textus, campi);
        si (n == ZEPHYRUM)
        {
            perge;
        }
        si (n < II)
        {
            fprintf(stderr, "runae_generare: %s:%u: linea mala\n", via,
                (insignatus integer)numerus_linea);
            fclose(f);
            redde FALSUM;
        }
        si (textus == linea)
        {
            data_visa = VERUM;
        }
        puncta = strstr(campi[ZEPHYRUM], "..");
        si (puncta != NIHIL)
        {
            *puncta = '\0';
            si (   !_hex(campi[ZEPHYRUM], &prima)
                || !_hex(puncta + II, &ultima))
            {
                fprintf(stderr,
                    "runae_generare: %s:%u: intervallum malum\n",
                    via, (insignatus integer)numerus_linea);
                fclose(f);
                redde FALSUM;
            }
        }
        alioquin
        {
            si (!_hex(campi[ZEPHYRUM], &prima))
            {
                fprintf(stderr, "runae_generare: %s:%u: runa mala\n",
                    via,
                    (insignatus integer)numerus_linea);
                fclose(f);
                redde FALSUM;
            }
            ultima = prima;
        }
        positum = interpres(campi[I], (n > II) ? campi[II] : "",
            &mascula);
        /* @missing post data proprietatis NOSTRAE data eius deleret
         * (DerivedCoreProperties: @missing InCB in medio - nobis in U2
         * nihil regit, ergo innocuum) */
        si (textus != linea && data_visa && mascula != ZEPHYRUM)
        {
            fprintf(stderr,
                "runae_generare: %s:%u: @missing post data\n",
                via, (insignatus integer)numerus_linea);
            fclose(f);
            redde FALSUM;
        }
        per (r = prima; r <= ultima; r++)
        {
            vexilla[r] = (i16)((vexilla[r] & ~mascula) | positum);
        }
    }
    fclose(f);
    redde VERUM;
}


/* ==================================================
 * Interpretes per filum: reddunt vexilla posita; mascula = vexilla
 * quae linea haec REGIT (purgantur ante positionem)
 * ================================================== */

interior i16
_categoria (
    constans character* v1,
    constans character* v2,
                   i16* mascula)
{
    (vacuum)v2;
    *mascula = (i16)(V_CATEGORIA_NULLA | V_SIGNUM_NM);
    si (   strcmp(v1, "Cc") == ZEPHYRUM || strcmp(v1, "Cs") == ZEPHYRUM
        || strcmp(v1, "Zl") == ZEPHYRUM || strcmp(v1, "Zp") == ZEPHYRUM)
    {
        redde (i16)V_CATEGORIA_NULLA;
    }
    si (strcmp(v1, "Mn") == ZEPHYRUM || strcmp(v1, "Me") == ZEPHYRUM)
    {
        redde (i16)V_SIGNUM_NM;
    }
    redde ZEPHYRUM;
}

interior i16
_latitudo_orientalis (
    constans character* v1,
    constans character* v2,
                   i16* mascula)
{
    (vacuum)v2;
    *mascula = (i16)V_LATA;
    redde (i16)((strcmp(v1, "W") == ZEPHYRUM
        || strcmp(v1, "F") == ZEPHYRUM)
        ? V_LATA : ZEPHYRUM);
}

interior i16
_proprietates_nuclei (
    constans character* v1,
    constans character* v2,
                   i16* mascula)
{
    (vacuum)v2;
    si (strcmp(v1, "Default_Ignorable_Code_Point") == ZEPHYRUM)
    {
        *mascula = (i16)V_IGNORABILIS;
        redde (i16)V_IGNORABILIS;
    }
    *mascula = ZEPHYRUM;   /* ceterae (InCB in U4) non reguntur */
    redde ZEPHYRUM;
}

interior i16
_ruptura_graphematis (
    constans character* v1,
    constans character* v2,
                   i16* mascula)
{
    (vacuum)v2;
    *mascula = (i16)(V_RI | V_MEDIAE_FINALES | V_PRAEPOSITUM);
    si (strcmp(v1, "Regional_Indicator") == ZEPHYRUM)
    {
        redde (i16)V_RI;
    }
    si (strcmp(v1, "V") == ZEPHYRUM || strcmp(v1, "T") == ZEPHYRUM)
    {
        redde (i16)V_MEDIAE_FINALES;
    }
    si (strcmp(v1, "Prepend") == ZEPHYRUM)
    {
        redde (i16)V_PRAEPOSITUM;
    }
    redde ZEPHYRUM;
}

interior i16
_emoji (
    constans character* v1,
    constans character* v2,
                   i16* mascula)
{
    (vacuum)v2;
    si (strcmp(v1, "Emoji_Modifier") == ZEPHYRUM)
    {
        *mascula = (i16)V_MODIFICATOR;
        redde (i16)V_MODIFICATOR;
    }
    *mascula = ZEPHYRUM;
    redde ZEPHYRUM;
}


/* ==================================================
 * Regula latitudinis (Ghostty)
 * ================================================== */

interior i8
_latitudo (
    s32 runa,
    i16 v)
{
    i32 solitaria;
    b32 nulla_in_graphemate;

    /* 1. latitudo solitaria (uucode wcwidth_standalone) */
    si (v & V_CATEGORIA_NULLA)
    {
        solitaria = ZEPHYRUM;
    }
    alioquin si (runa == 0x00AD)
    {
        solitaria = I;
    }
    alioquin si (v & V_IGNORABILIS)
    {
        solitaria = ZEPHYRUM;
    }
    alioquin si (runa == 0x2E3A)
    {
        solitaria = II;
    }
    alioquin si (runa == 0x2E3B)
    {
        solitaria = III;
    }
    alioquin si (v & V_LATA)
    {
        solitaria = II;
    }
    alioquin si (v & V_RI)
    {
        solitaria = II;
    }
    alioquin
    {
        solitaria = I;
    }
    si (runa == 0x20E3)
    {
        solitaria = II;
    }

    /* 2. nulla in graphemate (uucode wcwidth_zero_in_grapheme) */
    nulla_in_graphemate = (b32)(   solitaria == ZEPHYRUM
                                || (v & V_MODIFICATOR)
                                || (v & V_SIGNUM_NM)
                                || (v & V_MEDIAE_FINALES)
                                || (v & V_PRAEPOSITUM));

    /* 3. Ghostty WidthComponent */
    si (   nulla_in_graphemate && !(v & V_MODIFICATOR)
        && !(v & V_PRAEPOSITUM))
    {
        redde ZEPHYRUM;
    }
    redde (i8)((solitaria > II) ? II : solitaria);
}


/* ==================================================
 * Tabula et emissio
 * ================================================== */

interior vacuum
_tabulam_struere (vacuum)
{
    i32 t;
    i32 u;

    per (t = ZEPHYRUM; t < TRUNCI_PRIMI; t++)
    {
        per (u = ZEPHYRUM; u < numerus_unicorum; u++)
        {
            si (memcmp(&valores[t * TRUNCUS],
                    &valores[(i32)trunci_unici[u] * TRUNCUS], TRUNCUS)
                == ZEPHYRUM)
            {
                frange;
            }
        }
        si (u == numerus_unicorum)
        {
            trunci_unici[numerus_unicorum++] = (i16)t;
        }
        gradus_primus[t] = (i16)u;
    }
}

interior b32
_emittere (
    constans character* via,
    constans character* versio)
{
    FILE* f;
     i32  t;
     i32  k;

    f = fopen(via, "w");
    si (f == NIHIL)
    {
        fprintf(stderr, "runae_generare: scribi non potest: %s\n", via);
        redde FALSUM;
    }
    fprintf(f,
        "/* runae_tabulae.c - GENERATA: tools/runae_generare.sh (NE EDITA MANU)\n"
        " *\n"
        " * Ex datis Unicode %s (probationes/fixa/unicode/%s/). Tabula duorum\n"
        " * graduum (include/runae_tabulae.h): %u trunci unici CCLVI octetorum.\n"
        " * Valor: bits 0-1 latitudo (regula Ghostty, lib/runae.phase-log.md U2).\n"
        " */\n"
        "\n"
        "#include \"runae_tabulae.h\"\n"
        "\n"
        "constans i16 RUNAE_GRADUS_PRIMUS[RUNAE_TRUNCI_PRIMI] = {\n",
        versio, versio, (insignatus integer)numerus_unicorum);
    per (t = ZEPHYRUM; t < TRUNCI_PRIMI; t++)
    {
        fprintf(f, "%s%u%s", (t % XVI == ZEPHYRUM) ? "    " : "",
            (insignatus integer)gradus_primus[t],
            (t == TRUNCI_PRIMI - I) ? "\n"
                : ((t % XVI == XV) ? ",\n" : ", "));
    }
    fprintf(f, "};\n\nconstans i8 RUNAE_GRADUS_SECUNDUS[%u] = {\n",
        (insignatus integer)(numerus_unicorum * TRUNCUS));
    per (t = ZEPHYRUM; t < numerus_unicorum; t++)
    {
        constans i8* truncus = &valores[(i32)trunci_unici[t] * TRUNCUS];

        fprintf(f, "    /* truncus %u (ex U+%04X) */\n",
            (insignatus integer)t,
            (insignatus integer)((i32)trunci_unici[t] * TRUNCUS));
        per (k = ZEPHYRUM; k < TRUNCUS; k++)
        {
            b32 ultimum = (b32)(t == numerus_unicorum - I
                                && k == TRUNCUS - I);

            fprintf(f, "%s%u%s", (k % XXXII == ZEPHYRUM) ? "    " : "",
                (insignatus integer)truncus[k],
                ultimum ? "\n" : ((k % XXXII == XXXI) ? ",\n" : ","));
        }
    }
    fprintf(f, "};\n");
    fclose(f);
    redde VERUM;
}


integer
principale (
      integer   argc,
    character** argv)
{
                   s32  runa;
             character  versio[LINEA_MAXIMA];
    constans character* titulus_radicis;

    si (argc != III)
    {
        fprintf(stderr,
            "usus: runae_generare <radix_ucd> <exitus.c>\n");
        redde II;
    }
    si (   !_filum_legere(argv[I], "DerivedGeneralCategory.txt",
        _categoria)
        || !_filum_legere(argv[I], "EastAsianWidth.txt",
               _latitudo_orientalis)
        || !_filum_legere(argv[I], "DerivedCoreProperties.txt",
               _proprietates_nuclei)
        || !_filum_legere(argv[I], "GraphemeBreakProperty.txt",
               _ruptura_graphematis)
        || !_filum_legere(argv[I], "emoji-data.txt", _emoji))
    {
        redde I;
    }
    per (runa = ZEPHYRUM; runa < RUNAE_NUMERUS; runa++)
    {
        valores[runa] = _latitudo(runa, vexilla[runa]);
    }
    _tabulam_struere();

    /* versio = nomen plicaturae radicis (…/unicode/15.1.0) */
    titulus_radicis = strrchr(argv[I], '/');
    titulus_radicis = (titulus_radicis != NIHIL) ? titulus_radicis
        + I : argv[I];
    strncpy(versio, titulus_radicis, LINEA_MAXIMA - I);
    versio[LINEA_MAXIMA - I] = '\0';

    si (!_emittere(argv[II], versio))
    {
        redde I;
    }
    fprintf(stderr, "runae_generare: %s (%u trunci unici)\n", argv[II],
        (insignatus integer)numerus_unicorum);
    redde ZEPHYRUM;
}
