---
id: LIM0008
title: "macOS only (no Windows/Linux porting planned)"
type: chosen
source: "CLAUDE.md (rhubarb project)"
impacts: []
related: [LIM0006]
---

Development and deployment target is macOS on Apple Silicon (M2 MacBook) only. No Windows or Linux porting is planned in the near term.

Why:
- Single developer, limited bandwidth. Porting is a time sink that doesn't advance the core research.
- M2 is the actual hardware the model needs to run on, so optimizing for it specifically is appropriate.

Implications:
- Can use Apple Accelerate framework or Metal for SIMD/GPU acceleration if needed, though this creates platform coupling.
- Code should still be behind a platform independence layer so porting is possible later without rewriting core logic.
- File I/O, networking, threading (if any) should use POSIX APIs where possible since macOS is POSIX-compliant, which eases future Linux porting.
- Memory model assumptions are safe for Apple Silicon's unified memory architecture (CPU and GPU share memory).
