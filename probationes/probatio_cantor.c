/* probatio_cantor.c - Probationes cantoris (include/cantor.h)
 *
 * Concentus super exitum fictum (mixtura offline, horologium manuale)
 * qui iussa omnia SERVAT: probationes quadra exacta, notas, lucra et
 * praeparata iussorum legunt. Basis = quadrum quo carmen sonare coepit
 * (concentus_initium in incipere): nota pulsu p ad basis + q(p).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "concentus.h"
#include "carmen.h"
#include "cantor.h"
#include <stdio.h>
#include <string.h>

#define AGMEN   CCLVI
#define SERVATA (II * M)
#define NOTA_QUARTAE (XXIV * M)   /* quarta ad CXX */

nomen structura {
          Mixtura* mixtura;
              b32  apertum;
              i32  numerus;
    MixturaIussum  iussa[SERVATA];
              s64  quadra;
              f32  apex;       /* |exemplum| maximum agminis ultimi */
              f32  apex_maximus;   /* ab reponendo (manu) */
} ExitusFictus;

hic_manens ExitusFictus      fictus;
hic_manens f32               agmen[II * AGMEN];
hic_manens Concentus*        concentus;
hic_manens Cantor*           cantor;
hic_manens TibiaParametra    tibia_mea;

interior b32
_aperire (
     vacuum* ctx,
    Mixtura* mixtura)
{
    (vacuum)ctx;
    fictus.mixtura = mixtura;
    fictus.apertum = VERUM;
    redde VERUM;
}

interior vacuum
_claudere (
    vacuum* ctx)
{
    (vacuum)ctx;
    fictus.apertum = FALSUM;
}

interior s64
_momentum (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde fictus.mixtura ? mixtura_momentum(fictus.mixtura) : ZEPHYRUM;
}

interior i32
_mora (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde AGMEN;
}

interior b32
_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    (vacuum)ctx;
    si (fictus.numerus < SERVATA)
    {
        fictus.iussa[fictus.numerus++] = *iussum;
    }
    redde mixtura_iubere(fictus.mixtura, iussum);
}

interior b32
_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    (vacuum)ctx;
    redde fictus.mixtura
        && mixtura_nuntium_legere(fictus.mixtura, nuntius);
}

interior vacuum
_parare (
    CredoContextus* c)
{
     ConcentusExitus exitus;
        MixturaForma forma;

    memset(&fictus, ZEPHYRUM, magnitudo(fictus));
    memset(agmen, ZEPHYRUM, magnitudo(agmen));
    memset(&tibia_mea, ZEPHYRUM, magnitudo(tibia_mea));
    exitus.aperire = _aperire;
    exitus.claudere = _claudere;
    exitus.momentum = _momentum;
    exitus.mora = _mora;
    exitus.iubere = _iubere;
    exitus.nuntium_legere = _nuntium_legere;
    exitus.ctx = NIHIL;
    forma.voces = XLVIII;
    forma.iussa = CCLVI;
    forma.nuntii = CXXVIII;
    concentus = concentus_creare(c->piscina, &forma, &exitus);
    cantor     = concentus ? cantor_creare(c->piscina, concentus, NIHIL)
                           : NIHIL;
}

/* tempus 'ms' procedit: redditur, deinde concentus, deinde cantor */
interior vacuum
_currere (
    s64 ms)
{
    s64 finis = fictus.quadra + ms * (s64)SONUS_FREQUENTIA / (s64)M;
    i32 k;

    dum (fictus.quadra < finis)
    {
        fictus.apex = 0.0f;
        si (fictus.apertum && fictus.mixtura)
        {
            mixtura_reddere(fictus.mixtura, agmen, AGMEN);
            per (k = ZEPHYRUM; k < II * AGMEN; k++)
            {
                f32 x = agmen[k] < 0.0f ? -agmen[k] : agmen[k];

                fictus.apex = x > fictus.apex ? x : fictus.apex;
            }
            si (fictus.apex > fictus.apex_maximus)
            {
                fictus.apex_maximus = fictus.apex;
            }
        }
        fictus.quadra += AGMEN;
        (vacuum)concentus_agere(concentus, fictus.quadra * (s64)M
            / (s64)SONUS_FREQUENTIA);
        (vacuum)cantor_agere(cantor);
    }
}

interior Carmen*
_carmen (
        CredoContextus* c,
    constans character* stml)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_legere(chorda_ex_literis(stml, c->piscina), c->piscina,
                        liber, &vitium) || liber->numerus < I)
    {
        imprimere("  carmen: %s\n", vitium.causa ? vitium.causa : "-");
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

/* iussum k-tum generis dati; NIHIL si nullum */
interior constans MixturaIussum*
_iussum (
    MixturaIussumGenus genus,
                   i32 k)
{
    i32 i;

    per (i = ZEPHYRUM; i < fictus.numerus; i++)
    {
        si (fictus.iussa[i].genus == genus)
        {
            si (k == ZEPHYRUM)
            {
                redde &fictus.iussa[i];
            }
            k--;
        }
    }
    redde NIHIL;
}

interior i32
_numerus (
    MixturaIussumGenus genus)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < fictus.numerus; i++)
    {
        si (fictus.iussa[i].genus == genus)
        {
            n++;
        }
    }
    redde n;
}

