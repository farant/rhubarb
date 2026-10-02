/* probatio_eventus_conformitas.c - tabula conformitatis (eventus A4):
 * lectio tabulae; comparatio - ordo et numerus exacti, attributa
 * expectata subset, genera non considerata omissa, tempus neglectum.
 * Sine fenestra: fluxus actuales hic fabricantur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "eventus.h"
#include "eventus_conformitas.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior constans character* TABULA =
    "<conformitas>"
    "<scaena titulus=\"shift-a\" "
    "genera=\"clavis_depressus clavis_liberatus textus\">"
    "<immissio>"
    "<clavis codex=\"0\" modificantes=\"131074\" characteres=\"A\" "
    "depressa=\"1\"/>"
    "<clavis codex=\"0\" modificantes=\"131074\" characteres=\"A\" "
    "depressa=\"0\"/>"
    "</immissio>"
    "<expectata>"
    "<eventus genus=\"clavis_depressus\" codex=\"KeyA\" runa=\"97\" "
    "tempus=\"999\"/>"
    "<eventus genus=\"textus\" contentum=\"A\"/>"
    "<eventus genus=\"clavis_liberatus\" codex=\"KeyA\" "
    "actio=\"soluta\"/>"
    "</expectata>"
    "</scaena>"
    "<scaena titulus=\"ictus\" genera=\"mus_depressus mus_liberatus\">"
    "<immissio>"
    "<mus genus=\"depressio\" x=\"10\" y=\"-20\"/>"
    "</immissio>"
    "<expectata>"
    "<eventus genus=\"mus_depressus\" x=\"10\" y=\"-20\"/>"
    "</expectata>"
    "</scaena>"
    "</conformitas>";

interior vacuum
_clavem_addere (
                Xar* x,
    eventus_genus_t  genus,
       EventusCodex  codex,
       EventusActio  actio,
                s64  tempus)
{
    Eventus* e = (Eventus*)xar_addere(x);

    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus                = genus;
    e->tempus               = tempus;
    e->datum.clavis.clavis  = (clavis_t)'A';
    e->datum.clavis.codex   = codex;
    e->datum.clavis.runa    = 'a';
    e->datum.clavis.actio   = actio;
}

interior vacuum
_textum_addere (
                   Xar* x,
    constans character* t,
               Piscina* piscina)
{
    Eventus* e = (Eventus*)xar_addere(x);

    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus                   = EVENTUS_TEXTUS;
    e->tempus                  = V;
    e->datum.textus.contentum  = chorda_ex_literis(t, piscina);
}

interior vacuum
_motum_addere (
    Xar* x)
{
    Eventus* e = (Eventus*)xar_addere(x);

    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus        = EVENTUS_MUS_MOTUS;
    e->datum.mus.x  = CCC;
    e->datum.mus.y  = CCC;
}

/* fluxus conformis scaenae shift-a, cum motu non considerato */
interior Xar*
_fluxus_bonus (
    Piscina* piscina)
{
    Xar* x = xar_creare(piscina, (i32)magnitudo(Eventus));

    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_PRESSA, V);
    _motum_addere(x);
    _textum_addere(x, "A", piscina);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    redde x;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    Xar* tabula;
      ConformitasScaena* shift;
      ConformitasScaena* ictus;
    ConformitasImmissio* im;
                    Xar* x;
                 chorda  diagnosis;

    piscina = piscina_generare_dynamicum("probatio_eventus_conformitas",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- I. tabulam legere ---\n");
    tabula = eventus_conformitas_legere(TABULA, piscina, intern);
    CREDO_NON_NIHIL (tabula);
    si (tabula == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_AEQUALIS_I32 (xar_numerus(tabula), II);
    shift = (ConformitasScaena*)xar_obtinere(tabula, ZEPHYRUM);
    ictus = (ConformitasScaena*)xar_obtinere(tabula, I);
    CREDO_VERUM (chorda_aequalis_literis(shift->titulus, "shift-a"));
    CREDO_AEQUALIS_I32 (xar_numerus(shift->genera), III);
    CREDO_AEQUALIS_I32 (xar_numerus(shift->immissiones), II);
    CREDO_AEQUALIS_I32 (xar_numerus(shift->expectata), III);
    im = (ConformitasImmissio*)xar_obtinere(shift->immissiones, I);
    CREDO_VERUM (im->genus == CONFORMITAS_IMMISSIO_CLAVIS);
    CREDO_AEQUALIS_S32 (im->codex, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (im->modificantes, 131074);
    CREDO_VERUM (chorda_aequalis_literis(im->characteres, "A"));
    CREDO_FALSUM (im->depressa);
    im = (ConformitasImmissio*)xar_obtinere(ictus->immissiones,
        ZEPHYRUM);
    CREDO_VERUM (im->genus == CONFORMITAS_IMMISSIO_MUS);
    CREDO_VERUM (chorda_aequalis_literis(im->mus_genus, "depressio"));
    CREDO_AEQUALIS_S32 (im->y, -XX);
    /* genus ignotum in 'genera': tabula prava */
    CREDO_NIHIL (eventus_conformitas_legere(
        "<conformitas><scaena titulus=\"t\" genera=\"nemo\"/>"
        "</conformitas>", piscina, intern));

    imprimere("\n--- II. conformis: motus omissus, tempus neglectum\n");
    x = _fluxus_bonus(piscina);
    CREDO_VERUM (eventus_conformitas_comparare(shift, x, piscina,
        intern,
        &diagnosis));
    CREDO_AEQUALIS_I32 (diagnosis.mensura, ZEPHYRUM);

    imprimere("\n--- III. ordo permutatus: vitium ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _textum_addere(x, "A", piscina);
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_PRESSA, V);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_FALSUM (eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));
    CREDO_VERUM (diagnosis.mensura > ZEPHYRUM);

    imprimere("\n--- IV. eventus deest: vitium ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_PRESSA, V);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_FALSUM (eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));

    imprimere("\n--- V. eventus consideratus superfluus: vitium ---\n");
    x = _fluxus_bonus(piscina);
    _textum_addere(x, "B", piscina);
    CREDO_FALSUM (eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));

    imprimere("\n--- VI. attributum differt: vitium nominatum ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS,
        (EventusCodex)(EVENTUS_CODEX_LITTERAE + I),
        EVENTUS_ACTIO_PRESSA, V);
    _textum_addere(x, "A", piscina);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_FALSUM (eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));
    CREDO_VERUM (chorda_continet(diagnosis,
        chorda_ex_literis("codex", piscina)));
    CREDO_VERUM (chorda_continet(diagnosis,
        chorda_ex_literis("KeyB", piscina)));

    imprimere("\n--- VII. positio negativa ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    {
        Eventus* e = (Eventus*)xar_addere(x);

        memset(e, ZEPHYRUM, magnitudo(Eventus));
        e->genus        = EVENTUS_MUS_DEPRESSUS;
        e->datum.mus.x  = X;
        e->datum.mus.y  = -XX;
    }
    _motum_addere(x);
    CREDO_VERUM (eventus_conformitas_comparare(ictus, x, piscina,
        intern,
        &diagnosis));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
