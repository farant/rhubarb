# crusta_effectus worklog

Effect summary of scripts (project-specs/effectus-spec.md, plan
effectus-plan.md, park …5YXP). The analyzer itself arrives in T3; this
log starts at T1 because the dialect is its contract.

## 2026-10-05 - T1: dialect `effectus` + `effectus.canon`

- Contract before emitter: three hand summaries in
  `crusta/probationes/fixa/effectus/` (catena = the smallest real one,
  cursor = toml runner fragment with two processus, omnia = every
  element and every electio value), judged by `effectus.canon` (root,
  registry line `<effectus>`). Gate
  `probatio_crusta_effectus_dialectus` (48 assertions).
- Positions are materia's view, measured with `./crusta/arbor.sh x.sh
  -sedes`: `sedes="L:C-L:C"` 1-based with exclusive end column,
  `octeti="B-B"` 0-based half-open. A site's position = the node that
  NAMES the path (argument verbum, or the whole redirectio), so an
  excuse on the enclosing invocation covers it.
- The coverage guard (section III: every site element and every
  electio value occurs in omnia.stml) caught a real gap on its first
  run: omnia lacked `classis="instrumentum_domus"`. A fixture that does
  not cover the vocabulary does not test the contract.
- Contraries are permanent assertions, not just plants: a foreign
  electio value must give exactly one VALOR_MALUS, a site without
  `sedes` exactly one ATTRIBUTUM_DEEST.
- Plants: (a) `classis="aliena"` in omnia → II and III red, named;
  (b) `necessarium` dropped from probatio's `sedes` → IV red;
  (c) registry line absent → V red (the first run, before the line).
  Plant (a)'s first attempt used BSD sed `0,/re/` - unsupported, edited
  nothing, ran green: a no-op plant. Plants are applied with python and
  their effect counted (`grep -c`) before the run.
- Found while reading the house canons: `aedificatio.canon` drifted in
  fabrica slice 3 - `toml/aedificatio.stml` has 6 canon vitia (genus
  iudicium, ingressus fontationes / instrumentum_domus / identitas_clang,
  provenientia verdictum) and no gate judges declarations against the
  canon. Park …9XNXY. T7/T8's canon steps depend on it.
