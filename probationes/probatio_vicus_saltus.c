/* probatio_vicus_saltus.c - saltus ad colloquium (colloquia-plan T4c),
 * VIVUS contra servitorem tmux PROPRIUM (socket in build/pmx/,
 * numquam servitor usoris): admonitio volans pro panello fenestrae
 * "secunda" sessionis alpha; ictus in ea -> nullum terminale vici
 * adhaeret: terminale novum in acervo tabulae activae ("tmux <socket>
 * alpha"), cliens eius (per tty) sessioni adhaeret, fenestra secunda
 * activa. Deinde tabula alia et latus aliud ante terminale: saltus
 * iterum ad tabulam et latus idem redit (nullum terminale novum). */
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
#include "componens.h"
#include "dispensator.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "terminale.h"
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
          character  binarium[CXXVIII];
                b32  paratus;
            Volumen* volumen;
    VicusApplicatio  app;
          character  liber[CCLVI];
                i32  admonitiones;   /* ids sessionum distincta */
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
    constans character* area;
             character  cwd[CCLVI];
    constans character* loci[] = { "/opt/homebrew/bin/tmux",
        "/usr/local/bin/tmux", "/usr/bin/tmux", NIHIL };
                   i32 i;

    s = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(s, ZEPHYRUM, magnitudo(Mundus));
    c->datum = s;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
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
    /* socket in build/pmx/ (debitum_scripturae): via $TMPDIR portae
     * > CIV octeti (sun_path macOS) */
    radix = getenv("RHUBARB_RADIX");
    si (!radix && getcwd(cwd, magnitudo(cwd)))
    {
        radix = cwd;
    }
    sprintf(s->socket, "%s/build/pmx", radix ? radix : ".");
    (vacuum)filum_directorium_creare_si_necesse(s->socket);
    sprintf(s->socket + strlen(s->socket), "/v.%d", (integer)getpid());
    s->m.binarium  = s->binarium;
    s->m.socket    = s->socket;
    area           = credo_area(c);
    s->volumen     = volumen_temporarium(c->piscina,
        "probatio_vicus_saltus");
    si (!area || !s->volumen)
    {
        redde;
    }
    sprintf(s->liber, "%s/eventa.jsonl", area);
    /* nomina fenestrarum (non indices: base-index usoris ignotus) */
    s->paratus = imperium_currere(s, "new-session", "-d", "-s", "alpha",
            "-n", "prima", c->piscina)
        && imperium_currere(s, "new-window", "-d", "-t", "alpha", "-n",
            "secunda", c->piscina)
        && filum_appendere_firmiter(s->liber, chorda_ex_literis("",
            c->piscina))
        && vicus_applicatio_aedificare(&s->app, c->piscina,
            internamentum_creare(c->piscina), s->volumen, CDLXXX,
            CDLXXX + VICUS_ALTITUDO_TABULARUM)
        && vicus_applicatio_admonitiones_aperire(&s->app, s->liber);
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

/* panellus fenestrae nominatae */
interior constans MultiplexorPanellus*
panellus_fenestrae (
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
            redde q;
        }
    }
    redde NIHIL;
}

/* admonitio nova (licentia) pro panello; pulsus; ictus in volante
 * prima (corpus) */
interior b32
admonere_et_salire (
             Mundus* s,
    constans chorda  panellus,
            Piscina* p)
{
         character  linea[DXII];
         Componens* v;
           Eventus  e;
               i32  k;

    s->admonitiones++;
    sprintf(linea,
        "{\"tempus\":%ld%03d,\"sessio\":\"s-%d\",\"eventus\":"
        "\"Notification\",\"panellus\":\"%.*s\",\"servitor\":\"%s\","
        "\"via\":\"/w/alpha\",\"transcriptum\":\"\",\"genus\":"
        "\"permission_prompt\",\"nuntius\":\"Claude needs your "
        "permission\"}\n", (long)time(NIHIL),
        (integer)s->admonitiones,
        (integer)s->admonitiones, (integer)panellus.mensura,
        (constans character*)panellus.datum, s->socket);
    si (!filum_appendere_firmiter(s->liber, chorda_ex_literis(linea,
        p)))
    {
        redde FALSUM;
    }
    v = NIHIL;
    per (k = ZEPHYRUM; k < X && !v; k++)
    {
        (vacuum)vicus_pulsare(s->app.vicus);
        dispensator_recomponere(s->app.d);
        v = componens_invenire_per_id(dispensator_arbor(s->app.d),
            chorda_ex_literis("v1", p));
    }
    si (!v)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.tempus            = M;
    e.datum.mus.x       = v->fines.x + v->fines.latitudo / II;
    e.datum.mus.y       = v->fines.y + v->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(s->app.d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(s->app.d, &e);
    (vacuum)vicus_pulsare(s->app.vicus);
    redde VERUM;
}

/* pulsare donec cliens tty dati sessioni adhaeret (V s) */
interior b32
adhaesionem_exspectare (
       Mundus* s,
       chorda  tty,
      Piscina* p)
{
                           Xar* l;
    constans MultiplexorCliens* k;
                           i32  gradus;
                           i32  i;
               struct timespec  pausa;

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
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;    /* XX ms */
        (vacuum)nanosleep(&pausa, NIHIL);
    }
    redde FALSUM;
}

