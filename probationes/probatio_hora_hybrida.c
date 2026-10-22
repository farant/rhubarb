/* probatio_hora_hybrida.c - hora hybrida (genera-datorum D2):
 * compositio, signum numquam retro, horologium retro, numerator
 * plenus in millisecundam proximam transit */
#include "latina.h"
#include "hora_hybrida.h"
#include "credo.h"

/* MMXXVI-X-XX hora X (UTC): millisecunda ab epocha */
#define TEMPUS_PROBATIONIS ((Momentum)1792490400 * M + CXXIII)

interior vacuum
_probare_compositionem (
    CredoContextus* c)
{
    s64 h;

    (vacuum)c;
    h = hora_hybrida_componere(TEMPUS_PROBATIONIS, V);
    CREDO_AEQUALIS_S64(hora_hybrida_momentum(h), TEMPUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(hora_hybrida_numerator(h), V);
    /* ordo numericus = ordo temporis, deinde numeratoris */
    CREDO_AEQUALIS_S64(hora_hybrida_componere(TEMPUS_PROBATIONIS,
        ZEPHYRUM), TEMPUS_PROBATIONIS * (s64)LXV * (s64)M
        + TEMPUS_PROBATIONIS * (s64)DXXXVI);
    CREDO_VERUM(hora_hybrida_componere(TEMPUS_PROBATIONIS, VI) > h);
    CREDO_VERUM(hora_hybrida_componere(TEMPUS_PROBATIONIS + I,
        ZEPHYRUM) > hora_hybrida_componere(TEMPUS_PROBATIONIS,
        (i32)LXV * (i32)M + (i32)DXXXV));
}

interior vacuum
_probare_proximam (
    CredoContextus* c)
{
    s64 h;
    s64 prior;

    (vacuum)c;
    /* initium: tempus physicum, numerator nullus */
    h = hora_hybrida_proxima(ZEPHYRUM, TEMPUS_PROBATIONIS);
    CREDO_AEQUALIS_S64(h, hora_hybrida_componere(TEMPUS_PROBATIONIS,
        ZEPHYRUM));
    /* eadem millisecunda: numerator crescit */
    prior  = h;
    h      = hora_hybrida_proxima(prior, TEMPUS_PROBATIONIS);
    CREDO_AEQUALIS_S64(hora_hybrida_momentum(h), TEMPUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(hora_hybrida_numerator(h), I);
    /* horologium retro salit: signum tamen crescit */
    prior  = h;
    h      = hora_hybrida_proxima(prior, TEMPUS_PROBATIONIS - M);
    CREDO_VERUM(h > prior);
    CREDO_AEQUALIS_S64(hora_hybrida_momentum(h), TEMPUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(hora_hybrida_numerator(h), II);
    /* horologium progreditur: numerator renascitur */
    h = hora_hybrida_proxima(h, TEMPUS_PROBATIONIS + I);
    CREDO_AEQUALIS_S64(h, hora_hybrida_componere(TEMPUS_PROBATIONIS + I,
        ZEPHYRUM));
}

interior vacuum
_probare_numeratorem_plenum (
    CredoContextus* c)
{
    s64 prior;
    s64 h;

    (vacuum)c;
    prior = hora_hybrida_componere(TEMPUS_PROBATIONIS,
        (i32)LXV * (i32)M + (i32)DXXXV);
    h = hora_hybrida_proxima(prior, TEMPUS_PROBATIONIS);
    CREDO_VERUM(h > prior);
    CREDO_AEQUALIS_S64(h, hora_hybrida_componere(TEMPUS_PROBATIONIS + I,
        ZEPHYRUM));
}

interior vacuum
_probare_physicam (
    CredoContextus* c)
{
    Momentum a;
    Momentum b;

    (vacuum)c;
    a = hora_hybrida_physica();
    b = hora_hybrida_physica();
    /* post MMXXV, ante MMC; non retro inter vocationes proximas */
    CREDO_VERUM(a > (Momentum)1735689600 * M);
    CREDO_VERUM(a < (Momentum)4102444800 * M);
    CREDO_VERUM(b >= a);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "compositio",       _probare_compositionem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "proxima",          _probare_proximam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "numerator_plenus", _probare_numeratorem_plenum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "physica",          _probare_physicam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("hora_hybrida", SECTIONES);
}
