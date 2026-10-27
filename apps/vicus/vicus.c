/* vicus.c - vicus: hospes applicationum (scriba, pictor, terminale)
 * pro testa (testa.h: principale a testa)
 *
 * Compositio communis (vicus_applicatio) + forma pro testa;
 * fenestra ad SCALA II (pixelum nostrum = II puncta, ut terminale)
 * in plena visione (vicus-latera S2c; -fumus: magnitudo fixa);
 * tabulae vivae pulsantur (vicus-latera S1c);
 * gemellus vicus_terminalis.c (insula-rami-plan T4). Cmd+1..9, Cmd+0
 * = tabula (Ctrl-A ad latus focatum - tmux); ictus in tabulam. -fumus:
 * volumen temporarium, XXX quadra, exitus; -volumen <via>
 * (ordinarie vicus.volumen); -quaestio adiutor: $question per
 * bin/quaestio (via A) loco portae in processu (via B, http-vocatio).
 * -quies: singulis X secundis in stderr pulsus per secundum et pulsus
 * mutantes (quies T1: in ansa pulsus = excitatio).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "testa.h"
#include "vicus_applicatio.h"
#include "praeconium.h"
#include "energia.h"
#include "vicus_quaestio_porta.h"
#include "vicus_quaestio_interna.h"
#include "reactor.h"
#include "vicus_sonus_porta.h"
#include "fenestra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* applicationes CDLXXX x CDLXXX ut solae, linea tabularum supra */
#define VICUS_LATITUDO   CDLXXX
#define VICUS_ALTITUDO   (CDLXXX + VICUS_ALTITUDO_TABULARUM)
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA            II

/* tabulae vivae (terminale) in omni quadro pulsantur, etiam in fundo
 * (vicus-latera S1c). Reactor: hospes eum SOLUS possidet et in omni
 * pulsu poscit (http-vocatio R1); nihil inscriptum = nihil agit. */
nomen structura {
       Vicus* vicus;
     Reactor* reactor;
         b32  quies;     /* -quies: mensurae in stderr */
         s64  initium;   /* ms fenestrae, initium fenestrae X s */
         s64  pulsus;
         s64  mutata;
} PulsusVici;

/* quies T1: X secundorum fenestra - pulsus et mutata per secundum */
#define QUIES_FENESTRA_MS  (X * M)

interior vacuum
_quietem_numerare (
    PulsusVici* pv,
           b32  mutatum)
{
    s64 nunc;
    s64 spatium;

    nunc = fenestra_tempus_ms();
    si (pv->initium == ZEPHYRUM)
    {
        pv->initium = nunc;
    }
    pv->pulsus++;
    si (mutatum)
    {
        pv->mutata++;
    }
    spatium = nunc - pv->initium;
    si (spatium >= QUIES_FENESTRA_MS)
    {
        fprintf(stderr, "vicus quies: %ld ms: pulsus %ld (%ld/s),"
                " mutata %ld (%ld/s)\n", (long)spatium,
                (long)pv->pulsus,
                (long)(pv->pulsus * M / spatium), (long)pv->mutata,
                (long)(pv->mutata * M / spatium));
        pv->initium  = nunc;
        pv->pulsus   = ZEPHYRUM;
        pv->mutata   = ZEPHYRUM;
    }
}

interior b32
vicum_pulsare (
    vacuum* ctx)
{
    PulsusVici* pv = (PulsusVici*)ctx;
           b32  mutatum;

    (vacuum)reactor_poll(pv->reactor, ZEPHYRUM);
    mutatum = vicus_pulsare(pv->vicus);
    si (pv->quies)
    {
        _quietem_numerare(pv, mutatum);
    }
    redde mutatum;
}

/* vicus quies T6: mora vici (horologium, systemata, latera);
 * reactor et dispensator a ludo adduntur */
interior Mora
vicum_morari (
    vacuum* ctx)
{
    redde vicus_mora_proxima(((PulsusVici*)ctx)->vicus);
}

/* S5: porta pilae - energia_status systematis */
interior b32
pilam_systematis (
    vacuum* ctx,
       s32* centesimae,
       b32* oneratur)
{
    EnergiaStatus s;

    (vacuum)ctx;
    si (!energia_status(&s))
    {
        redde FALSUM;
    }
    *centesimae  = s.centesimae;
    *oneratur    = s.oneratur;
    redde VERUM;
}

/* R3: status hospitis totus in piscina testae - monstratores eius
 * (pulsus, portae) in formam et vicum exeunt */
nomen structura {
          VicusApplicatio  app;
    VicusPortaQuaestionis  porta_quaestionis;
           VicusPortaSoni  porta_soni;
               PulsusVici  pulsus;
                  Volumen* vol;
} HospesVici;

