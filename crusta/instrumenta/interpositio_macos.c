/* interpositio_macos.c - oraculum effectuum: bibliotheca dynamica quae
 * per DYLD_INSERT_LIBRARIES in bash Homebrew (non SIP) inseritur et
 * vocationes plagularum notat (effectus-spec par. 0, par. VI.2;
 * planum T5). macOS solum: sectio '__DATA,__interpose' dyld propria
 * est; lingua alia oraculum suum habebit.
 *
 * QUID NOTAT, linea una per eventum, in plagulam quam ambitus
 * INTERPOSITIO_LIBER nominat (O_APPEND, write() unum per lineam -
 * processus paralleli lineas integras servant):
 *   P  pid  progname  PATH          processus primum visus (filtrum
 *                                   quaesitionum PATH eget PATH SUO)
 *   E  pid  genus  via  rc          open/openat (LEGERE, SCRIBERE),
 *                                   stat, lstat, access, fstatat,
 *                                   faccessat, opendir, execve
 *   D  pid  cwd                     ante argv: directorium operis
 *   A  pid  index  verbum           argv execve (mandata SIP per
 *                                   argumenta et tabulam leguntur)
 *   R  pid  via                     shebang '#!/bin/bash' redirectum
 * Viae relativae cum cwd TEMPORE VOCATIONIS absolutae fiunt (bash 'cd'
 * cwd mutat); viae ad fossam directorii aliam quam AT_FDCWD notantur ut
 * 'dirfd:N/via'.
 *
 * SIP: dyld DYLD_* in binariis protectis (/bin, /usr/bin) delet.
 * Remedia duo (spica 2026-10-05): argv omnis execve notatur; scriptum
 * cuius linea prima '#!/bin/bash' est per /opt/homebrew/bin/bash
 * iterum exsequitur, ut subarbor visibilis maneat.
 *
 * Intra hanc imaginem vocationes NON interponuntur (lex dyld): open()
 * libri ipsius ad functionem veram it. */

#include "postulata_posix.h"

#include "latina.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define LINEA_MAXIMA  8192
#define BASH_DOMUS    "/opt/homebrew/bin/bash"
#define ARGV_MAXIMA   512

extern constans character* getprogname (vacuum);

nomen vacuum (*Functio) (vacuum);

nomen structura {
    Functio novum;
    Functio vetus;
} Interpositio;

hic_manens integer processus_notatus = 0;
hic_manens integer processus_pid     = 0;

/* chordam in lineam appendere; tabulae et lineae novae in spatium */
interior vacuum
appendere (
          character* linea,
           integer* longitudo,
    constans character* textus)
{
    dum (textus != NIHIL && *textus != '\0'
           && *longitudo < LINEA_MAXIMA - 2)
    {
        character c = *textus++;

        linea[(*longitudo)++] = (c == '\t' || c == '\n') ? ' ' : c;
    }
    linea[*longitudo] = '\0';
}

/* signum separationis crudum (tabula, linea nova) */
interior vacuum
separare (
    character* linea,
      integer* longitudo,
     character signum)
{
    si (*longitudo < LINEA_MAXIMA - II)
    {
        linea[(*longitudo)++] = signum;
    }
    linea[*longitudo] = '\0';
}

interior vacuum
appendere_numerum (
          character* linea,
           integer* longitudo,
           longus numerus)
{
    character  cifrae[24];
    integer   k = 0;
    integer   negativus = numerus < 0;
    insignatus longus n = negativus ? (insignatus longus)(-numerus)
                                : (insignatus longus)numerus;

    fac
    {
        cifrae[k++] = (character)('0' + (integer)(n % 10UL));
        n /= 10UL;
    }
    dum (n > 0UL && k < 22);
    si (negativus)
    {
        cifrae[k++] = '-';
    }
    dum (k > 0 && *longitudo < LINEA_MAXIMA - 2)
    {
        linea[(*longitudo)++] = cifrae[--k];
    }
    linea[*longitudo] = '\0';
}

interior vacuum
scribere (
    constans character* linea,
            integer longitudo)
{
    constans character* liber = getenv("INTERPOSITIO_LIBER");
    integer         fd;

    si (liber == NIHIL || *liber == '\0')
    {
        redde;
    }
    fd = open(liber, O_WRONLY | O_APPEND | O_CREAT, 0644);
    si (fd >= 0)
    {
        (vacuum)write(fd, linea, (size_t)longitudo);
        (vacuum)close(fd);
    }
}

