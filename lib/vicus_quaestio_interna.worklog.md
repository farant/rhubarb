# vicus_quaestio_interna.worklog.md

## 2026-10-14 - born: $question in process (http-vocatio T7, route B)

The `VicusPortaQuaestionis` that replaces the `bin/quaestio` helper:
`vates_vocare_rivum` on the vicus reactor. vicus now owns ONE reactor
(`apps/vicus/vicus.c`) and polls it with timeout 0 at the start of
every pulse (`vicum_pulsare`); the port's `pulsare` only reads its
handle (plan R1 - the host polls once for every reactor user). Route A
stays as `bin/vicus -quaestio adiutor` (R11).

- **The key (decision ...AAQVQ3, plan R2).** Read per request from
  `via_clavis` or `$HOME/.rhubarb/anthropic.clavis` (trimmed exactly as
  `apps/quaestio/quaestio.c` does) into the CALL's own piscina; the
  stack buffer is zeroed at once; `vates_aperire` makes Vates's single
  copy and ours is zeroed too. Every other copy (request header, the
  serialized request) lives in the same piscina, which is
  `piscina_vacare`d - every block zeroed - and destroyed when the call
  ends. Not buffer by buffer: Vates must keep its copy for retries.
- **Two piscinae.** The handle and the collected raw text live in the
  piscina vicus passes to `incipere` (they must outlive the call - vicus
  pulses once more after `finitus`); the key-bearing call lives in its
  own and is released only from `pulsare`/`abrumpere`, never inside the
  vates `finis` (code still runs after it returns).
- **finis copies, pulsare reports.** `_finis` maps the response with
  `quaestio_responsum_ex_vate` into the vicus piscina (error text, or
  usage on success - like route A's U frame); the next pulse reports
  `finitus` and releases the call.
- **Herbarium:** `sine_herbario` (R10). Route A did capture (bin/quaestio
  never set it); route B records nothing.

Tests `probationes/probatio_vicus_quaestio_interna.c` (6 sections vs
the scripted server, fake key in `$TMPDIR/vqi.<pid>.clavis`): bonum
(text across pulses, planned, usage 12/34, the key reached the
`x-api-key` header), clavis absens, mors (cut after a delta: error after
text), error (overloaded event), pravum (non-SSE after stream headers),
lente + abrumpere (finished within C ms, nothing left in the reactor).
No error text ever contains the fake key.
