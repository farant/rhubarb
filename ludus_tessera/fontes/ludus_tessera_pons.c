/* ludus_tessera_pons.c - Pons tesserae super terminalis; octeti
 * modorum (A1). Ratio in capite. */

#include "ludus_tessera_pons.h"

#include <string.h>

/* Octeti scrinii (ordo tessera_modi.h, quod hic includi nequit -
 * caput internum tesserae; probatio ordinem figit) */
#define SCRINIUM_INTRANDI  "\033[?1049h"
#define QUADRUM_CLAUDENDI  "\033[?2026l"
#define SCRINIUM_EXEUNDI   "\033[?1049l\033[0m\033[?25h"

interior vacuum
_appendere (
                    i8* buffer,
                   i32* mensura,
    constans character* literae)
{
    i32 n = (i32)strlen(literae);

    memcpy(buffer + *mensura, literae, (memoriae_index)n);
    *mensura += n;
}

b32
ludus_tessera_modos_componere (
    RivusTerminalis* rivus,
                i32  modi,
                 i8* intrandi,
                i32* mensura_intrandi,
                 i8* exeundi,
                i32* mensura_exeundi)
{
     i8 exitus_rivi[RIVUS_MODI_MAXIMUM];
    i32 n;
    i32 m;

    si (   !rivus || !intrandi || !mensura_intrandi || !exeundi
        || !mensura_exeundi)
    {
        redde FALSUM;
    }
    *mensura_intrandi  = ZEPHYRUM;
    *mensura_exeundi   = ZEPHYRUM;

    _appendere(intrandi, mensura_intrandi, SCRINIUM_INTRANDI);
    n = rivus_modos_intrare(rivus, modi, intrandi + *mensura_intrandi,
        LUDUS_TESSERA_MODI_MAXIMI - *mensura_intrandi);
    si (modi != ZEPHYRUM && n == ZEPHYRUM)
    {
        /* modi iam intrati: rivus nihil declaravit */
        *mensura_intrandi = ZEPHYRUM;
        redde FALSUM;
    }
    *mensura_intrandi += n;

    m = rivus_modos_exeundi(rivus, exitus_rivi, RIVUS_MODI_MAXIMUM);
    _appendere(exeundi, mensura_exeundi, QUADRUM_CLAUDENDI);
    memcpy(exeundi + *mensura_exeundi, exitus_rivi, (memoriae_index)m);
    *mensura_exeundi += m;
    _appendere(exeundi, mensura_exeundi, SCRINIUM_EXEUNDI);
    redde VERUM;
}


/* ==================================================
 * Pons
 * ================================================== */

/* Lector tesserae in hac via non adhibetur (Eventus a rivo): -1 =
 * "terminalis abiit" - usus erroneus clamat, non pendet */
interior s32
_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    (vacuum)datum;
    (vacuum)buffer;
    (vacuum)capacitas;
    (vacuum)mora_ms;
    redde -I;
}

interior b32
_scribere (
         vacuum* datum,
    constans i8* octeti,
            i32  numerus)
{
    (vacuum)datum;
    redde terminalis_scribere(octeti, numerus);
}

interior b32
_amplitudo (
    vacuum* datum,
       i32* latitudo,
       i32* altitudo)
{
    TerminalisAmplitudo a;

    (vacuum)datum;
    si (   !terminalis_amplitudo(&a) || a.columnae <= ZEPHYRUM
        || a.lineae <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    *latitudo = (i32)a.columnae;
    *altitudo = (i32)a.lineae;
    redde VERUM;
}

/* Intrare/egredi: terminalis (terminalis_intrare/exire) modum crudum
 * et octetos modorum iam tenet - hic nihil scribitur */
interior b32
_nihil_agere (
    vacuum* datum)
{
    (vacuum)datum;
    redde VERUM;
}

vacuum
ludus_tessera_pontem_initiare (
    TesseraPons* pons)
{
    si (!pons)
    {
        redde;
    }
    pons->datum      = NIHIL;
    pons->legere     = _legere;
    pons->scribere   = _scribere;
    pons->amplitudo  = _amplitudo;
    pons->intrare    = _nihil_agere;
    pons->egredi     = _nihil_agere;
    pons->resumptum  = NIHIL;
}
