# atrium.worklog.md

## 2026-08-13 — birth: the app layer over vitrea

Built from the decree parked at `01KZYHBD7W`. Design conversation with
Fran the same day; the reasoning that survives is in `atrium.h` — this
log is for what the code cost, not what it means.

### What the measurement actually said

Before writing anything I read the setup + loop + teardown of all three
apps (`forum.c`, `villa.c`, `mensor_ui.c`) rather than trusting the
decree's "~60-70 lines". The decree was right about the volume and
understated the interesting part.

Identical in all three: `figura_fenestrae` (only titulus/size differ),
`fenestra_creare` + check, `capsula_aperire` + check, **`figura_vitreae`
byte-for-byte identical**, `vitrea_creare` + check, `internuntius_creare`,
the loop condition, the two drain loops, the `PONS ? tractare : recargare`
branch, and the teardown order.

What varies is exactly one thing per app: a single non-blocking step
(`villa_agens_pulsare` / `processus_pulsare` / `vivarium_gressus` + inject).
That's what made "app keeps its loop" viable — the seam is small enough
that it doesn't need a callback at all.

### Loop ownership: the arena was a false objection

My first design had atrium owning the loop (`atrium_currere` + a tick
callback), justified partly by the per-tick arena: villa and forum pair
`piscina_notare` at tick top with `piscina_reficere` at tick bottom, and
a *pair* wants a scope.

Fran pushed back — prefer no inversion of control if it can be made
ergonomic. He was right, and the fix dissolved the objection rather than
working around it: **reset the arena on ENTRY to `atrium_gressus`, not on
exit.** One call does both halves, nothing to pair, no `PiscinaNotatio`
in the app.

It's also strictly safer than what the apps do today. `forum.c` allocates
JSON from `piscina_vocationis` in the fumus block and then resets the
arena *at the bottom of the same tick* — those allocations die
immediately after use. Reset-on-entry keeps them alive until the next
gressus. Villa's agent deliberately avoids the call arena entirely
(comment at `villa.c:799-802`), so it's unaffected.

With the arena settled, every remaining argument for owning the loop was
satisfiable without it: teardown order → `atrium_destruere`; loop
condition → `atrium_currendum`. Inversion bought nothing.

### Things found while reading that atrium fixes for free

**`VITREA_NUNTIUS_INTERITUS` is not a reload request.** It's the web
content process *dying*. All three apps respond with `vitrea_recargare`,
so it became house policy.

**Latent bug in mensor_ui:** after that crash-reload the page is fresh,
so the injected `window.MENSURAE` is gone and never comes back. mensor
doesn't handle it — its data never changes, so nobody has seen it.
`atrium_iniectionem_ponere` re-runs on every `paratus`, which fixes it
without anyone asking.

**A claim I had to walk back.** I initially wrote in the header that
"three apps answered the bridge-message question three ways." Checking
before building on it: only mensor's page calls `postMessage` at all —
villa and forum talk through vitrea's injected bridge and send nothing
but JSON. So it was really *one* app needing the guard, and two that
never hit the case. Header now says that. The design still holds (any app
that wants injection must announce readiness, and would otherwise
re-derive the guard) but the motivation is one measured case, not three.

### Ordering hazard, handled

`atrium_iniectionem_ponere` fires immediately if the page has *already*
announced itself, and `atrium_gressus` fires it on `paratus` if it's
already set. Either order works. Without the first half, setting the
injection after the page loaded would silently never run — the worst
failure shape (no error, empty page).

### Testing: what's real and what isn't

`probatio_atrium.c` is 37 assertions, headless, and deliberately does
**not** test the assembly. Everything it covers is logic that can fail
*quietly*: message discrimination, argv parsing, the four refusal paths,
NIHIL-safety on every accessor. A broken assembly gives an empty window
or a crash — loud. A broken discriminator gives console noise nobody
reads until it buries a real error.

All four `atrium_creare` refusals return before `thema_initiare`, so they
test without AppKit. That ordering is load-bearing for the test to exist
at all — if someone moves validation below window creation, the test
silently becomes untestable-in-CI. Worth knowing before rearranging.

Two traps hit while writing the test itself:
- `memcpy(tabula, "paratusIMMUNDITIA", XVII)` into `i8 tabula[XVI]` — a
  17-byte copy into a 16-byte buffer, in the test that exists to prove
  we respect chorda lengths. Caught by reading, not by the compiler.
- `character* argv[]` entries must be `character a0[] = "..."` arrays,
  not string literals — `-Wwrite-strings` rejects the literal form.

**Fault planted before believing the green:** removed the
`mensura == VII` equality from the discriminator, leaving only the
`memcmp`. Suite went to exit 1, 36/37, failing exactly the
not-null-terminated case. Reverted, back to 37/37. The length check is
genuinely load-bearing and the test genuinely guards it.

### Not proven yet

The assembly and the loop have **no automatic test** and cannot get one —
they need a real window. Their gate is the migration: villa `-fumus`,
forum's full-smoke, and `mensor_ui -json`/`-imago` are existing
end-to-end checks, and porting those three apps onto atrium is the
integration test. Until that's done, atrium compiles and its pure parts
are proven; the assembly is claimed, not demonstrated.

### Deferred

Build-time half is untouched: speculum still needs a generated source
capsula plus an `<aedilis obiectum=...>` annotation. The `fontes` field
makes the omission *visible* (mensor_ui would now be choosing to skip
speculum rather than nobody noticing) but not free. That's the `fabrica`
thread — parked `01KZYN4VPZ`, with Fran's refinement that producers
should be arbitrary operations with declared inputs/outputs and
**content hashes** rather than mtimes.
