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

## 2026-10-28 - sign-in inside pictor (communio-plan-5 T7, quarta)

`PictorCommunio` now owns the app's `CommunioInscriptio` and one wiring
call, `pictor_communionem_ligare(pc, montatio, d, machina)`, used by both
apps/pictor/pictor.c and the suites: composition gets communio_praesto,
inscripta, auctor, persona and the sign-in state (callbacks, ctx = pc);
actions get incipere / codicem_dare / abicere callbacks. `_nuntiare`
refreshes the bound montatio (inscripta, Shared pictures, keyboard hidden
when the state becomes INSCRIPTA) before recomposing.

Bug found by the resume test: with a sign-in PENDING the token file
already exists but is not registered yet. Treating it as "signed in"
started the client, which got 401, which (T5 rule) deleted the token -
killing the very sign-in being resumed. Now: `pictor_communio_inscripta`
is FALSE and `pictor_communio_incipere` does nothing while a sign-in is
pending; INSCRIPTA starts the client (a client stopped by an earlier 401
is replaced).

The device list only lists devices whose replica is bound, and the
client binds AFTER the first list request - so "Signed in as" had no
name. If our own replica is missing, the list is fetched again after 1 s
(V tries). `pictor_communionem_claudere` leaves a pending sign-in in place
(`communio_inscriptionem_relinquere`: stop, keep token + praesentia) so
the next launch resumes it.

## 2026-10-29 — final review fixes (plan-5)

- **Re-sign-in after 401 now restarts sync.** `_inscriptio_mutata` used to
  clear `recusata` before calling `pictor_communio_incipere`, so incipere's
  "client stopped by 401 -> replace it" branch never fired: it fell into
  `excitare` on a stopped client and nothing synced (UI still said "Signed in
  as …"). Now `recusata` stays set until incipere has read the new token and
  built a new client. Test: pictor_inscriptio "revocatus, iterum inscriptus"
  (a SECOND revocation must reach pictor = the new client is live).
- **excitare = "the app just came back".** It now (1) nudges a pending
  sign-in to check the server at once (`communio_inscriptionem_excitare`),
  even while `recusata` is set (re-sign-in after a revoke), and (2) calls
  `communio_cliens_renovare`, which aborts in-flight requests and restarts
  both lanes. On iOS a long-poll begun before sleep can sit in URLSession
  until its 55 s timeout; plain excitare returned early on `in_itinere`.
  Only RESUMPTIO calls it (and incipere on an existing client), so the
  abort never happens per stroke. Tests: pictor_communio "somnus",
  pictor_inscriptio "excitata e Mail".

## 2026-10-30 - "Sent to <address>" on the code step

Amanda's first live sign-in: typo, then the right address; the code screen never named the address, so a typo was invisible (server answers 202 for unknown addresses and sends nothing). `epistula_missa[CCLVI]` holds the address IN MEMORY only, set in inscriptionem_incipere, wiped on abicere, INSCRIPTA and claudere; never in the volume (the pending record stays {aditus, incepta_ms}), so after a kill/resume the panel falls back to the generic text. Accessor answers during PETIT too - the panel is already "Check your email" while the request is in flight.

Also learned live: 401s on GET /communio/apparatus while a sign-in waits are the link-check polling (`_inspicere`), not a fault.

## 2026-11-03 - pulsus-plan T9: push in pictor

- `pictor_communio_pulsum_parare(pc, ambitus)` is called once at start (apps/pictor/pictor.c). It asks the shell for permission now if signed in, otherwise right after sign-in reaches INSCRIPTA (`_inscriptio_mutata`). Once per process: a revoke + re-sign-in does not ask again, but the new client gets the stored token (`_pulsum_inscribere` runs whenever `incipere` makes a client).
- The device token lives in memory only (`pulsus_tessera`, wiped in claudere) and is never logged; a refusal logs only the shell's cause.
- Events come from the client's missa hook (accepted acts), on every signed-in device, whether or not this device allowed notifications - the notification is for the OTHER devices. Act -> picture mapping is `pictor_spatia_actorum` in pictor_documentum.c (same genus index as `pictor_documenta_recentia`, plus plagula-condita via). Id = `<own replica>:<spatium>`, the same pair the Shared grid's aliena entries carry (`aliena/<replica>/<spatium>/documentum`), root picture = `<replica>:`.
- "Notify family now" (`pulsus.statim`) replaces "Sign in…" in the ⋯ menu once signed in, so the menu still fits V entries. The menu lives on the canvas, where a picture is always open, so it always sends the open picture's id; the spec's "disabled when nothing pending and nothing open" never arises there.
- T9 passes NIHIL as the tap callback; a cold-launch tap the shell was holding is dropped until T10 wires it.
- Tests: the suites' servers don't mount pulsus, so `probationes/veredarius_pulsus.h` wraps the veredarius, records /communio/pulsus/ requests and answers 204/202 itself.

## 2026-11-03 - pulsus-plan T10: tap -> transient group

- The tap handler (`_pulsus_tactus`, handed to the shell by `pulsum_petere`) reads the payload with `pulsus_tactum_legere` and ignores any clavis other than pictor.picturae. `plures` or no ids -> Shared grid page 1.
- If every id already resolves, the viewer opens on the group at once. Otherwise the ids are copied into `tactus_scheda`, the grid shows "Loading new pictures…" (`PictorCommunia.onerans`, which also replaces the "1 / n" page label with "Loading…"), and `pictor_communio_excitare` restarts the pull. Each `advenerunt` retries, but it only opens once ALL ids resolve: a batch can arrive across several pulls, so opening on a partial set would drop pictures that are a second away. After 15 s (`TACTUS_MORA_MS`), it opens whatever resolves, or stays on the grid.
- Deferred opens use `solum_e_rete`: if the person left the grid while waiting, a late sync does not yank them into the viewer.
- The group lives in `PictorCommunia.grex` in its own piscina (`grex_scheda`) as replica/spatium pairs, not indices. `pictor_communia_parare` (run on every arrival) vacates the main scheda and reorders `picturae`, so movement re-resolves through `pictor_communia_index` each step.
- In apps/pictor/pictor.c, `pulsum_parare` now runs after `pictor_salutationem_ponere`, because the shell delivers a held cold-launch tap inside `pulsum_petere` and that tap sets the palette.
