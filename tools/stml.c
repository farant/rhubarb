/* stml.c (instrumentum) - instrumentum crustae STML: verba super
 * documenta .stml. Via fabricae bin/stml; institutum ~/.bin/stml
 * (in PATH), sicut silex.
 *
 * Verba:
 *   stml formare <via.stml> [-ad <exitus>] [-probare]
 *
 * formare = forma pulchra (semita pulchra, stml_scribere VERUM)
 * in stdout emissa; -ad <via> in plagulam scribit (numquam viam
 * introitus ipsam); -probare nihil emittit, formam pulchram
 * octetis disci confert (contractus lint pro perlustrationibus).
 *
 * SINE EXPANSIONE: formator FONTEM format, numquam sensum -
 * definitiones templorum, vocationes, mandata relationalia
 * intacta manent. Porta pulchra (probatio_stml_pulchrum) numquam
 * expandit; illa lex hic servatur. Spec par. 4 catenam par. 3
 * (expandere/distribuere) nominat - deliberate hic divergimus:
 * expansio verbo 'expandere' futuro pertinet, aliter formare et
 * expandere unum verbum essent et -probare omnem plagulam
 * templigeram in perpetuum divergentem clamaret.
 *
 * Contractus exitus:
 *   0 = emissum / conformis
 *   1 = divergentiae (-probare solum)
 *   2 = recusatio (usus / illegibilis / non parsabilis) - semper
 *       CLAMOSA: nuntius in stderr plagulam et causam nominat;
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
#include "filum.h"

#include <stdio.h>
#include <string.h>

interior vacuum
_usus_imprimere (vacuum)
{
    fputs("usus: stml formare <via.stml> [-ad <exitus>]"
          " [-probare]\n", stderr);
}

/* formare: legere -> scribere pulchre. Reddit codicem exitus. */
interior s32
_formare_currere (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* via      = NIHIL;
     constans character* ad       = NIHIL;
                    b32  probare  = FALSUM;
                integer  i;
            FilumStatus  status;
                 chorda  textus;
           StmlResultus  lectum;
                 chorda  emissum;

    per (i = II; i < argc; i++)
    {
        si (strcmp(argv[i], "-probare") == ZEPHYRUM)
        {
            probare = VERUM;
        }
        alioquin si (strcmp(argv[i], "-ad") == ZEPHYRUM)
        {
            si (i + I >= argc)
            {
                fprintf(stderr,
                    "stml formare: -ad viam exitus postulat\n");
                redde II;
            }
            i++;
            ad = argv[i];
        }
        alioquin si (argv[i][ZEPHYRUM] == '-')
        {
            fprintf(stderr,
                "stml formare: vexillum ignotum '%s'\n", argv[i]);
            _usus_imprimere();
            redde II;
        }
        alioquin si (via == NIHIL)
        {
            via = argv[i];
        }
        alioquin
        {
            fprintf(stderr,
                "stml formare: via una sola accipitur"
                " ('%s' superflua)\n", argv[i]);
            redde II;
        }
    }
    si (via == NIHIL)
    {
        _usus_imprimere();
        redde II;
    }
    si (probare && ad != NIHIL)
    {
        fprintf(stderr,
            "stml formare: -probare et -ad simul non congruunt"
            " (-probare nihil emittit)\n");
        redde II;
    }

    /* Truncatio ante lectionem: -ad viam introitus nominans
     * plagulam deleret priusquam legeretur. Collatio literalis
     * sola - aliases viarum ('./x' contra 'x') non capiuntur. */
    si (ad != NIHIL && strcmp(ad, via) == ZEPHYRUM)
    {
        fprintf(stderr,
            "stml formare: -ad viam introitus ipsam nominat"
            " ('%s') - truncatio ante lectionem, RECUSATUM\n",
            via);
        redde II;
    }

    si (!filum_status(via, &status))
    {
        fprintf(stderr, "stml formare: '%s' non exsistit\n", via);
        redde II;
    }
    si (!status.est_filum)
    {
        fprintf(stderr,
            "stml formare: '%s' non est plagula regularis\n",
            via);
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
        fprintf(stderr, "stml formare: '%s' legi nequit\n", via);
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
        fprintf(stderr, "stml formare: %s:%u:%u: %.*s\n",
            via, lectum.linea_erroris, lectum.columna_erroris,
            lectum.error.mensura > ZEPHYRUM
                ? (integer)lectum.error.mensura : VI,
            lectum.error.mensura > ZEPHYRUM
                ? (constans character*)lectum.error.datum
                : "vitium");
        redde II;
    }

    /* Radix DOCUMENTI, non elementum_radix: genus in = genus ex;
     * commenta et processio ad gradum documenti aliter numquam
     * emitterentur (spec par. 3.1 lex III) */
    emissum = stml_scribere(lectum.radix, piscina, VERUM);
    si (emissum.datum == NIHIL && textus.mensura > ZEPHYRUM)
    {
        fprintf(stderr,
            "stml formare: '%s' - emissio defecit\n", via);
        redde II;
    }

    si (probare)
    {
        si (chorda_aequalis(emissum, textus))
        {
            redde ZEPHYRUM;
        }
        fprintf(stderr,
            "stml formare: '%s' a forma pulchra divergit%s\n",
            via,
            lectum.crlf_canonicalizatum
                ? " (CRLF -> LF canonicalizatum)" : "");
        redde I;
    }

    si (ad != NIHIL)
    {
        si (!filum_scribere(ad, emissum))
        {
            fprintf(stderr,
                "stml formare: '%s' scribi nequit\n", ad);
            redde II;
        }
        redde ZEPHYRUM;
    }

    si (fwrite(emissum.datum, (memoriae_index)I,
               (memoriae_index)emissum.mensura, stdout)
        != (memoriae_index)emissum.mensura)
    {
        fprintf(stderr,
            "stml formare: emissio in stdout defecit\n");
        redde II;
    }
    redde ZEPHYRUM;
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
    fprintf(stderr, "stml: verbum ignotum '%s'\n", argv[I]);
    _usus_imprimere();
    redde II;
}
