/* terminale.c - applicatio terminalis (aemulator-plan E2, decisiones
 * XXIII-XXV). Vide terminale.h.
 *
 * Contextus PRIVATUS (actionibus et figurae communis): applicatio,
 * piscina scrutinii, clavis pendens, residuum rotulae - caput probatum
 * intactum manet.
 *
 * CLAVES: fons verus (fenestra, manus_ludus) clavem DEPRESSAM, deinde
 * TEXTUM eius, deinde LIBERATAM mittit; codificator par (clavis +
 * textus) ut seriem unam vult. Ergo clavis depressa PENDET: textus
 * sequens cum ea codificatur; aliud quodvis eventum eam solam prius
 * effundit (Enter, Ctrl-C, sagittae: textus nullus).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "mandatum.h"
#include "thema.h"
#include "color.h"
#include "stilus_terminalis.h"
#include "codificator_terminalis.h"
#include "aemulator.h"
#include "terminale.h"
#include <stdlib.h>
#include <string.h>

#define CELLULA_LATITUDO VI
#define CELLULA_ALTITUDO VIII
#define QUIES_MS         CCC

nomen structura {
     TerminaleApplicatio* app;
                 Piscina* scrutinium;
                 Eventus  pendens;
                     b32  pendet;
                     s32  rotula_residuum;
} TerminaleContextus;

/* xterm 0-15 (0x00RRGGBB) */
hic_manens constans i32 tabula_colorum[XVI] = {
    0x000000, 0xCD0000, 0x00CD00, 0xCDCD00,
    0x0000EE, 0xCD00CD, 0x00CDCD, 0xE5E5E5,
    0x7F7F7F, 0xFF0000, 0x00FF00, 0xFFFF00,
    0x5C5CFF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
};

/* gradus cubi 6x6x6 */
hic_manens constans i32 gradus_cubi[VI] = {
    0x00, 0x5F, 0x87, 0xAF, 0xD7, 0xFF
};


/* ==================================================
 * Colores
 * ================================================== */

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

/* 0x00RRGGBB -> color mandati RGBA = pixelum tabulae (mandatum.h) */
interior ColorMandati
color_rgb (
    i32 rgb)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_RGBA;
    cm.valor = color_ad_pixelum(color_ex_rgba((i8)((rgb >> XVI) & 0xFF),
        (i8)((rgb >> VIII) & 0xFF), (i8)(rgb & 0xFF), (i8)0xFF));
    redde cm;
}

/* index 0-255 -> 0x00RRGGBB (xterm) */
interior i32
tabulae_color (
    i32 index)
{
    i32 g;

    si (index < XVI)
    {
        redde tabula_colorum[index];
    }
    si (index < CCXXXII)
    {
        index -= XVI;
        redde (gradus_cubi[index / XXXVI] << XVI)
            | (gradus_cubi[(index / VI) % VI] << VIII)
            | gradus_cubi[index % VI];
    }
    g = VIII + (index - CCXXXII) * X;
    redde (g << XVI) | (g << VIII) | g;
}

/* color stili -> color mandati; nativus = signum thematis */
interior ColorMandati
colorem_resolvere (
    constans StilusColor* c,
                     b32  crassum,
              ColorThema  nativus)
{
    i32 index;

    commutatio (c->genus)
    {
        casus STILUS_COLOR_TABULA:
            index = c->valor & 0xFF;
            /* crassum: 0-7 clariores (xterm) */
            si (crassum && index < VIII)
            {
                index += VIII;
            }
            redde color_rgb(tabulae_color(index));
        casus STILUS_COLOR_RGB:
            redde color_rgb(c->valor);
        ordinarius:
            redde color_thematis(nativus);
    }
}


/* ==================================================
 * Figura
 * ================================================== */

