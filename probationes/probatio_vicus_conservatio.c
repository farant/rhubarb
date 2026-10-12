/* probatio_vicus_conservatio.c - systema persistentiae in vico
 * (status-plan S3): compositio VERA (vicus_applicatio); latus pictoris
 * "2_dextrum_pictor" statum suum in volumine vici servat et sessio
 * altera (applicatio recens, idem volumen) eum restituit. S7b:
 * registrum vim (capsa) unum applicationis in radice - deletum in
 * tabula III, insertum in IV, sessionem alteram supervivit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ctx = "attributum=valor" */
interior vacuum
ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     constans character* par;
     constans character* signum;
              character  titulus[LXIV];

    par     = (constans character*)ctx;
    signum  = strchr(par, '=');
    memcpy(titulus, par, (size_t)(signum - par));
    titulus[signum - par] = '\0';
    insula_attributum_ponere(radix, p, in, titulus, signum + I);
}

interior b32
scribere (
       VicusApplicatio* app,
    constans character* scriptor,
    constans character* par)
{
    InsulaRamus ramus;
      character copia[CXXVIII];

    strcpy(copia, par);
    ramus = insula_ramus(app->vicus->repo, "pictor",
        "2_dextrum_pictor");
    insula_scriptorem_ponere(app->vicus->repo,
        chorda_ex_literis(scriptor,
        app->piscina));
    redde mutare_ramum(&ramus, ponere, copia);
}

interior b32
valor_est (
       VicusApplicatio* app,
    constans character* titulus,
    constans character* valor)
{
    InsulaRamus  ramus;
         chorda* a;

    ramus = insula_ramus(app->vicus->repo, "pictor",
        "2_dextrum_pictor");
    a = insula_ramus_attributum(&ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior vacuum
_probare_latus_pictoris (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
         VicusApplicatio  prima;
         VicusApplicatio  altera;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_vicus_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&prima, c->piscina, in,
        vol, NIHIL, CM, DC));
    CREDO_NECESSE(valor_est(&prima, "instrumentum", "penicillus"));
    CREDO_VERUM(scribere(&prima, "instrumentum.eligere",
        "instrumentum=spongia"));
    CREDO_VERUM(scribere(&prima, "motus", "zoom=4"));
    CREDO_VERUM(scribere(&prima, "palette.aperire",
        "palette=color_primus"));

    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CM, DC));
    CREDO_VERUM(valor_est(&altera, "instrumentum", "spongia"));
    CREDO_VERUM(valor_est(&altera, "zoom", "4"));
    CREDO_VERUM(valor_est(&altera, "palette", NIHIL));
    volumen_claudere(vol);
}

/* S5c: dimissio in vico ligata - latus pictoris in acervo tabulae
 * activae, palette aperta, Esc per dispensatorem verum eam claudit */
interior vacuum
_probare_dimissionem (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
         VicusApplicatio  app;
             InsulaRamus  ramus;
                 Eventus  e;
                  chorda* a;
               character  par[] = "palette=color_primus";

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_vicus_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&app, c->piscina, in, vol,
        NIHIL, CM, DC));
    CREDO_NECESSE(vicus_acervo_aperire(app.vicus, "pictor", ""));
    (vacuum)vicus_pulsare(app.vicus);
    dispensator_recomponere(app.d);
    ramus = insula_ramus(app.vicus->repo, "pictor", "1_dextrum_pictor");
    CREDO_NECESSE(insula_ramus_nodus(&ramus) != NIHIL);
    insula_scriptorem_ponere(app.vicus->repo, chorda_ex_literis(
        "palette.aperire", c->piscina));
    CREDO_NECESSE(mutare_ramum(&ramus, ponere, par));
    dispensator_recomponere(app.d);
    app.d->motus.spatium = chorda_ex_literis("1_dextrum_pictor",
        c->piscina);
    memset(&e, ZEPHYRUM, magnitudo(e));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_EFFUGIUM;
    e.datum.clavis.producta  = XXVII;
    dispensator_tractare(app.d, &e);
    a = insula_ramus_attributum(&ramus, "palette");
    CREDO_VERUM(a == NIHIL);
    volumen_claudere(vol);
}

