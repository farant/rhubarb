# circulus worklog (and atomus)

## 2026-10-21 — S3a: atomus + circulus (sexta)

Spec `project-specs/sonus-spec.md` X.7; Fran approved the three S3
headers (atomus, circulus, auditus) the same day. Header slip caught by
the compiler before review: a field named `magnitudo` (latina macro =
`sizeof`) -> `mensura`; `slota` (not Latin) -> `loci`.

**atomus** = two calls, the only house code that touches `__atomic`
(`lib/atomus.c`, which includes `postulata_atomica.h` FIRST so silva
evokes the compiler surface - silva commit 96df46f3).

**circulus** = SPSC ring of fixed-size places in caller memory. Indices
are free-running unsigned counters (C defines the wrap); capacity a
power of two, so place = index & (capacity - 1) and count = head -
tail survive the 2^32 wrap (the suite starts both at 0xFFFFFFF0 and
0xFFFFFFFC). Each side reads ITS OWN index plainly (only it writes it)
and the OTHER's with `atomus_legere`; it publishes its own index with
`atomus_publicare` only AFTER the copy.

**Stress without threads.** The house has no threads (processus.h), so
the two-sided test runs writer and reader in two PROCESSES over
`mmap(MAP_SHARED | MAP_ANON)` + `fork` - real concurrency on separate
cores, no pthread anywhere. 200 000 records of four equal words
{k,k,k,k} through a 64-place ring: order (none skipped/repeated) and
integrity (no torn record). Empty/full spins are capped
(CIRCULUS_GYRI = 2^28) so a broken ring FAILS the suite, never hangs
it (the stub run ended in ~0.5 s through the caps). `MAP_SHARED` was
missing from silva's surface: added (Darwin 0x0001), auspex asserts it,
hand plant 0x0003 -> DISSENTIT, restored.

**Plants 2026-10-21 (silva.planta, 6/6 red, sources byte-identical):**
publish before copy (stress: integra), full one late, wrong mask, tail
not advanced, power-of-two unchecked - and the PROBE "atomus nudus":
`__atomic_store_n(..., RELEASE)` replaced by a plain volatile store.
It went RED in the two-process test: on this Apple Silicon machine the
reader saw stale/torn records without the release. This is spec X.7's
argument ("volatile is not enough; ARM reorders") measured on the
house's own hardware, not just reasoned. Caveat: the stress test is
probabilistic - one red run shows the hazard exists; a green run never
proves its absence.
