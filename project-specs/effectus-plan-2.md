# effectus plan 2 — what a variable HOLDS (effectus slice 2)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a reading or a measurement; a result outside
> the named branches is shown to Fran before work continues. Steps use
> checkbox syntax. Written 2026-10-05 from `effectus-spec-2.md`
> (interview Q1–Q8 in `effectus-2-interview.md`; A1–A5 decided = every
> recommendation). On approval: one opus per task in a park;
> `silva.commissio(opus=ID)` closes each.
> Park …VQ7D (region 'flow analysis'); opera T1 …FXZMX, T2 …XB8C,
> T3 …VTZ, T4 …E705, T5 …1639, T6 …PSK, T7 …8C3.

**Goal:** the unresolved share of the house's effect sites (44%, 3,304
of 7,584) falls because the evaluator can say what KIND of value a
variable holds - an exact set, a glob pattern, a prefix, a fresh temp
dir - instead of one string or failure; every site still unresolved
says WHY (`causa`); the key never shrinks except where a read provably
never touched the tree (`temporaria`).

**Architecture:** inside `crusta_effectus.c` only. A new internal
`Valor` replaces the `character area[VIA_MAXIMA]` + `longitudo` pair
that `_verbum_aestimare` / `_partes_aestimare` /
`_variabilem_aestimare` / `_substitutionem_aestimare` pass today;
`_definitiones_colligere` records arrays and loop lists instead of
`NIHIL`; `_viam_classificare` turns a `Valor` into one or more sites.
The dialect gains one `classis` value and one attribute; the key and
the oracle change in the named places only (spec §VII, §IX). The public
header (`crusta_effectus.h`) does not change.

**Tech stack:** C89 in Latin (`latina.h`), crusta (registry loci:
`CRUSTA_ASSIGNATIO_*`, `CRUSTA_TABULATUM_LIBERI`, `CRUSTA_ITERATIO_VERBA`,
`CRUSTA_EXPANSIO_*`), `stml.h`, `canon.h`, `xar.h`, `piscina.h`;
credo gates in crusta's runner.

**Spec:** `project-specs/effectus-spec-2.md` (§0 measurements, §I
principles, §II value domain, §III sources, §IV operators, §V sites,
§VI dialect + census, §VII key, §VIII lint, §IX oracle, §X measuring,
§XII review focus, §XIII done, §XIV decided).

## Global constraints

- Everything in `effectus-plan.md`'s Global constraints holds (C89 under
  `tools/vexilla.sh`; Latin identifiers; latina macros forbidden as
  identifiers; ≤ 72 columns + `./silva/formator.sh -vitia`; no `<word`
  in C comments; `vocabula.sh -nova` NOVA 0; commit only through
  `silva.commissio` with explicit paths; never Fran's files; freeze the
  tree while gates run; every gate born red; every fix proven by a
  compiling plant).
- **Soundness rule (spec §I.1):** no value form removes an input from
  the key. The single permitted shrink is `ignotum` -> `temporaria`
  (spec §VII); any other key line that disappears in a task is a
  defect until explained in that task's commit.
- **Direct binary calls** take an ABSOLUTE `-radix` (`effectus -radix
  .` answers "scriptum absens" - interview §0).
- **Identity proofs** compare summaries of all tracked `.sh` (outside
  `oracula/`) from two binaries: the previous one is copied to the
  scratchpad BEFORE the task's first edit.

## Review Focus (spec §XII -> where it is tested)

1. **Quoted globs** (`"$X"`, `X="lib/*.c"`) are literal files -> T5
   Step 2 CONTRARY case.
2. **Duplicate findings** from N sites at one sedes -> T5 Step 4 (one
   irresolutum per word; build-sine-domino per path).
3. **Cross products** hit the cap, not the buffer -> T5 Step 3
   (`$a/$b` with XVI x XVI members).
4. **Temp paths that leave the process** stay unresolved, never
   `temporaria` -> T3 Step 2 (`T=/tmp/x`, exported `T` in a child).
5. **`+=` from a sourced file** appends everywhere; an option pair in
   it still pairs -> T4 Step 3.
6. **Array elements that are paths** become reads -> T4 Step 2.
7. **Judge time** (toml RECENS 2.2–3.0 s today) must not get worse ->
   measured in T3, T6, T7.

## File structure

