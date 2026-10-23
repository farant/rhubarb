# corpus_infixum.sh worklog

## 2026-10-22 — corpus_silicis: 37.1 → 11.5 s, outputs byte-identical

`corpus_silicis` (memorabilis; inputs = all of lib/, include/, materia, toml, vendor, canons) misses on every library change, and was 26-28 s of slow generata runs in other trees. Traced one fabrica-style run (FABRICA_SCRIPTURA scratch dir, the way the judge runs it): 37.1 s.

| phase | before | what |
|---|---|---|
| silva/nexus.sh -renovare | 2.0 s | incremental index (real work) |
| awk over build/nexus.tsv + sort | 11.3 s (13.9 s isolated) | 166 MB, 2.4M lines; only 443k (18%) are `sedes` |
| corpus_sigillum | 18.3 s | `shasum` (a Perl script) + `cut` forked PER FILE, ~1000+ files |
| capsula_generare | 4.8 s | compression (real work) |

Changes:
- `LC_ALL=C grep -F "<TAB>sedes<TAB>"` before the awk. Both awk rules require `$2=="sedes"`, so the output cannot change; isolated 13.9 → 3.3 s.
- One `shasum -a 256` over all files (`xargs -0` keeps order), with lines re-formed `path<TAB>hash` by awk, so the outer fingerprint is identical. Corpus paths contain no spaces or backslashes (shasum would escape those).

Proof: a reference run (old script) and a new run into separate scratch dirs. corpus.versio (fingerprint e9f7a1f4...), corpus.symbola.tsv (17982 lines), corpus_silicis.toml and build/capsula_corpus_silicis.{c,h} (66 MB) are all byte-identical. `bin/fabrica iudicare -plenus -tacta tools/corpus_infixum.sh`: both artifacts "regeneratio congruit".

Remaining: capsula_generare compression ~4.8 s, nexus refresh ~2 s, the filtered awk ~3.3 s.
