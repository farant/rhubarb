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

========================================================================
PHASIS III — CSS: ADAPTATOR + LIGATOR TRIVIORUM. RELATIO (2026-08-27)
========================================================================

`./css/compile_probationes.sh`: 2/2, 213 + 189 assertiones, exitus 0.
materia 5/5, silva 50/50.

CONSILII T8 SIGNATURA AGI NON POTEST, ET RATIO STRUCTURALIS EST

T8 unam functionem bimodalem petit —
`css_adaptare(piscina, lexemata, b32 spatia_ut_contentum)` — quae
trivia aut absorbeat aut non, semel pro plagula tota. Illa forma
REGULAM D7 IMPLERE NON POTEST:

  spec par. VI spatia intra praeludia et valores CONTENTUM facit,
  ALIBI trivia. Quae regio praeludium sit CONTEXTU PARSURAE constat.
  Modo globali 'non-verbatim', spatium in `a b { }` iam trivium
  factum esset antequam parsator praeludium agnosceret — ergo in
  nodum `lexema-servatum` promoveri non posset.

Alter modus — bis adaptare, semel structurae, iterum praeludiis —
DUO lexemata materiae pro octetis IISDEM pareret, et invariatum
'domini unius' conventio fieret, non structura.

Consilium ipsum viam alteram nominat (T11 gradus III: "or
equivalently do not absorb trivia while inside a value"). Ea sumpta
est, et fortior quam alternativa:

  css_adaptare      UNUM PRO UNO, nihil absorbet
  CssLigator        status inter significantia; parsator eum tenet
                    quia SOLUS parsator scit quando spatium
                    contentum sit

'Dominus unus' non asseritur sed AEDIFICATUR: octetus bis converti
non potest quia bis conversus non est.

REGULA DIVISIONIS: PROSA CONSILII ET PROBATIO CONSILII DISSENTIUNT,
ET PROBATIO RECTA EST

T8 regulam prosa describit ("...NISI eodem in versu post lexema
praecedens stet ET NULLUM LEXEMA SIGNIFICANS SEQUATUR"), sed eadem
tabula `t0->spatia_post` NON-NIHIL pro `a { color : red }` asserit —
ubi `{` sequitur, ergo prosa 'ante' postularet. Contradictio.

Mensuratum in silva_lexema.c:950-1000 potius quam ex prosa sumptum.
Regula VERA: divisio ad primam NOVA_LINEA; quae ante eam INCLUSIVE
stant priori ut 'post', cetera sequenti ut 'ante'; SINE linea nova
OMNIA priori. Ergo probatio consilii recta, prosa laxa.

Notandum generaliter: cum consilium regulam BIS dicat — prosa et
probatione — et dissentiant, CODEX ORIGINALIS arbiter est, non
utravis.

DUAE DIVERGENTIAE CONSULTAE A SILVA, quia CSS lexema NOVA_LINEA
NON HABET (unum genus SPATIA lineas novas INTRA se absorbet):

  I. Silva lineam novam ipsam PRIORI dat (divisio = j + I); nos
     totum lexema SEQUENTI (divisio = j). Scindere non possumus,
     ergo eligendum — et indentatio lexematis sequentis pars maior
     et legibilior est quam terminator qui nihil legit.

 II. Divisor munus SPATIUM ferre DEBET. Sine ea condicione
     commentarium MULTILINEUM divisor fieret, et `a{} /* n\np */ b{}`
     commentarium ad `b` migraret. Silva id non facit
     (COMMENTUM_CLAUSUM lexema NOVA_LINEA non est), et lectio
     falsa esset.

Utraque per vitium plantatum probata (2 assertiones utraque).

VEXILLUM initium_lineae NON PONITUR, ET DISCREPANTIA OCTETIM
INVISIBILIS ESSET

CSS munus LINEA caret, ergo signum illud lingua dicere non potest.
Sed maius est: ETIAM SI PONERETUR, CIRCUITUM NON SUPERARET.
materia_arbor.c:908 attributa sedis (b, linea, columna,
linea-initium) SOLUM pro lexemate DERIVATO scribit
(`si (!sedes.est_fons)`). CSS derivationem non habet, ergo omne
lexema fons est, ergo documentum 'linea-initium' NUMQUAM fert;
lector eam ex cursore reficit (materia_arbor.c:2305), et cursor per
munus LINEA SOLUM movetur.

Discrepantia ergo scriptura->lectio->scriptura OCTETIM IDEM
redderet. Solum COMPARATOR ARBORUM eam videret.

Id EXEMPLUM CONCRETUM est eius quod phasis III initium abstracte
notavit: comparatio documentorum pro porta III substituit sed
GENUS EVIDENTIAE ALIUD est. Hic casus primus qui differentiam
ostendit, et argumentum pro `arbor_aequalitas` portanda quod
antea coniectura sola erat.

PORTA TEGUMENTI OCTETORUM (spec par. IX.7) ANTE TEMPUS

Consilium eam T10 ponit, post parsatorem. Hic advenit ANTE eum
quia iam agi POTEST: adaptatio et ligatio sunt ubi octetus perdi
potest; parsator eas solum ADHIBET. Vitium hic inventum vitium
UNIUS moduli est; idem post parsatorem inventum inter duos sederet.

Currit XVII casus inlineos (NUL inclusus, CRLF, \f, omnia trunca)
et V plagulas VERAS (fixa/css tres + speculum.css + style.css,
~XXIV milia octetorum), UTROQUE REGIMINE — structurali et contenti.
Regimen contenti probat suturam D7 EXPRIMI POSSE, non solum
describi.

Corpus SUUM mensurat: `lectae > II` assertum, quia porta quae
corpus suum non numerat 'N/N mundum' dicere potest cum NIHIL
legerit.

VITIUM PLANTATUM QUOD NON ARSIT, ET QUID MENSURATIO INVENIT

Quattuor vitia plantata; TRIA statim arserunt. Quartum —
`xar_obtinere` ut initium seriei CONTIGUAE tractare, dum Xar
SEGMENTATUS sit (xar.h:59) — NIHIL fregit, etiam casu triviorum
XXIII quae segmentum primum (XVI elementa) TRANSEUNT.

Non coniectatum sed MENSURATUM per probam:

  segmenta=3  primum=0x128008240  secundum=0x1280082c0  delta=128
  (XVI * VIII = 128)

Segmentum secundum IMMEDIATE post primum in piscina iacet, ergo
lectio ultra limitem in segmentum secundum CASU cadit et rectum
valorem reddit. FORTUNA, NON CONTRACTUS — eadem forma quam
materia_token.h de ordinatione notat ("luck, not contract").

Remedium probationis: probatio contractus quae inter cumulationes
ALIUD ex eadem piscina allocat, ut segmenta DISIUNGANTUR. Tunc
vitium plantatum ARDET. Contractus 'Xar contiguus non est' nunc
PROBATUS, non solum observatus.

Generalisandum: cum vitium plantatum non ardeat, responsum non est
'ergo codex ille otiosus est'. Quaerendum quid casu rectum
faciat — et si fortuna sit, PROBATIO fortunam removere debet.
Vitium plantatum quod non ardet est mensura de PROBATIONE, non de
codice.

VITIA PLANTATA, omnia ARSA
  I   Xar ut series contigua        1 assertio  (probatio contractus)
  II  divisio = j + I               2           (scissio vera)
  III divisor sine munere SPATIUM   2           (comment. multilineum)
  IV  trivium ultimum omissum      20           (porta tegumenti)

QUOD AEDIFICATUM EST
  css/fontes/css_adaptare.{h,c}              121 + 223 lineae
  css/probationes/probatio_css_adaptare.c    213 assertiones

PROXIMUM: spina parsatoris (T9), deinde regulae qualificatae (T10).
Porta tegumenti iam stat, ergo T10 eam ADHIBEBIT non aedificabit —
et oraculum separans (par. IX.4) iam habet quod separet: si porta
STML postea cadat, haec dicet analyzatorem sanum esse.

========================================================================
PHASIS III — SPINA PLAGULAE (T9), ET CONTRACTUS ALBI EMENDATUS.
RELATIO (2026-08-28)
========================================================================

css 3/3, materia 5/5, silva 50/50, shim CCCXLVI + CCCXLV + CCCXLV.

Spina aedificata (css_arbor.{h,c}, css_arbor_parsare), et STATIM
vitium substrati invenit quod nulla probatio C89 invenire poterat.

QUOD SPINA INVENIT

Casus degeneres — plagula vacua, spatia sola, commentarium solum —
per portam octetorum transierunt et per STML CECIDERUNT:

    causa: 'valor trivii textui non tutus'

Mensuratum, non coniectatum:

  C89   SPATIA       " "     REPETITUM    SPATIUM
        TABULAE      "\t"    REPETITUM    SPATIUM
        NOVA_LINEA   "\n"    TERMINATOR   LINEA
        CONTINUATIO  "\\\n"  TERMINATOR   LAMINA
        COMMENTUM_*  NIHIL   VERBATIM     COMMENTUM
  CSS   SPATIA       NIHIL   VERBATIM     SPATIUM   <- sola combinatio

Ergo PLAGULA CSS QUAELIBET SPATIUM FERENS proici non poterat, id
est CSS verum totum. C89 id numquam tangit quia genera eius spatii
REPETITUM aut TERMINATOR sunt; genera VERBATIM eius sola commenta
sunt, quae contentum non-album ferunt.

QUAESTIO FRAN QUAE FURCAM DISSOLVIT

Tres vias proposui — spatia CSS scindere, contractum STML laxare,
speciem novam addere — et Fran quaesivit: "an '<tag!> </tag>'
temptavimus?"

Illa quaestio furcam DESIGNI in MENSURATIONEM vertit. Proba stml
directa (scratchpad, non ratiocinatio):

  <lex-spatia!>\n\n  </lex-spatia>   pulchrum   -> "\n\n  " IDEM
  <lex-spatia>\n\n  </>              pulchrum   -> TEXTUS PERDITUS
  nidificatum, pulcher indentationem REVERA generans:
     liberum crudum   -> "\n\n  " servatum
     liberum non-crudum -> perditum

Ergo contractus RECTUS EST SUBSTANTIA, NIMIS LATUS AMBITU. Pulcher
dispositionem possidet EXTRA crudum; intra crudum octeti verbatim
sunt, et materia_arbor.c ipsa id iam dicebat duabus lineis infra
refutationem: "pulcher crudum numquam tangit".

CUSTODIA CONDICIONIS QUAE ACCIDERE NON POTEST

Semita triviorum sic stabat:

    si (!_textus_tutus(valor)) refutare;      <- album refutat
    ... nodum textus addere ...
    _valorem_crudum_notare(...)               <- elementum CRUDUM fit

Refutatio custodiebat ambiguitatem quam sententia proxima
REMOVEBAT. Et si notatio cruda fallat (valor sequentiam claudentem
fert) scriptor NIHIL reddit — ergo textus albus sine cruditate
numquam emittitur. Custodia mera erat.

DUAE PARTES CONGRUENTES, AMBAE NIMIS LATAE

Scriptor album refutabat; lector album praeteribat. Commentarium
lectoris id ut virtutem nominabat: "contractus unus per duas
partes". Congruebant — et AMBAE nimis latae erant.

CONGRUENTIA PARTIUM VERITATEM NON PROBAT. Duo dimidia inter se
consentientia speciem principii ferunt; hic species sola erat, et
species ipsa est quod inquisitionem per menses distulit. Genus
proprium: cum duae partes se mutuo citent ut rationem, neutra
rationem habet.

EMENDATIO, in materia (non in CSS)

  _nul_fert         quaestio NOVA, seorsum: NUL numquam
                    repraesentabilis, crudus an non
  scriptor triviorum  NUL SOLUM refutat; cruditas album licet
  lector            album praeterit NISI 'elementum->crudus'

C89 INTACTUS: shim CCCXLV/CCCXLV octetim, quod praedictum erat
(C89 spatium VERBATIM non habet) et nunc MENSURATUM est.

VITIA PLANTATA, omnia ARSA
  I   lector album inconditionaliter praeterit   4 assertiones
  II  scriptor album iterum refutat              suite fracta
  III NUL non amplius refutatur                  1 assertio
Utrumque dimidium ergo ONUS FERT, et NUL seorsum stat.

CONSTRAINTUM RELIQUUM, NOMINATUM ANTEQUAM MORDEAT

Semita LEXEMATIS (materia_arbor.c:945) crudum notat SOLUM cum
elementum textum SOLUM ferat — "elementum mixtum crudum esse NON
potest", quod verum est de STML, non incuria. Ergo lexema
significans cuius valor albus est ET quod trivia fert crudum esse
non potest, et valor eius adhuc perit.

Id CSS in regimine CONTENTI (D7) tangit: spatium ibi
'lexema-servatum' fit, et commentarium ei ut 'post' adhaerere
potest. T11 est, non T9 — sed nunc ANTE tempus notum, non post
casum.

QUOD AEDIFICATUM EST
  css/fontes/css_arbor.{h,c}              34 + 152 lineae
  css/fontes/css_registrum.h              enumerationes LOCORUM
  css/probationes/probatio_css_arbor.c    120 assertiones
  materia/fontes/materia_arbor.c          contractus albi
  materia/probationes/probatio_materia_arbor.c  16 assertiones novae

INCOMPLETUM NOMINATUM, non tacitum. Spina regulas non parsat, ergo
'a{}' octetos PERDIT. Probatio id ASSERIT (CREDO_FALSUM), ut porta
rubra esse DEBEAT donec T10 adveniat. Alternativa — lexemata in
'regula-mala' tacite congerere — portam VIRIDEM faceret dum nihil
parsatum esset. T10 has assertiones vertat.

LOCI NOMINATI: css_registrum.h nunc enumerationes indicum locorum
fert (CSS_DECL_VALOR etc.) et probatio eas contra TITULOS tabulae
retiret, OMNES XXIV. Sine ea probatione enumerationes tertia tabula
manu scripta essent quae tacite labi posset — et lapsus valorem in
locum alienum poneret sine querela.

PROXIMUM: T10 — regulae qualificatae et saepta. Porta tegumenti iam
stat (T8) et oraculum separans quoque (hic), ergo T10 eas ADHIBEBIT,
non aedificabit.

========================================================================
AUDITUM CONSILII ET OPERIS (2026-09-01) — post arcum stml, ante T10
========================================================================

Fran petiit: consilium et opus facta huc usque AUDIRE. Methodus:
quattuor documenta orientationis frigide relecta, PORTAE OMNES ad
HEAD recursae (prima verificatio plena post mutationes substrati
stml 2026-09-01), gelatio contra git probata, artificia generata
RE-DERIVATA et differentiata (numquam per mtime).

STATUS AD HEAD, MENSURATUS HODIE
  materia 5/5 · css 3/3 · silva 53/53 (post emendationem infra)
  shim: octeti 348/348 · STML 347/347 · circuitus bis 347/347
  Corpus shim CREVIT sponte (336->338 plagulae: stml_macros.c et
  stml_html.c, ambae hac hebdomade natae, ambae IDEM primo tactu).
  lexicon_c89 generatum RECENS: generator recursus, differentia
  nulla contra silvam post-gelationem.

DUO VITIA PORTARUM INVENTA ET EMENDATA (ambo extra materiam ipsam)

1. QUATTUOR suites silvae rubrae ad HEAD - causa UNA, substantia
   NULLA: pinnae corporis 155 in QUATTUOR plagulis, cum
   lib/stml_html.c plagula 156 facta sit. Sessio prior plagulam
   primam quam porta nominavit (canon_corpus) emendavit et stetit.
   Septem pinnae nunc 156; suites 1/1 + 3/3. Lectio: numerus
   pinctus est STATUS DUPLICATUS - cum movetur, numerum per
   probationes/ TOTAS quaere.

2. Porta shim NON CURRIBILIS ab 2026-08-31: obiecta aliena in
   silva/build (amalgama_verificatio.o = silva tota, 359 duplicata;
   instr_silva_formator.o) globum nexus fregerunt. Nemo eam inter
   08-28 et hodie cucurrit, ergo fractura INVISIBILIS erat.
   Exclusio ampliata (amalgama_verificatio + praefixum instr_).
   Familia vitiorum portae, membrum VI: index nexus globo pastus
   frangitur cum directorium commune obiectum novum accipit - et
   porta quae non curritur de statu suo NIHIL dicit.

INVENTA AUDITUS, DECISIONES FRANI EXSPECTANT

A. PORTA PHASIS I: verba specis (par. X) et memoriae adhuc dicunt
   "M1 subtree 281/281 per materiam, GO/NO-GO". Quod cucurrit est
   substitutum honestum sed ALIUD: latius (338 plagulae, octeti +
   STML + circuitus bis) et debilius GENERE (sine comparatore -
   arbor_aequalitas NON portata; sine subarboribus arbitrariis).
   RELATIO phasis I id candide dixit; spec et memoria non
   emendatae. T8 casum primum mensuravit quem soli octeti videre
   NON possunt (initium_lineae). Desideratum 01M1EZKKP0 conditum:
   comparatorem portare ANTE T14.

B. CONSILIUM CSS T10-T17: corpora taskorum STALA sub TRIBUS stratis
   deviationis - typi silvae (SilvaNodus, silva_scribere_nodum) in
   codice taskorum; gradus 1 taski X iam factus (porta tegumenti ad
   T8 ante tempus); T11 gradus 3 signaturam impossibilem primam
   nominat. css/CLAUDE.md monet, sed disciplina consilii-ut-fontis
   dicit valores exacti in corpore taski vivunt - et hi falsi sunt.
   Suadetur: T10-T17 re-scribere materia-nativa ANTE T10 (breve -
   octo taski), aut ex spec + css/CLAUDE.md exsequi, non ex plano.

C. GELATIO: 1373a3e8 (2026-08-31) 288 lineas ex silva_arbor.c
   sustulit (promotio matcher exemplarium) - REFACTIO, non fixum
   vitii, in zona gelata. Verificatum: extra subcopiam portatam,
   nulla replicatio debita. Sed iudicium illud NUSQUAM scriptum
   erat - habitus, non porta. Suadetur: omne commissum tangens
   silva/fontes lineam unam iudicii replicationis in hoc
   phase-log accipiat.

D. SPEC par. X dimidium posterius iam non describit quod aedificatum
   est: MateriaContextus (0.2) numquam exstitit - dissolutus in
   consilium + uncos frontis + campos config; schema 5 vocaminum
   facta est 7 (frons) + 3 (origo) = X unci; ambiguus optionalis
   facta est mutatione codicis, non stratificatione. Correctiones
   priores (2.2, 4.1, 0.1) in spec redierunt; posteriores in
   phase-log solo vivunt. Suadetur: nota "ut-aedificatum" in par. X
   quae ad phase-log ut testem operis monstrat.

QUOD TENUIT - et auditum id dicere debet

  M7 (tabula, numquam programma): ubique tenet. M8 (tenue): CSS
  uncos NULLOS addidit; ambae mutationes materiae a CSS actae
  (custodia speciei compressionis, contractus albi) praesumptiones
  C89 REMOVERUNT, non commoda CSS addiderunt - cliens secundus iam
  bis solvit ut probatio quam cliens primus esse non potest.
  Disciplina assertionis rubrae (spina T9 incompleta ASSERITUR);
  custodes derivae tabularum manu scriptarum plantati et arsi;
  documenta CLAUDE.md ambo VERA et profunda - melior fons quam
  spec ipsa pro statu currenti.

========================================================================
B1 — COMPARATOR ARBORUM PORTATUS. RELATIO (2026-09-01)
========================================================================

materia 6/6 (nova probatio 46 assertiones) · css 3/3 · shim: octeti
347/347 + STML 347/347 + circuitus bis 347/347 + **ARBOR (comparator)
347/347**, exitus 0. Desideratum 01M1EZKKP0 clausum.

QUOD AEDIFICATUM EST
  materia/fontes/materia_arbor_aequalitas.c   portatio ex silva
    (dimidium ARBORIS solum; dimidium parsurae = frons C89, manet)
  materia/fontes/materia_arbor.h              contractus + declarationes
  materia/probationes/probatio_materia_aequalitas.c
  shim: oraculum arboris in circuitu (mnodus contra relectam,
    STRUCTURALIS), numeratores ARBOR_IDEM/DISPAR in exitum iuncti

MODUS FISSUS MENSURATUS, NON PRAESUMPTUS: FIDELITAS addit byte_offset,
linea, columna (et longitudo, quae in caudam abiit); OMNIA CETERA
utroque modo - series triviorum inclusa, et signum provenientiae
(offset < 0) consulto utroque. Pinctum in probatione ne portatio a
fonte divergat. Consequentia pro B9 confirmata: 'a>b' et 'a > b'
arbores selectorum aequales erunt quia folia spatii omissa numquam
intrant et offsets in STRUCTURALI non conferuntur.

EXCLUSA CONSULTO: standard, scissurae, longitudo - cauda frontis,
cuius formam materia non videt. Sedes descensus nominata: uncus
comparationis frontis, phasis V.

ORACULUM BIS ARSIT PRIMO TACTU - plantatio per ignem vivum satisfacta
(porta quae in defectibus VERIS bis rubuit plantatione synthetica non
eget; ambae causae campum et viam nominaverunt, quod instrumentum
probat):

 1. CCXXXIV plagulae: conversor shim patres NON copiabat. Lector
    materiae (_patres_figere) arborem totam parentat; arbor conversa
    'non commissa' videbatur. Emendatum: conversor parentat.

 2. LXXVIII plagulae (capita omnia - nodi AMBIGUI retenti), via
    0.3>2.1>54.0, genus 54 = ambiguus: speculando nullitatem silvae
    inventa est DIVERGENTIA POLITICAE VERA - commissio silvae
    bracchia ambigui non canonica SINE patre relinquit (artificium
    ambulationis spinae), materia arborem TOTAM parentat. Directio
    MENSURATA proba una plagula ([a:nihil b:pater]), non coniectata.

    Documentum patrem NON fert - reconstructio utrimque est, ergo
    quaestio non 'quis mentitur' sed 'quae politica'. Shim politicam
    materiae sumit (arbores commensurabiles); DECISIO VERA ad phasim
    V nominatur: silva in materiam migrans decernat utrum consumptores
    eius a nullitate patris in bracchiis pendeant. Nota in conversore.

METHODUS NOTANDA: divergentia 'nodus/pater-nullitas' bis apparuit
causis OPPOSITIS (primum a-latus sine patre ex defectu conversoris,
deinde a-latus sine patre ex artificio silvae). Titulus idem, causae
diversae - iudica per tally causarum ET directionem mensuratam,
numquam per titulum solum.

========================================================================
B2 — REGULAE QUALIFICATAE + REGIMEN CONTENTI. RELATIO (2026-09-01)
========================================================================

css 3/3 (pinnae T9 VERSAE - id ipsum quod promittebant) · materia 6/6.
Viridis PRIMO CURSU post implementationem.

QUOD AEDIFICATUM EST (css_arbor.c, ~370 lineae novae)
  _significans_proximum   regimen duplex: structurali trivia
                          cumulantur; contenti OMNE lexema per
                          solvere it (pendentia vacua = prior solum
                          ponitur) et significans redditur
  _valorem_componentem_consumere   L3 5.4.7: FUNCTIO -> functio,
                          apertum -> saeptum, cetera -> servatum
  _saeptum_consumere      L3 5.4.8: clausura CONGRUENS sola claudit;
                          FINIS claudit (totalitas), tok_clausum
                          absens manet quem scriptores omittunt
  _functionem_consumere   L3 5.4.9 (lexator 'ident(' iam fudit)
  _regulam_qualificatam_consumere  praeludium CONTENTUM usque '{',
                          corpus saeptum structuralis

REGIMEN CONTENTI (D7 AUCTUM) EXSECUTUM: intra praeludia spatia ET
commentaria folia servata sunt, ligator praeteritur, trivia intra
regiones contenti NON exsistunt. Ratio triplex in capite plagulae
(ordo octetorum / constrictum T9 numquam ardet / modellum L3).
Mechanismi cardo MENSURATUS ante usum: css_ligator_solvere cum
pendentibus vacuis prior solum ponit (css_adaptare.c:162), ergo
omne lexema contentum per eam ire potest et prior trans limitem
regiminum numquam obsolescit - ordo octetorum ab hoc pendet.

FINIS intra regulam: campus p->finis in statu parsurae; regula cum
corpore absente redditur (octeti servati). TOTALITAS interim, non
forma finalis - B4 recuperationem (regula-mala) adfert, ut '}'
supremum et CDO/CDC.

VITIUM PLANTATUM ARSIT: tok_clausum omissum -> QUINQUE casus
tegumenti rubri (discrepantia mensurae). Restitutum, 3/3.

NOTA FORMATORIS: linea T9 quam imitabar ipsa divergens erat -
imitatio vicinorum divergentias ADDIT in plagula cum baseline.
Emendationes meae plagulam 23 -> 17 duxerunt (delta mea zero,
sex T9 gratis). Regula manet: metire baseline, adde zephyrum.

PROXIMUM: B3 - declarationes, praevalentia, valores contenti
(mechanismus iam stat; B3 eum solum ADHIBET intra corpora).

========================================================================
B3 — DECLARATIONES + PRAEVALENTIA. RELATIO (2026-09-01)
========================================================================

css 3/3 (155 assertiones in probatione arboris) · viridis primo cursu
completo post implementationem (uncus examinis duos adiutores
nondum scriptos in ipso limine cepit - _folia_appendere,
_malam_consumere_finita - ante ullum cursum).

QUOD AEDIFICATUM EST
  _corpus_regulae_consumere  L3 5.4.4: declarationes; ';' errantia
                             folia servata fiunt (L3 abicit, nos
                             servamus - sine iactura octetorum)
  _declarationem_consumere   L3 5.4.5: titulus, ':', valor CONTENTUS
                             usque ';'/'}'(retro)/FINIS; terminator
                             absens licet (scriptores omittunt)
  _malam_consumere(+_finita) L3 5.4.6: lexemata CRUDA cum
                             profunditate saeptorum (FUNCTIO quoque
                             aperit); ';' inclusum, '}' retrocessum
  _retrocedere               index retro; re-solvere innocuum

DECISIO DESIGNI: PRAEVALENTIA SYNTAXIS EST, NON VALOR. '!' et
'important' loci TOKEN sunt, ergo trivia STRUCTURALIA ferre possunt
quae folia contenta ferre non possunt: spatium medium
('! important') ante lexematis 'important' fit, spatium sequens
post eius - MANU adfixa (materia_token_trivia_*_ponere), sine
ligatore, quia regio contenti pendentia numquam habet. Confirmatio
ANTE constructionem: nisi post 'important' statim ';'/'}'/FINIS
(spatio licito) sequatur, omnia folia contenta ordinaria fiunt -
ordo octetorum utroque tramite servatur quia NIHIL transponitur.
Probatio pinxit: verbum->numerus_ante == I (spatium medium domum
habet) - id ipsum quod vitium plantatum fregit.

VITIUM PLANTATUM ARSIT: adfixio spatii medii omissa -> porta
octetorum ("! important" XXV) ET pinna structurae (numerus_ante)
ambae rubrae. Duo oracula, unum vitium, ambo arserunt.

PROXIMUM: B4 - regulae apud (tabula trium classium), CDO/CDC,
'}' supremum, recuperatio regula-mala.

========================================================================
B4 — REGULAE APUD + RECUPERATIO: L3 TOTALIS. RELATIO (2026-09-01)
========================================================================

css 3/3 (188 assertiones in probatione arboris). Parsator nunc
TOTALIS est: octeti quilibet intrant, arbor exit, numquam cadit.

QUOD AEDIFICATUM EST
  APUD_TABULA + _apud_classis   tres classes (spec par. IV): corpus
                                REGULIS (@media...) / DECLARATIONIBUS
                                (@font-face, @page, @top-left...) /
                                SENTENTIA (@import...); ignota ->
                                saeptum genericum. Crescere = ordinem
                                docere, numquam re-parsare. Congruentia
                                ASCII sine casu (L3).
  _regulam_apud_consumere       L3 5.4.2: praeludium CONTENTUM usque
                                ';' (terminator) / '{' (corpus per
                                classem) / '}' patris (retro) / FINIS
  _corpus_regularum_consumere   regulae nidificatae intra @media;
                                apud intra apud; CDO/CDC et ';' folia
  _corpus_regulae_consumere     + apud nidificata (@page{@top-left{}})
  suprema: CDO/CDC              folia servata (legalia, L3 ignorat,
                                nos servamus)
  suprema: '}'                  regula-mala UNI-lexematis (schema
                                recentius; CR vetus praeludio dabat -
                                divergentia NOMINATA in commento).
                                Error localis manet: '}a{}' = mala
                                deinde regula SANA.

MENSURA NOTANDA: ante implementationem QUATTUOR ex quinque
probationibus octetorum recuperationis IAM virides erant per viam
genericam B2/B3 ('}a{}', 'a{', 'a{color}', 'a{b:"…') - totalitas
per constructionem, non per casus speciales. Sola structura eas
distinguebat, et probatio structurae rubra CECIDIT (segfault in
arbore formae falsae) - rubor clamans, non tacens.

VITIUM PLANTATUM ARSIT: terminator '@import' omissus -> porta
octetorum (XIX) ET pinna structurae ambae rubrae.

PROXIMUM: B5 - porta corporis (VI plagulae reales + adversarius_2).

========================================================================
B5 — PORTA CORPORIS. RELATIO (2026-09-01)
========================================================================

css 4/4 (porta corporis nova: 24 assertiones). VI plagulae verae -
componentia 11517 + thema 2731 + adversarius 277 + adversarius_2
151 (novus: '}' supremum, CDO/CDC, CRLF mixtum, nidificatio
profunda, identificatores non-ASCII, '!' nudum, important cum
commento interrupto, '@' ad FINEM ipsum) + speculum.css 7680 +
style.css 1743 = 24099 octeti, OMNES octetim exacti.

PRAEDICTUM STANS 'corpus > fixturae' hac vice VACUUM cecidit:
corpus NIHIL invenit quod probationes unitatis non tetigerunt -
primum in historia portarum huius arcus. Non fortuna: totalitas
per constructionem (B2-B4 machinam generalem primum struxerunt;
B4 iam mensuravit IV/V probationes recuperationis ante
implementationem virides).

SED VIRIDIS INTERROGATA, non celebrata (lex domus: quid oraculum
IGNOSCIT?): oraculum octetorum parsatori qui omnia in 'mala'
congereret adhuc faveret. Ergo porta structuram quoque asserit:
plagulae MUNDAE quattuor (non adversariae) nullas malas et
declarationes veras ferre DEBENT - viridis vacua fieri non potest.
Vitium plantatum (ambulatio listas omittens) IV assertiones
fregit: numerator vivus, non vacuus.

PROXIMUM: B6 - circuitus STML super corpus, comparator B1 ut
oraculum secundum (classis quam octeti videre non possunt:
migratio dominii triviorum).

========================================================================
B6 — CIRCUITUS STML + ORACULUM DUPLEX; '\r' IN SUBSTRATO. RELATIO
(2026-09-01)
========================================================================

css 5/5 (probatio stml nova: 29 assertiones - XI casus inlinei +
discrimen oraculorum + corpus VI plagularum, circuitus BIS +
comparator + lectio->emissio). materia 6/6 · shim 348/348 (tres
oraculi). Porta paritatis CSS ad HEAD tota.

TRIA INVENTA, gradibus:

 1. PATER ITERUM (praedictum ab audito): parsator CSS patres non
    ponebat, lector omnes ponit - comparator primo casu inlineo
    arsit (nodus/pater-nullitas @ 0.0). Remedium SUBSTRATI:
    materia_arbor_patres_figere EXPORTATA (ambulatio interna
    lectoris publica facta), parsator CSS eam vocat. Idem remedium
    quod conversor shim (B1) - politica reconstructionis UNA.

 2. PROBATIO IPSA FALSA: divisio sine linea nova omnia PRIORI dat,
    ergo spatium caudale 'a{} ' ad '}' ligatur, non ad caudam -
    versio prima probationis contrarium credidit. Mensura vicit
    praesumptionem; probatio castigata directionem migrat inversam.

 3. '\r' IN TEXTU STML NON VIVIT - INVENTUM MAIUS. Duae plagulae
    adversariae (CRLF ferentes) circuitum documentorum I/II
    frangebant (delta II et I octeti). MENSURATUM per probam stml
    directam: 'a\r\nb' crudum ut 'a\nb' redit - lector STML CRLF
    ad LF normat, CRUDUS AN NON. Silva id sciebat pro TERMINATORE
    suo (attributum booleanum 'crlf'); generibus VERBATIM positio
    QUAEQUE opus est. REMEDIUM GENERALE in substrato: attributum
    'cr' (offsets decimales in valore vero), textus SINE '\r'
    scriptus, lector reinserit (_cr_exuere/_cr_induere, semitae
    triviorum ET lexematum, scriptor ET lector). Attributum abest =
    pretium nullum.

    VITIUM LATENS SILVAE PAR NOMINATUM: commentum C89 CR ferens
    circuitum STML silvae ipsius corrumperet - corpus id numquam
    continuit (348/348 shim viridis id probat). Materia nunc
    tractat quod silva tacite nequit; replicatio ad phasim V.

DISCRIMEN ORACULORUM MENSURATUM, non assertum: trivium ex '}'.post
in cauda.ante motum - octeti IDEM (memcmp), arbor DISPAR
(comparator). Classis tota ob quam B1 portatus est, nunc pinna
stans in probatione.

CLIENS SECUNDUS ITERUM SOLVIT: quartum vitium substrati a CSS
inventum (compressio-species T3, contractus albi T9, semita
lexematis B6-initio, normatio CR B6) - quodque C89 structuraliter
tangere non poterat.

PROXIMUM: B7 (canon manu scriptum) sessione RECENTI - vide
consilium B.

========================================================================
DEBRIEF INSTRUMENTORUM SESSIONIS (B1-B6, 2026-09-01)
========================================================================

ADHIBITA: formator (delta-metiendi disciplina per git stash, omni
tactu); examen post-editionem (uncus); comparator B1 ut oraculum
novum; proba stml directa (lex CRLF mensurata uno mandato);
probator scratchpad contra css/build (vitium B6 in documenta
diagnosi); shim tres-oraculorum; tabularium (desiderata).

FRUCTUS: examen adiutores nondum scriptos IN LIMINE cepit (bis);
formator-delta baseline-metire-adde-zephyrum bene functum est -
plagulae etiam MELIORES relictae (css_arbor 23->17, materia_arbor
462->455); proba stml directa quaestionem designi in mensuram
vertit (iterum - ars Frani T9).

ASPERITATES: (1) indices modulorum manu tenti TER momorderunt ->
desideratum 01M1F8M5XY. (2) Alineatio formatoris in plagulis cum
baseline magno (materia_arbor 462) laboriosa manu - anchorae
glomerum se movent dum emendo; forte formator '-scribere -intra
functio' (ambitus functionis unius) mereatur - non filatum, semel
tantum passus. (3) comm super indices linearum mutatos fallax -
tally classium solus fidelis.

DESIDERATA: 01M1F8M5XY (indices ex aedilis).

========================================================================
RELATIO - ROBUSTITAS CSS: FINIS HAERENS + PORTA TOTALITATIS (2026-09-01)
========================================================================

ORTUS: quaestio Frani 'quantum materia de tractatione erroris more
tree-sitter praestat?' in sondam versa (scratchpad, ~XL lineae:
parsare -> materia_scribere_nodum -> memcmp, processus singulus per
plagulam). Responsum mensuratum, non aestimatum: materia dimidium
contractus (emissio octetim) STRUCTURALITER tenet - ubi arbor
redditur, IDEM semper, etiam super octetos fortuitos; dimidium
alterum ('semper arborem') parsatoris est, et css_arbor id NON
praestabat, dupliciter:

  (1) 01M1FACW - 'a{b:(}' NIHIL. Consumens interior (saeptum /
      functio) FINEM videbat, p->finis ponebat, frangebat; exterior
      lexema aliud petebat, fluxus exhaustus, NIHIL usque ad
      radicem. Effectus: octeti fortuiti ~L% NIHIL, corpus mutatum
      II/IV, truncationes II/X. Portae IV numquam viderunt: corpus
      bene formatum; probatio B4 'non terminata' ad gradum regulae
      solum. REMEDIUM: FINIS HAERENS in _significans_proximum -
      semel visus (munus FINIS), omni petenti iterum redditur;
      ligator eum semel solvit. Lineae VI.
  (2) 01M1FAD8 - SIGSEGV sub nidificatione (parentheses clausae
      XX-XXX milia, @media{ ~XII milia). In PARSATORE, non in
      emissore (signum 'parsatum' + SOLUM_PARSARE: emissio x20000
      IDEM). Via mala iterativa (a{a{ x1,000,000 = IDEM 2MB) -
      forma iterativa probata. SILVA EANDEM CLASSEM habet (x20000
      SIGNUM 11). NON remediatum - decisio politicae profunditatis
      omnibus clientibus semel facienda (custos -> folia plana).
      PINNA in porta: CREDO_RUIT_CUM(..., SIGSEGV) x50000 - rubet
      cum remedium veniat.

PORTA SEXTA NATA: probatio_css_totalitas - generatores
linguae-agnostici (LCG seminatus, mutatio I/XL, truncatio XXIII
gradus, nidificatio VI formae x {clausa,aperta} x {I,X,C,M}),
assertiones tres (CREDO_NON_RUIT furca; non-NIHIL; memcmp), fons
peccans in css/build/totalitas_fractum.css scriptus. Casus CCCXLII.
RUBRA ante remedium (octeti fortuiti X/XXXII NIHIL), viridis post -
porta natalis quae defectum natalem cepisset. Semen desiderati
01M1FAE2 (harmonium transversale materiae: parsator + registrum ->
eadem porta; css nunc, html phasi IV, silva per shim phasi V).

SILVA SUB EISDEM FORMIS (arbor.sh): non clausum ad FINEM -> nodus
erroris, arbor completa (quod css non faciebat); octeti fortuiti ->
parsatio sana, scriptor STML clare recusat (NUL non textus).
CONTRACTUS DUO DISTINGUENDI: circuitus octetim totalis esse potest;
circuitus STML 'totalis SALVO NUL' - recusatio clara recta est.

LEX: caput quod 'TOTALIS' clamat sine porta commentarium mendax
est - css_arbor.h clamabat, mendax erat. Instrumenta nova portam
a nativitate accipiunt; PARSATORES QUOQUE.

------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: sonda scratchpad contra css/build (link list runner);
credo furca (NON_RUIT / RUIT_CUM - primus usus in css/materia/silva:
antea 0 plagulae); censor (casus = macro latina - momordit, cepit);
formator delta (arbor 59 held per duas iterationes; harness
-scribere bis, residua manu); examen calidum (ACCIPE IV/IV);
silva/arbor.sh ut oraculum alterius linguae.
FRUCTUS: sonda XL linearum duos defectus uno cursu; signum gradus
(SOLUM_PARSARE) parsatorem ab emissore separavit uno cursu;
CREDO_RUIT_CUM ruinam notam in pinnam vertit sine remedio.
ASPERITATES: (1) 'casus' identificator - macro latina; censor
cepit sed post compilationem fractam, non ante. (2) formator
-scribere glomera assignationum ad columnam +I ordinat quam manus
aestimat (21 vs 22) - regula non documentata in nuntio; empirice
inventa. (3) formator -scribere parametros functionis additae POST
scripturam non tangit nisi iterum curratur - bis currendum.
DESIDERATA: nihil novum (01M1FAE2 iam apertum).

========================================================================
RELATIO B7 - css.canon MANU SCRIPTUM + CUSTOS DERIVAE (2026-09-01)
========================================================================

FACTUM: css/grammatica/css.canon (510 lineae, 64 regulae: involucrum
III, genera X, loci XXIV [intra=genus], lexemata XXVII), fragmenta
IV (#lexema-unum maximum=1, #lexemata-multa, #valores, #trivia).
Sigillum registri PINNATUM ut electio unius optionis (2354fe63).
Se ipsum contra canon.canon iudicat: VITIA 0. Porta:
probatio_css_canon (100 assertiones) = custos derivae
bidirectionalis + pinna sigilli + iudicium XVI documentorum
inlinearium et VI corporis.

DECISIONES:
- <arbor> radix, NON in canones.registrum: clavis radicis '<arbor>'
  documentis subarboris silvae quoque competeret (registrum id
  consulto omittit). Canon per viam oneratur (probatio) aut
  'canon_examen -canon'.
- Collisio nominis locus/genus ('praevalentia' utrumque): intra=
  eam solvit - iudex clavem adstrictam 'parens/nomen' prius
  quaerit, deinde globalem (lib/canon.c _elementum_quaerere_vi).
  Locus 'praevalentia intra=declaratio' liberum 'praevalentia'
  globale fert. Idem exemplar quo c89.canon 'corpus' tractat.
- Constrictiones locorum MANU ex css_arbor.c (quid parsator
  ponat), non species nudae: 'tok_colon' lex-colon solum, etc.
  Ubi parsator quodlibet ponit (tok servati, tokens malarum)
  fragmentum totum. 'contentum' saepti = UNIO trium regiminum
  (valores / corpus regulae / corpus regularum) - species-level
  ex necessitate: genus unum tribus sedibus.
- Attributa positionis lexematum ABSUNT (derivatis solis) - si
  appareant, 'attributum ignotum' clamet. 'cr' in VERBATIM omnibus.
- Custos derivae per STML canonis ipsius ambulat (stml_legere +
  attributum_capere), non per interna Canon* (CanonElementum in
  lib/canon.c opacum) - contractus publicus solus.
- Tag lexematis in probatione ITERUM mangulatur (regula duplicata
  consulto): scriptor aliter -> iudicium rubet; probatio aliter ->
  custos rubet. Ambo simul mentiri nequeunt.

FALSIFICATIO (IV plantae, omnes arserunt ubi debebant):
  I   regula 'functio' deleta -> custos (a) ET iudicium (elementum
      extra canonem <functio> 'an lex-functio?' x21)
  II  pinna 'deadbeef' -> custos (II) ET iudicium (valor generi
      attributi non congruit <arbor> 'registrum-sigillum' x22)
  III 'declaratio' ex contentum remotum -> iudicium solum
      (liberum hic non licet <contentum> 'declaratio' x52)
  IV  regula 'fantasma' addita -> custos reversus solum
Restitutio cmp idem. Viridis primo cursu vero - quod sine plantis
nihil mensurasset.

ASPERITATES: runner RADIX_FONTES bis manu (canon, deinde
similitudo quam canon.o poscit) - ictus IV desiderati 01M1F8M5XY.
chorda_ex_literis allocat (piscina) - prospectus sine cast
-Wcast-qual violat; comparatio literalis (_aequalis_literis)
recta via. Slice-replace pythonis structuram inter ancoras
devoravit - ancora angustior.

PROXIMUM: B8 (genera selectorum XI + loci + custodes rubri primum
+ canon extensus + sigillum re-pinnatum).

------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: dump_stml sonda (proiectio vera ante regulas - tally
elementorum/attributorum corporis: vocabularium mensuratum, non
aestimatum); canon_examen (auto-iudicium canonis); lib/canon.c
lectio pro ordine quaerendi intra=; formator -scribere bis;
censor (PURUS); tabularium ictus.
FRUCTUS: tally proiectionis vocabularium totum uno cursu dedit
(47 elementa, 7 attributa) - regulae ex mensura, non ex memoria;
plantae IV portam falsificaverunt priusquam viridi crederem.
ASPERITATES: supra. DESIDERATA: nihil novum.

========================================================================
RELATIO B8 - GENERA SELECTORUM XI IN REGISTRO (2026-09-01)
========================================================================

FACTUM: css_registrum.h/.c + XI genera (XXI), + XXI loci (XLV),
enumerationes locorum nominatae (CSS_COMPLEXUS_COMBINATOR etc.;
selector-malus CssLocusMali communicat). css.canon + XXXII regulae
(XCVI), fragmenta #selectores/#partes-compositi, pinna sigilli
2354fe63 -> fb55041d. Custodes RUBRI PRIMUM: registrum (XXI contra
X), canon (genera XI sine regula + sigillum motum - vivum ex ipsa
porta lectum et re-pinnatum). Deinde 7/7.

DECISIONES CANONIS (species-level, B9 falsificet): combinator =
lex-delim | lex-spatia; sinister/dexter = complexus | compositus
(nidificatio sinistrorsum); tok_nomen pseudo-classis = identificator
| functio ('not(' lexema unum); argumenta = selectores | servata
cruda; valor attributi = identificator | chorda; operator = lex-delim
PRIMUM ('^=' alterum non refertur); '::' colon primum solum. Ambo
'non refertur' ex natura arboris analyseos: praeludium octetos
omnes possidet, arbor selectorum nullam portam octetorum habet.

FORMATOR: probatio_css_arbor 59 -> 82 (rows LOCI_NOMINATI XXI
longae, ut vicinae XXIV quae iam divergunt) -> bilineares -> 59.
Imitatio vicinorum divergentium divergentias ADDIT - lex iterum.

PROXIMUM: B9 css_selector.

========================================================================
RELATIO B9 - css_selector: ARBOR ANALYSEOS SELECTORUM (2026-09-01)
========================================================================

FACTUM: css/fontes/css_selector.{h,c} (~640 ll.), probatio
(CCXXXIII assertiones: structura, aequalitas per comparatorem,
canon, corpus CLI praeludiorum). 8/8 suites.

DESIGNUM (spec par. V/VI + consilium B9): lexemata communicata,
NODI NUMQUAM - pater unus (materia_arbor_patres_figere in arbore
selectorum vocatur; nodus communis patrem plagulae raperet). Folia
servata NOVA super lexemata argumentorum crudorum. Combinator super
folia servata: spatium ipsum combinator nisi punctuato adiacet.
Nidificatio sinistrorsum; sinister/dexter absentes pro relativo /
caudali; combinatores consecutivi = complexus dextro absente.
Functiones selectorum (tabula VII) argumenta recursione per
introitum publicum parsant; ceterae cruda. Attributum ex saepto
'[': forma IDENT (op valor)? flag? - aliter malus ex saepto toto;
operator lexema PRIMUM ('^' de '^='). '::' colon primum.

LECTIO NATALIS: adiutor probationis fontem in buffer acervi
struebat - parsator octetos NON copiat (css_arbor.h dicit), ergo
lexemata post reditum in compagem mortuam spectabant. Genera
sana (in piscina), octeti mendaces: 'a b' viridis (genus solum),
'a>b' rubra (octeti), scriptor 'NUL fert'. VIII fracturae uno
vitio - octeti in piscina, 233/233. Lex: fons cuius arbor vivit
in piscina vivat; probatio quae genera sola inspicit octetos non
custodit.

CANON B8 FALSIFICATUS: CLI praeludia corporis + XI casus inlineares
contra css.canon 0 vitia - constrictiones species-level tenent
(nihil mutandum fuit). Planta: spatium numquam combinator -> rubra.

PROXIMUM: B10 (registratio runners, emendationes spec, worklog,
parcum S6, memoria).
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: dump_stml (forma praeludii vera ante grammaticam);
comparator ut oraculum aequalitatis; canon ut oraculum formae
(porta B7 re-usa in probatione nova); formator -scribere bis;
censor PURUS; planta una.
FRUCTUS: forma praeludii mensurata grammaticam uno cursu dedit
(functio nodus pro ':not(', saeptum pro '['); porta natalis
vitium adiutoris cepit ante quam modulum culparem.
ASPERITATES: (1) examen DOMESTICUM cepit quod compilator tacuit:
sentinella '-I' ex functione i32 (INSIGNATO domus) reddita, deinde
'i < ZEPHYRUM' semper falsum - remedium s32. Lex: sentinella
negativa functionem SIGNATAM poscit; examen hoc scit, clang non.
(2) formator -scribere lineas longas non frangit - LVI manu;
adiutores extracti (_est_operator_praefixus, _est_valor_attributi,
_est_functio, _est_saeptum_attributi, _pseudo_classem_creare)
codicem meliorem reliquerunt quam fractio nuda.

------------------------------------------------------------------------
REPLICATIO-VERDICTUM (silva/fontes tactum sub gelu, 2026-09-01):
silva_c89_semantica.{c,h} codices 89/90 — frons C89 (iudex), extra
subset portatum (materia = substratum arboris). NULLA replicatio
debita. Vide silva/phase-log eodem die.

========================================================================
PHASIS III — B10: REGISTRATIO, CLAUSURA SPEC, RELATIO (2026-09-02)
========================================================================

Consilium B clausum. Cursores in tabula portarum pythonicae: css,
materia, materia-shim (novus). Registratio shim eum FRACTUM invenit:
cursor silvae ab 2026-09-02 probationes duobus gradibus compilat,
probatio_*.o in silva/build relinquit, glob obiectorum shim ea
nectebat - 'principale' duplex, nexus fractus, nemo currebat. Lex
iterum (tertia vice in hoc arcu): cursor extra tabulam portarum est
porta mortua. Exclusio 'probatio_' addita; 348/348 tribus oraculis,
XV s.

Emendationes spec: css-arbor-spec par. 6 (D7 ut aedificatum, tres
causae), par. 8 (canon manu scriptum, custos derivae pro porta
recentiae); materia-spec par. 10 (nota ut-aedificatum: porta phasis
I forma substituta = shim, MateriaContextus in consilium + X hooks
dissolutus, comparator portatus, IV vitia substrati a cliente
secundo). Worklog css/fontes/css_arbor.worklog.md: quid portae
invenerint quod unitates non tetigerunt, ubi sutura a praedictione
discesserit, quae praedictiones par. 12 falsae fuerint (XI lineae =
pavimentum; canon et machina selectorum NON hereditatae).

PROXIMUM: decisio Frani - phasis V (C89 per materiam = probatio
acceptionis) aut phasis IV (HTML). S6 parcum manet donec HTML.
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: pythonica (Textus pro spec/md, PORTAE + porta pro
registratione, commissio_umbra photographica pro commissione);
tabularium (res S6, census); phase-log ipse ut fons relationis.
FRUCTUS: registratio = probatio cursoris: 'porta("materia-shim")'
uno vocamine fracturam diei ostendit quam nemo viderat; umbra
photographica editionem inter portas non impedit.
ASPERITATES: nulla nova; lectio vetus iterata (cursor non
registratus = mutus).
DESIDERATA: fumus qui cursorem quemque (*/compile_probationes.sh,
*_probare.sh, *_porta.sh) in tabula PORTAE esse poscit - tertia
occasio eiusdem classis.

## 2026-09-03 — cliens tertius (md): terminator in loco + crlf

Markdown (md/) carries its newline as a CONTENT token (munus LINEA,
species TERMINATOR) in slots, never as trivia. Two gaps surfaced and
were fixed in `materia_arbor.c`: the slot lexeme writer did not emit the
`crlf` flag for a TERMINATOR (only the trivia writer did), and the slot
reader assigned the plain orthography without consulting `crlf` (only
the trivia reader did). Both now mirror the trivia branches. Consumer
gate: `probatio_md_stml` (CRLF case, fidelity). Substrate gates after
the change: materia 6/6, css 9/9, shim 348/348. Also observed, not
changed: the raw text form's refusal of a value containing its own
closing tag is real for any client whose values can contain STML-shaped
text — md avoids it by splitting tokens at `</md-`.

## 2026-09-10 — referentiae: a node can name a node (fourth client, oratio)

Oratio is the first client whose own processing REORDERS lists while
sideways links exist: an umbra under a verb's reading names its filler
by two ordinals (word, reading), and every preferred-reading permutation
stales the reading ordinal — `_ligationes_remittere` remapped after
each permutation and bit twice (umbrae, then the alternatives added in
T32 b). The problem is general, not oratio's: md stores reference links
by label (name), C89 is nothing but identifier→declaration links, HTML
brings label-for and href anchors, and materia had already met the
shape once for TOKENS (a macro expansion's tokens reachable from two
places — the `#lexN` fragment + `<<#lexN>>` transclusion, "identity
would lie" otherwise). Decree 01M260XYXB (Fran, 2026-09-10) from
desideratum 01M24Z4Q5Y frictio (a).

BUILT (materia 6/6, three planted faults red on the named assertion):

- `MATERIA_LOCUS_REFERENTIA` species (appended LAST — registry seals
  hash species numbers) and `MATERIA_VALOR_REFERENTIA` value kind, a
  NEW kind rather than a flag on NODUS so every walk is taught its
  case: writer pass I notes the target and never descends; the byte
  writer emits nothing; positions and `patres_figere` skip it
  (`materia_nodus_liberi` never listed it); the comparator compares
  references by PATH RELATIVE to the comparison root (locus/index
  steps up the parent chain), naming `referentia/semita`,
  `referentia/extra` (target outside the compared subtree),
  `referentia/nihil`.
- SERIALIZATION is a NAME, not a transclusion. Transclusion is a window
  (canon judges through it, refuses cycles, fragment must precede use);
  a dependency graph has mutual arcs and forward references, so the
  target carries `id="nodN"` on its own element (STML's `#` document
  space, selector `#x`) and the locus prints `#nodN` as text. Anchors
  are numbered as first met in the document (target or referrer,
  whichever comes first) — serialization identity of the same standing
  as the `lexN` fragment names, not a locus and not a mirror. The
  writer REFUSES a reference whose target is outside the written
  subtree (`referentia ad nodum extra arborem scriptam`, with sedes) —
  the alternative is a `#nodN` without an anchor that the reader would
  refuse later, elsewhere, without a seat.
- The READER resolves in a pass after the whole tree is built (same
  shape as `patres_figere`): `id` attributes fill an anchor table
  (duplicate refused), reference loci are recorded pending, then
  resolved; unknown anchor = `referentia ad ancoram ignotam` with the
  line. Mirror attributes (oratio's derived ordinals) are ignored on
  read like `n`.
- PLACING VERBS FIX PARENTS: `ponere`/`appendere`/`reponere` set
  `pater` on the owned NODUS child (list elements too when a list is
  placed whole). Before this `pater` was "post-acceptum SOLUM";
  `oratio_clausula.c` poked the field by hand and oratio refixed the
  whole tree after annotation. The comparator's relative paths and the
  ordinal mirrors need parents at construction. One comparator test
  had encoded the old contract (unparented construction) and now
  forges the null side by hand.

MEASURED, not argued: the arbor probatio round-trips a mutual pair
(A→B forward, B→A back) twice byte-identical, reads back pointer
identity (A'.socius == B'), permutes the list and shows the anchor
following the node with no remap, refuses orphan/duplicate/unsigiled
anchors and the outside-subtree write, and accepts a self-reference
(minimal cycle). Plants: reader resolving to self → `_socium_filii(a2)
== b2` red; writer guard inverted → `s1.successus` red; comparator
dropping the list index → the sibling-target divergence red.

COST TO CLIENTS WITHOUT REFERENCES: ZERO, measured. The first cut
created the referendum and anchor tables on every write and read and
looked every written node up by pointer key (one arena allocation per
lookup, the `_nota_lexematis` pattern) — `allocationes_arboris` moved
+1.4 % on every css/md/oratio computus pin. The tables are now created
lazily (pass I on the first reference; the reader on the first `id` or
reference locus) and a writer with no referenda skips the lookup, so
all three computus goldens are byte-identical without regeneration. A
representational change may move nothing; when it does, make it not.

NEXT (oratio half): `impletio-vocabulum`/`impletio-analysis` on umbra
and `vocabulum`/`analysis` on alterna become one REFERENTIA locus each
with derived ordinal mirrors; executor/oracle/clausula dereference
pointers; `_ligationes_remittere` deleted; canon declares `id` on the
analysis genera; registry seal + probatio pin updated by hand (frictio
(b), paid once more). Acceptance: all nine treebank pins BYTE-IDENTICAL
— a representational change may move nothing.
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: legati caput (tabula_dispersa API sine grep); sample(1) on
the oracle for the cost split that re-ranked frictio (c); formator
-scribere/-vitia; vocabula -nova (NOVA 0); silva.planta ×3.
FRUCTUS: reading materia_arbor.c whole found the token identity
mechanism and its two blockers for nodes (before-use, cycles) — the
design fell out of the code, not the other way round.
ASPERITATES: (1) BSD grep `\s` silently no-op again (sample parsing);
(2) the pretty writer's capturing-fragment form (`<socius(> #nod2`)
is what strstr must match — dump the text before pinning a layout;
(3) a test that encodes a contract ("pater post-acceptum SOLUM") goes
red when the contract improves — read it as a contract change, not a
regression.

## 2026-09-11 — empty reference = written absence (oratio drop-out fix)

The 2026-09-10 writer refused a REFERENTIA locus whose target is NIHIL
("referentia nihil"), mirroring the old INDEX refusal of a negative
index. Oratio writes exactly that when a later resolution stage revokes
a claim an earlier stage already bound (loci are write-once, so the
revocation overwrites the filling with an empty reference). The refusal
failed the WHOLE sentence projection at the next stage; the executor
counted it in a census field nobody printed and returned success, so
the sentence silently skipped every later stage. Measured with a
temporary probe: Seneca 25, charters dev 43, Perseus 19 sentence-stage
projections dropped, every one "referentia nihil".

LAW: an empty reference is WRITTEN ABSENCE. The writer omits the locus
(same branch as an unwritten locus), the reader never produces one, and
the comparator normalizes an empty reference to NIHIL before comparing
kinds — so written-and-read trees stay equal, and "empty vs target"
now reports `locus/genus-valoris` like absence vs value (the
`referentia/nihil` divergence is gone). In memory the locus stays
written: a client can still tell a revoked claim from one never made
(oratio never refills a revoked umbra). Probationes: the arbor suite
writes an empty reference, finds no `<socius`, reads NIHIL back, and
the comparator calls them equal; the comparator suite checks empty
against unwritten both ways. Oratio pins the summed projection
failures to ZERO on every pinned treebank, so the class cannot hide
again.

## 2026-09-15 — registrum coctum: the genera table generated (oratio wish 01M24Z4Q5Y b)

INTENTIO: remove the hand arithmetic from every client's registry
(offsets, counts, the enum kept in step) without removing the gates.
Fran's constraints: C89, usable without Python, STML declaration.

RELATIO: `materia_coctor` (library: text in, two texts out, no files)
+ `materia/instrumenta/coctor.c` + `./materia/coquere.sh` +
`materia/grammatica/registrum.canon` (registered by root in
`canones.registrum`). Declaration `<registrum>` → genus enum with its
`_NUMERUS_GENERUM` and the baked `LOCI`/`GENERA`/struct; identifiers
derived from kebab titles; notes as comments above rows; output a
fixed point of the formator (CONFORMIS on the real oratio render).
Refusals are named with the declaration line (duplicate locus title
within a genus, unknown species, missing root attribute, non-kebab
title, foreign root, parse failure, no loci at all). Oratio migrated:
hand tables deleted, the header includes the generated one, the two
analysis-range aliases are `#define`s, the registrum probatio gained a
freshness block (render in-process, compare bytes with the committed
files). Acceptance: tables equal row for row by an independent script,
seal `c4752e42` unchanged (canon gate 271/271), registrum 1121/1121,
birth gate 64/64 with byte-exact fixtures derived by hand before the
emitter ran. Left hand-written on purpose: locus enums (oratio's
analysis genera share a common enum; typedef names are genitives) and
the pins (count, seal). Next: css and md, then a thin Python wrapper.
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: legati caput (stml, chorda_aedificator, canon, filum APIs
without grep); legati corpus (StmlNodus, StmlResultus); scribe for
every new C file (the examen caught a comment closed by " */" inside
its own prose, an anchor-looking `<sedes>` in a comment, and a chorda
handed to strstr); formator -scribere/-vitia; quaere.sh on the new
identifier words before staging; canon_examen through the registry.
FRUCTUS: the byte-exact fixture written by hand from the wrap rule
matched the emitter on the first run — writing the rule down first
made the test a spec, not a transcript.
ASPERITATES: (1) the examen's "nodi erroris 1" carries no line; the
"vocatio implicita" warnings on the next function located it; (2)
`-Wcast-qual` on a needless `(Xar*)` cast; (3) `credo_aperire` must
precede the first assertion (loud, so cheap); (4) an English field
name copied from `CanonVitium` (`detail`) would have met the lint —
renamed before staging.

ADDENDUM (same day, tranche two): css and md migrated; tables equal
row for row (45/21, 94/28), seals fb55041d and ba282b8a unchanged,
`css_registrum.c`/`md_registrum.c` deleted (empty TU is not C89).
Titles admit `_` (css `tok_nomen`); the genus companion is now
`<P>_GENUS_NUMERUS_GENERUM` in every client (css/md usages renamed).
The freshness check moved into the library
(`materia_registrum_recens`) — one call in all three registrum
probationes and in the instrument's compare mode; gate part VI
exercises it on files in `materia/build`. Plants: a comment edit in
each generated header went red at `rancor.recens`, green on revert.
`silva.registrum_coquere` is the thin Python face (pythonica law:
oratio declaration recens rc 0; bad species rc 2). Worklog has the
detail. Point (c) of the oratio wish (re-projection) stays open.

## 2026-09-15 — arbor tradita: the writer's STML tree handed to the client (wish c, step one)

INTENTIO: oratio re-projected every sentence to STML TEXT per stage
and parsed it back for the pattern machine. A spike (temporary
clocks, reverted; table in oratio_resolutio.worklog.md) put write +
projection parse at 34–43 % of resolution, rule parsing at 4 %, the
expansion at 52–60 %. The writer already builds the STML node tree
before serializing: the text was a detour.

RELATIO: `materia_arbor_proicere_nodum` returns the envelope element
the writer built (`MateriaArborScriptura.arbor`, new LAST field;
`scribere_nodum` fills it too and then serializes). Body shared
(`_arborem_struere`): same walk, ornaments, anchors, reference
guards. Law: `stml_scribere(proicere(x).arbor, pulchrum)` ==
`scribere_nodum(x).textus` byte for byte; plant (inner root instead
of envelope) red, green on revert. Oratio's stage loop takes the tree,
parses only the stage's rules, prepends the projection, expands as
before; INTERN LAW: projection and rules in the same intern or every
rule is mute (planted red in the resolutio gate). Rows of seven
treebanks byte-identical to the text path; Perseus oracle 7.1 → 5.6 s.
NEXT for (c): step two (rows name nodes by anchor, not ordinal); a
per-rule probe of the expansion before touching the projection's
shape (lever = leaner projection per stage).

## 2026-09-15 — VISIO: a filtered projection (proiectio-macrior plan, tranche b)

INTENTIO: the per-rule spike (oratio worklog) made the expansion cost
≈ rules × projection bytes with no hot rule; oratio's tranche a
(language gating) landed; the remaining lever is a smaller
projection. Materia's half: `MateriaArborConsilium.loci_admissi` — a
flat mask over the registry's loci (index = genus offset + locus;
NIHIL = everything, today's behaviour); the writer skips an omitted
locus on its walk, species-blind; the envelope is stamped
`visio="partialis"` + `omissi="genus/locus …"`; the reader REFUSES a
stamped envelope ("visio partialis: non arbor"). A view lives in
memory for a matcher, never as a document. Gate in the arbor
probatio; plant = stamp dropped. The client (oratio, tranche c)
derives the mask from its programme.
RELATIO: built the same day. `loci_admissi` (constans b32*, flat over
the registry) on the consilium; the writer's locus walk skips an
unadmitted index before it touches the locus; `_arborem_struere`
stamps `visio="partialis"` and `omissi="genus/locus …"` (titles from
the registry, space-separated) when a mask is present; the reader
refuses a `visio` envelope before it reads the grammatica. Gate in
`probatio_materia_arbor`: mask omitting the test node's only locus →
stamp present, omissi begins with the genus title, the inner root has
no children, the serialized view is shorter than the writer's bytes,
`materia_arbor_legere` returns NIHIL with a cause naming "visio"; mask
NIHIL → no stamp and bytes identical to the writer's. Plant: the
stamp attribute misnamed → red at the stamp assertion, green on
revert. Nothing in materia knows what a locus means: the mask is the
client's table (oratio derives it from its programme in tranche c).
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: temporary clock probe under an env var (reverted, never
committed); `-machina -decreta -lites` snapshots of seven treebanks as
the byte-identity oracle; silva.planta ×2; formator; examen.
FRUCTUS: measuring before designing retired a lever I had already
written down (rules parsed once, 4 %) and showed the detour for what
it was; the acceptance test was a diff, not a judgment.
ASPERITATES: the probe printed per sentence because the oracle calls
the resolver per sentence (1.8 MB of output; the last block carries
the totals) — a probe that prints at "the end of the run" must know
what the run is.

## 2026-09-15 — phase 4: the HTML client (html-arbor-spec §11, html-arbor-plan H1–H11)

INTENTIO: Fran's order of 2026-09-15 (decree 01M2KC03NG): oratio
paused, consumers resume — HTML, then JS, then silva (phase 5). The
HTML spec of 2026-08-27 was written against silva as the S6
evaluation instrument; §11 (the addendum) retargets it to materia
before a line of `html/` exists: materia's writer/reader/comparator
for silva's, the registry GENERATED from a declaration, the canon
hand-written, `documentum` = `liberi` + `cauda` (a source may have
zero or several top-level elements; a synthetic root is H4-forbidden),
the lexicon with trivia INSIDE TAGS ONLY (comments are content, a
stray `/` inside a tag is trivia by HTML5's own definition), an
ITERATIVE builder over an explicit stack, and the reservations
re-scoped: measured, the reader refuses an unknown ELEMENT
(`materia_arbor.c:3090/3170`) and IGNORES an unknown ATTRIBUTE (it
reads `id` and nothing else), so v1 declares nothing reserved and
refusal is automatic for elements, canon-borne for attributes. What
this client asks of the substrate, named before building: absent
slots as meaning at scale (every void and implied close), a content-
dominant tree with almost no trivia, whitespace-only VERBATIM text
values at every level (T9's fix exercised thousands of times), raw-
text regions with `</`-shaped bytes inside token values through the
STML raw form and its refusal, materia's recursive walkers under an
iterative parser (the depth crash 01M1FAD8 measured on the
substrate's side alone), and a consumer of a consumer (md's rendered
HTML through this parser). Gates: registrum, adaptare (byte
coverage), arbor, corpus (fourteen files, every one with `<script>`),
stml (two cycles + comparator), canon, reservatio, totalitas,
computus, md. The shim runs at the end as the phase-boundary audit
(it ran 392/392 in 16 s on 2026-09-15 before this phase began, through
registrum coctum + arbor tradita + visio).
H1 DONE 2026-09-15 (411850b8): declaration (10 genera, 18 loci) →
generated tables (canon-judged, compare-mode fresh), slot enums,
lexicon (22 rows), runner, registrum gate (212 assertions); plants:
lexicon rows swapped → red on the title comparison; one byte of the
generated header → red on `rancor.recens`. Pythonica `PORTAE['html']`
registered at birth (a runner outside the gate table is a dead gate).
RELATIO (2026-09-15, same day, eleven commits 411850b8 → 8e68cf8f):
H1 declaration + generated registry + lexicon + runner + registrum
gate; H2 adaptare + ligator + byte coverage; H3 the iterative builder
(explicit stack, three pending states); H4 the void and implied-close
tables; H5 the corpus (fourteen files, 162,775 bytes, byte-identical
first run) + `adversarius_2.html`; H6 the STML round trip (two cycles,
comparator, direct emission of the re-read tree) — SUBSTRATE LIMIT
FOUND: a token value containing its own `</lex-…>` is refused
("valor sequentiam claudentem fert"), no fallback (01M2KJ3PK2,
pinned); H7 the hand-written canon (53 rules, seal pinned, drift guard
both ways); H8 reservation (reader ignores unknown attributes —
pinned — refuses unknown elements; canon names the attribute) +
totality (919 cases; FOUND the only builder bug: a pending close
taking a later `>` after a mutated `</h3`; DEPTH MEASURED: parse
100k lives, `materia_scribere_nodum` dies at 50k, STML quadratic —
01M1FAD8 noted, pinned); H9 computus twin + instruments + pythonica;
H10 md's rendered HTML through html (seven inputs, whole chain;
sabaw's notation = one malum, pinned, md polish noted); H11 this.
AUDIT: shim 392/392 (16 s) before and after; html 10/10 · materia 7/7
· css 9/9 · md 14/14 · briar 16/16 · oratio 19/19. Substrate changes
forced by HTML: ZERO (css 4, oratio 5). The five §11.10 questions
answered in `html/fontes/html_arbor.worklog.md`.
------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: coquere.sh (registry generated first try, canon-judged);
scribe.sh for every new C file (examen at write time caught one
`strstr`-on-chorda and the `CCCC` numeral); silva.planta ×9 (one
needed `tolerans='verba'` — a comment anchor is one lexeme); silva.Editio
for every C edit; formator -scribere/-vitia before every commit
(seventeen inherited long lines from css's computus files);
silva.commissio with explicit paths and the html gate; three scratch
probes (STML shape, first divergent byte with tokens, depth per
process); canon_examen on the declaration; html/arbor.sh once born.
FRUCTUS: two gates born red for real (totality, md) — the mutation
generator and the consumer-of-a-consumer are the two that earn their
cost; the depth probe per process turned "does it crash" into a table.
ASPERITATES: zsh bit three times (`$?` after a pipe; `set -- $spec`
and `"$F"` unsplit — every probe run got one mangled argument and
reported the same crash); the formator reflows a one-line block into a
worse shape; the css computus originals carry lines the formator now
refuses — transcription inherits debt.
DESIDERATA: a planting helper for non-C files (canon, TSV — done by
hand twice); a formator mode that reports without reflowing
one-liners; a `-vitia` that names the css originals as the source of
inherited findings.

## 2026-09-15 — phase 4 sequel: the html5lib oracle arc

**INTENTIO.** After H11 the byte gate could not see a wrong tree, so
the html client got an oracle: html5lib's tree-construction corpus
(1,708 cases, vendored from the local Go module cache — 83 of them in
NUL-bearing files a plain `grep` had silently dropped), read by
`html_exempla`, judged by `probatio_html_oraculum` through a COOKED
VIEW (`html_coctum`) with an unwrap rule for the html/head/body the
expected trees synthesize. Then Fran: burn the cheap classes down,
one commit each, the pin rising in the same commit.

**RELATIO.** Nine commits: 887 at birth → 1,195 (70 %). Builder
tables with `nisi` rows; `--!>` cooking; script-data escaping in
`lib/html_lexema.c`; the 2,231-name entity table as a generated
root-tier library shared with md; scope for p/li/dd/dt/button as
per-frame visibility indices; foreign content through a shared
`html_alienum` module; the lexer's full raw-text set and `plaintext`;
tokens the DOM ignores widening `elementum-malum` to its true meaning;
end tags in three classes. Substrate changes forced: NONE — the H1
vocabulary of ten genera held through every mechanism, because each
one was a placement decision expressed by absent slots. The two open
materia items (raw-form limit, emitter depth) are exactly where they
were. What remains is the first work that touches the registry: O7,
derived-view annotations (`synthesis`, `exemplar`, `sedes`), design
approved by Fran with the round-trip condition as the acceptance
gate.

**DEBRIEF INSTRUMENTORUM.** (1) The totality gate paid for itself a
second time: a stack walk per start tag was quadratic in depth and
killed the 100,000-deep pins while both oracles stayed green — keep
the builder O(1) per token. (2) `silva.Editio` on a `.sh` applies the
C width rule and breaks bash (quaestio 01M2KTVFG0). (3) A generated
`.c` needs its own `.h` or aedilis' include closure never links it.
(4) A suite that compiles another client's sources (briar builds md's
decoder) must be in the gate list when a lib is added — the O2b-4
commit broke briar unseen. (5) `lldb --batch` hangs on a forking
probatio; the crash report under DiagnosticReports is faster. (6)
`grep -c` without `-a` counts zero in a NUL file. (7) The identifier
lint refused `frameset`, `rcdata`, `math`, `numericam`, `titularis` —
each a renamed identifier, the glossary untouched.

**RELATIO, sequela II (2026-09-15/16): O7a, the corpus gate, O7b —
`ef802131`, `974da919`, `fef160d9`.** The design turn produced three
annotations on the html registry and no substrate change. O7a
`synthesis:index`: what HTML5 invents (html/head/body, tbody/tr/
colgroup) is a token-less element with an index, the emitter walks
through it, the cooked view prints it; the oracle's unwrap rule
retired, a fragment entry point added (the context is the frame under
the stack, and foreign breakout never fires in a fragment — x/net's
`!p.fragment`); 1,195 → 1,299. Fran asked whether the whole html5lib
corpus went through the byte round trip; it did not, so
`probatio_html_circuitus` now runs all 1,708 cases through direct
emission (all identical) and the STML cycle (1,672 identical, 36 NUL
refusals pinned BY CAUSE against 01M2KPJ0HW). O7b `sedes:referentia`
on four genera: the DOM parent of a node whose bytes lie elsewhere,
printed before the containing table or after everything; table modes
per frame, foster parenting by annotation, `</body>`/`</html>` now
close (O5 reversed) with the element open at that moment as the sedes
for later content, head content after `</head>`; 1,299 → 1,398 (82 %),
fragments 152/184, zero regressions. html is the first client after
oratio to declare a reference locus; materia's `id`/`#nodN` machinery
carried every reference through the corpus unchanged.

Instrument debrief II: (1) the corpus gate refused O7b's first cut one
commit after its birth — the invented body's closing tokens were
dropped, the oracle rewarded the matching tree, the byte law did not;
a tree oracle alone would have shipped it. (2) A diff of failing sets
loses one case per file when credo's dots prefix the first failure
line — strip, then COUNT against the pin before trusting a diff. (3)
A case can pass by accident of the view (foreign-fragment #48: the
cooked view painted the context namespace over an HTML tree); when
the builder gains the truth the accident becomes an honest failure —
read the reference implementation before "fixing" it. (4) A per-frame
index for every per-token question, never a stack walk (three more
this arc). (5) A mode may only move forward: accepting `<frameset>`
had reset it to "in body" and silently lost the post-body sedes.
(6) A gate that pins REFUSALS by cause turns a substrate limit into a
rising pin instead of a hidden skip. Next: O7c, the adoption agency
and `exemplar`.

**RELATIO, sequela III (2026-09-16): O7c, the adoption agency.** The
third annotation and a fourth the decree had not named. `exemplar:
referentia` on `elementum`: a formatting element reopened by the
adoption agency or by the reconstruction of the active formatting
list — no tokens, name and attributes read through the reference to
the original. `praecedens:referentia`: the moved block's DOM previous
sibling, because byte order cannot place a block after a clone born
after the block's bytes (`<b><a><div>x</b>y</a>`); the spec appends
the block right after the formatting element being closed and the
builder knows that element exactly. The builder's stack became the
spec's DOM stack with byte-open frames and receptors — O7b's
`sedes_posterior` dissolved into the general rule, and `</body>` no
longer pops. WHATWG's agency runs as written on that stack; the clone
wrapping the block's children is byte-tree surgery and byte-safe (a
token-less wrapper emits nothing). `</p>` with no `p` in scope and
`</br>` now yield synthesized elements. 1,398 → 1,504 (88 %),
fragments 152 → 155, zero regressions; adoption01 and adoption02 pass
whole. Substrate changes: none — `materia_nodus_reponere` (rewriting
a written locus) and the list value's append served the wrap and the
re-pointing of `sedes` unchanged. Twenty-five loci, seal `b307882e`.

Instrument debrief III: (1) a diff of failing sets must strip the
fragment-context suffix on BOTH sides — thirteen "regressions" were
cases that had never passed. (2) When a mechanism takes over a tag
(`a`, `nobr`), the old table entry for that tag must go, or two
mechanisms compose into something neither specifies (five `nobr`s
from three). (3) A test string's declared length is a claim the byte
gate cannot check — it round-tripped the terminating NUL happily; the
STML writer refused it with its named cause. (4) `renominare.sh`
refuses a dirty tree; the rename comes before the edits or by hand.
(5) A hand count of loci in a probatio rots the day the registry
grows — size from the registry, as the builder already does. (6) The
spec's "remove from the stack" can remove a byte-open frame from the
MIDDLE of the stack; anything that read "the frame below X" as "the
parent of X" (the foster parent) had to switch to the node's own
`pater`/`sedes`. Next: the foreign tail (SVG `title` lexed raw, `font`
breakout by attributes), then O3 wild fixtures.

## 2026-09-16 — phase 4: the shell client (crusta-arbor-spec §11, crusta-arbor-plan P1–P12)

INTENTIO (NOT FILED IN TIME — written at closure, and it says so). The
plan assumed an INTENTIO here before P1; none was written. What stands
in for it is the spec commit `34b38c36` and the plan commit `87938365`
(2026-09-16), whose §10 named the substrate questions before a line of
`crusta/` existed: a `referentia` to a later sibling; the same newline
bytes as two genera by mode in a language with a `LINEA` munus (what
FIDELITAS makes of it); trivia bound backward as the common case;
`lista-token` loci in daily use; token values with newlines through the
STML raw form; an iterative builder under materia's recursive walkers.
Fran's order of 2026-09-16: bash before JS (the mode questions JS needs,
rehearsed on a smaller grammar). Deliverable = library + oracles (C1);
dialect = bash 5.2.15's default-option grammar, whole (C2); corpus =
the house scripts + FreeBSD `bin/sh/tests/parser` vendored (C3, fetch
asked); decrees 01M2NJ16RG (heredoc body where its bytes are) and
01M2NJ1JR7 (lector = f(mode, position), builder iterative) filed at
approval against desideratum 01M2MC9M29.

RELATIO (2026-09-16, one day, sixteen commits `34b38c36` → P12):
P1 `ab660117` declaration (50 genera, 150 loci) → generated registry,
lexicon, runner, registrum gate; P2 `6b90aa6d` the lector (18 modes,
regions, heredoc queue, backtick depth in the token tail); P3
`4c3bbfef` builder I (frames = mode stack, delayed sentence append,
backward ligator, totality); P4 `bb52bfd8` arithmetic (the machine the
builder drives, 41 operators in 16 measured levels, `$((` rewind,
evaluator vs 43 bash values); P5 `0da4c53b` builder II (every compound,
function forms, `coproc`, `[[ ]]`, heredoc placement, the
POST_COMPOSITUM mode = 19); P6 `ab50f68b` case reader + corpus (231
house scripts and 89 FreeBSD byte-identical; the corpus found the one
bug behind ~150 insane scripts; loci 152); P7 `48b2a5ca` STML round
trip (two cycles, FIDELITAS measured then asserted, heredoc references
carried; SUBSTRATE CHANGE ONE, below); P8 `978f66fe` hand canon (species
tables MEASURED over every tree; zero violations first run); P9
`064107fb` reservation + totality (2,934 fuzz cases; four builder/lector
defects found and fixed; depth pins); P9b `f1eb6c9d` (Fran: "fix it
properly") heredoc bodies in every byte gap, 176 loci, seal `873ce8f4`,
totality strict; P10 `f28d5fae` computus twin + instruments +
pythonica; P11a `91bf20ba` the cooked view (`declare -f` measured as a
stream machine; 36 bash-generated gate cases, 36/36 first compile);
P11b `93d55ea6` the oracle instrument, goldens and gates (`declare -f`
92 → 103/112, `bash -n` 115 → 118/121, pinned rising; parser fixes each
through the arbor gate first); P12 this. Side commits: `c4acd7b0`
(pythonica: `silva.planta` false build-break), `3c8ae321` (html's
computus gate header, a copy slip noticed while transcribing).

NUMBERS: fontes 19 files / 12,328 lines; 14 gates / 10,200 lines /
12,567 assertions, every gate born red by a plant; instruments 5 /
1,223; declaration + canon 2,181 (279 rules). House corpus 234 scripts,
821,533 bytes: byte-identical, ZERO insane, mala pinned at zero; live
oracle 234/234 `declare -f` exact, 236/236 `bash -n`. STML 486 documents
round-trip (≈ 28× the source), FIDELITAS on every one; canon 504
documents clean. Depth at -O2: parse 100,000 lives; emission dies 40–45k
nodes; STML write quadratic, dies at 100,000; evaluator dies 130–160k.

SUBSTRATE: ONE CHANGE (the spec expected none). `_textus_tutus` refuses
a mixed element whose leading or trailing whitespace run holds a newline
— two bytes were silently dropped by the trivia ownership law on a quote
open to EOF (nota 01M2P8H0EA; materia/css/md/html/oratio green). One
LAW surfaced, not a change: the STML reader DERIVES `initium_lineae`
from LINEA trivia, so a client with a LINEA munus must set the flag by
the same rule (315 of 322 files differed until crusta did; nota
01M2P8H7VS). The raw-form limit (01M2KPJ0HW) and the depth crash
(01M1FAD8) reached again and pinned by cause. The §10 answers are in the
spec's §11.2.

AUDIT: `./materia/shim_probare.sh` 396/396, 0 fractae (14.5 s), at
closure. crusta 14/14.

------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: `coquere.sh` (three regenerations, 150 → 152 → 176 loci);
`scribe.sh` for every new C file; `silva.planta` for every gate birth;
`silva.commissio` with explicit paths and the crusta gate; formator
`-scribere`/`-vitia` before every commit; `./oratio/quaere.sh` before
staging; `./silva/nexus.sh` for new symbols; `crusta/arbor.sh` (after
P10 it replaced every scratch probe); `crusta/coctum.sh` and
`crusta/oraculum.sh -domus`; bash itself as a probe (`cat -e` over
`declare -f`, `bash -n` for every sanity verdict); a scratch
delta-debugger (`minuere.py`: shrink a fuzz failure while its class
holds) and `sample <pid>` (named the quadratic static walk in one shot);
the tabularium for every note, quaestio and desideratum.
FRUCTUS: the corpus gate (one bug behind 150 files); the fuzz gate
(five defects in P9/P9b, each hiding behind the previous exemption);
measured canon tables (clean first run); oracle-generated expectations
(no expected text in the P11 gates was typed by hand; 36/36 at first
compile); `-domus` (the whole house through two oracles in five
seconds).
ASPERITATES: `silva.planta` read `error:` anywhere in gate output as a
build break — bash's own diagnostics made a true red look broken
(fixed, `c4acd7b0`); zsh quoting and `printf` decoded escapes before
bash saw them (probes lied twice); latina macro names as locals
(`casus`, `interior`) give examen a syntax-node verdict that names
nothing; two-letter locals and dictionary gaps (`asyncus`, `penultimum`,
`numeralis`, `separatores`, `indentare`, `nul`) refused only at staging
time; the formator never reflows comments (hand rewraps every task);
`silva.Editio` refuses comment anchors and line-shape changes, and must
never touch a `.sh`; the vocabula lint could not link until oratio's
runner compiled `entitates_html` (P7); a stray `git stash` in an
exploratory line stashed the uncommitted builder (popped, verified).
DESIDERATA: examen naming a latina macro used as an identifier; a
comment-reflow (or report-only) mode in the formator; a planting helper
for non-C files (goldens, TSV); the vocabula check over untracked files
without `quaere.sh` by hand.

## 2026-09-17 — sedes: positions in the projection, diagnostics from declarations (materia-sedes-spec, materia-sedes-plan A1–C)

INTENTIO (filed with the plan commit, before A1). Fran, 2026-09-17: "a
line number thing for materia is a great idea", widened the same day
to "polish the quality" of line, column and substring results "with
all errors", once in materia so every client benefits. Desideratum
01M2PSWHM0. Spec `855589fb` (two parts, one spec); plan
`project-specs/materia-sedes-plan.md` (nine tranches + closure).

PART A, the positioned view. `consilium.sedes_scribere` stamps
`sedes="L:C-L:C"` and `octeti="B-B"` on every node, token and trivium
element; the envelope says `visio="sedes"`; the reader refuses it;
canons never see it (the VISIO precedent). A shared library gate
(`materia_sedes_verificare`) checks it in every client's STML gate
against an oracle disjoint from the writer (line table from the source
bytes, source slices, document order). The STML engine's repeated
captures learn TRANSPARENTIA (measured: a position attribute made two
captures of one identifier unequal even under `attributa=`). pythonica
rows gain `linea`, `columna`, `textus_fontis`.

PART B, diagnostics. A client DECLARES problems in its registry
(`diagnosticum` on a genus, `absentia` / `vacua` on a locus, `gravitas`,
`inanis`); `materia_coctor` bakes a separate table (no seal moves, no
hand table touched); one walker (`materia_diagnostica_derivare`) derives
located records plus a generic byte-order check; the writer's refusals
gain a source range. crusta (counters kept as the independent oracle;
presence per class, sana exact; two new differentia pins: severity
against bash's exit, first-error line against bash's line) and css
first; `lib/excerptum` prints the compiler format with a `^~~~`
underline; `./tools/diagnostica.sh` routes by suffix;
`silva.diagnostica` wraps it.

MEASURED WHILE PLANNING (the plan's Deltas D1–D10): md and oratio
already carry derived tokens (points, not ranges); the writer's
`sedes_valorum` was never filled (a dead half-port, left alone); strict
matching has no TRANSPARENTIA by design (unchanged); `{ }` leaves
crusta's list unwritten and `_lista_plena` ignores separators, heredocs
and mala (`vacua` redefined, new `inanis`); `elif`'s empty test is a
conditional (parser-emitted, merged); the arithmetic machine counts one
trace at several sites (presence, not counts); `bash -n` exits 0 for
`[[ a b ]]`, `(( 1 2 ))`, `(( 3 ! ))`, `$(( 1 + ))` and a heredoc at
EOF (those classes are `monitum`); crusta's and css's object sets
collide when linked together (the instrument builds its own).

SUBSTRATE: changes expected in `materia_nodus` (range), `materia_arbor`
(view, envelope, reader message, refusal range), `materia_coctor`
(table), new `materia_sedes` and `materia_diagnostica`; root lib
`stml_macros` (equality) and new `excerptum`. Every materia
writer/reader commit runs `./materia/shim_probare.sh` and all client
suites.

RELATIO (2026-09-17, one day, twelve commits `325180d5` → `45e0eb3b`
plus three side fixes). 95 files, +7,390/−434. Four new sources:
`materia_sedes.c` 713, `materia_diagnostica.c` 591, `tools/diagnostica.c`
291, `lib/excerptum.c` 122.

PART A, as built. `sedes="L:C-L:C" octeti="B-B"` on every node, token
and trivium element; locus wrappers never; the envelope says
`visio="sedes"` (or `"partialis sedes"`) and the reader refuses it by
name. Node ranges accumulate BOTTOM-UP in the writer's single walk —
O(n), not O(n·depth) — a decision that paid twice: B2's walker was
written recursively and per-node, and had to be rebuilt to this same
shape after the fuzz gate killed it.

`materia_sedes_verificare` is the load-bearing piece: one library gate
called from all five clients' STML gates, checking against an oracle
DISJOINT from the writer (line table rebuilt from source bytes by
binary search, source slices compared to token values, document order
independently walked), and also proving the view equals the plain
projection minus its three attributes, and that the reader refuses it.
Zero divergences at first run over **5,501,210 elements**: crusta
339,503 · css 9,679 · html 16,439 · md 2,491,843 · oratio 2,643,746.
Derived tokens are points everywhere (D1 held). The view costs **×2.47**
the plain projection over 237 house `.sh` (57,459,770 vs 23,264,631
bytes; source 826,532) — pretty breaks long attribute runs onto aligned
lines. Not pinned; a view is never a file.

A3 was the surprise. Repeated NODE captures compared bytes, so a
position attribute made two captures of the same identifier unequal even
under `attributa=`. The fix was never a regression: the trivia model
(`b4ce0559`, 08-24) predates regula V (`f7439242`, 08-31). Scanning
every tracked rule — 42 text captures, 10 attribute, 6 node — showed
the six node captures were all A3's own fixtures, so **no production
rule changed behaviour**. With Fran the copy became `_nodum_comparandum`:
authored form dropped ALWAYS (layout `spatia_*`, `</>`, `(>`, multiline
`indentatio`), TRANSPARENTIA parts when active, children re-parented —
because the writer reads a text's multiline prefix from its parent. A
live probe over real tool output found what literal fixtures hid (the
first child owns the newline). Exemplaria 392 → 414 assertions; oracle
rows byte-identical, 45.24 → 45.07 s.

PART B, as built. Declarations bake into a SEPARATE table
(`<P>_DIAGNOSTICA`), generated only when something is declared, so the
four silent clients regenerate byte-identical and **no seal moves** —
verified on a real client: crusta's `873ce8f4` before and after 39
declarations. Current declarations: crusta 1 genus + 28 `absentia` +
7 `vacua` + 3 `inanis` + 5 `gravitas`; css 3 genera; md, html and oratio
none.

One walker derives them, plus `materia:ordo-octetorum` free for every
client, plus `emissa` for classes with no tree trace. New gates and
their assertions: materia sedes 63, materia diagnostica 63, crusta
diagnostica 800, css diagnostica 70, excerptum 21. Gate counts: crusta
14 → 15, css 9 → 10.

crusta keeps its four counters as an INDEPENDENT oracle (presence per
class, `sana` exact): zero disagreements over house 235 / oracle 125 /
adversarial 17 / 16 inline shapes / 2,934 fuzz cases. The two
differentia pins read **gravitas 122/125** and **lineae 7/9**.

`./tools/diagnostica.sh` runs the whole house — 235 `.sh` — in **0.22 s**
with zero diagnostics.

WHAT THE PINS SAY, which is not what the plan predicted. D9 expected
its four monitum classes to resolve the three known `bash -n` discordes;
they did not, and the reason is better than the guess: all three are
desideratum 01M2PN1VYH (backslash-newline before lexing, twice; a
heredoc in a multi-line substitution, once), so **the gravitas pin now
measures that desideratum's price** and reaches 125 the day it is
fulfilled. The line pin's two misses are a complete law rather than a
residue: every case where bash names a TOKEN agrees; two of the three
where bash says `unexpected end of file` differ, because bash reports
the EOF line and we report the end of the unfinished construct. That
divergence is kept deliberately — the construct's place serves a reader
better than the end of the file.

THE FINDING THAT REPEATED, and the reason it gets its own paragraph:
**three separate planted faults came back GREEN, and each time the green
was measuring the TEST, not the code.** (i) crusta's `separator`
`inanis` rule: no corpus anywhere holds a list of only separators, so
sixteen inline shapes had to be written before the rule had a case at
all. (ii) The gravitas pin: it read 118, identical to `relatio.sana`'s
own pin case for case, and removing EVERY `gravitas="monitum"` in the
declaration could not move it — because no oracle case carried a
monitum without also carrying an erratum; D9's four shapes existed only
as inline shapes with no bash golden, and had to be added to
`pathologiae.sh` before the pin could ever fail. (iii) `lib/excerptum`'s
UTF-8 fixture used `é`: flipping the continuation mask leaves the
character-start count unchanged in a TWO-byte sequence (1 either way)
and only differs at THREE (1 vs 2), so the fixture covered UTF-8 and
still could not see the fault. Widened to `€`. **A pin that cannot fail
is measuring its neighbour, not the world** — and the cheapest way to
find out is to break the thing it watches and check that it notices.

A fourth, same family, caught before it shipped: css's `selector-malus`
is built ONLY in `css_selector.c`'s analysis tree, which shares tokens
with the stylesheet tree but never nodes, so a walk over
`css_arbor_parsare`'s result can never reach it. As the plan was
written, a third of css's declarations would have been untested and its
plant green. Ask which TREE a walk actually walks.

CLIENT BUILDS: B1 and B2 changed materia sources but ran only the crusta
and materia gates, so oratio's, md's and html's build directories held
objects older than those sources. `oratio/oraculum.sh` refused them —
the guard added in `e12c1f46` the same day, doing exactly its job — and
the PYTHONICA gate is where it surfaced, three commits later. A
materia-substrate edit invalidates every client's build; every client
suite belongs in that commit's gates. Rebuilt: oratio 19/19, md 14/14,
html 14/14.

SIDE FIXES the arc forced. Silva's lib count pin was already red at
baseline on an unchanged tree (176 vs 180); the first fix was incomplete
because it was gated on the `exemplaria` filter, and two more pin sites
lived elsewhere — **a count pin lives in many files; a filtered gate
proves one site, not the class** (`6e679497`, `3728f590`). And
`e12c1f46`: client runners never watched `materia/fontes` headers and
instruments relinked only on their own-prefix objects, so a stale object
could hold an older struct on its stack while fresh code wrote past it
(traced to a concrete UB path at `oratio_resolutio.c:2043`). Guards
widened, a post-build `excubitor.sh` check added to five runners, 16
instruments moved to `*.o`, both oracles now refuse stale objects.

------------------------------------------------------------------------
DEBRIEF INSTRUMENTORUM
ADHIBITA: `coquere.sh` (six regenerations across two clients, all
byte-identical on revert); `scribe.sh` for every new C file;
`silva.planta` for gate births (C only — see below); `silva.commissio`
and `commissio_umbra` with explicit paths; formator `-scribere`/`-vitia`
before every commit; `./oratio/quaere.sh` before staging;
`./crusta/oraculum.sh -scribere` with bash 5.2.15 in the golden's own
environment (`env -i LC_ALL=C PATH=/nonexistent bash -r`);
`COMPUTUS_SCRIBERE=1` with a named cause; `excubitor.sh` (which caught
the stale client builds); `silva.porta` for every gate; the tabularium
for every note, desideratum and quaestio; and bash itself as the oracle
for four shapes D9 had measured.
FRUCTUS: the shared verifier (five clients, one gate, zero divergences
over 5.5M elements at first run); the independent-counter design in
crusta (two implementations over disjoint code, so each covers the
other); declarations instead of code (css got located errors with THREE
attributes and no C at all); the instrument over the whole house in
0.22 s; and the fuzz gate, which is the only reason B2's walker was
found to be both recursive and quadratic before it shipped.
ASPERITATES: `silva.planta` cannot plant in a `.py` — its anchors are C
token sequences — and reports `ancora 0 vicibus inventa` against
perfectly present text; it also correctly REFUSED a plant that would not
compile (`-Wunused-parameter`), running nothing rather than reporting an
unearned red. **The Edit tool NORMALIZES line endings**: editing
`pathologiae.sh` through it stripped all four CR bytes from the `crlf`
case, and the regenerated golden duly recorded bash accepting the file
(status 2 → 0) — a fixture quietly neutered, caught only because the
golden diff showed a case I had not touched. C hex escapes are GREEDY
(`"a\xE2\x82\xACb"` is one out-of-range escape). `registrum` as a
parameter name parses as an error node (latina macro for `register`).
`i32` is UNSIGNED, so a negative sentinel and a `>= 0` assertion are
both silent bugs. The STML reader does not decode entities in attribute
values. And `silva.diagnostica` was ALREADY TAKEN — Python shadows a
duplicate `def` silently, so the new function was defined, shadowed and
dead, and the only symptom was a hand call returning the wrong TYPE.
DESIDERATA: a planting helper for non-C files (`.py`, `.sh`, goldens,
declarations — every plant in this arc outside C was written by hand);
a byte-preserving edit path for fixtures whose bytes are the point;
examen naming a latina macro used as an identifier; the vocabula check
over untracked files without `quaere.sh` by hand (01M2Q0MMVH, met
again); a `utf8_latitudo` table, because codepoints are not columns
(01M2RHXBWP — the excerpt's caret misses CJK by one column short and
combining marks by one long, and tessera says "latitudo 1 praesumpta").

## 2026-09-17 — sedes multiplices: a diagnostic that points at two places (sedes-multiplices-spec, sedes-multiplices-plan 1–4)

INTENTIO (filed with the plan commit, before task 1). Desideratum
01M2RKYA4Z, ordered FIRST by decree 01M2RNK5MG — before the
`Congruentia` adapter (01M2RNJ9XN) and before any lint rule, because
both change `MateriaDiagnosticum` and an adapter written against the
one-span record gets written twice. Fran's reason for the whole line
of work: diagnostics are the agent's interface to a parser. Spec
`69f06895`; plan `project-specs/sedes-multiplices-plan.md`, four
tasks.

THE MEASUREMENT THAT MAKES IT CHEAP. The second span is already
computed and thrown away: the `ABSENTIA` arm
(`materia_diagnostica.c:306`) takes the endpoint of the node's
accumulated range `t` and lets `t` itself fall off the stack. 28 of
crusta's 36 declarations are `absentia`, so the free span lands on
roughly four fifths of them the day it ships; css declares zero, so no
css output moves.

THE SHAPE, settled with Fran in a design conversation and not open
(SM1–SM8). The primary span stays a plain field — the header line and
the pinned sort key `(tractus.initium, codex)` both need exactly one
position, and a flat array would still need a "sedes[0] is primary"
law, the same privilege with an indirection in front of it and an
empty case the type allows and the facts forbid. Related spans carry a
LABEL, never a role enum: LSP's `DiagnosticRelatedInformation` is
`{location, message}` and clang's notes are a location and a sentence,
so a role would only ever pick a default label and would be discarded
on the way out. Labels are FINITE VERBS and never participles — a
participle agrees with its subject and the subject varies by node
genus (`grex` masculine, `redirectio` feminine), so one fixed
participle is quietly wrong most of the time it is shown; hence `hic
coepit` and `hic exspectatur`. Printing is rustc-shaped: one block,
source order, gutter sized once, no elision and no inline multi-caret
interleaving.

WHAT IT DOES NOT TOUCH, which is most of why it is cheap. No registry
vocabulary, so `MateriaTabDiagnosticum` is unchanged and NO SEAL
MOVES. No STML projection change, so no writer, no reader, no
round-trip gate, and `materia_sedes_verificare`'s law survives.
`excerptum_scribere` and all its callers unchanged. Every pinned
assertion in the existing gates reads `codex`, `gravitas` and the
PRIMARY tractus.

DELIBERATE OUTPUT CHANGE, stated so nobody reads the negative space as
"nothing moves": an `absentia` primary gains the label `hic
exspectatur` even when no opening span is found. Nothing pins the
human text — the gates read struct fields, the smoke test greps TSV
columns — so no gate moves, but the change is intended and visible.

CORRECTION CARRIED IN. The desideratum's own body claimed multi-span
would make crusta's `bash -n` line divergence evaporate. It does not:
it adds a THIRD position, and the divergence resolves only if someone
chooses a span set that includes bash's EOF line. Corrected in spec §8
AUDIENDA I and noted on the ticket.
