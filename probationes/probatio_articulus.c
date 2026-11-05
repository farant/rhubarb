/* probatio_articulus.c - forma canonica et sigillum articuli
 * (navigator N2.1): idem contentum -> idem sigillum (etiam per URL
 * aliam); mutatio quaelibet contenti -> sigillum aliud; campi
 * longitudine praefixi, ergo divisiones diversae non confunduntur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "articulus.h"
#include "vicipaedia.h"
#include "credo.h"
#include <string.h>

interior Articulus*
articulum_creare (
    constans character* titulus,
               Piscina* p)
{
    Articulus* a;

    a           = (Articulus*)piscina_allocare(p, magnitudo(Articulus));
    a->titulus  = chorda_ex_literis(titulus, p);
    a->fons      = chorda_ex_literis("https://la.wikipedia.org/wiki/X",
        p);
    a->sectiones = xar_creare(p, magnitudo(ArticulusSectio));
    redde a;
}

interior ArticulusSectio*
sectionem_addere (
             Articulus* a,
    constans character* titulus,
               Piscina* p)
{
    ArticulusSectio* s;

    s             = (ArticulusSectio*)xar_addere(a->sectiones);
    s->titulus    = chorda_ex_literis(titulus, p);
    s->ancora     = chorda_ex_literis(titulus, p);
    s->clausulae  = xar_creare(p, magnitudo(ArticulusClausula));
    s->adiuncta   = xar_creare(p, magnitudo(ArticulusAdiunctum));
    redde s;
}

interior ArticulusClausula*
clausulam_addere (
                    Xar* clausulae,
         ArticulusGenus  genus,
                    s32  gradus,
     constans character* clavis,
     constans character* textus,
                Piscina* p)
{
    ArticulusClausula* k;

    k          = (ArticulusClausula*)xar_addere(clausulae);
    k->genus   = genus;
    k->gradus  = gradus;
    k->clavis  = chorda_ex_literis(clavis, p);
    k->textus  = chorda_ex_literis(textus, p);
    k->notae   = xar_creare(p, magnitudo(ArticulusNota));
    redde k;
}

interior ArticulusNota*
notam_addere (
     ArticulusClausula* k,
    ArticulusNotaGenus  genus,
                   i32  initium,
                   i32  finis,
    constans character* scopus,
               Piscina* p)
{
    ArticulusNota* n;

    n           = (ArticulusNota*)xar_addere(k->notae);
    n->genus    = genus;
    n->initium  = initium;
    n->finis    = finis;
    n->scopus   = chorda_ex_literis(scopus, p);
    redde n;
}

interior ArticulusAdiunctum*
adiunctum_addere (
       ArticulusSectio* s,
    constans character* titulus,
               Piscina* p)
{
    ArticulusAdiunctum* d;

    d             = (ArticulusAdiunctum*)xar_addere(s->adiuncta);
    d->titulus    = chorda_ex_literis(titulus, p);
    d->clausulae  = xar_creare(p, magnitudo(ArticulusClausula));
    redde d;
}

/* exemplar: prooemium (paragraphus cum nexu, adiunctum taxonis),
 * sectio "Usus" (titulus III, index) */
interior Articulus*
exemplar (
    Piscina* p)
{
             Articulus* a;
       ArticulusSectio* s;
     ArticulusClausula* k;
    ArticulusAdiunctum* d;

    a = articulum_creare("Rheum", p);
    s = sectionem_addere(a, "", p);
    k = clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM,
        "", "Rheum est planta.", p);
    notam_addere(k, ARTICULUS_NEXUS, ZEPHYRUM, V,
        "https://la.wikipedia.org/wiki/Rheum", p);
    d = adiunctum_addere(s, "Taxon", p);
    clausulam_addere(d->clausulae, ARTICULUS_PAR, ZEPHYRUM, "Familia",
        "Polygonaceae", p);
    s = sectionem_addere(a, "Usus", p);
    clausulam_addere(s->clausulae, ARTICULUS_TITULUS, III, "",
        "Coquina", p);
    clausulam_addere(s->clausulae, ARTICULUS_INDEX, I, "", "Placenta",
        p);
    redde a;
}

