# mxl_fontes — sources of the `.mxl` fixtures (plan P3)

Each directory here is the unpacked content of `../<name>.mxl`. The
archives were built once (2026-10-30) with macOS `/usr/bin/zip` 3.0 from
inside each directory; rebuild with the same commands if a source
changes:

```sh
cd mxl_fontes/<name>
rm -f ../../<name>.mxl
# with a mimetype entry (STORED, first - MusicXML 3.1+):
zip -X -0 -q ../../<name>.mxl mimetype && zip -X -D -r -q ../../<name>.mxl . -x mimetype
# without one:
zip -X -D -r -q ../../<name>.mxl .
```

| archive | what it pins |
|---|---|
| `nota_una.mxl` | the canonical layout: `mimetype` STORED first, `META-INF/container.xml` and the score DEFLATE; must read exactly like `nota_una.musicxml` |
| `sine_mimetype.mxl` | no `mimetype` entry (older exporters; the Tallis file has none either) |
| `radix_in_directorio.mxl` | the root file in a subdirectory (`score/x.musicxml`), two rootfiles, the first without `media-type`: the FIRST is the score |
| `opus.mxl` | the root file is an `<opus>`: named vitium (spec §VI.4) |
| `container_malum.mxl` | `container.xml` not well-formed: vitium naming the entry |
| `utf16.mxl` | the root file in UTF-16LE with a BOM: vitium MUSICXML_NON_UTF8 (M13) |
| `radix_absens.mxl` | the rootfile names an entry that is not in the archive |

`utf16/score.musicxml` was written from `nota_una.musicxml` by
`python3 -I` (declaration changed to `encoding="UTF-16"`, BOM `FF FE`,
then UTF-16LE).
