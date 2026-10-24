# xar — worklog

## 2026-09-02 — RP Part II: the header was the cost

Measured with computus before touching anything: lib/stml.c parsed
into 457 MB handed out, 635 MB committed, 1.14 M allocations. The Xar
header was 576 bytes, 512 of them a 64-entry segment table inscribed
in every Xar, plus a 32-byte debug name nothing read; with a first
segment of 16 pointers, a one-element pointer Xar cost 704 bytes for
8 bytes of payload, and every token carried two of them for trivia.

Changes: the segment table is now allocated after the header in the
same allocation, sized at creation (`segmenta_maxima`): 24 entries by
default, 64 via `xar_creare_magnum`. Segment k holds first × 2^(k-1),
so the default with first 4 caps at 33.5 M elements and refuses
LOUDLY past it (stderr names xar_creare_magnum; xar_addere returns
NIHIL) — Fran's "big Xar" door, instead of a global shrink. First
segment 16 → 4 (10,395 of 11,010 trivia Xars held one element). The
debug name is gone. Header 576 → 48 bytes.

Same inputs, old headers on disk so the closure text is identical:
stml.c 457 → 150 MB handed out, 635 → 160 MB committed, idle 178 →
10 MB, parse 177 → 152 ms; json.c 127 → 41 MB; piscina.c 29 → 9.7 MB
and 18.5 → 9.6 ms. Allocations rose 3% (smaller first segments split
more often). Output byte-identical on 154/156 lib files via arbor.sh;
the two that differ are xar.c and piscina.c themselves.

Trap worth recording: the differential must hold the INPUT fixed. My
first after-capture ran with the edited headers in every file's
closure and would have flagged most of the corpus for the wrong
reason; the fix was to drive the built binaries directly with the
HEAD header text restored under a trap.

## 2026-09-02 — locate in constant time

`xar_locare` found the segment for an index by walking the doubling
sequence from segment 2, and `xar_obtinere` paid that walk on every
read: 16% of leaf samples on lib/stml.c after the arena fix. Closed
form: with q = index / first, the segment is highest_bit(q) + 1 and
its size is first << highest_bit(q); the bit is found by a five-step
binary shift (no intrinsics in C89). Same boundaries as the loop by
construction, pinned in the test against a reference walk for first
sizes 1, 3, 4, 7, 8 and 16 through segment count and capacity after
4352 appends, plus a value round trip. Measured (min of 7): stml.c
50.3 -> 43.1 ms, json.c 13.4 -> 11.8 ms, allocation counts identical.

## 2026-10-01 - xar_ordinare: selection sort -> stable merge sort

Found by profiling fabrica (park …AR15 / …6X0): a heal spent ~3 min
of wall time nowhere in its records. `sample` on a one-action heal
(82 s wall, 8 s of action) put ~47 s in `_vestigium_capere` ->
`xar_ordinare`: the footprint snapshot holds the whole tree, and
xar_ordinare was a SELECTION SORT, O(n^2), every access through
xar_obtinere's segment lookup - ~8 s per sort, two per action. The
tree walk itself (getdirentries/stat) was under 1.5 s total.

The old header also called it "stabilis", which selection sort with
swaps is not. Now:
- Bottom-up merge sort, O(n log n), truly STABLE (the right run wins
  only when strictly smaller). Elements are copied out of the segments
  into a contiguous scratch (2n elements, 16-aligned, from the xar's
  own piscina), merged back and forth, copied back; the scratch is
  returned with piscina_notare/piscina_reficere. Consequence, now in
  the header: the comparator must not allocate from the xar's piscina
  during a sort (reficere would free it).
- Fallback when the scratch doesn't fit (fixed-size piscina): stable
  in-place insertion, swaps through a 256-byte stack buffer in chunks,
  so no allocation at any element size. (xar_mutare still allocates
  per swap above CCLVI bytes - the old sort exhausted a fixed piscina
  that way; untouched here, nothing in the sort uses it.)

Tests (red on the old code first): 3000 pairs over 17 keys stay in
insertion order within equal keys, nothing lost, piscina usage equal
before/after the sort (read IMMEDIATELY after - CREDO itself writes to
the piscina, which first made this check fail on old and new code
alike); 40 x 300-byte elements in a 26 KB fixed piscina take the
fallback (asserted: the scratch cannot fit) and come out sorted and
stable. Old code: stability red, and the fixed piscina ran out
("indigentia 26040") through xar_mutare's per-swap allocation.

Effect: a full rebake's unrecorded time ~200 s -> ~17 s. Generated
outputs that changed: the silva amalgam (vendors xar.c; now keeps
piscina_notare/reficere/conari_allocare_ordinatum, excludes
xar_mutare) - ties now break by insertion order, so any generator
sorting equal keys may shift once; the gates compare regenerations.

## 2026-10-15 - clz in xar_locare; what xar really costs in percursus

New `tools/xar_mensura.c` (ns/op, CPU time, best of 5, 4K and 1M
elements; M2, -O2). Before: add 2.9, sequential obtinere 3.0, random
2.6, pointer-element ("lexemata") 3.0, flat C array 0.04. Experiments
(scratchpad): divide->shift -10%, -flto inlining alone -15% (the call is
NOT the cost), `__builtin_clz` for `_bitum_summum` -20..40%, clz + LTO
1.0 ns (3x: the 15-instruction dependent scan was the chain), a walk
segment by segment 0.05 ns (= flat array).

