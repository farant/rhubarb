/* fon_ad_fons.c - fontem bitmap Windows (.FON, NE) ex "The Ultimate
 * Oldschool PC Font Pack" (int10h.org) in tabulam C vertere, forma
 * fons_6x8.h (byte = ordo, bit VII sinister), ordine CP437 nativo
 * (nulla remappatio: importatio fidelis, glyphae ut in fonte).
 *
 * Usus: ./bin/aedilis tools/fon_ad_fons.c && bash
 *   build/aedilis/fon_ad_fons/struere.sh &&
 *   ./build/aedilis/fon_ad_fons/fon_ad_fons <via.FON> <nomen_tabulae>
 *   <titulus> > include/<nomen_tabulae>.h
 *
 * Forma .FON: MZ (e_lfanew ad 0x3C) -> caput NE (tabula resourcium ad
 * NE+0x24) -> resourcia generis 0x8008 (RT_FONT) -> FNT v2 (0x200) aut
 * v3 (0x300): dfPixHeight ad 88, dfFirstChar 95, dfLastChar 96,
 * tabula characterum ad 118 (v2: latitudo u16, offset u16) aut 148
 * (v3: latitudo u16, offset u32). Glypha VIII latitudine summum: bytes
 * altitudinis continui. Fontes latiores (> VIII) recusantur.
 *
 * Licentia fontium (omnes in fasciculo): CC BY-SA 4.0, (c) 2016-2020
 * VileR - caput emissum attributionem continet.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "friatio.h"
#include <stdio.h>
#include <string.h>

#define GLYPHAE        CCLVI
#define ALTITUDO_MAX   XXXII

hic_manens i8 glyphae[GLYPHAE][ALTITUDO_MAX];

/* u16 / u32 parvi-finis ad offset; FALSUM si extra datum */
interior b32
u16_legere (
    chorda  d,
       i32  offset,
       i32* valor)
{
    si (offset + II > d.mensura)
    {
        redde FALSUM;
    }
    *valor = (i32)d.datum[offset] | ((i32)d.datum[offset + I] << VIII);
    redde VERUM;
}

interior b32
u32_legere (
    chorda  d,
       i32  offset,
       i32* valor)
{
    i32 a;
    i32 b;

    si (!u16_legere(d, offset, &a) || !u16_legere(d, offset + II, &b))
    {
        redde FALSUM;
    }
    *valor = a | (b << XVI);
    redde VERUM;
}

/* offset resourcii FNT primi (RT_FONT); 0 si nullum */
interior i32
fontem_invenire (
    chorda  d,
       i32* longitudo)
{
    i32 ne;
    i32 tabula;
    i32 gradus;
    i32 p;
    i32 genus;
    i32 numerus;
    i32 offset;
    i32 mensura;

    si (   d.mensura < LXIV || d.datum[ZEPHYRUM] != 'M'
        || d.datum[I]      != 'Z' || !u32_legere(d, 0x3C, &ne)
        || ne + LXIV > d.mensura || d.datum[ne] != 'N'
        || d.datum[ne + I] != 'E' || !u16_legere(d, ne + 0x24, &tabula))
    {
        redde ZEPHYRUM;
    }
    p = ne + tabula;
    si (!u16_legere(d, p, &gradus))
    {
        redde ZEPHYRUM;
    }
    p += II;
    dum (u16_legere(d, p, &genus) && genus != ZEPHYRUM)
    {
        si (!u16_legere(d, p + II, &numerus))
        {
            redde ZEPHYRUM;
        }
        p += VIII;
        si (genus == 0x8008 && numerus > ZEPHYRUM)
        {
            si (   !u16_legere(d, p, &offset)
                || !u16_legere(d, p + II, &mensura))
            {
                redde ZEPHYRUM;
            }
            *longitudo = mensura << gradus;
            redde offset << gradus;
        }
        p += numerus * XII;
    }
    redde ZEPHYRUM;
}

/* nomen plagulae sine directoriis */
interior constans character*
basis (
    constans character* via)
{
    constans character* b;

    b = strrchr(via, '/');
    redde b ? b + I : via;
}

