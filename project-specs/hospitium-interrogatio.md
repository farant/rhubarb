# hospitium — interrogatio (raw Q&A, 2026-07-20)

*Interview for the house HTTP server, conducted after the
three-explorer audit (project-specs/servus-http-explorationes.md).
16 questions, 4 rounds. Answers are Fran's selections.*

## Round 1 — identity & posture

**Q1. Name?** (servitium taken by the entity registry)
**A: hospitium** — guest-house/hospitality; the server receives
guests (connections) and hosts them; hospes carries the
guest/host duality. NAME SEALED.

**Q2. Who is v1 FOR?**
**A: Infra first** — test fixture + vitrea dev-mode are the
day-one consumers; app-backend hardening still built, but the
exit bar is probatio_http localized + dev serving works.

**Q3. Threading posture?**
**A: Single-thread only** — vitrea precedent: pump, parse,
dispatch, write all on one thread by construction. Handlers must
be fast; slow work is the app's problem (actor.c workers hand
results back). Server library stays thread-free.

**Q4. Keep-alive / pipelining contract?**
**A: Keep-alive, no pipelining** — connections persist across
requests (per-connection piscina reset between them); a client
that pipelines gets request N finished, then an honest close.

## Round 2 — protocol strictness

**Q5. Chunked request bodies?** (today: silently discarded TODO stub)
**A: Reject with 411** Length Required. Client-side decoder
exists in http.c if ever wanted.

**Q6. Hard-reject guards in v1?** (multi-select)
**A: ALL FOUR** — duplicate Content-Length → 400; CL +
Transfer-Encoding → 400; unknown method → 501; garbage/missing
version → 400.

**Q7. Bare-LF request lines?** (today: parser hangs forever)
**A: Lenient — accept LF**, strip optional CR. Simpler than
CRLF-only matching, kills the hang, nc/telnet-friendly.

**Q8. Resource limits shape?**
**A: Configurable with defaults** — HospitiumConfiguratio carries
limits with house defaults (max request ~1MB, URI ~8KB, idle
timeout ~30s, header-read timeout ~10s; header cap 64 exists).
Zero-config for the fixture.

## Round 3 — API shape

**Q9. Handler ABI?** (router's current signature has no piscina/userdata)
**A: Context struct + generic router** — HospitiumTractator
(Colloquium*) carrying petitio/params/piscina-per-petitione/
responsum/userdata (internuntius precedent); router demoted to
pure matcher storing vacuum* per route.

**Q10. 404 vs 405 + HEAD→GET home?**
**A: Extend router matching** — RoutaResultus grows via_inventa +
methodi-permissae mask; hospitium renders 405 + Allow and treats
HEAD as GET-without-body. The matcher owns matching truth.

**Q11. Response ergonomics?**
**A: One-shot + addere** — respondere(colloquium, status, mimen,
corpus) covers 95%; caput_addere for extras. Serializer hardened:
Content-Length ALWAYS (incl. 0), Connection per keep-alive state,
dup-CL guard. No Date header in v1.

**Q12. Static serving in v1?**
**A: Directory only + shared MIME** —
hospitium_directorium_servire with ..-traversal refusal; MIME
table extracted from vitrea_macos.m into shared mimen lib (char*
+ chorda variants). Capsula-serving stays vitrea's.

## Round 4 — testing & runtime

**Q13. How does the localized probatio_http fixture run?**
**A: fork() in test setup** — child runs fixture on port 0,
reports port via pipe, teardown kills. (In-process same-thread
deadlocks: http_exsequi blocks on recv.)

**Q14. Loop ownership?**
**A: gressus() + currere()** — hospitium_gressus(ms) = one
bounded reactor turn (composable into a host loop, e.g. a vitrea
app self-hosting dev assets); currere() = blocking convenience.

**Q15. Dev conveniences?** (multi-select)
**A: ALL FOUR** — access log to stderr (config flag); ACAO knob
(Access-Control-Allow-Origin: * — retires 01KXZZW15P);
index.html default document; directory listing page when no
index.html.

**Q16. Bind address default?**
**A: 127.0.0.1** — localhost-only unless explicitly configured; a
dev server shouldn't be network-reachable by default.
