/* probatio_vicus_quaestio.c - '$question<>...</>' in pagina scribae
 * (Franus 2026-10-13; per rivum quaestio-rivus T8, 2026-10-14)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba), porta
 * quaestionis FICTA scripta (sine processu, sine rete): incipere
 * argumenta servat, pulsus quisque gradum scripti reddit (textus
 * PLANATUS totus hactenus - portae officium, ruling R9). Ictus: nuntius
 * "asking"; pulsus II: opus incipit (pagina clausa, "streaming..."),
 * pulsus deinde: pagina crescit sine commissione; finis: caput
 * "<answered>", commissio una, nuntius usus. Lineae et
 * latitudo portae: liberae - I et latitudo - II x margo (R8). Pagina
 * plena: porta abrumpitur, praecisio ad finem sententiae. Esc:
 * "[cancelled]". Error medio: "[error: ...]"; ante textum: pagina
 * intacta. Incipere nequit, sine porta, sine spatio, quaestio vacua.
 * Forma (lineae logicae) ut ante; aequale: pagina = dispositio textus
 * finalis.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_characterum.h"
#include "scriba_applicatio.h"
#include "scriba_documentum.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* porta ficta: gradus quisque pulsum unum (textus planatus additur);
 * post gradus aut finis cum usu, aut error; sine_fine: textus semper
 * crescit (pagina plena) */
#define GRADUS_MAXIMI VIII

nomen structura {
     constans character* gradus[GRADUS_MAXIMI];
                    i32  numerus;
     constans character* error;            /* post gradus */
     constans character* error_incipiendi; /* incipere nequit */
                    b32  sine_fine;
                    s32  vocationes;      /* incipere */
                    s32  pulsus;
                    b32  abruptum;
                 chorda  quaestio;
                    s32  lineae;
                    s32  latitudo;
                 chorda  textus;          /* hactenus */
} PortaFicta;

interior vacuum*
_incipere (
     vacuum* ctx,
     chorda  quaestio,
        s32  lineae,
        s32  latitudo,
    Piscina* p,
     chorda* error)
{
    PortaFicta* pf = (PortaFicta*)ctx;

    pf->vocationes++;
    pf->quaestio = chorda_ex_literis(chorda_ut_cstr(quaestio, p),
        piscina);
    pf->lineae    = lineae;
    pf->latitudo  = latitudo;
    pf->pulsus    = ZEPHYRUM;
    pf->abruptum  = FALSUM;
    pf->textus    = chorda_ex_literis("", piscina);
    si (pf->error_incipiendi)
    {
        *error = chorda_ex_literis(pf->error_incipiendi, p);
        redde NIHIL;
    }
    redde pf;
}

interior VicusPulsusQuaestionis
_pulsare (
     vacuum* manubrium,
    Piscina* p)
{
                PortaFicta* pf = (PortaFicta*)manubrium;
    VicusPulsusQuaestionis  r;
                       i32  k;

    (vacuum)p;
    memset(&r, ZEPHYRUM, magnitudo(r));
    k = (i32)pf->pulsus;
    pf->pulsus++;
    si (pf->sine_fine)
    {
        pf->textus = chorda_concatenare(pf->textus, chorda_ex_literis(
            "Word after word, a sentence ends. ", piscina), piscina);
        r.mutatum  = VERUM;
        r.textus   = pf->textus;
        redde r;
    }
    si (k < pf->numerus)
    {
        pf->textus = chorda_concatenare(pf->textus, chorda_ex_literis(
            pf->gradus[k], piscina), piscina);
        r.mutatum  = VERUM;
        r.textus   = pf->textus;
        redde r;
    }
    r.finitus = VERUM;
    si (pf->error)
    {
        r.error = chorda_ex_literis(pf->error, piscina);
        redde r;
    }
    r.input   = XII;
    r.output  = XXXIV;
    redde r;
}

interior vacuum
_abrumpere (
    vacuum* manubrium)
{
    ((PortaFicta*)manubrium)->abruptum = VERUM;
}

interior vacuum
portam_ponere (
    VicusApplicatio* app,
         PortaFicta* pf)
{
    VicusPortaQuaestionis porta;

    porta.incipere   = _incipere;
    porta.pulsare    = _pulsare;
    porta.abrumpere  = _abrumpere;
    porta.ctx        = pf;
    vicus_applicatio_quaestionis_portam_ponere(app, &porta);
}

