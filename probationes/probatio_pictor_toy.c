/* probatio_pictor_toy.c - gradus III/IV/V super applicationem
 * ludicram: plagula eventuum -> insulae + arbor; exemplar in textu;
 * restitutio (L8). */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "filum.h"
#include "eventus_stml.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "pictor_commune.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_sectio_prima (CredoContextus* c)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                 chorda  fons;
                    Xar* eventus;
                    i32  i;
                    i32  n;
                 chorda* a;

    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(c->piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(c->piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    CREDO_NON_NIHIL(d);

    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
                              c->piscina);
    CREDO_CHORDA_NON_VACUA(fons);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, c->piscina),
        c->piscina,
                                  intern);
    CREDO_NON_NIHIL(eventus);
    n = xar_numerus(eventus);
    CREDO_AEQUALIS_I32(n, X);
    per (i = ZEPHYRUM; i < n; i++)
    {
        dispensator_tractare(d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }

    /* asserta semantica - valent etiam sine exemplaribus */
    a = insula_attributum(repo, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, "ictus_numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    a = insula_attributum(repo, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "verum");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");
    /* ictus finitus insulam ephemerarum non tetigit: pan absens */
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));
    /* quadrum 0 + unum per eventum SALTEM (limen T5: eventus cum
     * derivatis semel amplius componit) */
    CREDO_VERUM(dispensator_numerus_compositionum(d) >= n + I);
}

interior vacuum
_sectio_secunda (CredoContextus* c)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                 chorda  fons;
                    Xar* eventus;
                    i32  i;
                    i32  n;
                 chorda  arbor_textus;

    /* status post plagulam eventuum in hac sectione refectus
     * (sectiones statum communem non mutant) */
    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(c->piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(c->piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
                              c->piscina);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, c->piscina),
        c->piscina,
                                  intern);
    si (d == NIHIL || eventus == NIHIL)
    {
        redde;
    }
    n = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        dispensator_tractare(d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }

    arbor_textus = componens_scribere_stml(dispensator_arbor(d),
        c->piscina,
                                           intern, VERUM);
    CREDO_CHORDA_NON_VACUA(arbor_textus);
    CREDO_VERUM(exemplar_conferre(c->piscina,
        "probationes/pictor/toy.arbor.stml", arbor_textus));
}

interior vacuum
_sectio_tertia (CredoContextus* c)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                 chorda  fons;
                    Xar* eventus;
                    i32  i;
                    i32  n;
                 chorda  insulae_textus;

    /* status post plagulam eventuum in hac sectione refectus
     * (sectiones statum communem non mutant) */
    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(c->piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(c->piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
                              c->piscina);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, c->piscina),
        c->piscina,
                                  intern);
    si (d == NIHIL || eventus == NIHIL)
    {
        redde;
    }
    n = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        dispensator_tractare(d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }

    /* C2: insula una */
    insulae_textus = chorda_concatenare(
        insula_scribere(repo, c->piscina),
        chorda_ex_literis("\n", c->piscina), c->piscina);
    CREDO_CHORDA_NON_VACUA(insulae_textus);
    CREDO_VERUM(exemplar_conferre(c->piscina,
        "probationes/pictor/toy.insulae.stml", insulae_textus));
}

interior vacuum
_sectio_quarta (CredoContextus* c)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                 chorda  fons;
                    Xar* eventus;
                    i32  i;
                    i32  n;
                 chorda  arbor_textus;
              Componens* arbor_ante;

    /* status post plagulam eventuum in hac sectione refectus
     * (sectiones statum communem non mutant) */
    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(c->piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(c->piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
                              c->piscina);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, c->piscina),
        c->piscina,
                                  intern);
    si (d == NIHIL || eventus == NIHIL)
    {
        redde;
    }
    n = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        dispensator_tractare(d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }
    arbor_textus = componens_scribere_stml(dispensator_arbor(d),
        c->piscina,
                                           intern, VERUM);

    arbor_ante = componens_legere_stml(chorda_ut_cstr(arbor_textus,
                                       c->piscina), c->piscina, intern);
    CREDO_NON_NIHIL(arbor_ante);
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    dispensator_recomponere(d);
    CREDO_VERUM(componens_aequalis(arbor_ante, dispensator_arbor(d)));
    /* focus restitutionem supervivit */
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Gradus V: plagula eventuum -> status", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Gradus III: imago arboris logicae", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Imago insularum", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Gradus IV: restitutio (L8)", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("probatio_pictor_toy", SECTIONES);
}
