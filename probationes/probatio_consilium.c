/* probatio_consilium.c - CONSILIUM (spec consilium S2): nucleus purus -
 * ordo ex datis, M1 (lectio sua), M3 (absentia), formae rerum, murus
 * caeci, cyclus nominatus, conflictus, deduplicatio, politica,
 * vestigium, effusio machinae; exsecutio (S5a): ordo actorum, fractura
 * SISTERE/PERGERE, observata contra propositum, recusatio ante actum.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "consilium.h"
#include "chorda_aedificator.h"
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

/* documentum ex lineis (litterae C89 > DIX octetis vetitae) */
interior chorda
_documentum (
              CredoContextus* c,
    constans character* constans* lineae)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        (memoriae_index)M);
                   i32 i;

    per (i = ZEPHYRUM; lineae[i] != NIHIL; i++)
    {
        chorda_aedificator_appendere_literis(a, lineae[i]);
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde chorda_aedificator_finire(a);
}

hic_manens constans character* constans DOCUMENTUM_SANUM[] = {
    "<consilium>",
    "  <regula titulus=\"arbor_secundaria\" classis=\"domus\"",
    "          sententia=\"vetitum\" causa=\"nihil extra arborem\"/>",
    "  <gradus titulus=\"forma\" genus=\"formatio\""
        " auctor=\"commissio\">",
    "    <effectus verbum=\"scribere\" res=\"lib/aedilis.c\"/>",
    "  </gradus>",
    "  <gradus titulus=\"sana bin/aedilis\" genus=\"institutio\"",
    "          auctor=\"forma\" sumptus_ms=\"4100\">",
    "    <effectus verbum=\"legere\" res=\"lib/*.c\""
        " forma=\"globus\"/>",
    "    <effectus verbum=\"scribere\" res=\"bin/aedilis\"",
    "              classis=\"instrumentum_domus\""
        " cognitio=\"declaratum\"/>",
    "    <vestigium exemplar=\"bin/*\"/>",
    "  </gradus>",
    "  <gradus titulus=\"institue\" genus=\"institutio\""
        " caecus=\"falsum\">",
    "    <effectus verbum=\"instituere\" res=\"~/.bin/aedilis\"",
    "              classis=\"domus\"/>",
    "    <sequitur titulus=\"sana bin/aedilis\"/>",
    "  </gradus>",
    "</consilium>",
    NIHIL
};

hic_manens constans character* constans DOCUMENTUM_MALUM[] = {
    "<consilium color=\"x\">",
    "  <regula titulus=\"r\" sententia=\"fortasse\"/>",
    "  <gradus genus=\"g\"/>",
    "  <gradus titulus=\"a\" caecus=\"saepe\">",
    "    <effectus verbum=\"volare\" res=\"x\"/>",
    "    <effectus verbum=\"legere\" res=\"y\" classis=\"luna\"/>",
    "    <effectus res=\"z\"/>",
    "    <nota/>",
    "  </gradus>",
    "  <aliud/>",
    "</consilium>",
    NIHIL
};

/* IX. documentum: lectio sana (omnia attributa), deinde ordo idem ac
 * per API; documentum malum causas OMNES cum lineis nominat */
interior vacuum
_sectio_documentum (
    CredoContextus* c)
{
                Xar* regulae = NIHIL;
             chorda  causa;
          Consilium* k = consilium_legere(_documentum(c,
              DOCUMENTUM_SANUM),
                         c->piscina, &regulae, &causa);
    ConsiliumGradus* g;
      ConsiliumOrdo* o;
                Xar* j;

    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_AEQUALIS_I32(xar_numerus(consilium_gradus(k)), III);
    CREDO_AEQUALIS_I32(xar_numerus(regulae), I);
    g = consilium_gradum_invenire(k, CH("sana bin/aedilis"));
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_AEQUALIS_I32((i32)g->sumptus_ms, (i32)4100);
    CREDO_AEQUALIS_I32(xar_numerus(g->effectus), II);
    CREDO_AEQUALIS_I32((i32)((constans ConsiliumEffectus*)xar_obtinere(
        g->effectus, ZEPHYRUM))->forma, (i32)CONSILIUM_GLOBUS);
    CREDO_AEQUALIS_I32((i32)((constans ConsiliumEffectus*)xar_obtinere(
        g->effectus, I))->classis, (i32)CONSILIUM_INSTRUMENTUM_DOMUS);
    CREDO_AEQUALIS_I32(xar_numerus(g->vestigium), I);
    o = consilium_ordinare(k, c->piscina);
    CREDO_AEQUALIS_I32(_unda(o, "forma"), I);
    CREDO_AEQUALIS_I32(_unda(o, "sana bin/aedilis"), II);
    CREDO_AEQUALIS_I32(_unda(o, "institue"), III);
    j = consilium_iudicare(k, regulae, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(j), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans ConsiliumIudicium*)
        xar_obtinere(j, ZEPHYRUM))->regula, "arbor_secundaria");

    k = consilium_legere(_documentum(c, DOCUMENTUM_MALUM), c->piscina,
        NIHIL, &causa);
    CREDO_NIHIL(k);
    CREDO_VERUM(chorda_continet(causa, CH("linea 1: attributum ignotum "
        "'color'")));
    CREDO_VERUM(chorda_continet(causa,
        CH("linea 2: sententia 'fortasse'"
        " ignotum (nota: licet rogandum vetitum)")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 3: attributum "
        "necessarium 'titulus' abest")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 4: veritas mala "
        "'saepe'")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 5: verbum 'volare' "
        "ignotum (nota: legere")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 6: classis 'luna' "
        "ignotum")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 7: attributum "
        "necessarium 'verbum' abest")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 8: elementum ignotum "
        "<nota> in <gradus>")));
    CREDO_VERUM(chorda_continet(causa, CH("linea 10: elementum ignotum "
        "<aliud>")));
}

