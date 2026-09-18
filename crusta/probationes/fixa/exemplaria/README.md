# domus.cruda.tsv, domus.plena.tsv — the frozen second opinion

**These are a SNAPSHOT of the Python extractor, not a specification.**
Written 2026-09-18, the day before that extractor was deleted.

## What they are

For the 234 tracked `.sh` files outside `probationes/fixa/`, run
against `crusta/lintrum/nt-aequalitas.stml`, sorted (`LC_ALL=C`):

| file | rows | what it pins |
|---|---|---|
| `domus.cruda.tsv` | 570 | both arms, before the declared subtraction — the EXTRACTOR |
| `domus.plena.tsv` | 14 | after subtraction — the SUBTRACTION |

Per arm, in `cruda`: `nt-aequalitas` 292, `nt-negata` 278. Those are
rule 1's census numbers; their history is in the git log for
`pythonica/probatio_silva.py`, which pinned them until this arc.

## Why they exist

Until 2026-09-18 the extractor existed TWICE: in C
(`materia_exemplaria`) and in Python (`stml vertere` →
`html.parser` → `_relata`). `crusta/instrumenta/differentia_exemplariorum.sh`
ran both over this corpus and diffed them — two implementations
sharing nothing, so they agreed only if both were right. That gate is
what earned the C extractor its trust: 234 files, zero divergences,
and planted faults proved it could go red (restoring descent into
captured subtrees moved 1136 rows and named the files).

Then the Python half was deleted (Fran, 2026-09-18). Re-deriving both
sides would have meant running the C against itself — a diff that
cannot fail, which is worse than no gate because it still prints "ok".

So the last thing the independent implementation ever produced was
frozen here. The gate now asks: **does the C still say what the two of
them agreed on?**

## The circularity that is NOT here, and the one that would be

These bytes came from Python. Regenerating them comes from C — so a
careless regeneration DOES make the gate circular, silently. Hence:

```
EXEMPLARIA_SCRIBERE=1 ./crusta/instrumenta/differentia_exemplariorum.sh
```

refuses without a cause, and the house rule applies: **goldens are
never regenerated without an audit.** The diff names every row that
moved; read it before accepting it.

A legitimate regeneration looks like the corpus growing. It happened
once already during this arc: 568 → 570, because `crusta/facies.sh`
was added carrying `! [ "$BIN" -nt "$SRC" ]` — a *correctly negated*
`-nt`, so both arms grew by one and `plena` stayed at 14. That is the
shape a healthy move has. A move where `plena` changes, or where the
arms move by different amounts, is a finding until proven otherwise.
