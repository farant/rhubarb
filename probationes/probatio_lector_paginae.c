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
    CREDO_AEQUALIS_I32(numerus_linearum(x, ZEPHYRUM), X);

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

hic_manens constans CredoSectio SECTIONES[] = {
    { "paginae", _sectio_paginae, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector_paginae", SECTIONES);
}
