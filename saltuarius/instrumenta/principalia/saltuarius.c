/* saltuarius.c - Principale (Phase B: columnae + visus fontis)
 *
 * TENUE consulto: argumenta, pons posix, ansa eventuum, dispensatio
 * iussorum per modum. Limes disci in saltuarius_limes; visus/
 * exemplaria discum numquam tangunt. Cache librorum LRU (N=8) in
 * bibliotheca - reapertio = positio cursoris servata (donum
 * gratuitum cache).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include "tessera.h"
#include "saltuarius_res.h"
#include "saltuarius_tabularium.h"
#include "saltuarius_columnae.h"
#include "saltuarius_claves.h"
#include "saltuarius_limes.h"
#include "saltuarius_nexus.h"
#include "saltuarius_visum.h"
#include "saltuarius_bibliotheca.h"
#include "saltuarius_origo.h"
#include "saltuarius_structura.h"
#include "saltuarius_quaestio.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

#define NUNTIUS_QUADRA XII   /* ~3s ad moram 250ms */

interior constans character*
_literis (Piscina* piscina, chorda textus)
{
    character* buffer;

    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura + I);
    si (buffer == NIHIL)
    {
        redde "";
    }
    memcpy(buffer, textus.datum, (memoriae_index)textus.mensura);
    buffer[textus.mensura] = '\0';
    redde buffer;
}

/* Status totalis app (pro dispensatoribus) */
nomen structura {
    Piscina*                piscina;
    SaltuariusRes*          res;
    SaltuariusColumnae*     columnae;
    SaltuariusLimes*        limes;
    SaltuariusNexus*        nexus;
    SaltuariusBibliotheca   bibliotheca;
    SaltuariusLiber*        liber;      /* apertus in MODUS_FONS */
    SaltuariusOrigo*        origo;
    SaltuariusStructura*    index;      /* TOC; NB "structura" =
                                         * macro (struct)! */
    SaltuariusQuaestio      quaestio;   /* '/' (D2) */
    TesseraOpus*            opus;
} SaltuariusApp;

/* Praebenda ex compile_flags.txt seminare (conventio clangd:
 * lineae -I radix-relativae) - limes discum ambulat, nexus
 * iniecta accipit */
interior vacuum
_silvam_seminare (SaltuariusApp* app, SaltuariusLimes* limes,
    chorda radix)
{
    chorda partes[II];
    chorda via_cf;
    chorda textus;
    chorda viae[XVI];
    i32 numerus_viarum = ZEPHYRUM;

    partes[ZEPHYRUM] = radix;
    partes[I] = chorda_ex_literis("compile_flags.txt",
        app->piscina);
    via_cf = via_iungere(partes, II, app->piscina);
    textus = filum_legere_totum(_literis(app->piscina, via_cf),
        app->piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        redde;   /* sine compile_flags: parse sine praebendis */
    }
    {
        i32 initium = ZEPHYRUM;
        i32 k;

        per (k = ZEPHYRUM; k <= textus.mensura
            && numerus_viarum < XVI; k++)
        {
            si (k == textus.mensura || textus.datum[k] == '\n')
            {
                i32 mensura = k - initium;

                si (mensura > II
                    && textus.datum[initium] == '-'
                    && textus.datum[initium + I] == 'I')
                {
                    chorda relativa;

                    relativa.datum = textus.datum + initium + II;
                    relativa.mensura = mensura - II;
                    partes[ZEPHYRUM] = radix;
                    partes[I] = relativa;
                    viae[numerus_viarum] = via_iungere(partes, II,
                        app->piscina);
                    numerus_viarum++;
                }
                initium = k + I;
            }
        }
    }
    {
        SaltuariusCaput* capita = NIHIL;
        i32 numerus = saltuarius_limes_capita_legere(limes,
            app->piscina, viae, numerus_viarum, &capita);

        si (numerus > ZEPHYRUM)
        {
            (vacuum)saltuarius_nexus_silvam_parare(app->nexus,
                capita, numerus);
        }
    }
}

