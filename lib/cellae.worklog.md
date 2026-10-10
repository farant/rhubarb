# cellae worklog

## 2026-10-10 - T2: catalogus (general closure, D82/D84 order)

Plan: project-specs/cellae-plan.md (approved by Fran). T1 (quaternio_proximi) is
in 3f023542.

**Order is semantics.** The catalog order fixes the axis table, the
first-maximum axis rule and the greedy search order. So the closure
implements D82/D84's construction exactly:
- element 0 = 1;
- the generators in the given order (depth 0, their own writhe);
- rounds r = 1, 2, …: every element present at the start of the round,
  in order, times every generator in order;
- equality up to sign;
- stop when a round adds nothing, or after gradus_maximi growing rounds
  (round 0 included);
- the limit cuts mid-round, and the catalog is flagged TRUNCATED rather
  than silently capped.

It is general in the generators and writhes (approved decision):
D82/D84 = (g1, g1*, g2, g2*) with writhes (+1, −1, +1, −1), limit 4096,
16 rounds.

**Duplicates by hash, not by scan.** The float code scans the whole
catalog per candidate (4096 x 4 x 4096 comparisons at zeta_12). Here the
key is the canonical text of the representative of {q, -q}: the first
nonzero part is made positive with the ring's signum, so the ring must
be ordered. That relies on the ring's text form being canonical; the
zeta_8 closure stopping at exactly 24 and the zeta_12 order match test
it. A rejected candidate (already present) resets the piscina to the
mark taken before its product. The venenum gate now builds the cellae
suite to watch that reset.

Credo (30 checks):
- zeta_8 over Q(sqrt2):
  - 24 entries in 4 rounds (depths 5/10/8/1);
  - 9 nulls;
  - not truncated.
- zeta_12 over Q(sqrt3), limit 4096:
  - truncated;
  - depth histogram 5/12/34/76/148/289/576/1152/1804 (D82's findings);
  - entry by entry equal to D82's float closure (rewritten inside the
    test): order, values (extensio's certified decimals, within 1e-12),
    depths, writhes. Same for zeta_8 against D84.
- Limits: limit 10 mid-round gives truncated; 2 rounds give 15 entries,
  not truncated.
- catalogus_pars keeps index order and refuses repeats or an empty list.
- Out-of-range accessors.
- Refusals: zero generators, mixed rings, an unordered ring (Z[t]).

Plants (7, all red):
- key without sign normalization (zeta_8 no longer closes at 24);
- depth off by one;
- writhe of the parent only;
- truncation not flagged;
- rounds limit off by one;
- product order g*i instead of i*g (the order test catches it);
- pars accepting repeats.

Test syntax notes: extensio's parser takes "-a/2" and "(100a - 99)/100",
not "-(a/2)" or "a - 99/100". Lint: `histogramma` became `distributio`
and `f_tors` became `f_torsio`.
