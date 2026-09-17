/* probatio_crusta_diagnostica.c - Concordia diagnosticorum (plan
 * materia-sedes B3; spec par. IX)
 *
 * Diagnostica DERIVATA (CRUSTA_DIAGNOSTICA declarata et emissa a
 * parsatore) contra numeros parsatoris, qui ORACULUM INDEPENDENS
 * sunt: praesentia per classem (mala, clausurae, vacuae,
 * transpositae) et 'sana' si et solum si diagnosticum nullum, super
 * corpus domus, casus oraculi (pathologiae + FreeBSD) et fixa
 * adversarii. Domus sana: errata et monita ZEPHYRUM pinnata. Numeri
 * utriusque lateris summantur et imprimuntur - machina arithmetica
 * pro vestigio uno plus semel numerare potest (delta D7), ergo
 * praesentia, non numerus, concordat.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOMUS_ERRATA     0
#define DOMUS_MONITA     0
#define DOMUS_MINIMUM    200
#define ORACULI_MINIMUM  100

nomen structura {
    i32 documenta;
    i32 discordes;
    i32 mala_parsatoris;
    i32 mala_derivata;
    i32 clausurae_parsatoris;
    i32 clausurae_derivatae;
    i32 vacuae_parsatoris;
    i32 vacuae_derivatae;
    i32 transpositae_parsatoris;
    i32 transpositae_derivatae;
    i32 errata;
    i32 monita;
} Summa;

interior vacuum
_documentum (
                Piscina* piscina,
    constans character* titulus,
    constans character* fons,
                    i32  mensura,
                  Summa* summa)
{
      CrustaParsura relatio;
    CrustaConcordia concordia;
       MateriaNodus* radix;

    memset(&relatio, ZEPHYRUM, magnitudo(relatio));
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        &relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde;
    }
    (vacuum)crusta_diagnostica_concordia(piscina, radix, &relatio,
        &concordia);
    CREDO_VERUM (concordia.concordat);
    si (!concordia.concordat)
    {
        summa->discordes++;
        imprimere("  DISCORS %s (%s): parsator m%d c%d v%d t%d sana %d"
            " | derivata m%d c%d v%d t%d e%d mon%d\n", titulus,
            concordia.causa != NIHIL ? concordia.causa : "-",
            (integer)relatio.mala,
            (integer)relatio.clausurae_absentes,
            (integer)relatio.listae_vacuae,
            (integer)relatio.heredoca_transposita,
            (integer)relatio.sana, (integer)concordia.mala,
            (integer)concordia.clausurae, (integer)concordia.vacuae,
            (integer)concordia.transpositae, (integer)concordia.errata,
            (integer)concordia.monita);
    }
    summa->documenta++;
    summa->mala_parsatoris          += relatio.mala;
    summa->mala_derivata            += concordia.mala;
    summa->clausurae_parsatoris     += relatio.clausurae_absentes;
    summa->clausurae_derivatae      += concordia.clausurae;
    summa->vacuae_parsatoris        += relatio.listae_vacuae;
    summa->vacuae_derivatae         += concordia.vacuae;
    summa->transpositae_parsatoris  += relatio.heredoca_transposita;
    summa->transpositae_derivatae   += concordia.transpositae;
    summa->errata                   += concordia.errata;
    summa->monita                   += concordia.monita;
}

interior vacuum
_summam_imprimere (
    constans character* titulus,
        constans Summa* summa)
{
    imprimere("  %s: %d documenta, discordes %d | mala %d/%d, "
        "clausurae %d/%d, vacuae %d/%d, transpositae %d/%d "
        "(parsator/derivata) | errata %d, monita %d\n", titulus,
        (integer)summa->documenta, (integer)summa->discordes,
        (integer)summa->mala_parsatoris,
        (integer)summa->mala_derivata,
        (integer)summa->clausurae_parsatoris,
        (integer)summa->clausurae_derivatae,
        (integer)summa->vacuae_parsatoris,
        (integer)summa->vacuae_derivatae,
        (integer)summa->transpositae_parsatoris,
        (integer)summa->transpositae_derivatae,
        (integer)summa->errata, (integer)summa->monita);
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix = getenv("RHUBARB_RADIX");
    constans character* causa;
                 Summa  domus;
                 Summa  oraculi;
                 Summa  adversarii;
             character* lista;
                   Xar* collectio;
                   i32  mensura = ZEPHYRUM;
                   i32  k;

    piscina = piscina_generare_dynamicum("probatio_crusta_diagnostica",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }
    memset(&domus, ZEPHYRUM, magnitudo(domus));
    memset(&oraculi, ZEPHYRUM, magnitudo(oraculi));
    memset(&adversarii, ZEPHYRUM, magnitudo(adversarii));

    /* FORMAE PROPRIAE: quas corpora non ferunt (planta
     * 'inanis' viridis mansit donec hi adessent - lista separatore
     * sola, lista vacua, clausura absens, malum) */
    imprimere("\n--- Concordia: casus propriae ---\n");
    {
        hic_manens constans character* constans FORMAE_PROPRIAE[] = {
            "{ ; }\n",
            "{ }\n",
            "( )\n",
            "if a; then fi\n",
            "if a; then b; elif; then c; fi\n",
            "while; do a; done\n",
            "for i in a; do done\n",
            "case x in a) ;; esac\n",
            "{ a; \n",
            "cat <<A\nx\n",
            ")\n",
            "[[ a b ]]\n",
            "(( 1 2 ))\n",
            "$(( 1 + ))\n",
            "\"a\n",
            "`a\n"
        };
        Summa propriae;
          i32 n;

        memset(&propriae, ZEPHYRUM, magnitudo(propriae));
        per (n = ZEPHYRUM;
             n < (i32)(magnitudo(FORMAE_PROPRIAE)
                 / magnitudo(FORMAE_PROPRIAE[0]));
             n++)
        {
            character titulus[64];

            sprintf(titulus, "forma propria %d", (integer)n);
            _documentum(piscina, titulus, FORMAE_PROPRIAE[n],
                (i32)strlen(FORMAE_PROPRIAE[n]), &propriae);
        }
        _summam_imprimere("propriae", &propriae);
        /* formae ipsae diagnostica gignant: porta quae nihil videt
         * mortua est */
        CREDO_VERUM (propriae.vacuae_derivatae > ZEPHYRUM);
        CREDO_VERUM (propriae.clausurae_derivatae > ZEPHYRUM);
        CREDO_VERUM (propriae.mala_derivata > ZEPHYRUM);
        CREDO_VERUM (propriae.monita > ZEPHYRUM);
    }

    imprimere("\n--- Concordia: corpus domus ---\n");
    lista = crusta_oraculum_legere(piscina, radix,
        "build/crusta_corpus.lst", &mensura);
    si (lista == NIHIL)
    {
        CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam "
            "scribit");
    }
    alioquin
    {
        i32 ab = ZEPHYRUM;

        dum (ab < mensura)
        {
                  i32  ad = ab;
            character  via[1024];
              Piscina* propria;
            character* textus;
                  i32  longitudo = ZEPHYRUM;

            dum (ad < mensura && lista[ad] != '\n')
            {
                ad++;
            }
            si (ad == ab || ad - ab >= (i32)magnitudo(via))
            {
                ab = ad + I;
                perge;
            }
            memcpy(via, lista + ab, (size_t)(ad - ab));
            via[ad - ab]  = '\0';
            ab            = ad + I;
            si (   strcmp(via, "probationes/fixa/crusta/pathologiae.sh")
                    == ZEPHYRUM
                || strcmp(via, "probationes/fixa/crusta/adversarius.sh")
                    == ZEPHYRUM)
            {
                perge;
            }
            propria = piscina_generare_dynamicum("crusta_diagnostica",
                4194304);
            si (propria == NIHIL)
            {
                CREDO_CULPA ("piscina plagulae deficit");
                perge;
            }
            textus = crusta_oraculum_legere(propria, radix, via,
                &longitudo);
            si (textus != NIHIL)
            {
                _documentum(propria, via, textus, longitudo, &domus);
            }
            piscina_destruere(propria);
        }
    }
    _summam_imprimere("domus", &domus);
    CREDO_VERUM (domus.documenta >= (i32)DOMUS_MINIMUM);
    CREDO_AEQUALIS_I32 (domus.errata, (i32)DOMUS_ERRATA);
    CREDO_AEQUALIS_I32 (domus.monita, (i32)DOMUS_MONITA);

    imprimere("\n--- Concordia: casus oraculi ---\n");
    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    CREDO_NON_NIHIL (collectio);
    per (k = ZEPHYRUM; collectio != NIHIL && k < xar_numerus(collectio);
         k++)
    {
        constans CrustaCasusOraculi* casus_oraculi =
            (constans CrustaCasusOraculi*)xar_obtinere(collectio, k);
        character titulus[256];

        sprintf(titulus, "%.*s",
            (integer)(casus_oraculi->clavis.mensura < (i32)CCL
                ? casus_oraculi->clavis.mensura : (i32)CCL),
            (constans character*)casus_oraculi->clavis.datum);
        _documentum(piscina, titulus,
            (constans character*)casus_oraculi->datum.datum,
            casus_oraculi->datum.mensura, &oraculi);
    }
    _summam_imprimere("oraculi", &oraculi);
    CREDO_VERUM (oraculi.documenta >= (i32)ORACULI_MINIMUM);

    imprimere("\n--- Concordia: adversarius per casus ---\n");
    {
        character* fons = crusta_oraculum_legere(piscina, radix,
            "probationes/fixa/crusta/adversarius.sh", &mensura);
              Xar* exempla = fons != NIHIL
                  ? crusta_exempla_legere(piscina, fons,
                  mensura) : NIHIL;

        CREDO_NON_NIHIL (exempla);
        per (k = ZEPHYRUM; exempla != NIHIL && k < xar_numerus(exempla);
             k++)
        {
            constans CrustaExemplum* exemplum =
                (constans CrustaExemplum*)xar_obtinere(exempla, k);
            character titulus[256];

            sprintf(titulus, "adversarius %d",
                (integer)exemplum->numerus);
            _documentum(piscina, titulus,
                (constans character*)exemplum->datum.datum,
                exemplum->datum.mensura, &adversarii);
        }
    }
    _summam_imprimere("adversarii", &adversarii);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
