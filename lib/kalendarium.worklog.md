# kalendarium worklog

## 2026-10-13 — A2a: month pane structure (agenda-plan A2)

Decisions (Fran 2026-10-13): one genus `calendarium` with a `visus`
field (mensis/dies/annus - views are alternatives, not nested children;
nesting stays for A6's embedded scriba); C prefix `kalendarium_`
because the liturgical library owns `calendarium_`; selection
TRANSIENT (every session opens on today), view mode persistent; weeks
start on Sunday.

Manifest apps/calendarium/organum.stml (embedded by
tools/organa_infigere.sh): binding `documentum` (agenda, from parent)
and port `hodie` declared - both wired by hand (gap 2). One action
`calendarium.agere` for keys and clicks (one writer per value field).
Forced fix after approval: `<porta modus>` admits only `petitio` |
`eventus` (organum.h) - "lectio" made the organ refuse to load (stderr
names it: "valor generi attributi non congruit: modus"); today is
asked for on demand = `petitio`.

`kalendarium_componere`: root "kalendarium" (focusable, the action) >
"titulus" + 42 day cells whose id IS the ISO date (a click reads the
date from the component id - no lookup table). Cell size from the
branch's superficies_* (absent: 280 x 256). `diem_scribere` sets the
writer to `calendarium.agere` and restores the previous one: calls from
outside the dispatcher (argumentum_ponere, mount) would otherwise be
refused silently by the owners. Month steps use `fasti_addere_menses`,
which already clamps (Jan 31 + 1 month = Feb 28).

vicus: `ContextusKalendarii` opens ONE agenda document per vicus volume
(namespace "agenda") lazily and hands it to every calendar pane; the
`hodie` port calls the vicus clock function itself (the source of the
root's tempus_* copies), so tests inject a fixed clock through
`vicus_horologium_ponere`. `$calendarium [YYYY-MM]` registered next to
$pictor/$terminale.

Tests: probatio_kalendarium (mount, grid, keys incl. the clamp, click,
argument, persistence: visus returns, selection does not) and
probatio_vicus_kalendarium (two panes, argument, clock port, shared
document). Plants: Monday-first grid, unshared document, dead clock
port - all red. ludus_tessera's hand list needed "agenda" and
"kalendarium" (vicus_applicatio links them).
