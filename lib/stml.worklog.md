# stml.c worklog

## 2026-07-20 — raw line capture `<tag! (>` (reader side)

Completed a feature the codebase had already half-built: a literal
`/* TODO: Implement line capture for raw tags */` sat in
`_tok_legere_tag`, the tokenizer already emitted CRUDUS tokens with
`captio_numerus > 0`, the raw-mode guard already excluded them from
scan-until-closing-tag, the parser's crudus branch already set both
flags and the CAPTIO_ANTE writer already emitted `<tag! (>` with
unescaped raw children. Even the token fields for the design
(`habet_captus`, `captus_contentus`) existed, initialized everywhere
and read nowhere. Surfaced by the aedilis Phase-0 stml specimens.

**Semantics (Fran, 2026-07-20)**: captures the rest of the line RAW
(no tags, no entities) up to but not including `\n`; trailing `\r`
trimmed; leading/trailing spaces of the content kept (raw is raw);
`((>` records captio_numerus=2 but still captures ONE line — the
multi-line + dedent question is deliberately deferred.

**The four changes**:
1. Tokenizer (`_tok_legere_tag`, the TODO branch): reads the
   post-`>` remainder of the line into `token.captus_contentus`
   (the pre-sketched fields), leaving `\n` to the normal stream.
2. Parser (`_parser_legere_elementum_crudus`): captured form
   attaches the text child from `captus_contentus` directly
   (interned raw — no unescape, no normalization) and returns.
   THE ACTUAL BUG lived here: the branch unconditionally consumed
   the next CLAUDERE token (correct for `<tag!>...</tag>`, wrong
   for captures which own no closing tag) — it devoured the
   PARENT's closing tag, and the parent then hit EOF →
   TAG_NON_CLAUSUM. That single consumption was why the whole
   feature appeared unimplemented.
3. Capture pass (`_processare_captiones`): ANTE branch now skips
   `crudus` nodes — their capture was satisfied at parse time;
   grabbing following siblings would double-capture.
4. Writer (CAPTIO_ANTE branch): in NON-pretty mode appends `\n`
   after the captured content — without it the following sibling
   is devoured on re-read. In pretty mode the parent's separator
   newline already serves (probatio pins both postures
   byte-stable).

Also corrected the stale stml.h header claim "No entity
references": text is escape/decode SYMMETRIC; attribute values are
raw both ways (`"` unrepresentable there); raw tags verbatim.

Tests: probatio_stml gained 7 blocks (asperities verbatim, the
devour regression pair, entities-intact, empty capture, `((>`
notation, dual-posture round-trip, constructor path — the aedilis
manifest lane). All green first run.

## 2026-07-30 — anonymous capture measured; fragment-capture design landed

Exploration session with Fran (STML vs XML). Measured: `<(>` parses
TODAY by fallthrough — empty tag name read without complaint, capture
parens counted, capture WORKS — but the element is nameless
(unaddressable by any selector) and the writer re-emits `< (>`
(byte round-trip broken). Nothing pinned. Board: 01KYRFMW58.
Same silent-degenerate family as the tolera angle-char fix.

Direction DECIDED same day (01KYRGEGV4, articuli capture blocks →
board res): the real feature is FRAGMENT capture — `<# (>` authored,
`<#01KYRF (>` after the daemon stamps the minted res id back into
the text. Fragment tokenizer currently reads id then expects `>` or
`/>` — the parens work lands in _tok_legere_fragmentum + the
fragment parser branch + writer + round-trip pins. Bare `<(>`
becomes loud refusal or normalization sugar when that ships.

Transclusion status also measured: `<<selector>>` tokenizes, parses,
round-trips, is tested — but NO resolution exists anywhere (syntax
without semantics). Fragment-id stamping gives it its natural first
scope when pulled: resolve `<<#id>>` against the forum corpus.

## 2026-07-30 (later) — fragment capture SHIPPED (arc ① of the articuli feature)

Fragments now take capture parens: `<#id (>` / `<# (>` / `<#a ((>`,
plus bare `<(>` as authoring sugar for an anonymous capturing
fragment. Four edits, all mirroring existing precedent:

- **Tokenizer** (_tok_legere_fragmentum): paren-count loop inserted
  after attribute parsing, exactly where _tok_legere_tag does it;
  self-closing `/>` check gated on captio_numerus == 0 (same as the
  element path). Token genus stays FRAGMENTUM_AUTO with
  captio_numerus discriminating — the CRUDUS precedent (raw-line
  capture already discriminates by field, not genus). No new enum
  member needed.
