# colloquia_visus worklog

## 2026-10-15 - T5 sessions view (colloquia-plan)

- Incremental fold in colloquia: `ColloquiaStatus` (per-session Status +
  pane -> last session) with create / apply / read;
  `colloquia_ex_eventibus` is now create + apply + read. Strings are
  COPIED into the state's arena (events come from a scratch arena).
  The test reads each chunk into a temporary arena, SCRIBBLES over it
  ('Z') and destroys it before reading the state back - freed memory
  usually keeps its bytes, so without the scribble an uncopied string
  still compares equal (plant P1 needed a session whose LAST event is a
  prompt; otherwise later events overwrite the uncopied line first).
- Shared source (ColloquiorumFons): log tail at start, then
  praeconium_legere_ab; titles cached per session from the transcript,
  directory-name fallback retried only when the session has a newer
  event; pane liveness every 5 s, only while a sessions pane is visible
  (pulsed from the pane's pulsare, vivit_in_fundo FALSE).
- BUG found by the live test: liveness asked only the DEFAULT tmux
  server - a session on another socket (the private test server, or any
  `tmux -L x`) had its pane "%1" looked up on the wrong server, was
  marked CLAUSUM and hidden. (T3 note "pane ids are per server" was
  exactly this.) Now: one list-panes per distinct `servitor` the
  sessions report; a session is judged only against its own server;
  sessions outside tmux are not judged. The multiplexor in the source is
  only the template (binary). The old behaviour also made the test read
  the user's real default server.
- Hand-built Eventus in the vicus live test never reached the pane's
  action (click nor Enter); manus_ludus (premere "#r1", clavem) does -
  drive panes through manus_ludus, as the other vicus tests do.
- Working dot is the text colour: accent and success are the same leaf
  green in this theme (headless render showed Working/Done identical).
- Jump shared with notifications: `saltus(ca, pane, socket)` in
  vicus_applicatio (T4c chain), both portae call it.
- Fran's look 2026-10-15: everything worked (groups, colours, j/k skip headers, c toggles Closed, Enter jumps to a real tmux pane via admonere.sh hic).
