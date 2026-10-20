# credo memoria — memoria per sectionem, custos cursoris, furca optionalis

*Born 2026-10-19. Prompted by the library audit: converted
`probatio_arbor_syntaxis` reached 49 GB of compressed memory while `ps` showed an RSS of
261 MB. Fran then asked whether credo could track the peak memory of a test. Ledger: wish 01M5AJAB1GMC
(credo memory per section), parent 01M5B3AQ1RXB (credo III); decree
01M5B812AVQ3 fixes WHERE the knobs live: signum `gravis`, env
`CREDO_MEMORIA_MB`, header constants `CREDO_MEMORIA_ORDINARIA_MB` /
`CREDO_MEMORIA_GRAVIS_MB`. Order agreed with Fran: C1 → C2 → C3.*

*Discipline note: the PULL rule applies. C3 (fork per section) is built
only if C1/C2 leave a real gap that a consumer names. Ceiling values are
MEASURED in C1 before any default is pinned.*

Status vocabulary: **FACTUM** (exists, cited) · **PROPOSITUM** (this spec)
· **APERTUM** (needs a decision or a measurement first).

## I. What already exists (FACTUM)

| Fact | Where |
|---|---|
| Per-arena peak: `piscina_summa_apex_usus` | `include/piscina.h:109` |
| Each credo section gets its own piscina (256 KiB initial, dynamic) | `lib/credo.c:2358` |
| Child hard time limit: `mora_maxima_ms` → SIGKILL + `PROCESSUS_ERROR_TEMPUS` | `include/processus.h:86-98`, poll loop `lib/processus_posix.c:605-625` |
| Child options struct `ProcessusOptiones` {directorium, ambitus} — 6 callers set fields one by one (mensura_contextus.c memsets; probatio_processus.c does NOT) | `include/processus.h:~108`, `lib/mensura_contextus.c:43-46`, `probationes/probatio_processus.c:772-796` |
| mensor records child max **RSS** per run via `getrusage(RUSAGE_CHILDREN)` — RSS is what hid the 49 GB | `tools/mensor.c:1002-1005` |
| sonda's memory guard polls `top -l 1 -pid … -stats mem` (footprint incl. compressed) against `SONDA_MEMORIA_MB` (4096) | `tools/sonda.sh:23,76,85` |
| macOS `proc_pid_rusage(pid, RUSAGE_INFO_V4, …)`: `ri_phys_footprint` (now) and `ri_lifetime_max_phys_footprint` (peak, never resets) | SDK `sys/resource.h:298,319`, `libproc.h:111` |
| Credo emits `INCIPIT` (flushed) before every section, so a killed suite still names the section that was running | `include/credo.h` (Sectiones, plagula) |
| fabrica's verdict reader ACCEPTS ONLY SECTIO lines of X or XI fields ("deformis" otherwise) | `lib/fabrica_probationes_c.c:290, 469-505` |
| officina compares every suite's **stdout** against a golden file | `officina/instrumenta/principalia/cursor.c:198` |

Nothing in `lib/` reads process memory today (`git grep proc_pid_rusage`
→ none).

## II. Constraints the design must honour

1. **Platform layer.** Footprint reading is macOS-specific. It goes in a
   tiny library with a neutral header and a `_macos.c` body, like
   `fenestra_tempus_macos.c` and `clonatio_macos.c`. Linux can follow
   later.
2. **Human output stays byte-identical** for passing sections. Every
   officina golden would break if each section printed a memory line.
   Memory goes to the machine TSV only, plus a named failure line when a
   ceiling is crossed.
3. **TSV and reader change together.** New SECTIO fields need the fabrica
   reader to accept XIV fields (and still X/XI) **in the same commit**,
   or every fabrica-run suite becomes "deformis".
4. **The peak can't be reset in-process.** `ri_lifetime_max_phys_footprint`
   only grows. In one process, a section can be credited with its arena
   peak (exact) and with how much it **raised** the process peak, not
   with its own true peak.
