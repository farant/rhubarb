# silva_arbor.worklog.md

Worklog for the arbor dialect — silva parse trees ⇄ canonical STML.
Plan: `project-specs/arbor-stml-plan.md`. Spec: `arbor-stml-spec-v2.md`.
Ledger: parcum 01M0DMSTEJ.

---

## 2026-08-20 — T2, the vocabulary layer

Four artifacts landed: the registry seal, registry name lookups, the
spelling table, and lexeme tag mangling. No tree code yet — T2 is
deliberately the layer that touches no trees, which is why it commits
cleanly on its own.

### The distinction that organised the work

Of the four artifacts, **only the spelling table creates new truth.**
The seal, the name lookups, and the tag mangling all *derive* from
tables that already exist, so they cannot disagree with anything —
if they're wrong, they're wrong in an immediately visible way. The
spelling table is different: silva has no reverse genus→spelling map
anywhere (keywords have a forward-only `VERBA_CLAUSA`; punctuation is
an if-else recogniser cascade in `_legere_interpunctionem`), so arbor
had to author one. That makes it a **second source of truth that can
silently drift from the lexer**, and it's where essentially all the
gate effort went.

### The seal is total, not a sample

Worth recording because it's a property, not a lucky accident:
`SilvaRegistrumCoctum` has exactly four fields — two array pointers
and their two counts. Walking `genera[]` and `loci[]` completely
therefore covers the entire struct. There is no hidden state a
grammar change could hide in, and no version field anywhere in the
tables (table CONTENT is the only version signal, which is exactly
what we want to hash).

Format: FNV-1a over `titulus` + `loci_offset` + `loci_numerus` per
genus, then `titulus` + `species` per locus, each field followed by a
separator — without the separator `"ab"+"c"` and `"a"+"bc"` produce
identical material. There's a test for exactly that.

Output is **8 hex digits, zero-padded**. Deliberately NOT
`chorda_aedificator_appendere_hex_i32`, which uses `%x` and therefore
emits variable width — a seal of `0x0000abcd` would render as `abcd`.
Seals get compared by eye in documents; fixed width is worth eight
lines of nibble loop.

### The spelling table's two gates

The table is positional (index = genus, O(1) lookup) and carries a
**redundant `genus` field whose only purpose is to let the lookup
verify its own alignment**. If someone reorders the enum without
reordering the table, `silva_arbor_orthographia` returns NIHIL rather
than the wrong spelling — it refuses instead of lying.

That alone would be a *dead* gate, though, because a test that
iterates "every genus that has a spelling" would simply skip the
broken entries and stay green. So the probatio names the enum ranges
that MUST have spellings (`AUTO..WHILE`, `QUADRA_APERTA..
CANCELLUM_CANCELLUM`) and asserts presence over those ranges. Then,
for each, it lexes the spelling and asserts the genus comes back.

**Both failure modes were planted and measured**, per doctrine:

| planted fault | result |
|---|---|
| `SAGITTA` spelled `"=>"` instead of `"->"` | 2 failures, named: `DIVERGENTIA: SAGITTA -> '=>' -> IGNOTUM` |
| `PLUS`/`MINUS` rows swapped | 3 failures, named: `DEEST: PLUS`, `DEEST: MINUS` |

There's also a canary asserting `SILVA_LEX_NUMERUS_GENERUM == 95`, so
a newly added lexeme genus breaks this test and forces someone to
classify it as fixed- or variable-spelling. That's intentional
brittleness: the table must stay complete.

All 80 fixed spellings (32 keywords + 48 punctuation) lex back
correctly on the first run, including `#`, `##`, `...`, `<<=`.

### Tag mangling and the measured collisions

`lex-` prefix, lowercase, `_`→`-`. The reverse direction is
implemented **by running the forward mangling over every genus and
comparing** rather than by a second parser, so the two directions
cannot drift. 95 genera scanned per lookup, which is fine — it runs
once per document element, not in a tight loop.

The two collisions from spec §0 are asserted explicitly, since they
are the entire justification for the prefix:
- `assignatio` is both node genus 35 and `SILVA_LEX_ASSIGNATIO`. The
  test asserts `lex-assignatio` is NOT a node genus, and that bare
  `assignatio` is NOT a lexeme tag.
- `corpus` is both a genus (index 2) and a locus name.

That second one produced the sharpest test in the file. `corpus` is a
real locus of `definitio-functionis` but NOT of the genus `corpus`
(whose loci are `tok_aperta`/`elementa`/`tok_clausa`). So a lookup
that ignored genus scope would return a plausible wrong answer. The
test asserts both the hit and the miss.

### Two traps hit

**`registrum` is a latina.h macro** (→ `register`), so
`SilvaRegistrumCoctum registrum;` expanded to a declaration of
nothing and clang reported "declaration does not declare anything"
plus a cascade of "assignatio classium incompatibilium" at every call
site — because the helper's *definition* failed to parse, so its
calls looked implicit. This is the documented landmine and it is on
the short list of worst offenders in MEMORY.md. Note for the record:
**examen flagged it correctly before clang did, and I doubted the
tool.** House name is `tabularium`.

**`/proc` does not exist on darwin**, so an `until [ ! -e /proc/$$ ]`
wait loop is instantly true and reports a still-running job as
finished. Same class as the lying-green gates: a guard that always
passes on a platform where its subject doesn't exist.

### Amalgam choreography — T1's gate earned itself

Adding `silva/fontes/silva_arbor.c` is precisely the silent-drift
case T1's freshness gate was built for, and this was its first live
firing on a real (not planted) drift. It refused, named both the
missing `.h` and `.c` rows, and named the regeneration command.

