# materia sedes — positions in the projection, diagnostics from declarations

*2026-09-17. Designed with Fran the day after crusta's plan closed.
Origin: desideratum `01M2PSWHM0` ("a line number thing for materia is
a great idea"), itself born when `silva.exemplaria` (pythonica
`e7802dd4`) returned match rows without lines and the crusta lint
survey (nota `01M2PRPGBC`) had to grep for them. Fran widened it the
same day: "polish the quality" of line, column and substring results
"with all errors", once, in materia, so every client benefits. Two
parts, one spec (Fran): **A** the positioned view, **B** diagnostics.*

**What it is.** Part A gives every materia client an opt-in STML
projection whose elements carry their source range, so a pattern
rule's rows say where they matched; it lands with a fix to the STML
engine without which positions would break equality captures. Part B
lets a client DECLARE in its registry what counts as a problem (a bad
node, a missing closer, an empty list) and has one materia walker
derive located diagnostics from any tree, printed in the compiler
format with a source excerpt by one instrument. The first consumers
are `silva.exemplaria` (A) and the crusta lint that follows this plan;
crusta, css and the materia writer are B's first diagnostics.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| S1 | **Two parts, one spec: A the positioned view, B diagnostics** | Fran, 2026-09-17. They share the range function (§2) and the order of work runs A then B. |
| S2 | **Positions live in a VIEW, never in the canonical document**; the envelope says `visio`, the reader refuses it, canons never judge it | The canonical document carries one anchor and the reader derives the rest (§1.2): "the document cannot lie". Rejected: positions in the document with a verifying reader (every client canon declares them, a second source of truth); a side table (expansion zeroes byte extents, §1.3 — measured dead). VISIO precedent (`loci_admissi`). |
| S3 | **Attribute shape `sedes="12:5-12:9"` + `octeti="340-344"`**: 1-based lines, 1-based BYTE columns, end exclusive | Fran approved the compact form. The `[initium, finis)` convention of `MateriaArborSedes`. Names distinct from the envelope's `b`/`linea`/`columna`, which mean where EMISSION begins (leading trivia included). |
| S4 | **Stamped on every node element and every token element, trivia included; never on locus wrappers**; a node's range runs from its first source token to its last | A wrapper (`<corpus>`, `<liberi>`) is a slot, not source. Trivia are tokens hung off tokens, so node ranges exclude leading comments for free, and a comment lint still gets positions. |
| S5 | **Repeated captures compare structurally under TRANSPARENTIA** (tags and attributes), not by serialized bytes | Measured (§1.3): a position attribute makes two captures of the same identifier unequal even with `TRANSPARENTIA attributa`. The reference §19.4 already promises the listed attributes are ignored "on both sides". |
| S6 | **B covers crusta, css and the materia writer**; html's parse errors are a named later tranche | Fran, 2026-09-17 (option chosen over "also html now" and "crusta only"). |
| S7 | **Diagnostics are DECLARED in the registry and DERIVED by one materia walker**; a parser emits directly only a class that leaves no trace in the tree | Fran, 2026-09-17. M7 (a table, never a program); works on parsed trees, trees read back from STML, and views. |
| S8 | **Declarations bake into a SEPARATE generated table**, not new fields of `MateriaTabLocus`/`MateriaTabGenus` | New struct fields would put every hand-written registry table (briar, the C89 shim, oratio_partes, probationes) under `-Wmissing-field-initializers`. The seal hashes names, offsets and species only (§1.6), so diagnostics move no seal. |
| S9 | **crusta keeps its four counters as the independent oracle** for its derived diagnostics | Two implementations over disjoint code cover each other; agreement is gated class by class on every case. |
| S10 | **One instrument `./tools/diagnostica.sh` routing by suffix + `silva.diagnostica`** | Fran, 2026-09-17 (over per-client flags and LSP now). In `tools/`, so materia never links its clients. |
| S11 | **Bytes stay the substrate's column unit**; character and UTF-16 columns are converted at the edges | The excerpt printer counts UTF-8 characters for its underline only; the LSP conversion is a named future. |

---

## 1. What exists (measured 2026-09-17)

### 1.1 The data is already there

