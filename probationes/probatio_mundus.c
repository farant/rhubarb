/* probatio_mundus.c - corpora in solo (mundus.h)
 *
 * Numeris solis, in loco ficto (mundus_locus_fictus.h). celeritas:
 * eadem quovis gradu; obliquum non celerius. rupes: corpus sistit.
 * lapsus: secundum parietem labitur. clivus et scalae: altitudo
 * sequitur. margo: nec extra locum nec in aquam nec in saxum. mora
 * longa: gradus cogitur. facies: ad intentum, via breviore, etiam
 * obstructa. ponere. vacua.
 */
#include "latina.h"
#include "piscina.h"
#include "statua.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <string.h>

nomen structura {
          Mundus mundus;
    MundusCorpus corpus;
} Scaena;

/* mundus cum corpore uno (radius ordinarius) in (x, z) */
interior Scaena*
_scaena (
    CredoContextus* c,
               f32  x,
               f32  z)
{
    Scaena* s = (Scaena*)loci_ficti_memoria(c, magnitudo(Scaena));

    s->mundus.solum     = mundus_solum_struere(loci_ficti_locus(c),
        c->piscina);
    s->mundus.corpora   = &s->corpus;
    s->mundus.corporum  = I;
    s->corpus.radius    = MUNDUS_CORPORIS_RADIUS;
    CREDO_VERUM (s->mundus.solum != NIHIL);
    CREDO_VERUM (mundus_corpus_ponere(&s->mundus, &s->corpus, x, z));
    redde s;
}

interior vacuum
_intendere (
    Scaena* s,
       f32  x,
       f32  z)
{
    s->corpus.intendit    = VERUM;
    s->corpus.intentum_x  = x;
    s->corpus.intentum_z  = z;
}

/* 'gradus' gradus temporis dt */
interior vacuum
_currere (
    Scaena* s,
       i32  gradus,
       f32  dt)
{
    i32 k;

    per (k = ZEPHYRUM; k < gradus; k++)
    {
        mundus_gradi(&s->mundus, dt);
    }
}

interior vacuum
_sectio_celeritas (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 1.0f, 6.0f);

    /* secunda una in gradibus tricesimae: IV unitates */
    _intendere(s, 1.0f, 0.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 5.0f, 0.05f);
    CREDO_F32_PROXIMUS (s->corpus.z, 6.0f, 0.001f);
    CREDO_VERUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 1.0f, 0.02f);
    /* gradus maiores quam limes: VII gradus septimae secundae ad 0.1
     * coguntur = 0.7 s = 2.8 unitates */
    s = _scaena(c, 1.0f, 6.0f);
    _intendere(s, 1.0f, 0.0f);
    _currere(s, VII, 1.0f / 7.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 3.8f, 0.05f);
    /* longitudo intenti neglegitur */
    s = _scaena(c, 1.0f, 6.0f);
    _intendere(s, 250.0f, 0.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 5.0f, 0.05f);
    /* gradus minutissimi: idem iter */
    s = _scaena(c, 1.0f, 6.0f);
    _intendere(s, 1.0f, 0.0f);
    _currere(s, M, 0.001f);
    CREDO_F32_PROXIMUS (s->corpus.x, 5.0f, 0.05f);
}

interior vacuum
_sectio_diagonalis (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 1.0f, 4.0f);
       f32  delta_x;
       f32  delta_z;

    _intendere(s, 1.0f, 1.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    delta_x = s->corpus.x - 1.0f;
    delta_z = s->corpus.z - 4.0f;
    CREDO_F32_PROXIMUS (delta_x * delta_x + delta_z * delta_z, 16.0f,
        0.5f);
    CREDO_F32_PROXIMUS (delta_x, delta_z, 0.01f);
}

