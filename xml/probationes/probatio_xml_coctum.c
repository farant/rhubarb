/* probatio_xml_coctum.c - Visio cocta: casus quos suite W3C non urget
 *
 * Limites (X4: billion laughs), profunditas coctionis (iterativa),
 * normalizatio attributorum (par. 3.3.3), praestituta, regula par. 5.1
 * (standalone), expansio duplex (Appendix D), spatia nominum, casus
 * auditus 2026-10-18 (spatia textus, a="", commentaria, attributa bis,
 * '&' nudum), sedes diagnosticorum in entitatibus (ad referentiam).
 */

#include "latina.h"
#include "credo.h"
#include "xml_coctum.h"
#include "materia_diagnosticum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior XmlCoctum
_coquere (
               Piscina* p,
    constans character* fons)
{
    redde xml_coquere(p, fons, (s32)strlen(fons), VERUM);
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

interior constans XmlCoctus*
_radix (
    constans XmlCoctum* k)
{
    i32 i;

    si (k->documentum == NIHIL || k->documentum->liberi == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(k->documentum->liberi); i++)
    {
        constans XmlCoctus* f = *(constans XmlCoctus**)xar_obtinere(
            k->documentum->liberi, i);

        si (f->genus == XML_COCTUS_ELEMENTUM)
        {
            redde f;
        }
    }
    redde NIHIL;
}

interior constans XmlCoctus*
_filius (
    constans XmlCoctus* x,
                   i32  i)
{
    redde x != NIHIL && x->liberi != NIHIL && i < xar_numerus(x->liberi)
        ? *(constans XmlCoctus**)xar_obtinere(x->liberi, i) : NIHIL;
}

interior constans XmlAttributumCoctum*
_attributum (
    constans XmlCoctus* x,
                   i32  i)
{
    redde x != NIHIL && x->attributa != NIHIL
        && i < xar_numerus(x->attributa)
        ? (constans XmlAttributumCoctum*)xar_obtinere(x->attributa, i)
        : NIHIL;
}

interior b32
_codex_adest (
    constans XmlCoctum* k,
    constans character* codex)
{
    i32 i;

    per (i = ZEPHYRUM; k->diagnostica != NIHIL
         && i < xar_numerus(k->diagnostica); i++)
    {
        si (strcmp(((constans MateriaDiagnosticum*)xar_obtinere(
            k->diagnostica, i))->codex, codex) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_limites_probare (
    CredoContextus* c)
{
    XmlCoctum k = _coquere(c->piscina,
        "<!DOCTYPE l [\n"
        "<!ENTITY a \"lollollollollollollollollollol\">\n"
        "<!ENTITY b \"&a;&a;&a;&a;&a;&a;&a;&a;&a;&a;\">\n"
        "<!ENTITY c \"&b;&b;&b;&b;&b;&b;&b;&b;&b;&b;\">\n"
        "<!ENTITY d \"&c;&c;&c;&c;&c;&c;&c;&c;&c;&c;\">\n"
        "<!ENTITY e \"&d;&d;&d;&d;&d;&d;&d;&d;&d;&d;\">\n"
        "<!ENTITY f \"&e;&e;&e;&e;&e;&e;&e;&e;&e;&e;\">\n"
        "<!ENTITY g \"&f;&f;&f;&f;&f;&f;&f;&f;&f;&f;\">\n"
        "<!ENTITY h \"&g;&g;&g;&g;&g;&g;&g;&g;&g;&g;\">\n"
        "<!ENTITY i \"&h;&h;&h;&h;&h;&h;&h;&h;&h;&h;\">\n"
        "]>\n<l>&i;</l>");

    CREDO_FALSUM (k.bene_formatum);
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_LIMES));

    k = _coquere(c->piscina,
        "<!DOCTYPE a [<!ENTITY e \"x&e;\">]><a>&e;</a>");
    CREDO_FALSUM (k.bene_formatum);
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_RECURSIO));
    k = _coquere(c->piscina,
        "<!DOCTYPE a [<!ENTITY e \"x&e;\">]><a b=\"&e;\"/>");
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_RECURSIO));
}

interior vacuum
_profunditatem_probare (
    CredoContextus* c)
{
          i32  n = (i32)C * M;
    character* b = (character*)piscina_allocare(c->piscina,
        (memoriae_index)n * VII + I);
          i32 j;
          i32 m = ZEPHYRUM;
    XmlCoctum k;

    CREDO_NON_NIHIL (b);
    si (b == NIHIL)
    {
        redde;
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        memcpy(b + m, "<a>", III);
        m += III;
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        memcpy(b + m, "</a>", IV);
        m += IV;
    }
    k = xml_coquere(c->piscina, b, (s32)m, VERUM);
    CREDO_VERUM (k.bene_formatum);
}

