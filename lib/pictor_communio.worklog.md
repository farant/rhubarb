# pictor_communio worklog

## 2026-10-27 - sync inside pictor (communio-plan-5 T5, quarta)

`PictorCommunio` wraps the communio client for the pictor app: token file
= volume path + ".tessera" (a worktree pictor with its own volume can
never pick up the Mac's real token), server from the shell's veredarius,
arrivals -> `mutata` (pictor recomposes). The household device list
(`GET /communio/apparatus`: replica, titulus, persona) is cached raw in
praesentia `communio/apparatus`, so credits ("Amanda · iPhone" =
persona + " \xC2\xB7 " + titulus) answer offline and after restarts; it
is fetched at start, on `excitare`, and whenever acts arrive from a
replica the cache does not know (a new device joined).

401 handling lives in the CLIENT (`communio_cliens_recusationem_ponere`,
`CommunioStatus.recusata`): one refused request stops both lanes for
good (no 30 s retry loop against a revoked token) and calls back once;
pictor_communio then deletes the token file so "Sign in..." comes back.
A 401 on the device-list request does the same.

Shell side: `TestaAmbitus` gained `reactor`, `veredarius`, `machina`. The
Mac shell creates a reactor + the house veredarius; pictor returns the
reactor in `forma.reactor` and the shell hands it to the window loop with
`ludus_fenestra_reactorem_ponere` (NOT quiet mode): the loop polls it
(timeout 0) on every wake and never sleeps past its next timer. pictor's
loop already wakes every 300 ms (dispensator quies_ms) when idle, so a
long-poll answer is seen within 300 ms without changing pictor's drawing
schedule. Android: no reactor/veredarius -> no sync, and the APK links
the client but no TLS/HTTP/server (checked: no such objects; the iOS
binary has 0 `_br_` symbols).

Test trap: "A received everything B has" must count B's COMMUNICATED
acts (non-derived), not all its acts - checkpoints never travel (8 vs 6).
