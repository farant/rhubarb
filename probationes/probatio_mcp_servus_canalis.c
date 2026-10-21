/* probatio_mcp_servus_canalis.c - MCP super canalem (canalis T2):
 * servus in filio furcato (reactor, tabula probationis: echo, tarde
 * dilatum, frange isError), parens cliens. vocare (0/1/2), dilatum,
 * vitium, nemo audit, linea longa clauditur, linea imperfecta omissa,
 * nonus recusatur, fuga in medio, pons sessionem totam transmittit,
 * pons post mortem servi exit (Review focus 1-2).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "json.h"
#include "reactor.h"
#include "tcp.h"
#include "canalis.h"
#include "mcp_servus.h"
#include "mcp_servus_canalis.h"
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINEA_INITII "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":" \
    "\"initialize\",\"params\":{\"protocolVersion\":\"2025-11-25\"," \
    "\"capabilities\":{},\"clientInfo\":{\"name\":\"probatio\"," \
    "\"version\":\"1\"}}}\n"
#define LINEA_INITIATUM "{\"jsonrpc\":\"2.0\",\"method\":" \
    "\"notifications/initialized\"}\n"
#define LINEA_ECHO "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":" \
    "\"tools/call\",\"params\":{\"name\":\"echo\",\"arguments\":" \
    "{\"textus\":\"salve\"}}}\n"
#define LINEA_TARDE "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":" \
    "\"tools/call\",\"params\":{\"name\":\"tarde\",\"arguments\":" \
    "{\"ms\":200}}}\n"

#define LINEA_TARDE_D "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":" \
    "\"tools/call\",\"params\":{\"name\":\"tarde\",\"arguments\":" \
    "{\"ms\":500}}}\n"

#define ALVEUS_PONTIS (IV * MXXIV)

/* ---- loci (canalis P4) ---- */

nomen structura {
    character directorium[CXXVIII];
    character via[CCLVI];
} Locus;

interior vacuum
_locus (
                 Locus* l,
    constans character* sectio)
{
    sprintf(l->directorium, "/tmp/mcpc-%ld-%s",
        (signatus longus)getpid(), sectio);
    sprintf(l->via, "%s/servus", l->directorium);
    (vacuum)filum_arborem_delere(l->directorium);
}

