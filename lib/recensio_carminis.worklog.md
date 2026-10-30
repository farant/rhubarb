# recensio_carminis worklog

## 2026-10-23 - one report, two thin writers (ledger wish ...7YT)

Composing vesperae took four separate runs; recensio_carminis_facere
gathers summary, censura, the full render + each track solo, silent bars
(and the score if asked) into ONE struct; _textus (terminal / agents -
few tokens) and _json (MCP: mcp_respondere_json takes a JsonValor*, so
structuredContent is native) both read it. Latin keys (Fran 10-23); the
score is not in by default (bulk). Each finding's JSON 'textus' IS the
censura line, so the two forms cannot drift.

Numbers: JSON values are rounded - and the first cut printed
'-12.200000000000001': floor(x/u + 0.5) * u lands NEXT to the nearest
double; floor(x * n + 0.5) / n gives the nearest one, so the writer's
shortest form is '-12.2'. Test pins both (no '0000000' run; rms equals the
exact rounded double).

The deliberately bad test song also CLIPPED for real (50 samples, +0.4
dBFS: triangle B2 + aether peaks aligned at 120 bpm) - monita came out 3,
not the 2 I designed. The report was right; the fixture now asserts all
three. Same parts at 84 bpm (vesperae) peak -1.8: the mixer has no
headroom policy - filed as a ledger wish.

## 2026-10-24 - limiter in the report; warning only beyond -3 dB

RecensioCarminis.mixtura = the render census. Sound line: ", limitator N quadra (-x.x
dB)" when the limiter engaged; JSON sonus.limitata / reductio_db. Warnings: + 1 if the
reduction is deeper than -3 dB (RECENSIO_REDUCTIO_MONENDA 0.708): a mix genuinely too
hot. Light limiting (chance peak alignment, the 'mala' fixture's +0.4 dB) is named, not
warned - 'mala' went from monita III to II (no clipping any more). New fixture 'calida':
three triangulum hard left (~1.8, -5 dB) = monita 1.

## 2026-10-26 - silent part per part (desideratum ...JZ3TS0T)

recensio already rendered every part alone; it only ran auscultatio_tacitae on the WHOLE
mix, so a part with an unknown instrument (pizzicato_gravis outside completorium) showed
-120 dBFS but "tacitae: nullae, monita 0" whenever other parts sounded. Now per part:
auscultatio_partis_tacitae(carmen, pars, solo summary) - bars where that part (not muted)
has a sounding note but its solo render is silent. RecensioCarminis.tacitae_partium[i]
{mensurae, numerus, tota}; tota = the solo render is silent overall (rms < TACITUM) - the
usual sign of an unresolved instrument. Text: part line gains "  TACET 3 7-9" (ranges
compressed) and " (tota: instrumentum?)"; JSON part object gains "tacitae": [..].

Monita: one per silent part - BUT only if it has a silent bar the mix-level tacitae does
not already name (deviation from the proposal, told Fran): the old 'mala' fixture's part
'nullus' (unknown instrument, only bar 4) made bar 4 mix-silent too, and counting both
reported one problem twice (monita 2 -> 3). The TACET note still prints.

Note: mix-level silence and part-level silence measure different things - a muted part is
never 'supposed to sound'; rest bars are not flagged (test: c1 | r1 | r1 | r1).

Plants 8/8 red on the first round (the rest-bar test was added before plants, from the
plant list: a check that dropped the 'has a note here' condition would flag rest bars).
