/* probatio_vicus_quies.c - vicus otiosus non laborat (vicus quies,
 * project-specs/vicus-quies-plan.md)
 *
 * T3: semitae laterum solum post scripturam in insula leguntur
 * (lectiones numerantur: otium nihil legit).
 *
 * T2: systema "terminalia" (clientes tmux per V s) - sine latere
 * terminali nullus tmux incipitur; cum latere quaestio SINE
 * EXSPECTATIONE currit (pulsus statim redit, resultus in pulsu
 * posteriore applicatur). tmux FICTUS (scriptum in area, primum in
 * PATH): vocationes in librum scribit, deinde dormit et clientem
 * fictum effundit - nullus servitor tmux verus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "filum.h"
#include "insula.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "terminale.h"
#include "scriba_applicatio.h"
#include "reactor.h"
#include "aemulator_hospes.h"
#include "fenestra_tempus.h"
#include "thema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
          character  liber[CCLVI];      /* praeconium (vacuum) */
          character  vocationes[CCLVI]; /* liber tmux ficti */
          character  clientes[CCLVI];   /* effusio tmux ficti */
          character  semita[CCLVI];     /* PATH prior */
                b32  paratus;
} Mundus;

/* tmux fictus: vocationem notat, 'mora' dormit, clientes effundit */
interior b32
tmux_fingere (
                 Mundus* s,
     constans character* area,
     constans character* mora)
{
    character via[CCLVI];
    character scriptum[DCCLXVIII];
    character bin[CCLVI];

    sprintf(bin, "%s/bin", area);
    sprintf(via, "%s/tmux", bin);
    sprintf(s->vocationes, "%s/vocationes", area);
    sprintf(s->clientes, "%s/clientes", area);
    sprintf(scriptum, "#!/bin/sh\n"
        "printf '%%s %%s\\n' \"$1\" \"$2\" >> '%s'\n"
        "/bin/sleep %s\n"
        "[ -f '%s' ] && /bin/cat '%s'\n"
        "exit 0\n", s->vocationes, mora, s->clientes, s->clientes);
    redde filum_directorium_creare_si_necesse(bin)
        && filum_scribere_literis(via, scriptum)
        && filum_modum_ponere(via, 0755)
        && setenv("PATH", bin, I) == ZEPHYRUM;
}

/* dispositio sine terminali (ordinaria terminale in tabula I habet):
 * latera terminalia sola quae probatio aperit */
#define DISPOSITIO_SCRIBAE                                          \
    "<tabulae activa=\"1\"><tabula id=\"1\" focus=\"sinistrum\">" \
    "<latus genus=\"scriba\"/><acervus><latus genus=\"scriba\"/>"    \
    "</acervus></tabula></tabulae>"
/* T3: scriba | terminale (terminale per pulsum scribens caperetur) */
#define DISPOSITIO_TERMINALIS                                       \
    "<tabulae activa=\"1\"><tabula id=\"1\" focus=\"sinistrum\">" \
    "<latus genus=\"scriba\"/><acervus><latus genus=\"terminale\"/>" \
    "</acervus></tabula></tabulae>"

interior vacuum
mundum_struere (
         CredoContextus* c,
     constans character* dispositio)
{
                Mundus* s;
    constans character* area;
    constans character* semita;

    s = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(s, ZEPHYRUM, magnitudo(Mundus));
    c->datum = s;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)unsetenv("TMUX");
    (vacuum)unsetenv("TMUX_PANE");
    semita = getenv("PATH");
    si (semita && strlen(semita) < magnitudo(s->semita))
    {
        strcpy(s->semita, semita);
    }
    area = credo_area(c);
    s->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_quies");
    si (!area || !s->volumen)
    {
        redde;
    }
    sprintf(s->liber, "%s/eventa.jsonl", area);
    s->paratus = volumen_praesens_ponere(s->volumen,
            chorda_ex_literis("vicus/latera", c->piscina),
            chorda_ex_literis(dispositio, c->piscina))
        && filum_appendere_firmiter(s->liber, chorda_ex_literis("",
            c->piscina))
        && vicus_applicatio_aedificare(&s->app, c->piscina,
            internamentum_creare(c->piscina), s->volumen, CDLXXX,
            CDLXXX + VICUS_ALTITUDO_TABULARUM)
        && vicus_applicatio_admonitiones_aperire(&s->app, s->liber);
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    mundum_struere(c, DISPOSITIO_SCRIBAE);
}

