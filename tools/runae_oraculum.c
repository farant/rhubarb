/* runae_oraculum.c - Oraculum ICU4C pro runae (U3)
 *
 * Usus:
 *   runae_oraculum                    omnem codicem contra runae_latitudo
 *   runae_oraculum -aurum <via>       aurum intervallorum scribere
 *   runae_oraculum -opentui <via.zon> mappam OpenTUI conferre (politica)
 *   optiones: -bibliotheca <via.dylib> -suffixum <_74>
 *
 * ICU4C (Homebrew, 74.2, Unicode 15.1) per dlopen/dlsym TITULIS
 * CHORDARUM aperitur: nulla capita ICU (C99/C++, vexilla domus non
 * ferunt), nulla identificatoria Anglica in fonte. Valores
 * enumerationum manu ex unicode/uchar.h 74.2 lecti (vide definitiones).
 * Regula latitudinis EADEM ac tools/runae_generare.c (Ghostty) - sed
 * proprietates ex ICU, non ex fasciculis UCD nostris: derivatio
 * independens tabulae. Regula ipsa utrique communis est (AUDIENDA).
 *
 * Instrumentum aetatis evolutionis; tools/runae_oraculum.sh struit.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "runae.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUNAE_NUMERUS 0x110000
#define LINEA_MAXIMA 256
#define DISCORDIAE_IMPRIMENDAE XX

/* unicode/uchar.h (ICU 74.2): UProperty */
#define PROPRIETAS_IGNORABILIS          5       /* UCHAR_DEFAULT_IGNORABLE_CODE_POINT */
#define PROPRIETAS_MODIFICATOR          59      /* UCHAR_EMOJI_MODIFIER */
#define PROPRIETAS_LATITUDO_ORIENTALIS  0x1004  /* UCHAR_EAST_ASIAN_WIDTH */
#define PROPRIETAS_RUPTURA              0x1012  /* UCHAR_GRAPHEME_CLUSTER_BREAK */
/* UCharCategory */
#define CATEGORIA_SIGNUM_INTERIUS       6       /* U_NON_SPACING_MARK (Mn) */
#define CATEGORIA_SIGNUM_CIRCUMDANS     7       /* U_ENCLOSING_MARK (Me) */
#define CATEGORIA_SIGNUM_SPATIANS       8       /* U_COMBINING_SPACING_MARK (Mc) */
#define CATEGORIA_SEPARATOR_LINEAE      13      /* U_LINE_SEPARATOR (Zl) */
#define CATEGORIA_SEPARATOR_PARAGRAPHI  14      /* U_PARAGRAPH_SEPARATOR (Zp) */
#define CATEGORIA_REGIMEN               15      /* U_CONTROL_CHAR (Cc) */
#define CATEGORIA_SURROGATUM            18      /* U_SURROGATE (Cs) */
/* UEastAsianWidth */
#define ORIENTALIS_PLENA                3       /* U_EA_FULLWIDTH (F) */
#define ORIENTALIS_LATA                 5       /* U_EA_WIDE (W) */
/* UGraphemeClusterBreak */
#define RUPTURA_FINALIS                 8       /* U_GCB_T */
#define RUPTURA_MEDIA                   9       /* U_GCB_V */
#define RUPTURA_PRAEPOSITA              11      /* U_GCB_PREPEND */
#define RUPTURA_REGIONIS              12      /* U_GCB_REGIONAL_INDICATOR */

nomen s8     (*FunctioCategoriae)(s32 runa);
nomen s8     (*FunctioBinaria)(s32 runa, integer proprietas);
nomen s32    (*FunctioIntegra)(s32 runa, integer proprietas);
nomen vacuum (*FunctioVersionis)(i8* versio);

hic_manens FunctioCategoriae  categoria_oraculi;
hic_manens FunctioBinaria     binaria_oraculi;
hic_manens FunctioIntegra     integra_oraculi;
hic_manens FunctioVersionis   versio_oraculi;


