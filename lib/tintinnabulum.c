/* tintinnabulum.c - Notificationes sonantes (vide include/tintinnabulum.h)
 *
 * STATUS: sonat = ansa aliqua cantilenae currentis in concentu vivit
 * (concentus_vox_vivit: rector sedem liberat cum VOX_FINITA legitur).
 * Exitus, quies et nuntii concentus sunt - hic nihil eorum.
 *
 * CANTILENAE DATAE: tres sedes (CantioPropria), quaeque textum suum et
 * piscinam suam habet; munera per indices permutantur - 'sonans'
 * (ultima data sonata, quam t->ultima fortasse monstrat), 'exspectans'
 * (si exspectat_propria), 'probanda' (libera: lectio nova hic, deinde
 * permutatur). Probanda numquam sonans est, ergo lectio nova numquam
 * cantilenam sonantem delet.
 */
#include "tintinnabulum.h"
#include "sonitus.h"
#include <string.h>

/* evanescentia tacendi: V ms (sine crepitu) */
#define TINTINNABULI_EVANESCENTIA CCXL
/* genus exspectatum: octeti maximi */
#define TINTINNABULI_GENUS      XXXII
/* sedes cantilenarum datarum: sonans, exspectans, probanda */
#define TINTINNABULI_SEDES      III

nomen structura {
    Piscina* piscina;     /* NIHIL donec primum adhibetur */
         i8  textus[TINTINNABULUM_TEXTUS_MAXIMUS];
     Cantio  cantio;
} CantioPropria;

structura Tintinnabulum {
          constans Cantuale* liber;
                   Concentus* concentus;
                        Sors sors;
                         i64 semen;
                         i32 cantus;        /* cantilenae inceptae */
                 MixturaIussum iussa[CANTUALE_NOTAE_MAXIMAE];
                 MixturaAnsa ansae[CANTUALE_NOTAE_MAXIMAE];
                         i32 ansae_numerus;
                         b32 exspectat;
                          i8 genus[TINTINNABULI_GENUS];
                         i32 genus_mensura;
             constans Cantio* ultima;
                         b32 exspectat_propria;   /* data, non genus */
               CantioPropria propriae[TINTINNABULI_SEDES];
                         i32 sonans;
                         i32 exspectans;
                         i32 probanda;
};

