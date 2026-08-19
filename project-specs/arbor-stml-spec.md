# Arbor — serialisatio canonica parsurae silvae (spec v1)

2026-08-19. Status: **v1 — awaiting explorer grounding** (v2 will pin
exact seam APIs: silva's vendored stml surface, fons-table access,
amalgam choreography). Lineage: arbor-stml-visio.md (the project,
parcum 01M0DMSTEJ) → arbor-stml-exhibita.md (hand-written candidates)
→ arbor-stml-interview.md (12 decisions, all recorded). Scope here =
**M1** (visio §6): subtree serialization, round-trip gate,
representational audit. M2 (full parsura, origo, fragments at scale)
extends this format; nothing here may break when it arrives.

## 1. The dialect

- **Name: "arbor".** Subtree documents envelope `<arbor>`; the M2
  full-document form will envelope `<parsura>` (second form, same
  dialect). Canon file, `silex iudicare` registration, and tooling all
  use "arbor".
- **Envelope attributes**: `grammatica="c89"` +
  `registrum-sigillum="…"` (house seal computed at runtime over the
  registry's genus/locus tables — name lists, species, layout order).
  The loader REFUSES loud on either mismatch: a tree judged by the
  wrong vocabulary is a lie. Plus the position anchor (§4):
  `fons="via"` and `b-initium`/`linea-initium`/`columna-initium` for
  the subtree's start.
- The document is a PROJECTION; the .c file remains truth (doctrine).

## 2. Vocabulary (all registry-given, nothing invented)

- **Node genus = element tag**, verbatim from SilvaTabGenus.titulus
  (already kebab-case Latin: `declaratio`, `vocatio`,
  `definitio-functionis`).
- **Locus = wrapper element**, verbatim from SilvaTabLocus.titulus
  (`specificatores`, `tok_terminator`). Genus and locus elements
  strictly alternate by depth. A NIHIL locus is OMITTED (canonical
  absence); loci appear in layout order.
- **Lexeme genus = element tag**, from NOMINA_GENERUM with one mangle
  rule: lowercase + `_`→`-` (`IDENTIFICATOR`→`identificator`,
  `PAREN_APERTA`→`paren-aperta`, `NOVA_LINEA`→`nova-linea`). Node and
  lexeme namespaces are disjoint by language (Latin vs English) —
  measured, exhibita §0.
- **LISTA loci** (NODUS/TOKEN/MIXTA): the wrapper contains the view's
  elements in order, separators included where the grammar interleaves
  them. Serialization writes the PROSPECTUS (view), never the shared
  repositorium — engine-internal sharing is not representational.
- **INDEX loci**: arabic text content — `<canonica>0</canonica>`.

## 3. Tokens

- **Valor is text content, never an attribute** (`"` is
  unrepresentable in STML attribute values — measured). Only
  variable-spelling genera carry text: identificator, integer, float,
  character-lit, string-lit, octetus-ignotus, the imperfecta, and
  comment trivia. Fixed-spelling genera (keywords, punctuation) are
  empty elements — the spelling IS the genus. The reader restores
  spelling from a fixed table.
- **Trivia**: `<ante>`/`<post>` wrapper children of the token element,
  present only when non-empty, containing compact lens forms:
  `<spatia n="4"/>`, `<tabulae n="2"/>`, `<nova-linea/>` (+ boolean
  `crlf` when `\r\n`), `<continuatio/>` (+ `crlf`), and comments as
  entity-escaped text (`<commentum-clausum>/* … */</commentum-clausum>`,
  `<commentum-linea>// …</commentum-linea>`). Lens forms are lossless
  (valor fully determined). Comments never use raw `!` framing (their
  content could contain the closing sequence; entity text is
  symmetric).
- **Scissurae**: `<scissura offset="3"/>` (+ `crlf`) children, rare.
- **Attributes carried only when non-default**: `standard` when not
  C89 (`standard="c99"`/`"extensio"`); `f="N"` (fons index) when not
  the document default. Def-site/pool tokens (M2) carry their own
  positions — another file's coordinates, not derivable here.
- **Mixed content is legal and deliberate**: a token element may hold
  valor text and ante/post/scissura children together.

## 4. Derived fields — the fixup class (and the audit)

NOT serialized; recomputed by the loader in a post-load fixup pass
(the pater/post-accept precedent):

| field | recomputation |
|---|---|
| pater | parent-assignment walk |
| initium_lineae | trivia-stream walk (first non-trivia of logical line) |
| longitudo | valor bytes + scissura widths |
| byte_offset / linea / columna (stratum 0) | emission walk from the envelope anchor (cumulative sums over trivia + spellings) |

**The law: a canonical document must not be able to lie.** Anything
derivable is derived; redundancy that can disagree is excluded from
the format. The round-trip gate therefore compares RECOMPUTED fields
against the original tree — the representational audit running
mechanically on every fixture. Every field found NOT to survive
tree→STML→tree is logged by name (audit obligation, visio §2.1).

## 5. Whitespace law (canon-driven significance)

Valor-bearing elements are whitespace-significant and the writer
always emits them TIGHT (open tag, content, close tag — no interior
formatting). Everywhere else, whitespace-only text nodes are IGNORED
at load. Which elements are valor-bearing is knowable from the
vocabulary alone (fixed table of lexeme genera + INDEX loci), so
pretty-printed output and hand-authored fixtures load identically.
Pretty remains the default written form — documents are for reading.

## 6. References (the dual law)

- **Shared content rendering in place** → named fragment at first use,
  transclusion at later uses: `<#lex1><identificator>i8…</#lex1>` then
  `<<#lex1>>`. Consumer one: AMBIGUUS arms sharing token objects
  (identity is semantics — duplication would lie). This is the first
  real pull on transclusion resolution in C (stml-visio §6 adopts on
  pull).
- **Identity pointers that are not content** → `#id` attribute values
  (document-space per decretum 01KZPEXT74). Consumer one: origo
  (`origo="#o1"` on the token; `<origines>` pool holds `<expansio
  macro="…" corpus="#t1" invocatio="#t2"/>` and the referenced
  tokens). M2 machinery — M1 reserves the attribute name and the
  `<origines>` envelope section, implements neither.
- **M1 expansion boundary: fractura clara.** A non-FONS token
  encountered during M1 serialization refuses loud (silva_scribere's
  boundary, same posture). The M1 corpus (roundtrip fixtures) is
  plain C89, so this costs nothing and keeps M1 honest.

## 7. Architecture

- **One generic reader + one generic writer, registry-driven at
  runtime** — parameterized by `constans SilvaRegistrumCoctum*`, no
  code generation, all three grammars (c89, sceletum, imparilis)
  served. Precedent: silva_scribere's "ambulatio grammaticae-ignara".
- Home: `silva/fontes/silva_arbor.{h,c}` (works against silva's
  VENDORED stml — amalgamare choreography applies; new public API =
  hand-extend silva/amalgama/silva.h).
- Surface (v1 sketch, v2 pins exact signatures):
  - `silva_arbor_scribere_nodum(piscina, nodus, registrum, contextus?) → SilvaScriptura`-shaped result carrying the STML text (and/or the StmlNodus tree for callers composing documents).
  - `silva_arbor_legere(piscina, intern, textus, registrum) →` loaded
    root SilvaNodus* + vitium report (refuse-loud struct: causa +
    linea, canon-style diagnostics). Loader validates: envelope,
    sigillum, genus/locus names against registry, locus species vs
    content, valor presence vs genus class, write-once via the
    checked `silva_nodus_ponere` path (S32 — construction checking
    for free).
  - Sigillum helper: `silva_registrum_sigillum(registrum) → chorda`.
- **Canon projection** (registry → generated arbor canon, judgeable by
  `silex iudicare`): NAMED DOOR, landing spot M2. In M1 the loader's
  own checks are the judgment.

## 8. Gates (M1)

1. **Round-trip**: for subtrees of every roundtrip-corpus fixture
   (probationes/fixa/roundtrip/): parse → pick subtree → serialize →
   load → TREE EQUALITY. Equality = structural recursion comparing
   genus, locus count/species, valor genus+text, trivia series,
   scissurae, standard, fons, AND the recomputed fixup fields (§4)
   against the original's. (The puritas probatio's token collector is
   the starting point for the comparator.)
