/* probatio_concentus.c - Probationes hospitis soni communis
 * (include/concentus.h)
 *
 * Exitus fictus: mixtura offline, horologium manuale - quadra per
 * agmen CCLVI redduntur (exitu aperto) et ms hospitis procedunt etiam
 * exitu clauso. Numerantur aperta, clausa, iussa; recusare (aperire) et
 * recusare_iussa (anulus plenus) fingunt defectus.
 */
#include "latina.h"
#include "credo.h"
#include "sonitus.h"
#include "tibia.h"
#include "concentus.h"
#include <stdio.h>
#include <string.h>

#define AGMEN CCLVI

nomen structura {
     Mixtura* mixtura;
         b32  apertum;
         b32  recusare;
         b32  recusare_iussa;
         i32  mora;        /* latentia ficta, quadra */
         i32  aperta;
         i32  clausa;
         i32  iussa;
         s64  quadra;      /* horologium hospitis, quadra */
} ExitusFictus;

hic_manens f32 exitus_agminis[II * AGMEN];

interior vacuum
_parare_agmen (
    CredoContextus* c)
{
    (vacuum)c;
    memset(exitus_agminis, ZEPHYRUM, magnitudo(exitus_agminis));
}

interior b32
_aperire (
     vacuum* ctx,
    Mixtura* mixtura)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    f->mixtura = mixtura;
    si (f->recusare)
    {
        redde FALSUM;
    }
    f->apertum = VERUM;
    f->aperta++;
    redde VERUM;
}

interior vacuum
_claudere (
    vacuum* ctx)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    f->apertum = FALSUM;
    f->clausa++;
}

interior s64
_momentum (
    vacuum* ctx)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    redde f->mixtura != NIHIL ? mixtura_momentum(f->mixtura) : ZEPHYRUM;
}

interior i32
_mora (
    vacuum* ctx)
{
    redde ((ExitusFictus*)ctx)->mora;
}

interior b32
_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    si (f->recusare_iussa)
    {
        redde FALSUM;
    }
    f->iussa++;
    redde mixtura_iubere(f->mixtura, iussum);
}

interior b32
_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    redde f->mixtura != NIHIL && mixtura_nuntium_legere(f->mixtura,
        nuntius);
}

interior s64
_nunc (
    constans ExitusFictus* f)
{
    redde f->quadra * (s64)M / (s64)SONUS_FREQUENTIA;
}

/* tempus 'ms' procedit per agmina: redditur si apertum, deinde agere */
interior vacuum
_currere (
       Concentus* c,
    ExitusFictus* f,
             s64  ms)
{
    s64 finis = f->quadra + ms * (s64)SONUS_FREQUENTIA / (s64)M;

    dum (f->quadra < finis)
    {
        si (f->apertum && f->mixtura != NIHIL)
        {
            mixtura_reddere(f->mixtura, exitus_agminis, AGMEN);
        }
        f->quadra += AGMEN;
        (vacuum)concentus_agere(c, _nunc(f));
    }
}

interior Concentus*
_parare (
        CredoContextus* c,
          ExitusFictus* f,
       ConcentusExitus* exitus,
                   i32  voces)
{
    MixturaForma forma;

    memset(f, ZEPHYRUM, magnitudo(*f));
    f->mora                 = AGMEN;
    exitus->aperire         = _aperire;
    exitus->claudere        = _claudere;
    exitus->momentum        = _momentum;
    exitus->mora            = _mora;
    exitus->iubere          = _iubere;
    exitus->nuntium_legere  = _nuntium_legere;
    exitus->ctx             = f;
    forma.voces             = voces;
    forma.iussa             = IV * voces;
    forma.nuntii            = II * voces;
    redde concentus_creare(c->piscina, &forma, exitus);
}

/* sonitus brevis (radius, ~0.23 s) in sede capta */
interior MixturaAnsa
_sonitum_incipere (
    Concentus* c,
          i32  prioritas)
{
    MixturaIussum i;
      MixturaAnsa a;

    a = mixtura_rector_capere(concentus_rector(c), prioritas);
    si (mixtura_ansa_nulla(a))
    {
        redde a;
    }
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM,
                          concentus_initium(c));
    i.ansa       = a;
    i.parametra  = *sonitus_praeparatum("radius");
    i.semen      = I;
    (vacuum)concentus_iubere(c, &i);
    redde a;
}


    /* ==================================================
     * CREATIO: formae invalidae, piger, clausus
     * ================================================== */

