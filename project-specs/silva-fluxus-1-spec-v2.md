# Silva fluxus gradus 1 — initialisatio dataflow (spec v2, codebase-grounded)

Date: 2026-07-17. Supersedes silva-fluxus-1-spec.md (v1) — same
decisions, now anchored file:line by three exploration passes.
Sources: calibration battery (17 specimens, Apple clang 16.0.0),
simulatio, interview (silva-fluxus-1-interview.md), park
01KXSETFAPHYG42S1K5QQC722R. All line numbers as of commit e273c70.

Deliverable: codices **71 LECTIO_ININITIATA** (definite,
oracle-paired -Wuninitialized, SUSPECTUM), **72
ININITIATA_QUANDOCUMQUE** (sometimes, oracle-paired
-Wsometimes-uninitialized, SUSPECTUM), **73 ININITIATA_FORSITAN**
(dormant census-only residual) + CFG short-circuit surgery + retained
dataflow tables (substrate for the codex-62 re-arm, decretum
01KXRBS0VQ, and solarium queries).

## §1 Oracle model + §2 pin table

Unchanged from v1 (read them there; they are binding). Summary:
three classes — definite ("is uninitialized", anchor at use),
sometimes ("whenever X" LITERAL, single blamable edge, anchor at
condition), residual ("may be", off-by-default). Address-taken = a
flow-positioned def event, const-T*-aware. Aggregates: member
reads invisible, member stores = may-def of the whole, whole-value
uses fire. Corpus cleanliness under -Wall = free negative oracle for
classes 1+2 exactly.

**Gate pins — RESOLVED (round 5, s18/s19/s20):**
1. `sizeof(x)` on uninit: SILENT (unevaluated) — extraction skips
   magnitudo subtrees.
2. member-write-then-whole-value-use: SILENT under BOTH flag sets,
   even with other members never written — **member/element store =
   FULL def of the whole aggregate** (both bitsets, same as scalar
   assignment). The sigillum residual fire is re-explained: its
   element writes sit inside a possibly-zero-trip loop — the
   residual class came from the loop, not from grain.
3. condition-position short-circuit: FIRES, "whenever '||' condition
   is true", in BOTH if-condition and while-condition seats — clang
   splits condition evaluation. §3's condition-position seat is IN
   SCOPE, unconditionally.

## §3 Chunk 0 — CFG surgery (grounded)

### Node facts (corrected from v1)
There are NO CONIUNCTIO/DISIUNCTIO genera. Both `&&` and `||` are
`SILVA_C89_GENUS_BINARIUM` (= 37, silva_tabulae_c89.h:59),
disambiguated by operator token `SILVA_LEX_ET_ET` / `SILVA_LEX_VEL_VEL`
(silva_token.h:111-112); accessors `silva_c89_binarium_sinister /
_tok_operator / _dexter` (tabulae h:210-212). Ternary =
`SILVA_C89_GENUS_TERNARIUS` (= 36, h:58); accessors
`_conditio / _verum / _falsum` (h:205-209). Edge origo for split
edges = the BINARIUM/TERNARIUS node; attribution vocabulary keys on
origo genus + operator token.

### Shape
One shared helper (working name `_expressionem_findere`) that walks
an expression tree and, when it contains BINARIUM(&&,||) or
TERNARIUS, emits evaluation-segment blocks with VERUS/FALSUS edges
(origo = operator node), leaving evaluation-ordered granules in
`sententiae`. Applied at TWO seats:
- **statement position**: the `ordinarius:` default of
  `_sententiam_ambulare` (fluxus.c:891-895) — detect (cheap tree
  scan per leaf; most statements lack the operators), split instead
  of `_folium_addere`;
