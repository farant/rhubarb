/* probatio_notarius.c - notarius dispensatoris (eventus A6a; spec
 * D6): eventus crudus + destinatum (scopus, punctum locale) notatur;
 * plagula notata circuitum servat; sine scopis = plagula simplex. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "filum.h"
#include "eventus_stml.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior EventusNotatum*
notatum (
    Xar* notata,
    i32  i)
{
    redde (EventusNotatum*)xar_obtinere(notata, i);
}

interior Eventus
mus (
    eventus_genus_t genus,
                s64 tempus,
                s32 x,
                s32 y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.tempus            = tempus;
    e.datum.mus.x       = x;
    e.datum.mus.y       = y;
    e.datum.mus.botton  = MUS_SINISTER;
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
                 chorda  fons;
                    Xar* eventus;
                    Xar* notata;
                    Xar* relecta;
                    Xar* nuda;
                 chorda  scriptum;
                 chorda  rescriptum;
                    i32  i;
                Eventus  e;

    piscina = piscina_generare_dynamicum("probatio_notarius", LXIV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
        &toy, CCC);
    CREDO_NON_NIHIL(d);

    imprimere("\n--- I. toy notatus: omnia eventa, scopi ---\n");
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
        piscina);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, piscina),
        piscina, intern);
    CREDO_NON_NIHIL(eventus);
    notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(d, notata);
    per (i = ZEPHYRUM; i < xar_numerus(eventus); i++)
    {
        dispensator_tractare(d,
            (constans Eventus*)xar_obtinere(eventus, i));
    }
    /* pulsus (nihil) quoque notatur: quies ex eo pendet */
    CREDO_AEQUALIS_I32(xar_numerus(notata), xar_numerus(eventus));
    /* ictus 35,20 in b1 (10,10,50,20) */
    CREDO_CHORDA_AEQUALIS_LITERIS(notatum(notata, ZEPHYRUM)->scopus,
        "b1");
    CREDO_AEQUALIS_S32(notatum(notata, ZEPHYRUM)->scopus_x, XXV);
    CREDO_AEQUALIS_S32(notatum(notata, ZEPHYRUM)->scopus_y, X);
    /* ictus 120,50 in tabula (70,10,100,80) */
    CREDO_CHORDA_AEQUALIS_LITERIS(notatum(notata, III)->scopus,
        "tabula");
    CREDO_AEQUALIS_S32(notatum(notata, III)->scopus_x, L);
    CREDO_AEQUALIS_S32(notatum(notata, III)->scopus_y, XL);
    /* clavis (Effugium): sine scopo */
    CREDO_VERUM(notatum(notata, VII)->eventus.genus
        == EVENTUS_CLAVIS_DEPRESSUS);
    CREDO_AEQUALIS_I32(notatum(notata, VII)->scopus.mensura, ZEPHYRUM);

    imprimere("\n--- II. sine scopis = plagula manu scripta ---\n");
    nuda = xar_creare(piscina, (i32)magnitudo(Eventus));
    per (i = ZEPHYRUM; i < xar_numerus(notata); i++)
    {
        *(Eventus*)xar_addere(nuda) = notatum(notata, i)->eventus;
    }
    CREDO_VERUM(chorda_aequalis(eventus_scribere_stml(nuda, piscina,
        intern, VERUM), fons));

    imprimere("\n--- III. circuitus plagulae notatae ---\n");
    scriptum = eventus_notata_scribere_stml(notata, piscina, intern,
        VERUM);
    relecta  = eventus_notata_legere_stml(chorda_ut_cstr(scriptum,
        piscina), piscina, intern);
    CREDO_NON_NIHIL(relecta);
    CREDO_AEQUALIS_I32(xar_numerus(relecta), xar_numerus(notata));
    CREDO_CHORDA_AEQUALIS_LITERIS(notatum(relecta, III)->scopus,
        "tabula");
    CREDO_AEQUALIS_S32(notatum(relecta, III)->scopus_y, XL);
    rescriptum = eventus_notata_scribere_stml(relecta, piscina, intern,
        VERUM);
    CREDO_VERUM(chorda_aequalis(scriptum, rescriptum));
    /* plagula simplex lecta ut notata: scopi vacui */
    relecta = eventus_notata_legere_stml(chorda_ut_cstr(fons, piscina),
        piscina, intern);
    CREDO_AEQUALIS_I32(notatum(relecta, ZEPHYRUM)->scopus.mensura,
        ZEPHYRUM);

    imprimere("\n--- IV. captura: scopus captus, locale < 0 ---\n");
    xar_vacare(notata);
    e = mus(EVENTUS_MUS_DEPRESSUS, X * M, CXX, L);
    dispensator_tractare(d, &e);
    /* extra tabulam (70..170), in radice: captura tenet */
    e = mus(EVENTUS_MUS_MOTUS, X * M + XX, LXV, L);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(xar_numerus(notata), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(notatum(notata, I)->scopus, "tabula");
    CREDO_AEQUALIS_S32(notatum(notata, I)->scopus_x, -V);
    CREDO_AEQUALIS_S32(notatum(notata, I)->scopus_y, XL);
    e = mus(EVENTUS_MUS_LIBERATUS, X * M + XL, LXV, L);
    dispensator_tractare(d, &e);

    imprimere("\n--- V. notarius tollitur ---\n");
    dispensator_notarium_ponere(d, NIHIL);
    e = mus(EVENTUS_MUS_MOTUS, XX * M, V, V);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(xar_numerus(notata), III);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