/* Ad locum ire: via + linea -> plagula aperta, stratum 0, cursor.
 * COMMUNIS origini et indici structurae (D1: unus saltus, duo
 * vocatores). */
interior vacuum
_ad_locum_ire (SaltuariusApp* app, chorda via, i32 linea)
{
    chorda via_meta;

    /* eadem plagula? */
    si (chorda_aequalis(via, app->liber->via))
    {
        saltuarius_liber_stratum_ponere(app->liber, ZEPHYRUM);
        app->liber->cursor_linea = (s32)linea - I;
        app->liber->cursor_columna = ZEPHYRUM;
        saltuarius_liber_movere(app->liber, ZEPHYRUM);
        redde;
    }
    /* resolutio: titulus praebendi -> absoluta; aut via absoluta */
    via_meta = saltuarius_nexus_fons_resolvere(app->nexus, via);
    si (via_meta.mensura == ZEPHYRUM
        && via.mensura > ZEPHYRUM
        && via.datum[ZEPHYRUM] == '/')
    {
        via_meta = via;
    }
    si (via_meta.mensura == ZEPHYRUM)
    {
        saltuarius_res_nuntiare(app->res, "(fons non resolutus)",
            NUNTIUS_QUADRA);
        redde;
    }
    {
        SaltuariusLiber* liber = saltuarius_bibliotheca_quaerere(
            &app->bibliotheca, via_meta);

        si (liber == NIHIL)
        {
            constans character* causa;

            liber = saltuarius_limes_librum_via(app->limes,
                app->piscina, app->nexus, via_meta, &causa);
            si (liber == NIHIL)
            {
                saltuarius_res_nuntiare(app->res, causa,
                    NUNTIUS_QUADRA);
                redde;
            }
            saltuarius_bibliotheca_condere(&app->bibliotheca,
                liber);
        }
        app->liber = liber;
        saltuarius_liber_stratum_ponere(liber, ZEPHYRUM);
        liber->cursor_linea = (s32)linea - I;
        liber->cursor_columna = ZEPHYRUM;
        saltuarius_liber_movere(liber, ZEPHYRUM);
    }
}

/* Saltus originis: gradus selectus -> plagula + linea */
interior vacuum
_saltum_facere (SaltuariusApp* app)
{
    constans SaltuariusGradus* gradus =
        saltuarius_origo_saltus(app->origo);

    si (gradus == NIHIL)
    {
        redde;
    }
    saltuarius_origo_claudere(app->origo);
    _ad_locum_ire(app, gradus->via, gradus->linea);
}

/* Saltus indicis structurae: inclusio -> IN plagulam (linea I);
 * definitio/regio -> linea plagulae currentis */
interior vacuum
_saltum_indicis (SaltuariusApp* app)
{
    constans SaltuariusOrdo* ordo =
        saltuarius_structura_saltus(app->index);

    si (ordo == NIHIL)
    {
        redde;
    }
    saltuarius_structura_claudere(app->index);
    commutatio (ordo->genus)
    {
        casus SALT_ORDO_INCLUSIO:
            _ad_locum_ire(app, ordo->via, I);
            frange;
        casus SALT_ORDO_DEFINITIO:
        casus SALT_ORDO_REGIO:
        casus SALT_ORDO_FUNCTIO:
        casus SALT_ORDO_PROTOTYPUM:
        casus SALT_ORDO_TYPUS:
        casus SALT_ORDO_DECLARATIO:
            _ad_locum_ire(app, app->liber->via, ordo->linea);
            frange;
        ordinarius:
            frange;
    }
}

