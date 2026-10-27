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
#include <math.h>
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

/* instrumenta libri: ante resolutorem (etiam proprium) - carmen suum
 * sonum definit */
interior vacuum
_sectio_instrumenta_libri (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina>"
        "<instrumentum titulus=\"meum\" basis=\"quadrans\""
        " officium=\"0.125\"/>"
        "<carmen titulus=\"libri\" tempus=\"120\">"
        "<pars titulus=\"a\" instrumentum=\"meum\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");
    CantorResolutor  r;
             Cantor* meus;

    CREDO_NECESSE (k != NIHIL && concentus != NIHIL);
    /* resolutor proprius 'meum' aliter (0.9) dicit: liber vincit */
    tibia_mea           = *tibia_praeparatum("quadrans");
    tibia_mea.officium  = 0.9f;
    r.tibia             = _tibia_mea;
    r.sonitus           = NIHIL;
    r.ctx               = NIHIL;
    meus                = cantor_creare(c->piscina, concentus, &r);
    CREDO_NECESSE (meus != NIHIL);
    CREDO_NECESSE (cantor_incipere(meus, k, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM,
        ZEPHYRUM)->tibia.officium == 0.125f);
}

/* instrumentarium ordinarium sub libro: 'cembalo' (chorda clara) sine
 * definitione in libro sonat */
interior vacuum
_sectio_instrumentarium (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"domus\" tempus=\"120\">"
        "<pars titulus=\"a\" instrumentum=\"cembalo\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");
    Cantor* meus;

    CREDO_NECESSE (k != NIHIL && concentus != NIHIL);
    meus = cantor_creare(c->piscina, concentus, NIHIL);
    CREDO_NECESSE (meus != NIHIL);
    CREDO_NECESSE (cantor_incipere(meus, k, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (   _iussum(MIXTURA_IUSSUM_TIBIAM,
        ZEPHYRUM)->tibia.genus
                    == (f32)TIBIA_CHORDA
                 && _iussum(MIXTURA_IUSSUM_TIBIAM,
                    ZEPHYRUM)->tibia.claritas == 0.95f);
}

/* aula et positio: iussum reverberationis initio (carminis forma et
 * reditus; sine aula reditus 0 - aula prior tacet), notae positionem
 * et missionem partis ferunt */
interior vacuum
_sectio_aula (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"aula\" tempus=\"120\">"
        "<reverberatio aula=\"0.7\" lucrum=\"0.4\"/>"
        "<pars titulus=\"a\" instrumentum=\"aether\" positio=\"-0.5\""
        " reverberatio=\"0.3\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* sicca = _carmen(c,
        "<carmina><carmen titulus=\"sicca\" tempus=\"120\">"
        "<pars titulus=\"a\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");
    Cantor* meus;

    CREDO_NECESSE (k != NIHIL && sicca != NIHIL && concentus != NIHIL);
    meus = cantor_creare(c->piscina, concentus, NIHIL);
    CREDO_NECESSE (meus != NIHIL);
    CREDO_NECESSE (cantor_incipere(meus, k, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    CREDO_NECESSE (_numerus(MIXTURA_IUSSUM_REVERBERATIONEM_PONERE)
        == I);
    CREDO_VERUM (   _iussum(MIXTURA_IUSSUM_REVERBERATIONEM_PONERE,
                        ZEPHYRUM)->lucrum == 0.4f
                 && _iussum(MIXTURA_IUSSUM_REVERBERATIONEM_PONERE,
                        ZEPHYRUM)->reverberatio.aula == 0.7f);
    CREDO_VERUM (   _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM)->positio
                    == -0.5f
                 && _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM)->missio
                    == 0.3f);
    CREDO_NECESSE (cantor_incipere(meus, sicca, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    CREDO_NECESSE (_numerus(MIXTURA_IUSSUM_REVERBERATIONEM_PONERE)
                   == II);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_REVERBERATIONEM_PONERE,
                         I)->lucrum == 0.0f);
}

/* resolutor ordinarius publicus: tituli noti, ignoti, chorda sine
 * terminatore (praefixum), titulus longior quam tabula; resolutor ex
 * functionibus publicis = NIHIL */
