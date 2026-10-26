# tools/briar_spectator_struere.sh - worklog

## 2026-10-26 — link the root objects from the closure, not build/*.o (park …QBDJVE1)

The script linked briar/build/*.o plus EVERY build/*.o that briar/build lacked. A renamed or
deleted lib leaves its object in build/ forever: after quarta renamed lib/testa_android_initus.c
-> lib/testa_initus.c, build/testa_android_initus.o and build/testa_initus.o both linked ->
'duplicate symbol _testa_geometriam_computare' and fusio_probare refused before any gate.

Now the extra objects come from `bin/aedilis tools/briar_spectator.c --partes`, as
tools/stml_struere.sh already did: O (house) and V (vendored: sqlite3, bearssl_amalgama) parts ->
build/<base>.o, skipping what briar/build provides (also under the alias amalgama_<base>.o -
the briar suite compiles silva/amalgama/silva.c as amalgama_silva.o) and generated sources under
build/ (the clang line compiles them itself). A part with no object = loud error. 101 closure parts
instead of whatever sat in build/ (422 objects); binary 31.2 MB -> 15.6 MB.
Proof: with build/testa_android_initus.o planted as a copy of testa_initus.o, the old script fails
with the duplicate symbols, the new one links; tools/briar_fumus.sh sanum (XII spectator -parare).
