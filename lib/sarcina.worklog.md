# sarcina worklog

## 2026-10-23 — L1: a read-only ZIP reader for EPUB (lector spec)

`sarcina` reads a ZIP archive held in memory: finds the End Of Central
Directory record, parses the central directory into an entry table,
returns any entry's bytes (stored copied, DEFLATE via
`flatura_inflare`), length and CRC checked. No ZIP64, no encryption, no
other methods, no writing - each refused by name.

Format notes that matter:
- The EOCD is NOT always the last 22 bytes: an archive comment of up to
  0xFFFF bytes may follow. Search backwards over that whole window.
- Sizes and CRC come from the CENTRAL directory, always. With a data
  descriptor (general-purpose flag bit 3) the local header holds zeros
  and the real values follow the data.
- The data starts at local header + 30 + LOCAL name length + LOCAL extra
  length. The local extra field can differ from the central one:
  macOS `zip` writes 28 bytes locally (UT + ux) and 24 centrally. Using
  the central length reads 4 bytes too early.
- Every read is bounds-checked (`binos_legere`/`quaternos_legere`), so a
  lying offset or size is refused, never followed. The explicit
  "central directory outside the archive" check is therefore a second
  guard (its plant stayed green); it stays for the clearer message.
- Each entry's general-purpose flags stay private (an internal struct
  wraps the public `SarcinaIntroitus` as its first member), so the
  approved header did not need a flags field.

Testing lesson: a reader tested only against archives from its own
test writer shares that writer's assumptions. Our writer never emitted
extra fields, so ignoring the LOCAL extra length passed every test
built in memory. The committed fixture `probationes/fixa/sarcina/
exemplum.zip` is made by macOS `zip` WITHOUT `-X` (with `-X` the extra
fields are stripped and the fixture would prove nothing); the same
plant goes red on it.

Real book (2026-10-23, manual, not committed): Gutenberg #394 Cranford,
`https://www.gutenberg.org/ebooks/394.epub3.images` ->
`~/.rhubarb/codices/cranford.epub`, 8,753,733 bytes: 134 entries, all
read with CRC OK, 0 failures, 9,056,168 bytes uncompressed (54 stored,
80 DEFLATE); `mimetype` first and stored; `META-INF/container.xml`
points to `OEBPS/content.opf`.
