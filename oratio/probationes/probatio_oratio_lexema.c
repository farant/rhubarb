/* probatio_oratio_lexema.c - Lexator orationis (T2)
 *
 * Fixturae: genera et positiones lexematum (litterae, interpunctio per
 * octetum et cursus eiusdem, punctum solum contra '...', hyphen et
 * apostrophus inter litteras contra extra, digiti, spatia, CRLF et CR
 * solus, UTF-8 ut litterae, NBSP spatium, citationes curvae et linea
 * longa interpunctio, apostrophus curvus inter litteras, octeti
 * imperii signum); lex octetorum: valores concatenati == fons in
 * omnibus; nullum lexema "</or-" continet. CORPUS: omnis plagula md
 * tracta lexata octetim (numeri publicati). Culpa plantata: '...'
 * ut puncta tria.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_lexema.h"
#include "oratio_lexicon.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior constans MateriaToken*
_t (
    constans Xar* xs,
             i32  i)
{
    redde *(constans MateriaToken**)xar_obtinere(xs, i);
}

interior b32
_aequalis (
    constans MateriaToken* t,
                      s32  genus,
       constans character* valor)
{
    i32 l = (i32)strlen(valor);

    redde (b32)(t->genus == genus && t->valor.mensura == l
        && (l == ZEPHYRUM || memcmp(t->valor.datum, valor, (size_t)l)
            == ZEPHYRUM));
}

/* concatenatio valorum == fons; nullum "</or-" */
interior b32
_lex_octetorum (
          constans Xar* xs,
    constans character* fons,
                   i32  mensura)
{
    i32 n = xar_numerus(xs);
    i32 i;
    s32 cursor = ZEPHYRUM;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaToken* t = _t(xs, i);

        si (t->byte_offset != cursor)
        {
            redde FALSUM;
        }
        si (   t->valor.mensura > ZEPHYRUM
            && memcmp(fons + cursor, t->valor.datum,
            (size_t)t->valor.mensura)
                != ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (t->valor.mensura >= V)
        {
            i32 k;

            per (k = ZEPHYRUM; k + V <= t->valor.mensura; k++)
            {
                si (memcmp((constans character*)t->valor.datum + k,
                    "</or-",
                        V) == ZEPHYRUM)
                {
                    redde FALSUM;
                }
            }
        }
        cursor = cursor + (s32)t->valor.mensura;
    }
    redde (b32)(cursor == (s32)mensura
        && _t(xs, n - I)->genus == (s32)ORATIO_LEX_FINIS);
}

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

