# excusatio declarata — implementation plan

> Execute INLINE on main, one task per turn, Fran approves each. No
> subagents. Steps use `- [ ]` for tracking.

**Goal:** a finding can be suppressed by an STML annotation the author
writes in an ordinary comment, scoped to the construct it precedes —
so a lint rule can be an `erratum` instead of a warning nobody reads.

**Ticket:** `01M2SD93PV` (Fran's idea). Survey `01M2PRPGBC` supplies
rule 1. Decree `01M2RNK5MG` step 3.

**Spec:** `project-specs/excusatio-declarata-spec.md` (`32245095`).
Read it first; this plan argues from it and **diverges from it in two
places, recorded below**.

---

## What is already true (measured 2026-09-18, do not re-derive)

**1. `silva_annotationes` is the working precedent, not a sketch.**
`silva/fontes/silva_annotationes.{h,c}` collects STML from C comments;
`<tolera codex=… (>causa` consumes it at
`silva_c89_semantica.c:1069`; the dead-suppression law is implemented
at `silva_c89_semantica.c:1084`. Read all three before writing Task 1
— this arc is the materia-side sibling, and the shape is settled.

**2. Trivia is on the token, not in the node's span.** `MateriaToken`
carries `spatia_ante` / `spatia_post` (`materia_token.h:85`). A
planted fixture measured `<conditio sedes="3:1-5:3" octeti="96-149">`
with its comment at `octeti="12-95"` — **outside** the node. So the
owner is found by walking, never by containment.

**3. `MATERIA_MUNUS_COMMENTUM` already exists** (`materia_lexicon.h:80`),
with `materia_lexicon_munus(lexicon, genus)` to read it
(`materia_lexicon.h:225`). Every client already declares which of its
lexeme genera are comments. **This is DIVERGENCE A** (below).

**4. `tools/diagnostica` already links the whole engine** —
`tools/diagnostica.sh:30-40` builds `stml`, `stml_macros`, `selectio`,
`excerptum`, all of materia, and both clients. The capability needs no
new link dependency.

**5. `MateriaTractus`** (`materia_nodus.h:183`) is
`{initium, finis, linea, columna, linea_finis, columna_finis,
fons_index, est_fons}`, `initium == -I` meaning no position.

**6. An existing token walk to match in style:** `_lexema_ambulare`,
`materia/fontes/materia_diagnostica.c:277`.

---

## Divergences from the spec, found while planning

**DIVERGENCE A — EX5's table shrinks to one field per client, and the
comment genus is not re-declared.** The spec proposed
`MateriaTabAnnotatio {genus, praefixum, clausura}` per comment genus.
But `MATERIA_MUNUS_COMMENTUM` already declares which genera are
comments, for all six clients. Only the decoration is undeclared.

**DIVERGENCE B — and the decoration is a parameter, not a baked
table.** Since exactly one string is missing, the collector takes
`praefixum` as an argument and the caller supplies it (`"#"` for
crusta). This deletes the whole coctor change: no
`<PRAEFIXUM>_ANNOTATIONES`, no regeneration of every client's tables,
no new cocta struct. **Named trigger to build the table anyway:** a
second client wanting annotations, or a client whose comments are
blocks and need `clausura` + continuation stripping. EX4's substance
is untouched — the *collection* is still generic and every client
still inherits it without writing C.

Both divergences make the arc smaller. Record them in the spec's
"As built" section at closing, as the sedes arc did.

---

## Global Constraints

- C89 in Latin via `latina.h`; house flags from `tools/vexilla.sh`.
- **`i32` and `i64` are UNSIGNED** — `s32`/`s64` for anything signed.
  `MateriaTractus.initium` is `s32` precisely because `-I` is a value.
- `chorda` is NOT NUL-terminated. Never hand `.datum` to a `str*`.
- Latina macros are forbidden identifiers (`nomen`, `casus`,
  `magnitudo`, `duplex`, `structura`, …). Single capitals are Roman
  numerals.
- New C files via `./silva/scribe.sh`, never `Write`.
- `./silva/formator.sh <viae> -scribere` then `-vitia` before commit.
- `./oratio/quaere.sh w1 w2…` on every new word BEFORE staging —
  untracked files are invisible to `vocabula.sh -nova`.
- **Every gate is born red by a plant** (`silva.planta`); a gate that
  cannot fail is DEFERRED WITH A NAMED TRIGGER, never written.
- **A fixture where two fields are EQUAL cannot discriminate them.**
- **When you change a format, run its CONSUMERS**, not just producers.
- Commits via `silva.commissio(nuntius, viae, portae)` with explicit
  paths. **Never** commit `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl` — Fran's. Never touch
  `project-specs/exempla/kalendarium.html` or `salutatio2.html`.
- Every materia writer/reader commit runs `./materia/shim_probare.sh`
  plus the client suites.
- Trailer: `Co-Authored-By: Claude Opus 5 (1M context) <noreply@anthropic.com>`

---

## Task 1: `materia_annotationes` — the collector

**Files:**
- Create: `materia/fontes/materia_annotationes.h`, `.c` (via `./silva/scribe.sh`)
- Create: `materia/probationes/probatio_materia_annotationes.c`
- Modify: `materia/compile_probationes.sh` (add the new module + probatio)

**Produces:** `MateriaAnnotatio` and
`materia_annotationes_colligere` as below; Task 2 consumes both.

- [ ] **Step 1: write the header.**

```c
nomen structura {
              chorda  textus;        /* corpus purgatum (praefixo exuto) */
              chorda  crudum;        /* octeti pleni, praefixo incluso */
      MateriaTractus  commentarium;  /* sedes COMMENTARII ipsius */
constans MateriaNodus* nodus;        /* nodus possidens; NIHIL impossibile */
      MateriaTractus  scopus;        /* sedes NODI - hic est AMBITUS */
                 b32  parsata;
          StmlNodus*  arbor;         /* elementum primum; NIHIL si fractum */
          StmlStatus  status;
                 i32  linea_erroris;   /* intra textum purgatum, I-basata */
                 i32  columna_erroris;
} MateriaAnnotatio;

/* Annotationes arboris totius: descensus nodorum, trivia ANTE
 * cuiusque lexematis, commentaria per MATERIA_MUNUS_COMMENTUM
 * inventa, praefixum exutum, stml_legere. Commentarium cuius
 * contentum primum non-spatium '<' NON est PROSA est et omittitur;
 * quod '<' fert sed parsari nequit RETINETUR (parsata FALSUM) -
 * numquam tacite prosa reclassificatum.
 *
 * 'scopus' sedes NODI POSSIDENTIS est, non commentarii: trivia extra
 * sedes nodorum iacent (mensuratum: conditio 96-149, commentarium
 * 12-95), ergo continentia possessorem invenire non potest, sed
 * descensus eum CONSTRUCTIONE novit.
 *
 * Xar de MateriaAnnotatio (per valorem), ordine fontis; NIHIL si
 * argumentum necessarium abest. */
Xar*
materia_annotationes_colligere (
                      Piscina* piscina,
        constans MateriaNodus* radix,
 constans MateriaLexiconRatum* lexicon,
           constans character* praefixum,   /* "#"; NIHIL = nullum */
    constans MateriaOrigoUncus* origo,      /* NIHIL licet */
          InternamentumChorda* intern);     /* NIHIL = pigre */
```

- [ ] **Step 2: write the probatio, and make it fail.**

Fixture (note the **deliberately different spans** — gate II of the
spec depends on the two constructs not being symmetric):

```c
constans character* FONS =
    "#!/bin/bash\n"
    "# <tolera codex=\"lint:x\" (>causa vera\n"
    "if [ -n \"$X\" -a \"$X\" -nt \"$b\" ]; then\n"
    "  echo unum\n"
    "fi\n"
    "if [ \"$B\" -nt \"$S\" ]; then echo duo; fi\n";
```

Assertions: exactly one annotation; `parsata` VERUM; the STML root tag
is `tolera`; `codex` attribute equals `lint:x`; `commentarium.initium`
lands inside line II; and **`scopus` covers the FIRST `if` and stops
before the second** — assert `scopus.finis` is strictly less than the
second `if`'s byte offset, and `scopus.linea_finis == V`.

Run: `./materia/compile_probationes.sh annotationes`
Expected: FAIL, undefined symbol.

- [ ] **Step 3: implement the walk.** Descend `MateriaNodus` by
`loci`, handling `MATERIA_VALOR_TOKEN`, `_LISTA` and `_NODUS`; **never
descend `MATERIA_VALOR_REFERENTIA`** (`materia_nodus.h:56` — the tree
would have a cycle). At each token read `spatia_ante`; for each
trivium whose `materia_lexicon_munus` is `MATERIA_MUNUS_COMMENTUM`,
strip `praefixum` and leading spaces, require the first byte to be
`'<'`, then `stml_legere`. `scopus` is the span of the node whose
locus the token sits in — available in the descent frame, computed the
way `materia_diagnostica.c` computes node ranges.

- [ ] **Step 4: run, expect PASS.** `./materia/compile_probationes.sh annotationes`

- [ ] **Step 5: plant the gate red.** `silva.planta` on the `scopus`
assignment — widen it to `nodus->pater`'s span. The "unsuppressed
sibling" assertion must go RED. If it stays green the fixture is
symmetric and must be fixed, per the constraint above. Restore.

- [ ] **Step 6: format, words, commit.**

```
./silva/formator.sh materia/fontes/materia_annotationes.c materia/fontes/materia_annotationes.h materia/probationes/probatio_materia_annotationes.c -scribere
./silva/formator.sh materia/fontes/materia_annotationes.c materia/fontes/materia_annotationes.h materia/probationes/probatio_materia_annotationes.c -vitia
./oratio/quaere.sh annotatio annotationes colligere scopus commentarium praefixum possidens purgatum
```

Commit via `silva.commissio` with `portae` = the materia suite,
`./materia/shim_probare.sh`, and **all six client suites** (a
substrate edit invalidates every client's build).

---

## Task 2: `materia_diagnostica_excusare` — the tolera semantics

**Files:**
- Modify: `materia/fontes/materia_diagnostica.h`, `.c`
- Modify: `materia/probationes/probatio_materia_diagnostica.c`

**Consumes:** `MateriaAnnotatio`, `materia_annotationes_colligere`.
**Produces:** `materia_diagnostica_excusare`, consumed by Task 3.

- [ ] **Step 1: declare.**

```c
#define MATERIA_CODEX_EXCUSATIO_MORTUA  "materia:excusatio-mortua"
#define MATERIA_CODEX_EXCUSATIO_SINE_CAUSA "materia:excusatio-sine-causa"
#define MATERIA_CODEX_EXCUSATIO_FRACTA  "materia:excusatio-fracta"

/* Diagnostica per annotationes <tolera codex= (>causa excusare.
 * Inventum excusatur si sedes PRIMARIA eius intra 'scopus'
 * annotationis iacet ET codices AEQUALES sunt (sedes relatae non
 * participant: inventum uno loco est, SM1).
 *
 * Excusatio MORTUA VITIUM EST: quae nihil absorbuit, aut sine causa
 * est, aut parsari nequit, diagnosticum SUUM parit, super
 * commentario positum. Codices 'lint:' ignoti NON iudicantur -
 * registrum eos nondum novit (EX8; signum = cursor regularum).
 *
 * Xar NOVUM reddit; argumentum non mutatur. */
Xar*
materia_diagnostica_excusare (
              Piscina* piscina,
          constans Xar* diagnostica,
          constans Xar* annotationes,
constans MateriaDiagnosticaCocta* declarata);  /* NIHIL = nulla validatio codicum */
```

- [ ] **Step 2: write five failing assertions**, one per behaviour:
suppressed finding drops; **sibling outside `scopus` survives**; dead
tolera reported; causeless tolera reported AND does not suppress;
unparseable annotation reported. Build the diagnostica list by hand
(no parse needed) so the test isolates the join.

Run: `./materia/compile_probationes.sh diagnostica` — expect FAIL.

- [ ] **Step 3: implement.** For each annotation whose `arbor` tag is
`tolera`: read `codex` via `stml_attributum_capere`, read the cause
via `stml_textus_normalizatus` and require a non-space byte (the
`silva_c89_semantica.c:1204` loop is the model). Mark absorbed.
Then emit a diagnostic per unabsorbed / causeless / unparsed
annotation, `gravitas` = `MATERIA_GRAVITAS_ERRATUM`, `tractus` =
`commentarium`.

- [ ] **Step 4: run, expect PASS.**

- [ ] **Step 5: plant each of the five red in turn.** Specifically:
delete the codex comparison (assertion 1 must go red), widen
containment to `<=` on the wrong end (assertion 2), skip the absorbed
flag (3), accept an empty cause (4), treat unparsed as prose (5).
**Five plants, five reds** — a plant that leaves everything green
means that assertion is measuring nothing.

- [ ] **Step 6: format, words, commit** with the same gate set as
Task 1.

---

## Task 3: the instrument honours exemptions

**Files:**
- Modify: `tools/diagnostica.c`
- Modify: `tools/diagnostica_fumus.sh` (gates VII–XI)

**Consumes:** `materia_diagnostica_excusare`.

- [ ] **Step 1: extend both paths.** In the derive path, collect
annotations from the tree already in hand. In `-lege`, the rows name
files — parse each distinct `via` once (suffix picks the client, as
`main` already does) and collect from that tree. Suppression runs
**before** the `Summa` counters are incremented, so the exit code
follows.

- [ ] **Step 2: add the fumus gates**, each with the plant named:

| gate | asserts | plant |
|---|---|---|
| VII | annotated file exits 0, unannotated exits 1 | filter after the counters |
| VIII | tier-1 codex exempts (`crusta:grex/tok_clausura`) | restrict matching to `lint:` |
| IX | dead tolera is itself an erratum | skip the absorbed check |
| X | `-lege` honours exemptions identically to the derive path | collect only in the derive path |
| XI | round trip still byte-identical with a tolera present | — (regression guard) |

Gate X is the one that matters: it is the **consumer** check the
adaptator arc learned the hard way.

- [ ] **Step 3: run the fumus, confirm each new gate is RED before
implementing it, green after.**

Run: `./tools/diagnostica_fumus.sh`

- [ ] **Step 4: commit**, gates = `tools/diagnostica_fumus.sh` +
materia suite + all six client suites.

---

## Task 4: rule 1 (`-nt`) — the pattern

**Files:**
- Create: `crusta/lintrum/nt-aequalitas.stml`

**Naming, already checked:** `lintra` (the natural plural) returns
**0 analyses** from `./oratio/quaere.sh` and would be refused at
commit; `lintrum` resolves. `regulae` also resolves but collides with
`CRUSTA_LEX_REGULA` (`crusta_lexicon.h`), a bash pattern lexeme — a
directory of lint rules next to a lexeme genus of the same name is a
reader trap. Hence the singular.
- Modify: `pythonica/probatio_silva.py` (pin the candidate count)

- [ ] **Step 1: read the corpus before writing the pattern.** The spec
(§6) gives the `probatio`-rooted form for the `if` case only. `-nt`
also appears under `while`, under `&&`, and as a bare statement. Run
`./crusta/arbor.sh` over three real sites of different shapes and find
the scope element that contains both the `-nt` and a possible `pipa`.
**Do not guess this** — the census error on this ticket was exactly a
guess about where `!` lives.

- [ ] **Step 2: write the rule**, rooted at whatever step 1 found:

```xml
<EXEMPLAR output="$p"><SCOPUS><**>
  <crusta-litteralis $t>-nt</crusta-litteralis></**></SCOPUS></EXEMPLAR>
<CATENA de="$p" output="$cand"><SINE>
  <pipa><praefixa><crusta-reservatum>!</crusta-reservatum></praefixa></pipa>
</SINE></CATENA>
<relatum lint="nt-aequalitas" gravitas="erratum"
         causa="'-nt' aequalitatem perdit: in eodem secundo opus omittitur">
  <PER congruentia="$cand"><situs>&@t;</situs></PER></relatum>
```

- [ ] **Step 3: measure.** Re-run A and B (289 → 291 last arc; may have
grown again). Assert `A - B == len(candidates)` — the arithmetic must
close, as it did at 289−260 = 29 and 291−262 = 29.

- [ ] **Step 4: pin the count** in `probatio_silva.py`, with the date
and the measured value in the message so a future drift is legible.

- [ ] **Step 5: commit.**

---

## Task 5: the rollout

**Files:** the ~29 sites named on `01M2PRPGBC`.

- [ ] **Step 1: fix the defective sites.** `[ "$BIN" -nt "$SRC" ]` →
`! [ "$SRC" -nt "$BIN" ]`. The worked example with its reason is at
`tools/diagnostica.sh:22-23`. Named on the ticket:
`tools/canon_coquere.sh:35`, `tools/natura_canones.sh:84`,
`tools/natura_glossae.sh:29`,
`tools/natura_metamodulus_generare.sh:28`, `tools/natura_visus.sh:65`,
`tools/silva_canon.sh:94`, `tools/silex_struere.sh:49-53` (three),
`officina/captare.sh:169`, `gesta/fori.sh:44`, and
`tools/briar_facies_capsula.sh:23` to inspect.

- [ ] **Step 2: annotate the legitimate sites**, each with a real
reason, not a restatement of the rule. "caput recentissimum computat;
aequalitas nihil refert" is a reason. "tolera nt" is not.

- [ ] **Step 3: answer AUDIENDUM 10.1 with the evidence just
gathered.** While annotating, record whether the natural site was the
line above (owning-node scope works) or the end of the line (trailing,
and the scope came out too narrow). **This is the measurement the spec
deferred the decision to.** Write the answer into the spec's "As
built" section — do not leave it open.

- [ ] **Step 4: run the rule; expect zero findings.** Every candidate
is now either fixed or annotated. **Then plant one:** remove one
annotation and confirm the rule goes red. A rule that reports nothing
because everything is annotated looks identical to a rule that reports
nothing because it is broken.

- [ ] **Step 5: commit**, gates = the full root suite (these are
scripts the whole house runs).

---

## Closing

- [ ] Spec "As built" section: DIVERGENCE A and B, the AUDIENDUM 10.1
  answer, and any gate whose plant behaved unexpectedly.
- [ ] `01M2SD93PV` → `impletum`, with the measured numbers.
- [ ] `01M2PRPGBC`: rule 1 struck from the open list; note whether
  gaps (b) and (c) moved.
- [ ] Memory `diagnostica-next-arc.md`: step 3 status, and the
  annotation capability as a thing that now exists on BOTH sides
  (silva and materia).
- [ ] Phase-log RELATIO with the instrument debrief.