interior vacuum
_attributa_probare (
    CredoContextus* c)
{
             XmlCoctum  k;
    constans XmlCoctus* r;

    /* TAB LF litterales -> spatium; &#10; et &#9; servantur; CR LF
     * fontis -> spatium unum */
    k = _coquere(c->piscina, "<a b=\"x\ty\nz&#10;w&#9;v\r\nu\"/>");
    r = _radix(&k);
    CREDO_NON_NIHIL (_attributum(r, ZEPHYRUM));
    si (_attributum(r, ZEPHYRUM) != NIHIL)
    {
        CREDO_VERUM (_aequat(_attributum(r, ZEPHYRUM)->valor,
            "x y z\nw\tv u"));
    }
    /* typus non CDATA contrahitur; praestitutum applicatur */
    k = _coquere(c->piscina,
        "<!DOCTYPE a [<!ATTLIST a t NMTOKENS #IMPLIED"
        " d CDATA ' p  q '>]><a t='  x   y '/>");
    r = _radix(&k);
    CREDO_VERUM (k.bene_formatum);
    CREDO_NON_NIHIL (_attributum(r, I));
    si (_attributum(r, I) != NIHIL)
    {
        CREDO_VERUM (_aequat(_attributum(r, ZEPHYRUM)->valor, "x y"));
        CREDO_VERUM (_aequat(_attributum(r, I)->titulus, "d"));
        CREDO_VERUM (_aequat(_attributum(r, I)->valor, " p  q "));
        CREDO_VERUM (_attributum(r, I)->praestitutum);
        CREDO_FALSUM (_attributum(r, ZEPHYRUM)->praestitutum);
    }
    /* casus auditus: a="" validum, attributum bis recusatum */
    k = _coquere(c->piscina, "<a b=\"\"/>");
    CREDO_VERUM (k.bene_formatum);
    k = _coquere(c->piscina, "<a b='1' b='2'/>");
    CREDO_FALSUM (k.bene_formatum);
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_ATTRIBUTUM_BIS));
    /* '<' per entitatem in valorem (WFC: No < in Attribute Values) */
    k = _coquere(c->piscina, "<!DOCTYPE a [<!ENTITY e \"<\">]>"
        "<a b=\"&e;\"/>");
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_MINOR_IN_VALORE));
}

interior vacuum
_regulam_probare (
    CredoContextus* c)
{
             XmlCoctum  k;
    constans XmlCoctus* r;

    /* par. 5.1: post %e; externam non lectam ATTLIST sequens non
     * tractatur (xmltest valid/sa/097) */
    k = _coquere(c->piscina,
        "<!DOCTYPE doc [<!ENTITY % e SYSTEM 'e.ent'>"
        "<!ATTLIST doc a1 CDATA 'v1'>%e;<!ATTLIST doc a2 CDATA 'v2'>]>"
        "<doc/>");
    r = _radix(&k);
    CREDO_VERUM (k.bene_formatum);
    CREDO_NON_NIHIL (_attributum(r, ZEPHYRUM));
    CREDO_NIHIL (_attributum(r, I));
    /* standalone='yes': regula non cessat */
    k = _coquere(c->piscina, "<?xml version='1.0' standalone='yes'?>"
        "<!DOCTYPE doc [<!ENTITY % e SYSTEM 'e.ent'>"
        "<!ATTLIST doc a1 CDATA 'v1'>%e;<!ATTLIST doc a2 CDATA 'v2'>]>"
        "<doc/>");
    r = _radix(&k);
    CREDO_NON_NIHIL (_attributum(r, I));
    /* entitas non declarata: sine DTD vitium, post %e; externam non */
    k = _coquere(c->piscina, "<a>&x;</a>");
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_ENTITAS_IGNOTA));
    k = _coquere(c->piscina,
        "<!DOCTYPE a [<!ENTITY % e SYSTEM 'e'>%e;]>"
        "<a>&x;</a>");
    CREDO_VERUM (k.bene_formatum);
}

