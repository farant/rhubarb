# herbarium spec 2 - the guard before use (custos ante usum)

Born 2026-10-09 (Fran, while discussing norma slice B: "a certain
minimum is: is this an outlier payload that i need to save as a specimen
before i do anything else"). Follows `herbarium-spec.md`; comes BEFORE
norma slice B (inference reads the outlier pile, so the pile must be
complete first).

*The question: can a program that talks to an API be sure that every
response it did not expect is on disk before any code that might choke on
it runs?*

## 0. Data (quarta 31c0f02c)

- `herbarium_vectura` wraps the transport; `_exsequi_capiens`
  (`lib/herbarium.c:440-457`) presses a specimen when `status >=
  status_minimus` (default CD) - BEFORE returning the response, i.e.
  before the consumer sees a byte. Error bodies already meet the floor.
- 200 bodies are pressed only by vates, AFTER parsing: `vates_mittere`
  calls `_legere` (`lib/vates.c:650-735`), which computes novelty late
  (stop_reason check, then `_novitates_ex_norma`) and returns early on
  non-JSON / non-object bodies with novelty empty; the press follows the
  parse (`lib/vates.c:1604-1611`). Measured gaps:
  1. a 200 whose body is not JSON, or whose root is not an object, is
     NOT saved (VATES_ERROR_PARSE, novelty empty) - Fran's exact case;
  2. statuses 201..399 are neither parsed (vates: "not 200" = error) nor
     saved (below CD);
  3. order: the novelty check runs after field extraction - a crash in
     extraction would precede the save.
- herbarium's existing hook types: `HerbariumClavis(petitio, responsum,
  piscina, datum) -> chorda` (kind key). The norma judge works on any
  JsonValor and reports, never crashes on shape.

## I. The invariant

> Every response is classified at the transport boundary, from the raw
> status and bytes, as EXPECTED or OUTLIER. An outlier is written to the
> specimen directory before the transport returns it to the caller.

Classification may use only code that is safe on hostile input: status
comparison, `json_legere`, `norma_iudicare`. Field extraction happens
after, in the caller, and can assume nothing.

Crash safety: the specimen is written (`filum_scribere` / the append-only
index) before return; a later process crash does not lose it (page cache).
Power-loss durability (fsync of the specimen) is NOT in this slice.

## II. herbarium: a pluggable judge

```c
/* iudex: causa cur responsum INEXSPECTATUM sit; chorda vacua =
 * exspectatum. Vocatur in vectura capiente pro responsis sub
 * status_minimus, ANTE redditionem - specimen ante usum. */
nomen chorda (*HerbariumIudex)(
    HttpPetitio*   petitio,
    HttpResponsum* responsum,
    Piscina*       piscina,
    vacuum*        datum);

/* HerbariumOptiones gains: */
         HerbariumIudex  iudex;        /* NIHIL = status solus (ut olim) */
                 vacuum* iudex_datum;
```

`_exsequi_capiens`: status >= minimum -> press "status" (unchanged);
otherwise, if `iudex` is set and returns a non-empty cause -> press with
that cause. Then return the response untouched. herbarium stays generic:
it knows nothing about JSON or norma; the consumer brings the judge.

## II.b Where specimens live - on by default, one pile per API host

Decided with Fran 2026-10-09. Today capture is the consumer's choice and
OFF by default (`HerbariumOptiones.directorium` / `VatesOptiones.herbarium_via`
NIHIL = no capture) - a floor you must remember to switch on is not a
floor. New default, modelled on main's `gesta/fontes/annales_sedes.c`:

```c
/* sedes ordinaria speciminum: $RHUBARB_HERBARIUM, aliter
 * ~/.rhubarb/herbarium; deinde '/<hospes>' (acervus unus per hospitem
 * API). Chorda vacua + causa si $RHUBARB_HERBARIUM directorium non
 * exstans nominat (numquam tacite alio cadit), si $HOME deest, aut si
 * hospes '/' aut '..' continet. */
chorda
herbarium_sedes_ordinaria (
    constans character* hospes,
               Piscina* piscina,
                chorda* causa);
