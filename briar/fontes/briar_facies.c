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
#include "briar_contextus.h"
#include "md_html.h"
#include "silex.h"
#include "silva.h"
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


/* ==================================================
 * Color: consilium ex experimento laboratorii 0023
 *
 * 'lexare mechanica, colorare consilium': lexator silvae verus
 * genera lexematum dat, tabulae infra Latina eligunt. Verba clausa
 * ANGLICA lexator ipse novit; Latina (si redde per) per latina.h
 * expanduntur, ergo lexatori identificatores sunt et HIC clavis
 * fiunt. Via TextMate (grammaticae regularium) consulto omissa:
 * lexatorem verum habemus.
 *
 * Venditum e silva: si genera lexematum crescunt, tabula relegenda.
 * ================================================== */

#define CL_COMMENTARIUM  "ca-sy-commentarium"
#define CL_CHORDA        "ca-sy-chorda"
#define CL_NUMERUS       "ca-sy-numerus"
#define CL_CLAVIS        "ca-sy-clavis"
#define CL_GENUS         "ca-sy-genus"
/* classis CSS ex experimento 0023 servatur (pinacotheca
 * eadem); identificator domus DIRECTIVA est - id quod re vera
 * coloratur linea directivae est, non praeprocessor ipse */
#define CL_DIRECTIVA     "ca-sy-praeprocessor"

interior constans character* constans _claves_latinae[] = {
    "si", "alioquin", "commutatio", "casus", "ordinarius",
    "per", "dum", "fac", "frange", "perge", "salta", "redde",
    "structura", "unio", "enumeratio", "nomen", "magnitudo",
    "constans", "volatilis", "sponte", "registrum", "staticus",
    "externus", "interior", "hic_manens", "universalis",
    "principale", "signatus", "insignatus", NIHIL };

interior constans character* constans _genera_nota[] = {
    "vacuum", "character", "brevis", "integer", "longus",
    "fluitans", "duplex", "i8", "i16", "i32", "i64",
    "s8", "s16", "s32", "s64", "b32", "f32", "f64",
    "memoriae_index", "chorda", NIHIL };

interior constans character* constans _constantia_nota[] = {
    "NIHIL", "VERUM", "FALSUM", "ZEPHYRUM", NIHIL };

nomen structura {
                    i32  ab;   /* columna octetorum in linea */
                    i32  ad;
     constans character* cl;
} BriarTectum;

