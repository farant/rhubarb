/* vicipaedia.c - adaptator Vicipaediae (navigator N1; include/
 * vicipaedia.h). Arbor html (html_arbor_parsare, arbor OCTETORUM -
 * Parsoid HTML generatum et mundum est, adoptio nulla exspectatur)
 * ambulatur: <section> summae -> ArticulusSectio, h2 = titulus eius,
 * sectiones interiores in eandem fluunt (h3-h6 = TITULUS gradus
 * III-VI);
 * clausulae per elementa bloci; textus et notae per collectorem
 * (entitates decoctae, spatia ASCII contracta, <br> = '\n', bloci
 * interiores lineae novae). Recursio C per profunditatem DOM, limite
 * PROFUNDITAS_MAXIMA. */
#include "vicipaedia.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "entitates_html.h"
#include "materia_nodus.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "uri.h"
#include "url.h"
#include "json.h"
#include <stdio.h>

#include <string.h>

#define PROFUNDITAS_MAXIMA CCLVI


/* ==================================================
 * Arbor
 * ================================================== */

interior MateriaToken*
tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   !nodus || locus >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_TOKEN)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.token;
}

interior i32
numerus_liberorum (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   !nodus || locus >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(nodus->loci[locus]);
}

interior MateriaNodus*
liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (   !nodus || locus >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus
        : NIHIL;
}