Excludenda then went **154 → 148**: six vendored functions stopped
being dead because arbor calls them — `chorda_aedificator_appendere_i32`,
`appendere_s32`, `spectare`, `destruere`, and the two internal
`_format_integer_*` helpers. Exactly the set the seal introduced,
which is a clean confirmation that the harvest tracks real
reachability rather than guessing.

### Named door, not taken

`silva_arbor_genus_index` / `_locus_index` are the **fifth**
hand-rolled name→index scan in silva (scribere:76, commissio:471,
parsare:10, quaestio:289). Well past the third-time rule. Promotion
to a shared helper in `silva_tabulae.h` touches five existing files
and wants its own tests — recorded in the source comment as a door,
deliberately not opened during M1.

### Counts

518 assertions, all green.

---

## 2026-08-20 — T3, the writer (tree → STML)

`silva_arbor_scribere_nodum`. 561 assertions green (43 new).

### The signature gained a parameter the plan didn't have

`grammatica` is passed in, not derived. The registry **cannot name
itself** — there is no version or identity field anywhere in
`SilvaRegistrumCoctum` (that's the same fact the seal exploits: table
content is the only version signal). The seal distinguishes grammars
cryptographically; the human-readable name has to come from the
caller. Hardcoding `"c89"` while accepting any registry would have
been a lie the document then carries.

### AMBIGUUS: the divergence is an ABSENCE of code

`silva_scribere` emits only the canonical arm, because bytes can spell
exactly one reading. Arbor emits all of them. The notable part is that
this required **no special case at all** — the generic walk emits the
whole `interpretationes` list, and *not* special-casing it IS the
divergence. Byte-roundtrip is structurally incapable of preserving
ambiguity; this is the representational audit's first real catch.

### Compact trivia lenses are REQUIRED, not cosmetic

This is the finding that would have silently corrupted documents.
Trivia valor for `SPATIA`/`TABULAE`/`NOVA_LINEA` is **whitespace
only** — and the stml pretty-writer deliberately drops whitespace-only
text nodes (fidelity vs legibility, lib/stml.c). Emitting indentation
as text content would therefore have been discarded with no error at
all. `<lex-spatia n="4"/>` carries the same information in an
attribute, where nothing strips it. Nothing is lost: the valor is
fully determined by the count.

Generalized, and worth carrying into T5: **when a format has a
"transparent" or "insignificant" class, any payload that falls into
that class needs a representation outside it.**

The same reasoning produced `_textus_tutus`, which REFUSES any
variable-spelling valor that is whitespace-only or NUL-bearing rather
than losing it silently. T6 will measure whether it ever fires on the
real corpus; today it is a guard against a class, not a known case.

### Two-pass, because first use can't know

Fragment/transclusion sharing needs to know at the *first* emission
whether a token will be used again. Pass 1 counts token uses into a
pointer-keyed hash; pass 2 emits, giving `<#lexN>` to the first use of
any token with count > 1 and `<<#lexN>>` to the rest. The alternative —
giving every token a fragment id — would be correct and unreadable.

Pass 1 also captures the position ANCHOR (first token in *walk* order,
not byte order, since the loader replays the walk).

### T0 shows up twice, in situ

1. The document's token elements are **mixed content** —
   `<lex-identificator>n<post><lex-spatia n="1"/></post></>` — which is
   exactly the shape T0 fixed. Before T0 the pretty writer injected
   newlines around `n` and corrupted the valor, compounding each
   cycle. There is now an assertion on that literal string, so T0's fix
   has a regression guard at a real call site rather than only in
   synthetic shapes.
2. Every closing tag is the tacit `</>` under the 30-line threshold,
   also T0. My first assertions guessed `</lex-integer>` and failed —
   the writer was right and the test was wrong.

### Amalgam: the prediction in the plan came true exactly

T7 step 2 anticipated that arbor would pull `stml_scribere` and
friends back out of `EXCLUDENDA_STML`, since silva had no runtime STML
*writer* before. Measured: excludenda **148 → 131**, and the 17
revived names are the entire writer half —
`stml_scribere`, `stml_scribere_ad_aedificator`, `_scribere_evasus`,
`_scribere_indentatio`, `_spatium_album_solum`, the element/attribute/
text constructors, and **`_lineae_contenti`**, the tacit-closure helper
written in T0 that was pruned at birth because its only caller was
itself excluded.

### A test bug worth remembering

`"<<#lex1>>"` CONTAINS `"<#lex"`, so a naive substring count of
fragments actually counts fragments + transclusions, and the identity
`fragments == transclusions` fails for reasons that look like a writer
bug. Fragments = raw − transclusions. Substring counting over a
nesting syntax needs the containment checked, not assumed.

---

## 2026-08-20 — T4, the comparator

`silva_arbor_aequalis` in its own `silva_arbor_aequalitas.c`.
626 assertions green (65 new).

### Positions forced a MODE, and Fran named why

The plan listed `initium_lineae` but not the coordinates. The spec's
§4, though, wants the gate to compare *recomputed* fields — and
positions are the largest part of that class, since we chose to derive
rather than carry them. Skipping them would leave T5's derivation
essentially unverified.

But comparing them unconditionally breaks the consumer the plan names:
mutatio's gates ask "did this transform preserve the tree" about a
subtree that legitimately *moved*. Fran's question — what about a pure
STML AST with no source text — is the same problem from the third
direction: authored trees have no coordinates at all.

So: `SILVA_ARBOR_COMPARATIO_STRUCTURALIS` vs `_FIDELITAS`. T6's oracle
A runs fidelity; mutatio will run structural.

