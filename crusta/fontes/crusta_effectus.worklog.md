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

## 2026-10-05 - T2: command census + table

- `crusta/instrumenta/mandata_census.c` (+ `crusta/mandata_census.sh`):
  two passes over every tracked `.sh` - house function names first
  (146: `deficere`, `sera_capere`, `credo`... are NOT commands), then
  every imperium title classified functio / aedificium / via /
  dynamicum / externum; wrappers `exec command env nohup` also count
  the wrapped word. 290 files, 334 titles, 51 externals with >= 5
  sites - every one has a row in `crusta/effectus_mandata.stml`.
- Man pages, not memory, decided the options that matter: BSD `sed -i`
  and `-I` CONSUME the next word as the backup extension (`sed -i ''
  ...`); macOS `date -r` takes seconds; `stat -f` takes a format; `jq
  --arg` eats two words (new attribute `optiones_binis_valoribus`).
- A canon has ONE root: adding `<elementum nomen="mandata"
  radix="verum">` to effectus.canon made `omnia.stml` fail ("elementum
  radicis aliud quam canon poscit: mandata"). The table got its own
  `crusta/effectus_mandata.canon` (spec §XIII.4).
- Gate section VI: canon via registry `<mandata>`, 51 rows clean, and
  what canon cannot say (title unique; each row purum, ignotum with
  causa, or carrying a role; purum never with roles). Plants: foreign
  role value, duplicate `wc`, registry line removed - each red at its
  assertion. Plants applied by a python helper asserting exactly one
  match, so a no-op plant cannot happen silently (T1 lesson).
- Known limits written into the rows: clang's `-I` header reads are not
  in its arguments (covered by bin/compilator + identitas_clang in
  gates); `perl -pi` matched as whole words only; `uniq in out` (second
  file is output) unused in the house.
- `crusta/instrumenta/mandata_census.c` has NO gate (INTECTA): it is a
  seed instrument; T6's `bin/effectus -census` supersedes its counts.
