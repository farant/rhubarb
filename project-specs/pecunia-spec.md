# pecunia - money as exact decimal amounts in C89 (spec)

**Status: SPEC 2026-10-19** - written after brainstorming with Fran;
awaiting his review, then a plan.

*Fran 2026-10-19, right after hospitium v3 reached main (ff e8cad828),
while scoping the remote MCP + OAuth arc (ledger desideratum
...VNA): "what do you think about having a 'money' library in c? that
seems like it could potentially be useful". The household budgeting app
(sub-project C of that arc) and the later Lunch Money pass (D) both
stand on it; sputnik already carries its own `SPUTNIK_VALOR_PECUNIA`
(s64 cents, `include/sputnik_interpres.h:58`) and is a future
consumer.*

Goal: **one small foundation library for money.** It holds exact
amounts with a currency, does arithmetic that refuses instead of
wrapping, rounds only when told how, splits totals without losing a
cent, converts at exact rates, and reads and writes amounts in one
canonical text form plus a display form for people. No floats anywhere.

Decisions (Fran, 2026-10-19, in conversation; ledger decision ...JBC
for the first two):
- **Crypto is out of scope for v1.** 18-decimal amounts (wei) do not fit;
  a `magnus` path can come later if ever needed.
- **The ISO 4217 table is a GENERATED data file** from the published ISO
  lists, with provenance and a fabrica generator action - not hand-typed.
- **Fixed scale of four decimals** for every amount in every currency
  (approach 1 of three): an amount is an s64 count of ten-thousandths
  plus a currency code. Lunch Money sends every amount as
  `^-?\d+(\.\d{1,4})?$` regardless of currency, and no ISO currency has
  more than four minor digits, so the scale is lossless for both.
- **Withdrawn and non-ISO currencies are kept** (option 2 of three): the
  current list, plus the withdrawn list, plus a tiny hand-written
  supplement with a citation per row for the minor units the withdrawn
  list does not carry. Rows carry a status so the app can warn about
  using a withdrawn currency for new entries while still reading old
  ones.
- **Rounding is always explicit** - every operation that can lose
  precision takes a mode. **Splits preserve the total exactly** (largest
  remainder).
- **Two text layers:** a strict canonical form for storage, a display
  form for people; display choices never reach stored data.

## I. What exists (quarta == main e8cad828, 2026-10-19)

- **fractio** (`include/fractio.h`): exact rationals over `Magnus`,
  canonical form, `fractio_ex_s64_s64`, `fractio_ex_chorda` ("a/b"),
  values in a piscina. Rates and multipliers come in as `Fractio`.
- **magnus** (`include/magnus.h`): arbitrary-precision integers with a
  small fast path. Intermediates of multiplication, conversion and
  splitting are computed in `Magnus` so that `quantitas * numerator`
  never overflows silently.
- **s64** = `signed long long` (`include/latina.h:4079`): the amount
  range at scale 10^4 is +-922,337,203,685,477.5807 units.
- **sors** (`include/sors.h`): seeded SplitMix-based RNG, same series on
  every machine (`sors_seminare`, `sors_intra`) - for property tests.
- **xml** (`include/xml.h`): `xml_legere`, `xml_invenire_omnes_liberos`,
  `xml_textus_internus` - enough to read the ISO lists in the generator.
- **Generator precedent:** `tools/runae_generare.sh` + `.c` read pinned
  Unicode data from `probationes/fixa/unicode/<versio>/` and write
  `lib/runae_tabulae.c` (GENERATUM, `-probare` = exact cmp); fabrica
  generator actions in `aedificatio.stml` (e.g. `entitates_html`,
  around line 456).
- **json does not keep number text** (`include/json.h:86`): fractional
  numbers arrive as `f64`. pecunia never reads JSON; this matters only
  for the Lunch Money client (sub-project D, see section IV).
- **Name check:** no `Pecunia`, `Moneta` or `pecunia_*` symbols outside
  sputnik's own `SPUTNIK_*` names and its static `_parsere_pecuniam`.