- **Sugar** (_tok_legere_captio_nuda, new): dispatch routes `<(` to
  it before the regular-tag path. Emits anonymous FRAGMENTUM_AUTO
  with the paren count. This RETIRES the 01KYRFMW58 accident — the
  old fallthrough (nameless element, `< (>` re-emission) is
  unreachable now.
- **Parser** (_parser_legere_fragmentum_auto): copies captio fields
  and — load-bearing — creates the `liberi` xar when capturing.
  Auto-fragments normally leave liberi NIHIL; _processare_captiones
  appends captives via xar_addere, which needs the receptacle.
  The capture processor itself needed ZERO changes: fragments are
  STML_NODUS_ELEMENTUM, so the existing ANTE branch just works.
- **Writer**: fragment branch gains an ANTE case before the
  habet_liberos split — ` ` + parens + `>` + children inline, no
  closing tag (same shape as element ANTE). Sugar normalizes on
  write: `<(>` → `<# (>` — deliberate; the authored form is
  ephemeral (the gesta daemon rewrites the line when stamping).

Round-trip law clarified while pinning: for capture forms the house
law is a FIXED POINT (write → re-read → re-write stable), not
authored-bytes — capture restructuring makes authored-bytes
impossible in general. But the canonical inline forms ARE
byte-exact: `<doc><#01KYRF (>eat a cake</doc>` round-trips
identically (pinned), which is the form the daemon stamps.

Pins: 4 new blocks in probatio_stml (named capture + byte round
trip; double parens capture 2; sugar normalization + fixed point;
the multi-line ideas block — the actual articuli target shape —
2 fragments, anonymous + stamped, circuit stable). All green FIRST
run; root suite 108/108 PLENUS.

Note: examen shows a pre-existing domesticum in stml.h (`/* <tag */`
comment anchors an unparsable annotation) — present in the committed
version too, only the line number moved. Not introduced here.

Next: arc ② — the gesta daemon-side capture pass (mint ULID prefix,
create genus/res, rewrite corpus stamping the id). Design on board
01KYRGEGV4.

## 2026-07-30 (later still) — anonymous closing tag </> shipped

Fran wrote `</>` in his first live capture articulus expecting it
to close the nearest tag, and it LOOKED like it worked (the daemon
line-scanner ignored it and the corpus happened to end there). Now
it actually works, both layers:

- Parser: a CLAUDERE token with an EMPTY name (which `</>` already
  tokenized as — the close branch read an empty name without
  complaint) is accepted by the element currently parsing. The
  recursive descent gives close-nearest for free: the innermost
  open element sees the token first. Named closes keep strict
  matching; a stray `</>` with nothing open stays TAG_IMPROPRIE.
- Round-trip: `clausura_anonyma` flag on StmlNodus; the writer
  emits `</>` when set — byte-identical, UNLIKE the `<(>` sugar
  (which normalizes because the daemon rewrites that line anyway;
  a closing line is never rewritten, so the authored form must
  survive).
- Scope: elements only. Raw tags end raw mode via their named
  close (`</>` inside them is literal text); fragments keep
  `</#>`.
- Daemon scanner: `_cap_blocum_claudit` accepts `</>` — closing
  the leak where capture lines after an ignored `</>` would have
  been silently swallowed into the block (pinned: id 813 in
  probatio_tabularium).

Pins: nearest + nested-double byte round-trip, orphan errors loud.
All first run; root PLENUS, gesta 4/4. Board 01KYSPRF9R.

## 2026-08-06 — stml_strictum: quattuor probationes, et duae quae NON factae sunt

Contractu naturae ad STML BENE FORMATUM muto (ab XML), quaesitum
est quid stml praeter parsationem probare deberet. Responsum
MENSURATUM est, non aestimatum, et mensura consilium mutavit.

METHODUS: pro quoque casu quem stml ACCIPIT ubi xmllint reicit,
arborem inspicere ET CIRCUITUM (parsare -> scribere -> conferre).
Circuitus oraculum est: si non redit octetim, parser aliud
intellexit quam scriptum est.

QUOD MENSURA OSTENDIT - duo ex sex 'defectibus' NON defectus sunt:

  attributum sine quotis   <a nomen=valor>
    -> circuitus '<a nomen="valor">'. RECTE intellectum, ad formam
       canonicam normalizatum. Lenitas consulta, non vitium.
  ens ignotum              <a>&ignotum;</a>
    -> circuitus '<a>&amp;ignotum;</a>'. Ut textus litteralis
       tractatum, quod praedicibile est et documentum non corrumpit.

