# OFFICINA M5 Implementation Plan — dorsum nativum aarch64

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** medulla functions whose call graphs are closed compile to
arm64 in memory and run from machinula through NATIVUM descriptors.
They share the interpreter's world, and the interpreter stays the
oracle.

**Architecture:**
- Two evergreen lib/ libraries: an A64 encoder and an executable-memory
  layer.
- Three new officina modules:
  - medulla analyses (`officina_medulla_fluxus`);
  - the backend (`officina_dorsum`);
  - a native entry/escape module (`officina_effugium`).
- machinula gains a NATIVUM arm and a public bridge.
- Before any native code, the IR is fixed (typed calls, static arca,
  float branches) and the cursor baseline is repaired.

**Tech Stack:** C89 + latina.h, credo, officina pipeline (silva →
demissio → medulla → machinula), aedilis via bibliothecae,
`clang -cc1as -show-encoding` (re-pin script only).

**Spec:** project-specs/officina-m5-spec.md (approved by Fran
2026-11-03). Grounding is in officina-m5-fundamenta.md (facts are cited
as F#). Decisions are in officina-m5-interrogatio.md (cited as A1…G24).

## Global Constraints

**House rules**
- Latin identifiers (`VOCABULA_VIAE_ADDITAE=... ./oratio/vocabula.sh
  -nova` → NOVA 0; prefer renaming over glossary).
- latina.h macros are never identifiers (`nomen`, `per`, numerals,
  `magnitudo`...).
- i32/i64 are UNSIGNED (s32/s64 for signed); chorda is not
  NUL-terminated; 72-byte lines; full warning flags.

**Workflow**
- **Each task's HEADER TEXT** (new or changed declarations, plus the
  grammar where the vocabulary changes) goes to Fran before code.
  One task per turn, inline.
- **TDD:** red first. After green, a COMPILING plant per new assertion
  via `silva.planta(...)` (back up, then `cmp` after = IDEM; read
  `build/portae/officina.planta_rubra.acta`).
- New C files go through `./silva/scribe.sh`. Each touched C file:
  examen ACCIPE, formator back to baseline.
- **New officina sources** are added to every `<mandatum>` in
  `officina/aedificatio.stml` whose launcher links them, then
  `./tools/generata_iudicare.sh` until sana. This is the T3/T4 slip;
  do it in the same task.
- **Commit** only on Fran's word, one commit per task, via the detached
  commissio script. The task's opus (ledger) is closed by the commit.

**M5 boundaries**
- PACTUM AMBULATIONIS CANONICAE: nothing reorders, inserts into or
  deletes from a MedullaFunctio after demissio. Analyses and the
  backend keep their own orders beside it.
- MAP_JIT / `pthread_jit_write_protect_np` / `sys_icache_invalidate`
  appear ONLY in `lib/memoria_exsecutabilis_macos.c`.
- x18 is never allocated, read or written (F18).
- Halt texts are byte-identical to machinula's:
  - `"divisio per zephyrum"`, `"divisio: superfluxus"`
    (officina_machinula.c:2557, 2568);
  - sistere's causa;
  - `MachinulaExitusGenus` values.
- Native AAPCS64 (B5). Narrow values (B6): the caller extends to 32
  bits; the callee extends a narrow return; bits 32-63 are never
  trusted.
- Out of scope (spec §III): callbacks, native→interpreted calls,
  variadic bodies, jump tables, Mach-O, arm64e, debug info, caching.

## Review Focus

1. **A native call with more than 8 integer arguments, or a mix of
   integer and float arguments** (199 lib calls pass more than 8). The
   expected result is the same value as interpreted: stack arguments
   packed at natural size, classes kept apart. Test: Task 8,
   `"argumenta multa"`.
2. **A variadic call from native code** (`printf("%d %f", i, d)`). The
   output is identical to interpreted; every variadic argument goes on
   the stack in 8-byte slots, and float varargs are promoted to f64.
   Test: Task 8, `"variadica"`.
3. **A native function returning `chorda` by value to an interpreted
   caller.** The caller's `%reditus` slot holds both words (x0:x1 are
   stored by the entry thunk). Test: Task 8, `"chorda reddita"`.
4. **An interpreted loop calling a native leaf 10^6 times.** No growth
   in machinula's stack or the native code area, no handler
   re-install per call, and the exit value is unchanged. Test: Task 6,
   `"transitus iteratus"`.
5. **A native fault inside a credo suite** (credo has its own signal
   handlers). The result is a VITIUM halt, credo's handlers still work
   afterwards, and a fault outside JIT code still reaches credo. Test:
   Task 9, `"vitium et credo"`.

---

### Task 0: the cursor baseline, and one whole-world resolver

The spec's acceptance item VI.1. Parks: 01M6FZMA, 01M6FZMG, 01M6FG1G.

**Why one shape:** cursor.c and interpretare.c each have their own
`_bibliothecam_demittere`, which opens `lib/` and keeps every `.c`.
That is how testa.c and testa_android.c both enter the world. Both
switch to one resolver call that keeps one variant per header (posix).

**Files:**
- Modify: `officina/instrumenta/bibliothecae.{h,c}`. Add
  `bibliothecae_mundum_totum`.
- Modify: `officina/instrumenta/principalia/cursor.c`.
  - Its world comes from `bibliothecae_mundum_totum`.
  - Suites and `-enumerare` are collected in one Xar-based enumerator,
    `_suitas_colligere`, with no cap. The 256-entry arrays at :624/:832
    go.
  - `-enumerare` prints its count on stderr.
- Modify: `officina/instrumenta/principalia/interpretare.c`. The
  IGNOTA fallback uses `bibliothecae_mundum_totum`.
- Modify: `officina/aedificatio.stml` (cursor mandatum += bibliothecae.c
  and its closure). Then regenerate `officina/cursor_fontes_generata.sh`.
- Test: `officina/probationes/probatio_officina_bibliothecae.c`,
  `officina/probationes/probatio_officina_lanceatores.c`.
- Then re-bless: `officina/captare.sh` → `fixa/stdout_nativa/`.

**Interfaces:**
- Produces (header text to Fran first):

```c
/* Mundus totus: pro omni capite include/*.h, clausura variantis
 * "posix" (bibliothecae_ex_textu). Capita recusata (solum .m,
 * nimis magna, ignota) NON frangunt: in omissa_out (chorda
 * "caput: causa") colliguntur. Fructus: viae .c radici relativae,
 * sine duplicatis, ordine alphabetico (determinismus). NIHIL solum
 * si include/ legi nequit. */
Xar*
bibliothecae_mundum_totum (
    Bibliothecae* b,
          Piscina* piscina,
              Xar** omissa_out);
```

