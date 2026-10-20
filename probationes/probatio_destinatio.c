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
                s32 x,
                s32 y)
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


interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               Componens* radix;
               Componens* pannus;
               Componens* b1;
               Componens* b2;
               Componens* b3;
               Componens* tabula;
                   Motus  motus;
                 Eventus  e;
              Destinatio  d;
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    e = mus(EVENTUS_MUS_DEPRESSUS, XV, XV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "b1");
    CREDO_VERUM(chorda_vacua(d.id_captum));
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), II);  /* b1, radix */
    CREDO_VERUM(destinatio_componens(&d) == b1);
    CREDO_AEQUALIS_S32(d.punctum_locale.x, V);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               Componens* radix;
               Componens* pannus;
               Componens* b1;
               Componens* b2;
               Componens* b3;
               Componens* tabula;
                   Motus  motus;
                 Eventus  e;
              Destinatio  d;
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    e = mus(EVENTUS_MUS_DEPRESSUS, XL, XV);   /* in b1 ET b3 */
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "b3");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, X);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               Componens* radix;
               Componens* pannus;
               Componens* b1;
               Componens* b2;
               Componens* b3;
               Componens* tabula;
                   Motus  motus;
                 Eventus  e;
              Destinatio  d;
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* schirmo (125,60) -> pannus (55,10) -> tabula locale (5,10) */
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LX);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "tabula");
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), III);
    CREDO_VERUM(destinatio_componens(&d) == tabula);
    CREDO_AEQUALIS_S32(d.punctum_locale.x, V);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, X);
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               Componens* radix;
               Componens* pannus;
               Componens* b1;
               Componens* b2;
               Componens* b3;
               Componens* tabula;
                   Motus  motus;
                 Eventus  e;
              Destinatio  d;
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* (80,10): in panno, sed liberi a y=50 incipiunt -> pannus ipse */
    e = mus(EVENTUS_MUS_DEPRESSUS, LXXX, X);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "pannus");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, X);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, X);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               Componens* radix;
               Componens* pannus;
               Componens* b1;
               Componens* b2;
               Componens* b3;
               Componens* tabula;
                   Motus  motus;
                 Eventus  e;
              Destinatio  d;
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* (125,80): in tabula non praecisa, sed infra pannum (70) */
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LXXX);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "radix");
    componens_ponere_sectio(pannus, FALSUM);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "tabula");
    componens_ponere_sectio(pannus, VERUM);
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
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
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
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
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    e = mus(EVENTUS_MUS_DEPRESSUS, LXV, XC);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "radix");
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), I);
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
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
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    /* tabula in panno translato (0,50): schirmo (125,60) -> locale
     * (5,10) - catena parentum cum translatione */
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LX);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "tabula");
    {
        Punctum s;
        Punctum l;

        s.x  = CXXV;
        s.y  = LX;
        l    = destinatio_ad_locale(tabula, s);
        CREDO_AEQUALIS_S32(l.x, d.punctum_locale.x);
        CREDO_AEQUALIS_S32(l.y, d.punctum_locale.y);
        CREDO_AEQUALIS_S32(l.x, V);
        CREDO_AEQUALIS_S32(l.y, X);
        /* extra tabulam sinistrorsum: negativum */
        s.x  = CXV;
        l    = destinatio_ad_locale(tabula, s);
        CREDO_AEQUALIS_S32(l.x, -V);
    }
}

interior vacuum
_sectio_nona (
    CredoContextus* c)
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
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    /* sinistrorsum b1 (10..60) et radicis: nihil tangitur - non b1,
     * non radix (positio vera, non ad marginem coarctata) */
    e = mus(EVENTUS_MUS_MOTUS, -V, XV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));
    /* tractus extra fenestram: captura tenet (punctum_locale ictus
     * GEOMETRICI est - hic nullus, ergo 0) */
    motus_captura_ponere(&motus, chorda_ex_literis("b1", piscina));
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b1);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));
    motus_captura_tollere(&motus);
}

interior vacuum
_sectio_decima (
    CredoContextus* c)
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
                  chorda  vacua;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
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
}

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
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

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    e      = eventus_nudus(EVENTUS_CLAVIS_DEPRESSUS);
    focus  = chorda_ex_literis("b2", piscina);
    d      = destinatio_geometrica(radix, &motus, focus, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b2);
    CREDO_AEQUALIS_I32(xar_numerus(d.ascensus), III);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));
    /* sine foco: radix */
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == radix);
}

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
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

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    motus_initiare(&motus, piscina);

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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    /* focus ut post sectionem "Clavis it ad focum" */
    focus  = chorda_ex_literis("b2", piscina);
    e      = eventus_nudus(EVENTUS_MUS_ROTULA);
    d      = destinatio_geometrica(radix, &motus, focus, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b2);
    CREDO_VERUM(chorda_vacua(d.id_geometricum));
}

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
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

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    /* focus ut post sectionem "Clavis it ad focum" */
    focus  = chorda_ex_literis("b2", piscina);
    e      = eventus_nudus(EVENTUS_TEXTUS);
    d      = destinatio_geometrica(radix, &motus, focus, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == b2);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_VERUM(destinatio_componens(&d) == radix);
}

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
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
                  chorda  focus;
                  chorda  vacua;

    piscina        = c->piscina;
    intern         = internamentum_creare(piscina);
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

    /* mundus ut post sectionem "Liberum cum finibus NEGATIVIS":
     * umbra in panno */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
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
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Ictus in b1", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Frater posterior SUPRA priorem (ordo z)", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Ictus in tabula per translationem panni", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Punctum in panno SUPRA liberos translatos", _sectio_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "Sectio praecidit liberum extra pannum", _sectio_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Liberum cum finibus NEGATIVIS (signatum)", _sectio_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Extra omnia -> radix", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "A6: destinatio_ad_locale = punctum_locale", _sectio_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "A3c: extra fenestram (x negativum)", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "Captura vincit geometriam", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Clavis it ad focum", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Rotula sine puncto it ad focum", _sectio_decima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Textus commissus it ad focum (scriba S1c)",
        _sectio_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Ordo foci", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("destinatio", SECTIONES);
}