/* processus primum visus: P pid progname PATH */
interior vacuum
processum_notare (vacuum)
{
    character linea[LINEA_MAXIMA];
    integer  longitudo = 0;
    integer  pid = (integer)getpid();

    si (processus_notatus && processus_pid == pid)
    {
        redde;
    }
    processus_notatus = 1;
    processus_pid     = pid;
    linea[0] = '\0';
    separare(linea, &longitudo, 'P');
    separare(linea, &longitudo, '\t');
    appendere_numerum(linea, &longitudo, (longus)pid);
    separare(linea, &longitudo, '\t');
    appendere(linea, &longitudo, getprogname());
    separare(linea, &longitudo, '\t');
    appendere(linea, &longitudo, getenv("PATH"));
    separare(linea, &longitudo, '\n');
    scribere(linea, longitudo);
}

/* via absoluta: relativa cum cwd praesenti; fossa alia notatur */
interior vacuum
viam_appendere (
          character* linea,
           integer* longitudo,
            integer fossa_directorii,
    constans character* via)
{
    character cwd[4096];

    si (via == NIHIL)
    {
        appendere(linea, longitudo, "?");
        redde;
    }
    si (via[0] != '/')
    {
        si (fossa_directorii != AT_FDCWD)
        {
            appendere(linea, longitudo, "dirfd:");
            appendere_numerum(linea, longitudo,
                (longus)fossa_directorii);
            appendere(linea, longitudo, "/");
        }
        alioquin si (getcwd(cwd, magnitudo cwd) != NIHIL)
        {
            appendere(linea, longitudo, cwd);
            si (!(cwd[0] == '/' && cwd[1] == '\0'))
            {
                appendere(linea, longitudo, "/");
            }
        }
    }
    appendere(linea, longitudo, via);
}

interior vacuum
notare (
    constans character* genus,
            integer fossa_directorii,
    constans character* via,
            integer rc)
{
    character linea[LINEA_MAXIMA];
    integer  longitudo = 0;

    si (getenv("INTERPOSITIO_LIBER") == NIHIL)
    {
        redde;
    }
    processum_notare();
    linea[0] = '\0';
    separare(linea, &longitudo, 'E');
    separare(linea, &longitudo, '\t');
    appendere_numerum(linea, &longitudo, (longus)getpid());
    separare(linea, &longitudo, '\t');
    appendere(linea, &longitudo, genus);
    separare(linea, &longitudo, '\t');
    viam_appendere(linea, &longitudo, fossa_directorii, via);
    separare(linea, &longitudo, '\t');
    appendere_numerum(linea, &longitudo, (longus)rc);
    separare(linea, &longitudo, '\n');
    scribere(linea, longitudo);
}

interior vacuum
argv_notare (
    character* constans* argumenta)
{
    character linea[LINEA_MAXIMA];
    character cwd[4096];
    integer  longitudo;
    integer  i;

    /* D pid cwd: argumenta relativa ad cwd TEMPORE exsecutionis */
    longitudo = 0;
    linea[0]  = '\0';
    separare(linea, &longitudo, 'D');
    separare(linea, &longitudo, '\t');
    appendere_numerum(linea, &longitudo, (longus)getpid());
    separare(linea, &longitudo, '\t');
    appendere(linea, &longitudo,
        getcwd(cwd, magnitudo cwd) != NIHIL ? cwd : "?");
    separare(linea, &longitudo, '\n');
    scribere(linea, longitudo);
    per (i = 0; argumenta != NIHIL && argumenta[i] != NIHIL; i++)
    {
        longitudo = 0;
        linea[0]  = '\0';
        separare(linea, &longitudo, 'A');
    separare(linea, &longitudo, '\t');
        appendere_numerum(linea, &longitudo, (longus)getpid());
        separare(linea, &longitudo, '\t');
        appendere_numerum(linea, &longitudo, (longus)i);
        separare(linea, &longitudo, '\t');
        appendere(linea, &longitudo, argumenta[i]);
        separare(linea, &longitudo, '\n');
        scribere(linea, longitudo);
    }
}

interior integer
legere_an (
    integer vexilla)
{
    redde (vexilla & (O_WRONLY | O_RDWR | O_CREAT | O_TRUNC
                       | O_APPEND)) == 0;
}


/* ==================================================
 * Functiones interpositae
 * ================================================== */

interior integer
interpositum_open (
    constans character* via,
            integer vexilla,
            ...)
{
    integer     modus = 0;
    integer     rc;
    va_list ap;

    si (vexilla & O_CREAT)
    {
        va_start(ap, vexilla);
        modus = va_arg(ap, integer);
        va_end(ap);
    }
    rc = open(via, vexilla, modus);
    notare(legere_an(vexilla) ? "LEGERE" : "SCRIBERE", AT_FDCWD, via,
        rc);
    redde rc;
}