- [ ] **Step 1: Write the failing tests.**
  - bibliothecae, section `"mundus totus"`. Result:
    - contains `lib/testa.c`, not `lib/testa_android.c`;
    - contains `lib/fenestra_tempus_posix.c`, not `_macos.c`;
    - contains no `.m`;
    - is sorted, with no duplicates.
    
    `omissa_out` has an entry starting `include/fenestra.h`.
  - lanceatores, section `"cursor sola"`: `officina/cursor.sh -sola
    probatio_piscina`. The output has no `NEXUS FRACTUS` and contains
    the suite's pass line. Exit 0.
  - lanceatores, section `"cursor sine tecto"`: `cursor -enumerare`
    stdout contains `probationes/probatio_xar.c`. That suite is
    alphabetically past the old 256 cut. The line count is at least
    400.
  - lanceatores, section `"interpretare mundus totus"`: interpretare
    runs a lib function absent from a fixture index (point the index
    at an empty tsv; if no override flag exists, add `-index <via>`).
    It prints the expected result, with no `NEXUS FRACTUS`.
- [ ] **Step 2:** `./officina/compile_probationes.sh bibliothecae` and
  `lanceatores` → FRACTA (no symbol; testa_principale collision;
  xar absent).
- [ ] **Step 3:** Implement. The cursor world is a union of
  `mundum_totum` plus the existing `probationes/` companions
  (unchanged). Lowering stays once per cursor run (fork per suite
  unchanged).
- [ ] **Step 4:** Suites green. Run the full cursor once in the
  background (`caffeinate -i`). Record passed/total, stdout
  AEQUALIS/DIFFERT and the new suite count in the officina phase-log.
  Any suite that newly DECIPULAs because it is outside the posix world
  is listed by name, not fixed here.
- [ ] **Step 5:** Re-bless: `./officina/captare.sh` (pins goldens for
  every enumerated suite). Rerun the cursor; record the stdout column.
- [ ] **Step 6: Plants:**
  - `mundum_totum` without the variant → `"mundus totus"` FRACTA;
  - the enumerator capped at 256 → `"cursor sine tecto"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 7:** Ledger: 01M6FZMA, 01M6FZMG, 01M6FG1G → clausum.
  Commit on Fran's word.

### Task 1: typed call sites in medulla (B4)

**Files:**
- Modify:
  - `officina/fontes/officina_medulla.{h,c}` (types, fields, builders);
  - `officina/fontes/officina_demissio.c` (record the prototype at the
    call site, DC 2486-2637; extern signatures from semantica);
  - `officina/fontes/officina_medulla_textus.{h,c}` (print and parse);
  - `officina/vocabularium-medullae.md` §IV-V (sealed vocabulary: the
    grammar goes to Fran with the header);
  - `officina/fontes/officina_machinula.c` (builtin and extern calls
    read argument types; f32 immediates, XC ~3075-3110);
  - `officina/fontes/officina_conexio.c` (carry extern signatures
    through the link; conflicting signatures for one titulus =
    fractura honesta).
- Re-pin: every `officina/probationes/fixa/*.medulla` golden and any
  `.medulla` text in suites.
- Test: `probatio_officina_textus.c`, `probatio_officina_demissio.c`,
  `probatio_officina_machinula.c`, `probatio_officina_conexio.c`.

**Interfaces:**
- Produces (header text to Fran first):

```c
/* typus argumenti/parametri in limine vocationis (B4). Scalaris:
 * typus = MedullaTypus. Aggregatum valore (operandum = inscriptio
 * copiae vocantis, ut hodie): typus = MEDULLA_TYPUS_AGGREGATUM,
 * magnitudo_octetorum > 0, hfa_typus = F32/F64 si 1-4 membra eiusdem
 * typi fluitantis, NIHIL aliter. est_reditus: parametrum %reditus
 * (C7 conventio) - dorsum id in x8 aut x0:x1 transfert (B5). */
#define MEDULLA_TYPUS_AGGREGATUM MEDULLA_TYPUS_NUMERUS
nomen structura {
    s32 typus;
    i32 magnitudo_octetorum;
    s32 hfa_typus;
    b32 est_reditus;
} MedullaTypusLiminis;

/* MedullaFunctio += Xar* operanda_typi  (MedullaTypusLiminis,
 *                     parallelum stivae operandorum)
 * MedullaInstructio += s32 argumenta_fixa (vocare: numerus fixus;
 *                     -I = non variadica)
 * MedullaParametrum += MedullaTypusLiminis limen
 * MedullaSymbolum   += s32 signatura (index in modulus->signaturae;
 *                     -I nulla)
 * MedullaModulus    += Xar* signaturae (MedullaSignatura) */
nomen structura {
    s32  typus_reditus;
    b32  est_variadica;
    Xar* parametra;            /* MedullaTypusLiminis */
} MedullaSignatura;

s32
medulla_operanda_typata_addere (
                  MedullaFunctio* functio,
       constans MedullaOperandum* operanda,
    constans MedullaTypusLiminis* typi,
                             s32  numerus);
s32
medulla_signaturam_ponere (
               MedullaModulus* modulus,
                          s32  symbolum,
    constans MedullaSignatura* signatura);
