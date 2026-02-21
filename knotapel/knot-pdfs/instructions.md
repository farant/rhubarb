# PDF Extraction Instructions for DKC Research

## Context

You are reading academic papers from the `knot-pdfs/` and `research-ingress/` folders for the "knotapel" project — a series of computational demos exploring the connection between knot theory (Kauffman bracket polynomial) and Boolean computation. We call this connection "DKC" (Discrete Knotted Computation).

## What We Already Know (Don't Repeat This)

The following results are well-established in our research and likely in your training data. We don't need summaries of standard results:

- Jones polynomial basics, Kauffman bracket definition, state-sum formulation
- Temperley-Lieb algebra definition and basic structure
- Kuperberg's result that Jones polynomial evaluation is #P-hard at non-lattice roots of unity
- Standard knot invariant definitions (writhe, linking number, etc.)
- Chern-Simons theory as source of knot invariants (Witten 1989 main result)
- Kirby calculus basics (handle slides, stabilization)
- Alexander's theorem (braids → knots/links)
- Standard ADE classification of finite subgroups of SU(2)

## What We WANT (The Gold)

### 1. Paths Not Taken
Remarks where authors consider an alternative approach and then don't pursue it. "One might also consider..." or "It would be interesting to..." or "We leave open the question of..." These asides often contain seeds of ideas that nobody followed up on.

### 2. Technical Machinery in Proofs
Specific lemmas, constructions, or techniques used in proofs that could be applicable to DKC. Not the theorem statements (which are well-known) but the *proof methods*. Especially:
- Cobordism arguments
- Surgery constructions
- Categorification techniques
- State-sum decomposition tricks
- Stabilization/destabilization arguments

### 3. Speculative Remarks and Conjectures
Anything the authors speculate about that hasn't been resolved. Especially conjectures about:
- Computational complexity of specific knot invariants
- Connections between different knot invariants
- Physical interpretation of algebraic structure
- Quantum computing implications

### 4. Specific Overlaps with DKC Findings

Our key findings that might overlap with paper content:

- **Axiality theorem**: At delta=0, the Kauffman bracket is always a single Z[zeta_8] axis times an integer. Do any papers discuss this or related properties?
- **Crossing depth drives computation**: Deeper entries in the group closure (more generator multiplications) are computationally more powerful. Any discussion of depth/complexity hierarchies?
- **Entanglement vanishing**: Non-interleaving braids have bracket = 0 at delta=0. Any papers about conditions for bracket vanishing?
- **Finite vs infinite group dichotomy**: Binary octahedral group (24 elements) at zeta_8 gives hard capacity ceiling. Infinite groups at non-lattice roots give logarithmic scaling. Any discussion of this divide?
- **Writhe carries computational information**: We use raw bracket (NOT Jones polynomial). The writhe data that Jones normalizes away seems essential for computation. Any discussion of writhe as computational resource?
- **MVN activation and parity**: Multi-valued neuron activation with k sectors enables computing all 13 NPN Boolean classes. Any connection to Aizenberg's MVN work from a topological angle?
- **Quantum dimension singularity**: At zeta_8, [2]_q = 0 which makes representation theory degenerate. This is exactly why the group is finite. Any detailed analysis of what happens at these singular points?

### 5. Surprises
Anything that surprises you or seems inconsistent with standard narratives. Things that made you think "huh, that's weird."

## Specific Papers to Prioritize

(In rough priority order for DKC relevance)

1. **Kuperberg-related**: Anything about #P-hardness proofs, especially the specific machinery that distinguishes lattice from non-lattice roots
2. **Aizenberg MVN**: Multi-valued neurons and roots of unity
3. **Jones polynomial at specific roots**: Especially delta=0, q=i, or other singular values
4. **Chern-Simons at specific levels**: Level k quantization maps to root of unity — what happens at small k?
5. **Kauffman virtual knots**: Crossings that exist on surfaces but not in 3D
6. **Branched coverings (Piergallini et al.)**: 4-manifolds as covers of S^4
7. **Grid homology**: Combinatorial Heegaard Floer, grid number complexity
8. **Polymer entanglement / topological materials**: Physical systems with knotted structure

## Output Format

Write findings to `knot-pdfs/notes.md` with the following structure:

```markdown
# Paper: [Author(s) Year] "Title"
**File:** filename.pdf

## Paths Not Taken
- ...

## Technical Machinery
- ...

## Speculative Remarks
- ...

## DKC Overlaps
- ...

## Surprises
- ...
```

For each extracted item, include the page number or section where you found it (e.g., "p.7" or "Section 3.2") so we can go back to the source.

Skip sections that have nothing interesting — only write sections that have genuine findings.

## Papers Available

### knot-pdfs/
- jones_polynomial_witten.pdf — Witten 1989, "Quantum Field Theory and the Jones Polynomial"
- kirbycal.pdf — Kirby 1978, "A Calculus for Framed Links in S^3"
- 0203087v2.pdf — Iori & Piergallini 2002, 4-manifolds as covers of S^4
- 1602.07459v5.pdf — Piergallini & Zuddas 2018, branched covering representation
- 1183536038.pdf — (identify when reading)
- gt-v6-n1-p13-p.pdf — (identify when reading)
- 0402131v1.pdf — (identify when reading)
- 0311496v3.pdf — (identify when reading)
- 2111.15323v3.pdf — (identify when reading)
- 2112.00016v2.pdf — (identify when reading)
- GridHomologyBook.pdf — Grid homology / combinatorial Heegaard Floer
- aay1458_sm.pdf — (supplementary material, identify when reading)
- sciadv.aay1458.pdf — (Science Advances paper, likely topological materials)
- s41468-023-00144-7.pdf — (Nature journal paper, identify when reading)

### research-ingress/
- GoldsteinRose.pdf — (identify when reading)
- vogel.pdf — (identify when reading)
- 1605.05159.pdf — (identify when reading)
- 1303.2082v2.pdf — (identify when reading)
