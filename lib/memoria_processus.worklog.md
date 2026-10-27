# memoria_processus worklog

## 2026-10-19 - T1 of project-specs/credo-memoria-spec.md

Born for credo memory accounting: `probatio_arbor_syntaxis` held 49 GB
compressed while RSS said 261 MB. macOS compresses idle pages, and RSS
excludes them; the physical footprint (`ri_phys_footprint`, what `top`
calls MEM and Activity Monitor calls Memory) includes them. Read via
`proc_pid_rusage(pid, RUSAGE_INFO_V4, ...)` from libproc (libSystem, no
link flag).

Measured: a fork child's `ri_lifetime_max_phys_footprint` starts at zero
(it does not inherit the parent's peak, and copy-on-write pages do not
count); an exited but unreaped child is still readable; footprint equals
`top -stats mem` exactly. `ri_interval_max_phys_footprint` would give a
resettable peak, but the reset call is private SPI.

examen sees system headers through silva's model, so `rusage_info_v4`,
`RUSAGE_INFO_V4`, `rusage_info_t` and `proc_pid_rusage` were added to
`silva/fontes/systema_posix.h` (sys/resource.h section + a new libproc.h
section), and `officina/auspex_posix.sh` asserts the layout against the
real SDK (296 octets; resident 64, footprint 72, lifetime max 240).

Gotcha while writing the test: `M` is the Roman numeral 1000, so
`M * MXXIV` is 1,024,000, not a MiB - use `MXXIV * MXXIV`.

## 2026-10-20 - Linux body (from quarta, ianua T3)

After main 7f1c85c5 was merged into quarta, EVERY Linux test link failed
(`undefined reference to memoria_processus_legere` from credo.c):
credo T2+ calls this library, but it had only the macOS body ("Linux can
follow later", credo-memoria-spec II.1). Found by ianua T3's Linux run;
`cives` (green on Linux before the merge) failed identically.

`lib/memoria_processus_posix.c` reads `/proc/<pid>/status`:
footprint = VmRSS + VmSwap (closest to ri_phys_footprint: pages held,
resident or swapped), peak = VmHWM (RSS peak; Linux keeps no
swap-inclusive peak), resident = VmRSS; kB -> bytes. No /proc (BSD) or a
zombie child without Vm* lines -> FALSUM, which the header allows and
credo already tolerates (memory recorded as 0).

PITFALL: /proc files report st_size 0, so `filum_legere_totum` would
return an EMPTY chorda with no error. Read with `filum_octeti_*` to EOF.
Under FABRICA_LECTIONES the read would be journaled; fabrica does not
run inside the Linux containers.

Also fixed in probatio_memoria_processus.c: `(vacuum)read(...)` fails
glibc builds (`warn_unused_result`; gcc ignores the void cast) - the
suite had never been built on Linux.

Oracle = the existing platform-neutral suite, unchanged: 20/20 on musl,
glibc and macOS. Hand plant (posix body not compiled on macOS, so no
silva.planta): kB not converted -> 18/20 on musl (the 64 MiB growth
section), restored -> 20/20.
