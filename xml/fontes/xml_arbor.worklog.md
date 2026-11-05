# xml_arbor worklog

## 2026-11-02/03 — exact-size child lists (memory pass, task 3)

The builder appended each child straight into its parent's list slot
(`materia_nodus_appendere`), so every list was a Xar growing in
segments of 4: a 64-byte header plus a 96-byte first segment even for
one child. On the largest MusicXML score (Reger op. 74) 61% of lists
hold a single element.

Now every open level (GradusArboris) records `basis`, the height of a
shared scratch stack (`Aedificatio.liberi`, MateriaValor) when the level
opened. Children go onto that stack. `_figere` copies the level's
pending children into an exact-size Xar and sets it into the node's
slot, then truncates the stack back to `basis`. It runs wherever a
level's list slot would change or the level ends:
- the three genus changes (open tag finished `>`, empty-element `/>`
  path, closing tag begins `</`), because `_locus_liberorum` depends on
  the genus (attributes while in the tag, children after);
- `_extollere` (pop);
- after the parse loop, for levels still open (the document level
  always, unclosed elements on bad input). This runs BEFORE
  `_ordinem_iudicare`, which reads the document's child list.

New invariant: a node's child-list slot stays empty until its level is
flushed. Nothing reads it mid-parse (checked: the only reader of a
list, `_ordinem_iudicare`, runs after the final flush). Token lists
(`XML_MALUM_TOKENS`, `XML_DECLARATIO_PARTES`) are separate slots and
still append directly, so no ordering mixes.

Also: `_solvere`'s temporary trivia array uses an 8-slot stack buffer
instead of the arena when it fits (the trivia setters copy it).

`_locus_liberorum` returns s32 now. It used `(i32)-1` as "no list" and
callers tested `< 0`, which is never true because i32 is unsigned
(examen flagged it; harmless only because materia_nodus_ponere refused
the huge slot index downstream).

Reger: materia tree 434.5 -> 392.2 MB, 6.40M -> 5.99M allocations;
xml_documentum 742.2 MB. Golden (xml computus): partitura -1 KB and -8
allocations; the two tiny documents grow (dtd +264 B, invalidum +48 B
and +1 allocation) from the scratch stack's fixed cost.

Plant: skip the flush when a closing tag begins -> 9 of 12 xml suites
FRACTA (api, arbor, canon, ...). Found and measured first by the
xml-arbor-memoria research agent.
