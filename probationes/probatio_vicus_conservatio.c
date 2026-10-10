/* probatio_vicus_conservatio.c - systema persistentiae in vico
 * (status-plan S3): compositio VERA (vicus_applicatio); latus pictoris
 * "2_dextrum_pictor" statum suum in volumine vici servat et sessio
 * altera (applicatio recens, idem volumen) eum restituit. */
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
    redde mutare_ramum(&ramus, INSULA_EPHEMERA, ponere, copia);
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
    a = insula_ramus_attributum(&ramus, INSULA_EPHEMERA, titulus);
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
    CREDO_VERUM(scribere(&prima, "quilibet", "palette=color_primus"));

    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CM, DC));
    CREDO_VERUM(valor_est(&altera, "instrumentum", "spongia"));
    CREDO_VERUM(valor_est(&altera, "zoom", "4"));
    CREDO_VERUM(valor_est(&altera, "palette", NIHIL));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus_pictoris", _probare_latus_pictoris,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_conservatio", SECTIONES);
}
