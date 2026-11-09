# pulsus worklog

## 2026-11-02 - T1 pulsus_forma (pure forms)

- One module for every JSON shape pulsus moves: event (client -> server),
  inscription (client -> server), APNs payload (server -> Apple), tap
  (payload -> app). Server and clients link only this, so the two ends
  cannot drift.
- Limits are C10 of the plan. Validation lives in the readers only; the
  writers trust their caller (the server judges every event anyway).
- Placeholders: a `{` that does not start `{persona}` or `{numerus}`
  rejects the template, so a typo like `{nomen}` is a 400 at the client's
  first send, not a literal brace on someone's lock screen.
- Payload truncation drops ids from the END one at a time and marks
  `"plures":true`; the app then opens the Shared grid instead of a
  partial group. Linear, but bounded by a batch of at most M ids.
- `json_scribere` is compact and keeps insertion order - the payload test
  pins the exact octets of spec III.1.
- Gotcha: a new lib/*.c needs `./tools/compile_tests_fontes_generare.sh`
  BEFORE its suite links (the plan listed it after PASS).

## 2026-11-02 - T2 pulsus core

- Time is pulled: `pulsus_maturos_mittere` sends every batch whose
  `ultimus_ms + mora_ms <= nunc`; T3's reactor timer will only call it.
  Tests drive a fake clock and call it directly - no sleeping.
- A batch row is DELETED before its notifications go out. The fake (and
  any synchronous) sender calls `finis` inside `mittere`, and a crash
  mid-send must never resend: at most once beats at least once for a
  "Fran shared 3 pictures" banner.
- `PulsusFinis` hands the sender's copy of the message back (ruling vs
  the plan), so pulsus knows which (apparatus, thema, token) finished
  without allocating a context per message. MORTUUM deletes only when the
  token still matches - a re-registration racing a 410 is not lost.
- Recipients are computed from `communio_apparatus_enumerare` each send:
  same space, other PERSON, person active, device not revoked. Revoked
  devices' inscriptions are purged in that same pass (no hook into
  communio_servus needed).
- Notification title = last segment of the thema ("pictor"); the server
  has no other source for the app's name.
- Bug caught on review: `scrinium_gradi` returns SCRINIUM_ERROR (0) on
  failure, not -1 - a `!= -I` test was always true.
- Non-statim event with no ids is a no-op (it would otherwise announce
  "0 pictures"); statim with no ids flushes a pending batch or does
  nothing.

## 2026-11-02 - T3: viae et hora reactoris

- Two routes beside communio's, same Bearer as communio (`communio_apparatum_verificare`): `PUT /communio/pulsus/inscriptio` -> 204, `POST /communio/pulsus/eventus` -> 202. Readers are T1's `pulsus_*_legere`; their `causa` goes back as `{"error":causa}` with 400.
- Body cap PULSUS_PETITIO_MAXIMA = 64 KiB, checked BEFORE parsing. Exemplar's hospitium allows 64 MB and spills bodies > 1 MB to a temp file (`corpus_via`), so without the cap a 2 MB junk body would be parsed in full. Spilled bodies are refused too.
- One reactor timer, re-armed after mount (resumes batches from a previous run), after every event, and after every fire. One-shot timers are marked inactive by the reactor BEFORE the callback, so `_ictus` clears `hora_viva` first and may re-arm from inside. Delay clamped to [1 ms, 1 h]; a farther batch gets an empty fire and a re-arm.
- `pulsus_destruere` cancels the timer, so it must run before the hospitium's reactor dies (exemplar: before `communio_servum_destruere`; tests: `rete_purgare` destroys pulsus first).
- The HTTP test sections run pulsus on the REAL clock: with the fake clock the timer fires but `pulsus_maturos_mittere` still sees the fake "now" and sends nothing.
- Exemplar logs `pulsus: apns absens - notificationes non mittuntur` at start until T5 gives it a sender.

## 2026-11-02 - T6: first Linux run of the push stack

`./tools/linux/probatio.sh -musl hpack http2_forma http2 tessera_es256 pulsus_forma pulsus apns_mittens` (+ cives as a control) and `./tools/linux/aedificare.sh apps/exemplar/exemplar.c`. hpack/http2/tessera/pulsus/apns_mittens needed **no** source change for Linux - every failure was elsewhere:

1. **Nothing compiled: `lib/credo.c:2057`** (gcc -Werror=format-overflow). `expressio[CCCLXXXIV]` got `"sectio '%.120s': ... %.200s (volumen_claudere deest)"` = up to 390 octets: a real overflow, not a gcc false alarm, in since 1d9b1756 (2026-10-23). clang on the Mac never warned, so every Linux suite run since then failed to compile. Buffer -> DXII. Same class in `probatio_apns_mittens.c` (`%s` of `signa[k]`, gcc cannot bound a 2-D row): `%.64s`, buffer DXII.
2. **Every temp-volume suite failed at run time** (pulsus, and cives too - not ours): alpine containers have no `/etc/machine-id`, `machina_identitas` refuses (by design, "RECUSAT, NON DEGENERAT"), a volume without a machine cannot append acts, so `volumen_creare` returns NIHIL silently at its first act. Fix in the runner, not the library: `tools/linux/probatio.sh` makes `build/porta_linux/machine-id` once (random, stable per host) and mounts it read-only at `/etc/machine-id`. The droplet has its own machine-id - exemplar there was never affected. Found with a throwaway diagnostic printing scrinium/machina/volumen step by step (nothing in the volume path logs this failure - worth a line in volumen someday).
3. **exemplar Linux build**: `_via` (`sprintf "%s/%s"` after a length check) - fine while every caller passed a literal, but T5's `_apns_parare` passes `clavis_via` from apns.stml and gcc cannot carry the check into the format. `_via` now memcpys under the same check.

Result: 8/8 suites green under musl (Go-oracle sections print NIHIL CURSUM in the container; they ran on the Mac), `build/linux/exemplar` static x86-64; under `--platform linux/amd64` it starts, logs `pulsus: apns absens - apns.stml deest`, answers `/salus`.

## 2026-11-05 — tergum T2: `content-available:1` in the alert push

`_corpus` (lib/pulsus_forma.c) now puts `"content-available":1` in `aps`
after `thread-id`. One push, not two: iOS shows the banner AND wakes the
app in the background (testa's `didReceiveRemoteNotification:
fetchCompletionHandler:`, tergum T5) so pictor can pull before the tap.
The key costs 22 octets; truncation measures the rendered body, so the
4096 limit holds without changing the budget (test: 400 ids still carry
the key, `plures:true`). Takes effect on devices only after the exemplar
deploy (tergum T7); until testa has the handler, iOS ignores the key for
an app without `UIBackgroundModes` remote-notification.
