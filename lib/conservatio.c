/* conservatio.c - systema persistentiae (include/conservatio.h). */

#include "conservatio.h"
#include "chorda_aedificator.h"
#include "stml.h"

#include <stdio.h>
#include <string.h>

/* ramus registratus: status persistens ultimus scriptus (aut
 * restitutus) in memoria propria, ne scriptura idem iteret */
nomen structura {
             InsulaRamus  ramus;
        constans Organum* organum;
                  chorda  clavis;     /* "status/<clavis>" */
                 Piscina* memoria;
                  chorda  ultimum;
} RamusConservatus;

structura Conservatio {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
     InsulaRepositorium* repo;
                Piscina* scheda;     /* vacatur ad actum quemque */
                    Xar* rami;       /* Xar de RamusConservatus */
};

/* versio 1 (ante C2): <status><durabilis/><ephemera/></status> */
hic_manens constans character* tituli_versionis_primae[II] = {
    "durabilis", "ephemera"
};


/* ==================================================
 * Status persistens rami -> textus
 * ================================================== */

/* campus servandus: persistens, in insula */
interior b32
servandus (
    constans OrganumCampus* k)
{
    redde k->vita == ORGANUM_VITA_PERSISTENS
        && k->sedes == ORGANUM_SEDES_INSULA;
}

interior b32
habet_quotam (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] == '"')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* versio 2 (C2, insula una): campi ut attributa ipsius <status> */
interior chorda
statum_scribere (
    constans RamusConservatus* rc,
                      Piscina* p)
{
    ChordaAedificator* a;
                  i32  i;

    a = chorda_aedificator_creare(p, (memoriae_index)CCLVI);
    chorda_aedificator_appendere_literis(a,
        "<status versio=\"2\" genus=\"");
    chorda_aedificator_appendere_chorda(a, organum_genus(rc->organum));
    chorda_aedificator_appendere_literis(a, "\"");
    per (i = ZEPHYRUM; i < organum_numerus_camporum(rc->organum); i++)
    {
        constans OrganumCampus* k;
                        chorda* v;

        k = organum_campus_index(rc->organum, i);
        si (!servandus(k))
        {
            perge;
        }
        v = insula_ramus_attributum(&rc->ramus,
            chorda_ut_cstr(k->titulus, p));
        si (!v || habet_quotam(*v))
        {
            /* quota duplex in attributo irrepraesentabilis */
            perge;
        }
        chorda_aedificator_appendere_literis(a, " ");
        chorda_aedificator_appendere_chorda(a, k->titulus);
        chorda_aedificator_appendere_literis(a, "=\"");
        chorda_aedificator_appendere_chorda(a, *v);
        chorda_aedificator_appendere_literis(a, "\"");
    }
    chorda_aedificator_appendere_literis(a, "/>");
    redde chorda_aedificator_finire(a);
}

interior chorda
via_status (
    chorda  clavis,
   Piscina* p)
{
    redde chorda_concatenare(chorda_ex_literis("status/", p), clavis,
        p);
}


/* ==================================================
 * Actarius: post omnem scripturam
 * ================================================== */

interior vacuum
actarius_conservationis (
     StmlNodus* radix,
        chorda  via,
        vacuum* ctx)
{
    Conservatio* c;
            i32  i;

    (vacuum)radix;
    (vacuum)via;
    c = (Conservatio*)ctx;
    piscina_vacare(c->scheda);
    per (i = ZEPHYRUM; i < xar_numerus(c->rami); i++)
    {
        RamusConservatus* rc;
                  chorda  status;

        rc      = (RamusConservatus*)xar_obtinere(c->rami, i);
        status  = statum_scribere(rc, c->scheda);
        si (chorda_aequalis(status, rc->ultimum))
        {
            perge;
        }
        si (volumen_plagulam_condere(c->volumen,
                via_status(rc->clavis, c->scheda), status,
                "conservatio") <= ZEPHYRUM)
        {
            fprintf(stderr, "conservatio: status %.*s non conditus\n",
                (int)rc->clavis.mensura,
                (constans character*)rc->clavis.datum);
            perge;
        }
        piscina_vacare(rc->memoria);
        rc->ultimum = chorda_transcribere(status, rc->memoria);
    }
}

