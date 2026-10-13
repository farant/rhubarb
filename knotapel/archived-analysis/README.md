# Archived analysis

Archived 2026-10-13. This folder holds knotapel's documentation from
before the exact audits. It is kept as a record of what was believed and
why. Read it as history, not as the current state of the results.

## What is here

- `atlas/` is the knotapel atlas, built February–March 2026 from the demo
  findings and the explorers' log:
  - `briefing.md` and `PROCESS.md`;
  - `inventory/`: the demo index and one entry per demo, theorems,
    data tables, proofs index, code assets, floating-point audit;
  - `synthesis/`: narrative, key insights, novelty, connections, dead ends;
  - `planning/`, `pitches/`, `research/`.
- `atlas/web/` is the HTML version, generated 2026-02-26 and published to
  GitHub Pages until 2026-10-13. **It has none of the correction notes.**
  The deploy workflow was removed when the atlas was archived.
- `generate_web.ts` is the generator of `atlas/web/` from the markdown,
  written for Node. Its paths are relative to this folder.

Every markdown file starts with a one-line ARCHIVED banner. Nothing below
the banner was changed by archiving.

## Why it was archived

From October 2026 the old demos were re-run on rhubarb's exact house
arithmetic (demos 114–125). Many headline claims did not survive. The
causes recur:
- `long` overflow;
- fixed arrays that silently capped catalogs, directions or winners;
- floating point breaking exact ties arbitrarily;
- results that were properties of one greedy search, not of the catalog;
- mislabelled knots (several early "trefoils" and "torus knots" are
  links);
- numbers that the demo's own code does not reproduce.

Correction notes were added inside 43 of these files wherever an audited
claim was recorded. Claims that were never audited carry no note, so the
absence of a note is not a confirmation. The overall picture changed too:
the atlas's synthesis, novelty and pitch documents tell a story that the
audits no longer support.

## Where current status lives

- **Each audit's findings:** `knotapel/demo_114_exact_audit/` through
  `knotapel/demo_125_exact_crossing_depth/`, each with `findings.md`,
  verdict tables and an independent oracle.
- **The overall reading:** "Synthesis (2026-10-13): What the Exact Audits
  Leave Standing" at the end of `knotapel/explorers-log.md`.
- **Demos 1–112 as built**, with reading notes on errors:
  `docs/knotapel.html`.
- **Planned:** a live claims-status index (one row per major claim:
  survives / falls / unaudited), replacing the atlas as the reference.

## Elsewhere

The css and html parser tests used `atlas/web/style.css` and
`atlas/web/index.html` as real-world inputs. Byte-identical frozen copies
now live at `probationes/fixa/css/knotapel_atlas_2026-02-26.css` and
`probationes/fixa/html/knotapel_atlas_index_2026-02-26.html`.
