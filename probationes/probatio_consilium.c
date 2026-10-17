/* probatio_consilium.c - CONSILIUM (spec consilium S2): nucleus purus -
 * ordo ex datis, M1 (lectio sua), M3 (absentia), formae rerum, murus
 * caeci, cyclus nominatus, conflictus, deduplicatio, politica,
 * vestigium, effusio machinae.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "consilium.h"
#include <string.h>

#define CH(s) chorda_ex_literis((s), c->piscina)

interior ConsiliumGradus*
_gradus (
    CredoContextus* c,
         Consilium* k,
 constans character* titulus)
{
    redde consilium_gradum_addere(k, CH(titulus), CH("generator"),
        CH(""));
}

interior vacuum
_effectus (
     CredoContextus* c,
          Consilium* k,
     ConsiliumGradus* g,
     ConsiliumVerbum verbum,
  constans character* res)
{
    consilium_effectum_addere(k, g, consilium_effectus(verbum,
        CH(res)));
}

/* unda (I-basata) gradus nominati; ZEPHYRUM = in nulla */
interior i32
_unda (
    constans ConsiliumOrdo* o,
        constans character* titulus)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < xar_numerus(o->undae); i++)
    {
        constans Xar* u = *(Xar**)xar_obtinere(o->undae, i);

        per (j = ZEPHYRUM; j < xar_numerus(u); j++)
        {
            si (chorda_aequalis_literis(
                    (*(ConsiliumGradus**)xar_obtinere(u, j))->titulus,
                    titulus))
            {
                redde i + I;
            }
        }
    }
    redde ZEPHYRUM;
}

/* I. ordo ex datis: scriptor ante lectorem; arcus causam nominat */
interior vacuum
_sectio_data (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* b = _gradus(c, k, "legit");
    ConsiliumGradus* a = _gradus(c, k, "scribit");
      ConsiliumOrdo* o;

    _effectus(c, k, b, CONSILIUM_LEGERE, "build/x.o");
    _effectus(c, k, a, CONSILIUM_SCRIBERE, "build/x.o");
    o = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(o->undae), II);
    CREDO_AEQUALIS_I32(_unda(o, "scribit"), I);
    CREDO_AEQUALIS_I32(_unda(o, "legit"), II);
    CREDO_AEQUALIS_I32(xar_numerus(o->arcus), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ConsiliumArcus*)
        xar_obtinere(o->arcus, ZEPHYRUM))->causa,
        "scribit scribit build/x.o quod legitur");
    CREDO_AEQUALIS_I32(xar_numerus(o->cyclus), ZEPHYRUM);
}

/* II. M1: lectio rei quam gradus ipse scribit ingressus non est; M3:
 * absentia probata scriptorem sequitur */
interior vacuum
_sectio_lectio_sua (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* g = _gradus(c, k, "generator");
    ConsiliumGradus* h = _gradus(c, k, "alter");
    ConsiliumGradus* p = _gradus(c, k, "probat");
    ConsiliumGradus* q = _gradus(c, k, "creat");
  ConsiliumEffectus  e;
      ConsiliumOrdo* o;

    /* generator scribit y et legit y (suum); alter y QUOQUE scribit -
     * lectio generatoris sua est, ergo alterum non sequitur */
    _effectus(c, k, g, CONSILIUM_SCRIBERE, "y");
    _effectus(c, k, g, CONSILIUM_LEGERE, "y");
    _effectus(c, k, h, CONSILIUM_SCRIBERE, "y");
    /* probat z ABSENTEM; creat z */
    e         = consilium_effectus(CONSILIUM_INSPICERE, CH("z"));
    e.exstat  = FALSUM;
    consilium_effectum_addere(k, p, e);
    _effectus(c, k, q, CONSILIUM_SCRIBERE, "z");
    o = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(_unda(o, "generator"), I);
    CREDO_AEQUALIS_I32(_unda(o, "creat"), I);
    CREDO_AEQUALIS_I32(_unda(o, "probat"), II);
    CREDO_AEQUALIS_I32(xar_numerus(o->arcus), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ConsiliumArcus*)
        xar_obtinere(o->arcus, ZEPHYRUM))->causa,
        "absentia: creat scribit z quod legitur");
}

/* III. formae: globus et praefixum; enumeratio directorii omnia sub eo
 * tangit; sequitur expresse; sequitur ignotum recusatur */
