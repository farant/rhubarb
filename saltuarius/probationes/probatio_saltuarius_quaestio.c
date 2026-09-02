/* probatio_saltuarius_quaestio.c - Quaestio incrementalis '/'
 * (Chunk D2)
 *
 * Sessiones clavium SCRIPTAE: eventa manu constructa in
 * quaestio_tradere fluunt. Probantur: angustatio incrementalis
 * (i -> in -> int: cursor ex "#defIne" ad "int" salit),
 * RETRORSUM relaxat (et runam INTEGRAM UTF-8 delet), REDITUS
 * committit + n/N cum volutione, FUGA originem restituit, nihil
 * inventum insignitur, QUAERE-QUOD-VIDES in strato I (textus
 * expansus solum), aureum lineae echo.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_quaestio.h"
#include "saltuarius_visum.h"
#include "saltuarius_proba.h"
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

/* Literis dactylographare (ASCII; runae singulae traduntur) */
interior vacuum
_dactylographare (
    SaltuariusQuaestio* quaestio,
       SaltuariusLiber* liber,
    constans character* literis)
{
    i32 k;

    per (k = ZEPHYRUM; literis[k] != '\0'; k++)
    {
        TesseraEventum ev = _ev_runa((s32)literis[k]);

        (vacuum)saltuarius_quaestio_tradere(quaestio, liber, &ev);
    }
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
       SaltuariusNexus* nexus;
       SaltuariusLiber* liber;
    SaltuariusQuaestio  quaestio;
    /* offsets: 'i' prima in "#defIne" = 4; "int alpha" = 23;
     * "int beta" = 34; "int gamma" = 44 */
    constans character* FONS =
        "#define G(x) ((x)+(x))\n"
        "int alpha;\n"
        "int beta;\n"
        "int gamma;\n"
        "G(2);\n";

    piscina = piscina_generare_dynamicum("probatio_salt_quaestio",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    CREDO_NON_NIHIL (nexus);
    CREDO_AEQUALIS_S32 (
        saltuarius_nexus_silvam_parare(nexus, NIHIL, ZEPHYRUM),
        ZEPHYRUM);
    liber = saltuarius_liber_aperire(piscina, nexus,
        chorda_ex_literis("probe.c", piscina),
        chorda_ex_literis(FONS, piscina));
    CREDO_NON_NIHIL (liber);
    CREDO_NON_NIHIL (liber->parsura);
    saltuarius_quaestio_parare(&quaestio);


    /* ========================================================
     * PROBARE: angustatio incrementalis + RETRORSUM relaxat
     * ======================================================== */

    {
        imprimere("\n--- Probans angustationem ---\n");

        saltuarius_liber_primum(liber);
        saltuarius_quaestio_aperire(&quaestio, liber);
        CREDO_VERUM (quaestio.activa);

        _dactylographare(&quaestio, liber, "i");
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            IV);
        _dactylographare(&quaestio, liber, "n");
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            IV);
        _dactylographare(&quaestio, liber, "t");
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XXIII);
        CREDO_FALSUM (quaestio.nihil_inventum);

        /* retrorsum: "in" iterum - relaxat ad 4 */
        {
            TesseraEventum ev = _ev_clavis(
                TESSERA_CLAVIS_RETRORSUM);

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }
        CREDO_AEQUALIS_I32 (quaestio.mensura, II);
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            IV);

        /* iterum 't' et committere */
        _dactylographare(&quaestio, liber, "t");
        {
            TesseraEventum ev = _ev_clavis(TESSERA_CLAVIS_REDITUS);

            CREDO_AEQUALIS_I32 (
                (i32)saltuarius_quaestio_tradere(&quaestio, liber,
                    &ev),
                (i32)SALT_QUAESTIO_COMMISSA);
        }
        CREDO_FALSUM (quaestio.activa);
        CREDO_AEQUALIS_I32 (quaestio.commissum_mensura, III);
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XXIII);
    }


    /* ========================================================
     * PROBARE: n/N cum volutione
     * ======================================================== */

    {
        b32 volvit;

        imprimere("\n--- Probans n/N ---\n");

        CREDO_VERUM (saltuarius_quaestio_proximum(&quaestio, liber,
            I, &volvit));
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XXXIV);
        CREDO_FALSUM (volvit);
        CREDO_VERUM (saltuarius_quaestio_proximum(&quaestio, liber,
            I, &volvit));
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XLIV);
        CREDO_FALSUM (volvit);

        /* ultra ultimum: volvit ad primum */
        CREDO_VERUM (saltuarius_quaestio_proximum(&quaestio, liber,
            I, &volvit));
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XXIII);
        CREDO_VERUM (volvit);

        /* N citra primum: volvit ad ultimum */
        CREDO_VERUM (saltuarius_quaestio_proximum(&quaestio, liber,
            -I, &volvit));
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XLIV);
        CREDO_VERUM (volvit);
        CREDO_VERUM (saltuarius_quaestio_proximum(&quaestio, liber,
            -I, &volvit));
        CREDO_AEQUALIS_S32 (saltuarius_liber_cursor_offset(liber),
            XXXIV);
        CREDO_FALSUM (volvit);
    }


    /* ========================================================
     * PROBARE: nihil inventum + FUGA restituit
     * ======================================================== */

    {
        s32 linea_ante;
        s32 columna_ante;

        imprimere("\n--- Probans fugam ---\n");

        liber->cursor_linea    = II;
        liber->cursor_columna  = IV;
        saltuarius_liber_movere(liber, ZEPHYRUM);
        linea_ante    = liber->cursor_linea;
        columna_ante  = liber->cursor_columna;

        saltuarius_quaestio_aperire(&quaestio, liber);
        _dactylographare(&quaestio, liber, "zzz");
        CREDO_VERUM (quaestio.nihil_inventum);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, linea_ante);
        {
            TesseraEventum ev = _ev_clavis(TESSERA_CLAVIS_FUGA);

            CREDO_AEQUALIS_I32 (
                (i32)saltuarius_quaestio_tradere(&quaestio, liber,
                    &ev),
                (i32)SALT_QUAESTIO_CANCELLATA);
        }
        CREDO_FALSUM (quaestio.activa);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, linea_ante);
        CREDO_AEQUALIS_S32 (liber->cursor_columna, columna_ante);
        /* commissum prius INTACTUM (memoria n/N vivit) */
        CREDO_AEQUALIS_I32 (quaestio.commissum_mensura, III);
    }


    /* ========================================================
     * PROBARE: RETRORSUM runam INTEGRAM delet (UTF-8)
     * ======================================================== */

    {
        imprimere("\n--- Probans runam UTF-8 ---\n");

        saltuarius_quaestio_aperire(&quaestio, liber);
        {
            TesseraEventum ev = _ev_runa(0x3C0);   /* pi: 2 bytes */

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }
        CREDO_AEQUALIS_I32 (quaestio.mensura, II);
        {
            TesseraEventum ev = _ev_clavis(
                TESSERA_CLAVIS_RETRORSUM);

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }
        CREDO_AEQUALIS_I32 (quaestio.mensura, ZEPHYRUM);
        CREDO_FALSUM (quaestio.nihil_inventum);
        {
            TesseraEventum ev = _ev_clavis(TESSERA_CLAVIS_FUGA);

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }
    }


    /* ========================================================
     * PROBARE: quaere quod vides (stratum I)
     * ======================================================== */

    {
        imprimere("\n--- Probans stratum ostensum ---\n");

        CREDO_MAIOR_I32 ((i32)liber->numerus_stratorum, I);
        saltuarius_liber_stratum_transferre(liber, nexus, I);
        saltuarius_liber_primum(liber);
        saltuarius_quaestio_aperire(&quaestio, liber);
        _dactylographare(&quaestio, liber, "2)+(2");
        CREDO_FALSUM (quaestio.nihil_inventum);
        {
            constans SaltuariusStratum* visus =
                saltuarius_liber_stratum_activum(liber);
            s32 offset = saltuarius_liber_cursor_offset(liber);

            CREDO_VERUM (offset >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)visus->textus.datum[offset],
                (i32)'2');
        }
        {
            TesseraEventum ev = _ev_clavis(TESSERA_CLAVIS_FUGA);

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }

        /* in strato 0 eadem quaestio NIHIL invenit */
        saltuarius_liber_stratum_transferre(liber, nexus,
            ZEPHYRUM);
        saltuarius_liber_primum(liber);
        saltuarius_quaestio_aperire(&quaestio, liber);
        _dactylographare(&quaestio, liber, "2)+(2");
        CREDO_VERUM (quaestio.nihil_inventum);
        {
            TesseraEventum ev = _ev_clavis(TESSERA_CLAVIS_FUGA);

            (vacuum)saltuarius_quaestio_tradere(&quaestio, liber,
                &ev);
        }
    }


    /* ========================================================
     * PROBARE: aureum lineae echo
     * ======================================================== */

    {
        TesseraPiscina* tp;
        TesseraPonsMemoriae* pm;
        TesseraOpus* opus;
        SaltuariusRes* res;

        imprimere("\n--- Probans lineam echo ---\n");

        tp = tessera_piscina_generare_dynamicum(
            "salt_quaestio_tessera", 16777216);
        pm    = tessera_pons_memoriae_creare(tp, XL, VIII);
        opus  = tessera_aperire(tp, &pm->pons);
        res   = saltuarius_res_creare(piscina, "/probe");
        CREDO_NON_NIHIL (opus);
        CREDO_NON_NIHIL (res);

        saltuarius_quaestio_aperire(&quaestio, liber);
        _dactylographare(&quaestio, liber, "int");
        saltuarius_visum_pingere(liber, res, opus);
        saltuarius_visum_quaestio(&quaestio, opus);
        CREDO_VERUM (saltuarius_proba_quaerere(opus, VII, "/int")
            >= ZEPHYRUM);

        /* insigne (nihil) */
        _dactylographare(&quaestio, liber, "zzz");
        saltuarius_visum_pingere(liber, res, opus);
        saltuarius_visum_quaestio(&quaestio, opus);
        CREDO_VERUM (saltuarius_proba_quaerere(opus, VII,
            "(nihil)") >= ZEPHYRUM);

        tessera_claudere(opus);
        tessera_piscina_destruere(tp);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
