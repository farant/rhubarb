# effectus slice 2 — spec (what a variable HOLDS)

*Born 2026-10-05 from the slice-2 interview
(`project-specs/effectus-2-interview.md`, Q1–Q8) and its measurement
(§0). Predecessor: `effectus-spec.md` (slice 1, closed 13064eed;
§IV.3 values, §VII the key, §XII as built). Ledger: desideratum …SZVT
(region 'flow analysis'), built on decision …DESGA. Question that
prompted it: 44% of the house's effect sites are unresolved, and the
plan was flow analysis - the data says the unresolved mass is a VALUE
problem, so this slice widens what the evaluator can say about a
variable and leaves ordering for the next one.*

*Status: APPROVED (2026-10-05) - §XIV A1–A5 decided by Fran (all recommendations: "those recommendations all make sense to me"); plan `effectus-plan-2.md`.*

## 0. What the measurements say (dated 2026-10-05)

Census (`./crusta/effectus.sh -census`): 7,584 sites, 3,304
`resolutio="nulla"`. Breakdown (interview §0; heuristic join of the
census with context-free per-file summaries, regex classification by
the first variable's definition - planning numbers, not a gate):

| class | sites | share |
|---|---|---|
| A temporary dir: `T="$(mktemp -d)"`, then `"$T/r"` | 1,119 | 34% |
| B flag arrays as operands: `"${GCC_FLAGS[@]}"` | 558 | 17% |
| C1 loop / read variable itself | 400 | 12% |
| C2 derived from a loop variable: `obj="$BUILD_DIR/$f.o"` | 668 | 20% |
| H assignment chains | 200 | 6% |
| D function arguments | 152 | 5% |
| other (`$(...)` values, env-only, literal) | 207 | 6% |

Why each fails today, in `crusta/fontes/crusta_effectus.c`:

- The evaluator's domain is ONE string or failure: `_partes_aestimare`
  (:782) appends each part into a `character area[VIA_MAXIMA]`;
  `_variabilem_aestimare` (:552) requires every definition of `$X` to
  evaluate to the SAME string (:625-629) and fails on none (:635).
- `$(...)` evaluates only `cd W && pwd`, `dirname`, `basename` (one
  argument) and `readlink -f` (`_substitutionem_aestimare`, :646);
  `mktemp` fails, so A fails.
- An assignment whose value is not a word - an array literal
  `TABULATUM`, a subscript, or `+=` - is recorded with `verbum = NIHIL`
  (`_definitiones_colligere`, :1234-1242): B fails.
- A `for` variable is recorded with `verbum = NIHIL` (:1255): C fails.
- `${X:-d}`, `${X%s}`, `${X##*/}` fail outright (:903).
- Options are recognized only in LITERAL words (`_tabulam_applicare`,
  :2527 `_titulus_staticus`), so `"${GCC_FLAGS[@]}"` lands as a
  positional operand and the table makes it a `lectio`.
- The oracle drops every observed path outside the tree
  (`crusta_effectus_observata`, :3992 `rel == NIHIL` -> `perge`), so
  temp reads are invisible to it today.
- `causa` is "valor ignotus" for every unresolved site (:1915) and the
  census writes `-` for their text: the tool cannot say WHY.

## I. Principles

1. **The soundness rule stands** (…DESGA): a fact derived without flow
   never removes an input from the key. Every value form below is an
   OVER-approximation of what the variable can hold.
2. **Values, not order.** Nothing here asks which definition reaches a
   use; "all definitions agree" (:625) generalizes to "the union of all
   definitions". Ordering is the next slice.
3. **Name before resolve** (Q5). The first task makes the analyzer name
   the cause of each unresolved site; every later task is measured
   against that baseline, by class.
4. **A refactor is proven by identity.** Changing the evaluator's domain
   (§II) without changing its answers is checked by byte-identical
   summaries of all 306 house scripts, old binary vs new (the slice-1
   T4 move).

## II. The value domain

A value (`Valor`, internal to `crusta_effectus.c`; no header change) is
one of:

| form | holds | example |
|---|---|---|
| `certus` | a set of 1..N exact strings | `x`, `{toml, css}` |
| `exemplar` | a bash glob pattern (`*` `?` `[..]`, never crossing `/`) | `lib/*.c` |
| `praefixum` | a known prefix + an unknown rest | `build/x/` + ? |
| `temporaria` | a path under a fresh temp dir: creation site + suffix | `mktemp@12:5` + `/r` |
| `ignotus` | nothing known, with a `causa` (§VI) | `$1` |

- **Join** (several definitions, §I.2): sets unite up to the cap (A2);
  over the cap, or `certus` with `exemplar`, the result is the longest
  common literal prefix as `praefixum`; anything with `ignotus` is
  `ignotus` (its causa: the first definition's).
- **Concatenation** (the parts of one word): `certus`·`certus` = the
  cross product (capped); a pattern or prefix absorbs what follows it
  (`exemplar` keeps literal suffixes: `lib/*` · `.c` = `lib/*.c`;
  `praefixum` drops them); `temporaria` keeps its creation site and
  grows its suffix; an `ignotus` part ends the word as `praefixum` if a
  literal prefix with `/` came before it (today's partial rule, :1873),
  else `ignotus`.
- **Quoting matters for patterns.** A glob becomes `exemplar` only where
  bash expands it: an unquoted literal (today's `_globus_habet`, :1527),
  or a `for` list word (bash expands it at loop time, so the loop
  variable holds one MATCH per turn: `exemplar`). `X="lib/*.c"; cat
  "$X"` reads the literal file `lib/*.c` - `certus`, not `exemplar`.

## III. Where values come from

| source | value |
|---|---|
| `X=word` | the word's value (as today) |
| `X=(w1 w2 ...)`, `declare -a X=(...)`, `local -a` | an ARRAY: the list of element words, each its own value (Q4) |
| `X+=(w ...)` | appended to every definition's array (order-free union) |
| `X+=word` | concatenation onto each definition (string append) |
| `for X in w1 w2 ...` | join of the words' values; `"${A[@]}"` in the list = the array's elements (Q3) |
| `for X in glob` | `exemplar` |
| `while read X`, `read X`, `mapfile` | `ignotus`, causa `ansa_read` |
| `X=$(mktemp ...)` (any flags, with or without template) | `temporaria` at the substitution's sedes (Q2) |
| `X=$(...)` other | today's idioms; else `ignotus`, causa `substitutio` |
| `$1`.. `$@`, `x="$1"` | `ignotus`, causa `argumentum` (Q7: out of scope) |
| no definition in scope | `ignotus`, causa `ambitus` - except `${X:-d}` (A4) |

Arrays defined in a SOURCED file count (`tools/vexilla.sh`'s
`VEXILLA_C89`): `_definitiones_colligere` already walks every plagula
of the process; it now records `TABULATUM` values instead of `NIHIL`.

## IV. Operators on values

`"${A[@]}"` / `${A[*]}` expand an array into its element words - this
happens BEFORE the command table sees the argument list (§V.2).
`${A[n]}` with literal n = that element; other subscripts `ignotus`.

String operators, applied to each member of a `certus` set and to the
literal tail of an `exemplar`; a `praefixum` or `temporaria` keeps only
what the operator provably preserves:

| operator | `certus` | `exemplar` |
|---|---|---|
| `${X%suf}` / `${X%%suf}` (literal suf) | strip | strip from the literal tail (`lib/*.c` -> `lib/*`) |
| `${X#pre}` / `${X##pre}` (literal pre; `*/` = path head) | strip | `##*/` keeps the last segment (`lib/*.c` -> `*.c`) |
| `$(basename W)`, `$(basename W suf)` | as today, + suffix | last segment, suffix stripped |
| `$(dirname W)` | as today | the directory part if it has no glob, else `praefixum` |
| `${X:-d}` / `${X-d}` | A4 | A4 |

Anything else (`${X/a/b}`, `${#X}`, `${!X}`, arithmetic) stays `ignotus`
with causa `operator`.

## V. From values to sites

1. **One site per member.** A `certus` set of N members at one word
   yields N sites with the same sedes and octeti (cap A2). A site is
   what the key and the lint judge; a read of `toml/x` and `css/x` is
   two reads.
2. **Arrays in argument lists.** `_tabulam_applicare` receives the
   argument words with array expansions replaced by their elements, so
   `-std=c89` is an option (by its leading `-`, as for a literal word)
   and `-o "$obj"` still pairs. Element words that are themselves
   unknown stay positional (they may be files).
3. **`exemplar`** -> `forma="globus"`, via = the pattern (the key already
   digests globs: `globus <exemplar>`).
4. **`praefixum`** -> `forma="praefixum"`, `resolutio="partialis"` (as
   today), `textus` = source word.
5. **`temporaria`** -> `resolutio="plena"`, `classis="temporaria"` (new
   value), representation per A1.
6. **`ignotus`** -> `resolutio="nulla"` with the named `causa` (§VI).

## VI. Dialect, canon, census

- `effectus.canon`: `classis` gains `temporaria`; `causa` stays text,
  with its vocabulary documented in the canon nota: `argumentum`,
  `ansa_read`, `substitutio`, `ambitus`, `operator`, `cwd ignotum`,
  `profunditas`, `discordia` (joins that hit the cap with no shared
  prefix), `mandatum ignotum` (unchanged).
- Census rows carry the site text for unresolved sites (column `via`,
  today `-`) and the causa (new column, appended - the census is a
  generated file, not an interface). `-census` prints per-causa counts
  next to the per-element counts it prints today.

## VII. The key (`-clavis`)

- `temporaria` sites contribute NO line. Sound without flow: a fresh
  temp path starts empty, so anything read under it was written in the
  same run. An unexcused unresolved site that becomes `temporaria`
  drops its `ignotum` line - the only way this slice shrinks a key, and
  it is the case §I.1 permits (the read never depended on the tree).
- `exemplar` reads: `globus <pattern>`, as today for literal globs.
- `certus` sets: one line per member (they are separate sites).
- Prefixes inside the tree: A3.

## VIII. The lint

- `effectus-irresolutum` judges `resolutio="nulla"` only; `temporaria`
  is resolved, so it is silent there by construction.
- `effectus-build-sine-domino` matches by CONTAINMENT (Q8): a writer
  covers a reader when every path the reader can name, the writer can
  name. `_tegit` (:2984) today compares two globs by equality (:2998);
  it gains glob-in-glob containment for patterns made of literal
  segments and `*` (segment by segment: a writer `*` covers any reader
  segment, a literal covers only the same literal; `?` and `[..]` fall
  back to equality). Fixture: reader wider than writer stays monitum.

## IX. The oracle

- `crusta_effectus_observata` keeps observed paths under a temp root
  (`$TMPDIR` of the PROCESS, `/var/folders/`, `/tmp/`, `/private/tmp/`,
  `/private/var/folders/`) instead of dropping them (:3992), tagged
  `classis="temporaria"`.
- `crusta_effectus_non_tecta`: an observed temp path is covered by a
  static `temporaria` site whose suffix matches the observed path's
  tail (or by a `praefixum`/`ignotus` of the same command, as
  "explicata" today). Uncovered = analyzer defect, as for tree paths.
- Fixture: a script whose `$T` comes from a `$(...)` that is NOT mktemp
  (plant: the oracle must report it uncovered).

## X. Measuring (Q5)

- Gate: `probatio_crusta_effectus` gains a section per value form with
  POSITIVE and NEGATIVE cases (the quoting rule of §II included), and
  per-causa counts over a fixture script that has one site of each
  class.
- Census before/after by causa recorded in §XII As built - the §0
  heuristic table is replaced by the tool's own.
- Toml's verdict chain: the key may change; every changed line is
  explained in As built (new lines = newly resolved reads; removed
  lines = only `ignotum` -> `temporaria`). iudicium-fumus stays green.
- Oracle on toml's chain: non tecta 0, with temp paths now observed.

## XI. Order (plan sketch; the plan file fixes it)

1. **Causa named + census text** (baseline; no resolution change).
2. **`Valor` refactor**: same answers - 306 summaries byte-identical,
   old binary vs new.
3. **`temporaria`** + the oracle's temp roots (§IX).
4. **Arrays** + argument expansion before the table (§V.2).
5. **Loops and patterns** + string operators (§IV) + `_tegit`
   containment (§VIII).
6. **A3 / A4** (prefix lines, `${X:-d}`), if decided in.
7. **Close**: census by class, toml key review, docs, ledger.

## XII. Review focus (failure modes no step above tests by default)

1. **Quoted globs.** `"$X"` with `X="lib/*.c"` is a literal file; only
   bash expansion makes a pattern (§II). A test that only uses `for f
   in lib/*.c` would never catch the confusion.
2. **Duplicate sites at one sedes** (§V.1): the lint document is per
   plagula; N members must not produce N copies of a finding that
   belongs to the word (irresolutum is per word; build-sine-domino is
   per path).
3. **Cross products** (`$a/$b` with two sets) must hit the cap, not
   allocate N*M strings into a fixed `VIA_MAXIMA` buffer.
4. **Temp paths that leave the process.** `export T`, `T=$T ./child`,
   or a temp path written into a file and read back by another script:
   the child sees `T` as `ambitus`, not `temporaria` - unresolved, not
   wrong. A temp path from a FIXED name (`T=/tmp/x`, no mktemp) is not
   `temporaria` at all.
5. **`+=` on a variable defined in a sourced file** appends to every
   definition (order-free), so `GCC_FLAGS+=(-O2)` in one branch makes
   `-O2` appear in every expansion: an over-approximation of OPTIONS -
   harmless for reads, but an option with a value (`-o x`) in such an
   append must still pair.
6. **Array elements that are paths** (`SRCS=(lib/a.c lib/b.c)`, then
   `clang "${SRCS[@]}"`): elements are real operands and must become
   reads, not vanish with the options.

## XIII. Done means

- Census unresolved share by causa, before/after, in As built; target:
  classes A, B, C (83% of §0) resolved or named, the rest named.
- `temporaria` proven by the oracle on toml's chain (temp paths
  observed and covered) and by a plant (a non-mktemp `$(...)`).
- 306 summaries canon-clean, zero crashes; the refactor step proven by
  identity.
- Toml chain: 0 errata, key changes explained, judge time not worse.

## XIV. AUDIENDA — DECIDED (Fran, 2026-10-05: every recommendation (a))

- **A1. How a `temporaria` site is written.** (a) Recommended:
  `via` = the suffix under the temp dir (`r`, `""` for the dir itself),
  new attribute `temporaria` = sedes of the creating `mktemp` (so two
  variables aliasing one dir - `R="$T/r"` - name the same file, and
  `scripta_in_ambitu` matches reads to writes inside it); (b) `via` =
  source text (`$T/r`), no identity - simpler, but `scripta_in_ambitu`
  cannot pair `"$T/r"` with `"$R"`.
- **A2. Set cap.** (a) Recommended: XVI members per value; over it, the
  common prefix (`praefixum`), causa `discordia` if there is none. (b)
  No cap (literal lists in the house are short) - but cross products
  are not.
- **A3. Prefixes inside the tree in the key.** Today a `partialis` read
  outside build/ is `ignotum` (never reused). (a) Recommended: a
  `directorium <prefix>` line (digest of that subtree) when the prefix
  is at least one segment below the root and not under build/ - sound,
  coarse, strictly better for reuse than `ignotum`; a root-level prefix
  stays `ignotum`. (b) Keep `ignotum` (slice 2 changes values only).
- **A4. `${X:-d}` when nothing in scope assigns X.** (a) Recommended:
  value = d's value, PLUS an `ambitus X` key line (the key already
  digests environment values: if the caller sets X, the key changes) -
  sound, and it resolves the env-default dirs the slice-1 oracle
  explained (`FABRICA_SCRIPTURA`); when X IS assigned in scope, the
  join of X's definitions and d. (b) Keep `ignotus`.
- **A5. Causa vocabulary** (§VI): the names `argumentum`, `ansa_read`,
  `substitutio`, `ambitus`, `operator`, `discordia` - (a) as listed;
  (b) Fran renames.

## XV. As built

**T1 (2026-10-05): causes named, census text.** Every unresolved site
now names its cause; the fallback "valor ignotus" occurs 0 times in the
house. Resolution counts identical to before (3,436 plena, 474
partialis, 3,304 nulla, 370 ignotum elements); summaries of all 298
compared scripts differ only in `causa`. The tool's breakdown
(`./crusta/effectus.sh -census`, column 11):

| causa | nulla | partialis |
|---|---|---|
| substitutio (mktemp among them) | 1,274 | 114 |
| ansa_read | 960 | 182 |
| tabulatum | 578 | - |
| argumentum | 150 | 123 |
| cwd ignotum | 88 | - |
| recursio | 87 | - |
| operator | 65 | 13 |
| ambitus | 51 | 42 |
| discordia | 41 | - |
| absens | 10 | - |

The three target classes (substitutio + ansa_read + tabulatum) are
2,812 of 3,304 (85%) against the §0 heuristic's 83%: within the plan's
10-point branch, T2 proceeds in order.

Two names beyond §VI's list, both forced by the data: `tabulatum`
(array values and subscripts, until T4) and `recursio` - the 87 sites
first measured as `profunditas` were all self-referential accumulations
(`obj_files="$obj_files $obj"` in a loop), which the depth limit only
hid; a stack of titles under evaluation now names the cycle.
`profunditas` occurs 0 times.

The census header named a `via` column the rows never wrote (9 values
under 10 names, since T6); rows now carry `via` (source text where
unresolved, sanitized: tabs and newlines become spaces - table causae
contain newlines) and `causa` as column 11.

**T2 (2026-10-05): the `Valor` refactor.** The evaluator passes a
`Valor` (forma, membra, textus, temporaria) instead of `character
area[VIA_MAXIMA]` + `longitudo`; concatenation is `_valorem_appendere`
(keeps the old 4,096-byte cap, so overflow behaves identically), the
join of definitions is `_valores_iungere` (still "all CERTUS and
equal"), failure is `_valorem_frangere` (collected text -> PRAEFIXUM,
nothing -> IGNOTUS: what the old buffer held after a failure). The
idioms in `$(...)` keep a scratch buffer, filled by
`_argumentum_in_aream`. Identity: 306 of 306 summaries byte-identical
against the pre-T2 binary, census byte-identical; plant (join keeps the
first definition on disagreement) -> 87 summaries differ. Census run
1.8 s both; peak RSS 4.64 GB before, 4.89 GB after (+5%, piscina
strings) - the 4.6 GB itself predates T2 (one piscina for the whole
census; vitium …7RD).
