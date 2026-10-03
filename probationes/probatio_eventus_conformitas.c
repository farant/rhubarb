/* probatio_eventus_conformitas.c - tabula conformitatis (eventus A4):
 * lectio tabulae; comparatio - ordo et numerus exacti, attributa
 * expectata subset, genera non considerata omissa, tempus neglectum.
 * (B4) modificantes in bits vocabularii; excusationes per FACULTATES
 * fluxus. Sine fenestra: fluxus actuales hic fabricantur. */
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

/* B4: scaenae cum excusationibus */
interior constans character* TABULA_EXCUSATIONUM =
    "<conformitas>"
    "<scaena titulus=\"shift-a\" "
    "genera=\"clavis_depressus clavis_liberatus textus\" "
    "excusationes=\"liberationes codex_physicus latera "
    "modificantes_textus\">"
    "<expectata>"
    "<eventus genus=\"clavis_depressus\" codex=\"KeyA\" runa=\"97\" "
    "modificantes=\"131074\"/>"
    "<eventus genus=\"textus\" contentum=\"A\"/>"
    "<eventus genus=\"clavis_liberatus\" codex=\"KeyA\" "
    "actio=\"soluta\"/>"
    "</expectata>"
    "</scaena>"
    "<scaena titulus=\"latera\" "
    "genera=\"clavis_depressus clavis_liberatus textus\" "
    "excusationes=\"latera\">"
    "<expectata>"
    "<eventus genus=\"clavis_depressus\" codex=\"KeyA\" "
    "modificantes=\"131074\"/>"
    "<eventus genus=\"textus\" contentum=\"A\"/>"
    "<eventus genus=\"clavis_liberatus\" codex=\"KeyA\"/>"
    "</expectata>"
    "</scaena>"
    "<scaena titulus=\"sagitta\" genera=\"clavis_depressus\">"
    "<expectata>"
    "<eventus genus=\"clavis_depressus\" codex=\"ArrowLeft\" "
    "modificantes=\"8519682\"/>"
    "</expectata>"
    "</scaena>"
    "<scaena titulus=\"ctrl-i\" genera=\"clavis_depressus\" "
    "excusationes=\"tabula_distincta\">"
    "<expectata>"
    "<eventus genus=\"clavis_depressus\" codex=\"KeyI\"/>"
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

interior vacuum
_modos_ponere (
    Xar* x,
    i32  modificantes)
{
    Eventus* e = (Eventus*)xar_obtinere(x, xar_numerus(x) - I);

    e->datum.clavis.modificantes = modificantes;
}

/* FACULTATES: omnes eaedem (legacy FALSUM, fenestra VERUM) praeter
 * latera */
interior vacuum
_facultates_addere (
    Xar* x,
    b32  omnes,
    b32  latera)
{
    Eventus* e = (Eventus*)xar_addere(x);

    memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus                                 = EVENTUS_FACULTATES;
    e->datum.facultates.liberationes         = omnes;
    e->datum.facultates.codex_physicus       = omnes;
    e->datum.facultates.tabula_distincta     = omnes;
    e->datum.facultates.modificantes_textus  = omnes;
    e->datum.facultates.latera               = latera;
}

/* fluxus legacy scaenae shift-a: sine liberatione, codex ignotus,
 * Shift invisibilis */
