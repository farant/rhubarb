# psalmodia Plan, part 2 — Latin syllables, the fitter, the tool, the corpus gate

**Ledger:** parcum 01M6V5H5V1EGK1V37TP5M6BFN1 ("psalmodia"); one opus
per task below, filed at T1.

**STATUS: APPROVED 2026-11-12** — Fran accepted the plan and all five
FRAN recommendations as written. Opera: T1 01M7850MQZ, T2 01M7850R28,
T3 01M7850V2N, T4 01M7850XXX, T5 01M78510RB, T6 01M78513GF,
T7 01M7851695, T8 01M785197C.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention, no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note.

**Goal:** `auditio -psalmus psalmus.txt -signum "8 G"` prints the gabc
of a psalm sung to that tone, laid out the way the books print it
(bold accents, italic preparations, hollow added notes, bars, verse
numbers, Gloria Patri); `praeco antiphonale` shows and plays it. The
fitter reproduces the 1912 Antiphonale's own worked examples, and a
corpus gate compares it against GregoBase.

**Spec:** `project-specs/psalmodia-spec.md` — P1 (gabc out, nothing
else), P6 (pointed text), P7-P8 (syllables, accents from marks), P9
(total), P10 (looks like the books), P11 (rhythm is the converter's),
P12 (tool), §III (syllables), §IV (the accent rule, short texts), §V
(tests and corpus). The table's header comment (REGULA ACCENTUS,
TEXTUS DEFICIENS) states the fitting rule in the book's terms. Part 1
(`project-specs/psalmodia-plan-1.md`, DONE) gives the typed table and
the lookup.

**Architecture.** Two new libraries and two consumers:
- `syllabae_latinae` (generic, P7): one word (UTF-8 chorda) → its
  syllables as byte spans + the accented syllable from the marks.
  Knows nothing of psalms. `verba` in carmen and the accented-text
  wish (01M62BR70ZS0ZZT6TD9HPQGTNN) can use it later.
- `psalmodia` (the fitter): pointed text → verses of syllables (each
  with its notes and style) → gabc text. Reads `psalmodia_tabula`
  types, calls `syllabae_latinae`; no carmen, no gabc reader except in
  its tests (P1).
- `auditio -psalmus` with the table embedded (GENERATUM, the
  `carmina_infigere.sh` way, P2).
- A corpus gate `psalmodia-oraculum` (shape of `musicxml-oraculum`):
  syllable division, EUOUAE endings, the 35 written-out psalms.

**Measured before writing (2026-11-11):**
- **GregoBase division** (every syllable boundary in `chants.csv`):
  `ex-sul-tá-te` 35, `ex-au-dí-vit` 31 (x stays with its vowel before
  a vowel: prefix *ex-*; `e-xau-di` 2); `ju-stum` 128, `ju-stí-ti-a`
  40 (st goes forward; `jus-ti` 15); `lin-gua` 58, `san-guis` 24 (gu +
  vowel after n = one consonant); `cu-i` 58 vs `cui` 28 (hiatus is the
  norm); `De-us` 1400+; `mi-hi`, `ni-hil` (h a consonant); `e-jus`
  (j a consonant); **`Is-ra-el` 156 and `Is-ra-ël` 72** — the
  diaeresis is a real convention, and the plain spelling still means
  three syllables; `A-a-ron`.
- **The book's examples** (part 1 table): 246, by `pars`: terminatio
  125, mediatio 91, versus 17, semiversus 8, initium 3, flexa 2;
  `forma` A 28, B 20, C 16, D 17, abbreviata 18, monosyllaba 16,
  contracta 2, antiqua 1. 26 carry `nota="hebr."`: the book prints
  Hebrew names unaccented; the transcriber added an accent where the
  music shows one (`ex Basán`, `Ísrael in Dómino`) and left it off
  where the abrupt cadence takes the word (`ex Sion` = h h i, `in
  Ephrata` = h h h i). One example is a known exception: tone 2
  *Magníficat* (`nota="initium solemne"`, the solemn intonation, P4).
  A throwaway script fitted the other 245 (table header comment).
