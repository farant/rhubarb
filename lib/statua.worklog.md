# statua worklog

## 2026-11-09 - T2 of project-specs/statuae-plan.md

- **The name.** Planned as `simulacrum`. In this house that word is a
  test double: `probationes/horreum_simulacrum.h` defines `Simulacrum`,
  `SimulacrumPars`, `simulacrum_creare` and ~20 more, and
  `horreum_simulacrum_rete.h` defines `SimulacrumRete`. Fran chose
  `statua` (2026-11-09). Found by `git grep` before any file was
  written, after `spatium` was found the hard way in T1.
- **A loop returns to its start at t == duration.** `statua_positura`
  wraps time into [0, duratio), so the last key is only ever
  approached, never shown (glTF loops repeat the first pose as the
  last key, so nothing is lost). The plan's "gradatim" test asked for
  the second key's value at t = 1.0 of a 1 s clip; that contradicts
  the wrap, so the test uses three keys and checks the middle one.
- **The bounding sphere samples without wrapping** (internal
  `_positura` takes resolved time), so both ends of each clip are in
  it: 32 evenly spaced times including 0 and duratio, plus the rest
  pose. Centre = centre of the box over all sampled vertices, radius
  = largest distance from it. Only vertices reached through a node's
  mesh count; an unreferenced mesh does not enlarge the sphere.
- **`statua_finire` is the one validator.** Every index a later stage
  follows is checked there (parents, meshes, parts, vertex indices,
  materials, textures, channel nodes, key times non-decreasing and
  not NaN). The pose function and the renderer may then index
  without checks. Key times are compared with negated `>=` so NaN is
  refused.
- No allocation in `statua_positura`: per node it scans the clip's
  channels (golem: 54 nodes x 140 channels, ~7.5 k comparisons per
  frame). If T4's measurement shows it, sort channels by node once in
  `statua_finire`.
- Plants: blend factor forced to 0 -> "linearis", "gradatim",
  "involutio", "sine_canali" red; wrap removed -> "involutio" red.