interior vacuum
_sectio_formae (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "globus");
    ConsiliumGradus* b = _gradus(c, k, "lector");
    ConsiliumGradus* d = _gradus(c, k, "enumerat");
    ConsiliumGradus* e = _gradus(c, k, "post");
  ConsiliumEffectus  x;
      ConsiliumOrdo* o;

    x = consilium_effectus(CONSILIUM_SCRIBERE, CH("build/obj/*.o"));
    x.forma = CONSILIUM_GLOBUS;
    consilium_effectum_addere(k, a, x);
    _effectus(c, k, b, CONSILIUM_LEGERE, "build/obj/a.o");
    _effectus(c, k, d, CONSILIUM_ENUMERARE, "build/obj/");
    *(chorda*)xar_addere(e->sequitur) = CH("lector");
    *(chorda*)xar_addere(e->sequitur) = CH("nusquam");
    o = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(_unda(o, "globus"), I);
    CREDO_AEQUALIS_I32(_unda(o, "lector"), II);
    CREDO_AEQUALIS_I32(_unda(o, "enumerat"), II);
    CREDO_AEQUALIS_I32(_unda(o, "post"), III);
    CREDO_AEQUALIS_I32(xar_numerus(o->recusationes), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(
        *(chorda*)xar_obtinere(o->recusationes, ZEPHYRUM),
        "post sequitur nusquam: non in consilio");
}

/* IV. gradus caecus murus est: priores ante, posteriores post */
interior vacuum
_sectio_caecus (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "a");
    ConsiliumGradus* m = _gradus(c, k, "caecus");
    ConsiliumGradus* b = _gradus(c, k, "b");
      ConsiliumOrdo* o;

    (vacuum)a;
    (vacuum)b;
    m->caecus  = VERUM;
    o          = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(_unda(o, "a"), I);
    CREDO_AEQUALIS_I32(_unda(o, "caecus"), II);
    CREDO_AEQUALIS_I32(_unda(o, "b"), III);
}

/* V. cyclus NOMINATUS: membra eius, ordine arcuum; extra cyclum
 * undae */
interior vacuum
_sectio_cyclus (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* s = _gradus(c, k, "solus");
    ConsiliumGradus* a = _gradus(c, k, "p");
    ConsiliumGradus* b = _gradus(c, k, "q");
      ConsiliumOrdo* o;
             chorda  e;

    (vacuum)s;
    _effectus(c, k, a, CONSILIUM_SCRIBERE, "p.txt");
    _effectus(c, k, a, CONSILIUM_LEGERE, "q.txt");
    _effectus(c, k, b, CONSILIUM_SCRIBERE, "q.txt");
    _effectus(c, k, b, CONSILIUM_LEGERE, "p.txt");
    o = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(_unda(o, "solus"), I);
    CREDO_AEQUALIS_I32(_unda(o, "p"), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(o->cyclus), II);
    e = consilium_scribere(k, o, NIHIL, NIHIL, VERUM, c->piscina);
    CREDO_VERUM(   chorda_continet(e, CH("CYCLUS\tp\tq\n"))
                || chorda_continet(e, CH("CYCLUS\tq\tp\n")));
}

/* VI. conflictus: duo scriptores sine ordine; sequitur eum tollit;
 * delere + delere nullus */
interior vacuum
_sectio_conflictus (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "arbor_crusta");
    ConsiliumGradus* b = _gradus(c, k, "arbor_md");
    ConsiliumGradus* d = _gradus(c, k, "d1");
    ConsiliumGradus* e = _gradus(c, k, "d2");
    ConsiliumGradus* f = _gradus(c, k, "f1");
    ConsiliumGradus* g = _gradus(c, k, "f2");
      ConsiliumOrdo* o;
                Xar* x;

    _effectus(c, k, a, CONSILIUM_SCRIBERE,
        "build/aedilis/arbor/manifestum.stml");
    _effectus(c, k, b, CONSILIUM_SCRIBERE,
        "build/aedilis/arbor/manifestum.stml");
    _effectus(c, k, d, CONSILIUM_DELERE, "tmp.lst");
    _effectus(c, k, e, CONSILIUM_DELERE, "tmp.lst");
    _effectus(c, k, f, CONSILIUM_SCRIBERE, "ordinatum.txt");
    _effectus(c, k, g, CONSILIUM_SCRIBERE, "ordinatum.txt");
    *(chorda*)xar_addere(g->sequitur) = CH("f1");
    o = consilium_ordinare(k, c->piscina);
    x = consilium_conflictus(k, o, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ConsiliumConflictus*)
        xar_obtinere(x, ZEPHYRUM))->res,
        "build/aedilis/arbor/manifestum.stml");
}

/* VII. deduplicatio: gradus idem bis = unus cum auctoribus II; effectus
 * idem bis = unus */
