# Fixa crusta — fontes externi

| directorium | origo | versio | plagulae | octeti |
|---|---|---|---|---|
| `freebsd/` | FreeBSD `bin/sh/tests/parser/` (github.com/freebsd/freebsd-src, sparse clone) | commit `3820d1e1a7e08c2c464aca0bdd063b1595dc9127` (main, 2026-09-16) | 89 | 17,297 |

Vendored 2026-09-16 (task P6; Fran approved the fetch). Copied: the
numbered scripts `name.N` only; skipped `Makefile`, `Makefile.depend`
and the `*.stdout` expectation files (they are outputs, not scripts).
Measured at vendoring: no CR bytes, no NUL bytes, every file ends in a
newline. File count and byte count are PINNED in
`probatio_crusta_exempla` and consumed by `probatio_crusta_corpus`
through the manifest `CRUSTA_FREEBSD_PLAGULAE` in
`crusta/fontes/crusta_exempla.c`; a re-vendor is a named change to
both.

These are POSIX `sh` parser tests (aliases, heredocs inside `$( )`,
`case` forms, `for` forms, line continuations, `${ }` corners); they
exercise the parser, not bash extensions. Many are wrapped in
`check '...'` strings passed to `eval`, so their inner text is a
literal word to the parser — the byte law still has to hold across
them.

Licence (the repository's `COPYRIGHT`, first notice, verbatim):

```
The compilation of software known as FreeBSD is distributed under the
following terms:

Copyright (c) 1992-2026 The FreeBSD Project.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
```

The house fixtures `pathologiae.sh` (spec Appendix A.9 plus P6's
additions, one `#### titulus` block each) and `adversarius.sh` (CRLF
throughout, every construct left open, a 200-deep `$( )` nest, a
backslash at EOF) are ours.
