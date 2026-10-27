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
#include "officina_machinula.h"   /* MACHINULA_* genera halitus */
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (
    constans character* literis,
               Piscina* piscina)
{
    redde chorda_ex_literis(literis, piscina);
}

interior b32
_chorda_continet (
                chorda  textus,
    constans character* acus)
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

/* diagnosticum cum codice examinis in relato? */
interior b32
_codicem_habet (
    constans SessioRelatum* r,
                       s32  codex)
{
    s32 k;

    per (k = ZEPHYRUM; k < r->diagnostica_numerus; k++)
    {
        si (r->diagnostica[k].codex == codex)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* radix ex ambitu (olim in praeludio principalis) */
interior constans character*
_radix (vacuum)
{
    constans character* radix = getenv("RHUBARB_RADIX");

    redde radix != NIHIL ? radix : ".";
}

/* configuratio communis sessionum (olim in praeludio principalis):
 * sectio quaeque suam reficit - nulla fixura, nihil commune */
interior vacuum
_configurationem_parare (
    SessioConfiguratio* cfg)
{
    memset(cfg, ZEPHYRUM, magnitudo(SessioConfiguratio));
    cfg->radix           = _radix();
    cfg->cum_posix       = FALSUM;
    cfg->sine_capitibus  = VERUM;
}


/* ==================================================
 * PROBARE: creatio + initus_completus (librae)
 * ================================================== */

interior vacuum
_initum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
        Sessio* s;

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
}


/* ==================================================
 * PROBARE: classificatio + acceptatio + reparatio
 * ================================================== */

interior vacuum
_genus_turni_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

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
}


/* ==================================================
 * PROBARE: reiectio (identificator ignotus + syntaxis +
 * multi-elementa) - documentum immotum
 * ================================================== */

interior vacuum
_reiectionem_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

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
}


/* ==================================================
 * PROBARE: positiones multi-lineares (circuitus C3)
 * ================================================== */

interior vacuum
_positiones_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

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
}


/* ==================================================
 * PROBARE: religatio in loco (substitutio)
 * ================================================== */

interior vacuum
_religationem_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

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
}


/* ==================================================
 * PROBARE: redefinitio reicit ad lineam veterem (C2)
 * ================================================== */

interior vacuum
_lineam_veterem_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

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
                si (   r.diagnostica[k].extra_turnum
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
}


/* ==================================================
 * PROBARE B1: persistentia globalium (scriptio-N/lectio-N+1)
 * - LACUNA exploratoris mundi: designata M2, probata HIC
 * ================================================== */

