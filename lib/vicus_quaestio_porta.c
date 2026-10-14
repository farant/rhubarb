/* vicus_quaestio_porta.c - porta $question per adiutorem (vide caput;
 * quaestio-rivus T8) */
#include "vicus_quaestio_porta.h"
#include "quaestio.h"
#include "processus.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

/* $question per rivum (quaestio-rivus T8): bin/quaestio per processum
 * (clavem SOLUS adiutor legit - vicus numquam); tabulae per pulsum ex
 * effusione hactenus leguntur; textus crudus colligitur et planatur
 * (quaestio_textum_planare: eadem planatio ac sine rivo, R9) */
#define QUAESTIO_MORA_ORDINARIA_MS (CLXXX * M)   /* adiutor occiditur */

nomen structura {
    constans character* via;           /* adiutor (copia) */
                   i32  mora_maxima_ms;
} PortaAdiutoris;

nomen structura {
            Processus* processus;
                  i32  positio;    /* tabulae lectae in effusione */
    ChordaAedificator* crudum;
                  b32  finitus;
              Piscina* piscina;
} ManubriumQuaestionis;

interior vacuum*
quaestionem_incipere (
     vacuum* ctx,
     chorda  quaestio,
        s32  lineae,
        s32  latitudo,
    Piscina* piscina,
     chorda* error)
{
          PortaAdiutoris* pa = (PortaAdiutoris*)ctx;
    ManubriumQuaestionis* h;
               character  numerus_linearum[XXIV];
               character  numerus_latitudinis[XXIV];
      constans character* argumenta[X];

    h = (ManubriumQuaestionis*)piscina_allocare(piscina,
        magnitudo(ManubriumQuaestionis));
    si (!h)
    {
        *error = chorda_ex_literis("out of memory", piscina);
        redde NIHIL;
    }
    memset(h, ZEPHYRUM, magnitudo(ManubriumQuaestionis));
    sprintf(numerus_linearum, "%d", (integer)lineae);
    sprintf(numerus_latitudinis, "%d", (integer)latitudo);
    argumenta[ZEPHYRUM]  = pa->via;
    argumenta[I]         = "-exemplar";
    argumenta[II]        = QUAESTIO_EXEMPLAR;
    argumenta[III]       = "-lineae";
    argumenta[IV]        = numerus_linearum;
    argumenta[V]         = "-latitudo";
    argumenta[VI]        = numerus_latitudinis;
    argumenta[VII]       = "-quaestio";
    argumenta[VIII]      = chorda_ut_cstr(quaestio, piscina);
    argumenta[IX]        = NIHIL;
    h->piscina           = piscina;
    h->crudum            = chorda_aedificator_creare(piscina, D);
    h->processus  = processus_incipere(argumenta, pa->mora_maxima_ms,
        piscina);
    si (!h->processus)
    {
        *error = chorda_ex_literis("helper not started", piscina);
        redde NIHIL;
    }
    redde h;
}