/* S6b: latus scribae "1_sinistrum_scriba" cursorem servat (systemata
 * repositorii communia), modus ut vim incipit */
interior vacuum
_probare_latus_scribae (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
         VicusApplicatio  prima;
         VicusApplicatio  altera;
             InsulaRamus  r;
               character  cursor[]  = "cursor_linea=3";
               character  modus[]   = "modus=inserere";
                  chorda* a;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_vicus_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&prima, c->piscina, in,
        vol, NIHIL, CM, DC));
    r = insula_ramus(prima.vicus->repo, "scriba", "1_sinistrum_scriba");
    CREDO_NECESSE(insula_ramus_nodus(&r) != NIHIL);
    insula_scriptorem_ponere(prima.vicus->repo, chorda_ex_literis(
        "pagina.clavis", c->piscina));
    CREDO_VERUM(mutare_ramum(&r, ponere, cursor));
    CREDO_VERUM(mutare_ramum(&r, ponere, modus));

    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CM, DC));
    r = insula_ramus(altera.vicus->repo, "scriba",
        "1_sinistrum_scriba");
    a = insula_ramus_attributum(&r, "cursor_linea");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "3"));
    a = insula_ramus_attributum(&r, "modus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "normalis"));
    volumen_claudere(vol);
}

/* S7b: latus scribae tabulae t (sinistrum) */
interior ScribaMontatio*
scriba_tabulae (
     VicusApplicatio* app,
                 i32  t)
{
    redde (ScribaMontatio*)vicus_latus(vicus_tabula(app->vicus, t),
        VICUS_SINISTRUM)->montatio;
}

/* S7b: registrum vim unum applicationis (radix), persistens */
interior vacuum
_probare_capsam (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
         VicusApplicatio  prima;
         VicusApplicatio  altera;
              ManusLudus* m;
          ScribaMontatio* s4;
             InsulaRamus  r;
                  chorda* a;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_vicus_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&prima, c->piscina, in,
        vol, NIHIL, CM, DC));
    m = manus_ludus_creare(c->piscina, prima.d);
    CREDO_NECESSE_NON_NIHIL(m);
    /* tabula III: scriba focata; "abc" scribitur et deletur (dd -
     * registrum; vim nostrum 'y' non habet) */
    CREDO_VERUM(manus_ludus_clavem(m, '3', MOD_SUPER));
    CREDO_VERUM(manus_ludus_scribere(m, "iabc"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(manus_ludus_scribere(m, "dd"));
    /* registrum in radice, non in latere */
    a = insula_attributum(prima.vicus->repo, "capsa_lineae");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
    a = insula_attributum(prima.vicus->repo, "capsa");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "abc"));
    r = insula_ramus(prima.vicus->repo, "scriba", "3_sinistrum_scriba");
    CREDO_NECESSE(insula_ramus_nodus(&r) != NIHIL);
    CREDO_NIHIL(insula_ramus_attributum(&r, "capsa"));
    /* tabula IV: 'p' registrum commune inserit */
    CREDO_VERUM(manus_ludus_clavem(m, '4', MOD_SUPER));
    CREDO_VERUM(manus_ludus_scribere(m, "p"));
    s4 = scriba_tabulae(&prima, III);
    CREDO_NECESSE_NON_NIHIL(s4);
    /* linea post cursorem (p infra inserit) */
    CREDO_VERUM(tabula_cellula(&s4->actiones_ctx.laboris, I, ZEPHYRUM)
        == 'a');
    CREDO_VERUM(tabula_cellula(&s4->actiones_ctx.laboris, I, I) == 'b');
    CREDO_VERUM(tabula_cellula(&s4->actiones_ctx.laboris, I, II)
        == 'c');

    /* sessio altera: registrum restitutum */
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CM, DC));
    a = insula_attributum(altera.vicus->repo, "capsa");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "abc"));
    a = insula_attributum(altera.vicus->repo, "capsa_lineae");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus_pictoris", _probare_latus_pictoris,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "dimissio", _probare_dimissionem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "latus_scribae", _probare_latus_scribae,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "capsa", _probare_capsam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_conservatio", SECTIONES);
}
