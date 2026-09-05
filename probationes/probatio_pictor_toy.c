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

s32 principale (vacuum)
{
                Piscina* piscina;
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
                 chorda  insulae_textus;
              Componens* arbor_ante;
                 chorda* a;

    piscina = piscina_generare_dynamicum("probatio_pictor_toy",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    toy.compositiones = ZEPHYRUM;
    toy.derivata = ZEPHYRUM;
    toy.compositiones_in_traditione = ZEPHYRUM;
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    CREDO_NON_NIHIL(d);

    imprimere("\n--- Gradus V: plagula eventuum -> status ---\n");
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
                              piscina);
    CREDO_CHORDA_NON_VACUA(fons);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, piscina),
        piscina,
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
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "ictus_numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "verum");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");
    /* ictus finitus insulam ephemerarum non tetigit: pan absens */
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    /* quadrum 0 + unum per eventum SALTEM (limen T5: eventus cum
     * derivatis semel amplius componit) */
    CREDO_VERUM(dispensator_numerus_compositionum(d) >= n + I);

    imprimere("\n--- Gradus III: imago arboris logicae ---\n");
    arbor_textus = componens_scribere_stml(dispensator_arbor(d),
        piscina,
                                           intern, VERUM);
    CREDO_CHORDA_NON_VACUA(arbor_textus);
    CREDO_VERUM(exemplar_conferre(piscina,
        "probationes/pictor/toy.arbor.stml", arbor_textus));

    imprimere("\n--- Imago insularum ---\n");
    insulae_textus = chorda_concatenare(
        insula_scribere(repo, INSULA_DURABILIS, piscina),
        chorda_ex_literis("\n", piscina), piscina);
    insulae_textus = chorda_concatenare(insulae_textus,
        insula_scribere(repo, INSULA_EPHEMERA, piscina), piscina);
    insulae_textus = chorda_concatenare(insulae_textus,
        chorda_ex_literis("\n", piscina), piscina);
    CREDO_CHORDA_NON_VACUA(insulae_textus);
    CREDO_VERUM(exemplar_conferre(piscina,
        "probationes/pictor/toy.insulae.stml", insulae_textus));

    imprimere("\n--- Gradus IV: restitutio (L8) ---\n");
    arbor_ante = componens_legere_stml(chorda_ut_cstr(arbor_textus,
                                       piscina), piscina, intern);
    CREDO_NON_NIHIL(arbor_ante);
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    dispensator_recomponere(d);
    CREDO_VERUM(componens_aequalis(arbor_ante, dispensator_arbor(d)));
    /* focus restitutionem supervivit */
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
