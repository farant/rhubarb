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
#include "glyphae_ductae.h"
#include "utf8.h"
#include "terminale.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CELLULA_LATITUDO VI
#define CELLULA_ALTITUDO VIII
#define QUIES_MS         CCC

#define TITULUS_MAXIMUS  CCLVI
/* CONTRASTUS MINIMUS (WCAG 2.0, ut Ghostty minimum-contrast; Franus
 * 2026-10-07: III.0, propulsio minima): programmata fundum obscurum
 * putant (Claude Code: grisei pallidi), thema nostrum lucidum est */
#define CONTRASTUS_MINIMUS  3.0
/* larva glyphae ductae (D7c): cellula maxima quam ducimus; maior =
 * textus ut antea */
#define LARVA_MAXIMA        (XXXII * LXIV)
/* ambitus infantis (vicus-latera S1b: ex applicatione in
 * bibliothecam motus, ut montatio in hospite eundem habeat) */
hic_manens constans character* constans ambitus_terminalis[] = {
    "TERM=xterm-256color",
    "COLORTERM=truecolor",
    "TERM_PROGRAM=terminale",
    NIHIL
};

/* canones montationis (vicus-latera S1b): radix hospitis liberos non
 * declarat, ergo montatio canonem suum fert (insula rami: canon
 * radicis ramos cum canone proprio non videt). Infixi, non e filo:
 * terminale_montare radicem viarum non accipit, et parvi sunt. */
#define CANON_DURABILIS \
    "<canon dialectus=\"terminale-durabilis\" versio=\"1\">" \
    "<elementum nomen=\"terminale\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "</elementum></canon>"
#define CANON_EPHEMERA \
    "<canon dialectus=\"terminale-ephemera\" versio=\"1\">" \
    "<elementum nomen=\"terminale\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<attributum nomen=\"focus\" genus=\"textus\"/>" \
    "<attributum nomen=\"focus_acervus\" genus=\"textus\"/>" \
    "<attributum nomen=\"superficies_latitudo\"" \
    " genus=\"numerus\"/>" \
    "<attributum nomen=\"superficies_altitudo\"" \
    " genus=\"numerus\"/>" \
    "</elementum></canon>"

/* SGR 2: littera ad fundum mixta (Ghostty faint-opacity 0.5) */
#define OBSCURUM_OPACITAS   0.5

nomen structura {
     TerminaleApplicatio* app;
                 Piscina* scrutinium;
                 Eventus  pendens;
                     b32  pendet;
                     s32  rotula_residuum;
    /* D6b: focus fenestrae (ordinarie VERUM, Ghostty) et ?1004 in
     * pulsu ultimo (positum -> relatio statim) */
                     b32 focus;
                     b32 focus_relatus;
               character titulus[TITULUS_MAXIMUS];
                     i32 titulus_mensura;
                     b32 titulus_mutatus;
    /* colores thematis in configuratione: nativus non mutatus = signum
     * thematis (thema vivum) */
                     i32 littera_thematis;
                     i32 fundus_thematis;
                     i32 cursor_thematis;
} TerminaleContextus;


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

/* color thematis -> 0x00RRGGBB (configuratio aemulatoris) */
interior i32
rgb_thematis (
    ColorThema c)
{
    Color k;

    k = thema_color(c);
    redde ((i32)(insignatus character)k.r << XVI)
        | ((i32)(insignatus character)k.g << VIII)
        | (i32)(insignatus character)k.b;
}

/* color dynamicus (litterae, fundus, cursor): non mutatus ab
 * programmate = signum thematis (thema vivum), aliter RGB vivum */
interior ColorMandati
colorem_dynamicum (
    constans TerminaleContextus* tc,
                            i32  index,
                            i32  thematis,
                     ColorThema  signum)
{
    i32 vivus;

    vivus = aemulator_color(aemulator_hospes_aemulator(tc->app->hospes),
        index);
    si (vivus == thematis)
    {
        redde color_thematis(signum);
    }
    redde color_rgb(vivus);
}


/* ==================================================
 * Contrastus (WCAG 2.0 relative luminance)
 * ================================================== */

/* park 011 (mensura: contrastus = XXII% quadri btop, pow() in
 * quaestione bipartita): canalis in tabula CCLVI valorum EADEM formula
 * semel computata (valores identici), et eventus contrastum_curare in
 * memoria parva per par (littera, fundus) - functio pura est */
#define MEMORIA_CONTRASTUS  CCLVI

nomen structura {
    i32 littera;
    i32 fundus;
    i32 exitus;
    b32 valida;
} ContrastusMemoria;