### The refinement that matters more than the mode

The tempting implementation is "compare positions only when both
tokens have `byte_offset >= 0`" — auto-skip when synthetic. That is
**exactly the guard-with-no-subject defect** from this morning's
`/proc` trap: a derivation bug that left every offset at -1 would sail
through in silence.

So position **values** follow the mode, but position **provenance** —
the sign of `byte_offset`, sourced vs synthetic — is compared in
**every** mode. A token silently changing provenance is a structural
fact about the tree, not a coordinate. There's a test asserting the
provenance mutation is caught in *both* modes.

### What the comparator deliberately CANNOT see

Trivia double-ownership. If one token appears in two `spatia_*` lists,
both owners look locally correct and a structural walk says nothing.
That is not a defect to fix here — it is the entire reason T6 has a
second oracle (byte emission, where it shows up as duplicated bytes).
The header says so explicitly, because the natural instinct on reading
this comparator is to "improve" it until it catches everything, which
would collapse two disjoint failure classes into one.

### pater: nullity only

Comparing `pater` pointers across two trees is meaningless — different
objects. The stronger check, "pater equals the node we descended
from", breaks on shared subtrees, because `committere` assigns from the
LAST visit in walk order (parked twice in the phase-log). So the
comparator compares **nullity**: `a->pater == NIHIL` iff
`b->pater == NIHIL`. That still catches the thing worth catching — a
loader that never fixed pater up at all.

### A guard concealing a vacuous test — caught by removing it

The pater test was first written with `si (a->pater != NIHIL) { ... }`
around the assertions. It passed. Removing the guard turned it red,
which revealed that **`declaratio` has no pater at all**: a top-level
node sits in the radix LIST, and commissio assigns pater between
*nodes*, not from a list. So the guarded version asserted precisely
nothing, and looked identical to a working test.

Fixed by running the pater assertions on a NESTED node
(`typus-primitivus`) that genuinely has one. Same lesson as the trivia
guard I removed in the same pass: **a conditional around an assertion
is a silent opt-out, and the only way to find out whether it is
load-bearing is to delete it and watch.**

### Field coverage

Planted and confirmed, each naming its own field: `lexema/genus`,
`/valor`, `/standard`, `/fons`, `/initium-lineae`, `/provenientia`,
`/offset`, `/linea`, `/columna`; `trivia/post` (count) and trivia valor;
`nodus/genus`, `/numerus-locorum`, `/pater-nullitas`;
`locus/genus-valoris`; plus NIHIL handling and self-equality.

The mode itself is tested from both sides — a mutated coordinate must
be caught by fidelity AND ignored by structural. One assertion alone
would not distinguish a working mode from a mode that does nothing.

### Diagnostic path

`SilvaArborDifferentia.via` carries a `genus.locus>genus.locus` trail,
built push/pop during descent into a fixed buffer (no allocation, so
the comparator needs no piscina). Genus is numeric because the
comparator takes no registry — it is deliberately grammar-ignorant,
touching only the two trees.

### The amalgam caught something the modular build cannot see

`silva_arbor_aequalitas.c` first defined `interior` helpers named
`_nodi_aequales` and `_valores_aequales`. Both files compiled cleanly
in isolation — but `silva_glr.c` already has statics by those exact
names, and **in a single-file amalgam every `static` shares one
namespace.** The amalgamator refused by name, naming both files.

Worth recording as a class: the modular build and the amalgam enforce
*different* rules, and the amalgam's rule is invisible until you run
it. Any new `interior` helper with a generic name is a latent
collision. All nine statics in the file now carry an `_arbor_` prefix,
which makes the collision structurally impossible rather than
merely absent today.

Second thing that earned itself in the same failure: the failed
excludenda run left the committed manifest **untouched**, because of
the snapshot+trap added in T1 after that generator destroyed 153
entries by failing mid-write. First real (unplanted) exercise of that
safety net.

---

## 2026-08-20 — T5a, the reader (validate + build)

`silva_arbor_legere`. 666 assertions green (40 new). Scope split from
T5 deliberately: T5a validates and builds, T5b derives fixups and
resolves transclusions.

### The asymmetry that shapes the whole tranche

The writer receives a tree the parser built, so it may assume
well-formedness and refuses only at the expansion boundary. The reader
receives arbitrary bytes and must establish everything. Most of T5a is
validation, not construction — eight named refusals against one build
path.

### `stml_textus_internus` is DESCENDANT-wide, and that silently lies on mixed content

The spec says valor comes through `stml_textus_internus` VERBATIM
rather than `_normalizatus`. That guidance is about *transformation*
and it is right. But it says nothing about **scope**, and the scope is
a trap: `textus_internus` concatenates every descendant text node. An
arbor token element is mixed content — text beside `<ante>`/`<post>` —
so a token carrying a comment in its trivia would have the comment's
text **absorbed into its valor**, silently.

So the reader takes DIRECT text children only. Same law as the
trivia-lens finding in T3, from the other side: it is not enough to
choose the right transformation, you have to choose the right extent.

### The writer's refusal is what makes the reader unambiguous

First run, the round-trip refused with "valor in genere orthographiae
fixae" at line 6. Real bug, mine: the pretty writer emits newline+
indent as text nodes, and those are *direct* children of a token
element, so every prettily-written token looked like it carried a
valor.

The fix is to skip whitespace-only direct text — and the reason that
is SAFE rather than a guess is that the writer REFUSES whitespace-only
valor (`_textus_tutus`, added in T3 for a different reason). Because
that value can never be a valor, skipping it is unambiguous.

