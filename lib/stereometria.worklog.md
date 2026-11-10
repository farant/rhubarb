# stereometria worklog

## 2026-11-09 - T1 of project-specs/simulacra-plan.md

- **The name.** Planned and approved as `spatium`. That word is
  taken: `spatium` is the component tree's namespace (componens.h
  `spatium`, `componens_ponere_spatium`) and `probationes/
  probatio_spatium.c` is its suite. Writing the new test through
  `silva/scribe.sh` overwrote that tracked file (scribe always
  writes); restored with `git checkout HEAD --`, both suites green
  after. Renamed to `stereometria` (solid measure). Lesson, again:
  `git ls-files | grep <name>` before choosing a file name - the
  check on `include/` for a `spatium_` prefix was not enough.
- **A new lib or probatio is not seen by `./compile_tests.sh`** until
  `./tools/compile_tests_fontes_generare.sh` regenerates
  `compile_tests_fontes_generata.sh` (the first run failed at link
  with every symbol undefined).
- **C89:** `f().e[k]` on a returned struct is "subscripting non-lvalue
  array" under -pedantic. Take the matrix into a local first. Member
  access `f().x` is fine.
- `stereometria_in_trs` was added to the header beyond the plan's
  sketch: the GLB reader needs it for nodes written as `matrix`.
  Mirrors (negative determinant) negate scale x; shear is dropped.
- Conventions are in the header (right-handed, column-major, column
  vectors, radians, quaternion x y z w). Proved by a plant: the
  product written transposed turns section "multiplicare" red.
- Matrices pass by value (64 bytes). Revisit only if the renderer's
  measurement (T4) says so.