/* ==================================================
 * Oraculum aperire
 * ================================================== */

/* dlsym indicatorem obiecti reddit; ISO C conversionem ad functionem
 * vetat - per memcpy transfertur (magnitudo aequalis in Darwin) */
interior b32
_symbolum (
                 vacuum* bibliotheca,
     constans character* titulus,
     constans character* suffixum,
                 vacuum* functio,
                    i32  mensura_functionis)
{
    character  plenum[LINEA_MAXIMA];
       vacuum* symbolum;

    sprintf(plenum, "%s%s", titulus, suffixum);
    symbolum = dlsym(bibliotheca, plenum);
    si (   symbolum           == NIHIL
        || mensura_functionis != (i32)magnitudo(symbolum))
    {
        fprintf(stderr, "runae_oraculum: symbolum absens: %s\n",
            plenum);
        redde FALSUM;
    }
    memcpy(functio, &symbolum, (memoriae_index)mensura_functionis);
    redde VERUM;
}

interior b32
_oraculum_aperire (
    constans character* via,
    constans character* suffixum)
{
    vacuum* bibliotheca = dlopen(via, RTLD_NOW);

    si (bibliotheca == NIHIL)
    {
        fprintf(stderr, "runae_oraculum: aperiri non potest: %s\n",
            via);
        redde FALSUM;
    }
    redde _symbolum(bibliotheca, "u_charType", suffixum,
               &categoria_oraculi, (i32)magnitudo(categoria_oraculi))
        && _symbolum(bibliotheca, "u_hasBinaryProperty", suffixum,
               &binaria_oraculi, (i32)magnitudo(binaria_oraculi))
        && _symbolum(bibliotheca, "u_getIntPropertyValue", suffixum,
               &integra_oraculi, (i32)magnitudo(integra_oraculi))
        && _symbolum(bibliotheca, "u_getUnicodeVersion", suffixum,
               &versio_oraculi, (i32)magnitudo(versio_oraculi));
}


/* ==================================================
 * Regula latitudinis (Ghostty) super proprietates ICU
 * ================================================== */

interior i32
_latitudo_oraculi (
    s32 runa)
{
    s32 categoria = (s32)categoria_oraculi(runa);
    b32 ignorabilis = (b32)binaria_oraculi(runa,
        PROPRIETAS_IGNORABILIS);
    b32 modificator = (b32)binaria_oraculi(runa,
        PROPRIETAS_MODIFICATOR);
    s32 orientalis = integra_oraculi(runa,
        PROPRIETAS_LATITUDO_ORIENTALIS);
    s32 ruptura = integra_oraculi(runa, PROPRIETAS_RUPTURA);
    i32 solitaria;
    b32 nulla_in_graphemate;

    si (   categoria == CATEGORIA_REGIMEN
        || categoria == CATEGORIA_SURROGATUM
        || categoria == CATEGORIA_SEPARATOR_LINEAE
        || categoria == CATEGORIA_SEPARATOR_PARAGRAPHI)
    {
        solitaria = ZEPHYRUM;
    }
    alioquin si (runa == 0x00AD)
    {
        solitaria = I;
    }
    alioquin si (ignorabilis)
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
    alioquin si (   orientalis == ORIENTALIS_LATA
                 || orientalis == ORIENTALIS_PLENA)
    {
        solitaria = II;
    }
    alioquin si (ruptura == RUPTURA_REGIONIS)
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
    nulla_in_graphemate = (b32)(   solitaria == ZEPHYRUM || modificator
                                || categoria
                                    == CATEGORIA_SIGNUM_INTERIUS
                                || categoria
                                    == CATEGORIA_SIGNUM_CIRCUMDANS
                                || ruptura == RUPTURA_MEDIA
                                || ruptura == RUPTURA_FINALIS
                                || ruptura == RUPTURA_PRAEPOSITA);
    si (   nulla_in_graphemate && !modificator
        && ruptura != RUPTURA_PRAEPOSITA)
    {
        redde ZEPHYRUM;
    }
    redde (solitaria > II) ? II : solitaria;
}

