/* tessera_pons_posix.c - Implementatio pontis POSIX (Phase B)
 *
 * PLAGULA UNICA CAPITUM SYSTEMATIS in tessera - omnia supra pontem
 * per pontem memoriae probantur; huius plagulae probatio =
 * spectaculum (oculi humani, terminal verum).
 */

#include "postulata_posix.h"
#include "tessera_pons_posix.h"

#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Effugia intrandi/exeundi: scrinium alternum + mus SGR */
#define INTRANDI "\033[?1049h\033[?1000h\033[?1006h"
#define EXEUNDI  "\033[?1006l\033[?1000l\033[?1049l\033[0m\033[?25h"

/* ==================================================
 * STATICUM SANCTUM: status restitutionis pro tractatoribus
 * signorum (async-signal-tuta: write + tcsetattr solum)
 * ================================================== */

hic_manens structura termios modus_pristinus;
hic_manens structura termios modus_crudus;
hic_manens volatilis sig_atomic_t vexillum_intratum = 0;
hic_manens volatilis sig_atomic_t vexillum_resumptum = 0;
hic_manens b32 tractatores_instituti = FALSUM;

interior vacuum
_restituere_cruda (vacuum)
{
    si (vexillum_intratum)
    {
        /* async-signal-tuta ambo */
        (vacuum)!write(I, EXEUNDI, magnitudo(EXEUNDI) - I);
        (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus);
        vexillum_intratum = 0;
    }
}

interior vacuum
_tractator_fatalis (signatus numerus)
{
    _restituere_cruda();
    (vacuum)signal(numerus, SIG_DFL);
    (vacuum)raise(numerus);
}

interior vacuum
_tractator_winch (signatus numerus)
{
    (vacuum)numerus;
    /* corpus vacuum: select EINTR accipit; lector amplitudinem
     * rogat */
}

interior vacuum
_tractator_tstp (signatus numerus)
{
    (vacuum)numerus;
    _restituere_cruda();
    (vacuum)signal(SIGTSTP, SIG_DFL);
    (vacuum)raise(SIGTSTP);
}

interior vacuum
_tractator_cont (signatus numerus)
{
    (vacuum)numerus;
    /* in crudum redire + tractatorem TSTP reinstituere */
    (vacuum)tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_crudus);
    (vacuum)!write(I, INTRANDI, magnitudo(INTRANDI) - I);
    vexillum_intratum = 1;
    (vacuum)signal(SIGTSTP, _tractator_tstp);
    vexillum_resumptum = 1;
}

interior vacuum
_ad_exitum (vacuum)
{
    _restituere_cruda();
}

