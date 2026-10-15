/* vicus_quaestio_interna.c - porta $question in processu (via B; vide
 * caput, http-vocatio-spec II.6) */
#include "postulata_posix.h"
#include "vicus_quaestio_interna.h"
#include "vates.h"
#include "quaestio.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
                         Reactor* reactor;
    VicusQuaestioInternaOptiones  optiones;   /* chordae copiatae */
} PortaInterna;

nomen structura {
               Piscina* piscina;            /* vici: manubrium */
               Piscina* piscina_vocationis; /* clavis intus: vacatur et
                                             * destruitur (R2) */
          VatesVocatio* vocatio;
     ChordaAedificator* crudum;
                   b32  mutatum;
                   b32  vocatio_finita;     /* finis vatis visus */
                   b32  finitus;            /* vico nuntiatum */
                chorda  error;
                   s64  input;
                   s64  output;
} ManubriumInternum;

interior constans character*
_copia (
    constans character* s,
               Piscina* p)
{
    character* c;

    si (!s)
    {
        redde NIHIL;
    }
    c = (character*)piscina_allocare(p, (memoriae_index)strlen(s) + I);
    strcpy(c, s);
    redde c;
}

/* piscina vocationis: VACARE (omnia blocka zephyrantur - clavis in
 * Vates, capitibus, octetis) deinde destruere. Numquam intra finem
 * vatis (codex post reditum currit) - solum ex pulsare/abrumpere. */
interior vacuum
_vocationem_dimittere (
    ManubriumInternum* h)
{
    si (h->piscina_vocationis)
    {
        piscina_vacare(h->piscina_vocationis);
        piscina_destruere(h->piscina_vocationis);
        h->piscina_vocationis = NIHIL;
    }
    h->vocatio = NIHIL;
}

/* clavis ex via (NIHIL: $HOME/.rhubarb/anthropic.clavis), spatia
 * utrimque tolluntur (ut apps/quaestio/quaestio.c); capsa localis
 * zephyratur; chorda vacua + *error si nequit */
interior chorda
_clavem_legere (
    constans character* via,
               Piscina* p_clavis,
               Piscina* p_erroris,
                chorda* error)
{
    constans character* domus;
             character  semita[DXII];
             character  capsa[DXII];
                  FILE* f;
                size_t  n;
                size_t  a;
                chorda  clavis;

    clavis.datum    = NIHIL;
    clavis.mensura  = ZEPHYRUM;
    si (!via)
    {
        domus = getenv("HOME");
        si (!domus || strlen(domus) > CD)
        {
            *error = chorda_ex_literis("HOME absent", p_erroris);
            redde clavis;
        }
        sprintf(semita, "%s/.rhubarb/anthropic.clavis", domus);
        via = semita;
    }
    /* clavis SECRETA tempore cursus, non ingressus aedificationis: per
     * librum lectionum fabrica contentum eius in clavem transitus
     * sigillaret (vestigium secreti in build/, verdictum mutata clave
     * irritum) */
    /* <tolera codex="CANALIS_PRAETERITUS" (>clavis secreta cursus */
    f = fopen(via, "rb");
    si (!f)
    {
        *error = chorda_ex_literis(
            "no API key (~/.rhubarb/anthropic.clavis)", p_erroris);
        redde clavis;
    }
    n = fread(capsa, I, magnitudo(capsa) - I, f);
    (vacuum)fclose(f);
    a = ZEPHYRUM;
    dum (   a < n && (capsa[a] == ' ' || capsa[a] == '\n'
        || capsa[a] == '\r' || capsa[a] == '\t'))
    {
        a++;
    }
    dum (   n > a && (capsa[n - I] == ' ' || capsa[n - I] == '\n'
        || capsa[n - I] == '\r' || capsa[n - I] == '\t'))
    {
        n--;
    }
    si (n > a)
    {
        clavis.datum = (i8*)piscina_allocare(p_clavis,
            (memoriae_index)(n - a));
        memcpy(clavis.datum, capsa + a, n - a);
        clavis.mensura = (i32)(n - a);
    }
    alioquin
    {
        *error = chorda_ex_literis("API key file empty", p_erroris);
    }
    memset(capsa, ZEPHYRUM, magnitudo(capsa));
    redde clavis;
}

interior vacuum
_delta (
    vacuum* datum,
    chorda  delta,
       b32  cogitatio)
{
    ManubriumInternum* h = (ManubriumInternum*)datum;

    si (!cogitatio)
    {
        chorda_aedificator_appendere_chorda(h->crudum, delta);
        h->mutatum = VERUM;
    }
}

/* finis vatis: copiae in piscinam VICI (vocationis mox destruitur) */
interior vacuum
_finis (
          vacuum* datum,
    VatesVocatio* vocatio)
{
    ManubriumInternum* h = (ManubriumInternum*)datum;
    QuaestioResponsum  q = quaestio_responsum_ex_vate(
        vates_vocatio_responsum(vocatio), h->piscina);

    h->vocatio_finita = VERUM;
    si (q.successus)
    {
        h->input   = q.usus.input;
        h->output  = q.usus.output;
    }
    alioquin
    {
        h->error = q.error;
    }
}