```

- **Text grammar** (the proposal for Fran; it replaces spec §V's
  sketch):
  - Scalar types are as today.
  - An aggregate is `aN` (N bytes, integer class) or `hKxT` (HFA, e.g.
    `h2xf64`).
  - A `%reditus` parameter or argument is `rN` / `rhKxT`.
  - ` ... ` separates fixed from variadic arguments.

  ```
  %t2 = vocare.s32 $externa, s32 %t1, f64 2.5
  %t3 = vocare.s32 $scribens, s32 2 ... s32 %n, s64 7
  vocare $facere, r8 %t4, s32 %n, s32 %t5
  functio $duplicare (r8 %reditus, a8 %p) -> vacuum
  externum $facere (r8, s32, s32) -> vacuum
  externum $printf (i64, ...) -> s32
  ```
  
  `externum` lines come after `modulus`, before the first `datum` or
  `functio`, in symbol order.
- **Variadic arguments** carry their C-promoted types (f32 → f64; narrow
  ints → s32/i32).
- **machinula** treats every `a`/`r`/`h` operand as an i64 address
  (unchanged semantics).

- [ ] **Step 1: Write the failing tests.**
  - textus `"vocare typata"`: `dump(parse(t)) == t` for each grammar
    line above, plus a malformed line (`vocare.s32 $f, %t1` with no
    type). The malformed line fails to parse, with a causa naming
    `typus argumenti`.
  - demissio `"limina"`: lower `fixa/aggregata.c`:
    - `$voca` has `argumenta_fixa` 1 on `$scribens` and 2 on
      `$externa`;
    - the `$externa` arguments are typed s32, f64;
    - the `$facere` signature is `(r8, s32, s32) -> vacuum`;
    - `$duplicare`'s parameter 0 has `est_reditus`, and parameter 1 is
      `a8`.
  - demissio `"hfa"`: `struct V { double x, y; } f(struct V v);` gives
    the parameter `h2xf64`. A 24-byte struct gives `a24`.
  - machinula `"f32 immediatum"`: a hand-written module calls the
    builtin `sqrtf` (or the nearest f32 builtin in AEDIFICATA) with
    the literal `f32 4.0`, and it returns 2.
  - conexio `"signaturae discordes"`: two modules declare `$f` with
    different signatures → `conexio_nectere` FALSUM, and the querela
    names `f`.
- [ ] **Step 2:** The officina suites → FRACTA (compile: fields absent).
- [ ] **Step 3:** Implement. Re-pin goldens with the fusor (`-textus`),
  reviewing the diff by eye: only `vocare`/`functio`/`externum` lines
  may change.
- [ ] **Step 4:** Whole officina suite green, root suite green, and
  `cursor -sola probatio_piscina` unchanged.
- [ ] **Step 5: Plants:**
  - argument types dropped in the dumper → `"vocare typata"` FRACTA;
  - the variadic split off by one → `"limina"` FRACTA;
  - the f32 immediate passed as raw bits → `"f32 immediatum"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 2: arca as a static slot (E14), float branches (E15), census (E17)

**Files:**
- Modify: `officina/fontes/officina_machinula.c`:
  - at frame entry (around :2245-2271), allocate every arca of the
    function once, at offsets computed at freeze time;
  - the ARCA op returns the precomputed address and no longer bumps
    `stiva_cursor` (:2889-2922);
  - the RAMUS arm (:2974) stays bit-exact for integers.
- Modify: `officina/fontes/officina_demissio.c`. Where a condition of
  float type reaches `ramus` (:746 and other `_em(...RAMUS...)`
  sites), emit `%c = inaequalis.fT x, 0.0` first and branch on `%c`.
- Modify: `officina/vocabularium-medullae.md`. State the contracts:
  "arca = one static slot per instruction" and "ramus conditio is
  integral".
- Test: `probatio_officina_machinula.c`, `probatio_officina_demissio.c`.
- Census: `$S/m5_censura_e17.py` over `fusor -textus` of lib. Result in
  `officina_demissio.worklog.md`.

**Interfaces:**
- Produces: the medulla contracts above. No new public functions.
- Census outcome, a decision recorded for Task 7:
  - If zero sites feed an s32-written register into a 64-bit op without
    `extendere`, the backend refuses such a function with
    `"registrum s32 in usu 64"` (a named refusal, never silent).
  - If there are sites, the backend emits `sxtw` after the write, and
    the count is recorded.

- [ ] **Step 1: Write the failing tests.**
  - machinula `"arca in ansa"`: a module whose loop calls a function
    returning `a16` (an arca each iteration) 1000 times →
    `machinula_stiva_apex` after the run equals the apex after 1
    iteration. Exit 42.
  - demissio + machinula `"ramus -0.0"`: C `int f(void){ double d =
    -0.0; if (d) return 1; return 42; }` lowered and run → 42. The
    lowered text contains `inaequalis.f64`.
  - machinula `"arca recursio"`: a recursive function using an arca
    returns its own slot per frame (addresses distinct per depth).
- [ ] **Step 2:** Run → FRACTA (apex grows; -0.0 returns 1).
- [ ] **Step 3:** Implement both. Run the census; write the outcome.
- [ ] **Step 4:** officina + root green. Run the cursor once; DIFFERT
  must not rise against Task 0's number.
- [ ] **Step 5: Plants:**
  - ARCA still bumps → `"arca in ansa"` FRACTA;
  - the inaequalis emission skipped → `"ramus -0.0"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 3: `codificator_a64`, the A64 encoder library (G22-23)

**Files:**
- Create: `include/codificator_a64.h`, `lib/codificator_a64.c`.
- Create: `probationes/probatio_codificator_a64.c`.
- Create: `probationes/fixa/codificator_a64/semen.tsv` (the hand-written
  seed: one row per case, structured fields + assembly text).
- Create: `probationes/fixa/codificator_a64/aurum.tsv` (seed + clang's
  word; plus generated exhaustive rows).
- Create: `tools/codificator_a64_oraculum.sh` (the re-pin: generates the
  bitmask and FMOV rows, runs `clang -cc1as -triple arm64-apple-macosx
  -show-encoding`, writes aurum.tsv).

**Interfaces:**
- Produces (header text to Fran first):

```c
nomen enumeratio {           /* ~30 formae (F22); index stabilis */
    CODIFICATOR_A64_ADD_IMM = 0, CODIFICATOR_A64_SUB_IMM,
    CODIFICATOR_A64_ADD_REG, CODIFICATOR_A64_SUB_REG,
    CODIFICATOR_A64_ADDS_REG, CODIFICATOR_A64_SUBS_REG,
    CODIFICATOR_A64_SUBS_IMM,      /* cmp */
    CODIFICATOR_A64_AND_REG, CODIFICATOR_A64_ORR_REG,
    CODIFICATOR_A64_EOR_REG, CODIFICATOR_A64_ORN_REG,
    CODIFICATOR_A64_AND_IMM, CODIFICATOR_A64_ORR_IMM,
    CODIFICATOR_A64_EOR_IMM,       /* bitmask */
    CODIFICATOR_A64_LSLV, CODIFICATOR_A64_LSRV, CODIFICATOR_A64_ASRV,
    CODIFICATOR_A64_UBFM, CODIFICATOR_A64_SBFM, /* lsl/lsr/asr/
                                                 * sxt/uxt imm */
    CODIFICATOR_A64_MADD, CODIFICATOR_A64_MSUB,
    CODIFICATOR_A64_SDIV, CODIFICATOR_A64_UDIV,
    CODIFICATOR_A64_MOVZ, CODIFICATOR_A64_MOVK, CODIFICATOR_A64_MOVN,
    CODIFICATOR_A64_CSINC,         /* cset */
    CODIFICATOR_A64_CSEL,
    CODIFICATOR_A64_LDR_IMM, CODIFICATOR_A64_STR_IMM, /* + latitudo
                                                       * octetorum */
    CODIFICATOR_A64_LDRS_IMM,      /* ldrsb/ldrsh/ldrsw */
    CODIFICATOR_A64_LDR_REG, CODIFICATOR_A64_STR_REG,
    CODIFICATOR_A64_LDP, CODIFICATOR_A64_STP, /* pre/post/offset */
    CODIFICATOR_A64_ADR, CODIFICATOR_A64_ADRP,
    CODIFICATOR_A64_B, CODIFICATOR_A64_BL, CODIFICATOR_A64_B_COND,
    CODIFICATOR_A64_CBZ, CODIFICATOR_A64_CBNZ,
    CODIFICATOR_A64_BR, CODIFICATOR_A64_BLR, CODIFICATOR_A64_RET,
    CODIFICATOR_A64_FADD, CODIFICATOR_A64_FSUB, CODIFICATOR_A64_FMUL,
    CODIFICATOR_A64_FDIV, CODIFICATOR_A64_FNEG, CODIFICATOR_A64_FCMP,
    CODIFICATOR_A64_FCMP_ZERO, CODIFICATOR_A64_FMOV_REG,
    CODIFICATOR_A64_FMOV_IMM, CODIFICATOR_A64_FMOV_GEN, /* x<->d */
    CODIFICATOR_A64_FCVT,          /* s<->d */
    CODIFICATOR_A64_SCVTF, CODIFICATOR_A64_UCVTF,
    CODIFICATOR_A64_FCVTZS, CODIFICATOR_A64_FCVTZU,
    CODIFICATOR_A64_BRK,
    CODIFICATOR_A64_NUMERUS
} CodificatorA64Forma;