hic_manens f64               tabula_linearis[CCLVI];
hic_manens b32               tabula_linearis_parata = FALSUM;
hic_manens ContrastusMemoria memoria_contrastus[MEMORIA_CONTRASTUS];

interior f64
canalis_linearis (
    i32 c)
{
    f64 v;
    i32 k;

    si (!tabula_linearis_parata)
    {
        per (k = ZEPHYRUM; k < CCLVI; k++)
        {
            v = (f64)k / 255.0;
            tabula_linearis[k] = v <= 0.03928
                ? v / 12.92 : pow((v + 0.055) / 1.055, 2.4);
        }
        tabula_linearis_parata = VERUM;
    }
    redde tabula_linearis[c & 0xFF];
}

interior f64
luminantia_rgb (
    i32 rgb)
{
    redde 0.2126 * canalis_linearis((rgb >> XVI) & 0xFF)
         + 0.7152 * canalis_linearis((rgb >> VIII) & 0xFF)
         + 0.0722 * canalis_linearis(rgb & 0xFF);
}

interior f64
ratio_contrastus (
    i32 a,
    i32 b)
{
    f64 la;
    f64 lb;

    la = luminantia_rgb(a);
    lb = luminantia_rgb(b);
    redde la > lb ? (la + 0.05) / (lb + 0.05)
                  : (lb + 0.05) / (la + 0.05);
}

/* a ad b per t (0 = a, 1 = b), canalis per canalem rotundatus */
interior i32
rgb_miscere (
    i32 a,
    i32 b,
    f64 t)
{
    i32 rgb;
    i32 k;
    f64 x;
    f64 y;

    rgb = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= XVI; k += VIII)
    {
        x    = (f64)((a >> k) & 0xFF);
        y    = (f64)((b >> k) & 0xFF);
        rgb  |= ((i32)(x + (y - x) * t + 0.5) & 0xFF) << k;
    }
    redde rgb;
}

/* contrastus minimus per propulsionem MINIMAM (Franus; Ghostty ad
 * nigrum/album saltat): versus extremum quod plus contrastus dare
 * potest, quaestio bipartita, deinde gradus post rotundationem */
interior i32
contrastum_computare (
    i32 littera,
    i32 fundus)
{
    i32 meta;
    i32 k;
    f64 imum;
    f64 summum;
    f64 medium;

    si (ratio_contrastus(littera, fundus) >= CONTRASTUS_MINIMUS)
    {
        redde littera;
    }
    meta = ratio_contrastus(0xFFFFFF, fundus)
               > ratio_contrastus(ZEPHYRUM, fundus)
         ? 0xFFFFFF : ZEPHYRUM;
    imum    = 0.0;
    summum  = 1.0;
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        medium = (imum + summum) / 2.0;
        si (   ratio_contrastus(rgb_miscere(littera, meta, medium),
            fundus)
            >= CONTRASTUS_MINIMUS)
        {
            summum = medium;
        }
        alioquin
        {
            imum = medium;
        }
    }
    dum (   summum < 1.0
         && ratio_contrastus(rgb_miscere(littera, meta, summum), fundus)
            < CONTRASTUS_MINIMUS)
    {
        summum += 1.0 / 256.0;
    }
    redde rgb_miscere(littera, meta, summum > 1.0 ? 1.0 : summum);
}

/* contrastum_computare per memoriam (directe mappata; collisio
 * superscribit) */
interior i32
contrastum_curare (
    i32 littera,
    i32 fundus)
{
    ContrastusMemoria* m;
                  i32  h;

    /* mixtura multiplicativa, octetus summus = index (XOR octetorum
     * colores permutatos ut (r,g,b)/(b,g,r) confundebat) */
    h = (littera * 0x9E3779B1) ^ (fundus * 0x85EBCA77);
    h ^= h >> XV;
    h *= 0x2C1B3C6D;
    m = &memoria_contrastus[(h >> XXIV) % MEMORIA_CONTRASTUS];
    si (!m->valida || m->littera != littera || m->fundus != fundus)
    {
        m->littera  = littera;
        m->fundus   = fundus;
        m->exitus   = contrastum_computare(littera, fundus);
        m->valida   = VERUM;
    }
    redde m->exitus;
}

/* color mandati -> 0xRRGGBB (signum thematis per thema; RGBA = pixelum
 * a<<24|b<<16|g<<8|r) */
interior i32
rgb_mandati (
    ColorMandati c)
{
    si (c.genus == COLOR_MANDATI_THEMA)
    {
        redde rgb_thematis((ColorThema)c.valor);
    }
    redde ((c.valor & 0xFF) << XVI) | (c.valor & 0xFF00)
         | ((c.valor >> XVI) & 0xFF);
}

