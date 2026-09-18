# excusatio declarata — a finding is an error unless the author says why

*2026-09-18, the morning after the adaptator lintris arc closed at
`d75ef03c`. Origin: desideratum `01M2SD93PV`, which is Fran's idea and
better than the `monitum` I had proposed. Ordered THIRD by decree
`01M2RNK5MG` — after the multi-span record (`01M2RKYA4Z`) and the
`Congruentia` adapter (`01M2RNJ9XN`), both shipped — and reached by way
of survey `01M2PRPGBC`, whose rule 1 turned out not to be a rule
problem at all.*

**What it is.** A lint rule can only be an error if every finding it
produces is a defect. Rule 1 (`-nt` used where equality matters) finds
29 sites, of which roughly 11 are defects and the rest are legitimate.
The difference is not structural — same tree, same shape — it lies in
what the body does with the answer. So the rule is either a weak
warning nobody reads twice, or it tries to infer intent, which is
tier 3 and the ladder excludes it on purpose.

This spec inverts it. **The author declares the intent; the lint does
not infer it.** A construct may carry an STML annotation inside an
ordinary comment saying *I meant this, and here is why*. The rule then
becomes an `erratum` — a gate that goes red and stays red — and the
legitimate sites gain one line each that states a reason, which is
documentation the house does not currently have.

This is the same move the diagnostics arc already made one layer down:
the registry **declares**, one walker **derives**.

## 0. What was already built, and how I found out

The ticket said this annotation layer was banked-but-unbuilt (parked
design `01KY3D7EJP`) and that `TOLERA` was a C precedent worth
examining. That understates it. The chain shipped 2026-07-22:

- **`silva/fontes/silva_annotationes.{h,c}`** walks trivia, strips ` * `
  decoration, runs `stml_legere` on any comment whose first non-space
  byte opens a tag, and attaches the result to a node (`SUPRA` /
  `INTERIOR` / `PLAGULA`). A comment that opens like an annotation but
  will not parse stays a **finding** — `silva_annotationes.h:8`:
  *"numquam tacite prosa reclassificatum"*.
- **`<tolera codex="…" (>causa`** is a live consumer at
  `silva_c89_semantica.c:1069`, suppressing severe examen codes.
- The law I filed as *build this in from the start* — **a dead
  suppression is a defect** — **is already implemented there.**
  `silva_c89_semantica.c:1084`: a tolera that absorbs nothing, carries
  no cause, or names an unknown code yields `IRRITUM`, and the comment
  cites the reasoning (*"lectio eslint: suppressiones putrescunt"*).

The ticket says *"TOLERA ipsa hoc probabiliter caret — inspiciendum."*
It does not lack it. It has it, with the causeless case and the
unknown-code case both covered. **Recorded because the shape of the
error repeats:** I reasoned from a memory of a design instead of
reading the code, exactly as I did with the `-nt` census, where I
looked for `!` on the `imperium` and it was on the `pipa`.

What genuinely does not transfer: `silva_annotationes` is bound to a
`SilvaParsura`. materia has no sibling. That sibling is this spec.

## 1. Decisions

**EX1 — the vocabulary is `<tolera>`, keyed by `codex=`.** One element
across the house, per the banked decree *familiae novae = vocabularium,
non grammatica*. The key is **the exact string the printer prints**:

```bash
# <tolera codex="lint:nt-aequalitas" (>caput recentissimum, aequalitas nihil refert
if [ -n "$X" -a "$X" -nt "$bin" ]; then
```

You suppress with the code you saw in the error message. This falls
out of the adapter arc for free, which gave every lint finding the
codex `lint:<name>` and every declared one `<grammatica>:<codex>`.

**EX2 — cause is mandatory.** A causeless tolera does not suppress; it
reports. Adopted verbatim from `silva_c89_semantica.c:1077`.

**EX3 — the scope is the owning node's span.** Not the file, not the
line, not a heuristic. See §2 — this is the decision the whole design
turns on.

**EX4 — collection is C, in materia, generic over annotation
families.** Not a tolera reader. Every future family (`nid`,
`intentio`, `contractus`) reuses it, and all six clients inherit it
without writing C.

**EX5 — the comment genus and its decoration are DECLARED in the
registry**, as a separate cocta table. Not new fields on
`MateriaTabGenus`: hand-written registry tables (briar, the C89 shim,
probationes) would break under `-Wmissing-field-initializers`. This is
exactly how `MateriaDiagnosticaCocta` was added in the sedes arc, so
the cost is known and the other clients stay byte-identical.

