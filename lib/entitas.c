#include "entitas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* strtod, strtol */
#include <errno.h>   /* ERANGE */
#include <limits.h>  /* INT_MIN, INT_MAX */


/* ==================================================
 * Parsing Helpers (Interior)
 * ================================================== */

/* Parsare chorda ad s32
 * Redde: VERUM si successus, FALSUM si error
 */
interior b32
_parsare_s32(
    chorda  valor,
    s32*    resultus)
{
    character buffer[64];
    character* endptr;
    longus     val;
    i32        len;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    /* Copiare ad null-terminated buffer */
    len = valor.mensura;
    si (len > 63)
    {
        len = 63;
    }
    memcpy(buffer, valor.datum, (memoriae_index)len);
    buffer[len] = '\0';

    /* Parsare */
    errno = 0;
    val = strtol(buffer, &endptr, 10);

    /* Verificare errores */
    si (errno == ERANGE || val < INT_MIN || val > INT_MAX)
    {
        redde FALSUM;  /* Overflow */
    }

    si (endptr == buffer || *endptr != '\0')
    {
        redde FALSUM;  /* Non numerus validus */
    }

    *resultus = (s32)val;
    redde VERUM;
}

/* Parsare chorda ad s64 (manual parser pro C89 compatibility) */
interior b32
_parsare_s64(
    chorda  valor,
    s64*    resultus)
{
    s64 val;
    s64 sign;
    i32 i;
    i32 start;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    val  = 0;
    sign = 1;
    i    = 0;

    /* Saltare whitespace */
    dum (i < valor.mensura &&
         (valor.datum[i] == ' ' || valor.datum[i] == '\t'))
    {
        i++;
    }

    /* Verificare signum */
    si (i < valor.mensura && valor.datum[i] == '-')
    {
        sign = -1;
        i++;
    }
    alioquin si (i < valor.mensura && valor.datum[i] == '+')
    {
        i++;
    }

    /* Verificare si aliquid ad parsare */
    start = i;
    si (i >= valor.mensura)
    {
        redde FALSUM;
    }

    /* Parsare digitos */
    dum (i < valor.mensura)
    {
        character c;
        c = (character)valor.datum[i];

        si (c >= '0' && c <= '9')
        {
            /* Verificare overflow ante multiplicare */
            si (val > 922337203685477580LL ||
                (val == 922337203685477580LL && (c - '0') > 7))
            {
                redde FALSUM;  /* Overflow */
            }
            val = val * 10 + (s64)(c - '0');
            i++;
        }
        alioquin
        {
            frange;  /* Non digitus */
        }
    }

    /* Verificare si aliquid parsatum */
    si (i == start)
    {
        redde FALSUM;  /* Nullus digitus */
    }

    /* Verificare si trailing garbage */
    si (i < valor.mensura)
    {
        redde FALSUM;  /* Characters post numerum */
    }

    *resultus = val * sign;
    redde VERUM;
}

/* Parsare chorda ad f64 */
interior b32
_parsare_f64(
    chorda  valor,
    f64*    resultus)
{
    character  buffer[128];
    character* endptr;
    f64        val;
    i32        len;

    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    /* Copiare ad null-terminated buffer */
    len = valor.mensura;
    si (len > 127)
    {
        len = 127;
    }
    memcpy(buffer, valor.datum, (memoriae_index)len);
    buffer[len] = '\0';

    /* Parsare */
    errno = 0;
    val = strtod(buffer, &endptr);

    /* Verificare errores */
    si (errno == ERANGE)
    {
        redde FALSUM;  /* Overflow/underflow */
    }

    si (endptr == buffer || *endptr != '\0')
    {
        redde FALSUM;  /* Non numerus validus */
    }

    *resultus = val;
    redde VERUM;
}

/* Parsare chorda ad b32
 * Accepta: "true", "false", "1", "0", "verum", "falsum"
 */
interior b32
_parsare_b32(
    chorda  valor,
    b32*    resultus)
{
    si (!valor.datum || valor.mensura == ZEPHYRUM || !resultus)
    {
        redde FALSUM;
    }

    /* Verificare "true" / "verum" / "1" */
    si (chorda_aequalis_literis(valor, "true") ||
        chorda_aequalis_literis(valor, "verum") ||
        chorda_aequalis_literis(valor, "1"))
    {
        *resultus = VERUM;
        redde VERUM;
    }

    /* Verificare "false" / "falsum" / "0" */
    si (chorda_aequalis_literis(valor, "false") ||
        chorda_aequalis_literis(valor, "falsum") ||
        chorda_aequalis_literis(valor, "0"))
    {
        *resultus = FALSUM;
        redde VERUM;
    }

    redde FALSUM;  /* Valor non cognitus */
}