/* littera super fundum (0xRRGGBB): obscurum mixtum, deinde contrastus
 * minimus - nisi graphica (lineae capsarum, quadra: Ghostty
 * noMinContrast, color figurae ipse est); immutata manet ipsa (signum
 * thematis vivum manet) */
interior ColorMandati
litteram_legibilem (
    ColorMandati littera,
             i32 fundus,
             b32 obscurum,
             b32 graphica)
{
    i32 rgb;
    i32 novum;

    rgb = rgb_mandati(littera);
    novum = obscurum ? rgb_miscere(rgb, fundus,
        OBSCURUM_OPACITAS) : rgb;
    novum = graphica ? novum : contrastum_curare(novum, fundus);
    si (novum == rgb && !obscurum)
    {
        redde littera;
    }
    redde color_rgb(novum);
}

/* DECSCNM ?5 (D7c): colores NATIVI permutantur, ut Ghostty
 * (render.zig reverse_colors); expliciti et cursor manent */
interior b32
schirmus_inversus (
    constans TerminaleContextus* tc)
{
    redde aemulator_modi(aemulator_hospes_aemulator(tc->app->hospes))
        .schirmus_inversus;
}

interior ColorMandati
littera_cruda (
    constans TerminaleContextus* tc)
{
    redde colorem_dynamicum(tc, AEMULATOR_COLOR_LITTERAE,
        tc->littera_thematis, COLOR_TEXT);
}

interior ColorMandati
fundus_crudus (
    constans TerminaleContextus* tc)
{
    redde colorem_dynamicum(tc, AEMULATOR_COLOR_FUNDI,
        tc->fundus_thematis, COLOR_BACKGROUND);
}

interior ColorMandati
littera_nativa (
    constans TerminaleContextus* tc)
{
    redde schirmus_inversus(tc) ? fundus_crudus(tc)
                                : littera_cruda(tc);
}

interior ColorMandati
fundus_nativus (
    constans TerminaleContextus* tc)
{
    redde schirmus_inversus(tc) ? littera_cruda(tc)
                                : fundus_crudus(tc);
}

/* color stili -> color mandati: tabula viva aemulatoris (OSC 4),
 * nativus dynamicus */
interior ColorMandati
colorem_resolvere (
    constans TerminaleContextus* tc,
           constans StilusColor* c,
                            b32  crassum,
                            b32  fundus)
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
            redde color_rgb(aemulator_color(
                aemulator_hospes_aemulator(tc->app->hospes), index));
        casus STILUS_COLOR_RGB:
            redde color_rgb(c->valor);
        ordinarius:
            redde fundus ? fundus_nativus(tc) : littera_nativa(tc);
    }
}


/* ==================================================
 * Figura
 * ================================================== */

/* runa sola graphematis quam glyphae_ductae pingunt; -1 si nulla
 * (graphema plurium runarum, aliena) */
interior s32
runa_ducta (
    chorda graphema)
{
    constans i8* p;
    constans i8* finis;
            s32  runa;

    si (graphema.mensura == ZEPHYRUM)
    {
        redde -I;
    }
    p      = graphema.datum;
    finis  = graphema.datum + graphema.mensura;
    runa   = utf8_decodere(&p, finis);
    si (p != finis || !glyphae_ductae_est(runa))
    {
        redde -I;
    }
    redde runa;
}

/* larva glyphae ductae ut rectangula: cursus aequalis opacitatis per
 * ordinem; plenum = littera, umbra = littera in fundum mixta. FALSUM
 * si cellula maior quam LARVA_MAXIMA (vocans textum pingit) */
interior b32
larvam_pingere (
         Mandata* m,
             s32  runa,
             s32  x0,
             s32  y0,
             s32  cw,
             s32  ch,
    ColorMandati  littera,
             i32  fundus)
{
              i8 larva[LARVA_MAXIMA];
           Fines f;
    ColorMandati color;
             s32 x;
             s32 y;
             s32 initium;
              i8 valor;

    si (   cw * ch > LARVA_MAXIMA
        || !glyphae_ductae_pingere(runa, (i32)cw, (i32)ch, larva))
    {
        redde FALSUM;
    }
    f.altitudo = I;
    per (y = ZEPHYRUM; y < ch; y++)
    {
        x = ZEPHYRUM;
        dum (x < cw)
        {
            valor = larva[y * cw + x];
            si (valor == ZEPHYRUM)
            {
                x++;
                perge;
            }
            initium = x;
            dum (x < cw && larva[y * cw + x] == valor)
            {
                x++;
            }
            color = valor == 0xFF ? littera
                  : color_rgb(rgb_miscere(fundus, rgb_mandati(littera),
                        (f64)valor / 255.0));
            f.x         = x0 + initium;
            f.y         = y0 + y;
            f.latitudo  = x - initium;
            mandata_rectangulum(m, f, color, VERUM);
        }
    }
    redde VERUM;
}

