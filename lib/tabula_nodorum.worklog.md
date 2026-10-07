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
