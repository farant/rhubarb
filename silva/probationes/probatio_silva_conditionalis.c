/* probatio_silva_conditionalis.c - Textura conditionalium (Phase 7
 * Chunk B, simulatio ⑦)
 *
 * Regiones conditionales in arborem texuntur ubi limites cum
 * sententiis congruunt: conditionalis {rami, finis}, ramus-sumptus
 * {directiva, contentum, conditio_id}, ramus-omissus {directiva,
 * cruda, conditio_id}. Detectio per extenta strati 0; transgressio =
 * degradatio (regio laminis possessa manet - mos pristinus). Dominus
 * unus etiam pro lineis (β): directivae fluxus lineas regionum NON
 * fert. Reconstructio octetim exacta UBIQUE - texta aut degradata.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

/* Genus ex registro per nomen; -1 si absens */
interior s32
_genus_invenire (constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < SILVA_SCELETUM_REGISTRUM.numerus_generum;
         i++)
    {
        constans SilvaTabGenus* genus =
            &SILVA_SCELETUM_REGISTRUM.genera[i];

        si (genus->titulus != NIHIL
            && strcmp(genus->titulus, titulus) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Locus generis per nomen; -1 si absens */
interior s32
_locum_invenire (s32 genus_index, constans character* titulus)
{
    constans SilvaTabGenus* genus;
    i32 k;

    si (genus_index < ZEPHYRUM)
    {
        redde -I;
    }
    genus = &SILVA_SCELETUM_REGISTRUM.genera[genus_index];
    per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
    {
        constans SilvaTabLocus* locus =
            &SILVA_SCELETUM_REGISTRUM.loci[genus->loci_offset + k];

        si (locus->titulus != NIHIL
            && strcmp(locus->titulus, titulus) == ZEPHYRUM)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* Parsare + reconstructio octetim exacta; parsura_out semper */
interior b32
_fidelis (Piscina* piscina, constans character* fons,
    SilvaParsura** parsura_out)
{
    SilvaParsura*  parsura;
    SilvaScriptura scriptura;
    i32 m = (i32)strlen(fons);

    parsura = silva_parsare(piscina, "probatio.c", fons, m,
        &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
    si (parsura_out != NIHIL)
    {
        *parsura_out = parsura;
    }
    si (parsura == NIHIL || !parsura->successus)
    {
        redde FALSUM;
    }
    scriptura = silva_scribere_fontem(piscina, parsura,
        &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
    si (!scriptura.successus || scriptura.textus.mensura != m)
    {
        redde FALSUM;
    }
    si (m > ZEPHYRUM
        && memcmp(scriptura.textus.datum, fons,
               (memoriae_index)m) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* Elementum radicis generis dati primum; NIHIL si absens */
interior SilvaNodus*
_elementum_generis (SilvaParsura* parsura, s32 genus)
{
    i32 n;
    i32 i;

    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    n = silva_valor_lista_numerus(parsura->commissio->radix);
    per (i = ZEPHYRUM; i < n; i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(
            parsura->commissio->radix, i);

        si (elem != NIHIL && elem->genus == SILVA_VALOR_NODUS
            && elem->datum.nodus->genus == genus)
        {
            redde elem->datum.nodus;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    s32 g_conditionalis;
    s32 g_sumptus;
    s32 g_omissus;
    s32 l_rami;
    s32 l_finis;
    s32 l_s_directiva;
    s32 l_s_contentum;
    s32 l_s_conditio;
    s32 l_o_directiva;
    s32 l_o_cruda;
    s32 l_o_conditio;

    piscina = piscina_generare_dynamicum("probatio_conditionalis",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: formae in registro cocto (generator eas imponit)
     * ======================================================== */
    imprimere("\n--- Probans formas generum ---\n");
    g_conditionalis = _genus_invenire("conditionalis");
    g_sumptus = _genus_invenire("ramus-sumptus");
    g_omissus = _genus_invenire("ramus-omissus");
    CREDO_VERUM (g_conditionalis >= ZEPHYRUM);
    CREDO_VERUM (g_sumptus >= ZEPHYRUM);
    CREDO_VERUM (g_omissus >= ZEPHYRUM);
    l_rami = _locum_invenire(g_conditionalis, "rami");
    l_finis = _locum_invenire(g_conditionalis, "finis");
    l_s_directiva = _locum_invenire(g_sumptus, "directiva");
    l_s_contentum = _locum_invenire(g_sumptus, "contentum");
    l_s_conditio = _locum_invenire(g_sumptus, "conditio_id");
    l_o_directiva = _locum_invenire(g_omissus, "directiva");
    l_o_cruda = _locum_invenire(g_omissus, "cruda");
    l_o_conditio = _locum_invenire(g_omissus, "conditio_id");
    CREDO_VERUM (l_rami >= ZEPHYRUM && l_finis >= ZEPHYRUM);
    CREDO_VERUM (l_s_directiva >= ZEPHYRUM && l_s_contentum >= ZEPHYRUM
        && l_s_conditio >= ZEPHYRUM);
    CREDO_VERUM (l_o_directiva >= ZEPHYRUM && l_o_cruda >= ZEPHYRUM
        && l_o_conditio >= ZEPHYRUM);

    /* ========================================================
     * PROBARE: duo rami - sumptus parsatum fert, omissus crudum;
     * finis lineam #endif; conditio_id iuncturam; dominus unus
     * (directivae fluxus lineas regionum NON fert)
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* conditionalis;

        imprimere("\n--- Probans texturam basicam ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\nint x;\n#else\nint y;\n#endif\nint z;\n",
            &parsura));
        CREDO_NON_NIHIL (parsura);
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            CREDO_AEQUALIS_I32 (parsura->regiones_omissae, ZEPHYRUM);
            /* radix: [conditionalis, declaratio z] */
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                parsura->commissio->radix), II);
            /* β: nullae lineae regionis in directivis */
            CREDO_AEQUALIS_I32 (xar_numerus(parsura->directivae),
                ZEPHYRUM);

            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            CREDO_NON_NIHIL (conditionalis);
            si (conditionalis != NIHIL)
            {
                SilvaValor rami = conditionalis->loci[l_rami];
                SilvaValor finis = conditionalis->loci[l_finis];
                SilvaValor* r0;
                SilvaValor* r1;

                CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(rami),
                    II);
                /* finis: lexemata lineae #endif */
                CREDO_VERUM (silva_valor_lista_numerus(finis) > ZEPHYRUM);

                r0 = silva_valor_lista_obtinere(rami, ZEPHYRUM);
                r1 = silva_valor_lista_obtinere(rami, I);
                CREDO_NON_NIHIL (r0);
                CREDO_NON_NIHIL (r1);
                si (r0 != NIHIL && r1 != NIHIL)
                {
                    SilvaNodus* sumptus = r0->datum.nodus;
                    SilvaNodus* omissus = r1->datum.nodus;

                    CREDO_AEQUALIS_I32 ((i32)sumptus->genus,
                        (i32)g_sumptus);
                    CREDO_AEQUALIS_I32 ((i32)omissus->genus,
                        (i32)g_omissus);
                    /* contentum: declaratio una (int x;) */
                    CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                        sumptus->loci[l_s_contentum]), I);
                    /* cruda: int y ; (tria lexemata) */
                    CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                        omissus->loci[l_o_cruda]), III);
                    /* conditio_id: iunctura in exp->rami (index+1,
                     * numquam 0) */
                    CREDO_VERUM (sumptus->loci[l_s_conditio]
                        .datum.index > ZEPHYRUM);
                    CREDO_VERUM (omissus->loci[l_o_conditio]
                        .datum.index > ZEPHYRUM);
                    CREDO_VERUM (sumptus->loci[l_s_conditio].datum.index
                        != omissus->loci[l_o_conditio].datum.index);
                    /* pater: commissio nodos textos ambulavit */
                    CREDO_VERUM (sumptus->pater == conditionalis);
                    CREDO_VERUM (omissus->pater == conditionalis);
                }
            }
        }
    }

    /* ========================================================
     * PROBARE: #else sumptus (#if 0 numquam) + ramus vacuus
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* conditionalis;

        imprimere("\n--- Probans else sumptum ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 0\nnumquam hoc;\n#else\nint y;\n#endif\n", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            CREDO_NON_NIHIL (conditionalis);
            si (conditionalis != NIHIL)
            {
                SilvaValor* r0 = silva_valor_lista_obtinere(
                    conditionalis->loci[l_rami], ZEPHYRUM);
                SilvaValor* r1 = silva_valor_lista_obtinere(
                    conditionalis->loci[l_rami], I);

                CREDO_VERUM (r0 != NIHIL && r0->datum.nodus->genus
                    == g_omissus);
                CREDO_VERUM (r1 != NIHIL && r1->datum.nodus->genus
                    == g_sumptus);
            }
        }

        /* ramus sumptus vacuus: contentum sine elementis */
        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\n#else\nint y;\n#endif\n", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            si (conditionalis != NIHIL)
            {
                SilvaValor* r0 = silva_valor_lista_obtinere(
                    conditionalis->loci[l_rami], ZEPHYRUM);

                CREDO_VERUM (r0 != NIHIL && r0->datum.nodus->genus
                    == g_sumptus);
                si (r0 != NIHIL)
                {
                    CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                        r0->datum.nodus->loci[l_s_contentum]),
                        ZEPHYRUM);
                }
            }
        }
    }

    /* ========================================================
     * PROBARE: nidificatio - conditionalis interior elementum
     * contenti exterioris
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* exterior;

        imprimere("\n--- Probans nidificationem ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\n#if 1\nint a;\n#endif\nint b;\n#endif\n",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, II);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                parsura->commissio->radix), I);
            exterior = _elementum_generis(parsura, g_conditionalis);
            CREDO_NON_NIHIL (exterior);
            si (exterior != NIHIL)
            {
                SilvaValor* r0 = silva_valor_lista_obtinere(
                    exterior->loci[l_rami], ZEPHYRUM);

                si (r0 != NIHIL)
                {
                    SilvaValor contentum =
                        r0->datum.nodus->loci[l_s_contentum];
                    SilvaValor* e0;

                    /* [conditionalis interior, declaratio b] */
                    CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                        contentum), II);
                    e0 = silva_valor_lista_obtinere(contentum,
                        ZEPHYRUM);
                    CREDO_VERUM (e0 != NIHIL
                        && e0->datum.nodus->genus == g_conditionalis);
                }
            }
        }
    }

    /* ========================================================
     * PROBARE: degradationes - regio intra sententiam; limes
     * transgressus. Reconstructio tenet UTRIMQUE (mos pristinus
     * est via regressus).
     * ======================================================== */
    {
        SilvaParsura* parsura;

        imprimere("\n--- Probans degradationes ---\n");

        /* regio intra sententiam: extentum sententiae limites
         * regionis transgreditur */
        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "int x =\n#if 1\n1;\n#else\n2;\n#endif\n", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (parsura->regiones_omissae, I);
        }

        /* sententia trans finem regionis */
        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\nint a; int\n#else\nint b; int\n#endif\nc;\n",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (parsura->regiones_omissae, I);
        }
    }

    /* ========================================================
     * PROBARE: regio definitionum solarum (nihil congregatum -
     * insertio ordinali; linea #define in directivis MANET,
     * intra conditionalem reinserta)
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* conditionalis;

        imprimere("\n--- Probans regionem definitionum ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 1\n#define Q 5\n#endif\nQ;\n", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            /* #define linea directiva ordinaria manet (non
             * structuralis) */
            CREDO_AEQUALIS_I32 (xar_numerus(parsura->directivae), I);
            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            CREDO_NON_NIHIL (conditionalis);
            /* Q; expansum extra regionem: sententia-expressionis */
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                parsura->commissio->radix), II);
        }
    }

    /* ========================================================
     * PROBARE: imperfecta (EOF ante #endif) - finis vacuum,
     * textura tenet
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* conditionalis;

        imprimere("\n--- Probans imperfectam ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina, "#if 1\nint x;\n", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            CREDO_NON_NIHIL (conditionalis);
            si (conditionalis != NIHIL)
            {
                /* finis: NIHIL valor (locus numquam positus) */
                CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                    conditionalis->loci[l_finis]), ZEPHYRUM);
            }
        }
    }

    /* ========================================================
     * PROBARE: includendum intra ramum - elementa plagulae alienae
     * EXTRA conditionalem manent; reconstructio AMBARUM plagularum
     * octetim exacta (uniformitas fontium, sim ⑦ C7)
     * ======================================================== */
    {
        SilvaExpansio* exp;
        SilvaParsura* parsura;
        SilvaScriptura scriptura;
        constans character* MODULUS = "int ex_modulo;\n";
        constans character* FONS =
            "#if 1\n#include \"modulus.h\"\nint x;\n#endif\n";
        s32 fons_moduli;

        imprimere("\n--- Probans includendum intra ramum ---\n");

        exp = silva_expansio_creare(piscina);
        fons_moduli = silva_includendum_praebere(exp, "modulus.h",
            MODULUS, (i32)strlen(MODULUS));
        CREDO_VERUM (fons_moduli >= ZEPHYRUM);

        parsura = silva_parsare_cum_expansione(piscina, exp,
            "probatio.c", FONS, (i32)strlen(FONS),
            &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (parsura);
        si (parsura != NIHIL)
        {
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            /* radix: [declaratio ex_modulo (aliena), conditionalis];
             * elementum alienum extra contentum */
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                parsura->commissio->radix), II);

            scriptura = silva_scribere_fontem(piscina, parsura,
                &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
            CREDO_VERUM (scriptura.successus);
            CREDO_AEQUALIS_I32 (scriptura.textus.mensura,
                (i32)strlen(FONS));
            CREDO_VERUM (memcmp(scriptura.textus.datum, FONS,
                strlen(FONS)) == ZEPHYRUM);

            scriptura = silva_scribere_fontem(piscina, parsura,
                &SILVA_SCELETUM_REGISTRUM, fons_moduli);
            CREDO_VERUM (scriptura.successus);
            CREDO_AEQUALIS_I32 (scriptura.textus.mensura,
                (i32)strlen(MODULUS));
            CREDO_VERUM (memcmp(scriptura.textus.datum, MODULUS,
                strlen(MODULUS)) == ZEPHYRUM);
        }
    }

    /* ========================================================
     * PROBARE: elif - rami tres, medius sumptus
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* conditionalis;

        imprimere("\n--- Probans elif ---\n");

        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#if 0\nint a;\n#elif 1\nint b;\n#else\nint c;\n#endif\n",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
            conditionalis = _elementum_generis(parsura,
                g_conditionalis);
            CREDO_NON_NIHIL (conditionalis);
            si (conditionalis != NIHIL)
            {
                SilvaValor rami = conditionalis->loci[l_rami];
                SilvaValor* r0 = silva_valor_lista_obtinere(rami,
                    ZEPHYRUM);
                SilvaValor* r1 = silva_valor_lista_obtinere(rami, I);
                SilvaValor* r2 = silva_valor_lista_obtinere(rami, II);

                CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(rami),
                    III);
                CREDO_VERUM (r0 != NIHIL && r0->datum.nodus->genus
                    == g_omissus);
                CREDO_VERUM (r1 != NIHIL && r1->datum.nodus->genus
                    == g_sumptus);
                CREDO_VERUM (r2 != NIHIL && r2->datum.nodus->genus
                    == g_omissus);
            }
        }
    }

    /* ========================================================
     * PROBARE: messis sepulcreti v1 (praeparator spot-check,
     * Phase 7 Chunk C): directivae INDENTATAE (initium_lineae,
     * non columna 1) + #endif sine linea nova ad EOF
     * ======================================================== */
    {
        SilvaParsura* parsura;

        imprimere("\n--- Probans messem v1 ---\n");

        /* directivae indentatae (v1 eas probavit; silva numquam):
         * nidificatio cum spatiis ante # */
        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina,
            "#ifdef EXTERIOR\n  #ifdef INTERIOR\n    int d;\n"
            "  #else\n    int e;\n  #endif\n#endif\n", &parsura));
        si (parsura != NIHIL)
        {
            /* exterior non sumptus (EXTERIOR indefinitum) - regio
             * una texta, interior intra crudum */
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
        }

        /* #endif sine linea nova ad EOF */
        parsura = NIHIL;
        CREDO_VERUM (_fidelis(piscina, "#if 1\nint a;\n#endif",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, I);
        }

        /* #if !defined(X) forma (v1 if-expressions) */
        CREDO_VERUM (_fidelis(piscina,
            "#if !defined(NUSQUAM)\nint p;\n#endif\n", NIHIL));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