/* linea unius pixeli in (x0 ... x0 + latitudo - 1, y): pixela ubi
 * ((x + phasis) % periodus) < plenum, cursus continui ut rectangula;
 * x ABSOLUTUS, ut forma trans cellulas continuetur */
interior vacuum
lineam_formatam (
         Mandata* m,
             s32  x0,
             s32  y,
             s32  latitudo,
             s32  periodus,
             s32  plenum,
             s32  phasis,
    ColorMandati  color)
{
    Fines f;
      s32 k;
      s32 initium;

    f.y         = y;
    f.altitudo  = I;
    initium     = -I;
    per (k = ZEPHYRUM; k <= latitudo; k++)
    {
        si (k < latitudo && (x0 + k + phasis) % periodus < plenum)
        {
            si (initium < ZEPHYRUM)
            {
                initium = k;
            }
            perge;
        }
        si (initium >= ZEPHYRUM)
        {
            f.x         = x0 + initium;
            f.latitudo  = k - initium;
            mandata_rectangulum(m, f, color, VERUM);
            initium     = -I;
        }
    }
}

/* ornamenta cellulae ut pixela (D7c): sublinea (formae V) colore
 * sublineae aut litterae; linea transfixa et superlinea colore
 * litterae */
interior vacuum
ornamenta_pingere (
                      Mandata* m,
    constans StilusTerminalis* st,
                          s32  x0,
                          s32  y0,
                          s32  latitudo,
                          s32  ch,
                 ColorMandati  littera,
                 ColorMandati  sublinea)
{
    s32 infima;

    infima = y0 + ch - I;
    commutatio (st->sublinea)
    {
        casus STILUS_SUBLINEA_SIMPLEX:
            lineam_formatam(m, x0, infima, latitudo, I, I, ZEPHYRUM,
                sublinea);
            frange;
        casus STILUS_SUBLINEA_DUPLEX:
            lineam_formatam(m, x0, infima, latitudo, I, I, ZEPHYRUM,
                sublinea);
            lineam_formatam(m, x0, infima - II, latitudo, I, I,
                ZEPHYRUM,
                sublinea);
            frange;
        casus STILUS_SUBLINEA_UNDULATA:
            /* binae columnae imae, binae superiores */
            lineam_formatam(m, x0, infima, latitudo, IV, II, ZEPHYRUM,
                sublinea);
            lineam_formatam(m, x0, infima - I, latitudo, IV, II, II,
                sublinea);
            frange;
        casus STILUS_SUBLINEA_PUNCTATA:
            lineam_formatam(m, x0, infima, latitudo, II, I, ZEPHYRUM,
                sublinea);
            frange;
        casus STILUS_SUBLINEA_LINEOLATA:
            lineam_formatam(m, x0, infima, latitudo, VI, III, ZEPHYRUM,
                sublinea);
            frange;
        ordinarius:
            frange;
    }
    si ((st->ornamenta & STILUS_TRANSFIXUM) != ZEPHYRUM)
    {
        lineam_formatam(m, x0, y0 + ch / II, latitudo, I, I, ZEPHYRUM,
            littera);
    }
    si ((st->ornamenta & STILUS_SUPERLINEA) != ZEPHYRUM)
    {
        lineam_formatam(m, x0, y0, latitudo, I, I, ZEPHYRUM, littera);
    }
}

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
                      s32  runa;
                      i32  fundus_rgb;
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
    /* fundus non thematis (OSC 11, ?5): superficies tota */
    fundus = fundus_nativus(tc);
    si (   fundus.genus != COLOR_MANDATI_THEMA
        || fundus.valor != (i32)COLOR_BACKGROUND)
    {
        f.x         = ZEPHYRUM;
        f.y         = ZEPHYRUM;
        f.latitudo  = (s32)aemulator_latitudo(a) * cw;
        f.altitudo  = (s32)aemulator_altitudo(a) * ch;
        mandata_rectangulum(m, f, fundus, VERUM);
    }
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
            littera  = colorem_resolvere(tc,
                &cellula.stilus.color_litterae,
                (cellula.stilus.ornamenta & STILUS_CRASSUM) != ZEPHYRUM,
                FALSUM);
            fundus   = colorem_resolvere(tc,
                &cellula.stilus.color_fundi,
                FALSUM, VERUM);
            fundus_proprius = cellula.stilus.color_fundi.genus
                              != STILUS_COLOR_NATIVUS;
            si (inversum)
            {
                ColorMandati t;

                t                = littera;
                littera          = fundus;
                fundus           = t;
                fundus_proprius  = VERUM;
            }
            runa        = runa_ducta(cellula.graphema);
            fundus_rgb  = rgb_mandati(fundus_proprius
                ? fundus : fundus_nativus(tc));
            littera = litteram_legibilem(littera, fundus_rgb,
                (cellula.stilus.ornamenta & STILUS_OBSCURUM)
                    != ZEPHYRUM,
                runa >= 0x2500 && runa <= 0x259F);
            si (fundus_proprius)
            {
                f.x = (s32)x * cw;
                f.y = (s32)y * ch;
                f.latitudo = cellula.latitudo
                    == AEMULATOR_LATA ? II * cw : cw;
                f.altitudo = ch;
                mandata_rectangulum(m, f, fundus, VERUM);
            }
            si ((cellula.stilus.ornamenta & STILUS_INVISIBILE)
                != ZEPHYRUM)
            {
                perge;   /* occultum: nec ornamenta (longitudo) */
            }
            /* lineae capsarum, quadra, braille: figurae ductae (D7c),
             * non textus - nec crassum fictum */
            si (   tc->app->ornamenta_pixelorum
                && runa >= ZEPHYRUM
                && larvam_pingere(m, runa, (s32)x * cw, (s32)y * ch, cw,
                       ch, littera, fundus_rgb))
            {
                /* pictum */
            }
            /* spatium: glypha vacua, nihil pingendum (btop omnem
             * cellulam spatio et fundo implet - park 011) */
            alioquin si (   cellula.graphema.mensura > ZEPHYRUM
                         && !(   cellula.graphema.mensura == I
                              && cellula.graphema.datum[ZEPHYRUM]
                                  == ' '))
            {
                mandata_textus(m, (s32)x * cw, (s32)y * ch,
                    cellula.graphema,
                    ZEPHYRUM, littera);
                /* crassum fictum: fons unius ponderis */
                si (   tc->app->ornamenta_pixelorum
                    && (cellula.stilus.ornamenta & STILUS_CRASSUM)
                       != ZEPHYRUM)
                {
                    mandata_textus(m, (s32)x * cw + I, (s32)y * ch,
                        cellula.graphema, ZEPHYRUM, littera);
                }
            }
            si (tc->app->ornamenta_pixelorum)
            {
                ornamenta_pingere(m, &cellula.stilus, (s32)x * cw,
                    (s32)y * ch,
                    cellula.latitudo == AEMULATOR_LATA ? II * cw : cw,
                    ch, littera,
                    cellula.stilus.color_sublineae.genus
                        == STILUS_COLOR_NATIVUS
                        ? littera
                        : colorem_resolvere(tc,
                              &cellula.stilus.color_sublineae, FALSUM,
                              FALSUM));
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
        mandata_rectangulum(m, f, colorem_dynamicum(tc,
            AEMULATOR_COLOR_CURSORIS, tc->cursor_thematis,
            COLOR_CURSOR),
            VERUM);
        si (   aemulator_cellula(a, cursor.x, cursor.y, &cellula)
            && tc->app->ornamenta_pixelorum
            && runa_ducta(cellula.graphema) >= ZEPHYRUM)
        {
            (vacuum)larvam_pingere(m, runa_ducta(cellula.graphema), f.x,
                f.y, cw, ch, fundus_nativus(tc),
                rgb_mandati(colorem_dynamicum(tc,
                    AEMULATOR_COLOR_CURSORIS, tc->cursor_thematis,
                    COLOR_CURSOR)));
        }
        alioquin si (   aemulator_cellula(a, cursor.x, cursor.y,
                            &cellula)
                     && cellula.graphema.mensura > ZEPHYRUM)
        {
            mandata_textus(m, f.x, f.y, cellula.graphema, ZEPHYRUM,
                fundus_nativus(tc));
        }
    }
}


