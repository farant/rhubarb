# Genera C89 — vocabularium (M2.0 NOMINA)

**STATUS: DECISUS (2026-07-04) — reviewed and approved by Fran.
This is the pinned vocabulary; changing a genus tag after rules
are authored is an API break (Eskil's rule applies).**

This document names every C89 construct's node genus (the
kebab-case tag in grammatica/c89.stml) plus the locus naming
conventions. These tags are the quaestio selector surface,
solarium's query language, and saltuarius's TOC rows — the
thousand-year names. Alternatives and reasoning are recorded so
decisions keep their why. English prose, Latin names.

Reading test for every candidate (per the M2 sketch): say it
aloud as a TOC row and as a selector. `definitio-functionis`
must sound right in both mouths.

Legend: **bold** = recommended. *(pinned)* = already shipped in
sceletum.stml or spec §9.3 — carried, not reopened.

---

## 0. Policies (decide once, apply everywhere)

**P1 — latina keywords as genus tags.** Genus tags are STML
strings; every generated C identifier is silva_-prefixed
(accessors like `silva_si_conditio`); macro expansion fires only
on whole tokens. So `si`, `redde`, `frange`, `structura`,
`magnitudo` are SAFE as tags and as accessor-name components.
Proposal: **the latina keyword IS the genus tag for keyword
constructs** — source reads `redde x;`, tree reads genus
`redde`. Maximal coherence between what Fran types and what he
queries. The six-firing prohibition on bare latina words in
hand-written C code is untouched.

**P2 — one genus per semantic shape, unified across
productions** *(pinned, S20)*. Grammar factoring (precedence
tiers, dangling-else) never leaks into the tree. All twelve
binary-operator tiers → one `binarium`.

**P3 — operator identity lives in the token, not the genus.**
"Find all `&&`" is `binarium` + operator filter (the token slot
carries the SILVA_LEX_ genus), not a per-operator genus. Keeps
the genus vocabulary ~55 names instead of ~90; selector
ergonomics for specific operators is the selector engine's job
(`:op()`-style pseudo, post-M2).

**P4 — folium- prefix for literal leaves** *(pinned in
sceletum)*. Distinguishes the literal `5` (folium-integer) from
the keyword `int` (typus-primitivus) at a glance in queries.

---

## 1. Radix — the per-file tree root

| construct | prior name | candidates |
|---|---|---|
| translation unit | TRANSLATION_UNIT (arbor2), summum (lapifex) | **truncus** / radix / unitas-translationis |

**truncus**: silva is the forest, each file's tree stands on its
trunk; rami (conditional branches) and folia (leaves) already
exist — the botanical system completes itself. `radix` would
echo STML (`StmlNodus* radix`), but silva's root-of-file is a
different thing from STML's root-of-document, and the collision
in cross-layer queries (solarium will query BOTH trees) argues
for distinct words. `unitas-translationis` is standard-faithful
but a mouthful nobody will type.

Loci: `elementa` (lista: declarations, function definitions,
and structural citizens in source order).

---

## 2. Expressiones (M2a)

| construct | prior name | genus | loci |
|---|---|---|---|
| comma expr `a, b` | virga (lapifex) | **virgula** | sinister, tok_operator, dexter |
| assignment (11 ops) | assignatio | **assignatio** | sinister, tok_operator, dexter |
| ternary `c ? a : b` | ternarius | **ternarius** | conditio, tok_quaestio, verum, tok_colon, falsum |
| binary (all 12 tiers) | binarium *(pinned)* | **binarium** | sinister, tok_operator, dexter |
| cast `(t)x` | conversio | **conversio** | tok_apertum, typus, tok_clausum, internum |
| unary prefix `!x &x *x -x ~x ++x --x` | unarium | **unarium** | tok_operator, internum |
| postfix `x++ x--` | post_unarium | **postcrementum** | internum, tok_operator |
| sizeof expr | sizeof | **magnitudo-expressionis** | tok_verbum, internum |
| sizeof(type) | sizeof | **magnitudo-typi** | tok_verbum, tok_apertum, typus, tok_clausum |
| call `f(a,b)` | vocatio | **vocatio** | functio, tok_apertum, argumenta (lista), tok_clausum |
| subscript `a[i]` | subscriptio | **subscriptio** | basis, tok_apertum, index, tok_clausum |
| member `.` / `->` | membrum (arbor2) | **accessus** | basis, tok_operator, tok_titulus |
| identifier | folium-identificator *(pinned)* | **folium-identificator** | tok_valor |
| integer literal | folium-integer *(pinned)* | **folium-integer** | tok_valor |
| float literal | float | **folium-fluitans** | tok_valor |
| char literal | char | **folium-character** | tok_valor |
| string literal | string | **folium-chorda** | tok_valor (lista? see note) |
| parenthesized | parenthesis *(pinned)* | **parenthesis** | tok_apertum, internum, tok_clausum |

