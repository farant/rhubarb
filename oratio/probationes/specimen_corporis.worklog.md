# specimen_corporis.h worklog

## 2026-10-22 — born: hash selection + 128 KB head cap (parks …C87H, …E2Y7J)

**Problem.** The corpus sections of probatio_oratio_stml (gravis, 1 GiB ceiling) and probatio_oratio_canon (default 256 MiB) sampled every 5th line of `oratio/build/corpus_md.txt`. Each uses a fresh arena per file, so a section's peak is roughly its single largest sampled file. Adding `oratio/fontes/oratio_partes.worklog.md` shifted every later line by one and pulled `silva/phase-log.md` (788 KB) into the sample: stml went from ~1 GiB to ~3.3–4 GiB and canon to ~560–885 MiB, both red. The oratio gate is fusio tier, so no commit saw it.

**Measured (quinta 770310fe):** swapping only phase-log's line for README.md (stride kept) made both green: stml ~1,010 MiB peak, canon 146 MiB. Single-file runs (target on line 5 of a 5-line list):

| file | size | stml peak RSS | canon apex |
|---|---|---|---|
| briar-plan-1-parsator.md | 172 KB | 804 MiB | 198 MiB |
| oratio-spec.md | 209 KB | 1,003 MiB | 246 MiB |
| phase-log.md | 788 KB | 2,636 MiB | 809 MiB |

So the stml round trip costs about 3.5–5 KB of peak per source byte and canon about 1.1 KB per byte. oratio-spec.md alone already sat at the stml ceiling.

**Fix (Fran's choice of options 1 + 2):**
- Selection by FNV-1a of the path, mod 5 (290 of 1,553 files today), so adding a file no longer reshuffles the sample. `ORATIO_CORPUS_TOTUS=1` still runs everything.
- Files over 128 KB are tested by their head, cut after the last blank line before the cap (else after the last `\n`, else at a UTF-8 code-point start). Big files still contribute content; the peak is bounded. 5 of 290 sampled files are capped today, 21 of 1,553 overall.

**After:** sample: canon 128 MiB, stml ~650 MiB RSS. Full corpus (phase-log included): canon 168 MiB, stml ~730 MiB. 0 failures. Canon needs no `gravis`. Plant (cap ×1000, full corpus): stml red at 3,529 MiB; reverted, green.

**Not exercised by the corpus:** the no-blank-line and no-newline fallbacks. Every capped file today has a blank line before 128 KB.

**Still open:** the per-byte cost itself (projection weight, park 01M5EGAWD5NT504BX9ZCR6JC37). The round trip holds two materia trees, the STML text (~810 B per source byte) and the reread STML tree.

## 2026-10-22 — per-document interner in the canon corpus loops (md, oratio)

The first fusio_probare after merging main went red on one gate: `probatio_md_canon`'s corpus section at 258.4 MiB against 256. That section judges ALL md files, not a sample, and main brought the corpus from 1,553 to 1,572 files. Cause: each document's STML was read with the FIXTURE's interner, so every distinct string of every document accumulated in one suite-wide table that is never freed. The per-document arena was destroyed, but its interned strings lived on. The fix is `internamentum_creare(p)` in the document's own arena. canon_iudicare compares strings by content (chorda_aequalis); interned pointer identity only matters in macro expansion, which wants the tree's OWN interner, which this is.

md_canon: 258.4 → 102.9 MiB, 1572 files, 0 failures. oratio_canon (same pattern): sample 128 → 90 MiB, full corpus 168 → 89.8 MiB. The peak is now flat in corpus size.
