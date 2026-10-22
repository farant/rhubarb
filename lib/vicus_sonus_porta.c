/* vicus_sonus_porta.c - porta soni vera: concentus super auditus;
 * tintinnabulum et voces vivae (aetherophonum) super eum */

#include "vicus_sonus_porta.h"
#include "tintinnabulum.h"
#include "tibia.h"
#include "cantuale.h"
#include "auditus.h"
#include "fenestra_tempus.h"
#include <stdio.h>
#include <string.h>

/* voces concentus: cantilena plena (XXXII) et vocibus vivis
 * (aetherophonum) locus */
#define PORTA_VOCES XLVIII
/* capacitates anulorum auditus (potentiae II): iussa >= sedes
 * cantilenae et SISTERE omnium; nuntii >= II x voces (auditus.h) */
#define PORTA_ANULI CXXVIII
/* voces vivae simul (aetherophonum: una; reliquae pro lateribus
 * pluribus) - tacere (mutus) omnes claudit */
#define PORTA_VIVAE IV
/* prioritas vocis vivae: supra cantilenas (I), ne surripiatur */
#define PORTA_PRIORITAS_VIVAE II

nomen structura {
           Piscina* anuli;   /* per aperitionem; NIHIL clauso exitu */
         Concentus* concentus;   /* hospes communis: mixtura, exitus */
     Tintinnabulum* t;
       MixturaAnsa  vivae[PORTA_VIVAE];   /* generatio 0 = vacua */
} PortaSoni;


/* ==================================================
 * Exitus tintinnabuli super auditus
 * ================================================== */

/* piscina PROPRIA anulorum: tintinnabulum post quietem claudit et
 * iterum aperit - in piscina vici anuli per dies cumularentur */
interior b32
exitum_aperire (
     vacuum* ctx,
    Mixtura* mixtura)
{
       PortaSoni* p;
    AuditusForma  forma;

    p             = (PortaSoni*)ctx;
    forma.iussa   = PORTA_ANULI;
    forma.nuntii  = PORTA_ANULI;
    p->anuli = piscina_generare_dynamicum("vicus_sonus_anuli",
        XXXII * M);
    si (!p->anuli)
    {
        redde FALSUM;
    }
    si (!auditus_aperire(mixtura, &forma, p->anuli))
    {
        piscina_destruere(p->anuli);
        p->anuli = NIHIL;
        redde FALSUM;
    }
    redde VERUM;
}

/* filum soni prius exspectatur (auditus_claudere), deinde anuli
 * liberantur */
interior vacuum
exitum_claudere (
    vacuum* ctx)
{
    PortaSoni* p = (PortaSoni*)ctx;

    auditus_claudere();
    si (p->anuli)
    {
        piscina_destruere(p->anuli);
        p->anuli = NIHIL;
    }
}

interior s64
exitus_momentum (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde auditus_momentum();
}

interior i32
exitus_mora (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde auditus_mora();
}

interior b32
exitum_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    (vacuum)ctx;
    redde auditus_iubere(iussum);
}

interior b32
exitus_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    (vacuum)ctx;
    redde auditus_nuntium_legere(nuntius);
}


/* ==================================================
 * Porta vici
 * ================================================== */

interior b32
porta_pulsare (
     vacuum* ctx,
     chorda  genus)
{
    redde tintinnabulum_pulsare(((PortaSoni*)ctx)->t, genus);
}

interior vacuum
porta_agere (
    vacuum* ctx)
{
    PortaSoni* p = (PortaSoni*)ctx;

    /* hoc ordine: nuntii prius (sedes liberantur), deinde exspectata */
    (vacuum)concentus_agere(p->concentus, fenestra_tempus_ms());
    (vacuum)tintinnabulum_agere(p->t);
}

interior b32
porta_canere (
     vacuum* ctx,
     chorda  textus,
        b32  intactus)
{
    redde tintinnabulum_canere(((PortaSoni*)ctx)->t, textus, intactus,
        NIHIL);
}

/* ansa <-> i64 portae: sedes in bitis altis, generatio in imis
 * (generatio 0 = ansa nulla = vox 0) */
interior i64
vocem_signare (
    MixturaAnsa a)
{
    redde ((i64)a.sedes << XXXII) | (i64)a.generatio;
}

interior MixturaAnsa
vocem_legere (
    i64 vox)
{
    MixturaAnsa a;

    a.sedes      = (i32)(vox >> XXXII);
    a.generatio  = (i32)(vox & 0xFFFFFFFFu);
    redde a;
}

interior vacuum
portam_vocis_claudere (
      PortaSoni* p,
    MixturaAnsa  a)
{
    MixturaIussum i;

    si (!concentus_vox_vivit(p->concentus, a))
    {
        redde;
    }
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_PORTAM_CLAUDERE,
        concentus_initium(p->concentus));
    i.ansa = a;
    (vacuum)concentus_iubere(p->concentus, &i);
}

interior vacuum
porta_tacere (
    vacuum* ctx)
{
    PortaSoni* p = (PortaSoni*)ctx;
          i32  k;

    /* voces vivae prius clausae: quiescere tintinnabuli exitum post
     * remissiones claudit */
    per (k = ZEPHYRUM; k < PORTA_VIVAE; k++)
    {
        portam_vocis_claudere(p, p->vivae[k]);
        p->vivae[k].generatio = ZEPHYRUM;
    }
    tintinnabulum_tacere(p->t);
}