**One contract living in two halves, and both halves needed writing
before either was safe.** Worth remembering as a positive instance of
the pattern that usually shows up as a defect.

### The comparator caught the loader on its first real use

T5a's tree is deliberately incomplete — no pater, no positions — so
the round-trip test asserts that `silva_arbor_aequalis` **refuses**,
and names what it refuses on. The first divergence is
`nodus/pater-nullitas`, which is exactly the case the T4 header says
that check exists for ("a loader that never fixed pater up at all").

Twice now I guessed which field would diverge first and was wrong
twice — `lexema/valor`, then `lexema/initium-lineae`. Printing the
comparator's own `campus` and `via` settled it in one run. The
diagnostic path (`16.0` = genus 16, locus 0) paid for itself
immediately: that is the out-struct doing the job a boolean could not.

When T5b lands, this assertion flips from "refuses, naming pater" to
full equality. That is the intended shape — the test currently
DOCUMENTS what T5a does not yet provide, rather than pretending.

### A planted fault that tested the wrong thing

The unknown-genus refusal first substituted `<declaratio>` — whose
closing tag is NAMED, because the element exceeds the 30-line tacit-
closure threshold from T0. Replacing only the opening tag produced
mismatched tags, so the document failed as an **STML parse error** and
the test "passed the refusal" for entirely the wrong reason. Switched
to `<typus-primitivus>`, which closes with `</>`.

Generalizes the planted-fault law: it is not enough for the fault to
be real and for the gate to go red. **The fault has to fail for the
reason under test** — a document that dies in the parser never reaches
the vocabulary check at all.

### Deferred loudly, not silently

Transclusion resolution is T5b, so a document containing one is
REFUSED by name ("transclusio nondum resoluta (T5b)") with its line.
Fragments are unwrapped rather than refused, so a shared document
fails at the transclusion — where the real deferral is — instead of at
the fragment, where the message would mislead. There is a test using
the real AMBIGUUS document for exactly this.

---

## 2026-08-20 — T5b, fixups + transclusion resolution

683 assertions green (17 new). **Oracle A is now live**: parse →
serialize → load → `silva_arbor_aequalis(FIDELITAS)` returns equal,
positions included.

### pater was a CALL, not a walk

The plan lists "recompute pater" as a step. It isn't one:
`silva_committere` already does the pater fixup — its own header says
"oraculum/resolutor NIHIL licent (tunc nulla resolutio - pater +
normalizatio solae)". So the fixup and the commit that makes a loaded
tree queryable are **the same operation**, and the reader just calls
it. Ten seconds of reading the header removed a whole step. Kin to the
house rule about pricing narrowings at the seam rather than from
memory.

### Positions: a second implementation, but a GATED one

Derivation walks in emission order maintaining `(offset, linea,
columna)` from the envelope anchor. I had hoped to reuse
`silva_lexema_emittere_in` so there would be no second byte-counting
implementation — but that primitive emits *ante + valor + post as one
blob* and gives no intermediate positions, and per-component positions
are exactly what is needed. So the scanner is genuinely a second
implementation.

What keeps it honest is that **the round-trip gate compares its output
against the lexer's**, on every fixture at T6. Unlike the spelling
table, the second source of truth here has an oracle that judges it
directly rather than by proxy.

Two details the scanner has to get right, both discovered by writing
it rather than by reading:
- **Scissurae must be counted.** Valor is the *clean* value; the
  `\<newline>` splices were removed. They are re-inserted on emission,
  so if the scanner skips them, everything after a splice-bearing
  token slides.
- **Logical vs physical lines.** `linea` counts physical newlines,
  including those inside a CONTINUATIO. But `initium_lineae` counts
  LOGICAL lines, so a CONTINUATIO must NOT set the "next token starts
  a line" flag while a NOVA_LINEA must. Two counters over the same
  bytes.

### Shared tokens must not advance the cursor twice

An AMBIGUUS emits every arm, and the arms cover the SAME source bytes.
Walking both would advance the cursor twice and slide everything after
it. Since resolution makes shared tokens the same object, the guard is
"already placed → skip", which falls out naturally from identity being
preserved rather than needing separate bookkeeping.

### Splitting the derivation made the authored case honest

First cut gated the whole walk on the anchor, so an anchorless
document got no derived fields at all — and the round-trip test then
diverged at `lexema/initium-lineae` rather than the provenance I
expected (third wrong guess; printing settled it again).

That was a design smell, not just a test problem. `initium_lineae` and
`longitudo` derive from **trivia and valor**, not from any anchor.
Only coordinates need one. Split accordingly: line structure is always
derived, coordinates only when anchored. Now "no anchor" means "no
coordinates" rather than "nothing derived", and an authored tree —
Fran's case — gets everything that is actually knowable about it.

### Planted faults against my OWN success

Full fidelity equality on the first run is suspicious, so the passing
result was attacked:

| planted | result |
|---|---|
| anchor `b="0"` → `b="500"` | fidelity diverges at `lexema/offset`; structural still equal |
| anchor attribute removed | coordinates stay -1, comparator refuses at `lexema/provenientia`, line structure still derived |
| transclusion id → unknown | refused: "transclusio ad fragmentum ignotum" |

The first is the important one: without it, "positions match" could
mean "derivation works" OR "the comparator never looked". Moving the
anchor proves the derived coordinates actually flow from it.

### Identity, asserted as identity

The AMBIGUUS document now loads, and the test asserts the two arms are
**different node objects** whose first tokens are **the same pointer**
(`CREDO_AEQUALIS_PTR`). Equality of content would have passed with
duplicated tokens; only pointer equality shows sharing survived.