/* X. domus (S4): classis ex via - '~', domus absoluta, externa, radix
 * absoluta, bin/ build/ .../build/; regula arboris secundariae */
interior vacuum
_sectio_domus (
    CredoContextus* c)
{
    constans character* radix = "/u/r/rhubarb-quinta";
    constans character* domus = "/u/r";
       ConsiliumRegula  r;

    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("~/.bin/briar"),
        radix, domus), (i32)CONSILIUM_DOMUS);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("~"), radix,
        domus), (i32)CONSILIUM_DOMUS);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("~x/y"), radix,
        domus), (i32)CONSILIUM_ARBOR);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(
        CH("/u/r/.rhubarb/a"), radix, domus), (i32)CONSILIUM_DOMUS);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("/usr/local/x"),
        radix, domus), (i32)CONSILIUM_EXTERNA);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(
        CH("/u/r/rhubarb-quinta/bin/fabrica"), radix, domus),
        (i32)CONSILIUM_INSTRUMENTUM_DOMUS);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("build/a.o"),
        radix,
        domus), (i32)CONSILIUM_BUILD);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(
        CH("silva/build/x.o"), radix, domus), (i32)CONSILIUM_BUILD);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("lib/xar.c"),
        NIHIL,
        NIHIL), (i32)CONSILIUM_ARBOR);
    CREDO_AEQUALIS_I32((i32)consilium_classis_viae(CH("/x/y"), NIHIL,
        NIHIL), (i32)CONSILIUM_EXTERNA);
    r = consilium_regula_arboris_secundariae(c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.titulus, "arbor_secundaria");
    CREDO_AEQUALIS_I32((i32)r.sententia, (i32)CONSILIUM_VETITUM);
    CREDO_AEQUALIS_I32((i32)r.classis, (i32)CONSILIUM_DOMUS);
}

/* MUNDUS IN MEMORIA (S5a): photographia = res ordinatae per viam;
 * gradus agit secundum tabulas {titulus, via} ('-via' = delere) */
nomen structura {
       CredoContextus* c;
                  Xar* res;          /* ConsiliumObservatio, ordinata */
                  s64  horologium;
    ChordaAedificator* acta;       /* tituli actorum, '|' separati */
   constans character* fractus;      /* hic codicem II reddit */
 constans character* constans* scripturae;  /* paria, NIHIL finis */
 constans character* constans* notatae;     /* paria, NIHIL finis */
                  b32  caecitas;     /* observare deficit */
                  i32  notationes;
                  i32  incepta;
    ChordaAedificator* diarium;    /* S5b: NIHIL = sine diario */
   constans character* arbor;      /* vestigium arboris (S5b) */
} Mundus;

interior vacuum
_mundum_parare (
    CredoContextus* c,
            Mundus* m)
{
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->c = c;
    m->res = xar_creare(c->piscina,
        (i32)magnitudo(ConsiliumObservatio));
    m->acta = chorda_aedificator_creare(c->piscina,
        (memoriae_index)LXIV);
}