interior vacuum
_sectio_ordinarius (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"ord\" tempus=\"120\">"
        "<pars titulus=\"a\" instrumentum=\"triangulum\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"ignotum\">"
        "<frustum mensura=\"1\"><textus!>d</textus></frustum></pars>"
        "<pars titulus=\"p\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>@punctum e</textus></frustum>"
        "</pars></carmen></carmina>");
          character  b[CC];
             chorda  q;
    CantorResolutor  r;
             Cantor* meus;

    CREDO_NECESSE (k != NIHIL && concentus != NIHIL);
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL,
        chorda_ex_literis("quadrans", c->piscina))
                 == tibia_praeparatum("quadrans"));
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL,
        chorda_ex_literis("triangulum", c->piscina))
                 == tibia_praeparatum("triangulum"));
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL,
        chorda_ex_literis("ignotum", c->piscina)) == NIHIL);
    /* praefixum: "quadransX" mensura VIII = quadrans */
    memcpy(b, "quadransX", X);
    q.datum    = (i8*)b;
    q.mensura  = VIII;
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL, q)
                 == tibia_praeparatum("quadrans"));
    q.mensura = ZEPHYRUM;
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL, q) == NIHIL);
    memset(b, 'a', magnitudo(b));
    q.mensura = (i32)magnitudo(b);
    CREDO_VERUM (cantor_tibia_ordinaria(NIHIL, q) == NIHIL);
    CREDO_VERUM (cantor_sonitus_ordinarius(NIHIL,
        chorda_ex_literis("punctum", c->piscina))
                 == sonitus_praeparatum("punctum"));
    CREDO_VERUM (cantor_sonitus_ordinarius(NIHIL,
        chorda_ex_literis("ignotum", c->piscina)) == NIHIL);

    r.tibia    = cantor_tibia_ordinaria;
    r.sonitus  = cantor_sonitus_ordinarius;
    r.ctx      = NIHIL;
    meus       = cantor_creare(c->piscina, concentus, &r);
    CREDO_NECESSE (meus != NIHIL);
    CREDO_NECESSE (cantor_incipere(meus, k, ZEPHYRUM));
    (vacuum)cantor_agere(meus);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM,
        ZEPHYRUM)->tibia.forma
                 == tibia_praeparatum("triangulum")->forma);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_SONITUM), I);
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

/* VOCEM_PONERE vocis (ansae) datae, ordine iussionis: k-tum aut
 * numerus (k < 0) */
interior s32
_vocem_ponere (
               MixturaAnsa   ansa,
                       s32   k,
    constans MixturaIussum** iussum)
{
    i32 i;
    s32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < fictus.numerus; i++)
    {
        constans MixturaIussum* x = &fictus.iussa[i];

        si (   x->genus          == MIXTURA_IUSSUM_VOCEM_PONERE
            && x->ansa.sedes     == ansa.sedes
            && x->ansa.generatio == ansa.generatio)
        {
            si (n == k && iussum != NIHIL)
            {
                *iussum = x;
            }
            n++;
        }
    }
    redde n;
}

#define CARMEN_CRESCENS \
    "<carmina><carmen titulus=\"d\" tempus=\"120\">" \
    "<dynamica pars=\"m\" mensura=\"1\" valor=\"p\"/>" \
    "<dynamica pars=\"m\" mensura=\"1\" valor=\"ff\" usque=\"2\"/>" \
    "<pars titulus=\"m\" instrumentum=\"aether\" positio=\"0.5\" " \
    "reverberatio=\"0.25\">" \
    "<frustum mensura=\"1\"><textus!>c1 | c1</textus></frustum>" \
    "</pars></carmen></carmina>"

/* dynamica: lucrum ad initium notae ex carmen_intensio; vox sonans
 * crescendum per octavas sequitur (VIII iussa, lucra crescentia,
 * momenta octavarum), per prospectum - non omnia ad initium; gradus
 * intra notam: iussum unum ad momentum suum, evanescentia brevis */
