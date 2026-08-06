# natura.worklog.md (lib/natura.c - onerator)

## 2026-08-06 — the loader shrank to its irreducible core

Rules V, VII (format half), VIII, XII and XV migrated OUT of this
file to natura/natura.canon (judged inside natura_examen via
lib/canon.c). What remains is exactly what no schema can hold:
cross-file resolution and inheritance — rules 2-4, 6, 7-ordering
(cross-attribute comparison), 9-11, 13, 14, 16, 17.

Post-migration contract, stated in the header comment: the loader
PRESUMES canon-sane input. Unknown elements are silently walked
through; a nameless genus/species is silently skipped (it cannot
be registered); a duplicate name keeps first-wins registration
(graph integrity) with NO diagnostic — the canon shouts, the
loader loads. Anyone running the loader WITHOUT canon judgment
(i.e. not through natura_examen) gets no layer-2 protection: that
is by design, not an oversight.

Deleted infrastructure that only served migrated rules:
ELEMENTA_NOTA/ATTRIBUTA_NOTA + in_literis (rule VIII),
machinam_probare whole (rule XII), claves_fontium table + its
build-time insertion (rule V), dies-format diagnostics (rule VII
format; dies_bene_formata SURVIVES as a guard on the ordering
check — comparing malformed dates means nothing).

probatio_natura: the vitiosa fixture's rule-VII fault changed from
a malformed date (now canon's) to an INVERTED interval
(valens_a="2020" valens_ad="1999") so the loader's surviving half
stays covered. Totals 15 → 11 vulnera.
