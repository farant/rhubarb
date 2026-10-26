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
#include "scriba_liber.h"
#include "tabula_characterum.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

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
               Piscina* piscina,
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

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
               Piscina* piscina;
   InternamentumChorda* intern;
               Volumen* vol;
    InsulaRepositorium* repo;
        PictorMontatio  pm;
        ScribaMontatio  m1;
        ScribaMontatio  m2;
             Componens* arbor;
             character  modus_visualis[]  = "visualis";
             character  modus_malus[]     = "xyz";

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    vol  = volumen_temporarium(piscina, "probatio_montatio");
    repo = insula_repositorium_creare(piscina, intern,
        "<schirmata focus=\"pagina\"/>");
    CREDO_NON_NIHIL(repo);

    CREDO_GRADUS("I: montare - elementa, documenta in spatiis");
    CREDO_VERUM(pictor_montare(&pm, piscina, intern, vol, repo, "p1",
        CDLXXX, CDLXXX, NIHIL));
    CREDO_VERUM(scriba_montare(&m1, piscina, intern, vol,
        repo, "s1", CDLXXX, CDLXXX, NIHIL, NIHIL));
    CREDO_VERUM(scriba_montare(&m2, piscina, intern, vol,
        repo, "s2", CDLXXX, CDLXXX, NIHIL, NIHIL));
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo)), III);
    /* C2: insula una - ramus quisque semel (latitudo cum statu) */
    /* S2c: documentum novum magnitudine superficiei - pictor:
     * superficies minus linea status (P1a: III cellulae, XXIV) et
     * margo cellulae utrimque (CDLXXX - XII x CDLXXX - XXIV - XVI) */
    CREDO_VERUM(valor_est(insula_ramus_attributum(&pm.ramus,
        "latitudo"), "468"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&pm.ramus,
        "altitudo"), "440"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&pm.ramus,
        "instrumentum"), "penicillus"));
    /* scriba: cellulae VI x VIII minus margines (II) et status (I) -
     * LXXX - II x LX - III */
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m1.ramus,
        "latitudo"), "78"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m1.ramus,
        "altitudo"), "57"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m2.ramus, "modus"),
        "normalis"));
    CREDO_VERUM(plagula_exstat(vol, piscina, "p1/documentum"));
    /* sine libro (documentum proprium): nullus index paginae */
    CREDO_NIHIL(insula_ramus_attributum(&m1.ramus,
        "pagina_positio"));
    CREDO_VERUM(plagula_exstat(vol, piscina, "s1/documentum"));
    CREDO_VERUM(plagula_exstat(vol, piscina, "s2/documentum"));
    CREDO_FALSUM(plagula_exstat(vol, piscina, "documentum"));
    /* radix statum applicationum non fert */
    CREDO_NIHIL(insula_attributum(repo, "modus"));

    CREDO_GRADUS("II: canones et domini in ramis");
    insula_scriptorem_ponere(repo, chorda_ex_literis("pagina.clavis",
        piscina));
    CREDO_FALSUM(mutare_ramum(&m1.ramus,
        modum_ponere,
        modus_malus));                                  /* electio */
    CREDO_VERUM(mutare_ramum(&m1.ramus,
        modum_ponere,
        modus_visualis));
    insula_scriptorem_ponere(repo, chorda_ex_literis("alius", piscina));
    CREDO_FALSUM(mutare_ramum(&m2.ramus,
        modum_ponere,
        modus_visualis));                               /* dominus */
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&m2.ramus, "modus"),
        "normalis"));

    CREDO_GRADUS("III: componere ex ramo");
    arbor = scriba_componere(repo, NIHIL, piscina, intern,
        &m1.compositio);
    CREDO_NON_NIHIL(arbor);
    si (arbor)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(componens_invenire_per_id(arbor,
            chorda_ex_literis("pagina", piscina))->titulus, "visualis");
    }

    CREDO_GRADUS("IV: remontatio super idem volumen");
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
            "<schirmata/>");
        /* superficies alia (S2c): documentum exstans magnitudinem
         * suam servat */
        CREDO_VERUM(scriba_montare(&r1, piscina, intern, vol, repo2,
            "s1", CCXL, CCXL, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32(scriba_documentum_numerus_vivorum(r1.doc),
                           vivi);
        CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(r1.doc),
            ZEPHYRUM, ZEPHYRUM) == 'x');
        CREDO_AEQUALIS_I32(scriba_documentum_tabula(r1.doc)->latitudo,
            LXXVIII);
    }

    CREDO_GRADUS("V: superficies parva - documenta minima");
    {
        InsulaRepositorium* repo_parvum;
            ScribaMontatio  sp;
            PictorMontatio  pp;

        repo_parvum = insula_repositorium_creare(piscina, intern,
            "<schirmata/>");
        CREDO_VERUM(scriba_montare(&sp, piscina, intern, vol,
            repo_parvum,
            "s9", LX, XL, NIHIL, NIHIL));
        CREDO_AEQUALIS_I32(scriba_documentum_tabula(sp.doc)->latitudo,
            XX);
        CREDO_AEQUALIS_I32(scriba_documentum_tabula(sp.doc)->altitudo,
            X);
        CREDO_VERUM(pictor_montare(&pp, piscina, intern, vol,
            repo_parvum,
            "p9", LX, XL, NIHIL));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&pp.ramus,
            "latitudo"), "64"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&pp.ramus,
            "altitudo"), "64"));
    }

    CREDO_GRADUS("VI: scriba nidificata (A6c)");
    {
        InsulaRepositorium* rn;
               InsulaRamus  parens;
            ScribaMontatio  sn;
               ScribaLiber* ld;

        rn = insula_repositorium_creare(piscina, intern,
            "<schirmata><calendarium id=\"c1\"/></schirmata>");
        CREDO_NECESSE_NON_NIHIL(rn);
        parens  = insula_ramus(rn, "calendarium", "c1");
        ld      = scriba_liber_aperire_in_spatio(piscina, intern, vol,
            "dies/2026-10-14", XL, XX);
        CREDO_NECESSE_NON_NIHIL(ld);
        CREDO_VERUM(scriba_montare_in_ramo(&sn, piscina, intern, vol,
            &parens, "c1_notae", CCXL, CLX, ld, NIHIL));
        /* sub parente, non in radice: radix liberum unum tenet */
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(rn)), I);
        CREDO_VERUM(chorda_continet(sn.ramus.via, chorda_ex_literis(
            "calendarium:c1", piscina)));
        CREDO_VERUM(chorda_continet(sn.ramus.via, chorda_ex_literis(
            "scriba:c1_notae", piscina)));
        /* documentum = pagina prima libri diei */
        CREDO_VERUM(sn.doc == scriba_liber_pagina(ld,
            chorda_ex_literis("1", piscina)));
        CREDO_VERUM(insula_ramus_attributum(&sn.ramus, "latitudo")
            != NIHIL);
        /* A6d: liber alius (dies XV): pagina sua; reditus ad XIV */
        {
                  ScribaLiber* l15;
             ScribaDocumentum* d14;
            TabulaCharacterum  folium;

            d14 = sn.doc;
            tabula_initiare(&folium, piscina, d14->latitudo,
                d14->altitudo);
            memcpy(folium.cellulae, scriba_documentum_tabula(d14)
                ->cellulae, (memoriae_index)(d14->latitudo
                * d14->altitudo));
            tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM) = 'k';
            CREDO_VERUM(scriba_documentum_committere(d14, &folium)
                > ZEPHYRUM);
            l15 = scriba_liber_aperire_in_spatio(piscina, intern, vol,
                "dies/2026-10-15", XL, XX);
            CREDO_VERUM(scriba_librum_ponere(&sn, l15));
            CREDO_VERUM(sn.doc == scriba_liber_pagina(l15,
                chorda_ex_literis("1", piscina)));
            CREDO_VERUM(sn.actiones_ctx.doc == sn.doc);
            CREDO_VERUM(tabula_cellula(&sn.actiones_ctx.laboris,
                ZEPHYRUM, ZEPHYRUM) != 'k');
            CREDO_VERUM(scriba_librum_ponere(&sn, ld));
            CREDO_VERUM(sn.doc == d14);
            CREDO_VERUM(tabula_cellula(&sn.actiones_ctx.laboris,
                ZEPHYRUM, ZEPHYRUM) == 'k');
            CREDO_FALSUM(scriba_librum_ponere(&sn, NIHIL));
        }
        /* D5: pagina nominata (scriba_paginam_ostendere): documentum,
         * plagula visus; nomen ignotum recusatur, visus manet */
        {
                       chorda  secunda;
                       chorda  visa;
             ScribaDocumentum* d2;
                          b32  inventum;

            secunda = scriba_liber_pagina_nova(ld);
            CREDO_NECESSE(secunda.mensura > ZEPHYRUM);
            d2 = scriba_liber_pagina(ld, secunda);
            CREDO_VERUM(scriba_paginam_ostendere(&sn, secunda));
            CREDO_VERUM(sn.doc == d2);
            CREDO_VERUM(sn.actiones_ctx.doc == d2);
            visa = volumen_plagulam_promere(vol, chorda_ex_literis(
                "scriba/visus/c1_notae", piscina), piscina, &inventum);
            CREDO_VERUM(inventum && chorda_aequalis(visa, secunda));
            CREDO_FALSUM(scriba_paginam_ostendere(&sn,
                chorda_ex_literis("ignota", piscina)));
            CREDO_VERUM(sn.doc == d2);
        }
    }

    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "pictor et duae scribae in repositorio uno", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("montatio", SECTIONES);
}
