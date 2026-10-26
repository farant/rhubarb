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
#include "fenestra_tempus.h"
#include "thema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* tmux tardus (DCCC ms): nullus pulsus eum exspectat; resultus
 * (cliens = tty lateris) argumentum "tmux - beta" ponit */
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
    CREDO_NECESSE(tmux_fingere(s, credo_area(c), "0.8"));
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "necessitas", _probare_necessitatem, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "sine_exspectatione", _probare_sine_exspectatione, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "semitae", _probare_semitas, mundum_terminalis_parare,
      mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_quies", SECTIONES);
}
