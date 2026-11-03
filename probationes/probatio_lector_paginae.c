/* probatio_lector_paginae.c - caput in paginas (lector L3 T1): fontes
 * (titulus 8x16, cetera 6x8), intervallum inter clausulas, latitudo
 * per runas, versus indentatus, verbum longum sectum, area minima,
 * caput vacuum, determinatio, pagina positionis. Capita in probatione
 * ficta (sine EPUB). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "fons.h"
#include "codex.h"
#include "lector_paginae.h"
#include "credo.h"
#include <string.h>

/* caput fictum: clausulae (genus, textus) "\n\n" iunctae */
interior CodexCaput*
caput_facere (
                       Piscina* p,
          constans CodexGenus* genera,
    constans character* constans* textus,
                           i32  n)
{
            CodexCaput* k;
         CodexClausula* b;
     ChordaAedificator* a;
                   i32  i;

    k = (CodexCaput*)piscina_allocare(p, magnitudo(CodexCaput));
    a = chorda_aedificator_creare(p, MXXIV);
    k->titulus = chorda_ex_literis("", p);
    k->clausulae = xar_creare(p, (i32)magnitudo(CodexClausula));
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, "\n\n");
        }
        b           = (CodexClausula*)xar_addere(k->clausulae);
        b->genus    = genera[i];
        b->initium  = (i32)chorda_aedificator_longitudo(a);
        chorda_aedificator_appendere_literis(a, textus[i]);
        b->finis = (i32)chorda_aedificator_longitudo(a);
    }
    k->textus = chorda_aedificator_finire(a);
    redde k;
}

interior constans LectorLinea*
linea (
     Xar* paginae,
     i32  k,
     i32  j)
{
    constans LectorPagina* g;

    g = (constans LectorPagina*)xar_obtinere(paginae, k);
    redde (constans LectorLinea*)xar_obtinere(g->lineae, j);
}

interior i32
numerus_linearum (
     Xar* paginae,
     i32  k)
{
    redde xar_numerus(((constans LectorPagina*)xar_obtinere(paginae,
        k))->lineae);
}

interior chorda
textus_lineae (
     constans CodexCaput* k,
    constans LectorLinea* l)
{
    redde chorda_sectio(k->textus, l->initium, l->finis);
}

