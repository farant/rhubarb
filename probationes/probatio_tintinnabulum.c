/* probatio_tintinnabulum.c - Probationes notificationum sonantium
 * (include/tintinnabulum.h)
 *
 * Exitus fictus: mixtura offline, horologium manuale - quadra per
 * agmen CCLVI redduntur (exitu aperto) et ms hospitis procedunt etiam
 * exitu clauso. Numerantur aperta, clausa, iussa, voces finitae et
 * surreptae; apex ostendit sonum vere redditum.
 */
#include <stdio.h>
#include <string.h>
#include "latina.h"
#include "credo.h"
#include "tintinnabulum.h"

#define AGMEN CCLVI

#define LIBER \
    "<cantuale>" \
    "<cantilena titulus=\"finis_a\" genera=\"finis\">" \
    "<textus!>t120 c4</textus></cantilena>" \
    "<cantilena titulus=\"finis_b\" genera=\"finis\">" \
    "<textus!>t120 d4</textus></cantilena>" \
    "<cantilena titulus=\"rogatio\" genera=\"permissio\">" \
    "<textus!>t120 e8</textus></cantilena>" \
    "<cantilena titulus=\"multae\" genera=\"multae\" intactus>" \
    "<textus!>@ventus t240 l64 c c c c c c c c c c c c c c c c " \
    "c c c c c c c c c c c c c c c c</textus></cantilena>" \
    "</cantuale>"

nomen structura {
     Mixtura* mixtura;
         b32  apertum;
         b32  recusare;
         i32  aperta;
         i32  clausa;
         i32  iussa;
         i32  finitae;
         i32  surreptae;
         s64  quadra;      /* horologium hospitis, quadra */
         f32  apex;        /* |exemplum| maximum redditum */
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
    (vacuum)ctx;
    redde AGMEN;
}

interior b32
_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    f->iussa++;
    redde mixtura_iubere(f->mixtura, iussum);
}

interior b32
_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    ExitusFictus* f = (ExitusFictus*)ctx;

    si (   f->mixtura == NIHIL
        || !mixtura_nuntium_legere(f->mixtura, nuntius))
    {
        redde FALSUM;
    }
    si (nuntius->genus == MIXTURA_NUNTIUS_VOX_FINITA)
    {
        f->finitae++;
        si (nuntius->causa == MIXTURA_FINIS_SURREPTA)
        {
            f->surreptae++;
        }
    }
    redde VERUM;
}

/* ms hospitis ex horologio */
interior s64
_nunc (
    constans ExitusFictus* f)
{
    redde f->quadra * (s64)M / (s64)SONUS_FREQUENTIA;
}

/* tempus 'ms' procedit per agmina: redditur si apertum, deinde agere */
interior vacuum
_currere (
     Tintinnabulum* t,
      ExitusFictus* f,
               s64  ms)
{
    s64 finis = f->quadra + ms * (s64)SONUS_FREQUENTIA / (s64)M;
    i32 k;

    dum (f->quadra < finis)
    {
        si (f->apertum && f->mixtura != NIHIL)
        {
            mixtura_reddere(f->mixtura, exitus_agminis, AGMEN);
            per (k = ZEPHYRUM; k < II * AGMEN; k++)
            {
                f32 x = exitus_agminis[k] < 0.0f ? -exitus_agminis[k]
                                                 : exitus_agminis[k];
                si (x > f->apex)
                {
                    f->apex = x;
                }
            }
        }
        f->quadra += AGMEN;
        (vacuum)tintinnabulum_agere(t, _nunc(f));
    }
}

/* tempus procedit donec non sonat (terminus 'ms'); ms consumpta */
interior s64
_donec_tacet (
     Tintinnabulum* t,
      ExitusFictus* f,
               s64  ms)
{
    s64 initium = _nunc(f);

    dum (tintinnabulum_sonat(t) && _nunc(f) - initium < ms)
    {
        _currere(t, f, V);
    }
    redde _nunc(f) - initium;
}

