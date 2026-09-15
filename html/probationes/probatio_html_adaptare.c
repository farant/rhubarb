/* probatio_html_adaptare.c - Adaptatio et ligatio triviorum HTML
 *
 * PORTA PRINCIPALIS HIC EST TEGUMENTUM OCTETORUM: omnis octetus
 * fontis EXACTE SEMEL apparet, sive in lexemate significante sive in
 * triviis eius - PER OCTETUM, offset post offset. Adaptatio et
 * ligatio sunt ubi octetus perdi potest; parsator (H3) eas solum
 * ADHIBET. Vitium hic inventum vitium unius moduli est.
 *
 * REGIMEN UNUM: HTML trivia intra tag sola habet (SPATIA, DELIM);
 * extra tags omnia contentum sunt. Nullum 'regimen contenti' (css D7)
 * exsistit - lexator id iam decrevit (H6).
 */

#include "latina.h"
#include "credo.h"
#include "html_adaptare.h"
#include "html_lexicon.h"
#include "html_lexema.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };


/* Lexare, adaptare, ligare. Seriem SIGNIFICANTIUM reddit (NIHIL in
 * fractura). */
hic_manens Xar*
_parare (
                         Piscina* piscina,
    constans MateriaLexiconRatum* lexicon,
              constans character* fons,
                             i32  mensura)
{
            Xar* lexemata;
            Xar* plana;
            Xar* significantia;
    HtmlLigator  ligator;
            i32  i;
            i32  numerus;

    lexemata = html_lexare(piscina, fons, mensura);
    si (lexemata == NIHIL)
    { redde NIHIL;
    }

    plana = html_adaptare(piscina, lexemata, &FORMA);
    si (plana == NIHIL)
    { redde NIHIL;
    }

    html_ligator_incipere(&ligator, piscina, lexicon);
    significantia = xar_creare(piscina, magnitudo(MateriaToken*));
    si (significantia == NIHIL)
    { redde NIHIL;
    }

    numerus = xar_numerus(plana);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MateriaToken* token;

        token = *(MateriaToken**)xar_obtinere(plana, i);
        si (materia_lexicon_trivium_est(lexicon, token->genus))
        {
            si (!html_ligator_cumulare(&ligator, token))
            { redde NIHIL;
            }
        }
        alioquin
        {
            MateriaToken** locus;
            si (!html_ligator_solvere(&ligator, token))
            { redde NIHIL;
            }
            locus = (MateriaToken**)xar_addere(significantia);
            si (locus == NIHIL)
            { redde NIHIL;
            }
            *locus = token;
        }
    }

    redde significantia;
}

/* TEGUMENTUM: ante, valor, post - ordine fontis, offset post offset,
 * ab octeto ZEPHYRUM ad mensuram. */
hic_manens b32
_tegumentum_probare (
                         Piscina* piscina,
    constans MateriaLexiconRatum* lexicon,
              constans character* fons,
                             i32  mensura)
{
    Xar* significantia;
    s32  exspectatus = ZEPHYRUM;
    i32  i;
    i32  numerus;

    significantia = _parare(piscina, lexicon, fons, mensura);
    si (significantia == NIHIL)
    { redde FALSUM;
    }

    numerus = xar_numerus(significantia);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MateriaToken* token;
        i32 j;

        token = *(MateriaToken**)xar_obtinere(significantia, i);

        per (j = ZEPHYRUM; j < token->numerus_ante; j++)
        {
            si (token->spatia_ante[j]->byte_offset != exspectatus)
            { redde FALSUM;
            }
            exspectatus += (s32)token->spatia_ante[j]->valor.mensura;
        }
        si (token->byte_offset != exspectatus)
        { redde FALSUM;
        }
        exspectatus += (s32)token->valor.mensura;
        per (j = ZEPHYRUM; j < token->numerus_post; j++)
        {
            si (token->spatia_post[j]->byte_offset != exspectatus)
            { redde FALSUM;
            }
            exspectatus += (s32)token->spatia_post[j]->valor.mensura;
        }
    }

    redde (b32)(exspectatus == (s32)mensura);
}