| file | responsibility |
|---|---|
| `crusta/fontes/crusta_effectus.c` | T1 causa; T2 `Valor`; T3 temporaria + oracle; T4 arrays; T5 loops, patterns, operators, `_tegit`; T6 `${X:-d}` |
| `crusta/instrumenta/effectus.c` | T1 census text + causa column; T3/T6 key lines (`-clavis`) |
| `effectus.canon` | T1 causa nota; T3 `classis` `temporaria` + attribute `temporaria` |
| `crusta/probationes/probatio_crusta_effectus.c` | T1–T6 one section per task |
| `crusta/probationes/probatio_crusta_effectus_lintrum.c` | T5 containment fixture |
| `crusta/probationes/fixa/effectus/oraculum{,_aura}/` | T3 temp fixture + golden |
| `tools/iudicium_fumus.sh` | T6 plants for `directorium` and `ambitus` lines |
| `crusta/fontes/crusta_effectus.worklog.md`, spec §XII, MEMORY, ledger | per task; T7 closes |

---

### Task T1: name the cause, write the text (baseline)

**Opens with a reading:** `_variabilem_aestimare`,
`_substitutionem_aestimare`, `_partes_aestimare`, `_viam_classificare`
and `_assignata_colligere` whole; list in the worklog every `redde
FALSUM` in the evaluator with the causa it will carry.

**Files:** `crusta_effectus.c`, `crusta/instrumenta/effectus.c`,
`effectus.canon`, `probatio_crusta_effectus.c`.

- [x] **Step 1: The failing section.** `XIII. causae`: one fixture
  script with one unresolved site per cause - `"$1/x"` (argumentum),
  `while read f; do cat "$f"; done` (ansa_read), `cat "$(git x)"`
  (substitutio), `cat "$NESCIO/y"` (ambitus), `cat "${X/a/b}"`
  (operator), `X=a; X=b; cat "$X"` (discordia), `A=(x y); cat "$A"`
  (tabulatum) - each asserted by `causa`. Red (all "valor ignotus").
- [x] **Step 2: Carry the cause.** A `causa` field in `Derivatio` set
  at the failing return (innermost wins), cleared before each word;
  `_viam_classificare` copies it instead of "valor ignotus". `read` /
  `mapfile` names (in `a->assignata`, no definition) -> `ansa_read`;
  `for` definitions (`verbum NIHIL`) -> `ansa_read`; digit / `@` / `*`
  titles -> `argumentum`. NOTE: `tabulatum` is a name beyond spec
  §VI's list (arrays are a cause until T4 resolves them) - shown to
  Fran with the T1 result.
- [x] **Step 3: Canon + census.** `effectus.canon` `causa` nota lists the
  vocabulary. Census rows carry `via` = source text for unresolved
  sites and a new last column `causa`; `-census` prints per-causa
  counts.
- [x] **Step 4: Identity of resolution.** Census `plena` / `partialis` /
  `nulla` counts identical to before (only text and causa added);
  summaries differ only in `causa` values (diff filtered by attribute).
- [x] **Step 5: Measurement.** Census by causa recorded in spec §XII
  (dated) next to the §0 heuristic. Branch: classes A+B+C within ~10
  points of the heuristic -> proceed; otherwise show Fran the table
  before T2 (the order of T3–T5 may change).
- [x] **Step 6: Plant** (compiles, red predicted): digit titles fall
  through to `ambitus` -> `argumentum` assertion red. Restore.
- [x] **Step 7: Commit** (gate `crusta`; owed gates by commissio).

### Task T2: the `Valor` refactor (same answers)

**Opens with:** copy `crusta/build/effectus` to the scratchpad
(`effectus.vetus`); summarize every tracked `.sh` with it into
`scratchpad/summ_vetus/`.

**Interfaces (internal, `crusta_effectus.c`):**

```c
nomen enumeratio {
    VALOR_CERTUS = 0, VALOR_EXEMPLAR, VALOR_PRAEFIXUM,
    VALOR_TEMPORARIA, VALOR_IGNOTUS
} ValorForma;

nomen structura {
              ValorForma  forma;
                    Xar*  membra;      /* character*: certus (1..XVI) */
              character*  textus;      /* exemplar, praefixum, suffixum temporariae */
     constans character*  temporaria;  /* sedes mktemp */
     constans character*  causa;       /* ignotus */
} Valor;
```

