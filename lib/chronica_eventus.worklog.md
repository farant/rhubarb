# chronica_eventus worklog

## 2026-10-30 - C1: the event scanner

`chronica_eventa_legere` turns page lines into events
(`<event year=... month= day= people= place= type=>text</event>` or
`</>`). Spec: project-specs/chronica-spec.md, plan task 1.

**One buffer, line starts kept.** Events can span lines, so the lines
are joined with '\n' and every position maps back to (line, column)
by binary search over the line starts. Blank lines are real lines here
(their index is the page line) - the CALLER must pass all lines:
`scriba_liber_paginam_legere` drops blank lines and would shift every
line number (C2 adds a reader that keeps them).

**Order of tests at each `<event`:** read attributes to '>' (a '<'
before '>' = broken tag, "non clausum"); then the FIRST of
"</event>", "</>", or the next valid `<event` decides. A tag start is
`<event` + whitespace or '>' (so `<events`, `<Event` are prose). An
unclosed event's range ends where the next `<event` begins (or at the
end of the text) - that range is what C6 colours as an error.

**A bad field spoils the whole event** (Fran approved): month 13, day
without month, day 40, year 0 or non-numeric -> a vitium, not an event
with that field dropped. Unknown attributes are ignored (future
`source=` won't break old notes). Values are raw (no entity decoding).

**Plant lesson.** My first "next tag ignored" plant widened the search
loop bound and stayed GREEN: the check after the loop (`clausura >=
proximus`) guards the same thing, so the bound alone is not the
decision point. The plant that bites replaces where `proximus` is
computed. Also: a plant that leaves `collapsum` unused does not compile
(-Wunused-function) - keep the function called and overwrite after.

Plants (all red, file IDEM after): names not trimmed; year 0 accepted;
`</>` refused; description not collapsed; next tag ignored. Tests: 11
sections (probatio_chronica_eventus).
