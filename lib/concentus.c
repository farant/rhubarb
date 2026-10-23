/* concentus.c - Hospes soni communis (vide include/concentus.h)
 *
 * VIVA = sedes in rectore occupata: rector sedem liberat solum cum
 * VOX_FINITA legitur (agere) aut iussum incipiens recusatur
 * (concentus_iubere) - ergo nuntius numquam amissus (forma.nuntii >= II
 * x voces) et quies vera.
 */
#include "concentus.h"

/* L ms ultra latentiam, ne iussum tardum veniat */
#define CONCENTUS_MARGO (II * M + CD)

structura Concentus {
    ConcentusExitus  exitus;
            Mixtura* mixtura;
      MixturaRector* rector;
                b32  apertum;
                b32  quiescens;    /* claudere cum primum nulla vox */
                b32  quies_nota;   /* quies_ab valet */
                s64  quies_ab;     /* ms: ex quo nulla vox */
};

Concentus*
concentus_creare (
                     Piscina* piscina,
       constans MixturaForma* forma,
    constans ConcentusExitus* exitus)
{
    Concentus* c;

    si (   forma == NIHIL || exitus == NIHIL || forma->voces == ZEPHYRUM
        || forma->iussa == ZEPHYRUM
        || forma->nuntii < II * forma->voces)
    {
        redde NIHIL;
    }
    c = (Concentus*)piscina_allocare(piscina, magnitudo(Concentus));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    c->exitus      = *exitus;
    c->mixtura     = mixtura_creare(piscina, forma);
    c->rector      = mixtura_rectorem_creare(piscina, forma->voces);
    c->apertum     = FALSUM;
    c->quiescens   = FALSUM;
    c->quies_nota  = FALSUM;
    c->quies_ab    = ZEPHYRUM;
    si (c->mixtura == NIHIL || c->rector == NIHIL)
    {
        redde NIHIL;
    }
    redde c;
}

b32
concentus_aperire (
    Concentus* c)
{
    c->quiescens = FALSUM;
    si (c->apertum)
    {
        redde VERUM;
    }
    si (!c->exitus.aperire(c->exitus.ctx, c->mixtura))
    {
        redde FALSUM;
    }
    c->apertum     = VERUM;
    c->quies_nota  = FALSUM;
    redde VERUM;
}

MixturaRector*
concentus_rector (
    Concentus* c)
{
    redde c->rector;
}

s64
concentus_initium (
    Concentus* c)
{
    si (!c->apertum)
    {
        redde ZEPHYRUM;
    }
    redde c->exitus.momentum(c->exitus.ctx)
        + (s64)c->exitus.mora(c->exitus.ctx) + CONCENTUS_MARGO;
}

interior b32
_incipiens (
    MixturaIussumGenus genus)
{
    redde genus == MIXTURA_IUSSUM_TABULAM
        || genus == MIXTURA_IUSSUM_SONITUM
        || genus == MIXTURA_IUSSUM_TIBIAM;
}

s64
concentus_momentum (
    Concentus* c)
{
    redde c->apertum ? c->exitus.momentum(c->exitus.ctx) : ZEPHYRUM;
}

b32
concentus_iubere (
                   Concentus* c,
      constans MixturaIussum* iussum)
{
    si (c->apertum && c->exitus.iubere(c->exitus.ctx, iussum))
    {
        redde VERUM;
    }
    si (_incipiens(iussum->genus))
    {
        mixtura_rector_dimittere(c->rector, iussum->ansa);
    }
    redde FALSUM;
}

interior vacuum
_claudere (
    Concentus* c)
{
    c->exitus.claudere(c->exitus.ctx);
    c->apertum     = FALSUM;
    c->quiescens   = FALSUM;
    c->quies_nota  = FALSUM;
}

b32
concentus_agere (
    Concentus* c,
          s64  nunc)
{
    MixturaNuntius n;

    si (!c->apertum)
    {
        redde FALSUM;
    }
    dum (c->exitus.nuntium_legere(c->exitus.ctx, &n))
    {
        mixtura_rector_tractare(c->rector, &n);
    }
    si (mixtura_rector_occupatae(c->rector) > ZEPHYRUM)
    {
        c->quies_nota = FALSUM;
        redde VERUM;
    }
    si (c->quiescens)
    {
        _claudere(c);
        redde FALSUM;
    }
    si (!c->quies_nota)
    {
        c->quies_nota  = VERUM;
        c->quies_ab    = nunc;
    }
    si (nunc - c->quies_ab >= (s64)CONCENTUS_QUIES)
    {
        _claudere(c);
    }
    redde FALSUM;
}

b32
concentus_vox_vivit (
    constans Concentus* c,
           MixturaAnsa  ansa)
{
    redde mixtura_rector_vivit(c->rector, ansa);
}

vacuum
concentus_quiescere (
    Concentus* c)
{
    si (c->apertum)
    {
        c->quiescens = VERUM;
    }
}

b32
concentus_apertum (
    constans Concentus* c)
{
    redde c->apertum;
}

vacuum
concentus_destruere (
    Concentus* c)
{
    si (c->apertum)
    {
        _claudere(c);
    }
}
