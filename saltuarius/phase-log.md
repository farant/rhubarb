# saltuarius phase-log

*Protocol: INTENTIO before a phase, RELATIO after. Re-read the
tail of this file at session start / post-compaction. The
implementation truth is project-specs/saltuarius-spec-v2.md.*

Phase map (spec-v2 §9): **A the walk** (columns, NO silva) →
*Silva additiones I* (in silva/) → **B the look** (source view +
coloring) → *Silva additiones II* (fons_via + strata + vistas) →
**C the magic** (layers/origo/jump — the money shot) → **D the
polish** (TOC + search + tuning). tessera_replere lands as a small
tessera 1.1 pass at Phase A close (decision §10.4).

---

## PHASE 0 — SCAFFOLD (2026-07-03)

Folder tree (fontes/, probationes/, instrumenta/principalia/),
CLAUDE.md, this log, compile_probationes.sh (tessera template +
the wider lib set + the tessera AMALGAM as an object),
saltuarius.sh (lustrum.sh model), probatio_saltuarius_salve
(dependency pledges — see below), and a walking-skeleton main
(open tessera on the real tty, draw a title, quit on q) proving
the whole toolchain end to end: app build path + tessera consumed
AS AMALGAM by a second host.

Salve pledges (the behaviors Phase A rests on): (a) tessera
amalgam through the memoria pons — draw, read cells back, golden
first-frame bytes (the FIRST-CONSUMER pledge); (b) iter_directoria
introitus classification on a real fixture tree; (c)
filum_legere_totum round through a written file; (d) via_nomen /
via_iungere shapes; (e) utf8 rune-slicing arithmetic (the
horizontal-scroll primitive).

---

## PHASE A — THE WALK: INTENTIO (2026-07-03)

