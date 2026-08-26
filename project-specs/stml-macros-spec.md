# STML Macros v1 — Parameterized Fragments (expansio)

2026-08-25. Interview: `stml-macros-interview.md` (raw transcript,
four rounds). Lineage: stml-visio.md §9.1 (the decreed pins),
arbor-parsura-spec.md §4 (the blessed `&@` slot design), visio §6
(the RESERVATUM this fulfills). Consumer: arbor (decided round 1).

Fran's definition anchors scope: **macros = fragments that take
arguments.** V1 is exactly that — no query fragments (staged), no
children-args (reserved), no cross-document (reserved).

## 0. What measurement decided

- **The transclusion lexer already carries arguments.**
  `_tok_legere_transclusio` (lib/stml.c:1189) scans `<<` … `>>`
  blindly and stores the whole interior as `valor` — so
  `<<#f versio="2">>` lexes TODAY; the interior grammar is the
  expander's to parse. Writer emits `<<` + valor + `>>` back —
  arguments round-trip byte-exact with zero writer changes.
- **Fragments already parse attributes** (`nodus->attributa`,
  lib/stml.c:3206) — slot declarations (`<#f position="@position">`)
  ride the existing parse.
- **`&@name;` in text is literal today** (unknown-entity passthrough,
  visio §3) and attribute values are raw both ways — the expander
  interprets `&@…;` at expansion time; the parser never learns it.
- **One real parser change**: the `>>` close-scan is not quote-aware,
  so an argument value containing `>>` would truncate the token.
  Upgrade the scan to skip quoted spans. Existing corpus unaffected
  (a quoted `>>` inside `<<…>>` is today a broken document).

## 1. Doctrine (inherited, restated once)

Files are truth: **the file keeps the macro form.** Expansion is a
pure tree→tree projection at load (`legere → expandere → consumer`),
in the caller's piscina; the original tree is untouched and stays
queryable (the strata decree); the writer only ever sees unexpanded
trees. No computation in templates: no conditionals, loops,
defaults, or expressions — substitution and (later) queries are
structural. Every reference failure is LOUD. Tree-level substitution
with provenance, never string-level.

## 2. Surface

### 2.0 Template-space sigil `#@` (decided 2026-08-26 — the arbor forcing case)

Fragment ids beginning `@` are TEMPLATE-space; everything the rest
of this spec describes (collection, dropping, filling, vitia,
provenance) applies to template-space ONLY. Fragments without the
sigil (`<#lex1>`, `<#>`) are CONTENT fragments and the engine never
touches them: the definition stays in the content view, its
transclusions stay unresolved nodes, no vitium fires.