interior vacuum
_sectio_semel (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = consilium_gradum_addere(k,
        CH("sana bin/fabrica"),
        CH("institutio"), CH("forma"));
    ConsiliumGradus* b = consilium_gradum_addere(k,
        CH("sana bin/fabrica"),
        CH("institutio"), CH("portae"));

    CREDO_VERUM(a == b);
    CREDO_AEQUALIS_I32(xar_numerus(consilium_gradus(k)), I);
    CREDO_AEQUALIS_I32(xar_numerus(a->auctores), II);
    _effectus(c, k, a, CONSILIUM_SCRIBERE, "bin/fabrica");
    _effectus(c, k, b, CONSILIUM_SCRIBERE, "bin/fabrica");
    CREDO_AEQUALIS_I32(xar_numerus(a->effectus), I);
}

/* VIII. politica: prima regula congruens vincit; LICET expressum
 * cessat; vestigium scripturas extra id vetat */
interior vacuum
_sectio_politica (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "institue");
    ConsiliumGradus* b = _gradus(c, k, "rete");
    ConsiliumGradus* d = _gradus(c, k, "generator");
  ConsiliumEffectus  e;
                Xar* regulae = xar_creare(c->piscina,
                    (i32)magnitudo(ConsiliumRegula));
    ConsiliumRegula* r;
                Xar* j;
             chorda  s;

    e = consilium_effectus(CONSILIUM_INSTITUERE, CH("~/.bin/fabrica"));
    e.classis = CONSILIUM_DOMUS;
    consilium_effectum_addere(k, a, e);
    e = consilium_effectus(CONSILIUM_SCRIBERE, CH("~/.rhubarb/x"));
    e.classis = CONSILIUM_DOMUS;
    consilium_effectum_addere(k, a, e);
    _effectus(c, k, b, CONSILIUM_RETE, "https://api");
    _effectus(c, k, d, CONSILIUM_SCRIBERE, "build/a/x.c");
    _effectus(c, k, d, CONSILIUM_SCRIBERE, "lib/y.c");
    *(chorda*)xar_addere(d->vestigium) = CH("build/a/*");
    /* LICET expressum pro ~/.rhubarb ante VETITUM domus */
    r           = (ConsiliumRegula*)xar_addere(regulae);
    r->titulus  = CH("annales"); r->verbum = CONSILIUM_OMNIA;
    r->classis = CONSILIUM_DOMUS; r->exemplar = CH("~/.rhubarb/*");
    r->sententia = CONSILIUM_LICET; r->causa = CH("");
    r           = (ConsiliumRegula*)xar_addere(regulae);
    r->titulus  = CH("arbor_secundaria"); r->verbum = CONSILIUM_OMNIA;
    r->classis = CONSILIUM_DOMUS; r->exemplar = CH("");
    r->sententia = CONSILIUM_VETITUM;
    r->causa = CH("in arbore secundaria nihil extra arborem scribitur");
    r = (ConsiliumRegula*)xar_addere(regulae);
    r->titulus = CH("rete"); r->verbum = CONSILIUM_RETE;
    r->classis = CONSILIUM_OMNIA; r->exemplar = CH("");
    r->sententia = CONSILIUM_ROGANDUM; r->causa = CH("Franus approbet");
    j = consilium_iudicare(k, regulae, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(j), III);
    s = consilium_scribere(k, NIHIL, NIHIL, j, VERUM, c->piscina);
    CREDO_VERUM(chorda_continet(s, CH("IUDICIUM\tinstitue\tinstituere\t"
        "~/.bin/fabrica\tVETITUM\tarbor_secundaria\t")));
    CREDO_VERUM(chorda_continet(s,
        CH("IUDICIUM\trete\trete\thttps://api"
        "\tROGANDUM\trete\tFranus approbet\n")));
    CREDO_VERUM(chorda_continet(s, CH("IUDICIUM\tgenerator\tscribere\t"
        "lib/y.c\tVETITUM\tvestigium\t")));
    CREDO_VERUM(!chorda_continet(s, CH("~/.rhubarb/x")));
    CREDO_VERUM(!chorda_continet(s, CH("build/a/x.c")));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordo ex datis", _sectio_data, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lectio sua et absentia", _sectio_lectio_sua, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "formae et sequitur", _sectio_formae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "murus caeci", _sectio_caecus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cyclus nominatus", _sectio_cyclus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "conflictus", _sectio_conflictus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deduplicatio", _sectio_semel, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "politica et vestigium", _sectio_politica, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("consilium", SECTIONES);
}
