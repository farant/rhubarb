#include "entitas_repositorium.h"
#include "persistentia.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "uuid.h"
#include <string.h>

/* ==================================================
 * ENTITAS REPOSITORIUM IMPL - Unified Event-Sourced Repository
 *
 * Single implementation backed by Persistentia interface.
 * All scaffolding, queries, and business logic lives here.
 * Same code path regardless of persistence backend (memory or file).
 *
 * ================================================== */


/* ==================================================
 * Structura Interna
 * ================================================== */

nomen structura {
    Piscina*             piscina;
    Persistentia*        persistentia;
    InternamentumChorda* intern;
    TabulaDispersa*      entitates;   /* id -> Entitas* */
    TabulaDispersa*      relationes;  /* relatio_id -> Relatio* */
} RepositoriumData;


/* ==================================================
 * Forward Declarations
 * ================================================== */

interior b32
_impl_proprietas_ponere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* clavis,
    constans character* valor);

interior Relatio*
_impl_relatio_addere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* genus,
    chorda*             destinatio_id);

interior Entitas*
_impl_proprietas_definitio_invenire(
    vacuum*  datum,
    chorda*  entitas_genus,
    chorda*  proprietas_nomen);


/* ==================================================
 * Read Functions
 * ================================================== */

interior Entitas*
_impl_capere_entitatem(
    vacuum* datum,
    chorda* id)
{
    RepositoriumData* data;
    vacuum*           valor;

    data = (RepositoriumData*)datum;

    si (!data || !id)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire(data->entitates, *id, &valor))
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
    RepositoriumData* data;
    Xar*              resultus;
    Relatio*          relatio;
    Entitas*          entitas_relata;
    Entitas**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !genus_relationis)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (!relatio)
        {
            perge;
        }

        /* Filtrare per genus (pointer comparison - interned) */
        si (relatio->genus != genus_relationis)
        {
            perge;
        }

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
    RepositoriumData* data;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas**         slot;

    data = (RepositoriumData*)datum;

    si (!data || !nota)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    iter = tabula_dispersa_iterator_initium(data->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

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
    RepositoriumData* data;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas**         slot;

    data = (RepositoriumData*)datum;

    si (!data || !praefixum)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    iter = tabula_dispersa_iterator_initium(data->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

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

/* Invenire ProprietasDefinitio pro entitate genus + proprietas nomen
 * Quaerit entities cum genus="ProprietasDefinitio"
 * ubi proprietas "entitas_genus" == target genus
 * et proprietas "name" == target clavis
 *
 * Redde: Entitas* (ProprietasDefinitio) si inventum, NIHIL si non
 */
interior Entitas*
_impl_proprietas_definitio_invenire(
    vacuum*  datum,
    chorda*  entitas_genus,
    chorda*  proprietas_nomen)
{
    RepositoriumData* data;
    TabulaIterator    iter;
    chorda            clavis_iter;
    vacuum*           valor;
    Entitas*          entitas;
    chorda*           prop_entitas_genus;
    chorda*           prop_proprietas_nomen;

    data = (RepositoriumData*)datum;

    si (!data || !entitas_genus || !proprietas_nomen)
    {
        redde NIHIL;
    }

    iter = tabula_dispersa_iterator_initium(data->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis_iter, &valor))
    {
        entitas = (Entitas*)valor;

        /* Verificare genus == "ProprietasDefinitio" */
        si (!entitas->genus ||
            !chorda_aequalis_literis(*entitas->genus, "ProprietasDefinitio"))
        {
            perge;
        }

        /* Capere proprietas "entitas_genus" */
        prop_entitas_genus = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(data->intern, "entitas_genus"));

        si (!prop_entitas_genus || prop_entitas_genus != entitas_genus)
        {
            perge;  /* Non aequalis (pointer comparison pro interned) */
        }

        /* Capere proprietas "name" */
        prop_proprietas_nomen = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(data->intern, "name"));

        si (!prop_proprietas_nomen || prop_proprietas_nomen != proprietas_nomen)
        {
            perge;  /* Non aequalis */
        }

        /* Inventum! */
        redde entitas;
    }

    redde NIHIL;  /* Non inventum */
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
    RepositoriumData* data;
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

    data = (RepositoriumData*)datum;

    si (!data || !query)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Entitas*));
    si (!resultus)
    {
        redde NIHIL;
    }

    iter = tabula_dispersa_iterator_initium(data->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;
        inventum = FALSUM;

        /* Verificare ID */
        si (entitas->id->mensura < CCLVI)
        {
            memcpy(buffer_id, entitas->id->datum,
                   (memoriae_index)entitas->id->mensura);
            buffer_id[entitas->id->mensura] = '\0';

            si (_continet_substring(buffer_id, query))
            {
                inventum = VERUM;
            }
        }

        /* Verificare genus */
        si (!inventum && entitas->genus && entitas->genus->mensura < CCLVI)
        {
            memcpy(buffer_valor, entitas->genus->datum,
                   (memoriae_index)entitas->genus->mensura);
            buffer_valor[entitas->genus->mensura] = '\0';

            si (_continet_substring(buffer_valor, query))
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
    RepositoriumData* data;
    chorda*           nota_root;

    data = (RepositoriumData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    nota_root = chorda_internare_ex_literis(data->intern, "#root");
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
    RepositoriumData* data;
    vacuum*           valor;

    data = (RepositoriumData*)datum;

    si (!data || !relatio_id)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire(data->relationes, *relatio_id, &valor))
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
    RepositoriumData* data;
    Xar*              resultus;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Relatio*          relatio;
    Relatio**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data || !entitas_id)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Relatio*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Iterare per omnes entitates */
    iter = tabula_dispersa_iterator_initium(data->entitates);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        entitas = (Entitas*)valor;

        numerus = xar_numerus(entitas->relationes);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
            si (!relatio)
            {
                perge;
            }

            si (relatio->destinatio_id == entitas_id)  /* Pointer comparison */
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
 * Genus Hierarchy Scaffolding
 * ================================================== */

/* Creare entitatem Genus pro via data (e.g., "Application-State::Widget")
 * Sine eventum - usatur interne tantum */
interior Entitas*
_assecurare_genus_entitatem_sine_evento(
    RepositoriumData*   data,
    constans character* via)
{
    chorda    uuid_chorda;
    chorda*   id_internatum;
    chorda*   genus_internatum;
    Entitas*  entitas_existens;
    Entitas*  entitas;
    character appellatio_buffer[CCLVI];

    /* Generare ID deterministicum: "Genus:<via>" */
    uuid_chorda = uuidv5_ex_genere_et_titulo(data->piscina, "Genus", via);
    si (!uuid_chorda.datum)
    {
        redde NIHIL;
    }

    id_internatum = chorda_internare(data->intern, uuid_chorda);
    si (!id_internatum)
    {
        redde NIHIL;
    }

    /* Verificare si iam existit */
    si (tabula_dispersa_invenire(data->entitates, *id_internatum, (vacuum**)&entitas_existens))
    {
        redde entitas_existens;
    }

    /* Creare novam entitatem Genus */
    genus_internatum = chorda_internare_ex_literis(data->intern, "Genus");
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    entitas = entitas_creare(data->piscina, id_internatum, genus_internatum);
    si (!entitas)
    {
        redde NIHIL;
    }

    si (!tabula_dispersa_inserere(data->entitates, *id_internatum, entitas))
    {
        redde NIHIL;
    }

    /* Emit event */
    {
        Eventum* e = eventum_creare_entitas(data->piscina, id_internatum, genus_internatum);
        si (e)
        {
            data->persistentia->scribere_eventum(data->persistentia->datum, e);
        }
    }

    /* Ponere proprietatem "name" ad segmentum ultimum */
    {
        constans character* segmentum;
        constans character* p;
        i32 seg_len;

        segmentum = via;
        p = via;
        dum (*p != '\0')
        {
            si (p[ZEPHYRUM] == ':' && p[I] == ':')
            {
                segmentum = p + II;
                p += II;
            }
            alioquin
            {
                p++;
            }
        }

        seg_len = ZEPHYRUM;
        dum (segmentum[seg_len] != '\0' && seg_len < CCLV)
        {
            appellatio_buffer[seg_len] = segmentum[seg_len];
            seg_len++;
        }
        appellatio_buffer[seg_len] = '\0';

        _impl_proprietas_ponere(data, entitas, "name", appellatio_buffer);
    }

    redde entitas;
}

/* Assecurare hierarchiam generis et creare relationes "contains" */
interior Entitas*
_assecurare_genus_hierarchiam(
    RepositoriumData*   data,
    constans character* genus)
{
    Entitas*   genus_radix;
    Entitas*   genus_parens;
    Entitas*   genus_currens;
    character  via_buffer[CCLVI];
    i32        via_len;
    i32        i;
    chorda*    contains_genus;

    si (!data || !genus)
    {
        redde NIHIL;
    }

    contains_genus = chorda_internare_ex_literis(data->intern, "contains");
    si (!contains_genus)
    {
        redde NIHIL;
    }

    /* Assecurare Genus::Genus (radix) */
    genus_radix = _assecurare_genus_entitatem_sine_evento(data, "Genus");
    si (!genus_radix)
    {
        redde NIHIL;
    }

    /* Creare entitatem pro toto genere */
    genus_currens = _assecurare_genus_entitatem_sine_evento(data, genus);
    si (!genus_currens)
    {
        redde NIHIL;
    }

    /* Construere hierarchiam */
    genus_parens = genus_radix;
    via_buffer[ZEPHYRUM] = '\0';
    via_len = ZEPHYRUM;

    per (i = ZEPHYRUM; genus[i] != '\0'; i++)
    {
        si (genus[i] == ':' && genus[i + I] != '\0' && genus[i + I] == ':')
        {
            via_buffer[via_len] = '\0';

            genus_currens = _assecurare_genus_entitatem_sine_evento(data, via_buffer);
            si (!genus_currens)
            {
                redde NIHIL;
            }

            /* Addere relatio "contains" si nondum existit */
            {
                Relatio* rel;
                i32      num_rel;
                i32      j;
                b32      iam_habet;

                iam_habet = FALSUM;
                num_rel = xar_numerus(genus_parens->relationes);
                per (j = ZEPHYRUM; j < num_rel; j++)
                {
                    rel = (Relatio*)xar_obtinere(genus_parens->relationes, j);
                    si (rel && rel->genus == contains_genus &&
                        rel->destinatio_id == genus_currens->id)
                    {
                        iam_habet = VERUM;
                        frange;
                    }
                }

                si (!iam_habet)
                {
                    _impl_relatio_addere(data, genus_parens, "contains",
                                         genus_currens->id);
                }
            }

            genus_parens = genus_currens;

            via_buffer[via_len] = ':';
            via_len++;
            via_buffer[via_len] = ':';
            via_len++;
            i++;
        }
        alioquin
        {
            via_buffer[via_len] = genus[i];
            via_len++;
        }
    }

    /* Creare genus finale */
    via_buffer[via_len] = '\0';
    genus_currens = _assecurare_genus_entitatem_sine_evento(data, via_buffer);
    si (!genus_currens)
    {
        redde NIHIL;
    }

    /* Addere relatio "contains" a parente ad folium */
    {
        Relatio* rel;
        i32      num_rel;
        i32      j;
        b32      iam_habet;

        iam_habet = FALSUM;
        num_rel = xar_numerus(genus_parens->relationes);
        per (j = ZEPHYRUM; j < num_rel; j++)
        {
            rel = (Relatio*)xar_obtinere(genus_parens->relationes, j);
            si (rel && rel->genus == contains_genus &&
                rel->destinatio_id == genus_currens->id)
            {
                iam_habet = VERUM;
                frange;
            }
        }

        si (!iam_habet)
        {
            _impl_relatio_addere(data, genus_parens, "contains",
                                 genus_currens->id);
        }
    }

    redde genus_currens;
}


/* ==================================================
 * Write Functions (with event emission)
 * ================================================== */

interior Entitas*
_impl_entitas_creare(
    vacuum*             datum,
    constans character* genus)
{
    RepositoriumData* data;
    chorda            uuid_chorda;
    chorda*           id_internatum;
    chorda*           genus_internatum;
    Entitas*          entitas;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !genus)
    {
        redde NIHIL;
    }

    /* Generare UUIDv7 */
    uuid_chorda = uuidv7_creare(data->piscina);
    si (!uuid_chorda.datum)
    {
        redde NIHIL;
    }

    id_internatum = chorda_internare(data->intern, uuid_chorda);
    si (!id_internatum)
    {
        redde NIHIL;
    }

    genus_internatum = chorda_internare_ex_literis(data->intern, genus);
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    /* Emit event */
    e = eventum_creare_entitas(data->piscina, id_internatum, genus_internatum);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde NIHIL;
    }

    /* Creare in memoria */
    entitas = entitas_creare(data->piscina, id_internatum, genus_internatum);
    si (!entitas)
    {
        redde NIHIL;
    }

    si (!tabula_dispersa_inserere(data->entitates, *id_internatum, entitas))
    {
        redde NIHIL;
    }

    /* Assecurare genus hierarchiam et addere "est" relationem
     * (non pro "Genus" ipso - evitare recursionem)
     */
    si (strcmp(genus, "Genus") != ZEPHYRUM)
    {
        Entitas* genus_folium;
        genus_folium = _assecurare_genus_hierarchiam(data, genus);
        si (genus_folium)
        {
            /* Addere "est" relationem: entitas --[est]--> Genus::X */
            _impl_relatio_addere(data, entitas, "est", genus_folium->id);
        }
    }

    redde entitas;
}