**ISO data as measured 2026-10-19:**
- `list-one.xml` (current), published 2026-09-17: 178 codes; minor
  units 2 x139, 0 x17, 3 x7 (BHD IQD JOD KWD LYD OMR TND), 4 x2 (CLF
  UYW), `N.A.` x13 (gold, silver, palladium, platinum, SDR, bond units,
  XSU, XUA, XTS, XXX).
- `list-three.xml` (withdrawn), published 2026-01-01: code, name,
  number, withdrawal date - **no minor units.**
- Lunch Money's `currencyEnum` (v2 spec 2.11.1, 130 codes) has 12 codes
  not on the current list: withdrawn BGN (2026-01), HRK, CUC, ANG, LVL,
  LTL, MRO; never-ISO GGP, IMP, JEP; crypto BTC, ETH.

## II. Design

### II.1 Types

```c
#define PECUNIA_SCALA (X * M)        /* 10000 */

nomen structura {
    character codex[IV];             /* "USD": majusculae, NUL */
} Moneta;

nomen structura {
          s64  quantitas;            /* decimillesimae: USD 12.34
                                      * = 123400 */
       Moneta  moneta;
} Pecunia;
```

- Values, not pointers: a `Pecunia` is 16 bytes and needs no piscina;
  only text output allocates.
- The currency is held by **code, not table index**: equality never
  depends on the generated table's order, and debuggers show `"USD"`.
  The table is consulted only for lookups (binary search by code).
- A `Moneta` is only ever built by `moneta_ex_codice`, so every `Moneta`
  in a `Pecunia` names a row of the table.

### II.2 Errors

Every fallible function returns a `PecuniaVitium` and leaves `exitus`
untouched on failure. *Refinement of the "b32 + exitus" pattern shown in
conversation: same shape, but the failure is NAMED, so a web form can
say "too many decimals" and a test can assert the reason.*

```c
nomen enumeratio {
    PECUNIA_BENE = 0,
    PECUNIA_VITIUM_FORMA,             /* textus malformatus */
    PECUNIA_VITIUM_DECIMALES,         /* plus quam IV decimales */
    PECUNIA_VITIUM_SUPERFLUXUS,       /* extra s64 */
    PECUNIA_VITIUM_MONETA_IGNOTA,     /* codex non in tabula */
    PECUNIA_VITIUM_MONETA_SINE_UNITATE, /* XAU XXX... */
    PECUNIA_VITIUM_MONETAE_DIVERSAE,  /* USD + EUR */
    PECUNIA_VITIUM_NON_SOLVIBILIS,    /* partiri ad unitatem: totum
                                       * non multiplex unitatis */
    PECUNIA_VITIUM_RATIONES           /* rationes negativae aut summa 0 */
} PecuniaVitium;

constans character*
pecunia_vitium_titulus (
    PecuniaVitium vitium);            /* "plus quam IV decimales" */
```

### II.3 Currencies

```c
nomen enumeratio {
    MONETA_CURRENS = 0,               /* in indice ISO uno */
    MONETA_HISTORICA,                 /* in indice ISO tertio solum */
    MONETA_EXTRA_ISO                  /* GGP IMP JEP */
} MonetaStatus;

nomen structura {
    constans character* codex;        /* "USD" */
                   s32  numerus;      /* 840; 0 si EXTRA_ISO */
                   s32  exponens;     /* 0..IV */
          MonetaStatus  status;
    constans character* titulus;      /* "US Dollar" */
} MonetaInformatio;

PecuniaVitium moneta_ex_codice (chorda codex, Moneta* exitus);
                              /* sine casu: "usd" == "USD" */
constans MonetaInformatio* moneta_informatio (Moneta m);
s32           moneta_exponens (Moneta m);
b32           moneta_aequalis (Moneta a, Moneta b);
```

### II.4 Construction and arithmetic

```c
PecuniaVitium pecunia_ex_unitatibus (s64 unitates, Moneta m,
                                     Pecunia* exitus);
                     /* -1234 USD -> USD -12.3400 (unitas minor) */
Pecunia       pecunia_nihil (Moneta m);
PecuniaVitium pecunia_adde (Pecunia a, Pecunia b, Pecunia* exitus);
PecuniaVitium pecunia_subtrahe (Pecunia a, Pecunia b, Pecunia* exitus);
PecuniaVitium pecunia_nega (Pecunia a, Pecunia* exitus);
PecuniaVitium pecunia_summa (constans Pecunia* a, memoriae_index n,
                             Pecunia* exitus);   /* n == 0: FORMA */
PecuniaVitium pecunia_compara (Pecunia a, Pecunia b, s32* exitus);
s32           pecunia_signum (Pecunia a);
b32           pecunia_aequalis (Pecunia a, Pecunia b);
```

