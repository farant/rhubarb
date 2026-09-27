# http.c worklog


## 2026-07-17 — receive-loop error path was a live crash vector

VANA adjudication find: http_exsequi's receive loop read
tcp_recipere/tls_recipere into unsigned `i32 n`. A transport error
mid-response returned -1 = ~4e9, which PASSED `si (n > 0)` as
"received data" and fed memcpy a (size_t)4e9 length from a 4KB
buffer. Not a dead check — an actively wrong one. `n` is now s32,
errors exit the loop; the positive-n accumulations cast back into
the i32 totals explicitly. Also removed the dead
`max_redirectiones < 0` validation (unsigned param, plain count
semantics, no callers pass negatives — the check could never fire).

## 2026-07-20 — hospitium Phase A: parser guards, lenient-LF, limits, serializer

Guard design that matters: status_suggestus is a FIELD on
HttpParseResultus (and the parser), NOT a new HttpParseStatus —
phases stay a state machine, the guard class is orthogonal
metadata, and every existing phase consumer compiled untouched.
Helpers set parser->status_suggestus and return FALSUM; the two
FALSUM branches in adicere mirror it into the result with a CD
default. Guards: 501 unknown method (O3 hard at parse), 400
version (replaced a SILENT default-to-1.1 — "GET /path" with no
version used to parse fine), 400 dup-CL and CL+TE in both arrival
orders via vidit_* flags, 413 announced-CL fail-fast (fires at
header parse, before any body buffering), 411 chunked (the old
TODO stub silently declared COMPLETA and discarded the body).

Lenient-LF: _parser_invenire_crlf → _parser_invenire_lf; the
'\r' is stripped RETROACTIVELY by the caller (linea_finis--), so
a CRLF split across two adicere feeds is safe — we wait for the
'\n', the '\r' is already buffered. The two parse helpers needed
zero changes: their param was already an exclusive content end.
Dropped the old <II early-out — a lone '\n' at position 0 is a
valid (empty) line.

Limits: total cap check is `longitudo > petitio_maxima -
buffer_mensura` — that subtraction is safe ONLY because the check
itself maintains the invariant buffer_mensura <= petitio_maxima
(i32 is unsigned here; the naive `mensura + longitudo > maxima`
form can wrap). URI cap is safe because uri_finis >= uri_initium
is guaranteed by the empty-URI check above it.

Reliquiae: made end-of-request honest first (cursor +=
content_length before COMPLETA) — before this, cursor stopped at
body start and buffer_mensura - cursor would have counted the
body itself as "leftover".

Serializer: one _serialize_impl; the 2-arg form is (FALSUM,
FALSUM), _cum_conexione is (VERUM, keep_alive) [O2 additive].
CL is now ALWAYS emitted (even 0 — the real keep-alive framing
fix) unless _http_corpus_prohibitum (1xx/204/304) [O1]. Caller
CL/Connection headers are skipped in the user-header loop —
serializer owns framing. Existing-caller bytes unchanged in
practice (all six either have bodies or are 204).

reset stays UNUSED by hospitium (reficere + fresh creare per the
memory story); it now clears status_suggestus + vidit_* flags but
NOT the two config fields — limits survive reset by design.

## 2026-07-28 - ansa hauriens iudicat + involutio scansionis

Arcus http/tcp (commissum 30190d6, res 01KYANH7AN/01KY05Q8AH/
01KYJS4YKW). Non-obvia pro lectore futuro:

- La ansa hauriens exit tribus modis (0 EOF / TCP_ITERUM tempus /
  -1 error) et POST ansam iudicatur - ante hoc omnes tres
  'successus' erant cum corpore quocumque. Corpus partiale
  CONSULTO abicitur: vocator (villa) condicionem lentam deprehendere
  vult, non corpus mendax. TLS: tls_recipere -1 solum novit, tempus
  TLS ut HTTP_ERROR_IO apparet (v1).
- Scansio capitum: forma 'i + III < total_size' NON aequivalens
  'i < total_size - III' - total_size insignatus, responso I-III
  octetorum subtractio involvitur et scansio extra fines currit.
  Forma additiva etiam invariantem constituit body_start <=
  total_size (tolera sedis body_len ei innititur).
- _chorda_ad_i32 saturat (non involvit) - custodes supra certo
  accenduntur. Familia lint: comparatio contra subtractionem
  insignatam (sine zephyro) codici 80 invisibilis est.
- Fixtura prava (probatio_http.c): servus crudus DEBET (1) accipere
  BLOCANS petere - defaltum non_blocans ante clientem pollit,
  'nullae connexiones' videt, exit, et cliens RST accipit; (2)
  petitionem TOTAM haurire ante clausuram - octeti illecti RST pro
  FIN gignunt.

## 2026-09-26 - corpus brevius quam promissum = ERROR (lapide bugs/015-016)

Causa radicalis in tls_macos.m (vide eius worklog): tls_recipere 0
reddebat pro WouldBlock. Hic defensio in profundo, ut quaecumque
causa truncationis futura numquam successum mendacem gignat:

- Content-Length > octeti recepti -> HTTP_ERROR_IO, "Corpus
  truncatum: Content-Length N, recepti M (corpus partiale
  abiectum)". DECISIO 07-28 INVERSA: probatio_truncatum_eof EOF
  mundum post X ex C octetis ut successum asserebat ("EOF = terminus
  legitimus"). RFC 9112 6.3 aliter: nuntius incompletus. Et doctrina
  eiusdem arcus ("corpus mendax peius quam error honestus") ipsa id
  vetat. Content-Length saturatum (probatio_caput_ingens) nunc
  quoque error est.
- chunked sine fragmento '0' -> error (olim "copiare quod possumus").
- chunked: magnitudo hex ante multiplicationem custoditur. '100000005'
  in i32 ad V involvebatur -> 'salve' ut corpus validum; 'FFFFFFFF'
  custodiam 'i + chunk_size <= len' ipsam involvebat (memcpy extra
  fines, hospite maligno). Nunc forma subtractiva + custos.
- HEAD et 1xx/204/304: nullum corpus per legem, Content-Length non
  iudicatur (_http_corpus_prohibitum ante http_exsequi motum).

Plantae omnes rubrae (Content-Length, completa, custos involutionis).
Nondum factum, notatum: HTTPS tempus petitionis (petitio->tempus_ms)
ignorat - tls_connectere optiones defaltas (XXX s) semper accipit.
