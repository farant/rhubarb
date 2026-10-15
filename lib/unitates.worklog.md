# unitates worklog

## 2026-10-15 - born (U3 step 2b, quinta)

Reader of `unitates.stml` (policy) + every `DIR/unitas.stml`
(declarations); answers "what is this file" with a CAUSA per field.
Spec `project-specs/unitates-codicis-spec.md` §VI⅞f; header doc is the
contract.

Design points that are not obvious from the code:

- **Sutura, not disk** (copied from `portae.h`): the lib never opens a
  file. Tests give an in-memory tree; the CLI will give
  `git ls-files -co --exclude-standard` + disk reads. Declarations are
  found in the enumeration, never by walking (a walk needs its own
  "skip build/" list - the very thing the arc removes).
- **Provenance is lazy.** `unitates_legere` reads only the policy and
  the declarations; `quid` reads line 1 of the asked file; `iudicat`
  reads it LAST, after custody/lifecycle/role/language - so asking the
  lint about `vendor/sqlite3.c` (9 MB) never reads it.
- **Inheritance**: custody = declared on the unit, else on the nearest
  ancestor that declares one, else the INNERMOST unit's kind default.
  Lifecycle = declared, else nearest ancestor's, else `viva`. A unit
  nested in `archivum` is archived; a unit with no custody inside one
  declaring `aliena` is foreign even if its own kind defaults to
  domestica.
- **Library beats the directory around it**; backends (`S_macos`)
  alias their base in the stem table (two passes, a backend's base may
  itself be a backend); family children get the family root as parent.
  `probationes/probatio_X.c` -> longest `_`-prefix of X that is a
  library stem, else a directory unit with that title (`pictor`),
  else the innermost directory (root). Library wins a tie with an app
  of the same title (`vicus`).
- **Identity = kind + title** (app vicus and library vicus coexist).
- Values are `chorda`, never enums: custody/lifecycle/role/language/
  usus are open sets declared in the policy; unknown -> refusal with
  the known list. `provenientiae` is the one closed set (`manu
  generatum`), because it is derived from a marker, not declared.

Plants (silva.planta, all red at the intended assertion, restored):
first-match roles, lifecycle inheritance, custody inheritance, backend
suffixes, provenance read in the judge. **The lifecycle plant first
stayed GREEN**: the only archived fixture sat directly in `archivum`,
which declares its own `vita` - inheritance was never exercised. Added
`archivum/materia` (nested, no vita) and `ora` (cura aliena) +
`ora/clay` (none) to the test world. A suite green on day one proves
nothing until a plant turns it red.

Test gotcha: C89 caps a string literal at 509 bytes (`-pedantic
-Werror` -> overlength-strings), concatenation included - the fake
policy file is an array of lines joined at read time.

**Lifetime of the sutura**: `Unitates` keeps the `UnitatesSutura*`
(lazy provenance reads go through it after `unitates_legere`
returns), so the caller's sutura and its datum must outlive the
`Unitates`. The first test put them in a stack frame as `static`;
examen's credo rule refused it (state across sections) - a plain local
would have dangled. Allocate both in the same piscina as the result.
