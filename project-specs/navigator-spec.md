# NAVIGATOR — a reader-style browser pane in vicus

*Designed with Fran 2026-10-27 (brainstorm in chat, after the comites
P9 work). A browser pane for a WHITELIST of sites, starting with
Wikipedia: pages are fetched, turned into one consistent house layout
and paginated, never scrolled. Not lynx and not a general browser.
DRAFT - the pane is `navigator` (Fran 2026-10-27); other Latin names
are proposals until each phase's header text is approved.*

## I. Intent

Fran wants to read parts of the web inside vicus the way he reads
books there: keyboard-first, paged, calm, in our own elements and
palette, with a companion notebook beside every page.

- **Reader view, not a renderer.** No site CSS, no scripts. Each
  supported site has an ADAPTER that knows where the content is and
  turns it into our document model; one house style lays it out.
- **Grow by whitelist.** Do a few sites well rather than every site
  badly. A link to an unsupported site is shown as such.
- **Saved, not live.** An opened article is kept; a refresh fetches
  again and keeps the new version only if the CONTENT changed.
- **First site: Wikipedia** - standard structure, broadly useful.

## II. Goals (v1)

- A new pane genus `navigator`, separate from the reader (Fran).
- **Bookmarks, Yahoo-style:** a tree grouped by root domain, drill down
  (`wikipedia.org` > `en.wikipedia.org` > articles). Only BOOKMARKED
  articles appear in it. Paginated lists like every other view.
- **Current page + history:** what you are reading now, with the trail
  of pages visited (back/forward); not part of the bookmark tree.
- **Open an article:** typed URL, Wikipedia search/title, a link, or a
  bookmark -> fetch -> adapter -> saved snapshot -> pages.
- **Pages:** headings, paragraphs, lists, quotes, the infobox, link
  spans; one consistent layout.
- **Page grid (Fran's idea, "NES Zelda"):** j/k = next/previous page
  (moving down/up); a page may carry a side column of ancillary pages
  to its right (l in, j/k within, h back to THAT page - §V.18), e.g.
  the infobox beside the first page. An indicator on a pane edge shows
  that a move in that direction exists.
- **Links:** selectable and followable; a link to a supported site
  opens through its adapter, others are marked unsupported.
- **Versions:** refresh fetches again and compares the extracted
  content (not the HTML bytes). If it differs: "new version available"
  - the version you are reading stays; you choose to switch. Every
  version is kept and can be opened again; notes live with the version
  they were written in.
- Locus per article (and version), so Cmd+T companions work as
  everywhere else.

## III. Non-goals (v1) and later

- Arbitrary sites, JavaScript, site CSS, logins/cookies, POST forms.
- Pictures - **v2** (Fran): fixed bounding boxes and positions,
  palette + Atkinson dithering (`include/dithering.h`), decoded by
  `imago` (stb_image, `lib/imago.c`).
- Sentence reader mode like lector's - wanted (Fran), after v1.
- Tables other than the infobox (Wikipedia data tables later).
- More sites: candidates Hacker News (JSON API), catholicbible.online
  (server-rendered, 41 KB front page, 3 small scripts - measured
  2026-10-27 with curl).

## IV. Anti-goals

- Scrolling views.
- Per-site visual styles leaking through (every site looks like us).
- A "refresh" that creates a new version when only markup changed.
- Silently losing a companion note or reading position because an
  article was updated.

## V. Decided (Fran 2026-10-27)

1. A separate **browser pane**, not part of the reader.
2. **Bookmarks** organised by root domain, browsable by drilling down.
3. **Saved articles**, with an option to fetch a new version that is
   kept only if a content diff shows it is actually new.
4. Reuse from the reader by pulling shared parts into component
   libraries where it fits; divergence is fine.
5. **Paginated**, consistent layout and style; our elements.
6. Pictures later, in fixed boxes, our palette and dithering.
7. A sentence reader like lector's is desirable.
8. Start with Wikipedia; widen a whitelist of sites done well.
9. Name: **navigator**.
10. Opening: typed URL, Wikipedia search/title, links, bookmarks.
11. Refresh: "new version available", the old version stays; all
    versions kept and switchable; notes live with their version.
12. Bookmark tree holds only bookmarked articles; a separate "current
    page" with a history.
13. Page navigation may be a GRID: j/k down/up through the article,
    h/l to side pages (infobox to the right of the first page as one
    option), edge indicators where a move exists. Infobox layout:
    try and see.
14. The grid starts with the infobox only - but it is a TOOL in our
    pagination toolbox, expected to find more uses later (tables,
    galleries, references; possibly other panes). Build it general
    enough to reuse, use it for one thing first.
15. Wikipedia search shows a LIST of results to pick from.
16. A companion on a new version starts EMPTY.
17. **Notes overview (left pane):** a paginated view of every page of
    the current web page that has companion notes, clustered by
    version - "thumbnails" of each view with notes, an easy way to
    see where you wrote and jump there.
18. **Not a global grid - side columns BELONG to a page.** The article
    is one main column (j/k). A page may carry ancillary content as a
    side column to its right (l enters it at its first page); inside
    it j/k move within that column; h returns to the page it belongs
    to. From page 1: l j j j h -> page 1 (not page 4). So the model is
    "main column + columns attached to individual pages", not a 2D
    array.
19. **Link index (Fran 2026-10-27):** the last pages of an article are
    an inventory of all its links (one line per distinct target).
20. **Mouse too (Fran 2026-10-27):** keyboard-first, but every view
    gets a decent mouse path.

## VI. Calls made by Claude (check these)

1. **Wikipedia source = the REST page-HTML endpoint**
   (`/api/rest_v1/page/html/<Title>`, Parsoid HTML): content only, in
   `<section>` elements, no site chrome - easier and steadier to
   extract than the normal page. Needs a User-Agent that follows
   Wikimedia's policy (requests without one are refused). Search
   results (§V.15) from Wikimedia's JSON search endpoint
   (`/w/rest.php/v1/search/page?q=...&limit=N`) through our `json`
   library - not yet fetched (AUDIENDA).
2. **Own document model** (not `codex`): codex's blocks are title /
   paragraph / verse (`include/codex.h:27`); the web needs list items,
   quotes, key/value (infobox), figures (v2) and link spans. Shape
   kept close to codex's (chapter text + blocks with byte offsets +
   sentences) so lector's paginator could later move into a shared
   library both use.
