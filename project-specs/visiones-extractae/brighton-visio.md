# Visiones extractae — brighton-visio.md

*Extracted 2026-10-02 from `project-specs/brighton-visio.md` (Fran's
text, 2026-09-23); filtered by Fran the same day (`[+]`, 19 of 73);
classified afterwards. Rebuilt from `docs/visiones/brighton.html`, whose
inventory held the extraction after this sheet was first retired. One
claim per bullet; `Lnn` = source line; `(illatum)` = my inference.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment,
`{VISIO-OPERIS}` project vision, `{DECRETUM}` load-bearing decision,
`{SUPELLEX}` furniture, `{nulla:…}` fits no tier (ratio, exemplum,
quaestio). `> Xn` = the item(s) it is evidence for; `> Rn` = the
project-wide regula it bears on.*

## I. What Brighton is and why

- [+] B1 Brighton is "an evolution of the internet", what you would design for shared scholarship with 30 years of hindsight (L78)  {VISIO-OPERIS}
  - [ ] B1.1 the current internet works well considering how unplanned it is, but would be done "more intentionally in certain areas" (L78)  {nulla:ratio}
  - [ ] B1.2 it is "something that rationally should exist" (L76)  {nulla:ratio}
  - [ ] B1.3 it must be made "as an integrated whole covering all these domains" to function as it should (L76)  {DECRETUM}
  - [ ] B1.4 its parts benefit from being implemented in tandem (L30)  {nulla:ratio > B1.3}
- [ ] B2 It could be Fran's product focus for the next year or so (L66)  {nulla:ratio}
  - [+] B2.1 it can prove much of the "private internet" technology (L66)  {OCCASIO}
  - [+] B2.2 it dogfoods rhubarb for web servers (L28)  {OCCASIO}
  - [ ] B2.3 personal motive: interest in Catholic texts (L66)  {nulla:ratio}
- [+] B3 Prove it on Catholic texts, then expand to all scholarly domains (L68)  {DECRETUM}

## II. The pieces

- [+] B4 A stable text-addressing scheme: `c1.s4.pxyz7.abcd8.2` (L36-42)  {DECRETUM}
  - [ ] B4.1 sentence id = hash of sentence text (Crockford-alphabet, uuid5-like nanoid), unique within the chapter (L40)  {DECRETUM}
  - [ ] B4.2 `.N` disambiguates identical sentences; minted in order, unique across history (L40)  {DECRETUM}
  - [ ] B4.3 paragraph id = merkle hash of its sentences, so it changes when any sentence changes (L40)  {DECRETUM}
  - [ ] B4.4 old ids redirect to the latest version (L40)  {DECRETUM}
  - [ ] B4.5 section and paragraph are optional in an address, but addressable for deep links (L42)  {SUPELLEX}
- [+] B5 Federated by design (L32, L46)  {DECRETUM}
  - [ ] B5.1 not running GitHub; not hosting gigabytes of other people's PDFs (L32)  {DECRETUM}
  - [ ] B5.2 own projects first; later a server binary others run on their own VPS, viewed together through a list of servers (L32)  {DECRETUM}
  - [ ] B5.3 links work across mirrors, giving "a distributed network of texts with links between each other" (L46)  {VISIO-OPERIS}
  - [ ] B5.4 fits the "1.0 and done" product philosophy (L32)  {REGULA > R1}
- [ ] B6 A web app first; desktop via vitrea (L44)  {DECRETUM}
- [ ] B7 Reader (L50)  {VISIO-OPERIS}
  - [ ] B7.1 highlights/favourites; ebook pagination; sentence-by-sentence pagination (L50)  {SUPELLEX}
  - [ ] B7.2 reading groups sharing highlights and notes; multi-tenancy and accounts (L50)  {VISIO-OPERIS}
  - [ ] B7.3 marginalia and glosses; extracts gathered into an "essay" or anthology (L50)  {VISIO-OPERIS}
  - [ ] B7.4 commentary viewable by Bible verse, saint, etc. (L50)  {VISIO-OPERIS}
  - [+] B7.5 something like a social network for readers of scholastic texts, avoiding the pitfalls of social networks (L28)  {VISIO-OPERIS}
- [ ] B8 Issues anchored to the text, the "GitHub" element (L34, L52)  {VISIO-OPERIS}
  - [+] B8.1 issues specific to the text's structure, with a visible history per passage (L34)  {VISIO-OPERIS}
  - [ ] B8.2 see the transcribed Latin or the original PDF page behind any passage (L52)  {VISIO-OPERIS}
  - [+] B8.3 the translation rubric given to the AI is visible, per language and per term (dulia, hyperdulia) (L52)  {VISIO-OPERIS}
  - [ ] B8.4 issues public; a text-administrator side to manage and resolve them (L52)  {VISIO-OPERIS}