interior vacuum
mundum_terminalis_parare (
    CredoContextus* c)
{
    mundum_struere(c, DISPOSITIO_TERMINALIS);
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* s;

    s = (Mundus*)c->datum;
    si (s && s->semita[ZEPHYRUM])
    {
        (vacuum)setenv("PATH", s->semita, I);
    }
    si (s && s->volumen)
    {
        volumen_claudere(s->volumen);
    }
}

/* latera terminalia montata in omnibus tabulis */
interior i32
terminalia_numerare (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;
            i32  n;
            i32  i;
            s32  k;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (i32)vicus_numerus_tabularum(v); i++)
    {
        t = vicus_tabula(v, i);
        per (k = -I; t && k < (s32)xar_numerus(t->acervus); k++)
        {
            l = k < ZEPHYRUM ? &t->sinistrum
                : (VicusLatus*)xar_obtinere(t->acervus, (i32)k);
            si (   l->montata && !l->finita
                && chorda_aequalis_literis(l->genus, "terminale"))
            {
                n++;
            }
        }
    }
    redde n;
}

interior vacuum
dormire_ms (
    i32 ms)
{
    struct timespec pausa;

    pausa.tv_sec   = (time_t)(ms / M);
    pausa.tv_nsec  = (long)(ms % M) * M * M;
    (vacuum)nanosleep(&pausa, NIHIL);
}

/* lineae libri vocationum (0 si liber abest) */
interior i32
vocationes_numerare (
     Mundus* s,
    Piscina* p)
{
    chorda textus;
       i32 n;
       i32 i;

    si (!filum_existit(s->vocationes))
    {
        redde ZEPHYRUM;
    }
    textus  = filum_legere_totum(s->vocationes, p);
    n       = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '\n')
        {
            n++;
        }
    }
    redde n;
}

/* sine latere terminali: nullus tmux; latere aperto: vocatio una
 * (servitor ordinarius - praeconium vacuum) statim, non post V s */