/* Intrare (columnae): directorium aut librum aperire */
interior vacuum
_intrare (SaltuariusApp* app)
{
    constans DirectoriumIntroitus* selectum =
        saltuarius_tabularium_selectum(app->limes->tab_currens);

    si (selectum == NIHIL)
    {
        redde;
    }
    commutatio (selectum->genus)
    {
        casus INTROITUS_DIRECTORIUM:
            saltuarius_res_intrare(app->res, selectum->titulus);
            si (!saltuarius_limes_indices(app->limes, app->res,
                app->columnae, NIHIL))
            {
                saltuarius_res_nuntiare(app->res, "(non legibile)",
                    NUNTIUS_QUADRA);
            }
            saltuarius_limes_praevisum(app->limes, app->res,
                app->columnae);
            frange;
        casus INTROITUS_FILUM:
        {
            /* cache primo (via plena in arena persistenti -
             * stillicidium parvum per aperturam, notatum) */
            chorda partes[II];
            chorda via_plena;
            SaltuariusLiber* liber;
            constans character* causa;

            partes[ZEPHYRUM] = app->res->via_currens;
            partes[I] = selectum->titulus;
            via_plena = via_iungere(partes, II, app->piscina);
            liber = saltuarius_bibliotheca_quaerere(
                &app->bibliotheca, via_plena);
            si (liber == NIHIL)
            {
                /* parse 44-150ms: nuntius ANTE pingitur */
                saltuarius_res_nuntiare(app->res, "silva legit...",
                    I);
                saltuarius_columnae_pingere(app->columnae, app->res,
                    app->opus);
                (vacuum)tessera_praesentare(app->opus);
                liber = saltuarius_limes_librum_aperire(app->limes,
                    app->piscina, app->nexus, app->res, &causa);
                si (liber == NIHIL)
                {
                    saltuarius_res_nuntiare(app->res, causa,
                        NUNTIUS_QUADRA);
                    redde;
                }
                saltuarius_bibliotheca_condere(&app->bibliotheca,
                    liber);
            }
            app->liber = liber;
            app->res->modus = SALT_MODUS_FONS;
            frange;
        }
        ordinarius:
            saltuarius_res_nuntiare(app->res, "(non intrabile)",
                NUNTIUS_QUADRA);
            frange;
    }
}

interior vacuum
_ascendere (SaltuariusApp* app)
{
    chorda selectandum;

    si (chorda_aequalis_literis(app->res->via_currens, "/"))
    {
        redde;
    }
    selectandum = via_nomen(app->res->via_currens, app->piscina);
    saltuarius_res_ascendere(app->res);
    (vacuum)saltuarius_limes_indices(app->limes, app->res,
        app->columnae, &selectandum);
    saltuarius_limes_praevisum(app->limes, app->res, app->columnae);
}

interior vacuum
_clicum_columnarum (SaltuariusApp* app, s32 x, s32 y)
{
    SaltuariusMetra metra;

    saltuarius_columnae_metiri(tessera_latitudo(app->opus),
        tessera_altitudo(app->opus), &metra);
    si (y < metra.contentum_y
        || y >= metra.contentum_y + metra.contentum_alt)
    {
        redde;
    }
    si (x >= metra.currens_x
        && x < metra.currens_x + metra.currens_lat)
    {
        s32 index = app->limes->tab_currens->volumen
            + (y - metra.contentum_y);

        si (index < (s32)app->limes->tab_currens->numerus)
        {
            app->limes->tab_currens->selectio = index;
            saltuarius_limes_praevisum(app->limes, app->res,
                app->columnae);
        }
    }
    alioquin si (x < metra.parens_lat)
    {
        _ascendere(app);
    }
    alioquin si (x >= metra.praevisus_x)
    {
        _intrare(app);
    }
}

interior vacuum
_clicum_fontis (SaltuariusApp* app, s32 x, s32 y)
{
    SaltuariusVisumMetra metra;

    saltuarius_visum_metiri(app->liber,
        tessera_latitudo(app->opus), tessera_altitudo(app->opus),
        &metra);
    si (y >= metra.contentum_alt || x < metra.textus_x)
    {
        redde;
    }
    app->liber->cursor_linea = app->liber->volumen_y + y;
    app->liber->cursor_columna = app->liber->volumen_x
        + (x - metra.textus_x);
    saltuarius_liber_movere(app->liber, ZEPHYRUM);      /* claudere */
    saltuarius_liber_movere_col(app->liber, ZEPHYRUM);
}

