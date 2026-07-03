/* probatio_saltuarius_visum.c - Aurei cellularum visus fontis
 * (Chunk B2)
 *
 * Terminal fictus 40x8. Asserta: cunula, COLORES PER CLASSEM in
 * cellis (latina/verbum/numerus/commentum incl. SPANSIONEM trans
 * lineas), cursor verus, volumen horizontale, linea status cum
 * pittacio (silva / textus merus).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_visum.h"
#include "saltuarius_proba.h"
#include <stdio.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    TesseraPiscina* tp;
    TesseraPonsMemoriae* pm;
    TesseraOpus* opus;
    SaltuariusNexus* nexus;
    SaltuariusRes* res;

    piscina = piscina_generare_dynamicum("probatio_salt_visum",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    res = saltuarius_res_creare(piscina, "/probe");
    tp = tessera_piscina_generare_dynamicum("salt_visum_tessera",
        16777216);
    pm = tessera_pons_memoriae_creare(tp, XL, VIII);
    opus = tessera_aperire(tp, &pm->pons);
    si (opus == NIHIL || nexus == NIHIL)
    {
        imprimere("FRACTA: apertura\n");
        redde I;
    }

    /* ========================================================
     * PROBARE: scaena colorata - cunula, classes, cursor, status
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans scaenam coloratam ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("probe.c", piscina),
            chorda_ex_literis(
                "si (x) redde; /* c1\nc2 */\nint y = 42;\n",
                piscina));
        CREDO_NON_NIHIL (liber);
        saltuarius_visum_pingere(liber, res, opus);

        /* cunula: "1".."4" dextro-ordinata in columna 0 */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, ZEPHYRUM,
            ZEPHYRUM).signum, (i32)'1');
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, ZEPHYRUM, II).signum,
            (i32)'3');

        /* textus: linea 0 incipit ad x=2 */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, ZEPHYRUM, "si (x)"),
            II);

        /* colores: si LATINA, x PLANUM, redde LATINA */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            ZEPHYRUM).color_litterae, 0x00C586C0);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, VI,
            ZEPHYRUM).color_litterae, (i32)TESSERA_COLOR_NATIVUS);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, IX,
            ZEPHYRUM).color_litterae, 0x00C586C0);

        /* commentum trans lineas: c1 in linea 0 (col XVII ->
         * x=XIX), c2 in linea I (x=II) - AMBO viridia */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, XIX,
            ZEPHYRUM).color_litterae, 0x006A9955);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            I).color_litterae, 0x006A9955);

        /* verbum C89 + numerus in linea II */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            II).color_litterae, 0x00569CD6);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, X,
            II).color_litterae, 0x00B5CEA8);

        /* cursor verus ad initium */
        CREDO_AEQUALIS_S32 (opus->cursor_x, II);
        CREDO_AEQUALIS_S32 (opus->cursor_y, ZEPHYRUM);

        /* status: via + 1:1 + stratum + pittacium silva */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, VII, "probe.c"),
            ZEPHYRUM);
        CREDO_VERUM (
            saltuarius_proba_quaerere(opus, VII, "1:1") > ZEPHYRUM);
        CREDO_VERUM (saltuarius_proba_quaerere(opus, VII,
            "stratum 0") > ZEPHYRUM);
        CREDO_VERUM (
            saltuarius_proba_quaerere(opus, VII, "silva")
            > ZEPHYRUM);

        /* motus: linea III columna V -> cursor et status */
        saltuarius_liber_movere(liber, II);
        saltuarius_liber_movere_col(liber, IV);
        saltuarius_visum_pingere(liber, res, opus);
        CREDO_AEQUALIS_S32 (opus->cursor_x, VI);
        CREDO_AEQUALIS_S32 (opus->cursor_y, II);
        CREDO_VERUM (
            saltuarius_proba_quaerere(opus, VII, "3:5") > ZEPHYRUM);

        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: volumen horizontale
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans volumen horizontale ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("longus.c", piscina),
            chorda_ex_literis(
                "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJ",
                piscina));
        CREDO_NON_NIHIL (liber);

        /* textus_lat = XL - II = XXXVIII; columna XLV ->
         * volumen_x = XLV - XXXVIII + I = VIII */
        saltuarius_liber_movere_col(liber, XLV);
        saltuarius_visum_pingere(liber, res, opus);
        CREDO_AEQUALIS_S32 (liber->volumen_x, VIII);
        /* runa prima visibilis = 'i' (index VIII) */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            ZEPHYRUM).signum, (i32)'i');
        CREDO_AEQUALIS_S32 (opus->cursor_x,
            II + (XLV - VIII));

        /* redire ad initium: volumen sequitur */
        saltuarius_liber_movere_col(liber, -C);
        saltuarius_visum_pingere(liber, res, opus);
        CREDO_AEQUALIS_S32 (liber->volumen_x, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            ZEPHYRUM).signum, (i32)'a');

        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: merus - sine coloribus, pittacium verum
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans merum ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("notae.md", piscina),
            chorda_ex_literis("si verba hic\n", piscina));
        CREDO_NON_NIHIL (liber);
        saltuarius_visum_pingere(liber, res, opus);

        /* "si" in .md NON colorata (nativus) */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, II,
            ZEPHYRUM).color_litterae, (i32)TESSERA_COLOR_NATIVUS);
        CREDO_VERUM (saltuarius_proba_quaerere(opus, VII,
            "textus merus") > ZEPHYRUM);

        saltuarius_liber_destruere(liber);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    tessera_piscina_destruere(tp);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
