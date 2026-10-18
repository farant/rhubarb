/* probatio_reactor_labores.c - labores cooperativi et signa in reactore
 * (hospitium-v3-spec II.1, plan T1): gradus unus per cyclum, poll sine
 * mora dum labor pendet, I/O inter gradus, tabula plena, labor in
 * labore proximo cyclo; signa per fistulam sui - tractator in ansa,
 * numquam in contextu signi; reactor solum signum blocat; reactor
 * alter recusatur donec primus destruitur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "reactor.h"
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* labor numerans: VERUM post 'finis' gradus */
nomen structura {
        i32  gradus;
        i32  finis;
    integer  scribenda;     /* >= 0: in gradu II octetum scribit */
    Reactor* reactor;      /* labor alterum addit (in_labore) */
        b32  addidit;
     vacuum* secundus;     /* Numerus addendus (in_labore) */
} Numerus;

interior b32
_numerare (
    vacuum* datum)
{
    Numerus* n = (Numerus*)datum;

    n->gradus++;
    si (n->scribenda >= ZEPHYRUM && n->gradus == II)
    {
        i8 c = 'x';

        CREDO_AEQUALIS_S32((s32)write(n->scribenda, &c, I), (s32)I);
    }
    redde n->finis > ZEPHYRUM && n->gradus >= n->finis;
}

interior b32
_numquam_finire (
    vacuum* datum)
{
    ((Numerus*)datum)->gradus++;
    redde FALSUM;
}

interior Numerus*
_numerum_creare (CredoContextus* c, i32 finis)
{
    Numerus* n = (Numerus*)piscina_allocare(c->piscina,
        magnitudo(Numerus));

    CREDO_NECESSE_NON_NIHIL(n);
    memset(n, ZEPHYRUM, magnitudo(*n));
    n->finis      = finis;
    n->scribenda  = -I;
    redde n;
}

/* labor qui primo gradu alterum (n->secundus) addit */
interior b32
_addere_alterum (
    vacuum* datum)
{
    Numerus* n = (Numerus*)datum;

    n->gradus++;
    si (!n->addidit)
    {
        CREDO_VERUM(reactor_laborem_adicere(n->reactor, _numerare,
            n->secundus));
        n->addidit = VERUM;
    }
    redde n->gradus >= III;
}

nomen structura {
        i32  vocationes;
        i32  gradus_laboris;    /* gradus laboris cum lectio accidit */
    Numerus* labor;
} Lectio;

interior vacuum
_legere (
    integer  fd,
        i32  eventus,
     vacuum* datum)
{
     Lectio* l = (Lectio*)datum;
         i8  c;

    (vacuum)eventus;
    {
        ssize_t lecta = read(fd, &c, I);   /* glibc warn_unused_result */

        (vacuum)lecta;
    }
    l->vocationes++;
    l->gradus_laboris = l->labor != NIHIL ? l->labor->gradus : ZEPHYRUM;
}

interior vacuum
_tacere (
    integer  fd,
        i32  eventus,
     vacuum* datum)
{
    (vacuum)fd;
    (vacuum)eventus;
    (vacuum)datum;
}

interior vacuum
_sectio_gradatim (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     Numerus* n = _numerum_creare(c, V);
         i32  k;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(reactor_laborem_adicere(r, _numerare, n));
    CREDO_AEQUALIS_I32(reactor_numerus_laborum(r), I);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        (vacuum)reactor_poll(r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(n->gradus, k + I);
    }
    CREDO_AEQUALIS_I32(reactor_numerus_laborum(r), I);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(n->gradus, V);
    CREDO_AEQUALIS_I32(reactor_numerus_laborum(r), ZEPHYRUM);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(n->gradus, V);
    reactor_destruere(r);
}

