
## 2026-10-13 - $question<>...</> (Fran)

Registered verb "question" (non-consuming), ctx = ContextusQuaestionis
whose port the window app sets (vicus_applicatio_quaestionis_portam_
ponere; NIHIL = "no API port (window app only)" - terminal twin and
tests without a port). Budget = (free trailing lines x page width -
len("<answer>") - len("</>")) x 85 / 100, from Iussum.lineae_liberae /
latitudo_tabulae. First call defers ("question: asking
claude-opus-5-5..."), second call asks through the port, lays the
answer out (responsum_disponere: "<answer>" + word wrap at the page
width, '\n' = new line, runs of '\n' = one blank line, over-long words
split, "</>" after the last word) and fits it into the free lines
(responsum_aptare: whole text, else the longest prefix ending a
sentence, else a word cut with "..."). Effect: head -> "<question
answered>", text "\n" + lines, status "question: <usage>". Tests:
probatio_vicus_quaestio (fake port; 7 sections), two plants red
(truncation off, deferral off).

## 2026-10-13 - answer layout: margins, hanging indents, verbatim code

responsum_disponere now reads quaestio's logical lines: two columns of
margin on each side (Fran; dropped on pages narrower than 24), text
reflows to the inner width with a hanging indent after its marker
("- ", "1. ", "> ", "# "), '\t'-lines are code - never reflowed, only
hard-broken at the inner width so nothing is lost - and blank lines
are kept. "<answer>" opens the first line at the margin (a code first
line goes below it); "</>" follows the last word or code line.
Truncation candidates now include logical-line ends as well as
sentence ends. probatio_vicus_quaestio "forma" checks heading, blank,
paragraph within margins, hanging indent, code indentation; planted
(code reflowed) -> red.

## 2026-10-14 — $question streams (quaestio-rivus T8)

The verb no longer blocks: stage II starts the port and returns an
`IussumOpus` (T7) whose pulse lays out the PLANNED text so far with the
existing `responsum_disponere` / `responsum_aptare`
(`vicus_quaestio_disponere`, public so the test can compare the
streamed page with the layout of the final text - section `aequale`).
- Plan ruling R9: the PORT plans (`quaestio_textum_planare` in
  `lib/vicus_quaestio_porta.c`), not this file - otherwise
  vicus_applicatio would include quaestio -> vates -> http/TLS and the
  terminal twin + ludus_tessera hand list would drag the API stack in.
  Same planning function as the non-streamed path, so equality holds.
- R8: prompt lines = free lines - 1 (the `<answer>` line), width = page
  width - 2*QUAESTIO_MARGO (or the page width under 20).
- Page full: when the laid-out text needs more than the free lines the
  port is aborted and the existing sentence-end truncation finishes it
  (`question: page full`). Esc: `[cancelled]` paragraph; error after
  text: `[error: <reason>]` paragraph, both committed as answered;
  error before any text: IRRITUM (page and head untouched).
- `OpusQuaestionis` lives in its own dynamic piscina, destroyed at the
  end; returned texts are built in the pulse piscina first.
- Fran's live check (2026-10-14, ../rhubarb-quarta ./apps/vicus/vicus.sh):
  "it works and the text appears in steady pieces" - spec AUDIENDA 1
  (SecureTransport hands partial TLS records to the reader promptly)
  answered yes.

## 2026-10-14 - $question head is now "<answered>"

Fran's choice after two split-question reports: the answered head was
`<question answered>` (XIX cells) in place of `$question<>` (XI) - the
+VIII overflowed long first lines and, with the answer position computed
before the move, split the question. `<answered>` (X) is shorter than
the command, so the line can never grow. Nothing parses the head; the
old marker stays in pages already answered. Placement itself is now
robust in scriba_actiones (`clausuram_quaerere`) - see its worklog.

## 2026-10-19 — semitae R3: shared manifest resolver, $via

`organa_vici` (ContextusOrganorum): embedded manifests via
organum_legere_genus, cached per genus; given to vicus with
`vicus_organa_ponere`. `$via(uri)` checks before asking vicus: genus
known, genus has routable fields ("<genus>: semitas nondum accipit"),
`semita_ex_uri` valid (its error verbatim) - then
`vicus_acervo_mutare(genus, uri)`; the pane's `argumentum_ponere`
applies it. The saints pane sets `f->ramus` (history).

## 2026-10-22 — D5 S3b: the search pane in vicus (genus "indagatio")

- **One registry, one agenda, one contacts document.** The registry the
  calendar built lazily (D4c, `dies` only) now also holds the four
  searchable kinds (`indagatio_genera_registrare` before `parare`), and
  the search index is created over it on the first search pane. The
  agenda document stays on ContextusKalendarii; the contacts document
  stays on ContextusNomenclatoris, reached from the calendar context
  (`ck->nomenclatoris`) - `personas_aperire` replaces the three inline
  lazy opens. Why it matters: per-rem kinds read the DOCUMENT, so a
  second document over the same volume would not see an item created
  in the calendar or a contact created in the contacts pane until it
  re-read. Test: probatio_vicus_indagatio "communia" (both plants red).
- `$via`'s checks moved into `semitam_aperire` (messages unchanged);
  the search pane's Enter gate uses it. Stopgap until S3c:
  `scriba/<name>` is not a route, so the gate opens it like
  `$scriba(name)` (vicus_acervo_aperire "scriba", name).
- `$search` = vicus_advocare (prompt open with the current q);
  `$search(words)` = vicus_acervo_mutare with the words (cognomen
  turns them into `indagatio?q=`); several arguments (commas in the
  words) are joined with ", ".
- Cycle: admonitiones, colloquia, calendarium "agenda", indagatio.
  Cmd+Shift+F: vicus_brevitatem_addere('f', MOD_SHIFT, "indagatio").
- Testing note: a click into a scriba page that is NOT focused only
  focuses (its first-click action runs a command under the cursor, it
  does not place the cursor), so typing after it lands at the old
  cursor. probatio_vicus_indagatio writes all commands up front (one
  per line, Enter between) as probatio_vicus_acervus does, then clicks
  each.
- Plants (silva.planta, filter vicus_indagatio), all red at their own
  assertions: 15 - modifiers not exact (Cmd+F fires), routing check
  blind (ctx dropped in destinare), dispenser drops the strategy ctx,
  advocatum not called, advocare flag not set, transient ignored in
  the mutare branch / after a new mount / not set by the pane, cycle
  without indagatio, common-notebook gate missing, Esc not returning,
  contacts or agenda not shared (second document), `$search` without
  argument not advocating, wrong shortcut key. The two routing plants
  are red ONLY at the terminal-focus step: with scriba focused the
  chord bubbles to the root handler anyway.

## 2026-10-22 — D5 S3b, after Fran's look: notebook hits open LEFT

Fran: a common-notebook hit is the main journal, so it belongs in the
left pane, not stacked on the right. The gate now switches the active
tab's left pane (if it is a mounted scriba) to the page with the new
`scriba_paginam_ostendere` and focuses it; any other left pane falls
back to `$scriba(name)` in the stack. Focus moves inside event
handling - fine, a click does the same. Plants red: focus left
missing, left never chosen, page ignored. `m->doc` not reassigned was
GREEN in vicus (focusing calls scriba_reficere, which resyncs m->doc),
so the direct test lives in probatio_montatio (page doc, saved view,
unknown name refused) - red there.

## 2026-10-22 - sonus S4 step 4b: notification sounds, $sound mutes

vicus_applicatio knows sound only through a PORT (`include/vicus_sonus.h`,
types only: pulsare(genus) / agere / tacere), same reasoning as the
$question port (ruling R9): calling tintinnabulum directly would put
mixtura/cantus/cantuale/sonitus (~7 libs) into ludus_tessera's hand list
and CoreAudio into every vicus test binary, and grow this file's header
closure (127 of CXCII). The real port is `lib/vicus_sonus_porta.c`, linked
IN-PROCESS by apps/vicus/vicus.c and vicus_terminalis.c (not a sidecar).
Fran (10-22): sound effects inside vicus later are fine - the port grows
(e.g. `sonare(praeparatum)`), the shape stays.

- `ContextusSoni` {porta, posita, mutus_prior}; `ContextusAdmonitionum.sonus`
  points at it. `systema_admonitionum`, when `admonitiones_sequi` > 0,
  pulses `admonitio(index, 0)->genus` - newest only, so N events in one
  pulse ring once (tintinnabulum then queues one, latest wins).
- system "sonus" (after "terminalia"): reads both mutes every pulse;
  tacere ONLY on the transition to muted (any writer, not just the
  button); agere every pulse, muted or not (the lazy device must still
  close after its quiet period).
- `$sound` / `$sound(notifications)`: non-consuming iussum, ctx = Vicus;
  flips `sonus_mutus` / `sonus_admonitionum_mutus` (persistent, scriptor
  `sonus.agere`, apps/vicus/organum.stml), status line "sound off/on",
  "notification sounds off/on"; other argument -> "sound: unknown
  argument (notifications): x". Works with no port (fields persist).
- organum.stml is EMBEDDED: after editing it run `./tools/organa_infigere.sh`
  (lib/organa_infixa.c, GENERATUM) - before that every write to the new
  fields was refused by the domini and the suite read like logic bugs.
- -fumus never sets the port: headless screenshot tests must not play a
  jingle when a real notification lands in the live praeconium log.
- An empty-genus guard in `sonum_admonitionis` survived its plant: no
  path produces one (admonitio_ex_eventu always sets a genus) and the
  port already answers FALSUM for a genus without a jingle - removed.

Suite `probatio_vicus_sonus` (7 sections, 123 asserts, fake port that
counts calls; TMUX_TMPDIR in the credo area so systema 'terminalia'
never sees Fran's server). Plants 13/13 red (12 C via silva.planta,
plus organum `vita="transiens"` by hand -> persistens red).

## 2026-10-22 — book lookup for scriba panes (comites P2)

ContextusScribae keeps opened companion books in a TabulaDispersa
(prefix -> ScribaLiber*), opened once with
`scriba_liber_aperire_in_spatio` at the common book's page size; ""
is the common book. Every scriba pane gets the lookup at mount. Not
plantable: dropping the cache (a reopened book reads the same volume
state; only cost and object identity change, not behaviour).

## 2026-10-22 — terminal locations (comites P4)

Terminal panes are registered with `TerminaleInVico` (TerminaleApplicatio
FIRST, so every existing cast stays valid) holding the pane id, the tmux
server from the argument ("tmux <server> <session>", "-" = default) and
a cached location. Location: the tmux client on the pane's tty -> its
session -> the active pane of the session's active window ->
`terminale/tmux/<session>/<window index>`, title "<session> / <window
name>"; no client -> `terminale/<pane id>`, "Terminal". Session + INDEX
(Fran): window ids reset with the tmux server, names auto-rename.
Asking tmux starts processes, so the answer is cached 1 s (the follow
check runs every pulse); results live in fixed buffers, the query
arena is created and destroyed per inspection. The one harmful lag
(switch tmux window, type into the companion at once -> previous
window's notebook) is closed by `facies.locum_renovare`: vicus calls
it on the right front before Cmd+T reads the location and when focus
moves into the left pane; the terminal drops its cache. Plants red:
no refresh on focus-left, none on Cmd+T, refresh a no-op. Test: probatio_vicus_comites_terminale against a
private tmux server (build/pmx, own TMUX_TMPDIR, as probatio_vicus_saltus).
Plants red: cache forever, client not matched, active window ignored,
argument's server ignored, hooks unset. Not plantable: removing the cache
(same answers, only cost).
