# Annotationes STML — Chunk 0 ceremony interview (raw Q&A)

2026-07-21, after the four-document reading (STML_SPEC → XML
language → intent essay → speculum-lingua v0.5.3) and the
Intentional Software brainstorm. Park: 01KY3D7EJP.

**Q1. Anchoring — how does the collector recognize an annotation
comment?**
A: **Tag at comment start.** A comment whose first non-whitespace
content is an STML open tag IS an annotation; anything else is
prose. (Aedilis Phase-0 lesson: anchored, never substring; prose
essentially never begins with '<'.)

**Q2. Identifier format?**
A: **ULID, shared with the tabularium** — one identity space for
board res and code nodes, one prefix-resolution discipline
(unambiguous ≥6 chars), timestamps encoded (naming-time
provenance for free).

**Q3. Name of the identity attribute/element?**
A: **nid** — kept as an untranslated technical term (offset
precedent); short because it is the most-typed token in the
system.

**Q4. First adoption class?**
A: **On-demand only.** No required class at birth; identity is
minted where something wants to anchor. Presence-lint starts
verdict-neutral with zero required classes; classes are added by
decretum as demand appears.

**Q5. Rationale element name?**
A: **intentio** — matches the design lineage end to end.

**Q6. Multi-line annotations?**
A: **Both registers from day one.** Decoration pre-pass specified
now: strip leading whitespace + '*' + one space per continuation
line before the STML reader sees the text.

**Q7. Attachment rule?**
A: **Above target, blank-skip** — annotation binds to the NEXT
top-level unit, skipping blank lines (the EXSPECTA-PROXIMA
precedent; matches how aedilis/TOLERA already sit). No following
unit = file-scope annotation.

**Q8. Where does minting live?**
A: **Explicit instrument** (generare.sh-class; the old STML_SPEC
boolean-nid-filled-on-format design). No hook writes source;
hook integration later on-pull if the manual step chafes.