/* Causa discordiae cum mappa aliena: classis runae ex ICU */
interior constans character*
_classis (
    s32 runa)
{
    s32 categoria = (s32)categoria_oraculi(runa);

    si (categoria == CATEGORIA_REGIMEN)
    {
        redde "regimen (Cc)";
    }
    si (categoria == CATEGORIA_SIGNUM_SPATIANS)
    {
        redde "signum spatians (Mc)";
    }
    si (   categoria == CATEGORIA_SIGNUM_INTERIUS
        || categoria == CATEGORIA_SIGNUM_CIRCUMDANS)
    {
        redde "signum (Mn/Me)";
    }
    si (binaria_oraculi(runa, PROPRIETAS_IGNORABILIS))
    {
        redde "ignorabilis (DI)";
    }
    si (integra_oraculi(runa, PROPRIETAS_RUPTURA) == RUPTURA_REGIONIS)
    {
        redde "indicator regionalis (RI)";
    }
    si (integra_oraculi(runa, PROPRIETAS_RUPTURA) == RUPTURA_PRAEPOSITA)
    {
        redde "praepositum (Prepend)";
    }
    redde "cetera";
}


/* ==================================================
 * Modi
 * ================================================== */

/* Omnis codex: oraculum contra tabulam */
interior integer
_conferre (vacuum)
{
    s32 runa;
    i32 discordiae = ZEPHYRUM;

    per (runa = ZEPHYRUM; runa < RUNAE_NUMERUS; runa++)
    {
        i32 nostra   = runae_latitudo(runa);
        i32 oraculi  = _latitudo_oraculi(runa);

        si (nostra != oraculi)
        {
            si (discordiae < DISCORDIAE_IMPRIMENDAE)
            {
                printf("  DISCORDIA U+%04X: runae %u, oraculum %u (%s)\n",
                    (insignatus integer)runa,
                    (insignatus integer)nostra,
                    (insignatus integer)oraculi, _classis(runa));
            }
            discordiae++;
        }
    }
    printf("runae_oraculum: %u codices collati, %u discordiae\n",
        (insignatus integer)RUNAE_NUMERUS,
        (insignatus integer)discordiae);
    redde (discordiae == ZEPHYRUM) ? ZEPHYRUM : I;
}

/* Aurum: intervalla latitudinis aequalis (PRIMA..ULTIMA LATITUDO) */
interior integer
_aurum_scribere (
    constans character* via,
    constans character* versio)
{
    FILE* f        = fopen(via, "w");
     s32  initium  = ZEPHYRUM;
     s32  runa;
     s32  latitudo;
     i32  intervalla = ZEPHYRUM;

    si (f == NIHIL)
    {
        fprintf(stderr, "runae_oraculum: scribi non potest: %s\n", via);
        redde I;
    }
    fprintf(f,
        "# runae aurum: latitudo cuiusque codicis per ORACULUM ICU4C\n"
        "# (Unicode %s), regula Ghostty super proprietates ICU - derivatio\n"
        "# independens a tabula nostra (fasciculi UCD). Scriptum a\n"
        "# tools/runae_oraculum.sh -aurum; iudicatur in probatio_runae.c.\n"
        "# Linea: PRIMA..ULTIMA LATITUDO (hex, intervalla contigua).\n",
        versio);
    latitudo = (s32)_latitudo_oraculi(ZEPHYRUM);
    per (runa = I; runa <= RUNAE_NUMERUS; runa++)
    {
        s32 l = (runa
            < RUNAE_NUMERUS) ? (s32)_latitudo_oraculi(runa) : -I;

        si (runa == RUNAE_NUMERUS || l != latitudo)
        {
            fprintf(f, "%04X..%04X %u\n", (insignatus integer)initium,
                (insignatus integer)(runa - I),
                (insignatus integer)latitudo);
            intervalla++;
            initium   = runa;
            latitudo  = l;
        }
    }
    fclose(f);
    printf("runae_oraculum: %s (%u intervalla)\n", via,
        (insignatus integer)intervalla);
    redde ZEPHYRUM;
}

