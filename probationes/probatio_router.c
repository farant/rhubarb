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
handler_simplex (
    HttpPetitioServeri* petitio,
         HttpResponsum* responsum,
           RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus++;
}

interior vacuum
handler_users (
    HttpPetitioServeri* petitio,
         HttpResponsum* responsum,
           RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = C;  /* 100 - users handler */
}

interior vacuum
handler_user_by_id (
    HttpPetitioServeri* petitio,
         HttpResponsum* responsum,
           RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = CC;  /* 200 - user by id handler */
}

interior vacuum
handler_posts (
    HttpPetitioServeri* petitio,
         HttpResponsum* responsum,
           RoutaParams* params)
{
    (vacuum)petitio;
    (vacuum)responsum;
    (vacuum)params;
    g_handler_vocatus = CCC;  /* 300 - posts handler */
}


/* ========================================================================
 * CAPSAE - router indices obiectorum solos tenet (index functionis in
 * vacuum* vetitus sub -pedantic); exemplar CAPSAE quod hospitium utetur
 * ======================================================================== */

nomen vacuum (*TractatorProbationis)(
    HttpPetitioServeri*, HttpResponsum*, RoutaParams*);

nomen structura {
    TractatorProbationis tractator;
} CapsaTractatoris;

hic_manens CapsaTractatoris g_capsa_simplex    = { handler_simplex };
hic_manens CapsaTractatoris g_capsa_users      = { handler_users };
hic_manens CapsaTractatoris g_capsa_user_by_id = { handler_user_by_id };
hic_manens CapsaTractatoris g_capsa_posts      = { handler_posts };

/* Vocare tractatorem ex capsa resultatus */
interior vacuum
_vocare (
    RoutaResultus* res)
{
    ((CapsaTractatoris*)res->datum)->tractator(NIHIL, NIHIL,
        &res->params);
}


/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

/* parare omnium sectionum: status plagulae ante quamque sectionem
 * reponitur - numerator, via ultima, et capsae (router indices earum
 * non constantes tenet) */
interior vacuum
reset_globals (
    CredoContextus* c)
{
    (vacuum)c;
    g_handler_vocatus             = 0;
    g_ultima_via.datum            = NIHIL;
    g_ultima_via.mensura          = 0;
    g_capsa_simplex.tractator     = handler_simplex;
    g_capsa_users.tractator       = handler_users;
    g_capsa_user_by_id.tractator  = handler_user_by_id;
    g_capsa_posts.tractator       = handler_posts;
}

interior chorda
chorda_test (
    constans character* s)
{
    redde chorda_ex_buffer((i8*)(size_t)s, (i32)strlen(s));
}


/* ========================================================================
 * PROBATIONES - LIFECYCLE
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
     Router* router;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    CREDO_VERUM(router_numerus_routarum(router) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ROUTE REGISTRATION
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
     Router* router;
        b32  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere GET routes */
    res = router_get(router, "/", &g_capsa_simplex);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == I);

    res = router_get(router, "/users", &g_capsa_users);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == II);

    /* Adicere POST route */
    res = router_post(router, "/users", &g_capsa_simplex);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == III);

    /* Adicere pattern route */
    res = router_get(router, "/users/:id", &g_capsa_user_by_id);
    CREDO_VERUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == IV);

    printf("  Routae registratae: %u\n",
        router_numerus_routarum(router));
    printf("\n");
}


interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
     Router* router;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    CREDO_VERUM(router_get(router, "/get", &g_capsa_simplex));
    CREDO_VERUM(router_post(router, "/post", &g_capsa_simplex));
    CREDO_VERUM(router_put(router, "/put", &g_capsa_simplex));
    CREDO_VERUM(router_delete(router, "/delete", &g_capsa_simplex));
    CREDO_VERUM(router_patch(router, "/patch", &g_capsa_simplex));

    CREDO_VERUM(router_numerus_routarum(router) == V);
    printf("  Omnes methodi registratae\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - EXACT MATCHING
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/", &g_capsa_simplex);
    router_get(router, "/users", &g_capsa_users);
    router_get(router, "/posts", &g_capsa_posts);

    /* Match root */
    res = router_matching(router, HTTP_GET, chorda_test("/"), piscina);
    CREDO_VERUM(res.invenit);
    CREDO_NON_NIHIL(res.datum);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == I);

    /* Match /users */
    res = router_matching(router, HTTP_GET, chorda_test("/users"),
        piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == C);

    /* Match /posts */
    res = router_matching(router, HTTP_GET, chorda_test("/posts"),
        piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == CCC);

    printf("  Matching exacta correcte\n");
    printf("\n");
}


interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users", &g_capsa_users);

    /* Via non existens */
    res = router_matching(router, HTTP_GET, chorda_test("/unknown"),
        piscina);
    CREDO_FALSUM(res.invenit);

    /* Methodus incorrecta */
    res = router_matching(router, HTTP_POST, chorda_test("/users"),
        piscina);
    CREDO_FALSUM(res.invenit);

    printf("  Non-matching correcte\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PATTERN MATCHING
 * ======================================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
           chorda  id_valor;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users/:id", &g_capsa_user_by_id);

    /* Match /users/123 */
    res = router_matching(router, HTTP_GET, chorda_test("/users/123"),
        piscina);
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
_sectio_septima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
           chorda  user_id;
           chorda  post_id;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users/:user_id/posts/:post_id",
        &g_capsa_simplex);

    /* Match /users/42/posts/7 */
    res = router_matching(router, HTTP_GET,
        chorda_test("/users/42/posts/7"), piscina);
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
_sectio_octava (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Exact ante pattern */
    router_get(router, "/users", &g_capsa_users);
    router_get(router, "/users/:id", &g_capsa_user_by_id);

    /* Debet match exact pro /users */
    res = router_matching(router, HTTP_GET, chorda_test("/users"),
        piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == C);  /* users handler */
    CREDO_VERUM(res.params.numerus == 0);

    /* Debet match pattern pro /users/123 */
    res = router_matching(router, HTTP_GET, chorda_test("/users/123"),
        piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == CC);  /* user_by_id handler */
    CREDO_VERUM(res.params.numerus == I);

    printf("  Mixed matching correcte\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARAM ACCESS
 * ======================================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
              i32  id;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/items/:id", &g_capsa_simplex);

    /* Test with numeric id */
    res = router_matching(router, HTTP_GET, chorda_test("/items/456"),
        piscina);
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
_sectio_decima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
              i32  id;
           chorda  slug;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/articles/:slug", &g_capsa_simplex);

    res = router_matching(router, HTTP_GET,
        chorda_test("/articles/hello-world"), piscina);
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
_sectio_decima_prima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
              b32  add_res;
           chorda  empty;

    piscina = c->piscina;
    /* Creare cum NIHIL piscina */
    router = router_creare(NIHIL);
    CREDO_NIHIL(router);

    router = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere cum NIHIL via */
    add_res = router_get(router, NIHIL, &g_capsa_simplex);
    CREDO_FALSUM(add_res);

    /* Adicere cum NIHIL datum */
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
_sectio_decima_secunda (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    router_get(router, "/users", &g_capsa_users);

    /* Sine trailing slash */
    res = router_matching(router, HTTP_GET, chorda_test("/users"),
        piscina);
    CREDO_VERUM(res.invenit);

    /* Cum trailing slash - different path, non debet match */
    res = router_matching(router, HTTP_GET, chorda_test("/users/"),
        piscina);
    CREDO_FALSUM(res.invenit);

    printf("  Trailing slash handled\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CAPACITAS
 * ======================================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
      Piscina* piscina;
       Router* router;
          b32  res;
          i32  i;
          i32  successus_count = 0;
    character  via_buffer[XXXII];

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere CXXVIII routes */
    per (i = 0; i < CXXVIII; i++)
    {
        sprintf(via_buffer, "/route%d", i);
        res = router_get(router, via_buffer, &g_capsa_simplex);
        si (res)
        {
            successus_count++;
        }
    }

    CREDO_VERUM(successus_count == CXXVIII);
    CREDO_VERUM(router_numerus_routarum(router) == CXXVIII);
    printf("  Routae additae: %u\n", successus_count);

    /* Proxima debet fallere */
    res = router_get(router, "/overflow", &g_capsa_simplex);
    CREDO_FALSUM(res);
    CREDO_VERUM(router_numerus_routarum(router) == CXXVIII);
    printf("  Routa CXXIX recte recusata\n");

    printf("\n");
}


interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Adicere eadem via bis */
    CREDO_VERUM(router_get(router, "/test", &g_capsa_users));
    CREDO_VERUM(router_get(router, "/test", &g_capsa_user_by_id));

    /* Secunda debet vincere (vel prima, dependent on impl) */
    res = router_matching(router, HTTP_GET, chorda_test("/test"),
        piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    /* Verificare quod handler est users (C) vel user_by_id (CC) */
    CREDO_VERUM(g_handler_vocatus == C || g_handler_vocatus == CC);

    printf("  Duplicata handled (handler: %u)\n", g_handler_vocatus);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PATH EDGE CASES
 * ======================================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
        character  via_longa[CCLVI];
              i32  i;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* Creare via longa */
    via_longa[0] = '/';
    per (i = I; i < CCL; i++)
    {
        via_longa[i] = 'a';
    }
    via_longa[CCL] = '\0';

    CREDO_VERUM(router_get(router, via_longa, &g_capsa_simplex));

    res = router_matching(router, HTTP_GET,
                          chorda_ex_buffer((i8*)via_longa, CCL),
                          piscina);
    CREDO_VERUM(res.invenit);

    printf("  Via longa (%u chars) matched\n", CCL);
    printf("\n");
}


interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    CREDO_VERUM(router_get(router, "/", &g_capsa_simplex));

    res = router_matching(router, HTTP_GET, chorda_test("/"), piscina);
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == I);

    printf("  Radix matched\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - LARVA METHODORUM (via_inventa + methodi_permissae)
 * ======================================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    router_get(router, "/users", &g_capsa_users);

    res = router_matching(router, HTTP_POST, chorda_test("/users"),
        piscina);
    CREDO_FALSUM(res.invenit);
    CREDO_VERUM(res.via_inventa);
    CREDO_VERUM(res.methodi_permissae == ROUTA_METHODUS_BIT(HTTP_GET));

    printf("  405-materia: via inventa, larva GET\n");
    printf("\n");
}


interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
              i32  larva_plena;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    router_get(router, "/res", &g_capsa_users);
    router_post(router, "/res", &g_capsa_simplex);
    router_put(router, "/res", &g_capsa_posts);

    larva_plena = ROUTA_METHODUS_BIT(HTTP_GET)
                | ROUTA_METHODUS_BIT(HTTP_POST)
                | ROUTA_METHODUS_BIT(HTTP_PUT);

    /* victoria in routa prima - larva TAMEN completa accumulatur */
    res = router_matching(router, HTTP_GET, chorda_test("/res"),
        piscina);
    CREDO_VERUM(res.invenit);
    CREDO_VERUM(res.methodi_permissae == larva_plena);

    printf("  Larva completa etiam post victoriam\n");
    printf("\n");
}


interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;
           chorda  x_valor;
           chorda  y_valor;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);

    /* exacta POST; pattern POST methodi falsae ANTE victorem registrata */
    router_post(router, "/a/7", &g_capsa_simplex);
    router_post(router, "/a/:y", &g_capsa_posts);
    router_get(router, "/a/:x", &g_capsa_user_by_id);

    res = router_matching(router, HTTP_GET, chorda_test("/a/7"),
        piscina);

    /* I: pattern GET vincit */
    CREDO_VERUM(res.invenit);
    _vocare(&res);
    CREDO_VERUM(g_handler_vocatus == CC);

    /* II: exacta et pattern ambo larvam conferunt */
    CREDO_VERUM(res.methodi_permissae
        == (ROUTA_METHODUS_BIT(HTTP_POST)
                    | ROUTA_METHODUS_BIT(HTTP_GET)));

    /* III: insula parametrorum - routa pattern methodi falsae (ante
     * victorem congruens) parametra victoris conculcare non potest */
    CREDO_VERUM(res.params.numerus == I);
    x_valor = router_param_obtinere(&res.params, "x");
    CREDO_VERUM(x_valor.mensura == I);
    CREDO_VERUM(memcmp(x_valor.datum, "7", I) == 0);
    y_valor = router_param_obtinere(&res.params, "y");
    CREDO_VERUM(y_valor.mensura == 0);

    printf("  Exacta + pattern conferunt; parametra insulata\n");
    printf("\n");
}


interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    router_get(router, "/users", &g_capsa_users);

    res = router_matching(router, HTTP_GET, chorda_test("/nusquam"),
        piscina);
    CREDO_FALSUM(res.invenit);
    CREDO_FALSUM(res.via_inventa);
    CREDO_VERUM(res.methodi_permissae == 0);

    printf("  404-materia: nihil inventum, larva vacua\n");
    printf("\n");
}


interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
          Piscina* piscina;
           Router* router;
    RoutaResultus  res;

    piscina  = c->piscina;
    router   = router_creare(piscina);
    CREDO_NON_NIHIL(router);
    router_get(router, "/solum", &g_capsa_users);

    /* router methodo-literalis manet - HEAD->GET officium hospitii est */
    res = router_matching(router, HTTP_HEAD, chorda_test("/solum"),
        piscina);
    CREDO_FALSUM(res.invenit);
    CREDO_VERUM(res.via_inventa);
    CREDO_VERUM(res.methodi_permissae == ROUTA_METHODUS_BIT(HTTP_GET));

    printf("  HEAD non congruit GET - literalis\n");
    printf("\n");
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "creare", _sectio_prima, reset_globals, NIHIL, NIHIL, NIHIL },
    { "adicere routas", _sectio_secunda, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "omnes methodi", _sectio_tertia, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "matching exacta", _sectio_quarta, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "non-matching", _sectio_quinta, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "matching pattern simplex", _sectio_sexta, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "matching pattern multiplex", _sectio_septima, reset_globals,
        NIHIL, NIHIL, NIHIL },
    { "matching mixed (exact + pattern)", _sectio_octava, reset_globals,
        NIHIL, NIHIL, NIHIL },
    { "param_obtinere_i32", _sectio_nona, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "param non numericus", _sectio_decima, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "nullum argumenta", _sectio_decima_prima, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "trailing slash", _sectio_decima_secunda, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "routa capacitas (CXXVIII max)", _sectio_decima_tertia,
        reset_globals, NIHIL, NIHIL, NIHIL },
    { "duplicata routa", _sectio_decima_quarta, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "via longa", _sectio_decima_quinta, reset_globals, NIHIL, NIHIL,
        NIHIL },
    { "via radix (/)", _sectio_decima_sexta, reset_globals, NIHIL,
        NIHIL, NIHIL },
    { "larva: methodus falsa", _sectio_decima_septima, reset_globals,
        NIHIL, NIHIL, NIHIL },
    { "larva: completa post victoriam", _sectio_decima_octava,
        reset_globals, NIHIL, NIHIL, NIHIL },
    { "larva: exacta + pattern conferunt", _sectio_decima_nona,
        reset_globals, NIHIL, NIHIL, NIHIL },
    { "larva: via omnino absens", _sectio_vicesima, reset_globals,
        NIHIL, NIHIL, NIHIL },
    { "larva: HEAD literalis", _sectio_vicesima_prima, reset_globals,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32
principale (vacuum)
{
    redde credo_suitam_currere("router", SECTIONES);
}
