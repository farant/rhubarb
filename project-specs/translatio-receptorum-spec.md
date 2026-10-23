# translatio receptorum — carrying green gate verdicts across trees (spec)

Decided with Fran 2026-10-23: decretum …DC4A8Z (option A, shadow first,
then option B gate by gate). Principles: …Z2Y555 (logic in C; a bun or
ruby wrapper must reimplement no more than "call, parse TSV") and
…RBA4W73 (what decides an outcome lives in the repo). Builds on
`portae-declaratae-spec.md` (annotation, `lib/portae.c`,
`bin/fabrica portae`), and is that spec's §VI follow-up, "runtime inputs
observed instead of `tegit`", in the narrow form a receipt needs.

## 0. Why (measured)

- **The rerun is expensive.** A merge that goes red once reruns every
  gate with no cache of its own. Today's pass 2 (670a2193 → d8e06d3b)
  took 27.6 min for 52 gates, of which ~25 min were gates without a
  fabrica memo. The fix was one golden `.tsv`, which owes only `silva`.
- **Why that happens.** A live receipt (`build/portae/<g>.viva.json`) is
  bound to `sigillum_arboris()`, i.e. the WHOLE tree, and commissio
  deletes all of them after every commit (pythonica/silva.py, receptum
  vivum comment). That was chosen deliberately on 09-07, because the
  oratio lint reads every .c/.h/.md, so a scope declared per gate
  would lie.
- **The replay of A over mensor history** (4,500 consecutive pairs of
  gate runs, 10-13..10-23):
  - with fabrica-judged gates included, 610 carries and 1 false
    negative (generata, park …AKFKR);
  - with them excluded, 405 carries (163 min) and 0 false negatives.
  - The commit-time lints mask corpus gates, so that zero is weak
    evidence.
- **The reads audit (A.2a, 40 gates traced with FABRICA_LECTIONES)
  confirms the 09-07 concern.** Eleven gates read the corpus far beyond
  their declared coverage: oratio, silva-m3, examen-corpus, pythonica,
  portae-declaratae, instrumenta, unci, crusta, diagnostica,
  interrogationes, fabrica-fumus. Detail is in the decretum's notes.

## I. INTERFACE A — the annotation (for Fran's approval)

Two attributes are added to the closed vocabulary of `<porta>`:

```sh
# <porta titulus="oratio" gradus="fusio" tegit="..." legit="**/*.c **/*.h **/*.md"/>
# <porta titulus="venenum" translatio="numquam"/>
```

- `legit` — path patterns (exemplar syntax, like `tegit`) that the gate
  READS beyond its derivation: a corpus, prose, the whole tree.
  **It counts only for carrying receipts; it never changes owed
  sets.** Commits and merges owe exactly what they owe today, and the
  commit-time lints keep guarding the corpus as they do now. A green
  verdict is NOT carried when any changed path matches `legit`.
- `translatio="numquam"` — this gate's verdict is never carried. It is
  for gates whose verdict IS a fabrica judgment (composites, generata,
  venenum, briar-fumus, and aedilis through its `iudicium` action): the
  fabrica memo is finer (per member, read-traced, drift-guarded) and
  stays the authority. A rerun costs them seconds. Any other value is
  refused.

## II. INTERFACE B — the C core and the CLI (for Fran's approval)

`include/portae.h` and `lib/portae.c`:

```c
/* PortaDeclarata gains */
        Xar* legit;          /* chorda: exemplaria lectionis */
         b32 translatio_numquam;

nomen enumeratio {
    PORTAE_TRANSLATIO_PORTAT = ZEPHYRUM, /* viae nihil huius portae tangunt */
    PORTAE_TRANSLATIO_DEBITA,            /* via portam debet (tactus) */
    PORTAE_TRANSLATIO_LECTA,             /* via 'legit' congruit */
    PORTAE_TRANSLATIO_NUMQUAM,           /* translatio="numquam" */
    PORTAE_TRANSLATIO_INSTRUMENTA        /* binarium quod porta currit mutatum */
} PortaeTranslatioGenus;

nomen structura {
                    chorda  porta;
    PortaeTranslatioGenus   genus;
                    chorda  causa;   /* via prima + ratio, '(+N aliae)' */
} PortaeTranslatio;

/* Per portam quaesitam: verdictum viride ad arborem praesentem portari
 * potest? viae = mutatae inter arborem recepti et praesentem. Purum
 * (sutura, ut debita); instrumenta a vocante iudicata. */
Xar*
portae_translationes_computare (
    constans PortaeSutura*  sutura,
             constans Xar*  operturae,
             constans Xar*  viae,
             constans Xar*  portae,      /* chorda: tituli quaesiti */
                  Piscina*  piscina);
```

