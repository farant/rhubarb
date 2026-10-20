/* xml_credo_schedae.c - schedae sectionum credo pro inventis xml.
 * Quaeque sectio HODIE frangitur et post emendationem transibit. */
#include "latina.h"
#include "xml.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "credo.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

interior b32
_chorda_ptr_eq_literis (
                chorda* ch,
    constans character* cstr)
{
    si (!ch)
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(*ch, cstr);
}

/* ---------- 1. profunditas in lectione ---------- */

interior vacuum
_profunditatem_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                    i32  profunditas)
{
            i8* alveus;
           i32  k;
           i32  positus;
        chorda  input;
    XmlResultus res;

    alveus  = (i8*)piscina_allocare(piscina,
        (memoriae_index)(profunditas * VII));
    positus = ZEPHYRUM;
    per (k = ZEPHYRUM; k < profunditas; k++)
    {
        memcpy(alveus + positus, "<a>", III);
        positus += III;
    }
    per (k = ZEPHYRUM; k < profunditas; k++)
    {
        memcpy(alveus + positus, "</a>", IV);
        positus += IV;
    }
    input.datum   = alveus;
    input.mensura = positus;
    res = xml_legere(input, piscina, intern);
    (vacuum)res;
}

interior vacuum
_sectio_profunditas_legere (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);

    /* Documentum bene formatum, profunditas C*M: successus aut status
     * erroris (limes profunditatis) licet - acervus exhaustus non. */
    CREDO_NON_RUIT(_profunditatem_legere(piscina, intern, C * M));
}

/* ---------- 2. profunditas in scriptione ---------- */

interior vacuum
_profunditatem_scribere (
                Piscina* piscina,
    InternamentumChorda* intern,
                    i32  profunditas)
{
    XmlNodus* radix_nodus;
    XmlNodus* currens;
    XmlNodus* novus;
         i32  k;
      chorda  scriptum;

    radix_nodus = xml_elementum_creare(piscina, intern, "a");
    currens     = radix_nodus;
    per (k = I; k < profunditas; k++)
    {
        novus = xml_elementum_creare(piscina, intern, "a");
        xml_liberum_addere(currens, novus);
        currens = novus;
    }
    scriptum = xml_scribere(radix_nodus, piscina, FALSUM);
    (vacuum)scriptum;
}

interior vacuum
_sectio_profunditas_scribere (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);

    CREDO_NON_RUIT(_profunditatem_scribere(piscina, intern, C * M));
}

/* ---------- 3. cyclus in xml_liberum_addere ---------- */

interior vacuum
_sectio_liberum_cyclus (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* pater;
        XmlNodus* filius;

        pater  = xml_elementum_creare(piscina, intern, "a");
        filius = xml_elementum_creare(piscina, intern, "b");

        /* Nodus sui ipsius liberum fieri non potest */
        CREDO_FALSUM(xml_liberum_addere(pater, pater));
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(pater), ZEPHYRUM);

        /* Avus liberum nepotis fieri non potest */
        CREDO_VERUM(xml_liberum_addere(pater, filius));
        CREDO_FALSUM(xml_liberum_addere(filius, pater));
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(filius), ZEPHYRUM);
    }
}

/* ---------- 4. parens unicus ---------- */

interior vacuum
_sectio_liberum_parens_unicus (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* primus;
        XmlNodus* secundus;
        XmlNodus* liberum;

        primus   = xml_elementum_creare(piscina, intern, "a");
        secundus = xml_elementum_creare(piscina, intern, "b");
        liberum  = xml_elementum_creare(piscina, intern, "c");

        CREDO_VERUM(xml_liberum_addere(primus, liberum));
        (vacuum)xml_liberum_addere(secundus, liberum);

        /* Recusatio aut translatio: liberum in UNA tabula tantum, et
         * ea est tabula parentis quem liberum->parens nominat. */
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(primus)
            + xml_numerus_liberorum(secundus), I);
        CREDO_AEQUALIS_PTR(xml_liberum_ad_indicem(liberum->parens,
            ZEPHYRUM), liberum);

        /* Idem liberum bis: numquam duplicatum */
        (vacuum)xml_liberum_addere(secundus, liberum);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(primus)
            + xml_numerus_liberorum(secundus), I);
    }
}