s32
principale (
          s32   argc,
    character** argv)
{
            Piscina* p;
             chorda  d;
                i32  fons_initium;
                i32  longitudo;
                i32  versio;
                i32  altitudo;
                i32  prima;
                i32  ultima;
                i32  tabula;
                i32  lat;
                i32  off;
                i32  c;
                i32  r;
    SHA256Contextus  ctx;
          character  magnum[LXIV];
                 i8  digestum[SHA256_DIGEST_MENSURA];

    si (argc != IV)
    {
        fprintf(stderr, "usus: fon_ad_fons <via.FON> <nomen_tabulae> "
            "<titulus>\n");
        redde I;
    }
    p = piscina_generare_dynamicum("fon_ad_fons", CCLVI * M);
    d = filum_legere_totum(argv[I], p);
    fons_initium = fontem_invenire(d, &longitudo);
    si (   fons_initium == ZEPHYRUM
        || !u16_legere(d, fons_initium, &versio)
        || (versio != 0x200 && versio != 0x300)
        || !u16_legere(d, fons_initium + LXXXVIII, &altitudo)
        || altitudo     == ZEPHYRUM || altitudo > ALTITUDO_MAX
        || fons_initium + CXLVIII > d.mensura)
    {
        fprintf(stderr, "fon_ad_fons: %s non est fons FNT legibilis\n",
            argv[I]);
        redde I;
    }
    prima   = (i32)d.datum[fons_initium + XCV];
    ultima  = (i32)d.datum[fons_initium + XCVI];
    tabula  = fons_initium + (versio == 0x300 ? CXLVIII : CXVIII);
    memset(glyphae, ZEPHYRUM, magnitudo(glyphae));
    per (c = prima; c <= ultima; c++)
    {
        i32 locus;

        locus = tabula + (c - prima) * (versio == 0x300 ? VI : IV);
        si (   !u16_legere(d, locus, &lat)
            || !(versio == 0x300 ? u32_legere(d, locus + II, &off)
                                 : u16_legere(d, locus + II, &off))
            || lat > VIII || fons_initium + off + altitudo > d.mensura)
        {
            fprintf(stderr, "fon_ad_fons: glypha 0x%02X: latitudo %u "
                "(VIII summum) aut offset extra fontem\n", c, lat);
            redde I;
        }
        per (r = ZEPHYRUM; r < altitudo; r++)
        {
            glyphae[c][r] = d.datum[fons_initium + off + r];
        }
    }
    sha256_initiare(&ctx);
    sha256_addere(&ctx, d.datum, d.mensura);
    sha256_finire(&ctx, digestum);

    imprimere("/* Fons bitmap - %s (%ux%u, ordo CP437 nativus)\n",
        argv[III],
        (i32)VIII, altitudo);
    imprimere(" *\n * ORIGO ET LICENTIA (materia adaptata):\n");
    imprimere(" *   The Ultimate Oldschool PC Font Pack v2.2,\n");
    imprimere(" *     (c) 2016-2020 VileR, "
        "https://int10h.org/oldschool-pc-fonts/\n");
    imprimere(" *     CC BY-SA 4.0, "
        "http://creativecommons.org/licenses/by-sa/4.0/\n");
    imprimere(" *   Mutata: ex %s in tabulam C conversa\n",
        basis(argv[I]));
    imprimere(" *     (tools/fon_ad_fons.c); glyphae ipsae non "
        "mutatae.\n");
    imprimere(" *     sha256 fontis: ");
    per (c = ZEPHYRUM; c < SHA256_DIGEST_MENSURA; c++)
    {
        imprimere("%02x", digestum[c]);
        si (c == XV)
        {
            imprimere("\n *       ");
        }
    }
    imprimere("\n * Hic fasciculus ergo sub CC BY-SA 4.0 (licentia "
        "adaptatoris).\n */\n\n");
    /* nomen magnum (custodia, constantes) ex nomine tabulae */
    per (c = ZEPHYRUM; argv[II][c] && c < LXIII; c++)
    {
        magnum[c] = (argv[II][c] >= 'a' && argv[II][c] <= 'z')
                  ? (character)(argv[II][c] - 'a' + 'A') : argv[II][c];
    }
    magnum[c] = '\0';
    imprimere("#ifndef %s_H\n#define %s_H\n\n#include \"latina.h\"\n\n",
        magnum, magnum);
    imprimere("#define %s_LATITUDO  VIII\n", magnum);
    imprimere("#define %s_ALTITUDO  XVI\n\n", magnum);
    imprimere("/* byte = ordo glyphae; bit VII = pixelum "
        "sinistrum */\n");
    imprimere("interior constans i8 %s[CCLVI][XVI] = {\n", argv[II]);
    per (c = ZEPHYRUM; c < GLYPHAE; c++)
    {
        imprimere("    /* 0x%02X", c);
        si (c > 0x20 && c < 0x7F && c != '/' && c != '*' && c != '\\')
        {
            imprimere(" '%c'", (character)c);
        }
        imprimere(" */\n    {");
        per (r = ZEPHYRUM; r < XVI; r++)
        {
            imprimere("0x%02X%s", r
                < altitudo ? glyphae[c][r] : (i8)ZEPHYRUM,
                r == XV ? "" : (r == VII ? ",\n     " : ", "));
        }
        imprimere("}%s\n", c == GLYPHAE - I ? "" : ",");
    }
    imprimere("};\n\n#endif /* %s_H */\n", magnum);
    piscina_destruere(p);
    redde ZEPHYRUM;
}