interior vacuum
_sectio_rupes (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 5.0f, 5.0f);

    _intendere(s, 1.0f, 0.0f);
    _currere(s, LX, 1.0f / 30.0f);
    /* ante parietem (x 8) sistit, radio distans */
    CREDO_VERUM (s->corpus.x > 7.0f);
    CREDO_VERUM (s->corpus.x < 7.75f);
    CREDO_F32_PROXIMUS (s->corpus.y, 0.0f, 0.001f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.0f, 0.001f);
    /* a superiore: in margine sistit, non decidit */
    s = _scaena(c, 10.0f, 5.0f);
    _intendere(s, -1.0f, 0.0f);
    _currere(s, LX, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x > 8.25f);
    CREDO_VERUM (s->corpus.x < 9.0f);
    CREDO_F32_PROXIMUS (s->corpus.y, 4.0f, 0.001f);
    CREDO_FALSUM (s->corpus.movetur);
}

interior vacuum
_sectio_lapsus_parietis (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 6.0f, 3.5f);

    /* oblique in parietem: x sistit, z pergit */
    _intendere(s, 1.0f, 1.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x > 7.0f);
    CREDO_VERUM (s->corpus.x < 7.75f);
    CREDO_VERUM (s->corpus.z > 5.8f);
    CREDO_VERUM (s->corpus.movetur);
}

interior vacuum
_sectio_clivus (
    CredoContextus* c)
{
    Scaena* s       = _scaena(c, 2.0f, 10.0f);
       f32  medium  = -1.0f;
       i32  k;

    _intendere(s, 1.0f, 0.0f);
    per (k = ZEPHYRUM; k < LXXV; k++)
    {
        mundus_gradi(&s->mundus, 1.0f / 30.0f);
        si (k == XXIX)
        {
            medium = s->corpus.y;
        }
    }
    /* post secundam: x 6, clivus medius */
    CREDO_F32_PROXIMUS (medium, 2.0f, 0.4f);
    CREDO_VERUM (s->corpus.x > 10.0f);
    CREDO_F32_PROXIMUS (s->corpus.y, 4.0f, 0.001f);
    /* et deorsum */
    _intendere(s, -1.0f, 0.0f);
    _currere(s, LXXV, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x < 3.0f);
    CREDO_F32_PROXIMUS (s->corpus.y, 0.0f, 0.001f);
    /* scalae */
    s = _scaena(c, 2.0f, 14.0f);
    _intendere(s, 1.0f, 0.0f);
    _currere(s, LXXV, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x > 10.0f);
    CREDO_F32_PROXIMUS (s->corpus.y, 4.0f, 0.001f);
    _intendere(s, -1.0f, 0.0f);
    _currere(s, LXXV, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x < 3.0f);
    CREDO_F32_PROXIMUS (s->corpus.y, 0.0f, 0.001f);
}

interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 2.0f, 4.5f);

    /* extra locum */
    _intendere(s, -1.0f, 0.0f);
    _currere(s, LX, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x >= 0.4f);
    CREDO_VERUM (s->corpus.x < 1.0f);
    CREDO_VERUM (mundus_solum_stare(s->mundus.solum, s->corpus.x,
        s->corpus.z, s->corpus.radius));
    /* in aquam (z [1, 3]) */
    s = _scaena(c, 2.0f, 4.5f);
    _intendere(s, 0.0f, -1.0f);
    _currere(s, LX, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.z > 3.25f);
    CREDO_VERUM (s->corpus.z < 4.0f);
    /* in saxum (x [5, 6], z [1, 2]) */
    s = _scaena(c, 3.8f, 1.5f);
    _intendere(s, 1.0f, 0.0f);
    _currere(s, LX, 1.0f / 30.0f);
    CREDO_VERUM (s->corpus.x < 4.75f);
    CREDO_VERUM (s->corpus.x > 4.0f);
}

interior vacuum
_sectio_mora_longa (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 6.9f, 5.0f);

    _intendere(s, 1.0f, 0.0f);
    mundus_gradi(&s->mundus, 5.0f);
    CREDO_VERUM (s->corpus.x - 6.9f <= 0.41f);
    CREDO_VERUM (s->corpus.x > 6.95f);
    CREDO_VERUM (s->corpus.x < 7.75f);
    CREDO_F32_PROXIMUS (s->corpus.y, 0.0f, 0.001f);
    /* in aperto: gradus unus maximus = 0.4 unitates */
    s = _scaena(c, 1.0f, 6.0f);
    _intendere(s, 1.0f, 0.0f);
    mundus_gradi(&s->mundus, 5.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 1.4f, 0.01f);
    /* dt malum: nihil movetur */
    mundus_gradi(&s->mundus, -1.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 1.4f, 0.01f);
    CREDO_FALSUM (s->corpus.movetur);
    mundus_gradi(&s->mundus, 0.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 1.4f, 0.01f);
}