interior ArticulusSectio*
sectio (
    Articulus* a,
          i32  i)
{
    redde (ArticulusSectio*)xar_obtinere(a->sectiones, i);
}

interior ArticulusClausula*
clausula (
    Xar* clausulae,
    i32  i)
{
    redde (ArticulusClausula*)xar_obtinere(clausulae, i);
}

/* sigillum ut chorda (piscina) */
interior chorda
sigillum (
    constans Articulus* a,
               Piscina* p)
{
    character hex[SIGILLUM_HEX_MENSURA];

    articulus_sigillum(a, hex);
    redde chorda_ex_literis(hex, p);
}

interior b32
sigilla_aequalia (
    constans Articulus* a,
    constans Articulus* b,
               Piscina* p)
{
    redde chorda_aequalis(sigillum(a, p), sigillum(b, p));
}

interior vacuum
_probare_canonicum (
    CredoContextus* c)
{
             Articulus* a;
       ArticulusSectio* s;
     ArticulusClausula* k;
    ArticulusAdiunctum* d;
                chorda  forma;
              Sigillum  sg;
             character  hex[SIGILLUM_HEX_MENSURA];

    CREDO_GRADUS("forma parva ad litteram");
    a = articulum_creare("A & B", c->piscina);
    s = sectionem_addere(a, "", c->piscina);
    k = clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM,
        "", "ab", c->piscina);
    notam_addere(k, ARTICULUS_EMPHASIS, ZEPHYRUM, I, "", c->piscina);
    d = adiunctum_addere(s, "T", c->piscina);
    clausulam_addere(d->clausulae, ARTICULUS_PAR, ZEPHYRUM, "k", "v",
        c->piscina);
    forma = articulus_canonicum(a, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(forma,
        "A5:A & B\n"
        "S0:0:\n"
        "K1;0;0:2:ab\n"
        "N1;0;1;0:\n"
        "D1:T\n"
        "K4;0;1:k1:v\n");

    CREDO_GRADUS("sigillum = SHA-256 formae canonicae");
    sg = sigillum_computare(forma.datum, (memoriae_index)forma.mensura);
    sigillum_hex(&sg, hex);
    CREDO_CHORDA_AEQUALIS_LITERIS(sigillum(a, c->piscina), hex);

    CREDO_GRADUS("indices et notae NIHIL = vacua");
    k->notae     = NIHIL;
    s->adiuncta  = NIHIL;
    CREDO_CHORDA_AEQUALIS_LITERIS(articulus_canonicum(a, c->piscina),
        "A5:A & B\n"
        "S0:0:\n"
        "K1;0;0:2:ab\n");
}

interior vacuum
_probare_fons (
    CredoContextus* c)
{
    Articulus* a;
    Articulus* b;

    CREDO_GRADUS("fons non intrat");
    a = exemplar(c->piscina);
    b = exemplar(c->piscina);
    b->fons = chorda_ex_literis("https://la.wikipedia.org/wiki/Y",
        c->piscina);
    CREDO_VERUM(sigilla_aequalia(a, b, c->piscina));
}

