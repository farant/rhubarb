# membrum__prompta worklog

## 2026-10-14 - U3: the prompt line, extracted from the calendar

nomenclator-plan U3, header approved by Fran. Moved from kalendarium.c
(A3b prompt, V2a look): the three island fields of the HOST's branch
(prompta = which prompt or "nulla"; prompta_textus escaped with
scriba_effugere; prompta_cursor), written in one act with the host's
writer name (restored after) - the dimissio pattern, so the library
touches the island only through the host's branch. vim insert mode on a
one-line TabulaCharacterum; Enter returns the text with trailing spaces
trimmed and leaves the line OPEN (the host commits and closes only if
the commit succeeded - a refused value keeps the line, as before); Esc
closes; every other key is consumed. Drawing takes a box in the
caller's space, so the calendar (own-space component figure) and the
nomenclator (root-space figure) can both use it.

Bug found by the new test: Home did nothing - vim's insert mode does
not handle VIM_CLAVIS_HOME/END here (vim.c handles them elsewhere), so
the calendar's prompt silently ignored Home all along (End only seemed
to work because the cursor was usually already at the end). The line
now moves the cursor itself for Home/End.

Traps: `longus` is the latina.h macro for `long` (test variable,
renamed prolixus); BSD sed has no \b, so a `sed 's/\blongus\b/...'`
rename silently did nothing - used python re.

Tests: probatio_membrum__prompta (aperire, claves, reditus, textus,
figura). Plants: Enter not trimming; Esc not closing; length cap
ignored; Home ignored; no escaping; label not dim - all red.
