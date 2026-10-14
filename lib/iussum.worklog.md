# iussum worklog

## 2026-10-08 - S3a: recognition only, known verbs only

Born for vicus-latera S3 (acme commands in scriba pages). Pure reader
over a TabulaCharacterum; no registry here - the host answers "is this
verb known?" through `IussumNotum`, an idea taken from prunifex (only
registered commands highlight or respond), which removes the need for
an escape syntax: `$5.00` and a half-typed `$foo(` stay prose.

Rules worth remembering: `$` must start the line or follow a
non-word character (`a$b`, `x_$dies` are not commands); arguments end
at the FIRST `)` on the same line (no nesting by design); an unclosed
`(` makes the whole token not-a-command rather than a command without
arguments, so half a command never runs. Empty cells are `'\0'` in
scriba pages - they count as spaces when trimming.

Gotcha: `tabula_cellula(t, l, c)` with s32 coordinates trips
-Wsign-conversion (i32 width is unsigned) - the lib goes through one
`cellula()` helper with explicit casts. And the installed `scribe`
judges against main's repo, so in a worktree it writes but answers
"via extra repositorium" (exit 4) - judge with ./silva/examen.sh.

## 2026-10-08 - S3b-1: the registry

Opaque `IussumRegistrum` over a Xar (a handful of verbs - linear
lookup is fine). A verb must have the same shape as in text
(`[a-z][a-z0-9_]*`), otherwise it could never be clicked; registering
an existing verb REPLACES it (plant: appending instead left the old
entry winning the lookup). `iussum_currere` clears the effect before
calling, so a verb never sees a stale error. Gotcha: `registrum` is a
latina.h macro (`register`) - the examen rejected it as a parameter
name; the `IussumNotum`-shaped function takes `ctx`.

## 2026-10-09 - '-' inside verb names (S3e, Fran)

For `$pictor-next` / `$pictor-prev`. Grammar is now
`[a-z][a-z0-9_-]*` with no trailing '-': `legere` scans '-' with the
other verb characters and then backs off trailing dashes, so "$dies-"
stays $dies followed by prose '-' (and "$dies--" too). Registration
applies the same rule (rejects "a-", "-a"; accepts "a-b"). The '$'
precondition (not after a verbal character) still uses
verbi_character, which does NOT include '-': "a-$b" is a command.

## 2026-10-13 - body form '$verb<>...</>' (Fran)

Fran's idea for $question (scriba asks Anthropic via vates): a single
multi-line argument in STML's own closer syntax. Rules (header):
'<>' right after the verb, body to the FIRST '</>' on this or a later
line, no nesting, unclosed = prose; body = one argument, lines
verbatim minus trailing spaces, '\n'-joined, outer whitespace trimmed;
commas and ')' are plain text; only the HEAD ('$verb<>') is the click
target, so clicking into the body edits it instead of firing it.

- '$' inside a body is not a command: legere() asks intra_corpus(),
  which walks the page from (0,0) jumping over every body it meets -
  so a '$' inside a body can never open a body itself (no nesting by
  construction). The walk parses in PROBE mode (plenum FALSUM: no
  body copy, no intra check -> no recursion). Cost: O(page cells) per
  '$' probed; pages are ~60 x 40, the figure probes per line.
- Iussum gained corpus / linea_clausurae / post_clausuram; finis stays
  the end of the clickable part (head), so iussum_ad_locum and the
  figure's per-line colouring loops are unchanged: only the head is
  coloured, the body is plain text.
- An empty body ('$verb<></>') gives zero arguments, like '()'.
- '$verb(...)<>...</>' is the simple form; the '<>' after it is prose.

## 2026-10-14 — IussumOpus (quaestio-rivus T7)

`IussumEffectus.opus` (NIHIL = synchronous, zeroed by iussum_currere's
memset) + `IussumOpus {datum, pulsare, abrumpere}` with statuses
CURRIT / FINITUM / IRRITUM (IRRITUM = ended with nothing to insert,
plan ruling R7). The host (scriba) owns lock/snapshot/commit; the verb
owns its text and layout. First user: $question (T8).
