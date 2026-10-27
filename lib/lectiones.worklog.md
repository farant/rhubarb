
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
