# base64 worklog

## 2026-10-19 - base64url (mcp-remotus T2)

RFC 4648 section 5 joins `base64.h`: `base64url_longitudo_codificata`,
`base64url_scribere` (piscina-free, no NUL - custodia's session token
lives in a fixed array), `base64url_codificare`,
`base64url_decodificare`. Unpadded only.

The decoder is strict on purpose: `=`, `+`, `/`, length % 4 == 1, and
non-zero leftover bits are all refused. The last one is the subtle one:
"Zg" and "Zh" both carry the byte `f` in their first 8 bits; a lenient
decoder accepts both, so two different strings would name the same
token. Strict = one spelling per value (same rule as pecunia's
canonical text form).

`""` decodes to `datum` pointing at a static byte with `mensura` 0 -
`datum == NIHIL` is the error signal, so empty must not be NIHIL.

Oracles: RFC 4648 section 10 vectors unpadded, RFC 7636 appendix B
(SHA-256 of the PKCE verifier) byte exact, 1000 random round trips
(sors, fixed seed). Python's `base64.urlsafe_b64encode` computed the
custodia CSRF pin.

The old file had never been through the formatter; it now has been
(banners 50 wide, `c >= 0 &&` on an unsigned `i8` dropped).
