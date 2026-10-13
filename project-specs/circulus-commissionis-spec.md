# circulus commissionis - commit-loop polish (spec)

Portfolio item (2), Fran 2026-10-12. Five small fixes, each from a pain
MEASURED in the portae-declaratae slice (T3-T6 commits) or ledger-bitten
before. Goal: fewer wasted commit cycles and no cold waits that aren't
caused by real changes. No new concepts; every fix lives where the pain is.

## I. Items

### P1 - closure cache keyed by the EXTRACTOR, not the whole binary

**Pain.** The aedilis/silva extraction cache (thesaurus
`build/aedilis/obiecta`) is keyed by `sigillum(binary ‖ aedilis.stml) ‖
source bytes`, with *binary* = the whole executable running it
(tools/fabrica_ansae.c `_gradus_parare`, tools/aedilis.c). So ANY rebuild of
bin/fabrica makes every closure cold: that is roughly 30 s for owed gates
(measured T3) and for gradus suites. bin/fabrica now links crusta, materia,
portae and silva, so this happens on most library edits. bin/aedilis and
bin/fabrica also run the SAME extractor but never share their caches.

**Fix.** Key = `sigillum(extractor digest ‖ aedilis.stml)`, where the
extractor digest is the digest of the sources that DETERMINE extraction
(the closure of lib/aedilis_silva.c: lib/aedilis*.c, silva amalgama, the
latina table). It is computed at build time into a provenance-style file
in build/ (as tools/provenientia_scribere.sh already does for binaries),
and linked into both bin/aedilis and bin/fabrica. Consequences:
- an edit outside the extractor keeps the cache warm;
- a real extractor change still invalidates it;
- aedilis and fabrica warm each other.

**Design question for Fran:** compute the digest (a) through a fabrica
action `extractor_aedilis` (`bin/fabrica digestum` over its inputs - one
source of truth, declared), or (b) directly in the build script over
`bin/aedilis --partes lib/aedilis_silva.c`. **Fran 2026-10-12: (a)**: it is
declared, judged and tested like every other digest, and stays correct
when the extractor's closure changes.

### P2 - `vocabula.sh -nova` refreshes the identifier index first

**Pain.** Ledger wish 01M46ER35 (ictus; bit 09-14, 10-05, and again in
portae T3). `-nova` reads `build/nexus.tsv` as it stands, so right after an
edit it reports `NOVA 0` falsely. commissio's pre-gate lint already heals
the index, so the remaining cost is every MANUAL check before a commit.
**Fix.** `-nova` runs the incremental nexus sweep (as `./silva/nexus.sh`
does) before judging.

### P3 - commissio heals a stale JUDGE instead of refusing

**Pain.** T4 attempt 1: the fabrica pre-check refused with "iudex fabrica
non recens - sana fabrica prius". The fix was always the same tree-local
command (`./tools/fabrica_struere.sh`), and the refusal cost a manual
round trip each time.
**Fix.** When the ONLY refusal is a stale judge (bin/fabrica,
bin/compilator or bin/aedilis - tree-local builds, never installs),
commissio runs that judge's struere script, prints `iudex sanatus: X`,
and judges again ONCE. Any other refusal, or a failed rebuild, refuses as
today.

### P4 - briar-fumus tests a TREE-LOCAL briar in a worktree

**Pain.** T4 attempt 3: in a worktree, the gate runs `~/.bin/briar`, which
is installed from MAIN. When main changes an API (insula C2 aa623c1b), the
worktree's fixtures and main's binary disagree, and the gate is red for
reasons that have nothing to do with the commit. It was excused by hand.
**Fix.** In a worktree (`git rev-parse --git-dir` != `--git-common-dir`)
the fumus uses the tree's own `bin/briar` (linked with
`FABRICA_AGIT=1 ./tools/briar_struere.sh`, no install) and puts it first
on PATH for the `#!/usr/bin/env briar` stages. In main it keeps testing
the INSTALLED binary, which is its purpose: "corpus vetus VIRIDE MENTITUR".
If the tree-local binary is missing, the gate exits 2 and names the
recipe.

### P5 - excuse ONE owed gate, not all of them

**Pain.** T4: excusing briar-fumus meant `sine_debitis`, which also stops
commissio from ADDING owed gates, so I had to recompute and hand-list the
other 43 (twice, after the haruspex fix).
**Fix.** `commissio(..., excusatae={'briar-fumus': '<causa>'})`. The named
owed gates are not added (`porta debita EXCUSATA: X - causa` is printed);
every other owed gate is still added and run. An empty cause, or a name
that is not owed, is refused before anything runs (no silent
over-excusing). `commissio_umbra` gets the same.

## II. Not in this slice

The cold-tree self-heal (undeclared house tools, portfolio item 4);
deriving the `-I` roots (item 1); auto-building missing tools such as
bin/mensor (that is item 4's territory).

## III. Done means

Each item has a test that failed first, plus a plant; P1 is measured
before and after (rebuild bin/fabrica, then time owed gates: cold before,
warm after); P4's gate passes in quinta with no hand excuse; ledger
01M46ER35 is closed.

## IV. As built - P1 note (2026-10-12)

Fran chose (a), a declared action. Implemented with the EXISTING action
`aedilis`, not a new `extractor_aedilis`:
- its digest already covers the extractor;
- bin/aedilis already embeds it as its own provenance;
- a new generator writing a provenance file (which carries the HEAD
  commit) would have been judged stale after every commit.

The digest is embedded at BUILD time, because a runtime digest of the
sources could describe code the binary does not contain yet. Details are
in tools/aedilis.worklog.md.

## V. As built (2026-10-12; T1 3d0c62bc, T2 d86738cf, T3 9e092185, T4 3ba31498, T5 707934e5)

- **P5 `excusatae={porta: causa}`** (commissio, commissio_umbra): excuses
  ONE owed gate, and the others are still added and run. It is refused
  before anything runs on an empty cause, an unknown gate, a gate that is
  also requested, or a combination with sine_debitis. Excusing a gate
  that is not owed is refused too. On the way: the `exspectare` test
  wrote its receipt in place, which flaked; it is now atomic.
- **P3 a stale judge heals itself:** the `IUDICES_STRUERE` table maps
  fabrica, compilator and aedilis to their own struere.sh (tree-local,
  never the printed recipe). Commissio prints `iudex sanatus: X` and
  judges ONCE more. A live check (bin/fabrica made stale with a comment)
  passed.
- **P2 `vocabula.sh` refreshes the identifier index** before every mode
  that reads identifiers (~1 s; a failed refresh exits 2). The hook
  passes VOCABULA_INDEX_RENOVATUS=1, so there is one sweep, and unci
  fumus XXI guards it. Ledger 01M46ER35 is fulfilled.
- **P4 briar-fumus in a worktree tests the TREE's bin/briar and
  bin/briar-spectator** (judged RECENS; recipe `bin/fabrica sanare
  bin/briar bin/briar-spectator`). A link directory goes first on PATH
  and the real ~/.bin is removed from it. New stage 0 asserts the binary
  under test. Main still tests the installed binary.
- **P1 the cache key is the extractor's embedded digest** (see §IV):
  41 s cold → 4.8 s after an unrelated rebuild, and aedilis and fabrica
  share the cache.
- **Plants per item, every one red then green:** P5 II, P3 II, P2 I,
  P4 II, P1 III.
- **Departures:** P1 uses the existing `aedilis` action (§IV), and its
  helper lives in lib/aedilis.c, not lib/aedilis_silva.c.