/* Plagulam ex disco legere; NIHIL si abest. */
hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
    FILE* f;
    longus longitudo;
    character* memoria;
    size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    { redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(f); redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    { fclose(f); redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    { redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

integer
principale (vacuum)
{
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
                    b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_html_adaptare",
        65536);
    credo_aperire(piscina);

    si (!materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM: %s\n",
            materia_lexicon_vitium_nomen(
                (MateriaLexVitium)iudicium.vitium));
        redde I;
    }


    /* ==================================================
     * PROBARE: adaptatio UNUM PRO UNO
     * ================================================== */

    {
        constans character* fons     = "<div class=\"x\" id=y>t</div>";
                       i32  mensura  = (i32)strlen(fons);
                       Xar* lexemata;
                       Xar* plana;
                       i32  i;

        imprimere("\n--- Probans adaptationem unum-pro-uno ---\n");

        lexemata = html_lexare(piscina, fons, mensura);
        CREDO_NON_NIHIL (lexemata);
        plana = html_adaptare(piscina, lexemata, &FORMA);
        CREDO_NON_NIHIL (plana);

        /* INVARIATUM CENTRALE: nihil absorbetur, nihil omittitur. */
        CREDO_AEQUALIS_I32 (xar_numerus(plana), xar_numerus(lexemata));
        CREDO_MAIOR_I32 (xar_numerus(plana), (i32)X);

        per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
        {
            constans HtmlLexema* lexema;
            MateriaToken* token;

            lexema  = (constans HtmlLexema*)xar_obtinere(lexemata, i);
            token   = *(MateriaToken**)xar_obtinere(plana, i);

            CREDO_AEQUALIS_S32 (token->genus, (s32)lexema->genus);
            CREDO_AEQUALIS_S32 (token->byte_offset,
                (s32)lexema->offset);
            CREDO_AEQUALIS_I32 (token->linea, lexema->linea);
            CREDO_AEQUALIS_I32 (token->columna, lexema->columna);
            CREDO_AEQUALIS_I32 (token->valor.mensura,
                lexema->valor.mensura);
            /* Valor in FONTEM spectat - idem punctator, non copia */
            CREDO_AEQUALIS_PTR (token->valor.datum,
                lexema->valor.datum);
            CREDO_AEQUALIS_S32 (token->fons_index, ZEPHYRUM);
            /* initium_lineae NUMQUAM ponitur - assertum POSITIVE */
            CREDO_FALSUM (materia_token_initium_lineae(token));
        }
    }


    /* ==================================================
     * PROBARE: regula divisionis intra tag, omnes rami
     * ================================================== */

    {
        Xar* significantia;
        MateriaToken* t;

        imprimere("\n--- Probans divisionem: sine linea nova ---\n");
        {
            /* '<a href=x >': spatium post '<a' priori ut 'post';
             * spatium ante '>' valori 'x' ut 'post'. Significantia:
             * <a href = x > FINIS = VI. */
            constans character* fons = "<a href=x >";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (significantia);
            CREDO_AEQUALIS_I32 (xar_numerus(significantia), VI);

            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_TAG_APERTURA);
            CREDO_AEQUALIS_I32 (t->numerus_post, I);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

            t = *(MateriaToken**)xar_obtinere(significantia, I);
            CREDO_AEQUALIS_S32 (t->genus,
                (s32)HTML_LEX_ATTRIBUTUM_NOMEN);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

            t = *(MateriaToken**)xar_obtinere(significantia, III);
            CREDO_AEQUALIS_S32 (t->genus,
                (s32)HTML_LEX_ATTRIBUTUM_VALOR);
            CREDO_AEQUALIS_I32 (t->numerus_post, I);

            t = *(MateriaToken**)xar_obtinere(significantia, IV);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_TAG_FINIS);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
        }

        imprimere("\n--- Probans divisionem cum DELIM ---\n");
        {
            /* '<a /\n href=x>': pendentia TRIA - SPATIA(' '),
             * DELIM('/'), SPATIA('\n '). Divisor = primum SPATIUM
             * (munere) lineam novam ferens = index II; DELIM munus
             * SPATIUM fert sed lineam non - ergo spatium et DELIM
             * priori, SPATIA sequenti. */
            constans character* fons = "<a /\n href=x>";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (significantia);

            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_TAG_APERTURA);
            CREDO_AEQUALIS_I32 (t->numerus_post, II);
            CREDO_AEQUALIS_S32 (t->spatia_post[ZEPHYRUM]->genus,
                (s32)HTML_LEX_SPATIA);
            CREDO_AEQUALIS_S32 (t->spatia_post[I]->genus,
                (s32)HTML_LEX_DELIM);

            t = *(MateriaToken**)xar_obtinere(significantia, I);
            CREDO_AEQUALIS_S32 (t->genus,
                (s32)HTML_LEX_ATTRIBUTUM_NOMEN);
            CREDO_AEQUALIS_I32 (t->numerus_ante, I);
            CREDO_AEQUALIS_S32 (t->spatia_ante[ZEPHYRUM]->genus,
                (s32)HTML_LEX_SPATIA);
        }

        imprimere("\n--- Probans divisionem: cauda ad EOF ---\n");
        {
            /* '<div ' + EOF: spatium sine linea nova PRIORI; FINIS
             * nihil accipit. '<div\n' + EOF: linea nova -> FINIS ut
             * 'ante' - via una qua trivia caudam attingunt. */
            constans character* fons_a = "<div ";
            constans character* fons_b = "<div\n";

            significantia = _parare(piscina, &ratum, fons_a,
                (i32)strlen(fons_a));
            CREDO_NON_NIHIL (significantia);
            CREDO_AEQUALIS_I32 (xar_numerus(significantia), II);
            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (t->numerus_post, I);
            t = *(MateriaToken**)xar_obtinere(significantia, I);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_FINIS);
            CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

            significantia = _parare(piscina, &ratum, fons_b,
                (i32)strlen(fons_b));
            CREDO_NON_NIHIL (significantia);
            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (t->numerus_post, ZEPHYRUM);
            t = *(MateriaToken**)xar_obtinere(significantia, I);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_FINIS);
            CREDO_AEQUALIS_I32 (t->numerus_ante, I);
        }

        imprimere("\n--- Probans divisionem sine priore ---\n");
        {
            /* Per constructionem inattingibilis (omne trivium tagi
             * apertionem sequitur), ergo ligator DIRECTE exercetur:
             * prior NIHIL -> omnia sequenti ut 'ante'. */
            HtmlLigator ligator;
            MateriaToken* trivium;
            MateriaToken* sequens;

            html_ligator_incipere(&ligator, piscina, &ratum);
            trivium = materia_token_creare(piscina, &FORMA,
                (s32)HTML_LEX_SPATIA, chorda_ex_literis(" ", piscina),
                ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
            sequens = materia_token_creare(piscina, &FORMA,
                (s32)HTML_LEX_TAG_FINIS, chorda_ex_literis(">",
                piscina),
                (s32)I, (i32)I, (i32)II, ZEPHYRUM);
            CREDO_VERUM (html_ligator_cumulare(&ligator, trivium));
            CREDO_VERUM (html_ligator_solvere(&ligator, sequens));
            CREDO_AEQUALIS_I32 (sequens->numerus_ante, I);
            CREDO_AEQUALIS_PTR (sequens->spatia_ante[ZEPHYRUM],
                trivium);
        }

        imprimere("\n--- Probans trivia trans SEGMENTA xar ---\n");
        {
            /* Xar SEGMENTATUS est, primo segmento XVI elementa. XII
             * paria ' /' = XXIV trivia + spatium ante href = XXV,
             * nulla linea nova: OMNIA priori. */
            constans character* fons =
                "<a / / / / / / / / / / / / href=x>";
            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (significantia);

            t = *(MateriaToken**)xar_obtinere(significantia, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (t->genus, (s32)HTML_LEX_TAG_APERTURA);
            CREDO_AEQUALIS_I32 (t->numerus_post, XXV);
            CREDO_MAIOR_I32 (t->numerus_post, (i32)XVI);
            CREDO_AEQUALIS_S32 (t->spatia_post[XXIV]->genus,
                (s32)HTML_LEX_SPATIA);
            CREDO_AEQUALIS_S32 (t->spatia_post[XXIII]->genus,
                (s32)HTML_LEX_DELIM);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, fons,
                (i32)strlen(fons)));
        }

        imprimere("\n--- Probans contractum: Xar NON CONTIGUUS ---\n");
        {
            /* Lectio css: casus supra vitium plantatum non cepit quia
             * piscina segmentum secundum IMMEDIATE post primum ponit
             * - fortuna, non contractus. Hic inter cumulationes ALIUD
             * allocamus ut segmenta disiungantur; tunc series plana
             * locum tenet et vitium plantatum ardet. */
            constans character* fons =
                "<a / / / / / / / / / / / / href=x>";
                     Xar* plana;
             HtmlLigator  ligator;
            MateriaToken* ultimum_trivium  = NIHIL;
            MateriaToken* significans      = NIHIL;
                     i32  i;

            plana = html_adaptare(piscina,
                html_lexare(piscina, fons, (i32)strlen(fons)), &FORMA);
            CREDO_NON_NIHIL (plana);

            html_ligator_incipere(&ligator, piscina, &ratum);
            per (i = I; i < xar_numerus(plana); i++)
            {
                MateriaToken* token;
                token = *(MateriaToken**)xar_obtinere(plana, i);
                si (materia_lexicon_trivium_est(&ratum, token->genus))
                {
                    CREDO_VERUM (html_ligator_cumulare(&ligator,
                        token));
                    ultimum_trivium = token;
                    /* DISIUNCTOR: allocatio aliena inter elementa */
                    (vacuum)piscina_allocare(piscina,
                        (memoriae_index)LXIV);
                }
                alioquin
                { significans = token; frange;
                }
            }
            CREDO_NON_NIHIL (significans);
            CREDO_NON_NIHIL (ultimum_trivium);

            CREDO_VERUM (html_ligator_solvere(&ligator, significans));
            CREDO_AEQUALIS_I32 (significans->numerus_ante, XXV);
            CREDO_AEQUALIS_PTR (
                significans->spatia_ante[significans->numerus_ante - I],
                ultimum_trivium);
        }

        imprimere("\n--- Probans: extra tags NULLA trivia ---\n");
        {
            /* '<p>\n  <b>x</b>\n</p>': omne spatium inter tags TEXTUS
             * est (significans), ergo nullum lexema trivia fert. */
            constans character* fons = "<p>\n  <b>x</b>\n</p>";
                           i32  i;

            significantia = _parare(piscina, &ratum, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (significantia);
            per (i = ZEPHYRUM; i < xar_numerus(significantia); i++)
            {
                t = *(MateriaToken**)xar_obtinere(significantia, i);
                CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
                CREDO_AEQUALIS_I32 (t->numerus_post, ZEPHYRUM);
            }
            /* <p > \n  <b > x </b > \n </p > FINIS = XII
             * significantia (apertura et '>' lexemata duo sunt) */
            CREDO_AEQUALIS_I32 (xar_numerus(significantia), XII);
        }
    }


    /* ==================================================
     * PORTA: TEGUMENTUM OCTETORUM, casus inlinei
     * ================================================== */

    {
        hic_manens constans character* CASUS[] = {
            "",
            " ",
            "\n\n  ",
            "<p>a</p>",
            "<a  href = x\n  >b</A >",
            "<div cl",
            "<div\n",
            "x &amp; y <!-- c --> <![CDATA[z]]>",
            "<script>a</b></script>",
            "<textarea><p></textarea>",
            "<br/>",
            "<a href=\"x",
            "<!DOCTYPE html>",
            "<!doctype",
            "</",
            "<",
            "&",
            "<p a='x' b=c />",
            "<a / / href=x>",
            "a\r\nb",
            "<a\r\nhref=x>",
            "<?xml version=\"1.0\"?>",
            "<!-- interminatum",
            "</3>",
            "<title>non <clausum</title>"
        };
        i32 numerus_casuum = (i32)(magnitudo(CASUS)
            / magnitudo(CASUS[0]));
        i32 i;
        i32 probata = ZEPHYRUM;

        imprimere("\n--- PORTA: tegumentum octetorum inlineum ---\n");

        per (i = ZEPHYRUM; i < numerus_casuum; i++)
        {
            i32 mensura = (i32)strlen(CASUS[i]);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, CASUS[i],
                mensura));
            probata = probata + I;
        }

        /* NUL inline - in probatione vivit, non in fixura */
        {
            hic_manens constans character NUL_CASUS[] =
                "a\0<b c=\0>";
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, NUL_CASUS,
                (i32)IX));
            probata = probata + I;
        }

        /* Custodia vacuitatis: ansa currisse DEBET. */
        CREDO_MAIOR_S32 ((s32)probata, (s32)XX);
    }


    /* ==================================================
     * PORTA: TEGUMENTUM super CORPUS VERUM (fixa html)
     * ================================================== */

    {
        hic_manens constans character* PLAGULAE[] = {
            "probationes/fixa/html/adversarius.html",
            "probationes/fixa/html/laboratorium_index_2026-08-18.html",
            "probationes/fixa/html/villa_index_2026-08-18.html"
        };
        i32 numerus_plagularum =
            (i32)(magnitudo(PLAGULAE) / magnitudo(PLAGULAE[0]));
        constans character* radix;
                       i32  i;
                       i32  lectae = ZEPHYRUM;
                       i32  octeti = ZEPHYRUM;

        imprimere("\n--- PORTA: tegumentum super corpus verum ---\n");

        radix = getenv("RHUBARB_RADIX");
        si (radix == NIHIL)
        { radix = ".";
        }

        per (i = ZEPHYRUM; i < numerus_plagularum; i++)
        {
            character  via[DXII];
            character* textus;
                  i32  mensura = ZEPHYRUM;

            sprintf(via, "%s/%s", radix, PLAGULAE[i]);
            textus = _plagulam_legere(piscina, via, &mensura);
            si (textus == NIHIL)
            {
                imprimere("  OMISSA (abest): %s\n", PLAGULAE[i]);
                perge;
            }
            imprimere("  %s (%d octeti)\n", PLAGULAE[i],
                (integer)mensura);
            CREDO_VERUM (_tegumentum_probare(piscina, &ratum, textus,
                mensura));
            lectae = lectae + I;
            octeti = octeti + mensura;
        }

        /* Porta quae CORPUS SUUM non mensurat 'N/N mundum' dicere
         * potest cum nihil legerit: fixa III, octeti > XXX milia. */
        CREDO_AEQUALIS_I32 (lectae, III);
        CREDO_MAIOR_I32 (octeti, (i32)(XXX * M));
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