3. **Extraction is pure and fixture-tested:** saved HTML files of real
   articles in `probationes/fixa/`, adapter -> model compared exactly;
   no network in the gates.
4. **Snapshots in the pane's volume.** The volume already stores
   content by hash (`plagulae.sigillum -> massae`), so "is this
   version new?" = does the extracted document's canonical text hash
   differ. Every version kept (Fran: switchable; notes live with the
   version they were written in), so the locus includes the version.
5. **Root domain** for the bookmark tree: v1 = what each whitelisted
   site's adapter declares (wikipedia.org for `*.wikipedia.org`), not
   a general rule - a general one needs the Public Suffix List
   (`co.uk` etc.).
6. Fetching through `http_vocatio` (non-blocking on vicus's reactor,
   `include/http_vocatio.h`); HTML through the `html/` tree builder
   (html5lib oracle 1510/1700 per `html/CLAUDE.md`).

## VII. Pieces

| Piece | State |
|---|---|
| DNS, TCP, TLS 1.3 (+ BearSSL 1.2), certificate trust | have (`nomina`, `tcp`, `tls`, `fiducia`) |
| HTTP/1.1: chunked, redirects; non-blocking call | have (`http`, `http_vocatio`) |
| URI parse + relative resolution | have (`uri_resolvere`, `include/uri.h:110`) |
| HTML tokenizer, HTML5 tree builder, entities | have (`html_lexema`, `html/`, `entitates_html`) |
| gzip | have (`flatura`) - not wired to Content-Encoding; v1 can just not ask for it |
| Sentence splitting | have (`sententia_fissio`) |
| Pagination of blocks into pages | have inside lector (`lector_paginae`) - candidate for a shared library |
| Image decode + palette dithering | have (`imago`, `dithering`) - v2 |
| Document model for web articles | **new** |
| Wikipedia adapter (Parsoid HTML -> model) | **new** |
| Snapshot store + content diff | **new** (on the volume) |
| Browser pane: bookmarks tree, article pages, links | **new** |

## VIII. Phases (sketch, to refine with Fran)

