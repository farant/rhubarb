# materia — orientation

*You are reading this because you touched a file in `materia/`.*

## Quid sit

materia is the general syntax-tree substrate, forked out of silva.
Silva is a forest; forests are made of *materia* — and scholastically
exact: materia is what receives *forma*, which is what a general
syntax substrate is, the matter a grammar informs.

**Silva is FROZEN to bug fixes while this fork lives** (see the GELATIO
notice atop `silva/CLAUDE.md`). Design: `project-specs/materia-spec.md`,
decree `01M12FJR`.

## Re-orientation (session start / post-compaction)

1. This file.
2. `project-specs/materia-spec.md` — M1-M8, the three layers, phase 0
   results, the six-phase roadmap (§10).
3. `materia/phase-log.md` — TAIL first. INTENTIO/RELATIO per phase.
4. The relevant `.worklog.md` next to files you're editing.

## THE LAW OF THIS FORK

**C89 is the ACCEPTANCE TEST, not the legacy burden.** materia is not
done when CSS works; it is done when silva's own gates run green
*through* materia (281/281 · 78/78 · 154/154 · hospes 39/39 ·
adversarial 24/24 · haruspex 243 TUs). A substrate proven only on CSS
proves almost nothing.

**materia stays THIN (M8).** The landscape's cautionary tale: rowan
stayed small and thrived; IntelliJ PSI tried to be general *and* rich
and got huge. Every "just one convenience that knows about a language"
is the rot. Language-specific code sits BESIDE materia, never inside.

**A table, never a program (M7).** Language definitions are declarative
data. No expression language, ever. It always starts as "we just need a
conditional here."

## Praesens status