**EX6 — the tolera semantics live in `materia_diagnostica`, not in the
instrument.** Exemption applies to tier-1 declared findings as much as
to tier-2 lint. `<tolera codex="crusta:grex/tok_clausura">` works the
day this lands.

**EX7 — a dead suppression is itself a diagnostic**, positioned on the
comment, `gravitas` = `erratum`. Absorbing nothing, no cause, or
unparseable all qualify.

**EX8 — unknown-codex validation is partial, and honestly so.** The
registry holds every declared tier-1 codex
(`MateriaTabDiagnosticum.codex`), so those are checkable now. `lint:`
names cannot be validated until a rule registry exists. **Named
trigger: the rule runner.** Until then a tolera naming a lint that did
not run this pass is not judged — never silently treated as valid.

**EX9 — Python orchestrates files; C does all judgment.** The extractor
(`silva._relata` and friends) stays Python for this arc by decree
`01M2S9GR5W`; everything new is C.

## 2. The scope, and the measurement that set it

The first draft of this design proposed **line-adjacency** — a tolera
covers the following line — because the Python adapter has only flat
rows and cannot find an owning node. The positioned tree says that was
a workaround for the wrong boundary, not a real constraint.

Measured 2026-09-18 on a planted fixture:

```
<programma sedes="3:1-9:1" octeti="96-201">
  <conditio sedes="3:1-5:3" octeti="96-149">
    <tok_apertura(>
      <crusta-reservatum sedes="3:1-3:3" octeti="96-98"><ante>
        <crusta-commentum! sedes="2:1-2:84" octeti="12-95">
          # <tolera codex="lint:nt-aequalitas" (>…
        </crusta-commentum>…</ante>if</>
```

Two facts:

1. **Trivia is excluded from node spans.** `<conditio>` starts at byte
   96, at `if`; the comment occupies 12–95. So the comment is *outside*
   the span of the node it is attached to, and no containment test can
   recover the owner. Even `<programma>` starts at 96 — leading trivia
   belongs to no node's span.
2. **Attachment is nonetheless exact, structurally.** The comment sits
   in `spatia_ante` of the token in locus `tok_apertura` of `conditio`.

`MateriaToken` carries `spatia_ante` / `spatia_post` as arrays of
`MateriaToken*` (`materia_token.h:85`). A C walk descends nodes and
reads each token's trivia, so **the owning node is known by
construction** — no ancestor heuristic, no dependence on tag-naming
conventions like `tok_*` versus `crusta-*`. This is how
`silva_annotationes` derives `SUPRA`.

Therefore the scope is the owning node's span, and the granularity is
the grammar's: a comment before `if` scopes to the whole `conditio`; a
comment mid-command scopes to that command. **There is no file-wide
disable, and there cannot be one** — leading trivia at the top of a
file attaches to the first construct, not to the document.

## 3. The collector

`materia/fontes/materia_annotationes.{h,c}`:

```c
nomen structura {
              chorda  textus;        /* corpus purgatum */
              chorda  crudum;        /* octeti pleni */
      MateriaTractus  commentarium;  /* sedes commentarii ipsius */
constans MateriaNodus* nodus;        /* nodus possidens */
      MateriaTractus  scopus;        /* sedes NODI - AMBITUS */
                 b32  parsata;
          StmlNodus*  arbor;         /* NIHIL si fractum */
          StmlStatus  status;
                 i32  linea_erroris;
                 i32  columna_erroris;
} MateriaAnnotatio;

Xar*
materia_annotationes_colligere (
                    Piscina* piscina,
      constans MateriaNodus* radix,
constans MateriaAnnotationesCocta* declaratio,
        InternamentumChorda* intern);
```

`scopus` is the whole exemption semantics in one field. Ordering is
source order. Unparseable annotations are retained with `parsata =
FALSUM` and their positions — the consumer reports them; the collector
never discards.

## 4. The registry declaration

Declared per client as `<PRAEFIXUM>_ANNOTATIONES`; generated only if
something is declared, so every other client is byte-identical.

```c
nomen structura {
                   s32  genus;      /* index generis commentarii */
    constans character* praefixum;  /* "#" - exuendum */
    constans character* clausura;   /* NIHIL pro linearibus */
} MateriaTabAnnotatio;

nomen structura {
    constans MateriaTabAnnotatio* tabula;
                             i32  numerus;
} MateriaAnnotationesCocta;
```

crusta declares genus `crusta-commentum`, praefixum `#`. Continuation
stripping (C's ` * ` on each line of a block comment, which
`silva_annotationes` implements) is **deferred with a named trigger: a
materia client whose comments are blocks.** None is.

## 5. The consumer

`materia_diagnostica_excusare(diagnostica, annotationes)`:

