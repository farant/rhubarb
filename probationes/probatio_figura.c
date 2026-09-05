/* probatio_figura.c - registrum figurarum, pingere (gradus VI) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "thema.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "credo.h"
#include <stdio.h>

/* Figura probationis: rectangulum plenum coloris thematis + titulus */
interior vacuum
figura_bottonis_probationis (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
           Fines f;
    ColorMandati color;

    (vacuum)thema;
    (*(i32*)ctx)++;
    f.x          = ZEPHYRUM;
    f.y          = ZEPHYRUM;
    f.latitudo   = c->fines.latitudo;
    f.altitudo   = c->fines.altitudo;
    color.genus  = COLOR_MANDATI_THEMA;
    color.valor  = (i32)COLOR_ACCENT_PRIMARY;
    mandata_rectangulum(m, f, color, VERUM);
    color.valor = (i32)COLOR_TEXT;
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM, c->titulus, ZEPHYRUM, color);
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
     Componens* c;
         Fines  f;

    c           = componens_creare(p, in, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    componens_ponere_fines(c, f);
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
        FiguraRegistrum* reg;
              Componens* radix;
              Componens* pannus;
              Componens* b1;
                Mandata* m;
                Mandata* m2;
               Mandatum* x;
               FiguraFn  fn;
                 vacuum* ctx;
                    i32  vocationes;

    piscina = piscina_generare_dynamicum("probatio_figura", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern      = internamentum_creare(piscina);
    vocationes  = ZEPHYRUM;

    imprimere("\n--- Registrum ---\n");
    reg = figura_registrum_creare(piscina);
    CREDO_NON_NIHIL(reg);
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                  figura_bottonis_probationis,
                                  &vocationes));
    /* duplex (partes, thema) recusatur */
    CREDO_FALSUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                   figura_bottonis_probationis,
                                   &vocationes));
    /* idem partes, thema aliud: licet */
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, I, figura_finium,
                                  NIHIL));
        CREDO_FALSUM(figura_registrare(reg, PARTES_CAMPUS, ZEPHYRUM,
            NIHIL,
                                   NIHIL));
    /* pannus (dialogus) figuram ET liberos habet: ordo z probatur */
    CREDO_VERUM(figura_registrare(reg, PARTES_DIALOGUS, ZEPHYRUM,
                                  figura_finium, NIHIL));
    fn   = NIHIL;
    ctx  = NIHIL;
    CREDO_VERUM(figura_invenire(reg, PARTES_BOTTONE, ZEPHYRUM, &fn,
        &ctx));
    CREDO_VERUM(fn == figura_bottonis_probationis);
    CREDO_VERUM(ctx == (vacuum*)&vocationes);
    CREDO_FALSUM(figura_invenire(reg, PARTES_TABULA, ZEPHYRUM, &fn,
        &ctx));

    /* radix (0,0,200,100)
     *   pannus (70,0,130,100) translatio (10,50) scala II sectio
     *     b1 (5,5,40,20) bottone "Ok" */
    radix  = nodus(piscina, intern, "radix", PARTES_NULLUM,
                   ZEPHYRUM, ZEPHYRUM, CC, C);
        pannus = nodus(piscina, intern, "pannus", PARTES_DIALOGUS,
                   LXX, ZEPHYRUM, CXXX, C);
    componens_ponere_transformatio(pannus, X, L, II);
    componens_ponere_sectio(pannus, VERUM);
    b1 = nodus(piscina, intern, "b1", PARTES_BOTTONE, V, V, XL, XX);
    componens_ponere_titulum(b1, "Ok");
    componens_addere_liberum(radix, pannus);
    componens_addere_liberum(pannus, b1);

    imprimere("\n--- Pingere: coetus per componens, figura ante liberos"
              " ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    CREDO_AEQUALIS_I32(vocationes, I);
        /* coetus radix, coetus pannus, RECT panni (figura ANTE liberos),
     * coetus b1, rect b1, textus = VI */
    CREDO_AEQUALIS_I32(mandata_numerus(m), VI);
    x = mandata_obtinere(m, ZEPHYRUM);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "radix");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, VI);
    x = mandata_obtinere(m, I);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "pannus");
    CREDO_AEQUALIS_S32(x->fines.x, LXX);
    CREDO_AEQUALIS_S32(x->translatio.x, X);
    CREDO_AEQUALIS_S32(x->translatio.y, L);
    CREDO_AEQUALIS_I32(x->scala, II);
        CREDO_VERUM(x->sectio);
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, V);
    x = mandata_obtinere(m, II);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);   /* fines panni */
    CREDO_FALSUM(x->impletum);
    CREDO_AEQUALIS_S32(x->fines.latitudo, CXXX);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "b1");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, III);
    x = mandata_obtinere(m, IV);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_VERUM(x->impletum);
    /* in spatio PROPRIO: origo (0,0), non (5,5) */
    CREDO_AEQUALIS_S32(x->fines.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(x->fines.latitudo, XL);
    CREDO_VERUM(x->color.genus == COLOR_MANDATI_THEMA);
        x = mandata_obtinere(m, V);
    CREDO_VERUM(x->genus == MANDATUM_TEXTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->textus, "Ok");

    imprimere("\n--- Thema alterum: figura alia ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, I, m);
    CREDO_AEQUALIS_I32(vocationes, I);              /* non vocata */
    /* figura_finium: rectangulum vacuum unum */
    CREDO_AEQUALIS_I32(mandata_numerus(m), IV);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_FALSUM(x->impletum);

    imprimere("\n--- Partes sine figura: coetus solus ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, II, m);
    CREDO_AEQUALIS_I32(mandata_numerus(m), III);

    imprimere("\n--- Puritas: bis idem ---\n");
    m   = mandata_creare(piscina, intern);
    m2  = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    pingere(radix, reg, ZEPHYRUM, m2);
    CREDO_VERUM(mandata_aequalia(m, m2));
    CREDO_AEQUALIS_I32(vocationes, III);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
