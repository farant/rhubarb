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
#include "canon.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        piscina,
        (constans i8*)"salve \x1B[31m\x1B[48;2;255;255;255mR\x1B[0m "
        "\x1B[44m \x1B[0m \x1B[7mI\x1B[0m\r\nlinea\x1B[3;1H"
        "\x1B[48;2;255;255;255m\x1B[1;31mB\x1B[0m\x1B[48;2;255;255;255m"
        "\x1B[38;5;196mC\x1B[38;5;244mG\x1B[0m\x1B[2;6H", CLIII,
        ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    /* textus per cellulam, colore nativo thematis */
    CREDO_VERUM(textus_cellulae(&mc, ZEPHYRUM, ZEPHYRUM, "s"));
    CREDO_VERUM(textus_cellulae(&mc, IV, ZEPHYRUM, "e"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    /* SGR 31: xterm rubrum (205, 0, 0) - super fundum album: mappatio
     * tabulae exacta manet (contrastus minimus III satis: fundus
     * thematis lucidus rubrum propelleret) */
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
    /* Cmd+V (D6c, Frani sessio cum Claude Code): clavis Cmd fenestrae
     * est, numquam programmatis - etiam sub kitty; glutinum solum */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[>1u"));
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    ev.datum.clavis.clavis        = (clavis_t)'V';
    ev.datum.clavis.runa          = 'v';
    ev.datum.clavis.producta      = 'v';
    ev.datum.clavis.modificantes  = MOD_SUPER;
    eventum_dare(&mc, &ev);
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    u.l                                = "ab";
    ev.genus                           = EVENTUS_TEXTUS;
    ev.datum.textus.contentum.datum    = u.m;
    ev.datum.textus.contentum.mensura  = II;
    ev.datum.textus.origo              = EVENTUS_ORIGO_GLUTINATA;
    eventum_dare(&mc, &ev);
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus                      = EVENTUS_CLAVIS_LIBERATUS;
    ev.datum.clavis.clavis        = (clavis_t)'V';
    ev.datum.clavis.runa          = 'v';
    ev.datum.clavis.modificantes  = MOD_SUPER;
    ev.datum.clavis.actio         = EVENTUS_ACTIO_SOLUTA;
    eventum_dare(&mc, &ev);
    CREDO_VERUM(captum_est(&mc, "ab"));
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
    /* tractus (?1002 + SGR, tmux): pressio, motus CUM bottone,
     * solutio - fenestra bottonem in motu nunc fert (D7, Franus) */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[?1002h\x1B[?1006h"));
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus             = EVENTUS_MUS_DEPRESSUS;
    ev.datum.mus.x       = XIII;
    ev.datum.mus.y       = XVII;
    ev.datum.mus.botton  = MUS_SINISTER;
    eventum_dare(&mc, &ev);
    ev.genus        = EVENTUS_MUS_MOTUS;
    ev.datum.mus.x  = XIX;
    eventum_dare(&mc, &ev);
    ev.genus               = EVENTUS_MUS_LIBERATUS;
    eventum_dare(&mc, &ev);
    CREDO_VERUM(captum_est(&mc,
        "\x1B[<0;3;3M\x1B[<32;4;3M\x1B[<0;4;3m"));
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
    /* OSC 4: tabula viva; OSC 10/11/12: litterae, fundus, cursor
     * (litterae lucidae super fundum obscurum: contrastus satis,
     * valores exacti) */
    CREDO_VERUM(machinam_effusione(&mc,
        "\x1B]4;1;#f0e0d0\x07\x1B[31mR\x1B[0m"
        "\x1B]10;#c0d0e0\x07\x1B]11;#102030\x07\x1B]12;#a0b0c0\x07N"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xF0, 0xE0, 0xD0)));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xC0, 0xD0, 0xE0)));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0x10, 0x20, 0x30)));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, II * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xA0, 0xB0, 0xC0)));
    terminale_claudere(&mc.app);
}

/* oraculum WCAG 2.0 (luminantia relativa, ratio contrastus) -
 * independens a lib/terminale.c */
interior f64
canalis_linearis (
    i32 c)
{
    f64 v;

    v = (f64)c / 255.0;
    redde v <= 0.03928 ? v / 12.92 : pow((v + 0.055) / 1.055, 2.4);
}

