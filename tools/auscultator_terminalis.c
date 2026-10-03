/* auscultator_terminalis.c - Eventus terminalis ut STML (eventus
 * B3b-iii; aspectus Frani in Terminal.app et Ghostty)
 *
 * Gemellus tools/auscultator.c (fenestra): modos OMNES declarat (mus,
 * super, glutinum, focus, kitty, depositio), octetos per terminalis_*
 * legit, per rivus_terminalis decodificat (lectio coalita) et omnem
 * Eventum ut lineam STML unam scribit - forma eadem ac fenestrae, ut
 * fontes comparari possint. Exitus: Ctrl-C (clavis in modo crudo).
 * Ctrl-Z suspendit; post fg RESUMPTIO scribitur.
 *
 * Curre per: ./tools/auscultator_terminalis.sh
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "eventus.h"
#include "eventus_stml.h"
#include "rivus_terminalis.h"
#include "terminalis.h"
#include "fenestra_tempus.h"
#include <stdio.h>
#include <string.h>

#define LECTIO CCLVI

#define MODI_OMNES (RIVUS_MODUS_MUS | RIVUS_MODUS_SUPER \
    | RIVUS_MODUS_GLUTINUM | RIVUS_MODUS_FOCUS | RIVUS_MODUS_KITTY \
    | RIVUS_MODUS_DEPOSITIO)

/* Linea in modo crudo: OPOST abest, ergo "\r\n" */
interior vacuum
_lineam_scribere (
    constans character* literae,
                   i32  mensura)
{
    (vacuum)terminalis_scribere((constans i8*)literae, mensura);
    (vacuum)terminalis_scribere((constans i8*)"\r\n", II);
}

interior vacuum
_eventum_scribere (
           constans Eventus* e,
                    Piscina* chartae,
        InternamentumChorda* intern)
{
       Xar* unum;
    chorda  linea;

    piscina_vacare(chartae);
    unum                       = xar_creare(chartae,
        (i32)magnitudo(Eventus));
    *(Eventus*)xar_addere(unum) = *e;
    linea = eventus_scribere_stml(unum, chartae, intern, FALSUM);
    _lineam_scribere((constans character*)linea.datum, linea.mensura);
}

/* Ctrl-C pressum (legacy 0x03 aut kitty 99;5u) */
interior b32
_exitus_petitus (
    constans Eventus* e)
{
    redde (b32)(   e->genus == EVENTUS_CLAVIS_DEPRESSUS
                && (e->datum.clavis.modificantes & MOD_IMPERIUM)
                && e->datum.clavis.runa == 'c');
}

s32
principale (vacuum)
{
                  Piscina* piscina;
                  Piscina* chartae;
      InternamentumChorda* intern;
          RivusTerminalis* rivus;
      TerminalisAmplitudo  amplitudo;
                  Eventus  e;
                       i8  intrandi[RIVUS_MODI_MAXIMUM];
                       i8  exeundi[RIVUS_MODI_MAXIMUM];
                       i8  buffer[LECTIO];
                      i32  n_intrandi;
                      i32  n_exeundi;
                      s32  cellula_latitudo;
                      s32  cellula_altitudo;
                      b32  currens = VERUM;

    si (!terminalis_adest())
    {
        fprintf(stderr, "auscultator_terminalis: terminal verum "
            "requiritur (isatty)\n");
        redde I;
    }
    piscina  = piscina_generare_dynamicum("auscultator_terminalis",
        M * M);
    chartae  = piscina_generare_dynamicum("auscultator_chartae", M * M);
    si (piscina == NIHIL || chartae == NIHIL)
    {
        fprintf(stderr, "auscultator_terminalis: piscina deest\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    /* cellula: pixela si terminalis narrat (Ghostty), aliter X x XX */
    (vacuum)terminalis_amplitudo(&amplitudo);
    cellula_latitudo  = (amplitudo.latitudo > ZEPHYRUM)
        ? amplitudo.latitudo / amplitudo.columnae : X;
    cellula_altitudo  = (amplitudo.altitudo > ZEPHYRUM)
        ? amplitudo.altitudo / amplitudo.lineae : XX;
    rivus = rivus_creare(piscina, cellula_latitudo, cellula_altitudo);
    si (rivus == NIHIL)
    {
        fprintf(stderr, "auscultator_terminalis: rivus deest\n");
        redde I;
    }
    n_intrandi  = rivus_modos_intrare(rivus, MODI_OMNES, intrandi,
        RIVUS_MODI_MAXIMUM);
    n_exeundi   = rivus_modos_exeundi(rivus, exeundi,
        RIVUS_MODI_MAXIMUM);
    si (!terminalis_intrare(intrandi, n_intrandi, exeundi, n_exeundi))
    {
        fprintf(stderr, "auscultator_terminalis: modus crudus deest\n");
        redde I;
    }
    {
        character titulus[CXXVIII];

        sprintf(titulus, "auscultator_terminalis: %dx%d cellulae, "
            "cellula %dx%d pixela - Ctrl-C ut exeas",
            (int)amplitudo.columnae, (int)amplitudo.lineae,
            (int)cellula_latitudo, (int)cellula_altitudo);
        _lineam_scribere(titulus, (i32)strlen(titulus));
    }
    dum (currens)
    {
        s32 mora;
        s32 lecti;
        i32 capax;

        dum (   currens
             && rivus_eventum_coalitum(rivus, fenestra_tempus_ms(),
            &e))
        {
            _eventum_scribere(&e, chartae, intern);
            currens = !_exitus_petitus(&e);
        }
        si (!currens)
        {
            frange;
        }
        si (terminalis_resumptum())
        {
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus   = EVENTUS_RESUMPTIO;
            e.tempus  = fenestra_tempus_ms();
            _eventum_scribere(&e, chartae, intern);
        }
        si (terminalis_amplitudo_mutata())
        {
            (vacuum)terminalis_amplitudo(&amplitudo);
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus   = EVENTUS_MUTARE_MAGNITUDINEM;
            e.tempus  = fenestra_tempus_ms();
            e.datum.mutare_magnitudinem.latitudo =
                (i32)(amplitudo.columnae
                * cellula_latitudo);
            e.datum.mutare_magnitudinem.altitudo =
                (i32)(amplitudo.lineae
                * cellula_altitudo);
            _eventum_scribere(&e, chartae, intern);
        }
        mora   = rivus_mora_ms(rivus);
        capax  = rivus_spatium(rivus);
        lecti  = terminalis_legere(buffer, (capax < LECTIO) ? capax
            : LECTIO, (mora > ZEPHYRUM) ? mora : CC);
        si (lecti > ZEPHYRUM)
        {
            (vacuum)rivus_tradere(rivus, buffer, (i32)lecti);
        }
        alioquin si (lecti == ZEPHYRUM && mora > ZEPHYRUM)
        {
            rivus_moram(rivus, fenestra_tempus_ms());
        }
        alioquin si (lecti < ZEPHYRUM)
        {
            currens = FALSUM;
        }
    }
    (vacuum)rivus_modos_exire(rivus, exeundi, RIVUS_MODI_MAXIMUM);
    (vacuum)terminalis_exire();
    printf("auscultator_terminalis: exitus\n");
    piscina_destruere(chartae);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
