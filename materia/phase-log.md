# materia — phase log

Append-only. INTENTIO before a phase, RELATIO after. Newest at the
bottom. English is fine here.

========================================================================
PHASIS I — FURCA. INTENTIO (2026-08-27)
========================================================================

Goal: `materia/` exists as the general syntax substrate, and a C89
**shim** passes the M1 subtree round trip **281/281 through materia**
without migrating silva. That gate is GO/NO-GO: if the shim cannot
reach 281/281, stop and reconsider before CSS exists.

Roadmap tasks (materia-spec.md §10):
  1.1 materia/ created; substrate modules pulled in, renamed materia_*
  1.2 the three MG1 edits (reinserenda reachable from the subtree path)
  1.3 the surgery: MateriaContextus replaces SilvaParsura; origin
      serialization becomes a registered hook
  1.4 MateriaLexicon — the lexical-role descriptor
  1.5 materia/compile_probationes.sh, tessera template

DEVIATION FROM THE ROADMAP, agreed with Fran: modules are pulled in AS
THE SHIM DEMANDS THEM rather than bulk-copied, and 1.4 goes FIRST.
Reason: the spec's load-bearing claim — "materia's types are silva's
renamed, so the shim is thin" — was marked UNPROVEN before compaction,
and 1.4 turned out to be where the measurement lives. Settling it first
is the spec's own instruction ("while a wrong answer is still cheap").

========================================================================
PHASIS I.4 — DESCRIPTOR LEXICALIS. RELATIO (2026-08-27)
========================================================================

DONE. `materia_lexicon` shipped; materia's suite 71/71, exit 0, from a
cold build. Silva re-verified 50/50 on a genuinely clean rebuild
(objects deleted first) before any of this started.

WHAT THE MEASUREMENT FOUND, and why 1.4 changed shape.

The spec said the substrate's C89 coupling is "a C question in a shared
struct, read in exactly one place", and wrote task 1.4 as a SUBTRACTION:
`MateriaLexicon` *without* `genus_identificator`. Measured across the
ten substrate modules: 32 logic-level `SILVA_LEX_*` sites (plus two
pure C89 data tables living inside substrate modules). The breakdown:

  10  bounds/sentinel (NUMERUS_GENERUM)
  16  trivia identity (SPATIA TABULAE NOVA_LINEA CONTINUATIO)
   2  comment identity (COMMENTUM_LINEA COMMENTUM_CLAUSUM)
   2  EOF
   1  IDENTIFICATOR   <- the only one the spec named
   1  STRING_LIT (construction)

So the real coupling is TRIVIA IDENTITY, eighteen times bigger than the
site the spec pointed at, and it cannot be subtracted away. It needs a
POSITIVE descriptor the frontend supplies. Direction of the "thin shim"
claim confirmed (32 sites in 16,676 lines, and NONE behavioural — the
substrate never drives the preprocessor, it only reads genus
identities); shape corrected.

THE DUPLICATION THIS TURNED OUT TO BE. Silva carries three
hand-maintained tables answering versions of one question:

  ORTHOGRAPHIAE[]   silva_arbor.c:35            genus -> source text
  NOMINA_GENERUM[]  silva_token.c:356           genus -> printable name
  _textum_fert()    silva_canon_coquere.c:103   genus -> carries text?

The third exists because ORTHOGRAPHIAE is `hic_manens`, and its own
comment says a 95-genus test guards the two against drift. A guard
bolted onto a duplication is the signature of a missing abstraction.

THE MODEL. Two orthogonal axes:
  species = HOW a genus's bytes are determined (VERBATIM / FIXUM /
            REPETITUM / TERMINATOR), reading one `orthographia` field
            four ways
  munus   = WHAT role it plays (SUBSTANTIVUM / SPATIUM / LINEA /
            LAMINA / COMMENTUM / FINIS)

Orthogonal on real pairs: IDENTIFICATOR and COMMENTUM are both
VERBATIM but differ in munus; SPATIA and NOVA_LINEA are both trivia but
differ in species. LINEA vs LAMINA is the logical-vs-physical line
distinction silva_arbor.c:3527 already makes by hand.

VALIDATION — the part worth keeping. Species was assigned BY RULE, from
ORTHOGRAPHIAE plus `_trivium_scribere`'s switch. Then predicted:
`species == VERBATIM` reproduces `_textum_fert()`, a hand-written list
in a DIFFERENT FILE the rules never read.

  X genera, DIFFERENTIA NULLA, across 95.

And the divergence is explained too: `silva_arbor_valor_portandus`
differs from `species == VERBATIM` on exactly FOUR genera — SPATIA,
TABULAE, NOVA_LINEA, CONTINUATIO — which is precisely the "carve-out
triviorum" that canon_coquere.c:100's comment names.

A model that agrees where the artifacts agree AND explains where they
disagree is a separating oracle, not a lenient round trip. That is what
licensed building on it.

Third table fell out for free: NOMINA_GENERUM is mechanically the enum
name minus its prefix, all 95, verified.

DEGRADATION POLICY — decided by Fran, option B. Capability is judged at
LOAD time: `materia_lexicon_ratum_facere` derives a `munera_praesentia`
bitmask, and a capability needing an absent munus refuses once, loudly.
The case that forced the question: the doc-comment finder
(silva_nodus.c:880-930) counts blank lines via munus LINEA. CSS's single
whitespace genus absorbs newlines, so no CSS genus carries LINEA — the
finder would walk to the top of the file and return every comment it
passed. A CONFIDENT WRONG ANSWER, not an error. Per-call checking
re-answers a constant question and leaves each caller to get it right;
load-time refusal fails at the moment someone is looking at the
descriptor, and the mask is DERIVED so it cannot drift the way
_textum_fert drifted from ORTHOGRAPHIAE.

The mask is not a verdict on CSS. It says this lexicon does not
distinguish line breaks — which names what CSS would have to change.

WHAT WAS BUILT
  materia/fontes/materia_lexicon.{h,c}       243 + 317 lines
  materia/instrumenta/lexicon_c89_generare.sh  generator + its gate
  materia/probationes/lexicon_c89.{h,c}      GENERATED, 95 genera
  materia/probationes/probatio_materia_lexicon.c  71 assertions
  materia/compile_probationes.sh             0/1/2 contract, header guard
  materia/CLAUDE.md