Ergo gradus I MINOR factus est quam aestimatus. Non omne discrimen
ab XML defectus est, et sine mensura utrumque frustra 'emendassem'.

QUATTUOR QUAE VERE NOCENT (omnia post-parsationem, arbore iam
exstante - nulla mutatio parseris):

  1. ATTRIBUTUM DUPLICATUM - periculosissimum, et non obvium.
     <a n="1" n="2"/> circuitum IDEM reddit et AMBO servat
     (xar_numerus == II), sed stml_attributum_capere PRIMUM
     reddit. Plagula igitur 'n=2' dicit dum omnis lector 'n=1'
     adhibet. Non error clamans sed DISCRIMEN TACITUM inter id
     quod scriptum est et id quod valet - genus pessimum, quia
     inspectio oculis plagulam rectam ostendit.
  2. RADICES PLURES - quicquid post primum elementum sequitur ab
     omni vocante qui elementum_radix legit TACITE ignoratur.
     In .genera hoc genus integrum evanescere sineret.
  3. TEXTUS EXTRA RADICEM - idem, minore pretio.
  4. TITULUS VACUUS - '<>' elementum sine nomine parit (mensuratum:
     titulus->mensura == 0, fragmentum == 0). Fragmenta excipiuntur,
     quibus titulum vacuum habere LICET.

FORMA: stml_strictum(radix, piscina) -> Xar de StmlStrictumVitium.
Parser eas NON cogit, quia STML fragmenta et usus alios legitime
fert; politia vocantis est. natura eas ad regulam I mappat.

Duplicata quadratice quaeruntur consulto: elementa attributa pauca
ferunt (in corpore naturae maximum VII), et tabula dispersa hic
plus constaret quam solveret.

PROBATIO: vitium quodque PLANTATUM est - porta silens et porta
mortua idem spectantur (lectio hodierna ab uncis mortuis empta).
Lenitas quoque probata est ne postea 'emendetur': attributum sine
quotis, ens ignotum, attributum booleanum, fragmentum anonymum -
omnia ZEPHYRUM vitia reddere DEBENT. XXIX assertiones novae.

## 2026-08-06 — normalizatio a PARSATIONE ad LECTIONEM migrata

Fran rogavit an vitium optionum in bibliotheca emendatum esset an
solum circumventum. **Circumventum erat** - natura_examen optiones
manu iungebat, stml intactum manebat. Quaestio recta fuit, et
inquisitio vitium multo maius aperuit.

VITIUM VERUM, mensuratum non coniectatum:

    '<p>salve <b>munde</b> iterum</p>'
      -> textus internus 'salvemundeiterum'
      -> circuitus '<p>salve<b>munde</b>iterum</p>'   FRACTUS

Circuitus contentu mixto quolibet frangebatur. Optiones
conglutinatae symptoma mitissimum erant.

CAUSA NON ERAT stml_textus_internus (quod fideliter concatenat)
sed PARSATIO: `_parser_legere_textus` spatium album normalizabat
et nodos spatii albi SOLIUS omnino abiciebat. Et id CONSULTO
factum erat - quattuor assertiones id ut morem intentum tenebant
(praecisio simplex, normalizatio multilinea, indentatio relativa,
spatium-solum-omissum). Normalizatio bona est: STML manu scriptum
indentatum legibile facit.

ERGO NON VITIUM FUNCTIONIS SED **RES BONA GRADU FALSO POSITA**.

RADIUS DISPLOSIONIS MENSURATUS ANTE CONSILIUM: normalizatione
prorsus remota, ex CXVIII probationum gregibus DUO soli fracti
sunt (stml VI assertiones, aedilis). CXVI intacti. Timor 'omnes
ab praecisione pendent' FALSUS erat - quod solum mensura ostendit.

FACTUM:
  - parser textum VERBATIM servat (nodi spatii albi quoque)
  - stml_textus_internus verbatim concatenat = textContent (DOM)
    et string() (XPath)
  - stml_textus_normalizatus() NOVA: idem, deinde normalizatio
    (indentatio communis remota, lineae vacuae abscisae)
  - quattuor assertiones ad functionem novam migratae; arborem
    fontem servare quoque nunc asserunt

DUO VITIA CONSEQUENTIA, utrumque a probationibus captum:

