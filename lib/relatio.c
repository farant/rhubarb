/* relatio.c - systema relationum (include/relatio.h). */

#include "relatio.h"

#include <stdio.h>
#include <string.h>

nomen structura {
               chorda genus_documenti;
               chorda genus_entis;
    RelatioEnumerator enumerator;
} GenusRelationis;

/* ligamen rami: documentum et historia eius (NIHIL donec ligatum) */
nomen structura {
       chorda  titulus;
       chorda  genus_documenti;   /* ex organo */
       vacuum* documentum;
     Historia* historia;
} LigamenRami;

nomen structura {
         InsulaRamus  ramus;
    constans Organum* organum;
                 Xar* ligamina;     /* Xar de LigamenRami */
} RamusRelationis;

structura Relatio {
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                Piscina* scheda;     /* vacatur ad verificationem */
                    Xar* genera;     /* Xar de GenusRelationis */
                    Xar* rami;       /* Xar de RamusRelationis */
                    Xar* observatae; /* Xar de Historia* */
};

hic_manens constans character scriptor_relationis[] = "relatio";


/* ==================================================
 * Auxilia
 * ================================================== */

/* S8: identitas = via tota (segmentum ultimum sub parentibus diversis
 * idem esse potest) */
interior b32
rami_aequales (
    constans InsulaRamus* a,
    constans InsulaRamus* b)
{
    redde a->repo == b->repo && chorda_aequalis(a->via, b->via);
}

interior RamusRelationis*
ramum_invenire (
                 Relatio* r,
    constans InsulaRamus* ramus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(r->rami); i++)
    {
        RamusRelationis* rr;

        rr = (RamusRelationis*)xar_obtinere(r->rami, i);
        si (rami_aequales(&rr->ramus, ramus))
        {
            redde rr;
        }
    }
    redde NIHIL;
}

interior constans RelatioEnumerator*
genus_entis_invenire (
    Relatio* r,
     chorda  genus_documenti,
     chorda  genus_entis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(r->genera); i++)
    {
        GenusRelationis* g;

        g = (GenusRelationis*)xar_obtinere(r->genera, i);
        si (   chorda_aequalis(g->genus_documenti, genus_documenti)
            && chorda_aequalis(g->genus_entis, genus_entis))
        {
            redde &g->enumerator;
        }
    }
    redde NIHIL;
}

interior LigamenRami*
ligamen_invenire (
    RamusRelationis* rr,
             chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(rr->ligamina); i++)
    {
        LigamenRami* l;

        l = (LigamenRami*)xar_obtinere(rr->ligamina, i);
        si (chorda_aequalis(l->titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Reparatio
 * ================================================== */

nomen structura {
    constans character* titulus;
    constans character* valor;      /* NIHIL = tollere */
} Reparatio;

interior vacuum
reparare_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Reparatio* r;

    r = (constans Reparatio*)ctx;
    si (r->valor)
    {
        insula_attributum_ponere(radix, p, in, r->titulus, r->valor);
    }
    alioquin
    {
        (vacuum)insula_attributum_tollere(radix, r->titulus);
    }
}

/* campus unus: valor inter entia? aliter reparatur */
interior vacuum
campum_verificare (
                   Relatio* r,
           RamusRelationis* rr,
    constans OrganumCampus* k)
{
    constans RelatioEnumerator* en;
                   LigamenRami* l;
                        chorda* valor;
                        chorda  ligamen;
                        chorda  genus_entis;
                     Reparatio  rep;
                        chorda  prior;
                           i32  i;
                           i32  n;
                           i32  lineola;

    valor = insula_ramus_attributum(&rr->ramus,
        chorda_ut_cstr(k->titulus, r->scheda));
    si (!valor || k->orbatus.mensura == ZEPHYRUM)
    {
        redde;
    }
    /* referentia 'ligamen/genus' (onerator formam iam iudicavit) */
    lineola = ZEPHYRUM;
    dum (   lineola < k->referentia.mensura
         && k->referentia.datum[lineola] != '/')
    {
        lineola++;
    }
    ligamen.datum        = k->referentia.datum;
    ligamen.mensura      = lineola;
    genus_entis.datum    = k->referentia.datum + lineola + I;
    genus_entis.mensura  = k->referentia.mensura - lineola - I;
    l                    = ligamen_invenire(rr, ligamen);
    si (!l || !l->documentum)
    {
        redde;
    }
    en = genus_entis_invenire(r, l->genus_documenti, genus_entis);
    si (!en)
    {
        redde;
    }
    n = en->numerus(l->documentum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(en->id(l->documentum, i, r->scheda),
            *valor))
        {
            redde;   /* vivit */
        }
    }
    /* orbatus */
    rep.titulus  = chorda_ut_cstr(k->titulus, r->scheda);
    rep.valor    = NIHIL;
    si (chorda_aequalis_literis(k->orbatus, "summum") && n > ZEPHYRUM)
    {
        rep.valor = chorda_ut_cstr(en->id(l->documentum, n - I,
            r->scheda), r->scheda);
    }
    prior = r->repo->scriptor;
    insula_scriptorem_ponere(r->repo, chorda_ex_literis(
        scriptor_relationis, r->scheda));
    si (!mutare_ramum(&rr->ramus, reparare_mutator, &rep))
    {
        fprintf(stderr, "relatio: %s non reparatum: %.*s\n",
            rep.titulus,
            (int)insula_causa(r->repo).mensura,
            (constans character*)insula_causa(r->repo).datum);
    }
    insula_scriptorem_ponere(r->repo, prior);
}

interior vacuum
ramum_verificare (
             Relatio* r,
     RamusRelationis* rr)
{
    i32 i;

    per (i = ZEPHYRUM; i < organum_numerus_camporum(rr->organum); i++)
    {
        constans OrganumCampus* k;

        k = organum_campus_index(rr->organum, i);
        si (   k->referentia.mensura == ZEPHYRUM
            || k->sedes              != ORGANUM_SEDES_INSULA)
        {
            perge;
        }
        campum_verificare(r, rr, k);
    }
}

/* observator: rami ad hanc historiam ligati */
interior vacuum
observare (
    Historia* historia,
      vacuum* ctx)
{
    Relatio* r;
        i32  i;
        i32  j;

    r = (Relatio*)ctx;
    piscina_vacare(r->scheda);
    per (i = ZEPHYRUM; i < xar_numerus(r->rami); i++)
    {
        RamusRelationis* rr;

        rr = (RamusRelationis*)xar_obtinere(r->rami, i);
        per (j = ZEPHYRUM; j < xar_numerus(rr->ligamina); j++)
        {
            si (((LigamenRami*)xar_obtinere(rr->ligamina,
                    j))->historia == historia)
            {
                ramum_verificare(r, rr);
                frange;
            }
        }
    }
}


/* ==================================================
 * API
 * ================================================== */

Relatio*
relatio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo)
{
    Relatio* r;

    si (!piscina || !intern || !repo)
    {
        redde NIHIL;
    }
    r = (Relatio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Relatio));
    r->piscina  = piscina;
    r->intern   = intern;
    r->repo     = repo;
    r->scheda     = piscina_generare_dynamicum("relatio_scheda",
        (memoriae_index)MXXIV);
    r->genera = xar_creare(piscina,
        (i32)magnitudo(GenusRelationis));
    r->rami = xar_creare(piscina,
        (i32)magnitudo(RamusRelationis));
    r->observatae = xar_creare(piscina, (i32)magnitudo(Historia*));
    redde r->scheda ? r : NIHIL;
}

