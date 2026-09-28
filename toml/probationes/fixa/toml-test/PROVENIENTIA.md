# toml-test — provenance

Frozen input for the toml client's oracle (plan `project-specs/toml-arbor-plan.md`, Q1).
**Never edited.** A new upstream tag is a new commit with a cause.

| | |
|---|---|
| repository | https://github.com/toml-lang/toml-test |
| tag | v2.2.0 (published 2026-04-30T16:19:17Z) |
| commit | ce08da1ddb075d1c7596d663c7fcba9a2ae02c5c |
| fetched | 2026-09-28, tarball `…/tarball/v2.2.0`, sha256 `dd7beaca6914f2e158ea2b281877665a5d2dbc2b3295f3198d6bb335006d0847` (98,717 bytes) |
| licence | MIT (`LICENSE`, copyright 2018 TOML authors) |
| kept | `tests/` (1,018 files, `tests/.gitattributes` kept: `*.toml -text` preserves the 9 CR-carrying cases) and `LICENSE`; the Go runner and specs dropped |
| asked | Fran, 2026-09-28, before the fetch |

## The 1.0.0 list (`tests/files-toml-1.0.0`)

884 entries: **205 valid** (`.toml` + expected `.json`) and **474 invalid**;
every listed file present. `tests/files-toml-1.1.0` is kept for the named
future (spec §IX), unused.

Tagged-JSON contract (verified by reading cases in Q1 Step 1): a table is a
JSON object, an array a JSON array, a scalar `{"type": T, "value": V}` with
`T` in `string integer float bool datetime datetime-local date-local
time-local` and `V` a string.

## Oracle agreement (`tomllib` vs toml-test, measured in Q1 Step 4)

Python 3.12.6 `tomllib` over the whole 1.0.0 list, compared BY VALUE
(floats as numbers, `nan` = `nan`, datetimes parsed; the expected strings
are normalized — `1e06` is `"1e+06"`, `+1.0` is `"1"`):
**valid 205/205 equal, invalid 474/474 rejected — total agreement.**
Either oracle is therefore a credible judge on 1.0.0; a disagreement
found later is ours to explain.

Verdicts measured for the plan's Review Focus (toml-test 1.0.0 has no
case for either):
- CRLF document: accepted; a multiline basic string's CRLF decodes as
  `\n`; a line-ending backslash consumes the CRLF and following
  whitespace. A lone CR is rejected.
- UTF-8 BOM at offset 0: `tomllib` REJECTS ("Invalid statement", line
  1 col 1). The client follows (named diagnostic, byte law kept) so the
  differential stays clean; relaxing it later is one rule.
