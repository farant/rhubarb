# nomina_macos.worklog.md

## 2026-10-14 - born (http-vocatio T2, spec II.1, decision ...CYT9A8)

Non-blocking name resolution for the in-process HTTP client (route B
of $question). `DNSServiceGetAddrInfo` from `<dns_sd.h>` (libSystem, no
framework): it goes through the system resolver, so answers match what
every other app on the machine gets (per-interface DNS, VPN split DNS,
/etc/resolver, /etc/hosts, mDNS). `DNSServiceRefSockFD` gives an fd that
goes into the CALLER's reactor; `DNSServiceProcessResult` runs when it
is readable and calls our reply callback.

- **Answers arrive per family.** With `kDNSServiceProtocol_IPv4 |
  _IPv6` each family answers separately - an address (`kDNSServiceFlagsAdd`)
  or `kDNSServiceErr_NoSuchRecord` carrying a sockaddr of that family.
  `kDNSServiceFlagsMoreComing` only marks a batch boundary, not "done".
  We finish when both families have answered, or 50 ms after the first
  address (RFC 8305 §3 "resolution delay": a slow AAAA must not hold
  up a working A answer). Both families negative -> "nomen ignotum".
- **Never finish inside the DNS-SD reply callback.** Finishing
  deallocates the service ref; doing that inside its own reply chain
  is asking for trouble, and the caller's `finis` might start the next
  step synchronously. Every finish goes through a reactor timer
  (0 ms when both families are in), which also gives the "finis never
  inside nomina_quaerere_incipere" rule for literal IPs.
- **Port is set here** (plan R3): addresses come back as raw sockaddr
  bytes with `sin_port`/`sin6_port` filled, so `tcp_posix.c` never needs
  `sockaddr_in6` (absent from the examen lexicon) - this is a `.m`
  platform file, not judged by examen.
- **Literal IPs** (`inet_pton` v4 then v6) skip DNS entirely: one
  address, finish on a 0 ms timer, no fd in the reactor.
- `abrumpere` deregisters the fd, deallocates the ref, cancels the
  timer and never calls `finis`; idempotent.

Tests `probationes/probatio_nomina.c` (credo v2): localhost (offline -
mDNSResponder answers from /etc/hosts), literal, abort, empty host.
The `.invalid` "unknown name" case needs the network and lives in the
rete file of T5. Not tested: a family that never answers (the caller's
connect deadline covers it).

## 2026-10-14 - negative answers need kDNSServiceFlagsReturnIntermediates (found in T5)

The rete file's `nomen ignotum` section (`nomen-nullum.invalid`) got NO
callback in 10 s. `DNSServiceGetAddrInfo` delivers negative answers
(`kDNSServiceErr_NoSuchRecord`) only with
`kDNSServiceFlagsReturnIntermediates`; without it an unknown name just
waits, so the HTTP layer would have reported "tempus connexionis
excessum" after 30 s instead of "nomen ignotum" - and the "both families
answered" fast path never fired (every lookup waited the full 50 ms
resolution delay). Flag added; checked by hand (gates only link rete
files): without it the section is red, with it green; the offline
suite stays 4/4.
