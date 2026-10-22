
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