5. **An in-process check can't stop a runaway.** Credo only gets control
   at assertions and section ends. The arbor runaway was a loop with no
   assertions, so only something outside the process (the runner, or a
   parent in fork mode) can kill it.

## III. C1 — relatio (reporting)  PROPOSITUM

**T1. Library `memoria_processus`.**
`include/memoria_processus.h` + `lib/memoria_processus_macos.c`:
```c
nomen structura {
    i64 vestigium;        /* ri_phys_footprint, octeti (nunc) */
    i64 vestigium_apex;   /* ri_lifetime_max_phys_footprint */
    i64 residens;         /* ri_resident_size (RSS) - ad comparandum */
} MemoriaProcessus;

b32 memoria_processus_legere (s32 pid, MemoriaProcessus* exitus);
                                  /* pid ZEPHYRUM = ipse; FALSUM si legi nequit */
```
Suite `probatio_memoria_processus`:
- the self read succeeds and `apex >= vestigium`;
- after touching 64 MiB of malloc'd memory, `vestigium` grows by at least ~60 MiB;
- a non-existent pid gives FALSUM;
- a forked child read from the parent works.

Plant: return zeros, and the suite goes red.

**T2. Per-section accounting in credo.** For each section, record:
- the arena peak (`piscina_summa_apex_usus(contextus.piscina)` read just
  before `piscina_destruere`);
- the footprint at section end;
- the rise in the lifetime peak (peak after minus peak before).

The SECTIO TSV line grows from XI to **XIV** fields: `arena_apex`,
`vestigium`, `apex_auctus` (octets). The fabrica reader accepts X | XI |
XIV (constraint 3). Human output is unchanged.

Test: `credo_fumus` stage XV.
- One section allocates and touches 32 MiB in `c->piscina`; one is tiny.
- The heavy section's `arena_apex` must be ≥ 32 MiB and the tiny one's must be small.
- The fields must parse.
- The fabrica fumus stage that reads SECTIO lines must still pass with old X/XI files.

**T3. Into the annals and a report.** fabrica's per-section child spans
(`_sectiones_annalibus_inscribere`, `tools/fabrica_memoria.c:~572`)
carry the three numbers. A new `bin/mensor quaestio graves [n=N]` lists
the heaviest sections across the tree, by arena peak and by peak rise.
**This run is how the ceiling defaults get chosen** (APERTUM until
measured): the default goes comfortably above today's heaviest normal
section, and `gravis` is about 4× that.

**T4. Soft ceiling at section end.** If a finished section's arena peak
or peak rise exceeds its ceiling, a named failure is recorded:
```
 FRACTA (credo_memoria): sectio 'corpus magnum' apex arenae 4.3 GB > tectum 2048 MB (signum gravis abest)
```
- The ceiling is `CREDO_MEMORIA_MB` if set, else `CREDO_MEMORIA_ORDINARIA_MB`.
- A section with the `gravis` signum uses `CREDO_MEMORIA_GRAVIS_MB` instead.
- `gravis` joins the signa vocabulary, so `CREDO_SIGNA_OMITTE=gravis` skips those sections.

This catches sections that are heavy but finish. Runaways are C2's job.
Note the shared-vocabulary wish 01M5B81737BF when adding the word.

## IV. C2 — custos cursoris (runner guard)  PROPOSITUM

**T5. `processus` gains a memory ceiling, next to its time limit.**
- `ProcessusOptiones.memoria_maxima` (octets, 0 = no limit).
- The poll loop that already checks the time limit
  (`lib/processus_posix.c:605-625`, plus `processus_pulsare`) also calls
  `memoria_processus_legere(pid)`. Over the limit: SIGKILL and the new
  `PROCESSUS_ERROR_MEMORIA`, appended at the end of the enum so nothing
  renumbers, with an `error_descriptio` naming the footprint.