interior vacuum
_expansionem_probare (
    CredoContextus* c)
{
             XmlCoctum  k;
    constans XmlCoctus* r;

    /* Appendix D: '&#38;#60;' -> textus substitutionis '&#60;' ->
     * characteris '<' in contento (textus) */
    k = _coquere(c->piscina, "<!DOCTYPE a [<!ENTITY e \"&#38;#60;\">]>"
        "<a>&e;</a>");
    r = _radix(&k);
    CREDO_VERUM (k.bene_formatum);
    CREDO_NON_NIHIL (_filius(r, ZEPHYRUM));
    si (_filius(r, ZEPHYRUM) != NIHIL)
    {
        CREDO_VERUM (_aequat(_filius(r, ZEPHYRUM)->textus, "<"));
    }
    /* '&#60;' -> '<' marcatio in contento: non bene formatum, sedes ad
     * referentiam (offset XXXVII) */
    k = _coquere(c->piscina, "<!DOCTYPE a [<!ENTITY e \"&#60;\">]>"
        "<a>&e;</a>");
    CREDO_FALSUM (k.bene_formatum);
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_CONTENTUM_ENTITATIS));
    si (k.diagnostica != NIHIL && xar_numerus(k.diagnostica) > ZEPHYRUM)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)
            xar_obtinere(k.diagnostica, ZEPHYRUM);

        CREDO_AEQUALIS_S32 (d->tractus.initium, (s32)XXXVII);
    }
    /* entitas cum elementis: liberi cocti */
    k = _coquere(c->piscina, "<!DOCTYPE a [<!ENTITY e \"<b>x</b>y\">]>"
        "<a>&e;</a>");
    r = _radix(&k);
    CREDO_VERUM (k.bene_formatum);
    CREDO_NON_NIHIL (_filius(r, I));
    si (_filius(r, I) != NIHIL)
    {
        CREDO_AEQUALIS_S32 ((s32)_filius(r, ZEPHYRUM)->genus,
            (s32)XML_COCTUS_ELEMENTUM);
        CREDO_VERUM (_aequat(_filius(r, I)->textus, "y"));
    }
}

interior vacuum
_spatia_probare (
    CredoContextus* c)
{
             XmlCoctum  k;
    constans XmlCoctus* r;

    k = _coquere(c->piscina, "<p:a xmlns:p='urn:x' xmlns='urn:d'>"
        "<b p:c='1'/></p:a>");
    r = _radix(&k);
    CREDO_VERUM (k.bene_formatum);
    si (r != NIHIL)
    {
        CREDO_VERUM (_aequat(r->spatium, "urn:x"));
        CREDO_VERUM (_aequat(r->localis, "a"));
        CREDO_VERUM (_aequat(_filius(r, ZEPHYRUM)->spatium, "urn:d"));
        CREDO_VERUM (_aequat(_attributum(_filius(r, ZEPHYRUM), ZEPHYRUM)
            ->spatium, "urn:x"));
    }
    k = _coquere(c->piscina, "<q:a/>");
    CREDO_VERUM (_codex_adest(&k, XML_CODEX_SPATIUM_NOMINUM));
    k = _coquere(c->piscina, "<a xmlns:p=''/>");
    CREDO_FALSUM (k.bene_formatum);
    k = _coquere(c->piscina, "<a:b:c/>");
    CREDO_FALSUM (k.bene_formatum);
    /* sine spatiis: idem documentum bene formatum */
    k = xml_coquere(c->piscina, "<a:b:c/>", VIII, FALSUM);
    CREDO_VERUM (k.bene_formatum);
}

interior vacuum
_auditum_probare (
    CredoContextus* c)
{
             XmlCoctum  k;
    constans XmlCoctus* r;

    /* spatium solum textus servatur */
    k = _coquere(c->piscina, "<a> </a>");
    r = _radix(&k);
    CREDO_NON_NIHIL (_filius(r, ZEPHYRUM));
    si (_filius(r, ZEPHYRUM) != NIHIL)
    {
        CREDO_VERUM (_aequat(_filius(r, ZEPHYRUM)->textus, " "));
    }
    /* spatia ducentia et sequentia servantur; commentaria manent */
    k = _coquere(c->piscina, "<a>\n x <!-- c --> y\n</a>");
    r = _radix(&k);
    CREDO_NON_NIHIL (_filius(r, II));
    si (_filius(r, II) != NIHIL)
    {
        CREDO_VERUM (_aequat(_filius(r, ZEPHYRUM)->textus, "\n x "));
        CREDO_AEQUALIS_S32 ((s32)_filius(r, I)->genus,
            (s32)XML_COCTUS_COMMENTARIUM);
        CREDO_VERUM (_aequat(_filius(r, II)->textus, " y\n"));
    }
    /* '&' nudum recusatur; radix ficta intra litteralem non */
    k = _coquere(c->piscina, "<a>&</a>");
    CREDO_FALSUM (k.bene_formatum);
    k = _coquere(c->piscina, "<a b='<b>'/>");
    CREDO_FALSUM (k.bene_formatum);
    /* CR LF in textu -> LF */
    k = _coquere(c->piscina, "<a>x\r\ny\rz</a>");
    r = _radix(&k);
    si (_filius(r, ZEPHYRUM) != NIHIL)
    {
        CREDO_VERUM (_aequat(_filius(r, ZEPHYRUM)->textus, "x\ny\nz"));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "limites: billion laughs, recursio", _limites_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "profunditas: C milia coquuntur", _profunditatem_probare,
        NIHIL, NIHIL, "gravis", NIHIL },
    { "attributa: normalizatio, typi, praestituta", _attributa_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "regula par. 5.1 et entitates ignotae", _regulam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "expansio: Appendix D, sedes, elementa", _expansionem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatia nominum", _spatia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "casus auditus 2026-10-18", _auditum_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_coctum", SECTIONES, NIHIL,
        NIHIL);
}
