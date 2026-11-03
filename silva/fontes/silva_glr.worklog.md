# silva/fontes/silva_glr.c — worklog

## 2026-11-03 — silva budget step 1a: first GSS predecessor inline

Context: ledger question 01M6FTG2WM (data-oriented layout for silva/materia)
and Fran's go for "steps 1-3" (the silva-internal ones, no API change).
A 15 s `sample` of `percursus` parse-only over the whole repo (2991 files,
78 MB) showed the parser instruction-bound (IPC 3.7, ~5100 instructions per
byte) with generic containers on top: xar_addere 13%, xar_obtinere 11%,
allocare_segmentum 6%, xar_creare 3%, tabula_dispersa ~10% (inclusive).
The GLR node routine (`_nodum_processare`) was 27%.

Cause found: `_nodus_creare` made a fresh `Xar` for `praedecessores` on
EVERY GSS node (one per shift, one per reduce). Almost every node has one
predecessor.

Change: `SilvaGSSNodus` carries `praedecessor_primus` inline;
`praedecessores_ceteri` (Xar) is created only when a second, distinct
predecessor arrives. Two helpers, `_praedecessorum_numerus` and
`_praedecessor(nodus, i)`, give the old indexed view in the same order
(first, then the rest in insertion order), so path enumeration — and so
every tree — is unchanged. `_praedecessorem_addere` now takes `glr` (it
needs the piscina for the lazy Xar).

Bench (scratchpad `silva_mensura.sh`, same machine, load 2.6 -> 4.1; the
instruction counts don't depend on load):

| | before (d064d35f) | after |
|---|---|---|
| percursus parse-only: instructions | 400.9 G | 364.0 G (-9.2%) |
| cycles | 108.7 G | 88.3 G (-18.8%) |
| wall | 33.8 s | 27.4 s |
| max RSS | 1128 MB | 714 MB (-37%) |
| percursus -semantica: instructions | 823.3 G | 778.4 G (-5.5%) |
| cycles | 240.2 G | 210.9 G (-12.2%) |
| max RSS | 2762 MB | 2363 MB |
| computus lib/stml.c (min of 7): parse | 28.67 ms | 23.25 ms (-19%) |
| glr phase | 16.10 ms | 12.00 ms (-25%) |
| allocations | 1.04 M | 0.58 M (-45%) |

Cycles fall twice as much as instructions: fewer allocations means less
memory touched (DOD point — allocation is the cost, not the arithmetic).

Equivalence: `silva/aequivalentia.sh -conferre` over lib, silva/fontes,
oratio/fontes, crusta/fontes, materia/fontes (483 files): 482/483
byte-identical, the one being silva_glr.c itself (allowed). percursus
fideles/errores unchanged (2990/2991, 60).

TEST GAP FOUND: no suite covered multi-predecessor nodes. Planted faults
(A: forget `ceteri`; B: drop the dedup on `primus`) were green on glr,
ambiguitas and parsare — those use toy grammars where branches reconverge
through the reduce index, never through a shift-frontier merge. Yet the case
is common: a temporary counter saw ~1.01 M second-predecessor events in one
corpus pass, and fault A changed 406 of the 483 equivalence documents.
Minimal input: `void f(T bytes[16]);` with T unknown — readings (a) type T,
declarator `bytes[16]`, (b) types `T bytes`, abstract declarator `[16]`;
both stacks shift `[` into the same state. New case "FRONS FUSA" in
probatio_silva_c89.c asserts one AMBIGUUS with two PARAMETRUM readings in
order (1 then 2 specifiers). Plants: A red (the suite crashes — the broken
engine, output lost to the crash), C (rest enumerated before first) red on
the order assertion. B is output-neutral — a duplicate predecessor only
yields duplicate paths that the equal-value check discards — so only the
benchmark guards it.

Next (same budget): deterministic LR fast path (step 1b), then the
preprocessor's name-keyed macro table (step 2: intern identifiers, table by
id), then flat arrays / bitsets in the hot loops (step 3).

## 2026-11-03 — silva budget step 1b: deterministic fast path

Profile after 1a (`sample` of percursus parse-only): GLR 28% of the run;
inside it container calls ~35% (xar_obtinere 18%, xar_addere 13%,
xar_numerus 4%), `_vias_recursio` 13%, the tree builder only ~15%.
Main cause: `_index_quaerere` scans every reduce-index entry of the step
with one `xar_obtinere` call per entry, and C expressions run chains of
~17 unit reductions per token (identifier -> primary -> ... ->
assignment) — quadratic in chain length, per token. Plus per reduction:
a `SilvaGSSVia` (32 values) appended to an Xar, recursion, index append,
reducenda append.

