/* probatio_html_lexema.c - lexator HTML totalis.
 *
 * Duo genera oraculorum:
 *  (1) series generum exspectata pro fontibus parvis (vocabularium,
 *      IMPERFECTA omnia, margines '<' et '&', decipulae notae HTML:
 *      'href=x/>', '</scripty', '<script/>');
 *  (2) FIDELITAS SUPER CORPUS: quaeque fixtura in
 *      probationes/fixa/html/ per lexare->emittere octetim redit;
 *      fixturae mundae (photographemata plagularum verarum domus)
 *      praeterea genera IMPERFECTA et PRAVA NULLA ferant oportet. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "html_lexema.h"
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
    redde html_lexare(piscina, fons, (i32)strlen(fons));
}

/* seriem generum probare (FINIS exclusa) */
interior vacuum
_seriem_probare (Piscina* piscina, constans character* fons,
    constans HtmlLexemaGenus* exspectata, i32 numerus);

interior vacuum
_seriem_probare (Piscina* piscina, constans character* fons,
    constans HtmlLexemaGenus* exspectata, i32 numerus)
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
        HtmlLexema* lx = (HtmlLexema*)xar_obtinere(l, k);

        si (lx->genus != exspectata[k])
        {
            imprimere("  [%d] '%.*s': %s pro %s exspectato\n",
                (integer)k, (integer)lx->valor.mensura,
                (constans character*)lx->valor.datum,
                html_lexema_genus_nomen(lx->genus),
                html_lexema_genus_nomen(exspectata[k]));
        }
        CREDO_AEQUALIS_I32 ((i32)lx->genus, (i32)exspectata[k]);
    }
}

/* valorem lexematis k probare (extensio verbatim) */
interior vacuum
_valorem_probare (Piscina* piscina, constans character* fons,
    i32 index, constans character* exspectatus);

interior vacuum
_valorem_probare (Piscina* piscina, constans character* fons,
    i32 index, constans character* exspectatus)
{
    Xar*        l = _lexare_literis(piscina, fons);
    HtmlLexema* lx;
    i32         m = (i32)strlen(exspectatus);

    CREDO_NON_NIHIL (l);
    si (l == NIHIL || index >= xar_numerus(l))
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    lx = (HtmlLexema*)xar_obtinere(l, index);
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
_fidelis (Piscina* piscina, constans character* fons, i32 mensura);

interior b32
_fidelis (Piscina* piscina, constans character* fons, i32 mensura)
{
    Xar*   l = html_lexare(piscina, fons, mensura);
    chorda e;
    i32    k;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        HtmlLexema* lx = (HtmlLexema*)xar_obtinere(l, k);

        si (lx->genus != HTML_LEX_FINIS && lx->valor.mensura == 0)
        {
            redde FALSUM;   /* progressus violatus */
        }
    }
    e = html_lexemata_emittere(piscina, l);
    si (e.mensura != mensura)
    {
        redde FALSUM;
    }
    redde mensura == 0
        || memcmp(e.datum, fons, (memoriae_index)mensura) == 0;
}

/* numerus generum imperfectorum ET pravorum in fonte (plagulae
 * mundae utraque nulla ferant) */
interior i32
_imperfecta_numerare (Piscina* piscina, constans character* fons,
    i32 mensura);