Every token carries `byte_offset`, `linea`, `columna`
(`materia/fontes/materia_token.h:90-93`), and every client fills them:
`css/fontes/css_adaptare.c:69`, `html/fontes/html_adaptare.c:54`,
`crusta/fontes/crusta_lector.c:205`, `md/fontes/md_lexema.c:1019`,
`oratio/fontes/oratio_lexema.c:197`. The FIDELITAS comparator checks
them against the reader's derivation in crusta, md and oratio
(`MATERIA_ARBOR_COMPARATIO_FIDELITAS` in their STML probationes).

### 1.2 The canonical document carries an anchor only

- The envelope gets `fons`, `b`, `linea`, `columna`, `linea-initium`
  of the first EMITTED byte (`materia/fontes/materia_arbor.c:1966-1983`).
- A lexeme element gets `b`/`linea`/`columna` only when its token is
  DERIVED (C89 macro expansion), because its bytes are not in this
  stream (`materia_arbor.c:1348-1372`).
- The reader derives every other position by walking the bytes
  (`materia_arbor.h:382-386`).
- VISIO: `MateriaArborConsilium.loci_admissi` (`materia_arbor.h:301-309`)
  stamps `visio="partialis"` (`materia_arbor.c:1923-1964`); the reader
  refuses ANY `visio` attribute (`materia_arbor.c:3643-3646`, message
  "visio partialis: non arbor").
- The writer can collect element pairs per node and token
  (`sedes_colligere`, `ArborPar`, `materia_arbor.c:29-33`, `1235-1247`,
  `1682-1692`).

**Correction to the desideratum.** silva does not stamp positions on
every lexeme: only top-level declarations (fragment anchors, e.g.
`<definitio-functionis b="0" linea="1" columna="1" linea-initium>`
from `./silva/arbor.sh`) and derived tokens. Its lint rules hide
`f b linea columna def-f def-l def-c`
(`silva/probationes/fixa/exemplaria/inutilis_localis.stml:1`), and its
rows carry no lines either: the gate counts `situs` elements
(`silva/probationes/probatio_silva_exemplaria_lint.c:572`).

### 1.3 The pattern engine and positions

- `stml_expandere` clones matched nodes and zeroes `positus`; `linea`
  (the STML document's line) survives (`project-specs/stml-exemplaria-spec.md`
  §1.3). A side table from rows back to tree nodes is therefore dead.
- Probes with `stml expandere` (scratch, 2026-09-17):
  - an attribute capture `<x sedes="$s">$t</x>` reaches the row
    (`<r sedes="&@s;">` prints `sedes="7:3-7:9"`);
  - a repeated NODE capture `<x $n/> … <x $n/>` over two `x` equal but
    for `sedes` does NOT match, with or without
    `TRANSPARENTIA attributa="sedes"`; without the attributes it does;
  - a repeated TEXT capture `<x>$n</x> … <x>$n</x>` matches.
- Cause: `_ligamen_ponere` (`lib/stml_macros.c:3652-3657`) and
  `_congruentia_silvae_aequales` (`lib/stml_macros.c:7381-7410`)
  compare `stml_scribere` bytes of both forests. The same comparison
  also sees trivia wrappers (`ante`/`post`) that TRANSPARENTIA tags hide
  from matching.

### 1.4 Diagnostics today

- **crusta** counts, never locates (`CrustaParsura`,
  `crusta/fontes/crusta_arbor.h`):
  - `mala`, from three sources: `malum` nodes
    (`crusta_arbor.c:1246-1257`), implicit operators in `[[ ]]`
    (`crusta_arbor.c:3105`, `3245`), and the arithmetic machine
    (`crusta_arithmetica.c:89-547`, summed at `crusta_arbor.c:602`);
  - `clausurae_absentes`: `_claudere` through the table
    `_locus_clausurae` (`crusta_arbor.c:765-770`) and an unterminated
    heredoc (`crusta_arbor.c:3662`);
  - `listae_vacuae`: `_vacuas_numerare` (`crusta_arbor.c:680-733`),
    conditional on the `elif`/`else` shape of `ramus`;
  - `heredoca_transposita`: a body appended out of byte order
    (`crusta_arbor.c:3571`).
