# chorda_aedificator.worklog.md

## 2026-09-22 — JSON escaper: the byte rule, not ctype (json audit follow-up)

`_evadere_json` decided "control character" with
`iscntrl((signed char)c)`. For bytes >= 0x80 that passes a NEGATIVE
value other than EOF to a ctype function — undefined behaviour — and
the answer also depends on the locale. It happened to work on macOS
(iscntrl returns false there, so UTF-8 passed through raw). JSON's rule
is a byte range, so the escaper now says exactly that: `< 0x20` or
`0x7F` → `\u00xx` (DEL kept, so output is byte-identical to before),
formatted by hand from a hex table — no `snprintf` (not C89) and no
`<ctype.h>` left in this file.

Also fixed in the same function: `chorda_aedificator_appendere_evasus_json`
returned FALSUM for an EMPTY string (the escaper's "0 bytes written" was
read as failure; `!s.datum` also rejected the house's NIHIL-datum empty
chorda), and the `_literis_` variant failed on "" because
`piscina_allocare(0)` returns NIHIL. Empty = VERUM, nothing appended.
No production caller read the return (only tests), so no behaviour
changes outside the tests.

Tests: all 256 byte values, each escaped alone and compared with an
independent statement of the rule (discords PRINTED by byte, one
assertion on their count); empty-string cases via both entry points.
The 256-byte table passed BEFORE the fix — the UB is invisible on this
platform — so its proof is a plant simulating the platform risk
(bytes >= 0x80 treated as control): red with all 128 high bytes named,
green on revert. The empty-string cases were born red.

**Formatter trap** (quaestio 01M363G5HF): the house formatter mangles
this file UNTOUCHED — every brace-less `si (cond) redde X;` over 72
columns gets a continuation indented to ~column 90. The pre-commit
formats whole files, so any edit here would have shipped that. The
five such lines are now braced blocks (house style) and the escaper's
declarations are hoisted to the function top (an inner-block
declaration also skewed the outer alignment). Amalgams regenerated
(silva, officina: this change only; tessera: also its drift since
2026-09-02, latina.h tabs→spaces).

## 2026-10-21 — growth through piscina_conari_ampliare; self-append

`_crescere` asks `piscina_conari_ampliare` first and only allocates + copies
when the pool cannot grow the buffer where it is (see lib/piscina.worklog.md).

**Behaviour change for stale views.** Before, a growth left the old buffer
in the arena, so a `spectare` view taken before an append still read the
old bytes. That was never the contract ("validus solum usque ad proximam
mutationem"), but it was what happened. Now:
- path I (in place): the view sees the new bytes;
- path II (realloc): the view may point at freed memory.

**Self-append** (`appendere_chorda(a, spectare(a))` with growth, pinned by
the audit probe `probationes/fixa/sondae_auditus/chorda_aedificator_alias.c`)
worked only because the old buffer survived. `_appendere_interna` now
notices a source inside its own buffer, keeps it as an offset, and
re-derives it after growth. Plant: without the re-derivation, the suite
built as the venenum gate builds it (ASan + VENENUM) aborts with
heap-use-after-free; restored, 715/715. The suite is now in the venenum
gate's list for exactly that reason.

Audit for other stale views: heuristic scan of all `x = spectare(a)` sites
followed by a mutation of `a` and a later use of `x` found one production
candidate (lib/rivus_eventuum.c:92, a false positive: the reset is followed
by `redde`) plus the audit probes. Views held in structs are not visible
to that scan; the ASan sweep covers them.