interior vacuum
_sectio_creatio (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k;
       MixturaForma  forma;
      MixturaIussum  i;

    CREDO_VERUM (_parare(c, &f, &exitus, ZEPHYRUM) == NIHIL);
    /* nuntii < II x voces: nuntius amissus = sedes numquam libera */
    forma.voces   = VIII;
    forma.iussa   = XXXII;
    forma.nuntii  = XV;
    CREDO_VERUM (concentus_creare(c->piscina, &forma, &exitus)
        == NIHIL);
    k = _parare(c, &f, &exitus, VIII);
    CREDO_NECESSE (k != NIHIL);
    CREDO_FALSUM (concentus_apertum(k));
    CREDO_AEQUALIS_I32 (f.aperta, ZEPHYRUM);
    CREDO_VERUM (concentus_rector(k) != NIHIL);
    CREDO_AEQUALIS_S64 (concentus_initium(k), ZEPHYRUM);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_CENSUM_PETERE, ZEPHYRUM);
    CREDO_FALSUM (concentus_iubere(k, &i));
    CREDO_AEQUALIS_I32 (f.iussa, ZEPHYRUM);
    CREDO_FALSUM (concentus_agere(k, ZEPHYRUM));
}


    /* ==================================================
     * APERIRE: semel, recusatio, initium
     * ================================================== */

interior vacuum
_sectio_aperire (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);

    CREDO_NECESSE (k != NIHIL);
    f.recusare = VERUM;
    CREDO_FALSUM (concentus_aperire(k));
    CREDO_FALSUM (concentus_apertum(k));
    f.recusare = FALSUM;
    CREDO_VERUM (concentus_aperire(k));
    CREDO_VERUM (concentus_aperire(k));
    CREDO_AEQUALIS_I32 (f.aperta, I);
    CREDO_VERUM (concentus_apertum(k));
    /* initium: momentum + mora + margo (mora magna: margo eam non
     * tegit) */
    _currere(k, &f, X);
    f.mora = SONUS_FREQUENTIA / II;
    CREDO_VERUM (concentus_initium(k) >= _momentum(&f) + f.mora);
    CREDO_VERUM (concentus_initium(k) < _momentum(&f) + f.mora
                 + SONUS_FREQUENTIA / X);
    f.mora = AGMEN;
}


    /* ==================================================
     * VOX: vivit donec nuntius legitur
     * ================================================== */

interior vacuum
_sectio_vox (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);
        MixturaAnsa  a;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    a = _sonitum_incipere(k, I);
    CREDO_NECESSE (!mixtura_ansa_nulla(a));
    CREDO_AEQUALIS_I32 (f.iussa, I);
    CREDO_VERUM (concentus_vox_vivit(k, a));
    CREDO_VERUM (mixtura_rector_vivit(concentus_rector(k), a));
    CREDO_AEQUALIS_I32 (mixtura_rector_occupatae(concentus_rector(k)),
        I);
    _currere(k, &f, C);
    CREDO_VERUM (concentus_vox_vivit(k, a));
    CREDO_VERUM (concentus_agere(k, _nunc(&f)));
    /* radius ~0.23 s: finitus intra D ms, nuntius lectus */
    _currere(k, &f, D);
    CREDO_FALSUM (concentus_vox_vivit(k, a));
    CREDO_AEQUALIS_I32 (mixtura_rector_occupatae(concentus_rector(k)),
                        ZEPHYRUM);
    CREDO_FALSUM (concentus_agere(k, _nunc(&f)));
    /* ansa stala: sedes eadem a voce nova occupata */
    {
        MixturaAnsa b = _sonitum_incipere(k, I);

        CREDO_NECESSE (b.sedes == a.sedes
            && b.generatio != a.generatio);
        CREDO_VERUM (concentus_vox_vivit(k, b));
        CREDO_FALSUM (concentus_vox_vivit(k, a));
    }
}


    /* ==================================================
     * RECUSATUM: incipiens sedem liberat, cetera non
     * ================================================== */

