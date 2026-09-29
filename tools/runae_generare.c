/* runae_generare.c - Tabulas runarum GENERARE ex datis Unicode
 *
 * Usus: runae_generare <radix_ucd> <exitus.c>
 *
 * Legit (radix_ucd = probationes/fixa/unicode/<versio>/):
 *   DerivedGeneralCategory.txt    - Cc Cs Zl Zp, Mn Me
 *   EastAsianWidth.txt            - W, F
 *   DerivedCoreProperties.txt     - Default_Ignorable_Code_Point, InCB
 *   GraphemeBreakProperty.txt     - Grapheme_Cluster_Break (totum)
 *   emoji-data.txt                - Emoji_Modifier, Extended_Pictographic
 *   emoji-variation-sequences.txt - bases variationis emoji
 * et scribit lib/runae_tabulae.c (tabula duorum graduum; dispositio
 * valoris in include/runae_tabulae.h, capite COMMUNI). Lineae
 * '# @missing:' ut lineae datorum tractantur (valor ordinarius) et ANTE
 * data PROPRIETATIS SUAE stare debent (DerivedCoreProperties: @missing
 * InCB in medio, post data DI - licet). Contentiones classium
 * (Extended_Pictographic in GCB Other etc.) ASSERUNTUR: versio Unicode
 * quae eas frangit generatorem sistit.
 *
 * REGULA LATITUDINIS = Ghostty (uucode wcwidth_standalone +
 * wcwidth_zero_in_grapheme, uucode_config.zig WidthComponent); vide
 * lib/runae.phase-log.md U2 et _latitudo infra.
 *
 * Instrumentum aetatis evolutionis; tools/runae_generare.sh id struit
 * et currit (-probare: tabula commissa recens?).
 */
#include "latina.h"
#include "runae_tabulae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNAE_NUMERUS 0x110000
#define TRUNCUS 256
#define TRUNCI_PRIMI (RUNAE_NUMERUS / TRUNCUS)
#define LINEA_MAXIMA 1024

/* Vexilla proprietatum per runam */
#define V_CATEGORIA_NULLA    0x0001   /* Cc Cs Zl Zp */
#define V_SIGNUM_NM          0x0002   /* Mn Me */
#define V_LATA               0x0004   /* EAW W aut F */
#define V_IGNORABILIS        0x0008   /* Default_Ignorable_Code_Point */
#define V_MODIFICATOR        0x0010   /* Emoji_Modifier */
#define V_PICTOGRAPHUM       0x0020   /* Extended_Pictographic */
#define V_BASIS_VARIATIONIS  0x0040   /* basis emoji-variation-sequences */

/* Campus Grapheme_Cluster_Break (bits 8-11) et InCB (bits 12-13) */
#define C_RUPTURA_POSITIO    8
#define C_RUPTURA_MASCULA    0x0F00
#define C_INCB_POSITIO       12
#define C_INCB_MASCULA       0x3000

/* Valores GCB (ordo = TITULI_RUPTURAE infra) */
nomen enumeratio {
    R_ALIA = 0, R_CR, R_LF, R_REGIMEN, R_EXTENSIO, R_IUNCTOR,
    R_REGIONIS, R_PRAEPOSITUM, R_SPATIANS, R_INITIALIS, R_MEDIA,
    R_FINALIS, R_APERTA, R_CLAUSA
} Ruptura;

nomen enumeratio {
    INCB_NULLA = 0, INCB_CONSONANS, INCB_EXTENSIO, INCB_CONIUNCTOR
} Incb;

#define RUPTURA(v) ((i32)(((v) & C_RUPTURA_MASCULA) >> C_RUPTURA_POSITIO))
#define INCB(v)    ((i32)(((v) & C_INCB_MASCULA) >> C_INCB_POSITIO))

hic_manens constans character* TITULI_RUPTURAE[] = {
    "Other", "CR", "LF", "Control", "Extend", "ZWJ",
    "Regional_Indicator", "Prepend", "SpacingMark", "L", "V", "T",
    "LV", "LVT"
};

hic_manens i32 vexilla[RUNAE_NUMERUS];
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

/* Proprietas: vexilla/campos ponere pro intervallo; mascula = bits quos
 * linea haec REGIT (purgantur ante positionem) */
nomen i32 (*Interpres)(constans character* v1, constans character* v2,
    i32* mascula);

interior b32
_filum_legere (
     constans character* radix,
     constans character* titulus,
              Interpres  interpres)
{
    character via[LINEA_MAXIMA];
    character linea[LINEA_MAXIMA];
        FILE* f;
          i32 visa = ZEPHYRUM;   /* masculae proprietatum iam visarum */
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
         character* puncta;
         character* spatium;
               i32  n;
               s32  prima;
               s32  ultima;
               i32  mascula;
               i32  positum;
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
        /* series variationis "0023 FE0E": runa prima sola */
        spatium = strchr(campi[ZEPHYRUM], ' ');
        si (spatium != NIHIL)
        {
            *spatium = '\0';
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
        /* @missing post data SUAE proprietatis ea deleret; proprietatis
         * ALIENAE (InCB post DI in DerivedCoreProperties) licet */
        si (textus != linea && (mascula & visa) != ZEPHYRUM)
        {
            fprintf(stderr,
                "runae_generare: %s:%u: @missing post data\n",
                via, (insignatus integer)numerus_linea);
            fclose(f);
            redde FALSUM;
        }
        si (textus == linea)
        {
            visa |= mascula;
        }
        per (r = prima; r <= ultima; r++)
        {
            vexilla[r] = (vexilla[r] & ~mascula) | positum;
        }
    }
    fclose(f);
    redde VERUM;
}


