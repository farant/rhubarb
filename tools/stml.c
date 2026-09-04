/* stml.c (instrumentum) - instrumentum crustae STML: verba super
 * documenta .stml. Via fabricae bin/stml; institutum ~/.bin/stml
 * (in PATH), sicut silex.
 *
 * Verba:
 *   stml formare   <via.stml> [-ad <exitus>] [-probare]
 *   stml expandere <via.stml> [-ad <exitus>]
 *   stml vertere   <via.stml> [-ad <exitus>]
 *
 * formare = forma pulchra (semita pulchra, stml_scribere VERUM)
 * in stdout emissa; -ad <via> in plagulam scribit (numquam viam
 * introitus ipsam); -probare nihil emittit, formam pulchram
 * octetis disci confert (contractus lint pro perlustrationibus).
 *
 * formare SINE EXPANSIONE: formator FONTEM format, numquam sensum
 * - definitiones templorum, vocationes, mandata relationalia
 * intacta manent. Porta pulchra (probatio_stml_pulchrum) numquam
 * expandit; illa lex hic servatur. Spec par. 4 catenam par. 3
 * nominat - deliberate hic divergimus: expansio verbo 'expandere'
 * pertinet, aliter formare et expandere unum verbum essent et
 * -probare omnem plagulam templigeram in perpetuum divergentem
 * clamaret.
 *
 * expandere = catena spec par. 3 tota: legere -> stml_expandere
 * (templa impleta, mandata relationalia consumpta, INDAGO
 * sepulcra relinquit) -> stml_distribuere (involucra listarum
 * dissoluta - NON optionalis: cardinalitatem mutat, spec par. 3.1
 * lex V) -> scribere pulchre. Leges catenae servatae: unum
 * internamentum per catenam totam (lex I); porta in successu,
 * numquam in radice (lex II - vitium VII molliter ponitur);
 * radix DOCUMENTI pascitur (lex III).
 *
 * vertere = catena eadem, deinde emissio HTML per lib/stml_html.c
 * (doctype sponte; clausura per leges HTML; effugium duplex;
 * recusationes nominatae; singularia emittuntur ut scripta -
 * decretum Franis 2026-09-01).
 *
 * Contractus exitus (verba omnia):
 *   0 = emissum / conformis
 *   1 = divergentiae (-probare solum)
 *   2 = recusatio (usus / illegibilis / non parsabilis / vitium
 *       expansionis) - semper CLAMOSA: nuntius in stderr plagulam
 *       et causam nominat (vitia expansionis NOMINE, non numero);
 *       numquam partialis (nihil in -ad scribitur nisi documentum
 *       totum emissum est)
 *
 * Spec: project-specs/stml-instrumentum-spec.md
 * Aedificatio: ./tools/stml_struere.sh */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "stml_macros.h"
#include "stml_html.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>

interior vacuum
_usus_imprimere (vacuum)
{
    fputs("usus: stml <verbum> <via.stml> ...\n"
          "  stml formare   <via.stml> [-ad <exitus>] [-probare]\n"
          "  stml expandere <via.stml> [-ad <exitus>]\n"
          "  stml vertere   <via.stml> [-ad <exitus>]\n", stderr);
}

/* Argumenta communia verbi: via una, -ad, fortasse -probare. */
nomen structura {
    constans character* via;
    constans character* ad;
                   b32  probare;
} StmlVerbiArgumenta;

