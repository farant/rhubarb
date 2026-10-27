# epistula worklog

## 2026-10-27 - the house email API (communio plan-3 T2, spec D7)

Callers say "send this message to this address"; a provider behind it does the
HTTP. Provider table by name like lib/vates.c: "resend" and "fictus" (records
copies in memory for tests). Adding Cloudflare or SMTP later = one more row.

- Asynchronous on the reactor. Each send takes one of EPISTULAE_MISSIONES (XVI)
  fixed slots with its OWN small piscina, destroyed when the send finishes - a
  long-running server sending a mail per sign-in never grows. The finish
  callback is always delivered on a later reactor turn (1 ms timer), never inside
  epistulam_mittere and never inside http_vocare's finis (the call's memory lives
  in the slot's pool).
- `epistulam_mittere` returns FALSUM for a malformed message AND when all slots
  are busy (the caller logs; nothing is queued silently).
- Resend: POST <basis or https://api.resend.com>/emails, Bearer key,
  Content-Type json, Idempotency-Key when given (the sign-in request id - a
  retried send never mails twice). Any non-2xx -> cause "resend: HTTP <n>",
  transport failure -> "resend: rete"; the key is never in a cause.
- The key is copied once into the Epistularius and zeroed by claudere; the
  Authorization buffer is zeroed right after the header is added (http_vocatio
  zeroes its serialized request itself).

Plants (radix gate): dropping the Idempotency-Key header and putting key bytes in
the failure cause both turned probatio_epistula red.
