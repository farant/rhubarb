# Silva semantica — M0 typus + index (design)

Date: 2026-07-08
Status: Design DECISUS'd (interview 2026-07-08, all eleven on
recommendation) + V2 GROUNDING complete (three explorers + two
empirical tiebreaks, §§VII-XI). INTENTIO-ready pending Fran's
sign-off on the three §XI items (trivalent oracle into M0a being
the big one).
Related: officina-spec-v2.md §VI (the grounded milestone plan),
officina-simulatio-1.md (three complications, dissolved fears),
officina-simulatio-1-interview.md (raw Q&A), silva-quaestio-design.md
(the design-doc precedent), silva-incrementalitas.md (lifetime
constraint), grammatica/genera-c89.md (loci layouts).

Semantica is silva's M0 milestone and officina's first dependency:
scoped symbol tables, real types, host-faithful layout, the clang
oracle, the project index, and the ambiguity-closure pass — over
committed silva trees, as parallel tables, trees immutable.

## I. DECISUS (Fran, 2026-07-08)

1. **Systema** — the platform-definition file: the FULL ISO C89
   library surface (all standard headers' types + ~145 function
   prototypes), hand-written once as a committed text fixture
   (silva/fontes/systema_c89.h), compiled in via the latina-datum
   mechanism and injected through contextus lexica/praebenda. One
   document = semantica's extern surface = machinula's builtin
   surface (officina M2 reads the same file). Simulation finding #2
   made deliverable.
2. **M0a / M0b split.** M0a: declarations + scopus + typus + forma +
   haruspex + index + closure. Bar: all corpus files analyzed, oracle
   green, ambigui 15,243 → near-zero (HONEST version post-census:
   requires the TRIVALENT oracle, §VII — systema alone buys ~10%),
   saltuarius type-jump unblocked. M0b: expression typing + usual arithmetic
   conversions as explicit annotations. Bar: every expression node in
   the corpus typed; conversion table pinned by fixtures; demissio's
   input contract frozen.
3. **Expression types = parallel tables at analysis time**
   (nodus → {typus, conversio}); demissio reads, quaestio pseudos and
   saltuarius may query. Oracle-responsa architectural precedent.
4. **Haruspex coverage**: all file-scope nameable types (~415 today:
   358 untagged-typedef + 57 named-tag) get direct compile-time
   asserts; block-scope types run the identical layout code path —
   coverage argument RECORDED as a named park with
   replicate-under-synthesized-tag as the path back.
5. **Poison-type diagnostics**: TYPUS_ERROR absorbs downstream
   operations without cascade; diagnostics table (nodus, causa);
   analysis always completes on any bytes — the tree-sitter bar
   extended to semantics. Corpus expectation: zero diagnostics
   (clang-clean corpus); percursus counts them.
6. **Per-configuration semantics**: taken arms only. Multi-config
   typing (re-analysis under an atom assignment) is NAMED into the
   config-query milestone. The wildcard pin stays honored: untaken
   arms are cruda until that milestone regardless.
7. **Parks approved**: K&R definitions typed as unprototyped
   functions (corpus cannot contain them: -Wstrict-prototypes
   -Werror); semantica lifetime nests inside the parsura's piscina
   (REPL invalidation = M4 + incrementalitas journal).
8. **Index**: in-memory query surface (symbolum → declaring
   file/node/typus); first consumer saltuarius type-jump (v0.1
   candidate), second demissio; persistence parked until a
   cold-start consumer exists.
9. **Type handle = single TypusC89 pointer.** Derived types
   (monstrator/acies/functio/qualificatus) interned — pointer
   equality is identity; qualified variants are interned wrapper
   nodes; tag types (structura/unio/enumeratio) are nominal:
   identity per declaration site, per C89.
10. **Names**: modules silva_c89_typus / silva_c89_scopus /
    silva_c89_forma, umbrella silva_c89_semantica →
    SilvaSemantica*; platform file **systema**; oracle instrument
    **haruspex**. (anima rejected — collides with silva.h's existing
    "anima semantica" section.)

## II. Architecture

