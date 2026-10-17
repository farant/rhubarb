# tls13_recordum worklog

## 2026-10-16 - T2 (tls13-plan)

RFC 8448 §3 gives every encrypted record as `payload` (inner content,
WITHOUT the type byte) and `complete record`; length check = 5 + payload
+ 1 + 16 (no padding anywhere in the traces). Sequence numbers per
direction and key: server s_hs = 0 (one flight record); server s_ap =
NST 0, data 1, alert 2; client c_hs = Finished 0; client c_ap = data 0,
alert 1. A nonce bug that only touches the high-order XOR bytes is
invisible at sequence 0 - the `data` section seals/opens records 1 and 2
for that reason (the nonce plant goes red on the client alert record).

ChaCha20-Poly1305 and AES-256-GCM have no RFC 8448 vectors; here they
only round-trip (+ tag tamper, + sequence mismatch). Their interop proof
is T6 (s_server -ciphersuites forced per suite); the primitives
themselves are KAT-checked by the `bearssl` gate.
