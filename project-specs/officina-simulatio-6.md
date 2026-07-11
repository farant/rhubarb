# officina simulatio ⑥ — M4a: lamina exsecutionis (examen)

*2026-07-11. Gradus 2/4 of the M4a on-ramp (grounding → SIMULATIO →
interrogatio → spec). Pretend-implementation against the sealed tree,
~2.5 simulated hours across four chunks: A diagnosticum v2,
B compatibility relation, C the verdict seats, D driver + differential
harness. Grounding facts assumed from
project-specs/officina-m4a-fundamenta.md (read it first). Three
real-code checks were made during the sim (the seat's actual body
silva_c89_semantica.c:3338-3364 incl. its own "Compatibilitas profunda
= lint" comment; the 245-diagnostic residuum ALL-classified at
silva/phase-log.md:6345; demissio's conversus consumption
officina_demissio.c:291-296 — absent = no conversion). No shipped code
was changed.*

**Outcome: NO reversal.** The fundamenta held; two of its claims got
REFINED (the fall-through is not all violations; severity is not
always call-site-intrinsic), one feared cost DISSOLVED (no member-wise
struct compatibility in v0), one interview question dissolved (Q8
fabricated-conversus tolerance — verified in real code), and the
regression proof for the subtractive fix turned out to already exist
(fusor byte-idem circuli).

## Chunk conclusions

- **A (diagnosticum v2):** struct gains {codex (stable enum),
  severitas, positio materializata, socius (related node)}; shared
  resolver `silva_c89_diagnosticum_positio` does the censor walk once
  (token → radix → linea:columna + via + macro-context per origo);
  44-site retrofit is one mechanical sweep; the 5 NIHIL-node sites
  have a node in scope one frame up (plumb) or take a fallback token.
  Sealable against the existing suite + amalgam gates alone.
- **B (relatio compatibilitatis):** pure function over two interned
  TypusC89* — pointee recursion + qualifier set-inclusion + array-size
  sentinel (-1) + prototype/K&R reconciliation + void*/null-constant
  cases. Nominal tag identity does the aggregate work (C2). Most
  unit-testable module in the layer: table-driven probatio, ~40 pairs.
  The severity-bearing decision procedure (LICET / MONE_* / VIOLATIO)
  sits on top and is where the taxonomy becomes code.
- **C (sedes):** `_ad_finem_annotare` becomes three-way (arith/ptr
  branches → aggregate-identity LICET → residue violatio) with the
  monstrator branch SPLIT (legal: null constant, void* either way,
  compatible pointee → keep annotating; illegal: incompatible pointee,
  qual-discard → diagnose + REFRAIN from annotating). Lvalue =
  compute-on-demand `_est_locus` form-walk at exactly three consumer
  sites (ASSIGNATIO, &, ++/--) — no storage, no contract change.
  Arity/return/controlling-scalar wire in the same pass with the
  exemption set named (unprototyped + est_implicitum + variadic
  fixed-part minimum).
- **D (driver + harness):** single-file driver defaults to loading the
  ISO systema seed (vindex_onerator pattern; -posix flag), prints
  verdict + diagnostics with positions, marks verdicts under
  retained-AMBIGUUS canonicals provisional. Differential = verdict
  column on percursus machinery (examen side) vs the calibrated oracle
  string forked per file (clang side); invalid corpus = include-free
  fixtures with EXSPECTA (line, warning-group) markers, grep/sed
  extractor, bless-from-oracle.
- **Sequencing (each independently sealable):** A diagnosticum v2 →
  B relation → C seats → D driver + differential. Scale feels
  M1-like (days), not M3-like: the machinery is concentrated and the
  hard part (detection) mostly exists.

## Complications (four-part schema)

**C1 — severity is not always call-site-intrinsic.**
Discovered-while: retrofitting severities across the 44 sites.
Consists-in: "typus nominatus ignotus" is violatio with a loaded
lexicon but an infrastructure gap without one (the 18,293→245 M0b
history is exactly this class). Consequences: a per-site severity
constant would misjudge lexicon gaps as rejections and break the
100%-accept bar. Handled-by: those sites get their own codex class
(LEXICON_DEEST); the harness treats that class as infrastructure,
never judgment. Severity stays a call-site constant; the context
dependence is confined to one named class.

