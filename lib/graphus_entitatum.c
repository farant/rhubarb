#include "graphus_entitatum.h"
#include "uuid.h"
#include <string.h>

/* ==================================================
 * Functiones Implementationis Repositorium - Lectio (Interior)
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

interior Relatio*
_impl_capere_relatio(
    vacuum* datum,
    chorda* relatio_id)
{
    GraphusEntitatum* graphus;
    vacuum*           valor;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !relatio_id)
    {
        redde NIHIL;
    }

    /* Quaerere in tabula dispersa relationum */
    si (tabula_dispersa_invenire(graphus->relationes, *relatio_id, &valor))
    {
        redde (Relatio*)valor;
    }

    redde NIHIL;
}

interior Xar*
_impl_capere_relationes_ad(
    vacuum* datum,
    chorda* entitas_id)
{
    GraphusEntitatum* graphus;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Relatio*          relatio;
    Relatio**         slot;
    i32               i;
    i32               numerus;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas_id)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(graphus->piscina, magnitudo(Relatio*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per omnes entitates et quaerere relationes ad entitas_id */
    iter = tabula_dispersa_iterator_initium(graphus->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

        /* Iterare per relationes huius entitatis */
        numerus = xar_numerus(entitas->relationes);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
            si (!relatio)
            {
                perge;
            }

            /* Verificare si destinatio est entitas_id */
            si (relatio->destinatio_id == entitas_id)  /* Aequalitas indicis */
            {
                slot = (Relatio**)xar_addere(resultus);
                si (slot)
                {
                    *slot = relatio;
                }
            }
        }
    }

    redde resultus;
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

    /* Creare tabulam dispersam pro relationes (index secundarius) */
    graphus->relationes = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (!graphus->relationes)
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
 * Additio Relationum (Index)
 * ================================================== */

b32
graphus_entitatum_registrare_relatio(
    GraphusEntitatum* graphus,
    Relatio*          relatio)
{
    si (!graphus || !relatio || !relatio->id)
    {
        redde FALSUM;
    }

    /* Verificare si ID iam existit */
    si (tabula_dispersa_continet(graphus->relationes, *relatio->id))
    {
        redde FALSUM;  /* ID iam existit */
    }

    /* Inserere in tabulam */
    redde tabula_dispersa_inserere(graphus->relationes, *relatio->id, relatio);
}

b32
graphus_entitatum_deregistrare_relatio(
    GraphusEntitatum* graphus,
    chorda*           relatio_id)
{
    si (!graphus || !relatio_id)
    {
        redde FALSUM;
    }

    /* Delere ex tabula */
    redde tabula_dispersa_delere(graphus->relationes, *relatio_id);
}


/* ==================================================
 * Functiones Implementationis Repositorium - Scriptio (Interior)
 * ================================================== */

interior Entitas*
_impl_entitas_creare(
    vacuum*             datum,
    constans character* genus)
{
    GraphusEntitatum* graphus;
    chorda*           id_internatum;
    chorda*           genus_internatum;
    Entitas*          entitas;
    chorda            uuid_chorda;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !genus)
    {
        redde NIHIL;
    }

    /* Generare UUIDv7 */
    uuid_chorda = uuidv7_creare(graphus->piscina);
    si (!uuid_chorda.datum)
    {
        redde NIHIL;
    }

    /* Internare ID et genus */
    id_internatum = chorda_internare(graphus->intern, uuid_chorda);
    si (!id_internatum)
    {
        redde NIHIL;
    }

    genus_internatum = chorda_internare_ex_literis(graphus->intern, genus);
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    /* Creare entitatem */
    entitas = entitas_creare(graphus->piscina, id_internatum, genus_internatum);
    si (!entitas)
    {
        redde NIHIL;
    }

    /* Addere ad graphum */
    si (!graphus_entitatum_addere_entitatem(graphus, entitas))
    {
        redde NIHIL;
    }

    redde entitas;
}

interior b32
_impl_entitas_delere(
    vacuum* datum,
    chorda* id)
{
    GraphusEntitatum* graphus;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !id)
    {
        redde FALSUM;
    }

    /* Delere ex tabula dispersa */
    redde tabula_dispersa_delere(graphus->entitates, *id);
}

interior b32
_impl_proprietas_ponere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* clavis,
    constans character* valor)
{
    GraphusEntitatum* graphus;
    chorda*           clavis_internata;
    chorda*           valor_internatus;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    /* Internare clavis et valor */
    clavis_internata = chorda_internare_ex_literis(graphus->intern, clavis);
    si (!clavis_internata)
    {
        redde FALSUM;
    }

    valor_internatus = chorda_internare_ex_literis(graphus->intern, valor);
    si (!valor_internatus)
    {
        redde FALSUM;
    }

    redde entitas_proprietas_addere(entitas, clavis_internata, valor_internatus);
}

