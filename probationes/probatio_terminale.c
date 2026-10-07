/* probatio_terminale.c - applicatio terminale sine fenestra
 * (aemulator-plan E2)
 *
 * Figura per mandata quadri (textus, color, positio per cellulam;
 * fundus, inversum, cursor) et per imaginem PNG (oculis); claves per
 * manus_ludus (par clavis + textus semel, Enter, Ctrl-C); magnitudo
 * superficiei -> cellulae; rotula -> visus; concha vera (/bin/sh). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "mandatum.h"
#include "color.h"
#include "tabula_pixelorum.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "manus_ludus.h"
#include "pseudoterminale.h"
#include "aemulator.h"
#include "aemulator_hospes.h"
#include "terminale.h"
#include "imago.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define CELL_X VI
#define CELL_Y VIII
#define VIA_IMAGINIS "build/probatio_terminale.png"

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

nomen structura {
    TerminaleApplicatio  app;
        Pseudoterminale* pt;
          LudusFenestra* lf;
             ManusLudus* manus;
                    s64  tempus;
} Machina;

/* terminale super infantem datum; superficies LXXX x XXIV cellulae */
interior b32
machinam_struere (
            Machina* mc,
    Pseudoterminale* pt)
{
    TabulaPixelorum* tabula;

    memset(mc, ZEPHYRUM, magnitudo(Machina));
    mc->pt = pt;
    si (!terminale_applicatio_aedificare(&mc->app, piscina, intern, pt,
            LXXX * CELL_X, XXIV * CELL_Y))
    {
        redde FALSUM;
    }
    tabula = tabula_pixelorum_creare_nuda(piscina, LXXX * CELL_X,
        XXIV * CELL_Y);
    mc->lf = ludus_fenestra_creare(piscina, mc->app.d, mc->app.figurae,
        ZEPHYRUM, NIHIL, NIHIL, tabula);
    mc->manus = manus_ludus_creare(piscina, mc->app.d);
    redde mc->lf != NIHIL && mc->manus != NIHIL;
}

/* pulsus + quadrum, ut principale facit */
interior AemulatorHospesPulsus
quadrum (
    Machina* mc,
        s32  mora_ms)
{
    AemulatorHospesPulsus p;

    p           = terminale_pulsare(&mc->app, mora_ms);
    mc->tempus  += XVI;
    ludus_quadrum(mc->lf, mc->tempus);
    redde p;
}

/* mandatum generis dati ad (x, y) in quadro ultimo; NIHIL si nullum */
interior Mandatum*
mandatum_ad (
          Machina* mc,
    MandatumGenus  genus,
              s32  x,
              s32  y)
{
    Mandatum* md;
         i32  i;

    per (i = ZEPHYRUM; i < mandata_numerus(mc->lf->mandata); i++)
    {
        md = mandata_obtinere(mc->lf->mandata, i);
        si (md->genus == genus && md->fines.x == x && md->fines.y == y)
        {
            redde md;
        }
    }
    redde NIHIL;
}

