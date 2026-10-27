
## 2026-10-27 — network-path references ("//host/path") resolved against the base host

uri_resolvere ignored the authority of a relative reference: "//thumb.wikimedia.org/a.jpg" against "https://en.wikipedia.org/wiki/X" came out as "https://en.wikipedia.org/a.jpg" (base host, reference path). RFC 3986 5.2.2: when the reference has an authority, take authority, path and query from it and only the scheme from the base. uri_parse already parsed protocol-relative input (hospes set); only the resolver dropped it. Found by navigator N1 (Wikipedia <img src="//thumb.wikimedia.org/...">). Test: probatio_uri uri_resolvere step "via retis"; plant (branch short-circuited) red. Only user so far: lib/vicipaedia.c.
