/* probatio_vicus_quaestio.c - '$question<>...</>' in pagina scribae
 * (Franus 2026-10-13)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba), porta
 * quaestionis FICTA (sine rete, sine vate). Ictus in caput: nuntius
 * "asking", folium intactum; pulsus II: caput "<question answered>",
 * responsum "<answer>...</>" infra, ad latitudinem paginae fractum,
 * nuntius usus. Spatium: (lineae liberae x latitudo - XI) x 85 / C
 * portae datum; responsum longum ad finem sententiae praeciditur, lineae
 * infra numquam deletae. Errores: sine porta, sine spatio, porta
 * defecit (folium intactum), quaestio vacua.
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
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* porta ficta: responsum aut error scriptum; argumenta servantur */
nomen structura {
     constans character* responsum;    /* NIHIL = error */
     constans character* error;
                    s32  vocationes;
                 chorda  quaestio;
                    s32  characteres;
                    s32  latitudo;
} PortaFicta;

interior b32
porta_ficta (
     vacuum* ctx,
     chorda  quaestio,
        s32  characteres,
        s32  latitudo,
    Piscina* p,
     chorda* responsum,
     chorda* nuntius,
     chorda* error)
{
    PortaFicta* pf = (PortaFicta*)ctx;

    pf->vocationes++;
    pf->quaestio = chorda_ex_literis(chorda_ut_cstr(quaestio, p),
        piscina);
    pf->characteres  = characteres;
    pf->latitudo     = latitudo;
    si (!pf->responsum)
    {
        *error = chorda_ex_literis(pf->error, p);
        redde FALSUM;
    }
    *responsum  = chorda_ex_literis(pf->responsum, p);
    *nuntius    = chorda_ex_literis("12 in, 34 out", p);
    redde VERUM;
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
pulsare_bis (
    Mundus* ms)
{
    (vacuum)vicus_pulsare(ms->v);
    (vacuum)vicus_pulsare(ms->v);
    dispensator_recomponere(ms->app.d);
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
    pulsare_bis(ms);
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
}

interior vacuum
_sectio_responsum (
    CredoContextus* c)
{
                        Mundus* ms = (Mundus*)c->datum;
    constans TabulaCharacterum* t;
                           s32  liberae;

    ms->porta.responsum = "Short answer.";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: asking claude-opus-5-5", piscina));
    CREDO_AEQUALIS_S32(ms->porta.vocationes, ZEPHYRUM);
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
    pulsare_bis(ms);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ms->porta.quaestio, "Why?");
    t        = &ms->sin->actiones_ctx.laboris;
    liberae  = (s32)t->altitudo - IV;   /* lineae IV..altitudo-1 albae */
    CREDO_AEQUALIS_S32(ms->porta.latitudo, (s32)t->latitudo);
    CREDO_AEQUALIS_S32(ms->porta.characteres,
        (liberae * (s32)t->latitudo - XI) * LXXXV / C);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<question answered>Why?</>");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV),
        "  <answer>Short answer.</>");
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: 12 in, 34 out", piscina));
}

/* responsum longum, lineae III liberae: praecisio ad finem sententiae,
 * omnes lineae intra latitudinem, nihil infra deletum */
interior vacuum
_sectio_longum (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                           s32  q   = (s32)t->altitudo - IV;
                           s32  l;
                        chorda  ultima;

    ms->porta.responsum =
        "The first sentence is here. The second one follows it. "
        "A third sentence keeps going. Then a fourth. Then a fifth "
        "sentence. Sixth. Seventh sentence goes on and on. Eighth. "
        "Ninth sentence is long enough to wrap around the page width. "
        "Tenth and last.";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, q, "$question<>Long?</>");
    ictus(ms->m, q, I);
    pulsare_bis(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, q),
        "<question answered>Long?</>");
    CREDO_VERUM(linea_est(ms->sin, q + I, "  <answer>"));
    /* margo II utrimque */
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

interior vacuum
_sectio_sine_spatio (
    CredoContextus* c)
{
                        Mundus* ms  = (Mundus*)c->datum;
    constans TabulaCharacterum* t   = &ms->sin->actiones_ctx.laboris;
                           s32  q   = (s32)t->altitudo - II;

    ms->porta.responsum = "x";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, q, "$question<>Where?</>");
    scribere(ms, q + I, "below");
    ictus(ms->m, q, I);
    CREDO_CHORDA_CONTINET(nuntius(ms),
        chorda_ex_literis("no free lines",
        piscina));
    pulsare_bis(ms);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, ZEPHYRUM);
    CREDO_VERUM(linea_est(ms->sin, q + I, "below"));
}

interior vacuum
_sectio_porta_defecit (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    ms->porta.responsum  = NIHIL;
    ms->porta.error      = "Overloaded";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, III, "$question<>Why?</>");
    ictus(ms->m, III, I);
    pulsare_bis(ms);
    CREDO_AEQUALIS_S32(ms->porta.vocationes, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis(
        "question: Overloaded", piscina));
    CREDO_VERUM(linea_est(ms->sin, III, "$question<>Why?</>"));
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, IV), "");
}

interior vacuum
_sectio_multilinea (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    ms->porta.responsum = "ok";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, III, "$question<>first line, (comma)");
    scribere(ms, IV, "second line</>");
    ictus(ms->m, III, I);
    pulsare_bis(ms);
    CREDO_CHORDA_AEQUALIS_LITERIS(ms->porta.quaestio,
        "first line, (comma)\nsecond line");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, III),
        "<question answered>first line, (comma)");
    CREDO_CHORDA_AEQUALIS_LITERIS(linea_legere(ms->sin, V),
        "  <answer>ok</>");
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Mundus* ms = (Mundus*)c->datum;

    ms->porta.responsum = "x";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, III, "$question<></>");
    ictus(ms->m, III, I);
    CREDO_CHORDA_CONTINET(nuntius(ms), chorda_ex_literis("question:",
        piscina));
    pulsare_bis(ms);
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

    ms->porta.responsum =
        "# Title\n"
        "\n"
        "This paragraph is long enough that it must wrap across several "
        "lines of the page at its width, and then some more words follow.\n"
        "- a list item that is also long enough to wrap onto a second line "
        "of the page here and onwards\n"
        "\tint f(void)\n"
        "\t{\n"
        "\t    return 1;\n"
        "\t}";
    vicus_applicatio_quaestionis_portam_ponere(&ms->app, porta_ficta,
        &ms->porta);
    scribere(ms, III, "$question<>Code?</>");
    ictus(ms->m, III, I);
    pulsare_bis(ms);
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "sine porta", _sectio_sine_porta, _parare, _purgare, NIHIL,
        NIHIL },
    { "responsum", _sectio_responsum, _parare, _purgare, NIHIL, NIHIL },
    { "longum", _sectio_longum, _parare, _purgare, NIHIL, NIHIL },
    { "sine spatio", _sectio_sine_spatio, _parare, _purgare, NIHIL,
        NIHIL },
    { "porta defecit", _sectio_porta_defecit, _parare, _purgare, NIHIL,
        NIHIL },
    { "multilinea", _sectio_multilinea, _parare, _purgare, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, _parare, _purgare, NIHIL, NIHIL },
    { "forma", _sectio_forma, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_quaestio", SECTIONES);
}
