/* musivum.c - Spectator Mandatorum in cellulis terminalis (tessellatio
 * T5; terminal verum!)
 *
 * Mandata (index delineandi ludi) per lib/tessellatio in cellulas
 * vertit et per musivum_pingere in tesseram ponit. Scaenae:
 *   1. probatio - ostentatio in pixelis nostris pro modulo VI x VIII
 *      (60 x 20 cellulae): titulus, tabula cum imagine (via pixelorum)
 *      et titulo super eam, textus plurium linguarum, mensa linearum
 *      (┌┬┐├┼┤└┴┘), triangulum et linea obliqua (via pixelorum),
 *      coetus praecisus;
 *   2. pictor.arbor - probationes/pictor/pictor.arbor.stml per pingere
 *      cum figuris MINIMIS (figura_finium + titulus ut pictoris):
 *      figurae pictoris veras nectere Cocoa trahit (pictor_documentum);
 *   3. plagulae Mandatorum STML ex argumentis.
 * Claves: m = modulus (VI x VIII, VIII x XVI, I x I); ] / dextra =
 * scaena proxima; [ / sinistra = prior; q = exire. Amplitudo mutata:
 * extensio = scrinium sine linea status.
 *
 * Curre per: ./tessera/musivum.sh [mandata.stml ...]
 * Sine terminali: musivum -textus COLUMNAE LINEAE [plagulae...] -
 * scaenas omnes (modulo VI x VIII) ut textum UTF-8 sine coloribus
 * imprimit (inspectio, exemplaria).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "thema.h"
#include "color.h"
#include "imago.h"
#include "imago_opus.h"
#include "mandatum.h"
#include "componens.h"
#include "figura.h"
#include "modulus.h"
#include "tessellatio.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_posix.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include "musivum_pictura.h"
#include "utf8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCAENAE_MAXIMAE  XXXII
#define FUNDUS           0x000000

nomen structura {
    constans character* titulus;
               Mandata* mandata;
} MusivumScaena;

nomen structura {
    s32 latitudo;
    s32 altitudo;
} MusivumMetrum;

/* Fons imaginum: "assumptio" (photographia Frani, ad 57 x 80 scalata) */
interior constans Imago*
_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    constans Imago* im = (constans Imago*)ctx;

    si (   im != NIHIL && im->pixela != NIHIL
        && chorda_aequalis_literis(provenientia, "assumptio"))
    {
        redde im;
    }
    redde NIHIL;
}

interior ColorMandati
_rgb (
    i32 rgb)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_RGBA;
    c.valor = color_ad_pixelum(color_ex_rgba((i8)((rgb >> XVI) & 0xFF),
        (i8)((rgb >> VIII) & 0xFF), (i8)(rgb & 0xFF), (i8)CCLV));
    redde c;
}

interior Fines
_fines (
    s32 x,
    s32 y,
    s32 lat,
    s32 alt)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = lat;
    f.altitudo  = alt;
    redde f;
}

interior Punctum
_punctum (
    s32 x,
    s32 y)
{
    Punctum p;

    p.x = x;
    p.y = y;
    redde p;
}

interior vacuum
_textus (
                Mandata* m,
                    s32  x,
                    s32  y,
     constans character* cstr,
                    i32  rgb)
{
    mandata_textus(m, x, y, chorda_ex_literis(cstr, m->piscina),
        ZEPHYRUM,
        _rgb(rgb));
}

