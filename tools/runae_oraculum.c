/* runae_oraculum.c - Oraculum ICU4C pro runae (U3)
 *
 * Usus:
 *   runae_oraculum                    omnem codicem contra runae_latitudo
 *   runae_oraculum -aurum <via>       aurum intervallorum scribere
  *   runae_oraculum -opentui <via.zon> mappam OpenTUI conferre (politica)
 *   runae_oraculum -graphemata <via>  limites graphematum corporis contra
 *                                     iteratorem ICU; aurum scribere
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
#include "utf8.h"
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

/* Iterator rupturae ICU (ubrk_*, utext_*): textus UTF-8 per UText, ergo
 * limites = offsets OCTETORUM. int64_t = longus (LP64, Darwin). */
nomen vacuum* (*FunctioIteratoris)(integer genus,
    constans character* locus,
    constans vacuum* textus, s32 longitudo, integer* status);
nomen vacuum  (*FunctioTextusPonendi)(vacuum* iterator, vacuum* textus,
    integer* status);
nomen s32     (*FunctioLimitis)(vacuum* iterator);
nomen vacuum  (*FunctioIteratorisClaudendi)(vacuum* iterator);
nomen vacuum* (*FunctioTextusOctetorum)(vacuum* textus,
    constans character* octeti, longus longitudo, integer* status);
nomen vacuum* (*FunctioTextusClaudendi)(vacuum* textus);

hic_manens FunctioIteratoris           iterator_aperire;
hic_manens FunctioTextusPonendi        iterator_textum_ponere;
hic_manens FunctioLimitis              iterator_primus;
hic_manens FunctioLimitis              iterator_proximus;
hic_manens FunctioIteratorisClaudendi  iterator_claudere;
hic_manens FunctioTextusOctetorum      textus_aperire;
hic_manens FunctioTextusClaudendi      textus_claudere;
hic_manens vacuum*                     bibliotheca_oraculi;
hic_manens constans character*         suffixum_oraculi;

#define RUPTURA_CHARACTERIS 0    /* UBRK_CHARACTER */
#define LIMES_NULLUS        (-1) /* UBRK_DONE */

/* Linguae corporis (probationes/fixa/runae/corpus/) */
hic_manens constans character* LINGUAE_CORPORIS[] = {
    "en", "la", "ar", "bn", "ceb", "de", "el", "es", "fa", "fr", "gu",
    "he", "hi", "hu", "id", "ig", "it", "ja", "ko", "ml", "nl", "pl",
    "pt", "ro", "ru", "rw", "sv", "sw", "ta", "th", "tl", "tr", "vi",
    "yo", "zh"
};


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
    bibliotheca_oraculi  = bibliotheca;
    suffixum_oraculi     = suffixum;
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

/* Iteratorem rupturae ICU aperire (solum pro -graphemata) */
interior b32
_iteratorem_parare (vacuum)
{
                vacuum* b = bibliotheca_oraculi;
    constans character* x = suffixum_oraculi;

    redde _symbolum(b, "ubrk_open", x, &iterator_aperire,
               (i32)magnitudo(iterator_aperire))
        && _symbolum(b, "ubrk_setUText", x, &iterator_textum_ponere,
               (i32)magnitudo(iterator_textum_ponere))
        && _symbolum(b, "ubrk_first", x, &iterator_primus,
               (i32)magnitudo(iterator_primus))
        && _symbolum(b, "ubrk_next", x, &iterator_proximus,
               (i32)magnitudo(iterator_proximus))
        && _symbolum(b, "ubrk_close", x, &iterator_claudere,
               (i32)magnitudo(iterator_claudere))
        && _symbolum(b, "utext_openUTF8", x, &textus_aperire,
               (i32)magnitudo(textus_aperire))
        && _symbolum(b, "utext_close", x, &textus_claudere,
               (i32)magnitudo(textus_claudere));
}

/* FNV-1a super IV octetos offseti (idem ac probatio_runae_graphemata) */
interior i32
_friare (
    i32 friatio,
    i32 valor)
{
    i32 k;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        friatio ^= (valor >> (k * VIII)) & 0xFF;
        friatio *= 16777619;
    }
    redde friatio;
}

/* Limites nostri (runae_rumpitur): 0, initia graphematum, finis */
interior i32
_limites_nostri (
    constans i8* octeti,
            i32  mensura,
            s32* limites)
{
     constans i8* cursor  = octeti;
     constans i8* finis   = octeti + mensura;
             s32  prior   = -I;
             i32  n       = ZEPHYRUM;
    RunaeRuptura  ruptura;

    runae_rupturam_initiare(&ruptura);
    dum (cursor < finis)
    {
        s32 offset  = (s32)(cursor - octeti);
        s32 runa    = utf8_decodere(&cursor, finis);

        si (prior == -I || runae_rumpitur(prior, runa, &ruptura))
        {
            limites[n++] = offset;
        }
        prior = runa;
    }
    limites[n++] = (s32)mensura;
    redde n;
}

