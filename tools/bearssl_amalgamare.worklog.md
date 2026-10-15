# bearssl_amalgamare worklog

## 2026-10-15 - why an amalgamation, and how collisions are handled

Five independent build paths (aedilis X.h->X.c, fabrica per-fons flags,
compile_tests.sh VENDOR_SOURCES, ludus_tessera hand list, silex
vendor/<basis> closure copy) all assume one vendor header + one source
(the sqlite3 shape). BearSSL is 294 sources. A naive unity build fails:
alternative implementations reuse static names (api_mul, P256_P,
t0_codeblock, typedef p256_jacobian...).

The generator asks clang (`-ast-dump=json`) for each file's OWN
file-scope names (static funcs/vars, typedefs, tags, enum constants;
decls from included files skipped via includedFrom/expansionLoc
tracking - spellingLoc first, expansionLoc wins). Names defined in >1
file get `#define n n__<file>` ... `#undef n`; each file's own textual
#defines are #undef'd after it. Measured on 7bea48e: 435 renames, 455
undefs; `test_crypto all` (88) + `test_x509` pass against the result
(gate `bearssl`). `#line` directives keep diagnostics on the real
upstream path.

Header macros: some files REDEFINE an inner.h macro identically (ARSH in
ec_c25519_m15/m31, ec_p256_m31 = inner.h:1030/1033, legal C). Undefining
those after the file would strip the macro from every later file, so
macros the headers define are excluded from the undef list - but only
if the file's definition is textually one of the header's (whitespace
normalised, all #if variants); a DIFFERENT redefinition, or an #undef
of a header macro, fails the generator (amalgam would lie).

Output name `bearssl_amalgama.{c,h}`, NOT `bearssl.h`: silva's tools
(examen, censor, nexus_percursus, ... - 12 copies of the same walk)
collect every .h in the tree and resolve a quoted include by BASENAME,
first-in-walk wins; `vendor/bearssl/inc/bearssl.h` was reached first and
bypassed the C89 `inline` shim (examen REICE on `static inline`). A
unique basename sidesteps it in all twelve without touching them; the
walk itself is ledgered as tool debt.

FABRICA_SCRIPTURA honoured (inputs from the tree, outputs under the
scratch root) - without it fabrica judged IGNOTUM "generator nihil
scripsit".
