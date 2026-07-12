/* probatio_officina_sessio.c - Vectis A chunki M4b: sessio nucleus
 *
 * Scenaria: classificatio (quinque genera), reparatio-echo `;`,
 * circuitus positionum (iudicata -> ostensa), verdictum documenti
 * totius (redefinitio-reicit-ad-lineam-veterem C2), religatio in
 * loco, regula elementi unius, macro trans turnos, #include per
 * praebenda. Sine machinula (chunk A).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "sessio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (constans character* literis, Piscina* piscina)
{
    redde chorda_ex_literis(literis, piscina);
}

interior b32
_chorda_continet (chorda textus, constans character* acus)
{
    i32 m = (i32)strlen(acus);
    i32 k;

    si (m == ZEPHYRUM || textus.mensura < m)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + m <= textus.mensura; k++)
    {
        si (memcmp(textus.datum + k, acus, (memoriae_index)m)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    constans character* radix;
    SessioConfiguratio cfg;

    piscina = piscina_generare_dynamicum("probatio_sessio",
        134217728);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    cfg.radix = radix;
    cfg.cum_posix = FALSUM;
    cfg.sine_capitibus = VERUM;

    /* ========================================================
     * PROBARE: creatio + initus_completus (librae)
     * ======================================================== */
    {
        Sessio* s;

        imprimere("\n--- Probans creatio + initus_completus ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), ZEPHYRUM);

        CREDO_VERUM(sessio_initus_completus(s,
            _ch("integer x = 5;", piscina)));
        CREDO_FALSUM(sessio_initus_completus(s,
            _ch("si (x) {", piscina)));
        CREDO_VERUM(sessio_initus_completus(s,
            _ch("si (x) { x = 1; }", piscina)));
        CREDO_FALSUM(sessio_initus_completus(s,
            _ch("/* apertum", piscina)));
        CREDO_FALSUM(sessio_initus_completus(s,
            _ch("f(1, 2", piscina)));
        CREDO_VERUM(sessio_initus_completus(s,
            _ch("x = \"{ non libra\";", piscina)));

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: classificatio + acceptatio + reparatio
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;

        imprimere("\n--- Probans classificatio + reparatio ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        /* declaratio */
        r = sessio_turnum_offerre(s, _ch("integer x = 5;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DECLARATIO);
        CREDO_FALSUM(r.reparatum);
        CREDO_AEQUALIS_I32((i32)r.turnus_index, ZEPHYRUM);
        CREDO_VERUM(chorda_aequalis_literis(r.nomen_declaratum, "x"));

        /* sententia-expressionis (assignatio) */
        r = sessio_turnum_offerre(s, _ch("x = x + 1;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_EXPRESSIO);

        /* expressio nuda -> reparatio `;` */
        r = sessio_turnum_offerre(s, _ch("x + 2", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_EXPRESSIO);
        CREDO_VERUM(r.reparatum);
        CREDO_VERUM(r.textus.mensura >= II);
        CREDO_AEQUALIS_I32((i32)r.textus.datum[r.textus.mensura - II],
            (i32)';');

        /* definitio functionis */
        r = sessio_turnum_offerre(s, _ch(
            "integer duplum(integer a) { redde a + a; }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DEFINITIO);
        CREDO_VERUM(chorda_aequalis_literis(r.nomen_declaratum,
            "duplum"));

        /* sententia (si) */
        r = sessio_turnum_offerre(s, _ch("si (x) { x = duplum(x); }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_SENTENTIA);

        /* directiva + macro trans turnos */
        r = sessio_turnum_offerre(s, _ch("#define BIS(x) ((x)+(x))",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DIRECTIVA);
        r = sessio_turnum_offerre(s, _ch("BIS(x)", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.reparatum);

        /* documentum: omnes textus in ordine */
        {
            chorda doc = sessio_documentum(s, piscina);

            CREDO_VERUM(_chorda_continet(doc, "integer x = 5;"));
            CREDO_VERUM(_chorda_continet(doc, "duplum"));
            CREDO_VERUM(_chorda_continet(doc, "#define BIS"));
            CREDO_FALSUM(_chorda_continet(doc, "turnus_"));
        }
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), VII);

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: reiectio (identificator ignotus + syntaxis +
     * multi-elementa) - documentum immotum
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;

        imprimere("\n--- Probans reiectio ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("integer a = 1;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* identificator ignotus: linea ostensa = 2 (post turnum
         * primum unius lineae) */
        r = sessio_turnum_offerre(s, _ch("ignotum + 1;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_VERUM(r.diagnostica_numerus >= (s32)I);
        CREDO_AEQUALIS_I32(r.diagnostica[ZEPHYRUM].linea, II);
        CREDO_FALSUM(r.diagnostica[ZEPHYRUM].extra_turnum);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), I);

        /* syntaxis insanabilis */
        r = sessio_turnum_offerre(s, _ch("integer @@@", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), I);

        /* multi-elementa */
        r = sessio_turnum_offerre(s, _ch("integer b; integer c;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_VERUM(r.diagnostica_numerus >= (s32)I);
        CREDO_VERUM(_chorda_continet(r.diagnostica[ZEPHYRUM].nuntius,
            "multi-elementorum"));
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), I);

        /* corpus compositum = fuga licita */
        r = sessio_turnum_offerre(s, _ch("{ integer b; a = b; }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_SENTENTIA);

        /* initus vacuus */
        r = sessio_turnum_offerre(s, _ch("   ", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: positiones multi-lineares (circuitus C3)
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;

        imprimere("\n--- Probans positiones multi-lineares ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("integer a = 1;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* turnus trium linearum; malum in linea secunda ->
         * ostensa = 1 (turnus prior) + 1 (linea prima turni) + 1 */
        r = sessio_turnum_offerre(s, _ch("{\nmalum = 1;\n}",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_VERUM(r.diagnostica_numerus >= (s32)I);
        CREDO_AEQUALIS_I32(r.diagnostica[ZEPHYRUM].linea, III);
        CREDO_VERUM(r.diagnostica[ZEPHYRUM].columna >= I);

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: religatio in loco (substitutio)
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;

        imprimere("\n--- Probans religatio in loco ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("integer y = 5;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("y = y + 1;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* religatio: y = 6 substituit turnum 0 */
        r = sessio_turnum_offerre(s, _ch("integer y = 6;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.substitutus);
        CREDO_AEQUALIS_I32((i32)r.turnus_substitutus, ZEPHYRUM);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), II);
        {
            chorda doc = sessio_documentum(s, piscina);

            CREDO_VERUM(_chorda_continet(doc, "y = 6"));
            CREDO_FALSUM(_chorda_continet(doc, "y = 5"));
        }

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: redefinitio reicit ad lineam veterem (C2)
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;

        imprimere("\n--- Probans redefinitio ad lineam veterem ---\n");
        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch(
            "integer f(integer a) { redde a; }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("f(1);", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* redefinitio incompatibilis: vocatio turni 1 aegrotat */
        r = sessio_turnum_offerre(s, _ch(
            "integer f(integer a, integer b) { redde a + b; }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_VERUM(r.diagnostica_numerus >= (s32)I);
        {
            b32 extra_inventum = FALSUM;
            s32 k;

            per (k = ZEPHYRUM; k < r.diagnostica_numerus; k++)
            {
                si (r.diagnostica[k].extra_turnum
                    && r.diagnostica[k].turnus_vetus == (s32)I)
                {
                    extra_inventum = VERUM;
                }
            }
            CREDO_VERUM(extra_inventum);
        }
        /* documentum immotum: f manet una-parametri */
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), II);
        CREDO_VERUM(_chorda_continet(sessio_turnus_textus(s,
            ZEPHYRUM), "redde a;"));

        /* mundus consistens: vocatio iterum accipitur */
        r = sessio_turnum_offerre(s, _ch("f(2);", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* religatio COMPATIBILIS accipitur (corpus mutatum) */
        r = sessio_turnum_offerre(s, _ch(
            "integer f(integer a) { redde a * 2; }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.substitutus);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), III);
        CREDO_VERUM(_chorda_continet(sessio_turnus_textus(s,
            ZEPHYRUM), "a * 2"));

        sessio_destruere(s);
    }

    /* ========================================================
     * PROBARE: #include per praebenda (capita ambulata)
     * ======================================================== */
    {
        Sessio* s;
        SessioRelatum r;
        SessioConfiguratio cfg_capita;

        imprimere("\n--- Probans #include per praebenda ---\n");
        cfg_capita = cfg;
        cfg_capita.sine_capitibus = FALSUM;
        s = sessio_creare(piscina, &cfg_capita);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("#include \"piscina.h\"",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DIRECTIVA);

        r = sessio_turnum_offerre(s, _ch(
            "Piscina* p = piscina_generare_dynamicum(\"proba\","
            " 1024);", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DECLARATIO);
        CREDO_VERUM(chorda_aequalis_literis(r.nomen_declaratum,
            "p"));

        sessio_destruere(s);
    }

    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_imprimere_compendium();
        credo_claudere();
        redde praeteritus ? ZEPHYRUM : I;
    }
}
