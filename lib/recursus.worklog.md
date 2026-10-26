# recursus — worklog

## 2026-10-23 - K1: the library (pensa-spec §III, plan pensa-k1-plan.md)

**Shape.** Tokenizer (lowercase; ',' its own token; '&' -> "and"; '.' and
spaces separate; numbers, ordinals "15th", ISO dates classified) -> every
token must be known vocabulary (else `unknown word "x"`) -> FORMAE[] tried
in order, each from the start: 0 = no match (locus restored), 1 = match,
-1 = committed error. A match must reach the end after an optional
"from|starting <date>" clause, then `regulam_perficere` applies the global
rules (interval >= 1, caps, anchor present and a member, once has no
start). Order in FORMAE matters (see the comments in the plan: "the 32nd"
must commit in the monthly-day pattern; "5th monday" falls through to the
ordinal pattern; "oct 30" must reach the one-off pattern's year error only
after the yearly pattern declined).

**Corpus contract.** lib/recursus.corpus.stml is the test suite AND the
documentation. Each exemplum: phrase -> canonical text -> exact days in a
window, and the canonical text parsed AGAIN must give the same canonical
text and days (round trip: whatever the read view shows can be typed
back). Values with `"` or `&` go in attribute ELEMENTS (`<@incipit=>..</>`,
`<@phrasis=>..</>`): stml leaves `&quot;`/`&amp;` encoded inside attribute
values. To teach a new phrase: add the corpus line, watch it fail, make it
pass. Expected days are computed with Python datetime, never by hand - that
check caught the spec's own example ("every other monday from oct 27 2026":
Oct 27 2026 is a Tuesday).

**Plants found two missing boundary cases** (both now in the corpus):
- week phase = Sunday-aligned weeks, not (days since anchor) / 7; they only
  differ when the anchor is late in its week relative to another member day
  ("every other week on mondays and fridays from fri 23 oct 2026").
- "last <weekday> of the month" uses `d.dies + 7 > days_in_month`; `+6`
  only differs when the last one is the 25th of a 31-day month (Dec 2026).

**Caps.** Intervals: weeks <= 52, days <= 366, so `recursus_proximus`'s
8-year scan always finds an answer (Feb 29 across the non-leap 2100:
2097-03-01 -> 2104-02-29).

**Traps hit:** `L` is the latina.h numeral 50 (parameter renamed `lector`;
examen caught it before compiling); `-Wunused-function` rejects a helper
before its first caller; date - weekday offset is a `MoraDierum` (span),
not a `DiesIulianus` (fasti's type contract).
