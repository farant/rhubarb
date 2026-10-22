/* vicus_sonus_porta.c - porta soni vera: tintinnabulum super auditus */

#include "vicus_sonus_porta.h"
#include "tintinnabulum.h"
#include "cantuale.h"
#include "auditus.h"
#include "fenestra_tempus.h"
#include <stdio.h>
#include <string.h>

/* capacitates anulorum auditus: iussa >= sedes cantilenae (XXXII) et
 * SISTERE omnium; nuntii >= II x voces (auditus.h) */
#define PORTA_ANULI LXIV

nomen structura {
          Piscina* anuli;   /* per aperitionem; NIHIL clauso exitu */
    Tintinnabulum* t;
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
    (vacuum)tintinnabulum_agere(((PortaSoni*)ctx)->t,
        fenestra_tempus_ms());
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

interior vacuum
porta_tacere (
    vacuum* ctx)
{
    tintinnabulum_tacere(((PortaSoni*)ctx)->t);
}

b32
vicus_sonus_porta (
    VicusPortaSoni* porta,
           Piscina* piscina,
               i64  semen)
{
               PortaSoni* p;
                Cantuale* liber;
      TintinnabuliExitus* exitus;
          CantualeVitium  vitium;

    si (!porta || !piscina)
    {
        redde FALSUM;
    }
    /* liber et exitus vivunt dum tintinnabulum: in piscina vocantis */
    p = (PortaSoni*)piscina_allocare(piscina,
        magnitudo(PortaSoni));
    liber = (Cantuale*)piscina_allocare(piscina, magnitudo(Cantuale));
    exitus  = (TintinnabuliExitus*)piscina_allocare(piscina,
        magnitudo(TintinnabuliExitus));
    si (!p || !liber || !exitus)
    {
        redde FALSUM;
    }
    memset(&vitium, ZEPHYRUM, magnitudo(CantualeVitium));
    si (!cantuale_ordinarium(piscina, liber, &vitium))
    {
        fprintf(stderr, "vicus: cantuale: %.*s: %s\n",
            (int)vitium.titulus.mensura,
            (constans character*)vitium.titulus.datum,
            vitium.causa ? vitium.causa : "memoria");
        redde FALSUM;
    }
    p->anuli = NIHIL;
    exitus->aperire = exitum_aperire;
    exitus->claudere = exitum_claudere;
    exitus->momentum = exitus_momentum;
    exitus->mora = exitus_mora;
    exitus->iubere = exitum_iubere;
    exitus->nuntium_legere = exitus_nuntium_legere;
    exitus->ctx = p;
    p->t = tintinnabulum_creare(piscina, liber, exitus, semen);
    si (!p->t)
    {
        redde FALSUM;
    }
    porta->pulsare  = porta_pulsare;
    porta->agere    = porta_agere;
    porta->tacere   = porta_tacere;
    porta->canere   = porta_canere;
    porta->ctx      = p;
    redde VERUM;
}