interior Entitas*
_impl_entitas_scaffoldare(
    vacuum*             datum,
    constans character* genus,
    constans character* titulus)
{
    RepositoriumData* data;
    chorda            uuid_chorda;
    chorda*           id_internatum;
    chorda*           genus_internatum;
    Entitas*          entitas_existens;
    Entitas*          entitas;
    Entitas*          genus_folium;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !genus || !titulus)
    {
        redde NIHIL;
    }

    /* Generare ID deterministicum */
    uuid_chorda = uuidv5_ex_genere_et_titulo(data->piscina, genus, titulus);
    si (!uuid_chorda.datum)
    {
        redde NIHIL;
    }

    id_internatum = chorda_internare(data->intern, uuid_chorda);
    si (!id_internatum)
    {
        redde NIHIL;
    }

    /* Verificare si existit */
    si (tabula_dispersa_invenire(data->entitates, *id_internatum, (vacuum**)&entitas_existens))
    {
        redde entitas_existens;
    }

    /* Non existit - creare */
    genus_internatum = chorda_internare_ex_literis(data->intern, genus);
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    /* Emit event */
    e = eventum_creare_entitas(data->piscina, id_internatum, genus_internatum);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde NIHIL;
    }

    /* Creare in memoria */
    entitas = entitas_creare(data->piscina, id_internatum, genus_internatum);
    si (!entitas)
    {
        redde NIHIL;
    }

    si (!tabula_dispersa_inserere(data->entitates, *id_internatum, entitas))
    {
        redde NIHIL;
    }

    /* Assecurare genus hierarchiam et addere "est" relationem */
    genus_folium = _assecurare_genus_hierarchiam(data, genus);
    si (genus_folium)
    {
        /* Addere "est" relationem: entitas --[est]--> Genus::X */
        _impl_relatio_addere(data, entitas, "est", genus_folium->id);
    }

    redde entitas;
}

