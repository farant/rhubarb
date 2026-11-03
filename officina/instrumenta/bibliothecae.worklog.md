# bibliothecae.c worklog

## 2026-11-02 — T2 (officina-bibliothecae): born

Resolver: `#include` lines -> radix-relative `.c` paths for the
interpreter, through aedilis (spec project-specs/officina-bibliothecae-spec.md).

**A header scope misses its own implementation.** `bin/aedilis
include/filum.h --enumerare` returns chorda, piscina,
chorda_aedificator - but NOT filum.c or lectiones.c: the scope is
walked, it is not itself an object. So every resolution goes through
a throwaway `.c` scope holding just the include lines
(`build/bibliothecae_scopi/scopus_<pid>_<n>.c`), deleted on every
return path. `bibliothecae_ex_via` scopes a real `.c` and puts it
first itself.

**Scope names are unique per PROCESS, not per resolver.** First cut
kept the counter in the struct; the test then built a resolver per
section (credo forbids cross-section state), every resolver started
at 0, and a later successful call overwrote + deleted an earlier
refusal's leftover - the "scope left behind" plant went GREEN (mute
gate). Counter is now a file static. AedilisSilva also caches parse
results by path within one extractor, which is the other reason a
path is never reused.

**Mac-only detection must look at ORIGO, not just caput.** First rule
was "a `.m` in the default-variant derivation whose caput has no
object in the posix derivation". fenestra.h defeated it: it carries
`<aedilis corpus="lib/fenestra_textus.c"/>`, so under posix the caput
DOES have an object (origo CORPUS = 4) even though its real
implementation, fenestra_macos.m (origo DERIVATUM = 0), has no posix
counterpart. Rule now: refuse when posix has no DERIVATUM object for
that caput (`_fons_conventionis`). A `_macos.c` in the detection set
is never a refusal - only `.m`.

**Symbol index is 193 MB and unsorted** (build/nexus.tsv, 2026-11-02),
so `bibliothecae_sedem_quaerere` streams it in 64 KiB chunks
(filum_octeti_*) and copies only matching rows - a line reader would
have allocated every line into the pool. Rows carry both declaration
(include/x.h) and definition (lib/x.c) sedes; one header wins
(variant bodies like fenestra_tempus_posix/_macos.c sit under one
header), else exactly one body, else AMBIGUA.

Plants (all red, IDEM): detection skipped, scope kept on refusal,
load variant NIHIL, size check x100, header preference flipped,
origo check dropped.
