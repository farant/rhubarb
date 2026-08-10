# silex novum -vitrea — the vitrea seed + generated build tiers (spec)

2026-08-10. Approved scoping from discussion: the low-discretion
remainder of desideratum 01KZ6CSBQN — items 1/2/3/6 of the -vitrea
gap list, with the two macro-design items (4/5) explicitly DEFERRED
on their own triggers. Prerequisite work all shipped: the corpus
(silex-corpus-spec.md), closure rules (.m/vendor), re-rooted
renovare, `silex partes`.

**One sentence:** `silex novum <titulus> -vitrea` scaffolds a
complete vitrea+volumen app — vocabularium's architecture,
miniaturized — with build scripts GENERATED as projections carrying
all four hand-carried tiers, their per-target source lists computed
by silex's own closure walk.

## Rulings

| Fork | Ruling |
|---|---|
| Seed shape | **Bridge + volumen heartbeat**: the hello handler appends a «salve» actum to `~/.rhubarb/<titulus>.volumen` and returns the count — bridge round-trip AND persistence proven on screen; closure pulls volumen→scrinium→sqlite so ALL FOUR tiers exist at birth and script generation needs no conditionals. |
| Generated scripts | **Projections** (Fran, 2026-08-10: "definitely comfortable with a class of scripts that are generated/projections") — same standing as vendored libs: copies are honest because something upstream can regenerate them. Hand-editing allowed; scripts carry a stamp naming their generator + corpus stamp. |
| Flag battery / SQLITE defines | **Stamped copies** in the generated scripts (v1 dodge). The shared machine-readable `vexilla` source of truth is DEFERRED — trigger: a third consumer appears or the battery changes. |
| Script regeneration covenant | **DEFERRED entirely** — trigger: renovare adds a file a generated script doesn't cover and it bites (then a partes-style staleness warning is the likely v1). Nothing in this spec regenerates or inspects existing scripts. |

## The seed (reference: vocabularium, miniaturized)

`silex novum <t> -vitrea` produces, beyond what plain novum makes:

```
<t>/
  <t>.volumen                  VCS document (as today)
  aedificare.sh                GENERATED, four tiers (below)
  probare.sh                   GENERATED, per-target (below)
  README.md                    vitrea-flavored usage
  fontes/<t>.c                 wiring: fenestra+capsula+vitrea+
                               internuntius + drain loop (villa
                               pattern; loud volumen refusal)
  fontes/<t>_pipa.h/.c         seam: Pipa{Volumen*} + handlers
                               salve {} -> {nuntius, numerus}
                               (appendere actum 'salutatio' +
                               count via volumen_acta_legere)
  assets/<t>.toml              capsula manifest (index.html, <t>.js)
  assets/index.html            minimal shell (titulus + <p id=...>)
  assets/<t>.js                internuntius.vocare('salve') ->
                               renders nuntius + numerus
  instrumenta/capsula_generare.c   vendored FROM THE FONS
  include/ lib/ vendor/        closure of the seed fontes
                               (vitrea tier + volumen tier + .m
                               pair + sqlite pair)
  probationes/probatio_<t>.c   credo: calls the salve handler
                               directly against build/probatio.volumen
                               (vocabularium's probatio shape)
```

Data volumen at `~/.rhubarb/<t>.volumen` (mensa convention — NEVER
in the project dir; identity rule). App architecture = the pipa
split, so the probatio links `fontes/<t>_pipa.c` without a second
`principale`.

Seed sources live as template strings in silex (the existing
SEMINA mechanism), with `<t>` substitution. Seed closure = scan the
template contents with the existing include scanner — the
templates' own includes are the truth, never a separate list.

## Generated aedificare.sh (four tiers, all unconditional)

Stamp header: `# GENERATUM a silice (novum -vitrea, <corpus
stampa>) - PROIECTIO: mutare licet` + the four-tier rationale
comment. Then:

1. **Instrumentum**: build `build/capsula_generare` from
   `instrumenta/capsula_generare.c` + its lib list, **computed at
   novum time by the closure walk over the vendored tool source**
   (today that computes to: piscina chorda chorda_aedificator xar
   toml filum flatura via — but the list is COMPUTED, never
   hardcoded); mtime-guarded; then run it on `assets/<t>.toml`
   every build (capsula ante omnia).
2. **Vendor**: `vendor/sqlite3.c` → `build/sqlite3.o`, relaxed
   flags + the 7 SQLITE defines (stamped copies), mtime-guarded.
   The rule is emitted for every `vendor/*.c` in the closure, not
   just sqlite by name.
3. **App**: house battery + `-Wno-overlength-strings
   -fbracket-depth=512`, `-Iinclude -Iassets`, explicit source list
   = fontes/<t>.c + fontes/<t>_pipa.c + assets/capsula_<t>.c +
   computed lib list (.c AND .m from the closure) + vendor objects,
   `-framework Cocoa -framework WebKit` (convention, hardcoded v1).
4. **Install**: rm before mv (SIGKILL trap).

## Generated probare.sh (per-target)

Same stamp + vendor tier; compile line = probationes/probatio_<t>.c
+ fontes/<t>_pipa.c + **the probatio target's own computed closure**
(no `.m`, no frameworks, no glob — gap 6 closed at the root). Runs
the binary; exit honest.

Plain (non-vitrea) novum's generated scripts are UNCHANGED.

## Mechanics in silex

- `-vitrea` vexillum on novum (argumenta flag; SilexNovumOptiones
  gains `b32 vitrea`).
- Script generation = template text + computed sections written via
  chorda_aedificator; source lists from `silex_clausuram_colligere`
  results partitioned by via prefix (include/ → -I only; lib/*.c,
  lib/*.m, vendor/* → their tiers).
- The tool source (`tools/capsula_generare.c`) is read through the
  FONS (it is in the corpus), so `-vitrea` works with the fabrica
  unplugged.
- Everything vendored/generated is condere'd in the birth conditio
  exactly as plain novum does.

## Testing

- probatio_silex: a `-vitrea` scaffold section — scaffold into the
  AREA, assert the inventory (pipa pair, assets, instrumenta, .m
  pair in lib/, vendor pair, both scripts contain their stamp line
  and the computed tool lib list), assert generated scripts contain
  NO `lib/*.c` glob. Scaffolded tree is NOT compiled by the suite
  (dogfood is compiled in silicetum, per the existing doctrine).
- The seed pipa handler logic is exercised by the SCAFFOLD's own
  probatio (generated), not by probatio_silex.

## Acceptance (item 6 — the gate)

In silicetum, with the fabrica renamed away (corpus-only):
`silex novum salve-vitreum -d . -vitrea` → `./aedificare.sh` →
`./probare.sh` green → `./bin/salve-vitreum` opens a window showing
«salve, munde…» + the visit count, which INCREMENTS across
relaunches (persistence visible) → `silex partes` shows M and V
entries ADEST → `silex condere`. Tier-class parity with
vocabularium's inventory (C/O/M/V + instrumenta all present).

## Out of scope

Shared `vexilla` data file (deferred, trigger above); script
regeneration/staleness detection (deferred, trigger above);
multi-platform frameworks; phone surface; mensa in the seed;
changing plain novum's output; closing 01KZ6CSBQN happens at
acceptance, with the two deferred items re-homed to a fresh
desideratum.
