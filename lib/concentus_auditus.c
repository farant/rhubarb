/* concentus_auditus.c - exitus concentus super auditus (vide
 * include/concentus_auditus.h) */

#include "concentus_auditus.h"
#include "auditus.h"
#include <string.h>

/* piscina anulorum per aperitionem (octeti initiales) */
#define CONCENTUS_AUDITUS_ANULI (XXXII * M)

nomen structura {
    Piscina* anuli;    /* per aperitionem; NIHIL clauso exitu */
        i32  iussa;
        i32  nuntii;
} ConcentusAuditus;

interior b32
_aperire (
     vacuum* ctx,
    Mixtura* mixtura)
{
    ConcentusAuditus* a = (ConcentusAuditus*)ctx;
        AuditusForma  forma;

    forma.iussa   = a->iussa;
    forma.nuntii  = a->nuntii;
    a->anuli = piscina_generare_dynamicum("concentus_auditus_anuli",
        CONCENTUS_AUDITUS_ANULI);
    si (a->anuli == NIHIL)
    {
        redde FALSUM;
    }
    si (!auditus_aperire(mixtura, &forma, a->anuli))
    {
        piscina_destruere(a->anuli);
        a->anuli = NIHIL;
        redde FALSUM;
    }
    redde VERUM;
}

/* filum soni prius exspectatur (auditus_claudere), deinde anuli
 * liberantur */
interior vacuum
_claudere (
    vacuum* ctx)
{
    ConcentusAuditus* a = (ConcentusAuditus*)ctx;

    auditus_claudere();
    si (a->anuli != NIHIL)
    {
        piscina_destruere(a->anuli);
        a->anuli = NIHIL;
    }
}

interior s64
_momentum (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde auditus_momentum();
}

interior i32
_mora (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde auditus_mora();
}

interior b32
_iubere (
                    vacuum* ctx,
    constans MixturaIussum* iussum)
{
    (vacuum)ctx;
    redde auditus_iubere(iussum);
}

interior b32
_nuntium_legere (
            vacuum* ctx,
    MixturaNuntius* nuntius)
{
    (vacuum)ctx;
    redde auditus_nuntium_legere(nuntius);
}

ConcentusExitus*
concentus_exitus_auditus (
                  Piscina* piscina,
    constans MixturaForma* forma)
{
    ConcentusAuditus* a;
     ConcentusExitus* e;

    si (piscina == NIHIL || forma == NIHIL)
    {
        redde NIHIL;
    }
    a = (ConcentusAuditus*)piscina_allocare(piscina,
        magnitudo(ConcentusAuditus));
    e = (ConcentusExitus*)piscina_allocare(piscina,
        magnitudo(ConcentusExitus));
    si (a == NIHIL || e == NIHIL)
    {
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(ConcentusAuditus));
    a->iussa           = forma->iussa;
    a->nuntii          = forma->nuntii;
    e->aperire         = _aperire;
    e->claudere        = _claudere;
    e->momentum        = _momentum;
    e->mora            = _mora;
    e->iubere          = _iubere;
    e->nuntium_legere  = _nuntium_legere;
    e->ctx             = a;
    redde e;
}