/* Scaena 1: ostentatio (pixela nostra, VI x VIII: 360 x 160) */
interior Mandata*
_scaena_probationis (
                Piscina* piscina,
    InternamentumChorda* intern)
{
     Mandata* m;
     Punctum  triangulum[III];
         i32  c;
         s32  k;

    m = mandata_creare(piscina, intern);
    mandata_rectangulum(m, _fines(ZEPHYRUM, ZEPHYRUM, CCCLX, VIII),
        _rgb(0x1F3A93), VERUM);
    _textus(m, VI, ZEPHYRUM,
        "musivum - Mandata in cellulis (tessellatio)", 0xFFFFFF);
    /* tabula cum imagine et titulo super eam */
    mandata_rectangulum(m, _fines(ZEPHYRUM, XVI, CLXXX, CIV),
        _rgb(0xFFFFFF), FALSUM);
    mandata_imago(m, chorda_ex_literis("assumptio", piscina),
        _fines(XII, XXIV, LVII, LXXX));
    _textus(m, XII, XCVI, "assumptio", 0xFFFFFF);
    _textus(m, LXXXIV, XXIV, "lingua:", 0xC0C0C0);
    _textus(m, LXXXIV, XL,
        "\xE4\xB8\xAD\xE6\x96\x87 \xE6\xBC\xA2\xE5\xAD\x97",
        0xFFFFFF);
    _textus(m, LXXXIV, LVI, "\xC3\xA9t\xC3\xA9 \xCE\xB5\xCE\xBB\xCE\xBB"
        "\xCE\xB7\xCE\xBD\xCE\xB9\xCE\xBA\xCE\xAC", 0xFFFFFF);
    _textus(m, LXXXIV, LXXII, "familia: \xF0\x9F\x91\xA8\xE2\x80\x8D"
        "\xF0\x9F\x91\xA9\xE2\x80\x8D\xF0\x9F\x91\xA7", 0xFFFFFF);
    /* mensa linearum: juncturae ex intersectionibus */
    per (k = ZEPHYRUM; k < IV; k++)
    {
        mandata_linea(m, _punctum(CXCII, XX + k * XXIV),
            _punctum(CCCXLVIII, XX + k * XXIV), I, _rgb(0x80C0FF));
    }
    mandata_linea(m, _punctum(CXCII, XX), _punctum(CXCII, XCII), I,
        _rgb(0x80C0FF));
    mandata_linea(m, _punctum(CCLXX, XX), _punctum(CCLXX, XCII), I,
        _rgb(0x80C0FF));
    mandata_linea(m, _punctum(CCCXLVIII, XX), _punctum(CCCXLVIII, XCII),
        I, _rgb(0x80C0FF));
    _textus(m, CXCVIII, XXVIII, "lingua", 0xFFFF80);
    _textus(m, CCLXXVI, XXVIII, "salutatio", 0xFFFF80);
    _textus(m, CXCVIII, LII, "Latina", 0xFFFFFF);
    _textus(m, CCLXXVI, LII, "Salve!", 0xFFFFFF);
    _textus(m, CXCVIII, LXXVI, "\xE4\xB8\xAD\xE6\x96\x87", 0xFFFFFF);
    _textus(m, CCLXXVI, LXXVI, "\xE4\xBD\xA0\xE5\xA5\xBD", 0xFFFFFF);
    /* via pixelorum: triangulum, linea obliqua */
    triangulum[ZEPHYRUM]  = _punctum(CXCII, CVIII);
    triangulum[I]         = _punctum(CCCXLVIII, CVIII);
    triangulum[II]        = _punctum(CCLXX, CXL);
    mandata_polygonum(m, triangulum, III, _rgb(0x30A050), VERUM);
    mandata_linea(m, _punctum(ZEPHYRUM, CLII), _punctum(CLXXX, CXXVIII),
        I, _rgb(0xFFD700));
    /* coetus praecisus: X cellulae */
    c = mandata_coetus_incipere(m, _fines(CXCII, CXLIV, LX, VIII),
        VERUM,
        ZEPHYRUM, ZEPHYRUM, I, chorda_ex_literis("praecisio", piscina));
    _textus(m, ZEPHYRUM, ZEPHYRUM,
        "praecisum: hic textus longe ultra fines currit", 0xFF8080);
    mandata_coetus_finire(m, c);
    redde m;
}

/* Figura tituli minima (ut figura_tituli pictoris: fundus + textus ad
 * 2,2) - figurae pictoris verae Cocoa trahunt */
