# md_arbor — spec (2026-09-03, from the interview)

> Status: v1.1 (2026-09-03, post-review: §5/§6 rewritten on the three ratified additions, §13 rulings, §14 decree). Written from `md-arbor-interview.md` (seven answers),
> the spike (`md-arbor-spica.md`), the census, and a read of the
> materia/css/stml_macros seams named below. Supersedes the draft
> plan's open questions; the plan (`md-arbor-plan.md`) is re-cut in
> §12. Deviations from earlier leans are marked **DEVIATION** and
> listed in §13 for Fran's review.

## 0. Identity

A markdown parser as materia's third client (`md/`, mirroring `css/`),
CommonMark 0.31.2 blocks and inlines plus GFM tables, task lists,
strikethrough and bare-URL autolinks, plus one house block (YAML front
matter). Byte-exact both ways over the house corpus (1,120 files);
projected to STML by materia's arbor writer; HTML produced by STML
template COMPOSITION over the projection as the second oracle against
the CommonMark spec examples; pythonica `Prosa` as the first consumer.

Not in v1: footnotes; math; definition lists; nested-language parsing
inside fences (2,540 C fences wait for the embedded-language hook);
sententiae addresses (a later consumer over this tree); the
`<PARSE>`/`<BUILD>` blocks (visio); STML namespaces (desideratum
01M1JNEN6N).

## 1. Decisions (the interview, verbatim in substance)

| # | question | decision |
|---|---|---|
| Q1 | recursion shape for md→html | ONE self-calling dispatcher; self-call legal only when every subtree argument is a STRICT subtree of the current instantiation's subtree argument (termination by descent); earlier non-recursive helpers stay ordinary macros |
| Q2 | h1..h6 | no element-from-value form; six-way switch written ONCE in an earlier helper that receives the transformed children as a subtree argument; composition tames the verbosity |
| Q3a | GFM | tables, task lists, strikethrough, bare-URL autolinks; footnotes stay text |
| Q3b | front matter | block genus `praefatio`, document start only, contents raw |
| Q4 | vocabulary | the table in §4, bare tags |
| Q5 | whitespace | zero-trivia line model (refined in §3 — **DEVIATION a**) |
| Q6 | html normalizer | C, inside the gate, forgives ONLY inter-tag whitespace and attribute order, forgiveness counted and reported |
| Q7 | Prosa addressing | selectio selectors over the projection + Python helper sugar |

Standing (from the plan, not re-opened): fences opaque; reference links
resolved in the parser; html bytes via `stml vertere`; two goldens per
CommonMark example; every gate born with a planted fault; runners
registered in pythonica `PORTAE` in the commit that creates them.

## 2. The materia facts this design rests on (read 2026-09-03)

- A node is `genus` + positional slots; slot kinds: NODUS, TOKEN,
  LISTA_NODUS, LISTA_TOKEN, LISTA_MIXTA, INDEX (`materia_nodus.h`).
  No string-valued slot exists: text lives ONLY in tokens.
- The emitter (`materia_scribere`) walks slots in layout order and
  emits every token's `valor` verbatim; it OMITS tokens whose
  `fons_index` differs from `consilium.fons_index` when that is ≥ 0
  (`materia_scribere.c:263`). Trivia (`spatia_ante/post`) are the only
  other bytes.
- The arbor writer writes an INDEX slot as a TEXT child (`<gradus>2
  </gradus>`), a token as an element whose tag is the lexicon prefix +
  genus title (content = valor for VERBATIM genera, nothing for FIXUM),
  `f="N"` on tokens from another source, and portable position
  (`b/linea/columna`) ONLY for tokens the origo hook declares
  non-source (`materia_arbor.c:1175`); with `origo = NIHIL` every
  token is source and positions are derived by the reader's cursor
  (`cursorem_movere` NIHIL = advance by valor length).
- Slots are written as named wrapper elements (css.canon: `<elementum
  nomen="praeludium" intra="regula-qualificata">`), so a value is
  matched as `<capitulum><gradus>2</gradus>…</capitulum>`, never as an
  attribute. The spike's `gradus="2"` shape was hand-written; §5's
  patterns use the real shape.
- `stml vertere` escapes ALL text (`&`, `<`), refuses an ambiguous
  entity in text (`&verbum;`), emits singletons as written, and has no
  tag-less raw splice (raw exists only as `<!script>`-style elements
  that still emit their tag).
