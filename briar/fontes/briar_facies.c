/* briar_facies.c - Pagina litterata (spec par. 4.6).
 *
 * Ordo emissionis: caput (provenientia), corpus (partes ordine
 * documenti), pes (inventarium partium), insula symbolorum. Involucrum
 * ultimum: notae substitutae, quaeque semel.
 *
 * Provenientia sine HOROLOGIO de industria: aurum octetorum tempus
 * ferre non potest. Clavis fabricae contentum nominat, quod tempus
 * numquam faceret.
 */

#include "briar_facies.h"

#include "briar_arbor.h"
#include "briar_nexus.h"
#include "briar_registrum.h"
#include "chorda_aedificator.h"
#include "md_html.h"
#include "silex.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Evasio et lineae
 * ================================================== */

interior vacuum
_evadere (
    ChordaAedificator* a,
               chorda  t)
{
    i32 k;

    per (k = ZEPHYRUM; k < t.mensura; k++)
    {
        character c = (character)t.datum[k];

        si (c == '&')
        {
            chorda_aedificator_appendere_literis(a, "&amp;");
        }
        alioquin si (c == '<')
        {
            chorda_aedificator_appendere_literis(a, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(a, "&gt;");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(a, "&quot;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, c);
        }
    }
}

/* pars prosae inter regiones saepe spatium solum est: sectio vacua
 * emitteretur. Fons albus solus = nihil; vitium md tacetur numquam. */
interior b32
_album_solum (
    chorda t)
{
    i32 k;

    per (k = ZEPHYRUM; k < t.mensura; k++)
    {
        character c = (character)t.datum[k];

        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* numerus lineae k-mae: tabula si adest, alioquin linea_prima + k */
interior i32
_linea_indicis (
    constans Xar* tabula,
             i32  linea_prima,
             i32  k)
{
    si (tabula != NIHIL && k < xar_numerus(tabula))
    {
        redde *(constans i32*)xar_obtinere(tabula, k);
    }
    redde linea_prima + k;
}

/* lineas textus emittere: una quaeque cum id sua .thistle linea et
 * data-n (numerus in CSS per ::before - textus purus manet, ergo
 * quod lector copiat codex est, non numeri) */
interior vacuum
_lineas_emittere (
      ChordaAedificator* a,
                 chorda  textus,
           constans Xar* tabula,
                    i32  linea_prima,
     constans character* praefixum_id)
{
    i32 cursor  = ZEPHYRUM;
    i32 k       = ZEPHYRUM;

    dum (cursor < textus.mensura)
    {
           i32 finis = cursor;
           i32 linea;
        chorda pars;

        dum (finis < textus.mensura && textus.datum[finis] != (i8)'\n')
        {
            finis = finis + I;
        }
        linea         = _linea_indicis(tabula, linea_prima, k);
        pars.datum    = textus.datum + cursor;
        pars.mensura  = finis - cursor;
        chorda_aedificator_appendere_literis(a,
            "<span class=\"fr-linea\" id=\"");
        chorda_aedificator_appendere_literis(a, praefixum_id);
        chorda_aedificator_appendere_i32(a, (i32)linea);
        chorda_aedificator_appendere_literis(a, "\" data-n=\"");
        chorda_aedificator_appendere_i32(a, (i32)linea);
        chorda_aedificator_appendere_literis(a, "\">");
        _evadere(a, pars);
        chorda_aedificator_appendere_literis(a, "</span>\n");
        cursor  = (finis < textus.mensura) ? finis + I : finis;
        k       = k + I;
    }
}


/* ==================================================
 * Partes
 * ================================================== */

interior vacuum
_prosam_emittere (
                Piscina* piscina,
    InternamentumChorda* intern,
      ChordaAedificator* a,
 constans BriarNexusRes* r,
                 chorda  exemplar)
{
            chorda octeti;
    MdHtmlResultus m;

    si (   r->pars        != NIHIL
        && r->pars->genus == (s32)BRIAR_GENUS_PROSA)
    {
        MateriaToken* t = briar_lexema(r->pars, (i32)BRIAR_PROSA_TOK);

        octeti = (t != NIHIL) ? t->valor : r->contentum;
    }
    alioquin
    {
        octeti = r->contentum;
    }
    si (_album_solum(octeti))
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a,
        "<section class=\"fr-prosa\" id=\"pars-");
    chorda_aedificator_appendere_i32(a, (i32)r->linea_initium);
    chorda_aedificator_appendere_literis(a, "\">\n");
    si (octeti.mensura > ZEPHYRUM)
    {
        m = md_html_reddere(piscina, intern,
            (constans character*)octeti.datum, octeti.mensura,
            exemplar);
        si (m.successus)
        {
            chorda_aedificator_appendere_chorda(a, m.html);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a,
                "<p class=\"fr-vitium\">prosa non reddita: ");
            _evadere(a, m.causa);
            chorda_aedificator_appendere_literis(a, "</p>\n");
        }
    }
    chorda_aedificator_appendere_literis(a, "</section>\n");
}

interior vacuum
_regionem_emittere (
      ChordaAedificator* a,
 constans BriarNexusRes* r)
{
    chorda_aedificator_appendere_literis(a,
        "<section class=\"fr-regio\" id=\"regio-");
    chorda_aedificator_appendere_i32(a, (i32)r->linea_initium);
    chorda_aedificator_appendere_literis(a, "\">\n<pre class=\"fr-c\">"
        "<code>");
    _lineas_emittere(a, r->contentum, NIHIL, r->linea_initium, "l");
    chorda_aedificator_appendere_literis(a,
        "</code></pre>\n</section>\n");
}

interior vacuum
_elementum_emittere (
      ChordaAedificator* a,
 constans BriarNexusRes* r)
{
    MateriaToken* t = (r->pars != NIHIL)
        ? briar_lexema(r->pars, (i32)BRIAR_ELEMENTUM_TOK) : NIHIL;

    chorda_aedificator_appendere_literis(a,
        "<section class=\"fr-elementum\" id=\"regio-");
    chorda_aedificator_appendere_i32(a, (i32)r->linea_initium);
    chorda_aedificator_appendere_literis(a,
        "\">\n<pre class=\"fr-stml\">"
        "<code>");
    si (t != NIHIL)
    {
        _lineas_emittere(a, t->valor, NIHIL, r->linea_initium, "l");
    }
    chorda_aedificator_appendere_literis(a,
        "</code></pre>\n</section>\n");
}


/* ==================================================
 * Caput et pes
 * ================================================== */

interior chorda
_caput_fingere (
                         Piscina* piscina,
              constans character* via,
    constans BriarFabricaFructus* fructus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "<header class=\"fr-caput\">\n<h1>");
    _evadere(a, chorda_ex_literis(fructus->titulus != NIHIL
        ? fructus->titulus : "thistle", piscina));
    chorda_aedificator_appendere_literis(a,
        "</h1>\n<dl class=\"fr-provenientia\">\n<dt>fons</dt><dd>");
    _evadere(a, chorda_ex_literis(via, piscina));
    chorda_aedificator_appendere_literis(a,
        "</dd>\n<dt>clavis</dt><dd>");
    _evadere(a, chorda_ex_literis(fructus->sigillum, piscina));
    chorda_aedificator_appendere_literis(a,
        "</dd>\n</dl>\n</header>\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_pedem_fingere (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);
    i32 i;

    chorda_aedificator_appendere_literis(a,
        "<footer class=\"fr-pes\">\n<h2>partes</h2>\n<ul"
        " class=\"fr-clausura\">\n");
    si (fructus->clausura != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(fructus->clausura); i++)
        {
            constans SilexRes* s = (constans SilexRes*)xar_obtinere(
                fructus->clausura, i);

            chorda_aedificator_appendere_literis(a, "<li>");
            _evadere(a, s->via);
            chorda_aedificator_appendere_literis(a, "</li>\n");
        }
    }
    chorda_aedificator_appendere_literis(a, "</ul>\n</footer>\n");
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Involucrum: notae substitutae
 * ================================================== */

nomen structura {
     constans character* titulus;
                 chorda  valor;
                    b32  visa;
} BriarNota;

interior chorda
_involucrum_implere (
              Piscina* piscina,
               chorda  involucrum,
            BriarNota* notae,
                  i32  numerus,
               chorda* causa)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)65536);
       i32 cursor = ZEPHYRUM;
       i32 i;
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    dum (cursor < involucrum.mensura)
    {
        i32 initium = cursor;
        i32 finis;
        b32 inventa = FALSUM;

        dum (   initium + V <= involucrum.mensura
             && memcmp(involucrum.datum + initium, "<!--@", V)
                != ZEPHYRUM)
        {
            initium = initium + I;
        }
        si (initium + V > involucrum.mensura)
        {
            chorda pars;

            pars.datum    = involucrum.datum + cursor;
            pars.mensura  = involucrum.mensura - cursor;
            chorda_aedificator_appendere_chorda(a, pars);
            frange;
        }
        {
            chorda ante;

            ante.datum    = involucrum.datum + cursor;
            ante.mensura  = initium - cursor;
            chorda_aedificator_appendere_chorda(a, ante);
        }
        finis = initium + V;
        dum (   finis + III <= involucrum.mensura
             && memcmp(involucrum.datum + finis, "-->", III)
                != ZEPHYRUM)
        {
            finis = finis + I;
        }
        si (finis + III > involucrum.mensura)
        {
            *causa = chorda_ex_literis(
                "involucrum: nota non clausa", piscina);
            redde vacua;
        }
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            chorda titulus;

            titulus.datum    = involucrum.datum + initium + V;
            titulus.mensura  = finis - initium - V;
            si (chorda_aequalis_literis(titulus, notae[i].titulus))
            {
                chorda_aedificator_appendere_chorda(a, notae[i].valor);
                notae[i].visa  = VERUM;
                inventa        = VERUM;
                frange;
            }
        }
        si (!inventa)
        {
            character b[128];
               chorda titulus;

            titulus.datum    = involucrum.datum + initium + V;
            titulus.mensura  = finis - initium - V;
            sprintf(b, "involucrum: nota ignota '%.*s'",
                (integer)titulus.mensura,
                (constans character*)titulus.datum);
            *causa = chorda_ex_literis(b, piscina);
            redde vacua;
        }
        cursor = finis + III;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!notae[i].visa)
        {
            character b[128];

            sprintf(b, "involucrum: nota '%s' abest", notae[i].titulus);
            *causa = chorda_ex_literis(b, piscina);
            redde vacua;
        }
    }
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Principale moduli
 * ================================================== */

