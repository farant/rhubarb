/* probatio_saltuarius_columnae.c - Aurei cellularum visus
 * columnarum (Chunk A2)
 *
 * Terminal fictus 60x12 per pontem memoriae; asserta in CELLAS
 * (proba_linea/quaerere/PROBA_CELLA) - praesentare numquam
 * vocatur. Metra 60x12: parens 0..6, sep 7, currens 8..29,
 * sep 30, praevisus 31..59; contentum ordines 1..10; status 11.
 *
 * Scaenae: dispositio, panni + suffixa + selectio INVERSUM,
 * praevisus directorii/textus (incl. purgatio \t et \r), motus
 * selectionis inter picturas (nihil manet), vacuum, nuntius,
 * truncatio ad limitem panni (separator intactus).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_columnae.h"
#include "saltuarius_proba.h"
#include <stdio.h>

interior DirectoriumIntroitus
_introitus (constans character* titulus, IntroitusGenus genus,
    Piscina* piscina)
{
    DirectoriumIntroitus intr;

    intr.titulus = chorda_ex_literis(titulus, piscina);
    intr.genus = genus;
    intr.mensura = ZEPHYRUM;
    redde intr;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    TesseraPiscina* tp;
    TesseraPonsMemoriae* pm;
    TesseraOpus* opus;
    SaltuariusRes* res;
    SaltuariusColumnae* columnae;
    SaltuariusTabularium* parens;
    SaltuariusTabularium* currens;
    SaltuariusTabularium* praevisus;

    piscina = piscina_generare_dynamicum("probatio_salt_col",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    tp = tessera_piscina_generare_dynamicum("salt_col_tessera",
        16777216);
    pm = tessera_pons_memoriae_creare(tp, LX, XII);
    opus = tessera_aperire(tp, &pm->pons);
    si (opus == NIHIL)
    {
        imprimere("FRACTA: apertura tesserae\n");
        redde I;
    }

    /* ========================================================
     * PROBARE: metra (mathesis dispositionis figitur)
     * ======================================================== */
    {
        SaltuariusMetra metra;

        imprimere("\n--- Probans metra (60x12) ---\n");

        saltuarius_columnae_metiri(LX, XII, &metra);
        CREDO_AEQUALIS_S32 (metra.parens_x, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (metra.parens_lat, VII);
        CREDO_AEQUALIS_S32 (metra.currens_x, VIII);
        CREDO_AEQUALIS_S32 (metra.currens_lat, XXII);
        CREDO_AEQUALIS_S32 (metra.praevisus_x, XXXI);
        CREDO_AEQUALIS_S32 (metra.praevisus_lat, XXIX);
        CREDO_AEQUALIS_S32 (metra.contentum_y, I);
        CREDO_AEQUALIS_S32 (metra.contentum_alt, X);
    }

    /* fixum commune */
    res = saltuarius_res_creare(piscina, "/a/b");
    columnae = saltuarius_columnae_creare(piscina);
    parens = saltuarius_tabularium_creare(piscina);
    currens = saltuarius_tabularium_creare(piscina);
    praevisus = saltuarius_tabularium_creare(piscina);
    {
        DirectoriumIntroitus fontes_parens[II];
        DirectoriumIntroitus fontes_currens[IV];
        DirectoriumIntroitus fontes_praevisus[II];

        fontes_parens[ZEPHYRUM] = _introitus("a",
            INTROITUS_DIRECTORIUM, piscina);
        fontes_parens[I] = _introitus("b", INTROITUS_DIRECTORIUM,
            piscina);
        saltuarius_tabularium_implere(parens, piscina, fontes_parens, II);
        parens->selectio = I;   /* "b" = directorium currens */

        fontes_currens[ZEPHYRUM] = _introitus("fontes",
            INTROITUS_DIRECTORIUM, piscina);
        fontes_currens[I] = _introitus("lib",
            INTROITUS_DIRECTORIUM, piscina);
        fontes_currens[II] = _introitus("opus.c", INTROITUS_FILUM,
            piscina);
        fontes_currens[III] = _introitus("nexus", INTROITUS_ALIUS,
            piscina);
        saltuarius_tabularium_implere(currens, piscina, fontes_currens, IV);

        fontes_praevisus[ZEPHYRUM] = _introitus("x.c",
            INTROITUS_FILUM, piscina);
        fontes_praevisus[I] = _introitus("y.c", INTROITUS_FILUM,
            piscina);
        saltuarius_tabularium_implere(praevisus, piscina, fontes_praevisus,
            II);
    }
    columnae->parens = parens;
    columnae->currens = currens;
    columnae->praevisus_genus = SALT_PRAEVISUS_DIRECTORIUM;
    columnae->praevisus_tab = praevisus;

    /* ========================================================
     * PROBARE: scaena prima - panni, suffixa, selectio, status
     * ======================================================== */
    {
        chorda linea;

        imprimere("\n--- Probans scaenam primam ---\n");

        saltuarius_columnae_pingere(columnae, res, opus);

        /* ordo 0: via, crassa */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, ZEPHYRUM, "/a/b"),
            ZEPHYRUM);
        CREDO_VERUM (PROBA_CELLA(opus, ZEPHYRUM, ZEPHYRUM).ornamenta
            & TESSERA_ORNAMENTUM_CRASSUM);

        /* pannus parens: a/ et b/ (b crassum - selectum sine
         * foco) */
        linea = saltuarius_proba_linea(piscina, opus, I, ZEPHYRUM,
            VII);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "a/");
        linea = saltuarius_proba_linea(piscina, opus, II, ZEPHYRUM,
            VII);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "b/");
        CREDO_VERUM (PROBA_CELLA(opus, ZEPHYRUM, II).ornamenta
            & TESSERA_ORNAMENTUM_CRASSUM);
        CREDO_FALSUM (PROBA_CELLA(opus, ZEPHYRUM, II).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);

        /* separatores */
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, VII, I).signum,
            TESSERA_SIGNUM_SIMPLEX_V);
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, XXX, V).signum,
            TESSERA_SIGNUM_SIMPLEX_V);

        /* pannus currens: ordinatum (directoria primum), suffixa,
         * selectio INVERSUM trans pannum */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, I, "fontes/"), VIII);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, II, "lib/"), VIII);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, III, "nexus?"), VIII);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, IV, "opus.c"), VIII);
        CREDO_VERUM (PROBA_CELLA(opus, VIII, I).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);
        /* vectis trans pannum totum (cella ultima panni) */
        CREDO_VERUM (PROBA_CELLA(opus, XXIX, I).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);
        CREDO_FALSUM (PROBA_CELLA(opus, VIII, II).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);

        /* praevisus: directorium selectum (fontes) -> x.c y.c */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, I, "x.c"), XXXI);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, II, "y.c"), XXXI);

        /* status: titulus selectus sinistra, 1/4 dextra */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, XI, "fontes"),
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, XI, "1/4"), LVII);
    }

    /* ========================================================
     * PROBARE: scaena secunda - selectio mota, praevisus textus
     * (purgatio \t, \r), nihil primae manet
     * ======================================================== */
    {
        chorda linea;

        imprimere("\n--- Probans scaenam secundam ---\n");

        currens->selectio = II;   /* opus.c */
        columnae->praevisus_genus = SALT_PRAEVISUS_TEXTUS;
        columnae->praevisus_textus = chorda_ex_literis(
            "alpha\nbeta\rcum\ttabs\r\nfinis", piscina);
        saltuarius_columnae_pingere(columnae, res, opus);

        /* selectio mota: INVERSUM ad ordinem IV (opus.c currens
         * ordinatum: fontes lib nexus? opus.c -> index II =
         * ordo... ordinatum: fontes/ lib/ (dirs), deinde nexus,
         * opus.c: index II = nexus? NON - ordinatio: directoria
         * (fontes, lib) primum, deinde fila ordine octetorum
         * (nexus, opus.c). index II = nexus (ALIUS non
         * directorium!) ordo III. */
        CREDO_VERUM (PROBA_CELLA(opus, VIII, III).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);
        CREDO_FALSUM (PROBA_CELLA(opus, VIII, I).ornamenta
            & TESSERA_ORNAMENTUM_INVERSUM);

        /* praevisus textus: lineae fissae, \r intra lineam
         * purgatum in spatium, \t purgatum, \r\n tonsum */
        linea = saltuarius_proba_linea(piscina, opus, I, XXXI,
            XXIX);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "alpha");
        linea = saltuarius_proba_linea(piscina, opus, II, XXXI,
            XXIX);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "beta cum tabs");
        linea = saltuarius_proba_linea(piscina, opus, III, XXXI,
            XXIX);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "finis");

        /* scaenae prioris nihil: x.c abest */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, I, "x.c"), -I);

        /* status: 3/4 */
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, XI, "3/4"), LVII);
    }

    /* ========================================================
     * PROBARE: truncatio - titulus longior quam pannus;
     * separator intactus
     * ======================================================== */
    {
        DirectoriumIntroitus longa[I];
        chorda linea;

        imprimere("\n--- Probans truncationem ---\n");

        longa[ZEPHYRUM] = _introitus(
            "titulus_valde_longus_qui_non_capit.c",
            INTROITUS_FILUM, piscina);
        saltuarius_tabularium_implere(currens, piscina, longa, I);
        columnae->praevisus_genus = SALT_PRAEVISUS_NIHIL;
        saltuarius_columnae_pingere(columnae, res, opus);

        /* XXII runae positae, cellae XXIX (post pannum) vacuae -
         * praecisio NOSTRA, non tesserae */
        linea = saltuarius_proba_linea(piscina, opus, I, VIII,
            XXII);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea,
            "titulus_valde_longus_q");
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, XXX, I).signum,
            TESSERA_SIGNUM_SIMPLEX_V);
        linea = saltuarius_proba_linea(piscina, opus, I, XXXI,
            XXIX);
        CREDO_AEQUALIS_I32 (linea.mensura, ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: vacuum + nuntius
     * ======================================================== */
    {
        chorda linea;

        imprimere("\n--- Probans vacuum et nuntium ---\n");

        saltuarius_tabularium_implere(currens, piscina, NIHIL, ZEPHYRUM);
        saltuarius_res_nuntiare(res, "silva legit...", II);
        saltuarius_columnae_pingere(columnae, res, opus);

        linea = saltuarius_proba_linea(piscina, opus, I, VIII,
            XXII);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "(vacuum)");
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, XI, "silva legit..."),
            ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: amplitudo mutata (scripta) - dispositio nova,
     * pictura plena intra fines novos
     * ======================================================== */
    {
        DirectoriumIntroitus unus[I];
        SaltuariusMetra metra;
        chorda linea;

        imprimere("\n--- Probans amplitudinem mutatam ---\n");

        unus[ZEPHYRUM] = _introitus("brevis.c", INTROITUS_FILUM,
            piscina);
        saltuarius_tabularium_implere(currens, piscina, unus, I);
        res->nuntius_aetas = ZEPHYRUM;

        tessera_pons_memoriae_amplitudo(pm, XL, VIII);
        CREDO_VERUM (tessera_magnitudinem_renovare(opus));
        CREDO_AEQUALIS_I32 (tessera_latitudo(opus), XL);
        CREDO_AEQUALIS_I32 (tessera_altitudo(opus), VIII);

        saltuarius_columnae_pingere(columnae, res, opus);
        saltuarius_columnae_metiri(XL, VIII, &metra);
        /* 40/8=5: parens 0..4, sep 5, currens 6..20, sep 21,
         * praevisus 22..39; status ordo 7 */
        CREDO_AEQUALIS_S32 (metra.currens_x, VI);
        linea = saltuarius_proba_linea(piscina, opus, I, VI, XV);
        CREDO_CHORDA_AEQUALIS_LITERIS (linea, "brevis.c");
        CREDO_AEQUALIS_I32 (PROBA_CELLA(opus, XXI, I).signum,
            TESSERA_SIGNUM_SIMPLEX_V);
        CREDO_AEQUALIS_S32 (
            saltuarius_proba_quaerere(opus, VII, "1/1"), XXXVII);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    tessera_piscina_destruere(tp);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
