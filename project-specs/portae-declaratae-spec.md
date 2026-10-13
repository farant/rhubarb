# portae declaratae - gate coverage in the repo, computed in C (spec)

Ledger question …GGMGY94 (Fran 2026-10-09). Principles: …RBA4W73 ("what
changes what a build or test does lives in the repo; the ledger may point
at it, never be read to decide an outcome") and …Z2Y555 (Fran 2026-10-10:
"pythonica is a THIN wrapper - logic that runs regularly is C; test: what
would a node/ruby wrapper have to reimplement?"). Decisions (Fran
2026-10-10): judgement in an ANNOTATION in each gate's runner; derivable
coverage COMPUTED, never stored; `PORTAE` stays the hand registry; the
ledger inventory becomes a generated view or nothing.

## 0. Census (2026-10-10; detail in …GGMGY94)

337 inventory cells / 50 rows: 132 derived and equal, 13 differing, 192
without derivation - of which most ARE derivable: `manu tantum` (50) =
"not in PORTAE" in 48/50; `tegit` (23) prose nothing reads; `tegit viae`
205 entries = 115 the runner or paths it names, 52 family globs, 38 real
judgement (runtime inputs); `currit binaria` 83 = 24 named in the runner,
41 launched by tests, 18 n.a.; `cur extra PORTAE` 11 pure judgement.
Drift: stored `tegit fontes` LESS complete than derivation in 7 rows; the
filename heuristic misses 5 rows and invents 1; two manual gates
(rete_fumus, vates_fumus) have no row though their headers say "NON PORTA
... manu curritur"; the derivation's PORTAE regex drops `silva-m3`.

Python today: ~550 lines decide what a commit owes (pythonica/silva.py
~340: inventory, closures, matching, merging, relatio; tools/
inventarium_suitarum.py ~200: derivation by regex + shelling out). By
…Z2Y555 that logic belongs in C; this slice MOVES it and deletes it.

## I. INTERFACE A - the annotation (for Fran's approval)

In the runner's header comments (all 45 PORTAE runners are `.sh`; read
with `materia_annotationes` through the crusta client, like `<tolera>`):

```sh
# <porta titulus="credo" tegit="tools/credo_*.py probationes/fixa/credo_convertere/*"/>
# <porta titulus="examen-vectis"/>
# <porta titulus="examen-corpus" tegit="silva/probationes/fixa/examinis/exclusiones.txt"/>
# <porta manu causa="rete poscit (TLS, badssl, API vera - centesimae)"/>
```

- `titulus` - a PORTAE key this script runs; one element per gate when a
  script runs several (arguments stay in PORTAE). Unknown key = refusal.
- `manu` + `causa` - a gate a HUMAN runs (GUI, network, money, a basis
  built by hand): owed edits name it under MANU, never run it.
- `tegit` - paths/globs (exemplar syntax) covered BEYOND derivation.
- `currit` - programs its tests launch that derivation cannot see.
- `instrumentum` - house binaries this gate OWNS (Fran 2026-10-12, T4):
  other gates that run them inherit nothing (build infrastructure, not
  their subject); an explicit `currit` still counts. Owned twice =
  refusal.
- `titulus` + `manu` - a registered gate a human runs (owed edits name
  it, never run it), e.g. aequivalentia.
An annotated script IS a gate row - no filename heuristic. Prose stays
plain comments.

## II. INTERFACE B - the C core (for Fran's approval)

Library `include/portae.h` + `lib/portae.c` (pure engine, I/O through a
sutura like aedilis), built on existing libraries:

| need | library |
|---|---|
| annotations in shell comments | materia_annotationes (crusta client) |
| what a runner executes/sources/reads | crusta_effectus |
| closures of C sources | aedilis (+ aedilis_silva) |
| what tests launch (system/processus) | silva |
| `tegit` globs | exemplar |

