# scaena_repositorium worklog

## 2026-11-13 - first build (graphis-plan T7)

- Copied from `lib/statuae_repositorium.c` and kept apart from it on
  purpose: own act kinds (`scaena-addita`, `scaena-remota`), no
  `genus` field, so neither list can see the other's entries and a
  title may exist in both. The test section `separata` pins this; a
  plant that renames the act kind to the models' turns it red.
- Titles are stricter than model titles (a-z, 0-9, `-`, `_`), the
  same rule `scaena.h` applies to a view's title, plus "not
  `mundus`". Acts with a title that fails the rule are skipped when
  the index is read.
- `scaena_removere` returns FALSUM when the title is absent, unlike
  `statuae_removere`. `bin/scaena removere` uses that to tell the
  user the view was not there.
- The store never reads the blob. `bin/scaena` checks it with
  `scaena_legere` before `scaena_addere`.
- `tools/scaena.c` tells a refusal in a picture from one in a piece
  by remembering the last picture the builder asked for:
  `ScaenaFructus.plagula` equal to that name means the picture,
  anything else a piece. A piece named like its picture is reported
  under the picture's path.
- `condere` opens the volume only after every file has been built,
  so a refused run cannot create or touch it. The smoke script
  checks this with a checksum of the volume file.