2. **Refusal gates**: skewed sigillum refuses; unknown genus/locus
   refuses; species mismatch refuses; non-FONS token refuses
   (fractura clara). PLANTED FAULT for each at birth (silent gate ≡
   dead gate).
3. **Representational audit log**: fields that had to be excluded or
   couldn't round-trip, named in the worklog + spec update.

**Deferred, with landing spots (no silent caps):** mined pure
subtrees from lib/ via est_fons_purus (landing: M1-hardening, when
fixture shapes prove too tame); hand-authored golden trees (landing:
M3 explorer, where authoring becomes a surface); origo/caecatio/
conditional regions/strata (landing: M2 `<parsura>`); canon
projection (M2); explorer (M3); compressed nodes/components (M4,
speculum-door doctrine per visio §4).

## 9. Exempla normativa

From arbor-stml-exhibita.md with the interview's decisions applied —
exhibit I in final form:

```stml
<arbor grammatica="c89" registrum-sigillum="…"
       fons="minimum.c" b-initium="0" linea-initium="1" columna-initium="1">
  <declaratio>
    <specificatores>
      <typus-primitivus>
        <tok_verba>
          <int><post><spatia n="1"/></post></int>
        </tok_verba>
      </typus-primitivus>
    </specificatores>
    <declaratores>
      <declarator-initiatus>
        <declarator>
          <declarator-titulus>
            <tok_titulus>
              <identificator>n<post><spatia n="1"/></post></identificator>
            </tok_titulus>
          </declarator-titulus>
        </declarator>
        <tok_operator>
          <assignatio><post><spatia n="1"/></post></assignatio>
        </tok_operator>
        <initiator>
          <folium-integer>
            <tok_valor><integer>0</integer></tok_valor>
          </folium-integer>
        </initiator>
      </declarator-initiatus>
    </declaratores>
    <tok_terminator>
      <semicolon><post><nova-linea/></post></semicolon>
    </tok_terminator>
  </declaratio>
</arbor>
```

Exhibit III (AMBIGUUS sharing via fragments) and exhibit II (origo
pool, M2 preview) stand as written in exhibita §II–III, with
positions stripped per Q2.

## 10. Quaestiones for the v2 explorers

1. Silva's vendored stml surface: exact renamed symbols available
   inside fontes/ (silva_stml_*?), and whether fragments +
   transclusion parsing are in the vendored copy.
2. Fons-table access from a SilvaParsura/SilvaContextus for the
   envelope `fons=` and per-token `f=` defaults.
3. The sigillum: which house hash is available inside silva
   (differre's seal machinery?) and what exactly feeds it.
4. Emission-walk reuse: can the position fixup share code with
   silva_scribere's walk instead of a parallel walk?
5. Piscina discipline for load (per-document opus arena — the
   formator CLI lesson).
6. Where the tree-equality comparator lives so probationes and future
   consumers (differre? mutatio gates?) can share it.
7. Amalgam: what silva.h hand-extension + fontes_politica adjustments
   the new module needs.
