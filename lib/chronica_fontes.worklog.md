# chronica_fontes worklog

## 2026-10-30 - C2 built (Opus 5.5)

**Shape.** One conspectus per book (`eventa_comitis/<locus>`,
`eventa_libri/<pagina>`), each computing that book's events
(`EventaLibri{res}`). The master `ChronicaTabula` is the union, copied
into its own piscina, rebuilt only when an auditor reported a key
since the last build. Unchanged books stay RECENS, so a rebuild only
recomputes the book that changed.

**Why state lives in ChronicaFontes, not the registry.** The plan had
`chronica_tabula(ConspectusRegistrum*)`, but the registry is opaque:
there was nowhere to keep the cached table or the stale flag. Fran
approved an opaque `ChronicaFontes` (creare / registrare / tabula /
destruere) instead.

**Declaration names use '_' not '-'.** tools/conspectus_infigere.sh
builds the C identifier `_declaratio_<file key>` from the file name,
so a hyphen won't compile, and every declaration's `nomen` equals its
file key. So the names are `eventa_comitis` and `eventa_libri`.

**No enumeration from conspectus.** `conspectus_enumerare` only serves
`<quaerendum>` declarations, and these must stay out of search. The
build lists books itself: a plagula scan for `comes/*/paginae` (sorted
by bytes) plus `scriba_liber_nomina_legere(NIHIL)` for the common book.

**Copy, never point.** A per-book value lives in its conspectus
piscina, which is freed on recompute OR eviction by the byte cap
(`eicere`). Reading book N can evict book K, so the table copies each
book's res the moment it reads them.

**Long keys (> ~240 bytes).** conspectus builds `nomen/clavis` in a
256-byte buffer: `conspectus_legere` returns NIHIL for longer keys,
and `clavem_irritare` skips the cache entry, though it still calls
the auditor. Without help, a long Wikipedia locus would silently
vanish from the timeline. Fix: when `conspectus_legere` returns NIHIL,
`librum_legere` computes the book directly into the temp piscina.
Correct because the auditor still fires for long keys. Test 'clavis
longa' (280-byte prefix) covers it.

**Pointer test needs fresh addresses.** 'recens' asserts the same
pointer when nothing changed. A rebuild allocates the NEW piscina
before destroying the old one, so a rebuilt table can never reuse the
old address. Otherwise the 'semper reaedificat' plant could pass by
malloc luck.

**Lines.** `scriba_liber_paginam_legere_omnes` keeps blank lines (and
lines of only spaces, which come back as empty chordas) so that
`exitus[i]` is page line i. Columns are scriba CELLS (tabs become
spaces cell by cell); the C1 header says "octetus". They match for
ASCII. A multi-byte character in the grid may shift columns. That
matters only for C6 colouring and C4b landing; check there.

**Not tracked.** Writing the companion note (`comes/<locus>/comes`)
does not touch `comes/<locus>/paginae`, so a title change alone
leaves `titulus_libri` stale until the next page edit in that book.
Pages are written after the note in practice. Scriba has no page
deletion or rename, so a page vanishing without its text plagula
changing can't happen today.

Plants (7, all red, files IDEM): auditor ignored; common book not
enumerated; order ignores month; title not trimmed; always rebuild;
long key without direct compute; blank lines dropped.