interior vacuum
_probare_necessitatem (
    CredoContextus* c)
{
    Mundus* s;
     Vicus* v;
       i32  gradus;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    CREDO_NECESSE(tmux_fingere(s, credo_area(c), "0"));
    v = s->app.vicus;
    CREDO_NECESSE(terminalia_numerare(v) == ZEPHYRUM);

    CREDO_GRADUS("sine terminali: nullus tmux");
    per (gradus = ZEPHYRUM; gradus < X; gradus++)
    {
        (vacuum)vicus_pulsare(v);
        dormire_ms(XX);
    }
    CREDO_AEQUALIS_I32(vocationes_numerare(s, c->piscina), ZEPHYRUM);

    CREDO_GRADUS("terminale apertum: quaestio una statim");
    CREDO_NECESSE(vicus_acervo_aperire(v, "terminale", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE(terminalia_numerare(v) == I);
    per (gradus = ZEPHYRUM; gradus < C
                            && vocationes_numerare(s, c->piscina)
                               == ZEPHYRUM; gradus++)
    {
        (vacuum)vicus_pulsare(v);
        dormire_ms(XX);
    }
    CREDO_AEQUALIS_I32(vocationes_numerare(s, c->piscina), I);
    CREDO_VERUM(chorda_continet(filum_legere_totum(s->vocationes,
        c->piscina), chorda_ex_literis("-u list-clients", c->piscina)));
}

/* tmux tardus (CD ms): nullus pulsus eum exspectat; resultus
 * (cliens = tty lateris) argumentum "tmux - beta" ponit. Mora ficta
 * sub termino duro quaestionis (multiplexor: M ms, excessus occiditur)
 * cum margine: olim DCCC ms - sub onere processus occisus, effusio
 * vacua, probatio fluxa (quies T6b, 2026-10-26) */
interior vacuum
_probare_sine_exspectatione (
    CredoContextus* c)
{
                 Mundus* s;
                  Vicus* v;
             VicusLatus* l;
    TerminaleApplicatio* t;
                 chorda  tty;
              character  linea[CCLVI];
                    s64  ante;
                    s64  spatium;
                    s64  maximum;
                    i32  gradus;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    CREDO_NECESSE(tmux_fingere(s, credo_area(c), "0.4"));
    v = s->app.vicus;
    CREDO_NECESSE(vicus_acervo_aperire(v, "terminale", NIHIL));
    maximum  = ZEPHYRUM;
    ante     = fenestra_tempus_ms();
    (vacuum)vicus_pulsare(v);
    spatium  = fenestra_tempus_ms() - ante;
    maximum  = spatium > maximum ? spatium : maximum;
    l        = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "terminale"));
    t    = (TerminaleApplicatio*)l->montatio;
    tty  = terminale_tty(t);
    CREDO_NECESSE(tty.mensura > ZEPHYRUM
                  && tty.mensura < (i32)magnitudo(linea) - XVI);
    sprintf(linea, "%.*s\tbeta\n", (integer)tty.mensura,
        (constans character*)tty.datum);
    CREDO_NECESSE(filum_scribere_literis(s->clientes, linea));

    CREDO_GRADUS("pulsus tmux tardum non exspectat");
    per (gradus = ZEPHYRUM; gradus < CL
                            && l->argumentum.mensura == ZEPHYRUM;
         gradus++)
    {
        ante = fenestra_tempus_ms();
        (vacuum)vicus_pulsare(v);
        spatium = fenestra_tempus_ms() - ante;
        maximum = spatium > maximum ? spatium : maximum;
        dormire_ms(XX);
    }
    si (maximum >= CL)
    {
        imprimere("  pulsus maximus: %ld ms\n", (long)maximum);
    }
    CREDO_VERUM(maximum < CL);

    CREDO_GRADUS("resultus in pulsu posteriore applicatur");
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - beta");
    CREDO_AEQUALIS_I32(vocationes_numerare(s, c->piscina), I);
}

/* T3: lectiones semitarum omnium laterum montatorum (summa) */
interior i32
lectiones_summare (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;
            i32  n;
            i32  i;
            s32  k;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (i32)vicus_numerus_tabularum(v); i++)
    {
        t = vicus_tabula(v, i);
        per (k = -I; t && k < (s32)xar_numerus(t->acervus); k++)
        {
            l = k < ZEPHYRUM ? &t->sinistrum
                : (VicusLatus*)xar_obtinere(t->acervus, (i32)k);
            si (l->montata)
            {
                n += l->semitae_lectiones;
            }
        }
    }
    redde n;
}

/* T3: semitae laterum solum post scripturam in insula leguntur:
 * otium (C pulsus) nihil legit; scriptura una -> lectio una per latus
 * cum ramo, deinde iterum otium */
interior vacuum
_probare_semitas (
    CredoContextus* c)
{
         Mundus* s;
          Vicus* v;
     VicusLatus* sinistrum;
     VicusLatus* dextrum;
            i32  ante_s;
            i32  ante_d;
            i32  summa;
            i32  versio;
            i32  gradus;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v = s->app.vicus;
    /* concha incipit, prompta scribitur - status quiescit */
    per (gradus = ZEPHYRUM; gradus < L; gradus++)
    {
        (vacuum)vicus_pulsare(v);
        dormire_ms(XX);
    }
    sinistrum  = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    dextrum    = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    CREDO_NECESSE(sinistrum && sinistrum->montata
                  && sinistrum->facies.ramus);
    CREDO_NECESSE(dextrum && dextrum->montata
                  && chorda_aequalis_literis(dextrum->genus,
                      "terminale"));
    CREDO_NECESSE(sinistrum->semitae_lectae);

    CREDO_GRADUS("otium: C pulsus, nulla lectio");
    summa   = lectiones_summare(v);
    versio  = (i32)insula_versio(s->app.vicus->repo);
    per (gradus = ZEPHYRUM; gradus < C; gradus++)
    {
        (vacuum)vicus_pulsare(v);
        dormire_ms(II);
    }
    si (insula_versio(s->app.vicus->repo) != versio)
    {
        imprimere("  insula scripta in otio: versio %u -> %u\n",
            (insignatus integer)versio,
            (insignatus integer)insula_versio(s->app.vicus->repo));
    }
    CREDO_AEQUALIS_I32(lectiones_summare(v), summa);

    CREDO_GRADUS("scriptura una: lectio una per latus");
    ante_s = sinistrum->semitae_lectiones;
    ante_d = dextrum->semitae_lectiones;
    CREDO_NECESSE(vicus_radicem_scribere(v, "admonitiones",
        "admonitio_nova", "probatio"));
    per (gradus = ZEPHYRUM; gradus < X; gradus++)
    {
        (vacuum)vicus_pulsare(v);
    }
    CREDO_AEQUALIS_I32(sinistrum->semitae_lectiones, ante_s + I);
    /* terminale sine ramo semel lectum, deinde numquam */
    si (dextrum->facies.ramus)
    {
        CREDO_AEQUALIS_I32(dextrum->semitae_lectiones, ante_d + I);
    }
    alioquin
    {
        CREDO_AEQUALIS_I32(dextrum->semitae_lectiones, ante_d);
    }
}

