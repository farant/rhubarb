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

## 2026-10-05 - T4: fontationes as a projection

- `crusta_fontationes.c` 1,910 -> 334 lines: derive the summary with an
  EMPTY command table (externals cannot be fontationes; fake test trees
  have no table), keep `fontatio` + syntax-level `exsecutio`
  (`per="aedificium"` - table exsecutio like `python3 x.py` were never
  fontationes), map classis/resolutio to the five genera (header
  comment holds the table), re-add every processus root. Its gate
  UNCHANGED and green (36); plant (PRODUCTUM/INSTRUMENTUM swapped) -> 4
  red at exactly those genera.
- An empty table needs ONE row: `titulus=""` reads back as absent, the
  loader saw zero rows and refused ("tabula mandatorum illegibilis") -
  every house output differed until the placeholder became `titulus="-"`.
- Byte comparison over 286 house scripts: 89 differed, all in the
  first-site column except tools/briar_fumus.sh (a correction - spec
  §XIII.5). Key equality proven through fabrica itself: toml pass
  recorded with the new code (33 s) -> RECENS under the old code too.
  The toml verdict had been IGNOTUM before T4 (stale since earlier
  commits), checked by judging with the old code first.
- `textus` attribute added to the dialect (canon, omnia.stml) for
  partial sites.

## 2026-10-05 - T5: the oracle

- Fence (A2 decided; Fran chose (a) when examen refused): the
  interposer is house C89 in Latin, `crusta/instrumenta/
  interpositio_macos.c` (house `_macos` suffix). examen's POSIX lexicon
  learned execve, access, faccessat, fstatat, openat, AT_FDCWD (own
  commit ef274065, nine gates; auspex certifies AT_FDCWD == -2 against
  the SDK). The `__attribute__((used, section("__DATA,__interpose")))`
  table is excused in place (`tolera EXTENSIO_COMPILATORIS`).
- Interposer log lines: P (pid, progname, PATH), E (event, absolute
  path at call time), D (cwd at exec), A (argv), plus SHEBANG events.
  Two mistakes found on first runs: I wrote it in plain C keywords
  (converted to latina.h), and my sanitizing appender turned the TAB/NL
  separators into spaces - the whole log was one line (`wc -l` 0).
- Library: `crusta_effectus_observata` (log -> same dialect,
  `per="observatum"`; bash pids only; PATH lookups by the process's OWN
  PATH; argv of commands bash ran interpreted by the SAME table;
  read-before-write per pid -> ante_scripta) and
  `crusta_effectus_non_tecta` (coverage by kind + path/glob/prefix;
  process roots cover reads of the scripts themselves).
- **Two categories, on purpose.** On toml's live chain 9 effects had no
  resolved static site, but every one had an UNRESOLVED static site of
  the same kind and command (`${FABRICA_SCRIPTURA:-.}` dirs in
  corpus_indicem.sh / tomllib_aurum.sh; sera.sh's `$1`; cursor_communis
  `clang -o "$2"`). The key is already IGNOTUM there, so these are
  "ignota explicata" (named, counted), not analyzer misses. Exit 1 only
  for "non tecta". Toml chain: non tecta 0, explicata 9.
- Found by the oracle: (1) top-level `cd lib` resolved its own argument
  against itself (`lib/lib`) - a cd applies only if it ENDS before the
  site; case first (lines 29-30), then fixed. (2) the argv interpreter
  ignored the table's `optiones_ignotae` (`perl -e 'print'` became an
  exsecutio of "print ..."); fixture line first, then fixed. (3) `cd W`
  is now a `probatio` (operator `cd`): bash stats W.
- Gate (C14, nothing spawned): probatio_crusta_effectus XII - 8
  fixtures (`crusta/probationes/fixa/effectus/oraculum/`) static vs
  committed goldens (`oraculum_aura/`), non tecta 0 AND explicata 0, plus
  pins (data/p.txt proves the shebang redirect; data/b.txt via `head`
  argv proves the SIP path). Regeneration is deterministic (goldens
  byte-identical after a re-run). Plants: shebang redirect disabled ->
  puer golden 3 sites, pin red; static `<` read never matched (a plant
  that COMPILES - `FALSUM &&` did not) -> XII names data/a.txt and
  data/b.txt.
- Table check against Homebrew GNU coreutils (Review Focus 3): rows
  VERIFIED by the commands' own observed effects: cat, head, tail, wc,
  sort (-o), touch, ls, cp (read side). UNVERIFIED (their effects go
  through calls the interposer does not see: mkdir(), unlinkat,
  renameat, symlinkat, and Darwin symbol variants such as
  open$NOCANCEL used by stdio): cut, paste, comm, cmp, uniq, tee, mv,
  ln, mkdir, rm, readlink, stat, tr. The interposer sees nine calls by
  their plain names - enough for bash itself (the fixtures prove it),
  not a full syscall trace.
- `-tabula` option: fixture trees have no command table; the driver
  passes the repo's.
