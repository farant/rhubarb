/* probatio_multiplexor.c - tmux (colloquia-plan T1): lectio pura ex
 * fixis (panelli, clientes, lineae malae), inventio, vector
 * adhaerendi; vivus contra servitorem PROPRIUM (socket in build/pmx/,
 * numquam servitor usoris): panelli, sessio exsistit, eligere,
 * clientes, servitor absens, binarium absens. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "processus.h"
#include "multiplexor.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VIA_FIXORUM "probationes/fixa/multiplexor/"

/* panelli: lineae malae (sine tabulis, pid non numerus) omittuntur;
 * spatia in titulo et via servantur */
interior vacuum
_probare_panellos_legere (
    CredoContextus* c)
{
                             Xar* l;
    constans MultiplexorPanellus* p;

    l = multiplexor_panellos_legere(filum_legere_totum(VIA_FIXORUM
        "panelli.txt", c->piscina), c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), IV);
    p = multiplexor_panellum_invenire(l, chorda_ex_literis("%3",
        c->piscina));
    CREDO_NECESSE(p != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(p->sessio, "alpha");
    CREDO_AEQUALIS_S32(p->fenestra, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(p->fenestra_titulus,
        "cum spatiis in titulo");
    CREDO_AEQUALIS_S32(p->index, I);
    CREDO_AEQUALIS_S32(p->pid, M + II);
    CREDO_CHORDA_AEQUALIS_LITERIS(p->tty, "/dev/ttys002");
    CREDO_CHORDA_AEQUALIS_LITERIS(p->imperium, "2.1.296");
    CREDO_CHORDA_AEQUALIS_LITERIS(p->via, "/Users/x/via cum spatio");
    CREDO_FALSUM(p->activus);
    CREDO_FALSUM(p->fenestra_activa);
    p = multiplexor_panellum_invenire(l, chorda_ex_literis("%4",
        c->piscina));
    CREDO_NECESSE(p != NIHIL);
    CREDO_AEQUALIS_S32(p->index, II);
    CREDO_VERUM(p->activus);
    CREDO_VERUM(multiplexor_panellum_invenire(l, chorda_ex_literis("%9",
        c->piscina)) == NIHIL);
    CREDO_VERUM(multiplexor_panellum_invenire(l, chorda_ex_literis(
        "%99", c->piscina)) == NIHIL);
    /* effusio vacua: Xar vacuus, non NIHIL */
    l = multiplexor_panellos_legere(chorda_ex_literis("", c->piscina),
        c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
}

interior vacuum
_probare_clientes_legere (
    CredoContextus* c)
{
                           Xar* l;
    constans MultiplexorCliens* k;

    l = multiplexor_clientes_legere(filum_legere_totum(VIA_FIXORUM
        "clientes.txt", c->piscina), c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), II);
    k = (constans MultiplexorCliens*)xar_obtinere(l, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->tty, "/dev/ttys021");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->sessio, "beta");
}

interior b32
vector_est (
    constans character** v,
    constans character** exspectatus)
{
    i32 i;

    per (i = ZEPHYRUM; v && exspectatus[i]; i++)
    {
        si (!v[i] || strcmp(v[i], exspectatus[i]) != ZEPHYRUM)
        {
            imprimere("  vector[%d]: %s (exspectatum %s)\n", (integer)i,
                v[i] ? v[i] : "NIHIL", exspectatus[i]);
            redde FALSUM;
        }
    }
    redde v && v[i] == NIHIL;
}

/* vector adhaerendi: binarium datum (creditur); -L nomen, -S via,
 * nullum; sessio exacta ("=nomen" - tmux aliter praefixum congruit) */
