/* probatio_elenchus.c - menu contextuale (agenda-plan M2b): fines
 * (reflexio, coactio), arbor (ids, actio, claudit), index, claves,
 * figura */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "thema.h"
#include "componens.h"
#include "mandatum.h"
#include "eventus.h"
#include "elenchus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* res: "Open" (Enter), divisor, "Rename" obscura, "Delete" (X) */
hic_manens constans ElenchusRes res_probationis[III] = {
    { "aperire", "Open", "Enter", VERUM, FALSUM },
    { "renominare", "Rename", NIHIL, FALSUM, VERUM },
    { "delere", "Delete", "X", VERUM, FALSUM }
};

interior Elenchus
elenchus_probationis (
    vacuum)
{
    Elenchus e;

    e.res      = res_probationis;
    e.numerus  = III;
    e.electa   = -I;
    redde e;
}

interior Fines
fines_ex (
    s32 x,
    s32 y,
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    redde f;
}

interior Punctum
punctum_ex (
    s32 x,
    s32 y)
{
    Punctum p;

    p.x = x;
    p.y = y;
    redde p;
}

interior b32
fines_aequales (
    Fines a,
    Fines b)
{
    redde a.x == b.x && a.y == b.y && a.latitudo == b.latitudo
        && a.altitudo == b.altitudo;
}

/* eventus clavis (littera aut clavis nominata) */
interior Eventus
clavis_ex (
    clavis_t clavis,
         i32 modificantes)
{
    Eventus ev;

    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    ev.datum.clavis.clavis        = clavis;
    ev.datum.clavis.modificantes  = modificantes;
    redde ev;
}

/* rectangulum fines et colore thematis datis (impletum aut margo) ? */
interior b32
rectangulum_adest (
    constans Mandata* md,
               Fines  f,
          ColorThema  color,
                 b32  impletum)
{
    i32 i;

    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        constans Mandatum* x;

        x = mandata_obtinere(md, i);
        si (   x->genus       == MANDATUM_RECTANGULUM
            && fines_aequales(x->fines, f) && x->impletum == impletum
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)color)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus datus: mandatum eius aut NIHIL */
interior constans Mandatum*
textus_mandatum (
       constans Mandata* md,
     constans character* textus)
{
    i32 i;

    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        constans Mandatum* x;

        x = mandata_obtinere(md, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

/* linea horizontalis intra [y0, y1) ? */
interior b32
linea_horizontalis_inter (
    constans Mandata* md,
                 s32  y0,
                 s32  y1)
{
    i32 i;

    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        constans Mandatum* x;

        x = mandata_obtinere(md, i);
        si (   x->genus              == MANDATUM_LINEA
            && x->puncta[ZEPHYRUM].y == x->puncta[I].y
            && x->puncta[ZEPHYRUM].y >= y0
            && x->puncta[ZEPHYRUM].y < y1)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fines: latitudo VI + textus + XII + clavis + VI, altitudo II + res
 * XVI + divisor VIII + II, umbra II in utraque */
interior vacuum
_probare_fines (
    CredoContextus* c)
{
       Elenchus e;
          Fines f;
          Fines area;
    ElenchusRes sine[II];

    e     = elenchus_probationis();
    area  = fines_ex(ZEPHYRUM, ZEPHYRUM, CCC, CC);
    /* "Delete"/"Rename" VI litterae, "Enter" V: VI + XXXVI + XII + XXX
     * + VI = XC; II + XLVIII + VIII + II = LX */
    f = elenchus_fines(&e, punctum_ex(X, XX), area);
    CREDO_VERUM(fines_aequales(f, fines_ex(X, XX, XCII, LXII)));
    /* margo dexter: sinistrorsum reflexus */
    f = elenchus_fines(&e, punctum_ex(CCL, XX), area);
    CREDO_AEQUALIS_S32(f.x, CCL - XCII);
    CREDO_AEQUALIS_S32(f.y, XX);
    /* margo inferus: sursum reflexus */
    f = elenchus_fines(&e, punctum_ex(X, CLXX), area);
    CREDO_AEQUALIS_S32(f.x, X);
    CREDO_AEQUALIS_S32(f.y, CLXX - LXII);
    /* area angusta: reflexio non sufficit, intra aream coactus */
    area  = fines_ex(XL, XXX, C, LXX);
    f     = elenchus_fines(&e, punctum_ex(XC, LX), area);
    CREDO_AEQUALIS_S32(f.x, XL);
    CREDO_AEQUALIS_S32(f.y, XXX);
    /* sine clavibus: intervallum nullum (VI + XXXVI + VI) */
    sine[ZEPHYRUM]         = res_probationis[ZEPHYRUM];
    sine[ZEPHYRUM].clavis  = NIHIL;
    sine[I]                = res_probationis[II];
    sine[I].clavis         = NIHIL;
    e.res                  = sine;
    e.numerus              = II;
    f = elenchus_fines(&e, punctum_ex(ZEPHYRUM, ZEPHYRUM), fines_ex(
        ZEPHYRUM, ZEPHYRUM, CCC, CC));
    CREDO_AEQUALIS_S32(f.latitudo, XLVIII + II);
    (vacuum)c;
}

/* arbor: DIALOGUS "elenchus", OPTIO "elenchus.<k>" sub divisore
 * deorsum motae; actio omnibus, claudit solum habilibus */
interior vacuum
_probare_arborem (
    CredoContextus* c)
{
               Elenchus  e;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* r;
                  Fines  f;

    e       = elenchus_probationis();
    intern  = internamentum_creare(c->piscina);
    f       = fines_ex(X, XX, XCII, LXII);
    radix   = elenchus_componere(&e, f, "calendarium.agere", c->piscina,
        intern);
    CREDO_NECESSE_NON_NIHIL(radix);
    CREDO_CHORDA_AEQUALIS_LITERIS(radix->id, "elenchus");
    CREDO_VERUM(radix->partes == PARTES_DIALOGUS);
    CREDO_VERUM(fines_aequales(radix->fines, f));
    CREDO_NECESSE(componens_numerus_liberorum(radix) == III);
    r = componens_liberum(radix, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->id, "elenchus.0");
    CREDO_VERUM(r->partes == PARTES_OPTIO);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "Open");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->actio, "calendarium.agere");
    CREDO_VERUM(r->claudit);
    /* intra marginem: x I, y II, latitudo corporis minus II */
    CREDO_VERUM(fines_aequales(r->fines, fines_ex(I, II, LXXXVIII,
        XVI)));
    /* obscura: actio sine claudit; divisor ante (VIII) */
    r = componens_liberum(radix, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->id, "elenchus.1");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->actio, "calendarium.agere");
    CREDO_FALSUM(r->claudit);
    CREDO_AEQUALIS_S32(r->fines.y, II + XVI + VIII);
    r = componens_liberum(radix, II);
    CREDO_VERUM(r->claudit);
    CREDO_AEQUALIS_S32(r->fines.y, II + XVI + VIII + XVI);
}

