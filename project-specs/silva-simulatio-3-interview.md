# Silva Simulatio ③ — Post-Simulation Interview (Raw Q&A)

*2026-07-02. Interview on the open questions from silva-simulatio-3.md (annotated
grammar → generated construction) plus two questions that arose writing the sim.*

---

## Q: S21, the central decision: what do generated constructions build? (arbor2's union node charges every node the size of the biggest arm — the 21-field declaratio — so a binary-op node wastes ~120 bytes.)

**A:** Uniform header + slot array. Fixed header (genus, span, fons, standard-tag,
pater, semantic fields) + per-genus tagged slot array described by generated layout
tables. Nodes are SMALLER than the union; children()/selector-tags/STML
serialization become table-driven for free; type safety via generated checked
accessors (silva_binarium_sinister(n)). The code-as-data philosophy applied to the
AST itself.

## Q: S26 found pater pointers can't be assigned during speculative GLR construction (shared subtrees between live forks — a latent bug in arbor2/lapifex today). What's silva's parent-pointer strategy?

**A:** Post-accept fixup pass. Constructions stay pure; after a top-level slice
commits, one walk assigns pater along the committed tree (canonical spine for
AMBIGUUS; re-canonicalization re-runs local fixup). Queries keep O(1) parent
access; cost is one linear walk per slice.

## Q: S24: per_clausula's 8 grammar variants collapsed into one genus with optional slots — real node in the tree, or unpacked into the for node's slots?

**A:** Real node in the tree. Honest structure: selectors can match per-clausula,
the formatter formats it as a unit, grammar factoring maps 1:1 to tree shape.

## Q: S18: the annotation syntax everything downstream hangs on — taste check before it freezes:

**A:** symbol@slot inline.
`<productio genus="binarium">expr@sinister PLUS@tok_operator term@dexter</productio>`
— mapping rides the symbol, so editing the RHS carries annotations along (the
drift-proof property, R3). Terse, readable at production width.

---

## Q: S30: structural genera (CONDITIONALIS, COMMENTUM, DIRECTIVUM, ERROR, AMBIGUUS) need slots/tags in the same registry. Where are they declared?

**A:** <genera-extra> in the grammar file. One file is the single source of truth
for the entire genus registry — grammar productions and structural genera side by
side, one generator input, impossible for the two sources to skew.

## Q: The generated files are committed and Fran reads all code in this repo. What's the generated-code style contract?

**A:** Fully readable, Latin-commented. Generated code follows the same standards
as hand code: Latin comments citing the source production (id + rule), aligned
tables, readable dispatch. Diffs are meaningful in review; you can actually read
your parser.

## Q: Production id attributes (id="expr-plus") — optional or mandatory?

**A:** Mandatory on node productions (pass-throughs exempt). Generator enforces
uniqueness. Every diagnostic, generated comment, and conflict report names
productions stably — the C89 grammar is ~300 productions and unnamed ones WILL
drift in conversation and worklogs.

---

## Q: Anything else for the construction layer? (Prompts offered: verify the latent arbor2 cross-fork pater bug, and whether solarium widgets consume nodes through accessors only vs raw slot access inside silva's own modules.)

**A:** i think that all makes sense. i think as an IDE solarium is a 3d environment
so it is going to be a combination of: code as a database + code as a 3d
representation of its structure + literate programming. there is also a layer
called speculum as a kind of additional meta language i can share just so you have
an idea of what that looks like, it might suggest what some usage patterns might
look like, but i can probably do that after you document this if you want

**Context recorded:** solarium = 3D IDE environment combining (1) code as a
database, (2) code as a 3D representation of its structure, (3) literate
programming. This directly reinforces the S21 decision — the slot-array node with
generated layout tables IS the code-as-database representation (every node
generically enumerable, table-described, serializable). Speculum (a meta-language
layer) to be shared in a future session; expected to suggest usage patterns for the
query/transform surface. The two prompt questions (arbor2 bug verification test;
accessor-only discipline for external consumers) were left open — not blocking,
revisit when the accessor surface is generated.
