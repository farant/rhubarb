# tls13_nuntii worklog

## 2026-10-16 - T3 (tls13-plan)

Readers take message BODIES (the engine has read the 4-byte header)
and every one ends with an exact-consumption check. Consequence found
while planting: a truncation sweep cannot see a broken bounds check -
the truncated input is still rejected (consumption mismatch) and the
over-read lands in the rest of the full message buffer, valid memory, no
crash. The sweep's crash pass now places each prefix flush against an
unmapped region (mmap 2 x 64 KiB, munmap the upper half; no mprotect -
it is not in examen's POSIX lexicon), so one byte past the end faults.
General lesson: "rejects truncation" and "never reads past the end" are
different properties; the second needs a guard region (or ASan).

RFC 8448 §5's HRR has a real cookie extension (0x002c, 116 octets incl.
the inner length; 114-octet cookie) - the test reuses it and checks our
CH2 echoes it exactly as the RFC's CH2 does.

A ServerHello cut right after the compression byte IS a valid TLS 1.2
ServerHello without extensions (versio_1_2) - the sweep names that one
prefix as licit.

ClientHello order: server_name, supported_groups, signature_algorithms,
supported_versions, cookie (CH2 only), key_share. CH2 keeps
supported_groups {x25519, secp256r1} (RFC 8446 4.1.2: only key_share,
cookie, early_data, psk, padding may change).