Order of judgment per gate: NUMQUAM, then DEBITA (the same tactus as
`portae debitae`, so blind gates are owed by every path), then LECTA,
then PORTAT. The binary-digest comparison (INSTRUMENTA) happens in the
CLI, because digests are disk state, not repository logic.

CLI (`tools/fabrica_portae.c`), declared TSV contract added to the
EFFUSIO block of `portae.h`:

```
bin/fabrica portae translatio [-machina] -portae TSV
        [-receptum <porta> <arbor> <instrumenta>]...  <porta>...
  ARBOR        <arbor praesens>                    (git tree hash)
  INSTRUMENTA  <porta> <digestum>                  (binaria quae currit)
  TRANSLATIO   <porta> <portat|debita|lecta|numquam|instrumenta|nullum> <causa>
```

- `arbor` is the git tree of the WORKING state (tracked changes plus
  untracked files that aren't ignored), written through a temporary
  index (`GIT_INDEX_FILE`, `git add -A`, `git write-tree`). That makes
  a dirty commit-time tree diffable later
  (`git diff --name-only <a> <b>`).
- `digestum` is the sigillum of the contents of the `bin/x` files the
  gate runs (`PortaeOpertura.binaria`, the same list the preflight
  heals).
- A gate with no `-receptum` gets `nullum`. An unknown tree (pruned)
  is reported as `debita`, with the cause naming the tree.
- One call per pass, covering all gates.

## III. The wrapper (what stays in Python)

`silva._portas_currere`:
- calls `translatio` once before the loop;
- prints one line per gate in shadow ("umbra translationis: X portaret
  (…)"), and stores nothing that changes a verdict;
- after a green, non-stale, unfiltered live run, writes
  `build/portae/translatio/<g>.json` = {arbor, instrumenta} from that
  call;
- appends each decision and the actual verdict to
  `build/portae/translatio_umbra.tsv`, flagging DISSENSUS (would carry
  but red).

A bun wrapper reimplements: call, parse four line types, write a json
file. Shadow (A.2c) changes nothing about which gates run; switching on
(A.3) means a PORTAT gate is not run and its line says so.

## IV. Tasks

- **T1** C core: parse `legit`/`translatio`, `portae_translationes_computare`,
  effusio (LEGIT/TRANSLATIO lines in `declaratae`), tests with a fake
  sutura in probatio_portae.c. Born red.
- **T2** CLI `translatio`: arbor via the temporary index, digests,
  `-receptum`; a fumus case in tools/portae_declaratae_fumus.sh.
- **T3** Annotations:
  - `translatio="numquam"` on the fabrica-authority gates;
  - `legit` on the class-4 gates from the audit;
  - examine class 3 (credo, bearssl, qr-gyrus, iudicium-fumus,
    silex-semen, examen-vectis) one by one.
- **T4** Wrapper shadow and its pythonica test.
- **A.3** Switch on after several merges with zero DISSENSUS (Fran's
  call).

## IV-bis. Decisions (Fran 2026-10-23)

- **The interfaces are approved as written.** That covers `legit` and
  `translatio="numquam"` (A), and the C core plus the CLI (B). The logic
  lives in C. The test: a bun wrapper reimplements only a thin call.
- **Scope (a) first.** A record counts only between the passes of ONE
  merge attempt, or the retries of ONE commit. A merge's first pass
  stays fresh, exactly as today. Scope (b), carrying across commits,
  needs a separate decision, made with shadow data.
- **Every decision names its cause in place** (e.g. `currit - lecta:
  lib/x.c ~ **/*.c`, `translatum ab <arbor>: N viae, nulla huius
  portae`), and the CLI can be queried with no run. That is the
  condition for the mental model staying simple.
- **A concept to watch, not acted on.** *Owed* = responsibility (what
  this gate is FOR: derivation + `tegit`); *carried* = dependence (what
  can change its answer: derivation + `legit`). Today's `tegit` mixes
  the two; fabrica already separates them (an action's subject vs its
  ingressus), so composites (option B) may make `legit` redundant. Fran
  asked to check in when such concepts surface.

## V. AUDIENDA

- Tracing can't see Python, plain-shell or raw-IO reads (class 3; also
  examen-corpus, which is blind but corpus by design).
- Undeclared reads overstate dependence wherever a gate rebuilds its
  own tool (class 5: header mtime scans).
- officina can't run under tracing (…AV0N8).
- The replay used today's declarations, not historical ones.