/* ==================================================
 * Interpretes per filum
 * ================================================== */

interior i32
_categoria (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    (vacuum)v2;
    *mascula = V_CATEGORIA_NULLA | V_SIGNUM_NM;
    si (   strcmp(v1, "Cc") == ZEPHYRUM || strcmp(v1, "Cs") == ZEPHYRUM
        || strcmp(v1, "Zl") == ZEPHYRUM || strcmp(v1, "Zp") == ZEPHYRUM)
    {
        redde V_CATEGORIA_NULLA;
    }
    si (strcmp(v1, "Mn") == ZEPHYRUM || strcmp(v1, "Me") == ZEPHYRUM)
    {
        redde V_SIGNUM_NM;
    }
    redde ZEPHYRUM;
}

interior i32
_latitudo_orientalis (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    (vacuum)v2;
    *mascula = V_LATA;
    redde (strcmp(v1, "W") == ZEPHYRUM || strcmp(v1, "F") == ZEPHYRUM)
        ? V_LATA : ZEPHYRUM;
}

interior i32
_proprietates_nuclei (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    si (strcmp(v1, "Default_Ignorable_Code_Point") == ZEPHYRUM)
    {
        *mascula = V_IGNORABILIS;
        redde V_IGNORABILIS;
    }
    si (strcmp(v1, "InCB") == ZEPHYRUM)
    {
        i32 valor = INCB_NULLA;

        *mascula = C_INCB_MASCULA;
        si (strcmp(v2, "Consonant") == ZEPHYRUM)
        {
            valor = INCB_CONSONANS;
        }
        alioquin si (strcmp(v2, "Extend") == ZEPHYRUM)
        {
            valor = INCB_EXTENSIO;
        }
        alioquin si (strcmp(v2, "Linker") == ZEPHYRUM)
        {
            valor = INCB_CONIUNCTOR;
        }
        redde valor << C_INCB_POSITIO;
    }
    *mascula = ZEPHYRUM;   /* ceterae non reguntur */
    redde ZEPHYRUM;
}

interior i32
_ruptura_graphematis (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    i32 k;

    (vacuum)v2;
    *mascula = C_RUPTURA_MASCULA;
    per (k = ZEPHYRUM;
         k < (i32)(magnitudo(TITULI_RUPTURAE)
             / magnitudo(TITULI_RUPTURAE[0]));
         k++)
    {
        si (strcmp(v1, TITULI_RUPTURAE[k]) == ZEPHYRUM)
        {
            redde k << C_RUPTURA_POSITIO;
        }
    }
    fprintf(stderr,
        "runae_generare: Grapheme_Cluster_Break ignotum: %s\n",
        v1);
    exit(I);
    redde ZEPHYRUM;   /* numquam attingitur (exit) */
}

interior i32
_emoji (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    (vacuum)v2;
    si (strcmp(v1, "Emoji_Modifier") == ZEPHYRUM)
    {
        *mascula = V_MODIFICATOR;
        redde V_MODIFICATOR;
    }
    si (strcmp(v1, "Extended_Pictographic") == ZEPHYRUM)
    {
        *mascula = V_PICTOGRAPHUM;
        redde V_PICTOGRAPHUM;
    }
    *mascula = ZEPHYRUM;
    redde ZEPHYRUM;
}

interior i32
_variationes (
    constans character* v1,
    constans character* v2,
                   i32* mascula)
{
    (vacuum)v1;
    (vacuum)v2;
    *mascula = V_BASIS_VARIATIONIS;
    redde V_BASIS_VARIATIONIS;
}


/* ==================================================
 * Regula latitudinis (Ghostty)
 * ================================================== */

interior i32
_latitudo (
    s32 runa,
    i32 v)
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
    alioquin si (RUPTURA(v) == R_REGIONIS)
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
                                || RUPTURA(v) == R_MEDIA
                                || RUPTURA(v) == R_FINALIS
                                || RUPTURA(v) == R_PRAEPOSITUM);

    /* 3. Ghostty WidthComponent */
    si (   nulla_in_graphemate && !(v & V_MODIFICATOR)
        && RUPTURA(v) != R_PRAEPOSITUM)
    {
        redde ZEPHYRUM;
    }
    redde (solitaria > II) ? II : solitaria;
}


/* ==================================================
 * Classis rupturae (RunaeClassis) cum assertionibus contentionis
 * ================================================== */

