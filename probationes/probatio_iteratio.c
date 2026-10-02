/* probatio_iteratio.c - iteratio plagulae notatae (eventus A6b; spec
 * D6): CRUDA contra SEMANTICA super dispositionem motam. Sessio toy
 * notatur; dispositio XV pixela deorsum movetur; cruda b1 amittit
 * (divergentia nuntiata), semantica statum finalem reddit. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "filum.h"
#include "eventus_stml.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     InsulaRepositorium* repo;
            Dispensator* d;
             ManusLudus* m;
              ToyStatus  toy;
} Sessio;

interior vacuum
sessio_creare (
                 Sessio* s,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  translatio_y)
{
    ActioRegistrum* reg;

    memset(s, ZEPHYRUM, magnitudo(Sessio));
    s->toy.translatio_y = translatio_y;
    s->repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    toy_registrare(reg, &s->toy);
    s->d = dispensator_creare(piscina, intern, s->repo, reg,
        toy_componere, &s->toy, CCC);
    s->m = manus_ludus_creare(piscina, s->d);
}

/* status finalis: insulae ambae + focus */
interior chorda
status (
     Sessio* s,
    Piscina* piscina)
{
    chorda c;

    c = chorda_concatenare(
        insula_scribere(s->repo, INSULA_DURABILIS, piscina),
        insula_scribere(s->repo, INSULA_EPHEMERA, piscina), piscina);
    redde chorda_concatenare(c, dispensator_focus(s->d), piscina);
}

interior ManusDivergentia*
divergentia (
    Xar* x,
    i32  i)
{
    redde (ManusDivergentia*)xar_obtinere(x, i);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                 Sessio  a;
                 Sessio  b;
                 chorda  fons;
                 chorda  status_a;
                    Xar* eventus;
                    Xar* notata;
                    Xar* div;
                    i32  i;

    piscina = piscina_generare_dynamicum("probatio_iteratio", LXIV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- A. sessio notata (dispositio ordinaria) ---\n");
    sessio_creare(&a, piscina, intern, ZEPHYRUM);
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
        piscina);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, piscina),
        piscina, intern);
    CREDO_NON_NIHIL(eventus);
    notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(a.d, notata);
    per (i = ZEPHYRUM; i < xar_numerus(eventus); i++)
    {
        dispensator_tractare(a.d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }
    status_a = status(&a, piscina);
    CREDO_NON_NIHIL(insula_attributum(a.repo, INSULA_EPHEMERA,
        "numerus"));

    imprimere("\n--- B. semantica, eadem dispositio ---\n");
    sessio_creare(&b, piscina, intern, ZEPHYRUM);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, notata,
        MANUS_ITERATIO_SEMANTICA, div), ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), status_a));

    imprimere("\n--- C. cruda, dispositio mota XV: b1 amissus ---\n");
    sessio_creare(&b, piscina, intern, XV);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, notata,
        MANUS_ITERATIO_CRUDA, div), II);
    CREDO_AEQUALIS_I32(xar_numerus(div), II);
    CREDO_AEQUALIS_I32(divergentia(div, ZEPHYRUM)->index, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(divergentia(div, ZEPHYRUM)
        ->scopus_notatus, "b1");
    CREDO_CHORDA_AEQUALIS_LITERIS(divergentia(div, ZEPHYRUM)
        ->scopus_crudus, "radix");
    CREDO_AEQUALIS_I32(divergentia(div, I)->index, I);
    /* ictus in b1 periit: numerus abest - status differt */
    CREDO_NIHIL(insula_attributum(b.repo, INSULA_EPHEMERA, "numerus"));
    CREDO_FALSUM(chorda_aequalis(status(&b, piscina), status_a));

    imprimere("\n--- D. semantica, mota XV: status idem ---\n");
    sessio_creare(&b, piscina, intern, XV);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    /* divergentiae eaedem NUNTIANTUR (positio cruda b1 non tangit) */
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, notata,
        MANUS_ITERATIO_SEMANTICA, div), II);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), status_a));
    CREDO_NON_NIHIL(insula_attributum(b.repo, INSULA_EPHEMERA,
        "numerus"));

    imprimere("\n--- E. divergentiae NIHIL licet ---\n");
    sessio_creare(&b, piscina, intern, XV);
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, notata,
        MANUS_ITERATIO_CRUDA, NIHIL), II);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
