/* probatio_stml_exemplaria.c - EXEMPLAR/PER/TRANSPARENTIA (gradus II)
 *
 * Spec: project-specs/stml-exemplaria-spec.md par. 2-5. Fixturae
 * unitatis per regulam: congruentia laxa (subsequentia ordinata,
 * capturae, aequalitas iterata, '<*>'), applicatio fluitans contra
 * ancoratam, modus, TRANSPARENTIA (caecitas mensurabilis), visio
 * contenti (fragmenta dissolvuntur, transclusiones praeteritae),
 * catenatio de=, pons PER (textus + splex + voca=), lex stratorum
 * (supra solum), vitium quodque XII-XIX, et CATENA (involucrum
 * filans: forma nominata aequivalens, '<(>' ornamentum, planatio,
 * nexus ex templis portati, modus gradui, vitia XX).
 *
 * INVARIATUM ubique: EXEMPLAR/PER/TRANSPARENTIA numquam in arbore
 * expansa - comparationes chordarum id ipsae asserunt.
 */
#include "latina.h"
#include "stml.h"
#include "stml_macros.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>

/* Fistula brevis: legere + expandere ex litteris */
interior StmlExpansioResultus
_expandere_litteras (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons)
{
            StmlResultus res;
    StmlExpansioResultus expansio;

    res = stml_legere(chorda_ex_literis(fons, piscina), piscina,
                      intern);
    si (!res.successus)
    {
        expansio.successus           = FALSUM;
        expansio.radix_expansa       = NIHIL;
        expansio.vitium              = STML_EXPANSIO_BENE;
        expansio.linea               = ZEPHYRUM;
        expansio.tabula_expansionum  = NIHIL;
        expansio.fragmentum.datum    = NIHIL;
        expansio.fragmentum.mensura  = ZEPHYRUM;
        expansio.loculus.datum       = NIHIL;
        expansio.loculus.mensura     = ZEPHYRUM;
        redde expansio;
    }
    redde stml_expandere(res.radix, piscina, intern);
}

    /* --- index sui ipsius: fluitans + PER textus (spec par. 3) --- */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><sectio titulus=\"a\"/><sectio titulus=\"b\"/>"
            "<EXEMPLAR output=\"$s\"><sectio titulus=\"$t\"/>"
            "</EXEMPLAR>"
            "<index><PER congruentia=\"$s\"><res>&@t;</res></PER>"
            "</index></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><sectio titulus=\"a\"/>"
                "<sectio titulus=\"b\"/>"
                "<index><res>a</res><res>b</res></index></radix>");
        }
    }
}

    /* --- captura nodi + splex silvae in PER --- */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><x n=\"1\"/></p>"
            "<EXEMPLAR output=\"$m\"><p $n/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><copia>&@n;</copia></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x n=\"1\"/></p>"
                "<copia><p><x n=\"1\"/></p></copia></radix>");
        }
    }
}

    /* --- subsequentia ordinata: A..C congruit, C..A non --- */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><f><a/><b/><c/></f>"
            "<EXEMPLAR output=\"$m\"><f><a/><c/></f></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><f><a/><b/><c/></f><n/></radix>");
        }
        e = _expandere_litteras(piscina, intern,
            "<radix><f><a/><b/><c/></f>"
            "<EXEMPLAR output=\"$m\"><f><c/><a/></f></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            /* ordo violatus: nulla congruentia, nulla <n/> */
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><f><a/><b/><c/></f></radix>");
        }
    }
}

    /* --- '<*>' elementum quodlibet --- */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><f><q/></f><g/>"
            "<EXEMPLAR output=\"$m\"><f><*/></f></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><f><q/></f><g/><n/></radix>");
        }
    }
}

    /* --- captura iterata = aequalitas (regula V) --- */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><par a=\"x\" b=\"x\"/><par a=\"x\" b=\"y\"/>"
            "<EXEMPLAR output=\"$m\"><par a=\"$v\" b=\"$v\"/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><e>&@v;</e></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            /* prima sola (b=y discrepat) */
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><par a=\"x\" b=\"x\"/><par a=\"x\" b=\"y\"/>"
                "<e>x</e></radix>");
        }
    }
}

    /* --- captura iterata SUB TRANSPARENTIA (materia-sedes A3) --- */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* attributa perspicua: valores diversi, nodi aequales */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
            "<TRANSPARENTIA tags=\"nihil\" attributa=\"s\"/>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
                "<r/></radix>");
        }
        /* tagi perspicui: subarbor diversa, nodi aequales */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x><t>1</t>a</x><y><x>a</x></y></p>"
            "<TRANSPARENTIA tags=\"t\"/>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x><t>1</t>a</x><y><x>a</x></y></p>"
                "<r/></radix>");
        }
        /* sine TRANSPARENTIA: attributa diversa non congruunt */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
                "</radix>");
        }
    }
}

    /* --- captura iterata: FORMA AUCTORIS neglecta (materia-sedes A3,
     * quaestio 01M2Q2BDH4) - dispositio, '</>', formae capturae,
     * indentatio multilineae; contentum manet --- */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* spatia inter nodos: liberum primum lineam novam possidet */
        e = _expandere_litteras(piscina, intern,
            "<radix><l>\n  <i>x</i>\n  <i>x</i>\n</l>"
            "<EXEMPLAR output=\"$m\"><l><i $n/><i $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l>\n  <i>x</i>\n  <i>x</i>\n</l><r/></radix>");
        }
        /* profunditas diversa sub TRANSPARENTIA attributorum */
        e = _expandere_litteras(piscina, intern,
            "<radix><p>\n  <x s=\"1\">a</x>\n  <y>\n"
            "    <x s=\"2\">a</x>\n  </y>\n</p>"
            "<TRANSPARENTIA tags=\"nihil\" attributa=\"s\"/>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p>\n  <x s=\"1\">a</x>\n  <y>\n"
                "    <x s=\"2\">a</x>\n  </y>\n</p><r/></radix>");
        }
        /* clausura anonyma contra nominatam */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><i>x</i><i>x</></l>"
            "<EXEMPLAR output=\"$m\"><l><i $n/><i $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><i>x</i><i>x</></l><r/></radix>");
        }
        /* forma capturae '(>' contra liberos expressos */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><i><j/></i><i(> <j/></l>"
            "<EXEMPLAR output=\"$m\"><l><i $n/><i $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><i><j/></i><i(> <j/></l><r/></radix>");
        }
        /* spatia intra tagum et ante attributum */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><i a=\"1\" >x</i><i  a=\"1\">x</i></l>"
            "<EXEMPLAR output=\"$m\"><l><i $n/><i $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><i a=\"1\" >x</i><i  a=\"1\">x</i></l>"
                "<r/></radix>");
        }
        /* indentatio multilineae: lineae eaedem, praefixum diversum */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><t\\>\n  a\n  b\n</t>"
            "<k><t\\>\n      a\n      b\n</t></k></l>"
            "<EXEMPLAR output=\"$m\"><l><t $n/><k><t $n/></k></l>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><t\\>\n  a\n  b\n</t>"
                "<k><t\\>\n      a\n      b\n</t></k></l><r/></radix>");
        }
        /* CONTENTUM manet: spatia intra crudum textus sunt */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><s!> </s><s!>  </s></l>"
            "<EXEMPLAR output=\"$m\"><l><s $n/><s $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><s!> </s><s!>  </s></l></radix>");
        }
        /* ... et crudum aequale congruit (contra-probatio) */
        e = _expandere_litteras(piscina, intern,
            "<radix><l><s!> </s><s!> </s></l>"
            "<EXEMPLAR output=\"$m\"><l><s $n/><s $n/></l></EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><l><s!> </s><s!> </s></l><r/></radix>");
        }
    }
}

    /* --- ancorata: radix scopi sola --- */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix r=\"1\"><sectio/>"
            "<EXEMPLAR ancorata output=\"$m\"><radix r=\"$v\"/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix r=\"1\"><sectio/><n>1</n></radix>");
        }
        /* sectio exsistit INFRA radicem - ancorata eam non videt */
        e = _expandere_litteras(piscina, intern,
            "<radix><sectio/>"
            "<EXEMPLAR ancorata output=\"$m\"><sectio/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><sectio/></radix>");
        }
    }
}

    /* --- cursus fratrum (incrementum XX, 2026-09-07): congruentia
     * una per initium liberorum, primus fixus, saltus post --- */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* sine cursu: par UNUM avidum per parentem (inventum T17) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
            "<EXEMPLAR output=\"$m\">"
            "<s><w a=\"$x\"/><w a=\"$y\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;&@y;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
                "<p>12</p></radix>");
        }
        /* cum cursu: par per initium (12, 23); initium 2 sine
         * secundo */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
            "<EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s><w a=\"$x\"/><w a=\"$y\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;&@y;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
                "<p>12</p><p>23</p></radix>");
        }
        /* primus FIXUS: littera in primo libero initium eligit
         * (unum) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
            "<EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s><w a=\"2\"/><w a=\"$y\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@y;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/></s>"
                "<p>3</p></radix>");
        }
        /* saltus POST initium manent; initium in liberum alienum
         * cadit */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><x/><w a=\"2\"/><w a=\"3\"/></s>"
            "<EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s><w a=\"$x\"/><w a=\"$y\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;&@y;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><x/><w a=\"2\"/><w a=\"3\"/>"
                "</s><p>12</p><p>23</p></radix>");
        }
        /* capturae capitis per initia servatae; modus primum = initium
         * primum solum */
        e = _expandere_litteras(piscina, intern,
            "<radix><s r=\"k\"><w a=\"1\"/><w a=\"2\"/></s>"
            "<EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s r=\"$r\"><w a=\"$x\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@r;&@x;</p></PER>"
            "<EXEMPLAR cursus=\"fratrum\" modus=\"primum\""
            " output=\"$n\">"
            "<s><w a=\"$z\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$n\"><q>&@z;</q></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s r=\"k\"><w a=\"1\"/><w a=\"2\"/></s>"
                "<p>k1</p><p>k2</p><q>1</q></radix>");
        }
        /* ancorata + cursus: liberi radicis scopi */
        e = _expandere_litteras(piscina, intern,
            "<radix><w a=\"1\"/><w a=\"2\"/>"
            "<EXEMPLAR ancorata cursus=\"fratrum\" output=\"$m\">"
            "<radix><w a=\"$x\"/></radix></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n>&@x;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><w a=\"1\"/><w a=\"2\"/><n>1</n><n>2</n>"
                "</radix>");
        }
        /* '<**>' intra cursum: existentiale hoistatum per initium */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><w a=\"2\"><t/></w></s>"
            "<EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s><w a=\"$x\"/><**><t/></**></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><w a=\"2\"><t/></w></s>"
                "<p>1</p><p>2</p></radix>");
        }
        /* cursus STRICTUS (2026-09-07, ianua olim reservata): liberi
         * sequentes candidatum PROXIMUM solum - par 12 per <x/>
         * disiunctum cadit, 23 vicinum manet; initium quodque ut in
         * fratrum */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><x/><w a=\"2\"/><w a=\"3\"/></s>"
            "<EXEMPLAR cursus=\"strictus\" output=\"$m\">"
            "<s><w a=\"$x\"/><w a=\"$y\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;&@y;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><x/><w a=\"2\"/><w a=\"3\"/>"
                "</s><p>23</p></radix>");
        }
        /* strictus: liber tertius quoque proximus (123 solum, non
         * 1_23 saltu) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/><x/>"
            "<w a=\"4\"/></s>"
            "<EXEMPLAR cursus=\"strictus\" output=\"$m\">"
            "<s><w a=\"$x\"/><w a=\"$y\"/><w a=\"$z\"/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@x;&@y;&@z;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w a=\"1\"/><w a=\"2\"/><w a=\"3\"/><x/>"
                "<w a=\"4\"/></s><p>123</p></radix>");
        }
        /* vitia XIII: valor ignotus; forma sine libero ordinario */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w/></s>"
            "<EXEMPLAR cursus=\"alius\" output=\"$m\">"
            "<s><w/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
        e = _expandere_litteras(piscina, intern,
                        "<radix><s/><EXEMPLAR cursus=\"fratrum\" output=\"$m\">"
            "<s/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
    }
}

    /* --- retentatio (incrementum XXI, 2026-09-08): quaesitio
     * COMPLETA, primum a sinistra - liberum formae posterius cadens
     * electionem prioris retro tentat (lectionem proximam candidati
     * eiusdem, non candidatum proximum solum); regula V per
     * retentationem servata; electio prior MANET dum impletio ulla
     * exstat. OPTIO per EXEMPLAR quaesitio="completa" - defalta = lex
     * avida (mensurata 2026-09-08: quaesitio completa sub regulis
     * orationis litteralibus casum et ligationem deprimebat) --- */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* classicus: lectio prima c=1 cadit apud fratrem, lectio
         * secunda c=2 congruit - avidus nihil inveniebat */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/></w></s>"
            "<EXEMPLAR quaesitio=\"completa\" output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/>"
                "</w></s><p>2</p></radix>");
        }
        /* captura textus (forma orationis <casus>$c</casus>), gradus
         * duo infra fratrem */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a><c>1</c></a><a><c>2</c></a></w>"
            "<w><a><c>2</c></a></w></s>"
            "<EXEMPLAR quaesitio=\"completa\" output=\"$m\">"
            "<s><w><a><c>$c</c></a></w><w><a><c>$c</c></a></w></s>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a><c>1</c></a><a><c>2</c></a></w>"
                "<w><a><c>2</c></a></w></s><p>2</p></radix>");
        }
        /* ligamina prioris electionis SOLVUNTUR retentatione: n
         * lectionis secundae, non primae */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a n=\"1\" c=\"1\"/><a n=\"2\" c=\"2\"/></w>"
            "<w><a c=\"2\"/></w></s>"
            "<EXEMPLAR quaesitio=\"completa\" output=\"$m\">"
            "<s><w><a n=\"$a\" c=\"$c\"/></w><w><a c=\"$c\"/></w></s>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@a;&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a n=\"1\" c=\"1\"/><a n=\"2\" c=\"2\"/></w>"
                "<w><a c=\"2\"/></w></s><p>22</p></radix>");
        }
        /* electio prior MANET dum impletio exstat: lectio prima c=1
         * fratrem tertium saltu invenit ante quam lectio secunda
         * fratrem secundum - primum a sinistra, non proximum
         * (proximitas = cursus strictus) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/></w>"
            "<w><a c=\"1\"/></w></s>"
            "<EXEMPLAR cursus=\"fratrum\" quaesitio=\"completa\""
            " output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/>"
                "</w><w><a c=\"1\"/></w></s><p>1</p></radix>");
        }
        /* strictus + retentatio: vicinitas servata, retentatio INTRA
         * candidatum fixum (lectio secunda c=2 cum fratre proximo) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/></w>"
            "<w><a c=\"1\"/></w></s>"
            "<EXEMPLAR cursus=\"strictus\" quaesitio=\"completa\""
            " output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/>"
                "</w><w><a c=\"1\"/></w></s><p>2</p></radix>");
        }
        /* nulla impletio: quaesitio finit, ordines nulli */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"3\"/></w></s>"
            "<EXEMPLAR quaesitio=\"completa\" output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"3\"/>"
                "</w></s></radix>");
        }
        /* quaesitio OMNES: completio quaeque per initium ordo suus,
         * ordine quaesitionis (lectio prima cum fratre tertio c=1,
         * lectio secunda cum fratre secundo c=2); initium alterum
         * sine socio */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/></w>"
            "<w><a c=\"1\"/></w></s>"
            "<EXEMPLAR cursus=\"fratrum\" quaesitio=\"omnes\""
            " output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/>"
                "</w><w><a c=\"1\"/></w></s><p>1</p><p>2</p></radix>");
        }
        /* omnes classicus (sine cursu): completiones ambae, ordine
         * lectionum candidati primi; deinde nihil */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w>"
            "<w><a c=\"2\"/><a c=\"1\"/></w></s>"
            "<EXEMPLAR quaesitio=\"omnes\" output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w>"
                "<w><a c=\"2\"/><a c=\"1\"/></w></s><p>1</p><p>2</p>"
                "</radix>");
        }
        /* DEFALTA AVIDA: eadem forma sine quaesitio= nihil invenit
         * (lectio prima c=1 cadit, nulla retentatio) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/></w></s>"
            "<EXEMPLAR output=\"$m\">"
            "<s><w><a c=\"$c\"/></w><w><a c=\"$c\"/></w></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><p>&@c;</p></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><w><a c=\"1\"/><a c=\"2\"/></w><w><a c=\"2\"/>"
                "</w></s></radix>");
        }
        /* vitium XIII: valor quaesitionis ignotus */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><w/></s>"
            "<EXEMPLAR quaesitio=\"alia\" output=\"$m\">"
            "<s><w/></s></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
    }
}

    /* --- modus: unum / optional / primum --- */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s/><EXEMPLAR modus=\"unum\" output=\"$m\">"
            "<s/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);

        e = _expandere_litteras(piscina, intern,
            "<radix><s/><s/><EXEMPLAR modus=\"unum\" output=\"$m\">"
            "<s/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_UNUM_VIOLATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><s/><s/>"
            "<EXEMPLAR modus=\"optional\" output=\"$m\"><s/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_OPTIONAL_MULTIPLEX);

        e = _expandere_litteras(piscina, intern,
            "<radix><s k=\"1\"/><s k=\"2\"/>"
            "<EXEMPLAR modus=\"primum\" output=\"$m\">"
            "<s k=\"$k\"/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n>&@k;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s k=\"1\"/><s k=\"2\"/><n>1</n></radix>");
        }
    }
}

    /* --- TRANSPARENTIA: caecitas mensurabilis (spec par. 2.7) --- */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* SINE declaratione: fluitans intra <meta> descendit */
        e = _expandere_litteras(piscina, intern,
            "<radix><t><meta><id>NULL</id></meta></t>"
            "<EXEMPLAR output=\"$m\"><id>NULL</id></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><t><meta><id>NULL</id></meta></t><n/>"
                "</radix>");
        }
        /* CUM declaratione: subarbor meta invisibilis */
        e = _expandere_litteras(piscina, intern,
            "<radix><t><meta><id>NULL</id></meta></t>"
            "<TRANSPARENTIA tags=\"meta\"/>"
            "<EXEMPLAR output=\"$m\"><id>NULL</id></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><t><meta><id>NULL</id></meta></t></radix>");
        }
    }
}

    /* --- visio contenti: fragmentum dissolvitur in liberis --- */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><f><#g><a/></#></f>"
            "<EXEMPLAR output=\"$m\"><f><a/></f></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><f><#g><a/></#></f><n/></radix>");
        }
    }
}

    /* --- visio contenti: transclusio praeterita (identitas semel) --- */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><f><#g><a/></#><<#g>></f>"
            "<EXEMPLAR modus=\"unum\" output=\"$m\"><a/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        /* definitio numerat, transclusio non - congruentia UNA */
        CREDO_VERUM (e.successus);
    }
}

    /* --- catenatio de=: angustatio super relationem priorem --- */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q k=\"1\"/></p><p><r/></p>"
            "<EXEMPLAR output=\"$pp\"><p/></EXEMPLAR>"
            "<EXEMPLAR de=\"$pp\" output=\"$qq\"><q k=\"$k\"/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- lex stratorum: contentum INFRA non visum --- */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><EXEMPLAR output=\"$m\"><s/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER><s/></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s/></radix>");
        }
    }
}

    /* --- PER voca=: delegatio (pons = vocatio) --- */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f t=\"@t\"><li>&@t;</li></#>"
            "<s v=\"a\"/><s v=\"b\"/>"
            "<EXEMPLAR output=\"$m\"><s v=\"$t\"/></EXEMPLAR>"
            "<PER congruentia=\"$m\" voca=\"#@f\"/></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s v=\"a\"/><s v=\"b\"/>"
                "<li>a</li><li>b</li></radix>");
        }
    }
}

    /* --- relatio vacua: legalis, PER nihil parit --- */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><k/><EXEMPLAR output=\"$m\"><nusquam/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/></radix>");
        }
    }
}

    /* --- vitia XII, XIII, XIV, XV, XVIII, XIX --- */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* XII: sine output= */
        e = _expandere_litteras(piscina, intern,
            "<radix><EXEMPLAR><s/></EXEMPLAR></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_SINE_EXITU);

        /* XIII: modus ignotus */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><EXEMPLAR modus=\"cuncta\" output=\"$m\">"
            "<s/></EXEMPLAR></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        /* XIII: EXEMPLAR intra corpus templi (porta '@arg' nondum) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f><EXEMPLAR output=\"$x\"><a/></EXEMPLAR>"
            "</#><<#@f>></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        /* XIV: scopus ignotus */
        e = _expandere_litteras(piscina, intern,
            "<radix><PER congruentia=\"$nusquam\"><n/></PER>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SCOPUS_IGNOTUS);

        /* XV: corpus silvestre */
        e = _expandere_litteras(piscina, intern,
            "<radix><EXEMPLAR output=\"$m\"><a/><b/></EXEMPLAR>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CORPUS_SILVESTRE);

        /* XVIII: exitus non consumptus (custos finis, mollis -
         * ambulatio perfecta, successus cadit) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><EXEMPLAR output=\"$m\"><s/></EXEMPLAR>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_EXITUS_NON_CONSUMPTUS);
        CREDO_NON_NIHIL (e.radix_expansa);

        /* XIX: TRANSPARENTIA sine tags */
        e = _expandere_litteras(piscina, intern,
            "<radix><TRANSPARENTIA/></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_TRANSPARENTIA_MALFORMATA);
    }
}

    /* --- CATENA: catena basica == catenatio nominata --- */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p/></EXEMPLAR>"
            "<EXEMPLAR><q k=\"$k\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- CATENA: de= involucri (lex eadem ac EXEMPLAR) --- */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q k=\"1\"/></p><p><r/></p>"
            "<EXEMPLAR output=\"$pp\"><p/></EXEMPLAR>"
            "<CATENA de=\"$pp\" output=\"$qq\">"
            "<EXEMPLAR><q k=\"$k\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- CATENA: saccharum '<(>' = ornamentum (ratificatio V) --- */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\">"
            "<(><EXEMPLAR><p/></EXEMPLAR>"
            "<(><EXEMPLAR><q k=\"$k\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- CATENA nidificata litteralis: PLANATUR --- */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p/></EXEMPLAR>"
            "<CATENA><EXEMPLAR><q k=\"$k\"/></EXEMPLAR></CATENA>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- nexus ex templo: vocatio -> EXEMPLAR (portatio) --- */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@gq><EXEMPLAR><q k=\"$k\"/></EXEMPLAR></#>"
            "<p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p/></EXEMPLAR>"
            "<<#@gq>>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- nexus ex templo: vocatio -> CATENA (planatio) --- */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@gc><CATENA><EXEMPLAR><q k=\"$k\"/></EXEMPLAR>"
            "</CATENA></#>"
            "<p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p/></EXEMPLAR>"
            "<<#@gc>>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- nexus ex templo: silva graduum SPLICATUR --- */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@gs><EXEMPLAR><p/></EXEMPLAR>"
            "<EXEMPLAR><q k=\"$k\"/></EXEMPLAR></#>"
            "<p><q k=\"1\"/></p><p><r/></p>"
            "<CATENA output=\"$qq\"><<#@gs>></CATENA>"
            "<PER congruentia=\"$qq\"><v>&@k;</v></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q k=\"1\"/></p><p><r/></p><v>1</v>"
                "</radix>");
        }
    }
}

    /* --- modus= gradui: primum truncat, unum medium clamat --- */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s k=\"1\"/><s k=\"2\"/>"
            "<CATENA output=\"$m\">"
            "<EXEMPLAR modus=\"primum\"><s k=\"$k\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$m\"><n>&@k;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s k=\"1\"/><s k=\"2\"/><n>1</n></radix>");
        }
        /* unum medio catenae = adfirmatio transactionalis */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><s/>"
            "<CATENA output=\"$m\">"
            "<EXEMPLAR modus=\"unum\"><s/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_UNUM_VIOLATUM);
    }
}

    /* --- catena vacuans: gradus sine congruentiis -> relatio
     *     vacua propagatur, PER nihil parit --- */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><k/>"
            "<CATENA output=\"$m\">"
            "<EXEMPLAR><nusquam/></EXEMPLAR>"
            "<EXEMPLAR><q/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/></radix>");
        }
    }
}

    /* --- vitia XX (CATENA malformata) + XII/XIV/XVIII --- */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* XII: involucrum sine output= */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><CATENA><EXEMPLAR><s/></EXEMPLAR></CATENA>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_SINE_EXITU);

        /* XX: catena vacua */
        e = _expandere_litteras(piscina, intern,
            "<radix><CATENA output=\"$m\"></CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: liber non-nexus (elementum alienum) */
        e = _expandere_litteras(piscina, intern,
            "<radix><CATENA output=\"$m\"><alienum/></CATENA>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: textus in catena */
        e = _expandere_litteras(piscina, intern,
            "<radix><CATENA output=\"$m\">textus</CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: nexus cum output= (uncus RESERVATUS) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><CATENA output=\"$m\">"
            "<EXEMPLAR output=\"$x\"><s/></EXEMPLAR></CATENA>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: nexus cum de= (filum frangeret) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><EXEMPLAR output=\"$a\"><s/></EXEMPLAR>"
            "<CATENA output=\"$m\">"
            "<EXEMPLAR de=\"$a\"><s/></EXEMPLAR></CATENA>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: CATENA nidificata cum attributis (nuda solum) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><CATENA output=\"$m\">"
            "<CATENA output=\"$x\"><EXEMPLAR><s/></EXEMPLAR>"
            "</CATENA></CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: modus= in involucro (gradibus pertinet) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><CATENA modus=\"unum\" output=\"$m\">"
            "<EXEMPLAR><s/></EXEMPLAR></CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XX: adsignatio gemina (nomen iam ligatum) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><EXEMPLAR output=\"$m\"><s/></EXEMPLAR>"
            "<CATENA output=\"$m\"><EXEMPLAR><s/></EXEMPLAR>"
            "</CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CATENA_MALFORMATA);

        /* XIV: de= involucri relationem ignotam nominans */
        e = _expandere_litteras(piscina, intern,
            "<radix><CATENA de=\"$nusquam\" output=\"$m\">"
            "<EXEMPLAR><s/></EXEMPLAR></CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SCOPUS_IGNOTUS);

        /* XVIII: exitus catenae non consumptus (custos caudae) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><CATENA output=\"$m\">"
            "<EXEMPLAR><s/></EXEMPLAR></CATENA></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_EXITUS_NON_CONSUMPTUS);
        CREDO_NON_NIHIL (e.radix_expansa);
    }
}

    /* --- DIRIBITIO: custos verus -> bracchium applicatum --- */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><k/><s v=\"1\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><k/></EXEMPLAR></EST>"
            "<EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/><s v=\"1\"/><n>1</n></radix>");
        }
    }
}

    /* --- DIRIBITIO: custos falsus -> relatio vacua effluens --- */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s v=\"1\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><k/></EXEMPLAR></EST>"
            "<EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s v=\"1\"/></radix>");
        }
    }
}

    /* --- DIRIBITIO: primum-congruens-vincit --- */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><k/><s v=\"1\"/><t v=\"2\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><k/></EXEMPLAR></EST>"
            "<EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CASUS>"
            "<CASUS><EST><EXEMPLAR><k/></EXEMPLAR></EST>"
            "<EXEMPLAR><t v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/><s v=\"1\"/><t v=\"2\"/><n>1</n>"
                "</radix>");
        }
    }
}

    /* --- DIRIBITIO: ORDINARIUS = bracchium-semper --- */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s v=\"1\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><nusquam/></EXEMPLAR></EST>"
            "<EXEMPLAR><t/></EXEMPLAR></CASUS>"
            "<ORDINARIUS><EXEMPLAR><s v=\"$v\"/></EXEMPLAR>"
            "</ORDINARIUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s v=\"1\"/><n>1</n></radix>");
        }
    }
}

    /* --- DIRIBITIO: bracchium relationem ORIGINALEM accipit
     *     (custos, non angustator - decretum) --- */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p><q/></p><p><r v=\"2\"/></p>"
            "<EXEMPLAR output=\"$pp\"><p/></EXEMPLAR>"
            "<DIRIBITIO de=\"$pp\" output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><q/></EXEMPLAR></EST>"
            "<EXEMPLAR><r v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            /* conditio in ordine PRIMO vera; bracchium r in ordine
             * ALTERO invenit - relationem totam vidit */
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><q/></p><p><r v=\"2\"/></p><n>2</n>"
                "</radix>");
        }
    }
}

    /* --- DIRIBITIO: catena ut conditio et ut bracchium --- */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><f><g/></f><s v=\"1\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><CATENA><EXEMPLAR><f/></EXEMPLAR>"
            "<EXEMPLAR><g/></EXEMPLAR></CATENA></EST>"
            "<CATENA><EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CATENA>"
            "</CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><f><g/></f><s v=\"1\"/><n>1</n></radix>");
        }
    }
}

    /* --- DIRIBITIO ut nexus catenae + diribitio in est --- */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><k/><p><s v=\"1\"/></p>"
            "<CATENA output=\"$m\">"
            "<EXEMPLAR><p/></EXEMPLAR>"
            "<DIRIBITIO>"
            "<CASUS><EST><DIRIBITIO>"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></EST>"
            "<EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "</CATENA>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/><p><s v=\"1\"/></p><n>1</n></radix>");
        }
    }
}

    /* --- DIRIBITIO ex templo (nexus) + templum in est --- */

interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@cust><EXEMPLAR><k/></EXEMPLAR></#>"
            "<#@disp><DIRIBITIO>"
            "<CASUS><EST><<#@cust>></EST>"
            "<EXEMPLAR><s v=\"$v\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></#>"
            "<k/><s v=\"1\"/>"
            "<CATENA output=\"$m\"><<#@disp>></CATENA>"
            "<PER congruentia=\"$m\"><n>&@v;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><k/><s v=\"1\"/><n>1</n></radix>");
        }
    }
}

    /* --- DIRIBITIO: conditio ancorata + modus in bracchio --- */

interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix r=\"1\"><s k=\"1\"/><s k=\"2\"/>"
            "<DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR ancorata><radix r=\"1\"/>"
            "</EXEMPLAR></EST>"
            "<EXEMPLAR modus=\"primum\"><s k=\"$k\"/></EXEMPLAR>"
            "</CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@k;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix r=\"1\"><s k=\"1\"/><s k=\"2\"/><n>1</n>"
                "</radix>");
        }
    }
}

    /* --- vitia XXI + XVIII --- */

interior vacuum
_sectio_tricesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* XXI: liber non-bracchium */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\"><alienum/>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: CASUS sine <EST> */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<CASUS><EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: corpus bracchii mandata duo */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR><EXEMPLAR><s/></EXEMPLAR>"
            "</CASUS></DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: radix conditionis cum modus= (probatio nuda) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR modus=\"unum\"><s/></EXEMPLAR>"
            "</EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: radix conditionis cum output= */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR output=\"$x\"><s/></EXEMPLAR>"
            "</EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: ORDINARIUS non ultimus */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<ORDINARIUS><EXEMPLAR><s/></EXEMPLAR></ORDINARIUS>"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: ORDINARIUS cum <EST> */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<ORDINARIUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></ORDINARIUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: diribitio sine bracchiis */
        e = _expandere_litteras(piscina, intern,
            "<radix><DIRIBITIO output=\"$m\"></DIRIBITIO>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XXI: modus= in involucro */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO modus=\"unum\" output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_DIRIBITIO_MALFORMATA);

        /* XII: involucrum sine output= */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO>"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_EXEMPLAR_SINE_EXITU);

        /* XVIII: exitus non consumptus (custos caudae) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s/><DIRIBITIO output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><s/></EXEMPLAR></EST>"
            "<EXEMPLAR><s/></EXEMPLAR></CASUS>"
            "</DIRIBITIO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
                         == STML_EXPANSIO_EXITUS_NON_CONSUMPTUS);
        CREDO_NON_NIHIL (e.radix_expansa);
    }
}

    /* --- LEX EXTENSIONIS: capturae per catenam accumulantur,
     *     tabula PER omnia accumulata fert (decretum) --- */

interior vacuum
_sectio_quadragesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p t=\"X\"><q k=\"1\"/></p>"
            "<EXEMPLAR output=\"$pp\"><p t=\"$t\"/></EXEMPLAR>"
            "<EXEMPLAR de=\"$pp\" output=\"$qq\"><q k=\"$k\"/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$qq\"><n>&@t;-&@k;</n></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"X\"><q k=\"1\"/></p><n>X-1</n>"
                "</radix>");
        }
    }
}

    /* --- extensio per gradus CATENAE --- */

