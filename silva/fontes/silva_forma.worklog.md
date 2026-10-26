# silva_forma.c - worklog

## 2026-10-26 — T1: fragment parsing (transformare-plan.md)

`silva_formam_parsare(piscina, textus, mensura, EXPRESSIO|SENTENTIA, &causa)`: holes `$x` ->
`__trf_x`, `$...r` -> `__trfv_r` (string/char literals and /* */ comments untouched; a name used
both ways is refused), wrapped as `vacuum __trf_forma(vacuum) { FRAG; }`, parsed with the compiled
Latin lexicon (`silva_contextus_latinam_addere` - no file reads), then walked by SLOT NAME
(`silva_nodi_locus`, over SILVA_C89_REGISTRUM) to the one statement or its `expressio`.

Things learned on the way (all now in code comments):
- The commission root (`parsura->commissio->radix`) is a LISTA of top-level items (directives,
  latina's declarations, the wrapper definition, the tail) - not a node.
- `argumenta` of a call is a LISTA alternating argument nodes and comma tokens, even for one
  argument - the `$...` hole will match a list suffix.
- An EXPRESSIO wrapper adds `;`, so a statement fragment given as EXPRESSIO parses as statement +
  `sententia-vacua`; empty statements are not counted, so the cause reads "non est expressio".
- Kind names are Latin (`si`, `vocatio`, `binarium` with `sinister`/`dexter`, `folium-integer`);
  ZEPHYRUM arrives as `folium-integer` (post-expansion tree).
Test: silva/probationes/probatio_silva_forma.c (4 sections, 37 assertions). Plants: literal skipping
removed, empty statement counted, '$...' not recognized - each red.
New silva/fontes file -> fontes_generata.h and the amalgam regenerate (fabrica links the amalgam:
rebuild bin/fabrica after).