interior vacuum
_sectio_dynamica (
    CredoContextus* c)
{
           constans Carmen* k = _carmen(c, CARMEN_CRESCENS);
    constans MixturaIussum* prima;
    constans MixturaIussum* secunda;
    constans MixturaIussum* i;
                       s64  basis;
                       f32  prius  = 0.0f;
                       b32  bene   = VERUM;
                       i32  j;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    CREDO_VERUM (cantor_agere(cantor));
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL);
    CREDO_VERUM (fabs(prima->lucrum - 0.32f) < 1e-5f);
    /* prospectus: non omnia praeiussa */
    CREDO_VERUM (_vocem_ponere(prima->ansa, -I, NIHIL) <= II);
    _currere(MMM);
    CREDO_NECESSE (_vocem_ponere(prima->ansa, -I, NIHIL) == VIII);
    per (j = ZEPHYRUM; j < VIII; j++)
    {
        (vacuum)_vocem_ponere(prima->ansa, (s32)j, &i);
        bene = bene
            && i->momentum == basis + (s64)j * (NOTA_QUARTAE / II)
             && (s64)i->clivus == NOTA_QUARTAE / II && i->lucrum > prius
             && i->positio == 0.5f && i->missio == 0.25f;
        prius = i->lucrum;
    }
    CREDO_VERUM (bene);
    CREDO_VERUM (prius > 0.99f && prius <= 1.0f);
    secunda = _iussum(MIXTURA_IUSSUM_TIBIAM, I);
    CREDO_NECESSE (secunda != NIHIL);
    CREDO_VERUM (secunda->lucrum == 1.0f);
    CREDO_VERUM (_vocem_ponere(secunda->ansa, -I, NIHIL) == ZEPHYRUM);
}

#define CARMEN_ITERATUM \
    "<carmina><carmen titulus=\"i\" tempus=\"120\">" \
    "<dynamica pars=\"m\" mensura=\"1\" valor=\"p\"/>" \
    "<dynamica pars=\"m\" mensura=\"1\" valor=\"ff\" usque=\"2\"/>" \
    "<pars titulus=\"m\" instrumentum=\"aether\">" \
    "<frustum mensura=\"1\"><textus!>c1</textus></frustum>" \
    "</pars></carmen></carmina>"

/* iteratio: regio [0, 1920) super notam c1 crescentem - vox transitus
 * prioris (sonans ultra finem regionis) puncta transitus novi non
 * accipit: puncta eius sola 0 480 960 1440 (IV) */
interior vacuum
_sectio_dynamica_iteratio (
    CredoContextus* c)
{
           constans Carmen* k = _carmen(c, CARMEN_ITERATUM);
    constans MixturaIussum* prima;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, VERUM, ZEPHYRUM, (s64)MCMXX);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    CREDO_VERUM (cantor_agere(cantor));
    _currere(MMM);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL
        && _numerus(MIXTURA_IUSSUM_TIBIAM) >= II);
    CREDO_VERUM (_vocem_ponere(prima->ansa, -I, NIHIL) == IV);
}

#define CARMEN_GRADUS_DYN \
    "<carmina><carmen titulus=\"g\" tempus=\"120\">" \
    "<dynamica pars=\"m\" mensura=\"1\" ictus=\"3\" valor=\"pp\"/>" \
    "<pars titulus=\"m\" instrumentum=\"aether\">" \
    "<frustum mensura=\"1\"><textus!>c1</textus></frustum>" \
    "</pars></carmen></carmina>"

/* gradus intra notam sonantem: pp ad tempus III */
interior vacuum
_sectio_dynamica_gradus (
    CredoContextus* c)
{
           constans Carmen* k = _carmen(c, CARMEN_GRADUS_DYN);
    constans MixturaIussum* prima;
    constans MixturaIussum* i = NIHIL;
                       s64  basis;

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    CREDO_VERUM (cantor_agere(cantor));
    _currere(MMM);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL && prima->lucrum == 1.0f);
    CREDO_NECESSE (_vocem_ponere(prima->ansa, -I, NIHIL) == I);
    (vacuum)_vocem_ponere(prima->ansa, ZEPHYRUM, &i);
    CREDO_AEQUALIS_S64 (i->momentum, basis + II * NOTA_QUARTAE);
    CREDO_VERUM (fabs(i->lucrum - 0.2f) < 1e-6f);
    CREDO_VERUM (   i->clivus > ZEPHYRUM
                 && (s64)i->clivus < NOTA_QUARTAE / VIII);
}

#define CARMEN_SERUM                                                  \
    "<carmina><carmen titulus=\"s\" tempus=\"120\">"                  \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"19\"><textus!>c4</textus></frustum>"          \
    "</pars></carmen></carmina>"