Built: `XAR_BITUM_SUMMUM` = one clz under clang/gcc, the old C89 scan
otherwise (`-DXAR_SINE_INTRINSECIS` forces it; both paths 34/34 sections
of probatio_xar in a scratch build). For a power-of-two first segment
(all house callers: I, IV, VIII, XVI) locare is now h = log2(index),
segment h - p + 1, size 2^h, offset index - 2^h - no divide; other first
sizes (the finium test uses III, VII) keep the divide path, exact
capacities unchanged. After: add 2.2-2.5, sequential 2.4, random 1.8,
lexemata 2.4 ns.

Real workloads: lexer A/B no change (it only appends, ~40 ns/token);
percursus lib A/B within noise. `sample` of percursus explains it: xar
is ~31% of the run (obtinere 16%, numerus 5%, addere 4%, creare 4%), but
62% of the obtinere samples come from LINEAR pointer-keyed lookups -
silva_scribere `_extentum_quaerere` + `_extentum_continens` (every
expanded token of an object-like macro scans all function-like
invocations twice and finds nothing) and
`silva_oraculum_responsum_quaerere`. The lever is an index there, not
xar. Segment-span API (option B) still worthwhile for genuine
sequential loops; ThinLTO would double xar again.

## 2026-10-21 — segment table grows on demand: header 240 -> 64 bytes

Found through the credo v2 migration. The 256 MiB section ceiling cut
off toml_stml's corpora section at 313 MiB. One wild TOML file
(gtk-0.15.5/Gir.toml, 70 KB) costs ~298 MiB in the STML round trip;
156 MiB of that is materia_sedes_verificare, which projects the
document into STML trees twice. A probe
(build/sonda_stml/sonda_proiectio.c) on one plain projection: 33.8 MiB
for 70 KB of input. **65% of it was Xar headers**: 96,382 Xars, 53,342
of them empty, each 48 bytes plus a 192-byte table for 24 segment
pointers. Segment payload actually used: 0.45 MiB. StmlNodus was 25%,
strings and the interner 6%. Every STML element creates two Xars
(attributa, liberi) eagerly.

Change: the first two segment pointers live in the header
(`segmenta_inlinea[XAR_SEGMENTA_INLINEA]`, II), and `segmenta` points
at them. The full table (`segmenta_maxima`: 24, or 64 for magnum) is
allocated from the xar's piscina on the first request for segment 2,
which is the 9th element with first = IV. The inline pointers are
copied over, once (`_tabulam_crescere`). `mensura_tabulae` holds the
current table length. Reads that can reach past it (xar_locare's two
non-first paths, the allocate-up-to loops in obtinere_vel_creare /
addere_multos, xar_addere's check) go through
`_segmentum(xar, k)`, which returns NIHIL past the table. Reads that
only happen after allocation stay direct. No caller changed: the table
was always private to xar.c. The hand copies SilvaXar
(silva/amalgama/silva.h) and OfficinaXar (officina.h) follow field for
field.

Trap: the field was first called `tabula_mensura`. The officina
amalgamator renames by PREFIX, `tabula_` included (meant for
tabula_dispersa_*), and token-level renaming catches struct fields too:
`xar->officina_tabula_mensura`, "no member named". A field name must
not start with chorda_/tabula_/piscina_/xar_/friatio_.

Measured, same file: plain projection 33.80 -> 17.62 MiB (-48%),
decorated 46.71 -> 30.53 MiB (-35%). Of 96,382 Xars only 7 grew the
full table. toml parse arena 2.85 -> 2.42 MiB. toml_stml corpora
section footprint rise 313 -> 184 MiB, under the ceiling without
`gravis`. Worst case: a Xar past 8 elements costs 64 + 192 = 256 bytes
against 240 before (+16). Everything smaller saves 176 bytes.

Tests: new section 'tabula crescens' in probatio_xar:
- an empty Xar costs exactly magnitudo(Xar) of arena;
- 8 elements stay inline;
- the 9th grows the table to 24 with the first segments' values intact;
- magnum grows to 64;
- sparse obtinere_vel_creare(1000) on a fresh Xar.
Plant (growth drops the inline pointers instead of copying): red at
the first post-growth read, green after revert. 35/35 sections, 1310.

What's left in the projection, in order: StmlNodus 152 B x 58k = 48%
now. Lazy attributa/liberi Xars in stml (53k empty) and a slimmer node
(rare fields to a side struct) are the next levers. sedes_verificare
building two full projections is a materia-level question.

Follow-up (same day): the computus golds pin arena numbers, so the
first commit attempt went red on css (`probatio_css_computus`: apex,
commissa and allocationes all diverged), as it should. All 8 golds
were regenerated with COMPUTUS_SCRIBERE=1. Sums over each corpus:

| corpus | apex | commissa | allocationes |
|---|---|---|---|
| silva | -45.1% | 0% | +0.3% |
| oratio | -37.6% | -28.0% | +0.9% |
| md | -37.1% | -40.0% | +0.5% |
| css | -34.4% | -33.3% | +0.5% |
| briar | -28.6% | 0% | +3.0% |
| crusta | -26.7% | 0% | +0.1% |
| html | -26.2% | 0% | +0.5% |
| toml | -16.6% | 0% | +0.3% |

The extra allocations are the one-time table growth of Xars past 8
elements.