- **Output model**: GregoBase psalm 273 —
  `(c4) Lau(g)da,(h) Je(j)rú(j)sa(j)lem,(j) <b>Dó</b>(k)mi(jr)num :(j.)
  (:) * lau(j)da(j) ... <i>tu</i>(i)<i>um,</i>(j) <b>Si</b>(h gr)on.(g.)
  (::)2. Quó(j)ni(j)am`. Verse numbers glued to the first syllable.
- **auditio** already has `-tonus` (a switch: key analysis,
  `tools/auditio.c:2071`) — P12's example `-tonus "8 G"` collides;
  this plan uses `-signum`. `-gabc VIA` exists; its conversion path
  takes gabc text.
- **Embedding precedent**: `tools/carmina_infigere.sh` → `od -tx1`
  bytes into `lib/carmina_infixum.c` (GENERATUM, line 1 = generator).
- **Corpus gate precedent**: `tools/musicxml_silvestria.sh` — corpus
  absent = `OMISSA` (exit 0, said aloud), run under `tools/custos.sh`,
  results compared with committed seals.
- **Lookup over GregoBase** (R4): 1150 of 1315 named labels resolve;
  ~5900 chants have no label in a many-ending mode.

## FRAN — decisions (all five ACCEPTED as recommended, 2026-11-12)

1. **Hebrew names.** The fitter must know which words are Hebrew
   (hiatus *Is-ra-el*; the abrupt cadence at a half-verse end in tones
   that print one; no mark in the book). Recommended: **a small list
   in the fitter's code** (`VOCES_HEBRAICAE`: Israel, Jacob, Sion,
   David, Aaron, Juda, Ephrata, Basan, Sina, Jerusalem, Sabaoth, Amen
   — each with its division, its accent if the music shows one, and
   "abrupt"), extended when a psalm needs a new word; the input may
   still write the diaeresis (`Israël`). Alternatives: a marker in the
   pointed text (every psalm file carries it), or a data file.
2. **Tool surface**: `auditio -psalmus VIA -signum "8 G"` prints gabc;
   `-titulus T` names it; `-sine-gloria` drops Gloria Patri;
   `-antiqua` takes tone 6's ad libitum mediant; auditio's play flags
   play it. (`-tonus` is taken.)
3. **Gloria Patri** appended by default (P10), its pointed text fixed
   in the fitter: *Glória Patri, et Fílio, \* et Spirítui Sancto.* /
   *Sicut erat in princípio, et nunc, et semper, \* et in sǽcula
   sæculórum. Amen.*
4. **Where psalm texts live** (spec §VI q4): `musica/psalmi/`, one
   pointed `.txt` per psalm. T7 commits one, Ps 116 *Laudáte
   Dóminum, omnes gentes* (Vulgate, accents as AR 1912 prints them),
   for the smoke test. Which Office supplies the rest stays §VI q1.
5. **Corpus disagreements**: the gate never bends the table. Each
   check writes its disagreements (sorted: table, fitter, edition);
   Fran reviews the first list; it is committed as a manifest; the
   gate then fails only on a NEW disagreement (or a fixed one, so the
   manifest shrinks honestly).

## Global Constraints

- C89 with latina.h; Latin identifiers, comments, messages; every
  latina.h macro and every Roman numeral is a forbidden identifier
  (pitch letters `c d i l m v x` never as single-letter names).
