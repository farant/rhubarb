/* probatio_vicus_comites_terminale.c - comites P4: locus terminalis.
 * VIVUS contra servitorem tmux PROPRIUM (socket in build/pmx/,
 * TMUX_TMPDIR proprium - numquam servitor usoris; ut
 * probatio_vicus_saltus). Terminale adhaerens sessioni "alpha"
 * (fenestrae "prima", "secunda"): locus "terminale/tmux/alpha/<index
 * fenestrae activae>", titulus "alpha / <nomen fenestrae>";
 * select-window -> locus novus post memoriam (M ms); Cmd+T comes
 * terminalis et sequela per fenestras; focus in sinistrum et Cmd+T
 * locum statim recentem quaerunt (facies.locum_renovare). Terminale
 * sine tmux: locus "terminale/<id lateris>", titulus "Terminal". */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "filum.h"
#include "processus.h"
#include "multiplexor.h"
#include "insula.h"
#include "dispensator.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "terminale.h"
#include "scriba_applicatio.h"
#include "scriba_liber.h"
#include "thema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

nomen structura {
        Multiplexor  m;
          character  socket[CCLVI];
          character  tmpdir[CCLVI];
          character  binarium[CXXVIII];
                b32  paratus;
            Volumen* volumen;
    VicusApplicatio  app;
} Mundus;