interior f64
luminantia (
    i32 r,
    i32 g,
    i32 b)
{
    redde 0.2126 * canalis_linearis(r) + 0.7152 * canalis_linearis(g)
         + 0.0722 * canalis_linearis(b);
}

/* ratio inter pixelum mandati (a<<24|b<<16|g<<8|r) et colorem
 * thematis */
interior f64
ratio_contra (
           i32 pixelum,
    ColorThema fundus)
{
    Color k;
      f64 l1;
      f64 l2;

    k   = thema_color(fundus);
    l1  = luminantia((i32)(pixelum & 0xFF), (i32)((pixelum
        >> VIII) & 0xFF),
        (i32)((pixelum >> XVI) & 0xFF));
    l2  = luminantia((i32)(insignatus character)k.r,
        (i32)(insignatus character)k.g, (i32)(insignatus character)k.b);
    redde l1 > l2 ? (l1 + 0.05) / (l2 + 0.05) : (l2 + 0.05) / (l1
        + 0.05);
}

/* VIII: contrastus minimus (III.0, propulsio minima) et obscurum */
interior vacuum
contrastum_probare (vacuum)
{
     Machina  mc;
    Mandatum* md;
         i32  v;

    imprimere("\n--- VIII: contrastus minimus et obscurum ---\n");
    CREDO_VERUM(machinam_effusione(&mc,
        "\x1B[38;2;200;200;200mA\x1B[0m\x1B[30mB\x1B[0mC"
        "\x1B[2mD\x1B[0m\x1B[48;2;0;0;0m\x1B[38;2;20;20;20mE"));
    /* griseus pallidus: propulsus ad III, griseus manet, non ultra */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && md->color.genus == COLOR_MANDATI_RGBA);
    v = md ? md->color.valor : ZEPHYRUM;
    CREDO_VERUM(ratio_contra(v, COLOR_BACKGROUND) >= 3.0);
    CREDO_VERUM(ratio_contra(v, COLOR_BACKGROUND) < 3.3);
    CREDO_VERUM((v & 0xFF) == ((v >> VIII) & 0xFF)
             && (v & 0xFF) == ((v >> XVI) & 0xFF));
    /* niger sufficit: immutatus */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM)));
    /* nativus non tactus: signum thematis manet */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, II * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    /* obscurum: mixtum cum fundo, differt a nativo, legibile manet */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, III * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && md->color.genus == COLOR_MANDATI_RGBA);
    CREDO_VERUM(md && ratio_contra(md->color.valor, COLOR_BACKGROUND)
                      >= 3.0);
    /* fundus proprius niger: littera fere nigra illuminatur */
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, IV * CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && md->color.genus == COLOR_MANDATI_RGBA);
    v = md ? md->color.valor : ZEPHYRUM;
    CREDO_VERUM((v & 0xFF) > 0x50);
    terminale_claudere(&mc.app);
}

/* rectangulum unius pixeli altitudinis ad (x, y) latitudinis datae -
 * linea ornamenti (D7c) */
interior Mandatum*
linea_ad (
    Machina* mc,
        s32  x,
        s32  y,
        s32  latitudo)
{
    Mandatum* md;
         i32  i;

    per (i = ZEPHYRUM; i < mandata_numerus(mc->lf->mandata); i++)
    {
        md = mandata_obtinere(mc->lf->mandata, i);
        si (   md->genus          == MANDATUM_RECTANGULUM
            && md->fines.x        == x
            && md->fines.y        == y
            && md->fines.latitudo == latitudo
            && md->fines.altitudo == I)
        {
            redde md;
        }
    }
    redde NIHIL;
}

/* textus ad (x, y) numeratus (crassum fictum = bis, D7c) */
interior i32
textus_numerus (
    Machina* mc,
        s32  x,
        s32  y)
{
    Mandatum* md;
         i32  i;
         i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(mc->lf->mandata); i++)
    {
        md = mandata_obtinere(mc->lf->mandata, i);
        si (   md->genus   == MANDATUM_TEXTUS
            && md->fines.x == x
            && md->fines.y == y)
        {
            n++;
        }
    }
    redde n;
}

/* IX: ornamenta ut pixela (D7c, ambulatio vttest: sublinea non
 * pingebatur). Cellula VI x VIII: sublinea ima linea (VII), duplex
 * VII et V, undulata binae columnae inter VII et VI, punctata pixelum
 * alternum, lineolata III/III, transfixa IV, superlinea 0; phasis ex x
 * absoluto. Crassum fictum: littera iterum uno pixelo dextrorsum. */
