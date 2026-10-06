/* probatio_aemulator_vectores.c - vectores aemulatoris, phasis A
 * (aemulator-plan A0: oraculum ante codicem)
 *
 * Iterator generalis super probationes/fixa/aemulator/vectores.stml
 * (ex Ghostty @ 12752b2 versi, et domus): nucleus per Probandum
 * (tabula functionum) tangitur, ut API aemulatoris (A1, Franus
 * probat) iteratorem non praeiudicet.
 *
 * 0.  Vectores sani SINE nucleo: effugia solubilia, magnitudo,
 *     gradus, cellulae cum x y, omne exemplum cum assertione.
 * I.  Iterator se ipsum probat contra nucleum fictum: exemplum
 *     congruens transit, discrepans frangit - iudex qui frangere non
 *     potest mortuus est.
 * II. Vectores contra nucleum verum. DEBITA PER NOMEN: nucleus absens
 *     = omnia debita (numerus imprimitur); exemplum debitum="causa"
 *     frangere licet, sed debitum solutum FRANGIT (promove).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "filum.h"
#include "xar.h"
#include "stml.h"
#include "stilus_terminalis.h"
#include "aemulator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;


/* ==================================================
 * Probandum: quod iterator a nucleo poscit
 * ================================================== */

nomen structura {
    vacuum* (*creare)   (Piscina* p, i32 latitudo, i32 altitudo);
    vacuum  (*scribere) (vacuum* t, constans i8* octeti, i32 n);
    /* effusio plana ut Ghostty plainString */
    chorda  (*textus)   (vacuum* t, Piscina* p);
    vacuum  (*cursor)   (vacuum* t, s32* x, s32* y, b32* pendens,
                         b32* visibilis);
    /* FALSUM si extra schirmum; latitudo: angusta lata cauda caput;
     * stilus: SGR canonicum (stilus_codificare) */
    b32     (*cellula)  (vacuum* t, s32 x, s32 y, Piscina* p,
                         chorda* graphema,
                         constans character** latitudo,
                         chorda* stilus);
    b32     (*altera)   (vacuum* t);
    i32     (*ignota)   (vacuum* t);
    b32     (*modus)    (vacuum* t, i32 numerus, b32 privatus);
} Probandum;


/* ==================================================
 * Effugia vectorum: \xHH \n \r \t \\
 * ================================================== */

interior s32
hex_valor (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    redde -I;
}

/* FALSUM si effugium malum (vitium plagulae vectorum) */
interior b32
effugia_solvere (
     chorda  in,
     chorda* ex)
{
    ChordaAedificator* a;
                  i32  i;
                  s32  h;
                  s32  l;

    a = chorda_aedificator_creare(piscina, in.mensura + I);
    per (i = ZEPHYRUM; i < in.mensura; i++)
    {
        si (in.datum[i] != '\\')
        {
            chorda_aedificator_appendere_character(a,
                (character)in.datum[i]);
            perge;
        }
        si (i + I >= in.mensura)
        {
            redde FALSUM;
        }
        i++;
        commutatio (in.datum[i])
        {
            casus 'n':
                chorda_aedificator_appendere_character(a, '\n');
                frange;
            casus 'r':
                chorda_aedificator_appendere_character(a, '\r');
                frange;
            casus 't':
                chorda_aedificator_appendere_character(a, '\t');
                frange;
            casus '\\':
                chorda_aedificator_appendere_character(a, '\\');
                frange;
            casus 'x':
                si (i + II >= in.mensura)
                {
                    redde FALSUM;
                }
                h = hex_valor(in.datum[i + I]);
                l = hex_valor(in.datum[i + II]);
                si (h < ZEPHYRUM || l < ZEPHYRUM)
                {
                    redde FALSUM;
                }
                chorda_aedificator_appendere_character(a,
                    (character)(h * XVI + l));
                i += II;
                frange;
            ordinarius:
                redde FALSUM;
        }
    }
    *ex = chorda_aedificator_finire(a);
    redde VERUM;
}

