# xml_coctum worklog

## 2026-11-03 — text and attribute values as slices; sized lists (memory pass, task 4)

A research agent charged every allocation of `xml_coquere` to its
source line on Reger op. 74 (15.8 MB XML, 418K elements, 788K text
nodes): XmlCoctus structs 147 MB, text builders 78 MB (two allocations
per text node, even for one plain token), eager attribute Xars 66 MB,
child Xars 47 MB, attribute-value builders 10 MB. The biggest item, the
diagnostics "seen" set (344 MB), was fixed in materia instead
(materia_diagnostica worklog, 2026-11-02).

Four changes, no API change:
- **Text without copying.** `_textum_simplicem` keeps the first piece of
  a text node as a slice (`Coquina.simplex`) of the source or of the
  entity's replacement text, both of which live as long as the
  document. A builder is created only when a second piece arrives (it
  copies the slice in first). A piece that contains '\r' still goes
  through `_lineas_appendere` (CR/CRLF -> LF), so only already-normal
  text is sliced. Every read of the pending state goes through
  `_textum_fundere` / `_textus` / `_textum_simplicem`, and Coquina is
  memset to zero.
- **Attribute values as slices** when the raw value has none of `& <
  \t \n \r`. `_valorem_normare` only acts on those bytes (references,
  the '<' error, whitespace normalization), so the value is unchanged
  and its diagnostics are the same. `_contrahere` (non-CDATA declared
  attributes) builds a fresh copy and never writes into its input.
- **Attribute lists lazy:** NIHIL when an element has none, otherwise
  sized to the next power of two of the count. A defaulted attribute
  from `<!ATTLIST>` creates the list on demand. All readers already
  handle NIHIL.
- **Child lists pre-sized** from the materia node's child count (next
  power of two).

Values and text now point into the source text more often. That's not
a new requirement: the header already says the source must outlive the
document, and element names already pointed into it.

Reger: xml_documentum 742.2 -> 601.5 MB, 10.71M -> 8.20M allocations;
musicxml_legere 690.2 MB. The agent checked the cooked output
byte-for-byte (tree dump, diagnostics, notations) on all 3,210 W3C
files with namespaces on and off under ASan. Golden: usus_coquendi
lower on all three documents (partitura -12%).

Plants: (A) slice text even when it contains '\r' -> probatio_xml_coctum
(CRLF case at :347), xml_differentia and xml_oraculum FRACTA; (B) treat
'\t' as plain in attribute values -> xml_differentia (expat over the
wild corpus) and xml_oraculum (W3C) FRACTA.