s32
principale (vacuum)
{
               Piscina* piscina;
                   Xar* xs;
    constans character* radix_viae;

    piscina = piscina_generare_dynamicum("probatio_oratio_lexema",
        4194304);
    credo_aperire(piscina);

    imprimere("\n--- Probans genera et positiones ---\n");
    {
        constans character* f = "Hello, world!\nOk.";

        xs = oratio_lexare(piscina, f, (i32)strlen(f));
        CREDO_NON_NIHIL (xs);
        CREDO_AEQUALIS_I32 (xar_numerus(xs), IX);
        CREDO_VERUM (_aequalis(_t(xs, ZEPHYRUM),
            (s32)ORATIO_LEX_LITTERAE, "Hello"));
        CREDO_VERUM (_aequalis(_t(xs, I), (s32)ORATIO_LEX_INTERPUNCTIO,
            ","));
        CREDO_VERUM (_aequalis(_t(xs, II), (s32)ORATIO_LEX_SPATIUM,
            " "));
        CREDO_VERUM (_aequalis(_t(xs, III), (s32)ORATIO_LEX_LITTERAE,
            "world"));
        CREDO_VERUM (_aequalis(_t(xs, IV), (s32)ORATIO_LEX_INTERPUNCTIO,
            "!"));
        CREDO_VERUM (_aequalis(_t(xs, V), (s32)ORATIO_LEX_LINEA, "\n"));
        CREDO_VERUM (_aequalis(_t(xs, VI), (s32)ORATIO_LEX_LITTERAE,
            "Ok"));
        CREDO_VERUM (_aequalis(_t(xs, VII), (s32)ORATIO_LEX_PUNCTUM,
            "."));
        CREDO_VERUM (_aequalis(_t(xs, VIII), (s32)ORATIO_LEX_FINIS,
            ""));
        CREDO_AEQUALIS_I32 (_t(xs, III)->linea, I);
        CREDO_AEQUALIS_I32 (_t(xs, III)->columna, VIII);
        CREDO_AEQUALIS_I32 (_t(xs, VI)->linea, II);
        CREDO_AEQUALIS_I32 (_t(xs, VI)->columna, I);
        CREDO_AEQUALIS_I32 (_t(xs, VIII)->linea, II);
        CREDO_AEQUALIS_I32 (_t(xs, VIII)->columna, IV);
        CREDO_VERUM (_lex_octetorum(xs, f, (i32)strlen(f)));
    }

    imprimere("\n--- Probans iuncturas: hyphen, apostrophus, punctum, digiti ---\n");
    {
        constans character* f =
            "don't well-known e.g. 3.14 -- ... a - b 'q' x''y";

        xs = oratio_lexare(piscina, f, (i32)strlen(f));
        CREDO_NON_NIHIL (xs);
        CREDO_VERUM (_lex_octetorum(xs, f, (i32)strlen(f)));
        CREDO_VERUM (_aequalis(_t(xs, ZEPHYRUM),
            (s32)ORATIO_LEX_LITTERAE, "don"));
        CREDO_VERUM (_aequalis(_t(xs, I), (s32)ORATIO_LEX_APOSTROPHUS,
            "'"));
        CREDO_VERUM (_aequalis(_t(xs, II), (s32)ORATIO_LEX_LITTERAE,
            "t"));
        CREDO_VERUM (_aequalis(_t(xs, IV), (s32)ORATIO_LEX_LITTERAE,
            "well"));
        CREDO_VERUM (_aequalis(_t(xs, V), (s32)ORATIO_LEX_HYPHEN, "-"));
        CREDO_VERUM (_aequalis(_t(xs, VI), (s32)ORATIO_LEX_LITTERAE,
            "known"));
        CREDO_VERUM (_aequalis(_t(xs, VIII), (s32)ORATIO_LEX_LITTERAE,
            "e"));
        CREDO_VERUM (_aequalis(_t(xs, IX), (s32)ORATIO_LEX_PUNCTUM,
            "."));
        CREDO_VERUM (_aequalis(_t(xs, X), (s32)ORATIO_LEX_LITTERAE,
            "g"));
        CREDO_VERUM (_aequalis(_t(xs, XI), (s32)ORATIO_LEX_PUNCTUM,
            "."));
        CREDO_VERUM (_aequalis(_t(xs, XIII), (s32)ORATIO_LEX_DIGITI,
            "3"));
        CREDO_VERUM (_aequalis(_t(xs, XIV), (s32)ORATIO_LEX_PUNCTUM,
            "."));
        CREDO_VERUM (_aequalis(_t(xs, XV), (s32)ORATIO_LEX_DIGITI,
            "14"));
        CREDO_VERUM (_aequalis(_t(xs, XVII),
            (s32)ORATIO_LEX_INTERPUNCTIO, "--"));
        CREDO_VERUM (_aequalis(_t(xs, XIX),
            (s32)ORATIO_LEX_INTERPUNCTIO, "..."));
        CREDO_VERUM (_aequalis(_t(xs, XXIII),
            (s32)ORATIO_LEX_INTERPUNCTIO, "-"));
        CREDO_VERUM (_aequalis(_t(xs, XXVII),
            (s32)ORATIO_LEX_INTERPUNCTIO, "'"));
        CREDO_VERUM (_aequalis(_t(xs, XXVIII), (s32)ORATIO_LEX_LITTERAE,
            "q"));
        CREDO_VERUM (_aequalis(_t(xs, XXIX),
            (s32)ORATIO_LEX_INTERPUNCTIO, "'"));
        CREDO_VERUM (_aequalis(_t(xs, XXXI), (s32)ORATIO_LEX_LITTERAE,
            "x"));
        CREDO_VERUM (_aequalis(_t(xs, XXXII),
            (s32)ORATIO_LEX_INTERPUNCTIO, "''"));
        CREDO_VERUM (_aequalis(_t(xs, XXXIII), (s32)ORATIO_LEX_LITTERAE,
            "y"));
    }

    imprimere("\n--- Probans lineas: CRLF, CR solus ---\n");
    {
        constans character* f = "a\r\nb\rc\n";

        xs = oratio_lexare(piscina, f, (i32)strlen(f));
        CREDO_NON_NIHIL (xs);
        CREDO_VERUM (_lex_octetorum(xs, f, (i32)strlen(f)));
        CREDO_VERUM (_aequalis(_t(xs, I), (s32)ORATIO_LEX_LINEA,
            "\r\n"));
        CREDO_VERUM (_aequalis(_t(xs, III), (s32)ORATIO_LEX_LINEA_CR,
            "\r"));
        CREDO_VERUM (_aequalis(_t(xs, V), (s32)ORATIO_LEX_LINEA, "\n"));
        CREDO_AEQUALIS_I32 (_t(xs, IV)->linea, III);
        CREDO_AEQUALIS_I32 (_t(xs, VI)->linea, IV);
    }

    imprimere("\n--- Probans UTF-8: litterae, NBSP, interpunctio curva, apostrophus curvus ---\n");
    {
        /* "\xC3\x86r\xC3\xB8" = "Ærø"; NBSP; "\xE2\x80\x9C" = left quote;
         * "\xE2\x80\x94" = em dash; "\xE2\x80\x99" = right single quote */
        constans character* f =
            "\xC3\x86r\xC3\xB8 \xC2\xA0x \xE2\x80\x9Cq\xE2\x80\x9D \xE2\x80\x94 "
            "don\xE2\x80\x99t \xE2\x80\x99z \x01\x02.";

        xs = oratio_lexare(piscina, f, (i32)strlen(f));
        CREDO_NON_NIHIL (xs);
        CREDO_VERUM (_lex_octetorum(xs, f, (i32)strlen(f)));
        CREDO_VERUM (_aequalis(_t(xs, ZEPHYRUM),
            (s32)ORATIO_LEX_LITTERAE, "\xC3\x86r\xC3\xB8"));
        CREDO_VERUM (_aequalis(_t(xs, I), (s32)ORATIO_LEX_SPATIUM,
            " \xC2\xA0"));
        CREDO_VERUM (_aequalis(_t(xs, II), (s32)ORATIO_LEX_LITTERAE,
            "x"));
        CREDO_VERUM (_aequalis(_t(xs, IV), (s32)ORATIO_LEX_INTERPUNCTIO,
            "\xE2\x80\x9C"));
        CREDO_VERUM (_aequalis(_t(xs, V), (s32)ORATIO_LEX_LITTERAE,
            "q"));
        CREDO_VERUM (_aequalis(_t(xs, VI), (s32)ORATIO_LEX_INTERPUNCTIO,
            "\xE2\x80\x9D"));
        CREDO_VERUM (_aequalis(_t(xs, VIII),
            (s32)ORATIO_LEX_INTERPUNCTIO, "\xE2\x80\x94"));
        CREDO_VERUM (_aequalis(_t(xs, X), (s32)ORATIO_LEX_LITTERAE,
            "don"));
        CREDO_VERUM (_aequalis(_t(xs, XI), (s32)ORATIO_LEX_APOSTROPHUS,
            "\xE2\x80\x99"));
        CREDO_VERUM (_aequalis(_t(xs, XII), (s32)ORATIO_LEX_LITTERAE,
            "t"));
        CREDO_VERUM (_aequalis(_t(xs, XIV),
            (s32)ORATIO_LEX_INTERPUNCTIO, "\xE2\x80\x99"));
        CREDO_VERUM (_aequalis(_t(xs, XV), (s32)ORATIO_LEX_LITTERAE,
            "z"));
        CREDO_VERUM (_aequalis(_t(xs, XVII), (s32)ORATIO_LEX_SIGNUM,
            "\x01\x02"));
        CREDO_VERUM (_aequalis(_t(xs, XVIII), (s32)ORATIO_LEX_PUNCTUM,
            "."));
        CREDO_AEQUALIS_I32 (_t(xs, II)->columna, IX);   /* octeti, non characteres */
    }

    imprimere("\n--- Probans plagulam vacuam et derivatum ---\n");
    {
        MateriaToken* d;
        chorda v;

        xs = oratio_lexare(piscina, "", ZEPHYRUM);
        CREDO_NON_NIHIL (xs);
        CREDO_AEQUALIS_I32 (xar_numerus(xs), I);
        CREDO_VERUM (_aequalis(_t(xs, ZEPHYRUM), (s32)ORATIO_LEX_FINIS,
            ""));
        v = chorda_ex_literis("verbum substantivum", piscina);
        d = oratio_lexema_derivatum(piscina, (s32)ORATIO_LEX_DERIVATUM,
            v,
            _t(xs, ZEPHYRUM));
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 (d->fons_index, ORATIO_FONS_DERIVATUS);
        CREDO_AEQUALIS_S32 (d->byte_offset, (s32)-I);
        CREDO_VERUM (d->valor.datum != v.datum
            && d->valor.mensura == v.mensura);
    }

    imprimere("\n--- CORPUS: plagulae md tractae, lex octetorum ---\n");
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    {
        character via[DXII];
        FILE* lista;
        i32 plagulae = ZEPHYRUM;
        i32 fractae  = ZEPHYRUM;
        s64 lexemata = ZEPHYRUM;
        s64 octeti   = ZEPHYRUM;

        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
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
                   size_t  l        = strlen(linea);
                      Xar* ys;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("oratio_corpus",
                    16777216);
                sprintf(via, "%s/%s", radix_viae, linea);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                plagulae++;
                octeti  = octeti + (s64)mensura;
                ys      = oratio_lexare(p, textus, mensura);
                si (ys == NIHIL || !_lex_octetorum(ys, textus, mensura))
                {
                    fractae++;
                    si (fractae <= X)
                    {
                        imprimere("    FRACTA: %s\n", linea);
                    }
                }
                alioquin
                {
                    lexemata = lexemata + (s64)xar_numerus(ys);
                }
                piscina_destruere(p);
            }
            fclose(lista);
            imprimere("    plagulae %d, fractae %d, octeti %ld, lexemata %ld\n",
                (integer)plagulae, (integer)fractae, (longus)octeti,
                (longus)lexemata);
            CREDO_MAIOR_I32 (plagulae, (i32)1000);
            CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