/* ZEPHYRUM = bene; alioquin codex exitus (nuntius iam emissus). */
interior s32
_argumenta_verbi_parsare (
    constans character*  verbum,
               integer   argc,
             character** argv,
                   b32   probare_licet,
    StmlVerbiArgumenta*  exitus)
{
    integer i;

    exitus->via      = NIHIL;
    exitus->ad       = NIHIL;
    exitus->probare  = FALSUM;

    per (i = II; i < argc; i++)
    {
        si (probare_licet && strcmp(argv[i], "-probare") == ZEPHYRUM)
        {
            exitus->probare = VERUM;
        }
        alioquin si (strcmp(argv[i], "-ad") == ZEPHYRUM)
        {
            si (i + I >= argc)
            {
                fprintf(stderr,
                    "stml %s: -ad viam exitus postulat\n", verbum);
                redde II;
            }
            i++;
            exitus->ad = argv[i];
        }
        alioquin si (argv[i][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "stml %s: vexillum ignotum '%s'\n",
                verbum, argv[i]);
            _usus_imprimere();
            redde II;
        }
        alioquin si (exitus->via == NIHIL)
        {
            exitus->via = argv[i];
        }
        alioquin
        {
            fprintf(stderr,
                "stml %s: via una sola accipitur ('%s' superflua)\n",
                verbum, argv[i]);
            redde II;
        }
    }
    si (exitus->via == NIHIL)
    {
        _usus_imprimere();
        redde II;
    }
    si (exitus->probare && exitus->ad != NIHIL)
    {
        fprintf(stderr,
            "stml %s: -probare et -ad simul non congruunt"
            " (-probare nihil emittit)\n", verbum);
        redde II;
    }

    /* Truncatio ante lectionem: -ad viam introitus nominans
     * plagulam deleret priusquam legeretur. Collatio literalis
     * sola - aliases viarum ('./x' contra 'x') non capiuntur. */
    si (   exitus->ad                      != NIHIL
        && strcmp(exitus->ad, exitus->via) == ZEPHYRUM)
    {
        fprintf(stderr,
            "stml %s: -ad viam introitus ipsam nominat ('%s')"
            " - truncatio ante lectionem, RECUSATUM\n",
            verbum, exitus->via);
        redde II;
    }
    redde ZEPHYRUM;
}

/* Praeparatio communis: plagulam probare, legere, parsare. UNUM
 * internamentum per catenam totam (spec par. 3.1 lex I) hic
 * nascitur. ZEPHYRUM = bene; alioquin codex exitus (nuntius iam
 * emissus). */
interior s32
_fons_parsare (
      constans character*  verbum,
      constans character*  via,
                 Piscina** piscina_exitus,
     InternamentumChorda** intern_exitus,
                  chorda*  textus_exitus,
            StmlResultus*  lectum_exitus)
{
                Piscina* piscina;
    InternamentumChorda* intern;
            FilumStatus  status;
                 chorda  textus;
           StmlResultus  lectum;

    si (!filum_status(via, &status))
    {
        fprintf(stderr, "stml %s: '%s' non exsistit\n", verbum,
            via);
        redde II;
    }
    si (!status.est_filum)
    {
        fprintf(stderr, "stml %s: '%s' non est plagula regularis\n",
            verbum, via);
        redde II;
    }

    piscina = piscina_generare_dynamicum("stml", 65536);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "stml: piscina generari non potuit\n");
        redde II;
    }

    /* filum_legere_totum mensuram ZEPHYRUM in defectu reddit -
     * plagula vacua vera per filum_mensura discernitur */
    textus = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM && filum_mensura(via) > ZEPHYRUM)
    {
        fprintf(stderr, "stml %s: '%s' legi nequit\n", verbum, via);
        redde II;
    }

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        fprintf(stderr,
            "stml: internamentum generari non potuit\n");
        redde II;
    }

    lectum = stml_legere(textus, piscina, intern);
    si (!lectum.successus)
    {
        fprintf(stderr, "stml %s: %s:%u:%u: %.*s\n",
            verbum, via, lectum.linea_erroris,
            lectum.columna_erroris,
            lectum.error.mensura > ZEPHYRUM
                ? (integer)lectum.error.mensura : VI,
            lectum.error.mensura > ZEPHYRUM
                ? (constans character*)lectum.error.datum
                : "vitium");
        redde II;
    }

    *piscina_exitus  = piscina;
    *intern_exitus   = intern;
    *textus_exitus   = textus;
    *lectum_exitus   = lectum;
    redde ZEPHYRUM;
}