/* via scribitur (tempus novum) aut deletur ('-'); ordo servatur */
interior vacuum
_mundo_scribere (
             Mundus* m,
 constans character* via)
{
    b32  delere = via[ZEPHYRUM] == '-';
 chorda  v = chorda_ex_literis(delere ? via + I : via, m->c->piscina);
    Xar* nova = xar_creare(m->c->piscina,
        (i32)magnitudo(ConsiliumObservatio));
    b32 positum = FALSUM;
    i32 i;

    m->horologium++;
    per (i = ZEPHYRUM; i <= xar_numerus(m->res); i++)
    {
        ConsiliumObservatio* x = i < xar_numerus(m->res)
            ? (ConsiliumObservatio*)xar_obtinere(m->res, i) : NIHIL;
                         s32 ordo = x == NIHIL ? I
                             : chorda_comparare(x->via, v);

        si (!positum && ordo >= ZEPHYRUM)
        {
            positum = VERUM;
            si (!delere)
            {
                ConsiliumObservatio* y = (ConsiliumObservatio*)
                    xar_addere(nova);

                y->via        = v;
                y->tempus_ns  = m->horologium;
                y->mensura    = I;
            }
            si (ordo == ZEPHYRUM)
            {
                perge;   /* vetus substitutum aut deletum */
            }
        }
        si (x != NIHIL)
        {
            *(ConsiliumObservatio*)xar_addere(nova) = *x;
        }
    }
    m->res = nova;
}

interior b32
_mundus_agere (
                    vacuum* datum,
  constans ConsiliumGradus* gradus,
                   Piscina* piscina,
            ConsiliumActum* actum)
{
    Mundus* m = (Mundus*)datum;
       i32  i;

    si (chorda_aedificator_longitudo(m->acta) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(m->acta, '|');
    }
    chorda_aedificator_appendere_chorda(m->acta, gradus->titulus);
    per (i = ZEPHYRUM; m->scripturae != NIHIL
         && m->scripturae[i] != NIHIL; i += II)
    {
        si (chorda_aequalis_literis(gradus->titulus, m->scripturae[i]))
        {
            _mundo_scribere(m, m->scripturae[i + I]);
        }
    }
    per (i = ZEPHYRUM; m->notatae != NIHIL
         && m->notatae[i] != NIHIL; i += II)
    {
        si (chorda_aequalis_literis(gradus->titulus, m->notatae[i]))
        {
            si (actum->notata == NIHIL)
            {
                actum->notata = xar_creare(piscina,
                    (i32)magnitudo(chorda));
            }
            *(chorda*)xar_addere(actum->notata) = chorda_ex_literis(
                m->notatae[i + I], piscina);
        }
    }
    actum->codex = m->fractus != NIHIL
        && chorda_aequalis_literis(gradus->titulus, m->fractus) ? II
        : ZEPHYRUM;
    actum->cauda = chorda_ex_literis(actum->codex != ZEPHYRUM ? "fracta"
        : "", piscina);
    redde VERUM;
}

interior b32
_mundus_observare (
     vacuum*  datum,
    Piscina*  piscina,
        Xar** out)
{
    Mundus* m = (Mundus*)datum;
       Xar* copia;
       i32  i;

    si (m->caecitas)
    {
        redde FALSUM;
    }
    copia = xar_creare(piscina, (i32)magnitudo(ConsiliumObservatio));
    per (i = ZEPHYRUM; i < xar_numerus(m->res); i++)
    {
        *(ConsiliumObservatio*)xar_addere(copia) =
            *(ConsiliumObservatio*)xar_obtinere(m->res, i);
    }
    *out = copia;
    redde VERUM;
}

interior vacuum
_mundus_incipit (
                    vacuum* datum,
  constans ConsiliumGradus* gradus,
                       i32  unda)
{
    Mundus* m = (Mundus*)datum;

    m->incepta++;
    si (m->diarium != NIHIL)
    {
        chorda_aedificator_appendere_chorda(m->diarium,
            consilium_diarium_incepti(gradus, unda, m->c->piscina));
    }
}

interior vacuum
_mundus_notare (
                   vacuum* datum,
  constans ConsiliumActus* actus)
{
    Mundus* m = (Mundus*)datum;

    m->notationes++;
    si (m->diarium != NIHIL)
    {
        chorda_aedificator_appendere_chorda(m->diarium,
            consilium_diarium_actus(actus, chorda_ex_literis(m->arbor,
            m->c->piscina), m->c->piscina));
    }
}

interior ConsiliumSutura
_sutura (
    Mundus* m)
{
    ConsiliumSutura s;

    s.datum      = m;
    s.agere      = _mundus_agere;
    s.observare  = _mundus_observare;
    s.incipit    = _mundus_incipit;
    s.notare     = _mundus_notare;
    redde s;
}

