# Arbor — serialisatio canonica parsurae silvae (spec v2, codebase-grounded)

2026-08-19. Supersedes arbor-stml-spec.md (v1) wherever they differ.
v2 = v1 + four parallel explorer sweeps over the real code + seven
follow-ups, plus three decisions Fran made on measured facts that
v1 had assumed wrongly. Every claim below carries evidence.

Lineage: arbor-stml-visio.md (project, parcum 01M0DMSTEJ) →
arbor-stml-exhibita.md (hand-written candidates) →
arbor-stml-interview.md (12 decisions) → v1 → **this**.
Scope: **M1** (subtree serialization + round-trip gate +
representational audit).

## 0. What measurement changed (read this first)

| v1 assumed | measured | consequence |
|---|---|---|
| Lexeme and node namespaces disjoint by language | `assignatio` is BOTH node genus 35 and `SILVA_LEX_ASSIGNATIO`; `corpus` is both a genus and a locus name | **Lexeme tags gain a `lex-` prefix** (§2) |
| Serializer takes a subtree | fons table lives on `SilvaExpansio.fontes`; a bare `SilvaNodus*` cannot resolve `fons_index` | **`SilvaExpansio*` is a required parameter** (§7) |
| Pretty output is safe | pretty mode CORRUPTS text-with-element-siblings and COMPOUNDS per cycle (live probe) | **lib/stml.c fix is a prerequisite tranche** (§9) |
| Seal needs the house `sigillum` | `sigillum` is tool-side only, never amalgamated; `friatio_fnv1a` IS already in the amalgam, same TU | **FNV-1a, zero plumbing** (§6) |
| Comparator can be a probatio helper | silva/probationes has ZERO shared helpers by construction (each probatio its own TU) | **Comparator is fontes public API** (§8) |
| One round-trip oracle suffices | tree-equality is BLIND to trivia double-ownership (both owners look locally correct) | **Two oracles, disjoint failure classes** (§8) |
| stml writer might not emit fragments | it fully does, tested, including hand-constructed nodes | sharing mechanism works as designed |

## 1. The dialect (unchanged from v1 except as noted)

Name **arbor**; subtree envelope `<arbor>`, M2 full-document form
`<parsura>`. Envelope carries `grammatica="c89"` +
`registrum-sigillum="…"` + position anchor. Loader REFUSES loud on
seal mismatch. Document is a PROJECTION; the .c file remains truth.

## 2. Vocabulary (measured against the baked registry)

`SILVA_C89_REGISTRUM` = 58 genera, 176 locus rows (62 distinct
names), both counts real struct fields — the whole registry is
walkable from one `constans SilvaRegistrumCoctum*`
(silva_tabulae.h:100-105; instance silva_tabulae_c89.c:11698).

- **Node genus = element tag**, verbatim. VERIFIED as the shared
  vocabulary: quaestio's `_genus_invenire`
  (silva_quaestio.c:289-309) matches selector tags against
  `SilvaTabGenus.titulus` by `strlen`+`memcmp`, no aliasing, and an
  unknown tag is a hard compile failure ("tag registro ignotum",
  :924-932). **The thesis "registry titulus = selector tag = STML
  tag" is measured fact for node genera.**
- **Locus = wrapper element**, verbatim from `SilvaTabLocus.titulus`.
  Genus and locus elements strictly alternate by depth. NIHIL locus
  = omitted. Locus names are NOT globally unique (62 names over 176
  rows) — a locus element means something only relative to its
  parent genus; canon content models must be genus-scoped.
- **Lexeme = `lex-` prefixed element tag**: `NOMINA_GENERUM`
  (silva_token.c:354) lowercased, `_`→`-`, prefix `lex-` —
  `<lex-identificator>`, `<lex-paren-aperta/>`, `<lex-assignatio/>`.
  **Rationale (decision, 2026-08-19):** lexeme genera have never
  been in the shared vocabulary at all — quaestio cannot match token
  genera (it compares `token->valor`, the TEXT, at
  silva_quaestio.c:1050-1092; `NOMINA_GENERUM` is a separate table
  never wired into `SilvaRegistrumCoctum`, which has exactly two
  tables). The prefix therefore STATES the namespace boundary rather
  than hiding it, and removes a live ambiguity for any STML selector
  engine querying arbor documents.
- **INDEX loci**: arabic text content, `<canonica>0</canonica>`.
  `canonica` indexes BY POSITION into the interpretationes list,
  0-based (confirmed in two readers: silva_scribere.c:395-396,
  silva_commissio.c:404-413).

