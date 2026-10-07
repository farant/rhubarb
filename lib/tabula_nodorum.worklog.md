# tabula_nodorum worklog

## 2026-10-07 - v1: KnotInfo <= 10 crossings, identification by Alexander + Jones

**Data path (Fran approved: names/PD/symmetry only, polynomials ours).**

- Source: `database_knotinfo` 2026.10.5 (github.com/soehms/database_knotinfo),
  `knotinfo_data_complete.csv`, sha256 `eb511ebc...67c7d`, 88,986,732 bytes.
  The CSV is NEVER committed (89 MB; and we only want the parts we cannot
  compute). Pipe-delimited, no quoting; row 1 = column names, row 2 = display
  labels. 1-based columns used: 1 name, 27 pd_notation, 29 crossing_number,
  63 alexander_polynomial, 67 jones_polynomial, 127 symmetry_type.
  Some symmetry values carry a trailing space - stripped.
- `tools/tabula_nodorum_extrahere.sh <csv>` checks the sha, writes the
  committed fixture `probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv`
  (name, crossings, symmetry, PD; 250 knots, 34 KB) and KnotInfo's two
  polynomial columns to `build/tabula_nodorum/knotinfo_polynomia.tsv`
  (uncommitted), then runs the collation.
- `tools/tabula_nodorum_generare.sh` computes Alexander (polynomium_normale
  form) and Jones per knot via `laqueus_*_ex_pd` and writes
  `lib/tabula_nodorum_data.c` (GENERATUM line 1). `-probare` = cmp with a
  fresh regeneration; `-collatio <tsv>` = compare against KnotInfo's columns
  (Jones exact, Alexander after normalising both). KnotInfo format
  `t^(-2)-t^(-1)+ 1-3*t` -> strip spaces, `*`, parentheses.
- **Collation result: 250/250 Alexander and Jones identical to KnotInfo.**
  This is the strong external oracle for laqueus PD (KnotInfo's chirality
  convention = laqueus physical convention, see laqueus worklog). Plants on
  the collation input: a corrupted Jones (8_20), a swapped Alexander
  (10_132), and 3_1's MIRROR Jones are each named - chirality is checked,
  not just up to mirror.
- Generator declared in `aedificatio.stml` (actio `tabula_nodorum`, generator,
  ingressus = wrapper, generator .c, fixture, laqueus closure), so main's
  fabrica-based generata gate (T7a) judges it after the merge; also stage IX
  of tertia's `tools/generata_probare.sh` (main deleted that script in T7c -
  on merge, take main's deletion; the declaration carries the check).
  Plant: one Jones string edited in the data file -> fabrica "1 stala" with
  the sanatio line; generator changed without regeneration -> `-probare`
  RANCIDA.

**Facts the tests now pin (independent of our code):**
- symmetry counts: 200 reversible, 29 chiral, 13 fully amphichiral,
  7 negative amphichiral, 0 positive, 1 unknot; per-crossing counts
  1,1,2,3,7,21,49,165 (OEIS A002863).
- every amphichiral knot has J(t) = J(1/t); every Alexander is symmetric.
- chiral knots whose Jones is symmetric (Jones cannot see their chirality):
  exactly 9_42, 10_48, 10_71, 10_91, 10_104, 10_125 - the known list.
- (Alexander, Jones) collisions up to mirror among primes <= 10: exactly the
  five classic Jones coincidences of the Rolfsen table: 5_1/10_132,
  8_8/10_129, 8_16/10_156, 10_25/10_56, 10_40/10_103. `agnoscere` returns
  both members of each.