- **css**: unparseable input becomes bad-node genera `regula-mala`,
  `declaratio-mala`, `selector-malus`
  (`css/grammatica/css.registrum.stml:61`, `66`, `121`).
- **html, css, md** parse entry points return a tree only
  (`html/fontes/html_arbor.h:45`, `css/fontes/css_arbor.h:34`,
  `md/fontes/md_arbor.h:34`). html's parser cites the spec's
  "parse error" points in comments only.
- **materia writer** refusal: `causa` + `MateriaNodus* sedes`, no source
  position (`materia_arbor.h:84-98`). **Reader** refusal: `causa` +
  the STML document's line (`materia_arbor.h:354-357`).
- **Format precedent**: `./silva/examen.sh` prints
  `via:linea:columna: [violatio] causa`. No house code renders a
  source excerpt with an underline.

### 1.5 Oracles for error positions already on disk

- `probationes/fixa/crusta/oraculum/sanitas.txt`: 121 `bash -n` blocks,
  11 with an error text carrying a line
  (`bash: -c: line 1: syntax error near unexpected token `fi'`).
- `probationes/fixa/html/html5lib/*.dat`: 1,625 `#errors` sections,
  2,780 positioned lines (`(1,3): expected-doctype-but-got-chars`);
  html5lib-python's own names, not the spec's (html, later).

### 1.6 The registry seal

`materia_arbor_sigillum` (`materia_arbor.c:109-180`) hashes each
genus's titulus, loci offset and count, and each locus's titulus and
species. Nothing else.

---

# PART A — THE POSITIONED VIEW

## 2. The range (shared by A and B)

```c
/* materia_nodus.h, beside MateriaSedes */
nomen structura {
    s32 initium;         /* primus octetus fontis; -I = nullum */
    s32 finis;           /* post octetum ultimum (EXCLUSIVUS) */
    i32 linea;           /* initii, I-basata */
    i32 columna;         /* initii, I-basata, octetis */
    i32 linea_finis;
    i32 columna_finis;   /* post octetum ultimum */
    s32 fons_index;
} MateriaTractus;

/* Tractus lexematis: sedes efficax (materia_sedes_tokeni) + valor. */
vacuum
materia_tractus_lexematis (
    constans MateriaOrigoUncus* uncus,
        constans MateriaToken* token,
             MateriaTractus* tractus);

/* Tractus nodi: a lexemate fontis primo ad ultimum (ordine locorum,
 * listae ordine, REFERENTIA non descensa). FALSUM = nullum lexema
 * fontis (tractus.initium -I). */
b32
materia_tractus_nodi (
    constans MateriaOrigoUncus* uncus,
        constans MateriaNodus* nodus,
             MateriaTractus* tractus);
```

- **Start** = the effective position (`materia_sedes_tokeni`), so a
  derived token reports its use site once C89 runs through materia.
- **End** = start advanced over the value's bytes by the reader's
  cursor rule (`_cursorem_promovere`, `materia_arbor.c:3273`): a `\n`
  starts a new line at column 1; every other byte adds one column. For
  every client today the value IS the source bytes; a frons with
  laminae (C89) moves the cursor itself (`cursorem_movere`), wired in
  phase 5.
- **Synthetic tokens** (`byte_offset` −1) are skipped by the node rule
  and give `initium` −1 alone.
- **Trivia** are not node loci, so a node's range never includes its
  leading comment.
- **Fontes**: a node's range takes the first source token's fons and
  skips tokens of other fontes (only C89 includes produce them, phase 5).
- The writer computes node ranges bottom-up inside its one walk; the
  public function walks the subtree for callers outside the writer
  (B's derivation, writer refusals).

## 3. The view

**Consilium.** `MateriaArborConsilium.sedes_scribere` (`b32`), the LAST
field, so existing positional initializers keep compiling;
`materia_arbor_consilium_nudum` sets FALSUM.

**What is stamped** when it is VERUM:

| element | attributes | order |
|---|---|---|
| node element | `sedes`, `octeti` from `materia_tractus_nodi`; none if FALSUM | after `id` and the frons's `nodum_ornare` attributes |
| token element (loci and lists) | `sedes`, `octeti` from `materia_tractus_lexematis`; none if synthetic | after the existing attributes (`f`, frons, `cr`, `crlf`, derived `b`/`linea`/`columna`) |
| trivium element inside `ante`/`post` | the same | after existing attributes |
| locus wrapper, `ante`/`post`, envelope | nothing | — |

- Values: `sedes="L:C-L:C"` (a multi-line token reads `3:1-7:2`, an
  empty token `4:1-4:1`), `octeti="B-B"`; decimal, no spaces.
- A token shared by several parents is written once as a fragment
  (`<#lexN>`); its range sits in the fragment's content. Whether rows
  matched through `<<#lexN>>` aliases see it is AUDIENDA I.
- **Envelope**: `visio="sedes"`; with `loci_admissi` too,
  `visio="partialis sedes"` (fixed order). The reader already refuses
  any `visio`; its message becomes `"visio (…): non arbor"` naming the
  value.
- **Arbor tradita**: `materia_arbor_proicere_nodum` stamps the same
  attributes in the in-memory tree (values in `consilium->intern`, the
  intern law of `materia/CLAUDE.md`).
- **Canon**: views are never judged; client canons are untouched.
- **The normal projection is byte-identical** to today's.

## 4. The engine fix: equality under TRANSPARENTIA

`_ligamen_ponere` and `_congruentia_silvae_aequales` stop comparing
serialized bytes and compare structurally:

- elements: same interned tag; the same attributes in the same order
  after removing TRANSPARENTIA `attributa`; children pairwise after
  removing TRANSPARENTIA `tags` subtrees whole;
- text and raw leaves: equal bytes;
- with no TRANSPARENTIA declared the result equals today's byte
  comparison (the minimal semantic change: removal of what the
  document already declared invisible).

Reference §19.2's row "forests compared byte-wise" becomes "forests
compared structurally, TRANSPARENTIA removed". Before and after, the
tranche runs every rule corpus and names every row that changes:
silva's `probatio_silva_exemplaria_*` rules, the stml macro
probationes (vitia XII–XXV), oratio's resolution rules, and the crusta
survey rules of nota `01M2PRPGBC`. Oratio's resolution time is
measured before and after (it compares on every resolution pass).

## 5. Surfaces (A)

- **Instruments**: `-sedes` on `crusta/arbor.sh`, `html/arbor.sh`,
  `md/arbor.sh`, `oratio/arbor.sh`. css has no `arbor.sh`; briar's
  lives in the `../rhubarb-briar` worktree (named future, §14).
- **pythonica**:
  - `silva.arbor(via, nudum=False, sedes=False)`; `sedes` for silva (C)
    raises, as `nudum` does for clients.
  - `silva.exemplaria` asks for the view for materia clients; its
    default prelude becomes
    `<TRANSPARENTIA tags="ante post" attributa="sedes octeti"/>`.
  - A rule that declares its own TRANSPARENTIA without
    `attributa` listing `sedes` and `octeti` is REFUSED with that cause
    (its repeated captures would silently never match).
  - `Congruentia` gains appended fields `linea`, `columna`,
    `textus_fontis`: from the row element's own `sedes` if the rule
    wrote one, else the first descendant carrying `sedes`;
    `textus_fontis` = the file's bytes at `octeti`. No position →
    `None`, and the per-file count of positionless rows is returned.

## 6. Gates (A)

Every gate born red by a planted fault.

- **materia** (`probatio_materia_sedes`, materia's test registry):
  range of a single-line token, a multi-line token, a CRLF token, a
  synthetic token; node ranges skipping synthetic tokens, excluding
  trivia, with no tokens; the view stamps; `visio` on the envelope, and
  both forms with `loci_admissi`; reader refusal naming the value;
  normal projection unchanged.
- **every client** (crusta, html, md, oratio STML gates, over their
  corpora): for every token with a source position, its element's
  `octeti` equals `byte_offset` to `byte_offset + valor.mensura`, the
  source slice equals `valor`, and `sedes` equals what a line table
  built by the GATE from the source says; for every node element,
  `octeti` equals the min/max the gate computes by its own walk; the
  reader refuses the view. css through its canon/arbor probatio.
  Nothing typed by hand; synthetic tokens counted and printed.
- **stml engine**: new cases in the stml macro probationes — repeated
  node capture under `attributa` and under `tags` matches; without
  TRANSPARENTIA still byte-sensitive; the row deltas of §4 recorded in
  the worklog.
- **pythonica** (`probatio_silva.py`): `arbor(sedes=True)` carries
  `visio="sedes"`; an exemplaria row on a fixture `.sh` carries
  `linea`, `columna` and `textus_fontis`; a rule with a TRANSPARENTIA
  lacking the attributes is refused.
- **Size**: the view's growth over the house `.sh` corpus measured and
  recorded in the worklog, not pinned.

---

# PART B — DIAGNOSTICS

## 7. Declarations

In `<cliens>/grammatica/<cliens>.registrum.stml`:

| where | attribute | meaning |
|---|---|---|
| `<genus>` | `diagnosticum="causa"` | every node of this genus is a diagnostic |
| `<locus>` | `absentia="causa"` | this locus unwritten is a diagnostic |
| `<locus>` (list species) | `vacua="causa"` | this locus written as a list of zero elements is a diagnostic |
| same element | `gravitas="monitum"` | severity (absent = `erratum`) |

- `causa` is short Latin, without a position (positions are the
  record's).
- **Codex** = `genus` or `genus/locus` (`malum`, `grex/tok_clausura`,
  `grex/liberi`); instruments print `<grammatica>:<codex>`.
- `materia/grammatica/registrum.canon` accepts the four attributes
  (`diagnosticum` on `<genus>`, `absentia`/`vacua` on `<locus>`,
  `gravitas` on both as an `electio` of `erratum|monitum`). "`vacua`
  only on a list species" depends on another attribute's value, which a
  canon cannot say (no expression language, ever): the coctor refuses
  it by name.
- `materia_coctor` / `./materia/coquere.sh` bakes a second table into
  the same `_registrum_coctum.{h,c}`:

```c
/* materia_registrum.h - lex manet: nihil praeter latina.h */
nomen enumeratio {
    MATERIA_DIAGNOSTICUM_GENUS = 0,   /* nodus generis ipse */
    MATERIA_DIAGNOSTICUM_ABSENTIA,    /* locus non scriptus */
    MATERIA_DIAGNOSTICUM_VACUA        /* lista scripta sine elementis */
} MateriaDiagnosticumSpecies;

nomen enumeratio {
    MATERIA_GRAVITAS_ERRATUM = 0,
    MATERIA_GRAVITAS_MONITUM
} MateriaGravitas;

nomen structura {
                   s32  genus;     /* index generis */
                   s32  locus;     /* intra genus; -I pro GENUS */
                   s32  species;   /* MateriaDiagnosticumSpecies */
                   s32  gravitas;  /* MateriaGravitas */
    constans character* codex;     /* "grex/tok_clausura" */
    constans character* causa;
} MateriaTabDiagnosticum;

nomen structura {
    constans MateriaTabDiagnosticum* tabula;
                                i32  numerus;
} MateriaDiagnosticaCocta;
```

- A client with no declarations gets an empty table (`numerus` 0), so
  every client regenerates once; `materia_registrum_recens` already
  gates the whole generated file.
- **Law**: diagnostics are judgments over a tree, not its shape. The
  seal does not hash the table (§1.6); adding a declaration moves no
  seal and no document.

## 8. Derivation

```c
/* materia/fontes/materia_diagnostica.h (novum) */
nomen structura {
                       s32  gravitas;
       constans character*  codex;
       constans character*  causa;
    constans MateriaNodus*  nodus;    /* NIHIL pro ordine rupto */
    constans MateriaToken*  lexema;   /* ordo ruptus solum */
            MateriaTractus  tractus;
} MateriaDiagnosticum;

/* Xar de MateriaDiagnosticum, ordine (tractus.initium, codex).
 * NIHIL = memoria deficit; Xar vacuum = sanum. */
Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* registrum,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus);
```

**Where each diagnostic points:**

| species | range |
|---|---|
| GENUS | the node's range |
| ABSENTIA | a zero-width point at the node's range end ("expected `}`" after the last token) |
| VACUA | the node's range |
| node with no source token | a zero-width point after the previous source token in walk order (file start if none) |

**The generic check** every client gets, no declaration needed: codex
`materia:ordo-octetorum`, erratum — a source token whose `byte_offset`
lies below the greatest end already seen in the same fons, in the byte
emitter's walk order (loci in order, REFERENTIA not descended). Range =
that token. For crusta this is `heredoca_transposita`.

**Writer refusals** get a place: `MateriaArborScriptura` gains a LAST
field `MateriaTractus tractus` filled by `materia_tractus_nodi` from
the refused `sedes` node (`initium` −1 when there is none). Reader
refusals keep the STML line; instruments print them against the
`.stml` path.

The walk has the writer's depth limits (`01M1FAD8`), no worse.

## 9. crusta and css

**crusta's declarations**, each measured against its counter before it
is kept:

- `malum` → `diagnosticum`;
- the closing loci of `_locus_clausurae` → `absentia`; the heredoc
  delimiter end → `absentia` with the severity bash gives it
  (AUDIENDA IV);
- the list loci of `_vacuas_numerare` → `vacua`, if written-but-empty
  expresses the `elif`/`else` condition (AUDIENDA III);
- the implicit operators of `[[ ]]` and the arithmetic machine's mala
  → `absentia` on the operator locus if they leave it unwritten,
  otherwise parser-emitted and named here at build time (AUDIENDA II);
- `heredoca_transposita` → the generic order check.

**Agreement gate** (`probatio_crusta_diagnostica`): on the house
corpus, the FreeBSD fixtures, the oracle cases and every one of the
fuzz gate's generated cases, per class, the derived diagnostics equal
the counter (mala, clausurae_absentes, listae_vacuae,
heredoca_transposita), and `sana` ⇔ zero `erratum`. No named
exemptions at birth (P9's lesson: an exemption hides the next bug).

**bash's lines** (`probatio_crusta_differentia`): for each sanitas
block whose error text names `line N`, compare the first erratum's
line with N; print every disagreement; pin agreements RISING. EOF
errors are where bash and a node-end point may differ, so equality is
not promised.

**css**: `diagnosticum` on `regula-mala`, `declaratio-mala`,
`selector-malus`; gate: diagnostics equal bad nodes over the css corpus,
plus a few small positioned fixtures.

## 10. The excerpt printer

```c
/* lib/excerptum.h */
b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea);
```

```
   41 |   if [ -n "$x" ]; then {
      |                        ^~~
```

- The line containing `initium`, CR removed; gutter width = digits of
  `linea`, at least 4.
- Pad under the text: a tab stays a tab, a UTF-8 continuation byte
  adds nothing, any other byte adds a space.
- `^` at `initium`, then `~` per UTF-8 character up to `finis` or the
  end of that line (a multi-line range underlines its first line only).
- Zero width: `^` alone. `initium` on the newline or at EOF: the caret
  after the last character. Control bytes other than tab print as a
  space.
- `probationes/probatio_excerptum.c`: tabs, UTF-8, no final newline,
  zero width at EOF, multi-line range, CRLF.

## 11. Surfaces (B)

- **`./tools/diagnostica.sh <viae…> [-machina] [-sine-excerpto]`**
  (C main placed in the plan; `tools/aedilis.c` precedent). Routes
  `.sh` → crusta, `.css` → css; another suffix is refused by name.
  Per file: parse, derive, and project once (normal view) so writer
  refusals surface as `materia:scriptura`.
  - Human form: `via:linea:columna: [erratum] crusta:grex/tok_clausura`,
    the cause, the excerpt; summary on stderr
    `N diagnostica (erratum E, monitum M) in F plagulis`.
  - `-machina`: TSV `via linea columna linea_finis columna_finis
    gravitas codex causa`.
  - Exit 0 = no erratum (monita allowed), 1 = at least one erratum,
    2 = nothing ran (no file judged; the house law).
- **pythonica**: `silva.diagnostica(viae)` (a path, a list, or a git
  glob like `exemplaria`) through `-machina` →
  `[Diagnosticum(via, linea, columna, linea_finis, columna_finis,
  gravitas, codex, causa, textus)]`, `textus` = the source slice.

## 12. Gates (B)

- **coctor/canon**: the registry canon refuses an unknown `gravitas`
  and `absentia` on a genus; the coctor refuses `vacua` on a non-list
  locus; every client's generated file regenerated and gated; every
  seal unchanged (asserted).
- **materia** (`probatio_materia_diagnostica`, test registry): each
  species, the no-token point rule, the order check, sorting, the
  writer refusal's range (a planted NUL in a token value).
- **crusta**: the agreement gate and the bash line pin (§9).
- **css**: §9.
- **printer**: §10.
- **instrument** (`tools/diagnostica_fumus.sh`, a fumus like the
  others): the house `.sh` corpus → zero erratum, exit 0; a planted
  file → exact output and exit 1; an unknown suffix → exit 2 by name.
- **pythonica**: `silva.diagnostica` on the planted file returns the
  record with its `textus`.

---

## 13. Order of work

| tranche | content |
|---|---|
| A1 | `MateriaTractus` + range functions; `sedes_scribere` in the writer; `visio` envelope; reader message; materia gate |
| A2 | client STML gates over their corpora; `-sedes` on four instruments |
| A3 | engine equality under TRANSPARENTIA; row deltas + oratio timing |
| A4 | pythonica `arbor(sedes)`, exemplaria rows, TRANSPARENTIA refusal |
| B1 | declaration attributes + registry canon + coctor table; all clients regenerate; seals asserted |
| B2 | `materia_diagnostica_derivare`, order check, writer refusal range; materia gate |
| B3 | crusta declarations measured class by class; agreement gate; bash line pin |
| B4 | css declarations + gate |
| B5 | excerpt printer; `tools/diagnostica.sh` + fumus; `silva.diagnostica` |
| — | closure: phase-log RELATIO, `materia/CLAUDE.md`, desideratum `01M2PSWHM0` fulfilled, futures filed |

The phase-log INTENTIO is filed with the plan commit (crusta's lesson:
its INTENTIO never was). Every materia writer/reader commit runs
`./materia/shim_probare.sh` and all client suites.

## 14. Named futures

- **The crusta lint** as rule documents over `silva.exemplaria` — the
  next plan and the first consumer of A's rows.
- **html parse errors**: the parser emits at its spec "parse error"
  sites (no tree trace); positions pinned rising against html5lib's
  2,780 errors.
- **LSP**: legatus publishes these diagnostics for `.sh` and `.css`.
- **Character and UTF-16 columns** at the edges (editors, LSP).
- **Diagnostics in the view** as elements, so rules can query them.
- **briar**: `-sedes` on `briar/arbor.sh` (projects through materia,
  `briar/instrumenta/arbor.c:115`) once its worktree is level.
- **C89 through materia** (phase 5): derived-token ranges through
  `extentum_quaerere`, laminae through `cursorem_movere`, multi-fons
  views.
- md and oratio have no parse errors by definition (CommonMark accepts
  every input; oratio's findings are lint).

## 15. AUDIENDA (not verified when written)

- I. Whether a row matched through a `<<#lexN>>` alias carries the
  fragment content's `sedes` (§19.3 says aliases resolve to fragment
  content; not probed). A1 probes it.
- II. Whether `[[ ]]` implicit operators and the arithmetic machine's
  mala leave an unwritten operator locus (declarable) or not. B3.
- III. Whether `_vacuas_numerare`'s `elif`/`else` condition equals
  "written with zero elements". B3.
- IV. `bash -n`'s exit status and message class for an unterminated
  heredoc, which sets that declaration's `gravitas`. B3.
- V. Whether `heredoca_transposita` equals the generic order check on
  every fuzz case. B3.
- VI. The engine fix's row deltas and oratio's resolution time are
  unmeasured. A3.
- VII. The view's size growth is unmeasured. A2.
- VIII. `-Wmissing-field-initializers` on the hand tables (S8) is
  asserted from the house flag set, not compiled.
- IX. html's parse-error sites were counted by grep (63 lines of
  `html_arbor.c` mention errors), not audited.