interior integer
interpositum_openat (
            integer fossa_directorii,
    constans character* via,
            integer vexilla,
            ...)
{
    integer     modus = 0;
    integer     rc;
    va_list ap;

    si (vexilla & O_CREAT)
    {
        va_start(ap, vexilla);
        modus = va_arg(ap, integer);
        va_end(ap);
    }
    rc = openat(fossa_directorii, via, vexilla, modus);
    notare(legere_an(vexilla) ? "LEGERE" : "SCRIBERE", fossa_directorii,
        via, rc);
    redde rc;
}

interior integer
interpositum_stat (
    constans character*  via,
    structura stat* status)
{
    integer rc = stat(via, status);

    notare("STAT", AT_FDCWD, via, rc);
    redde rc;
}

interior integer
interpositum_lstat (
    constans character*  via,
    structura stat* status)
{
    integer rc = lstat(via, status);

    notare("LSTAT", AT_FDCWD, via, rc);
    redde rc;
}

interior integer
interpositum_access (
    constans character* via,
            integer modus)
{
    integer rc = access(via, modus);

    notare("ACCESS", AT_FDCWD, via, rc);
    redde rc;
}

interior integer
interpositum_fstatat (
            integer  fossa_directorii,
    constans character*  via,
    structura stat* status,
            integer  vexilla)
{
    integer rc = fstatat(fossa_directorii, via, status, vexilla);

    notare("STAT", fossa_directorii, via, rc);
    redde rc;
}

interior integer
interpositum_faccessat (
            integer fossa_directorii,
    constans character* via,
            integer modus,
            integer vexilla)
{
    integer rc = faccessat(fossa_directorii, via, modus, vexilla);

    notare("ACCESS", fossa_directorii, via, rc);
    redde rc;
}

interior DIR*
interpositum_opendir (
    constans character* via)
{
    DIR* d = opendir(via);

    notare("OPENDIR", AT_FDCWD, via, d != NIHIL ? 0 : -1);
    redde d;
}

/* execve: argv notatur; '#!/bin/bash' per bash Homebrew */
interior integer
interpositum_execve (
    constans character*  via,
    character* constans* argumenta,
    character* constans* ambitus)
{
    character  caput[64];
    integer   fd;
    longus  n = 0;

    notare("EXECVE", AT_FDCWD, via, 0);
    argv_notare(argumenta);
    fd = open(via, O_RDONLY);
    si (fd >= 0)
    {
        n = (longus)read(fd, caput, magnitudo caput - 1);
        (vacuum)close(fd);
    }
    si (n > 11)
    {
        caput[n] = '\0';
        si (strncmp(caput, "#!/bin/bash", 11) == 0
            && (caput[11] == '\n' || caput[11] == ' '))
        {
            character* nova[ARGV_MAXIMA];
            character  interpres[magnitudo BASH_DOMUS];
            character  scriptum[4096];
            integer   k = 0;
            integer   i;

            strcpy(interpres, BASH_DOMUS);
            strncpy(scriptum, via, magnitudo scriptum - I);
            scriptum[magnitudo scriptum - I] = '\0';
            nova[k++] = interpres;
            nova[k++] = scriptum;
            per (i = 1; argumenta != NIHIL && argumenta[i] != NIHIL
                        && k < ARGV_MAXIMA - 1; i++)
            {
                nova[k++] = argumenta[i];
            }
            nova[k] = NIHIL;
            notare("SHEBANG", AT_FDCWD, via, 0);
            redde execve(BASH_DOMUS, nova, ambitus);
        }
    }
    redde execve(via, argumenta, ambitus);
}


/* ==================================================
 * Tabula interpositionis (dyld legit sectionem)
 * ================================================== */

/* Sectio '__DATA,__interpose' est quam dyld legit (par
 * novum/vetus); 'used' vetat ne nexor tabulam nusquam citatam
 * deleat. Sine eis oraculum iners est - extensio CONSULTO. */
hic_manens constans Interpositio interpositiones[]
    /* <tolera codex="EXTENSIO_COMPILATORIS" (>tabula dyld */
    __attribute__((used, section("__DATA,__interpose"))) = {
    { (Functio)interpositum_open,      (Functio)open      },
    { (Functio)interpositum_openat,    (Functio)openat    },
    { (Functio)interpositum_stat,      (Functio)stat      },
    { (Functio)interpositum_lstat,     (Functio)lstat     },
    { (Functio)interpositum_access,    (Functio)access    },
    { (Functio)interpositum_fstatat,   (Functio)fstatat   },
    { (Functio)interpositum_faccessat, (Functio)faccessat },
    { (Functio)interpositum_opendir,   (Functio)opendir   },
    { (Functio)interpositum_execve,    (Functio)execve    }
};