/* claudenda tibiae ansae datae; NIHIL si nulla */
interior constans MixturaIussum*
_porta (
    MixturaAnsa ansa)
{
    i32 i;

    per (i = ZEPHYRUM; i < fictus.numerus; i++)
    {
        si (   fictus.iussa[i].genus == MIXTURA_IUSSUM_PORTAM_CLAUDERE
            && fictus.iussa[i].ansa.sedes == ansa.sedes
            && fictus.iussa[i].ansa.generatio == ansa.generatio)
        {
            redde &fictus.iussa[i];
        }
    }
    redde NIHIL;
}

#define CARMEN_UNA                                                    \
    "<carmina><carmen titulus=\"una\" tempus=\"120\">"                \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>l4 c d e f</textus></frustum>"   \
    "</pars></carmen></carmina>"


    /* ==================================================
     * TEMPUS: quadra exacta, prospectus, mutatio temporis
     * ================================================== */

interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
           constans Carmen* k = _carmen(c, CARMEN_UNA);
    constans MixturaIussum* i;
                       s64  basis;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    CREDO_VERUM (cantor_sonat(cantor));
    CREDO_VERUM (cantor_agere(cantor));

    CREDO_GRADUS ("prospectus: nota prima sola");
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    i = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (i != NIHIL);
    CREDO_AEQUALIS_S64 (i->momentum, basis);
    CREDO_VERUM (i->nota == 60.0f);
    CREDO_VERUM (i->lucrum == 1.0f);
    CREDO_NECESSE (_porta(i->ansa) != NIHIL);
    CREDO_AEQUALIS_S64 (_porta(i->ansa)->momentum, basis
        + NOTA_QUARTAE);

    CREDO_GRADUS ("secunda una: d et e, f nondum");
    _currere(M);
    CREDO_VERUM (fictus.apex > 0.0f);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), III);
    i = _iussum(MIXTURA_IUSSUM_TIBIAM, II);
    CREDO_NECESSE (i != NIHIL);
    CREDO_AEQUALIS_S64 (i->momentum, basis + II * NOTA_QUARTAE);
    CREDO_VERUM (i->nota == 64.0f);
    _currere(MM);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), IV);

    CREDO_GRADUS ("semel: post finem tacet");
    _currere(MM);
    CREDO_FALSUM (cantor_sonat(cantor));
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), IV);
}

interior vacuum
_sectio_mutatio (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"duo\" tempus=\"120\">"
        "<tempus mensura=\"2\" valor=\"60\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>l2 c c c</textus></frustum>"
        "</pars></carmen></carmina>");
    constans MixturaIussum* i;
                       s64  basis;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MMM);
    i = _iussum(MIXTURA_IUSSUM_TIBIAM, II);
    CREDO_NECESSE (i != NIHIL);
    /* mensura II ad CXX = IV quartae = XCVI milia; dimidia ad LX par */
    CREDO_AEQUALIS_S64 (i->momentum, basis + IV * NOTA_QUARTAE);
    CREDO_NECESSE (_porta(i->ansa) != NIHIL);
    CREDO_AEQUALIS_S64 (_porta(i->ansa)->momentum,
                        basis + VIII * NOTA_QUARTAE);
}


    /* ==================================================
     * PARTES: lucrum, mutus, solus
     * ================================================== */

interior vacuum
_sectio_partes (
    CredoContextus* c)
{
    Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"tres\">"
        "<pars titulus=\"a\" instrumentum=\"aether\" lucrum=\"0.5\">"
        "<frustum mensura=\"1\"><textus!>v50 c</textus></frustum>"
        "</pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\" mutus>"
        "<frustum mensura=\"1\"><textus!>d</textus></frustum></pars>"
        "<pars titulus=\"c\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>e</textus></frustum></pars>"
        "</carmen></carmina>");
    constans MixturaIussum* i;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    (vacuum)cantor_agere(cantor);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), II);
    i = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (i != NIHIL);
    CREDO_VERUM (i->nota == 60.0f);
    CREDO_VERUM (i->lucrum == 0.25f);
    i = _iussum(MIXTURA_IUSSUM_TIBIAM, I);
    CREDO_NECESSE (i != NIHIL);
    CREDO_VERUM (i->nota == 64.0f);

    /* solus: alii tacent */
    k->partes[II].solus = VERUM;
    cantor_sistere(cantor);
    fictus.numerus = ZEPHYRUM;
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    (vacuum)cantor_agere(cantor);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM)->nota
        == 64.0f);
}


    /* ==================================================
     * REGIO: iteratio sine hiatu, regio media
     * ================================================== */

