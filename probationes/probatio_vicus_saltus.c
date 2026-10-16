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
#include "colloquia_visus.h"
#include "thema.h"
#include "manus_ludus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

nomen structura {
        Multiplexor  m;
          character  socket[CCLVI];
          character  binarium[CXXVIII];
                b32  paratus;
            Volumen* volumen;
    VicusApplicatio  app;
          character  liber[CCLVI];
                i32  admonitiones;   /* ids sessionum distincta */
    /* T6: servitor "ordinarius" sub TMUX_TMPDIR proprio (numquam
     * usoris); vicus alter in eodem volumine (restitutio) */
          character tmpdir[CCLVI];
          character communis[CCLVI];
    VicusApplicatio altera;
} Mundus;

interior b32
imperium_in (
                 Mundus* s,
     constans character* socket,
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
    v[II]        = socket;
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
    redde imperium_in(s, s->socket, a0, a1, a2, a3, a4, a5, p);
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
    /* T6: TMUX_TMPDIR proprium - "tmux attach" sine -S servitorem
     * ordinarium hic quaerit, numquam usoris */
    sprintf(s->tmpdir, "%s/build/pmx/t.%d", radix ? radix : ".",
        (integer)getpid());
    (vacuum)mkdir(s->tmpdir, (mode_t)0700);
    (vacuum)setenv("TMUX_TMPDIR", s->tmpdir, I);
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
        si (s->communis[ZEPHYRUM])
        {
            (vacuum)imperium_in(s, s->communis, "kill-server", NIHIL,
                NIHIL, NIHIL, NIHIL, NIHIL, c->piscina);
            (vacuum)filum_delere(s->communis);
            *strrchr(s->communis, '/') = '\0';
            (vacuum)rmdir(s->communis);
        }
        (vacuum)rmdir(s->tmpdir);
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

/* pulsare (vicus datus) donec cliens tty dati sessioni adhaeret in
 * servitore dato (V s) */
interior b32
adhaesionem_in (
                   Vicus* v,
    constans Multiplexor* mx,
      constans character* sessio,
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
        (vacuum)vicus_pulsare(v);
        l = multiplexor_clientes(mx, p);
        per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
        {
            k = (constans MultiplexorCliens*)xar_obtinere(l, i);
            si (   chorda_aequalis(k->tty, tty)
                && chorda_aequalis_literis(k->sessio, sessio))
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

/* cliens tty dati in servitore dato nunc adhaeret? */
interior b32
adhaeret (
    constans Multiplexor* mx,
                  chorda  tty,
                 Piscina* p)
{
                           Xar* l;
                           i32  i;

    l = multiplexor_clientes(mx, p);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        constans MultiplexorCliens* k;

        k = (constans MultiplexorCliens*)xar_obtinere(l, i);
        si (chorda_aequalis(k->tty, tty))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
adhaesionem_exspectare (
       Mundus* s,
       chorda  tty,
      Piscina* p)
{
    redde adhaesionem_in(s->app.vicus, &s->m, "alpha", tty, p);
}

/* textus intra D ms? */
interior b32
textum_brevi (
                   Vicus* v,
     TerminaleApplicatio* t,
      constans character* textus,
                 Piscina* p)
{
                i32 gradus;
    struct timespec  pausa;

    per (gradus = ZEPHYRUM; gradus < XXV; gradus++)
    {
        (vacuum)vicus_pulsare(v);
        si (chorda_continet(aemulator_textum_effundere(
                aemulator_hospes_aemulator(t->hospes), p),
                chorda_ex_literis(textus, p)))
        {
            redde VERUM;
        }
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;
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

/* T5: Enter in latere sessionum -> saltus ad panellum sessionis
 * (terminale novum adhaerens, fenestra secunda activa) */
interior vacuum
_probare_sessiones (
    CredoContextus* c)
{
                          Mundus* s;
                           Vicus* v;
    constans MultiplexorPanellus* secunda;
                          chorda  id;
                       character  linea[DXII];
                      VicusLatus* l;
                      ManusLudus* m;
                             i32  k;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v        = s->app.vicus;
    secunda  = panellus_fenestrae(s, "secunda", c->piscina);
    CREDO_NECESSE(secunda != NIHIL);
    id = chorda_transcribere(secunda->id, c->piscina);
    /* sessio laborans in panello secundae */
    sprintf(linea, "{\"tempus\":%ld000,\"sessio\":\"s-viva\","
        "\"eventus\":\"UserPromptSubmit\",\"panellus\":\"%.*s\","
        "\"servitor\":\"%s\",\"via\":\"/w/alpha\",\"transcriptum\":"
        "\"\",\"genus\":\"\",\"nuntius\":\"working\"}\n",
        (long)time(NIHIL), (integer)id.mensura,
        (constans character*)id.datum, s->socket);
    CREDO_NECESSE(filum_appendere_firmiter(s->liber, chorda_ex_literis(
        linea, c->piscina)));
    /* sessio altera in panello quod in servitore non est: clausa */
    sprintf(linea, "{\"tempus\":%ld000,\"sessio\":\"s-mortua\","
        "\"eventus\":\"UserPromptSubmit\",\"panellus\":\"%%99\","
        "\"servitor\":\"%s\",\"via\":\"/w/beta\",\"transcriptum\":"
        "\"\",\"genus\":\"\",\"nuntius\":\"gone\"}\n",
        (long)time(NIHIL) - I, s->socket);
    CREDO_NECESSE(filum_appendere_firmiter(s->liber, chorda_ex_literis(
        linea, c->piscina)));
    CREDO_NECESSE(vicus_activam_ponere(v, "5"));
    CREDO_NECESSE(vicus_acervo_aperire(v, "colloquia", NIHIL));
    per (k = ZEPHYRUM; k < V; k++)
    {
        (vacuum)vicus_pulsare(v);
    }
    l = frons(v);
    CREDO_NECESSE(l && chorda_aequalis_literis(l->genus, "colloquia"));
    /* vivi per servitorem suum: s-viva vivit, s-mortua clausa */
    {
                         Xar* sessiones;
         constans Colloquium* k;
                         i32  j;

        sessiones = colloquia_visus_fontis(((ColloquiorumMontatio*)
            l->montatio)->fons, c->piscina);
        CREDO_NECESSE(sessiones != NIHIL
            && xar_numerus(sessiones) == II);
        per (j = ZEPHYRUM; j < II; j++)
        {
            k = (constans Colloquium*)xar_obtinere(sessiones, j);
            si (chorda_aequalis_literis(k->sessio, "s-viva"))
            {
                CREDO_VERUM(k->panellus_vivit);
                CREDO_AEQUALIS_S32((s32)k->status,
                    (s32)COLLOQUIUM_LABORAT);
            }
            alioquin
            {
                CREDO_FALSUM(k->panellus_vivit);
                CREDO_AEQUALIS_S32((s32)k->status,
                    (s32)COLLOQUIUM_CLAUSUM);
            }
        }
    }
    CREDO_NECESSE(vicus_focum_ponere(v, VICUS_DEXTRUM));
    dispensator_recomponere(s->app.d);
    /* sessio viva (panellus in servitore SUO) - non clausa: ordo r1;
     * ictus in eo (focus et electa), deinde Enter */
    m = manus_ludus_creare(c->piscina, s->app.d);
    CREDO_NECESSE(m != NIHIL);
    CREDO_NECESSE(manus_ludus_premere(m, "#r1"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    (vacuum)vicus_pulsare(v);
    l = frons(v);
    CREDO_NECESSE(l && l->montata);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "terminale");
    CREDO_VERUM(adhaesionem_exspectare(s, chorda_transcribere(
        terminale_tty((TerminaleApplicatio*)l->montatio), c->piscina),
        c->piscina));
    secunda = multiplexor_panellum_invenire(multiplexor_panelli(&s->m,
        c->piscina), id);
    CREDO_NECESSE(secunda != NIHIL);
    CREDO_VERUM(secunda->fenestra_activa);
}

/* T6: terminale planum, "tmux attach" manu (servitor communis sub
 * TMUX_TMPDIR) -> argumentum notatur; vicus alter in eodem volumine
 * iterum adhaeret; post detachmentum concha manet */
interior vacuum
_probare_restitutionem (
    CredoContextus* c)
{
                  Mundus* s;
                   Vicus* v;
              VicusLatus* l;
              VicusLatus* prima;
                  chorda  tty_prima;
                     b32  visum;
     TerminaleApplicatio* t;
             Multiplexor  communis;
                  chorda  tty;
               character  imperium[CCCLXXXIV];
               character  exspectatum[XXXII];
                     i32  gradus;
        struct timespec  pausa;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v = s->app.vicus;
    memset(&communis, ZEPHYRUM, magnitudo(communis));
    communis.binarium = s->binarium;
    /* servitor "ordinarius" sine -S (TMUX_TMPDIR; tmux directorium
     * tmux-<uid> ipse creat), deinde via socket eius */
    {
         constans character* v_novae[VII];
         constans character* v_viae[VIII];
          ProcessusResultus  r;

        v_novae[ZEPHYRUM] = s->binarium;
        v_novae[I] = "new-session";
        v_novae[II] = "-d";
        v_novae[III] = "-s";
        v_novae[IV] = "beta";
        v_novae[V] = NIHIL;
        r = processus_exsequi(v_novae, V * M, c->piscina);
        CREDO_NECESSE(r.successus && r.codex_exitus == ZEPHYRUM);
        v_viae[ZEPHYRUM] = s->binarium;
        v_viae[I] = "display-message";
        v_viae[II] = "-p";
        v_viae[III] = "-t";
        v_viae[IV] = "=beta";
        v_viae[V] = "#{socket_path}";
        v_viae[VI] = NIHIL;
        r = processus_exsequi(v_viae, V * M, c->piscina);
        CREDO_NECESSE(   r.successus && r.effusio.mensura > I
                      && r.effusio.mensura < CCL);
        sprintf(s->communis, "%.*s", (integer)(r.effusio.mensura - I),
            (constans character*)r.effusio.datum);
    }

    CREDO_GRADUS("I: tmux attach manu - argumentum notatur");
    CREDO_NECESSE(vicus_activam_ponere(v, "6"));
    CREDO_NECESSE(vicus_acervo_aperire(v, "terminale", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = frons(v);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "terminale"));
    CREDO_AEQUALIS_I32(l->argumentum.mensura, ZEPHYRUM);
    t    = (TerminaleApplicatio*)l->montatio;
    tty  = chorda_transcribere(terminale_tty(t), c->piscina);
    sprintf(imperium, "%s attach -t =beta\r", s->binarium);
    CREDO_NECESSE(aemulator_hospes_scribere(t->hospes,
        (constans i8*)imperium, (i32)strlen(imperium)) > ZEPHYRUM);
    CREDO_VERUM(adhaesionem_in(v, &communis, "beta", tty, c->piscina));
    /* systema per V s: argumentum notatur */
    per (gradus = ZEPHYRUM; gradus < CDL
                            && l->argumentum.mensura == ZEPHYRUM;
         gradus++)
    {
        (vacuum)vicus_pulsare(v);
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;
        (vacuum)nanosleep(&pausa, NIHIL);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - beta");
    prima      = l;
    tty_prima  = tty;

    CREDO_GRADUS("II: restitutio - vicus alter iterum adhaeret");
    CREDO_NECESSE(vicus_applicatio_aedificare(&s->altera, c->piscina,
        internamentum_creare(c->piscina), s->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    l = vicus_latus(vicus_tabula(s->altera.vicus, V), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "terminale"));
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - beta");
    t    = (TerminaleApplicatio*)l->montatio;
    tty  = chorda_transcribere(terminale_tty(t), c->piscina);
    CREDO_VERUM(adhaesionem_in(s->altera.vicus, &communis, "beta", tty,
        c->piscina));

    CREDO_GRADUS("III: detachmentum - concha manet");
    CREDO_NECESSE(imperium_in(s, s->communis, "detach-client", "-t",
        chorda_ut_cstr(tty, c->piscina), NIHIL, NIHIL, NIHIL,
        c->piscina));
    sprintf(exspectatum, "post_%s", "detachmentum");
    sprintf(imperium, "echo post_\"\"detachmentum\r");
    /* concha post exitum tmux: exspectare donec cliens abiit (aliter
     * claves ad tmux exeuntem eunt), deinde textus scriptus redit */
    per (gradus = ZEPHYRUM; gradus < CCL && adhaeret(&communis, tty,
                            c->piscina); gradus++)
    {
        (vacuum)vicus_pulsare(s->altera.vicus);
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;
        (vacuum)nanosleep(&pausa, NIHIL);
    }
    per (gradus = ZEPHYRUM; gradus < XXV; gradus++)
    {
        (vacuum)vicus_pulsare(s->altera.vicus);
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;
        (vacuum)nanosleep(&pausa, NIHIL);
    }
    /* tmux exiens terminale cum TCSAFLUSH restituit: initus pendens
     * abicitur - textus iteratur donec concha respondet (ut homo) */
    visum = FALSUM;
    per (gradus = ZEPHYRUM; gradus < X && !visum; gradus++)
    {
        CREDO_NECESSE(aemulator_hospes_scribere(t->hospes,
            (constans i8*)imperium, (i32)strlen(imperium)) > ZEPHYRUM);
        visum = textum_brevi(s->altera.vicus, t, exspectatum,
            c->piscina);
    }
    CREDO_VERUM(visum);
    CREDO_FALSUM(l->finita);

    CREDO_GRADUS("IV: detachmentum in vico primo - argumentum abit");
    CREDO_NECESSE(imperium_in(s, s->communis, "detach-client", "-t",
        chorda_ut_cstr(tty_prima, c->piscina), NIHIL, NIHIL, NIHIL,
        c->piscina));
    per (gradus = ZEPHYRUM; gradus < CDL
                            && prima->argumentum.mensura > ZEPHYRUM;
         gradus++)
    {
        (vacuum)vicus_pulsare(v);
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = XX * M * M;
        (vacuum)nanosleep(&pausa, NIHIL);
    }
    CREDO_AEQUALIS_I32(prima->argumentum.mensura, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "saltus", _probare_saltum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "sessiones", _probare_sessiones, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "restitutio", _probare_restitutionem, mundum_parare,
      mundum_purgare, "lentus", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_saltus", SECTIONES);
}