- **condition position**: the five compound builders (`_si_fluere`
  :434, `_dum_fluere` :500, `_fac_dum_fluere` :544, `_per_fluere`
  :584, `_commutationem_fluere` :659) — pin #3 resolved IN SCOPE:
  conditions with short-circuits route through the same helper.
  Constant folding runs FIRST (existing `_aestimare` on the whole
  condition); only non-constant trees split. The hard spot is loop
  conditions: the back edge (and per's passus edge) must target the
  FIRST condition-segment block, since evaluation restarts whole.
Statements/conditions without the operators are byte-identical to
stage 0. The full statement node stays recoverable from any granule
via pater-climb.

### Blast radius (audited, exhaustive)
All stage-0 CFG consumers live in ONE function `_fluxum_examinare`
(semantica.c:3091-3243):
- codex 64 empty-label-head test reads `sententiae` count (:3157) —
  split blocks have `titulus_dux == NIHIL`, not label heads →
  unaffected;
- codex 65 iterates `sententiae` and picks the first non-skippable
  node as culpa (:3197-3211) — post-split, an unreachable
  continuation block names a GRANULE (subexpression) as culpa;
  position still valid; acceptable, note in RELATIO;
- codices 66/67 read violation tables (:3113-3130) — untouched;
- codex 63 reads `cadit_attingibilis` only (:3227-3242) — untouched.
Probationes that WILL move (chunk 0 updates them deliberately):
- probatio_silva_fluxus.c margin-count pins :196-298 (splitting adds
  VERUS/FALSUS edges), `_bloci_mortui_pleni` pins :352/:406;
- probatio_silva_examen.c e2e codex pins :1479-1612 (only if
  topology shifts diagnostics — expected stable).

### Bars (all before chunk A)
vectis TENET exit 0; census meters byte-stable (63=0 pure wall,
64=1, 65=1, 66/67 + all non-fluxus meters unchanged); probationes
green (root + silva + officina); amalgams VERIFICATUM.

## §4 Chunk A — eventa (grounded)

### Module + seam
New `silva/fontes/silva_c89_fluxus_datorum.{h,c}`. Auto-discovered
by examen.sh (glob :53-61) and compile_probationes.sh (glob :83);
amalgam needs ONE entry each in `CAPITA_SILVAE[]`
(amalgamator.c:137-155) and `CORPORA_SILVAE[]` (:157-174), +
`CADENDA_TYPEDEF[]` if public typedefs.

**Dependency direction (new decision, v2):** datorum must NOT
include semantica.h (circularity — semantica.h already includes
fluxus.h). It takes an auxilia-style seam like fluxus itself (XI-1
pattern, caller policy): `FluxusDatorumAuxilia { symbolum(ctx,
nodus) → SemanticaSymbolum-opaque*, canonicum(ctx, nodus), contextus }`
— semantica passes wrappers over `silva_c89_symbolum_nodi`
(semantica.h:709-710, canonical-read, def :8964-8976) and
`_canonicum`, exactly as `_fluxus_canonicum_ligamen` (:3071) /
`_fluxus_aestimator_ligamen` (:3078) do today. The qualificatus
strip loop (cf. `_qualibus_exutum` semantica.c:4479-4487, interior)
is reimplemented locally — it is a 4-line idiom.
Because SemanticaSymbolum itself lives in semantica.h, the seam
returns the FACTS datorum needs rather than the struct:
`b32 symbolum_indagare(ctx, nodus, FluxusSymbolumFacta* facta)`
filling `{ identitas (vacuum*), genus_grege (TRACTANDUM |
PARAMETRUM | ALIENUM), typus_facta (SCALARE | AGGREGATUM),
functio_facta pro vocatis (parametra const-ness per positionem) }`.
Semantica computes them from `symbolum->genus/repositio/profunditas/
typus`; datorum stays semantica-blind. Exact facta shape is chunk-A
API design (Eskil rule: interface first).

### Tracked-set filter (exact, from SemanticaSymbolum h:175-198)
tracked (starts UNINIT) = `genus == SEMANTICA_SYMBOLUM_VARIABILE`
∧ `profunditas > 0` ∧ `!(repositio & (REPOSITIO_STATICA |
REPOSITIO_EXTERNA))`;
pre-initialized at entry = `genus == SEMANTICA_SYMBOLUM_PARAMETRUM`;
everything else (file-scope, functions, typedefs, constants) =
untracked. `REPOSITIO_REGISTRI` tracks normally.

### Event extraction
Per-block ordered `FluxusEventum { identitas, genus (USUS |
DEFINITIO | DEFINITIO_LOCI), nodus, ordo }`.
Two sources keyed on edge genus (conditions live on edges): leaf
statements/granules in order, then the terminal condition expression
of conditional out-edges (VERUS/FALSUS share origo; switch head =
pater-climb from a CASUS edge origo). Eval-order rules per v1 §2
(RHS-before-LHS-def, use-then-def for `+=`/`++`, magnitudo skip,
&-operand not a read, subscript reads index, declarators
left-to-right, `int x = x` flagged during extraction).
Member/element stores emit DEFINITIO of the whole variable (pin #2:
full def, both bitsets — clang's grain). No may-def event genus
exists: post-surgery every conditionally-evaluated def sits in its
own block; if an unliftable case ever appears, it returns as a named
addition, not a silent weakening.
&-const: callee symbol via the seam → `typus->datum.functio`
(`parametra / numerus_parametrorum / est_variadica / est_prototypata`,
semantica.h:118-124, guard `genus == TYPUS_C89_FUNCTIO`) →
positional param type → strip qualificatus → const ⇒ USUS not
DEFINITIO_LOCI. Unknown callee / variadic tail / K&R = non-const.
ERROR/AMBIGUUS opaque leaves ⇒ def-all. Alienity + TU-degradation
abstention inherited (69/70 pattern).

## §5 Chunk B — punctum fixum

Unchanged from v1: predecessor index (margines are outgoing-only,
one pass); may (union) + must (intersection) bitsets per block, one
transfer (DEFINITIO and DEFINITIO_LOCI both set both bitsets);
worklist from introitus; never-visited preds
skipped in meets (folded `si (0)` invisibility = s04d parity,
structural). No per-use state storage — post-fixpoint re-walk
judges uses streaming.

## §6 Chunk C — iudicium (grounded)

### Registration (mechanical)
Three enum lines in `ExamenCodex` before `EXAMEN_CODEX_NUMERUS`
(semantica.h:376; 68/69/70 at :373-375) — ordinals become 71/72/73
automatically (mappa generated from enum order,
examen_vectis.sh:82-85; -census needs zero wiring). Three matching
`_codices[]` rows (semantica.c:100-167, static-assert :170-172):
71/72 `EXAMEN_SUSPECTUM` (codex 63's severity, row :157-158 as
exemplar), 73 severity nominal (never fires). TOLERA is per-fire-site:
guard 71/72 firings with `_tolera_absorbere(sem, nodus, codex)`
(codex 64 exemplar :3168-3172). Oracle-pair positions via
`silva_c89_diagnosticum_addere_cum_socio` (:389-396).

### Seat
Extend `_fluxum_examinare` (the single CFG-consumer function): after
the stage-0 codices, build datorum
(`silva_c89_fluxus_datorum_aedificare(piscina, fluxus, &aux_datorum)`),
store the pointer on FluxusFunctionis (`datorum` field, NIHIL until
built), run judgment. Always-on (interview Q8); RELATIO reports push
latency + piscina delta; capsula_libri INFRA-under-load is the
canary.

### Judgment
- 71: use U of x, x ∉ may(running). Anchor at use. Self-init causa
  variant.
- 72: x ∈ may ∖ must + blame edge E=(P→S): (a) x ∉ may_out(P), (b) U
  inevitable from S (every path reaches U before def-of-x and before
  exitus). Per-candidate backward walk (rare). Anchor at E.origo,
  socius = use. Attribution from origo genus + operator token:
  SI+FALSUS "quandocumque conditio 'si' falsa est" / SI+VERUS "…vera
  est" / DUM|PER+FALSUS "quandocumque ansa exit quia conditio falsa
  est" / BINARIUM(ET_ET)+FALSUS "quandocumque '&&' falsum est" /
  BINARIUM(VEL_VEL)+VERUS "quandocumque '||' verum est" /
  ORDINARIUS-edge with origo genus ORDINARIUS "quandocumque
  ordinarius sumitur" (implicit ORDINARIUS edges carry origo =
  commutatio → NOT blamable — the labeled-default distinction is
  already in the graph, fluxus.c:747 vs :705).
- 73: maybe-uses with no blame edge; `_forsitan_vigil` FALSUM
  (DEGRADATA 62 pattern); census row is the standing meter; lib/
  baseline 3 (chorda:1259, flatura:1783, sigillum:184 — all
  false-positive-shaped).

## §7 Bars

- Specimens: all 20 (s01–s20) graduate to
  fixa/examinis as `.suspectum` fixtures with `ORACULUM
  -Wuninitialized` directives (leg ②b′, examen_vectis.sh:194-244;
  grade $4=="suspectum"): firing specimens carry EXSPECTA-PROXIMA
  pins; SILENT specimens ride the same leg with ZERO pins — the
  count(pins)==count(warnings) assert (:182-form) makes
  silence-parity checkable. In-arc verification: the leg accepts
  pin-less fixtures (if not, minor vectis extension first). There is
  no `.probus` extension — silence lives in the equal-counts
  contract.
- Corpus first contact: PURE WALL expected for 71/72 (classes 1+2 =
  exactly corpus cleanliness). Any site = predicate bug or real bug
  clang missed; both are harvest.
- Census meters: 71=0, 72=0 walls; 73 = new dormant meter.
- Suites: root + silva 31/31 + officina 12/12; amalgams
  VERIFICATUM; vectis TENET on script exit (the law).
- New unit probatio: `probatio_silva_fluxus_datorum.c`
  (auto-discovered, compile_probationes.sh:100) — ordered-event
  extraction pins + bitset fixpoint pins on small graphs.

## §8 Named deferrals (paths back)

Unchanged from v1: unknown-callee const-ness assumed non-const;
setjmp unhandled (named); full clang-style linearization not needed
for parity (split-at-short-circuit is the v1 granularity contract);
codex 62 re-arm = its own pull once datorum def-use chains exist.
