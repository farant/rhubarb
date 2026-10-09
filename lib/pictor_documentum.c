/* pictor_documentum.c - pars pictoris documenti (pixela, ictus);
 * cauda per historia */

#include "pictor_documentum.h"
#include "sors.h"
#include "delineare_mandata.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"
#include "stml.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior memoriae_index
mensura_pixelorum (
    constans PictorDocumentum* doc)
{
    redde (memoriae_index)doc->latitudo * (memoriae_index)doc->altitudo
         * magnitudo(i32);
}

interior s32
attributum_s32 (
             StmlNodus* n,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

/* vacatio: color fundi thematis (Franus 2026-10-09: ut scriba et
 * terminale; olim PALETTE_WHITE, in themate flavum clarum) - fundus
 * non in actis: picturae omnes novo fundo reddi */
interior vacuum
vacare_fundo (
    PictorDocumentum* doc)
{
    /* color semanticus ipse (thema_color), non index: colores ictuum
     * palettam COLORATIONIS legunt, semantici palettam aliam - index
     * fundi in coloratione colorem atramenti dabat */
    tabula_pixelorum_vacare(doc->tabula, color_ad_pixelum(
        thema_color(COLOR_BACKGROUND)));
}


/* ==================================================
 * Applicatio actorum
 * ================================================== */

/* segmentum ad tabulam [0, latitudo) x [0, altitudo) praecidere
 * (Cohen-Sutherland, s64 - nihil revolvitur); FALSUM si totum extra.
 * Coordinatae negativae ut i32 insignatum ingentes fiebant: tractus
 * extra marginem ad oppositum saliebat (Franus 2026-10-08). */
interior b32
segmentum_praecidere (
     s64* x0,
     s64* y0,
     s64* x1,
     s64* y1,
     s64  latitudo,
     s64  altitudo)
{
    s64 x_ultimum;
    s64 y_ultimum;
    s64 x;
    s64 y;
    i32 c0;
    i32 c1;
    i32 c;

    x_ultimum = latitudo - I;
    y_ultimum = altitudo - I;
    si (x_ultimum < ZEPHYRUM || y_ultimum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (VERUM)
    {
        c0 = (*x0 < ZEPHYRUM ? I : ZEPHYRUM) | (*x0
            > x_ultimum ? II : ZEPHYRUM)
            | (*y0 < ZEPHYRUM ? IV : ZEPHYRUM) | (*y0
               > y_ultimum ? VIII : ZEPHYRUM);
        c1 = (*x1 < ZEPHYRUM ? I : ZEPHYRUM) | (*x1
            > x_ultimum ? II : ZEPHYRUM)
            | (*y1 < ZEPHYRUM ? IV : ZEPHYRUM) | (*y1
               > y_ultimum ? VIII : ZEPHYRUM);
        si (!(c0 | c1))
        {
            redde VERUM;
        }
        si (c0 & c1)
        {
            redde FALSUM;
        }
        c = c0 ? c0 : c1;
        si (c & IV)
        {
            x = *x0 + (*x1 - *x0) * (ZEPHYRUM - *y0) / (*y1 - *y0);
            y = ZEPHYRUM;
        }
        alioquin si (c & VIII)
        {
            x = *x0 + (*x1 - *x0) * (y_ultimum - *y0) / (*y1 - *y0);
            y = y_ultimum;
        }
        alioquin si (c & II)
        {
            y = *y0 + (*y1 - *y0) * (x_ultimum - *x0) / (*x1 - *x0);
            x = x_ultimum;
        }
        alioquin
        {
            y = *y0 + (*y1 - *y0) * (ZEPHYRUM - *x0) / (*x1 - *x0);
            x = ZEPHYRUM;
        }
        si (c == c0)
        {
            *x0 = x;
            *y0 = y;
        }
        alioquin
        {
            *x1 = x;
            *y1 = y;
        }
    }
}

/* gutta: sors ex (semen, puncto, gutta) - rivus derivatus, nulla
 * dependentia ordinis; rejectio in disco (XVI tentamina; deinde
 * centrum) */
vacuum
pictor_gutta (
    s64  semen,
    i32  i,
    i32  k,
    s32  radius,
    s32* dx,
    s32* dy)
{
    Sors s;
     s32 x;
     s32 y;
     i32 n;

    *dx = ZEPHYRUM;
    *dy = ZEPHYRUM;
    si (radius < I)
    {
        redde;
    }
    sors_seminare(&s, (i64)semen, (i64)i * (i64)M + (i64)k);
    per (n = ZEPHYRUM; n < XVI; n++)
    {
        x = sors_inter(&s, -radius, radius);
        y = sors_inter(&s, -radius, radius);
        si (x * x + y * y <= radius * radius)
        {
            *dx = x;
            *dy = y;
            redde;
        }
    }
}

/* aspergillum: guttae circa quodque punctum (vide caput) */
interior vacuum
guttas_applicare (
    PictorDocumentum* doc,
           StmlNodus* ictus,
               Color  color,
                 s32  magnitudo_penicilli)
{
    StmlNodus* punctum;
          s64  semen;
          s32  radius;
          s32  x;
          s32  y;
          s32  t;
          s32  t_ante;
          s32  dx;
          s32  dy;
          s32  numerus;
          i32  i;
          i32  k;
          i32  n;
          i32  ordo;

    semen   = (s64)attributum_s32(ictus, "semen", ZEPHYRUM);
    radius  = PICTOR_ASPERGILLI_RADIUS * magnitudo_penicilli;
    n       = stml_numerus_liberorum(ictus);
    t_ante  = ZEPHYRUM;
    ordo    = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        punctum = stml_liberum_ad_indicem(ictus, i);
        si (punctum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        x        = attributum_s32(punctum, "x", ZEPHYRUM);
        y        = attributum_s32(punctum, "y", ZEPHYRUM);
        t        = attributum_s32(punctum, "t", t_ante);
        numerus  = PICTOR_GUTTAE_PUNCTO;
        si (ordo > ZEPHYRUM && t > t_ante)
        {
            numerus += (t - t_ante) / PICTOR_GUTTA_MS;
        }
        per (k = ZEPHYRUM; k < (i32)numerus; k++)
        {
            pictor_gutta(semen, ordo, k, radius, &dx, &dy);
            si (   x + dx >= ZEPHYRUM && y + dy >= ZEPHYRUM
                && x + dx < (s32)doc->tabula->latitudo
                && y + dy < (s32)doc->tabula->altitudo)
            {
                tabula_pixelorum_ponere_pixelum(doc->tabula,
                    (i32)(x + dx), (i32)(y + dy),
                    color_ad_pixelum(color));
            }
        }
        t_ante = t;
        ordo++;
    }
}

/* <ictus instrumentum color magnitudo><punctum x y/>...</ictus> */
interior vacuum
ictum_applicare (
    PictorDocumentum* doc,
           StmlNodus* ictus)
{
    ContextusDelineandi* ctx;
                  Color  color;
                    s32  magnitudo_penicilli;
                    s32  x;
                    s32  y;
                    s32  x_ante;
                    s32  y_ante;
                    i32  i;
                    i32  n;
              StmlNodus* punctum;
                    s64  x0;
                    s64  y0;
                    s64  x1;
                    s64  y1;
                    s64  sinistra;
                    s64  summa;
                    s64  dextra;
                    s64  infima;
                    s64  lat;
                    s64  alt;

    ctx = delineare_creare_contextum(doc->piscina, doc->tabula);
    si (!ctx)
    {
        redde;
    }
    color = thema_color_ex_indice_colorationis(
        (i8)attributum_s32(ictus, "color", (s32)PALETTE_BLACK));
    magnitudo_penicilli = attributum_s32(ictus, "magnitudo", I);
    si (magnitudo_penicilli < I)
    {
        magnitudo_penicilli = I;
    }
    si (   stml_attributum_capere(ictus, "instrumentum")
        && chorda_aequalis_literis(*stml_attributum_capere(ictus,
               "instrumentum"), "aspergillum"))
    {
        guttas_applicare(doc, ictus, color, magnitudo_penicilli);
        delineare_restituere_contextum(ctx);
        redde;
    }
    lat     = (s64)doc->tabula->latitudo;
    alt     = (s64)doc->tabula->altitudo;
    n       = stml_numerus_liberorum(ictus);
    x_ante  = ZEPHYRUM;
    y_ante  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        punctum = stml_liberum_ad_indicem(ictus, i);
        si (punctum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        x = attributum_s32(punctum, "x", ZEPHYRUM);
        y = attributum_s32(punctum, "y", ZEPHYRUM);
        si (i > ZEPHYRUM)
        {
            x0 = (s64)x_ante;
            y0 = (s64)y_ante;
            x1 = (s64)x;
            y1 = (s64)y;
            si (segmentum_praecidere(&x0, &y0, &x1, &y1, lat, alt))
            {
                delineare_lineam(ctx, (i32)x0, (i32)y0, (i32)x1,
                    (i32)y1, color);
            }
        }
        /* punctum penicilli: rectangulum ad tabulam praecisum */
        sinistra  = (s64)x - magnitudo_penicilli / II;
        summa     = (s64)y - magnitudo_penicilli / II;
        dextra    = sinistra + magnitudo_penicilli;
        infima    = summa + magnitudo_penicilli;
        sinistra  = sinistra < ZEPHYRUM ? ZEPHYRUM : sinistra;
        summa     = summa < ZEPHYRUM ? ZEPHYRUM : summa;
        dextra    = dextra > lat ? lat : dextra;
        infima    = infima > alt ? alt : infima;
        si (dextra > sinistra && infima > summa)
        {
            delineare_rectangulum_plenum(ctx, (i32)sinistra, (i32)summa,
                (i32)(dextra - sinistra), (i32)(infima - summa), color);
        }
        x_ante = x;
        y_ante = y;
    }
    delineare_restituere_contextum(ctx);
}

interior vacuum
actum_applicare (
    PictorDocumentum* doc,
              chorda  datum)
{
    StmlResultus res;

    res = stml_legere_ex_literis(chorda_ut_cstr(datum, doc->piscina),
                                 doc->piscina, doc->intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde;
    }
    si (chorda_aequalis_literis(*res.elementum_radix->titulus, "ictus"))
    {
        ictum_applicare(doc, res.elementum_radix);
    }
    /* ramus: nihil pingit; cetera v1 ignorata (worklog) */
}

/* proiectio pro historia: memoria = pixela tabulae */
interior vacuum
proiectio_vacare (
    vacuum* ctx)
{
    vacare_fundo((PictorDocumentum*)ctx);
}

interior vacuum
proiectio_applicare (
    vacuum* ctx,
    chorda  actum)
{
    actum_applicare((PictorDocumentum*)ctx, actum);
}

interior HistoriaProiectio
proiectio_facere (
    PictorDocumentum* doc)
{
    HistoriaProiectio p;

    p.memoria    = (i8*)doc->tabula->pixela;
    p.mensura    = mensura_pixelorum(doc);
    p.vacare     = proiectio_vacare;
    p.applicare  = proiectio_applicare;
    p.ctx        = doc;
    redde p;
}


/* ==================================================
 * Vita
 * ================================================== */

/* clavis manifesti: "spatium/documentum" (spatium vacuum: nuda) */
interior chorda
clavis_manifesti (
               Piscina* piscina,
    constans character* spatium)
{
    chorda c;

    si (!spatium || !spatium[ZEPHYRUM])
    {
        redde chorda_ex_literis("documentum", piscina);
    }
    c = chorda_concatenare(chorda_ex_literis(spatium, piscina),
                           chorda_ex_literis("/documentum", piscina),
                           piscina);
    redde c;
}

interior PictorDocumentum*
documentum_struere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;

    doc = (PictorDocumentum*)piscina_allocare(piscina,
                                              magnitudo(*doc));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PictorDocumentum));
    doc->volumen      = volumen;
    doc->piscina      = piscina;
    doc->intern       = intern;
    doc->latitudo     = latitudo;
    doc->altitudo     = altitudo;
    doc->intervallum  = intervallum > ZEPHYRUM ? intervallum : LXIV;
    doc->tabula = tabula_pixelorum_creare_nuda(piscina, latitudo,
        altitudo);
    si (!doc->tabula)
    {
        redde NIHIL;
    }
    doc->proiectio = imago_ex_tabula(doc->tabula);
    redde doc;
}

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;
              chorda  manifestum;

    si (   !piscina || !intern || !volumen || latitudo <= ZEPHYRUM
        || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    doc = documentum_struere(piscina, intern, volumen, latitudo,
        altitudo,
                             intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    manifestum = chorda_ex_literis("<documentum latitudo=\"", piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)latitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)altitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" intervallum=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)doc->intervallum, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\"/>", piscina), piscina);
    volumen_plagulam_condere(volumen, clavis_manifesti(piscina,
        spatium),
                             manifestum, "pictor:documentum");
    doc->historia = historia_creare(piscina, intern, volumen, spatium,
        "ictus",
                                    "pictor:checkpoint",
                                    doc->intervallum,
                                    proiectio_facere(doc));
    si (!doc->historia)
    {
        redde NIHIL;
    }
    redde doc;
}

PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium)
{
    PictorDocumentum* doc;
              chorda  manifestum;
                 b32  inventum;
        StmlResultus  res;
                 s32  latitudo;
                 s32  altitudo;
                 s32  intervallum;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    manifestum = volumen_plagulam_promere(volumen,
        clavis_manifesti(piscina, spatium), piscina, &inventum);
    si (!inventum)
    {
        redde NIHIL;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(manifestum, piscina),
                                 piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }
    latitudo    = attributum_s32(res.elementum_radix, "latitudo",
        ZEPHYRUM);
    altitudo    = attributum_s32(res.elementum_radix, "altitudo",
        ZEPHYRUM);
    intervallum = attributum_s32(res.elementum_radix, "intervallum",
        LXIV);
    doc = documentum_struere(piscina, intern, volumen, (i32)latitudo,
                             (i32)altitudo, (i32)intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    doc->historia = historia_aperire(piscina, intern, volumen, spatium,
        "ictus",
                                     "pictor:checkpoint",
                                     doc->intervallum,
                                     proiectio_facere(doc));
    si (!doc->historia)
    {
        redde NIHIL;
    }
    redde doc;
}


/* ==================================================
 * Acta, revocare, reficere (historia)
 * ================================================== */

s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml)
{
    redde doc ? historia_actum(doc->historia, actum_stml) : ZEPHYRUM;
}

b32
pictor_documentum_revocare (
    PictorDocumentum* doc)
{
    redde doc ? historia_revocare(doc->historia) : FALSUM;
}

b32
pictor_documentum_reficere (
    PictorDocumentum* doc)
{
    redde doc ? historia_reficere(doc->historia) : FALSUM;
}


/* ==================================================
 * Lectio et verificatio
 * ================================================== */

constans Imago*
pictor_documentum_proiectio (
    constans PictorDocumentum* doc)
{
    redde doc ? &doc->proiectio : NIHIL;
}

chorda
pictor_documentum_sigillum_hex (
    constans PictorDocumentum* doc,
                      Piscina* piscina)
{
    chorda vacua;

    si (!doc)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde historia_sigillum_hex(doc->historia, piscina);
}

b32
pictor_documentum_verificare (
    PictorDocumentum* doc)
{
    redde doc ? historia_verificare(doc->historia) : FALSUM;
}

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_cursor(doc->historia) : ZEPHYRUM;
}

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_finis(doc->historia) : ZEPHYRUM;
}

i32
pictor_documentum_numerus_vivorum (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_numerus_vivorum(doc->historia) : ZEPHYRUM;
}
