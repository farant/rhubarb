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
