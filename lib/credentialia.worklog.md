# credentialia worklog

## 2026-10-20 - born (ianua T2)

Passwords per person, moved out of exemplar's `Opus` slots and
`_intrare_post` / `_verificatio_facta` / `_renovatio_facta`
(exemplar_nucleus.c :542-906; exemplar keeps its copy until ianua T5).
Spec II.2, plan T2.

Design notes:
- Hashing split from writing: `credentialia_digerere` (custodia worker)
  hands back the PHC; `credentialia_tesseram_ponere` writes it inside
  the CALLER's transaction (P3). One password per person: put deletes
  the previous `tessera` row first.
- `credentialia_verificare` answers synchronously with a start code
  (P5): COEPTUM (callback later, always from the reactor), FRENATUM
  (no callback ever), OCCUPATUM, PRAVUM. The rate-limit delay is a
  reactor timer, so the call never blocks.
- Unknown person, suspended/deleted person, or a person without a
  password all verify against an EMPTY phc -> custodia's full dummy
  hash -> same work, bona FALSUM. The test compares
  `custodia_gradus_summa` deltas, not wall time.
- Frenum keys unchanged from exemplar (P6): "par:<ip>", "usor:<handle>".
  Both are notified on completion.
- The slot lives until the callback, or - when custodia reports
  `renovanda` - until the rehash is written: the password copy is still
  needed (exemplar :796-801). The rehash is the ONE place the library
  opens its own transaction; it runs in a reactor callback, after the
  reply, so it never nests in a caller's transaction.
- If custodia's queue is full after a rate-limit delay (rare), the
  callback reports FALSUM (cannot return OCCUPATUM from a timer).

Tests: probatio_credentialia (credo v2, 8 sections, 162 assertions,
~1.3 s with cheap argon2: m 64 KiB, t 1). Green on macOS, Linux musl +
glibc.

Plants (all red then green): PHC written into the event ->
acta_sine_secretis red; status check removed -> status_non_activus red;
delay ignored -> frenum red (elapsed < 240 ms); dummy hash skipped for
unknown handles -> ignotus red (work deltas differ).
