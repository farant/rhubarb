/* router.c - Implementatio Router (HTTP Route Matching)
 *
 * Matches HTTP requests to handlers, extracts path parameters.
 */

#include "router.h"

#include <string.h>


/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* Segment in parsed path */
nomen structura {
    chorda valor;
    b32    est_param;  /* Starts with : */
} RoutaSegmentum;

/* Internal route structure */
nomen structura {
    HttpMethodus    methodus;
    chorda          via;           /* Original path */
    RoutaTipus      tipus;
    RoutaHandler    handler;
    b32             activus;

    /* Parsed segments */
    RoutaSegmentum  segmenta[ROUTER_SEGMENTA_MAXIMA];
    i32             segmenta_numerus;
} Routa;

/* Router structure */
structura Router {
    Piscina* piscina;
    Routa    routae[ROUTER_ROUTA_MAXIMA];
    i32      numerus;
};


/* ========================================================================
 * FUNCTIONES INTERNAE - HELPER
 * ======================================================================== */

/* Creare chorda ex raw memory cum copia */
interior chorda
_chorda_ex_raw(constans character* data, i32 len, Piscina* piscina)
{
    chorda temp;
    chorda res;
    i8* buffer;

    si (len <= 0 || !data || !piscina)
    {
        res.datum = NIHIL;
        res.mensura = 0;
        redde res;
    }

    /* Allocare buffer et copiare */
    buffer = (i8*)piscina_allocare(piscina, (i64)len);
    si (!buffer)
    {
        res.datum = NIHIL;
        res.mensura = 0;
        redde res;
    }

    memcpy(buffer, data, (size_t)len);
    temp.datum = buffer;
    temp.mensura = len;

    redde temp;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - PATH PARSING
 * ======================================================================== */

/* Verificare si via habet parameters */
interior b32
_via_habet_params(constans character* via)
{
    constans character* p = via;

    dum (*p)
    {
        si (*p == ':')
        {
            redde VERUM;
        }
        p++;
    }

    redde FALSUM;
}

/* Parse via in segmenta
 * Returns numerus segmentorum, vel -1 si error
 */
interior i32
_parse_via(
    constans character* via,
    RoutaSegmentum*     segmenta,
    i32                 max_segmenta,
    Piscina*            piscina)
{
    constans character* p;
    constans character* segment_start;
    i32 numerus = 0;
    i32 len;
    chorda seg;

    si (!via || !segmenta || !piscina)
    {
        redde 0;
    }

    p = via;

    /* Skip leading slash */
    si (*p == '/')
    {
        p++;
    }

    dum (*p && numerus < max_segmenta)
    {
        segment_start = p;

        /* Find end of segment */
        dum (*p && *p != '/')
        {
            p++;
        }

        len = (i32)(p - segment_start);
        si (len > 0)
        {
            /* Check if parameter (starts with :) */
            si (*segment_start == ':')
            {
                segmenta[numerus].est_param = VERUM;
                /* Skip : in param name */
                seg = _chorda_ex_raw(segment_start + I, len - I, piscina);
            }
            alioquin
            {
                segmenta[numerus].est_param = FALSUM;
                seg = _chorda_ex_raw(segment_start, len, piscina);
            }

            segmenta[numerus].valor = seg;
            numerus++;
        }

        /* Skip slash */
        si (*p == '/')
        {
            p++;
        }
    }

    redde numerus;
}


/* Parse request path in segmenta (simpler - no param detection) */
interior i32
_parse_request_via(
    chorda          via,
    chorda*         segmenta,
    i32             max_segmenta)
{
    i8* p;
    i8* segment_start;
    i8* via_end;
    i32 numerus = 0;
    i32 len;

    si (via.mensura == 0 || !segmenta)
    {
        redde 0;
    }

    p = via.datum;
    via_end = via.datum + via.mensura;

    /* Skip leading slash */
    si (*p == '/')
    {
        p++;
    }

    dum (p < via_end && numerus < max_segmenta)
    {
        segment_start = p;

        /* Find end of segment */
        dum (p < via_end && *p != '/')
        {
            p++;
        }

        len = (i32)(p - segment_start);
        si (len > 0)
        {
            segmenta[numerus].datum = segment_start;
            segmenta[numerus].mensura = len;
            numerus++;
        }

        /* Skip slash */
        si (p < via_end && *p == '/')
        {
            p++;
        }
    }

    redde numerus;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - MATCHING
 * ======================================================================== */

/* Match route pattern against request path */
interior b32
_match_pattern(
    Routa*       routa,
    chorda*      req_segmenta,
    i32          req_numerus,
    RoutaParams* params,
    Piscina*     piscina)
{
    i32 i;

    (vacuum)piscina;

    /* Must have same number of segments */
    si (routa->segmenta_numerus != req_numerus)
    {
        redde FALSUM;
    }

    params->numerus = 0;

    per (i = 0; i < req_numerus; i++)
    {
        si (routa->segmenta[i].est_param)
        {
            /* Parameter - extract value */
            si (params->numerus >= ROUTER_PARAM_MAXIMA)
            {
                redde FALSUM;
            }

            params->params[params->numerus].titulus = routa->segmenta[i].valor;
            params->params[params->numerus].valor = req_segmenta[i];
            params->numerus++;
        }
        alioquin
        {
            /* Literal - must match exactly */
            si (!chorda_aequalis(routa->segmenta[i].valor, req_segmenta[i]))
            {
                redde FALSUM;
            }
        }
    }

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - LIFECYCLE
 * ======================================================================== */

Router*
router_creare(Piscina* piscina)
{
    Router* router;

    si (!piscina)
    {
        redde NIHIL;
    }

    router = (Router*)piscina_allocare(piscina, (i64)magnitudo(Router));
    si (!router)
    {
        redde NIHIL;
    }

    router->piscina = piscina;
    router->numerus = 0;
    memset(router->routae, 0, magnitudo(router->routae));

    redde router;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - ROUTE REGISTRATION
 * ======================================================================== */

b32
router_adicere(
    Router*             router,
    HttpMethodus        methodus,
    constans character* via,
    RoutaHandler        handler)
{
    Routa* routa;
    i32 len;

    si (!router || !via || !handler)
    {
        redde FALSUM;
    }

    si (router->numerus >= ROUTER_ROUTA_MAXIMA)
    {
        redde FALSUM;
    }

    routa = &router->routae[router->numerus];

    len = (i32)strlen(via);
    routa->via = _chorda_ex_raw(via, len, router->piscina);
    routa->methodus = methodus;
    routa->handler = handler;
    routa->activus = VERUM;

    /* Determine type and parse */
    si (_via_habet_params(via))
    {
        routa->tipus = ROUTA_PATTERN;
        routa->segmenta_numerus = _parse_via(
            via,
            routa->segmenta,
            ROUTER_SEGMENTA_MAXIMA,
            router->piscina);
    }
    alioquin
    {
        routa->tipus = ROUTA_EXACTA;
        routa->segmenta_numerus = 0;
    }

    router->numerus++;

    redde VERUM;
}

b32
router_get(
    Router*             router,
    constans character* via,
    RoutaHandler        handler)
{
    redde router_adicere(router, HTTP_GET, via, handler);
}

b32
router_post(
    Router*             router,
    constans character* via,
    RoutaHandler        handler)
{
    redde router_adicere(router, HTTP_POST, via, handler);
}

b32
router_put(
    Router*             router,
    constans character* via,
    RoutaHandler        handler)
{
    redde router_adicere(router, HTTP_PUT, via, handler);
}

b32
router_delete(
    Router*             router,
    constans character* via,
    RoutaHandler        handler)
{
    redde router_adicere(router, HTTP_DELETE, via, handler);
}

b32
router_patch(
    Router*             router,
    constans character* via,
    RoutaHandler        handler)
{
    redde router_adicere(router, HTTP_PATCH, via, handler);
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - MATCHING
 * ======================================================================== */

RoutaResultus
router_matching(
    Router*      router,
    HttpMethodus methodus,
    chorda       via,
    Piscina*     piscina)
{
    RoutaResultus res;
    chorda req_segmenta[ROUTER_SEGMENTA_MAXIMA];
    i32 req_numerus;
    i32 i;
    Routa* routa;

    memset(&res, 0, magnitudo(res));
    res.invenit = FALSUM;

    si (!router || !piscina)
    {
        redde res;
    }

    /* First try exact matches */
    per (i = 0; i < router->numerus; i++)
    {
        routa = &router->routae[i];

        si (!routa->activus)
        {
            perge;
        }

        si (routa->methodus != methodus)
        {
            perge;
        }

        si (routa->tipus == ROUTA_EXACTA)
        {
            si (chorda_aequalis(routa->via, via))
            {
                res.invenit = VERUM;
                res.handler = routa->handler;
                res.params.numerus = 0;
                redde res;
            }
        }
    }

    /* Parse request path for pattern matching */
    req_numerus = _parse_request_via(via, req_segmenta, ROUTER_SEGMENTA_MAXIMA);

    /* Try pattern matches */
    per (i = 0; i < router->numerus; i++)
    {
        routa = &router->routae[i];

        si (!routa->activus)
        {
            perge;
        }

        si (routa->methodus != methodus)
        {
            perge;
        }

        si (routa->tipus == ROUTA_PATTERN)
        {
            si (_match_pattern(routa, req_segmenta, req_numerus, &res.params, piscina))
            {
                res.invenit = VERUM;
                res.handler = routa->handler;
                redde res;
            }
        }
    }

    redde res;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - PARAMETER ACCESS
 * ======================================================================== */

chorda
router_param_obtinere(
    RoutaParams*        params,
    constans character* titulus)
{
    chorda empty;
    i32 i;
    i32 len;

    empty.datum = NIHIL;
    empty.mensura = 0;

    si (!params || !titulus)
    {
        redde empty;
    }

    len = (i32)strlen(titulus);

    per (i = 0; i < params->numerus; i++)
    {
        si (params->params[i].titulus.mensura == len)
        {
            si (memcmp(params->params[i].titulus.datum, titulus, (size_t)len) == 0)
            {
                redde params->params[i].valor;
            }
        }
    }

    redde empty;
}

i32
router_param_obtinere_i32(
    RoutaParams*        params,
    constans character* titulus,
    i32                 default_valor)
{
    chorda valor;
    i32 result = 0;
    i32 i;
    i8 c;

    valor = router_param_obtinere(params, titulus);

    si (valor.mensura == 0)
    {
        redde default_valor;
    }

    /* Parse integer */
    per (i = 0; i < valor.mensura; i++)
    {
        c = valor.datum[i];
        si (c < '0' || c > '9')
        {
            redde default_valor;
        }
        result = result * X + (i32)(c - '0');
    }

    redde result;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - UTILITY
 * ======================================================================== */

i32
router_numerus_routarum(Router* router)
{
    si (!router)
    {
        redde 0;
    }

    redde router->numerus;
}
