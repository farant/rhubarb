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

/* ORACULUM SEPARANS: parsare -> scribere_fontem, SINE STML.
 * Respondet 'utrum vitium nostrum an silvae sit'. VERUM = silva
 * sola formam tractat, ergo quidquid circuitus amittit NOSTRUM est. */
interior b32
_circuitus_directus (
               Piscina* piscina,
    constans character* fons)
{
    SilvaParsura*  origo;
    SilvaScriptura emissio;
    i32            mensura;

    mensura = (i32)strlen(fons);
    origo = silva_parsare(piscina, "probatio.c", fons, mensura,
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        redde FALSUM;
    }
    emissio = silva_scribere_fontem(piscina, origo,
        &SILVA_C89_REGISTRUM, origo->fons_princeps);
    si (!emissio.successus)
    {
        redde FALSUM;
    }
    redde (b32)(   emissio.textus.mensura == mensura
                && memcmp(emissio.textus.datum, fons,
                       (memoriae_index)mensura) == ZEPHYRUM);
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
     * PROBARE: regiones conditionales - OMNES rami servantur
     *
     * Ramus non sumptus lexemata CRUDA fert (lexata, numquam
     * parsata), ergo codicem qui parsari NON posset ferre potest -
     * et octeti eius nihilominus supervivere DEBENT.
     * ======================================================== */

    {
        constans character* causa;
              SilvaParsura* lecta;
              SilvaParsura* origo_regionis;
        constans character* fons_regionis =
            "#if 0\nint mortuus = ;\n#else\nint n = 0;\n#endif\n";

        /* CUSTODIA CONTRA PROBATIONEM VACUAM: si silva regionem
         * omnino non creat, circuitus octetim exactus transiret ET
         * NIHIL DE REGIONIBUS PROBARET. Ergo PRIMUM asserimus
         * regionem vere exsistere - aliter viriditas nihil dicit. */
        origo_regionis = silva_parsare(piscina, "regio.c",
            fons_regionis, (i32)strlen(fons_regionis),
            &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (origo_regionis);
        CREDO_NON_NIHIL (origo_regionis->expansio);
        CREDO_VERUM (xar_numerus(
            origo_regionis->expansio->regiones) > ZEPHYRUM);
        imprimere("  REGIONES textae=%d omissae=%d supremae=%d\n",
            (integer)origo_regionis->regiones_textae,
            (integer)origo_regionis->regiones_omissae,
            (integer)xar_numerus(
                origo_regionis->expansio->regiones));

        CREDO_VERUM (_circuitus_cum(piscina, fons_regionis,
            &causa, &lecta));
        si (causa != NIHIL)
        {
            imprimere("  causa (regio non sumpta): %s\n", causa);
        }
    }

    {
        constans character* causa;
              SilvaParsura* lecta;

        CREDO_VERUM (_circuitus_cum(piscina,
            "#ifdef IGNOTUM\nint x = 1;\n#endif\nint n = 0;\n",
            &causa, &lecta));
        si (causa != NIHIL)
        {
            imprimere("  causa (ifdef ignotum): %s\n", causa);
        }
    }


    /* ========================================================
     * REGIONES TEXTAE vs DEGRADATAE - limes MENSURATUS
     *
     * Regio TEXTA lineas suas EX ARBORE emittit (dominus unus),
     * ergo semita nodorum M1 eam iam integre tractat. Regio
     * DEGRADATA (limes sententiae transgressus, fontes mixti,
     * elementum opacum) eas ut REINSERENDA possidet, et illa sola
     * opus T3 verum est.
     *
     * Formae infra limitem PINNANT. Cum T3 regiones degradatas
     * impleverit, formae I et II VIRIDES fient et haec probatio
     * CADET - quod consilium est: pinna transitum cogit
     * annuntiari, non tacite fieri.
     * ======================================================== */

    {
        hic_manens constans character* FORMAE[] = {
            "int a = 1;\n#if 0\nint b = ;\n#endif\nint c = 2;\n",
            "int x[] = {\n#if 0\n1,\n#endif\n2\n};\n",
            "int f(void)\n{\n#if 0\n}\nint g(void)\n{\n#endif\nreturn 0;\n}\n",
            "#if 0\nint incompletus =\n#endif\n3;\n",
            "#if 1\nint p = 1;\n#else\nint q = ;\n#endif\n"
        };
        /* VERUM = texta, FALSUM = degradata. Classificatio
         * FACTUM SILVAE est, non arboris - asseritur ne probatio
         * tacite formam mutet. */
        hic_manens constans b32 TEXTA_EXSPECTATA[] = {
            VERUM, FALSUM, FALSUM, VERUM, VERUM
        };
        i32 f;

        per (f = ZEPHYRUM; f < (i32)(magnitudo(FORMAE)
                 / magnitudo(FORMAE[0])); f++)
        {
                  SilvaParsura* pars;
            constans character* causa;

            pars = silva_parsare(piscina, "forma.c", FORMAE[f],
                (i32)strlen(FORMAE[f]), &SILVA_C89_GRAMMATICA,
                NIHIL, NIHIL, NIHIL);
            CREDO_NON_NIHIL (pars);
            CREDO_NON_NIHIL (pars->expansio);
            /* Regio vere exsistit - aliter probatio vacua esset */
            CREDO_VERUM (xar_numerus(pars->expansio->regiones)
                > ZEPHYRUM);
            /* Textura ut exspectata */
            CREDO_VERUM ((pars->regiones_textae > ZEPHYRUM)
                == (TEXTA_EXSPECTATA[f] != FALSUM));
            /* T3b: circuitus viridis pro OMNIBUS formis - textis
             * per arborem, degradatis per laminas + cursorem
             * lacunarum conscium. Pinna prior ('viridis si et
             * solum si texta') hic CECIDIT, quod munus eius fuit:
             * transitum annuntiari coegit. */
            CREDO_VERUM (_circuitus(piscina, FORMAE[f], &causa));
            si (causa != NIHIL)
            {
                imprimere("  forma %d FRACTA: %s\n", (integer)f,
                    causa);
            }
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
        /* sine '>' - collapsus '<fontes(>' aut blocus '<fontes>'
         * pro latitudine viae fontis */
        CREDO_CHORDA_CONTINET (scriptura.textus,
            chorda_ex_literis("<fontes", piscina));

        /* INVOLUCRUM ancoram non fert: plagula ipsa initium est
         * (spec §1). Liberi SUPREMI tamen ancoras FERUNT, et
         * necessario - contentum non-arboreum (laminae regionum
         * degradatarum) intra spatium octetorum nodi iacere potest,
         * ergo ordo documenti ordinem octetorum exprimere nequit.
         * Correctio spec §1, implementatione inventa (T3b). */
        {
            chorda prima_linea;
               i32 finis;

            finis = ZEPHYRUM;
            dum (   finis < scriptura.textus.mensura
                 && scriptura.textus.datum[finis] != '\n')
            {
                finis++;
            }
            prima_linea.datum    = scriptura.textus.datum;
            prima_linea.mensura  = finis;
            CREDO_FALSUM (chorda_continet(prima_linea,
                chorda_ex_literis(" b=\"", piscina)));
        }
        /* Liberi ancoras ferunt - id quod circuitum verum reddit */
        CREDO_CHORDA_CONTINET (scriptura.textus,
            chorda_ex_literis(" b=\"", piscina));

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


    /* ========================================================
     * PROBARE: comparator parsurarum (T4)
     *
     * DIAGNOSIS, non verdictum. Assertio PRIMA est aequalitas -
     * sine ea probationes vitiorum plantatorum vacuae essent
     * (comparator qui semper 'inaequales' dicit omnia 'caperet').
     * ======================================================== */

    {
        constans character* fons_t4 =
            "#include \"a.h\"\nint a = 1;\n#if 0\nint m = ;\n"
            "#endif\nint b = 2;\n";
              SilvaParsura* origo;
              SilvaParsura* lecta;
       SilvaArborScriptura  scriptura;
          SilvaArborVitium  vitium;
     SilvaArborDifferentia  differentia;

        origo = silva_parsare(piscina, "t4.c", fons_t4,
            (i32)strlen(fons_t4), &SILVA_C89_GRAMMATICA, NIHIL,
            NIHIL, NIHIL);
        CREDO_NON_NIHIL (origo);

        scriptura = silva_arbor_scribere_parsuram(piscina, origo,
            &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
            NIHIL);
        CREDO_VERUM (scriptura.successus);

        lecta = silva_arbor_legere_parsuram(piscina, NIHIL,
            scriptura.textus, &SILVA_C89_REGISTRUM, "c89", &vitium);
        CREDO_NON_NIHIL (lecta);

        si (lecta != NIHIL)
        {
            /* AEQUALITAS - fundamentum omnium sequentium */
            CREDO_VERUM (silva_arbor_parsurae_aequales(origo, lecta,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            si (differentia.campus != NIHIL)
            {
                imprimere("  T4 divergentia inexspectata: %s"
                    " (via %s, index %d)\n", differentia.campus,
                    differentia.via, (integer)differentia.index);
            }

            /* VITIUM PLANTATUM I: lexema directivae mutatum.
             * INTERIUS plantatum (lexema II, non primum) - vitium
             * ad limitem plantatum tacere potest si emendatio
             * limitem eximat (lectio M1 T4). */
            {
                       Xar* lamina;
                SilvaToken* lexema;
                    chorda  servata;

                lamina = *(Xar**)xar_obtinere(lecta->directivae,
                    ZEPHYRUM);
                CREDO_VERUM (xar_numerus(lamina) > I);
                lexema   = *(SilvaToken**)xar_obtinere(lamina, I);
                servata  = lexema->valor;
                lexema->valor = chorda_ex_literis("MUTATUM",
                    piscina);
                CREDO_FALSUM (silva_arbor_parsurae_aequales(origo,
                    lecta, SILVA_ARBOR_COMPARATIO_FIDELITAS,
                    &differentia));
                CREDO_VERUM (differentia.lexema_a != NIHIL);
                lexema->valor = servata;
            }

            /* VITIUM PLANTATUM II: numerus directivarum */
            {
                Xar** sedes;
                Xar*  servata;

                sedes = (Xar**)xar_obtinere(lecta->directivae,
                    ZEPHYRUM);
                servata  = *sedes;
                *sedes   = NIHIL;
                CREDO_FALSUM (silva_arbor_parsurae_aequales(origo,
                    lecta, SILVA_ARBOR_COMPARATIO_FIDELITAS,
                    &differentia));
                *sedes = servata;
            }

            /* VITIUM PLANTATUM III: cauda (trivia caudae campus
             * est qui tacite cadere solet) */
            {
                SilvaToken* servatum;

                servatum             = lecta->lexema_finis;
                lecta->lexema_finis  = NIHIL;
                CREDO_FALSUM (silva_arbor_parsurae_aequales(origo,
                    lecta, SILVA_ARBOR_COMPARATIO_FIDELITAS,
                    &differentia));
                lecta->lexema_finis = servatum;
            }

            /* RESTITUTIO probata: post omnia vitia sanata parsurae
             * iterum aequales. Sine hac assertione vitium plantatum
             * non restitutum sequentia tacite inquinaret. */
            CREDO_VERUM (silva_arbor_parsurae_aequales(origo, lecta,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }
    }

    /* T4 pars II: campi qui in forma T4-I absunt - lamina regionis
     * DEGRADATAE, et delegatio per nodum supremum (index). */

    {
        constans character* fons_t4b =
            "int x[] = {\n#if 0\n1,\n#endif\n2\n};\nint y = 3;\n";
              SilvaParsura* origo;
              SilvaParsura* lecta;
       SilvaArborScriptura  scriptura;
          SilvaArborVitium  vitium;
     SilvaArborDifferentia  differentia;

        origo = silva_parsare(piscina, "t4b.c", fons_t4b,
            (i32)strlen(fons_t4b), &SILVA_C89_GRAMMATICA, NIHIL,
            NIHIL, NIHIL);
        CREDO_NON_NIHIL (origo);
        /* Regio vere DEGRADATA - aliter vitium infra vacuum esset */
        CREDO_VERUM (origo->regiones_omissae > ZEPHYRUM);

        scriptura = silva_arbor_scribere_parsuram(piscina, origo,
            &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
            NIHIL);
        CREDO_VERUM (scriptura.successus);
        lecta = silva_arbor_legere_parsuram(piscina, NIHIL,
            scriptura.textus, &SILVA_C89_REGISTRUM, "c89", &vitium);
        CREDO_NON_NIHIL (lecta);

        si (lecta != NIHIL && lecta->expansio != NIHIL)
        {
            CREDO_VERUM (silva_arbor_parsurae_aequales(origo, lecta,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
            si (differentia.campus != NIHIL)
            {
                imprimere("  T4b divergentia inexspectata: %s\n",
                    differentia.campus);
            }

            /* VITIUM: lamina cruda regionis degradatae */
            si (xar_numerus(lecta->expansio->regiones) > ZEPHYRUM)
            {
                SilvaRegio* regio;
                SilvaRamus* ramus;

                regio = *(SilvaRegio**)xar_obtinere(
                    lecta->expansio->regiones, ZEPHYRUM);
                CREDO_NON_NIHIL (regio);
                CREDO_VERUM (xar_numerus(regio->rami) > ZEPHYRUM);
                ramus = *(SilvaRamus**)xar_obtinere(regio->rami,
                    ZEPHYRUM);
                CREDO_NON_NIHIL (ramus);
                si (ramus != NIHIL && ramus->lexemata_cruda != NIHIL)
                {
                    Xar* servata;

                    servata                = ramus->lexemata_cruda;
                    ramus->lexemata_cruda  = NIHIL;
                    CREDO_FALSUM (silva_arbor_parsurae_aequales(
                        origo, lecta,
                        SILVA_ARBOR_COMPARATIO_FIDELITAS,
                        &differentia));
                    ramus->lexemata_cruda = servata;
                }
            }

            /* VITIUM: nodus supremus SECUNDUS ('int y = 3;') -
             * index in differentia probat delegationem per nodum
             * rectam esse, non solum primum conferri */
            si (silva_valor_lista_numerus(lecta->commissio->radix)
                    > I)
            {
                SilvaValor* elementum;
                SilvaNodus* nodus;

                elementum = silva_valor_lista_obtinere(
                    lecta->commissio->radix, I);
                CREDO_NON_NIHIL (elementum);
                nodus = elementum->datum.nodus;
                CREDO_NON_NIHIL (nodus);
                si (nodus != NIHIL)
                {
                    s32 servatum;

                    servatum      = nodus->genus;
                    nodus->genus  = servatum + I;
                    CREDO_FALSUM (silva_arbor_parsurae_aequales(
                        origo, lecta,
                        SILVA_ARBOR_COMPARATIO_FIDELITAS,
                        &differentia));
                    CREDO_AEQUALIS_S32 (differentia.index, I);
                    nodus->genus = servatum;
                }
            }

            CREDO_VERUM (silva_arbor_parsurae_aequales(origo, lecta,
                SILVA_ARBOR_COMPARATIO_FIDELITAS, &differentia));
        }
    }

    /* ==============================================================
     * T7b - CASUS ADVERSARII: extenta invocationum et lacunae
     *
     * Corpus quod habemus macra ea sola exercet quibus 'lib' utitur.
     * Extentum invocationis (sextum clausurae elementum) horis
     * paucis natum est; formae quas corpus NON fert hic quaeruntur
     * CONSULTO. Quaeque forma circuitum plenum petit: octeti in,
     * octeti idem out.
     *
     * NOTA: parsura NUDA macra in ipsa plagula definita EXPANDIT
     * (id est quod portam apparatus prope fefellit - vide
     * probatio_silva_arbor_plagula). Ergo clausura hic non opus est.
     * ============================================================== */
    {
        nomen structura {
            constans character* titulus;
            constans character* fons;
                         b32    debet_transire;
            constans character* causa_nota;
        } CasusAdversarius;

        constans CasusAdversarius CASUS[] = {
            /* --- invocationes functio-similes --- */
            { "functio-similis simplex",
              "#define F(a) (a)\nint n = F(1);\n", VERUM, NIHIL },
            { "argumenta duo",
              "#define F(a,b) ((a)+(b))\nint n = F(1, 2);\n", VERUM, NIHIL },
            { "sine argumentis",
              "#define F() 0\nint n = F();\n", VERUM, NIHIL },
            { "comma intra parentheses",
              "#define F(a) (a)\nint n = F((1,2));\n", VERUM, NIHIL },
            { "invocatio multi-linearis",
              "#define F(a,b) ((a)+(b))\nint n = F(1,\n    2);\n", VERUM, NIHIL },
            { "commentum intra argumenta",
              "#define F(a) (a)\nint n = F(/* x */ 1);\n", VERUM, NIHIL },
            { "spatia intra argumenta",
              "#define F(a) (a)\nint n = F( 1 );\n", VERUM, NIHIL },
            { "duae invocationes una linea",
              "#define F(a) (a)\nint n = F(1) + F(2);\n", VERUM, NIHIL },
            { "invocatio intra argumentum sui",
              "#define F(a) (a)\nint n = F(F(1));\n", VERUM, NIHIL },

            /* --- nidificatio --- */
            { "nidus duorum graduum",
              "#define G(a) ((a)*2)\n#define F(a) G(a)\nint n = F(3);\n", VERUM, NIHIL },
            { "nidus trium graduum",
              "#define H(a) ((a)+1)\n#define G(a) H(a)\n"
              "#define F(a) G(a)\nint n = F(3);\n", VERUM, NIHIL },
            { "obiectum-simile intra functio-similem",
              "#define X 5\n#define F(a) ((a)+X)\nint n = F(1);\n", VERUM, NIHIL },
            { "obiectum-simile ad invocationem expansum",
              "#define G(a) (a)\n#define F G(1)\nint n = F;\n", VERUM, NIHIL },

            /* --- expansio vacua --- */
            { "expansio vacua",
              "#define V(a)\nV(x)\nint n = 0;\n", VERUM, NIHIL },
            { "expansio vacua obiectum-similis",
              "#define W\nW\nint n = 0;\n", VERUM, NIHIL },
            { "expansio vacua bis eadem linea",
              "#define V(a)\nint n = 0; V(x) V(y)\n", VERUM, NIHIL },

            /* --- arma originis PASTA et CHORDA --- */
            { "stringificatio",
              "#define S(a) #a\nconstans char* s = S(salve);\n", FALSUM,
              "NOSTRUM: extentum pro origine CHORDA non scribitur "
              "(silva sola TRANSIT - vide _extentum_continens)" },
            { "pasta",
              "#define P(a,b) a##b\nint ab = 0;\nint n = P(a,b);\n", VERUM, NIHIL },
            { "pasta parente ex CORPORE",
              "#define C(a) pre##a\nint prex = 0;\nint n = C(x);\n",
              VERUM, NIHIL },

            /* --- lacunae et regiones --- */
            { "directiva intra regionem",
              "#ifndef X\n#define X 1\n#endif\nint n = 0;\n", VERUM, NIHIL },
            { "directiva cum commento multi-lineari intra regionem",
              "#ifndef X\n#define X 1 /* prima\n              * altera */\n"
              "#endif\nint n = 0;\n", VERUM, NIHIL },
            { "regio intra corpus functionis",
              "int f(void)\n{\n#ifdef A\n    return 1;\n#else\n"
              "    return 2;\n#endif\n}\n", VERUM, NIHIL },
            { "regiones duae",
              "#ifndef A\n#define A 1\n#endif\nint n = 0;\n"
              "#ifndef B\n#define B 2\n#endif\nint m = 1;\n", VERUM, NIHIL },
            { "macrum et regio miscentur",
              "#define F(a) (a)\n#ifndef X\n#define X 1\n#endif\n"
              "int n = F(X);\n", VERUM, NIHIL }
        };

        constans i32 NUMERUS_CASUUM =
            (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
        i32 c;
        i32 fracti;

        imprimere("\n--- T7b: casus adversarii (%d) ---\n",
            (integer)NUMERUS_CASUUM);
        fracti = ZEPHYRUM;
        per (c = ZEPHYRUM; c < NUMERUS_CASUUM; c++)
        {
            constans character* causa;
                           b32  transiit;

            transiit = _circuitus(piscina, CASUS[c].fons, &causa);
            si (transiit == CASUS[c].debet_transire)
            {
                perge;
            }
            fracti++;
            si (!transiit)
            {
                /* REGRESSUS: forma quae transire DEBEBAT fracta est */
                imprimere("  REGRESSUS [%s]: %s  {silva sola: %s}\n",
                    CASUS[c].titulus, causa ? causa : "?",
                    _circuitus_directus(piscina, CASUS[c].fons)
                        ? "TRANSIT - vitium NOSTRUM"
                        : "FRANGITUR - vitium SILVAE");
            }
            alioquin
            {
                /* SANATUM: limes notus non iam viget. Hoc quoque
                 * CLAMARE debet - aliter pinna limitem mortuum
                 * perpetuo custodiret et nemo sciret. */
                imprimere("  SANATUM [%s]: limes notus solutus est"
                    " - pinnam et notam renovare\n"
                    "    (nota vetus: %s)\n",
                    CASUS[c].titulus,
                    CASUS[c].causa_nota ? CASUS[c].causa_nota : "?");
            }
        }
        imprimere("  discrepantiae ab expectatione: %d / %d\n",
            (integer)fracti, (integer)NUMERUS_CASUUM);

        /* LIMITES NOTI (III), quisque causa NOMINATA in tabula:
         *   expansio vacua  - SILVA
         *   stringificatio  - NOSTRUM (silva sola transit)
         *   pasta           - SILVA, deferral nominatum
         * Pinna DISCREPANTIAS numerat, non fracturas: sic et
         * regressus et SANATIO clamant. */
        CREDO_AEQUALIS_I32 (fracti, ZEPHYRUM);
    }

    /* --- T7c: templa macronea (stml macros v1) - scriptor parsurae
     * formam communem '<post><lex-spatia n="N"/></post>' ut
     * fragmentum templi '#@post-spatia' auctorat: definitio UNA
     * post <fontes>, vocationes ad sedes. Onerator expandit
     * (visio contenti), ergo circuitus C-octetorum tenet.
     * Cave iuncturam tituli-attributi (doctrina M2b): definitio
     * attributa fert, ergo nomen SOLUM asseritur; vocatio
     * transclusio est (valor verbatim), tota asseritur. --- */
    {
        SilvaParsura*        origo;
        SilvaArborScriptura  scriptura;
        constans character*  causa;
        constans character*  fons = "int x = 1;\n";

        imprimere("\n--- T7c: templa macronea (post-spatia) ---\n");
        origo = silva_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), &SILVA_C89_GRAMMATICA, NIHIL, NIHIL,
            NIHIL);
        CREDO_NON_NIHIL (origo);
        si (origo != NIHIL)
        {
            scriptura = silva_arbor_scribere_parsuram(piscina, origo,
                &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
                NIHIL);
            CREDO_VERUM (scriptura.successus);
            si (scriptura.successus)
            {
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<#@post-spatia", piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<<#@post-spatia n=\"1\">>",
                        piscina));
            }
        }
        causa = NIHIL;
        CREDO_VERUM (_circuitus(piscina, fons, &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }

    /* --- T7c-b: templum ante-spatia (speculum post-spatia) -
     * indentatio: post lineam novam possidet, residuum spatii
     * ante lexema sequens cadit ('<ante><lex-spatia n="4"/>'). --- */
    {
        SilvaParsura*        origo;
        SilvaArborScriptura  scriptura;
        constans character*  causa;
        constans character*  fons =
            "int f(void)\n{\n    int x;\n    return x;\n}\n";

        imprimere("\n--- T7c-b: templum ante-spatia ---\n");
        origo = silva_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), &SILVA_C89_GRAMMATICA, NIHIL, NIHIL,
            NIHIL);
        CREDO_NON_NIHIL (origo);
        si (origo != NIHIL)
        {
            scriptura = silva_arbor_scribere_parsuram(piscina, origo,
                &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
                NIHIL);
            CREDO_VERUM (scriptura.successus);
            si (scriptura.successus)
            {
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<#@ante-spatia", piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<<#@ante-spatia n=\"4\">>",
                        piscina));
            }
        }
        causa = NIHIL;
        CREDO_VERUM (_circuitus(piscina, fons, &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }

    /* --- T7d: folia macronum - folium lexematis expansionem
     * ferens quod BIS identice apparet (positiones ex sede
     * DEFINITIONIS - ergo octetim aequale) in definitionem
     * '<#@m-<macro>>' capitis levatur, sedes vocationes fiunt.
     * Compressio ex contento derivata (independens invocationis). --- */
    {
        SilvaParsura*        origo;
        SilvaArborScriptura  scriptura;
        constans character*  causa;
        constans character*  fons =
            "#define N 0\nint a = N;\nint b = N;\n";

        imprimere("\n--- T7d: folia macronum (m-fragmenta) ---\n");
        origo = silva_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), &SILVA_C89_GRAMMATICA, NIHIL, NIHIL,
            NIHIL);
        CREDO_NON_NIHIL (origo);
        si (origo != NIHIL)
        {
            scriptura = silva_arbor_scribere_parsuram(piscina, origo,
                &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
                NIHIL);
            CREDO_VERUM (scriptura.successus);
            si (scriptura.successus)
            {
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<#@m-N", piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<<#@m-N>>", piscina));
            }
        }
        causa = NIHIL;
        CREDO_VERUM (_circuitus(piscina, fons, &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
    }

    /* --- T7e: familia parametrorum - parametrum formae summae
     * (monstrator: 'T* x') in vocationem templi cum argumentis
     * BLOCI comprimitur: structura (involucra specificatores/
     * declarator/tok) in definitionem capitis semel, contenta tok
     * in argumenta subarborea (par. 6.1 - axis participationis
     * lexN in argumentum solvitur). Sceletum ambulans v2:
     * congruentia = definitio retro currens. --- */
    {
        SilvaParsura*        origo;
        SilvaArborScriptura  scriptura;
        constans character*  causa;
        constans character*  fons =
            "typedef int T;\nint f(T* x)\n{\n    return *x;\n}\n";

        imprimere("\n--- T7e: familia parametrorum ---\n");
        origo = silva_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), &SILVA_C89_GRAMMATICA, NIHIL, NIHIL,
            NIHIL);
        CREDO_NON_NIHIL (origo);
        si (origo != NIHIL)
        {
            scriptura = silva_arbor_scribere_parsuram(piscina, origo,
                &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps,
                NIHIL);
            CREDO_VERUM (scriptura.successus);
            si (scriptura.successus)
            {
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<#@par-monstratum",
                        piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<<#@par-monstratum>>",
                        piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<@typus=", piscina));
                CREDO_CHORDA_CONTINET (scriptura.textus,
                    chorda_ex_literis("<@nomen=", piscina));
            }
        }
        causa = NIHIL;
        CREDO_VERUM (_circuitus(piscina, fons, &causa));
        si (causa != NIHIL)
        {
            imprimere("  causa: %s\n", causa);
        }
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