/* octeti legibiles in nuntiis (effugia eadem) */
interior constans character*
legibile (
    chorda c)
{
    hic_manens constans character  notae[] = "0123456789ABCDEF";
                ChordaAedificator* a;
                              i32  i;
                              i32  u;

    a = chorda_aedificator_creare(piscina, c.mensura * IV + II);
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        u = ((i32)c.datum[i]) & 0xFF;
        si (u == '\n')
        {
            chorda_aedificator_appendere_literis(a, "\\n");
        }
        alioquin si (u >= 0x20 && u < 0x7F && u != '\\')
        {
            chorda_aedificator_appendere_character(a, (character)u);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "\\x");
            chorda_aedificator_appendere_character(a,
                notae[u >> IV]);
            chorda_aedificator_appendere_character(a,
                notae[u & 0xF]);
        }
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(a), piscina);
}


/* ==================================================
 * Lectio attributorum
 * ================================================== */

interior b32
numerum_legere (
             StmlNodus* n,
    constans character* titulus,
                   s32* valor)
{
    chorda* a;

    a = stml_attributum_capere(n, titulus);
    redde a ? chorda_ut_s32(*a, valor) : FALSUM;
}

interior b32
veritatem_legere (
             StmlNodus* n,
    constans character* titulus,
                   b32* valor)
{
    chorda* a;

    a = stml_attributum_capere(n, titulus);
    si (!a)
    {
        redde FALSUM;
    }
    *valor = chorda_aequalis_literis(*a, "verum");
    redde VERUM;
}

interior constans character*
literae (
             StmlNodus* n,
    constans character* titulus)
{
    chorda* a;

    a = stml_attributum_capere(n, titulus);
    redde a ? chorda_ut_cstr(*a, piscina) : "?";
}


/* ==================================================
 * Iterator
 * ================================================== */

nomen structura {
    constans character* titulus;
    constans character* fons;
                   i32  gradus;
                   i32  assertiones;
                   b32  transiit;
                   b32  loqui;
} Iudicium;

interior vacuum
discrepantia (
               Iudicium* iu,
     constans character* campus,
     constans character* expectatum,
     constans character* receptum)
{
    iu->transiit = FALSUM;
    si (iu->loqui)
    {
        imprimere("  discrepantia '%s' (%s) gradus %d: %s "
                  "expectatum '%s' receptum '%s'\n", iu->titulus,
                  iu->fons, (integer)iu->gradus, campus, expectatum,
                  receptum);
    }
}

interior vacuum
numerum_conferre (
               Iudicium* iu,
     constans character* campus,
                    s32  expectatum,
                    s32  receptum)
{
    character e[XXIV];
    character r[XXIV];

    iu->assertiones++;
    si (expectatum != receptum)
    {
        sprintf(e, "%d", (integer)expectatum);
        sprintf(r, "%d", (integer)receptum);
        discrepantia(iu, campus, e, r);
    }
}

interior vacuum
veritatem_conferre (
               Iudicium* iu,
     constans character* campus,
                    b32  expectatum,
                    b32  receptum)
{
    iu->assertiones++;
    si ((expectatum ? I : ZEPHYRUM) != (receptum ? I : ZEPHYRUM))
    {
        discrepantia(iu, campus, expectatum ? "verum" : "falsum",
                     receptum ? "verum" : "falsum");
    }
}

interior vacuum
chordam_conferre (
               Iudicium* iu,
     constans character* campus,
                 chorda  expectatum,
                 chorda  receptum)
{
    iu->assertiones++;
    si (!chorda_aequalis(expectatum, receptum))
    {
        discrepantia(iu, campus, legibile(expectatum),
                     legibile(receptum));
    }
}

