# officina_regio worklog

## 2026-07-10 — natus (M2a Chunk-of-one)

The owned region: one 64GB fixed-base mmap reservation at
0x300000000000 (lazy commit — reservation is free on macOS, physical
cost = touched pages). Areas: globalia 8GB | stiva 4GB | acervus
~52GB. regio_generare REFUSES a non-hint base (NIHIL, never silently
different — determinism before convenience); probatio pins the base
VALUE and same-base-across-create-cycles.

Heap allocator: first-fit free list, 16-byte headers, free-flag in
the size's low bit (sizes are 16-multiples). LIFO reuse is PINNED in
the probatio (free-then-alloc same size = same pointer — allocator
determinism). Split when remainder holds header+16. NO COALESCING
v1 — deliberate: corpus malloc callers are concentrated behind
piscina (few, large allocations — fundus-libc); named park, M2b/M2c
counters (numerus_allocationum/liberationum/apex already here)
decide if it ever matters.

malloc semantics kept INDETERMINATE (never zero-fill); custodia flag
poisons 0xAA on alloc / 0xDD on free — recognizable, not zero.
liberare VALIDATES always (in-heap, aligned, sane header, not
already free) and returns b32 — machinula turns FALSUM into an
honest halt.

Landmine log: `externus` = latina's `extern` — 8th macro firing
(probatio variable renamed `alienus`).
