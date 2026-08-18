/* probatio_css_lexema.c - lexator CSS totalis.
 *
 * Duo genera oraculorum:
 *  (1) series generum exspectata pro fontibus parvis (vocabularium,
 *      IMPERFECTA omnia, margines effugiorum et url);
 *  (2) FIDELITAS SUPER CORPUS: quaeque fixtura in
 *      probationes/fixa/css/ per lexare->emittere octetim redit;
 *      fixturae mundae (photographemata plagularum verarum domus)
 *      praeterea genera IMPERFECTA NULLA ferant oportet. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "css_lexema.h"
#include "filum.h"
#include "iter_directoria.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

interior Xar*
_lexare_literis (Piscina* piscina, constans character* fons);

interior Xar*
_lexare_literis (Piscina* piscina, constans character* fons)
{
    redde css_lexare(piscina, fons, (i32)strlen(fons));
}

/* seriem generum probare (FINIS exclusa) */
interior vacuum
_seriem_probare (Piscina* piscina, constans character* fons,
    constans CssLexemaGenus* exspectata, i32 numerus);

interior vacuum
_seriem_probare (Piscina* piscina, constans character* fons,
    constans CssLexemaGenus* exspectata, i32 numerus)
{
    Xar* l = _lexare_literis(piscina, fons);
    i32  k;

    CREDO_NON_NIHIL (l);
    si (l == NIHIL)
    {
        redde;
    }
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), numerus + 1);
    per (k = 0; k < numerus && k < xar_numerus(l); k = k + 1)
    {
        CssLexema* lx = (CssLexema*)xar_obtinere(l, k);

        si (lx->genus != exspectata[k])
        {
            imprimere("  [%d] '%.*s': %s pro %s exspectato\n",
                (integer)k, (integer)lx->valor.mensura,
                (constans character*)lx->valor.datum,
                css_lexema_genus_nomen(lx->genus),
                css_lexema_genus_nomen(exspectata[k]));
        }
        CREDO_AEQUALIS_I32 ((i32)lx->genus, (i32)exspectata[k]);
    }
}

/* fidelitas + progressus: emissio == fons, nullum lexema vacuum
 * nisi FINIS */
interior b32
_fidelis (Piscina* piscina, constans character* fons, i32 mensura);

interior b32
_fidelis (Piscina* piscina, constans character* fons, i32 mensura)
{
    Xar*   l = css_lexare(piscina, fons, mensura);
    chorda e;
    i32    k;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        CssLexema* lx = (CssLexema*)xar_obtinere(l, k);

        si (lx->genus != CSS_LEX_FINIS && lx->valor.mensura == 0)
        {
            redde FALSUM;   /* progressus violatus */
        }
    }
    e = css_lexemata_emittere(piscina, l);
    si (e.mensura != mensura)
    {
        redde FALSUM;
    }
    redde mensura == 0
        || memcmp(e.datum, fons, (memoriae_index)mensura) == 0;
}

/* numerus generum imperfectorum in fonte */
interior i32
_imperfecta_numerare (Piscina* piscina, constans character* fons,
    i32 mensura);