/* runae (codepoints) in [a, b) textus */
interior i32
runae (
    chorda c,
       i32 a,
       i32 b)
{
    i32 n;
    i32 i;

    n = ZEPHYRUM;
    per (i = a; i < b; i++)
    {
        si ((c.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

/* lineae omnes ordine spatio iunctae == expectatum, omnes intra
 * latitudinem, initia crescentia */
interior b32
lineae_tegunt (
     constans CodexCaput* k,
                     Xar* paginae,
                     i32  latitudo,
      constans character* expectatum,
                 Piscina* p)
{
        ChordaAedificator* a;
     constans LectorLinea* l;
                      i32  g;
                      i32  j;
                      s32  prius;

    a      = chorda_aedificator_creare(p, MXXIV);
    prius  = -I;
    per (g = ZEPHYRUM; g < xar_numerus(paginae); g++)
    {
        per (j = ZEPHYRUM; j < numerus_linearum(paginae, g); j++)
        {
            l = linea(paginae, g, j);
            si (   (s32)l->initium <= prius
                || runae(k->textus, l->initium, l->finis)
                   * (i32)fons_latitudo(l->fons) + (i32)l->x > latitudo)
            {
                redde FALSUM;
            }
            prius = (s32)l->initium;
            si (chorda_aedificator_longitudo(a) > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            chorda_aedificator_appendere_chorda(a, textus_lineae(k, l));
        }
    }
    redde chorda_aequalis_literis(chorda_aedificator_finire(a),
        expectatum);
}

/* paginae eaedem (numerus, lineae campis omnibus)? */
interior b32
paginae_aequales (
    Xar* a,
    Xar* b)
{
    i32 g;
    i32 j;

    si (xar_numerus(a) != xar_numerus(b))
    {
        redde FALSUM;
    }
    per (g = ZEPHYRUM; g < xar_numerus(a); g++)
    {
        si (numerus_linearum(a, g) != numerus_linearum(b, g))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < numerus_linearum(a, g); j++)
        {
            si (memcmp(linea(a, g, j), linea(b, g, j),
                    magnitudo(LectorLinea)) != ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_paginae (
    CredoContextus* c)
{
                  Piscina* p;
               CodexCaput* k;
                      Xar* x;
                      Xar* y;
     constans LectorLinea* l;
        ChordaAedificator* a;
                   chorda  verba;
                      i32  i;
                      i32  g;
                      i32  lineae;
      constans CodexGenus  tp[] = { CODEX_TITULUS, CODEX_PARAGRAPHUS };
      constans CodexGenus  pp[] = { CODEX_PARAGRAPHUS };
      constans CodexGenus  vv[] = { CODEX_VERSUS };
    constans character* constans duo[] = { "Caput I\nDe Cranfordia",
        "In primis Cranfordia ab Amazonibus tenetur." };
    constans character* constans versus[] = {
        "line one long enough to wrap\nline two" };
    constans character* constans lexicon[] = { "in", "primis",
        "Cranfordia", "ab", "Amazonibus", "tenetur", "et", "regitur" };
    constans CodexGenus tres_genera[] = { CODEX_PARAGRAPHUS,
        CODEX_PARAGRAPHUS, CODEX_PARAGRAPHUS };
    constans character* constans tres[]     = { "unus", "duo", "tres" };
    constans character* constans geminum[]  = { "alpha  beta" };
    constans character* unus[I];
             character  verbum[CCI];
             character  curvi[CXXIV];

    p = c->piscina;

    CREDO_GRADUS("titulus 8x16 (y 0, XVI), paragraphus 6x8 post "
        "intervallum (y XL)");
    k = caput_facere(p, tp, duo, II);
    x = lector_paginare(k, CCXL, CC, p);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), IV);
    l = linea(x, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32(l->fons, FONS_TITULI);
    CREDO_AEQUALIS_S32(l->y, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, l), "Caput I");
    l = linea(x, ZEPHYRUM, I);
    CREDO_AEQUALIS_I32(l->fons, FONS_TITULI);
    CREDO_AEQUALIS_S32(l->y, XVI);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, l), "De Cranfordia");
    l = linea(x, ZEPHYRUM, II);
    CREDO_AEQUALIS_I32(l->fons, FONS_PARVUS);
    CREDO_AEQUALIS_S32(l->y, XL);
    CREDO_VERUM(lineae_tegunt(k, x, CCXL,
        "Caput I De Cranfordia In primis Cranfordia ab Amazonibus "
        "tenetur.", p));

    CREDO_GRADUS("paragraphus CD verborum: paginae plures, textus "
        "totus ordine, latitudo servata, pagina II sine intervallo");
    a = chorda_aedificator_creare(p, IV * MXXIV);
    per (i = ZEPHYRUM; i < CD; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_literis(a, lexicon[i % VIII]);
    }
    verba           = chorda_aedificator_finire(a);
    unus[ZEPHYRUM]  = chorda_ut_cstr(verba, p);
    k               = caput_facere(p, pp, unus, I);
    x               = lector_paginare(k, CCXL, LXXX, p);
    CREDO_VERUM(xar_numerus(x) > I);
    CREDO_VERUM(lineae_tegunt(k, x, CCXL, unus[ZEPHYRUM], p));
    CREDO_AEQUALIS_S32(linea(x, I, ZEPHYRUM)->y, ZEPHYRUM);

    CREDO_GRADUS("spatium duplex: lineae textus XVI px distant; LXXX "
        "px "
        "= V lineae (0..LXIV)");
    CREDO_AEQUALIS_S32(linea(x, ZEPHYRUM, I)->y, XVI);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), V);

    CREDO_GRADUS("linea vacua post ultimam non requiritur: LXXII px = "
        "V "
        "lineae");
    y = lector_paginare(k, CCXL, LXXII, p);
    CREDO_AEQUALIS_I32(numerus_linearum(y, ZEPHYRUM), V);

    CREDO_GRADUS("determinatio: eadem area eaedem paginae, alia aliae");
    y = lector_paginare(k, CCXL, LXXX, p);
    CREDO_VERUM(paginae_aequales(x, y));
    y = lector_paginare(k, CDLXXX, LXXX, p);
    CREDO_VERUM(xar_numerus(y) < xar_numerus(x));

    CREDO_GRADUS("pagina positionis");
    CREDO_AEQUALIS_I32(lector_pagina_positionis(x, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(lector_pagina_positionis(x, ((LectorPagina*)
        xar_obtinere(x, I))->initium), I);
    CREDO_AEQUALIS_I32(lector_pagina_positionis(x, ((LectorPagina*)
        xar_obtinere(x, I))->initium - I), ZEPHYRUM);
    CREDO_AEQUALIS_I32(lector_pagina_positionis(x, k->textus.mensura
        + C), xar_numerus(x) - I);

    CREDO_GRADUS("latitudo per runas: XL uncini curvi (III octeti) in "
        "linea una CCXL px, XLI in duabus");
    per (i = ZEPHYRUM; i < XL; i++)
    {
        memcpy(curvi + i * III, "\xe2\x80\x9c", III);
    }
    curvi[XL * III]  = '\0';
    unus[ZEPHYRUM]   = curvi;
    k                = caput_facere(p, pp, unus, I);
    x                = lector_paginare(k, CCXL, LXXX, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), I);
    CREDO_AEQUALIS_I32(linea(x, ZEPHYRUM, ZEPHYRUM)->finis, CXX);
    memcpy(curvi + XL * III, "\xe2\x80\x9c", III);
    curvi[XLI * III]  = '\0';
    k                 = caput_facere(p, pp, unus, I);
    x                 = lector_paginare(k, CCXL, LXXX, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), II);

    CREDO_GRADUS("versus: continuatio II cellulis indentata");
    k = caput_facere(p, vv, versus, I);
    x = lector_paginare(k, LX, CC, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), V);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        ZEPHYRUM)), "line one");
    CREDO_AEQUALIS_S32(linea(x, ZEPHYRUM, ZEPHYRUM)->x, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        I)), "long");
    CREDO_AEQUALIS_S32(linea(x, ZEPHYRUM, I)->x, XII);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        II)), "enough");
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        III)), "to wrap");
    CREDO_AEQUALIS_S32(linea(x, ZEPHYRUM, III)->x, XII);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        IV)), "line two");
    CREDO_AEQUALIS_S32(linea(x, ZEPHYRUM, IV)->x, ZEPHYRUM);

    CREDO_GRADUS("verbum C runarum (e acutum) in LX px: X lineae X "
        "runarum, numquam intra runam");
    per (i = ZEPHYRUM; i < C; i++)
    {
        memcpy(verbum + i * II, "\xc3\xa9", II);
    }
    verbum[CC]      = '\0';
    unus[ZEPHYRUM]  = verbum;
    k               = caput_facere(p, pp, unus, I);
    x               = lector_paginare(k, LX, CC, p);
    lineae          = ZEPHYRUM;
    per (g = ZEPHYRUM; g < xar_numerus(x); g++)
    {
        per (i = ZEPHYRUM; i < numerus_linearum(x, g); i++)
        {
            l = linea(x, g, i);
            CREDO_AEQUALIS_I32(l->initium, lineae * XX);
            CREDO_AEQUALIS_I32(l->finis - l->initium, XX);
            lineae++;
        }
    }
    CREDO_AEQUALIS_I32(lineae, X);

    CREDO_GRADUS("area minor linea: pagina quaeque lineam unam, finis");
    k = caput_facere(p, tp, duo, II);
    CREDO_NON_PENDET(lector_paginare(k, LX, IV, p), MM);
    x = lector_paginare(k, LX, IV, p);
    per (g = ZEPHYRUM; g < xar_numerus(x); g++)
    {
        CREDO_AEQUALIS_I32(numerus_linearum(x, g), I);
    }
    CREDO_VERUM(xar_numerus(x) > V);

    CREDO_GRADUS("clausula in pagina nova: sine intervallo (y 0)");
    k = caput_facere(p, tres_genera, tres, III);
    x = lector_paginare(k, CCXL, XVI, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), III);
    per (g = ZEPHYRUM; g < III; g++)
    {
        CREDO_AEQUALIS_S32(linea(x, g, ZEPHYRUM)->y, ZEPHYRUM);
    }

    CREDO_GRADUS("spatia finalia dempta (spatium duplex ad finem)");
    k = caput_facere(p, pp, geminum, I);
    x = lector_paginare(k, XXXVI, LXXX, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        ZEPHYRUM)), "alpha");
    CREDO_CHORDA_AEQUALIS_LITERIS(textus_lineae(k, linea(x, ZEPHYRUM,
        I)), "beta");

    CREDO_GRADUS("caput vacuum: pagina una sine lineis");
    k = caput_facere(p, pp, unus, ZEPHYRUM);
    x = lector_paginare(k, CCXL, LXXX, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(lector_pagina_positionis(x, X), ZEPHYRUM);
}