interior vacuum
_sectio_recusatum (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);
        MixturaAnsa  a;
        MixturaAnsa  b;
      MixturaIussum  i;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    a = _sonitum_incipere(k, I);
    CREDO_NECESSE (concentus_vox_vivit(k, a));
    f.recusare_iussa  = VERUM;
    b                 = _sonitum_incipere(k, I);
    CREDO_VERUM (!mixtura_ansa_nulla(b));
    CREDO_FALSUM (concentus_vox_vivit(k, b));
    CREDO_AEQUALIS_I32 (mixtura_rector_occupatae(concentus_rector(k)),
        I);
    /* SISTERE recusatum: vox viva manet viva */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE,
                          concentus_initium(k));
    i.ansa = a;
    CREDO_FALSUM (concentus_iubere(k, &i));
    CREDO_VERUM (concentus_vox_vivit(k, a));
    /* dimittere ansae stalae (sedes a, generatio alia) vocem vivam non
     * tollit */
    b            = a;
    b.generatio  = a.generatio + I;
    mixtura_rector_dimittere(concentus_rector(k), b);
    CREDO_VERUM (concentus_vox_vivit(k, a));
    f.recusare_iussa = FALSUM;
}


    /* ==================================================
     * QUIES: post CONCENTUS_QUIES clauditur, iterum aperitur
     * ================================================== */

interior vacuum
_sectio_quies (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    (vacuum)_sonitum_incipere(k, I);
    _currere(k, &f, D);
    _currere(k, &f, (s64)CONCENTUS_QUIES - M);
    CREDO_VERUM (concentus_apertum(k));
    CREDO_AEQUALIS_I32 (f.clausa, ZEPHYRUM);
    _currere(k, &f, MD);
    CREDO_FALSUM (concentus_apertum(k));
    CREDO_FALSUM (f.apertum);
    CREDO_AEQUALIS_I32 (f.clausa, I);
    CREDO_VERUM (concentus_aperire(k));
    CREDO_AEQUALIS_I32 (f.aperta, II);
    /* quies ante vocem non numeratur: vox horologium reponit */
    _currere(k, &f, (s64)CONCENTUS_QUIES / II);
    (vacuum)_sonitum_incipere(k, I);
    _currere(k, &f, D);
    _currere(k, &f, (s64)CONCENTUS_QUIES - M);
    CREDO_VERUM (concentus_apertum(k));
    _currere(k, &f, MD);
    CREDO_FALSUM (concentus_apertum(k));
    CREDO_VERUM (concentus_aperire(k));
    /* apertum sine voce ulla: quies ab apertione numeratur */
    _currere(k, &f, (s64)CONCENTUS_QUIES + C);
    CREDO_FALSUM (concentus_apertum(k));
}


    /* ==================================================
     * QUIESCERE: statim cum nulla vox; vox aliena tenet
     * ================================================== */

interior vacuum
_sectio_quiescere (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);
        MixturaAnsa  a;
      MixturaIussum  i;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    /* nulla vox: agere proximo clauditur */
    concentus_quiescere(k);
    _currere(k, &f, V);
    CREDO_FALSUM (concentus_apertum(k));
    CREDO_AEQUALIS_I32 (f.clausa, I);

    /* vox tibiae (alius cliens) viva: apertum manet */
    CREDO_VERUM (concentus_aperire(k));
    a = mixtura_rector_capere(concentus_rector(k), II);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TIBIAM,
        concentus_initium(k));
    i.ansa   = a;
    i.tibia  = *tibia_praeparatum("aether");
    i.nota   = 69.0f;
    CREDO_VERUM (concentus_iubere(k, &i));
    concentus_quiescere(k);
    _currere(k, &f, M);
    CREDO_VERUM (concentus_apertum(k));
    CREDO_VERUM (concentus_vox_vivit(k, a));
    /* porta clausa: post remissionem (0.25 s) statim, non post XXX s */
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_PORTAM_CLAUDERE,
                          concentus_initium(k));
    i.ansa = a;
    CREDO_VERUM (concentus_iubere(k, &i));
    _currere(k, &f, D);
    CREDO_FALSUM (concentus_vox_vivit(k, a));
    CREDO_FALSUM (concentus_apertum(k));
    CREDO_AEQUALIS_I32 (f.clausa, II);

    /* aperire (iam aperto) quiescere petitum abicit: post finem vocis
     * apertum manet usque ad quietem */
    CREDO_VERUM (concentus_aperire(k));
    (vacuum)_sonitum_incipere(k, I);
    concentus_quiescere(k);
    CREDO_VERUM (concentus_aperire(k));
    _currere(k, &f, D);
    CREDO_VERUM (concentus_apertum(k));
    CREDO_AEQUALIS_I32 (f.clausa, II);
}


    /* ==================================================
     * PRIORITAS: vox altior a cantilena non surripitur
     * ================================================== */