interior vacuum*
_incipere (
     vacuum* ctx,
     chorda  quaestio,
        s32  lineae,
        s32  latitudo,
    Piscina* piscina,
     chorda* error)
{
          PortaInterna* pi = (PortaInterna*)ctx;
     ManubriumInternum* h;
                chorda  clavis;
                 Vates* vates;
         VatesOptiones  optiones;
                chorda  vacua;

    h = (ManubriumInternum*)piscina_allocare(piscina,
        magnitudo(ManubriumInternum));
    memset(h, ZEPHYRUM, magnitudo(ManubriumInternum));
    h->piscina  = piscina;
    h->crudum   = chorda_aedificator_creare(piscina, D);
    h->piscina_vocationis  =
        piscina_generare_dynamicum("quaestio-interna",
        LXIV * M);
    si (!h->piscina_vocationis)
    {
        *error = chorda_ex_literis("out of memory", piscina);
        redde NIHIL;
    }
    clavis = _clavem_legere(pi->optiones.via_clavis,
        h->piscina_vocationis, piscina, error);
    si (clavis.mensura == ZEPHYRUM)
    {
        _vocationem_dimittere(h);
        redde NIHIL;
    }
    optiones                 = vates_optiones_ordinariae();
    optiones.url             = pi->optiones.url;
    optiones.sine_herbario   = VERUM;    /* R10 */
    optiones.conatus_maximi  = pi->optiones.conatus_maximi;
    optiones.mora_iterandi_ms =
        (MoraAngusta)pi->optiones.mora_iterandi_ms;
    vates = vates_aperire(h->piscina_vocationis, chorda_ex_literis(
        "anthropic", h->piscina_vocationis), clavis, &optiones);
    /* vates copiam suam unam tenet; nostra zephyratur (R2) */
    memset(clavis.datum, ZEPHYRUM, (size_t)clavis.mensura);
    si (!vates)
    {
        _vocationem_dimittere(h);
        *error = chorda_ex_literis("vates anthropic not opened",
            piscina);
        redde NIHIL;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    h->vocatio = vates_vocare_rivum(vates, quaestio_petitionem_parare(
        vacua, quaestio, lineae, latitudo, h->piscina_vocationis),
        chorda_ex_literis("quaestio", h->piscina_vocationis),
        pi->reactor,
        _delta, h, _finis, h, h->piscina_vocationis);
    si (!h->vocatio)
    {
        _vocationem_dimittere(h);
        *error = chorda_ex_literis("question not started", piscina);
        redde NIHIL;
    }
    redde h;
}

/* R1: hospes reactorem iam poscit; hic manubrium solum legitur */
interior VicusPulsusQuaestionis
_pulsare (
     vacuum* manubrium,
    Piscina* piscina)
{
         ManubriumInternum* h = (ManubriumInternum*)manubrium;
    VicusPulsusQuaestionis  r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (h->finitus)
    {
        r.finitus = VERUM;
        redde r;
    }
    si (h->mutatum)
    {
        h->mutatum  = FALSUM;
        r.mutatum   = VERUM;
        r.textus    =
            quaestio_textum_planare(chorda_aedificator_spectare(
            h->crudum), piscina);
    }
    si (h->vocatio_finita)
    {
        r.finitus   = VERUM;
        r.error     = h->error;
        r.input     = h->input;
        r.output    = h->output;
        h->finitus  = VERUM;
        _vocationem_dimittere(h);
    }
    redde r;
}

interior vacuum
_abrumpere (
    vacuum* manubrium)
{
    ManubriumInternum* h = (ManubriumInternum*)manubrium;

    si (h->finitus)
    {
        redde;
    }
    si (h->vocatio && !h->vocatio_finita)
    {
        vates_vocationem_abrumpere(h->vocatio);   /* finis synchronus */
    }
    h->finitus = VERUM;
    _vocationem_dimittere(h);
}

VicusPortaQuaestionis
vicus_quaestio_interna (
                                  Reactor* reactor,
    constans VicusQuaestioInternaOptiones* optiones,
                                  Piscina* piscina)
{
    VicusPortaQuaestionis  porta;
             PortaInterna* pi;

    memset(&porta, ZEPHYRUM, magnitudo(porta));
    pi = (reactor && piscina) ? (PortaInterna*)piscina_allocare(piscina,
        magnitudo(PortaInterna)) : NIHIL;
    si (!pi)
    {
        redde porta;            /* incipere NIHIL: porta nulla */
    }
    memset(pi, ZEPHYRUM, magnitudo(PortaInterna));
    pi->reactor = reactor;
    si (optiones)
    {
        pi->optiones                   = *optiones;
        pi->optiones.via_clavis        = _copia(optiones->via_clavis,
            piscina);
        pi->optiones.url               = _copia(optiones->url, piscina);
    }
    porta.incipere   = _incipere;
    porta.pulsare    = _pulsare;
    porta.abrumpere  = _abrumpere;
    porta.ctx        = pi;
    redde porta;
}
