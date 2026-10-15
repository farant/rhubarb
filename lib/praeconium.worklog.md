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