interior vacuum
_figura_tituli (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    ColorMandati fundus;
    ColorMandati littera;

    (vacuum)thema;
    (vacuum)ctx;
    fundus.genus   = COLOR_MANDATI_THEMA;
    fundus.valor   = (i32)COLOR_BACKGROUND;
    littera.genus  = COLOR_MANDATI_THEMA;
    littera.valor  = (i32)COLOR_TEXT;
    mandata_rectangulum(m, _fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), fundus, VERUM);
    mandata_textus(m, II, II, c->titulus, ZEPHYRUM, littera);
}

/* Scaena 2: pictor.arbor per pingere (NIHIL si plagula abest) */
interior Mandata*
_scaena_pictoris (
             constans character* via,
                        Piscina* piscina,
            InternamentumChorda* intern)
{
              chorda  fons;
           character* cstr;
           Componens* radix;
     FiguraRegistrum* reg;
             Mandata* m;
                 i32  p;

    fons = filum_legere_totum(via, piscina);
    si (fons.mensura <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    cstr = (character*)piscina_allocare(piscina,
        (memoriae_index)fons.mensura + I);
    memcpy(cstr, fons.datum, (memoriae_index)fons.mensura);
    cstr[fons.mensura]  = '\0';
    radix               = componens_legere_stml(cstr, piscina, intern);
    reg                 = figura_registrum_creare(piscina);
    si (radix == NIHIL || reg == NIHIL)
    {
        redde NIHIL;
    }
    per (p = ZEPHYRUM; p < (i32)PARTES_NUMERUS; p++)
    {
        si (p == (i32)PARTES_TITULUS)
        {
            figura_registrare(reg, (Partes)p, ZEPHYRUM, _figura_tituli,
                NIHIL);
        }
        alioquin
        {
            figura_registrare(reg, (Partes)p, ZEPHYRUM, figura_finium,
                NIHIL);
        }
    }
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    redde m;
}

/* Scaena ex plagula Mandatorum STML */
interior Mandata*
_scaena_plagulae (
             constans character* via,
                        Piscina* piscina,
            InternamentumChorda* intern)
{
       chorda  fons;
    character* cstr;

    fons = filum_legere_totum(via, piscina);
    si (fons.mensura <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    cstr = (character*)piscina_allocare(piscina,
        (memoriae_index)fons.mensura + I);
    memcpy(cstr, fons.datum, (memoriae_index)fons.mensura);
    cstr[fons.mensura] = '\0';
    redde mandata_legere_stml(cstr, piscina, intern);
}

/* Scaenam ut textum imprimere (modulus VI x VIII; sine coloribus):
 * unitas, junctura per runam, spatium; continuatio nihil. */
interior vacuum
_textum_imprimere (
    constans MusivumScaena* scaena,
                       s32  columnae,
                       s32  lineae,
                     Imago* imago)
{
             Piscina* arena;
             Modulus  modulus;
  TessellatioCellula* cellulae;
                 s32  i;
                 s32  j;
                 s32  runa;
                  i8  octeti[IV];
                 s32  n;

    arena = piscina_generare_dynamicum("musivum_textus", 16777216);
    si (arena == NIHIL)
    {
        redde;
    }
    modulus   = modulus_creare(VI, VIII, columnae * VI, lineae * VIII);
    cellulae  = (TessellatioCellula*)piscina_allocare_ordinatum(arena,
        (memoriae_index)(columnae * lineae) * magnitudo(TessellatioCellula),
        VIII);
    tessellatio_computare(scaena->mandata, &modulus,
        RUNAE_POLITICA_GRAPHEMATUM, FUNDUS, _fons, (vacuum*)imago,
        arena,
        cellulae);
    printf("=== %s (%dx%d, VI x VIII)\n", scaena->titulus,
        (int)columnae,
        (int)lineae);
    per (j = ZEPHYRUM; j < lineae; j++)
    {
        printf("|");
        per (i = ZEPHYRUM; i < columnae; i++)
        {
            constans TessellatioCellula* c = &cellulae[j * columnae
                + i];

            si (c->latitudo == ZEPHYRUM)
            {
                perge;
            }
            si (c->unitas != NIHIL)
            {
                printf("%.*s", (int)c->mensura, (constans character*)
                    c->unitas);
                perge;
            }
            runa = tessellatio_runa_juncturae(c->juncturae);
            n = runa > ZEPHYRUM ? utf8_codere(runa,
                octeti) : ZEPHYRUM;
            si (n > ZEPHYRUM)
            {
                printf("%.*s", (int)n, (constans character*)octeti);
            }
            alioquin
            {
                printf(" ");
            }
        }
        printf("|\n");
    }
    piscina_destruere(arena);
}

interior vacuum
_pingere (
                  TesseraOpus* opus,
       constans MusivumScaena* scaena,
                          i32  index,
                          i32  numerus,
       constans MusivumMetrum* metrum,
                        Imago* imago)
{
    TesseraStilus status  = tessera_stilus(TESSERA_COLOR_NATIVUS,
        TESSERA_COLOR_NATIVUS, TESSERA_ORNAMENTUM_INVERSUM);
              i32 lat     = tessera_latitudo(opus);
              i32 alt     = tessera_altitudo(opus) > I
                  ? tessera_altitudo(opus) - I : I;
          Piscina* arena   =
              piscina_generare_dynamicum("musivum_quadrum",
              16777216);
          Modulus modulus;
              s32 columnae  = ZEPHYRUM;
              s32 lineae    = ZEPHYRUM;
        character linea[CCLVI];

    tessera_purgare(opus, tessera_stilus_nativus());
    modulus = modulus_creare(metrum->latitudo, metrum->altitudo,
        (s32)lat * metrum->latitudo, (s32)alt * metrum->altitudo);
    modulus_extensio_cellularum(&modulus, &columnae, &lineae);
    si (arena != NIHIL)
    {
        TessellatioCellula* cellulae = (TessellatioCellula*)
            piscina_allocare_ordinatum(arena, (memoriae_index)(columnae
                * lineae) * magnitudo(TessellatioCellula), VIII);

        si (cellulae != NIHIL)
        {
            tessellatio_computare(scaena->mandata, &modulus,
                (opus->politica == TESSERA_POLITICA_SIMPLEX)
                    ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM,
                FUNDUS, _fons, (vacuum*)imago, arena, cellulae);
            musivum_pingere(opus, ZEPHYRUM, ZEPHYRUM, cellulae,
                columnae,
                lineae);
        }
        piscina_destruere(arena);
    }
    tessera_replere(opus, ZEPHYRUM, (s32)alt, (s32)lat, I, (i32)' ',
        status);
    sprintf(linea,
        " %s %u/%u | modulus %dx%d | %dx%d cellulae | %s | m ] "
        "[ q", scaena->titulus, (insignatus integer)(index + I),
        (insignatus integer)numerus, (int)metrum->latitudo,
        (int)metrum->altitudo, (int)columnae, (int)lineae,
        (opus->colores == TESSERA_COLORES_CCLVI) ? "CCLVI" : "PLENI");
    tessera_scribere_literis(opus, ZEPHYRUM, (s32)alt, linea, status);
    tessera_cursorem_ponere(opus, -I, -I);
    (vacuum)tessera_praesentare(opus);
}

s32
principale (
                    s32  numerus_argumentorum,
     constans character* argumenta[])
{
                Piscina* piscina;
    InternamentumChorda* intern;
            TesseraPons* pons;
            TesseraOpus* opus;
          TesseraLector* initus;
           MusivumScaena  scaenae[SCAENAE_MAXIMAE];
           MusivumMetrum  metra[III];
                   Imago  imago;
             ImagoFructus  f;
                     i32  numerus  = ZEPHYRUM;
                     i32  index    = ZEPHYRUM;
                     i32  metrum   = ZEPHYRUM;
                     b32  currens  = VERUM;
                     s32  k;

    piscina = piscina_generare_dynamicum("musivum", 67108864);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "musivum: piscina deest\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();
    metra[0].latitudo = VI;    metra[0].altitudo = VIII;
    metra[1].latitudo = VIII;  metra[1].altitudo = XVI;
    metra[2].latitudo = I;     metra[2].altitudo = I;

    imago.pixela    = NIHIL;
    imago.latitudo  = ZEPHYRUM;
    imago.altitudo  = ZEPHYRUM;
    f =
        imago_caricare_ex_file("probationes/fixa/quadrans/assumptio.jpg",
        piscina);
    si (f.successus)
    {
        imago = imago_scalare(&f.imago, LVII, LXXX, IMAGO_SCALA_AREA,
            piscina);
    }

    scaenae[numerus].titulus = "probatio";
    scaenae[numerus].mandata = _scaena_probationis(piscina, intern);
    numerus++;
    scaenae[numerus].mandata  = _scaena_pictoris(
        "probationes/pictor/pictor.arbor.stml", piscina, intern);
    si (scaenae[numerus].mandata != NIHIL)
    {
        scaenae[numerus].titulus = "pictor.arbor (figurae minimae)";
        numerus++;
    }
    k = I;
    si (   numerus_argumentorum            >= IV
        && strcmp(argumenta[I], "-textus") == ZEPHYRUM)
    {
        k = IV;
    }
    per (; k < numerus_argumentorum && numerus < SCAENAE_MAXIMAE; k++)
    {
        scaenae[numerus].mandata = _scaena_plagulae(argumenta[k],
            piscina,
            intern);
        si (scaenae[numerus].mandata == NIHIL)
        {
            fprintf(stderr, "musivum: %s legi non potest\n",
                argumenta[k]);
            perge;
        }
        scaenae[numerus].titulus = argumenta[k];
        numerus++;
    }

    si (   numerus_argumentorum            >= IV
        && strcmp(argumenta[I], "-textus") == ZEPHYRUM)
    {
        s32 col = (s32)atoi(argumenta[II]);
        s32 lin = (s32)atoi(argumenta[III]);
        i32 j;

        per (j = ZEPHYRUM; j < numerus; j++)
        {
            _textum_imprimere(&scaenae[j], col > ZEPHYRUM ? col : LX,
                lin > ZEPHYRUM ? lin : XX, &imago);
        }
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    pons = tessera_pons_posix_creare(piscina);
    si (pons == NIHIL)
    {
        fprintf(stderr,
            "musivum: terminal verum requiritur (isatty)\n");
        redde I;
    }
    opus    = tessera_aperire(piscina, pons);
    initus  = tessera_lector_creare(piscina, pons);
    si (opus == NIHIL || initus == NIHIL)
    {
        fprintf(stderr, "musivum: apertura fracta\n");
        redde I;
    }
    tessera_politicam_ponere(opus, tessera_politica_ambitus());
    tessera_colores_ponere(opus, tessera_colores_ambitus());

    dum (currens)
    {
        TesseraEventum ev;

        _pingere(opus, &scaenae[index], index, numerus, &metra[metrum],
            &imago);
        commutatio (tessera_eventum_expectare(initus, &ev, -I))
        {
            casus TESSERA_EVENTUM_CLAVIS:
                si (ev.runa == (s32)'q')
                {
                    currens = FALSUM;
                }
                alioquin si (ev.runa == (s32)'m')
                {
                    metrum = (metrum + I) % III;
                }
                alioquin si (   ev.runa   == (s32)']'
                             || ev.clavis == TESSERA_CLAVIS_DEXTRA)
                {
                    index = (index + I) % numerus;
                }
                alioquin si (   ev.runa   == (s32)'['
                             || ev.clavis == TESSERA_CLAVIS_SINISTRA)
                {
                    index = (index + numerus - I) % numerus;
                }
                frange;
            casus TESSERA_EVENTUM_AMPLITUDO:
            casus TESSERA_EVENTUM_RESUMPTUM:
                tessera_magnitudinem_renovare(opus);
                frange;
            ordinarius:
                frange;
        }
    }

    tessera_claudere(opus);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