interior vacuum
_sectio_facies (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 1.0f, 6.0f);

    CREDO_F32_PROXIMUS (s->corpus.facies, 0.0f, 0.001f);
    /* gradus unus: versura XXIV gradus, non plus */
    _intendere(s, 1.0f, 0.0f);
    mundus_gradi(&s->mundus, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 24.0f, 0.01f);
    _currere(s, XXIX, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 90.0f, 0.01f);
    /* intentum sublatum: facies manet, motus desinit */
    s->corpus.intendit = FALSUM;
    mundus_gradi(&s->mundus, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 90.0f, 0.01f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.0f, 0.001f);
    CREDO_F32_PROXIMUS (s->corpus.x, 5.0f, 0.05f);
    /* via breviore trans 0: a CCCL ad X per XX gradus, non per CCCXL */
    s->corpus.facies = 350.0f;
    _intendere(s, 0.17364818f, 0.98480775f);
    mundus_gradi(&s->mundus, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 10.0f, 0.05f);
    /* et retro trans 0: a X ad CCCL (differentia cruda CCCXL) */
    s->corpus.facies = 10.0f;
    _intendere(s, -0.17364818f, 0.98480775f);
    mundus_gradi(&s->mundus, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 350.0f, 0.05f);
    /* -z = CLXXX; -x = CCLXX */
    _intendere(s, 0.0f, -1.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 180.0f, 0.01f);
    _intendere(s, -1.0f, 0.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 270.0f, 0.01f);
    /* obstructum quoque vertitur */
    s = _scaena(c, 7.4f, 5.0f);
    _intendere(s, 1.0f, 0.0f);
    _currere(s, XXX, 1.0f / 30.0f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.facies, 90.0f, 0.01f);
    /* intentum nullum: nec motus nec facies nec numerus corruptus */
    s = _scaena(c, 1.0f, 6.0f);
    _intendere(s, 0.0f, 0.0f);
    _currere(s, X, 1.0f / 30.0f);
    CREDO_F32_PROXIMUS (s->corpus.x, 1.0f, 0.0001f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 0.0f, 0.0001f);
    CREDO_FALSUM (s->corpus.movetur);
}

/* tempus quietis: crescit dum corpus stat, purgatur dum movetur */
interior vacuum
_sectio_quies (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 1.0f, 6.0f);

    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.0f, 0.0001f);
    _currere(s, X, 0.05f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.5f, 0.001f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.0f, 0.0001f);
    /* gradus longus ad 0.1 cogitur; dt malum nihil addit */
    mundus_gradi(&s->mundus, 5.0f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.6f, 0.001f);
    mundus_gradi(&s->mundus, -1.0f);
    mundus_gradi(&s->mundus, 0.0f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.6f, 0.001f);
    /* intentum sine directione: stat */
    _intendere(s, 0.0f, 0.0f);
    _currere(s, II, 0.05f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.7f, 0.001f);
    /* movetur: purgatur */
    _intendere(s, 1.0f, 0.0f);
    _currere(s, II, 0.05f);
    CREDO_VERUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.0f, 0.0001f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.1f, 0.001f);
    /* sistit: a nihilo iterum */
    s->corpus.intendit = FALSUM;
    _currere(s, III, 0.05f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.15f, 0.001f);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.0f, 0.0001f);
    /* intendit sed impeditur (margo loci): stat, tempus crescit */
    s = _scaena(c, 2.0f, 4.5f);
    _intendere(s, -1.0f, 0.0f);
    _currere(s, LX, 1.0f / 30.0f);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_VERUM (s->corpus.tempus_quietis > 1.0f);
    CREDO_VERUM (s->corpus.tempus_quietis < 1.9f);
}

