# router.c worklog

## 2026-07-20 — hospitium Phase B: pure matcher + method mask

The router no longer knows what a handler is: RoutaHandler is
deleted and registration takes vacuum* datum. The reason is an
ISO-C constraint, not taste — function pointers may not be stored
in vacuum* under -pedantic, so the consumer allocates a box
({tractator, datum}) and registers &capsa (the CAPSA pattern;
hospitium will do exactly this from its piscina, the tests
preview it).

router_matching rewrite: the old code skipped wrong-method routes
and returned on first hit, which made 404-vs-405 IMPOSSIBLE to
distinguish. Now there is NO early return — every path match in
both passes accumulates via_inventa + a method bit into
methodi_permissae (ROUTA_METHODUS_BIT), and the winner is
assigned under a !res.invenit guard. Exact-over-pattern is
preserved structurally (exact pass runs first); first-registered-
wins within a pass falls out of the guard.

The subtle bug this design dodges: pattern candidates now match
into a SCRATCH RoutaParams (memset'd per route), copied into
res.params only on a win. Without that, a wrong-method pattern
route that path-matches BEFORE the winner would write its params
into the result and clobber the winner's — pinned by the
params-isolation sub-assert (POST /a/:y registered before
GET /a/:x; match GET /a/7 → x=7 present, y absent).

HEAD stays method-literal here (pinned by test): HEAD⇒GET,
Allow rendering, +OPTIONS are hospitium's job (Phase C), built
from the mask.
