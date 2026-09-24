# sors.worklog.md

## 2026-09-23 — natus

`sors` (Latin "lot", as in casting lots): the house's seeded, deterministic
pseudo-random numbers. Before it there was no library, only ~20 inline
copies (the classic ANSI LCG `x*1103515245+12345` in six materia fuzz suites,
partitio, flatura, tempus; xorshift32 in silva's robustness fuzz; LCGs in
knotapel demos) plus two OS-random sources for ids (moneta, uuid) that are
secure but NOT seedable. Survey: tabularium note …F5PV89 in the region
"random number generator library". Waiting consumers: pictor's spray can
(the stroke log is REPLAYED and checkpoints are verified by re-projection,
so a spray stroke must carry a seed, not fresh randomness), procedural
assets (ludus decision II: procedural is the default asset format), fuzz
suites.

Choices (prior art, well studied): PCG32 for the stream (64-bit state,
32-bit output, O'Neill); SplitMix64 to expand a user seed into a well-mixed
PCG state (Vigna), so seeds 1, 2, 3 do not start correlated. `series`
selects an independent PCG stream (the increment). Integer-only, so output
is bit-identical on every machine — the property replay depends on.
`Sors` is a plain 16-byte value: no piscina, no global state.
`sors_derivare` is PURE on the parent (constans): child = f(parent state,
key), so deriving stroke 7's stream never depends on how many other
streams were derived first. Ranges by Lemire's multiply-and-reject (no
modulo bias). Floats from integer bits (53 for f64, 24 for f32), never
rand()/RAND_MAX. `sors_lcg_classicus` keeps the legacy LCG under a name so
the fuzz suites can adopt the library without their generated cases
changing (migration is a separate batch job, not this commit).

Reference vectors come from an INDEPENDENT Python implementation, itself
checked first against the canonical pcg32 demo (initstate 42, initseq 54
-> 0xa15c02b7 0x7b47f409 ...) and SplitMix64's first output for seed
1234567 (6457827717110365317) — two sides, differential.