b32
relatio_genus_addere (
                 Relatio* relatio,
      constans character* genus_documenti,
      constans character* genus_entis,
       RelatioEnumerator  enumerator)
{
    GenusRelationis* g;
             chorda  gd;
             chorda  ge;

    si (   !relatio || !genus_documenti || !genus_entis
        || !enumerator.numerus || !enumerator.id)
    {
        redde FALSUM;
    }
    gd = chorda_ex_literis(genus_documenti, relatio->piscina);
    ge = chorda_ex_literis(genus_entis, relatio->piscina);
    si (genus_entis_invenire(relatio, gd, ge))
    {
        redde FALSUM;
    }
    g                   = (GenusRelationis*)xar_addere(relatio->genera);
    g->genus_documenti  = gd;
    g->genus_entis      = ge;
    g->enumerator       = enumerator;
    redde VERUM;
}

b32
relatio_ramum_addere (
                 Relatio* relatio,
    constans InsulaRamus* ramus,
        constans Organum* organum)
{
     RamusRelationis* rr;
                 i32  i;

    si (   !relatio || !ramus || !organum
        || ramum_invenire(relatio, ramus))
    {
        redde FALSUM;
    }
    rr           = (RamusRelationis*)xar_addere(relatio->rami);
    rr->ramus    = *ramus;
    rr->organum  = organum;
    rr->ligamina = xar_creare(relatio->piscina,
        (i32)magnitudo(LigamenRami));
    per (i = ZEPHYRUM; i < organum_numerus_ligaminum(organum); i++)
    {
        constans OrganumLigamen* g;
                    LigamenRami* l;

        g                   = organum_ligamen_index(organum, i);
        l                   = (LigamenRami*)xar_addere(rr->ligamina);
        l->titulus          = g->titulus;
        l->genus_documenti  = g->genus;
        l->documentum       = NIHIL;
        l->historia         = NIHIL;
    }
    redde VERUM;
}

b32
relatio_ligare (
                 Relatio* relatio,
    constans InsulaRamus* ramus,
      constans character* ligamen,
                  vacuum* documentum,
                Historia* historia)
{
     RamusRelationis* rr;
         LigamenRami* l;
                 i32  i;
                 b32  observata;

    si (!relatio || !ramus || !ligamen)
    {
        redde FALSUM;
    }
    rr = ramum_invenire(relatio, ramus);
    l  = rr ? ligamen_invenire(rr, chorda_ex_literis(ligamen,
        relatio->scheda)) : NIHIL;
    si (!l)
    {
        redde FALSUM;
    }
    l->documentum  = documentum;
    l->historia    = historia;
    observata      = FALSUM;
    per (i = ZEPHYRUM; historia && i < xar_numerus(relatio->observatae);
         i++)
    {
        si (*(Historia**)xar_obtinere(relatio->observatae, i)
            == historia)
        {
            observata = VERUM;
        }
    }
    si (historia && !observata)
    {
        *(Historia**)xar_addere(relatio->observatae) = historia;
        (vacuum)historia_observatorem_addere(historia, observare,
            relatio);
    }
    piscina_vacare(relatio->scheda);
    ramum_verificare(relatio, rr);
    redde VERUM;
}