1. For each annotation with `parsata` and a root element named
   `tolera`: read `codex=`, read the line capture as cause.
2. A finding is suppressed when its **primary** span lies within the
   annotation's `scopus` and the codices match exactly. Related spans
   do not participate — a finding is at one place (SM1).
3. Suppression happens **before the exit-code decision**, so a fully
   exempted file exits 0.
4. Emit a diagnostic for each dead suppression (EX7), positioned on
   `commentarium`.

## 6. The rule, and the direction SINE searches

The two exemplars banked on `01M2PRPGBC` find the **negated** set.
Rule 1 needs the complement, and this is where the pattern fights:
`SINE` searches **downward**, inside the row's root, but `!` lives on a
`pipa` that is an **ancestor** of the `-nt`. A rule rooted at the
literal can never see its own negation.

So the row must be rooted at a scope containing both:

```xml
<EXEMPLAR output="$p"><probatio><**>
  <crusta-litteralis $t>-nt</crusta-litteralis></**></probatio></EXEMPLAR>
<CATENA de="$p" output="$cand"><SINE>
  <pipa><praefixa><crusta-reservatum>!</crusta-reservatum></praefixa></pipa>
</SINE></CATENA>
```

This works because `pipa` exists only with `|`, `!` or `time` (law C8),
so a bare test has no `pipa` at all and the `SINE` body simply fails to
match. **Which scope element is correct across all 291 sites is not
settled here** — `-nt` also appears under `while`, under `&&`, and as a
bare statement, and `probatio` is only the `if` case. The plan settles
it by reading the corpus, not by guessing.

## 7. Rollout

1. Re-measure A and B (the corpus grew from 289 to 291 during the last
   arc; it may have grown again).
2. Fix the ~11 defective sites to `! [ "$obj" -nt "$src" ]`. The
   doctrine and a worked example are at `tools/diagnostica.sh:22-23`,
   which carries the correct form *and a comment saying why*.
3. Annotate the ~18 legitimate sites, each with a real reason.
4. Rule 1 becomes `erratum`.

Step 3 is the load-bearing one: it is the first real use of the
capability, and it is where the trailing-comment question (§9) gets
answered by measurement.

## 8. Gates, each born red by a plant

| # | gate | the plant that must kill it |
|---|---|---|
| I | suppressed finding drops | remove the codex comparison — the wrong lint suppresses |
| II | **unsuppressed sibling survives** | widen `scopus` to the parent — the neighbouring construct goes quiet |
| III | dead tolera reported | skip the absorbed-nothing check |
| IV | causeless tolera reported, and does not suppress | accept an empty capture |
| V | malformed annotation reported | reclassify unparseable as prose |
| VI | exit code follows suppression | filter after the exit decision instead of before |
| VII | tier-1 codex exempts too | restrict matching to the `lint:` prefix |

**Gate II is the one this arc's own law demands.** The fixture's two
constructs must have *deliberately different* spans; a fixture where
they are equal cannot discriminate a scope that is one node too wide.
That law has now fired four times (the `é` plant, the gravitas pin at
118, the `1:1` opening span, the 999/1000 gutter) and is the dominant
failure mode.

## 9. Non-goals and named doors

- **No rule runner, no CI gate** (Fran, 2026-09-18). Rules stay invoked
  by hand until there are two or three to generalise from. The runner
  is also the trigger for EX8.
- **No file-wide or block-wide disable.** §2 shows the tree cannot
  express one, and that is a feature.
- **No extractor migration.** Decree `01M2S9GR5W`; sizing on
  `01M2RYR3JJ`.
- **Trailing comments** (`spatia_post`) attach to their owning node
  like leading ones, which may be narrower than a reader expects.
  Leading is the normal form and clearly correct. **Trigger: the
  annotation pass in §7 step 3** — if trailing turns out to be how one
  naturally writes these, that measurement decides the rule.
- **Block-comment continuation stripping** — trigger in §4.

## 10. AUDIENDA

1. Does `scopus` want to be the owning node, or the nearest node that
   *starts a line*? The second is more predictable to a reader and less
   predictable to implement. §7 step 3 is the evidence.
2. Should a tolera be allowed to name several codices
   (`codex="a b"`)? silva's does not. Cheap to add later, impossible to
   remove.
3. `materia_annotationes` returns every annotation, including families
   nobody consumes yet. Is an unconsumed family a dead suppression? It
   must not be — but the check in EX7 has to know the difference, and
   today it knows only `tolera`.
4. When the rule runner lands, does EX8's unknown-codex check want the
   set of rules that *ran*, or a declared registry of rule names? The
   second is stricter and is probably where `lint:` codices should have
   come from all along.
