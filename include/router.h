/* router.h - Bibliotheca Router (HTTP Route Matching)
 *
 * Matches HTTP requests to handlers, extracts path parameters.
 *
 * USUS:
 *   Router* router = router_creare(piscina);
 *
 *   router_get(router, "/users", handler_users);
 *   router_get(router, "/users/:id", handler_user);
 *   router_post(router, "/users", handler_create_user);
 *
 *   RoutaResultus res = router_matching(router, HTTP_GET, via, piscina);
 *   si (res.invenit)
 *   {
 *       res.handler(petitio, responsum, &res.params);
 *   }
 */

#ifndef ROUTER_H
#define ROUTER_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http.h"


/* ========================================================================
 * CONSTANTES
 * ======================================================================== */

/* Limites */
#define ROUTER_PARAM_MAXIMA     XVI    /* 16 parameters per route */
#define ROUTER_ROUTA_MAXIMA     CXXVIII /* 128 routes maximum */
#define ROUTER_SEGMENTA_MAXIMA  XVI    /* 16 path segments maximum */


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Route parameter */
nomen structura {
    chorda titulus;   /* Parameter name without : */
    chorda valor;     /* Extracted value */
} RoutaParam;

/* Parameters container */
nomen structura {
    RoutaParam params[ROUTER_PARAM_MAXIMA];
    i32        numerus;
} RoutaParams;

/* Handler signature */
nomen vacuum (*RoutaHandler)(
    HttpPetitioServeri* petitio,
    HttpResponsum*      responsum,
    RoutaParams*        params);

/* Route types */
nomen enumeratio {
    ROUTA_EXACTA = 0,  /* /users - exact match */
    ROUTA_PATTERN      /* /users/:id - has parameters */
} RoutaTipus;

/* Opaque router handle */
nomen structura Router Router;

/* Match result */
nomen structura {
    b32          invenit;
    RoutaHandler handler;
    RoutaParams  params;
} RoutaResultus;


/* ========================================================================
 * FUNCTIONES - LIFECYCLE
 * ======================================================================== */

/* Creare novum router
 *
 * piscina: Arena pro allocationibus
 *
 * Redde: Router vel NIHIL si error
 */
Router*
router_creare(Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - ROUTE REGISTRATION
 * ======================================================================== */

/* Adicere route pro GET method */
b32
router_get(
    Router*             router,
    constans character* via,
    RoutaHandler        handler);

/* Adicere route pro POST method */
b32
router_post(
    Router*             router,
    constans character* via,
    RoutaHandler        handler);

/* Adicere route pro PUT method */
b32
router_put(
    Router*             router,
    constans character* via,
    RoutaHandler        handler);

/* Adicere route pro DELETE method */
b32
router_delete(
    Router*             router,
    constans character* via,
    RoutaHandler        handler);

/* Adicere route pro PATCH method */
b32
router_patch(
    Router*             router,
    constans character* via,
    RoutaHandler        handler);

/* Adicere route generica
 *
 * router:  Router
 * methodus: HTTP method
 * via:      Path pattern (e.g., "/users/:id")
 * handler:  Handler function
 *
 * Redde: VERUM si successus
 */
b32
router_adicere(
    Router*             router,
    HttpMethodus        methodus,
    constans character* via,
    RoutaHandler        handler);


/* ========================================================================
 * FUNCTIONES - MATCHING
 * ======================================================================== */

/* Match request to route
 *
 * router:  Router
 * methodus: HTTP method
 * via:      Request path
 * piscina:  Arena pro param allocationibus
 *
 * Redde: RoutaResultus cum invenit=VERUM si matched
 */
RoutaResultus
router_matching(
    Router*      router,
    HttpMethodus methodus,
    chorda       via,
    Piscina*     piscina);


/* ========================================================================
 * FUNCTIONES - PARAMETER ACCESS
 * ======================================================================== */

/* Obtinere parameter per nomen
 *
 * Redde: Parameter valor vel chorda vacua si non invenit
 */
chorda
router_param_obtinere(
    RoutaParams*        params,
    constans character* titulus);

/* Obtinere parameter ut i32
 *
 * Redde: Valor vel default_valor si non invenit vel non numerus
 */
i32
router_param_obtinere_i32(
    RoutaParams*        params,
    constans character* titulus,
    i32                 default_valor);


/* ========================================================================
 * FUNCTIONES - UTILITY
 * ======================================================================== */

/* Obtinere numerum routarum registratarum */
i32
router_numerus_routarum(Router* router);


#endif /* ROUTER_H */