/* cantor tenet: nota ad XXXVI s (post XXX s quietis) iubetur; post
 * finem carminis exitus XXX s deinde clauditur; sistere demittit */
interior vacuum
_sectio_tenet (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, CARMEN_SERUM);

    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(XL * M);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_VERUM (fictus.apertum);
    _currere(XXXII * M);
    CREDO_FALSUM (fictus.apertum);
    /* quaerere dum sonat: tenens non cumulatur; post finem clauditur */
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(M);
    cantor_quaerere(cantor, ZEPHYRUM);
    _currere(XL * M);
    _currere(XXXII * M);
    CREDO_FALSUM (fictus.apertum);
    /* sistere demittit: sine voce XXX s deinde clauditur */
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(M);
    CREDO_VERUM (fictus.apertum);
    cantor_sistere(cantor);
    _currere(XXXII * M);
    CREDO_FALSUM (fictus.apertum);
}

/* iussum generis dati ansae datae, k-tum; NIHIL si nullum */
interior constans MixturaIussum*
_iussum_ansae (
    MixturaIussumGenus genus,
           MixturaAnsa ansa,
                   i32 k)
{
    i32 i;

    per (i = ZEPHYRUM; i < fictus.numerus; i++)
    {
        si (   fictus.iussa[i].genus          == genus
            && fictus.iussa[i].ansa.sedes     == ansa.sedes
            && fictus.iussa[i].ansa.generatio == ansa.generatio)
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

#define CARMEN_ILLAPSUS(textus)                                     \
    "<carmina><carmen titulus=\"gl\" tempus=\"120\">"                 \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>" textus "</textus></frustum>"   \
    "</pars></carmen></carmina>"

/* portamentum: vox una per notas glissatas (NOTAM_PONERE ad initium
 * proximae, clivus sedecimae), porta ad finem ultimae, lucrum novum */
interior vacuum
_sectio_illapsus (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
    constans MixturaIussum* g;
                       s64  basis;

    CREDO_GRADUS ("c/ e g: duae voces, e glissatur");
    k = _carmen(c, CARMEN_ILLAPSUS("l4 c/ e g"));
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MM);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), II);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_NOTAM_PONERE), I);
    prima  = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    g      = _iussum(MIXTURA_IUSSUM_NOTAM_PONERE, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL && g != NIHIL);
    CREDO_VERUM (   g->ansa.sedes == prima->ansa.sedes
                 && g->ansa.generatio == prima->ansa.generatio);
    CREDO_VERUM (g->nota == 64.0f);
    CREDO_AEQUALIS_S64 (g->momentum, basis + NOTA_QUARTAE);
    CREDO_AEQUALIS_I32 (g->clivus, NOTA_QUARTAE / IV);
    /* porta vocis primae una, ad finem e */
    CREDO_NECESSE (_porta(prima->ansa) != NIHIL);
    CREDO_AEQUALIS_S64 (_porta(prima->ansa)->momentum,
                        basis + II * NOTA_QUARTAE);
    CREDO_VERUM (_iussum_ansae(MIXTURA_IUSSUM_PORTAM_CLAUDERE,
                               prima->ansa, I) == NIHIL);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_PORTAM_CLAUDERE), II);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, I)->nota == 67.0f);
}

/* catena et lucrum notae proximae */
interior vacuum
_sectio_illapsus_catena (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
    constans MixturaIussum* v;
                       s64  basis;

    k = _carmen(c, CARMEN_ILLAPSUS("l4 c/ e/8 v50 g"));
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MM);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_TIBIAM), I);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_NOTAM_PONERE), II);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_NOTAM_PONERE, I)->nota
        == 67.0f);
    CREDO_AEQUALIS_I32 (_iussum(MIXTURA_IUSSUM_NOTAM_PONERE, I)->clivus,
                        NOTA_QUARTAE / II);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (_porta(prima->ansa)->momentum,
                        basis + III * NOTA_QUARTAE);
    /* g v50: lucrum vocis ad dimidium ad initium suum */
    v = _iussum_ansae(MIXTURA_IUSSUM_VOCEM_PONERE, prima->ansa,
        ZEPHYRUM);
    CREDO_NECESSE (v != NIHIL);
    CREDO_AEQUALIS_S64 (v->momentum, basis + II * NOTA_QUARTAE);
    CREDO_F32_PROXIMUS (v->lucrum, 0.5f, 0.0001f);
}