---

## 2026-08-20 — T6, the M1 gate + representational audit

`probatio_silva_arbor_circuitus.c`. **78 files, 281 subtrees, 270/281
on BOTH oracles, zero divergences.** The 11 non-passing cases are the
documented M1 refusal (`lexema non-FONS`, the expansion boundary) and
are designed behaviour, not residue.

### The audit found two real defects — which is the point

First run: oracle A **79/281**. The audit named the classes, and both
turned out to be genuine bugs rather than representational limits.

**1. `nodus/pater-nullitas` × 109 — a bug in MY comparator.** A
subtree selected from the middle of a tree HAS a parent; the same
subtree loaded standalone cannot. So the pater-nullity check was
comparing something whose answer lies outside the comparison. Fixed:
pater is compared for INTERIOR nodes only. The T4 test that planted
this fault had planted it at the comparison root, so it went silent
after the fix — moved to an interior node, plus a new assertion that
the root deliberately ISN'T compared.

**2. `lexema/offset` × 178 — a real bug in the anchor.** The writer
captured the anchor from the first *token*, but emission begins with
that token's *leading trivia*. So the reader placed the cursor at the
token, emitted the trivia first, and every position slid by the
indentation width.

**The small test could not have caught this.** `int n = 0;` has its
first token at offset 0 with no leading trivia — the one shape where
the bug vanishes. It took the corpus. Straight instance of "fixtures
share your assumptions; a corpus does not."

**3. `lexema/initium-lineae` × 10 — a representational fact, not a
bug.** Whether the first token starts a line depends on what precedes
the subtree IN THE FILE. It is not derivable from the subtree at all.
So it is context, exactly like the anchor, and is now carried on the
envelope as `linea-initium`.

After all three: **270/281, zero divergences.**

### The spec's justification for oracle B is WRONG, measured