/* Mappa OpenTUI: '.{ .codepoint = "U+XXXX", .width = N },' */
interior integer
_mappam_alienam_conferre (
    constans character* via)
{
         FILE* f = fopen(via, "r");
    character  linea[LINEA_MAXIMA];
          i32  introitus   = ZEPHYRUM;
          i32  discordiae  = ZEPHYRUM;

    si (f == NIHIL)
    {
        fprintf(stderr, "runae_oraculum: legi non potest: %s\n", via);
        redde II;
    }
    dum (fgets(linea, LINEA_MAXIMA, f) != NIHIL)
    {
        constans character* u = strstr(linea, "\"U+");
        constans character* w = strstr(linea, ".width = ");
                       s32  runa;
                       i32  illorum;
                       i32  nostra;

        si (u == NIHIL || w == NIHIL)
        {
            perge;
        }
        runa     = (s32)strtol(u + III, NIHIL, XVI);
        illorum  = (i32)strtol(w + IX, NIHIL, X);
        nostra   = runae_latitudo(runa);
        introitus++;
        si (nostra != illorum)
        {
            printf("  POLITICA U+%04X: runae %u, OpenTUI %u - %s\n",
                (insignatus integer)runa, (insignatus integer)nostra,
                (insignatus integer)illorum, _classis(runa));
            discordiae++;
        }
    }
    fclose(f);
    printf("runae_oraculum: OpenTUI %u introitus, %u discordiae\n",
        (insignatus integer)introitus, (insignatus integer)discordiae);
    redde ZEPHYRUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
    constans character* bibliotheca =
        "/opt/homebrew/opt/icu4c/lib/libicuuc.74.dylib";
     constans character* suffixum      = "_74";
     constans character* aurum         = NIHIL;
     constans character* mappa_aliena  = NIHIL;
                     i8  versio[IV];
              character  versio_textus[XXXII];
                integer  i;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-aurum") == ZEPHYRUM && i + I < argc)
        {
            aurum = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-opentui") == ZEPHYRUM
                     && i + I < argc)
        {
            mappa_aliena = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-bibliotheca") == ZEPHYRUM
                     && i + I < argc)
        {
            bibliotheca = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-suffixum") == ZEPHYRUM
                     && i + I < argc)
        {
            suffixum = argv[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: runae_oraculum [-aurum via] [-opentui "
                "via.zon] [-bibliotheca via] [-suffixum _NN]\n");
            redde II;
        }
    }
    si (!_oraculum_aperire(bibliotheca, suffixum))
    {
        redde II;
    }

    /* versio Unicode oraculi == versio tabularum (sine ea collatio
     * discordias versionis, non vitia, numeraret) */
    versio_oraculi(versio);
    sprintf(versio_textus, "%u.%u.%u",
        (insignatus integer)versio[ZEPHYRUM],
        (insignatus integer)versio[I], (insignatus integer)versio[II]);
    printf("runae_oraculum: ICU Unicode %s, runae %s\n", versio_textus,
        RUNAE_VERSIO);
    si (strcmp(versio_textus, RUNAE_VERSIO) != ZEPHYRUM)
    {
        fprintf(stderr, "runae_oraculum: versiones discrepant\n");
        redde II;
    }

    si (aurum != NIHIL)
    {
        redde _aurum_scribere(aurum, versio_textus);
    }
    si (mappa_aliena != NIHIL)
    {
        redde _mappam_alienam_conferre(mappa_aliena);
    }
    redde _conferre();
}