/* ---------- 5. xml_legere_ex_literis(NIHIL) ---------- */

interior vacuum
_sectio_ex_literis_nihil (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);

    CREDO_NON_RUIT(xml_legere_ex_literis(NIHIL, piscina, intern));
}

/* ---------- 6. XML_ERROR_MEMORIA in piscina certa ---------- */

/* In filio: si xml_legere REDIT cum XML_ERROR_MEMORIA, SIGTERM
 * mittit (signum = testimonium reditus). exire(I) intra allocatorem
 * exitus normalis est, non signum - ergo hodie frangitur. */
interior vacuum
_memoriam_angustam_legere (
    InternamentumChorda* intern)
{
        Piscina* angusta;
    XmlResultus  res;

    angusta = piscina_generare_certae_magnitudinis("angusta", DXII);
    res = xml_legere_ex_literis(
        "<r><e k=\"v\">t</e><e k=\"v\">t</e><e k=\"v\">t</e>"
        "<e k=\"v\">t</e><e k=\"v\">t</e><e k=\"v\">t</e></r>",
        angusta, intern);
    si (!res.successus && res.status == XML_ERROR_MEMORIA)
    {
        (vacuum)raise(SIGTERM);
    }
}

interior vacuum
_sectio_memoria_status (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);

    CREDO_RUIT_CUM(_memoriam_angustam_legere(intern), SIGTERM);
}

/* ---------- 7. valor attributi vacuus in lectione ---------- */

interior vacuum
_sectio_attributum_vacuum_legere (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;
            chorda* valor;

        res = xml_legere_ex_literis("<a x=\"\"/>", piscina, intern);
        CREDO_VERUM(res.successus);
        valor = xml_attributum_capere(res.radix, "x");
        CREDO_NON_NIHIL(valor);
        si (valor)
        {
            CREDO_AEQUALIS_I32(valor->mensura, ZEPHYRUM);
        }

        res = xml_legere_ex_literis("<a x=''/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(xml_attributum_habet(res.radix, "x"));
    }
}

/* ---------- 8. valor attributi vacuus in constructione ---------- */

interior vacuum
_sectio_attributum_vacuum_addere (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
         chorda* valor;

        nodus = xml_elementum_creare(piscina, intern, "a");
        CREDO_VERUM(xml_attributum_addere(nodus, piscina, intern,
            "x", ""));
        CREDO_VERUM(xml_attributum_habet(nodus, "x"));
        valor = xml_attributum_capere(nodus, "x");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(xml_scribere(nodus, piscina,
            FALSUM), "<a x=\"\"/>");
    }
}

/* ---------- 9. nomen elementi vacuum ---------- */

interior vacuum
_sectio_elementum_nomen_vacuum (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;

        /* Aut NIHIL redditur, aut nodus cum titulo - numquam nodus
         * cuius titulus NIHIL est (scriptor eum dereferentiat). */
        nodus = xml_elementum_creare(piscina, intern, "");
        CREDO_VERUM(nodus == NIHIL || nodus->titulus != NIHIL);
    }
}

/* ---------- 10. spatium initiale textus ---------- */

interior vacuum
_sectio_textus_spatium_initiale (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;
          XmlNodus* tertius;

        res = xml_legere_ex_literis("<a>  x</a>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(xml_textus_internus(res.radix,
            piscina), "  x");

        res = xml_legere_ex_literis("<a>x <b/> y</a>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(res.radix), III);
        tertius = xml_liberum_ad_indicem(res.radix, II);
        CREDO_NON_NIHIL(tertius);
        si (tertius)
        {
            CREDO_VERUM(_chorda_ptr_eq_literis(tertius->valor, " y"));
        }

        /* Reditus: textum_addere(" lead") -> scribere -> legere */
        {
            XmlNodus* nodus;
              chorda  scriptum;

            nodus = xml_elementum_creare(piscina, intern, "a");
            xml_textum_addere(nodus, piscina, intern, " lead");
            scriptum = xml_scribere(nodus, piscina, FALSUM);
            res = xml_legere(scriptum, piscina, intern);
            CREDO_VERUM(res.successus);
            CREDO_CHORDA_AEQUALIS_LITERIS(xml_textus_internus(res.radix,
                piscina), " lead");
        }
    }
}

/* ---------- 11. contentum post radicem ---------- */