```c
nomen structura {                    /* una porta declarata */
    chorda  via;                     /* cursor (scriptum annotatum) */
    chorda  titulus;                 /* clavis PORTAE; vacua si manu */
       b32  manu;
    chorda  causa;                   /* manu: cur homo currit */
      Xar*  tegit;                   /* chorda: exemplaria */
      Xar*  currit;                  /* chorda */
} PortaDeclarata;

nomen structura {                    /* debitum unum */
    chorda  porta;                   /* titulus, aut via cursoris si manu */
    chorda  causa;                   /* "x.c in clausura y.c" ... */
       b32  manu;
} PortaeDebitum;

/* declarationes omnium cursorum; recusatio (annotatio mala, titulus
 * ignotus, PORTAE clavis sine cursore) nominata in *causa_out */
Xar* portae_declaratas_legere (PortaeSutura* s, Piscina* p,
                               chorda* causa_out);
/* debita viis mutatis: derivatio + declarationes; intecta in *intecta */
Xar* portae_debita_computare (PortaeSutura* s, Xar* declaratae,
                              Xar* viae, Xar** intecta_out, Piscina* p);
```

CLI as a `bin/fabrica` subcommand (fabrica is already the build tool and
its vision is "owed gates from the graph"; no new installed binary):

```
bin/fabrica portae debitae [-machina] -portae <tsv> <viae...>  # quae portae debentur
bin/fabrica portae declaratae [-machina] -portae <tsv>          # tabula portarum (et recusationes)
```

`-portae <tsv>` = the registry (`clavis\tcursor` per PORTAE entry), handed
in by the wrapper: PORTAE stays the hand registry in pythonica/silva.py
(§VI) and C never parses Python. A node/ruby wrapper would pass its own.
Moving PORTAE into repo data is the follow-up that makes the core
standalone.

`-machina` TSV: `DEBITUM\t<porta>\t<0|1 manu>\t<causa>`,
`INTECTUM\t<via>`, `RECUSATIO\t<via:linea>\t<causa>`. The columns are a
DECLARED CONTRACT (documented once in portae.h, tested), not a format
implied by one wrapper - a declared output schema is what keeps every
wrapper thin (cf. lapide-v2 briar-feedback FR-010 "structured commands":
commands with declared input/output schemas, exposable later over MCP -
"what does this change owe?").

## III. The wrapper (what stays in Python)

`silva.portae_debitae(viae)` runs `bin/fabrica portae debitae -machina`
and maps the TSV to the existing `Debitum` tuples; `silva.commissio`
keeps orchestration (add owed gates, print, run). Target: ~40 lines.
DELETED: `inventarium()` use for coverage, `_clausurae*`, the matching
and merging in `portae_debitae`, `portae_sine_ordine` (becomes the C
check + gate), `tools/inventarium_suitarum.py`. Node/ruby test: a new
wrapper would reimplement only "call, parse 4-column TSV, print".

## IV. Migration (shadow first)

1. C core lands; a TEMPORARY comparison script (throwaway by …Z2Y555 -
   deleted at cutover) prints the ledger-based (current Python) and
   repo-based answers side by side over all tracked paths - nothing
   switches;
2. annotations generated ONCE from the ledger's judgement cells, reviewed
   as an ordinary diff, committed;
3. cut over when `comparare` over all tracked paths is empty or every
   difference is explained (typically derivation finding MORE);
4. delete the Python logic (III); the ledger inventory is removed, or
   regenerated read-only if a real query use appears.

## V. Done means

- every gate runner annotated; `portae declaratae` refuses bad
  annotations BY NAME (plants: malformed, unknown title, PORTAE key
  without a runner);
- owed gates computed from the repo only - a fresh clone with no
  `~/.rhubarb` computes them; an old commit can say what it owed;
- C tests next to the C logic (fake suturae, like aedilis/fabrica);
  pythonica tests shrink to the wrapper; gate `portae-declaratae` born
  red by a plant;
- the ~550 Python lines are gone; manual gates (rete_fumus, vates_fumus,
  aequivalentia) named under MANU by edits to their sources.

## VI. Not in this slice

