#include "entitas.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Creatio
 * ================================================== */

Entitas*
entitas_creare(
    Piscina* piscina,
    chorda*  id,
    chorda*  genus)
{
    Entitas* entitas;

    si (!piscina || !id || !genus)
    {
        redde NIHIL;
    }

    /* Allocare structuram entitatis */
    entitas = (Entitas*)piscina_allocare(piscina, magnitudo(Entitas));
    si (!entitas)
    {
        redde NIHIL;
    }

    /* Ponere campos */
    entitas->id    = id;
    entitas->genus = genus;

    /* Creare Xars pro proprietatibus, relationibus, notis */
    entitas->proprietates = xar_creare(piscina, magnitudo(Proprietas));
    si (!entitas->proprietates)
    {
        redde NIHIL;
    }

    entitas->relationes = xar_creare(piscina, magnitudo(Relatio));
    si (!entitas->relationes)
    {
        redde NIHIL;
    }

    entitas->notae = xar_creare(piscina, magnitudo(chorda*));
    si (!entitas->notae)
    {
        redde NIHIL;
    }

    redde entitas;
}


/* ==================================================
 * Proprietates
 * ================================================== */

b32
entitas_proprietas_addere(
    Entitas* entitas,
    chorda*  clavis,
    chorda*  valor)
{
    Proprietas* prop;
    i32         i;
    i32         numerus;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    /* Quaerere si clavis iam existit (renovare valorem) */
    numerus = xar_numerus(entitas->proprietates);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop && prop->clavis == clavis)  /* Aequalitas indicis */
        {
            /* Renovare valorem existentem */
            prop->valor = valor;
            redde VERUM;
        }
    }

    /* Clavis non existit - addere novam */
    prop = (Proprietas*)xar_addere(entitas->proprietates);
    si (!prop)
    {
        redde FALSUM;
    }

    prop->clavis = clavis;
    prop->valor  = valor;

    redde VERUM;
}

chorda*
entitas_proprietas_capere(
    Entitas* entitas,
    chorda*  clavis)
{
    Proprietas* prop;
    i32         i;
    i32         numerus;

    si (!entitas || !clavis)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(entitas->proprietates);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop && prop->clavis == clavis)  /* Aequalitas indicis */
        {
            redde prop->valor;
        }
    }

    redde NIHIL;  /* Non inventum */
}

b32
entitas_proprietas_habet(
    Entitas* entitas,
    chorda*  clavis)
{
    redde entitas_proprietas_capere(entitas, clavis) != NIHIL;
}

b32
entitas_proprietas_delere(
    Entitas* entitas,
    chorda*  clavis)
{
    Proprietas* prop;
    Proprietas* ultima;
    i32         i;
    i32         numerus;

    si (!entitas || !clavis)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(entitas->proprietates);

    /* Quaerere proprietatem */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop && prop->clavis == clavis)  /* Aequalitas indicis */
        {
            /* Inventum! Swap-and-pop */
            si (i < numerus - I)
            {
                /* Non ultima - swap cum ultima */
                ultima = (Proprietas*)xar_obtinere(entitas->proprietates, numerus - I);
                si (ultima)
                {
                    *prop = *ultima;
                }
            }
            /* Truncare */
            xar_truncare(entitas->proprietates, numerus - I);
            redde VERUM;
        }
    }

    redde FALSUM;  /* Non inventum */
}


/* ==================================================
 * Relationes
 * ================================================== */