interior VicusLatus*
frons (
    Vicus* v)
{
    redde vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
}

interior i32
terminalia_in_acervo (
    VicusTabula* t)
{
    i32 n;
    i32 k;

    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
    {
        si (chorda_aequalis_literis(((VicusLatus*)xar_obtinere(
            t->acervus, k))->genus, "terminale"))
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_probare_saltum (
    CredoContextus* c)
{
                          Mundus* s;
                           Vicus* v;
    constans MultiplexorPanellus* secunda;
                          chorda  id;
                          chorda  tty;
                      VicusLatus* l;
                       character  exspectatum[CCCLXXXIV];
                             i32  terminalia;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v        = s->app.vicus;
    secunda  = panellus_fenestrae(s, "secunda", c->piscina);
    CREDO_NECESSE(secunda != NIHIL);
    CREDO_FALSUM(secunda->fenestra_activa);
    id = chorda_transcribere(secunda->id, c->piscina);

    CREDO_GRADUS("I: nullum adhaerens - terminale novum");
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    CREDO_NECESSE(admonere_et_salire(s, id, c->piscina));
    l = frons(v);
    CREDO_NECESSE(l != NIHIL && l->montata);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "terminale");
    sprintf(exspectatum, "tmux %s alpha", s->socket);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, exspectatum);
    CREDO_AEQUALIS_I32(vicus_activa(v)->focus, VICUS_DEXTRUM);
    tty = chorda_transcribere(terminale_tty((TerminaleApplicatio*)
        l->montatio), c->piscina);
    CREDO_VERUM(adhaesionem_exspectare(s, tty, c->piscina));
    secunda = multiplexor_panellum_invenire(multiplexor_panelli(&s->m,
        c->piscina), id);
    CREDO_NECESSE(secunda != NIHIL);
    CREDO_VERUM(secunda->fenestra_activa);
    terminalia = (i32)terminalia_in_acervo(vicus_activa(v));

    CREDO_GRADUS("II: tabula alia, latus aliud ante - saltus redit");
    CREDO_NECESSE(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(v)->genus, "nomenclator");
    CREDO_NECESSE(vicus_focum_ponere(v, VICUS_SINISTRUM));
    CREDO_NECESSE(vicus_activam_ponere(v, "4"));
    /* fenestra prima in sessione activa (saltus eam mutabit) */
    CREDO_NECESSE(imperium_currere(s, "select-window", "-t",
        "=alpha:prima", NIHIL, NIHIL, NIHIL, c->piscina));
    CREDO_NECESSE(admonere_et_salire(s, id, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_activa(v)->id, "3");
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(v)->genus, "terminale");
    CREDO_VERUM(chorda_aequalis(terminale_tty((TerminaleApplicatio*)
        frons(v)->montatio), tty));
    CREDO_AEQUALIS_I32(vicus_activa(v)->focus, VICUS_DEXTRUM);
    CREDO_AEQUALIS_I32(terminalia_in_acervo(vicus_activa(v)),
        terminalia);
    CREDO_AEQUALIS_I32(terminalia_in_acervo(vicus_tabula(v, III)),
        ZEPHYRUM);
    secunda = multiplexor_panellum_invenire(multiplexor_panelli(&s->m,
        c->piscina), id);
    CREDO_NECESSE(secunda != NIHIL);
    CREDO_VERUM(secunda->fenestra_activa);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "saltus", _probare_saltum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_saltus", SECTIONES);
}