interior b32
_impl_proprietas_delere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* clavis)
{
    GraphusEntitatum* graphus;
    chorda*           clavis_internata;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !clavis)
    {
        redde FALSUM;
    }

    /* Internare clavis */
    clavis_internata = chorda_internare_ex_literis(graphus->intern, clavis);
    si (!clavis_internata)
    {
        redde FALSUM;
    }

    redde entitas_proprietas_delere(entitas, clavis_internata);
}

interior Relatio*
_impl_relatio_addere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* genus,
    chorda*             destinatio_id)
{
    GraphusEntitatum* graphus;
    chorda*           genus_internatum;
    Relatio*          relatio;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !genus || !destinatio_id)
    {
        redde NIHIL;
    }

    /* Internare genus */
    genus_internatum = chorda_internare_ex_literis(graphus->intern, genus);
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    /* Addere relationem */
    relatio = entitas_relatio_addere(
        entitas,
        graphus->piscina,
        graphus->intern,
        genus_internatum,
        destinatio_id);

    si (!relatio)
    {
        redde NIHIL;
    }

    /* Registrare in indice */
    graphus_entitatum_registrare_relatio(graphus, relatio);

    redde relatio;
}

interior b32
_impl_relatio_delere(
    vacuum* datum,
    chorda* relatio_id)
{
    GraphusEntitatum* graphus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !relatio_id)
    {
        redde FALSUM;
    }

    /* Deregistrare ex indice */
    graphus_entitatum_deregistrare_relatio(graphus, relatio_id);

    /* Quaerere entitatem cum hac relatione et delere */
    iter = tabula_dispersa_iterator_initium(graphus->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;
        si (entitas_relatio_delere(entitas, relatio_id))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior b32
_impl_nota_addere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* nota)
{
    GraphusEntitatum* graphus;
    chorda*           nota_internata;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !nota)
    {
        redde FALSUM;
    }

    /* Internare nota */
    nota_internata = chorda_internare_ex_literis(graphus->intern, nota);
    si (!nota_internata)
    {
        redde FALSUM;
    }

    redde entitas_nota_addere(entitas, nota_internata);
}

interior b32
_impl_nota_delere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* nota)
{
    GraphusEntitatum* graphus;
    chorda*           nota_internata;

    graphus = (GraphusEntitatum*)datum;

    si (!graphus || !entitas || !nota)
    {
        redde FALSUM;
    }

    /* Internare nota */
    nota_internata = chorda_internare_ex_literis(graphus->intern, nota);
    si (!nota_internata)
    {
        redde FALSUM;
    }

    redde entitas_nota_delere(entitas, nota_internata);
}


/* ==================================================
 * Repositorium Interface
 * ================================================== */

EntitasRepositorium*
graphus_entitatum_repositorium_creare(
    GraphusEntitatum* graphus)
{
    EntitasRepositorium* repositorium;

    si (!graphus)
    {
        redde NIHIL;
    }

    /* Allocare structuram repositorium */
    repositorium = (EntitasRepositorium*)piscina_allocare(
        graphus->piscina,
        magnitudo(EntitasRepositorium));
    si (!repositorium)
    {
        redde NIHIL;
    }

    /* Ponere datum */
    repositorium->datum = graphus;

    /* Ponere indices functionum - Lectio */
    repositorium->capere_entitatem             = _impl_capere_entitatem;
    repositorium->capere_entitates_relatae     = _impl_capere_entitates_relatae;
    repositorium->quaerere_cum_nota            = _impl_quaerere_cum_nota;
    repositorium->quaerere_cum_praefixo_notae  = _impl_quaerere_cum_praefixo_notae;
    repositorium->quaerere_textum              = _impl_quaerere_textum;
    repositorium->capere_radices               = _impl_capere_radices;
    repositorium->capere_relatio               = _impl_capere_relatio;
    repositorium->capere_relationes_ad         = _impl_capere_relationes_ad;

    /* Ponere indices functionum - Scriptio */
    repositorium->entitas_creare               = _impl_entitas_creare;
    repositorium->entitas_delere               = _impl_entitas_delere;
    repositorium->proprietas_ponere            = _impl_proprietas_ponere;
    repositorium->proprietas_delere            = _impl_proprietas_delere;
    repositorium->relatio_addere               = _impl_relatio_addere;
    repositorium->relatio_delere               = _impl_relatio_delere;
    repositorium->nota_addere                  = _impl_nota_addere;
    repositorium->nota_delere                  = _impl_nota_delere;

    redde repositorium;
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
