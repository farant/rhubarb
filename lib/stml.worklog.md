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

## 2026-08-24 — trivia M1 T2: the heart landed, and what it taught

Implementation session, tranches: T1 CRLF (2fd925bb), T2 model +
parser + non-pretty reassembly (this entry). Finds an implementer
of M2+ should not re-derive:

- **The §2 clausurae exclusion was WRONG and is rescinded.** The
  spec excluded `spatia_clausurae` from internus so `<a>\n</a>`
  reads "" — "the ONE bridge change." The goldens measured ~6,000
  divergent element records: the close-tag indentation of every
  nested element lives in clausurae. internus now reassembles
  clausurae too (recursively) and the bridge is EXACT. The
  empty-reads-empty semantics belongs to `stml_textus_valor` (M3),
  whose trim yields "" anyway. Spec §1.3/§2/§8 amended in place.
- **`pulchrum` was carrying a third meaning and it broke.** The
  TERMINI pretty path renders inline-mixed children with
  pulchrum=FALSUM ("don't self-indent"); reassembly read that as
  "fidelity" and emitted their trivia → doubled newlines on the
  pretty fixed point. `_scribere_nucleus` now threads a separate
  `fidelitas` flag (immutable down the recursion) — the hoist the
  2026-08-19 worklog entry predicted. Trivia emission and
  stream-order decisions key on fidelitas; layout keys on pulchrum.
- **Capture emission is stream-order now (spec §6, pulled into M1
  by fidelity):** non-crudus ANTE captor emits [tag][post][capti];
  CRUDUS captor emits [tag][linea capta][post] — the captured line
  sits BEFORE its terminating newline, the reverse of the normal
  case (first implementation got this wrong; probatio_stml caught
  it). RETRO emits children BEFORE the tag; FARCIMEN emits
  child[0], tag, rest. Pretty keeps captor-first (reformatter).
  The SED-SEMEL-TANTUM hack is deleted; a grammar guard remains
  for hand-built raw captors with no spatia_post (emit '\n' or the
  next sibling is devoured on re-read).
- **Old retro/farcimen "roundtrips" were reorderings.** The old
  writer emitted captor-first, so `<b/>\n<) a>` came back as
  `<) a>\n<b/>` — probatio_stml PINNED that as expected. Now
  byte-exact; expectations updated.
- **Childless element with clausurae must NOT self-close** —
  `<root>   \n   </root>` has zero children after elision; `/>`
  would drop the interior bytes. habet_liberos treats non-NIHIL
  clausurae as content in fidelity mode.
