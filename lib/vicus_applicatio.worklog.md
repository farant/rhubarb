
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

## 2026-10-22 - praeco cantus step B: vicus plays Claude-sent jingles

Transport = the praeconium log vicus already tails. `praeco cantus` will
append `{"eventus":"cantus","genus":"intactus"|"","nuntius":"<MML>"}`.
No second reader: `AdmonitionumSequax.eventus` now keeps ALL events the
last `admonitiones_sequi` read (scheda, valid until the next pulse), and
`systema_admonitionum` takes the newest `cantus` from it
(`cantum_recentissimum`, scanning from the end). The sequax starts at the
log's tail, so jingles written before vicus opened never replay.

- Both mutes silence jingles (Fran 10-22: Claude's jingle = Claude getting
  your attention, same channel as notifications).
- Notification + jingle in one pulse: `sonum_pulsus` calls them in TIME
  order, later last - tintinnabulum's queue makes the last call win.
- `VicusPortaSoni.canere` is required: a port without it is refused as a
  whole (section porta_dimidia; the plant segfaulted = NULL call).
- The real port forwards to `tintinnabulum_canere(..., NIHIL)`: vicus does
  not judge the text - praeco does, before writing (step C).

Plants 9/9 red (8 vicus_sonus + sequax.eventus via admonitiones_visus).

## 2026-10-22 - S5c: aetherophonum pane kind, $theremin

Pane kind "aetherophonum" registered with ctx = ContextusSoni (which now
also holds the Vicus and StatusSystemata). Wrappers aetheri_incipere /
_ponere / _dimittere: no port or MASTER mute -> 0 / nothing; the
notification mute is ignored on purpose. The port must now provide the
three live-voice calls or it is refused as a whole (porta_dimidia).
ludus_tessera's hand list gained "aetherophonum" (vicus_applicatio links
the pane; the pane links nothing sound-related).