Relatio*
entitas_relatio_addere(
    Entitas*             entitas,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              genus,
    chorda*              destinatio_id)
{
    chorda   id_chorda;
    chorda*  id_interned;

    si (!entitas || !piscina || !intern || !genus || !destinatio_id)
    {
        redde NIHIL;
    }

    /* Generare UUIDv7 pro ID relationis */
    id_chorda = uuidv7_creare(piscina);
    si (id_chorda.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Internare ID */
    id_interned = chorda_internare(intern, id_chorda);
    si (!id_interned)
    {
        redde NIHIL;
    }

    /* Delegare ad entitas_relatio_addere_cum_id */
    redde entitas_relatio_addere_cum_id(entitas, piscina, intern,
                                         id_interned, genus, destinatio_id);
}

Relatio*
entitas_relatio_addere_cum_id(
    Entitas*             entitas,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              relatio_id,
    chorda*              genus,
    chorda*              destinatio_id)
{
    Relatio* relatio;

    (vacuum)piscina;  /* Non usatum */
    (vacuum)intern;   /* Non usatum */

    si (!entitas || !relatio_id || !genus || !destinatio_id)
    {
        redde NIHIL;
    }

    /* Addere relationem */
    relatio = (Relatio*)xar_addere(entitas->relationes);
    si (!relatio)
    {
        redde NIHIL;
    }

    relatio->id            = relatio_id;
    relatio->origo_id      = entitas->id;
    relatio->genus         = genus;
    relatio->destinatio_id = destinatio_id;

    redde relatio;
}

Relatio*
entitas_relatio_capere(
    Entitas* entitas,
    chorda*  relatio_id)
{
    Relatio* relatio;
    i32      i;
    i32      numerus;

    si (!entitas || !relatio_id)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (relatio && relatio->id == relatio_id)  /* Aequalitas indicis */
        {
            redde relatio;
        }
    }

    redde NIHIL;  /* Non inventum */
}

Xar*
entitas_relationes_generis_capere(
    Entitas* entitas,
    chorda*  genus,
    Piscina* piscina)
{
    Relatio* relatio;
    Relatio* nova;
    Xar*     resultus;
    i32      i;
    i32      numerus;

    si (!entitas || !genus || !piscina)
    {
        redde NIHIL;
    }

    /* Creare Xar pro resultis */
    resultus = xar_creare(piscina, magnitudo(Relatio));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Filtrare relationes per genus */
    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (relatio && relatio->genus == genus)  /* Aequalitas indicis */
        {
            nova = (Relatio*)xar_addere(resultus);
            si (!nova)
            {
                redde NIHIL;
            }
            *nova = *relatio;
        }
    }

    redde resultus;
}

b32
entitas_relatio_delere(
    Entitas* entitas,
    chorda*  relatio_id)
{
    Relatio* relatio;
    Relatio* ultima;
    i32      i;
    i32      numerus;

    si (!entitas || !relatio_id)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(entitas->relationes);

    /* Quaerere relationem per ID */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (relatio && relatio->id == relatio_id)  /* Aequalitas indicis */
        {
            /* Inventum! Swap-and-pop */
            si (i < numerus - I)
            {
                /* Non ultima - swap cum ultima */
                ultima = (Relatio*)xar_obtinere(entitas->relationes, numerus - I);
                si (ultima)
                {
                    *relatio = *ultima;
                }
            }
            /* Truncare */
            xar_truncare(entitas->relationes, numerus - I);
            redde VERUM;
        }
    }

    redde FALSUM;  /* Non inventum */
}


/* ==================================================
 * Notae (Tags)
 * ================================================== */

b32
entitas_nota_addere(
    Entitas* entitas,
    chorda*  nota)
{
    chorda** slot;

    si (!entitas || !nota)
    {
        redde FALSUM;
    }

    /* Verificare si iam habet (evitare duplicatas) */
    si (entitas_nota_habet(entitas, nota))
    {
        redde VERUM;  /* Iam habet - successus */
    }

    /* Addere notam */
    slot = (chorda**)xar_addere(entitas->notae);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot = nota;
    redde VERUM;
}

