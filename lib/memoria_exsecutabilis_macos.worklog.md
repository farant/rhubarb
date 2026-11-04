# memoria_exsecutabilis_macos worklog

## 2026-11-04 — born (officina M5 T4)

The only house file with MAP_JIT, pthread_jit_write_protect_np and
sys_icache_invalidate (spec F18). One reservation at a requested
address (mmap hint, no MAP_FIXED); if the kernel puts it elsewhere the
region is unmapped and the call refuses with a causa - officina's
native code relies on deterministic addresses (bl between functions,
F19). ZEPHYRUM = any address (tests). Size must be a whole number of
16 KiB pages (refused, not rounded).

W^X on Apple silicon: the MAP_JIT region is RWX in the page tables;
writability is a per-THREAD switch (pthread_jit_write_protect_np 0 =
write, 1 = execute). sigillare flips to execute AND invalidates the
icache for the range in one call, so the invalidation cannot be
forgotten. A range outside the region returns FALSUM (write is still
closed).

Measured (M-series, macOS 27): with the invalidation removed, the
"rescribere" section's 200,000-call loop after rewriting movz #42 ->
movz #7 at the same address saw stale results - the CPU really does
run the old instruction without it (spec fact 16 confirmed here).
fork: the child runs the sealed code fine (MAP_PRIVATE copy-on-write).

examen needed systema_posix.h entries for PROT_EXEC/MAP_JIT and the
two functions (see silva/fontes/systema_posix.worklog.md).