interior vacuum
_fluxum_vetustum_addere (
        Xar* x,
    Piscina* piscina)
{
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_IGNOTUS,
        EVENTUS_ACTIO_PRESSA, V);
    _textum_addere(x, "A", piscina);
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
                    Xar* excusationum;
      ConformitasScaena* sc;
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
    CREDO_VERUM (CONFORMITAS_CONFORMIS
        == eventus_conformitas_comparare(shift, x, piscina,
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
    CREDO_VERUM (CONFORMITAS_FRACTA
        == eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));
    CREDO_VERUM (diagnosis.mensura > ZEPHYRUM);

    imprimere("\n--- IV. eventus deest: vitium ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_PRESSA, V);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_VERUM (CONFORMITAS_FRACTA
        == eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));

    imprimere("\n--- V. eventus consideratus superfluus: vitium ---\n");
    x = _fluxus_bonus(piscina);
    _textum_addere(x, "B", piscina);
    CREDO_VERUM (CONFORMITAS_FRACTA
        == eventus_conformitas_comparare(shift, x, piscina,
        intern, &diagnosis));

    imprimere("\n--- VI. attributum differt: vitium nominatum ---\n");
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS,
        (EventusCodex)(EVENTUS_CODEX_LITTERAE + I),
        EVENTUS_ACTIO_PRESSA, V);
    _textum_addere(x, "A", piscina);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_VERUM (CONFORMITAS_FRACTA
        == eventus_conformitas_comparare(shift, x, piscina,
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
    CREDO_VERUM (CONFORMITAS_CONFORMIS
        == eventus_conformitas_comparare(ictus, x, piscina,
        intern,
        &diagnosis));

    imprimere("\n--- VIII. excusationes legere (B4) ---\n");
    excusationum = eventus_conformitas_legere(TABULA_EXCUSATIONUM,
        piscina, intern);
    CREDO_NON_NIHIL (excusationum);
    si (excusationum == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    sc = (ConformitasScaena*)xar_obtinere(excusationum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sc->excusationes,
        CONFORMITAS_EXCUSATIO_LIBERATIONES
        | CONFORMITAS_EXCUSATIO_CODEX_PHYSICUS
        | CONFORMITAS_EXCUSATIO_LATERA
        | CONFORMITAS_EXCUSATIO_MODIFICANTES_TEXTUS);
    CREDO_AEQUALIS_I32 (((ConformitasScaena*)xar_obtinere(excusationum,
        II))->excusationes, ZEPHYRUM);
    /* facultas ignota: tabula prava */
    CREDO_NIHIL (eventus_conformitas_legere(
        "<conformitas><scaena titulus=\"t\" genera=\"textus\" "
        "excusationes=\"nemo\"/></conformitas>", piscina, intern));

    imprimere("\n--- IX. modificantes: bits vocabularii solum ---\n");
    sc  = (ConformitasScaena*)xar_obtinere(excusationum, II);
    x   = xar_creare(piscina, (i32)magnitudo(Eventus));
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS,
        EVENTUS_CODEX_SAGITTA_SINISTRA, EVENTUS_ACTIO_PRESSA, V);
    /* fenestra 0x820002 (Function AppKit), terminalis sine 0x800000 */
    _modos_ponere(x, MOD_SHIFT | MOD_SHIFT_SINISTER);
    CREDO_VERUM (CONFORMITAS_CONFORMIS == eventus_conformitas_comparare(
        sc, x, piscina, intern, &diagnosis));
    _modos_ponere(x, MOD_SHIFT);       /* latus deest, non excusatum */
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));

    imprimere("\n--- X. excusationes SOLUM facultate negata ---\n");
    sc  = (ConformitasScaena*)xar_obtinere(excusationum, ZEPHYRUM);
    x   = xar_creare(piscina, (i32)magnitudo(Eventus));
    _facultates_addere(x, FALSUM, FALSUM);
    _fluxum_vetustum_addere(x, piscina);
    CREDO_VERUM (CONFORMITAS_CONFORMIS == eventus_conformitas_comparare(
        sc, x, piscina, intern, &diagnosis));
    /* sine FACULTATIBUS: nihil excusatur */
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _fluxum_vetustum_addere(x, piscina);
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));
    /* facultates affirmatae: nihil excusatur */
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _facultates_addere(x, VERUM, VERUM);
    _fluxum_vetustum_addere(x, piscina);
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));
    /* FACULTATES ULTIMAE valent */
    x = xar_creare(piscina, (i32)magnitudo(Eventus));
    _facultates_addere(x, FALSUM, FALSUM);
    _facultates_addere(x, VERUM, VERUM);
    _fluxum_vetustum_addere(x, piscina);
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));

    imprimere("\n--- XI. latera sola (kitty: Shift sine latere) ---\n");
    sc  = (ConformitasScaena*)xar_obtinere(excusationum, I);
    x   = xar_creare(piscina, (i32)magnitudo(Eventus));
    _facultates_addere(x, VERUM, FALSUM);
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_PRESSA, V);
    _modos_ponere(x, MOD_SHIFT);
    _textum_addere(x, "A", piscina);
    _clavem_addere(x, EVENTUS_CLAVIS_LIBERATUS, EVENTUS_CODEX_LITTERAE,
        EVENTUS_ACTIO_SOLUTA, VI);
    CREDO_VERUM (CONFORMITAS_CONFORMIS == eventus_conformitas_comparare(
        sc, x, piscina, intern, &diagnosis));
    ((Eventus*)xar_obtinere(x, ZEPHYRUM))->datum.facultates.latera =
        VERUM;
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));

    imprimere("\n--- XII. tabula_distincta: scaena EXCUSATA ---\n");
    sc  = (ConformitasScaena*)xar_obtinere(excusationum, III);
    x   = xar_creare(piscina, (i32)magnitudo(Eventus));
    _facultates_addere(x, FALSUM, FALSUM);
    _clavem_addere(x, EVENTUS_CLAVIS_DEPRESSUS, EVENTUS_CODEX_TABULA,
        EVENTUS_ACTIO_PRESSA, V);
    CREDO_VERUM (CONFORMITAS_EXCUSATA == eventus_conformitas_comparare(
        sc, x, piscina, intern, &diagnosis));
    ((Eventus*)xar_obtinere(x, ZEPHYRUM))->datum.facultates
        .tabula_distincta = VERUM;
    CREDO_VERUM (CONFORMITAS_FRACTA == eventus_conformitas_comparare(sc,
        x, piscina, intern, &diagnosis));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