/* rectangulum coloris cursoris in quadro ultimo? */
interior b32
cursor_pictus (
    Machina* mc)
{
    Mandatum* md;
         i32  i;

    per (i = ZEPHYRUM; i < mandata_numerus(mc->lf->mandata); i++)
    {
        md = mandata_obtinere(mc->lf->mandata, i);
        si (   md->genus       == MANDATUM_RECTANGULUM
            && md->color.genus == COLOR_MANDATI_THEMA
            && md->color.valor == (i32)COLOR_CURSOR)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
textus_cellulae (
               Machina* mc,
                   i32  x,
                   i32  y,
    constans character* textus)
{
    Mandatum* md;

    md = mandatum_ad(mc, MANDATUM_TEXTUS, (s32)(x * CELL_X),
        (s32)(y * CELL_Y));
    redde md && chorda_aequalis_literis(md->textus, textus);
}

/* RGB -> valor mandati (pixelum tabulae, mandatum.h) */
interior i32
pixelum_rgb (
    i32 r,
    i32 g,
    i32 b)
{
    redde color_ad_pixelum(color_ex_rgba((i8)r, (i8)g, (i8)b,
        (i8)0xFF));
}

interior b32
color_est (
         ColorMandati c,
    ColorMandatiGenus genus,
                  i32 valor)
{
    redde c.genus == genus && c.valor == valor;
}

/* pixelum (x, y) imaginis = (r, g, b, opacum) */
interior b32
pixelum_est (
    constans Imago* im,
               i32  x,
               i32  y,
               i32  r,
               i32  g,
               i32  b)
{
    constans i8* p;

    p = im->pixela + (y * im->latitudo + x) * IV;
    redde (i32)p[ZEPHYRUM] == r && (i32)p[I] == g && (i32)p[II] == b
        && (i32)p[III] == CCLV;
}

interior b32
captum_est (
               Machina* mc,
    constans character* expectatum)
{
    redde chorda_aequalis_literis(
        pseudoterminale_memoriae_captum(mc->pt), expectatum);
}

interior vacuum
figuram_probare (vacuum)
{
            Machina  mc;
           Mandatum* md;
    TabulaPixelorum* t;
       ImagoFructus  lecta;

    imprimere("\n--- I: figura ---\n");
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_memoriae_creare(
        piscina, (constans i8*)"salve \x1B[31mR\x1B[0m \x1B[44m \x1B[0m"
        " \x1B[7mI\x1B[0m\r\nlinea\x1B[3;1H\x1B[1;31mB\x1B[0m"
        "\x1B[38;5;196mC\x1B[38;5;244mG\x1B[0m\x1B[2;6H", XCVI,
        ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    /* textus per cellulam, colore nativo thematis */
    CREDO_VERUM(textus_cellulae(&mc, ZEPHYRUM, ZEPHYRUM, "s"));
    CREDO_VERUM(textus_cellulae(&mc, IV, ZEPHYRUM, "e"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    /* SGR 31: xterm rubrum (205, 0, 0) */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, VI * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && chorda_aequalis_literis(md->textus, "R"));
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xCD, ZEPHYRUM, ZEPHYRUM)));
    /* SGR 44: fundus xterm caeruleus (0, 0, 238) */
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, VIII * CELL_X,
        ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(ZEPHYRUM, ZEPHYRUM, 0xEE)));
    /* SGR 7: fundus = textus thematis, littera = fundus thematis */
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, X * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, X * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_BACKGROUND));
    /* crassum 0-7 clarius (xterm 9: 255,0,0); cubus 196; griseus 244 */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, II * CELL_Y);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xFF, ZEPHYRUM, ZEPHYRUM)));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, CELL_X, II * CELL_Y);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xFF, ZEPHYRUM, ZEPHYRUM)));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, II * CELL_X, II * CELL_Y);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0x80, 0x80, 0x80)));
    /* cursor post "linea" (V, I) */
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, V * CELL_X, I * CELL_Y);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_CURSOR));
    /* imago (oculis inspicienda) ET pixela vera: mandatum recte
     * colorabatur dum pixela errabant (ordo octetorum RGBA - inventum
     * oculis, nunc hic pinnatum) */
    CREDO_VERUM(ludus_fenestra_imaginem_scribere(mc.lf, VIA_IMAGINIS));
    t = mc.lf->tabula;
    CREDO_AEQUALIS_I32(t->latitudo, LXXX * CELL_X);
    lecta = imago_caricare_ex_file(VIA_IMAGINIS, piscina);
    CREDO_VERUM(lecta.successus);
    CREDO_VERUM(pixelum_est(&lecta.imago, LI, IV, ZEPHYRUM, ZEPHYRUM,
                            0xEE));
    CREDO_VERUM(pixelum_est(&lecta.imago, XXXVII, III, 0xCD, ZEPHYRUM,
                            ZEPHYRUM));
    terminale_claudere(&mc.app);
}

