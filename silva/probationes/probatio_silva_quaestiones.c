/* probatio_silva_quaestiones.c - bibliotheca selectorum nominatorum
 * (lectio + validatio + substitutio + executio; classes fracturae
 * omnes nominatae; INTENTIO phase-log 2026-07-31, parcum
 * 01KXPV9FPK) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_quaestio.h"
#include "silva_quaestiones.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* culpa verbum continet? (quaestio simplex, sine casu) */
interior b32
_continet (chorda culpa, constans character* verbum)
{
    i32 mensura = (i32)strlen(verbum);
    i32 i;

    si (mensura == ZEPHYRUM) redde VERUM;
    si (culpa.mensura < mensura) redde FALSUM;
    per (i = ZEPHYRUM; i + mensura <= culpa.mensura; i++)
    {
        si (memcmp(culpa.datum + i, verbum, (size_t)mensura)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* legere quod deficere DEBET: NIHIL + culpa cum verbo */
interior b32
_fractura_exspectata (Piscina* piscina,
    constans character* fons_stml, constans character* verbum)
{
    chorda culpa;
    chorda fons = chorda_ex_literis(fons_stml, piscina);
    SilvaQuaestiones* bibliotheca = silva_quaestiones_legere(
        piscina, &SILVA_C89_REGISTRUM, NIHIL, fons, &culpa);

    si (bibliotheca != NIHIL) redde FALSUM;
    si (!_continet(culpa, verbum))
    {
        imprimere("  culpa inexspectata: %.*s (verbum '%s')\n",
            (integer)culpa.mensura,
            (constans character*)culpa.datum, verbum);
        redde FALSUM;
    }
    redde VERUM;
}

/* argumentum unum implere (series plana - forma API) */
interior vacuum
_argumentum (SilvaQuaestionesArgumentum* argumentum,
    Piscina* piscina, constans character* titulus,
    constans character* valor)
{
    argumentum->titulus = chorda_ex_literis(titulus, piscina);
    argumentum->valor = chorda_ex_literis(valor, piscina);
}

s32
principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_quaestiones",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: bibliotheca valida legitur
     * ======================================================== */
    {
        constans character* fons_stml =
            "<quaestiones>\n"
            "  <quaestio titulus=\"declarator-nominatus\""
            " gradus=\"oculi\">\n"
            "    <selector>declarator-titulus"
            "[tok_titulus=\"$titulus\"]</selector>\n"
            "    <parametrum titulus=\"titulus\"/>\n"
            "    <causa>declaratores nomine dato invenire</causa>\n"
            "  </quaestio>\n"
            "  <quaestio titulus=\"vocantes-continentes\""
            " gradus=\"oculi\">\n"
            "    <selector>definitio-functionis$def"
            ":vocat($functio)</selector>\n"
            "    <parametrum titulus=\"functio\"/>\n"
            "    <causa>functiones quae vocationem continent</causa>\n"
            "  </quaestio>\n"
            "  <quaestio titulus=\"redde-intra-redde\""
            " gradus=\"invarians\">\n"
            "    <selector>redde redde</selector>\n"
            "    <causa>structura impossibilis - probationis causa"
            "</causa>\n"
            "  </quaestio>\n"
            "</quaestiones>\n";
        chorda fons = chorda_ex_literis(fons_stml, piscina);
        chorda culpa;
        SilvaQuaestiones* bibliotheca;
        constans SilvaQuaestioNominata* nominata;

        imprimere("\n--- Probans lectionem validam ---\n");

        bibliotheca = silva_quaestiones_legere(piscina,
            &SILVA_C89_REGISTRUM, NIHIL, fons, &culpa);
        CREDO_NON_NIHIL (bibliotheca);
        CREDO_AEQUALIS_I32 (silva_quaestiones_numerus(bibliotheca),
            III);

        nominata = silva_quaestiones_invenire(bibliotheca,
            "vocantes-continentes");
        CREDO_NON_NIHIL (nominata);
        CREDO_AEQUALIS_S32 (nominata->gradus,
            (s32)SILVA_QUAESTIONES_OCULI);
        CREDO_AEQUALIS_I32 (xar_numerus(nominata->parametra), I);

        nominata = silva_quaestiones_invenire(bibliotheca,
            "redde-intra-redde");
        CREDO_NON_NIHIL (nominata);
        CREDO_AEQUALIS_S32 (nominata->gradus,
            (s32)SILVA_QUAESTIONES_INVARIANS);
        CREDO_VERUM (nominata->causa.mensura > ZEPHYRUM);

        CREDO_NIHIL (silva_quaestiones_invenire(bibliotheca,
            "nusquam"));

        /* ad_indicem fines */
        CREDO_NON_NIHIL (silva_quaestiones_ad_indicem(bibliotheca,
            ZEPHYRUM));
        CREDO_NIHIL (silva_quaestiones_ad_indicem(bibliotheca,
            III));

        /* ====================================================
         * PROBARE: substitutio + executio contra fixturam
         * ==================================================== */
        {
            constans character* fons_c =
                "int quadratum(int x) { return x * x; }\n"
                "int bis(int x) { return quadratum(x)"
                " + quadratum(x); }\n"
                "void nihil_facere(void) { return; }\n";
            SilvaParsura* parsura;
            SilvaQuaestionesArgumentum argumenta[II];
            SilvaQuaestio* quaestio;
            Xar* resultata;

            imprimere("\n--- Probans substitutionem/executionem"
                " ---\n");

            parsura = silva_c89_parsare(piscina, "probatio.c",
                fons_c, (i32)strlen(fons_c), NIHIL);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);

            /* contextus (a): valor attributi citatus */
            nominata = silva_quaestiones_invenire(bibliotheca,
                "declarator-nominatus");
            _argumentum(&argumenta[ZEPHYRUM], piscina, "titulus",
                "quadratum");
            quaestio = silva_quaestiones_parare(piscina,
                bibliotheca, nominata, argumenta, I, &culpa);
            CREDO_NON_NIHIL (quaestio);
            resultata = silva_quaestio_exsequi(quaestio,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);

            /* contextus (b): argumentum pseudo integrum + captura
             * intacta */
            nominata = silva_quaestiones_invenire(bibliotheca,
                "vocantes-continentes");
            _argumentum(&argumenta[ZEPHYRUM], piscina, "functio",
                "quadratum");
            quaestio = silva_quaestiones_parare(piscina,
                bibliotheca, nominata, argumenta, I, &culpa);
            CREDO_NON_NIHIL (quaestio);
            resultata = silva_quaestio_exsequi(quaestio,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            /* bis sola vocationem continet */
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)xar_obtinere(
                        resultata, ZEPHYRUM);

                CREDO_NON_NIHIL (r);
                CREDO_NON_NIHIL (r->capturae);
                CREDO_AEQUALIS_I32 (xar_numerus(r->capturae), I);
            }

            /* invarians (zero parametra): executio directa */
            nominata = silva_quaestiones_invenire(bibliotheca,
                "redde-intra-redde");
            quaestio = silva_quaestiones_parare(piscina,
                bibliotheca, nominata, NIHIL, ZEPHYRUM, &culpa);
            CREDO_NON_NIHIL (quaestio);
            resultata = silva_quaestio_exsequi(quaestio,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), ZEPHYRUM);

            /* ================================================
             * PROBARE: fracturae parandi
             * ================================================ */
            imprimere("\n--- Probans fracturas parandi ---\n");

            nominata = silva_quaestiones_invenire(bibliotheca,
                "vocantes-continentes");

            /* argumentum absens */
            CREDO_NIHIL (silva_quaestiones_parare(piscina,
                bibliotheca, nominata, NIHIL, ZEPHYRUM, &culpa));
            CREDO_VERUM (_continet(culpa, "argumentum absens"));

            /* argumentum ignotum */
            _argumentum(&argumenta[ZEPHYRUM], piscina, "functio",
                "bis");
            _argumentum(&argumenta[I], piscina, "alienum", "x");
            CREDO_NIHIL (silva_quaestiones_parare(piscina,
                bibliotheca, nominata, argumenta, II, &culpa));
            CREDO_VERUM (_continet(culpa, "argumentum ignotum"));

            /* argumentum iteratum */
            _argumentum(&argumenta[ZEPHYRUM], piscina, "functio",
                "bis");
            _argumentum(&argumenta[I], piscina, "functio", "bis");
            CREDO_NIHIL (silva_quaestiones_parare(piscina,
                bibliotheca, nominata, argumenta, II, &culpa));
            CREDO_VERUM (_continet(culpa, "argumentum iteratum"));

            /* valor illicitus */
            _argumentum(&argumenta[ZEPHYRUM], piscina, "functio",
                "mala\")definitio");
            CREDO_NIHIL (silva_quaestiones_parare(piscina,
                bibliotheca, nominata, argumenta, I, &culpa));
            CREDO_VERUM (_continet(culpa, "valor illicitus"));
        }
    }

    /* ========================================================
     * PROBARE: classes fracturae lectionis (plagula tota aut
     * nihil - quaeque culpa nominata)
     * ======================================================== */
    {
        imprimere("\n--- Probans classes fracturae ---\n");

        /* stml malformatum */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio", "stml non parsatur"));

        /* radix aliena */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<alia></alia>", "radix non est"));

        /* elementum ignotum sub radice */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><ignotum></ignotum></quaestiones>",
            "elementum ignotum sub radice"));

        /* quaestio sine titulo */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio gradus=\"oculi\">"
            "<selector>redde</selector><causa>c</causa>"
            "</quaestio></quaestiones>",
            "sine titulo"));

        /* gradus absens */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\">"
            "<selector>redde</selector><causa>c</causa>"
            "</quaestio></quaestiones>",
            "gradus absens"));

        /* gradus ignotus */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"porta\">"
            "<selector>redde</selector><causa>c</causa>"
            "</quaestio></quaestiones>",
            "gradus ignotus"));

        /* selector absens */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\"><causa>c</causa>"
            "</quaestio></quaestiones>",
            "selector absens"));

        /* causa absens (obligatoria) */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\"><selector>redde</selector>"
            "</quaestio></quaestiones>",
            "causa absens"));

        /* parametrum ignotum in selectore */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\">"
            "<selector>declarator-titulus"
            "[tok_titulus=\"$alienum\"]</selector>"
            "<causa>c</causa></quaestio></quaestiones>",
            "parametrum ignotum"));

        /* parametrum declaratum non adhibitum */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\"><selector>redde</selector>"
            "<parametrum titulus=\"otiosum\"/>"
            "<causa>c</causa></quaestio></quaestiones>",
            "parametrum non adhibitum"));

        /* parametrum vacuum ('$' nudum in citatione) */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\">"
            "<selector>declarator-titulus"
            "[tok_titulus=\"$\"]</selector>"
            "<causa>c</causa></quaestio></quaestiones>",
            "parametrum vacuum"));

        /* parametrum iteratum */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\">"
            "<selector>declarator-titulus"
            "[tok_titulus=\"$p\"]</selector>"
            "<parametrum titulus=\"p\"/>"
            "<parametrum titulus=\"p\"/>"
            "<causa>c</causa></quaestio></quaestiones>",
            "parametrum iteratum"));

        /* invarians cum parametris */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"invarians\">"
            "<selector>declarator-titulus"
            "[tok_titulus=\"$p\"]</selector>"
            "<parametrum titulus=\"p\"/>"
            "<causa>c</causa></quaestio></quaestiones>",
            "invarians cum parametris"));

        /* selector non compilatur */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\">"
            "<selector>tagus-nusquam-notus</selector>"
            "<causa>c</causa></quaestio></quaestiones>",
            "selector non compilatur"));

        /* titulus iteratus */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones>"
            "<quaestio titulus=\"t\" gradus=\"oculi\">"
            "<selector>redde</selector><causa>c</causa></quaestio>"
            "<quaestio titulus=\"t\" gradus=\"oculi\">"
            "<selector>redde</selector><causa>c</causa></quaestio>"
            "</quaestiones>",
            "titulus iteratus"));

        /* elementum ignotum intra quaestionem */
        CREDO_VERUM (_fractura_exspectata(piscina,
            "<quaestiones><quaestio titulus=\"t\""
            " gradus=\"oculi\"><selector>redde</selector>"
            "<causa>c</causa><mysterium></mysterium>"
            "</quaestio></quaestiones>",
            "elementum ignotum"));
    }

    /* ========================================================
     * PROBARE: plagula commissa silva/quaestiones.stml valida
     * (pinna - entrium invalidum suite rubram facit)
     * ======================================================== */
    {
        constans character* radix = getenv("RHUBARB_RADIX");
        character via[512];
        FILE* filum;

        imprimere("\n--- Probans plagulam commissam ---\n");

        CREDO_NON_NIHIL (radix);
        si (radix != NIHIL)
        {
            (vacuum)sprintf(via, "%s/silva/quaestiones.stml",
                radix);
            filum = fopen(via, "rb");
            CREDO_NON_NIHIL (filum);
            si (filum != NIHIL)
            {
                character* textus = (character*)piscina_allocare(
                    piscina, (memoriae_index)65536);
                size_t lecta = fread(textus, (size_t)I,
                    (size_t)65535, filum);
                chorda fons;
                chorda culpa;
                SilvaQuaestiones* bibliotheca;

                (vacuum)fclose(filum);
                CREDO_VERUM (lecta > (size_t)ZEPHYRUM);
                fons.datum = (i8*)textus;
                fons.mensura = (i32)lecta;
                bibliotheca = silva_quaestiones_legere(piscina,
                    &SILVA_C89_REGISTRUM, NIHIL, fons, &culpa);
                si (bibliotheca == NIHIL)
                {
                    imprimere("  culpa: %.*s\n",
                        (integer)culpa.mensura,
                        (constans character*)culpa.datum);
                }
                CREDO_NON_NIHIL (bibliotheca);
                CREDO_VERUM (silva_quaestiones_numerus(bibliotheca)
                    >= IV);
                CREDO_NON_NIHIL (silva_quaestiones_invenire(
                    bibliotheca, "exsequi-in-pulsatione"));
            }
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
