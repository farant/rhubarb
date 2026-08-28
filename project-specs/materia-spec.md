# materia — the general syntax-tree substrate, forked out of silva

*2026-08-27. Designed with Fran, on the evidence of
`css-arbor-spec.md`, `css-arbor-plan.md`, and `html-arbor-spec.md` —
two frontends designed on paper before any of their code was written.
That exercise is what justified this document; §1 lists what it found.*

**Thesis:** silva is two libraries wearing one name, and the general
half deserves to be its own. Fork it, generalize freely where nothing
must stay green, then migrate silva/C89 onto it — with **C89 migration
as the acceptance gate, not a someday**.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| M1 | **Fork, not extract-in-place** | What we found needs *redesign*, not movement (§1). In-place extraction must keep 281/281, 154/154, 78/78 and haruspex green at every intermediate step; a fork may be broken for a week. It is also the cheapest way to be wrong: a bad fork is discarded, a bad retrofit is surgery on a working system. |
| M2 | **THREE layers, not two** | substrate · LR toolkit · C89 frontend. §2. This is what dissolves G4 and HG2 rather than working around them. |
| M3 | **C89 migration is the ACCEPTANCE GATE** | The substrate is not real until silva's own gates run green through it. §7. |
| M4 | **`silva_tabulae.h` splits FIRST, inside silva** | Mechanical, logic-free, independently verifiable against the green gates. It makes the fork's boundary visible before committing to it. §3. |
| M5 | **CSS is the fork's first client; C89 the acceptance test; HTML the third** | Easy client proves the API is usable; hardest client proves it is sufficient; third proves it generalizes. |
| M6 | **LR table generation becomes an OPTIONAL output** | §5.3. Languages opt in. Hand-written frontends never declare `conditionalis`. |
| M7 | **The definition is a TABLE, never a program** | Same decree canon carries: *no expression language, ever*. §5.5 names the failure modes. |
| M8 | **materia stays THIN** | The landscape's cautionary tale (§8): rowan stayed small and thrived; PSI tried to be general *and* rich and got huge. Every "just one convenience that knows about a language" is the rot. |

**Names.** `materia` — silva is a forest; forests are made of *materia*.
And scholastically exact: materia is what receives *forma*, which is
what a general syntax substrate is — the matter a grammar informs.
The generator is `faber`, the craftsman who works it.

---

## 1. What the paper exercise found (the evidence for M1)

Four findings, all reached by reading, none by building. Full detail
in `html-arbor-spec.md` §2 and the G4 note under quaestio
`01M12BDHHQBKEKZEXFBRANKQ5F`.

1. **`SilvaLexicon.genus_identificator` is a C question in a shared
   struct.** `SILVA_LEX_IDENTIFICATOR` is read at `silva_arbor.c:2851`
   in exactly one place — synthesizing a macro-definition-site token
   during origin-chain reconstruction. Every frontend would supply a
   value never read.
2. **Five structural genera are mandatory** at `silva_generare.c:1315`
   — `ambiguus`, `error`, `conditionalis`, `ramus-sumptus`,
   `ramus-omissus` — three of them modelling the C preprocessor. Both
   CSS and HTML must declare them to be accepted at all.
3. **`reinserenda` covers non-tree bytes only** (`silva_scribere.c:760`).
   Full HTML5 foster parenting moves *tree* content out of byte order,
   which the mechanism does not cover.
4. **The coupling census was a floor, not a count.** It measured
   *lexical* coupling (`SILVA_LEX_*`) and could not see *structural*
   coupling (required node genera). Neutrality in one is not
   neutrality in the other.

**Why this argues for a fork and not an extraction:** items 1, 2, and 3
are not code in the wrong place. They are code that is *wrong* — an
interface that asks the wrong question, a requirement that should not
exist, a mechanism scoped too narrowly. Extraction moves; only a fork
can redesign.

---

## 2. The three layers, measured

Measured 2026-08-27, `.c` + `.h`:

| layer | modules | lines |
|---|---|---|
| **materia** | nodus, token, arbor, arbor_aequalitas, scribere, quaestio, quaestiones, commissio, contextus, latina_datum, + registry types | **16,676** |
| **LR toolkit** | glr (1,567) + generare/coquere/canon_coquere (7,089) | **8,656** |
| **C89 frontend** | lexema, expandere, conditio, semantica, oraculum, fluxus, fluxus_datorum, annotationes, parsare | **30,654** |
| *(generated)* | tabulae_c89 | *17,307* |

