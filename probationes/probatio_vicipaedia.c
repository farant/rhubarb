/* probatio_vicipaedia.c - adaptator Vicipaediae (navigator N1): HTML
 * paginae REST (Parsoid) -> Articulus. Fixa veri articuli
 * (probationes/fixa/vicipaedia/, FONS.md): Rhubarb integer, Thomas
 * Aquinas sectionibus 0, 1, 52 servatis. Exspectata ex fontibus lecta
 * (scriptum Pythonis in spec navigator X), non ex exitu adaptatoris. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "articulus.h"
#include "vicipaedia.h"
#include "credo.h"
#include <string.h>

#define VIA_FIXORUM "probationes/fixa/vicipaedia/"

/* spatium non frangens (U+00A0), spatium tenue (U+2009) */
#define INSECABILE "\xC2\xA0"
#define TENUE      "\xE2\x80\x89"

interior Articulus*
articulum_legere (
    constans character* fixum,
    constans character* fons,
               Piscina* p)
{
    character via[CCLVI];
       chorda html;

    strcpy(via, VIA_FIXORUM);
    strcat(via, fixum);
    html = filum_legere_totum(via, p);
    si (html.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde vicipaedia_articulus(html, fons, p);
}

interior ArticulusSectio*
sectio (
    Articulus* a,
          i32  i)
{
    redde (a && a->sectiones && i < xar_numerus(a->sectiones))
        ? (ArticulusSectio*)xar_obtinere(a->sectiones, i) : NIHIL;
}

interior ArticulusSectio*
sectionem_invenire (
             Articulus* a,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; a && i < xar_numerus(a->sectiones); i++)
    {
        si (chorda_aequalis_literis(sectio(a, i)->titulus, titulus))
        {
            redde sectio(a, i);
        }
    }
    redde NIHIL;
}

interior ArticulusClausula*
clausula (
    Xar* clausulae,
    i32  i)
{
    redde (clausulae && i < xar_numerus(clausulae))
        ? (ArticulusClausula*)xar_obtinere(clausulae, i) : NIHIL;
}

/* clausula prima generis dati cuius textus praefixo incipit */
interior ArticulusClausula*
clausulam_invenire (
                    Xar* clausulae,
         ArticulusGenus  genus,
     constans character* praefixum,
                Piscina* p)
{
    ArticulusClausula* k;
                  i32  i;

    per (i = ZEPHYRUM; clausulae && i < xar_numerus(clausulae); i++)
    {
        k = clausula(clausulae, i);
        si (   k->genus == genus
            && chorda_incipit(k->textus,
                   chorda_ex_literis(praefixum, p)))
        {
            redde k;
        }
    }
    redde NIHIL;
}

/* valor PAR cuius clavis data; NIHIL-chorda si abest */
interior b32
par_est (
    ArticulusAdiunctum* ad,
    constans character* clavis,
    constans character* valor)
{
    ArticulusClausula* k;
                  i32  i;

    per (i = ZEPHYRUM; ad && i < xar_numerus(ad->clausulae); i++)
    {
        k = clausula(ad->clausulae, i);
        si (   k->genus == ARTICULUS_PAR
            && chorda_aequalis_literis(k->clavis, clavis))
        {
            redde chorda_aequalis_literis(k->textus, valor);
        }
    }
    redde FALSUM;
}