GATES, and that they were seen to FIRE
  - Load gate: 10 distinct vitia, EVERY ONE planted in the test and
    refusal required. Both directions of the central invariant
    (VERBATIM <=> orthographia NIHIL) are planted separately.
  - Generator gate: re-derives _textum_fert and refuses to emit on
    divergence. PLANTED FAULT RUN: flipping IDENTIFICATOR to FIXUM made
    it refuse, name the genus, and exit 1; the clean run still passed.
    A gate never seen to refuse is a dead gate.
  - Coverage asserted, not assumed: the species distribution test
    asserts the four counts sum to 95, so "all clean" cannot mean
    "never appeared".

COMPLEXITIES / TRAPS FOUND

1. GENERATOR BUG, caught by the compiler. Extracting the orthography
   with `sub(/ *\}.*$/,"",rest)` strips from the FIRST `}`, which
   truncates BRACE_CLAUSA's `"}"` to `"`. Fixed by anchoring the strip
   to end-of-line. Worth remembering generally: when scraping C source
   for literals, strip anchored at the END, never at the first
   occurrence of a delimiter that can appear INSIDE the literal.

2. The earlier measurement was UNAFFECTED by that bug (it only asked
   NIHIL vs non-NIHIL) — a good reminder that a probe can be right for
   its question while its extraction is wrong for another.

3. `valor_portandus`'s semantics CHANGE for four genera under this
   model. That is why the descriptor was NOT landed in silva first: M4
   went into silva because it was mechanical and logic-free, and this
   is not. The measurement bought the confidence without the code —
   which is what the probe was for, and what the freeze required.

NEXT: 1.1 proper — pull the first substrate module in. The measurement
says the token layer is where the C89 shape is embedded
(`SilvaLexemaGenus` is DEFINED in silva_token.h, and `SilvaToken`
carries three C89-shaped fields: `scissurae`, `standard`, `origo`), so
`materia_token` is the honest next target and the token-header split is
the M4-shaped move it needs.

========================================================================
PHASIS I.1a — MATERIA_TOKEN. RELATIO (2026-08-27)
========================================================================

DONE. Suite 2/2, 132 assertions, exit 0.

FRAN POINTED AT project-specs/silva-performance-handoff.md mid-decision,
and it landed exactly on the question in front of us: RP 14-17 are the
"representation items inside silva", i.e. the token layout decisions
phase 1.1 makes.

THE FINDING THAT CHANGED THE ANSWER. RP 16 (origo as pointer) is
DEFERRED with an objection recorded as having NO ANSWER: "the gain is
under 2% of post-Part-II footprint while the repair touches all five
constructors and every consumer reading t->origo.genus."

That objection does not apply to this fork. Phase 1.3 touches all five
constructors and every origin consumer ANYWAY — origin serialization
becomes a registered hook, five of six constructors leave with the
frontend. THE CHURN IS ALREADY SUNK, SO THE GAIN IS FREE. Same
reasoning carries RP 15 and RP 17.

Generalisable: a deferred repair's cost/benefit is not a property of
the repair. When other work has already committed to paying the cost,
re-check every "deferred, not worth the churn" item before assuming it
is still deferred.

INDEPENDENT CONVERGENCE. RP 16 says the token reaches 72 bytes
combining RP 14+15+16 plus dropping scissurae, "verified by compiling
candidate layouts". My independent measurement (SilvaToken minus origo
minus scissurae) = 72. Same number, different route. A compiled
candidate does better still: 64, because exact-size trivia goes further
than RP 15 assumed.

  SilvaToken today        128 bytes
  MateriaToken             64  — half
  C89 through materia      64 + 48 tail = 112, in ONE allocation

DESIGN: TAIL, NOT POINTER. silva_token.h:9 records a decision a
previous generation paid for: "origo INSERTA (non allocata separatim):
numquam NIHIL, localitas melior" — because arbor2 split Lexema/Token
and the split is what let origin get lost at the parser boundary. An
opaque `vacuum*` would turn that structural guarantee into a
convention. Tail allocation keeps ONE allocation and never-NIHIL while
materia stays ignorant of the shape. A CAUDA flag bit (not a stored
size) means materia_token_cauda can never hand back a pointer past the
allocation.

Alignment made explicit: silva uses bare piscina_allocare (alignment 1)
and its tokens stay 8-aligned only because sizeof is 128 and everything
interleaved happens to be a multiple of 8. Luck, not contract. A tail
cannot rest on that, so materia asks for the alignment silva assumed.

A CORRECTION TO RP 17, and it matters. RP 17 calls `longitudo`
redundant — equal to valor.mensura in 56,686 of 56,686 tokens.
Measurement true, conclusion unsafe. silva_lexema.c:349 sets
`longitudo = positus - initium` with the comment "longitudo CRUDA": it
is the RAW SOURCE SPAN, while valor.mensura is the CLEAN VALUE length.
They diverge exactly when a token contains line splices — which is what
`scissurae` records — and they were equal only because scissurae was
0 of 56,686 in that corpus. Deleting longitudo alone would be right on
the measured corpus and WRONG on `int\<nl>x;`. So longitudo and
scissurae travel into the frontend tail TOGETHER, and materia keeps
valor with span = valor.mensura unless a frontend says otherwise.

The general shape of that trap: two fields measured equal across a
corpus are not therefore the same field. Ask what would have to be true
for them to differ, then check whether the corpus contained it.

