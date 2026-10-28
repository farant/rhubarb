/* probatio_pictor_canones.c - canon insularum pictoris et tabula
 * dominorum, EX MANIFESTO generati (status-plan S2:
 * apps/pictor/organum.stml + apps/organum_commune.stml): iudicant,
 * refusant */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include "pictor_organum_probationis.h"
#include <stdio.h>

interior vacuum
ponere_ignotum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "ignotissimum", "x");
}

interior vacuum
ponere_paletta (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "paletta", "1,2,3");
}

interior vacuum
ponere_palettam (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "palette", "color_primus");
}

interior vacuum
ponere_zoom (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "zoom", "2");
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                  Canon* canon;
     InsulaRepositorium* repo;
              StmlNodus* domini;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    CREDO_GRADUS("Canon et domini ex manifesto");
    canon   = NIHIL;
    domini  = NIHIL;
    CREDO_VERUM(pictor_organum_parare(piscina, intern, &canon,
        &domini));
    CREDO_NON_NIHIL(canon);
    CREDO_NON_NIHIL(domini);
    si (!canon || !domini)
    {
        redde;
    }

    CREDO_GRADUS("Insulae pictoris iudicantur");
    repo = insula_repositorium_creare(piscina, intern,
        "<pictor latitudo=\"320\" altitudo=\"200\""
        " instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo_penicilli=\"1\""
        " stratum_activum=\"0\" zoom=\"1\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    insula_ponere_canonem(repo, canon);
    insula_ponere_canonem(repo, canon);
    /* attributum non declaratum: recusatur */
    CREDO_FALSUM(insula_mutare(repo, ponere_ignotum, NIHIL));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));
    /* declarationes mortuae (census IX inventum 7) iam ignotae */
    CREDO_FALSUM(insula_mutare(repo, ponere_paletta, NIHIL));

    CREDO_GRADUS("Domini leguntur; motus solus zoom scribit");
    /* XVIII: XIII pictoris (scriptor, insula; S5c palette quoque;
     * glutinatio pendens 2026-10-13, dominus glutinatio.agere) + semen
     * (aspergillum.ictus, in insula usque ad migrationem) + IV basis
     * (focus, focus_acervus, superficies_*: dispensator). Olim
     * XVIII: cursus, gradus (mortui) abierunt. Durabilis: olim I
     * (sigillum, numquam scriptum), nunc nullus. XIX: pinacotheca T5
     * pagina_pinacothecae (dominus pinacotheca.agere). XXI:
     * communio-plan-5 T6 pagina_communium (communia.agere), spectata
     * (spectator.agere). */
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo,
                                             domini), XXI);
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("zoom.ponere", piscina));
    CREDO_FALSUM(insula_mutare(repo, ponere_zoom, NIHIL));
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus", piscina));
    CREDO_VERUM(insula_mutare(repo, ponere_zoom, NIHIL));
    /* S5c: palette - dominus aperiens et systema dimissio soli */
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("quilibet", piscina));
    CREDO_FALSUM(insula_mutare(repo, ponere_palettam, NIHIL));
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("palette.aperire", piscina));
    CREDO_VERUM(insula_mutare(repo, ponere_palettam, NIHIL));
    CREDO_VERUM(insula_restituere(repo));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "canon insularum pictoris et domini ex manifesto", _sectio_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_canones", SECTIONES);
}