```
SilvaParsura (committed tree)          systema_c89.h (lexicon)
        │                                     │ (praebenda/lexica)
        └────► silva_c89_semantica_analysare ◄┘
                        │
        ┌───────────────┼───────────────────┐
        ▼               ▼                   ▼
   scopus tables   typus interning     forma (lazy layout)
   (4 namespaces)  (+ consteval)       (size/align/offset)
        │               │                   │
        ▼               ▼                   ▼
   index (project) parallel tables     haruspex emitter
                   nodus→typus[+conv]  (generated TU; compile
                        │               IS the verdict)
                        ▼
              closure pass (silva_recanonicare,
              datum_resolutoris = semantica tables)
```

- **scopus**: scope stack; namespaces ordinaria (vars, functions,
  typedefs, enum CONSTANTS — one namespace), tags, membra
  (per-struct), labels (per-function). Tag completion pointers;
  sizeof(incomplete) = diagnostic.
- **typus**: canonical primitive table (14 valid multisets; char /
  signed char / unsigned char are THREE types), derived-type
  interning (dispersa-keyed), construction by declarator-chain
  unwind (internum walk; parenthesis transparent). Contains the
  **integer constant-expression evaluator** (simulation finding #1):
  literals, unary/binary/ternary, casts, char literals, enum
  constants, sizeof — mutually recursive with forma via a shared
  context; layout lazy/on-demand (cycles impossible in valid C89).
- **forma**: arm64 LP64, natural alignment, standard padding, union
  = max. No bitfields/packing in corpus (parks stand). macho.c is
  the host-ABI-faithfulness stakeholder; haruspex is the proof.
- **haruspex**: walks the type inventory → ONE generated TU of
  compile-time asserts (typedef char probatio_N[(sizeof(X)==N)?1:-1]
  + offsetof per DIRECT member — C89 has no anonymous members, so
  per-type direct asserts compose; nesting verified transitively).
  Partitioned by PROVENANCE (simulation finding #3): repo types →
  assert clang truth; systema types → assert opacity contract only
  (machinula rule recorded for M2: builtins never dereference
  systema-opaque types). Compile with clang -std=c89; exit code =
  verdict; nothing runs.
- **closure**: after tables are built, silva_recanonicare with a
  resolutor whose datum carries semantica's scoped tables — the
  existing SilvaResolutor interface is UNCHANGED (the vacuum* was
  waiting for this). Block-scope typedefs (12 in corpus, decisiones
  13) close here; systema dissolves the header-typedef mass.
- **Latina wrinkle: none** — semantica sees post-expansion vanilla
  streams; predefined macros already execution-grade (pre-M1).

## III. API sketch (vanilla spellings for the eventual silva.h rows)

```
SilvaSemantica* silva_c89_semantica_analysare(
    SilvaPiscina*, const SilvaParsura*, /* systema iam in contextu */
    ...);                     /* exact shape at INTENTIO */
const TypusC89* silva_c89_typus_declarationis(sem, nodus);
const TypusC89* silva_c89_typus_expressionis(sem, nodus);  /* M0b */
SilvaSymbolum*  silva_c89_symbolum_invenire(sem, scopus, chorda);
SilvaForma      silva_c89_forma_typi(sem, typus);  /* {magnitudo,
                                                      ordinatio} */
/* index: enumeration + lookup surface; haruspex as instrument
 * (instrumenta/principalia/haruspex.c + haruspex.sh) */
```
Semantica is c89-specific by nature (no grammar-agnostic pretense —
unlike quaestio).

## IV. Testing

- probatio_silva_semantica (new): the interview-pinned fixtures —
  qualifier-through-typedef (const IntPtr ⇒ int* const), the three
  chars, incomplete-tag completion, enum-constant collision, implicit
  function declaration (C89 extern int), array-size consteval with
  enum constant, self-referential struct via pointer, const-strip
  union, poison-type non-cascade, taken-arm-only fixture.
- percursus grows a semantica sweep mode: per-file diagnostics count
  (expect 0), ambigui-after-closure count (the 15k→~0 headline),
  symbol/type totals.
- haruspex over the corpus: the M0a wall. Suite + amalgam gates as
  always; silva.h rows + hospes calls per standing rule.

## V. Sequencing note

M0a is officina's critical path AND silva's own named milestone —
the projects pull the same thread from both ends. Quaestio QB/QC/QD
remain independent (interlocking, not competing): the :typus-flavored
pseudos would CONSUME SilvaSemantica when both exist.

## VI. Open at INTENTIO time (not design questions)

- Exact analysare signature (does systema ride the contextus the
  parsura already carried, or a separate param?) — v2 pass decides
  from the latina-datum wiring details.
- silva.h public rows for M0a: which query functions go public at
  M0a vs M0b (standing rule: each brings hospes calls).
- Whether percursus mode is a flag or a sibling instrument.

---

# V2 GROUNDING (2026-07-08 — three explorers + two empirical tiebreaks)

## VII. The ambigui census — what the 15,243 actually are

Census by discriminant (60-file sample over the non-outlier mass,
proportions stable; instrument: census_ambigui.c, see §X):

| cause | share | fixed by |
|---|---|---|
| C  repo-internal NON-types (funcs/vars/arrays) | ~84% | TRIVALENT oracle + non-type closure ONLY |
| A  C89 system typedefs (size_t, FILE, clock_t) | ~9.5% | systema typedefs (binary oracle suffices) |
| D  libc FUNCTION names (free, memcpy...) | ~6% | systema prototypes THROUGH the trivalent path |
| B  C99/POSIX names (uint32_t, DIR...) | ~0% | (corpus is C89-clean; nothing needed) |

One generated file dominates: lib/arbor2_glr_tabula.c holds 9,580 =
63% of ALL corpus ambigui, every one `magnitudo(repo_array)` — sizeof
type-vs-expr + cast-vs-call forks on repo non-types.

**Consequence — scope amendment recommended (Fran sign-off, §XI.1):**
the TRIVALENT oracle ("non-typus notus" — named as future work in
silva_commissio.h:36-54 since M1) moves INTO M0a. Semantica's scopus
pass is the natural non-type harvester: it already learns every
declared function/variable/array. Resolver gains the kill-the-
declaration-reading rule for known non-types. Without it the closure
demo collapses ~10%; with it + systema, ~98%.

**Named park (new):** ~2% nested-cast resolver limitation — repo
typedefs (i32/s32) stay retained in nested cast-in-call-arg shapes
(34 instances, e.g. lib/delineare.c ~L835) even with registration
fine. Neither systema nor trivalent clears it; needs a resolver/GLR
look. Landing spot: M0a stretch or its own small chunk after.

## VIII. Systema wiring — empirically settled

Agent contradiction (lexicon-channel typedef registration) settled by
direct experiment (scratchpad probo_lexicon_typedef.c, canonical
forking fixture `(size_t)(x);`):

```
basis (nihil)          ambigui=1 novit=0   (fork confirmed)
lexicon                ambigui=1 novit=0   <- LEXICON TYPEDEFS INERT
praebendum+#include    ambigui=0 novit=1   (works)
oraculum praeoneratum  ambigui=0 novit=1   (works)
```

Code truth (silva_parsare.c:1296-1298): lexica run through
directivas_processare and the non-directive remainder is DISCARDED —
"Reliqua lexici abiciuntur". Only #defines survive the lexicon
channel. census-percursus's contrary claim is refuted; its census
data (§VII) is unaffected and stands.

**Wiring DECISUS-candidate (Fran sign-off, §XI.2), two channels:**
- Typedefs → oracle PRELOAD: silva_systema_oraculum_praeonerare()
  from a generated name table (proven pattern:
  probatio_silva_c89.c:2245). Byte-exactness-inert, always-on.
- Prototypes/decls → channel (B): parse systema_c89.h ONCE as its
  own fons per SilvaSemantica session; harvest declaration vistas
  into semantica's tables; user parses stay pristine (systema = its
  own queryable layer — the layers-are-the-product pin). Channel (A)
  praebendum+#include works too (verified) but co-mingles systema
  tokens into user parses.
- Non-type names (category D) ride the same preload, once the
  trivalent oracle exists (silva_systema_non_typos_praeonerare or a
  unified praeonerare with kind marks).

Implementation facts recorded: the amalgamator datum emitter is
single-source-hardcoded (silva_amalgama.c:770 reads include/latina.h
unconditionally) — needs per-datum source generalization for a
second datum, or systema ships as plain committed .h text (no datum)
since channel (B) reads it as a file anyway — LEAN: no datum until
the amalgam itself needs embedded systema (machinula will, M2).
est_syntheticus is WRITE-ONLY today (no readers) — build the reader
only when hiding synthetic decls becomes a real need.

## IX. Typing-walk input contract (digest; full report in interview
notes — the authoritative loci table is silva_tabulae_c89.c:11400)