interior vacuum
_sectio_quadragesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p t=\"X\"><q k=\"1\"/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p t=\"$t\"/></EXEMPLAR>"
            "<EXEMPLAR><q k=\"$k\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><n>&@t;-&@k;</n></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"X\"><q k=\"1\"/></p><n>X-1</n>"
                "</radix>");
        }
    }
}

    /* --- extensio per bracchium DIRIBITIONIS --- */

interior vacuum
_sectio_quadragesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p t=\"X\"><q k=\"1\"/></p>"
            "<EXEMPLAR output=\"$pp\"><p t=\"$t\"/></EXEMPLAR>"
            "<DIRIBITIO de=\"$pp\" output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><q/></EXEMPLAR></EST>"
            "<EXEMPLAR><q k=\"$k\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><n>&@t;-&@k;</n></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"X\"><q k=\"1\"/></p><n>X-1</n>"
                "</radix>");
        }
    }
}

    /* --- XXII: collisio capturarum trans gradus --- */

interior vacuum
_sectio_quadragesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p n=\"A\"><q n=\"B\"/></p>"
            "<EXEMPLAR output=\"$pp\"><p n=\"$n\"/></EXEMPLAR>"
            "<EXEMPLAR de=\"$pp\" output=\"$qq\"><q n=\"$n\"/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$qq\"><x>&@n;</x></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CAPTURA_COLLISA);

        /* eadem per nexus catenae */
        e = _expandere_litteras(piscina, intern,
            "<radix><p n=\"A\"><q n=\"B\"/></p>"
            "<CATENA output=\"$qq\">"
            "<EXEMPLAR><p n=\"$n\"/></EXEMPLAR>"
            "<EXEMPLAR><q n=\"$n\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$qq\"><x>&@n;</x></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_CAPTURA_COLLISA);
    }
}


    /* =============== SINE (antiiunctio, XXIII) =============== */

    /* --- SINE: filtrum fundamentale (servare in ZEPHYRO) --- */