interior vacuum
ornamenta_probare (vacuum)
{
         Machina  mc;
        Mandatum* md;
    ImagoFructus  lecta;

    imprimere("\n--- IX: ornamenta (D7c) ---\n");
    CREDO_VERUM(machinam_effusione(&mc,
        "\x1B[4mA\x1B[0m\x1B[9mB\x1B[0m\x1B[53mC\x1B[0m\x1B[4:2mD"
        "\x1B[4:3mEE\x1B[4:4mFF\x1B[4:5mGG\x1B[0m\x1B[1mH\x1B[0m"
        "\x1B[4;58;2;255;0;0mI\x1B[0m\x1B[8;4mJ\x1B[0m\x1B[5;1H"));
    /* A: sublinea simplex, colore litterae (thema) */
    md = linea_ad(&mc, ZEPHYRUM, VII, VI);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    /* B: transfixa; C: superlinea */
    CREDO_VERUM(linea_ad(&mc, VI, IV, VI) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XII, ZEPHYRUM, VI) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, VI, VII, VI) == NIHIL);
    /* D: duplex */
    CREDO_VERUM(linea_ad(&mc, XVIII, VII, VI) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XVIII, V, VI) != NIHIL);
    /* EE (x XXIV-XXXV): unda continua trans cellulas */
    CREDO_VERUM(linea_ad(&mc, XXIV, VII, II) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXVI, VI, II) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXVIII, VII, II) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXX, VI, II) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXXII, VII, II) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXX, VII, II) == NIHIL);
    /* FF (x XXXVI-XLVII): puncta */
    CREDO_VERUM(linea_ad(&mc, XXXVI, VII, I) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXXVIII, VII, I) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, XXXVII, VII, I) == NIHIL);
    /* GG (x XLVIII-LIX): lineolae III */
    CREDO_VERUM(linea_ad(&mc, XLVIII, VII, III) != NIHIL);
    CREDO_VERUM(linea_ad(&mc, LIV, VII, III) != NIHIL);
    /* H: crassum fictum - bis, secundum uno pixelo dextrorsum */
    CREDO_AEQUALIS_I32(textus_numerus(&mc, LX, ZEPHYRUM), I);
    CREDO_AEQUALIS_I32(textus_numerus(&mc, LXI, ZEPHYRUM), I);
    /* I: color sublineae proprius (SGR 58), etiam in pixelis */
    md = linea_ad(&mc, LXVI, VII, VI);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xFF, ZEPHYRUM, ZEPHYRUM)));
    CREDO_VERUM(ludus_fenestra_imaginem_scribere(mc.lf, VIA_IMAGINIS));
    lecta = imago_caricare_ex_file(VIA_IMAGINIS, piscina);
    CREDO_VERUM(lecta.successus);
    CREDO_VERUM(lecta.successus && pixelum_est(&lecta.imago, LXVII, VII,
        0xFF, ZEPHYRUM, ZEPHYRUM));
    /* J: occultum - nec sublinea */
    CREDO_VERUM(linea_ad(&mc, LXXII, VII, VI) == NIHIL);
    /* geminus terminalis: ornamenta pixelorum remota */
    mc.app.ornamenta_pixelorum = FALSUM;
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(linea_ad(&mc, ZEPHYRUM, VII, VI) == NIHIL);
    CREDO_VERUM(linea_ad(&mc, VI, IV, VI) == NIHIL);
    CREDO_AEQUALIS_I32(textus_numerus(&mc, LXI, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(textus_numerus(&mc, LX, ZEPHYRUM), I);
    terminale_claudere(&mc.app);
}

/* X: schirmus inversus ?5 (D7c; Ghostty render.zig reverse_colors):
 * colores NATIVI permutantur, superficies tota fundo novo; colores
 * expliciti manent; ?5l restituit */
interior vacuum
inversum_probare (vacuum)
{
     Machina  mc;
    Mandatum* md;

    imprimere("\n--- X: schirmus inversus (D7c) ---\n");
    CREDO_VERUM(machinam_effusione(&mc,
        "x\x1B[41my\x1B[0m\x1B[?5h"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_BACKGROUND));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && md->fines.latitudo == LXXX * CELL_X
                   && md->fines.altitudo == XXIV * CELL_Y
                   && color_est(md->color, COLOR_MANDATI_THEMA,
                          (i32)COLOR_TEXT));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, CELL_X, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xCD, ZEPHYRUM, ZEPHYRUM)));
    terminale_claudere(&mc.app);
    CREDO_VERUM(machinam_effusione(&mc, "x\x1B[?5h\x1B[?5l"));
    md = mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    md = mandatum_ad(&mc, MANDATUM_RECTANGULUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM(md == NIHIL || md->fines.latitudo != LXXX * CELL_X);
    terminale_claudere(&mc.app);
}

