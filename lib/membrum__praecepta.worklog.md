# membrum__praecepta worklog

## 2026-10-14 - U2: the command table, extracted from the calendar

nomenclator-plan U2, header approved by Fran. Name: `praeceptum`
("order, command") - `mandatum` is the drawing-command library,
`iussum` the in-text `$verbs`, `imperium` the vitrea remote control,
`registrum_commandi` the older page-based $-tag registry; prefix
`praecepta_` checked free before proposing (lesson from `catalogus`).

Moved verbatim from kalendarium.c: the key forms ("x" = produced
character without Ctrl/Cmd - Cmd chords also arrive as keys; "Enter",
"Esc", "C-r" = Ctrl + rune) and the table check (fields set, unique
title, a key once per view; also refuses visus 0). Kept host-side on
purpose: what a requirement bit means (PraeceptaRequisita predicate on
the host's context) and the calendar's policy that an unavailable key
is swallowed except Esc. Key handling is split (praecepta_ad_clavem,
then praecepta_status) so the host builds its context only when a key
matches.

Tests: probatio_membrum__praecepta (verificatio, claves, ad_clavem,
status, invocatio). Plants: Cmd not excluded for letters; a key twice
per view allowed; requirements ignored; view ignored when matching a
key; invoke without the status check - all red (the last two needed
compiling variants: dropping the check left a parameter unused).
Lint caught `invocationem` (not in the dictionary) - renamed
`_probare_invocare`.

## 2026-10-22 — "Up" / "Down" key forms (D5 S3b)

Arrow keys as named forms (no Shift/Ctrl/Alt/Cmd). Matcher bug fixed
on the way: forms longer than one character that are not named
("Tab", typos) fell through to the produced-character path and matched
their FIRST letter. Now only single-character forms take that path;
anything else matches nothing. Scanned every Praeceptum table: no
table used such a form, so nothing changed behaviour.
