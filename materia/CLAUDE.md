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

## Visio (2026-09-15)

**A filtered projection is a view, not the document.**
`MateriaArborConsilium.loci_admissi` (a flat `b32` mask over the
registry's loci; NIHIL = everything) makes the writer skip unadmitted
loci; the envelope is stamped `visio="partialis"` +
`omissi="genus/locus …"` and the reader REFUSES it ("visio partialis:
non arbor"). A view lives in memory for a pattern consumer (arbor
tradita); it never becomes a file. The mask is the client's table
(oratio derives it from the element names its rules use). Plan:
`project-specs/proiectio-macrior-plan.md`.

## Sedes (2026-09-17)

**A positioned projection is a view.** `consilium.sedes_scribere` stamps
`sedes="L:C-L:C"` (1-based lines, byte columns, end exclusive) and
`octeti="B-B"` on every node, token and trivium element; locus wrappers
never. A node's range runs over its loci tokens (trivia are not loci),
accumulated bottom-up in the writer's one walk. A DERIVED token (md,
oratio) is a zero-width point. The envelope says `visio="sedes"` (or
`"partialis sedes"`), the reader refuses it, canons never see it. Range
API: `materia_tractus_lexematis` / `_nodi` / `_conferre`
(`materia_nodus.h`). Every client STML gate calls
`materia_sedes_verificare` (`materia_sedes.h`) per corpus file: an
oracle disjoint from the writer (line table from the source bytes,
source slices, document order) that also proves the view equals the
plain projection minus its attributes and that the reader refuses it.
Instruments: `<cliens>/arbor.sh -sedes`. Spec `project-specs/materia-sedes-spec.md`, plan
`project-specs/materia-sedes-plan.md`, worklog
`fontes/materia_arbor.worklog.md`.

## Diagnostica (2026-09-17)

**Declared in the registry, derived by ONE walker.** A client writes no
error-reporting code: its declaration marks tree shapes
(`diagnosticum` / `absentia` / `vacua`, above), and
`materia_diagnostica_derivare` (`materia_diagnostica.h`) walks any tree
— parsed, read back, or a view — returning `MateriaDiagnosticum` rows
(severity, codex `genus/locus`, cause, node or token, `MateriaTractus`),
sorted by (start, codex). GENUS and VACUA point at the node's range;
ABSENTIA at a ZERO-WIDTH point at its end; a node with no source token
of its own at a point after the last token seen before it. Every client
also gets `materia:ordo-octetorum` free: a source token starting before
the greatest end already seen (crusta's transposed heredoc,
generalised). What a parser must report itself — a class leaving no
trace in the tree — arrives through `emissa`. **The writer's refusal
carries a place**: `MateriaArborScriptura.tractus` (last field), from
the refused token when known, else the `sedes` node; `initium` −1 =
none. Printing (excerpt, caret) is NOT materia's: the substrate gives
the place, `lib/excerptum` and `./tools/diagnostica.sh` give the form.

**ARC CLOSED 2026-09-17** (twelve commits `325180d5` → `45e0eb3b`;
spec §16 "As built" governs, RELATIO + instrument debrief in the
phase-log). Positions verified over 5,501,210 elements across five
clients, zero divergences at first run; the view costs ×2.47 the plain
projection. Declarations today: crusta 1 genus + 28 `absentia` + 7
`vacua` + 3 `inanis` + 5 `gravitas`, css 3 genera, md/html/oratio none
— and **no seal moved**. Reach for it with
`./tools/diagnostica.sh <file…> [-machina] [-sine-excerpto]` (exit 0
clean / 1 error / 2 nothing judged; the whole house, 235 `.sh`, in
0.22 s) or `silva.diagnostica_materiae(viae)` in Python — **NOT**
`silva.diagnostica`, which is the legati C89 verdict and a different
thing entirely.

**If you add a diagnostic and its plant stays green, the gate is dead.**
This arc hit that three times, each time because the corpus lacked the
case rather than because the rule was wrong (a rule with no corpus
shape, a pin with no bash golden, a UTF-8 fixture too narrow to
discriminate), and a fourth time because a walk could not reach the
tree the class lives in. Ask what the gate would print if the thing it
watches never happened — and ask WHICH TREE the walk actually walks.

## The whitespace contract (2026-08-28)

**A `VERBATIM` trivium whose value is whitespace-only projects to
STML, because the element is raw.** Pretty owns layout *outside* raw;
inside raw the bytes are verbatim. Measured, not argued:

```
<lex-spatia!>\n\n  </lex-spatia>   pretty  ->  "\n\n  "  IDEM
<lex-spatia>\n\n  </>              pretty  ->  TEXT LOST
```

The reader (`_textus_directus`) skips whitespace-only text **unless
`elementum->crudus`**. Both halves are load-bearing; both were
verified by planting.