interior vacuum
claves_probare (vacuum)
{
    Machina mc;

    imprimere("\n--- II: claves ---\n");
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_memoriae_creare(
        piscina, (constans i8*)"", ZEPHYRUM, ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    /* par clavis + textus semel codificatur */
    CREDO_VERUM(manus_ludus_scribere(mc.manus, "ls"));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "ls"));
    CREDO_VERUM(manus_ludus_clavem(mc.manus, '\r', ZEPHYRUM));
    CREDO_VERUM(manus_ludus_clavem(mc.manus, 'c', MOD_IMPERIUM));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "ls\r\x03"));
    terminale_claudere(&mc.app);
}

interior vacuum
magnitudinem_probare (vacuum)
{
      Machina mc;
      Eventus ev;
          i32 lat;
          i32 alt;
          i32 i;
    character b[XVI];
    character effusio[CCLVI];

    imprimere("\n--- III: magnitudo et rotula ---\n");
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_memoriae_creare(
        piscina, (constans i8*)"", ZEPHYRUM, ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), LXXX);
    /* superficies nova: C x XXX cellulae, ad infantem quoque */
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus = EVENTUS_MUTARE_MAGNITUDINEM;
    ev.datum.mutare_magnitudinem.latitudo = C * CELL_X;
    ev.datum.mutare_magnitudinem.altitudo = XXX * CELL_Y;
    ludus_fenestra_tractare(mc.lf, &ev, mc.tempus);
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), C);
    CREDO_AEQUALIS_I32(aemulator_altitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), XXX);
    pseudoterminale_memoriae_amplitudo(mc.pt, &lat, &alt);
    CREDO_AEQUALIS_I32(lat, C);
    CREDO_AEQUALIS_I32(alt, XXX);
    terminale_claudere(&mc.app);

    /* rotula: XL lineae per infantem in XXIV -> historia XVII; tres
     * gradus sursum */
    effusio[ZEPHYRUM] = '\0';
    per (i = ZEPHYRUM; i < XL; i++)
    {
        sprintf(b, "%u\r\n", (unsigned)i);
        strcat(effusio, b);
    }
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_memoriae_creare(
        piscina, (constans i8*)effusio, (i32)strlen(effusio),
        ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_historia(
        aemulator_hospes_aemulator(mc.app.hospes)), XVII);
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus               = EVENTUS_MUS_ROTULA;
    ev.datum.rotula.genus  = EVENTUS_ROTULA_GRADATA;
    ev.datum.rotula.dy     = III * CELL_Y;
    ev.datum.rotula.x      = X;
    ev.datum.rotula.y      = X;
    ludus_fenestra_tractare(mc.lf, &ev, mc.tempus);
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_visus(
        aemulator_hospes_aemulator(mc.app.hospes)), III);
    /* in historia cursor non pingitur */
    CREDO_FALSUM(cursor_pictus(&mc));
    /* residuum: duo dimidii gradus = linea una */
    ev.datum.rotula.dy = CELL_Y / II;
    ludus_fenestra_tractare(mc.lf, &ev, mc.tempus);
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_visus(
        aemulator_hospes_aemulator(mc.app.hospes)), III);
    ludus_fenestra_tractare(mc.lf, &ev, mc.tempus);
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_visus(
        aemulator_hospes_aemulator(mc.app.hospes)), IV);
    /* scribere visum ad imum reducit */
    CREDO_VERUM(manus_ludus_scribere(mc.manus, "q"));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_visus(
        aemulator_hospes_aemulator(mc.app.hospes)), ZEPHYRUM);
    terminale_claudere(&mc.app);
}

interior vacuum
concham_probare (vacuum)
{
                        Machina  mc;
    PseudoterminaleConfiguratio  cfg;
                            i32  i;
                         chorda  textus;
             constans character* argumenta[] = { "/bin/sh", "-c",
        "read x; printf 'accepi:%s' \"$x\"; sleep 1", NIHIL };

    imprimere("\n--- IV: concha vera ---\n");
    pseudoterminale_configuratio_initiare(&cfg);
    cfg.argumenta = argumenta;
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_posix_creare(
        piscina, &cfg, NIHIL, NIHIL)));
    (vacuum)quadrum(&mc, C);
    CREDO_VERUM(manus_ludus_scribere(mc.manus, "abc"));
    CREDO_VERUM(manus_ludus_clavem(mc.manus, '\r', ZEPHYRUM));
    textus.mensura = ZEPHYRUM;
    per (i = ZEPHYRUM; i < C; i++)
    {
        (vacuum)quadrum(&mc, L);
        textus = aemulator_textum_effundere(
            aemulator_hospes_aemulator(mc.app.hospes), piscina);
        si (   textus.mensura >= X
            && memcmp(textus.datum + textus.mensura - X, "accepi:abc",
                      X) == ZEPHYRUM)
        {
            frange;
        }
    }
    CREDO_VERUM(i < C);
    terminale_claudere(&mc.app);
}

