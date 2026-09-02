/* probatio_saltuarius_salve.c - Harnesa saltuarii + pignora
 * dependentiarum (Phase 0)
 *
 * Quinque mores PIGNERANTUR in quibus Phase A nititur:
 *   (a) AMALGAMA TESSERAE ut hospes secundus verus: pingere, cellas
 *       relegere SINE praesentare (disciplina aureorum), octeti
 *       aurei quadri primi, clavis scripta per pontem memoriae.
 *       NB: piscinae DUAE — TesseraPiscina (vendicata) pro tessera,
 *       Piscina (rhubarb) pro statu app — haec est vita civis.
 *   (b) iter_directoria: classificatio introituum in arbore ficta
 *       (iterator . et .. ipse saltat — lib/iter_directoria.c:125).
 *   (c) filum_legere_totum: circuitus per filum scriptum.
 *   (d) via_nomen / via_iungere formae.
 *   (e) utf8: arithmetica sectionis runarum (primitivum
 *       truncationis praevisus).
 * Si mos dependentiae labitur, suite saltuarii id dicit ANTEQUAM
 * codex saltuarii ipse erret.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "via.h"
#include "iter_directoria.h"
#include "utf8.h"
#include "credo.h"
#include "tessera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIXUM "/tmp/saltuarius_salve_fixum"

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_salt_salve",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: (a) amalgama tesserae - hospes secundus verus
     * ======================================================== */

    {
        TesseraPiscina* tp;
        TesseraPonsMemoriae* pm;
        TesseraOpus* opus;
        TesseraCellula cella;
        TesseraChorda captum;
        chorda visus;

        imprimere("\n--- Probans amalgama tesserae (hospes II) ---\n");

        tp = tessera_piscina_generare_dynamicum("salve_tessera",
            8388608);
        CREDO_NON_NIHIL (tp);
        pm = tessera_pons_memoriae_creare(tp, XX, V);
        CREDO_NON_NIHIL (pm);
        opus = tessera_aperire(tp, &pm->pons);
        CREDO_NON_NIHIL (opus);

        /* pingere et cellas relegere SINE praesentare */
        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM,
            "saltuarius", tessera_stilus_nativus());
        cella = tessera_cellulam_legere(opus, ZEPHYRUM, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (cella.signum, (i32)'s');
        cella = tessera_cellulam_legere(opus, IX, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (cella.signum, (i32)'s');
        cella = tessera_cellulam_legere(opus, X, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (cella.signum, ZEPHYRUM);

        /* octeti aurei quadri primi (speculum hospitis tesserae) */
        CREDO_VERUM (tessera_praesentare(opus));
        captum         = tessera_pons_memoriae_captum(pm);
        visus.mensura  = captum.mensura;
        visus.datum    = captum.datum;
        CREDO_CHORDA_AEQUALIS_LITERIS (visus,
            "\033[?25l\033[2J\033[1;1H\033[0msaltuarius");

        /* clavis scripta per pontem memoriae (mechanismus aureorum
         * sessionum A2) */
        {
            TesseraLector* lector;
            TesseraEventum ev;

            lector = tessera_lector_creare(tp, &pm->pons);
            CREDO_NON_NIHIL (lector);
            CREDO_VERUM (tessera_pons_memoriae_initum(pm,
                (constans i8*)"j", I));
            tessera_eventum_expectare(lector, &ev, X);
            CREDO_AEQUALIS_I32 ((i32)ev.genus,
                (i32)TESSERA_EVENTUM_CLAVIS);
            CREDO_AEQUALIS_S32 (ev.runa, (s32)'j');
        }

        tessera_piscina_destruere(tp);
    }


    /* ========================================================
     * PROBARE: (b) iter_directoria in arbore ficta
     * ======================================================== */

    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* intr;
                         b32  subdir_inventum  = FALSUM;
                         b32  plaga_inventa    = FALSUM;
                         i32  numerus          = ZEPHYRUM;
        FILE* f;

        imprimere("\n--- Probans iter_directoria (arbor ficta) ---\n");

        (vacuum)system("rm -rf " FIXUM " && mkdir -p " FIXUM "/subdir");
        f = fopen(FIXUM "/plaga.txt", "wb");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            fputs("salve silva\n", f);
            fclose(f);
        }

        iter = directorium_iterator_aperire(FIXUM, piscina);
        CREDO_NON_NIHIL (iter);
        dum ((intr = directorium_iterator_proximum(iter)) != NIHIL)
        {
            numerus++;
            si (chorda_aequalis_literis(intr->titulus, "subdir"))
            {
                subdir_inventum =
                    (intr->genus == INTROITUS_DIRECTORIUM);
            }
            si (chorda_aequalis_literis(intr->titulus, "plaga.txt"))
            {
                plaga_inventa = (intr->genus == INTROITUS_FILUM)
                    && (intr->mensura == XII);
            }
        }
        directorium_iterator_claudere(iter);
        CREDO_AEQUALIS_I32 ((i32)numerus, II);  /* . et .. saltata */
        CREDO_VERUM (subdir_inventum);
        CREDO_VERUM (plaga_inventa);
    }


    /* ========================================================
     * PROBARE: (c) filum_legere_totum circuitus
     * ======================================================== */

    {
        chorda contentum;

        imprimere("\n--- Probans filum (circuitus) ---\n");

        contentum = filum_legere_totum(FIXUM "/plaga.txt", piscina);
        CREDO_AEQUALIS_I32 (contentum.mensura, XII);
        CREDO_CHORDA_AEQUALIS_LITERIS (contentum, "salve silva\n");

        contentum = filum_legere_totum(FIXUM "/abest.txt", piscina);
        CREDO_AEQUALIS_I32 (contentum.mensura, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: (d) via formae
     * ======================================================== */

    {
        chorda nomen_fili;
        chorda iuncta;
        chorda partes[II];

        imprimere("\n--- Probans viam (formae) ---\n");

        nomen_fili = via_nomen(
            chorda_ex_literis("/a/b/c.txt", piscina), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (nomen_fili, "c.txt");

        partes[ZEPHYRUM]  = chorda_ex_literis("silva", piscina);
        partes[I]         = chorda_ex_literis("fontes", piscina);
        iuncta            = via_iungere(partes, II, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (iuncta, "silva/fontes");
    }


    /* ========================================================
     * PROBARE: (e) utf8 sectio runarum (truncatio praevisus)
     * ======================================================== */

    {
        /* "a" + U+2500 + "b" = V octeti, III runae */
        hic_manens constans i8 LINEA[V] =
            { 'a', 0xE2, 0x94, 0x80, 'b' };
        constans i8* cursor;
        constans i8* finis;
                i32  runae;

        imprimere("\n--- Probans utf8 (sectio runarum) ---\n");

        finis   = LINEA + V;
        cursor  = LINEA;
        runae   = ZEPHYRUM;
        dum (cursor < finis && runae < II)
        {
            cursor = utf8_proxima_runa(cursor, finis);
            runae++;
        }
        /* II runae consumptae -> offset IV octetorum: sectio ad
         * latitudinem II columnarum finit ante 'b' */
        CREDO_AEQUALIS_PTR ((constans vacuum*)cursor,
            (constans vacuum*)(LINEA + IV));
        CREDO_AEQUALIS_S32 (utf8_numerare_runas(LINEA, V), III);
    }

    (vacuum)system("rm -rf " FIXUM);

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