## 3. Tokens

Valor as text content (attributes cannot hold `"`), only for
variable-spelling genera; fixed-spelling genera are empty elements.
Trivia as `<ante>`/`<post>` with compact lenses. Scissurae as
`<scissura offset="N"/>`. `standard` and `f` (fons) attributes only
when non-default.

**The spelling table is a NEW artifact with a mandatory gate.** Silva
has a FORWARD keyword table (`VERBA_CLAUSA`, `hic_manens` in
silva_lexema.c:52) and, for punctuation, only an if-else recognizer
cascade (`_legere_interpunctionem`, silva_lexema.c:717) — **no
reverse genus→spelling mapping exists anywhere**. Arbor must author
one, which makes it a second source of truth that can drift from the
lexer. Gate at birth: for every fixed-spelling genus, lex the table's
spelling and assert the genus comes back. Silva implements NO
trigraphs (grep: absent), so spellings are genuinely 1:1 today; if
trigraph support is ever added the table stops being 1:1 and valor
must be carried — named dependency.

## 4. Derived fields — the fixup class (unchanged)

pater, initium_lineae, longitudo, and stratum-0 positions are
recomputed at load, never serialized. The law: **a canonical document
must not be able to lie.** The gate compares RECOMPUTED fields, so
the representational audit runs mechanically on every fixture.

`silva_token_ex_fonte` (silva_token.h:261) sets genus/valor/
byte_offset/longitudo/linea/columna/fons_index/standard/origo and
leaves `initium_lineae`, `spatia_ante`, `spatia_post`, `scissurae`
unset. Tokens are plain mutable structs; assigning those four
afterward is the ESTABLISHED house pattern, not a workaround
(silva_lexema.c:888, :976, :988, :350).

## 5. Whitespace — blocked on an stml fix (§9)

The v1 law (valor-bearing elements written tight, whitespace-only
text ignored at load) is **not achievable against today's writer**.
Measured: an element holding text plus element children takes the
mixed branch (lib/stml.c:4364-4388) which injects `\n` before the
first child and after every child unconditionally; the TEXTUS case
(:4400-4419) never indents, so real text ends up bracketed by bare
newlines that become part of its value. `<t><ante>x</ante>n</t>` →
pretty → reread yields valor `"\nn\n"`, and a second cycle yields
`"\n\nn\n\n"` — it compounds. "Ignore whitespace-only text" does not
save it: the corrupted node is not whitespace-only.

**Decision (Fran, 2026-08-19): fix lib/stml.c, do not route around
it.** Contract: a non-whitespace-only TEXTUS child is emitted
verbatim with no injected whitespace either side; whitespace-only
TEXTUS children stay skipped in pulchrum mode; element-ish children
keep newline+indent. Tracked as quaestio **01M0EDQ735BV**, linked
`impedit` → the arbor parcum. Until it lands, arbor writes
non-pretty (verified safe: non-pulchrum always emits verbatim,
lib/stml.c:4413).

## 6. The seal — FNV-1a, already vendored

`friatio_fnv1a` / `friatio_fnv1a_literis` are the ONLY two friatio
functions the amalgamator whitelists (`SERVANDA_FRIATIONIS`); SHA-1,
SHA-256, CRC32 and DJB2 are textually deleted at amalgamation. The
surviving pair appears in the amalgam as
`static i32 silva_friatio_fnv1a_literis(...)`
(silva/amalgama/silva.c:2513-2520) — `static`, but arbor lands in
the SAME translation unit, so it is directly callable with **zero
manifest edits, zero vendored lines**.

`sigillum` (SHA-256) is tool-side only — its lone caller
silva_differre.c is under `instrumenta/`, never amalgamated — so
using it would mean vendoring ~245 lines plus four hand-edits.
Rejected: **the seal's job is to make accidental grammar skew
impossible to MISS, not impossible to forge.** We perform ONE
comparison (document seal vs current registry seal), so a false
accept costs 2⁻³², not the birthday-bound figure that applies to
pairwise comparison across a set.

Implementation: walk `genera[]` then `loci[]` appending titulus +
int fields into a ChordaAedificator, one `friatio_fnv1a_literis`
pass. Note: no version/grammar-id field exists anywhere in the
tables — table CONTENT is the only version signal, which is exactly
what we want to hash.

## 7. Architecture

