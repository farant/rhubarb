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
