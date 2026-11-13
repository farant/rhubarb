# negotia — phase log

Append-only. INTENTIO opens a phase, RELATIO closes it. Chronology is
the re-entry reader's map — never insert, only append.

## NATIVITAS (2026-11-04)

Fran's question (secunda session 134c8417): the library is a garden of
hundreds of threads, each worked a day or two and left for months, and
he works through ~five parallel conversations with no view of what
happened in each. negotia = projects as places: typed records (bones
before fields) with stable identity and revision history in the vicus
volume, and a vicus pane to see and resume them. Spec
`project-specs/negotia.md` (decisions VIII, builder's calls VIII-a),
plan `project-specs/negotia-plan.md` (T1-T6). negotia dogfoods itself:
project `negotia` (01M6MG51K2KHYKSYCZ5SCFYKBZ) in
`~/.rhubarb/paginae-proba.volumen`, this conversation linked.

## FIRST SLICE — RELATIO (2026-11-04): T1-T6 done

| | commit | what |
|---|---|---|
| T1 | 5df7cc6ab | `lib/negotia.c`: projects, features (status + body), every edit = one act = one revision; sees other processes' writes via `volumen_seq_ultimum` |
| T2 | ad184307d | `bin/negotia` CLI (index, creare, ostendere, facultas, mutare, revisiones, involvere) |
| T3 | cda8bd1b4 | `$projects` pane: project list + feature list, mouse-first (hover, single click), 1 s refresh; `negotia_versio` per pane (shared handle) |
| T4 | 07e87631d | feature details: wrapped body + revision history (local time), paginated; clip by characters |
| T5 | ca7cc7c1c | vicus ring per tab, Cmd+H / Cmd+L, saved with the tab; virtual genera (`vicus_aperitorem_addere`) |
| T6 | 6d09c30a4 | conversations in the ring: live tmux pane -> terminal in the project's own tab; else card with Resume (claude --resume inside tmux, new-session -A); directory stored on the link |

Turns taken on the way (details in `lib/negotia.worklog.md`,
`lib/negotia_visus.worklog.md`, `lib/vicus.worklog.md`):
- One `Negotia*` per vicus: `negotia_renovare` signals only its first
  caller, so each pane compares `negotia_versio` with what it last drew.
- Ring position = the ring entry in front, else the stored one (after
  Esc or Cmd+] you continue where you were).
- Fran's live check (T6): the ring jump first reused $sessions' saltus
  and switched tabs; now `saltus_modo(in_tabula)` keeps it in the
  project's tab.
- Resume runs inside tmux so a resumed conversation is jumpable later;
  `-A` reattaches instead of starting a second Claude (two processes on
  one session interleave into one transcript - Claude Code docs).

State: `negotia index` -> `negotia  8 shipped · 4 planned · 1
conversation`. Still planned (spec §VII "later"): vision, complications
(four-part form), activity log (spec Q4), ask-on-a-point notes.

Open on the ledger (park 01M6M6D7AJ): wish 01M6MXK6MW (archive linked
transcripts so they resume forever - `claude --resume <path.jsonl>` is
documented; meanwhile Fran may raise `cleanupPeriodDays`), wish
01M6MXQ4S6 (create a project -> it starts its own conversation).