WHY: the first consumer (arbor, T7) exposed that one surface carried
two opposite semantics. Arbor's `<#lexN>` + `<<#lexN>>` is
TRANSCLUSION in the original (Nelson) sense — aliasing: one object,
many windows, identity preserved ("identitas res est, duplicatio
mentiretur" — silva_arbor.c). A macro call is INSTANTIATION: fresh
copy per site, divergent under arguments. Blanket expansion would
have deleted every shared lexeme's first use and cloned every
identity. The split is not a workaround, it is a correction: the
NEW thing (templates) carries the mark; bare `<<#id>>` keeps meaning
what transclusion always meant. Doctrine: **transclusio = alias
(consumer-resolved, identity); templum = instantiatio
(engine-resolved, `#@`)**. The interview's "same apparatus" claim is
hereby narrowed to template space.

Mechanics: the `@` is PART of the id (`@f` is interned; definition
`<#@f>` and call `<<#@f>>` match on it). The stml lexer accepts `@`
only as the FIRST character of a fragment id.

### 2.1 Definition side (parsura §4, blessed as-is; re-spelled `#@` 2026-08-26)

```stml
<#@lex-zephyrum position="@position">
  <lex-integer position="&@position;">0</lex-integer>
</#>
```

- Slots are DECLARED on the fragment's opening tag (`attr="@name"`),
  never inferred — a typo cannot mint an empty slot.
- `&@name;` = template-space reference (the three-space carve gains
  a fourth mark INSIDE the `&…;` delimiter: `#` document · `&x;`
  world · `.` kind · `&@x;` template).
- Body positions: attribute value (whole or interpolated —
  `via="&@basis;/x.c"`), and text/children position.
- `&@...children;` spread: RESERVED (semantics banked in parsura §4),
  not in v1.
- Define-side and use-side stay visually distinct (`&@x;` appears
  only inside definition bodies) — the surviving `:`/`::` discipline.

### 2.2 Call side (decided round 2)

```stml
<<#@lex-zephyrum position="123">>
```

Transclusion-with-arguments: interior = `#@id` + attribute-syntax
pairs. `<#@id>` stays purely definitional; invocation is
reference-shaped. A no-argument call to a slotless TEMPLATE is plain
splice-resolution by the same machinery — but content-space
transclusion (`<<#lex1>>`, `<<selector>>`) passes through as an
unresolved node for the CONSUMER to resolve (§2.0: alias semantics
cannot be served by a cloning engine).

### 2.3 Strata (decided round 2)

Document order. A call may reference only fragments defined EARLIER
in the document; a body may therefore only call earlier macros.
Forward reference = vitium. Termination by construction, zero graph
machinery. Nested calls expand recursively during the fill; a nota's
`stratum` = fill-recursion depth.

### 2.4 Vitium taxonomy (all loud, first error wins; template-space only per §2.0)

| vitium | trigger |
|---|---|
| `FRAGMENTUM_IGNOTUM` | call to an id defined nowhere |
| `FRAGMENTUM_POSTERIUS` | call to an id defined LATER (strata violation) |
| `FRAGMENTUM_GEMINUM` | two definitions with the same id |
| `LOCULUS_NON_IMPLETUS` | declared slot the call did not fill |
| `ARGUMENTUM_SUPERFLUUM` | call argument naming no declared slot |
| `LOCULUS_IGNOTUS` | body references a slot not declared |

`&@x;` OUTSIDE any fragment body stays literal text (the entity
passthrough rule; the parsura canon will refuse it per-dialect —
canon gating is the reservation, not the expander's job).

## 3. Semantics

One left-to-right walk over the document:

1. `<#id>` definition encountered → recorded (id → node), DROPPED
   from output (round 3: the expanded tree is the CONTENT view;
   definitions remain queryable in the unexpanded tree).
2. Transclusion whose valor starts `#` → parse interior (id + args,
   quote-aware), look up (must already exist), judge slots both
   directions, CLONE the body with substitution, splice in place of
   the call, record a nota. Bodies containing calls recurse (depth =
   stratum).
3. Everything else → cloned verbatim. Non-`#` transclusions pass
   through as nodes.

Cloning: fresh nodes in the caller's piscina (originals immutable;
the token-sharing-forbidden lesson from mutatio simulatio I).
Interned chordae are shared by pointer — immutable, safe.
Substitution in attribute values replaces `&@name;` spans within the
leaf string; in text positions the reference is replaced by the
argument's text. Argument values are STRINGS in v1 (attribute
syntax carries no trees) — tree-valued arguments arrive with
children-args, reserved.

## 4. API (lib/stml_macros.c — new TU, round 3)

Depends only on the public stml.h tree surface (which this build
proves sufficient). Sketch, refinable at implementation:

```c
nomen structura {
    StmlNodus* nodus;          /* radix insertionis in arbore expansa */
    chorda*    fragmentum_id;
    StmlNodus* vocatio;        /* nodus transclusionis in arbore ORIGINALI */
    i32        stratum;        /* profunditas impletionis, I-basata */
} StmlExpansioNota;

nomen structura {
    b32         successus;
    StmlNodus*  radix_expansa;     /* arbor nova; originalis intacta */
    Xar*        tabula_expansionum; /* StmlExpansioNota, ordine splicis */
    /* vitium: genus + linea + fragmentum/loculus nominati */
} StmlExpansioResultus;

StmlExpansioResultus
stml_expandere (StmlNodus* radix, Piscina* piscina,
                InternamentumChorda* intern);
```

The side table is the sedes precedent: StmlNodus untouched, queries
join the table. Nota granularity = splice ROOT (descendants implied;
inner splices carry their own notas).

## 5. Gates

- **probatio_stml_macros.c** (born-red first, house pattern):
  attribute fill · interpolated fill · text fill · nested call
  (stratum II asserted) · zero-arg slotless call (= transclusion
  resolution) · definitions dropped · original tree untouched
  (pre/post comparison) · tabula asserted (count, ids, strata,
  vocatio identity) · all six vitia · quote-aware `>>` fixture ·
  unexpanded roundtrip byte-exact (writer never sees expansion).