- `materia_lexicon` — descriptor of a language's lexical vocabulary.
  Two axes: `species` (how a genus's bytes are determined) and `munus`
  (what role it plays). Replaces THREE hand-maintained silva tables.
- Degradation policy is **B** (Fran, 2026-08-27): capability judged at
  LOAD time via a derived `munera_praesentia` mask, not per call. A
  capability needing a `munus` the lexicon never assigns refuses once,
  loudly, with a named cause — because the alternative is a confident
  wrong answer (the CSS comment-block case; see `materia_lexicon.h`).

## Referentiae (2026-09-10)

**A node can name another node of the same tree without owning it.**
`MATERIA_LOCUS_REFERENTIA` / `MATERIA_VALOR_REFERENTIA` — a NEW value
kind (never a flag on NODUS), so every walk is taught not to descend.
Serialized as a NAME in STML's `#` document space: the target carries
`id="nodN"`, the locus prints `#nodN`; never a transclusion (a window
that canon judges through, refusing cycles and forward use — a
dependency graph has both). The reader resolves after the whole tree is
built; unknown or duplicate anchors refuse loudly; the writer refuses a
reference whose target lies outside the written subtree. The comparator
compares references by path RELATIVE to the comparison root. Placing
verbs (`ponere`/`appendere`/`reponere`) now fix `pater` on owned
children — parents are no longer "post-acceptum SOLUM". Born for oratio
(ordinals stale under permutation; remap bit twice); general to md
links, C89 identifiers, HTML anchors. Decree `01M260XYXB`; phase-log
2026-09-10. **An EMPTY reference (NIHIL target) is WRITTEN ABSENCE**
(2026-09-11): omitted on write, never produced on read, equal to an
unwritten locus in the comparator; in memory it stays written, so a
client can tell "revoked" from "never set". Refusing it silently
dropped oratio sentences out of later resolution stages.

## Arbor tradita (2026-09-15)

**The writer builds the STML node tree before it writes bytes; a
client that runs STML patterns over the projection can take the tree
and skip the text.** `materia_arbor_proicere_nodum` returns it in
`MateriaArborScriptura.arbor` (last field; `scribere_nodum` fills it
too). Law: serializing that tree equals the writer's bytes. INTERN
LAW for pattern clients: build the projection in the SAME intern as
the patterns (`consilium.intern`), or the machine's interned identity
comparison makes every pattern mute. Oratio was the first consumer
(resolution −40 %, rows byte-identical); measured in
`oratio/fontes/oratio_resolutio.worklog.md` (2026-09-15).

## The whitespace contract (2026-08-28)

**A `VERBATIM` trivium whose value is whitespace-only projects to
STML, because the element is raw.** Pretty owns layout *outside* raw;
inside raw the bytes are verbatim. Measured, not argued:

```
<lex-spatia!>\n\n  </lex-spatia>   pretty  ->  "\n\n  "  IDEM
<lex-spatia>\n\n  </>              pretty  ->  TEXT LOST
```

The writer refuses only **NUL** (`_nul_fert`) for trivia — NUL is
unrepresentable raw or not. The reader (`_textus_directus`) skips
whitespace-only text **unless `elementum->crudus`**. Both halves are
load-bearing; both were verified by planting.

*How it was wrong before, and why that shape is worth recognising:*
the writer refused whitespace-only and the reader skipped it, and the
reader's comment cited the writer as its justification — "contractus
unus per duas partes". They agreed, and **both were too wide**. Two
halves that cite each other as the reason look like a principle and
are not one. The writer's refusal sat two lines above
`_valorem_crudum_notare`, i.e. it guarded a condition the very next
statement removed.

The cost was real and invisible: any language whose whitespace genus
is `VERBATIM` — CSS, where one genus absorbs spaces, tabs and
newlines — could not project **any** file containing whitespace. C89
never hit it, so no C89 gate could have found it. Found by CSS's
second client, exactly as the fork predicted.

**Still constrained:** the *lexeme* path (`materia_arbor.c:945`)
raw-marks only when text is the element's sole child (`elementum
mixtum crudum esse NON potest`). A significant whitespace token that
also carries trivia therefore still cannot project — CSS content mode
(D7), a T11 problem, named ahead of time.

## Currere

```
./materia/compile_probationes.sh            # omnes
./materia/compile_probationes.sh lexicon    # filtrum substringae
```

Contract 0 sanum / 1 fractae / **2 = NULLA CURSA** (bad filter — never
gate on exit 0 alone). Log: `build/test_logs/materia.log`.

**A header-only change rebuilds nothing unless the runner's header
guard fires.** materia's runner carries that guard from birth; it
exists because a header-only edit in silva reported 50/50 against
six-hour-stale objects on 2026-08-27.

## Generata

**Registrum coctum (2026-09-15).** A client's genus enum and baked
tables (`<cliens>_registrum_coctum.{h,c}`) are GENERATED from
`<cliens>/grammatica/<cliens>.registrum.stml` by
`./materia/coquere.sh <declaratio> [-scribere]` (library
`materia_coctor`, canon `materia/grammatica/registrum.canon`). To add
a locus: edit the declaration, run `-scribere`, then move the client's
locus-count pin and the seal in its canon BY HAND with a cause. Never
hand-edit a `_coctum` file: the client's registrum probatio compares
it byte for byte with a fresh render and names the first divergent
line. Locus enums stay hand-written (client-shaped). All three clients
migrated (oratio, css, md; the hand tables and `css/md_registrum.c` are
gone — an empty translation unit is not C89). The gate itself is one
library call, `materia_registrum_recens`, the same in every client
probatio and in the instrument's compare mode. Python:
`silva.registrum_coquere(via, scribere=False)` shells to the instrument.
Worklog: `materia/fontes/materia_coctor.worklog.md`.

`materia/probationes/lexicon_c89.{h,c}` is GENERATED by
`materia/instrumenta/lexicon_c89_generare.sh` from silva's sources.
Do not hand-edit. The generator carries its own gate: it re-derives
`_textum_fert()` from `silva_canon_coquere.c` — a hand-written list in
a file its rules never read — and REFUSES to emit if
`species == VERBATIM` diverges from it.

That descriptor's home is the C89 frontend, which lives in silva. It
sits in materia's probationes only while phase 1 runs, because it is
the gate's vehicle. Phase 5 moves it.
