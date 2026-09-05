/* probatio_eventus_stml.c - Eventus[] <-> STML (plagulae replay) */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "eventus_stml.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus*
addere (
                Xar* index,
    eventus_genus_t  genus,
                s64  tempus)
{
    Eventus* e;
    e = (Eventus*)xar_addere(index);
    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus   = genus;
    e->tempus  = tempus;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    Xar* index;
                    Xar* index2;
                Eventus* e;
                 chorda  textus;

    piscina = piscina_generare_dynamicum("probatio_eventus_stml",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans tituli generum ---\n");
    CREDO_VERUM (eventus_genus_ex_titulo("mus_depressus")
        == EVENTUS_MUS_DEPRESSUS);
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_CLAVIS_LIBERATUS),
                        "clavis_liberatus") == ZEPHYRUM);
    CREDO_VERUM (eventus_genus_ex_titulo("ignotissimum")
        == EVENTUS_NIHIL);

    imprimere("\n--- Probans circuitum ---\n");
    index = xar_creare(piscina, (i32)magnitudo(Eventus));
    e = addere(index, EVENTUS_MUS_DEPRESSUS, M);
    e->datum.mus.x = X;
    e->datum.mus.y = XX;
    e->datum.mus.modificantes = II;
    e = addere(index, EVENTUS_CLAVIS_DEPRESSUS, M + L);
    e->datum.clavis.typus = 'p';
    e = addere(index, EVENTUS_MUS_ROTULA, M + C);
    e->datum.rotula.delta_y = -1.5f;
    e = addere(index, EVENTUS_MUTARE_MAGNITUDINEM, M + CC);
    e->datum.mutare_magnitudinem.latitudo = CDLXXX;
    e->datum.mutare_magnitudinem.altitudo = CCC;
    e = addere(index, EVENTUS_NIHIL, XL * M * M);   /* tempus > s32 */

    textus = eventus_scribere_stml(index, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    index2 = eventus_legere_stml(chorda_ut_cstr(textus, piscina),
        piscina, intern);
    CREDO_NON_NIHIL (index2);
    CREDO_AEQUALIS_I32 (xar_numerus(index2), V);
    e = (Eventus*)xar_obtinere(index2, ZEPHYRUM);
    CREDO_VERUM (e->genus == EVENTUS_MUS_DEPRESSUS);
    CREDO_VERUM (e->tempus == M);
    CREDO_AEQUALIS_I32 (e->datum.mus.x, X);
    CREDO_AEQUALIS_I32 (e->datum.mus.modificantes, II);
    e = (Eventus*)xar_obtinere(index2, I);
    CREDO_VERUM (e->genus == EVENTUS_CLAVIS_DEPRESSUS);
    CREDO_VERUM (e->datum.clavis.typus == 'p');
    e = (Eventus*)xar_obtinere(index2, II);
    CREDO_VERUM (e->datum.rotula.delta_y < -1.4f);
    CREDO_VERUM (e->datum.rotula.delta_y > -1.6f);
    e = (Eventus*)xar_obtinere(index2, III);
    CREDO_AEQUALIS_I32 (e->datum.mutare_magnitudinem.latitudo, CDLXXX);
    e = (Eventus*)xar_obtinere(index2, IV);
    CREDO_VERUM (e->tempus == XL * M * M);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
