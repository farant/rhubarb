/* probatio_md_inlinea.c - Arbor inlinea (A7): structura et valores
 *
 * Exempla CommonMark par. 6 (code spans, emphasis regulae I-XVII
 * selectae, nexus, imagines, autonexus, html, fracturae) et GFM
 * (deletio, autonexus nudi). Quaeque fixtura etiam octetim (emissio
 * fonte ZEPHYRUM) - arbor inlinea octetos possidet.
 */

#include "latina.h"
#include "credo.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior MateriaNodus*
_parsa (
               Piscina* piscina,
    constans character* fons,
                   b32* sani)
{
              MateriaNodus* radix;
           MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;
                        i32  mensura = (i32)strlen(fons);

    *sani = FALSUM;
    radix = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL: %s)\n", fons);
        redde NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &MD_REGISTRUM);
    consilium.fons_index = MD_FONS_PLAGULAE;
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    *sani = (b32)(emissa.successus && emissa.textus.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.textus.datum, fons, (size_t)mensura)
                == ZEPHYRUM));
    si (!*sani)
    {
        imprimere("    (octeti dispari: %s)\n", fons);
    }
    redde radix;
}

interior MateriaNodus*
_elementum (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v = materia_valor_lista_obtinere(nodus->loci[locus],
        i);

    redde (v != NIHIL
        && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus : NIHIL;
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(nodus->loci[locus]) : ZEPHYRUM;
}

/* inlinea blocci primi (paragraphus) */
interior MateriaNodus*
_inl (
    constans MateriaNodus* d)
{
    MateriaNodus* b = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);

    si (b == NIHIL)
    {
        redde NIHIL;
    }
    si (b->genus == (s32)MD_GENUS_PARAGRAPHUS)
    {
        redde b->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus;
    }
    si (b->genus == (s32)MD_GENUS_CAPITULUM)
    {
        redde b->loci[MD_CAPITULUM_INLINEA].datum.nodus;
    }
    redde NIHIL;
}

interior MateriaNodus*
_liber (
    constans MateriaNodus* inl,
                      i32  i)
{
    redde _elementum(inl, (i32)MD_INLINEA_LIBERI, i);
}

interior s32
_genus (
    constans MateriaNodus* inl,
                      i32  i)
{
    MateriaNodus* n = _liber(inl, i);

    redde n ? n->genus : (s32)-I;
}

/* lexema loci TOKEN == litterae */
interior b32
_lexema_aequalis (
    constans MateriaNodus* nodus,
                      i32  locus,
       constans character* litterae)
{
    constans MateriaToken* t;

    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    t = nodus->loci[locus].datum.token;
    redde (b32)(t->valor.mensura == (i32)strlen(litterae)
                && memcmp(t->valor.datum, litterae,
                (size_t)t->valor.mensura) == ZEPHYRUM);
}

/* concatenatio lexematum loci LISTA_TOKEN == litterae */
interior b32
_cruda_aequalia (
    constans MateriaNodus* nodus,
                      i32  locus,
       constans character* litterae)
{
    character buffer[DXII];
          i32 n = ZEPHYRUM;
          i32 m;
          i32 i;

    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    m = materia_valor_lista_numerus(nodus->loci[locus]);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MateriaToken* t =
            materia_valor_lista_obtinere(nodus->loci[locus],
            i)->datum.token;

        si (n + t->valor.mensura >= (i32)magnitudo(buffer))
        {
            redde FALSUM;
        }
        memcpy(buffer + n, t->valor.datum, (size_t)t->valor.mensura);
        n = n + t->valor.mensura;
    }
    redde (b32)(n == (i32)strlen(litterae)
        && memcmp(buffer, litterae, (size_t)n) == ZEPHYRUM);
}

interior b32
_adest (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (b32)(nodus != NIHIL
        && nodus->loci[locus].genus != MATERIA_VALOR_NIHIL);
}

interior s32
_index (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].datum.index;
}

#define P(lit) _parsa(piscina, (lit), &sani)