/* ===== T6a: moras et fossas declarant latera, systemata, horologium
 * - genera ficta in vico nudo ('quietus' moram et fossam declarat,
 * 'ignotus' pulsum solum habet: XVI dum pulsatur) ===== */

nomen structura {
    Mora mora;
     s32 fossa;
} Quietus;

interior b32
quietum_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)p;
    (vacuum)in;
    (vacuum)vol;
    (vacuum)r;
    (vacuum)id;
    (vacuum)argumentum;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)ctx;
    ((Quietus*)sedes)->mora   = -I;
    ((Quietus*)sedes)->fossa  = -I;
    redde VERUM;
}

interior VicusPulsus
quietum_pulsare (
    vacuum* ctx)
{
    VicusPulsus p;

    (vacuum)ctx;
    p.mutatum = FALSUM;
    p.finitus = FALSUM;
    redde p;
}

interior Mora
quieti_mora (
    vacuum* ctx)
{
    redde ((Quietus*)ctx)->mora;
}

interior s32
quieti_fossa (
    vacuum* ctx,
       i32* eventus)
{
    *eventus = REACTOR_LEGERE;
    redde ((Quietus*)ctx)->fossa;
}

interior vacuum
quietum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    f->pulsare      = quietum_pulsare;
    f->pulsare_ctx  = montatio;
    f->mora         = quieti_mora;
    f->fossa        = quieti_fossa;
}

/* pulsus sine mora declarata */
interior vacuum
ignotum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    f->pulsare      = quietum_pulsare;
    f->pulsare_ctx  = montatio;
}

interior b32
systema_inane (
     Vicus* v,
    vacuum* ctx)
{
    (vacuum)v;
    (vacuum)ctx;
    redde FALSUM;
}

interior Mora
systematis_mora (
    vacuum* ctx)
{
    redde *(Mora*)ctx;
}

interior vacuum
horologium_fictum (
         vacuum* ctx,
    VicusTempus* tempus)
{
    *tempus = *(VicusTempus*)ctx;
}

/* vicus nudus cum generibus fictis, dispositione data, pulsu uno */
interior Vicus*
vicum_nudum (
         CredoContextus* c,
                Volumen* vol,
     constans character* dispositio)
{
    Vicus* v;

    v = vicus_creare(c->piscina, internamentum_creare(c->piscina), vol,
        CDLXXX, CDLXXX);
    si (   !v
        || !vicus_genus_addere(v, "quietus", magnitudo(Quietus),
               quietum_montare, quietum_describere, NIHIL)
        || !vicus_genus_addere(v, "ignotus", magnitudo(Quietus),
               quietum_montare, ignotum_describere, NIHIL)
        || !vicus_aperire(v, dispositio))
    {
        redde NIHIL;
    }
    (vacuum)vicus_pulsare(v);
    redde v;
}

