/* probatio_canalis.c - canalis (canalis T1): canalis localis ad servum
 * currentem - iter (octeti utrimque), par (uid paris legitur), modi
 * (directorium 0700, socket 0600), directorium apertum recusatum,
 * socket vetus substitutum, alienum (filum regulare) recusatum et
 * intactum, via longa recusata, claudere viam delet, tcp_ex_fossa sine
 * SIGPIPE. Viae /tmp/canalis-<pid>-<n>/servus (canalis P4).
 *
 * Recusatio uid alieni sine radice probari nequit: par (uid VERE
 * lectus) et comparatio unius lineae eam tenent (canalis-spec III).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "tcp.h"
#include "canalis.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

nomen structura {
    character directorium[CXXVIII];
    character via[CCLVI];        /* directorium + "/servus" (gcc
                                  * format-overflow) */
} Locus;

/* locus novus et purus: /tmp/canalis-<pid>-<sectio> */
interior vacuum
_locus (
                 Locus* l,
    constans character* sectio)
{
    sprintf(l->directorium, "/tmp/canalis-%ld-%s",
        (signatus longus)getpid(), sectio);
    sprintf(l->via, "%s/servus", l->directorium);
    (vacuum)filum_arborem_delere(l->directorium);
}

interior vacuum
_dormire_ms (
    s32 ms)
{
    structura timespec t;

    t.tv_sec   = (time_t)(ms / M);
    t.tv_nsec  = (signatus longus)(ms % M) * (signatus longus)M * M;
    (vacuum)nanosleep(&t, NIHIL);
}

interior vacuum
_purgare (
    Locus* l)
{
    (vacuum)filum_arborem_delere(l->directorium);
}

/* accipere cum mora (socket non-blocans): usque ad M ms */
interior TcpConnexio*
_accipere (
    CanalisAuditor* a,
           Piscina* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < M; k++)
    {
        TcpResultus r = canalis_accipere(a, p);

        si (r.successus)
        {
            redde r.connexio;
        }
        si (r.error != TCP_ERROR_ITERUM)
        {
            redde NIHIL;
        }
        _dormire_ms(I);
    }
    redde NIHIL;
}

/* recipere donec 'exspectata' tota venerit (aut M ms) */
interior b32
_recipere (
            TcpConnexio* c,
    constans character* exspectata)
{
    character alveus[LXIV];
          i32 n          = ZEPHYRUM;
          i32 longitudo  = (i32)strlen(exspectata);
          i32 k;

    per (k = ZEPHYRUM; k < M && n < longitudo; k++)
    {
        s32 r = tcp_recipere(c, (i8*)(alveus + n),
                    (i32)magnitudo(alveus) - I - n);

        si (r == TCP_ITERUM)
        {
            _dormire_ms(I);
            perge;
        }
        si (r <= ZEPHYRUM)
        {
            frange;
        }
        n += (i32)r;
    }
    redde    n == longitudo
          && memcmp(alveus, exspectata, (memoriae_index)n) == ZEPHYRUM;
}

/* adestne aliquid in via? filum_existit fopen tentat - socket numquam
 * aperitur, ergo hic filum_status (socket quoque) */
interior b32
_adest (
    constans character* via)
{
    FilumStatus st;

    redde filum_status(via, &st);
}

interior b32
_continet (
                 chorda  c,
     constans character* acus,
                Piscina* p)
{
    redde chorda_continet(c, chorda_ex_literis(acus, p));
}