/* Iussa in modo columnarum */
interior vacuum
_iussum_columnarum (SaltuariusApp* app,
    constans SaltuariusIussum* iussum)
{
    b32 praevisus_recens = FALSUM;

    commutatio (iussum->genus)
    {
        casus SALT_ACTIO_SURSUM:
        casus SALT_ACTIO_ROTA_SURSUM:
            saltuarius_tabularium_movere(app->limes->tab_currens,
                -I);
            praevisus_recens = VERUM;
            frange;
        casus SALT_ACTIO_DEORSUM:
        casus SALT_ACTIO_ROTA_DEORSUM:
            saltuarius_tabularium_movere(app->limes->tab_currens,
                I);
            praevisus_recens = VERUM;
            frange;
        casus SALT_ACTIO_PAGINA_SURSUM:
        casus SALT_ACTIO_PAGINA_DEORSUM:
        {
            SaltuariusMetra metra;
            s32 dimidium;

            saltuarius_columnae_metiri(tessera_latitudo(app->opus),
                tessera_altitudo(app->opus), &metra);
            dimidium = metra.contentum_alt / II;
            si (dimidium < I)
            {
                dimidium = I;
            }
            saltuarius_tabularium_movere(app->limes->tab_currens,
                (iussum->genus == SALT_ACTIO_PAGINA_SURSUM)
                    ? -dimidium : dimidium);
            praevisus_recens = VERUM;
            frange;
        }
        casus SALT_ACTIO_PRIMUM:
            saltuarius_tabularium_primum(app->limes->tab_currens);
            praevisus_recens = VERUM;
            frange;
        casus SALT_ACTIO_ULTIMUM:
            saltuarius_tabularium_ultimum(app->limes->tab_currens);
            praevisus_recens = VERUM;
            frange;
        casus SALT_ACTIO_SALTARE:
            si (saltuarius_tabularium_saltare(
                app->limes->tab_currens, iussum->runa))
            {
                praevisus_recens = VERUM;
            }
            frange;
        casus SALT_ACTIO_INTRARE:
            _intrare(app);
            frange;
        casus SALT_ACTIO_PARENS:
            _ascendere(app);
            frange;
        casus SALT_ACTIO_CLICUS:
            _clicum_columnarum(app, iussum->x, iussum->y);
            frange;
        casus SALT_ACTIO_FINIRE:
            app->res->currens = FALSUM;
            frange;
        ordinarius:
            frange;
    }
    si (praevisus_recens)
    {
        saltuarius_limes_praevisum(app->limes, app->res,
            app->columnae);
    }
}

interior b32
liber_multistratus (constans SaltuariusApp* app)
{
    redde (app->liber != NIHIL
        && app->liber->numerus_stratorum > I) ? VERUM : FALSUM;
}

/* Iussa dum tabella originis aperta (micro-modus) */
interior vacuum
_iussum_tabellae (SaltuariusApp* app,
    constans SaltuariusIussum* iussum)
{
    commutatio (iussum->genus)
    {
        casus SALT_ACTIO_SURSUM:
        casus SALT_ACTIO_ROTA_SURSUM:
            saltuarius_origo_movere(app->origo, -I);
            frange;
        casus SALT_ACTIO_DEORSUM:
        casus SALT_ACTIO_ROTA_DEORSUM:
            saltuarius_origo_movere(app->origo, I);
            frange;
        casus SALT_ACTIO_INTRARE:
            _saltum_facere(app);
            frange;
        casus SALT_ACTIO_FINIRE:
        casus SALT_ACTIO_ORIGO:
        casus SALT_ACTIO_CLICUS:   /* clicus extra = claudere (D3) */
            saltuarius_origo_claudere(app->origo);
            frange;
        ordinarius:
            frange;
    }
}