- Overflow is refused (`SUPERFLUXUS`), including negating the s64
  minimum. Mixed currencies are refused (`MONETAE_DIVERSAE`), never
  converted silently.

### II.5 Rounding, multiplication, payable amounts

```c
nomen enumeratio {
    PECUNIA_PAR = 0,             /* dimidium ad par (argentariorum) */
    PECUNIA_DIMIDIUM_SURSUM,     /* dimidium a nihilo (2.5->3, -2.5->-3) */
    PECUNIA_VERSUS_NIHIL,        /* truncatio */
    PECUNIA_SOLUM,               /* versus -infinitum */
    PECUNIA_TECTUM               /* versus +infinitum */
} PecuniaModus;

PecuniaVitium pecunia_multiplica (Pecunia a, Fractio f,
                                  PecuniaModus modus, Piscina* p,
                                  Pecunia* exitus);
PecuniaVitium pecunia_ad_unitatem (Pecunia a, PecuniaModus modus,
                                   Pecunia* exitus);
b32           pecunia_solvibilis (Pecunia a); /* multiplex unitatis? */
```

- `pecunia_multiplica` computes `quantitas * num / den` exactly in
  `Magnus`, then rounds once to the scale with `modus` (8.875% tax =
  `71/800`).
- `pecunia_ad_unitatem` rounds to the currency's minor unit
  (USD 3.4590 -> 3.46 with `PECUNIA_PAR`; JPY to whole yen).

### II.6 Splitting

```c
nomen enumeratio {
    PECUNIA_GRANUM_SCALA = 0,    /* partes in decimillesimis */
    PECUNIA_GRANUM_UNITAS        /* partes solvibiles (centesimi) */
} PecuniaGranum;

PecuniaVitium pecunia_partiri (Pecunia totum,
                               constans s64* rationes,
                               memoriae_index n,
                               PecuniaGranum granum,
                               Pecunia* exitus);   /* n partes */
PecuniaVitium pecunia_dividere (Pecunia totum, memoriae_index n,
                                PecuniaGranum granum,
                                Pecunia* exitus);  /* aequaliter */
```

- **Largest remainder:** each part gets `floor(totum * r_i / sum)` in
  grains (computed in `Magnus`); the leftover grains go one each to the
  parts with the largest remainders, **ties to the lower index**. The
  parts always sum to `totum` exactly.
- Negative totals split by magnitude and keep the sign (-10.00 / 3 =
  -3.34, -3.33, -3.33).
- `GRANUM_UNITAS` requires `totum` to be payable already
  (`NON_SOLVIBILIS` otherwise): USD 10.0050 cannot be split into cents
  that sum to it. Ratios must be >= 0 with a positive sum
  (`RATIONES`).
- `pecunia_dividere` is `partiri` with equal ratios; its parts differ by
  at most one grain.

### II.7 Conversion

```c
PecuniaVitium pecunia_convertere (Pecunia a, Fractio cambium,
                                  Moneta ad, PecuniaModus modus,
                                  Piscina* p, Pecunia* exitus);
```

- `cambium` = units of `ad` per one unit of `a.moneta`, must be > 0
  (`FORMA` otherwise). Exact product in `Magnus`, one rounding to the
  scale; the caller rounds to a payable amount separately if wanted.

### II.8 Text forms

**Canonical (storage, events): strict, one spelling per value.**

```c
chorda        pecunia_ad_textum (Pecunia a, Piscina* p);
                              /* "USD -12.3400" */
PecuniaVitium pecunia_ex_textu (chorda textus, Pecunia* exitus);
```

- Upper-case code, one space, optional `-`, integer part without
  leading zeros (but `0` itself), always exactly four decimals, no
  grouping, no `+`. Zero is `USD 0.0000`, never `-0.0000`.