/* Parsare chorda ad tempus (s64 milliseconds)
 * Accepta: raw millisecond string, vel ISO 8601 (futuro)
 */
interior b32
_parsare_tempus(
    chorda  valor,
    s64*    resultus)
{
    /* Pro nunc, solum acceptare raw millisecond strings */
    redde _parsare_s64(valor, resultus);
}

/* Convertere chorda typus literalis ad enum
 * Accepta: "chorda", "s32", "s64", "f64", "b32", "tempus"
 * Redde: TypusLiteralis enum valor, vel TYPUS_NIHIL si non cognitus
 */
TypusLiteralis
typus_literalis_ex_chorda(
    chorda typus)
{
    si (!typus.datum || typus.mensura == ZEPHYRUM)
    {
        redde TYPUS_NIHIL;
    }

    si (chorda_aequalis_literis(typus, "chorda"))
    {
        redde TYPUS_CHORDA;
    }
    si (chorda_aequalis_literis(typus, "s32"))
    {
        redde TYPUS_S32;
    }
    si (chorda_aequalis_literis(typus, "s64"))
    {
        redde TYPUS_S64;
    }
    si (chorda_aequalis_literis(typus, "f64"))
    {
        redde TYPUS_F64;
    }
    si (chorda_aequalis_literis(typus, "b32"))
    {
        redde TYPUS_B32;
    }
    si (chorda_aequalis_literis(typus, "tempus"))
    {
        redde TYPUS_TEMPUS;
    }

    redde TYPUS_NIHIL;  /* Typus non cognitus */
}

/* Parsare proprietatem secundum typum specificatum
 * Caches result in prop->parsitus si successus
 */
b32
proprietas_parsare_ut_typum(
    Proprietas*    prop,
    TypusLiteralis typus)
{
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    commutatio (typus)
    {
        casus TYPUS_NIHIL:
        casus TYPUS_CHORDA:
            /* Chorda semper valida */
            prop->typus_literalis  = TYPUS_CHORDA;
            prop->parsitus_validus = VERUM;
            redde VERUM;

        casus TYPUS_S32:
        {
            s32 val;
            si (_parsare_s32(*prop->valor, &val))
            {
                prop->parsitus.ut_s32  = val;
                prop->typus_literalis  = TYPUS_S32;
                prop->parsitus_validus = VERUM;
                redde VERUM;
            }
            redde FALSUM;
        }

        casus TYPUS_S64:
        {
            s64 val;
            si (_parsare_s64(*prop->valor, &val))
            {
                prop->parsitus.ut_s64  = val;
                prop->typus_literalis  = TYPUS_S64;
                prop->parsitus_validus = VERUM;
                redde VERUM;
            }
            redde FALSUM;
        }

        casus TYPUS_F64:
        {
            f64 val;
            si (_parsare_f64(*prop->valor, &val))
            {
                prop->parsitus.ut_f64  = val;
                prop->typus_literalis  = TYPUS_F64;
                prop->parsitus_validus = VERUM;
                redde VERUM;
            }
            redde FALSUM;
        }

        casus TYPUS_B32:
        {
            b32 val;
            si (_parsare_b32(*prop->valor, &val))
            {
                prop->parsitus.ut_b32  = val;
                prop->typus_literalis  = TYPUS_B32;
                prop->parsitus_validus = VERUM;
                redde VERUM;
            }
            redde FALSUM;
        }

        casus TYPUS_TEMPUS:
        {
            s64 val;
            si (_parsare_tempus(*prop->valor, &val))
            {
                prop->parsitus.ut_tempus = val;
                prop->typus_literalis    = TYPUS_TEMPUS;
                prop->parsitus_validus   = VERUM;
                redde VERUM;
            }
            redde FALSUM;
        }

        ordinarius:
            redde FALSUM;
    }
}


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

    /* Proprietas definitiones cache - NIHIL initiale, populabitur later */
    entitas->proprietas_definitiones = NIHIL;

    redde entitas;
}


/* ==================================================
 * Proprietates
 * ================================================== */