interior b32
imperium_currere (
                 Mundus* s,
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
mundum_parare (
    CredoContextus* c)
{
                Mundus* s;
    constans character* radix;
             character  cwd[CCLVI];
    constans character* loci[] = { "/opt/homebrew/bin/tmux",
        "/usr/local/bin/tmux", "/usr/bin/tmux", NIHIL };
                   i32 i;

    s = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(s, ZEPHYRUM, magnitudo(Mundus));
    c->datum = s;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    (vacuum)unsetenv("TMUX");
    (vacuum)unsetenv("TMUX_PANE");
    (vacuum)unsetenv("LANG");
    (vacuum)unsetenv("LC_ALL");
    (vacuum)unsetenv("LC_CTYPE");
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
    radix = getenv("RHUBARB_RADIX");
    si (!radix && getcwd(cwd, magnitudo(cwd)))
    {
        radix = cwd;
    }
    sprintf(s->socket, "%s/build/pmx", radix ? radix : ".");
    (vacuum)filum_directorium_creare_si_necesse(s->socket);
    sprintf(s->socket + strlen(s->socket), "/c.%d", (integer)getpid());
    /* servitor ordinarius sub TMUX_TMPDIR proprio: numquam usoris */
    sprintf(s->tmpdir, "%s/build/pmx", radix ? radix : ".");
    (vacuum)setenv("TMUX_TMPDIR", s->tmpdir, I);
    s->m.binarium  = s->binarium;
    s->m.socket    = s->socket;
    s->volumen     = volumen_temporarium(c->piscina,
        "probatio_vicus_comites_terminale");
    si (!s->volumen)
    {
        redde;
    }
    /* nomina fenestrarum explicita: automatic-rename non mutat */
    s->paratus = imperium_currere(s, "new-session", "-d", "-s", "alpha",
            "-n", "prima", c->piscina)
        && imperium_currere(s, "new-window", "-d", "-t", "alpha", "-n",
            "secunda", c->piscina)
        && vicus_applicatio_aedificare(&s->app, c->piscina,
            internamentum_creare(c->piscina), s->volumen, CDLXXX,
            CDLXXX + VICUS_ALTITUDO_TABULARUM)
        && vicus_activam_ponere(s->app.vicus, "3");
    si (s->paratus)
    {
        (vacuum)vicus_pulsare(s->app.vicus);
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* s;

    s = (Mundus*)c->datum;
    si (s && s->binarium[ZEPHYRUM])
    {
        (vacuum)imperium_currere(s, "kill-server", NIHIL, NIHIL, NIHIL,
            NIHIL, NIHIL, c->piscina);
        (vacuum)filum_delere(s->socket);
    }
    si (s && s->volumen)
    {
        volumen_claudere(s->volumen);
    }
}

interior vacuum
dormire_ms (
    s32 ms)
{
    struct timespec pausa;

    pausa.tv_sec   = ms / M;
    pausa.tv_nsec  = (long)(ms % M) * M * M;
    (vacuum)nanosleep(&pausa, NIHIL);
}

/* index fenestrae nominatae (base-index usoris ignotus); -1 si abest */
interior s32
index_fenestrae (
                 Mundus* s,
     constans character* fenestra,
                Piscina* p)
{
                             Xar* l;
    constans MultiplexorPanellus* q;
                             i32  i;

    l = multiplexor_panelli(&s->m, p);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        q = (constans MultiplexorPanellus*)xar_obtinere(l, i);
        si (chorda_aequalis_literis(q->fenestra_titulus, fenestra))
        {
            redde q->fenestra;
        }
    }
    redde -I;
}

/* pulsare donec cliens tty dati sessioni adhaeret (V s) */
interior b32
adhaerere (
     Mundus* s,
     chorda  tty,
    Piscina* p)
{
                           Xar* l;
    constans MultiplexorCliens* k;
                           i32  gradus;
                           i32  i;

    per (gradus = ZEPHYRUM; gradus < CCL; gradus++)
    {
        (vacuum)vicus_pulsare(s->app.vicus);
        l = multiplexor_clientes(&s->m, p);
        per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
        {
            k = (constans MultiplexorCliens*)xar_obtinere(l, i);
            si (   chorda_aequalis(k->tty, tty)
                && chorda_aequalis_literis(k->sessio, "alpha"))
            {
                redde VERUM;
            }
        }
        dormire_ms(XX);
    }
    redde FALSUM;
}

interior b32
locus_frontis_est (
                 Mundus* s,
     constans character* locus,
     constans character* titulus,
                Piscina* p)
{
    VicusLatus* l;
        chorda  a;
        chorda  b;

    l = vicus_latus(vicus_activa(s->app.vicus), VICUS_DEXTRUM);
    a = vicus_latus_locus(s->app.vicus, l, p);
    b = vicus_latus_locus_titulus(s->app.vicus, l, p);
    si (   !chorda_aequalis_literis(a, locus)
        || !chorda_aequalis_literis(b, titulus))
    {
        fprintf(stderr, "  locus '%.*s' titulus '%.*s'\n",
            (int)a.mensura, (constans character*)a.datum,
            (int)b.mensura, (constans character*)b.datum);
        redde FALSUM;
    }
    redde VERUM;
}

/* praefixum libri scribae sinistrae */
interior b32
comes_sinister_est (
                 Mundus* s,
     constans character* praefixum)
{
         VicusLatus* l;
     ScribaMontatio* m;

    l = vicus_latus(vicus_activa(s->app.vicus), VICUS_SINISTRUM);
    m = l ? (ScribaMontatio*)l->montatio : NIHIL;
    redde m && m->liber && chorda_aequalis_literis(
        scriba_liber_praefixum(m->liber), praefixum);
}

interior vacuum
_sectio_tmux (
    CredoContextus* c)
{
        Mundus* s;
    VicusLatus* l;
     character  argumentum[CD];
     character  locus[CCLVI];
     character  comes[CCLVI];
           s32  prima;
           s32  secunda;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    prima    = index_fenestrae(s, "prima", c->piscina);
    secunda  = index_fenestrae(s, "secunda", c->piscina);
    CREDO_NECESSE(prima >= ZEPHYRUM && secunda >= ZEPHYRUM);
    sprintf(argumentum, "tmux %s alpha", s->socket);
    CREDO_NECESSE(vicus_acervo_aperire(s->app.vicus, "terminale",
        argumentum));
    (vacuum)vicus_pulsare(s->app.vicus);
    l = vicus_latus(vicus_activa(s->app.vicus), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata
        && chorda_aequalis_literis(l->genus, "terminale"));
    CREDO_NECESSE(adhaerere(s, terminale_tty((constans
        TerminaleApplicatio*)l->montatio), c->piscina));

    CREDO_GRADUS("fenestra activa: prima");
    dormire_ms(DC);
    sprintf(locus, "terminale/tmux/alpha/%d", (integer)prima);
    CREDO_VERUM(locus_frontis_est(s, locus, "alpha / prima",
        c->piscina));

    CREDO_GRADUS("Cmd+T: comes fenestrae");
    CREDO_VERUM(vicus_comitem_commutare(s->app.vicus));
    (vacuum)vicus_pulsare(s->app.vicus);
    sprintf(comes, "comes/terminale/tmux/alpha/%d", (integer)prima);
    CREDO_VERUM(comes_sinister_est(s, comes));

    CREDO_GRADUS("select-window: locus et comes sequuntur");
    CREDO_NECESSE(imperium_currere(s, "select-window", "-t",
        "alpha:secunda", NIHIL, NIHIL, NIHIL, c->piscina));
    dormire_ms(M + CC);
    (vacuum)vicus_pulsare(s->app.vicus);
    sprintf(locus, "terminale/tmux/alpha/%d", (integer)secunda);
    CREDO_VERUM(locus_frontis_est(s, locus, "alpha / secunda",
        c->piscina));
    (vacuum)vicus_pulsare(s->app.vicus);
    sprintf(comes, "comes/terminale/tmux/alpha/%d", (integer)secunda);
    CREDO_VERUM(comes_sinister_est(s, comes));

    CREDO_GRADUS("focus in sinistrum: locus statim recens");
    CREDO_VERUM(vicus_focum_ponere(s->app.vicus, VICUS_DEXTRUM));
    CREDO_NECESSE(imperium_currere(s, "select-window", "-t",
        "alpha:prima", NIHIL, NIHIL, NIHIL, c->piscina));
    CREDO_VERUM(vicus_focum_ponere(s->app.vicus, VICUS_SINISTRUM));
    (vacuum)vicus_pulsare(s->app.vicus);
    sprintf(comes, "comes/terminale/tmux/alpha/%d", (integer)prima);
    CREDO_VERUM(comes_sinister_est(s, comes));

    CREDO_GRADUS("Cmd+T: locus statim recens");
    CREDO_VERUM(vicus_comitem_commutare(s->app.vicus));
    CREDO_VERUM(comes_sinister_est(s, ""));
    CREDO_VERUM(vicus_focum_ponere(s->app.vicus, VICUS_DEXTRUM));
    CREDO_NECESSE(imperium_currere(s, "select-window", "-t",
        "alpha:secunda", NIHIL, NIHIL, NIHIL, c->piscina));
    CREDO_VERUM(vicus_comitem_commutare(s->app.vicus));
    sprintf(comes, "comes/terminale/tmux/alpha/%d", (integer)secunda);
    CREDO_VERUM(comes_sinister_est(s, comes));
}

/* terminale sine tmux (tabula 1, dextrum): locus lateris */
interior vacuum
_sectio_sine_tmux (
    CredoContextus* c)
{
    Mundus* s;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    CREDO_NECESSE(vicus_activam_ponere(s->app.vicus, "1"));
    (vacuum)vicus_pulsare(s->app.vicus);
    CREDO_VERUM(locus_frontis_est(s, "terminale/1_dextrum_terminale",
        "Terminal", c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tmux", _sectio_tmux, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "sine_tmux", _sectio_sine_tmux, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_comites_terminale", SECTIONES);
}