| Phase | Content |
|---|---|
| **N1** | Document model + Wikipedia adapter, pure, on saved fixtures (2-3 articles of different shapes). DONE 2026-10-27: `include/articulus.h`, `include/vicipaedia.h` + `lib/vicipaedia.c` (headers approved), fixtures Rhubarb + Thomas Aquinas (trimmed), suite probatio_vicipaedia (minimum, rhubarb, aquinas); plants red (4). Found on the way: `uri_resolvere` dropped the host of `//host/path` references - fixed (probatio_uri). |
| **N2** | Fetch (User-Agent, redirects) + snapshot store + content diff on refresh. Steps (Fran 2026-10-27): N2.1 content fingerprint (`articulus_canonicum`/`articulus_sigillum`, `lib/articulus.c`) DONE 2026-10-27; N2.2 Wikipedia URL <-> API URL (`VicipaediaSedes`, `vicipaedia_sedem_legere` / `_url_articuli` / `_url_fontis`; `url_viam_decodificare` keeps "+") DONE 2026-10-27; N2.3a `uri_resolvere` to the RFC 3986 5.4 table + scheme only as a prefix in `uri_parse` (Fran: fix the library before redirects rely on it) DONE 2026-10-27; N2.3 redirects in `http_vocare` (`redirectiones_maximae`, 0 = off; `http_vocatio_url`) DONE 2026-10-27; N2.4 `navigator_bibliotheca` (articles + versions on the volume, ULIDs, raw HTML kept; "new?" = both re-extracted with the CURRENT adapter, fingerprints compared; acts `navigator-articulus` / `navigator-versio`, HTML as massa, index from own acts) DONE 2026-10-27; N2.5 end-to-end non-blocking fetch + CLI (`navigator_petitio`, `tools/navigator.c`; UA from `~/.rhubarb/navigator/agens`, Fran: https://franarant.com) DONE 2026-10-27 - live: Rhubarb, Aquinas -> Thomas_Aquinas (rest_v1 now redirects to `/w/rest.php/v1/page/<T>/html`; sedem_legere accepts it). UA = a setting (`~/.rhubarb/navigator/agens`). Search moves to N3. |
| **N3** | Pane: bookmarks tree by root domain, open article, paginated pages in the house style. Steps (Fran 2026-10-27): N3.1 Articulus -> one CodexCaput flow -> `lector_paginare` (`navigator_paginae`; lists "• " + 2 spaces/level, figures skipped until N5, infobox kept for the side column, link spans carried) DONE 2026-10-27 (Rhubarb: 7 pages at 500x600 px); N3.2 pane skeleton + locus `navigator/pagina/<art>?versio=&pagina=` (`navigator.h`, apps/navigator manifest, `$nav`; version PINNED on open so companions stay with their version; mouse: rows, double click, arrows) DONE 2026-10-27; N3.1b link index on fresh last pages (Fran 2026-10-27: one line per distinct target, first-appearance order, external host shown, lines are links for N4) DONE 2026-10-27 (Rhubarb: 7 article pages + 6 link pages, 136 targets - spacing is the paragraph spacing; denser layout is a later tweak); N3.3 URL prompt + `$nav(url)` + fetch on vicus loop (o / "Open URL..." button; saved URL opens offline; messages in `nota`; new fetch cancels the pending one; pane idle when nothing pending) DONE 2026-10-27; N3.3b Wikipedia search -> results list (V.15; words in the url: prompt, "la: Roma" picks the language, Enter/double click opens through the URL path) DONE 2026-10-27; N3.4 bookmarks (acts) + current page + history - split N3.4a bookmarks (act `navigator-favor`, root "Saved" -> domain rows with counts -> bookmarks by host; b / footer star toggle; Esc up a level) DONE 2026-10-27, N3.4c remember the reading page per article (presence "navigator/positio/<ULID>": version + text offset of the page, link-index flag; restored on open without a named page, survives pane resize; other version -> page 1) DONE 2026-10-27, N3.4b recent list (presence "navigator/recentes", newest first, unique, cap 20; "Recent" group at the root under Bookmarks, Enter opens, b bookmarks) DONE 2026-10-27; N3.5 infobox side column (grid tool); N3.6 refresh / versions. |
| **N4** | Links: select, follow (whitelisted -> adapter; others marked). Fran 2026-10-27: links drawn YELLOW like hashtags in scriba, clickable (mouse) as well as keyboard-selectable; the link-index pages behave the same. Steps (Fran 2026-10-28): N4.1 links drawn in the editor's hashtag color (COLORATIO_LINK), lines cut into pieces, wrapped links colored on both lines DONE 2026-10-28; N4.2 mouse click opens (hit test line -> column -> offset -> note; non-Wikipedia = "Unsupported site: <host/path>"; Ctrl+O back via vicus history) DONE 2026-10-28; N4.3 Tab / Shift+Tab select (across pages), Enter opens, Esc clears, footer shows the target DONE 2026-10-28; N4.4 maybe: #Section links jump to the heading's page; N4.5 `~` hides body links for clean reading (persistent per pane; link index stays active; h/l stay reserved for the grid) DONE 2026-10-28. |
| **N5** | Pictures in fixed boxes (thumbnail fetched at box size, dithered). |
| **N6** | Sentence reader mode. Steps (Fran 2026-10-28): N6.1 sentence view (s from a page: first sentence starting on it; j/k; s/Esc back to the containing page; footer arrows; lector's codex_sententiae + lector_sententiam_disponere reused) DONE 2026-10-28; N6.2 location/restore/companions in sentence view ("navigator/sententia/<art>?versio=&sententia="; bare form -> saved position; pilcrow) DONE 2026-10-29; N6.3 links in sentence view; later maybe lector's split/join + sentence favourites. |
| **N7** | Notes overview: pages with companion notes, by version (§V.17). |

## IX. Open questions for Fran

Answered 2026-10-27 -> §V items 9-17. Still open:

1. ~~Grid movement inside a side column~~ - decided, §V.18.
2. **Infobox layout** - try and see (Fran); N1 extracts it as
   key/value data so any layout can be tried later.
3. **Notes overview "thumbnails":** a scaled-down drawing of the page,
   or a text card (page title/first line + the note's first lines)?
   Decide with a mock-up at that phase.

## X. Groundwork (2026-10-27, curl + a throwaway Python survey)

**Wikipedia REST page HTML** (`/api/rest_v1/page/html/<Title>`, UA
"rhubarb-navigator/0.1 (...)", all 200, `profile=.../HTML/2.8.0`):

| Article | Bytes | Visible text | Attribute bytes | `<section>` |
|---|---|---|---|---|
| Rhubarb | 295,780 | 42,182 | 199,484 | 17 |
| Thomas Aquinas | 1,362,111 | 203,038 | 874,923 | 53 |
| Hagia Sophia | 1,459,344 | - | - | - |

Most bytes are attributes (`data-mw`, ids, `about`); most visible text
in a long article is navboxes (Aquinas: 4,130 `li`, 188 `navbox-list`)
and references (274 `mw-ref`), not prose. Structure is regular:
- `<section data-mw-section-id="N">` per section (0 = lead), heading
  `<h2>`/`<h3>` with the anchor as `id`;
- infobox `table.infobox` (`th.infobox-label` / `td.infobox-data`;
  Rhubarb has TWO: the taxon box and a nutrition box);
- links `<a rel="mw:WikiLink" href="./Title">` (external:
  `rel="mw:ExtLink"`);
- figures `<figure typeof="mw:File/Thumb">` with `<figcaption>`;
- to drop: `sup.mw-ref` (reference markers), `.navbox*`,
  `.hatnote` (or keep as a small note), `<style>`/`<link>`
  (templatestyles), the References/Further reading/External links
  sections (or move them to a side column).

**Search** (`/w/rest.php/v1/search/page?q=aquinas&limit=5`): 2,772
bytes JSON, `pages[]` with `key` (title for the HTML endpoint),
`title`, `excerpt` (HTML with `<span class="searchmatch">`),
`description`, `thumbnail{url,width,height}`.

**HTML tree:** `html/arbor.sh` parses both samples cleanly (Aquinas
1.0 s real / 0.30 s user, including the STML projection the tool
prints). A consumer outside `html/` already walks the tree:
`tools/runae_corpus.c` (`html_arbor_parsare` -> `MateriaNodus`,
element slots `HTML_ELEMENTUM_*` in `html/fontes/html_registrum.h`,
entities via `entitates_html`). It walks the BYTE tree; adoption /
foster parenting can make that differ from the DOM on messy HTML -
Parsoid output is generated and clean, so the byte tree should do.

## AUDIENDA

- The HTTP client has not been tried against Wikimedia's servers
  (curl was used for the groundwork).
- How the tree is linked into a `lib/` pane (aedilis + materia
  objects) is not checked; `tools/runae_corpus.c` builds against
  `html/build` objects through its own script.
- Wikimedia's User-Agent policy asks for contact details; the
  groundwork UA had none (Fran's call).
- How a companion note anchored in an old version follows to a new
  version is not designed (VI.4 keeps old versions so nothing is
  lost, but that is not the same as carrying notes forward).