Spec §8 and the plan both say oracle B is needed because the
comparator is **blind to trivia double-ownership** ("both owners look
locally correct"). The plan also said: if A fails too, stop and
investigate. I planted the fault and measured:

    dominium geminum: A CEPIT, B CEPIT

**Both catch it.** The reason is straightforward once seen: A compares
trivia series PER TOKEN, so a second owner shows a series where the
original had none, and A reports `trivia/ante`. The scenario the spec
imagined — where both owners look locally correct — requires the
original to have equal-content trivia at BOTH sites; but then the
emitted bytes match too, and B is equally silent.

**In the round-trip configuration, A is not weaker than B on trivia
ownership.**

B is kept, for a reason that IS true: it is an **independent** oracle.
It exercises `silva_scribere_nodum` over *loaded* trees — a path A
never touches — and it would catch a defect in A itself. A comparator
that wrongly says "equal" is invisible to A by construction and loud
in B.

That the oracles are genuinely disjoint is now asserted directly: a
planted `longitudo` mutation makes **A fail and B pass** (longitudo is
compared but never emitted). Without that assertion, "both oracles
green" could equally mean "we ran the same check twice".

### Method note

Three times this arc I guessed which field would diverge and was wrong
three times. Printing the comparator's own `campus` and `via` settled
it in one run, every time. The out-struct earned its existence: with a
boolean, each of these would have been a bisect.

---

## 2026-08-20 — T7, amalgam integration. **M1 COMPLETE.**

`silva/amalgama/silva.h` extended with the arbor public surface in
vanilla C89 (`int`/`const`, `SilvaPiscina*`, no latina). hospes 38/38.

### The header edit tripped the mechanism the header documents

`silva.h` says a discrepancy between it and the bodies "breaks
compilation itself (self-correcting drift)". First attempt proved it
— but from an angle I hadn't predicted. The amalgamator keeps a
`CADENDA_TYPEDEF` list of types `silva.h` OWNS, and drops the
equivalent typedefs out of the internal headers so each exists once
per translation unit. My four new types weren't on it, so both copies
landed and the amalgam refused with `typedef redefinition`.

So the hand surface for a new public TYPE is two places, not one:
the declaration in `silva.h` AND the name in `CADENDA_TYPEDEF`. Note
the enum needed it too — C89 forbids duplicate *enumerators*, not
just duplicate typedefs.

### The declarations were unverified, and nearly shipped that way

`silva.c` does **not** include `silva.h`. Nothing in the build
compiles the hand-written prototypes against the amalgamated bodies.
So adding declarations and watching the gates go green would have
proven only that the header *parses* — the whole arbor surface could
have been mis-declared and every gate would still have passed.

Cure: `hospes.c` now runs a full arbor round trip **through the
header** — seal, writer, reader, comparator — and links against the
amalgam. That is the gate at birth this surface was missing.

Calibrated rather than assumed: declaring `grammatica` as `int`
instead of `const char*` makes hospes fail to compile, naming the
parameter. Reverted, compiles clean.

**Honest limit of this gate**: it catches mismatches a CALLER would
notice (wrong type at a call site, missing declaration). It does NOT
catch a pure ABI mismatch that still type-checks, because C never
cross-checks signatures between translation units. That residue is
inherent to the amalgam shape, not something this gate chose to skip.

### M1 scoreboard

| | |
|---|---|
| M1 gate | 78 files, 281 subtrees, **270/281 both oracles**, zero divergences |
| refusals | 11, all `lexema non-FONS` — the documented expansion boundary |
| unit tests | 683 assertions in `probatio_silva_arbor` |
| suite | silva 44/44 |
| amalgam | standalone + hospes 38/38 + nm-intersection 0 + censura |

### What M1 does NOT do, stated plainly

- **Whole files.** A file carries directives, non-taken conditional
  branches and tail trivia that live outside the node tree. Those need
  the `<parsura>` form — M2.
- **Macro-bearing subtrees.** Macros ARE in the tree (expanded tokens
  with origo chains); what M1 can't serialise is the origo chain,
  because it references tokens that are not in the tree. The
  `<origines>` pool is M2, and `origo=` is already reserved.
- **Practical reach**: because latinized code expands `si`/`per`/
  `redde` through macros, most subtrees of rhubarb's OWN sources would
  refuse. The 11/281 rate reflects the roundtrip corpus being plain C.
  This makes the origo pool the highest-value part of M2, not a
  nice-to-have.

---

## 2026-08-21 — M2 §2: the `<parsura>` full-document form (T1–T6b)

Both gates are now at **full coverage**:

| gate | before M2 | after T6b |
|---|---|---|
| M1 subtrees | 270/281 both oracles, 11 refused | **281/281 both oracles**, 0 refused |
| M2 whole files | — | **78/78 byte-exact**, 0 divergences |

Shipped: `bcd8a80f` T1 envelope · `117a58b8` T2 directives+hoist ·
`d1799aed` T3a boundary measured · `6171d8eb` T3b gap-aware cursor ·
`87b1eee3` T4 comparator · `e70f2588` T5 gate · `befe0d71` T6 origin ·
`913299ee` T6b anchor chain.

### THE ANCHOR LAW, which has now failed three times in three faces

**An anchor is where EMISSION BEGINS — not where the token is.**

1. **M1 T6** — emission starts with the token's *leading trivia*, so an
   anchor taken from the token slid everything by the indentation.
   178 divergences.
2. **M2 T5** — the identical bug, reintroduced, because I wrote a
   *fresh* anchor helper for the parsura writer instead of reusing
   M1's. 4 files, deltas equal to each file's leading comment block.
3. **M2 T6b** — emission of an *expanded* token starts at its
   stratum-0 **invocation**, and `silva_token_ex_expansione`
   (`silva_token.c:78`) copies the def-site's coordinates onto the
   expanded token. So a node whose first token is expanded anchored
   into *another file*. In `latina.h`, `nomen insignatus brevis i16;`
   at line 349 anchored at line 39 — where `#define nomen typedef`
   lives.

The general lesson from face 2 is the one worth keeping: **a new
surface that re-implements an old concept re-earns the old concept's
bugs.** The reasoning should have been copied, not re-derived.
`_parsura_lexema_emissionis` now resolves the chain and is used by
*both* anchor sites.

### Positions: derive where derivation is possible, carry where it is not

M1's law ("a canonical document must not be able to lie") stands, but
M2 found two places where derivation is *impossible*, not merely
inconvenient:

- **Top-level children need their own anchors.** Non-tree content
  (degraded-region laminae) can sit *inside* a node's byte span, so
  document order cannot express byte order. The envelope still carries
  none — a file is its own beginning.
- **Expanded tokens carry `b`/`linea`/`columna`, and only they do.**
  Their coordinates are the def-site's — a position in a *different
  file*, absent from this byte stream entirely. Carrying is honest;
  deriving would be invention.

### Gap-aware cursor, and why two passes

The tree emits its bytes contiguously, but its tokens are *interrupted*
by directives and degraded-region laminae. Anchors fix the reinserenda;
they cannot fix the tree, whose own tokens are what gets interrupted.

`ArborCursor` now carries a gap list and `_positiones_lexematis` skips
gaps the cursor has reached. All derivation funnels through that one
leaf, so it is a single change point; subtree documents pass an empty
list and are unaffected. A gap is **not** a carried position: it is
computed from deriving the lamina itself (start = its anchor, end =
where the cursor stands afterwards).

This forces **two passes** in the reader: laminae *create* the gaps, so
they must be read before the tree is derived.

### An expanded token holds no bytes

`silva_scribere.h:22` — a non-FONS token doesn't emit itself; its
stratum-0 invocation does. Derivation must mirror that: place the
invocation, skip the expansion. Otherwise the cursor advances twice.

**Guard-order trap that cost a cycle**: I placed that branch *after*
the existing "already placed" guard. Since expanded tokens now *carry*
a position, the guard fired and the invocation was never placed. The
delta was exactly 11 bytes = `len("MASCA_ASCII")` — the discrepancy
named its own cause. **Adding a carried value can silently disable a
guard that assumed it was absent.**

### Document order ≠ loaded structure

The tension between "directives should read where you wrote them" and
"the loaded tree must stay structurally pure" dissolves once these are
separated. A `<directiva>` element is distinguishable from a node, so
the document interleaves and the loader **hoists** them back into
`parsura->directivae`. Purity is a property of the loaded tree.

### Conditional regions: only the DEGRADED case was work

Measured across five shapes before writing anything: a **woven**
region emits its lines from the tree (single owner), so
`_regiones_colligere` skips it and M1's node path already handled it.
**Three of five shapes were already green.** Only degraded regions —
which own their bytes as reinserenda — needed the document elements
and the region-tree reconstruction.

`est_sumptum` is not carried: a taken arm appears as nodes, an untaken
one as `<cruda>` tokens, so the shape already states it. VISIO also
holds that the default track is one configuration among many.

### Named representational gaps (not silent)

- **Woven regions do not reappear in the loaded `expansio->regiones`.**
  Their laminae point at the *same* tokens the conditionalis node
  carries; rebuilding them separately would break identity, which the
  dual law forbids. Closing this needs `#id` references **across
  sections** — the machinery T6 introduced for origo. The comparator
  therefore compares the flattened sequence of *degraded* regions:
  what the format actually represents.
- **`expressio` is not carried** — its bytes already live in
  `directiva`, and `_regiones_colligere` never reinserts it.
- **Caecatio (hidesets) excluded** — reconstruction never consults
  them. Reserved as `cauda="#c7"`.
- **Included-file documents refuse by name** (`cauda plagulae inclusae
  nondum lata`) rather than silently omitting a tail.

### A hypothesis I published and then refuted

`befe0d71`'s message claims the residual divergence came from expanded
tokens *sharing* def-site objects. Plausible, and **wrong** —
`silva_token_ex_expansione` allocates a fresh token per expansion.
One targeted read refuted it; had I compacted first, the next session
would have inherited it as a lead. **A hypothesis written into a commit
must say "unproven", and should be settled before compaction, while
settling it is still cheap.**

### Test-discipline notes

- **A planted fault that breaks the build proves nothing.** My first
  T2 fault removed a comparison and left variables unused under
  `-Werror`; it had to be re-planted as an *inverted* comparison.
- **Calibration says which test does the work.** Dropping `<cauda>`
  failed exactly one of three round trips — the trailing newline after
  `;` rides as `post` trivia on the semicolon, so only *surplus* blank
  lines reach EOF. Two of the three tests were decorative for tails.
- **A guard around an assertion is a silent opt-out.** A vacuity check
  landed inside `si (causa != NIHIL)` — the branch that only runs on
  failure. It compiled, passed, and never executed. Only the assertion
  *count* not moving revealed it; root cause was a `python .replace()`
  that matched nothing (the formatter had re-aligned lines) while still
  printing "patched". **Assert that replacements land.**
- **The formatter reformats between edits**, so `Edit`/`replace`
  anchors go stale constantly. Line-range replacement with an
  assertion is the reliable shape.

---

## 2026-08-21 (later) — T7: the latinized tier

The plain-C corpus (78 files) is **pure C**: no `latina.h`, so no expanded
tokens at all. Everything M2 proved, it proved on code that looks nothing
like ours. T7 points the gate at `lib/*.c` — 154 files, **all** latinized,
**129,884** tokens expanded from `latina.h`.

**Byte-exact went 0/154 → 140/154.** Write refusals 0. Read refusals 32 → 1.

### The apparatus gate had a hole in it

Built first, per spec §6.5, then calibrated by **withholding** the closure.
That calibration paid for itself immediately: the first version counted
*any* EXPANSIO token, which let **59 of 154 files pass** while `latina.h`
was never opened — because any file with a local `#define` of its own
(`XAR_FACTOR_DUPLICANDI` in `lib/xar.c`) produces expansions with no
closure supplied. It would have certified 43 files as clean round trips of
latinized code that was never latinized.

The gate now counts only expansions whose **def-site is latina.h**
(`origo.datum.expansio.corpus->fons_index`). Re-calibrated: 154/154 caught,
zero false passes.

**Law: a gate that measures a proxy for the thing measures nothing. "Did
any macro expand" is a proxy; "did THIS file's macros expand" is the fact.**

The `praebere` parameter on `_plagulam_probare` is a permanent calibration
seam. Its polarity is safe: the wrong setting makes the gate SCREAM, not
go quiet.

### Four defects, none findable by unit tests

**1. Asymmetric default on `f` (122 files).** The writer omits a token's
`fons_index` when it equals `scriptor->ancora_fons`; the reader restores an
absent one to `lector->fons_ordinarius` = `fons-princeps`. **Two different
defaults for the same absence.** With one source those are the same number
— which is exactly why 78/78 stayed green with the bug present. Supply a
closure and `fons_princeps` is the LAST index (provided sources take
earlier ones), so every token of source 0 silently became the main file.

Second layer: setting `ancora_fons` at scriptor init was **silently
clobbered** by `_numerare_lexema`, which re-anchors from the first token of
the walk — and in a latinized file that token comes from a *header*. The
assignment had to move to after `_numerare_valorem`.

**2. Trivia carried no source (7 files).** `_trivium_legere` creates them
with `fons_ordinarius`, so a header's newline became a main-file newline
and the emitter filed it under the wrong file. Fixed by **inheritance**,
not by carrying: trivia demonstrably live in their token's file, so M1's
"derive where possible" holds.

**3. The origin path didn't unwrap fragments (31 files).** An invocation is
usually written `<#lexN>`; `_origo_legere` handed the wrapper straight to
`_lexema_legere`, which looked up `#lexN` in the token registry. The tree
walk already called `_fragmentum_aperire`. **Third time this session a new
surface re-earned a problem the old surface had solved.**

**4. `extenta` was never built — the sixth closure element.** Spec §0.1
names six things the emitter reads; I shipped five. `expansio->extenta`
(function-like invocation laminae) was not among them, and no plain-C
fixture could expose it.

`SHA1_ROL(a, b)` covers bytes `[nomen..')']`, but the invocation token is
the **name alone**. Arguments, parens and commas are consumed — referenced
by *no* tree token — so they cannot be derived. Carried, per M1's law.

**Arithmetic named the cause; guessing had failed twice before it:**

| file | invocation | uncounted | delta |
|---|---|---|---|
| `hospitium.c:677` | `ROUTA_METHODUS_BIT(m)` | `(m)` = 3 | **3** |
| `friatio.c:145` | `SHA1_ROL(w[i-III] ^ …, I)` | 55 | **55** |

Object-like macros hide this completely: for `si`/`NIHIL` the name *is* the
whole invocation. Built as `<extentum>` inside `<expansio>`, written once
per invocation (later uses transclude), and **derivation walks the lamina
too** — emission and derivation must traverse the same bytes, so
`ArborCursor` now carries the expansio.

M1 subtree documents deliberately do NOT carry extents: the M1 gate
compares silva output against silva output, so it is symmetric. Named
limit, not a hidden one.

### Also

- **Healed silva's POSIX lexicon** (`popen`/`pclose` in `systema_posix.h`'s
  stdio section) rather than pinning them — that section's own comment
  documents this same class from the 2026-08-03 glibc sweep.
