/* probatio_router.c - Probationes pro Router (HTTP Route Matching)
 *
 * Probationes functionum router: route registration, matching, param extraction.
 */

#include "router.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * VARIABILES PRO PROBATIONIBUS
 * ======================================================================== */

hic_manens i32 g_handler_vocatus = 0;
hic_manens chorda g_ultima_via;


/* ========================================================================
 * HANDLERS PRO PROBATIONIBUS
 * ======================================================================== */

interior vacuum
handler_simplex(HttpPetitioServeri* petitio, HttpResponsum* responsum, RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus++;
}

interior vacuum
handler_users(HttpPetitioServeri* petitio, HttpResponsum* responsum, RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = C;  /* 100 - users handler */
}

interior vacuum
handler_user_by_id(HttpPetitioServeri* petitio, HttpResponsum* responsum, RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = CC;  /* 200 - user by id handler */
}

interior vacuum
handler_posts(HttpPetitioServeri* petitio, HttpResponsum* responsum, RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = CCC;  /* 300 - posts handler */
}


/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

interior vacuum
reset_globals(vacuum)
{
    g_handler_vocatus = 0;
    g_ultima_via.datum = NIHIL;
    g_ultima_via.mensura = 0;
}

interior chorda
chorda_test(constans character* s)
{
    redde chorda_ex_buffer((i8*)(size_t)s, (i32)strlen(s));
}


/* ========================================================================
 * PROBATIONES - LIFECYCLE
 * ======================================================================== */

interior vacuum
probatio_creare(Piscina* piscina)
{
    Router* router;

    printf("--- Probans creare ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    CREDO_VERUM(router_numerus_routarum(router) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ROUTE REGISTRATION
 * ======================================================================== */

interior vacuum
probatio_adicere_routas(Piscina* piscina)
{
    Router* router;
    b32 res;

    printf("--- Probans adicere routas ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere GET routes */
    res = router_get(router, "/", handler_simplex);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == I);

    res = router_get(router, "/users", handler_users);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == II);

    /* Adicere POST route */
    res = router_post(router, "/users", handler_simplex);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == III);

    /* Adicere pattern route */
    res = router_get(router, "/users/:id", handler_user_by_id);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == IV);

    printf("  Routae registratae: %u\n", router_numerus_routarum(router));
    printf("\n");
}