PORTAE derived from annotations; owed gates from fabrica's build graph
(the core is a library fabrica can call later); runtime inputs observed
through fabrica's recorded reads instead of `tegit`; moving
tools/credo_convertere.py to a silva instrument (parked …04K4QDC).
Dogfooding note (…Z2Y555): anything painful in C here (TSV/STML I/O,
process running, globbing) is recorded as a missing house capability.
Checklist: lapide-v2 briar-feedback FR-008 "scriba" - `capere` (run +
capture, like `$(...)`), `per_lineas`/`campi` (lines/fields), pool
formatting with `%C`, sorted glob, atomic write, `recens(out, in...)`
(tools/instrumentum_nectere.sh is today's shell `recens`). If the core
needs `git ls-files` + line splitting, a minimal scriba slice is the
candidate first step, not a workaround.

## VII. As built (2026-10-12, T1-T6: 06a2c6e5 ac01588b 9cc16083 8a603900 def78408)

- **The annotation is as approved, plus two forms decided during the build.**
  - `instrumentum="bin/x"` (Fran's decision (B), T4) means this gate OWNS
    the build tool. Other gates that run it inherit none of its sources;
    an explicit `currit` still counts. Owners: aedilis, fabrica-fumus,
    compilator, mensor-suitae.
  - `titulus` + `manu` means a registered gate run by hand
    (aequivalentia).
  - 52 declarations in 51 runners, generated ONCE from the ledger in T4.
- **C core** (`include/portae.h`, `lib/portae.c`): reader → derivation
  (`PortaeOpertura`: exemplaria, fontes_c, ignota, notae) → `tactus`
  (every reason per path/gate pair) → `debita` → writers. The TSV contract
  and the human form both live in the library, and their exact bytes are
  tested.
- **The sutura has five callbacks:** enumerate, read, effectus, test
  literals, closure. The CLI wires them in tools/fabrica_portae.c, plus
  tools/fabrica_litterae.c, because silva/amalgama/silva.h clashes with
  stml.h in a single translation unit.
- **What the derivation counts as coverage** (each rule was found by the
  shadow comparison):
  - only CONTENT sites (lectio/exsecutio/fontatio); test probes and
    listings only inspect mtime or existence;
  - nothing guarded by `custodia`;
  - house-tool executions and test-launched binaries map to their entry
    source, with a `tools/` preference and ownership applied;
  - any tracked path a test names is a dependency;
  - untracked `.c` files get no closure.
- **Unknowns:**
  - blind (effectus or literals unreadable): owed for every path, but
    covering none;
  - unknown closure: owed and covering (Python parity);
  - ambiguous name: all candidates;
  - binary with no source: NOTA.
- **CLI:** `bin/fabrica portae debitae|tactus|declaratae [-machina]
  -portae TSV`. Measured: 2.4 s warm / 30 s cold after a rebuild of
  bin/fabrica (the closure extraction cache is keyed by the binary's
  digest); 4.6 s for a whole commit's owed gates, against 22-26 s in
  Python.
- **Wrapper:** `silva.portae_debitae` ≈ 60 lines; silva.py is net -265
  lines; tools/inventarium_suitarum.py and the shadow comparison script
  are deleted. The ledger inventory 'suitae probationum' is REMOVED
  (Fran, all 50 rows, cause recorded). The ledger is never read; a fresh
  clone computes owed gates (gate `portae-declaratae` II).
- **Shadow comparison result** (T4): over 9815 paths, 0 pairs were owed
  only by the old answer. Of 11973 pairs owed only by the new answer,
  2021 remain, all real dependencies the ledger never recorded.
- **Gate `portae-declaratae`** (3 s): no refusal over the real registry,
  and owed gates without the annals. It is owed by every `.sh`,
  silva.py, and the C core.
- **Departures from the spec:**
  - `portae_sine_ordine` was not turned into a C check: the gate makes
    it unnecessary, because every key must have an annotated runner.
  - `PortaeDebitum` carries `porta`/`causa`/`manu` as specified;
    `PortaeTactus` is new.
  - `-I` dirs remain a named gap, carried by `tegit`.
- **Open** (ledger parks, natum-de …PXC):
  - plumbing ownership at script level (excubitor via silva/nexus.sh),
    …SRFSD9;
  - hand-kept `-I` lists (compile_tests.sh, silva/haruspex.sh) should
    derive from aedilis.stml, …W5W01;
  - a path-glob home outside iter_directoria (three implementations
    today), …H5021.
