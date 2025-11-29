#include "cursus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Interior - Obtinere vel creare capsam
 * ================================================== */

interior Capsa*
_cursus_obtinere_vel_creare_capsam(
    CursusNuntiorum* cursus,
    chorda*          entitas_id)
{
    vacuum* valor;
    Capsa*  capsa;

    si (!cursus || !entitas_id)
    {
        redde NIHIL;
    }

    /* Quaerere capsam existentem */
    si (tabula_dispersa_invenire(cursus->capsae, *entitas_id, &valor))
    {
        redde (Capsa*)valor;
    }

    /* Creare novam capsam (allocatio pigra) */
    capsa = capsa_creare(cursus->piscina, entitas_id);
    si (!capsa)
    {
        redde NIHIL;
    }

    /* Inserere in tabulam */
    si (!tabula_dispersa_inserere(cursus->capsae, *entitas_id, capsa))
    {
        redde NIHIL;
    }

    redde capsa;
}


/* ==================================================
 * Interior - Tractare unum nuntium
 * ================================================== */

interior b32
_cursus_tractare_nuntium(
    CursusNuntiorum* cursus,
    chorda*          entitas_id,
    Nuntius*         nuntius)
{
    Entitas*           entitas;
    FunctioTractandi   tractator;
    ContextusTractandi ctx;

    si (!cursus || !entitas_id || !nuntius)
    {
        redde FALSUM;
    }

    /* Obtinere entitatem */
    entitas = cursus->repositorium->capere_entitatem(
        cursus->repositorium->datum,
        entitas_id);

    si (!entitas)
    {
        /* Entitas non existit - nuntius ignoratus */
        redde FALSUM;
    }

    /* Quaerere tractator */
    tractator = registrum_tractatoris_invenire(
        cursus->tractator,
        entitas->genus,
        nuntius->genus);

    si (!tractator)
    {
        /* Nullus tractator - nuntius ignoratus (no-op) */
        redde VERUM;
    }

    /* Preparare contextum */
    ctx.entitas      = entitas;
    ctx.nuntius      = nuntius;
    ctx.repositorium = cursus->repositorium;
    ctx.cursus       = cursus;
    ctx.piscina      = cursus->piscina;
    ctx.datum_custom = cursus->datum_custom;

    /* Invocare tractator */
    redde tractator(&ctx);
}


/* ==================================================
 * Creatio
 * ================================================== */

CursusNuntiorum*
cursus_creare(
    Piscina*              piscina,
    RegistrumTractatoris* tractator,
    EntitasRepositorium*  repositorium,
    InternamentumChorda*  intern)
{
    CursusNuntiorum* cursus;

    si (!piscina || !tractator || !repositorium || !intern)
    {
        redde NIHIL;
    }

    cursus = (CursusNuntiorum*)piscina_allocare(piscina, magnitudo(CursusNuntiorum));
    si (!cursus)
    {
        redde NIHIL;
    }

    cursus->piscina      = piscina;
    cursus->tractator    = tractator;
    cursus->repositorium = repositorium;
    cursus->intern       = intern;
    cursus->datum_custom = NIHIL;

    /* Creare tabulam capsarum */
    cursus->capsae = tabula_dispersa_creare_chorda(piscina, LXIV);
    si (!cursus->capsae)
    {
        redde NIHIL;
    }

    redde cursus;
}

vacuum
cursus_ponere_datum(
    CursusNuntiorum* cursus,
    vacuum*          datum)
{
    si (cursus)
    {
        cursus->datum_custom = datum;
    }
}


/* ==================================================
 * Mittere Nuntios
 * ================================================== */

b32
cursus_mittere(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,
    chorda*             destinatio_id,
    constans character* genus_nuntii,
    TabulaDispersa*     datum)
{
    Nuntius* nuntius;
    Capsa*   capsa;

    si (!cursus || !destinatio_id || !genus_nuntii)
    {
        redde FALSUM;
    }

    /* Creare nuntium */
    nuntius = nuntius_creare(cursus->piscina, cursus->intern, mittens_id, genus_nuntii);
    si (!nuntius)
    {
        redde FALSUM;
    }

    /* Copiare datum si provisum */
    si (datum)
    {
        nuntius->datum = datum;
    }

    /* Obtinere vel creare capsam */
    capsa = _cursus_obtinere_vel_creare_capsam(cursus, destinatio_id);
    si (!capsa)
    {
        redde FALSUM;
    }

    /* Addere nuntium ad capsam */
    redde capsa_addere(capsa, nuntius);
}

b32
cursus_mittere_nuntium(
    CursusNuntiorum* cursus,
    chorda*          destinatio_id,
    Nuntius*         nuntius)
{
    Capsa* capsa;

    si (!cursus || !destinatio_id || !nuntius)
    {
        redde FALSUM;
    }

    /* Obtinere vel creare capsam */
    capsa = _cursus_obtinere_vel_creare_capsam(cursus, destinatio_id);
    si (!capsa)
    {
        redde FALSUM;
    }

    /* Addere nuntium ad capsam */
    redde capsa_addere(capsa, nuntius);
}

