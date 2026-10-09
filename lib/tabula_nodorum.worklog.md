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

## 2026-10-07 - review of d34cf0c7 (v2, 13 crossings)

No bugs. Independent checks by the reviewer, against the pinned CSV and its
own exhaustive prime + two-sum search:
- all 12,966 knots in both chiralities (25,932 queries): identical sets, no
  duplicates, 330 composite candidates;
- 4,800 random sums, including Alexander-1 factors;
- all 760 pairs of Alexander-1 knots;
- a det beyond s64 (filter off) and (1, 1);
- every pinned fact, and Demo 116's new ambiguity groups, confirmed as
  genuine KnotInfo groups.
Changes:
- **Correction:** the d34cf0c7 commit message says DCCLXXXII (782)
  notation-only discrepancies; the true count is 7,782 (as stated above in
  this worklog). Commits are not rewritten.
- **Pinned:** 9_12 ~ 4_1 # 5_2 and 9_12* ~ 4_1 # 5_2*. This is the smallest
  of the 95 primes up to 13 crossings (up to mirror) that share Alexander +
  Jones with a two-knot sum. The count 95 is the reviewer's; the 9_12 case
  was re-checked here. Mentioned in the header.
- **Runtime:** the full self-recognition sweep took 126 s under
  ASan/UBSan/VENENUM. It now covers every knot up to 10 crossings plus every
  37th above (the full sweep was the review's exhaustive comparison). The
  suite takes 1.5 s, with 107 checks.
- **Plant G1** (hash chain walked without the string compare) passes the
  suite. That is unobservable through table queries, since Jones always
  separates the extra candidates, so it is left as is. Plants G2 (det filter
  reversed) and G3 (j > i) are red.
- **Not done:** the optional speedup (check Jones degrees before multiplying
  each composite pair).

## 2026-10-08 - PD as one string per knot (silva token limit)

**Found by** the fresh-clone run (`tools/frigida_probare.sh`) after
merging main into tertia: 5 silva tests red (arbor_plagula, canon_corpus,
exemplaria_inutilia/lint/obsoleta), all on this file: "latina.h in
clausura, ZERO lexemata expansa", round trip 243/244, examen RECUSO
("fines tactae: expansio trunca"). Cause: silva caps one file's expanded
token stream at 2^20 (SILVA_LIMEN_LEXEMATUM_DEFALTUM, a deliberate
guard against judging a truncated TU). The 13-crossing table
(d34cf0c7) emitted `TABULA_NODORUM_PD[]` as 657,484 integers, about 2.4M
tokens with commas; the 10-crossing version was far under. d34cf0c7 named
its gates by hand without `silva`, so nothing saw it (same trap as the
memory note on hand-picked gates missing `generata`).

**Fix (Fran chose option 1 of 3; the others were splitting the file or
raising silva's cap):** `NodusTabulae.initium_pd` -> `constans character*
pd` = "1,5,2,4,..." (<= ~150 chars at 13 crossings, under C89's 509);
`TABULA_NODORUM_PD[]` removed; `tabula_nodorum_pd(n, piscina)` decodes
into the pool. The file now has ~210K tokens (the struct array was already
~200K), headroom to about 5x. 4.7 MB instead of 5.6 MB.

**Decoder is strict:** decimal fields without a leading '0', each label
in 1..2n (arcs of an n-crossing diagram), exactly 4n fields, no empty
field, no trailing comma, nothing but digits and commas; anything else ->
NIHIL. Also NIHIL for n == NIHIL and the trivial knot.

**Evidence:**
- every knot's PD parsed by Python straight from the OLD data file
  (independent of the C decoder) == the new decoder's output for all
  12,966 knots (scratch `pd_oraculum.tsv` vs `pd_novum.tsv`, cmp);
- generator `-probare` recens; examen ACCIPE (was RECUSO);
- suite 119 (decoder: one good string + 10 malformed);
- plants (7): leading zero accepted, trailing comma accepted, 1..2n range
  ignored, count unchecked, stray character skipped, 3_1 data corrupted,
  8_20 data corrupted - all RED. The trailing-comma plant first SURVIVED:
  my malformed string had 11 numbers, so the count check refused it
  anyway; now the trailing comma follows a complete 12.

Callers: only this suite. knotapel demo 116's snapshot embeds its own
frozen copy and is untouched; no live demo calls the accessor.

## 2026-10-09 - strings out of __cstring (the 42 GB log cache)

Found from Fran's disk alarm: /private/var/db/uuidtext (macOS log
string cache) grew to 42 GB in a day, 99% rhubarb test binaries. Chain,
each link measured: (1) compile_tests.sh links every lib object plus
Cocoa/Security/WebKit into EVERY root test; (2) loading any of those
frameworks makes a process write to the system log at startup (a
two-line program + `-framework Security` gets a uuidtext entry; without
frameworks, none; OS_ACTIVITY_MODE=disable does NOT prevent it -
tested on a real test binary given a fresh LC_UUID and re-signed);
(3) the entry copies the binary's __TEXT,__cstring - 3.33 MB per test,
of which the 12,965 PD strings of this table (cd292cfb, the switch from
numbers to strings for silva's 2^20-token cap) were nearly all; (4)
every distinct build that runs leaves one file: edits, plants, five
worktrees, umbrae clones, frigida temp trees.

Fix here (generator, API unchanged): every string of the table goes
into one of four NAMED arrays (`TEXTUS_TITULORUM`, `TEXTUS_PD`,
`TEXTUS_ALEXANDER`, `TEXTUS_JONES`; one literal per knot ending
"\0"), entries point `TEXTUS_PD + offset`. Named arrays live in
__TEXT,__const, which uuidtext does not copy - measured on a probe:
1.14 MB of literals -> uuidtext file 1.14 MB; the same as named arrays
-> 150 bytes. Precedent: biblia_dr's `textus_dr[]`. Generator refuses
'"', '\\' and non-printables in any string. Tokens: ~2 more per field
(well under silva's cap); examen ACCIPE in 1.7 s.

Oracle: a program printing every field of all 12,966 knots (+ FONS,
NUMERUS), built against the OLD and the NEW data file: byte-identical.
__cstring of the table alone 3.0 MB -> 169 B; of a root test
(probatio_piscina) 3.3 MB -> 280 KB, so each cached build ~12x smaller.
The rest of the 280 KB is other libraries' strings in every test -
slice 7's per-test closures (only GUI tests load Cocoa) stop most tests
logging at all. Cleanup + monitoring: tools/uuidtext_census.py.
