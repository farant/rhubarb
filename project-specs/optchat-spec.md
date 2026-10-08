# optchat spec - an endless chat whose memory is its own history

Born 2026-10-07 (brainstorm with Fran; ledger parcum …SQM6; memory
`optchat-project`). Source design: Taelin's OptChat,
gist.github.com/VictorTaelin/91837951a5ce5b38f341ec1ba1df6449, frozen
copy `project-specs/optchat-taelin-2026-10-07.md` (gist updated_at
2026-10-08T01:02:16Z - the author is still revising it; our copy is the
reference, later revisions are read and adopted deliberately).

Lives here until O0 scaffolds `../optchat`; then it moves to
`../optchat/SPEC.md`.

*This spec does NOT restate Taelin's. It says where code and data
live, which of his sections we follow verbatim, and every deviation
with its reason. A sentence of his spec that this file does not
contradict is in force.*

## 0. Data (2026-10-07)

- LLM calls go through **vates** (`project-specs/vates-spec.md`),
  built first in `../rhubarb-quarta` and consumed via silex.
- Monthly API credits (Max plan) cover API-key calls only; they expire
  each billing cycle (support.claude.com article 17154008). A
  fresh-call-per-turn harness is what they pay for.
- Prices (skill cache 2026-10-06): Opus 5.5 $4 / $20 per MTok, cache
  read $0.20; Sonnet 5.5 $2 / $10, cache read $0.20. Cache write 1.25x
  input (5 min TTL). Minimum cacheable prefix 512 tokens on both.
- Opus 5.5 cannot disable thinking (`effort` is the only control,
  default `medium`); Sonnet 5.5 can only via `between_tools`. Forced
  `tool_choice` is a 400 on both. Neither matters to OptChat: every
  turn is a fresh, append-only call with `tool_choice: auto`.

## I. Layout

| What | Where | Versioning |
|---|---|---|
| app code | `../optchat` (silex project, vates closure vendored) | decided at O0 (git and/or silex) |
| chat data | `../optchat-acta` (path given on the command line) | its own git repo, one commit per turn |
| API key | `~/.rhubarb/anthropic.clavis`, mode 0600 | none; written by Fran, never read into a transcript |

The data repo holds, beside `main/` and `tree/`: `optchat.stml`
(config), `MASTER.md`, `VIEW_DOC.md`, `COMPACT.md`, and Fran's
instructions file. Prompts are files so experiments are versioned with
the data they produced.

## II. Followed verbatim

§2 storage format and durability (one write + fsync per line, torn-line
repair, unix-socket single-writer lock) · §3 the tree (purely binary,
free nodes, NODE = 512 bytes, `id+n` addressing) · §4.1 the pump and
its order rule (rule 3) · §4.2 what a compactor call sees (context
block first, NO ids anywhere, SCALE line, whole message) · §4.3 size
enforcement (cut-at-limit feedback, TRIES = 5, keep shortest, cut on a
UTF-8 boundary) · §4.4 COMPACT prompt · §5 the view (append, then merge
the most due pair; never split; never a whole message; refold at load)
· §6 wait, don't cut (settle before every call) · §7 turn loop and
§7.1 tools (`zoom`, `date`) and §7.2 MASTER / VIEW_DOC prompts · §8
request layout and breakpoints (view marks at 50,000 / 80,000 /
100,000 characters + end of request; no 1 h TTL; no keep-alive pings)
· §11 the checklist, all fifteen items.

On-disk field names stay his (`i, kind, text, size, date`;
`l, i, text, size`; kinds `user talk tool echo note`) so our logs are
readable by any OptChat implementation and vice versa. C identifiers
are Latin; the wire format is his.

## III. Deviations

1. **Provenance on tree lines** (additive JSON fields; readers that
   ignore unknown keys are unaffected): `model`, `effort`, `prompt`
   (hash of the COMPACT text that built the node). Why: nodes are
   permanent; once two compactors have run, only provenance tells
   their lines apart (Fran: easy model experiments).
2. **JOBS = 1 in v1.** vates is blocking and the house has no threads.
   Free nodes cover short messages and many merges, so model calls per
   turn should be few - measured at O5. `JOBS > 1` arrives with
   `vates_incipere/pulsare` (vates spec §X).