#define DISPOSITIO_QUIETA                                           \
    "<tabulae activa=\"A\"><tabula id=\"A\">"                         \
    "<latus genus=\"quietus\"/>"                                      \
    "<acervus><latus genus=\"quietus\"/></acervus></tabula>"          \
    "<tabula id=\"B\"><latus genus=\"ignotus\"/></tabula></tabulae>"
#define DISPOSITIO_IGNOTA                                           \
    "<tabulae activa=\"A\"><tabula id=\"A\">"                         \
    "<latus genus=\"ignotus\"/></tabula></tabulae>"

interior vacuum
_probare_moram (
    CredoContextus* c)
{
        Volumen* vol;
          Vicus* v;
     VicusLatus* l;
        Quietus* q;
    VicusTempus  tempus;
           Mora  mora_systematis;

    vol = volumen_temporarium(c->piscina, "probatio_vicus_quies_mora");
    CREDO_NECESSE(vol);
    v = vicum_nudum(c, vol, DISPOSITIO_QUIETA);
    CREDO_NECESSE(v);
    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    CREDO_NECESSE(l && l->montata);
    q = (Quietus*)l->montatio;

    CREDO_GRADUS("nihil debitum: -1 (ignotus occultus neglegitur)");
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), -I);

    CREDO_GRADUS("latus visibile: mora sua");
    q->mora = III * M;
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), III * M);
    q->mora = -I;

    CREDO_GRADUS("horologium: usque ad minutum proximum");
    memset(&tempus, ZEPHYRUM, magnitudo(tempus));
    tempus.annus     = MMXXVI;
    tempus.mensis    = X;
    tempus.dies      = XXVI;
    tempus.hora      = X;
    tempus.minutum   = XXX;
    tempus.secundum  = XLV;
    vicus_horologium_ponere(v, horologium_fictum, &tempus);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), XV * M);
    vicus_horologium_ponere(v, NIHIL, NIHIL);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), -I);

    CREDO_GRADUS("petitio pendens: 0 (pulsus proximus statim)");
    CREDO_VERUM(vicus_acervo_aperire(v, "quietus", NIHIL));
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), -I);

    CREDO_GRADUS("systema cum mora: sua; sine mora: XVI");
    mora_systematis = D;
    CREDO_VERUM(vicus_systema_cum_mora_addere(v, "probatio_mora",
        systema_inane, systematis_mora, &mora_systematis));
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), D);
    CREDO_VERUM(vicus_systema_addere(v, "probatio_ignota",
        systema_inane, NIHIL));
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), XVI);

    CREDO_GRADUS("latus visibile sine mora: XVI");
    volumen_claudere(vol);
    vol = volumen_temporarium(c->piscina, "probatio_vicus_quies_ign");
    CREDO_NECESSE(vol);
    v = vicum_nudum(c, vol, DISPOSITIO_IGNOTA);
    CREDO_NECESSE(v);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), XVI);
    volumen_claudere(vol);
}

/* fossae in reactore numeratae (fd datus: eventus; -1: numerus) */
nomen structura {
    integer fd;
        i32 numerus;
        i32 eventus;
} FossaeNumeratae;

interior vacuum
fossam_numerare (
     vacuum* ctx,
    integer  fd,
        i32  eventus)
{
    FossaeNumeratae* n = (FossaeNumeratae*)ctx;

    n->numerus++;
    si (fd == n->fd)
    {
        n->eventus = eventus;
    }
}

interior FossaeNumeratae
fossas_numerare (
    Reactor* reactor,
    integer  fd)
{
    FossaeNumeratae n;

    n.fd       = fd;
    n.numerus  = ZEPHYRUM;
    n.eventus  = ZEPHYRUM;
    reactor_fossas_visitare(reactor, fossam_numerare, &n);
    redde n;
}