interior vacuum
_sectio_quadragesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p t=\"A\"><q/></p><p t=\"B\"/>"
            "<CATENA output=\"$pp\">"
            "<EXEMPLAR><p t=\"$t\"/></EXEMPLAR>"
            "<SINE><q/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$pp\"><n>&@t;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"A\"><q/></p><p t=\"B\"/><n>B</n>"
                "</radix>");
        }
    }
}

    /* --- SINE: PONS IPSE IUNCTIO EST (correlatio per ordinem) ---
     * probatio incorrelata ambos necaret (u adest utrobique);
     * pons '&@n;' solum ordinem primum necat */

interior vacuum
_sectio_quadragesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/><u n=\"x\"/></s>"
            "<s><d n=\"y\"/><u n=\"x\"/></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s><d n=\"$n\"/></s></EXEMPLAR>"
            "<SINE><u n=\"&@n;\"/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><mortuum>&@n;</mortuum></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/><u n=\"x\"/></s>"
                "<s><d n=\"y\"/><u n=\"x\"/></s>"
                "<mortuum>y</mortuum></radix>");
        }
    }
}

    /* --- SINE medio catenae: filtrum purum (radix eadem,
     * capturae hereditatae per SINE fluunt) --- */

interior vacuum
_sectio_quadragesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/><u n=\"x\"/></s>"
            "<s><d n=\"y\"/><m v=\"1\"/></s>"
            "<CATENA output=\"$rr\">"
            "<EXEMPLAR><s><d n=\"$n\"/></s></EXEMPLAR>"
            "<SINE><u n=\"&@n;\"/></SINE>"
            "<EXEMPLAR><m v=\"$v\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$rr\"><x>&@n;-&@v;</x></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/><u n=\"x\"/></s>"
                "<s><d n=\"y\"/><m v=\"1\"/></s>"
                "<x>y-1</x></radix>");
        }
    }
}

    /* --- SINE: vacuitas propagata (relatio vacua influens) --- */

interior vacuum
_sectio_quadragesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<CATENA output=\"$rr\">"
            "<EXEMPLAR><nusquam/></EXEMPLAR>"
            "<SINE><q/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><a/></radix>");
        }
    }
}

    /* --- SINE: '<(>' ornamentum (dissolutio fragmentorum) --- */

interior vacuum
_sectio_quadragesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><p t=\"A\"><q/></p><p t=\"B\"/>"
            "<CATENA output=\"$pp\">"
            "<(><EXEMPLAR><p t=\"$t\"/></EXEMPLAR>"
            "<(><SINE><q/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$pp\"><n>&@t;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"A\"><q/></p><p t=\"B\"/><n>B</n>"
                "</radix>");
        }
    }
}

    /* --- SINE ex templo portatum (vocatio -> nexus filtrans) --- */

interior vacuum
_sectio_quadragesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@gf><SINE><q/></SINE></#>"
            "<p t=\"A\"><q/></p><p t=\"B\"/>"
            "<CATENA output=\"$pp\">"
            "<EXEMPLAR><p t=\"$t\"/></EXEMPLAR>"
            "<<#@gf>>"
            "</CATENA>"
            "<PER congruentia=\"$pp\"><n>&@t;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p t=\"A\"><q/></p><p t=\"B\"/><n>B</n>"
                "</radix>");
        }
    }
}

    /* --- dualitas: catena SINE ferens ut conditio DIRIBITIONIS
     * ('si quid DEEST, bracchium hoc sume') --- */

interior vacuum
_sectio_quinquagesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* usus deest -> conditio vera -> bracchium sumptum */
        e = _expandere_litteras(piscina, intern,
            "<radix><s t=\"S1\"><d n=\"x\"/></s>"
            "<EXEMPLAR output=\"$ss\"><s t=\"$t\"/></EXEMPLAR>"
            "<DIRIBITIO de=\"$ss\" output=\"$m\">"
            "<CASUS><EST><CATENA>"
            "<EXEMPLAR><s><d n=\"$n\"/></s></EXEMPLAR>"
            "<SINE><u n=\"&@n;\"/></SINE>"
            "</CATENA></EST>"
            "<EXEMPLAR><d n=\"$k\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><clamor>&@k;</clamor></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s t=\"S1\"><d n=\"x\"/></s>"
                "<clamor>x</clamor></radix>");
        }

        /* usus adest -> conditio falsa -> relatio vacua, nihil */
        e = _expandere_litteras(piscina, intern,
            "<radix><s t=\"S1\"><d n=\"x\"/><u n=\"x\"/></s>"
            "<EXEMPLAR output=\"$ss\"><s t=\"$t\"/></EXEMPLAR>"
            "<DIRIBITIO de=\"$ss\" output=\"$m\">"
            "<CASUS><EST><CATENA>"
            "<EXEMPLAR><s><d n=\"$n\"/></s></EXEMPLAR>"
            "<SINE><u n=\"&@n;\"/></SINE>"
            "</CATENA></EST>"
            "<EXEMPLAR><d n=\"$k\"/></EXEMPLAR></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><clamor>&@k;</clamor></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s t=\"S1\"><d n=\"x\"/><u n=\"x\"/></s>"
                "</radix>");
        }
    }
}

    /* --- XXIII: attributa machinae in SINE --- */

interior vacuum
_sectio_quinquagesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE de=\"$pp\"><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE output=\"$oo\"><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE modus=\"unum\"><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE ancorata=\"vera\"><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);
    }
}

    /* --- XXIII: corpus malformatum --- */

interior vacuum
_sectio_quinquagesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* corpus vacuum */
        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE/></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);

        /* elementa duo */
        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE><q/><r/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);

        /* textus */
        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE>textus</SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);
    }
}

    /* --- XXIII: fons NIHIL (absentia lata documenti = ianua
     * posterior, CLARE recusata) --- */

interior vacuum
_sectio_quinquagesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<SINE><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);
    }
}

    /* --- caecitas attributorum (TRANSPARENTIA attributa=): forma
     * splicata provenientiam fert (b=) quae littera fieret -
     * caecitate declarata usus invenitur, ordo necatur --- */

interior vacuum
_sectio_quinquagesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* sine caecitate: b="7" in forma = littera, usus b="9"
         * numquam congruit -> ordo servatur (situs falsus) */
        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\" b=\"7\"/><u n=\"x\" b=\"9\"/></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s><d $c/></s></EXEMPLAR>"
            "<SINE><u n=\"x\" b=\"7\"/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><x/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\" b=\"7\"/>"
                "<u n=\"x\" b=\"9\"/></s><x/></radix>");
        }

        /* cum caecitate: b= perspicuum -> usus congruit -> ordo
         * necatur, nihil emittitur */
        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<TRANSPARENTIA tags=\"nihil-tale\" attributa=\"b\"/>"
            "<s><d n=\"x\" b=\"7\"/><u n=\"x\" b=\"9\"/></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s><d $c/></s></EXEMPLAR>"
            "<SINE><u n=\"x\" b=\"7\"/></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><x/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\" b=\"7\"/>"
                "<u n=\"x\" b=\"9\"/></s></radix>");
        }

        /* attributa= vacuum = XIX */
        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<TRANSPARENTIA tags=\"t\" attributa=\" \"/>"
            "<a/></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_TRANSPARENTIA_MALFORMATA);
    }
}

    /* --- aliasa contenti resoluta (congruentia per
     * repraesentationem videt): usus solum per '<<#id>>' testatum
     * ordinem necat --- */

