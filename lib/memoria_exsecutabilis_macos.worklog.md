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

## 2026-11-10 — fork: the child can be born in WRITE mode (fixed)

CORRECTION to the entry above: "fork: the child runs the sealed code
fine" is false under load. Ledger question 01M73GJW40KQ0939W8TQBM9BVV
(the 'furca' section reddened a cold clone at merge time; 20 of 200
red on a busy machine, 0 of 500 quiet).

Evidence (scratch reproducer, handler printing the fault): the child
dies with SIGBUS, ESR 0x8200000f = instruction abort, permission
fault, pc = first byte of the MAP_JIT page. Retrying in the child
never heals (3 of 3). So the page is mapped but not executable for
that thread: the child's per-thread JIT switch is in write mode.

Cause: fork gives the child the parent thread's JIT switch as it was
SAVED at the parent's last context switch, not its live value. If the
parent was switched out between aperire and sigillare and forks before
being switched again, the child starts in write mode. Proof by
forcing it, quiet machine, one process:
- 1 ms sleep inside the write window, seal, fork: 434/500, 440/500 red
- same + 1 ms sleep after the seal: 0/300
- same, child calls pthread_jit_write_protect_np(1) first: 0/300
- one region reused, no recent write, 16 copies: 0/48000
- fresh region per fork, 8 copies at once: 44/24000; with an atfork
  child handler: 0/24000
"Under load" was only ever "preemption inside the write window is
likelier". Not specific to this library - a 40-line standalone
program shows it.

Fix: the library keeps the LOGICAL state (_scriptura_aperta, set by
aperire, cleared by sigillare) and registers a pthread_atfork child
handler once, at the first reservation; the handler re-applies the
logical state in the child. So a child inherits what the parent
meant: sealed stays executable, open stays writable. One static is
enough because the house has no threads (the switch is per thread; a
threaded client would need per-thread state). A failed pthread_atfork
refuses the reservation with a causa.

Tests: "furca post scripturam interruptam" (sleep inside the window,
seal, fork, child calls; 20 rounds - ~85% red per round without the
fix) and "furca scriptura aperta" (parent opens and forks; the child
writes, seals and calls; the parent then finishes its own write).
Plants, each red then green: handler not registered (interrupta, l.288);
handler ignores the state (aperta: child killed writing); sigillare
leaves the state open (plain 'furca': child killed calling). Real
test binary under load after the fix: 0 of 2800 (8 x 150 and 16 x 100
copies); before: 18 of 480.

For officina M5: the cursor forks per suite and spec F18 requires
surviving fork - that holds now, through this handler. Raw fork users
that bypass the library's state (none today) would not be covered.
