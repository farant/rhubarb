/* probatio_pulsus_forma.c - pulsus_forma (pulsus-plan T1): eventus
 * (iter et reditus, limites C10), textus ex formis, corpus APNs
 * (spec III.1, truncatio ad MMMMXCVI octetos), tactus ex corpore. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "pulsus_forma.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior Xar*
res_ex (
    constans character* constans* ids,
                          i32  n)
{
    Xar* x;
    i32  i;

    x = xar_creare(piscina, magnitudo(chorda));
    per (i = ZEPHYRUM; i < n; i++)
    {
        *(chorda*)xar_addere(x) = lit(ids[i]);
    }
    redde x;
}

/* eventus pictoris (spec III.4) */
interior PulsusEventus
eventus_pictoris (vacuum)
{
    hic_manens constans character* constans IDS[] = {
        "01JABCDEFGHJKMNPQRSTVWXYZ0:pictura-1",
        "01JABCDEFGHJKMNPQRSTVWXYZ0:pictura-2"
    };
    PulsusEventus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.clavis  = lit("pictor.picturae");
    e.thema   = lit("earth.golden.pictor");
    e.res     = res_ex(IDS, II);
    e.mora_s  = CCC;
    e.unus    = lit("{persona} shared a picture");
    e.plures  = lit("{persona} shared {numerus} pictures");
    e.statim  = FALSUM;
    redde e;
}

/* eventum mutatum scribit et legit: FALSUM exspectatur, causa campum
 * nominat */
interior vacuum
_recusatum (
     constans PulsusEventus* e,
        constans character* campus)
{
    PulsusEventus r;
           chorda causa;
           chorda json;

    json = pulsus_eventum_scribere(e, piscina);
    CREDO_CHORDA_NON_VACUA(json);
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(pulsus_eventum_legere(json, piscina, &r, &causa));
    CREDO_CHORDA_CONTINET(causa, lit(campus));
}