nomen structura {
    s32 forma;
    s32 latitudo;      /* XXXII/LXIV (W/X; S/D); octeti pro LDR/STR */
    s32 rd, rn, rm, ra;
    s64 immediatum;    /* imm/offset/displacement octetorum */
    s32 translatio;    /* LSL/shift quantitas; modus LDP/STP */
    s32 conditio;      /* CODIFICATOR_A64_EQ .. AL */
} CodificatorA64Instructio;

/* FALSUM si forma/latitudo/immediatum extra fines (nulla
 * approximatio tacita) */
b32
codificator_a64_codificare (
    constans CodificatorA64Instructio* instructio,
                                  i32* verbum_out);

/* algorithmi publici (F22: quinque soli logicam veram egent) */
b32 codificator_a64_bitmask (i64 valor, s32 latitudo,
                             i32* n_immr_imms_out);
s32 codificator_a64_constans (i64 valor, s32 latitudo, s32 rd,
                              i32* verba_out /* IV max */);
b32 codificator_a64_fmov_immediatum (f64 valor, i32* imm8_out);

/* emissor cum notis saltuum (fixups) */
nomen structura CodificatorA64 CodificatorA64;
CodificatorA64* codificator_a64_creare (Piscina* piscina);
b32  codificator_a64_emittere (CodificatorA64* c,
         constans CodificatorA64Instructio* instructio);
s32  codificator_a64_notam_creare (CodificatorA64* c);
vacuum codificator_a64_notam_ponere (CodificatorA64* c, s32 nota);
b32  codificator_a64_saltum_emittere (CodificatorA64* c,
         constans CodificatorA64Instructio* instructio, s32 nota);
/* notas resolvit; FALSUM + causa si saltus extra fines */
b32  codificator_a64_finire (CodificatorA64* c, chorda* causa_out);
i32  codificator_a64_numerus (constans CodificatorA64* c);
constans i32* codificator_a64_verba (constans CodificatorA64* c);
```

  (Exact parameter spelling is settled in the header text. `valor`
  in `bitmask` is a raw 64-bit pattern; i64 is unsigned here.)

- [ ] **Step 1: Write the failing tests** (root suite
  `probatio_codificator_a64`):
  - `"aurum"`: every row of aurum.tsv → `codificare` → word ==
    golden. The failure message prints the row's assembly text.
    Rows cover every forma × latitudo, with range edges at the
    maximum, maximum+1 (expect FALSUM) and minimum.
  - `"bitmask omnes"`: the 5,334 valid 64-bit patterns (generated
    rows) encode; 0 and ~0 are refused.
  - `"fmov omnes"`: the 256 FMOV immediates encode; 0.1 is refused.
  - `"constans"`: 0, 0xFFFF, 0x10000, -1, 0x300000000000,
    0x123456789ABCDEF0. Instruction count ≤ 4, and executing the
    sequence by hand-simulation yields the value. The words are in
    aurum.
  - `"notae"`: a forward b.cond, a backward b, cbz ±1 MiB edge → the
    displacements are correct. A displacement out of range →
    `finire` FALSUM with a causa.
- [ ] **Step 2:** `./compile_tests.sh codificator_a64` → FRACTA.
- [ ] **Step 3:** Write semen.tsv, the oracle script and the library.
  Run the oracle; commit aurum.tsv.
- [ ] **Step 4:** Suite green. `tools/codificator_a64_oraculum.sh`
  rerun → `cmp` aurum.tsv IDEM (the re-pin is byte-for-byte stable).
  Gates never call clang.
- [ ] **Step 5: Plants:**
  - bitmask rotate off by one → `"bitmask omnes"` FRACTA;
  - LDR scale dropped → `"aurum"` FRACTA;
  - fixup sign flipped → `"notae"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 4: `memoria_exsecutabilis`, the executable-memory layer (F18-19)

**Files:**
- Create: `include/memoria_exsecutabilis.h`,
  `lib/memoria_exsecutabilis_macos.c`.
- Create: `probationes/probatio_memoria_exsecutabilis.c`.

**Interfaces:**
- Produces (header text to Fran first):