Notes:
- **virgula** not lapifex's `virga` (rod/twig): virgula is the
  actual Latin for the comma mark (whence "virgule").
- **assignatio ≠ binarium**: assignment is semantically distinct
  (lvalue target, sequencing) and "find all assignments to X" is
  a first-class query. Distinct genus, same slot shape.
- **postcrementum**: post + crementum (growth — real Latin; the
  lexeme genera are already INCREMENTUM/DECREMENTUM). Covers both
  ++ and -- via the token. Alternative: unarium-postfixum.
- **magnitudo split in two genera** (not one like arbor2): the
  `sizeof(t)` vs `sizeof (expr)` fork is an M2a named ambiguity —
  when both readings live under an AMBIGUUS node, distinct genera
  make the interpretations distinguishable in queries. P1 word.
- **accessus** for member access frees **membrum** for the
  member DECLARATION (§3) — arbor2 overloaded MEMBRUM; the query
  surface wants "find accesses of .campus" and "find the
  declaration of campus" to be different questions.
- **folium-chorda**: chorda is THE house word for string. C89
  adjacent-literal concatenation ("a" "b"): one folium-chorda
  whose tok_valor is a lista of STRING_LIT tokens. **DECIDED
  M2a Chunk A** — expressible in annotation v0 with zero format
  changes (hidden modus-lista rule collects the tokens; the
  genus production takes the lista in one slot). tok_valor stays
  the locus name across ALL folia (query uniformity); its slot
  species alone differs (lista-token here, token elsewhere).

---

## 3. Declarationes (M2b)

| construct | prior name | genus | loci |
|---|---|---|---|
| declaration | declaratio *(pinned)* | **declaratio** | specificatores (lista), declaratores (lista), tok_terminator |
| primitive type spec | typus-primitivus *(pinned)* | **typus-primitivus** | tok_verba (lista: `unsigned long int`) |
| typedef-name use | typus-nominatus *(pinned)* | **typus-nominatus** | tok_titulus |
| struct specifier | struct_specifier | **structura** | tok_verbum, tok_titulus?, tok_aperta?, membra (lista), tok_clausa? |
| union specifier | (union) | **unio** | (same shape as structura) |
| enum specifier | enum_specifier | **enumeratio** | tok_verbum, tok_titulus?, tok_aperta?, enumeratores (lista), tok_clausa? |
| enumerator | enumerator | **enumerator** | tok_titulus, tok_operator?, valor? |
| member declaration | struct_member | **membrum** | specificatores (lista), declaratores (lista), tok_terminator |
| bitfield declarator | (3.5 lesson) | **campus** | declarator?, tok_colon, latitudo |
| name declarator | declarator-titulus *(pinned)* | **declarator-titulus** | tok_titulus |
| pointer declarator | declarator-monstrator *(pinned)* | **declarator-monstrator** | tok_stella, qualificatores (lista), internum |
| array declarator | (array) | **declarator-aciei** | internum, tok_apertum, mensura?, tok_clausum |
| function declarator | declarator_functi | **declarator-functionis** | internum, tok_apertum, parametra (lista), tok_clausum |
| abstract declarator | (abstract) | **declarator-abstractus** | (same shapes, titulus absent) |
| init-declarator `x = v` | init_declarator | **declarator-initiatus** | declarator, tok_operator, initiator |
| braced initializer `{...}` | INITIALIZOR_LISTA | **congeries** | tok_aperta, elementa (lista), tok_clausa |
| type-name (in cast/sizeof) | nomen_typi | **species-typi** | specificatores (lista), declarator? |
| parameter declaration | parameter_decl | **parametrum** | specificatores (lista), declarator? |
| function definition | definitio_functi | **definitio-functionis** | specificatores (lista), declarator, declarationes-kr (lista), corpus |

