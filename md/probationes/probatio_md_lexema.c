/* probatio_md_lexema.c - Tabula linearum, scanners, fabrica, corpus
 *
 * PORTA CORPORIS A2: lexatio cruda super OMNES plagulas markdown quas
 * git tractat (md/build/corpus_md.txt, a cursore scriptum) octetim
 * exacta. Lista absens CLAMAT (CREDO_CULPA) - numquam tacite
 * praeteritur; tegumentum SUUM metitur (plagulae > M, octeti > MM).
 *
 * Scanners contra exempla CommonMark 0.31.2 (par. 4.2 ATX, 4.3
 * setext, 4.1 fractura thematica, 4.5 saepta, 5.2 marcae listarum,
 * 5.1 citationes) et GFM (5.3 officia, 4.10 tabulae).
 */

#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
#include "credo.h"
#include "md_lexema.h"
#include "md_lexicon.h"
#include "md_registrum.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = lectiones_fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* Valores lexematum concatenati == fons, et byte_offset currens. */
interior b32
_circuitus (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   i32* lexemata)
{
    Xar* xs      = md_lexare_crude(piscina, fons, mensura);
    s32  cursor  = ZEPHYRUM;
    i32  i;
    i32  n;

    si (xs == NIHIL)
    {
        imprimere("    (lexatio NIHIL)\n");
        redde FALSUM;
    }
    n = xar_numerus(xs);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaToken* t = *(MateriaToken**)xar_obtinere(xs, i);

        si (t->byte_offset != cursor)
        {
            imprimere("    (lexema %d: offset %d, cursor %d)\n",
                (integer)i, (integer)t->byte_offset, (integer)cursor);
            redde FALSUM;
        }
        si (   t->valor.mensura > ZEPHYRUM
            && memcmp(t->valor.datum, fons + cursor,
                   (size_t)t->valor.mensura) != ZEPHYRUM)
        {
            imprimere("    (lexema %d: octeti dispari)\n", (integer)i);
            redde FALSUM;
        }
        cursor = cursor + (s32)t->valor.mensura;
    }
    si (cursor != (s32)mensura)
    {
        imprimere("    (summa %d contra fontem %d)\n", (integer)cursor,
            (integer)mensura);
        redde FALSUM;
    }
    *lexemata = n;
    redde VERUM;
}

/* Lexema i ex lexatione cruda chordae */
interior constans MateriaToken*
_lexema (
    Xar* xs,
    i32  i)
{
    redde *(MateriaToken**)xar_obtinere(xs, i);
}


/* ==================================================
 * PROBARE: tabula linearum
 * ================================================== */

interior vacuum
_tabulam_linearum_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    {
        MdLineae tab;

        CREDO_VERUM (md_lineae_scindere(piscina, "", ZEPHYRUM, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), ZEPHYRUM);

        CREDO_VERUM (md_lineae_scindere(piscina, "a", I, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), I);
        CREDO_AEQUALIS_I32 (md_linea(&tab, ZEPHYRUM)->mensura, I);
        CREDO_AEQUALIS_I32 (md_linea(&tab, ZEPHYRUM)->terminator,
            ZEPHYRUM);

        /* terminator ultimus lineam vacuam ulteriorem NON parit */
        CREDO_VERUM (md_lineae_scindere(piscina, "a\n", II, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), I);
        CREDO_AEQUALIS_I32 (md_linea(&tab, ZEPHYRUM)->terminator, I);

        CREDO_VERUM (md_lineae_scindere(piscina, "a\r\nb", IV, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), II);
        CREDO_AEQUALIS_I32 (md_linea(&tab, ZEPHYRUM)->terminator, II);
        CREDO_AEQUALIS_S32 (md_linea(&tab, I)->offset, (s32)III);
        CREDO_AEQUALIS_I32 (md_linea(&tab, I)->terminator, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (md_linea(&tab, I)->numerus, II);

        CREDO_VERUM (md_lineae_scindere(piscina, "a\rb\n", IV, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), II);
        CREDO_AEQUALIS_I32 (md_linea(&tab, ZEPHYRUM)->terminator, I);

        CREDO_VERUM (md_lineae_scindere(piscina, "\n\n", II, &tab));
        CREDO_AEQUALIS_I32 (md_lineae_numerus(&tab), II);
        CREDO_AEQUALIS_I32 (md_linea(&tab, I)->mensura, ZEPHYRUM);
        CREDO_NIHIL (md_linea(&tab, II));
    }
}