```c
nomen structura MemoriaExsecutabilis MemoriaExsecutabilis;

/* Reservatio una (MAP_JIT) ad inscriptionem optatam; NIHIL + causa
 * si systema aliam dat (ut regio: nulla sedes tacite mota) aut
 * mmap deficit. */
MemoriaExsecutabilis*
memoria_exsecutabilis_reservare (
          Piscina* piscina,
    memoriae_index inscriptio_optata,
    memoriae_index mensura_octetorum,
           chorda* causa_out);

vacuum*        memoria_exsecutabilis_basis (
                   constans MemoriaExsecutabilis* m);
memoriae_index memoria_exsecutabilis_mensura (
                   constans MemoriaExsecutabilis* m);

/* scriptura permissa (filo huic - F16: per filum) */
vacuum memoria_exsecutabilis_aperire (MemoriaExsecutabilis* m);
/* scriptura clausa + icache invalidata pro [offset, offset+mensura)
 * - UNA vocatio, ne invalidatio oblivioni detur (anti-goal) */
vacuum memoria_exsecutabilis_sigillare (MemoriaExsecutabilis* m,
           memoriae_index offset, memoriae_index mensura);
vacuum memoria_exsecutabilis_liberare (MemoriaExsecutabilis* m);
```

  The constants live in M5's caller (Task 6):
  - hint `0x2FFFF0000000` (256 MiB below the region base);
  - size `0x10000000`.

- [ ] **Step 1: Write the failing tests.** Code words are written by
  hand (`movz w0,#42; ret` = 0x52800540, 0xD65F03C0). The call goes
  through a function pointer obtained by `memcpy` from the `vacuum*`
  (C89 -pedantic forbids the object→function cast).
  - `"scribere sigillare vocare"` → 42.
  - `"rescribere"`: the same address is rewritten to return 7, sealed,
    then called 200,000 times. Every call returns 7 (F16 staleness).
  - `"locus optatus"`: the base == hint. A second reservation at the
    same hint → NIHIL, causa non-empty.
  - `"furca"`: after `fork()`, the child calls the code and exits with
    its value. The parent sees 42.
- [ ] **Step 2:** `./compile_tests.sh memoria_exsecutabilis` → FRACTA.
- [ ] **Step 3:** Implement (the only file with MAP_JIT/pthread_jit_*/
  sys_icache_invalidate).
- [ ] **Step 4:** Suite green; examen ACCIPE.
- [ ] **Step 5: Plants:**
  - invalidate removed from `sigillare` → `"rescribere"` FRACTA;
  - the hint check removed → `"locus optatus"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 5: `officina_medulla_fluxus`, medulla analyses (G24)

**Files:**
- Create: `officina/fontes/officina_medulla_fluxus.{h,c}`.
- Create: `officina/probationes/probatio_officina_fluxus.c`.
- Fixtures: `fixa/fluxus.medulla`, `fixa/omnia.medulla`, plus inline
  texts for joins (`&&`/`?:`) and unreachable `@insequens_N`.
- Modify: `officina/compile_probationes.sh` (the object), and
  `officina/aedificatio.stml` (later launchers link it in Task 6).

**Interfaces:**
- Produces (header text to Fran first):

```c
nomen structura MedullaFluxus MedullaFluxus;

/* Analyses SOLUM LEGUNT - functio post computare octetim eadem
 * (pactum ambulationis canonicae). */
MedullaFluxus*
medulla_fluxus_computare (
                  Piscina* piscina,
    constans MedullaFunctio* functio);

/* praedecessores blocci (ordine indicum crescente) */
s32 medulla_fluxus_praedecessores (constans MedullaFluxus* f,
        s32 bloccus, constans s32** indices_out);
b32 medulla_fluxus_attingibilis (constans MedullaFluxus* f,
        s32 bloccus);
/* ordo dorsi: RPO bloccorum attingibilium, corporibus ansarum
 * contiguis; numerus_out */
constans s32* medulla_fluxus_ordo (constans MedullaFluxus* f,
        s32* numerus_out);
s32 medulla_fluxus_profunditas_ansae (constans MedullaFluxus* f,
        s32 bloccus);
/* positio linearis (in ordine dorsi, 2 per instructionem: usus,
 * definitio) <-> index planus (pactum) */
i32 medulla_fluxus_index_planus (constans MedullaFluxus* f,
        i32 positio);
/* intervallum vitae registri super positiones (multi-def et
 * iuncturae comprehensae); FALSUM si registrum numquam vivit */
b32 medulla_fluxus_intervallum (constans MedullaFluxus* f,
        s32 registrum, i32* initium_out, i32* finis_out);
/* VERUM si intervallum vocationem transit (dorsum: callee-saved) */
b32 medulla_fluxus_transit_vocationem (constans MedullaFluxus* f,
        s32 registrum);
```

- [ ] **Step 1: Write the failing tests.** The expectations are
  hand-computed and written in the test as tables.
  - `"praedecessores"`: on fluxus.medulla's loop function, a table of
    pred sets per block.
  - `"ordo"`: RPO with the loop body contiguous. An unreachable
    `@insequens_1` is absent and `attingibilis` FALSUM.
  - `"iunctura"`: an `a && b` join temp written in two blocks has one
    interval spanning both defs to the use.
  - `"usus sine definitione"`: a register read before any def gets an
    interval from function entry (F13: a zero-filled register file).
  - `"vocationem transit"`: a value live across `vocare` → VERUM; one
    dead before it → FALSUM.
  - `"pactum"`: the textual dump of every fixture is byte-identical
    before and after `computare`.
- [ ] **Step 2:** `./officina/compile_probationes.sh fluxus` → FRACTA.
- [ ] **Step 3:** Implement: iterative liveness over blocks in
  postorder; intervals as the [min, max] cover of live positions (no
  holes in M5).
- [ ] **Step 4:** Green; whole officina green.
- [ ] **Step 5: Plants:**
  - successors from `ramus` b/c swapped → `"praedecessores"` FRACTA;
  - the join's second def ignored → `"iunctura"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 6: NATIVUM, the entry thunk and the escape (C7, D10, D12)

A seam with no compiler yet: a hand-encoded native function, flipped
to NATIVUM, is called by machinula. A reviewer can accept this task
without the backend.

**Files:**
- Modify: `officina/fontes/officina_conexio.h`
  (`#define CONEXIO_SIGNUM_NATIVUM 4`; comment: index = entry address).
- Create: `officina/fontes/officina_effugium.{h,c}`:
  - the universal trampoline, emitted once into the JIT area with
    codificator_a64;
  - sigsetjmp entry;
  - a SIGSEGV/SIGBUS/SIGILL handler, installed ONCE on a sigaltstack,
    that chains to the previous handler when the pc is outside the JIT
    area or no entry is active;
  - the escape call used by trap stubs.