- **Audit the 6 `ProcessusOptiones` callers:** each must memset or set
  the new field (`probatio_processus.c:772-796` sets two fields with no
  memset).

Tests in `probatio_processus`:
- a child that touches memory past a 64 MiB limit is killed with MEMORIA well before its natural end;
- a child under the limit is untouched;
- a limit of 0 disables the check.

Plant: skip the poll, and the suite goes red.

**T6. fabrica runs probationes members with the ceiling.**
- **The value.** The runner can't tell which section is running, so its
  limit is the highest any section may use: `CREDO_MEMORIA_GRAVIS_MB`
  unless `FABRICA_MEMORIA_MB` overrides it. The finer per-section
  limits stay with T4.
- **Naming the section.** On a MEMORIA kill, the reader finds the
  `INCIPIT` line with no matching SECTIO and reports
  `RUIT: memoria excessa (N GB) in sectione 'X'`.
- **v1 suites** without sections get the same kill, reported as
  `totum`.

Test: a fabrica fumus stage with a fixture suite whose second section
runs away. The verdict must name that section and say "memoria", and the
first section's verdict must survive.

**T7. sonda.** Replace its `top` polling (`tools/sonda.sh:85`) with a
call to the same reader (a small `bin/` helper, or sonda runs its probe
via `processus`). One mechanism for every guard.

## V. C3 — furca per sectionem  APERTUM (PULL)

`CREDO_ISOLATIO=furca` would run each section in a forked child, reusing
the `credo_processus_*` machinery. The parent polls the child's
footprint against that **section's own** ceiling. That gives an exact
per-section peak and enforced per-section ceilings, and a crash or
runaway in one section no longer stops the rest.

**Cost:**
- The child has to send its assertion counts and first failure to the
  parent over a pipe. Credo's globals live in the child, so the
  `Totalis`/compendium would otherwise be wrong.
- One fork per section, roughly 1 ms each.
- Nesting with `CREDO_NON_RUIT` needs thought.

**Build only if** C1/C2 leave a named gap, for example a suite whose
sections can't be told apart by arena peak alone.

## VI. Order, commits, gates

| Commit | Content | Gate cost |
|---|---|---|
| 1 | T1 (new lib + suite) | small |
| 2 | T2 + fabrica reader XIV (constraint 3) + T4 | `credo.h` change → ~25 gates, ~30 min (slice A measured 1831 s) |
| 3 | T3 (annals + `mensor quaestio graves`), then set the default constants from its numbers | fabrica/mensor gates |
| 4 | T5 + caller audit | `processus.h` is widely included → large |
| 5 | T6 + T7 | fabrica gates |

## VII. AUDIENDA (not verified)

**Settled by T1 (2026-10-19, `probatio_memoria_processus` section IV and
a scratch comparison):**
- **A fork child starts its own lifetime peak at zero.** After the parent
  had touched 66 MiB, the child read peak 0 MiB and footprint < 1 MiB.
  Copy-on-write pages aren't counted. So in C3 the child's lifetime peak
  IS the section's own peak.
- **An exited, unreaped child is still readable** (`proc_pid_rusage`
  → VERUM), so T5/T6 can take a final reading.
- **`ri_phys_footprint` equals `top -stats mem`.** Finder read 287.0M
  against top's 287M (and RSS 45M). It's the number sonda trusts.
- **`ri_interval_max_phys_footprint` exists, but its reset
  (`proc_reset_footprint_interval`) is private SPI**, absent from
  `libproc.h`. Not used.

**Still open:**

- What a `proc_pid_rusage` call costs per poll (expected microseconds;
  not measured).
- The meaning of `piscina_summa_apex_usus`: peak of **used** bytes, not
  allocated blocks. Read its body before T2.
- Whether mensor's span schema can hold three more numbers without a
  schema change (T3).
- The default ceiling values, which T3 measures first.
- The Linux path (`/proc/<pid>/status` gives RSS peak, not footprint).
  Later.
