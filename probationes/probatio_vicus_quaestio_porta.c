/* probatio_vicus_quaestio_porta.c - porta $question VERA per processum
 * (quaestio-rivus T8, ruling R10)
 *
 * Adiutor fictus (probationes/fixa/quaestio/adiutor_fictus.sh) tabulas
 * scribit ut bin/quaestio, scaena per -quaestio: bonum (textus
 * planatus,
 * usus, finis), mors (adiutor sine F moritur: error nominatus cum
 * erratis), pravum (tabula prava: error, adiutor occiditur), error (E +
 * F), lente (textus primus, deinde abrumpere statim finit). Argumenta
 * adiutori: lineae et latitudo ut data. Via adiutoris iuxta argv[0].
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "vicus_applicatio.h"
#include "vicus_quaestio_porta.h"
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define ADIUTOR "probationes/fixa/quaestio/adiutor_fictus.sh"

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* pulsus usque ad finem (aut tectum ms); summa: textus ultimus, error,
 * usus */
nomen structura {
                       b32 finitus;
                    chorda textus;
                    chorda error;
                       s64 input;
                       s64 output;
                       i32 pulsus_textus;   /* pulsus cum mutato */
} Summa;

interior Summa
_currere (
    VicusPortaQuaestionis* porta,
                   vacuum* manubrium,
                      s64  tectum_ms,
                      b32  usque_ad_textum,
                  Piscina* p)
{
    Summa s;
      s64 initium = _ms_nunc();

    memset(&s, ZEPHYRUM, magnitudo(s));
    dum (_ms_nunc() - initium < tectum_ms)
    {
        VicusPulsusQuaestionis r = porta->pulsare(manubrium, p);

        si (r.mutatum)
        {
            s.textus = chorda_concatenare(chorda_ex_literis("", p),
                r.textus, p);
            s.pulsus_textus++;
        }
        si (r.error.mensura > ZEPHYRUM)
        {
            s.error = chorda_concatenare(chorda_ex_literis("", p),
                r.error, p);
        }
        si (r.input || r.output)
        {
            s.input   = r.input;
            s.output  = r.output;
        }
        si (r.finitus)
        {
            s.finitus = VERUM;
            frange;
        }
        si (usque_ad_textum && s.pulsus_textus > ZEPHYRUM)
        {
            frange;
        }
        (vacuum)usleep(X * M);
    }
    redde s;
}

interior vacuum*
_incipere (
    VicusPortaQuaestionis* porta,
       constans character* scaena,
                  Piscina* p)
{
    chorda error;

    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    redde porta->incipere(porta->ctx, chorda_ex_literis(scaena, p), X,
        LVI, p, &error);
}

