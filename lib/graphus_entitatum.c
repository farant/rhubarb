#include "graphus_entitatum.h"
#include <string.h>

/* ==================================================
 * Functiones Implementationis Providor (Interior)
 * ================================================== */

interior Entitas*
_impl_capere_entitatem(
    vacuum* datum,
    chorda* id)
{
    GraphusEntitatum* graphus;
    vacuum*           valor;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !id)
    {
        redde NIHIL;
    }

    /* Quaerere in tabula dispersa */
    si (tabula_dispersa_invenire(graphus->entitates, *id, &valor))
    {
        redde (Entitas*)valor;
    }

    redde NIHIL;
}

interior Xar*
_impl_capere_entitates_relatae(
    vacuum*  datum,
    Entitas* entitas,
    chorda*  genus_relationis)
{
    GraphusEntitatum* graphus;
    Xar*              resultus;
    Relatio*          relatio;
    Entitas*          entitas_relata;
    Entitas**         slot;
    i32               i;
    i32               numerus;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !genus_relationis)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(graphus->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per relationes entitatis */
    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (!relatio)
        {
            perge;
        }

        /* Filtrare per genus */
        si (relatio->genus != genus_relationis)  /* Aequalitas indicis */
        {
            perge;
        }

        /* Resolvere destinatio ID ad entitatem */
        entitas_relata = _impl_capere_entitatem(datum, relatio->destinatio_id);
        si (entitas_relata)
        {
            slot = (Entitas**)xar_addere(resultus);
            si (slot)
            {
                *slot = entitas_relata;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_quaerere_cum_nota(
    vacuum* datum,
    chorda* nota)
{
    GraphusEntitatum* graphus;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas**         slot;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !nota)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(graphus->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per omnes entitates */
    iter = tabula_dispersa_iterator_initium(graphus->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

        /* Verificare si entitas habet notam */
        si (entitas_nota_habet(entitas, nota))
        {
            slot = (Entitas**)xar_addere(resultus);
            si (slot)
            {
                *slot = entitas;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_quaerere_cum_praefixo_notae(
    vacuum*              datum,
    constans character*  praefixum)
{
    GraphusEntitatum* graphus;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas**         slot;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !praefixum)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(graphus->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per omnes entitates */
    iter = tabula_dispersa_iterator_initium(graphus->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

        /* Verificare si entitas habet notam cum praefixo */
        si (entitas_nota_cum_praefixo_habet(entitas, praefixum))
        {
            slot = (Entitas**)xar_addere(resultus);
            si (slot)
            {
                *slot = entitas;
            }
        }
    }

    redde resultus;
}

interior b32
_continet_substring(constans character* haystack, constans character* needle)
{
    redde strstr(haystack, needle) != NIHIL;
}

interior Xar*
_impl_quaerere_textum(
    vacuum*              datum,
    constans character*  query)
{
    GraphusEntitatum* graphus;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas**         slot;
    Proprietas*       prop;
    i32               i;
    i32               numerus_prop;
    b32               inventum;
    character         buffer_id[CCLVI];
    character         buffer_valor[CCLVI];

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !query)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(graphus->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per omnes entitates */
    iter = tabula_dispersa_iterator_initium(graphus->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;
        inventum = FALSUM;

        /* Verificare ID */
        si (entitas->id->mensura < CCLVI)
        {
            memcpy(buffer_id, entitas->id->datum, (memoriae_index)entitas->id->mensura);
            buffer_id[entitas->id->mensura] = '\0';

            si (_continet_substring(buffer_id, query))
            {
                inventum = VERUM;
            }
        }

        /* Verificare valores proprietatum */
        si (!inventum)
        {
            numerus_prop = xar_numerus(entitas->proprietates);
            per (i = ZEPHYRUM; i < numerus_prop; i++)
            {
                prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
                si (!prop || !prop->valor)
                {
                    perge;
                }

                si (prop->valor->mensura < CCLVI)
                {
                    memcpy(buffer_valor, prop->valor->datum,
                           (memoriae_index)prop->valor->mensura);
                    buffer_valor[prop->valor->mensura] = '\0';

                    si (_continet_substring(buffer_valor, query))
                    {
                        inventum = VERUM;
                        frange;
                    }
                }
            }
        }

        /* Si inventum, addere ad resultus */
        si (inventum)
        {
            slot = (Entitas**)xar_addere(resultus);
            si (slot)
            {
                *slot = entitas;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_capere_radices(
    vacuum* datum)
{
    GraphusEntitatum* graphus;
    chorda*           nota_root;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus)
    {
        redde NIHIL;
    }

    /* Quaerere omnes entitates cum nota "#root" */
    nota_root = chorda_internare_ex_literis(graphus->intern, "#root");
    si (!nota_root)
    {
        redde NIHIL;
    }

    redde _impl_quaerere_cum_nota(datum, nota_root);
}


/* ==================================================
 * Creatio
 * ================================================== */

GraphusEntitatum*
graphus_entitatum_creare(
    Piscina* piscina)
{
    GraphusEntitatum* graphus;

    si (!piscina)
    {
        redde NIHIL;
    }

    /* Allocare structuram graphi */
    graphus = (GraphusEntitatum*)piscina_allocare(piscina, magnitudo(GraphusEntitatum));
    si (!graphus)
    {
        redde NIHIL;
    }

    graphus->piscina = piscina;

    /* Creare internamentum */
    graphus->intern = internamentum_creare(piscina);
    si (!graphus->intern)
    {
        redde NIHIL;
    }

    /* Creare tabulam dispersam pro entitates */
    graphus->entitates = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (!graphus->entitates)
    {
        redde NIHIL;
    }

    redde graphus;
}


/* ==================================================
 * Additio Entitatum
 * ================================================== */

b32
graphus_entitatum_addere_entitatem(
    GraphusEntitatum* graphus,
    Entitas*          entitas)
{
    si (!graphus || !entitas || !entitas->id)
    {
        redde FALSUM;
    }

    /* Verificare si ID iam existit */
    si (tabula_dispersa_continet(graphus->entitates, *entitas->id))
    {
        redde FALSUM;  /* ID iam existit */
    }

    /* Inserere in tabulam */
    redde tabula_dispersa_inserere(graphus->entitates, *entitas->id, entitas);
}


/* ==================================================
 * Providor Interface
 * ================================================== */

EntitasProvidor*
graphus_entitatum_providor_creare(
    GraphusEntitatum* graphus)
{
    EntitasProvidor* providor;

    si (!graphus)
    {
        redde NIHIL;
    }

    /* Allocare structuram providor */
    providor = (EntitasProvidor*)piscina_allocare(
        graphus->piscina,
        magnitudo(EntitasProvidor));
    si (!providor)
    {
        redde NIHIL;
    }

    /* Ponere datum */
    providor->datum = graphus;

    /* Ponere indices functionum */
    providor->capere_entitatem             = _impl_capere_entitatem;
    providor->capere_entitates_relatae     = _impl_capere_entitates_relatae;
    providor->quaerere_cum_nota            = _impl_quaerere_cum_nota;
    providor->quaerere_cum_praefixo_notae  = _impl_quaerere_cum_praefixo_notae;
    providor->quaerere_textum              = _impl_quaerere_textum;
    providor->capere_radices               = _impl_capere_radices;

    redde providor;
}


/* ==================================================
 * Utilitas
 * ================================================== */

InternamentumChorda*
graphus_entitatum_internamentum(
    GraphusEntitatum* graphus)
{
    si (!graphus)
    {
        redde NIHIL;
    }
    redde graphus->intern;
}

i32
graphus_entitatum_numerus(
    GraphusEntitatum* graphus)
{
    si (!graphus || !graphus->entitates)
    {
        redde ZEPHYRUM;
    }
    redde tabula_dispersa_numerus(graphus->entitates);
}
