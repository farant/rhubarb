
## 2026-10-27 — network-path references ("//host/path") resolved against the base host

uri_resolvere ignored the authority of a relative reference: "//thumb.wikimedia.org/a.jpg" against "https://en.wikipedia.org/wiki/X" came out as "https://en.wikipedia.org/a.jpg" (base host, reference path). RFC 3986 5.2.2: when the reference has an authority, take authority, path and query from it and only the scheme from the base. uri_parse already parsed protocol-relative input (hospes set); only the resolver dropped it. Found by navigator N1 (Wikipedia <img src="//thumb.wikimedia.org/...">). Test: probatio_uri uri_resolvere step "via retis"; plant (branch short-circuited) red. Only user so far: lib/vicipaedia.c.

## 2026-10-27 - RFC 3986 5.4 table + scheme as prefix (navigator N2.3a)

Ledger audit item …F0V (2026-10-18) confirmed with a fresh probe: 12 of
42 RFC 5.4.1/5.4.2 rows wrong. Causes:
- the resolver normalized with `via_normalizare`, which is FILE-path
  normalization: it drops trailing slashes and collapses "//". For URLs
  "g/" and "g" are different resources. Now `_puncta_removere` =
  RFC 5.2.4 remove_dot_segments (steps A-E, written to the letter).
- empty-path references ("?y", "#s") took the path from the reference
  (empty) instead of the base, and lost the base query; "" returned
  NIHIL instead of the base without fragment.
- `uri_parse` took the first "://" ANYWHERE as the scheme separator:
  "/proxy/http://evil.com/x" parsed as absolute with host evil.com.
  Now the scheme must be a prefix of RFC 3.1 scheme characters.
Left on purpose (comment in probatio_uri): "g:h" / "http:g" (scheme
without "//" - Uri has no authority flag and uri_construere always
writes "://") and "//g" -> "http://g/" (same page). Test: section
"RFC 3986 5.4" iterates the table; plants (via_normalizare back, base
query not inherited, empty ref NIHIL, segment not removed, '/' allowed
in scheme) each red.