interior vacuum
_sectio_eventus (
    CredoContextus* c)
{
    PulsusEventus e;
    PulsusEventus r;
           chorda causa;
           chorda json;
              i32 i;
        character id[XVI];

    (vacuum)c;
    e     = eventus_pictoris();
    json  = pulsus_eventum_scribere(&e, piscina);
    CREDO_VERUM(pulsus_eventum_legere(json, piscina, &r, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS(r.clavis, "pictor.picturae");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.thema, "earth.golden.pictor");
    CREDO_AEQUALIS_I32(xar_numerus(r.res), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(r.res, I),
        "01JABCDEFGHJKMNPQRSTVWXYZ0:pictura-2");
    CREDO_AEQUALIS_I32(r.mora_s, CCC);
    CREDO_CHORDA_AEQUALIS(r.unus, e.unus);
    CREDO_CHORDA_AEQUALIS(r.plures, e.plures);
    CREDO_FALSUM(r.statim);

    /* statim sine rebus: licet (Notify now sine pictura aperta) */
    e         = eventus_pictoris();
    e.res     = res_ex(NIHIL, ZEPHYRUM);
    e.statim  = VERUM;
    json      = pulsus_eventum_scribere(&e, piscina);
    CREDO_VERUM(pulsus_eventum_legere(json, piscina, &r, &causa));
    CREDO_VERUM(r.statim);
    CREDO_AEQUALIS_I32(xar_numerus(r.res), ZEPHYRUM);

    /* limites C10 */
    e         = eventus_pictoris();
    e.mora_s  = MMMDCI;
    _recusatum(&e, "mora");
    e       = eventus_pictoris();
    e.unus  = lit("Fran \x01 shared");
    _recusatum(&e, "unus");
    e         = eventus_pictoris();
    e.plures  = lit("{nomen} shared {numerus}");
    _recusatum(&e, "plures");
    e        = eventus_pictoris();
    e.thema  = lit("earth golden");
    _recusatum(&e, "thema");
    e         = eventus_pictoris();
    e.clavis  = lit("");
    _recusatum(&e, "clavis");
    e      = eventus_pictoris();
    e.res  = xar_creare(piscina, magnitudo(chorda));
    per (i = ZEPHYRUM; i < CI; i++)
    {
        sprintf(id, "id-%u", (unsigned)i);
        *(chorda*)xar_addere(e.res) = lit(id);
    }
    _recusatum(&e, "res");

    /* corpus non JSON, aut sine campis */
    CREDO_FALSUM(pulsus_eventum_legere(lit("{"), piscina, &r, &causa));
    CREDO_FALSUM(pulsus_eventum_legere(lit("{\"clavis\":\"a\"}"),
        piscina, &r, &causa));
}

interior vacuum
_sectio_inscriptio (
    CredoContextus* c)
{
    chorda tessera;
    chorda ambitus;
    chorda thema;
    chorda causa;

    (vacuum)c;
    CREDO_VERUM(pulsus_inscriptionem_legere(lit("{\"tessera\":\"a1b2\","
        "\"ambitus\":\"sandbox\",\"thema\":\"earth.golden.pictor\"}"),
        piscina, &tessera, &ambitus, &thema, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS(tessera, "a1b2");
    CREDO_CHORDA_AEQUALIS_LITERIS(ambitus, "sandbox");
    CREDO_CHORDA_AEQUALIS_LITERIS(thema, "earth.golden.pictor");
    CREDO_FALSUM(pulsus_inscriptionem_legere(lit("{\"tessera\":\"zz\","
        "\"ambitus\":\"sandbox\",\"thema\":\"x\"}"),
        piscina, &tessera, &ambitus, &thema, &causa));
    CREDO_CHORDA_CONTINET(causa, lit("tessera"));
    CREDO_FALSUM(pulsus_inscriptionem_legere(lit("{\"tessera\":\"abc\","
        "\"ambitus\":\"sandbox\",\"thema\":\"x\"}"),
        piscina, &tessera, &ambitus, &thema, &causa));
    CREDO_CHORDA_CONTINET(causa, lit("tessera"));
    CREDO_FALSUM(pulsus_inscriptionem_legere(lit("{\"tessera\":\"ab\","
        "\"ambitus\":\"evolutio\",\"thema\":\"x\"}"),
        piscina, &tessera, &ambitus, &thema, &causa));
    CREDO_CHORDA_CONTINET(causa, lit("ambitus"));
}

interior vacuum
_sectio_textus (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_CHORDA_AEQUALIS_LITERIS(pulsus_textum_facere(
        lit("{persona} shared {numerus} pictures"), lit("Fran"), III,
        piscina), "Fran shared 3 pictures");
    CREDO_CHORDA_AEQUALIS_LITERIS(pulsus_textum_facere(
        lit("{persona} shared a picture"), lit("Fran"), I, piscina),
        "Fran shared a picture");
    CREDO_CHORDA_AEQUALIS_LITERIS(pulsus_textum_facere(
        lit("{numerus} by {persona}"), lit("Amanda"), XII, piscina),
        "12 by Amanda");
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
    hic_manens constans character* constans IDS[] = {
        "R1:a", "R1:b", "R2:c"
    };
          chorda  corpus;
             Xar* multa;
             i32  i;
       character  id[LXIV];
    PulsusTactus  t;

    (vacuum)c;
    corpus = pulsus_corpus_facere(lit("pictor"),
        lit("Fran shared 3 pictures"), lit("pictor.picturae"),
        lit("CIVIS1"), res_ex(IDS, III), piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus,
        "{\"aps\":{\"alert\":{\"title\":\"pictor\",\"body\":"
        "\"Fran shared 3 pictures\"},\"sound\":\"default\","
        "\"thread-id\":\"pictor.picturae\"},\"pulsus\":{\"clavis\":"
        "\"pictor.picturae\",\"res\":[\"R1:a\",\"R1:b\",\"R2:c\"],"
        "\"a\":\"CIVIS1\"}}");

    /* CD res XL signorum: truncatur, plures, praefixum ordine */
    multa = xar_creare(piscina, magnitudo(chorda));
    per (i = ZEPHYRUM; i < CD; i++)
    {
        sprintf(id, "01JABCDEFGHJKMNPQRSTVWXYZ0:pict-%08u",
            (unsigned)i);
        *(chorda*)xar_addere(multa) = lit(id);
    }
    corpus = pulsus_corpus_facere(lit("pictor"), lit("Fran shared 400"),
        lit("pictor.picturae"), lit("CIVIS1"), multa, piscina);
    CREDO_MINOR_AUT_AEQUALIS_I32(corpus.mensura, PULSUS_CORPUS_MAXIMUM);
    CREDO_VERUM(pulsus_tactum_legere(corpus, piscina, &t));
    CREDO_VERUM(t.plures);
    CREDO_MAIOR_I32(xar_numerus(t.res), ZEPHYRUM);
    CREDO_MINOR_I32(xar_numerus(t.res), CD);
    per (i = ZEPHYRUM; i < xar_numerus(t.res); i++)
    {
        CREDO_CHORDA_AEQUALIS(*(chorda*)xar_obtinere(t.res, i),
            *(chorda*)xar_obtinere(multa, i));
    }
}

interior vacuum
_sectio_tactus (
    CredoContextus* c)
{
    hic_manens constans character* constans IDS[] = { "R1:a", "R2:c" };
    PulsusTactus t;
          chorda corpus;

    (vacuum)c;
    corpus = pulsus_corpus_facere(lit("pictor"), lit("Fran shared 2"),
        lit("pictor.picturae"), lit("CIVIS1"), res_ex(IDS, II),
        piscina);
    CREDO_VERUM(pulsus_tactum_legere(corpus, piscina, &t));
    CREDO_CHORDA_AEQUALIS_LITERIS(t.clavis, "pictor.picturae");
    CREDO_CHORDA_AEQUALIS_LITERIS(t.a, "CIVIS1");
    CREDO_AEQUALIS_I32(xar_numerus(t.res), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(t.res, I),
        "R2:c");
    CREDO_FALSUM(t.plures);
    CREDO_FALSUM(pulsus_tactum_legere(lit("{\"aps\":{}}"), piscina,
        &t));
    CREDO_FALSUM(pulsus_tactum_legere(lit("non json"), piscina, &t));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "eventus", _sectio_eventus, _parare, _purgare, NIHIL, NIHIL },
    { "inscriptio", _sectio_inscriptio, _parare, _purgare, NIHIL,
        NIHIL },
    { "textus", _sectio_textus, _parare, _purgare, NIHIL, NIHIL },
    { "corpus", _sectio_corpus, _parare, _purgare, NIHIL, NIHIL },
    { "tactus", _sectio_tactus, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pulsus_forma", SECTIONES);
}