interior VicusPulsusQuaestionis
quaestionem_pulsare (
     vacuum* manubrium,
    Piscina* piscina)
{
      ManubriumQuaestionis* h = (ManubriumQuaestionis*)manubrium;
    VicusPulsusQuaestionis  r;
           ProcessusStatus  status;
                    chorda  fluxus;
            QuaestioTabula  t;
            QuaestioLectio  lectio;

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (h->finitus)
    {
        r.finitus = VERUM;
        redde r;
    }
    status = processus_pulsare(h->processus);
    /* effusio hactenus: chorda in sacculum ipsum - statim legitur */
    fluxus = processus_effusio_hactenus(h->processus);
    dum ((lectio = quaestio_tabulam_legere(fluxus, &h->positio, &t,
              h->piscina)) == QUAESTIO_LECTIO_TABULA)
    {
        commutatio (t.genus)
        {
            casus QUAESTIO_TABULA_TEXTUS:
                chorda_aedificator_appendere_chorda(h->crudum,
                    t.textus);
                r.mutatum = VERUM;
                frange;
            casus QUAESTIO_TABULA_ERROR:
                r.error = t.textus;
                frange;
            casus QUAESTIO_TABULA_USUS:
                r.input   = t.input;
                r.output  = t.output;
                frange;
            ordinarius:
                h->finitus  = VERUM;
                r.finitus   = VERUM;
                frange;
        }
    }
    si (lectio == QUAESTIO_LECTIO_PRAVA)
    {
        processus_abrumpere(h->processus);
        h->finitus  = VERUM;
        r.finitus   = VERUM;
        r.error     = chorda_ex_literis("helper: bad frame", piscina);
    }
    alioquin si (!h->finitus && status == PROCESSUS_PARATUS)
    {
        /* adiutor mortuus sine F: causa ex erratis aut processu */
        ProcessusResultus pr = processus_metere(h->processus);
                   chorda erratum = processus_erratum_hactenus(
                       h->processus);

        h->finitus  = VERUM;
        r.finitus   = VERUM;
        r.error     = chorda_ex_literis("helper exited without answer",
            piscina);
        si (pr.error_descriptio.mensura > ZEPHYRUM)
        {
            r.error = chorda_concatenare(r.error, chorda_concatenare(
                chorda_ex_literis(": ", piscina), pr.error_descriptio,
                piscina), piscina);
        }
        alioquin si (erratum.mensura > ZEPHYRUM)
        {
            erratum.mensura = erratum.mensura
                > CC ? CC : erratum.mensura;
            r.error = chorda_concatenare(r.error, chorda_concatenare(
                chorda_ex_literis(": ", piscina), erratum, piscina),
                piscina);
        }
    }
    si (r.mutatum)
    {
        r.textus = quaestio_textum_planare(chorda_aedificator_spectare(
            h->crudum), piscina);
    }
    redde r;
}

interior vacuum
quaestionem_abrumpere (
    vacuum* manubrium)
{
    ManubriumQuaestionis* h = (ManubriumQuaestionis*)manubrium;

    si (!h->finitus)
    {
        processus_abrumpere(h->processus);
        h->finitus = VERUM;
    }
}

constans character*
vicus_quaestio_adiutoris_via (
    constans character* vocatio_prima,
               Piscina* piscina)
{
     constans character* ultimum =
         vocatio_prima ? strrchr(vocatio_prima, '/') : NIHIL;
              character* via;
                 size_t  n;

    si (!ultimum || !piscina)
    {
        redde "quaestio";
    }
    n    = (size_t)(ultimum - vocatio_prima) + I;
    via  = (character*)piscina_allocare(piscina,
        (memoriae_index)(n + IX));
    si (!via)
    {
        redde "quaestio";
    }
    memcpy(via, vocatio_prima, n);
    strcpy(via + n, "quaestio");
    redde via;
}

VicusPortaQuaestionis
vicus_quaestio_porta (
    constans character* via_adiutoris,
                   i32  mora_maxima_ms,
              Piscina* piscina)
{
    VicusPortaQuaestionis  porta;
           PortaAdiutoris* pa;
                character* via;

    memset(&porta, ZEPHYRUM, magnitudo(porta));
    pa = piscina ? (PortaAdiutoris*)piscina_allocare(piscina,
        magnitudo(PortaAdiutoris)) : NIHIL;
    si (!pa || !via_adiutoris)
    {
        redde porta;   /* incipere NIHIL: porta nulla */
    }
    via = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(via_adiutoris) + I);
    si (!via)
    {
        redde porta;
    }
    strcpy(via, via_adiutoris);
    pa->via             = via;
    pa->mora_maxima_ms  = mora_maxima_ms > ZEPHYRUM ? mora_maxima_ms
        : QUAESTIO_MORA_ORDINARIA_MS;
    porta.incipere   = quaestionem_incipere;
    porta.pulsare    = quaestionem_pulsare;
    porta.abrumpere  = quaestionem_abrumpere;
    porta.ctx        = pa;
    redde porta;
}