/* actus gradus nominati (NIHIL si abest) */
interior constans ConsiliumActus*
_actus (
          constans Xar* acta,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; acta != NIHIL && i < xar_numerus(acta); i++)
    {
        constans ConsiliumActus* x = (constans ConsiliumActus*)
            xar_obtinere(acta, i);

        si (chorda_aequalis_literis(x->gradus->titulus, titulus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

/* XI. exsecutio ordine undarum (ex datis, non additionis); observata */
interior vacuum
_sectio_exsecutio (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* b = _gradus(c, k, "lege");
    ConsiliumGradus* a = _gradus(c, k, "scribe");
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;
    constans character* constans scripturae[] = {
        "scribe", "build/x.o", NIHIL
    };

    _effectus(c, k, b, CONSILIUM_LEGERE, "build/x.o");
    _effectus(c, k, a, CONSILIUM_SCRIBERE, "build/x.o");
    _mundum_parare(c, &m);
    m.scripturae  = scripturae;
    s             = _sutura(&m);
    acta  = consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina,
        &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_AEQUALIS_I32(xar_numerus(acta), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.acta),
        "scribe|lege");
    CREDO_AEQUALIS_I32(_actus(acta, "scribe")->unda, I);
    CREDO_AEQUALIS_I32(_actus(acta, "lege")->unda, II);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "scribe")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "lege")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32(xar_numerus(_actus(acta, "scribe")->observata),
        I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(
        _actus(acta, "scribe")->observata, ZEPHYRUM), "build/x.o");
    CREDO_AEQUALIS_I32(xar_numerus(_actus(acta, "lege")->observata),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(m.incepta, II);
    CREDO_AEQUALIS_I32(m.notationes, II);
}

/* plan fracturae: A (fractus) scribit build/a; B legit build/a et
 * scribit build/b; D legit build/b; C independens */
interior Consilium*
_consilium_fracturae (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "A");
    ConsiliumGradus* x = _gradus(c, k, "C");
    ConsiliumGradus* b = _gradus(c, k, "B");
    ConsiliumGradus* d = _gradus(c, k, "D");

    _effectus(c, k, a, CONSILIUM_SCRIBERE, "build/a");
    _effectus(c, k, x, CONSILIUM_SCRIBERE, "build/c");
    _effectus(c, k, b, CONSILIUM_LEGERE, "build/a");
    _effectus(c, k, b, CONSILIUM_SCRIBERE, "build/b");
    _effectus(c, k, d, CONSILIUM_LEGERE, "build/b");
    redde k;
}

/* XII. fractura: SISTERE (nova non incipiuntur) et PERGERE
 * (independentes pergunt); dependentes OMISSI cum priore nominato */
interior vacuum
_sectio_fractura (
    CredoContextus* c)
{
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;

    _mundum_parare(c, &m);
    m.fractus  = "A";
    s          = _sutura(&m);
    acta  = consilium_exsequi(_consilium_fracturae(c), NIHIL, &s,
        CONSILIUM_SISTERE, NIHIL, c->piscina, &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.acta),
        "A");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "A")->eventus,
        (i32)CONSILIUM_FRACTUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "A")->causa,
        "exitus 2: fracta");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "C")->eventus,
        (i32)CONSILIUM_OMISSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "C")->causa,
        "post fracturam: nova non incipiuntur");
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "B")->causa,
        "dependentia fracta: A");
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "D")->causa,
        "dependentia fracta: B");
    CREDO_AEQUALIS_I32(m.notationes, IV);
    CREDO_AEQUALIS_I32(m.incepta, I);

    _mundum_parare(c, &m);
    m.fractus  = "A";
    s          = _sutura(&m);
    acta  = consilium_exsequi(_consilium_fracturae(c), NIHIL, &s,
        CONSILIUM_PERGERE, NIHIL, c->piscina, &causa);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.acta),
        "A|C");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "C")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "B")->eventus,
        (i32)CONSILIUM_OMISSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "D")->causa,
        "dependentia fracta: B");
    CREDO_AEQUALIS_S32(_actus(acta, "D")->actum.codex, -I);
}

/* XIII. observata contra propositum: extra = FRACTUM nominatum;
 * vestigium permittit; notata (liber) numerantur; deletio scriptura;
 * caecus et non PLENUS nominant sed non franguntur */
