/* probatio_js_lexema.c - lexator JS totalis (dialectus domus).
 *
 * Duo genera oraculorum:
 *  (1) series generum exspectata pro fontibus parvis - pericula
 *      nominata imprimis: REGEX contra DIVISIONEM (heuristice
 *      lexematis prioris), FORMULAE cum interpolationibus nidatis
 *      (acervus bracarum), munch maximus interpunctionis;
 *  (2) FIDELITAS SUPER CORPUS: quaeque fixtura in
 *      probationes/fixa/js/ per lexare->emittere octetim redit;
 *      fixturae mundae praeterea genera IMPERFECTA et IGNOTA
 *      NULLA ferant oportet (componentia.js 113KB, speculum.js,
 *      dialectus domus manu scriptus). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "js_lexema.h"
#include "filum.h"
#include "iter_directoria.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

interior Xar*
_lexare_literis (
               Piscina* piscina,
    constans character* fons);

interior Xar*
_lexare_literis (
               Piscina* piscina,
    constans character* fons)
{
    redde js_lexare(piscina, fons, (i32)strlen(fons));
}

/* seriem generum probare (FINIS exclusa) */
interior vacuum
_seriem_probare (
                   Piscina* piscina,
        constans character* fons,
    constans JsLexemaGenus* exspectata,
                       i32  numerus);

interior vacuum
_seriem_probare (Piscina* piscina, constans character* fons,
    constans JsLexemaGenus* exspectata, i32 numerus)
{
    Xar* l = _lexare_literis(piscina, fons);
    i32  k;

    CREDO_NON_NIHIL (l);
    si (l == NIHIL)
    {
        redde;
    }
    si ((i32)xar_numerus(l) != numerus + 1)
    {
        imprimere("  fons: %s\n", fons);
    }
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), numerus + 1);
    per (k = 0; k < numerus && k < xar_numerus(l); k = k + 1)
    {
        JsLexema* lx = (JsLexema*)xar_obtinere(l, k);

        si (lx->genus != exspectata[k])
        {
            imprimere("  [%d] '%.*s': %s pro %s exspectato\n",
                (integer)k, (integer)lx->valor.mensura,
                (constans character*)lx->valor.datum,
                js_lexema_genus_nomen(lx->genus),
                js_lexema_genus_nomen(exspectata[k]));
        }
        CREDO_AEQUALIS_I32 ((i32)lx->genus, (i32)exspectata[k]);
    }
}

/* valorem lexematis k probare (extensio verbatim) */
interior vacuum
_valorem_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  index,
    constans character* exspectatus);

interior vacuum
_valorem_probare (Piscina* piscina, constans character* fons,
    i32 index, constans character* exspectatus)
{
         Xar* l = _lexare_literis(piscina, fons);
    JsLexema* lx;
         i32  m = (i32)strlen(exspectatus);

    CREDO_NON_NIHIL (l);
    si (l == NIHIL || index >= xar_numerus(l))
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    lx = (JsLexema*)xar_obtinere(l, index);
    si (lx->valor.mensura != m)
    {
        imprimere("  valor [%d] '%.*s' pro '%s'\n", (integer)index,
            (integer)lx->valor.mensura,
            (constans character*)lx->valor.datum, exspectatus);
    }
    CREDO_AEQUALIS_I32 ((i32)lx->valor.mensura, m);
    si (lx->valor.mensura == m && m > 0)
    {
        CREDO_VERUM (memcmp(lx->valor.datum, exspectatus,
            (memoriae_index)m) == 0);
    }
}

/* fidelitas + progressus: emissio == fons, nullum lexema vacuum
 * nisi FINIS */
interior b32
_fidelis (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

interior b32
_fidelis (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
       Xar* l = js_lexare(piscina, fons, mensura);
    chorda  e;
       i32  k;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        JsLexema* lx = (JsLexema*)xar_obtinere(l, k);

        si (lx->genus != JS_LEX_FINIS && lx->valor.mensura == 0)
        {
            redde FALSUM;   /* progressus violatus */
        }
    }
    e = js_lexemata_emittere(piscina, l);
    si (e.mensura != mensura)
    {
        redde FALSUM;
    }
    redde mensura == 0
        || memcmp(e.datum, fons, (memoriae_index)mensura) == 0;
}

/* numerus generum imperfectorum ET ignotorum in fonte */
interior i32
_imperfecta_numerare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

