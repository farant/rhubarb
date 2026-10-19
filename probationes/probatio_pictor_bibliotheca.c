/* probatio_pictor_bibliotheca.c - bibliotheca picturarum (S3e-2)
 *
 * Applicatio pictoris vera (radix, spatium ""), volumen temporarium;
 * pictura altera "b" (C x LX) documento directo; plagula
 * "c/documentum" originis alienae (scriba: dimensiones quoque habet,
 * aperiri posset). I: pictor_documenta_enumerare - "b" et ""
 * (ordine viae), "c" non. II: pictor_picturam_ponere "b" -
 * documentum, contextus actionum et figurarum, dimensiones durabiles,
 * tabula composita. III: ictus in "b" scribitur, radix intacta. IV:
 * memoria - redire et iterum: documenta eadem. V: spatium sine
 * pictura: FALSUM, nihil mutatum. */
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

    /* status-plan C1: dimensiones in ephemera (insula una) */
    a = insula_attributum(app->repo, titulus);
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

/* piscina et intern (durabile, nodus_arboris) sectionis sunt;
 * volumen temporarium in c->datum - purgare id SEMPER claudit */
interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    c->datum = volumen_temporarium(piscina,
        "probatio_pictor_bibliotheca");
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    si (c->datum)
    {
        volumen_claudere((Volumen*)c->datum);
        c->datum = NIHIL;
    }
    piscina  = NIHIL;
    intern   = NIHIL;
}

/* gradus I-V seriatim in una applicatione: quisque statum prioris
 * legit, ergo sectio una cum gradibus nominatis */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* radix;
     PictorDocumentum* b;
     PictorDocumentum* b_primum;
                  i32  vivi_radicis;
                  i32  vivi_b;

    thema_initiare();
    vol = (Volumen*)c->datum;
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        CDLXXX, CDLXXX));
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

    CREDO_GRADUS("I: bibliotheca");
    {
        Xar* l;

        l = pictor_documenta_enumerare(vol, piscina);
        CREDO_NON_NIHIL(l);
        CREDO_VERUM(l && xar_numerus(l) == II);
        si (l && xar_numerus(l) == II)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l,
                ZEPHYRUM), "b");
            CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, I),
                "");
        }
    }

    CREDO_GRADUS("II: pictura 'b' ostensa");
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

    CREDO_GRADUS("III: ictus in 'b'");
    vivi_radicis  = pictor_documentum_numerus_vivorum(radix);
    vivi_b        = pictor_documentum_numerus_vivorum(b_primum);
    CREDO_AEQUALIS_I32(vivi_b, I);
    /* tabula ad cellulam (VI, VIII) */
    CREDO_VERUM(manus_ludus_premere_ad(m, VI + X, VIII + X));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b_primum),
        vivi_b + I);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(radix),
        vivi_radicis);

    CREDO_GRADUS("IV: memoria");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, ""));
    CREDO_VERUM(app.montatio.doc == radix);
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "latitudo"), "468");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, "b"));
    CREDO_VERUM(app.montatio.doc == b_primum);

    CREDO_GRADUS("V: spatium sine pictura");
    CREDO_FALSUM(pictor_picturam_ponere(&app.montatio, "nihil"));
    CREDO_FALSUM(pictor_picturam_ponere(&app.montatio, "c"));
    CREDO_VERUM(app.montatio.doc == b_primum);
    CREDO_CHORDA_AEQUALIS_LITERIS(durabile(&app, "latitudo"), "100");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bibliotheca picturarum", _sectio_prima, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_bibliotheca", SECTIONES);
}