interior vacuum
_sectio_regio (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, CARMEN_UNA);
    constans Carmen* tria = _carmen(c,
        "<carmina><carmen titulus=\"tria\" tempus=\"120\">"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c1 | d1 | e1</textus>"
        "</frustum>"
        "</pars></carmen></carmina>");
    constans MixturaIussum* i;
                       s64  basis;
                       i32  n;

    CREDO_NECESSE (k != NIHIL && tria != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, VERUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere((s64)(V * M));
    /* notae VIII+: transitus secundus exacte mensura una post */
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM) >= VIII);
    per (n = ZEPHYRUM; n < VIII; n++)
    {
        i = _iussum(MIXTURA_IUSSUM_TIBIAM, (i32)n);
        CREDO_AEQUALIS_S64 (i->momentum, basis + (s64)n * NOTA_QUARTAE);
    }
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, IV)->nota == 60.0f);
    CREDO_VERUM (cantor_sonat(cantor));

    /* regio [mensura II, III): solum d, iterans */
    cantor_sistere(cantor);
    fictus.numerus = ZEPHYRUM;
    cantor_regio(cantor, VERUM, carmen_pulsus_mensurae(tria, II),
                 carmen_pulsus_mensurae(tria, III));
    CREDO_NECESSE (cantor_incipere(cantor, tria,
        carmen_pulsus_mensurae(tria, II)));
    basis = concentus_initium(concentus);
    _currere((s64)(V * M));
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM) >= III);
    per (n = ZEPHYRUM; n < III; n++)
    {
        i = _iussum(MIXTURA_IUSSUM_TIBIAM, (i32)n);
        CREDO_VERUM (i->nota == 62.0f);
        CREDO_AEQUALIS_S64 (i->momentum, basis
            + (s64)n * IV * NOTA_QUARTAE);
    }
}


    /* ==================================================
     * SISTERE et QUAERERE
     * ================================================== */

interior vacuum
_sectio_sistere (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"cito\" tempus=\"120\">"
        "<pars titulus=\"m\" instrumentum=\"quadrans\">"
        "<frustum mensura=\"1\"><textus!>l16 c d e f g a b >c</textus>"
        "</frustum></pars></carmen></carmina>");
    i32 n;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(C);
    CREDO_VERUM (fictus.apex > 0.0f);
    /* prospectus: notae nondum sonantes iam iussae */
    n = _numerus(MIXTURA_IUSSUM_TIBIAM);
    CREDO_VERUM (n >= III);
    cantor_sistere(cantor);
    CREDO_FALSUM (cantor_sonat(cantor));
    /* post evanescentiam (V ms): silentium PER TOTUM - notae iam iussae
     * sed nondum sonantes numquam sonant */
    _currere(XX);
    fictus.apex_maximus = 0.0f;
    _currere(DC);
    CREDO_VERUM (fictus.apex_maximus == 0.0f);
    _currere(M);
    CREDO_VERUM (fictus.apex == 0.0f);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), n);
}

interior vacuum
_sectio_quaerere (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"tria\" tempus=\"120\">"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c1 | d1 | e1</textus>"
        "</frustum>"
        "</pars></carmen></carmina>");
    i32 n;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(C);
    n = _numerus(MIXTURA_IUSSUM_TIBIAM);
    cantor_quaerere(cantor, carmen_pulsus_mensurae(k, III));
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_SISTERE) >= I);
    _currere(C);
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM) == n + I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, (i32)n)->nota == 64.0f);
    CREDO_VERUM (cantor_sonat(cantor));
    /* lusor ad mensuram III */
    CREDO_VERUM (cantor_pulsus(cantor) >= carmen_pulsus_mensurae(k,
        III));
}


    /* ==================================================
     * VIVUM, INSTRUMENTA, LUSOR
     * ================================================== */

interior vacuum
_sectio_vivum (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, CARMEN_UNA);

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    (vacuum)cantor_agere(cantor);
    /* nota quarta (f) nondum iussa: mutata in exemplare auditur */
    k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae[III].nota = LXXII;
    _currere(MM);
    CREDO_NECESSE (_iussum(MIXTURA_IUSSUM_TIBIAM, III) != NIHIL);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, III)->nota == 72.0f);
}


interior constans TibiaParametra*
_tibia_mea (
     vacuum* ctx,
     chorda  instrumentum)
{
    (vacuum)ctx;
    si (chorda_aequalis_literis(instrumentum, "meum"))
    {
        redde &tibia_mea;
    }
    redde NIHIL;
}