- Modify: `officina/fontes/officina_machinula.{h,c}`:
  - the VOCARE arm handles NATIVUM: it marshals arguments by the typed
    call (Task 1) and calls `officina_effugium_vocare`;
  - the escape maps to MachinulaExitus;
  - `machinula_currere`/`aperire` on a NATIVUM top function enter
    directly;
  - public entry points as below.
- Test: `officina/probationes/probatio_officina_nativum.c` (new suite,
  extended by Tasks 7-10).

**Interfaces:**
- Consumes: `memoria_exsecutabilis_*` (Task 4), `codificator_a64_*`
  (Task 3), `MedullaTypusLiminis` and the typed calls (Task 1).
- Produces (header text to Fran first):

```c
/* officina_effugium.h */
#define OFFICINA_NATIVUM_LOCUS   0x2FFFF0000000UL  /* F19 */
#define OFFICINA_NATIVUM_MENSURA 0x10000000UL

nomen enumeratio {
    EFFUGIUM_REDDITUM = 0,  /* functio rediit */
    EFFUGIUM_SALTUS,        /* trap: officina_effugium_salire */
    EFFUGIUM_VITIUM         /* signum hardware in codice nativo */
} EffugiumGenus;

nomen structura {
               s32 genus;
               s32 signum_systematis;  /* SIGSEGV... (VITIUM) */
               s64 codex;              /* SALTUS: causa a trap */
    memoriae_index pc, fp;             /* VITIUM/SALTUS: positio */
               i64 x[II];              /* x0:x1 reditus */
               f64 d[IV];              /* d0-d3 reditus (HFA) */
} EffugiumFructus;

nomen structura {                      /* argumenta in AAPCS64 */
    i64 x[VIII]; s32 numerus_x;
    f64 d[VIII]; s32 numerus_d;
    i64* stiva;  s32 numerus_stivae;   /* verba, iam ordinata */
    i64 x8;                            /* sret > 16 octeti */
} EffugiumArgumenta;

/* praeparat aream (reservare + trampolinum) et tractatorem
 * signorum semel; FALSUM + causa */
b32 officina_effugium_parare (Piscina* piscina, chorda* causa_out);
MemoriaExsecutabilis* officina_effugium_area (vacuum);
EffugiumFructus officina_effugium_vocare (vacuum* introitus,
        constans EffugiumArgumenta* argumenta);
/* a codice nativo (trap stub) vocatur; numquam redit */
vacuum officina_effugium_salire (s64 codex, memoriae_index pc,
        memoriae_index fp);

/* officina_machinula.h */
/* functionem nominatam in signum NATIVUM vertit (introitus datus);
 * FALSUM si titulus ignotus */
b32 machinula_nativum_ponere (Machinula* machinula,
        chorda titulus_functionis, vacuum* introitus);
```

- [ ] **Step 1: Write the failing tests** (`probatio_officina_nativum`).
  The hand-encoded functions are built with codificator_a64 into the
  effugium area.
  - `"nativum vocatum"`: a module `$main` calls `$f(s32 %a) -> s32`.
    `$f` is flipped to a native `add w0,w0,#1; ret` (with a frame
    record). Calling `$f(41)` from interpreted `$main` → 42.
  - `"nativum principale"`: `$main` itself native (`movz w0,#42`) →
    `machinula_currere` BENE, codex 42.
  - `"saltus"`: a native stub calls `officina_effugium_salire(7, ...)` →
    EFFUGIUM_SALTUS, codex 7. machinula maps this to a halt. The
    machinula is reusable afterwards (`currere` again → 42).
  - `"vitium"`: a native `ldr x0,[xzr]` → VITIUM halt with a position.
    The process survives.
  - `"transitus iteratus"` (RF4): interpreted loop of 10^6 calls to a
    native leaf → correct sum; `machinula_stiva_apex` constant;
    sigaction installed once (a counter exposed for tests).
- [ ] **Step 2:** `./officina/compile_probationes.sh nativum` → FRACTA.
- [ ] **Step 3:** Implement. The handler: if `effugium active && pc in
  area`, siglongjmp; else re-dispatch to the saved previous action
  (SIG_DFL → reset and re-raise).
- [ ] **Step 4:** Green; officina + root green.
- [ ] **Step 5: Plants:**
  - the handler not chaining → `"vitium"` FRACTA or credo ruins
    (record which);
  - the NATIVUM arm calling with untyped args → `"nativum vocatum"`
    FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 7: backend I — integer leaf functions, and `interpretare -nativum`

**Files:**
- Create: `officina/fontes/officina_dorsum.{h,c}` (split
  `officina_dorsum_registra.c` if the allocator passes ~800 lines).
- Modify: `officina/fontes/officina_machinula.{h,c}`
  (`machinula_nativum_compilare`).
- Modify: `officina/instrumenta/principalia/interpretare.c` (`-nativum`
  flag: compile the target before running; on refusal print the causa
  and run interpreted, exit unchanged).
- Modify: `officina/aedificatio.stml` (interpretare, colloquium, cursor,
  vindex mandata += dorsum, fluxus, effugium, codificator_a64,
  memoria_exsecutabilis_macos). Then regenerate the generata.
- Test: `probatio_officina_nativum.c`, `probatio_officina_lanceatores.c`.

**Decisions this task carries (spec §VII):**
- **Registers:**
  - x0-x7 arguments;
  - x9-x15 caller-saved temps;
  - x16/x17 scratch (x16 = host-call target);
  - x19-x28 callee-saved, for intervals that cross a call
    (`transit_vocationem`);
  - x29 fp, x30 lr, never x18;
  - d0-d7/d16-d31 temps, d8-d15 callee-saved (Task 9).
- **Allocation:** linear scan over `medulla_fluxus` positions. Spill the
  interval with the furthest end, weighted by loop depth. Spills go to
  fixed frame slots.
- **Frame:** `stp x29,x30,[sp,#-N]!; mov x29,sp` ALWAYS (F21), then
  callee-saved pairs, spill slots and arca slots (Task 8). 16-byte
  aligned.
- **Selection:**
  - compare + `ramus` fold into `cmp; b.cond` when the compare result
    is single-use and immediately precedes the `ramus` in the same
    block;
  - immediates where encodable, else `codificator_a64_constans`;
  - W ops for 32-bit suffixes, with explicit sxt/uxt at
    `extendere`/`amputare` (B6);
  - narrow returns are extended by the callee.
- **Division:** emit checks before sdiv/udiv. Zero → trap stub with the
  causa index for `"divisio per zephyrum"`. Signed MIN/−1 →
  `"divisio: superfluxus"`. Both go through `officina_effugium_salire`.