- The macro engine (`lib/stml_macros.c`, 5,763 lines): templates
  `<#@f s="@s">`, args scalar or SUBTREE (block form `<@s>…</>`; a
  whole-text `&@x;` naming a caller's subtree threads it), COMMUTATIO
  in bodies (literal `est`, presence arms; a subtree scrutinee may use
  `non-nihil`; `est` vs subtree = vitium VII), EXEMPLAR/PER/CATENA/
  DIRIBITIO/SINE at document level and as chain links, captures →
  arguments (`_ligamina_ad_argumenta`: node captures are ORIGINAL
  pointers, cloned at splice), `tectum` = calls resolve only to
  definitions with ordo < the containing definition (a self-call is
  FRAGMENTUM_POSTERIUS today), `_exemplar_implere(nodus, ctx,
  stratum, tectum)` has NO argument map in reach (door 5 must thread
  it).

## 3. The tree — the line model (Q5 refined)

**DEVIATION a (from Q5's wording, same commitments).** Q5 said
container prefixes are "tokens of the container that consumed them".
The emitter is purely structural (container → child blocks), so a
container owning its `>` markers would emit them all before its
content instead of once per line. Therefore:

> **Every line is owned by exactly one LEAF block. Container markers
> (`>`, list markers, indentation) are PREFIX tokens of the line they
> appear on. Containers own no bytes.** No `spatia_ante/post` anywhere.

Consequences, all measurable:
- one owner per byte is BUILT (a byte is converted once, into one
  token, placed in one slot), not asserted;
- looseness is visible (blank lines are `linea-vacua` blocks in the
  container that was open, so `lista` sees them among its items);
- an EMPTY item or quote (`-`, `>`) contains one `linea-vacua` whose
  prefix holds the marker — no special case;
- the STML projection carries no trivia wrappers.

**DEVIATION b — the inline tree OWNS its bytes.** The plan's lean was
the css-B9 shape (analysis tree sharing tokens, no byte gate). Sharing
would put the same token in two emitted slots (bytes doubled) or force
token aliases into the projection. So for leaf blocks WITH inline
content (paragraph, heading, table cell) the block phase stores raw
content lines, and the inline phase REPLACES them with the inline tree
whose leaves own finer tokens over exactly the same bytes; line
endings inside a paragraph become `fractura-mollis`/`fractura-dura`
nodes that own the newline AND the next line's prefixes (lazy
continuation `> a\n> b` keeps its `> `). The inline layer is therefore
byte-gated by the same corpus gate as blocks — stronger than planned,
at no extra cost.

**The semantic channel — derived tokens.** CommonMark inline parsing
CONSUMES bytes (backslash escapes, entity decoding, code-span
normalization) and the transform has no expression language, and
`vertere` refuses raw `&copy;` in text. So a node whose meaning differs
from its bytes carries a DERIVED token: `fons_index = 1` (the
`<fontes>` table names source 1 "derivata"), `byte_offset = -1`, valor
in the parse arena. The emitter's `consilium.fons_index = 0` omits
them (byte round trip intact); the projection writes them with
`f="1"` and portable position via an md origo hook that reports
`est_fons = FALSUM` for source 1, and an md frons whose
`cursorem_movere` returns VERUM (moved nothing) for them. Derived
slots are PRESENT ONLY WHEN THEY DIFFER from the raw bytes (the
transform's arm order handles presence: pattern arm with `<valor>`
first, raw arm second). Uses: `textus.valor` (decoded text),
`nexus.url`/`nexus.descriptio` (resolved for reference links, unescaped
for inline links), `saeptum.lingua` (first word of the info string,
unescaped), `imago.url`/`descriptio`.

**The push-down law.** An argument is a subtree with no upward pointer,
so the transform can never ask about a node's PARENT. Every decision
that needs parent context is made by the PARSER and stored on the node
as an INDEX slot: `laxa` on `lista`, `nudus` on a paragraph that is the
direct child of a tight list item (html: no `<p>`), `officium` on a task
item, `ordinatio` on every cell. This is CommonMark's own habit
(tightness is a list property applied to paragraphs) made into a rule.

## 4. Registry — genera and slots (Q4)

Bare tags. Slot kinds in brackets; `†` = derived token (present only
when it differs); `#` = INDEX. Order = layout (emission) order.

**Blocks**
- `documentum`: praefatio[NODUS?] bloci[LISTA_NODUS] finis[TOKEN: EOF]
- `praefatio`: apertum[NODUS linea `---`] lineae[LISTA_NODUS linea] clausum[NODUS linea `---`]
- `linea`: praefixa[LISTA_TOKEN] contentum[TOKEN?] finis[TOKEN?: newline] — the generic raw line; `linea-vacua` is its blank-line genus (praefixa + finis only)
- `paragraphus`: praefixa[LISTA_TOKEN] nudus[#0/1: direct child of a TIGHT item] inlinea[NODUS]
- `capitulum`: praefixa · gradus[#1..6] · marca[TOKEN? `##` + spaces] · inlinea[NODUS] · clausum[TOKEN? closing `##`] · finis[TOKEN?] · subductio[NODUS? linea: the setext underline] (forma is derivable: setext ⇔ subductio present)
- `saeptum`: forma[#0 saeptum / 1 indentatum] · apertum[NODUS? linea: fence + info] · lingua[TOKEN†] · lineae[LISTA_NODUS linea] · clausum[NODUS? linea]
- `citatio`: bloci[LISTA_NODUS]
- `lista`: genus[#0 punctata / 1 numerata] · initium[#] · laxa[#0/1] · elementa[LISTA_NODUS elementum]
- `elementum`: officium[#0 nullum / 1 apertum / 2 perfectum] · bloci[LISTA_NODUS] (the marker and a task box are prefixes of its first line)
- `divisio`: linea[NODUS linea]
- `tabula`: caput[NODUS ordo] · separator[NODUS linea] · ordines[LISTA_NODUS ordo]
- `ordo`: praefixa[LISTA_TOKEN] · cellae[LISTA_NODUS cella] · clausum[TOKEN? trailing pipe+spaces] · finis[TOKEN?]
- `cella`: ordinatio[#0 nulla / 1 sinistra / 2 media / 3 dextra] · apertum[TOKEN pipe+spaces] · inlinea[NODUS]
- `html`: lineae[LISTA_NODUS linea] (opaque, CommonMark's seven start conditions)
- `definitio-nexus`: lineae[LISTA_NODUS linea] · titulus[TOKEN†] · url[TOKEN†] · descriptio[TOKEN†] (kept in place, bytes intact; consumers ignore it)

**Inline** (under `inlinea`, whose one slot is liberi[LISTA_NODUS])
- `textus`: crudum[LISTA_TOKEN] · valor[TOKEN†]
- `emphasis` / `fortis` / `deletio`: apertum[TOKEN delimiter run] · liberi[LISTA_NODUS] · clausum[TOKEN]
- `verbatim`: apertum[TOKEN backticks] · crudum[LISTA_TOKEN] · valor[TOKEN†] · clausum[TOKEN]
- `nexus`: forma[#0 inlineus / 1 referens / 2 auto] · apertum[TOKEN `[` or `<`] · liberi[LISTA_NODUS] · cauda[LISTA_TOKEN: `](…)` / `][ref]` / `>` bytes] · url[TOKEN†] · descriptio[TOKEN†]
- `imago`: as nexus with apertum `![`
- `fractura-mollis`: finis[TOKEN newline] · praefixa[LISTA_TOKEN next line's markers]
- `fractura-dura`: signum[TOKEN trailing spaces or backslash] · finis · praefixa
- `html-inlineum`: crudum[LISTA_TOKEN]
- an unresolved reference, a stray delimiter, a literal `[`: `textus` (CommonMark semantics)

**Lexicon (token genera, prefix `md-`)**: FINIS, LINEA (newline —
munus LINEA, so the reader's cursor and FIDELIS comparator work, unlike
css), INDENTATIO, MARCA-CITATIONIS, MARCA-PUNCTI, MARCA-NUMERI,
MARCA-OFFICII, MARCA-ATX, CLAUSURA-ATX, SUBDUCTIO, SAEPTUM, INFO,
DIVISIO, PIPA, TEXTUS (raw run), DELIMITATOR (`*`/`_`/`~` runs),
GRAVIS (backtick run), EFFUGIUM (`\x`), ENS (`&…;`), NEXUS-APERTUM,
NEXUS-CAUDA, AUTONEXUS, HTML, SPATIA-FINALIA, DERIVATUM (the source-1
value token). Count asserted against the table by TITLES (css
lesson).

## 5. The STML projection (A8) and the html program (B2)

Projection = `materia_arbor_scribere_nodum` with an md
`MateriaArborConsilium` (registry, lexicon, grammar name "md", origo
hook + frons for derived tokens, `sedes_colligere`). A paragraph
`Hello *w*\n` inside a quote projects as (wrappers are materia's own —
every slot is a named element, as css.canon shows):

```stml
<paragraphus>
  <praefixa><md-marca-citationis b="0" …>&gt; </md-marca-citationis></praefixa>
  <nudus>0</nudus>
  <inlinea><liberi>
    <textus><crudum><md-textus>Hello </md-textus></crudum></textus>
    <emphasis><apertum><md-delimitator>*</md-delimitator></apertum>
      <liberi><textus><crudum><md-textus>w</md-textus></crudum></textus></liberi>
      <clausum><md-delimitator>*</md-delimitator></clausum></emphasis>
    <fractura-mollis><finis><md-linea/></finis><praefixa/></fractura-mollis>
  </liberi></inlinea>
</paragraphus>
```

**The wrappers are the interface.** A node's slots are what the
registry declares; the projection names them; the transform reads them
by name through the slot projections of §6.1. So `&@n.gradus;` is the
heading level and `&@n.inlinea;` is the heading's content forest — the
registry is the API of the projection (the css lesson generalized),
and the canon judges that API.

The html program `md/html/md-html.stml` (B2), the shape decided by Q1
and Q2, ratified 2026-09-03 with the three additions:

```stml
<!-- helpers first (strata law) -->
<#@html-h g="@g" corpus="@corpus">
  <COMMUTATIO de="&@g;">
    <CASUS est="1"><h1>&@corpus;</h1></CASUS>
    … <CASUS est="6"><h6>&@corpus;</h6></CASUS>
  </COMMUTATIO>
</#>
<#@html-textus n="@n">
  <COMMUTATIO de="&@n;">
    <CASUS><EST><EXEMPLAR><textus><valor/></textus></EXEMPLAR></EST>&@n.valor;</CASUS>
    <ORDINARIUS>&@n.crudum;</ORDINARIUS>
  </COMMUTATIO>
</#>

<!-- the ONE self-calling dispatcher: one arm per genus, one line each -->
<#@md-nodus n="@n">
  <COMMUTATIO de="&@n;">
    <CASUS tag="documentum"><PER de="@n.bloci" voca="#@md-nodus"/></CASUS>
    <CASUS tag="capitulum">
      <<#@html-h g="&@n.gradus;">><@corpus=><PER de="@n.inlinea" voca="#@md-nodus"/></>
    </CASUS>
    <CASUS><EST><EXEMPLAR><paragraphus><nudus>1</nudus></paragraphus></EXEMPLAR></EST>
      <PER de="@n.inlinea" voca="#@md-nodus"/></CASUS>
    <CASUS tag="paragraphus"><p><PER de="@n.inlinea" voca="#@md-nodus"/></p></CASUS>
    <CASUS tag="citatio"><blockquote><PER de="@n.bloci" voca="#@md-nodus"/></blockquote></CASUS>
    <CASUS><EST><EXEMPLAR><lista><genus>1</genus></lista></EXEMPLAR></EST>
      <ol start="&@n.initium;"><PER de="@n.elementa" voca="#@md-nodus"/></ol></CASUS>
    <CASUS tag="lista"><ul><PER de="@n.elementa" voca="#@md-nodus"/></ul></CASUS>
    <CASUS tag="elementum"><li><PER de="@n.bloci" voca="#@md-nodus"/></li></CASUS>
    <CASUS tag="saeptum"><pre><code class="language-&@n.lingua;">&@n.lineae;</code></pre></CASUS>
    <CASUS tag="divisio"><hr/></CASUS>
    <CASUS tag="inlinea"><PER de="@n.liberi" voca="#@md-nodus"/></CASUS>
    <CASUS tag="textus"><<#@html-textus>><@n=>&@n;</></CASUS>
    <CASUS tag="emphasis"><em><PER de="@n.liberi" voca="#@md-nodus"/></em></CASUS>
    <CASUS tag="fortis"><strong><PER de="@n.liberi" voca="#@md-nodus"/></strong></CASUS>
    <CASUS tag="deletio"><del><PER de="@n.liberi" voca="#@md-nodus"/></del></CASUS>
    <CASUS tag="verbatim"><code>&@n.valor;</code></CASUS>
    <CASUS tag="nexus"><a href="&@n.url;" title="&@n.descriptio;"><PER de="@n.liberi" voca="#@md-nodus"/></a></CASUS>
    <CASUS tag="fractura-mollis">&#10;</CASUS>
    <CASUS tag="fractura-dura"><br/>&#10;</CASUS>
    <ORDINARIUS/>   <!-- markers, prefixes, definitions, praefatio, tables (B2 adds): nothing -->
  </COMMUTATIO>
</#>

<EXEMPLAR modus="unum" output="$doc"><documentum $d/></EXEMPLAR>
<PER congruentia="$doc"><<#@md-nodus>><@n=>&@d;</></PER>
```

Open details B2 measures rather than decides here: `&@n.lineae;` on a
fence must yield the raw line bytes as text (a forest of `linea` nodes
would need its own arm); `title=` must be OMITTED when the description
is absent (an arm pair, or an "absent slot = attribute dropped" rule —
usability data); table arms; tight lists (`nudus` on the paragraph,
pushed down by the parser — §3's law). The saeptum arm above stands in
for that decision; if slot projection of a `lineae` list of `linea`
nodes cannot become text without an expression, the parser gives fences
a derived `valor` token (the semantic channel again).

## 6. Engine increment B1 — five pieces in `lib/stml_macros.c`, all from existing reservations

Each gets a fixture in `probationes/fixa/exemplaria/` with expected
output pinned, a vitium, and INDAGO coverage; the existing L1 / lint II
/ codex-69 gates stay byte-identical.

1. **Slot projections on subtree arguments — `&@n.slot;`** (ratified
   2026-09-03). The engine already reserves dotted slot names as sparse
   forms ("'.' INCLUSUM consulto … formae sparsae reservatae"). Meaning:
   `n` must be a SUBTREE argument whose single root is an element; `slot`
   names a CHILD ELEMENT of that root (a materia wrapper); the projection
   yields the wrapper's TEXT as a scalar when the wrapper holds only
   text, else the wrapper's content FOREST. Paths chain by LITERAL
   child steps — every segment names a child element of the element
   the previous segment reached, no hidden hop: a NODUS slot wraps
   the genus element, so the heading's inline children are
   `&@n.inlinea.inlinea.liberi;` (the tree as it is; the program never
   chains through a NODUS slot — PER over `@n.inlinea` hands the genus
   element to the next fill, whose `&@n.liberi;` is one step). A missing wrapper = loud vitium
   `LOCULUS_PROIECTIO_ABSENS`, naming argument and slot (a wrapper that
   is present but empty yields the empty forest — that is what "absent
   derived slot" looks like in §3, and arm order handles it). A forest
   in a string position = the existing vitium VII.
   **Ratified 2026-09-03 (Fran, after the token-slot review):** the
   path names a wrapper and yields its CONTENT, never the wrapper
   itself (the element `<c>text</c>` is the parent's content; choosing
   among same-tag siblings is a PATTERN's job, piece 2, never a path's).
   Text content is a scalar with nothing converted. Raw (`!`) elements
   are bytes by declaration but stay elements: `&@n.crudum!;` — the
   raw suffix glued to the reference, the registry's own sigil in its
   own position — yields the bytes of text and raw leaves concatenated;
   `!` is allowed on any projection as an assertion (loud vitium VII
   if structure appears). No implicit element→text rule exists. A
   flattening verb `<TEXTUS>` (DOM textContent semantics) is RESERVED
   in the visio, not built: md's needs are met by paths and by the
   parser's derived tokens (a heading's plain text for an anchor id
   would be a derived slot, never an engine flattening).
2. **`de="@arg"` and `de="@arg.slot"` (exemplaria door 5, with a path).**
   Inside a fill, EXEMPLAR/CATENA/DIRIBITIO/PER may name a subtree
   argument, or a slot projection of one, as scope. The argument map is
   threaded into the exemplaria path (today `_exemplar_implere` cannot
   see it). Scalar or absent = loud.
3. **PER over a forest with delegation — `<PER de="@n.bloci"
   voca="#@md-nodus"/>`** (ratified 2026-09-03). Each element of the
   forest becomes ONE row whose single capture is the delegated
   template's sole subtree slot (a template with more than one subtree
   slot, or none, refuses: `PER_DELEGATIO_AMBIGUA`). Inline body form
   also allowed (`<PER de="@n.bloci" ut="x">…&@x;…</PER>`) when no
   delegation is wanted. This is the child loop in one line.
   **As built (2026-09-03, B1.3):** rows are the ELEMENTS of the
   projected forest (text and comments are not rows); the delegated
   template's row slot is its sole REQUIRED slot (optional slots stay
   unfilled), and delegation is a true call — a fresh argument table,
   the caller's scope does not leak. The `ut=` form fills within the
   same instantiation: the outer scope stays visible, the row name is
   local to the PER body (a reference outside it is LOCULUS_IGNOTUS),
   and a row name that shadows a declared slot is refused (XIII). An
   EMPTY wrapper (`<elementa/>`, the empty scalar of §6.1) is a source
   with zero rows, never a fault. `!` in `de=` is refused: the source
   is a forest, not bytes.
4. **Pattern cases in COMMUTATIO (door 7, fill plane) + `tag=` sugar.**
   `<CASUS><EST><EXEMPLAR>…</EXEMPLAR></EST>arm</CASUS>` with a SUBTREE
   scrutinee: the pattern is petitioned `ancorata` at the scrutinee's
   root, the arm is taken on a match, and the pattern's captures flow
   into the arm as arguments (residue 5, decreed here for the fill
   plane). `<CASUS tag="capitulum">` = literal equality on the root's
   tag, no captures (inside THE LINE: enumerable structure). `est=`
   literal arms keep working for scalars. First match wins; ORDINARIUS
   last; no match and no ORDINARIUS = CASUS_NULLUS.
   **As built (2026-09-03, B1.4):** the pattern is FILLED with the
   current arguments before matching (`<p id="&@k;"/>` is a
   parameterized pattern — the bridge idiom in the fill plane), then
   petitioned anchored at the scrutinee's single element root; its
   captures (node `$q`, value `attr="$v"`, text `<t>$x</t>`) join the
   arm's argument table beside the outer arguments (a name already in
   the table = CAPTURA_COLLISA; a capture naming a declared slot is
   refused at collection). A tag or pattern arm against a scalar WITH
   text is vitium XXX ARGUMENTUM_SCALARE, the mirror of VII; the empty
   scalar of §6.1 has no root and simply does not match; an absent
   optional scrutinee matches neither. `<EST>` holds exactly one
   attribute-less EXEMPLAR with a one-element body; EST is consumed,
   never emitted.
5. **Self-call as a CHECK, not a spelling.** A call (or a `voca=`) whose
   id is the definition being filled (ordo == tectum) is admitted iff
   every subtree argument of the call is a strict descendant (pointer
   containment in the original tree) of some subtree argument of the
   current instantiation; otherwise `RECURSIO_NON_DESCENDENS`, loud,
   naming the template. Through piece 3 the condition holds by
   construction; the check stays so an explicit self-call with a
   non-descending argument cannot loop. Mutual recursion stays illegal.
   The existing `stratum` depth cap remains the last guard.

**§1.3 extents — DEFERRED (DEVIATION c).** The rule says nothing that
WRITES may ride matches until extents survive expansion. The md→html
transform GENERATES a fresh document; no consumer reads source
positions off its output (Prosa reads positions from the parser's own
projection). The obligation stays decreed and is pulled by the first
MUTATION consumer. If Fran wants it built here, it is one task
appended to B1.

**Raw html passthrough (B3 item).** `vertere` has no tag-less raw
splice, so html blocks and inline html cannot be emitted as bytes. B3
adds a decreed raw form to `stml_html` (proposal: `<!crudum>` =
children bytes as written, no tag, lint-visible) OR the html-block
sections are reported as known-red in the pass count. Decided at B3.

**Usability log (the experiment's real product).** B2 keeps a section
in `md/fontes/md_arbor.worklog.md` titled "quid STML voluit": every
place the program is verbose, needs parent context, fights a wrapper,
or wants a form the engine lacks — with the workaround used. Findings
feed the exemplaria interview file and the visio, per Fran's 2026-09-03
steer (this is exploratory design; a program that works but reads badly
is a finding).

## 7. Gates (every one red at birth by a planted fault)

| gate | oracle | exit condition |
|---|---|---|
| `probatio_md_registrum` | enum ↔ table by titles; lexicon count | permutation caught |
| `probatio_md_lexema` | line lexer round trip over ALL 1,120 files | 1,120/1,120 bytes; coverage asserted (files read > 0, bytes > 0) |
| `probatio_md_arbor` | parse → emit on fixtures; structure pinned | clean files: 0 `mala`, headings/lists counted > 0 |
| `probatio_md_corpus` | parse → emit over 1,120 files + every CommonMark/GFM example input | byte-exact all; blank/indented-code/setext counts REPORTED (the census heuristics get measured here) |
| `probatio_md_inlinea` | inline structure pinned for chosen examples; emphasis algorithm cases from spec §6.2 | structure equal; still byte-exact (inline owns bytes) |
| `probatio_md_stml` | write → read → write twice, STRUCTURALIS and FIDELIS comparators | documents I/II identical, trees equal, derived tokens round-trip with `f="1"` |
| `probatio_md_canon` | `md/grammatica/md.canon` hand-written, drift guard both ways, sigillum pinned | rule-less genus red; stale sigillum red |
| `probatio_md_totalitas` | random bytes, mutated/truncated corpus, depth | never NIHIL/crash; byte-exact on garbage |
| `probatio_md_computus` | `md/computus.sh` twin, css columns | golden `fixa/computus/basis.tsv` |
| `probatio_md_html` (B3) | spec.txt examples → parse → project → expand → vertere → C normalizer → compare | pass count pinned and only rising; failures listed by section; forgiveness counted |
| exemplaria fixtures (B1) | spike documents + one per feature, expected output pinned | L1/lint II/codex 69 gates unchanged |
| pythonica gate (C1) | `Prosa` edits by heading and list item; refusals name lines | green; stale-read refused |

Runner `md/compile_probationes.sh` = the css runner with `md`
substituted (mensor prefix `md.`, log `build/test_logs/md.log`),
REGISTERED in pythonica `PORTAE` as `md` (signal `MD PROBATIONES:
\d+/\d+`) and in `commissio_umbra`'s prefix map in the same commit.

**The CommonMark corpus**: vendor `md/probationes/fixa/commonmark/
spec.txt` (0.31.2) and `…/gfm/spec.txt` (0.29-gfm, only its
extension sections are judged: Tables, Task list items,
Strikethrough, Autolinks extension). ONE C reader of the spec.txt
example form (32-backtick fence + ` example`, `.` separator, `→` =
tab, sections from `#` headings). No JSON. Version and byte count
pinned in the gate.

## 8. The instruments (`md/*.sh`, css pattern, mensor-wired)

- `md/arbor.sh <x.md> [-tacitus]` — the projection document.
- `md/extenta.sh <x.md> '<selector>'` — selectio over the projection;
  prints TSV `via  selector-index  b-initium  b-finis  linea  columna`
  per match by walking the matched element's tokens (first/last `b`);
  the formator `-extenta` twin that `Prosa` reads.
- `md/html.sh <x.md>` — projection + program → `vertere` bytes.
- `md/computus.sh <x.md> [-machina] [-iter N]` — the bench twin.

## 9. Arc C — `Prosa` in `pythonica/silva.py`

`Prosa(via)` = `Textus` (stale-read guard, all-or-nothing write, no
formatter/examen) + structural anchors:
- `.selecta(selector)` → list of `Extentum` via `md/extenta.sh`;
- sugar: `.capitulum(titulus, gradus=None)` (selector `capitulum` +
  text match on the heading's textus values), `.elementum(n, intra=)`,
  `.saeptum(lingua=, n=)`;
- `.substituere(sel_or_extentum, text)`, `.inserere_post/ante`,
  `.corpus(sel)`; refusals name lines and counts like `Editio`;
- `.applicare()` re-parses the result and asserts byte round trip and
  that every anchor used still resolves (the judge markdown can have,
  since any text is valid markdown).
Registered gate: the pythonica suite edits a copy of a real spec by
heading and by list item.

## 10. Order and dependencies

A2 (registry, lexicon, line lexer, runner) → A3 (leaf blocks) → A4
(containers) → A5 (tables, definitions, resolution) → A6 (corpus gate,
interrogated) → A7 (inline tree, derived tokens) → A8 (projection with
origo/frons, canon, totality, computus). B1 (engine) can start after
A3 on the spike documents; B2 needs A8's real projection; B3 needs B2.
C1 needs A8 + `extenta.sh`; C2 closes.

## 11. Measurements the gates must publish (not assert)

- indented-code blocks found in the corpus (census heuristic: 1,082
  candidates) — eyeball ten before trusting green;
- setext headings vs thematic breaks vs front-matter fences among the
  3,842 underline-shaped lines;
- derived tokens per file (decoded text, resolved links) and their
  share of projection bytes;
- forgiveness count in `probatio_md_html` by kind (whitespace,
  attribute order).

## 12. The plan, re-cut

A1 census + this spec (done by this commit) · A2 · A3 · A4 · A5 · A6 ·
A7 · A8 (four gates) · B1 (four engine features + fixtures) · B2
(program) · B3 (oracle + raw passthrough decision) · C1 · C2. About 20
tasks; A7 and B1 are the hard ones.

## 13. Review record — deviations and calls (Fran, 2026-09-03)

Ratified: the three engine additions (§6.1, §6.3, §6.4 `tag=`) and the
sense/form decree (§14). The items below carry my recommendation as the
DEFAULT; each is one line to overturn.

- **a.** Line model refined: lines are nodes, container markers are line
  prefixes (emitter order). Same commitments as Q5. — default: keep.
- **b.** Inline tree owns bytes (byte-gated), not a shared-token analysis
  view. — default: keep.
- **c.** §1.3 extent provenance deferred to the first mutation consumer.
  — default: defer.
- **d.** Derived tokens (source 1) as the semantic channel; md origo hook
  + frons — the C89 seam reused. — default: keep.
- **e.** CommonMark + GFM vendored as two `spec.txt` files with one C
  reader; no JSON. — default: keep.
- **f.** Raw html passthrough: `<!crudum>` in `vertere`, or html-block
  sections known-red. — decided at B3 with the first failing example in
  hand.
- **g.** Front matter: `praefatio` + `lineae`; no key/value projection.
  — Fran's answer.
- **h.** Parent context pushed down at parse time (`nudus`, `laxa`,
  `officium`) — the law of §3. — default: keep.
- **i.** (2026-09-03, B1 start) Slot projections yield wrapper CONTENT;
  raw elements become bytes only with the explicit `!` suffix
  (`&@n.crudum!;`); `<TEXTUS>` flattening reserved, not built. —
  Fran: "i would go either way, whichever you think makes the most
  sense" → explicit `!` chosen (intent on the page, no heuristic).

## 14. Decree — html SENSE and FORM (2026-09-03)

Two html trees exist and each is canonical for one job:

- the **FORM** tree is the materia html projection (next client):
  tokens, whitespace, entity spellings, quote styles; byte-exact;
  describes a file that exists;
- the **SENSE** tree is the DOM-shaped STML tree that `stml vertere`
  consumes: elements = tags, attributes, text; no bytes of its own;
  the shape generation wants.

Laws: `vertere` IS the sense→form synthesis for html (the `<BUILD
lang="html">` verb of the visio without its block syntax yet); the html
parser MUST provide a form→sense projection whose output for a
document equals what md→html produces for the same content (the
html5lib tree-construction fixtures are DOM-shaped, so the parser needs
that projection for its own oracle anyway); md→html targets SENSE; the
form tree for generated html is reached only by md → sense → vertere →
html parser → form, which becomes a third oracle once the parser
exists. A direct md→form transform is refused: it would hand-synthesize
every `<`, `>` and quote as tokens and gain no oracle.

Syntax seams this locates in `vertere` and the B3 normalizer, not in the
tree choice: void elements (`<hr>` vs `<hr />` — one start tag to the
html lexer, named forgiveness); entity spelling (decode both sides, as
cmark's normalizer does, named forgiveness); the doctype `vertere`
prepends (a fragment option on `vertere`, not a forgiveness); `<pre>`
content (whitespace is meaning — the inter-tag forgiveness switches OFF
inside it); raw html blocks (§6, B3); boolean attributes (`checked`,
GFM emits `checked=""`).