```

- Order: an explicit consumer directory wins; else `$RHUBARB_HERBARIUM`
  (must exist - a typo never silently captures elsewhere); else
  `~/.rhubarb/herbarium` (created). Then `/<host>`, e.g.
  `~/.rhubarb/herbarium/api.anthropic.com/` - each API its own pile, the
  unit norma slice B infers over.
- Outside every repository on purpose: specimens hold response bodies
  (model output); a repo gets one only by deliberate promotion into
  `probationes/fixa/...`. Growth is bounded by design (at most III
  variants per kind; repeat sightings only counted in `index.jsonl`).
- vates: `herbarium_via` NIHIL now means "the default location" (host
  from the provider's base URL); a new `b32 sine_herbario` in
  `VatesOptiones` turns capture off EXPLICITLY. If the default cannot be
  resolved, vates runs without capture and says so once on stderr (as
  `herbarium_aperire` already does) - never fails the call.
- Test isolation: once on by default, any test building a Vates without
  a path would write into the real `~/.rhubarb/herbarium`. Tests set
  `sine_herbario` or point `$RHUBARB_HERBARIUM` at a temporary
  directory; `probatio_vates` asserts the default resolution lands in
  that temporary directory (`<tmp>/api.anthropic.com/`).

## III. vates: its judge, and the late press removed

`_iudex_anthropic` (static in `lib/vates.c`), installed by
`_herbarium_adiungere`:

| Condition | Cause |
|---|---|
| status != 200 (and below CD) | `status inexspectatus: N` |
| body not JSON | `corpus non JSON` |
| root not an object | `radix non objectum` |
| schema notes/errors (`vates_norma_responsum`) | the joined notes, as today |
| unknown `stop_reason` | `stop_reason ignota: X`, as today |

The last two are today's novelty, moved: one helper
`_novitates_corporis(radix, ...)` computes it for the judge; `_legere`
no longer computes or returns novelty, and the press after `_legere` in
`vates_mittere` is deleted (one capture point, not two).

## IV. Testing

- **herbarium (`probatio_herbarium`)**: a scripted inner transport + a
  judge that flags bodies containing "x": flagged 200 pressed with the
  judge's cause, unflagged 200 not pressed, 429 pressed as "status"
  without calling the judge, judge NIHIL = old behaviour.
- **vates (`probatio_vates`), the invariant**: through `vates_mittere`
  with a scripted transport and a temp herbarium directory, each of:
  non-JSON 200, array-root 200, empty 204, 302, 200 with an unknown
  field, 200 missing a required field, unknown stop_reason, plain valid
  200 (NOT pressed), 429 (pressed by status). Assert: specimen present
  (or absent for the valid one), cause as in §III, and vates' own result
  unchanged (errors stay errors).
- **Order, structurally**: the same hostile bodies through
  `herbarium_vectura` + the vates judge WITHOUT `vates_mittere` - the
  specimen exists although nothing parsed the body. The capture does not
  depend on the consumer's parse.
- Plants: judge never called -> invariant red; judge called after
  return (moved into vates again) -> order test red; `_legere` still
  pressing -> double press visible in `index.jsonl` counts.

## V. Tasks (quarta; one per turn)

| Task | Content |
|---|---|
| H0 | headers to Fran: herbarium.h (`HerbariumIudex`, `iudex`/`iudex_datum`, `herbarium_sedes_ordinaria`), vates.h (`sine_herbario`, `herbarium_via` NIHIL = default) |
| H1 | herbarium: judge in `_exsequi_capiens`; `herbarium_sedes_ordinaria` (env, home, host, refusals); tests, worklog |
| H2 | vates: `_iudex_anthropic`, `_novitates_corporis`, late press removed, default location + `sine_herbario`, test isolation, invariant + order tests, worklog; `tools/vates_fumus` keeps an explicit path |

## VI. Not in this slice

- A non-HTTP guard (files, stdin, other boundaries) - same pattern, own
  shape later.
- Responses lost in transport (truncated / reset): there is no body to
  save; the transport error is the record.
- fsync of specimens.
- norma slice B (inference over the pile) - next.

## AUDIENDA

- Not verified: that no committed specimen or test depends on the late
  press's cause text format (H2 runs the suite; the stored cause is
  metadata, the kind key is unchanged).
- Not verified: herbarium tests' current transport-scripting helper can
  return arbitrary statuses (H1 reads it).

## As built (2026-10-09, quarta dcf7589b..4b18fa1c)

Done as specified (H0 headers, H1 herbarium d983bffa, H2 vates 4b18fa1c).
Departures:

- **Order test.** The "judge without `vates_mittere`" variant of §IV
  would need vates' judge public; it stays static. Order is proven by
  H1's in-loop count (the wrapper has the specimen on disk when the first
  flagged response returns) and H2's non-JSON case (saved although vates'
  own parse exits early - the measured gap).
- **Hosts.** Anthropic's host is derived from `VATES_URL_ANTHROPIC`; the
  `fictus` test provider captures into its own pile `<sedes>/fictus` so
  fake responses never join the real one.
- **Not a part of this slice but seen during it:** the `aedilis` gate
  once returned rc=1 with an all-green log (ledger …CD1ZY, third
  sighting across worktrees) - suspected tabularium resident rewriting
  `gesta/build/` during the gate.
