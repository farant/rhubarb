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
