# fabrica slice 6 - interview: the tool (chassis, steps, volumes)

Vision: `project-specs/fabrica-visio.md` (RESERVATUM, 30dffd7d; ledger
…F9ESW3). Follows slice 5 (`project-specs/fabrica-spec-5.md` §XI) and
the pythonica speed work (S1-S3). This interview turns the vision's
§VII agenda into decisions for a first slice.

## 0. Data (2026-10-07, before any question)

**The chassis seed.** `FabricaGenus` (`include/fabrica.h:415`):
`titulus`, `sigillare`, `enumerare`, `locare`, determinism flag - 12
input kinds registered as static tables in `lib/fabrica.c`. No
conformance fixture is required of a kind; plants exist per feature
(probatio_fabrica, iudicium-fumus), not per kind.

**House tools cross process boundaries.** Judging a verdict spawns
`crusta/effectus.sh -clavis` per effectus input; closures come from
`bin/aedilis` processes (24 per pythonica run before S2); compilation
through `bin/compilator`. All three are C with library cores.

**Pilot candidates** (subsystem runners; effectus key of each):

| runner | lines | tests | unresolved sites | today |
|---|---:|---:|---:|---|
| toml/compile_probationes.sh | 145 | 13 | 0 | verdict, reuse 78% (spec 5 T4) |
| css/compile_probationes.sh | 117 | 10 | 0 | crude gate |
| crusta/compile_probationes.sh | 145 | 20 | 7 | crude gate |
| materia/compile_probationes.sh | 220 | 14 | 5 | crude gate |
| md / html | 255 / 263 | 14 / 14 | 31 / 30 | crude gates |
| silva | 305 | 55 | 46 | crude gate |

toml is the only runner that is already a whole-runner verdict: the
same suite expressed as steps gives a direct A/B of runner-level vs
step-level reuse on identical history (`tools/reusus_retro.sh`).

**filum** has 32 public functions (`include/filum.h`) - the surface a
backend vtable would cover. `volumen` (`include/volumen.h`) has blobs,
an event log, a path projection and folds; no layer API yet.

## Questions

**DECIDED (Fran 2026-10-07): every recommendation (a), Q1-Q15** -
chassis + shape step kinds proven on the toml pilot (old runner as
oracle, then per-step vs runner reuse A/B); registries with mandatory
conformance fixtures and a property census; two-axis inputs with
aliases and `repositorium`; no templating; compilator and aedilis
closure as libraries for the pilot; per-step area + declared env;
volumes the NEXT slice; bootstrap stage rides along, snapshots and
one-graph gates as recorded direction; ordering (`post`) separate from
keying; gates as composites over a shared step pool; per-test facts as
source annotations; closures replace hand lists; `locare` may read
inputs, file sets name their source; one flag source, passes cached,
stable step ids, base env from the tool.

**Q1. What is the first slice?** (a) Recommended: chassis + step
vocabulary, proven on ONE pilot runner (Q5); volumes and the filum
backend are a later slice (Q8). The pilot tells us whether steps pay
before we build the world they run in. (b) Volumes/VFS first (scratch
and hermetic tests), steps later. (c) Both together.

**Q2. Chassis shape.** (a) Recommended: one registry per extension
point - input kinds, step kinds, evidence strategies, queries,
commands - each a C vtable compiled in (no dynamic loading); every
registered entry MUST ship a conformance fixture (its input class
changed -> judgment flips and names the particle; unrelated change ->
RECENS; declared writes honoured), run by one chassis test; a census
command lists entries by properties met (static/dynamic reads,
deterministic/volatile, particles/coarse). (b) Formalize input kinds
only now; the rest as they come. (c) Dynamic plugins (shared objects).

**Q3. Input model.** (a) Recommended: two axes in the declaration -
WHAT (plagula, plagulae, directorium, instrumentum, ambitus,
repositorium) and HOW KEYED (contentum, exstantia, nomina, clausura,
provenientia, identitas); today's kind names stay as aliases mapping to
a pair, so nothing existing breaks; `repositorium` (HEAD, index) is new
and makes "this depends on the live repo" visible in the declaration.
(b) Keep today's kinds; add only `repositorium`.

**Q4. First step kinds (REVISED after the mocks, §1 below).** (a)
Recommended: NO templating - a step inside a set acts on the member by
convention, and each kind owns its input -> output naming (no `{p}`,
`{area}`: that is an expression language by the back door). Recurring
shapes become kinds rather than syntax: `probationes_c` (per member:
link via aedilis closure + compilator store, then run; capabilities
gate the run, the link is still judged), `probationes_py`,
`generare_et_conferre`, `registra_cocta`/`capsulae` (family kinds over
a pattern), `regio_generata` (a generated region inside a hand file,
e.g. latina.h), `differentia` (oracle comparison: two inputs, an
agreement verdict), whole-set checks (`nexus_purus`), `installare`;
`mandatum` stays as the priced escape hatch. (b) The generic kinds of
the first draft (`compilare`, `nectere`, `currere` + `pro_omni` with
templates). (c) Also conditionals.