/* <purus/> schirmum: VISUS hospitis, cellula per cellulam */
interior vacuum
terminale_figura (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
       TerminaleContextus* tc;
       constans Aemulator* a;
         AemulatorCellula  cellula;
          AemulatorCursor  cursor;
             ColorMandati  littera;
             ColorMandati  fundus;
                    Fines  f;
                      b32  inversum;
                      b32  fundus_proprius;
                      s32  cw;
                      s32  ch;
                      i32  x;
                      i32  y;

    (vacuum)c;
    (vacuum)thema;
    tc  = (TerminaleContextus*)ctx;
    a   = aemulator_hospes_aemulator(tc->app->hospes);
    cw  = (s32)tc->app->cellula_latitudo;
    ch  = (s32)tc->app->cellula_altitudo;
    per (y = ZEPHYRUM; y < aemulator_altitudo(a); y++)
    {
        per (x = ZEPHYRUM; x < aemulator_latitudo(a); x++)
        {
            si (   !aemulator_visus_cellula(a, x, y, &cellula)
                || cellula.latitudo == AEMULATOR_CAUDA
                || cellula.latitudo == AEMULATOR_CAPUT)
            {
                perge;
            }
            inversum = (cellula.stilus.ornamenta & STILUS_INVERSUM)
                != ZEPHYRUM;
            littera  = colorem_resolvere(&cellula.stilus.color_litterae,
                (cellula.stilus.ornamenta & STILUS_CRASSUM) != ZEPHYRUM,
                COLOR_TEXT);
            fundus   = colorem_resolvere(&cellula.stilus.color_fundi,
                FALSUM, COLOR_BACKGROUND);
            fundus_proprius = cellula.stilus.color_fundi.genus
                              != STILUS_COLOR_NATIVUS;
            si (inversum)
            {
                ColorMandati t;

                t                = littera;
                littera          = cellula.stilus.color_fundi.genus
                                   == STILUS_COLOR_NATIVUS
                                 ? color_thematis(COLOR_BACKGROUND)
                                 : fundus;
                fundus           = t;
                fundus_proprius  = VERUM;
            }
            si (fundus_proprius)
            {
                f.x = (s32)x * cw;
                f.y = (s32)y * ch;
                f.latitudo = cellula.latitudo
                    == AEMULATOR_LATA ? II * cw : cw;
                f.altitudo = ch;
                mandata_rectangulum(m, f, fundus, VERUM);
            }
            si (   cellula.graphema.mensura > ZEPHYRUM
                && (cellula.stilus.ornamenta & STILUS_INVISIBILE)
                    == ZEPHYRUM)
            {
                mandata_textus(m, (s32)x * cw, (s32)y * ch,
                    cellula.graphema,
                    ZEPHYRUM, littera);
            }
        }
    }
    /* cursor: solum in imo visus (vivum), visibilis */
    cursor = aemulator_cursor(a);
    si (aemulator_visus(a) == ZEPHYRUM && cursor.visibilis)
    {
        f.x         = (s32)cursor.x * cw;
        f.y         = (s32)cursor.y * ch;
        f.latitudo  = cw;
        f.altitudo  = ch;
        mandata_rectangulum(m, f, color_thematis(COLOR_CURSOR), VERUM);
        si (   aemulator_cellula(a, cursor.x, cursor.y, &cellula)
            && cellula.graphema.mensura > ZEPHYRUM)
        {
            mandata_textus(m, f.x, f.y, cellula.graphema, ZEPHYRUM,
                color_thematis(COLOR_BACKGROUND));
        }
    }
}


/* ==================================================
 * Actiones
 * ================================================== */

/* eventa[0..n) codificare et ad infantem mittere */
interior vacuum
eventa_mittere (
    TerminaleContextus* tc,
      constans Eventus* eventa,
                   i32  n)
{
      CodificatorModi  modi;
    ChordaAedificator* aed;
               chorda  octeti;

    memset(&modi, ZEPHYRUM, magnitudo(CodificatorModi));
    modi.cellula_latitudo = (s32)tc->app->cellula_latitudo;
    modi.cellula_altitudo = (s32)tc->app->cellula_altitudo;
    piscina_vacare(tc->scrutinium);
    aed = chorda_aedificator_creare(tc->scrutinium, LXIV);
    si (!aed)
    {
        redde;
    }
    (vacuum)codificator_eventa(&modi, eventa, n, aed);
    octeti = chorda_aedificator_spectare(aed);
    si (octeti.mensura > ZEPHYRUM)
    {
        (vacuum)aemulator_hospes_scribere(tc->app->hospes, octeti.datum,
            octeti.mensura);
    }
}

