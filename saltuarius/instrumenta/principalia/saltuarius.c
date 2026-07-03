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
    TesseraOpus*            opus;
} SaltuariusApp;

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
        casus SALT_ACTIO_ORIGO:
        casus SALT_ACTIO_INTRARE:
            saltuarius_res_nuntiare(app->res,
                "strata et origo: Phase C venit", NUNTIUS_QUADRA);
            frange;
        casus SALT_ACTIO_CLICUS:
            _clicum_fontis(app, iussum->x, iussum->y);
            frange;
        casus SALT_ACTIO_FINIRE:
            /* redire ad columnas (liber in cache manet) */
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
    si (app.res == NIHIL || app.columnae == NIHIL
        || app.nexus == NIHIL || app.limes == NIHIL)
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

        si (saltuarius_claves_tradere(&claves, app.res->modus, &ev,
            &iussum))
        {
            si (app.res->modus == SALT_MODUS_FONS
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