interior b32
_parare (
        CredoContextus* c,
              Cantuale* liber,
          ExitusFictus* f,
    TintinnabuliExitus* exitus,
        Tintinnabulum** t)
{
    CantualeVitium vitium;

    memset(f, ZEPHYRUM, magnitudo(*f));
    exitus->aperire         = _aperire;
    exitus->claudere        = _claudere;
    exitus->momentum        = _momentum;
    exitus->mora            = _mora;
    exitus->iubere          = _iubere;
    exitus->nuntium_legere  = _nuntium_legere;
    exitus->ctx             = f;
    si (!cantuale_legere(chorda_ex_literis(LIBER, c->piscina),
        c->piscina,
                         liber, &vitium))
    {
        imprimere("  liber: %s\n", vitium.causa);
        CREDO_VERUM (FALSUM);
        redde FALSUM;
    }
    *t = tintinnabulum_creare(c->piscina, liber, exitus, XLII);
    CREDO_VERUM (*t != NIHIL);
    redde *t != NIHIL;
}

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior b32
_generis (
       constans Cantio* cantio,
        CredoContextus* c,
    constans character* genus)
{
    redde cantio != NIHIL && cantio_generis_est(cantio, _ch(c, genus));
}


    /* ==================================================
     * CREARE et PULSARE: piger, statim sonat, finitur
     * ================================================== */

interior vacuum
_sectio_pulsare (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    /* creatum: exitus clausus, nihil sonat */
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_FALSUM (tintinnabulum_apertum(t));
    CREDO_VERUM (tintinnabulum_ultima(t) == NIHIL);
    CREDO_AEQUALIS_I32 (f.aperta, ZEPHYRUM);

    /* genus ignotum: nihil aperitur */
    CREDO_FALSUM (tintinnabulum_pulsare(t, _ch(c, "nullum")));
    CREDO_AEQUALIS_I32 (f.aperta, ZEPHYRUM);

    /* finis: aperit et statim incipit */
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_AEQUALIS_I32 (f.aperta, I);
    CREDO_VERUM (tintinnabulum_apertum(t));
    CREDO_VERUM (tintinnabulum_sonat(t));
    CREDO_VERUM (_generis(tintinnabulum_ultima(t), c, "finis"));
    CREDO_AEQUALIS_I32 (f.iussa, I);
    CREDO_VERUM (tintinnabulum_agere(t, _nunc(&f)));

    /* quarta ad t120 (0.5 s) + cauda: finita intra M ms */
    CREDO_MINOR_S64 (_donec_tacet(t, &f, M), (s64)M);
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_AEQUALIS_I32 (f.finitae, I);
    CREDO_VERUM (f.apex > 0.0f);
    CREDO_VERUM (tintinnabulum_apertum(t));     /* quies nondum */
    CREDO_FALSUM (tintinnabulum_agere(t, _nunc(&f)));
}


    /* ==================================================
     * EXSPECTATIO: una simul, recentissima vincit, non eadem bis
     * ================================================== */

interior vacuum
_sectio_exspectatio (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;
       constans Cantio* prima;
       constans Cantio* secunda;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    prima = tintinnabulum_ultima(t);
    /* sonante: permissio exspectat, deinde finis eam substituit */
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "permissio")));
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_VERUM (tintinnabulum_ultima(t) == prima);
    CREDO_AEQUALIS_I32 (f.iussa, I);

    /* in medio primae: nondum altera */
    _currere(t, &f, CC);
    CREDO_VERUM (tintinnabulum_ultima(t) == prima);
    CREDO_VERUM (tintinnabulum_agere(t, _nunc(&f)));

    /* prima finita: exspectata (finis, non permissio) incipit, alia
     * quam prima */
    _currere(t, &f, DC);
    secunda = tintinnabulum_ultima(t);
    CREDO_VERUM (_generis(secunda, c, "finis"));
    CREDO_VERUM (secunda != prima);
    CREDO_AEQUALIS_I32 (f.iussa, II);

    /* nihil aliud exspectabat */
    _currere(t, &f, MD);
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_VERUM (tintinnabulum_ultima(t) == secunda);
    CREDO_AEQUALIS_I32 (f.iussa, II);
    CREDO_AEQUALIS_I32 (f.finitae, II);
    CREDO_AEQUALIS_I32 (f.aperta, I);
}


    /* ==================================================
     * QUIES: exitus post TINTINNABULUM_QUIES clauditur, iterum aperitur
     * ================================================== */