interior vacuum
_sectio_instrumenta (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"inst\" tempus=\"120\">"
        "<pars titulus=\"a\" instrumentum=\"meum\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"ignotum\">"
        "<frustum mensura=\"1\"><textus!>d</textus></frustum></pars>"
        "<pars titulus=\"p\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>@punctum e</textus></frustum>"
        "</pars></carmen></carmina>");
    CantorResolutor  r;
             Cantor* meus;

    CREDO_NECESSE (k != NIHIL && concentus != NIHIL);
    tibia_mea           = *tibia_praeparatum("quadrans");
    tibia_mea.officium  = 0.125f;
    r.tibia             = _tibia_mea;
    r.sonitus           = NIHIL;
    r.ctx               = NIHIL;
    meus                = cantor_creare(c->piscina, concentus, &r);
    CREDO_NECESSE (meus != NIHIL);
    CREDO_NECESSE (cantor_incipere(meus, k, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    /* meum per resolutorem; ignotum tacet; '@' sonitus */
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM,
        ZEPHYRUM)->tibia.officium
                 == 0.125f);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_SONITUM), I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_SONITUM,
        ZEPHYRUM)->parametra.forma
                 == sonitus_praeparatum("punctum")->forma);
}

interior vacuum
_sectio_lusor (
    CredoContextus* c)
{
     constans Carmen* k = _carmen(c, CARMEN_UNA);
                 s64  p;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_AEQUALIS_S64 (cantor_pulsus(cantor), ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(M);
    /* secunda una ad CXX = II quartae; minus latentia et margo */
    p = cantor_pulsus(cantor);
    CREDO_VERUM (p > (s64)CMLX * II - (s64)CMLX / II);
    CREDO_VERUM (p <= (s64)CMLX * II);
    CREDO_AEQUALIS_S64 (concentus_momentum(concentus), fictus.quadra);
}

/* cantilenae (prioritas I) voces musicae non surripiunt */
interior vacuum
_sectio_prioritas (
    CredoContextus* c)
{
         constans Carmen* k = _carmen(c, CARMEN_UNA);
             MixturaAnsa  a;
                     i32  n;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    (vacuum)cantor_agere(cantor);
    a = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM)->ansa;
    per (n = ZEPHYRUM; n < C; n++)
    {
        (vacuum)mixtura_rector_capere(concentus_rector(concentus), I);
    }
    CREDO_VERUM (concentus_vox_vivit(concentus, a));
}

/* regio brevissima: CANTOR_ITERATIONES per pulsum, deinde pergit
 * sine hiatu (sexagesima quarta ad CD = CDL quadra) */
interior vacuum
_sectio_brevis (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"brevis\" tempus=\"400\">"
        "<pars titulus=\"m\" instrumentum=\"quadrans\">"
        "<frustum mensura=\"1\"><textus!>c64</textus></frustum>"
        "</pars></carmen></carmina>");
    s64 basis;
    i32 n;
    i32 primus;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, VERUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    (vacuum)cantor_agere(cantor);
    primus = _numerus(MIXTURA_IUSSUM_TIBIAM);
    CREDO_AEQUALIS_I32 (primus, CANTOR_ITERATIONES + I);
    _currere(L);
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM) > primus);
    per (n = ZEPHYRUM; n < _numerus(MIXTURA_IUSSUM_TIBIAM); n++)
    {
        CREDO_AEQUALIS_S64 (_iussum(MIXTURA_IUSSUM_TIBIAM,
            (i32)n)->momentum,
                            basis + (s64)n * CDL);
    }
    /* voces finitae obliviscuntur: notae multo plures quam CANTOR_VOCES
     * (III s = CCCXX fere) */
    _currere(MMM);
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM)
        > (i32)(CANTOR_VOCES * IV));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tempus", _sectio_tempus, _parare, NIHIL, NIHIL, NIHIL },
    { "mutatio", _sectio_mutatio, _parare, NIHIL, NIHIL, NIHIL },
    { "partes", _sectio_partes, _parare, NIHIL, NIHIL, NIHIL },
    { "regio", _sectio_regio, _parare, NIHIL, NIHIL, NIHIL },
    { "sistere", _sectio_sistere, _parare, NIHIL, NIHIL, NIHIL },
    { "quaerere", _sectio_quaerere, _parare, NIHIL, NIHIL, NIHIL },
    { "vivum", _sectio_vivum, _parare, NIHIL, NIHIL, NIHIL },
    { "instrumenta", _sectio_instrumenta, _parare, NIHIL, NIHIL,
        NIHIL },
    { "lusor", _sectio_lusor, _parare, NIHIL, NIHIL, NIHIL },
    { "prioritas", _sectio_prioritas, _parare, NIHIL, NIHIL, NIHIL },
    { "brevis", _sectio_brevis, _parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cantor", SECTIONES);
}