interior b32
notam_habet (
          ArticulusClausula* k,
         ArticulusNotaGenus  genus,
                        i32  initium,
                        i32  finis,
         constans character* scopus)
{
    ArticulusNota* n;
              i32  i;

    per (i = ZEPHYRUM; k && k->notae && i < xar_numerus(k->notae); i++)
    {
        n = (ArticulusNota*)xar_obtinere(k->notae, i);
        si (   n->genus == genus && n->initium == initium
            && n->finis == finis
            && (!scopus || chorda_aequalis_literis(n->scopus, scopus)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus aliquis (clausulae, claves, adiuncta) chordam continet? */
interior b32
textus_continet (
               Articulus* a,
      constans character* quaesita,
                 Piscina* p)
{
        ArticulusSectio* s;
     ArticulusAdiunctum* ad;
      ArticulusClausula* k;
                 chorda  q;
                    i32  i;
                    i32  j;
                    i32  m;

    q = chorda_ex_literis(quaesita, p);
    per (i = ZEPHYRUM; a && i < xar_numerus(a->sectiones); i++)
    {
        s = sectio(a, i);
        per (j = ZEPHYRUM; j < xar_numerus(s->clausulae); j++)
        {
            k = clausula(s->clausulae, j);
            si (   chorda_continet(k->textus, q)
                || chorda_continet(k->clavis, q))
            {
                redde VERUM;
            }
        }
        per (j = ZEPHYRUM; s->adiuncta && j < xar_numerus(s->adiuncta);
             j++)
        {
            ad = (ArticulusAdiunctum*)xar_obtinere(s->adiuncta, j);
            per (m = ZEPHYRUM; m < xar_numerus(ad->clausulae); m++)
            {
                k = clausula(ad->clausulae, m);
                si (   chorda_continet(k->textus, q)
                    || chorda_continet(k->clavis, q))
                {
                    redde VERUM;
                }
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_probare_rheum (
    CredoContextus* c)
{
             Articulus* a;
       ArticulusSectio* s;
    ArticulusAdiunctum* ad;
     ArticulusClausula* k;

    a = articulum_legere("rhubarb.html",
        "https://en.wikipedia.org/wiki/Rhubarb", c->piscina);
    CREDO_NECESSE(a != NIHIL);

    CREDO_GRADUS("titulus, fons, prooemium primum");
    CREDO_CHORDA_AEQUALIS_LITERIS(a->titulus, "Rhubarb");
    CREDO_CHORDA_AEQUALIS_LITERIS(a->fons,
        "https://en.wikipedia.org/wiki/Rhubarb");
    s = sectio(a, ZEPHYRUM);
    CREDO_NECESSE(s != NIHIL);
    CREDO_AEQUALIS_I32(s->titulus.mensura, ZEPHYRUM);

    CREDO_GRADUS("sectiones ordine; References et sequentes omissae");
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, I)->titulus, "Etymology");
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, I)->ancora, "Etymology");
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, II)->titulus,
        "Cultivation");
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, III)->titulus, "Cultivars");
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, IV)->titulus, "Uses");
    CREDO_VERUM(sectionem_invenire(a,
        "Phytochemistry and potential toxicity") != NIHIL);
    CREDO_VERUM(sectionem_invenire(a, "Pests") != NIHIL);
    CREDO_VERUM(sectionem_invenire(a, "References") == NIHIL);
    CREDO_VERUM(sectionem_invenire(a, "Further reading") == NIHIL);
    CREDO_VERUM(sectionem_invenire(a, "External links") == NIHIL);

    CREDO_GRADUS("hatnota prima, deinde paragraphus cum notis");
    k = clausula(s->clausulae, ZEPHYRUM);
    CREDO_NECESSE(k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus, "For other uses, see "
        "Rhubarb (disambiguation). For the cartoon, see Roobarb.");
    k = clausulam_invenire(s->clausulae, ARTICULUS_PARAGRAPHUS,
        "Rhubarb is the fleshy", c->piscina);
    CREDO_NECESSE(k != NIHIL);
    CREDO_VERUM(chorda_incipit(k->textus, chorda_ex_literis(
        "Rhubarb is the fleshy, edible stalks (petioles) of species "
        "and hybrids (culinary rhubarb) of Rheum in the family "
        "Polygonaceae, which are cooked and used for food. The plant",
        c->piscina)));
    CREDO_VERUM(notam_habet(k, ARTICULUS_FORTIS, ZEPHYRUM, VII, NIHIL));
    CREDO_VERUM(notam_habet(k, ARTICULUS_NEXUS, XXXVIII, XLVI,
        "https://en.wikipedia.org/wiki/Petiole_(botany)"));
    CREDO_VERUM(notam_habet(k, ARTICULUS_EMPHASIS, XCIII, XCVIII,
        NIHIL));
    CREDO_VERUM(notam_habet(k, ARTICULUS_NEXUS, XCIII, XCVIII,
        "https://en.wikipedia.org/wiki/Rheum_(plant)"));
    CREDO_VERUM(notam_habet(k, ARTICULUS_NEXUS, CXIII, CXXV,
        "https://en.wikipedia.org/wiki/Polygonaceae"));

    CREDO_GRADUS("infobox taxi: adiunctum prooemii");
    CREDO_NECESSE(s->adiuncta && xar_numerus(s->adiuncta) == I);
    ad = (ArticulusAdiunctum*)xar_obtinere(s->adiuncta, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(ad->titulus,
        "Rhubarb (culinary hybrid)");
    CREDO_VERUM(clausulam_invenire(ad->clausulae, ARTICULUS_TITULUS,
        "Scientific classification", c->piscina) != NIHIL);
    CREDO_VERUM(par_est(ad, "Kingdom", "Plantae"));
    CREDO_VERUM(par_est(ad, "Family", "Polygonaceae"));

    CREDO_GRADUS("infobox nutritionis: adiunctum sectionis Uses");
    s = sectionem_invenire(a, "Uses");
    CREDO_NECESSE(s && s->adiuncta && xar_numerus(s->adiuncta) == I);
    ad = (ArticulusAdiunctum*)xar_obtinere(s->adiuncta, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(ad->titulus, "Rhubarb, raw");
    CREDO_VERUM(clausulam_invenire(ad->clausulae, ARTICULUS_TITULUS,
        "Nutritional value per 100" INSECABILE "g (3.5" INSECABILE
        "oz)",
        c->piscina) != NIHIL);
    CREDO_VERUM(par_est(ad, "Energy",
        "88" INSECABILE "kJ (21" INSECABILE "kcal)"));
    CREDO_VERUM(par_est(ad, "Dietary fiber", "1.8 g"));
    CREDO_VERUM(clausulam_invenire(s->clausulae, ARTICULUS_TITULUS,
        "Nutrition", c->piscina) != NIHIL);

    CREDO_GRADUS("figura: URL absoluta (via retis //thumb...), "
        "titulus");
    k = clausulam_invenire(sectionem_invenire(a, "Cultivation")
        ->clausulae, ARTICULUS_FIGURA, "Young rhubarb flowers",
        c->piscina);
    CREDO_NECESSE(k != NIHIL);
    CREDO_VERUM(chorda_incipit(k->clavis, chorda_ex_literis(
        "https://thumb.wikimedia.org/wikipedia/commons/thumb/1/12/"
        "Rheum_rhabarbarum_%28Rabarber%29_Frambozenrood_bloeiwijze.jpg/"
        "250px-", c->piscina)));
    CREDO_VERUM(chorda_continet(k->clavis, chorda_ex_literis(
        "&utm_campaign=parser", c->piscina)));

    CREDO_GRADUS("nullae notae referentiarum, nullus stylus");
    CREDO_FALSUM(textus_continet(a, "[1]", c->piscina));
    CREDO_FALSUM(textus_continet(a, "[2]", c->piscina));
    CREDO_FALSUM(textus_continet(a, "mw-parser-output", c->piscina));
}

interior vacuum
_probare_aquinas (
    CredoContextus* c)
{
             Articulus* a;
       ArticulusSectio* s;
    ArticulusAdiunctum* ad;
     ArticulusClausula* k;

    a = articulum_legere("thomas_aquinas.html",
        "https://en.wikipedia.org/wiki/Thomas_Aquinas", c->piscina);
    CREDO_NECESSE(a != NIHIL);

    CREDO_GRADUS("titulus; prooemium et Biography (External links "
        "omissa)");
    CREDO_CHORDA_AEQUALIS_LITERIS(a->titulus, "Thomas Aquinas");
    CREDO_AEQUALIS_I32(xar_numerus(a->sectiones), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(sectio(a, I)->titulus, "Biography");

    CREDO_GRADUS("sectiones interiores: tituli gradus III in "
        "Biography");
    s = sectio(a, I);
    k = clausulam_invenire(s->clausulae, ARTICULUS_TITULUS,
        "Early life (1225\xE2\x80\x93" "1244)", c->piscina);
    CREDO_NECESSE(k != NIHIL);
    CREDO_AEQUALIS_S32(k->gradus, III);
    k = clausulam_invenire(s->clausulae, ARTICULUS_TITULUS,
        "Late career, cessation of writing, and death", c->piscina);
    CREDO_VERUM(k != NIHIL && k->gradus == III);

    CREDO_GRADUS("infobox personae: PAR, lineae per <br> et indicem");
    s = sectio(a, ZEPHYRUM);
    CREDO_NECESSE(s->adiuncta && xar_numerus(s->adiuncta) >= I);
    ad = (ArticulusAdiunctum*)xar_obtinere(s->adiuncta, ZEPHYRUM);
    CREDO_VERUM(par_est(ad, "Born",
        "c." TENUE "1225\nRoccasecca, Kingdom of Sicily"));
    CREDO_VERUM(par_est(ad, "Venerated" INSECABILE "in",
        "Catholic Church\nAnglican Communion\nLutheranism"));
    CREDO_VERUM(par_est(ad, "Major shrine",
        "Church of the Jacobins, Toulouse, France"));
}

/* HTML minimum: fragmenta sine sectionibus, entitates, indices */
interior vacuum
_probare_minimum (
    CredoContextus* c)
{
            Articulus* a;
      ArticulusSectio* s;
    ArticulusClausula* k;

    a = vicipaedia_articulus(chorda_ex_literis(
        "<html><head><title>A &amp; B</title></head><body>"
        "<section data-mw-section-id=\"0\"><p>Alpha &amp;  beta"
        "<sup class=\"mw-ref reference\"><a href=\"#n\">[1]</a></sup>"
        "</p><ul><li>unus<ul><li>duo</li></ul></li></ul>"
        "<blockquote><p>Verbum</p></blockquote>"
        "<div class=\"navbox\"><p>NAVBOX</p></div>"
        "<style>.x{color:red}</style></section>"
        "<section data-mw-section-id=\"1\"><h2 id=\"Vacua\">Vacua</h2>"
        "</section></body></html>", c->piscina),
        "https://en.wikipedia.org/wiki/A", c->piscina);
    CREDO_NECESSE(a != NIHIL);

    CREDO_GRADUS("titulus decoctus; sectio vacua omissa");
    CREDO_CHORDA_AEQUALIS_LITERIS(a->titulus, "A & B");
    CREDO_AEQUALIS_I32(xar_numerus(a->sectiones), I);

    CREDO_GRADUS("paragraphus: entitas, spatia contracta, sine [1]");
    s = sectio(a, ZEPHYRUM);
    k = clausula(s->clausulae, ZEPHYRUM);
    CREDO_NECESSE(k != NIHIL);
    CREDO_AEQUALIS_I32((i32)k->genus, (i32)ARTICULUS_PARAGRAPHUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->textus, "Alpha & beta");

    CREDO_GRADUS("index: gradus per profunditatem; citatio");
    k = clausula(s->clausulae, I);
    CREDO_VERUM(k && k->genus == ARTICULUS_INDEX && k->gradus == I
        && chorda_aequalis_literis(k->textus, "unus"));
    k = clausula(s->clausulae, II);
    CREDO_VERUM(k && k->genus == ARTICULUS_INDEX && k->gradus == II
        && chorda_aequalis_literis(k->textus, "duo"));
    k = clausula(s->clausulae, III);
    CREDO_VERUM(k && k->genus == ARTICULUS_CITATIO
        && chorda_aequalis_literis(k->textus, "Verbum"));

    CREDO_GRADUS("navbox et style omissa");
    CREDO_AEQUALIS_I32(xar_numerus(s->clausulae), IV);
    CREDO_FALSUM(textus_continet(a, "NAVBOX", c->piscina));
    CREDO_FALSUM(textus_continet(a, "color", c->piscina));
}

/* sedes ex URL: VERUM et campi exspectati */
interior b32
sedes_est (
    constans character* url,
    constans character* hospes,
    constans character* titulus,
    constans character* ancora,
               Piscina* p)
{
    VicipaediaSedes s;

    si (!vicipaedia_sedem_legere(chorda_ex_literis(url, p), &s, p))
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(s.hospes, hospes)
        && chorda_aequalis_literis(s.titulus, titulus)
        && chorda_aequalis_literis(s.ancora, ancora);
}

interior b32
recusata (
    constans character* url,
               Piscina* p)
{
    VicipaediaSedes s;

    redde !vicipaedia_sedem_legere(chorda_ex_literis(url, p), &s, p);
}

/* URL articuli et fontis titulo dato (en.wikipedia.org) */
interior b32
inscriptiones_sunt (
    constans character* titulus,
    constans character* articuli,
    constans character* fontis,
               Piscina* p)
{
    VicipaediaSedes s;

    s.hospes   = chorda_ex_literis("en.wikipedia.org", p);
    s.titulus  = chorda_ex_literis(titulus, p);
    s.ancora   = chorda_ex_literis("", p);
    redde chorda_aequalis_literis(vicipaedia_url_articuli(&s, p),
        articuli) && chorda_aequalis_literis(vicipaedia_url_fontis(&s,
        p), fontis);
}

/* URL ex sedibus et retro: eadem sedes */
interior b32
circuitus (
    constans character* titulus,
               Piscina* p)
{
    VicipaediaSedes s;
    VicipaediaSedes r;

    s.hospes   = chorda_ex_literis("en.wikipedia.org", p);
    s.titulus  = chorda_ex_literis(titulus, p);
    s.ancora   = chorda_ex_literis("", p);
    redde vicipaedia_sedem_legere(vicipaedia_url_articuli(&s, p), &r, p)
        && chorda_aequalis(r.titulus, s.titulus)
        && chorda_aequalis(r.hospes, s.hospes)
        && vicipaedia_sedem_legere(vicipaedia_url_fontis(&s, p), &r, p)
        && chorda_aequalis(r.titulus, s.titulus);
}

interior vacuum
_probare_sedes (
    CredoContextus* c)
{
    Piscina* p;

    p = c->piscina;

    CREDO_GRADUS("formae eiusdem articuli");
    CREDO_VERUM(sedes_est(
        "https://en.wikipedia.org/wiki/Thomas_Aquinas",
        "en.wikipedia.org", "Thomas_Aquinas", "", p));
    CREDO_VERUM(sedes_est(
        "https://en.m.wikipedia.org/wiki/Thomas_Aquinas#Early_life",
        "en.wikipedia.org", "Thomas_Aquinas", "Early_life", p));
    CREDO_VERUM(sedes_est(
        "https://en.wikipedia.org/api/rest_v1/page/html/Thomas_Aquinas",
        "en.wikipedia.org", "Thomas_Aquinas", "", p));
    CREDO_VERUM(sedes_est("https://EN.Wikipedia.ORG/wiki/Rhubarb",
        "en.wikipedia.org", "Rhubarb", "", p));
    CREDO_VERUM(sedes_est("http://en.wikipedia.org/wiki/Rhubarb",
        "en.wikipedia.org", "Rhubarb", "", p));

    CREDO_GRADUS("titulus decodicatus, spatia -> _, quaestio neglecta");
    CREDO_VERUM(sedes_est("https://la.wikipedia.org/wiki/Caf%C3%A9?a=b",
        "la.wikipedia.org", "Caf\xC3\xA9", "", p));
    CREDO_VERUM(sedes_est(
        "https://en.wikipedia.org/wiki/Thomas%20Aquinas",
        "en.wikipedia.org", "Thomas_Aquinas", "", p));
    CREDO_VERUM(sedes_est("https://en.wikipedia.org/wiki/C++",
        "en.wikipedia.org", "C++", "", p));
    CREDO_VERUM(sedes_est("https://en.wikipedia.org/wiki/AC/DC",
        "en.wikipedia.org", "AC/DC", "", p));
    CREDO_VERUM(sedes_est(
        "https://simple.wikipedia.org/wiki/Rheum_(plant)",
        "simple.wikipedia.org", "Rheum_(plant)", "", p));
    CREDO_VERUM(sedes_est(
        "https://be-tarask.wikipedia.org/wiki/X#Caf%C3%A9",
        "be-tarask.wikipedia.org", "X", "Caf\xC3\xA9", p));

    CREDO_GRADUS("recusata");
    CREDO_VERUM(recusata("https://wikipedia.org.evil.com/wiki/X", p));
    CREDO_VERUM(recusata("https://en.wikipedia.org.evil.com/wiki/X",
        p));
    CREDO_VERUM(recusata("https://evilwikipedia.org/wiki/X", p));
    CREDO_VERUM(recusata("https://wikipedia.org/wiki/X", p));
    CREDO_VERUM(recusata("https://a.b.wikipedia.org/wiki/X", p));
    CREDO_VERUM(recusata("https://en.wikipedia.org/wiki/", p));
    CREDO_VERUM(recusata("https://en.wikipedia.org/w/index.php?title=X",
        p));
    CREDO_VERUM(recusata("https://example.com/wiki/X", p));
    CREDO_VERUM(recusata("ftp://en.wikipedia.org/wiki/X", p));
    CREDO_VERUM(recusata("./X", p));

    CREDO_GRADUS("URL articuli et fontis codificatae");
    CREDO_VERUM(inscriptiones_sunt("Thomas_Aquinas",
        "https://en.wikipedia.org/wiki/Thomas_Aquinas",
        "https://en.wikipedia.org/api/rest_v1/page/html/Thomas_Aquinas",
        p));
    CREDO_VERUM(inscriptiones_sunt("C++",
        "https://en.wikipedia.org/wiki/C%2B%2B",
        "https://en.wikipedia.org/api/rest_v1/page/html/C%2B%2B", p));
    CREDO_VERUM(inscriptiones_sunt("AC/DC",
        "https://en.wikipedia.org/wiki/AC%2FDC",
        "https://en.wikipedia.org/api/rest_v1/page/html/AC%2FDC", p));
    CREDO_VERUM(inscriptiones_sunt("Rheum_(plant)",
        "https://en.wikipedia.org/wiki/Rheum_%28plant%29",
        "https://en.wikipedia.org/api/rest_v1/page/html/"
        "Rheum_%28plant%29",
        p));

    CREDO_GRADUS("circuitus URL -> sedes");
    CREDO_VERUM(circuitus("C++", p));
    CREDO_VERUM(circuitus("AC/DC", p));
    CREDO_VERUM(circuitus("Caf\xC3\xA9_au_lait", p));
    CREDO_VERUM(circuitus("Rheum_(plant)", p));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "minimum", _probare_minimum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rhubarb", _probare_rheum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aquinas", _probare_aquinas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sedes", _probare_sedes, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicipaedia", SECTIONES);
}