interior vacuum
_contentio (
                   b32  vera,
                   s32  runa,
    constans character* quid)
{
    si (!vera)
    {
        fprintf(stderr,
            "runae_generare: contentio fracta ad U+%04X: %s "
            "(versio Unicode nova? RunaeClassis retractanda)\n",
            (insignatus integer)runa, quid);
        exit(I);
    }
}

interior i32
_classis (
    s32 runa,
    i32 v)
{
    i32 ruptura       = RUPTURA(v);
    i32 incb          = INCB(v);
    b32 pictographum  = (b32)((v & V_PICTOGRAPHUM) != ZEPHYRUM);
    b32 modificator   = (b32)((v & V_MODIFICATOR) != ZEPHYRUM);

    _contentio(!pictographum || ruptura == R_ALIA, runa,
        "Extended_Pictographic extra GCB Other");
    _contentio(!pictographum || incb == INCB_NULLA, runa,
        "Extended_Pictographic cum InCB");
    _contentio(incb != INCB_CONSONANS || ruptura == R_ALIA, runa,
        "InCB Consonant extra GCB Other");
    _contentio(incb != INCB_EXTENSIO || ruptura == R_EXTENSIO
               || ruptura == R_IUNCTOR, runa,
               "InCB Extend extra Extend/ZWJ");
    _contentio(incb != INCB_CONIUNCTOR || ruptura == R_EXTENSIO, runa,
        "InCB Linker extra GCB Extend");
    _contentio(ruptura != R_IUNCTOR || incb == INCB_EXTENSIO, runa,
        "ZWJ sine InCB Extend");
    _contentio(!modificator
        || (ruptura == R_EXTENSIO && incb == INCB_NULLA), runa,
        "Emoji_Modifier extra Extend (InCB None)");
    _contentio(!(v & V_BASIS_VARIATIONIS) || ruptura == R_ALIA, runa,
        "basis variationis extra GCB Other");

    commutatio (ruptura)
    {
        casus R_ALIA:
            si (pictographum)
            {
                redde RUNAE_CLASSIS_PICTOGRAPHUM;
            }
            redde (incb == INCB_CONSONANS) ? RUNAE_CLASSIS_CONSONANS
                                           : RUNAE_CLASSIS_ALIA;
        casus R_CR:
            redde RUNAE_CLASSIS_CR;
        casus R_LF:
            redde RUNAE_CLASSIS_LF;
        casus R_REGIMEN:
            redde RUNAE_CLASSIS_REGIMEN;
        casus R_EXTENSIO:
            si (modificator)
            {
                redde RUNAE_CLASSIS_MODIFICATOR;
            }
            si (incb == INCB_EXTENSIO)
            {
                redde RUNAE_CLASSIS_EXTENSIO_INCB;
            }
            redde (incb == INCB_CONIUNCTOR) ? RUNAE_CLASSIS_CONIUNCTOR
                                            : RUNAE_CLASSIS_EXTENSIO;
        casus R_IUNCTOR:
            redde RUNAE_CLASSIS_IUNCTOR;
        casus R_REGIONIS:
            redde RUNAE_CLASSIS_REGIONIS;
        casus R_PRAEPOSITUM:
            redde RUNAE_CLASSIS_PRAEPOSITUM;
        casus R_SPATIANS:
            redde RUNAE_CLASSIS_SPATIANS;
        casus R_INITIALIS:
            redde RUNAE_CLASSIS_SYLLABA_INITIALIS;
        casus R_MEDIA:
            redde RUNAE_CLASSIS_SYLLABA_MEDIA;
        casus R_FINALIS:
            redde RUNAE_CLASSIS_SYLLABA_FINALIS;
        casus R_APERTA:
            redde RUNAE_CLASSIS_SYLLABA_APERTA;
        ordinarius:
            redde RUNAE_CLASSIS_SYLLABA_CLAUSA;
    }
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
        " * Valor: bits 0-1 latitudo (regula Ghostty, lib/runae.phase-log.md U2),\n"
        " * bits 2-6 classis rupturae, bit 7 basis variationis (runae_tabulae.h).\n"
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

            fprintf(f, "%s%u%s", (k % XVI == ZEPHYRUM) ? "    " : "",
                (insignatus integer)truncus[k],
                ultimum ? "\n" : ((k % XVI == XV) ? ",\n" : ","));
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
        || !_filum_legere(argv[I], "emoji-data.txt", _emoji)
        || !_filum_legere(argv[I], "emoji-variation-sequences.txt",
               _variationes))
    {
        redde I;
    }
    per (runa = ZEPHYRUM; runa < RUNAE_NUMERUS; runa++)
    {
        i32 v = vexilla[runa];

        valores[runa] = (i8)(  (i32)_latitudo(runa, v)
                             | (_classis(runa, v)
                                 << RUNAE_CLASSIS_POSITIO)
                             | ((v & V_BASIS_VARIATIONIS)
                                    ? RUNAE_BASIS_VARIATIONIS : ZEPHYRUM));
    }
    _tabulam_struere();

    /* versio = nomen plicaturae radicis (…/unicode/15.1.0) */
    titulus_radicis = strrchr(argv[I], '/');
    titulus_radicis = (titulus_radicis != NIHIL) ? titulus_radicis + I
                                                 : argv[I];
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