**Of materia's 16,676, only 8,775 need surgery** — `silva_arbor`
(7,310 + 566) and `silva_scribere` (804 + 95). Both `#include`
`silva_expandere` and `silva_parsare`. **Cutting the preprocessor out
of the STML writer and the byte emitter IS the project.** Everything
else moves nearly clean:

```
silva_nodus            -> silva_token                see 2.2 ↓
silva_token            -> (none)                     CLEAN
silva_commissio        -> nodus tabulae token        registry types only
silva_quaestio         -> nodus tabulae token        registry types only
silva_quaestiones      -> quaestio tabulae           registry types only
silva_arbor_aequalitas -> arbor                      follows arbor
silva_arbor            -> ... expandere parsare      ← SURGERY
silva_scribere         -> ... expandere parsare      ← SURGERY
```

### 2.2 CORRECTION 2026-08-27 — `silva_nodus` is NOT clean

The table above measured **includes**. `silva_nodus.c` includes only
`silva_token.h`, so it read as clean. It is not: **five query families
walk the origin chain**, measured against the source —

| site | function | question |
|---|---|---|
| 375 | `silva_valor_extensionem` | byte extent |
| 434 | `_extensionem_lineis_valoris` | line/column extent |
| 537 | `silva_valor_est_fons_purus` | source purity |
| 630 | `_sedes_colligere` | geometry fidelity |
| 795 | `_lexema_primum_valoris` | leading comment block |

— because the extent of an *expanded* token is not the token's own
site but its **invocation's**. Phase 0.1 named one genuine core
intrusion (`ArborCursor.expansio`); **this is a second, in a module
this table called clean.**

**Same error class as the coupling census** (§1 item 4): the
measurement answered a *narrower* question than the conclusion drawn
from it. Includes were measured; semantics were concluded. Worth
stating as a standing rule for this fork — *an include graph is a
lower bound on coupling, never a description of it.*

**Resolved by ONE hook.** All five ask the same thing: *what is this
token's effective source site?* `MateriaOrigoUncus` answers it; a
frontend with no derivation leaves it `NIHIL` and gets the token's own
fields, which is correct for CSS and HTML rather than degraded.
Shipped in `materia/fontes/materia_nodus.h`.

### 2.1 What M2 dissolves

The five-mandatory-genera requirement (§1 item 2) lives in
`silva_generare.c:1315` — **the LR toolkit**. Hand-written frontends
never invoke it. So under M2:

- **HG2 evaporates.** CSS and HTML never declare `conditionalis`.
- **G4's answer is retired.** No vestigial production, because the
  registry does not come from the LR grammar compiler (§5.3).
- The generator stops assuming every language is parsed the way C is.

A wart that survives two clients is a design error; a wart that
disappears under the right decomposition was a *layering* error all
along. This one was the latter.

---

## 3. M4 — the first move, inside silva

`silva_tabulae.h` is two headers wearing one name:

| LR table types | registry types |
|---|---|
| `SilvaTabActio`, `SilvaTabGoto`, `SilvaTabStatus`, `SilvaTabSymbolum`, `SilvaTabProductio`, `SilvaTabPraelata`, `SilvaTabulaCocta` | `SilvaTabGenus`, `SilvaTabLocus`, `SilvaRegistrumCoctum` |

**Measured: the registry types are CONTIGUOUS** —
`silva_tabulae.h:88-105`, three plain structs under one section
banner, holding `constans character*`, `s32`, `i32`, and pointers to
each other. **Nothing in that block needs `silva_token.h`** (only
`SilvaTabSymbolum:65`, on the LR side, carries a lexeme genus). So the
split is a literal lift of one contiguous block into
`silva_registrum.h`, whose only include is `latina.h`. `commissio`, `quaestio`,
and `quaestiones` then depend on the registry header alone, and the
substrate boundary becomes **visible in the include graph** rather
than asserted in a spec.

Done **in silva, before the fork**: mechanical, no logic change,
verified by the existing gates staying green. If this split is hard,
the fork's premise is wrong and we learn it for the price of a header.