interior b32
octeti_aequales_minusculi (
                 chorda  c,
     constans character* titulus)
{
    i32 n;
    i32 i;
     i8 x;

    n = (i32)strlen(titulus);
    si (c.mensura != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = c.datum[i];
        si (x >= 'A' && x <= 'Z')
        {
            x = (i8)(x + ('a' - 'A'));
        }
        si ((character)x != titulus[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* elementum hoc titulo (litteris neglectis)? verum aut fictum */
interior b32
est (
    constans MateriaNodus* nodus,
       constans character* titulus)
{
           MateriaToken* apertura;
    constans character* fictum;
                 chorda  nomen_tag;

    si (!nodus || nodus->genus != (s32)HTML_GENUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    apertura = tok(nodus, (i32)HTML_ELEMENTUM_TOK_APERTURA);
    si (apertura && apertura->valor.mensura > I)
    {
        nomen_tag = chorda_sectio(apertura->valor, I,
            apertura->valor.mensura);
        redde octeti_aequales_minusculi(nomen_tag, titulus);
    }
    si (   (i32)HTML_ELEMENTUM_SYNTHESIS < nodus->numerus_locorum
        && nodus->loci[HTML_ELEMENTUM_SYNTHESIS].genus
           == MATERIA_VALOR_INDEX)
    {
        fictum = html_arbor_synthesis_titulus(
            nodus->loci[HTML_ELEMENTUM_SYNTHESIS].datum.index);
        redde fictum && strcmp(fictum, titulus) == ZEPHYRUM;
    }
    redde FALSUM;
}

/* valor attributi (citationes exutae, entitates decoctae); vacua si
 * abest */
interior chorda
attributum (
    constans MateriaNodus* nodus,
       constans character* titulus,
                  Piscina* piscina)
{
         MateriaNodus* a;
         MateriaToken* t;
         MateriaToken* v;
    ChordaAedificator* aed;
               chorda  crudus;
            character  exitus[XVI];
                  i32  longitudo;
                  s32  post;
                  s32  i;
                  i32  n;
                  i32  k;

    n = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_ATTRIBUTA);
    per (k = ZEPHYRUM; k < n; k++)
    {
        a = liber(nodus, (i32)HTML_ELEMENTUM_ATTRIBUTA, k);
        t = tok(a, (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        si (!t || !octeti_aequales_minusculi(t->valor, titulus))
        {
            perge;
        }
        v = tok(a, (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si (!v)
        {
            frange;
        }
        crudus = v->valor;
        si (   crudus.mensura >= II
            && (crudus.datum[ZEPHYRUM] == '"'
                || crudus.datum[ZEPHYRUM] == '\'')
            && crudus.datum[crudus.mensura - I]
               == crudus.datum[ZEPHYRUM])
        {
            crudus = chorda_sectio(crudus, I, crudus.mensura - I);
        }
        aed = chorda_aedificator_creare(piscina,
            (memoriae_index)crudus.mensura + I);
        si (!aed)
        {
            frange;
        }
        i = ZEPHYRUM;
        dum (i < (s32)crudus.mensura)
        {
            si (   crudus.datum[i] == '&'
                && entitates_html_decoquere(
                       (constans character*)crudus.datum, i,
                       (s32)crudus.mensura, VERUM, exitus, &longitudo,
                       &post))
            {
                exitus[longitudo] = '\0';
                chorda_aedificator_appendere_literis(aed, exitus);
                i = post;
                perge;
            }
            chorda_aedificator_appendere_character(aed,
                (character)crudus.datum[i]);
            i++;
        }
        redde chorda_aedificator_finire(aed);
    }
    redde chorda_ex_literis("", piscina);
}

/* classis (signum inter spatia) praesens? */
interior b32
classem_habet (
    constans MateriaNodus* nodus,
       constans character* classis,
                  Piscina* piscina)
{
    chorda c;
       i32 i;
       i32 initium;

    c        = attributum(nodus, "class", piscina);
    initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= c.mensura; i++)
    {
        si (   i == c.mensura || c.datum[i] == ' ' || c.datum[i] == '\t'
            || c.datum[i] == '\n')
        {
            si (   i > initium
                && chorda_aequalis_literis(chorda_sectio(c, initium, i),
                       classis))
            {
                redde VERUM;
            }
            initium = i + I;
        }
    }
    redde FALSUM;
}

/* elementa omittenda: notae referentiarum, navbox, stylus, occulta,
 * pinacothecae (v1) */
interior b32
omittendum (
    constans MateriaNodus* nodus,
                  Piscina* piscina)
{
    hic_manens constans character* constans classes[] = {
        "mw-ref", "navbox", "navbox-styles", "noprint", "metadata",
        "shortdescription", "mw-empty-elt", "reflist",
        "mw-references-wrap", "gallery", NIHIL };
    i32 i;

    si (   est(nodus, "style") || est(nodus, "link")
        || est(nodus, "meta") || est(nodus, "script")
        || est(nodus, "noscript") || est(nodus, "img"))
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; classes[i]; i++)
    {
        si (classem_habet(nodus, classes[i], piscina))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
est_blocus (
    constans MateriaNodus* nodus)
{
    hic_manens constans character* constans bloci[] = {
        "p", "ul", "ol", "dl", "dt", "dd", "li", "blockquote", "table",
        "figure", "section", "div", "h1", "h2", "h3", "h4", "h5", "h6",
        "pre", "hr", "center", "header", "footer", "nav", "aside",
        "main", "article", "details", "summary", "figcaption",
        "caption", "tbody", "thead", "tfoot", "tr", "td", "th",
        "address", "form", "fieldset", NIHIL };
    i32 i;

    per (i = ZEPHYRUM; bloci[i]; i++)
    {
        si (est(nodus, bloci[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* gradus tituli h1-h6; 0 si non titulus */
interior s32
gradus_tituli (
    constans MateriaNodus* nodus)
{
    hic_manens constans character* constans tituli[] = {
        "h1", "h2", "h3", "h4", "h5", "h6" };
    i32 i;

    per (i = ZEPHYRUM; i < VI; i++)
    {
        si (est(nodus, tituli[i]))
        {
            redde (s32)i + I;
        }
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Collector textus et notarum
 * ================================================== */

nomen structura {
    Piscina* piscina;
        Uri* basis;     /* NIHIL si fons nullus aut malus */
        Uri  uri;       /* repositorium basis */
} Contextus;

nomen structura {
            Contextus* ctx;
    ChordaAedificator* textus;
                  Xar* notae;
                  b32  spatium_pendens;
                  b32  initium_lineae;
                  b32  sine_indicibus;  /* li: ul/ol interiores
                                         * suae clausulae sunt */
} Collector;

interior b32
collectorem_parare (
    Collector* col,
    Contextus* ctx)
{
    memset(col, ZEPHYRUM, magnitudo(*col));
    col->ctx             = ctx;
    col->textus          = chorda_aedificator_creare(ctx->piscina,
        CCLVI);
    col->notae           = xar_creare(ctx->piscina,
        (i32)magnitudo(ArticulusNota));
    col->initium_lineae  = VERUM;
    redde col->textus && col->notae;
}

interior i32
longitudo (
    Collector* col)
{
    redde (i32)chorda_aedificator_longitudo(col->textus);
}

interior vacuum
octetum_addere (
    Collector* col,
           i8  octetus)
{
    si (   octetus == ' ' || octetus == '\t' || octetus == '\n'
        || octetus == '\r' || octetus == '\f')
    {
        si (!col->initium_lineae)
        {
            col->spatium_pendens = VERUM;
        }
        redde;
    }
    si (octetus == ZEPHYRUM)
    {
        redde;
    }
    si (col->spatium_pendens)
    {
        chorda_aedificator_appendere_character(col->textus, ' ');
        col->spatium_pendens = FALSUM;
    }
    chorda_aedificator_appendere_character(col->textus,
        (character)octetus);
    col->initium_lineae = FALSUM;
}

/* spatium pendens nunc scribere (initium notae post spatium cadat) */
interior vacuum
spatium_solvere (
    Collector* col)
{
    si (col->spatium_pendens && !col->initium_lineae)
    {
        chorda_aedificator_appendere_character(col->textus, ' ');
    }
    col->spatium_pendens = FALSUM;
}

interior vacuum
lineam_novam (
    Collector* col)
{
    si (!col->initium_lineae && longitudo(col) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(col->textus, '\n');
    }
    col->spatium_pendens  = FALSUM;
    col->initium_lineae   = VERUM;
}

interior vacuum
textum_addere (
    Collector* col,
       chorda  textus)
{
    character exitus[XVI];
          i32 n;
          s32 post;
          s32 i;
          i32 k;

    i = ZEPHYRUM;
    dum (i < (s32)textus.mensura)
    {
        si (   textus.datum[i] == '&'
            && entitates_html_decoquere((constans character*)
                   textus.datum, i, (s32)textus.mensura, FALSUM,
                   exitus, &n, &post))
        {
            per (k = ZEPHYRUM; k < n; k++)
            {
                octetum_addere(col, (i8)exitus[k]);
            }
            i = post;
            perge;
        }
        octetum_addere(col, textus.datum[i]);
        i++;
    }
}

/* scopus nexus absolutus (basis + relatum); relatum crudum si
 * basis nulla */
interior chorda
scopum_resolvere (
    Contextus* ctx,
       chorda  relatum)
{
    Uri* u;

    si (!ctx->basis || relatum.mensura == ZEPHYRUM)
    {
        redde relatum;
    }
    u = uri_resolvere(ctx->basis, relatum, ctx->piscina);
    redde u ? uri_construere(u, ctx->piscina) : relatum;
}

interior vacuum
colligere (
                Collector* col,
    constans MateriaNodus* nodus,
                      i32  profunditas);

interior vacuum
elementum_colligere (
                Collector* col,
    constans MateriaNodus* l,
                      i32  profunditas);

interior vacuum
notam_colligere (
                Collector* col,
    constans MateriaNodus* nodus,
       ArticulusNotaGenus  genus,
                   chorda  scopus,
                      i32  profunditas)
{
    ArticulusNota* n;
              i32  initium;

    spatium_solvere(col);
    initium = longitudo(col);
    colligere(col, nodus, profunditas + I);
    si (longitudo(col) > initium)
    {
        n = (ArticulusNota*)xar_addere(col->notae);
        si (n)
        {
            n->genus    = genus;
            n->initium  = initium;
            n->finis    = longitudo(col);
            n->scopus   = scopus;
        }
    }
}

interior vacuum
colligere (
                Collector* col,
    constans MateriaNodus* nodus,
                      i32  profunditas)
{
    MateriaNodus* l;
    MateriaToken* t;
             i32  n;
             i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    n = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (!l)
        {
            perge;
        }
        si (   l->genus == (s32)HTML_GENUS_TEXTUS
            || l->genus == (s32)HTML_GENUS_REFERENTIA
            || l->genus == (s32)HTML_GENUS_TEXTUS_CRUDUS)
        {
            t = tok(l, ZEPHYRUM);
            si (t)
            {
                textum_addere(col, t->valor);
            }
            perge;
        }
        elementum_colligere(col, l, profunditas);
    }
}

/* elementum unum in collectorem (omissa, br, nexus, emphasis, bloci
 * interiores ut lineae) */
interior vacuum
elementum_colligere (
                Collector* col,
    constans MateriaNodus* l,
                      i32  profunditas)
{
    chorda relatum;

    {
        si (   l->genus != (s32)HTML_GENUS_ELEMENTUM
            || omittendum(l, col->ctx->piscina))
        {
            redde;
        }
        si (est(l, "br"))
        {
            lineam_novam(col);
        }
        alioquin si (est(l, "table"))
        {
            redde;   /* tabulae intra textum: v1 omittuntur */
        }
        alioquin si (   col->sine_indicibus
                     && (est(l, "ul") || est(l, "ol")))
        {
            redde;
        }
        alioquin si (est(l, "a"))
        {
            relatum = attributum(l, "href", col->ctx->piscina);
            si (relatum.mensura > ZEPHYRUM)
            {
                notam_colligere(col, l, ARTICULUS_NEXUS,
                    scopum_resolvere(col->ctx, relatum), profunditas);
            }
            alioquin
            {
                colligere(col, l, profunditas + I);
            }
        }
        alioquin si (est(l, "i") || est(l, "em"))
        {
            notam_colligere(col, l, ARTICULUS_EMPHASIS,
                chorda_ex_literis("", col->ctx->piscina), profunditas);
        }
        alioquin si (est(l, "b") || est(l, "strong"))
        {
            notam_colligere(col, l, ARTICULUS_FORTIS,
                chorda_ex_literis("", col->ctx->piscina), profunditas);
        }
        alioquin si (est_blocus(l))
        {
            lineam_novam(col);
            colligere(col, l, profunditas + I);
            lineam_novam(col);
        }
        alioquin
        {
            colligere(col, l, profunditas + I);
        }
    }
}

/* textus finitus (lineae novae ultimae resectae, notae ad eum
 * circumcisae) */
interior chorda
collectorem_finire (
    Collector*  col,
          Xar** notae)
{
           chorda  textus;
    ArticulusNota* n;
              i32  i;

    textus = chorda_aedificator_finire(col->textus);
    dum (   textus.mensura > ZEPHYRUM
         && (textus.datum[textus.mensura - I] == '\n'
             || textus.datum[textus.mensura - I] == ' '))
    {
        textus.mensura--;
    }
    per (i = ZEPHYRUM; i < xar_numerus(col->notae); i++)
    {
        n = (ArticulusNota*)xar_obtinere(col->notae, i);
        si (n->finis > textus.mensura)
        {
            n->finis = textus.mensura;
        }
        si (n->initium > n->finis)
        {
            n->initium = n->finis;
        }
    }
    *notae = col->notae;
    redde textus;
}


/* ==================================================
 * Clausulae
 * ================================================== */

interior ArticulusClausula*
clausulam_addere (
               Xar* clausulae,
    ArticulusGenus  genus,
               s32  gradus,
           Piscina* piscina)
{
    ArticulusClausula* k;

    k = (ArticulusClausula*)xar_addere(clausulae);
    si (k)
    {
        memset(k, ZEPHYRUM, magnitudo(*k));
        k->genus   = genus;
        k->gradus  = gradus;
        k->textus  = chorda_ex_literis("", piscina);
        k->clavis  = chorda_ex_literis("", piscina);
    }
    redde k;
}

/* clausula ex textu elementi; vacua non additur */
interior vacuum
clausulam_colligere (
                Contextus* ctx,
                      Xar* clausulae,
           ArticulusGenus  genus,
                      s32  gradus,
    constans MateriaNodus* nodus,
                      b32  sine_indicibus)
{
            Collector  col;
    ArticulusClausula* k;
               chorda  textus;
                  Xar* notae;

    si (!collectorem_parare(&col, ctx))
    {
        redde;
    }
    col.sine_indicibus = sine_indicibus;
    colligere(&col, nodus, ZEPHYRUM);
    textus = collectorem_finire(&col, &notae);
    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    k = clausulam_addere(clausulae, genus, gradus, ctx->piscina);
    si (k)
    {
        k->textus  = textus;
        k->notae   = notae;
    }
}


/* ==================================================
 * Adiuncta (infobox)
 * ================================================== */

interior chorda
textum_cellae (
                Contextus*  ctx,
    constans MateriaNodus*  cella,
                      Xar** notae)
{
    Collector col;

    si (!collectorem_parare(&col, ctx))
    {
        *notae = NIHIL;
        redde chorda_ex_literis("", ctx->piscina);
    }
    colligere(&col, cella, ZEPHYRUM);
    redde collectorem_finire(&col, notae);
}

/* nodus tabulam (descendentem) continet? */
interior b32
tabulam_continet (
    constans MateriaNodus* nodus,
                      i32  profunditas)
{
    MateriaNodus* l;
             i32  n;
             i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde FALSUM;
    }
    n = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (   l && (est(l, "table") || tabulam_continet(l,
                profunditas + I)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
ordines_legere (
                Contextus* ctx,
       ArticulusAdiunctum* ad,
    constans MateriaNodus* nodus,
                      i32  profunditas);

/* tabulas descendentes ordine legere */
interior vacuum
tabulas_interiores (
                Contextus* ctx,
       ArticulusAdiunctum* ad,
    constans MateriaNodus* nodus,
                      i32  profunditas)
{
    MateriaNodus* l;
             i32  n;
             i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    n = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (!l || omittendum(l, ctx->piscina))
        {
            perge;
        }
        si (est(l, "table"))
        {
            ordines_legere(ctx, ad, l, profunditas + I);
        }
        alioquin
        {
            tabulas_interiores(ctx, ad, l, profunditas + I);
        }
    }
}

/* clavis purgata: ':' et spatia ultima */
interior chorda
clavem_purgare (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && (c.datum[c.mensura - I] == ':'
             || c.datum[c.mensura - I] == ' '))
    {
        c.mensura--;
    }
    redde c;
}

interior vacuum
ordinem_legere (
                Contextus* ctx,
       ArticulusAdiunctum* ad,
    constans MateriaNodus* ordo,
                      i32  profunditas)
{
     MateriaNodus* cellae[II];
     MateriaNodus* l;
ArticulusClausula* k;
           chorda  textus;
              Xar* notae;
              Xar* notae_clavis;
              i32  numerus;
              i32  n;
              i32  i;

    si (tabulam_continet(ordo, ZEPHYRUM))
    {
        tabulas_interiores(ctx, ad, ordo, profunditas + I);
        redde;
    }
    numerus = ZEPHYRUM;
    cellae[ZEPHYRUM] = NIHIL;
    cellae[I] = NIHIL;
    n = numerus_liberorum(ordo, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(ordo, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (l && (est(l, "th") || est(l, "td")))
        {
            si (numerus < II)
            {
                cellae[numerus] = l;
            }
            numerus++;
        }
    }
    si (numerus == ZEPHYRUM)
    {
        redde;
    }
    si (numerus >= II)
    {
        textus = textum_cellae(ctx, cellae[I], &notae);
        si (textus.mensura == ZEPHYRUM)
        {
            redde;
        }
        k = clausulam_addere(ad->clausulae, ARTICULUS_PAR, ZEPHYRUM,
            ctx->piscina);
        si (k)
        {
            k->clavis  = clavem_purgare(textum_cellae(ctx,
                cellae[ZEPHYRUM], &notae_clavis));
            k->textus  = textus;
            k->notae   = notae;
            si (k->clavis.mensura == ZEPHYRUM)
            {
                k->genus = ARTICULUS_PARAGRAPHUS;
            }
        }
        redde;
    }
    textus = textum_cellae(ctx, cellae[ZEPHYRUM], &notae);
    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (est(cellae[ZEPHYRUM], "th"))
    {
        si (   ad->titulus.mensura        == ZEPHYRUM
            && xar_numerus(ad->clausulae) == ZEPHYRUM)
        {
            ad->titulus = textus;
            redde;
        }
        k = clausulam_addere(ad->clausulae, ARTICULUS_TITULUS, III,
            ctx->piscina);
    }
    alioquin
    {
        k = clausulam_addere(ad->clausulae, ARTICULUS_PARAGRAPHUS,
            ZEPHYRUM, ctx->piscina);
    }
    si (k)
    {
        k->textus  = textus;
        k->notae   = notae;
    }
}

interior vacuum
ordines_legere (
                Contextus* ctx,
       ArticulusAdiunctum* ad,
    constans MateriaNodus* nodus,
                      i32  profunditas)
{
    MateriaNodus* l;
           chorda  textus;
              Xar* notae;
             i32  n;
             i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    n = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (   !l || l->genus != (s32)HTML_GENUS_ELEMENTUM
            || omittendum(l, ctx->piscina))
        {
            perge;
        }
        si (est(l, "caption"))
        {
            textus = textum_cellae(ctx, l, &notae);
            si (ad->titulus.mensura == ZEPHYRUM)
            {
                ad->titulus = textus;
            }
        }
        alioquin si (est(l, "tr"))
        {
            ordinem_legere(ctx, ad, l, profunditas + I);
        }
        alioquin si (   est(l, "tbody") || est(l, "thead")
                     || est(l, "tfoot"))
        {
            ordines_legere(ctx, ad, l, profunditas + I);
        }
    }
}


/* ==================================================
 * Bloci
 * ================================================== */

nomen structura {
             Contextus* ctx;
       ArticulusSectio* sectio;   /* summa currens */
} Ambulatio;

interior vacuum
contenta_legere (
                Ambulatio* amb,
    constans MateriaNodus* nodus,
                      Xar* clausulae,
                      s32  gradus_indicis,
                      i32  profunditas);

interior vacuum
indicem_legere (
                Ambulatio* amb,
    constans MateriaNodus* index,
                      Xar* clausulae,
                      s32  gradus,
                      i32  profunditas)
{
    MateriaNodus* li;
    MateriaNodus* l;
             i32  n;
             i32  m;
             i32  i;
             i32  j;

    n = numerus_liberorum(index, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        li = liber(index, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (!li || !est(li, "li") || omittendum(li, amb->ctx->piscina))
        {
            perge;
        }
        clausulam_colligere(amb->ctx, clausulae, ARTICULUS_INDEX,
            gradus, li, VERUM);
        m = numerus_liberorum(li, (i32)HTML_ELEMENTUM_LIBERI);
        per (j = ZEPHYRUM; j < m; j++)
        {
            l = liber(li, (i32)HTML_ELEMENTUM_LIBERI, j);
            si (   l && (est(l, "ul") || est(l, "ol"))
                && !omittendum(l, amb->ctx->piscina)
                && profunditas < PROFUNDITAS_MAXIMA)
            {
                indicem_legere(amb, l, clausulae, gradus + I,
                    profunditas + I);
            }
        }
    }
}

interior vacuum
figuram_legere (
                Ambulatio* amb,
    constans MateriaNodus* figura,
                      Xar* clausulae)
{
    ArticulusClausula* k;
         MateriaNodus* l;
         MateriaNodus* imago;
               chorda  src;
               chorda  titulus;
                  Xar* notae;
                  i32  n;
                  i32  i;

    imago    = NIHIL;
    titulus  = chorda_ex_literis("", amb->ctx->piscina);
    notae    = NIHIL;
    n        = numerus_liberorum(figura, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(figura, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (!l)
        {
            perge;
        }
        si (est(l, "figcaption"))
        {
            titulus = textum_cellae(amb->ctx, l, &notae);
        }
        alioquin si (est(l, "a") || est(l, "span"))
        {
            /* <a><img/></a> aut <span><img/></span> */
            si (   numerus_liberorum(l, (i32)HTML_ELEMENTUM_LIBERI)
                   > ZEPHYRUM
                && est(liber(l, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM),
                       "img"))
            {
                imago = liber(l, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
            }
        }
        alioquin si (est(l, "img"))
        {
            imago = l;
        }
    }
    src = imago ? attributum(imago, "src", amb->ctx->piscina)
        : chorda_ex_literis("", amb->ctx->piscina);
    si (src.mensura == ZEPHYRUM)
    {
        redde;
    }
    k = clausulam_addere(clausulae, ARTICULUS_FIGURA, ZEPHYRUM,
        amb->ctx->piscina);
    si (k)
    {
        k->clavis  = scopum_resolvere(amb->ctx, src);
        k->textus  = titulus;
        k->notae   = notae;
    }
}

interior vacuum
adiunctum_legere (
                Ambulatio* amb,
    constans MateriaNodus* tabula)
{
    ArticulusAdiunctum* ad;

    si (!amb->sectio)
    {
        redde;
    }
    si (!amb->sectio->adiuncta)
    {
        amb->sectio->adiuncta = xar_creare(amb->ctx->piscina,
            (i32)magnitudo(ArticulusAdiunctum));
    }
    ad = amb->sectio->adiuncta ? (ArticulusAdiunctum*)xar_addere(
        amb->sectio->adiuncta) : NIHIL;
    si (!ad)
    {
        redde;
    }
    ad->titulus    = chorda_ex_literis("", amb->ctx->piscina);
    ad->clausulae  = xar_creare(amb->ctx->piscina,
        (i32)magnitudo(ArticulusClausula));
    si (ad->clausulae)
    {
        ordines_legere(amb->ctx, ad, tabula, ZEPHYRUM);
    }
}

/* paragraphus anonymus: textus et elementa inlinea inter blocos */
nomen structura {
      Collector col;
            b32 apertus;
} Anonymus;

interior vacuum
anonymum_claudere (
     Ambulatio* amb,
      Anonymus* an,
           Xar* clausulae)
{
    ArticulusClausula* k;
               chorda  textus;
                  Xar* notae;

    si (!an->apertus)
    {
        redde;
    }
    an->apertus  = FALSUM;
    textus       = collectorem_finire(&an->col, &notae);
    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    k = clausulam_addere(clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM,
        amb->ctx->piscina);
    si (k)
    {
        k->textus  = textus;
        k->notae   = notae;
    }
}

interior vacuum
contenta_legere (
                Ambulatio* amb,
    constans MateriaNodus* nodus,
                      Xar* clausulae,
                      s32  gradus_indicis,
                      i32  profunditas)
{
    MateriaNodus* l;
         Anonymus  an;
              s32  gradus;
              i32  n;
              i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    an.apertus  = FALSUM;
    n           = numerus_liberorum(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, (i32)HTML_ELEMENTUM_LIBERI, i);
        si (!l)
        {
            perge;
        }
        si (   l->genus != (s32)HTML_GENUS_ELEMENTUM
            && l->genus != (s32)HTML_GENUS_TEXTUS
            && l->genus != (s32)HTML_GENUS_REFERENTIA)
        {
            perge;
        }
        si (   l->genus == (s32)HTML_GENUS_ELEMENTUM
            && omittendum(l, amb->ctx->piscina))
        {
            perge;
        }
        si (l->genus != (s32)HTML_GENUS_ELEMENTUM || !est_blocus(l))
        {
            /* inlineum: in paragraphum anonymum */
            si (!an.apertus)
            {
                si (!collectorem_parare(&an.col, amb->ctx))
                {
                    perge;
                }
                an.apertus = VERUM;
            }
            si (l->genus == (s32)HTML_GENUS_ELEMENTUM)
            {
                elementum_colligere(&an.col, l, profunditas);
            }
            alioquin
            {
                MateriaToken* t;

                t = tok(l, ZEPHYRUM);
                si (t)
                {
                    textum_addere(&an.col, t->valor);
                }
            }
            perge;
        }
        anonymum_claudere(amb, &an, clausulae);
        gradus = gradus_tituli(l);
        si (   gradus                              == II && amb->sectio
            && amb->sectio->titulus.mensura        == ZEPHYRUM
            && xar_numerus(amb->sectio->clausulae) == ZEPHYRUM)
        {
            Collector  col;
                  Xar* notae;

            si (collectorem_parare(&col, amb->ctx))
            {
                colligere(&col, l, profunditas + I);
                amb->sectio->titulus = collectorem_finire(&col, &notae);
                amb->sectio->ancora = attributum(l, "id",
                    amb->ctx->piscina);
            }
        }
        alioquin si (gradus > ZEPHYRUM)
        {
            clausulam_colligere(amb->ctx, clausulae, ARTICULUS_TITULUS,
                gradus, l, FALSUM);
        }
        alioquin si (   est(l, "p") || est(l, "dt") || est(l, "dd")
                     || est(l, "pre") || est(l, "address")
                     || est(l, "figcaption"))
        {
            clausulam_colligere(amb->ctx, clausulae,
                ARTICULUS_PARAGRAPHUS, ZEPHYRUM, l, FALSUM);
        }
        alioquin si (est(l, "ul") || est(l, "ol"))
        {
            indicem_legere(amb, l, clausulae, gradus_indicis + I,
                profunditas + I);
        }
        alioquin si (est(l, "blockquote"))
        {
            clausulam_colligere(amb->ctx, clausulae, ARTICULUS_CITATIO,
                ZEPHYRUM, l, FALSUM);
        }
        alioquin si (est(l, "figure"))
        {
            figuram_legere(amb, l, clausulae);
        }
        alioquin si (est(l, "table"))
        {
            si (classem_habet(l, "infobox", amb->ctx->piscina))
            {
                adiunctum_legere(amb, l);
            }
        }
        alioquin si (est(l, "hr"))
        {
            perge;
        }
        alioquin
        {
            /* section, div, dl et ceteri: continentes */
            contenta_legere(amb, l, clausulae, gradus_indicis,
                profunditas + I);
        }
    }
    anonymum_claudere(amb, &an, clausulae);
}


/* ==================================================
 * Documentum
 * ================================================== */

interior ArticulusSectio*
sectionem_novam (
    Articulus* a,
      Piscina* piscina)
{
    ArticulusSectio* s;

    s = (ArticulusSectio*)xar_addere(a->sectiones);
    si (s)
    {
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->titulus  = chorda_ex_literis("", piscina);
        s->ancora   = chorda_ex_literis("", piscina);
        s->clausulae  = xar_creare(piscina,
            (i32)magnitudo(ArticulusClausula));
        si (!s->clausulae)
        {
            redde NIHIL;
        }
    }
    redde s;
}

/* corpus: <section> summae -> sectiones (contentum extra eas
 * neglegitur); sine ulla <section> corpus totum prooemium est */
interior vacuum
corpus_legere (
                Ambulatio* amb,
                Articulus* a,
    constans MateriaNodus* corpus,
                      i32  profunditas)
{
    MateriaNodus* m;
             b32  sectiones;
             i32  numerus;
             i32  k;

    sectiones  = FALSUM;
    numerus    = numerus_liberorum(corpus, (i32)HTML_ELEMENTUM_LIBERI);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (est(liber(corpus, (i32)HTML_ELEMENTUM_LIBERI, k),
                "section"))
        {
            sectiones = VERUM;
        }
    }
    si (!sectiones)
    {
        amb->sectio = sectionem_novam(a, amb->ctx->piscina);
        si (amb->sectio)
        {
            contenta_legere(amb, corpus, amb->sectio->clausulae,
                ZEPHYRUM, profunditas + I);
        }
        redde;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        m = liber(corpus, (i32)HTML_ELEMENTUM_LIBERI, k);
        si (!est(m, "section"))
        {
            perge;
        }
        amb->sectio = sectionem_novam(a, amb->ctx->piscina);
        si (amb->sectio)
        {
            contenta_legere(amb, m, amb->sectio->clausulae, ZEPHYRUM,
                profunditas + I);
        }
    }
}

/* titulum (head) et corpus quaerere */
interior vacuum
documentum_ambulare (
                Ambulatio* amb,
                Articulus* a,
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  profunditas)
{
    MateriaNodus* l;
       Collector  col;
             Xar* notae;
             i32  n;
             i32  i;

    si (profunditas > PROFUNDITAS_MAXIMA)
    {
        redde;
    }
    n = numerus_liberorum(nodus, locus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        l = liber(nodus, locus, i);
        si (!l || l->genus != (s32)HTML_GENUS_ELEMENTUM)
        {
            perge;
        }
        si (est(l, "title"))
        {
            si (   a->titulus.mensura == ZEPHYRUM
                && collectorem_parare(&col, amb->ctx))
            {
                colligere(&col, l, ZEPHYRUM);
                a->titulus = collectorem_finire(&col, &notae);
            }
        }
        alioquin si (est(l, "html") || est(l, "head"))
        {
            documentum_ambulare(amb, a, l, (i32)HTML_ELEMENTUM_LIBERI,
                profunditas + I);
        }
        alioquin si (est(l, "body"))
        {
            corpus_legere(amb, a, l, profunditas + I);
        }
    }
}

/* sectiones v1 omissae aut vacuae tolluntur */
interior b32
sectio_servanda (
    constans ArticulusSectio* s)
{
    hic_manens constans character* constans omissae[] = {
        "References", "Further reading", "External links", NIHIL };
    i32 i;

    per (i = ZEPHYRUM; omissae[i]; i++)
    {
        si (chorda_aequalis_literis(s->titulus, omissae[i]))
        {
            redde FALSUM;
        }
    }
    redde xar_numerus(s->clausulae) > ZEPHYRUM
        || (s->adiuncta && xar_numerus(s->adiuncta) > ZEPHYRUM);
}

Articulus*
vicipaedia_articulus (
                 chorda  html,
     constans character* fons,
                Piscina* piscina)
{
       Articulus* a;
    MateriaNodus* radix;
       Contextus  ctx;
       Ambulatio  amb;
             Xar* servatae;
             i32  i;

    a = (Articulus*)piscina_allocare(piscina, magnitudo(Articulus));
    radix = html_arbor_parsare(piscina, (constans character*)html.datum,
        html.mensura);
    si (!a || !radix)
    {
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->titulus  = chorda_ex_literis("", piscina);
    a->fons     = chorda_ex_literis(fons ? fons : "", piscina);
    a->sectiones  = xar_creare(piscina,
        (i32)magnitudo(ArticulusSectio));
    servatae      = xar_creare(piscina,
        (i32)magnitudo(ArticulusSectio));
    si (!a->sectiones || !servatae)
    {
        redde NIHIL;
    }
    ctx.piscina  = piscina;
    ctx.basis    = (fons && uri_parse(fons, &ctx.uri, piscina))
                 ? &ctx.uri : NIHIL;
    amb.ctx     = &ctx;
    amb.sectio  = NIHIL;
    documentum_ambulare(&amb, a, radix, (i32)HTML_DOCUMENTUM_LIBERI,
        ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(a->sectiones); i++)
    {
        ArticulusSectio* s;
        ArticulusSectio* novus;

        s = (ArticulusSectio*)xar_obtinere(a->sectiones, i);
        si (!sectio_servanda(s))
        {
            perge;
        }
        novus = (ArticulusSectio*)xar_addere(servatae);
        si (!novus)
        {
            redde NIHIL;
        }
        *novus = *s;
    }
    a->sectiones = servatae;
    redde a;
}


/* ==================================================
 * Sedes (N2.2)
 * ================================================== */

#define VIA_ARTICULI "/wiki/"
#define VIA_FONTIS   "/api/rest_v1/page/html/"
/* API MediaWiki nova: rest_v1 paginas redirectionis huc mittit
 * (".../page/<T>/html?redirect=no", Vicipaedia viva 2026-10-27) */
#define VIA_REST     "/w/rest.php/v1/page/"
#define SUFFIXUM     ".wikipedia.org"

/* "<lingua>[.m].wikipedia.org" (sine casu) -> "<lingua>.wikipedia.org";
 * lingua = [a-z0-9-]+ */
interior b32
hospitem_legere (
      chorda  hospes,
      chorda* exitus,
     Piscina* piscina)
{
    chorda h;
    chorda suffixum;
       i32 n;
       i32 i;
        i8 c;

    h         = chorda_minuscula(hospes, piscina);
    suffixum  = chorda_ex_literis(SUFFIXUM, piscina);
    si (!chorda_terminatur(h, suffixum))
    {
        redde FALSUM;
    }
    n = h.mensura - suffixum.mensura;
    si (n > II && h.datum[n - II] == '.' && h.datum[n - I] == 'm')
    {
        n -= II;
    }
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        c = h.datum[i];
        si (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
              || c == '-'))
        {
            redde FALSUM;
        }
    }
    *exitus = chorda_concatenare(chorda_sectio(h, ZEPHYRUM, n),
        suffixum, piscina);
    redde VERUM;
}

/* percent-decodicatum (sine '+' -> spatium), spatia -> '_' */
interior chorda
titulum_purgare (
      chorda  codificatus,
     Piscina* piscina)
{
    chorda t;
       i32 i;

    t = url_viam_decodificare(codificatus, piscina);
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == ' ')
        {
            t.datum[i] = '_';
        }
    }
    redde t;
}

b32
vicipaedia_sedem_legere (
               chorda  url,
      VicipaediaSedes* exitus,
              Piscina* piscina)
{
       Uri u;
    chorda via;
    chorda praefixum;
    chorda titulus;
       s32 k;

    si (   !exitus || !piscina || url.mensura == ZEPHYRUM
        || !uri_parse(chorda_ut_cstr(url, piscina), &u, piscina))
    {
        redde FALSUM;
    }
    si (   !(chorda_aequalis_case_insensitivus(u.schema,
              chorda_ex_literis("https", piscina))
          || chorda_aequalis_case_insensitivus(u.schema,
              chorda_ex_literis("http", piscina)))
        || u.userinfo.mensura > ZEPHYRUM
        || !hospitem_legere(u.hospes, &exitus->hospes, piscina))
    {
        redde FALSUM;
    }
    via        = u.via;
    praefixum  = chorda_ex_literis(VIA_FONTIS, piscina);
    si (!chorda_incipit(via, praefixum))
    {
        praefixum = chorda_ex_literis(VIA_REST, piscina);
    }
    si (chorda_incipit(via, praefixum))
    {
        /* fons (rest_v1 aut rest.php): titulus usque ad '/' crudum
         * (revisio aut "/html" sequitur) */
        titulus = chorda_sectio(via, praefixum.mensura, via.mensura);
        k       = chorda_invenire_index(titulus, chorda_ex_literis("/",
            piscina));
        si (k >= ZEPHYRUM)
        {
            titulus = chorda_sectio(titulus, ZEPHYRUM, (i32)k);
        }
    }
    alioquin
    {
        praefixum = chorda_ex_literis(VIA_ARTICULI, piscina);
        si (!chorda_incipit(via, praefixum))
        {
            redde FALSUM;
        }
        titulus = chorda_sectio(via, praefixum.mensura, via.mensura);
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    exitus->titulus  = titulum_purgare(titulus, piscina);
    exitus->ancora   = titulum_purgare(u.fragmentum, piscina);
    redde VERUM;
}

/* "https://" + hospes + via + titulus codificatus */
interior chorda
inscriptionem_construere (
    constans VicipaediaSedes* sedes,
          constans character* via,
                     Piscina* piscina)
{
    chorda r;

    r = chorda_concatenare(chorda_ex_literis("https://", piscina),
        sedes->hospes, piscina);
    r = chorda_concatenare(r, chorda_ex_literis(via, piscina), piscina);
    redde chorda_concatenare(r, url_codificare(sedes->titulus, piscina),
        piscina);
}

chorda
vicipaedia_url_articuli (
    constans VicipaediaSedes* sedes,
                     Piscina* piscina)
{
    redde inscriptionem_construere(sedes, VIA_ARTICULI, piscina);
}

chorda
vicipaedia_url_fontis (
    constans VicipaediaSedes* sedes,
                     Piscina* piscina)
{
    redde inscriptionem_construere(sedes, VIA_FONTIS, piscina);
}


/* ==================================================
 * Quaestio (N3.3b)
 * ================================================== */

chorda
vicipaedia_url_quaestionis (
      chorda  hospes,
      chorda  verba,
         i32  limes,
     Piscina* piscina)
{
       chorda r;
    character numerus[XVI];

    sprintf(numerus, "&limit=%u", (insignatus integer)limes);
    r = chorda_concatenare(chorda_ex_literis("https://", piscina),
        hospes,
        piscina);
    r = chorda_concatenare(r, chorda_ex_literis(
        "/w/rest.php/v1/search/page?q=", piscina), piscina);
    r = chorda_concatenare(r, url_codificare(verba, piscina), piscina);
    redde chorda_concatenare(r, chorda_ex_literis(numerus, piscina),
        piscina);
}

Xar*
vicipaedia_eventus_legere (
      chorda  json,
     Piscina* piscina)
{
    JsonResultus  r;
       JsonValor* paginae;
             Xar* x;
             i32  i;
          chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    r              = json_legere(json, piscina);
    si (!r.successus || !r.radix || !json_est_objectum(r.radix))
    {
        redde NIHIL;
    }
    paginae = json_objectum_capere(r.radix, "pages");
    si (!paginae || !json_est_tabulatum(paginae))
    {
        redde NIHIL;
    }
    x = xar_creare(piscina, magnitudo(VicipaediaEventus));
    per (i = ZEPHYRUM; x && i < json_tabulatum_numerus(paginae); i++)
    {
                JsonValor* p = json_tabulatum_obtinere(paginae, i);
                   chorda  clavis;
        VicipaediaEventus* e;

        si (!p || !json_est_objectum(p))
        {
            perge;
        }
        clavis = json_capere_chorda(p, "key", vacua);
        si (clavis.mensura == ZEPHYRUM)
        {
            perge;
        }
        e = (VicipaediaEventus*)xar_addere(x);
        si (!e)
        {
            frange;
        }
        e->clavis      = clavis;
        e->titulus     = json_capere_chorda(p, "title", clavis);
        e->descriptio  = json_capere_chorda(p, "description", vacua);
    }
    redde x;
}
