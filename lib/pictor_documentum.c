/* pictor_documentum.c - pars pictoris documenti (pixela, ictus);
 * cauda per historia */

#include "pictor_documentum.h"
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

interior vacuum
vacare_albam (
    PictorDocumentum* doc)
{
    tabula_pixelorum_vacare(doc->tabula, color_ad_pixelum(
        thema_color_ex_indice_colorationis((i8)PALETTE_WHITE)));
}


/* ==================================================
 * Applicatio actorum
 * ================================================== */

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
            delineare_lineam(ctx, (i32)x_ante, (i32)y_ante, (i32)x,
                (i32)y,
                             color);
        }
        delineare_rectangulum_plenum(ctx,
            (i32)(x - magnitudo_penicilli / II),
            (i32)(y - magnitudo_penicilli / II),
            (i32)magnitudo_penicilli, (i32)magnitudo_penicilli, color);
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
    vacare_albam((PictorDocumentum*)ctx);
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
