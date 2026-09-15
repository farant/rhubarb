# Proiectio macrior — plan (materia wish 01M24Z4Q5Y, point c, after step one)

*Born 2026-09-15 from two measured spikes (tables in
`oratio/fontes/oratio_resolutio.worklog.md`, entries of that day).
Step one, the writer's STML tree handed over without text, is built
and committed (f423636a). What remains of the re-projection cost is
the pattern expansion: 52–60 % of resolution, flat per rule, each rule
a separate pass over a 52–125 KB projection. This plan takes the two
cheap levers and leaves the engine alone.*

**Goal.** Cut the expansion's cost without changing one row: every
tranche is accepted by the machine rows of the nine Latin treebanks
and the English EWT files being BYTE-IDENTICAL to the committed
behaviour (`oraculum.sh -machina -decreta -lites`, diffed), then by
the wall time.

**Constraints (house).** C89 in Latin via latina.h, house flags; every
gate red at birth by a planted fault; docs first, gates last, commit
through `silva.commissio` with explicit paths; one tranche per turn
with Fran's approval; pins move only with a named cause; a table,
never a program (M7); materia stays thin (M8) — the substrate gets a
filter and a contract, never knowledge of what a slot means.

**Spec.** materia-spec §9 "Arbor tradita" and the paragraph "Visio"
that follows it (the view contract); oratio-spec §2 decisions 60–61.

## Tranche a — language gating of rules (oratio only)

Fourteen English rules run on every Latin sentence and cannot match:
~13 % of the expansion (per-rule spike).

1. `oratio/partes/resolutio.stml`: attribute `lingua="latina|anglica"`
   on every `<regula>` that is language-bound (the `-latinum` /
   `-anglicum` titles); canon `oratio.canon` declares the attribute
   (electio, optional: absent = every language).
2. Loader `oratio_resolutio_programma_legere`: `OratioRegula.lingua`
   (index, −1 = any).
3. Stage loop `_sententiam_resolvere_gradu`: a rule whose `lingua` is
   set and differs from `cursus->lingua` is not appended to the stage's
   rule text. A stage left without rules is skipped (already the law).
4. Gate: rows byte-identical on the nine treebanks AND on
   `la`-less input (EWT dev/test); `probatio_oratio_resolutio` law: a
   two-rule inline programme with one rule gated to the other language
   yields the rows of the ungated rule only. Plant: comparison
   inverted (the gated rule runs, the ungated one does not) → red.
5. Measure: Perseus wall before/after; record.

## Tranche b — the view: a filtered projection (materia only)

1. `MateriaArborConsilium.loci_admissi`: a table of admitted locus
   TITLES per genus title (NIHIL = project everything, today's
   behaviour). Species-blind: the writer walks as before and skips a
   locus whose title is not in its genus's row. Loci of species
   LISTA_NODUS that are omitted are omitted whole (their subtrees are
   not reachable); the table is the client's responsibility.
2. THE VIEW CONTRACT: a projection with a filter is not the canonical
   document. The writer stamps the envelope `visio="partialis"` and
   `omissi="<genus>/<locus> …"`; `materia_arbor_legere` REFUSES an
   envelope carrying `visio` with a named cause ("visio partialis: non
   arbor"); the canon judge does not see a view (it never reaches a
   file); the comparator is not involved. A view exists only in memory
   for a consumer that matches over it (arbor tradita).
3. Gate (`probatio_materia_arbor`): with a filter the omitted loci are
   absent from the tree and the admitted ones present in order; the
   envelope carries the stamp; the reader refuses the serialized view;
   with NIHIL the bytes equal the unfiltered writer's. Plant: the stamp
   dropped → the reader accepts a view → red.
4. Records: materia-spec "Visio", materia/CLAUDE.md, phase-log.

## Tranche c — the derivation (oratio)

1. At program load, walk every parsed `<EXEMPLAR>` of every rule and
   collect the element names it uses (including `<*>` wildcards, which
   admit nothing by themselves but mark the genus as matched). Map the
   names against the registry: genus titles name nodes, locus titles
   name the admitted loci; structural lists the patterns descend
   through (`elementa`, `analyses`, `umbrae`) are admitted because they
   are named. Everything a rule never names (lemma, lingua, fons,
   nativum, sensus, alternae, auctor, habitus, decisio, clausula …) is
   omitted. The set is the program's, computed once, printed by
   `oraculum.sh -regulae` for inspection.
2. The stage loop passes the set as the consilium's `loci_admissi`.
3. Gate: rows byte-identical on the nine treebanks and EWT. A
   strict-cursus pattern that begins to match across an omitted sibling
   shows up as a changed row on a named file: the decision is then
   explicit (admit that locus, with the cause), never a silent widening.
   Probatio law: the derived set for the committed programme equals a
   hand-listed expectation (a fixture that moves only with a cause).
   Plant: a locus the rules name dropped from the set → rows change →
   red.
4. Measure: Perseus wall and the projection's node count per stage
   before/after; record in the worklog and the ledger.

## Named after this plan

- Step two of wish (c): rows name nodes by ANCHOR instead of ordinal
  (writer mints `id` for every node on request; executor resolves by
  anchor). Identity across the seam; independent of a–c.
- A multi-pattern pass in `lib/stml_macros.c` (48 passes → one).
  Engine-level; the loose head tier stays 14 literal rules by the
  measured T27/T28 decision, so this is the only lever left for it.

## Status

| tranche | status |
|---|---|
| plan + spec paragraphs | written 2026-09-15 |
| a language gating | done 2026-09-15 — `lingua` on 18 rules + canon, `OratioRegula.lingua`, `_regula_linguae_cursus`, counter `regulae_omissae_lingua` printed as `REGULAE-OMISSAE-LINGUA`; nine treebanks byte-identical (EWT omits the six Latin rules); Perseus 5.6 → 5.2 s; plant = comparison inverted → skeleton law red |
| b the view (materia) | done 2026-09-15 — `loci_admissi` flat mask on the consilium, writer skip, envelope `visio`/`omissi`, reader refusal; arbor gate (omitted locus absent, stamp, refusal, NIHIL bytes identical); plant = stamp misnamed → red |
| c the derivation (oratio) | — |