b32
cursus_mittere_simplex(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,
    chorda*             destinatio_id,
    constans character* genus_nuntii)
{
    redde cursus_mittere(cursus, mittens_id, destinatio_id, genus_nuntii, NIHIL);
}


/* ==================================================
 * Processare Nuntios
 * ================================================== */

i32
cursus_processare(
    CursusNuntiorum* cursus,
    i32              maximus_nuntii)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;
    i32            tractati;

    si (!cursus || maximus_nuntii <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    tractati = ZEPHYRUM;

    /* Iterare per omnes capsas */
    iter = tabula_dispersa_iterator_initium(cursus->capsae);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        Capsa*   capsa;
        Nuntius* nuntius;
        chorda*  entitas_id;

        capsa = (Capsa*)valor;
        entitas_id = capsa->possessor_id;

        /* Tractare nuntios in hac capsa */
        dum (!capsa_vacua_est(capsa) && tractati < maximus_nuntii)
        {
            nuntius = capsa_tollere(capsa);
            si (nuntius)
            {
                _cursus_tractare_nuntium(cursus, entitas_id, nuntius);
                tractati++;
            }
        }

        /* Si attigimus limitem, exire */
        si (tractati >= maximus_nuntii)
        {
            frange;
        }
    }

    redde tractati;
}

i32
cursus_processare_entitatem(
    CursusNuntiorum* cursus,
    chorda*          entitas_id,
    i32              maximus_nuntii)
{
    Capsa*   capsa;
    Nuntius* nuntius;
    i32      tractati;

    si (!cursus || !entitas_id || maximus_nuntii <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    capsa = cursus_obtinere_capsam(cursus, entitas_id);
    si (!capsa)
    {
        redde ZEPHYRUM;
    }

    tractati = ZEPHYRUM;

    dum (!capsa_vacua_est(capsa) && tractati < maximus_nuntii)
    {
        nuntius = capsa_tollere(capsa);
        si (nuntius)
        {
            _cursus_tractare_nuntium(cursus, entitas_id, nuntius);
            tractati++;
        }
    }

    redde tractati;
}


/* ==================================================
 * Quaestio
 * ================================================== */

i32
cursus_numerus_pendentium(
    CursusNuntiorum* cursus)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;
    i32            summa;

    si (!cursus)
    {
        redde ZEPHYRUM;
    }

    summa = ZEPHYRUM;

    iter = tabula_dispersa_iterator_initium(cursus->capsae);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        Capsa* capsa;
        capsa = (Capsa*)valor;
        summa += capsa_numerus(capsa);
    }

    redde summa;
}

i32
cursus_numerus_pendentium_entitatis(
    CursusNuntiorum* cursus,
    chorda*          entitas_id)
{
    Capsa* capsa;

    si (!cursus || !entitas_id)
    {
        redde ZEPHYRUM;
    }

    capsa = cursus_obtinere_capsam(cursus, entitas_id);
    si (!capsa)
    {
        redde ZEPHYRUM;
    }

    redde capsa_numerus(capsa);
}

b32
cursus_habet_capsam(
    CursusNuntiorum* cursus,
    chorda*          entitas_id)
{
    si (!cursus || !entitas_id)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet(cursus->capsae, *entitas_id);
}

Capsa*
cursus_obtinere_capsam(
    CursusNuntiorum* cursus,
    chorda*          entitas_id)
{
    vacuum* valor;

    si (!cursus || !entitas_id)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire(cursus->capsae, *entitas_id, &valor))
    {
        redde (Capsa*)valor;
    }

    redde NIHIL;
}


/* ==================================================
 * Utilitas
 * ================================================== */

vacuum
cursus_vacare(
    CursusNuntiorum* cursus)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;

    si (!cursus)
    {
        redde;
    }

    iter = tabula_dispersa_iterator_initium(cursus->capsae);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        Capsa* capsa;
        capsa = (Capsa*)valor;
        capsa_vacare(capsa);
    }
}

vacuum
cursus_imprimere(
    CursusNuntiorum* cursus)
{
    TabulaIterator iter;
    chorda         clavis;
    vacuum*        valor;
    i32            total_capsae;
    i32            total_nuntii;

    si (!cursus)
    {
        imprimere("CursusNuntiorum: NIHIL\n");
        redde;
    }

    total_capsae = tabula_dispersa_numerus(cursus->capsae);
    total_nuntii = cursus_numerus_pendentium(cursus);

    imprimere("CursusNuntiorum:\n");
    imprimere("  Capsae: %d\n", total_capsae);
    imprimere("  Nuntii pendentes: %d\n", total_nuntii);

    si (total_capsae > ZEPHYRUM)
    {
        imprimere("  Per entitatem:\n");

        iter = tabula_dispersa_iterator_initium(cursus->capsae);
        dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
        {
            Capsa* capsa;
            capsa = (Capsa*)valor;

            imprimere("    %.*s: %d nuntii\n",
                      clavis.mensura, clavis.datum,
                      capsa_numerus(capsa));
        }
    }
}
