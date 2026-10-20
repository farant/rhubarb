# pecunia worklog

## 2026-10-19 - T1: ISO data, generator, table, currency lookup

**Data pinned** in `probationes/fixa/iso4217/2026-09-17/`: SIX
`list-one.xml` (Pblshd 2026-09-17) and `list-three.xml` (Pblshd
2026-01-01), unchanged, sha256 in `SHA256SUMS`. They have CRLF line
endings; the repo has no `.gitattributes` and no `core.autocrlf`, so git
stores the bytes as-is (checked). If someone ever adds `* text=auto`,
these hashes break first.

**Supplement sourced row by row** (nothing guessed, nothing left out):
the withdrawn list carries codes, numbers and dates but no minor units,
so each HISTORICA row cites the newest archived list-one that still
carried it (Wayback Machine): LVL/LTL from the 2013-04-09 list
(`table_a1.xml`, the old file name), MRO from 2017-01-01, ANG/BGN/CUC/HRK
from 2022-09-23. All 2 - MRO too, despite the ouguiya's khoums (fifths):
ISO always gave MRO two decimals. GGP/IMP/JEP are local issues of
sterling at par (government pages cited); exponent = GBP's.
Withdrawn rows take their ISO NUMBER from list-three (BGN 975, HRK 191,
MRO 478); EXTRA_ISO rows have numerus 0.

**Counts:** 175 rows = 165 current (178 codes - 13 N.A.) + 7 historic +
3 extra ISO; 13 codes without a minor unit (XAG XAU XBA XBB XBC XBD XDR
XPD XPT XSU XTS XUA XXX) kept in a separate list so lookups can say
SINE_UNITATE instead of IGNOTA.

**Generator built by aedilis** (it reads XML through `xml.h` and the
supplement through `stml.h`), unlike runae's stdio-only generator. The
fabrica action declares `build/aedilis/pecunia_monetae_generare` as
`vestigium`; `bin/fabrica iudicare -plenus lib/pecunia_monetae.c`
regenerates and compares: recens. Titles with non-ASCII bytes are
written as 3-digit OCTAL escapes - a `\x` escape would swallow a
following hex-letter (`Bol\xC3\xADvar` is fine, `Caf\xC3\xA9e` is not).

**`-fumus` mode** feeds the generator six bad supplements (contradicts
list-one, superfluous, same code twice, "historic" code not in
list-three, "extra ISO" code that is in list-three, missing source) and
expects each refused with its NAMED reason. Real data never exercises
these rules, so without the fumus deleting one would be mute. An empty
attribute (`fons=""`) reads as ABSENT in STML, so that case is refused
as "attributum deest: fons" - still the right refusal.

**Plant hazard found (park ...RN9):** after `silva.planta` restored the
generator, a fresh build still ran the PLANTED code - the object in
`build/aedilis/obiecta` was compiled from the plant in the same second
the source was restored, and `struere.sh` judges with `-nt` under
`/bin/sh` (bash 3.2, whole seconds). Deleting the binary is not enough;
the object itself is stale. `touch` + rebuild fixed it here. Same family
as the `cp -p` memory (plant-restore-mtime).

**Plants (red, revert, green):** supplement contradiction check
disabled -> `-fumus` red; binary search stepping two -> `tabula` red;
case folding disabled -> `codices` red. Before the plants the test was
widened so EVERY row is looked up by its own code (a search off by one
at the edges would hide behind the handful of named codes).

**Lint:** `solvibilis` (Medieval Latin, from solvere "to pay") added to
the glossary; WORDS lacks it.

## 2026-10-19 - T2: the value type and refusing arithmetic

`Pecunia` = s64 ten-thousandths + `PecuniaMoneta`, 16 bytes, by value.
`PECUNIA_QUANTITAS_MAXIMA/MINIMA` are exported (every house lib defines
its own s64 limit - magnus, polynomium, surdus - there is no shared one).

**Overflow is checked BEFORE the operation**, never by inspecting a
wrapped result (signed overflow is undefined in C). `pecunia_subtrahe`
has its own two-sided check rather than `adde(a, nega(b))`, because
`nega(MINIMA)` is itself out of range while `0 - MINIMA` must be
refused and `-1 - MAXIMA` must succeed (it equals MINIMA exactly).

**`pecunia_ex_unitatibus` bound:** `|unitates| <= MAXIMA / factor` on
both sides. The lower bound is not `MINIMA / factor`: C89 leaves the
rounding of negative division implementation-defined, and the true
bound is `ceil(MINIMA / f) = -(MAXIMA / f)` because every factor > 1 is
a multiple of 5 and never divides 2^63. CLF (factor 1) takes any s64.

**`pecunia_summa` refuses overflow mid-way** even when the final sum
would fit (`MAXIMA + 1 - 1`): reordering to dodge it would make the
result depend on more than the input, and a partial sum out of range
in a budget is a data error worth surfacing.

Plants: `_adde_tutum` upper check disabled -> `arithmetica` red; summa's
currency check only on element 0 -> `summa_mixta` red.

## 2026-10-19 - T3: rounding, multiplication, payable amounts, conversion

**Oracle:** `tools/pecunia_rotundatio_oraculum.py` (Python `decimal`,
precision 80, stdlib only) writes `probationes/fixa/pecunia/rotundatio.txt`
once; the C test pins its sha256 and checks that it consumed exactly
3320 `M` (multiply) and 1560 `U` (round to payable) lines, so a
truncated or half-parsed fixture cannot pass quietly. Two runs of the
script give the same bytes. The fixture's first line deliberately does
NOT say GENERATUM - that word in line 1 makes the `generata` gate demand
a fabrica generator action, and this file is a pinned oracle, not a
build artifact.