1. LINEA TERMINALIS DUPLICATA. Scriptor post tag crudum
   line-captantem '\n' caece addebat (recte, cum parser eam
   abiceret). Textu servato, frater textus eam IAM ferebat: unde
   circuitus scribere->legere->rescribere lineam vacuam quoque
   cursu CRESCENTEM pariebat. Emendatum: additur nisi frater
   sequens iam '\n' incipiat.

2. PULCHRUM ET FIDES COMPUGNANT. Modus pulcher dispositionem
   SUAM generat; spatio fontis quoque servato, duae indentationes
   se cumulabant et circuitus rursus crescebat.
   SOLUTIO DOCTRINALIS, non technica: **duo modi, duae
   promissiones.** Non-pulcher FIDEM praestat (circuitus octetim)
   ergo verbatim scribit; pulcher LEGIBILITATEM praestat ergo
   nodos spatii albi solius OMITTIT. Pulcher fidem numquam
   promisit - reformator est, non exscriptor. Quod prius latebat
   quia parser utrumque simul praestare fingebat abiciendo.

LEX GENERALIS: eadem proprietas parseris in uno campo strepitum
tollit et in alio contentum corrumpit. 'Melius' non est proprietas
parseris sed USUS. Quapropter normalizatio ELIGI debet, non
IMPONI - et id est tota migratio.

## 2026-08-10 — dotted tag names + the empty-name crash family

Two changes shipped together (arbor porphyriana tasks I-II, e825ce3 +
6a9ad05):

**The crash that was already there.** Any tag whose name starts outside
`[a-zA-Z_:]` (`<9bad/>`, `<.x>`) made `_tok_legere_nomen` return an
empty chorda with the position UNMOVED. Downstream, two different
failures: with attributes present, consumers dereferencing the empty
interned title segfaulted (canon_examen exit 139); without attributes,
the anonymous-close machinery `</>` swallowed the malformed close and
the parse SUCCEEDED with a corrupted tree (`<.species>x</.species>`
emitted as `<>.species&gt;x</>.species&gt;`, successus=1). The silent
form is the nastier one — no test could assert on an error that never
fired. Fix: `_titulus_male_incipit` guard at all four tag-name read
sites (open/close/backward/sandwich) → STML_TOKEN_ERRATUM →
STML_ERROR_SYNTAXIS. `<>` stays lenient (pinned: strictum judges
TITULUS_VACUUS), `</>` stays legal.

**Leading dot.** `_tok_legere_nomen` accepts ONE leading `.` iff
followed by a normal name-start char. The dot is part of the NAME
(byte-exact round-trip free; close tags match by ordinary equality).
Attributes can never get it: the attribute path gates on
`_est_nomen_initium` BEFORE calling the reader — do not "fix" that
gate. Writer, raw-close matchers, and tag dispatch were measured
dot-transparent before the change. coloratio.c (private lexer copy)
taught the same rule at its tag-position sites.

Trap for the future: `_est_nomen_initium` IS the attribute gate.
Adding `.` to it silently legalizes `<a .attr="x">`.

## 2026-08-10 — augmentation `<% &clavis;> … </%>` (librarium W3)

New construct: an element-typed node (titulus "%", genus stays
ELEMENTUM so every genus-switch consumer is untouched) carrying a
mandatory sigiled target key. The key is stored VERBATIM including
`&`/`;` in `augmentum_clavis` — the same bytes the canon citation
lookup compares against claves-externae, so the canon layer needs
zero re-sigiling. One lawful verb (append) lives in the sigil; no
attributes on `<%` itself.

Shape follows the fragment (`<#>`) precedent exactly: paired
open/close tokens (PERCENTUM_APERIRE XVIII / PERCENTUM_CLAUDERE
XIX), dedicated lexer functions dispatched in `_tok_proximus`
(`</%` check BEFORE the fall-through to `_tok_legere_tag`), a
`_parser_legere_percentum` that loops children until the close
token, and a writer arm ahead of the fragment arm emitting the
canonical `<% ` + key + `>` … `</%>` (byte-exact over canonical
form; children serialized inline non-pretty, like fragments).

Deliberate STRICTNESS at birth (unlike the lenient legacy paths):
missing/bare/kind/empty key (`<%>`, `<% laika>`, `<% .canis>`,
`<% &;>`) = ERRATUM → SYNTAXIS; `</%x>` = ERRATUM; unclosed at EOF
= TAG_NON_CLAUSUM (an augmentation must never silently swallow the
rest of the document).

Two lessons:
1. **Status is last-error-wins.** `<%></%>` carries TWO errors (bad
   open → SYNTAXIS, then orphan close → TAG_IMPROPRIE) and the
   parser overwrites status, so the second wins. Negative pins that
   assert an EXACT status must use inputs with exactly one vitium.