interior vacuum
_sectio_bonum (
    CredoContextus* c)
{
    VicusPortaQuaestionis porta = vicus_quaestio_porta(ADIUTOR, V * M,
        c->piscina);
                   vacuum* h;
                    Summa  s;

    CREDO_NECESSE(porta.incipere != NIHIL);
    h = _incipere(&porta, "bonum", c->piscina);
    CREDO_NECESSE_NON_NIHIL(h);
    s = _currere(&porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(s.finitus);
    CREDO_AEQUALIS_I32(s.error.mensura, ZEPHYRUM);
    /* textus planatus totus (crudum collectum) */
    CREDO_CHORDA_AEQUALIS_LITERIS(s.textus, "Salve munde.");
    CREDO_AEQUALIS_S64(s.input, XII);
    CREDO_AEQUALIS_S64(s.output, XXXIV);
    /* post finem: finitus statim, nihil novi */
    CREDO_VERUM(porta.pulsare(h, c->piscina).finitus);
}

/* adiutor sine F moritur: error nominatus, textus servatus */
interior vacuum
_sectio_mors (
    CredoContextus* c)
{
    VicusPortaQuaestionis porta = vicus_quaestio_porta(ADIUTOR, V * M,
        c->piscina);
                   vacuum* h;
                    Summa  s;

    h = _incipere(&porta, "mors", c->piscina);
    CREDO_NECESSE_NON_NIHIL(h);
    s = _currere(&porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(s.finitus);
    CREDO_CHORDA_CONTINET(s.error, chorda_ex_literis(
        "helper exited without answer", c->piscina));
    CREDO_CHORDA_CONTINET(s.error, chorda_ex_literis("moritur",
        c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(s.textus, "Pars");
}

/* tabula prava: error, adiutor occiditur (non V s exspectatur) */
interior vacuum
_sectio_pravum (
    CredoContextus* c)
{
    VicusPortaQuaestionis porta = vicus_quaestio_porta(ADIUTOR, X * M,
        c->piscina);
                   vacuum* h;
                    Summa  s;
                      s64  initium;

    h = _incipere(&porta, "pravum", c->piscina);
    CREDO_NECESSE_NON_NIHIL(h);
    initium  = _ms_nunc();
    s        = _currere(&porta, h, IV * M, FALSUM, c->piscina);
    CREDO_VERUM(s.finitus);
    CREDO_CHORDA_CONTINET(s.error, chorda_ex_literis("bad frame",
        c->piscina));
    CREDO_VERUM(_ms_nunc() - initium < II * M);
}

interior vacuum
_sectio_error (
    CredoContextus* c)
{
    VicusPortaQuaestionis porta = vicus_quaestio_porta(ADIUTOR, V * M,
        c->piscina);
                   vacuum* h;
                    Summa  s;

    h = _incipere(&porta, "error", c->piscina);
    CREDO_NECESSE_NON_NIHIL(h);
    s = _currere(&porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(s.finitus);
    CREDO_CHORDA_AEQUALIS_LITERIS(s.error, "Overloaded");
    CREDO_AEQUALIS_I32(s.textus.mensura, ZEPHYRUM);
}

/* lente: textus primus advenit dum adiutor currit; abrumpere statim
 * finit (adiutor XXX s dormit) */
interior vacuum
_sectio_lente (
    CredoContextus* c)
{
    VicusPortaQuaestionis porta = vicus_quaestio_porta(ADIUTOR, LX * M,
        c->piscina);
                   vacuum* h;
                    Summa  s;
                      s64  initium;

    h = _incipere(&porta, "lente", c->piscina);
    CREDO_NECESSE_NON_NIHIL(h);
    s = _currere(&porta, h, III * M, VERUM, c->piscina);
    CREDO_FALSUM(s.finitus);
    CREDO_CHORDA_AEQUALIS_LITERIS(s.textus, "Prima");
    initium = _ms_nunc();
    porta.abrumpere(h);
    CREDO_VERUM(porta.pulsare(h, c->piscina).finitus);
    CREDO_VERUM(_ms_nunc() - initium < M);
}

/* R4: via adiutoris iuxta argv[0] */
interior vacuum
_sectio_via (
    CredoContextus* c)
{
    CREDO_VERUM(strcmp(vicus_quaestio_adiutoris_via("bin/vicus",
        c->piscina), "bin/quaestio") == ZEPHYRUM);
    CREDO_VERUM(strcmp(vicus_quaestio_adiutoris_via(
        "/a/b/build/vicus", c->piscina), "/a/b/build/quaestio")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(vicus_quaestio_adiutoris_via("vicus",
        c->piscina), "quaestio") == ZEPHYRUM);
    CREDO_VERUM(strcmp(vicus_quaestio_adiutoris_via(NIHIL, c->piscina),
        "quaestio") == ZEPHYRUM);
    /* via NIHIL: porta nulla */
    CREDO_VERUM(vicus_quaestio_porta(NIHIL, ZEPHYRUM,
        c->piscina).incipere
        == NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bonum", _sectio_bonum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mors", _sectio_mors, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pravum", _sectio_pravum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error", _sectio_error, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lente", _sectio_lente, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via", _sectio_via, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_quaestio_porta", SECTIONES);
}
