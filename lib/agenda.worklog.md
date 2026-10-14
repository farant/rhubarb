# agenda worklog

## 2026-10-13 — A1a: document, acts, projection (agenda-plan A1)

`AgendaDocumentum` wraps a `historia` (genus "agenda", checkpoint
origin "agenda:checkpoint"), like pictor_documentum. The projection is
not fixed-size, so it uses historia's L1 hooks: the canonical state
text `<agenda proximum><res id dies rubricae perfecta><@titulus=>..</>
<nota dies>..</nota></res></agenda>` is both the checkpoint
(codificare/decodificare) and the seal input (sigillare hashes it).
Deleted items are absent from it; `proximum` keeps their ids counted.

Pools: `status` holds the projection and is emptied in `vacare`
(historia replays into it); `scheda` parses one act / builds the seal
text; `acta` builds a new act and lives until historia has appended and
applied it (it must NOT be `scheda`: applicare empties scheda while the
act chorda is still being read). pictor parses acts into its long-lived
pool, which grows per act - not copied here.

Format findings (Fran 2026-10-13):
- STML attribute values are raw both ways: a `"` cannot be represented
  (conservatio silently skips such values). Titles are user text, so
  they are ALWAYS written in attribute-element form `<@titulus=>..</>`
  (Fran's suggestion) - `stml_attributum_capere` reads it through its
  second lookup path, so readers do not care which spelling was used.
  The element must be among the FIRST children (lookup stops at the
  first non-attribute child). No public constructor: element "@" +
  `attributum_titulus` + text child. Plant: title as a plain attribute
  -> the quoted-title round trip goes red.
- Empty attribute values are avoided (the interner refuses ""): no
  `dies` = unscheduled, no `rubricae` = empty set. Readers treat absent
  and empty alike.
- Note text is element content (entities symmetric); `stml_textus_valor`
  reads it. Two write-read cycles (and a checkpoint restore with
  intervallum III) keep `<a & "b">\n  'c' &amp; ]]>` intact. Plant:
  checkpoint drops note text -> red.

Validation lives in the typed act functions (empty title, bad date, bad
rubric, absent/deleted item -> 0, nothing appended); `applicare`
ignores malformed acts so replay is robust. Rubric sets are sorted and
de-duplicated so the seal does not depend on typing order.

## 2026-10-13 — A1b: queries + seal cost

Queries (header text approved): `agenda_res_diei` (assigned to the
day), `agenda_perfectae_die` (completed that day, wherever assigned -
decision 4), `agenda_signa_mensis` (one pass; assigned / still open /
completed per day; days beyond the month zeroed; bad month refused).
Dates are validated at write, apply and decode, so `signa[dies - 1]`
is always in bounds. Plant: completions counted by the ASSIGNED day ->
red at the 14th's mark.

Seal cost (agenda-plan AUDIENDA; `tools/agenda_mensura.c`, items with
a date and a note, 200 title edits measured at size n):

| items | per act | reopen | build (3 acts/item) |
|---|---|---|---|
| 100 | 178 us | 0.7 ms | 31 ms |
| 500 | 786 us | 2.5 ms | 0.6 s |
| 1000 | 1.56 ms | 4.7 ms | 2.4 s |
| 2000 | 3.0 ms | 10.9 ms | 9.3 s |

Linear in state size (~1.5 us per item per act): every act rebuilds
and hashes the whole canonical text. `sample`: seal ~90%+ (tree build
~43%, hash ~31%, serialize ~25% of it); apply and volume append under
6%. Fine for human-paced acts at hundreds/thousands of items; bulk
creation is quadratic (import would want it). Path if ever needed:
incremental seal - hash per item cached, seal = hash of item hashes in
id order - no API change.

## 2026-10-13 — A6a: agenda_perfectae

Live completed items, newest completion first (completion day
descending, then id descending), deleted and reopened ones excluded.
Insertion sort while scanning the records (history lists are short; n^2
is fine until it is not - pagination does not change the query).
Plant: oldest first -> red.