interior vacuum
_sectio_extra_propositum (
    CredoContextus* c)
{
          Consilium* k   = consilium_creare(c->piscina);
    ConsiliumGradus* k1  = _gradus(c, k, "K");   /* caecus PRIMUS: murus
                                                 * post fractos eum
                                                 * omitteret */
    ConsiliumGradus* s1 = _gradus(c, k, "S");
    ConsiliumGradus* n1 = _gradus(c, k, "N");
    ConsiliumGradus* d1 = _gradus(c, k, "D");
    ConsiliumGradus* p1 = _gradus(c, k, "P");
  ConsiliumEffectus  e;
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;
             chorda  t;
    constans character* constans scripturae[] = {
        "S", "build/x.o", "S", "build/tmp/y", "S", "~/.bin/z",
        "D", "-build/vetus",
        "P", "build/p/q", "P", "lib/r.c",
        "K", "lib/q.c", NIHIL
    };
    constans character* constans notatae[] = {
        "N", "~/.rhubarb/q", NIHIL
    };

    _effectus(c, k, s1, CONSILIUM_SCRIBERE, "build/x.o");
    *(chorda*)xar_addere(s1->vestigium) = CH("build/*");
    _effectus(c, k, d1, CONSILIUM_DELERE, "build/vetus");
    e = consilium_effectus(CONSILIUM_SCRIBERE, CH("build/p/"));
    e.forma = CONSILIUM_PRAEFIXUM;
    e.certitudo = CONSILIUM_EX_PARTE;
    consilium_effectum_addere(k, p1, e);
    k1->caecus = VERUM;
    (vacuum)n1;
    _mundum_parare(c, &m);
    _mundo_scribere(&m, "build/vetus");
    m.scripturae  = scripturae;
    m.notatae     = notatae;
    s             = _sutura(&m);
    acta  = consilium_exsequi(k, NIHIL, &s, CONSILIUM_PERGERE, NIHIL,
        c->piscina, &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "S")->eventus,
        (i32)CONSILIUM_FRACTUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "S")->causa,
        "scripsit extra propositum: ~/.bin/z");
    CREDO_AEQUALIS_I32(xar_numerus(_actus(acta, "S")->observata), III);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "N")->eventus,
        (i32)CONSILIUM_FRACTUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "N")->causa,
        "scripsit extra propositum: ~/.rhubarb/q");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "D")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(
        _actus(acta, "D")->observata, ZEPHYRUM), "build/vetus");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "P")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32(xar_numerus(_actus(acta, "P")->extra), I);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "K")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32(xar_numerus(_actus(acta, "K")->extra), I);
    t = consilium_acta_scribere(acta, FALSUM, c->piscina);
    CREDO_VERUM(chorda_continet(t,
        CH("extra propositum, non promissum: "
        "lib/q.c")));
    t = consilium_acta_scribere(acta, VERUM, c->piscina);
    CREDO_VERUM(chorda_continet(t, CH("EXTRA\tS\t~/.bin/z\n")));
    CREDO_VERUM(chorda_continet(t, CH("SUMMA\tacta\t3\tfracta\t2"
        "\tomissa\t0\n")));
}

/* XIV. recusatio ante quodvis actum: VETITUM, cyclus, conflictus;
 * observatio deficiens = non incepit */
interior vacuum
_sectio_recusatio (
    CredoContextus* c)
{
          Consilium* k;
    ConsiliumGradus* a;
    ConsiliumGradus* b;
  ConsiliumEffectus  e;
                Xar* regulae = xar_creare(c->piscina,
                    (i32)magnitudo(ConsiliumRegula));
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;

    _mundum_parare(c, &m);
    s = _sutura(&m);
    *(ConsiliumRegula*)xar_addere(regulae) =
        consilium_regula_arboris_secundariae(c->piscina);
    k = consilium_creare(c->piscina);
    a = _gradus(c, k, "institue");
    e = consilium_effectus(CONSILIUM_INSTITUERE, CH("~/.bin/x"));
    e.classis = CONSILIUM_DOMUS;
    consilium_effectum_addere(k, a, e);
    acta = consilium_exsequi(k, regulae, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina, &causa);
    CREDO_NIHIL(acta);
    CREDO_VERUM(chorda_continet(causa,
        CH("VETITUM: institue instituere "
        "~/.bin/x <- arbor_secundaria")));

    k                                  = consilium_creare(c->piscina);
    a                                  = _gradus(c, k, "A");
    b                                  = _gradus(c, k, "B");
    *(chorda*)xar_addere(a->sequitur)  = CH("B");
    *(chorda*)xar_addere(b->sequitur)  = CH("A");
    acta = consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina,
        &causa);
    CREDO_NIHIL(acta);
    CREDO_VERUM(chorda_continet(causa, CH("cyclus:")));

    k = consilium_creare(c->piscina);
    a = _gradus(c, k, "A");
    b = _gradus(c, k, "B");
    _effectus(c, k, a, CONSILIUM_SCRIBERE, "build/x");
    _effectus(c, k, b, CONSILIUM_SCRIBERE, "build/x");
    acta = consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina,
        &causa);
    CREDO_NIHIL(acta);
    CREDO_VERUM(chorda_continet(causa, CH("conflictus: A et B scribunt "
        "build/x")));
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.acta),
        "");

    m.caecitas  = VERUM;
    k           = consilium_creare(c->piscina);
    a           = _gradus(c, k, "A");
    acta = consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina,
        &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_AEQUALIS_I32((i32)_actus(acta, "A")->eventus,
        (i32)CONSILIUM_FRACTUM);
    CREDO_CHORDA_INCIPIT(_actus(acta, "A")->causa,
        CH("observatio ante actum deficit"));
    CREDO_AEQUALIS_I32(m.incepta, ZEPHYRUM);
}