/* Cauda communis: in stdout aut -ad emittere. */
interior s32
_exitum_emittere (
    constans character* verbum,
                chorda  emissum,
    constans character* ad)
{
    si (ad != NIHIL)
    {
        si (!filum_scribere(ad, emissum))
        {
            fprintf(stderr, "stml %s: '%s' scribi nequit\n",
                verbum, ad);
            redde II;
        }
        redde ZEPHYRUM;
    }
    si (fwrite(emissum.datum, (memoriae_index)I,
               (memoriae_index)emissum.mensura, stdout)
        != (memoriae_index)emissum.mensura)
    {
        fprintf(stderr, "stml %s: emissio in stdout defecit\n",
            verbum);
        redde II;
    }
    redde ZEPHYRUM;
}

/* formare: legere -> scribere pulchre. Reddit codicem exitus. */
interior s32
_formare_currere (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     StmlVerbiArgumenta  arg;
                 chorda  textus;
           StmlResultus  lectum;
                 chorda  emissum;
                    s32  codex;

    codex = _argumenta_verbi_parsare("formare", argc, argv, VERUM,
                                     &arg);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }
    codex = _fons_parsare("formare", arg.via, &piscina, &intern,
                          &textus, &lectum);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }

    /* Radix DOCUMENTI, non elementum_radix: genus in = genus ex;
     * commenta et processio ad gradum documenti aliter numquam
     * emitterentur (spec par. 3.1 lex III) */
    emissum = stml_scribere(lectum.radix, piscina, VERUM);
    si (emissum.datum == NIHIL && textus.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "stml formare: '%s' - emissio defecit\n",
            arg.via);
        redde II;
    }

    si (arg.probare)
    {
        si (chorda_aequalis(emissum, textus))
        {
            redde ZEPHYRUM;
        }
        fprintf(stderr,
            "stml formare: '%s' a forma pulchra divergit%s\n",
            arg.via,
            lectum.crlf_canonicalizatum
                ? " (CRLF -> LF canonicalizatum)" : "");
        redde I;
    }
    redde _exitum_emittere("formare", emissum, arg.ad);
}