- **Arbor slice** (round 4: in this milestone): the emitter authors
  ONE shared shape as fragment + calls in a generated parsura
  document; the loader expands on read; the M2 byte-gate holds on
  the macro-form file; the loaded semantics equal the unshared form.
- **Amalgam duty**: new lib → fontes_generare → excludenda →
  CADENDA_TYPEDEF for the two new public types → hospes exercise →
  amalgamare VERIFICATUM. New probatio →
  tools/compile_tests_fontes_generare.sh.

## 6. Reserved, with landing spots

| what | where it lands |
|---|---|
| ~~`&@...children;` + call-site children~~ | **RETIRED (decree 2026-08-26)** — superseded by NAMED argument-elements (§6.1 below). The `&@...x;` spread surface stays permanently refused (LOCULUS_IGNOTUS) — now as a dead form, not a reservation |
| query fragments | visio §9.1; forces the STML-side query engine; subsumes conditional inclusion, arms retained |
| cross-document libraries | the declared-world arc (visio §5); v1 is same-document only |
| selector transclusion resolution | RESERVATUM continues (needs the selector engine) |
| canon gating of loculi | parsura-canon work; expander stays consumer-opt-in |
| attribute-NAME sugar `<lex-x &@position;/>` | parsura §4 — reserved, never required |

### 6.1 Argument-form equivalence (decreed 2026-08-26, undesigned in detail)

Every call gets two EQUIVALENT spellings the body never distinguishes:
inline (`<<#@f attr="v">>`, scalars) and BLOCK — argument-elements as
immediately-following siblings, tag `@`-prefixed `=`-suffixed:

```stml
<<#@f>>
<@attr=>123</>
<@type=><some-tag><child/></some-tag></>
```

A tag-form argument's value is its CHILDREN — arguments generalize
from strings to subtrees, which answers the children-slot question
with NAMED subtree arguments (Fran: better in general than anonymous
nesting; positional spreads retired above). Decisions bound:

- **Association**: a call consumes the maximal run of
  immediately-following argument-elements. Self-delimiting — no
  capture parens (arity-brittle, the M2b churn class), no `<</>>`
  closer (`<<#@f>>` is already a complete call). Lexer only blesses
  the `<@attr=>` name form (precedent: `<% &clavis;>` — ordinary
  ELEMENTUM + marker field, consumers intact).
- **Equivalence lives in the ENGINE's argument map**, never the
  parser — fidelity keeps the authored spelling byte-exact; the
  emitter chooses per site (scalars inline, subtrees block).
- **Seventh vitium**: a subtree argument filling an ATTRIBUTE
  position refuses loud — never silent stringification. Slots stay
  untyped; body positions constrain.
- **Canon**: argument-elements are part of the call, not content of
  the parent (third touch in the template-space doctrinal line).

### 6.2 COMMUTATIO — arm selection in template bodies (decreed 2026-08-26, unbuilt)

**CAPS-Latin tags are hereby the BUILTIN-VERB grammar**: data tags
stay lowercase kebab; template-layer builtins SCREAM. Zero new
lexing; every future builtin has a home without spending a sigil.
(`<!switch>` rejected — `<!` is comment/DOCTYPE lexer territory.)

First builtin — value-driven arm selection inside definition bodies:

```stml
<COMMUTATIO de="&@con-stella;">
  <CASUS est="verum"><tok_stella>&@stella;</tok_stella></CASUS>
  <CASUS nihil/>
  <ORDINARIUS>...</ORDINARIUS>
</COMMUTATIO>
```

- **THE LINE (the no-expression-language law, restated for arms):
  cases are LITERALS (`est="..."`) or PRESENCE-tests (`nihil`,
  `non-nihil` boolean attrs) — never predicates over values.** Every
  arm's body is statically declared, so a future canon can judge
  every alternative before expansion ("judge the plan" survives).
  When richer cases arrive they arrive as PATTERNS (enumerable,
  judgeable), never expressions. CASUS-vs-subtree-argument matching
  is the same lockstep matcher as emitter recognition and future
  query patterns — one machinery, three seams.