/* labor pendens: poll non exspectat; sine labore exspectat */
interior vacuum
_sectio_sine_mora (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     Numerus* n = _numerum_creare(c, ZEPHYRUM);
     integer  fistula[II];
         s64  initium;
         s64  duratio;

    CREDO_NECESSE(pipe(fistula) == ZEPHYRUM);
    CREDO_VERUM(reactor_adicere(r, fistula[ZEPHYRUM],
        (i32)REACTOR_LEGERE, _tacere, NIHIL));
    initium = _ms_nunc();
    (vacuum)reactor_poll(r, C);
    duratio = _ms_nunc() - initium;
    CREDO_VERUM(duratio >= LXXX);
    CREDO_VERUM(reactor_laborem_adicere(r, _numquam_finire, n));
    initium = _ms_nunc();
    (vacuum)reactor_poll(r, M);
    duratio = _ms_nunc() - initium;
    CREDO_VERUM(duratio < L);
    CREDO_AEQUALIS_I32(n->gradus, I);
    reactor_destruere(r);
    (vacuum)close(fistula[ZEPHYRUM]);
    (vacuum)close(fistula[I]);
}

/* I/O inter gradus laboris */
interior vacuum
_sectio_alternatio (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     Numerus* n = _numerum_creare(c, X);
     integer  fistula[II];
      Lectio  l;
         i32  k;

    CREDO_NECESSE(pipe(fistula) == ZEPHYRUM);
    memset(&l, ZEPHYRUM, magnitudo(l));
    l.labor       = n;
    n->scribenda  = fistula[I];
    CREDO_VERUM(reactor_adicere(r, fistula[ZEPHYRUM],
        (i32)REACTOR_LEGERE, _legere, &l));
    CREDO_VERUM(reactor_laborem_adicere(r, _numerare, n));
    per (k = ZEPHYRUM; k < XII; k++)
    {
        (vacuum)reactor_poll(r, ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32(n->gradus, X);
    CREDO_AEQUALIS_I32(l.vocationes, I);
    CREDO_VERUM(l.gradus_laboris < X);
    reactor_destruere(r);
    (vacuum)close(fistula[ZEPHYRUM]);
    (vacuum)close(fistula[I]);
}

interior vacuum
_sectio_plena (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     Numerus* n = _numerum_creare(c, ZEPHYRUM);
         i32  k;

    per (k = ZEPHYRUM; k < REACTOR_LABOR_MAXIMA; k++)
    {
        CREDO_VERUM(reactor_laborem_adicere(r, _numquam_finire, n));
    }
    CREDO_FALSUM(reactor_laborem_adicere(r, _numquam_finire, n));
    CREDO_AEQUALIS_I32(reactor_numerus_laborum(r),
        REACTOR_LABOR_MAXIMA);
    reactor_destruere(r);
}

/* labor in labore additus: cyclo PROXIMO currit */
interior vacuum
_sectio_in_labore (CredoContextus* c)
{
    Reactor* r = reactor_creare(c->piscina);
    Numerus* a = _numerum_creare(c, ZEPHYRUM);
    Numerus* b = _numerum_creare(c, II);

    a->reactor   = r;
    a->secundus  = b;
    CREDO_VERUM(reactor_laborem_adicere(r, _addere_alterum, a));
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(a->gradus, I);
    CREDO_AEQUALIS_I32(b->gradus, ZEPHYRUM);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(b->gradus, I);
    reactor_destruere(r);
}

/* labor qui SECUNDO gradu alterum addit */
interior b32
_addere_secundo (
    vacuum* datum)
{
    Numerus* n = (Numerus*)datum;

    n->gradus++;
    si (n->gradus == II)
    {
        CREDO_VERUM(reactor_laborem_adicere(n->reactor, _numerare,
            n->secundus));
    }
    redde n->gradus >= IV;
}

/* labor additus in locum LIBERATUM ante cursorem transitus (locus 1,
 * cum A in loco 0 currit): cyclo proximo, non hoc */
interior vacuum
_sectio_locus_iteratus (CredoContextus* c)
{
    Reactor* r = reactor_creare(c->piscina);
    Numerus* a = _numerum_creare(c, ZEPHYRUM);
    Numerus* x = _numerum_creare(c, I);
    Numerus* y = _numerum_creare(c, ZEPHYRUM);
    Numerus* b = _numerum_creare(c, II);

    a->reactor   = r;
    a->secundus  = b;
    /* loca: 0 = a, 1 = x (gradu uno finit), 2 = y */
    CREDO_VERUM(reactor_laborem_adicere(r, _addere_secundo, a));
    CREDO_VERUM(reactor_laborem_adicere(r, _numerare, x));
    CREDO_VERUM(reactor_laborem_adicere(r, _numquam_finire, y));
    (vacuum)reactor_poll(r, ZEPHYRUM);   /* x finit: locus 1 liber */
    CREDO_AEQUALIS_I32(x->gradus, I);
    (vacuum)reactor_poll(r, ZEPHYRUM);   /* a addit b in locum 1 */
    CREDO_AEQUALIS_I32(a->gradus, II);
    CREDO_AEQUALIS_I32(b->gradus, ZEPHYRUM);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(b->gradus, I);
    reactor_destruere(r);
}

nomen structura {
    i32 vocationes;
} Signatum;

interior vacuum
_signum_venit (
    vacuum* datum)
{
    ((Signatum*)datum)->vocationes++;
}

interior vacuum
_sectio_signum (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
    Signatum  s;

    s.vocationes = ZEPHYRUM;
    CREDO_VERUM(reactor_signum_adicere(r, SIGUSR1, _signum_venit, &s));
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)raise(SIGUSR1), ZEPHYRUM);
    /* nondum: tractator in ansa, non in contextu signi */
    CREDO_AEQUALIS_I32(s.vocationes, ZEPHYRUM);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(s.vocationes, I);
    CREDO_AEQUALIS_S32((s32)raise(SIGUSR1), ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)raise(SIGUSR1), ZEPHYRUM);
    (vacuum)reactor_poll(r, C);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_VERUM(s.vocationes >= II && s.vocationes <= III);
    reactor_destruere(r);
}

