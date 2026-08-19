# silva_nodus worklog

## 2026-08-19 — provenance queries: est_fons_purus + geometria_fida (01M0D4QV5S part 2)

Born from the formator's five hand-rolled guards (evolutio III): extents
over macro-expanded subtrees return plausible-but-degenerate geometry and
consumers couldn't detect the lie. Two queries, not one, because the
obvious one is nearly useless here:

- **est_fons_purus** — every token in the subtree is origin-FONS
  (stratum 0). Clean semantics, but in latinized code EVERY statement
  keyword (`si`→`if`) is an EXPANSIO token, so statement-level subtrees
  are almost never pure. Still the right building block: strictly pure
  subtrees carry their own geometry, which the RELEXATION PROPERTY
  exploits (probatio section VII) — relex the byte extent, the non-trivia
  token series must match exactly. That's a classification oracle the
  byte-roundtrip gate structurally cannot provide.
- **geometria_fida** — the query the formator actually needs: are the
  ROOT-ORIGIN SITES of the subtree's tokens DISTINCT? A 1:1 expansion
  (latina keywords) maps one token to one true invocation site — honest.
  A 1:N expansion collapses N tokens onto ONE site — the lying-extent
  class (flatura −21 gaps, coloratio mid-call cb). Definition: collapse
  = two DISTINCT token objects claiming the same (fons, byte) root site.

Design traps resolved:

1. **AMBIGUUS arms share token OBJECTS** (GLR forks read one stream), so
   a naive same-site check would false-positive on every ambiguous
   subtree. The collapse test therefore compares token IDENTITY within a
   same-site run: same pointer twice = ambiguity sharing (harmless);
   distinct pointers = real collapse. Sort by (fons, byte) via
   xar_ordinare, adjacent scan — any run with ≥2 distinct tokens has an
   adjacent distinct pair, so adjacent compare suffices.
2. **Synthetic radix** (byte_offset < 0, e.g. ORIGO_API) = FALSUM
   immediately: the site is unknowable, geometry has a hole.
3. **Other-file radices** (fons_index >= 0 given): EXCLUDED, not fatal —
   they make no geometric claims in the judged file (praebere'd headers).
4. **-Wcast-qual**: the nodus-level entries loop loci themselves rather
   than wrapping through silva_valor_nodus (which takes non-const).

Live confirmation the class is real: the new probatio file itself, being
full of CREDO_* macro invocations, drew 46 FALSE spatium-definitionis
rows from the formator's R2-call rule (gap −15) the moment the post-edit
hook linted it — the first named adoption site for geometria_fida.
