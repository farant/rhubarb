/* terminalis_posix.c - Vide terminalis.h
 *
 * Forma ex pontis tesserae (tessera_pons_posix.c) sumpta, sed octeti
 * modorum a vocante dantur (rivus), non literae fixae. Probatio =
 * auscultator terminalis (oculi humani, terminal verum); probatio per
 * pty in B4 (lexicon posix_openpt nondum novit).
 */

#include "postulata_posix.h"
#include "terminalis.h"

#include <termios.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* ==================================================
 * Status staticus: pro tractatoribus signorum (write + tcsetattr)
 * ================================================== */

hic_manens structura termios modus_pristinus;
hic_manens structura termios modus_crudus;
hic_manens i8 octeti_intrandi[TERMINALIS_MODI_MAXIMI];
hic_manens i8 octeti_exeundi[TERMINALIS_MODI_MAXIMI];
hic_manens i32 mensura_intrandi_servata = ZEPHYRUM;
hic_manens i32 mensura_exeundi_servata = ZEPHYRUM;
hic_manens volatilis sig_atomic_t vexillum_intratum = 0;
hic_manens volatilis sig_atomic_t vexillum_resumptum = 0;
hic_manens volatilis sig_atomic_t vexillum_amplitudinis = 0;
hic_manens b32 tractatores_instituti = FALSUM;

interior vacuum
_restituere (vacuum)
{
    si (vexillum_intratum)
    {
        /* async-signal-tuta ambo */
        (vacuum)!write(I, octeti_exeundi,
            (memoriae_index)mensura_exeundi_servata);
        (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus);
        vexillum_intratum = 0;
    }
}

interior vacuum
_tractator_fatalis (
    signatus numerus)
{
    _restituere();
    (vacuum)signal(numerus, SIG_DFL);
    (vacuum)raise(numerus);
}

interior vacuum
_tractator_amplitudinis (
    signatus numerus)
{
    (vacuum)numerus;
    vexillum_amplitudinis = 1;   /* poll EINTR reddit */
}

interior vacuum
_tractator_tstp (
    signatus numerus)
{
    (vacuum)numerus;
    _restituere();
    (vacuum)signal(SIGTSTP, SIG_DFL);
    (vacuum)raise(SIGTSTP);
}

interior vacuum
_tractator_cont (
    signatus numerus)
{
    (vacuum)numerus;
    (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_crudus);
    (vacuum)!write(I, octeti_intrandi,
        (memoriae_index)mensura_intrandi_servata);
    vexillum_intratum = 1;
    (vacuum)signal(SIGTSTP, _tractator_tstp);
    vexillum_resumptum = 1;
}

interior vacuum
_ad_exitum (vacuum)
{
    _restituere();
}