One generic reader + one generic writer, registry-driven at runtime
(precedent: silva_scribere's "ambulatio grammaticae-ignara"). Home
`silva/fontes/silva_arbor.{h,c}`. Inside `fontes/` we write **plain
`stml_*` / `Stml*` names against the root header** — the
`silva_stml_*` prefixing is applied automatically at amalgamation
and rewrites silva's own files too (proven: silva_annotationes.c's
`stml_legere` appears as `silva_stml_legere` at
silva/amalgama/silva.c:62474).

Required signature shape:
- writer takes `(Piscina*, constans SilvaNodus*, constans SilvaRegistrumCoctum*, constans SilvaExpansio*, InternamentumChorda*)` — **`SilvaExpansio*` is mandatory**, not optional: a bare node cannot resolve `fons_index` (the table is `SilvaExpansio.fontes`, silva_expandere.h:176; house access pattern is the direct cast at silva_scribere.c:729).
- `InternamentumChorda*` optional-with-lazy-create, following
  silva_annotationes.c:388-391 — `SilvaContextus` carries no
  internamentum.
- reader returns tree + refuse-loud vitium struct (causa + linea).
  Validates envelope, seal, genus/locus names, locus species vs
  content, valor presence vs genus class, and builds through the
  checked `silva_nodus_ponere` path (S32 write-once, species-checked,
  silva_nodus.c:212-242). **The loader must police `LISTA_MIXTA`
  element kinds itself** — `silva_nodus_appendere` validates only
  that the species is a list kind, never what goes inside.
- **`silva_committere(piscina, radix, tabularium, NIHIL, NIHIL,
  NIHIL)` after load**: a loaded tree is not queryable or
  re-canonicalizable until committed (`commissio->ambigui` is
  populated only by that walk). Bare commit with null oracle and
  resolutor is explicitly sanctioned (silva_commissio.h:163-165).

**Do NOT share silva_scribere's walk.** Its recursion is `interior`
with no visitor seam, and its token half is coupled to byte
reconstruction (origin unwinding, macro-extent matching, directive
splicing, offset dedup) that arbor has no use for. Shared walk
SHAPE, independent implementations. Registry name→index lookup has
no library helper either — four files already hand-roll it
(scribere:76, commissio:471, parsare:10, quaestio:289); arbor needs
two more, which puts a promoted helper well past the third-time
rule (**named door**, not M1-blocking).

Arbor DIVERGES from scribere on AMBIGUUS deliberately: scribere
emits only the canonical interpretation (silva_scribere.c:367-403)
because bytes can spell one reading. **Arbor emits all of them** —
this is the representational audit's first substantive catch:
byte-roundtrip is structurally incapable of preserving ambiguity.

