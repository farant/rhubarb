# fiducia_diffisa.worklog.md (the house blocklist: lib/fiducia_diffisa.c + the generated table)

## 2026-10-22 - house blocklist born (fiducia-linux T2)

- **Snapshot pinned** at probationes/fixa/fiducia/onecrl/2026-10-21/
  (changeset 826 KB, timestamp 1792622041776, sha256 1c26d8ad...;
  re-fetched for the pin and byte-identical to the planning copy):
  1907 records, 645 `enabled:false` skipped, 4 duplicates folded ->
  1258 entries; the shared octet pool is 44.6 KB (issuer DNs repeat
  a lot), the generated .c 467 KB, generation 0.24 s.
- **Entry kinds** are Mozilla's two: issuer+serial and subject+key
  (subject DER AND SHA-256 of the SPKI). A cross-sign keeps subject and
  key, so subject+key catches it.
- **The `geminus` fixture was wrong in T1**: it had CN=geminus, so it
  did not model a cross-sign at all (a subject+key entry, correctly,
  did not match it). Now it reuses CN=localhost and localhost.key with
  a new serial - same subject, same key - and the test asserts the
  localhost entry refuses it. T1's assertions (same key, other serial,
  same issuer) still hold.
- **Real certificates as fixtures** (probationes/fixa/fiducia/emudhra/,
  NOTICE with crt.sh ids and fingerprints): "EM DV TLS CA - G3B" under
  emSign and the Sectigo cross-sign under AAA Certificate Services.
  Without them nothing could show that `fiducia_iudicare` consults the
  BUILT-IN table (no test can mint a blocked certificate); with them,
  `probatio_fiducia` "diffisa vera" goes through fiducia_iudicare on
  both platforms and must name "clavis diffisa ... 2083382". They also
  prove fiducia_der on real-world certificates.
- Both platform files call `fiducia_diffisas_iudicare` FIRST. macOS:
  before SecTrust; Linux: before the (still refusing) placeholder, so
  "diffisa vera" is already green there while the other 6 Linux checks
  wait for T3.
- **Generator rules** (`tools/fiducia_diffisa_generare.sh -fumus`, 8
  cases: bad genus, bad base64, missing attribute, short key hash, bad
  date, SHA256SUMS mismatch, undated directory, a good two-entry list
  sorted) are run BY HAND, like pecunia_monetae - plan ruling. The
  generata gate covers table == generator output.
- Plants: serial compared without its leading 0x00 -> emittens_series
  red (silva.planta); generator not skipping disabled records -> 1903
  entries -> tabula_vera's `<= 1262` red (by hand: edit, regenerate,
  test; restored table byte-identical).
- **Layout fixed before the first commit.** The check first lived in
  `lib/fiducia.c` with `fiducia_diffisa.h` naming two corpora (as
  tabula_nodorum.h does). The radix gate refused the commit:
  `probatio_silex` "novum -vitrea" - silex maps X.h to `lib/X.c`
  before `X_macos.m`, so the new `lib/fiducia.c` hid fiducia_macos.m
  from silex-made projects. Now one implementation per header:
  `fiducia_diffisa.h` -> `lib/fiducia_diffisa.c`,
  `fiducia_diffisa_tabula.h` -> `lib/fiducia_diffisa_tabula.c`
  (GENERATUM). 99 stale objects of the old names deleted from build/
  (`mv` keeps the older mtime, so builders would have kept table
  objects under the function file's name).
- **Second refusal, not ours but in our way:** with new lib/*.c files
  the silex capsule (`corpus_silicis`, inputs = every lib/*.c) went
  stale; commissio's heal ran `vates_responsum` first (its output is a
  lib/*.c input of the capsule), and that generator's
  `norma_struere.sh` builds bin/norma + build/aedilis/norma without
  declaring them -> "scripsit extra vestigium" -> FRACTUM. Declared both
  (plus `praecondicio aedilis`) on `vates_responsum` and `normae_fixae`,
  as pecunia_monetae does; then `bin/fabrica sanare
  build/capsula_corpus_silicis.c` healed in 22 s.