interior vacuum
_probare_fossas (
    CredoContextus* c)
{
            Volumen* vol;
              Vicus* v;
         VicusLatus* l;
            Quietus* q;
            Reactor* reactor;
            integer  p[II];
    FossaeNumeratae  n;
                i32  versio;
                i32  gradus;

    vol = volumen_temporarium(c->piscina, "probatio_vicus_quies_fos");
    CREDO_NECESSE(vol);
    v = vicum_nudum(c, vol, DISPOSITIO_QUIETA);
    CREDO_NECESSE(v);
    reactor = reactor_creare(c->piscina);
    CREDO_NECESSE(reactor && pipe(p) == ZEPHYRUM);
    vicus_reactor_ponere(v, reactor);
    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    CREDO_NECESSE(l && l->montata);
    q = (Quietus*)l->montatio;

    CREDO_GRADUS("fossa lateris in reactore inscribitur (semel)");
    q->fossa = p[0];
    (vacuum)vicus_pulsare(v);
    n = fossas_numerare(reactor, p[0]);
    CREDO_AEQUALIS_I32(n.numerus, I);
    CREDO_AEQUALIS_I32(n.eventus, REACTOR_LEGERE);
    versio = reactor_versio_fossarum(reactor);
    per (gradus = ZEPHYRUM; gradus < V; gradus++)
    {
        (vacuum)vicus_pulsare(v);
    }
    CREDO_AEQUALIS_I32(reactor_versio_fossarum(reactor), versio);
    CREDO_AEQUALIS_I32(fossas_numerare(reactor, p[0]).numerus, I);

    CREDO_GRADUS("fossa evanida: e reactore tollitur");
    q->fossa = -I;
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(fossas_numerare(reactor, p[0]).numerus,
        ZEPHYRUM);

    reactor_destruere(reactor);
    close(p[0]);
    close(p[I]);
    volumen_claudere(vol);
}

/* scriba: otiosa -1, opus aut iussum dilatum -> XVI */
interior vacuum
_probare_scribam (
    CredoContextus* c)
{
            Mundus* s;
        VicusLatus* l;
    ScribaMontatio* m;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    (vacuum)vicus_pulsare(s->app.vicus);
    l = vicus_latus(vicus_activa(s->app.vicus), VICUS_SINISTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "scriba"));
    CREDO_NECESSE(l->facies.mora != NIHIL);
    m = (ScribaMontatio*)l->facies.pulsare_ctx;

    CREDO_GRADUS("scriba otiosa: -1");
    CREDO_FALSUM(scriba_occupata(m));
    CREDO_AEQUALIS_S64(l->facies.mora(l->facies.pulsare_ctx), -I);

    CREDO_GRADUS("iussum dilatum: XVI");
    m->actiones_ctx.dilatum = I;
    CREDO_VERUM(scriba_occupata(m));
    CREDO_AEQUALIS_S64(l->facies.mora(l->facies.pulsare_ctx), XVI);
    m->actiones_ctx.dilatum = ZEPHYRUM;
}

/* ===== T2b: tmux solum cum programma in fronte mutatur (aut, latere
 * visibili cum tmux in fronte, per V s) ===== */

/* pulsare usque ad 'ms' aut donec vocationes >= 'meta' (meta 0: totum
 * tempus pulsare); vocationes redduntur */
interior i32
pulsare_usque (
      Mundus* s,
     Piscina* p,
         i32  ms,
         i32  meta)
{
    s64 finis;
    i32 n;

    finis  = fenestra_tempus_ms() + (s64)ms;
    n      = vocationes_numerare(s, p);
    dum (fenestra_tempus_ms() < finis && (meta == ZEPHYRUM || n < meta))
    {
        (vacuum)vicus_pulsare(s->app.vicus);
        dormire_ms(XX);
        n = vocationes_numerare(s, p);
    }
    redde n;
}