Notes:
- **structura / unio / enumeratio** are the latina keywords (P1).
  TOC row reads perfectly: `structura SilvaLexema`. Anonymous
  form: tok_titulus unfilled (NIHIL).
- **campus** (field!) for the bitfield struct-declarator — short,
  real Latin, and "bitfield" IS "field" in C culture. latitudo =
  the width expression.
- **acies** for array: classical Latin for an ordered
  arrangement (battle line) — short, distinctive, unclaimed in
  the codebase (xar is the dynamic array; C arrays are a
  different thing and deserve a different word). Alternatives:
  series, ordinata.
- **ADDENDA M2b Chunk A (approved 2026-07-04)**: (1) the
  parenthesized declarator `(*f)` REUSES genus `parenthesis`
  (identical loci; "find parenthesized things" queries
  uniformly across expressions and declarators). (2) Storage
  classes and qualifiers ride the specificatores lista as bare
  TOKENS interleaved with typus nodes. (3) typus-primitivus =
  one MAXIMAL RUN of primitive keywords (`unsigned long` = one
  node, verba II) — but a qualifier SPLITS runs: `unsigned
  const long` = [typus-primitivus(unsigned), CONST,
  typus-primitivus(long)]. The run-grouping is grammatical
  (two-state specifier lists), so it can never see through
  interleaved tokens. (4) Abstract array/function declarators
  reuse declarator-aciei / declarator-functionis with internum
  absent; declarator-abstractus itself remains the star-chain
  genus, per the DECISUS table.
