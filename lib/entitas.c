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

b32
entitas_relatio_addere(
    Entitas* entitas,
    chorda*  genus,
    chorda*  destinatio_id)
{
    Relatio* relatio;

    si (!entitas || !genus || !destinatio_id)
    {
        redde FALSUM;
    }

    relatio = (Relatio*)xar_addere(entitas->relationes);
    si (!relatio)
    {
        redde FALSUM;
    }

    relatio->genus         = genus;
    relatio->destinatio_id = destinatio_id;

    redde VERUM;
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
    i32      index)
{
    Relatio* relatio;
    Relatio* ultima;
    i32      numerus;

    si (!entitas)
    {
        redde FALSUM;
    }

    numerus = xar_numerus(entitas->relationes);

    /* Verificare index */
    si (index < ZEPHYRUM || index >= numerus)
    {
        redde FALSUM;  /* Index invalidus */
    }

    relatio = (Relatio*)xar_obtinere(entitas->relationes, index);
    si (!relatio)
    {
        redde FALSUM;
    }

    /* Swap-and-pop */
    si (index < numerus - I)
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
            imprimere("    -[%.*s]-> %.*s\n",
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