interior vacuum
_tractatores_instituere (vacuum)
{
    structura sigaction actio;

    si (tractatores_instituti)
    {
        redde;
    }
    /* WINCH sine SA_RESTART: select EINTR reddit */
    actio.sa_handler = _tractator_winch;
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
 * Tabula functionum
 * ================================================== */

interior s32
_legere_posix (vacuum* datum, i8* buffer, i32 capacitas, s32 mora_ms)
{
    fd_set legenda;
    structura timeval mora;
    signatus fructus;

    (vacuum)datum;
    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde -I;
    }
    FD_ZERO(&legenda);
    FD_SET(ZEPHYRUM, &legenda);
    si (mora_ms >= ZEPHYRUM)
    {
        mora.tv_sec = mora_ms / 1000;
        mora.tv_usec = (mora_ms % 1000) * 1000;
        fructus = select(I, &legenda, (fd_set*)NIHIL, (fd_set*)NIHIL,
            &mora);
    }
    alioquin
    {
        fructus = select(I, &legenda, (fd_set*)NIHIL, (fd_set*)NIHIL,
            (structura timeval*)NIHIL);
    }
    si (fructus < ZEPHYRUM)
    {
        redde (errno == EINTR) ? ZEPHYRUM : -I;  /* WINCH = mora */
    }
    si (fructus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    {
        ssize_t n = read(ZEPHYRUM, buffer, (memoriae_index)capacitas);

        si (n < ZEPHYRUM)
        {
            redde (errno == EINTR) ? ZEPHYRUM : -I;
        }
        si (n == ZEPHYRUM)
        {
            redde -I;  /* EOF: terminal abiit */
        }
        redde (s32)n;
    }
}

interior b32
_scribere_posix (vacuum* datum, constans i8* octeti, i32 numerus)
{
    i32 scripti = ZEPHYRUM;

    (vacuum)datum;
    si (octeti == NIHIL)
    {
        redde FALSUM;
    }
    dum (scripti < numerus)
    {
        ssize_t n = write(I, octeti + scripti,
            (memoriae_index)(numerus - scripti));

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

interior b32
_amplitudo_posix (vacuum* datum, i32* latitudo_out, i32* altitudo_out)
{
    structura winsize fenestra;  /* NB "magnitudo" = macro latina! */

    (vacuum)datum;
    si (latitudo_out == NIHIL || altitudo_out == NIHIL)
    {
        redde FALSUM;
    }
    si (ioctl(I, TIOCGWINSZ, &fenestra) != ZEPHYRUM
        || fenestra.ws_col == ZEPHYRUM
        || fenestra.ws_row == ZEPHYRUM)
    {
        *latitudo_out = LXXX;   /* 80x24 refugium */
        *altitudo_out = XXIV;
        redde VERUM;
    }
    *latitudo_out = (i32)fenestra.ws_col;
    *altitudo_out = (i32)fenestra.ws_row;
    redde VERUM;
}

interior b32
_intrare_posix (vacuum* datum)
{
    structura termios modus;

    (vacuum)datum;
    si (tcgetattr(ZEPHYRUM, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    modus_pristinus = modus;
    cfmakeraw(&modus);
    modus.c_cc[VMIN] = I;   /* select moram dat; read saltem unum */
    modus.c_cc[VTIME] = ZEPHYRUM;
    /* ISIG redux sed SUSP SOLUM: Ctrl-Z SIGTSTP verum generat
     * (interview: "Ctrl-Z numquam frangit"), Ctrl-C et Ctrl-\
     * claves ordinariae manent (VINTR/VQUIT singulatim vetita -
     * cfmakeraw ISIG totum stinxerat, quo Ctrl-Z octetus 0x1A
     * merus fiebat: oculi Fran in spectaculo id invenerunt) */
    modus.c_lflag |= (insignatus longus)ISIG;
    modus.c_cc[VINTR] = _POSIX_VDISABLE;
    modus.c_cc[VQUIT] = _POSIX_VDISABLE;
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    modus_crudus = modus;
    _tractatores_instituere();
    vexillum_intratum = 1;
    redde _scribere_posix(NIHIL, (constans i8*)INTRANDI,
        (i32)(magnitudo(INTRANDI) - I));
}

interior b32
_egredi_posix (vacuum* datum)
{
    (vacuum)datum;
    si (!vexillum_intratum)
    {
        redde VERUM;
    }
    (vacuum)_scribere_posix(NIHIL, (constans i8*)EXEUNDI,
        (i32)(magnitudo(EXEUNDI) - I));
    si (tcsetattr(ZEPHYRUM, TCSAFLUSH, &modus_pristinus) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    vexillum_intratum = 0;
    redde VERUM;
}

interior b32
_resumptum_posix (vacuum* datum)
{
    (vacuum)datum;
    si (vexillum_resumptum)
    {
        vexillum_resumptum = 0;
        redde VERUM;
    }
    redde FALSUM;
}

TesseraPons*
tessera_pons_posix_creare (Piscina* piscina)
{
    TesseraPons* pons;

    si (piscina == NIHIL || !isatty(ZEPHYRUM) || !isatty(I))
    {
        redde NIHIL;  /* sine terminali = pons memoriae, expressus */
    }
    pons = (TesseraPons*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraPons), IV);
    si (pons == NIHIL)
    {
        redde NIHIL;
    }
    pons->datum = NIHIL;
    pons->legere = _legere_posix;
    pons->scribere = _scribere_posix;
    pons->amplitudo = _amplitudo_posix;
    pons->intrare = _intrare_posix;
    pons->egredi = _egredi_posix;
    pons->resumptum = _resumptum_posix;
    redde pons;
}