interior vacuum
_sectio_contentum_post_radicem (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<a/><b/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<a/>trailing", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<a/></b>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("hello<a/>", piscina, intern);
        CREDO_FALSUM(res.successus);

        /* Licita post radicem et ante eam manent licita */
        res = xml_legere_ex_literis("<a/>\n<!-- c -->\n<?pi x?>\n",
            piscina, intern);
        CREDO_VERUM(res.successus);
        res = xml_legere_ex_literis("\xEF\xBB\xBF<a/>", piscina, intern);
        CREDO_VERUM(res.successus);
    }
}

/* ---------- 12/13. DOCTYPE cum literis citatis ---------- */

interior vacuum
_sectio_doctype_citata (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<!DOCTYPE x SYSTEM \"a><b/>\"><c/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix)
        {
            CREDO_VERUM(_chorda_ptr_eq_literis(res.radix->titulus, "c"));
        }

        res = xml_legere_ex_literis("<!DOCTYPE x SYSTEM \"a[b\"><c/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, XML_SUCCESSUS);
    }
}

/* ---------- 14. commentum reditus ---------- */

interior vacuum
_sectio_commentum_reditus (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
        XmlResultus res;
        XmlNodus* primus;

        nodus = xml_elementum_creare(piscina, intern, "a");
        xml_liberum_addere(nodus, xml_commentum_creare(piscina, intern,
            "salve"));
        res = xml_legere(xml_scribere(nodus, piscina, FALSUM), piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(res.radix), I);
        primus = xml_liberum_ad_indicem(res.radix, ZEPHYRUM);
        CREDO_NON_NIHIL(primus);
        si (primus)
        {
            CREDO_AEQUALIS_I32(primus->genus, XML_NODUS_COMMENTUM);
        }
    }
}

/* ---------- 15. commentum cum '-->' ---------- */

interior vacuum
_sectio_commentum_iniectio (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
        XmlNodus* commentum;
        XmlResultus res;

        commentum = xml_commentum_creare(piscina, intern,
            "x --> <b/> <!-- y");
        si (commentum)
        {
            nodus = xml_elementum_creare(piscina, intern, "a");
            xml_liberum_addere(nodus, commentum);
            res = xml_legere(xml_scribere(nodus, piscina, FALSUM),
                piscina, intern);
            /* Textus commenti numquam elementum fit */
            CREDO_NIHIL(xml_invenire_liberum(res.radix, "b"));
        }
        alioquin
        {
            CREDO_NIHIL(commentum);
        }

        /* '--' intra commentum: recusandum */
        CREDO_NIHIL(xml_commentum_creare(piscina, intern, "a -- b -"));
    }
}

/* ---------- 16. nomen elementi invalidum ---------- */

interior vacuum
_sectio_elementum_nomen_invalidum (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);

    CREDO_NIHIL(xml_elementum_creare(piscina, intern, "a b"));
    CREDO_NIHIL(xml_elementum_creare(piscina, intern, "a><z"));
}

/* ---------- 17. attributa duplicata ---------- */

interior vacuum
_sectio_attributum_duplex (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
        XmlResultus res;

        nodus = xml_elementum_creare(piscina, intern, "a");
        xml_attributum_addere(nodus, piscina, intern, "x", "1");
        (vacuum)xml_attributum_addere(nodus, piscina, intern, "x", "2");
        /* Recusatio aut substitutio: unum attributum 'x' tantum */
        CREDO_AEQUALIS_I32(xar_numerus(nodus->attributa), I);

        res = xml_legere_ex_literis("<a x=\"1\" x=\"2\"/>", piscina,
            intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_ATTRIBUTUM);
    }
}

/* ---------- 18. tab et linea nova in valore attributi ---------- */

interior vacuum
_sectio_attributum_spatia_effugium (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
          chorda  scriptum;

        nodus = xml_elementum_creare(piscina, intern, "a");
        xml_attributum_addere(nodus, piscina, intern, "x", "p\tq\nr");
        scriptum = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis("&#9;", piscina));
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis("&#10;",
            piscina));
    }
}

/* ---------- 19. pulchrum in contento mixto ---------- */

