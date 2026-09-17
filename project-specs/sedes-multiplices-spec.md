# sedes multiplices — a diagnostic that points at two places

*2026-09-17, the evening the materia-sedes arc closed at `d6d0f752`.
Origin: desideratum `01M2RKYA4Z`, filed hours earlier out of Fran's
question about peer libraries for `excerptum`, and ordered FIRST by
decree `01M2RNK5MG` — before the `Congruentia` adapter (`01M2RNJ9XN`)
and before any lint rule — because both of those change
`MateriaDiagnosticum` and an adapter written against the one-span
record gets written twice. Fran's reason for the whole line of work,
which the tickets alone do not convey: diagnostics are the agent's
interface to a parser, so the gap between `relatio.mala == 2` and
`file:9:1: [erratum] crusta:grex/tok_clausura` plus an excerpt is the
gap between knowing something is wrong and knowing where to look.*

**What it is.** `MateriaDiagnosticum` carries one `MateriaTractus`, so
the shape clang and rustc use constantly cannot be expressed:

```
error: '}' exspectata ut hoc '{' claudat
   3 | if true; then
     | ^~~~~~~~~~~~~ hic coepit
   9 |
     | ^ hic exspectatur
```

This spec gives the record a list of related spans, teaches the
printer to draw them in one block, and fills the list for free in the
one case that already has both positions in hand. It adds **no
registry vocabulary**, moves **no seal**, and leaves the STML
projection untouched.

**The measurement that makes it cheap.** The second span is already
computed and thrown away. `materia/fontes/materia_diagnostica.c:300`,
the `absentia` arm:

```c
casus MATERIA_DIAGNOSTICUM_ABSENTIA:
    valor = &nodus->loci[r->locus];
    si (   valor->genus == MATERIA_VALOR_NIHIL
        || (   valor->genus == MATERIA_VALOR_REFERENTIA
            && valor->datum.nodus == NIHIL))
    {
        finis = habet ? _punctum_finis(&t) : punctum;
        _addere(d, r->gravitas, r->codex, r->causa, nodus,
            NIHIL, &finis);
    }
    frange;
```

`t` is the node's own accumulated range — the opening construct. The
walker holds it, takes its endpoint, and lets it fall off the stack.
Measured 2026-09-17: **28 of crusta's 36 declarations are `absentia`**
(`grep -o` over `crusta/grammatica/*.stml`: 28 `absentia`, 7 `vacua`,
1 `diagnosticum`), so the free span lands on roughly four fifths of
them the day it ships. **css declares zero `absentia`**, so no css
output moves.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| SM1 | **The primary span stays a plain field**; related spans are a list of `{tractus, nota}` | The header line `x.sh:9:1: …` needs exactly one position, and the pinned sort key `(tractus.initium, codex)` reads it directly. A flat array would still need a law saying `sedes[0]` is primary — the same privilege with an indirection in front of it, plus an empty-array case the type allows and the facts forbid. rustc and clang both distinguish a primary for the same reason. |
| SM2 | **Related spans carry a LABEL, never a role enum** | LSP's `DiagnosticRelatedInformation` is `{location, message}` and clang's notes are a location and a sentence — the one machine consumer already ticketed (`01M2RJGTB2`) would discard a role on the way out. A role only ever selects a default label; with the label present it is a table from an enum to a string we could have written directly. Rejected: inventing three role names on the strength of one real case (`apertura`) and two imagined ones. |
| SM3 | **Labels are finite verbs, never participles** | A participle agrees with its subject and the subject varies by node genus — `apertus` for a masculine `grex`, `aperta` for a feminine `redirectio`. One fixed string cannot agree with all of them, so a participle label is quietly wrong most of the time it is shown. `hic coepit` and `hic exspectatur` agree with nothing. |
| SM4 | **The derived span is the node's FULL accumulated range, automatic for every `absentia`, with no flag** | `absentia` means the slot was never written, and the node owning the slot is always what should have written it. There is no case where showing it is wrong, so a flag would be vocabulary with no false branch. `excerptum` clamps to end of line, so a node spanning lines 3–9 underlines line 3 and stops. |
| SM5 | **Three sources of spans; only DERIVED and EMISSA are built** | A declared second span needs registry vocabulary naming a second locus — the arm closest to the expression language M7 forbids, and no client has asked for it. |
| SM6 | **One block, source order, gutter sized once**; no elision, no inline multi-caret interleaving | Fran chose rustc-shaped over clang-shaped `note:` blocks, 2026-09-17. Elision and interleaving are the genuinely hard half of rustc's renderer and buy presentation only — nothing a consumer can act on. Each printed line already carries its own number, so a gap is never ambiguous. |
| SM7 | **The TSV grows two columns, never extra rows** | One row stays one diagnostic. Extra rows would silently inflate every count that greps the file, including `tools/diagnostica_fumus.sh` gate I. Appended columns leave the existing mid-line greps matching. |
| SM8 | **Diagnostics still do not appear in the STML projection** | `01M2RJKBA3` stays deferred. Keeping them out means no writer change, no reader change, no round-trip gate and no seal movement — the whole of this spec lives downstream of the tree. |