s32
principale (vacuum)
{
       b32  praeteritus;
   Piscina* piscina;
       b32  sani;
    MateriaNodus* d;
    MateriaNodus* in;
    MateriaNodus* n;
    MateriaNodus* m;

    piscina = piscina_generare_dynamicum("probatio_md_inlinea",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans textum, effugia, entia ---\n");
    d = P("plain text\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_TEXTUS);
    CREDO_FALSUM (_adest(n, (i32)MD_TEXTUS_VALOR));   /* nihil decoctum */
    d = P("\\*not\\* &amp; x\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_TEXTUS_CRUDUM,
        "\\*not\\* &amp; x"));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_TEXTUS_VALOR,
        "*not* & x"));
    CREDO_AEQUALIS_S32 (n->loci[MD_TEXTUS_VALOR].datum.token->fons_index,
        MD_FONS_DERIVATUS);
    /* spatia finalia lineae ultimae e valore sublata */
    d = P("foo   \n"); CREDO_VERUM (sani); in  = _inl(d);
    n                                          = _liber(in, ZEPHYRUM);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_TEXTUS_CRUDUM, "foo   "));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_TEXTUS_VALOR, "foo"));

    imprimere("\n--- Probans emphasin (par. 6.2) ---\n");
    d = P("*foo bar*\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_EMPHASIS);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_ORNATUS_APERTUM, "*"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_ORNATUS_LIBERI), I);
    d = P("a * foo bar*\n"); CREDO_VERUM (sani); in = _inl(d);   /* regula I: non */
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_TEXTUS);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    d = P("foo*bar*\n"); CREDO_VERUM (sani); in = _inl(d);   /* intra verbum '*' licet */
    CREDO_AEQUALIS_S32 (_genus(in, I), (s32)MD_GENUS_EMPHASIS);
    d = P("foo_bar_\n"); CREDO_VERUM (sani); in = _inl(d);   /* intra verbum '_' non */
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    d = P("**foo bar**\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_FORTIS);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_ORNATUS_CLAUSUM, "**"));
    d = P("***foo***\n"); CREDO_VERUM (sani); in = _inl(d);   /* em(strong) */
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_EMPHASIS);
    m = _elementum(n, (i32)MD_ORNATUS_LIBERI, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (m->genus, (s32)MD_GENUS_FORTIS);
    d = P("foo***bar***baz\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), III);
    CREDO_AEQUALIS_S32 (_genus(in, I), (s32)MD_GENUS_EMPHASIS);
    d = P("*foo**bar*\n"); CREDO_VERUM (sani); in = _inl(d);   /* regula III (multiplum III) */
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_EMPHASIS);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_ORNATUS_LIBERI), I);
    CREDO_VERUM (_cruda_aequalia(_elementum(n, (i32)MD_ORNATUS_LIBERI,
        ZEPHYRUM),
        (i32)MD_TEXTUS_CRUDUM, "foo**bar"));
    d = P("**foo*bar*baz**\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_FORTIS);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_ORNATUS_LIBERI), III);
    CREDO_AEQUALIS_S32 (_elementum(n, (i32)MD_ORNATUS_LIBERI, I)->genus,
        (s32)MD_GENUS_EMPHASIS);
    d = P("*foo\nbar*\n"); CREDO_VERUM (sani); in = _inl(d);   /* trans lineas */
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_EMPHASIS);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_ORNATUS_LIBERI), III);
    CREDO_AEQUALIS_S32 (_elementum(n, (i32)MD_ORNATUS_LIBERI, I)->genus,
        (s32)MD_GENUS_FRACTURA_MOLLIS);
    d = P("**\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_TEXTUS);

    imprimere("\n--- Probans deletionem (GFM) ---\n");
    d = P("~~del~~ ~x~ ~~~no~~~\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_DELETIO);
    CREDO_AEQUALIS_S32 (_genus(in, II), (s32)MD_GENUS_DELETIO);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), IV);   /* del, ' ', del, ' ~~~no~~~' */

    imprimere("\n--- Probans verbatim (par. 6.1) ---\n");
    d = P("`code`\n"); CREDO_VERUM (sani); in  = _inl(d);
    n                                          = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_VERBATIM);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_VERBATIM_CRUDUM, "code"));
    CREDO_FALSUM (_adest(n, (i32)MD_VERBATIM_VALOR));   /* idem crudo */
    d = P("` a `\n"); CREDO_VERUM (sani); in  = _inl(d);
    n                                         = _liber(in, ZEPHYRUM);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_VERBATIM_VALOR, "a"));
    d = P("`` ` ``\n"); CREDO_VERUM (sani); in  = _inl(d);
    n                                           = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_VERBATIM);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_VERBATIM_VALOR, "`"));
    d = P("`a\nb`\n"); CREDO_VERUM (sani); in  = _inl(d);   /* trans lineas: LINEA in crudo */
    n                                          = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_VERBATIM);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_VERBATIM_CRUDUM), III);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_VERBATIM_VALOR, "a b"));
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), I);
    d = P("`open\n"); CREDO_VERUM (sani); in = _inl(d);   /* non clausum: litterae */
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_TEXTUS);
    d = P("*foo`*`\n"); CREDO_VERUM (sani); in = _inl(d);   /* verbatim vincit */
    CREDO_AEQUALIS_S32 (_genus(in, I), (s32)MD_GENUS_VERBATIM);

    imprimere("\n--- Probans nexus (par. 6.3) ---\n");
    d = P("[a](/u \"t\")\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_AEQUALIS_S32 (_index(n, (i32)MD_NEXUS_FORMA),
        (s32)MD_NEXUS_INLINEUS);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_APERTUM, "["));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_NEXUS_LIBERI), I);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_NEXUS_CAUDA,
        "](/u \"t\")"));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL, "/u"));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_DESCRIPTIO, "t"));
    d = P("[a](<b c>) [d]()\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_VERUM (_lexema_aequalis(_liber(in, ZEPHYRUM),
        (i32)MD_NEXUS_URL, "b c"));
    CREDO_VERUM (_lexema_aequalis(_liber(in, II), (i32)MD_NEXUS_URL,
        ""));
    CREDO_FALSUM (_adest(_liber(in, II), (i32)MD_NEXUS_DESCRIPTIO));
    d = P("[a](/u\\)x) [b](/u \"t\\\"q\")\n"); CREDO_VERUM (sani); in =
                                                                       _inl(d);   /* effugia in url/descriptione */
    CREDO_VERUM (_lexema_aequalis(_liber(in, ZEPHYRUM),
        (i32)MD_NEXUS_URL, "/u)x"));
    CREDO_VERUM (_lexema_aequalis(_liber(in, II),
        (i32)MD_NEXUS_DESCRIPTIO, "t\"q"));
    d = P("[a] (/u)\n"); CREDO_VERUM (sani); in = _inl(d);   /* spatium: non nexus */
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_TEXTUS);
    d = P("*[a](/u)* [*b*](/u)\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_EMPHASIS);
    CREDO_AEQUALIS_S32 (_elementum(n, (i32)MD_ORNATUS_LIBERI,
        ZEPHYRUM)->genus, (s32)MD_GENUS_NEXUS);
    n = _liber(in, II);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_AEQUALIS_S32 (_elementum(n, (i32)MD_NEXUS_LIBERI,
        ZEPHYRUM)->genus, (s32)MD_GENUS_EMPHASIS);
    d = P("[a [b](/u) c](/v)\n"); CREDO_VERUM (sani); in = _inl(d);   /* nexus intra nexum non */
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_TEXTUS);
    CREDO_AEQUALIS_S32 (_genus(in, I), (s32)MD_GENUS_NEXUS);
    d = P("![i](/p.png \"alt\")\n"); CREDO_VERUM (sani); in = _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_IMAGO);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_APERTUM, "!["));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL, "/p.png"));

    imprimere("\n--- Probans nexus referentes ---\n");
    d =
                                                                                             P("[foo] [Foo][] [bar][foo] [nope]\n\n[FOO]: /url \"t\"\n"); CREDO_VERUM (sani); in =
                                                                                                                                                                                  _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_AEQUALIS_S32 (_index(n, (i32)MD_NEXUS_FORMA),
        (s32)MD_NEXUS_REFERENS);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL, "/url"));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_DESCRIPTIO, "t"));
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_NEXUS_CAUDA, "]"));
    n = _liber(in, II);   /* collapsus */
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_NEXUS_CAUDA, "][]"));
    n = _liber(in, IV);   /* plenus */
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_VERUM (_cruda_aequalia(n, (i32)MD_NEXUS_CAUDA, "][foo]"));
    /* [nope] litterae: '[' 'nope' ']' itema lexematum cum spatio praecedenti in
     * textum UNUM coalescunt */
    CREDO_AEQUALIS_S32 (_genus(in, V), (s32)MD_GENUS_TEXTUS);
    CREDO_VERUM (_cruda_aequalia(_liber(in, V), (i32)MD_TEXTUS_CRUDUM,
        " [nope]"));
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), VI);

    imprimere("\n--- Probans autonexus ---\n");
    d =
                                                                                               P("<http://x.y/z> <a@b.co> www.x.com/p. http://q.r/s) ftp\n"); CREDO_VERUM (sani); in =
                                                                                                                                                                                      _inl(d);
    n = _liber(in, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_AEQUALIS_S32 (_index(n, (i32)MD_NEXUS_FORMA),
        (s32)MD_NEXUS_AUTO);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL,
        "http://x.y/z"));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_APERTUM, "<"));
    n = _liber(in, II);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL,
        "mailto:a@b.co"));
    n = _liber(in, IV);   /* GFM nudus, punctum finale exclusum */
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_FALSUM (_adest(n, (i32)MD_NEXUS_APERTUM));
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL,
        "http://www.x.com/p"));
    n = _liber(in, VI);   /* ')' dispar exclusa */
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_NEXUS);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_NEXUS_URL,
        "http://q.r/s"));

    imprimere("\n--- Probans fracturas (par. 6.7, 6.8) ---\n");
    d = P("a  \nb\\\nc\nd\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_I32 (_numerus(in, (i32)MD_INLINEA_LIBERI), VII);
    n = _liber(in, I);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_FRACTURA_DURA);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_DURA_SIGNUM, "  "));
    CREDO_VERUM (_adest(n, (i32)MD_DURA_FINIS));
    n = _liber(in, III);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_FRACTURA_DURA);
    CREDO_VERUM (_lexema_aequalis(n, (i32)MD_DURA_SIGNUM, "\\"));
    CREDO_AEQUALIS_S32 (_genus(in, V), (s32)MD_GENUS_FRACTURA_MOLLIS);
    CREDO_VERUM (_cruda_aequalia(_liber(in, ZEPHYRUM),
        (i32)MD_TEXTUS_CRUDUM, "a"));
    d = P("> a  \n> b\n"); CREDO_VERUM (sani);   /* praefixa in fractura dura */
    n = _elementum(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM),
        (i32)MD_CITATIO_BLOCI, ZEPHYRUM);
    in  = n->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus;
    n   = _liber(in, I);
    CREDO_AEQUALIS_S32 (n->genus, (s32)MD_GENUS_FRACTURA_DURA);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)MD_DURA_PRAEFIXA), I);

    imprimere("\n--- Probans html inlineum (par. 6.6) ---\n");
    d =
                                                                                  P("<b class=\"x\">y</b> <!-- c --> <?p?> a<b\n"); CREDO_VERUM (sani); in =
                                                                                                                                                            _inl(d);
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM),
        (s32)MD_GENUS_HTML_INLINEUM);
    CREDO_VERUM (_cruda_aequalia(_liber(in, ZEPHYRUM),
        (i32)MD_HTML_INLINEUM_CRUDUM, "<b class=\"x\">"));
    CREDO_AEQUALIS_S32 (_genus(in, II), (s32)MD_GENUS_HTML_INLINEUM);
    CREDO_AEQUALIS_S32 (_genus(in, IV), (s32)MD_GENUS_HTML_INLINEUM);
    CREDO_AEQUALIS_S32 (_genus(in, VI), (s32)MD_GENUS_HTML_INLINEUM);
    CREDO_AEQUALIS_S32 (_genus(in, VII), (s32)MD_GENUS_TEXTUS);   /* ' a<b' litterae */

    imprimere("\n--- Probans capitula et cellas ---\n");
    d = P("# *t* `c`\n"); CREDO_VERUM (sani); in = _inl(d);
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_EMPHASIS);
    CREDO_AEQUALIS_S32 (_genus(in, II), (s32)MD_GENUS_VERBATIM);
    d = P("| *a* | b |\n| - | - |\n"); CREDO_VERUM (sani);
    n   = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
    n   = n->loci[MD_TABULA_CAPUT].datum.nodus;
    n   = _elementum(n, (i32)MD_ORDO_CELLAE, ZEPHYRUM);
    in  = n->loci[MD_CELLA_INLINEA].datum.nodus;
    CREDO_AEQUALIS_S32 (_genus(in, ZEPHYRUM), (s32)MD_GENUS_EMPHASIS);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