/* Iussa dum index structurae apertus (micro-modus alter; cum
 * tabella originis MUTUO EXCLUSIVUS - dispensatio ordinem
 * praestat: origo -> index -> fons) */
interior vacuum
_iussum_indicis (SaltuariusApp* app,
    constans SaltuariusIussum* iussum)
{
    commutatio (iussum->genus)
    {
        casus SALT_ACTIO_SURSUM:
        casus SALT_ACTIO_ROTA_SURSUM:
            saltuarius_structura_movere(app->index, -I);
            frange;
        casus SALT_ACTIO_DEORSUM:
        casus SALT_ACTIO_ROTA_DEORSUM:
            saltuarius_structura_movere(app->index, I);
            frange;
        casus SALT_ACTIO_PAGINA_SURSUM:
        casus SALT_ACTIO_PAGINA_DEORSUM:
        {
            s32 dimidium = ((s32)tessera_altitudo(app->opus) - III)
                / II;

            si (dimidium < I)
            {
                dimidium = I;
            }
            saltuarius_structura_movere(app->index,
                (iussum->genus == SALT_ACTIO_PAGINA_SURSUM)
                    ? -dimidium : dimidium);
            frange;
        }
        casus SALT_ACTIO_INTRARE:
            _saltum_indicis(app);
            frange;
        casus SALT_ACTIO_FINIRE:
        casus SALT_ACTIO_STRUCTURA:
        casus SALT_ACTIO_CLICUS:   /* clicus extra = claudere (D3) */
            saltuarius_structura_claudere(app->index);
            frange;
        ordinarius:
            frange;
    }
}

/* Iussa in modo fontis */
interior vacuum
_iussum_fontis (SaltuariusApp* app,
    constans SaltuariusIussum* iussum)
{
    commutatio (iussum->genus)
    {
        casus SALT_ACTIO_SURSUM:
        casus SALT_ACTIO_ROTA_SURSUM:
            saltuarius_liber_movere(app->liber, -I);
            frange;
        casus SALT_ACTIO_DEORSUM:
        casus SALT_ACTIO_ROTA_DEORSUM:
            saltuarius_liber_movere(app->liber, I);
            frange;
        casus SALT_ACTIO_SINISTRA:
            saltuarius_liber_movere_col(app->liber, -I);
            frange;
        casus SALT_ACTIO_DEXTRA:
            saltuarius_liber_movere_col(app->liber, I);
            frange;
        casus SALT_ACTIO_PAGINA_SURSUM:
        casus SALT_ACTIO_PAGINA_DEORSUM:
        {
            SaltuariusVisumMetra metra;
            s32 dimidium;

            saltuarius_visum_metiri(app->liber,
                tessera_latitudo(app->opus),
                tessera_altitudo(app->opus), &metra);
            dimidium = metra.contentum_alt / II;
            si (dimidium < I)
            {
                dimidium = I;
            }
            saltuarius_liber_movere(app->liber,
                (iussum->genus == SALT_ACTIO_PAGINA_SURSUM)
                    ? -dimidium : dimidium);
            frange;
        }
        casus SALT_ACTIO_PRIMUM:
            saltuarius_liber_primum(app->liber);
            frange;
        casus SALT_ACTIO_ULTIMUM:
            saltuarius_liber_ultimum(app->liber);
            frange;
        casus SALT_ACTIO_STRATUM:
            si (liber_multistratus(app))
            {
                saltuarius_liber_stratum_transferre(app->liber,
                    app->nexus,
                    (app->liber->stratum_currens + I)
                        % (s32)app->liber->numerus_stratorum);
            }
            alioquin
            {
                saltuarius_res_nuntiare(app->res,
                    "(stratum unicum)", NUNTIUS_QUADRA);
            }
            frange;
        casus SALT_ACTIO_ORIGO:
        casus SALT_ACTIO_INTRARE:
            si (!saltuarius_origo_aedificare(app->origo,
                app->liber, app->nexus))
            {
                saltuarius_res_nuntiare(app->res,
                    "(origo nihil narrat)", NUNTIUS_QUADRA);
            }
            frange;
        casus SALT_ACTIO_STRUCTURA:
            si (!saltuarius_structura_aedificare(app->index,
                app->liber))
            {
                saltuarius_res_nuntiare(app->res,
                    "(structura nihil habet)", NUNTIUS_QUADRA);
            }
            frange;
        casus SALT_ACTIO_QUAESTIO:
            saltuarius_quaestio_aperire(&app->quaestio,
                app->liber);
            frange;
        casus SALT_ACTIO_PROXIMUM:
        casus SALT_ACTIO_PRIUS:
        {
            b32 volvit;

            si (app->quaestio.commissum_mensura == ZEPHYRUM)
            {
                saltuarius_res_nuntiare(app->res,
                    "(quaestio vacua)", NUNTIUS_QUADRA);
            }
            alioquin si (!saltuarius_quaestio_proximum(
                &app->quaestio, app->liber,
                (iussum->genus == SALT_ACTIO_PROXIMUM) ? I : -I,
                &volvit))
            {
                saltuarius_res_nuntiare(app->res,
                    "(nihil inventum)", NUNTIUS_QUADRA);
            }
            alioquin si (volvit)
            {
                saltuarius_res_nuntiare(app->res, "(finis)",
                    NUNTIUS_QUADRA);
            }
            frange;
        }
        casus SALT_ACTIO_FRUCTUS:
            app->res->fructus_visibilis =
                !app->res->fructus_visibilis;
            frange;
        casus SALT_ACTIO_CLICUS:
            _clicum_fontis(app, iussum->x, iussum->y);
            frange;
        casus SALT_ACTIO_FINIRE:
            /* redire ad columnas (liber in cache manet) */
            saltuarius_origo_claudere(app->origo);
            saltuarius_structura_claudere(app->index);
            app->res->modus = SALT_MODUS_COLUMNAE;
            saltuarius_limes_praevisum(app->limes, app->res,
                app->columnae);
            frange;
        ordinarius:
            frange;
    }
}

