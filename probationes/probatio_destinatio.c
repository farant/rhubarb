/* probatio_destinatio.c - strategia geometrica, sectio, captura,
 * focus, ordo foci */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "fenestra.h"
#include "componens.h"
#include "motus.h"
#include "destinatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Componens cum finibus et focusabilitate uno ictu */
interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h,
                    b32  focusabilis)
{
     Componens* c;
         Fines  f;

    c           = componens_creare(p, in, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    componens_ponere_fines(c, f);
    componens_ponere_focusabilis(c, focusabilis);
    redde c;
}

/* Eventus muris ad punctum */
interior Eventus
mus (
    eventus_genus_t genus,
                i32 x,
                i32 y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = genus;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
}

/* Eventus sine puncto (clavis, rotula) */
interior Eventus
eventus_nudus (
    eventus_genus_t genus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = genus;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* pannus;
              Componens* b1;
              Componens* b2;
              Componens* b3;
              Componens* umbra;
              Componens* tabula;
                  Motus  motus;
                Eventus  e;
             Destinatio  d;
                 chorda  focus;
                 chorda  vacua;

    piscina = piscina_generare_dynamicum("probatio_destinatio",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    motus_initiare(&motus, piscina);
    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;

    /* radix 0,0,200,100
     *   b1     10,10,50,20   focusabilis
     *   pannus 70,0,130,70   translatio (0,50), sectio; liberi in
     *                        spatio panni:
     *     b2     0,0,40,20   focusabilis  -> schirmo 70..110, 50..70
     *     tabula 50,0,60,40  proprius     -> schirmo 120..180, 50..90
     *                        (infra pannum 70.. praecisa)
     */
    radix  = nodus(piscina, intern, "radix", PARTES_NULLUM,
                   ZEPHYRUM, ZEPHYRUM, CC, C, FALSUM);
    b1     = nodus(piscina, intern, "b1", PARTES_BOTTONE,
                   X, X, L, XX, VERUM);
    pannus = nodus(piscina, intern, "pannus", PARTES_NULLUM,
                   LXX, ZEPHYRUM, CXXX, LXX, FALSUM);
    componens_ponere_transformatio(pannus, ZEPHYRUM, L, I);
    componens_ponere_sectio(pannus, VERUM);
    b2     = nodus(piscina, intern, "b2", PARTES_BOTTONE,
                   ZEPHYRUM, ZEPHYRUM, XL, XX, VERUM);
    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA,
                   L, ZEPHYRUM, LX, XL, VERUM);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    /* b3 30,10,50,20 supra b1 (frater posterior = ordo z) */
    b3 = nodus(piscina, intern, "b3", PARTES_NULLUM,
               XXX, X, L, XX, FALSUM);
    componens_addere_liberum(radix, b1);
    componens_addere_liberum(radix, pannus);
    componens_addere_liberum(radix, b3);
    componens_addere_liberum(pannus, b2);
    componens_addere_liberum(pannus, tabula);

    imprimere("\n--- Ictus in b1 ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, XV, XV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "b1");
    CREDO_VERUM(chorda_vacua(d.id_captum));
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), II);  /* b1, radix */
    CREDO_VERUM(destinatio_componens(&d) == b1);
    CREDO_AEQUALIS_S32(d.punctum_locale.x, V);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);

    imprimere("\n--- Frater posterior SUPRA priorem (ordo z) ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, XL, XV);   /* in b1 ET b3 */
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "b3");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, X);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);

    imprimere("\n--- Ictus in tabula per translationem panni ---\n");
    /* schirmo (125,60) -> pannus (55,10) -> tabula locale (5,10) */
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LX);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "tabula");
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), III);
    CREDO_VERUM(destinatio_componens(&d) == tabula);
    CREDO_AEQUALIS_S32(d.punctum_locale.x, V);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, X);

    imprimere("\n--- Punctum in panno SUPRA liberos translatos ---\n");
    /* (80,10): in panno, sed liberi a y=50 incipiunt -> pannus ipse */
    e = mus(EVENTUS_MUS_DEPRESSUS, LXXX, X);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "pannus");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, X);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, X);

    imprimere("\n--- Sectio praecidit liberum extra pannum ---\n");
    /* (125,80): in tabula non praecisa, sed infra pannum (70) */
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LXXX);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "radix");
    componens_ponere_sectio(pannus, FALSUM);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "tabula");
    componens_ponere_sectio(pannus, VERUM);

    imprimere("\n--- Liberum cum finibus NEGATIVIS (signatum) ---\n");
    /* umbra (-20,-20,40,40) in panno: schirmo 50..90, 30..70 -
     * ante translationem panni (0,50) et originem (70,0) */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    e = mus(EVENTUS_MUS_DEPRESSUS, LXXV, XXXV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "umbra");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, XXV);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);

    imprimere("\n--- Extra omnia -> radix ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, LXV, XC);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "radix");
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), I);

    imprimere("\n--- Captura vincit geometriam ---\n");
    motus_captura_ponere(&motus, chorda_ex_literis("tabula", piscina));
    e = mus(EVENTUS_MUS_MOTUS, XV, XV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_captum, "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "b1");
    CREDO_VERUM(destinatio_componens(&d) == tabula);
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), III);
    /* captura ignota: geometria manet */
    motus_captura_ponere(&motus, chorda_ex_literis("nemo", piscina));
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_captum, "nemo");
    CREDO_VERUM(destinatio_componens(&d) == b1);
    motus_captura_tollere(&motus);

    imprimere("\n--- Clavis it ad focum ---\n");
    e      = eventus_nudus(EVENTUS_CLAVIS_DEPRESSUS);
    focus  = chorda_ex_literis("b2", piscina);
    d      = destinatio_geometrica(radix, &motus, focus, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b2);
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), III);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));
    /* sine foco: radix */
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == radix);

    imprimere("\n--- Rotula sine puncto it ad focum ---\n");
    e = eventus_nudus(EVENTUS_MUS_ROTULA);
    d = destinatio_geometrica(radix, &motus, focus, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b2);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));

    imprimere("\n--- Ordo foci ---\n");
    focus = destinatio_focus_proximum(radix, vacua, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "b1");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "b2");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "tabula");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "b1");        /* involvit */
    focus = destinatio_focus_praecedens(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "tabula");
    focus = destinatio_focus_praecedens(radix, vacua, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "tabula");    /* ultimus */
    focus = destinatio_focus_proximum(radix,
        chorda_ex_literis("nemo", piscina), piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(focus, "b1");        /* ignotus */

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
