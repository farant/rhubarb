/* probatio_pictor_componentia.c - componere pictoris: arbor ex
 * insulis + motu, pura; exemplar arboris (gradus III) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "insula.h"
#include "motus.h"
#include "componens.h"
#include "pictor_componentia.h"
#include "pictor_commune.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes   = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes  = *(Punctum*)ctx;
}

interior vacuum
pan_ponere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -V;
    motus->pan.y = III;
}

interior vacuum
instrumentum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "instrumentum",
                             (constans character*)ctx);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                  Motus  motus;
       PictorCompositio  ctx;
              Componens* arbor;
              Componens* arbor2;
              Componens* c;
                Punctum  p;
              character  ignotum[]     = "ignotum";
              character  penicillus[]  = "penicillus";

    piscina = piscina_generare_dynamicum("probatio_pictor_componentia",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" zoom=\"2\""
        " focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    motus_initiare(&motus, piscina);
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    p.x = X;
    p.y = XX;
    mutare_motum(&motus, punctum_addere, &p, M + X);
    p.x = XII;
    p.y = XXII;
    mutare_motum(&motus, punctum_addere, &p, M + XX);
    ctx.fenestra_latitudo  = CCCXX;
    ctx.fenestra_altitudo  = CCXL;
    ctx.status_altitudo    = XII;

    imprimere("\n--- Actio ex instrumento ---\n");
    CREDO_VERUM(chorda_aequalis_literis(
        chorda_ex_literis(pictor_actio_instrumenti(
            chorda_ex_literis("penicillus", piscina)), piscina),
        "penicillus.ictus"));
    CREDO_VERUM(pictor_actio_instrumenti(
        chorda_ex_literis("ignotum", piscina))[ZEPHYRUM] == ZEPHYRUM);

    imprimere("\n--- Arbor ---\n");
    arbor = pictor_componere(repo, &motus, piscina, intern, &ctx);
    CREDO_NON_NIHIL(arbor);
    CREDO_CHORDA_AEQUALIS_LITERIS(arbor->id, "radix");
    CREDO_CHORDA_AEQUALIS_LITERIS(arbor->actio, "instrumentum.eligere");
    CREDO_AEQUALIS_S32(arbor->fines.latitudo, CCCXX);
    CREDO_AEQUALIS_I32(componens_numerus_liberorum(arbor), II);
    c = componens_invenire_per_id(arbor, chorda_ex_literis("prospectus",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->sectio);
    CREDO_AEQUALIS_S32(c->fines.altitudo, CCXL - XII);
    CREDO_AEQUALIS_S32(c->translatio.x, -V);
    CREDO_AEQUALIS_S32(c->translatio.y, III);
    CREDO_AEQUALIS_I32(c->scala, II);
    CREDO_FALSUM(c->focusabilis);
    c = componens_invenire_per_id(arbor, chorda_ex_literis("tabula",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->partes == PARTES_TABULA);
    CREDO_VERUM(c->praedicatum == PRAEDICATUM_PROPRIUS);
    CREDO_VERUM(c->focusabilis);
    CREDO_AEQUALIS_S32(c->fines.latitudo, CCCXX);
    CREDO_AEQUALIS_S32(c->fines.altitudo, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->actio, "penicillus.ictus");
    CREDO_AEQUALIS_I32(c->numerus_punctorum, II);
    CREDO_AEQUALIS_S32(c->puncta[I].x, XII);
    CREDO_VERUM(c->parens != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->parens->id, "prospectus");
    c = componens_invenire_per_id(arbor, chorda_ex_literis("status",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->partes == PARTES_TITULUS);
    CREDO_AEQUALIS_S32(c->fines.y, CCXL - XII);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "penicillus");
    CREDO_FALSUM(c->focusabilis);

    imprimere("\n--- Instrumentum ignotum: tabula sine actione; zoom"
              " absens: scala I ---\n");
    CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere, ignotum));
    arbor2 = pictor_componere(repo, &motus, piscina, intern, &ctx);
    c = componens_invenire_per_id(arbor2, chorda_ex_literis("tabula",
                                                            piscina));
    CREDO_VERUM(chorda_vacua(c->actio));
    c = componens_invenire_per_id(arbor2, chorda_ex_literis("status",
                                                            piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "ignotum");
    {
        InsulaRepositorium* repo2;
        repo2 = insula_repositorium_creare(piscina, intern,
            "<documentum latitudo=\"16\" altitudo=\"8\"/>",
            "<ephemera/>");
        arbor2 = pictor_componere(repo2, &motus, piscina, intern, &ctx);
        c = componens_invenire_per_id(arbor2,
                chorda_ex_literis("prospectus", piscina));
        CREDO_AEQUALIS_I32(c->scala, I);
        c = componens_invenire_per_id(arbor2,
                chorda_ex_literis("status", piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "nihil");
    }

    imprimere("\n--- Puritas et exemplar (gradus III) ---\n");
    arbor2 = pictor_componere(repo, &motus, piscina, intern, &ctx);
    /* instrumentum mutatum: arbor alia */
    CREDO_FALSUM(componens_aequalis(arbor, arbor2));
    CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere, penicillus));
    arbor2 = pictor_componere(repo, &motus, piscina, intern, &ctx);
    CREDO_VERUM(componens_aequalis(arbor, arbor2));    /* puritas */
    CREDO_VERUM(exemplar_conferre(piscina,
        "probationes/pictor/pictor.arbor.stml",
        componens_scribere_stml(arbor, piscina, intern, VERUM)));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
