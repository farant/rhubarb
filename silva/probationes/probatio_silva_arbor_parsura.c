/* probatio_silva_arbor_parsura.c - forma <parsura> (M2 §2)
 *
 * Probat documentum quod plagulam INTEGRAM fert, non subarborem.
 *
 * ORACULUM: octeti contra FONTEM ORIGINALEM. Hoc fortius est quam
 * porta M1 quia valor expectatus EXTRA silvam iacet - vitium
 * symmetricum inter scriptorem et lectorem silvae in eo latere
 * NEQUIT. Circuitus: parsare -> scribere <parsura> -> legere ->
 * silva_scribere_fontem -> conferre cum fonte.
 *
 * Consilium: project-specs/arbor-parsura-spec.md §6.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Circuitum plenum unius fontis currere; VERUM si octeti congruunt.
 * causa_out (si non NIHIL) causam fracturae nominat. */
interior b32
_circuitus_cum (
               Piscina*  piscina,
    constans character*  fons,
    constans character** causa_out,
          SilvaParsura** lecta_out)
{
         SilvaParsura* origo;
         SilvaParsura* lecta;
  SilvaArborScriptura  scriptura;
     SilvaArborVitium  vitium;
       SilvaScriptura  emissio;
                  i32  mensura;

    *causa_out = NIHIL;
    si (lecta_out != NIHIL)
    {
        *lecta_out = NIHIL;
    }
    mensura = (i32)strlen(fons);

    origo = silva_parsare(piscina, "probatio.c", fons, mensura,
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        *causa_out = "parsura originalis fracta";
        redde FALSUM;
    }

    scriptura = silva_arbor_scribere_parsuram(piscina, origo,
        &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        *causa_out = scriptura.causa ? scriptura.causa
                                     : "scriptura fracta";
        redde FALSUM;
    }

    lecta = silva_arbor_legere_parsuram(piscina, NIHIL,
        scriptura.textus, &SILVA_C89_REGISTRUM, "c89", &vitium);
    si (lecta == NIHIL)
    {
        *causa_out = vitium.causa ? vitium.causa : "lectio fracta";
        redde FALSUM;
    }

    si (lecta_out != NIHIL)
    {
        *lecta_out = lecta;
    }

    emissio = silva_scribere_fontem(piscina, lecta,
        &SILVA_C89_REGISTRUM, lecta->fons_princeps);
    si (!emissio.successus)
    {
        *causa_out = emissio.causa ? emissio.causa
                                   : "emissio fracta";
        redde FALSUM;
    }

    si (   emissio.textus.mensura != mensura
        || memcmp(emissio.textus.datum, fons,
               (memoriae_index)mensura) != ZEPHYRUM)
    {
        *causa_out = "octeti divergunt";
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_circuitus (
               Piscina*  piscina,
    constans character*  fons,
    constans character** causa_out)
{
    redde _circuitus_cum(piscina, fons, causa_out, NIHIL);
}

/* Chordam substituere (pro vitiis plantatis) */
interior chorda
_substituere (
               Piscina* piscina,
                chorda  textus,
    constans character* vetus,
    constans character* novus)
{
    chorda  fructus;
        i8* buffer;
       i32  vm;
       i32  nm;
       i32  i;
       i32  j;

    vm = (i32)strlen(vetus);
    nm = (i32)strlen(novus);
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura + (memoriae_index)nm + 16u);
    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    si (buffer == NIHIL)
    { redde fructus;
    }

    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (   i + (i32)vm <= textus.mensura
            && memcmp(&textus.datum[i], vetus,
                   (memoriae_index)vm) == ZEPHYRUM)
        {
            memcpy(&buffer[j], novus, (memoriae_index)nm);
            j += nm;
            i += vm - I;
            perge;
        }
        buffer[j++] = textus.datum[i];
    }
    fructus.datum    = buffer;
    fructus.mensura  = j;
    redde fructus;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_arbor_parsura",
        67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: circuitus minimus - declaratio sola
     *
     * Nulla directiva, nulla regio conditionalis: forma minima
     * quae involucrum + fontes + arborem + caudam exercet.
     * ======================================================== */

    {
        constans character* causa;

        CREDO_VERUM (_circuitus(piscina, "int n = 0;\n", &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }

    /* Plures nodi supremi: numeratio fragmentorum DOCUMENTO-scopata
     * est, ergo duo nodi identitates geminas ferre NON debent. */
    {
        constans character* causa;

        CREDO_VERUM (_circuitus(piscina,
            "int a = 1;\nint b = 2;\n", &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }

    /* Trivia caudae: linea vacua post ultimum lexema EOF portat. */
    {
        constans character* causa;

        CREDO_VERUM (_circuitus(piscina, "int n = 0;\n\n\n",
            &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }


    /* ========================================================
     * PROBARE: directivae ordine DOCUMENTI + RETRACTIO
     *
     * Octeti soli NON sufficiunt: circuitus octetim exactus
     * transiret etiam si directiva in ARBORE relicta esset. Ergo
     * structura quoque asseritur - arbor onerata directivam NON
     * continet, et parsura->directivae eam FERT.
     * ======================================================== */

    {
        constans character* causa;
              SilvaParsura* lecta;

        CREDO_VERUM (_circuitus_cum(piscina,
            "#include \"a.h\"\nint n = 0;\n", &causa, &lecta));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
        si (lecta != NIHIL && lecta->commissio != NIHIL)
        {
            /* RETRACTIO: radix nodum UNUM fert (declarationem),
             * directivam NON */
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                lecta->commissio->radix), I);
            CREDO_NON_NIHIL (lecta->directivae);
            CREDO_AEQUALIS_I32 (xar_numerus(lecta->directivae), I);
        }
    }

    /* Directiva INTER nodos: ordo documenti ordo plagulae est */
    {
        constans character* causa;
              SilvaParsura* lecta;

        CREDO_VERUM (_circuitus_cum(piscina,
            "int a = 1;\n#define X 1\nint b = 2;\n",
            &causa, &lecta));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
        si (lecta != NIHIL && lecta->commissio != NIHIL)
        {
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                lecta->commissio->radix), II);
            CREDO_AEQUALIS_I32 (xar_numerus(lecta->directivae), I);
        }
    }

    /* Directivae plures ante omnem nodum */
    {
        constans character* causa;
              SilvaParsura* lecta;

        CREDO_VERUM (_circuitus_cum(piscina,
            "#include \"a.h\"\n#include \"b.h\"\nint n = 0;\n",
            &causa, &lecta));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
        si (lecta != NIHIL)
        {
            CREDO_AEQUALIS_I32 (xar_numerus(lecta->directivae), II);
        }
    }


    /* ========================================================
     * PROBARE: sigillum - arbor vocabulario falso iudicata
     * mendacium est, ergo documentum mutatum RECUSATUR
     * ======================================================== */

    {
             SilvaParsura* origo;
      SilvaArborScriptura  scriptura;
         SilvaArborVitium  vitium;
                   chorda  mutata;

        origo = silva_parsare(piscina, "probatio.c", "int n = 0;\n",
            11, &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (origo);

        scriptura = silva_arbor_scribere_parsuram(piscina, origo,
            &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
            NIHIL);
        CREDO_VERUM (scriptura.successus);

        /* Involucrum <parsura> adest, non <arbor> */
        CREDO_CHORDA_CONTINET (scriptura.textus,
            chorda_ex_literis("<parsura", piscina));
        CREDO_CHORDA_CONTINET (scriptura.textus,
            chorda_ex_literis("<fontes>", piscina));

        /* NULLA ancora: plagula ipsa initium est (spec §1) */
        CREDO_FALSUM (chorda_continet(scriptura.textus,
            chorda_ex_literis(" b=\"", piscina)));

        /* grammatica falsa -> RECUSATIO */
        CREDO_NIHIL (silva_arbor_legere_parsuram(piscina, NIHIL,
            scriptura.textus, &SILVA_C89_REGISTRUM, "sceletum",
            &vitium));
        CREDO_VERUM (vitium.causa != NIHIL);

        /* sigillum mutatum uno nibble -> RECUSATIO */
        mutata = _substituere(piscina, scriptura.textus,
            "registrum-sigillum=\"", "registrum-sigillum=\"f");
        CREDO_NIHIL (silva_arbor_legere_parsuram(piscina, NIHIL,
            mutata, &SILVA_C89_REGISTRUM, "c89", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