Change: `_passum_celerem_currere`. When the frontier is one node, walk
the chain: one action -> SHIFT finishes the step; REDUCE reads its single
path through `praedecessor_primus` straight into a stack array, looks up
goto, checks the (status, basis) key against a small local chain array,
builds, creates the node, continues. It hands off to the general path
BEFORE doing anything for the current action when: actions != 1, a path
node has `praedecessores_ceteri` (several paths) or no predecessor, goto
missing, key repeats, ACCEPT/RECUPERARE, chain full (64). Hand-off writes
the chain into the index exactly as the general path would have (processed
links exhausted, the current node unexhausted and queued in reducenda),
so the general path continues as if it had run all along. Counters
(reductiones_factae, eventa_marginis_novi) are kept on the fast path too.

Kill switch: `SilvaGLR.via_celer` (VERUM by default, set in
`silva_glr_creare`). FALSUM = general path only. It is the oracle for the
differential test and a one-line diagnosis ("is it the fast path?").

Bench (`silva_mensura.sh`, baseline re-taken at e6772b14 same session):

| | before | after |
|---|---|---|
| percursus parse-only: instructions | 364.2 G | 310.9 G (-14.6%) |
| cycles | 89.6 G | 86.0 G (-4%, load 4.2 -> 4.8) |
| percursus -semantica: instructions | 779.6 G | 719.5 G (-7.7%) |
| computus glr phase, lib/stml.c (min of 7) | 12.37 ms | 8.93 ms (-28%) |
| silva_expandere.c | 5.95 ms | 4.47 ms (-25%) |
| lib/chorda.c | 3.12 ms | 2.27 ms (-27%) |
| oratio_resolutio.c | 10.81 ms | 8.58 ms (-21%) |

Cycles fall much less than instructions here (unlike 1a): the removed
work was cheap, cache-hot container calls at high IPC, not memory
traffic. The glr-phase timings (min of 7) are the steadier signal.

Equivalence: aequivalentia -conferre, 483 files: 482 byte-identical (the
changed silva_glr.c itself differs). New check this step: a scratch probe
(arbor.c with the tree output replaced by parsura counters: fusiones,
transmutationes, transmutationes_negatae, eventa_marginis_novi,
frons_maxima, segmenta) — identical on all 482 unchanged files. The corpus
exercises everything: 199,726 fusions, 133,218 transmutations, 311,211
new-edge events.

Test: differential section in probatio_silva_ambiguitas.c — the same
token stream parsed with via_celer on and off; trees structurally equal
(tokens by identity), all counters equal, nodi_creati and
reductiones_factae equal. Inputs: sceletum and imparilis forks
(compaction, transmutation) and C89 snippets (frontier merge, `T * p;`
forks, chained assignment, casts), with coverage assertions (fusions,
transmutations, new-edge events > 0, frontier > 2) so the test can't go
vacuous.

Plants (silva.planta on probatio_silva_ambiguitas, plus corpus runs):
- C (ignore `praedecessores_ceteri`): red. F (values reversed): red.
  G (pending node not queued): red.
- E (fast-path new-edge counter dropped): red only after adding
  `a = b = c = d;` — the first inputs produced new-edge events only in
  the general path; chained assignment repeats the "assignment after ="
  state on different bases inside one deterministic chain.
- B (processed chain links not written to the index on hand-off): green
  on every suite at first. Corpus run: trees identical but `marg` off by
  1-2 in 16 files — after a hand-off, a general-path reduction reaches a
  chain link's STATE on another basis (status_visus), and without the
  links in the index that event is lost. Greedy reduction of lib/carmen.c
  with a plant-B probe vs a good probe gave `void g(void) { a[*n] = y; }`;
  added to the differential test -> red.
- A (links not marked exhausted) and D (repeated key not handed off):
  corpus byte-identical AND counters identical — unreachable on C89. They
  matter only if a later branch re-reaches a chain link's exact key (same
  nonterminal over the same span), which needs a grammar cycle (X =>+ X).
  Kept for exactness on any grammar (the general path handles cycles via
  the index; D also stops the fast path spinning on one). Guarded by
  reasoning, not by a test.

Lesson (again, cf. 1a): output equivalence alone missed B — the counter
probe caught it. Counters are product (spec-v2 par 12.2), so they belong
in the equivalence check whenever the GLR engine changes. Probe recipe:
copy silva/instrumenta/principalia/arbor.c, replace the tree write with a
printf of parsura counters, link against silva/build objects (see
arbor.sh); env NUMER_FONS lets a scratch text stand in for a repo path so
aedilis still supplies the header closure.

Computus golden: allocations fall slightly (index/reducenda segments no
longer touched in deterministic steps) — regenerated in the same commit,
cause named.