interior vacuum
_sectio_quinquagesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/>"
            "<#tok><signum n=\"x\"/></#>"
            "<u><<#tok>></u></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s><d n=\"$n\"/></s></EXEMPLAR>"
            "<SINE><u><signum n=\"&@n;\"/></u></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><mortuum>&@n;</mortuum></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/>"
                "<#tok><signum n=\"x\"/></#>"
                "<u><<#tok>></u></s></radix>");
        }
    }
}

    /* --- SINE ut mandatum bracchii directum = XXI (lex vetus:
     * mandata pura EXEMPLAR/CATENA/DIRIBITIO sola; catena SINE
     * ferens via composita est) --- */

interior vacuum
_sectio_quinquagesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<EXEMPLAR output=\"$ss\"><a/></EXEMPLAR>"
            "<DIRIBITIO de=\"$ss\" output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><a/></EXEMPLAR></EST>"
            "<SINE><q/></SINE></CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_DIRIBITIO_MALFORMATA);
    }
}

    /* ===== UNIFORMITAS PONTIS (referentia = electio) ===== */
    /* --- gradus positivus correlatus: iunctio trans gradus ---
     * pons '&@n;' in gradu positivo = impletio per ordinem
     * (machina SINE signo positivo) --- */

interior vacuum
_sectio_quinquagesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/><probatio de-quo=\"x\"/></s>"
            "<s><d n=\"y\"/><probatio de-quo=\"z\"/></s>"
            "<CATENA output=\"$rr\">"
            "<EXEMPLAR><s/></EXEMPLAR>"
            "<EXEMPLAR radix=\"fontis\"><d n=\"$n\"/></EXEMPLAR>"
            "<EXEMPLAR><probatio de-quo=\"&@n;\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$rr\"><iuncta>&@n;</iuncta></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/><probatio de-quo=\"x\"/>"
                "</s>"
                "<s><d n=\"y\"/><probatio de-quo=\"z\"/></s>"
                "<iuncta>x</iuncta></radix>");
        }
    }
}

    /* --- XIII: referentia sine ordinibus (scopus documenti) --- */

interior vacuum
_sectio_quinquagesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<EXEMPLAR output=\"$r\"><u n=\"&@n;\"/></EXEMPLAR>"
            "<PER congruentia=\"$r\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
    }
}

    /* ========= ANGUSTANS (custos fit angustator) ========= */
    /* --- vigilia UNA VICE scripta: conditio filtrat, bracchium
     * angustans identitatem accipit (EXEMPLAR ancorata cum
     * stella = mandatum identitatis) --- */

interior vacuum
_sectio_quinquagesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<punctum gradus=\"theorema\" index=\"4.1\">"
            "<probatio/></punctum>"
            "<punctum gradus=\"theorema\" index=\"4.3\"/>"
            "<EXEMPLAR output=\"$th\">"
            "<punctum gradus=\"theorema\" index=\"$i\"/>"
            "</EXEMPLAR>"
            "<DIRIBITIO de=\"$th\" output=\"$v\">"
            "<CASUS angustans>"
            "<EST><CATENA><SINE><probatio/></SINE></CATENA></EST>"
            "<EXEMPLAR ancorata><*/></EXEMPLAR>"
            "</CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$v\"><clamor>&@i;</clamor></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<punctum gradus=\"theorema\" index=\"4.1\">"
                "<probatio/></punctum>"
                "<punctum gradus=\"theorema\" index=\"4.3\"/>"
                "<clamor>4.3</clamor></radix>");
        }
    }
}

    /* --- vestigium angustationis (INDAGO): 'bracchium
     * angustatum' + relatio conditionis in numeris visa --- */

interior vacuum
_sectio_sexagesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><INDAGO/>"
            "<punctum gradus=\"theorema\" index=\"4.1\">"
            "<probatio/></punctum>"
            "<punctum gradus=\"theorema\" index=\"4.3\"/>"
            "<EXEMPLAR output=\"$th\">"
            "<punctum gradus=\"theorema\" index=\"$i\"/>"
            "</EXEMPLAR>"
            "<DIRIBITIO de=\"$th\" output=\"$v\">"
            "<CASUS angustans>"
            "<EST><CATENA><SINE><probatio/></SINE></CATENA></EST>"
            "<EXEMPLAR ancorata><*/></EXEMPLAR>"
            "</CASUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$v\"><clamor>&@i;</clamor></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<punctum gradus=\"theorema\" index=\"4.1\">"
                "<probatio/></punctum>"
                "<punctum gradus=\"theorema\" index=\"4.3\"/>"
                "<!--indago EXEMPLAR $th (radix): ordines 2"
                " (capturae: i)-->"
                "<!--indago DIRIBITIO $v (de $th 2):"
                " CASUS 1 conditio 1 SUMPTUM;"
                " bracchium angustatum; EXEMPLAR * 1->1;"
                " exitus 1-->"
                "<clamor>4.3</clamor>"
                "<!--indago PER $v: ordines 1--></radix>");
        }
    }
}

    /* --- XXI: ORDINARIUS angustans (nihil angustandum) --- */

interior vacuum
_sectio_sexagesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<EXEMPLAR output=\"$ss\"><a/></EXEMPLAR>"
            "<DIRIBITIO de=\"$ss\" output=\"$m\">"
            "<ORDINARIUS angustans><EXEMPLAR><a/></EXEMPLAR>"
            "</ORDINARIUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_DIRIBITIO_MALFORMATA);
    }
}

    /* ========= RETENTIO RADICIS 'radix="fontis"' ========= */
    /* --- pluralitas in scopo uno: ordo per declarationem, radix
     * scopi retenta - antiiunctio per-declarationem (forma codicis
     * 69 in parvo), deinde gradus ulterior ex radice retenta --- */

interior vacuum
_sectio_sexagesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/><d n=\"y\"/><u n=\"x\"/>"
            "<m v=\"1\"/></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s/></EXEMPLAR>"
            "<EXEMPLAR radix=\"fontis\"><d n=\"$n\"/></EXEMPLAR>"
            "<SINE><u n=\"&@n;\"/></SINE>"
            "<EXEMPLAR><m v=\"$v\"/></EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><x>&@n;-&@v;</x></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/><d n=\"y\"/><u n=\"x\"/>"
                "<m v=\"1\"/></s>"
                "<x>y-1</x></radix>");
        }
    }
}

    /* --- radix congruentiae ipsius per capturam claram --- */

interior vacuum
_sectio_sexagesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d n=\"x\"/></s>"
            "<CATENA output=\"$mm\">"
            "<EXEMPLAR><s/></EXEMPLAR>"
            "<EXEMPLAR radix=\"fontis\"><d $d n=\"$n\"/>"
            "</EXEMPLAR>"
            "</CATENA>"
            "<PER congruentia=\"$mm\"><cella>&@d;</cella></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d n=\"x\"/></s>"
                "<cella><d n=\"x\"/></cella></radix>");
        }
    }
}

    /* --- XIII: valor ignotus; fontis sine ordine intrante --- */

interior vacuum
_sectio_sexagesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$r\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<EXEMPLAR radix=\"alienum\"><a/></EXEMPLAR></CATENA>"
            "<PER congruentia=\"$r\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<EXEMPLAR output=\"$r\" radix=\"fontis\"><a/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$r\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
    }
}


    /* ============ DESCENSUS '<**>' (axis, XXV) ============ */

    /* --- pertinentia: captura per profunditatem ignotam --- */

interior vacuum
_sectio_sexagesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><x><m><n><b v=\"1\"/></n></m></x>"
            "<EXEMPLAR output=\"$r\"><x><**><b v=\"$v\"/></**>"
            "</x></EXEMPLAR>"
            "<PER congruentia=\"$r\"><f>&@v;</f></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><m><n><b v=\"1\"/></n></m></x>"
                "<f>1</f></radix>");
        }
    }
}

    /* --- sublatio: existentiale extra ordinem fratrum --- */

interior vacuum
_sectio_sexagesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><x><alte><b/></alte><a/></x>"
            "<EXEMPLAR output=\"$r\"><x><a/><**><b/></**></x>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$r\"><est/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><alte><b/></alte><a/></x><est/>"
                "</radix>");
        }
    }
}

    /* --- ligatio prima COMPATIBILIS: regula V per descensum
     * (iunctio sui profunda - probatio se citans) --- */

interior vacuum
_sectio_sexagesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<punctum index=\"4\"><probatio ref=\"9\"/>"
            "<s><probatio ref=\"4\"/></s></punctum>"
            "<punctum index=\"7\"><probatio ref=\"9\"/></punctum>"
            "<EXEMPLAR output=\"$r\"><punctum index=\"$i\">"
            "<**><probatio ref=\"$i\"/></**></punctum></EXEMPLAR>"
            "<PER congruentia=\"$r\"><se>&@i;</se></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<punctum index=\"4\"><probatio ref=\"9\"/>"
                "<s><probatio ref=\"4\"/></s></punctum>"
                "<punctum index=\"7\"><probatio ref=\"9\"/>"
                "</punctum>"
                "<se>4</se></radix>");
        }
    }
}

    /* --- caecitas: descensus perspicua numquam intrat --- */

interior vacuum
_sectio_sexagesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<TRANSPARENTIA tags=\"celatum\"/>"
            "<x><celatum><b/></celatum></x>"
            "<EXEMPLAR output=\"$r\"><x><**><b/></**></x>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$r\"><est/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><celatum><b/></celatum></x></radix>");
        }
    }
}

    /* --- descensus in corpore SINE (absentia profunda) --- */

interior vacuum
_sectio_sexagesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s t=\"A\"><w><x><malum/></x></w></s>"
            "<s t=\"B\"><w><x/></w></s>"
            "<CATENA output=\"$rr\">"
            "<EXEMPLAR><s t=\"$t\"/></EXEMPLAR>"
            "<SINE><w><**><malum/></**></w></SINE>"
            "</CATENA>"
            "<PER congruentia=\"$rr\"><n>&@t;</n></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s t=\"A\"><w><x><malum/></x></w></s>"
                "<s t=\"B\"><w><x/></w></s>"
                "<n>B</n></radix>");
        }
    }
}

    /* --- compositio positionalis: '<**>' intra '<*>' in sede
     * cursoris (descensus positionalis numquam primitivum) --- */