- Coverage: `crusta/instrumenta/interpositio_macos.c` is INTECTA by
  design (a by-hand oracle; C14: gates never spawn it) - its goldens are
  what the gate checks. `effectus.c` gains coverage in T7.

## 2026-10-05 - T6: lint, chains, guarded builders, excuses, census

- Reading crusta_facies.c: gradus II = projection -> per rule (compose,
  expand, extract, subtract) -> annotations ONCE from the source ->
  excuses for rules that ran. The effectus route reuses every step
  except the projection: `crusta_effectus_diagnostica` builds ONE
  document per file from the summaries' sites with that `plagula`
  (exemplaria key by offset), runs `crusta/lintrum/effectus/*.stml`
  (own directory - in crusta's dir they would run against the crusta
  projection and their excuses would read dead), downgrades erratum to
  monitum outside verdict chains, and filters annotations to
  `codex="lint:effectus-*"` (otherwise crusta's own excuses are judged
  dead or causa-less a second time - the diagnostica gate caught both).
- Chains (A4): roots = `ingressus genus fontationes|effectus` in the
  aedificatio.stml files fabrica.stml lists, read as plain STML by
  `crusta_effectus_catenae` (spec §XIII.7: not `bin/fabrica catenae` -
  the lint would have had to link or spawn fabrica).
- **Guarded builders (Fran, option (a)).** 45 errata on toml's chain,
  ~30 inside `[ -x bin/X ] || tools/X_struere.sh` builders. The
  analyzer marks the exsecutio `custodia="bin/X"` and a post-pass marks
  a processus custodial iff EVERY path from the root crosses a guard
  (vexilla.sh, reached both ways, stays free). Chain membership skips
  custodial processes; the fontationes projection is untouched.
- Found by the guard test: top-level `cd ..` after `cd lib` was
  evaluated against the ROOT (each cd resolved relative to radix, not
  to the cwd before it). `_directorium_loci` now walks top-level cds
  sequentially (unknown until an absolute cd), then catena cds on top.
- Excuses (19, all with reasons): sera.sh x3 functions + `ps`, mensor
  timing (perl, eval, metrics file), cursor_communis link steps x2,
  tomllib_aurum counting its own output x2, corpus_indicem's git (its
  output is a declared action, regenerated at every judgment). Toml
  chain: 0 errata. Plant: one sera excuse removed -> diagnostica exit 1
  with erratum in the chain; the same function copied outside the chain
  -> monitum, exit 0.
- Gate `probatio_crusta_effectus_lintrum` (42): positive + negative per
  rule, gravitas by chain, function-level excuse covers its three
  sites, misplaced excuse -> `materia:excusatio-mortua`, chain roots
  from fake declarations (plant: fasciculus counted as root -> red).
- Census (`./crusta/effectus.sh -census` -> build/effectus/census.tsv):
  306 scripts, 7,477 sites; resolution plena 3,375 / partialis 472 /
  nulla 3,264 (44%: function arguments and loop variables - slice 2);
  findings irresolutum 2,442, build-sine-domino 54, mandatum-ignotum
  242; excused 19.
- Cost: the effectus test went 2.2 s -> 5 s (whole house): the
  sequential top-level cd walk is O(sites x commands) per file
  (compile_tests.sh 42 ms). Toml's derivation is 10 ms - inside budget.
- diagnostica_fumus XIII's fixture changed from `[ $a -nt $b ]` to
  `[ a -nt b ]`: it tests that a crusta excuse leaves NOTHING printed,
  and the effectus pass rightly warns about unresolvable `$a`.

## 2026-10-05 - T7: fabrica genus `effectus`

- Q8 amended (Fran, decretum …39JMC): outside verdict chains
  `effectus-irresolutum` is SILENT (filtered after excusation, so its
  excuses never read dead); the other two rules still warn. House
  monita 2,736 -> 294; the census still records every site.
- `./crusta/effectus.sh -clavis <script>`: one line per key item
  (octeti, provenientia, probatio, nomina, globus, directorium,
  ambitus, dominus, ignotum). "Excused" is computed by the lint itself:
  an unresolved site becomes `ignotum` only if a finding survives
  excusation with in_catena VERUM. Custodial processes contribute their
  guard's provenance only. Toml: 29 lines, zero ignotum.
- The wrapper passes the TOOL tree's table and rules (`-tabula`,
  `CRUSTA_LINTRUM`): judged trees (iudicium-fumus radices) have neither;
  the caller's `-radix` comes last and wins.
- House binaries called by path get their table row too (new row
  `compilator`: only `-o` is written - its reads are in the read
  ledger). Without it fumus's `clang build/x/a.o` link read had no
  writer in scope and the key would say "build/ sine domino".
- iudicium-fumus: P11 `$(cat flag.txt)` edited -> not RECENS DIRECTLY
  (before T7 only the audit caught it); P12 `[ -f optio.txt ]` created;
  P13 read-and-rewritten build state file edited (soundness rule). The
  audit case moved to `caeca.txt`, read through an EXCUSED unresolved
  site - the class the key deliberately ignores and only the audit
  catches. Plant: key skips scripta-in-scope build reads -> P13 red.