/* Titulus vitii expansionis - numerus solus auctori nihil dicit. */
interior constans character*
_vitium_expansionis_titulus (
    StmlExpansioVitium vitium)
{
    commutatio (vitium)
    {
        casus STML_EXPANSIO_FRAGMENTUM_IGNOTUM:
            redde "FRAGMENTUM_IGNOTUM";
        casus STML_EXPANSIO_FRAGMENTUM_POSTERIUS:
            redde "FRAGMENTUM_POSTERIUS";
        casus STML_EXPANSIO_FRAGMENTUM_GEMINUM:
            redde "FRAGMENTUM_GEMINUM";
        casus STML_EXPANSIO_LOCULUS_NON_IMPLETUS:
            redde "LOCULUS_NON_IMPLETUS";
        casus STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM:
            redde "ARGUMENTUM_SUPERFLUUM";
        casus STML_EXPANSIO_LOCULUS_IGNOTUS:
            redde "LOCULUS_IGNOTUS";
        casus STML_EXPANSIO_ARGUMENTUM_ARBOREUM:
            redde "ARGUMENTUM_ARBOREUM";
        casus STML_EXPANSIO_ARGUMENTUM_GEMINUM:
            redde "ARGUMENTUM_GEMINUM";
        casus STML_EXPANSIO_COMMUTATIO_MALFORMATA:
            redde "COMMUTATIO_MALFORMATA";
        casus STML_EXPANSIO_CASUS_NULLUS:
            redde "CASUS_NULLUS";
        casus STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS:
            redde "LOCULUS_NON_ANGUSTATUS";
        casus STML_EXPANSIO_EXEMPLAR_SINE_EXITU:
            redde "EXEMPLAR_SINE_EXITU";
        casus STML_EXPANSIO_EXEMPLAR_MALFORMATUM:
            redde "EXEMPLAR_MALFORMATUM";
        casus STML_EXPANSIO_SCOPUS_IGNOTUS:
            redde "SCOPUS_IGNOTUS";
        casus STML_EXPANSIO_CORPUS_SILVESTRE:
            redde "CORPUS_SILVESTRE";
        casus STML_EXPANSIO_UNUM_VIOLATUM:
            redde "UNUM_VIOLATUM";
        casus STML_EXPANSIO_OPTIONAL_MULTIPLEX:
            redde "OPTIONAL_MULTIPLEX";
        casus STML_EXPANSIO_EXITUS_NON_CONSUMPTUS:
            redde "EXITUS_NON_CONSUMPTUS";
        casus STML_EXPANSIO_TRANSPARENTIA_MALFORMATA:
            redde "TRANSPARENTIA_MALFORMATA";
        casus STML_EXPANSIO_CATENA_MALFORMATA:
            redde "CATENA_MALFORMATA";
        casus STML_EXPANSIO_DIRIBITIO_MALFORMATA:
            redde "DIRIBITIO_MALFORMATA";
        casus STML_EXPANSIO_CAPTURA_COLLISA:
            redde "CAPTURA_COLLISA";
        casus STML_EXPANSIO_SINE_MALFORMATUM:
            redde "SINE_MALFORMATUM";
        casus STML_EXPANSIO_INDAGO_MALFORMATUM:
            redde "INDAGO_MALFORMATUM";
        casus STML_EXPANSIO_DESCENSUS_MALFORMATUS:
            redde "DESCENSUS_MALFORMATUS";
        casus STML_EXPANSIO_PROIECTIO_MALFORMATA:
            redde "PROIECTIO_MALFORMATA";
        casus STML_EXPANSIO_PROIECTIO_ABSENS:
            redde "PROIECTIO_ABSENS";
        casus STML_EXPANSIO_PROIECTIO_AMBIGUA:
            redde "PROIECTIO_AMBIGUA";
        casus STML_EXPANSIO_PER_DELEGATIO_AMBIGUA:
            redde "PER_DELEGATIO_AMBIGUA";
        casus STML_EXPANSIO_ARGUMENTUM_SCALARE:
            redde "ARGUMENTUM_SCALARE";
        ordinarius:
            redde "IGNOTUM";
    }
}

/* Catena spec par. 3 (expandere -> distribuere) cum nuntiis suis.
 * Porta in successu, NUMQUAM in radice: vitium VII molliter
 * ponitur et radix_expansa non-NIHIL sed falsa manet (lex II).
 * Distributio NON optionalis: cardinalitatem et praesentiam
 * attributorum mutat (lex V). ZEPHYRUM = bene (radix_exitus
 * impleta); alioquin codex exitus (nuntius iam emissus). */
interior s32
_catenam_exsequi (
     constans character*  verbum,
     constans character*  via,
                Piscina*  piscina,
    InternamentumChorda*  intern,
              StmlNodus*  radix,
              StmlNodus** radix_exitus)
{
       StmlExpansioResultus expansio;
    StmlDistributioResultus distributio;

    expansio = stml_expandere(radix, piscina, intern);
    si (!expansio.successus)
    {
        fprintf(stderr, "stml %s: %s:%u: %s",
            verbum, via, expansio.linea,
            _vitium_expansionis_titulus(expansio.vitium));
        si (expansio.fragmentum.mensura > ZEPHYRUM)
        {
            fprintf(stderr, " (fragmentum '%.*s')",
                (integer)expansio.fragmentum.mensura,
                (constans character*)expansio.fragmentum.datum);
        }
        si (expansio.loculus.mensura > ZEPHYRUM)
        {
            fprintf(stderr, " (loculus '%.*s')",
                (integer)expansio.loculus.mensura,
                (constans character*)expansio.loculus.datum);
        }
        fputc('\n', stderr);
        redde II;
    }

    distributio = stml_distribuere(expansio.radix_expansa, piscina,
                                   intern);
    si (!distributio.successus)
    {
        fprintf(stderr, "stml %s: %s:%u: distributio %s",
            verbum, via, distributio.linea,
            distributio.vitium == STML_DISTRIBUTIO_MIXTA
                ? "MIXTA" : "MEMORIA");
        si (distributio.titulus.mensura > ZEPHYRUM)
        {
            fprintf(stderr, " (involucrum '%.*s')",
                (integer)distributio.titulus.mensura,
                (constans character*)distributio.titulus.datum);
        }
        fputc('\n', stderr);
        redde II;
    }
    *radix_exitus = distributio.radix_distributa;
    redde ZEPHYRUM;
}