interior vacuum
pendentem_effundere (
    TerminaleContextus* tc)
{
    si (tc->pendet)
    {
        tc->pendet = FALSUM;
        eventa_mittere(tc, &tc->pendens, I);
    }
}

interior b32
terminale_clavis (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    TerminaleContextus* tc;
               Eventus  par[II];
                   s32  lineae;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    tc = (TerminaleContextus*)ctx;
    commutatio (ev->genus)
    {
        casus EVENTUS_CLAVIS_DEPRESSUS:
            pendentem_effundere(tc);
            tc->pendens  = *ev;
            tc->pendet   = VERUM;
            redde VERUM;
        casus EVENTUS_TEXTUS:
            si (   tc->pendet
                && ev->datum.textus.genus == EVENTUS_TEXTUS_COMMISSUM
                && ev->datum.textus.origo == EVENTUS_ORIGO_SCRIPTA)
            {
                par[ZEPHYRUM]  = tc->pendens;
                par[I]         = *ev;
                tc->pendet     = FALSUM;
                eventa_mittere(tc, par, II);
                redde VERUM;
            }
            pendentem_effundere(tc);
            eventa_mittere(tc, ev, I);
            redde VERUM;
        casus EVENTUS_CLAVIS_LIBERATUS:
            pendentem_effundere(tc);
            redde VERUM;
        casus EVENTUS_MUS_ROTULA:
            pendentem_effundere(tc);
            tc->rotula_residuum += ev->datum.rotula.dy;
            lineae = tc->rotula_residuum
                   / (s32)tc->app->cellula_altitudo;
            tc->rotula_residuum -= lineae
                                 * (s32)tc->app->cellula_altitudo;
            si (lineae != ZEPHYRUM)
            {
                aemulator_hospes_visum_movere(tc->app->hospes, lineae);
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}


/* ==================================================
 * Componere
 * ================================================== */

interior s32
superficies (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  ordinarium)
{
    chorda* v;
       s32  n;

    v = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (!v || !chorda_ut_s32(*v, &n) || n < I)
    {
        redde ordinarium;
    }
    redde n;
}

/* <componens/> radix + schirmum (PARTES_CAMPUS) superficiem totam
 * tegens; focusabile, actio "terminale.clavis" */
interior Componens*
terminale_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
              TerminaleContextus* tc;
              constans Aemulator* a;
                       Componens* radix;
                       Componens* schirmum;
                           Fines  f;

    (vacuum)motus;
    tc   = (TerminaleContextus*)ctx;
    a    = aemulator_hospes_aemulator(tc->app->hospes);
    f.x  = ZEPHYRUM;
    f.y  = ZEPHYRUM;
    f.latitudo  = superficies(repo, "superficies_latitudo",
        (s32)(aemulator_latitudo(a) * tc->app->cellula_latitudo));
    f.altitudo  = superficies(repo, "superficies_altitudo",
        (s32)(aemulator_altitudo(a) * tc->app->cellula_altitudo));
    radix = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    schirmum = componens_creare(piscina, intern, "schirmum",
        PARTES_CAMPUS);
    si (!radix || !schirmum)
    {
        redde radix;
    }
    componens_ponere_fines(radix, f);
    componens_ponere_fines(schirmum, f);
    componens_ponere_focusabilis(schirmum, VERUM);
    componens_ponere_actio(schirmum, "terminale.clavis");
    componens_addere_liberum(radix, schirmum);
    redde radix;
}


/* ==================================================
 * Applicatio
 * ================================================== */

constans character* constans*
terminale_argumenta (
     Piscina*  piscina,
         s32   argc,
   character** argv,
         b32*  fumus)
{
     constans character** v;
     constans character*  concha;
                    s32   i;

    *fumus = FALSUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            *fumus = VERUM;
        }
    }
    v = (constans character**)piscina_allocare(piscina,
        IV * magnitudo(character*));
    si (*fumus)
    {
        /* imago certa: colores, inversum, sublineatum, lineae */
        v[ZEPHYRUM]  = "/bin/sh";
        v[I]         = "-c";
        v[II]        = "printf 'terminale \\033[1;32mfumus\\033[0m\\n"
            "\\033[31mrubrum \\033[32mviride \\033[34mcaeruleum"
            "\\033[0m\\n\\033[7m inversum \\033[0m \\033[44m fundus "
            "\\033[0m\\n'; for i in 1 2 3; do printf 'linea %s\\n' $i;"
            " done; sleep 5";
        v[III]       = NIHIL;
        redde v;
    }
    concha       = getenv("SHELL");
    v[ZEPHYRUM]  = (concha && concha[ZEPHYRUM]) ? concha : "/bin/zsh";
    v[I]         = "-l";
    v[II]        = NIHIL;
    redde v;
}