interior vacuum
_probare_argumenta (
    CredoContextus* c)
{
           Multiplexor  m;
    constans character* nomine[] = { "/opt/x/tmux", "-u", "-L",
        "socius",
        "attach", "-t", "=alpha", NIHIL };
    constans character* via[] = { "/opt/x/tmux", "-u", "-S", "/tmp/x/s",
        "attach", "-t", "=beta", NIHIL };
    constans character* nullo[] = { "/opt/x/tmux", "-u", "attach", "-t",
        "=gamma", NIHIL };

    memset(&m, ZEPHYRUM, magnitudo(m));
    m.binarium  = "/opt/x/tmux";
    m.socket    = "socius";
    CREDO_VERUM(vector_est(multiplexor_argumenta_adhaerendi(&m,
        chorda_ex_literis("alpha", c->piscina), c->piscina), nomine));
    m.socket = "/tmp/x/s";
    CREDO_VERUM(vector_est(multiplexor_argumenta_adhaerendi(&m,
        chorda_ex_literis("beta", c->piscina), c->piscina), via));
    m.socket = NIHIL;
    CREDO_VERUM(vector_est(multiplexor_argumenta_adhaerendi(&m,
        chorda_ex_literis("gamma", c->piscina), c->piscina), nullo));
}


/* ==================================================
 * Vivus: servitor proprius in $TMPDIR
 * ================================================== */

nomen structura {
    Multiplexor m;
      character socket[CCLVI];
      character binarium[CXXVIII];
            b32 paratus;
} Minister;

interior b32
imperium_currere (
               Minister* s,
     constans character* a0,
     constans character* a1,
     constans character* a2,
     constans character* a3,
     constans character* a4,
     constans character* a5,
                Piscina* p)
{
    constans character* v[X];
     ProcessusResultus  r;

    v[ZEPHYRUM]  = s->binarium;
    v[I]         = "-S";
    v[II]        = s->socket;
    v[III]       = a0;
    v[IV]        = a1;
    v[V]         = a2;
    v[VI]        = a3;
    v[VII]       = a4;
    v[VIII]      = a5;
    v[IX]        = NIHIL;
    r            = processus_exsequi(v, V * M, p);
    redde r.successus && r.codex_exitus == ZEPHYRUM;
}

interior vacuum
ministrum_parare (
    CredoContextus* c)
{
              Minister* s;
    constans character* radix;
             character  cwd[CCLVI];
    constans character* loci[] = { "/opt/homebrew/bin/tmux",
        "/usr/local/bin/tmux", "/usr/bin/tmux", NIHIL };
              i32 i;

    s = (Minister*)piscina_allocare(c->piscina, magnitudo(Minister));
    memset(s, ZEPHYRUM, magnitudo(Minister));
    c->datum = s;
    per (i = ZEPHYRUM; loci[i]; i++)
    {
        si (filum_existit(loci[i]))
        {
            strcpy(s->binarium, loci[i]);
            frange;
        }
    }
    si (!s->binarium[ZEPHYRUM])
    {
        redde;
    }
    /* socket in build/pmx/ (debitum_scripturae): $TMPDIR portae in
     * area profunda est - via socket > CIV octeti (sun_path macOS),
     * tmux socket creare non potest */
    radix = getenv("RHUBARB_RADIX");
    si (!radix && getcwd(cwd, magnitudo(cwd)))
    {
        radix = cwd;
    }
    sprintf(s->socket, "%s/build/pmx", radix ? radix : ".");
    (vacuum)filum_directorium_creare_si_necesse(s->socket);
    sprintf(s->socket + strlen(s->socket), "/s.%d", (integer)getpid());
    s->m.binarium  = s->binarium;
    s->m.socket    = s->socket;
    /* sine localitate (ut vicus ex Doco et portae): bibliotheca -u
     * ponere debet, aliter tabulationes effusionis '_' fiunt */
    (vacuum)unsetenv("LANG");
    (vacuum)unsetenv("LC_ALL");
    (vacuum)unsetenv("LC_CTYPE");
    /* sessio alpha: fenestra I "prima" (activa), fenestra II
     * "secunda" cum panellis duobus (-d: primus manet activus) */
    s->paratus = imperium_currere(s, "new-session", "-d", "-s", "alpha",
            "-n", "prima", c->piscina)
        && imperium_currere(s, "new-window", "-d", "-t", "alpha", "-n",
            "secunda", c->piscina)
        && imperium_currere(s, "split-window", "-d", "-t", "alpha:2",
            NIHIL, NIHIL, c->piscina);
}