**Q5. Pilot runner.** (a) Recommended: toml - already a verdict, 0
unresolved sites, 13 tests; express its suite as steps, keep the
runner as oracle until the step version agrees (verdict text and
pass/fail per test), then measure per-step reuse against the runner
verdict over the same 150 commits. (b) css - a crude gate today, so
the win is visible as a new verdict. (c) silva - the big one (55
tests), most to gain, most to learn.

**Q6. House tools as libraries - when?** (a) Recommended: in the pilot
for what the pilot needs: compilator and the aedilis closure as
libraries linked into fabrica (effectus stays a process until a step
needs it - steps declare reads, so effectus matters less). (b) All
three now. (c) Keep processes; libraries later.

**Q7. Places and environment for steps.** (a) Recommended: each step
gets `build/fabrica/area/<step>/` (directory, created and emptied by
the tool; reads inside it are products) and a declared environment
(PATH from a fixed base, nothing inherited unless declared); a step
writing outside its area or reading an undeclared variable is a
refusal. Volumes replace the directory later (Q8). (b) Environment
only now. (c) Defer both.

**Q8. Volumes and the filum backend.** (a) Recommended: the NEXT
slice, in the vision's staged order - filum backend vtable (real FS
default, useful alone), then a volumen-backed backend with one
conformance suite run against both (oracle), then a pilot library's
tests on volume fixtures, then volume scratch areas for steps. (b) Now,
before steps. (c) Park.

**Q9. Snapshots, gates in one graph, bootstrap - when?** (a)
Recommended: direction recorded, not in slice 6: snapshot builds as
default and `PORTAE`/inventory lenses becoming derived views follow
once more runners are steps; the bootstrap stage (judges fresh first,
named refusal) is small and can ride along in slice 6. (b) All in
slice 6. (c) Bootstrap later too.

**Q10. Ordering vs keying.** (a) Recommended: separate - `<post
actio="..."/>` at group level says "produce this first" (known up
front); a dynamic step's KEY comes from its trace (only the test that
reads the corpus is voided by it). Declaring a dynamic read per step
for ordering would void every member. (b) Declare inputs per step for
both.

**Q11. Gates as composites over a shared step pool.** (a) Recommended:
a gate is a `compositum` of step verdicts (RECENS = all parts RECENS;
otherwise the parts are named) - the signal line and its grep retire;
steps are keyed by content, so gates that run the same steps SHARE them
(radix and the aedilis gate both run the same 237 tests today - the
second would get them free). (b) Keep one verdict per gate.

**Q12. Per-test facts as source annotations.** (a) Recommended: the
house already writes build facts in C sources (`<aedilis obiectum=..>`);
add `facultas` (fenestra, rete, repositorium), `instrumentum` (scripts
and binaries a test execs - T5b's ssh stub, bin/generare), `post`
(actions that must exist first - the tabulariumd daemon) and `ambitus`
(extra environment). The runner lists (GUI, RETICULARIS,
REPOSITORIUM_VIVUM) retire; a new test is right on its first commit.
`/* generare: ... */` directives become a step kind, not a prelude.
(b) Keep per-test facts in declarations.

**Q13. Closures replace hand lists.** (a) Recommended: every
hand-maintained object/source list in runners (silva `RADIX_FONTES`,
oratio's, officina's - the list that refused T5a) is replaced by the
aedilis closure of each member. (b) Keep hand lists where they exist.

**Q14. Outputs from input contents; file-set sources.** (a)
Recommended: `locare` may READ declared inputs deterministically (a
capsule's toml names its own outputs), never run tools; a file set
declares its source - `fons="arbor"` (present files) or
`fons="repositorium"` (tracked files, as oratio's corpus wants). (b)
Static outputs only; filesystem sets only.

**Q15. Policies.** (a) Recommended: one flag source (`aedilis.stml`;
`tools/vexilla.sh` derived or retired); passes cached, failures always
re-run with the cause recorded; stable step identities
(`<action>/<member>`), deleted members reported as orphans as fabrica
already does for build/aedilis; base environment provided by the tool
(`RHUBARB_RADIX` = root of the tree being built, live or snapshot) -
not declared per action. (b) Decide case by case.

## §1. The mocks (2026-10-07)

Seven runners mocked in the revised shape (toml, root suite, aedilis
gate, silva, generators, briar, oratio, pythonica). None needed
templating or conditionals once the kinds above existed. Findings:
(1) shared steps across gates; (2) closures replace hand lists; (3) a
source-annotation vocabulary for per-test facts; (4) kinds own naming,
`locare` may read inputs; (5) new kinds: oracle comparison, whole-set
check, generated region; (6) file sets need a source (tree vs index);
(7) capabilities gate the run, not the step. briar's install chain
needed almost nothing new - the model already fits declarative chains.
The root suite after the change:

```xml
<actio titulus="probationes_radicis" genus="iudicium">
  <post actio="tabulariumd"/>
  <post actio="speculum_hospes"/>
  <probationes_c exemplar="probationes/probatio_*.c" praeter="*_benchmark.c"/>
</actio>
```

## AUDIENDA

- Whether compilator and aedilis library cores link into fabrica
  without pulling their CLI layers - not checked.
- Per-step reuse for toml is an estimate until the pilot exists; the
  retro tool can replay it only once step traces are stored.
- The step dialect has no canon yet; Q4's names are proposals.
