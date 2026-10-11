# portae worklog

## 2026-10-10 - T1: declarations reader (portae-declaratae plan)

`portae_declaratas_legere(sutura, piscina, &causa)`: every candidate file
the sutura enumerates is read; a cheap prefilter (`chorda_continet` on
"<porta") skips files without declarations; the rest go through crusta
(`crusta_arbor_parsare`, CRUSTA_BASH) and `materia_annotationes_colligere`
with the crusta lexicon ratum and "#" - so a `<porta>` inside a quoted
string or plain prose is never a declaration (tested). Elements named
`porta` in an annotation's document become PortaDeclarata; vocabulary is
CLOSED (titulus manu causa tegit currit). After ALL files: registry
checks (title outside PORTAE, alien runner, duplicate title, PORTAE key
with no annotated runner). Refusal returns NIHIL with every cause, one
line each, `via:linea: ...`.

Bug caught by writing the cross-file test: the file loop originally ran
`while numerus == 0`, i.e. stopped reading files after the first problem
- "all causes" held only within one file. The guard was meant only for a
broken foundation (lexicon, enumeration) - now an explicit flag. Plant
(old condition back) turns the new section red.

Traps hit: `registrum` is a latina macro (register) - field renamed
`tabula_portarum` (my blunt rename then also rewrote a test's EXPECTED
string - the test failed for the test's fault; a debug imprimere showed
it). Wrote a private substring helper twice before noticing
`chorda_continet` exists - replaced.

Dogfooding (principle …Z2Y555, checklist lapide FR-008 "scriba"):
- building cause messages piecewise with chorda_aedificator
  (appendere_chorda / literis / sprintf of a line number) is the main
  verbosity - FR-008's pool formatter with a `%C` verb for chorda would
  turn `_causa(a, &n, via, linea, "titulus '", t, "' bis declaratus")` into
  one format call;
- compile_tests.sh's -I list says it mirrors aedilis.stml <inclusa> but is
  a hand-kept SUBSET (7 of ~30 dirs): lib/portae.c needed -Icrusta/fontes
  added by hand. Derive it from aedilis.stml (one source) - candidate.
