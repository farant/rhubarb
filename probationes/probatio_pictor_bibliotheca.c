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
#include <time.h>

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

/* pinacotheca T2: spatia recentiae ordine, "|" separata ("" = radix)
 * - et seq stricte descendentes */
interior chorda
ordo_recens (
    Volumen* vol)
{
               Xar* l;
     PictorPictura* p;
     PictorPictura* prior;
            chorda  s;
               i32  i;

    s      = chorda_ex_literis("", piscina);
    prior  = NIHIL;
    l      = pictor_documenta_recentia(vol, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        p = (PictorPictura*)xar_obtinere(l, i);
        si (i > ZEPHYRUM)
        {
            s = chorda_concatenare(s, chorda_ex_literis("|", piscina),
                piscina);
        }
        s = chorda_concatenare(s, p->spatium, piscina);
        si (prior && !(p->seq < prior->seq))
        {
            s = chorda_concatenare(s, chorda_ex_literis("(seq!)",
                piscina), piscina);
        }
        prior = p;
    }
    redde s;
}

interior vacuum
actum_addere (
    PictorDocumentum* doc)
{
    (vacuum)pictor_documentum_actum(doc, chorda_ex_literis(
        "<ictus instrumentum=\"penicillus\" color=\"0\""
        " magnitudo=\"1\"><punctum x=\"2\" y=\"2\"/></ictus>",
        piscina));
}

/* pinacotheca T2 (spec D5): ordo = actum ultimum picturae (aut
 * conditio), descendens; aspicere non movet; revocatio actum est;
 * documenta aliena (scriba) absunt */
interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
     PictorDocumentum* a;
     PictorDocumentum* b;
     PictorDocumentum* cc;

    thema_initiare();
    vol = (Volumen*)c->datum;
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        CDLXXX, CDLXXX));
    a = pictor_documentum_creare(piscina, intern, vol, "a", C, LX,
        LXIV);
    b = pictor_documentum_creare(piscina, intern, vol, "b", C, LX,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    volumen_plagulam_condere(vol, chorda_ex_literis("x/documentum",
        piscina), chorda_ex_literis(
        "<documentum latitudo=\"40\" altitudo=\"20\"/>", piscina),
        "scriba:documentum");
    (vacuum)volumen_actum_appendere(vol, "x/ramus",
        chorda_ex_literis("{}", piscina));

    CREDO_GRADUS("conditio ordinem dat");
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "b|a|");

    CREDO_GRADUS("actum in 'a'");
    actum_addere(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "a|b|");

    CREDO_GRADUS("aspicere 'b' non movet");
    CREDO_VERUM(pictor_picturam_ponere(&app.montatio, "b"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "a|b|");

    CREDO_GRADUS("pictura nova vacua prima");
    cc = pictor_documentum_creare(piscina, intern, vol, "c", C, LX,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(cc);
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "c|a|b|");

    CREDO_GRADUS("revocatio actum est");
    actum_addere(b);
    actum_addere(cc);
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "c|b|a|");
    CREDO_VERUM(pictor_documentum_revocare(b));
    CREDO_CHORDA_AEQUALIS_LITERIS(ordo_recens(vol), "b|c|a|");
}

/* volumen vetus: radix sola (ante pinacothecam) - pictura una */
interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
              Volumen* vol;
     PictorDocumentum* radix;
                  Xar* l;

    vol   = (Volumen*)c->datum;
    radix = pictor_documentum_creare(piscina, intern, vol, "", C, LX,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(radix);
    actum_addere(radix);
    l = pictor_documenta_recentia(vol, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((PictorPictura*)xar_obtinere(l,
        ZEPHYRUM))->spatium, "");
    CREDO_VERUM(((PictorPictura*)xar_obtinere(l, ZEPHYRUM))->seq
        > ZEPHYRUM);
}

/* mensura (nulla assertio temporis): XIII picturae x CC acta - quam
 * diu recentia in hac machina (Fire 7 tardior) */
interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
              Volumen* vol;
     PictorDocumentum* d;
                  Xar* l;
              clock_t  t0;
            character  spatium[XVI];
                  i32  k;
                  i32  j;

    vol = (Volumen*)c->datum;
    per (k = ZEPHYRUM; k < XIII; k++)
    {
        sprintf(spatium, "p%d", (integer)k);
        d = pictor_documentum_creare(piscina, intern, vol, spatium, C,
            LX, LXIV);
        CREDO_NECESSE_NON_NIHIL(d);
        per (j = ZEPHYRUM; j < CC; j++)
        {
            actum_addere(d);
        }
    }
    t0  = clock();
    l   = pictor_documenta_recentia(vol, piscina);
    imprimere("  recentia: XIII picturae x CC acta: %.1f ms\n",
        (duplex)(clock() - t0) * 1000.0 / (duplex)CLOCKS_PER_SEC);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), XIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(((PictorPictura*)xar_obtinere(l,
        ZEPHYRUM))->spatium, "p12");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bibliotheca picturarum", _sectio_prima, _parare, _purgare, NIHIL,
        NIHIL },
    { "recentia (pinacotheca D5)", _sectio_secunda, _parare, _purgare,
        NIHIL, NIHIL },
    { "volumen vetus: radix sola", _sectio_tertia, _parare, _purgare,
        NIHIL, NIHIL },
    { "mensura: XIII x CC", _sectio_quarta, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_bibliotheca", SECTIONES);
}
