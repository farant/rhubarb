# frigida — worklog

## 2026-09-21 — born: the command-line write path gains status, mutation and link

**Why it exists.** A commit hook and pythonica cannot speak MCP. "The
commit closes the task" and "the gate writes measured progress" (park
01KY57P2WD) need a write path from a shell. The old cold tool could
only add a note or create an item.

**One door.** The new verbs go through the tabularium MACHINE
(`tabularium_tractare`, a `gerere` call built with the JSON builder),
not through `gesta_scribere`. So the command line gets the same rules
(canonical link verbs), the same projections (`tabula.md`, entities)
and the same refusals as MCP — there is no way around the rules
because there is no second door. The test proves which door was used:
after `-status`, `tabula.md` shows the new status, which a direct
engine write would never touch.

**Two doors remain in the TOOL, on purpose.** The legacy forms (note,
`-crea`) still write straight through the engine: the emergency door
stays dumb and robust for the day the machine itself is sick. They
also still do no JSON escaping (the NB in `nota_frigida.c`); the new
verbs escape correctly because they use the builder — pinned with a
value containing a double quote.

**Both refusal laws from birth (decree 01M32X21NR).**
- *Never drip, ACROSS LAYERS.* For a link the machine's read-only
  pre-check (`tabularium_nexum_praeiudicare`, new, public) always
  runs, so command-line causes (a superfluous operand) and machine
  causes (a synonym verb) arrive in ONE response. To avoid reporting
  one fault twice, the machine owns "verbum deest / alterum deest"
  for links; the command line reports only what the machine cannot
  see (options, unknown flag, extra operands, missing `res`).
- *A refusal teaches.* `IMPERIUM VALIDUM:` is the machine's corrected
  call turned into a paste-ready command line — ends already swapped,
  verb already canonical, the caller's own `-actor`/`-origo` kept.

**The server lost a drip in passing.** `_tab_gerere` used to return on
an unresolvable `res` BEFORE the link pre-check, so "unknown item +
bad verb" cost two round trips. The pre-check now resolves `res`
itself and reports it among the other causes (ambiguous titles list
their candidates, via `_candidatos_appendere`, factored out of the
ambiguity response).

**THE COLD PATH WAS BROKEN, AND NOBODY KNEW.** `./gesta/frigida.sh`
failed to LINK: its hand-kept object list did not know that
`tabularium.c` started using `processus` on 2026-07-29 (renovare).
Two months dead, advertised in the doctrine the whole time, because
no gate ever ran it (open wish 01KYPZ4T6J said the list had lied
once already). Fixed by deriving the list: the aedilis-generated
`RADIX_FONTES` the server launcher already reads, plus every file in
`gesta/fontes/`.

**The birth gate paid for itself in its first run.**
`gesta/frigida_fumus.sh` (gate `frigida`) builds and runs the REAL
launcher — the C test cannot see launcher rot. First run: red. A
mistyped flag (`-statum`) was not known to the new machine, fell into
the legacy form and was read as an ITEM TITLE: "res ignota -statum".
Now every flag except `-crea` goes to the machine and gets a proper
refusal with the valid forms. The gate writes nothing to the live
ledger and ASSERTS that (line count before and after).

**Plants.** Four in C, four different assertions: (A) machine
pre-check skipped for links; (B) the valid command takes `alterum`
from the wrong field; (C) superfluous operands not reported; (D)
server: unknown `res` no longer among the causes. One by hand on the
launcher (never `Editio` on a `.sh`): the short hand list put back →
five smoke assertions red. That plant also exposed a lying-green line
in my own gate: "synonym → exit 1" stayed `ok` with the launcher
broken, because a failed link ALSO exits 1. It now requires the
refusal text as well. To make a launcher plant fire you must delete
`gesta/build/nota_frigida` first — the launcher relinks only when an
object is newer than the binary.

**Two weak assertions caught before shipping, same cause both times:**
checking the annals for a bare verb spelling passes or fails for the
wrong reason, because the SEED writes every synonym spelling into the
log. Check `"verbum":"…"` (a link's datum), not the bare word.

**Not done here:** wiring `silva.commissio` to close a task; legal
next statuses in a `-status` refusal (needs a public accessor for an
item's genus machine); the status and mutation paths inside the
machine still report their first failure only.

Exit codes: 0 written · 1 refused, nothing written · 2 usage, nothing
attempted.

## 2026-09-21 — `-res`: a read verb, because a pre-check needs one

`silva.commissio(opus=ID)` must check the task BEFORE the gates run,
and pythonica cannot speak MCP. `-res <res>` returns the compact view
through the same machine and writes nothing (pinned: the annals are
byte-identical before and after). The first line, `Title (genus,
status)`, is now a contract with that reader. `FrigidaForma` gained an
`instrumentum` field; a form with `actus == NIHIL` is a read and
carries no actor or origin.

## 2026-09-22 — `-vis`, and legal statuses in the paste-ready command

`-status` now runs the machine's read-only status pre-check, so an
illegal transition is refused with the legal successors in the valid
command (`-status "<id>" <tractum | clausum>`) and a line on how to
force. `-vis` is a bare flag before the verb; it is forwarded to the
machine as `vis: "verum"`. A placeholder the machine already shaped
(`<a | b>`) is now passed through verbatim instead of being replaced
by `<novus>`. The item in the valid command is the RESOLVED id, not
the title the caller typed — pinned. See tabularium.worklog.md same
date.

## 2026-09-22 — `-mappa`: the first READ form with no operand

Fran wanted something to run by hand that prints the region tree like
`tree`, names only. The tool grew `forma: "nomina"`; the cold path
grew `-mappa`, which sends exactly that through the one door.

The first zero-operand form exposed an assumption: the machine was
opened only when an operand had been given (`data > 0`), because
every earlier form needed a res to pre-judge. With no operand the
send path would have handed a NIHIL tabularium to `tabularium_tractare`.
The send block now opens the world itself when nothing opened it
before. The read-form branch of `_gerere_mittere` chooses the argument
by instrument: `breviter` for `res`, `forma: nomina` for `mappa`.

Suite: region + sub-region in the fixture; `-mappa` exits 0, prints
both with the child indented, annals byte-identical; `-mappa extra`
is usage (exit 2). Smoke gate: the live launcher prints Fran's roots.

## 2026-09-22 — `-mappa-plena`

Second zero-operand read form. The read branch of `_gerere_mittere`
now chooses the map's `forma` by the flag, `nomina` or `plena`. The
fixture gained a vision inside the region so the two forms are told
apart by content, not by exit code.