/* solum signum registratum: poll exspectat donec signum venit */
interior vacuum
_sectio_signum_solum (CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
    Signatum  s;
         s64  initium;
         s64  duratio;

    s.vocationes = ZEPHYRUM;
    CREDO_VERUM(reactor_signum_adicere(r, SIGALRM, _signum_venit, &s));
    initium = _ms_nunc();
    (vacuum)alarm(I);
    (vacuum)reactor_poll(r, MMM);
    duratio = _ms_nunc() - initium;
    CREDO_AEQUALIS_I32(s.vocationes, I);
    CREDO_VERUM(duratio >= DCCC && duratio < MM);
    reactor_destruere(r);
}

interior vacuum
_sectio_signum_alter (CredoContextus* c)
{
     Reactor* a = reactor_creare(c->piscina);
     Reactor* b = reactor_creare(c->piscina);
    Signatum  s;

    s.vocationes = ZEPHYRUM;
    CREDO_VERUM(reactor_signum_adicere(a, SIGUSR2, _signum_venit, &s));
    CREDO_FALSUM(reactor_signum_adicere(b, SIGUSR2, _signum_venit, &s));
    reactor_destruere(a);
    CREDO_VERUM(reactor_signum_adicere(b, SIGUSR2, _signum_venit, &s));
    CREDO_AEQUALIS_S32((s32)raise(SIGUSR2), ZEPHYRUM);
    (vacuum)reactor_poll(b, C);
    CREDO_AEQUALIS_I32(s.vocationes, I);
    reactor_destruere(b);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "gradatim", _sectio_gradatim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_mora", _sectio_sine_mora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alternatio", _sectio_alternatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "in_labore", _sectio_in_labore, NIHIL, NIHIL, NIHIL, NIHIL },
    { "locus_iteratus", _sectio_locus_iteratus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "signum", _sectio_signum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signum_solum", _sectio_signum_solum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "signum_alter", _sectio_signum_alter, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("reactor_labores", SECTIONES);
}