/* responsum unius gradus */
interior vacuum
responsum_ponere (
            PortaFicta* pf,
    constans character* textus)
{
    pf->gradus[ZEPHYRUM]  = textus;
    pf->numerus           = I;
}

interior ScribaMontatio*
visus (
    Vicus* v)
{
    redde (ScribaMontatio*)vicus_latus(vicus_activa(v),
        VICUS_SINISTRUM)->montatio;
}

interior vacuum
ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

interior b32
linea_est (
        ScribaMontatio* sm,
                   s32  linea,
    constans character* textus)
{
    constans TabulaCharacterum* t = &sm->actiones_ctx.laboris;
    redde memcmp(t->cellulae + (i32)linea * t->latitudo, textus,
        strlen(textus)) == ZEPHYRUM;
}

/* linea tota (spatiis finalibus demptis) in chordam */
interior chorda
linea_legere (
    ScribaMontatio* sm,
               s32  linea)
{
    constans TabulaCharacterum* t = &sm->actiones_ctx.laboris;
                        chorda  c;
                           s32  n = (s32)t->latitudo;

    dum (   n > ZEPHYRUM
         && (t->cellulae[(i32)linea * t->latitudo + (i32)(n - I)] == ' '
        || t->cellulae[(i32)linea * t->latitudo + (i32)(n - I)]
            == '\0'))
    {
        n--;
    }
    c.datum    = (i8*)(t->cellulae + (i32)linea * t->latitudo);
    c.mensura  = (i32)n;
    redde chorda_ex_literis(chorda_ut_cstr(c, piscina), piscina);
}

nomen structura {
             Volumen* vol;
     VicusApplicatio  app;
               Vicus* v;
          ManusLudus* m;
      ScribaMontatio* sin;
          PortaFicta  porta;
} Mundus;

interior vacuum
_parare (
    CredoContextus* c)
{
    Mundus* ms;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    ms       = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    CREDO_NECESSE_NON_NIHIL(ms);
    memset(ms, ZEPHYRUM, magnitudo(Mundus));
    c->datum = ms;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    ms->vol = volumen_temporarium(piscina, "probatio_vicus_quaestio");
    CREDO_NECESSE_NON_NIHIL(ms->vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&ms->app, piscina, intern,
        ms->vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    ms->v = ms->app.vicus;
    ms->m = manus_ludus_creare(piscina, ms->app.d);
    CREDO_NECESSE(vicus_activam_ponere(ms->v, "3"));
    dispensator_recomponere(ms->app.d);
    ms->sin = visus(ms->v);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    si (ms != NIHIL && ms->vol != NIHIL)
    {
        volumen_claudere(ms->vol);
    }
}

/* nuntius lineae status lateris sinistri (titulus nodi 'nuntius') */
interior chorda
nuntius (
    Mundus* ms)
{
     VicusLatus* l = vicus_latus(vicus_activa(ms->v), VICUS_SINISTRUM);
    InsulaRamus  r = insula_ramus(ms->v->repo, "scriba",
        chorda_ut_cstr(l->id, piscina));
    chorda* a = insula_ramus_attributum(&r, "nuntius");
    redde a ? *a : chorda_ex_literis("", piscina);
}

interior vacuum
scribere (
               Mundus* ms,
                   s32  linea,
    constans character* textus)
{
    character mandatum[CCLVI];

    ictus(ms->m, linea, ZEPHYRUM);
    sprintf(mandatum, "i%s", textus);
    CREDO_VERUM(manus_ludus_scribere(ms->m, mandatum));
    CREDO_VERUM(manus_ludus_clavem(ms->m, (character)XXVII, ZEPHYRUM));
}

interior vacuum
pulsare_n (
    Mundus* ms,
       i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        (vacuum)vicus_pulsare(ms->v);
    }
    dispensator_recomponere(ms->app.d);
}

/* pulsus usque ad finem operis (tectum XL) */
interior vacuum
ad_finem (
    Mundus* ms)
{
    i32 k;

    pulsare_n(ms, II);
    per (k = ZEPHYRUM; k < XL && ms->sin->actiones_ctx.opus; k++)
    {
        (vacuum)vicus_pulsare(ms->v);
    }
    dispensator_recomponere(ms->app.d);
}

interior s64
cursor_documenti (
    Mundus* ms)
{
    redde scriba_documentum_cursor(ms->sin->actiones_ctx.doc);
}

interior vacuum
_sectio_sine_porta (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis("no API port",
        piscina));
    pulsare_n(ms, II);
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
}

