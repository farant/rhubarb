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
