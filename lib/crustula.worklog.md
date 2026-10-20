# crustula worklog

## 2026-10-17 - born (hospitium v3 T4)

- **Refuse, never escape.** `crustulum_scribere` returns an empty chorda
  for any title outside the RFC 2616 token set, any value outside
  RFC 6265 cookie-octet, and any Path/Domain with `;` or a CTL. The
  caller (hospitium) then simply does not send the header. Escaping or
  truncating would hand the browser something the app did not mean.
- **Attribute order is fixed** (name=value; Max-Age; Path; Domain;
  Secure; HttpOnly; SameSite) so tests can compare whole strings.
- **SameSite=None without Secure is refused** - browsers drop such
  cookies silently, which is worse than a loud refusal. Same idea for
  the `__Secure-` / `__Host-` prefixes.
- **Reading is lenient** (RFC 6265 4.2 says servers should be): extra
  spaces, a quoted value, pairs without `=` or without a name are
  skipped, first match wins, names are case-SENSITIVE.
- Absent vs empty: absent = `datum NIHIL`; `a=` = non-NIHIL, mensura 0.
