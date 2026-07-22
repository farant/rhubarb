# silva_annotationes worklog

## 2026-07-21 — Chunk A: the collector (born with the amalgam seat)

Park 01KY3D7EJP chunk A; spec = project-specs/annotationes-stml-spec.md.
Module shape: trivia walk (TOLERA pattern, radix chain included) →
tag-at-comment-start anchoring → decoration pre-pass → stml_legere →
byte-based attachment (SUPRA/INTERIOR/PLAGULA). probatio: 9 sections,
63 pins.

**Seat decision (Fran):** stml vendored INTO the silva amalgam.
The pre-amalgam-arc cost argument was stale — fontes lists are
aedilis-derived and excludenda compiler-harvested, so the real hand
cost was ~12 rename entries + the silva.h reading subset + 2 policy
lines. internamentum rides along (stml interns names); selectio is
in the closure but ships EXCLUDED wholesale (dead subsystem — via
retro documented in fontes_politica.sh for the day annotation
selector queries want it).

**Bug of the day — the extensionem accumulator contract.**
`silva_nodus_extensionem` does NOT initialize its out-params; the
header's "*minimum initia < 0, *maximum initia 0" is an instruction
TO THE CALLER (it's an accumulator — that's why the recursion passes
the pointers through). Both my call sites passed uninitialized
locals: the probatio's stack slots happened to hold 0 → a
plausible-looking [0,94] "whole file" extent; the module's held a
spilled 64-bit unit POINTER → min=1/max=0x50082E80 (the pointer's
two halves — the decode that cracked the case). Every SUPRA pin in
the first run passed BY ACCIDENT (garbage min=1 > offset). The
hospes extensionem block had the contract right all along
(minimum=-1, maximum=0 before the call) — the precedent existed,
I just hadn't read it. Fix: initialize per contract in _affigere.
NOTE: this is exactly the codex-71 class (uninitialized read),
invisible to FLUXUS because the writes hide across the call
boundary — the interprocedural-summary gap the annotation type
layer (park 01KXTX7FA3) exists to close. The bug advertises the
product.