/* vox numquam pendet: finis regionis, proxima '@' (sonitus), quaerere
 * in proximam (nota ut solet) */
interior vacuum
_sectio_illapsus_orba (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
                       s64  basis;
                       i32  n;

    CREDO_GRADUS ("regio ad finem c: porta ad finem c");
    k = _carmen(c, CARMEN_ILLAPSUS("l4 c/ e"));
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, (s64)CMLX);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MM);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_NOTAM_PONERE),
        ZEPHYRUM);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL && _porta(prima->ansa) != NIHIL);
    CREDO_AEQUALIS_S64 (_porta(prima->ansa)->momentum,
                        basis + NOTA_QUARTAE);

    CREDO_GRADUS ("proxima sonitus: nulla glissatio, porta ad finem c");
    cantor_sistere(cantor);
    fictus.numerus  = ZEPHYRUM;
    k               = _carmen(c, CARMEN_ILLAPSUS("l4 c/ @punctum e"));
    CREDO_NECESSE (k != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MM);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_NOTAM_PONERE),
        ZEPHYRUM);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL && _porta(prima->ansa) != NIHIL);
    CREDO_AEQUALIS_S64 (_porta(prima->ansa)->momentum,
                        basis + NOTA_QUARTAE);

    CREDO_GRADUS ("quaerere in e: e ut solet incipit");
    cantor_sistere(cantor);
    fictus.numerus  = ZEPHYRUM;
    k               = _carmen(c, CARMEN_ILLAPSUS("l4 c/ e g"));
    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    _currere(C);
    cantor_quaerere(cantor, (s64)CMLX);
    n = _numerus(MIXTURA_IUSSUM_TIBIAM);
    _currere(MM);
    CREDO_VERUM (_numerus(MIXTURA_IUSSUM_TIBIAM) == n + II);
    CREDO_VERUM (_iussum(MIXTURA_IUSSUM_TIBIAM, (i32)n)->nota == 64.0f);
    CREDO_AEQUALIS_I32 (_numerus(MIXTURA_IUSSUM_NOTAM_PONERE),
        ZEPHYRUM);
}

/* VOCEM_PONERE vocis ad momentum datum; NIHIL nullum */
interior constans MixturaIussum*
_vocem_ad (
    MixturaAnsa ansa,
            s64 momentum)
{
    constans MixturaIussum* x = NIHIL;
                       s32  n = _vocem_ponere(ansa, -I, NIHIL);
                       s32  j;

    per (j = ZEPHYRUM; j < n; j++)
    {
        (vacuum)_vocem_ponere(ansa, j, &x);
        si (x->momentum == momentum)
        {
            redde x;
        }
    }
    redde NIHIL;
}

#define CARMEN_DYN_ILLAPSUS(dyn, textus)                              \
    "<carmina><carmen titulus=\"di\" tempus=\"120\">" dyn             \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>" textus "</textus></frustum>"   \
    "</pars></carmen></carmina>"

/* dynamica et portamentum: gradus ad initium notae glissatae; gradus et
 * clivus eadem positione (saltus, deinde clivus); puncta notae prioris
 * eodem prospectu ac nota proxima */
interior vacuum
_sectio_illapsus_dynamica (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
    constans MixturaIussum* x;
                       s64  basis;

    CREDO_GRADUS ("gradus pp ad initium d glissatae");
    k = _carmen(c, CARMEN_DYN_ILLAPSUS(
        "<dynamica mensura=\"2\" valor=\"pp\"/>", "o4 c1/ | d1 | r1"));
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(IV * M);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL
        && _numerus(MIXTURA_IUSSUM_TIBIAM) == I);
    x = _vocem_ad(prima->ansa, basis + IV * NOTA_QUARTAE);
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (fabs(x->lucrum - 0.2f) < 1e-5f);
    CREDO_AEQUALIS_I32 (x->clivus, CANTOR_EVANESCENTIA);
}