interior i32
_imperfecta_numerare (Piscina* piscina, constans character* fons,
    i32 mensura)
{
    Xar* l = html_lexare(piscina, fons, mensura);
    i32  k;
    i32  n = 0;

    si (l == NIHIL)
    {
        redde (i32)9999;
    }
    per (k = 0; k < xar_numerus(l); k = k + 1)
    {
        HtmlLexema* lx = (HtmlLexema*)xar_obtinere(l, k);

        si (lx->genus == HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS
            || lx->genus == HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS
            || lx->genus == HTML_LEX_COMMENTARIUM_IMPERFECTUM
            || lx->genus == HTML_LEX_COMMENTARIUM_PRAVUM
            || lx->genus == HTML_LEX_DOCTYPE_IMPERFECTUM
            || lx->genus == HTML_LEX_CDATA_IMPERFECTA)
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
        "probatio_html_lexema", 33554432);

    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Vocabularium fundamentale ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_TAG_FINIS,
            HTML_LEX_TEXTUS, HTML_LEX_TAG_CLAUSURA,
            HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<p class=\"x\">salve</p>",
            exspectata, 9);
    }
    {
        /* valor nudus, citatus simplex, attributum sine valore */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<a href=/x y='z' hidden>",
            exspectata, 12);
    }
    {
        /* valor citatus cum '>' intus - solum citatio claudit */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<b t=\"a > b\">", exspectata, 6);
    }

    imprimere("\n--- Se-claudentia et decipulae '/' ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS_SOLUS };

        _seriem_probare(piscina, "<br/>", exspectata, 2);
    }
    {
        /* decipula: '<a href=x/>' valorem 'x/' dat, finis normalis */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<a href=x/>", exspectata, 6);
        _valorem_probare(piscina, "<a href=x/>", (i32)4, "x/");
        /* '<a href=/>' valorem '/' dat */
        _seriem_probare(piscina, "<a href=/>", exspectata, 6);
        _valorem_probare(piscina, "<a href=/>", (i32)4, "/");
    }
    {
        /* '/' solivagus intra tag: delim */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA, HTML_LEX_DELIM,
            HTML_LEX_SPATIA, HTML_LEX_ATTRIBUTUM_NOMEN,
            HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<a / b>", exspectata, 6);
    }

    imprimere("\n--- Referentiae characterum ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TEXTUS, HTML_LEX_REFERENTIA, HTML_LEX_TEXTUS,
            HTML_LEX_REFERENTIA, HTML_LEX_TEXTUS,
            HTML_LEX_REFERENTIA, HTML_LEX_TEXTUS };

        _seriem_probare(piscina, "a &amp; b &#65; &#x41; &nope c",
            exspectata, 7);
    }

    imprimere("\n--- Commentaria ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_COMMENTARIUM, HTML_LEX_TEXTUS };

        _seriem_probare(piscina, "<!--x-->y", exspectata, 2);
        _seriem_probare(piscina, "<!--a--!>b", exspectata, 2);
    }
    {
        /* abrupta clausa (spec, cum errore parsationis) */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_COMMENTARIUM };

        _seriem_probare(piscina, "<!-->", exspectata, 1);
        _seriem_probare(piscina, "<!--->", exspectata, 1);
        _seriem_probare(piscina, "<!--a - b-->", exspectata, 1);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_COMMENTARIUM_IMPERFECTUM };

        _seriem_probare(piscina, "<!-- apertum", exspectata, 1);
    }

    imprimere("\n--- Doctype, pravum, CDATA ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_DOCTYPE, HTML_LEX_TAG_APERTURA,
            HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<!DOCTYPE html><p>",
            exspectata, 3);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_DOCTYPE_IMPERFECTUM };

        _seriem_probare(piscina, "<!doctype apertum",
            exspectata, 1);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_COMMENTARIUM_PRAVUM, HTML_LEX_TEXTUS };

        _seriem_probare(piscina, "<?php x ?>a", exspectata, 2);
        _seriem_probare(piscina, "</3>a", exspectata, 2);
        _seriem_probare(piscina, "<!x>a", exspectata, 2);
        _seriem_probare(piscina, "</>a", exspectata, 2);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_CDATA };

        _seriem_probare(piscina, "<![CDATA[a>b]]>", exspectata, 1);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_CDATA_IMPERFECTA };

        _seriem_probare(piscina, "<![CDATA[apertum", exspectata, 1);
    }

    imprimere("\n--- Textus crudus (script/style/title/textarea) ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS,
            HTML_LEX_TEXTUS_CRUDUS, HTML_LEX_TAG_CLAUSURA,
            HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<script>if(a<b){}</script>",
            exspectata, 5);
        _valorem_probare(piscina, "<script>if(a<b){}</script>",
            (i32)2, "if(a<b){}");
        /* '</scripty' NON claudit (terminus tituli deest) */
        _seriem_probare(piscina,
            "<script>a\"</scripty\"b</script>", exspectata, 5);
        _seriem_probare(piscina, "<style>a{}</style>",
            exspectata, 5);
        /* RCDATA: referentiae NON secantur - crudus totus */
        _seriem_probare(piscina, "<title>x &amp; <y</title>",
            exspectata, 5);
        _valorem_probare(piscina, "<title>x &amp; <y</title>",
            (i32)2, "x &amp; <y");
        /* clausura aliena textareae textus manet */
        _seriem_probare(piscina,
            "<textarea></span></textarea>", exspectata, 5);
        _valorem_probare(piscina,
            "<textarea></span></textarea>", (i32)2, "</span>");
    }
    {
        /* casu neglecto, spatium ante '>' clausurae licitum */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS,
            HTML_LEX_TEXTUS_CRUDUS, HTML_LEX_TAG_CLAUSURA,
            HTML_LEX_SPATIA, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<SCRIPT>x</ScRiPt >",
            exspectata, 6);
    }
    {
        /* '<script/>' modum crudum intrat (mos navigatorum) */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS_SOLUS,
            HTML_LEX_TEXTUS_CRUDUS, HTML_LEX_TAG_CLAUSURA,
            HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<script/>x</script>",
            exspectata, 5);
    }
    {
        /* crudus vacuus lexema nullum dat (progressus) */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS,
            HTML_LEX_TAG_CLAUSURA, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<script></script>",
            exspectata, 4);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_TAG_FINIS,
            HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS };

        _seriem_probare(piscina, "<script>apertum",
            exspectata, 3);
        /* '</script' + EOF clausura non est - crudus manet */
        _seriem_probare(piscina, "<script>x</script",
            exspectata, 3);
    }

    imprimere("\n--- Margines '<' et truncationes ---\n");
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TEXTUS };

        _seriem_probare(piscina, "a < b", exspectata, 1);
        _seriem_probare(piscina, "a <3 b", exspectata, 1);
        _seriem_probare(piscina, "<", exspectata, 1);
        _seriem_probare(piscina, "x</", exspectata, 1);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA };

        _seriem_probare(piscina, "<div", exspectata, 1);
    }
    {
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS };

        _seriem_probare(piscina, "<a href=\"x", exspectata, 5);
    }
    {
        /* linea nova intra valorem citatum LICITA (non CSS) */
        interior constans HtmlLexemaGenus exspectata[] = {
            HTML_LEX_TAG_APERTURA, HTML_LEX_SPATIA,
            HTML_LEX_ATTRIBUTUM_NOMEN, HTML_LEX_AEQUALE,
            HTML_LEX_ATTRIBUTUM_VALOR, HTML_LEX_TAG_FINIS };

        _seriem_probare(piscina, "<a t=\"x\ny\">", exspectata, 6);
    }

    imprimere("\n--- Totalitas: NUL et fidelitas marginum ---\n");
    {
        interior constans character truncus[] = { 'a', '\0', 'b' };

        CREDO_VERUM (_fidelis(piscina, truncus, (i32)3));
        {
            Xar* l = html_lexare(piscina, truncus, (i32)3);

            CREDO_AEQUALIS_I32 ((i32)xar_numerus(l), (i32)2);
            CREDO_AEQUALIS_I32 (
                (i32)((HtmlLexema*)xar_obtinere(l, 0))->genus,
                (i32)HTML_LEX_TEXTUS);
        }
        CREDO_VERUM (_fidelis(piscina, "", (i32)0));
    }
    {
        interior constans character* margines[] = {
            "<a href=\"ap", "<script>ap", "<!-- ap",
            "<!doctype ap", "<![CDATA[ap", "</", "<",
            "<a t='x\ny'>z</a>", "<script>x</script" };
        i32 k;

        per (k = 0; k < (i32)9; k = k + 1)
        {
            CREDO_VERUM (_fidelis(piscina, margines[k],
                (i32)strlen(margines[k])));
        }
    }

    imprimere("\n--- Positiones (octeti, 1-basatae) ---\n");
    {
        Xar*        l = _lexare_literis(piscina, "<p>\n<em x=1>");
        HtmlLexema* lx;

        CREDO_NON_NIHIL (l);
        /* [0]='<p' [1]='>' [2]='\n' [3]='<em' ... */
        lx = (HtmlLexema*)xar_obtinere(l, 3);
        CREDO_AEQUALIS_I32 ((i32)lx->genus,
            (i32)HTML_LEX_TAG_APERTURA);
        CREDO_AEQUALIS_I32 (lx->offset, (i32)4);
        CREDO_AEQUALIS_I32 (lx->linea, (i32)2);
        CREDO_AEQUALIS_I32 (lx->columna, (i32)1);
        lx = (HtmlLexema*)xar_obtinere(l, 5);
        CREDO_AEQUALIS_I32 ((i32)lx->genus,
            (i32)HTML_LEX_ATTRIBUTUM_NOMEN);
        CREDO_AEQUALIS_I32 (lx->offset, (i32)8);
        CREDO_AEQUALIS_I32 (lx->linea, (i32)2);
        CREDO_AEQUALIS_I32 (lx->columna, (i32)5);
    }

    imprimere("\n--- Fidelitas super corpus (fixa/html) ---\n");
    {
        DirectoriumIterator*  iter;
        DirectoriumIntroitus* e;
        i32                   probatae = 0;

        iter = directorium_iterator_aperire("probationes/fixa/html",
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
                    "probationes/fixa/html/");
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