interior vacuum
_purgare (
    Locus* l)
{
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
_echo (
    McpVocatio* v)
{
    chorda t;

    (vacuum)mcp_argumentum_chorda(v, "textus", &t);
    mcp_respondere_textum(v, t);
}

interior vacuum
_tarde_respondere (
    vacuum* datum)
{
     McpDilatum* d = (McpDilatum*)datum;
     McpVocatio* v = mcp_dilatum_vocatio(*d);

    si (v)
    {
        mcp_respondere_textum(v, chorda_ex_literis("tarde",
            mcp_vocatio_piscina(v)));
    }
}

/* differt; timer reactoris post 'ms' respondet */
interior vacuum
_tarde (
    McpVocatio* v)
{
       Reactor* r   = (Reactor*)mcp_vocatio_datum(v);
           s64  ms  = ZEPHYRUM;
    McpDilatum* d;

    (vacuum)mcp_argumentum_integer(v, "ms", &ms);
    d = (McpDilatum*)piscina_allocare(mcp_vocatio_piscina(v),
        (memoriae_index)magnitudo(McpDilatum));
    *d = mcp_differre(v);
    (vacuum)reactor_timer_adicere(r, (i32)ms, FALSUM, _tarde_respondere,
        d);
}

interior vacuum
_frange (
    McpVocatio* v)
{
    mcp_respondere_vitium(v, chorda_ex_literis("fractum",
        mcp_vocatio_piscina(v)));
}

hic_manens constans McpArgumentum ARG_ECHO[] = {
    { "textus", "textus reddendus", MCP_CHORDA, VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_TARDE[] = {
    { "ms", "mora in ms", MCP_INTEGER, VERUM, NIHIL }
};

hic_manens constans McpInstrumentum INSTRUMENTA[] = {
    { "echo", "textum reddit", ARG_ECHO, I, "administratio", VERUM,
        FALSUM, _echo },
    { "tarde", "post moram respondet", ARG_TARDE, I, "administratio",
        VERUM, FALSUM, _tarde },
    { "frange", "semper deficit", NIHIL, ZEPHYRUM, "administratio",
        VERUM, FALSUM, _frange }
};

/* filius: servus in via usque ad signum; parens pid accipit */
interior s32
_servus_filius (
    constans character* via)
{
    s32 pid;

    fflush(NIHIL);
    pid = (s32)fork();
    si (pid == ZEPHYRUM)
    {
                Piscina* p = piscina_generare_dynamicum("servus_filius",
                             LXIV * MXXIV);
                Reactor* r;
        McpConfiguratio  cfg;
              McpServus* s;
                 chorda  error;

        si (!p)
        {
            _exit(III);
        }
        r = reactor_creare(p);
        memset(&cfg, ZEPHYRUM, magnitudo(cfg));
        cfg.titulus      = "probatio";
        cfg.versio       = "1";
        cfg.instrumenta  = INSTRUMENTA;
        cfg.numerus      = III;
        cfg.datum        = r;
        s                = mcp_servus_creare(&cfg, p);
        si (!r || !s || !mcp_canalis_praebere(s, r, via, &error, p))
        {
            _exit(III);
        }
        reactor_currere(r);
        _exit(ZEPHYRUM);
    }
    redde pid;
}

/* servus auscultat? (connexio probationis, statim clausa) */
interior b32
_paratus (
    constans character* via,
               Piscina* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < CC; k++)
    {
        TcpResultus r = canalis_connectere(via, p);

        si (r.successus)
        {
            tcp_claudere(r.connexio);
            redde VERUM;
        }
        _dormire_ms(X);
    }
    redde FALSUM;
}

interior vacuum
_finire (
    s32 pid)
{
    integer status;

    si (pid > ZEPHYRUM)
    {
        (vacuum)kill((pid_t)pid, SIGKILL);
        (vacuum)waitpid((pid_t)pid, &status, ZEPHYRUM);
    }
}

/* servus vivitne? si mortuus, causam (signum aut exitum) imprimit */
interior b32
_vivus (
    s32 pid)
{
    integer status = ZEPHYRUM;

    si (waitpid((pid_t)pid, &status, WNOHANG) == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (WIFSIGNALED(status))
    {
        imprimere("\n  servus mortuus: signum %d\n", WTERMSIG(status));
    }
    alioquin si (WIFEXITED(status))
    {
        imprimere("\n  servus exiit: %d\n", WEXITSTATUS(status));
    }
    redde FALSUM;
}

interior TcpConnexio*
_crudus (
    constans character* via,
               Piscina* p)
{
    TcpResultus r = canalis_connectere(via, p);

    redde r.successus ? r.connexio : NIHIL;
}

interior b32
_scribere (
           TcpConnexio* c,
    constans character* literae)
{
    redde tcp_mittere_omnia(c, (constans i8*)literae,
        (i32)strlen(literae));
}

/* legit (et omittit) donec EOF intra 'ms': VERUM si finis venit */
interior b32
_ad_finem (
    integer fd,
        s32 ms)
{
     character alveus[DXII];
           s32 k;

    per (k = ZEPHYRUM; k < ms / X + I; k++)
    {
        structura pollfd pf;
                 ssize_t n;

        pf.fd       = fd;
        pf.events   = POLLIN;
        pf.revents  = ZEPHYRUM;
        si (poll(&pf, I, X) <= ZEPHYRUM)
        {
            perge;
        }
        n = read(fd, alveus, magnitudo(alveus));
        si (n <= ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* legit in alveum donec 'lineae' lineae aut EOF aut 'ms'; longitudo */
interior s32
_legere_lineas (
      integer  fd,
    character* alveus,
          s32  capacitas,
          s32  lineae,
          s32  ms)
{
    s32 n      = ZEPHYRUM;
    s32 visae  = ZEPHYRUM;
    s32 k;

    per (k = ZEPHYRUM; k < ms / X + I && visae < lineae; k++)
    {
        structura pollfd pf;
                 ssize_t r;
                     s32 j;

        pf.fd       = fd;
        pf.events   = POLLIN;
        pf.revents  = ZEPHYRUM;
        si (poll(&pf, I, X) <= ZEPHYRUM)
        {
            perge;
        }
        r = read(fd, alveus + n, (memoriae_index)(capacitas - I - n));
        si (r <= ZEPHYRUM)
        {
            frange;
        }
        per (j = n; j < n + (s32)r; j++)
        {
            si (alveus[j] == '\n')
            {
                visae++;
            }
        }
        n += (s32)r;
    }
    alveus[n] = '\0';
    redde n;
}

interior JsonValor*
_argumenta_echo (
    Piscina* p)
{
    JsonValor* a = json_objectum_creare(p);

    json_objectum_ponere(a, "textus", json_chorda_creare_literis(p,
        "salve"));
    redde a;
}

/* ---- sectiones ---- */

interior vacuum
_sectio_vocare (
    CredoContextus* c)
{
     Locus l;
       s32 pid;
    chorda t;

    _locus(&l, "vocare");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "salve");
    _finire(pid);
    _purgare(&l);
}

interior vacuum
_sectio_dilatum (
    CredoContextus* c)
{
         Locus  l;
           s32  pid;
        chorda  t;
     JsonValor* a = json_objectum_creare(c->piscina);

    _locus(&l, "dilatum");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    json_objectum_ponere(a, "ms", json_integer_creare(c->piscina, L));
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "tarde", a, &t,
        c->piscina), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "tarde");
    _finire(pid);
    _purgare(&l);
}

interior vacuum
_sectio_vitium (
    CredoContextus* c)
{
     Locus l;
       s32 pid;
    chorda t;

    _locus(&l, "vitium");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "frange", NIHIL, &t,
        c->piscina), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "fractum");
    _finire(pid);
    _purgare(&l);
}

/* nemo audit: vocare et pons II */
interior vacuum
_sectio_nemo (
    CredoContextus* c)
{
      Locus l;
     chorda t;
    integer f[II];

    _locus(&l, "nemo");
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), II);
    CREDO_NECESSE(pipe(f) == ZEPHYRUM);
    CREDO_AEQUALIS_S32(mcp_canalis_pons(l.via, f[ZEPHYRUM], f[I]), II);
    close(f[ZEPHYRUM]);
    close(f[I]);
    _purgare(&l);
}

