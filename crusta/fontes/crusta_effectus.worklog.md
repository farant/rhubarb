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

## 2026-10-05 - T3 opening reading: crusta_fontationes.c (1,910 lines)

MOVE unchanged into crusta_effectus.c (T4 then deletes them from
fontationes, which becomes a projection):
- path helpers `_duplicare _appendere _viam_normare _absolutam_facere
  _directorium_viae _caudam_viae _relativa _sub_build`;
- node helpers `_token _nodus _locus_vacuus _nodi_listae
  _functio_circumdans _textum_colligere _titulus_staticus`;
- the evaluator `_verbum_aestimare _partes_aestimare
  _variabilem_aestimare _substitutionem_aestimare _variabilis_sola`
  (idioms: cd&&pwd, dirname, basename, readlink -f; `$0`,
  `BASH_SOURCE`; `local` scoping; empty definitions dropped only for a
  whole-word `$X`);
- scope machinery `_plagulam_parare _nodos_colligere _locales_colligere
  _definitiones_colligere _ambitum_derivare` (fixpoint: re-collect until
  no plagula joins; only the LAST iteration's sites count);
- `_directorium_loci` (catena cd).

GENERALIZE:
- `_nodos_colligere` also collects redirectio, iudicium-praeposita /
  -binaria, verbum (for globs), parametrum / expansio (env reads),
  functio (scope function names).
- `_locum_tractare` (one site kind with five genera) becomes
  `_viam_classificare` (word + cwd -> via, forma, resolutio, classis)
  shared by every site kind; fontatio/exsecutio keep their extra duty
  (join the plagula to the scope / open a new scope).
- `_imperium_tractare`: static titles that are neither source nor
  bash/sh nor wrappers now go to (a) builtins with effects (`[`/`test`
  -> probatio, eval -> ignotum, cd/read/... nothing), (b) scope
  functions (nothing: their bodies are walked anyway, flow-insensitive),
  (c) the command table, (d) otherwise ignotum "mandatum ignotum".
- `_directorium_loci`: + the last TOP-LEVEL `cd W` before the site in
  source order (spec §IV.3).

NEW: redirections, `[[ ]]` tests, glob enumeratio (one source: the word
scan; table reads only emit the lectio), env reads, the table reader,
`scripta_in_ambitu` (after the fixpoint, per processus: equal path,
glob match, or prefix), positions via `materia_tractus_nodi` (materia's
own computation), STML emission (`processus` per scope, sites in
source order).

## 2026-10-05 - T3: the analyzer (crusta_effectus) + bin/effectus

- `crusta_effectus_derivare` (3,300 lines; ~1,100 moved verbatim from
  fontationes - T4 removes the duplicate) emits dialect `effectus`;
  `./crusta/effectus.sh <script>` prints it. Gate
  `probatio_crusta_effectus` (67): one fake runner, one line per site
  class, each with its contrary; then EVERY tracked .sh (294) is
  summarized - zero NIHIL, zero canon vitia (the drift guard).
- Positions are `materia_tractus_nodi(NIHIL, nodus, ...)` - materia's
  own computation, so `sedes`/`octeti` equal what `arbor.sh -sedes`
  prints (asserted on one site: `3:8-3:21`, `72-85`).
- **A plant stayed green and exposed a missing contrary.** Plant (a)
  made every `>` also a read; the suite only asserted that expected
  sites EXIST, so an extra lectio passed. Added: no lectio of
  build/out.txt, exactly two sites on line 6. Then red. (b) fixpoint cut
  to one pass -> lib.sh's read vanishes (I, V red); (c) `sedes` dropped
  on probatio -> I-III red and 219 house summaries fail the canon.
- Found on the toml runner (Step 5): `done < <(cat ...)` became a lectio
  whose path was literally `<(cat`. A redirection whose target is a
  process substitution reads nothing itself; the inner commands are
  sites already. Case first (line 28), then `_processum_habet`.
- `per` is a latina macro (for) - used as a struct field for the
  attribute it raised 111 examen violations at once. Field renamed
  `medium`; the attribute name stays `"per"` in string literals. The
  plan's global constraints had named exactly this trap.
- Toml runner summary: 9 processus (root + 8 executed scripts), 41
  lectio, 67 scriptura, 34 exsecutio, 29 probatio, 9 fontatio, 8
  enumeratio, 9 ambitus_lectio, 6 ignotum. Against the spike's 62
  observed effects: tools sourced, closure-list `cat`s (glob, written in
  scope), vexilla stamp (`-f` + `cat`, and its write), `<` of a closure
  list (covered by prefix `toml/build/clausurae/`), mandata.tsv and .lst
  writes all present. Expected gaps for T5 to name: sera.sh's paths
  come from `$1` of its functions (`valor ignotus`; interprocedural =
  slice 2 territory, or excuse in T6); `cd` targets are STAT-ed by bash
  but `cd` is not modelled as a probatio (decide in T5); clang's
  `$objs` is unquoted and unknown.
- `crusta/instrumenta/effectus.c` (the CLI) is INTECTA until T7's
  fabrica genus calls it (iudicium-fumus then covers it). diagnostica
  gate run before commit (globs crusta/fontes/*.c): sanum.
