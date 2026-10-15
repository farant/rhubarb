# membrum__elenchi worklog

## 2026-10-14 - U4: menu wiring, extracted from the calendar

nomenclator-plan U4, header approved by Fran. Moved from kalendarium.c
(M2c): reading the open menu from the HOST's island branch (fields
`elenchus` "<contextus> <x> <y>" and `elenchus_electa`), building the
Elenchus items from the host's menu table through its Praeceptum table
(omitted outside the view with the divider carried to the next shown
item; greyed when the host's requirement predicate fails), and the
event handling while open (hover highlights; click / Return on an
enabled item closes FIRST then invokes the command; greyed swallowed;
DIALOGUS padding keeps it open; outside click and Esc close; other
keys, text and DUPLEX swallowed). The host decides when and where a
menu opens and keeps its drawing wrapper (legere + elenchus_figura)
and the two lines that add the subtree as the root's last child.

Format change (approved): the context in the field is now the host's
decimal mask (calendar: I item, II month day, IV list background)
instead of the calendar's letters r/d/v; the calendar manifest note
was updated.

The command context must stay alive while elenchi_agere runs (an item
invokes the command with hospes->ctx), so the calendar builds
ContextusMandati + ElenchiHospes in the same scope as the call
(hospitem_parare).

Tests: probatio_membrum__elenchi (legere, mus, claves). Plants: divider
not carried; view ignored; greyed item acting; outside click not
closing; Esc not closing; item acting without closing (needed a
compiling variant - every parameter must stay used) - all red.