interior vacuum
_sectio_septuagesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* p sub liberO POST a: congruit */
        e = _expandere_litteras(piscina, intern,
            "<radix><x><a/><w><d><p/></d></w><b/></x>"
            "<EXEMPLAR output=\"$r\"><x><a/><*><**><p/></**>"
            "</*><b/></x></EXEMPLAR>"
            "<PER congruentia=\"$r\"><est/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><a/><w><d><p/></d></w><b/></x><est/>"
                "</radix>");
        }

        /* p solum sub libero ANTE a: cursor non attingit */
        e = _expandere_litteras(piscina, intern,
            "<radix><x><w><d><p/></d></w><a/><b/></x>"
            "<EXEMPLAR output=\"$r\"><x><a/><*><**><p/></**>"
            "</*><b/></x></EXEMPLAR>"
            "<PER congruentia=\"$r\"><est/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><w><d><p/></d></w><a/><b/></x>"
                "</radix>");
        }
    }
}

    /* --- XXV: forma descensus mala (praeparatione, CLARE) --- */

interior vacuum
_sectio_septuagesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><EXEMPLAR output=\"$r\"><x><**/></x>"
            "</EXEMPLAR><PER congruentia=\"$r\"><y/></PER>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_DESCENSUS_MALFORMATUS);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><EXEMPLAR output=\"$r\"><x><**><a/><b/>"
            "</**></x></EXEMPLAR><PER congruentia=\"$r\"><y/></PER>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_DESCENSUS_MALFORMATUS);

        e = _expandere_litteras(piscina, intern,
            "<radix><a/><EXEMPLAR output=\"$r\"><x>"
            "<** malum=\"1\"><a/></**></x></EXEMPLAR>"
            "<PER congruentia=\"$r\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_DESCENSUS_MALFORMATUS);

        /* etiam in corpore SINE */
        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$r\">"
            "<EXEMPLAR><a/></EXEMPLAR>"
            "<SINE><x><**/></x></SINE></CATENA>"
            "<PER congruentia=\"$r\"><y/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium
            == STML_EXPANSIO_DESCENSUS_MALFORMATUS);
    }
}


    /* =============== INDAGO (vestigatio, XXIV) =============== */

    /* --- lapis sepulcralis catenae + PER intra relatum --- */

interior vacuum
_sectio_septuagesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><s><d/></s><s/>"
            "<INDAGO/>"
            "<CATENA output=\"$rr\"><EXEMPLAR><s $c/></EXEMPLAR>"
            "<SINE><d/></SINE></CATENA>"
            "<relatum><PER congruentia=\"$rr\"><x/></PER>"
            "</relatum></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><s><d/></s><s/>"
                "<!--indago CATENA $rr (radix);"
                " 1 EXEMPLAR s radix->2 (capturae: c);"
                " 2 SINE d 2->1 (necati 1); exitus 1-->"
                "<relatum><x/>"
                "<!--indago PER $rr: ordines 1--></relatum>"
                "</radix>");
        }
    }
}

    /* --- lapis DIRIBITIONIS: praeteritum + ORDINARIUS + fabula
     * bracchii --- */

interior vacuum
_sectio_septuagesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><a/>"
            "<INDAGO/>"
            "<EXEMPLAR output=\"$ss\"><a $x/></EXEMPLAR>"
            "<DIRIBITIO de=\"$ss\" output=\"$m\">"
            "<CASUS><EST><EXEMPLAR><nusquam/></EXEMPLAR></EST>"
            "<EXEMPLAR><b/></EXEMPLAR></CASUS>"
            "<ORDINARIUS><EXEMPLAR><a/></EXEMPLAR></ORDINARIUS>"
            "</DIRIBITIO>"
            "<PER congruentia=\"$m\"><y/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><a/>"
                "<!--indago EXEMPLAR $ss (radix): ordines 1"
                " (capturae: x)-->"
                "<!--indago DIRIBITIO $m (de $ss 1):"
                " CASUS 1 conditio 0 praeteritum;"
                " ORDINARIUS SUMPTUM; bracchium;"
                " EXEMPLAR a 1->1; exitus 1-->"
                "<y/>"
                "<!--indago PER $m: ordines 1--></radix>");
        }
    }
}

    /* --- XXIV: INDAGO non nudum --- */

interior vacuum
_sectio_septuagesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><INDAGO malum=\"1\"/><a/></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_INDAGO_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><INDAGO><a/></INDAGO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_INDAGO_MALFORMATUM);
    }
}


    /* ==================================================
     * B1 (md-arbor-spec par. 6.1, ratificatio 2026-09-03) -
     * PROIECTIONES LOCULORUM: '&@n.slot;' = CONTENTUM involucri
     * liberi (textus = scalaris, aliter silva; numquam involucrum
     * ipsum), '&@n.slot!;' = octeti textus et foliorum crudorum,
     * vitia XXVI-XXVIII. Forma fixturarum: contentum, EXEMPLAR
     * modus=unum, PER cum vocatione templi cui radix capta '&@c;'
     * ut argumentum subarboreum 'n' transit (fixtura 6.1b) -
     * corpus templi proiectiones legit.
     * ================================================== */

    /* --- proiectio scalaris (attributum + textus), via catenata --- */

interior vacuum
_sectio_septuagesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\"><h g=\"&@n.gradus;\">&@n.gradus;/&@n.inlinea.t;"
            "</h></#>"
            "<capitulum><gradus>2</gradus><inlinea><t>x</t></inlinea>"
            "</capitulum>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><capitulum $c/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<capitulum><gradus>2</gradus><inlinea><t>x</t></inlinea>"
                "</capitulum>"
                "<h g=\"2\">2/x</h></radix>");
        }
    }
}

    /* --- proiectio silvae (splex), '!' octeti crudorum, vacuum --- */

interior vacuum
_sectio_septuagesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\"><p>&@n.inlinea;</p><q>&@n.crudum;</q>"
            "<r>&@n.crudum!;</r><s>[&@n.vacuum;]</s></#>"
            "<textus><inlinea><t>x</t><v/></inlinea>"
            "<crudum><tok!>Hello </tok><tok!>w</tok></crudum>"
            "<vacuum/></textus>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><textus $c/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<textus><inlinea><t>x</t><v/></inlinea>"
                "<crudum><tok!>Hello </tok><tok!>w</tok></crudum>"
                "<vacuum/></textus>"
                "<p><t>x</t><v/></p>"
                "<q><tok!>Hello </tok><tok!>w</tok></q>"
                "<r>Hello w</r><s>[]</s></radix>");
        }
    }
}

    /* --- '!' super structuram = vitium VII (chordificatio tacita
     * numquam) --- */

interior vacuum
_sectio_septuagesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.inlinea!;</r></#>"
            "<textus><inlinea><t>x</t></inlinea></textus>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><textus $c/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_ARGUMENTUM_ARBOREUM);
    }
}

    /* --- XXVII ABSENS / XXVIII AMBIGUA (impletione) --- */

interior vacuum
_sectio_septuagesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.nusquam;</r></#>"
            "<x><a/></x>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><x $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_PROIECTIO_ABSENS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "nusquam");

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.t;</r></#>"
            "<x><t>1</t><t>2</t></x>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><x $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_PROIECTIO_AMBIGUA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "t");

        /* gradus litterales: 'b' liberum de 'a' (saltus nullus) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.a.b;</r></#>"
            "<x><a><b>1</b><b>2</b></a></x>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><x $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_PROIECTIO_AMBIGUA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "b");

        /* radices plures argumenti subarborei: segmentum primum */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.q;</r></#>"
            "<<#@f>><@n=><x/><y/></></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_PROIECTIO_AMBIGUA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "q");

        /* argumentum scalare cum via: involucra nulla */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.x;</r></#><<#@f n=\"5\">>"
            "</radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_PROIECTIO_ABSENS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "x");
    }
}

    /* --- XXVI MALFORMATA (collectione, sine vocatione); forma
     * imperfecta '&@n!x;' littera manet --- */

interior vacuum
_sectio_septuagesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;
                StmlResultus ref;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n..x;</r></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_PROIECTIO_MALFORMATA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "n..x");
        CREDO_CHORDA_AEQUALIS_LITERIS (e.fragmentum, "@f");

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@.x;</r></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_PROIECTIO_MALFORMATA);

        /* declaratio cum puncto: loculus 'a.b' olim tacite factus */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@a.b\"><r/></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_PROIECTIO_MALFORMATA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "a.b");

        /* '!' non ultimum: nulla referentia - littera (regula entis
         * ignoti), nec collectio nec impletio eam tangit */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n!x;</r></#><<#@f n=\"1\">>"
            "</radix>");
        CREDO_VERUM (e.successus);
        ref =
            stml_legere(chorda_ex_literis("<radix><r>&@n!x;</r></radix>",
                                            piscina), piscina, intern);
        CREDO_VERUM (ref.successus);
        si (e.successus && ref.successus)
        {
            CREDO_CHORDA_AEQUALIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                stml_scribere(ref.radix, piscina, FALSUM));
        }
    }
}

    /* --- COMMUTATIO super proiectionem; involucrum vacuum = scalaris
     * vacuus PRAESENS (non-nihil congruit). 'est=""' scribi nequit:
     * grammatica basis valorem vacuum ut sepulcrum legit (attributum
     * nudum, valor NIHIL) - absentia in materia involucrum DEEST
     * (XXVII), bracchium exemplaris eam discernit (B1.4) --- */

interior vacuum
_sectio_octogesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\">"
            "<COMMUTATIO de=\"&@n.gradus;\"><CASUS est=\"2\"><h2/></CASUS>"
            "<ORDINARIUS><h/></ORDINARIUS></COMMUTATIO>"
            "<COMMUTATIO de=\"&@n.descriptio;\"><CASUS non-nihil>"
            "<cum>[&@n.descriptio;]</cum></CASUS><ORDINARIUS><sine/>"
            "</ORDINARIUS></COMMUTATIO>"
            "</#>"
            "<capitulum><gradus>2</gradus><descriptio/></capitulum>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><capitulum $c/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><capitulum><gradus>2</gradus><descriptio/>"
                "</capitulum><h2/><cum>[]</cum></radix>");
        }
    }
}

    /* --- proiectio ut argumentum bloci vocationis interioris
     * ('<@m=>&@n.inlinea;</>' silvam involucri fert) --- */

interior vacuum
_sectio_octogesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@g m=\"@m\"><w>&@m;</w></#>"
            "<#@f n=\"@n\"><<#@g>><@m=>&@n.inlinea;</></#>"
            "<textus><inlinea><t>x</t></inlinea></textus>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><textus $c/>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><textus><inlinea><t>x</t></inlinea></textus>"
                "<w><t>x</t></w></radix>");
        }
    }
}


    /* ==================================================
     * B1.3 (md-arbor-spec par. 6.3) - PER SUPER SILVAM ARGUMENTI in
     * corpore templi: '<PER de="@n.elementa" voca="#@item"/>' (ordo =
     * elementum -> loculus requisitus unicus templi delegati) et
     * '<PER de="@n.elementa" ut="e">…&@e.t;…</PER>' (corpus, scopus
     * exterior visibilis). Vitium XXIX; malformationes XIII.
     * ================================================== */

    /* --- delegatio: ansa liberorum in una linea --- */