### 3.1 M4 DONE 2026-08-27 — and the premise held

**Blast radius, measured over all 33 files including `silva_tabulae.h`:**

| file | registry refs | LR refs |
|---|---|---|
| `silva_arbor.h` | **11** | 0 |
| `silva_scribere.h` | 3 | 0 |
| `silva_quaestio.h` | 3 | 0 |
| `silva_quaestiones.h` | 2 | 0 |
| `silva_commissio.h` | 1 | 0 |
| `silva_glr.h` | 0 | **5** |

**All five materia-core headers are registry-only; `silva_glr.h` is the
exact mirror.** Four files use both (`silva_parsare.h` and the three
GENERATED `tabulae_*`); four are LR-only; 15 include the header without
naming any type from it. **No module straddles the boundary** — the
split runs along the layer seam, not through the middle of a habit.

**What was done:** `silva_registrum.h` created (57 lines, includes
`latina.h` and nothing else); `silva_tabulae.h` re-includes it so
existing callers are untouched; the five registry-only headers switched
to include `silva_registrum.h` directly.

**Verified:** silva 50/50 on a *clean* rebuild, root suite exit 0,
amalgam re-verified (standalone + hospes 40/40 + nm-intersection 0).

**The boundary is ENFORCED, not merely declared** — planted-fault
checked in both directions, per header:

| header | naming an LR type |
|---|---|
| `silva_commissio.h` | **refused** — boundary real |
| `silva_quaestio.h` | **refused** — boundary real |
| `silva_quaestiones.h` | **refused** — boundary real |
| `silva_arbor.h` | *still visible* — declarative only |
| `silva_scribere.h` | *still visible* — declarative only |

**And the split is the same one §4 found.** `arbor` and `scribere`
remain coupled to the LR half only because they include
`silva_parsare.h`, which uses both — and they are **exactly** the two
modules that also pull in `silva_expandere` and need the preprocessor
surgery. Two independent measurements, one target. That convergence is
the strongest evidence yet that the three-layer decomposition is real
rather than a convenient story.

### 3.2 A false green caught in passing

The first post-split run reported **50/50 — against objects six hours
stale.** `silva/compile_probationes.sh` tracks `.c` mtimes, not header
dependencies, so a header-only edit rebuilds nothing. The excubitor
staleness guard caught it (`VERDICTUM: STALA 17`, naming the chains);
`rm -f silva/build/*.o` and a re-run gave the real green.

**A header-only change needs a forced rebuild before its suite result
means anything.** This belongs in any task that edits a header — the
CSS plan's T4 and T5 both do.

---

## 4. The surgery: cutting the preprocessor out of arbor and scribere

`silva_arbor` and `silva_scribere` need `SilvaExpansio` for three
things, all preprocessor-shaped:

1. **Origin chains** — `<expansio>`, `<pasta>`, `<stringificatio>`,
   `<api>` nested inside token elements; def-site references.
2. **The expansion boundary** — a token whose origin is not FONS emits
   its stratum-0 invocation instead of itself.
3. **`extenta`** — function-like invocation spans, since a macro call's
   argument bytes are named by no tree token.

None of these exist for a language without a preprocessor. The cut:

- **materia's `token` carries `origo` as an OPAQUE, OPTIONAL hook.** A
  frontend with no preprocessor sets it once and never reads it.
- **Origin *serialization* leaves materia entirely** — it becomes a
  C89-frontend concern registered with the writer, not a branch inside
  it. This is what makes `genus_identificator` (§1 item 1) disappear
  rather than merely move: the code that reads it moves with it.
- **`reinserenda` generalizes from "non-tree bytes" to "bytes emitted
  out of tree order"** (§1 item 3), which serves C directives *and*
  HTML foster parenting with one mechanism. This closes HG3.

That last point is the clearest evidence the fork is worth doing: the
same generalization pays a C debt and an HTML debt at once, and
neither client could have motivated it alone.

### 4.1 MG1 MEASURED (2026-08-27) — right in direction, low in estimate

Two probes, both run; artifacts in the session scratchpad.

**Probe A — the gap is REAL.** A hand-built `error` node holding two
tokens in tree order A,B whose byte offsets are reversed (A at 3, B at
0), against a true source of `"AAABBB"`:

