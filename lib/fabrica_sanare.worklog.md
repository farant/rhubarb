
## 2026-10-31 — install journal explains foreign ~/.bin writes (park …5NQBP, Fran option a)

Problem: in a multi-worktree setup, main's post-merge `sanare installata`
rewrites ~/.bin while another tree's gates run. Fabrica's before/after
photograph then sees e.g. ~/.bin/briar-spectator change during a radix
member and fails the member: "scripsit extra vestigium" (2026-10-30
merge commit). Fabrica cannot know who wrote a path.

Fix: `tools/instituere.sh` (the only installer; refuses in worktrees
since 10-24) appends `<epoch>\t~/.bin/<name>` to
~/.rhubarb/institutiones.tsv, once before rm and once after cp. Two
lines are needed so that both read windows below cover the install.
Fabrica reads the journal right after the "before" photograph and again
after the action. Lines appended in between are installs that happened
DURING the action. `_extra_institutionibus_explicare` drops `~/` paths
those lines explain (exact path, or `path.`/`path/` beneath it, e.g.
.dSYM). It reports them through the new `FabricaSutura.monere` hook
(the real tool prints `fabrica: MONITUM ...`; fakes may leave it NIHIL).
Anything unexplained stays FRACTA. No clock is involved: the order of
the two reads defines the window. The journal path itself is treated as
explained when it grew, because ~/.rhubarb is photographed when an
action declares an output there (the fake disk photographs everything,
and that is how the test found it).

Safety rules:
- Only `~/` paths can be explained, never tree paths (plant: dropping
  that check turns case IV red).
- The journal must grow by appending (prefix check); a rotated journal
  explains nothing.
- instituere keeps the journal under 32 KB (tail 200), since fabrica
  reads it twice per action.

Wired at all three photograph sites: serial members (`_membrum_agere`),
plain actions (`_actionem_agere`) and parallel waves
(`_scripturas_ignotas`). Not covered: the 2026-10-30 main-heal race
where something wrote silva/build/censor INSIDE main's tree during
briar's build. That is a tree-local write, not an install.
