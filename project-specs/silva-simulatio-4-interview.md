# Silva Simulatio ④ — Post-Simulation Interview (Raw Q&A)

*2026-07-02. The final M0 interview — three questions from the amalgamation
register (silva-simulatio-4.md).*

---

## Q: friatio subsetting (S44): once internals go static, friatio's unused SHA1/SHA256/CRC32 become -Wunused-function → -Werror failures. Silva needs only fnv1a, fnv1a_literis, djb2. How is the trim done?

**A:** Manifest function exclusions. The amalgamator manifest lists excluded
functions per file; the assembler drops their definitions and prototypes during
assembly. The canonical lib/friatio.c stays untouched; the mechanism generalizes
to any future partially-needed vendored lib.

## Q: silva.h boundary (S43): confirm plain C89 spellings throughout — including documentation comments and examples — with latina strictly internal to silva.c?

**A:** Confirm: vanilla C89 boundary. silva.h reads like a normal C library
header to any host (the SQLite experience); latina's 388 defines never cross the
boundary; a generated host-pollution test (host file declares variables named
si, per, character after including silva.h) enforces it forever. Internal silva.c
code keeps full latina idiom.

## Q: Vendored include guards (S46): keep verbatim or uniquify?

**A:** Keep verbatim. Zero transformation, and the collision with rhubarb's own
headers is a FEATURE: a host that links rhubarb libs can't double-define. The
macro names are obscure enough that accidental host collision is negligible.
