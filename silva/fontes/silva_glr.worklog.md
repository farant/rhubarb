# silva/fontes/silva_glr.c — worklog

## 2026-11-03 — silva budget step 1a: first GSS predecessor inline

Context: ledger question 01M6FTG2WM (data-oriented layout for silva/materia)
and Fran's go for "steps 1-3" (the silva-internal ones, no API change).
A 15 s `sample` of `percursus` parse-only over the whole repo (2991 files,
78 MB) showed the parser instruction-bound (IPC 3.7, ~5100 instructions per
byte) with generic containers on top: xar_addere 13%, xar_obtinere 11%,
allocare_segmentum 6%, xar_creare 3%, tabula_dispersa ~10% (inclusive).
The GLR node routine (`_nodum_processare`) was 27%.

Cause found: `_nodus_creare` made a fresh `Xar` for `praedecessores` on
EVERY GSS node (one per shift, one per reduce). Almost every node has one
predecessor.

Change: `SilvaGSSNodus` carries `praedecessor_primus` inline;
`praedecessores_ceteri` (Xar) is created only when a second, distinct
predecessor arrives. Two helpers, `_praedecessorum_numerus` and
`_praedecessor(nodus, i)`, give the old indexed view in the same order
(first, then the rest in insertion order), so path enumeration — and so
every tree — is unchanged. `_praedecessorem_addere` now takes `glr` (it
needs the piscina for the lazy Xar).

Bench (scratchpad `silva_mensura.sh`, same machine, load 2.6 -> 4.1; the
instruction counts don't depend on load):

| | before (d064d35f) | after |
|---|---|---|
| percursus parse-only: instructions | 400.9 G | 364.0 G (-9.2%) |
| cycles | 108.7 G | 88.3 G (-18.8%) |
| wall | 33.8 s | 27.4 s |
| max RSS | 1128 MB | 714 MB (-37%) |
| percursus -semantica: instructions | 823.3 G | 778.4 G (-5.5%) |
| cycles | 240.2 G | 210.9 G (-12.2%) |
| max RSS | 2762 MB | 2363 MB |
| computus lib/stml.c (min of 7): parse | 28.67 ms | 23.25 ms (-19%) |
| glr phase | 16.10 ms | 12.00 ms (-25%) |
| allocations | 1.04 M | 0.58 M (-45%) |

Cycles fall twice as much as instructions: fewer allocations means less
memory touched (DOD point — allocation is the cost, not the arithmetic).

Equivalence: `silva/aequivalentia.sh -conferre` over lib, silva/fontes,
oratio/fontes, crusta/fontes, materia/fontes (483 files): 482/483
byte-identical, the one being silva_glr.c itself (allowed). percursus
fideles/errores unchanged (2990/2991, 60).

TEST GAP FOUND: no suite covered multi-predecessor nodes. Planted faults
(A: forget `ceteri`; B: drop the dedup on `primus`) were green on glr,
ambiguitas and parsare — those use toy grammars where branches reconverge
through the reduce index, never through a shift-frontier merge. Yet the case
is common: a temporary counter saw ~1.01 M second-predecessor events in one
corpus pass, and fault A changed 406 of the 483 equivalence documents.
Minimal input: `void f(T bytes[16]);` with T unknown — readings (a) type T,
declarator `bytes[16]`, (b) types `T bytes`, abstract declarator `[16]`;
both stacks shift `[` into the same state. New case "FRONS FUSA" in
probatio_silva_c89.c asserts one AMBIGUUS with two PARAMETRUM readings in
order (1 then 2 specifiers). Plants: A red (the suite crashes — the broken
engine, output lost to the crash), C (rest enumerated before first) red on
the order assertion. B is output-neutral — a duplicate predecessor only
yields duplicate paths that the equal-value check discards — so only the
benchmark guards it.

Next (same budget): deterministic LR fast path (step 1b), then the
preprocessor's name-keyed macro table (step 2: intern identifiers, table by
id), then flat arrays / bitsets in the hot loops (step 3).
