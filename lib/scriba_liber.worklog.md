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