/* Corpus totum: limites ICU contra nostros; aurum ICU scribere */
interior integer
_graphemata (
    constans character* via_auri)
{
    FILE* aurum = fopen(via_auri, "w");
     i32  j;
     i32  discordiae_totae = ZEPHYRUM;

    si (aurum == NIHIL || !_iteratorem_parare())
    {
        fprintf(stderr, "runae_oraculum: aurum aut iterator deficit\n");
        redde II;
    }
    fprintf(aurum,
        "# runae aurum graphematum: limites graphematum corporis Lapidis per\n"
        "# ORACULUM ICU4C (iterator characterum, textus UTF-8), scriptum a\n"
        "# tools/runae_oraculum.sh -graphemata; iudicatur in\n"
        "# probatio_runae_graphemata.c. Linea: LINGUA GRAPHEMATA FRIATIO\n"
        "# (FNV-1a super offsets octetorum limitum, 0 et finis inclusi).\n");
    per (j = ZEPHYRUM;
         j < (i32)(magnitudo(LINGUAE_CORPORIS)
             / magnitudo(LINGUAE_CORPORIS[0]));
         j++)
    {
        character via[LINEA_MAXIMA];
            FILE* f;
           longus mensura;
              i8* octeti;
             s32* nostri;
             s32* illorum;
              i32 numerus_nostri;
              i32 numerus_illorum = ZEPHYRUM;
          integer status = ZEPHYRUM;
          vacuum* textus;
          vacuum* iterator;
              s32 limes;
              i32 k;
              i32 discordiae = ZEPHYRUM;
              i32 friatio = 2166136261u;

        sprintf(via, "probationes/fixa/runae/corpus/%s.txt",
            LINGUAE_CORPORIS[j]);
        f = fopen(via, "rb");
        si (f == NIHIL)
        {
            fprintf(stderr, "runae_oraculum: legi non potest: %s\n",
                via);
            fclose(aurum);
            redde II;
        }
        (vacuum)fseek(f, 0L, SEEK_END);
        mensura = ftell(f);
        rewind(f);
        octeti = (i8*)malloc((size_t)mensura + I);
        nostri = (s32*)malloc(((size_t)mensura
            + II) * magnitudo(s32));
        illorum = (s32*)malloc(((size_t)mensura
            + II) * magnitudo(s32));
        si (   octeti == NIHIL || nostri == NIHIL || illorum == NIHIL
            || fread(octeti, I, (size_t)mensura, f) != (size_t)mensura)
        {
            fprintf(stderr,
                "runae_oraculum: memoria aut lectio deficit\n");
            fclose(f);
            fclose(aurum);
            redde II;
        }
        fclose(f);

        textus    = textus_aperire(NIHIL, (constans character*)octeti,
                        mensura, &status);
        iterator  = iterator_aperire(RUPTURA_CHARACTERIS, "en", NIHIL,
                        ZEPHYRUM, &status);
        iterator_textum_ponere(iterator, textus, &status);
        si (status > ZEPHYRUM)
        {
            fprintf(stderr, "runae_oraculum: ICU status %d\n", status);
            fclose(aurum);
            redde II;
        }
        per (limes = iterator_primus(iterator); limes != LIMES_NULLUS;
             limes = iterator_proximus(iterator))
        {
            illorum[numerus_illorum++]  = limes;
            friatio                     = _friare(friatio, (i32)limes);
        }
        iterator_claudere(iterator);
        (vacuum)textus_claudere(textus);

        numerus_nostri = _limites_nostri(octeti, (i32)mensura, nostri);
        per (k = ZEPHYRUM; k < numerus_nostri
            || k < numerus_illorum; k++)
        {
            s32 a = (k < numerus_nostri) ? nostri[k] : -I;
            s32 b = (k < numerus_illorum) ? illorum[k] : -I;

            si (a != b)
            {
                si (discordiae == ZEPHYRUM)
                {
                    printf("  DISCORDIA %s: limes %u: runae ad %d, ICU ad %d\n",
                        LINGUAE_CORPORIS[j], (insignatus integer)k,
                        (integer)a, (integer)b);
                }
                discordiae++;
            }
        }
        printf("  %-3s graphemata %u (runae %u), discordiae %u\n",
            LINGUAE_CORPORIS[j], (insignatus integer)(numerus_illorum
                - I),
            (insignatus integer)(numerus_nostri - I),
            (insignatus integer)discordiae);
        fprintf(aurum, "%s %u %08x\n", LINGUAE_CORPORIS[j],
            (insignatus integer)(numerus_illorum - I),
            (insignatus integer)friatio);
        discordiae_totae += discordiae;
        free(octeti);
        free(nostri);
        free(illorum);
    }
    fclose(aurum);
    printf("runae_oraculum: graphemata, discordiae %u\n",
        (insignatus integer)discordiae_totae);
    redde (discordiae_totae == ZEPHYRUM) ? ZEPHYRUM : I;
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
     constans character* graphemata    = NIHIL;
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
        alioquin si (   strcmp(argv[i], "-graphemata") == ZEPHYRUM
                     && i + I < argc)
        {
            graphemata = argv[++i];
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
    si (graphemata != NIHIL)
    {
        redde _graphemata(graphemata);
    }
    si (mappa_aliena != NIHIL)
    {
        redde _mappam_alienam_conferre(mappa_aliena);
    }
    redde _conferre();
}
