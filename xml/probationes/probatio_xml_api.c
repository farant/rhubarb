/* probatio_xml_api.c - API publica xml_documentum.h
 *
 * Lectio, successus, diagnostica reddita ('via:linea:columna'),
 * ambulatio (xml_filius/xml_frater, xml_liberum), attributa (QName,
 * nomen expansum, praestituta), textus internus, casus extremi
 * (documentum vacuum, BOM solum, attributum absens, liberi non
 * elementa), argumenta NIHIL.
 */

#include "latina.h"
#include "credo.h"
#include "xml_documentum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior XmlDocumentum*
_legere (
               Piscina* p,
    constans character* fons)
{
    redde xml_documentum_legere(chorda_ex_literis(fons, p), p);
}

interior b32
_aequat (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n && (n == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)n) == ZEPHYRUM);
}

interior b32
_continet (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_ambulatio_probare (
    CredoContextus* c)
{
    XmlDocumentum* d = _legere(c->piscina,
        "<score>\n  <part id='P1'><measure n='1'/>"
        "<measure n='2'/></part>\n  <!-- c -->\n"
        "  <part id='P2'><measure n='1'/></part>\n</score>");
    constans XmlCoctus* r;
    constans XmlCoctus* p;
    constans XmlCoctus* m;
                chorda  v;
                   i32  n = ZEPHYRUM;

    CREDO_NON_NIHIL (d);
    CREDO_VERUM (xml_documentum_successus(d));
    r = xml_documentum_radix(d);
    CREDO_VERUM (_aequat(xml_titulus(r), "score"));
    CREDO_AEQUALIS_I32 (xml_filiorum_numerus(r, "part"), (i32)II);
    CREDO_AEQUALIS_I32 (xml_filiorum_numerus(r, NIHIL), (i32)II);
    /* liberi omnes: textus spatiorum et commentarium inclusi */
    CREDO_AEQUALIS_I32 (xml_liberorum_numerus(r), (i32)VII);
    CREDO_AEQUALIS_S32 ((s32)xml_nodi_genus(xml_liberum(r, ZEPHYRUM)),
        (s32)XML_COCTUS_TEXTUS);
    per (p = xml_filius(r, "part"); p != NIHIL; p = xml_frater(p,
        "part"))
    {
        n++;
        per (m = xml_filius(p, "measure"); m != NIHIL;
             m = xml_frater(m, "measure"))
        {
            CREDO_VERUM (xml_attributum(m, "n", &v));
        }
    }
    CREDO_AEQUALIS_I32 (n, (i32)II);
    p = xml_frater(xml_filius(r, "part"), "part");
    CREDO_VERUM (xml_attributum(p, "id", &v));
    CREDO_VERUM (_aequat(v, "P2"));
    CREDO_FALSUM (xml_attributum(p, "absens", &v));
    CREDO_NIHIL (xml_filius(p, "nullum"));

    /* fratres mixti: xml_frater titulum comparat */
    d = _legere(c->piscina, "<r><a/><b/><a/><c/></r>");
    r = xml_documentum_radix(d);
    n = ZEPHYRUM;
    per (p = xml_filius(r, "a"); p != NIHIL; p = xml_frater(p, "a"))
    {
        n++;
    }
    CREDO_AEQUALIS_I32 (n, (i32)II);
    CREDO_VERUM (_aequat(xml_titulus(xml_frater(xml_filius(r, "b"),
        NIHIL)), "a"));
}

interior vacuum
_attributa_probare (
    CredoContextus* c)
{
    XmlDocumentum* d = _legere(c->piscina,
        "<!DOCTYPE a [<!ATTLIST a v CDATA 'praest'>]>"
        "<a xmlns:x='urn:x' x:k='1' k='2'>t<b>u</b>v</a>");
    constans XmlCoctus* r = xml_documentum_radix(d);
                chorda  v;

    CREDO_VERUM (xml_documentum_successus(d));
    CREDO_VERUM (xml_attributum(r, "x:k", &v));
    CREDO_VERUM (_aequat(v, "1"));
    CREDO_VERUM (xml_attributum_spatii(r, "urn:x", "k", &v));
    CREDO_VERUM (_aequat(v, "1"));
    CREDO_VERUM (xml_attributum_spatii(r, "", "k", &v));
    CREDO_VERUM (_aequat(v, "2"));
    CREDO_VERUM (xml_attributum(r, "v", &v));
    CREDO_VERUM (_aequat(v, "praest"));
    CREDO_VERUM (xml_textus_totus(r, c->piscina, &v));
    CREDO_VERUM (_aequat(v, "tuv"));
}

interior vacuum
_diagnostica_probare (
    CredoContextus* c)
{
        XmlDocumentum* d = _legere(c->piscina, "<a>\n  <b>\n</a>\n");
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
               chorda  t;

    CREDO_NON_NIHIL (d);
    CREDO_FALSUM (xml_documentum_successus(d));
    CREDO_NIHIL (xml_documentum_radix(d));
    CREDO_MAIOR_I32 (xar_numerus(xml_documentum_diagnostica(d)),
        ZEPHYRUM);
    CREDO_VERUM (xml_documentum_diagnostica_scribere(d, "x.xml", a));
    t = chorda_aedificator_finire(a);
    CREDO_VERUM (_continet(t, "x.xml:2:3"));
    CREDO_VERUM (_continet(t, "tag finalis deest"));
}

interior vacuum
_extrema_probare (
    CredoContextus* c)
{
    XmlDocumentum* d;
           chorda  v;

    d = _legere(c->piscina, "");
    CREDO_NON_NIHIL (d);
    CREDO_FALSUM (xml_documentum_successus(d));
    d = _legere(c->piscina, "\xef\xbb\xbf");
    CREDO_FALSUM (xml_documentum_successus(d));
    d = _legere(c->piscina, "<a/>");
    CREDO_VERUM (xml_documentum_successus(d));
    CREDO_AEQUALIS_I32 (xml_liberorum_numerus(xml_documentum_radix(d)),
        ZEPHYRUM);
    CREDO_VERUM (xml_textus_totus(xml_documentum_radix(d),
        c->piscina,
        &v));
    CREDO_AEQUALIS_I32 (v.mensura, ZEPHYRUM);
    /* NIHIL ubique */
    CREDO_NIHIL (xml_documentum_legere(chorda_ex_literis("<a/>",
        c->piscina), NIHIL));
    CREDO_FALSUM (xml_documentum_successus(NIHIL));
    CREDO_NIHIL (xml_documentum_radix(NIHIL));
    CREDO_NIHIL (xml_documentum_diagnostica(NIHIL));
    CREDO_NIHIL (xml_filius(NIHIL, "a"));
    CREDO_NIHIL (xml_frater(NIHIL, "a"));
    CREDO_NIHIL (xml_liberum(NIHIL, ZEPHYRUM));
    CREDO_AEQUALIS_I32 (xml_liberorum_numerus(NIHIL), ZEPHYRUM);
    CREDO_FALSUM (xml_attributum(NIHIL, "a", &v));
    CREDO_FALSUM (xml_attributum_spatii(NIHIL, "", "a", &v));
    CREDO_FALSUM (xml_textus_totus(NIHIL, c->piscina, &v));
    CREDO_AEQUALIS_I32 (xml_titulus(NIHIL).mensura, ZEPHYRUM);
    /* index extra fines */
    d = _legere(c->piscina, "<a>x</a>");
    CREDO_NIHIL (xml_liberum(xml_documentum_radix(d), (i32)V));
}

/* casus domus ex probatio_xml.c veteris (P15) servati:
 * (a) referentiae SEMEL decodificantur - "&amp;#67;" textus "&#67;"
 *     manet (recensio horrei I3); &#13; CR verum, non normatur;
 * (b) attributum vacuum (xmlns="", alt='') validum - OPF EPUB
 *     mercatorium id fert; valor vacuus, attributa sequentia leguntur;
 * (c) DOCTYPE Apple per lineas plures, pars interna cum entitate:
 *     vetus parsator eam RECUSABAT, novus legit et expandit;
 * (d) referentia mala (&#; &#xZZ; &#1114112;) iam erratum est, non
 *     textus litteralis. */
interior vacuum
_casus_domus_probare (
    CredoContextus* c)
{
         XmlDocumentum* d;
    constans XmlCoctus* r;
                 chorda  v;

    d = _legere(c->piscina,
        "<d a=\"&#65;&#x42;&amp;#67;&#13;\">&#65;&#x42;&amp;#67;"
        "&#x1234;&#13;|</d>");
    CREDO_NECESSE (xml_documentum_successus(d));
    r = xml_documentum_radix(d);
    CREDO_VERUM (xml_textus_totus(r, c->piscina, &v));
    CREDO_VERUM (_aequat(v, "AB&#67;\xE1\x88\xB4\r|"));
    CREDO_VERUM (xml_attributum(r, "a", &v));
    CREDO_VERUM (_aequat(v, "AB&#67;\r"));

    d = _legere(c->piscina,
        "<meta xmlns=\"\" alt='' name=\"cover\" content=\"c\"/>");
    CREDO_NECESSE (xml_documentum_successus(d));
    r = xml_documentum_radix(d);
    CREDO_VERUM (xml_attributum(r, "alt", &v));
    CREDO_AEQUALIS_I32 (v.mensura, ZEPHYRUM);
    CREDO_VERUM (xml_attributum(r, "content", &v));
    CREDO_VERUM (_aequat(v, "c"));
    CREDO_AEQUALIS_I32 (xml_elementi_spatium(r).mensura, ZEPHYRUM);

    d = _legere(c->piscina,
        "<!DOCTYPE plist PUBLIC\n"
        "  \"-//Apple//DTD PLIST 1.0//EN\"\n"
        "  \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
        "<plist version=\"1.0\"><array/></plist>");
    CREDO_VERUM (xml_documentum_successus(d));
    CREDO_VERUM (_aequat(xml_titulus(xml_documentum_radix(d)),
        "plist"));
    d = _legere(c->piscina, "<!DOCTYPE x [ <!ENTITY a \"b\"> ]>\n"
        "<x>&a;</x>");
    CREDO_VERUM (xml_documentum_successus(d));
    CREDO_VERUM (xml_textus_totus(xml_documentum_radix(d), c->piscina,
        &v));
    CREDO_VERUM (_aequat(v, "b"));

    CREDO_FALSUM (xml_documentum_successus(_legere(c->piscina,
        "<d>&#;</d>")));
    CREDO_FALSUM (xml_documentum_successus(_legere(c->piscina,
        "<d>&#xZZ;</d>")));
    CREDO_FALSUM (xml_documentum_successus(_legere(c->piscina,
        "<d>&#1114112;</d>")));
}

/* xml_locus: sedes fontis nodi cocti (linea, columna I-basatae);
 * nodus ex entitate sedem referentiae reddit */
interior vacuum
_locus_probare (
    CredoContextus* c)
{
    XmlDocumentum* d = _legere(c->piscina,
        "<a>\n  <b x=\"1\"/>\n</a>");
    constans XmlCoctus* r;
    constans XmlCoctus* e;
                   i32  linea    = ZEPHYRUM;
                   i32  columna  = ZEPHYRUM;

    CREDO_NECESSE (xml_documentum_successus(d));
    r = xml_documentum_radix(d);
    CREDO_VERUM (xml_locus(r, &linea, &columna));
    CREDO_AEQUALIS_I32 (linea, (i32)I);
    CREDO_AEQUALIS_I32 (columna, (i32)I);
    CREDO_VERUM (xml_locus(xml_filius(r, "b"), &linea, &columna));
    CREDO_AEQUALIS_I32 (linea, (i32)II);
    CREDO_AEQUALIS_I32 (columna, (i32)III);
    CREDO_FALSUM (xml_locus(NIHIL, &linea, &columna));

    d = _legere(c->piscina, "<!DOCTYPE x [ <!ENTITY a \"<e/>\"> ]>\n"
        "<x>\n  &a;</x>");
    CREDO_NECESSE (xml_documentum_successus(d));
    e = xml_filius(xml_documentum_radix(d), "e");
    CREDO_NON_NIHIL (e);
    CREDO_VERUM (xml_locus(e, &linea, &columna));
    CREDO_AEQUALIS_I32 (linea, (i32)III);
    CREDO_AEQUALIS_I32 (columna, (i32)III);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "xml_locus: sedes fontis, entitas -> referentia", _locus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambulatio: filius, frater, liberum", _ambulatio_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributa: QName, spatium, praestitutum, textus internus",
        _attributa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "diagnostica reddita", _diagnostica_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "extrema et NIHIL", _extrema_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "casus domus (ex probatio_xml veteris)", _casus_domus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_api", SECTIONES, NIHIL, NIHIL);
}
