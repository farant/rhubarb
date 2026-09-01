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

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_stml_exemplaria",
                                         4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    /* --- index sui ipsius: fluitans + PER textus (spec par. 3) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- index sui: fluitans + PER ---\n");
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

    /* --- captura nodi + splex silvae in PER --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- captura nodi + splex ---\n");
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

    /* --- subsequentia ordinata: A..C congruit, C..A non --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- subsequentia ordinata ---\n");
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

    /* --- '<*>' elementum quodlibet --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- elementum quodlibet '<*>' ---\n");
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

    /* --- captura iterata = aequalitas (regula V) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- captura iterata: aequalitas ---\n");
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

    /* --- ancorata: radix scopi sola --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- ancorata ---\n");
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

    /* --- modus: unum / optional / primum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- modus ---\n");
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

    /* --- TRANSPARENTIA: caecitas mensurabilis (spec par. 2.7) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- TRANSPARENTIA ---\n");
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

    /* --- visio contenti: fragmentum dissolvitur in liberis --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- fragmentum dissolutum ---\n");
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

    /* --- visio contenti: transclusio praeterita (identitas semel) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- transclusio praeterita ---\n");
        e = _expandere_litteras(piscina, intern,
            "<radix><f><#g><a/></#><<#g>></f>"
            "<EXEMPLAR modus=\"unum\" output=\"$m\"><a/></EXEMPLAR>"
            "<PER congruentia=\"$m\"><n/></PER></radix>");
        /* definitio numerat, transclusio non - congruentia UNA */
        CREDO_VERUM (e.successus);
    }

    /* --- catenatio de=: angustatio super relationem priorem --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- catenatio de= ---\n");
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

    /* --- lex stratorum: contentum INFRA non visum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- strata: supra solum ---\n");
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

    /* --- PER voca=: delegatio (pons = vocatio) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- PER voca= ---\n");
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

    /* --- relatio vacua: legalis, PER nihil parit --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- relatio vacua ---\n");
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

    /* --- vitia XII, XIII, XIV, XV, XVIII, XIX --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- vitia ---\n");
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

    /* --- CATENA: catena basica == catenatio nominata --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: catena basica ---\n");
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

    /* --- CATENA: de= involucri (lex eadem ac EXEMPLAR) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: de= involucri ---\n");
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

    /* --- CATENA: saccharum '<(>' = ornamentum (ratificatio V) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: '<(>' ornamentum ---\n");
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

    /* --- CATENA nidificata litteralis: PLANATUR --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA nidificata: planatio ---\n");
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

    /* --- nexus ex templo: vocatio -> EXEMPLAR (portatio) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: templum -> EXEMPLAR ---\n");
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

    /* --- nexus ex templo: vocatio -> CATENA (planatio) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: templum -> CATENA ---\n");
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

    /* --- nexus ex templo: silva graduum SPLICATUR --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: templum -> silva graduum ---\n");
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

    /* --- modus= gradui: primum truncat, unum medium clamat --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: modus gradui ---\n");
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

    /* --- catena vacuans: gradus sine congruentiis -> relatio
     *     vacua propagatur, PER nihil parit --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: vacuitas propagata ---\n");
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

    /* --- vitia XX (CATENA malformata) + XII/XIV/XVIII --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- CATENA: vitia ---\n");
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

    /* --- DIRIBITIO: custos verus -> bracchium applicatum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: custos verus ---\n");
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

    /* --- DIRIBITIO: custos falsus -> relatio vacua effluens --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: custos falsus ---\n");
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

    /* --- DIRIBITIO: primum-congruens-vincit --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: primum vincit ---\n");
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

    /* --- DIRIBITIO: ORDINARIUS = bracchium-semper --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: ORDINARIUS ---\n");
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

    /* --- DIRIBITIO: bracchium relationem ORIGINALEM accipit
     *     (custos, non angustator - decretum) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: relatio originalis ---\n");
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

    /* --- DIRIBITIO: catena ut conditio et ut bracchium --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: catenae interius ---\n");
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

    /* --- DIRIBITIO ut nexus catenae + diribitio in est --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: nexus + est recursivum ---\n");
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

    /* --- DIRIBITIO ex templo (nexus) + templum in est --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: portatio templorum ---\n");
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

    /* --- DIRIBITIO: conditio ancorata + modus in bracchio --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: ancorata + modus bracchii ---\n");
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

    /* --- vitia XXI + XVIII --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- DIRIBITIO: vitia ---\n");
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

    /* --- LEX EXTENSIONIS: capturae per catenam accumulantur,
     *     tabula PER omnia accumulata fert (decretum) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- extensio: de= hereditat ---\n");
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

    /* --- extensio per gradus CATENAE --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- extensio: catena hereditat ---\n");
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

    /* --- extensio per bracchium DIRIBITIONIS --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- extensio: bracchium hereditat ---\n");
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

    /* --- XXII: collisio capturarum trans gradus --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- extensio: collisio XXII ---\n");
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


    /* =============== SINE (antiiunctio, XXIII) =============== */

    /* --- SINE: filtrum fundamentale (servare in ZEPHYRO) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: filtrum fundamentale ---\n");
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

    /* --- SINE: PONS IPSE IUNCTIO EST (correlatio per ordinem) ---
     * probatio incorrelata ambos necaret (u adest utrobique);
     * pons '&@n;' solum ordinem primum necat */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: pons = iunctio ---\n");
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

    /* --- SINE medio catenae: filtrum purum (radix eadem,
     * capturae hereditatae per SINE fluunt) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: medio catenae ---\n");
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

    /* --- SINE: vacuitas propagata (relatio vacua influens) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: vacuitas propagata ---\n");
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

    /* --- SINE: '<(>' ornamentum (dissolutio fragmentorum) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: '<(>' ornamentum ---\n");
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

    /* --- SINE ex templo portatum (vocatio -> nexus filtrans) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: portatio templorum ---\n");
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

    /* --- dualitas: catena SINE ferens ut conditio DIRIBITIONIS
     * ('si quid DEEST, bracchium hoc sume') --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: conditio DIRIBITIONIS ---\n");
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

    /* --- XXIII: attributa machinae in SINE --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: vitia attributorum XXIII ---\n");
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

    /* --- XXIII: corpus malformatum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: vitia corporis XXIII ---\n");
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

    /* --- XXIII: fons NIHIL (absentia lata documenti = ianua
     * posterior, CLARE recusata) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: fons NIHIL XXIII ---\n");
        e = _expandere_litteras(piscina, intern,
            "<radix><a/><CATENA output=\"$rr\">"
            "<SINE><q/></SINE></CATENA>"
            "<PER congruentia=\"$rr\"><x/></PER></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_SINE_MALFORMATUM);
    }

    /* --- caecitas attributorum (TRANSPARENTIA attributa=): forma
     * splicata provenientiam fert (b=) quae littera fieret -
     * caecitate declarata usus invenitur, ordo necatur --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: caecitas attributorum ---\n");
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

    /* --- aliasa contenti resoluta (congruentia per
     * repraesentationem videt): usus solum per '<<#id>>' testatum
     * ordinem necat --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: alias contenti resolutum ---\n");
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

    /* --- SINE ut mandatum bracchii directum = XXI (lex vetus:
     * mandata pura EXEMPLAR/CATENA/DIRIBITIO sola; catena SINE
     * ferens via composita est) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- SINE: bracchium directum XXI ---\n");
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


    /* ========= RETENTIO RADICIS 'radix="fontis"' ========= */

    /* --- pluralitas in scopo uno: ordo per declarationem, radix
     * scopi retenta - antiiunctio per-declarationem (forma codicis
     * 69 in parvo), deinde gradus ulterior ex radice retenta --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- retentio: pluralitas scopi ---\n");
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

    /* --- radix congruentiae ipsius per capturam claram --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- retentio: captura radicis propriae ---\n");
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

    /* --- XIII: valor ignotus; fontis sine ordine intrante --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- retentio: vitia XIII ---\n");
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


    /* ============ DESCENSUS '<**>' (axis, XXV) ============ */

    /* --- pertinentia: captura per profunditatem ignotam --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: pertinentia ---\n");
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

    /* --- sublatio: existentiale extra ordinem fratrum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: sublatio ordinis ---\n");
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

    /* --- ligatio prima COMPATIBILIS: regula V per descensum
     * (iunctio sui profunda - probatio se citans) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: regula V compatibilis ---\n");
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

    /* --- caecitas: descensus perspicua numquam intrat --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: caecitas ---\n");
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

    /* --- descensus in corpore SINE (absentia profunda) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: in SINE ---\n");
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

    /* --- compositio positionalis: '<**>' intra '<*>' in sede
     * cursoris (descensus positionalis numquam primitivum) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: compositio cursoris ---\n");
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

    /* --- XXV: forma descensus mala (praeparatione, CLARE) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- descensus: vitia XXV ---\n");
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


    /* =============== INDAGO (vestigatio, XXIV) =============== */

    /* --- lapis sepulcralis catenae + PER intra relatum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- INDAGO: catena + PER ---\n");
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

    /* --- lapis DIRIBITIONIS: praeteritum + ORDINARIUS + fabula
     * bracchii --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- INDAGO: diribitio ---\n");
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

    /* --- XXIV: INDAGO non nudum --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- INDAGO: vitia XXIV ---\n");
        e = _expandere_litteras(piscina, intern,
            "<radix><INDAGO malum=\"1\"/><a/></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_INDAGO_MALFORMATUM);

        e = _expandere_litteras(piscina, intern,
            "<radix><INDAGO><a/></INDAGO></radix>");
        CREDO_VERUM (!e.successus);
        CREDO_VERUM (e.vitium == STML_EXPANSIO_INDAGO_MALFORMATUM);
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        si (praeteritus)
        {
            redde ZEPHYRUM;
        }
    }
    redde I;
}