- `pecunia_ex_textu` accepts **only** that form (`FORMA` otherwise), so
  `pecunia_ad_textum(pecunia_ex_textu(t)) == t` for every accepted `t`.

**Bare decimal (Lunch Money and other APIs):**

```c
chorda        pecunia_numerus (Pecunia a, Piscina* p);  /* "-12.3400" */
PecuniaVitium pecunia_ex_numero (chorda textus, Moneta m,
                                 Pecunia* exitus);
```

- Accepts `^-?\d+(\.\d{1,4})?$` exactly (Lunch Money's pattern).

**Display (web app, messages Claude shows):**

```c
nomen structura {
             b32  plena;         /* VERUM: IV decimales; FALSUM:
                                  * unitas minor monetae */
    PecuniaModus  modus;         /* rotundatio si non plena */
          chorda  coetus;        /* separator coetuum: "," ("" nullus) */
          chorda  decimalis;     /* "." */
             b32  rationaria;    /* (12.34) pro -12.34 */
          chorda  signum;        /* "$" a vocante; vacua -> codex */
             b32  nota_post;     /* "12.34 USD" pro "USD 12.34" */
} PecuniaForma;

vacuum        pecunia_forma_ordinaria (PecuniaForma* forma);
              /* unitas minor, PAR, ",", ".", "-", codex ante */
chorda        pecunia_formare (Pecunia a, constans PecuniaForma* f,
                               Piscina* p);
PecuniaVitium pecunia_ex_ingressu (chorda textus, Moneta m,
                                   constans PecuniaForma* f,
                                   Pecunia* exitus);
```

- Separators are `chorda`, so multi-byte ones (a narrow no-break space)
  work. ISO defines no symbols (they live in CLDR); in v1 the caller
  supplies `signum`, and the struct is where locale support attaches
  later.
- `pecunia_ex_ingressu` (web forms, amounts Claude passes to tools) is
  lenient by explicit rules: optional leading `-` or accounting
  parentheses, the configured symbol or the code before or after,
  surrounding spaces, grouping separators **only in valid positions**
  (`1,234.56`; not `12,34.56`), up to four decimals. Everything else is
  refused with a named reason: `1e5`, five decimals, two signs, and
  `12,34` (ambiguous between 1234 and 12.34 - refuse, never guess).

### II.9 The ISO table

- **Pinned sources** under `probationes/fixa/iso4217/2026-09-17/`:
  `list-one.xml` and `list-three.xml` from SIX (the ISO 4217
  maintenance agency, `https://www.six-group.com/dam/download/
  financial-information/data-center/iso-currrency/lists/`), sha256 in a
  `SHA256SUMS` beside them. Updating = a deliberate new download into a
  new dated directory, regenerate, review the diff. The build never
  touches the network.
- **Supplement** `probationes/fixa/iso4217/supplementum.stml`: one row
  per code the lists cannot place - minor unit plus status plus
  citation. v1 rows: BGN HRK CUC ANG LVL LTL MRO (`HISTORICA`, minor
  unit from the last list-one that carried them) and GGP IMP JEP
  (`EXTRA_ISO`, issuing authority). BTC/ETH are not rows (crypto out of
  scope).
- **Generator** `tools/pecunia_monetae_generare.c` (+ `.sh` wrapper with
  `-probare` = exact cmp, as runae) reads the XML through `xml.h` and
  the supplement through `stml.h`, and writes `lib/pecunia_monetae.c`
  (GENERATUM, line 1): one row per code - current entries first-wins on
  duplicates (EUR appears under many countries), withdrawn entries only
  for codes absent from list-one and present in the supplement, `N.A.`
  entries dropped - sorted by code. A supplement row for a code the
  lists DO place with a different minor unit is a generator error, not
  a silent override.
- **fabrica:** a generator action `pecunia_monetae` in
  `aedificatio.stml` with inputs the two XML files, the supplement and
  the generator sources; output `lib/pecunia_monetae.c`
  (`provenientia="regeneratio"`), so the `generata` gate holds the
  committed file to the generator's output byte for byte.

### II.10 Files

- `include/pecunia.h`, `lib/pecunia.c` - types, arithmetic, text.
- `lib/pecunia_monetae.c` - GENERATED table (internal; reached through
  `moneta_*`).
- `tools/pecunia_monetae_generare.c`, `tools/pecunia_monetae_generare.sh`.
- `probationes/fixa/iso4217/...` as above; `probationes/fixa/pecunia/
  rotundatio.txt` (section III).
- `probationes/probatio_pecunia.c`.
- `lib/pecunia.interrogationes.stml` - typed questions (U5b): how do I
  split a bill; how do I store an amount; why is my input refused; how
  do I convert currencies.
- `lib/pecunia.worklog.md`.

## III. Tests

`probationes/probatio_pecunia.c`, credo v2, one section per area:
construction, arithmetic, rounding, splitting, conversion, canonical
text, display, input, table.

- **Rounding oracle = Python's `decimal`**, run once by
  `tools/pecunia_rotundatio_oraculum.py`: all five modes over a few
  hundred chosen values (exact halves, near halves, negatives, zero, the
  s64 extremes; multiplications by awkward fractions such as 1/3, 2/3,
  71/800) written to `probationes/fixa/pecunia/rotundatio.txt` with its
  sha256 pinned like the RFC fixtures. The C test reads only the
  fixture; Python is never needed at test time.
- **Property checks** with a fixed `sors` seed, a few thousand cases
  each: splits sum exactly to the total and equal splits differ by at
  most one grain; `ex_textu(ad_textum(x)) == x` and the reverse for
  valid text; `subtrahe(adde(a, b), b) == a`; nothing wraps at the s64
  edges (overflow is refused, never returned).
- **Table checks:** USD 2, JPY 0, KWD 3, CLF 4; BGN `HISTORICA`; GGP
  `EXTRA_ISO`; XAU and XXX refused `MONETA_SINE_UNITATE`; `usd` found;
  EUR present once; plus the generator's `-probare` and the `generata`
  gate.
- **Named reasons:** each refusal case asserts its `PecuniaVitium`, not
  just failure.
- **Born-red plants** via `silva.planta`, each caught by a named test:
  half-even swapped for half-up; split leftover given to the last part
  instead of the largest remainders; overflow check removed; `-0.0000`
  allowed in canonical output; `12,34` accepted; supplement allowed to
  override a listed minor unit.
- **Linux:** `./tools/linux/probatio.sh -musl pecunia` and `-glibc`
  (names on the command line; the compile sweep in
  `tools/linux/percursus.sh` already covers every `lib/*.c`).

## IV. Not in this spec

- Crypto and any scale beyond four decimals (decision ...JBC).
- Currency symbols and locale data (CLDR); the caller passes a symbol.
- Exchange-rate sources; rates arrive as `Fractio` from the caller.
- Keeping JSON number text in `json.h` - needed only by the Lunch Money
  client, which can receive a balance as a JSON number; belongs to
  sub-project D of desideratum ...VNA (a widely-gated header edit).
- The budgeting app's data model (sub-project C) and sputnik migrating
  to `Pecunia`.

## AUDIENDA (not verified)

- Minor units for the supplement rows are not yet sourced row by row:
  BGN HRK CUC ANG LVL LTL are 2 in common knowledge; **MRO** (ouguiya,
  divided into 5 khoums) needs the last list-one that carried it
  checked; GGP IMP JEP follow sterling (2) per their issuers, citation
  to find.
- That `xml.h` reads the SIX files as-is (UTF-8 country names such as
  `ÅLAND ISLANDS`, the XML declaration with `standalone`) - checked only
  with Python's parser so far.
- That the Latin identifier lint accepts the new words (`decimillesimae`,
  `solvibilis`, `rationaria`, `granum`...) - glossary entries may be
  needed.
- *Checked, not open:* `magnus_divide` (`include/magnus.h:134-138`)
  gives a Euclidean quotient and remainder (`0 <= r < |d|`) and
  `magnus_ad_s64` reports values that do not fit - enough for rounding
  and splitting without a new magnus function.
- SIX's list URLs are as fetched 2026-10-19 (note the `currrency`
  spelling in the path); the sha256 of each pinned file is recorded
  when it is committed, not before.