interior b32
_impl_entitas_delere(
    vacuum* datum,
    chorda* id)
{
    RepositoriumData* data;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !id)
    {
        redde FALSUM;
    }

    /* Obtinere entitas pro genus */
    {
        Entitas* entitas_temp;
        si (!tabula_dispersa_invenire(data->entitates, *id, (vacuum**)&entitas_temp))
        {
            redde FALSUM;
        }

        /* Emit event */
        e = eventum_delere_entitas(data->piscina, id, entitas_temp->genus);
    }
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Delere ex memoria */
    redde tabula_dispersa_delere(data->entitates, *id);
}

interior b32
_impl_proprietas_ponere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* clavis,
    constans character* valor)
{
    RepositoriumData* data;
    chorda*           clavis_internata;
    chorda*           valor_internatus;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    clavis_internata = chorda_internare_ex_literis(data->intern, clavis);
    si (!clavis_internata)
    {
        redde FALSUM;
    }

    valor_internatus = chorda_internare_ex_literis(data->intern, valor);
    si (!valor_internatus)
    {
        redde FALSUM;
    }

    /* Emit event */
    e = eventum_ponere_proprietas(data->piscina, entitas->id, entitas->genus,
                                   clavis_internata, valor_internatus);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Update in memoria */
    redde entitas_proprietas_ponere(entitas, clavis_internata, valor_internatus);
}

