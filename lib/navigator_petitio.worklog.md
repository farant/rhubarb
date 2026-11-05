# navigator_petitio worklog

## 2026-10-27 - N2.5 (secunda)

URL -> Wikipedia sedes -> REST URL -> http_vocare (User-Agent from
~/.rhubarb/navigator/agens, Accept with the Parsoid profile, 5
redirects) -> canonical URL from the FINAL url's path under the
original host -> navigator_versionem_offerre. Own piscina per fetch
(the 0.3-1.4 MB body), freed by dimittere after the callback. The offer
(extract + sqlite) runs synchronously inside the HTTP finis - ~10-30 ms
for a big article; fine for v1, move off the pulse if it ever shows.

LIVE FIND (first run against en.wikipedia.org): for a redirect title
the old REST endpoint answers 302 to the NEW MediaWiki REST API -
`/w/rest.php/v1/page/Thomas_Aquinas/html?redirect=no`. sedem_legere
only knew `/wiki/` and `/api/rest_v1/page/html/`, so canonicalization
fell back to the requested title and "Aquinas" was saved as its own
article. The scripted test had assumed the redirect stays on rest_v1.
Fixed in vicipaedia (VIA_REST) and the test now uses the real Location.
Fetching Thomas_Aquinas directly (rest_v1) afterwards gave IDEM against
the rest.php copy: the fingerprint holds across both endpoints' HTML.
rest_v1 is on Wikimedia's deprecation path; switching url_fontis to
rest.php is a one-line change when we choose to.

Test seam: `radix_fontis` swaps "https://<host>" for the scripted
server; canonicalization takes only the PATH of the final URL and puts
it under the sedes host, which is why the seam works with 127.0.0.1.
Live tool: build/aedilis/navigator/navigator (petere/index, -volumen
required so vicus's volume is never touched).