b32
terminale_applicatio_aedificare (
    TerminaleApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
        Pseudoterminale* pt,
                    i32  latitudo,
                    i32  altitudo)
{
     AemulatorHospesConfiguratio  cfg;
              TerminaleContextus* tc;

    si (!app || !piscina || !intern || !pt)
    {
        si (pt)
        {
            pt->claudere(pt->datum);
        }
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(TerminaleApplicatio));
    app->piscina           = piscina;
    app->intern            = intern;
    app->cellula_latitudo  = CELLULA_LATITUDO;
    app->cellula_altitudo  = CELLULA_ALTITUDO;
    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.latitudo = latitudo / CELLULA_LATITUDO > ZEPHYRUM
                           ? latitudo / CELLULA_LATITUDO : I;
    cfg.aemulator.altitudo = altitudo / CELLULA_ALTITUDO > ZEPHYRUM
                           ? altitudo / CELLULA_ALTITUDO : I;
    app->hospes = aemulator_hospes_creare(piscina, &cfg, pt);
    tc = (TerminaleContextus*)piscina_conari_allocare(piscina,
        magnitudo(TerminaleContextus));
    si (!app->hospes || !tc)
    {
        redde FALSUM;
    }
    memset(tc, ZEPHYRUM, magnitudo(TerminaleContextus));
    tc->app         = app;
    tc->scrutinium  = piscina_generare_dynamicum("terminale_scrutinium",
        LXIV * MXXIV);
    app->repo = insula_repositorium_creare(piscina, intern,
        "<terminale/>", "<terminale focus=\"schirmum\"/>");
    app->actiones  = actio_registrum_creare(piscina, intern);
    app->figurae   = figura_registrum_creare(piscina);
    si (   !tc->scrutinium || !app->repo || !app->actiones
        || !app->figurae
        || !actio_registrare(app->actiones, "terminale.clavis",
               terminale_clavis, tc)
        || !figura_registrare(app->figurae, PARTES_CAMPUS, ZEPHYRUM,
               terminale_figura, tc))
    {
        redde FALSUM;
    }
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->actiones, terminale_componere, tc, QUIES_MS);
    redde app->d != NIHIL;
}

AemulatorHospesPulsus
terminale_pulsare (
    TerminaleApplicatio* app,
                    s32  mora_ms)
{
    constans Aemulator* a;
                   s32  lat;
                   s32  alt;
                   i32  columnae;
                   i32  lineae;

    a    = aemulator_hospes_aemulator(app->hospes);
    lat  = superficies(app->repo, "superficies_latitudo",
        (s32)(aemulator_latitudo(a) * app->cellula_latitudo));
    alt  = superficies(app->repo, "superficies_altitudo",
        (s32)(aemulator_altitudo(a) * app->cellula_altitudo));
    columnae  = (i32)lat / app->cellula_latitudo;
    lineae    = (i32)alt / app->cellula_altitudo;
    si (columnae < I)
    {
        columnae = I;
    }
    si (lineae < I)
    {
        lineae = I;
    }
    si (   columnae != aemulator_latitudo(a)
        || lineae   != aemulator_altitudo(a))
    {
        (vacuum)aemulator_hospes_amplitudo(app->hospes, columnae,
            lineae, (i32)lat, (i32)alt);
    }
    redde aemulator_hospes_pulsare(app->hospes, mora_ms);
}

vacuum
terminale_claudere (
    TerminaleApplicatio* app)
{
    si (app && app->hospes)
    {
        aemulator_hospes_claudere(app->hospes);
    }
}
