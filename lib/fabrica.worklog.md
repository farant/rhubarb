# lib/fabrica.worklog.md

## 2026-09-29 — T1: the pure core (plan 1a)

`include/fabrica.h` + `lib/fabrica.c`: input kinds, input-set digest,
aedilis manifest reading, verdicts, dependency order. Disk and processes
only through `FabricaSutura` (the `AedilisExtractor` pattern); the gate
`probationes/probatio_fabrica.c` runs on an in-memory seam (55 checks).

Decisions made while building:
- **Scratch contract.** `currere` must hand the generator an EMPTY
  scratch dir. Without that a mute generator (exit 0, writes nothing)
  would be compared against the previous run's leftover output and
  judged RECENS — Review Focus 4. The fake seam in the gate empties it,
  and the "mute" case runs right after a writing run to prove it.
- **Digest = path + content.** Per input, in path order: `via NUL
  sha256(bytes)`. The gate's first "contents swapped between two paths"
  assert did NOT pin the path: with path bytes planted out, swapping
  still reorders the content hashes, so it stayed green. The rename
  assert (same content, different path) is the one that went red under
  the plant. Lesson for later gates: ask what an assert would still
  accept if the property were gone.
- **Manifest file never digested whole** (`generatum=` timestamp,
  `commissum=` change on every run); its listed files are. `systemata`
  excluded. A manifest carrying `inresolutae` (P1, 982fec44) makes the
  digest FALSUM with the header named — never "current".
- **Directory inputs** digest their sorted name list, keyed `via/` so a
  directory never collides with a file of the same path. A new header
  in an include root must change every closure digest over that root.
- **Blind-link rule held:** `nm -u build/fabrica.o` = house lib symbols +
  libc only; `compile_tools.sh` (links every `build/*.o`) still links.
- Unsigned trap caught by examen: `index < 0` on an `i32` (always
  false) for `chorda_invenire_index`'s `-1` — now an `s32`.
- Lint renames: INSTALLATIO → INSTITUTIO (house says "institutum:
  ~/.bin/stml"), `FabricaGenusProvenientiae` → `FabricaProvenientia`.
  The plan/spec prose still says "installatio" for the kind.