/* gradus pp et crescendo eadem positione: saltus V ms, clivus post */
interior vacuum
_sectio_dynamica_saltus (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
    constans MixturaIussum* x;
                       s64  basis;

    k = _carmen(c, CARMEN_DYN_ILLAPSUS(
        "<dynamica mensura=\"2\" valor=\"pp\"/>"
        "<dynamica mensura=\"2\" valor=\"ff\" usque=\"3\"/>",
        "o4 c1 & c1 | r1"));
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(IV * M);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL);
    x = _vocem_ad(prima->ansa, basis + IV * NOTA_QUARTAE);
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (fabs(x->lucrum - 0.2f) < 1e-5f);
    CREDO_AEQUALIS_I32 (x->clivus, CANTOR_EVANESCENTIA);
    x = _vocem_ad(prima->ansa,
                  basis + IV * NOTA_QUARTAE + CANTOR_EVANESCENTIA);
    CREDO_NECESSE (x != NIHIL);
    CREDO_AEQUALIS_I32 (x->clivus,
        NOTA_QUARTAE / II - CANTOR_EVANESCENTIA);
    CREDO_VERUM (x->lucrum > 0.2f);
}

/* tempus 400: gradus pp intra notam priorem, nota glissata eodem
 * prospectu - punctum prioris non perit */
interior vacuum
_sectio_illapsus_prospectus (
    CredoContextus* c)
{
           constans Carmen* k;
    constans MixturaIussum* prima;
    constans MixturaIussum* x;
                       s64  basis;

    k = _carmen(c,
        "<carmina><carmen titulus=\"p\" tempus=\"400\" metrum=\"8/8\">"
        "<dynamica mensura=\"1\" ictus=\"2\" valor=\"pp\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 c4/ d2. | r1</textus>"
        "</frustum></pars></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL && cantor != NIHIL);
    cantor_regio(cantor, FALSUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (cantor_incipere(cantor, k, ZEPHYRUM));
    basis = concentus_initium(concentus);
    _currere(MM);
    prima = _iussum(MIXTURA_IUSSUM_TIBIAM, ZEPHYRUM);
    CREDO_NECESSE (prima != NIHIL
        && _numerus(MIXTURA_IUSSUM_TIBIAM) == I);
    /* octava ad CD: LXXV ms */
    x = _vocem_ad(prima->ansa,
                  basis + (s64)SONUS_FREQUENTIA * LXXV / (s64)M);
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (fabs(x->lucrum - 0.2f) < 1e-5f);
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
    { "instrumenta_libri", _sectio_instrumenta_libri, _parare, NIHIL,
        NIHIL,
        NIHIL },
    { "instrumentarium", _sectio_instrumentarium, _parare, NIHIL, NIHIL,
        NIHIL },
    { "aula", _sectio_aula, _parare, NIHIL, NIHIL, NIHIL },
    { "ordinarius", _sectio_ordinarius, _parare, NIHIL, NIHIL, NIHIL },
    { "lusor", _sectio_lusor, _parare, NIHIL, NIHIL, NIHIL },
    { "prioritas", _sectio_prioritas, _parare, NIHIL, NIHIL, NIHIL },
    { "brevis", _sectio_brevis, _parare, NIHIL, NIHIL, NIHIL },
    { "dynamica", _sectio_dynamica, _parare, NIHIL, NIHIL, NIHIL },
    { "dynamica_gradus", _sectio_dynamica_gradus, _parare, NIHIL, NIHIL,
        NIHIL },
    { "dynamica_iteratio", _sectio_dynamica_iteratio, _parare, NIHIL,
        NIHIL, NIHIL },
    { "tenet", _sectio_tenet, _parare, NIHIL, NIHIL, NIHIL },
    { "portamentum", _sectio_illapsus, _parare, NIHIL, NIHIL,
        NIHIL },
    { "portamentum_catena", _sectio_illapsus_catena, _parare, NIHIL,
        NIHIL, NIHIL },
    { "portamentum_dynamica", _sectio_illapsus_dynamica, _parare, NIHIL,
        NIHIL, NIHIL },
    { "dynamica_saltus", _sectio_dynamica_saltus, _parare, NIHIL, NIHIL,
        NIHIL },
    { "portamentum_prospectus", _sectio_illapsus_prospectus, _parare,
        NIHIL, NIHIL, NIHIL },
    { "portamentum_orba", _sectio_illapsus_orba, _parare, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cantor", SECTIONES);
}
