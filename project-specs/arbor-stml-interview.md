# Arbor ⇄ STML — Colloquium (raw transcript)

2026-08-19, post-compact session. Method: exhibits-first (see
arbor-stml-exhibita.md, written same session against the real baked
registry), then structured interview via option-questions with STML
previews. Fran's selections recorded verbatim; the framing text is
condensed from the conversation. Precedes arbor-stml-spec.md per house
method (raw transcripts precede specs).

## Staging (pre-exhibits)

**Q0.1 — Process order for the run-up to the M1 spec?**
Options: exhibits-first micro-simulatio (seam reads + hand-written
candidate STML anchoring the interview) / interview first, §7 verbatim /
straight to simulatio+spec.
**Fran: Exhibits-first.**

**Q0.2 — Which downstream direction drives M1's scope (the §7 opening
question)?**
Options: pattern substrate (canonical-minimal, registry-driven; what
the M2 fidelity oracle also wants) / explorer / storage-interchange /
speculum.
**Fran: Pattern substrate.** (Explorer still lands at M3 unchanged.)

## Batch I — the structural forks

**Q1 — Document shape: inline (tokens live where the tree references
them, fragments for sharing) vs token-table (flat lexemata pool, tree
references by id)?** Context: M2's full parsura is natively
table-shaped (parsura->lexemata IS a flat stream) either way.
Options: inline + sibling forms (M1 subtree docs inline; M2
full-parsura a second, table-shaped form of the same dialect) / table
from day one / decide at the M2 seam.
**Fran: Inline + sibling forms.**

**Q2 — Stratum-0 token positions (byte/linea/columna): carry as
attributes or derive at load** from an envelope anchor + emission walk?
(Def-site tokens in origo pools carry theirs either way — another
file's coordinates.)
Options: derive at load (canonical doc cannot lie; gate compares
RECOMPUTED fields = the representational audit running mechanically;
joins pater/initium_lineae in the fixup class) / carry as attributes /
derive with carry-on-request projection flag.
**Fran: Derive at load.**

**Q3 — Locus representation: uniform wrapper elements always, or merge
TOKEN-species loci** (`<tok_terminator genus="semicolon"/>`)?
**Fran: Uniform wrappers.** (Genus and locus strictly alternate by
depth; one reading rule; pattern addressing stays clean.)

**Q4 — Lexeme representation: genus as element tag
(`<identificator>n</identificator>`) vs uniform `<lexema genus="…">`?**
Context: measured namespace disjointness (Latin node genera vs English
lexeme names); one mangle rule (lowercase + `_`→`-`).
**Fran: Genus as tag.**

## Batch II — text, trivia, references

**Q5 — The whitespace law.** Problem surfaced by the exhibits: STML's
pretty writer indents around children and stml_textus_internus is
VERBATIM, so a pretty-printed valor element would gain whitespace on
reload. Options: canon-driven significance (valor-bearing elements
whitespace-significant + always written tight; structural elements
ignore whitespace-only text at load; pretty docs and hand-written
fixtures load identically) / canonical = non-pretty single line / raw
`!` framing for valor.
**Fran: Canon-driven significance.**

**Q6 — Trivia in canonical form: compact lenses or verbatim text?**
Options: compact lenses (`<spatia n="4"/>`, `<tabulae n="2"/>`,
`<nova-linea/>` +crlf, `<continuatio/>`; comments stay entity-escaped
text — valor fully determined, nothing lost) / verbatim text everywhere.
**Fran: Compact lenses.**

**Q7 — The dual reference law: content shared between tree positions =
fragments + transclusion (`<#lex1>…` / `<<#lex1>>`); identity pointers
(origo) = `#id` attribute values. Accept or force one mechanism?**
**Fran: Dual law.**

**Q8 — INDEX-species loci (ambiguus.canonica, ramus.conditio_id)?**
Options: text content arabic (`<canonica>0</canonica>` — wrappers
contain their value, faithful to the C-side s32) / attribute /
fragment-id reference.
**Fran: Text content, arabic.**

## Batch III — naming, versioning, architecture, gate

**Q9 — Naming.**
Options: arbor/parsura (subtree envelope `<arbor>`, full-document form
`<parsura>`, dialect "arbor") / silva-prefixed family / fresh coin.
**Fran: arbor / parsura.**

**Q10 — Registry version skew.**
Options: name + sigillum, refuse loud (envelope carries
`grammatica="c89"` + a registry seal computed over the genus/locus
tables at runtime; loader refuses on mismatch — gate at birth) / name
only, warn / nothing.
**Fran: Name + sigillum, refuse.**

**Q11 — Reader/writer architecture.**
Options: generic registry-driven pair (one reader + one writer
parameterized by SilvaRegistrumCoctum, as silva_scribere already is —
all three grammars served, no generation step; canon projection stays
generated FROM the registry separately) / generated per grammar
(canon_coquere lineage) / hand-written c89-only.
**Fran: Generic, registry-driven.** (This option emerged from the
exhibits — §5 of the visio had only listed generated-vs-handwritten.)

**Q12 — What anchors M1's round-trip gate? (multi-select)**
Options: roundtrip corpus subtrees / mined pure subtrees from lib/
(est_fons_purus as miner) / hand-authored golden trees.
**Fran: Roundtrip corpus subtrees ONLY.** Mined subtrees and hand
goldens deliberately deferred — named in the spec with landing spots
(no silent caps).