interior b32
_in_tabula (
                 SilvaChorda  valor,
    constans character* constans* tabula)
{
    i32 k;

    per (k = ZEPHYRUM; tabula[k] != NIHIL; k++)
    {
        size_t mensura = strlen(tabula[k]);

        si (   (size_t)valor.mensura                   == mensura
            && memcmp(valor.datum, tabula[k], mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* classis lexematis; NIHIL = sine colore (interpunctio, trivia) */
interior constans character*
_classis (
    constans SilvaToken* t)
{
    si (   t->genus == SILVA_LEX_COMMENTUM_CLAUSUM
        || t->genus == SILVA_LEX_COMMENTUM_LINEA)
    {
        redde CL_COMMENTARIUM;
    }
    si (   t->genus == SILVA_LEX_STRING_LIT
        || t->genus == SILVA_LEX_STRING_IMPERFECTUM
        || t->genus == SILVA_LEX_CHARACTER_LIT
        || t->genus == SILVA_LEX_CHARACTER_IMPERFECTUM)
    {
        redde CL_CHORDA;
    }
    si (t->genus == SILVA_LEX_INTEGER || t->genus == SILVA_LEX_FLOAT)
    {
        redde CL_NUMERUS;
    }
    si (t->genus >= SILVA_LEX_AUTO && t->genus <= SILVA_LEX_WHILE)
    {
        redde CL_CLAVIS;
    }
    si (t->genus == SILVA_LEX_IDENTIFICATOR)
    {
        si (_in_tabula(t->valor, _claves_latinae))
        {
            redde CL_CLAVIS;
        }
        si (_in_tabula(t->valor, _genera_nota))
        {
            redde CL_GENUS;
        }
        si (_in_tabula(t->valor, _constantia_nota))
        {
            redde CL_NUMERUS;
        }
    }
    redde NIHIL;
}

interior vacuum
_tectum_addere (
                   Xar* tecta_lineae,
                   i32  ab,
                   i32  ad,
    constans character* cl)
{
    BriarTectum* t;

    si (ad <= ab || tecta_lineae == NIHIL)
    {
        redde;
    }
    t = (BriarTectum*)xar_addere(tecta_lineae);
    si (t == NIHIL)
    {
        redde;
    }
    t->ab = ab;
    t->ad = ad;
    t->cl = cl;
}

/* extensionem [off, fin) octetorum trans lineas SECARE: commentarium
 * bilineum tectum in utraque linea habet, numquam demittitur */
interior vacuum
_extensionem_addere (
                   Xar* tecta,
          constans i32* initia,
                   i32  numerus_linearum,
                   i32  linea,
                   i32  off,
                   i32  fin,
    constans character* cl)
{
    i32 cursor = off;

    dum (cursor < fin && linea <= numerus_linearum)
    {
        i32 finis_contenti = initia[linea] - I;

        si (finis_contenti > fin)
        {
            finis_contenti = fin;
        }
        _tectum_addere(*(Xar**)xar_obtinere(tecta, linea - I),
            cursor - initia[linea - I],
            finis_contenti - initia[linea - I], cl);
        cursor  = initia[linea];
        linea   = linea + I;
    }
}

/* tecta per lineam: Xar de (Xar* de BriarTectum) */
interior Xar*
_tecta_computare (
          Piscina* piscina,
     SilvaPiscina* silva_piscina,
           chorda  textus)
{
          Xar* tecta;
          Xar* cruda;
          i32* initia;
          i32  numerus_linearum = I;
          i32  k;
          i32  n;
          i32  linea_directivi = ZEPHYRUM;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == (i8)'\n')
        {
            numerus_linearum = numerus_linearum + I;
        }
    }
    initia = (i32*)piscina_allocare(piscina,
        (memoriae_index)((numerus_linearum + I) * (i32)magnitudo(i32)));
    si (initia == NIHIL)
    {
        redde NIHIL;
    }
    initia[0]  = ZEPHYRUM;
    n          = I;
    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == (i8)'\n')
        {
            initia[n]  = k + I;
            n          = n + I;
        }
    }
    initia[numerus_linearum] = textus.mensura + I;

    tecta = xar_creare(piscina, magnitudo(Xar*));
    si (tecta == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus_linearum; k++)
    {
        *(Xar**)xar_addere(tecta) = xar_creare(piscina,
            magnitudo(BriarTectum));
    }
    cruda = (Xar*)silva_lexare_cruda(silva_piscina,
        (constans character*)textus.datum,
        (insignatus integer)textus.mensura, ZEPHYRUM);
    si (cruda == NIHIL)
    {
        redde tecta;
    }
    per (k = ZEPHYRUM;
        k < (i32)silva_xar_numerus((SilvaXar*)cruda); k++)
    {
        constans SilvaToken* t = *(constans SilvaToken**)
            silva_xar_obtinere((SilvaXar*)cruda,
            (insignatus integer)k);
        constans character* cl;

        si (   t == NIHIL || t->genus == SILVA_LEX_EOF
            || t->byte_offset < 0)
        {
            perge;
        }
        /* CANCELLUM initio lineae: linea TOTA praeprocessor,
         * cetera lexemata eius lineae tacent */
        si (t->genus == SILVA_LEX_CANCELLUM && t->initium_lineae)
        {
            linea_directivi = (i32)t->linea;
            _tectum_addere(*(Xar**)xar_obtinere(tecta,
                (i32)t->linea - I),
                (i32)t->byte_offset - initia[t->linea - I],
                initia[t->linea] - I - initia[t->linea - I],
                CL_DIRECTIVA);
            perge;
        }
        si (   linea_directivi != ZEPHYRUM
            && (i32)t->linea   == linea_directivi)
        {
            perge;
        }
        cl = _classis(t);
        si (cl == NIHIL)
        {
            perge;
        }
        _extensionem_addere(tecta, initia, numerus_linearum,
            (i32)t->linea, (i32)t->byte_offset,
            (i32)t->byte_offset + (i32)t->longitudo, cl);
    }
    redde tecta;
}

interior vacuum
_lineam_coloratam_emittere (
    ChordaAedificator* a,
               chorda  linea,
         constans Xar* tecta_lineae)
{
    i32 cursor = ZEPHYRUM;
    i32 i;

    si (tecta_lineae == NIHIL)
    {
        _evadere(a, linea);
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(tecta_lineae); i++)
    {
        constans BriarTectum* t = (constans BriarTectum*)xar_obtinere(
            tecta_lineae, i);
        chorda pars;

        si (t->ab > cursor)
        {
            pars.datum    = linea.datum + cursor;
            pars.mensura  = t->ab - cursor;
            _evadere(a, pars);
        }
        pars.datum    = linea.datum + t->ab;
        pars.mensura  = ((t->ad
            < linea.mensura) ? t->ad : linea.mensura)
            - t->ab;
        si (pars.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, "<span class=\"");
            chorda_aedificator_appendere_literis(a, t->cl);
            chorda_aedificator_appendere_literis(a, "\">");
            _evadere(a, pars);
            chorda_aedificator_appendere_literis(a, "</span>");
        }
        cursor = t->ad;
    }
    si (cursor < linea.mensura)
    {
        chorda pars;

        pars.datum    = linea.datum + cursor;
        pars.mensura  = linea.mensura - cursor;
        _evadere(a, pars);
    }
}