---

## 1. The record

`materia/fontes/materia_diagnostica.h`:

```c
/* Sedes cognata: locus alter quem diagnosticum nominat, cum nota sua.
 * 'nota' verbum FINITUM est, non participium (SM3): subiectum per
 * genera variat, verbum cum nullo convenit. */
nomen structura {
        MateriaTractus  tractus;
    constans character* nota;      /* NIHIL licet */
} MateriaSedesRelata;

nomen structura {
                           s32  gravitas;
            constans character* codex;
            constans character* causa;
         constans MateriaNodus* nodus;
         constans MateriaToken* lexema;
                MateriaTractus  tractus;   /* PRIMARIA: caput et ordo */
            constans character* nota;      /* nota sedis primariae */
    constans MateriaSedesRelata* relata;   /* NIHIL = ut hodie */
                           i32  numerus_relatorum;
} MateriaDiagnosticum;
```

`relata` is allocated from the same piscina as the diagnostics `Xar`,
so its lifetime is the caller's arena and nothing new is freed.

**A diagnostic with `relata == NIHIL` AND `nota == NIHIL` renders
exactly as today, byte for byte** — which is every `GENUS` and every
`VACUA` diagnostic, and so every diagnostic css declares. That is what
keeps silent clients silent.

`ABSENTIA` output **does** change, deliberately: its primary gains the
label `hic exspectatur` even when no opening span was found. Nothing
pins the human text — the gates read struct fields and the smoke test
greps TSV columns — so no gate moves, but the change is visible and
intended, not incidental.

The substrate's two labels, and the only two it supplies:

| constant | value | where |
|---|---|---|
| `MATERIA_NOTA_COEPIT` | `"hic coepit"` | the derived opening span |
| `MATERIA_NOTA_EXSPECTATUR` | `"hic exspectatur"` | the primary of an `absentia` |

## 2. Where spans come from

| source | who supplies it | built? |
|---|---|---|
| **derived** | the walker's `t`, discarded today at `:306` | **yes**, automatic for every `absentia` |
| **emissa** | the client hands over a finished record | **yes**, free once the field exists |
| **declared** | the registry names a second locus | **no** — deferred until a client asks |

The `absentia` arm becomes:

```c
casus MATERIA_DIAGNOSTICUM_ABSENTIA:
    valor = &nodus->loci[r->locus];
    si (   valor->genus == MATERIA_VALOR_NIHIL
        || (   valor->genus == MATERIA_VALOR_REFERENTIA
            && valor->datum.nodus == NIHIL))
    {
        finis = habet ? _punctum_finis(&t) : punctum;
        _addere_cum_apertura(d, r, nodus, &finis,
            habet ? &t : NIHIL);
    }
    frange;
```

