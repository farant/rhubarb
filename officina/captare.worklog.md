# captare.sh — worklog

## 2026-07-24 — captare was linking with ZERO objects, and exiting 0

Chased from the other end: cursor's stdout column reported 4 DIFFERUNT.
All four turned out to be stale goldens rather than machinula divergence
— but the re-pin then only fixed one of them, which is what exposed the
real bug.

**The bug.** `extract_sources` scraped its object list out of another
file by format:

```sh
sed -n "/^declare -a $1=(/,/^)/p" compile_tests.sh
```

`7e1d1de` (*aedilis via media: suite fontes derivati*) moved
`SOURCE_FILES` into the generated `compile_tests_fontes_generata.sh`;
`compile_tests.sh` now sources it at line 34. The scrape has returned
zero lines ever since. `OBJS` was empty and every native link was
attempted with no objects at all.

**Why it stayed invisible for three days.** Three independent covers:

1. The `obiectum deest` guard was *inside* the for-loop. Zero sources →
   zero iterations → the guard never ran. A validity check that lives
   inside the loop it is meant to validate cannot fire on the empty case.
2. `build/captare_bin/` caches binaries, and the relink condition only
   compared the **test source** against the binary, never the objects. So
   67 suites reported `FIXA` from binaries that were never relinked —
   goldens pinned from arbitrarily old binaries and recorded as native
   truth. This is the dangerous one: it produces confident wrong data
   rather than an error.
3. Exit was unconditionally 0, including the run with 19 broken links.

**The repair** (four parts, all in captare.sh):

- Source the generated file instead of scraping it, + a gate refusing an
  empty/absent `SOURCE_FILES`.
- Restore link inputs that had drifted from `compile_tests.sh:321` —
  `-framework WebKit` and `build/sqlite3.o` (scrinium calls sqlite).
- Recompute `NOVISSIMUM_OBIECTUM` once and relink when any object is
  newer than the cached binary.
- Final gate: `nexus fracti` or `fractae` → exit 1. Plus five named
  exclusions (speculum/vitrea suites needing generated capsules captare
  doesn't build; none pass under machinula, none had goldens) so the gate
  *can* be green. **A gate that cannot reach green gets ignored — that is
  precisely how this rotted.**

**Result.** `81 fixae | 7 nondet | 0 fractae | 0 nexus fracti | 5
exclusae`, exit 0. Cursor's column went `64/4/4` → `72/0/0` with the
TABULA line byte-identical (79 praeteritae, 0 ruinae / 93), i.e. the
goldens moved and the interpreter results did not.

**The transferable lesson.** A tool that scrapes another file's *format*
has a silent coupling to that file's shape. When the other file became
generated, the scrape returned empty and the tool reported success.
Scraping without a gate is programmed rot. Where we can source, source;
where we must scrape, a gate must catch the empty result.

Root-cause detail + the three staleness flavors found on the way in:
tabularium nota `01KYADAP`.