interior b32
_impl_proprietas_delere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* clavis)
{
    RepositoriumData* data;
    chorda*           clavis_internata;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !clavis)
    {
        redde FALSUM;
    }

    clavis_internata = chorda_internare_ex_literis(data->intern, clavis);
    si (!clavis_internata)
    {
        redde FALSUM;
    }

    /* Emit event */
    e = eventum_delere_proprietas(data->piscina, entitas->id, entitas->genus,
                                   clavis_internata);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Update in memoria */
    redde entitas_proprietas_delere(entitas, clavis_internata);
}

interior Relatio*
_impl_relatio_addere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* genus,
    chorda*             destinatio_id)
{
    RepositoriumData* data;
    chorda            relatio_uuid;
    chorda*           relatio_id;
    chorda*           genus_internatum;
    Relatio*          relatio;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !genus || !destinatio_id)
    {
        redde NIHIL;
    }

    /* Generare relatio ID */
    relatio_uuid = uuidv7_creare(data->piscina);
    si (!relatio_uuid.datum)
    {
        redde NIHIL;
    }

    relatio_id = chorda_internare(data->intern, relatio_uuid);
    si (!relatio_id)
    {
        redde NIHIL;
    }

    genus_internatum = chorda_internare_ex_literis(data->intern, genus);
    si (!genus_internatum)
    {
        redde NIHIL;
    }

    /* Emit event */
    e = eventum_addere_relatio(data->piscina, entitas->id, entitas->genus,
                               relatio_id, genus_internatum, destinatio_id);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde NIHIL;
    }

    /* Addere in memoria */
    relatio = entitas_relatio_addere_cum_id(
        entitas,
        data->piscina,
        data->intern,
        relatio_id,
        genus_internatum,
        destinatio_id);

    si (!relatio)
    {
        redde NIHIL;
    }

    /* Registrare in index */
    tabula_dispersa_inserere(data->relationes, *relatio_id, relatio);

    redde relatio;
}

