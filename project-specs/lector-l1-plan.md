# lector L1 — sarcina (read-only ZIP) Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use
> checkbox (`- [ ]`) syntax for tracking.

**Goal:** a small library that opens a ZIP archive held in memory, lists
its entries and returns any entry's bytes (stored or DEFLATE), CRC
checked - the container layer under EPUB (`codex`, L2).

**Architecture:** the archive is a `chorda` of bytes (the caller reads
the file, e.g. `filum_legere_totum`). `sarcina_aperire` finds the End
Of Central Directory record, parses the central directory into an
entry table; `sarcina_legere` locates an entry's local header, skips it
and returns the data, inflating DEFLATE with `flatura_inflare` and
checking `flatura_crc32`. No writing, no ZIP64, no encryption.

**Tech Stack:** C89 in Latin; `flatura` (include/flatura.h:79
`flatura_inflare`, :150 `flatura_crc32`, :114 `flatura_deflare` for
test fixtures), `piscina`, `chorda`, credo v2.

**Spec:** `project-specs/lector-spec.md` §VII (`sarcina`) and §VIII L1;
approved by Fran 2026-10-23 ("that sounds good to me").

## Global Constraints

- C89 + latina.h; every Roman numeral is a macro (no numeral
  identifiers); `i32` unsigned, `s32` signed; `chorda` not
  NUL-terminated. New lint words must be real Latin.
- ZIP integers are little-endian; read byte by byte (no struct casts,
  no alignment assumptions).
- Never read outside the archive: every offset and length is checked
  against `datum.mensura` before use; a bad archive yields an error
  message, never a crash.
- New library => add `"sarcina"` to `ludus_tessera/compile_probationes.sh`
  hand list only if a ludus_tessera build links it (L1: no); regenerate
  `./tools/compile_tests_fontes_generare.sh`.
- Commit only through `silva.commissio`, only on Fran's word.

## Header text (approved with the plan)

`include/sarcina.h`:

```c
/* sarcina.h - arca ZIP SOLUM LEGENDA (lector L1; spec
 * project-specs/lector-spec.md): directorium centrale, introitus per
 * nomen, data reposita (modus 0) aut DEFLATE (modus VIII, flatura),
 * CRC probatur. Arca tota in memoria (chorda; vocans filum legit). Non
 * sustinet: ZIP64, cryptographiam, modos alios, scripturam. Numquam
 * extra arcam legit: arca mala -> vitium (Anglice), numquam ruina. */

nomen structura Sarcina Sarcina;   /* opaca */

nomen structura {
    chorda  titulus;            /* nomen intra arcam ("META-INF/
                                 * container.xml"); in arca ipsa */
       i32  modus;              /* 0 repositum, VIII DEFLATE, alius =
                                 * non legitur */
       i32  mensura;            /* octeti expliciti */
       i32  mensura_compressa;  /* octeti in arca */
       i32  crc;
       i32  positio;            /* offset capitis localis */
       b32  directorium;        /* titulus in '/' desinit */
} SarcinaIntroitus;

/* arcam aperire: directorium centrale legitur; NIHIL et *vitium si
 * arca non est aut mala (vitium in piscina) */
Sarcina*
sarcina_aperire (
      chorda  datum,
    Piscina*  piscina,
     chorda*  vitium);

i32
sarcina_numerus (
    constans Sarcina* s);

/* introitus k (ordine directorii); NIHIL extra ambitum */
constans SarcinaIntroitus*
sarcina_introitus (
    constans Sarcina* s,
                 i32  k);

/* introitus per nomen exactum; NIHIL si abest */
constans SarcinaIntroitus*
sarcina_invenire (
    constans Sarcina* s,
              chorda  titulus);

/* data introitus (explicita, CRC probata) in piscina; FALSUM et
 * *vitium si modus ignotus, cryptographia, fines mali, CRC aut
 * mensura non congruunt */
b32
sarcina_legere (
    constans Sarcina* s,
    constans SarcinaIntroitus* e,
             Piscina* piscina,
              chorda* exitus,
              chorda* vitium);
```

## Review Focus

1. **Archive with a trailing comment** (the EOCD is not the last 22
   bytes): must still open. Pinned in Task 1.
2. **Data descriptor entries** (general-purpose flag bit 3: sizes and
   CRC zero in the LOCAL header, real values only in the central
   directory and after the data): must read using the central values.
   Pinned in Task 2.
3. **Local extra field longer/shorter than the central one** (common
   with real zip tools): data offset = local header + 30 + LOCAL name
   length + LOCAL extra length. Pinned in Task 3 (the independent
   `/usr/bin/zip` fixture) - a hand-built writer could mirror the
   reader's own mistake.
4. **Truncated or lying archives** (sizes or offsets past the end):
   error, never a read out of bounds. Pinned in Task 1 and Task 2.
5. **ZIP64 / encrypted / unknown method**: a clear refusal naming the
   reason. Pinned in Task 1 (ZIP64 marker 0xFFFFFFFF) and Task 2.

---

### Task 1: header, opening, the central directory

**Files:** Create `include/sarcina.h` (header text above),
`lib/sarcina.c`, `probationes/probatio_sarcina.c`; regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces:**
- Produces: the whole header API above; `sarcina_legere` may be a stub
  returning FALSUM with vitium "non iam" until Task 2.
- Test helper (in the test file only) `arcam_struere(Piscina* p,
  constans character* constans* nomina, constans character* constans*
  data, i32 n, b32 comprimere, constans character* commentum)` -> `chorda`:
  writes local headers + data (DEFLATE via `flatura_deflare` when
  `comprimere`), the central directory and the EOCD with the comment.

