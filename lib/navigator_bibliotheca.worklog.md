# navigator_bibliotheca worklog

## 2026-10-27 - N2.4 (secunda)

Saved articles + versions on the volume. Truth = two act genera
(`navigator-articulus`, `navigator-versio`, JSON datum); raw HTML = a
massa named by its SHA-256 in the version act (communio's "64-hex in
datum names a massa" rule makes it travel with the act later). The index
is rebuilt from this volume's OWN acts (`volumen_acta_generum_replicae_
legere` with `volumen_replica`, per the volume's rule for local state)
and every write re-reads acts after the last seq - one code path, so the
index cannot drift from the log.

"New version?" re-extracts BOTH the fetched HTML and the latest stored
HTML with the caller's adapter and compares `articulus_sigillum`. Both
extractions use the CANONICAL url as `fons`: the final URL after
redirects can sit in another directory (`/w/...`), and relative links
would then resolve differently and fake a content change. A plant that
used url_finalis stayed green until the test's second offer also used a
non-canonical final URL - the first offer alone never compares.

Extraction runs in a throwaway piscina (`ALVEUS_EXTRACTIONIS`, 4 MB
first block) destroyed after the offer: a 1.3 MB article's tree would
otherwise pile up in vicus's long-lived arena on every refresh.
Comparison is with the LATEST version only (A -> B -> A gives three
versions). If the latest massa is missing or the adapter now fails on
it, the offer is stored as a new version (cannot compare = keep).
