# Fines — structured value constraints on canon attributes

**Date:** 2026-08-08 · **Status:** spec + plan combined (design settled in conversation; grounded against code)
**Origin:** documentation loop I, misses L4/L7 (project-specs/documentatio-lacunae.md). Two independent probes measured the same wall: `probabilitas="0.7"` and `praecisio="0.5"` both rejected — the one property documenting its own 0–1 range can legally hold only 0 and 1.

## 1. Ruling (Fran, 2026-08-08)

Value **constraints** are part of a property's nature and belong in grammar, not prose: ranges, integer-vs-decimal, "all that kind of stuff." Machine **types** (double, char*) stay out. The line: canon may state a value's *written form and value space*, never its storage. Precedent: `dies` already states written form (yyyy / yyyy-mm-dd).

## 2. Design

Constraints are **attributes on `<attributum>`** — no palette growth:

```xml
<attributum nomen="probabilitas" genus="numerus"
    fractio="verum" minimum="0" maximum="1"/>
```

- `fractio="verum"` — this numerus admits one fraction point (digits required on both sides: `0.5` yes, `.5`/`5.` no). Default falsum: all existing sites keep integer strictness; integer-ness stays load-bearing for counts.
- `minimum=` / `maximum=` — INCLUSIVE bounds, themselves written in the same lexical form (decimal bounds legal). Enforced at judgment with a NEW vitium `CANON_VALOR_EXTRA_FINES = XIV` (diagnostic names the bound: form failures stay `VALOR_MALUS`).
- Scope v1: `genus="numerus"` only. Declaring fines/fractio on any other genus = canon rejected at load (canon mendax). Typed element text (`textus="numerus"`) does NOT take fines v1 (deferral).
- Closed declarative vocabulary —**no expression language, ever**. A future constraint is a new attribute + ruling, never a pattern string.
- Comparison implemented WITHOUT floats: parse (sign, integer digits, fraction digits), compare sign → stripped integer magnitude → fraction digits padded. Fitting, in a house with no doubles in sight.
- `ordinarius=` must satisfy fines at canon load (existing "praestitutum mendax canonem frangit" doctrine extends); `minimum > maximum` or ill-formed bounds likewise reject at load with causa.
- **Name note:** `<liberum>` already carries `minimum=`/`maximum=` as CARDINALITY. Same attribute names on a different element — value-bounds live on `<attributum>` only. Deliberate reuse; context disambiguates; canon.canon documents both.
- **Self-hosting:** canon.canon declares the new `minimum`/`maximum` attributes as `genus="numerus" fractio="verum"` — the mechanism describes itself and is judged by itself.

Chain: `.genera` `<proprietas>` gains the same three attributes (natura.canon; loader untouched — hybrid depth) → `natura_canones` carries them verbatim through `NcMembrum` → generated `<attributum>` carries them → `lib/canon.c` enforces.

Corpus v1 (measured sites only): `periculum.probabilitas` fractio+0..1; `mensura` `quantitas.valor` and `quantitas.praecisio` fractio. Others as future content forces.

**Deferrals:** rich value kinds (`genus="quantitas"`, 31 flattened sites — own walk); fines on typed element text; fines on `dies` (would give the BC ruling 01KZC7F388 a lawful home — Fran's ruling stands open); multiplex name-only hole (01KZE65SAM, separate mechanism).

## 3. Plan

### T1 — Mechanism in canon (lib/canon.c + canon.h + canon.canon), tests first
- probatio_canon.c new fixtures + section (RED first, pre-fix oracle: old code ignores unknown `fractio=` so `0.5` fires VALOR_MALUS where test expects 0):
  - fixture canon attr `gradus` numerus fractio minimum="0" maximum="1"; judge: `0.7`→0 vitia; `0.5` on an UNfractioned numerus→VALOR_MALUS; `1.5`→EXTRA_FINES I; `-1` with minimum="0"→EXTRA_FINES; boundaries `0`/`1`→0; `0.50` vs maximum `0.5` equal→0; `.5`/`5.`→VALOR_MALUS.
  - load rejections (canon_legere→NIHIL+causa): ordinarius extra fines; minimum>maximum; fines on genus="textus".
- canon.h: `CANON_VALOR_EXTRA_FINES = XIV` (grep the enum window first — collision lesson).
- lib/canon.c: CanonAttributum += `b32 fractio; chorda* finis_minimus; chorda* finis_maximus;` (init at load site ~:760; INIT THE STACK TMP in the typed-textus path ~:1180 too). numerus branch of valor_congruit takes fractio into account (signature: pass a; it already receives CanonAttributum*). New `_numeri_comparare(a,b)` helper + `_intra_fines(v,a)`; judgment loop ~:1144: form ok + fines present + !intra → vitium XIV. Load branch ~:798: capture three attrs; validate genus==numerus, bounds well-formed, min<=max, ordinarius intra.
- nuntius: "valor extra fines".
- canon.canon: attributum def += `fractio` (veritas), `minimum`/`maximum` (numerus fractio=verum), nota naming the liberum-cardinality name-reuse.
- Rebuild ./tools/canon_struere.sh; `bin/canon_examen canon.canon` 0 vitia; ./tools/canon_coquere.sh -probare 0; ./compile_tests.sh canon green. Commit.

### T2 — natura grammar + catalog
- natura.canon `<elementum nomen="proprietas">` += same three attribute declarations.
- Regenerate METAMODULUS §3 catalog: ./tools/natura_metamodulus_generare.sh (NEVER hand-edit the table).
- probatio_canon disk-judgment section already loads natura.canon — add a mini-genera fixture with a fractioned proprietas → 0 vitia (RED first: old natura.canon lacks the attrs → ATTRIBUTUM_IGNOTUM).
- Gates: bin/natura_examen; visus catalog gate. Commit.

### T3 — Projection + corpus + sentinels
- Sentinels first in probatio_natura_canones (RED): natura/cocta/periculum.canon contains `fractio="verum" minimum="0" maximum="1"`; then a judgment probe: `<periculum nomen="#p" probabilitas="0.7"/>` under regenerated individua.canon → 0 vitia (descriptor's exact refused probe, now green).
- tools/natura_canones.h NcMembrum += `chorda* fractio; chorda* finis_minimus; chorda* finis_maximus;`; NIHIL-init at EVERY `xar_addere(el->membra)` site (grep them all); capture in the proprietas branch (~:1320) verbatim from the node; emission in `_membrum_attributum_scribere` (~:732) writes them (numerus only) between genus= and ordinarius=.
- Corpus: periculum.genera probabilitas + mensura.genera quantitas.valor/praecisio gain the attrs; notas trimmed to what grammar now says.
- Rebuild natura_struere; regen ./tools/natura_canones.sh (expect periculum/mensura/individua canons to change); suites green. Commit.

### T4 — Records
- Worklogs (lib/canon.worklog.md mechanism entry; tools/natura_canones.worklog.md carry-through), memory updates, tabularium: close the L4 thread on nota 01KZFBS83Y; note the BC-door opened. Commit.