/* attributum effugiis scriptum -> octeti; vitium plagulae frangit */
interior b32
octetos_legere (
               Iudicium* iu,
              StmlNodus* n,
     constans character* titulus,
                 chorda* ex)
{
    chorda* a;

    a = stml_attributum_capere(n, titulus);
    si (!a)
    {
        redde FALSUM;
    }
    si (!effugia_solvere(*a, ex))
    {
        discrepantia(iu, titulus, "effugia sana", "effugium malum");
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
cellulas_conferre (
                 Iudicium* iu,
       constans Probandum* pr,
                   vacuum* t,
                StmlNodus* gradus)
{
                    Xar* cellulae;
              StmlNodus* c;
                    s32  x;
                    s32  y;
                 chorda  graphema;
                 chorda  stilus;
                 chorda  expectatum;
     constans character* latitudo;
                    b32  vacua;
                    i32  i;
              character  locus[XLVIII];

    cellulae = stml_invenire_omnes_liberos(gradus, "cellula", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(cellulae); i++)
    {
        c = *(StmlNodus**)xar_obtinere(cellulae, i);
        si (!numerum_legere(c, "x", &x) || !numerum_legere(c, "y", &y))
        {
            discrepantia(iu, "cellula", "x et y", "absentes");
            perge;
        }
        sprintf(locus, "cellula (%d,%d)", (integer)x, (integer)y);
        graphema.mensura  = ZEPHYRUM;
        graphema.datum    = NIHIL;
        stilus            = graphema;
        latitudo          = "?";
        iu->assertiones++;
        si (!pr->cellula(t, x, y, piscina, &graphema, &latitudo,
                &stilus))
        {
            discrepantia(iu, locus, "intra schirmum", "extra");
            perge;
        }
        si (octetos_legere(iu, c, "textus", &expectatum))
        {
            chordam_conferre(iu, locus, expectatum, graphema);
        }
        si (veritatem_legere(c, "vacua", &vacua))
        {
            veritatem_conferre(iu, locus, vacua,
                               graphema.mensura == ZEPHYRUM);
        }
        si (stml_attributum_habet(c, "latitudo"))
        {
            iu->assertiones++;
            si (strcmp(literae(c, "latitudo"), latitudo) != ZEPHYRUM)
            {
                discrepantia(iu, locus, literae(c, "latitudo"),
                             latitudo);
            }
        }
        si (octetos_legere(iu, c, "stilus", &expectatum))
        {
            chordam_conferre(iu, locus, expectatum, stilus);
        }
    }
}

interior vacuum
modos_conferre (
                 Iudicium* iu,
       constans Probandum* pr,
                   vacuum* t,
                StmlNodus* gradus)
{
          Xar* modi;
    StmlNodus* m;
          s32  numerus;
          b32  status;
          i32  i;

    modi = stml_invenire_omnes_liberos(gradus, "modus", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(modi); i++)
    {
        m = *(StmlNodus**)xar_obtinere(modi, i);
        si (   !numerum_legere(m, "privatus", &numerus)
            || !veritatem_legere(m, "status", &status))
        {
            discrepantia(iu, "modus", "privatus et status", "absentes");
            perge;
        }
        veritatem_conferre(iu, "modus", status,
                           pr->modus(t, (i32)numerus, VERUM));
    }
}

/* exemplum totum: VERUM si omnes assertiones congruunt. Nucleus
 * absens (creare NIHIL) -> FALSUM, *absens VERUM. */
interior b32
exemplum_currere (
    constans Probandum* pr,
             StmlNodus* exemplum,
                   b32  loqui,
                   b32* absens)
{
     Iudicium  iu;
          Xar* gradus;
    StmlNodus* g;
       vacuum* t;
          s32  latitudo;
          s32  altitudo;
          s32  n;
          s32  x;
          s32  y;
          b32  pendens;
          b32  visibilis;
          b32  b;
       chorda  octeti;
          i32  i;

    *absens         = FALSUM;
    iu.titulus      = literae(exemplum, "titulus");
    iu.fons         = literae(exemplum, "fons");
    iu.gradus       = ZEPHYRUM;
    iu.assertiones  = ZEPHYRUM;
    iu.transiit     = VERUM;
    iu.loqui        = loqui;
    si (   !numerum_legere(exemplum, "latitudo", &latitudo)
        || !numerum_legere(exemplum, "altitudo", &altitudo))
    {
        discrepantia(&iu, "magnitudo", "latitudo et altitudo",
                     "absentes");
        redde FALSUM;
    }
    t = pr->creare(piscina, (i32)latitudo, (i32)altitudo);
    si (!t)
    {
        *absens = VERUM;
        redde FALSUM;
    }
    gradus = stml_invenire_omnes_liberos(exemplum, "gradus", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(gradus); i++)
    {
        g          = *(StmlNodus**)xar_obtinere(gradus, i);
        iu.gradus  = (i32)(i + I);
        si (octetos_legere(&iu, g, "initus", &octeti))
        {
            pr->scribere(t, octeti.datum, octeti.mensura);
        }
        pr->cursor(t, &x, &y, &pendens, &visibilis);
        si (numerum_legere(g, "cursor_x", &n))
        {
            numerum_conferre(&iu, "cursor_x", n, x);
        }
        si (numerum_legere(g, "cursor_y", &n))
        {
            numerum_conferre(&iu, "cursor_y", n, y);
        }
        si (veritatem_legere(g, "pendens", &b))
        {
            veritatem_conferre(&iu, "pendens", b, pendens);
        }
        si (veritatem_legere(g, "visibilis", &b))
        {
            veritatem_conferre(&iu, "visibilis", b, visibilis);
        }
        si (veritatem_legere(g, "altera", &b))
        {
            veritatem_conferre(&iu, "altera", b, pr->altera(t));
        }
        si (numerum_legere(g, "ignota", &n))
        {
            numerum_conferre(&iu, "ignota", n, (s32)pr->ignota(t));
        }
        si (octetos_legere(&iu, g, "textus", &octeti))
        {
            chordam_conferre(&iu, "textus", octeti,
                             pr->textus(t, piscina));
        }
        cellulas_conferre(&iu, pr, t, g);
        modos_conferre(&iu, pr, t, g);
    }
    /* exemplum sine assertione nihil probat: vitium plagulae */
    si (iu.assertiones == ZEPHYRUM)
    {
        discrepantia(&iu, "assertiones", "plures quam nulla", "nulla");
    }
    redde iu.transiit;
}


/* ==================================================
 * Validatio (0): vectores sani etiam sine nucleo
 * ================================================== */

interior b32
effugia_sana (
             StmlNodus* n,
    constans character* titulus,
                   i32* assertiones)
{
     chorda* a;
     chorda  ex;

    a = stml_attributum_capere(n, titulus);
    si (!a)
    {
        redde VERUM;
    }
    si (assertiones)
    {
        (*assertiones)++;
    }
    redde effugia_solvere(*a, &ex);
}

/* VERUM si exemplum sanum; causa impressa si non */
interior b32
exemplum_validare (
    StmlNodus* exemplum)
{
    hic_manens constans character* campi[] = {
        "cursor_x", "cursor_y", "pendens", "visibilis", "altera",
        "ignota"
    };
          Xar* gradus;
          Xar* filii;
    StmlNodus* g;
    StmlNodus* c;
          s32  n;
          i32  assertiones;
          i32  propriae;
          i32  i;
          i32  j;
          i32  k;

    si (   !numerum_legere(exemplum, "latitudo", &n) || n < I
        || !numerum_legere(exemplum, "altitudo", &n) || n < I
        || !stml_attributum_habet(exemplum, "fons"))
    {
        imprimere("  vitium '%s': magnitudo aut fons\n",
            literae(exemplum, "titulus"));
        redde FALSUM;
    }
    gradus       = stml_invenire_omnes_liberos(exemplum, "gradus",
        piscina);
    assertiones  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(gradus); i++)
    {
        g = *(StmlNodus**)xar_obtinere(gradus, i);
        si (   !effugia_sana(g, "initus", NIHIL)
            || !effugia_sana(g, "textus", &assertiones))
        {
            imprimere("  vitium '%s' gradus %d: effugium malum\n",
                literae(exemplum, "titulus"), (integer)(i + I));
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < (i32)(magnitudo(campi)
                / magnitudo(campi[ZEPHYRUM])); k++)
        {
            si (stml_attributum_habet(g, campi[k]))
            {
                assertiones++;
            }
        }
        filii = stml_invenire_omnes_liberos(g, "cellula", piscina);
        per (j = ZEPHYRUM; j < xar_numerus(filii); j++)
        {
            c         = *(StmlNodus**)xar_obtinere(filii, j);
            propriae  = ZEPHYRUM;
            si (   !numerum_legere(c, "x", &n)
                || !numerum_legere(c, "y", &n)
                || !effugia_sana(c, "textus", &propriae)
                || !effugia_sana(c, "stilus", &propriae))
            {
                imprimere("  vitium '%s' gradus %d: cellula\n",
                    literae(exemplum, "titulus"), (integer)(i + I));
                redde FALSUM;
            }
            propriae += stml_attributum_habet(c, "vacua") ? I
                                                          : ZEPHYRUM;
            propriae += stml_attributum_habet(c, "latitudo") ? I
                                                             : ZEPHYRUM;
            si (propriae == ZEPHYRUM)
            {
                imprimere("  vitium '%s' gradus %d: cellula sine"
                    " assertione\n", literae(exemplum, "titulus"),
                    (integer)(i + I));
                redde FALSUM;
            }
            assertiones += propriae;
        }
        filii = stml_invenire_omnes_liberos(g, "modus", piscina);
        per (j = ZEPHYRUM; j < xar_numerus(filii); j++)
        {
            c = *(StmlNodus**)xar_obtinere(filii, j);
            si (   !numerum_legere(c, "privatus", &n)
                || !stml_attributum_habet(c, "status"))
            {
                imprimere("  vitium '%s' gradus %d: modus\n",
                    literae(exemplum, "titulus"), (integer)(i + I));
                redde FALSUM;
            }
            assertiones++;
        }
    }
    si (xar_numerus(gradus) == ZEPHYRUM || assertiones == ZEPHYRUM)
    {
        imprimere("  vitium '%s': nullus gradus aut nulla assertio\n",
            literae(exemplum, "titulus"));
        redde FALSUM;
    }
    redde VERUM;
}


/* ==================================================
 * Nucleus fictus (I): textus semper "Hello", cursor (5, 0)
 * ================================================== */

hic_manens i32 fictus_sedes;

interior vacuum*
fictus_creare (
    Piscina* p,
        i32  latitudo,
        i32  altitudo)
{
    (vacuum)p;
    (vacuum)latitudo;
    (vacuum)altitudo;
    redde &fictus_sedes;
}

interior vacuum
fictus_scribere (
             vacuum* t,
        constans i8* octeti,
                i32  n)
{
    (vacuum)t;
    (vacuum)octeti;
    (vacuum)n;
}

interior chorda
fictus_textus (
     vacuum* t,
    Piscina* p)
{
    (vacuum)t;
    redde chorda_ex_literis("Hello", p);
}

interior vacuum
fictus_cursor (
    vacuum* t,
       s32* x,
       s32* y,
       b32* pendens,
       b32* visibilis)
{
    (vacuum)t;
    *x          = V;
    *y          = ZEPHYRUM;
    *pendens    = FALSUM;
    *visibilis  = VERUM;
}

interior b32
fictus_cellula (
                 vacuum*  t,
                    s32   x,
                    s32   y,
                Piscina*  p,
                 chorda*  graphema,
     constans character** latitudo,
                 chorda*  stilus)
{
    (vacuum)t;
    (vacuum)x;
    (vacuum)y;
    (vacuum)p;
    (vacuum)graphema;
    (vacuum)latitudo;
    (vacuum)stilus;
    redde FALSUM;
}

interior b32
fictus_altera (
    vacuum* t)
{
    (vacuum)t;
    redde FALSUM;
}

interior i32
fictus_ignota (
    vacuum* t)
{
    (vacuum)t;
    redde ZEPHYRUM;
}

interior b32
fictus_modus (
    vacuum* t,
       i32  numerus,
       b32  privatus)
{
    (vacuum)t;
    (vacuum)numerus;
    (vacuum)privatus;
    redde FALSUM;
}

/* nucleus verus: aemulator (A1) */
interior vacuum*
verus_creare (
    Piscina* p,
        i32  latitudo,
        i32  altitudo)
{
    AemulatorConfiguratio cfg;

    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo = latitudo;
    cfg.altitudo = altitudo;
    redde aemulator_creare(p, &cfg);
}

interior vacuum
verus_scribere (
             vacuum* t,
        constans i8* octeti,
                i32  n)
{
    aemulator_scribere((Aemulator*)t, octeti, n);
}

interior chorda
verus_textus (
     vacuum* t,
    Piscina* p)
{
    redde aemulator_textum_effundere((Aemulator*)t, p);
}

interior vacuum
verus_cursor (
    vacuum* t,
       s32* x,
       s32* y,
       b32* pendens,
       b32* visibilis)
{
    AemulatorCursor c;

    c           = aemulator_cursor((Aemulator*)t);
    *x          = (s32)c.x;
    *y          = (s32)c.y;
    *pendens    = c.pendens;
    *visibilis  = c.visibilis;
}

interior b32
verus_cellula (
                 vacuum*  t,
                    s32   x,
                    s32   y,
                Piscina*  p,
                 chorda*  graphema,
     constans character** latitudo,
                 chorda*  stilus)
{
    hic_manens constans character* tituli[] = {
        "angusta", "lata", "cauda", "caput"
    };
     AemulatorCellula  c;
    ChordaAedificator* a;

    si (   x < ZEPHYRUM || y < ZEPHYRUM
        || !aemulator_cellula((Aemulator*)t, (i32)x, (i32)y, &c))
    {
        redde FALSUM;
    }
    *graphema  = c.graphema;
    *latitudo  = tituli[c.latitudo];
    a          = chorda_aedificator_creare(p, LXIV);
    stilus_codificare(a, NIHIL, &c.stilus, STILUS_CODIFICATIO_PLENA);
    *stilus    = chorda_aedificator_finire(a);
    redde VERUM;
}

interior b32
verus_altera (
    vacuum* t)
{
    redde aemulator_alterum((Aemulator*)t);
}

interior i32
verus_ignota (
    vacuum* t)
{
    redde aemulator_ignota((Aemulator*)t);
}

interior b32
verus_modus (
    vacuum* t,
       i32  numerus,
       b32  privatus)
{
    redde aemulator_modus((Aemulator*)t, numerus, privatus);
}

interior StmlNodus*
exemplum_titulo (
                   Xar* exempla,
    constans character* titulus)
{
    StmlNodus* e;
          i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
    {
        e = *(StmlNodus**)xar_obtinere(exempla, i);
        si (strcmp(literae(e, "titulus"), titulus) == ZEPHYRUM)
        {
            redde e;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
       StmlResultus  res;
                Xar* exempla;
          StmlNodus* e;
          Probandum  fictus;
          Probandum  verus;
             chorda  octeti;
                b32  absens;
                b32  transiit;
                i32  i;
                i32  aliena;
                i32  viridia;
                i32  debita;
                i32  absentia;

    piscina = piscina_generare_dynamicum("probatio_aemulator_vectores",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    res = stml_legere_ex_literis(chorda_ut_cstr(filum_legere_totum(
        "probationes/fixa/aemulator/vectores.stml", piscina), piscina),
        piscina, intern);
    CREDO_VERUM(res.successus);
    si (!res.successus)
    {
        credo_imprimere_compendium();
        redde I;
    }
    exempla = stml_invenire_omnes_liberos(res.elementum_radix, "casus",
        piscina);
    aliena = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
    {
        e = *(StmlNodus**)xar_obtinere(exempla, i);
        si (strcmp(literae(e, "fons"), "domus") != ZEPHYRUM)
        {
            aliena++;
        }
    }
    imprimere("vectores: %d exempla (%d ex Ghostty @ %s, %d domus)\n",
        (integer)xar_numerus(exempla), (integer)aliena,
        literae(res.elementum_radix, "pin"),
        (integer)(xar_numerus(exempla) - aliena));
    CREDO_VERUM(xar_numerus(exempla) > XXX);

    imprimere("\n--- 0: vectores sani sine nucleo ---\n");
    per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
    {
        CREDO_VERUM(exemplum_validare(
            *(StmlNodus**)xar_obtinere(exempla, i)));
    }

    imprimere("\n--- I: iterator contra nucleum fictum ---\n");
    CREDO_VERUM(effugia_solvere(chorda_ex_literis(
        "A\\x1B[\\n\\\\", piscina), &octeti));
    CREDO_VERUM(chorda_aequalis_literis(octeti, "A\033[\n\\"));
    CREDO_FALSUM(effugia_solvere(chorda_ex_literis("\\x1", piscina),
        &octeti));
    CREDO_FALSUM(effugia_solvere(chorda_ex_literis("\\q", piscina),
        &octeti));
    fictus.creare    = fictus_creare;
    fictus.scribere  = fictus_scribere;
    fictus.textus    = fictus_textus;
    fictus.cursor    = fictus_cursor;
    fictus.cellula   = fictus_cellula;
    fictus.altera    = fictus_altera;
    fictus.ignota    = fictus_ignota;
    fictus.modus     = fictus_modus;
    e                = exemplum_titulo(exempla, "basic print");
    CREDO_NON_NIHIL(e);
    si (e)
    {
        CREDO_VERUM(exemplum_currere(&fictus, e, FALSUM, &absens));
    }
    /* gradus I cursorem (0,0) poscit, fictus (5,0) dat */
    e = exemplum_titulo(exempla, "cursor movement");
    CREDO_NON_NIHIL(e);
    si (e)
    {
        CREDO_FALSUM(exemplum_currere(&fictus, e, FALSUM, &absens));
    }
    /* solum textus discrepat (cursor 5,0 congruit) */
    e = exemplum_titulo(exempla, "backspace");
    CREDO_NON_NIHIL(e);
    si (e)
    {
        CREDO_FALSUM(exemplum_currere(&fictus, e, FALSUM, &absens));
    }
    /* solum pendens discrepat */
    e = exemplum_titulo(exempla, "linefeed unsets pending wrap");
    CREDO_NON_NIHIL(e);
    si (e)
    {
        CREDO_FALSUM(exemplum_currere(&fictus, e, FALSUM, &absens));
    }
    /* cellula extra: fictus omnia FALSUM reddit */
    e = exemplum_titulo(exempla, "print wide char");
    CREDO_NON_NIHIL(e);
    si (e)
    {
        CREDO_FALSUM(exemplum_currere(&fictus, e, FALSUM, &absens));
    }

    imprimere("\n--- II: vectores contra nucleum ---\n");
    verus.creare    = verus_creare;
    verus.scribere  = verus_scribere;
    verus.textus    = verus_textus;
    verus.cursor    = verus_cursor;
    verus.cellula   = verus_cellula;
    verus.altera    = verus_altera;
    verus.ignota    = verus_ignota;
    verus.modus     = verus_modus;
    viridia         = ZEPHYRUM;
    debita          = ZEPHYRUM;
    absentia        = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
    {
        e         = *(StmlNodus**)xar_obtinere(exempla, i);
        transiit  = exemplum_currere(&verus, e, VERUM, &absens);
        si (absens)
        {
            absentia++;
        }
        alioquin si (stml_attributum_habet(e, "debitum"))
        {
            si (transiit)
            {
                imprimere("  DEBITUM SOLUTUM '%s' - promove\n",
                    literae(e, "titulus"));
                CREDO_FALSUM(transiit);
            }
            alioquin
            {
                imprimere("  debitum manet '%s': %s\n",
                    literae(e, "titulus"), literae(e, "debitum"));
                debita++;
            }
        }
        alioquin
        {
            CREDO_VERUM(transiit);
            si (transiit)
            {
                viridia++;
            }
        }
    }
    imprimere("vectores: %d viridia, %d debita, %d sine nucleo\n",
        (integer)viridia,
        (integer)debita, (integer)absentia);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
