# annales_sedes - worklog

## 2026-10-08 - one home for the ledger data, guards before every open

Why: the ledger records the state of the whole house, but every
launcher took its data location from its own directory (`-radix
"$RADIX_DIR"`, then hard-coded relative paths in
`tabularium_principale.c`, `nota_frigida.c`, `tabulariumd_principale.c`).
In a worktree that meant the worktree's copy: the committed journal
(last committed 2026-09-01 - five weeks stale) and NO database.
`gesta_aperire` creates the journal if absent, and a fresh empty
database starts at seq 1 - so the resident (…MKMD2) or the cold path
via `portae_debitae`, which commissio now calls on every commit
(…VSY50E: 26 seeding events), appended to the worktree's journal.
Fran's friction: those writes turn into merge conflicts, because the
in-tree journal must not diverge from main.

What exists already and was kept: several writers are safe by design.
Each write is one SQLite transaction (busy_timeout 5000 in scrinium),
and the journal line is appended INSIDE it, before COMMIT - SQLite's
write lock serializes the appends across processes. Three
`tabularium -mcp` processes (one per session) were running on main's
files at the time. No daemon-as-sole-writer needed.

The module: `annales_sedem_invenire` (order: `$RHUBARB_ANNALES` - a
named directory that does not exist is a REFUSAL, never a silent fall
back; else `~/.rhubarb/annales` if it exists; else the tree, legacy
layout `gesta/annales/` + databases at the root), `annales_via` (one
name table per layout), `annales_custodire` (journal absent -> refuse
unless genesis is explicit; journal present but database absent ->
refuse and name `./gesta/frigida.sh -restituere`; database without
journal -> refuse). An EMPTY journal is a valid fresh ledger: the
guard tests existence, not size (first draft tested size and refused
its own genesis output).

Code-index paths (`build/nexus.tsv`, identitates, citationes) stay
per tree on purpose: they describe that tree's code.

nota_frigida gained three modes: `-sedes` (TSV of the resolved paths -
the one source for scripts; frigida_fumus now reads the live journal
path from it), `-restituere` (database from the journal alone, only if
absent, verified with gesta_annales_verificare), `-genesis` (empty
ledger, only in an empty location). The forum daemon resolves and
guards only its DEFAULT paths: explicit `-scrinium/-annales` (its own
fumus, apps/forum) are deliberate and bypass both.

Measured: restoring a copy of the full live journal (5055 events)
takes 1.8 s and gives exactly the live database's count and max seq -
the migration can move the journal alone and rebuild. Four concurrent
cold-path writers x 50 creates: 400 events, 0 failures, journal ==
database. Known residual risk, not observed: transactions use a
deferred BEGIN; in WAL mode a read-then-write transaction can get
SQLITE_BUSY_SNAPSHOT (not retried by the busy handler) if another
writer commits in between. If that ever shows, switch gesta's batch
writer to BEGIN IMMEDIATE.

Gate `annales` = `gesta/annales_fumus.sh` (temp locations only): I
location, II guards (nothing created on refusal), III genesis, IV two
concurrent writers (dense unique seq, journal in seq order, journal ==
acts, both writers' notes present), V restore == written, VI MCP and
forum daemon refuse, VII fresh-worktree legacy case refused with the
journal untouched. Plants (silva.planta, fumus as a callable gate):
guard admits journal-without-db -> II; silent genesis -> II; env var
ignored -> I; cold path without guard -> II; daemon without guard ->
VI. TRAP found by that last plant: an unguarded daemon SERVES and never
exits, so the fumus hung (and killing the plant job mid-run skipped
planta's revert - restored by hand from a saved copy). Every process
check now runs under a 5 s cap (`tecto`; 124 = did not refuse).
`planta` returns summary STRINGS, not Porta objects.

Next (Fran's step): move the data to ~/.rhubarb/annales (own git
repo), git rm --cached the in-repo copies + .gitignore; then drop them
from VETITAE / generata table; close …MKMD2, …VSY50E.