interior vacuum
_globalia_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("integer g = 7;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("g = g + 35;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.exsecutum);
        r = sessio_turnum_offerre(s, _ch("printf(\"%d\\n\", g);",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(chorda_aequalis_literis(r.effusio, "42\n"));

        /* B2: exit non fatale (vexillum halitus, mundus reusabilis) */
        r = sessio_turnum_offerre(s, _ch("{ exit(3); }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.halitus_genus,
            (i32)MACHINULA_BENE);
        CREDO_VERUM(r.halitus_codex == (s64)III);
        r = sessio_turnum_offerre(s, _ch("printf(\"%d\\n\", g);",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(chorda_aequalis_literis(r.effusio, "42\n"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE B3: recusationes (scripturae + tempus)
 * ================================================== */

interior vacuum
_recusationes_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        /* B3a: iudicium - scriptura cruda vocata = codex XCVII
         * (EXAMEN_CODEX_SCRIPTURA_PRAETERITA, VIOLATIO), ut examen;
         * turnus numquam currit */
        r = sessio_turnum_offerre(s, _ch("{ remove(\"nulla\"); }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_VERUM(_codicem_habet(&r, (s32)XCVII));
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), ZEPHYRUM);

        /* B3b: machinula - per monstratorem functionis (non vocatio
         * nominis: iudicium tacet) recusatio in tempore manet */
        r = sessio_turnum_offerre(s, _ch(
            "{ integer (*f)(constans character*) = remove;"
            " f(\"nulla\"); }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_FALSUM(_codicem_habet(&r, (s32)XCVII));
        CREDO_AEQUALIS_I32((i32)r.halitus_genus,
            (i32)MACHINULA_RECUSATIO);
        CREDO_VERUM(_chorda_continet(r.halitus_nuntius, "recusatum"));
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), ZEPHYRUM);

        r = sessio_turnum_offerre(s, _ch("{ clock(); }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_AEQUALIS_I32((i32)r.halitus_genus,
            (i32)MACHINULA_RECUSATIO);

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE B4: historia mutata (redefinitio effusionem mutat)
 * ================================================== */

interior vacuum
_historiam_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch(
            "integer f(vacuum) { printf(\"unus\\n\"); redde 1; }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("f();", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(chorda_aequalis_literis(r.effusio, "unus\n"));
        CREDO_AEQUALIS_I32((i32)r.turni_mutati_numerus, ZEPHYRUM);

        /* redefinitio compatibilis: effusio turni 1 mutatur */
        r = sessio_turnum_offerre(s, _ch(
            "integer f(vacuum) { printf(\"duo\\n\"); redde 2; }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.substitutus);
        CREDO_AEQUALIS_I32((i32)r.turni_mutati_numerus, I);
        CREDO_AEQUALIS_I32((i32)r.turni_mutati[ZEPHYRUM], I);
        CREDO_VERUM(chorda_aequalis_literis(
            sessio_turnus_effusio(s, I), "duo\n"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE B5: C8 - vocatio implicita ACCIPITUR iudicio,
 * DECIPULA in replicatione -> reiectio + restitutio mundi
 * ================================================== */

interior vacuum
_decipulam_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("integer h = 5;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        /* implicita: iudicium ACCIPIT (suspectum), replicatio
         * DECIPULAT */
        r = sessio_turnum_offerre(s, _ch("ignota();", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_AEQUALIS_I32((i32)r.halitus_genus,
            (i32)MACHINULA_DECIPULA);
        CREDO_AEQUALIS_I32(sessio_turni_numerus(s), I);

        /* mundus restitutus: turnus sequens vivit */
        r = sessio_turnum_offerre(s, _ch("printf(\"%d\\n\", h);",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(chorda_aequalis_literis(r.effusio, "5\n"));

        /* B6 (interludium): initiator globalis non constans ->
         * SCISSIO AUTOMATICA (declaratio + assignatio) */
        r = sessio_turnum_offerre(s, _ch("integer mala = h + 1;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.divisum);
        CREDO_VERUM(chorda_aequalis_literis(r.nomen_declaratum,
            "mala"));
        {
            chorda doc = sessio_documentum(s, piscina);

            CREDO_VERUM(_chorda_continet(doc, "integer mala;"));
            CREDO_VERUM(_chorda_continet(doc, "mala = h + 1;"));
        }
        r = sessio_turnum_offerre(s, _ch("mala;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.valor == (s64)VI);

        /* adressa constans LICET (sine scissione) */
        r = sessio_turnum_offerre(s, _ch("integer* adr = &h;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_FALSUM(r.divisum);

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE B7: bibliotheca vera (piscina demissa in mundum)
 * ================================================== */

interior vacuum
_bibliothecam_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
                               Sessio* s;
                        SessioRelatum  r;
                   SessioConfiguratio  cfg_bibliothecae;
                   constans character* plagulae[] = {
            "lib/piscina.c"
        };

        cfg_bibliothecae = cfg;
        cfg_bibliothecae.sine_capitibus = FALSUM;
        cfg_bibliothecae.cum_posix = VERUM;   /* piscina.c = mmap */
        cfg_bibliothecae.plagulae = plagulae;
        cfg_bibliothecae.plagulae_numerus = I;
        s = sessio_creare(piscina, &cfg_bibliothecae);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("#include \"piscina.h\"",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("Piscina* p = NIHIL;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch(
            "{ p = piscina_generare_dynamicum(\"proba\", 4096); }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch(
            "printf(\"%d\\n\", p != NIHIL);", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(chorda_aequalis_literis(r.effusio, "1\n"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE C: transcripta formata (vectis C - formator machinae)
 * ================================================== */

interior vacuum
_transcripta_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;
               chorda  f;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        /* echo declarationis: nomen : typus */
        r = sessio_turnum_offerre(s, _ch("integer x = 5;", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f, "x : integer\n"));

        /* valor : typus (+ echo reparationis) */
        r = sessio_turnum_offerre(s, _ch("x + 2", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f,
            "\xE2\x80\xA6 x + 2;\n7 : integer\n"));

        /* V3 VIVUM: f64 per codex (figura bitorum) */
        r = sessio_turnum_offerre(s, _ch(
            "duplex media(duplex a) { redde a / 2; }", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f, "media : functio\n"));
        r = sessio_turnum_offerre(s, _ch("media(5);", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f, "2.5 : duplex\n"));

        /* praevisus char* ex memoria regionis */
        r = sessio_turnum_offerre(s, _ch(
            "constans character* salve = \"salve\";", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("salve;", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(_chorda_continet(f,
            "\"salve\" : constans character*"));
        CREDO_VERUM(_chorda_continet(f, "0x"));

        /* insignatus */
        r = sessio_turnum_offerre(s, _ch(
            "insignatus integer u = 7;", piscina));
        CREDO_VERUM(chorda_aequalis_literis(
            sessio_relatum_formare(s, &r, piscina),
            "u : insignatus integer\n"));

        /* effusio ante valorem (programma loquitur primum) */
        r = sessio_turnum_offerre(s, _ch(
            "printf(\"salve!\\n\");", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f,
            "salve!\n7 : integer\n"));

        /* recusatio formata */
        r = sessio_turnum_offerre(s, _ch("{ clock(); }", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(_chorda_continet(f, "[halitus RECUSATIO]"));
        CREDO_VERUM(_chorda_continet(f, "recusatum"));

        /* diagnosticum formatum */
        r = sessio_turnum_offerre(s, _ch("ignotus_omnino;", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(_chorda_continet(f, "<sessio>:"));
        CREDO_VERUM(_chorda_continet(f, "[violatio]"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE C2: historia mutata formata + sessio plena
 * ================================================== */

interior vacuum
_historiam_formatam_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;
               chorda  f;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch(
            "integer f(vacuum) { printf(\"unus\\n\"); redde 1; }",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("f();", piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f,
            "unus\n1 : integer\n"));

        r = sessio_turnum_offerre(s, _ch(
            "integer f(vacuum) { printf(\"duo\\n\"); redde 2; }",
            piscina));
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(chorda_aequalis_literis(f,
            "f : functio\nturnus 1: effusio mutata\n"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE D1: recusatio stdin (familia tertia - vectis M4b)
 * ================================================== */

interior vacuum
_recusationem_ingressus_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;
               chorda  f;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch(
            "{ character b[8]; fgets(b, 8, stdin); }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_REIECTUM);
        CREDO_AEQUALIS_I32((i32)r.halitus_genus,
            (i32)MACHINULA_RECUSATIO);
        f = sessio_relatum_formare(s, &r, piscina);
        CREDO_VERUM(_chorda_continet(f, "stdin"));

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE D2: scriptum #! (fixum verum, bibliothecae II,
 * scripturae verae) - VECTIS M4b
 * ================================================== */

interior vacuum
_scriptum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;
    constans character* radix = _radix();

    _configurationem_parare(&cfg);
    {
                               Sessio* s;
                        SessioRelatum  r;
                   SessioConfiguratio  cfg_scripti;
        /* filum + dependentiae: scriptum per canalem lectionum
         * scribit/legit (examen 96 fopen crudum recusat) */
                   constans character* plagulae_scripti[] = {
            "lib/piscina.c", "lib/chorda.c", "lib/chorda_aedificator.c",
            "lib/lectiones.c", "lib/filum.c"
        };
        chorda scriptum;
           s32 recepti;
           s64 codex = -I;

        cfg_scripti = cfg;
        cfg_scripti.sine_capitibus = FALSUM;
        cfg_scripti.cum_posix = VERUM;
        cfg_scripti.sine_recusationibus = VERUM;
        cfg_scripti.plagulae = plagulae_scripti;
        cfg_scripti.plagulae_numerus = V;
        s = sessio_creare(piscina, &cfg_scripti);
        CREDO_NON_NIHIL(s);

        {
            character via_scripti[1024];
            FILE* pl;
            long m_l = 0L;
            i8* datum = NIHIL;

            sprintf(via_scripti,
                "%s/officina/probationes/fixa/sessio_scripta/"
                "salutatio.scriptum", radix);
            pl = fopen(via_scripti, "rb");
            CREDO_NON_NIHIL(pl);
            fseek(pl, 0L, SEEK_END);
            m_l = ftell(pl);
            fseek(pl, 0L, SEEK_SET);
            datum = piscina_allocare(piscina, (memoriae_index)m_l);
            CREDO_VERUM(fread(datum, I, (memoriae_index)m_l, pl)
                == (memoriae_index)m_l);
            fclose(pl);
            scriptum.datum    = datum;
            scriptum.mensura  = (i32)m_l;
        }

        recepti = sessio_scriptum_offerre(s, scriptum, &r);
        CREDO_AEQUALIS_I32((i32)recepti, IV);   /* 3 directivae + main */
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(sessio_functionem_currere(s, "main", &codex));
        CREDO_VERUM(codex == (s64)ZEPHYRUM);
        {
            /* scriptura VERA facta (recusationes desunt) */
            character via_effectus[1024];
            FILE* pl;

            sprintf(via_effectus,
                "%s/officina/build/salutatio_scripti.txt", radix);
            pl = fopen(via_effectus, "rb");
            CREDO_NON_NIHIL(pl);
            si (pl != NIHIL)
            {
                fclose(pl);
            }
        }
        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE D3: circulus serva/aperi (documentum -> sessio
 * recens -> documentum idem) - VECTIS M4b
 * ================================================== */

interior vacuum
_circulum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
               Sessio* s_a;
               Sessio* s_b;
        SessioRelatum  r;
               chorda  doc_a;
               chorda  doc_b;
                  s32  recepti;

        s_a = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s_a);
        r = sessio_turnum_offerre(s_a, _ch("integer x = 5;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s_a, _ch(
            "integer duplum(integer a) { redde a + a; }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s_a, _ch("x = duplum(x);",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        doc_a = sessio_documentum(s_a, piscina);
        CREDO_VERUM(doc_a.mensura > ZEPHYRUM);
        /* UNA regio uno tempore: sessio prior destruenda ante
         * exsecutionem alterius (basis fixa regionis) */
        sessio_destruere(s_a);
        s_a = NIHIL;

        s_b = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s_b);
        recepti = sessio_scriptum_offerre(s_b, doc_a, &r);
        CREDO_AEQUALIS_I32((i32)recepti, III);
        doc_b = sessio_documentum(s_b, piscina);
        CREDO_VERUM(chorda_aequalis(doc_a, doc_b));

        /* sessio aperta PERGIT (continuatio interactiva) */
        r = sessio_turnum_offerre(s_b, _ch("x;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_VERUM(r.valor == (s64)X);

        sessio_destruere(s_b);
    }
}


/* ==================================================
 * PROBARE D4: exportatio stricta compilat (clang vexillis
 * domus) - VECTIS M4b
 * ================================================== */

interior vacuum
_documentum_strictum_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;
    constans character* radix = _radix();

    _configurationem_parare(&cfg);
    {
               Sessio* s;
        SessioRelatum  r;
               chorda  strictum;
            character  via_export[1024];
            character  imperium[2048];
        FILE* pl;

        s = sessio_creare(piscina, &cfg);
        CREDO_NON_NIHIL(s);
        r = sessio_turnum_offerre(s, _ch("integer x = 5;", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch(
            "integer duplum(integer a) { redde a + a; }", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        r = sessio_turnum_offerre(s, _ch("x = duplum(x);", piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);

        strictum = sessio_documentum_strictum(s, piscina);
        CREDO_VERUM(_chorda_continet(strictum,
            "#include \"latina.h\""));
        CREDO_VERUM(_chorda_continet(strictum,
            "integer turnus_1(void)"));
        CREDO_VERUM(_chorda_continet(strictum,
            "integer principale(vacuum)"));

        sprintf(via_export, "%s/officina/build/strictum_export.c",
            radix);
        pl = lectiones_fopen(via_export, "wb");
        CREDO_NON_NIHIL(pl);
        fwrite(strictum.datum, I, (memoriae_index)strictum.mensura,
            pl);
        fclose(pl);

        sprintf(imperium, "clang -std=c89 -pedantic -Wall -Wextra"
            " -Werror -Wconversion -Wsign-conversion -Wcast-qual"
            " -Wstrict-prototypes -Wwrite-strings -Wno-long-long"
            " -I%s/include -c %s -o %s/officina/build/"
            "strictum_export.o", radix, via_export, radix);
        CREDO_AEQUALIS_I32((i32)system(imperium), ZEPHYRUM);

        sessio_destruere(s);
    }
}


/* ==================================================
 * PROBARE: #include per praebenda (capita ambulata)
 * ================================================== */

interior vacuum
_capita_probare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    SessioConfiguratio  cfg;

    _configurationem_parare(&cfg);
    {
                    Sessio* s;
             SessioRelatum  r;
        SessioConfiguratio  cfg_capita;

        cfg_capita = cfg;
        cfg_capita.sine_capitibus = FALSUM;
        s = sessio_creare(piscina, &cfg_capita);
        CREDO_NON_NIHIL(s);

        r = sessio_turnum_offerre(s, _ch("#include \"piscina.h\"",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DIRECTIVA);

        /* chunk B: initiator globalis non-constans reiceretur
         * (demissio) - constans nulla licita */
        r = sessio_turnum_offerre(s, _ch("Piscina* p = NIHIL;",
            piscina));
        CREDO_AEQUALIS_I32((i32)r.verdictum, (i32)SESSIO_ACCEPTUM);
        CREDO_AEQUALIS_I32((i32)r.genus,
            (i32)SESSIO_TURNUS_DECLARATIO);
        CREDO_VERUM(chorda_aequalis_literis(r.nomen_declaratum,
            "p"));

        sessio_destruere(s);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio + initus_completus", _initum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "classificatio + reparatio", _genus_turni_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "reiectio", _reiectionem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "positiones multi-lineares", _positiones_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "religatio in loco", _religationem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "redefinitio ad lineam veterem", _lineam_veterem_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "persistentia globalium (B1)", _globalia_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "recusationes (B3)", _recusationes_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "historia mutata (B4)", _historiam_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "C8 decipula + restitutio (B5)", _decipulam_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "bibliotheca vera (B7)", _bibliothecam_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "transcripta formata (C)", _transcripta_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "historia mutata formata (C2)", _historiam_formatam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusatio stdin (D1)", _recusationem_ingressus_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "scriptum #! (D2)", _scriptum_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "circulus serva/aperi (D3)", _circulum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "exportatio stricta (D4)", _documentum_strictum_probare, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "#include per praebenda", _capita_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_sessio", SECTIONES);
}
