# Saltuarius — Spec v1

*The forest warden: a TUI code explorer for the rhubarb monorepo.
Miller-column file browser outside, silva-powered source explorer
inside. The first true application in the monorepo and the
real-host proof for both amalgams (silva.c, tessera.c).*

Interview record: project-specs/saltuarius-interview.md (2026-07-03,
20 questions). This is the v1 spec written from that conversation;
v2 (codebase-grounded) follows agent exploration.

---

## 1. Identity

- **What**: `saltuarius [radix]` — open a terminal file browser
  rooted at radix (default cwd). Ranger/joshuto-style miller
  columns navigate the filesystem; Enter on a file opens a
  full-screen SOURCE VIEW where silva's preprocessor understanding
  becomes interactive: lexeme coloring, layer flips, origo chains,
  conditional regions, a navigable structure TOC.
- **Why**: (a) daily utility — a fast explorer for the monorepo;
  (b) the real-host proof — silva and tessera consumed through
  their amalgam boundaries by a program with real needs; (c) the
  M2 forcing function — the moment the TOC wants "functions in
  this file," M2's grammar work has its concrete customer.
- **The money shot** (v0 acceptance scenario): cursor on a token
  in expanded source of silva_parsare.c → key → the provenance
  chain renders (macro definition site ← include path ← layer) →
  Enter on a chain link jumps to that file at that location.
  Go-to-definition for the preprocessor. Nobody else has this.

## 2. Non-negotiable pins

1. **Amalgam boundary is the API**: saltuarius includes
   silva/amalgama/silva.h and tessera/amalgama/tessera.h — NEVER
   either project's fontes/. Where the amalgam surface is
   insufficient, that gap is a deliverable finding (API pressure),
   not a license to reach inside.
2. **Monorepo citizen otherwise**: app state uses rhubarb libs
   normally (piscina, chorda, xar, chorda_aedificator, ...).
3. **Fines degrade, never break** (inherited posture): every file
   opens, worst case as plain text + status badge; parse failure
   never kills browsing.
4. **Library-shaped app**: view-model + render code in
   saltuarius/fontes/, testable through the memoria pons;
   probationes assert on the CELL GRID (content), not escape
   bytes. Thin main in instrumenta/principalia/.
5. **House rules wholesale**: C89 Latin, silva/tessera CLAUDE.md
   discipline (phase-log INTENTIO/RELATIO, ≤31-char names, i32
   unsigned / s32 for sentinels, forbidden identifiers
   nomen/registrum/magnitudo/exire).

## 3. The two worlds

### 3.1 Columns world (ranger outside)

- Three miller columns: parent / current / preview.
- Preview pane: for .c/.h — lexeme-colored head of the file (once
  silva lands, Phase B); for other text — plain head; for
  directories — entry listing.
- Type-to-jump: typing letters selects the next matching filename
  in the focused column.
- Ordinary file-manager niceties are NOT v0 scope (no
  copy/move/delete — this is an explorer, not a manager).

### 3.2 Source world (silva inside)

Full-screen view replacing the columns (q returns). Components:

- **Text pane**: line numbers + lexeme-colored source of the
  CURRENT LAYER. Colors by lexeme genus (comment, string, number,
  directive, keyword) + latina macro names as their own class
  (the Latin-visible screen). Conditional regions: taken arms
  full-color; omitted arms present but dimmed (tree citizens,
  visibly).
- **Structure sidebar (TOC)**: navigable lists — includenda,
  macro definitions, conditional regions. Tab focuses; Enter
  jumps the source cursor. This is M1's table of contents; its
  poverty relative to "functions" is the designed M2 pressure.
- **Layer flip**: L cycles stratum 0 → 1 → ... → max → 0. Whole
  buffer re-renders as that layer's text. Cursor tracks TOKEN
  IDENTITY through the flip (via origo/extents both directions):
  same logical thing, new text. Status line shows the stratum.
- **Origo popup**: o on any token opens the provenance chain
  (definition sites, include path, layer ancestry) as a MENU;
  Enter on a link jumps (opens that file's source view at that
  position). Links outside the parsed universe: plain-text open
  if the file exists, else status message.
- **In-file search**: / opens literal incremental search over the
  displayed layer's text; n/N cycle matches.
- **Status line**: path · line:col · stratum N · region badge when
  cursor is inside a conditional arm · transient messages. Debug
  key toggles tessera fructus counters (cellulae mutatae, octeti,
  ms) into the line.

## 4. Silva integration

- **Project shape**: radix + compile_flags.txt (-I lines) if
  present; CLI -I overrides/adds. Rhubarb works with zero config.
- **Contextus**: one long-lived SilvaContextus configured at
  startup (generous default fines — interactive tool, caps
  degrade visibly, never crash).
- **Parse cache**: parse on first open; keep forever; one piscina
  per parsed root file (its include graph lives in the same
  arena). Re-opening = cache hit. Duplicate parsing of shared
  headers across roots accepted in v0. Eviction/dedup = v0.1.
