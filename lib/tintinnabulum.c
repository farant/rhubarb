/* tintinnabulum.c - Notificationes sonantes (vide include/tintinnabulum.h)
 *
 * STATUS: 'sonantes' = voces cantilenae currentis nondum finitae (ex
 * nuntiis VOX_FINITA numeratae). Quieta = sonantes 0. 'terminus' =
 * quadrum post quod voces finitae habentur etiam sine nuntio (nuntius
 * amissus exitum numquam clausum tenere non debet).
 */
#include "tintinnabulum.h"
#include "sonitus.h"
#include <string.h>

/* L ms ultra latentiam, ne nota prima tarda sit */
#define TINTINNABULI_MARGO      (II * M + CD)
/* evanescentia tacendi: V ms (sine crepitu) */
#define TINTINNABULI_EVANESCENTIA CCXL
/* genus exspectatum: octeti maximi */
#define TINTINNABULI_GENUS      XXXII

structura Tintinnabulum {
          constans Cantuale* liber;
          TintinnabuliExitus exitus;
                    Mixtura* mixtura;
              MixturaRector* rector;
                        Sors sors;
                         i64 semen;
                         i32 cantus;        /* cantilenae inceptae */
                 MixturaIussum iussa[CANTUALE_NOTAE_MAXIMAE];
                 MixturaAnsa ansae[CANTUALE_NOTAE_MAXIMAE];
                         i32 ansae_numerus;
                         b32 apertum;
                         i32 sonantes;
                         s64 terminus;      /* quadrum */
                         b32 tacens;        /* clauditur cum evanuerunt */
                         b32 exspectat;
                          i8 genus[TINTINNABULI_GENUS];
                         i32 genus_mensura;
                         s64 quies_ab;      /* ms: ex quo quieta */
             constans Cantio* ultima;
};

Tintinnabulum*
tintinnabulum_creare (
                        Piscina* piscina,
              constans Cantuale* liber,
    constans TintinnabuliExitus* exitus,
                            i64  semen)
{
    Tintinnabulum* t;
     MixturaForma  forma;

    t = (Tintinnabulum*)piscina_allocare(piscina,
        magnitudo(Tintinnabulum));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    memset(t, ZEPHYRUM, magnitudo(Tintinnabulum));
    /* iussa: sonitus et sistere per vocem; nuntii >= II x voces */
    forma.voces   = CANTUALE_NOTAE_MAXIMAE;
    forma.iussa   = II * CANTUALE_NOTAE_MAXIMAE;
    forma.nuntii  = II * CANTUALE_NOTAE_MAXIMAE;
    t->mixtura    = mixtura_creare(piscina, &forma);
    t->rector     = mixtura_rectorem_creare(piscina, forma.voces);
    si (t->mixtura == NIHIL || t->rector == NIHIL)
    {
        redde NIHIL;
    }
    t->liber   = liber;
    t->exitus  = *exitus;
    t->semen   = semen;
    sors_seminare(&t->sors, semen, ZEPHYRUM);
    redde t;
}

