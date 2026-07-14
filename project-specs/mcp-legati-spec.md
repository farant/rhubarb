# MCP LEGATI — SPEC v1

2026-07-14. Sources: the post-outgoingCalls design conversation,
instrumenta-simulatio-3.md (C1-C12), and the twelve interview
decisions (instrumenta-simulatio-3-interview.md). v2 follows after
agent verification of codebase specifics (§X lists the open facts).

## I. Quid sit

LEGATI: a name-based, pull-based MCP stdio server complementing the
positional push-based LSP. Same binary as legatus (`-mcp` mode),
resident per session (harness-managed stdio lifecycle), answering
from the warm index + praeparatio with LAZY REVALIDATION — every
answer checks its watch before speaking. The third context
instrument prophesied by root CLAUDE.md (Fran / CLAUDE.md / project
MCP server).

Demand record (why now): the LSP shakedown mapped two client-side
constraints the server cannot fix — no diagnostics-pull channel,
and didOpen scoping to session-edited files (call hierarchy dead on
merely-read files). Both are structural to the LSP client; both
vanish under MCP's model (agent asks by name; server reads disk).

## II. DECISUS (the pinned decisions)

1. ONE BINARY — legatus grows `-mcp`; no separate program. Zero
   drift between surfaces; vigilia and warm world shared.
   (LEGATUSD later adds the socket + cross-session sharing; this is
   LEGATUSD-lite.)
2. v0 IN-FILE (C11/Q1) — banner sections in legatus.c (~5,600
   lines accepted); unit split deferred to the LEGATUSD refactor.
3. PURE LATIN (Q2) — tool names, descriptions, result text,
   CAUTIO lines: all Latin. House rule; agent reads it.
4. EXAMEN PARITY (Q3) — diagnostica's text output = examen.sh's
   format exactly (verdict + positioned rows, INFRA policy
   matching the LSP publish).
5. FIXED CAPS (Q4) — nexus-style grouped output, per-file + total
   caps as constants, explicit "et alia N" tails. verbosum knob =
   named park.
6. INSTRUCTIONS = DOCTRINE (Q5) — initialize's `instructions`
   carries a short Latin tool-choice paragraph (dolor→mandatum) +
   the freshness promise. Drafted at ship, reviewed at seal.
7. LSP RELOAD BACKPORT IN-ARC (Q6/C7) — chunk 2 gives BOTH modes
   the tsv-mtime index reload. Bar: LSP suite stays green.
8. NAME: legati (Q7) — tools mcp__legati__{diagnostica, symbolum,
   vocantes, vocata}.
9. PROTOCOL: echo client's version if date-shaped, else our pin
   (Q8). Tools-subset only: initialize, notifications/initialized,
   ping, tools/list, tools/call; unknown → -32601.
10. STALE SELF: answer + CAUTIO (Q9) — "LEGATI IPSE STALUS — /mcp
    reconnect renovat" on every result while dirty; never refuse.
11. POSIX: server-wide OFF in v0 (Q10) — posix-seam files show the
    known lexicon-gap class; DUAL-PRAEPARATIO = named park (second
    resident praeparatio, posix:true per-call arg, ~1-2MB).
12. LEAN CARD (Q11) — symbolum = sedes + genus + signatura +
    uses-per-file + macro corpus + similitudo fallback. Call
    hierarchy stays in vocantes/vocata.
13. goToImplementation RIDER (Q12) — 12-line alias to the
    definition handler folded into chunk 1; client ops 9/9.

## III. Framing + handshake

- tabellarius grows TWO functions (no mode state):
  `tabellarius_lineam_legere(FILE*, Piscina*, b32* finitus)` —
  unbounded line read via ChordaAedificator, strip trailing \r,
  skip blank lines, EOF mid-line = finitus;
  `tabellarius_lineam_scribere(FILE*, chorda)` — body + '\n' +
  fflush, ASSERTS no raw '\n' in corpus (refuses with stderr clamor
  rather than corrupt the channel).
- SERIALIZATION LAW (C1): MCP path uses compact json_scribere ONLY.
  _evadere_json covers \n \r \t " \\ in string values; pretty mode
  is forbidden on this path. Golden: diagnostic message containing
  a tab round-trips escaped.
- Envelope layer (tabellarius_responsum/errorem) reused UNCHANGED.
- RADIX (C2): MCP initialize has no rootUri. legatus.sh appends
  `-radix "$RADIX_DIR"` always; principale parses it into
  LegatusConfiguratio; LSP mode's rootUri wins when present. Zero
  new POSIX calls.
- MCP initialize performs the same setup as LSP initialize (radix,
  exclusiones, praeparatio, index, vigilia), then responds
  {protocolVersion, capabilities:{tools:{}}, serverInfo:
  {name:"legati"}, instructions:<doctrine>}.

## IV. The revalidation lattice (epoch law, C6)

Three tiers, lifetimes NESTED BY POLICY; invalidation kills
downward; nothing in tier N+1 survives a tier-N event.

- TIER 1 — praeparatio (capita in piscina_capitum).
  Check: any caput mtime ≥ praeparatio timestamp → rebuild
  praeparatio AND reload index. SAFE in MCP mode because
  DOCUMENT-FREE (C5): no long-lived tree points into
  piscina_capitum; _recensere trees are ephemeral. (Recorded: a
  future documents-in-MCP feature re-arms the C11 trap.)
- TIER 2 — index (piscina_indicis: tsv rows + overlays + symbol
  cards + via-mtime table). Check: build/nexus.tsv mtime changed →
  destroy piscina_indicis wholesale + _indicem_onerare fresh
  (48 MB / ~720k rows ≈ 1s, disclosed: "index renovatus post
  commissionem"). Nothing outside the piscina points in (verified;
  LSP suite re-proves for LSP mode after backport).