interior b32
_impl_relatio_delere(
    vacuum* datum,
    chorda* relatio_id)
{
    RepositoriumData* data;
    TabulaIterator    iter;
    chorda            clavis;
    vacuum*           valor;
    Entitas*          entitas;
    Entitas*          entitas_origo;
    Relatio*          relatio;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !relatio_id)
    {
        redde FALSUM;
    }

    /* Quaerere relatio pro obtinere entitas origo */
    si (!tabula_dispersa_invenire(data->relationes, *relatio_id, (vacuum**)&relatio))
    {
        redde FALSUM;
    }

    /* Quaerere entitas origo */
    si (!tabula_dispersa_invenire(data->entitates, *relatio->origo_id, (vacuum**)&entitas_origo))
    {
        redde FALSUM;
    }

    /* Emit event */
    e = eventum_delere_relatio(data->piscina, entitas_origo->id,
                                entitas_origo->genus, relatio_id);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Deregistrare ex index */
    tabula_dispersa_delere(data->relationes, *relatio_id);

    /* Quaerere entitatem cum hac relatione et delere */
    iter = tabula_dispersa_iterator_initium(data->entitates);
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
    RepositoriumData* data;
    chorda*           nota_internata;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !nota)
    {
        redde FALSUM;
    }

    nota_internata = chorda_internare_ex_literis(data->intern, nota);
    si (!nota_internata)
    {
        redde FALSUM;
    }

    /* Emit event */
    e = eventum_addere_nota(data->piscina, entitas->id, entitas->genus, nota_internata);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Update in memoria */
    redde entitas_nota_addere(entitas, nota_internata);
}

interior b32
_impl_nota_delere(
    vacuum*             datum,
    Entitas*            entitas,
    constans character* nota)
{
    RepositoriumData* data;
    chorda*           nota_internata;
    Eventum*          e;

    data = (RepositoriumData*)datum;

    si (!data || !entitas || !nota)
    {
        redde FALSUM;
    }

    nota_internata = chorda_internare_ex_literis(data->intern, nota);
    si (!nota_internata)
    {
        redde FALSUM;
    }

    /* Emit event */
    e = eventum_delere_nota(data->piscina, entitas->id, entitas->genus, nota_internata);
    si (!e || !data->persistentia->scribere_eventum(data->persistentia->datum, e))
    {
        redde FALSUM;
    }

    /* Update in memoria */
    redde entitas_nota_delere(entitas, nota_internata);
}


/* ==================================================
 * Replay Functions (rebuild state from events)
 * ================================================== */

interior b32
_applicare_eventum(
    RepositoriumData* data,
    Eventum*          eventum)
{
    chorda*  id_internatum;
    Entitas* entitas;

    si (!data || !eventum)
    {
        redde FALSUM;
    }

    /* Internare entitas_id */
    id_internatum = chorda_internare(data->intern, *eventum->entitas_id);
    si (!id_internatum)
    {
        redde FALSUM;
    }

    commutatio (eventum->genus)
    {
        casus EVENTUS_CREARE_ENTITAS:
        {
            chorda* genus_internatum;

            si (!eventum->entitas_genus)
            {
                redde FALSUM;
            }

            genus_internatum = chorda_internare(data->intern, *eventum->entitas_genus);
            si (!genus_internatum)
            {
                redde FALSUM;
            }

            entitas = entitas_creare(data->piscina, id_internatum, genus_internatum);
            si (!entitas)
            {
                redde FALSUM;
            }

            redde tabula_dispersa_inserere(data->entitates, *id_internatum, entitas);
        }

        casus EVENTUS_DELERE_ENTITAS:
            redde tabula_dispersa_delere(data->entitates, *id_internatum);

        casus EVENTUS_PONERE_PROPRIETAS:
        {
            chorda* clavis_internata;
            chorda* valor_internatus;

            si (!tabula_dispersa_invenire(data->entitates, *id_internatum,
                                          (vacuum**)&entitas))
            {
                redde FALSUM;
            }

            clavis_internata = chorda_internare(data->intern,
                                                *eventum->datum.proprietas.clavis);
            valor_internatus = chorda_internare(data->intern,
                                                *eventum->datum.proprietas.valor);

            si (!clavis_internata || !valor_internatus)
            {
                redde FALSUM;
            }

            redde entitas_proprietas_ponere(entitas, clavis_internata, valor_internatus);
        }

        casus EVENTUS_DELERE_PROPRIETAS:
        {
            chorda* clavis_internata;

            si (!tabula_dispersa_invenire(data->entitates, *id_internatum,
                                          (vacuum**)&entitas))
            {
                redde FALSUM;
            }

            clavis_internata = chorda_internare(data->intern,
                                                *eventum->datum.proprietas.clavis);
            si (!clavis_internata)
            {
                redde FALSUM;
            }

            redde entitas_proprietas_delere(entitas, clavis_internata);
        }

        casus EVENTUS_ADDERE_RELATIO:
        {
            chorda*  relatio_id;
            chorda*  genus_internatum;
            chorda*  dest_id;
            Relatio* relatio;

            si (!tabula_dispersa_invenire(data->entitates, *id_internatum,
                                          (vacuum**)&entitas))
            {
                redde FALSUM;
            }

            relatio_id = chorda_internare(data->intern,
                                          *eventum->datum.relatio.relatio_id);
            genus_internatum = chorda_internare(data->intern,
                                                *eventum->datum.relatio.relatio_genus);
            dest_id = chorda_internare(data->intern,
                                       *eventum->datum.relatio.destinatio_id);

            si (!relatio_id || !genus_internatum || !dest_id)
            {
                redde FALSUM;
            }

            relatio = entitas_relatio_addere_cum_id(
                entitas, data->piscina, data->intern,
                relatio_id, genus_internatum, dest_id);

            si (!relatio)
            {
                redde FALSUM;
            }

            redde tabula_dispersa_inserere(data->relationes, *relatio_id, relatio);
        }

        casus EVENTUS_DELERE_RELATIO:
        {
            chorda*        relatio_id;
            TabulaIterator iter;
            chorda         clavis;
            vacuum*        valor;

            relatio_id = chorda_internare(data->intern,
                                          *eventum->datum.relatio.relatio_id);
            si (!relatio_id)
            {
                redde FALSUM;
            }

            tabula_dispersa_delere(data->relationes, *relatio_id);

            /* Quaerere et delere ex entitate */
            iter = tabula_dispersa_iterator_initium(data->entitates);
            dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
            {
                entitas = (Entitas*)valor;
                si (entitas_relatio_delere(entitas, relatio_id))
                {
                    redde VERUM;
                }
            }
            redde VERUM;
        }

        casus EVENTUS_ADDERE_NOTA:
        {
            chorda* nota_internata;

            si (!tabula_dispersa_invenire(data->entitates, *id_internatum,
                                          (vacuum**)&entitas))
            {
                redde FALSUM;
            }

            nota_internata = chorda_internare(data->intern,
                                              *eventum->datum.nota.nota);
            si (!nota_internata)
            {
                redde FALSUM;
            }

            redde entitas_nota_addere(entitas, nota_internata);
        }

        casus EVENTUS_DELERE_NOTA:
        {
            chorda* nota_internata;

            si (!tabula_dispersa_invenire(data->entitates, *id_internatum,
                                          (vacuum**)&entitas))
            {
                redde FALSUM;
            }

            nota_internata = chorda_internare(data->intern,
                                              *eventum->datum.nota.nota);
            si (!nota_internata)
            {
                redde FALSUM;
            }

            redde entitas_nota_delere(entitas, nota_internata);
        }

        ordinarius:
            redde FALSUM;
    }
}

