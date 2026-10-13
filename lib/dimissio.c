/* dimissio.c - systema dimissionis (include/dimissio.h). */

#include "dimissio.h"

#include <stdio.h>
#include <string.h>

/* ramus registratus cum campis suis aspectus popover */
nomen structura {
    InsulaRamus  ramus;
         chorda  spatium;     /* = via rami (spatium componentium) */
            Xar* campi;       /* Xar de constans OrganumCampus* */
} RamusDimissionis;

structura Dimissio {
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                    Xar* rami;   /* Xar de RamusDimissionis */
};

hic_manens constans character scriptor_dimissionis[] = "dimissio";


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
apertus (
    constans RamusDimissionis* rd,
       constans OrganumCampus* k)
{
    chorda* a;

    a = insula_ramus_attributum(&rd->ramus,
        chorda_ut_cstr(k->titulus, rd->ramus.repo->piscina));
    redde a && a->mensura > ZEPHYRUM;
}

interior vacuum
tollere_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)p;
    (vacuum)in;
    (vacuum)insula_attributum_tollere(radix, (constans character*)ctx);
}

/* campum claudere scriptore 'dimissio' (scriptor prior restituitur) */
interior b32
claudere (
                     Dimissio* dim,
    constans RamusDimissionis* rd,
       constans OrganumCampus* k)
{
    chorda prior;
       b32 bene;

    prior = dim->repo->scriptor;
    insula_scriptorem_ponere(dim->repo, chorda_ex_literis(
        scriptor_dimissionis, dim->piscina));
    bene = mutare_ramum(&rd->ramus, tollere_mutator,
        (vacuum*)chorda_ut_cstr(k->titulus, dim->piscina));
    insula_scriptorem_ponere(dim->repo, prior);
    redde bene;
}

/* popover omnes aperti rami spatii dati clauduntur; VERUM si quis */
interior b32
spatium_claudere (
    Dimissio* dim,
      chorda  spatium)
{
    i32 i;
    i32 j;
    b32 clausum;

    clausum = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(dim->rami); i++)
    {
        RamusDimissionis* rd;

        rd = (RamusDimissionis*)xar_obtinere(dim->rami, i);
        si (!chorda_aequalis(rd->spatium, spatium))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(rd->campi); j++)
        {
            constans OrganumCampus* k;

            k = *(constans OrganumCampus**)xar_obtinere(rd->campi, j);
            si (apertus(rd, k) && claudere(dim, rd, k))
            {
                clausum = VERUM;
            }
        }
    }
    redde clausum;
}

/* ascensus componentem habet in spatio dato: DIALOGUS (intus) aut
 * actionem datam (aperiens) */