/* expandere: catena tota (legere -> expandere -> distribuere) et
 * fructus pulchre emissus. Reddit codicem exitus. */
interior s32
_expandere_currere (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     StmlVerbiArgumenta  arg;
                 chorda  textus;
           StmlResultus  lectum;
              StmlNodus* radix;
                 chorda  emissum;
                    s32  codex;

    codex = _argumenta_verbi_parsare("expandere", argc, argv,
                                     FALSUM, &arg);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }
    codex = _fons_parsare("expandere", arg.via, &piscina, &intern,
                          &textus, &lectum);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }
    codex = _catenam_exsequi("expandere", arg.via, piscina, intern,
                             lectum.radix, &radix);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }

    emissum = stml_scribere(radix, piscina, VERUM);
    si (emissum.datum == NIHIL && textus.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "stml expandere: '%s' - emissio defecit\n",
            arg.via);
        redde II;
    }
    redde _exitum_emittere("expandere", emissum, arg.ad);
}

/* vertere: catena tota, deinde emissio HTML (lib/stml_html.c).
 * Recusationes emissoris NOMINE cum detail (tagum/attributum/ens
 * peccans). */
interior s32
_vertere_currere (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     StmlVerbiArgumenta  arg;
                 chorda  textus;
           StmlResultus  lectum;
              StmlNodus* radix;
       StmlHtmlResultus  versio;
                    s32  codex;

    codex = _argumenta_verbi_parsare("vertere", argc, argv,
                                     FALSUM, &arg);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }
    codex = _fons_parsare("vertere", arg.via, &piscina, &intern,
                          &textus, &lectum);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }
    codex = _catenam_exsequi("vertere", arg.via, piscina, intern,
                             lectum.radix, &radix);
    si (codex != ZEPHYRUM)
    {
        redde codex;
    }

    versio = stml_html_vertere(radix, piscina);
    si (!versio.successus)
    {
        fprintf(stderr, "stml vertere: %s: %s",
            arg.via, stml_html_vitium_titulus(versio.vitium));
        si (versio.detail.mensura > ZEPHYRUM)
        {
            fprintf(stderr, " ('%.*s')",
                (integer)versio.detail.mensura,
                (constans character*)versio.detail.datum);
        }
        fputc('\n', stderr);
        redde II;
    }
    redde _exitum_emittere("vertere", versio.html, arg.ad);
}

integer
principale (
      integer   argc,
    character** argv)
{
    si (argc < II)
    {
        _usus_imprimere();
        redde II;
    }
    si (strcmp(argv[I], "formare") == ZEPHYRUM)
    {
        redde (integer)_formare_currere(argc, argv);
    }
    si (strcmp(argv[I], "expandere") == ZEPHYRUM)
    {
        redde (integer)_expandere_currere(argc, argv);
    }
    si (strcmp(argv[I], "vertere") == ZEPHYRUM)
    {
        redde (integer)_vertere_currere(argc, argv);
    }
    fprintf(stderr, "stml: verbum ignotum '%s'\n", argv[I]);
    _usus_imprimere();
    redde II;
}