/* linea ultra MiB sine fine: connexio clauditur; servus manet */
interior vacuum
_sectio_linea_longa (
    CredoContextus* c)
{
           Locus  l;
             s32  pid;
          chorda  t;
     TcpConnexio* cl;
       character* x;
             i32  k;

    _locus(&l, "longa");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    cl = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(cl);
    CREDO_VERUM(_scribere(cl, LINEA_INITII));
    x = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(LXIV * MXXIV));
    CREDO_NECESSE_NON_NIHIL(x);
    memset(x, 'x', (memoriae_index)(LXIV * MXXIV));
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        si (!tcp_mittere_omnia(cl, (constans i8*)x, LXIV * MXXIV))
        {
            frange;
        }
    }
    (vacuum)tcp_mittere_omnia(cl, (constans i8*)x, I);
    CREDO_VERUM(_ad_finem((integer)tcp_obtinere_fd(cl), MM));
    tcp_claudere(cl);
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), ZEPHYRUM);
    _finire(pid);
    _purgare(&l);
}

/* linea ultima sine '\n', deinde finis: omittitur, servus manet
 * (Review focus 2) */
interior vacuum
_sectio_linea_imperfecta (
    CredoContextus* c)
{
           Locus  l;
             s32  pid;
          chorda  t;
     TcpConnexio* cl;

    _locus(&l, "imperfecta");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    cl = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(cl);
    CREDO_VERUM(_scribere(cl, LINEA_INITII));
    CREDO_VERUM(_scribere(cl, "{\"jsonrpc\":\"2.0\""));
    tcp_claudere(cl);
    _dormire_ms(L);
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), ZEPHYRUM);
    CREDO_VERUM(_vivus(pid));
    _finire(pid);
    _purgare(&l);
}

/* VIII tenentur; nonus statim clauditur; uno soluto vocare transit */
interior vacuum
_sectio_nonus (
    CredoContextus* c)
{
           Locus  l;
             s32  pid;
          chorda  t;
     TcpConnexio* tenentes[VIII];
     TcpConnexio* nonus;
             i32  k;

    _locus(&l, "nonus");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    /* connexio probationis _parati locum suum prius solvat - aliter
     * octava tenentium (non nona) recusatur */
    _dormire_ms(C);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        tenentes[k] = _crudus(l.via, c->piscina);
        CREDO_NECESSE_NON_NIHIL(tenentes[k]);
    }
    _dormire_ms(C);
    nonus = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(nonus);
    CREDO_VERUM(_ad_finem((integer)tcp_obtinere_fd(nonus), M));
    tcp_claudere(nonus);
    /* tenentes vivi manent */
    CREDO_FALSUM(_ad_finem((integer)tcp_obtinere_fd(tenentes[I]), C));
    tcp_claudere(tenentes[ZEPHYRUM]);
    _dormire_ms(C);
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), ZEPHYRUM);
    per (k = I; k < VIII; k++)
    {
        tcp_claudere(tenentes[k]);
    }
    _finire(pid);
    _purgare(&l);
}

