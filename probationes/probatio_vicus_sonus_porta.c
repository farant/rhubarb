/* probatio_vicus_sonus_porta.c - porta soni vera super exitum fictum
 * (gabc pars II P5): carmen in porta vici
 *
 * vicus_sonus_porta_cum_exitu: concentus et tintinnabulum veri, exitus
 * FICTUS (mixtura offline, horologium manuale, iussa servata) - nihil
 * sonat. Tempus procedit: mixtura redditur, deinde porta->agere (ut
 * vicus omni pulsu). Carmen: incipere -> pulsus crescit; quaerere
 * salit; sistere et tacere (mutus) -> -1; finis naturalis -> -1;
 * agere cantorem agit (notae post primam iubentur). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "concentus.h"
#include "carmen.h"
#include "cantus.h"
#include "vicus_sonus.h"
#include "vicus_sonus_porta.h"
#include <string.h>

#define AGMEN    CCLVI
#define SERVATA  (II * M)

#define CARMEN_SCALA                                                  \
    "<carmina><carmen titulus=\"scala\" tempus=\"120\">"              \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>l4 c d e f g a b >c</textus>"    \
    "</frustum></pars></carmen></carmina>"
#define CARMEN_BREVE                                                  \
    "<carmina><carmen titulus=\"breve\" tempus=\"120\">"              \
    "<pars titulus=\"m\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>l4 c</textus>"                   \
    "</frustum></pars></carmen></carmina>"

nomen structura {
          Mixtura* mixtura;
              b32  apertum;
              i32  numerus;
    MixturaIussum  iussa[SERVATA];
              s64  quadra;
} ExitusFictus;

hic_manens ExitusFictus     fictus;
hic_manens f32              agmen[II * AGMEN];
hic_manens ConcentusExitus  exitus;
hic_manens VicusPortaSoni   porta;

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
    memset(&fictus, ZEPHYRUM, magnitudo(fictus));
    memset(agmen, ZEPHYRUM, magnitudo(agmen));
    memset(&porta, ZEPHYRUM, magnitudo(porta));
    exitus.aperire         = _aperire;
    exitus.claudere        = _claudere;
    exitus.momentum        = _momentum;
    exitus.mora            = _mora;
    exitus.iubere          = _iubere;
    exitus.nuntium_legere  = _nuntium_legere;
    exitus.ctx             = NIHIL;
    (vacuum)vicus_sonus_porta_cum_exitu(&porta, c->piscina, (i64)VII,
        &exitus);
}

/* tempus 'ms' procedit: redditur, deinde porta agit */
interior vacuum
_currere (
    s64 ms)
{
    s64 finis;

    finis = fictus.quadra + ms * (s64)SONUS_FREQUENTIA / (s64)M;
    dum (fictus.quadra < finis)
    {
        si (fictus.apertum && fictus.mixtura)
        {
            mixtura_reddere(fictus.mixtura, agmen, AGMEN);
        }
        fictus.quadra += AGMEN;
        porta.agere(porta.ctx);
    }
}

interior constans Carmen*
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
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

interior i32
_tibiae (vacuum)
{
    i32 k;
    i32 n;

    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < fictus.numerus; k++)
    {
        si (fictus.iussa[k].genus == MIXTURA_IUSSUM_TIBIAM)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_carmen_probare (
    CredoContextus* c)
{
    constans Carmen* scala;
    constans Carmen* breve;
                s64  pulsus;

    scala = _carmen(c, CARMEN_SCALA);
    breve = _carmen(c, CARMEN_BREVE);
    CREDO_NECESSE (scala != NIHIL && breve != NIHIL);
    CREDO_NECESSE (porta.ctx != NIHIL && porta.carmen_incipere != NIHIL
        && porta.carmen_quaerere != NIHIL
        && porta.carmen_sistere != NIHIL
        && porta.carmen_pulsus != NIHIL);

    CREDO_GRADUS ("ante incipere: -1");
    CREDO_AEQUALIS_S64 (porta.carmen_pulsus(porta.ctx), -I);

    CREDO_GRADUS ("incipere: pulsus crescit; agere cantorem agit");
    CREDO_VERUM (porta.carmen_incipere(porta.ctx, scala, ZEPHYRUM));
    _currere(M);
    pulsus = porta.carmen_pulsus(porta.ctx);
    CREDO_VERUM (pulsus >= II * CANTUS_PULSUS_QUARTAE
        - CANTUS_PULSUS_QUARTAE / VIII);
    CREDO_VERUM (pulsus < II * CANTUS_PULSUS_QUARTAE + XCVI);
    CREDO_MAIOR_AUT_AEQUALIS_I32 (_tibiae(), III);

    CREDO_GRADUS ("quaerere: salit ad pulsum datum");
    porta.carmen_quaerere(porta.ctx, (s64)(V * CANTUS_PULSUS_QUARTAE));
    pulsus = porta.carmen_pulsus(porta.ctx);
    CREDO_VERUM (pulsus >= V * CANTUS_PULSUS_QUARTAE);
    CREDO_VERUM (pulsus < V * CANTUS_PULSUS_QUARTAE + XCVI);
    _currere(CCL);
    CREDO_VERUM (porta.carmen_pulsus(porta.ctx)
        > (s64)(V * CANTUS_PULSUS_QUARTAE));

    CREDO_GRADUS ("sistere: -1");
    porta.carmen_sistere(porta.ctx);
    CREDO_AEQUALIS_S64 (porta.carmen_pulsus(porta.ctx), -I);
    _currere(CCL);
    CREDO_AEQUALIS_S64 (porta.carmen_pulsus(porta.ctx), -I);

    CREDO_GRADUS ("tacere (mutus) carmen quoque sistit");
    CREDO_VERUM (porta.carmen_incipere(porta.ctx, scala, ZEPHYRUM));
    _currere(CCL);
    CREDO_VERUM (porta.carmen_pulsus(porta.ctx) >= ZEPHYRUM);
    porta.tacere(porta.ctx);
    CREDO_AEQUALIS_S64 (porta.carmen_pulsus(porta.ctx), -I);

    CREDO_GRADUS ("finis naturalis: -1");
    CREDO_VERUM (porta.carmen_incipere(porta.ctx, breve, ZEPHYRUM));
    _currere(CCL);
    CREDO_VERUM (porta.carmen_pulsus(porta.ctx) >= ZEPHYRUM);
    _currere(MM);
    CREDO_AEQUALIS_S64 (porta.carmen_pulsus(porta.ctx), -I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "carmen in porta", _carmen_probare, _parare, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_sonus_porta", SECTIONES);
}