- **Text node extents were degenerate** ([finis,finis) — node
  created AFTER progredi read the NEXT token's start). Fixed:
  created before progredi, extent covers the full run. The spec's
  "extents continue to cover the FULL run" was aspirational.
- **stml_legere built the document node from uninitialized
  ctx.current** (stack garbage in linea/positus) — found because
  the goldens wobbled between builds (c7476a72).
- **Distribution mechanism**: one child-loop `_liberos_legere`
  (elementum/fragmentum/percentum/documentum — the quadruplicated
  loops collapsed); `_textum_tractare` classifies runs (§1.3) and
  distributes via a loop-local `pendens` chorda — no parser-global
  state, so nesting can't mis-own across levels. prior==NIHIL
  (after open tag) gives the whole run to the next node's ante,
  per §1.2.
- **Corpus audit result**: 61 files elision-count-only; 7 files
  with internus deltas, all capture-bearing (aedilis ×2, WORKSHOP
  ×5) — trailing indentation moved from captured text values to
  the next sibling's ante (the §0.1.3 fix), plus retro captures no
  longer relocating whitespace. Root-level same-length hash
  changes = retro-captured whitespace no longer moves position.

## 2026-08-24 — trivia M1 T3: the kind ladder, and two traps

`\` lexing + dedent-at-parse + indentatio + refusals landed. Finds:

- **The transform rides ON the trivia machinery, not beside it.**
  `<tag\>` content is still one text child; the general §1.3 edge
  clip runs first, then `_multilineam_transformare` reconstructs
  interior = ante+valor, re-clips the LEADING edge to the first
  newline ALONE (the general maximal clip would eat line 1's
  indentation and destroy relative structure), and re-applies the
  general TRAILING clip (needed for `!\`, whose raw interior never
  went through classification; idempotent for the non-raw path).
  Trailing remainder lands in element clausurae per §1.2 — so the
  trailing-equivalence decree (`123</>` ≡ `123\n</>` ≡
  `123\n  </>`) holds mechanically, no special case.
- **Prefix reinsertion must skip line 0 for tag-line content.**
  `<m\>abc\n  def` — line 0 never carried the prefix (excluded
  from dedent, PEP-257), so `_valorem_praefixo_scribere` takes
  `prima_quoque` = (spatia_ante != NIHIL). First emission got this
  wrong: `<m\>  abc`.
- **First error wins, now enforced** (`_errorem_ponere`): a
  refusal that denies a node leaves its close tag orphaned, and
  the orphan's TAG_IMPROPRIE was OVERWRITING the named MULTILINEA
  status (same for `\!` → SYNTAXIS). All parser status setters
  route through the guard: later errors are usually cascata of the
  first.
- Multilinea interior layout is DECLARED content: text-child
  ante/post + prefix emit in BOTH writer modes (`_intra_
  multilineam`), unlike ordinary trivia (fidelitas-gated). M2's
  re-indent liberty will regenerate the prefix, never the
  interior.
- Refusals live in the parser, not the lexer, so they can NAME
  themselves (status MULTILINEA + causa in result.error). Only
  `\!` is lexer-level ERRATUM (it would change lexing itself).

## 2026-08-24 — trivia M1 T4+T5: tag-interior trivia, planted faults

T4 (§1.6): lexer records inter-attribute whitespace
(StmlAttributum.spatia_ante, single-space-canonical = NIHIL) and
the pre-'>' span (token → nodus->spatia_intra_tagum; capture forms
excluded — their layout stays canonical). The multi-line tag
fidelity hole is CLOSED: `<a\n  b="1"\n  c="2">x</a>` round-trips
byte-exact. `=`-whitespace normalizes both ways (named exception).
Finds:

- **The five duplicated attribute-emission loops are now ONE
  surface** (`_attributa_scribere`) — they had already drifted
  (one had a comment the others lacked); §1.6 would have needed
  the same edit five times.
- **The token-struct uninit family bit AGAIN**: StmlToken grew
  twice this session (multilinea, spatia_prae_finem) and only the
  two tag constructors initialized the new fields — `<(>`'s
  bare-capture token carried stack garbage into chorda_internare →
  friatio crash. Same disease as the uninit documentum (c7476a72).
  Fixed by initializing in ALL constructors at the
  habet_captus anchor. If StmlToken grows again: grep
  `token.habet_captus` and extend EVERY site, or introduce a
  single token-zeroing constructor.
- The pre-attribute `_tok_praeterire_spatium` calls in tag and
  fragment readers were DELETED — the attribute loop's own span
  capture must see the first attribute's whitespace.

T5 (§7.6) — planted faults, each must redden a gate before the
gates are trusted:
1. mis-owned newline (partiri exclusive instead of inclusive) →
   probatio_stml (tree assertions) + aurea (per-node records) RED
2. dropped trailing edge → probatio_stml + aurea RED
3. doubled raw-capture delimiter (the SED SEMEL disease
   re-planted) → probatio_stml (circuitus crudae) RED; aurea
   correctly blind (writer output is not internus's domain)
All restored; all green after restoration.

## 2026-08-24 (M2 T1+T2) — TERMINI deleted; pretty = one layout decision per element

T1 built the gates BEFORE surgery (probatio_stml_pulchrum: fixed
point §5 + tree-equivalence-modulo-trivia §7.7 over the aurea
corpus + 18 litterae). Born red on 3 real bug classes — the honest
work map:

1. **Drop-class**: TERMINI's pretty skipped same-line
   whitespace-only text nodes. Post-M1 those are DELIBERATE CONTENT
   (`<sep>   </sep>` decree) — the skip silently deleted values.
   Corpus hit: rhubarb.census `</glossa>  </facultas>`.
2. **Retro form-dishonesty**: pretty emitted `<) a>` captor-first
   with the captured child AFTER — reparse reversed the capture
   direction (captor grabbed nothing, child became a sibling).
   The old "roundtrip" never was one.
3. **Farcimen**: same disease — child[0] must precede the tag.

Fixes (T2): the TERMINI state machine deleted wholesale; replaced
by ONE decision per element — any text child → INLINE (children on
the tag line, pulchrum=FALSUM, text KEPT), else BLOCK (one child
per line). Retro/farcimen now emit stream order in BOTH modes.
Document level got the same inline-if-text rule (a '\n' separator
before a same-line text child would convert it to trivia on
reparse). Pretty multilinea (`\`) now REGENERATES the indent prefix
to nesting depth (_valorem_pulchre_indentare) instead of emitting
the stored one — moved nodes reformat correctly; fidelity still
uses the stored prefix byte-exact.

Finds for the next reader:

- The flag-extinguish trick (inline children get pulchrum=FALSUM)
  SURVIVES the deletion and is now harmless: since M1 split
  fidelitas from pulchrum, "don't self-indent" no longer drags
  "fidelity" along. The 2026-08-19 "pretium nominatum" debt is
  paid, not just documented.
- `_spatium_album_solum` deleted (both callers were the disease).
  If pretty ever wants to special-case whitespace again, that is
  the smell of a new TERMINI.
- WORKSHOP.taxonomy.stml does not parse (status 4, predecessor-
  generation reference file). The aurea golden RECORDS the refusal
  (`arbor successus:0`) so refusal-change is guarded there; the
  pulchrum gate skips it LOUDLY (recusatio_licet only for corpus
  files — litterae must parse).
- Expectation churn: 8 of 13 TERMINI-battery cases in
  probatio_stml rewrote to inline expectations; case VI's doctrine
  INVERTED (whitespace "transparens" → "servatus").
- Planted fault (stored ante emitted in pretty block) reddened the
  fixed point across litterae AND corpus; reverted.

Gates after: stml family 4/4, full suite 140/140, amalgam
re-verified (hospes 39/39), silva suite green.

## 2026-08-24 (M2 T3) — the collapse: glued parens, rule A, chained captures, spine emission

The feature Fran asked for (`<t1(> <t2(> <t3(> foo`) landed in three
tranches. Finds for the next reader:

- **`<tag(>` always lexed** — `(` is a name terminator (line ~185).
  M1 explicitly DISCARDED the pre-paren span for capture forms and
  the writer hardcoded `" ("`, so glued input silently reformatted.
  Now stored (spatia_intra_tagum), NIHIL = glued canonical; pretty
  emits glued-sans-attrs / spaced-after-attrs (§0.2). The `<(>`
  sugar's canonical form changed `<# (>` → `<#(>`.
- **`_parser_legere_captio_ante` was the one node path missing the
  spatia_prae_finem attach** — elementum/crudus/fragment paths all
  copied it. Symptom: authored `<wrapper (>` re-emitted glued.
- **Chained forward captures NEVER worked.** The flat left-to-right
  pass had `<a(> <b(> x` resolve as a→[b-bare], x orphaned. No
  corpus document uses spines (the formatter is what will produce
  them), so nothing ever tested it. `_captorem_ante_satiare` is the
  recursive fix: a captee that is itself an unsatisfied ANTE captor
  satiates from the remaining stream first.
- **Comments between captor and captee VANISHED** — the old loop
  skipped them in counting but never preserved them anywhere. §6's
  "skipped-but-preserved" was half-implemented. They now enter the
  captor's children in stream order. Aurea recorded the buggy
  4-node tree; judged + regenerated. Farcimen/retro comment
  interleaving still has the disease — M4 (counting policy).
- **A captor's parse extent covers ONLY ITS TAG** (positus_finis
  set at token consumption, before reparenting). The collapse
  writer's sedes entries must match — spine link finis is recorded
  after `(>`, BEFORE the separator. The sedes parity gate caught
  the full-spine-extent version immediately.
- **`_valor_capturabilis`**: text terminals must be non-empty,
  single-line, first byte non-whitespace — otherwise rule A
  re-attributes the leading bytes on re-read (the `<sep>   </sep>`
  fixed-point break the gates caught on first run).
- **The canonical separator after `(>` must ALSO come from the
  plain ANTE branch in pretty** (captio_numerus == 1 only) — else
  pass 2 emits `<x(>foo` where pass 1 wrote `<x(> foo` and the
  fixed point dies. Multi-captee captors stay glued: a space
  between captees would parse to a text node and corrupt counting
  (M4 corner).
- **chorda_aedificator_truncare** added (offset clamp) — enables
  optimistic-render + rollback (aedificator + xar_truncare on the
  sedes table) for the all-or-nothing width check.
- Silva arbor probationes string-match the STML text — collapsed
  forms churned ~11 assertions (`<lex-int(>` etc.). The 154-file
  memcmp gate and both canon freshness gates never flinched: tree
  meaning is what they eat, and Gate B guards exactly that.

## 2026-08-24 (M2b) — vertical collapse: capture form universal, width picks layout

The second collapse decree landed as a writer-only change (parser
untouched — stacked captees were already legal input via the
ownership law). Finds:

- **The all-or-nothing core is GONE.** `_spinam_collabere_conari`
  (optimistic render + full rollback) was replaced by
  `_spinam_pulchre_scribere`: capture form is universal for
  eligible spines; the 72-column budget only picks between packing
  captor links on the current line and breaking to a new line ONE
  step deeper (per LINE, not per link). Rollback machinery
  (builder + sedes truncate) survives, but per-LINK now, for the
  greedy wrap decision.
- **The ANTE unification was structurally free.** The collapse
  hook sits at the top of casus ELEMENTUM, BEFORE the
  augmentum/fragmentum/captio dispatch. Extending
  `_spinae_liberum_unicum` to accept single-paren non-crudus ANTE
  captors meant authored captors route through the same
  width-aware routine with no changes to the flat ANTE branch —
  it survives only as the fallback for multi-paren, crudus,
  comment-children, and captee-less shapes.
- **The final captor and its inline terminal are measured
  TOGETHER.** Placing the last link by its own width would leave
  the terminal overflowing past it; the spec's deep-chain exemplar
  (`<t3(>` wraps with `foo`) pins this.
- **Text terminals keep the 72/73 fixture exactly** via the one
  remaining FALSUM path: a spine that IS a single link whose text
  doesn't fit even at line start refuses, and the caller emits the
  plain open form. Mid-chain, the same condition renders the last
  element in plain form one step below the captor line (captors
  above it still capture).
- **Only 2 root expectations churned** (probatio_stml case VIII:
  wrapper `</>` line deleted) and 9 silva arbor assertions — all
  the same disease: string-matching non-collapsed forms.
  Corruption-substitution patterns need care: after M2b the
  packing can put a line break where a trailing space was
  (`"<lex-int(> "` matched nothing — the break landed right after
  it), and `strstr` targets should drop the trailing `>` and
  mutate INSIDE the name so the tag form (captor or plain)
  survives in either emission.
- Eligibility stays conservative: exotic children
  (crudus/multilinea/fragmentum/retro/farcimen/multi-paren,
  comment children) refuse conversion and keep today's forms —
  M4 corners.

Gates after: stml family 4/4 (incl. sedes parity on stacked
extents), silva 50/50 (corpus 73+154 at 0 vitia), both canon
freshness gates, amalgam re-verified (hospes 39/39).

## 2026-08-24 (M2b sequela) — blank lines are paragraph separators, not layout

Fran ran the formatter over silva/grammatica/c89.stml and spotted
pretty eating every authored blank line. Decree: N authored blanks
at block level → min(N, 2) emitted — paragraph separation is
MEANING-adjacent, not regenerable layout. Finds:

- **The blank count lives entirely in the FOLLOWER's spatia_ante**
  by the ownership law — the prior's post takes exactly the first
  newline of the gap, everything beyond lands in the next node's
  ante. So `_lineae_vacuae` counts ante newlines over a baseline.
- **The baseline is positional and asymmetric**: I for the first
  block child (open-tag→first-child gets the WHOLE run, so one
  newline is just the line break), ZEPHYRUM between siblings AND
  before the close tag (the last child's post already took the
  first newline; clausurae holds only the extra). Getting
  clausurae's baseline wrong (I) would silently delete
  single-blank-before-close.
- Three call sites: element block branch (per-child + clausurae)
  and the documentum loop (i > 0 only — leading file blanks
  drop). Collapsed spines and inline content stay tight by
  design.
- Fixed-point stable by construction: the emitted form reparses
  to the same ante newline counts. The 148-file pulchrum corpus
  (which carries real paragraph breaks) went green on the first
  compile.
- c89.stml live: 1217 → 843 lines (was 748 blank-less) — the
  structural compression stays, ~95 paragraph breaks return.

## 2026-08-24 (M2b sequela II) — the depth floor: tectum = max(72, indent + 40)

Fran named the degenerate zone: under heavy indentation the flat
72-column budget leaves almost no content room. Measurement showed
it is not hypothetical — lib/xar.c's arbor document reaches
36 columns of indent (36 of room), and the packer's wrap rule
COMPOUNDS the problem at depth: each wrapped line is one step
deeper and two columns narrower, so past ~34 indent the output
degenerates into a staircase of one-link overflowing lines.

The fix is a content floor: tectum = max(LXXII, gradus*2 + XL),
computed per LINE (gradus, so a wrap's fresh line gets its full
room). Nothing changes at indent ≤ 16; deeper, total line length
grows with depth but content room never drops below 40. The
budget derives only from tree depth → deterministic → fixed point
unaffected. One site (the packer's single width check).

Fixture: programmatically built 20-level block nest (`<n><z/>` ×20
— two children per level so no collapse) with a 35-column spine at
indent 20: flat-72 refused it (32 of room), the floor captures it;
plus a deep fixed-point assert. Gates: stml 4/4, silva 50/50
(arbor gates re-derive both sides, no goldens flinched), canons
recentes.

## 2026-08-24 (M3 accessores) — valor + fluxus: the meaning readings land

The triad is complete (§2). Finds:

- **`stml_textus_valor` needed ZERO kind logic** — it is a pure
  recursive concatenation of text-node valors (comments and
  processing instructions contribute nothing). M1 put the truth
  in the model: `\` content is stored dedented, raw is stored
  verbatim, flow edges live in trivia. The accessor just reads.
- **`stml_textus_fluxus` is a two-flag state machine** (pendens =
  a soft space is owed, emissum = content has been emitted).
  Every §2 edge rule falls out of the flag discipline: edge
  trivia never emits (pendens without emissum is dropped),
  consecutive soft boundaries merge (pendens is idempotent),
  comments are transparent (contribute no content, their
  boundaries still set pendens). Same-line ws runs are literal —
  INCLUDING ws-only text nodes (`<sep>   </sep>` flows to
  "   "), consistent with the M2 content doctrine.
- **Kind outranks the accessor**: marked (`\`/`!`) elements
  contribute their valor verbatim through `_fluxum_octetos_
  appendere` — first byte flushes the pending space, empty
  contributions never flush (so a marked element with no text is
  boundary-transparent like a comment).
- **Migration (§9) was smaller than the spec guessed.** canon:
  ONE site (typed-text judging) moved to valor — behavior-neutral
  because the call site's own praecidere already did the trim;
  the two internus sites are identity-sensitive (sealed keys) and
  stay on the bridge FOREVER. silva arbor: NO migration — its
  `_textus_directus` carries a two-part contract (writer refuses
  ws-only values ⇄ reader skips ws-only nodes) that valor
  deliberately does not share; only its stale rationale comment
  was fixed (the newline-injection class it feared died in M1 —
  those are trivia now, never nodes). The remaining ~17 consumers
  migrate when touched, never forced.
- **Amalgam note**: the new accessors are unused by silva, so the
  excludenda harvest prunes them (regenerated, 143 names). No
  struct changes → no silva.h hand-mirror work, no lab re-vendor
  needed this milestone.

## 2026-08-24/25 (M3 re-involutio) — flow re-wrap + the arbor collision

The fluxus-gated liberty landed: pretty OWNS flow prose
(always-canonical). Finds:

- **The writer machinery composed cleanly**: `_fluxu_evasum_scribere`
  (joined emission) in the spine's text terminal + a new
  `_textum_refluere_conari` (block-with-text greedy fill) as the
  hook's fallback after spine refusal. `_valor_capturabilis` v2:
  unilinear requirement DROPPED (joined at emission), LAST-byte
  non-white ADDED — closing a latent M2b hole (a trailing space in
  a captured text died into post on reparse; never bitten, now
  pinned) and guarding block-fill edges the same way.
- **Gate B relaxed modulo fluxus for FLOW text only** (harness-side
  `_fluxu_pares`, newline-bearing runs ≡ one space) — `\`/`!`
  content stays byte-exact.
- **The arbor collision — the session's big find.** The plagula
  gate (C → STML → pretty → reparse → emit C → memcmp) reds the
  moment pretty owns prose: arbor stored CODE BYTES (comments,
  lexeme values) as flow text. Doctrine says the DOCUMENT was
  mis-declaring; the kind ladder is the fix. THREE attempts,
  each taught something:
  1. `\`-marking multi-line values: valor round-trip exact ONLY
     in the canonical shape — a hand-built text node needs
     `spatia_ante="\n"` or the writer glues line 1 to the tag,
     line 1 exits the dedent's common-prefix computation, and
     continuation lines lose their own leading space.
  2. Even so, LONG SINGLE-LINE comments hit the block-fill —
     width-based marking is unprincipled. The honest kind for
     code bytes is RAW (`!`): never touched, verbatim both ways,
     uniform for one line or many. With a LOUD guard: a value
     containing its own closing sequence (`</tag>`) refuses.
  3. Raw is only legal for TEXT-ONLY elements — mixed lexeme
     elements (value + involucra) had the raw scan EAT the
     `<post>`/`<expansio>` markup as text. And the single-child
     test must run AFTER ALL children attach (origo lands late —
     the first placement marked, then expansio joined, boom).
     Final shape: trivium elements always raw; lexeme elements
     raw IFF still single-child at the END of the builder; mixed
     lexeme values stay flow — safe because v1 re-flow skips
     mixed content.
- Result: plagula gate 78+154 at 0 divergences THROUGH a
  pretty-print — reformat-safety of arbor documents HOLDS. The
  planned "fidelity-mode fallback" was never needed (and was
  mechanically impossible anyway: a fresh tree has no trivia, so
  arbor's only emission IS pretty).
- c89.stml pretty: 197 → 33 over-72 lines; every survivor is an
  attr-heavy open tag = the reserved §1.6 multi-line-attribute
  layout, now the ONLY remaining width feature.
- Churn: 1 arbor assertion (`<lex-integer(> 0` → raw form), the
  73-boundary fixture (over-budget text now block-fills instead
  of overflowing inline).

## 2026-08-25 (decretum quartum) — capture-stacked fill: the M2b invariant completes

Fran, eyeballing c89-formatted.stml, saw that the block-filled
text form could drop its close tag by using capture syntax:
`<productio ... (>` + filled lines, no `</>`. Confirmed sound:
a text RUN splits only at markup — never at spaces — so the
stacked fill parses as ONE captured text node; rule A owns the
leading boundary; and the writer cannot produce the text-sibling
merging hazard (text-bearing parents lay out inline, never
stacked). Now canonical: `_textum_refluere_conari` emits the
captor frame via `_vinculum_scribere` (canonical paren + TAG-ONLY
sedes — captor semantics, the parity gate confirmed) and the
closer block is deleted. The M2b invariant is now exact: close
tags exist precisely where multi-child block form does — EVERY
single-child element is capture form. c89.stml pretty:
1197 → 1021 lines (one `</>` per filled element).

## 2026-08-25 (decretum quintum) — multiline attributes: the §1.6 promise redeemed

The last formatting frontier c89-formatted.stml exposed: 33
over-72 lines, every one an attribute-heavy `<terminalis .../>`.
Fran's decree: one attribute per line, names RIGHT-ALIGNED into a
shared `=` column (the C89 declaration-column aesthetic), column
directly after the tag name with a two-space floor from `<`; long
names or wide capture closers push the column right. Gluing
closers (`>`, `/>`) stay glued to the last value; CAPTURE closers
take their own line, `>` landing in the `=` column.

Implementation notes:
- The decision is ARITHMETIC (`_tagum_latitudo_inline` +
  `_attributorum_columna`), not render-and-rollback — attribute
  values never carry newlines, so inline width is a plain sum.
  Cheaper and clearer than the packer's truncare pattern.
- FOUR emission sites, one shared writer
  (`_attributa_multilinea_scribere`): the normal-element branch,
  the ANTE-captor branch (both pulchrum-only — inline children
  mid-line never break), the refluere captor
  (`_vinculum_multilineum_scribere`), and the packer's line-start
  acceptance point — a link ALONE over budget converts in situ,
  `initium_lineae` re-bases to the closer line (length = columna
  + 1) and the spine continues there.
- The parser needed NOTHING: `_tok_legere_attributa` already
  skips newlines between attributes and banks pre-closer
  whitespace in spatia_prae_finem/spatia_intra_tagum — M1's §1.6
  modeling was built for exactly this day. All 9 born-red
  fixtures went green on the first full implementation pass.
- `_tectum_lineae` extracted (the depth-floor formula existed
  twice; lex superficiei duplicatae).
- c89-formatted.stml: 33 → 2 long lines (both single attributes
  whose VALUES exceed 72 alone — values never break, report-only
  per decree), 1021 → 1105 lines, full-document fixed point
  verified by running the formatter on its own output.

## 2026-08-25 (decretum quartum emendatum) — hanging fill: the closer line is a tag line

Fran's refinement, eyeballing the fifth-decree output: the
capture-stacked fill opened a fresh line under `(>` even when the
text had room beside it. Now the text starts ON the closer line
(`<t(> primus ...`, or the aligned `(>` line of a multiline-attr
captor) and continuation lines hang aligned under the first text
column — the same behavior the spine packer already had for
ELEMENT captees after a multiline closer, now uniform for text.

Two guards make "if it has the width budget" precise:
- FIRST-ATOM TEST: hang only if the first atom fits on the closer
  line within the tectum (render + truncare — escaping can change
  length). This preserved the 72/73 boundary fixtures untouched:
  a single over-wide atom is genuinely narrower in vertical form
  (indent+1 < hang column).
- ROOM GUARD: hang only if the hanging column leaves ≥ XL columns
  under the tectum (the depth-floor number). Without it a wide
  INLINE captor (`<t a="...40..." (>`, column 51) would hang its
  continuation in a sliver; those and deep indentation keep the
  vertical fill.

Implementation: `_atomi_finis` extracted (the atom scanner was
about to be needed twice); the fill loop parameterized on
`columna_impletionis` (spaces count — hang column or
(indent+1)*2) so one loop serves both forms. c89-formatted:
1105 → 1091 lines, the productio blocks now read exactly as
Fran sketched. Fixed point verified on the full document.

## 2026-08-25 (decretum sextum) — multi-captee capture: `((>` and `(((>`

Fran's decree: 2-3 children under ~10-15 lines collapse to
multi-paren capture form, children stacked on following lines,
never on the tag line, never beyond three. The stacking instinct
is what makes it SOUND: same-line captees would need a space
between them, and a same-line whitespace run is a text NODE that
corrupts the capture count — the M4 counting corner. Stacked,
the inter-captee whitespace bears newlines and is trivia (§1.3),
so counting is exact. `STML_CAPTURA_MULTIPLEX_LINEAE = XII`.

Three bugs the gates caught before shipping, each a lesson:

1. **Blank baseline drift (Gate A).** The first captee's blank
   count baseline must depend on the parent's AUTHORED form: an
   open tag gives the whole whitespace run to its first child
   (baseline 1); a captor keeps the first newline in its own
   post (§1.2), so a reparsed captee carries one fewer
   positional newline (baseline 0). With a fixed baseline the
   fixed point lost one authored blank per pass.
2. **Glued cascade under spines (Gate A).** `_spina_idonea`'s
   chain test refused a multi-paren captor child (neither link
   nor planum), so reparsing our own `<a(> <b(>\n  <c((>...`
   made the WHOLE spine refuse and fall into the CAPTIO_ANTE
   glued cascade (`<a(> <b(> <c((><d/><e/>`). Fix: the chain
   accepts a multiplex-ELIGIBLE element as a block terminal
   (`_capturae_multiplicis_idoneum`, structure only).
3. **Exponential height check (plagula gate, by hanging).**
   Height-by-rendering with rollback re-renders children at
   every nesting level a rollback occurs — 2^depth on deep
   over-height trees; probatio_silva_arbor_plagula burned 10+
   CPU-minutes before I killed it. Replaced with
   `_lineas_aestimare`: tree-derived, early-bail, conservative
   (does not simulate spine/multiplex collapse — deeply chained
   content over-estimates and stays block, which is why arbor
   documents kept their shapes and silva churn was ZERO).

Sedes: the captor's nota is appended AFTER its children — the
sedes table contract is post-order (elements note at close), and
the parity fixtures hit a capture conversion for the first time.
NOTE the latent sibling issue: SPINE link notas are appended in
pre-order (link-by-link) and would violate the same contract —
unexercised by any fixture today, filed as a quaestio.

c89-formatted: 1091 → 1041 (50 `</>` lines deleted); the
close-tag invariant is now "block form only": 4+ children,
over-height, mixed content, or ineligible children.

## 2026-08-25 (sedes spinae) — post-order restored via double reversal

The quaestio filed during the sixth decree (01M0WVG98N), closed
same day while the packer knowledge was warm. The spine packer
noted link sedes PRE-order (each link as written — a captor's
tag-only extent is fully known at emission, which is exactly the
convenience that created the drift; normal elements can't note
early, so the post-order contract was enforced by necessity
everywhere except here). Born-red fixture: FIXTURA_SPINAE
(`<a><b><c><d/><e/></c></b></a>`) — the first sedes fixture whose
pretty form contains a spine; writer table [a,b,d,e,c] vs
post-order [d,e,c,b,a].

Fix: `_sedes_postordinare` — in-place DOUBLE REVERSAL over the
slice appended since spine entry, no allocation: [L1..Lk, T...]
→ reverse whole → [rev(T), Lk..L1] → reverse the T part →
[T..., Lk..L1]. Link counting (`vincula`) increments at the loop
bottom and at each terminal success; rollback paths never count
(their notas are truncated). Reorder called at every success
exit; full refusal needs none (everything truncated). Verified:
[a,b,d,e,c] → [d,e,c,b,a] on the fixture, exercising spine +
multiplex composition in one shape.

DISCOVERED WHILE FIXING (filed, not fixed - 01M0X12PWS): the
nucleus end-of-case notation includes CAPTEES in authored
captors' extents (CAPTIO_ANTE glued branch, retro, farcimen) —
parser records tag-only. Same latent-contract family; no fixture
exercises authored captors in the sedes suite yet.

## 2026-08-25 (later): captor extents — nucleus branches note tag-only (01M0X12PWS)

The sister lie filed during the spine-order fix, now retired.
The nucleus's bracket notation (grab `initium_sedis` at case
start, note `[initium_sedis, longitudo)` at end-of-case) spans
tag + captees for every authored captor, because capture
branches emit their captees INSIDE the case. The parser records
the capture TOKEN only (`_parser_creare_nodus` presets the
token's span; capture parsers pin `finis_ultimus` right after
consuming their own token; reparenting never touches positus).
RETRO/FARCIMEN were worse: captees emit BEFORE the tag, so
`initium_sedis` didn't even point at `<` — the fixture's
`emissum.datum[initium] == '<'` anchor caught that face.

Born-red first: FIXTURA_CAPTORUM (authored ANTE, retro,
farcimen, fragment captor, crudus captor — XI elements, both
modes) + a hand-numbered absolute anchor (`<r><t (> <a/></r>`,
t = [3,8)) so both oracles can't share the "extents include
captees" falsehood. 13 red, all four defect faces.

Fix shape: extracted `_sedes_notare` (the nota block existed
verbatim 4×: vinculum, vinculum multilineum, multiplex,
end-of-case — one derivation now), then each capture branch
notes its OWN extent and resets `initium_sedis` to the -I
sentinel so the end-of-case bracket skips:

- ANTE element: `finis_tagi` right after the closer (both attr
  paths converge there); nota AFTER captees (post-order).
- ANTE crudus: the capture LEXEME carries the captured line
  itself ('\n' left to the stream), so parser extent = tag +
  line; `finis_tagi` re-grabbed after the children loop, before
  the fidelity terminator. NOT tag-only — match the token, not
  a slogan.
- RETRO: `initium_tagi` grabbed after captees (tag stands after
  them in flow order); nota right after `>` — captees already
  noted, post-order free.
- FARCIMEN: tag extent bracketed between captee[0] and the
  rest; nota after the trailing loop.
- Fragment ANTE: same as ANTE element, before its `frange`.

Post-order came free everywhere because every branch's nota is
emitted after its captees' recursion — no `_sedes_postordinare`
needed here (that machinery stays spine-only).

Trap for the future: "tag-only" is the RULE but the crudus
captor is the exception — the extent contract is "the capture
token's span", which for crudus includes the captured line.
Any new capture form should ask what its TOKEN spans first.

## 2026-08-25 (later still): the 8.6s sedes suite — unsigned underflow spin

probatio_stml_sedes sat at a rock-stable ~8.6s for 465 tiny
assertions — insensitive to adding circuits, which is the tell
for a FIXED cost, not workload. sample(1) put 100% of it in
`_sedes_postordinare` → `_sedes_revertere` → `xar_obtinere`;
per-section timestamps convicted ordinaria(pulchrum=1) alone.

The chain: radix is multiplex, so captee `caput` emits while the
sedes table is EMPTY (captor notes post-children). caput is a
one-link spine with a TEXT terminal — text carries no nota, so
totum == vincula and the second reversal gets quantum ZEPHYRUM
at initium ZEPHYRUM. In `_sedes_revertere`,
`b = initium + quantum - I` = 0+0-1 → i32 is UNSIGNED → b ≈ 2^32
→ `dum (a < b)` spins ~2^31 bounds-checked no-op iterations.
Zero swaps (the indices are never in range together), so every
gate stayed green — pure heat, 8.4s of it, since the suite's
birth. Only fires when a text-terminal spine is the document's
FIRST emission (any other position: b = initium-1 < a, loop
never entered) — hence exactly one circuit burning.

Fix: quantum < II guard in `_sedes_revertere` (nothing to
reverse; makes the underflow arithmetic unreachable for every
caller). 8.59s → 0.25s, verdict identical 465/465.

House landmine census: this is the same "i32 is unsigned" class
as the clock-delta bug — subtraction in index/count arithmetic
needs either a guard proving minuend ≥ subtrahend or s32/s64.
A loop bound derived by subtraction deserves suspicion on sight.

## 2026-08-26 — attribute-elements `<@titulus=>` (macros spec §6.3, v1.5 step 1)

Base-format repair shipped end to end: any element may spell an
attribute in element form; a `"` in an attribute value is finally
representable (inline attr values are RAW both ways per the header
doctrine, so `"` was irrepresentable inline — text children decode
entities, so `<@t=>x"y</>` carries it).

Shape (percentum pattern exactly): ordinary ELEMENTUM, titulus `"@"`
shared, name in new field `attributum_titulus` (interned; non-NIHIL =
the marker). Lexer blessing is LOCAL to the opening-tag path in
`_tok_legere_tag` — closing tags, retro/farcimen captures still refuse
`@`, so undecreed forms stay unlexable. `=` after the name is tag
syntax, consumed; the `@` stays part of the token valor and the parser
splits it in `_titulum_ex_tokeno_ponere` (three sites: elementum,
auto_claudere, captio_ante). `!` and `\` refused on attr-elements
(raw-mode close matching needs a named closer they can never have).

Traps found and decided this session:

- **The writer normalizes ALL empty pairs to self-closing**
  (`<a></a>` → `<a/>`, measured) — so the planned two-spelling
  distinction (empty-pair = `""`, self-closing = tombstone) was
  unrepresentable without a new fidelity bit. Resolution (decree
  amended in spec §6.3): BOTH empty spellings = TOMBSTONE (explicit
  absence; capere NIHIL; blocks future inheritance); empty-STRING has
  no element spelling and needs none — inline `m=""` always works.
- **Pretty: attr-elements join the exotic-form family** (same
  exclusion list as augmentum in `_spinae_liberum_unicum`,
  `_elementum_planum`, `_capturae_multiplicis_idoneum` parent+child,
  `_terminalis_inline`). Reason it is a LAW not a convenience: spine
  terminal collapse (`_fluxu_evasum_scribere`) and refluere re-wrap
  text BYTES, which is canonical layout for prose but VALUE CORRUPTION
  for an attribute (a string literal, not flow). Capture-form pretty
  for unilinear values is a possible later refinement — the guard to
  keep is: never let reflow touch an attr value.
- **Position law runs as a POST-PARSE pass** (`_attributa_elementa_
  probare`, called after `_processare_captiones`): prefix-of-children
  = parent's attribute (TEXT-only children, no duplicate vs inline or
  element form), immediately-after-`#@`-call = call argument
  (subtrees legal, run continues through consecutive attr-elements),
  else loud STML_ERROR_ATTRIBUTUM with named cause (multilinea
  pattern). Post-parse because captures REPARENT: `<a (> <@m=>x</>`
  is legal — the final tree is judged, which is also what consumers
  see. Third copy of the `#@` guard (stml_macros `_est_vocatio`,
  canon `_transclusionis_petitum`, now this) — one law, three sites.
- **capere dual lookup**: attributa scan, then child-PREFIX scan
  (stops at first non-attr child); tombstone (empty liberi) → NIHIL;
  value = single text child's valor (already entity-decoded).
  Equivalence is in MEANING, not bytes: inline spells raw, element
  spells text-law.
- **Canon dual view**: attr-elements never in `_liberos_effectivos`
  (parent-bound = attributes, call-bound = quoted call material);
  parent-bound judged AS attributes via extracted
  `_attributum_praesens_iudicare` (shared with the inline loop —
  suggestion machinery included); required-attr check needed ZERO
  work — it already goes through `stml_attributum_capere`, so the
  dual lookup satisfied it for free ("equivalence free for accessor
  users", as the decree predicted). Tombstone: DEEST fires via capere,
  name still judged (unknown tombstone = IGNOTUM).
- Amalgam: hand-mirror `SilvaStmlNodus` in silva/amalgama/silva.h
  needed the new field (compile clamavit, as designed).

Gates at close: root 141/141, silva 50/50, canon corpus untouched,
amalgamare VERIFICATUM (hospes 39/39), natura rebuilt.

## 2026-08-27 — `<>` rehabilitated as anonymous-fragment sugar (DISTRIBUTIO step 1)

`<>` was a tolerated form with no meaning (empty-titulus element,
strictum TITULUS_VACUUS). Now it lexes as an anonymous fragment —
sugar for `<#>`, exactly parallel to `<(>` = `<# (>`: ephemeral,
writer-normalized. One new lexer function (`_tok_legere_fragmentum_nudum`,
dispatched on `<` + `>`), no writer change at all.

The companion widening: the anonymous closer `</>` now closes
fragments. MEASURED before building: `<#>x</>` was a parse FAILURE
(status 4), so this is a pure widening, not a behavior change. Two
sites: an extra break in `_liberos_legere` (guarded by the
FRAGMENTUM_CLAUDERE terminator, so element parsing is untouched)
and the consume in `_parser_legere_fragmentum`. `</#>` remains
canonical; a named false closer (`<a><>x</a>`) still fails loudly.

NAMED strictum fixture change: the TITULUS_VACUUS fixture moved
from `<>x</>` to `< >x</>` (verified still condemned). The vitium
survives for genuinely malformed elements; `<>` never reaches the
element path anymore.

Trap for the future: elements PRESERVE an authored `</>`
(clausura_anonyma — first-class form), fragments NORMALIZE it away.
My first fixture expected `</nota>` from an authored `</>` and the
writer correctly disagreed.
