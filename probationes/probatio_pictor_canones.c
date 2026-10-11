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

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                  Canon* canon;
     InsulaRepositorium* repo;
              StmlNodus* domini;

    piscina = piscina_generare_dynamicum("probatio_pictor_canones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Canon et domini ex manifesto ---\n");
    canon   = NIHIL;
    domini  = NIHIL;
    CREDO_VERUM(pictor_organum_parare(piscina, intern, &canon,
        &domini));
    CREDO_NON_NIHIL(canon);
    CREDO_NON_NIHIL(domini);
    si (!canon || !domini)
    {
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- Insulae pictoris iudicantur ---\n");
    repo = insula_repositorium_creare(piscina, intern,
        "<pictor latitudo=\"320\" altitudo=\"200\"/>",
        "<pictor instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo_penicilli=\"1\""
        " stratum_activum=\"0\""
        " zoom=\"1\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    insula_ponere_canonem(repo, INSULA_DURABILIS, canon);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, canon);
    /* attributum non declaratum: recusatur */
    CREDO_FALSUM(mutare_ephemera(repo, ponere_ignotum, NIHIL));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));
    /* declarationes mortuae (census IX inventum 7) iam ignotae */
    CREDO_FALSUM(mutare_durabile(repo, ponere_paletta, NIHIL));

    imprimere("\n--- Domini leguntur; motus solus zoom scribit ---\n");
    /* XVII: XII pictoris (scriptor, insula; S5c palette quoque) + semen
     * (aspergillum.ictus, in insula usque ad migrationem) + IV basis
     * (focus, focus_acervus, superficies_*: dispensator). Olim
     * XVIII: cursus, gradus (mortui) abierunt. Durabilis: olim I
     * (sigillum, numquam scriptum), nunc nullus. */
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_EPHEMERA,
                                             domini), XVII);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_DURABILIS,
                                             domini), ZEPHYRUM);
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("zoom.ponere", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    /* S5c: palette - dominus aperiens et systema dimissio soli */
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("quilibet", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_palettam, NIHIL));
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("palette.aperire", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_palettam, NIHIL));
    CREDO_VERUM(insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