interior i32
_imperfecta_numerare (Piscina* piscina, constans character* fons,
    i32 mensura)
{
    Xar* l = css_lexare(piscina, fons, mensura);
    i32  k;
    i32  n = 0;

    si (l == NIHIL)
    {
        redde (i32)9999;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        CssLexema* lx = (CssLexema*)xar_obtinere(l, k);

        si (lx->genus == CSS_LEX_CHORDA_IMPERFECTA
            || lx->genus == CSS_LEX_URL_IMPERFECTA
            || lx->genus == CSS_LEX_COMMENTARIUM_IMPERFECTUM)
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
        "probatio_css_lexema", 33554432);

    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Vocabularium fundamentale ---\n");
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_IDENTIFICATOR, CSS_LEX_SPATIA,
            CSS_LEX_BRACE_APERTA, CSS_LEX_SPATIA,
            CSS_LEX_IDENTIFICATOR, CSS_LEX_COLON, CSS_LEX_SPATIA,
            CSS_LEX_IDENTIFICATOR, CSS_LEX_SEMICOLON,
            CSS_LEX_SPATIA, CSS_LEX_BRACE_CLAUSA };

        _seriem_probare(piscina, "a { b: c; }", exspectata, 11);
    }
    {
        /* functio, variabilis consueta, cancellum, delim */
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_DELIM, CSS_LEX_IDENTIFICATOR,
            CSS_LEX_BRACE_APERTA, CSS_LEX_IDENTIFICATOR,
            CSS_LEX_COLON, CSS_LEX_FUNCTIO,
            CSS_LEX_IDENTIFICATOR, CSS_LEX_COMMA, CSS_LEX_SPATIA,
            CSS_LEX_CANCELLUM, CSS_LEX_PAREN_CLAUSA,
            CSS_LEX_BRACE_CLAUSA };

        _seriem_probare(piscina, ".x{c:var(--v, #fff)}",
            exspectata, 12);
    }

    imprimere("\n--- Numeri, centesimae, dimensiones ---\n");
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_NUMERUS, CSS_LEX_SPATIA,
            CSS_LEX_NUMERUS, CSS_LEX_SPATIA,
            CSS_LEX_NUMERUS, CSS_LEX_SPATIA,
            CSS_LEX_NUMERUS, CSS_LEX_SPATIA,
            CSS_LEX_NUMERUS, CSS_LEX_SPATIA,
            CSS_LEX_CENTESIMA, CSS_LEX_SPATIA,
            CSS_LEX_DIMENSIO, CSS_LEX_SPATIA,
            CSS_LEX_DIMENSIO };

        _seriem_probare(piscina, "12 1.5 .5 +3 -2.5e-2 50% 12px 1.5em",
            exspectata, 15);
    }
    {
        /* -webkit-x et --varia identificatores, CDO/CDC */
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_IDENTIFICATOR, CSS_LEX_SPATIA,
            CSS_LEX_IDENTIFICATOR, CSS_LEX_SPATIA,
            CSS_LEX_CDO, CSS_LEX_SPATIA, CSS_LEX_CDC };

        _seriem_probare(piscina, "-webkit-x --varia <!-- -->",
            exspectata, 7);
    }
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_APUD_VERBUM, CSS_LEX_SPATIA,
            CSS_LEX_PAREN_APERTA, CSS_LEX_IDENTIFICATOR,
            CSS_LEX_COLON, CSS_LEX_DIMENSIO,
            CSS_LEX_PAREN_CLAUSA };

        _seriem_probare(piscina, "@media (min-width:10em)",
            exspectata, 7);
    }

    imprimere("\n--- Url: quattuor viae ---\n");
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_URL };

        _seriem_probare(piscina, "url(imago.png)", exspectata, 1);
    }
    {
        /* citata -> functio + chorda (spec) */
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_FUNCTIO, CSS_LEX_SPATIA, CSS_LEX_CHORDA,
            CSS_LEX_SPATIA, CSS_LEX_PAREN_CLAUSA };

        _seriem_probare(piscina, "url( 'x.png' )", exspectata, 5);
    }
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_URL_IMPERFECTA };

        _seriem_probare(piscina, "url(mala url)", exspectata, 1);
        _seriem_probare(piscina, "url(aperta", exspectata, 1);
    }

    imprimere("\n--- Chordae: perfectae et imperfectae ---\n");
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_CHORDA };

        _seriem_probare(piscina, "\"salve\"", exspectata, 1);
        _seriem_probare(piscina, "\"a\\\"b\"", exspectata, 1);
        /* continuatio: '\' + linea nova intra chordam */
        _seriem_probare(piscina, "'a\\\nb'", exspectata, 1);
    }
    {
        /* linea nova cruda: imperfecta, linea NON consumpta */
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_CHORDA_IMPERFECTA, CSS_LEX_SPATIA,
            CSS_LEX_IDENTIFICATOR, CSS_LEX_CHORDA_IMPERFECTA };

        _seriem_probare(piscina, "'a\nb'", exspectata, 4);
    }

    imprimere("\n--- Commentaria et effugia ---\n");
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_COMMENTARIUM, CSS_LEX_IDENTIFICATOR };

        _seriem_probare(piscina, "/*x*/y", exspectata, 2);
    }
    {
        interior constans CssLexemaGenus exspectata[] = {
            CSS_LEX_COMMENTARIUM_IMPERFECTUM };

        _seriem_probare(piscina, "/* apertum", exspectata, 1);
    }
    {
        /* effugium hex + spatium PARS identificatoris:
         * '\41 x' identificator unus quinque octetorum */
        Xar* l = _lexare_literis(piscina, "\\41 x");

        CREDO_NON_NIHIL (l);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), (i32)2);
        {
            CssLexema* lx = (CssLexema*)xar_obtinere(l, 0);

            CREDO_AEQUALIS_I32 ((i32)lx->genus,
                (i32)CSS_LEX_IDENTIFICATOR);
            CREDO_AEQUALIS_I32 ((i32)lx->valor.mensura, (i32)5);
        }
    }

    imprimere("\n--- Totalitas: NUL et octeti ignoti ---\n");
    {
        interior constans character truncus[] = { '\0', 1, 'a' };

        CREDO_VERUM (_fidelis(piscina, truncus, (i32)3));
        {
            Xar* l = css_lexare(piscina, truncus, (i32)3);

            CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), (i32)4);
            CREDO_AEQUALIS_I32 (
                (i32)((CssLexema*)xar_obtinere(l, 0))->genus,
                (i32)CSS_LEX_DELIM);
        }
        CREDO_VERUM (_fidelis(piscina, "", (i32)0));
    }

    imprimere("\n--- Positiones (octeti, 1-basatae) ---\n");
    {
        Xar*       l = _lexare_literis(piscina, "a\n  bc");
        CssLexema* lx;

        CREDO_NON_NIHIL (l);
        lx = (CssLexema*)xar_obtinere(l, 2);
        CREDO_AEQUALIS_I32 ((i32)lx->genus,
            (i32)CSS_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 (lx->offset, (i32)4);
        CREDO_AEQUALIS_I32 (lx->linea, (i32)2);
        CREDO_AEQUALIS_I32 (lx->columna, (i32)3);
    }

    imprimere("\n--- Fidelitas super corpus (fixa/css) ---\n");
    {
        DirectoriumIterator*  iter;
        DirectoriumIntroitus* e;
        i32                   probatae = 0;

        iter = directorium_iterator_aperire("probationes/fixa/css",
            piscina);
        CREDO_NON_NIHIL (iter);
        si (iter != NIHIL)
        {
            dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
            {
                ChordaAedificator* aed;
                chorda             via;
                chorda             contentum;
                b32                adversaria;

                si (e->genus != INTROITUS_FILUM)
                {
                    perge;
                }
                aed = chorda_aedificator_creare(piscina,
                    (memoriae_index)128);
                chorda_aedificator_appendere_literis(aed,
                    "probationes/fixa/css/");
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
        CREDO_VERUM (probatae >= (i32)3);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        redde ZEPHYRUM;
    }
    redde I;
}
