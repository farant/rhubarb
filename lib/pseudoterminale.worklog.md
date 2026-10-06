# pseudoterminale worklog

Library: a child process on a pseudo-terminal. Header
`include/pseudoterminale.h` (Fran approved 2026-10-06, decisions 15-16
in `project-specs/aemulator-plan.md`); posix bridge
`lib/pseudoterminale_posix.c`, portable part + memoriae bridge
`lib/pseudoterminale.c`. Design notes: terminal-planning module 008.

## 2026-10-06 — B3: built

**One header, two objects.** aedilis maps one header to one object by
convention (`X.h` -> `lib/X_macos`, `lib/X_posix`, then `lib/X.c` - the
first found wins). `pseudoterminale.h` resolves to
`pseudoterminale_posix.c`; the header's `<aedilis corpus=
"lib/pseudoterminale.c"/>` annotation ADDS the portable object
(configuration defaults, error names, the memoriae bridge). The
annotation composes with the convention, it does not replace it.

**/bin/sh hides a missing TIOCSCTTY (plant P2 survived at first).**
The first controlling-terminal test ran `sh -c 'printf x >/dev/tty'`.
With the `TIOCSCTTY` ioctl removed it still passed: `/bin/sh` acquires
a controlling terminal by itself at startup. A direct probe (a helper
that opens /dev/tty, exec'd with no shell) showed the truth: without
the ioctl, open fails with ENXIO ("Device not configured") and
tcgetpgrp is -1. The test now execs `/bin/stty -f /dev/tty size`
directly (no shell): "24 80" with the ioctl, an error without. Rule:
a test of the child's environment must not go through a shell that
repairs it. `setsid` (P1) is caught by the same check.

**"Lowest free fd before == after" is not a leak check (P16
survived).** Leaving the slave open in the parent leaked one fd per
child, but descriptors above a free hole are invisible to "lowest free
fd". The test now counts every open descriptor 0..255 before and
after.

**Non-blocking writes, measured.** A raw-mode child that never reads
accepts 1022 bytes, then `write` returns EAGAIN -> `scribere` returns
0. A canonical-mode child accepts EVERYTHING (macOS line discipline
discards past MAX_INPUT and rings the bell - IMAXBEL), so the
partial-write test uses `stty raw -echo`. The hang guard
(CREDO_NON_PENDET, forked) runs first; only if nothing hung does the
parent count the accepted bytes (assertions inside the forked guard do
not reach the parent's tally).

**Signals.** Dispositions set to SIG_IGN survive exec; the child resets
HUP INT QUIT PIPE ALRM TERM CHLD TSTP TTIN TTOU WINCH to SIG_DFL (as
Ghostty does). Test: the host ignores SIGPIPE, the child's
`kill -PIPE $$` must kill it (plant P3).

**claudere.** SIGHUP to the group and the child (only before reaping -
an unreaped pid cannot be reused), close the master, poll waitpid
WNOHANG 20 x 5 ms, then SIGKILL + blocking waitpid. A child that
ignores SIGHUP is reaped through the SIGKILL path (P7).

**Exec failure.** CLOEXEC pipe, as processus: the child writes
{step, errno} (chdir or exec) and `_exit(127)`; the parent reads until
EOF (exec succeeded) or a record (failure -> reap, NIHIL +
PSEUDOTERMINALE_ERROR_EXEC). Leaving the write end without CLOEXEC
makes `creare` wait until the child exits - plant P4 hangs the suite
(caught by the plant runner's timeout).

**Lexicon (examen) learned:** util.h `openpty`; `TIOCSWINSZ`,
`TIOCSCTTY`; `SIGHUP`, `SIGQUIT`, `SIGCHLD`, `SIGTTIN`, `SIGTTOU`;
`EIO`; `chdir`. auspex_posix asserts the macro values against the real
headers (plant: a wrong TIOCSWINSZ -> DISSENTIT). The new API names
went into the glossary as ignotum-permissum, `pseudoterminale` as a
neuter noun (three forms); oratio stayed 19/19 (no pins moved).

**Lint index was stale.** The first lint run said "nihil novi"; after
`./silva/nexus.sh pseudoterminale_posix_creare` it found 12 new words.
Refresh the index before trusting a clean lint on new files.