**NUL was the last unrepresentable byte, and is no longer one
(2026-09-19).** The writer used to refuse it (`_nul_fert`) on the
grounds that raw could not carry it — true, and the wrong conclusion.
A byte the *form* cannot carry can be **stripped and its offsets kept
in an attribute**, which is what `cr` had already done for CR twelve
lines away. So `_cr_exuere`/`_cr_induere` became
`_octetum_exuere`/`_octetum_induere`, parameterised by (byte,
attribute), and `nul` joined `cr`. Neither `&null;` nor a `<byte/>`
element can work here: entities are not resolved inside raw (that is
what raw *means*), and an element would make the value MIXED, which
cannot be raw — both would trade raw away to keep NUL and lose the
whitespace and closing-tag cases with it.

**ORDER IS LAW.** Write strips CR first (offsets in the TRUE value,
the old contract, so every existing `cr` document stays byte-identical)
then NUL (offsets in the CR-stripped value); the reader restores NUL
first, CR second. **Swapping the read order passed every suite** until
a case carrying BOTH bytes was added to `probatio_materia_arbor` — a
pin you cannot make fail is measuring its neighbour, not the world.

Measured: html's `circuitus` gate went 1,672/1,708 → **1,708/1,708**;
NUL refusals across the html5lib corpus 36 → 0. `nul` is declared
beside `cr` in all six canons. NB `_textus_tutus` still carries a
`_nul_fert` check that its only caller can no longer reach (the value
is stripped before it); kept as a cheap guard, revisit if a second
caller appears.

**THIRD INSTANCE, 2026-09-19** — same shape, different pair: the STML
raw-region TERMINATOR was defined twice. The scanner stopped at `</T`
followed by `>`, `!` **or whitespace**; `_valorem_crudum_notare`
refused only `</T>`. So a value carrying `</T ` passed the guard, was
written raw, and the reader could not parse our own document
(`STML parsari non potuit`). Loud only by luck. Fixed by EXPORTING one
predicate (`stml_crudi_terminator_est`) that both call — not by making
the guard agree, since a guard that merely agrees today drifts again.

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
also carries trivia therefore still cannot project — CSS content
mode (D7), a T11 problem, named ahead of time.
**Reached from the other side by crusta (2026-09-16, P7):** a value
whose LEADING or TRAILING whitespace run contains a newline, in a
mixed element, was written escaped and the reader dropped the run by
the ownership law — two bytes silently lost on a quote open to EOF
(`a 'b ` + newline). `_textus_tutus` now REFUSES it (`valor lexematis
textui non tutus (mixtum)`); whitespace without a newline at the
edges still projects. Measured: `'b \n`, `'b\n `, `'b\n\n`, `'b\t\n`
lost the whole run; `'b ` survived; `'b\n` as a sole child is raw
and survives.

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

**Every CLIENT runner must list `materia/fontes` in its guard, and
now checks itself (2026-09-17).** css, md, html, oratio and crusta
listed only `include/` and their own directories, so A1's new last
field in `MateriaArborConsilium` rebuilt nothing that merely included
`materia_arbor.h`: `oratio_resolutio.o` kept the smaller struct on its
stack while the fresh `consilium_nudum` wrote the new field past it.
Their guards now list `materia/fontes` (plus `md/fontes` where md is
compiled), and after building each runs `./excubitor.sh
<cliens>/build/ -tacitus`, which reads the DERIVED include graph: a
stale object after the build means the hand list missed a header, and
the runner exits 2 before any test compiles. Instruments relink on any
newer `*.o`; both `oraculum.sh` refuse stale objects the same way.
Adding a substrate header directory therefore needs no runner edit to
be noticed — the check names the object and the header.

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

**Diagnostics are a second, separate table (2026-09-17, materia-sedes
B1).** The same declaration may mark tree shapes as diagnostics:
`diagnosticum="causa"` on a genus, `absentia="causa"` / `vacua="causa"`
on a locus (`vacua` only on list species — the coctor refuses the rest),
`gravitas="monitum"` on either, `inanis="verum"` on a genus whose nodes
do not fill a `vacua` list. They bake into `<PRAEFIXUM>_DIAGNOSTICA`
(`MateriaDiagnosticaCocta`, `materia_registrum.h`), generated ONLY when
something is declared, so a client without declarations regenerates
byte-identical. **Diagnostics move no seal and no document**: the seal
hashes names, offsets and species only. Causes are short (≤ LX bytes),
without control bytes, and pass VERBATIM — the reader does not decode
entities in attribute values, so a `"` goes in the attribute-element
form `<@absentia=>…</>`.

`materia/probationes/lexicon_c89.{h,c}` is GENERATED by
`materia/instrumenta/lexicon_c89_generare.sh` from silva's sources.
Do not hand-edit. The generator carries its own gate: it re-derives
`_textum_fert()` from `silva_canon_coquere.c` — a hand-written list in
a file its rules never read — and REFUSES to emit if
`species == VERBATIM` diverges from it.

That descriptor's home is the C89 frontend, which lives in silva. It
sits in materia's probationes only while phase 1 runs, because it is
the gate's vehicle. Phase 5 moves it.
