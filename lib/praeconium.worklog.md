# praeconium worklog

## 2026-10-15 - T2: the session event log (colloquia-plan)

Header approved by Fran. One JSON object per line in
~/.rhubarb/colloquia/eventa.jsonl (RHUBARB_COLLOQUIA overrides the
directory), shared by all trees. Writes go through
`filum_appendere_firmiter` (one write() with O_APPEND, then fsync): the
`concurrentes` section forks four writers x 50 events and reads back all
200 lines intact. Long text is cut at 200 bytes, never inside a UTF-8
character (if the first excluded byte is a continuation byte, the cut
moves back to that character's lead byte).

Field mapping follows the Claude Code hooks reference (checked
2026-10-15, plan section V): Notification -> genus = notification_type,
nuntius = message; PreToolUse / PostToolUse(Failure) / PermissionRequest
-> genus = tool_name, nuntius = tool_input description, else command,
file_path, pattern, url; UserPromptSubmit -> prompt; Stop / SubagentStop
-> last_assistant_message; StopFailure -> error; SessionStart -> genus =
trigger (or older `source`); SessionEnd -> reason. TMUX is
"socket,pid,session" - only the socket is kept (`servitor`).

Tail read: no tail API in filum, and touching filum.h would owe ~29
gates, so `lectiones_fopen` (the examen-approved channel) + fseek/fread.
A first version skipped the first line whenever the read started
mid-file; that dropped a COMPLETE event when the cut landed exactly on
a line start, and a cut line never parses as a JSON object anyway - the
skip was removed and parsing decides.

`tools/praeco.c` is thin (hamus / status / cauda). `praeco hamus`
always exits 0 (a hook must never break a session); malformed input
writes nothing. Time via gettimeofday: the only house ms clock is in
mensura (sqlite), too heavy for a binary that runs on every hook.
Build: tools/praeco_struere.sh (copy of clonare's, no install);
aedificatio.stml gains `praeco` (bin/praeco) and `institutio_praeco`
(~/.bin/praeco), both in `installata` - installed only when main runs
`bin/fabrica sanare installata` after the merge.

Plants red: UTF-8 cut, notification genus key, TMUX socket split,
directory not created, tool_input key order, RHUBARB_COLLOQUIA ignored.
Examen caught a parameter named `nomen` (latina.h typedef) and a const
cast-away in a helper during writing.

## 2026-10-27 — comites P8: tmux hooks (`praeco tmux`, `praeco tmux instituere`)

New event `tmux` (servitor = #{socket_path}, panellus = #{pane_id})
written by `praeco tmux <servitor> <panellus>`, which tmux hooks call
on session-window-changed, client-session-changed and
window-pane-changed. Checked on a private server first: all three
fire; `set-hook -g window-pane-changed` lands as a window hook (shows
under `show-hooks -gw`) and still fires; sourcing the same lines again
REPLACES the hook ([0]), never duplicates. `#{pane_id}` is the pane
that is active AFTER the change - for a window switch that is the new
window's active pane, which is why the live test checks the window
hook and the pane hook with separate commands (a combined
select-window + select-pane let one hook's line satisfy the other
step; the first plant showed it).

Pure helpers here: `praeconium_tmux_configuratio` (hook file text,
absolute praeco path - a tmux server started with a short PATH must
still find it; a path with ' or " gives "" since the quoting would
break) and `praeconium_tmux_conf` (marked block "# praeco tmux:
initium/finis" with `source-file -q "<file>"`, replaced in place or
appended, rest untouched). The hooks live in praeco's OWN file
(<colloquia dir>/tmux.conf) rather than in ~/.tmux.conf directly:
applying them live means sourcing a file, and re-sourcing Fran's
~/.tmux.conf would repeat its `set -as` appends every time.

`instituere` (tools/praeco.c): own path via realpath (argv[0], or a
PATH search when it has no '/'); ~/.tmux.conf written through a temp
file + rename, symlinks resolved first (the link's target changes,
not the link), one-time copy to `.ante-praeco`; then source-file into
$TMUX's server (else the default). Exit 0 always for `praeco tmux`
(run-shell would print a failure in tmux's status line). Sandboxed
check (fake HOME, RHUBARB_COLLOQUIA, private server via $TMUX): block
added once ("sectio iam praesens" on the second run), backup intact,
a window switch wrote a `tmux` event.

## 2026-11-05 — gabc part 2 P8: `antiphonale` event

`praeconium_antiphonale_parare(via, e, piscina)`: eventus "antiphonale",
nuntius = realpath(via) (relative to the caller's cwd); a missing file
returns FALSE and leaves the event untouched, so `praeco antiphonale`
exits 1 without writing. vicus (admonitiones system) takes the newest
such event of the pulse and calls vicus_acervo_mutare. The follower
only hands over events written after it opened the log, so the startup
tail never reaches it; vicus still ignores events stamped before its own
start (a re-read rotated log, a writer with a clock behind). realpath
added to silva/fontes/systema_posix.h (examen had flagged praeco.c's
three uses as implicit declarations too).
