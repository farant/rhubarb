# Knotapel Research

Literature surveys, prior art, and open directions for the DKC (Diagrammatic Knot Computation) research arc.

## Contents

| File | Description |
|------|-------------|
| `knot-quantum-geometry-correspondence.md` | Master theorem hierarchy (A1-E5): proven results from knot theory, TQFT, and geometry organized by derivation order |
| `dkc-integration-notes.md` | What the correspondence theorems mean for forward DKC — rigidity, Vassiliev hierarchy, testable predictions |
| `literature-survey-demo27.md` | Full literature survey from Demo 27 (complex reverse DKC): CVNNs, Kauffman bracket, skein algebras, gauge symmetry, TQNNs — 27 papers |
| `literature-survey-demo29.md` | Full literature survey from Demo 29 (forward DKC): cyclotomic+NN, lattice codes, bracket exact arithmetic, TL algebras, algebraic NT codes — 22 papers |
| `prior-art-index.md` | One-line-per-paper quick reference across all surveys, with relevance tags and deduplication |
| `open-directions.md` | Ranked priorities, testable predictions, and future demo ideas |

## The Core Result

The Z[zeta_8] lattice of Kauffman bracket values at A = e^{i*5*pi/4} is **saturated** with XOR solutions (~35% of all triples work). 65,536 valid triples from 64 distinct bracket values. Forward DKC computes XOR without training — exact integer arithmetic, zero floating-point.

## Novelty Claim

Nobody has connected these four fields:
- **Aizenberg**: roots-of-unity neurons (prescribes structure) — we show it's emergent
- **Habiro**: bracket values at roots of unity ARE cyclotomic integers (math foundation)
- **Nazer-Gastpar**: algebraic integer lattices compute functions (info-theory frame)
- **Abramsky**: TL diagrams encode computation (categorical bridge)

The question "can topological invariants be compiled into neural network weights?" has no direct precedent in the literature.
