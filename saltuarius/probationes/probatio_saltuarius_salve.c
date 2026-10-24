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
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
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


/* FIXURA SUITAE: arbor ficta in area suitae (credo_area: propria
 * cursus, post purgare deleta - olim /tmp fixum, cursibus simul
 * communis); sectiones (b) et (c) eam LEGUNT. */
nomen structura {
    character via[DXII];
} FixuraArboris;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
          FixuraArboris* fx;
     constans character* area = credo_area(c);
              character  mandatum[MXXIV];
              character  via[DXII];
                   FILE* f;

    fx = (FixuraArboris*)piscina_allocare(c->piscina,
        magnitudo(FixuraArboris));
    si (fx == NIHIL || area == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    sprintf(fx->via, "%s/arbor", area);
    sprintf(mandatum, "mkdir -p %s/subdir", fx->via);
    (vacuum)system(mandatum);
    sprintf(via, "%s/plaga.txt", fx->via);
    f = lectiones_fopen(via, "wb");
    CREDO_NON_NIHIL (f);
    si (f != NIHIL)
    {
        fputs("salve silva\n", f);
        fclose(f);
    }
    c->datum = fx;
}


/* ==================================================
 * PROBARE: (a) amalgama tesserae - hospes secundus verus
 * ================================================== */

interior vacuum
_amalgama_probare (
    CredoContextus* c)
{
    (vacuum)c;

    {
        TesseraPiscina* tp;
        TesseraPonsMemoriae* pm;
        TesseraOpus* opus;
        TesseraCellula cella;
        TesseraChorda captum;
        chorda visus;

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
            "\033[?2026h" "\033[?25l\033[0m\033[2J\033[1;1H\033[0msaltuarius"
            "\033[?2026l");

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
}


/* ==================================================
 * PROBARE: (b) iter_directoria in arbore ficta
 * ================================================== */

interior vacuum
_iter_probare (
    CredoContextus* c)
{
    constans FixuraArboris* fx = (constans FixuraArboris*)c->suita;
                   Piscina* piscina = c->piscina;

    si (fx == NIHIL)
    {
        redde;
    }
    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* intr;
                         b32  subdir_inventum  = FALSUM;
                         b32  plaga_inventa    = FALSUM;
                         i32  numerus          = ZEPHYRUM;

        iter = directorium_iterator_aperire(fx->via, piscina);
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
}


/* ==================================================
 * PROBARE: (c) filum_legere_totum circuitus
 * ================================================== */

interior vacuum
_filum_probare (
    CredoContextus* c)
{
    constans FixuraArboris* fx = (constans FixuraArboris*)c->suita;
                   Piscina* piscina = c->piscina;
                 character  via[DXII];

    si (fx == NIHIL)
    {
        redde;
    }
    {
        chorda contentum;

        sprintf(via, "%s/plaga.txt", fx->via);
        contentum = filum_legere_totum(via, piscina);
        CREDO_AEQUALIS_I32 (contentum.mensura, XII);
        CREDO_CHORDA_AEQUALIS_LITERIS (contentum, "salve silva\n");

        sprintf(via, "%s/abest.txt", fx->via);
        contentum = filum_legere_totum(via, piscina);
        CREDO_AEQUALIS_I32 (contentum.mensura, ZEPHYRUM);
    }
}


/* ==================================================
 * PROBARE: (d) via formae
 * ================================================== */

interior vacuum
_viam_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        chorda nomen_fili;
        chorda iuncta;
        chorda partes[II];

        nomen_fili = via_nomen(
            chorda_ex_literis("/a/b/c.txt", piscina), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (nomen_fili, "c.txt");

        partes[ZEPHYRUM]  = chorda_ex_literis("silva", piscina);
        partes[I]         = chorda_ex_literis("fontes", piscina);
        iuncta            = via_iungere(partes, II, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (iuncta, "silva/fontes");
    }
}


/* ==================================================
 * PROBARE: (e) utf8 sectio runarum (truncatio praevisus)
 * ================================================== */

interior vacuum
_utf8_probare (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* "a" + U+2500 + "b" = V octeti, III runae */
        hic_manens constans i8 LINEA[V] =
            { 'a', 0xE2, 0x94, 0x80, 'b' };
        constans i8* cursor;
        constans i8* finis;
                i32  runae;

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
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "(a) amalgama tesserae - hospes secundus verus",
        _amalgama_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "(b) iter_directoria in arbore ficta", _iter_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "(c) filum_legere_totum circuitus", _filum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "(d) via formae", _viam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "(e) utf8 sectio runarum", _utf8_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("saltuarius_salve", SECTIONES,
        _suitam_parare, NIHIL);
}