interior vacuum
_sectio_octogesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@item n=\"@n\"><li>&@n.t;</li></#>"
            "<#@list n=\"@n\"><ul><PER de=\"@n.elementa\" voca=\"#@item\"/>"
            "</ul></#>"
            "<lista><elementa><e><t>a</t></e><e><t>b</t></e></elementa>"
            "</lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<lista><elementa><e><t>a</t></e><e><t>b</t></e></elementa>"
                "</lista>"
                "<ul><li>a</li><li>b</li></ul></radix>");
        }
    }
}

    /* --- forma corporis ut=: scopus exterior visibilis; textus et
     * commenta silvae ordines non sunt --- */

interior vacuum
_sectio_octogesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@list n=\"@n\"><ul><PER de=\"@n.elementa\" ut=\"e\">"
            "<li>&@e.t;/&@n.titulus;</li></PER></ul></#>"
            "<lista><titulus>L</titulus><elementa>tx<e><t>a</t></e>"
            "<!--c--><e><t>b</t></e></elementa></lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<lista><titulus>L</titulus><elementa>tx<e><t>a</t></e>"
                "<!--c--><e><t>b</t></e></elementa></lista>"
                "<ul><li>a/L</li><li>b/L</li></ul></radix>");
        }
    }
}

    /* --- silva vacua = ordines nulli; PER nidificata (ansa duplex) --- */

interior vacuum
_sectio_octogesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@list n=\"@n\"><ul>[<PER de=\"@n.elementa\" ut=\"e\"><li/>"
            "</PER>]</ul></#>"
            "<lista><elementa/></lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><lista><elementa/></lista><ul>[]</ul></radix>");
        }

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@leaf n=\"@n\"><i>&@n.t;</i></#>"
            "<#@item n=\"@n\"><li><PER de=\"@n.sub\" voca=\"#@leaf\"/></li></#>"
            "<#@list n=\"@n\"><ul><PER de=\"@n.elementa\" voca=\"#@item\"/>"
            "</ul></#>"
            "<lista><elementa><e><sub><s><t>a</t></s><s><t>b</t></s></sub>"
            "</e></elementa></lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix>"
                "<lista><elementa><e><sub><s><t>a</t></s><s><t>b</t></s></sub>"
                "</e></elementa></lista>"
                "<ul><li><i>a</i><i>b</i></li></ul></radix>");
        }
    }
}

    /* --- vitia: XXIX delegatio ambigua; XIII formae malae; collectio
     * (radix ignota, ordo localis, ut obumbrans, via mala) --- */

interior vacuum
_sectio_octogesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        /* XXIX: templum cum loculis requisitis duobus */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@two a=\"@a\" b=\"@b\"><x/></#>"
            "<#@list n=\"@n\"><PER de=\"@n.elementa\" voca=\"#@two\"/></#>"
            "<lista><elementa><e/></elementa></lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_PER_DELEGATIO_AMBIGUA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.fragmentum, "@two");

        /* XIII: voca + corpus */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@item n=\"@n\"><x/></#>"
            "<#@list n=\"@n\"><PER de=\"@n.elementa\" voca=\"#@item\"><y/>"
            "</PER></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        /* XIII: ut sine corpore */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n.elementa\" ut=\"e\"/>"
            "</#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        /* XIII: '!' in de (fons silva, non octeti) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n.elementa!\" ut=\"e\"><x/>"
            "</PER></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_EXEMPLAR_MALFORMATUM);

        /* XXX: fons scalaris cum textu (impletione; speculum VII) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n.titulus\" ut=\"e\"><x/>"
            "</PER></#>"
            "<lista><titulus>L</titulus></lista>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><lista $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@list>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_ARGUMENTUM_SCALARE);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "n.titulus");

        /* collectio: radix de ignota */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@nusquam.x\" ut=\"e\"><x/>"
            "</PER></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_LOCULUS_IGNOTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "nusquam");

        /* collectio: ordo localis - '&@e;' extra corpus PER */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n.elementa\" ut=\"e\"><x/>"
            "</PER><z>&@e;</z></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_LOCULUS_IGNOTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "e");

        /* collectio: ut loculum declaratum obumbrans */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n.elementa\" ut=\"n\"><x/>"
            "</PER></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_EXEMPLAR_MALFORMATUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "n");

        /* collectio: via mala in de */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@list n=\"@n\"><PER de=\"@n..x\" ut=\"e\"><x/>"
            "</PER></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_PROIECTIO_MALFORMATA);
    }
}


    /* ==================================================
     * B1.4 (md-arbor-spec par. 6.4) - BRACCHIA EXEMPLARIA COMMUTATIONIS:
     * '<CASUS tag="x">' (aequalitas tituli radicis) et '<CASUS><EST>
     * <EXEMPLAR>forma</EXEMPLAR></EST>bracchium</CASUS>' (forma
     * argumentis impleta, ancorata ad radicem scrutantis petita,
     * capturae -> argumenta bracchii). Vitium XXX; malformationes IX.
     * ================================================== */

    /* --- dispensatio per tag: ordo ansae -> bracchium per genus --- */

interior vacuum
_sectio_octogesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"a\"><A/></CASUS>"
            "<CASUS tag=\"b\"><B>&@n.t;</B></CASUS>"
            "<ORDINARIUS><Z/></ORDINARIUS></COMMUTATIO></#>"
            "<#@g n=\"@n\"><PER de=\"@n.liberi\" voca=\"#@f\"/></#>"
            "<r><liberi><a/><b><t>x</t></b><c/></liberi></r>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><r $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@g>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><r><liberi><a/><b><t>x</t></b><c/></liberi></r>"
                "<A/><B>x</B><Z/></radix>");
        }
    }
}

    /* --- bracchia exemplaria: primum congruens vincit; capturae
     * valoris, nodi, textus ut argumenta; descensus; tag ultimus --- */

interior vacuum
_sectio_octogesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><p><nudus>1</nudus></p></EXEMPLAR></EST>"
            "<tight/></CASUS>"
            "<CASUS><EST><EXEMPLAR><p gradus=\"$g\" $q/></EXEMPLAR></EST>"
            "<h g=\"&@g;\">&@q.t;</h></CASUS>"
            "<CASUS><EST><EXEMPLAR><p><t>$x</t></p></EXEMPLAR></EST>"
            "<tx>&@x;</tx></CASUS>"
            "<CASUS><EST><EXEMPLAR><p><**><deep/></**></p></EXEMPLAR></EST>"
            "<dp/></CASUS>"
            "<CASUS tag=\"p\"><loose/></CASUS>"
            "</COMMUTATIO></#>"
            "<#@g n=\"@n\"><PER de=\"@n.liberi\" voca=\"#@f\"/></#>"
            "<r><liberi><p><nudus>1</nudus></p><p gradus=\"2\"><t>x</t></p>"
            "<p><t>y</t></p><p><w><deep/></w></p><p/></liberi></r>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><r $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@g>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><r><liberi><p><nudus>1</nudus></p>"
                "<p gradus=\"2\"><t>x</t></p><p><t>y</t></p>"
                "<p><w><deep/></w></p><p/></liberi></r>"
                "<tight/><h g=\"2\">x</h><tx>y</tx><dp/><loose/></radix>");
        }
    }
}

    /* --- exemplar parametrizatum ('&@k;' in forma); scrutans
     * optionalis absens -> ORDINARIUS --- */

interior vacuum
_sectio_octogesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\" k=\"@k\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><p id=\"&@k;\"/></EXEMPLAR></EST><hit/>"
            "</CASUS><ORDINARIUS><miss/></ORDINARIUS></COMMUTATIO></#>"
            "<#@g n=\"@n\"><PER de=\"@n.liberi\" ut=\"e\">"
            "<<#@f k=\"2\">><@n=>&@e;</></PER></#>"
            "<r><liberi><p id=\"1\"/><p id=\"2\"/></liberi></r>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><r $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@g>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><r><liberi><p id=\"1\"/><p id=\"2\"/></liberi></r>"
                "<miss/><hit/></radix>");
        }

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n?\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"p\"><hit/></CASUS>"
            "<ORDINARIUS><miss/></ORDINARIUS></COMMUTATIO></#>"
            "<<#@f>></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><miss/></radix>");
        }
    }
}

    /* --- vitia: XXX scalaris in positione arboris; IX formae malae
     * (collectione); XXII captura collisa; X nullum bracchium --- */

interior vacuum
_sectio_octogesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"p\"><hit/></CASUS></COMMUTATIO></#>"
            "<<#@f n=\"5\">></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_ARGUMENTUM_SCALARE);

        /* IX: EST + est */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS est=\"1\"><EST><EXEMPLAR><p/></EXEMPLAR></EST><x/></CASUS>"
            "</COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_COMMUTATIO_MALFORMATA);

        /* IX: EST cum EXEMPLARIBUS duobus */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><p/></EXEMPLAR><EXEMPLAR><q/></EXEMPLAR>"
            "</EST><x/></CASUS></COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_COMMUTATIO_MALFORMATA);

        /* IX: EXEMPLAR cum attributo (relationes in plano impletionis
         * nullae) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR modus=\"unum\"><p/></EXEMPLAR></EST><x/>"
            "</CASUS></COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_COMMUTATIO_MALFORMATA);

        /* IX: captura loculum declaratum iterans */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><p $n/></EXEMPLAR></EST><x/></CASUS>"
            "</COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_COMMUTATIO_MALFORMATA);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "n");

        /* IX: tag cum referentia (LINEA) */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\" k=\"@k\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"&@k;\"><x/></CASUS></COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_COMMUTATIO_MALFORMATA);

        /* collectio: captura extra bracchium suum ignota */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><p $q/></EXEMPLAR></EST><x/></CASUS>"
            "<ORDINARIUS>&@q;</ORDINARIUS></COMMUTATIO></#></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_LOCULUS_IGNOTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "q");

        /* X: nullum bracchium exemplare congruit, ORDINARIUS abest */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"q\"><x/></CASUS></COMMUTATIO></#>"
            "<<#@f>><@n=><p/></></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium, STML_EXPANSIO_CASUS_NULLUS);
    }
}


    /* ==================================================
     * B1.5 (md-arbor-spec par. 6.5) - VOCATIO SUI ut PROBATIO: templum
     * se vocat (vocatione aut PER voca=) solum argumentis subarboreis
     * DESCENDENTIBUS STRICTIS argumentorum instantiationis currentis.
     * Exitus B1 (consilium): lista nidificata spicae -> <ul> unum cum
     * <ul> intra <li> suum. Vitium XXXI.
     * ================================================== */

    /* --- dispensator sui vocans: lista nidificata spicae --- */

