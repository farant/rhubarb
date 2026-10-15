# gradus portarum - two tiers of gates: commit and merge (spec)

*Born 2026-10-15 in quinta. The lexer commit (benchmark + keyword fix,
6 files) owed ~45 gates and needed five attempts of ~8-30 min each
(load from other worktrees, a timing-flaky fumus, two pre-flight gaps).
Fran: "maybe we need a limit of ~5 minutes of mandatory runs between
commits. If the agent wants to run extra tests themselves that's fine
... and then we have the merge into main touchpoint where we can have
more mandatory stuff." Decree …GDFF4J (Fran): tier by EXPLICIT
annotation; ~5 min mandatory per commit; the rest at the merge. Fran
deferred the merge-tier mechanism and the first cut to Claude (§II,
§III). This deliberately relaxes the 10-12 rule "commissio runs every
owed gate" (portae-declaratae).*

*Status: APPROVED 2026-10-15 (Fran: "yes, that sounds great"); T1-T3
built (§VII).*

## 0. Data (build/portae/tempora.tsv, last 10 healthy runs per gate; the week's 371 commits through `portae tactus`)

- Medians: 30 gates at or under 30 s (most under 10 s); 16 over 30 s,
  the top being examen-corpus 194, pythonica 186, silva 175, venenum
  167, aedilis 159, oratio 137, silva-m3 112.
- How often owed: radix 76% of commits, aedilis 71%, ludus_tessera 39%,
  the converted suites ~15-18% (mostly through `currit tools/fabrica.c`,
  fixed in canalis T8).
- Gate time over the week: 20 h; pythonica 20%, radix 19%, aedilis 11%,
  silva 10%, fabrica-fumus 7%.

## I. INTERFACE - the tier annotation (for Fran's approval)

```sh
# <porta titulus="pythonica" gradus="fusio" tegit="pythonica/*.py"/>
```

- `gradus` = `commissio` (default) or `fusio`; any other value refused
  by name in the C reader, like `genus`. `genus` (pre-check / test)
  and `gradus` (when it is mandatory) are independent.
- `portae declaratae -machina` writes `GRADUS <porta> fusio` for the
  non-default tier; `portae ordo -machina` gains a fifth field
  (`ORDO <porta> <genus> <ms|-> <gradus>`).

## II. Commit tier (silva.commissio)

- Owed gates of tier `commissio` are added and run as today (order,
  pre-checks together, fail-fast).
- Owed gates of tier `fusio` are NOT run: `porta debita DILATA ad
  fusionem: X - causa`, and recorded as branch debt in
  `build/portae/debita_fusionis.tsv` (gate, commit, reason) - visible,
  never silently skipped.