interior vacuum
_probare_indicem (
    CredoContextus* c)
{
               Elenchus  e;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* alius;

    e       = elenchus_probationis();
    intern  = internamentum_creare(c->piscina);
    radix   = elenchus_componere(&e, fines_ex(ZEPHYRUM, ZEPHYRUM, XCII,
        LXII), "a", c->piscina, intern);
    CREDO_NECESSE_NON_NIHIL(radix);
    CREDO_AEQUALIS_S32(elenchus_index(componens_liberum(radix, II)),
        II);
    CREDO_AEQUALIS_S32(elenchus_index(componens_liberum(radix,
        ZEPHYRUM)), ZEPHYRUM);
    CREDO_AEQUALIS_S32(elenchus_index(radix), -I);
    CREDO_AEQUALIS_S32(elenchus_index(NIHIL), -I);
    /* id simile, partes aliae */
    alius = componens_creare(c->piscina, intern, "elenchus.1",
        PARTES_ITEM);
    CREDO_AEQUALIS_S32(elenchus_index(alius), -I);
    alius = componens_creare(c->piscina, intern, "elenchus.x",
        PARTES_OPTIO);
    CREDO_AEQUALIS_S32(elenchus_index(alius), -I);
}

/* claves: obscuras saltat, circumit; j/k sine Imperio; Reditus */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
          Elenchus e;
           Eventus ev;
               s32 electa;
    ElenchusExitus x;
       ElenchusRes obscurae[II];

    (vacuum)c;
    e       = elenchus_probationis();
    electa  = -I;
    ev      = clavis_ex(CLAVIS_DEORSUM, ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_MOTUM);
    CREDO_AEQUALIS_S32(electa, ZEPHYRUM);
    e.electa = electa;
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_MOTUM);
    CREDO_AEQUALIS_S32(electa, II);
    e.electa  = electa;
    ev        = clavis_ex((clavis_t)'j', ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_MOTUM);
    CREDO_AEQUALIS_S32(electa, ZEPHYRUM);
    e.electa  = electa;
    ev        = clavis_ex(CLAVIS_SURSUM, ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_MOTUM);
    CREDO_AEQUALIS_S32(electa, II);
    /* Sursum ex -1: ultima habilis */
    e.electa  = -I;
    ev        = clavis_ex((clavis_t)'k', ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_MOTUM);
    CREDO_AEQUALIS_S32(electa, II);
    /* j sub Imperio: non menu */
    e.electa  = ZEPHYRUM;
    electa    = ZEPHYRUM;
    ev        = clavis_ex((clavis_t)'j', (i32)MOD_IMPERIUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_NIHIL);
    CREDO_AEQUALIS_S32(electa, ZEPHYRUM);
    /* Reditus: habilis ELECTUM, obscura et nulla NIHIL */
    e.electa  = II;
    electa    = II;
    ev        = clavis_ex(CLAVIS_REDITUS, ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_ELECTUM);
    CREDO_AEQUALIS_S32(electa, II);
    e.electa = I;
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_NIHIL);
    e.electa = -I;
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_NIHIL);
    /* clavis alia */
    ev = clavis_ex((clavis_t)'x', ZEPHYRUM);
    CREDO_VERUM(elenchus_clavem_agere(&e, &ev, &electa)
        == ELENCHUS_NIHIL);
    /* omnes obscurae: electa manet -1 */
    obscurae[ZEPHYRUM]  = res_probationis[I];
    obscurae[I]         = res_probationis[I];
    e.res               = obscurae;
    e.numerus           = II;
    e.electa            = -I;
    electa              = -I;
    ev                  = clavis_ex(CLAVIS_DEORSUM, ZEPHYRUM);
    x                   = elenchus_clavem_agere(&e, &ev, &electa);
    CREDO_VERUM(x == ELENCHUS_NIHIL);
    CREDO_AEQUALIS_S32(electa, -I);
}

