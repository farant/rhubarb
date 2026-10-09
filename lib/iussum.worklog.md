# iussum worklog

## 2026-10-08 - S3a: recognition only, known verbs only

Born for vicus-latera S3 (acme commands in scriba pages). Pure reader
over a TabulaCharacterum; no registry here - the host answers "is this
verb known?" through `IussumNotum`, an idea taken from prunifex (only
registered commands highlight or respond), which removes the need for
an escape syntax: `$5.00` and a half-typed `$foo(` stay prose.

Rules worth remembering: `$` must start the line or follow a
non-word character (`a$b`, `x_$dies` are not commands); arguments end
at the FIRST `)` on the same line (no nesting by design); an unclosed
`(` makes the whole token not-a-command rather than a command without
arguments, so half a command never runs. Empty cells are `'\0'` in
scriba pages - they count as spaces when trimming.

Gotcha: `tabula_cellula(t, l, c)` with s32 coordinates trips
-Wsign-conversion (i32 width is unsigned) - the lib goes through one
`cellula()` helper with explicit casts. And the installed `scribe`
judges against main's repo, so in a worktree it writes but answers
"via extra repositorium" (exit 4) - judge with ./silva/examen.sh.