- Gates named explicitly (`portae=[...]`) always run, whatever the tier.
- `commissio(..., plena=True)` runs every owed gate (today's behaviour).
- Before the gates: `aestimatio portarum: N s (commissio), M dilatae` from
  the medians; over ~300 s a MONITUM (not a refusal).
- First cut (§0): tier `commissio` = gates with median <= 30 s + radix
  (its cost is proportional to the change - only stale members run - and
  it is the main safety net, owed by 76%): ~35-40 s expected per commit,
  ~4.7 min worst case. Tier `fusio` = the 15 others: pythonica,
  examen-corpus, silva, venenum, aedilis, oratio, silva-m3,
  oraculum-silvae, fabrica-fumus (its stage L is a wall-clock check that
  flaked under load 2026-10-15), iudicium-fumus, credo,
  nexus-instrumentorum, unci, officina, briar-fumus. silva is
  `fusio` because its re-runs are large when aedilis.stml or the silva
  semantics change; an agent working on silva runs it at will.

## III. Merge tier (one command in the merge ritual)

`silva.fusio_probare()` (and `tools/fusio_probare.sh`), run in the
worktree:
1. refuses unless main is an ancestor of HEAD ("merge main first" - the
   existing ritual step);
2. owed gates over `git diff --name-only <merge-base main>..HEAD` (the
   branch as a whole), BOTH tiers, same order and failure policy as a
   commit; reused passes cost ~0, anything stale re-runs against the
   merged state;
3. then `tools/frigida_probare.sh` (fresh clone);
4. green -> the branch debt file is cleared and the command prints the
   fast-forward recipe; red -> names the gate and, from the debt file,
   the commits whose paths owed it (bisect candidates).
main moves only to a state that passed both tiers; installata follows
in main as today.

## IV. The impact list (agent's discretion)

`tools/portae_debitae.sh -ramus` (and `silva.portae_debitae_ramus()`):
owed gates since main, each with tier, median cost, and whether it
would actually run (live receipt or fabrica-reused pass) - plus the
current debt file. The agent picks extra gates from it
(`commissio(..., portae=[...])` or `silva.porta(X)`).

## V. Order of work

T1 `gradus` in lib/portae.c + reader/writers + tests + plant; T2 the 15
annotations; T3 commissio (dilatae, debt file, estimate, `plena`,
explicit always) + pythonica tests + plants; T4 `fusio_probare` +
tests; T5 the branch impact list; T6 merge ritual note in the worktree
memory and CLAUDE-facing docs; measure the next real commit.

## VI. Not in this slice

Incremental examen-corpus (per-file verdicts keyed by checker + include
closure - the performance list); merge-tier parallelism; changing
which gates exist.

## AUDIENDA

- whether `git diff` against the merge base covers renames/deletions as
  portae expects (deleted paths cannot be enumerated - today commissio
  gets deletions from the index);
- whether a debt recorded in one worktree and the merge run in another
  needs sharing (today each worktree merges itself - per-tree file is
  enough);
- frigida's own leak (…APY6G) before it becomes a mandatory step.

## VII. AS BUILT

- **T1** (2026-10-15): `gradus` in the closed attribute vocabulary of
  lib/portae.c (`PortaGradus` PORTA_COMMISSIO/PORTA_FUSIO,
  `PortaDeclarata.gradus`, `PortaeOrdo.gradus`); unknown value refused
  by name (`gradus ignotus 'X' in <porta> (commissio fusio)`).
  `declaratae` writes `GRADUS <porta> fusio` / `    gradus fusio` only
  for the non-default tier; `ordo -machina` lines have a fifth field
  `commissio|fusio`, the human line marks ` [fusio]`.
  `silva._portas_ordinare` now returns (portae, genera, gradus) and
  accepts ONLY five-field lines (a four-field line from a stale
  bin/fabrica = MONITUM, order as given). Tests: probatio_portae XV
  (+XIV fifth field), pythonica ordo D; plants II red (parse, ordo copy).
  No gate annotated yet (T2) - behaviour unchanged.
- **T2+T3** (2026-10-15, one commit - T2 alone changes nothing, T3 alone
  dilates nothing): the 15 merge-tier runners carry `gradus="fusio"`.
  commissio: `_portae_debitas_addere(..., additae_out=)` collects the
  ADDED gates (not the requested ones); `_portas_ordinare(portae,
  additae, plena)` splits them - `porta debita DILATA ad fusionem: X -
  causa` for tier fusio, `porta debita addita` for the rest - removes the
  dilated from the order line, prints `aestimatio portarum: N s[, K sine
  historia][; dilatae ad fusionem: M]` and a MONITUM over
  `AESTIMATIO_MONITUM` (300 s). Order is now debts -> ordo -> instrumenta
  (binaries healed only for gates that run). After the commit,
  `_debita_fusionis_scribere(h, dilatae)` appends `tempus porta commissio
  causa` to `build/portae/debita_fusionis.tsv` (`DEBITA_FUSIONIS_VIA`,
  env `SILVA_DEBITA_FUSIONIS_VIA`); `debita_fusionis()` reads it.
  Uncomputable order = nothing dilated (more runs, never fewer).
  `plena=True` = old behaviour; commissio_umbra follows the same split.
  This commit itself ran with `plena=True` (it would otherwise have
  dilated pythonica, the gate that tests it). Dry run on this very diff:
  27 s commit tier, 15 dilated (the ~25 min it would have cost before).

