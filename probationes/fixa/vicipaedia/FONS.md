# vicipaedia fixtures

Wikipedia articles from the REST page-HTML endpoint (Parsoid HTML,
`profile=https://www.mediawiki.org/wiki/Specs/HTML/2.8.0`), fetched
2026-10-27 for `probationes/probatio_vicipaedia.c` (navigator N1).

- `rhubarb.html` - https://en.wikipedia.org/api/rest_v1/page/html/Rhubarb
  (whole, 295,780 bytes).
- `thomas_aquinas.html` - https://en.wikipedia.org/api/rest_v1/page/html/Thomas_Aquinas
  trimmed at top-level `<section>` boundaries: kept sections 0 (lead),
  1 (Biography) and 52 (External links); everything before the first
  section and after the last is unchanged (669,878 bytes).

Text and media descriptions are from Wikipedia, licensed under the
Creative Commons Attribution-ShareAlike 4.0 License
(https://creativecommons.org/licenses/by-sa/4.0/); authors listed in
each article's history on en.wikipedia.org. Used here unchanged (apart
from the trimming above) as test data.

- `quaestio_aquinas.json` - https://en.wikipedia.org/w/rest.php/v1/search/page?q=aquinas&limit=20
  (2026-10-27, 11,557 bytes; navigator N3.3b) - search response, unchanged.
