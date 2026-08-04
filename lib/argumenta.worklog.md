# argumenta worklog

## 2026-08-04 — _parsere_breve: exact match, no more truncation

Measured under silex proicere: registering "-scribere" as a brevis
name did nothing — _parsere_breve truncated EVERY single-dash token
to its first two chars before lookup, so "-scribere" matched (or
errored as) "-s". Silent-false-match family: a user typing
"-verbose" would silently get "-v"'s behavior.

Fix: look up the whole token exactly. "-v" still works (it IS its
own exact token); multi-char single-dash names — the house flag
style (-scribere, -ad, -musl, -fumus in the shell tools) — now work
in argumenta-based CLIs; unknown-option errors name the token the
user actually typed. Blast radius checked: the four other consumers
(aedilis, generare, lector_eventuum, sputnik) register only 2-char
shorts and double-dash longs. Pinned in probatio_argumenta:
"-scribere" present, "-s" NOT falsely set, "-ad XII" option value
delivered.