- Accessor on unfilled locus → SILVA_VALOR_NIHIL; always check genus.
  Lists ONLY via silva_valor_lista_numerus/_obtinere (prospectus
  views; xar_numerus overreads shared repositories).
- specificatores (LISTA_MIXTA): bare TOKENS for storage/qualifiers,
  typus-primitivus per MAXIMAL primitive run (a qualifier SPLITS
  runs: `unsigned const long` = [prim(unsigned), CONST, prim(long)]),
  typus-nominatus for typedef names, inline structura/unio/enumeratio
  tag nodes. typedef-ness = bare TYPEDEF token present.
- Declarator layers: walk internum to declarator-titulus; ABSTRACT =
  chain bottoms at NIHIL (aciei/functionis REUSED with internum
  NIHIL), not a distinct genus. parametrum has ONE declarator locus
  for abstract and concrete alike; ELLIPSIS rides bare in parametra.
- struct/union/enum def-vs-ref: discriminate on tok_aperta PRESENCE,
  NOT membra count (`struct S {}` is a definition with 0 membra).
  Anonymous = tok_titulus NIHIL.
- declaratio-nuda: type in specificatores, declaratores count 0
  (`struct S {...};`). definitio-functionis implicit-int forms leave
  specificatores NIHIL. K&R param types ONLY in declarationes-kr.