interior vacuum
_sectio_pulchrum_mixtum (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlNodus* nodus;
        XmlResultus res;

        nodus = xml_elementum_creare(piscina, intern, "a");
        xml_textum_addere(nodus, piscina, intern, "x");
        xml_liberum_addere(nodus, xml_elementum_creare(piscina, intern,
            "b"));
        xml_textum_addere(nodus, piscina, intern, "y");

        res = xml_legere(xml_scribere(nodus, piscina, VERUM), piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(xml_textus_internus(res.radix,
            piscina), "xy");
    }
}

/* ---------- 20. linea erroris ---------- */

interior vacuum
_sectio_linea_erroris (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<a>\n\n<b></c></a>", piscina, intern);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_TAG_IMPROPRIE);
        CREDO_AEQUALIS_I32(res.linea_erroris, III);

        res = xml_legere_ex_literis(
            "<?xml version=\"1.0\"?>\n<plist>\n  <dict>\n"
            "    <key>k</key>\n    <string>v</strin>\n  </dict>\n</plist>",
            piscina, intern);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_TAG_IMPROPRIE);
        CREDO_AEQUALIS_I32(res.linea_erroris, V);

        res = xml_legere_ex_literis("<!--\n\n-->\n<a></c>", piscina,
            intern);
        CREDO_AEQUALIS_I32(res.linea_erroris, IV);
    }
}

/* ---------- 21. nomen UTF-8 ---------- */

interior vacuum
_sectio_nomen_utf8 (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<caf\xC3\xA9 r\xC3\xA9s=\"1\"/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        si (res.radix)
        {
            CREDO_VERUM(_chorda_ptr_eq_literis(res.radix->titulus,
                "caf\xC3\xA9"));
        }
    }
}

/* ---------- 22. '&' nudum -> XML_ERROR_EFFUGIUM ---------- */

interior vacuum
_sectio_effugium_nudum (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<a>a & b</a>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_EFFUGIUM);

        res = xml_legere_ex_literis("<a>&amp</a>", piscina, intern);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_EFFUGIUM);

        res = xml_legere_ex_literis("<a k=\"x&y\"/>", piscina, intern);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_EFFUGIUM);
    }
}

/* ---------- 23. nuntius erroris ---------- */

interior vacuum
_sectio_nuntius_erroris (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<a></b>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_CHORDA_NON_VACUA(res.error);
        CREDO_NON_NIHIL(res.error.datum);
    }
}

/* ---------- 24. lenitas syntaxis ---------- */

interior vacuum
_sectio_syntaxis_stricta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<a x=\"1\"y=\"2\"/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<a x=\"<\"/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<1a/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<-a/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        res = xml_legere_ex_literis("<a><!-- a -- b --></a>", piscina,
            intern);
        CREDO_FALSUM(res.successus);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "profunditas legere", _sectio_profunditas_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "profunditas scribere", _sectio_profunditas_scribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liberum cyclus", _sectio_liberum_cyclus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liberum parens unicus", _sectio_liberum_parens_unicus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ex_literis NIHIL", _sectio_ex_literis_nihil, NIHIL, NIHIL, NIHIL, NIHIL },
    { "memoria status", _sectio_memoria_status, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributum vacuum legere", _sectio_attributum_vacuum_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributum vacuum addere", _sectio_attributum_vacuum_addere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "elementum nomen vacuum", _sectio_elementum_nomen_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus spatium initiale", _sectio_textus_spatium_initiale, NIHIL, NIHIL, NIHIL, NIHIL },
    { "contentum post radicem", _sectio_contentum_post_radicem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "doctype citata", _sectio_doctype_citata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "commentum reditus", _sectio_commentum_reditus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "commentum iniectio", _sectio_commentum_iniectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "elementum nomen invalidum", _sectio_elementum_nomen_invalidum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributum duplex", _sectio_attributum_duplex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributum spatia effugium", _sectio_attributum_spatia_effugium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pulchrum mixtum", _sectio_pulchrum_mixtum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea erroris", _sectio_linea_erroris, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomen utf8", _sectio_nomen_utf8, NIHIL, NIHIL, NIHIL, NIHIL },
    { "effugium nudum", _sectio_effugium_nudum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nuntius erroris", _sectio_nuntius_erroris, NIHIL, NIHIL, NIHIL, NIHIL },
    { "syntaxis stricta", _sectio_syntaxis_stricta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("xml_schedae", SECTIONES);
}