interior vacuum
ministrum_purgare (
    CredoContextus* c)
{
    Minister* s;

    s = (Minister*)c->datum;
    si (s && s->binarium[ZEPHYRUM])
    {
        (vacuum)imperium_currere(s, "kill-server", NIHIL, NIHIL, NIHIL,
            NIHIL, NIHIL, c->piscina);
        (vacuum)filum_delere(s->socket);
    }
}

/* panellum fenestrae et indicis */
interior constans MultiplexorPanellus*
panellus_ad (
    Xar* l,
    s32  fenestra,
    s32  index)
{
    constans MultiplexorPanellus* p;
                             i32  i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        p = (constans MultiplexorPanellus*)xar_obtinere(l, i);
        si (p->fenestra == fenestra && p->index == index)
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior vacuum
_probare_vivum (
    CredoContextus* c)
{
                        Minister* s;
                             Xar* l;
                             Xar* k;
    constans MultiplexorPanellus* p;
                          chorda  id;
                     Multiplexor  absens;

    s = (Minister*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    l = multiplexor_panelli(&s->m, c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), III);
    CREDO_VERUM(multiplexor_sessio_exsistit(&s->m, chorda_ex_literis(
        "alpha", c->piscina), c->piscina));
    CREDO_FALSUM(multiplexor_sessio_exsistit(&s->m, chorda_ex_literis(
        "nulla", c->piscina), c->piscina));
    /* praefixum non congruit (scopus exactus "=alpha") */
    CREDO_FALSUM(multiplexor_sessio_exsistit(&s->m, chorda_ex_literis(
        "alp", c->piscina), c->piscina));

    /* fenestra II, panellus II (scissura -d: non activus): eligere */
    p = panellus_ad(l, II, II);
    CREDO_NECESSE(p != NIHIL);
    CREDO_FALSUM(p->activus);
    CREDO_FALSUM(p->fenestra_activa);
    id = chorda_transcribere(p->id, c->piscina);
    CREDO_VERUM(multiplexor_panellum_eligere(&s->m, id, c->piscina));
    l = multiplexor_panelli(&s->m, c->piscina);
    p = multiplexor_panellum_invenire(l, id);
    CREDO_NECESSE(p != NIHIL);
    CREDO_VERUM(p->activus);
    CREDO_VERUM(p->fenestra_activa);
    p = panellus_ad(l, I, I);
    CREDO_NECESSE(p != NIHIL);
    CREDO_FALSUM(p->fenestra_activa);

    /* nullus cliens: Xar vacuus; vertere clientem ignotum deficit */
    k = multiplexor_clientes(&s->m, c->piscina);
    CREDO_NECESSE(k != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(k), ZEPHYRUM);
    CREDO_FALSUM(multiplexor_clientem_vertere(&s->m, chorda_ex_literis(
        "/dev/ttys999", c->piscina), id, c->piscina));

    /* id ignotum: eligere deficit */
    CREDO_FALSUM(multiplexor_panellum_eligere(&s->m, chorda_ex_literis(
        "%999", c->piscina), c->piscina));

    /* servitor absens et binarium absens: NIHIL */
    absens         = s->m;
    absens.socket  = "/nusquam/probatio_multiplexor";
    CREDO_VERUM(multiplexor_panelli(&absens, c->piscina) == NIHIL);
    absens           = s->m;
    absens.binarium  = "/nusquam/tmux";
    CREDO_VERUM(multiplexor_panelli(&absens, c->piscina) == NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "panellos_legere", _probare_panellos_legere, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "clientes_legere", _probare_clientes_legere, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "argumenta", _probare_argumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vivus", _probare_vivum, ministrum_parare, ministrum_purgare,
      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("multiplexor", SECTIONES);
}