interior vacuum
_probare_mutationes (
    CredoContextus* c)
{
             Articulus* a;
             Articulus* b;
     ArticulusClausula* k;
         ArticulusNota* n;
    ArticulusAdiunctum* d;

    a = exemplar(c->piscina);

    CREDO_GRADUS("verbum unum");
    b          = exemplar(c->piscina);
    k          = clausula(sectio(b, ZEPHYRUM)->clausulae, ZEPHYRUM);
    k->textus  = chorda_ex_literis("Rheum est herba.", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("scopus nexus");
    b = exemplar(c->piscina);
    k = clausula(sectio(b, ZEPHYRUM)->clausulae, ZEPHYRUM);
    n = (ArticulusNota*)xar_obtinere(k->notae, ZEPHYRUM);
    n->scopus = chorda_ex_literis("https://la.wikipedia.org/wiki/Herba",
        c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("fines notae");
    b         = exemplar(c->piscina);
    k         = clausula(sectio(b, ZEPHYRUM)->clausulae, ZEPHYRUM);
    n         = (ArticulusNota*)xar_obtinere(k->notae, ZEPHYRUM);
    n->finis  = IV;
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("valor adiuncti");
    b = exemplar(c->piscina);
    d = (ArticulusAdiunctum*)xar_obtinere(sectio(b, ZEPHYRUM)->adiuncta,
        ZEPHYRUM);
    clausula(d->clausulae, ZEPHYRUM)->textus = chorda_ex_literis(
        "Rosaceae", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("gradus tituli");
    b = exemplar(c->piscina);
    clausula(sectio(b, I)->clausulae, ZEPHYRUM)->gradus = IV;
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("genus clausulae");
    b = exemplar(c->piscina);
    clausula(sectio(b, I)->clausulae, I)->genus = ARTICULUS_PARAGRAPHUS;
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("ancora sectionis");
    b                     = exemplar(c->piscina);
    sectio(b, I)->ancora  = chorda_ex_literis("Usus_2", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("titulus articuli");
    b           = exemplar(c->piscina);
    b->titulus  = chorda_ex_literis("Rhabarbarum", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));
}

interior vacuum
_probare_ambiguitas (
    CredoContextus* c)
{
          Articulus* a;
          Articulus* b;
    ArticulusSectio* s;

    CREDO_GRADUS("\"ab\",\"c\" contra \"a\",\"bc\"");
    a = articulum_creare("X", c->piscina);
    s = sectionem_addere(a, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "ab", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "c", c->piscina);
    b = articulum_creare("X", c->piscina);
    s = sectionem_addere(b, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "a", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "bc", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("clavis et valor PAR non permutabiles");
    a = articulum_creare("X", c->piscina);
    s = sectionem_addere(a, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PAR, ZEPHYRUM, "ab", "",
        c->piscina);
    b = articulum_creare("X", c->piscina);
    s = sectionem_addere(b, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PAR, ZEPHYRUM, "", "ab",
        c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("clausula sectionis contra adiuncti");
    a = articulum_creare("X", c->piscina);
    s = sectionem_addere(a, "", c->piscina);
    clausulam_addere(s->clausulae, ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "",
        "p", c->piscina);
    adiunctum_addere(s, "T", c->piscina);
    b = articulum_creare("X", c->piscina);
    s = sectionem_addere(b, "", c->piscina);
    clausulam_addere(adiunctum_addere(s, "T", c->piscina)->clausulae,
        ARTICULUS_PARAGRAPHUS, ZEPHYRUM, "", "p", c->piscina);
    CREDO_FALSUM(sigilla_aequalia(a, b, c->piscina));
}

/* articulus verus: idem HTML per URL redirectionis = idem sigillum */
interior vacuum
_probare_rheum (
    CredoContextus* c)
{
       chorda  html;
    Articulus* a;
    Articulus* b;

    html = filum_legere_totum(
        "probationes/fixa/vicipaedia/rhubarb.html", c->piscina);
    CREDO_VERUM(html.mensura > ZEPHYRUM);
    a = vicipaedia_articulus(html,
        "https://en.wikipedia.org/wiki/Rhubarb", c->piscina);
    b = vicipaedia_articulus(html,
        "https://en.wikipedia.org/wiki/Rheum_rhabarbarum", c->piscina);
    CREDO_VERUM(a != NIHIL && b != NIHIL);

    CREDO_GRADUS("URL redirectionis: idem sigillum");
    CREDO_VERUM(sigilla_aequalia(a, b, c->piscina));

    CREDO_GRADUS("forma canonica totum contentum tenet");
    CREDO_VERUM(articulus_canonicum(a, c->piscina).mensura
        > (i32)(X * M));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "canonicum", _probare_canonicum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fons", _probare_fons, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mutationes", _probare_mutationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambiguitas", _probare_ambiguitas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rheum", _probare_rheum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("articulus", SECTIONES);
}