- **sistere:** a trap stub with the causa index and the instruction's
  flat index.
- **pc map:** per function, a sorted array `{offset_verbi, index_planus}`
  at the start of each IR instruction. Lookup is a binary search.
- **Refusal reasons** (exact strings):
  - `"vocat $<f> interpretatum"`
  - `"operatio <op> nondum nativa"`
  - `"registrum s32 in usu 64"` (if Task 2 found no sites)
  - `"functio variadica"`

**Interfaces:**
- Produces (header text to Fran first):

```c
nomen structura OfficinaDorsum OfficinaDorsum;

/* hospes: inscriptiones aedificatorum purorum + thunk pontis
 * (Task 8 implet; hic vacua licet) */
OfficinaDorsum*
officina_dorsum_creare (
                 Piscina* piscina,
       constans Conexio* conexio,
    MemoriaExsecutabilis* area);

/* functionem (index conexionis) et clausuram vocatorum compilat;
 * FALSUM + causa nominata (nihil in area mutatum quod vocari
 * possit). */
b32 officina_dorsum_compilare (OfficinaDorsum* d,
        s32 functio_index, chorda* causa_out);
vacuum* officina_dorsum_introitus (constans OfficinaDorsum* d,
        s32 functio_index);
/* pc -> (functio, index planus); FALSUM extra codicem dorsi */
b32 officina_dorsum_positio (constans OfficinaDorsum* d,
        memoriae_index pc, s32* functio_index_out,
        i32* instructio_out);

/* officina_machinula.h */
b32 machinula_nativum_compilare (Machinula* machinula,
        chorda titulus_functionis, chorda* causa_out);
```