```
lexema A: valor='BBB' offset=3
lexema B: valor='AAA' offset=0
fons verus : "AAABBB"
emissum    : "BBBAAA"     <- ORDO ARBORIS
```

`silva_scribere_nodum` emits in **tree order**, confirming its header's
claim (*"numquam positione dependens"*) and confirming that
foster-parented content would emit wrong today.

**Probe B — the machinery is LIVE.** `silva_scribere_fontem` over
`"#define X 1\nint f(vacuum);\n"` round-trips **27/27 bytes exactly**.
The `#define` line is consumed by no tree token, so it can only have
arrived via the reinserenda pour. The mechanism works on real data.

**But it is UNREACHABLE from the path CSS and HTML use.** Measured:

| fact | site |
|---|---|
| `SilvaReinserendum = {s32 offset; Xar* lamina}` — content-agnostic | `silva_scribere.c:30-34` |
| `_laminam_emittere` just emits tokens, zero directive logic | `:249-258` |
| skip predicate: `st->fons_index >= ZEPHYRUM && token->fons_index != st->fons_index` → skip | `:299-306` |
| **`silva_scribere_nodum` passes `fons_index = -I`** → the skip can never fire | `:686` |
| `_scriptor_parare` hardcodes `st->reinserenda = NIHIL`; only `silva_scribere_fontem` fills it | `:613`, `:758` |

**Verdict: the design is sound; the estimate was low.** The claim was
"widen the collection". The truth is the mechanism must first be made
*reachable* from the subtree path at all. Three named edits, all in
`silva_scribere.c`:

1. `_scriptor_parare` takes a reinserenda list instead of hardcoding
   `NIHIL`.
2. The skip predicate generalizes from "belongs to a different source
   file" to a general **"emitted elsewhere"** hook.
3. Collection generalizes — today it lives inside
   `silva_scribere_fontem` and knows about directives and regions.

More work than stated, but **better specified than before**: three
edits in one file rather than a vague generalization.

**The elegant part, and it is real:** edit 2 is not speculative. An
`#include`'d token *is* a tree token that the walk skips while other
bytes are poured at its offset — so **"tree token skipped, bytes
supplied out of band" is already proven on real data** by the 154/154
gate. Foster parenting wants the same shape with a different predicate.

**Design consequence for §6:** `css-arbor-spec.md` §7 and
`html-arbor-spec.md` §7 both emit via the **subtree** path. If
out-of-order content is ever needed there, materia needs either
reinserenda plumbed into the subtree path (edit 1 — the smaller change,
recommended) or a third document-emission entry point that is neither
"pure structural subtree" nor "C `<parsura>`".

---

## 5. faber — the porcelain generator

### 5.1 What exists already

The pipeline is not new. `grammatica.canon` already schema-governs the
definition; the generator already emits constants, registry tables,
**176 guarded accessors**, and a canon. Guarded, not raw:

```c
SilvaValor
silva_c89_definitio_functionis_specificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}
```

### 5.2 The gap: constructors

Silva generates **accessors but not constructors**, for a structural
reason: GLR builds nodes from productions, so C89's parser never
constructs a node by hand — only consumers needed porcelain.

**A hand-written recursive-descent parser is entirely node
construction.** So CSS and HTML need a generation direction silva has
never had reason to want. Today a hand-written parser writes:

```c
silva_nodus_ponere(n, 3, valor, SILVA_LOCUS_LISTA_NODUS);
```

— a raw slot index plus a species the caller must get right. That is
a bug class invisible until a round-trip gate catches it three tasks
downstream. Generated instead:

```c
SilvaNodus*
css_regula_qualificata_creare (
    Piscina*   piscina,
    SilvaValor praeludium,   /* lista-nodus */
    SilvaValor corpus);      /* nodus */
```