interior vacuum
_sectio_quies (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "permissio")));
    (vacuum)_donec_tacet(t, &f, M);
    CREDO_FALSUM (tintinnabulum_sonat(t));

    _currere(t, &f, (s64)TINTINNABULUM_QUIES - C);
    CREDO_VERUM (tintinnabulum_apertum(t));
    CREDO_AEQUALIS_I32 (f.clausa, ZEPHYRUM);
    _currere(t, &f, CC);
    CREDO_FALSUM (tintinnabulum_apertum(t));
    CREDO_FALSUM (f.apertum);
    CREDO_AEQUALIS_I32 (f.clausa, I);

    /* clausum: pulsus iterum aperit */
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_AEQUALIS_I32 (f.aperta, II);
    CREDO_VERUM (tintinnabulum_sonat(t));
}


    /* ==================================================
     * TACERE: evanescit, exspectata abicitur, exitus mox clauditur
     * ================================================== */

interior vacuum
_sectio_tacere (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;
       constans Cantio* prima;
                   i32  iussa;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    /* quietum: nihil fit */
    tintinnabulum_tacere(t);
    CREDO_AEQUALIS_I32 (f.clausa, ZEPHYRUM);

    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    prima = tintinnabulum_ultima(t);
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "permissio")));
    _currere(t, &f, C);
    tintinnabulum_tacere(t);
    /* evanescentia brevis: intra C ms clausum, non post quietem */
    _currere(t, &f, C);
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_FALSUM (tintinnabulum_apertum(t));
    CREDO_AEQUALIS_I32 (f.clausa, I);
    CREDO_AEQUALIS_I32 (f.finitae, I);
    /* exspectata abiecta: numquam incipit */
    _currere(t, &f, MM);
    CREDO_AEQUALIS_I32 (f.aperta, I);
    CREDO_VERUM (tintinnabulum_ultima(t) == prima);

    /* ... nec post cantilenam proximam (exspectata vetus non latet) */
    iussa = f.iussa;
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    (vacuum)_donec_tacet(t, &f, M);
    _currere(t, &f, MD);
    CREDO_AEQUALIS_I32 (f.iussa, iussa + I);
    CREDO_VERUM (_generis(tintinnabulum_ultima(t), c, "finis"));
}


    /* ==================================================
     * VITANDA: XX cantilenae finis deinceps, numquam eadem bis
     * ================================================== */

interior vacuum
_sectio_vitanda (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;
       constans Cantio* prior = NIHIL;
                   i32  k;
                   b32  iterata = FALSUM;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    per (k = ZEPHYRUM; k < XX; k++)
    {
        (vacuum)tintinnabulum_pulsare(t, _ch(c, "finis"));
        si (tintinnabulum_ultima(t) == prior)
        {
            iterata = VERUM;
        }
        prior = tintinnabulum_ultima(t);
        (vacuum)_donec_tacet(t, &f, M);
    }
    CREDO_FALSUM (iterata);
    CREDO_AEQUALIS_I32 (f.iussa, XX);
}


    /* ==================================================
     * EXITUS RECUSAT: sine sono, nihil fractum
     * ================================================== */

interior vacuum
_sectio_recusat (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    f.recusare = VERUM;
    CREDO_FALSUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_FALSUM (tintinnabulum_apertum(t));
    _currere(t, &f, CC);
    CREDO_AEQUALIS_I32 (f.iussa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.clausa, ZEPHYRUM);

    /* instrumentum redit: pulsus proximus sonat */
    f.recusare = FALSUM;
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_VERUM (tintinnabulum_sonat(t));
}


    /* ==================================================
     * VOCES: XXXII notae SIMUL sonantes (ventus intactus: caudae II.V s
     * super D ms) sine surreptione
     * ================================================== */

interior vacuum
_sectio_voces (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "multae")));
    CREDO_AEQUALIS_I32 (f.iussa, CANTUALE_NOTAE_MAXIMAE);
    (vacuum)_donec_tacet(t, &f, (s64)(IV * M));
    CREDO_FALSUM (tintinnabulum_sonat(t));
    CREDO_AEQUALIS_I32 (f.finitae, CANTUALE_NOTAE_MAXIMAE);
    CREDO_AEQUALIS_I32 (f.surreptae, ZEPHYRUM);
}


    /* ==================================================
     * CANERE: cantilena data - copiatur, iudicatur, exspectat ut
     * occasio; piscina vocantis non crescit; destruere claudit
     * ================================================== */