- `ARBOR_DEFIGERE=<path>` on the plagula test dumps the STML of a
  read-refusing file and prints the refusal's STML line. That is how the
  fragment bug was found: line 1,816,470 of a 1.8M-line document.

### Still open (14 files, four classes — all shapes, not guesses)

| class | files | shape |
|---|---|---|
| region gaps | 3 | `via 55.1`; `#ifndef X / #define X / #endif`. `tempus.c` delta 36 = exactly that `#define` line + newline |
| in-function regions | 4 | deep paths; `uuid.c` `#ifdef __APPLE__` with both arms retained, delta 112 |
| coordinate space | 2 | `tcp_posix.c`, `imago_opus.c`: token from a header (`f=2`, princeps 6), byte close but line wildly off (312 vs 404) |
| directive count | 3 | `directivae/numerus` — NEWLY EXPOSED by the extenta fix, not necessarily caused by it |
| + | 1 | `initium-lineae`; 1 transclusion refusal (`arbor2_glr_tabula.c`, the generated table — biggest doc in the corpus) |

**Read these counts as a FLOOR, not an estimate.** The comparator reports
only the first divergence per file, so every fix so far has revealed a
further layer rather than closing the count: `lexema/fons` became
`lexema/offset`; clearing 31 read refusals turned them into 20 new
divergences.