`_addere_cum_apertura` sets `nota = MATERIA_NOTA_EXSPECTATUR` and, when
the opening range exists, allocates a single `MateriaSedesRelata`
holding `t` and `MATERIA_NOTA_COEPIT`. When `habet` is false there was
no lexeme to point at and the list stays empty — the diagnostic
degrades to a single labelled span rather than inventing a position.

**For `emissa`, the `initium == -I` auto-compute stays primary-only.**
It derives a position from `nodus`, and a related span points
somewhere other than its own node by definition. Related spans in an
emissa record must arrive concrete; the walker copies them unchanged.

**`GENUS` and `VACUA` gain nothing.** Neither has a second position in
hand, and manufacturing one would be a program, not a table.

## 3. The printer

`include/excerptum.h` gains a span type and a multi-span entry point.
`excerptum_scribere` keeps its signature and its callers.

```c
nomen structura {
                    s32  initium;
                    s32  finis;
                    i32  linea;
    constans character* nota;      /* NIHIL licet */
} ExcerptumSedes;

/* Sedes ORDINATAE esse debent (initium crescens); aliter FALSUM -
 * recusatio clara, ne ordo tacite fallat. FALSUM etiam si numerus
 * ZEPHYRUM aut sedes ulla extra [0, mensura]. */
b32
excerptum_scribere_multa (
         ChordaAedificator* exitus,
        constans character* fons,
                       i32  mensura,
    constans ExcerptumSedes* sedes,
                       i32  numerus);
```

Rules:

1. **Sortedness is required, not repaired.** The function verifies
   non-decreasing `initium` and returns FALSUM otherwise. No
   allocation, no mutation of a const array, and a loud refusal
   instead of silently reordered output.
2. **The gutter is sized once**, from the largest `linea` in the
   array: `latitudo = max(IV, digits(linea_maxima))`. Today's
   `_marginem` takes a minimum of IV, so blocks align for free up to
   line 9999 and go ragged at the 9999/10000 boundary — that pair is the
   gate in §6.
3. **Each distinct line prints its source once**, followed by one
   caret row per span on that line. Sorted-by-`initium` makes
   same-line spans adjacent, so the grouping is a single pass.
4. **A caret row** is `^` at the span start column and `~` per UTF-8
   character to the span end or the line end, whichever comes first —
   unchanged from the one-span printer — then a space and the `nota`
   when present.
5. **No elision.** Distant lines print back to back; each carries its
   own number.

**Pinnable identity:** with `numerus == I` and `nota == NIHIL`, the
output is byte-identical to `excerptum_scribere` on the same span.

## 4. The machine faces

### 4.1 TSV

Ten fields become twelve. Both are appended, so every existing
mid-line grep survives.

| # | field |
|---|---|
| 11 | related spans, `L:C-L:C\|nota` entries joined by `;`; empty when none |
| 12 | the primary's own `nota`; empty when NIHIL |

A vertical bar, a semicolon, a TAB or a NEWLINE inside a label is
replaced by a single space before writing. Lossy, never corrupting.
Today's two substrate labels contain none of them; a rule-supplied
label later might.

### 4.2 pythonica

```python
SedesRelata = namedtuple('SedesRelata',
                         'linea columna linea_finis columna_finis nota')

Diagnosticum = namedtuple('Diagnosticum', 'via linea columna linea_finis '
                          'columna_finis gravitas codex causa textus '
                          'nota relata')
```

`relata` is a tuple, empty when the field is empty. `nota` is `None`
when field 12 is empty.

## 5. What does not change

Stated loudly, because the negative space is most of why this is
cheap:

- **Registry vocabulary.** No new attribute. `MateriaTabDiagnosticum`
  and `MateriaDiagnosticaCocta` are untouched, so **the registry seal
  does not move** and every client regenerates byte-identically.
- **`materia_diagnostica_derivare`'s signature.**
- **The sort order** — `(tractus.initium, codex)`, primary only.
  Related spans never affect ordering.