Index and species become un-typoable, and the parser reads like the
grammar. **In scope for the fork (M5's first client needs it).**

### 5.3 Declared outputs (M6)

The definition declares what it wants built:

```xml
<exitus>
  <registrum/>      <!-- semper -->
  <porcellana/>     <!-- accessores + constructores -->
  <canon/>
  <!-- <tabulae-lr/> ABSENS: CSS descensu recursivo parsatur -->
</exitus>
```

`<tabulae-lr/>` absent ⇒ no LR construction ⇒ no `<initium>`, no
productions, no five mandatory structural genera. **In scope for the
fork.**

### 5.4 Deferred, but shaped now

Two extensions we are **not** building yet, recorded so the definition
format does not preclude them:

- **Token vocabulary block.** `silva_arbor_orthographia`'s header
  confesses it is *"fons veritatis SECUNDUS, qui a lexatore divergere
  potest"* — and the mandatory orthography gate exists *because* the
  table is hand-maintained. Emit the lexicon *and* the lexer's genus
  enum from one declaration and divergence becomes structurally
  impossible; the gate drops from load-bearing to cheap insurance.
- **Slot genus constraints** — `slots="corpus:nodus(saeptum)"`. **One
  addition, three payoffs**: G3's species-level canon closes,
  constructors get typed parameters, runtime guards check genus rather
  than only species. When one addition pays three ways it is usually
  the right addition; we defer it only because CSS does not need it to
  ship.

### 5.5 The four failure modes, and the discipline for each

1. **The definition becomes a programming language.** *A table, never
   a program* (M7). This is the one with real gravity; it always
   starts as "we just need a conditional here."
2. **Generated code you cannot debug.** Generate only *thin* things. If
   a generated function needs a branch that is not a guard, it does not
   belong in the generator.
3. **The generator becomes every language's bottleneck.** Generate only
   what is *uniform*. Language-specific code sits **beside** the
   generated file, never inside it.
4. **Over-generation hides the model.** Already mitigated: the
   definition is the readable artifact, it is STML, and a canon judges
   it.

---

## 6. What this changes in the CSS and HTML specs

Neither is invalidated; both are relocated.

| item | was | becomes |
|---|---|---|
| CSS plan T4, T5 (lexicon seam, widen genus) | retrofit into silva | **materia design** — no retrofit |
| CSS plan T6 (amalgam export) | extend `silva.h` | **deleted** — CSS links materia directly |
| CSS plan T1 (G4 vestigial production) | required | **retired** by M6 |
| CSS spec D4 (placement, `css/` beside silva) | consumes `silva.h` | consumes `materia.h` — and **G1's layering inversion disappears**, since CSS no longer depends on the C parser |
| CSS spec §4 (registry) | five structural genera declared | **not declared** |
| HTML spec HG1, HG2 | open gaps | **closed by M2 and §4** |
| HTML spec HG3 (foster parenting) | deferred extension | **folded into §4's reinserenda generalization** |

Roughly 13 of the CSS plan's 17 tasks survive untouched. **G1 —
the named layering debt this whole thread began with — is not paid
down; it never gets incurred.**

---

## 7. M3 — C89 as the acceptance gate

The fork is **not done** when CSS works. It is done when silva's own
gates run green **through materia**:

| gate | bar |
|---|---|
| subtree round trip | 281/281, both oracles |
| plain-C plagulae | 78/78 |
| latinized plagulae | 154/154 |
| hospes through the header | 39/39 |
| adversarial | 24/24 |
| `haruspex` clang agreement | 243 TUs |

**The reframe that makes this attractive rather than a tax: C89 is not
the legacy burden, it is the acceptance test.** These gates are the
hardest exam a general syntax-tree library can sit — byte-exact round
trip *through macro expansion*, GLR ambiguity, conditional regions,
154 latinized files. A substrate proven only on CSS proves almost
nothing. One that carries C89 will carry anything.

This is also the answer to the fork's real risk. Silva is
`c-parser-attempt-4`; there is a predecessor graveyard (arbor2,
lapifex, praeparator) with a live landmine — `lib/nuntium_schema.c`
still builds on lapifex. **But attempts 1-4 were rewrites; this is an
extraction.** Parity is the starting point, not the goal, so the
rewrite failure mode — never reaching parity — does not apply. The
*other* fork failure mode does: divergence while both live, and
"migrate C89 at some point" quietly becoming never. M3 is the
mitigation, and it is a gate rather than an intention.

**Silva is frozen to bug fixes for the fork's duration**, each fix
replayed into materia in the same commit. If that becomes onerous, the
fork is taking too long and should be cut down, not extended.

---

## 8. Landscape

materia is not novel, and that is reassuring. Roslyn's green nodes,
rowan's `SyntaxKind` as `u16`, tree-sitter's opaque `TSNode`, and
`SilvaNodus` all independently landed on the same model: uniform node,
**kind as an integer index**, children in a slot array, no per-language
C structs. Nobody copied anybody; it is what you get when one tree
walker must serve N languages.

**The direct precedent: `rowan` was extracted from rust-analyzer** — a
language-specific parser that grew a general lossless tree library,
which was then lifted out. Same move, same order (hard client first),
and it landed rather than becoming a graveyard entry.

Two things are genuinely unusual here, and both are worth knowing:

- **A readable serialization of the tree is rare.** tree-sitter has
  none, Roslyn has none, rowan has none. srcML and SwiftSyntax are the
  exceptions. The tree *model* is a well-trodden attractor; the tree
  *as a document you can diff, version, edit, and gate with a schema*
  is not. **arbor/STML is the novel half of this project**, not
  materia.
- **Nobody does full fidelity through a preprocessor.** tree-sitter's
  C grammar does not expand macros; Roslyn keeps `#if`-disabled text as
  trivia. Silva's origo chains plus byte-exact emission *across*
  expansion may be close to unique in open code — which is exactly why
  §7's gate means something.

And the cautionary tale, which we have already dodged: ANTLR's and
PSI's generic trees are so generic that every serious user hand-builds
a typed layer. The 176 generated accessors are that layer, already
built. materia can be untyped at the core **because** faber emits the
typed face — which is precisely rowan's bargain, and why M8 matters.

---

## 9. Open, and named

| # | item | note |
|---|---|---|
| ~~MG1~~ | **CLOSED 2026-08-27** — measured, both directions (§4.1) | Design sound, estimate low: three named edits in `silva_scribere.c`, not one widening. Closes HTML's HG5 too. |
| MG2 | Token vocabulary block and slot genus constraints deferred (§5.4) | Shaped now so the format does not preclude them. |
| MG3 | Whether `quaestio` belongs in materia or above it | It needs registry types only, so it *can* live in materia — but M8 says thin. Decide when CSS actually wants a query. |
| MG4 | The lapifex landmine | `lib/nuntium_schema.c` builds on a predecessor generation. Untouched by this fork, but it is the standing proof that superseded generations here do not die on schedule. |


---

## 10. Roadmap — materia → CSS → HTML → silva

*Order per Fran 2026-08-27: new clients first, silva migrated last.
Six phases, ~55 tasks. Each phase ends with a gate that can fail.*

### The measurement that shapes it (2026-08-27)

What `silva_arbor` and `silva_scribere` actually use from the
preprocessor layer:

| from `expandere` | n | from `parsare` | n |
|---|---|---|---|
| `SilvaRegio` | 24 | `parsura->expansio` | 19 |
| `SilvaExpansio` | 14 | `parsura->lexema_finis` | 14 |
| `SilvaFons` | 6 | `parsura->directivae` | 10 |
| **functions** | **3** | `parsura->commissio` | 6 |
| | | `parsura->fons_princeps` | 4 |
| | | `parsura->successus` | 1 |

**The coupling is DATA-SHAPE, not BEHAVIOURAL.** They never drive the
preprocessor; they read fields off a C89-specific aggregate. Three
function references in 8,775 lines.

So the surgery is not "extract preprocessor logic" — it is **"replace
`SilvaParsura` with a materia-level document context"**:

```c
nomen structura {
    MateriaNodus*  radix;            /* universale */
    MateriaToken*  lexema_finis;     /* universale (CSS/HTML cauda) */
    Xar*           reinserenda;      /* octeti extra ordinem arboris (MG1) */
    s32            fons_princeps;    /* universale */
    constans vacuum* origo;          /* OPACUM - frons C89 solus legit */
} MateriaContextus;
```

Four of six fields universal; `directivae` becomes the generalized
`reinserenda` MG1 already specified; `expansio` becomes one opaque
pointer. **This is why the fork is cheap.**

### Phase 0 — specify the surgery *(in silva, ~2 tasks)*

#### 0.1 DONE 2026-08-27 — GATE PASSED, better than predicted

**Measured, `silva_arbor.c` (7,310 lines):**

```
earliest SilvaFons  : 4048        earliest SilvaRegio : 4385
core writer entry   : 2042 (silva_arbor_scribere_nodum)
SilvaRegio|SilvaRamus|SilvaFons in lines 1..4029 : ZERO
```

**The core walk never touches them.** Every site sits inside a
`_parsura_*` function:

| site | function |
|---|---|
| 4033, 6281 | `_parsura_fontes_scribere`, `_parsura_fontes_legere` |
| 4385, 4480 | `_parsura_primum_offset`, `_parsura_regiones_colligere` |
| 6568, 6636 | `_parsura_laminam_legere`, `_parsura_regionem_obtinere` |
| 6943, 7250 | `silva_arbor_legere_parsuram` |

And in `silva_scribere.c`, `_regiones_colligere` does exactly one thing:
walk the region tree calling `_reinserendum_addere(st, piscina,
<Xar of SilvaToken*>)`. **The scriptor never sees a `SilvaRegio`** — the
whole region-to-emitter interaction is *laminae become reinserenda*.

**The prediction was wrong in a useful direction.** §10 guessed the 24
`SilvaRegio` uses would reduce to *reinserenda + origin hook*. They
reduce to a **third category the spec never named: a frontend-owned
document section.** `<fontes>`, `<regio-directiva>`, `<regio-cruda>`,
`<regio-finis>` are C89 document sections, and they were **already
factored out** — the `_parsura_` prefix has been the layer boundary all
along. Nobody had noticed.

**Final classification, nothing left "unclear":**

| concern | category | destination |
|---|---|---|
| `SilvaRegio` / `SilvaRamus` | frontend document section | C89 frontend |
| `SilvaFons` / fontes table | frontend document section | C89 frontend |
| region laminae -> reinserenda | frontend collector | C89 frontend; materia takes the list |
| **`ArborCursor.expansio`** (extenta) | **origin hook** | **the one genuine core intrusion** |
| origin chains (`<expansio>`, `<pasta>`, ...) | origin hook | frontend token extension |

#### 0.2 The revised contract

`MateriaContextus` needs **neither** `regiones` **nor** `origo` — origin
is a per-token hook, not a document field:

```c
nomen structura {
    MateriaNodus*  radix;
    MateriaToken*  lexema_finis;
    Xar*           reinserenda;   /* octeti extra ordinem arboris */
    s32            fons_princeps;
} MateriaContextus;

/* Uncini frontis - OMNES optionales (NIHIL = lingua eos non habet) */
nomen structura {
    vacuum* datum;                                    /* status frontis */
    b32   (*sectiones_scribere)(vacuum*, StmlNodus*); /* <fontes>, <regio-*> */
    b32   (*sectiones_legere) (vacuum*, StmlNodus*);
    b32   (*origo_scribere)   (vacuum*, StmlNodus*, MateriaToken*);
    b32   (*origo_legere)     (vacuum*, StmlNodus*, MateriaToken*);
    Xar*  (*extentum_quaerere)(vacuum*, constans MateriaToken*);
} MateriaFrons;
```

Four callbacks plus one extent lookup. CSS and HTML pass `NIHIL` for
every field. **The surgery is smaller than par. IV estimated** — not
because the estimate was careless, but because the `_parsura_` factoring
had already done most of it.

**Gate:** every use site classified, none left "unclear". **PASSED.**

### Phase 1 — the fork *(~10 tasks)*

- 1.1 `materia/` created; substrate modules copied, renamed `materia_*`.
  **Types are silva's types renamed** — this is what makes the Phase 1
  gate cheap.
- 1.2 The three MG1 edits: reinserenda reachable from the subtree path.
- 1.3 The surgery: `MateriaContextus` replaces `SilvaParsura`; origin
  serialization becomes a registered hook.
- 1.4 `MateriaLexicon` — **without** `genus_identificator` (HG1).
- 1.5 `materia/compile_probationes.sh`, tessera template.

**Gate — the one that matters:** a C89 **shim** passes the M1 subtree
round trip **281/281 through materia**, without migrating silva.
Because materia's types are silva's renamed, the shim should be thin —
**UNPROVEN, and load-bearing.** It is the assumption that makes this
gate affordable; if the shim turns out expensive, the gate is still
right but phase 1 is bigger than costed. Settle it FIRST inside phase 1,
before the surgery, while a wrong answer is still cheap.
**This buys a C89 measurement four phases before the C89 migration** —
the mitigation for the ordering risk below.

**GO/NO-GO.** If the shim cannot reach 281/281, stop and reconsider
before CSS exists.

### ⚠ ORDER RETARGETED 2026-08-27 (Fran) — faber DEFERRED

**CSS is written by hand; faber emerges from what CSS actually needed.**
Phase 2 below is not next; phase 3 is.

Three options were weighed: (A) faber first as written, (B) migrate
silva first and end the freeze, (C) skip faber, hand-write CSS.

**C, because:** faber would generate constructors **no one has used
yet**, and §5.5 already warns against speculative generation — what is
*uniform* cannot be known from one client. Because the payoff claim
("materia gives every language the arbor/STML projection for free") is
**still untested**, and C reaches it fastest. And because B is not
urgent: the ordering risk it would remove was retired by the phase 1
gate **exactly as designed** — 336 files round-trip through materia —
while the freeze has so far cost one bug fix, replayed trivially.

**Trigger that promotes B:** if the freeze gets expensive — more fixes
needing replay, or work genuinely required inside silva's core — then B
becomes the right order. Named now so it is not rediscovered later.

### Phase 2 — faber, minimal *(~5 tasks, DEFERRED — see above)*

- 2.1 Registry-only output; `<exitus>` declaration (M6).
- 2.2 **Typed constructors** (§5.2) — what hand-written parsers need.
- 2.3 Canon output.

Deferred by name: token-vocabulary block, slot genus constraints (§5.4).

**Gate:** `css.stml` → registry + constructors + canon, generated, with
no `<tabulae-lr/>` and no vestigial production.

### Phase 3 — CSS *(~14 tasks)*

`css-arbor-plan.md` retargeted: **T1 retired** (M6), **T4/T5 relocated**
into Phase 1, **T6 deleted** (no amalgam export needed). T2, T3, T7-T17
survive.

**Gate:** byte round trip over the corpus, STML round trip (two cycles),
canon, selectors, the absent-slot and one-owner assertions.

### Phase 4 — HTML *(~14 tasks)*

`html-arbor-spec.md`: adaptare, the simple builder, the reserved-and-
**refused** vocabulary (§6.3), the absent-slot gate, the reservation
gate.

**Gate:** the HTML corpus round trips, and a document carrying `ancora`
/ `clonatum` / `<reinserendum>` is REFUSED with a named cause.

**This is the generality test.** CSS and HTML are structurally alike;
if materia needs changes for HTML, that is real information about the
substrate, not about HTML.

### Phase 5 — migrate silva *(~8 tasks)*

- 5.1 silva's nodus/token/arbor/scribere/aequalitas/quaestio deleted;
  silva includes materia.
- 5.2 Origin serialization registered as C89's hook.
- 5.3 Amalgam regenerated (`silva.h` hand-mirror extended).

**Gate — M3, the acceptance test:** 281/281 · 78/78 · 154/154 ·
hospes 39/39 · adversarial 24/24 · haruspex 243 TUs.

### Phase 6 — cleanup *(~4 tasks)*

LR toolkit's final home decided; duplicated code deleted; `MEMORY.md`,
`rhubarb.census`, `MAP.txt` updated; the lapifex landmine (MG4)
re-checked.

### The ordering risk, named

CSS and HTML are **structurally alike**: hand-written recursive descent,
no preprocessor, no ambiguity, no conditional regions. C89 needs origin
chains, expansion boundaries, `extenta`, `ambiguus` nodes, and
conditional regions. **If C89 comes last, materia's API is designed by
two similar clients and the hardest one arrives with no say.**

Three mitigations, all cheap:

1. **Phase 1's C89 shim gate** — a real C89 measurement before CSS.
2. **A "can C89 still land?" audit at every phase boundary**, against
   §7's gate list.
3. **Silva frozen to bug fixes** for phases 1-5, each fix replayed into
   materia in the same commit. If replay becomes onerous, the fork is
   taking too long — cut scope, do not extend.

The alternative order — migrate silva in Phase 2, before CSS — removes
the risk entirely and removes the freeze. It is the safer sequence and
the slower one to any new capability. **Recorded here so the trade is a
decision, not a default.**