interior vacuum
_sectio_prioritas (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, IV);
        MixturaAnsa  a;
      MixturaIussum  i;
                i32  n;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    a = mixtura_rector_capere(concentus_rector(k), II);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TIBIAM,
        concentus_initium(k));
    i.ansa   = a;
    i.tibia  = *tibia_praeparatum("aether");
    i.nota   = 69.0f;
    CREDO_VERUM (concentus_iubere(k, &i));
    per (n = ZEPHYRUM; n < X; n++)
    {
        (vacuum)_sonitum_incipere(k, I);
    }
    _currere(k, &f, C);
    CREDO_VERUM (concentus_vox_vivit(k, a));
    CREDO_AEQUALIS_I32 (mixtura_rector_occupatae(concentus_rector(k)),
                        IV);
}


    /* ==================================================
     * DESTRUERE: statim claudit
     * ================================================== */

interior vacuum
_sectio_destruere (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);
          Concentus* clausus;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    (vacuum)_sonitum_incipere(k, I);
    concentus_destruere(k);
    CREDO_AEQUALIS_I32 (f.clausa, I);
    CREDO_FALSUM (f.apertum);
    /* clausus: nihil clauditur */
    clausus = _parare(c, &f, &exitus, VIII);
    CREDO_NECESSE (clausus != NIHIL);
    concentus_destruere(clausus);
    CREDO_AEQUALIS_I32 (f.clausa, ZEPHYRUM);
}

/* cauda reverberationis: vox finita sed cauda sonat - agere VERUM
 * (exitus apertus, offline reddit); deinde tacet - FALSUM */
interior vacuum
_sectio_cauda (
    CredoContextus* c)
{
       ExitusFictus  f;
    ConcentusExitus  exitus;
          Concentus* k = _parare(c, &f, &exitus, VIII);
        MixturaAnsa  a;
      MixturaIussum  i;
                i32  n;

    CREDO_NECESSE (k != NIHIL && concentus_aperire(k));
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_REVERBERATIONEM_PONERE,
                          concentus_initium(k));
    i.lucrum                  = 1.0f;
    i.reverberatio.aula       = 0.9f;
    i.reverberatio.mollities  = 0.3f;
    i.reverberatio.latitudo   = 1.0f;
    (vacuum)concentus_iubere(k, &i);
    a = mixtura_rector_capere(concentus_rector(k), I);
    CREDO_NECESSE (!mixtura_ansa_nulla(a));
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM,
                          concentus_initium(k));
    i.ansa       = a;
    i.parametra  = *sonitus_praeparatum("radius");
    i.semen      = I;
    i.missio     = 1.0f;
    (vacuum)concentus_iubere(k, &i);
    _currere(k, &f, D);
    CREDO_FALSUM (concentus_vox_vivit(k, a));
    CREDO_VERUM (concentus_agere(k, _nunc(&f)));
    per (n = ZEPHYRUM; n < LX && concentus_agere(k, _nunc(&f)); n++)
    {
        _currere(k, &f, D);
    }
    CREDO_VERUM (n < LX);
    CREDO_FALSUM (concentus_agere(k, _nunc(&f)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio", _sectio_creatio, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "aperire", _sectio_aperire, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "vox", _sectio_vox, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "recusatum", _sectio_recusatum, _parare_agmen, NIHIL, NIHIL,
        NIHIL },
    { "quies", _sectio_quies, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "quiescere", _sectio_quiescere, _parare_agmen, NIHIL, NIHIL,
        NIHIL },
    { "prioritas", _sectio_prioritas, _parare_agmen, NIHIL, NIHIL,
        NIHIL },
    { "destruere", _sectio_destruere, _parare_agmen, NIHIL, NIHIL,
        NIHIL },
    { "cauda", _sectio_cauda, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("concentus", SECTIONES);
}