- **Degradation**: intermissa/fail-loud/non-C → plain-text view +
  truthful badge; layer/origo keys no-op with a message.

## 5. App architecture sketch (v2 refines against real APIs)

    saltuarius/
      CLAUDE.md            orientation (pins + protocol)
      phase-log.md         INTENTIO/RELATIO record
      compile_probationes.sh
      saltuarius.sh        build + run the app (tty)
      fontes/
        saltuarius_res.h        app-wide state (Res = the thing)
        saltuarius_tabularium.* directory listing model (columns)
        saltuarius_columnae.*   miller columns view-model + render
        saltuarius_liber.*      an opened file: text, lines,
                                parse handle, per-layer text,
                                cursor, scroll (liber = book)
        saltuarius_visum.*      source view render (text pane,
                                sidebar, status line)
        saltuarius_origo_tab.*  origo popup view-model + render
        saltuarius_quaestio.*   in-file search state
        saltuarius_nexus.*      silva bridge: contextus, cache,
                                lexeme→color, token↔position,
                                degradation classification
        saltuarius_claves.*     key/mouse dispatch per mode
      probationes/
        probatio_saltuarius_*.c cell-grid goldens via memoria pons
      instrumenta/principalia/
        saltuarius.c            thin main: posix pons, arg parse,
                                event loop

Event loop shape: tessera_eventum_expectare → claves dispatch →
view-model mutation → visum pingere → tessera_praesentare.
Everything between pons and praesentare is probationes territory.

## 6. Input map (v0)

| Key | Columns | Source view |
|-----|---------|-------------|
| h/←, l/→ | parent / enter | horizontal cursor |
| j/k, ↓/↑ | selection | cursor line |
| Enter | open (dir or file) | activate (TOC/origo jump) |
| q | quit app | back to columns / close popup |
| gg / G | top / bottom | top / bottom |
| ctrl-d/u | half page | half page |
| Tab | — | cycle focus (text ↔ TOC) |
| L | — | cycle stratum |
| o | — | origo popup on cursor token |
| / | type-to-jump (implicit) | incremental search; n/N |
| F-debug | fructus toggle | fructus toggle |
| wheel | scroll focused pane | scroll focused pane |
| click | focus + select | focus + move cursor |

(Exact debug key and any conflicts resolved in v2.)

## 7. Phases

- **Phase A — the walk** (no silva): scaffold + columns browse
  rhubarb with plain-text previews, type-to-jump, resize,
  suspend/resume. Exit: usable browser, cell-grid goldens for
  navigation, runs as daily tool.
- **Phase B — the look**: source view with lexeme coloring
  (silva enters via nexus), degradation path (plain text + badge)
  proven on non-C files. Exit: colored source of any rhubarb file,
  goldens for the render.
- **Phase C — the magic**: layers + token-tracked cursor + origo
  popup + jump. Exit: THE MONEY SHOT on silva_parsare.c, goldens
  scripting the full chain.
- **Phase D — the polish**: navigable TOC, / search, status
  refinements, fructus toggle. Exit: the daily-driver DONE bar
  (interview Q20) in full.

Each phase: INTENTIO before, RELATIO after, mid-phase simulation
where warranted (3-for-3 so far).

## 8. Named deferrals (not lost, chosen)

- ':' command line + cross-file quaerere (v0.1 headline)
- Per-invocation macro unfold (fold metaphor)
- Jump history / back-stack (ctrl-o)
- LRU parse eviction + shared-header dedup
- File operations (copy/move/delete)
- Click-to-follow links; drag
- User-resizable panes; config file
- Functions/typedefs in the TOC ← **THE M2 TRIGGER, by design**

## 9. v2 exploration agenda (the questions that matter)

1. **THE load-bearing one**: map silva.h's actual public surface
   against every interaction here — per-token origo chain access,
   layer enumeration/text reconstruction, token↔line:col mapping,
   extents queries, region/arm enumeration, macro-definition
   listing, include listing. Every gap → listed as "API pressure
   finding" with a proposed silva.h addition (additive to M1's
   gates).
2. Tessera.h surface check: everything §3–6 needs (popup drawing =
   quadrum + cells; no widget assumed).
3. compile_flags.txt parsing (trivial, but check rhubarb's actual
   file), directory reading strategy (opendir is POSIX — where
   does app-level POSIX live, given saltuarius is an app not a
   library? A saltuarius-side pons or plain #includes in fontes?
   Decide with codebase eyes).
4. Lexeme-color mapping: what silva.h exposes about lexeme genus
   on layer-0 vs expanded tokens; where the C89-keyword and
   latina word-sets live (latina datum is IN the silva amalgam —
   reusable?).
5. Cell-grid golden ergonomics: assertion helpers worth building
   once (chorda-from-row, cursor-position probe) in probationes.
6. Scale check: parse wall-time and arena size for the biggest
   real roots (silva_parsare.c with full include graph; the
   460KB amalgam itself as a stress case) — informs whether
   parse-on-open ever needs a spinner.