**Tree shape:** commissio->radix is a SILVA_VALOR_LISTA of
top-level sentence values (not a node) — "fortasse religata" is
real. silva_annotationes_unitates() accepts both LISTA and NODUS
radix shapes and is public (probatio needs pointer-identical units;
chunk B's index wants titles).

**Attachment semantics as shipped:** extents count REAL tokens only
(trivia excluded), maximum = one-past-end of last token. End-of-line
annotation after a unit's `;` therefore binds SUPRA to the NEXT
unit (the recorded wrinkle). INTERIOR = byte containment. Error
line mapping: fons = annotatio.linea + linea_erroris - 1 (pre-pass
preserves line count; columns shift under decoration strip).

**Amalgam integration battle log** (harvest fixes live in
tools/amalgama_excludenda_generare.sh + tools/aedilis.worklog.md):
- S41 guard caught `_est_littera` (selectio vs silva_lexema) on
  first contact → renamed `_est_littera_selectoris` at source.
- InternamentumChorda restructured to the SEPARATED typedef-opaque
  idiom (typedef + body as distinct units) so CADENDA can drop the
  typedef while the body stays internal (SilvaOraculum pattern).
- The harvest cascade seed was a STALE HAND SERVANDA:
  SERVANDA_FRIATIONIS kept only fnv1a while the new libs made the
  tabula literis path live → signal B ("callers of servanda-dropped
  fns are dead-in-context") amplified one stale line into a
  hollowed-out live core, swallowing stml_legere itself. Cure:
  fnv1a_literis added to servanda + PROTECTA guard (non_statica
  names may never be excluded — refusal names the seed).
- internamentum_globale's file-scope statics moved INSIDE the
  function so its exclusion is self-contained (the
  unused-variable-under-excluded-caller class; selectio's cache
  statics were the same class, solved by the wholesale drop).
- Final: harvest fixpoint 6 gyri / 147 names; amalgamare
  VERIFICATUM (standalone + hospes 33/33 + nm 0 + censura);
  silva.c ~2.15 MB.

**Bootstrap deadlock note:** a failed amalgamare leaves a BROKEN
committed silva.c on disk, and bin/aedilis (which the fontes
generator rebuilds) links silva.c → the generator dies against the
broken amalgam. Loop-break: `git checkout -- silva/amalgama/silva.c`
(HEAD is always a VERIFICATUM amalgam), then run the chain forward.
Remember this before diagnosing "aedilis suddenly won't build."

**Launcher lists:** new fontes module nobody includes yet broke the
principal-closure assumption in silva/*_fontes_generata.sh (and four
launchers carried FROZEN glob expansions). tools/
silva_fontes_generare.sh (no args) now owns all 12: live glob
expansion + principals persisted in each snippet's "# principalia:"
line.

## 2026-07-21 (evening) — Chunk B: index + mint + bench verb

Three deliverables shipped in one arc:

**Extraction + locator (fontes-side, pure).**
silva_annotationes_identitates: tree walk (document order) reads
given identities (<nid v="U"/>, nid="U" on any element) and
petitiones (boolean nid / boolean v on nid / bare <nid/>); a
SEPARATE textual scanner over the RAW comment bytes locates splice
offsets (cleaned-text offsets don't map to fons; raw ones do) —
in-tag word scan with quote skipping, inline decoration skip, tag
-name context for the v rule, closing tags never mintable. The two
sides are zipped by order with a count+genus correspondence check;
dissent (e.g. literal nid="true") leaves offset -1 and the
instrument refuses loud. 100/100 probatio pins, all offsets
byte-check asserted (memcmp at offset, not hand-counted).

**One mint (lib/moneta.c).** scrinium's ULID implementation
EXTRACTED to a standalone lib (scrinium vendors sqlite — 250k
lines to link for 26 characters was absurd); scrinium_ulid now
delegates. One identity space, one coin press: board and code
draw from the same well. moneta.c joined exclusiones.txt (POSIX
gettimeofday lane, beside scrinium/tcp_posix).

**The instrument (identitates.c + identitates.sh).** --renovare
walks prefiltered candidates (git ls-files + cheap grep; generata/
amalgama/fixa excluded — the amalgam EMBEDS fontes comments, so
indexing it would duplicate every nid) → build/identitates.tsv
(11 columns incl. byte extents, so --solvere slices annotation +
unit source from disk with NO reparse). Mint: dry by default,
--scribere applies ascending-offset splices to a temp file,
RE-COLLECTS and verifies (petitiones→0, identities grown by
exactly the splice count, length delta exact), renames only on
success. -porta gate: fixture copy → mint → sed-strip minted
values → byte-identical to fixture + re-dry finds nothing.

**First real identity:** 01KY3TRZ2GY3HG9Y4WQRSGR2BX — the
collector's own intentio (res="01KY3D7EJP"), minted by the
instrument into the file that implements it, resolved by
./silva/identitates.sh '##01KY3TRZ'. The mint's monotonic ULIDs
showed in the porta run: ...DQES / ...DQET consecutive.

Notes for the future:
- identitates.c carries the examen capita-praebere recipe
  (duplicated ~90 lines, house pattern for principals) but NO
  lexicon/semantica — collection, not judgment.
- The extensionem calls in --renovare follow the accumulator
  contract (init -1/0 before the call) — the lesson from this
  morning, now with a comment citing it.
- Launcher candidate prefilter deliberately over-matches (probatio
  string literals) — parse-level anchoring makes false positives
  harmless (one wasted parse), false negatives impossible.

## 2026-07-21 (night) — Chunk C: the codices (74-76)

Three examen codices, all DOMESTICUM (verdict-neutral at birth,
graduation by decretum): 74 ANNOTATIO_MALFORMATA (anchored-but-
unparsable — the lint half of never-silently-prose; position =
annotation line + linea_erroris - 1, fallback comment start), 75
NID_DUPLICATUM (same nid twice in ONE file, second occurrence
fires; cross-file = CAUTIO in identitates --renovare via a
seen-set — the examen judges one file, the index owns global
truth), 76 IDENTITAS_INVALIDA (nid = exactly 26 Crockford chars;
res= citations = valid >= 6 prefix, FORMAT only — resolution is
chunk D's, where the board resolver lives).

Seat A dividend collected: semantica calls the collector directly
(_annotationes_examinare hooked beside _toleras_irritas_examinare)
— the callback seam chunk C would have needed under seat C was
never built. Annotations are trivia (no node), so a raw-position
diagnostic emitter variant (nodus NIHIL licet; MACRO_DOMESTICUM
manual-position precedent). Petitiones are never judged — minting
is the instrument's business, not a defect.

Battery: 9 new e2e cases in probatio_silva_examen (fires + the
silences: prose, valid annotation, valid res=, petitio), 1294/1294.
Enum/table/static-assert/silva.h all tail-appended (ABI append-only
law). Corpus contact: the one real intentio stays ACCIPE clean —
the codices watched it and had nothing to say.

## 2026-07-22 (early) — Chunk D: the board learns the identity layer

Cross-project chunk (silva instrument + gesta resident):
- --renovare now emits build/citationes.tsv (res-praefixum, via,
  linea — one row per res= citation in annotation trees).
- Tabularium: ancora genus "nid" resolved against build/
  identitates.tsv via a lazy index (exact twin of the nexus.tsv
  loader); resolution renders "via:linea (genus unitatis)";
  absence = a CAUTIO that names its meaning (annotation deleted
  or index stale) and its cure (-renovare). Ambiguous prefix =
  its own CAUTIO.
- Reverse links: res output gains "citata ex codice: via:linea"
  for every code citation whose praefixum prefixes the res_id.
- Census: citation health — "citationes ex codice N (inresolutae
  M)" with unresolved ones NAMED (the code→board rot signal).
- Loader design note: cache-on-SUCCESS only — an index file
  appearing after resident boot becomes visible at next use
  (also what made the probatio's runtime-written citationes
  fixture possible; static fixtures can't know minted ULIDs).
- gesta launcher fontes list regenerated (moneta edge — the
  scrinium delegation reached the resident's link line).

Gesta suite 220/220. Live demo staged: the park 01KY3D7EJP now
carries {"genus":"nid","scopus":"01KY3TRZ..."} — the park that
conceived the layer, anchored to the layer's first identity,
which itself cites the park back (citationes row). The loop
closes on its own history; old resident renders the anchor
inert, /mcp reconnect completes the demo.

## 2026-07-22 — E1: the include-splice swallow + stale-object trap

Chunk E1 (EXSPECTA pin migration) flushed a real collector gap:
an annotation comment directly above a `#include`/`#define` was
SILENTLY dropped. Mechanism: the comment's trivia rides the
directive-line tokens, and directive lines are consumed by the
preprocessor — they never enter `parsura->lexemata`, so the
trivia walk never saw them. This placement (file-head annotation
above the include block) is the most natural spot for file-level
annotations, so the gap would have bitten adoption hard. Fix:
`silva_annotationes_colligere` now walks `parsura->directivae`
(Xar de Xar de SilvaToken* — retained for byte-exact source
reconstruction) between the lexeme stream and the EOF tail walk;
dedup by (fons, byte_offset) makes overlap harmless. Order note:
directive-riding annotations land after stream ones in the
result — order is documented as approximate, consumers key on
positions. Resurrection case = probatio section X (#define form,
hermetic — same class as #include without needing resolvable
headers).

Debugging wrinkle worth remembering: the fix appeared to fail on
first test because of the STALE-OBJECT TRAP in identitates.sh —
editing a fons .c rebuilt the .o but the binary's relink
condition only checked identitates.c and headers, so the OLD
binary kept running. The launcher now also relinks when any
object outruns the binary. (Bisection that cracked it: the probe
worked only after an unrelated edit to identitates.c — the edit
was forcing the relink, not fixing anything.)

Also structural, found by the bisection gate (67/67 byte-parity
required): one absolute pin lived INSIDE the fixture's prose
header comment (ordo_pravus_vendicati). The awk was line-based
and read it; the grammar anchors per-comment, so a pin embedded
in prose is invisible BY DESIGN. Cure: the pin became its own
annotation comment. This is the grammar teaching correct habits
— annotations are comments, not lines.
