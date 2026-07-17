# legatus.worklog.md

## 2026-07-13 — birth (chunks B–D, one day)

Design notes that will matter to future maintenance:

- **Hover is USE-SITES ONLY.** nexus symbolorum records symbols at
  resolution sites (M4b sessio find) — hover on a DECLARED name
  returns null by construction. Path back: silva-side declarator-name
  binding export. Do not "fix" this in legatus by string matching.
- **C11 ordering is load-bearing** (_didsave_tractare): document
  trees point into the capita piscina (praebere'd header text), so
  the teardown order is trees → praeparatio+capita → re-analyse.
  Reordering = use-after-free on the next hover. The capita piscina
  is deliberately owned per-Praeparatio (rebuilds stay bounded);
  exclusiones stay in perennis (loaded once).
- **Excluded documents are never stored** — didOpen publishes empty
  and returns before _documentum_parare. The rebuild loop therefore
  never needs exclusion checks.
- **Fixtures must be plain C89 or carry includes** (chunk B find):
  latina-dialect text without #include is judged INFRA
  ("typus nominatus ignotus" clemency) → publishes NOTHING. The
  engine is right; the differential is `./silva/examen.sh -machina`.
- **utf-16 remap** is per-line and two-directional; positions in
  utf-8 mode are byte-exact (silva columns ARE bytes). Corpus is
  BMP-only but the remap handles surrogates anyway.
- Named v0 limits: documentSymbol lacks struct/enum tags (symbol
  table has no tag rows) and does not dedupe decl+def pairs;
  cross-file diagnostics dropped with stderr count (path back:
  group-by-via multi-publish); didClose of .h does not rebuild
  (didSave only).

## 2026-07-13 — first live reaches (shakedown, resumed session)

First contact through the agent's native LSP tool: documentSymbol
(lib/piscina.c 23 rows; include/piscina.h 17 rows — the .h lane
pays off immediately, headers were never judged per-edit before),
hover use-sites (`piscina : structura Piscina*`, `mensura :
insignatus longus`). Rough edges AT THE REACH:

- **ASPERITAS #1 — hover on a function gives the bare name**
  (`_allocare_interna`, no signature). "What are the parameters"
  is THE agent hover question. TypusC89.datum.functio carries
  reditus + parametra + numerus — renderable. Options: extend
  silva_c89_typum_scribere (function arm), or compose in legatus
  from parts. TOP desideratum, demand = day one.
- **FINDING — the client IGNORES capability negotiation for
  tool-invoked requests**: goToDefinition was sent although we
  never advertised definitionProvider. R7's polite −32601 is
  LOAD-BEARING, not theoretical — and it surfaces verbatim in the
  tool output ("methodus ignota", Latin error through the whole
  chain). Also: this is a live DEMAND SIGNAL for definition
  (v0.1 pull counter: 1).
- Hover on declared names (null, use-sites park) will bite most
  when reading HEADERS — decl-heavy files. Same park, sharper
  pull.
- Shakedown watch: double-push (hook + legatus) on first real .c
  edit — expected differential noise, retire hook at bar.
- **SHAKEDOWN FINDING — binary upgrade path**: killing the running
  server externally (SIGTERM) to force a rebuilt binary left the
  client WEDGED: "Cannot send request … server is running" with NO
  process alive (restartOnCrash evidently arms only on failures the
  client itself observes, or is slow). Recovery = /reload-plugins
  (user-side) or session restart. LESSON: upgrade a live legatus
  via /reload-plugins, never kill-and-pray. Prebuild first
  regardless (startupTimeout). CONFIRMED: /reload-plugins cleared
  the wedge and respawned the new binary cleanly.
- **ASPERITAS #1 RESOLVED same session**: function-signature hover
  live — `_allocare_interna : vacuum*(structura Piscina*,
  insignatus longus, insignatus longus, integer)`. Renderer
  extended SILVA-SIDE (TYPUS_C89_FUNCTIO arm in
  _typum_scribere_intus: K&R = empty parens, prototyped-zero =
  "(vacuum)", variadic = ", ..."; pointer-to-function composes via
  the monstrator arm for free); amalgam VERIFICATUM; suites 12/12 +
  30/30 (sessio goldens unaffected — function-decl echo was never
  pinned); probatio pins `probatio_functio : integer(integer)`.
  First full dogfood loop closed: reach → asperitas-in-worklog →
  silva fix → gates → live, within the hour.

## 2026-07-13 — goToDefinition (declaration tier) + hover-on-declared

- **The declarator-export park DISSOLVED by inspection**:
  `_symbolum_registrare(..., dv->datum.nodus, tok)` — symbols have
  carried their declaring NAME TOKEN (`lexema`) since M0. No silva
  change needed; `_symbolum_declaratum_ad_byte` scans the symbol
  table (O(symbols), fine). Fixes hover-on-declared-names AND
  serves definition-on-declared-names in one helper.
- Definition = use-site symbol (shared `_invenire_ad_byte` descent,
  refactored out of hover) OR declared-name scan → `lexema` radix →
  Location. **Cross-file jump into praebere'd HEADERS works**: fons
  ≠ princeps → basename via silva_fons_via → absolute path via the
  NEW `praeparatio.viae_capitum` map (praeparator records
  basename→path during the capita sweep; lives/dies with
  piscina_capitum). systema fontes → NIHIL (no jump into synthetic
  text; correct).
- Header-target ranges use raw byte columns (no line table for
  unopened files) — exact under utf-8; utf-16 approximation on
  non-ASCII header lines (quasi-ASCII corpus; acceptable, noted).
- **Definition body-tier stays v0.1**: the jump lands on the LATEST
  registered declaration preceding the use (static defined above
  its uses = the definition itself; header-declared = the header
  decl). True defining-.c jump = the fifth exploration's index.
- LIVE MOMENT during this work: legatus flagged my own incomplete
  refactor (stale identifiers in the render block) via the
  diagnostics push before any compile ran — the dogfood channel
  catching bugs in its own source.
- Bar: 121/121 (declared hover, definition {0,25}/{0,4} exact,
  header jump uri-suffix + line>0); suite 12/12; teeth 3/verified.

## 2026-07-13 — v0.1b live probes (post-reload)

- incomingCalls(nexus_ordines_fundere) → sweep caller only:
  CORRECT-per-index — legatus.c's own call site postdates the last
  -renovare. This is the staleness/overlay dynamic live: judged
  files overlay fresh rows; closed files trust the sweep. Habit:
  run `./silva/nexus.sh -renovare` after committing large additions
  (2.5 min) so the cold baseline follows the code.
- Concurrent LSP tool calls raced the server restart ("server is
  starting") — second request failed transiently, clean retry
  succeeded. Client-side behavior; nothing to fix server-side.
- references(similitudo_optima) = 9 across 5 files (def + header +
  probatio ×4 + nexus ×2 + emitte fortasse) — complete and correct.
- workspaceSymbol("tabellarius_epistulam") → both framing fns at
  .c body lines. Fuzzy ranking = nexus -similis voice.

## 2026-07-13 — shakedown find #1: hover signature declines on FILE*

- hover(legatus_currere at its own definition) → bare name, no
  signature. hover(_methodus_est) same file → full signature. The
  difference is the parameter list: legatus_currere takes FILE*
  (systema type). silva_c89_typum_scribere evidently returns
  irreddibilis (0) for the FILE typedef chain, and legatus falls
  back to name-only — graceful, but every function touching stdio
  (a large club: currere entry points, ansae plumbing) hovers
  thin. Root cause to confirm silva-side: is FILE materialized as
  an unresolvable/incomplete typedef in the systema layer, or is
  one renderer arm missing? Candidate fix lives in
  silva_c89_semantica.c (_typum_scribere_intus), not legatus.
- Found during post-compact bench warming — first organic
  shakedown find of the agitatio period.

## 2026-07-13 — shakedown find #1 RESOLVED (silva-side, ~1 hr find-to-fix)

Root cause was NOT FILE* (it renders via its `_systema_FILE` tag) —
it was the TAGLESS `LegatusConfiguratio` typedef. Fix landed in
silva_c89_semantica.c: typedef baptism of anonymous tags + ACIES
renderer arm + `<anonyma>` placeholder (details in
silva_c89_semantica.worklog.md). No legatus-side change needed —
the graceful name-only fallback stays as the last-resort tier.
Expected hover after reload:
`legatus_currere : integer(structura _systema_FILE*, structura
_systema_FILE*, constans structura LegatusConfiguratio*)`.
Also fixed in passing: silva/compile_probationes.sh choked on
amalgam-world nexus_ordines.c (broken since chunk C, unnoticed —
officina runner was the only one exercised). Second organic
shakedown find, same afternoon.

## 2026-07-13 — instrument debrief, find-#1 session (desiderata ledger)

Reaches that fell back to grep though the bench could in theory serve:
1. MACRO LOOKUPS ×2 (do XII/CXXVIII exist in latina.h? which
   CREDO_AEQUALIS_* variants exist?) — the named macro-index park
   firing in real work. No #define rows in nexus.tsv = no
   workspaceSymbol/hover for macros. Demand +2 in one session.
2. FIELD REFERENCES (who reads tag.titulus? — THE safety audit for
   baptism) — members are not ordinary symbols, no index rows, so
   references/nexus can't answer; grep with false-positive risk was
   the only path. Semantica already types member accesses, so the
   knowledge exists. NEW desideratum, biggest of the day.
3. TYPE DECLARATIONS AS TEXT (how is LegatusConfiguratio/FILE
   declared?) — workspaceSymbol+definition exist but return
   locations; I wanted source text + context, so grep+Read won on
   round trips. emitte speaks that voice for functions only.
   Desideratum: emitte (and definition-with-body) for typedefs/
   structs/enums.
Roughness (not a fallback): hover's name-only degraded tier is
SILENT — indistinguishable from a complete answer unless you already
know a signature should be there. An explicit marker (e.g.
"<signatura irreddibilis>") would have started the diagnosis one
probe closer. Moot for the render-failure class after the baptism
fix, but the tier still exists (poison types).
Fructus worth naming: nexus zero-USUS on typedef_registrare =
authoritative negative evidence (grep can't prove absence) → the
two-registration-paths discovery → fix landed on both. emitte ×2
byte-exact function reads. nexus -fortasse caught a typo'd symbol.
Where grep was RIGHT (no gap): comment text, shell scripts,
within-file locals (est_typedef), header context sweeps.

## 2026-07-13 — v0.2 chunk 2: macro resolution (sero)

Design find of the chunk: HOVER ORDERING. Hover on a macro use
descends the node tree into the EXPANSION — `GRADUS` lands on the
literal `7` (typus tier renders "integer"), `imprimere` would land
on printf. The invocation the writer typed must beat the type of
its invisible expansion, so the origo tier runs BEFORE the
symbol/typus render; the raw-word tier stays last (directive lines
only — never hijacks real symbols because those resolve earlier).
_macro_ad_byte(cum_verbo) gates the two tiers.
Definition/references need no early tier: expansion nodes carry no
symbolum, so they fall through to the macro tier naturally.

Same-file #defines are the hard fixture: body tokens ARE
fons_princeps, so expression extents can span def-site AND use-site
bytes. The origo tier's radix-extent match is immune (exact
invocation-root interval). Genus guards added on every symbol-path
row walk (definition body-tier, references, incomingCalls) — macro
and symbol rows share titulus chains; without guards a macro named
like a function would inject phantom rows.

Push-channel tally this chunk: 3× arg-count mismatches during the
_symbolum_petitionis signature change (each fixed before compile),
1× "multiset primitivorum invalidum" — my scenario insertion SPLIT
principale's two-line signature, leaving an orphaned `integer`
above the new block. All caught at edit time by legatus's own
diagnostics. Bar: 165/165 (29 macro asserts: hover-beats-expansion,
directive raw-word, definition→line 0, references sedes+usus,
workspaceSymbol kind 14, documentSymbol functio+2 macra), 12/12
suites, clang PURUS throughout.

## 2026-07-13 — v0.2 polish: multi-line hover via vista extents (+ ABI find)

Fran's instinct confirmed: no scanning needed. SilvaMacroDef.corpus
holds raw source tokens with exact byte offsets, and SilvaRamusVista
already had corpus_initium/corpus_finis for the same reason —
SilvaMacroVista just never got them. Added (fontes + amalgam mirror
+ expansio unit tests w/ hand-computed offsets across '\'
continuations). Legatus: _corpus_finis_macronis matches the index
sedes against the requesting doc's expansio (titulus + linea +
_viae_congruunt suffix-symmetric: praebere basename vs absolute vs
repo-relative), then the line reader copies [line-start ..
corpus_finis] VERBATIM — backslashes and newlines included, honest
source. Single-line + " ..." stays as fallback for macros not
visible in the requesting TU; truncation to buffer also marks
" ...". hover CREDO_VERUM now shows the whole body.

THE BIG FIND (all 7 macro asserts failed on first run): STALE-OBJECT
ABI CORRUPTION. compile_probationes.sh's per-unit blocks checked
only their own .c/.h mtimes — NOT silva.h. The amalgam object
rebuilt fresh (6-field SilvaMacroVista), nexus_ordines.o stayed
stale (4-field stack struct) → silva_macro_vista overflowed the
caller's frame → emission loop locals smashed → ZERO macro rows →
every downstream feature dead. newest_header() existed and scanned
silva/amalgama — only the lib loop used it. Fixed: all four unit
blocks now call it; legatus.sh got an explicit SILVA_H -nt condition
(its build dir had the SAME stale objects — the production reload
would have shipped the corruption). Debugging-lessons instance:
stale-object trap, amalgam-ABI flavor — struct extended in a shared
header = every consumer object must rebuild; the failure reads as
"feature returns nothing", not a crash.

Bars: expansio unit tests (UNUS/MULTI/VACUUS extents exact),
probatio_macra + SUMMA multi-line hover verbatim (167/167), officina
12/12, silva 30/30 FULL, hospes 31/31.

## 2026-07-13 (nocte) — VIGILIA SUI (excubitor chunk 3)

Legatus now watches ITSELF for staleness — the resident-binary
class no script sees (a script checks at spawn; the process then
outlives its own sources). Mechanism: cfg.binarium_via (argv[0]
from principale; NIHIL in probationes = feature inert), fabrica.tsv
"binarium" rows suffix-matched against own path give the unit .c
roots, BFS over build/inclusiones.tsv gives the closure (~all own
sources + headers), praeparator_tempus_plagulae (new; sys/stat.h
joins dirent in the ONE posix-seam file) sweeps mtimes at
initialize. Per-publish: doc in own closure + disk mtime > captured
binary mtime → se_stalus. Once stale, EVERY publish carries one
Warning diagnostic ("LEGATUS IPSE STALUS: <via> recentior binario
residente - /reload-plugins renovat") — any answer is now suspect,
so the nag is global by design.

CAPTURE-AT-INIT SEMANTICS (accidental and correct): binarium_tempus
is stat'd once at initialize. If legatus.sh later rebuilds the
binary on disk, the RESIDENT keeps comparing against its own
vintage and keeps nagging — right, because the resident IS still
old. /reload-plugins is the only cure, which is exactly what the
message says.

Test: probatio_vigilia — fabrica ficta (binarium fictum ←
officina/instrumenta/legatus.c), fake binary mtime forced ancient
via utime() → publish carries exactly the STALUS diagnostic;
recreate binary fresh → publish clean. Real closure through the
real graph (inclusiones.tsv joins nexus.tsv as a test dependency —
precedent already set by v0.1b).

LEXICON GAP NOTED: the diagnostics push flagged `structura utimbuf`
as incomplete in the probatio — <utime.h> is outside silva's
modeled system lexicon (systema_c89/posix don't carry it). clang
compiles it clean; same class as the M4b contextus-lexicon finding.
If posix-header coverage grows, utime.h is a candidate.

## 2026-07-13 (nocte, addendum) — vigilia LIVE-VERIFIED + spawn subtlety

The resident pushed "⚠ LEGATUS IPSE STALUS ... /reload-plugins
renovat" attached to the very edit that aged it. But the FIRST live
probe was silent, CORRECTLY: the client spawns the server LAZILY
via legatus.sh, which rebuilds — my edit and the spawn-rebuild
landed in the same second, so the "resident" answering was younger
than (well, coeval with — strict > says fresh) the edit. Operational
truth: the vigilia covers the long-resident case (server up, then
sources move); the lazy spawn-rebuild covers edit-before-spawn for
free. Between them the stale-answers window is closed from both
ends. Same-second equality counts as fresh by design (strict >),
matching excubitor.sh.

## 2026-07-13 (nocte) — asperitates from the excubitor-arc debrief

① NO DIAGNOSTICS PULL: when I needed "what diagnostics does legatus
hold for this file RIGHT NOW" (vigilia verification), no channel
existed — the client is push-only and pushes surface only on edits.
Had to hand-roll a stdio probe script (scratchpad; drove
initialize+didOpen by hand). The probe doubled as the honest
end-to-end test, but the gap is real. Natural homes: a one-shot
sonda driver script, or LEGATUSD answering queries. ② HOVER EMPTY
MISS: hover at a wrong position returns bare "no hover information"
— no nearest-symbol hint; cost a round-trip + grep. similitudo
exists; a fuzzy "did you mean the symbol at line N" would convert
misses into hits. ③ VIGILIA SOURCE FIELD: the self-staleness
diagnostic reports source "silva" (_diagnosticum_json default) —
should say "excubitor". Cosmetic, park. ④ USAGE NOTE (not a tool
gap): for mapping a big file's functions I still reach for grep -n
out of habit; documentSymbol IS the bench answer now — symptom
table addition.

## 2026-07-14 — outgoingCalls (nona operationis octava impleta)

callHierarchy/outgoingCalls shipped as the exact inverse of
incomingCalls, reusing every mechanism that arc built: the
extenta_functionum cache + _functio_continens give the caller's
body extent; the omnes_ordines walk (vicinitas idiom) finds usus
rows of genus "functio" inside it; _sedes_ex_ordine + the v0.1b
body-def gradus policy (new helper _ordo_corporis_tituli: .c
primary -> .c secondary -> any sedes) resolve each callee's "to"
item. Unlike our per-use incomingCalls entries, outgoing GROUPS by
callee — one CallHierarchyOutgoingCall per distinct callee with
plural fromRanges (the LSP-spec shape; live probe: _lex_currens
13 call sites in one entry, including a two-calls-one-line pair).
libc callees resolve to silva/fontes/systema_c89.h — the modeled
lexicon is a real, jumpable sedes; a pleasant surprise, no
special-casing needed. Known approximation: a bare function-name
use (function pointer taken, no call) counts as an outgoing call —
index rows can't distinguish; acceptable for an agent client.

FIND (probe time): on a file with UNCOMMITTED line-shifting edits,
outgoingCalls via the closed-file path returns empty — the extent
comes from a fresh on-demand parse (current lines) while tsv usus
rows carry committed-time lines; the line-range intersection
misses. Same staleness class as references/definition, but the
INTERSECTION makes it fail closed (empty) rather than approximate.
In real sessions the client didOpens the file first -> overlay
re-judges with current lines -> extent and rows agree. The
degenerate case is stdio probes without didOpen on a dirty file.
Remedy if it ever bites: didOpen first (or commit); post-commit
hook re-pins the tsv.

Item resolution detail: prefer item.range.start (via
_functio_continens) but VERIFY the extent's titulus matches
item.name — a junk range must not silently pick a different
function; on mismatch fall back to the titulus scan of the file's
extents. Item without range (our goldens, hand probes) goes
straight to the titulus scan.

## 2026-07-14 — outgoingCalls LIVE in-session + client didOpen scoping

Post-reload verification: outgoingCalls on _outgoingcalls_tractare
ITSELF (uncommitted legatus.c) returned its complete call graph —
21 callees grouped by file, every hour-old helper (_chordae_pares,
_ordo_functio_est, _ordo_corporis_tituli) at CURRENT dirty-file
lines. The overlay-coherence prediction from the ship-time FIND
held exactly: client didOpen -> re-judge -> extent and rows agree.
Client renders grouped fromRanges as "called from: a, b, c" — the
per-callee grouping paid off in the reading surface.

ASPERITAS (client-side, new): the Claude Code LSP client only
didOpens files that were EDITED in the session (a partial Read did
not open one; hover/prepare on a merely-Read file return empty —
"No call hierarchy item found"). Since prepareCallHierarchy needs
an open document, call-hierarchy THROUGH THE CLIENT only works on
the session's working set. The SERVER answers closed-file
outgoingCalls fine (stdio probe, no didOpen: on-demand parse).
Same family as the no-diagnostics-pull quirk: the client is the
constraint, not the server. This is the sharpest demand signal yet
for MCP LEGATI — a name-based "quis vocatur ab X" needs no
document, no position, no prepare handshake.

## 2026-07-14 — chunk 2 (recensere + epoch lattice): the prototype-shadow find

FIND (golden-caught, PRE-EXISTING since v0.1b): a forward prototype
in the same file shadows the definition extent. SemanticaSymbolum
has only `declarans` (silva.h:1210) — the FIRST declaration — so a
file with `int f(int);` before `int f(int){...}` yields a one-line
extent and every call inside the body falls outside it: outgoing
calls silently empty, incoming attribution misses that caller. My
chunk-1 outgoingCalls inherited this invisibly; the chunk-2 golden's
fixture (written prototype-first out of clang habit) exposed it.
NAMED SILVA PULL: definition-wins declarans (semantica updates the
node when the definition arrives) — improves hover-on-declared and
extents everywhere; needs silva-side ceremony (differential bar),
not a legatus rider. Until then: house style rarely
forward-declares in .c (interior functions defined before use), so
exposure is limited (json.c's _scribere_valor is a known case).

Design notes that shipped:
- _via_recens compares recorded-vs-current mtime by IDENTITY (!=),
  not threshold — no clock semantics at all; the opposite polarity
  from the vigilia's strict > is commented at both sites (C8).
  Residual: same-second same-size rewrite invisible (nanosecond
  park).
- _recensere passes "./via" as the analysis label so overlay rows
  land in index form — the absolute-path label would have emitted
  rows no lookup could match (caught at design time by tracing the
  didOpen precedent).
- _extenta_ex_semantica now allocates into a caller piscina: the
  open-doc path allocated its per-request extents into
  piscina_indicis FOREVER (pre-existing slow leak, one Xar per
  call-hierarchy request on an open doc); now they go to the
  message piscina.
- _indicem_renovare re-emits open documents' overlays from their
  LIVE trees after the wholesale piscina swap (no re-parse) — the
  uncommitted-open-edits hole the sim predicted (C7) is closed.
- Live-verified via fifo (prescripted tmpfile streams cannot mutate
  disk mid-session — the tsv-swap golden from the sim was
  UNGOLDENABLE in-harness; fifo probes are the honest substitute
  and the sonda-una-ictus park grows a third seed): tier-2 touch →
  "index renovatur (tsv mutatus)" + correct answers; overlay
  survival (phantasma symbol found post-reload); tier-1 MCP cascade
  (touched fixture header → "praeparatio stala" → "tsv coactus" →
  requests answer).

## 2026-07-14 — LEGATI shakedown: the debounce-poisoning find

First session with legati registered. Instructions field CONFIRMED
landing in agent context verbatim (C12 unknown closed). All four
tools exercised through the real client.

THE FIND: symbolum '_recensere' answered "titulus ignotus" while
vocata resolved chunk-3 functions at current lines. Root cause was
an INTERFERENCE BUG between two of our own staleness mechanisms:
the post-commit hook debounced on the tsv's MTIME (find -mmin -10),
and the chunk-2 live tier-2 probes TOUCHED build/nexus.tsv to
trigger resident reloads without content change — so the chunk-2
commit's hook saw a fresh mtime over stale content and SKIPPED
regeneration. The tsv silently stayed at chunk-1 state across two
commits. vocata still answered correctly because its walk runs
through _extenta_viae -> _recensere overlay (the self-heal masks
index gaps for VIA-scoped questions); symbolum's liveness check is
NAME-scoped (no via to re-judge — the C4 chicken-and-egg boundary)
and had nothing to heal from. FIX: hook debounces on the GENERATUM
stamp INSIDE the file (only true regeneration changes it) — the
same identity-over-mtime lesson as _via_recens, now applied to the
hook. PROBE RULE going forward: live tests that want to trip the
resident's tier-2 should utime the tsv to an ANCIENT time, not
touch it — ancient trips the server's identity compare without
faking recency to anything else.

The heal was the demo: after nexus -renovare, the NEXT tools/call
hit the resident's tier-2 check, reloaded 720k rows mid-session,
and answered with the current signature — no reconnect, no
intervention. The epoch lattice fixed in production the exact
failure it was designed for, twenty minutes after shipping.

## 2026-07-14 — prototype-shadow RESOLVED legatus-side (no silva change)

Fran pulled the "silva declarans" fix while fresh. Recon REVERSED
the diagnosis: _symbolum_registrare does not merge — every
declaration allocates a FRESH SemanticaSymbolum and sem->symbola
keeps ALL of them. The definition's own registration already
carries the definition node as declarans; extents held BOTH entries
(prototype [n,n] AND definition [body]) all along. That's why
incomingCalls/containment always worked (containment self-selects
the body-spanning entry) and only NAME-SCANS broke (first-match
took the prototype). The named silva pull
(definition-wins-declarans) is CLOSED UNNEEDED — mutating declarans
would have destroyed information (first-declaration site) and
risked hover-on-declared regressions; the code-is-a-database pin
wins again: both facts were already in the database, the consumer
was selecting wrong.

Fix: card field est_definitio (honest test:
silva_c89_definitio_functionis_corpus(declarans).genus !=
SILVA_VALOR_NIHIL — the silva.h accessor contract "generis alieni
-> SILVA_VALOR_NIHIL" makes this a clean public-API probe) +
_extentum_tituli (definition-preferred titulus scan) replacing
first-match at three sites (outgoingCalls fallback + range-upgrade,
legati vocata, legati symbolum signature). Regression golden:
_stala_scribere flipped BACK to prototype-first — this morning's
failing case is now the proof. Live: vocata _scribere_valor
(json.c's real forward-prototyped case) 0 -> 6 callees.

SHAKEDOWN FIND #2 (same batch): MCP CAUTIO never fired — se_stalus
was computed ONLY at initialize; post-spawn edits invisible (LSP
re-checks per-publish, MCP had no per-request analog). Fix:
clausura_viae retained (perennis Xar of the same durable keys) +
_vigiliam_percurrere extracted from the init sweep, called at MCP
request entry via _aetates_custodire. Live via fifo: touch
legatus.c mid-session -> next answer carries the CAUTIO line under
a complete answer (Q9 semantics: answer + disclose, never refuse).

## 2026-07-14 — quintum instrumentum MCP: inclusiones

Demand signal fired the same day it was recorded (rung 5 debrief:
"who includes X" went to awk three times; excubitor-custos already
answers the closure variant on every header edit). Design choice:
per-request disk read of build/inclusiones.tsv (the diagnostica
freshness law — no cache, no epoch-lattice integration needed; the
graph is ~500KB, parse is ~ms). The vigilia loader's line-walk was
reused nearly verbatim; the reverse-closure BFS is the same
computation excubitor does in awk — now answerable warm, by name.
Output: includit / includitur ab / clausura reversa with exact
counts, praetermissa annotations, XL-capped closure listing.
Unknown via gets an "in disco est - tabula vetustior?" hint when
the file exists but the graph predates it. If the tool ever gets
hot, promote to retained adjacency + tier-2 swap — interface
unchanged (Eskil).

## 2026-07-14 — commentarium on cards (+ the declarans shape lesson)

symbolum cards now carry the leading comment block (tight-above
rule, silva_commentarium_ducens) under each sedes — corpus always,
first header sedes when it differs. Extents captured at judge time
(macro-vista pattern), bytes read verbatim at render (open-doc
first, disk fallback), 24-line cap.

THE LESSON: SemanticaSymbolum->declarans is NOT one shape.
Definition registrations carry the whole definitio-functionis node
(specifiers included); prototype registrations carry only the
DECLARATOR — which starts at the name token, so the leading
comment's trivia (riding on the first specifier) are OUTSIDE the
node. Any consumer walking declarans for position/trivia purposes
must decide which enclosure it means. Cards are depth-0 only, so
pater-climb to the root node = the containing declaration, always.
Debug detour recorded: the bisection probe (standalone amalgam
parse) proved silva correct in one shot and pointed the blame at
the node handed over, not the accessor.

Also: a stale silva.h in the session scratchpad shadowed the quoted
include of the probe (quoted includes search the includer's dir
first) — phantom "missing declaration" errors against a file that
was fine. Angle-include or clean scratchpad. Same family as the
stale-object ABI trap.

## 2026-07-14 — corpus {titulus} + vigilia once-per-epoch

corpus = the "show me the code" tool: card gained corpus byte
extents (whole declaration via the pater-climbed root — same node
the comment anchors to), rendered verbatim through the same
open-doc-first byte capture as comments, 200-line cap. symbolum's
unknown/macro arms extracted into shared helpers
(_titulus_vivus/_ignotum_respondere/_macro_respondere) — corpus
and symbolum stay in lockstep by construction. Named park: full
macro bodies in MCP mode need the parsura out of _recensere
(document-free path has no LegatusDocumentum for
_corpus_finis_macronis); first-line-with-ellipsis until then.

vigilia noise fix: the per-publish STALUS warning now fires always
for files in legatus's own closure, once per staleness epoch
otherwise (stalus_nuntiatum, reset at both FALSUM->VERUM flip
sites). The repetition was correct disclosure that trained the
reader to skim — the failure mode disclosure exists to prevent.
MCP CAUTIO untouched (per-request, part of the answer's trust
envelope).

## 2026-07-14 — new-header discovery + via disambiguator

The judge-miss root cause was deeper than the park's name: the
tier-1 sweep compared KNOWN headers' mtimes, so a newly created
header was invisible in BOTH modes — MCP never actually healed
this either. Fix: directories ride in tempora_capitum alongside
headers (a new/deleted/renamed file bumps the parent dir's mtime,
POSIX-guaranteed; stat works on dirs; the sweep loop is unchanged).
Recorded BEFORE each dir is scanned — creation during the walk is
caught next check. LSP didOpen now runs the sweep and rebuilds via
_contextum_reaedificare (extracted from _didsave — the rigid C11
①②③ order shared); MCP inherits through _aetates_custodire.
Same-second window remains (mtime seconds granularity — the old
nanosecond-mtimes park covers it; tests force the difference with
utime).

via disambiguator: symbolum/corpus take optional 'via' (suffix
match with '/' boundary). Golden lesson: principale is NOT a
usable specimen — it's a latina macro; the function rows are
titled 'main'. _ch (seventeen definitions across probatio files)
is the canonical ambiguity specimen now. Misses list known sedes
(capped 8) — the tool teaches its own arguments.

## 2026-07-14 (macro-domesticum session debrief) — two observations

PHANTOM DIAGNOSTIC (one occurrence, unexplained): the diagnostics
push once named "specimen_usus.c" 15:15/15:21 (mensura aciei +
identificator ignotus) — a file that exists NOWHERE (repo searched,
scratchpad searched, not created this session). Transient, never
recurred. Class concern: diagnostics for nonexistent files erode
channel trust. Suspects: hook argument parsing on a transient path,
or stale open-doc state surviving somewhere. Watch for recurrence
before investing.

HABIT GAP, NOT TOOL GAP: the session's dominant discovery reach was
struct/enum/typedef bodies (~15 grep+sed+Read cycles: SilvaParsura,
SilvaOrigo, SilvaFons, SilvaInclusio, ExamenCodex, chorda,
xar_creare...). corpus serves ALL of these — verified post-session
(SilvaParsura returns the full struct WITH field comments; the tool
description even says "functio/variabile/typus/macro"). The reach
never happened out of grep muscle-memory + untested assumption that
type rows might not resolve. Fix applied: bench-table memory line.
Possible tool-side lever: doctrine/instructions could SHOW a type
example (one line: "corpus SilvaParsura -> struct body w/ field
comments") — examples teach reaches better than type lists.

FIELD REFERENCES demand +2 data points this session ("where does
est_syntheticus get initialized", "where are ctx->lexica consumed")
— both greps, both below symbol granularity. Pull board #1
reconfirmed third session running.

## 2026-07-16 — serverInfo.version = binary sigillum (K2.1 port)

Ported the identity half of tabularium's vigilia: legatus.sh
computes shasum of the freshly linked binary, passes -signum;
both serverInfo sites (LSP "legatus" + MCP "legati") report the
16-hex prefix instead of hardcoded "0". Find while porting:
legatus already HAD the warning half (se_stalus watches its
source closure vs startup binary mtime — fires BEFORE rebuild,
persists after since the mtime is cached at spawn; complementary
to tabularium's disk-hash watch which fires AFTER rebuild). The
two servers now cover the binary→process staleness rung from
both directions. Config: LegatusConfiguratio.signum (NIHIL="0",
memset-zero tests unaffected — pinned transcripts keep "0").
Poetic footnote: the running legati resident announced its own
staleness via diagnostics push while we were adding the identity
stamp to its successor.

## 2026-07-16 (later) — vigilia migrated to lib/vigilia

The fabrica-BFS closure (~200 lines: _vigilia_res_congruit,
_vigiliam_construere, _vigiliam_percurrere) replaced by lib/vigilia
with a launcher-written manifest (legatus.sh now emits
build/legatus/legatus.vigilia listing exactly what it compiles).
The MANIFEST INVARIANT is the design win: the watcher can never
disagree with the builder about what triggers a rebuild — the old
BFS was ABI-precise but could diverge from legatus.sh's actual
rebuild conditions. Kept: LSP epoch semantics (stalus_nuntiatum),
pinned "LEGATUS IPSE STALUS" texts (server voice, library
mechanism), in-closure always-warn via vigilia_continet. Gained:
disk-hash watch (post-rebuild detection) legatus never had, and
ns-precision mtimes. cfg.fabrica_via → cfg.via_manifesti;
transcripts stayed green without re-blessing. Note:
probatio_officina_legatus is REICE under examen (utime fixtures)
— PRE-EXISTING, count went 7→6 with this change.

## 2026-07-17 — didChange capita-staleness guard (the K4 false-positive burst)

FIELD SYMPTOM (K4 chunk A, gesta session): diagnostics push flagged
five gesta_agere calls in probatio_gesta.c with "numerus argumentorum
discrepat" + "monstratores incompatibiles" — calls that were CORRECT
(7 args). Same moment, MCP diagnostica said ACCIPE. Two processes,
two views: the MCP twin re-checks capita at every request entry
(_aetates_custodire); the LSP resident's didChange path judged with
NO staleness check at all. Its praeparatio still held a PRE-ACTOR
gesta.h snapshot (6-param gesta_agere) — 7-arg calls against it =
arity + shifted-position pointer errors, exactly the burst. The lie
persisted until some header event finally fired a rebuild.

THE HOLE (asymmetry audit): didOpen had the caput_stalum guard
(2026-07-14, new-header arc); didSave rebuilds for .h and no-ops for
.c; didChange — the HIGHEST-frequency judgment path — had nothing.
A header changed on disk without an LSP event (Edit tool touching a
never-opened header; same-second window) poisoned every subsequent
didChange judgment indefinitely.

REPRO: deterministic, scripted LSP session (python driver, scratch):
didOpen consumer clean → rewrite header ON DISK ONLY → didChange
consumer with corrected call → false "numerus argumentorum
discrepat" on a correct call. First run.

FIX: the didOpen guard mirrored into _didchange_tractare — placed
AFTER _documentum_textum_ponere deliberately, so the rebuild's
step-③ republication judges the NEW text (no stale-flavored
transient publish; first placement BEFORE the text update produced a
transient true-at-that-instant mismatch publish, superseded but
noisy). Verified: all publishes clean post-fix.

REPRO-SCRIPT LESSON: my first driver read the FIRST
publishDiagnostics per file and declared the fix dead — the rebuild
emits transient + final publishes; LSP semantics are last-wins.
Drain to silence before judging a publish stream.

PINNED: probatio_capita_nova gained the FILE-tier gradus (known
header, content rewritten, utime-deterministic 2000000L) — the dir
tier alone misses rewrites (fopen "wb" on existing file never bumps
parent dir mtime). 295/295; 12/12 suites.

RESIDUALS (named): (1) same-second window remains — detector
compares second-granular st_mtime by identity; nanosecond-mtimes
park unchanged (st_mtimespec on Darwin, praeparator_tempus_plagulae
+ PraeparatorCaputTempus.tempus widen together). (2) hover/
definition REQUESTS between a disk header change and the next
didChange/didOpen still answer from the stale world — request-tier
guard (MCP's _aetates_custodire pattern at LSP request entry) =
follow-up candidate, lower stakes. (3) interleaved-transcript
engine harness (batch model can't mutate disk mid-conversation) =
park; natural at LEGATUSD (socket transport).