/* cliens abit dum vocatio dilata pendet: servus vivit, proximum
 * servit */
interior vacuum
_sectio_fuga (
    CredoContextus* c)
{
           Locus  l;
             s32  pid;
          chorda  t;
     TcpConnexio* cl;

    _locus(&l, "fuga");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    cl = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(cl);
    CREDO_VERUM(_scribere(cl, LINEA_INITII));
    CREDO_VERUM(_scribere(cl, LINEA_INITIATUM));
    CREDO_VERUM(_scribere(cl, LINEA_TARDE));
    tcp_claudere(cl);
    _dormire_ms(CCC);
    CREDO_VERUM(_vivus(pid));
    CREDO_AEQUALIS_S32(mcp_canalis_vocare(l.via, "echo",
        _argumenta_echo(c->piscina), &t, c->piscina), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "salve");
    _finire(pid);
    _purgare(&l);
}

/* connexio clauditur dum responsum dilatum pendet (linea longa), locus
 * eius statim iteratus: responsum serum ad connexionem NOVAM non
 * pervenit (generatio) */
interior vacuum
_sectio_locus_iteratus (
    CredoContextus* c)
{
           Locus  l;
             s32  pid;
     TcpConnexio* cl;
     TcpConnexio* nova;
       character* x;
       character  alveus[ALVEUS_PONTIS];
             i32  k;

    _locus(&l, "iteratus");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    _dormire_ms(C);
    cl = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(cl);
    CREDO_VERUM(_scribere(cl, LINEA_INITII));
    CREDO_VERUM(_scribere(cl, LINEA_INITIATUM));
    CREDO_VERUM(_scribere(cl, LINEA_TARDE_D));
    x = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(LXIV * MXXIV));
    CREDO_NECESSE_NON_NIHIL(x);
    memset(x, 'x', (memoriae_index)(LXIV * MXXIV));
    per (k = ZEPHYRUM; k < XVII; k++)
    {
        si (!tcp_mittere_omnia(cl, (constans i8*)x, LXIV * MXXIV))
        {
            frange;
        }
    }
    CREDO_VERUM(_ad_finem((integer)tcp_obtinere_fd(cl), MM));
    tcp_claudere(cl);
    /* locus solutus: connexio nova eum accipit; responsum 'tarde'
     * (D ms) post hoc venit */
    nova = _crudus(l.via, c->piscina);
    CREDO_NECESSE_NON_NIHIL(nova);
    CREDO_VERUM(_scribere(nova, LINEA_INITII));
    _dormire_ms(DCCC);
    (vacuum)_legere_lineas((integer)tcp_obtinere_fd(nova), alveus,
        ALVEUS_PONTIS, II, CC);
    CREDO_VERUM(strstr(alveus, "protocolVersion") != NIHIL);
    CREDO_VERUM(strstr(alveus, "tarde") == NIHIL);
    tcp_claudere(nova);
    CREDO_VERUM(_vivus(pid));
    _finire(pid);
    _purgare(&l);
}

/* filium exspectare usque ad 'ms': VERUM si exiit (status ponitur);
 * aliter necatur et FALSUM - pons fractus suitam non suspendit */
interior b32
_exspectare_filium (
        s32  pid,
        s32  ms,
    integer* status)
{
    s32 k;

    per (k = ZEPHYRUM; k < ms / X + I; k++)
    {
        si (waitpid((pid_t)pid, status, WNOHANG) == (pid_t)pid)
        {
            redde VERUM;
        }
        _dormire_ms(X);
    }
    _finire(pid);
    redde FALSUM;
}

/* filius: pons super fistulas; exitus = reditus pontis */
interior s32
_pons_filius (
    constans character* via,
               integer  intra,
               integer  extra,
               integer  claudenda_a,
               integer  claudenda_b)
{
    s32 pid;

    fflush(NIHIL);
    pid = (s32)fork();
    si (pid == ZEPHYRUM)
    {
        close(claudenda_a);
        close(claudenda_b);
        _exit((integer)mcp_canalis_pons(via, intra, extra));
    }
    redde pid;
}