/* terminale super effusionem programmatis fixam (modi, OSC) */
interior b32
machinam_effusione (
                Machina* mc,
     constans character* effusio)
{
    si (!machinam_struere(mc, pseudoterminale_memoriae_creare(piscina,
            (constans i8*)effusio, (i32)strlen(effusio), ZEPHYRUM)))
    {
        redde FALSUM;
    }
    (vacuum)quadrum(mc, ZEPHYRUM);
    redde VERUM;
}

/* eventum crudum per fenestram, deinde quadrum */
interior vacuum
eventum_dare (
             Machina* mc,
    constans Eventus* ev)
{
    ludus_fenestra_tractare(mc->lf, ev, mc->tempus);
    (vacuum)quadrum(mc, ZEPHYRUM);
}

/* clavis non typica (sagitta): depressa et liberata */
interior vacuum
clavem_crudam (
         Machina* mc,
        clavis_t  clavis,
    EventusCodex  codex)
{
    Eventus ev;

    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    ev.datum.clavis.clavis  = clavis;
    ev.datum.clavis.codex   = codex;
    eventum_dare(mc, &ev);
    ev.genus               = EVENTUS_CLAVIS_LIBERATUS;
    ev.datum.clavis.actio  = EVENTUS_ACTIO_SOLUTA;
    eventum_dare(mc, &ev);
}

interior vacuum
rotulam_dare (
    Machina* mc,
        s32  lineae)
{
    Eventus ev;

    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus               = EVENTUS_MUS_ROTULA;
    ev.datum.rotula.genus  = EVENTUS_ROTULA_GRADATA;
    ev.datum.rotula.dy     = lineae * CELL_Y;
    ev.datum.rotula.x      = XIII;
    ev.datum.rotula.y      = XVII;
    eventum_dare(mc, &ev);
}

/* V: modi ad codificatorem (D6b) */
interior vacuum
modos_probare (vacuum)
{
    Machina mc;
    Eventus ev;
    unio { constans character* l; i8* m; } u;

    imprimere("\n--- V: modi ad codificatorem (D6b) ---\n");
    /* DECCKM: sagitta SS3 */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1h"));
    clavem_crudam(&mc, CLAVIS_SURSUM, EVENTUS_CODEX_SAGITTA_SURSUM);
    CREDO_VERUM(captum_est(&mc, "\x1BOA"));
    terminale_claudere(&mc.app);
    /* LNM: Enter CR LF */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[20h"));
    CREDO_VERUM(manus_ludus_clavem(mc.manus, '\r', ZEPHYRUM));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "\r\n"));
    terminale_claudere(&mc.app);
    /* kitty: Effugium disambiguatum */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[>1u"));
    CREDO_VERUM(manus_ludus_clavem(mc.manus, (character)0x1B,
        ZEPHYRUM));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "\x1B[27u"));
    terminale_claudere(&mc.app);
    /* glutinum (?2004) */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?2004h"));
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    u.l                                = "ab";
    ev.genus                           = EVENTUS_TEXTUS;
    ev.datum.textus.contentum.datum    = u.m;
    ev.datum.textus.contentum.mensura  = II;
    ev.datum.textus.origo              = EVENTUS_ORIGO_GLUTINATA;
    eventum_dare(&mc, &ev);
    CREDO_VERUM(captum_est(&mc, "\x1B[200~ab\x1B[201~"));
    terminale_claudere(&mc.app);
}

