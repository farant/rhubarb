# fiducia_der.worklog.md

## 2026-10-22 - born (fiducia-linux T1)

A small DER reader for exactly the certificate parts fiducia needs and
BearSSL's decoder does not expose: issuer and subject (whole Name TLVs,
for OneCRL's issuer+serial and subject+key entries), the serial's
INTEGER octets (leading 0x00 KEPT - OneCRL's serialNumber is those
octets exactly), the SubjectPublicKeyInfo TLV (hashed for pubKeyHash),
and SAN iPAddress entries (BearSSL's name elements cannot extract them,
vendor/bearssl_amalgama.h:9449). It judges FORM only; signatures stay
with BearSSL.

- **Lengths:** definite only (short form, or long form with 1..4
  octets); indefinite (0x80) and high tag numbers refused. Every TLV is
  checked against what is left of its PARENT, and the outer Certificate
  must cover the input exactly.
- **The plant that went green first.** Removing the per-TLV bound check
  passed every test at first: the outer "covers the input exactly" rule
  rejects any truncated or over-long OUTER input before an inner TLV
  matters. The missing case was an INNER length running past its
  parent: `der_malum` now gives tbsCertificate 100 extra octets with
  zero padding after the certificate, so without the bound the walk
  reads into the padding (00 00 = empty TLVs) and returns VERUM. Plant
  red at that assertion, restored green. (A first plant run looked red
  only because the radix gate also runs the plist suite, which failed
  on my CREDO_SECTIO - never restrict sections for a silva.planta run.)
- Fixtures added to probationes/fixa/tls/generare.sh: serie_alta (0x8001
  -> 00 80 01), serie_longa (20 octets), ip_solum (IPv4 + IPv6, no DNS),
  sine_san (CN only), geminus (localhost.key again, other serial - the
  cross-sign stand-in), sha1, rsa_parva (RSA-1024); the last two are for
  T3. They are made with `x509 -req -set_serial` because `openssl ca`
  takes serials from its own file. `generare.sh` now falls back to the
  system openssl when Homebrew's is absent, and both Linux images gained
  `openssl` (OpenSSL 3.3.7 Alpine, 3.0.2 Ubuntu - both still sign
  SHA-1 leaves for the test).
- Linux: probatio_fiducia had never compiled under gcc
  (-Wformat-overflow on 512-byte path buffers); widened. It now compiles
  and fails 6 checks - the placeholder's refusals, T3's red.
- Names: SAN -> "alterna" (nomina alterna), tbs -> "signandum", TLV ->
  "_elementum", OID -> "ident" (vocabula lint).