b32
entitas_nota_habet(
    Entitas* entitas,
    chorda*  nota)
{
    chorda** slot;
    i32      i;
    i32      numerus;

    si (!entitas || !nota)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(entitas->notae);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        slot = (chorda**)xar_obtinere(entitas->notae, i);
        si (slot && *slot == nota)  /* Aequalitas indicis */
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
entitas_nota_cum_praefixo_habet(
    Entitas*             entitas,
    constans character*  praefixum)
{
    chorda** slot;
    chorda*  nota;
    i32      i;
    i32      numerus;
    i32      mensura_praefixum;
    i32      j;

    si (!entitas || !praefixum)
    {
        redde FALSUM;
    }

    mensura_praefixum = (i32)strlen(praefixum);
    numerus = xar_numerus(entitas->notae);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        slot = (chorda**)xar_obtinere(entitas->notae, i);
        si (!slot || !*slot)
        {
            perge;
        }

        nota = *slot;

        /* Verificare si nota commencitur cum praefixo */
        si (nota->mensura < mensura_praefixum)
        {
            perge;  /* Nota nimis brevis */
        }

        /* Comparare characteres praefixum */
        per (j = ZEPHYRUM; j < mensura_praefixum; j++)
        {
            si (nota->datum[j] != (i8)praefixum[j])
            {
                frange;  /* Non aequalis */
            }
        }

        /* Si pervenit ad finem praefixum, aequalis est */
        si (j == mensura_praefixum)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
entitas_nota_delere(
    Entitas* entitas,
    chorda*  nota)
{
    chorda** slot;
    chorda** ultima;
    i32      i;
    i32      numerus;

    si (!entitas || !nota)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(entitas->notae);

    /* Quaerere notam */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        slot = (chorda**)xar_obtinere(entitas->notae, i);
        si (slot && *slot == nota)  /* Aequalitas indicis */
        {
            /* Inventum! Swap-and-pop */
            si (i < numerus - I)
            {
                /* Non ultima - swap cum ultima */
                ultima = (chorda**)xar_obtinere(entitas->notae, numerus - I);
                si (ultima)
                {
                    *slot = *ultima;
                }
            }
            /* Truncare */
            xar_truncare(entitas->notae, numerus - I);
            redde VERUM;
        }
    }

    redde FALSUM;  /* Non inventum */
}


/* ==================================================
 * Utilitas
 * ================================================== */

chorda*
entitas_titulum_capere(
    Entitas* entitas)
{
    i32         i;
    i32         numerus;
    Proprietas* prop;

    si (!entitas)
    {
        redde NIHIL;
    }

    /* Primo: quaerere "name" */
    numerus = xar_numerus(entitas->proprietates);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop && prop->clavis && prop->clavis->datum &&
            chorda_aequalis_literis(*prop->clavis, "name"))
        {
            si (prop->valor && prop->valor->datum)
            {
                redde prop->valor;
            }
        }
    }

    /* Secundo: quaerere "title" */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop && prop->clavis && prop->clavis->datum &&
            chorda_aequalis_literis(*prop->clavis, "title"))
        {
            si (prop->valor && prop->valor->datum)
            {
                redde prop->valor;
            }
        }
    }

    /* Tertio: redde id */
    redde entitas->id;
}

i32
entitas_numerus_proprietatum(
    Entitas* entitas)
{
    si (!entitas || !entitas->proprietates)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(entitas->proprietates);
}

i32
entitas_numerus_relationum(
    Entitas* entitas)
{
    si (!entitas || !entitas->relationes)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(entitas->relationes);
}

i32
entitas_numerus_notarum(
    Entitas* entitas)
{
    si (!entitas || !entitas->notae)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(entitas->notae);
}

vacuum
entitas_imprimere(
    Entitas* entitas)
{
    Proprietas* prop;
    Relatio*    relatio;
    chorda**    nota_slot;
    i32         i;
    i32         numerus;

    si (!entitas)
    {
        imprimere("Entitas: NIHIL\n");
        redde;
    }

    imprimere("Entitas:\n");
    imprimere("  ID: %.*s\n", entitas->id->mensura, entitas->id->datum);
    imprimere("  Genus: %.*s\n", entitas->genus->mensura, entitas->genus->datum);

    /* Proprietates */
    imprimere("  Proprietates (%d):\n", entitas_numerus_proprietatum(entitas));
    numerus = xar_numerus(entitas->proprietates);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (prop)
        {
            imprimere("    %.*s = %.*s\n",
                      prop->clavis->mensura, prop->clavis->datum,
                      prop->valor->mensura, prop->valor->datum);
        }
    }

    /* Relationes */
    imprimere("  Relationes (%d):\n", entitas_numerus_relationum(entitas));
    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        relatio = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (relatio)
        {
            imprimere("    [%.*s] -[%.*s]-> %.*s\n",
                      relatio->id->mensura, relatio->id->datum,
                      relatio->genus->mensura, relatio->genus->datum,
                      relatio->destinatio_id->mensura, relatio->destinatio_id->datum);
        }
    }

    /* Notae */
    imprimere("  Notae (%d):\n", entitas_numerus_notarum(entitas));
    numerus = xar_numerus(entitas->notae);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        nota_slot = (chorda**)xar_obtinere(entitas->notae, i);
        si (nota_slot && *nota_slot)
        {
            imprimere("    %.*s\n", (*nota_slot)->mensura, (*nota_slot)->datum);
        }
    }
}