- A changed input yields IGNOTUM ("key changed"), not STALUM - the same
  as fontationes' P2. The plan's "STALUM directly" means "not reused".
- Cost: judging toml RECENS 2.2-3.0 s; ~1.0 s is the effectus.sh
  wrapper's build check (cursor_instrumentum_struere), the analyzer is
  20 ms. fontationes' wrapper cost the same; target 2 s missed by the
  wrapper, named (spec §XII).

## 2026-10-05 - T8: fontationes retired, slice closed

- No caller of crusta_fontationes remained after T7 (porta_toml on
  genus effectus), so the whole module went, not just the CLI. Its test
  was the only place the house idioms (`X=1 cmd`, `cd && pwd`, `${X:-y}`,
  `source` vs `bash`, the CURSOR script) were pinned as a list; ported to
  probatio_crusta_effectus_idiomata against summary SITES rather than the
  old CrustaFontatio rows. The old test found specific irresolute rows
  ($NESCIO, $q, P=""); the port keeps those AND adds a total (VI), which
  the old one never pinned.
- The lintrum catenae fixture kept `genus="fontationes"` as a NEGATIVE
  case (expect I root, not II): a retired genus must not silently make a
  chain.
- Cursor bug: `cursor_fontes_compilare` unions `clausurae/*.lst`, and
  nothing removed a deleted test's .lst. Symptom: "clang: no such file
  crusta_fontationes.c / FRACTA". Fix in cursor_clausuras_derivare:
  `rm -f probatio_*.lst` first (instrumenta closures live in their own
  dirs). Frigida (fresh clone) cannot catch this class: it is a stale
  CACHE in a live tree, the mirror of "a test reading a gitignored
  input".
- Done-means check (spec §X): all met except toml judge time (2.2-3.0 s,
  wrapper's build check ~1 s, named in §XIII).

## 2026-10-05 - slice 2 T1: causes named

- One choke point: every unresolved site was born at
  `_viam_classificare`'s "valor ignotus". The evaluator now records WHY
  it failed in `d->causa` through `_deficere` (first failure wins = the
  innermost, so `"$v"` with `v="$1"` says argumentum). Definitions carry
  their own causa for `verbum NIHIL` (for -> ansa_read; arrays,
  subscripts -> tabulatum; `X+=word` -> operator).
- The word's cause must be captured BEFORE `_directorium_loci`: a
  `cd "$NESCIO"` evaluated for the cwd would otherwise set its own
  cause first (section VIII asserts `cwd ignotum` stays).
- `profunditas` was a disguise: all 87 were `X="$X y"` accumulations
  recursing to depth XVI. A title stack (`d->acervus`, wrapper around
  `_variabilem_intus`) names them `recursio`. These are the T5 loop
  class in another costume (an accumulation over a loop = a set).
- Census: the header has said `via` since T6 but rows never wrote it;
  table causae contain NEWLINES (the .git row), which split census
  rows until sanitized. The census also dedupes sites by (element,
  octeti) - T5's one-site-per-member will need (element, octeti, via).
- Formator: the census loop was nested so deep that `-scribere` output
  failed `-vitia` (declarations pushed past col 72). Extracting
  `_censum_lineam_scribere` fixed it; struct members with trailing
  comments that cannot align inside 72 get the comment above instead.
- `effectus -radix .` (relative) says "scriptum absens" for everything
  - use an absolute radix when calling the binary directly.

## 2026-10-05 - slice 2 T2: Valor, same answers

- The old evaluator's contract was implicit: return FALSUM and the area
  holds whatever was appended before the failing part - nested partial
  values included (a single definition that is itself partial keeps its
  prefix: "praefixum definitionis unicae servatur"). The new one makes
  it explicit: every failing return goes through `_valorem_frangere`,
  which turns the collected text into PRAEFIXUM (IGNOTUS if empty). The
  only reader of a failed value inside the evaluator is
  `_variabilem_intus` (per definition), and the two outside entries
  (`_viam_classificare`, `_cd_aestimare`).
- `$(...)` idioms (dirname, cd && pwd, readlink -f) still manipulate a
  char buffer in place; `_argumentum_in_aream` evaluates their argument
  as a Valor and copies the CERTUS text in. Scratch, not a value.
- T1's commit d2e2ba5c carried mis-indented blocks (control lines one
  level deep, a parameter list shifted): my scratchpad edit helper
  prepended the original line's indentation to text that already had
  it, and the formator neither re-indents nor reports a wrong NESTING
  level (R5 checks only spaces vs tabs) - vitium …MCGT. Fixed here;
  edits are exact-string replacements from now on. Crude checker
  (scratchpad indentatio.py) still reports 17 lines here - all inside
  braced `casus` blocks, the same 17 the T3-era file shows.
- A declaration block with `character**` came out with the type column
  LEFT of the block indent after -scribere (CONFORMIS); rewritten with
  single pointers.
- Identity: index-numbered summaries (path flattening collided 8 names
  in T1's check) - 306/306 identical.