2. **The uninitialized-field family was real and got fixed.**
   `_parser_creare_nodus`, all four public creators, and
   `_duplicare_recursivum` never touched fragmentum/fragmentum_id —
   fresh-piscina zeroing hid it (a DUPLICATED fragment silently
   became an ordinary element named "#", which only round-tripped
   by coincidence of the `<#` spelling). All three fields
   (fragmentum, fragmentum_id, augmentum_clavis) now explicit at
   every allocation site, and duplicare copies them.

Amalgam note: the amalgamator drops the StmlNodus DEFINITION from
the body because hand-written silva/amalgama/silva.h owns it
(CADENDA_DEFINITIO) — a new struct field must be added there BY
HAND (vanilla C89 idiom) or the standalone verify fails with
"no member named …". Done; hospes 37/37, nm-intersectio 0.

Cosmetic follow-up not taken: coloratio has no `<%`-aware arm (its
`#` handling is hashtag styling, not STML tag structure) — `%`
renders as operator color, acceptable.

## 2026-08-19 — scriptio pulchra: contentum mixtum corrumpebat (et cumulabat)

Found while grounding the arbor⇄STML spec (silva parse trees →
canonical STML), which wants tokens carrying their spelling as text
content alongside `<ante>`/`<post>` trivia element children — i.e.
mixed content, in every single token, in a document meant to be
pretty-printed and hand-authored. A live probe compiled against the
real libs said no:

    <t><ante>x</ante>n</t>  --pretty-->  <t>\n  <ante>x</ante>\nn\n</t>

Re-read, the token's spelling is no longer `n` but `\nn\n`. It does
not stabilise — a second cycle gives `\n\nn\n\n`. Each pass adds a
newline per side. "Ignore whitespace-only text on load" does NOT
rescue this: the corrupted node is not whitespace-only, it contains
the real text.

**Root cause.** The mixed-children branch emitted `\n` before the
first child and after every child unconditionally, blind to the
child's genus, while indentation *spaces* come from each child's own
case (ELEMENTUM/COMMENTUM/PROCESSIO/DOCTYPE self-indent; TEXTUS and
TRANSCLUSIO never did). So real text got bracketed by bare newlines
that became part of its value.

**Second defect, same cause, found by the tests not the analysis.**
A whitespace-only text child already had its *content* suppressed,
but the loop still appended its trailing newline — a stray blank
line. Consequence nobody had noticed: **pretty output was never a
fixed point for ANY element with 2+ children.** The all-elements
regression guard `<t><a/><b/></t>` writes correctly the first time
and then grows a blank line before every child on each subsequent
read-write cycle. The scoping pass had verified the first write and
stopped there; only the two-cycle assertion caught it.

**Fix.** The loop now decides per BOUNDARY, not per child: a boundary
carries whitespace unless either side is significant text.
Whitespace-only text children are fully transparent — skipped, and
boundaries computed against the real neighbours. When a boundary
collapses, the element neighbour must also not self-indent, so it is
invoked with `pulchrum=FALSUM` (the crudus/single-text branch above
already used exactly this trick). Named price: that child's own
interior formatting flattens too. Acceptable here; the cleaner
alternative — move indentation from the four child cases up into the
loop so the boundary decision is atomic — is a bigger refactor, left
as a door.

Also folded in: TRANSCLUSIO never called `_scribere_indentatio` at
all, so `<<#nav>>` stood unindented among indented siblings. One line.

**Lesson worth keeping: one cycle is not evidence.** Both defects
compound, and both would read as "fine" under a single
write→read→compare. Every pretty-mode test now asserts a SECOND
cycle. The pre-existing pretty test was the warning sign in
hindsight — it built an all-structural tree and asserted only that
the output was non-empty and contained a newline, which is a test
that cannot fail.

Blast radius measured before touching anything: five `pulchrum=VERUM`
call sites (gutenberg_index, librarium_merge, aedilis, one test), all
building text nodes as the sole child of their element, so all on the
pre-existing safe path; the three committed `.stml` artifacts do not
move. `librarium_collector` writes STML by hand-concatenating strings
and never calls the writer at all — separately filed, since that also
means it never escapes entities (01M0EEK3YYEYVBWV9BFWD028D5).

## 2026-08-19 — clausura tacita in modo pulchro (strepitum minuere)

