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

## IV. Status

- T1 DONE 2026-10-15: `multiplexor` (lib/multiplexor.c) - panes and
  clients parsed from our TAB `-F` format, has-session, select
  window+pane, switch-client, attach vector; exact `=name` targets;
  `-S` for socket paths (tests run a private server in $TMPDIR).
- T2 next: `praeco` + event log (check hook payload fields first).

## V. AUDIENDA

- Claude Code channels: mechanism and availability not verified (T7).
- Hook event set and payload fields for PreToolUse / UserPromptSubmit
  as of the installed Claude Code version: check before T2 fixes the
  log format.