s32 principale (s32 argc, character** argv)
{
    SaltuariusApp app;
    TesseraPiscina* tp;
    TesseraPons* pons;
    TesseraLector* lector;
    SaltuariusTabularium* tab_parens;
    SaltuariusTabularium* tab_currens;
    SaltuariusTabularium* tab_praevisus;
    SaltuariusClaves claves;

    app.piscina = piscina_generare_dynamicum("saltuarius",
        33554432);
    si (app.piscina == NIHIL)
    {
        fprintf(stderr, "saltuarius: piscina non generata\n");
        redde I;
    }
    app.liber = NIHIL;

    {
        chorda radix_abs = via_absoluta(
            chorda_ex_literis((argc > I) ? argv[I] : ".",
                app.piscina), app.piscina);

        app.res = saltuarius_res_creare(app.piscina,
            _literis(app.piscina, radix_abs));
    }
    app.columnae = saltuarius_columnae_creare(app.piscina);
    app.nexus = saltuarius_nexus_creare(app.piscina);
    tab_parens = saltuarius_tabularium_creare(app.piscina);
    tab_currens = saltuarius_tabularium_creare(app.piscina);
    tab_praevisus = saltuarius_tabularium_creare(app.piscina);
    app.limes = saltuarius_limes_creare(app.piscina, tab_parens,
        tab_currens, tab_praevisus);
    saltuarius_bibliotheca_parare(&app.bibliotheca);
    app.origo = saltuarius_origo_creare(app.piscina);
    app.index = saltuarius_structura_creare(app.piscina);
    saltuarius_quaestio_parare(&app.quaestio);
    si (app.res == NIHIL || app.columnae == NIHIL
        || app.nexus == NIHIL || app.limes == NIHIL
        || app.origo == NIHIL || app.index == NIHIL)
    {
        fprintf(stderr, "saltuarius: initium fractum\n");
        redde I;
    }
    saltuarius_claves_parare(&claves);

    tp = tessera_piscina_generare_dynamicum("saltuarius_tessera",
        16777216);
    pons = (tp != NIHIL) ? tessera_pons_posix_creare(tp) : NIHIL;
    si (pons == NIHIL)
    {
        fprintf(stderr,
            "saltuarius: terminal verum requiritur (isatty)\n");
        redde I;
    }
    app.opus = tessera_aperire(tp, pons);
    lector = tessera_lector_creare(tp, pons);
    si (app.opus == NIHIL || lector == NIHIL)
    {
        fprintf(stderr, "saltuarius: apertura fracta\n");
        redde I;
    }

    _silvam_seminare(&app, app.limes, app.res->radix);

    si (!saltuarius_limes_indices(app.limes, app.res, app.columnae,
        NIHIL))
    {
        saltuarius_res_nuntiare(app.res, "(radix non legibilis)",
            NUNTIUS_QUADRA);
    }
    saltuarius_limes_praevisum(app.limes, app.res, app.columnae);

    dum (app.res->currens)
    {
        TesseraEventum ev;
        SaltuariusIussum iussum;

        si (app.res->modus == SALT_MODUS_FONS
            && app.liber != NIHIL)
        {
            saltuarius_visum_pingere(app.liber, app.res, app.opus);
            si (app.origo->apertum)
            {
                saltuarius_visum_tabella(app.origo, app.opus);
            }
            si (app.index->apertum)
            {
                saltuarius_visum_structura(app.index, app.opus);
            }
            si (app.res->fructus_visibilis)
            {
                saltuarius_visum_fructus(app.liber,
                    app.bibliotheca.numerus, app.opus);
            }
            si (app.quaestio.activa)
            {
                saltuarius_visum_quaestio(&app.quaestio, app.opus);
            }
        }
        alioquin
        {
            saltuarius_columnae_pingere(app.columnae, app.res,
                app.opus);
            tessera_cursorem_ponere(app.opus, -I, -I);  /* celatus */
        }
        (vacuum)tessera_praesentare(app.opus);

        commutatio (tessera_eventum_expectare(lector, &ev, CCL))
        {
            casus TESSERA_EVENTUM_AMPLITUDO:
            casus TESSERA_EVENTUM_RESUMPTUM:
                (vacuum)tessera_magnitudinem_renovare(app.opus);
                frange;
            ordinarius:
                frange;
        }

        /* modus initus: eventa CRUDA ad quaestionem (claves
         * praeteritae - '/' solum ex _iussum_fontis attingitur,
         * ergo origo/index necessario clausi) */
        si (app.res->modus == SALT_MODUS_FONS
            && app.liber != NIHIL && app.quaestio.activa)
        {
            (vacuum)saltuarius_quaestio_tradere(&app.quaestio,
                app.liber, &ev);
        }
        alioquin si (saltuarius_claves_tradere(&claves,
            app.res->modus, &ev, &iussum))
        {
            si (app.res->modus == SALT_MODUS_FONS
                && app.liber != NIHIL && app.origo->apertum)
            {
                _iussum_tabellae(&app, &iussum);
            }
            alioquin si (app.res->modus == SALT_MODUS_FONS
                && app.liber != NIHIL && app.index->apertum)
            {
                _iussum_indicis(&app, &iussum);
            }
            alioquin si (app.res->modus == SALT_MODUS_FONS
                && app.liber != NIHIL)
            {
                _iussum_fontis(&app, &iussum);
            }
            alioquin
            {
                _iussum_columnarum(&app, &iussum);
            }
        }

        (vacuum)saltuarius_res_nuntius_senescere(app.res);
    }

    tessera_claudere(app.opus);
    saltuarius_bibliotheca_vacare(&app.bibliotheca);
    saltuarius_limes_destruere(app.limes);
    tessera_piscina_destruere(tp);
    piscina_destruere(app.piscina);
    redde ZEPHYRUM;
}