/* audire, connectere, accipere; octeti utrimque */
interior vacuum
_sectio_iter (
    CredoContextus* c)
{
               Locus  l;
     CanalisResultus  a;
         TcpResultus  cl;
         TcpConnexio* sv;

    _locus(&l, "iter");
    a = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(a.successus);
    cl = canalis_connectere(l.via, c->piscina);
    CREDO_NECESSE(cl.successus);
    sv = _accipere(a.auditor, c->piscina);
    CREDO_NECESSE_NON_NIHIL(sv);
    CREDO_VERUM(tcp_mittere_omnia(cl.connexio, (constans i8*)"salve\n",
        VI));
    CREDO_VERUM(_recipere(sv, "salve\n"));
    CREDO_VERUM(tcp_mittere_omnia(sv, (constans i8*)"vale\n", V));
    CREDO_VERUM(_recipere(cl.connexio, "vale\n"));
    tcp_claudere(sv);
    tcp_claudere(cl.connexio);
    canalis_claudere(a.auditor);
    _purgare(&l);
}

/* uid paris VERE lectus et noster */
interior vacuum
_sectio_par (
    CredoContextus* c)
{
               Locus  l;
     CanalisResultus  a;
         TcpResultus  cl;
         TcpConnexio* sv;
                 i32  uid = ZEPHYRUM;

    _locus(&l, "par");
    a = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(a.successus);
    cl = canalis_connectere(l.via, c->piscina);
    CREDO_NECESSE(cl.successus);
    sv = _accipere(a.auditor, c->piscina);
    CREDO_NECESSE_NON_NIHIL(sv);
    CREDO_VERUM(canalis_par_uid(sv, &uid));
    CREDO_AEQUALIS_I32(uid, (i32)geteuid());
    CREDO_VERUM(canalis_par_admissus(sv));
    tcp_claudere(sv);
    tcp_claudere(cl.connexio);
    canalis_claudere(a.auditor);
    _purgare(&l);
}

/* directorium 0700, socket 0600 */
interior vacuum
_sectio_modi (
    CredoContextus* c)
{
              Locus l;
    CanalisResultus a;
        FilumStatus st;

    _locus(&l, "modi");
    a = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(a.successus);
    CREDO_VERUM(filum_status(l.directorium, &st));
    CREDO_AEQUALIS_S32((s32)st.modus, (s32)0700);
    CREDO_VERUM(filum_status(l.via, &st));
    CREDO_VERUM(st.est_socket);
    CREDO_AEQUALIS_S32((s32)st.modus, (s32)0600);
    canalis_claudere(a.auditor);
    _purgare(&l);
}

/* directorium gregi legibile: recusatur, modus non mutatur */
interior vacuum
_sectio_directorium_apertum (
    CredoContextus* c)
{
              Locus l;
    CanalisResultus a;
        FilumStatus st;

    _locus(&l, "directorium_apertum");
    CREDO_NECESSE(filum_directorium_creare_cum_modo(l.directorium,
        0750));
    a = canalis_audire(l.via, c->piscina);
    CREDO_FALSUM(a.successus);
    CREDO_VERUM(_continet(a.error_descriptio, "apertum", c->piscina));
    CREDO_VERUM(filum_status(l.directorium, &st));
    CREDO_AEQUALIS_S32((s32)st.modus, (s32)0750);
    CREDO_FALSUM(_adest(l.via));
    _purgare(&l);
}

/* socket relictum (servus mortuus sine claudere): substituitur */
interior vacuum
_sectio_vetus (
    CredoContextus* c)
{
              Locus  l;
    CanalisResultus  a;
    CanalisResultus  b;
        TcpResultus  cl;
        TcpConnexio* sv;

    _locus(&l, "vetus");
    a = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(a.successus);
    close(canalis_fossa(a.auditor));   /* mors sine claudere */
    CREDO_VERUM(_adest(l.via));
    b = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(b.successus);
    cl = canalis_connectere(l.via, c->piscina);
    CREDO_NECESSE(cl.successus);
    sv = _accipere(b.auditor, c->piscina);
    CREDO_NON_NIHIL(sv);
    si (sv)
    {
        tcp_claudere(sv);
    }
    tcp_claudere(cl.connexio);
    canalis_claudere(b.auditor);
    _purgare(&l);
}