interior b32
_textus_est (
       constans Cantio* cantio,
    constans character* textus)
{
    redde cantio != NIHIL && chorda_aequalis_literis(cantio->textus,
        textus);
}

interior vacuum
_sectio_canere (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;
        CantualeVitium  vitium;
             character  alveus[CCLVI];
                chorda  textus;
                chorda  prima;
                chorda  altera;
        memoriae_index  usus;
        memoriae_index  propria;
                   i32  iussa;
                   i32  k;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    CREDO_GRADUS ("quieta: statim, textus copiatus");
    strcpy(alveus, "t120 g4");
    textus.datum    = (i8*)alveus;
    textus.mensura  = (i32)strlen(alveus);
    CREDO_VERUM (tintinnabulum_canere(t, textus, FALSUM, &vitium));
    CREDO_AEQUALIS_I32 (f.aperta, I);
    CREDO_VERUM (tintinnabulum_sonat(t));
    CREDO_AEQUALIS_I32 (f.iussa, I);
    CREDO_VERUM (_textus_est(tintinnabulum_ultima(t), "t120 g4"));
    CREDO_VERUM (tintinnabulum_ultima(t) != NIHIL
        && tintinnabulum_ultima(t)->textus.datum != textus.datum);
    CREDO_VERUM (tintinnabulum_ultima(t) != NIHIL
        && !tintinnabulum_ultima(t)->intactus);

    CREDO_GRADUS ("fracta: vitium, nihil mutatur");
    vitium.offset = CMXCIX;
    CREDO_FALSUM (tintinnabulum_canere(t, _ch(c, "c @nullum d"), FALSUM,
                                       &vitium));
    CREDO_AEQUALIS_S32 ((s32)vitium.offset, II);
    CREDO_FALSUM (tintinnabulum_canere(t, _ch(c, "c @nullum d"), FALSUM,
                                       NIHIL));
    CREDO_AEQUALIS_I32 (f.iussa, I);
    CREDO_VERUM (_textus_est(tintinnabulum_ultima(t), "t120 g4"));

    CREDO_GRADUS ("sonans: data exspectat; alveus vocantis mutatus");
    strcpy(alveus, "t120 a4");
    textus.mensura = (i32)strlen(alveus);
    CREDO_VERUM (tintinnabulum_canere(t, textus, VERUM, &vitium));
    CREDO_AEQUALIS_I32 (f.iussa, I);
    strcpy(alveus, "t120 b4");
    _currere(t, &f, DCCC);
    CREDO_AEQUALIS_I32 (f.iussa, II);
    CREDO_VERUM (_textus_est(tintinnabulum_ultima(t), "t120 a4"));
    CREDO_VERUM (tintinnabulum_ultima(t) != NIHIL
        && tintinnabulum_ultima(t)->intactus);
    _currere(t, &f, MD);
    CREDO_FALSUM (tintinnabulum_sonat(t));

    CREDO_GRADUS ("recentissima vincit: genus post datam");
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_VERUM (tintinnabulum_canere(t, _ch(c, "t120 c4"), FALSUM,
                                      &vitium));
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "permissio")));
    _currere(t, &f, DCCC);
    CREDO_VERUM (_generis(tintinnabulum_ultima(t), c, "permissio"));
    _currere(t, &f, MD);

    CREDO_GRADUS ("recentissima vincit: data post genus");
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "finis")));
    CREDO_VERUM (tintinnabulum_pulsare(t, _ch(c, "permissio")));
    CREDO_VERUM (tintinnabulum_canere(t, _ch(c, "t120 d8"), FALSUM,
                                      &vitium));
    _currere(t, &f, DCCC);
    CREDO_VERUM (_textus_est(tintinnabulum_ultima(t), "t120 d8"));
    _currere(t, &f, MD);
    CREDO_FALSUM (tintinnabulum_sonat(t));

    CREDO_GRADUS ("limes: CC octeti licent, CCI non");
    memset(alveus, ' ', CCI);
    memcpy(alveus, "t240 c", VI);
    textus.datum    = (i8*)alveus;
    textus.mensura  = TINTINNABULUM_TEXTUS_MAXIMUS;
    CREDO_VERUM (tintinnabulum_canere(t, textus, FALSUM, &vitium));
    _currere(t, &f, MD);
    textus.mensura  = TINTINNABULUM_TEXTUS_MAXIMUS + I;
    iussa           = f.iussa;
    CREDO_FALSUM (tintinnabulum_canere(t, textus, FALSUM, &vitium));
    CREDO_AEQUALIS_I32 (f.iussa, iussa);

    CREDO_GRADUS ("L cantilenae binae: sonant, piscina non crescit");
    prima    = _ch(c, "t240 l32 c e g");
    altera   = _ch(c, "t240 l32 e g");
    iussa    = f.iussa;
    usus     = piscina_summa_usus(c->piscina);
    propria  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < L; k++)
    {
        CREDO_VERUM (tintinnabulum_canere(t, prima, FALSUM, &vitium));
        CREDO_VERUM (tintinnabulum_canere(t, altera, FALSUM, &vitium));
        _currere(t, &f, DC);
        si (k == II)
        {
            propria = tintinnabulum_memoria(t);
        }
    }
    CREDO_AEQUALIS_I32 (f.iussa, iussa + L * V);
    CREDO_AEQUALIS_I64 ((i64)piscina_summa_usus(c->piscina), (i64)usus);
    /* piscinae propriae: sedes quaeque lectionem suam solam */
    CREDO_VERUM (propria > ZEPHYRUM);
    CREDO_AEQUALIS_I64 ((i64)tintinnabulum_memoria(t), (i64)propria);

    CREDO_GRADUS ("destruere: exitus apertus clauditur");
    CREDO_VERUM (tintinnabulum_apertum(t));
    k = f.clausa;
    tintinnabulum_destruere(t);
    CREDO_AEQUALIS_I32 (f.clausa, k + I);
    CREDO_FALSUM (f.apertum);
}