- [x] **Step 1:** `_partes_aestimare` / `_verbum_aestimare` /
  `_variabilem_aestimare` / `_substitutionem_aestimare` take and fill a
  `Valor*` (piscina strings, no `VIA_MAXIMA` buffers); the join
  (spec §II) is ONE function `_valores_iungere`, the concatenation ONE
  function `_valorem_appendere`. Only `CERTUS` (one member),
  `PRAEFIXUM` and `IGNOTUS` are produced in this task.
- [x] **Step 2:** `_viam_classificare` consumes a `Valor`; `_cd_aestimare`
  and the guarded-builder path likewise.
- [x] **Step 3: Identity.** New binary over the same scripts:
  `cmp` every summary against `summ_vetus/` -> 0 differences; census
  byte-identical; `probatio_crusta_effectus*` green unchanged.
- [x] **Step 4: Plant:** `_valores_iungere` keeps the FIRST definition
  instead of failing on disagreement -> identity check names the
  differing summaries (red). Restore.
- [x] **Step 5: Commit** (gate `crusta`).

### Task T3: `temporaria`

**Opens with a measurement:** under the slice-1 oracle
(`crusta/effectus_oraculum.sh -domus`), run
`toml/compile_probationes.sh registrum` and list observed paths under
temp roots (spec §IX) - today dropped at `crusta_effectus.c` (`rel ==
NIHIL`); record the count in the worklog.

**Files:** `crusta_effectus.c`, `crusta/instrumenta/effectus.c`
(`-clavis`), `effectus.canon`, `probatio_crusta_effectus.c`, oracle
fixtures + goldens.

- [x] **Step 1: Failing section** `XIV. temporaria`: `T="$(mktemp -d)"`
  then `echo x > "$T/a"; cat "$T/a"` -> lectio `classis="temporaria"`,
  `via="a"`, `temporaria=<sedes of mktemp>`, `scripta_in_ambitu
  ="verum"`; `R="$T/r"; cat "$R"` names the same dir (A1); `mktemp`
  without `-d`, with a template, inside a function.
- [x] **Step 2: CONTRARIES:** `T=/tmp/x` (fixed name) -> not
  temporaria; `T="$(mktemp -d)"` exported and read in a child script
  (`ambitus`) -> unresolved in the child; `T="$(other)"` -> substitutio.
- [x] **Step 3: Implement** (substitution of `mktemp`; concatenation
  keeps the creation sedes; site emission; canon `classis` value +
  attribute `temporaria` with nota).
- [x] **Step 4: Key.** `-clavis` emits no line for `temporaria`;
  probatio_fabrica / iudicium-fumus green; toml chain key diffed
  against the T2 key - every removed line is an `ignotum` that became
  `temporaria` (listed in the commit), no other line moves.
- [x] **Step 5: Oracle.** Observed paths under temp roots kept and
  tagged; `crusta_effectus_non_tecta` covers them by `temporaria`
  suffix; new fixture `temporaria.sh` + golden; PLANT: a fixture whose
  `$T` comes from a non-mktemp `$(...)` -> reported uncovered (red as
  predicted). Toml chain live: non tecta 0.
- [x] **Step 6: Census delta** by causa (worklog); judge time measured.
- [x] **Step 7: Commit** (gates `crusta`, `iudicium-fumus`, owed).

### Task T4: arrays

**Opens with a reading:** every `declare -a` / `X=(` / `X+=(` in
`tools/vexilla.sh` and the three biggest runners
(`compile_tests.sh`, `crusta/compile_probationes.sh`,
`tools/cursor_communis.sh`); list the expansion forms used
(`"${X[@]}"`, `${X[*]}`, `${X[n]}`, `${#X[@]}`).

- [x] **Step 1: Failing section** `XV. tabulata`: `declare -a
  F=("-std=c89" "-I$R/include"); clang "${F[@]}" -c a.c -o b.o` ->
  exactly one lectio (`a.c`) and one scriptura (`b.o`), no site for
  the flags; the same with `F` defined in a SOURCED file; `F+=(-O2)`.
- [x] **Step 2: CONTRARY (RF 6):** `S=(lib/a.c lib/b.c); clang
  "${S[@]}" -o x` -> two lectiones.
- [x] **Step 3: RF 5:** `F+=(-o "$OUT")` in one place, `clang "${F[@]}"
  a.c` elsewhere -> `$OUT` is a scriptura, never a lectio.