interior vacuum
_vicum_finire (
    vacuum* ctx)
{
    volumen_claudere(((HospesVici*)ctx)->vol);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
             HospesVici* h;
    InternamentumChorda* intern;
                Reactor* reactor;
                    b32  fumus;
                    b32  per_adiutorem;
                    b32  plena;
                    s32  k;
                    i32  latitudo;
                    i32  altitudo;

    h = (HospesVici*)piscina_allocare(a->piscina,
        magnitudo(HospesVici));
    si (!h)
    {
        redde FALSUM;
    }
    memset(h, ZEPHYRUM, magnitudo(HospesVici));
    reactor  = reactor_creare(a->piscina);
    intern   = internamentum_creare(a->piscina);
    thema_initiare();

    h->vol = vicus_volumen_aperire(a->piscina, a->argc, a->argv,
        &fumus);
    si (!h->vol)
    {
        fprintf(stderr, "vicus: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    /* S2c: vicus in PLENA VISIONE aperitur (Franus 2026-10-08: sic
     * laborat) - compositio et documenta magnitudine schirmi totius;
     * fenestram in area utili testa ponit (testa.c). fumus: magnitudo
     * fixa, sine plena visione (imagines certae; testa schirmum sub
     * fumo non quaerit). Olim 'plena' sine initio sub fumo legebatur
     * (parcum ...J49JH): nunc semper definitum. */
    plena = !fumus && a->schirmi_latitudo > ZEPHYRUM
                   && a->schirmi_altitudo > ZEPHYRUM;
    latitudo = VICUS_LATITUDO;
    altitudo = VICUS_ALTITUDO;
    si (plena)
    {
        latitudo = a->schirmi_latitudo / SCALA;
        altitudo = a->schirmi_altitudo / SCALA;
    }
    si (!vicus_applicatio_aedificare(&h->app, a->piscina, intern,
        h->vol,
            latitudo, altitudo))
    {
        redde FALSUM;
    }
    /* S4: horologium locale in linea tabularum */
    vicus_horologium_ponere(h->app.vicus, vicus_horologium_locale,
        NIHIL);
    /* S5: pila (IOKit; machina mensae: nulla) */
    vicus_pilam_ponere(h->app.vicus, pilam_systematis, NIHIL);
    /* T4a: admonitiones ex libro praeconii (colloquia Claude Code) */
    (vacuum)vicus_applicatio_admonitiones_aperire(&h->app,
        praeconium_via(a->piscina));
    /* sonus S4: admonitiones sonantes; -fumus tacet */
    si (   !fumus && vicus_sonus_porta(&h->porta_soni, a->piscina,
            (i64)time(NIHIL)))
    {
        vicus_applicatio_soni_portam_ponere(&h->app, &h->porta_soni);
    }
    /* L5: Cmd+V in pictore: imago e clipboard (servitium testae) */
    vicus_applicatio_imaginis_portam_ponere(&h->app, a->imago_glutinata,
        a->imago_ctx);
    /* yy / y / dd in scriba: registrum in clipboard (servitium testae) */
    vicus_applicatio_textus_portam_ponere(&h->app, a->textum_exportare,
        a->textus_ctx);
    /* $question: in processu (via B) nisi -quaestio adiutor (via A) */
    per_adiutorem = FALSUM;
    per (k = I; k + I < a->argc; k++)
    {
        si (   strcmp(a->argv[k], "-quaestio")   == ZEPHYRUM
            && strcmp(a->argv[k + I], "adiutor") == ZEPHYRUM)
        {
            per_adiutorem = VERUM;
        }
    }
    h->porta_quaestionis = per_adiutorem
        ? vicus_quaestio_porta(vicus_quaestio_adiutoris_via(
              a->argc > ZEPHYRUM ? a->argv[ZEPHYRUM] : NIHIL,
              a->piscina),
              ZEPHYRUM, a->piscina)
        : vicus_quaestio_interna(reactor, NIHIL, a->piscina);
    vicus_applicatio_quaestionis_portam_ponere(&h->app,
        &h->porta_quaestionis);

    h->pulsus.vicus    = h->app.vicus;
    h->pulsus.reactor  = reactor;
    per (k = I; k < a->argc; k++)
    {
        si (strcmp(a->argv[k], "-quies") == ZEPHYRUM)
        {
            h->pulsus.quies = VERUM;
        }
    }
    forma->titulus       = "vicus";
    forma->latitudo      = latitudo;
    forma->altitudo      = altitudo;
    forma->scala         = SCALA;
    forma->plena_visio   = plena;
    forma->d             = h->app.d;
    forma->figurae       = vicus_figurae(h->app.vicus);
    forma->thema         = ZEPHYRUM;
    forma->fons          = vicus_imago_fons;
    forma->fons_ctx      = h->app.vicus;
    forma->pulsator      = vicum_pulsare;
    forma->pulsator_ctx  = &h->pulsus;
    /* vicus quies T6: ansa usque ad debitum proximum; fossae laterum
     * (pty) in reactore */
    vicus_reactor_ponere(h->app.vicus, reactor);
    forma->mora_proxima  = vicum_morari;
    forma->mora_ctx      = &h->pulsus;
    forma->reactor       = reactor;
    forma->finis         = _vicum_finire;
    forma->finis_ctx     = h;
    redde VERUM;
}

/* introitus: testa (testa.h) - macOS fenestra et ansa; Android
 * ingressus suus (ANativeActivity_onCreate) */
s32
principale (
      integer   argc,
    character** argv)
{
    redde testa_principale(argc, argv);
}
