/* probatio_pictor_ramus.c - pictor in ramo NON radicis
 * (insula-rami-plan R3)
 *
 * Repositorium hospitis cum duobus pictoribus montatis (p1, p2);
 * canones et domini pictoris VERI ramo p1 addicti; actiones et
 * compositio ramum p1 in contextu ferunt. Probat: arbor ex ramo
 * (dimensiones documenti), 'p' instrumentum in p1 SOLO scribit (non
 * radici, non p2; dominus iudicat), ictus colorem e ramo legit. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "actio.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior Canon*
canonem_legere (
    constans character* via)
{
    chorda causa;

    redde canon_legere(filum_legere_totum(via, piscina), piscina,
        intern,
                       &causa);
}

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

s32 principale (vacuum)
{
                 Volumen* vol;
        PictorDocumentum* doc;
      InsulaRepositorium* repo;
             InsulaRamus  p1;
             InsulaRamus  p2;
          ActioRegistrum* reg;
          PictorActiones  actiones;
        PictorCompositio  cfg;
             Dispensator* d;
              ManusLudus* m;
                 Punctum  via[II];
                     Xar* acta;
                  chorda  ultimum;

    piscina = piscina_generare_dynamicum("probatio_pictor_ramus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_ramus");
    doc = pictor_documentum_creare(piscina, intern, vol, "p1", XL, XXX,
                                   LXIV);
    repo = insula_repositorium_creare(piscina, intern,
        "<hospes><pictor id=\"p1\" latitudo=\"40\" altitudo=\"30\"/>"
        "<pictor id=\"p2\" latitudo=\"8\" altitudo=\"8\"/></hospes>",
        "<hospes focus=\"tabula\">"
        "<pictor id=\"p1\" instrumentum=\"nihil\" color_primus=\"3\""
        " magnitudo=\"1\" zoom=\"1\" superficies_latitudo=\"120\""
        " superficies_altitudo=\"64\"/>"
        "<pictor id=\"p2\" instrumentum=\"nihil\"/></hospes>");
    CREDO_NON_NIHIL(repo);
    p1 = insula_ramus(repo, "pictor", "p1");
    p2 = insula_ramus(repo, "pictor", "p2");
    insula_ramus_canonem_ponere(&p1, INSULA_DURABILIS,
        canonem_legere("apps/pictor/canones/durabilis.canon"));
    insula_ramus_canonem_ponere(&p1, INSULA_EPHEMERA,
        canonem_legere("apps/pictor/canones/ephemera.canon"));
    {
        StmlResultus res;

        res = stml_legere_ex_literis(chorda_ut_cstr(filum_legere_totum(
            "apps/pictor/canones/domini.stml", piscina), piscina),
            piscina, intern);
        CREDO_VERUM(insula_ramus_dominos_legere(&p1, INSULA_EPHEMERA,
            res.elementum_radix) > ZEPHYRUM);
    }
    reg             = actio_registrum_creare(piscina, intern);
    memset(&actiones, ZEPHYRUM, magnitudo(actiones));
    actiones.doc    = doc;
    actiones.ramus  = p1;
    pictor_actiones_registrare(reg, &actiones);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.fenestra_latitudo  = CCCXX;
    cfg.fenestra_altitudo  = CCXII;
    cfg.cellula_latitudo   = VI;
    cfg.cellula_altitudo   = VIII;
    cfg.status_lineae      = I;
    cfg.ramus              = p1;
    d = dispensator_creare(piscina, intern, repo, reg, pictor_componere,
                           &cfg, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);

    imprimere("\n--- I: arbor ex ramo p1 ---\n");
    CREDO_MANUS_LUDUS_EXISTIT(m, "#tabula");
    {
        Componens* t;

        t = componens_invenire_per_id(dispensator_arbor(d),
            chorda_ex_literis("tabula", piscina));
        CREDO_NON_NIHIL(t);
        si (t)
        {
            CREDO_AEQUALIS_S32(t->fines.latitudo, XL);
            CREDO_AEQUALIS_S32(t->fines.altitudo, XXX);
        }
    }

    imprimere("\n--- II: 'p' instrumentum in p1 solo ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&p1, INSULA_EPHEMERA,
        "instrumentum"), "penicillus"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&p2, INSULA_EPHEMERA,
        "instrumentum"), "nihil"));
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "instrumentum"));
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n--- III: ictus colorem e ramo legit ---\n");
    via[ZEPHYRUM].x  = V;
    via[ZEPHYRUM].y  = V;
    via[I].x         = XX;
    via[I].y         = X;
    CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, II));
    acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    ultimum = ((VolumenActum*)xar_obtinere(acta,
        xar_numerus(acta) - I))->datum;
    CREDO_CHORDA_CONTINET(ultimum, chorda_ex_literis("color=\"3\"",
        piscina));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc),
        ((VolumenActum*)xar_obtinere(acta, xar_numerus(acta)
            - I))->seq);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