- [x] **Step 4: Implement:** `TABULATUM` definitions (element words),
  `+=` as order-free append, argument lists expanded BEFORE
  `_tabulam_applicare` (spec §V.2), `for x in "${A[@]}"`, `${A[n]}`.
- [x] **Step 5: Plant:** expansion skipped in the argument list ->
  flags become lectiones again (red). Restore.
- [x] **Step 6: Census delta; toml key diff** (new lines = newly
  resolved reads, explained). **Commit** (gates `crusta`, owed).

### Task T5: loops, patterns, operators, containment

- [x] **Step 1: Failing section** `XVI. ansae et exemplaria`: `for f in
  toml css; do cat "$f/x"; done` -> two lectiones, same sedes; `for f
  in lib/*.c; do cat "$f"; done` -> one lectio `forma="globus"`
  `via="lib/*.c"`; `obj="$B/$(basename "$f" .c).o"` -> `build/x/*.o`;
  `${f%.c}`, `${f##*/}`.
- [x] **Step 2: CONTRARY (RF 1):** `X="lib/*.c"; cat "$X"` -> `certus`
  `lib/*.c` (`forma="via"`); `cat $X` (unquoted) -> globus.
- [x] **Step 3: Cap (RF 3, A2):** a XVII-word list -> `praefixum` of the
  common prefix; `$a/$b` with XVI x XVI -> cap, no overflow; causa
  `discordia` when no prefix.
- [x] **Step 4: Lint (RF 2, Q8):** a two-member read of an unowned
  build path -> two build-sine-domino findings (per path) but an
  irresolutum word -> one finding; `_tegit` glob-in-glob containment:
  writer `build/x/*.o` covers reader `build/x/a*.o`; reader `build/*`
  vs writer `build/x/*` -> still monitum (lintrum gate).
- [x] **Step 5: Implement** (`for` definitions = join of the list's
  values; `exemplar` only from bash expansion; one site per member;
  operators of spec §IV; `_tegit` containment).
- [x] **Step 6: Plants:** (a) quoted `"$X"` treated as a pattern ->
  Step 2 red; (b) containment reversed -> Step 4 red. Restore.
- [x] **Step 7: Census delta; toml key diff; Commit** (gates `crusta`,
  owed).

### Task T6: `${X:-d}` and prefixes in the key (A3, A4)

- [x] **Step 1: Failing section** `XVII. praedefinita`: `cat
  "${FS:-build/fs}/x"` with no `FS=` in scope -> lectio
  `build/fs/x` + `ambitus_lectio FS`; with `FS=a` in scope -> two
  lectiones (join).
- [x] **Step 2: Key (`-clavis`):** in-tree `partialis` read with ≥ 1
  segment, not build/ -> `directorium <prefix>`; root-level prefix ->
  `ignotum` (unchanged).
- [x] **Step 3: iudicium-fumus plants:** P14 a file under a
  `directorium` prefix edited -> not reused; P15 the env default's
  variable set by the caller -> key changes (IGNOTUM, not reused).
- [x] **Step 4: Implement; census delta; toml key diff** (new
  `directorium` / `ambitus` lines explained); judge time measured.
- [x] **Step 5: Commit** (gates `crusta`, `iudicium-fumus`, owed).

### Task T7: close

- [x] **Step 1:** Census by causa, before (T1 baseline) and after, in
  spec §XII As built; unresolved share; classes A/B/C vs spec §XIII.
- [x] **Step 2:** Oracle on toml's chain: non tecta 0, temp paths
  observed and covered; toml chain 0 errata; judge time vs 2.2–3.0 s.
- [x] **Step 3:** Docs: crusta_effectus worklog, crusta/CLAUDE.md
  (value forms, causa vocabulary), MEMORY.
- [x] **Step 4:** Ledger: park closed; next slice (ordering / flow +
  function arguments, D 5% + whatever T1–T6 left) filed as a
  desideratum under 'flow analysis', seeded by the T7 census by causa.
- [x] **Step 5: Commit.**

## Not in this plan (stated)

- Ordering / flow; function-argument binding per call site (Q7).
- Arithmetic (`$((...))`), `${X/a/b}`, indirect `${!X}` - named
  (`operator`), not resolved.
- Temp paths across processes (export, files) - unresolved by design
  (RF 4).
- A C emitter (silva).