interior b32
_replay_eventus(
    RepositoriumData* data)
{
    Xar*     eventus;
    Eventum* e;
    i32      i;
    i32      numerus;

    /* Legere omnes eventus ex persistentia */
    eventus = data->persistentia->legere_eventus(data->persistentia->datum,
                                                  data->piscina);
    si (!eventus)
    {
        redde VERUM;  /* No events - OK */
    }

    /* Applicare each */
    numerus = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        e = *(Eventum**)xar_obtinere(eventus, i);
        si (e)
        {
            _applicare_eventum(data, e);
        }
    }

    redde VERUM;
}


/* ==================================================
 * Event Log Read Functions
 * ================================================== */

interior Xar*
_impl_legere_omnes_eventus(
    vacuum* datum)
{
    RepositoriumData* data;

    data = (RepositoriumData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    redde data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
}

interior i32
_impl_numerus_eventorum(
    vacuum* datum)
{
    RepositoriumData* data;
    Xar*              eventus;

    data = (RepositoriumData*)datum;

    si (!data)
    {
        redde ZEPHYRUM;
    }

    eventus = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!eventus)
    {
        redde ZEPHYRUM;
    }

    redde xar_numerus(eventus);
}

interior Xar*
_impl_legere_eventus_entitatis(
    vacuum* datum,
    chorda* entitas_id)
{
    RepositoriumData* data;
    Xar*              omnes;
    Xar*              resultus;
    Eventum*          e;
    Eventum**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data || !entitas_id)
    {
        redde NIHIL;
    }

    omnes = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!omnes)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(omnes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        e = *(Eventum**)xar_obtinere(omnes, i);
        si (e && e->entitas_id && chorda_aequalis(*e->entitas_id, *entitas_id))
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = e;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_legere_eventus_post_indicem(
    vacuum* datum,
    i32     index)
{
    RepositoriumData* data;
    Xar*              omnes;
    Xar*              resultus;
    Eventum*          e;
    Eventum**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    omnes = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!omnes)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(omnes);
    per (i = index; i < numerus; i++)
    {
        si (i < ZEPHYRUM)
        {
            perge;
        }

        e = *(Eventum**)xar_obtinere(omnes, i);
        si (e)
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = e;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_legere_eventus_recentes(
    vacuum* datum,
    i32     numerus_petitus)
{
    RepositoriumData* data;
    Xar*              omnes;
    Xar*              resultus;
    Eventum*          e;
    Eventum**         slot;
    i32               i;
    i32               numerus_totalis;
    i32               initium;

    data = (RepositoriumData*)datum;

    si (!data || numerus_petitus <= ZEPHYRUM)
    {
        redde NIHIL;
    }

    omnes = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!omnes)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus_totalis = xar_numerus(omnes);
    initium = numerus_totalis - numerus_petitus;
    si (initium < ZEPHYRUM)
    {
        initium = ZEPHYRUM;
    }

    per (i = initium; i < numerus_totalis; i++)
    {
        e = *(Eventum**)xar_obtinere(omnes, i);
        si (e)
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = e;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_legere_eventus_generis_entitatis(
    vacuum*             datum,
    constans character* genus_entitatis)
{
    RepositoriumData* data;
    Xar*              omnes;
    Xar*              resultus;
    Eventum*          e;
    Eventum**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data || !genus_entitatis)
    {
        redde NIHIL;
    }

    omnes = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!omnes)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(omnes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        e = *(Eventum**)xar_obtinere(omnes, i);
        si (e && e->entitas_genus &&
            chorda_aequalis_literis(*e->entitas_genus, genus_entitatis))
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = e;
            }
        }
    }

    redde resultus;
}

