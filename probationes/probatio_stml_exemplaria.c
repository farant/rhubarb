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