- TIER 3 — per-via freshness (via → last-judged mtime, stored in
  piscina_indicis ON PURPOSE). Check inside lookups: involved via
  mtime ≥ recorded → _recensere(via).
- `_aetates_custodire` runs tiers 1-2 at request entry (~50 stats,
  <1ms); tier 3 is per-lookup.
- POLARITY (C8): revalidation compares ≥ (coeval = suspect);
  vigilia keeps strict > (coeval = fresh). Both comparison sites
  carry a comment naming the other polarity and why.

## V. _recensere — the on-demand judge (C3)

`_recensere(l, via)`: read from disk + praeparator_analysare +
_ordines_plagulae_necare + overlay row emission (the
_superpositio_recipere callback — document-free already) + extents
refresh + SYMBOL-CARD harvest (C9: depth-0 symbols get
{titulus, linea_a/b, signatura-string, genus} rendered at judge
time) + record via→mtime; trees destroyed before return.
- One judge, three consumers (diagnostics text / overlay rows /
  extents+cards), one home.
- _extenta_viae's closed-file branch REFACTORS ONTO IT (today it
  parses then discards diagnostics and rows; net code shrinks).
  This refactor is why the LSP suite is chunk 2's load-bearing bar.
- Failure honesty: syntax-broken dirty file → old rows necati, no
  fresh rows, answers carry "plagula X non analysabilis" CAUTIO.
- TWO-PASS LAW (C4): name lookups run pass 1 (collect involved
  viae + dirty check + _recensere), then RE-RUN the lookup if
  anything was judged; bounded at two passes because judged mtimes
  are recorded.

## VI. Tool surface (v0)

All results: Latin text content [{type:"text",text}], grouped
nexus-style, capped with explicit tails; CAUTIO lines for
staleness/cost/self-staleness. isError=true for tool-level failures
(via ignota, extra radicem); JSON-RPC errors for protocol failures.

1. **diagnostica** {via} — _recensere + examen-format render
   (verdict ACCIPE/REICE + positioned rows; INFRA policy = LSP
   publish parity). Warm examen.
2. **symbolum** {titulus} — lean card: sedes list (body-def policy
   first: .c primary → .c secondary → any sedes), genus, signatura
   (from symbol cards), uses-per-file counts, macro branch
   (#define line + corpus extents, v0.2 machinery); similitudo
   top-3 fallback on miss (vicinitas pattern).
3. **vocantes** {titulus} — incoming calls by name: index walk,
   revalidate only DIRTY using-viae, existing 20-file analysis
   ceiling, grouped by caller with via:linea.
4. **vocata** {titulus} — outgoing calls by name: body-def via →
   tier-3 check → extent + usus-in-extent walk (the outgoingCalls
   internals, no item/range/prepare), grouped by callee.

Via normalization everywhere: absolute → strip radix; "./" strip;
outside-radix → tool error.

## VII. Registration

.mcp.json (repo root, committed):
{"mcpServers":{"legati":{"command":"./officina/legatus.sh",
"args":["-mcp"]}}}
(-radix is appended by the script itself.) stdout hygiene already
guaranteed (stderr-only build chatter + exec).

## VIII. Chunks + bars

| chunk | contents | bar |
|---|---|---|
| 1 | tabellarius newline mode; -mcp/-radix; handshake; tools/list; ping; goToImplementation rider | golden: handshake + list over newline framing; C1 escape golden; LSP 9/9 golden |
| 2 | _recensere (three consumers + cards); epoch lattice (_aetates_custodire, via-mtime table, tsv reload BOTH modes); _extenta_viae refactor | staleness goldens (dirty-via re-judge; tsv-swap reload; utime tricks per vigilia precedent); **LSP suite green** |
| 3 | four tools + caps + similitudo fallback + instructions doctrine + .mcp.json | tool goldens ×4; live in-session shakedown (C12 list) |

## IX. Parks (named)

- verbosum/plenum knob on list tools (Q4).
- DUAL-PRAEPARATIO posix:true (Q10; design sketched in interview).
- excubitor-verdict tool (vigilia BFS generalization; bash tool
  covers today).
- sonda una-ictus (two scratchpad probe scripts = seeds; LEGATUSD
  absorbs).
- LEGATUSD: socket daemon, cross-session shared world, mtime-watch;
  unit split of legatus.c lands there (Q1).
- Nanosecond mtimes (st_mtimespec) in the posix seam if same-second
  churn ever measures.

## X. Open facts for v2 (agent verification targets)

1. Exact semantica diagnostics walk API (function names, struct
   fields, severity mapping, INFRA handling) — for diagnostica +
   examen parity.
2. Signature renderer: name, dependencies, callable from standalone
   sem without a document? — for symbol cards.
3. Macro hover machinery: callable document-free? — for symbolum's
   macro branch.
4. praeparator capita inventory (the exact file list a tier-1 mtime
   sweep must stat); PraeparatorConfiguratio fields.
5. The nexus_ordines emission entry point + necare-then-emit order
   in the existing overlay path — for _recensere.
6. _initialize_tractare's setup order — for MCP initialize parity.
7. probatio helpers (_scribe/_lege) shapes — for newline variants.

## XI. What the sim cannot answer (first-contact shakedown, C12)

tools/list fetch cadence; token weight of schemas in agent context;
whether `instructions` lands in agent context; /mcp reconnect vs
the vigilia nag; spawn cwd/env for project-scope .mcp.json;
approval-prompt UX. The shakedown list is chunk 3's live bar,
exactly as the LSP shakedown was.