/* rectangulum quodvis (x, y, latitudo, altitudo) */
interior Mandatum*
rectangulum_ad (
    Machina* mc,
        s32  x,
        s32  y,
        s32  latitudo,
        s32  altitudo)
{
    Mandatum* md;
         i32  i;

    per (i = ZEPHYRUM; i < mandata_numerus(mc->lf->mandata); i++)
    {
        md = mandata_obtinere(mc->lf->mandata, i);
        si (   md->genus          == MANDATUM_RECTANGULUM
            && md->fines.x        == x
            && md->fines.y        == y
            && md->fines.latitudo == latitudo
            && md->fines.altitudo == altitudo)
        {
            redde md;
        }
    }
    redde NIHIL;
}

/* XI: lineae capsarum, quadra, braille ut figurae ductae (D7c,
 * glyphae_ductae): rectangula per cursum ordinis, nullus textus;
 * geminus textum servat; cursor larvam colore fundi pingit */
interior vacuum
ductas_probare (vacuum)
{
     Machina  mc;
    Mandatum* md;

    imprimere("\n--- XI: figurae ductae (D7c) ---\n");
    /* ─ │ ╭ █(rubrum) ░ ⣿ x ; cursor ad (VII, 0) */
    CREDO_VERUM(machinam_effusione(&mc,
        "\xE2\x94\x80\xE2\x94\x82\xE2\x95\xAD\x1B[31m\xE2\x96\x88"
        "\x1B[0m\xE2\x96\x91\xE2\xA3\xBF" "x\x1B[1;8H"));
    /* ─: ordo III totus, colore litterae; nullus textus */
    md = rectangulum_ad(&mc, ZEPHYRUM, III, CELL_X, I);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_THEMA,
        (i32)COLOR_TEXT));
    CREDO_VERUM(mandatum_ad(&mc, MANDATUM_TEXTUS, ZEPHYRUM, ZEPHYRUM)
        == NIHIL);
    /* │: columna II, ordo quisque */
    CREDO_VERUM(rectangulum_ad(&mc, CELL_X + II, ZEPHYRUM, I, I)
        != NIHIL);
    CREDO_VERUM(rectangulum_ad(&mc, CELL_X + II, VII, I, I) != NIHIL);
    /* ╭: ordo III columnae III-V */
    CREDO_VERUM(rectangulum_ad(&mc, II * CELL_X + III, III, III, I)
        != NIHIL);
    /* █ rubrum: ordo 0 totus colore xterm (205, 0, 0) */
    md = rectangulum_ad(&mc, III * CELL_X, ZEPHYRUM, CELL_X, I);
    CREDO_VERUM(md && color_est(md->color, COLOR_MANDATI_RGBA,
        pixelum_rgb(0xCD, ZEPHYRUM, ZEPHYRUM)));
    /* ░: opacitas 0x40 - mixtum, nec littera nec fundus */
    md = rectangulum_ad(&mc, IV * CELL_X, ZEPHYRUM, CELL_X, I);
    CREDO_VERUM(md && md->color.genus == COLOR_MANDATI_RGBA);
    /* ⣿: punctum (I, 0) cellulae V */
    CREDO_VERUM(rectangulum_ad(&mc, V * CELL_X + I, ZEPHYRUM, I, I)
        != NIHIL);
    /* x: textus ut antea */
    CREDO_VERUM(textus_cellulae(&mc, VI, ZEPHYRUM, "x"));
    terminale_claudere(&mc.app);

    /* cursor super ─: larva colore fundi super cursorem */
    CREDO_VERUM(machinam_effusione(&mc,
        "\xE2\x94\x80\x1B[1;1H"));
    md = NIHIL;
    {
        i32 i;

        per (i = ZEPHYRUM; i < mandata_numerus(mc.lf->mandata); i++)
        {
            Mandatum* x;

            x = mandata_obtinere(mc.lf->mandata, i);
            si (   x->genus   == MANDATUM_RECTANGULUM
                && x->fines.y == III && x->fines.altitudo == I
                && color_est(x->color, COLOR_MANDATI_THEMA,
                       (i32)COLOR_BACKGROUND))
            {
                md = x;
            }
        }
    }
    CREDO_VERUM(md != NIHIL);
    terminale_claudere(&mc.app);

    /* crassum: nec textus nec duplicatio */
    CREDO_VERUM(machinam_effusione(&mc, "\x1B[1m\xE2\x94\x80"));
    CREDO_AEQUALIS_I32(textus_numerus(&mc, ZEPHYRUM, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(textus_numerus(&mc, I, ZEPHYRUM), ZEPHYRUM);
    /* geminus: textus ut antea */
    mc.app.ornamenta_pixelorum = FALSUM;
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_VERUM(textus_cellulae(&mc, ZEPHYRUM, ZEPHYRUM,
        "\xE2\x94\x80"));
    CREDO_VERUM(rectangulum_ad(&mc, ZEPHYRUM, III, CELL_X, I) == NIHIL);
    terminale_claudere(&mc.app);
}

/* ratio inter duo pixela mandatorum (oraculum independens) */
interior f64
ratio_pixelorum (
    i32 a,
    i32 b)
{
    f64 la;
    f64 lb;

    la = luminantia((i32)(a & 0xFF), (i32)((a >> VIII) & 0xFF),
        (i32)((a >> XVI) & 0xFF));
    lb = luminantia((i32)(b & 0xFF), (i32)((b >> VIII) & 0xFF),
        (i32)((b >> XVI) & 0xFF));
    redde la > lb ? (la + 0.05) / (lb + 0.05) : (lb + 0.05) / (la
        + 0.05);
}

/* pixelum b inter a et extremum (0 aut CCLV) in omni canali? -
 * propulsio minima colorem SUUM versus nigrum aut album movet */
interior b32
inter_extremum (
    i32 a,
    i32 b,
    i32 extremum)
{
    i32 k;
    i32 x;
    i32 y;

    per (k = ZEPHYRUM; k < XXIV; k += VIII)
    {
        x = (a >> k) & 0xFF;
        y = (b >> k) & 0xFF;
        si (extremum ? (y < x) : (y > x))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* XII: memoria contrastus (park 011) - CCLVI paria (littera, fundus)
 * truecolor, XVI litterae x XVI fundi (quaeque littera cum XVI fundis,
 * quisque fundus cum XVI litteris - collisiones clavem partialem
 * produnt; sub mixtura praesenti VIII collisiones eiusdem fundi et
 * XII eiusdem litterae, computatae - mixtura mutata: plantae M2/M3
 * iterum currendae): par satis contrarium colorem SUUM servat,
 * cetera contra fundum SUUM III attingunt. Memoria cuius clavis
 * fallit (collisio) colorem alterius paris redderet. */
interior vacuum
memoriam_contrastus_probare (vacuum)
{
               Machina  mc;
              Mandatum* textus;
              Mandatum* fundus;
                   i32  k;
                   i32  littera;
                   i32  fundus_rgb;
                   i32  servati;
                   i32  propulsi;
                   b32  boni;
             character  effusio[XVI * MXXIV];
                   i32  n;

    imprimere("\n--- XII: memoria contrastus (park 011) ---\n");
    n = ZEPHYRUM;
    n += (i32)sprintf(effusio + n, "\x1B[2J\x1B[H");
    per (k = ZEPHYRUM; k < CCLVI; k++)
    {
        n += (i32)sprintf(effusio + n,
            "\x1B[38;2;%u;%u;%u;48;2;%u;%u;%umx",
            (unsigned)(((k % XVI) * XXXVII) & 0xFF),
            (unsigned)(((k % XVI) * XCI) & 0xFF),
            (unsigned)(((k % XVI) * LIII) & 0xFF),
            (unsigned)(((k / XVI) * XIII + LXIV) & 0xFF),
            (unsigned)(((k / XVI) * LXXIII) & 0xFF),
            (unsigned)(((k / XVI) * XXIX + CXXVIII) & 0xFF));
    }
    CREDO_VERUM(machinam_effusione(&mc, effusio));
    boni      = VERUM;
    servati   = ZEPHYRUM;
    propulsi  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < CCLVI; k++)
    {
        littera    = pixelum_rgb(((k % XVI) * XXXVII) & 0xFF,
            ((k % XVI) * XCI) & 0xFF, ((k % XVI) * LIII) & 0xFF);
        fundus_rgb = pixelum_rgb(((k / XVI) * XIII + LXIV) & 0xFF,
            ((k / XVI) * LXXIII) & 0xFF,
            ((k / XVI) * XXIX + CXXVIII) & 0xFF);
        textus = mandatum_ad(&mc, MANDATUM_TEXTUS,
            (s32)((k % LXXX) * CELL_X), (s32)((k / LXXX) * CELL_Y));
        fundus = mandatum_ad(&mc, MANDATUM_RECTANGULUM,
            (s32)((k % LXXX) * CELL_X), (s32)((k / LXXX) * CELL_Y));
        si (!textus || !fundus || fundus->color.valor != fundus_rgb)
        {
            boni = FALSUM;
            perge;
        }
        si (ratio_pixelorum(littera, fundus_rgb) >= 3.0)
        {
            servati++;
            si (textus->color.valor != littera)
            {
                boni = FALSUM;
            }
        }
        alioquin
        {
            propulsi++;
            si (   ratio_pixelorum(textus->color.valor, fundus_rgb)
                < 3.0
                || (   !inter_extremum(littera, textus->color.valor,
                           ZEPHYRUM)
                    && !inter_extremum(littera, textus->color.valor,
                           I)))
            {
                boni = FALSUM;
            }
        }
    }
    CREDO_VERUM(boni);
    /* utraque via exercetur */
    CREDO_VERUM(servati > XXXII);
    CREDO_VERUM(propulsi > XXXII);
    terminale_claudere(&mc.app);
}

/* XIII: ramus (vicus-latera S1a) - superficies ex RAMO legitur, non ex
 * radice repositorii proprii: terminale in hospite (vicus) ramum
 * <terminale id> habet */
interior vacuum
ramum_probare (vacuum)
{
               Machina  mc;
    InsulaRepositorium* alter;

    imprimere("\n--- XIII: ramus (vicus-latera S1a) ---\n");
    CREDO_VERUM(machinam_struere(&mc, pseudoterminale_memoriae_creare(
        piscina, (constans i8*)"", ZEPHYRUM, ZEPHYRUM)));
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), LXXX);
    /* repositorium hospitis: ramus t1 cum superficie sua (XX x X
     * cellulae); radix applicationis intacta */
    alter = insula_repositorium_creare(piscina, intern,
        "<vicus><terminale id=\"t1\"/></vicus>",
        "<vicus><terminale id=\"t1\" superficies_latitudo=\"120\""
        " superficies_altitudo=\"80\"/></vicus>");
    CREDO_NON_NIHIL(alter);
    mc.app.ramus = insula_ramus(alter, "terminale", "t1");
    (vacuum)terminale_pulsare(&mc.app, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), XX);
    CREDO_AEQUALIS_I32(aemulator_altitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), X);
    /* compositio quoque ex ramo: radix XX x X cellulae (fines =
     * ictus muris) */
    (vacuum)quadrum(&mc, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(mc.app.hospes)), XX);
    CREDO_NON_NIHIL(dispensator_arbor(mc.app.d));
    CREDO_AEQUALIS_I32((i32)dispensator_arbor(mc.app.d)->fines.latitudo,
        XX * CELL_X);
    terminale_claudere(&mc.app);
}

