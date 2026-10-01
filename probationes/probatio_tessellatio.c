/* probatio_tessellatio.c - Mandata in cellulas (tessellatio T3: via
 * cellularum). Scaena principalis MANU PRAEDICTA ante codicem. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "color.h"
#include "thema.h"
#include "utf8.h"
#include "mandatum.h"
#include "modulus.h"
#include "tessellatio.h"
#include "imago_typus.h"
#include "componens.h"
#include "figura.h"
#include "filum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define ALBUS   0xFFFFFF
#define NIGER   0x000000
#define CAERULEUS  0x0000FF
#define FLAVUS  0xFFFF00
#define VIRIDIS 0x00FF00

interior ColorMandati
_rgba (
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

/* Linea cellularum -> UTF-8 (unitas; juncturae per runam; spatium;
 * continuatio nihil scribit) */
interior vacuum
_linea_textus (
    constans TessellatioCellula* c,
                            s32  columnae,
                            s32  linea,
                      character* exitus)
{
    s32 i;
    s32 n;
    s32 j;
     i8 octeti[IV];
    s32 runa;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < columnae; i++)
    {
        constans TessellatioCellula* x = &c[linea * columnae + i];

        si (x->latitudo == ZEPHYRUM)
        {
            perge;
        }
        si (x->unitas != NIHIL)
        {
            per (j = ZEPHYRUM; j < (s32)x->mensura; j++)
            {
                exitus[n++] = (character)x->unitas[j];
            }
            perge;
        }
        runa = tessellatio_runa_juncturae(x->juncturae);
        si (runa > ZEPHYRUM)
        {
            s32 k = utf8_codere(runa, octeti);

            per (j = ZEPHYRUM; j < k; j++)
            {
                exitus[n++] = (character)octeti[j];
            }
            perge;
        }
        exitus[n++] = ' ';
    }
    exitus[n] = '\0';
}

interior b32
_scaena_congruit (
    constans TessellatioCellula*  c,
                            s32   columnae,
                            s32   lineae,
             constans character** exspectata)
{
    character linea[DXII];
          s32 l;
          b32 congruit;

    congruit = VERUM;
    per (l = ZEPHYRUM; l < lineae; l++)
    {
        _linea_textus(c, columnae, l, linea);
        si (strcmp(linea, exspectata[l]) != ZEPHYRUM)
        {
            imprimere("  linea %d:\n    habita  [%s]\n    exspect [%s]\n",
                (int)l, linea, exspectata[l]);
            congruit = FALSUM;
        }
    }
    redde congruit;
}

/* Fons imaginum probationis: "rubrum" 12x16, "dimidium" 3x8 (ambo
 * rubra opaca) */
nomen structura {
    Imago rubrum;
    Imago dimidium;
} ImaginesProbationis;

interior constans Imago*
_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    ImaginesProbationis* imagines = (ImaginesProbationis*)ctx;

    si (chorda_aequalis_literis(provenientia, "rubrum"))
    {
        redde &imagines->rubrum;
    }
    si (chorda_aequalis_literis(provenientia, "dimidium"))
    {
        redde &imagines->dimidium;
    }
    redde NIHIL;
}

interior Imago
_imago_rubra (
    Piscina* piscina,
        i32  lat,
        i32  alt)
{
    Imago im;
      i32 k;

    im.latitudo = lat;
    im.altitudo = alt;
    im.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    per (k = ZEPHYRUM; k < lat * alt; k++)
    {
        im.pixela[k * IV]        = (i8)CCLV;
        im.pixela[k * IV + I]    = ZEPHYRUM;
        im.pixela[k * IV + II]   = ZEPHYRUM;
        im.pixela[k * IV + III]  = (i8)CCLV;
    }
    redde im;
}