/* filum regulare in via: recusatur, manet cum contento */
interior vacuum
_sectio_alienum (
    CredoContextus* c)
{
              Locus l;
    CanalisResultus a;
             chorda lectum;

    _locus(&l, "alienum");
    CREDO_NECESSE(filum_directorium_creare_cum_modo(l.directorium,
        0700));
    CREDO_NECESSE(filum_scribere_literis(l.via, "manet"));
    a = canalis_audire(l.via, c->piscina);
    CREDO_FALSUM(a.successus);
    CREDO_CHORDA_NON_VACUA(a.error_descriptio);
    lectum = filum_legere_totum(l.via, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "manet");
    _purgare(&l);
}

/* via longior sun_path: recusatur, limes nominatur, nihil creatum */
interior vacuum
_sectio_longus (
    CredoContextus* c)
{
              Locus l;
    CanalisResultus a;
          character via[CC];
                i32 n;

    _locus(&l, "longus");
    n = (i32)strlen(l.directorium);
    memcpy(via, l.directorium, (memoriae_index)n);
    via[n++] = '/';
    dum (n < CXX)
    {
        via[n++] = 'x';
    }
    via[n]  = '\0';
    a       = canalis_audire(via, c->piscina);
    CREDO_FALSUM(a.successus);
    CREDO_VERUM(_continet(a.error_descriptio, "sun_path", c->piscina));
    CREDO_FALSUM(filum_directorium_existit(l.directorium));
    _purgare(&l);
}

/* claudere: via deleta, connexio nova recusatur */
interior vacuum
_sectio_claudere (
    CredoContextus* c)
{
              Locus l;
    CanalisResultus a;
        TcpResultus cl;

    _locus(&l, "claudere");
    a = canalis_audire(l.via, c->piscina);
    CREDO_NECESSE(a.successus);
    canalis_claudere(a.auditor);
    CREDO_FALSUM(_adest(l.via));
    cl = canalis_connectere(l.via, c->piscina);
    CREDO_FALSUM(cl.successus);
    canalis_claudere(NIHIL);
    _purgare(&l);
}

/* tcp_ex_fossa: scriptio ad parem clausum errorem reddit, processum non
 * necat (macOS SO_NOSIGPIPE; Linux MSG_NOSIGNAL iam) */
interior vacuum
_sectio_nosigpipe (
    CredoContextus* c)
{
    s32 pid;
    s32 status = ZEPHYRUM;

    fflush(NIHIL);
    pid = (s32)fork();
    CREDO_NECESSE(pid >= ZEPHYRUM);
    si (pid == ZEPHYRUM)
    {
            integer  par[II];
        TcpConnexio* t;
                s32  r1;
                s32  r2;

        si (socketpair(AF_UNIX, SOCK_STREAM, ZEPHYRUM, par) != ZEPHYRUM)
        {
            _exit(II);
        }
        t = tcp_ex_fossa(par[ZEPHYRUM], c->piscina);
        close(par[I]);
        r1 = tcp_mittere(t, (constans i8*)"x", I);
        r2 = tcp_mittere(t, (constans i8*)"x", I);
        _exit((r1 < ZEPHYRUM && r2 < ZEPHYRUM) ? ZEPHYRUM : I);
    }
    CREDO_NECESSE(waitpid((pid_t)pid, &status, ZEPHYRUM) == (pid_t)pid);
    CREDO_VERUM(WIFEXITED(status));
    CREDO_AEQUALIS_S32((s32)WEXITSTATUS(status), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iter", _sectio_iter, NIHIL, NIHIL, NIHIL, NIHIL },
    { "par", _sectio_par, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modi", _sectio_modi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "directorium_apertum", _sectio_directorium_apertum, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vetus", _sectio_vetus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienum", _sectio_alienum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "longus", _sectio_longus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claudere", _sectio_claudere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nosigpipe", _sectio_nosigpipe, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("canalis", SECTIONES);
}