interior b32
ascensus_continet (
     constans Destinatio* des,
                  chorda  spatium,
                     b32  dialogus,
                  chorda  actio)
{
    i32 i;

    per (i = ZEPHYRUM; des->ascensus && i < xar_numerus(des->ascensus);
         i++)
    {
        Componens* c;

        c = *(Componens**)xar_obtinere(des->ascensus, i);
        si (!chorda_aequalis(componens_spatium(c), spatium))
        {
            perge;
        }
        si (dialogus ? c->partes == PARTES_DIALOGUS
                     : chorda_aequalis(c->actio, actio))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Hami
 * ================================================== */

interior b32
ante (
                 vacuum* ctx,
            Dispensator* d,
    constans Destinatio* des,
       constans Eventus* e)
{
     Dimissio* dim;
       chorda  spatium_ictus;
          b32  consumptus;
          i32  i;
          i32  j;

    dim = (Dimissio*)ctx;
    /* Esc: popover rami activi - NISI gestus in cursu (captura): is
     * interior est (tractus intra palettam, linea pendens) et Esc
     * suum primum accipit; Esc proximus popover claudit (census XIII,
     * acervus effugii: interius primum) */
    si (   e->genus               == EVENTUS_CLAVIS_DEPRESSUS
        && e->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        si (d->motus.captura.mensura > ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde spatium_claudere(dim, d->motus.spatium_intimum);
    }
    si (e->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    spatium_ictus = (des->ascensus && xar_numerus(des->ascensus)
                     > ZEPHYRUM)
        ? componens_spatium(*(Componens**)xar_obtinere(des->ascensus,
              ZEPHYRUM))
        : d->motus.spatium_intimum;
    consumptus = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(dim->rami); i++)
    {
        RamusDimissionis* rd;

        rd = (RamusDimissionis*)xar_obtinere(dim->rami, i);
        per (j = ZEPHYRUM; j < xar_numerus(rd->campi); j++)
        {
            constans OrganumCampus* k;

            k = *(constans OrganumCampus**)xar_obtinere(rd->campi, j);
            si (!apertus(rd, k))
            {
                perge;
            }
            /* intus (DIALOGUS rami) aut aperiens (dominus): manet */
            si (   ascensus_continet(des, rd->spatium, VERUM,
                k->scriptor)
                || ascensus_continet(des, rd->spatium, FALSUM,
                       k->scriptor))
            {
                perge;
            }
            si (   claudere(dim, rd, k)
                && chorda_aequalis(spatium_ictus, rd->spatium))
            {
                consumptus = VERUM;   /* in eodem organo: deglutitur */
            }
        }
    }
    redde consumptus;
}

/* spatium efficax actoris (ut componens_spatium, sine mutatione):
 * proximum ascendens cum spatio; vacuum = hospes */
interior chorda
spatium_actoris (
    constans Componens* c)
{
    chorda vacuum_spatium;

    dum (c)
    {
        si (c->spatium.mensura > ZEPHYRUM)
        {
            redde c->spatium;
        }
        c = c->parens;
    }
    vacuum_spatium.mensura  = ZEPHYRUM;
    vacuum_spatium.datum    = NIHIL;
    redde vacuum_spatium;
}

interior vacuum
post (
                 vacuum* ctx,
            Dispensator* d,
     constans Componens* actor,
       constans Eventus* e)
{
    (vacuum)d;
    (vacuum)e;
    si (actor->claudit)
    {
        (vacuum)spatium_claudere((Dimissio*)ctx,
            spatium_actoris(actor));
    }
}


/* ==================================================
 * API
 * ================================================== */

Dimissio*
dimissio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo)
{
    Dimissio* dim;

    si (!piscina || !intern || !repo)
    {
        redde NIHIL;
    }
    dim = (Dimissio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Dimissio));
    dim->piscina  = piscina;
    dim->intern   = intern;
    dim->repo     = repo;
    dim->rami = xar_creare(piscina,
        (i32)magnitudo(RamusDimissionis));
    redde dim;
}

b32
dimissio_ramum_addere (
                Dimissio* dimissio,
    constans InsulaRamus* ramus,
        constans Organum* organum)
{
     RamusDimissionis* rd;
                  i32  i;

    si (!dimissio || !ramus || !organum)
    {
        redde FALSUM;
    }
    rd           = (RamusDimissionis*)xar_addere(dimissio->rami);
    rd->ramus    = *ramus;
    rd->spatium  = ramus->via;
    rd->campi   = xar_creare(dimissio->piscina,
        (i32)magnitudo(constans OrganumCampus*));
    per (i = ZEPHYRUM; i < organum_numerus_camporum(organum); i++)
    {
        constans OrganumCampus* k;

        k = organum_campus_index(organum, i);
        si (   chorda_aequalis_literis(k->aspectus, "popover")
            && k->sedes == ORGANUM_SEDES_INSULA)
        {
            *(constans OrganumCampus**)xar_addere(rd->campi) = k;
        }
    }
    redde VERUM;
}

vacuum
dimissio_dispensatori_ligare (
       Dimissio* dimissio,
    Dispensator* d)
{
    si (!dimissio || !d)
    {
        redde;
    }
    dispensator_medium_ponere(d, ante, post, dimissio);
}