/* ==================================================
 * PROBARE: lexatio cruda - positiones et circuitus
 * ================================================== */

interior vacuum
_lexare_crude_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    {
        constans character* f = "ab\n  cd";
                       Xar* xs;
                       i32  n;

        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &n));
        CREDO_AEQUALIS_I32 (n, V);
        xs = md_lexare_crude(piscina, f, (i32)strlen(f));
        CREDO_NON_NIHIL (xs);
        CREDO_AEQUALIS_S32 (_lexema(xs, ZEPHYRUM)->genus,
            (s32)MD_LEX_TEXTUS);
        CREDO_AEQUALIS_I32 (_lexema(xs, ZEPHYRUM)->linea, I);
        CREDO_AEQUALIS_I32 (_lexema(xs, ZEPHYRUM)->columna, I);
        CREDO_AEQUALIS_S32 (_lexema(xs, I)->genus, (s32)MD_LEX_LINEA);
        CREDO_AEQUALIS_I32 (_lexema(xs, I)->columna, III);
        CREDO_AEQUALIS_S32 (_lexema(xs, II)->genus,
            (s32)MD_LEX_INDENTATIO);
        CREDO_AEQUALIS_I32 (_lexema(xs, II)->linea, II);
        CREDO_AEQUALIS_I32 (_lexema(xs, II)->columna, I);
        CREDO_AEQUALIS_S32 (_lexema(xs, III)->genus,
            (s32)MD_LEX_TEXTUS);
        CREDO_AEQUALIS_I32 (_lexema(xs, III)->columna, III);
        CREDO_AEQUALIS_S32 (_lexema(xs, IV)->genus, (s32)MD_LEX_FINIS);
        CREDO_AEQUALIS_I32 (_lexema(xs, IV)->linea, II);
        CREDO_AEQUALIS_I32 (_lexema(xs, IV)->columna, V);
        CREDO_AEQUALIS_S32 (_lexema(xs, IV)->fons_index,
            MD_FONS_PLAGULAE);

        /* terminator ultimus: FINIS in linea nova, columna I */
        f = "x\r\n";
        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &n));
        xs = md_lexare_crude(piscina, f, (i32)strlen(f));
        CREDO_AEQUALIS_I32 (n, III);
        CREDO_AEQUALIS_S32 (_lexema(xs, I)->genus, (s32)MD_LEX_LINEA);
        CREDO_AEQUALIS_I32 (_lexema(xs, I)->valor.mensura, II);
        CREDO_AEQUALIS_I32 (_lexema(xs, II)->linea, II);
        CREDO_AEQUALIS_I32 (_lexema(xs, II)->columna, I);

        /* '\r' solum = LINEA_CR */
        f = "x\ry";
        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &n));
        xs = md_lexare_crude(piscina, f, (i32)strlen(f));
        CREDO_AEQUALIS_S32 (_lexema(xs, I)->genus,
            (s32)MD_LEX_LINEA_CR);

        /* plagula vacua = FINIS solus */
        CREDO_VERUM (_circuitus(piscina, "", ZEPHYRUM, &n));
        CREDO_AEQUALIS_I32 (n, I);
    }
}


/* ==================================================
 * PROBARE: scanners - exempla CommonMark
 * ================================================== */

