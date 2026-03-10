---
id: LIM0007
title: "No external dependencies"
type: chosen
source: "CLAUDE.md (rhubarb project)"
impacts: []
related: [LIM0005, LIM0006]
---

No npm, no pip, no cargo, no package managers, no third-party libraries. Every library is written in-house as part of the rhubarb monorepo.

Why:
- Supply chain attacks are becoming real (summer/autumn 2025 context).
- Dependencies introduce "rot" — external code changes on its own schedule, breaks APIs, introduces vulnerabilities, requires maintenance attention.
- Programs are machines that don't experience entropy unless you opt into it through dependencies.
- Once you make a library you have it forever.

Implications:
- Must write all math libraries (matrix operations, linear algebra) from scratch.
- Must write all data structures from scratch — already have: chorda (strings), piscina (arena), tabula_dispersa (hash table), xar (VLA), internamentum (string interning).
- Must write training infrastructure (backprop, optimizer, data loading) from scratch.
- Must write any serialization (already have nuntium, protobuf-like).
- The rhubarb monorepo accumulates reusable C89 libraries over time, building a comprehensive self-owned toolkit.
- Platform independence layer isolates OS-specific code so portability is possible later without rewriting tightly coupled code.
