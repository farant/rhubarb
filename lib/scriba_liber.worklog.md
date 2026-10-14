# scriba_liber worklog

## 2026-10-08 - S2b-1: the page book (vicus-latera)

The shared-pages model (decisions 14-16 in project-specs/vicus-latera-
plan.md) starts here: pages are NAMED documents in the volume, ONE
`ScribaDocumentum` per name per book, so every scriba view of a page
holds the same object (two instances of one historia in one volume would
write diverging act logs). Index plagula `scriba/paginae`
(`<paginae><pagina nomen=…/>…</paginae>`, order = navigation order);
each page's text in namespace `paginae/<name>`. No page limit (legacy
libro_paginarum capped at 100). New pages are named by the next number
not already used ("1","3" -> "4"). Documents open lazily on first ask.

Lesson: I named a parameter `nomen` - the latina.h macro for typedef -
and clang said "invalid storage class specifier in function declarator".
The project rule exists for exactly this.

## 2026-10-13 — A6c: a book under any prefix (day books)

The index path ('scriba/paginae') and the page namespace ('paginae/')
were constants; they are now fields of ScribaLiber filled by an internal
`librum_aperire`. `scriba_liber_aperire` passes the old paths exactly
(shared vicus book untouched - all old sections pass);
`scriba_liber_aperire_in_spatio(prefix)` uses '<prefix>/paginae' and
'<prefix>/paginae/<name>' - the calendar's day book is
"dies/2026-10-14". Test VIII: day page text is separate from the shared
page '1' and from another day. Plant: prefix ignored -> red.