/* destruere clauso: nihil clauditur; canere recusatum (exitus) */
interior vacuum
_sectio_canere_clausum (
    CredoContextus* c)
{
              Cantuale  liber;
          ExitusFictus  f;
    TintinnabuliExitus  exitus;
         Tintinnabulum* t;

    si (!_parare(c, &liber, &f, &exitus, &t))
    {
        redde;
    }
    f.recusare = VERUM;
    CREDO_FALSUM (tintinnabulum_canere(t, _ch(c, "t120 c4"), FALSUM,
                                       NIHIL));
    CREDO_FALSUM (tintinnabulum_sonat(t));
    f.recusare = FALSUM;
    CREDO_VERUM (tintinnabulum_canere(t, _ch(c, "t120 c4"), FALSUM,
                                      NIHIL));
    (vacuum)_donec_tacet(t, &f, M);
    _currere(t, &f, (s64)TINTINNABULUM_QUIES + C);
    CREDO_FALSUM (tintinnabulum_apertum(t));
    /* recusata bis: ultima (sonata) intacta - sedes non permutatae */
    f.recusare = VERUM;
    CREDO_FALSUM (tintinnabulum_canere(t, _ch(c, "t120 d4"), FALSUM,
                                       NIHIL));
    CREDO_FALSUM (tintinnabulum_canere(t, _ch(c, "t120 e4"), FALSUM,
                                       NIHIL));
    CREDO_VERUM (_textus_est(tintinnabulum_ultima(t), "t120 c4"));
    f.recusare = FALSUM;
    tintinnabulum_destruere(t);
    CREDO_AEQUALIS_I32 (f.clausa, I);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "pulsare", _sectio_pulsare, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "exspectatio", _sectio_exspectatio, _parare_agmen, NIHIL, NIHIL,
        NIHIL },
    { "quies", _sectio_quies, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "tacere", _sectio_tacere, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "vitanda", _sectio_vitanda, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "recusat", _sectio_recusat, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "voces", _sectio_voces, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "canere", _sectio_canere, _parare_agmen, NIHIL, NIHIL, NIHIL },
    { "canere_clausum", _sectio_canere_clausum, _parare_agmen, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tintinnabulum", SECTIONES);
}