### The separating oracle — whose gap was it?

Fran asked whether the function-like macro gap was OUR projection or
silva's AST construction. That deserved a measurement, not a reading of
the code, so the plagula test now emits **directly** from the original
parsura (`parsare -> silva_scribere_fontem`, no STML) beside the full
round trip:

| corpus | silva alone | through STML |
|---|---|---|
| plain C (78) | 78/78 | 78/78 |
| latinized (154) | **153/154** | 140/154 |

**The gap is ours.** Silva records invocation extents correctly
(`silva_expandere.c` populates `exp->extenta`; `silva_scribere.c`
consults it) and emits our own latinized code at 153/154. Every
divergence the STML round trip shows is the projection layer losing
something silva already had. Spec §0.1 had named `extenta` as part of
the closure; the miss was in the build, not the design.

**Keep this oracle permanently.** Without it, a core defect and a
projection defect are indistinguishable in one number — and the whole
value of the M2 gate is that its expected value is external to silva.
This splits the *other* axis: silva-vs-us.

**New finding it exposed:** `lib/arbor2_glr_tabula.c` (1.5 MB, the
generated GLR table) fails silva's **own** direct emission —
`successus=0`, output length 0. That is a silva-core defect, not a
projection one, and it is the same file that fails our transclusion
path. Its size makes it the likely pathological case. Recorded, not
fixed — it is out of T7's scope and belongs to silva proper.

### T7 tail: the gap index, and one hypothesis that was wrong

**Gap index re-seek (9 files, 140 → 149).** `lacuna_proxima` was a
**monotonic** index — correct only if derivation were one linear walk in
byte order. It is not: top-level children stand in **tree** order, and
header content sits between them (`declaratio b=8135 linea=403` from
another file), so an anchor moves the cursor *backwards* too. Any gap the
index had already passed was lost forever.

Measured on `tempus.c`: the region `#ifndef M_PI / #define M_PI … /
#endif`. The `#define` directive registers gap [93,129) correctly, but by
the time the `conditionalis` was read at anchor 79 the index sat beyond
it — so `#endif` derived at 93, the `#define`'s own offset. Delta 36 =
exactly that line plus its newline. The plain corpus cannot show this: with
one source, top-level children are *already* in byte order.

**Per-source gaps — correct, but NOT the fix I predicted.** I reasoned that
the last 4 files (all sharing path `16.0>21.1 index 12`) were a main-file
gap being applied to a header token, since `Piscina` from `f=2` landed 835
bytes ahead. Added `fons` to `ParsuraLacuna` and made the skip require a
source match. **The count did not move.** Verified the object rebuilt, so
the hypothesis was simply wrong.

Kept anyway, labelled for what it is: **correctness by construction, not a
measured fix.** A gap is a byte range in a specific file; applying one to
another file's token is wrong by definition, and the old code had no
guard. But it fixed nothing observable, and the next session should not
read it as having addressed the remaining four.

**The remaining 4 need a fresh diagnosis.** What is known: all four share
one path, the token is `Piscina` in a shared header at line 11 col 17,
and the loaded position is line 30 col 17 — *same column, 19 lines on*.
Same-column strongly suggests the derived cursor landed on a **different
prototype's** `Piscina` in an aligned header, i.e. an over-advance of 835
bytes inside that header, not a mis-anchor. That is a lead, not a
conclusion — the last two guesses here cost a build each, and printing the
field settled it both times.

**Score at the end of T7:** plain C 78/78 both oracles; latinized
**149/154 through STML**, against **153/154 for silva alone**. So our
projection now loses exactly 4 of the files silva itself can round-trip,
and `arbor2_glr_tabula.c` (1.5 MB generated table) is silva's own defect —
Fran's read is arena exhaustion, plausible and untested.