chorda
briar_faciem_fingere (
                          Piscina* piscina,
              InternamentumChorda* intern,
                              Xar* nexus,
                              Xar* fragmenta,
     constans BriarFabricaFructus* fructus,
                           chorda  fons,
               constans character* via,
             constans BriarVestis* vestis,
                           chorda* causa)
{
     ChordaAedificator* corpus;
             BriarNota  notae[6];
                   i32  i;
                chorda  vacua;

    vacua.datum     = NIHIL;
    vacua.mensura   = ZEPHYRUM;
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    (vacuum)fragmenta;
    (vacuum)fons;
    corpus = chorda_aedificator_creare(piscina,
        (memoriae_index)65536);
    chorda_aedificator_appendere_literis(corpus,
        "<main class=\"fr-documentum\">\n");
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);

        si (r->genus == BRIAR_NEXUS_MD)
        {
            _prosam_emittere(piscina, intern, corpus, r,
                vestis->exemplar);
        }
        alioquin si (r->genus == BRIAR_NEXUS_STML)
        {
            _elementum_emittere(corpus, r);
        }
        alioquin
        {
            _regionem_emittere(corpus, r);
        }
    }
    chorda_aedificator_appendere_literis(corpus, "</main>\n");
    chorda_aedificator_appendere_chorda(corpus,
        _pedem_fingere(piscina, fructus));

    notae[0].titulus = "titulus";
    notae[0].valor   = chorda_ex_literis(fructus->titulus != NIHIL
        ? fructus->titulus : "thistle", piscina);
    notae[1].titulus  = "styli";
    notae[1].valor    = vestis->styli;
    notae[2].titulus  = "caput";
    notae[2].valor    = _caput_fingere(piscina, via, fructus);
    notae[3].titulus  = "corpus";
    notae[3].valor    = chorda_aedificator_finire(corpus);
    notae[4].titulus  = "symbola";
    notae[4].valor    = vacua;
    notae[5].titulus  = "scriptum";
    notae[5].valor    = vestis->scriptum;
    per (i = ZEPHYRUM; i < (i32)6; i++)
    {
        notae[i].visa = FALSUM;
    }
    redde _involucrum_implere(piscina, vestis->involucrum, notae,
        (i32)6, causa);
}