- **What it uniquely buys**: conditional WRAPPERS. (Conditional
  CONTENT is already free — an empty forest argument splices
  nothing.) Collapses structural-variant axes in fragment families
  (par-stella/par-planus → one fragment, one arm choice). Tradeoff
  noted: the emitter matcher must match THROUGH switches (try arms);
  flat families may READ better — decide per family, not by law.
- **Optional slots (the hidden contract change)**: `nihil` arms
  require absence to be legal → optional declaration `p="@p?"`, and
  the NARROWING rule keeps it loud: an optional loculus referenced
  OUTSIDE an arm establishing its presence = collection-time vitium
  (flow-narrowing, the honest optionality — never silent empty).
- **Exhaustiveness**: no arm matches and no ORDINARIUS = loud
  fill-time vitium. Never silent emptiness.
- First-match-wins in document order; ORDINARIUS last.
- Surface details (attr spellings, vitium numbering) adjustable at
  build time; the LINE and the CAPS-builtin mint are the decree.

### 6.3 Attribute-elements on ALL tags (decreed 2026-08-26, unbuilt) — a BASE-format repair

`<@attr=>` generalizes beyond calls: any element may spell any
attribute in element form. This lands in lib/stml.c (base grammar),
not the macro layer — it repairs a documented hole (a double-quote
in an attribute value is irrepresentable; text children have
escaping) and gives long values real layout.

**The two bindings, made non-overlapping by position:**

1. PARENT-binding: attr-elements must be a PREFIX of the child list
   (they belong to the tag's head, before all ordinary children).
2. CALL-binding: argument-elements immediately follow their call
   (§6.1, unchanged — calls are atomic transclusions).
3. Any other position = LOUD vitium, never a guess. Deterministic,
   zero lookahead. Capture arity counts attr-elements as ordinary
   children (`<hello ((>` = attr-element + one real child).

**The data-model line**: ordinary elements' attributes remain
STRING-valued (StmlAttributum.valor stays a chorda). Attr-elements
on ordinary tags therefore take TEXT-ONLY children — element
children there are a vitium. Subtree values remain a macro-call
power, living in the engine's transient argument map only.

**Empty = TOMBSTONE (decreed 2026-08-26)**: an attr-element with
zero children (`<@m=/>`, and the pair spelling `<@m=></>` which the
writer normalizes to it — MEASURED: the base writer already
canonicalizes ALL empty pairs to self-closing, `<a></a>` emits
`<a/>`) means the attribute is explicitly ABSENT — capere returns
NIHIL, canon judges it missing (though the NAME is still checked
against the element's canon entry — tombstoning an unknown
attribute is still speaking about it), and any future attribute
INHERITANCE (template-provided attrs, DISTRIBUTIO carry-down) is
blocked by it. Motivation (Fran): nodes need a delete verb, not
just override, if attrs ever flow down from templates/wrappers —
this strengthens DISTRIBUTIO's leaned item-wins collision rule.
The empty-STRING value has no element spelling and needs none:
inline `m=""` always represents it (the element form exists for
values that NEED it — quotes, layout — and an empty value never
does). Tombstone + inline same name = the duplicate-attribute
vitium (present-and-absent is a contradiction).

**Implementation postures fixed now:**
- Fidelity via the augmentation pattern: the attr-element stays a
  child node with a marker, NEVER normalized into attributa at
  parse; `stml_attributum_capere` grows a second lookup path
  (equivalence free for accessor users; direct attributa-Xar
  iterators get a one-time audit). Same name in both forms = the
  existing duplicate-attribute vitium.
- Canon: two judgment postures keyed by what the element is — on
  CALLS argument-elements are invisible (quoted call-material,
  §6.1); on ORDINARY elements they are judged AS attributes (name
  against the element's canon entry, value genus checked). Fourth
  canon touch in the template-space doctrinal line.

## 7. Milestone plan

1. Quote-aware `>>` scan (one lexer touch + fixture).
2. lib/stml_macros.c: interior parser (id + args) → definition
   collection → clone-with-substitution → notas. Gates born red,
   then green.
3. Amalgam + suite wiring.
4. Arbor slice: pick the shared shape (lex-zephyrum-class), emitter
   authors it, loader expands, byte-gate + semantics-equality green.
5. Bookkeeping: visio §9.1 pointer flips to VIVIT-partial, parsura
   §4 "reserved" note gains the pointer here, ledger res for the
   milestone.
