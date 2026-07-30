# similitudo — worklog

## 2026-07-10 — nata (desideratum primum phasis instrumentorum)

Fuzzy subsequence matching (fzf-v1 style) for identifiers, named
by Fran same day. lib/similitudo.{c,h} + probatio_similitudo (20
asserts, green first run). Pure functions over chordae — no
piscina, no allocation, no state; greedy left-to-right scoring
(boundary +16, consecutive +8, exact-case +1, gap −3/−1). The
optimal-alignment DP upgrade is a NAMED deferral; the API
(punctare/optima) would not change.

Consumers: emitte "fortasse:" (live same day), nexus -similis
(next), saltuarius type-jump (v0.1 backlog).

**FINDING (first hour of dogfood): subsequence direction inverts
for the did-you-mean case.** fzf-style matching assumes a SHORT
fragment query inside longer candidates. But emitte's not-found
case is a wrongly-guessed FULL name (scr_signatura_computare) —
a subsequence of nothing, zero suggestions. The fix is CONSUMER
policy, not lib mechanics: shrink the query from the right until
candidates appear (what a human does in fzf by deleting chars;
floor 3 chars). Lib API stayed pure. If a second consumer needs
the same loop, promote a `similitudo_decurtans` convenience row —
not before (rule of two).

Suite note: probatio_similitudo joined the officina cursor wave
automatically (80→81) — the new lib runs INTERPRETED in machinula
as part of the standing corpus bar, stdout byte-diffed against its
pinned native capture. Cathedral perk: every new lib gets VM
coverage for free.

## 2026-07-30 — optima_decurtata promoted (rule of two fired)

The shrink-from-right did-you-mean loop (query truncated from the
right until candidates appear, floor III) was born with emitte's
"fortasse" (2026-07-10, "consumer shrinks query from right — lib
stays pure, rule of two before API promotion"), hand-copied into
nexus's fortasse, and was about to be copied a third time into
legatus's _ignotum_respondere. Promoted instead:
similitudo_optima_decurtata — same purity contract (no piscina, no
state; the by-value chorda is shrunk locally). All three consumers
migrated; behavior byte-identical to the hand loops (same floor,
same first-hit-wins). Note the floor also means a query SHORTER than
III is never attempted at all — pinned in probatio_similitudo.

## 2026-07-30 (later) — cross-level merge: the level is not a tier

The promoted decurtata's greedy stop-at-first-productive-level had a
measured flaw within hours: 'legatus_tractare' suggested LEGATUS_*
macros (matching 'legatus_t', 9 chars, case-mismatched with gaps) and
never reached legatus_currere (matching 'legatus_', 8 chars, clean).
One extra matched char of the guess's WRONG TAIL trumped every
quality signal — but the tail is the least trustworthy part of a
guess; that's why we shrink it.

Hand-measurement against the weights settled the design before any
code: currere@8 = 80, macro@9 = 67, macro@8 = 73. So (1) raw merge
suffices — case premiums + gap penalties already rank clean-short
above sloppy-long, no discard-penalty term needed; (2) a net-negative
tail match demotes ITSELF (67 < 73).

THE COLLAPSE: cumulative score at consumption k in one greedy walk
== punctare(prefix_k) exactly (deterministic walk; candidate chars
after the last consumption never score). So "merge N levels" became
"one walk per candidate, running max past the floor" — decurtata now
costs exactly what optima costs, and is FASTER than the old
loop-until-hit on misses.

Structure: _ambulare = the ONE walk body (punctare is now a wrapper;
the two scorers can never drift); _inserere = optima's insertion
extracted, shared. Hand-computed 80/73 pinned in probatio (with the
macro competitors IN the candidate pool — a natal pin without the
competitors would have passed before the fix too). Emitte bonus: the
merged pool gives 3 ranked suggestions where the level-stop gave 1.