/* responsum unius gradus: rogatum (lineae, latitudo R8), pagina clausa,
 * textus pulsu tertio, finis pulsu quarto */
interior vacuum
_sectio_responsum (
    CredoContextus* c)
{
                        Mundus* ms = (Mundus*)c->datum;
    constans TabulaCharacterum* t;
                           s32  liberae;

    responsum_ponere(&ms->porta, "Short answer.");
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: asking claude-opus-5-5", piscina));
    CREDO_AEQUALIS_S32(ms->porta.vocationes, ZEPHYRUM);
    pulsare_n(ms, II);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, I);
    CREDO_NECESSE(ms->sin->actiones_ctx.opus != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(ms->porta.quaestio, "Why?");
    t        = &ms->sin->actiones_ctx.laboris;
    liberae  = (s32)t->altitudo - IV;   /* IV..altitudo-1 albae */
    CREDO_AEQUALIS_S32(ms->porta.lineae, liberae - I);
    CREDO_AEQUALIS_S32(ms->porta.latitudo, (s32)t->latitudo - IV);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: streaming... (Esc cancels)", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV), "");
    pulsare_n(ms, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "$question<>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Short answer.</>");
    pulsare_n(ms, I);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Short answer.</>");
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: 12 in, 34 out", piscina));
}

/* crescit per pulsus, nihil commissum, claves negleguntur; finis:
 * commissio una ('u' totum tollit) */
interior vacuum
_sectio_crescit (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;
       s64  cursor;

    ms->porta.gradus[ZEPHYRUM]  = "Salve ";
    ms->porta.gradus[I]         = "munde. ";
    ms->porta.gradus[II]        = "Iterum.";
    ms->porta.numerus           = III;
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    cursor = cursor_documenti(ms);
    ictus(ms->m, III, I);
    pulsare_n(ms, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Salve</>");
    CREDO_VERUM(manus_ludus_scribere(ms->m, "dd"));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "$question<>Why?</>");
    pulsare_n(ms, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Salve munde.</>");
    CREDO_VERUM(cursor_documenti(ms) == cursor);
    pulsare_n(ms, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Salve munde. Iterum.</>");
    pulsare_n(ms, I);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_VERUM(cursor_documenti(ms) == cursor + I);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>Why?</>");
    ictus(ms->m, XX, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(ms->m, "u"));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "$question<>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV), "");
}

/* pagina finalis = dispositio textus finalis (quaestio-rivus spec:
 * idem ac via sine rivo) */
interior vacuum
_sectio_aequale (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                        chorda  totum;
                        chorda  dispositum;
                           s32  linea;
                           i32  a;
                           i32  k;
                           i32  mala = ZEPHYRUM;

    ms->porta.gradus[ZEPHYRUM]  = "# Title\n\nFirst paragraph of the "
        "answer that runs long enough ";
    ms->porta.gradus[I]         = "to wrap at least once on the page.\n"
        "- item one\n- item two that also";
    ms->porta.gradus[II]        = " wraps when the page is narrow.\n\n"
        "\tcode line();";
    ms->porta.numerus           = III;
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    ad_finem(ms);
    totum = chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        ms->porta.gradus[ZEPHYRUM], piscina), chorda_ex_literis(
        ms->porta.gradus[I], piscina), piscina), chorda_ex_literis(
        ms->porta.gradus[II], piscina), piscina);
    dispositum = vicus_quaestio_disponere(totum, (s32)t->latitudo,
        (s32)t->altitudo - IV, piscina);
    CREDO_NECESSE(dispositum.mensura > I && dispositum.datum[ZEPHYRUM]
        == (i8)'\n');
    linea  = IV;
    a      = I;
    per (k = I; k <= dispositum.mensura; k++)
    {
        si (k == dispositum.mensura || dispositum.datum[k] == (i8)'\n')
        {
            chorda pars;

            pars.datum    = dispositum.datum + a;
            pars.mensura  = k - a;
            si (!chorda_aequalis(linea_legere(ms->sin, linea),
                    chorda_ex_literis(chorda_ut_cstr(pars, piscina),
                    piscina)))
            {
                mala++;
            }
            linea++;
            a = k + I;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    CREDO_VERUM(linea > VIII);
}

/* pagina plena: porta abrumpitur, praecisio ad finem sententiae */
interior vacuum
_sectio_plenum (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                           s32  q   = (s32)t->altitudo - IV;
                           s32  l;
                        chorda  ultima;

    ms->porta.sine_fine = VERUM;
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, q, "$question<>Long?</>");
    ictus(ms->m, q, I);
    ad_finem(ms);
    CREDO_VERUM(ms->porta.abruptum);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: page full", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, q),
        "<answered>Long?</>");
    per (l = q + I; l < (s32)t->altitudo; l++)
    {
        CREDO_VERUM(linea_legere(ms->sin, l).mensura
            <= (i32)t->latitudo - II);
    }
    ultima = linea_legere(ms->sin, (s32)t->altitudo - I);
    CREDO_VERUM(ultima.mensura >= IV
        && memcmp(ultima.datum + ultima.mensura - IV, ".</>", IV)
            == ZEPHYRUM);
}