interior CodexSententia
sententia_facta (
    i32 initium,
    i32 finis)
{
    CodexSententia t;

    t.initium   = initium;
    t.finis     = finis;
    t.clausula  = ZEPHYRUM;
    redde t;
}

/* lineae omnes intra aream (x, y, latitudo per runas) */
interior b32
intra_aream (
    constans CodexCaput* k,
                    Xar* lineae,
                    i32  latitudo,
                    i32  altitudo)
{
     constans LectorLinea* l;
                      i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        l = (constans LectorLinea*)xar_obtinere(lineae, i);
        si (   l->x < ZEPHYRUM || l->y < ZEPHYRUM
            || l->x + (s32)(runae(k->textus, l->initium, l->finis)
               * (i32)fons_latitudo(l->fons)) > (s32)latitudo
            || l->y + fons_altitudo(l->fons) > (s32)altitudo)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_sententia (
    CredoContextus* c)
{
                  Piscina* p;
               CodexCaput* k;
                      Xar* x;
     constans LectorLinea* l;
           CodexSententia  t;
                      b32  praecisa;
                      i32  i;
      constans CodexGenus  pp[] = { CODEX_PARAGRAPHUS };
      constans CodexGenus  tp[] = { CODEX_TITULUS };
    constans character* constans parva[] = {
        "Primum hic. Secundum ibi. Tertium!" };
    constans character* constans titulus[] = {
        "Caput I\nDe Cranfordia" };
    constans character* unus[I];
             character  longum[CDI];
             character  curvi[XCI];

    p = c->piscina;

    CREDO_GRADUS("brevis: FONS_TITULI, linea una, centrata");
    k = caput_facere(p, pp, parva, I);
    t = sententia_facta(ZEPHYRUM, XI);
    x = lector_sententiam_disponere(k, &t, CCXL, CC, &praecisa, p);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_FALSUM(praecisa);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    l = (constans LectorLinea*)xar_obtinere(x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(l->fons, FONS_TITULI);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_sectio(k->textus, l->initium,
        l->finis), "Primum hic.");
    CREDO_AEQUALIS_S32(l->x, (CCXL - XI * VIII) / II);
    CREDO_AEQUALIS_S32(l->y, (CC - XVI) / II);

    CREDO_GRADUS("titulus cum '\\n': lineae II, XXIV px distant");
    k = caput_facere(p, tp, titulus, I);
    t = sententia_facta(ZEPHYRUM, k->textus.mensura);
    x = lector_sententiam_disponere(k, &t, CCXL, CC, &praecisa, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_sectio(k->textus,
        ((LectorLinea*)xar_obtinere(x, I))->initium, ((LectorLinea*)
        xar_obtinere(x, I))->finis), "De Cranfordia");
    CREDO_AEQUALIS_S32(((LectorLinea*)xar_obtinere(x, I))->y
        - ((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->y, XXIV);

    CREDO_GRADUS("lineae plures: grex centratus, lineae ad sinistrum "
        "(x idem; linea longissima XIII x VIII px)");
    CREDO_AEQUALIS_S32(((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->x,
        (CCXL - XIII * VIII) / II);
    CREDO_AEQUALIS_S32(((LectorLinea*)xar_obtinere(x, I))->x,
        (CCXL - XIII * VIII) / II);

    CREDO_GRADUS("involvere: fons datus (6x8 etsi 8x16 caperet), x et "
        "y nulli, '\\n' linea nova");
    x = lector_sententiam_involvere(k, &t, FONS_PARVUS, CCXL, p);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_I32(xar_numerus(x), II);
    l = (constans LectorLinea*)xar_obtinere(x, I);
    CREDO_AEQUALIS_I32(l->fons, FONS_PARVUS);
    CREDO_AEQUALIS_S32(l->x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(l->y, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_sectio(k->textus, l->initium,
        l->finis), "De Cranfordia");
    CREDO_GRADUS("involvere angusta: XLVIII px in 8x16 = VI cellulae, "
        "'Caput I' in duas");
    x = lector_sententiam_involvere(k, &t, FONS_TITULI, XLVIII, p);
    CREDO_AEQUALIS_I32(((constans LectorLinea*)xar_obtinere(x,
        ZEPHYRUM))->fons, FONS_TITULI);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_sectio(k->textus,
        ((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->initium,
        ((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->finis), "Caput");

    CREDO_GRADUS("longior pro 8x16 in LX px: FONS_PARVUS, lineae XVI "
        "px, "
        "grex centratus");
    per (i = ZEPHYRUM; i < LXXX; i++)
    {
        longum[i] = (i % VI == V) ? ' ' : 'a';
    }
    longum[LXXX] = '\0';
    unus[ZEPHYRUM] = longum;
    k = caput_facere(p, pp, unus, I);
    t = sententia_facta(ZEPHYRUM, LXXX);
    x = lector_sententiam_disponere(k, &t, CCXL, LX, &praecisa, p);
    CREDO_FALSUM(praecisa);
    /* 8x16: XXX cellulae, III lineae = LXIV px > LX; 6x8: verba V
     * litterarum, VI per lineam XL cellularum -> III lineae, XL px */
    CREDO_AEQUALIS_I32(xar_numerus(x), III);
    l = (constans LectorLinea*)xar_obtinere(x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(l->fons, FONS_PARVUS);
    CREDO_AEQUALIS_S32(((LectorLinea*)xar_obtinere(x, I))->y - l->y,
        XVI);
    CREDO_AEQUALIS_S32(l->y, (LX - (II * XVI + VIII)) / II);
    CREDO_VERUM(intra_aream(k, x, CCXL, LX));

    CREDO_GRADUS("nimis alta etiam 6x8: praecisa, lineae quae capiunt");
    per (i = ZEPHYRUM; i < CD; i++)
    {
        longum[i] = (i % VI == V) ? ' ' : 'a';
    }
    longum[CD] = '\0';
    unus[ZEPHYRUM] = longum;
    k = caput_facere(p, pp, unus, I);
    t = sententia_facta(ZEPHYRUM, CD);
    x = lector_sententiam_disponere(k, &t, CCXL, XL, &praecisa, p);
    CREDO_VERUM(praecisa);
    CREDO_AEQUALIS_I32(xar_numerus(x), III);
    CREDO_VERUM(intra_aream(k, x, CCXL, XL));

    CREDO_GRADUS("runae numerantur: XXX uncini curvi in linea una "
        "8x16");
    per (i = ZEPHYRUM; i < XXX; i++)
    {
        memcpy(curvi + i * III, "\xe2\x80\x9c", III);
    }
    curvi[XC] = '\0';
    unus[ZEPHYRUM] = curvi;
    k = caput_facere(p, pp, unus, I);
    t = sententia_facta(ZEPHYRUM, XC);
    x = lector_sententiam_disponere(k, &t, CCXL, CC, &praecisa, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_AEQUALIS_I32(((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->fons,
        FONS_TITULI);
    CREDO_AEQUALIS_S32(((LectorLinea*)xar_obtinere(x, ZEPHYRUM))->x,
        ZEPHYRUM);
}

/* N3.4d (Franus 2026-10-27): fractiones paginarum. Linea textus VIII
 * px, passus XVI; titulus XVI px, passus XVI; inter clausulas VIII.
 * Verbum XXXIX litterarum in CCXL px = linea una (XL cellulae). */

/* paragraphus n linearum: verba XXXIX litterarum, quodque linea sua;
 * littera prima = signum (a, b, ...), deinde index */
interior constans character*
paragraphus_linearum (
      Piscina* p,
    character  signum,
          i32  n)
{
    ChordaAedificator* a;
                  i32  i;
                  i32  j;

    a = chorda_aedificator_creare(p, MXXIV);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_character(a, signum);
        chorda_aedificator_appendere_character(a, (character)('0'
            + (character)(i % X)));
        per (j = II; j < XXXIX; j++)
        {
            chorda_aedificator_appendere_character(a, 'x');
        }
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(a), p);
}

/* linea j paginae g incipit signo et indice? ("b0", "T") */
interior b32
incipit (
    constans CodexCaput* k,
                    Xar* paginae,
                    i32  g,
                    i32  j,
     constans character* praefixum,
                Piscina* p)
{
    si (   g >= xar_numerus(paginae)
        || j >= numerus_linearum(paginae, g))
    {
        redde FALSUM;
    }
    redde chorda_incipit(textus_lineae(k, linea(paginae, g, j)),
        chorda_ex_literis(praefixum, p));
}

interior vacuum
_sectio_fractiones (
    CredoContextus* c)
{
                 Piscina* p;
              CodexCaput* k;
                     Xar* x;
     constans CodexGenus  pp[] = { CODEX_PARAGRAPHUS,
        CODEX_PARAGRAPHUS };
     constans CodexGenus p1[] = { CODEX_PARAGRAPHUS };
     constans CodexGenus cum_titulo[] = { CODEX_PARAGRAPHUS,
         CODEX_TITULUS,
        CODEX_PARAGRAPHUS };
     constans CodexGenus cum_titulis[] = { CODEX_PARAGRAPHUS,
         CODEX_TITULUS,
        CODEX_TITULUS, CODEX_PARAGRAPHUS };
     constans CodexGenus ad_titulum[] = { CODEX_PARAGRAPHUS,
         CODEX_TITULUS };
     constans CodexGenus  vv[] = { CODEX_VERSUS };
      constans character* textus[IV];

    p = c->piscina;

    CREDO_GRADUS("prima linea sola in imo non manet: paragraphus "
        "totus in paginam novam (LXXX px: IV + I capiunt)");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', IV);
    textus[I]         = paragraphus_linearum(p, 'b', VI);
    k                 = caput_facere(p, pp, textus, II);
    x                 = lector_paginare(k, CCXL, LXXX, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), IV);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "b0", p));

    CREDO_GRADUS("ultima linea sola in summo non manet: duae transeunt "
        "(VI in LXXII px: IV + II, non V + I)");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', VI);
    k                 = caput_facere(p, p1, textus, I);
    x                 = lector_paginare(k, CCXL, LXXII, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), II);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), IV);
    CREDO_AEQUALIS_I32(numerus_linearum(x, I), II);

    CREDO_GRADUS("paginae plures: XI lineae in LXXII px = V, IV, II");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', XI);
    k                 = caput_facere(p, p1, textus, I);
    x                 = lector_paginare(k, CCXL, LXXII, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), III);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), V);
    CREDO_AEQUALIS_I32(numerus_linearum(x, I), IV);
    CREDO_AEQUALIS_I32(numerus_linearum(x, II), II);

    CREDO_GRADUS("paragraphus III linearum non finditur (II capiunt: "
        "totus transit)");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', III);
    textus[I]         = paragraphus_linearum(p, 'b', III);
    k                 = caput_facere(p, pp, textus, II);
    x                 = lector_paginare(k, CCXL, LXXX, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), III);
    CREDO_AEQUALIS_I32(numerus_linearum(x, I), III);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "b0", p));

    CREDO_GRADUS("titulus cum sequente: titulus capit, linea una "
        "sequentis tantum - titulus transit");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', III);
    textus[I]         = "Titulus";
    textus[II]        = paragraphus_linearum(p, 'b', V);
    k                 = caput_facere(p, cum_titulo, textus, III);
    x                 = lector_paginare(k, CCXL, LXXXVIII, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), III);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "Titulus", p));
    CREDO_VERUM(incipit(k, x, I, I, "b0", p));

    CREDO_GRADUS("titulus cum paragrapho III linearum (non finditur): "
        "II capiunt, III non - titulus transit");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', II);
    textus[I]         = "Titulus";
    textus[II]        = paragraphus_linearum(p, 'b', III);
    k                 = caput_facere(p, cum_titulo, textus, III);
    x                 = lector_paginare(k, CCXL, XCVI, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), II);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "Titulus", p));
    CREDO_AEQUALIS_I32(numerus_linearum(x, I), IV);

    CREDO_GRADUS("tituli duo in imo: ambo transeunt (non secundus "
        "solus)");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', II);
    textus[I]         = "Pars";
    textus[II]        = "Caput";
    textus[III]       = paragraphus_linearum(p, 'b', V);
    k                 = caput_facere(p, cum_titulis, textus, IV);
    x                 = lector_paginare(k, CCXL, XCVI, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), II);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "Pars", p));
    CREDO_VERUM(incipit(k, x, I, I, "Caput", p));
    CREDO_VERUM(incipit(k, x, I, II, "b0", p));

    CREDO_GRADUS("titulus duarum linearum non finditur");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', IV);
    textus[I]         = "Caput I\nDe Cranfordia";
    k                 = caput_facere(p, ad_titulum, textus, II);
    x                 = lector_paginare(k, CCXL, XCVI, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), IV);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "Caput I", p));
    CREDO_VERUM(incipit(k, x, I, I, "De Cranfordia", p));

    CREDO_GRADUS("grex nusquam capit (XLVIII px): titulus saltem "
        "integer in paginam novam");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', I);
    textus[I]         = "Caput I\nDe Cranfordia";
    textus[II]        = paragraphus_linearum(p, 'b', IV);
    k                 = caput_facere(p, cum_titulo, textus, III);
    x                 = lector_paginare(k, CCXL, XLVIII, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), I);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "Caput I", p));
    CREDO_VERUM(incipit(k, x, I, I, "De Cranfordia", p));

    CREDO_GRADUS("versus: eaedem leges (VI in LXXII px: IV + II)");
    textus[ZEPHYRUM]  = "v0\nv1\nv2\nv3\nv4\nv5";
    k                 = caput_facere(p, vv, textus, I);
    x                 = lector_paginare(k, CCXL, LXXII, p);
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), IV);
    CREDO_VERUM(incipit(k, x, I, ZEPHYRUM, "v4", p));

    CREDO_GRADUS("textus totus ordine in omnibus");
    textus[ZEPHYRUM]  = paragraphus_linearum(p, 'a', XI);
    k                 = caput_facere(p, p1, textus, I);
    x                 = lector_paginare(k, CCXL, LXXII, p);
    CREDO_VERUM(lineae_tegunt(k, x, CCXL, textus[ZEPHYRUM], p));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "paginae", _sectio_paginae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sententia", _sectio_sententia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fractiones", _sectio_fractiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector_paginae", SECTIONES);
}