**No currency has exponent 1** in the 2026-09-17 ISO list (0, 2, 3, 4
only), and `pecunia_ad_unitatem` takes the exponent from the currency,
so exponent-1 lines could not be tested through the API - the oracle
writes exponents {0, 2, 3, 4} only (plan R5). If ISO ever adds one, the
`_factor_unitatis` case for I already exists; extend the oracle then.

**One rounding decision, two arithmetic paths.** `_sursum(modus,
residuum_nullum, dimidium, quotiens_negativus, quotiens_impar)` decides
"floor + 1 or not" from the floor quotient and remainder. `multiplica`
/ `convertere` feed it from `Magnus` (`magnus_divide` is Euclidean, so
the quotient is already the floor and `2r` vs `den` gives the half
test); `ad_unitatem` feeds it from s64 directly (divisor <= 10^4) with
a hand Euclidean step - C89 leaves `/` and `%` of negatives
implementation-defined, but `a == q*f + r` always holds, so a negative
`r` is corrected (`q -= 1; r += f`). The first draft of `ad_unitatem`
went through Magnus and created a piscina per call for values that
always fit in s64; the API has no piscina parameter, so the s64 path is
the honest one.

**Half-away-from-zero** = Python's ROUND_HALF_UP: on a tie, step up when
the floor quotient is >= 0 (value q + 0.5, positive), stay when it is
negative (q + 0.5 <= -0.5, so q is the one further from zero).

All 4880 oracle lines agreed on the first run. Plants: PAR made to
behave like half-up; the s64 negative-floor correction removed;
VERSUS_NIHIL made SOLUM for negatives - each `oraculum` red.

## 2026-10-19 - T4: splitting

`pecunia_partiri(totum, rationes, n, granum, piscina, exitus)` - largest
remainder: each part gets `floor(G * r_i / S)` grains (G = grains of the
total's magnitude, S = sum of ratios, both in `Magnus`, so huge ratios
like {MAXIMA, MAXIMA} are fine), the leftover grains (always < n) go one
each to the largest remainders, ties to the LOWER index (the first
maximum found wins - no sort, O(n * leftover)). A zero ratio has
remainder 0 and can never win a leftover grain: the leftover equals the
sum of the fractional parts, so it is smaller than the number of parts
whose remainder is positive.

**Bug the edge test caught:** splitting MINIMA into ONE part. Its
magnitude is 2^63, which does not fit in s64, and the first draft
converted the magnitude part to s64 before restoring the sign. Now the
sign is restored in `Magnus` and only the signed part is converted; it
always fits (same sign as the total, never larger in magnitude). All
parts are computed before any `exitus` is written, so a failure leaves
the output untouched.

`GRANUM_UNITAS` refuses a total that is not already payable
(`NON_SOLVIBILIS`): USD 10.0050 cannot become cents that add back up to
it. With `GRANUM_SCALA` the same total splits to 5.0025 + 5.0025.

`Piscina*` added to both functions (plan P8) - the spec's sketch had
none.

Property check: 3000 random cases (USD/JPY/KWD/CLF, n 1..12, ratios
0..1000, both grains): parts sum exactly to the total, never take the
opposite sign, are payable under UNITAS; equal splits differ by at most
one grain. Plants: leftover to the last parts, ties to the higher index,
sign not restored - each red in `exempla`.

## 2026-10-19 - T5: canonical text and bare decimals

**Canonical form `USD -12.3400`:** upper-case code, one space, optional
`-`, integer part without leading zeros (but `0` itself), exactly four
decimals, no grouping, no `+`, zero never signed. The reader accepts
ONLY that, so text equality is value equality - clean event logs and
diffs. The reader checks the code is three upper-case letters BEFORE
lookup (`usd 12.3400` is FORMA, not a currency question), then reads
the quantity, then looks the code up (XAU -> SINE_UNITATE, ABC ->
IGNOTA).

**Bare decimal = Lunch Money's pattern** `^-?\d+(\.\d{1,4})?$`: leading
zeros allowed (even 46 of them - accumulation stays 0 until the first
non-zero digit, so no false overflow), `12.` and `.5` refused, five
decimals -> DECIMALES (named, so a form can say why).

**Shape first, value second.** `_quantitatem_legere` scans the whole
text and refuses bad shape (FORMA / DECIMALES) before computing; the
value is accumulated in `i64` (unsigned 64-bit), the integer part
capped at 2^63 / 10^4 per digit, then compared with 2^63 (negative) or
2^63 - 1 (positive). Writers take the magnitude as `i64` too:
`-(q + 1) + 1` gets |MINIMA| = 2^63 without overflow.

**Bitten again by a latina.h macro:** a local named `magnitudo` - that is
`sizeof`. examen flagged it ("magnitudo macrum latina.h est"); renamed
`amplitudo`. And a local `usd` was a new word for the Latin lint ->
`nummus`.

Round trip over 5000 random s64 values (full range, USD and JPY): both
`ex_textu(ad_textum(x)) == x` and `ex_numero(numerus(x)) == x`. Plants:
`-0.0000` accepted, a leading zero accepted, the positive bound
loosened by one (`...5808` would wrap to MINIMA) - each red.