interior vacuum
_tractatores_instituere (vacuum)
{
    structura sigaction actio;

    si (tractatores_instituti)
    {
        redde;
    }
    /* WINCH sine SA_RESTART: poll EINTR reddit */
    actio.sa_handler = _tractator_amplitudinis;
    sigemptyset(&actio.sa_mask);
    actio.sa_flags = ZEPHYRUM;
    (vacuum)sigaction(SIGWINCH, &actio, (structura sigaction*)NIHIL);

    (vacuum)signal(SIGTSTP, _tractator_tstp);
    (vacuum)signal(SIGCONT, _tractator_cont);
    (vacuum)signal(SIGSEGV, _tractator_fatalis);
    (vacuum)signal(SIGBUS, _tractator_fatalis);
    (vacuum)signal(SIGFPE, _tractator_fatalis);
    (vacuum)signal(SIGABRT, _tractator_fatalis);
    (vacuum)signal(SIGTERM, _tractator_fatalis);
    (vacuum)signal(SIGINT, _tractator_fatalis);
    (vacuum)atexit(_ad_exitum);
    tractatores_instituti = VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

b32
terminalis_adest (vacuum)
{
    redde (b32)(isatty(ZEPHYRUM) && isatty(I));
}

b32
terminalis_scribere (
    constans i8* octeti,
            i32  mensura)
{
    i32 scripti = ZEPHYRUM;

    si (octeti == NIHIL)
    {
        redde (b32)(mensura == ZEPHYRUM);
    }
    dum (scripti < mensura)
    {
        ssize_t n = write(I, octeti + scripti,
            (memoriae_index)(mensura - scripti));

        si (n < ZEPHYRUM)
        {
            si (errno == EINTR)
            {
                perge;
            }
            redde FALSUM;
        }
        scripti += (i32)n;
    }
    redde VERUM;
}

b32
terminalis_intrare (
    constans i8* intrandi,
            i32  mensura_intrandi,
    constans i8* exeundi,
            i32  mensura_exeundi)
{
    structura termios modus;

    si (   vexillum_intratum || !terminalis_adest()
        || mensura_intrandi > TERMINALIS_MODI_MAXIMI
        || mensura_exeundi > TERMINALIS_MODI_MAXIMI)
    {
        redde FALSUM;
    }
    si (tcgetattr(ZEPHYRUM, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (mensura_intrandi > ZEPHYRUM)
    {
        memcpy(octeti_intrandi, intrandi,
            (memoriae_index)mensura_intrandi);
    }
    si (mensura_exeundi > ZEPHYRUM)
    {
        memcpy(octeti_exeundi, exeundi,
            (memoriae_index)mensura_exeundi);
    }
    mensura_intrandi_servata  = mensura_intrandi;
    mensura_exeundi_servata   = mensura_exeundi;
    modus_pristinus           = modus;
    cfmakeraw(&modus);
    modus.c_cc[VMIN]   = I;   /* poll moram dat; read saltem unum */
    modus.c_cc[VTIME]  = ZEPHYRUM;
    /* ISIG solum pro SUSP: Ctrl-Z SIGTSTP verum (ut tessera), Ctrl-C
     * et Ctrl-\ claves ordinariae */
    modus.c_lflag      |= (insignatus longus)ISIG;
    modus.c_cc[VINTR]  = _POSIX_VDISABLE;
    modus.c_cc[VQUIT]  = _POSIX_VDISABLE;
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    modus_crudus = modus;
    _tractatores_instituere();
    vexillum_intratum = 1;
    redde terminalis_scribere(octeti_intrandi, mensura_intrandi);
}

b32
terminalis_exire (vacuum)
{
    si (!vexillum_intratum)
    {
        redde VERUM;
    }
    (vacuum)terminalis_scribere(octeti_exeundi,
        mensura_exeundi_servata);
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    vexillum_intratum = 0;
    redde VERUM;
}

s32
terminalis_legere (
     i8* buffer,
    i32  capacitas,
    s32  mora_ms)
{
    structura pollfd fossa;
        signatus fructus;
         ssize_t n;

    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde -I;
    }
    fossa.fd       = ZEPHYRUM;
    fossa.events   = POLLIN;
    fossa.revents  = ZEPHYRUM;
    fructus        = poll(&fossa, (nfds_t)I,
        (mora_ms < ZEPHYRUM) ? -I : (signatus)mora_ms);
    si (fructus < ZEPHYRUM)
    {
        redde (errno == EINTR) ? ZEPHYRUM : -I;   /* signum = mora */
    }
    si (fructus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    n = read(ZEPHYRUM, buffer, (memoriae_index)capacitas);
    si (n < ZEPHYRUM)
    {
        redde (errno == EINTR) ? ZEPHYRUM : -I;
    }
    si (n == ZEPHYRUM)
    {
        redde -I;   /* EOF: terminalis abiit */
    }
    redde (s32)n;
}

b32
terminalis_amplitudo (
    TerminalisAmplitudo* amplitudo)
{
    structura winsize w;   /* NB "magnitudo" = macro latina */

    si (amplitudo == NIHIL)
    {
        redde FALSUM;
    }
    si (   ioctl(I, TIOCGWINSZ, &w) != ZEPHYRUM || w.ws_col == ZEPHYRUM
        || w.ws_row                 == ZEPHYRUM)
    {
        amplitudo->columnae  = LXXX;
        amplitudo->lineae    = XXIV;
        amplitudo->latitudo  = ZEPHYRUM;
        amplitudo->altitudo  = ZEPHYRUM;
        redde VERUM;
    }
    amplitudo->columnae  = (s32)w.ws_col;
    amplitudo->lineae    = (s32)w.ws_row;
    amplitudo->latitudo  = (s32)w.ws_xpixel;
    amplitudo->altitudo  = (s32)w.ws_ypixel;
    redde VERUM;
}

b32
terminalis_resumptum (vacuum)
{
    si (vexillum_resumptum)
    {
        vexillum_resumptum = 0;
        redde VERUM;
    }
    redde FALSUM;
}

b32
terminalis_amplitudo_mutata (vacuum)
{
    si (vexillum_amplitudinis)
    {
        vexillum_amplitudinis = 0;
        redde VERUM;
    }
    redde FALSUM;
}
