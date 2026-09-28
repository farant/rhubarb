/* probatio_toml_api.c - API publica toml (Q10)
 *
 * Casus tester lapide feature-requests/013 ad verbum (tabulae servatae,
 * via punctata, enumeratio, clavis iterata nominata cum sedibus II);
 * getters typati (adest, abest, genus alienum - exitus intactus, fines
 * s64); viae clavium (Review Focus 5: vacua, 'a..b', per seriem, in
 * chordam, segmentum quotatum cum puncto, effugia, spatia, cauda
 * aliena, quotatio aperta); enumeratio; pictura trium vitiorum ordine
 * octetorum cum via:linea:columna et caret; textus vacuus.
 */

#include "latina.h"
#include "credo.h"
#include "toml.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Adiumenta
 * ================================================== */

interior TomlDocumentum*
_legere (
               Piscina* piscina,
    constans character* litterae,
    constans character* via)
{
       chorda  textus;
          i32  n = (i32)strlen(litterae);
    character* f = (character*)piscina_allocare(piscina, (i64)n + I);

    memcpy(f, litterae, (size_t)n + I);
    textus.datum    = (i8*)f;
    textus.mensura  = n;
    redde toml_legere(textus, via, piscina);
}

interior b32
_aequat (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde c.mensura == n
        && (n == ZEPHYRUM || memcmp(c.datum, litterae, (size_t)n)
            == ZEPHYRUM);
}

/* positio primae occurrentiae (aut -I) */
interior s32
_invenire (
                 chorda  c,
     constans character* acus)
{
    i32 n = (i32)strlen(acus);
    i32 k;

    per (k = ZEPHYRUM; k + n <= c.mensura; k++)
    {
        si (memcmp(c.datum + k, acus, (size_t)n) == ZEPHYRUM)
        {
            redde (s32)k;
        }
    }
    redde (s32)-I;
}

interior i32
_numerare (
       chorda c,
    character signum)
{
    i32 k;
    i32 summa = ZEPHYRUM;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        si (c.datum[k] == (i8)signum)
        {
            summa++;
        }
    }
    redde summa;
}

hic_manens constans character* GENERA_VALORUM =
    "s = \"x\"\n"
    "i = 42\n"
    "f = 1.5\n"
    "b = true\n"
    "t = 1979-05-27T07:32:00Z\n"
    "max = 9223372036854775807\n"
    "min = -9223372036854775808\n"
    "arr = [1, 2]\n"
    "[tab]\n"
    "\"b.c\" = 7\n"
    "kA = 8\n";


