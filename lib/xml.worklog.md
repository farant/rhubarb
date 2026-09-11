
## 2026-09-11 — no DOCTYPE branch: a real `Info.plist` could not be parsed

Found while designing plist (`project-specs/plist-spec.md` §6.1) by
reading, not by a failing test. `xml_legere`'s prolog loop skipped an
XML declaration (`<?xml …?>`) and comments, then fell straight through
to the root element — there was no branch for a doctype declaration.
Every Apple-written plist carries one, so `xml_legere` refused all of
them with `XML_ERROR_SYNTAXIS`, and nothing in the suite noticed
because no fixture had a DOCTYPE.

Added the skip in the PROLOG ONLY. A doctype is legal only before the
root element, so the other declaration skip (inside `_legere_elementum`)
was deliberately left alone.

**The doctype's internal part is REFUSED, not skipped.** `<!DOCTYPE x [ <!ENTITY
a "b"> ]>` cannot be skipped by scanning to the first `>` — that `>`
sits inside the subset, so the parser would resume mid-declaration and
mangle the rest of the document. New status
`XML_ERROR_PARS_INTERNA` (appended, value VIII — never reorder this
enum) names it. Refusing a construct we cannot skip correctly beats
silently mis-parsing it.

Calibration: blinding the `[` test made the refusal assertion at
`probatio_xml.c:668` go red — the malformed document was accepted —
and green again after the revert.

## Measured the same day and deliberately NOT fixed

**Numeric character references are not decoded.** The entity decoder
handles exactly the five named entities; `lib/xml.c:951-957` copies any
unrecognized entity through verbatim, so `&#65;` survives as five bytes
instead of becoming `A`. Apple's writers use numeric references for
control characters, so a plist carrying one reads back wrong —
silently, since nothing refuses it.

Parked in `plist-spec.md` §9 with its pull condition (a fixture that
actually contains one). The write side is closed differently: plist
REFUSES a `<string>` holding a C0 control character rather than emit
something this escape layer cannot express.

Note for readers of the diff: this was `lib/xml.c`'s first pass under
the formatter, so the commit carries many `cosmetica` rewraps beside the
two substantive changes (`XmlStatus`, `xml_legere`). One pre-existing
call near `:1234` came out with odd continuation alignment — inherited
from its previous layout, valid C, not touched by hand.
