# tls13_clavis worklog

## 2026-10-16 - T1 (tls13-plan)

Key schedule + transcript on BearSSL primitives (br_hmac with the
sha256/sha384 vtables; br_hash_compat_context for the transcript, whose
`out()` does not change state, so digest-then-continue is free).
HKDF-Expand-Label is ours (P7): BearSSL's br_hkdf is one Expand stream
per context, the key schedule expands one secret under many labels.

Oracle = the unmodified RFC text (probationes/fixa/tls13/rfc8448.txt,
sha256 6564d137...fba) read by probationes/rfc8448.h. Things about the
RFC's format that the reader depends on:
- action headers are `   {client}  ...` / `   {server}  ...`; some wrap
  onto a 6-space line ending in `):` with no "octets" - skipped;
- fields are `      name (N octets):  hex`, continuation lines 9 spaces;
  `(empty)` for 0 octets; `salt:  0 (all zero octets)` is NOT a field;
- page breaks: a `\f` line, `Thomson ... [Page N]`, `RFC 8448 ...`;
- "calculate finished" gives PRK, an EMPTY hash (the Expand context),
  finished_key and verify_data - but not the transcript hash; the test
  rebuilds it from the RFC's CH SH EE Certificate CV.
The reader checks every declared octet count; a plant that stops a field
16 octets early is caught by that check alone.

Mutation note: `k += II` -> `k += III` in the hex scanner is an
EQUIVALENT mutation (pairs are always single-space separated), not a
test gap.

HRR transcript (`tls13_transcriptum_hrr`, 0xfe message_hash) is mute
here by design - proven against RFC 8448 §5 in T5.