/* Esc: porta abrumpitur, textus + "[cancelled]", caput responsum */
interior vacuum
_sectio_esc (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    ms->porta.gradus[ZEPHYRUM]  = "First sentence. ";
    ms->porta.gradus[I]         = "Second sentence.";
    ms->porta.numerus           = II;
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    pulsare_n(ms, III);
    CREDO_VERUM(manus_ludus_clavem(ms->m, (character)XXVII, ZEPHYRUM));
    dispensator_recomponere(ms->app.d);
    CREDO_VERUM(ms->porta.abruptum);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>First sentence.");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, V), "");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, VI),
        "  [cancelled]</>");
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: cancelled", piscina));
}

/* error post textum: "[error: ...]" in pagina, caput responsum */
interior vacuum
_sectio_error_medio (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    responsum_ponere(&ms->porta, "Part one.");
    ms->porta.error = "Overloaded";
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Part one.");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, VI),
        "  [error: Overloaded]</>");
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: Overloaded", piscina));
}

/* error ante textum: pagina intacta, caput manet (iterum rogari
 * potest) */
interior vacuum
_sectio_error_ante_textum (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;
       s64  cursor;

    ms->porta.numerus  = ZEPHYRUM;
    ms->porta.error    = "Overloaded";
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    cursor = cursor_documenti(ms);
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, I);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: Overloaded", piscina));
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV), "");
    CREDO_VERUM(cursor_documenti(ms) == cursor);
}

/* incipere nequit (adiutor absens): nuntius, nullum opus */
interior vacuum
_sectio_incipere_nequit (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    responsum_ponere(&ms->porta, "x");
    ms->porta.error_incipiendi = "helper not found";
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    pulsare_n(ms, II);
    CREDO_VERUM(ms->sin->actiones_ctx.opus == NIHIL);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: helper not found", piscina));
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
}

interior vacuum
_sectio_sine_spatio (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                           s32  q   = (s32)t->altitudo - II;

    responsum_ponere(&ms->porta, "x");
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, q, "$question<>Where?</>");
    scribere(ms, q + I, "below");
    ictus(ms->m, q, I);
    CREDO_CHORDA_CONTINET(nuntius(ms),
        chorda_ex_literis("no free lines",
        piscina));
    pulsare_n(ms, II);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, ZEPHYRUM);
    CREDO_VERUM(linea_est(ms->sin, q + I, "below"));
}

interior vacuum
_sectio_multilinea (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    responsum_ponere(&ms->porta, "ok");
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>first line, (comma)");
    scribere(ms, IV, "second line</>");
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(ms->porta.quaestio,
        "first line, (comma)\nsecond line");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>first line, (comma)");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, V),
        "  <answer>ok</>");
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    responsum_ponere(&ms->porta, "x");
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<></>");
    ictus(ms->m, III, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis("question:",
        piscina));
    pulsare_n(ms, II);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, ZEPHYRUM);
}

/* lineae logicae (forma quaestio.h): capitulum, linea vacua,
 * paragraphus refluens intra margines, elementum cum indentatione
 * pendente, codex verbatim (indentatio servata, numquam refluit) */