/* ==================================================
 * Actiones
 * ================================================== */

/* modi aemulatoris -> modi codificatoris (D6b; enumerationes
 * proprias utrumque habet) */
interior vacuum
modos_transferre (
    constans TerminaleContextus* tc,
                CodificatorModi* modi)
{
    AemulatorModi am;

    am = aemulator_modi(aemulator_hospes_aemulator(tc->app->hospes));
    memset(modi, ZEPHYRUM, magnitudo(CodificatorModi));
    modi->cellula_latitudo        = (s32)tc->app->cellula_latitudo;
    modi->cellula_altitudo        = (s32)tc->app->cellula_altitudo;
    modi->kitty_vexilla           = am.kitty_vexilla;
    modi->glutinum                = am.glutinum;
    modi->focus                   = am.focus;
    modi->sagittae_applicationis  = am.sagittae_applicationis;
    modi->lnm                     = am.lnm;
    commutatio (am.mus)
    {
        casus AEMULATOR_MUS_X10:
            modi->mus = CODIFICATOR_MUS_X10;
            frange;
        casus AEMULATOR_MUS_PRESSIO:
            modi->mus = CODIFICATOR_MUS_PRESSIO;
            frange;
        casus AEMULATOR_MUS_TRACTUS:
            modi->mus = CODIFICATOR_MUS_TRACTUS;
            frange;
        casus AEMULATOR_MUS_OMNIS:
            modi->mus = CODIFICATOR_MUS_OMNIS;
            frange;
        ordinarius:
            modi->mus = CODIFICATOR_MUS_NULLUS;
            frange;
    }
    commutatio (am.mus_forma)
    {
        casus AEMULATOR_MUS_FORMA_UTF8:
            modi->mus_forma = CODIFICATOR_FORMA_UTF8;
            frange;
        casus AEMULATOR_MUS_FORMA_SGR:
            modi->mus_forma = CODIFICATOR_FORMA_SGR;
            frange;
        casus AEMULATOR_MUS_FORMA_URXVT:
            modi->mus_forma = CODIFICATOR_FORMA_URXVT;
            frange;
        casus AEMULATOR_MUS_FORMA_SGR_PIXELA:
            modi->mus_forma = CODIFICATOR_FORMA_SGR_PIXELA;
            frange;
        ordinarius:
            modi->mus_forma = CODIFICATOR_FORMA_X10;
            frange;
    }
}

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

    modos_transferre(tc, &modi);
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

