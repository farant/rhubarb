
## 2026-10-27 - lectiones_notare preserves errno

The hook opens/appends the book when FABRICA_LECTIONES is set and saved
nothing around it, so a caller reading errno right after a raw call
could see the hook's errno instead (officina's machinula hands errno
back to the guest program via _errno_retro; filum reads errno in its
error paths). lectiones_notare is now a thin wrapper: save errno, call
_lectionem_notare (the old body, six return points), restore. Test
IIf: book in a missing directory (the hook's open fails and sets
ENOENT) and a valid book, errno preset to EDOM (no file call sets it)
must survive both; plant (restore dropped) red at the missing-dir
case. Trap found writing the test: a section that unsets the variable
and deletes the book must make one more notare call first, or the
cached descriptor keeps pointing at the deleted inode and the NEXT
section's lines vanish (section III went red deterministically).

## 2026-10-31 — per-process line dedupe (budget B2 I2)

fabrica reads a book as a SET (first occurrence wins), yet a process
kept writing the same line: single-file aedilis books were 1348 lines /
724 unique, and the remaining L/X repeats survived I1. A direct-mapped
table (FNV-1a of the whole line → slot) now holds copies of written
lines. A line already present in its slot skips the write(). A
collision only rewrites; a new line is never dropped (full memcmp, not
just length). The table is freed whenever the descriptor closes or
reopens (book unset or changed), which matches the existing "absent =
CLOSED" rule. After fork the child inherits the table, which is correct
because the parent's lines are already in the shared O_APPEND
descriptor. libc only (calloc/malloc/free); lectiones still has zero
house deps.

`LECTIONES_MEMORIA_LOCI` (power of two, default 16384) is read when
the table is allocated, so a forked test child can force every line
into one slot. New section IIIb: x.c, x.h, x.c, x.c → 3 lines, both
present. Plant: length-only compare → 2 lines, x.h lost, FRACTA.
Section III (two parallel children, untorn O_APPEND lines) now varies
the path per iteration: dedupe would otherwise collapse 1000 identical
lines to 2, while what it really checks, untorn lines, is unchanged.

Measured with I1 (`aedilis --corpus lib --partes`, warm): 6.2 G
instructions, 2.1 s, book 22 k lines (baseline 28.4 G, 5.35 s, 721 k;
I1 alone 11.3 G, 2.8 s, 213 k). stdout and the book's first-occurrence
order are identical. fabrica suites, fabrica-fumus(-vivus),
iudicium-fumus, radix (270 members ran), silva green.
