# colloquia worklog

## 2026-10-15 - T3: sessions from the event log (colloquia-plan)

Header approved by Fran. `colloquia_ex_eventibus` replays the praeconium
log per session in write order (state machine in the header comment)
and sorts most recent first. Each session also keeps (internally) the
time of its last prompt and of its last explicit `praeco status`, plus
both candidate lines; the shown line is the explicit one while it is
newer than the last prompt, else the derived one ("Bash: Run calendar
suites", the needs-you message, the FIRST line of the last reply).

`praeco status` carries no session id, only TMUX_PANE: a pane -> last
session map is updated by every event that has a session, so a status
line goes to the session that last reported from that pane at that
moment. The fixture reuses pane %1 after session a ends: c's status
must land on c (plant C2 red).

Titles: custom-title and ai-title records RECUR through a transcript
(this session: 3385 / 4601, seven of each in the last 256 KB), so the
last 256 KB is read (lectiones_fopen + fseek) and scanned from the end:
the first custom-title wins, else the newest ai-title, else the cwd's
last component. Checked once on this session's real 220 MB transcript:
"secunda". Fixture gotcha: Python's json.dumps writes `"type": "..."`
with a space; real transcripts are compact and the search matches the
compact form - fixtures regenerated with separators=(',',':').

Pane liveness uses multiplexor's pure parser in tests (no tmux);
`colloquia_legere` applies live panes only when tmux answers (tmux
absent = statuses untouched, not all CLOSED).

AUDIENDA: pane ids are per tmux SERVER; matching ignores `servitor`
(one default server today). A session on a second server would be
matched against the default server's panes.

Plants red: explicit-vs-prompt comparison, pane map not updated, Stop
first line, idle_prompt ignored, custom-title ignored, dead pane not
closed, sort reversed, directory fallback.

## 2026-10-27 - comites P7.2: colloquium_panelli

Which Claude session is in a tmux pane, for the companion key
`terminale/claude/<session id>` (spec project-specs/comites.md §XI).
Pure over the list colloquia_status_legere gives (most recent first):
the first session that is not CLAUSUM with the same pane id and the
same server. After `/clear` the old session ends (SessionEnd ->
CLAUSUM) and the new one is more recent, so the pane maps to the new
id - Fran: a new conversation gets a new companion. Server
equivalence: "-", empty, and any socket path whose last component is
"default" all mean the default server (events carry the full socket
path, a terminal's restore argument may carry "-").
Trap: the local flag was first called `ordinarius` - a latina macro
(`default`); clang's "expected ':' after 'default'" is the tell.
Plants (silva.planta): drop the CLAUSUM check -> red on %6; ignore
the server -> red on the unknown server. Both restored byte-identical.