interior vacuum
probatio_omnes_methodi(Piscina* piscina)
{
    Router* router;

    printf("--- Probans omnes methodi ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    CREDO_VERUM(router_get(router, "/get", handler_simplex));
    CREDO_VERUM(router_post(router, "/post", handler_simplex));
    CREDO_VERUM(router_put(router, "/put", handler_simplex));
    CREDO_VERUM(router_delete(router, "/delete", handler_simplex));
    CREDO_VERUM(router_patch(router, "/patch", handler_simplex));

    CREDO_VERUM(router_numerus_routarum(router) == V);
    printf("  Omnes methodi registratae\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - EXACT MATCHING
 * ======================================================================== */

interior vacuum
probatio_matching_exacta(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;

    printf("--- Probans matching exacta ---\n");

    reset_globals();
    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/", handler_simplex);
    router_get(router, "/users", handler_users);
    router_get(router, "/posts", handler_posts);

    /* Match root */
    res = router_matching(router, HTTP_GET, chorda_test("/"), piscina);
    CREDO_VERUM(res.invenit);
    CREDO_NON_NIHIL(res.handler);
    res.handler(NIHIL, NIHIL, &res.params);
    CREDO_VERUM(g_handler_vocatus == I);

    /* Match /users */
    res = router_matching(router, HTTP_GET, chorda_test("/users"), piscina);
    CREDO_VERUM(res.invenit);
    res.handler(NIHIL, NIHIL, &res.params);
    CREDO_VERUM(g_handler_vocatus == C);

    /* Match /posts */
    res = router_matching(router, HTTP_GET, chorda_test("/posts"), piscina);
    CREDO_VERUM(res.invenit);
    res.handler(NIHIL, NIHIL, &res.params);
    CREDO_VERUM(g_handler_vocatus == CCC);

    printf("  Matching exacta correcte\n");
    printf("\n");
}


interior vacuum
probatio_non_matching(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;

    printf("--- Probans non-matching ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users", handler_users);

    /* Via non existens */
    res = router_matching(router, HTTP_GET, chorda_test("/unknown"), piscina);
    CREDO_FALSUM(res.invenit);

    /* Methodus incorrecta */
    res = router_matching(router, HTTP_POST, chorda_test("/users"), piscina);
    CREDO_FALSUM(res.invenit);

    printf("  Non-matching correcte\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PATTERN MATCHING
 * ======================================================================== */

interior vacuum
probatio_matching_pattern_simplex(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;
    chorda id_valor;

    printf("--- Probans matching pattern simplex ---\n");

    reset_globals();
    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users/:id", handler_user_by_id);

    /* Match /users/123 */
    res = router_matching(router, HTTP_GET, chorda_test("/users/123"), piscina);
    CREDO_VERUM(res.invenit);
    CREDO_VERUM(res.params.numerus == I);

    /* Verificare param */
    id_valor = router_param_obtinere(&res.params, "id");
    CREDO_VERUM(id_valor.mensura == III);
    CREDO_VERUM(memcmp(id_valor.datum, "123", III) == 0);

    printf("  Pattern simplex: id=123\n");
    printf("\n");
}


interior vacuum
probatio_matching_pattern_multiplex(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;
    chorda user_id;
    chorda post_id;

    printf("--- Probans matching pattern multiplex ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users/:user_id/posts/:post_id", handler_simplex);

    /* Match /users/42/posts/7 */
    res = router_matching(router, HTTP_GET, chorda_test("/users/42/posts/7"), piscina);
    CREDO_VERUM(res.invenit);
    CREDO_VERUM(res.params.numerus == II);

    /* Verificare params */
    user_id = router_param_obtinere(&res.params, "user_id");
    CREDO_VERUM(user_id.mensura == II);
    CREDO_VERUM(memcmp(user_id.datum, "42", II) == 0);

    post_id = router_param_obtinere(&res.params, "post_id");
    CREDO_VERUM(post_id.mensura == I);
    CREDO_VERUM(memcmp(post_id.datum, "7", I) == 0);

    printf("  Pattern multiplex: user_id=42, post_id=7\n");
    printf("\n");
}


interior vacuum
probatio_matching_mixed(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;

    printf("--- Probans matching mixed (exact + pattern) ---\n");

    reset_globals();
    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Exact ante pattern */
    router_get(router, "/users", handler_users);
    router_get(router, "/users/:id", handler_user_by_id);

    /* Debet match exact pro /users */
    res = router_matching(router, HTTP_GET, chorda_test("/users"), piscina);
    CREDO_VERUM(res.invenit);
    res.handler(NIHIL, NIHIL, &res.params);
    CREDO_VERUM(g_handler_vocatus == C);  /* users handler */
    CREDO_VERUM(res.params.numerus == 0);

    /* Debet match pattern pro /users/123 */
    res = router_matching(router, HTTP_GET, chorda_test("/users/123"), piscina);
    CREDO_VERUM(res.invenit);
    res.handler(NIHIL, NIHIL, &res.params);
    CREDO_VERUM(g_handler_vocatus == CC);  /* user_by_id handler */
    CREDO_VERUM(res.params.numerus == I);

    printf("  Mixed matching correcte\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARAM ACCESS
 * ======================================================================== */

interior vacuum
probatio_param_obtinere_i32(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;
    i32 id;

    printf("--- Probans param_obtinere_i32 ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/items/:id", handler_simplex);

    /* Test with numeric id */
    res = router_matching(router, HTTP_GET, chorda_test("/items/456"), piscina);
    CREDO_VERUM(res.invenit);

    id = router_param_obtinere_i32(&res.params, "id", 0);
    CREDO_VERUM(id == CDLVI);  /* 456 */
    printf("  id = %u (expectatum 456)\n", id);

    /* Test with non-existent param */
    id = router_param_obtinere_i32(&res.params, "unknown", XCIX);
    CREDO_VERUM(id == XCIX);  /* default */

    printf("\n");
}


interior vacuum
probatio_param_non_numericus(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;
    i32 id;
    chorda slug;

    printf("--- Probans param non numericus ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/articles/:slug", handler_simplex);

    res = router_matching(router, HTTP_GET, chorda_test("/articles/hello-world"), piscina);
    CREDO_VERUM(res.invenit);

    /* String param */
    slug = router_param_obtinere(&res.params, "slug");
    CREDO_VERUM(slug.mensura == XI);  /* "hello-world" = 11 chars */

    /* Non-numeric param as i32 returns default */
    id = router_param_obtinere_i32(&res.params, "slug", 0);
    CREDO_VERUM(id == 0);

    printf("  Non-numeric param handled correcte\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;
    b32 add_res;
    chorda empty;

    printf("--- Probans nullum argumenta ---\n");

    /* Creare cum NIHIL piscina */
    router = router_creare(NIHIL);
    CREDO_NIHIL(router);

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere cum NIHIL via */
    add_res = router_get(router, NIHIL, handler_simplex);
    CREDO_FALSUM(add_res);

    /* Adicere cum NIHIL handler */
    add_res = router_get(router, "/test", NIHIL);
    CREDO_FALSUM(add_res);

    /* Matching cum NIHIL router */
    res = router_matching(NIHIL, HTTP_GET, chorda_test("/"), piscina);
    CREDO_FALSUM(res.invenit);

    /* Param obtinere cum NIHIL */
    empty = router_param_obtinere(NIHIL, "id");
    CREDO_VERUM(empty.mensura == 0);

    /* Numerus cum NIHIL */
    CREDO_VERUM(router_numerus_routarum(NIHIL) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - EDGE CASES
 * ======================================================================== */

interior vacuum
probatio_trailing_slash(Piscina* piscina)
{
    Router* router;
    RoutaResultus res;

    printf("--- Probans trailing slash ---\n");

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users", handler_users);

    /* Sine trailing slash */
    res = router_matching(router, HTTP_GET, chorda_test("/users"), piscina);
    CREDO_VERUM(res.invenit);

    /* Cum trailing slash - different path, non debet match */
    res = router_matching(router, HTTP_GET, chorda_test("/users/"), piscina);
    CREDO_FALSUM(res.invenit);

    printf("  Trailing slash handled\n");
    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES ROUTER\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_router", CXXVIII * M);
    credo_aperire(piscina);

    probatio_creare(piscina);
    probatio_adicere_routas(piscina);
    probatio_omnes_methodi(piscina);
    probatio_matching_exacta(piscina);
    probatio_non_matching(piscina);
    probatio_matching_pattern_simplex(piscina);
    probatio_matching_pattern_multiplex(piscina);
    probatio_matching_mixed(piscina);
    probatio_param_obtinere_i32(piscina);
    probatio_param_non_numericus(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_trailing_slash(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