- [ ] **Step 1: Failing tests** (section `directorium`):
  - two entries `"mimetype"` (stored, `"application/epub+zip"`) and
    `"OEBPS/c1.xhtml"` (deflated, `"<p>Cranford</p>"`):
    `sarcina_numerus == II`; entry 0 titulus `"mimetype"`, modus 0,
    mensura 20; `sarcina_invenire(s, "OEBPS/c1.xhtml")` non NIHIL, modus
    VIII, mensura 15; `sarcina_invenire(s, "absens")` NIHIL;
    `sarcina_introitus(s, II)` NIHIL.
  - same archive with a 40-byte comment: opens, II entries.
  - a directory entry `"OEBPS/"`: `directorium` VERUM.
  - errors (NIHIL + vitium non-empty, no crash): empty chorda; 100 bytes
    of `'x'`; the archive cut 10 bytes before its end; the EOCD's
    central-directory offset pointing past the end; an entry whose
    central sizes are 0xFFFFFFFF (ZIP64 marker) -> vitium contains
    "ZIP64".
- [ ] **Step 2: Run** `./compile_tests.sh probatio_sarcina` — Expected:
  FAIL (no library).
- [ ] **Step 3: Implement** in `lib/sarcina.c`: EOCD search backwards
  from `mensura - XXII` to at most `mensura - XXII - 0xFFFF` for the
  signature `0x06054b50`; central records `0x02014b50` (46 bytes +
  name + extra + comment); entry count from the EOCD; every field read
  with little-endian helpers that check bounds.
- [ ] **Step 4: Run** — Expected: PASS (section directorium).
- [ ] **Step 5: Plants** (silva.planta): EOCD search only at the last
  22 bytes (red: comment case); no bounds check on the central offset
  (red: must fail cleanly - plant must COMPILE and the test must catch
  a wrong result, not a crash); ZIP64 check removed (red).

---

### Task 2: reading entries (stored, DEFLATE, CRC)

**Files:** `lib/sarcina.c`, tests section `legere`.

**Interfaces:**
- Consumes: Task 1's `Sarcina`, `SarcinaIntroitus`, helper
  `arcam_struere` (extended with a flag for data descriptors and one to
  corrupt a CRC).
- Produces: `sarcina_legere` (header signature).

- [ ] **Step 1: Failing tests** (section `legere`):
  - stored `"mimetype"` -> exactly `"application/epub+zip"`.
  - deflated entry of 5,000 bytes (a repeated sentence) -> identical
    bytes back, mensura 5000.
  - data descriptor variant (flag bit 3, zero sizes/CRC in the local
    header, descriptor `0x08074b50` after the data): reads correctly.
  - CRC corrupted in the central directory -> FALSUM, vitium contains
    "CRC".
  - method 12 (bzip2) in the central record -> FALSUM, vitium names the
    method; flag bit 0 (encrypted) -> FALSUM, vitium "encrypted".
  - local header offset past the end, or compressed size running past
    the end -> FALSUM, no crash.
- [ ] **Step 2: Run** — Expected: FAIL (stub).
- [ ] **Step 3: Implement** `sarcina_legere`: local header signature
  `0x04034b50`; data at `positio + XXX + local name length + LOCAL
  extra length`; sizes and CRC ALWAYS from the central record; stored =
  copy; DEFLATE = `flatura_inflare` then check `mensura` and
  `flatura_crc32`.
- [ ] **Step 4: Run** — PASS.
- [ ] **Step 5: Plants**: data offset with the CENTRAL extra length
  (red only with Task 3's fixture - record if green here); CRC check
  removed (red); sizes from the local header (red: data descriptor).

---

### Task 3: an independent archive, a real EPUB, docs

**Files:** `probationes/fixa/sarcina/exemplum.zip` (new, committed,
< 2 KB), tests section `oraculum`, `lib/sarcina.worklog.md`,
`project-specs/lector-spec.md` (§VIII L1 status).

- [ ] **Step 1: Make the fixture with an independent tool** (not our
  writer): a directory with `mimetype` (`application/epub+zip`, no
  newline) and `OEBPS/c1.xhtml` (a few hundred bytes of text);
  `zip -0 exemplum.zip mimetype && zip -r exemplum.zip OEBPS` (ruling at execution: NOT `-X` - it strips the extra fields this fixture exists to carry)
  (stored mimetype first, rest deflated, as EPUB requires). Record the
  exact command and the two files' contents in the test comment.
- [ ] **Step 2: Failing test** (section `oraculum`): read the fixture
  with `filum_legere_totum`; entries `mimetype` (modus 0),
  `OEBPS/` (directorium), `OEBPS/c1.xhtml` (modus VIII); contents equal
  the recorded originals. Expected: PASS on first run (Tasks 1-2 are
  done) - this test is the interoperability proof. Its red: re-run Task
  2's plant "data offset with the CENTRAL extra length", which a
  self-built archive (same writer, same mistake) cannot catch.
- [ ] **Step 3: Real EPUB check (manual, not committed)**: download
  `https://www.gutenberg.org/ebooks/394.epub3.images` to
  `~/.rhubarb/codices/cranford.epub`; a throwaway probe lists entries:
  `mimetype` first and stored, `META-INF/container.xml` present and
  readable, every entry reads with CRC OK. Record counts in the
  worklog.
- [ ] **Step 4**: worklog (format notes: EOCD search, central vs local
  sizes, data descriptors, local extra length), spec §VIII L1 DONE,
  lint (`VOCABULA_VIAE_ADDITAE="include/sarcina.h lib/sarcina.c
  probationes/probatio_sarcina.c" ./oratio/vocabula.sh -nova`, NOVA 0).
- [ ] **Step 5: Commit** on Fran's word via `silva.commissio` (gates
  radix/generata/aedilis); paths: header, lib, test, fixture, worklog,
  spec, this plan, generated link lists.