/* XV. acta machinae: ordo exsecutionis, codex -1 omissorum, summa */
interior vacuum
_sectio_acta (
    CredoContextus* c)
{
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;

    _mundum_parare(c, &m);
    m.fractus  = "A";
    s          = _sutura(&m);
    acta  = consilium_exsequi(_consilium_fracturae(c), NIHIL, &s,
        CONSILIUM_SISTERE, NIHIL, c->piscina, &causa);
    CREDO_CHORDA_AEQUALIS_LITERIS(consilium_acta_scribere(acta, VERUM,
        c->piscina),
        "ACTUS\t1\tA\tFRACTUM\t2\t0\texitus 2: fracta\n"
        "ACTUS\t1\tC\tOMISSUM\t-1\t0\tpost fracturam: nova non "
        "incipiuntur\n"
        "ACTUS\t2\tB\tOMISSUM\t-1\t0\tdependentia fracta: A\n"
        "ACTUS\t3\tD\tOMISSUM\t-1\t0\tdependentia fracta: B\n"
        "SUMMA\tacta\t0\tfracta\t1\tomissa\t3\n");
}

/* consilium catenae: A -> B -> C (data) */
interior Consilium*
_consilium_catenae (
    CredoContextus* c)
{
          Consilium* k = consilium_creare(c->piscina);
    ConsiliumGradus* a = _gradus(c, k, "A");
    ConsiliumGradus* b = _gradus(c, k, "B");
    ConsiliumGradus* x = _gradus(c, k, "C");

    _effectus(c, k, a, CONSILIUM_SCRIBERE, "build/a");
    _effectus(c, k, b, CONSILIUM_LEGERE, "build/a");
    _effectus(c, k, b, CONSILIUM_SCRIBERE, "build/b");
    _effectus(c, k, x, CONSILIUM_LEGERE, "build/b");
    redde k;
}

/* XVI. sigillum: idem consilium = idem; res, sequitur, regula mutant;
 * auctor non mutat */
interior vacuum
_sectio_sigillum (
    CredoContextus* c)
{
          Consilium* k1 = _consilium_catenae(c);
          Consilium* k2 = _consilium_catenae(c);
          Consilium* k3 = _consilium_catenae(c);
          Consilium* k4 = _consilium_catenae(c);
                Xar* regulae = xar_creare(c->piscina,
                    (i32)magnitudo(ConsiliumRegula));
             chorda s1;

    s1 = consilium_sigillum(k1, NIHIL, c->piscina);
    CREDO_AEQUALIS_I32(s1.mensura, LXIV);
    (vacuum)consilium_gradum_addere(k2, CH("A"), CH("generator"),
        CH("auctor alius"));
    CREDO_CHORDA_AEQUALIS(consilium_sigillum(k2, NIHIL, c->piscina),
        s1);
    _effectus(c, k3, consilium_gradum_invenire(k3, CH("C")),
        CONSILIUM_SCRIBERE, "build/c");
    CREDO_VERUM(!chorda_aequalis(consilium_sigillum(k3, NIHIL,
        c->piscina), s1));
    *(chorda*)xar_addere(consilium_gradum_invenire(k4,
        CH("C"))->sequitur) = CH("A");
    CREDO_VERUM(!chorda_aequalis(consilium_sigillum(k4, NIHIL,
        c->piscina), s1));
    *(ConsiliumRegula*)xar_addere(regulae) =
        consilium_regula_arboris_secundariae(c->piscina);
    CREDO_VERUM(!chorda_aequalis(consilium_sigillum(k1, regulae,
        c->piscina), s1));
}

