/* probatio_montatio.c - pictor et duae scribae in repositorio uno
 * (insula-rami-plan T1a)
 *
 * pictor_montare / scriba_montare in repositorium hospitis cum
 * volumine UNO: elementa initialia, documenta in spatiis suis, canones
 * et domini in ramis, componere ex ramo, remontatio super idem volumen
 * documenta sua reaperit. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "stml.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

interior b32
plagula_exstat (
               Volumen* vol,
    constans character* via)
{
    b32 inventum;

    (vacuum)volumen_plagulam_promere(vol, chorda_ex_literis(via,
        piscina),
                                     piscina, &inventum);
    redde inventum;
}

interior vacuum
modum_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(nodus, p, in, "modus",
                             (constans character*)ctx);
}

hic_manens character modus_visualis[] = "visualis";
hic_manens character modus_malus[]    = "xyz";

s32 principale (vacuum)
{
               Volumen* vol;
    InsulaRepositorium* repo;
        PictorMontatio  pm;
        ScribaMontatio  m1;
        ScribaMontatio  m2;
             Componens* arbor;

    piscina = piscina_generare_dynamicum("probatio_montatio", LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol  = volumen_temporarium(piscina, "probatio_montatio");
    repo = insula_repositorium_creare(piscina, intern, "<schirmata/>",
        "<schirmata focus=\"pagina\"/>");
    CREDO_NON_NIHIL(repo);

    imprimere("\n--- I: montare - elementa, documenta in spatiis\n");
    CREDO_VERUM(pictor_montare(&pm, piscina, intern, vol, repo, "p1",
        NIHIL, CDLXXX, CDLXXX));
    CREDO_VERUM(scriba_montare(&m1, piscina, intern, vol,
        repo, "s1",
        NIHIL, CDLXXX, CDLXXX));
    CREDO_VERUM(scriba_montare(&m2, piscina, intern, vol,
        repo, "s2",
        NIHIL, CDLXXX, CDLXXX));
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo,
        INSULA_DURABILIS)), III);
    CREDO_VERUM(valor_est(insula_ramus_attributum(&pm.ramus,
        INSULA_DURABILIS, "latitudo"), "320"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&pm.ramus,
        INSULA_EPHEMERA, "instrumentum"), "penicillus"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m1.ramus,
        INSULA_DURABILIS, "latitudo"), "68"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m2.ramus,
        INSULA_EPHEMERA, "modus"), "normalis"));
    CREDO_VERUM(plagula_exstat(vol, "p1/documentum"));
    CREDO_VERUM(plagula_exstat(vol, "s1/documentum"));
    CREDO_VERUM(plagula_exstat(vol, "s2/documentum"));
    CREDO_FALSUM(plagula_exstat(vol, "documentum"));
    /* radix statum applicationum non fert */
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "modus"));

    imprimere("\n--- II: canones et domini in ramis ---\n");
    insula_scriptorem_ponere(repo, chorda_ex_literis("pagina.clavis",
        piscina));
    CREDO_FALSUM(mutare_ramum(&m1.ramus, INSULA_EPHEMERA,
        modum_ponere,
        modus_malus));                                  /* electio */
    CREDO_VERUM(mutare_ramum(&m1.ramus, INSULA_EPHEMERA,
        modum_ponere,
        modus_visualis));
    insula_scriptorem_ponere(repo, chorda_ex_literis("alius", piscina));
    CREDO_FALSUM(mutare_ramum(&m2.ramus, INSULA_EPHEMERA,
        modum_ponere,
        modus_visualis));                               /* dominus */
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m2.ramus,
        INSULA_EPHEMERA, "modus"), "normalis"));

    imprimere("\n--- III: componere ex ramo ---\n");
    arbor = scriba_componere(repo, NIHIL, piscina, intern,
        &m1.compositio);
    CREDO_NON_NIHIL(arbor);
    si (arbor)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(componens_invenire_per_id(arbor,
            chorda_ex_literis("pagina", piscina))->titulus, "visualis");
    }

    imprimere("\n--- IV: remontatio super idem volumen ---\n");
    {
        InsulaRepositorium* repo2;
            ScribaMontatio  r1;
                       i32  vivi;

        CREDO_VERUM(scriba_documentum_committere(m1.doc,
            &m1.actiones_ctx.laboris) == ZEPHYRUM);
        tabula_cellula(&m1.actiones_ctx.laboris, ZEPHYRUM,
            ZEPHYRUM)
            = 'x';
        CREDO_VERUM(scriba_documentum_committere(m1.doc,
            &m1.actiones_ctx.laboris) > ZEPHYRUM);
        vivi = scriba_documentum_numerus_vivorum(m1.doc);
        repo2 = insula_repositorium_creare(piscina, intern,
            "<schirmata/>", "<schirmata/>");
        CREDO_VERUM(scriba_montare(&r1, piscina, intern, vol, repo2,
            "s1",
            NIHIL, CDLXXX, CDLXXX));
        CREDO_AEQUALIS_I32(scriba_documentum_numerus_vivorum(r1.doc),
                           vivi);
        CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(r1.doc),
            ZEPHYRUM, ZEPHYRUM) == 'x');
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