b32
entitas_proprietas_ponere(
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
            /* Reset parsing state - will be re-parsed if needed */
            prop->typus_literalis  = TYPUS_NIHIL;
            prop->parsitus_validus = FALSUM;
            redde VERUM;
        }
    }

    /* Clavis non existit - addere novam */
    prop = (Proprietas*)xar_addere(entitas->proprietates);
    si (!prop)
    {
        redde FALSUM;
    }

    prop->clavis           = clavis;
    prop->valor            = valor;
    prop->typus_semanticus = NIHIL;
    prop->typus_literalis  = TYPUS_NIHIL;
    prop->parsitus_validus = FALSUM;

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

Proprietas*
entitas_proprietas_capere_plena(
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
            redde prop;
        }
    }

    redde NIHIL;  /* Non inventum */
}

b32
entitas_proprietas_capere_s32(
    Entitas* entitas,
    chorda*  clavis,
    s32*     valor)
{
    Proprietas* prop;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    /* Si iam parsitum ut s32, redde cached valor */
    si (prop->parsitus_validus && prop->typus_literalis == TYPUS_S32)
    {
        *valor = prop->parsitus.ut_s32;
        redde VERUM;
    }

    /* Tentare parsare on-demand */
    si (_parsare_s32(*prop->valor, valor))
    {
        /* Cache resultatum */
        prop->parsitus.ut_s32  = *valor;
        prop->typus_literalis  = TYPUS_S32;
        prop->parsitus_validus = VERUM;
        redde VERUM;
    }

    redde FALSUM;
}

b32
entitas_proprietas_capere_s64(
    Entitas* entitas,
    chorda*  clavis,
    s64*     valor)
{
    Proprietas* prop;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    /* Si iam parsitum ut s64, redde cached valor */
    si (prop->parsitus_validus && prop->typus_literalis == TYPUS_S64)
    {
        *valor = prop->parsitus.ut_s64;
        redde VERUM;
    }

    /* Tentare parsare on-demand */
    si (_parsare_s64(*prop->valor, valor))
    {
        /* Cache resultatum */
        prop->parsitus.ut_s64  = *valor;
        prop->typus_literalis  = TYPUS_S64;
        prop->parsitus_validus = VERUM;
        redde VERUM;
    }

    redde FALSUM;
}

b32
entitas_proprietas_capere_f64(
    Entitas* entitas,
    chorda*  clavis,
    f64*     valor)
{
    Proprietas* prop;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    /* Si iam parsitum ut f64, redde cached valor */
    si (prop->parsitus_validus && prop->typus_literalis == TYPUS_F64)
    {
        *valor = prop->parsitus.ut_f64;
        redde VERUM;
    }

    /* Tentare parsare on-demand */
    si (_parsare_f64(*prop->valor, valor))
    {
        /* Cache resultatum */
        prop->parsitus.ut_f64  = *valor;
        prop->typus_literalis  = TYPUS_F64;
        prop->parsitus_validus = VERUM;
        redde VERUM;
    }

    redde FALSUM;
}

b32
entitas_proprietas_capere_b32(
    Entitas* entitas,
    chorda*  clavis,
    b32*     valor)
{
    Proprietas* prop;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    /* Si iam parsitum ut b32, redde cached valor */
    si (prop->parsitus_validus && prop->typus_literalis == TYPUS_B32)
    {
        *valor = prop->parsitus.ut_b32;
        redde VERUM;
    }

    /* Tentare parsare on-demand */
    si (_parsare_b32(*prop->valor, valor))
    {
        /* Cache resultatum */
        prop->parsitus.ut_b32  = *valor;
        prop->typus_literalis  = TYPUS_B32;
        prop->parsitus_validus = VERUM;
        redde VERUM;
    }

    redde FALSUM;
}

b32
entitas_proprietas_capere_tempus(
    Entitas* entitas,
    chorda*  clavis,
    s64*     valor)
{
    Proprietas* prop;

    si (!entitas || !clavis || !valor)
    {
        redde FALSUM;
    }

    prop = entitas_proprietas_capere_plena(entitas, clavis);
    si (!prop || !prop->valor)
    {
        redde FALSUM;
    }

    /* Si iam parsitum ut tempus, redde cached valor */
    si (prop->parsitus_validus && prop->typus_literalis == TYPUS_TEMPUS)
    {
        *valor = prop->parsitus.ut_tempus;
        redde VERUM;
    }

    /* Tentare parsare on-demand */
    si (_parsare_tempus(*prop->valor, valor))
    {
        /* Cache resultatum */
        prop->parsitus.ut_tempus = *valor;
        prop->typus_literalis    = TYPUS_TEMPUS;
        prop->parsitus_validus   = VERUM;
        redde VERUM;
    }

    redde FALSUM;
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