Tintinnabulum*
tintinnabulum_creare (
              Piscina* piscina,
    constans Cantuale* liber,
            Concentus* concentus,
                  i64  semen)
{
    Tintinnabulum* t;

    si (concentus == NIHIL)
    {
        redde NIHIL;
    }
    t = (Tintinnabulum*)piscina_allocare(piscina,
        magnitudo(Tintinnabulum));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    memset(t, ZEPHYRUM, magnitudo(Tintinnabulum));
    t->liber       = liber;
    t->concentus   = concentus;
    t->semen       = semen;
    t->sonans      = ZEPHYRUM;
    t->exspectans  = I;
    t->probanda    = II;
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

/* ansa aliqua cantilenae currentis vivit */
interior b32
_sonat (
    constans Tintinnabulum* t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->ansae_numerus; i++)
    {
        si (concentus_vox_vivit(t->concentus, t->ansae[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* cantionem iubet; concentum aperit si clausus */
interior b32
_sonare (
      Tintinnabulum* t,
    constans Cantio* cantio)
{
     CantusForma forma;
    CantusVitium vitium;
             i32 i;

    si (!concentus_aperire(t->concentus))
    {
        redde FALSUM;
    }
    forma.color      = sonitus_praeparatum("tonus");
    forma.resolutor  = NIHIL;
    forma.contextus  = NIHIL;
    forma.tenere     = !cantio->intactus;
    forma.grex       = MIXTURA_GREX_INTERFACIES;
    forma.lucrum     = 1.0f;
    forma.prioritas  = I;
    forma.semen      = t->semen + (i64)t->cantus * (i64)C;
    forma.momentum   = concentus_initium(t->concentus);
    si (!cantus_iussa_parare(&cantio->cantilena, &forma,
                             concentus_rector(t->concentus), t->iussa,
                             &vitium))
    {
        redde FALSUM;   /* liber in lectione iudicatus: non attingitur */
    }
    t->cantus++;
    t->ultima         = cantio;
    t->ansae_numerus  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < cantio->cantilena.numerus; i++)
    {
        /* recusatum: concentus sedem iam liberavit - ansa eius non
         * vivit, ergo servari nihil nocet */
        si (!mixtura_ansa_nulla(t->iussa[i].ansa))
        {
            (vacuum)concentus_iubere(t->concentus, &t->iussa[i]);
            t->ansae[t->ansae_numerus++] = t->iussa[i].ansa;
        }
    }
    redde VERUM;
}

/* cantilenam generis eligit (non ultimam) et sonat */
interior b32
_incipere (
    Tintinnabulum* t,
           chorda  genus)
{
    constans Cantio* cantio;

    cantio = cantuale_eligere(t->liber, genus, &t->sors, t->ultima);
    redde cantio != NIHIL && _sonare(t, cantio);
}

/* munera sedium permutare */
interior vacuum
_permutare (
    i32* a,
    i32* b)
{
    i32 k = *a;

    *a = *b;
    *b = k;
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
    si (_sonat(t))
    {
        /* sonus redit: quiescere petitum (tacere) abicitur */
        (vacuum)concentus_aperire(t->concentus);
        per (i = ZEPHYRUM; i < genus.mensura; i++)
        {
            t->genus[i] = genus.datum[i];
        }
        t->genus_mensura      = genus.mensura;
        t->exspectat          = VERUM;
        t->exspectat_propria  = FALSUM;
        redde VERUM;
    }
    redde _incipere(t, genus);
}

b32
tintinnabulum_canere (
     Tintinnabulum* t,
            chorda  textus,
               b32  intactus,
    CantualeVitium* vitium)
{
     CantualeVitium  proprium;
      CantioPropria* p;
             chorda  copia;

    si (vitium == NIHIL)
    {
        vitium = &proprium;
    }
    vitium->titulus.datum    = NIHIL;
    vitium->titulus.mensura  = ZEPHYRUM;
    vitium->offset           = ZEPHYRUM;
    si (textus.mensura > TINTINNABULUM_TEXTUS_MAXIMUS)
    {
        vitium->offset  = TINTINNABULUM_TEXTUS_MAXIMUS;
        vitium->causa   = "textus longior quam CC octeti";
        redde FALSUM;
    }
    p = &t->propriae[t->probanda];
    si (p->piscina == NIHIL)
    {
        p->piscina = piscina_generare_dynamicum("tintinnabulum_cantio",
            II * M);
        si (p->piscina == NIHIL)
        {
            vitium->causa = "memoria deest";
            redde FALSUM;
        }
    }
    piscina_vacare(p->piscina);
    memcpy(p->textus, textus.datum, (size_t)textus.mensura);
    copia.datum    = p->textus;
    copia.mensura  = textus.mensura;
    si (!cantionem_legere(copia, intactus, p->piscina, &p->cantio,
                          vitium))
    {
        redde FALSUM;
    }
    si (_sonat(t))
    {
        (vacuum)concentus_aperire(t->concentus);
        _permutare(&t->probanda, &t->exspectans);
        t->exspectat          = VERUM;
        t->exspectat_propria  = VERUM;
        redde VERUM;
    }
    si (!concentus_aperire(t->concentus))
    {
        redde FALSUM;
    }
    _permutare(&t->probanda, &t->sonans);
    redde _sonare(t, &t->propriae[t->sonans].cantio);
}

vacuum
tintinnabulum_destruere (
    Tintinnabulum* t)
{
    i32 i;

    per (i = ZEPHYRUM; i < TINTINNABULI_SEDES; i++)
    {
        si (t->propriae[i].piscina != NIHIL)
        {
            piscina_destruere(t->propriae[i].piscina);
            t->propriae[i].piscina = NIHIL;
        }
    }
    t->exspectat = FALSUM;
}

b32
tintinnabulum_agere (
    Tintinnabulum* t)
{
    si (_sonat(t))
    {
        redde VERUM;
    }
    si (t->exspectat && t->exspectat_propria)
    {
        t->exspectat = FALSUM;
        _permutare(&t->exspectans, &t->sonans);
        si (_sonare(t, &t->propriae[t->sonans].cantio))
        {
            redde VERUM;
        }
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
    si (!concentus_apertum(t->concentus))
    {
        redde;
    }
    momentum = concentus_initium(t->concentus);
    per (k = ZEPHYRUM; k < t->ansae_numerus; k++)
    {
        si (concentus_vox_vivit(t->concentus, t->ansae[k]))
        {
            mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE, momentum);
            i.ansa    = t->ansae[k];
            i.clivus  = TINTINNABULI_EVANESCENTIA;
            (vacuum)concentus_iubere(t->concentus, &i);
        }
    }
    concentus_quiescere(t->concentus);
}

memoriae_index
tintinnabulum_memoria (
    constans Tintinnabulum* t)
{
    memoriae_index n = ZEPHYRUM;
               i32 i;

    per (i = ZEPHYRUM; i < TINTINNABULI_SEDES; i++)
    {
        si (t->propriae[i].piscina != NIHIL)
        {
            n += piscina_summa_usus(t->propriae[i].piscina);
        }
    }
    redde n;
}

b32
tintinnabulum_sonat (
    constans Tintinnabulum* t)
{
    redde _sonat(t);
}

constans Cantio*
tintinnabulum_ultima (
    constans Tintinnabulum* t)
{
    redde t->ultima;
}