**C2 — member-wise struct compatibility DISSOLVES in v0.**
Discovered-while: fearing a recursion guard for
`struct S {struct S* next;}`. Consists-in: within one semantica world,
tag compat is nominal-by-declarans — identity or incompatible; C89's
structural member-wise rule (6.1.2.6) only applies ACROSS translation
units, and M4a judges one TU at a time. Function-type recursion
terminates through nominal tags. Consequences: no recursion guard, no
member-walk; the relation stays ~150 lines. Handled-by: v0 scope
statement in the spec; cross-TU composite types named as the M4b/link
question it actually is.

**C3 — the qualifier discipline splits in two.**
Discovered-while: writing the relation's qualifier rule next to the
seat's existing `_qualibus_exutum(finis)`. Consists-in: assignment
legality needs POINTEE-qualifier inclusion (const char* ← char* legal;
reverse is the CV); modifiability needs TOP-LEVEL quals — which the
seat rightly strips for the value conversion. Consequences: one
conflated check gets both wrong (e.g. rejecting legal top-level-const
initialization, or missing const-pointee discard). Handled-by: two
homes by design — relation owns pointee quals; `_est_locus` owns
top-level quals. Spec draws the line explicitly.

**C4 — the silent fall-through includes LEGAL aggregate assignment.**
Discovered-while: rewriting the seat's tail (real code read this
session, c:3338-3364). Consists-in: `struct S a; a = b;` falls off the
same silent end as `int z = struct_val;` — correctly unannotated today
(no conversion exists); the fundamenta/explorer-II table framed the
tail as pure violation territory. Consequences: rejecting the
fall-through wholesale rejects every struct assignment in the corpus —
instant 100%-accept catastrophe on first contact. Handled-by:
three-way verdict with an aggregate-identity LICET branch (nominal
identity via C2 makes it a pointer compare). Fundamenta fact 3
REFINED, not reversed.

**C5 — the regression proof for the subtractive fix already exists.**
Discovered-while: asking how to prove refrain-from-annotating breaks
nothing. Consists-in: demissio reads absent conversus as "no
conversion" (officina_demissio.c:291-296, verified); the corpus is
clang-clean so the illegal monstrator subset should never fire on it.
Consequences: if any corpus site DOES ride the fabricating path, its
lowered bytes change. Handled-by: fusor's standing byte-idem circuli
sweep IS the proof — run before/after the seat fix; byte-identical =
the fix is invisible on valid code. Zero new machinery. (Interview Q8
dissolved.)

**C6 — lvalue bit wants NO storage.**
Discovered-while: instinct said "add est_locus to SemanticaTypatio."
Consists-in: that struct is demissio's frozen input contract; adding a
field demissio never reads disturbs the layout for nothing.
Consequences: contract churn + amalgam layout ceremony for a
three-consumer property. Handled-by: compute-on-demand recursive
`_est_locus(nodus)` form-walk called at exactly the three judgment
sites. Modifiable-locus = locus ∧ ¬const(top-level) ∧ ¬array ∧
complete ∧ ¬function. Vocabulary candidate: locus/valor.

**C7 — arity's exemption set must be named or the corpus screams.**
Discovered-while: wiring the VOCATIO count check. Consists-in: C89
imposes NO argument constraint without a prototype in scope; K&R
declarations and every est_implicitum synthesis are unprototyped;
variadic checks fixed-part minimum only. Consequences: naive
count-compare rejects legal unprototyped calls corpus-wide.
Handled-by: est_prototypata/est_variadica flags already on the type
(h:118-123) — the check gates on them; the exemption set is stated in
the spec, and implicit-callee arity divergence lands in the SUSPECT
tier at most.