interior vacuum
_sectio_ponere (
    CredoContextus* c)
{
          Scaena* s = _scaena(c, 10.0f, 5.0f);
    MundusCorpus  prius;

    CREDO_F32_PROXIMUS (s->corpus.y, 4.0f, 0.001f);
    s->corpus.facies          = 123.0f;
    s->corpus.forma           = VII;
    s->corpus.movetur         = VERUM;
    s->corpus.tempus_motus    = 2.0f;
    s->corpus.tempus_quietis  = 3.0f;
    CREDO_VERUM (mundus_corpus_ponere(&s->mundus, &s->corpus, 2.0f,
        6.0f));
    CREDO_F32_PROXIMUS (s->corpus.tempus_quietis, 0.0f, 0.0001f);
    CREDO_F32_PROXIMUS (s->corpus.x, 2.0f, 0.0001f);
    CREDO_F32_PROXIMUS (s->corpus.y, 0.0f, 0.001f);
    CREDO_F32_PROXIMUS (s->corpus.facies, 123.0f, 0.0001f);
    CREDO_AEQUALIS_S32 (s->corpus.forma, VII);
    CREDO_FALSUM (s->corpus.movetur);
    CREDO_F32_PROXIMUS (s->corpus.tempus_motus, 0.0f, 0.0001f);
    /* recusata: corpus non mutatur */
    prius = s->corpus;
    CREDO_FALSUM (mundus_corpus_ponere(&s->mundus, &s->corpus, 2.0f,
        2.0f));
    CREDO_FALSUM (mundus_corpus_ponere(&s->mundus, &s->corpus, 7.8f,
        5.0f));
    CREDO_FALSUM (mundus_corpus_ponere(&s->mundus, &s->corpus, -5.0f,
        5.0f));
    CREDO_VERUM (memcmp(&prius, &s->corpus, magnitudo(prius))
        == ZEPHYRUM);
    CREDO_FALSUM (mundus_corpus_ponere(NIHIL, &s->corpus, 2.0f, 6.0f));
    CREDO_FALSUM (mundus_corpus_ponere(&s->mundus, NIHIL, 2.0f, 6.0f));
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Scaena* s = _scaena(c, 1.0f, 6.0f);
    Mundus  vacuus;

    /* corpora plura: quodque suo intento */
    {
        MundusCorpus duo[II];

        memset(duo, ZEPHYRUM, magnitudo(duo));
        duo[ZEPHYRUM].radius  = MUNDUS_CORPORIS_RADIUS;
        duo[I].radius         = MUNDUS_CORPORIS_RADIUS;
        s->mundus.corpora     = duo;
        s->mundus.corporum    = II;
        CREDO_VERUM (mundus_corpus_ponere(&s->mundus, &duo[ZEPHYRUM],
            1.0f, 6.0f));
        CREDO_VERUM (mundus_corpus_ponere(&s->mundus, &duo[I], 1.0f,
            6.0f));
        duo[I].intendit    = VERUM;
        duo[I].intentum_x  = 1.0f;
        _currere(s, XXX, 1.0f / 30.0f);
        CREDO_F32_PROXIMUS (duo[ZEPHYRUM].x, 1.0f, 0.0001f);
        CREDO_F32_PROXIMUS (duo[I].x, 5.0f, 0.05f);
    }
    /* mundus sine solo, sine corporibus, NIHIL: nihil fit */
    memset(&vacuus, ZEPHYRUM, magnitudo(vacuus));
    mundus_gradi(&vacuus, 0.1f);
    mundus_gradi(NIHIL, 0.1f);
    vacuus.solum = s->mundus.solum;
    mundus_gradi(&vacuus, 0.1f);
    CREDO_AEQUALIS_I32 (vacuus.corporum, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "celeritas", _sectio_celeritas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "diagonalis", _sectio_diagonalis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rupes", _sectio_rupes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lapsus_parietis", _sectio_lapsus_parietis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "clivus", _sectio_clivus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mora_longa", _sectio_mora_longa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "facies", _sectio_facies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quies", _sectio_quies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ponere", _sectio_ponere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus", SECTIONES);
}