interior vacuum
_probare_frontem (
    CredoContextus* c)
{
                 Mundus* s;
             VicusLatus* l;
    TerminaleApplicatio* t;
                 chorda  tty;
              character  linea[CCLVI];
                    i32  n;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    CREDO_NECESSE(tmux_fingere(s, credo_area(c), "0"));
    (vacuum)vicus_pulsare(s->app.vicus);
    l = vicus_latus(vicus_activa(s->app.vicus), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "terminale"));
    t = (TerminaleApplicatio*)l->montatio;

    CREDO_GRADUS("latus apertum: quaestio (concha in fronte)");
    n = pulsare_usque(s, c->piscina, II * M, I);
    CREDO_VERUM(n >= I);
    /* concha incipit; confirmatio post M ms - quiescat */
    n = pulsare_usque(s, c->piscina, II * M + D, ZEPHYRUM);

    CREDO_GRADUS("concha sola in fronte: nulla quaestio in V s");
    CREDO_AEQUALIS_I32(pulsare_usque(s, c->piscina, V * M + D,
        ZEPHYRUM), n);

    CREDO_GRADUS("programma incipit: quaestio; finit: quaestio");
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(t->hospes,
        (constans i8*)"sleep 1\n", VIII), VIII);
    CREDO_AEQUALIS_I32(pulsare_usque(s, c->piscina, II * M, n + I),
        n + I);
    /* finis programmatis et confirmationes */
    CREDO_VERUM(pulsare_usque(s, c->piscina, III * M + D, ZEPHYRUM)
                >= n + II);

    CREDO_GRADUS("tmux in fronte visibilis: quaestio per V s");
    tty = terminale_tty(t);
    CREDO_NECESSE(tty.mensura > ZEPHYRUM
                  && tty.mensura < (i32)magnitudo(linea) - XVI);
    sprintf(linea, "%.*s\tbeta\n", (integer)tty.mensura,
        (constans character*)tty.datum);
    CREDO_NECESSE(filum_scribere_literis(s->clientes, linea));
    CREDO_AEQUALIS_I32(aemulator_hospes_scribere(t->hospes,
        (constans i8*)"sleep 1\n", VIII), VIII);
    /* frontes mutatae, confirmationes - omnia quiescant */
    n = pulsare_usque(s, c->piscina, III * M + D, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - beta");
    /* nulla frons mutata, nulla confirmatio: sola regula V s */
    CREDO_VERUM(pulsare_usque(s, c->piscina, VI * M + D, ZEPHYRUM)
                >= n + I);
}

/* ===== T6c: liber praeconii per speculam - scriptura vicum
 * expergefacit, pulsus legit, fossa iterum quieta ===== */

nomen structura {
    structura pollfd  fossae[XVI];
                 i32 numerus;
} FossaePollendae;

interior vacuum
fossam_pollendam_addere (
     vacuum* ctx,
    integer  fd,
        i32  eventus)
{
    FossaePollendae* fp = (FossaePollendae*)ctx;

    (vacuum)eventus;
    si (fp->numerus < XVI)
    {
        fp->fossae[fp->numerus].fd       = fd;
        fp->fossae[fp->numerus].events   = POLLIN;
        fp->fossae[fp->numerus].revents  = ZEPHYRUM;
        fp->numerus++;
    }
}

/* fossa ulla reactoris legenda (mora ms; 0 statim) */
interior b32
reactor_legendus (
    Reactor* reactor,
        s32  mora,
        i32* numerus)
{
    FossaePollendae fp;

    fp.numerus = ZEPHYRUM;
    reactor_fossas_visitare(reactor, fossam_pollendam_addere, &fp);
    *numerus = fp.numerus;
    redde fp.numerus > ZEPHYRUM
        && poll(fp.fossae, (nfds_t)fp.numerus, (integer)mora)
            > ZEPHYRUM;
}

interior vacuum
_probare_praeconium (
    CredoContextus* c)
{
     Mundus* s;
      Vicus* v;
    Reactor* reactor;
     chorda* a;
        i32  numerus;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v        = s->app.vicus;
    reactor  = reactor_creare(c->piscina);
    CREDO_NECESSE(reactor);
    vicus_reactor_ponere(v, reactor);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);

    CREDO_GRADUS("specula libri in reactore, quieta");
    CREDO_FALSUM(reactor_legendus(reactor, ZEPHYRUM, &numerus));
    CREDO_VERUM(numerus >= I);

    CREDO_GRADUS("linea scripta: legenda; pulsus legit; quieta iterum");
    CREDO_NECESSE(filum_appendere_firmiter(s->liber, chorda_ex_literis(
        "{\"tempus\":400,\"sessio\":\"s-c\","
        "\"eventus\":\"Notification\","
        "\"panellus\":\"%1\",\"servitor\":\"\",\"via\":\"/w/gamma\","
        "\"transcriptum\":\"\",\"genus\":\"permission_prompt\","
        "\"nuntius\":\"probatio\"}\n", c->piscina)));
    CREDO_VERUM(reactor_legendus(reactor, CC, &numerus));
    (vacuum)vicus_pulsare(v);
    a = insula_attributum(v->repo, "admonitio_nova");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "colloquia:s-c:400");
    }
    CREDO_FALSUM(reactor_legendus(reactor, ZEPHYRUM, &numerus));

    vicus_reactor_ponere(v, NIHIL);
    reactor_destruere(reactor);
}