interior vacuum
_explorationes_probare (
    CredoContextus* c)
{
    (vacuum)c;
    {
                 MdAtx a;
                   i32 g;
                   s32 p;
                   i32 columna;
             MdSaeptum s;
         MdMarcaListae m;
        MdMarcaOfficii o;
                   i32 ord[VIII];


#define AB(lit) (lit), ZEPHYRUM, (s32)strlen(lit)

        /* ATX (par. 4.2) */
        CREDO_VERUM (md_scan_atx(AB("# foo"), &a));
        CREDO_AEQUALIS_I32 (a.gradus, I);
        CREDO_AEQUALIS_S32 (a.marca_ad, (s32)II);
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)V);
        CREDO_AEQUALIS_S32 (a.clausura_ab, (s32)V);
        CREDO_VERUM (md_scan_atx(AB("###### foo"), &a));
        CREDO_AEQUALIS_I32 (a.gradus, VI);
        CREDO_FALSUM (md_scan_atx(AB("####### foo"), &a));
        CREDO_FALSUM (md_scan_atx(AB("#5 bolt"), &a));
        CREDO_FALSUM (md_scan_atx(AB("#hashtag"), &a));
        CREDO_VERUM (md_scan_atx(AB("#"), &a));
        CREDO_AEQUALIS_S32 (a.marca_ad, (s32)I);
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)I);
        CREDO_VERUM (md_scan_atx(AB("#\tfoo"), &a));
        CREDO_VERUM (md_scan_atx(AB("# foo ##"), &a));
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)V);
        CREDO_AEQUALIS_S32 (a.clausura_ab, (s32)V);
        CREDO_VERUM (md_scan_atx(AB("### foo ###     "), &a));
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)VII);
        CREDO_AEQUALIS_S32 (a.clausura_ab, (s32)VII);
        CREDO_VERUM (md_scan_atx(AB("# foo#"), &a));
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)VI);   /* nulla clausura */
        CREDO_VERUM (md_scan_atx(AB("### ###"), &a));
        CREDO_AEQUALIS_S32 (a.marca_ad, (s32)IV);
        CREDO_AEQUALIS_S32 (a.contentum_ad, (s32)IV);   /* contentum vacuum */
        CREDO_AEQUALIS_S32 (a.clausura_ab, (s32)IV);
        CREDO_VERUM (md_scan_atx(AB("# foo \\###"), &a));
        CREDO_AEQUALIS_S32 (a.clausura_ab, (s32)X);     /* '\#' non clausura */

        /* setext (par. 4.3) */
        CREDO_VERUM (md_scan_subductio(AB("==="), &g));
        CREDO_AEQUALIS_I32 (g, I);
        CREDO_VERUM (md_scan_subductio(AB("---"), &g));
        CREDO_AEQUALIS_I32 (g, II);
        CREDO_VERUM (md_scan_subductio(AB("=  "), &g));
        CREDO_FALSUM (md_scan_subductio(AB("= ="), &g));
        CREDO_FALSUM (md_scan_subductio(AB("-- -"), &g));
        CREDO_FALSUM (md_scan_subductio(AB(""), &g));

        /* fractura thematica (par. 4.1) */
        CREDO_VERUM (md_scan_divisio(AB("***")));
        CREDO_VERUM (md_scan_divisio(AB("---")));
        CREDO_VERUM (md_scan_divisio(AB("___")));
        CREDO_FALSUM (md_scan_divisio(AB("+++")));
        CREDO_FALSUM (md_scan_divisio(AB("===")));
        CREDO_FALSUM (md_scan_divisio(AB("--")));
        CREDO_VERUM (md_scan_divisio(AB("- - -")));
        CREDO_VERUM (md_scan_divisio(AB("_ _ _ _ _")));
        CREDO_VERUM (md_scan_divisio(AB("-  -   -    -  ")));
        CREDO_FALSUM (md_scan_divisio(AB("*-*")));
        CREDO_FALSUM (md_scan_divisio(AB("***a")));

        /* saepta (par. 4.5) */
        CREDO_VERUM (md_scan_saeptum_apertum(AB("```"), &s));
        CREDO_AEQUALIS_I32 (s.longitudo, III);
        CREDO_FALSUM (s.info_adest);
        CREDO_VERUM (md_scan_saeptum_apertum(AB("````c"), &s));
        CREDO_AEQUALIS_I32 (s.longitudo, IV);
        CREDO_VERUM (s.info_adest);
        CREDO_AEQUALIS_S32 (s.info_ab, (s32)IV);
        CREDO_FALSUM (md_scan_saeptum_apertum(AB("```a`b"), &s));
        CREDO_VERUM (md_scan_saeptum_apertum(AB("~~~ a`b"), &s));
        CREDO_FALSUM (md_scan_saeptum_apertum(AB("``"), &s));
        CREDO_VERUM (md_scan_saeptum_clausum(AB("````"), '`', III, &p));
        CREDO_AEQUALIS_S32 (p, (s32)IV);
        CREDO_FALSUM (md_scan_saeptum_clausum(AB("```"), '`', IV, &p));
        CREDO_FALSUM (md_scan_saeptum_clausum(AB("``` x"), '`', III,
            &p));
        CREDO_VERUM (md_scan_saeptum_clausum(AB("~~~  "), '~', III,
            &p));

        /* marcae listarum (par. 5.2) */
        CREDO_VERUM (md_scan_marca_listae(AB("- foo"), ZEPHYRUM, &m));
        CREDO_FALSUM (m.numerata);
        CREDO_VERUM (m.delimitator == '-');
        CREDO_AEQUALIS_S32 (m.marca_ad, (s32)I);
        CREDO_AEQUALIS_S32 (m.ad, (s32)II);
        CREDO_AEQUALIS_I32 (m.spatia, I);
        CREDO_VERUM (md_scan_marca_listae(AB("1. foo"), ZEPHYRUM, &m));
        CREDO_VERUM (m.numerata);
        CREDO_AEQUALIS_I32 (m.initium, I);
        CREDO_VERUM (m.delimitator == '.');
        CREDO_AEQUALIS_I32 (m.latitudo, II);
        CREDO_VERUM (md_scan_marca_listae(AB("123456789) x"), ZEPHYRUM,
            &m));
        CREDO_AEQUALIS_I32 (m.initium, (i32)123456789);
        CREDO_FALSUM (md_scan_marca_listae(AB("1234567890. x"),
            ZEPHYRUM, &m));
        CREDO_FALSUM (md_scan_marca_listae(AB("-foo"), ZEPHYRUM, &m));
        CREDO_FALSUM (md_scan_marca_listae(AB("1.foo"), ZEPHYRUM, &m));
        CREDO_VERUM (md_scan_marca_listae(AB("-"), ZEPHYRUM, &m));
        CREDO_AEQUALIS_I32 (m.spatia, ZEPHYRUM);
        CREDO_VERUM (md_scan_marca_listae(AB("*"), ZEPHYRUM, &m));
        CREDO_VERUM (md_scan_marca_listae(AB("-     foo"), ZEPHYRUM,
            &m));
        CREDO_AEQUALIS_I32 (m.spatia, V);
        CREDO_AEQUALIS_S32 (m.ad, (s32)VI);
        CREDO_VERUM (md_scan_marca_listae(AB("-\tfoo"), ZEPHYRUM, &m));
        CREDO_AEQUALIS_I32 (m.spatia, III);   /* tabula ad columnam IV */
        CREDO_FALSUM (md_scan_marca_listae(AB(""), ZEPHYRUM, &m));

        /* citationes (par. 5.1) */
        CREDO_AEQUALIS_S32 (md_scan_citatio(AB(">")), (s32)I);
        CREDO_AEQUALIS_S32 (md_scan_citatio(AB("> x")), (s32)II);
        CREDO_AEQUALIS_S32 (md_scan_citatio(AB(">>")), (s32)I);
        CREDO_AEQUALIS_S32 (md_scan_citatio(AB("x")), ZEPHYRUM);

        /* officia (GFM par. 5.3) */
        CREDO_VERUM (md_scan_officium(AB("[ ] x"), &o));
        CREDO_FALSUM (o.perfectum);
        CREDO_AEQUALIS_S32 (o.ad, (s32)III);
        CREDO_VERUM (md_scan_officium(AB("[x] y"), &o));
        CREDO_VERUM (o.perfectum);
        CREDO_VERUM (md_scan_officium(AB("[X]"), &o));
        CREDO_FALSUM (md_scan_officium(AB("[y] z"), &o));
        CREDO_FALSUM (md_scan_officium(AB("[ ]x"), &o));

        /* indentatio, columnae */
        p = md_scan_indentatio(AB("  \tx"), ZEPHYRUM, &columna);
        CREDO_AEQUALIS_S32 (p, (s32)III);
        CREDO_AEQUALIS_I32 (columna, IV);
        p = md_scan_indentatio(AB("\t x"), II, &columna);
        CREDO_AEQUALIS_I32 (columna, III);   /* a columna II: tabula ad IV, + I */

        /* separator tabulae (GFM par. 4.10) */
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("| --- | :-: |"),
            ord, VIII), II);
        CREDO_AEQUALIS_I32 (ord[0], (i32)MD_ORDINATIO_NULLA);
        CREDO_AEQUALIS_I32 (ord[1], (i32)MD_ORDINATIO_MEDIA);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB(":--|--:"), ord,
            VIII), II);
        CREDO_AEQUALIS_I32 (ord[0], (i32)MD_ORDINATIO_SINISTRA);
        CREDO_AEQUALIS_I32 (ord[1], (i32)MD_ORDINATIO_DEXTRA);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("---"), ord,
            VIII), I);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("| a |"), ord,
            VIII),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("|"), ord,
            VIII),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("| --- |  "),
            ord,
            VIII), I);
        CREDO_AEQUALIS_I32 (md_scan_tabula_separator(AB("|---|---|---|"),
            ord, II), III);   /* capacitas II: numerus verus redditur */