/* VI: mus, rotula, focus (D6b) */
interior vacuum
murem_probare (vacuum)
{
    Machina mc;
    Eventus ev;

    imprimere("\n--- VI: mus, rotula, focus (D6b) ---\n");
    /* sine modo muris: ictus nihil mittit */
    CREDO_VERUM(machinam_effusione(&mc, ""));
    CREDO_VERUM(manus_ludus_premere_ad(mc.manus, XIII, XVII));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, ""));
    terminale_claudere(&mc.app);
    /* ?1000 + ?1006: SGR, cellula (2, 2) */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1000h\x1B[?1006h"));
    CREDO_VERUM(manus_ludus_premere_ad(mc.manus, XIII, XVII));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "\x1B[<0;3;3M\x1B[<0;3;3m"));
    /* rotula ad programma, non ad visum */
    rotulam_dare(&mc, I);
    CREDO_VERUM(captum_est(&mc,
        "\x1B[<0;3;3M\x1B[<0;3;3m\x1B[<64;3;3M"));
    terminale_claudere(&mc.app);
    /* ?1000 solum: forma X10 (ordinaria terminalis) */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1000h"));
    CREDO_VERUM(manus_ludus_premere_ad(mc.manus, XIII, XVII));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "\x1B[M ##\x1B[M###"));
    terminale_claudere(&mc.app);
    /* ?1007: schirmum alterum sine mure - rotula = sagittae */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1049h"));
    rotulam_dare(&mc, II);
    CREDO_VERUM(captum_est(&mc, "\x1B[A\x1B[A"));
    rotulam_dare(&mc, -I);
    CREDO_VERUM(captum_est(&mc, "\x1B[A\x1B[A\x1B[B"));
    terminale_claudere(&mc.app);
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1049h\x1B[?1h"));
    rotulam_dare(&mc, I);
    CREDO_VERUM(captum_est(&mc, "\x1BOA"));
    terminale_claudere(&mc.app);
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1049h\x1B[?1007l"));
    rotulam_dare(&mc, I);
    CREDO_VERUM(captum_est(&mc, ""));
    terminale_claudere(&mc.app);
    /* ?1004: relatio statim (focus ordinarie VERUM), deinde eventa */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1004h"));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(captum_est(&mc, "\x1B[I"));
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus = EVENTUS_DEFOCUS;
    eventum_dare(&mc, &ev);
    CREDO_VERUM(captum_est(&mc, "\x1B[I\x1B[O"));
    terminale_claudere(&mc.app);
}

/* VII: titulus et colores (D6b) */
interior vacuum
titulum_colores_probare (vacuum)
{
     Machina  mc;
    Mandatum* md;
      chorda  t;
         b32  mutatus;

    imprimere("\n--- VII: titulus et colores (D6b) ---\n");
    CREDO_VERUM(machinam_effusione(&mc, "\x1B]2;salve\x07"));
    t = terminale_titulus(&mc.app, &mutatus);
    CREDO_VERUM(mutatus);
    CREDO_VERUM(chorda_aequalis_literis(t, "salve"));
    t = terminale_titulus(&mc.app, &mutatus);
    CREDO_FALSUM(mutatus);
    CREDO_VERUM(chorda_aequalis_literis(t, "salve"));
    terminale_claudere(&mc.app);
    /* OSC 4: tabula viva; OSC 10/11/12: litterae, fundus, cursor */
    CREDO_VERUM(machinam_effusione(&mc,
        "\x1B]4;1;#010203\x07\x1B[31mR\x1B[0m"
        "\x1B]10;#405060\x07\x1B]11;#102030\x07\x1B]12;#a0b0c0\x07N"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(I, II, III)));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0x40, 0x50, 0x60)));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0x10, 0x20, 0x30)));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, II * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xA0, 0xB0, 0xC0)));
    terminale_claudere(&mc.app);
}

s32
principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_terminale",
        LXIV * MXXIV * MXXIV);
    intern  = internamentum_creare(piscina);
    credo_aperire(piscina);
    thema_initiare();

    figuram_probare();
    claves_probare();
    magnitudinem_probare();
    concham_probare();
    modos_probare();
    murem_probare();
    titulum_colores_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