3. **No mid-run user messages in v1** (same cause). Ctrl-C cancels a
   turn; the message stays in the log, unanswered (his §7 rule).
4. **Models**: master Opus 5.5, compactor Sonnet 5.5 (his tested
   choice was Sonnet at medium effort), both set in `optchat.stml`.
   Thinking is never logged (his §2); the terminal may show the
   summarized form (`display: "summarized"`, config).
5. **Refusals**: master requests carry the server-side refusal fallback
   (`fallbacks: "default"`, beta header, via vates' escape hatches),
   switchable in config. A refusal that still happens is printed and
   logged as `talk`.
6. **Compactor caching**: one breakpoint on the `<chat>` context block
   (his §8 last paragraph says to put it first and let it cache; we
   mark it explicitly).
7. **Accounting**: every call goes through the vates ledger with
   `propositum` = `magister` or `compactor`, so cost per role and per
   model is a query over `../optchat-acta/rationarium.jsonl`. A short
   usage line prints after each turn.
8. **Not in v1**: subagents and computer use (his §9), the HTML
   browser (§10), importing old sessions (§10). Named for later:
   `optchat refacere --arbor <dir> --compactor <model>` - rebuild a
   tree from the same log with another compactor, to compare views on
   real data (costs money; about two calls per message).

## IV. Modules (in `../optchat`)

| Module | Owns | Pure? |
|---|---|---|
| `acta` | the log: append + fsync, load, torn-line repair, lock | I/O |
| `arbor` | node arithmetic, addressing, free nodes, tree file | mostly pure |
| `visus` | the fold (append, most-due merge), render, cache marks | pure |
| `compactor` | the pump, one node build, size enforcement | over vates |
| `colloquium` | settle, one turn, the tool loop, `zoom`/`date` | over vates |
| `optchat` | CLI, config, prompts, per-turn commit | thin |

Each module gets a credo suite. `visus` and `arbor` are where the
promotion to rhubarb starts once they settle.

## V. Testing

- Everything runs under credo on the vates **fictus** backend, offline
  and free.
- `acta`: torn final line; missing final newline; second process
  refused by the lock; stale lock taken over.
- `arbor` / `visus`: addressing round-trips; free-node boundaries at
  511 / 512 / 513 bytes, multi-byte characters at the cut; never-split;
  budget held; **replay**: fold a long synthetic chat (thousands of
  messages, deterministic fake summaries of realistic sizes) and
  measure the shared prefix of consecutive views - his §5.4 numbers
  are the reference.
- `compactor`: rule 3 order; a scripted model that overshoots then
  shrinks (TRIES, shortest kept); retry after failure; NO ids in any
  compactor request (asserted on fictus' recorded requests).
- `colloquium`: settle waits; view rendered before the new message is
  logged; tool loop logs `tool` + `echo`; `zoom` contract (powers of
  two, bounds, "No line id+n."); breakpoints where §8 says.
- Every behaviour born red by a plant (house rule).

## VI. Build order

- **O0** `silex novum` with the vates closure; `git init
  ../optchat-acta`; Fran writes the key file; move this spec.
- **O1** `acta` · **O2** `arbor` + `visus` · **O3** `compactor` ·
  **O4** `colloquium` + CLI on fictus, then live.
- **O5** first real conversation, then a debrief with measurements:
  compactor calls per turn, cache-read share per turn (master and
  compactor), $ per turn, settle latency, view size in bytes and
  tokens.

## VII. Done means (v1)

A conversation of several days in which optchat recalls, by zooming,
something said on day one that no longer appears in the view at full
resolution - with the costs from O5 known, not guessed.

## AUDIENDA

- **The agent's name in the prompts.** His prompts say "OptChat" and
  ask implementers to rename. Kept as "OptChat" until Fran names it.
- **Sonnet 5.5 safeguards on the compactor.** His §2 measured
  `reasoning_extraction` refusals when thoughts were logged; we do not
  log thoughts, but Sonnet 5.5 has five refusal categories. A compactor
  refusal fails the node (retried forever); watch the ledger for
  `causa_finis = RECUSATIO` with `propositum = compactor`.
- **Token size of the view on current tokenizers.** His "128 KB ~
  62-64k tokens" was measured on "Opus-class tokenizers"; ours is
  measured at O5 from `usage`.
- **Whether `../optchat` versions with git or silex** - O0.
