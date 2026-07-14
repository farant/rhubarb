# INSTRUMENTA-SIMULATIO-3 — INTERVIEW (MCP LEGATI)

2026-07-14, immediately after the sim. Twelve questions in three
rounds; every recommendation accepted. Raw record — the spec
(mcp-legati-spec.md) is the authority; this file preserves the
reasoning as asked and answered.

## Round 1 — fork-value questions from the sim seeds

**Q1 (C11, unit split).** ~1,000 new lines push legatus.c to ~5,600.
Organize how?
**A: v0 in-file, split later.** Banner sections; split when LEGATUSD
forces the seam anyway. The interior-function reuse (outgoingCalls
consumed six interior helpers) makes extraction ceremony expensive
right now.

**Q2 (language).** Tool descriptions + result text are agent-facing.
**A: Pure Latin.** House rule — these are messages. Tool names
(symbolum, vocantes...) are self-describing; the doctrine stays
uniform.

**Q3 (examen parity).** diagnostica output format?
**A: Exact examen parity.** ACCIPE/REICE verdict + positioned rows,
the shape the agent already reads from the bench;
differential-testable against examen.sh on clean trees.

**Q4 (caps).** Result-size policy for list-shaped answers?
**A: Fixed caps, verbosum parked.** Nexus-style grouping, per-file +
total caps as v0 constants, explicit "et alia N" tails (no-silent-
caps law). A verbosum/plenum knob becomes a named park if caps pinch.

## Round 2 — integration and identity

**Q5 (instructions field).** What goes in the initialize
`instructions` paragraph (injected into agent context)?
**A: Tool-choice doctrine.** Short Latin paragraph: which tool for
which question-shape (dolor→mandatum pattern), plus the freshness
promise (answers self-revalidate; CAUTIO lines disclose costs/
staleness). Drafted at ship time, reviewed at seal.

**Q6 (LSP backport, C7 park).** Backport the tsv-mtime index reload
to LSP mode in this arc?
**A: Same arc, chunk 2.** The reload machinery is built in chunk 2
anyway; the epoch law covers both modes. The LSP suite must prove
documents hold no index pointers (sim verified; suite confirms).

**Q7 (name).** Server identity in .mcp.json?
**A: legati.** The named future. Tools: mcp__legati__symbolum /
diagnostica / vocantes / vocata.

**Q8 (protocolVersion).** Negotiation policy?
**A: Echo client's version** (if date-shaped), fall back to our
pinned latest-known. We speak only the stable tools subset.

## Round 3 — emergent questions

**Q9 (stale self).** Vigilia says the resident binary is stale —
tools refuse or answer?
**A: Answer + CAUTIO line** ("LEGATI IPSE STALUS — /mcp reconnect
renovat"). LSP precedent; refusing would disable the server during
exactly the work (editing legatus) where it is most consulted.

**Q10 (POSIX seam).** praeparatio is posix-or-not server-wide;
per-call switching = rebuild (~300ms + epoch cascade) or dual
resident praeparationes. v0 policy?
**A: Server-wide off + park.** v0 matches the LSP resident: posix
files get the known lexicon-gap noise (utimbuf class, documented).
Dual-praeparatio (posix:true answered from a second resident
praeparatio, ~1-2MB) = named park with design sketched.

**Q11 (card scope).** symbolum's composite richness?
**A: Lean card.** Sedes (body-def first) + genus + signatura +
uses-per-file + macro corpus + similitudo fallback on miss.
Callers/callees stay one call away — composability over
completeness.

**Q12 (9/9 sweep).** goToImplementation (~12-line alias)?
**A: Fold into chunk 1.** Rider on a chunk already touching the
dispatch table; closes the client operation set; one extra golden.
