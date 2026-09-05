/* pictor_documentum.c - acta -> proiectio */

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

/* nullum chorda_ex_s64 in domo: per f64 sine decimalibus (exactum
 * infra 2^53), ut eventus_stml tempus scribit */
interior chorda
seq_chorda (
        s64  seq,
    Piscina* piscina)
{
    redde chorda_ex_f64((f64)seq, ZEPHYRUM, piscina);
}

interior vacuum
sigillum_renovare (
    PictorDocumentum* doc)
{
    doc->sigillum = sigillum_computare(doc->tabula->pixela,
                                       mensura_pixelorum(doc));
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

/* acta viva usque ad 'ad' (inclusive), rami honorati: reddit Xar de
 * VolumenActum ordine seq, mortuis exclusis */
/* acta viva usque ad 'ad' (inclusive): ICTUS soli (volumen sua acta
 * interserit - volumen-creatum, plagula-condita - quae hic nihil
 * sunt), rami honorati: reddit Xar de VolumenActum ordine seq */
interior Xar*
acta_viva (
    PictorDocumentum* doc,
                 s64  post,
                 s64  ad)
{
             Xar* omnia;
             Xar* viva;
    VolumenActum* a;
    VolumenActum* sedes;
             i32  i;
             i32  n;
             i32  k;
             s64  ab;
    StmlResultus  res;

    omnia  = volumen_acta_legere(doc->volumen, post, doc->piscina);
    viva   = xar_creare(doc->piscina, (i32)magnitudo(VolumenActum));
    n      = xar_numerus(omnia);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(omnia, i);
        si (a->seq > ad)
        {
            frange;
        }
        si (chorda_aequalis_literis(a->genus, "ramus"))
        {
            res = stml_legere_ex_literis(chorda_ut_cstr(a->datum,
                doc->piscina), doc->piscina, doc->intern);
            ab = res.successus
                ? (s64)attributum_s32(res.elementum_radix, "ab",
                ZEPHYRUM)
                : ZEPHYRUM;
            /* tollere viva cum seq > ab */
            k = xar_numerus(viva);
            dum (   k > ZEPHYRUM
                 && ((VolumenActum*)xar_obtinere(viva, k - I))->seq
                      > ab)
            {
                k--;
            }
            xar_truncare(viva, k);
            perge;
        }
        si (!chorda_aequalis_literis(a->genus, "ictus"))
        {
            perge;
        }
        sedes   = (VolumenActum*)xar_addere(viva);
        *sedes  = *a;
    }
    redde viva;
}

/* proiectio ad seq 'ad': checkpoint proximus <= ad, deinde acta viva
 * post eum; sine checkpoint ex alba */
/* seq in actis vivis usque ad 'ad'? */
interior b32
seq_vivum (
    constans Xar* viva,
             s64  seq)
{
    i32 i;
    i32 n;

    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (((VolumenActum*)xar_obtinere(viva, i))->seq == seq)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* checkpoint proximus: plagula 'checkpoint/<seq>' cum seq maximo
 * <= ad ET vivo; redde seq (0 = nullus). Plagulae enumerantur -
 * seqs multipla intervalli non sunt (volumen acta sua interserit). */
interior s64
checkpoint_proximus (
    PictorDocumentum* doc,
                 s64  ad,
        constans Xar* viva)
{
               Xar* plagulae;
    VolumenPlagula* pl;
            chorda  praefixum;
            chorda  cauda;
               s32  s;
               s64  optimum;
               i32  i;
               i32  n;

    praefixum  = chorda_ex_literis("checkpoint/", doc->piscina);
    plagulae   = volumen_plagulas_enumerare(doc->volumen, doc->piscina);
    optimum    = ZEPHYRUM;
    n          = xar_numerus(plagulae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        pl = (VolumenPlagula*)xar_obtinere(plagulae, i);
        si (!chorda_incipit(pl->via, praefixum))
        {
            perge;
        }
        cauda.datum    = pl->via.datum + praefixum.mensura;
        cauda.mensura  = pl->via.mensura - praefixum.mensura;
        si (!chorda_ut_s32(cauda, &s))
        {
            perge;
        }
        si ((s64)s <= ad && (s64)s > optimum && seq_vivum(viva, (s64)s))
        {
            optimum = (s64)s;
        }
    }
    redde optimum;
}

/* proiectio ad seq 'ad': checkpoint proximus vivus <= ad, deinde
 * acta viva post eum; sine checkpoint ex alba */
interior vacuum
proicere_ad (
    PictorDocumentum* doc,
                 s64  ad,
                 b32  sine_checkpoint)
{
             Xar* viva;
    VolumenActum* a;
             i32  i;
             i32  n;
             s64  basis;
          chorda  hex;
          chorda  massa;
             b32  inventum;
          chorda  clavis;

    viva   = acta_viva(doc, ZEPHYRUM, ad);
    basis  = ZEPHYRUM;
    si (!sine_checkpoint)
    {
        basis = checkpoint_proximus(doc, ad, viva);
        si (basis > ZEPHYRUM)
        {
            clavis = chorda_concatenare(
                chorda_ex_literis("checkpoint/", doc->piscina),
                seq_chorda(basis, doc->piscina), doc->piscina);
            hex = volumen_plagulam_promere(doc->volumen, clavis,
                                           doc->piscina, &inventum);
            massa = inventum
                  ? volumen_massam_promere(doc->volumen, hex,
                  doc->piscina,
                                           &inventum)
                  : hex;
            si (   inventum
                && massa.mensura == (i32)mensura_pixelorum(doc))
            {
                memcpy(doc->tabula->pixela, massa.datum,
                       mensura_pixelorum(doc));
            }
            alioquin
            {
                basis = ZEPHYRUM;
            }
        }
    }
    si (basis == ZEPHYRUM)
    {
        vacare_albam(doc);
    }
    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        si (a->seq > basis)
        {
            actum_applicare(doc, a->datum);
        }
    }
    sigillum_renovare(doc);
}