**Goal**: a genuinely usable miller-column browser of the rhubarb
tree — NO silva anywhere. Navigation machinery, rendering
discipline, the golden-test harness, and the daily-use feel all
get real before the parser enters the building (mirrors tessera's
pure-core-then-pons risk ordering; interview Q19 "walk then
talk").

**Deliverables** (fontes modules per spec-v2 §3):

1. `saltuarius_res.*` — app state: modus (MODUS_COLUMNAE only for
   now, enum ready for MODUS_FONS), focus, radix path, transient
   status message + its frame countdown, quit flag.
2. `saltuarius_tabularium.*` — the directory-listing MODEL.
   Constructed FROM an injected DirectoriumIntroitus-shaped list
   (titulus/genus/mensura records) — never calls iter_directoria
   itself. Owns: sorting (directoria first, then fila,
   alphabetical within each), dotfile filtering, selection index,
   scroll offset, type-jump.
3. `saltuarius_columnae.*` — miller view-model + render: three
   panes (parens / currens / praevisus), pane widths from terminal
   dims, selection bar (INVERSUM in focused pane), directory
   contents or plain-text file head in the preview pane, empty/
   unreadable placeholders ("(vacuum)" / "(non legibile)").
4. `saltuarius_claves.*` — dispatch for Phase A keys: h/l (and
   arrows) parent/enter, j/k selection, gg/G (pending-key state),
   ctrl-d/u half-page, Enter (= l), q quit, type-jump letters,
   wheel scroll, click select+focus.
5. `saltuarius_proba.{h,c}` (probationes/) — the golden helpers
   from spec-v2 §7: proba_linea (row → chorda), proba_quaerere
   (find text in row), PROBA_CELLA (style asserts). Built HERE
   because Phase A goldens are their first users.
6. `instrumenta/principalia/saltuarius.c` — thin main grows the
   real event loop: posix pons, radix arg, disk edge (fills
   tabularium via iter_directoria + filum preview reads), resize
   (AMPLITUDO → magnitudinem_renovare + re-clamp + repaint),
   RESUMPTUM → full repaint, transient-message rendering.

**Named narrowings (decided now, in the open)**:
- Dotfiles hidden, no toggle in v0 (toggle = named deferral).
- Sort: directoria first, alphabetical (byte-order on chorda —
  locale collation is a non-goal).
- Type-jump v0 = single-letter cycle (press 'p' → next entry
  whose titulus starts with 'p', wrapping); full incremental
  prefix matching = named deferral with the ':' line.
- Preview: first 200 lines max, each line rune-sliced to pane
  width (utf8 walk; no horizontal scroll in the PREVIEW pane —
  that's the source view's job in Phase B).
- INTROITUS_ALIUS / _IGNOTUS render with a trailing '?' marker,
  non-enterable, previewless.
- Navigating ABOVE the radix is allowed (radix is a start point,
  not a jail) — the parent column simply keeps working.
- Symlinked directories are not followed by the walker in v0
  (whatever iter_directoria classifies them as, they are ALIUS to
  us — no cycle risk).
- Preview reads are UNCACHED in Phase A (filum read per selection
  move is measurably instant at rhubarb file sizes; caching is a
  Phase B concern alongside the parse cache).

**Exit criteria**:
- Fran browses rhubarb with it comfortably on a real terminal:
  columns + preview + type-jump + mouse wheel/click + resize +
  Ctrl-Z/fg all behaving (the manual bar, spectaculum-style).
- Suite green headless: cell-grid goldens through the memoria
  pons + fixture listings covering: initial render, j/k/h/l
  navigation, selection bar styling, preview content, type-jump,
  gg/G, scrolling in long listings, scripted resize, scripted
  resume, empty/unreadable placeholders.
- saltuarius.sh builds (both amalgams as objects) and runs.
- Phase A RELATIO written; tessera_replere decision executed at
  close (one small tessera 1.1 pass: add fill-rect, regenerate,
  re-gate — then columnae render adopts it).

**Chunks**: A1 models (res + tabularium + claves state — pure
logic, no tessera, plain probationes); A2 render + proba helpers
+ cell-grid goldens (columnae through memoria pons); A3 thin main
+ disk edge + real-tty manual bar + polish (mouse, resize,
resume, transient messages).

**Spec sections to re-read when starting a chunk**: spec-v2 §3
(architecture), §5 (rendering truths), §6 (input map), §7
(testing plan).

### Chunk A1 — models: DONE (2026-07-03)

fontes/: saltuarius_tabularium.{h,c} (injected-list model: dotfile
filter, dirs-first byte-order insertion sort, clamped selection,
gg/G, f<littera> jump cyclic + case-folded, volumen window math),
saltuarius_res.{h,c} (modus/radix/via_currens via the via lib —
pure string navigation, "/" fixed-point, transient nuntius
lifecycle), saltuarius_claves.{h,c} (TesseraEventum →
SaltuariusIussum dispatch; g/f pending-key state with clear-on-
other and abort-on-special semantics; ctrl-d/u; mouse wheel/click;
AMPLITUDO/RESUMPTUM deliberately not ours — principale's job).

Suite 4/4, 124 assertions, WHOLE CHUNK first-compile-first-run
green under the full flag set. App build still links (fontes
objects join saltuarius.sh).

**Narrowing amended in the open**: type-jump is `f<littera>` (vim's
find motion), NOT bare letters as the INTENTIO sketched — bare
letters are already navigation (j/k/h/l/q/G/g); a bare-letter jump
would make navigation keys ambiguous. f-prefix keeps the vim
dialect coherent and gains "f then ANY letter" (fj jumps to a
j-named file; probatio pins this). Cost: two keystrokes per jump.

Design note: tabularium stores DirectoriumIntroitus VALUES (the
iter_directoria struct) — type reuse is citizenship; the pin is
about who CALLS the filesystem, and tabularium never does (implere
takes an injected array; old arrays stay in the arena — impletion
is rare, not per-frame; acceptable arena growth noted in-code).

### Chunk A2 — render + goldens: DONE (2026-07-03)

probationes/saltuarius_proba.{h,c}: the spec-v2 §7 helper set —
proba_linea (cell row → chorda, signum bytes unpacked LSB-first,
trailing blanks trimmed), proba_quaerere (rune-wise find in a
row), PROBA_CELLA (direct style asserts). fontes/
saltuarius_columnae.{h,c}: passive scene struct (panes + preview
genus DIRECTORIUM/TEXTUS/NUNTIUS), metiri (1:3:4 layout, public
because click-routing (A3) and probationes need the math), and
pingere (whole-frame repaint: path row, box-glyph separators,
panes with dir/alius styling + suffixes, INVERSUM bar across the
focused pane, preview, status line with nuntius-or-selection left
and N/M right).

**The design keystone: _textum_ponere** — ONE drawing primitive
that walks runes (utf8), sanitizes control bytes to spaces
(unsanitized \t in a cell would break tessera's grid contract:
the terminal would interpret HT and shear the diff), clips at the
pane budget (tessera only clips at the GRID edge), and places
cells directly. No intermediate string buffers anywhere in the
renderer → zero per-frame allocation BY CONSTRUCTION, not by
discipline. CRLF handled at the preview line-splitter (trailing
\r trimmed; interior \r sanitized like any control byte).

Suite 5/5 total; columnae goldens 39/39 FIRST RUN (metra math,
both scene repaints with stale-content check, INVERSUM across
full pane width, \t/\r sanitization pinned, truncation with
separator intact, vacuum + nuntius). Two -Werror traps caught at
writing time (s32/i32 compare; -Wcast-qual on const aliasing) —
the pointer+length primitive signature exists BECAUSE of the
second one.

NEXT: Chunk A3 — thin main grows the event loop + disk edge
(iter_directoria/filum wiring, click routing via metra, resize/
resume, senescere per frame), then the manual bar on Fran's
terminal.

### Chunk A3 — disk edge + event loop: CODE COMPLETE (2026-07-03);
### MANUAL BAR PENDING (Fran runs ./saltuarius/saltuarius.sh)

fontes/saltuarius_limes.{h,c}: THE disk-edge module (spec's nexus
precursor) — dual reficere'd scratch arenas (indicis refilled on
navigation, praevisus per selection: piscina_notare/reficere keeps
sessions memory-FLAT; probatio pins usage equality across 10
refills), directory reads capped at 4096 entries (silent degrade),
preview guards (>1MB "(filum magnum)" without reading, NUL-in-512
"(filum binarium)", "(non legibile)", "(vacuum)"), parens
auto-selected by basename, selectandum for ascend. A1 API amended:
implere takes the array arena (tabularium structs live persistent,
arrays live in refill arenas).

instrumenta/principalia/saltuarius.c: the real event loop —
radix absolutized (via_absoluta), initial fill, repaint+praesentare
per iteration, AMPLITUDO/RESUMPTUM → magnitudinem_renovare,
iussum dispatch (move/page/gg/G/f-jump → praevisum refresh;
INTRARE by genus with Phase-B teaser nuntius on files; PARENS with
basename reselection; click routing via metra: currens=select,
parens=ascend, praevisus=enter), senescere per frame.

**Complexity (probatio integrationis earned its keep on day one)**:
iterator introitus are "validus usque ad proximum" — the titulus
buffer is REUSED per entry, so value-copying aliased every title
to the last one read ("planus.txt" everywhere, mangled fragments
in parens). Symptom: garbage titles + cascade into wrong-genus
preview. Fix: deep-copy each titulus (chorda_transcribere) at the
read boundary. The ONE probatio allowed to touch disk caught the
ONE lifetime bug only disk could produce.

Suite 6/6, 202 assertions (limes integration probatio: real /tmp
tree incl. chmod-000 unreadable + sparse 1MB+1 file; columnae
gained the scripted-resize golden: 40x8 relayout + content +
separator + status). App builds; headless refusal correct.

REMAINING FOR PHASE A CLOSE: (1) Fran's manual bar —
./saltuarius/saltuarius.sh, walk rhubarb: hjkl/arrows, Enter/l
into dirs, h up (selection follows), gg/G, ctrl-d/u, f<littera>
jump, wheel + click all panes, resize, Ctrl-Z + fg, q; previews
incl. binary (build/) and big files; (2) tessera_replere 1.1 pass
(decision §10.4); (3) RELATIO.

## RELATIO — PHASE A COMPLETE (2026-07-03): THE WALK

**Saltuarius browses rhubarb.** Manual bar PASSED (Fran:
"everything is working") — columns, previews, type-jump, mouse,
resize, Ctrl-Z, quit; all on the first true dual-amalgam host in
the monorepo. Suite 6/6, 202 assertions, all headless. Exit
criteria of the INTENTIO met in full.

What exists: tabularium (injected-list model), res (pure path
state), claves (event→iussum dispatch with pending-key machine),
columnae (scene + renderer on the _textum_ponere primitive),
proba (cell-grid golden helpers), limes (the disk edge with
reficere'd arenas), and a thin main with the real event loop.

**tessera_replere shipped as tessera 1.1** (decision §10.4
executed): signature confirmed by this phase's real usage
(selection bar = alt-1 fill; Phase C popup interior = the rect
case). Added to fontes + probatio (7 asserts incl. clip) +
hand-written tessera.h; amalgam regenerated, ALL GATES green
(standalone/hospes/nm); saltuarius adopted it and its suite
stayed green. The eskil loop closed: the caller designed the API,
the library gained it, both proved it — in one afternoon.

Complexities (four-part schema):
- WHAT: iterator introitus titles aliased to the last entry read.
  SYMPTOM: garbage titles, scrambled sort, wrong-genus previews,
  segfault cascade in the limes probatio. CAUSE: iter_directoria's
  introitus is "validus usque ad proximum" — the titulus buffer is
  reused per entry; value-copy kept the pointer. FIX: deep copy
  (chorda_transcribere) at the read boundary; the one probatio
  allowed to touch disk existed precisely to catch this class.
- WHAT: type-jump respecified. The INTENTIO's bare-letter jump
  collided with vim navigation keys; became f<littera> (vim's
  find motion), recorded openly at A1.
- WHAT: control bytes in cells would break tessera's grid
  contract (terminal interprets \t, shears the diff). FIX:
  sanitization lives INSIDE the one drawing primitive
  (_textum_ponere), so no call site can forget it.

Named deferrals (unchanged from INTENTIO narrowings): dotfile
toggle, incremental prefix type-jump (wants ':'), preview caching,
horizontal scroll in preview (source view's job), locale
collation, drag.

**PHASE A CLOSED. Next: Silva additiones I** (in silva/ — its own
INTENTIO: ledger #1 silva_lexare declaration + #7 helpers + #8
arena telemetry; gates: amalgam byte-discipline, suites, hospes,
nm), then Phase B — the look (source view + layer-0 lexeme
coloring + degradation badge).

## PHASE B — THE LOOK: INTENTIO (2026-07-03)

**Goal**: Enter on a source file opens a full-screen SOURCE VIEW
with layer-0 lexeme coloring — the Latin-visible screen. Unblocked
by Silva additiones I (silva_lexare_cruda public).

**The load-bearing scope decision: Phase B is LEX-ONLY.** No
SilvaContextus, no praebenda seeding, no parse, no 84-242MB arenas
— silva_lexare_cruda on raw file bytes colors everything. The
full parse pipeline (and its LRU pressure) arrives in Phase C when
origo/layers need it. Phase B libri are lex-light (~KB not MB);
the LRU cache structure ships NOW (N=8, trivial eviction) so C
swaps lex→parse inside an existing shape.

**The design keystone: CLASSIS-PER-BYTE map.** One u8 color-class
per byte of the file, filled in a single walk of the cruda stream
at load. Rendering row Y = walk that line's runes, look up
classis[byte_offset], done. Multi-line tokens (block comments!)
color their continuation lines FOR FREE — no per-line token index,
no span bookkeeping, no editor-classic continuation-state bug.
Cost: file-size bytes in the liber arena (460KB worst case —
nothing). Phase C hit-testing will use vera lexemata + byte_offset
binary search; the classis map stays the RENDER path.

Classification (all from token genus, single walk):
- C89 keywords: genus in the SILVA_LEX_AUTO..WHILE band — SILVA
  CLASSIFIES THEM (no app-side keyword set needed!)
- latina names: IDENTIFICATOR + membership in the latina set,
  built at startup by LEXING silva_latina_textus WITH SILVA ITSELF
  (the guard-probatio pattern: # define IDENTIFICATOR triples)
- commenta (both genera), string/character lits, numeri
  (INTEGER/FLOAT), directiva (CANCELLUM at initium_lineae + the
  directive word after it), OCTETUS_IGNOTUS → ignotum class
- Palette (explicit RGB, tuneable): commentum 0x6A9955, chorda
  0xCE9178, numerus 0xB5CEA8, verbum 0x569CD6, LATINA 0xC586C0,
  directiva 0xD7BA7D, ignotum 0xFF6666, planum nativus.

**Modules**:
1. `saltuarius_nexus.*` — latina set (tabula_dispersa), classis
   map builder, degradation classify (.c/.h → colorata; alia
   texta → "textus merus" plain classis), liber LRU cache (N=8).
2. `saltuarius_liber.*` — an opened file: TWO arenas (rhubarb for
   textus+lines+classis, SilvaPiscina for lexemata — the
   tokens-alias-fons contract means textus and tokens must
   co-live; both destroyed together on evict), line index
   (offset+mensura per line, one pass), cursor (linea/columna in
   RUNES), volumen (y lines, x runes), aptare math. Textus is
   INJECTED (limes/main reads disk; probationes hand bytes).
3. `saltuarius_visum.*` — render: gutter (width = digits+1),
   text pane (runes via classis lookup, horizontal rune-slicing),
   cursor via tessera_cursorem_ponere (real terminal cursor),
   status line (via | linea:columna | stratum 0 | badge | n/m).
4. limes: librum_aperire (filum read behind the magnum/binarium
   gates; refusal = nuntius, browsing continues).
5. main: modus wiring — INTRARE on FILUM opens MODUS_FONS; q in
   fons = BACK TO COLUMNS (not quit); h/l/j/k etc. are the SAME
   iussum enum interpreted per-mode by main (PARENS = cursor-left
   in fons; documented, not renamed); L and o exist as keys but
   nuntiare "Phase C venit".

**Named narrowings**: no '/' search (Phase D); no TOC sidebar
(Phase D); no layer indicator beyond the constant "stratum 0"
(Phase C); stilus per classis is fg-color only (no bg/bold mixing
in v0 palette); tab stops render as sanitized spaces (the
_textum_ponere discipline — column drift vs real tabs accepted in
Phase B, NAMED: proper tab expansion when the editor story
arrives); CRLF files render via the same trim as previews.

**Chunks**: B1 nexus + liber (pure models, no tessera; 2
probationes). B2 visum + cell goldens (colored render, spanning
comment continuation, h-scroll, gutter, badge, cursor). B3 limes
librum_aperire + main wiring + LRU + the manual bar (open
silva_parsare.c: THE LATIN-VISIBLE SCREEN — si/redde purple, C89
keywords blue, comments green).

**Exit criteria**: manual bar (colored silva_parsare.c + textus
merus badge on a .md + refusal nuntius on build/ binaries);
suites green with the new probationes; zero per-frame allocation
discipline holds (classis lookup is an array read).

### Chunk B1 — nexus + liber: DONE (2026-07-03)

THE SILVA AMALGAM ENTERS THE BUILD (both .sh loops: "tessera"
"silva") — saltuarius is now the first dual-amalgam host making
real silva calls. Suite 8/8.

fontes/saltuarius_nexus.{h,c}: latina index built by LEXING
silva_latina_textus with silva itself (keys alias the amalgam's
STATIC latina text — eternal, no copies; temp SilvaPiscina
destroyed after); est_fons_c (manual extension walk);
classificare = the classis-per-byte builder (single cruda walk;
C89 keywords FREE via the SILVA_LEX_AUTO..WHILE genus band;
directive heuristic: CANCELLUM at line start + following word,
whitespace doesn't break the pair, newline does). Probatio pins
the SPANNING COMMENT (c1 before \n, c2 after — both COMMENTUM,
zero span bookkeeping) and QQ-after-#define staying PLANUM.

fontes/saltuarius_liber.{h,c}: dual arenas per liber (rhubarb:
textus copy + line index + classis; silva: lexemata — honoring
tokens-alias-fons by co-locating lifetimes; both destroyed
together, struct stays in caller arena = accepted ~100B leak per
eviction, named); line index one-pass (CRLF trimmed, no-trailing-
newline and empty-file cases pinned); rune-unit cursor with
vim-style clamp-to-shorter-line; 2-axis volumen aptare.

Landmine caught at writing: chorda_sectio takes (s, initium,
FINIS) not (s, initium, mensura) — off-by-mensura would have
shipped garbage line slices; the probatio's exact-line asserts
existed before the fix ran.

NEXT: B2 — saltuarius_visum (gutter + classis-colored text pane +
cursor + status badge) + cell goldens incl. the spanning-comment
render and h-scroll.

### Chunk B2 — visum: DONE (2026-07-03)

fontes/saltuarius_penicillus.{h,c}: the drawing primitives
EXTRACTED from columnae (pen_textum/literis/numerum/digiti) — one
sanitizing rune-walk drawing path shared by both views; columnae
refactored to it, goldens stayed green. fontes/saltuarius_visum
.{h,c}: metiri (gutter width from line-count digits) + pingere
(gutter, per-rune classis→stilus lookup via _lineam_ponere — the
one place stilus varies per rune; REAL terminal cursor via
tessera_cursorem_ponere; status = right-side-priority composition
[L:C][stratum 0][pittacium] with via/nuntius truncated to the
remainder — first layout had the priority inverted and 5 goldens
caught it on a 40-col terminal before any eye did).

Goldens 29/29 (suite 9/9): gutter numerals, LATINA purple on si/
redde with plain x between, C89-keyword blue, number green,
SPANNING COMMENT rendered green on both sides of the line break,
real-cursor position tracking, horizontal volumen (first visible
rune + cursor after clamp), merus fallback (.md uncolored +
"textus merus" badge), nuntius override on the status line.

NEXT: B3 — limes librum_aperire (filum read behind magnum/
binarium gates) + MODUS_FONS wiring in main (Enter on FILUM
opens, q returns, per-mode iussum interpretation) + LRU cache
(N=8) + THE MANUAL BAR (silva_parsare.c in color).

### Chunk B3 — wiring: CODE COMPLETE (2026-07-03); MANUAL BAR
### PENDING (Fran: THE LATIN-VISIBLE SCREEN)

claves gained the modus param (the h/l collision forced it exactly
as anticipated: in fonte they are cursor motion SINISTRA/DEXTRA,
in columnis parent/enter; L/o = STRATUM/ORIGO in fonte only, q
stays FINIRE and PRINCIPALE interprets it as "back" in fonte).
fontes/saltuarius_bibliotheca.{h,c}: LRU N=8 (front = newest,
evict = liber_destruere; probatio pins move-to-front, eviction
order, and CURSOR POSITION SURVIVING reopen through the cache —
the free gift). limes gained librum_aperire (same gates as
preview: magnum refused unread, binarium by NUL-scan, non
legibile; causa as static literis for the nuntius). Main rewrote
into SaltuariusApp + per-mode dispatch (_iussum_columnarum /
_iussum_fontis); terminal cursor hidden in columns mode, live in
fons mode; strata/origo keys tease "Phase C venit".

Suite 10/10 (bibliotheca probatio + librum_aperire section in the
limes integration probatio). App builds; headless refusal intact.

REMAINING FOR PHASE B CLOSE: Fran's manual bar —
./saltuarius/saltuarius.sh, then: (1) navigate to silva/fontes/,
Enter on silva_parsare.c → THE LATIN-VISIBLE SCREEN (si/redde/
dum purple, C89 keywords blue, comments green incl. block
comments, directives gold, strings orange, numbers pale green);
(2) j/k/h/l + gg/G + ctrl-d/u + wheel + click to move the REAL
cursor; (3) q back to columns, reopen → cursor position remembered
(cache); (4) open a .md → uncolored + "textus merus" badge;
(5) try a build/ binary → refusal nuntius, browsing lives;
(6) L or o → "Phase C venit" tease; (7) resize + Ctrl-Z in fons
mode. Then RELATIO.

## RELATIO — PHASE B COMPLETE (2026-07-03): THE LOOK

**The Latin-visible screen is real.** Manual bar PASSED (Fran:
"working great, don't see any problems") — silva_parsare.c renders
with si/redde purple, C89 keywords blue, spanning comments green,
directives gold; cursor motion, cache-preserved reopen positions,
merus badges, binary refusals, mode transitions, resize and Ctrl-Z
all behaved. Suite 10/10 headless. Saltuarius is the first program
to consume BOTH amalgams with real calls across both boundaries.

What Phase B proved about the architecture:
- **Lex-only was the right scope**: full coloring with zero parse
  machinery — no contextus, no praebenda, no 100MB arenas. The
  heavy pipeline stays in Phase C where origo needs it.
- **The classis-per-byte map carried every render feature** it was
  designed for (spanning comments, h-scroll, per-rune styles) at
  file-size cost, and its per-byte lookup kept the render
  allocation-free by construction.
- **Silva additiones I API held**: lexare_cruda fed the classifier,
  the latina index came from silva lexing its own embedded
  latina.h, genus bands replaced keyword tables. No new
  API-pressure findings in Phase B — the ledger's Phase C items
  (#2 fons_via, #3 strata, #4/5/6 vistas) remain the open set.

Complexities (four-part schema):
- WHAT: chorda_sectio takes (initium, FINIS) not (initium,
  mensura). FOUND: writing liber's line accessor; the probatio's
  exact-line asserts predate the first run. CONSEQUENCE: garbage
  slices if shipped. HANDLED: fixed at write time; noted as an API
  shape to remember (sectio = half-open range).
- WHAT: status-line layout priority. FOUND: 5 goldens failed on a
  40-col fixture — right side vanished because the via reserved
  width first. HANDLED: right-side-priority composition; via
  truncates to remainder. The goldens caught a layout bug no
  quick manual glance on a wide terminal would have.
- WHAT: h/l action ambiguity across modes. HANDLED: claves modus
  param (planned in INTENTIO, executed in B3); iussum enum stays
  mode-neutral, mains interpret.

Named deferrals (Phase D unless stated): '/' search, TOC sidebar,
tab-stop expansion (editor story), colored previews in columns
(needs feel-testing; lexing is cheap now that silva_lexare is
public), vim f-motion in fons mode.

**PHASE B CLOSED. Next: SILVA ADDITIONES II** (in silva/, own
INTENTIO; the heavy one: #2 silva_fons_via accessor, #3 STRATA
RETENTION — the ledger's only engine work, #4/5/6 slim vistas;
arm-extent stream pinning during design). Then **Phase C — the
magic**: layers, origo popup, Enter-to-jump. The o key stops
teasing.

## PHASE C — THE MAGIC: INTENTIO (2026-07-03)

**Goal**: THE MONEY SHOT. Layer flips across the retained strata
with token-identity cursor tracking; the origo popup with
Enter-to-jump (cross-file); omitted conditional arms dimmed grey.
All silva.h prerequisites shipped (additiones I+II; ledger closed).

**Design keystone #1: a stratum is a (textus, lineae, classis)
triple.** Phase B's entire render path works unchanged on ANY
stratum — the visum never learns about tokens. Stratum 0 = the
Phase B fields (as-written). Stratum k>0 = MATERIALIZED from
parsura->strata[k-1]: serialize the token stream (spatia_ante
trivia bytes + valor bytes per token), building classis directly
from token genus during the walk (no re-lex), plus the POSITION
MAP: per-token {token*, byte_initium, byte_finis} in stream order
(binary-searchable by byte; scannable by token identity).
Materialization is LAZY (first visit) and cached in the liber
arena. A macro-dense stream with no newline trivia renders as one
long line — accepted (h-scroll exists; named).

**Design keystone #2: the origo popup is direction-aware.**
- On stratum N>0: cursor token → walk the transparent SilvaOrigo
  chain DOWN (datum.expansio.corpus = definition site,
  .invocatio = use site, nomen_macro) to the FONS radix. Rows:
  "stratum N: <valor>" / "← expansio MACRO (via:linea)" per step /
  "scriptum: via:linea". via from silva_fons_via.
- On stratum 0: cursor byte → FORWARD search: scan the final
  stream (parsura->lexemata) for the first token whose chain's
  invocatio byte-range covers the cursor → "what this becomes" +
  its chain. O(stream) per keypress — fine.
- Every row with a resolvable location is a JUMP TARGET (Enter):
  same-file → cursor+aptare; cross-file → nexus resolves the fons
  via (praebere-name → absolute path map, recorded during
  seeding) → bibliotheca/limes open → switch liber. Synthetic or
  unresolvable fons → nuntius "(fons syntheticus)". Jump history
  stays DEFERRED (named, v0.1 with ctrl-o).

**Design keystone #3: dimming is a classis overwrite.** New class
SALT_CLASSIS_OMISSUM (grey 0x707070 per §10.3 — uniform, overrides
syntax color BY DESIGN: flat = inert). After classis builds: for
each ramus vista with !est_sumptum AND fons_index ==
fons_princeps, overwrite classis[corpus_initium..corpus_finis).
Byte extents make this three lines. Stratum 0 only (omitted arms
do not exist in expanded streams — nothing to dim).

**Cursor tracking across flips** (interview Q6): cursor byte in
stratum A → token via A's position map (nearest on trivia) →
RADIX via silva_token_radix → in stratum B: radix byte_offset IS
the position (B=0) or first token in B's map sharing that radix
(B>0); fallback = nearest line proportion. This is the
origo-API-stress feature, on purpose.

**The parse enters the nexus** (spec §4 recipe, finally):
contextus creare + latinam_addere + praebenda pre-seeding (walk
-I dirs from compile_flags.txt — parsed by main, radix-relative —
filum-read each .h, praebere; record praebere-name → absolute
path for jump resolution). liber_aperire gains the parse (cum
contextu) alongside the UNTOUCHED Phase B lex path — coloring
works even when the parse degrades (badge "silva: intermissa",
layer/origo keys no-op politely; DELIBERATE redundancy). Parse
cost 44-150ms → "silva legit..." nuntius drawn+presented BEFORE
the parse. LRU N=8 now carries real weight (84-242MB/root).

**Keys/UI**: L = stratum cycle (status: "stratum N/M"); o and
Enter open the origo popup; popup is an input MICRO-MODE (main
routes SURSUM/DEORSUM/INTRARE/FINIRE to it while open; q/o/Esc
close); popup rendered with tessera_replere interior (its rect
case, as promised at 1.1); wheel/click keep working under it
(close-on-click-outside = deferred nicety).

**Modules**: nexus grows (contextus/praebenda/via-map);
liber grows (parsura, strata materialization, position maps,
stratum_currens, OMISSUM overlay); NEW saltuarius_origo.{h,c}
(chain build → gradus rows + jump targets; popup state);
visum grows (stratum indicator, popup render); limes gains
librum_aperire_via (extracted core; existing fn wraps); main
wires the micro-mode + cross-file jump.

**Chunks**: C1 parse+strata (nexus recipe, liber materialization,
maps, OMISSUM; pure probationes incl. "G(1); → 1+1" stratum text
and dim bytes). C2 origo module + cursor tracking (pure; chain
rows pinned on a fixture: macro name, def via:linea, invocatio).
C3 visum popup + claves/main wiring + cross-file jump + goldens
(scripted full chain: open → flip → o → navigate → Enter → jump
lands) + THE MANUAL BAR: silva_parsare.c, cursor on a TEXERE
invocation, o, chain visible, Enter, land in the macro
definition. The thing no other tool on earth does.

**Exit criteria**: manual bar passed on real rhubarb sources;
suites green (goldens script the full chain headless); degradation
proven (parse-refused file keeps Phase B behavior + badge);
memory ceiling observed under LRU with real parses; RELATIO.

### Chunk C1 — parse + strata: DONE (2026-07-03)

nexus grew the parse pipeline: silvam_parare (contextus +
latinam_addere + praebenda from INJECTED capita — the disk walk
stays in limes, the injection seam holds even here; basename
collision = first wins; praebere-name → absolute-path map for C2
jumps), nexus_parsare (cum contextu, sceleton grammatica),
fons_resolvere, and the public classis-per-token helper (Phase B
classifier refactored onto it). liber grew the stratum model:
strata_visus[] of (textus, lineae, classis, positiones) triples —
[0] shares the Phase B fields; k>0 LAZILY materialized from
parsura->strata[k-1] (two-pass serialization: trivia-ante + valor
+ trivia-post per token, single-owner so nothing doubles; classis
from token genus during the walk; position map for C2); ALL
cursor/line accessors now read the ACTIVE stratum (line index
builder extracted and shared). OMISSUM overlay: non-taken rami of
the root fons painted grey via the BYTE extents — three lines, as
the sim promised.

Suite 10/10; silva 26/26. Probationes pin: "G(2);" materializes
to "((2)+(2))" in stratum 1 TEXT; "int a;" (the #if 0 arm) is
OMISSUM in stratum 0 and ABSENT from stratum 1; stratum clamping;
merus degradation (1 stratum, ponere clamps to 0).

Complexities:
- **THE NOMEN LANDMINE, FIFTH FIRING**: named a struct field
  `nomen` (SaltuariusCaput) and a param the same — chorda typedef;
  — caught at compile, renamed titulus. The forbidden list works;
  the reflex still needs training.
- Contextus parses record latina.h's own #include <stddef.h> as an
  UNRESOLVED inclusio (fons_ad -1) — correct "discens" journal
  behavior; probationes (and the Phase D TOC) must expect
  contextus-origin entries and look includes up BY VIA, not index.

NEXT: C2 — origo module (chain build, direction-aware) + cursor
identity tracking across flips (pure probationes).

### Chunk C2 — origo + vestigium identitatis: DONE (2026-07-03)

fontes/saltuarius_origo.{h,c}: the direction-aware chain builder.
DEORSUM (stratum N>0): position-map hit → walk the transparent
SilvaOrigo chain (EXPANSIO: nomen_macro + corpus def-site
via:linea via silva_fons_via, jumpable; PASTA/CHORDA/API: labeled,
walk continues through sinister/primus; FONS: "scriptum
via:linea", jumpable). PRORSUM (stratum 0): scan the final stream
for the first non-FONS token whose FONS-side ancestry covers the
cursor byte → "fit <valor>" + its chain. Rows built into a
reficere'd private arena (memoria frenata); navigation clamps;
saltus returns the selected gradus only when saltabile.

liber gained cursor↔offset conversion and
saltuarius_liber_stratum_transferre — the interview-Q6 feature:
0→k finds the first target token whose ancestry covers the cursor
byte; k→0 jumps to the radix byte; k→m matches shared radix
identity; proportional-line fallback. All through the PUBLIC
silva.h chain (radix helper + transparent origo union) — the
origo API stress-test the design intended.

Suite 11/11, probatio origo 29/29 FIRST RUN. The probatio output
already prints the money shot as text:
  [0] lexema  +
  [1] <- expansio G  probe.c:1
  [2] scriptum    probe.c:2
The + token knows its whole story. C3 puts this in a popup.

NEXT: C3 — visum popup render (tessera_replere interior) +
stratum indicator + claves/main micro-mode wiring + cross-file
jump (limes librum_aperire_via) + goldens + THE MANUAL BAR.

### Chunk C3 — wiring: CODE COMPLETE (2026-07-03); MANUAL BAR
### PENDING — THE MONEY SHOT

limes: capita_legere (walks -I dirs, reads .h into the PERSISTENT
arena — praebenda live as long as the contextus; missing dirs =
silent degrade) + librum_via (via-based open for cross-file
jumps, gates shared). main: _silvam_seminare (compile_flags.txt
parsed radix-relative → capita → nexus; no file = parse without
praebenda), the origo MICRO-MODE (popup open routes
SURSUM/DEORSUM/INTRARE/FINIRE; L cycles strata via
stratum_transferre — identity tracking live; o/Enter builds the
chain or nuntiates "(origo nihil narrat)"), _saltum_facere
(same-file → stratum 0 + linea; cross-file → fons_resolvere or
absolute → bibliotheca/librum_via → liber switch), "silva
legit..." painted BEFORE the parse hitch. visum: dynamic "stratum
N/M" indicator + saltuarius_visum_tabella (tessera_replere
interior — THE RECT CASE promised at 1.1 — rounded border, ORIGO
title, grey non-jumpable rows, INVERSUM selection).

Suite 11/11 (visum goldens grew the C3 section: stratum 0/1 →
flip → 1/1 with "((2)+(2))" visible in cells; tabella over the
scene with "lexema"/"expansio G" rows; selection INVERSUM moving
with navigation). Cross-project: silva 26/26, tessera 5/5.

REMAINING FOR PHASE C CLOSE — THE MANUAL BAR:
./saltuarius/saltuarius.sh, then silva/fontes/silva_parsare.c:
(1) L — the whole buffer becomes the NEXT LAYER (expansions
unfold, cursor follows the token); L again cycles; (2) cursor on
a macro use (TEXERE, XAR_*, any latina word is prettier still) →
o → THE CHAIN; (3) j/k in the popup, Enter on a definition row →
LAND IN THE HEADER (cross-file through the praebenda map);
(4) #if arms: omitted = grey in stratum 0, GONE in stratum 1;
(5) q closes popup, q again back to columns; (6) a .md: L and o
politely refuse. Then RELATIO and the phase closes.

## RELATIO — PHASE C COMPLETE (2026-07-03): THE MAGIC

**THE MONEY SHOT WORKS ON REAL SOURCES.** Manual bar PASSED:
layer flips across real strata with the cursor following token
identity, the origo popup showing full provenance chains, Enter
landing in the defining header cross-file, omitted arms grey in
stratum 0 and gone above it. Cursor on a token; see where it came
from; press Enter; BE THERE. The thing no other tool shows,
running in a C89 TUI over two amalgams.

Suite 11/11 headless; silva 26/26; tessera 5/5. All three chunks
landed same-day (C1 parse+strata, C2 origo+tracking, C3 wiring).

**The manual bar earned its keep twice** — two real bugs the
goldens had not pinned, both caught by Fran's eyes, both now
pinned forever:
- WHAT: stale colors after L. CAUSE: renderer took TEXT from the
  active stratum but CLASSIS+LINEAE from stratum 0's legacy
  fields — new text, old colors. FIX: the (textus, lineae,
  classis) triple is now read atomically from
  stratum_activum; golden asserts stratum-1 COLORS (nativus '(' ,
  NUMERUS '2'), not just text.
- WHAT: "constPiscina", "typedefstructchorda" in stratum 1.
  CAUSE: expansion-produced tokens do not inherit the replaced
  token's trivia — inter-token whitespace died with the latina
  invocations. FIX: SAFE-JOIN in the materializer (adjacent
  identifier-ish tokens or repeated operator chars get one
  space); golden pins "const int" present and "constint"
  impossible.
Lesson recorded: a MATERIALIZED VIEW is only as honest as its
join rules — token streams are not text until whitespace
provenance is accounted for. (Silva-side note: invocation-trivia
inheritance could someday make materialization byte-faithful for
unexpanded spans — NAMED as a possible additiones III item, not
needed for the viewer.)

What Phase C proved: the stratum-triple keystone held (Phase B's
renderer needed only the atomic-read fix to display any layer);
the direction-aware origo design held (both directions shipped
without revision); identity tracking exercised the public origo
API exactly as the interview intended; the praebenda map turned
fons_index into real jumpable paths.

Named deferrals to Phase D: '/' search + TOC (as planned), popup
close-on-click-outside, jump history (ctrl-o), vim f-motion in
fons mode.

**PHASE C CLOSED. Saltuarius does the thing it was born for.**
Next: Phase D — the polish (TOC sidebar from the vistas, '/'
search, fructus debug line, LRU tuning) → v0 daily-driver bar
(interview Q20) → saltuarius v0 SHIPS.

## PHASE D — THE POLISH: INTENTIO (2026-07-03)

**Goal**: the interview-Q20 DAILY-DRIVER BAR, then v0 SHIPS.
Everything here is a known quantity — no research risk, all
prerequisites shipped (vistas from additiones II, telemetry from
additiones I, micro-mode pattern from Phase C).

**Scope**:
1. **TOC sidebar** (fons mode; the designed M2 pressure point):
   a STRUCTURA pane listing includenda (inclusio vistas — look up
   BY VIA, contextus adds entries like stddef.h; unresolved shown
   honestly with fons_ad -1), macros (macro vistas, journal
   semantics), regiones (ramus vistas: genus + sumptum/numquam
   badge + linea). Tab cycles focus textus ↔ structura; j/k in
   the sidebar; Enter JUMPS — includes resolve through the SAME
   praebenda-map machinery as origo jumps (file switch via
   bibliotheca); macros/regiones jump to their linea in the
   current file. New module saltuarius_structura.{h,c}
   (view-model + render; vista walks at build time, cached per
   liber entry into the sidebar's own reficere'd arena).
2. **'/' search** (spec module saltuarius_quaestio): literal
   non-regex incremental search over the DISPLAYED stratum's text
   (search what you see — interview fiat), as an input MICRO-MODE
   like the origo popup (echo line at status row, every keystroke
   re-searches from cursor, Enter commits, Esc cancels, n/N
   repeat forward/back with wraparound + "(finis)" nuntius). The
   app's first text-input: paste arrives as rune bursts
   (bracketed markers swallowed by tessera) — acceptable, named.
3. **F2 fructus/telemetry line**: toggle swaps the status line's
   left side for: tessera fructus (cellulae mutatae, octeti,
   tempus ms) + silva arena bytes (silva_piscina_summa_usus/apex
   on the current liber's arena — additiones I telemetry earning
   its keep) + cache occupancy (bibliotheca numerus/N).
4. **LRU observation**: with real parses, open 10+ heavy roots,
   confirm eviction keeps the ceiling (the Phase C exit criterion
   that carried); tune N only if the bar demands it.
5. **Deferral DISPOSITION** (decide, not necessarily do): colored
   previews (lexing is cheap now — FEEL-TEST on the manual bar,
   adopt if instant), popup close-on-click-outside, jump history
   ctrl-o, vim f-motion in fons, dotfile toggle, incremental
   type-jump. Each either lands in D3, or gets a NAMED v0.1 home.

**Chunks**: D1 structura (vistas → view-model + render + jumps +
goldens incl. the stddef.h-unresolved row). D2 quaestio ('/' +
n/N + echo line + goldens with scripted keystroke sessions). D3
fructus line + LRU observation + deferral disposition + THE
DAILY-DRIVER BAR (Fran browses + reads + jumps + searches through
a real work session) → v0 RELATIO → spec-v2 marked SHIPPED →
memory close-out.

**Exit criteria** (= interview Q20 verbatim): browse rhubarb
comfortably; money shot reproducible on demand; TOC + '/' work;
suites green; degradation badges proven. "I actually use it" is
the bar. v0 SHIPS.

### Chunk D1 — structura (TOC sidebar): INTENTIO (2026-07-03)

**Goal**: Tab in fons mode opens a STRUCTURA pane — the file's
table of contents built from the additiones-II vistas — j/k
navigates, Enter jumps, Tab/q closes. The designed M2 pressure
point goes live: the day the TOC wants FUNCTIONS listed is the
day M2 (C89 grammar) gets its pull.

**Shape** (mirrors origo exactly — proven pattern):
- NEW saltuarius_structura.{h,c}: view-model with its OWN
  reficere'd arena (creare/aedificare/claudere/movere/saltus).
  Rows built fresh at each open by walking the vistas off
  liber->parsura->expansio — no per-liber caching (rebuild is
  O(vistae), macro_vista O(acta) per call → O(acta²) worst case,
  trivial at TOC scale; caching deferred NAMED to v0.1 if a
  giant file ever makes open laggy).
- Row model: three sections with grey non-jumpable header rows —
  INCLUSIONES (via text + "(non resoluta)" when fons_ad -1 +
  "(praetermissa)" badge), DEFINITIONES (macro name +
  "(functio)" badge, journal semantics AS-IS: redefinitions
  appear twice, #undef erases nothing — the journal is the
  product), REGIONES (#if/#ifdef/#ifndef/#elif/#else genus +
  linea + sumptum/NUMQUAM/omissum badge).
- **SCOPE DECISION**: rows filtered to the PRINCIPAL file
  (inclusio fons_ex == parsura->fons_princeps; macro/ramus
  fons_index == fons_princeps — NB fons_princeps != 0 post
  praebere, already learned). Rationale: it is the TOC of THIS
  file; latina.h alone would otherwise drown every list with
  ~50 macro rows. The filter also auto-excludes latina.h's own
  `#include <stddef.h>` journal entry. DEFERRED NAMED: a
  project-wide/all-fontes view (scope toggle) → v0.1.
- Unbounded row count: selectio + volumen scroll in the
  view-model (origo capped at 16; a TOC cannot).
- Jumps: macro/ramus rows → same-file linea (vista linea is
  UNSIGNED — s32 casts at the boundary). Inclusio rows → land at
  LINE 1 of the TARGET file (the vista carries no include-site
  linea; jumping INTO the header is the ranger-native semantics
  anyway) through the SAME resolution as origo: fons_resolvere,
  absolute-path fallback, bibliotheca/librum_via. main's
  _saltum_facere refactors into a shared _ad_locum_ire(app, via,
  linea) that both origo and structura saltus call.
- Render: saltuarius_visum_structura in visum.c (tabella
  precedent — all painting stays in one file; deviation from the
  phase-INTENTIO's "render in structura module" sketch, for
  consistency). Overlay pane anchored RIGHT, full content
  height, tessera_replere opaque interior + border, INVERSUM
  selection, grey headers/badges. No metra reflow — the text
  pane underneath is untouched (z-order = paint order, as
  tabella proved).
- Claves: TESSERA_CLAVIS_TABULA → SALT_ACTIO_STRUCTURA (fons
  mode). Ctrl+I aliases to Tab by xterm nature — that is the
  SANCTIONED side of the forbidden-bindings pin (we bind TABULA,
  never Ctrl+I separately). Micro-mode dispatch in main:
  origo popup and structura pane MUTUALLY EXCLUSIVE (opening one
  closes the other); dispatch order origo → structura → fons.
- Refusals: textus merus / no parsura / zero rows → "(structura
  nihil habet)" nuntius, pane never opens broken (every-file-
  opens pin).

**Probationes** (probatio_saltuarius_structura.c + growth):
fixture with praebenda latina.h + own `#include <stddef.h>` +
object & function macros + #if 0 / #ifdef arms; assert: section
structure, principal-file filter (latina.h's stddef row ABSENT,
own stddef row PRESENT as non-resoluta — the honest-unresolved
golden from the phase INTENTIO), functio badge, ramus badges,
journal duplicate visible, saltus targets (macro linea; inclusio
via resolvable). Cell goldens: pane over scene, title, INVERSUM
selection moving, grey header. Claves: Tab case. Scroll clamp
probatio for a long macro list.

### Chunk D1 — structura: CODE COMPLETE (2026-07-03); MANUAL BAR
### PENDING

Shipped: saltuarius_structura.{h,c} (view-model, origo pattern:
own reficere'd arena, rebuild-per-open, one-pass vista walk with
fons_princeps filter, selectio starts on first saltabile row,
volumen scroll for unbounded row counts), saltuarius_visum_
structura (right-anchored overlay, replere interior + rounded
border, " STRUCTURA " title, grey CRASSUM section headers,
indented rows, INVERSUM selection), claves TESSERA_CLAVIS_TABULA
→ SALT_ACTIO_STRUCTURA (fons mode only; Ctrl+I aliases in — the
sanctioned side of the forbidden-bindings pin), main micro-mode
_iussum_indicis (dispatch origo → index → fons enforces mutual
exclusion by construction; q from fons closes both), and the
planned refactor: _saltum_facere → shared _ad_locum_ire(via,
linea) with thin origo/index callers (INCLUSIO jumps INTO the
header at line 1; DEFINITIO/REGIO jump same-file).

Suite 12/12 (structura probatio: 55 asserts — filter both ways,
badges, journal duplicate V×2, saltus targets, scroll clamp,
merus refusal, cell goldens incl. INVERSUM movement). App links
clean under the full wall.

COMPLEXITY — `structura` IS A LATINA MACRO (SIXTH firing).
- DISCOVERED WHILE: naming the view-model's parameters in the
  module literally named saltuarius_structura.
- CONSISTS IN: latina.h defines `structura` = struct; the
  module's own noun is unusable as an identifier. Type name
  SaltuariusStructura and function prefixes are fine (different
  tokens); the bare word is poison.
- CONSEQUENCES: every param/local/field named `structura`
  expands to a keyword → syntax soup. clangd caught it PRE-
  compile this time (progress: firing #5 needed the compiler,
  #4 needed the amalgam).
- HANDLED BY: variables/fields named `index` (proper Latin for
  a book's TOC — the better name anyway); forbidden lists in
  all three CLAUDE.md files updated with the GENERAL rule:
  every word #defined in latina.h is reserved — check latina.h
  before coining any identifier.

COMPLEXITY — ramus vista linea is the REGION's line, not the
arm's.
- DISCOVERED WHILE: probatio asserted #else row at :11, got :9.
- CONSISTS IN: silva_ramus_vista faithfully reports
  regio->linea — the OPENING directive's line, shared by every
  arm of the group. The per-arm truth lives in corpus_initium/
  corpus_finis (byte extents of the arm BODY), already public.
- CONSEQUENCES: naive TOC rows for all arms of a conditional
  would jump to the same line.
- HANDLED BY: byte→line conversion in saltuarius against the
  liber's own lineae table (public data — NOT an amalgam
  fall-short, no silva change needed); row displays AND jumps
  to the arm-body line (#else → the `int c;` line — where a
  jump wants to land anyway); empty arm (corpus -1) falls back
  to the region line. Golden pins :12 (else body) and :14
  (empty-arm fallback). Named nicety if it ever itches: vistas
  could carry the arm's own directive line (additiones-III
  class, no pressure yet).

MANUAL BAR (D1 slice): ./saltuarius/saltuarius.sh → open a real
source (silva/fontes/silva_parsare.c is rich) → Tab: pane with
INCLUSIONES/DEFINITIONES/REGIONES → j/k scrolls a long list →
Enter on an include LANDS IN THE HEADER → Tab again on a macro
row lands on its #define → q closes pane, q again to columns →
Tab on a .md politely refuses. The FUNCTIONES itch (M2 pressure
point) is now live — note whether you feel it.

**D1 MANUAL BAR PASSED (2026-07-03, Fran: "the manual bar
implementation looks great"). D1 CLOSED.** The FUNCTIONES-itch
observation stays live through the D3 daily-driver bar.

### Chunk D2 — quaestio ('/' search): INTENTIO (2026-07-03)

**Goal**: '/' in fons mode opens the app's FIRST TEXT-INPUT
micro-mode — literal, non-regex, incremental search over the
DISPLAYED stratum (search what you see — interview fiat). Enter
commits, Esc cancels (cursor restored), n/N repeat with
wraparound + "(finis)" nuntius.

**Shape**:
- NEW saltuarius_quaestio.{h,c}: view-model with FIXED buffers
  (256 bytes query + 256 committed — no arena needed; queries
  are short by nature). State: activa, litterae/mensura (query
  in flight), commissum (survives close — n/N's memory),
  origin snapshot (cursor offset + linea/columna + volumina —
  Esc restores ALL of it), nihil_inventum (echo badge).
- INPUT MODE IS DIFFERENT: while activa, main bypasses
  saltuarius_claves_tradere entirely and feeds the raw
  TesseraEventum to saltuarius_quaestio_tradere — printable
  runes append (encoded to UTF-8 bytes), RETRORSUM deletes one
  RUNE (walk back a whole codepoint, not a byte), REDITUS
  commits, FUGA cancels. claves stays pure command-translation;
  the probationes hand-construct events as ever. Paste arrives
  as a rune burst — acceptable, named (bracketed markers are
  swallowed by tessera).
- Incremental semantics (vim wrapscan model): the ORIGIN offset
  is snapshotted at '/' — every keystroke re-searches from the
  ORIGIN (not the moving cursor, else typing leapfrogs), match
  inclusive at origin, wrapping to the top if needed; cursor
  moves to the match (cursor_ad_offset — the C2 rune/byte
  machinery reused); no match → cursor sits back at origin +
  " (nihil)" in the echo.
- n/N: strictly-after / strictly-before current cursor offset,
  byte-literal (memcmp scan — O(n·m) per keystroke is trivial
  at buffer scale), wraparound sets a volvit flag → main
  nuntiates "(finis)". Empty committed query → "(quaestio
  vacua)".
- Echo line: saltuarius_visum_quaestio overlay painted LAST —
  replere the status row, draw "/query" + optional "(nihil)".
  The right-side status yields while typing (deliberate,
  simple; B2's right-side-priority rule applies to the NORMAL
  status line, not the input echo).
- claves: '/' → SALT_ACTIO_QUAESTIO, 'n'/'N' →
  SALT_ACTIO_PROXIMUM/PRIUS (all fons-mode only). Dispatch:
  quaestio-activa checked FIRST (origo/index are necessarily
  closed — '/' only reachable from _iussum_fontis).
- Search domain: saltuarius_liber_stratum_activum(liber)->textus
  — the DISPLAYED text, so a search in stratum 1 finds what the
  expansion shows, not what the file says. That is the feature.

**Probationes** (probatio_saltuarius_quaestio.c — scripted
keystroke sessions): type-search-land (incremental narrows),
backspace re-widens (and removes a whole UTF-8 rune, not a
byte), Enter commits + n/N walk matches + wraparound flag, Esc
restores cursor AND volumina, no-match keeps origin + flags
nihil, search-what-you-see on stratum 1 (query matching expanded
text only), echo-line cell golden ("/int" visible on status
row). Claves growth: '/', 'n', 'N' fons-only cases.

### Chunk D2 — quaestio: CODE COMPLETE (2026-07-03); MANUAL BAR
### PENDING

Shipped exactly as the INTENTIO drew it: saltuarius_quaestio.
{h,c} (fixed 256-byte buffers, origin snapshot with volumina,
vim-wrapscan incremental from ORIGIN, n/N strictly-after/before
with volvit flag), raw-event bypass in principale (input mode
skips claves entirely — the dispatch guarantee holds: '/' only
reachable from _iussum_fontis, so origo/index are closed),
saltuarius_visum_quaestio echo overlay painted last, claves
'/'/'n'/'N' fons-only, nuntii "(finis)"/"(nihil inventum)"/
"(quaestio vacua)".

MONOREPO GROWTH (not a complexity — the system working): the
utf8 lib had decode/walk but NO ENCODER; text input needs
rune→bytes. utf8_codere added to lib/utf8.c + include/utf8.h
(surrogates/range rejected, 1-4 byte forms), probatio_utf8
grew a codere section incl. codere→decodere roundtrip; root
suite green. First text input in the monorepo pulled the
library forward — quaestio consumes it for append, and
utf8_prior_runa (already there) does rune-whole backspace.

Suite 13/13 (quaestio probatio: 49 asserts FIRST RUN — the
incremental-narrowing fixture is self-documenting: 'i' lands in
"#defIne" at offset 4, 'in' holds, 'int' jumps to the real
declaration; search-what-you-see proven both directions on
stratum 1 vs 0 with "2)+(2"). App links clean. Zero new
complexities — D1's landmine list and the C2 cursor machinery
(cursor_ad_offset) carried the whole chunk.

MANUAL BAR (D2 slice, fold into D3's daily-driver bar or eyeball
now): / then type — watch the cursor chase the narrowing match;
Backspace widens; Enter, then n n n to a "(finis)" wrap; N back;
Esc from a garbage query — cursor snaps home; L to stratum 1,
'/' an expansion-only string — found there, "(nihil)" on
stratum 0.

NEXT: D3 — F2 fructus line + LRU observation + deferral
disposition + THE DAILY-DRIVER BAR → v0 RELATIO → v0 SHIPS.

### Chunk D3 — fructus + dispositio: INTENTIO (2026-07-03)

- **F2 fructus line**: SALT_ACTIO_FRUCTUS (TESSERA_CLAVIS_FUNCTIO
  numerus 2, fons mode), toggles res->fructus_visibilis; overlay
  saltuarius_visum_fructus takes the WHOLE status row (debug
  view; quaestio echo precedent) with: praesentationes, cellulae
  mutatae (cumulativae — tessera zeroes only at aperire), octeti,
  ms MEDIUM (tempus/praesentationes, tenths), silva arena
  summa/apex in MB tenths (the additiones-I telemetry earning its
  keep; "-" for merus), libri N/8 (THE LRU instrument — the
  observation happens live on the daily-driver bar).
- **Click-outside closes popups**: CLICUS → claudere in both
  micro-modes (deferral LANDED — 6 lines).
- **Deferral disposition** (the rest): colored previews = NOT
  instant (threads nexus/classis into colorless Phase A columnae
  + limes preview arena) → v0.1 FIRST ITEM if the daily-driver
  bar itches; ctrl-o jump history → v0.1; vim f-motion in fons →
  v0.1; dotfile toggle → v0.1; incremental type-jump → v0.1.
- Probationes: F2 claves case; fructus-line cell golden (praesentare
  once on memoria pons to populate counters, assert "libri"/"arena"
  visible). Click-close is main-level (not link-visible headlessly)
  — manual bar covers it, named.

### Chunk D3 — fructus + dispositio: CODE COMPLETE (2026-07-03);
### THE DAILY-DRIVER BAR REMAINS

Shipped: F2 → SALT_ACTIO_FRUCTUS (FUNCTIO numerus 2, fons only;
F3 pinned inert) toggling res->fructus_visibilis;
saltuarius_visum_fructus full-row telemetry overlay; CLICUS
closes both popups (deferral LANDED). Suite 13/13; app links.

COMPLEXITY (small, caught by golden): the fructus line first
led with tessera counters — on the 40-col fixture terminal the
"libri N/8" tail CLIPPED. The B2 lesson re-fired in new clothes:
on narrow terminals the MOST DIAGNOSTIC fields must survive.
Reordered: libri N/8 (LRU instrument) → arena summa/apex MB →
q/ms/mut/oct (tessera counters, sacrificial tail). Golden now
pins "libri 3/8" and "arena" visible at 40 cols.

DEFERRAL DISPOSITION (each with its named home):
- Click-outside closes popups → LANDED (D3).
- Colored previews → v0.1 FIRST CANDIDATE, contingent on the
  daily-driver feel-test. NOT instant: threads nexus/classis
  into colorless Phase A columnae + limes preview arena. The
  lexing itself IS cheap (additiones I proved it); it's the
  plumbing that costs.
- ctrl-o jump history → v0.1 (needs a jump stack; pairs with
  origo/TOC jumps naturally).
- vim f-motion in fons mode → v0.1 (claves already carries
  f-pending; only the fons-side cursor hop is missing).
- dotfile toggle → v0.1 (tabularium filter flag).
- Incremental type-jump in columns → v0.1 (tabularium prefix
  match; interacts with f-motion design).

WHAT REMAINS FOR v0 (the only unchecked exit criteria):
1. THE DAILY-DRIVER BAR (interview Q20 verbatim): Fran browses,
   reads, jumps, and searches through a REAL work session.
   "I actually use it" is the bar.
2. LRU OBSERVATION during that session: F2 on, open 10+ heavy
   files (silva/tessera fontes are the heavy ones), watch libri
   hold at 8/8 and arena numbers stay per-file sane (84-242MB
   ceiling per parse arena was the measured range). Tune N only
   if the bar demands.
3. The FUNCTIONES-itch note (M2 pressure point) from living in
   the Tab pane.
Then: v0 RELATIO + phase-log audit (phase-boundary rule: re-read
the WHOLE log, every complexity resolved-or-parked) + spec-v2
marked SHIPPED + memory close-out. SALTUARIUS v0 SHIPS.