interior vacuum
_sectio_nonagesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@md-nodus n=\"@n\"><COMMUTATIO de=\"&@n;\">"
            "<CASUS tag=\"documentum\"><PER de=\"@n.bloci\" voca=\"#@md-nodus\"/>"
            "</CASUS>"
            "<CASUS tag=\"lista\"><ul><PER de=\"@n.elementa\" voca=\"#@md-nodus\"/>"
            "</ul></CASUS>"
            "<CASUS tag=\"elementum\"><li><PER de=\"@n.bloci\" voca=\"#@md-nodus\"/>"
            "</li></CASUS>"
            "<CASUS tag=\"textus\">&@n.crudum!;</CASUS>"
            "<ORDINARIUS/></COMMUTATIO></#>"
            "<documentum><bloci><lista><elementa>"
            "<elementum><bloci><textus><crudum><t!>one</t></crudum></textus>"
            "<lista><elementa><elementum><bloci><textus><crudum><t!>nested</t>"
            "</crudum></textus></bloci></elementum></elementa></lista>"
            "</bloci></elementum>"
            "<elementum><bloci><textus><crudum><t!>two</t></crudum></textus>"
            "</bloci></elementum>"
            "</elementa></lista></bloci></documentum>"
            "<EXEMPLAR modus=\"unum\" output=\"$d\"><documentum $d/></EXEMPLAR>"
            "<html><PER congruentia=\"$d\"><<#@md-nodus>><@n=>&@d;</></PER></html>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            chorda scriptum = stml_scribere(e.radix_expansa, piscina,
                FALSUM);
            chorda html     = chorda_ex_literis(
                "<html><ul><li>one<ul><li>nested</li></ul></li><li>two</li>"
                "</ul></html></radix>", piscina);

            CREDO_VERUM (scriptum.mensura > html.mensura);
            si (scriptum.mensura > html.mensura)
            {
                chorda cauda;

                cauda.datum = scriptum.datum + scriptum.mensura
                    - html.mensura;
                cauda.mensura = html.mensura;
                CREDO_CHORDA_AEQUALIS (cauda, html);
            }
        }
    }
}

    /* --- vocatio sui explicita per proiectionem (descensus strictus);
     * terminatio per bracchium exemplare --- */

interior vacuum
_sectio_nonagesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix>"
            "<#@f n=\"@n\"><d><COMMUTATIO de=\"&@n;\">"
            "<CASUS><EST><EXEMPLAR><a><liberi/></a></EXEMPLAR></EST>"
            "<<#@f>><@n=>&@n.liberi;</></CASUS>"
            "<ORDINARIUS><leaf/></ORDINARIUS></COMMUTATIO></d></#>"
            "<w><a><liberi><a><liberi><a/></liberi></a></liberi></a></w>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><w><a $c/></w></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><w><a><liberi><a><liberi><a/></liberi></a></liberi></a>"
                "</w><d><d><d><leaf/></d></d></d></radix>");
        }
    }
}

    /* --- XXXI: argumentum idem, blocum recens, scalaria sola; recursio
     * mutua POSTERIUS manet --- */

interior vacuum
_sectio_nonagesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><<#@f>><@n=>&@n;</></#>"
            "<<#@f>><@n=><x/></></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_RECURSIO_NON_DESCENDENS);
        CREDO_CHORDA_AEQUALIS_LITERIS (e.fragmentum, "@f");
        CREDO_CHORDA_AEQUALIS_LITERIS (e.loculus, "n");

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><<#@f>><@n=><y/></></#>"
            "<<#@f>><@n=><x/></></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_RECURSIO_NON_DESCENDENS);

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f k=\"@k\"><<#@f k=\"1\">></#><<#@f k=\"0\">></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_RECURSIO_NON_DESCENDENS);

        /* PER voca= sui: ordines descendentes stricti argumenti (etiam
         * bloci recentis vocationis exterioris - descensus RELATIVUS
         * argumentis currentibus); involucrum vacuum = ordines nulli
         * = terminatio */
        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><d><PER de=\"@n.liberi\" voca=\"#@f\"/></d>"
            "</#><<#@f>><@n=><a><liberi><a><liberi/></a></liberi></a></>"
            "</radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><d><d/></d></radix>");
        }

        e = _expandere_litteras(piscina, intern,
            "<radix><#@g n=\"@n\"><<#@f>><@n=>&@n.liberi;</></#>"
            "<#@f n=\"@n\"><<#@g>><@n=>&@n.liberi;</></#>"
            "<<#@f>><@n=><a><liberi><a/></liberi></a></></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_AEQUALIS_I32 (e.vitium,
                            STML_EXPANSIO_FRAGMENTUM_POSTERIUS);
    }
}

    /* --- '!' super folium vacuum: octeti nulli (terminator materiae
     * intra lexemata cruda - B3.1 md); folium vacuum sine '!' silva
     * manet --- */

interior vacuum
_sectio_nonagesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlExpansioResultus e;

        e = _expandere_litteras(piscina, intern,
            "<radix><#@f n=\"@n\"><r>&@n.crudum!;</r><s>&@n.crudum;</s></#>"
            "<x><crudum><t!>a</t><nl/><t!>b</t></crudum></x>"
            "<EXEMPLAR modus=\"unum\" output=\"$c\"><x $c/></EXEMPLAR>"
            "<PER congruentia=\"$c\"><<#@f>><@n=>&@c;</></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><x><crudum><t!>a</t><nl/><t!>b</t></crudum></x>"
                "<r>ab</r><s><t!>a</t><nl/><t!>b</t></s></radix>");
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index sui: fluitans + PER", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "captura nodi + splex", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "subsequentia ordinata", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "elementum quodlibet '<*>'", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "captura iterata: aequalitas", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "captura iterata sub TRANSPARENTIA", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "captura iterata: forma auctoris", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "ancorata", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus fratrum", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "retentatio", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modus", _sectio_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "TRANSPARENTIA", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fragmentum dissolutum", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "transclusio praeterita", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "catenatio de=", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "strata: supra solum", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "PER voca=", _sectio_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "relatio vacua", _sectio_decima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vitia", _sectio_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "CATENA: catena basica", _sectio_vicesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "CATENA: de= involucri", _sectio_vicesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "CATENA: '<(>' ornamentum", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA nidificata: planatio", _sectio_vicesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA: templum -> EXEMPLAR", _sectio_vicesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA: templum -> CATENA", _sectio_vicesima_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA: templum -> silva graduum", _sectio_vicesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA: modus gradui", _sectio_vicesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "CATENA: vacuitas propagata", _sectio_vicesima_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "CATENA: vitia", _sectio_vicesima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "DIRIBITIO: custos verus", _sectio_tricesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "DIRIBITIO: custos falsus", _sectio_tricesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "DIRIBITIO: primum vincit", _sectio_tricesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: ORDINARIUS", _sectio_tricesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "DIRIBITIO: relatio originalis", _sectio_tricesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: catenae interius", _sectio_tricesima_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: nexus + est recursivum", _sectio_tricesima_sexta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: portatio templorum", _sectio_tricesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: ancorata + modus bracchii", _sectio_tricesima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "DIRIBITIO: vitia", _sectio_tricesima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "extensio: de= hereditat", _sectio_quadragesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "extensio: catena hereditat", _sectio_quadragesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "extensio: bracchium hereditat", _sectio_quadragesima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "extensio: collisio XXII", _sectio_quadragesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: filtrum fundamentale", _sectio_quadragesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: pons = iunctio", _sectio_quadragesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "SINE: medio catenae", _sectio_quadragesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "SINE: vacuitas propagata", _sectio_quadragesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: '<(>' ornamentum", _sectio_quadragesima_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: portatio templorum", _sectio_quadragesima_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: conditio DIRIBITIONIS", _sectio_quinquagesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: vitia attributorum XXIII", _sectio_quinquagesima_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "SINE: vitia corporis XXIII", _sectio_quinquagesima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "SINE: fons NIHIL XXIII", _sectio_quinquagesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "SINE: caecitas attributorum", _sectio_quinquagesima_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "SINE: alias contenti resolutum", _sectio_quinquagesima_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "SINE: bracchium directum XXI", _sectio_quinquagesima_sexta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "pons: gradus positivus", _sectio_quinquagesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "pons: vitium sine ordinibus", _sectio_quinquagesima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "angustans: filtrum semel", _sectio_quinquagesima_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "angustans: vestigium INDAGO", _sectio_sexagesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "angustans: ORDINARIUS XXI", _sectio_sexagesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "retentio: pluralitas scopi", _sectio_sexagesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "retentio: captura radicis propriae", _sectio_sexagesima_tertia,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "retentio: vitia XIII", _sectio_sexagesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "descensus: pertinentia", _sectio_sexagesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "descensus: sublatio ordinis", _sectio_sexagesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "descensus: regula V compatibilis", _sectio_sexagesima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "descensus: caecitas", _sectio_sexagesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "descensus: in SINE", _sectio_sexagesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "descensus: compositio cursoris", _sectio_septuagesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "descensus: vitia XXV", _sectio_septuagesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "INDAGO: catena + PER", _sectio_septuagesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "INDAGO: diribitio", _sectio_septuagesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "INDAGO: vitia XXIV", _sectio_septuagesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "proiectio: scalaris + via catenata", _sectio_septuagesima_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: silva, '!', involucrum vacuum",
        _sectio_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: '!' super structuram", _sectio_septuagesima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: vitia XXVII / XXVIII", _sectio_septuagesima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: vitium XXVI + littera", _sectio_septuagesima_nona,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: COMMUTATIO de=proiectio", _sectio_octogesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "proiectio: transitus in argumentum bloci",
        _sectio_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "PER silvae: delegatio voca=", _sectio_octogesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "PER silvae: corpus ut=", _sectio_octogesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "PER silvae: vacua + nidificata", _sectio_octogesima_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "PER silvae: vitia", _sectio_octogesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "CASUS tag=: dispensatio", _sectio_octogesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "CASUS <EST>: formae + capturae", _sectio_octogesima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "CASUS <EST>: forma parametrizata + absens",
        _sectio_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "CASUS exemplaria: vitia", _sectio_octogesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vocatio sui: dispensator, lista nidificata", _sectio_nonagesima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "vocatio sui: explicita, proiectio descendens",
        _sectio_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vocatio sui: vitia XXXI / POSTERIUS", _sectio_nonagesima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "proiectio: '!' folium vacuum", _sectio_nonagesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("stml_exemplaria", SECTIONES);
}