Fran's request, second half of the same sitting. In pretty mode, emit
`</>` instead of `</name>` while the content is short; keep the name
past ~30 lines, on the reasoning that once the open tag has scrolled
off screen the name stops being noise and starts being help.
Threshold is `STML_CLAUSURA_TACITA_LINEAE` (XXX).

Decided from the bytes already emitted, not from the tree: record the
builder length after `>`, count newlines from there at closing time.
That measures exactly what the reader will see, and it is what the
writer itself just laid out.

**Fixed point holds, and here is why it is not luck.** `</>` and
`</name>` occupy the same number of LINES, so the choice cannot feed
back into the measurement that produced it — no oscillation is
possible. All thirteen pretty tests assert two cycles; all stable.

**Invariant worth knowing** (pinned by test XIII): a parent's content
strictly contains its children's, so a parent is never shorter than a
child. Therefore a tacit closer on a parent IMPLIES tacit closers on
every descendant — `</>` runs are always contiguous at the end of a
block, never interleaved with named ones on the way down.

**Exclusions.** Raw (`crudus`) elements keep the explicit name because
the grammar does not let `</>` close them. Non-pretty mode is
untouched and still preserves the author's own form byte-for-byte —
fidelity mode does not get opinions about layout.

**Named unevenness, not incidental.** An element sitting on a
COLLAPSED boundary is rendered with `pulchrum=FALSUM` (so it will not
self-indent), and that same flag also means "preserve the author's
closing form" — so it keeps `</post>` while its siblings get `</>`.
One flag is now carrying two meanings. Tests II and X pin the current
behaviour so it cannot drift silently. The clean separation is the
door already named in the previous entry: hoist indentation out of the
four child cases into the loop, at which point "do not indent me" and
"fidelity mode" stop sharing a channel.

**Rejected on measurement: the forward-capture form.** Fran also
proposed `<foo (>abc` on its own line instead of `<foo>abc</foo>`.
Probed against the real parser: text nodes run to the next `<`, so the
terminating newline becomes PART of the captured value — `abc` reads
back as `abc\n`, and re-writing compounds it. The raw-line variant
`<foo! (>` does not parse in that position at all, and an element with
empty text emits `<foo (>` which then captures the FOLLOWING SIBLING
as its child. Making it safe would require the parser to trim, which
needs a form-flag to keep the round-trip exact — and cuts against the
2026-08-06 ruling that moved normalization OUT of parsing into the
accessors. `</>` gets `<lex-identificator>n</>` at 23 chars against
capture's 22, so the whole benefit survives at none of the cost.
Capture stays what it already is: an authoring affordance, used by the
47 hand-written `<via (>path` lines in aedilis.stml, whose only reader
already normalizes them.

## 2026-08-24 — trivia-model design session: where the bodies are

Full design: project-specs/stml-trivia-spec.md (parcum 01M0T5XYC3).
Code-level finds an implementer should NOT re-derive:

- **The capture/whitespace bug lives at `_processare_captiones`**
  (~lib/stml.c:2545): the capture loop skips comments
  (`!_est_commentum`) but COUNTS text nodes — `<a (>` + newline
  captures the whitespace text node, not the element. This is the
  precise reason the earlier capture-collapse run deferred.
- **`stml_textus_normalizatus` IS the dedent** — the accessor's
  body `_normalizare_spatium_album` (~:1984) already does
  trim-empty-edge-lines + strip-common-indent + preserve-relative.
  The spec's "pre reading" exists; only `fluxus` (prose) is new.
- **Raw multi-capture is half-implemented**: lexer note at :737 —
  `captio_numerus > 1 notatur sed adhuc UNAM lineam capit`. The
  spec supersedes it with `<tag!\>` and turns multi-paren raw into
  a loud refusal.
- **Tag-interior whitespace is a standing fidelity hole**: the
  lexer accepts newlines between attributes, the writer normalizes
  to single spaces — non-pretty round-trip is NOT byte-exact for
  multi-line tags today. Spec §1.6 models it (per-attribute
  spatia_ante + pre-`>` chorda); whitespace around `=` stays a
  named normalized exception.
- The four-member bug family that motivated the model: raw-capture
  accumulating newline (fixed 2026-08-06, "SED SEMEL TANTUM"),
  TERMINI (2026-08-19), capture counting (above), clausura tacita
  line-counting. All one root: whitespace lives inside text nodes.
- **M1 order matters**: capture `stml_textus_internus` GOLDENS over
  the fixture corpus BEFORE any surgery — they are the old
  behavior's testimony, unobtainable afterwards.