/* ===== T6d: omnia declarata - vicus otiosus nihil debet; porta soni
 * ficta moram suam dat ===== */

interior b32
sonus_pulsare_fictus (
     vacuum* ctx,
     chorda  genus)
{
    (vacuum)ctx;
    (vacuum)genus;
    redde FALSUM;
}

interior vacuum
sonus_inanis (
    vacuum* ctx)
{
    (vacuum)ctx;
}

interior b32
sonus_canere_fictus (
     vacuum* ctx,
     chorda  textus,
        b32  intactus)
{
    (vacuum)ctx;
    (vacuum)textus;
    (vacuum)intactus;
    redde FALSUM;
}

interior i64
sonus_vocem_incipere_fictus (
                 vacuum* ctx,
     constans character* tibia,
                    b32  vibratus,
                    f32  nota,
                    f32  lucrum)
{
    (vacuum)ctx;
    (vacuum)tibia;
    (vacuum)vibratus;
    (vacuum)nota;
    (vacuum)lucrum;
    redde ZEPHYRUM;
}

interior vacuum
sonus_vocem_ponere_fictus (
    vacuum* ctx,
       i64  vox,
       f32  nota,
       f32  lucrum,
       i32  ms)
{
    (vacuum)ctx;
    (vacuum)vox;
    (vacuum)nota;
    (vacuum)lucrum;
    (vacuum)ms;
}

interior vacuum
sonus_vocem_dimittere_fictus (
    vacuum* ctx,
       i64  vox)
{
    (vacuum)ctx;
    (vacuum)vox;
}

interior s64
sonus_mora_ficta (
    vacuum* ctx)
{
    redde *(s64*)ctx;
}

interior vacuum
_probare_otium (
    CredoContextus* c)
{
            Mundus* s;
             Vicus* v;
    VicusPortaSoni  porta;
               s64  mora_portae;
               i32  gradus;

    s = (Mundus*)c->datum;
    CREDO_NECESSE(s && s->paratus);
    v = s->app.vicus;
    per (gradus = ZEPHYRUM; gradus < V; gradus++)
    {
        (vacuum)vicus_pulsare(v);
    }

    CREDO_GRADUS("omnia declarata, nihil debitum: -1");
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), -I);

    CREDO_GRADUS("porta soni: mora sua; sine mora XVI");
    memset(&porta, ZEPHYRUM, magnitudo(porta));
    porta.pulsare          = sonus_pulsare_fictus;
    porta.agere            = sonus_inanis;
    porta.tacere           = sonus_inanis;
    porta.canere           = sonus_canere_fictus;
    porta.vocem_incipere   = sonus_vocem_incipere_fictus;
    porta.vocem_ponere     = sonus_vocem_ponere_fictus;
    porta.vocem_dimittere  = sonus_vocem_dimittere_fictus;
    porta.mora             = sonus_mora_ficta;
    porta.ctx              = &mora_portae;
    mora_portae            = DCC;
    vicus_applicatio_soni_portam_ponere(&s->app, &porta);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), DCC);
    mora_portae = -I;
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), -I);
    porta.mora = NIHIL;
    vicus_applicatio_soni_portam_ponere(&s->app, &porta);
    CREDO_AEQUALIS_S64(vicus_mora_proxima(v), XVI);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "necessitas", _probare_necessitatem, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "sine_exspectatione", _probare_sine_exspectatione, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "semitae", _probare_semitas, mundum_terminalis_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "mora", _probare_moram, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fossae", _probare_fossas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scriba", _probare_scribam, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "frons", _probare_frontem, mundum_terminalis_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "praeconium", _probare_praeconium, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "otium", _probare_otium, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_quies", SECTIONES);
}