/* lineae rotulae integrae: ad programma si murem petivit; in schirmo
 * altero cum ?1007 sagittae (Ghostty mouse_alternate_scroll); aliter
 * visus */
interior vacuum
rotulam_tractare (
    TerminaleContextus* tc,
      constans Eventus* ev,
                   s32  lineae)
{
    constans Aemulator* a;
         AemulatorModi  am;
               Eventus  e;
                   s32  k;

    a   = aemulator_hospes_aemulator(tc->app->hospes);
    am  = aemulator_modi(a);
    si (am.mus != AEMULATOR_MUS_NULLUS)
    {
        e                  = *ev;
        e.datum.rotula.dx  = ZEPHYRUM;
        e.datum.rotula.dy  = lineae * (s32)tc->app->cellula_altitudo;
        eventa_mittere(tc, &e, I);
        redde;
    }
    si (aemulator_alterum(a) && am.rotula_sagittis)
    {
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
        e.datum.clavis.clavis  = lineae > ZEPHYRUM ? CLAVIS_SURSUM
                                                   : CLAVIS_DEORSUM;
        e.datum.clavis.codex   = lineae > ZEPHYRUM
                               ? EVENTUS_CODEX_SAGITTA_SURSUM
                               : EVENTUS_CODEX_SAGITTA_DEORSUM;
        per (k = ZEPHYRUM; k < (lineae > ZEPHYRUM ? lineae : -lineae);
             k++)
        {
            eventa_mittere(tc, &e, I);
        }
        redde;
    }
    aemulator_hospes_visum_movere(tc->app->hospes, lineae);
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
    /* Cmd (super) = brevitates fenestrae (Cmd+V glutinat per menu,
     * D6c), numquam programmati - ut Terminal.app, iTerm; codificator
     * legacy ea iam tacebat, kitty 'super+v' mittebat (Claude Code:
     * 'v' ante glutinum) */
    si (   (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_SUPER))
    {
        pendentem_effundere(tc);
        redde VERUM;
    }
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
        casus EVENTUS_MUS_DEPRESSUS:
        casus EVENTUS_MUS_LIBERATUS:
        casus EVENTUS_MUS_MOTUS:
            /* codificator decernit (mus non petitus: nihil) */
            pendentem_effundere(tc);
            eventa_mittere(tc, ev, I);
            redde VERUM;
        casus EVENTUS_FOCUS:
        casus EVENTUS_DEFOCUS:
            pendentem_effundere(tc);
            tc->focus = (b32)(ev->genus == EVENTUS_FOCUS);
            eventa_mittere(tc, ev, I);
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
                rotulam_tractare(tc, ev, lineae);
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* effectus titulus (OSC 0/2): copiatur, praecisus */
interior vacuum
titulum_notare (
     vacuum* datum,
     chorda  titulus)
{
    TerminaleContextus* tc;
                   i32  n;

    tc  = (TerminaleContextus*)datum;
    n   = titulus.mensura < TITULUS_MAXIMUS ? titulus.mensura
                                            : TITULUS_MAXIMUS;
    memcpy(tc->titulus, titulus.datum, (memoriae_index)n);
    tc->titulus_mensura = n;
    tc->titulus_mutatus = VERUM;
}


/* ==================================================
 * Componere
 * ================================================== */

/* vicus-latera S1a: ex RAMO applicationis (radix in applicatione
 * sola, <terminale id> in hospite) */
interior s32
superficies (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  ordinarium)
{
    chorda* v;
       s32  n;

    v = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    si (!v || !chorda_ut_s32(*v, &n) || n < I)
    {
        redde ordinarium;
    }
    redde n;
}

/* <componens/> radix + schirmum (PARTES_CAMPUS) superficiem totam
 * tegens; focusabile, actio "terminale.clavis". ctx = applicatio
 * (vicus-latera S1b: publica, hospes eam vocat) */
Componens*
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
                     InsulaRamus  ramus;

    (vacuum)motus;
    si (!ctx)
    {
        redde NIHIL;
    }
    tc  = (TerminaleContextus*)((TerminaleApplicatio*)ctx)->contextus;
    a   = aemulator_hospes_aemulator(tc->app->hospes);
    /* ramus applicationis; sine eo radix repositorii dati */
    ramus = tc->app->ramus.repo ? tc->app->ramus
                                : insula_ramus_radix(repo);
    f.x = ZEPHYRUM;
    f.y = ZEPHYRUM;
    f.latitudo  = superficies(&ramus, "superficies_latitudo",
        (s32)(aemulator_latitudo(a) * tc->app->cellula_latitudo));
    f.altitudo  = superficies(&ramus, "superficies_altitudo",
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
    /* eventa nec focalia nec positionalia (focus fenestrae, D6b) ad
     * radicem eunt: eadem actio */
    componens_ponere_actio(radix, "terminale.clavis");
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

/* canonem infixum legere */
interior Canon*
canonem_infixum (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons)
{
    chorda causa;

    redde canon_legere(chorda_ex_literis(fons, piscina), piscina,
        intern, &causa);
}

/* elementum montationis: <terminale id="…" attributa/> */
interior constans character*
elementum_montationis (
                Piscina* piscina,
     constans character* id,
     constans character* attributa)
{
    chorda c;

    c = chorda_concatenare(chorda_ex_literis("<terminale id=\"",
        piscina), chorda_ex_literis(id, piscina), piscina);
    c = chorda_concatenare(c, chorda_ex_literis("\"", piscina),
        piscina);
    c = chorda_concatenare(c, chorda_ex_literis(attributa, piscina),
        piscina);
    c = chorda_concatenare(c, chorda_ex_literis("/>", piscina),
        piscina);
    redde chorda_ut_cstr(c, piscina);
}

/* aedificatio communis: repo NIHIL = applicatio sola (repositorium
 * proprium, ramus radicis, dispensator); aliter montatio in
 * repositorio hospitis (ramus <terminale id>, canones sui, nullus
 * dispensator) */
interior b32
applicationem_struere (
    TerminaleApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
        Pseudoterminale* pt,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo)
{
     AemulatorHospesConfiguratio  cfg;
              TerminaleContextus* tc;

    si (!app || !piscina || !intern || !pt || (repo && !id))
    {
        si (pt)
        {
            pt->claudere(pt->datum);
        }
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(TerminaleApplicatio));
    app->piscina              = piscina;
    app->intern               = intern;
    app->cellula_latitudo     = CELLULA_LATITUDO;
    app->cellula_altitudo     = CELLULA_ALTITUDO;
    app->ornamenta_pixelorum  = VERUM;
    tc = (TerminaleContextus*)piscina_conari_allocare(piscina,
        magnitudo(TerminaleContextus));
    si (!tc)
    {
        pt->claudere(pt->datum);
        redde FALSUM;
    }
    memset(tc, ZEPHYRUM, magnitudo(TerminaleContextus));
    tc->app               = app;
    tc->focus             = VERUM;
    tc->littera_thematis  = rgb_thematis(COLOR_TEXT);
    tc->fundus_thematis   = rgb_thematis(COLOR_BACKGROUND);
    tc->cursor_thematis   = rgb_thematis(COLOR_CURSOR);
    app->contextus        = tc;
    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.latitudo = latitudo / CELLULA_LATITUDO > ZEPHYRUM
                           ? latitudo / CELLULA_LATITUDO : I;
    cfg.aemulator.altitudo = altitudo / CELLULA_ALTITUDO > ZEPHYRUM
                           ? altitudo / CELLULA_ALTITUDO : I;
    cfg.aemulator.color_litterae = tc->littera_thematis;
    cfg.aemulator.color_fundi = tc->fundus_thematis;
    cfg.aemulator.color_cursoris = tc->cursor_thematis;
    cfg.aemulator.effectus.titulus = titulum_notare;
    cfg.aemulator.effectus.datum = tc;
    app->hospes = aemulator_hospes_creare(piscina, &cfg, pt);
    si (!app->hospes)
    {
        redde FALSUM;
    }
    tc->scrutinium  = piscina_generare_dynamicum("terminale_scrutinium",
        LXIV * MXXIV);
    si (repo)
    {
        /* montatio: canones PRIMUM (radix hospitis eam non videt),
         * deinde elementum initiale in utroque genere */
        app->repo   = repo;
        app->ramus  = insula_ramus(repo, "terminale", id);
        insula_ramus_canonem_ponere(&app->ramus, INSULA_DURABILIS,
            canonem_infixum(piscina, intern, CANON_DURABILIS));
        insula_ramus_canonem_ponere(&app->ramus, INSULA_EPHEMERA,
            canonem_infixum(piscina, intern, CANON_EPHEMERA));
        si (   !insula_ramum_initiare(&app->ramus, INSULA_DURABILIS,
                   elementum_montationis(piscina, id, ""))
            || !insula_ramum_initiare(&app->ramus, INSULA_EPHEMERA,
                   elementum_montationis(piscina, id,
                       " focus=\"schirmum\"")))
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        app->repo = insula_repositorium_creare(piscina, intern,
            "<terminale/>", "<terminale focus=\"schirmum\"/>");
        app->ramus = app->repo ? insula_ramus_radix(app->repo)
                               : app->ramus;
    }
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
    si (repo)
    {
        redde VERUM;   /* dispensator hospitis */
    }
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->actiones, terminale_componere, app, QUIES_MS);
    redde app->d != NIHIL;
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
    redde applicationem_struere(app, piscina, intern, pt, NIHIL, NIHIL,
        latitudo, altitudo);
}

b32
terminale_montare (
    TerminaleApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo)
{
    PseudoterminaleConfiguratio  cfg;
                Pseudoterminale* pt;
                            b32  fumus;

    si (!app || !piscina || !intern || !repo || !id)
    {
        redde FALSUM;
    }
    pseudoterminale_configuratio_initiare(&cfg);
    cfg.argumenta  = terminale_argumenta(piscina, ZEPHYRUM, NIHIL,
        &fumus);
    cfg.ambitus    = ambitus_terminalis;
    cfg.latitudo   = latitudo / CELLULA_LATITUDO > ZEPHYRUM
                   ? latitudo / CELLULA_LATITUDO : I;
    cfg.altitudo   = altitudo / CELLULA_ALTITUDO > ZEPHYRUM
                   ? altitudo / CELLULA_ALTITUDO : I;
    pt = pseudoterminale_posix_creare(piscina, &cfg, NIHIL, NIHIL);
    si (!pt)
    {
        redde FALSUM;
    }
    redde applicationem_struere(app, piscina, intern, pt, repo, id,
        latitudo, altitudo);
}

/* ?1004 modo novo posito relatio status currentis statim (Ghostty
 * stream_handler focus_event) */
interior vacuum
focum_nuntiare (
    TerminaleContextus* tc)
{
    AemulatorModi am;
          Eventus e;

    am = aemulator_modi(aemulator_hospes_aemulator(tc->app->hospes));
    si (am.focus && !tc->focus_relatus)
    {
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = tc->focus ? EVENTUS_FOCUS : EVENTUS_DEFOCUS;
        eventa_mittere(tc, &e, I);
    }
    tc->focus_relatus = am.focus;
}

AemulatorHospesPulsus
terminale_pulsare (
    TerminaleApplicatio* app,
                    s32  mora_ms)
{
    AemulatorHospesPulsus  pulsus;
       constans Aemulator* a;
                      s32  lat;
                      s32  alt;
                      i32  columnae;
                      i32  lineae;

    a    = aemulator_hospes_aemulator(app->hospes);
    lat  = superficies(&app->ramus, "superficies_latitudo",
        (s32)(aemulator_latitudo(a) * app->cellula_latitudo));
    alt  = superficies(&app->ramus, "superficies_altitudo",
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
    pulsus = aemulator_hospes_pulsare(app->hospes, mora_ms);
    focum_nuntiare((TerminaleContextus*)app->contextus);
    redde pulsus;
}

chorda
terminale_titulus (
    TerminaleApplicatio* app,
                    b32* mutatus)
{
    TerminaleContextus* tc;
                chorda  t;

    tc         = (TerminaleContextus*)app->contextus;
    t.datum    = (i8*)tc->titulus;
    t.mensura  = tc->titulus_mensura;
    si (mutatus)
    {
        *mutatus = tc->titulus_mutatus;
    }
    tc->titulus_mutatus = FALSUM;
    redde t;
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