- [ ] **Step 1: Write the failing tests.**
  - nativum `"fixturae I"`: summa, arithmetica, bitalia, comparationes,
    fluxus. Each is compiled with `machinula_nativum_compilare("main")`
    and run → 42, and the interpreted run → 42 (both in the same
    test).
  - nativum `"divisio"`: an inline module dividing by a zero argument →
    halt genus and nuntius == interpreted (`"divisio per zephyrum"`).
    INT_MIN/−1 → `"divisio: superfluxus"`.
  - nativum `"sistere"`: sistere.medulla natively gives genus, causa and
    `machinula_positionem_inspicere` == interpreted.
  - nativum `"recusatio"`: `$f` calling an interpreted-only `$g`
    (which uses an op the backend doesn't yet emit) → FALSUM, causa
    `"vocat $g interpretatum"`. Running still gives the interpreted
    result.
  - nativum `"latitudo"`: `extendere.s32.s64` of −1 → −1;
    `amputare.i64.i32` of 0x1_0000_0005 → 5; a narrow s8 return of 200
    → −56 seen by the interpreted caller.
  - lanceatores `"interpretare -nativum"`:
    `./officina/interpretare.sh -nativum fasti_est_bissextilis 2024` →
    stdout `1`, exit 0.
- [ ] **Step 2:** → FRACTA.
- [ ] **Step 3:** Implement.
- [ ] **Step 4:** Green; officina + root + generata sana.
- [ ] **Step 5: Plants:**
  - the cmp fold uses the wrong condition for `minor.i32` (unsigned) →
    `"fixturae I"` FRACTA;
  - the division check removed → `"divisio"` FRACTA;
  - the pc map off by one → `"sistere"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 8: backend II — memory, data, calls, builtins, aggregates (B5, D11, E14)

**Files:**
- Modify: `officina/fontes/officina_dorsum.c`.
- Modify: `officina/fontes/officina_machinula.{h,c}`. The bridge goes
  public; the AEDIFICATA table entries gain a `purum` flag, confirming
  each of the ~33/~28 split (AUDIENDA).
- Test: `probatio_officina_nativum.c`.

**Decisions:**
- **arca:** a static frame slot per ARCA instruction (Task 2
  contract).
- **legere/scribere:** `[base,#k]` folding when an `addere` with an
  immediate feeds only the address.
- **locus $sym:** adrp+add (the area is placed within ±4 GiB of the
  globals).
- **Calls:**
  - native→native is `bl` (or `blr` via the descriptor's index for an
    indirect call);
  - an indirect call through a non-NATIVUM descriptor → trap stub
    `"vocatio indirecta non nativa"` (a halt, not a crash: C9's
    boundary at runtime);
  - host calls are `ldr x16,[slot]; blr x16` through a per-area table
    (F20).
- **Builtins:**
  - pure: slot = the libc address;
  - stateful: slot = a C thunk `machinula_pons_nativus`, which takes
    the Machinula* from a fixed slot, the builtin index in x17, and
    arguments as in AAPCS64. It calls the same `_aedificatum_vocare`
    channel.
- **Aggregates (B5):**
  - ≤16 B integer class: in x registers;
  - HFA: in d registers;
  - >16 B: by the address of a copy (already medulla's form).
  - `r8`/`r16` returns: x0:x1 stored into the caller's slot. Larger:
    x8.
  - The entry thunk (Task 6) does the same for an interpreted caller.

**Interfaces:**
- Produces (header text to Fran first):

```c
/* officina_machinula.h - pons publicus (D11) */
s32 machinula_aedificatum_quaerere (chorda titulus);   /* -I */
b32 machinula_aedificatum_purum (s32 aedificatum);
vacuum* machinula_aedificatum_inscriptio (s32 aedificatum);
                                     /* purum: functio libc */
/* thunk pontis: dorsum eius inscriptionem in tabulam hospitis
 * ponit (signatura AAPCS64 generica; x17 = aedificatum) */
vacuum* machinula_pons_nativus (vacuum);
vacuum  machinula_nativum_machinulam_ponere (Machinula* machinula);
```

- [ ] **Step 1: Write the failing tests.**
  - nativum `"fixturae II"`: memoria, data → 42 native and
    interpreted. (vocationes uses f32 compares, so it is Task 9.)
  - `"argumenta multa"` (RF1): `$f` takes 10 s32 + 3 f64 arguments,
    called native→native and interpreted→native → the same sum.
  - `"variadica"` (RF2): native `$g` calls `printf("%d %.2f %s\n", 7,
    2.5, "x")`. The capture (machinula_ansam_ponere to a tmpfile) ==
    the interpreted capture.
  - `"chorda reddita"` (RF3): native `$h` returns `chorda` by value
    (`r16`) to an interpreted caller and to a native caller. Both words
    are correct.
  - `"aedificata"`: native `strlen`, `memcpy` (pure, direct) and
    `malloc` + `free` (bridged: the address lies in the region's
    acervus) agree with interpreted.
  - `"indirecta"`: native call through a descriptor of a native
    function → OK. Through an interpreted descriptor → halt
    `"vocatio indirecta non nativa"`.
- [ ] **Step 2:** → FRACTA.
- [ ] **Step 3:** Implement.
- [ ] **Step 4:** Green; officina + root green.
- [ ] **Step 5: Plants:**
  - variadic args in registers → `"variadica"` FRACTA;
  - only x0 stored for r16 → `"chorda reddita"` FRACTA;
  - malloc called direct (not bridged) → `"aedificata"` FRACTA.
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 9: backend III — floating point and faults

**Files:**
- Modify: `officina/fontes/officina_dorsum.c`,
  `officina/fontes/officina_effugium.c`, `officina_machinula.c` (the
  halt report prints native frames from the fp chain via
  `officina_dorsum_positio`, tagged `[nativum]`).
- Test: `probatio_officina_nativum.c`.

**Decisions:**
- f32/f64 ops on S/D registers. FP immediates via FMOV-imm, else a
  literal pool in the area. Float compares via `fcmp` + cset with the
  ordered conditions C requires (NaN → false for <, <=, >, >=, ==;
  true for !=).
- `integrare` uses fcvtzs/fcvtzu. Out-of-range inputs are a named
  difference, "machinula strictior" (E16).
- A native fault → VITIUM with the pc mapped to (function, flat index).
  Stack overflow is caught on the sigaltstack and becomes VITIUM
  `"stiva nativa exhausta"`.

**Interfaces:**
- Consumes: Tasks 6-8. Produces: no new public names (the halt-report
  format gains `[nativum]` frames).
- Produces for Task 10: the differential classes
  `AEQUALE | DIFFERT | MACHINULA_STRICTIOR` as a test helper in the
  nativum suite.

- [ ] **Step 1: Write the failing tests.**
  - nativum `"fixturae III"`: conversiones, vocationes → 42 native and
    interpreted.
  - `"nan"`: comparisons with NaN, native == interpreted == the C table
    above.
  - `"ramus -0.0"` natively → 42 (Task 2's fix, now through the
    backend).
  - `"nullus"`: native `*(int*)0` → VITIUM, position == the flat index
    of the `legere`; the halt report contains `[nativum]` and the
    function name.
  - `"vitium et credo"` (RF5): after `"nullus"`, a deliberate fault
    outside JIT code inside `CREDO_NON_RUIT` is still caught by credo.
  - `"stiva"`: unbounded native recursion → VITIUM `"stiva nativa
    exhausta"`, and the process lives.
- [ ] **Step 2:** → FRACTA.
- [ ] **Step 3:** Implement.
- [ ] **Step 4:** Green; officina + root green.
- [ ] **Step 5: Plants:**
  - an unordered fcmp condition used for `<` → `"nan"` FRACTA;
  - the sigaltstack skipped → `"stiva"` FRACTA (the process dies: the
    plant shows the suite RUINA, which counts as red).
  
  Restore, IDEM.
- [ ] **Step 6:** Commit on Fran's word.

### Task 10: the C set, the timing pair, RELATIO (VI.7, VI.9)

**Files:**
- Modify: `probatio_officina_nativum.c` (the C set).
- Create: `officina/probationes/fixa/nativum/` workloads:
  - `fasti_ansa.c` (fasti_est_bissextilis over years 1..10^6);
  - `nucleus_integer.c` (a CRC32 or sieve kernel);
  - `nucleus_memoria.c` (a struct array sort/sum).
- Create: `officina/mensura_nativa.sh`. It runs each workload
  interpreted and `-nativum` 5×, takes the median wall time plus
  `machinula_numerus_instructionum`, and prints a table.
- Modify: `officina/phase-log.md` (M5 RELATIO), `officina/CLAUDE.md`
  (the native path), the officina worklogs.

- [ ] **Step 1: Write the failing tests.**
  - nativum `"scalaria"` / `"fluxus C"`: every function in
    `fixa/scalaria.c` and `fixa/fluxus.c` over an input table (edges:
    0, ±1, INT_MIN, INT_MAX, and NaN for floats) → the differential
    class AEQUALE, or MACHINULA_STRICTIOR with its named reason. Zero
    DIFFERT.
  - `"fasti"`: `fasti_est_bissextilis` over {1900, 2000, 2023, 2024,
    2100, 2400} == interpreted.
- [ ] **Step 2:** → FRACTA for any function the backend still refuses
  (fix, or record the refusal reason; a refusal is acceptable only for
  a named non-goal).
- [ ] **Step 3:** Fix what the C set exposes.
- [ ] **Step 4:** Green. Run `mensura_nativa.sh`. Write RELATIO with the
  measured table, replacing the unsourced 30-600× estimates (F25), and
  correct the docs that cite them.
- [ ] **Step 5:** Every gate green (spec VI.10). The ledger closes
  parcum 01KXJ2NV with a note, and M5b items are filed as desiderata:
  - callbacks;
  - re-entrant machinula;
  - the cursor native lane;
  - sessio caching.
  
  Commit on Fran's word.

---

## Self-review notes

- **Spec coverage:** every VI item maps to a task.

  | VI item | task |
  |---|---|
  | VI.1 | 0 |
  | VI.2 | 1, 2 |
  | VI.3 | 3 |
  | VI.4 | 4 |
  | VI.5 | 5 |
  | VI.6 | 7, 8, 9 |
  | VI.7 | 7 (smoke), 10 |
  | VI.8 | 9 |
  | VI.9 | 10 |
  | VI.10 | every commit |

  §VII builder decisions are settled:
  - task cut, registers, selection and pc map: Task 7;
  - placement: Tasks 4/6;
  - E17: Task 2;
  - the bridge: Task 8.
  - Latin names: proposals, final per header approval.
- **Changed from the spec:**
  - The spec's backend I-III became Tasks 6-9. NATIVUM, the thunk and
    the escape were split out (Task 6) so the seam is reviewed before
    the compiler.
  - Placement is decided at hint 0x2FFFF0000000, size 256 MiB.
- **AUDIENDA carried forward:**
  - Task 0 may find suites that DECIPULA in the posix world (they are
    listed, not fixed).
  - The pure/stateful split is confirmed entry by entry in Task 8.