**C8 — judgment without a lexicon is judgment of a guess.**
Discovered-while: designing the single-file driver's default behavior.
Consists-in: a bare snippet with `Foo * bar;` canonicalizes to the
expression reading and fires spurious unknown-id violations (explorer
II t/12); interactive/fresh code hits this constantly. Consequences:
the driver's naive mode would reject valid code and poison trust in
the tool. Handled-by: driver DEFAULTS to loading the ISO systema seed
(vindex_onerator's exact pattern; -posix opt-in), and hard verdicts
are gated on resolvedness — nodes under a retained-AMBIGUUS canonical
print as provisional (extend _intra_ambiguum, no new mechanism).
Interview Q7 gets a concrete recommendation.

**C9 — the accept bar is zero-VIOLATIO, not zero-diagnostics.**
Discovered-while: pointing the pretend accept-sweep at the corpus.
Consists-in: the real corpus fires 245 standing diagnostics in 28
files TODAY (silva/phase-log.md:6345) — and they are already
ALL-classified in the M0b worklog table. Consequences: a
zero-diagnostics bar fails immediately and wrongly. Handled-by: the
causa→tier classification (needed anyway, C1) maps the existing
245-classification to tiers; the bar asserts the violatio tier is
EMPTY on the corpus. Anything violatio-tier in those 245 is either a
severity mis-assignment or a real house bug — both wanted.

**C10 — system-header parity is dodged, not solved.**
Discovered-while: sketching the two-sided differential on fixtures.
Consists-in: clang judges TUs against real macOS headers; examen
against house systema text — divergent 'unknownness' at the edges.
Consequences: fixture diagnostics could disagree for
non-judgment reasons. Handled-by: the invalid corpus is INCLUDE-FREE
by rule; the real-corpus side reuses the enumerated TU_FLAGS include
set (haruspex precedent) where both sides have compiled cleanly for
years. The parity axis is named and avoided in v0, not resolved.

**C11 — the EXSPECTA extractor must not be silva.**
Discovered-while: writing the pretend marker-extractor. Consists-in:
using silva to parse fixtures that exist to break silva-adjacent
layers couples the test harness to the thing under test.
Consequences: a parser/judgment bug could silently corrupt the
harness's own expectations. Handled-by: grep/sed extraction only
(captare.sh precedent); markers live in trailing comments with a rigid
one-line grammar; pins at (linea, warning-group), blessed from oracle
output.

## Effect on the interview questions (fundamenta §III)

- Q7 (ambiguity policy): concrete recommendation — provisional-mark +
  lexicon-by-default in the driver (C8). Fran decides suppress vs mark.
- Q8 (fabricated-conversus tolerance): DISSOLVED — verified in real
  code; fusor byte-idem is the standing proof (C5).
- Q11 (secondary bar): cheap — fusor sistere census already reports
  the semantica-diagnosed class; recommend adopt as recorded
  measurement, gate optional.
- Q4 (severity policy): gains the LEXICON_DEEST class (C1) and the
  245-mapping exercise (C9) as concrete deliverables.
- Q6 (v0 scope): sim supports core = relation + seat three-way +
  locus + arity/return/scalar; switch case-set and initializer
  validation stay named-deferred (nothing in the sim needed them).
- NEW ceremony question: Latin vocabulary for the layer's public
  words — severitas tier names (VIOLATIO / SUSPECTUM / LINT-DOMUS?),
  lvalue = locus?, verdict = iudicium?, the LEXICON_DEEST codex — plus
  the layer/driver name itself (examen candidate) and the driver's
  home. One vocabulary pass at the interview, M1 precedent.

## Measure-first unknowns (carried to the spec)

- Inline enforcement cost on the percursus sweep (relation is pointer
  compares + shallow recursion over 1.5M nodes — expect noise, but
  measure-and-record per the freeze precedent).
- Exact clang-side wall time forked over the corpus (23-41ms/file
  measured serial; the few-seconds estimate wants confirming).
- How many of the 245 land violatio-tier on first mapping (expect 0;
  any nonzero = finds).
