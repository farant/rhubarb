
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
