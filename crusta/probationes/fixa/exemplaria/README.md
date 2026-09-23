# domus.cruda.tsv, domus.plena.tsv — extractor regression snapshot over a frozen corpus

**A snapshot of what the tier-2 extractor (`materia_exemplaria`) reports
for rule 1 over a FIXED corpus.** Checked by
`crusta/instrumenta/differentia_exemplariorum.sh` on every full crusta run.

## What they are

The corpus is the 234 tracked `.sh` files outside `probationes/fixa/`
**as they stood at commit `ad73f539`** (`CORPUS_COMMISSIO` in the
gate). The gate extracts them with `git archive` into a fresh temp
directory — read-only against the repository: working tree, index,
HEAD and stash are never touched — and runs rule 1
(`crusta/lintrum/nt-aequalitas.stml`) over them, sorted (`LC_ALL=C`):

| file | rows | what it pins |
|---|---|---|
| `domus.cruda.tsv` | 570 | both arms, before the declared subtraction — the EXTRACTOR |
| `domus.plena.tsv` | 14 | after subtraction — the SUBTRACTION |

Per arm, in `cruda`: `nt-aequalitas` 292, `nt-negata` 278.

It is a BROAD regression net: restoring descent into captured subtrees
moved every one of the 570 rows (1140 diff lines) and 28 in `plena`
(measured again 2026-09-23). Small fixtures would not see that.

## Why the corpus is frozen (2026-09-23)

Until then the golden was compared against the LIVE tree. A snapshot of
a moving input: every line added to any house script shifted rows, the
gate went red three times in one day with no finding changed, and the
fix each time was "regenerate and eyeball it" — a ritual that trains
people not to look, and the path by which a real regression would one
day be waved through. Worse, each regeneration replaced rows with the
extractor's output about ITSELF (70 of 570 rows in four days).

**A snapshot needs a frozen input; a live corpus needs invariants.**
Findings in new or edited scripts are the lint's own business (commit-
time diagnostics), not this gate's.

## History

The golden was born 2026-09-18 from a SECOND, independent extractor in
Python (`stml vertere` → `html.parser`), which agreed with the C on this
corpus with zero divergences before it was deleted. That cross-check is
how the C earned trust. The C reproduces the Python-born golden BYTE
FOR BYTE over the frozen corpus (measured 2026-09-23), so the
provenance no longer distinguishes anything: these rows are simply the
verified answer of the extractor over a fixed corpus. The files here
are the original Python-born bytes, restored.

## When to regenerate

```
EXEMPLARIA_SCRIBERE=1 ./crusta/instrumenta/differentia_exemplariorum.sh
```

rewrites BOTH files — commit both. Legitimate only when the extractor or
rule 1 changes ON PURPOSE; the printed diff IS that change, read it like
any golden. Live edits can no longer move these rows.

To widen coverage (newer scripts carry constructs worth pinning), move
`CORPUS_COMMISSIO` forward AND regenerate in ONE commit, cause named.
If the commit is unreachable (a shallow clone), the gate exits 2 and
says so — it never silently falls back to the live tree.

Rule 2 (`vexilla-domus`) is checked in the same gate differently: a
COUNT pinned over the frozen corpus (70; stability), and `rule ⊆ grep`
over the LIVE tree (an independent oracle stays meaningful live).
