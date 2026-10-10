/* probatio_pictor_bibliotheca.c - bibliotheca picturarum (S3e-2)
 *
 * Applicatio pictoris vera (radix, spatium ""), volumen temporarium;
 * pictura altera "b" (C x LX) documento directo; plagula
 * "c/documentum" originis alienae (scriba: dimensiones quoque habet,
 * aperiri posset). I: pictor_documenta_enumerare - "b" et "" (ordine
 * viae), "c" non. II: pictor_picturam_ponere "b" - documentum, contextus
 * actionum et figurarum, dimensiones durabiles, tabula composita. III:
 * ictus in "b" scribitur, radix intacta. IV: memoria - redire et
 * iterum: documenta eadem. V: spatium sine pictura: FALSUM, nihil
 * mutatum. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior chorda
durabile (
    PictorApplicatio* app,
  constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, INSULA_DURABILIS, titulus);
    redde a ? *a : chorda_ex_literis("", piscina);
}

interior Componens*
nodus_arboris (
    PictorApplicatio* app,
  constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* radix;
     PictorDocumentum* b;
     PictorDocumentum* b_primum;
                  i32  vivi_radicis;
                  i32  vivi_b;

    piscina = piscina_generare_dynamicum("probatio_pictor_bibliotheca",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_bibliotheca");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m      = manus_ludus_creare(piscina, app.d);
    radix  = app.montatio.doc;
    b      = pictor_documentum_creare(piscina, intern, vol, "b", C, LX,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(b);
    (vacuum)pictor_documentum_actum(b, chorda_ex_literis(
        "<ictus instrumentum=\"penicillus\" color=\"0\""
        " magnitudo=\"1\"><punctum x=\"1\" y=\"1\"/></ictus>",
        piscina));
    volumen_plagulam_condere(vol, chorda_ex_literis("c/documentum",
        piscina), chorda_ex_literis(
        "<documentum latitudo=\"40\" altitudo=\"20\"/>", piscina),
        "scriba:documentum");

    imprimere("\n--- I: bibliotheca ---\n");
    {
        Xar* l;

        l = pictor_documenta_enumerare(vol, piscina);
        CREDO_NECESSE_NON_NIHIL(l);
        CREDO_AEQUALIS_I32(xar_numerus(l), II);
        si (xar_numerus(l) == II)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l,
                ZEPHYRUM), "b");
            CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, I),
                "");
        }
    }

    imprimere("\n--- II: pictura 'b' ostensa ---\n");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, "b"));
    b_primum = app.montatio.doc;
    CREDO_VERUM(b_primum != NIHIL && b_primum != radix);
    CREDO_VERUM(b_primum && b_primum->latitudo == C
        && b_primum->altitudo == LX);
    CREDO_VERUM(app.montatio.actiones_ctx.doc == b_primum);
    CREDO_VERUM(app.montatio.figurae_ctx.doc == b_primum);
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "latitudo"), "100");
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "altitudo"), "60");
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_S32(nodus_arboris(&app, "tabula")->fines.latitudo,
        C);
    CREDO_AEQUALIS_S32(nodus_arboris(&app, "tabula")->fines.altitudo,
        LX);

    imprimere("\n--- III: ictus in 'b' ---\n");
    vivi_radicis  = pictor_documentum_numerus_vivorum(radix);
    vivi_b        = pictor_documentum_numerus_vivorum(b_primum);
    CREDO_AEQUALIS_I32(vivi_b, I);
    /* tabula ad cellulam (VI, VIII) */
    CREDO_VERUM(manus_ludus_premere_ad(m, VI + X, VIII + X));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b_primum),
        vivi_b + I);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(radix),
        vivi_radicis);

    imprimere("\n--- IV: memoria ---\n");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, ""));
    CREDO_VERUM(app.montatio.doc == radix);
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "latitudo"), "468");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, "b"));
    CREDO_VERUM(app.montatio.doc == b_primum);

    imprimere("\n--- V: spatium sine pictura ---\n");
    CREDO_FALSUM(pictor_picturam_ponere(&app.montatio, "nihil"));
    CREDO_FALSUM(pictor_picturam_ponere(&app.montatio, "c"));
    CREDO_VERUM(app.montatio.doc == b_primum);
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "latitudo"), "100");

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
