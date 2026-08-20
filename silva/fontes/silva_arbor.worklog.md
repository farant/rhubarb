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