- `chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED — offsets
  and lines `i32`, "none" `s32` -1 or NIHIL.
- **Totality (P9):** any bytes → a result (NIHIL only if memory
  fails) with located diagnostics `linea:columna: causa`; no crash, no
  hang; tested with random and truncated input under
  `CREDO_NON_RUIT`.
- **Accents from marks only (P8):** a word of three or more syllables
  without a mark (and not in the Hebrew list) is a diagnostic naming
  the word — never a guess.
- **Output (P10, P11):** gabc text only; accent syllables of a cadence
  in `<b>`, preparatory in `<i>`, added note `r`, `*(:)` at the
  mediant, `(::)` at the verse end, flex `†(,)`; plain puncta on the
  tenor, the table's own marks in the cadences, nothing else.
- Layers: `syllabae_latinae` includes chorda, piscina, utf8 only;
  `psalmodia` adds `psalmodia_tabula`, `syllabae_latinae`,
  chorda_aedificator; neither includes carmen or vicus.
- New C files through `./silva/scribe.sh`; aedilis tag in each header;
  `./tools/compile_tests_fontes_generare.sh` after a new lib.
- Every new test section born red by a plant that COMPILES
  (`silva.planta`), recorded in the Executed note and the worklog.
- Commit through `silva.commissio` with `portae=[('radix', ...)]`
  (plus the new gates from T7/T8 once they exist); tree frozen while
  gates run; Latin lint with `VOCABULA_VIAE_ADDITAE` for new files.
- **Headers reviewed by Fran before implementation:** T1 and T3 STOP.
- Committed fixtures meet the GregoBase sample rule (printed by 1930):
  the 82-chant sample, AR 1912's examples, hand-written cases. The
  corpus stays in `build/`.

## Review Focus

Five conditions the spec implies but no feature names; each pinned by
a test in the named task.

1. **Words without marks** — a three-syllable word with no accent, a
   Hebrew name (*Israel*, *Sion*), an enclitic (*Dominúsque*), a
   capitalised accented vowel (*Ómnes*), `ǽ`: each gets the right
   accent or a located diagnostic naming the word; never a silent
   guess. (T2 `_accentus_probare`, T4 `_textus_probare`.)
2. **Half-verses too short for their cadence** — a one-word half, a
   monosyllable alone, a first half shorter than intonation +
   mediant: the book's rules (drop the ending's first notes; fold the
   mediant's rest onto the first syllable; drop the mediant), else a
   defined result in the worklog, never a crash. (T5
   `_breves_probare`.)
3. **Messy text files** — CRLF, BOM, blank lines, a leading verse
   number (`1. Dixit`), `+` for `†`, a missing `*`, two `*`, a `†`
   after the `*`, punctuation glued to words: each read or reported
   with line and column. (T4 `_textus_probare`.)
4. **The odd tones** — tone 4 altera (c3), peregrinus (second-half
   tenor g and its own intonation on verse 1), in directum (no
   intonation, abrupt ending), tone 2 "D or A" (`clavis` on the
   ending), tone 6 antiqua. (T5 `_toni_rari_probare`.)
5. **Output that does not read back** — every gabc the fitter writes
   reads with `gabc_legere` with 0 vitia, the same syllable count, and
   the bars where we put them. (T6 `_reditum_probare`, run over every
   example and a random text.)

---

## Task T1: The header `syllabae_latinae.h` (STOP: Fran reviews)

**Files:**
- Create: `include/syllabae_latinae.h`, `lib/syllabae_latinae.worklog.md`
- Modify: `oratio/glossarium.stml` (only if the lint asks)

**Interfaces:** Produces (exact names; T2 implements):
- `SyllabaLatina { i32 initium; i32 finis; }` — byte span in the word
  as given; leading/trailing punctuation belongs to the first/last
  syllable's span; a forcing `-` belongs to no span.
- `VerbumLatinum { SyllabaLatina* syllabae; i32 numerus; s32
  accentus; b32 signatus; }` — `accentus` -1 = none (monosyllable, or
  3+ syllables unmarked); `signatus` VERUM when a mark gave it.
- `b32 syllabae_latinae_dividere(chorda verbum, Piscina* piscina,
  VerbumLatinum* exitus)` — FALSUM only if memory fails.
- `b32 syllabae_latinae_vocalis(chorda verbum, i32 positio, i32*
  longitudo)` — is a vowel letter (any case, accented, diaeresis, æ œ
  ǽ) at that byte; used by T4 for diagnostics.

- [x] **Step 1: Measure.** A `python3 -I` script over `chants.csv`:
  for every word (syllables joined), print the division classes
  that need a rule — x, st/sc/sp, mute + liquid, gu/qu/su + vowel,
  i/j initial, hiatus pairs (ui, eu, ae with diaeresis), ch/ph/th —
  each with its counts. Record in the worklog; the header's RULES
  comment is written from it (majority = rule, minority = edition,
  counted).
- [x] **Step 2: Draft the header** — head comment: USUS, RULES (from
  step 1, each with its GregoBase count), ACCENT (mark → its
  syllable; two syllables unmarked → the first; one → none; three+
  unmarked → none + `signatus` FALSUM), FORCING (`-` divides,
  diaeresis = hiatus), TOTALITY (no letter → zero syllables; never
  NIHIL output).
- [x] **Step 3:** `./silva/examen.sh`, `./silva/formator.sh -vitia`,
  `./oratio/vocabula.sh -nova`; header compiles alone under
  `VEXILLA_C89`.
- [x] **Step 4: STOP — show Fran the header.** Commit after approval.

---

## Task T2: Division and accent

**Files:**
- Create: `lib/syllabae_latinae.c`, `probationes/probatio_syllabae_latinae.c`
- Modify: the worklog, `compile_tests_fontes_generata.sh`

**Interfaces:** Consumes T1. Produces its two functions.

- [x] **Step 1: Measure.** Count the words in the 82 committed sample
  files (`probationes/fixa/gabc/gregobase/`) via `gabc_legere`
  (`verbum_incipit` starts a word): the oracle size for step 2.
- [x] **Step 2: Write the failing tests:**
  - `_divisio_probare`: hand cases `Dó-mi-nus`, `ex-sul-tá-te`,
    `ex-au-dí-vit`, `ju-stí-ti-a`, `lin-gua`, `san-guis`, `cu-i`,
    `De-us`, `mi-hi`, `e-jus`, `Chri-stus`, `pa-tris`, `ec-clé-si-a`,
    `sǽ-cu-la`, `Is-ra-ël` (diaeresis), `Is-ra-el` written
    `Is-ra-el` (forcing), `qui`, `et`; spans keep punctuation:
    `Dómino,` → last span `no,`.
  - `_exemplar_probare`: every word of the 82 sample files divides as
    the file divides it; disagreements listed in the test as known
    (word, theirs, ours) — the list starts EMPTY and grows only by a
    ruling recorded in the worklog.
  - `_accentus_probare`: `Dóminus` 0 signatus; `Deus` 0 not signatus;
    `et` -1; `Dominus` -1 not signatus; `Ómnes` 0; `sǽcula` 0;
    `Dominúsque` 2.
  - `_totalitas_probare`: empty, `"--"`, `"1."`, non-UTF-8 bytes,
    4000 random bytes under `CREDO_NON_RUIT`; spans always inside the
    word and increasing.
- [x] **Step 3: Run** `./compile_tests.sh syllabae` → FAIL (no link).
- [x] **Step 4: Implement** per the header's RULES: classify letters
  (vowel / consonant / mute / liquid; `u` after q, after g+vowel when
  preceded by n; initial `i`/`j` before a vowel = consonant); place
  nuclei (diphthongs ae oe au æ œ unless diaeresis); between nuclei,
  one consonant goes forward, mute + liquid goes forward, s + mute
  goes forward, `x` stays back, otherwise split before the last.
- [x] **Step 5: Run** → PASS. Plants: (a) treat `ae` as a diphthong
  even with diaeresis → `Is-ra-ël` red; (b) send `st` back → the
  `ju-stí-ti-a` case and the sample red. Restore.
- [x] **Step 6: Commit.**

---

## Task T3: The header `psalmodia.h` (STOP: Fran reviews)

**Files:**
- Create: `include/psalmodia.h`, `lib/psalmodia.worklog.md`

**Interfaces:** Produces (exact names; T4-T6 implement):
- `PsalmodiaStilus`: `PSALMODIA_STILUS_NULLUS` / `_ACCENTUS` (b) /
  `_PRAEPARATIO` (i).
- `PsalmodiaSyllaba { chorda textus; chorda notae; PsalmodiaStilus
  stilus; b32 verbum_incipit; i32 linea; i32 columna; }` — the bar
  marks `†` and `*` are syllables of their own with `notae` "†"/"*"
  (so a verse's notes join exactly like an `<exemplum notae>`).
- `PsalmodiaVersus { PsalmodiaSyllaba* syllabae; i32 numerus; i32
  linea; }`.
- `PsalmodiaAdmonitio { i32 linea; i32 columna; chorda causa; }`.
- `PsalmodiaOptiones { chorda titulus; b32 gloria; b32 antiqua; b32
  canticum; }` — `gloria` default VERUM; `canticum` = intonation on
  every verse.
- `PsalmodiaPsalmus { PsalmodiaVersus* versus; i32 numerus_versuum;
  PsalmodiaAdmonitio* admonitiones; i32 numerus_admonitionum; }`.
- `PsalmodiaFinis`: `PSALMODIA_FINIS_NULLUS` (text continues: the
  book's "initium" examples) / `_FLEXA` / `_MEDIATIO` /
  `_TERMINATIO`.
- `PsalmodiaPsalmus* psalmodia_aptare(constans PsalmodiaTonus* tonus,
  constans PsalmodiaClausula* terminatio, chorda textus, constans
  PsalmodiaOptiones* optiones, Piscina* piscina)` — the whole psalm.
- `PsalmodiaVersus* psalmodia_partem_aptare(constans PsalmodiaTonus*
  tonus, constans PsalmodiaClausula* clausula, PsalmodiaFinis finis,
  b32 intonatio, chorda textus, Piscina* piscina, PsalmodiaPsalmus*
  admonitiones)` — one half-verse (or the part before a flex), for
  the book's examples; `clausula` NIHIL for FLEXA and NULLUS.
- `b32 psalmodia_gabc_scribere(constans PsalmodiaPsalmus* psalmus,
  constans PsalmodiaTonus* tonus, constans PsalmodiaClausula*
  terminatio, constans PsalmodiaOptiones* optiones,
  ChordaAedificator* exitus)`.
- `b32 psalmodia_admonitiones_scribere(constans PsalmodiaPsalmus*,
  constans character* via, ChordaAedificator*)` — `via:linea:columna:
  causa`.

- [x] **Step 1: Measure.** Re-read the table's REGULA ACCENTUS and
  TEXTUS DEFICIENS; list, from the 246 examples, every input shape
  the header must name (bars, Hebrew, forms); confirm FRAN 1-3
  answers are reflected.
- [x] **Step 2: Draft the header** with the interfaces above. Head
  comment: POINTED TEXT GRAMMAR (UTF-8, one verse per line, `*`
  mediant, `†`/`+` flex, accents as printed, `-` forces a division,
  diaeresis = hiatus, leading `N.` dropped, blank lines and `%`
  comment lines skipped, CRLF/BOM accepted); THE RULE (verbatim
  from the table: backwards, accent moves back after its word's
  stress, proparoxytone last syllable may carry an accent, short
  texts); HEBREW (per FRAN 1); OUTPUT GRAMMAR (header `name: ;`
  `mode: ;` `annotation: 8 G;` `%%`, clef, syllable = text(notes),
  styles, bars, `N. ` on the first syllable, Gloria Patri as two
  last verses); TOTALITY.
- [x] **Step 3:** examen, formator, lint; compiles alone.
- [x] **Step 4: STOP — show Fran the header.** Commit after approval.

---

## Task T4: Reading pointed text

**Files:**
- Create: `lib/psalmodia.c`, `probationes/probatio_psalmodia.c`
- Modify: worklog, `compile_tests_fontes_generata.sh`

**Interfaces:** Consumes T2, T3. Produces the internal reader
(`_textum_legere`: text → lines → half-verse parts → words →
`VerbumLatinum` + Hebrew facts) used by T5; its effects are visible
through `psalmodia_aptare` with a tone whose cadences are all tenor.

- [x] **Step 1: Measure.** Count `+` vs `†` and leading numbers in the
  35 corpus psalms' text (strip notes); note in the worklog.
- [x] **Step 2: Write the failing tests** — `_textus_probare`: CRLF +
  BOM + blank lines + `% nota` → same verses as clean text; `1.
  Dixit` → number dropped; `+` = `†`; no `*` → admonitio "sine
  asterisco" at line:col of the line end, verse fitted as a second
  half only (ruling); two `*` → admonitio at the second; `†` after
  `*` → admonitio; `Dominus` unmarked (3 syll) → admonitio naming
  `"Dominus"` at its column; `Israel` → three syllables, no
  admonitio; `Sion` at a half-verse end → abrupt flag; punctuation
  stays in `textus`. Random bytes and truncated lines under
  `CREDO_NON_RUIT`.
- [x] **Step 3: Run** → FAIL. **Step 4: Implement.** Columns count
  characters (UTF-8 lead bytes), 1-based.
- [x] **Step 5: Run** → PASS. Plants: (a) Hebrew lookup case-sensitive
  only → `israel` red; (b) column counts bytes → the `Dóminus,
  Dominus` column assertion red. Restore.
- [x] **Step 6: Commit.**

---

## Task T5: The fitter — the book's 246 examples

**Files:** Modify `lib/psalmodia.c`, `probationes/probatio_psalmodia.c`,
worklog.

**Interfaces:** Consumes T4, part 1 (`psalmodia_signum_invenire`,
`psalmodia_mediationem_eligere`, `psalmodia_clausulam_monosyllabam`).
Produces `psalmodia_partem_aptare` and the verse-level body of
`psalmodia_aptare`.

- [ ] **Step 1: Measure.** Group the 246 examples by (pars, forma,
  tone); write the call each maps to (below) into the worklog.
- [ ] **Step 2: Write the failing tests:**
  - `_exempla_libri_probare`: for every example of the real table,
    the notes of the fitted syllables joined by spaces equal its
    `notae` joined by spaces. Mapping: `mediatio` →
    `partem_aptare(tonus, eligere(tonus, forma=="antiqua") [→
    clausulam_monosyllabam if casus], MEDIATIO, FALSUM, textus)`;
    `terminatio` → the ending named by `differentia` (or the only
    one) [→ monosyllabic sister], TERMINATIO; `flexa` → NIHIL, FLEXA;
    `initium` → NIHIL, NULLUS, intonatio VERUM; `semiversus` →
    mediant, MEDIATIO, intonatio VERUM; `versus` → `psalmodia_aptare`
    of the one line (`primus`, `canticum` from the example). Expected:
    **245 equal; the one `nota="initium solemne"` example is skipped
    and counted**; a failure prints tone, page, text, ours, theirs.
  - `_breves_probare` (Review Focus 2): a mediant of tone 1 on *Qui*
    → `hixih`; tone 7 on *Qui* → `ikj`; a tone 8 ending on *Deus* →
    first notes dropped (AR p. 6\*); tone 1 *Magníficat* as a first
    half → mediant dropped; an empty half → admonitio, no crash.
  - `_toni_rari_probare` (Review Focus 4): peregrinus verse 1 second
    half starts `hixi` on tenor g; verse 2 does not; in directum has
    no intonation; tone 4 altera notes under c3; tone 6 `antiqua`.
- [ ] **Step 3: Run** → FAIL. **Step 4: Implement** the rule as the
  table states it: per part, syllables of the cadence are assigned
  from the end (last locus → last syllable; each accent locus → its
  nominal syllable unless that syllable directly follows its word's
  stress, then the stress, and the skipped syllable takes the
  supervenient `r`); `anticipata`; preparatory loci before; tenor for
  the rest; intonation (verse 1, or every verse with `canticum`)
  takes the first syllables; flex lowers the last syllable, and the
  one before if the word is proparoxytone (book p. 5\*); styles: the
  syllables accent loci land on → ACCENTUS, preparatory → PRAEPARATIO.
- [ ] **Step 5: Run** → PASS. Plants: (a) never move an accent back →
  forms B/D red; (b) intonation on every verse → `_toni_rari` and
  the versus examples red. Restore.
- [ ] **Step 6: Commit.**

---

## Task T6: Writing gabc

**Files:** Modify `lib/psalmodia.c`, `probationes/probatio_psalmodia.c`,
worklog.

**Interfaces:** Consumes T5. Produces `psalmodia_gabc_scribere`,
`psalmodia_admonitiones_scribere`.

- [ ] **Step 1: Measure.** Read 3 of the corpus psalms' gabc for bar
  and number spacing; settle the exact byte form, write it in the
  header's OUTPUT GRAMMAR if T3 left a choice.
- [ ] **Step 2: Write the failing tests:**
  - `_scriptura_probare`: Ps 116's two verses on tone 8 G with
    Gloria Patri → an expected gabc string fixed in the test; with
    `gloria` FALSUM → two verses only; header `annotation: 8 G;`.
  - `_reditum_probare` (Review Focus 5): for every example and for
    the Ps 116 text: `gabc_legere` of our output → `gabc_successus`,
    0 VITIA, syllable count = ours + bars, `<b>`/`<i>` styles where
    ours are.
  - `_totalitas_probare`: random pointed text → gabc that reads back
    (or admonitiones only); `CREDO_NON_RUIT`.
- [ ] **Step 3: Run** → FAIL. **Step 4: Implement.** **Step 5: Run**
  → PASS. Plants: (a) no space before `*(:)` → reditum red (bar
  glued to text); (b) Gloria always added → `gloria` FALSUM case
  red. Restore.
- [ ] **Step 6: Commit.**

---

## Task T7: The table in binaries; `auditio -psalmus`

**Files:**
- Create: `tools/toni_psalmorum_infigere.sh`,
  `lib/toni_psalmorum_infixum.c` (GENERATUM), `include/toni_psalmorum_infixum.h`,
  `musica/psalmi/116.txt`
- Modify: `tools/auditio.c`, `tools/auditio_fumus.sh`, the producers
  list the generator needs (`build/consilium/producentes.stml` via
  fabrica, as `carmina_infigere.sh` is registered)

**Interfaces:** Produces `chorda toni_psalmorum_infixum_textus(Piscina*)`
and the flags of FRAN 2.

- [ ] **Step 1: Measure.** How `carmina_infixum` is registered with
  fabrica (generator, `GENERATUM` gate) — copy the registration.
- [ ] **Step 2: Write the failing tests:** a section in
  `probatio_psalmodia_tabula.c`: the embedded text reads with 0 vitia
  and equals the file byte for byte; `auditio_fumus.sh` cases: `-psalmus
  musica/psalmi/116.txt -signum "8 G"` → gabc on stdout that
  `auditio -gabc -` (or a temp file) accepts; `-signum "9"` → exit 1
  with `tonus "9" nullus`; `-sine-gloria` → no *Glória*.
- [ ] **Step 3: Run** → FAIL. **Step 4: Implement.** Diagnostics go to
  stderr as `via:linea:columna: causa`; exit 1 if any.
- [ ] **Step 5: Run** → PASS; open the output in `praeco antiphonale`
  and LISTEN once (note in the worklog what it sounds like). Plant:
  stale embedded copy (one byte changed) → the byte-equality
  assertion red. Restore.
- [ ] **Step 6: Commit** (GENERATUM gate green).

---

## Task T8: The corpus gate `psalmodia-oraculum`

**Files:**
- Create: `tools/psalmodia_oraculum.sh`,
  `probationes/probatio_psalmodia_corpus.c`,
  `probationes/fixa/psalmodia/discordiae.tsv` (after Fran's review)

**Interfaces:** Consumes everything above. The test reads the corpus
from `PSALMODIA_CORPUS` (= `build/gabc/bacor_v0.4/x`); absent → every
section prints `OMISSA` and passes.

- [ ] **Step 1: Measure.** Count, in the corpus: words (syllable
  check), antiphons with a resolvable label AND a written EUOUAE
  (spec §I: 610), written psalms whose tone label parses (of 35).
- [ ] **Step 2: Write the sections:**
  - `_syllabae_corporis`: every corpus word divided by us vs the
    file; disagreements by rule class.
  - `_euouae`: per antiphon, fit *sæculórum. Amen* to its ending,
    compare pitches as diatonic steps (clef-normalized; a constant
    offset = "transposita", reported apart); per label, agree / disagree.
  - `_psalmi_scripti`: strip notes and styles, keep accents, fit to
    the parsed tone, compare notes, styles, bars.
  Each writes `build/psalmodia/discordiae.tsv` (kind, id, label,
  ours, theirs) and compares it with the committed manifest.
- [ ] **Step 3: Run** → the first list. **STOP: Fran reviews it**
  (sorted into table errors, fitter errors, edition variants). Table
  errors are fixed in the table (P3: from the book, cited), fitter
  errors in T5's code with a test; what remains is committed as
  `discordiae.tsv`.
- [ ] **Step 4:** `tools/psalmodia_oraculum.sh` under `custos.sh`,
  registered as a porta like `musicxml-oraculum`. Plant: one manifest
  line deleted → gate red (a "new" disagreement). Restore.
- [ ] **Step 5: Commit; Executed notes; spec §VI q4 answered; ledger
  opera closed.** Report to Fran.

---

## Executed notes

- **T1 (2026-11-12, Fran approved the header).** Measured two
  witnesses: GregoBase (221 595 words) and the AR 1912 OCR
  (archive.org djvu text, 24 238 hyphenated words; only surviving
  hyphens count). Rulings: (1) house style = AR 1912 (longest onset
  forward: *ju-stí-ti-a*, *San-ctus*, *pro-pté-re-a*), not GregoBase's
  majority, which mixes three styles - cost if wrong: hyphens only;
  (2) prefix division kept (Fran: yes) - ab ad ex in ob per red sub
  trans + vowel, sic-ut, et-énim, with a look-alike list;
  (3) field `signatus` -> `notatus` (latina.h macro); (4) T2's sample
  oracle checks count + accent per word, hyphen style by AR hand
  cases. Tables in lib/syllabae_latinae.worklog.md.
- **T2 (2026-11-12).** `lib/syllabae_latinae.c` + test (5 sections,
  3866 assertions). Sample oracle per T1 ruling 4: 3295 words, count
  and accent equal except 3 listed (Israel - Hebrew; cui, Patre -
  hymn meter/elision). Rulings: eu exception narrowed to a word list
  (*eum* is *e-um*); AR evidence added sus- + consonant, nobis-cum,
  vobis-cum, super-ex-, and corrected the header example to
  *red-ém-ptor*; no dl/tl onset. Plants (a)-(c) red then green; (c)
  also red in the sample. Worklog has the details.
- **T3 (2026-11-12, Fran approved the header).** Hebrew words: two
  behaviours in the 26 examples (ordinary accent; "abrupt" =
  monosyllabic sister at a half-verse end, tenor held at a flex);
  list = division + accent + abrupt; Cedar added, Amen dropped.
  Clef = the ending's if given (II A c3), one per psalm. Verses
  numbered from 1 (GregoBase written psalms), Gloria unnumbered.
  Interface rulings (worklog): `sine_gloria` (zeroed options =
  defaults), `sine_intonatione`, the psalm carries tonus/terminatio,
  `partem_aptare` returns a one-verse psalm and picks the
  monosyllabic sister itself, gabc-special characters dropped with
  an admonition.
- **T4 (2026-11-12).** `lib/psalmodia.c` reader + provisional fitter,
  `probationes/probatio_psalmodia.c` (5 sections, 546 assertions,
  hand-built tone). Measured: `†` 13 / `+` 7 (one transcriber) in the
  35 corpus psalms, numbers `N.` (one `V/. 1`). Rulings (worklog):
  T4 fitter = tenor + the cadence's last note on each half's last
  syllable (sister if abrupt) - equals the real rule for one-locus
  cadences; sign errors dropped with an admonition, no `*` = all
  second half; vowel-less words join the neighbouring syllable with a
  space (T6 read-back decides); utf8 + runae included (runae_basis for
  the Hebrew key). Plants (a) capitals not folded -> *Israel* red,
  (b) byte columns -> column assertion red; both green after restore.