- [ ] B9 Publishing: AI-first translation (L54)  {VISIO-OPERIS}
  - [ ] B9.1 tasks for the LLM, tracked to completion; manual translation still allowed (L54)  {SUPELLEX}
  - [+] B9.2 a whole book (sentence metadata, page images…) in one file: sqlite to start, maybe `.brighton` (L54)  {DECRETUM}
  - [ ] B9.3 "mount" the file as a git repo of many files for LLM editing ergonomics, synced back (L54)  {SUPELLEX}
  - [ ] B9.4 publish as a git repo too, for mirroring and history (L54)  {SUPELLEX}
- [ ] B10 Multilingual throughout (L56)  {VISIO-OPERIS}
  - [ ] B10.1 every translated sentence records the rubric version it was translated under, so outdated translations are searchable (L56)  {DECRETUM}
  - [ ] B10.2 rubrics cover terms, Bible quotations, saint names, register… (L56)  {SUPELLEX}
- [ ] B11 Indexing: coverage, entries for people and places, entities deduplicated across works (L58)  {VISIO-OPERIS}
- [ ] B12 Encyclopedia (L60, L72)  {VISIO-OPERIS}
  - [+] B12.1 entries for saints, places, events, works: AI-generated and/or human-edited (L60)  {VISIO-OPERIS}
  - [ ] B12.2 citations deep-link into translated sources (Lapide → St. Basil's 10th sermon), in the reader's language with fallback (L60)  {VISIO-OPERIS}
  - [ ] B12.3 entries carry their own translations and provenance (rubric, date, model) (L60)  {DECRETUM}
  - [ ] B12.4 with expansion it becomes "a proper encyclopedia" anchored in sources also readable in the system (L72)  {VISIO-OPERIS}
- [+] B13 Language learning (L62-64)  {VISIO-OPERIS}
  - [ ] B13.1 not strictly necessary, but Fran is personally interested (L62)  {nulla:ratio}
  - [ ] B13.2 flash cards, games, quizzes, translation exercises graded by the user's own AI subscription (L62)  {SUPELLEX}
  - [ ] B13.3 users share authored materials; "courses" emerge (L64)  {VISIO-OPERIS}
- [+] B14 A standalone desktop app for running such translation projects yourself (L28)  {VISIO-OPERIS}

## III. Beyond texts

- [+] B15 All scholarly domains: math, medicine, sciences, from published books and papers (L68)  {VISIO-OPERIS}
  - [ ] B15.1 notebook-like interactive simulations embedded (L68)  {VISIO-OPERIS}
  - [+] B15.2 "the base computational artifact for any domain would be a single compileable c89 file" (L68)  {DECRETUM > R2}
  - [ ] B15.3 courses that walk through primary sources and papers on a subject (L70)  {VISIO-OPERIS}
- [+] B16 A claim graph or syllogism graph: claims linked to the claims they rest on (L74)  {VISIO-OPERIS}
  - [+] B16.1 shows the point of departure of a disagreement (L74)  {VISIO-OPERIS}
  - [+] B16.2 shows the ripple effect when a claim is verified, disproven or qualified (L74)  {VISIO-OPERIS}

## IV. Lapide: the proving ground and its next pass

- [+] B17 Lapide's translation is complete but a first draft; every reviewed chapter has had issues, often significant (L14)  {nulla:ratio}
  - [ ] B17.1 most issues come from agents taking licence and cutting corners (L16)  {PRINCIPIUM}
  - [ ] B17.2 the work stalled partly because Lapide's scope needs to expand and the approach is unclear (L18)  {nulla:ratio}
- [ ] B18 Floor of quality: compare the original PDF page image with the Latin and the English (L20)  {DECRETUM}
  - [ ] B18.1 page images (~20 PDFs × ~800 pages) outgrow GitHub hosting; needs a deployment held to gitops standards (L22)  {nulla:ratio}
  - [ ] B18.2 keep the minimal UI as default; add a paginated split-view mode (L24)  {SUPELLEX}
- [ ] B19 An annotation-heavy pass (L26)  {DECRETUM}
  - [ ] B19.1 minimum: map Latin and English text to PDF pages (L26)  {DECRETUM}
  - [ ] B19.2 open: spatial mapping to page regions; sentence/paragraph alignment Latin ↔ English (L26)  {nulla:quaestio}
  - [ ] B19.3 tension: batch changes into one pass (each full pass is costly) vs overloading the agent and hurting quality (L26)  {PRINCIPIUM}
- [ ] B20 The same quality standard for many texts, not only Lapide (L28)  {VISIO-OPERIS}

## V. What the text leaves open (illatum: questions the text raises, not states)

- [ ] B21 order of building: which piece first (reader, issues, publishing, addressing)? (illatum)  {nulla:quaestio}
- [ ] B22 how heavy the annotation pass should be (L26 states it as open)  {nulla:quaestio}
- [ ] B23 the design of the paginated mode (L24 states it as open)  {nulla:quaestio}
