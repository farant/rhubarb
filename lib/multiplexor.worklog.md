# multiplexor worklog

## 2026-10-15 - T1: tmux through its command line (colloquia-plan)

Header approved by Fran. Pure parsing of our own `-F` format (fields
separated by TAB, which tmux passes through literally; window titles
and paths keep their spaces) is separate from execution, so the
fixtures under probationes/fixa/multiplexor/ (synthetic - never Fran's
real sessions) test it. A line with the wrong field count or a
non-numeric number is skipped; empty output is an empty Xar, not NIHIL
(NIHIL means "could not ask").

Execution goes through `processus` (argument vector, no shell). Two
details beyond the header text, reported to Fran:
- `socket` containing '/' is passed as `-S <path>` (otherwise `-L
  <name>`): the live test puts its PRIVATE server's socket in
  $TMPDIR/probatio_multiplexor.<pid> - the place root tests may write -
  and kills it and deletes the socket in `purgare` (always runs).
  Fran's own server is never touched by the suite.
- Sessions are targeted as `=name`: plain `-t alpha` matches by
  PREFIX in tmux (`alp` finds `alpha`, `2` would find `20`). The live
  test asserts the prefix does not count.

Binary: given -> trusted (a missing one fails at exec, so `panelli`
returns NIHIL); NIHIL -> PATH, then /opt/homebrew/bin, /usr/local/bin,
/usr/bin (vicus launched from the Dock has a short PATH).

Test gotcha: `split-window -d` keeps the ORIGINAL pane active; the
first live run assumed the new one.

Plants red: pid not validated, active flag inverted, always -L,
session not exact, select-window skipped, exit code ignored. After the
-L plant, /tmp/tmux-501 was checked: no stray server.

Gate finding (first commit refused): under fabrica the root test's
TMPDIR is deep in the build area (<repo>/build/fabrica/area/
probationes_radicis/probatio_multiplexor/tmp/), and the socket path
reached ~144 bytes - over macOS's 104-byte sun_path, so tmux could not
create its server (passed in a plain shell, red in the gate). The live
test now uses <RHUBARB_RADIX or cwd>/build/pmx/s.<pid> (~72 bytes here,
~88 in a frigida checkout), declared as <debitum_scripturae> in
aedificatio.stml.

Second gate finding (first fix still red: server up, ZERO panes): the
gate runs tests with a bare environment (no LANG/LC_*). Without a
UTF-8 locale tmux SANITIZES control characters in `-F` output - our
TAB separators came out as '_' (`%0_alpha_zsh`), so every line had one
field and was skipped. Vicus launched from the Dock can have no locale
either, so this was a real bug, not a test artefact. Fix: always pass
`-u` (force UTF-8): tabs and UTF-8 names ("café") survive with no
locale (checked by hand with `env -i`). The live test now unsets LANG,
LC_ALL and LC_CTYPE in parare so a plain shell run reproduces the gate;
plant "no -u" red. Debugging note: zsh does not word-split "$E" in
`$E cmd` - use ${=E}.
