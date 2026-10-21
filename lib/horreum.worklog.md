# horreum worklog

## 2026-10-21 - T2: the pure core

- **Signed path == wire path by construction.** The signer gets the
  DECODED path; the URL gets `signatura_aws_codificare(via, VERUM)` -
  the same function the signer uses - and the wire query is the
  canonical query (line 3 of the canonical request). The test does not
  peek at internals: it rebuilds the signature from the wire request
  (`http_petitio_visus`: decoded path, decoded query, headers named in
  SignedHeaders) the way a server would, and compares Authorization.
  That helper is the seed of T3's fake-server verifier.
- **`http.c` passes the URL path through verbatim** (`_parse_url`
  keeps everything after the host) and adds `Host` only when absent,
  so horreum sets `Host` itself (with `:port` for generic) and the
  signed host matches.
- **`xml.h` decodes named entities but not numeric references**
  (`&#x1234;`, `&#13;` stay raw); horreum's `_textus` decodes them to
  UTF-8 (plan P15). Caveat: a key whose literal text is `&#x41;`
  arrives as `&amp;#x41;`, which xml.h turns into `&#x41;` and horreum
  then decodes once more - indistinguishable after xml.h; accepted.
- **Profile errors and secrets.** The obvious rule "name the unknown
  key" leaks: in the old positional format (bare id / secret /
  endpoint lines) the secret IS the "key". An unknown key is named only
  when it is `[a-z_]` and at most 16 chars; otherwise the message gives
  only the line number. Planted red (`profilum_vitia` -> renamed
  `tabella_vitia`).
- **Renamed for the Latin lint:** profile -> `tabella`
  (`horreum_ex_tabella`, "tabella" = a small written tablet);
  `praesignare` kept (classical *praesigno*) with a glossary entry.
- `magnitudo` (the sizeof macro) slipped in as a parameter name;
  examen caught it before the compiler did.
- The `secretum` and `clavis_id` live in fixed arrays inside the
  `Horreum` struct so `horreum_claudere` can zero them; the test scans
  the struct's memory after close (allocated in a fixed-size piscina so
  the scan stays inside the arena).