interior b32
_fragmentum_habet (
    constans Xar* fragmenta,
          chorda  id)
{
    i32 i;

    si (fragmenta == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(fragmenta); i++)
    {
        constans BriarFragmentum* f = (constans BriarFragmentum*)
            xar_obtinere(fragmenta, i);

        si (chorda_aequalis(f->id, id))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lineas textus emittere.
 *
 * Forma SICUT SCRIPTA (praefixum_id datum): id="lN" cum N linea
 * .thistle, data-n idem (numerus in CSS per ::before - textus purus
 * manet, ergo quod lector copiat CODEX est, non numeri); linea
 * transclusionis fit NEXUS ad definitionem.
 *
 * Forma CONTEXTA (praefixum_id NIHIL): ids absunt (aliter geminae
 * essent - eaedem lineae bis in pagina stant), et numerus fit ancora
 * VACUA ad lineam fontis: nexus sine textu, ergo copia adhuc munda.
 */
interior vacuum
_lineas_emittere (
      ChordaAedificator* a,
                 chorda  textus,
           constans Xar* tabula,
                    i32  linea_prima,
     constans character* praefixum_id,
           constans Xar* tecta,
           constans Xar* fragmenta)
{
    i32 cursor  = ZEPHYRUM;
    i32 k       = ZEPHYRUM;

    dum (cursor < textus.mensura)
    {
           i32 finis = cursor;
           i32 linea;
        chorda pars;
        chorda id;
        chorda praefixum;

        dum (finis < textus.mensura && textus.datum[finis] != (i8)'\n')
        {
            finis = finis + I;
        }
        linea         = _linea_indicis(tabula, linea_prima, k);
        pars.datum    = textus.datum + cursor;
        pars.mensura  = finis - cursor;
        si (praefixum_id != NIHIL)
        {
            chorda_aedificator_appendere_literis(a,
                "<span class=\"fr-linea\" id=\"");
            chorda_aedificator_appendere_literis(a, praefixum_id);
            chorda_aedificator_appendere_i32(a, (i32)linea);
            chorda_aedificator_appendere_literis(a, "\" data-n=\"");
            chorda_aedificator_appendere_i32(a, (i32)linea);
            chorda_aedificator_appendere_literis(a, "\">");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a,
                "<span class=\"fr-linea\"><a class=\"fr-numerus\""
                " href=\"#l");
            chorda_aedificator_appendere_i32(a, (i32)linea);
            chorda_aedificator_appendere_literis(a, "\" data-n=\"");
            chorda_aedificator_appendere_i32(a, (i32)linea);
            chorda_aedificator_appendere_literis(a, "\"></a>");
        }
        si (   praefixum_id != NIHIL
            && briar_contextus_lineam_legere(pars, &id, &praefixum) == I
            && _fragmentum_habet(fragmenta, id))
        {
            _evadere(a, praefixum);
            chorda_aedificator_appendere_literis(a,
                "<a class=\"fr-transclusio\" href=\"#frag-");
            _evadere(a, id);
            chorda_aedificator_appendere_literis(a, "\">&lt;&lt;#");
            _evadere(a, id);
            chorda_aedificator_appendere_literis(a, "&gt;&gt;</a>");
        }
        alioquin
        {
            _lineam_coloratam_emittere(a, pars,
                (tecta != NIHIL && k < xar_numerus(tecta))
                ? *(constans Xar* constans*)xar_obtinere(tecta, k)
                : NIHIL);
        }
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

/* fragmentum huius regionis aut NIHIL (radix) */
interior constans BriarFragmentum*
_fragmentum_regionis (
              constans Xar* fragmenta,
    constans BriarNexusRes* r)
{
    i32 i;

    si (fragmenta == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(fragmenta); i++)
    {
        constans BriarFragmentum* f = (constans BriarFragmentum*)
            xar_obtinere(fragmenta, i);

        si (f->regio == r)
        {
            redde f;
        }
    }
    redde NIHIL;
}

/* usus fragmenti ut nexus; vacui = 'non adhibitum' (vitium non est,
 * sed iudicium quod '-partes' idem imprimit) */
interior vacuum
_usus_emittere (
           ChordaAedificator* a,
    constans BriarFragmentum* f)
{
    i32 i;
    i32 n = (f->usus != NIHIL) ? xar_numerus(f->usus) : ZEPHYRUM;

    si (n == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a,
            "<span class=\"fr-otiosum\">non adhibitum</span>");
        redde;
    }
    chorda_aedificator_appendere_literis(a,
        "<span class=\"fr-usus\">adhibitum: ");
    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 linea = *(constans i32*)xar_obtinere(f->usus, i);

        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ", ");
        }
        chorda_aedificator_appendere_literis(a, "<a href=\"#l");
        chorda_aedificator_appendere_i32(a, (i32)linea);
        chorda_aedificator_appendere_literis(a, "\">");
        chorda_aedificator_appendere_i32(a, (i32)linea);
        chorda_aedificator_appendere_literis(a, "</a>");
    }
    chorda_aedificator_appendere_literis(a, "</span>");
}