/* superficies ut hospes (vicus) scribit: scriptor 'dispensator' */
nomen structura {
    s32 latitudo;
    s32 altitudo;
} SuperficiesProbanda;

interior vacuum
superficiem_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans SuperficiesProbanda* sp;

    sp = (constans SuperficiesProbanda*)ctx;
    insula_attributum_ponere(nodus, p, in, "superficies_latitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->latitudo, p), p));
    insula_attributum_ponere(nodus, p, in, "superficies_altitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->altitudo, p), p));
}

interior Canon*
canonem_vici (
    constans character* via)
{
    chorda causa;

    redde canon_legere(filum_legere_totum(via, piscina), piscina,
        intern, &causa);
}

/* XIV: montatio in hospite (vicus-latera S1b) - repositorium cum
 * canonibus VERIS vici (radix liberos non declarat: montatio sine
 * canone suo recusaretur); concha vera (SHELL=/bin/sh) */
interior vacuum
montationem_probare (vacuum)
{
    TerminaleApplicatio  app;
     InsulaRepositorium* repo;
            InsulaRamus  r;
    SuperficiesProbanda  sp;
              Componens* arbor;
                 chorda  textus;
                    i32  k;
                    b32  visum;

    imprimere("\n--- XIV: montatio (vicus-latera S1b) ---\n");
    (vacuum)setenv("SHELL", "/bin/sh", I);
    repo = insula_repositorium_creare(piscina, intern, "<vicus/>",
        "<vicus/>");
    CREDO_NON_NIHIL(repo);
    insula_ponere_canonem(repo, INSULA_DURABILIS,
        canonem_vici("apps/vicus/canones/durabilis.canon"));
    insula_ponere_canonem(repo, INSULA_EPHEMERA,
        canonem_vici("apps/vicus/canones/ephemera.canon"));
    CREDO_VERUM(terminale_montare(&app, piscina, intern, repo, "t1",
        CDLXXX, CXCII));
    /* ramus in utroque genere; dispensator nullus (hospitis est) */
    r = insula_ramus(repo, "terminale", "t1");
    CREDO_NON_NIHIL(insula_ramus_nodus(&r, INSULA_DURABILIS));
    CREDO_NON_NIHIL(insula_ramus_nodus(&r, INSULA_EPHEMERA));
    CREDO_NIHIL(app.d);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(app.hospes)), LXXX);
    /* hospes superficiem scribit: canon terminalis eam admittit */
    insula_scriptorem_ponere(repo, chorda_ex_literis("dispensator",
        piscina));
    sp.latitudo = CXX;
    sp.altitudo = LXXX;
    CREDO_VERUM(mutare_ramum(&r, INSULA_EPHEMERA, superficiem_ponere,
        &sp));
    (vacuum)terminale_pulsare(&app, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_latitudo(
        aemulator_hospes_aemulator(app.hospes)), XX);
    /* componere publica, ctx = applicatio: radix ex ramo */
    arbor = terminale_componere(repo, NIHIL, piscina, intern, &app);
    CREDO_NON_NIHIL(arbor);
    CREDO_VERUM(arbor && arbor->fines.latitudo == CXX);
    /* concha vera currit */
    (vacuum)aemulator_hospes_scribere(app.hospes,
        (constans i8*)"echo salve_montatio\r", XX);
    visum = FALSUM;
    per (k = ZEPHYRUM; k < CC && !visum; k++)
    {
        (vacuum)terminale_pulsare(&app, XX);
        textus = aemulator_textum_effundere(
            aemulator_hospes_aemulator(app.hospes), piscina);
        visum = chorda_continet(textus,
            chorda_ex_literis("salve_montatio\n", piscina));
    }
    CREDO_VERUM(visum);
    /* ambitus bibliothecae ad conchum montatam pervenit */
    (vacuum)aemulator_hospes_scribere(app.hospes,
        (constans i8*)"echo ambitus_$TERM_PROGRAM\r", XXVII);
    visum = FALSUM;
    per (k = ZEPHYRUM; k < CC && !visum; k++)
    {
        (vacuum)terminale_pulsare(&app, XX);
        textus = aemulator_textum_effundere(
            aemulator_hospes_aemulator(app.hospes), piscina);
        visum = chorda_continet(textus,
            chorda_ex_literis("ambitus_terminale\n", piscina));
    }
    CREDO_VERUM(visum);
    /* accessor publicus (S1c): vector quem principalia dant */
    CREDO_VERUM(strcmp(terminale_ambitus()[ZEPHYRUM],
        "TERM=xterm-256color") == ZEPHYRUM);
    CREDO_VERUM(strcmp(terminale_ambitus()[II],
        "TERM_PROGRAM=terminale") == ZEPHYRUM);
    CREDO_VERUM(terminale_ambitus()[III] == NIHIL);
    terminale_claudere(&app);
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
    contrastum_probare();
    ornamenta_probare();
    inversum_probare();
    ductas_probare();
    memoriam_contrastus_probare();
    ramum_probare();
    montationem_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