/* figura: umbra, corpus, margo, electa, textus (obscura obscurus),
 * clavis ad dextrum, divisor */
interior vacuum
_probare_figuram (
    CredoContextus* c)
{
               Elenchus  e;
    InternamentumChorda* intern;
              Componens* radix;
                Mandata* md;
      constans Mandatum* t;
                  Fines  r;

    e         = elenchus_probationis();
    e.electa  = II;
    intern    = internamentum_creare(c->piscina);
    radix     = elenchus_componere(&e, fines_ex(X, XX, XCII, LXII), "a",
        c->piscina, intern);
    CREDO_NECESSE_NON_NIHIL(radix);
    md = mandata_creare(c->piscina, intern);
    elenchus_figura(radix, md, ZEPHYRUM, &e);
    /* spatium proprium: umbra II infra-dextrorsum, corpus XC x LX */
    CREDO_VERUM(rectangulum_adest(md, fines_ex(II, II, XC, LX),
        COLOR_TEXT, VERUM));
    CREDO_VERUM(rectangulum_adest(md, fines_ex(ZEPHYRUM, ZEPHYRUM, XC,
        LX), COLOR_BACKGROUND, VERUM));
    CREDO_VERUM(rectangulum_adest(md, fines_ex(ZEPHYRUM, ZEPHYRUM, XC,
        LX), COLOR_TEXT, FALSUM));
    /* electa (II): fundus selectionis in finibus rei */
    r = componens_liberum(radix, II)->fines;
    CREDO_VERUM(rectangulum_adest(md, r, COLOR_SELECTION, VERUM));
    r = componens_liberum(radix, ZEPHYRUM)->fines;
    CREDO_FALSUM(rectangulum_adest(md, r, COLOR_SELECTION, VERUM));
    /* textus ad marginem VI, IV infra summum rei */
    t = textus_mandatum(md, "Open");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_S32(t->fines.x, VI);
    CREDO_AEQUALIS_S32(t->fines.y, II + IV);
    CREDO_VERUM(t->color.valor == (i32)COLOR_TEXT);
    t = textus_mandatum(md, "Rename");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_VERUM(t->color.valor == (i32)COLOR_TEXT_DIM);
    /* clavis ad dextrum: finis = latitudo corporis minus VI */
    t = textus_mandatum(md, "Enter");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_S32(t->fines.x + V * VI, XC - VI);
    t = textus_mandatum(md, "X");
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_S32(t->fines.x + VI, XC - VI);
    /* divisor intra fasciam suam (II + XVI .. + VIII) */
    CREDO_VERUM(linea_horizontalis_inter(md, II + XVI, II + XVI
        + VIII));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fines", _probare_fines, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arbor", _probare_arborem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "index", _probare_indicem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "figura", _probare_figuram, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("elenchus", SECTIONES);
}