interior vacuum
_regionem_emittere (
                Piscina* piscina,
           SilvaPiscina* silva_piscina,
      ChordaAedificator* a,
 constans BriarNexusRes* r,
           constans Xar* fragmenta)
{
    constans BriarFragmentum* f = _fragmentum_regionis(fragmenta, r);
                         Xar* tecta = _tecta_computare(piscina,
                             silva_piscina, r->contentum);

    si (f != NIHIL)
    {
        chorda_aedificator_appendere_literis(a,
            "<section class=\"fr-fragmentum\" id=\"frag-");
        _evadere(a, f->id);
        chorda_aedificator_appendere_literis(a,
            "\">\n<header class=\"fr-titulus\"><span"
            " class=\"fr-id\">#");
        _evadere(a, f->id);
        chorda_aedificator_appendere_literis(a, "</span> ");
        _usus_emittere(a, f);
        chorda_aedificator_appendere_literis(a, "</header>\n");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a,
            "<section class=\"fr-regio\" id=\"regio-");
        chorda_aedificator_appendere_i32(a, (i32)r->linea_initium);
        chorda_aedificator_appendere_literis(a, "\">\n");
    }
    chorda_aedificator_appendere_literis(a,
        "<pre class=\"fr-c\"><code>");
    _lineas_emittere(a, r->contentum, NIHIL, r->linea_initium, "l",
        tecta, fragmenta);
    chorda_aedificator_appendere_literis(a, "</code></pre>\n");
    /* radix texta: forma quam compilator videt, cum numeris
     * .thistle in margine - unaquaeque ancora ad fontem suum */
    si (   f                  == NIHIL
        && r->lineae          != NIHIL
        && r->contextus.datum != NIHIL
        && !chorda_aequalis(r->contextus, r->contentum))
    {
        Xar* tecta_contexta = _tecta_computare(piscina, silva_piscina,
            r->contextus);

        chorda_aedificator_appendere_literis(a,
            "<details class=\"fr-contextum\">\n<summary>contextum (");
        chorda_aedificator_appendere_i32(a,
            (i32)xar_numerus(r->lineae));
        chorda_aedificator_appendere_literis(a,
            " lineae)</summary>\n<pre class=\"fr-c\"><code>");
        _lineas_emittere(a, r->contextus, r->lineae, r->linea_initium,
            NIHIL, tecta_contexta, NIHIL);
        chorda_aedificator_appendere_literis(a,
            "</code></pre>\n</details>\n");
    }
    chorda_aedificator_appendere_literis(a, "</section>\n");
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
        _lineas_emittere(a, t->valor, NIHIL, r->linea_initium, "l",
            NIHIL, NIHIL);
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
          SilvaPiscina* silva_piscina;

    vacua.datum     = NIHIL;
    vacua.mensura   = ZEPHYRUM;
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    (vacuum)fons;
    /* piscina silvae PROPRIA: color etiam pro regionibus quibus
     * arbor deest (fragmenta, regiones fractae) - lex par. 4.6 F4
     * poscit ut pagina semper reddatur */
    silva_piscina = silva_piscina_generare_dynamicum("briar_facies",
        (insignatus longus)16777216);
    si (silva_piscina == NIHIL)
    {
        *causa = chorda_ex_literis("piscina silvae deest", piscina);
        redde vacua;
    }
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
            _regionem_emittere(piscina, silva_piscina, corpus, r,
                fragmenta);
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
    {
        chorda pagina = _involucrum_implere(piscina,
            vestis->involucrum, notae, (i32)6, causa);

        silva_piscina_destruere(silva_piscina);
        redde pagina;
    }
}
