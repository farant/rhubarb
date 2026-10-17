# colloquia — Claude Code sessions, tmux and notifications in vicus

*Written 2026-10-15 in secunda from Fran's idea and answers. The
question: Fran works with several Claude Code sessions in tmux panes;
the hooks fire AppleScript notifications that open Script Editor when
clicked. Vicus should know about the sessions: native notifications
that jump to the right conversation, a view of every session with a
status line, and tmux re-attached when vicus restores a terminal.
Built as reusable libraries first, the vicus features composed of
them.*

*Discipline: phases land one at a time, each red-first with plants,
each committed with its gates. PULL rule - nothing built ahead of the
phase that needs it. Header text shown to Fran before coding.*

## I. Facts established (2026-10-15)

- Hooks run as children of the Claude process and inherit the tmux
  environment: `TMUX_PANE` (e.g. `%17`) names the exact pane - `tmux
  display-message -p -t %17 '#{session_name}:#{window_index}...'`
  resolves it. No process-tree guessing needed.
- `tmux list-panes -a -F ...` gives session, window, pane, pid, tty,
  current command (Claude panes report their version, e.g. `2.1.296`)
  and path. tmux 3.4 at /opt/homebrew/bin/tmux.
- Hook stdin JSON carries `session_id`, `transcript_path`, `cwd`.
  Titles live in the transcript: `custom-title` (/rename) and
  `ai-title` records (what ~/.claude/hooks/notify.sh reads today).
- Transcripts are large (this session: 220 MB): read TAILS only,
  never parse whole files. They live under the directory the session
  STARTED in (~/.claude/projects/<cwd-mangled>/<id>.jsonl).
- House pieces: `processus` (commands), `reactor` (fd watchers and
  timers), `internuntius` (RPC envelope, transport-agnostic).

## II. Decisions (Fran, 2026-10-15)

1. Libraries first, features composed of them.
2. Names: `multiplexor` (tmux), `praeco` (hook CLI, "herald"),
   `colloquia` (sessions).
3. Hook -> vicus channel = an append-only event log FILE under
   ~/.rhubarb/ (works with vicus closed; history for free), not a
   socket.
4. Status lines: automatic from hook events (prompt, tool, waiting for
   permission, done) plus explicit `praeco status "..."` that agents
   are told to run (CLAUDE.md / skill) - no Claude Code modification.
5. macOS notifications: vicus toasts replace them while vicus is
   frontmost; a system notification (and sound) only when vicus is in
   the background - later, small.
6. Order T1 -> T7 below.

## III. Phases

| Phase | Content |
|---|---|
| **T1** | `multiplexor`: typed wrapper over the tmux CLI - list sessions / windows / panes into structs (pure parsing of `-F` output, fixture-tested), select window / pane, switch client, attach command, capture, send keys; live smoke against real tmux. |
| **T2** | `praeco` CLI + event log library: one line per hook event (time, session id, event, TMUX_PANE, cwd, message); hooks switched over from notify.sh; `praeco status "..."`. |
| **T3** | `colloquia`: sessions from the log + transcript tails (titles) + `multiplexor` (pane alive, window): status working / needs-you / done / idle, with status line. |
| **T4** | vicus notifications: model (source, title, body, time, read, action), toasts, notification-center view; Claude action = focus the pane (vicus terminal already attached to that tmux session -> its tab + select-window/pane; else open a terminal in the left pane with `tmux attach -t <session>`). |
| **T5** | vicus sessions view: every session with status line, click to jump. |
| **T6** | tmux re-attach on restore: a terminal remembers its tmux session and vicus re-attaches it. |
| **T7** | EXPERIMENT (progressive enhancement): Claude Code channels (MCP servers that push messages into a running session) - ask an agent for a status line, deliver a reply into the conversation. Verify the current mechanism against the docs first; a general `canales` library if it pans out. Nothing in T1-T6 depends on it. |

### Follow-ups (Fran)