Conservatio*
conservatio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo)
{
    Conservatio* c;

    si (!piscina || !intern || !volumen || !repo || repo->actarius)
    {
        redde NIHIL;
    }
    c = (Conservatio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Conservatio));
    c->piscina  = piscina;
    c->intern   = intern;
    c->volumen  = volumen;
    c->repo     = repo;
    c->scheda  = piscina_generare_dynamicum("conservatio_scheda",
        (memoriae_index)(IV * MXXIV));
    c->rami    = xar_creare(piscina, (i32)magnitudo(RamusConservatus));
    si (!c->scheda)
    {
        redde NIHIL;
    }
    insula_ponere_actarium(repo, actarius_conservationis, c);
    redde c;
}


/* ==================================================
 * Restitutio
 * ================================================== */

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParRestituendum;

nomen structura {
    ParRestituendum* paria;
                i32  numerus;
} Restituenda;

interior vacuum
restituere_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Restituenda* r;
                     i32  i;

    r = (constans Restituenda*)ctx;
    per (i = ZEPHYRUM; i < r->numerus; i++)
    {
        insula_attributum_ponere(radix, p, in, r->paria[i].titulus,
            r->paria[i].valor);
    }
}

/* attributa servata (nodus fusus) in ramum (per portam); numerus
 * mutatorum, aut 0 si recusatum */