- AMBIGUUS appears as LIST ELEMENTS too (bitfield forks inside
  membra); canonical reading for decl-vs-expr forks is the
  EXPRESSION by policy — never assume canonical == declaration.
- conditionalis/ramus-sumptus wrap declarations — the walk recurses
  through contentum (taken arms); ramus-omissus = cruda, skip
  (DECISUS 6). ERROR nodes: skip, never type.
- Walk-order visibility (the sanatio invariant): register-before-use
  in ONE source-order pass; never compare byte_offset across fontes.
- **Initializer type-CHECKING → named M0b park** (tree gives congeries
  structure + values; binding brace shape to aggregate type is a
  semantic pass — typing-contract recommendation, adopted).

## X. Percursus + instruments

- percursus gains flags DEFAULT OFF (baseline 15,243 stays pristine):
  `-systema` (context injection) and the closure riding the parse
  path once trivalent lands; existing ambigui counter reports the
  after. Per-file closure fits percursus's loop; no sibling needed.
- percursus context recipe semantica must replicate: praebere ALL
  repo .h from repo root (inclusio vera — NO unconditional latina
  lexicon; files get latina only by real #include). Systema differs
  deliberately: modeling headers silva can never expand (<stdio.h>),
  via preload + channel (B), not praebenda.
- census_ambigui.c (silva/instrumenta/principalia/, uncommitted
  throwaway from the census agent): the before/after meter for the
  closure demo. Recommend: keep for M0a, delete at RELATIO (Fran's
  call at commit time).

## XI. Fran sign-offs — ALL APPROVED 2026-07-08

1. **Trivalent oracle into M0a scope** — APPROVED (the 84%+6%
   keystone; already silva-named future work; semantica is its
   natural harvester).
2. **Systema channel (B)** — APPROVED: parse-once-and-harvest +
   oracle preload (channel (A) verified working but not chosen).
3. **Nested-cast resolver caveat** — PARKED as recommended (~2%,
   34 known instances; landing spot: post-M0a resolver look).

M0a INTENTIO written to silva/phase-log.md same day.
