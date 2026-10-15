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

### Phase 2 (Fran, 2026-10-15): gate and test progress dashboard

| Phase | Content |
|---|---|
| **T8** | Live runs from mensor's journal: open runs (started, not finished) with their nested children (fabrica actions, suites, credo sections) and status; estimate from history (median of the last N runs of the same gate on this machine); "stage x of y" = children done vs the last run's count. |
| **T9** | Run -> Claude session: join on cwd/worktree and the hook log (PreToolUse carries the Bash command and TMUX_PANE); fallback: `praeco` exports MENSOR_SESSIO into the session via CLAUDE_ENV_FILE at SessionStart. |
| **T10** | vicus dashboard: per session, running gates with elapsed / estimated time, stage x/y, failures as they happen; also runs started outside any session (no link). Beside or inside the T5 sessions view. |

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
