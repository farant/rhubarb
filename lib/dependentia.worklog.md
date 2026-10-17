# dependentia worklog

## 2026-10-17 — born (knotapel D130 T1a)

Dependency-tree measures for D130 (Latin trees as tangles), house tier
by Fran's decision 4 (a) in `knotapel/demo_130_latin_tangles/plan.md`
(archive.sh cannot freeze oratio/fontes). Definitions are Kuhlmann and
Nivre 2006 (COLING/ACL poster, pp. 507-514), checked against the paper:
planar (Def. 4) is WITHOUT the root arc and strictly weaker than
projective (Def. 3); their Fig. 2a = capita {2, 0, 1} is the minimal
witness.

Design notes:

- Two projectivity paths on purpose: `dependentia_cruces` (no crossing
  once the root arc (0, r) is added) and `dependentia_mensurare` (every
  yield an interval, gap degree 0). The suite compares them on every
  tree; agreement is the theorem, not a tautology of shared code.
- Dominance by DFS entry/exit times (iterative, no recursion; children
  linked in position order by inserting right to left).
- Well-nestedness: if disjoint subtrees T(u), T(v) interleave, the
  children of their lowest common ancestor on the two paths are
  siblings whose yields are supersets, so they interleave too. Checking
  sibling pairs is enough (the test oracle checks ALL disjoint pairs
  and all quadruples, so the reduction is verified, not assumed).
- Edge degree (Def. 9) literally: components of the subgraph induced by
  the span, ENDPOINTS INCLUDED. In an induced sub-forest of a tree each
  component has one root, the node whose head lies outside the span (or
  is 0); count roots not dominated by the edge's head. The component
  containing the head's own ancestor counts (e.g. root 2, 2->3, 3->1:
  edge (3,1) has degree 1). Oracle builds components by union-find
  instead.
- Planar core: maximum non-crossing subset of arcs on a line, interval
  DP. f(i,j) = h(i,j) + [arc (i,j)], h(i,j) = max(f(i+1,j), max over
  arcs (i,k), k < j: f(i,k) + f(k,j)). The arc (i,j) never crosses an
  arc inside [i,j], so it is always added; choosing (i,k) as the
  LONGEST chosen arc from i splits the rest cleanly at k (arcs may share
  k). O(n^2) states, O(n^2) total work over the CSR right-endpoint
  lists; n^2 s32 of memory (4 MB at n = 1000).
- The random-order null needs only `paria_disiuncta` (pairs of real
  arcs with four distinct endpoints): E[crossings] = paria / 3 exactly.
  For a tree it equals C(n-1, 2) - sum_v C(deg v, 2); computed here by
  direct pair test (order-independent, which the suite checks across
  all n! orders).
- All measures validate first and refuse an unsound tree (FALSUM / -1);
  O(n^2) validation is noise next to the O(n^2) measures.

Tests (`probationes/probatio_dependentia.c`, 8 sections, 90 checks,
~1.3 s): every head array for n <= 6 against brute oracles written in
the test (dominance by head walks, all-pairs crossings, sorted yields,
all quadruples for well-nestedness, union-find edge degree, all arc
subsets for the planar core, relaxation for components); sound trees
= n^(n-1) (1, 2, 9, 64, 625, 7776); projective = 1, 2, 7, 30, 143, 728;
the two projectivity paths agree; projective => planar => well-nested
holds on all of them; 2,000 random trees n = 7..17 against the oracles;
the 1/3 law by summing crossings over ALL orders (n <= 5 exhaustive, 40
random trees of 6-7 words): 3 * sum == n! * paria exactly. All green on
the first run (written before the tests, so red evidence = plants).

Plants: 18 in this file, all red. One first draft survived and was an
equivalent mutant: the cycle walk bound `passus <= n` -> `< n` (n steps
always reach the root of a sound tree, so the original allows one spare
step); sharpened to `passus + 1 < n`, red. venenum: suite added
(ASan/UBSan + poisoned pool), clean.

## 2026-10-17 — T1a review fixes (independent review, H 0 · M 1 · L 5 · N 4)

Reviewer's own Python oracle (from the K&N text) agreed with the library
on all 2,097,152 head arrays n <= 7, 20,000 random trees n <= 40 and all
11,426 sentences of the nine treebanks: no wrong results. Fixes:

- **M1: per-arc measures.** The plan used "non-projective arcs" without
  defining it and the API only counted non-planar (crossed) arcs; on
  CIRCSE 2,750 arcs are crossed but 1,284 non-projective - in a crossing
  pair usually only the hyperbaton arc is non-projective. New
  `dependentia_arcus` (per arc: crossings, crosses root arc,
  non-projective, edge degree). Non-projective (h, d) = some word
  strictly inside the span not dominated by h; this equals edge degree
  > 0 (a word not dominated by h lies in a component whose root is not
  dominated by h, and conversely), which the suite checks per arc.
  Identities checked on every tree: sum of per-arc crossings = 2 *
  cruces, arcs with a crossing = arcus_secti, max degree =
  gradus_arcuum, tree projective iff no non-projective arc. Gradus
  factored into `_gradus_arcus`, shared by mensurare and arcus.
- **L2:** `dependentia_omittere` returns `DependentiaOmissa` (FACTA,
  NON_SANA, RADIX, MEMORIA) instead of one FALSUM for three causes; the
  root check now runs before any allocation. (Enum named from `omissa`:
  `omissio` is real Latin missing from the lexicon, and a glossary entry
  would shadow oratio's oracle.)
- **L1:** the test's edge-degree oracle had reduced to the library's
  own rule ("head outside the span"); it now takes the component root
  as the member dominating every other member (head walks).
- **N1:** Def. 9 with endpoints included and the interior reading give
  the same degree for every tree (the reviewer's interior-span mutant
  was byte-identical on all inputs): the earlier note "literally,
  ENDPOINTS INCLUDED" is not a design choice; header says so.
- **N2:** pinned planar counts 1, 2, 9, 48, 275, 1638 (n * A001764(n-1))
  and well-nested 1, 2, 9, 64, 605, 6996 (two independent oracles agree;
  sequence not identified); edge degree 0 <=> projective as a third
  projectivity route.
- **N3:** nucleus refuses n > 65535 (i32 index i * n). Not tested: at
  that size the n^2 table fails to allocate anyway, so no cheap test can
  tell the guard from the allocation failure.
- **N4 (correction to the note above):** in C(n-1, 2) - sum_v C(deg v,
  2), deg counts REAL arcs only (the root arc excluded).

Suite 105 checks; 6 new plants (per-arc crossings, root crossing,
dominance by the wrong end, degree of the wrong word, two swapped
statuses) all red; clean under ASan/UBSan + poisoned pool.