/* vox viva: tibia praeparata (ignota = aether), vibratus aut non */
interior i64
porta_vocem_incipere (
                 vacuum* ctx,
     constans character* tibia,
                    b32  vibratus,
                    f32  nota,
                    f32  lucrum)
{
                    PortaSoni* p = (PortaSoni*)ctx;
      constans TibiaParametra* praeparatum;
                MixturaIussum  i;
                  MixturaAnsa  a;
                          i32  k;
                          i32  locus = PORTA_VIVAE;

    per (k = ZEPHYRUM; k < PORTA_VIVAE; k++)
    {
        si (!concentus_vox_vivit(p->concentus, p->vivae[k]))
        {
            locus = k;
            frange;
        }
    }
    praeparatum = tibia_praeparatum(tibia);
    si (praeparatum == NIHIL)
    {
        praeparatum = tibia_praeparatum("aether");
    }
    si (   locus == PORTA_VIVAE || praeparatum == NIHIL
        || !concentus_aperire(p->concentus))
    {
        redde ZEPHYRUM;
    }
    a = mixtura_rector_capere(concentus_rector(p->concentus),
        PORTA_PRIORITAS_VIVAE);
    si (mixtura_ansa_nulla(a))
    {
        redde ZEPHYRUM;
    }
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TIBIAM,
        concentus_initium(p->concentus));
    i.ansa    = a;
    i.grex    = MIXTURA_GREX_MUSICA;
    i.lucrum  = lucrum;
    i.tibia   = *praeparatum;
    i.nota    = nota;
    si (!vibratus)
    {
        i.tibia.vibratus = 0.0f;
    }
    si (!concentus_iubere(p->concentus, &i))
    {
        redde ZEPHYRUM;
    }
    p->vivae[locus] = a;
    redde vocem_signare(a);
}

/* nota et lucrum cum glissando 'ms' */
interior vacuum
porta_vocem_ponere (
    vacuum* ctx,
       i64  vox,
       f32  nota,
       f32  lucrum,
       i32  ms)
{
     PortaSoni* p = (PortaSoni*)ctx;
   MixturaAnsa  a = vocem_legere(vox);
 MixturaIussum  i;
           s64  momentum;
           i32  quadra = ms * (i32)(SONUS_FREQUENTIA / M);

    si (!concentus_vox_vivit(p->concentus, a))
    {
        redde;
    }
    momentum = concentus_initium(p->concentus);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_NOTAM_PONERE, momentum);
    i.ansa    = a;
    i.nota    = nota;
    i.clivus  = quadra;
    (vacuum)concentus_iubere(p->concentus, &i);
    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE, momentum);
    i.ansa    = a;
    i.lucrum  = lucrum;
    i.clivus  = quadra;
    (vacuum)concentus_iubere(p->concentus, &i);
}

interior vacuum
porta_vocem_dimittere (
    vacuum* ctx,
       i64  vox)
{
    portam_vocis_claudere((PortaSoni*)ctx, vocem_legere(vox));
}

b32
vicus_sonus_porta (
    VicusPortaSoni* porta,
           Piscina* piscina,
               i64  semen)
{
               PortaSoni* p;
                Cantuale* liber;
         ConcentusExitus* exitus;
          CantualeVitium  vitium;
            MixturaForma  forma;

    si (!porta || !piscina)
    {
        redde FALSUM;
    }
    /* liber et exitus vivunt dum tintinnabulum: in piscina vocantis */
    p = (PortaSoni*)piscina_allocare(piscina,
        magnitudo(PortaSoni));
    liber = (Cantuale*)piscina_allocare(piscina, magnitudo(Cantuale));
    exitus  = (ConcentusExitus*)piscina_allocare(piscina,
        magnitudo(ConcentusExitus));
    si (!p || !liber || !exitus)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(PortaSoni));
    memset(&vitium, ZEPHYRUM, magnitudo(CantualeVitium));
    si (!cantuale_ordinarium(piscina, liber, &vitium))
    {
        fprintf(stderr, "vicus: cantuale: %.*s: %s\n",
            (int)vitium.titulus.mensura,
            (constans character*)vitium.titulus.datum,
            vitium.causa ? vitium.causa : "memoria");
        redde FALSUM;
    }
    p->anuli                = NIHIL;
    exitus->aperire         = exitum_aperire;
    exitus->claudere        = exitum_claudere;
    exitus->momentum        = exitus_momentum;
    exitus->mora            = exitus_mora;
    exitus->iubere          = exitum_iubere;
    exitus->nuntium_legere  = exitus_nuntium_legere;
    exitus->ctx             = p;
    forma.voces             = PORTA_VOCES;
    forma.iussa             = PORTA_ANULI;
    forma.nuntii            = PORTA_ANULI;
    p->concentus            = concentus_creare(piscina, &forma, exitus);
    p->t           = p->concentus == NIHIL ? NIHIL
        : tintinnabulum_creare(piscina, liber, p->concentus, semen);
    si (!p->t)
    {
        redde FALSUM;
    }
    porta->pulsare          = porta_pulsare;
    porta->agere            = porta_agere;
    porta->tacere           = porta_tacere;
    porta->canere           = porta_canere;
    porta->vocem_incipere   = porta_vocem_incipere;
    porta->vocem_ponere     = porta_vocem_ponere;
    porta->vocem_dimittere  = porta_vocem_dimittere;
    porta->ctx              = p;
    redde VERUM;
}