- **The STML projection**, hence the writer, the reader, the
  round-trip gate and `materia_sedes_verificare`'s law that the view
  is a bare projection apart from `sedes`/`octeti`/`visio`.
- **`excerptum_scribere`** and all of its callers.
- **Every pinned assertion in the existing gates**, which read
  `codex`, `gravitas` and the primary `tractus`
  (`css/probationes/probatio_css_diagnostica.c:207-212`).

## 6. Gates, each born red by a plant that kills one specific line

The house lesson from the parent arc — hit three times — is that **a
diagnostic whose plant stays green is a dead gate**, and every time
the cause was a missing corpus case. Each gate below names the line
its plant removes and the assertion that must go red.

| # | plant | must go red on |
|---|---|---|
| I | delete the `relata` assignment in the `absentia` arm | a crusta fixture that **provably** yields an `absentia`: `numerus_relatorum == 1` **and** the related span's `initium` equal to the node start. Both, because the count alone passes if the span is filled with the wrong range. |
| II | remove same-line grouping | two spans on one line must print **one** source line and two caret rows; the plant prints two source lines |
| III | revert to a per-block gutter width | a span pair on lines **9999 and 10000** must share width V; the plant makes them ragged. Its own gate precisely because nothing else would ever notice. |
| IV | accept unsorted input instead of refusing | `excerptum_scribere_multa` on a descending array must return FALSUM |
| V | drop the label escape | a label containing `;` must not produce a TSV field that parses as two spans |

Gate I's fixture must be chosen by **reading crusta's derived output
first**, not by assuming a shape produces an `absentia`. The three
green plants in the parent arc were all this mistake.

## 7. Non-goals

Role vocabulary. `...` elision. Inline multi-caret interleaving.
Registry-declared second spans. Diagnostics as view elements
(`01M2RJKBA3`). Colour. Display-width correction (`01M2RHXBWP`).

## 8. AUDIENDA

**I. The `bash -n` pins now have a third position to choose from.**
crusta's two pins (gravitas 122/125, lineae 7/9) compare our reported
line to bash's. This spec does not change what they compare — the
**primary** — and that must stay deliberate. Loosening a pin to
"bash's line is among our spans" would weaken it, and must never
happen as a side effect of a span being added. The ticket's claim that
multi-span makes the divergence "evaporate" is wrong and is corrected
here: it adds a third position, and the divergence resolves only if
someone chooses a span set that includes bash's EOF line.

**II. Nothing checks the grammar of a rule-supplied label.** SM3 is
enforced by the substrate for its own two strings. When the
`Congruentia` adapter (`01M2RNJ9XN`) lets a rule write
`nota="…"`, the participle trap reopens with no gate on it.

**III. Two caret rows double the display-width error.** The caret
counts UTF-8 characters, so CJK lands one column short and combining
marks one long (`01M2RHXBWP`). One misaligned row was easy to miss;
two stacked rows will not be.

**IV. Same-line adjacency is an inference, not an invariant.** The
grouping pass assumes sorted-by-`initium` puts same-line spans next to
each other. That holds when every span's `linea` matches its offset.
The sortedness refusal is the only thing guarding it; a caller passing
a wrong `linea` gets wrong output, not a refusal.

**VI. CORRECTED IN BUILD (Task 2).** This spec twice named 999/1000
as the width boundary. It is **9999/10000**: `_latitudo` takes a
minimum of IV, so 999 (three digits) and 1000 (four) both render at
width IV and a per-line gutter would look identical there. A gate
written at 999/1000 would have been a fourth green plant — it could
never have failed. Measured and fixed while building; the gate is at
9999/10000.

**V. The first client to want a declared second span forces a real
decision.** Either the registry names a second locus — a table entry,
survivable — or the diagnostic names a *relationship* between slots,
which is the expression language M7 forbids. Decide that when it
arrives, not before.
