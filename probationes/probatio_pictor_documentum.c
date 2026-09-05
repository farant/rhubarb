/* probatio_pictor_documentum.c - acta -> proiectio -> sigillum
 * (gradus VIII); checkpoints; revocare/reficere; ramus */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "sigillum.h"
#include "fenestra.h"
#include "pictor_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ictus unus: linea horizontalis (x0..x1, y) magnitudine I */
interior chorda
ictus (
    Piscina* p,
        s32  x0,
        s32  x1,
        s32  y,
        i32  color)
{
    chorda s;

    s = chorda_ex_literis("<ictus instrumentum=\"penicillus\" color=\"",
                          p);
    s = chorda_concatenare(s, chorda_ex_s32((s32)color, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"1\">"
                                                "<punctum x=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(x0, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\"/><punctum x=\"", p),
                           p);
    s = chorda_concatenare(s, chorda_ex_s32(x1, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\"/></ictus>", p), p);
    redde s;
}

interior i32
pixelum (
    constans Imago* im,
               s32  x,
               s32  y)
{
    constans i8* p;

    p = im->pixela + (y * (s32)im->latitudo + x) * IV;
    redde color_ad_pixelum(color_ex_rgba(p[ZEPHYRUM], p[I], p[II],
        p[III]));
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
       PictorDocumentum* doc2;
                 chorda  s1;
                 chorda  s2;
                 chorda  s3;
                 chorda  clavis;
                 chorda  hex;
                    b32  inventum;
                    s64  q1;
                    s64  q2;
                    s64  q3;
                    s64  q4;
                    s64  massae_ante;
                    i32  albus;
                    i32  niger;
                    i32  i;

    piscina = piscina_generare_dynamicum("probatio_pictor_documentum",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    albus = color_ad_pixelum(thema_color_ex_indice_colorationis(
        (i8)PALETTE_WHITE));
    niger = color_ad_pixelum(thema_color_ex_indice_colorationis(
        (i8)PALETTE_BLACK));

    imprimere("\n--- Creatio: proiectio alba, cursor 0 ---\n");
    vol = volumen_temporarium(piscina, "probatio_pictor_documentum");
    CREDO_NON_NIHIL(vol);
    doc = pictor_documentum_creare(piscina, intern, vol, XXXII, XVI,
        II);
    CREDO_NON_NIHIL(doc);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), V, V),
                       albus);
    s1 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_AEQUALIS_I32(s1.mensura, LXIV);

    imprimere("\n--- Tres ictus: seqs voluminis, checkpoint II ---\n");
    massae_ante = volumen_summa_massarum(vol);
    q1 = pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, II, PALETTE_BLACK));
    q2 = pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, V, PALETTE_BLACK));
    q3 = pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, VIII, PALETTE_BLACK));
    CREDO_VERUM(q1 > ZEPHYRUM && q2 > q1 && q3 > q2);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), q3);
    CREDO_AEQUALIS_S64(pictor_documentum_finis(doc), q3);
    CREDO_AEQUALIS_I32(doc->numerus_vivorum, III);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, II),
                       niger);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X,
        VIII),
                       niger);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X,
        III),
                       albus);
    s2 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_FALSUM(chorda_aequalis(s1, s2));
    /* checkpoint post ictum II (intervallum II): plagula + massa */
    clavis = chorda_concatenare(chorda_ex_literis("checkpoint/",
        piscina),
        chorda_ex_f64((f64)q2, ZEPHYRUM, piscina), piscina);
    hex = volumen_plagulam_promere(vol, clavis, piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_AEQUALIS_I32(hex.mensura, LXIV);
    CREDO_VERUM(volumen_summa_massarum(vol) > massae_ante);

    imprimere("\n--- Gradus VIII: documentum se ipsum verificat ---\n");
    CREDO_VERUM(pictor_documentum_verificare(doc));

    imprimere("\n--- Revocare: ad q2 ex checkpoint, ad q1, ad 0 ---\n");
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), q2);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X,
        VIII),
                       albus);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, V),
                       niger);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), q1);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), ZEPHYRUM);
    /* nihil ultra */
    CREDO_FALSUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, II),
                       albus);

    imprimere("\n--- Reficere: ad q3, sigillum idem ---\n");
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), q3);
    CREDO_FALSUM(pictor_documentum_reficere(doc));
    s3 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_VERUM(chorda_aequalis(s2, s3));

    imprimere("\n--- Ramus: ictus novus post revocare ---\n");
    CREDO_VERUM(pictor_documentum_revocare(doc));
    q4 = pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, XII, PALETTE_BLACK));
    CREDO_VERUM(q4 > q3);
    CREDO_FALSUM(pictor_documentum_reficere(doc));
    CREDO_AEQUALIS_I32(doc->numerus_vivorum, III);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X,
        VIII),
                       albus);   /* mortuus */
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X,
        XII),
                       niger);
    CREDO_VERUM(pictor_documentum_verificare(doc));

    imprimere("\n--- Aperire idem volumen: proiectio eadem ---\n");
    doc2 = pictor_documentum_aperire(piscina, intern, vol);
    CREDO_NON_NIHIL(doc2);
    CREDO_AEQUALIS_I32(doc2->latitudo, XXXII);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc2), q4);
    CREDO_AEQUALIS_I32(doc2->numerus_vivorum, III);
    s1 = pictor_documentum_sigillum_hex(doc, piscina);
    s2 = pictor_documentum_sigillum_hex(doc2, piscina);
    CREDO_VERUM(chorda_aequalis(s1, s2));

    imprimere("\n--- Mensura: CC ictus, tempus per actum ---\n");
    {
        s64 t0;
        s64 t1;
        t0 = fenestra_tempus_ms();
        per (i = ZEPHYRUM; i < CC; i++)
        {
            pictor_documentum_actum(doc,
                ictus(piscina, ZEPHYRUM, XXXI, (s32)(i % XVI),
                PALETTE_BLACK));
        }
        t1 = fenestra_tempus_ms();
        imprimere("  CC acta: %ld ms totales\n", (long)(t1 - t0));
        t0 = fenestra_tempus_ms();
        pictor_documentum_revocare(doc);
        t1 = fenestra_tempus_ms();
        imprimere("  revocare unum: %ld ms\n", (long)(t1 - t0));
        CREDO_VERUM(t1 >= t0);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