interior vacuum
_sectio_forma (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                           s32  w   = (s32)t->latitudo;
                           s32  l;
                        chorda  x;

    responsum_ponere(&ms->porta,
        "# Title\n"
        "\n"
        "This paragraph is long enough that it must wrap across several "
        "lines of the page at its width, and then some more words follow.\n"
        "- a list item that is also long enough to wrap onto a second line "
        "of the page here and onwards\n"
        "\tint f(void)\n"
        "\t{\n"
        "\t    return 1;\n"
        "\t}");
    portam_ponere(&ms->app, &ms->porta);
    scribere(ms, III, "$question<>Code?</>");
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer># Title");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, V), "");
    /* paragraphus: margo, intra latitudinem, plures lineae */
    l = VI;
    CREDO_VERUM(linea_est(ms->sin, l, "  This paragraph"));
    dum (l < (s32)t->altitudo && !linea_est(ms->sin, l, "  - a list"))
    {
        x = linea_legere(ms->sin, l);
        CREDO_VERUM(x.mensura <= (i32)(w - II));
        CREDO_VERUM(x.mensura > II && x.datum[0] == (i8)' '
            && x.datum[I] == (i8)' ' && x.datum[II] != (i8)' ');
        l++;
    }
    CREDO_VERUM(l > VII);
    /* elementum: linea secunda indentatione pendente (margo + "- ") */
    CREDO_VERUM(linea_est(ms->sin, l, "  - a list item"));
    l++;
    x = linea_legere(ms->sin, l);
    CREDO_VERUM(x.mensura > IV && linea_est(ms->sin, l, "    ")
        && x.datum[IV] != (i8)' ');
    dum (   l < (s32)t->altitudo
         && !linea_est(ms->sin, l, "  int f(void)"))
    {
        l++;
    }
    /* codex: verbatim, indentatio servata */
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, l),
        "  int f(void)");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, l + I), "  {");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, l + II),
        "      return 1;");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, l + III),
        "  }</>");
}


/* Franus 2026-10-14: linea prima longa + continuatio indentata. Olim
 * caput longius ("<question answered>", +VIII) lineam primam
 * transfluebat, cauda in lineam NOVAM ibat, et responsum ad lineam
 * '</>' VETEREM (iam caudam) inserebatur - quaestio in medio scissa */
interior vacuum
_sectio_prima_longa (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    responsum_ponere(&ms->porta, "ok");
    portam_ponere(&ms->app, &ms->porta);
    /* XXXVI cellulae in pagina XXXVIII */
    scribere(ms, III, "$question<>a question that continues");
    scribere(ms, IV, "  across the next line</>");
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<answered>a question that continues");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  across the next line</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, V),
        "  <answer>ok</>");
}

/* linea una usque ad marginem (latitudo - I): nihil transfluit,
 * responsum statim infra */
interior vacuum
_sectio_ad_marginem (
    CredoContextus* c)
{
       Mundus* ms  = (Mundus*)c->datum;
          s32  w   = (s32)ms->sin->actiones_ctx.laboris.latitudo;
          s32  k;
    character  linea[CCLVI];
    character  exspectata[CCLVI];

    responsum_ponere(&ms->porta, "ok");
    portam_ponere(&ms->app, &ms->porta);
    strcpy(linea, "$question<>");
    strcpy(exspectata, "<answered>");
    per (k = ZEPHYRUM; k < w - I - XI - III; k++)
    {
        strcat(linea, "x");
        strcat(exspectata, "x");
    }
    strcat(linea, "</>");
    strcat(exspectata, "</>");
    CREDO_NECESSE((s32)strlen(linea) == w - I);
    scribere(ms, III, linea);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III), linea);
    ictus(ms->m, III, I);
    ad_finem(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        exspectata);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>ok</>");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sine porta", _sectio_sine_porta, _parare, _purgare, NIHIL,
        NIHIL },
    { "responsum", _sectio_responsum, _parare, _purgare, NIHIL, NIHIL },
    { "crescit", _sectio_crescit, _parare, _purgare, NIHIL, NIHIL },
    { "aequale", _sectio_aequale, _parare, _purgare, NIHIL, NIHIL },
    { "plenum", _sectio_plenum, _parare, _purgare, NIHIL, NIHIL },
    { "esc", _sectio_esc, _parare, _purgare, NIHIL, NIHIL },
    { "error medio", _sectio_error_medio, _parare, _purgare, NIHIL,
        NIHIL },
    { "error ante textum", _sectio_error_ante_textum, _parare, _purgare,
        NIHIL, NIHIL },
    { "incipere nequit", _sectio_incipere_nequit, _parare, _purgare,
        NIHIL, NIHIL },
    { "sine spatio", _sectio_sine_spatio, _parare, _purgare, NIHIL,
        NIHIL },
    { "multilinea", _sectio_multilinea, _parare, _purgare, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, _parare, _purgare, NIHIL, NIHIL },
    { "forma", _sectio_forma, _parare, _purgare, NIHIL, NIHIL },
    { "prima longa", _sectio_prima_longa, _parare, _purgare, NIHIL,
        NIHIL },
    { "ad marginem", _sectio_ad_marginem, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_quaestio", SECTIONES);
}