interior i32
_imperfecta_numerare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Xar* l = js_lexare(piscina, fons, mensura);
    i32  k;
    i32  n = 0;

    si (l == NIHIL)
    {
        redde (i32)9999;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        JsLexema* lx = (JsLexema*)xar_obtinere(l, k);

        si (   lx->genus == JS_LEX_CHORDA_IMPERFECTA
            || lx->genus == JS_LEX_REGEX_IMPERFECTA
            || lx->genus == JS_LEX_FORMULA_IMPERFECTA
            || lx->genus == JS_LEX_COMMENTARIUM_IMPERFECTUM
            || lx->genus == JS_LEX_IGNOTUM)
        {
            n = n + 1;
        }
    }
    redde n;
}

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum(
        "probatio_js_lexema", 67108864);

    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Verba et identificatores ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_VERBUM, JS_LEX_SPATIA, JS_LEX_IDENTIFICATOR,
            JS_LEX_SPATIA, JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "let x = fn", exspectata, 7);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_VERBUM, JS_LEX_SPATIA, JS_LEX_IDENTIFICATOR,
            JS_LEX_SPATIA, JS_LEX_VERBUM, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "class A extends B",
            exspectata, 7);
    }
    {
        /* praefixum verbi non verbum */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "letx typeofy", exspectata, 3);
    }

    imprimere("\n--- Numeri ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_NUMERUS, JS_LEX_SPATIA, JS_LEX_NUMERUS,
            JS_LEX_SPATIA, JS_LEX_NUMERUS, JS_LEX_SPATIA,
            JS_LEX_NUMERUS, JS_LEX_SPATIA, JS_LEX_NUMERUS,
            JS_LEX_SPATIA, JS_LEX_NUMERUS, JS_LEX_SPATIA,
            JS_LEX_NUMERUS, JS_LEX_SPATIA, JS_LEX_NUMERUS };

        _seriem_probare(piscina,
            "12 1.5 .5 1e3 1E+3 0x1F 0b101 0o17", exspectata, 15);
    }
    {
        /* munch maximus numeri: '1..toString' = '1.' '.' nomen */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_NUMERUS, JS_LEX_INTERPUNCTIO,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "1..toString", exspectata, 3);
        _valorem_probare(piscina, "1..toString", (i32)0, "1.");
    }

    imprimere("\n--- Chordae ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_CHORDA };

        _seriem_probare(piscina, "'a\"b'", exspectata, 1);
        _seriem_probare(piscina, "'a\\'b'", exspectata, 1);
        /* continuatio: '\' + linea nova intra chordam */
        _seriem_probare(piscina, "'a\\\nb'", exspectata, 1);
    }
    {
        /* linea nova cruda: imperfecta, linea NON consumpta */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_CHORDA_IMPERFECTA, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR, JS_LEX_CHORDA_IMPERFECTA };

        _seriem_probare(piscina, "'a\nb'", exspectata, 4);
    }

    imprimere("\n--- Regex contra divisionem ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA, JS_LEX_REGEX };

        _seriem_probare(piscina, "x = /ab/g", exspectata, 5);
        _valorem_probare(piscina, "x = /ab/g", (i32)4, "/ab/g");
    }
    {
        /* divisio bis post identificatores */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "a / b / c", exspectata, 9);
    }
    {
        /* post 'return' regex (verbum sine valore) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_VERBUM, JS_LEX_SPATIA, JS_LEX_REGEX,
            JS_LEX_INTERPUNCTIO, JS_LEX_IDENTIFICATOR,
            JS_LEX_INTERPUNCTIO, JS_LEX_IDENTIFICATOR,
            JS_LEX_INTERPUNCTIO };

        _seriem_probare(piscina, "return /a/.test(s)",
            exspectata, 8);
    }
    {
        /* post 'this' divisio (verbum valorem ferens) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_VERBUM, JS_LEX_SPATIA, JS_LEX_INTERPUNCTIO,
            JS_LEX_SPATIA, JS_LEX_NUMERUS };

        _seriem_probare(piscina, "this / 2", exspectata, 5);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_VERBUM, JS_LEX_SPATIA, JS_LEX_REGEX };

        _seriem_probare(piscina, "typeof /re/", exspectata, 3);
    }
    {
        /* post ')' et ']' divisio (simplificatio domus) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_INTERPUNCTIO, JS_LEX_NUMERUS,
            JS_LEX_INTERPUNCTIO, JS_LEX_INTERPUNCTIO,
            JS_LEX_NUMERUS };

        _seriem_probare(piscina, "(1)/2", exspectata, 5);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_INTERPUNCTIO,
            JS_LEX_NUMERUS, JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA, JS_LEX_NUMERUS };

        _seriem_probare(piscina, "x[0] / 2", exspectata, 8);
    }
    {
        /* '++' postfixum praesumptum -> divisio */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_INTERPUNCTIO,
            JS_LEX_SPATIA, JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "a++ / b", exspectata, 6);
    }
    {
        /* '/=' utrimque: divisio-assignatio et regex '=' */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA, JS_LEX_NUMERUS };

        _seriem_probare(piscina, "x /= 2", exspectata, 5);
        _valorem_probare(piscina, "x /= 2", (i32)2, "/=");
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_INTERPUNCTIO,
            JS_LEX_REGEX, JS_LEX_INTERPUNCTIO };

        _seriem_probare(piscina, "f(/=/)", exspectata, 4);
        _valorem_probare(piscina, "f(/=/)", (i32)2, "/=/");
    }
    {
        /* classis characterum: '/' intra [..] litteralis */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_REGEX };

        _seriem_probare(piscina, "/^a[/]b$/i", exspectata, 1);
        _valorem_probare(piscina, "/^a[/]b$/i", (i32)0,
            "/^a[/]b$/i");
    }
    {
        /* linea nova intra regex: imperfecta, linea manet */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_REGEX_IMPERFECTA, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "x = /ab\nc", exspectata, 7);
    }

    imprimere("\n--- Formulae (litterae formularum) ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_TOTA };

        _seriem_probare(piscina, "`salve`", exspectata, 1);
        /* multilinearis - lexema unum trans lineas */
        _seriem_probare(piscina, "`li\nnea`", exspectata, 1);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_IDENTIFICATOR,
            JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`a${b}c`", exspectata, 3);
        _valorem_probare(piscina, "`a${b}c`", (i32)0, "`a${");
        _valorem_probare(piscina, "`a${b}c`", (i32)2, "}c`");
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_IDENTIFICATOR,
            JS_LEX_FORMULA_MEDIA, JS_LEX_IDENTIFICATOR,
            JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`a${b}c${d}e`", exspectata, 5);
        _valorem_probare(piscina, "`a${b}c${d}e`", (i32)2,
            "}c${");
    }
    {
        /* obiectum intra interpolationem: liber bracarum -
         * '}' interior interpunctio manet, ultimus resumit */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_IDENTIFICATOR,
            JS_LEX_INTERPUNCTIO, JS_LEX_NUMERUS,
            JS_LEX_INTERPUNCTIO, JS_LEX_INTERPUNCTIO,
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`x${ {a:1}.a }y`",
            exspectata, 11);
    }
    {
        /* formula intra formulam (acervus profundior) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_FORMULA_INITIUM,
            JS_LEX_IDENTIFICATOR, JS_LEX_FORMULA_FINIS,
            JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`a${`b${c}d`}e`",
            exspectata, 5);
    }
    {
        /* divisio et regex INTRA interpolationem */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_IDENTIFICATOR,
            JS_LEX_SPATIA, JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_NUMERUS, JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`${a / 2}`", exspectata, 7);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_REGEX,
            JS_LEX_FORMULA_FINIS };

        _seriem_probare(piscina, "`${/re/}`", exspectata, 3);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_IMPERFECTA };

        _seriem_probare(piscina, "`apertum", exspectata, 1);
    }
    {
        /* EOF intra interpolationem: truncatio structurae
         * (INITIUM integrum, FINIS numquam venit) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_FORMULA_INITIUM, JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "`apertum${x", exspectata, 2);
    }

    imprimere("\n--- Commentaria ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_COMMENTARIUM };

        _seriem_probare(piscina, "// x", exspectata, 1);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_COMMENTARIUM, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "// x\ny", exspectata, 3);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_COMMENTARIUM, JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "/* a\nb */x", exspectata, 2);
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_COMMENTARIUM_IMPERFECTUM };

        _seriem_probare(piscina, "/* ap", exspectata, 1);
    }

    imprimere("\n--- Munch maximus interpunctionis ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_INTERPUNCTIO, JS_LEX_SPATIA,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "a >>>= b", exspectata, 5);
        _valorem_probare(piscina, "a >>>= b", (i32)2, ">>>=");
        _seriem_probare(piscina, "x === y", exspectata, 5);
        _valorem_probare(piscina, "x === y", (i32)2, "===");
        _seriem_probare(piscina, "a => b", exspectata, 5);
        _valorem_probare(piscina, "a => b", (i32)2, "=>");
        _seriem_probare(piscina, "x ?? y", exspectata, 5);
        _valorem_probare(piscina, "x ?? y", (i32)2, "??");
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_INTERPUNCTIO, JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "...rest", exspectata, 2);
        _valorem_probare(piscina, "...rest", (i32)0, "...");
    }
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_INTERPUNCTIO,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "a?.b", exspectata, 3);
        _valorem_probare(piscina, "a?.b", (i32)1, "?.");
    }
    {
        /* '?.' + digitus = ternarium cum '.5' (regula ES2020) */
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IDENTIFICATOR, JS_LEX_INTERPUNCTIO,
            JS_LEX_NUMERUS, JS_LEX_INTERPUNCTIO,
            JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "c?.5:d", exspectata, 5);
        _valorem_probare(piscina, "c?.5:d", (i32)1, "?");
        _valorem_probare(piscina, "c?.5:d", (i32)2, ".5");
    }

    imprimere("\n--- Ignota, NUL, fidelitas marginum ---\n");
    {
        interior constans JsLexemaGenus exspectata[] = {
            JS_LEX_IGNOTUM, JS_LEX_IDENTIFICATOR, JS_LEX_SPATIA,
            JS_LEX_IGNOTUM, JS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "#x @y", exspectata, 5);
    }
    {
        interior constans character truncus[] = { 'a', '\0', 'b' };

        CREDO_VERUM (_fidelis(piscina, truncus, (i32)3));
        {
            Xar* l = js_lexare(piscina, truncus, (i32)3);

            CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), (i32)4);
            CREDO_AEQUALIS_I32 (
                (i32)((JsLexema*)xar_obtinere(l, 1))->genus,
                (i32)JS_LEX_IGNOTUM);
        }
        CREDO_VERUM (_fidelis(piscina, "", (i32)0));
    }
    {
        interior constans character* margines[] = {
            "\"ap", "'a\\", "/* ap", "`a${b", "x=/a[/",
            "`li\nnea ${a}`", "'a\r\nb'", "\\", "x=/ab\\",
            "`${`${`${a`" };
        i32 k;

        per (k = 0; k < (i32)10; k = k + 1)
        {
            CREDO_VERUM (_fidelis(piscina, margines[k],
                (i32)strlen(margines[k])));
        }
    }

    imprimere("\n--- Positiones (octeti, 1-basatae) ---\n");
    {
             Xar* l = _lexare_literis(piscina, "a\n  bc");
        JsLexema* lx;

        CREDO_NON_NIHIL (l);
        lx = (JsLexema*)xar_obtinere(l, 2);
        CREDO_AEQUALIS_I32 ((i32)lx->genus,
            (i32)JS_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 (lx->offset, (i32)4);
        CREDO_AEQUALIS_I32 (lx->linea, (i32)2);
        CREDO_AEQUALIS_I32 (lx->columna, (i32)3);
    }
    {
        /* post formulam multilinearem linea recte numeratur */
             Xar* l = _lexare_literis(piscina, "`a\nb`\nx");
        JsLexema* lx;

        CREDO_NON_NIHIL (l);
        lx = (JsLexema*)xar_obtinere(l, 2);
        CREDO_AEQUALIS_I32 ((i32)lx->genus,
            (i32)JS_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 (lx->linea, (i32)3);
        CREDO_AEQUALIS_I32 (lx->columna, (i32)1);
    }

    imprimere("\n--- Fidelitas super corpus (fixa/js) ---\n");
    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
                         i32  probatae = 0;

        iter = directorium_iterator_aperire("probationes/fixa/js",
            piscina);
        CREDO_NON_NIHIL (iter);
        si (iter != NIHIL)
        {
            dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
            {
                ChordaAedificator* aed;
                           chorda  via;
                           chorda  contentum;
                              b32  adversaria;

                si (e->genus != INTROITUS_FILUM)
                {
                    perge;
                }
                aed = chorda_aedificator_creare(piscina,
                    (memoriae_index)128);
                chorda_aedificator_appendere_literis(aed,
                    "probationes/fixa/js/");
                chorda_aedificator_appendere_chorda(aed, e->titulus);
                via = chorda_aedificator_finire(aed);
                contentum = filum_legere_totum(
                    chorda_ut_cstr(via, piscina), piscina);
                imprimere("  %.*s (%d octeti)\n",
                    (integer)e->titulus.mensura,
                    (constans character*)e->titulus.datum,
                    (integer)contentum.mensura);
                CREDO_VERUM (_fidelis(piscina,
                    (constans character*)contentum.datum,
                    contentum.mensura));
                /* plagulae mundae genera imperfecta nulla ferant */
                adversaria = chorda_continet(e->titulus,
                    chorda_ex_literis("adversarius", piscina));
                si (!adversaria)
                {
                    CREDO_AEQUALIS_I32 (_imperfecta_numerare(
                        piscina,
                        (constans character*)contentum.datum,
                        contentum.mensura), (i32)0);
                }
                alioquin
                {
                    CREDO_VERUM (_imperfecta_numerare(piscina,
                        (constans character*)contentum.datum,
                        contentum.mensura) > 0);
                }
                probatae = probatae + 1;
            }
            directorium_iterator_claudere(iter);
        }
        CREDO_VERUM (probatae >= (i32)4);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        redde ZEPHYRUM;
    }
    redde I;
}