/* sessio tota per pontem: II lineae responsi, EOF ordinatus -> 0 */
interior vacuum
_sectio_pons (
    CredoContextus* c)
{
        Locus l;
          s32 pid;
          s32 pons;
      integer in[II];
      integer ex[II];
      integer status = ZEPHYRUM;
    character alveus[ALVEUS_PONTIS];
          s32 n;
          s32 k;
          s32 lineae = ZEPHYRUM;

    _locus(&l, "pons");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    CREDO_NECESSE(pipe(in) == ZEPHYRUM && pipe(ex) == ZEPHYRUM);
    pons = _pons_filius(l.via, in[ZEPHYRUM], ex[I], in[I],
        ex[ZEPHYRUM]);
    CREDO_NECESSE(pons > ZEPHYRUM);
    close(in[ZEPHYRUM]);
    close(ex[I]);
    CREDO_VERUM(write(in[I], LINEA_INITII, strlen(LINEA_INITII)) > 0);
    CREDO_VERUM(write(in[I], LINEA_INITIATUM, strlen(LINEA_INITIATUM))
        > 0);
    CREDO_VERUM(write(in[I], LINEA_ECHO, strlen(LINEA_ECHO)) > 0);
    close(in[I]);
    n = _legere_lineas(ex[ZEPHYRUM], alveus, ALVEUS_PONTIS, III,
        MMM);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (alveus[k] == '\n')
        {
            lineae++;
        }
    }
    CREDO_AEQUALIS_S32(lineae, II);
    CREDO_VERUM(strstr(alveus, "salve") != NIHIL);
    CREDO_VERUM(strstr(alveus, "protocolVersion") != NIHIL);
    close(ex[ZEPHYRUM]);
    CREDO_NECESSE(_exspectare_filium(pons, MM, &status));
    CREDO_VERUM(WIFEXITED(status));
    CREDO_AEQUALIS_S32((s32)WEXITSTATUS(status), ZEPHYRUM);
    _finire(pid);
    _purgare(&l);
}

/* servus moritur dum pons connexus: pons intra M ms exit (Review
 * focus 1) */
interior vacuum
_sectio_pons_finis (
    CredoContextus* c)
{
        Locus l;
          s32 pid;
          s32 pons;
      integer in[II];
      integer ex[II];
      integer status = ZEPHYRUM;
    character alveus[ALVEUS_PONTIS];
          b32 exiit = FALSUM;

    _locus(&l, "pons_finis");
    pid = _servus_filius(l.via);
    CREDO_NECESSE(pid > ZEPHYRUM);
    CREDO_NECESSE(_paratus(l.via, c->piscina));
    CREDO_NECESSE(pipe(in) == ZEPHYRUM && pipe(ex) == ZEPHYRUM);
    pons = _pons_filius(l.via, in[ZEPHYRUM], ex[I], in[I],
        ex[ZEPHYRUM]);
    CREDO_NECESSE(pons > ZEPHYRUM);
    close(in[ZEPHYRUM]);
    close(ex[I]);
    CREDO_VERUM(write(in[I], LINEA_INITII, strlen(LINEA_INITII)) > 0);
    CREDO_MAIOR_S32(_legere_lineas(ex[ZEPHYRUM], alveus,
        ALVEUS_PONTIS, I, MM), ZEPHYRUM);
    _finire(pid);
    exiit = _exspectare_filium(pons, M, &status);
    CREDO_VERUM(exiit);
    si (exiit)
    {
        CREDO_VERUM(WIFEXITED(status));
        CREDO_MINOR_AUT_AEQUALIS_S32((s32)WEXITSTATUS(status), I);
    }
    close(in[I]);
    close(ex[ZEPHYRUM]);
    _purgare(&l);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vocare", _sectio_vocare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dilatum", _sectio_dilatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitium", _sectio_vitium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nemo", _sectio_nemo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea_longa", _sectio_linea_longa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea_imperfecta", _sectio_linea_imperfecta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nonus", _sectio_nonus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fuga", _sectio_fuga, NIHIL, NIHIL, NIHIL, NIHIL },
    { "locus_iteratus", _sectio_locus_iteratus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pons", _sectio_pons, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pons_finis", _sectio_pons_finis, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mcp_servus_canalis", SECTIONES);
}