interior i32
attributa_restituere (
                  Conservatio* c,
    constans RamusConservatus* rc,
                    StmlNodus* nodus,
                      Piscina* p)
{
    Restituenda r;
            i32 i;
            i32 mutati;

    si (!nodus->attributa || xar_numerus(nodus->attributa) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    r.paria   = (ParRestituendum*)piscina_allocare(p,
        (memoriae_index)(magnitudo(ParRestituendum)
        * xar_numerus(nodus->attributa)));
    r.numerus  = ZEPHYRUM;
    mutati     = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(nodus->attributa); i++)
    {
                StmlAttributum* at;
        constans OrganumCampus* k;
                        chorda* hodie;

        at  = (StmlAttributum*)xar_obtinere(nodus->attributa, i);
        k   = organum_campus(rc->organum, *at->titulus);
        si (!k || !servandus(k))
        {
            perge;   /* migratio: ignotus aut non persistens abicitur */
        }
        r.paria[r.numerus].titulus  = chorda_ut_cstr(*at->titulus, p);
        r.paria[r.numerus].valor    = chorda_ut_cstr(*at->valor, p);
        hodie = insula_ramus_attributum(&rc->ramus,
            r.paria[r.numerus].titulus);
        si (!hodie || !chorda_aequalis(*hodie, *at->valor))
        {
            mutati++;
        }
        r.numerus++;
    }
    si (r.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (!mutare_ramum(&rc->ramus, restituere_mutator, &r))
    {
        fprintf(stderr, "conservatio: status servatus %.*s "
            "recusatus: %.*s\n", (int)rc->clavis.mensura,
            (constans character*)rc->clavis.datum,
            (int)insula_causa(c->repo).mensura,
            (constans character*)insula_causa(c->repo).datum);
        redde ZEPHYRUM;
    }
    redde mutati;
}

/* attributa nodi in fusionem (insula_attributum_ponere: substituit
 * si adest); praetermittit versio et genus radicis */
interior vacuum
attributa_fundere (
               StmlNodus* fusio,
               StmlNodus* fons,
                 Piscina* p,
     InternamentumChorda* in)
{
    i32 i;

    per (i = ZEPHYRUM; fons && fons->attributa
         && i < xar_numerus(fons->attributa); i++)
    {
        StmlAttributum* at;

        at = (StmlAttributum*)xar_obtinere(fons->attributa, i);
        si (   !at->titulus || !at->valor
            || chorda_aequalis_literis(*at->titulus, "versio")
            || chorda_aequalis_literis(*at->titulus, "genus"))
        {
            perge;
        }
        (vacuum)insula_attributum_ponere(fusio, p, in,
            chorda_ut_cstr(*at->titulus, p),
            chorda_ut_cstr(*at->valor, p));
    }
}

/* status servatus -> nodus fusus attributorum; NIHIL si versio
 * ignota. versio 2: attributa radicis; versio 1 (MIGRATIO, ante C2):
 * <durabilis/> deinde <ephemera/> - ephemera vincit si idem campus */
interior StmlNodus*
statum_fundere (
               StmlNodus* radix,
                 Piscina* p,
     InternamentumChorda* in)
{
    StmlNodus* fusio;
       chorda* versio;
          i32  g;

    versio = stml_attributum_capere(radix, "versio");
    si (!versio)
    {
        redde NIHIL;
    }
    fusio = stml_elementum_creare(p, in, "status");
    si (chorda_aequalis_literis(*versio, "2"))
    {
        attributa_fundere(fusio, radix, p, in);
        redde fusio;
    }
    si (chorda_aequalis_literis(*versio, "1"))
    {
        per (g = ZEPHYRUM; g < II; g++)
        {
            attributa_fundere(fusio, stml_invenire_liberum(radix,
                tituli_versionis_primae[g]), p, in);
        }
        redde fusio;
    }
    redde NIHIL;
}

i32
conservatio_ramum_addere (
              Conservatio* conservatio,
     constans InsulaRamus* ramus,
         constans Organum* organum,
       constans character* clavis)
{
    RamusConservatus  rc;
             Piscina* p;
              chorda  fons;
                 b32  inventum;
        StmlResultus  res;
                 i32  restituti;
           StmlNodus* fusio;

    si (!conservatio || !ramus || !organum || !clavis)
    {
        redde ZEPHYRUM;
    }
    memset(&rc, ZEPHYRUM, magnitudo(rc));
    rc.ramus    = *ramus;
    rc.organum  = organum;
    rc.clavis   = chorda_ex_literis(clavis, conservatio->piscina);
    rc.memoria = piscina_generare_dynamicum("conservatio_rami",
        (memoriae_index)MXXIV);
    si (!rc.memoria)
    {
        redde ZEPHYRUM;
    }

    /* restitutio PRIMUM (ramus nondum registratus: scriptura sua
     * actarium non movet). Piscina montationis, NON scheda: scriptura
     * restitutionis actarium vocat, qui schedam vacat - arbor lecta
     * inter insulas evanesceret. */
    restituti  = ZEPHYRUM;
    p          = conservatio->piscina;
    fons = volumen_plagulam_promere(conservatio->volumen,
        via_status(rc.clavis, p), p, &inventum);
    si (inventum)
    {
        res    = stml_legere(fons, p, conservatio->intern);
        fusio  = (res.successus && res.elementum_radix)
            ? statum_fundere(res.elementum_radix, p,
            conservatio->intern)
            : NIHIL;
        si (!fusio)
        {
            fprintf(stderr, "conservatio: status servatus %s "
                "praeteritus (forma aut versio ignota)\n", clavis);
        }
        alioquin
        {
            restituti = attributa_restituere(conservatio, &rc, fusio,
                p);
        }
    }

    /* status hodiernus = ultimus: restitutio non iterum scribitur */
    piscina_vacare(conservatio->scheda);
    rc.ultimum = chorda_transcribere(statum_scribere(&rc,
        conservatio->scheda), rc.memoria);
    *(RamusConservatus*)xar_addere(conservatio->rami) = rc;
    redde restituti;
}
