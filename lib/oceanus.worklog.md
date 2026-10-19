# oceanus worklog

## 2026-10-19 - born (hospitium v3 T10)

- **Blocking by design.** oceanus runs from the Mac (CLI, setup
  script), never inside the server loop, so it uses `http_exsequi`
  directly; HTTPS goes through tls13 + fiducia_macos. On Linux
  fiducia_posix refuses every chain (T9), so oceanus is Mac-only for
  real use - fine, deploys start on the Mac.
- **Token handling:** raw `open(O_NOFOLLOW)` + `fstat` (regular file,
  no group/other bits) + `dop_v1_` prefix; the token lives only in the
  struct's fixed buffer, zeroed in `oceanus_claudere`; the per-request
  "Bearer ..." string is zeroed right after `http_petitio_caput_addere`
  (which copies it). Errors carry DigitalOcean's `message` field or a
  connection cause - the test captures stderr and checks the token is
  nowhere.
- **Paging:** DO returns an absolute `links.pages.next`. It is followed
  only when it starts with our base + "/"; a link to any other host is
  an error (the token would be sent there).
- **assecurare is idempotent:** one GET of all records, then create /
  PUT / nothing; IDEM sends no write (the fake counts writes). Two
  matching records = ambiguous -> error, nothing changed.