interior vacuum
checkpoint_condere (
    PictorDocumentum* doc,
                 s64  seq)
{
    character hex[SIGILLUM_HEX_MENSURA];
       chorda contentum;
       chorda clavis;

    contentum.datum    = (i8*)doc->tabula->pixela;
    contentum.mensura  = (i32)mensura_pixelorum(doc);
    si (!volumen_massam_condere(doc->volumen, contentum, hex))
    {
        redde;
    }
    clavis = chorda_concatenare(chorda_ex_literis("checkpoint/",
        doc->piscina),
                                seq_chorda(seq, doc->piscina),
                                doc->piscina);
    volumen_plagulam_condere(doc->volumen, clavis,
                             chorda_ex_literis(hex, doc->piscina),
                             "pictor:checkpoint");
}


/* ==================================================
 * Vita
 * ================================================== */

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
    volumen_plagulam_condere(volumen, chorda_ex_literis("documentum",
        piscina),
                             manifestum, "pictor:documentum");
    vacare_albam(doc);
    sigillum_renovare(doc);
    redde doc;
}

PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    PictorDocumentum* doc;
              chorda  manifestum;
                 b32  inventum;
        StmlResultus  res;
                 s32  latitudo;
                 s32  altitudo;
                 s32  intervallum;
                 Xar* viva;
                 i32  n;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    manifestum = volumen_plagulam_promere(volumen,
        chorda_ex_literis("documentum", piscina), piscina, &inventum);
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
    /* finis = seq ultimum vivum; cursor = finis */
    viva  = acta_viva(doc, ZEPHYRUM, volumen_summa_actorum(volumen));
    n     = xar_numerus(viva);
        doc->finis  = n > ZEPHYRUM
                ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
                : ZEPHYRUM;
    doc->cursor           = doc->finis;
    doc->numerus_vivorum  = n;
    proicere_ad(doc, doc->cursor, FALSUM);
    redde doc;
}


/* ==================================================
 * Acta, revocare, reficere
 * ================================================== */

s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml)
{
       s64 seq;
    chorda ramus;

    si (!doc || chorda_vacua(actum_stml))
    {
        redde ZEPHYRUM;
    }
        si (doc->cursor < doc->finis)
        {
        doc->numerus_vivorum =
            xar_numerus(acta_viva(doc, ZEPHYRUM, doc->cursor));
        ramus = chorda_ex_literis("<ramus ab=\"", doc->piscina);
        ramus = chorda_concatenare(ramus,
                                   seq_chorda(doc->cursor,
                                   doc->piscina),
                                   doc->piscina);
        ramus = chorda_concatenare(ramus,
                                   chorda_ex_literis("\"/>",
                                   doc->piscina),
                                   doc->piscina);
        volumen_actum_appendere(doc->volumen, "ramus", ramus);
        }
    seq = volumen_actum_appendere(doc->volumen, "ictus", actum_stml);
    si (seq <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
        actum_applicare(doc, actum_stml);
    doc->cursor  = seq;
    doc->finis   = seq;
    doc->numerus_vivorum++;
    sigillum_renovare(doc);
    si (doc->numerus_vivorum % doc->intervallum == ZEPHYRUM)
    {
        checkpoint_condere(doc, seq);
    }
    redde seq;
}

b32
pictor_documentum_revocare (
    PictorDocumentum* doc)
{
    Xar* viva;
    i32  n;
    s64  ad;

    si (!doc || doc->cursor <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* actum vivum proximum infra cursor */
    viva  = acta_viva(doc, ZEPHYRUM, doc->cursor - I);
    n     = xar_numerus(viva);
    ad = n > ZEPHYRUM ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
                      : ZEPHYRUM;
        doc->cursor = ad;
    doc->numerus_vivorum--;
    proicere_ad(doc, ad, FALSUM);
    redde VERUM;
}

b32
pictor_documentum_reficere (
    PictorDocumentum* doc)
{
             Xar* viva;
    VolumenActum* a;
             i32  i;
             i32  n;

    si (!doc || doc->cursor >= doc->finis)
    {
        redde FALSUM;
    }
    viva  = acta_viva(doc, doc->cursor, doc->finis);
    n     = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        si (a->seq > doc->cursor)
        {
                        actum_applicare(doc, a->datum);
            doc->cursor = a->seq;
            doc->numerus_vivorum++;
            sigillum_renovare(doc);
            redde VERUM;
        }
    }
    redde FALSUM;
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
    character hex[SIGILLUM_HEX_MENSURA];
       chorda vacua;

    si (!doc)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    sigillum_hex(&doc->sigillum, hex);
    redde chorda_ex_literis(hex, piscina);
}

b32
pictor_documentum_verificare (
    PictorDocumentum* doc)
{
    Sigillum ante;

    si (!doc)
    {
        redde FALSUM;
    }
    ante = doc->sigillum;
    proicere_ad(doc, doc->cursor, VERUM);
    redde sigillum_aequale(&ante, &doc->sigillum);
}

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc)
{
    redde doc ? doc->cursor : ZEPHYRUM;
}

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc)
{
    redde doc ? doc->finis : ZEPHYRUM;
}