interior Xar*
_impl_legere_eventus_typi(
    vacuum*      datum,
    EventusGenus typus)
{
    RepositoriumData* data;
    Xar*              omnes;
    Xar*              resultus;
    Eventum*          e;
    Eventum**         slot;
    i32               i;
    i32               numerus;

    data = (RepositoriumData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    omnes = data->persistentia->legere_eventus(data->persistentia->datum, data->piscina);
    si (!omnes)
    {
        redde NIHIL;
    }

    resultus = xar_creare(data->piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(omnes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        e = *(Eventum**)xar_obtinere(omnes, i);
        si (e && e->genus == typus)
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = e;
            }
        }
    }

    redde resultus;
}


/* ==================================================
 * Factory Function
 * ================================================== */

EntitasRepositorium*
entitas_repositorium_creare(
    Piscina*      piscina,
    Persistentia* persistentia)
{
    EntitasRepositorium* repo;
    RepositoriumData*    data;

    si (!piscina || !persistentia)
    {
        redde NIHIL;
    }

    /* Allocare structuras */
    repo = (EntitasRepositorium*)piscina_allocare(piscina,
                                                   magnitudo(EntitasRepositorium));
    si (!repo)
    {
        redde NIHIL;
    }

    data = (RepositoriumData*)piscina_allocare(piscina,
                                                magnitudo(RepositoriumData));
    si (!data)
    {
        redde NIHIL;
    }

    data->piscina = piscina;
    data->persistentia = persistentia;

    /* Usare internamentum globale pro chordas internatae */
    data->intern = internamentum_globale();
    si (!data->intern)
    {
        redde NIHIL;
    }

    /* Creare hash tables */
    data->entitates = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (!data->entitates)
    {
        redde NIHIL;
    }

    data->relationes = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (!data->relationes)
    {
        redde NIHIL;
    }

    /* Replay events to rebuild state */
    si (!_replay_eventus(data))
    {
        redde NIHIL;
    }

    /* Wire up interface */
    repo->datum = data;

    repo->capere_entitatem = _impl_capere_entitatem;
    repo->capere_entitates_relatae = _impl_capere_entitates_relatae;
    repo->quaerere_cum_nota = _impl_quaerere_cum_nota;
    repo->quaerere_cum_praefixo_notae = _impl_quaerere_cum_praefixo_notae;
    repo->quaerere_textum = _impl_quaerere_textum;
    repo->capere_radices = _impl_capere_radices;
    repo->capere_relatio = _impl_capere_relatio;
    repo->capere_relationes_ad = _impl_capere_relationes_ad;

    repo->entitas_creare = _impl_entitas_creare;
    repo->entitas_scaffoldare = _impl_entitas_scaffoldare;
    repo->entitas_delere = _impl_entitas_delere;
    repo->proprietas_ponere = _impl_proprietas_ponere;
    repo->proprietas_delere = _impl_proprietas_delere;
    repo->relatio_addere = _impl_relatio_addere;
    repo->relatio_delere = _impl_relatio_delere;
    repo->nota_addere = _impl_nota_addere;
    repo->nota_delere = _impl_nota_delere;

    /* Event log read functions */
    repo->legere_omnes_eventus = _impl_legere_omnes_eventus;
    repo->numerus_eventorum = _impl_numerus_eventorum;
    repo->legere_eventus_entitatis = _impl_legere_eventus_entitatis;
    repo->legere_eventus_post_indicem = _impl_legere_eventus_post_indicem;
    repo->legere_eventus_recentes = _impl_legere_eventus_recentes;
    repo->legere_eventus_generis_entitatis = _impl_legere_eventus_generis_entitatis;
    repo->legere_eventus_typi = _impl_legere_eventus_typi;

    redde repo;
}