Sharing: **preserve token identity** (Fran, 2026-08-19) via named
fragment at first use + `<<#id>>` at later uses. Verified available:
`stml_scribere` emits transclusion (lib/stml.c:4460-4467) and all
fragment forms (:4018-4104), with round-trip tests including the
hand-constructed-node path (probatio_stml.c:2815-2848); `StmlNodus`
is a transparent struct so arbor sets `genus`/`fragmentum`/
`fragmentum_id` directly. Nothing type-enforces sharing, but
`geometria_fida` is deliberately written to recognize it
(silva_nodus.h:226-232). **Named cost accepted**: committere's
pater fixup assigns from the last visit in walk order, so pater on
genuinely shared subtrees is walk-order-defined — parked twice
(phase-log:1356-1359, :5288-5295, "the first query consumer that
walks pater across interpretations owns the revisit"). Arbor may be
that consumer. Comparison stays deterministic (both trees walked
identically), so the gate is unaffected; the semantic question is
inherited, not created.

## 8. Gates (M1) — TWO oracles, disjoint failure classes

1. **Tree equality**: parse fixture → select subtree → serialize →
   load → compare structurally (genus, locus count/species, valor,
   trivia series, scissurae, standard, fons) AND the recomputed
   fixup fields.
2. **Byte round-trip through scribere**: load → `silva_scribere_nodum`
   → compare against the original subtree's emitted bytes.
   **This is not redundant.** Trivia single-ownership (each trivia
   token in exactly one `spatia_ante`/`spatia_post` list, tree-wide)
   is enforced NOWHERE at runtime: lex-time construction is disjoint
   only because one linear pass splits each run (silva_lexema.c:
   930-1010); the generator's check is static, over grammar SOURCE
   files (silva_generare.c:614-651); `ponere` never inspects trivia.
   A structural comparator is **blind to double-ownership** — both
   owners look locally correct — and only emission reveals it, as
   duplicated bytes. (Orphaned trivia shows as missing bytes.)
3. **Refusal gates, each with a PLANTED FAULT at birth**: seal
   mismatch, unknown genus, unknown locus, species mismatch, valor
   on a fixed-spelling genus, non-FONS token (fractura clara at the
   expansion boundary, mirroring scribere).
4. **Spelling-table gate** (§3): lex each table entry, expect its
   genus.
5. **Representational audit log**: every field that could not
   round-trip, named in the worklog.

**The comparator is fontes public API, not a test helper.**
`silva/probationes/` has zero shared helper files — every probatio
is an independent TU (compile_probationes.sh globs `probatio_*.c`)
and duplicates its `interior` helpers by convention. The nearest
existing thing, probatio_silva_puritas.c's `_relexatio_congruit`,
compares leaf tokens on genus+valor ONLY — far narrower than tree
equality. And the next consumer is already visible: mutatio's gates
ask exactly "did this transform preserve the tree".

**Corpus**: repo-root `probationes/fixa/roundtrip/` (NOT under
silva/) — 78 fixtures (73 .c + 5 .h, 3359 lines), each a scar from
a real past parser bug. Path via `RHUBARB_RADIX` env var, else
`".."` (probatio_silva_fidelitas.c:194-198). No reusable enumeration
helper exists — fidelitas inlines its `opendir` loop (:207-246); a
gate copies ~35 lines or factors it out.

**Subtree selection**: one-line compiled selector —
`silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM, "genus",
&causa)` + `silva_quaestio_exsequi(q, parsura->commissio->radix,
piscina)` (precedent probatio_silva_quaestio.c:44-58, :260; house
idiom in silva/quaestiones.stml). This makes the gate DOGFOOD the
shared vocabulary. Parse entry: `silva_c89_parsare(piscina, via,
fons, mensura, NIHIL)` — oraculum may be NIHIL.

Deferred with landing spots: mined pure subtrees via
est_fons_purus (M1-hardening); hand-authored golden trees (M3,
where authoring becomes a surface); origo/caecatio/conditional
regions (M2 `<parsura>`); canon projection (M2); registry-lookup
helper promotion (door).

## 9. Prerequisite tranche + integration choreography

**T0 — lib/stml.c pretty fix** (quaestio 01M0EDQ735BV, blocks
arbor's pretty output). TDD: the failing cases first, including a
TWO-cycle stability assertion (one cycle does not reveal
compounding). Today's only pretty test asserts "non-empty and
contains a newline" over an all-structural tree — a lying-green.
Blast radius under measurement before any edit: every
`pulchrum=VERUM` caller repo-wide, and whether any COMMITTED
generated artifact's bytes change.

**Amalgam choreography — a silent trap, and a proposed gate.**
`amalgamare.sh` does NOT regenerate its manifest; it rebuilds from
the committed `fontes_generata.h`/`excludenda_generata.h`. Adding a
fontes module and skipping the generators produces a silently
WRONG amalgam that passes its own gates. Required order: write
`silva_arbor.{h,c}` → `tools/amalgama_fontes_generare.sh silva` →
`tools/amalgama_excludenda_generare.sh silva` → extend
`silva/amalgama/silva.h` with the public prototypes →
`silva/amalgamare.sh`. (No `fontes_politica.sh` or `amalgamator.c`
edits needed — FNV-1a and stml are already vendored; those edits
would only have been required by the rejected sigillum route.)
Note `compile_probationes.sh` needs zero registration.
**Proposed (scope decision pending): a `-probare` freshness gate**
that re-derives the closure and refuses on drift, following the
`natura_canones.sh -probare` precedent. "There is no staleness
gate" is a finding that should leave a gate behind, not a paragraph.

## 10. Open (deliberately)

- Whether the manifest freshness gate is in THIS project or its own.
- Registry name→index helper promotion (fifth duplication).
- Transclusion's write path is tested but thinner than fragments'
  (no nested case, no write→read→write stability) and arbor will be
  its heaviest user — arbor's gates should close that.
- The pseudo-bridge limit sharpened: 1D selectors can ask WHETHER a
  locus exists (`vocatio[functio]`) but cannot descend into it by
  name — no locus combinator exists (silva_quaestio.h:103-109). So
  path-shaped 2D patterns that descend through named loci have no
  1D equivalent. Record in mutatio-visio §4.