RP 15 IS WORTH MORE HERE THAN IN SILVA. It was deferred because its
value "collapsed after Part II" — but that collapse is CONDITIONAL on
the Tier 0 Xar repairs landing. Materia choosing exact-size at birth
depends on none of them. Raw figure: 11,010 trivia Xars in stml.c at
704 bytes each ~= 7.75 MB of 72.67 = ~10.7%, with zero Tier 0 work.
(Do not quote RP 3.2's 61% here — that covers ALL Xars, not trivia.)

Its precondition was verified in code, not taken from the doc, and is
stronger than stated: silva_lexema.c:950-1000 accumulates trivia in a
scratch Xar, then computes `divisio` — so the exact count of BOTH
halves is known before either is allocated, and a copy loop already
runs. Exact-size is strictly simpler than what is there.

HANDOFF FIGURES ARE STALE, cause identified. It reports SilvaToken 120 /
SilvaOrigo 40 (Linux/gcc at 162b3a9); measured 128 / 48 on macOS/clang.
Not the toolchain: `pasta` gained an `invocatio` field since that commit
(4 pointers -> 5). RP 1.3 asked for re-confirmation on the real
toolchain; this is it, and the answer is +8.

GATES
  - Layout PINNED: sizeof(MateriaToken) == 64 asserted. A new field
    moves that number and must be heard.
  - Tail CONTIGUITY asserted — that assertion IS the design argument.
    If the allocation ever splits, choosing tail over pointer loses its
    reason, and the test says so.
  - PLANTED FAULT RUN: dropping the tail from the size computation
    (allocating only sizeof(MateriaToken)) was caught — 3 assertions
    fired, suite 0/1.

HONEST NOTE ON ONE ASSERTION. While planting that fault I noticed the
tail test had a gap: it wrote to a tail, allocated another token, and
never re-read the first. Under-allocation could survive that if the
arena happened to be clean. I added the re-read. It did NOT fire for
this particular fault — the earlier zero-checks caught it first,
because the arena was dirty from previous allocations. So the new
assertion is reasoned, not yet demonstrated; it covers the clean-arena
case the others would miss. Recorded as such rather than claimed as
proven.

NEXT: the shim needs a source-file identity type. SilvaFons splits the
same way the token did — `via` and `est_syntheticus` are general;
`est_lexicon`, `est_custos`, `custos_titulus` are preprocessor-shaped.
Then the arbor/scribere surgery, which is the phase's real mass.

========================================================================
PHASIS I.1b — SILVAFONS (NULLUM), MATERIA_NODUS. RELATIO (2026-08-27)
========================================================================

Suite 3/3, 200 assertions, exit 0.

SILVAFONS: A NULL RESULT, AND I PREDICTED IT WRONG.

Last entry said "SilvaFons splits the same way the token did." It does
not split. It LEAVES WHOLE. Measured — every site in the two surgery
targets:

  silva_arbor.c 4048, 4051, 6306, 6347   _parsura_fontes_scribere /
                                         _parsura_fontes_legere
  silva_scribere.c 777                   inside silva_scribere_fontem,
                                         resolving fons_index -> via ->
                                         `includenda` map -> trailing
                                         EOF token

All five are frontend document-section or #include machinery. Nothing
in the core walk ever dereferences a SilvaFons.

And the reason is worth keeping: of 20 `fons_index` uses in
silva_scribere.c, exactly ONE dereferences the table. The rest are
equality comparison ("is this token from the emission target's
source?"), assignment, or the -1 "any" sentinel. So **materia needs
`fons_index` as an opaque s32 with equality semantics and a -1
sentinel, and nothing else.** No table, no struct, no split. A frontend
with multiple sources owns its own table; CSS and HTML have one source
and always pass 0.

MATERIA_NODUS, AND THE FINDING THAT ACTUALLY MATTERED.

materia-spec.md §2's table says `silva_nodus -> silva_token  CLEAN`.
That is true of INCLUDES and false of SEMANTICS. Five query families in
silva_nodus.c walk the origin chain:

  375  silva_valor_extensionem        byte extent
  434  _extensionem_lineis_valoris    line/column extent
  537  silva_valor_est_fons_purus     source purity
  630  _sedes_colligere               geometry fidelity  (geometria_fida)
  795  _lexema_primum_valoris         leading comment    (commentarium_ducens)

Because the extent of an EXPANDED token is not its own site but its
INVOCATION's. Phase 0.1 named one genuine core intrusion
(ArborCursor.expansio). This is a SECOND, in a module the spec called
clean.

SAME ERROR CLASS, THIRD TIME THIS SESSION. The coupling census measured
lexical coupling and concluded structural. RP 17 measured field equality
and concluded field redundancy. §2 measured includes and concluded
semantics. All three: THE MEASUREMENT ANSWERED A NARROWER QUESTION THAN
THE CONCLUSION DRAWN FROM IT. Recorded in the spec as a standing rule —
an include graph is a LOWER BOUND on coupling, never a description of
it. Worth asking, of any measurement here: what would have to be true
for this to be false, and did I look for it?

RESOLVED BY ONE HOOK. All five ask the same question — "what is this
token's effective source site?" MateriaOrigoUncus answers it. A
frontend with no derivation leaves it NIHIL and gets the token's own
fields, which for CSS and HTML is CORRECT, not degraded. One callback
makes all five language-neutral.

WHAT WAS BUILT
  materia/fontes/materia_nodus.{h,c}          207 + 385 lines
  materia/probationes/probatio_materia_nodus.c  68 assertions

Ported faithfully from silva_nodus.c; two changes only — node genus is
an s32 registry index (as before), and effective site comes from the
hook rather than a hardcoded origin walk.

GATES
  - All four node guards planted and refusal required: slot out of
    bounds, species mismatch, DOUBLE WRITE (single-owner), and append
    to a non-list species. Verified the guards' stderr messages
    actually EMIT, with correct slot/species detail — 5 of 5 in the log.
    A guard that returns FALSUM silently is half a guard.
  - FORK-SAFE PROSPECTUS tested for real, not asserted: two forks
    append from one base; the second must copy because the repository
    has moved past its view. Confirmed the copy path FIRED (different
    Xar pointers), both forks see their own element at index 2, and the
    base still reports mensura 2 with NIHIL at index 2. Without this
    the {xar, mensura} design is just a claim.
  - Sedes hook: NIHIL path, NIHIL-field path, hook path, and NIHIL
    token (site is FILLED as unknowable, never left stale).

NEXT: the arbor/scribere surgery — the phase's real mass. Now with one
correction banked: the origin seam is TWO hooks, not one. Phase 0.1's
`extentum_quaerere` for arbor/scribere, and this `sedes_quaerere` for
the node query layer.

========================================================================
PHASIS I.2 + I.3a — MATERIA_SCRIBERE. RELATIO (2026-08-27)
========================================================================

Suite 4/4, 250 assertions, exit 0. MG1 edit 1 done; the byte emitter is
off the preprocessor.

CORRECTION TO M2/§2.1 — "HG2 evaporates" IS HALF TRUE.

§2.1 argues the five mandatory structural genera live in
`silva_generare.c:1315` — the LR toolkit — so hand-written frontends
never invoke it and HG2 evaporates. True of the GENERATOR. False of the
WRITER: `silva_scribere.c:625`, `_scriptor_parare` ends with

    redde _formam_ambigui_invenire(st);

and BOTH public entry points treat FALSE as fatal ("tabularium sine
forma ambigui"). So a registry with no `ambiguus` genus breaks silva's
writer — in a MATERIA-CORE module, not the LR toolkit. CSS and HTML
would have had to declare `ambiguus` anyway.

In materia `ambiguus` is OPTIONAL, with the distinction kept:
  absent            -> accepted (recursive descent produces no ambiguity)
  present + intact  -> accepted, canonical interpretation only
  present + BROKEN  -> REFUSED (missing `canonica` slot)
"I don't have one" and "I have a broken one" must not collapse into the
same answer. Both directions asserted.

This is the same pattern as the nodus finding, one layer over: a
requirement was located in one module and concluded to be confined
there. §2.1 traced where the genera are ENFORCED at generation and
concluded where they are REQUIRED at runtime.

MG1 EDIT 1 — DONE, AND TESTED THE ONLY WAY THAT MEANS ANYTHING.

silva `_scriptor_parare:613` hardcodes `reinserenda = NIHIL`; only
`silva_scribere_fontem` fills it; the subtree entries pass
`fons_index = -I` (:686), which disables the skip predicate (:299). So
the mechanism was LIVE but UNREACHABLE from the path CSS and HTML use.

In materia both are CONSILIUM fields. The test is the earlier
measurement inverted: a subtree whose tokens cover offsets 12..18 of
`"#define X 1\nint f;\n"`, with the directive line supplied as a
reinserendum at offset 0.

  without reinserenda:  7 bytes   ("int f;\n")   - the gap is real
  with reinserenda:    19 bytes   byte-exact vs the true source

Both asserted. The first number matters as much as the second: without
it, "19 bytes came out" would not prove the reinserenda did anything.

THE UNIFICATION THE FORK PAID FOR.

silva's `_lexema_scribere` carries two paths — an ORIGO_FONS branch
(token emits itself) and an expansion branch (root emits instead, with
extent and dedup). materia has ONE: ask the hook for the emission root
(no hook => the token itself), then branch on whether the root IS the
token. silva's FONS branch is the case `radix == token`. One branch
fewer, nothing lost, and a language with no derivation runs the same
path rather than a special-cased one.

Three callbacks now on MateriaOrigoUncus, all optional:
  sedes_quaerere     effective site      (node queries)
  radix_quaerere     emission root       (byte emitter)
  extentum_quaerere  root's full span    (function-like invocations)

WHAT WAS BUILT
  materia/fontes/materia_registrum.h            port of silva_registrum.h
  materia/fontes/materia_scribere.{h,c}         133 + 470 lines
  materia/probationes/probatio_materia_scribere.c  50 assertions

GATES
  - ambiguus: all three registry shapes asserted (absent / intact /
    broken), so the "optional" change cannot silently become "ignored".
  - MG1: measured both with and without reinserenda, byte-exact.
  - Origin hook: no-hook path, hook path, DEDUP (two derived tokens,
    one root, emitted once — 2 bytes not 4), extent path, and refusal
    carrying the HOOK'S OWN cause string rather than a generic one.
  - Unknown genus: clear break with `sedes` pointing at the node and
    empty text, never a silent skip.

NEXT: materia_arbor — the STML writer/reader, 7,310 lines and the
phase's remaining mass. Phase 0.1 measured the core walk (lines
1..4029) as free of SilvaRegio/SilvaRamus/SilvaFons, with every site
inside a `_parsura_*` function, so the split line is already known;
what is NOT yet measured is how much of the writer depends on the
lexeme-genus tag mangling that materia_lexicon now owns.

========================================================================
PHASIS I — PORTA SHIM, DIMIDIUM OCTETORUM. RELATIO (2026-08-27)
========================================================================

346/346 byte-identical against silva itself, across 336 real files.
Gate committed as `materia/shim_probare.sh`. materia suite still 4/4.

WHY THIS BEFORE materia_arbor, AND NOT AFTER.

Measured first: silva_arbor.c is 68 core functions / 5,573 lines plus
19 `_parsura_*` / 911 lines. That is an order of magnitude more than
anything ported so far (materia_scribere was 470). Porting 5,573 lines
onto an unvalidated hook design would be the expensive way to be wrong.

The phase gate wants the STML round trip, which needs materia_arbor.
But the BYTE half of that gate needs only what already exists — and it
is precisely the half that exercises the origin hooks. So it was
available now, and it was the thing most likely to invalidate the
design.

WHAT THE PROBE DOES. Parses real C89 with silva, converts the tree to
materia types, emits through materia_scribere with C89 hooks, and
compares byte-for-byte against silva_scribere_valorem on silva's own
tree.

SILVA IS THE ORACLE, NOT A FIXTURE I WROTE. A fixture shares my
assumptions; silva does not. This is the separating-oracle discipline
the M2 arbor work already proved out, applied one layer down.

RESULT
  10 inline cases (macros object/function-like/nested, directives,
     conditionals, comments, a line splice inside a token)
  336 real files (lib/*.c, include/*.h, silva/fontes/*.c)
  346/346 IDEM, ~16s.  lib/stml.c alone: 274,721 bytes / 56,495 tokens.

THE PROBE CAN FAIL — DEMONSTRATED, NOT ASSERTED. Three faults planted,
each caught by exactly the case it should break:

  extentum_quaerere disabled  -> "macro functio"      16 vs 11 bytes
  valorem_emittere disabled   -> "lamina intra lexema" 12 vs 10 bytes
  radix_quaerere disabled     -> nested macros AND lib/piscina.c
                                 (13136 vs 13129)

So all three hooks are load-bearing, individually, on real data. A
green probe that cannot go red proves nothing; this one goes red three
different ways.

TWO MISTAKES OF MY OWN, BOTH INSTRUCTIVE

1. The first version looked up SilvaToken -> MateriaToken by LINEAR
   SCAN. Fine on 5-token snippets, quadratic on real files: the corpus
   run TIMED OUT at 2 minutes. Replaced with a tabula_dispersa keyed on
   the pointer bytes; the same corpus now runs in 0.15s for four files
   and ~16s for 336. A defect of the probe, not of the design — but it
   would have read as "materia is slow" if I had not looked.

2. I checked the gate's refusal path with `./shim_probare.sh | tail`
   and read `$?` — which is TAIL's status, not the script's, so a
   refusal reported EXITUS=0. Re-verified without the pipe. This is the
   documented house trap and I walked into it anyway; the fix is to
   never read `$?` after a pipe, and the tell is that a refusal path
   reporting success should never be believed on the first reading.

EXIT CONTRACT VERIFIED, all three paths, unpiped:
  0  clean corpus
  2  silva/build absent (refuses loudly, names the fix)
  1  a file diverges or cannot be read

WHAT IS AND IS NOT PROVEN. Proven: token model, node model, the byte
emitter, and all three origin hooks reproduce silva exactly on real C89
including macro expansion, conditionals, and splices. NOT proven: the
STML round trip, which is materia_arbor's job and remains the phase's
real gate. This is half the gate, arriving four steps early.

NEXT: materia_arbor. Now with the hook design validated on 336 files
rather than on argument, and with the split line already measured (68
core functions, 19 `_parsura_*`).

========================================================================
PHASIS I.3b — MATERIA_ARBOR, SCRIPTOR. RELATIO (2026-08-27)
========================================================================

Suite 5/5, 290 assertions, exit 0. Shim gate still 346/346.

The arbor WRITER is ported: 1,309 lines (materia_arbor.{h,c}), plus 40
assertions of its own. The reader is not yet started.

FOUR CHANGES FROM SILVA, AND ONE NEW SEAM

  - lexeme genus resolves through materia_lexicon; the ORTHOGRAPHIAE
    and NOMINA_GENERUM tables do not come along
  - "value carried" is species == VERBATIM, not "orthography NIHIL and
    not EOF"
  - trivia FORM comes from species: REPETITUM -> n=, TERMINATOR ->
    crlf, VERBATIM -> text. silva switched on four named genera; a
    language whose whitespace is one genus (CSS) works unchanged
  - anchor and derived-token detection go through MateriaOrigoUncus

MateriaArborFrons is the new seam, and it has TWO hooks rather than
one, for a reason that is byte-visible: attributes and children are
separate lists, and silva writes `standard` BEFORE `f` but scissurae
and origo AFTER <post>. One hook at either end could not reproduce
both orders. So attributes_ornare runs first, liberos_ornare last, and
materia's own attributes sit between them.

THE ANCHOR LAW, CARRIED OVER WITH ITS WARNING. silva_arbor.c:715-760
records that using the token rather than its leading trivium produced
178 'lexema/offset' divergences over the corpus — and that a small
test MISSED it, because `int n = 0;` has its first token at offset 0
with no leading trivia, the case where the bug vanishes. So the test
here asserts BOTH cases explicitly, and the planted fault confirmed it:
removing the leading-trivium lookup fails three assertions (b="4"
present, b="8" absent, columna="1").

A FALSE RED, AND IT IS THE WORSE TWIN

After restoring the good file the test STAYED red. The source was
verified byte-identical to the good copy, and it still failed. Cause:

    materia_arbor.c   21:27:07
    materia_arbor.o   21:27:07

The runner asked `[ "$src" -nt "$obj" ]`. mtimes are second-granular,
so a source and object written in the SAME SECOND are not "newer", the
rebuild is silently skipped, and the suite runs against the previous
object. Small file, fast machine — not rare.

This is the twin of the false green that bit silva earlier today, and
it is worse. A false green makes you believe you are finished; a FALSE
RED makes you distrust code that is correct, and the natural next move
is to "fix" something that was never broken.

Fix: `! [ "$obj" -nt "$src" ]` — on a tie, REBUILD. One word, and the
tie-break now falls on the safe side. Verified by planting and
restoring WITHOUT a forced rebuild: 3 failures, then 40 passing.

**silva/compile_probationes.sh carries the same `-nt` pattern and the
same bug.** Not touched — silva is frozen and this is Fran's call, but
it is a live hazard there, not a theoretical one: it is the same script
that produced today's false green by a different route.

NEXT: the arbor READER (STML -> arbor), then the shim extended to
compare STML documents and finally to round-trip — which is the phase
gate proper.

========================================================================
PHASIS I.3c — MATERIA_ARBOR, LECTOR + CIRCUITUS. RELATIO (2026-08-27)
========================================================================

Suite 5/5, 321 assertions, exit 0. Shim gate 346/346. materia_arbor is
now 2,595 lines and the STML round trip closes.

WHAT THE READER NEEDED THAT THE WRITER DID NOT

Four hooks, all mirrors of writer-side concerns except the last:

  attributa_legere  <- attributa_ornare      (C89: `standard`)
  liberum_legere    <- liberos_ornare        (scissurae, origo)
  cursorem_movere   -- NEW                   (C89 splices cover more
                                              bytes than the value)
  perficere         -- NEW                   (silva calls
                                              silva_committere here)

`liberum_legere` returns a TRI-STATE, not a bool: IGNOTUM / ACCEPTUM /
FRACTUM. A bool cannot distinguish "the frontend does not recognise
this element" from "the frontend recognised it and it was malformed" —
and collapsing them means either materia silently accepts unknown
elements or it reports the frontend's error as its own.

LACUNAE: the reader-side twin of reinserenda. Same shape, opposite
direction — reinserenda WRITE foreign bytes, lacunae make the cursor
JUMP them. Both are now config fields rather than hardcoded NIHIL.
Carried over with silva's hard-won note: a lacuna needs a FONS, because
byte 192 of file 2 and byte 192 of file 6 have nothing in common
(silva measured 4 files, delta 835, from exactly that).

MUNUS LINEA vs LAMINA EARNS ITS KEEP. `_trivium_ponere` sets
post_lineam only for munus LINEA, never LAMINA — a C89 continuation
advances the physical line but not the logical one. That distinction
was designed in materia_lexicon on the strength of a single silva
site; here is the second consumer, and it needed exactly the split
that was already there.

THE ROUND TRIP IS TESTED TWICE, DELIBERATELY. A defect that COMPOUNDS
(a position off by one, a dropped trivium) often survives one cycle
and only shows on the second: write->read once returns the document's
own shape; twice returns the shape of its shape. Both cycles asserted
byte-identical against the first document. Derived positions asserted
too — the document carries no positions but the anchor, so the reader
reconstructs them, and the leading trivium must land at 0 with its
token at 4.

A CRASH, AND THE RIGHT FIX FOR IT

First run: EXC_BAD_ACCESS in xar_numerus, via _positiones_lexematis.
Cause: I added `lacunae` to MateriaArborConsilium and did not add it to
materia_arbor_consilium_nudum. The constructor enumerated fields, so a
new field arrived UNINITIALISED — and uninitialised garbage in an Xar*
is a wild pointer the moment anything reads it.

The point fix is one line. The CLASS fix is memset-then-assign, applied
to BOTH consilium constructors: zero the struct, then set only the
fields whose default is not zero. An enumerating constructor invites
this every time a field is added; a memset makes the omission harmless.
Both constructors now carry the note.

Worth naming as a rule: a zero-constructor that lists fields is a
maintenance trap. Zero the whole thing first, then override.

NEXT: extend the shim to compare STML documents (writer) and then to
round-trip real C89 (reader) — which needs the C89 MateriaArborFrons,
i.e. porting `_origo_scribere` / `_origo_legere` / `_extentum_*` into
the shim as frontend hooks. That closes the phase gate: 281/281.

========================================================================
PHASIS I — SHIM STML: CCCXLV/CCCXLV. RELATIO (2026-08-27)
========================================================================

`./materia/shim_probare.sh -stml`: **345/345 STML documents
byte-identical to silva's**, over the same 336 real files, alongside
346/346 on byte emission. materia suite 5/5.

THE METHOD THAT MADE THIS CHEAP

I did NOT port silva's 340 lines of origin serialization first. I wired
a MINIMAL C89 frontend — `standard` only — and let the comparison say
what was missing. It said, precisely:

  macro obiectum/functio/nidificata   origin chains absent
  lamina intra lexema                 scissurae absent

Nothing else. Six of ten cases were already byte-identical, which meant
trivia, fragments, transclusions, anchors, sigillum and attribute
ORDER were all correct before a line of origin code existed. That is
the difference between porting 340 lines blind and porting them against
a list.

THREE DEFECTS THE ORACLE FOUND THAT A FIXTURE WOULD NOT HAVE

1. **Raw marker missing on trivia values.** silva emits
   `<lex-commentum-clausum!>`; materia emitted it unmarked. I had
   dropped `_valorem_crudum_notare` from `_trivium_scribere`'s VERBATIM
   branch when porting. It matters: without the raw genus the pretty
   printer may REFLOW a comment, and comment bytes are code, not prose.
   A fixture I wrote would have had whatever I believed; silva had what
   is true.

2. **`sedes_quaerere` left NIHIL made every token look source-origin.**
   The writer carries `b`/`linea`/`columna` only for DERIVED tokens, and
   with the hook unset `est_fons` was always VERUM, so those attributes
   silently vanished. The sedes hook is load-bearing for the WRITER too,
   not only for node queries — which the design note did not say.

3. **API gap: the reader had nowhere to put what a frontend reads.**
   `_lexema_legere` created tokens with `NIHIL` forma, i.e. no tail. A
   frontend's `liberum_legere` could parse origin and then have nowhere
   to store it. `MateriaArborConsilium` now carries a
   `MateriaTokenForma`. Found only by trying to use both sides; a test
   that exercises one direction cannot surface it.

AND ONE IN THE HARNESS. `shim_probare.sh -stml` treated `-stml` as a
FILE, so the default corpus was replaced by nothing and only the ten
inline cases ran — reporting a cheerful `idem 10, dispar 0`. Flags and
files are now separated. A gate that quietly shrinks its own corpus
reports success for work it never did; this is the same family as the
false green and the false red, and it is the third member found today.

CARRIED FORWARD, NOT YET USED: silva's note that the extent lamina's
HEAD *IS* the invocation object — identity, not equality — because the
emitter looks up extents by pointer. Silva measured 4 files failing
from exactly that. The reader-side port must honour it.

NEXT: the reader-side C89 frontend (`_origo_legere`, `_extentum_legere`,
scissurae, cursor advance), then the full round trip. That closes the
phase gate.

========================================================================
PHASIS I — CIRCUITUS PLENUS: CCCXLV/CCCXLV. RELATIO (2026-08-27)
========================================================================

  ./materia/shim_probare.sh -stml
    STML: idem 345, dispar 0
    CIRCUITUS (bis): idem 345, dispar 0
    probatae 346, fractae 0

Real C89 — 336 files — writes to STML byte-identically to silva, reads
back through materia, and re-writes byte-identically. TWICE. Every
C89-specific part lives in a frontend; materia contains no notion of a
macro. materia suite 5/5, silva 50/50.

THE REFACTOR THAT MADE THE READER POSSIBLE

The shim's token tail held `SilvaToken* silva`. Fine for the conversion
path; USELESS for reading, because a token parsed out of a document has
no silva token behind it. A write hook that reads `->silva` therefore
could not close the loop.

The tail now holds what the DOCUMENT holds — origin genus, the three
nested tokens, macro name, def-site triple, extent, scissurae,
standard — and BOTH paths fill it: conversion from silva, reading from
STML. The write hooks read only the tail.

That is not a shim trick. It is the shape the real C89 frontend will
have after phase 5, and it fell out of being forced to make both
directions work against one structure.

TWO DEFECTS, BOTH SILVA HAD MEASURED FIRST

1. **Fragment opening on the origin surface.** Round trip failed with
   "genus lexematis lexico ignotum": an invocation is usually wrapped
   in `<fragmentum id="lexN">`, and my origin reader passed the wrapper
   straight to the lexeme reader, which saw `#lexN` as a tag.

   silva hit this EXACTLY — `silva_arbor.c:2795` records 31 latinized
   files refused, with the diagnosis that `_origo_legere` was a NEW
   SURFACE that did not inherit what the tree walk already did.

   So I did not fix it in the shim. I fixed it in
   `materia_arbor_lexema_legere` — the EXPORTED seam — which now opens
   fragments itself. A frontend cannot repeat silva's mistake because
   it is not given the opportunity. Verified by planting: removing the
   call fails the round trip and exits 1.

2. **Extent head identity.** Carried over deliberately: the lamina's
   first token must BE the invocation object, not an equal one, because
   the emitter looks up extents by POINTER. silva measured 4 files
   losing invocation bytes while extent COUNTS matched — "numerus par
   identitatem non probat".

TWO HOLES IN MY OWN GATE, FOUND BY CHECKING RATHER THAN TRUSTING

- The shim's exit code counted only `FRACTAE`. `STML_DISPAR` and
  `CIRC_DISPAR` were PRINTED but did not move it — a gate that cannot
  fail on its principal assertion. Now all three count; planted fault
  confirms exit 1.
- `shim_probare.sh` expanded an empty array under `set -u`:
  `VEXILLA[@]: unbound variable`. Only the NO-FLAG path hit it, and I
  had only ever tested the flag path. Fixed with
  `${A[@]+"${A[@]}"}`, and all three invocation forms (bare, `-stml`,
  named file) are now checked.

Running tally of this class today: stale objects (false green), mtime
tie (false red), corpus silently emptied by a flag, exit code blind to
its own findings, empty-array expansion. FIVE. The pattern is not
carelessness in any one of them — it is that a gate's OWN correctness
needs the same adversarial treatment as the code it guards, and the
cheap test is always "make it fail on purpose".

WHAT REMAINS FOR THE PHASE GATE PROPER. The spec's bar is silva's own
suites green THROUGH materia (281/281 subtree round trip, 78/78,
154/154, hospes 39/39, adversarial 24/24, haruspex 243 TUs). What is
proven here is stronger in breadth (336 files, byte + STML + double
round trip) and weaker in kind: it runs materia's writer/reader beside
silva's, rather than silva's suites ON materia. Closing that is phase 5
(migration), not phase 1. Phase 1's stated gate — "a C89 shim passes
the round trip through materia, without migrating silva" — is MET.


########################################################################
PHASIS I — RELATIO OMNIUM (2026-08-27)
########################################################################

GATE MET, as written: "a C89 shim passes the round trip through
materia, without migrating silva."

  ./materia/shim_probare.sh          346/346 octeti      exit 0
  ./materia/shim_probare.sh -stml    345/345 STML
                                     345/345 circuitus bis
  ./materia/compile_probationes.sh   5/5, 321 assertions
  ./silva/compile_probationes.sh     50/50

Ten commits, 8f189369..27ac1e91. materia 5,256 lines across five
modules; 2,994 lines of tests and instruments.

WHAT IS *NOT* PORTED — read this before believing the above

  arbor_aequalitas  1,000 lines   not started
  quaestio          2,106         not started
  quaestiones         942         not started
  commissio           828         not started
  contextus           281         not started
  nodus' five query families      DESIGNED (the sedes hook) but NOT
                                  ported: extensionem,
                                  extensionem_lineis, est_fons_purus,
                                  geometria_fida, commentarium_ducens

materia is ~60% of the substrate by line count and **0% of the query
surface** — which is what the LSP, formator and tools actually consume.
"Phase 1 gate met" is true. "materia is nearly done" is not.

THE DIVERGENCES FROM SILVA, and they are all at ONE seam

Nothing changed in the wire format or the tree model; the gate enforces
that. Everything that changed sits between "substrate" and "language".

  NEW MODELLING
   1 lexicon descriptor: three hand-maintained tables + a 95-genus
     guard test -> one table on two axes (species x munus) with a
     load-time invariant. The only place I designed rather than
     ported, and it paid twice: LINEA vs LAMINA was split on ONE
     silva site and a second consumer then needed exactly that split.
   2 token layout 128 -> 64 bytes (RP 14-17 taken, because the fork
     pays their churn regardless)

  INVERSIONS (same behaviour, control moved outward)
   3 origin: three hooks, not a struct field read inline
   4 MateriaArborFrons: six hooks for what silva does in the walker
   5 ambiguus OPTIONAL; silva's writer requires it
   6 reinserenda / lacunae as config, not hardcoded NIHIL
   7 fons_index opaque - no SilvaFons at all

  SIMPLIFICATIONS THE FORK ENABLED
   8 one emission path, not two (silva's FONS branch is radix==token)
   9 fragment opening moved INTO the exported seam, so silva's
     31-file bug is structurally unavailable to a frontend

ASSESSMENT OF THE PREMISE

Feasibility validated. Worth NOT yet validated, and the distinction
matters.

The byte-identical result is simultaneously the strongest and the
weakest evidence. Strongest: nothing was lost, and the seam suffices
for the hardest client that exists. Weakest: materia currently does
exactly what silva does. Phase 1 produced no new capability BY DESIGN,
so the entire payoff is deferred to clients that do not exist.

For: the decomposition is real — three independent measurements (M4's
include split, phase 0.1's `_parsura_` boundary, the expandere/parsare
census) converged on the same two modules, and that convergence was
found, not constructed. Nine hooks removed a preprocessor from ~3,200
lines of walker.

Against: the spec mislocated the coupling SIX times, every correction
in the direction of MORE coupling than predicted, in places the
measurement had not looked. Nothing fatal — but phases 2-5 estimates
deserve the same discount, especially `quaestio` (2,106 lines,
unmeasured, and the surface everything downstream uses).

The real payoff claim, stated so it can be tested: materia gives every
new language the arbor/STML projection for free, and per spec par. VIII
that projection is the novel half of this whole project (tree-sitter,
Roslyn and rowan all have the uniform-node model; none has a readable,
diffable, schema-gated serialization). If CSS gets it for the cost of a
recursive-descent parser, the fork paid. UNTESTED — and phase 3 is what
tests it.

RISK TO WATCH: materia is 5,256 lines heading for perhaps 9,000, and
the seam is nine hooks. If CSS and HTML each add two or three more, the
seam becomes the thing one must understand to use it — the PSI failure
mode M8 was written against. Each new hook deserves the question: does
this belong to the SUBSTRATE, or is it a language leaking through?

THE METHOD THAT WORKED, worth carrying into phase 3

  - silva as a SEPARATING ORACLE, never fixtures I wrote. A fixture
    carries my assumptions; silva carries what is true. It caught a
    missing raw marker, an unset hook that silently dropped attributes,
    and a fragment bug silva had already measured.
  - MINIMAL-THEN-DIAGNOSE, never port-then-hope. A `standard`-only
    frontend made the STML comparison name the two missing things
    precisely, turning 340 lines of blind porting into 340 against a
    list.
  - PLANT THE FAULT. Every gate this phase was made to fail on purpose
    before being believed.

AND THE RECURRING DEFECT CLASS, five instances in one phase: stale
objects (false green), mtime tie (false red), corpus silently emptied
by a flag, exit code blind to its own findings, empty-array expansion
under set -u. Not carelessness in any one. The lesson is that a GATE'S
OWN CORRECTNESS needs the same adversarial treatment as the code it
guards, and the cheap test is always "make it fail on purpose".

DECISION FOR WHAT FOLLOWS — see the next entry.


========================================================================
DECRETUM — ORDO RETARGETATUS: CSS ANTE FABRUM (Fran, 2026-08-27)
========================================================================

Spec par. X ordinem ponit: faber (II) -> CSS (III) -> HTML (IV) ->
silva (V). ORDO MUTATUR: **faber DIFFERTUR; CSS manu scribitur; faber
ex eo quod CSS revera poposcit ORITUR.**

Tres optiones ponderatae:
  A  faber prior, ut spec dicit
  B  silvam migrare prius (gelationem finire)
  C  fabrum omittere, CSS manu, fabrum ex usu oriri

CUR C. Tria:

 1. Faber constructores generaret quos NEMO ADHUC ADHIBUIT. Spec
    par. 5.5 ipsa contra generationem speculativam monet ('generate
    only what is UNIFORM'), et quod uniforme sit ex UNO cliente sciri
    non potest. CSS manu scriptum dicet quae pars revera mechanica
    fuerit; illa sola generanda est.

 2. Assertio pretii NONDUM PROBATA est - 'materia proiectionem
    arbor/STML omni linguae GRATIS dat'. C eo citissime pervenit.
    A moram unius phasis interponit ante rem quae totam furcam
    iustificat.

 3. B non urget. Periculum ordinis quod B tolleret ('C89 ultima venit
    et nihil dicit') porta phasis I sicut DESIGNATA est retiratum -
    CCCXXXVI plagulae per materiam eunt. Gelatio adhuc vilis est: una
    emendatio vitii replicata, trivialiter.

SIGNUM QUOD B EXCITET: si gelatio silvae carior fiat - emendationes
plures replicandae, aut opus in nucleo silvae revera necessarium -
tunc B ordo rectus fit. Signum NUNC nominatur ne postea inveniendum
sit.

QUID C POSCAT QUOD NONDUM ADEST. Probandum ANTE inceptum:
  - CSS registrum HAND-SCRIPTUM (T3 semitam generatam sumebat, quae
    T1 pendebat, quod RETIRATUM est)
  - descriptor lexicalis CSS: species x munus. CSS_LEX_SPATIA
    VERBATIM/SPATIUM est (spatia+tabulae+lineas novas absorbet), ergo
    munus LINEA DEEST - larva capacitatis id nominatim recusabit, quod
    RECTUM est, non defectus
  - constructores manu scripti (faber eos postea generabit)
  - quaestiones nodi (extensionem etc.) NON portatae sunt: an CSS eas
    poscat ANTE inceptum quaerendum est. Portae CSS (circuitus
    octetorum, circuitus STML, canon, selectores) eas fortasse non
    tangunt.

CONSILIUM CSS (css-arbor-plan.md) BIS RETARGETANDUM. Caput primum
T1/T4/T5/T6 iam notat; C addit: T3 semitam MANU-SCRIPTAM sumat, et
omnis mentio 'SilvaLexicon' (T4/T7/T14/T16) ad MateriaLexiconRatum
vertatur - cuius FORMA ALIA est (species x munus, non genus_eof +
trivia + numerus_triviorum).


========================================================================
PHASIS III INCEPTA — CSS: DESCRIPTOR + REGISTRUM (2026-08-27)
========================================================================

`./css/compile_probationes.sh`: 1/1, 189 assertions, exit 0.

THE UNMEASURED QUESTION, ANSWERED FIRST. CSS's nine gates (spec §9)
resolve to `materia_scribere_nodum`, `materia_arbor_scribere_nodum`,
`materia_arbor_legere` — all present. **The unported query families are
not needed.** One nuance: gate 3 says "comparator equal", and silva's
comparator (`silva_arbor_aequalis`, in the unported `arbor_aequalitas`)
compares TREES. Document-byte comparison substitutes for the gate but
is a different KIND of evidence; worth porting eventually as a second
oracle, does not block CSS.

A LATENT MATERIA BUG, FOUND BY WRITING THE DESCRIPTOR

The STML template compression writes `#@ante-spatia n="4"` for a
single-space trivia wrapper. The READER inverts it **by species**, and
only `REPETITUM` turns a count back into bytes. CSS's space genus is
`VERBATIM` — so CSS would have written a count and read back text, a
silently broken round trip.

`materia_arbor.c` now requires `species == REPETITUM`, not just the
right genus, before compressing. Found by writing a second language's
descriptor, NOT by a test: `templa_activa` is false for CSS, so the
fault was latent and would have detonated for whichever consumer first
turned templates on. **A second client is a kind of test the first
client cannot be.**

WHAT WAS BUILT
  css/fontes/css_lexicon.{h,c}     27 genera, species x munus
  css/fontes/css_registrum.{h,c}   10 genera, 24 slots
  css/probationes/probatio_css_registrum.c   189 assertions
  css/compile_probationes.sh       consumes materia; 0/1/2 contract
  css/CLAUDE.md

TWO CONSEQUENCES OF HAND-WRITING, both good:
 - the genus enum can have a TYPEDEF and a COUNT, which silva's
   generator-emitted anonymous enum could not
 - the five structural genera are simply absent; `ambiguus` is optional
   in materia and recursive descent produces no ambiguity. Robustness
   is spec-defined (`regula-mala`, `declaratio-mala`), not a generic
   error node

DRIFT GUARDS, PLANTED AND SEEN TO FIRE. A hand-written table's real
failure is a silent one, so both are asserted and both were verified by
planting:
 - lexicon order vs `CssLexemaGenus`, BY TITLE — swapped COLON/SEMICOLON
   fails 2 assertions
 - registry `loci_offset` CONTIGUOUS and exactly filling the loci table
   — one-slot shift fails. This is where a hand-written registry
   actually breaks, and the break is silent: a node reads someone
   else's slot layout

NEXT: the parser (plan T7-T12). Deliberately NOT started here — it
depends on the plan and materia's headers, both readable cold, whereas
the descriptor/registry depended on knowing materia's constraints from
having just built them. Fresh context spent where only fresh context
was cheap.