/* ==================================================
 * Validation Functions (Schema-Aware)
 * ================================================== */

Entitas*
entitas_repositorium_proprietas_definitio_invenire(
    EntitasRepositorium* repo,
    chorda*              entitas_genus,
    chorda*              proprietas_nomen)
{
    si (!repo || !entitas_genus || !proprietas_nomen)
    {
        redde NIHIL;
    }

    redde _impl_proprietas_definitio_invenire(
        repo->datum,
        entitas_genus,
        proprietas_nomen);
}

b32
entitas_repositorium_proprietas_validare(
    EntitasRepositorium* repo,
    Entitas*             entitas,
    chorda*              clavis)
{
    RepositoriumData* data;
    Entitas*          definitio;
    Entitas*          typus_semanticus_ent;
    Proprietas*       prop;
    chorda*           typus_literalis_str;
    chorda*           typus_semanticus_nomen;
    chorda*           est_genus;
    Relatio*          rel;
    TypusLiteralis    typus_expected;
    b32               parse_success;
    i32               i;
    i32               numerus_rel;

    si (!repo || !entitas || !clavis)
    {
        redde FALSUM;
    }

    data = (RepositoriumData*)repo->datum;

    /* Invenire PropertyDefinition pro hac proprietate */
    definitio = _impl_proprietas_definitio_invenire(
        repo->datum,
        entitas->genus,
        clavis);

    /* Si nullum schema, nullum validationem - semper validum */
    si (!definitio)
    {
        redde VERUM;
    }

    /* Capere proprietatem ab entitate */
    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop)
    {
        /* Proprietas non existit - validum (non required) */
        redde VERUM;
    }

    /* Sequi relatio "est" ad TypusSemanticus entitas
     * (non ad Genus - omnes entitates habent "est" ad suum Genus)
     */
    est_genus = chorda_internare_ex_literis(data->intern, "est");
    typus_semanticus_ent = NIHIL;

    numerus_rel = xar_numerus(definitio->relationes);
    per (i = ZEPHYRUM; i < numerus_rel; i++)
    {
        Entitas* dest_ent;

        rel = (Relatio*)xar_obtinere(definitio->relationes, i);
        si (rel && rel->genus == est_genus)
        {
            dest_ent = _impl_capere_entitatem(repo->datum, rel->destinatio_id);
            /* Verificare destinatio est TypusSemanticus, non Genus */
            si (dest_ent && dest_ent->genus &&
                chorda_aequalis_literis(*dest_ent->genus, "TypusSemanticus"))
            {
                typus_semanticus_ent = dest_ent;
                frange;
            }
        }
    }

    /* Si nulla relatio "est", nullum typum - validum */
    si (!typus_semanticus_ent)
    {
        redde VERUM;
    }

    /* Capere typus_literalis ex TypusSemanticus entitas */
    typus_literalis_str = entitas_proprietas_capere(
        typus_semanticus_ent,
        chorda_internare_ex_literis(data->intern, "typus_literalis"));

    si (!typus_literalis_str)
    {
        /* Nullum typus definitus in TypusSemanticus - validum */
        redde VERUM;
    }

    typus_expected = typus_literalis_ex_chorda(*typus_literalis_str);
    si (typus_expected == TYPUS_NIHIL || typus_expected == TYPUS_CHORDA)
    {
        /* Generic string type - semper validum */
        redde VERUM;
    }

    /* Tentare parsing secundum expected type */
    parse_success = proprietas_parsare_ut_typum(prop, typus_expected);

    /* Set semantic type ex TypusSemanticus titulum (slug ut nomen) */
    si (parse_success)
    {
        typus_semanticus_nomen = entitas_titulum_capere(typus_semanticus_ent);
        si (typus_semanticus_nomen)
        {
            prop->typus_semanticus = typus_semanticus_nomen;
        }
    }

    /* TODO: Si parse_success == FALSUM, creare ValidationError entity */

    redde parse_success;
}

b32
entitas_repositorium_validare(
    EntitasRepositorium* repo,
    Entitas*             entitas)
{
    Proprietas* prop;
    i32         i;
    i32         numerus;
    b32         omnia_valida;

    si (!repo || !entitas)
    {
        redde FALSUM;
    }

    omnia_valida = VERUM;
    numerus = xar_numerus(entitas->proprietates);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (!prop || !prop->clavis)
        {
            perge;
        }

        si (!entitas_repositorium_proprietas_validare(repo, entitas, prop->clavis))
        {
            omnia_valida = FALSUM;
            /* Perge validare ceteras - colligere omnes errores */
        }
    }

    redde omnia_valida;
}