- **T4f** (2026-10-16, revised 10-17): ~~Cmd+` toggles the
  notifications center~~ - Cmd+` is macOS's "Move focus to next window"
  (on by default), so Fran chose Cmd+] / Cmd+[ (free in macOS, vicus,
  pictor, terminale and fenestra's menus) and a CYCLE of the active
  tab's right pane: home -> notifications -> sessions -> home, Cmd+[
  reversed. Home = topmost non-cycle pane in the right stack (none: the
  bottom one). A step to notifications/sessions opens it or brings it
  to the top (as the badge does), focus right. A step home closes the
  cycle panes above home (as Esc) and restores the focus home had when
  a cycle pane first covered it - by Cmd+], the badge or
  `$notifications`, so a cycle can start from the badge. Steps are
  queued and applied in vicus_pulsare in order (two presses in one
  frame = two steps). API: `vicus_cyclum_ponere`, `vicus_cyclo_gradi`
  (include/vicus.h). Tests: probatio_vicus_acervus VIII-XIII.
  STATUS 10-17: green, 7 plants red; Fran's look OK.
  ADDED 10-17 (Fran): the agenda as the last stop - home ->
  notifications -> sessions -> agenda -> home. The agenda is
  calendarium with argument "agenda", so cycle entries became type +
  argument (identity as vicus_acervo_aperire): `vicus_cyclum_ponere(v,
  genera, argumenta, numerus)`. A calendarium pane without an argument
  stays home (test XIII). 4 more plants red. Fran's look OK.

### Phase 2 (Fran, 2026-10-15): gate and test progress dashboard

| Phase | Content |
|---|---|
| **T8** | Live runs from mensor's journal: open runs (started, not finished) with their nested children (fabrica actions, suites, credo sections) and status; estimate from history (median of the last N runs of the same gate on this machine); "stage x of y" = children done vs the last run's count. |
| **T9** | Run -> Claude session: join on cwd/worktree and the hook log (PreToolUse carries the Bash command and TMUX_PANE); fallback: `praeco` exports MENSOR_SESSIO into the session via CLAUDE_ENV_FILE at SessionStart. |
| **T10** | vicus dashboard: per session, running gates with elapsed / estimated time, stage x/y, failures as they happen; also runs started outside any session (no link). Beside or inside the T5 sessions view. |

### Phase 2 facts (research 2026-10-16, read-only)

1. **Live at the outer level only.** Runs write `initium` at start and
   `finis` at end (lib/mensura, analytica spec II): commissio, phases,
   `porta:*`, chassis scripts - an open run = initium without finis.
   BUT fabrica actions and credo sections (`member#section`) write BOTH
   lines after the action ends, initium backdated
   (tools/fabrica_memoria.c). Inside a gate, finished children stream
   in; the running ones are invisible.
2. **Two stores.** Lines append to ~/.rhubarb/mensurae.diarium;
   `mensor curare` (detached) folds it into ~/.rhubarb/mensurae.volumen
   when it passes 256 KB or daily. A 30-minute commit can be folded
   mid-run: its initium in the volume, its finis later in the diary.
   `mensura_cursus_legere` reads the volume only - a live view needs the
   volume's open runs + a diary follower (as admonitiones follows
   eventa.jsonl).
3. **Running vs killed is indistinguishable.** CURSUS_INTERRUPTUS means
   "no finis (or still running)". The id carries a pid, but for
   silva.py runs (`mensor cursus incipere` subprocess) it is the
   short-lived mensor's pid. Needs an owner pid recorded, or a
   heartbeat.
4. **Nesting is there.** parens via MENSOR_PARENS: commissio -> porta:X
   -> fabrica actions -> credo sections. Real commit 1dc057b6: 206 s,
   generata 83 s (bearssl_amalgama 53 s), radix 74 s.