- granny (3_1#3_1) and square (3_1#3_1*) share Alexander with 8_20 and with
  each other; Jones separates all three.

**agnoscere design.** Primes: Alexander equal (normal form), then Jones as
drawn, then Jones(1/t) as mirror unless amphichiral (an amphichiral knot is
reported once; a chiral knot with symmetric Jones is reported TWICE, as K and
K*, honestly). Composites of two nontrivial table knots: Alexander span must
add up (every nontrivial knot <= 10 has span >= 2, since the first Delta = 1
knots are 11n34/11n42), then exact Alexander product, then the up-to-4 Jones
mirror combinations; i == j skips (mirror, plain) as a duplicate of
(plain, mirror). Pair work is rolled back per pair; the whole call leaves the
piscina where it found it.

**Plants (all red):** amphichiral guard removed (4_1 reported twice);
i == j duplicate guard removed (square knot twice); one Jones corrupted in the
data file (recompute test + -probare); composite span off by two (granny and
3_1#4_1 lost); NEGATIVA dropped from amphichiralis (8_17 checks); generator
maps negative amphichiral -> reversible (symmetry counts).

**Traps.**
- BSD sed treats `\t` in a pattern as the letter t - my first plant script
  silently planted nothing on two of three lines. Plant with python.
- a word-boundary rename of `knotinfo` also hit a path comment and a URL
  string - restored by hand; check comments/strings after regex renames.
- `renominare.sh` refuses block-scoped locals in an untracked file
  (REFUSIO, nothing written).
- vocabula -nova reads the nexus index: after renames run `./silva/nexus.sh`
  once or it reports the old names.

**Later.** 11-13 crossings (KnotInfo has them; the fixture would grow to
~12k knots and Jones state sums to 2^13 per knot - fine); HOMFLY or
Khovanov to split the Jones coincidences; links (LinkInfo).

## 2026-10-07 - review I (recensor-mathematicus, e6700cad)

The reviewer independently re-verified the data against the pinned CSV in
Python: the fixture matches the CSV row by row, its own parse of KnotInfo's
polynomial strings gives 0 mismatches over 250 knots, and every pinned fact
holds. It also ran the suite under ASan, UBSan and VENENUM. No bugs in
agnoscere. But 3 (+1) plausible composite-search plants compiled and passed
all 72 tests, because the composite cases were only 3_1#3_1, 3_1#3_1* and
3_1#4_1:
- mirror of the FIRST factor tried only when i == j (3_1* # 5_1 -> 0);
- skip pairs whose later knot has the smaller Alexander span (5_1 # 6_1 lost);
- a negative-amphichiral second factor treated as chiral (3_1 # 8_17 twice);
- the exact Alexander product check dropped (span alone).
New tests: 3_1*#5_1 and 3_1*#5_1* (each also returns the 10_132 partner,
since J(5_1) = J(10_132*)), 5_1#6_1 (returns 6_1#10_132* as well; table
order), 3_1#8_17 and 8_17#8_17 exactly once, and granny Jones with
4_1#4_1 Alexander (same span) -> nothing. All four plants are now red
(4, 2, 2 and 1 failures). The 5_1/10_132 test now pins the partner's
chirality (10_132*) instead of accepting either.
Nits fixed: the generator refuses an empty symmetry with crossings != 0
(and the reverse); the fabrica ingressus list now holds the full header
closure (from the aedilis manifest) plus tools/vexilla.sh; the header
documents that a chiral knot with symmetric Jones is returned twice, and
that composites are two nontrivial factors, mirrors considered, orientation
of the summands ignored.

## 2026-10-07 - v2: up to 13 crossings (12,966 knots)

Fran's decisions: keep the polynomials as strings (accept about 8.5 MB of
committed generated data rather than change the approved struct), and keep
KnotInfo's names VERBATIM ("11n_34", not "11n34"). Names up to 10 crossings
are unchanged.

**Data.** Same pinned CSV. The fixture is now `nodi_xiii.tsv` (2.9 MB,
replaces `nodi_x.tsv`): 1 + 249 + 552 + 2,176 + 9,988 = 12,966 (A002863), and
every knot has a PD code and a symmetry type. `lib/tabula_nodorum_data.c` is
5.6 MB and compiles in 0.5 s. Generation takes about 16 s, so the generata
stage and fabrica's heal take about 16 s longer when an input changes.
- **Collation: 12,966 / 12,966 Alexander and Jones equal to KnotInfo**,
  chirality included. This is an external oracle for laqueus up to 13
  crossings.
- **Trap:** beyond 10 crossings KnotInfo writes negative powers as `N/t` and
  `N/t^K` (with `t^(-K)` when the coefficient is 1). The old converter gave
  7,782 "discrepancies" that were only notation. `alienum_purgare` now
  rewrites `N/t^K` -> `Nt^-K`. Surveyed every notation in the ≤ 13 rows
  first; no other forms occur.

**agnoscere internals (API unchanged).** Brute-force two-knot pairs would be
about 84M per query, so:
- primes: hash (FNV-1a) of the normalized Alexander STRING (the table stores
  exactly polynomium_ad_chordam of the normal form). Chains are kept in table
  order by inserting in reverse, so the output order is unchanged;
- composites: for each table knot i with span_i <= span and det_i | det
  (s64 filter; det = |Delta(-1)|, odd and nonzero for knots), divide exactly
  and hash-look-up the quotient, j >= i. The filter is `<=`, not `<`:
  **Alexander-1 knots are now in the table** (19 nontrivial ones, starting
  11n_34 and 11n_42), and K # 11n_34 has Alexander Delta_K. Plant H2 (strict
  `<`) misses 3_1 # 11n_34: red.

**New facts pinned (from data equal to KnotInfo's columns):**
- counts per crossing number, with symmetry totals 4,649 reversible, 8,238
  chiral, 30 fully, 47 negative and 1 positive amphichiral (12a_427, the only
  one up to 13 crossings);
- 78 chiral knots up to 13 crossings with symmetric Jones (6 up to 10, as
  before);
- the Rolfsen coincidence groups grow: 8_8 ~ 10_129* ~ 13n_1836, and
  10_40 ~ 10_103 ~ 12n_412; the other three stay pairs;
- **prime knots that look like sums:** 13n_586* has the Alexander and Jones
  of 3_1* # 5_1, 13n_593* those of 3_1* # 5_1*, and 11a_176 those of
  3_1 # 8_17. Alexander + Jones cannot separate them;
- Conway (11n_34) and Kinoshita-Terasaka (11n_42): Alexander 1, the same
  Jones; both returned, and (1, 1) still names only the unknot.

**Tests (103, about 15 s).**
- Every knot: theorems (Alexander symmetric, amphichiral J(t) = J(1/t)),
  simplification fixpoints, and self-recognition in both chiralities.
- Recomputation from PD for all knots up to 10 crossings plus every 37th
  above. The full recomputation is the generator's job, and the generata
  gate compares it byte for byte.
- Plants: H2 red; H3 (no j >= i) red with 6 failures; H4 (hash chain cut
  after the first match) red with 16.

**Demo 116's snapshot** was frozen against the 10-crossing table. Run live
now, its output differs (the 13 unmatched pairs get names). That is the
expected kind of library change and is documented here; the follow-up demo
re-runs it.
