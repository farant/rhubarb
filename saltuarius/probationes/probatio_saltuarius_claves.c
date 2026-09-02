/* probatio_saltuarius_claves.c - Dispensatio clavium (Chunk A1)
 *
 * Eventa tesserae MANU construuntur (structurae valorum - pons
 * nullus). Probantur: runae nudae, claves speciales, gg/f<littera>
 * status pendens (incl. abortus et purgatio), ctrl-d/u, mus,
 * eventa non-nostra (AMPLITUDO) reiecta.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "saltuarius_claves.h"
#include <stdio.h>
#include <string.h>

interior TesseraEventum
_ev_runa (
    s32 runa)
{
    TesseraEventum ev;

    memset(&ev, ZEPHYRUM, magnitudo(ev));
    ev.genus  = TESSERA_EVENTUM_CLAVIS;
    ev.runa   = runa;
    redde ev;
}

interior TesseraEventum
_ev_clavis (
    TesseraClavis clavis)
{
    TesseraEventum ev;

    memset(&ev, ZEPHYRUM, magnitudo(ev));
    ev.genus   = TESSERA_EVENTUM_CLAVIS;
    ev.clavis  = clavis;
    redde ev;
}

interior TesseraEventum
_ev_imperium (
    s32 runa)
{
    TesseraEventum ev = _ev_runa(runa);

    ev.modificatores = TESSERA_MODIFICATOR_IMPERIUM;
    redde ev;
}

s32
principale (vacuum)
{
                 b32  praeteritus;
             Piscina* piscina;
    SaltuariusClaves  claves;
    SaltuariusIussum  iussum;
      TesseraEventum  ev;

    piscina = piscina_generare_dynamicum("probatio_salt_claves",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    saltuarius_claves_parare(&claves);


    /* ========================================================
     * PROBARE: runae nudae + claves speciales
     * ======================================================== */

    {
        imprimere("\n--- Probans claves simplices ---\n");

        ev = _ev_runa((s32)'j');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_DEORSUM);

        ev = _ev_runa((s32)'k');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_SURSUM);

        ev = _ev_runa((s32)'q');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_FINIRE);

        ev = _ev_runa((s32)'G');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_ULTIMUM);

        ev = _ev_clavis(TESSERA_CLAVIS_DEORSUM);
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_DEORSUM);

        ev = _ev_clavis(TESSERA_CLAVIS_REDITUS);
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_INTRARE);

        ev = _ev_clavis(TESSERA_CLAVIS_SINISTRA);
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PARENS);

        /* runa ignota: nihil */
        ev = _ev_runa((s32)'z');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
    }


    /* ========================================================
     * PROBARE: gg - pendens, complementum, purgatio
     * ======================================================== */

    {
        imprimere("\n--- Probans gg ---\n");

        ev = _ev_runa((s32)'g');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_VERUM (claves.g_pendens);
        ev = _ev_runa((s32)'g');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PRIMUM);
        CREDO_FALSUM (claves.g_pendens);

        /* g deinde j: pendens purgatum, j NORMALITER agit */
        ev = _ev_runa((s32)'g');
        (vacuum)saltuarius_claves_tradere(&claves, SALT_MODUS_COLUMNAE,
            &ev, &iussum);
        ev = _ev_runa((s32)'j');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_DEORSUM);
        CREDO_FALSUM (claves.g_pendens);
    }


    /* ========================================================
     * PROBARE: f<littera> - saltus, abortus
     * ======================================================== */

    {
        imprimere("\n--- Probans f<littera> ---\n");

        ev = _ev_runa((s32)'f');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_VERUM (claves.f_pendens);
        ev = _ev_runa((s32)'p');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_SALTARE);
        CREDO_AEQUALIS_S32 (iussum.runa, (s32)'p');
        CREDO_FALSUM (claves.f_pendens);

        /* f deinde sagitta (runa nulla): saltus abortus */
        ev = _ev_runa((s32)'f');
        (vacuum)saltuarius_claves_tradere(&claves, SALT_MODUS_COLUMNAE,
            &ev, &iussum);
        ev = _ev_clavis(TESSERA_CLAVIS_SURSUM);
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_FALSUM (claves.f_pendens);

        /* post abortum: j iterum normalis */
        ev = _ev_runa((s32)'j');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_DEORSUM);

        /* f capit etiam claves navigationis ut litteras: f 'j'
         * saltat ad 'j', non movet */
        ev = _ev_runa((s32)'f');
        (vacuum)saltuarius_claves_tradere(&claves, SALT_MODUS_COLUMNAE,
            &ev, &iussum);
        ev = _ev_runa((s32)'j');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_SALTARE);
        CREDO_AEQUALIS_S32 (iussum.runa, (s32)'j');
    }


    /* ========================================================
     * PROBARE: ctrl-d / ctrl-u
     * ======================================================== */

    {
        imprimere("\n--- Probans ctrl-d/u ---\n");

        ev = _ev_imperium((s32)'d');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PAGINA_DEORSUM);

        ev = _ev_imperium((s32)'u');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PAGINA_SURSUM);

        /* ctrl-x ignotum */
        ev = _ev_imperium((s32)'x');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
    }


    /* ========================================================
     * PROBARE: mus + eventa non-nostra
     * ======================================================== */

    {
        imprimere("\n--- Probans murem ---\n");

        memset(&ev, ZEPHYRUM, magnitudo(ev));
        ev.genus      = TESSERA_EVENTUM_MUS;
        ev.mus_genus  = TESSERA_MUS_ROTA_SURSUM;
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_ROTA_SURSUM);

        memset(&ev, ZEPHYRUM, magnitudo(ev));
        ev.genus      = TESSERA_EVENTUM_MUS;
        ev.mus_genus  = TESSERA_MUS_PRESSUS;
        ev.mus_x      = V;
        ev.mus_y      = III;
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_CLICUS);
        CREDO_AEQUALIS_S32 (iussum.x, V);
        CREDO_AEQUALIS_S32 (iussum.y, III);

        /* solutio muris: nihil */
        memset(&ev, ZEPHYRUM, magnitudo(ev));
        ev.genus      = TESSERA_EVENTUM_MUS;
        ev.mus_genus  = TESSERA_MUS_SOLUTUS;
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));

        /* AMPLITUDO: principale tractat, non nos */
        memset(&ev, ZEPHYRUM, magnitudo(ev));
        ev.genus = TESSERA_EVENTUM_AMPLITUDO;
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev,
            &iussum));
    }


    /* ========================================================
     * PROBARE: modus fontis - h/l cursor, L/o strata/origo
     * ======================================================== */

    {
        imprimere("\n--- Probans modum fontis ---\n");

        ev = _ev_runa((s32)'h');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_SINISTRA);

        ev = _ev_runa((s32)'l');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_DEXTRA);

        ev = _ev_clavis(TESSERA_CLAVIS_SINISTRA);
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_SINISTRA);

        ev = _ev_runa((s32)'L');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_STRATUM);

        ev = _ev_runa((s32)'o');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_ORIGO);

        /* Tab in fonte: index structurae (D1); NB Ctrl+I ad Tab
         * aliatur - TABULAM ligamus, hoc probamus */
        ev = _ev_clavis(TESSERA_CLAVIS_TABULA);
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_STRUCTURA);

        /* '/' + n/N in fonte: quaestio (D2) */
        ev = _ev_runa((s32)'/');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_QUAESTIO);
        ev = _ev_runa((s32)'n');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PROXIMUM);
        ev = _ev_runa((s32)'N');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_PRIUS);

        /* F2 in fonte: linea fructus (D3); F3 nihil */
        memset(&ev, ZEPHYRUM, magnitudo(ev));
        ev.genus    = TESSERA_EVENTUM_CLAVIS;
        ev.clavis   = TESSERA_CLAVIS_FUNCTIO;
        ev.numerus  = II;
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_FRUCTUS);
        ev.numerus = III;
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));

        /* in columnis L/o/Tab/'/'/n nihil */
        ev = _ev_runa((s32)'L');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev, &iussum));
        ev = _ev_runa((s32)'o');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev, &iussum));
        ev = _ev_clavis(TESSERA_CLAVIS_TABULA);
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev, &iussum));
        ev = _ev_runa((s32)'/');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev, &iussum));
        ev = _ev_runa((s32)'n');
        CREDO_FALSUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_COLUMNAE, &ev, &iussum));

        /* q in fonte adhuc FINIRE (principale redire interpretatur) */
        ev = _ev_runa((s32)'q');
        CREDO_VERUM (saltuarius_claves_tradere(&claves,
            SALT_MODUS_FONS, &ev, &iussum));
        CREDO_AEQUALIS_I32 ((i32)iussum.genus,
            (i32)SALT_ACTIO_FINIRE);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