interior b32
_generis_exstat (
    constans Tintinnabulum* t,
                    chorda  genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->liber->numerus; i++)
    {
        si (cantio_generis_est(&t->liber->cantiones[i], genus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_claudere (
    Tintinnabulum* t)
{
    t->exitus.claudere(t->exitus.ctx);
    t->apertum  = FALSUM;
    t->tacens   = FALSUM;
}

/* cantilenam generis eligit et iubet; exitum aperit si clausus */
interior b32
_incipere (
    Tintinnabulum* t,
           chorda  genus)
{
                  CantusForma  forma;
                 CantusVitium  vitium;
              constans Cantio* cantio;
                          s64  initium;
                          s64  finis;
                          i32  i;

    si (!t->apertum)
    {
        si (!t->exitus.aperire(t->exitus.ctx, t->mixtura))
        {
            redde FALSUM;
        }
        t->apertum = VERUM;
    }
    cantio = cantuale_eligere(t->liber, genus, &t->sors, t->ultima);
    si (cantio == NIHIL)
    {
        redde FALSUM;
    }
    initium = t->exitus.momentum(t->exitus.ctx)
            + (s64)t->exitus.mora(t->exitus.ctx) + TINTINNABULI_MARGO;
    forma.color      = sonitus_praeparatum("tonus");
    forma.resolutor  = NIHIL;
    forma.contextus  = NIHIL;
    forma.tenere     = !cantio->intactus;
    forma.grex       = MIXTURA_GREX_INTERFACIES;
    forma.lucrum     = 1.0f;
    forma.prioritas  = I;
    forma.semen      = t->semen + (i64)t->cantus * (i64)C;
    forma.momentum   = initium;
    si (!cantus_iussa_parare(&cantio->cantilena, &forma, t->rector,
                             t->iussa, &vitium))
    {
        redde FALSUM;   /* liber in lectione iudicatus: non attingitur */
    }
    t->cantus++;
    t->ultima         = cantio;
    t->sonantes       = ZEPHYRUM;
    t->ansae_numerus  = ZEPHYRUM;
    finis             = initium;
    per (i = ZEPHYRUM; i < cantio->cantilena.numerus; i++)
    {
        s64 cauda;

        si (   mixtura_ansa_nulla(t->iussa[i].ansa)
            || !t->exitus.iubere(t->exitus.ctx, &t->iussa[i]))
        {
            perge;
        }
        t->ansae[t->ansae_numerus++] = t->iussa[i].ansa;
        t->sonantes++;
        cauda = t->iussa[i].momentum
              + (s64)sonitus_longitudo(&t->iussa[i].parametra,
                                       t->iussa[i].semen);
        si (cauda > finis)
        {
            finis = cauda;
        }
    }
    t->terminus = finis + (s64)SONUS_FREQUENTIA;
    redde VERUM;
}

b32
tintinnabulum_pulsare (
    Tintinnabulum* t,
           chorda  genus)
{
    i32 i;

    si (   !_generis_exstat(t, genus)
        || genus.mensura > TINTINNABULI_GENUS)
    {
        redde FALSUM;
    }
    t->tacens = FALSUM;   /* sonus redit: exspectata non abicitur */
    si (t->sonantes > ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < genus.mensura; i++)
        {
            t->genus[i] = genus.datum[i];
        }
        t->genus_mensura  = genus.mensura;
        t->exspectat      = VERUM;
        redde VERUM;
    }
    redde _incipere(t, genus);
}

b32
tintinnabulum_agere (
    Tintinnabulum* t,
              s64  nunc)
{
    MixturaNuntius n;

    si (!t->apertum)
    {
        redde FALSUM;
    }
    dum (t->exitus.nuntium_legere(t->exitus.ctx, &n))
    {
        si (   n.genus == MIXTURA_NUNTIUS_VOX_FINITA
            && t->sonantes > ZEPHYRUM)
        {
            t->sonantes--;
            si (t->sonantes == ZEPHYRUM)
            {
                t->quies_ab = nunc;
            }
        }
        mixtura_rector_tractare(t->rector, &n);
    }
    si (   t->sonantes > ZEPHYRUM
        && t->exitus.momentum(t->exitus.ctx) > t->terminus)
    {
        t->sonantes = ZEPHYRUM;   /* nuntius amissus */
        t->quies_ab = nunc;
    }
    si (t->sonantes > ZEPHYRUM)
    {
        redde VERUM;
    }
    si (t->tacens)
    {
        _claudere(t);
        redde FALSUM;
    }
    si (t->exspectat)
    {
        chorda genus;

        genus.datum    = t->genus;
        genus.mensura  = t->genus_mensura;
        t->exspectat   = FALSUM;
        si (_incipere(t, genus))
        {
            redde VERUM;
        }
    }
    si (nunc - t->quies_ab >= (s64)TINTINNABULUM_QUIES)
    {
        _claudere(t);
    }
    redde FALSUM;
}

vacuum
tintinnabulum_tacere (
    Tintinnabulum* t)
{
    MixturaIussum i;
              s64 momentum;
              i32 k;

    t->exspectat = FALSUM;
    si (!t->apertum)
    {
        redde;
    }
    si (t->sonantes == ZEPHYRUM)
    {
        _claudere(t);
        redde;
    }
    momentum = t->exitus.momentum(t->exitus.ctx)
             + (s64)t->exitus.mora(t->exitus.ctx);
    per (k = ZEPHYRUM; k < t->ansae_numerus; k++)
    {
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE, momentum);
        i.ansa    = t->ansae[k];
        i.clivus  = TINTINNABULI_EVANESCENTIA;
        (vacuum)t->exitus.iubere(t->exitus.ctx, &i);
    }
    t->tacens    = VERUM;
    t->terminus  = momentum + TINTINNABULI_EVANESCENTIA
                 + (s64)SONUS_FREQUENTIA;
}

b32
tintinnabulum_sonat (
    constans Tintinnabulum* t)
{
    redde t->sonantes > ZEPHYRUM;
}

b32
tintinnabulum_apertum (
    constans Tintinnabulum* t)
{
    redde t->apertum;
}

constans Cantio*
tintinnabulum_ultima (
    constans Tintinnabulum* t)
{
    redde t->ultima;
}