s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_toml_api", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * lapide feature-requests/013 ad verbum
     * ================================================== */

    {
           TomlDocumentum* doc;
                   chorda  c;
       constans TomlValor* radix;
                   chorda  textus;

        imprimere("\n--- Probans feature-requests/013 ---\n");
        doc = _legere(piscina,
            "[llama-server]\nversio = \"b11201\"\n"
            "[poppler]\nversio = \"26.09.0\"\n", "config.toml");
        CREDO_VERUM (toml_successus(doc));
        CREDO_AEQUALIS_I32 (xar_numerus(toml_diagnostica(doc)),
            ZEPHYRUM);
        CREDO_VERUM (toml_chorda(doc, "llama-server.versio", &c));
        CREDO_VERUM (_aequat(c, "b11201"));
        CREDO_VERUM (toml_chorda(doc, "poppler.versio", &c));
        CREDO_VERUM (_aequat(c, "26.09.0"));
        radix = toml_radix(doc);
        CREDO_AEQUALIS_I32 (toml_tabulae_numerus(radix), II);
        CREDO_VERUM (_aequat(toml_tabulae_clavis(radix, ZEPHYRUM),
            "llama-server"));
        CREDO_VERUM (_aequat(toml_tabulae_clavis(radix, I), "poppler"));
        CREDO_AEQUALIS_S32 ((s32)toml_tabulae_valor(radix, I)->genus,
            (s32)TOML_VALOR_TABULA);
        textus = toml_diagnostica_scribere(piscina, doc, VERUM);
        CREDO_AEQUALIS_I32 (textus.mensura, ZEPHYRUM);

        /* versio bis in sectione UNA: refutatur, nominatur, sedes II */
        doc = _legere(piscina,
            "[llama-server]\nversio = \"b11201\"\nversio = \"b11202\"\n"
            "[poppler]\nversio = \"26.09.0\"\n", "config.toml");
        CREDO_FALSUM (toml_successus(doc));
        textus = toml_diagnostica_scribere(piscina, doc, VERUM);
        imprimere("%.*s", (integer)textus.mensura,
            (constans character*)textus.datum);
        CREDO_MAIOR_S32 (_invenire(textus,
            "clavis 'versio' iam definita"),
            (s32)-I);
        CREDO_MAIOR_S32 (_invenire(textus, "config.toml:3:1"), (s32)-I);
        CREDO_MAIOR_I32 (_numerare(textus, '^'), I);
        /* definitio prima manet quaerenda */
        CREDO_VERUM (toml_chorda(doc, "llama-server.versio", &c));
        CREDO_VERUM (_aequat(c, "b11201"));
    }


    /* ==================================================
     * Getters typati
     * ================================================== */

    {
         TomlDocumentum* doc = _legere(piscina, GENERA_VALORUM,
             "typati.toml");
                 chorda c;
                    s64 i = (s64)VII;
                    f64 f = 0.0;
                    b32 b = FALSUM;
             TomlTempus t;
                    s64 maximus = ((s64)(((i64)I << LXIII) - I));

        imprimere("\n--- Probans getters typatos ---\n");
        CREDO_VERUM (toml_successus(doc));
        CREDO_VERUM (toml_chorda(doc, "s", &c));
        CREDO_VERUM (_aequat(c, "x"));
        CREDO_FALSUM (toml_integer(doc, "s", &i));
        CREDO_AEQUALIS_S64 (i, (s64)VII);         /* intactus */
        CREDO_VERUM (toml_integer(doc, "i", &i));
        CREDO_AEQUALIS_S64 (i, (s64)XLII);
        CREDO_VERUM (toml_fluitans(doc, "f", &f));
        CREDO_VERUM (f == 1.5);
        CREDO_FALSUM (toml_fluitans(doc, "i", &f));
        CREDO_VERUM (toml_boolean(doc, "b", &b));
        CREDO_VERUM (b);
        CREDO_VERUM (toml_tempus(doc, "t", &t));
        CREDO_AEQUALIS_S32 (t.annus, (s32)MCMLXXIX);
        CREDO_AEQUALIS_S32 ((s32)t.genus, (s32)TOML_TEMPUS_CUM_ZONA);
        CREDO_VERUM (toml_integer(doc, "max", &i));
        CREDO_AEQUALIS_S64 (i, maximus);
        CREDO_VERUM (toml_integer(doc, "min", &i));
        CREDO_AEQUALIS_S64 (i, -maximus - I);
        CREDO_FALSUM (toml_integer(doc, "nope", &i));
        CREDO_FALSUM (toml_chorda(doc, "tab", &c));
    }


    /* ==================================================
     * Viae clavium (Review Focus 5)
     * ================================================== */

    {
         TomlDocumentum* doc = _legere(piscina, GENERA_VALORUM,
             "typati.toml");
                    s64 i;

        imprimere("\n--- Probans vias clavium ---\n");
        CREDO_NIHIL (toml_quaerere(doc, ""));
        CREDO_NIHIL (toml_quaerere(doc, "   "));
        CREDO_NIHIL (toml_quaerere(doc, "a..b"));
        CREDO_NIHIL (toml_quaerere(doc, "tab..kA"));
        CREDO_NIHIL (toml_quaerere(doc, "arr.x"));
        CREDO_NIHIL (toml_quaerere(doc, "s.y"));
        CREDO_NIHIL (toml_quaerere(doc, "tab."));
        CREDO_NIHIL (toml_quaerere(doc, ".tab"));
        CREDO_NIHIL (toml_quaerere(doc, "tab kA"));
        CREDO_NIHIL (toml_quaerere(doc, "tab.b.c"));
        CREDO_NIHIL (toml_quaerere(doc, "tab.\"b.c"));
        CREDO_NIHIL (toml_quaerere(doc, "tab.\"\\q\""));
        CREDO_NIHIL (toml_quaerere(doc, "tab = 1"));
        CREDO_NIHIL (toml_quaerere(NIHIL, "tab"));
        CREDO_NIHIL (toml_quaerere(doc, NIHIL));
        CREDO_VERUM (toml_integer(doc, "tab.\"b.c\"", &i));
        CREDO_AEQUALIS_S64 (i, (s64)VII);
        CREDO_VERUM (toml_integer(doc, "tab.'b.c'", &i));
        CREDO_AEQUALIS_S64 (i, (s64)VII);
        CREDO_VERUM (toml_integer(doc, "tab.\"k\\u0041\"", &i));
        CREDO_AEQUALIS_S64 (i, (s64)VIII);
        CREDO_VERUM (toml_integer(doc, " tab . kA ", &i));
        CREDO_AEQUALIS_S64 (i, (s64)VIII);
        CREDO_NON_NIHIL (toml_quaerere(doc, "tab"));
    }


    /* ==================================================
     * Enumeratio
     * ================================================== */

    {
            TomlDocumentum* doc = _legere(piscina, GENERA_VALORUM,
                "typati.toml");
        constans TomlValor* radix  = toml_radix(doc);
        constans TomlValor* arr    = toml_quaerere(doc, "arr");

        imprimere("\n--- Probans enumerationem ---\n");
        CREDO_AEQUALIS_I32 (toml_tabulae_numerus(radix), IX);
        CREDO_VERUM (_aequat(toml_tabulae_clavis(radix, ZEPHYRUM),
            "s"));
        CREDO_VERUM (_aequat(toml_tabulae_clavis(radix, VIII), "tab"));
        CREDO_AEQUALIS_S32 ((s32)toml_tabulae_valor(radix, VIII)->genus,
            (s32)TOML_VALOR_TABULA);
        CREDO_NIHIL (toml_tabulae_valor(radix, IX));
        CREDO_AEQUALIS_I32 (toml_tabulae_clavis(radix, IX).mensura,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (toml_seriei_numerus(arr), II);
        CREDO_AEQUALIS_S64 (toml_seriei_elementum(arr, I)->datum
            .integer_valor, (s64)II);
        CREDO_NIHIL (toml_seriei_elementum(arr, II));
        CREDO_AEQUALIS_I32 (toml_seriei_numerus(radix), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (toml_tabulae_numerus(arr), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (toml_tabulae_numerus(NIHIL), ZEPHYRUM);
    }


    /* ==================================================
     * Pictura: tria vitia, ordine octetorum
     * ================================================== */

    {
         TomlDocumentum* doc;
                 chorda  textus;
                    s32  p2;
                    s32  p3;
                    s32  p4;

        imprimere("\n--- Probans picturam ---\n");
        doc = _legere(piscina,
            "a = 1\na = 2\nb = 9223372036854775808\nc = ?\n",
            "tres.toml");
        CREDO_FALSUM (toml_successus(doc));
        CREDO_AEQUALIS_I32 (xar_numerus(toml_diagnostica(doc)), III);
        textus = toml_diagnostica_scribere(piscina, doc, VERUM);
        imprimere("%.*s", (integer)textus.mensura,
            (constans character*)textus.datum);
        p2 = _invenire(textus, "tres.toml:2:1");
        p3 = _invenire(textus, "tres.toml:3:5");
        p4 = _invenire(textus, "tres.toml:4:5");
        CREDO_MAIOR_S32 (p2, (s32)-I);
        CREDO_MAIOR_S32 (p3, p2);
        CREDO_MAIOR_S32 (p4, p3);
        CREDO_MAIOR_I32 (_numerare(textus, '^'), II);
        textus = toml_diagnostica_scribere(piscina, doc, FALSUM);
        CREDO_AEQUALIS_I32 (_numerare(textus, '^'), ZEPHYRUM);
        CREDO_MAIOR_S32 (_invenire(textus, "tres.toml:4:5"), (s32)-I);
    }


    /* ==================================================
     * toml_chordae: series chordarum
     * ================================================== */

    {
        TomlDocumentum* doc = _legere(piscina,
            "a = [\"x\", \"yz\"]\nm = [\"x\", 1]\ne = []\ns = \"x\"\n",
            "chordae.toml");
                   Xar* x = NIHIL;

        imprimere("\n--- Probans toml_chordae ---\n");
        CREDO_VERUM (toml_chordae(doc, "a", piscina, &x));
        CREDO_AEQUALIS_I32 (xar_numerus(x), II);
        CREDO_VERUM (_aequat(*(chorda*)xar_obtinere(x, I), "yz"));
        x = NIHIL;
        /* mixta; exitus intactus */
        CREDO_FALSUM (toml_chordae(doc, "m", piscina, &x));
        CREDO_NIHIL (x);
        /* non series; absens; vacua licet */
        CREDO_FALSUM (toml_chordae(doc, "s", piscina, &x));
        CREDO_FALSUM (toml_chordae(doc, "nope", piscina,
            &x));
        CREDO_VERUM (toml_chordae(doc, "e", piscina, &x));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
    }


    /* ==================================================
     * Casus probationes/probatio_toml.c (lib/toml.c, Q12 deleta)
     * re-collocati: significatio NOVA (sectiones non planae)
     * ================================================== */

    {
        TomlDocumentum* doc;
                 chorda c;
                    s64 n;
                    b32 b;
                   Xar* x;

        imprimere("\n--- Probans casus bibliothecae veteris ---\n");
        doc = _legere(piscina, "Title = \"Hello World\"", "v.toml");
        CREDO_VERUM (toml_chorda(doc, "Title", &c));
        CREDO_VERUM (_aequat(c, "Hello World"));
        doc = _legere(piscina, "Empty = \"\"", "v.toml");
        CREDO_VERUM (toml_chorda(doc, "Empty", &c));
        CREDO_AEQUALIS_I32 (c.mensura, ZEPHYRUM);
        doc = _legere(piscina, "Year = 1920", "v.toml");
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        CREDO_AEQUALIS_S64 (n, (s64)MCMXX);
        doc = _legere(piscina, "Offset = -100", "v.toml");
        CREDO_VERUM (toml_integer(doc, "Offset", &n));
        CREDO_AEQUALIS_S64 (n, -(s64)C);
        doc = _legere(piscina, "Year = -371", "v.toml");
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        CREDO_AEQUALIS_S64 (n, -(s64)CCCLXXI);
        doc = _legere(piscina, "Tags = [\"fiction\", \"drama\"]",
            "v.toml");
        CREDO_VERUM (toml_chordae(doc, "Tags", piscina, &x));
        CREDO_AEQUALIS_I32 (xar_numerus(x), II);
        doc = _legere(piscina,
            "Tags = [\n    \"fiction\",\n    \"drama\",\n"
            "    \"classic\"\n]", "v.toml");
        CREDO_VERUM (toml_chordae(doc, "Tags", piscina, &x));
        CREDO_AEQUALIS_I32 (xar_numerus(x), III);
        CREDO_VERUM (_aequat(*(chorda*)xar_obtinere(x, II), "classic"));
        doc = _legere(piscina,
            "Summary = \"\"\"\nThis is a multi-line\n"
            "string value.\n\"\"\"", "v.toml");
        CREDO_VERUM (toml_chorda(doc, "Summary", &c));
        CREDO_VERUM (_aequat(c,
            "This is a multi-line\nstring value.\n"));
        doc = _legere(piscina, "Title = \"The Great Gatsby\"\n"
            "Author = \"F. Scott Fitzgerald\"\nYear = 1925\n\n"
            "Tags = [\"fiction\", \"classic\", \"american\"]\n",
            "v.toml");
        CREDO_VERUM (toml_successus(doc));
        CREDO_VERUM (toml_chorda(doc, "Author", &c));
        CREDO_VERUM (_aequat(c, "F. Scott Fitzgerald"));
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        CREDO_AEQUALIS_S64 (n, (s64)MCMXXV);
        doc = _legere(piscina,
            "# This is a comment\nTitle = \"Hello\"\n"
            "# Another comment\nYear = 2020", "v.toml");
        CREDO_VERUM (toml_successus(doc));
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        CREDO_AEQUALIS_S64 (n, (s64)MMXX);
        doc = _legere(piscina, "  Title   =   \"Hello\"  \nYear=1920",
            "v.toml");
        CREDO_VERUM (toml_chorda(doc, "Title", &c));
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        doc = _legere(piscina, "Title \"Hello\"", "v.toml");
        CREDO_FALSUM (toml_successus(doc));
        doc = _legere(piscina, "Summary = \"\"\"\nUnclosed multiline",
            "v.toml");
        CREDO_FALSUM (toml_successus(doc));
        doc = _legere(piscina, "Active = true\nDisabled = false",
            "v.toml");
        CREDO_VERUM (toml_boolean(doc, "Active", &b));
        CREDO_VERUM (b);
        CREDO_VERUM (toml_boolean(doc, "Disabled", &b));
        CREDO_FALSUM (b);
        /* vetus sectionem planabat ('categories' in summo); nunc
         * tabula */
        doc = _legere(piscina, "Title = \"Test\"\n[Tags]\n"
            "categories = [\"fiction\"]\nactive = true", "v.toml");
        CREDO_VERUM (toml_successus(doc));
        CREDO_VERUM (toml_chorda(doc, "Title", &c));
        CREDO_NIHIL (toml_quaerere(doc, "categories"));
        CREDO_VERUM (toml_chordae(doc, "Tags.categories", piscina, &x));
        CREDO_VERUM (toml_boolean(doc, "Tags.active", &b));
        CREDO_VERUM (b);
        doc = _legere(piscina, "Title = \"Fruitfulness\"\n"
            "Author = \"Emile Zola\"\nYear = 1899\n\nTags = [\n"
            "    \"fiction\",\n    \"french\"\n]\n", "v.toml");
        CREDO_VERUM (toml_successus(doc));
        CREDO_VERUM (toml_integer(doc, "Year", &n));
        CREDO_AEQUALIS_S64 (n, (s64)MDCCCXCIX);
    }


    /* ==================================================
     * Textus vacuus (plagula vacua: datum NIHIL)
     * ================================================== */

    {
                chorda vacua;
        TomlDocumentum* doc;

        imprimere("\n--- Probans textum vacuum ---\n");
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        doc            = toml_legere(vacua, NIHIL, piscina);
        CREDO_NON_NIHIL (doc);
        CREDO_VERUM (toml_successus(doc));
        CREDO_AEQUALIS_I32 (toml_tabulae_numerus(toml_radix(doc)),
            ZEPHYRUM);
        CREDO_FALSUM (toml_successus(NIHIL));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