/* Figura tituli ut pictoris (fundus + textus ad 2,2) - pro scaena
 * pictor.arbor (figurae pictoris verae Cocoa trahunt, parcum 003) */
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

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Mandata* m;
                 Modulus  modulus;
      TessellatioCellula  cellulae[CLX];
      TessellatioCellula* c;
                     i32  coetus;
                     s32  columnae;
                     s32  lineae;
                     i32  k;
      constans character* scaena_a[VIII];
      constans character* nexus[XVI];
                 Piscina* pixela;
     ImaginesProbationis  imagines;
                 Punctum  triangulum[III];

    piscina = piscina_generare_dynamicum("probatio_tessellatio",
        CCLVI * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- I. juncturae -> runae ---\n");
    nexus[0]   = " ";  nexus[1]  = "│"; nexus[2]  = "─"; nexus[3]  =
                                                                 "└";
    nexus[4]   = "│"; nexus[5]  = "│"; nexus[6]  =
                                                                  "┌"; nexus[7] =
                                                                             "├";
    nexus[8] = "─"; nexus[9]  = "┘"; nexus[10] =
                                                                  "─"; nexus[11] =
                                                                             "┴";
    nexus[12] = "┐"; nexus[13] = "┤"; nexus[14] =
                                                                  "┬"; nexus[15] =
                                                                             "┼";
    per (k = ZEPHYRUM; k < XVI; k++)
    {
         i8 octeti[IV];
        s32 runa = tessellatio_runa_juncturae(k);
        s32 n = runa > ZEPHYRUM ? utf8_codere(runa,
            octeti) : ZEPHYRUM;

        si (k == ZEPHYRUM)
        {
            CREDO_AEQUALIS_S32(runa, ZEPHYRUM);
            perge;
        }
        CREDO_VERUM(n > ZEPHYRUM
            && (s32)strlen(nexus[k]) == n
            && memcmp(octeti, nexus[k], (memoriae_index)n) == ZEPHYRUM);
    }
    CREDO_VERUM(tessellatio_mensor(RUNAE_POLITICA_SIMPLEX).genus
        == MODULUS_MENSOR_RUNARUM);

    imprimere("\n--- II. scaena principalis (praedicta) ---\n");
    modulus = modulus_creare(VI, VIII, CXX, LXIV);
    modulus_extensio_cellularum(&modulus, &columnae, &lineae);
    CREDO_AEQUALIS_S32(columnae, XX);
    CREDO_AEQUALIS_S32(lineae, VIII);
    m = mandata_creare(piscina, intern);
    mandata_rectangulum(m, _fines(VI, VIII, LX, XXIV), _rgba(CAERULEUS),
        VERUM);
    mandata_rectangulum(m, _fines(LXVI, VIII, XLVIII, XXXII),
        _rgba(ALBUS), FALSUM);
    mandata_textus(m, XII, XVI,
        chorda_ex_literis("Ok \xE4\xB8\xAD\xC3\xA9", piscina), ZEPHYRUM,
        _rgba(ALBUS));
    mandata_linea(m, _punctum(ZEPHYRUM, LVI), _punctum(CXIV, LVI), I,
        _rgba(FLAVUS));
    mandata_linea(m, _punctum(XC, ZEPHYRUM), _punctum(XC, LXIII), I,
        _rgba(VIRIDIS));
    coetus = mandata_coetus_incipere(m, _fines(ZEPHYRUM, XL, XXX, XVI),
        VERUM, ZEPHYRUM, ZEPHYRUM, I, chorda_ex_literis("sectio",
        piscina));
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis("clipped", piscina), ZEPHYRUM, _rgba(ALBUS));
    mandata_textus(m, ZEPHYRUM, VIII,
        chorda_ex_literis("abcd\xE4\xB8\xAD", piscina), ZEPHYRUM,
        _rgba(ALBUS));
    mandata_coetus_finire(m, coetus);
    coetus = mandata_coetus_incipere(m, _fines(LXVI, XLVIII, ZEPHYRUM,
        ZEPHYRUM), FALSUM, ZEPHYRUM, ZEPHYRUM, II,
        chorda_ex_literis("scala", piscina));
    mandata_textus(m, III, ZEPHYRUM, chorda_ex_literis("xy", piscina),
        ZEPHYRUM, _rgba(ALBUS));
    mandata_coetus_finire(m, coetus);
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    scaena_a[0] = "               │    ";
    scaena_a[1] = "           ┌───┼──┐ ";
    scaena_a[2] = "  Ok 中é   │   │  │ ";
    scaena_a[3] = "           │   │  │ ";
    scaena_a[4] = "           └───┼──┘ ";
    scaena_a[5] = "clipp          │    ";
    scaena_a[6] = "abcd        xy │    ";
    scaena_a[7] =
        "───────────────┴────";
    CREDO_VERUM(_scaena_congruit(cellulae, XX, VIII, scaena_a));
    /* colores */
    c = &cellulae[II * XX + II];                          /* 'O' */
    CREDO_AEQUALIS_I32(c->color_litterae, ALBUS);
    CREDO_AEQUALIS_I32(c->color_fundi, CAERULEUS);           /* fundus manet */
    CREDO_AEQUALIS_I32(cellulae[II * XX + V].latitudo, II);   /* 中 */
    CREDO_AEQUALIS_I32(cellulae[II * XX + VI].latitudo, ZEPHYRUM);
    CREDO_AEQUALIS_I32(cellulae[II * XX + VI].color_fundi, CAERULEUS);
    CREDO_AEQUALIS_I32(cellulae[I * XX + I].color_fundi, CAERULEUS);
    CREDO_AEQUALIS_I32(cellulae[I * XX + X].color_fundi, CAERULEUS);
    CREDO_AEQUALIS_I32(cellulae[I * XX + XI].color_fundi, NIGER);
    CREDO_AEQUALIS_I32(cellulae[IV * XX + X].color_fundi, NIGER);
    CREDO_AEQUALIS_I32(cellulae[I * XX + XI].color_litterae, ALBUS);
    CREDO_AEQUALIS_I32(cellulae[I * XX + XV].color_litterae, VIRIDIS);
    CREDO_AEQUALIS_I32(cellulae[VII * XX + ZEPHYRUM].color_litterae,
        FLAVUS);
    CREDO_AEQUALIS_I32(cellulae[VII * XX + XV].color_litterae, VIRIDIS);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL
        && cellulae[ZEPHYRUM].juncturae == ZEPHYRUM
        && cellulae[ZEPHYRUM].color_fundi == NIGER);

    imprimere("\n--- III. regulae tegendi ---\n");
    modulus = modulus_creare(VI, VIII, XXXVI, XVI);       /* 6 x 2 */
    /* unitas super continuationem: dimidium primum spatium fit */
    m = mandata_creare(piscina, intern);
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis("\xE4\xB8\xAD", piscina), ZEPHYRUM,
        _rgba(ALBUS));
    mandata_textus(m, VI, ZEPHYRUM, chorda_ex_literis("a", piscina),
        ZEPHYRUM, _rgba(ALBUS));
    /* unitas super dimidium primum: continuatio spatium fit */
    mandata_textus(m, XVIII, ZEPHYRUM,
        chorda_ex_literis("\xE4\xB8\xAD", piscina), ZEPHYRUM,
        _rgba(ALBUS));
    mandata_textus(m, XVIII, ZEPHYRUM, chorda_ex_literis("b", piscina),
        ZEPHYRUM, _rgba(ALBUS));
    /* textus negativus: pars extra cadit; '\n' ad lineam proximam */
    mandata_textus(m, -VI, VIII, chorda_ex_literis("yz\nq", piscina),
        ZEPHYRUM, _rgba(ALBUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL
        && cellulae[ZEPHYRUM].latitudo == I);
    CREDO_VERUM(cellulae[I].unitas != NIHIL
        && cellulae[I].unitas[0] == 'a');
    CREDO_VERUM(cellulae[III].unitas != NIHIL
        && cellulae[III].unitas[0] == 'b');
    CREDO_VERUM(cellulae[IV].unitas == NIHIL
        && cellulae[IV].latitudo == I);
    CREDO_VERUM(cellulae[VI].unitas != NIHIL
        && cellulae[VI].unitas[0] == 'z');
    /* 'q' post '\n' ad columnam initii (-1) cadit: extra */
    CREDO_VERUM(cellulae[VII].unitas == NIHIL);
    /* linea textum tegit; rectangulum impletum juncturas purgat */
    m = mandata_creare(piscina, intern);
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis("abc", piscina), ZEPHYRUM, _rgba(ALBUS));
    mandata_linea(m, _punctum(VI, ZEPHYRUM), _punctum(VI, XV), I,
        _rgba(FLAVUS));
    mandata_linea(m, _punctum(ZEPHYRUM, VIII), _punctum(XXXV, VIII), I,
        _rgba(FLAVUS));
    mandata_rectangulum(m, _fines(XXIV, VIII, XII, VIII),
        _rgba(CAERULEUS),
        VERUM);
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    CREDO_VERUM(cellulae[I].unitas == NIHIL
        && cellulae[I].juncturae == TESSELLATIO_JUNCTURA_DEORSUM);
    CREDO_AEQUALIS_I32(cellulae[VI + I].juncturae,
        TESSELLATIO_JUNCTURA_SURSUM | TESSELLATIO_JUNCTURA_DEXTRA
        | TESSELLATIO_JUNCTURA_SINISTRA);
    CREDO_AEQUALIS_I32(cellulae[VI + IV].juncturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(cellulae[VI + V].juncturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(cellulae[VI + V].color_fundi, CAERULEUS);
    CREDO_AEQUALIS_I32(cellulae[VI + III].juncturae,
        TESSELLATIO_JUNCTURA_DEXTRA | TESSELLATIO_JUNCTURA_SINISTRA);
    /* evanescunt: rectangulum 1x1 non impletum; impletum cuius margines
     * ad eandem marginem cellulae rotundantur (13 -> 12, 14 -> 12).
     * NB: (13, 2 lat) NON evanescit - margo 15 = 2,5 cellulae, dimidium
     * sursum -> 18: regula marginum, non "parva evanescunt". */
    m = mandata_creare(piscina, intern);
    mandata_rectangulum(m, _fines(ZEPHYRUM, ZEPHYRUM, VI, VIII),
        _rgba(ALBUS), FALSUM);
    mandata_rectangulum(m, _fines(XIII, II, I, III), _rgba(CAERULEUS),
        VERUM);
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    per (k = ZEPHYRUM; k < XII; k++)
    {
        CREDO_VERUM(cellulae[k].juncturae == ZEPHYRUM
            && cellulae[k].color_fundi == NIGER);
    }

    /* anguli: fines linearum bita sua ferunt (┘ └ ┐ ┌, non ─) - bita
     * finis solum in angulo videntur */
    {
        constans character* anguli[II];

        m = mandata_creare(piscina, intern);
        /* ┘ in (2,1): horizontalis 0..2 linea 1, verticalis col 2
         * lineae 0..1; ┌ in (3,0): horizontalis 3..5, verticalis col 3
         * lineae 0..1 */
        mandata_linea(m, _punctum(ZEPHYRUM, VIII), _punctum(XII, VIII),
            I,
            _rgba(ALBUS));
        mandata_linea(m, _punctum(XII, ZEPHYRUM), _punctum(XII, VIII),
            I,
            _rgba(ALBUS));
        mandata_linea(m, _punctum(XVIII, ZEPHYRUM), _punctum(XXX,
            ZEPHYRUM), I, _rgba(ALBUS));
        mandata_linea(m, _punctum(XVIII, ZEPHYRUM), _punctum(XVIII,
            VIII),
            I, _rgba(ALBUS));
        tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
            NIGER, NIHIL, NIHIL, NIHIL, cellulae);
        anguli[ZEPHYRUM]  = "  │┌──";
        anguli[I]         = "──┘│  ";
        CREDO_VERUM(_scaena_congruit(cellulae, VI, II, anguli));
    }

    imprimere("\n--- IV. colores: THEMA, INDEX ---\n");
    m = mandata_creare(piscina, intern);
    {
        ColorMandati thema_c;
        ColorMandati index_c;
               Color expect;

        thema_c.genus = COLOR_MANDATI_THEMA;
        thema_c.valor = (i32)COLOR_TEXT;
        index_c.genus = COLOR_MANDATI_INDEX;
        index_c.valor = VII;
        mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
            chorda_ex_literis("t", piscina), ZEPHYRUM, thema_c);
        mandata_rectangulum(m, _fines(VI, ZEPHYRUM, VI, VIII), index_c,
            VERUM);
        tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
            NIGER, NIHIL, NIHIL, NIHIL, cellulae);
        expect = thema_color(COLOR_TEXT);
        CREDO_AEQUALIS_I32(cellulae[ZEPHYRUM].color_litterae,
            ((i32)expect.r << XVI) | ((i32)expect.g << VIII)
                | expect.b);
        expect = thema_color_ex_indice_colorationis((i8)VII);
        CREDO_AEQUALIS_I32(cellulae[I].color_fundi,
            ((i32)expect.r << XVI) | ((i32)expect.g << VIII)
                | expect.b);
    }

    imprimere("\n--- V. via pixelorum (T4, praedicta) ---\n");
    pixela       = piscina_generare_dynamicum("tessellatio_pixela",
        IV * M * M);
    imagines.rubrum    = _imago_rubra(piscina, XII, XVI);
    imagines.dimidium  = _imago_rubra(piscina, III, VIII);
    modulus            = modulus_creare(VI, VIII, XXX, XXIV);     /* 5 x 3 */
    /* A + C + D: imago congrua; titulus super eam; rectangulum post */
    m = mandata_creare(piscina, intern);
    mandata_imago(m, chorda_ex_literis("rubrum", piscina),
        _fines(VI, VIII, XII, XVI));
    mandata_textus(m, VI, VIII, chorda_ex_literis("ab", piscina),
        ZEPHYRUM, _rgba(ALBUS));
    mandata_rectangulum(m, _fines(XII, XVI, VI, VIII), _rgba(CAERULEUS),
        VERUM);
    /* B: dimidium sinistrum cellulae (1,0) */
    mandata_imago(m, chorda_ex_literis("dimidium", piscina),
        _fines(VI, ZEPHYRUM, III, VIII));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        _fons, &imagines, pixela, cellulae);
    c = &cellulae[V + I];                                     /* (1,1) */
    CREDO_VERUM(c->unitas != NIHIL && c->unitas[0] == 'a');
    CREDO_AEQUALIS_I32(c->color_fundi, 0xFF0000);
    CREDO_AEQUALIS_I32(c->color_litterae, ALBUS);
    CREDO_AEQUALIS_I32(cellulae[V + II].color_fundi, 0xFF0000);
    c = &cellulae[X + I];                                     /* (1,2) */
    CREDO_VERUM(c->unitas == NIHIL);
    CREDO_AEQUALIS_I32(c->color_fundi, 0xFF0000);
    c = &cellulae[X + II];                                    /* (2,2) */
    CREDO_VERUM(c->unitas == NIHIL);
    CREDO_AEQUALIS_I32(c->color_fundi, CAERULEUS);
    c = &cellulae[I];                                         /* (1,0) */
    CREDO_VERUM(c->unitas != NIHIL && c->mensura == III
        && memcmp(c->unitas, "\xE2\x96\x90", III) == ZEPHYRUM);  /* ▐ */
    CREDO_AEQUALIS_I32(c->color_litterae, NIGER);
    CREDO_AEQUALIS_I32(c->color_fundi, 0xFF0000);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL
        && cellulae[ZEPHYRUM].color_fundi == NIGER);
    CREDO_VERUM(cellulae[V + III].unitas == NIHIL
        && cellulae[V + III].color_fundi == NIGER);
    /* E: linea axialis sola - via pixelorum eam NON pingit (filtrum) */
    m = mandata_creare(piscina, intern);
    mandata_imago(m, chorda_ex_literis("dimidium", piscina),
        _fines(ZEPHYRUM, XVI, III, VIII));
    mandata_linea(m, _punctum(ZEPHYRUM, IV), _punctum(XXIX, IV), I,
        _rgba(FLAVUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        _fons, &imagines, pixela, cellulae);
    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_VERUM(cellulae[k].juncturae != ZEPHYRUM
            && cellulae[k].color_fundi == NIGER);
    }
    /* F: linea obliqua -> glyphi in diagonali solum */
    m = mandata_creare(piscina, intern);
    mandata_linea(m, _punctum(ZEPHYRUM, ZEPHYRUM), _punctum(XXIX,
        XXIII),
        I, _rgba(FLAVUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        _fons, &imagines, pixela, cellulae);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas != NIHIL);
    CREDO_VERUM(cellulae[V + II].unitas != NIHIL);
    CREDO_VERUM(cellulae[X + IV].unitas != NIHIL);
    CREDO_VERUM(cellulae[IV].unitas == NIHIL
        && cellulae[IV].color_fundi == NIGER);
    CREDO_VERUM(cellulae[X].unitas == NIHIL
        && cellulae[X].color_fundi == NIGER);
    /* G: triangulum impletum */
    m                     = mandata_creare(piscina, intern);
    triangulum[ZEPHYRUM]  = _punctum(ZEPHYRUM, ZEPHYRUM);
    triangulum[I]         = _punctum(XXIX, ZEPHYRUM);
    triangulum[II]        = _punctum(ZEPHYRUM, XXIII);
    mandata_polygonum(m, triangulum, III, _rgba(CAERULEUS), VERUM);
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        _fons, &imagines, pixela, cellulae);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL
        && cellulae[ZEPHYRUM].color_fundi == CAERULEUS);
    CREDO_VERUM(cellulae[X + IV].unitas == NIHIL
        && cellulae[X + IV].color_fundi == NIGER);
    /* H: sine piscina via pixelorum omittitur (T3) */
    m = mandata_creare(piscina, intern);
    mandata_imago(m, chorda_ex_literis("rubrum", piscina),
        _fines(VI, VIII, XII, XVI));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        _fons, &imagines, NIHIL, cellulae);
    CREDO_AEQUALIS_I32(cellulae[V + I].color_fundi, NIGER);

    imprimere("\n--- VI. textus: margo PROXIMUS (T5b, D5 emendata) ---\n");
    modulus  = modulus_creare(VI, VIII, XXXVI, XVI);       /* 6 x 2 */
    m        = mandata_creare(piscina, intern);
    /* (4,6): pavimentum (0,0), proximum (1,1) - cellula maxime tecta */
    mandata_textus(m, IV, VI, chorda_ex_literis("t", piscina), ZEPHYRUM,
        _rgba(ALBUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL);
    CREDO_VERUM(cellulae[VI + I].unitas != NIHIL
        && cellulae[VI + I].unitas[0] == 't');
    /* pictor in parvo: fascia y 4..16 -> linea 1; titulus y 6 eadem */
    m = mandata_creare(piscina, intern);
    mandata_rectangulum(m, _fines(ZEPHYRUM, IV, XXXVI, XII),
        _rgba(CAERULEUS), VERUM);
    mandata_textus(m, II, VI, chorda_ex_literis("ab", piscina),
        ZEPHYRUM,
        _rgba(ALBUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    CREDO_VERUM(cellulae[VI].unitas != NIHIL
        && cellulae[VI].unitas[0] == 'a');
    CREDO_AEQUALIS_I32(cellulae[VI].color_fundi, CAERULEUS);
    CREDO_VERUM(cellulae[ZEPHYRUM].unitas == NIHIL);
    /* lineae PAVIMENTUM servant: y 7 in linea 0 */
    m = mandata_creare(piscina, intern);
    mandata_linea(m, _punctum(ZEPHYRUM, VII), _punctum(XXXV, VII), I,
        _rgba(FLAVUS));
    tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
        NIGER,
        NIHIL, NIHIL, NIHIL, cellulae);
    CREDO_VERUM(cellulae[ZEPHYRUM].juncturae != ZEPHYRUM);
    CREDO_VERUM(cellulae[VI].juncturae == ZEPHYRUM);

    imprimere("\n--- VII. scaena pictor.arbor (exemplar, praedictum) ---\n");
    {
                 chorda  fons;
              character* cstr;
              Componens* radix;
        FiguraRegistrum* reg;
                    i32  p;
                    s32  l;
     TessellatioCellula* grandes;
     constans character* exspectata[XXX];
              character  medium[CXXVIII];
              character  summum[CCLVI];        /* ─ = III octeti */

        fons =
            filum_legere_totum("probationes/pictor/pictor.arbor.stml",
            piscina);
        CREDO_VERUM(fons.mensura > ZEPHYRUM);
        cstr = (character*)piscina_allocare(piscina,
            (memoriae_index)fons.mensura + I);
        memcpy(cstr, fons.datum, (memoriae_index)fons.mensura);
        cstr[fons.mensura] = '\0';
        radix = componens_legere_stml(cstr, piscina, intern);
        reg = figura_registrum_creare(piscina);
        CREDO_NON_NIHIL(radix);
        per (p = ZEPHYRUM; p < (i32)PARTES_NUMERUS; p++)
        {
            figura_registrare(reg, (Partes)p, ZEPHYRUM,
                p
                    == (i32)PARTES_TITULUS ? _figura_tituli : figura_finium,
                NIHIL);
        }
        m = mandata_creare(piscina, intern);
        pingere(radix, reg, ZEPHYRUM, m);
        modulus  = modulus_creare(VI, VIII, CCCLX, CCXL);   /* 60 x 30 */
        grandes  =
            (TessellatioCellula*)piscina_allocare_ordinatum(piscina,
            (memoriae_index)(LX * XXX) * magnitudo(TessellatioCellula),
            VIII);
        tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
            NIGER, NIHIL, NIHIL, NIHIL, grandes);
        /* summum: ┬ (margo prospectus, translatio -5, cum radice) +
         * ─ x LI + ┬ + VII spatia; medium: │ + LI + │ + VII; linea 29:
         * titulus SUPER fasciam suam (fascia margines delet) */
        strcpy(summum, "\xE2\x94\xAC");
        per (l = ZEPHYRUM; l < LI; l++)
        {
            strcat(summum, "\xE2\x94\x80");
        }
        strcat(summum, "\xE2\x94\xAC       ");
        strcpy(medium, "\xE2\x94\x82");
        per (l = ZEPHYRUM; l < LI; l++)
        {
            strcat(medium, " ");
        }
        strcat(medium, "\xE2\x94\x82       ");
        exspectata[ZEPHYRUM] = summum;
        per (l = I; l < XXIX; l++)
        {
            exspectata[l] = medium;
        }
        exspectata[XXIX] =
            "penicillus                                        "
            "          ";
        CREDO_VERUM(_scaena_congruit(grandes, LX, XXX, exspectata));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
