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