- **congeries** for the braced initializer: an aggregate/heap —
  and braced initializers initialize AGGREGATES. Colorful but
  precise. Alternatives: lista-initialis, initiatio-composita.
  (arbor2's INITIALIZOR is a barbarism we retire.) The scalar
  `= expr` form needs no genus — the expression rides in the
  initiator locus directly.
- **species-typi** for C's "type-name" avoids `nomen-typi`
  (nomen is P1-safe but needlessly loaded — it is the single
  most-fired latina landmine) and avoids overloading
  typus-nominatus. species = the appearance/form of a type
  without a declared name. Alternative: designatio-typi.
- **parametrum**: Greek loan, but standard scientific Latin and
  universally readable. K&R identifier-list params: the separate
  declarations ride in definitio-functionis's declarationes-kr
  lista.
- **AMENDMENT PROPOSED (2026-07-04, M2c Chunk B — awaiting
  Fran)**: K&R identifiers ride as ordinary parametrum readings
  (typus-nominatus / declarator-titulus — the same GLR fork as
  prototypes, resolved or honestly retained by the oracle), NOT
  as folium-identificator as this note originally said. Reason: a
  dedicated folium-identificator production in parametra would
  add a THIRD reading to every single-identifier parameter
  everywhere (prototypes included) — permanent fork pollution for
  a naming nicety. The presence of a non-empty declarationes-kr
  lista is what marks a definition as K&R; a vista/query-level
  interpretation can present params-as-names wherever the kr
  lista names them. Path back: the production is additive if the
  query surface ever demands the distinct genus.
- **typedef gets NO distinct genus** (proposal): a typedef is a
  declaratio whose specificatores contain TYPEDEF — grammar
  reality. The oracle detects it at segment commit from the
  token, and "find all typedefs" becomes a selector pseudo or a
  semantic header field, not a genus. Alternative (rejected but
  recorded): definitio-typi genus — would fork declaration
  handling in the grammar for a query-surface convenience.
- Ellipsis `...` in parameter lists: rides as an interleaved
  token in the parametra lista (separated-list rule), no genus.

---

## 4. Sententiae (M2c)

| construct | prior name | genus | loci |
|---|---|---|---|
| compound block | corpus | **corpus** | tok_aperta, elementa (lista), tok_clausa *(amended 2026-07-04, see note)* |
| expression stmt | sententia-expressionis *(pinned)* | **sententia-expressionis** | expressio, tok_terminator |
| empty stmt `;` | sententia_vacua | **sententia-vacua** | tok_terminator |
| if / if-else | si | **si** | tok_verbum, tok_apertum, conditio, tok_clausum, consequens, tok_alioquin?, alioquin? |
| while | dum | **dum** | tok_verbum, tok_apertum, conditio, tok_clausum, corpus |
| do-while | fac | **fac-dum** | tok_fac, corpus, tok_dum, tok_apertum, conditio, tok_clausum, tok_terminator |
| for | per | **per** | tok_verbum, tok_apertum, clausula, tok_clausum, corpus |
| for-clause *(spec-pinned real node)* | per_clausula | **per-clausula** | initium?, tok_terminator_i, conditio?, tok_terminator_ii, passus? |
| switch | commutatio | **commutatio** | tok_verbum, tok_apertum, discrimen, tok_clausum, corpus |
| case group | casus | **casus** | tok_verbum, valor, tok_colon, sententiae (lista) |
| default group | ordinarius | **ordinarius** | tok_verbum, tok_colon, sententiae (lista) |
| labeled stmt | titulatum | **titulatum** | tok_titulus, tok_colon, sententia |
| goto | salta | **salta** | tok_verbum, tok_destinatio, tok_terminator |
| break | frange | **frange** | tok_verbum, tok_terminator |
| continue | perge | **perge** | tok_verbum, tok_terminator |
| return | redde | **redde** | tok_verbum, valor?, tok_terminator |

Notes:
- All keyword statements take the latina keyword as genus (P1).
  The accessor reads become sentences: `silva_si_conditio(n)`,
  `silva_redde_valor(n)`, `silva_per_clausula(n)`.
- **si is ONE genus** for both if and if-else (P2 — the
  dangling-else factoring disappears; unfilled alioquin = NIHIL).
  The else-arm locus is named **alioquin** — the latina keyword
  for else, so the accessor is `silva_si_alioquin`.
- **passus** for the for-loop's third clause (step): mille
  passus — the Roman mile is a thousand steps. Alternatives:
  gressus, incrementum.
- **discrimen** for the switched-on expression: the deciding
  difference. Alternatives: selectum, subiectum.
- **Switch bodies are GROUPED** (decided 2026-07-04, Fran):
  casus/ordinarius OWN the statements that follow them, until the
  next casus/ordinarius — the tree-sitter shape, not the ISO
  labeled-statement shape. "Everything in case 1" is plain
  children; P2 already establishes that grammar factoring never
  leaks into the tree. Fall-through `casus 1: casus 2: x;` =
  first group's sententiae empty, second holds x. Roundtrip
  unaffected (loci order = byte order holds). Grammar mechanics
  (keeping the group list from swallowing the next label) are an
  M2c INTENTIO item. titulatum (goto labels) stays ISO-shaped —
  grouping is switch-specific.
- **AMENDED (2026-07-04, Fran, M2c INTENTIO)**: corpus carries
  ONE lista (elementa, mixing declaratio | sententia — mirrors
  the segment level exactly). The original two-lista shape
  (declarationes, sententiae — "mirrors the standard's grammar")
  put the declaration-vs-expression fork readings in DIFFERENT
  listas, so GLR stacks could not re-merge until the whole
  corpus reduced: the AMBIGUUS wrapper would cover the entire
  block and the element-wise resolutor would go blind. One lista
  keeps forks per-element and the oracle machinery unchanged —
  the standard's factoring leaking into tree shape is exactly
  what P2 forbids. C89's declarations-before-statements ordering
  becomes a LINT query; "this block's declarations" = genus
  filter over elementa.

---

## 5. Structuralia (genera-extra — carried registry)

All *(pinned)* from sceletum.stml / spec §9.3; c89.stml's
`<genera-extra>` must be a SUPERSET of what the M1 skeleton
registry actually ships (verify at authoring time):

- **error** — tokens:lista
- **ambiguus** — interpretationes:lista, canonica:index
- **conditionalis** — rami:lista, finis:lista
- **ramus-sumptus** / **ramus-omissus** — directiva, contentum/
  cruda, conditio_id
- **commentum**, **directivum** — named in spec §9.3's registry;
  confirm their M1-skeleton slot shapes and carry verbatim.

---

## 6. Loci — naming conventions (pinned by this doc)

- **tok_ prefix for every terminal locus** *(pinned sceletum)*.
  Role-named after the underscore: tok_operator, tok_apertum /
  tok_clausum (round), tok_aperta / tok_clausa (brace — gender
  distinguishes paren from brace, matching PAREN_APERTA /
  BRACE_APERTA), tok_terminator (;), tok_verbum (keyword),
  tok_titulus (name), tok_colon, tok_quaestio, tok_stella.
- **Child loci are semantic Latin nouns**, never positional:
  sinister/dexter, internum (the wrapped/inner thing), conditio,
  corpus, basis (the thing indexed/accessed), index, functio,
  argumenta, typus, declarator, initiator, valor, mensura
  (array size), latitudo (bitfield width), consequens/alioquin,
  initium/passus, discrimen, elementa.
- **Lists are always lists** *(pinned)*: modus lista-initium /
  lista-appendere; separated lists interleave separator tokens
  with elements in ONE ordered list; unfilled slot = NIHIL.
- Genus unification across productions = slot union; kind
  conflicts are generation errors *(pinned, §9.3)*.

---

## 7. Decisiones (the naming session, 2026-07-03/04)

All nine agenda questions resolved with Fran:

1. **P1 APPROVED** — latina keywords are genus tags (si, redde,
   structura, magnitudo...). Blanket approval of the draft.
2. **P3 APPROVED** — one binarium, operator identity in the
   token slot.
3. **Root = truncus.**
4. **Array = acies.**
5. **Braced initializer = congeries.**
6. **accessus/membrum split approved.**
7. **typedef: semantic mark, NO distinct genus** (explicit
   sign-off 2026-07-04). One declaratio genus; the oracle
   detects TYPEDEF at segment commit; "find typedefs" = selector
   pseudo / semantic header field. Path back if wrong: a genus
   split would be ADDITIVE (new tag, rules re-annotated), not a
   tag rename.
8. **All taste-pass names approved** as drafted (postcrementum,
   campus, passus, discrimen, virgula, species-typi).
9. **Grammar file: c89.stml FRESH** (explicit sign-off
   2026-07-04) — own baked symbols; sceletum.stml FROZEN as the
   generator's fixture; saltuarius switches grammars at M2d.
10. **Switch bodies GROUPED** (explicit sign-off 2026-07-04,
    preview compared) — see §4 note.
11. **corpus = ONE lista** (explicit sign-off 2026-07-04, M2c
    INTENTIO, preview compared) — DECISUS amendment; see §4
    AMENDED note. Path back if wrong: additive (a two-lista
    vista/query view is derivable from elementa; the grammar
    shape itself is what the amendment fixes).
12. **`<praelatio>`** (explicit sign-off 2026-07-04) — the
    grammar element declaring a table-time preference resolution
    for a named terminal/action pair; first use = dangling-else
    (ELSE binds innermost — a language rule, so deterministic
    resolution is CORRECT). Census discipline extends: cellae
    praelatae are a pinned census category, enumerated like
    conflict cells.
13. **Block-scope typedefs DEFERRED BY NAME** (explicit sign-off
    2026-07-04) — registration stays file-scope; block-local
    typedef uses stay AMBIGUUS-retained (never resolved wrong).
    Evidence gate: count block-local typedefs over solarium +
    rhubarb at the M2d sweep; landing spot M2d or lint.