/* XVII. diarium: lineae per incipit/notare, campi purgati */
interior vacuum
_sectio_diarium (
    CredoContextus* c)
{
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
             chorda  causa;

    _mundum_parare(c, &m);
    m.fractus  = "B";
    m.arbor    = "f1";
    m.diarium  = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CCLVI);
    chorda_aedificator_appendere_chorda(m.diarium,
        consilium_diarium_caput(CH("s1"), CH("f1"), FALSUM,
        c->piscina));
    s     = _sutura(&m);
    acta  = consilium_exsequi(_consilium_catenae(c), NIHIL, &s,
        CONSILIUM_SISTERE, NIHIL, c->piscina, &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.diarium),
        "CONSILIUM\ts1\tf1\tnovum\n"
        "INCEPIT\t1\tA\n"
        "ACTUS\t1\tA\tACTUM\t0\t0\t\n"
        "ARBOR\tf1\n"
        "INCEPIT\t2\tB\n"
        "ACTUS\t2\tB\tFRACTUM\t2\t0\texitus 2: fracta\n"
        "ARBOR\tf1\n"
        "ACTUS\t3\tC\tOMISSUM\t-1\t0\tdependentia fracta: B\n"
        "ARBOR\tf1\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(consilium_diarium_caput(CH("a\tb"),
        CH("x\ny"), VERUM, c->piscina),
        "CONSILIUM\ta b\tx y\tresumptum\n");
}

/* XVIII. resumptio: interruptus resumitur sine actu iterato; catena
 * cursuum resumptorum; consilium aut arbor mutata, acta vacua,
 * deformis, cursus novus non hereditat, linea interrupta neglecta */
