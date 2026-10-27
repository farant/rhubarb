
## 2026-10-27 - truncated input is INCOMPLETUS, not corruption

`flatura_inflare` never returned FLATURA_STATUS_INCOMPLETUS (the name
existed only in the message table). On running out of input the bit
reader returns -1 and the Huffman decoder fails; the failure surfaced as
whatever the caller was decoding - FRACTA_DATUM or FRACTA_DISTANTIA.
lib/git reads pack objects through a 64 KB window it quadruples ONLY on
INCOMPLETUS, so any packed object whose compressed form exceeds 64 KB
was unreadable. Invisible until a `git gc` packed a 537 KB blob as the
delta base of lib/piscina.c: pythonica's differre_git test went red.
(The audit swarm had seen it on 10-19: probe
probationes/fixa/sondae_auditus/flatura_r2.c.)

Fix: `FlaturaLector.exhaustus`, set where the reader runs dry
(`_flatura_legere_bits` past the end; Huffman with no bits left or a code
longer than the real bits that remain); `flatura_inflare` maps any
failure after exhaustion to INCOMPLETUS. Genuine corruption (reserved
block type) is still corruption. Only lib/git branches on INCOMPLETUS;
every other caller tests == OK. gzip path untouched.

Tests: probatio_flatura 'truncatio = INCOMPLETUS' (200 KB, deflated and
stored, eight cut points each -> INCOMPLETUS; full -> OK; BTYPE 11 ->
not INCOMPLETUS); probatio_git 'sarcinam magnam' builds a hermetic
one-object pack + idx v2 (300 KB blob, > 64 KB compressed) and reads it.
Plant (mapping disabled): both red.