5. **Whole-gate medians are useless as estimates.** porta:radix: median
   81 s, range 23..580 s (n=147) - bimodal by fabrica cache hits
   ("membra cursa 0/351"). `fabrica sanare -siccum` already computes the
   plan (N agenda, M fortasse, ~X s from each member's last run). Stage
   "x of y" needs that plan recorded at the START of a composite - today
   nothing records it.
6. **T9 join is direct.** Claude Code exports `CLAUDE_CODE_SESSION_ID`
   (= the hook's session_id) and TMUX_PANE into Bash commands; a
   detached commissio inherits them. mensura already reads CLAUDECODE
   (vocator agens) but records neither. Recording the session id at
   initium replaces the CLAUDE_ENV_FILE fallback. (Verify it survives
   the double fork.)

7. **The record format is frozen by sharing.** `mensura_cursus_ex_linea`
   demands exactly XIV (initium) / XII (finis) fields, and
   `mensura_condere` DROPS a malformed line and then truncates the
   diary. Every tree's bin/mensor folds the shared diary - so a new field
   written by secunda would be silently destroyed by quinta's (or any
   unmerged tree's) next fold. New facts about a run go in as ordinary
   MENSURA records (sessio id parens titulus valor unitas momentum,
   parens = run id), which every binary already accepts.
8. **Owner pid.** `mensor cursus incipere` (silva.py runs) is a child
   of the owner: getppid() = the Python process that will write finis.
   `mensor cursus currere -- cmd` waits for its child: getpid(). No
   liveness call exists; house precedent (fabrica core count) shells
   out via processus rather than sysctl(3): `/bin/ps -o etime= -p PID`
   gives existence + start time (start after the run's initium = pid
   reused). Builds fork constantly, so pids wrap within hours - the
   start-time check is needed, not optional.
9. **fabrica has no up-front plan in a real run.** The wave path
   (`_sanare_per_undas`, lib/fabrica_sanare.c) judges a wave, runs its
   stale members, then judges the next; only `-siccum` judges all
   first (71 s for radix). Known at start: the members in scope and the
   waves (`fabrica_undas_formare`). Stale counts appear wave by wave.

## IV. Status

- T1 DONE 2026-10-15: `multiplexor` (lib/multiplexor.c) - panes and
  clients parsed from our TAB `-F` format, has-session, select
  window+pane, switch-client, attach vector; exact `=name` targets;
  `-S` for socket paths (tests run a private server in $TMPDIR).
- T2 DONE 2026-10-15: `praeconium` (lib) + `praeco` (tools/praeco.c:
  hamus / status / cauda); JSONL log ~/.rhubarb/colloquia/eventa.jsonl,
  one O_APPEND write per event, UTF-8-safe 200-byte cut; installed via
  `installata` at the main merge. Hooks in ~/.claude/settings.json: diff
  for Fran after install.
- T3 DONE 2026-10-15: `colloquia` - per-session status replay, pane-
  attached `praeco status` lines, titles from transcript tails (256 KB;
  verified on a real 220 MB transcript), live panes via multiplexor.
- T4 split (Fran 2026-10-15): T4a model + watcher + center, T4b toasts,
  T4c jump (tty in pseudoterminale/terminale, terminal command argument).
- T4a library part DONE 2026-10-15: `admonitiones` (sorted index, copies,
  ex_eventu) + `praeconium_legere_ab` (incremental read). Next: vicus side
  of T4a (colloquia system + notification-center genus; manifest first).
- T4a vicus side 2026-10-15: generic vicus hook (`vicus_systema_addere`,
  `vicus_radicem_scribere`); `admonitiones_visus` (center genus + log
  follower); root fields `admonitiones_non_lectae`, `admonitio_nova`,
  `admonitiones_lectae_ad` (persistent); `$notifications` opens the
  center; Enter / double-click selects the tmux pane (multiplexor).
  The follower starts only via `vicus_applicatio_admonitiones_aperire`
  (apps), never in tests. Deviations in lib/admonitiones_visus.worklog.md.
- T4b 2026-10-15: generic vicus layer (`vicus_stratum_addere`) +
  `admonitiones_volantes` (toasts top-right: 8 s finished / 30 s needs
  you, 3 max, click = action + read, x = dismiss) + unread badge in the
  tab line (click opens the center). Suppress-while-center-visible
  deferred.
- T4c 2026-10-15: jump = select pane in tmux + focus the vicus terminal
  attached to that session (any tab) or open one in the active tab
  ("tmux <socket> <session>" argument - persisted, so it re-attaches on
  restore: most of T6). pseudoterminale tty, terminale argv + tty,
  vicus_latus_focare; terminals drop TMUX/TMUX_PANE.
- T5 2026-10-15: `$sessions` pane (colloquia_visus) - groups Needs you
  / Working / Done / Idle (Closed with 'c'), status line, Enter/double
  click jumps (shared saltus). Incremental fold in colloquia
  (ColloquiaStatus). Pane liveness now per tmux server (T3 caveat
  fixed for the view).
- T6 2026-10-16: terminals attached by hand (`tmux attach`) are noticed
  (system "terminalia", tty -> tmux client, every 5 s) and saved as
  "tmux <socket> <session>"; detaching clears it. Restore (and every
  jump) runs `$SHELL -l -c 'tmux attach ...; exec $SHELL -l'`: shell
  after detach (Fran 10-16). vicus_latus_argumentum_ponere.
- T4d 2026-10-16 (Fran): Esc in the notifications or sessions pane pops
  it (vicus_acervo_redire; opt-in VicusFacies.effugium_redit), the
  previous pane returns.
- T4e 2026-10-16 (Fran): read notifications hidden by default, `r`
  toggles (persistent); x moves the selection to the next unread.
- T5b 2026-10-16: a sessions pane restored from the layout (mounted
  before the log sources open) stayed empty - the source is now always
  passed and fills once opened.
- T6b 2026-10-16: restored/jump tmux terminals wait until vicus answers
  a DSR query before `tmux attach` (tmux forwards query replies later
  than 5 s as keystrokes; vicus startup is 9-13 s because pictor
  replays its history - parked …GCKD0W2).
- HOOKS LIVE 2026-10-16: ~/.claude/settings.json calls
  `~/.rhubarb/colloquia/praeco hamus` (async) on 7 events; notify.sh
  replaced by plain `afplay` (Ping on permission, Glass on Stop) - no
  native notification (Fran). Backup:
  ~/.claude/settings.json.bak-colloquia-2026-10-16. The binary is an
  AD HOC copy of secunda's bin/praeco (never ~/.bin from a worktree).
- T8a 2026-10-16: `processus_vivus` (pid alive + start time via
  `ps -o etime=`) and the pure `processus_aetatem_legere`. Plants 5/6
  red; the `pid <= 0` guard is unobservable (`ps -p 0` exits 1 itself)
  and stays as defence.
- T8b 2026-10-16: `mensor cursus incipere` / `currere` write a
  `cursus.custos` measurement (getppid / getpid) beside the run;
  `mensura_mensuras_legere` reads measurements back by title prefix.
  Plants: reader 1/2 red (the genus check is redundant - mensura_ex_linea
  rejects run lines); mensor 3/3 red by hand - silva.planta does not
  rebuild bin/mensor for the `currit` gate (false "porta muta").
- T8c 2026-10-16: fabrica `progressum_notare` (FabricaSutura): start
  (0/N, waves W), after each wave (cumulative), serial path after each
  member; siccum never. bin/fabrica writes `fabrica.ambitus` /
  `fabrica.undae` / `fabrica.iudicata` measurements under MENSOR_PARENS
  (throttled to one line a second, last always; alien root without an
  explicit diary writes nothing - LV guard). `mensor cursus factum`
  (new CLI) writes a measurement on the open run in the RUN diary
  (`mensor addere` defaults to /tmp/mensurae.diarium - wrong file);
  silva.commissio records `consilium.portae`. The "last line always"
  rule is unobservable in the fixture (members take > 1 s).
- FOUND 2026-10-16: the ~1000 s IUDICIUM durations were a CLAMSHELL
  SLEEP (pmset: 10:45:04-11:01:49 local). lib/processus times and
  kills by WALL clock (gettimeofday): sleep past 600 s = generator
  killed on wake. silva uses time.monotonic (pauses in sleep). Fran:
  fix it (T8d0, processus + mensor on CLOCK_UPTIME_RAW), ship on code
  review; the real lid-close probe (sleep 60 under a 120 s timeout,
  lid shut ~3 min, old build red / new green) is OWED.
- T8d0 2026-10-16: lib/processus_posix.c `_tempus_ms` and tools/mensor.c
  `_horologium_ms` on HOROLOGIUM_VIGILANS = CLOCK_UPTIME_RAW (Darwin),
  CLOCK_MONOTONIC elsewhere; every use is a difference. Lexicon
  (silva/fontes/systema_posix.h) gains CLOCK_UPTIME_RAW 8 (auspex
  certifies) - before, examen analysed the #else branch - and getppid
  (examen suspected T8b's call as implicit). Glossary: uptime, getppid.
  No automated test can sleep the machine: shipped on review (Fran).
- T8d 2026-10-16: `cursus_quaerere_currentes` (open runs as a tree;
  owner, gates x/y, members judged after the LATEST scope, finished
  children - gates under a commit, non-IUDICIUM under a gate - and the
  last five passing runs' range) and `mensor quaestio currentes`
  (liveness in the CLI: processus_vivus + born-after-the-run = reused
  pid; dead roots hidden and counted; pre-T8 roots shown one day;
  `-machina` CURRENS lines carry a life column for T10). Fumus XVI fakes
  a reused pid with `cursus factum` (a newer process as owner). Plants:
  lib 5/5, CLI 3/3.
- GAP (seen live during T8d's own commit): judge-only gates (generata =
  `bin/fabrica iudicare`) write no member progress - progressum_notare
  lives in sanare only. Follow-up for T10: progress in iudicare too.
- T9 2026-10-16 (Fran: option A): a TOP-LEVEL run started under Claude
  Code carries a measurement titled `colloquium:<CLAUDE_CODE_SESSION_ID>`
  (value 1; title = key, like cursus.custos). The variable survives the
  detached double-fork commit (checked). currentes: CursusCurrens.
  colloquium (children inherit the root's), `colloquium=<id>` filter,
  root tag `[colloquium <8>]`, last -machina column. CLAUDE_CODE_SESSION_ID
  is undocumented - fallback if it vanishes: praeco exports our own
  variable at SessionStart via CLAUDE_ENV_FILE. Plants 4/4.
- T10 decisions (Fran 2026-10-16): mensor installed to ~/.bin (like
  praeco) for vicus to call; runs shown INSIDE $sessions under each
  session; an "Other runs" group for runs with no session. vicus polls
  `mensor quaestio currentes -machina` asynchronously (single-threaded
  UI; the query costs 0.3-0.5 s).
- T10a 2026-10-16: the CURRENS line is a lib contract
  (cursus_currentem_ad_lineam / _ex_linea, 19 TAB fields, CursusVita
  moved from mensor into the lib); CursusCurrens.fracta = failed
  children's titles (space-joined, IUDICIUM excluded); mensor prints
  -machina through the lib and lists failures from fracta. Plants 3/3
  (the separator plant needed a second failed member in the fixture).
- T10b 2026-10-16: ColloquiorumFons polls `mensor quaestio currentes
  -machina` in a child process (processus incremental path: never
  blocks), every 5 s while pulsed, 30 s query timeout; keeps only trees
  whose root owner is ALIVE (children follow the root); FNV signature ->
  VERUM on change; a failing mensor keeps the old snapshot.
  `colloquia_visus_fontem_mensorem_ponere` / `colloquia_visus_cursus`.
  vicus resolves RHUBARB_MENSOR, then ~/.bin/mensor (untested until the
  T10c display test). aedificatio: institutio_mensor in installata
  (installs from MAIN after the merge). Plants 4/4.
- T10c 2026-10-16 (Fran's look: both joins seen): $sessions shows one
  line per running root under its session ("commit rhubarb-secunda 1m40s
  | gates 2/7 | radix 1m10s 120/351 (1m14s-1m36s) | failed: x", failed
  part in the warning colour); the row grows by one line per run.
  "Other runs" (g5) = roots whose session is not SHOWN - including a
  closed session while Closed is hidden (a commit outliving its
  session must not vanish). Its title and rows are ALBUM_FASCIA, so
  j/k never select them. colloquia_visus_cursus_textus is the pure,
  tested piece. Plants 7/7 (two re-planted; the selectability test
  first passed by fixture coincidence - c4's number = r4's).
- PHASE 2 IN MAIN 2026-10-16 (ff d2ccd129, after main 58bf676d merged
  in as d2ccd129): fusio_probare 37 gates + frigida green (first try
  stopped: tertia's post-ff installata rewrote ~/.bin/briar+silex mid-
  check - benign, retried); installata 24/24 incl. institutio_mensor
  -> ~/.bin/mensor. Owed: lid-close probe (T8d0), iudicare progress.
- IN MAIN 2026-10-16 (ff 1dc057b6, fusio_probare green + frigida,
  installata 23/23). The 7 hooks now call `~/.bin/praeco hamus`
  (backup ~/.claude/settings.json.bak-praeco-bin-2026-10-16); the ad
  hoc copy is deleted. A praeco change now ships like any house binary:
  merge to main, then `bin/fabrica sanare installata` there.

## V. Hook facts (Claude Code docs, checked 2026-10-15)

- Common stdin fields: session_id, transcript_path, cwd,
  hook_event_name, permission_mode (+ agent_id / agent_type /
  prompt_id when relevant).
- Notification: message, title, notification_type (matchers include
  permission_prompt, idle_prompt, agent_needs_input, agent_completed,
  elicitation_dialog, ...). PreToolUse: tool_name, tool_input,
  tool_use_id. Stop: last_assistant_message, stop_hook_active.
  SessionStart: trigger (startup/resume/clear/compact/fork). SessionEnd:
  reason. StopFailure: error.
- Hooks inherit the Claude process environment (TMUX, TMUX_PANE);
  CLAUDE_PROJECT_DIR is set; CLAUDE_ENV_FILE (SessionStart, Setup,
  CwdChanged, FileChanged) persists KEY=VALUE into later Bash commands.
- Hooks are synchronous by default (30 s timeout); `"async": true` =
  fire and forget.
- Channels: research preview; MCP server with the channel capability;
  `claude --channels plugin:<name>@<marketplace>`; custom channels need
  `--dangerously-load-development-channels`; docs
  code.claude.com/docs/en/channels.md and channels-reference.md.

## VI. AUDIENDA

- Pane ids are per tmux server; T3 ignores `servitor` when matching
  (one default server today).

- Claude Code channels: documented (V); not tried yet (T7).
- Hook payload fields come from the docs (V), not yet from captured
  real payloads - T2's first live run checks them.
