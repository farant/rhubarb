/* probatio_dispensator.c - ansa: derivare -> destinatio -> actio ->
 * componere; focus in insula; super derivatum; sedes quietis */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
mus (
    eventus_genus_t genus,
                s64 t,
                s32 x,
                s32 y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = genus;
    e.tempus       = t;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
}

interior Eventus
clavis (
          s64 t,
    character typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus               = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus              = t;
    e.datum.clavis.typus  = typus;
    /* clavis logica: clavis_t ASCII ipsum fert (Tab IX, Effugium
     * XXVII) - ut fenestra */
    e.datum.clavis.clavis = (clavis_t)typus;
    redde e;
}

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
          ActioRegistrum* reg;
               ToyStatus  toy;
             Dispensator* d;
                 Eventus  e;
                  chorda* a;
                  chorda  vacua_focus;
                     i32  n0;

    piscina = piscina_generare_dynamicum("probatio_dispensator",
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
    /* quadrum 0 ante initum */
    CREDO_NON_NIHIL(dispensator_arbor(d));
    CREDO_AEQUALIS_I32(toy.compositiones, I);
    CREDO_AEQUALIS_I32(dispensator_numerus_compositionum(d), I);
    CREDO_VERUM(chorda_vacua(dispensator_focus(d)));
    CREDO_VERUM(chorda_vacua(dispensator_super(d)));

    imprimere("\n--- Ictus in b1: actio, focus, recompositio ---\n");
    n0     = toy.compositiones;
        e  = mus(EVENTUS_MUS_DEPRESSUS, M, XXXV, XX);
    dispensator_tractare(d, &e);
    /* focus_captus ad b1 TRADITUS post recompositionem, contra
     * arborem novam: compositiones in traditione > ante (T5) */
    CREDO_AEQUALIS_I32(toy.derivata, I);
    CREDO_VERUM(toy.compositiones_in_traditione > n0);
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* regula staleness: post omnem eventum componitur */
    CREDO_VERUM(toy.compositiones > n0);
    e = mus(EVENTUS_MUS_LIBERATUS, M + L, XXXV, XX);
    dispensator_tractare(d, &e);
    /* alter ictus: incrementare SUBSTITUIT, non geminat */
    e = mus(EVENTUS_MUS_DEPRESSUS, M + DC, XXXV, XX);
    dispensator_tractare(d, &e);
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "2");
    e = mus(EVENTUS_MUS_LIBERATUS, M + DC + L, XXXV, XX);
    dispensator_tractare(d, &e);

    imprimere("\n--- Motus super tabulam: super derivatur ---\n");
    e = mus(EVENTUS_MUS_MOTUS, M + DCC, CXX, L);
        dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");
    CREDO_AEQUALIS_I32(toy.derivata, II);        /* intravit tabula */
    e = mus(EVENTUS_MUS_MOTUS, M + DCC + X, V, XC);
        dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "radix");
    CREDO_AEQUALIS_I32(toy.derivata, IV);        /* exiit, intravit */

    imprimere("\n--- Ictus in tabula: captura, motus, UNA scriptura"
              " durabilis ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, II * M, CXX, L);
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_motus(d)->captura,
                                  "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    e = mus(EVENTUS_MUS_MOTUS, II * M + XX, CXXV, LV);
    dispensator_tractare(d, &e);
    e = mus(EVENTUS_MUS_MOTUS, II * M + XL, CXXX, LX);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       III);
    /* nondum scriptum */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_DURABILIS), ZEPHYRUM);
    e = mus(EVENTUS_MUS_LIBERATUS, II * M + C, CXXX, LX);
    dispensator_tractare(d, &e);
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    a = insula_attributum(repo, INSULA_DURABILIS, "ictus_numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    /* ephemera intacta ab ictu */
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));

    imprimere("\n--- Clavis ESC ascendit ad radicem ---\n");
    e = clavis(II * M + CC, (character)XXVII);
    dispensator_tractare(d, &e);
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "verum");

    imprimere("\n--- Tab: focus proximum, involvens ---\n");
    e = clavis(II * M + CCC, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    e = clavis(II * M + CCCX, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    e = clavis(II * M + CCCXX, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    imprimere("\n--- A5: Tab per clavem logicam, Shift verum ---\n");
    /* Ctrl+I: characteres '\t' (typus) sed clavis 'I' - focus NON
     * movetur (Tab != Ctrl+I) */
    e                            = clavis(II * M + CD, '\t');
    e.datum.clavis.clavis        = (clavis_t)'I';
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_IMPERIUM_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* directio: DUO focusabiles (b1, tabula) - ex foco b1 et ante et
     * retro 'tabula' est. Ex foco NULLO directio videtur: ante ->
     * primus (b1), retro -> ultimus (tabula). */
    /* Shift+Tab cum vexillis veris macOS (0x20002) -> retro */
    vacua_focus.datum    = NIHIL;
    vacua_focus.mensura  = ZEPHYRUM;
    dispensator_focus_ponere(d, vacua_focus);
    e                            = clavis(II * M + CDX, '\t');
    e.datum.clavis.modificantes  = MOD_SHIFT | MOD_SHIFT_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    /* Ctrl+Tab (0x40001): ante, NON retro - bitus 0x1 latus Ctrl est */
    dispensator_focus_ponere(d, vacua_focus);
    e                            = clavis(II * M + CDXX, '\t');
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_IMPERIUM_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    imprimere("\n--- Acervus foci ---\n");
    dispensator_focus_impellere(d, chorda_ex_literis("tabula",
        piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    dispensator_focus_impellere(d, chorda_ex_literis("b1", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* acervus vacuus: extrahere nihil mutat */
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    imprimere("\n--- Quies: pulsus post CCC ms effundit motus ---\n");
    dispensator_motus(d)->pan.x                      = VII;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = III * M;
    dispensator_pulsare(d, III * M + CC);
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    dispensator_pulsare(d, III * M + CD);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "7");
    CREDO_FALSUM(dispensator_motus(d)->sordida);

    imprimere("\n--- Restitutio honesta post omnia; focus"
              " supervivit ---\n");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
