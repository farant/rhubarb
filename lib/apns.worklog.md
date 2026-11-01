# apns worklog

## 2026-10-31 - T11: provider client for the phase-1 finish line

Library (`include/apns.h`, `lib/apns.c`) + thin CLI (`tools/apns.c`): config from `~/.rhubarb/ios/apns.stml` (no secret in it), one blocking request over `http2_nexus` (own reactor, 30 s), the spec III.4 rule, a random well-formed device token for the retry.

- **The key never passes through this library**: the CLI reads it with `tessera_clavem_legere` (moved out of `nundinae_apple_aperire`, which now calls it - one secret-handling path, mode 0600, O_NOFOLLOW, buffers zeroed), signs with `tessera_apns_facere`, zeroes the DER, hands over only the JWT. `authorization` is sent with `numquam` set; the live suite checks Go logged the field as `(sensitive)` - the never-indexed encoding really is on the wire.
- **Rule** (pure, `apns_iudicare`): transport failure -> DEFECTUS; 200 or BadDeviceToken -> TRANSITUS; Invalid/Expired/MissingProviderToken, TopicDisallowed, BadTopic -> DEFECTUS; anything else after the all-zero token -> ITERANDUM (CLI retries once with `apns_signum_fortuitum`), otherwise TRANSITUS.
- **Preflight** (`apns -praeparare`) only *looks* at the PUSH_NOTIFICATIONS capability through `nundinae_apple_capacitas(..., creare=FALSUM)`; switching it on is the separate create-only verb `nundinae_apple -capacitas <app> PUSH_NOTIFICATIONS`, run on Fran's word.
- Local oracle: Go route `/3/device/` echoes the received apns-topic / apns-push-type / authorization in x-* response headers and answers like APNs (403 MissingProviderToken without "bearer ", 400 BadDeviceToken; `-modus iterare` makes the zero token answer DeviceTokenNotForTopic).
- Smoke 2026-10-31: `apns -probare` with an openssl-generated P-256 `.p8` against the local Go server -> 400 BadDeviceToken, TRANSIT; same key at 0644 -> refused naming chmod 600.