interior vacuum
_sectio_resumptio (
    CredoContextus* c)
{
          Consilium* k    = _consilium_catenae(c);
             chorda  sig  = consilium_sigillum(k, NIHIL, c->piscina);
             Mundus  m;
    ConsiliumSutura  s;
                Xar* acta;
                Xar* facta;
             chorda  causa;
             chorda  diarium;
             chorda  primum;

    /* cursus primus: A actum, B fractus (interruptus) */
    _mundum_parare(c, &m);
    m.fractus  = "B";
    m.arbor    = "f1";
    m.diarium  = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CCLVI);
    chorda_aedificator_appendere_chorda(m.diarium,
        consilium_diarium_caput(sig, CH("f0"), FALSUM, c->piscina));
    s = _sutura(&m);
    (vacuum)consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, NIHIL,
        c->piscina, &causa);
    primum = chorda_aedificator_finire(m.diarium);
    CREDO_VERUM(consilium_diarium_resumere(primum, sig, CH("f1"),
        c->piscina, &facta, &causa));
    CREDO_AEQUALIS_I32(xar_numerus(facta), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(facta,
        ZEPHYRUM), "A");

    /* resumptio: A non agitur, B et C aguntur */
    _mundum_parare(c, &m);
    m.arbor    = "f2";
    m.diarium  = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CCLVI);
    chorda_aedificator_appendere_chorda(m.diarium, primum);
    chorda_aedificator_appendere_chorda(m.diarium,
        consilium_diarium_caput(sig, CH("f1"), VERUM, c->piscina));
    s     = _sutura(&m);
    acta  = consilium_exsequi(k, NIHIL, &s, CONSILIUM_SISTERE, facta,
        c->piscina, &causa);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(m.acta),
        "B|C");
    CREDO_CHORDA_AEQUALIS_LITERIS(_actus(acta, "A")->causa,
        "ex actis: actum ante");
    CREDO_AEQUALIS_I32((i32)_actus(acta, "C")->eventus,
        (i32)CONSILIUM_ACTUM);
    CREDO_AEQUALIS_I32(m.incepta, II);
    CREDO_AEQUALIS_I32(m.notationes, III);
    diarium = chorda_aedificator_finire(m.diarium);

    /* catena: cursus resumptus + primus = A B C */
    CREDO_VERUM(consilium_diarium_resumere(diarium, sig, CH("f2"),
        c->piscina, &facta, &causa));
    CREDO_AEQUALIS_I32(xar_numerus(facta), III);

    /* arbor mutata post ultimum actum; consilium mutatum */
    CREDO_FALSUM(consilium_diarium_resumere(diarium, sig, CH("f9"),
        c->piscina, &facta, &causa));
    CREDO_CHORDA_INCIPIT(causa, CH("arbor mutata post ultimum actum "
        "(acta f2, nunc f9)"));
    CREDO_FALSUM(consilium_diarium_resumere(diarium, CH("aliud"),
        CH("f2"), c->piscina, &facta, &causa));
    CREDO_CHORDA_INCIPIT(causa, CH("consilium mutatum post initium"));

    /* acta vacua; linea deformis; linea interrupta neglecta */
    CREDO_FALSUM(consilium_diarium_resumere(CH(""), sig, CH("f0"),
        c->piscina, &facta, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS(causa,
        "acta vacua: nihil resumendum");
    CREDO_FALSUM(consilium_diarium_resumere(chorda_concatenare(primum,
        CH("QUID\tx\n"), c->piscina), sig, CH("f1"), c->piscina, &facta,
        &causa));
    CREDO_CHORDA_INCIPIT(causa, CH("linea 10 deformis: QUID"));
    CREDO_VERUM(consilium_diarium_resumere(chorda_concatenare(primum,
        CH("ARBOR\tf7"), c->piscina), sig, CH("f1"), c->piscina, &facta,
        &causa));

    /* cursus NOVUS post primum: A primi non hereditatur */
    CREDO_VERUM(consilium_diarium_resumere(chorda_concatenare(primum,
        consilium_diarium_caput(sig, CH("f1"), FALSUM, c->piscina),
        c->piscina), sig, CH("f1"), c->piscina, &facta, &causa));
    CREDO_AEQUALIS_I32(xar_numerus(facta), ZEPHYRUM);
}

hic_manens constans character* constans DOCUMENTUM_MANDATI[] = {
    "<consilium>",
    "  <gradus titulus=\"sana\">",
    "    <mandatum>",
    "      <argumentum valor=\"tools/fabrica_struere.sh\"/>",
    "      <argumentum valor=\"cum spatio\"/>",
    "    </mandatum>",
    "  </gradus>",
    "</consilium>",
    NIHIL
};

hic_manens constans character* constans DOCUMENTUM_MANDATI_MALI[] = {
    "<consilium>",
    "  <gradus titulus=\"duplex\">",
    "    <mandatum><argumentum valor=\"a\"/></mandatum>",
    "    <mandatum><argumentum valor=\"b\"/></mandatum>",
    "  </gradus>",
    "  <gradus titulus=\"vacuum\"><mandatum/></gradus>",
    "  <gradus titulus=\"malum\">",
    "    <mandatum><arg valor=\"a\"/><argumentum/></mandatum>",
    "  </gradus>",
    "</consilium>",
    NIHIL
};

/* XIX. mandatum: argv ordine (spatia tuta), causae omnes; mandatum in
 * sigillo */
interior vacuum
_sectio_mandatum (
    CredoContextus* c)
{
             chorda  causa;
          Consilium* k = consilium_legere(_documentum(c,
              DOCUMENTUM_MANDATI), c->piscina, NIHIL, &causa);
          Consilium* k2;
    ConsiliumGradus* g;
             chorda  s1;

    CREDO_NECESSE_NON_NIHIL(k);
    g = consilium_gradum_invenire(k, CH("sana"));
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_AEQUALIS_I32(xar_numerus(g->mandatum), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(g->mandatum,
        ZEPHYRUM), "tools/fabrica_struere.sh");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(g->mandatum,
        I),
        "cum spatio");
    s1 = consilium_sigillum(k, NIHIL, c->piscina);
    k2 = consilium_legere(_documentum(c, DOCUMENTUM_MANDATI),
        c->piscina,
        NIHIL, &causa);
    CREDO_CHORDA_AEQUALIS(consilium_sigillum(k2, NIHIL, c->piscina),
        s1);
    *(chorda*)xar_obtinere(consilium_gradum_invenire(k2,
        CH("sana"))->mandatum, I) = CH("aliud");
    CREDO_VERUM(!chorda_aequalis(consilium_sigillum(k2, NIHIL,
        c->piscina), s1));

    CREDO_NIHIL(consilium_legere(_documentum(c,
        DOCUMENTUM_MANDATI_MALI),
        c->piscina, NIHIL, &causa));
    CREDO_VERUM(chorda_continet(causa, CH("mandatum duplex in gradu "
        "duplex")));
    CREDO_VERUM(chorda_continet(causa, CH("mandatum vacuum in gradu "
        "vacuum")));
    CREDO_VERUM(chorda_continet(causa, CH("elementum ignotum <arg> in "
        "<mandatum>")));
    CREDO_VERUM(chorda_continet(causa, CH("attributum necessarium "
        "'valor' abest")));
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
    { "documentum", _sectio_documentum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "domus", _sectio_domus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exsecutio", _sectio_exsecutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fractura", _sectio_fractura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extra propositum", _sectio_extra_propositum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "recusatio ante actum", _sectio_recusatio, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "acta", _sectio_acta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sigillum", _sectio_sigillum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "diarium", _sectio_diarium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "resumptio", _sectio_resumptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mandatum", _sectio_mandatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("consilium", SECTIONES);
}