#undef AB
    }
}


/* ==================================================
 * PROBARE: corpus - omnes plagulae md a git tractatae
 * ================================================== */

interior vacuum
_corpus_probare (
    CredoContextus* c)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    (vacuum)c;
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }

    {
        character via[DXII];
             FILE* lista;
              i32  plagulae   = ZEPHYRUM;
              i32  absentes   = ZEPHYRUM;
              s64  octeti     = ZEPHYRUM;
              s64  lexemata   = ZEPHYRUM;
              i32  fractae    = ZEPHYRUM;

        sprintf(via, "%s/md/build/corpus_md.txt", radix_viae);
        lista = lectiones_fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("md/build/corpus_md.txt absens - cursor eam scribit"
                " (git ls-files '*.md'); e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                     Piscina* p;
                   character* textus;
                         i32  mensura  = ZEPHYRUM;
                         i32  n        = ZEPHYRUM;
                      size_t  l        = strlen(linea);

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[l - I]  = '\0';
                    l             = l - I;
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                sprintf(via, "%s/%s", radix_viae, linea);
                p = piscina_generare_dynamicum("corpus_md", 262144);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    absentes = absentes + I;
                    imprimere("  ABEST: %s\n", linea);
                    piscina_destruere(p);
                    perge;
                }
                plagulae  = plagulae + I;
                octeti    = octeti + (s64)mensura;
                si (!_circuitus(p, textus, mensura, &n))
                {
                    fractae = fractae + I;
                    imprimere("  FRACTA: %s\n", linea);
                }
                lexemata = lexemata + (s64)n;
                piscina_destruere(p);
            }
            fclose(lista);

            imprimere("  plagulae %d, octeti %ld, lexemata %ld, absentes %d\n",
                (integer)plagulae, (longus)octeti, (longus)lexemata,
                (integer)absentes);
            CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (absentes, ZEPHYRUM);
            /* tegumentum SUUM: 'N/N mundae' potest significare 'numquam
             * apparuit' - plagulae et octeti asseruntur */
            CREDO_MAIOR_I32 (plagulae, (i32)1000);
            CREDO_MAIOR_S32 ((s32)(octeti / (s64)1000), (s32)1000);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tabulam linearum", _tabulam_linearum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "lexationem crudam", _lexare_crude_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "scanners", _explorationes_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "PORTA CORPORIS: plagulae md a git tractatae", _corpus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("md_lexema", SECTIONES);
}
